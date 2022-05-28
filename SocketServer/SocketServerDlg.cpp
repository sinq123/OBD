
// SocketServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MYLib_D.lib")
#else
#pragma comment( lib, "..\\Release\\MYLib.lib" )
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketServerDlg 对话框

CSocketServerDlg *g_pThisDlg = NULL;


CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bResponseRunning = false;
}

CSocketServerDlg:: ~CSocketServerDlg()
{

}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_BOOT_PORT, m_edBootPort);
	DDX_Control(pDX, IDC_PARENT_SERVE, m_IAParentIP);
	DDX_Control(pDX, IDC_ED_PARENT_PORT, m_edParentPort);

	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_reLog);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CSocketServerDlg::OnBnClickedBtnStart)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSocketServerDlg 消息处理程序

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_pThisDlg = this;

	LoadConfig();

	OnBnClickedBtnStart();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocketServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	
	m_bResponseRunning = false;
}

void CSocketServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bResponseRunning)
	{
		AfxMessageBox(L"请先停止停止服务");
		return;
	}

	CDialogEx::OnClose();
}

void CSocketServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSocketServerDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	m_btnStart.GetWindowTextW(strTemp);

	if (L"启动服务" == strTemp)
	{
		// 服务器端操作
		CString strBootPort, strParentIP, strParentPort;
		m_edBootPort.GetWindowTextW(strBootPort);
		m_IAParentIP.GetWindowTextW(strParentIP);
		m_edParentPort.GetWindowTextW(strParentPort);

		if (L""==strBootPort || L"0"==strBootPort)
		{
			//AfxMessageBox(L"请输入监听端口");
			PrintLog(L"请输入监听端口");
			m_edBootPort.SetFocus();
			return;
		}

		if (L""==strParentPort || L"0"==strParentPort)
		{
			//AfxMessageBox(L"请输入监听端口");
			PrintLog(L"请输入上级端口");
			m_edParentPort.SetFocus();
			return;
		}

		if (strParentIP.IsEmpty())
		{
			//AfxMessageBox(L"请输入监听端口");
			PrintLog(L"请输入上级端口");
			m_IAParentIP.SetFocus();
			return;
		}

		SetConfig();
		m_tcpService.SetServer(_wtoi(strBootPort));
		if (!m_tcpService.Listen())
		{
			//AfxMessageBox(L"监听失败");
			PrintLog(L"监听失败");
			return;
		}

		m_tcpClient.SetClient(strParentIP, _wtoi(strParentPort));
		if (!m_tcpClient.Listen())
		{
			m_tcpService.DisConnect();
			PrintLog(L"socket打开失败");
			return;
		}


		CreateThread(NULL, 0, ListenThread, 0, 0, NULL);	
		m_bResponseRunning = true;

		m_btnStart.SetWindowTextW(L"停止服务");
		m_edBootPort.EnableWindow(FALSE);
		m_edParentPort.EnableWindow(FALSE);
		m_IAParentIP.EnableWindow(FALSE);

	}
	else
	{
		m_bResponseRunning = false;
		m_tcpService.DisConnect();
		m_tcpClient.DisConnect();
		m_btnStart.SetWindowTextW(L"启动服务");
		m_edBootPort.EnableWindow(TRUE);
		m_edParentPort.EnableWindow(TRUE);
		m_IAParentIP.EnableWindow(TRUE);

		PrintLog(L"停止服务");
	}
}


void CSocketServerDlg::PrintLog(LPCTSTR szFormat, ...)
{
	TCHAR szBuf[4096] = {0};
	va_list args;
	va_start(args, szFormat);
	_vstprintf(szBuf, 4096, szFormat, args);
	va_end(args);

	CString strLog;
	strLog.AppendFormat(_T("%s	%s\r\n"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), szBuf);
		
	m_reLog.LineScroll(m_reLog.GetLineCount());
	int nLen = m_reLog.GetWindowTextLength();
	m_reLog.SetSel(nLen, nLen);
	m_reLog.ReplaceSel(strLog);

}

void CSocketServerDlg::LoadConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"SocketService.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	CString strTemp;
	//启动端口
	//BootPort=10012
	strTemp = (si.GetString(L"Service", L"BootPort", L"10022"));
	m_edBootPort.SetWindowTextW(strTemp);
	//上级服务器IP
	strTemp = (si.GetString(L"Service", L"ParentIP", L"127.0.0.1"));
	m_IAParentIP.SetWindowTextW(strTemp);
	//上级服务器端口
	strTemp = (si.GetString(L"Service", L"ParentPort", L"10023"));
	m_edParentPort.SetWindowTextW(strTemp);

}

