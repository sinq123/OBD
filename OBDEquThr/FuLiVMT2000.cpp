#include "StdAfx.h"
#include "FuLiVMT2000.h"

#include <cassert>
#include <cstdio>
#include <string>

CFuLiVMT2000::CFuLiVMT2000(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_csComm);
}

CFuLiVMT2000::~CFuLiVMT2000(void)
{
	Close();

	::DeleteCriticalSection(&m_csComm);
}

// �򿪴���
DWORD CFuLiVMT2000::Open(BYTE bPort)
{
	// �ر��Ѵ��ڵĴ��ھ��
	Close();

	// ���봮�ڲ����ٽ���
	::EnterCriticalSection(&m_csComm);

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
		OutputDebugStringW(L"Error:fail to open com port\n");

		// �뿪���ڲ����ٽ���
		::LeaveCriticalSection(&m_csComm);
		// �򿪴���ʧ��
		return TAC_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��������������������С
	SetupComm(m_hComm, 512, 256);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨������Ϊ19200 bps
	dcb.BaudRate = CBR_19200;
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨һ��ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ���ã�200����֮��Ҫִ�����д������
	// ע��,���ܹ�����CommTimeouts.ReadTotalTimeoutConstant = 100;��Ϊ�Ƚ����ļ����,���ᷢ����ʱ��
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 200;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	// �뿪���ڲ����ٽ���
	::LeaveCriticalSection(&m_csComm);
	// �򿪴��ڳɹ�
	return TAC_OPEN_SUCCEED;
}

bool CFuLiVMT2000::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

// �رմ���
DWORD CFuLiVMT2000::Close(void)
{
	if (IsOpen())
	{
		// ���봮�ڲ����ٽ���
		::EnterCriticalSection(&m_csComm);

		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;

		// �뿪���ڲ����ٽ���
		::LeaveCriticalSection(&m_csComm);

		if (b)
		{
			// �رմ��ڳɹ�
			return TAC_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			return TAC_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
		return TAC_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

DWORD CFuLiVMT2000::GetRev(int *pnRev)
{
	assert(NULL != pnRev);

	// ��鴮���Ƿ��
	if (!IsOpen())
	{
		// д��ʧ�ܣ�����δ��
		return TAC_WR_PORT_NOT_OPEN;
	}
		
	DWORD dw(0);
	BYTE bWriteBuffer[9] = {0x4E, 0x54, 0x30, 0xFF, 0x53, 0x03, 0x01, 0x80, 0x7B};
	BYTE pbReadBuffer[50] = {0};
	DWORD dwReadLength = 50;
	dw = WriteAndReadData(bWriteBuffer, 9, pbReadBuffer, dwReadLength, true, true);


	if (TAC_WR_SUCCEED == dw)
	{
		if (dwReadLength>6 && pbReadBuffer[4]==0x41)
		{
			if (pnRev != NULL)
			{
				*pnRev = MAKEWORD(pbReadBuffer[9], pbReadBuffer[8]);
			}
		}
		else
		{
			return TAC_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	return dw;
}

DWORD CFuLiVMT2000::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD &dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
{
	assert(NULL!=pbWriteBuffer);
	assert(1UL<dwWriteLength);
	assert(NULL!=pbReadBuffer);
	assert(1UL<dwReadLength);

	::EnterCriticalSection(&m_csComm);
	if(IsOpen())
	{
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer+5, dwWriteLength-6);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		dwReadLength = 0;
		// �ֶ��ζ�������һ������ͷ��6byte�����ڶ��ζ�ʣ�µ���Ч����
		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 6, &dwBytesRead, NULL);
		if (bSucceed && 6UL==dwBytesRead)
		{
			dwReadLength = dwBytesRead;
			// �жϷǷ������ָ���
			if(memcmp(pbReadBuffer, pbWriteBuffer, 4) == 0)	// �Ƚϰ�ͷ
			{
				// �����µ�����
				bSucceed = ReadFile(m_hComm, pbReadBuffer+6, pbReadBuffer[5], &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(pbReadBuffer[5]))
				{
					dwReadLength += dwBytesRead;
					if (bNeedReadChkSum)
					{
						// ��Ҫ��У��
						if (pbReadBuffer[6+dwBytesRead-1] != CheckSum(pbReadBuffer+5, dwBytesRead))
						{
							::LeaveCriticalSection(&m_csComm);
							// ��������У���������ʧ��
							return TAC_WR_CHECK_SUM_FAIL;
						}
						else
						{
							::LeaveCriticalSection(&m_csComm);
							// �����ɹ�
							return TAC_WR_SUCCEED;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_csComm);
						// �����ɹ�
						return TAC_WR_SUCCEED;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_csComm);
					// �������󣺶�ָ������ʧ�ܣ���������Ч����ʧ�ܣ�
					return TAC_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&m_csComm);
				// ���������������ش���״̬��
				return TAC_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_csComm);
			// �������󣺶�ָ������ʧ�ܣ�������ͷʧ�ܣ�
			return TAC_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_csComm);
		// ���󣺴���û�д�
		return TAC_WR_PORT_NOT_OPEN;
	}
}

BYTE CFuLiVMT2000::CheckSum(BYTE *pb, UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	bSum = ~bSum;
	return bSum;                

}
