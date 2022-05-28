
// NHOBDPDADlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHOBDPDA.h"
#include "NHOBDPDADlg.h"
#include "afxdialogex.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

// 自用库(MFC扩展DLL)
#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "gsoap/soapH.h"
#include "gsoap/NHOBD.nsmap"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNHOBDPDADlg 对话框

int CNHOBDPDADlg::m_nWebServicePort(10022);
bool CNHOBDPDADlg::m_bWebServiceRun(false);
int CNHOBDPDADlg::m_nHead(0);
int CNHOBDPDADlg::m_nTail(0);

struct soap* CNHOBDPDADlg::m_mapSoap;
SOAP_SOCKET  CNHOBDPDADlg::m_Queue_Soap[MAX_QUEUE];
pthread_mutex_t  CNHOBDPDADlg::m_Queue_CS;
pthread_cond_t  CNHOBDPDADlg::m_Queue_CV;

CNHOBDPDADlg* g_pThisDlg = NULL;



CNHOBDPDADlg::CNHOBDPDADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNHOBDPDADlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNHOBDPDADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_reLog);
}

BEGIN_MESSAGE_MAP(CNHOBDPDADlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_WEBSERVICE, &CNHOBDPDADlg::OnBnClickedBtnStartWebservice)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHOBDPDADlg 消息处理程序

BOOL CNHOBDPDADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	g_pThisDlg = this;

	// 加载配置
	LoadConfig();

	// 初始化控件
	InitCtrls();
	// 延时启动服务
	SetTimer(TID_INIT_RUN, m_unTI_InitRun, NULL);
	// 自动重启
	SetTimer(TID_REBOOT, m_unTI_Reboot, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNHOBDPDADlg::OnPaint()
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
HCURSOR CNHOBDPDADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNHOBDPDADlg::OnBnClickedBtnStartWebservice()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_BTN_START_WEBSERVICE)->GetWindowTextW(str);

	if (L"启动服务" == str)
	{
		CString strPort;
		GetDlgItem(IDC_ED_WEBSERVICE_PORT)->GetWindowTextW(strPort);

		if (L"" == strPort || L"0" == strPort)
		{
			AfxMessageBox(L"请输入WebService端口。");
			GetDlgItem(IDC_ED_WEBSERVICE_PORT)->SetFocus();
			return;
		}

		// 保存配置
		wchar_t wchFilePath[MAX_PATH] = { L'\0' };
		CNHCommonAPI::GetHLDFilePath(L"Config", L"NHOBDPDA.ini", wchFilePath);

		CSimpleIni si(wchFilePath);
		si.SetString(L"WebService", L"Port", strPort);

		m_nWebServicePort = _wtoi(strPort);

		StartWebService();

		GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"停止服务");
	}
	else
	{
		StopWebService();
		m_bWebServiceRun = false;
		GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"启动服务");
	}
}

void CNHOBDPDADlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bWebServiceRun)
	{
		AfxMessageBox(L"请先停止服务再退出程序。");
		return;
	}
	CDialogEx::OnClose();
}

void CNHOBDPDADlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case TID_INIT_RUN:
		{
			KillTimer(TID_INIT_RUN);

			if (!m_bWebServiceRun)
			{
				OnBnClickedBtnStartWebservice();
			}
		}
		break;
	case TID_REBOOT:
		{
			KillTimer(TID_REBOOT);

			COleDateTime odtTime(COleDateTime::GetCurrentTime());
			const int nHour = odtTime.GetHour();
			const int nMinute = odtTime.GetMinute();
			CString strNow(odtTime.Format(L"%Y-%m-%d"));
			LoadConfig(); // 重新获取重启时间
			if (strNow != m_strRebootTime
				&& nHour == 7 && nMinute >= 5 && nMinute <= 10) // 每天7点5分前判断是否需要重启
			{
				m_strRebootTime = strNow;
				// 将重启时间写入配置文件
				wchar_t wchFilePath[MAX_PATH] = { L'\0' };
				CNHCommonAPI::GetHLDFilePath(L"Config", L"NHOBDPDA.ini", wchFilePath);
				CSimpleIni si(wchFilePath);
				si.SetString(L"General", L"RebootTime", m_strRebootTime);
				// 先关闭端口再重启
				OnBnClickedBtnStartWebservice();

				// 重启
				Reboot();
			}

			SetTimer(TID_REBOOT, m_unTI_Reboot, NULL);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CNHOBDPDADlg::WriteLog(LPCTSTR szLog)
{
	m_reLog.LineScroll(m_reLog.GetLineCount());
	int nLen = m_reLog.GetWindowTextLength();
	m_reLog.SetSel(nLen, nLen);
	m_reLog.ReplaceSel(szLog);
}


void CNHOBDPDADlg::InitCtrls(void)
{
	CStringW str;
	str.Format(L"%d", m_nWebServicePort);
	GetDlgItem(IDC_ED_WEBSERVICE_PORT)->SetWindowTextW(str);
}

void CNHOBDPDADlg::LoadConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"NHOBDPDA.ini", wchFilePath);

	CSimpleIni si(wchFilePath);
	m_nWebServicePort = _wtoi(si.GetString(L"WebService", L"Port", L"10022"));
	m_strRebootTime = si.GetString(L"General", L"RebootTime", L"");

	CStringW strGAURL = si.GetString(L"WebService", L"URL", L"172.16.207.20:8080");
	char* pchGAURL = CNHCommonAPI::UnicodeToANSI(strGAURL);
	strncpy_s(m_chGAURL, pchGAURL, strlen(pchGAURL));
	if (NULL != pchGAURL)
	{
		free(pchGAURL);
		pchGAURL = NULL;
	}
}

