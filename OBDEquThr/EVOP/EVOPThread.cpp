// EVOPThread.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"

#include "EVOPThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CEVOPThread, CWinThread)

CEVOPThread::CEVOPThread(void)
	: m_bQuitThread(false)
	, m_pAdapter(NULL)
{
	// ���󴴽�ʱ,��ʼ���豸
	InitializeInstrument(L"ZENYUAN");
	
	m_bComPort = 255;
	m_nComBaudRate = CBR_115200;
}

CEVOPThread::~CEVOPThread()
{
	// ��������ʱ,ж���豸
	UninitializeInstrument();
}

BOOL CEVOPThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CEVOPThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CEVOPThread, CWinThread)
END_MESSAGE_MAP()


// CEVOPThread ��Ϣ�������

BOOL CEVOPThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (m_bQuitThread)
	{
		if(WM_QUIT == pMsg->message)
		{
			return CWinThread::PreTranslateMessage(pMsg);
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return CWinThread::PreTranslateMessage(pMsg);
	}
	//return CWinThread::PreTranslateMessage(pMsg);
}

void CEVOPThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

void CEVOPThread::InitializeInstrument(const wchar_t *szProtocol)
{
	UninitializeInstrument();

	if (_wcsicmp(szProtocol, L"NHEVOP") == 0)
	{
		m_pAdapter = new CNHEVOP();
		m_emInstrumentModel = IM_NHEVOP;
	}
	else
	{
		// ��ƥ��
		m_pAdapter = new CNHEVOP();
		m_emInstrumentModel = IM_NHEVOP;
	}
}

void CEVOPThread::UninitializeInstrument(void)
{
	if (NULL != m_pAdapter)
	{
		delete m_pAdapter;
		m_pAdapter = NULL;
	}
}

DWORD CEVOPThread::Open(const BYTE bPort, const int nBaudRate, const wchar_t *szProtocol)
{
	m_bComPort = bPort;
	m_nComBaudRate = nBaudRate;

	InitializeInstrument(szProtocol);
	return m_pAdapter->Open(bPort, nBaudRate);
}

DWORD CEVOPThread::Close(void)
{
	DWORD dw = 0x00;
	if (NULL != m_pAdapter)
	{
		dw = m_pAdapter->Close();
	}
	UninitializeInstrument();
	return dw;
}

bool CEVOPThread::IsOpen(void) const
{
	if (m_pAdapter == NULL)
	{
		return false;
	}

	return m_pAdapter->IsOpen();
}

DWORD CEVOPThread::StartTest(const int nItem, const int nType)
{
	return m_pAdapter->StartTest(nItem, nType);
}

DWORD CEVOPThread::GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T)
{
	return m_pAdapter->GetRTData(fP1, fP2, fP1_T, fP2_T);
}

DWORD CEVOPThread::GetState(int &nState)
{
	return m_pAdapter->GetState(nState);
}

DWORD CEVOPThread::GetTestInfo(int &nTestState, int &nTimeCount)
{
	return m_pAdapter->GetTestInfo(nTestState, nTimeCount);
}

DWORD CEVOPThread::EndTest(int &nItem, int &nJudge)
{
	return m_pAdapter->EndTest(nItem, nJudge);
}
