// TacThread.cpp : 实现文件
//

#include "stdafx.h"
#include "TacThread.h"


// CTacThread

IMPLEMENT_DYNCREATE(CTacThread, CWinThread)

CTacThread::CTacThread()
: m_nRev(0)
, m_bQuitThread(false)
, m_pTac(NULL)
, m_dwStatus(0x00)
{
	// 对象创建时,初始化设备为:RPM5300
	InitializeInstrument(L"RPM5300");
}

CTacThread::~CTacThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL CTacThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CTacThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTacThread, CWinThread)
	ON_THREAD_MESSAGE(WM_TAC_GET_REV, OnGetRev)
END_MESSAGE_MAP()


// CTacThread 消息处理程序

BOOL CTacThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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

DWORD CTacThread::Open(const BYTE bPort, const wchar_t *szProtocol)
{
	InitializeInstrument(szProtocol);

	return m_pTac->Open(bPort);
}

DWORD CTacThread::Close(void)
{
	return m_pTac->Close();
}

DWORD CTacThread::GetRev(int *pnRev)
{
	m_dwStatus = m_pTac->GetRev(pnRev);
	return m_dwStatus;
}

void CTacThread::GetRev_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_TAC_GET_REV, 0, (LPARAM)hCallBackWnd);
}

void CTacThread::OnGetRev(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = m_pTac->GetRev(&m_nRev);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_TAC_GET_REV, (WPARAM)dw, 0);
	}
}

void CTacThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

void CTacThread::InitializeInstrument(const wchar_t *szProtocol)
{
	UninitializeInstrument();
		
	if(_wcsicmp(szProtocol, L"RPM5300") == 0)
	{
		m_pTac = new CNanhuaRpm5300();
		m_emInstrumentModel = IM_NANHUA_RPM5300;
	}
	else if(_wcsicmp(szProtocol, L"VMT2000") == 0)
	{
		m_pTac = new CFuLiVMT2000();
		m_emInstrumentModel = IM_FULI_VMT2000;
	}
	else if(_wcsicmp(szProtocol, L"VTET1000") == 0)
	{
		m_pTac = new CHuaGongVTET1000();
		m_emInstrumentModel = IM_HUAGONG_VTET1000;
	}
	else if(_wcsicmp(szProtocol, L"MQZ3") == 0)
	{
		m_pTac = new CMingQuanMQZ3();
		m_emInstrumentModel = IM_MINGQUAN_MQZ3;
	}
	else
	{
		// 无匹配
		m_pTac = new CNanhuaRpm5300();
		m_emInstrumentModel = IM_NANHUA_RPM5300;
	}
}

void CTacThread::UninitializeInstrument(void)
{
	if (NULL != m_pTac)
	{
		delete m_pTac;
		m_pTac = NULL;
	}
}