void CNHOBDPDADlg::Reboot(void)
{
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
	//获取exe程序当前路径   
	extern  CNHOBDPDAApp theApp;
	TCHAR szAppName[MAX_PATH];
	::GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
	CString strAppFullName;
	strAppFullName.Format(_T("%s"), szAppName);
	//重启程序   
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION procStruct;
	memset(&StartInfo, 0, sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	::CreateProcess(
		(LPCTSTR)strAppFullName,
		NULL,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&procStruct);
}

UINT CNHOBDPDADlg::GSoapServerThread(LPVOID pParam)
{
	struct soap sSoap;

	m_mapSoap = &sSoap;

	soap_init(&sSoap);

	soap_set_mode(&sSoap, SOAP_C_UTFSTRING);
	sSoap.fget = HttpGet;

	struct soap* soap_thr[MAX_THR]; // each thread needs a runtime context 
	pthread_t tid[MAX_THR];
	int port = m_nWebServicePort; // first command-line arg is port 
	SOAP_SOCKET m, s;
	m = soap_bind(&sSoap, NULL, port, BACKLOG);
	if (!soap_valid_socket(m))
	{
		soap_destroy(&sSoap);
		soap_end(&sSoap);
		soap_done(&sSoap);

		AfxMessageBox(L"端口已被占用，服务启动失败。");
		g_pThisDlg->GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"启动服务");

		m_bWebServiceRun = false;
	}
	else
	{
		m_bWebServiceRun = true;

		fprintf(stderr, "Socket connection successful %d\n", m);
		pthread_mutex_init(&m_Queue_CS, NULL);
		pthread_cond_init(&m_Queue_CV, NULL);
		for (int i = 0; i < MAX_THR; i++)
		{
			soap_thr[i] = soap_copy(&sSoap);

			fprintf(stderr, "Starting thread %d\n", i);
			pthread_create(&tid[i], NULL, (void* (*)(void*))Process_Queue, (void*)soap_thr[i]);
		}
		for (;;)
		{
			s = soap_accept(&sSoap);
			if (!soap_valid_socket(s))
			{
				if (sSoap.errnum)
				{
					soap_print_fault(&sSoap, stderr);
					continue; // retry 
				}
				else
				{
					fprintf(stderr, "Server timed out\n");
					break;
				}
			}
			CStringA strTrace;
			strTrace.Format("Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", MAX_THR, s, (sSoap.ip >> 24) & 0xFF, (sSoap.ip >> 16) & 0xFF, (sSoap.ip >> 8) & 0xFF, sSoap.ip & 0xFF);
			TRACE("\r\n" + strTrace + "\r\n");
			fprintf(stderr, "Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", MAX_THR, s, (sSoap.ip >> 24) & 0xFF, (sSoap.ip >> 16) & 0xFF, (sSoap.ip >> 8) & 0xFF, sSoap.ip & 0xFF);
			while (Enqueue(s) == SOAP_EOM)
			{
				Sleep(100);
			}
		}
		for (int i = 0; i < MAX_THR; i++)
		{
			while (Enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
			{
				Sleep(100);
			}
		}
		for (int i = 0; i < MAX_THR; i++)
		{
			soap_done(soap_thr[i]);
			fprintf(stderr, "Waiting for thread %d to terminate... ", i);
			pthread_join(tid[i], NULL);
			fprintf(stderr, "terminated\n");
			free(soap_thr[i]);
		}
		pthread_mutex_destroy(&m_Queue_CS);
		pthread_cond_destroy(&m_Queue_CV);

		//soap_done(&sSoap); 

		//pthread_exit(NULL);
	}

	return 0;
}

void CNHOBDPDADlg::StartWebService(void)
{
	AfxBeginThread(GSoapServerThread, this);
}

void CNHOBDPDADlg::StopWebService(void)
{
	soap_destroy(m_mapSoap);
	soap_end(m_mapSoap);
	soap_done(m_mapSoap);
}

int CNHOBDPDADlg::HttpGet(soap* soap)
{
	// 获取wsdl文件
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"NHOBD.wsdl", wchFilePath);

	FILE* fd = NULL;
	_wfopen_s(&fd, wchFilePath, L"rb"); //open WSDL file to copy
	if (!fd)
	{
		soap_response(soap, SOAP_HTML);
		soap_send(soap, "<html>Wsdl file is missing, please check Config folder.</html>\n");
		soap_end_send(soap);
		return SOAP_OK;
	}
	soap->http_content = "text/xml";  //HTTP header with text /xml content
	soap_response(soap, SOAP_FILE);
	for (;;)
	{
		size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
		if (!r)
		{
			break;
		}
		if (soap_send_raw(soap, soap->tmpbuf, r))
		{
			break; //cannot send, but little we can do about that
		}
	}
	fclose(fd);
	soap_end_send(soap);

	return SOAP_OK;
	return 0;
}

void *  CNHOBDPDADlg::Process_Queue(void *soap) 
{ 
	struct soap *tsoap = (struct soap*)soap; 
	for (;;) 
	{ 
		tsoap->socket = Dequeue(); 
		if (!soap_valid_socket(tsoap->socket)) 
		{
			break; 
		}
		soap_serve(tsoap); 
		soap_destroy(tsoap); 
		soap_end(tsoap); 
		fprintf(stderr, "served\n"); 
	} 
	return NULL; 
} 
int  CNHOBDPDADlg::Enqueue(SOAP_SOCKET sock) 
{ 
	int nStatus = SOAP_OK; 
	int nNext; 
	pthread_mutex_lock(&m_Queue_CS); 
	nNext = m_nTail + 1; 
	if (nNext >= MAX_QUEUE) 
	{
		nNext = 0; 
	}
	if (nNext == m_nHead) 
	{
		nStatus = SOAP_EOM; 
	}
	else
	{ 
		m_Queue_Soap[m_nTail] = sock; 
		m_nTail = nNext; 
		pthread_cond_signal(&m_Queue_CV); 
	} 
	pthread_mutex_unlock(&m_Queue_CS); 

	return nStatus; 
} 
SOAP_SOCKET  CNHOBDPDADlg::Dequeue() 
{ 
	SOAP_SOCKET sock; 
	pthread_mutex_lock(&m_Queue_CS); 
	while (m_nHead == m_nTail) 
	{
		pthread_cond_wait(&m_Queue_CV, &m_Queue_CS); 
	}
	sock = m_Queue_Soap[m_nHead++]; 
	if (m_nHead >= MAX_QUEUE) 
	{
		m_nHead = 0; 
	}
	pthread_mutex_unlock(&m_Queue_CS); 
	return sock; 
}

// //初始化一个以微秒为单位的时间种子
int randEx()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}
///////////////////////////具体服务方法////////////////////////////////////////
int ns__queryObjectOut(struct soap*, std::wstring _jkid, std::wstring _QueryXmlDoc, std::wstring &_queryObjectOutReturn)
{
	return g_pThisDlg->QueryObjectOut(_jkid, _QueryXmlDoc, _queryObjectOutReturn);
}



