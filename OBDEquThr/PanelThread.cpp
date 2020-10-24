// PanelThread.cpp : 实现文件
//

#include "stdafx.h"
#include "PanelThread.h"


// CPanelThread

IMPLEMENT_DYNCREATE(CPanelThread, CWinThread)



CPanelThread::CPanelThread()
: m_bQuitThread(false)
, m_pLEDPanel(NULL)
{
	// 对象创建时,初始化设备为:NH50R
	InitializeInstrument(IM_NANHUA_NH50R);
}

CPanelThread::~CPanelThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL CPanelThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPanelThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPanelThread, CWinThread)
END_MESSAGE_MAP()


// CPanelThread 消息处理程序


BOOL CPanelThread::PreTranslateMessage(MSG* pMsg)
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

void CPanelThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}


void CPanelThread::InitializeInstrument(const enum InstrumentModel im)
{
	UninitializeInstrument();

	if(IM_NANHUA_NH50R == im)
	{
		m_pLEDPanel = new CNH50R();
	}
	else if (IM_NANHUA_NH1 == im)
	{
		//m_pLEDPanel = new CNH1();
	}
	else if (IM_NANHUA_NH2 == im)
	{
		//m_pLEDPanel = new CNH2();
	}
	else if (IM_NANHUA_NHSZKS == im)
	{
		m_pLEDPanel = new CNHSZKS();
	}
	else if (IM_NANHUA_NHSHTHD == im)
	{
		//m_pLEDPanel = new CNHSHTHD();
	}
	else
	{
		// 无匹配
		m_pLEDPanel = new CNH50R();
	}
}

void CPanelThread::UninitializeInstrument(void)
{
	if (NULL != m_pLEDPanel)
	{
		delete m_pLEDPanel;
		m_pLEDPanel = NULL;
	}
}

DWORD CPanelThread::ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort)
{
	return m_pLEDPanel->ConnectServer(pwchIP, usPort);
}

DWORD CPanelThread::ConnectServer(const BYTE bPort, const int nBaudRate, const enum InstrumentModel im/*= IM_NANHUA_NH50R*/)
{
	InitializeInstrument(im);
	return m_pLEDPanel->ConnectServer(bPort, nBaudRate);
}

DWORD CPanelThread::DisconnectServer(void)
{
	return m_pLEDPanel->DisconnectServer();
}

DWORD CPanelThread::SetList(void)
{
	return m_pLEDPanel->SetList();
}

DWORD CPanelThread::DelList(void)
{
	return m_pLEDPanel->DelList();
}

DWORD CPanelThread::Display(const wchar_t *const pwchDisplayTxt,
		const bool bTwinking,
		const char chLineSpace,
		const char chInDisplayMode,
		const char chOutDisplayMode,
		const char chFramePauseTime,
		const char chDisplaySpeed,
		const char chFontAndSize,
		const char chSetupType,
		const char chhAlign,
		const char chvAlign
		)
{
	return m_pLEDPanel->Display(pwchDisplayTxt,
		bTwinking,
		chLineSpace,
		chInDisplayMode,
		chOutDisplayMode,
		chFramePauseTime,
		chDisplaySpeed,
		chFontAndSize,
		chSetupType,
		chhAlign,
		chvAlign
		);
}

DWORD CPanelThread::SendToServer(const char *const pchBuf, const int nLen)
{
	return m_pLEDPanel->SendToServer(pchBuf, nLen);
}

DWORD CPanelThread::DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt)
{
	return m_pLEDPanel->DisLine(nPanel, nLine, pwchLineTxt);
}