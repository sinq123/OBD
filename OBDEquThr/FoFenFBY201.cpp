#include "StdAfx.h"
#include "FoFenFBY201.h"

#include <cassert>
#include <cstdio>
#include <string>

CFoFenFBY201::CFoFenFBY201(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}

CFoFenFBY201::~CFoFenFBY201(void)
{
	::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CFoFenFBY201::Open(BYTE bPort)
{
	assert(0 != bPort);

	// �ر��Ѵ��ڵĴ��ھ��
	Close();

	wchar_t *pwchPort = new wchar_t[20]();
	swprintf_s(pwchPort, 20, L"\\\\.\\COM%hhu", bPort);
	m_hComm = CreateFileW(pwchPort,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	delete[] pwchPort;
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		// �򿪴���ʧ��
		OutputDebugStringW(L"Error:fail to open com port\n");
		return SMO_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��������������������С
	SetupComm(m_hComm, 512, 256);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨������Ϊ9600 bps
	dcb.BaudRate = CBR_9600; 
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨2λֹͣλ
	dcb.StopBits = TWOSTOPBITS;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ���ã�500����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return SMO_OPEN_SUCCEED;
}

bool CFoFenFBY201::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

// �رմ���
DWORD CFoFenFBY201::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// �رմ��ڳɹ�
			return SMO_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			return SMO_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
		return SMO_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

DWORD CFoFenFBY201::WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const DWORD dwSleepBeforeRead)
{
	assert(NULL!=pbWriteBuffer && 0UL<dwWriteLength);
	assert(NULL!=pbReadBuffer && 0UL<dwReadLength);
	assert(dwSleepBeforeRead < 1000UL);

	::EnterCriticalSection(&cInfoSection);

	if(IsOpen())
	{

		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		if(dwSleepBeforeRead>0)
		{
			Sleep(dwSleepBeforeRead);
		}
		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			// �жϷǷ������ָ���
			if(0x15 != pbReadBuffer[0])
			{
				::LeaveCriticalSection(&cInfoSection);
				return 7UL;
			}
			else
			{
				::LeaveCriticalSection(&cInfoSection);
				// �����������ش���״̬��
				return 6UL;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// ���󣺶���һ���ֽ�����ʧ��
			return 3UL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// ���󣺴���û�д�
		return SMO_WR_PORT_NOT_OPEN;
	}
}

BYTE CFoFenFBY201::CheckSum(BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	bSum = ~bSum + 1;
	return bSum;                

}

DWORD CFoFenFBY201::AutoAdj(BYTE *const pbResult)
{
	assert(NULL != pbResult);
	// ����ִ�гɹ�
	return SMO_WR_SUCCEED;
}

DWORD CFoFenFBY201::Reset(BYTE *const pbResult)
{
	//assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9A);
	BYTE bReadBuf(0);

	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(500);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			if (NULL != pbResult)
			{
				*pbResult = bReadBuf;
			}

			// ����ִ�гɹ�
			return SMO_WR_SUCCEED;
		}
		else
		{
			if (NULL != pbResult)
			{
				*pbResult = 0xFF;
			}
			// ����ִ�д��󣺶�ָ�����ݴ���
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			*pbResult = 0xFF;
		}
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::StartAutoTest(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x92);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		// �建����
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);	
		if (bSucceed && 1==dwBytesRead)
		{
			if (0x06 == bReadBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bReadBuf)
			{
				*pbResult = 0x15;
			}
			else
			{
				// �����ϲ�����ִ�е�����
			}

			// ����ִ�гɹ�
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// ����ִ�д��󣺶�ָ�����ݴ���
			return 2UL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}
DWORD CFoFenFBY201::AutoReadData(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	BYTE bWriteBuf(0x9c);

	if(IsOpen())
	{		
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		bSucceed = WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(500);
		if (bSucceed && 1==dwBytesWritten)
		{
			if (0x06 == bWriteBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bWriteBuf)
			{
				*pbResult = 0x15;
			}
			else
			{
				if(0x94 == bWriteBuf)
				{
					*pbResult = 0x94; //������
				}
			}

			// ����ִ�гɹ�
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// ����ִ�д��󣺶�ָ�����ݴ���
			return 2UL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::GetData1(BYTE *const pbResult, float *const pfData1)
{
	assert(NULL != pbResult);
	assert(NULL != pfData1);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x96);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		//UINT unReadCount(0);
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(500);
		do
		{
			bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
			if (bSucceed && 1==dwBytesRead)
			{
				//ReadBuf[ReadCount] = bReadBuf;
				//unReadCount += dwBytesRead;	
				str.push_back(bReadBuf);
			}
			else
			{
				// ����ִ�ж�ȡָ������ʧ��
				*pbResult = 0xFF;
				*pfData1 = 0.0f;
				return SMO_WR_READ_SPECIFIED_DATA_FAIL;
			}
		} while(bReadBuf!=0xFF);

		//str = str.substr(1, 4);
		if(str.length() >= 4)
		{
			*pfData1 = (float)atof(str.substr(1, 4).c_str());
		}
		else
		{
			*pfData1 = 0.0f;
		}

		*pbResult = 0x00;
		return 0x00;
	}
	else
	{
		// ����ִ�д��󣺴���δ��
		*pbResult = 0xFF;
		*pfData1 = 0.0f;
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4)
{
	assert(NULL != pbResult);
	assert(NULL != pfData1);
	assert(NULL != pfData2);
	assert(NULL != pfData3);
	assert(NULL != pfData4);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x97);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		//int ReadCount = 0;
		//BOOL nResult = 0; 
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(500);
		do
		{
			bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
			if (bSucceed && 1==dwBytesRead)
			{
				//ReadBuf[ReadCount] = bReadBuf;
				//unReadCount += dwBytesRead;	
				str.push_back(bReadBuf);
			}
			else
			{
				// ����ִ�ж�ȡָ������ʧ��
				*pbResult = 0xFF;
				*pfData1 = 0.0f;
				*pfData2 = 0.0f;
				*pfData3 = 0.0f;
				*pfData4 = 0.0f;
				return SMO_WR_READ_SPECIFIED_DATA_FAIL;
			}
		} while(bReadBuf!=0xFF);

		if(str.length() >= 16)
		{
			*pfData1 = (float)atof(str.substr(1, 4).c_str());
			*pfData2 = (float)atof(str.substr(5, 4).c_str());
			*pfData3 = (float)atof(str.substr(9, 4).c_str());
			*pfData4 = (float)atof(str.substr(13, 4).c_str());
		}
		else
		{
			*pfData1 = 0.0f;
			*pfData2 = 0.0f;
			*pfData3 = 0.0f;
			*pfData4 = 0.0f;
		}

		*pbResult = 0x00;
		return 0x00;

	}
	else
	{
		*pbResult = 0xFF;
		*pfData1 = 0.0f;
		*pfData2 = 0.0f;
		*pfData3 = 0.0f;
		*pfData4 = 0.0f;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::GetTestState(BYTE *const pbResult, BYTE *const pbState)
{
	assert(NULL != pbResult);
	assert(NULL != pbState);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0xA0);
	BYTE bReadBuf[3] = {0};
	BOOL bSucceed(FALSE);

	if(IsOpen())
	{	
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(200);
		bSucceed = ReadFile(m_hComm, &bReadBuf[0], 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			if (bReadBuf[0] == 0x02)
			{
				bSucceed = ReadFile(m_hComm, &bReadBuf[1], 2, &dwBytesRead, NULL);
				if (bSucceed && 2==dwBytesRead)
				{
					if (bReadBuf[1] == 0x03)
					{
						*pbState = 0x41;
					}
					return SMO_WR_SUCCEED;
				}
				else
				{
					// ����ִ�ж�ȡָ������ʧ��
					*pbState = 0xFF;
					*pbResult = 0xFF;
					return SMO_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}	
			else
			{
				*pbState = bReadBuf[0];
			}
			return SMO_WR_SUCCEED;
		}
		else
		{
			// ����ִ�ж�ȡָ������ʧ��
			*pbState = 0xFF;
			*pbResult = 0xFF;
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbState = 0xFF;
		*pbResult = 0xFF;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::StartRealTest(BYTE *const pbResult)
{
	if (NULL != pbResult)
	{
		*pbResult = 0x06;
	}
	// ����ִ�гɹ�
	return SMO_WR_SUCCEED;
}

DWORD CFoFenFBY201::AllowTest(BYTE *const pbResult)
{
	return SMO_WR_SUCCEED;

	assert(NULL != pbResult);
	// ����һ�β���
	BYTE bWriteBuf(0x88);
	DWORD dwBytesWritten(0);
	BYTE bReadBuf(0);
	DWORD dwBytesRead(0);

	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			if (0x88 == bReadBuf)
			{
				// ����ִ�гɹ�
				return SMO_WR_SUCCEED;
			}
			else
			{
				*pbResult = 0xFF;
				// ����ִ�ж�ȡָ������ʧ��
				return SMO_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			*pbResult = 0xFF;
			// ����ִ�ж�ȡָ������ʧ��
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::GetSign(BYTE *const pbResult)
{
	assert(NULL != pbResult);
	BYTE bResult(0);
	BYTE bState(0);
	GetTestState(&bResult, &bState);
	if(bState == 0x41)
	{
		*pbResult = 0x94;
		return SMO_WR_SUCCEED;
	}
	// ����ִ�д��󣺴���δ��
	return SMO_WR_PORT_NOT_OPEN;
}

DWORD CFoFenFBY201::QuitAutoTest(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9d);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		// �建����
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);	
		if (bSucceed && 1==dwBytesRead)
		{
			if (0x06 == bReadBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bReadBuf)
			{
				*pbResult = 0x15;
			}
			else
			{
				// �����ϲ�����ִ�е�����
			}

			// ����ִ�гɹ�
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// ����ִ�д��󣺶�ָ�����ݴ���
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CFoFenFBY201::HardReset(BYTE *const pbResult)
{
	return SMO_WR_EQUIP_RETURE_0X15;
}