void CSocketServerDlg::SetConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"SocketService.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	CString strTemp;
	//启动端口
	m_edBootPort.GetWindowTextW(strTemp);
	(si.SetString(L"Service", L"BootPort", strTemp));
	//上级服务器IP
	m_IAParentIP.GetWindowTextW(strTemp);
	(si.SetString(L"Service", L"ParentIP", strTemp));
	//上级服务器端口
	m_edParentPort.GetWindowTextW(strTemp);
	(si.SetString(L"Service", L"ParentPort", strTemp));
}


DWORD CSocketServerDlg::ListenThread(LPVOID lpParam)
{
	TRACE("ListenThread线程开始...\n");	
	g_pThisDlg->PrintLog(L"监听线程开始...");	
	while (true)  
	{ 
		SOCKET sClient = g_pThisDlg->m_tcpService.Accept();
		if (sClient == SOCKET_ERROR)
		{
			break;
		}
		CreateThread(NULL, 0, ResponseThread, (LPVOID)sClient, 0, NULL);
	}
	
	TRACE("ListenThread线程退出...\n");
	g_pThisDlg->PrintLog(L"监听线程退出...");
	return 0;
}

DWORD CSocketServerDlg::ResponseThread(LPVOID lpParam)
{
	TRACE("ResponseThread线程[%d]开始...\n", GetCurrentThreadId());
	SOCKET sClient = (SOCKET)lpParam;
	g_pThisDlg->PrintLog(L"服务线程[%d]开始...", GetCurrentThreadId());
	g_pThisDlg->PrintLog(L"客户端[%d]%s连接...", sClient, g_pThisDlg->m_tcpService.GetPeerInfo(sClient));
	
	// 等待socket握手
	Sleep(200);	

	while (g_pThisDlg->m_bResponseRunning)
	{
		ZDPacket sZDPSend(L"");	// struct类型，不会自动调用构造函数
		if (!g_pThisDlg->m_tcpService.RecvReqPacket(sClient, sZDPSend))
		{
			break;
		}
		else
		{
			// 经过中转上传后返回
			ZDPacket sZDPRecv(L"");
			g_pThisDlg->m_tcpClient.SendAndRecvPacket(sZDPSend, sZDPRecv);

			g_pThisDlg->PrintLog(L"收到%s请求", g_pThisDlg->m_tcpService.GetPeerInfo(sClient));
			// 响应请求		
#ifdef _DEBUG
			CString strXML(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
			strXML.AppendFormat(L"<response>");
			strXML.AppendFormat(L"<cmd>GetVCIState</cmd>");
			strXML.AppendFormat(L"<code>0</code>");
			strXML.AppendFormat(L"<msg>成功</msg>");
			strXML.AppendFormat(L"</response>");
			ZDPacket zd(strXML);
			if (sZDPRecv.strXML.empty())
			{
				sZDPRecv = zd;
			}
#endif
			if (!g_pThisDlg->Response(sClient, sZDPRecv))
			{
				break;
			}
		}
	}
	
	// 关闭并删除记录
	g_pThisDlg->m_tcpService.ClostClient(sClient);

	TRACE("ResponseThread线程[%d]退出...\n", GetCurrentThreadId());
	g_pThisDlg->PrintLog(L"服务线程[%d]退出...", GetCurrentThreadId());

	return 0;
}

bool CSocketServerDlg::Xml2Map(LPCTSTR szXML, std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, std::map<std::wstring, std::wstring> &mapData)
{
	CXmlReader xmlReader;
	if (xmlReader.Parse(szXML))
	{
		if (xmlReader.OpenNode(L"response/cmd"))
		{
			xmlReader.GetNodeContent(strCmd);
		}
		if (xmlReader.OpenNode(L"response/code"))
		{
			xmlReader.GetNodeContent(strCode);
		}
		if (xmlReader.OpenNode(L"response/msg"))
		{
			xmlReader.GetNodeContent(strMsg);
		}
		if (xmlReader.OpenNode(L"response/data"))
		{
			xmlReader.EnterNode();
			do
			{
				std::wstring strName, strContent;
				xmlReader.GetNodeName(strName);
				xmlReader.GetNodeContent(strContent);
				mapData[strName] = strContent;
			}
			while (xmlReader.NextSibling());
		}
		return true;
	}
	return false;
}

bool CSocketServerDlg::Response(SOCKET sockfd, ZDPacket &sPacketReq)
{
	std::wstring strCmd, strCode, strMsg;
	std::map<std::wstring, std::wstring> mapData;

	if (Xml2Map(sPacketReq.strXML.c_str(), strCmd, strCode, strMsg, mapData))
	{
		if (strCmd == L"")
		{
		}
	}
	// 是否解析成功都要转发
	if (m_tcpService.Response(sockfd, sPacketReq))
	{
		return true;
	}
	return false;
}