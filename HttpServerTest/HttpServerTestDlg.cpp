
// HttpServerTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HttpServerTest.h"
#include "HttpServerTestDlg.h"
#include "afxdialogex.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
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


// CHttpServerTestDlg 对话框
// 启动端口
CString CHttpServerTestDlg:: m_strBootPort(L"1022");
// 上级服务器IP
CString CHttpServerTestDlg::m_strParentIP(L"127.0.0.1");
// 上级服务器端口
CString CHttpServerTestDlg::m_strParentPort(L"1023");


CHttpServerTestDlg* g_pCHttpServerTestDlg = NULL;



CHttpServerTestDlg::CHttpServerTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHttpServerTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHttpServerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_reLog);

	DDX_Control(pDX, IDC_ED_BOOT_PORT, m_edBootPort);
	DDX_Control(pDX, IDC_PARENT_SERVE, m_IAParentIP);
	DDX_Control(pDX, IDC_ED_PARENT_PORT, m_edParentPort);
}

BEGIN_MESSAGE_MAP(CHttpServerTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_WEBSERVICE, &CHttpServerTestDlg::OnBnClickedBtnStartWebservice)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CHttpServerTestDlg 消息处理程序

BOOL CHttpServerTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	g_pCHttpServerTestDlg = this;
	// 加载配置
	LoadConfig();

	// 延时启动服务
	SetTimer(1, 500, NULL);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHttpServerTestDlg::OnPaint()
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
HCURSOR CHttpServerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHttpServerTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
}


void CHttpServerTestDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CHttpServerTestDlg::PrintLog(LPCTSTR szFormat, ...)
{
	TCHAR szBuf[4096] = { 0 };
	va_list args;
	va_start(args, szFormat);
	_vstprintf(szBuf, 4096, szFormat, args);
	va_end(args);

	CString strLog;
	strLog.AppendFormat(_T("[%s] %s\r\n"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), szBuf);

	m_reLog.LineScroll(m_reLog.GetLineCount());
	int nLen = m_reLog.GetWindowTextLength();
	m_reLog.SetSel(nLen, nLen);
	m_reLog.ReplaceSel(strLog);
}

void CHttpServerTestDlg::LoadConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"HttpServer.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	//启动端口
	//BootPort=10012
	m_strBootPort = (si.GetString(L"HttpService", L"BootPort", L"1022"));
	m_edBootPort.SetWindowTextW(m_strBootPort);
	//上级服务器IP
	m_strParentIP = (si.GetString(L"HttpService", L"ParentIP", L"127.0.0.1"));
	m_IAParentIP.SetWindowTextW(m_strParentIP);
	//上级服务器端口
	m_strParentPort = (si.GetString(L"HttpService", L"ParentPort", L"1023"));
	m_edParentPort.SetWindowTextW(m_strParentPort);

}

void CHttpServerTestDlg::SetConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"HttpServer.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	//启动端口
	(si.SetString(L"HttpService", L"BootPort", m_strBootPort));
	//上级服务器IP
	(si.SetString(L"HttpService", L"ParentIP", m_strParentIP));
	//上级服务器端口
	(si.SetString(L"HttpService", L"ParentPort", m_strParentPort));
}

void CHttpServerTestDlg::OnBnClickedBtnStartWebservice()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_BTN_START_WEBSERVICE)->GetWindowTextW(str);

	if (L"启动服务" == str)
	{
		CString strParentPort, strParentIP, strBootPort;
		m_edBootPort.GetWindowTextW(strBootPort);
		m_IAParentIP.GetWindowTextW(strParentIP);
		m_edParentPort.GetWindowTextW(strParentPort);

		if (L"" == strParentPort || L"0" == strParentPort
			|| L"" == strParentIP || L"0" == strParentIP
			|| L"" == strParentPort || L"0" == strParentPort)
		{
			AfxMessageBox(L"请输入HttpService信息。");
			m_edBootPort.SetFocus();
			return;
		}

		m_strBootPort = strBootPort;
		m_strParentIP = strParentIP;
		m_strParentPort = strParentPort;
		SetConfig();

		StartWebService();

		GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"关闭软件");
	}
	else
	{
		CDialogEx::OnCancel();
	}
}

void CHttpServerTestDlg::StartWebService(void)
{
	/*
	// 单线程
	struct mg_server *server;
	server = mg_create_server(NULL);
	mg_set_option(server, "listening_port", TCharToUTF8(m_strWebServicePort));
	mg_add_uri_handler(server, "/", env_handler);
	PrintLog(L"Starting on port %s ...\r\n", UTF8ToTChar(mg_get_option(server, "listening_port")));

	for (;;)
	{
	mg_poll_server(server, 100);  // 超时时间（ms）
	}
	mg_destroy_server(&server);
	*/

	// 多线程
	struct mg_server *server1, *server2;

	server1 = mg_create_server((void *) "1");
	server2 = mg_create_server((void *) "2");

	mg_add_uri_handler(server1, "/", request_handler);
	mg_add_uri_handler(server2, "/", request_handler);

	mg_set_option(server1, "listening_port", TCharToUTF8(m_strBootPort));
	mg_set_listening_socket(server2, mg_get_listening_socket(server1));

	mg_start_thread(serve, server1);
	mg_start_thread(serve, server2);
	getchar();
}

int CHttpServerTestDlg::env_handler(struct mg_connection *conn)
{
	static int counter = 0;
	counter++;
	printf("counter: %d\n", counter);

	const char * str_data = conn->content;  // 服务端收到的消息
	int str_len = conn->content_len;        // 服务端收到的消息长度
	string str(str_data, str_len);
	mg_printf(conn, "Received: %s, %d", str.c_str(), counter);
	g_pCHttpServerTestDlg->PrintLog(L"Received %s,%d ...\r\n", UTF8ToTChar(str.c_str()), counter);
	return 0;
}

int CHttpServerTestDlg::request_handler(struct mg_connection *conn) 
{
	// 地址
	const char* str_uri = conn->uri;
	std::wstring wstrURL;
	wstrURL.append(m_strParentIP + L":" + m_strParentPort + UTF8ToTChar(str_uri)); 
	const char * str_data = conn->content;  // 服务端收到的消息
	const char * http_headers_name = conn->http_headers->name;
	const char * http_headers_value = conn->http_headers->value;

	int str_len = conn->content_len;        // 服务端收到的消息长度
	string str(str_data, str_len);
	g_pCHttpServerTestDlg->PrintLog(L"接收请求:%s %s, %d ...", wstrURL.c_str(), UTF8ToTChar(str.c_str()));

	std::wstring strRet = L""; 
	int nRet = 0;
	nRet = g_pCHttpServerTestDlg->CurlPost(wstrURL, UTF8ToTChar(str.c_str()), strRet);
	g_pCHttpServerTestDlg->PrintLog(L"中间接收上级服务器数据:%d, %s ...", nRet, strRet.c_str());
	// 头
	mg_send_header(conn, "Content-Type", "application/json; charset=utf-8");
	// 返回
	mg_send_status(conn, nRet);
	// 参数
	mg_printf_data(conn, "%s", TCharToUTF8(strRet.c_str()));

	return 0;
}

void *CHttpServerTestDlg::serve(void *server) 
{
	for (;;) mg_poll_server((struct mg_server *) server, 1000);
	return NULL;
}



int CHttpServerTestDlg::CurlPost(const std::wstring strURl, const std::wstring strPost, std::wstring& strRet)
{
	return CCurlHttp_API::CurlPost(strURl, strPost.c_str(), strRet);
}