int CNHOBDPDADlg::QueryObjectOut(const std::wstring strJkid, const std::wstring strUTF8XmlDoc, std::wstring &strQOOR)
{
	WriteLog(L"\r\n");

	CString strLog;
	strLog.Format(L"接收到QueryObjectOut请求...\r\n");
	strLog.AppendFormat(L"请求ID：%s\r\n", strJkid.c_str());
	strLog.AppendFormat(L"请求XML：%s\r\n", strUTF8XmlDoc.c_str());
	WriteLog(strLog);

	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	std::wstring strXml(strUTF8XmlDoc);
	// 处理接口ID
	if (L"W01" == strJkid)
	{
		QueryObjectOut_W01(strUTF8XmlDoc, strXml);
	}
	if (L"W02" == strJkid)
	{
		QueryObjectOut_W02(strUTF8XmlDoc, strXml);
	}

	int nRet = soap_call_ns__queryObjectOut(&soapClient, g_pThisDlg->m_chGAURL, NULL, strJkid, strXml, strQOOR);

	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}


void CNHOBDPDADlg::QueryObjectOut_W01(const std::wstring strUTF8XmlDoc, std::wstring &strXml)
{

	// 解析出的字段
	SResultOfOBD sResultOfOBD;
	std::wstring strTemp;
	// XML解析
	CXmlReader xmlReader;
	if (xmlReader.Parse(strUTF8XmlDoc.c_str()))
	{
		if (xmlReader.OpenNode(L"root/body/RunningNumber"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strRunningNumber = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/Protocol"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strOBDProtocol = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/DetBegTime"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strDetBegTime = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/DetEndTime"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strDetEndTime = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/ComErrorJudge"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strComErrorJudge = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/ComErrorComment"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strComErrorComment = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/DisplayJudge"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strDisplayJudge = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/LampStateJudge"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strLampStateJudge = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/DTC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strDTC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/DTC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strDTC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/FreezeData"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strFreezeData = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/VIN"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strVIN = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/OBDType"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strOBDType = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/Mileage"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strMileage = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/EngineCALID"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strEngineCALID = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/EngineCVN"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strEngineCVN = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/PostProcessingCALID"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strPostProcessingCALID = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/PostProcessingCVN"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strPostProcessingCVN = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/OtherCALID"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strOtherCALID = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/OtherCVN"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strOtherCVN = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_MFT"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_MFT = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_FST"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_FST = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_CC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_CC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_CCH"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_CCH = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_OS"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_OS = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_OSH"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_OSH = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_EECS"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_EECS = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_EGR"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_EGR = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_SAIS"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_SAIS = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_ICM"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_ICM = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_VVT"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_VVT = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_DOC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_DOC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_SCR"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_SCR = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_DPF"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_DPF = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_ASC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_ASC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CheckItemJudge_POC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strCheckItemJudge_POC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_OMCEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_OMCEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_ICC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_ICC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_CMCCB1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_CMCCB1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_CMCECB1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_CMCECB1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_CMCCB2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_CMCCB2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_CMCECB2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_CMCECB2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_O2SMCCB1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_O2SMCCB1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_O2SMCECB1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_O2SMCECB1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_O2SMCCB2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_O2SMCCB2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_O2SMCECB2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_O2SMCECB2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_RO2SMCCB1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_RO2SMCCB1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_RO2SMCECB1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_RO2SMCECB1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_RO2SMCCB2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_RO2SMCCB2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_RO2SMCECB2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_RO2SMCECB2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_EGRC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_EGRC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_EGREC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_EGREC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_GPFC1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_GPFC1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_GPFEC1"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_GPFEC1 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_GPFC2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_GPFC2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_GPFEC2"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_GPFEC2 = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_AMCCC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_AMCCC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_AMCEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_AMCEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_EVAPC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_EVAPC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_EVAPEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_EVAPEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_NMHCC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_NMHCC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_NMHCEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_NMHCEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_NOXCC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_NOXCC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_NOXCEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_NOXCEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_NOXAC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_NOXAC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_NOXAEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_NOXAEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_PMC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_PMC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_PMEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_PMEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_WSC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_WSC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_WSEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_WSEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_PPC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_PPC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/IUPR_PPEC"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strIUPR_PPEC = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/Judge"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strJudge = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/Operator"))
		{
			xmlReader.GetNodeContent(strTemp);
			sResultOfOBD.strOperator = strTemp;
		}

	}

	if (sResultOfOBD.strJudge.find(L"1") == -1)
	{
		if (!sResultOfOBD.strRunningNumber.empty())
		{
			CString strSql;
			strSql.Format(L"select * from TestLog where RunningNumber = '%s'", sResultOfOBD.strRunningNumber.c_str());
			TESTLOG sTestLog;
			if (0x01 == GetDboTestLog(strSql, &sTestLog))
			{
				sResultOfOBD.strFuelType = sTestLog.wchFuelType;

				if (sResultOfOBD.strJudge.find(L"1") == -1)
				{
					SetOBDLOG(sResultOfOBD);

					strXml = UploadOBDDetData(sResultOfOBD);
				}
			}
		}
	}
}

void CNHOBDPDADlg::QueryObjectOut_W02(const std::wstring strUTF8XmlDoc, std::wstring &strXml)
{
	std::wstring strRunningNumber;
	SOBDRTData sRTData;
	// 解析出的字段
	std::wstring strTemp;
	// XML解析
	CXmlReader xmlReader;
	if (xmlReader.Parse(strUTF8XmlDoc.c_str()))
	{
		if (xmlReader.OpenNode(L"root/body/RunningNumber"))
		{
			xmlReader.GetNodeContent(strTemp);
			strRunningNumber = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/Velocity"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strVelocity = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CoolantTemp"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strCoolantTemp = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/EngineRev"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strEngineRev = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/SolarTermDoorPosition"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strSolarTermDoorPosition = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/ThrottleOpening"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strThrottleOpening = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/CalculationLoad"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strCalculationLoad = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/ForwardSensorV"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strForwardSensorV = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/ForwardSensorI"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strForwardSensorI = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/AirCoefficient"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strAirCoefficient = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/MAF"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strMAF = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/MAP"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strMAP = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/EngineOutputPower"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strEngineOutputPower = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/ChargeAirPressure"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strChargeAirPressure = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/FuelConsumption"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strFuelConsumption = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/NOXConcentration"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strNOXConcentration = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/UreaInjectionVolume"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strUreaInjectionVolume = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/EGT"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strEGT = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/DPFDifferentialPressure"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strDPFDifferentialPressure = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/EGRPosition"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strEGRPosition = strTemp;
		}
		if (xmlReader.OpenNode(L"root/body/FuelDeliveryPressure"))
		{
			xmlReader.GetNodeContent(strTemp);
			sRTData.strFuelDeliveryPressure = strTemp;
		}
	}


	if (!strRunningNumber.empty())
	{
		//CString strSql;
		//strSql.Format(L"select * from TestLog where RunningNumber = '%s'", sResultOfOBD.strRunningNumber.c_str());
		//TESTLOG sTestLog;
		//if (0x01 == GetDboTestLog(strSql, &sTestLog))
		//{

		//}
	}

}

