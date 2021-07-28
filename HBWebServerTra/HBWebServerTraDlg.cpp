
// HBWebServerTraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HBWebServerTra.h"
#include "HBWebServerTraDlg.h"
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

#include "gsoap/soapH.h"
#include "gsoap/DeviceSwapIfaceImplServiceSoapBinding.nsmap"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHBWebServerTraDlg 对话框

int CHBWebServerTraDlg::m_nWebServicePort(10022);
bool CHBWebServerTraDlg::m_bWebServiceRun(false);
int CHBWebServerTraDlg::m_nHead(0);
int CHBWebServerTraDlg::m_nTail(0);

struct soap* CHBWebServerTraDlg::m_mapSoap;
SOAP_SOCKET  CHBWebServerTraDlg::m_Queue_Soap[MAX_QUEUE];
pthread_mutex_t  CHBWebServerTraDlg::m_Queue_CS;
pthread_cond_t  CHBWebServerTraDlg::m_Queue_CV;

CHBWebServerTraDlg* g_pCHBWebServerTraDlg = NULL;


CHBWebServerTraDlg::CHBWebServerTraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHBWebServerTraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CHBWebServerTraDlg::~CHBWebServerTraDlg()
{
}

void CHBWebServerTraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHBWebServerTraDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_WEBSERVICE, &CHBWebServerTraDlg::OnBnClickedBtnStartWebservice)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHBWebServerTraDlg 消息处理程序

BOOL CHBWebServerTraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_pCHBWebServerTraDlg = this;
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

void CHBWebServerTraDlg::OnPaint()
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
HCURSOR CHBWebServerTraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHBWebServerTraDlg::OnBnClickedBtnStartWebservice()
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
		CNHCommonAPI::GetHLDFilePath(L"Config", L"HBWebServerTra.ini", wchFilePath);

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


void CHBWebServerTraDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bWebServiceRun)
	{
		AfxMessageBox(L"请先停止服务再退出程序。");
		return;
	}
	CDialogEx::OnClose();
}


void CHBWebServerTraDlg::OnTimer(UINT_PTR nIDEvent)
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
			CNHCommonAPI::GetHLDFilePath(L"Config", L"HBWebServerTra.ini", wchFilePath);
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

void CHBWebServerTraDlg::InitCtrls(void)
{
	CStringW str;
	str.Format(L"%d", m_nWebServicePort);
	GetDlgItem(IDC_ED_WEBSERVICE_PORT)->SetWindowTextW(str);
}

void CHBWebServerTraDlg::LoadConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"HBWebServerTra.ini", wchFilePath);

	CSimpleIni si(wchFilePath);
	m_nWebServicePort = _wtoi(si.GetString(L"WebService", L"Port", L"10022"));
	m_strRebootTime = si.GetString(L"General", L"RebootTime", L"");

	CStringW strGAURL = si.GetString(L"WebService", L"URL", L"http://10.111.102.91:8130/interface/services/deviceSwap");
	char* pchGAURL = CNHCommonAPI::UnicodeToANSI(strGAURL);
	strncpy_s(m_chGAURL, pchGAURL, strlen(pchGAURL));
	if (NULL != pchGAURL)
	{
		free(pchGAURL);
		pchGAURL = NULL;
	}
}

void CHBWebServerTraDlg::Reboot(void)
{
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
	//获取exe程序当前路径   
	extern  CHBWebServerTraApp theApp;
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

UINT CHBWebServerTraDlg::GSoapServerThread(LPVOID pParam)
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
		g_pCHBWebServerTraDlg->GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"启动服务");

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

void CHBWebServerTraDlg::StartWebService(void)
{
	AfxBeginThread(GSoapServerThread, this);
}

void CHBWebServerTraDlg::StopWebService(void)
{
	soap_destroy(m_mapSoap);
	soap_end(m_mapSoap);
	soap_done(m_mapSoap);
}

int CHBWebServerTraDlg::HttpGet(soap* soap)
{
	// 获取wsdl文件
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"deviceSwap.wsdl", wchFilePath);

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

void *  CHBWebServerTraDlg::Process_Queue(void *soap) 
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
int  CHBWebServerTraDlg::Enqueue(SOAP_SOCKET sock) 
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
SOAP_SOCKET  CHBWebServerTraDlg::Dequeue() 
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

std::wstring CHBWebServerTraDlg::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = { 0 };
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"HBWebServerTra测试%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

///////////////////////////具体服务方法////////////////////////////////////////

