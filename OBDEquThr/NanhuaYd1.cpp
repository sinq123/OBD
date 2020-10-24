#include "StdAfx.h"
#include "NanhuaYd1.h"

#include <cassert>
#include <cstdio>
#include <string>

CNanhuaYd1::CNanhuaYd1(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}

CNanhuaYd1::~CNanhuaYd1(void)
{
	::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaYd1::Open(BYTE bPort)
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
	// �趨������Ϊ2400 bps
	//dcb.BaudRate = CBR_2400;
	dcb.BaudRate = CBR_2400; //change for qin
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨һ��ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	//dcb.StopBits = TWOSTOPBITS;//change for qin 
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ���ã�700����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 1000;
	CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 700;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return SMO_OPEN_SUCCEED;
}

bool CNanhuaYd1::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

// �رմ���
DWORD CNanhuaYd1::Close(void)
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

DWORD CNanhuaYd1::WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const DWORD dwSleepBeforeRead)
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

BYTE CNanhuaYd1::CheckSum(BYTE *const pb, const UINT un)
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

DWORD CNanhuaYd1::AutoAdj(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9B);
	BYTE bReadBuf(0);
	if (IsOpen())
	{
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
                 *pbResult = 0x06;
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

DWORD CNanhuaYd1::Reset(BYTE *const pbResult)
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
		Sleep(200);
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

DWORD CNanhuaYd1::StartAutoTest(BYTE *const pbResult)
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
DWORD CNanhuaYd1::AutoReadData(BYTE *const pbResult)
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

DWORD CNanhuaYd1::GetData1(BYTE *const pbResult, float *const pfData1)
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
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4)
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

DWORD CNanhuaYd1::GetTestState(BYTE *const pbResult, BYTE *const pbState)
{
	assert(NULL != pbResult);
	assert(NULL != pbState);

	//DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	//BYTE bWriteBuf(0);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		//unsigned char ReadChar;
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1== dwBytesRead)
		{
			if(bReadBuf == 0x93)
			{
				//��һ�μ�����
				*pbState = 0x93;
			}
			else if(bReadBuf == 0x95)
			{
				//�ڶ������μ�����
				*pbState = 0x95;
			}		
			else if(bReadBuf == 0x94)
			{
				//���Ĵμ�����
				*pbState = 0x94;
			}
			else
			{
				// ��������
				*pbState = 0xFF;
			}

			*pbResult = 0x00;
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			*pbState = 0xFF;
			// ����ִ�ж�ȡָ������ʧ��
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		*pbState = 0xFF;
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::StartRealTest(BYTE *const pbResult)
{
	//assert(NULL != pbResult);

	BYTE bWriteBuf(0x9e);
	DWORD dwBytesWritten(0);
	BYTE bReadBuf(0);
	DWORD dwBytesRead(0);

	if(IsOpen())
	{
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			//if (0x06 == bReadBuf)
			//{
			//	*pbResult = 0x06;
			//}
			//else if (0x15 == bReadBuf)
			//{
			//	*pbResult = 0x15;
			//}
			//else
			//{
			//	// �����ϲ�����ִ�е�����
			//}
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
			// ����ִ�ж�ȡָ������ʧ��
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

DWORD CNanhuaYd1::AllowTest(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	BYTE bWriteBuf(0x90);
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
			//if (0x06 == bReadBuf)
			//{
			//	*pbResult = 0x06;
			//}
			//else if (0x15 == bReadBuf)
			//{
			//	*pbResult = 0x15;
			//}
			//else
			//{
			//	// �����ϲ�����ִ�е�����
			//}
			*pbResult = bReadBuf;

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
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::GetSign(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	//DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	//BYTE bWriteBuf(0);
	BYTE bReadBuf(0);

	if(IsOpen())
	{
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			//if (bReadBuf == 0x94)
			//{
			//	bReadBuf = 0x94;
			//}
			//else if(bReadBuf == 0x9F)
			//{
			//	bReadBuf = 0x9F;
			//}
			//else
			//{
			//	bReadBuf = 0xFF;
			//}
			*pbResult = bReadBuf;

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
		// ����ִ�д��󣺴���δ��
		return SMO_WR_PORT_NOT_OPEN;
	}
}
DWORD CNanhuaYd1::QuitAutoTest(BYTE *const pbResult)
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

DWORD CNanhuaYd1::HardReset(BYTE *const pbResult)
{
	//assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9D);
	BYTE bReadBuf(0);

	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
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