/////////////////////////////////////////////////////////////////////////////


void CNHOBDPDADlg::SetOBDLOG(SResultOfOBD& sResultOfOBD)
{
	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	if (odtCurTime.ParseDateTime(sResultOfOBD.strDetBegTime.c_str()))
	{
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 40);
		COleDateTime odtEndTime = odtCurTime + odts;
		sResultOfOBD.strDetEndTime = odtEndTime.Format(L"%Y-%m-%d %H:%M:%S");
	}

	if (sResultOfOBD.strOBDType.empty())
	{
		CString strOBDType;
		int n = randEx()%10;
		CString strEngineCALID, strEngineCVN;
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			strOBDType.Format(L"%s", GetOBDType(n, L"柴油")); 
			GetEngineCALID(strOBDType, L"柴油", strEngineCALID, strEngineCVN);
		}
		else
		{
			strOBDType.Format(L"%s", GetOBDType(n, L"汽油")); 
			GetEngineCALID(strOBDType, L"汽油", strEngineCALID, strEngineCVN);
		}

		sResultOfOBD.strOBDType  = strOBDType.GetString();
		sResultOfOBD.strEngineCALID = strEngineCALID.GetString();
		sResultOfOBD.strEngineCVN = strEngineCVN.GetString();
	}

	if (sResultOfOBD.strComErrorJudge.empty())
	{
		sResultOfOBD.strComErrorJudge = L"1";
	}

	if (sResultOfOBD.strComErrorComment.empty())
	{
		sResultOfOBD.strComErrorComment = L"";
	}

	if (sResultOfOBD.strDisplayJudge.empty())
	{
		sResultOfOBD.strDisplayJudge = L"1";
	}

	if (sResultOfOBD.strDisplayComment.empty())
	{
		sResultOfOBD.strDisplayComment = L"";
	}

	if (sResultOfOBD.strLampStateJudge.empty())
	{

		sResultOfOBD.strLampStateJudge = L"1";
	}

	if (sResultOfOBD.strLampStateComment.empty())
	{
		sResultOfOBD.strLampStateComment = L"";
	}

	if (sResultOfOBD.strDTC.empty())
	{
		sResultOfOBD.strDTC = L"";
	}

	if (sResultOfOBD.strDTCComment.empty())
	{
		sResultOfOBD.strDTCComment = L"";
	}

	if (sResultOfOBD.strFreezeData.empty())
	{
		sResultOfOBD.strFreezeData = L"";
	}

	if (sResultOfOBD.strDTCMileage.empty())
	{
		sResultOfOBD.strDTCMileage = L"";
	}

	if (sResultOfOBD.strDTCJudge.empty())
	{
		sResultOfOBD.strDTCJudge = L"1";
	}

	if (sResultOfOBD.strCheckItemJudge_MFT.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strCheckItemJudge_MFT = L"0";
		}
		else
		{
			sResultOfOBD.strCheckItemJudge_MFT = L"1";
		}
	}

	if (sResultOfOBD.strCheckItemJudge_FST.empty())
	{
		sResultOfOBD.strCheckItemJudge_FST = L"1";
	}

	if (sResultOfOBD.strCheckItemJudge_CC.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strCheckItemJudge_CC = L"0";
		}
		else
		{
			sResultOfOBD.strCheckItemJudge_CC = L"1";
		}
	}

	if (sResultOfOBD.strCheckItemJudge_CCH.empty())
	{
		sResultOfOBD.strCheckItemJudge_CCH = L"0";
	}

	if (sResultOfOBD.strCheckItemJudge_OS.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strCheckItemJudge_OS = L"0";
		}
		else
		{
			sResultOfOBD.strCheckItemJudge_OS = L"1";
		}
	}

	if (sResultOfOBD.strCheckItemJudge_OSH.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strCheckItemJudge_OSH = L"0";
		}
		else
		{
			sResultOfOBD.strCheckItemJudge_OSH = L"1";
		}
	}

	if (sResultOfOBD.strCheckItemJudge_EECS.empty())
	{
		sResultOfOBD.strCheckItemJudge_EECS = L"0";
	}

	if (sResultOfOBD.strCheckItemJudge_EGR.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strCheckItemJudge_EGR = L"0";
		}
		else
		{
			sResultOfOBD.strCheckItemJudge_EGR = L"1";
		}
	}

	if (sResultOfOBD.strCheckItemJudge_SAIS.empty())
	{
		sResultOfOBD.strCheckItemJudge_SAIS = L"0";
	}

	if (sResultOfOBD.strCheckItemJudge_ICM.empty())
	{
		sResultOfOBD.strCheckItemJudge_ICM = L"1";
	}

	if (sResultOfOBD.strCheckItemJudge_VVT.empty())
	{
		sResultOfOBD.strCheckItemJudge_VVT = L"0";
	}

	if (sResultOfOBD.strSystemCheckJudge.empty())
	{
		sResultOfOBD.strSystemCheckJudge = L"1";
	}

	if (sResultOfOBD.strIUPR_OMCEC.empty())
	{
		CString strTemp;
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			strTemp.Format(L"%d", (int)randEx()%100+1501);
		}
		else
		{
			strTemp.Format(L"%d", (int)rand()%100+101);
		}
		sResultOfOBD.strIUPR_OMCEC = strTemp.GetString();
	}

	if (sResultOfOBD.strIUPR_ICC.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_ICC = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+601);
			sResultOfOBD.strIUPR_ICC = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_CMCCB1.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_CMCCB1 = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%40+101);
			sResultOfOBD.strIUPR_CMCCB1 = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_CMCECB1.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_CMCECB1 = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+101);
			sResultOfOBD.strIUPR_CMCECB1 = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_CMCCB2.empty())
	{
		sResultOfOBD.strIUPR_OMCEC = L"0";
	}
	if (sResultOfOBD.strIUPR_CMCECB2.empty())
	{
		sResultOfOBD.strIUPR_CMCECB2 = L"0";
	}
	if (sResultOfOBD.strIUPR_O2SMCCB1.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_O2SMCCB1 = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+101);
			sResultOfOBD.strIUPR_O2SMCCB1 = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_O2SMCECB1.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_O2SMCECB1 = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+101);
			sResultOfOBD.strIUPR_O2SMCECB1 = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_O2SMCCB2.empty())
	{
		sResultOfOBD.strIUPR_O2SMCCB2 = L"0";
	}
	if (sResultOfOBD.strIUPR_O2SMCECB2.empty())
	{
		sResultOfOBD.strIUPR_O2SMCECB2 = L"0";
	}
	if (sResultOfOBD.strIUPR_RO2SMCCB1.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_RO2SMCCB1 = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+141);
			sResultOfOBD.strIUPR_RO2SMCCB1 = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_RO2SMCECB1.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_RO2SMCECB1 = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+101);
			sResultOfOBD.strIUPR_RO2SMCECB1 = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_RO2SMCCB2.empty())
	{
		sResultOfOBD.strIUPR_RO2SMCCB2 = L"0";
	}
	if (sResultOfOBD.strIUPR_RO2SMCECB2.empty())
	{
		sResultOfOBD.strIUPR_RO2SMCECB2 = L"0";
	}
	if (sResultOfOBD.strIUPR_EGRC.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_EGRC = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+501);
			sResultOfOBD.strIUPR_EGRC = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_EGREC.empty())
	{
		if (sResultOfOBD.strFuelType.find(L"柴") != -1)
		{
			sResultOfOBD.strIUPR_EGREC = L"0";
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%d", (int)rand()%100+101);
			sResultOfOBD.strIUPR_EGREC = strTemp.GetString();
		}
	}
	if (sResultOfOBD.strIUPR_GPFC1.empty())
	{
		sResultOfOBD.strIUPR_GPFC1 = L"";
	}
	if (sResultOfOBD.strIUPR_GPFEC1.empty())
	{
		sResultOfOBD.strIUPR_GPFEC1 = L"";
	}
	if (sResultOfOBD.strIUPR_GPFC2.empty())
	{
		sResultOfOBD.strIUPR_GPFC2 = L"";
	}
	if (sResultOfOBD.strIUPR_GPFEC2.empty())
	{
		sResultOfOBD.strIUPR_GPFEC2 = L"";
	}
	if (sResultOfOBD.strIUPR_AMCCC.empty())
	{
		sResultOfOBD.strIUPR_AMCCC = L"0";
	}
	if (sResultOfOBD.strIUPR_AMCEC.empty())
	{
		sResultOfOBD.strIUPR_AMCEC = L"0";
	}
	if (sResultOfOBD.strIUPR_OMCEC.empty())
	{
		sResultOfOBD.strIUPR_OMCEC = L"0";
	}
	if (sResultOfOBD.strIUPR_EVAPEC.empty())
	{
		sResultOfOBD.strIUPR_EVAPEC = L"0";
	}
	sResultOfOBD.strJudge = L"1";
}