int __ns1__argChang(struct soap*, ns1__argChang* ns1__argChang_, ns1__argChangResponse& ns1__argChangResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", L"", L"");

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__argChang", strLog);

	int nRet = soap_call___ns1__argChang(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__argChang_, ns1__argChangResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeLog(struct soap*, ns1__lightproofSmokeLog* ns1__lightproofSmokeLog_, ns1__lightproofSmokeLogResponse& ns1__lightproofSmokeLogResponse_) 
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeLog_->businessId->c_str(), ns1__lightproofSmokeLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeLog", strLog);

	int nRet = soap_call___ns1__lightproofSmokeLog(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeLog_, ns1__lightproofSmokeLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__getCarInfo(struct soap*, ns1__getCarInfo* ns1__getCarInfo_, ns1__getCarInfoResponse& ns1__getCarInfoResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__getCarInfo_->businessId->c_str(), ns1__getCarInfo_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__getCarInfo", strLog);

	int nRet = soap_call___ns1__getCarInfo(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__getCarInfo_, ns1__getCarInfoResponse_);

	if (ns1__getCarInfoResponse_.return_->carCardNumber == NULL
		&& ns1__getCarInfoResponse_.return_->airInflow == NULL
		&& ns1__getCarInfoResponse_.return_->fuelType == NULL
		&& ns1__getCarInfoResponse_.return_->isSYJHQ == NULL
		)
	{
		std::wstring strcarCardNumber = L"";
		std::wstring strairInflow = L"";
		std::wstring strfuelType = L"";
		std::wstring strisSYJHQ = L"";
		std::wstring strspeedChanger = L"";
		std::wstring stroilSupply = L"";
		double dmaxWeight = 0.0;
		double dmotorPower = 0.0;
		double dmotorRate = 0.0;
		double dstandardWeight = 0.0;

		ns1__getCarInfoResponse_.return_->carCardNumber = &strcarCardNumber;
		ns1__getCarInfoResponse_.return_->airInflow = &strairInflow;
		ns1__getCarInfoResponse_.return_->fuelType = &strfuelType;
		ns1__getCarInfoResponse_.return_->isSYJHQ = &strisSYJHQ;
		ns1__getCarInfoResponse_.return_->speedChanger = &strspeedChanger;
		ns1__getCarInfoResponse_.return_->oilSupply = &stroilSupply;
		ns1__getCarInfoResponse_.return_->maxWeight = dmaxWeight;
		ns1__getCarInfoResponse_.return_->motorPower = dmotorPower;
		ns1__getCarInfoResponse_.return_->motorRate = dmotorRate;
		ns1__getCarInfoResponse_.return_->standardWeight = dstandardWeight;
	}

	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__sendMessage(struct soap*, ns1__sendMessage* ns1__sendMessage_, ns1__sendMessageResponse& ns1__sendMessageResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__sendMessage_->businessId->c_str(), ns1__sendMessage_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__sendMessage", strLog);

	int nRet = soap_call___ns1__sendMessage(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__sendMessage_, ns1__sendMessageResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__bgAirHC(struct soap*, ns1__bgAirHC* ns1__bgAirHC_, ns1__bgAirHCResponse& ns1__bgAirHCResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__bgAirHC_->businessId->c_str(), ns1__bgAirHC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__bgAirHC", strLog);

	int nRet = soap_call___ns1__bgAirHC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__bgAirHC_, ns1__bgAirHCResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleData_USCOREFC(struct soap*, ns1__doubleIdleData_USCOREFC* ns1__doubleIdleData_USCOREFC_, ns1__doubleIdleDataResponse& ns1__doubleIdleData_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleData_USCOREFC_->businessId->c_str(), ns1__doubleIdleData_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleData_USCOREFC", strLog);

	int nRet = soap_call___ns1__doubleIdleData_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleData_USCOREFC_, ns1__doubleIdleData_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjLASelfcheck(struct soap*, ns1__cgjLASelfcheck* ns1__cgjLASelfcheck_, ns1__cgjLASelfcheckResponse& ns1__cgjLASelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjLASelfcheck_->registCode->c_str(), ns1__cgjLASelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjLASelfcheck", strLog);

	int nRet = soap_call___ns1__cgjLASelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjLASelfcheck_, ns1__cgjLASelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjSelfcheck(struct soap*, ns1__cgjSelfcheck* ns1__cgjSelfcheck_, ns1__cgjSelfcheckResponse& ns1__cgjSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjSelfcheck_->registCode->c_str(), ns1__cgjSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjSelfcheck", strLog);

	int nRet = soap_call___ns1__cgjSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjSelfcheck_, ns1__cgjSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__obdResult(struct soap*, ns1__obdResult* ns1__obdResult_, ns1__obdResultResponse& ns1__obdResultResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__obdResult_->businessId->c_str(), ns1__obdResult_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__obdResult", strLog);

	int nRet = soap_call___ns1__obdResult(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__obdResult_, ns1__obdResultResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleLog_USCOREFC(struct soap*, ns1__doubleIdleLog_USCOREFC* ns1__doubleIdleLog_USCOREFC_, ns1__doubleIdleLogResponse& ns1__doubleIdleLog_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleLog_USCOREFC_->businessId->c_str(), ns1__doubleIdleLog_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleLog_USCOREFC", strLog);

	int nRet = soap_call___ns1__doubleIdleLog_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleLog_USCOREFC_, ns1__doubleIdleLog_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__vmasLog(struct soap*, ns1__vmasLog* ns1__vmasLog_, ns1__vmasLogResponse& ns1__vmasLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__vmasLog_->businessId->c_str(), ns1__vmasLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__vmasLog", strLog);

	int nRet = soap_call___ns1__vmasLog(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__vmasLog_, ns1__vmasLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__ydjSelfcheck(struct soap*, ns1__ydjSelfcheck* ns1__ydjSelfcheck_, ns1__ydjSelfcheckResponse& ns1__ydjSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__ydjSelfcheck_->registCode->c_str(), ns1__ydjSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__ydjSelfcheck", strLog);

	int nRet = soap_call___ns1__ydjSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__ydjSelfcheck_, ns1__ydjSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__dyhwSelfcheck(struct soap*, ns1__dyhwSelfcheck* ns1__dyhwSelfcheck_, ns1__dyhwSelfcheckResponse& ns1__dyhwSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__dyhwSelfcheck_->registCode->c_str(), ns1__dyhwSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__dyhwSelfcheck", strLog);

	int nRet = soap_call___ns1__dyhwSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__dyhwSelfcheck_, ns1__dyhwSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__loadDownLog(struct soap*, ns1__loadDownLog* ns1__loadDownLog_, ns1__loadDownLogResponse& ns1__loadDownLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__loadDownLog_->businessId->c_str(), ns1__loadDownLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__loadDownLog", strLog);

	int nRet = soap_call___ns1__loadDownLog(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__loadDownLog_, ns1__loadDownLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__dyhwAdjust(struct soap*, ns1__dyhwAdjust* ns1__dyhwAdjust_, ns1__dyhwAdjustResponse& ns1__dyhwAdjustResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__dyhwAdjust_->registCode->c_str(), ns1__dyhwAdjust_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__dyhwAdjust", strLog);

	int nRet = soap_call___ns1__dyhwAdjust(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__dyhwAdjust_, ns1__dyhwAdjustResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__obdResultLog(struct soap*, ns1__obdResultLog* ns1__obdResultLog_, ns1__obdResultLogResponse& ns1__obdResultLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__obdResultLog_->businessId->c_str(), ns1__obdResultLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__obdResultLog", strLog);

	int nRet = soap_call___ns1__obdResultLog(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__obdResultLog_, ns1__obdResultLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__getCarInfo_USCOREFC(struct soap*, ns1__getCarInfo_USCOREFC* ns1__getCarInfo_USCOREFC_, ns1__getCarInfoResponse& ns1__getCarInfo_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__getCarInfo_USCOREFC_->businessId->c_str(), ns1__getCarInfo_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__getCarInfo_USCOREFC", strLog);

	int nRet = soap_call___ns1__getCarInfo_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__getCarInfo_USCOREFC_, ns1__getCarInfo_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__software2centerNew_USCOREFC(struct soap*, ns1__software2centerNew_USCOREFC* ns1__software2centerNew_USCOREFC_, ns1__software2centerNew_USCOREFCResponse& ns1__software2centerNew_USCOREFCResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__software2centerNew_USCOREFC_->optionType->c_str(), ns1__software2centerNew_USCOREFC_->dataJson->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__software2centerNew_USCOREFC", strLog);

	int nRet = soap_call___ns1__software2centerNew_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__software2centerNew_USCOREFC_, ns1__software2centerNew_USCOREFCResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cycdyhwAdjust(struct soap*, ns1__cycdyhwAdjust* ns1__cycdyhwAdjust_, ns1__cycdyhwAdjustResponse& ns1__cycdyhwAdjustResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cycdyhwAdjust_->registCode->c_str(), ns1__cycdyhwAdjust_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cycdyhwAdjust", strLog);

	int nRet = soap_call___ns1__cycdyhwAdjust(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cycdyhwAdjust_, ns1__cycdyhwAdjustResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__hjcsgyqSelfcheck(struct soap*, ns1__hjcsgyqSelfcheck* ns1__hjcsgyqSelfcheck_, ns1__hjcsgyqSelfcheckResponse& ns1__hjcsgyqSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);
	
	CString strLog;
	strLog.Format(L"%s->%s->", ns1__hjcsgyqSelfcheck_->registCode->c_str(), ns1__hjcsgyqSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__hjcsgyqSelfcheck", strLog);

	int nRet = soap_call___ns1__hjcsgyqSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__hjcsgyqSelfcheck_, ns1__hjcsgyqSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__pqfxySelfcheck(struct soap*, ns1__pqfxySelfcheck* ns1__pqfxySelfcheck_, ns1__pqfxySelfcheckResponse& ns1__pqfxySelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__pqfxySelfcheck_->registCode->c_str(), ns1__pqfxySelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__pqfxySelfcheck", strLog);

	int nRet = soap_call___ns1__pqfxySelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__pqfxySelfcheck_, ns1__pqfxySelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeDataLog_USCOREFC(struct soap*, ns1__lightproofSmokeDataLog_USCOREFC* ns1__lightproofSmokeDataLog_USCOREFC_, ns1__lightproofSmokeDataLog_USCOREFCResponse& ns1__lightproofSmokeDataLog_USCOREFCResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeDataLog_USCOREFC_->businessId->c_str(), ns1__lightproofSmokeDataLog_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeDataLog_USCOREFC", strLog);

	int nRet = soap_call___ns1__lightproofSmokeDataLog_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeDataLog_USCOREFC_, ns1__lightproofSmokeDataLog_USCOREFCResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__loadDownData(struct soap*, ns1__loadDownData* ns1__loadDownData_, ns1__loadDownDataResponse& ns1__loadDownDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__loadDownData_->businessId->c_str(), ns1__loadDownData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__loadDownData", strLog);

	int nRet = soap_call___ns1__loadDownData(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__loadDownData_, ns1__loadDownDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__vmasData(struct soap*, ns1__vmasData* ns1__vmasData_, ns1__vmasDataResponse& ns1__vmasDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__vmasData_->businessId->c_str(), ns1__vmasData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__vmasData", strLog);

	int nRet = soap_call___ns1__vmasData(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__vmasData_, ns1__vmasDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__motoDbIdleData(struct soap*, ns1__motoDbIdleData* ns1__motoDbIdleData_, ns1__motoDbIdleDataResponse& ns1__motoDbIdleDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__motoDbIdleData_->businessId->c_str(), ns1__motoDbIdleData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__motoDbIdleData", strLog);

	int nRet = soap_call___ns1__motoDbIdleData(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__motoDbIdleData_, ns1__motoDbIdleDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__getSatate(struct soap*, ns1__getSatate* ns1__getSatate_, ns1__getSatateResponse& ns1__getSatateResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__getSatate_->registCode->c_str(), ns1__getSatate_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__getSatate", strLog);

	ns1__getSatateResponse ns1__getRes;

	int nRet = soap_call___ns1__getSatate(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__getSatate_, ns1__getSatateResponse_);


	if (ns1__getSatateResponse_.return_->businessId == NULL
		&& ns1__getSatateResponse_.return_->methodId == NULL
		&& ns1__getSatateResponse_.return_->state == NULL
		)
	{
		std::wstring strbusinessId = L"";
		std::wstring strmethodId = L"";
		std::wstring strstate = L"";
		ns1__getSatateResponse_.return_->businessId = &strbusinessId;
		ns1__getSatateResponse_.return_->methodId = &strmethodId;
		ns1__getSatateResponse_.return_->state = &strstate;
	}

	//if (ns1__getSatateResponse_.return_->businessId->empty())
	//{
	//	std::wstring str = L"2";
	//	*ns1__getSatateResponse_.return_->businessId = str;
	//}

	//if (ns1__getSatateResponse_.return_->methodId->empty())
	//{
	//	std::wstring str = L"2";
	//	*ns1__getSatateResponse_.return_->methodId = str;
	//}

	//if (ns1__getSatateResponse_.return_->state->empty())
	//{
	//	std::wstring str = L"0";
	//	*ns1__getSatateResponse_.return_->state = str;
	//}


	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);
	
	

	return nRet;
}

int __ns1__doubleIdleData(struct soap*, ns1__doubleIdleData* ns1__doubleIdleData_, ns1__doubleIdleDataResponse& ns1__doubleIdleDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleData_->businessId->c_str(), ns1__doubleIdleData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleData", strLog);

	int nRet = soap_call___ns1__doubleIdleData(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleData_, ns1__doubleIdleDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleLog(struct soap*, ns1__doubleIdleLog* ns1__doubleIdleLog_, ns1__doubleIdleLogResponse& ns1__doubleIdleLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleLog_->businessId->c_str(), ns1__doubleIdleLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleLog", strLog);

	int nRet = soap_call___ns1__doubleIdleLog(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleLog_, ns1__doubleIdleLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjPLHPSelfcheck(struct soap*, ns1__cgjPLHPSelfcheck* ns1__cgjPLHPSelfcheck_, ns1__cgjPLHPSelfcheckResponse& ns1__cgjPLHPSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjPLHPSelfcheck_->registCode->c_str(), ns1__cgjPLHPSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjPLHPSelfcheck", strLog);

	int nRet = soap_call___ns1__cgjPLHPSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjPLHPSelfcheck_, ns1__cgjPLHPSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__wqfxyAdjust(struct soap*, ns1__wqfxyAdjust* ns1__wqfxyAdjust_, ns1__wqfxyAdjustResponse& ns1__wqfxyAdjustResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__wqfxyAdjust_->registCode->c_str(), ns1__wqfxyAdjust_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__wqfxyAdjust", strLog);

	int nRet = soap_call___ns1__wqfxyAdjust(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__wqfxyAdjust_, ns1__wqfxyAdjustResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeData_USCOREFC(struct soap*, ns1__lightproofSmokeData_USCOREFC* ns1__lightproofSmokeData_USCOREFC_, ns1__lightproofSmokeDataResponse& ns1__lightproofSmokeData_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeData_USCOREFC_->businessId->c_str(), ns1__lightproofSmokeData_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeData_USCOREFC", strLog);

	int nRet = soap_call___ns1__lightproofSmokeData_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeData_USCOREFC_, ns1__lightproofSmokeData_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeData(struct soap*, ns1__lightproofSmokeData* ns1__lightproofSmokeData_, ns1__lightproofSmokeDataResponse& ns1__lightproofSmokeDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeData_->businessId->c_str(), ns1__lightproofSmokeData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeData", strLog);

	int nRet = soap_call___ns1__lightproofSmokeData(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeData_, ns1__lightproofSmokeDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__qyfxSelfcheck(struct soap*, ns1__qyfxSelfcheck* ns1__qyfxSelfcheck_, ns1__qyfxSelfcheckResponse& ns1__qyfxSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__qyfxSelfcheck_->registCode->c_str(), ns1__qyfxSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__qyfxSelfcheck", strLog);

	int nRet = soap_call___ns1__qyfxSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__qyfxSelfcheck_, ns1__qyfxSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__sdsqtfxySelfcheck(struct soap*, ns1__sdsqtfxySelfcheck* ns1__sdsqtfxySelfcheck_, ns1__sdsqtfxySelfcheckResponse& ns1__sdsqtfxySelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__sdsqtfxySelfcheck_->registCode->c_str(), ns1__sdsqtfxySelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__sdsqtfxySelfcheck", strLog);

	int nRet = soap_call___ns1__sdsqtfxySelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__sdsqtfxySelfcheck_, ns1__sdsqtfxySelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__sendMessage_USCOREFC(struct soap*, ns1__sendMessage_USCOREFC* ns1__sendMessage_USCOREFC_, ns1__sendMessageResponse& ns1__sendMessage_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__sendMessage_USCOREFC_->businessId->c_str(), ns1__sendMessage_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__sendMessage_USCOREFC", strLog);

	int nRet = soap_call___ns1__sendMessage_USCOREFC(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__sendMessage_USCOREFC_, ns1__sendMessage_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__wqfxySelfcheck(struct soap*, ns1__wqfxySelfcheck* ns1__wqfxySelfcheck_, ns1__wqfxySelfcheckResponse& ns1__wqfxySelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__wqfxySelfcheck_->registCode->c_str(), ns1__wqfxySelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__wqfxySelfcheck", strLog);

	int nRet = soap_call___ns1__wqfxySelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__wqfxySelfcheck_, ns1__wqfxySelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjALPSelfcheck(struct soap*, ns1__cgjALPSelfcheck* ns1__cgjALPSelfcheck_, ns1__cgjALPSelfcheckResponse& ns1__cgjALPSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjALPSelfcheck_->registCode->c_str(), ns1__cgjALPSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjALPSelfcheck", strLog);

	int nRet = soap_call___ns1__cgjALPSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjALPSelfcheck_, ns1__cgjALPSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lljSelfcheck(struct soap*, ns1__lljSelfcheck* ns1__lljSelfcheck_, ns1__lljSelfcheckResponse& ns1__lljSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lljSelfcheck_->registCode->c_str(), ns1__lljSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lljSelfcheck", strLog);

	int nRet = soap_call___ns1__lljSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lljSelfcheck_, ns1__lljSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjLSPSelfcheck(struct soap*, ns1__cgjLSPSelfcheck* ns1__cgjLSPSelfcheck_, ns1__cgjLSPSelfcheckResponse& ns1__cgjLSPSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjLSPSelfcheck_->registCode->c_str(), ns1__cgjLSPSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjLSPSelfcheck", strLog);

	int nRet = soap_call___ns1__cgjLSPSelfcheck(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjLSPSelfcheck_, ns1__cgjLSPSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__argChang_(struct soap*, ns1__argChang* ns1__argChang_, ns1__argChangResponse& ns1__argChangResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__argChang_->registCode->c_str(), ns1__argChang_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__argChang_", strLog);

	int nRet = soap_call___ns1__argChang_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__argChang_, ns1__argChangResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeLog_(struct soap*, ns1__lightproofSmokeLog* ns1__lightproofSmokeLog_, ns1__lightproofSmokeLogResponse& ns1__lightproofSmokeLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeLog_->businessId->c_str(), ns1__lightproofSmokeLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeLog_", strLog);

	int nRet = soap_call___ns1__lightproofSmokeLog_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeLog_, ns1__lightproofSmokeLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__getCarInfo_(struct soap*, ns1__getCarInfo* ns1__getCarInfo_, ns1__getCarInfoResponse& ns1__getCarInfoResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__getCarInfo_->businessId->c_str(), ns1__getCarInfo_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__getCarInfo_", strLog);

	int nRet = soap_call___ns1__getCarInfo_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__getCarInfo_, ns1__getCarInfoResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__sendMessage_(struct soap*, ns1__sendMessage* ns1__sendMessage_, ns1__sendMessageResponse& ns1__sendMessageResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__sendMessage_->businessId->c_str(), ns1__sendMessage_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__sendMessage_", strLog);

	int nRet = soap_call___ns1__sendMessage_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__sendMessage_, ns1__sendMessageResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__bgAirHC_(struct soap*, ns1__bgAirHC* ns1__bgAirHC_, ns1__bgAirHCResponse& ns1__bgAirHCResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__bgAirHC_->businessId->c_str(), ns1__bgAirHC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__bgAirHC_", strLog);

	int nRet = soap_call___ns1__bgAirHC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__bgAirHC_, ns1__bgAirHCResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleData_USCOREFC_(struct soap*, ns1__doubleIdleData_USCOREFC* ns1__doubleIdleData_USCOREFC_, ns1__doubleIdleDataResponse& ns1__doubleIdleData_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleData_USCOREFC_->businessId->c_str(), ns1__doubleIdleData_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleData_USCOREFC_", strLog);

	int nRet = soap_call___ns1__doubleIdleData_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleData_USCOREFC_, ns1__doubleIdleData_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjLASelfcheck_(struct soap*, ns1__cgjLASelfcheck* ns1__cgjLASelfcheck_, ns1__cgjLASelfcheckResponse& ns1__cgjLASelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjLASelfcheck_->registCode->c_str(), ns1__cgjLASelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjLASelfcheck_", strLog);

	int nRet = soap_call___ns1__cgjLASelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjLASelfcheck_, ns1__cgjLASelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjSelfcheck_(struct soap*, ns1__cgjSelfcheck* ns1__cgjSelfcheck_, ns1__cgjSelfcheckResponse& ns1__cgjSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjSelfcheck_->registCode->c_str(), ns1__cgjSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjSelfcheck_", strLog);

	int nRet = soap_call___ns1__cgjSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjSelfcheck_, ns1__cgjSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__obdResult_(struct soap*, ns1__obdResult* ns1__obdResult_, ns1__obdResultResponse& ns1__obdResultResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__obdResult_->businessId->c_str(), ns1__obdResult_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__obdResult_", strLog);

	int nRet = soap_call___ns1__obdResult_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__obdResult_, ns1__obdResultResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleLog_USCOREFC_(struct soap*, ns1__doubleIdleLog_USCOREFC* ns1__doubleIdleLog_USCOREFC_, ns1__doubleIdleLogResponse& ns1__doubleIdleLog_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleLog_USCOREFC_->businessId->c_str(), ns1__doubleIdleLog_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleLog_USCOREFC_", strLog);

	int nRet = soap_call___ns1__doubleIdleLog_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleLog_USCOREFC_, ns1__doubleIdleLog_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__vmasLog_(struct soap*, ns1__vmasLog* ns1__vmasLog_, ns1__vmasLogResponse& ns1__vmasLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__vmasLog_->businessId->c_str(), ns1__vmasLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__vmasLog_", strLog);

	int nRet = soap_call___ns1__vmasLog_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__vmasLog_, ns1__vmasLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__ydjSelfcheck_(struct soap*, ns1__ydjSelfcheck* ns1__ydjSelfcheck_, ns1__ydjSelfcheckResponse& ns1__ydjSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__ydjSelfcheck_->registCode->c_str(), ns1__ydjSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__ydjSelfcheck_", strLog);

	int nRet = soap_call___ns1__ydjSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__ydjSelfcheck_, ns1__ydjSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__dyhwSelfcheck_(struct soap*, ns1__dyhwSelfcheck* ns1__dyhwSelfcheck_, ns1__dyhwSelfcheckResponse& ns1__dyhwSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__dyhwSelfcheck_->registCode->c_str(), ns1__dyhwSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__dyhwSelfcheck_", strLog);

	int nRet = soap_call___ns1__dyhwSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__dyhwSelfcheck_, ns1__dyhwSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__loadDownLog_(struct soap*, ns1__loadDownLog* ns1__loadDownLog_, ns1__loadDownLogResponse& ns1__loadDownLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__loadDownLog_->businessId->c_str(), ns1__loadDownLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__loadDownLog_", strLog);

	int nRet = soap_call___ns1__loadDownLog_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__loadDownLog_, ns1__loadDownLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__dyhwAdjust_(struct soap*, ns1__dyhwAdjust* ns1__dyhwAdjust_, ns1__dyhwAdjustResponse& ns1__dyhwAdjustResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__dyhwAdjust_->registCode->c_str(), ns1__dyhwAdjust_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__dyhwAdjust_", strLog);

	int nRet = soap_call___ns1__dyhwAdjust_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__dyhwAdjust_, ns1__dyhwAdjustResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__obdResultLog_(struct soap*, ns1__obdResultLog* ns1__obdResultLog_, ns1__obdResultLogResponse& ns1__obdResultLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__obdResultLog_->businessId->c_str(), ns1__obdResultLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__obdResultLog_", strLog);

	int nRet = soap_call___ns1__obdResultLog_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__obdResultLog_, ns1__obdResultLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__getCarInfo_USCOREFC_(struct soap*, ns1__getCarInfo_USCOREFC* ns1__getCarInfo_USCOREFC_, ns1__getCarInfoResponse& ns1__getCarInfo_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__getCarInfo_USCOREFC_->businessId->c_str(), ns1__getCarInfo_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__getCarInfo_USCOREFC_", strLog);

	int nRet = soap_call___ns1__getCarInfo_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__getCarInfo_USCOREFC_, ns1__getCarInfo_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__software2centerNew_USCOREFC_(struct soap*, ns1__software2centerNew_USCOREFC* ns1__software2centerNew_USCOREFC_, ns1__software2centerNew_USCOREFCResponse& ns1__software2centerNew_USCOREFCResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__software2centerNew_USCOREFC_->optionType->c_str(), ns1__software2centerNew_USCOREFC_->dataJson->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__software2centerNew_USCOREFC_", strLog);

	int nRet = soap_call___ns1__software2centerNew_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__software2centerNew_USCOREFC_, ns1__software2centerNew_USCOREFCResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cycdyhwAdjust_(struct soap*, ns1__cycdyhwAdjust* ns1__cycdyhwAdjust_, ns1__cycdyhwAdjustResponse& ns1__cycdyhwAdjustResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cycdyhwAdjust_->registCode->c_str(), ns1__cycdyhwAdjust_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cycdyhwAdjust_", strLog);

	int nRet = soap_call___ns1__cycdyhwAdjust_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cycdyhwAdjust_, ns1__cycdyhwAdjustResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__hjcsgyqSelfcheck_(struct soap*, ns1__hjcsgyqSelfcheck* ns1__hjcsgyqSelfcheck_, ns1__hjcsgyqSelfcheckResponse& ns1__hjcsgyqSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__hjcsgyqSelfcheck_->registCode->c_str(), ns1__hjcsgyqSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__hjcsgyqSelfcheck_", strLog);

	int nRet = soap_call___ns1__hjcsgyqSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__hjcsgyqSelfcheck_, ns1__hjcsgyqSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__pqfxySelfcheck_(struct soap*, ns1__pqfxySelfcheck* ns1__pqfxySelfcheck_, ns1__pqfxySelfcheckResponse& ns1__pqfxySelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__pqfxySelfcheck_->registCode->c_str(), ns1__pqfxySelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__pqfxySelfcheck_", strLog);

	int nRet = soap_call___ns1__pqfxySelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__pqfxySelfcheck_, ns1__pqfxySelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeDataLog_USCOREFC_(struct soap*, ns1__lightproofSmokeDataLog_USCOREFC* ns1__lightproofSmokeDataLog_USCOREFC_, ns1__lightproofSmokeDataLog_USCOREFCResponse& ns1__lightproofSmokeDataLog_USCOREFCResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeDataLog_USCOREFC_->businessId->c_str(), ns1__lightproofSmokeDataLog_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeDataLog_USCOREFC_", strLog);

	int nRet = soap_call___ns1__lightproofSmokeDataLog_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeDataLog_USCOREFC_, ns1__lightproofSmokeDataLog_USCOREFCResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__loadDownData_(struct soap*, ns1__loadDownData* ns1__loadDownData_, ns1__loadDownDataResponse& ns1__loadDownDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__loadDownData_->businessId->c_str(), ns1__loadDownData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__loadDownData_", strLog);

	int nRet = soap_call___ns1__loadDownData_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__loadDownData_, ns1__loadDownDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__vmasData_(struct soap*, ns1__vmasData* ns1__vmasData_, ns1__vmasDataResponse& ns1__vmasDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__vmasData_->businessId->c_str(), ns1__vmasData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__vmasData_", strLog);

	int nRet = soap_call___ns1__vmasData_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__vmasData_, ns1__vmasDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__motoDbIdleData_(struct soap*, ns1__motoDbIdleData* ns1__motoDbIdleData_, ns1__motoDbIdleDataResponse& ns1__motoDbIdleDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__motoDbIdleData_->businessId->c_str(), ns1__motoDbIdleData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__motoDbIdleData_", strLog);

	int nRet = soap_call___ns1__motoDbIdleData_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__motoDbIdleData_, ns1__motoDbIdleDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__getSatate_(struct soap*, ns1__getSatate* ns1__getSatate_, ns1__getSatateResponse& ns1__getSatateResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__getSatate_->registCode->c_str(), ns1__getSatate_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__getSatate_", strLog);

	int nRet = soap_call___ns1__getSatate_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__getSatate_, ns1__getSatateResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleData_(struct soap*, ns1__doubleIdleData* ns1__doubleIdleData_, ns1__doubleIdleDataResponse& ns1__doubleIdleDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleData_->businessId->c_str(), ns1__doubleIdleData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleData_", strLog);

	int nRet = soap_call___ns1__doubleIdleData_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleData_, ns1__doubleIdleDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__doubleIdleLog_(struct soap*, ns1__doubleIdleLog* ns1__doubleIdleLog_, ns1__doubleIdleLogResponse& ns1__doubleIdleLogResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__doubleIdleLog_->businessId->c_str(), ns1__doubleIdleLog_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__doubleIdleLog_", strLog);

	int nRet = soap_call___ns1__doubleIdleLog_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__doubleIdleLog_, ns1__doubleIdleLogResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjPLHPSelfcheck_(struct soap*, ns1__cgjPLHPSelfcheck* ns1__cgjPLHPSelfcheck_, ns1__cgjPLHPSelfcheckResponse& ns1__cgjPLHPSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjPLHPSelfcheck_->registCode->c_str(), ns1__cgjPLHPSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjPLHPSelfcheck_", strLog);

	int nRet = soap_call___ns1__cgjPLHPSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjPLHPSelfcheck_, ns1__cgjPLHPSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__wqfxyAdjust_(struct soap*, ns1__wqfxyAdjust* ns1__wqfxyAdjust_, ns1__wqfxyAdjustResponse& ns1__wqfxyAdjustResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__wqfxyAdjust_->registCode->c_str(), ns1__wqfxyAdjust_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__wqfxyAdjust_", strLog);

	int nRet = soap_call___ns1__wqfxyAdjust_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__wqfxyAdjust_, ns1__wqfxyAdjustResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeData_USCOREFC_(struct soap*, ns1__lightproofSmokeData_USCOREFC* ns1__lightproofSmokeData_USCOREFC_, ns1__lightproofSmokeDataResponse& ns1__lightproofSmokeData_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeData_USCOREFC_->businessId->c_str(), ns1__lightproofSmokeData_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeData_USCOREFC_", strLog);

	int nRet = soap_call___ns1__lightproofSmokeData_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeData_USCOREFC_, ns1__lightproofSmokeData_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lightproofSmokeData_(struct soap*, ns1__lightproofSmokeData* ns1__lightproofSmokeData_, ns1__lightproofSmokeDataResponse& ns1__lightproofSmokeDataResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lightproofSmokeData_->businessId->c_str(), ns1__lightproofSmokeData_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lightproofSmokeData_", strLog);

	int nRet = soap_call___ns1__lightproofSmokeData_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lightproofSmokeData_, ns1__lightproofSmokeDataResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__qyfxSelfcheck_(struct soap*, ns1__qyfxSelfcheck* ns1__qyfxSelfcheck_, ns1__qyfxSelfcheckResponse& ns1__qyfxSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__qyfxSelfcheck_->registCode->c_str(), ns1__qyfxSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__qyfxSelfcheck_", strLog);

	int nRet = soap_call___ns1__qyfxSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__qyfxSelfcheck_, ns1__qyfxSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__sdsqtfxySelfcheck_(struct soap*, ns1__sdsqtfxySelfcheck* ns1__sdsqtfxySelfcheck_, ns1__sdsqtfxySelfcheckResponse& ns1__sdsqtfxySelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__sdsqtfxySelfcheck_->registCode->c_str(), ns1__sdsqtfxySelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__sdsqtfxySelfcheck_", strLog);

	int nRet = soap_call___ns1__sdsqtfxySelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__sdsqtfxySelfcheck_, ns1__sdsqtfxySelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__sendMessage_USCOREFC_(struct soap*, ns1__sendMessage_USCOREFC* ns1__sendMessage_USCOREFC_, ns1__sendMessageResponse& ns1__sendMessage_USCOREFCResponse)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__sendMessage_USCOREFC_->businessId->c_str(), ns1__sendMessage_USCOREFC_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__sendMessage_USCOREFC_", strLog);

	int nRet = soap_call___ns1__sendMessage_USCOREFC_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__sendMessage_USCOREFC_, ns1__sendMessage_USCOREFCResponse);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__wqfxySelfcheck_(struct soap*, ns1__wqfxySelfcheck* ns1__wqfxySelfcheck_, ns1__wqfxySelfcheckResponse& ns1__wqfxySelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__wqfxySelfcheck_->registCode->c_str(), ns1__wqfxySelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__wqfxySelfcheck_", strLog);

	int nRet = soap_call___ns1__wqfxySelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__wqfxySelfcheck_, ns1__wqfxySelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjALPSelfcheck_(struct soap*, ns1__cgjALPSelfcheck* ns1__cgjALPSelfcheck_, ns1__cgjALPSelfcheckResponse& ns1__cgjALPSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjALPSelfcheck_->registCode->c_str(), ns1__cgjALPSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjALPSelfcheck_", strLog);

	int nRet = soap_call___ns1__cgjALPSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjALPSelfcheck_, ns1__cgjALPSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__lljSelfcheck_(struct soap*, ns1__lljSelfcheck* ns1__lljSelfcheck_, ns1__lljSelfcheckResponse& ns1__lljSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__lljSelfcheck_->registCode->c_str(), ns1__lljSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__lljSelfcheck_", strLog);

	int nRet = soap_call___ns1__lljSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__lljSelfcheck_, ns1__lljSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__cgjLSPSelfcheck_(struct soap*, ns1__cgjLSPSelfcheck* ns1__cgjLSPSelfcheck_, ns1__cgjLSPSelfcheckResponse& ns1__cgjLSPSelfcheckResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	CString strLog;
	strLog.Format(L"%s->%s->", ns1__cgjLSPSelfcheck_->registCode->c_str(), ns1__cgjLSPSelfcheck_->registCode->c_str());

	CNHLogAPI::WriteLogEx(g_pCHBWebServerTraDlg->LogFilePath().c_str(), L"接收", L"__ns1__cgjLSPSelfcheck_", strLog);

	int nRet = soap_call___ns1__cgjLSPSelfcheck_(&soapClient, g_pCHBWebServerTraDlg->m_chGAURL, NULL, ns1__cgjLSPSelfcheck_, ns1__cgjLSPSelfcheckResponse_);

	soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}