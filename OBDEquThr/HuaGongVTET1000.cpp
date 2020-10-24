#include "StdAfx.h"
#include "HuaGongVTET1000.h"

#include <cassert>
#include <cstdio>
#include <string>

CHuaGongVTET1000::CHuaGongVTET1000(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_csComm);
}

CHuaGongVTET1000::~CHuaGongVTET1000(void)
{
	Close();

	::DeleteCriticalSection(&m_csComm);
}

// �򿪴���
DWORD CHuaGongVTET1000::Open(BYTE bPort)
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
	// �趨������Ϊ38400 bps
	dcb.BaudRate = CBR_38400;
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
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	// �뿪���ڲ����ٽ���
	::LeaveCriticalSection(&m_csComm);
	// �򿪴��ڳɹ�
	return TAC_OPEN_SUCCEED;
}

bool CHuaGongVTET1000::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

// �رմ���
DWORD CHuaGongVTET1000::Close(void)
{
	if (IsOpen())
	{
		// �建����
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
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

DWORD CHuaGongVTET1000::GetRev(int *pnRev)
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

	DWORD dw(0);

	const DWORD dwBytesToRead(14);	
	DWORD dwBytesRead(0);
	char szBuf[128] = {0};
	BOOL b = ReadFile(m_hComm, szBuf, dwBytesToRead, &dwBytesRead, NULL);

	// �뿪���ڲ����ٽ���
	::LeaveCriticalSection(&m_csComm);

	//���� "fdjzsj:0000rpm";
	if (b && dwBytesToRead==dwBytesRead)
	{
		std::string strTmp = szBuf;
		if (strTmp.substr(0, 7) == "fdjzsj:")
		{
			*pnRev = atoi(strTmp.substr(7, 4).c_str());

			// д���ɹ�
			dw = TAC_WR_SUCCEED;
		}
		else
		{
			*pnRev = 0;
			// д��ʧ�ܣ���ָ������ʧ��
			dw = TAC_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		// д��ʧ�ܣ���ָ������ʧ��
		dw = TAC_WR_READ_SPECIFIED_DATA_FAIL;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////end

	return dw;
}