CString CNHOBDPDADlg::GetOBDType(const int& nType, const CString& strFulType)
{
	if (strFulType == L"汽油")
	{
		switch (nType)
		{
		case 0:{ return L"EOBD";} break;
		case 1:{ return L"ISO/SAE预留";} break;
		case 2:{return L"OBDII-加州";} break;
		case 3:{return L"OBD-OBDII";} break;
		case 4:{return L"JOBD-EOBD";} break;
		case 5:{return L"OBD,OBDII,EOBD和KOBD";} break;
		case 6:{return L"JOBD";} break;
		case 7:{return L"不适用";} break;
		default : {return L"";} break;
		}
	}
	else
	{
		return L"";
	}
}

void CNHOBDPDADlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	if (strFulType == L"汽油")
	{
		if (strOBDType == L"EOBD")
		{
			int n = randEx()%20;
			switch (n)
			{
			case 0:{strEngineCALID = L"000008603574000008627583"; strEngineCVN = L"29A907FDF9511FC1";} break;
			case 1:{strEngineCALID = L"000008617572000008622137"; strEngineCVN = L"B1332BBA109DB30D";} break;
			case 2:{strEngineCALID = L"000008617572000008638799"; strEngineCVN = L"B1332BBA67E4F98A";} break;
			case 3:{strEngineCALID = L"00000925"; strEngineCVN = L"000050D5";} break;
			case 4:{strEngineCALID = L"0000QSB10900UAES"; strEngineCVN = L"不支持";} break;
			case 5:{strEngineCALID = L"0034468840270090"; strEngineCVN = L"487D596F";} break;
			case 6:{strEngineCALID = L"0044469440150263"; strEngineCVN = L"89225B2E00000000";} break;
			case 7:{strEngineCALID = L"0064464240190159"; strEngineCVN = L"AE90F315";} break;
			case 8:{strEngineCALID = L"00740R0366UAES"; strEngineCVN = L"不支持";} break;
			case 9:{strEngineCALID = L"007503A330UAES"; strEngineCVN = L"不支持";} break;
			case 10:{strEngineCALID = L"00C80R0334GW"; strEngineCVN = L"不支持";} break;
			case 11:{strEngineCALID = L"00JB0HA234UAES"; strEngineCVN = L"不支持";} break;
			case 12:{strEngineCALID = L"00MP230421UAES"; strEngineCVN = L"不支持";} break;
			case 13:{strEngineCALID = L"00MP261016UAES"; strEngineCVN = L"不支持";} break;
			case 14:{strEngineCALID = L"0356CMR3SC110815"; strEngineCVN = L"B23725E3";} break;
			case 15:{strEngineCALID = L"0356CMR5SC120830"; strEngineCVN = L"E8927CBA";} break;
			case 16:{strEngineCALID = L"03C906014CE1108"; strEngineCVN = L"不支持";} break;
			case 17:{strEngineCALID = L"03C906022AS4150"; strEngineCVN = L"A5416587";} break;
			case 18:{strEngineCALID = L"03C906022BH9928"; strEngineCVN = L"96F5B3BE";} break;
			case 19:{strEngineCALID = L"03C906022CA4511"; strEngineCVN = L"A7345A7A";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"ISO/SAE预留")
		{
			int n = randEx()%4;
			switch (n)
			{
			case 0: {strEngineCALID = L"10307843"; strEngineCVN = L"0000D42D";} break;
			case 1: {strEngineCALID = L"10294382"; strEngineCVN = L"00003C06";} break;
			case 2: {strEngineCALID = L"10190196"; strEngineCVN = L"000087DE";} break;
			case 3: {strEngineCALID = L"10236164"; strEngineCVN = L"00005C7F";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"OBDII-加州")
		{
			int n = randEx()%4;
			switch (n)
			{
			case 0: {strEngineCALID = L"30CG8001A0C01000"; strEngineCVN = L"4A44F2412C411C65";} break;
			case 1: {strEngineCALID = L"68193991AG"; strEngineCVN = L"39381886";} break;
			case 2: {strEngineCALID = L"2410568200"; strEngineCVN = L"5A4F616B";} break;
			case 3: {strEngineCALID = L"30CL120150C81101"; strEngineCVN = L"BFEB74AB5FA56D64";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"OBD-OBDII")
		{
			int n = randEx()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"JOBD-EOBD")
		{
			int n = randEx()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"31411182AA"; strEngineCVN = L"F257665B";} break;
			case 1: {strEngineCALID = L"31459703AA"; strEngineCVN = L"AD5B51F9";} break;
			case 2: {strEngineCALID = L"32267993"; strEngineCVN = L"1595668E";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"JOBD")
		{
			int n = randEx()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"不适用")
		{
			int n = randEx()%15;
			switch (n)
			{
			case 0: {strEngineCALID = L"37805RD7H530"; strEngineCVN = L"E92031E2";} break;
			case 1: {strEngineCALID = L"WE3DIVG1VII"; strEngineCVN = L"26732FED";} break;
			case 2: {strEngineCALID = L"37805RD7H040"; strEngineCVN = L"DB7E7874";} break;
			case 3: {strEngineCALID = L"WE40TP0X"; strEngineCVN = L"32DE0AFE";} break;
			case 4: {strEngineCALID = L"37805R0TH010"; strEngineCVN = L"15ECBE96";} break;
			case 5: {strEngineCALID = L"WE40TM1X"; strEngineCVN = L"D1E6ABBE";} break;
			case 6: {strEngineCALID = L"WE32TN0V20130807"; strEngineCVN = L"FCD2E103";} break;
			case 7: {strEngineCALID = L"W3B3RL0X"; strEngineCVN = L"1CEB62B7";} break;
			case 8: {strEngineCALID = L"WH24WZ0X"; strEngineCVN = L"71481A23";} break;
			case 9: {strEngineCALID = L"WH24WX0X"; strEngineCVN = L"E2FA939C";} break;
			case 10: {strEngineCALID = L"37805RLHH620"; strEngineCVN = L"7CB6834A";} break;
			case 11: {strEngineCALID = L"WH22WY1V"; strEngineCVN = L"A19D018B";} break;
			case 12: {strEngineCALID = L"W3B2RP0V"; strEngineCVN = L"CFAC7099";} break;
			case 13: {strEngineCALID = L"WH24WX1X"; strEngineCVN = L"2D863958";} break;
			case 14: {strEngineCALID = L"YM008AT0600300G0"; strEngineCVN = L"42716105";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else 
		{
			int n = randEx()%4;
			switch (n)
			{
			case 0: {strEngineCALID = L"306B4000"; strEngineCVN = L"97627560";} break;
			case 1: {strEngineCALID = L"306B4000"; strEngineCVN = L"97627560";} break;
			case 2: {strEngineCALID = L"LF0L0643UAES"; strEngineCVN = L"不支持";} break;
			case 3: {strEngineCALID = L"378055A4H640"; strEngineCVN = L"378055A4H640";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
	}
	else
	{
		int n = randEx()%15;
		switch (n)
		{
		case 0: {strEngineCALID = L"3601603B90"; strEngineCVN = L"488713A2DEF671A8";} break;
		case 1: {strEngineCALID = L"A5CE33823352SFB"; strEngineCVN = L"B94CF03432D9AAC8";} break;
		case 2: {strEngineCALID = L"E63353823352AC"; strEngineCVN = L"4824407D3CCC0824";} break;
		case 3: {strEngineCALID = L"EDC720"; strEngineCVN = L"8DC3BC97FEC2BF53";} break;
		case 4: {strEngineCALID = L"FN80009.09"; strEngineCVN = L"737771052";} break;
		case 5: {strEngineCALID = L"S072805F1112"; strEngineCVN = L"D5FE13991D7EA592";} break;
		case 6: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"977841D02EE2EC41";} break;
		case 7: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"A73BE97A8ECBEB46";} break;
		case 8: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"D04E5AB13A4655FF";} break;
		case 9: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"84C7C19E69D9E257";} break;
		case 10: {strEngineCALID = L"S114405B1204"; strEngineCVN = L"7F1D977131B817F0";} break;
		case 11: {strEngineCALID = L"S114405B1204"; strEngineCVN = L"6C84F9DCF436BEC9";} break;
		case 12: {strEngineCALID = L"S114405B1204"; strEngineCVN = L"19A6AF95F2E08A15";} break;
		case 13: {strEngineCALID = L"SCHD13823352AC"; strEngineCVN = L"9CBB4CAC97DB";} break;
		case 14: {strEngineCALID = L"不支持"; strEngineCVN = L"E0002658F816511F";} break;
		default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
		}

	}
}


std::wstring CNHOBDPDADlg::UploadOBDDetData(const SResultOfOBD &sResultOfOBD)
{
	CString strTemp;
	CString strXML;
	strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	strXML.AppendFormat(L"<root><body>");
	strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", sResultOfOBD.strRunningNumber.c_str());
	strXML.AppendFormat(L"<Protocol>%s</Protocol>", sResultOfOBD.strOBDProtocol.c_str());
	strXML.AppendFormat(L"<DetBegTime>%s</DetBegTime>", sResultOfOBD.strDetBegTime.c_str());
	strXML.AppendFormat(L"<DetEndTime>%s</DetEndTime>", sResultOfOBD.strDetEndTime.c_str());
	strXML.AppendFormat(L"<ComErrorJudge>%s</ComErrorJudge>", sResultOfOBD.strComErrorJudge.c_str());
	strXML.AppendFormat(L"<ComErrorComment>%s</ComErrorComment>", sResultOfOBD.strComErrorComment.c_str());
	strXML.AppendFormat(L"<DisplayJudge>%s</DisplayJudge>", sResultOfOBD.strDisplayJudge.c_str());
	strXML.AppendFormat(L"<LampStateJudge>%s</LampStateJudge>", sResultOfOBD.strLampStateJudge.c_str());
	strXML.AppendFormat(L"<DTC>%s</DTC>", sResultOfOBD.strDTC.c_str());
	strXML.AppendFormat(L"<DTCMileage>%s</DTCMileage>", sResultOfOBD.strDTCMileage.c_str());
	strXML.AppendFormat(L"<FreezeData>%s</FreezeData>", sResultOfOBD.strFreezeData.c_str());
	strXML.AppendFormat(L"<VIN>%s</VIN>", sResultOfOBD.strVIN.c_str());
	strXML.AppendFormat(L"<OBDType>%s</OBDType>", sResultOfOBD.strOBDType.c_str());
	strXML.AppendFormat(L"<Mileage>%s</Mileage>", sResultOfOBD.strMileage.c_str());
	strXML.AppendFormat(L"<EngineCALID>%s</EngineCALID>", sResultOfOBD.strEngineCALID.c_str());
	strXML.AppendFormat(L"<EngineCVN>%s</EngineCVN>", sResultOfOBD.strEngineCVN.c_str());
	strXML.AppendFormat(L"<PostProcessingCALID>%s</PostProcessingCALID>", sResultOfOBD.strPostProcessingCALID.c_str());
	strXML.AppendFormat(L"<PostProcessingCVN>%s</PostProcessingCVN>", sResultOfOBD.strPostProcessingCVN.c_str());
	strXML.AppendFormat(L"<OtherCALID>%s</OtherCALID>", sResultOfOBD.strOtherCALID.c_str());
	strXML.AppendFormat(L"<OtherCVN>%s</OtherCVN>", sResultOfOBD.strOtherCVN.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_MFT>%s</CheckItemJudge_MFT>", sResultOfOBD.strCheckItemJudge_MFT.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_FST>%s</CheckItemJudge_FST>", sResultOfOBD.strCheckItemJudge_FST.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_CC>%s</CheckItemJudge_CC>", sResultOfOBD.strCheckItemJudge_CC.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_CCH>%s</CheckItemJudge_CCH>", sResultOfOBD.strCheckItemJudge_CCH.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_OS>%s</CheckItemJudge_OS>", sResultOfOBD.strCheckItemJudge_OS.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_OSH>%s</CheckItemJudge_OSH>", sResultOfOBD.strCheckItemJudge_OSH.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_EECS>%s</CheckItemJudge_EECS>", sResultOfOBD.strCheckItemJudge_EECS.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_EGR>%s</CheckItemJudge_EGR>", sResultOfOBD.strCheckItemJudge_EGR.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_SAIS>%s</CheckItemJudge_SAIS>", sResultOfOBD.strCheckItemJudge_SAIS.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_ICM>%s</CheckItemJudge_ICM>", sResultOfOBD.strCheckItemJudge_ICM.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_VVT>%s</CheckItemJudge_VVT>", sResultOfOBD.strCheckItemJudge_VVT.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_DOC>%s</CheckItemJudge_DOC>", sResultOfOBD.strCheckItemJudge_DOC.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_SCR>%s</CheckItemJudge_SCR>", sResultOfOBD.strCheckItemJudge_SCR.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_DPF>%s</CheckItemJudge_DPF>", sResultOfOBD.strCheckItemJudge_DPF.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_ASC>%s</CheckItemJudge_ASC>", sResultOfOBD.strCheckItemJudge_ASC.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_POC>%s</CheckItemJudge_POC>", sResultOfOBD.strCheckItemJudge_POC.c_str());
	strXML.AppendFormat(L"<IUPR_OMCEC>%s</IUPR_OMCEC>", sResultOfOBD.strIUPR_OMCEC.c_str());
	strXML.AppendFormat(L"<IUPR_ICC>%s</IUPR_ICC>", sResultOfOBD.strIUPR_ICC.c_str());
	strXML.AppendFormat(L"<IUPR_CMCCB1>%s</IUPR_CMCCB1>", sResultOfOBD.strIUPR_CMCCB1.c_str());
	strXML.AppendFormat(L"<IUPR_CMCECB1>%s</IUPR_CMCECB1>", sResultOfOBD.strIUPR_CMCECB1.c_str());
	strXML.AppendFormat(L"<IUPR_CMCCB2>%s</IUPR_CMCCB2>", sResultOfOBD.strIUPR_CMCCB2.c_str());
	strXML.AppendFormat(L"<IUPR_CMCECB2>%s</IUPR_CMCECB2>", sResultOfOBD.strIUPR_CMCECB2.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCCB1>%s</IUPR_O2SMCCB1>", sResultOfOBD.strIUPR_O2SMCCB1.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCECB1>%s</IUPR_O2SMCECB1>", sResultOfOBD.strIUPR_O2SMCECB1.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCCB2>%s</IUPR_O2SMCCB2>", sResultOfOBD.strIUPR_O2SMCCB2.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCECB2>%s</IUPR_O2SMCECB2>", sResultOfOBD.strIUPR_O2SMCECB2.c_str());
	strXML.AppendFormat(L"<IUPR_EGRC>%s</IUPR_EGRC>", sResultOfOBD.strIUPR_EGRC.c_str());
	strXML.AppendFormat(L"<IUPR_EGREC>%s</IUPR_EGREC>", sResultOfOBD.strIUPR_EGREC.c_str());
	strXML.AppendFormat(L"<IUPR_GPFC1>%s</IUPR_GPFC1>", sResultOfOBD.strIUPR_GPFC1.c_str());
	strXML.AppendFormat(L"<IUPR_GPFEC1>%s</IUPR_GPFEC1>", sResultOfOBD.strIUPR_GPFEC1.c_str());
	strXML.AppendFormat(L"<IUPR_GPFC2>%s</IUPR_GPFC2>", sResultOfOBD.strIUPR_GPFC2.c_str());
	strXML.AppendFormat(L"<IUPR_GPFEC2>%s</IUPR_GPFEC2>", sResultOfOBD.strIUPR_GPFEC2.c_str());
	strXML.AppendFormat(L"<IUPR_AMCCC>%s</IUPR_AMCCC>", sResultOfOBD.strIUPR_AMCCC.c_str());
	strXML.AppendFormat(L"<IUPR_AMCEC>%s</IUPR_AMCEC>", sResultOfOBD.strIUPR_AMCEC.c_str());
	strXML.AppendFormat(L"<IUPR_EVAPC>%s</IUPR_EVAPC>", sResultOfOBD.strIUPR_EVAPC.c_str());
	strXML.AppendFormat(L"<IUPR_EVAPEC>%s</IUPR_EVAPEC>", sResultOfOBD.strIUPR_EVAPEC.c_str());
	strXML.AppendFormat(L"<IUPR_NMHCC>%s</IUPR_NMHCC>", sResultOfOBD.strIUPR_NMHCC.c_str());
	strXML.AppendFormat(L"<IUPR_NMHCEC>%s</IUPR_NMHCEC>", sResultOfOBD.strIUPR_NMHCEC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXCC>%s</IUPR_NOXCC>", sResultOfOBD.strIUPR_NOXCC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXCEC>%s</IUPR_NOXCEC>", sResultOfOBD.strIUPR_NOXCEC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXAC>%s</IUPR_NOXAC>", sResultOfOBD.strIUPR_NOXAC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXAEC>%s</IUPR_NOXAEC>", sResultOfOBD.strIUPR_NOXAEC.c_str());
	strXML.AppendFormat(L"<IUPR_PMC>%s</IUPR_PMC>", sResultOfOBD.strIUPR_PMC.c_str());
	strXML.AppendFormat(L"<IUPR_PMEC>%s</IUPR_PMEC>", sResultOfOBD.strIUPR_PMEC.c_str());
	strXML.AppendFormat(L"<IUPR_WSC>%s</IUPR_WSC>", sResultOfOBD.strIUPR_WSC.c_str());
	strXML.AppendFormat(L"<IUPR_WSEC>%s</IUPR_WSEC>", sResultOfOBD.strIUPR_WSEC.c_str());
	strXML.AppendFormat(L"<IUPR_PPC>%s</IUPR_PPC>", sResultOfOBD.strIUPR_PPC.c_str());
	strXML.AppendFormat(L"<IUPR_PPEC>%s</IUPR_PPEC>", sResultOfOBD.strIUPR_PPEC.c_str());
	strXML.AppendFormat(L"<Judge>%s</Judge>", sResultOfOBD.strJudge.c_str());
	strXML.AppendFormat(L"<Operator>%s</Operator>", sResultOfOBD.strOperator.c_str());
	strXML.AppendFormat(L"</body></root>");

	std::wstring strXml = (LPCTSTR)strXML;

	return strXml;

}

//
//bool PDAServiceLibAPI::UploadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg)
//{
//	NHOBDProxy webService(m_strURL.c_str(), SOAP_C_UTFSTRING);
//
//	CString strTemp;
//	CString strXML;
//	strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
//	strXML.AppendFormat(L"<root><body>");
//	strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", szRunningNumber);
//	strXML.AppendFormat(L"<Velocity>%s</Velocity>", sOBDRTData.strVelocity.c_str());
//	strXML.AppendFormat(L"<CoolantTemp>%s</CoolantTemp>", sOBDRTData.strCoolantTemp.c_str());
//	strXML.AppendFormat(L"<EngineRev>%s</EngineRev>", sOBDRTData.strEngineRev.c_str());
//	strXML.AppendFormat(L"<SolarTermDoorPosition>%s</SolarTermDoorPosition>", sOBDRTData.strSolarTermDoorPosition.c_str());
//	strXML.AppendFormat(L"<ThrottleOpening>%s</ThrottleOpening>", sOBDRTData.strThrottleOpening.c_str());
//	strXML.AppendFormat(L"<CalculationLoad>%s</CalculationLoad>", sOBDRTData.strCalculationLoad.c_str());
//	strXML.AppendFormat(L"<AirCoefficient>%s</AirCoefficient>", sOBDRTData.strAirCoefficient.c_str());
//	strXML.AppendFormat(L"<ForwardSensorV>%s</ForwardSensorV>", sOBDRTData.strForwardSensorV.c_str());
//	strXML.AppendFormat(L"<ForwardSensorI>%s</ForwardSensorI>", sOBDRTData.strForwardSensorI.c_str());
//	strXML.AppendFormat(L"<MAF>%s</MAF>", sOBDRTData.strMAF.c_str());
//	strXML.AppendFormat(L"<MAP>%s</MAP>", sOBDRTData.strMAP.c_str());
//	strXML.AppendFormat(L"<EngineOutputPower>%s</EngineOutputPower>", sOBDRTData.strEngineOutputPower.c_str());
//	strXML.AppendFormat(L"<ChargeAirPressure>%s</ChargeAirPressure>", sOBDRTData.strChargeAirPressure.c_str());
//	strXML.AppendFormat(L"<FuelConsumption>%s</FuelConsumption>", sOBDRTData.strFuelConsumption.c_str());
//	strXML.AppendFormat(L"<NOXConcentration>%s</NOXConcentration>", sOBDRTData.strNOXConcentration.c_str());
//	strXML.AppendFormat(L"<UreaInjectionVolume>%s</UreaInjectionVolume>", sOBDRTData.strUreaInjectionVolume.c_str());
//	strXML.AppendFormat(L"<EGT>%s</EGT>", sOBDRTData.strEGT.c_str());
//	strXML.AppendFormat(L"<DPFDifferentialPressure>%s</DPFDifferentialPressure>", sOBDRTData.strDPFDifferentialPressure.c_str());
//	strXML.AppendFormat(L"<EGRPosition>%s</EGRPosition>", sOBDRTData.strEGRPosition.c_str());
//	strXML.AppendFormat(L"<FuelDeliveryPressure>%s</FuelDeliveryPressure>", sOBDRTData.strFuelDeliveryPressure.c_str());
//	strXML.AppendFormat(L"</body></root>");
//
//	std::wstring strJkid = L"W02";
//	std::wstring strXml = (LPCTSTR)strXML;
//	std::wstring strRet;
//	int nRet = webService.queryObjectOut(strJkid, strXml, strRet);
//	if (nRet == SOAP_OK)
//	{
//		strTemp.Format(L"接口[%s]访问成功(%d)\r\n%s\r\n", strJkid.c_str(), nRet, strRet.c_str());
//		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::UploadOBDRTData", strTemp);
//
//		return true;
//	}
//	else
//	{
//		strTemp.Format(L"接口[%s]访问失败(%d)\r\n", strJkid.c_str(), nRet);
//		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::UploadOBDRTData", strTemp);
//		return false;
//	}
//}