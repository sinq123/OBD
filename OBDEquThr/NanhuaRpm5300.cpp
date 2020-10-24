#include "StdAfx.h"
#include "NanhuaRpm5300.h"

#include <cassert>
#include <cstdio>
#include <string>

CNanhuaRpm5300::CNanhuaRpm5300(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_csComm);
}

CNanhuaRpm5300::~CNanhuaRpm5300(void)
{
	Close();

	::DeleteCriticalSection(&m_csComm);
}

// �򿪴���
DWORD CNanhuaRpm5300::Open(BYTE bPort)
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
	// �趨������Ϊ9600 bps
	dcb.BaudRate = CBR_9600;
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

bool CNanhuaRpm5300::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

// �رմ���
DWORD CNanhuaRpm5300::Close(void)
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

DWORD CNanhuaRpm5300::GetRev(int *pnRev)
{
	assert(NULL != pnRev);

	// ��鴮���Ƿ��
	if (!IsOpen())
	{
		// д��ʧ�ܣ�����δ��
		return TAC_WR_PORT_NOT_OPEN;
	}

	// ���봮�ڲ����ٽ���
	::EnterCriticalSection(&m_csComm);

	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	//const int nBufSize(6);
	//DWORD dwBytesRead(0);
	//char *pch = new char[nBufSize]();
	//const int nTimeOut(2);
	//BYTE bReadBuffer('\0');
	//int i(0);
	//int j(0);

	//do
	//{
	//	BOOL b = ReadFile(m_hComm, &bReadBuffer, 1, &dwBytesRead, NULL);
	//	if (b && 1UL==dwBytesRead)
	//	{
	//		pch[i++] = bReadBuffer;

	//		if (5 == i)
	//		{
	//			if (0x0D != bReadBuffer)
	//			{
	//				ZeroMemory(pch, nBufSize*sizeof(char));
	//			}

	//			break;
	//		}

	//		if (0x0D==bReadBuffer&& 5!=i)
	//		{
	//			i = 0;
	//			j++;
	//			if (j >= nTimeOut)
	//			{
	//				ZeroMemory(pch, nBufSize*sizeof(char));
	//				break;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		// �����ݿ�ȡ���˳�
	//		ZeroMemory(pch, nBufSize*sizeof(char));
	//		break;
	//	}
	//} while (1);

	//BYTE bReadBuffer[6];
	//ZeroMemory(bReadBuffer, sizeof(bReadBuffer));
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////begin
	// �����������RPM5300ȡת������
	// ������³�ʱ���ã��ڳ�ʼ���׶Ρ������ݷ��ؽ׶ε������ʱ240ms,������ת�����ݷ��ؽ׶���ʱ���97ms������0ms

	// ���г�ʱ���ã�200����֮��Ҫִ�����д������
	// COMMTIMEOUTS CommTimeouts;
	// CommTimeouts.ReadIntervalTimeout = 0;
	// CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	// CommTimeouts.ReadTotalTimeoutConstant = 200;
	// CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	// CommTimeouts.WriteTotalTimeoutConstant = 100;
	// SetCommTimeouts(m_hComm, &CommTimeouts);

	DWORD dw(0);

	const DWORD dwBytesToRead(5);
	const int nBufSize(6);
	DWORD dwBytesRead(0);
	char *pch = new char[nBufSize]();
	BOOL b = ReadFile(m_hComm, pch, dwBytesToRead, &dwBytesRead, NULL);

	// �뿪���ڲ����ٽ���
	::LeaveCriticalSection(&m_csComm);

	if (b && dwBytesToRead==dwBytesRead && 0x0D==pch[dwBytesToRead-1])
	{
		std::string strTmp = pch;
		strTmp = strTmp.substr(0, 4);
		*pnRev = atoi(strTmp.c_str());

		// д���ɹ�
		dw = TAC_WR_SUCCEED;
	}
	else
	{
		*pnRev = 0;

		// д��ʧ�ܣ���ָ������ʧ��
		dw = TAC_WR_READ_SPECIFIED_DATA_FAIL;
	}
	delete[] pch;
	pch = NULL;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////end

	return dw;
}