
// ACHBTra_2.0Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ACHBTra_2.0.h"
#include "ACHBTra_2.0Dlg.h"
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

// ���ÿ�(MFC��չDLL)
#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "gsoap/soapH.h"
#include "gsoap/WSServiceImplServiceSoapBinding.nsmap"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACHBTra_20Dlg �Ի���

int CACHBTra_20Dlg::m_nWebServicePort(10022);
bool CACHBTra_20Dlg::m_bWebServiceRun(false);
int CACHBTra_20Dlg::m_nHead(0);
int CACHBTra_20Dlg::m_nTail(0);

struct soap* CACHBTra_20Dlg::m_mapSoap;
SOAP_SOCKET  CACHBTra_20Dlg::m_Queue_Soap[MAX_QUEUE];
pthread_mutex_t  CACHBTra_20Dlg::m_Queue_CS;
pthread_cond_t  CACHBTra_20Dlg::m_Queue_CV;

CACHBTra_20Dlg* g_pCACHBTraDlg = NULL;


CACHBTra_20Dlg::CACHBTra_20Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CACHBTra_20Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CACHBTra_20Dlg::~CACHBTra_20Dlg()
{
}

void CACHBTra_20Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CACHBTra_20Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_WEBSERVICE, &CACHBTra_20Dlg::OnBnClickedBtnStartWebservice)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CACHBTra_20Dlg ��Ϣ�������

BOOL CACHBTra_20Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	g_pCACHBTraDlg = this;
	// ��������
	LoadConfig();

	// ��ʼ���ؼ�
	InitCtrls();
	// ��ʱ��������
	SetTimer(TID_INIT_RUN, m_unTI_InitRun, NULL);
	// �Զ�����
	SetTimer(TID_REBOOT, m_unTI_Reboot, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CACHBTra_20Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CACHBTra_20Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CACHBTra_20Dlg::OnBnClickedBtnStartWebservice()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_BTN_START_WEBSERVICE)->GetWindowTextW(str);

	if (L"��������" == str)
	{
		CString strPort;
		GetDlgItem(IDC_ED_WEBSERVICE_PORT)->GetWindowTextW(strPort);

		if (L"" == strPort || L"0" == strPort)
		{
			AfxMessageBox(L"������WebService�˿ڡ�");
			GetDlgItem(IDC_ED_WEBSERVICE_PORT)->SetFocus();
			return;
		}

		// ��������
		wchar_t wchFilePath[MAX_PATH] = { L'\0' };
		CNHCommonAPI::GetHLDFilePath(L"Config", L"HBWebServerTra.ini", wchFilePath);

		CSimpleIni si(wchFilePath);
		si.SetString(L"WebService", L"Port", strPort);

		m_nWebServicePort = _wtoi(strPort);

		StartWebService();

		GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"ֹͣ����");
	}
	else
	{
		StopWebService();
		m_bWebServiceRun = false;
		GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"��������");
	}
}


void CACHBTra_20Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bWebServiceRun)
	{
		AfxMessageBox(L"����ֹͣ�������˳�����");
		return;
	}
	CDialogEx::OnClose();
}


void CACHBTra_20Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
			LoadConfig(); // ���»�ȡ����ʱ��
			if (strNow != m_strRebootTime
				&& nHour == 7 && nMinute >= 5 && nMinute <= 10) // ÿ��7��5��ǰ�ж��Ƿ���Ҫ����
			{
				m_strRebootTime = strNow;
				// ������ʱ��д�������ļ�
				wchar_t wchFilePath[MAX_PATH] = { L'\0' };
				CNHCommonAPI::GetHLDFilePath(L"Config", L"HBWebServerTra.ini", wchFilePath);
				CSimpleIni si(wchFilePath);
				si.SetString(L"General", L"RebootTime", m_strRebootTime);
				// �ȹرն˿�������
				OnBnClickedBtnStartWebservice();

				// ����
				Reboot();
			}

			SetTimer(TID_REBOOT, m_unTI_Reboot, NULL);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CACHBTra_20Dlg::InitCtrls(void)
{
	CStringW str;
	str.Format(L"%d", m_nWebServicePort);
	GetDlgItem(IDC_ED_WEBSERVICE_PORT)->SetWindowTextW(str);

	m_cbDelType.AddString(L"");
	m_cbDelType.AddString(L"����˲̬");
	m_cbDelType.AddString(L"��̬");
	m_cbDelType.AddString(L"˫����");
	m_cbDelType.AddString(L"���ؼ���");
	m_cbDelType.AddString(L"���ɼ���");
}

void CACHBTra_20Dlg::LoadConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"HBWebServerTra.ini", wchFilePath);

	CSimpleIni si(wchFilePath);
	m_nWebServicePort = _wtoi(si.GetString(L"WebService", L"Port", L"10022"));
	m_strRebootTime = si.GetString(L"General", L"RebootTime", L"");
	m_bIsLog = (si.GetString(L"General", L"IsLog", L"1") == L"1") ? true: false;


	CStringW strGAURL = si.GetString(L"WebService", L"URL", L"http://10.111.102.91:8130/interface/services/deviceSwap");
	char* pchGAURL = CNHCommonAPI::UnicodeToANSI(strGAURL);
	strncpy_s(m_chGAURL, pchGAURL, strlen(pchGAURL));
	if (NULL != pchGAURL)
	{
		free(pchGAURL);
		pchGAURL = NULL;
	}
}

void CACHBTra_20Dlg::Reboot(void)
{
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
	//��ȡexe����ǰ·��   
	extern  CACHBTra_20App theApp;
	TCHAR szAppName[MAX_PATH];
	::GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
	CString strAppFullName;
	strAppFullName.Format(_T("%s"), szAppName);
	//��������   
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

UINT CACHBTra_20Dlg::GSoapServerThread(LPVOID pParam)
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

		AfxMessageBox(L"�˿��ѱ�ռ�ã���������ʧ�ܡ�");
		g_pCACHBTraDlg->GetDlgItem(IDC_BTN_START_WEBSERVICE)->SetWindowTextW(L"��������");

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

void CACHBTra_20Dlg::StartWebService(void)
{
	AfxBeginThread(GSoapServerThread, this);
}

void CACHBTra_20Dlg::StopWebService(void)
{
	soap_destroy(m_mapSoap);
	soap_end(m_mapSoap);
	soap_done(m_mapSoap);
}

int CACHBTra_20Dlg::HttpGet(soap* soap)
{
	// ��ȡwsdl�ļ�
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"VeptsOutAccess.wsdl", wchFilePath);

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

void *  CACHBTra_20Dlg::Process_Queue(void *soap) 
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
int  CACHBTra_20Dlg::Enqueue(SOAP_SOCKET sock) 
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
SOAP_SOCKET  CACHBTra_20Dlg::Dequeue() 
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
// //��ʼ��һ����΢��Ϊ��λ��ʱ������
int randEx()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}

std::wstring CACHBTra_20Dlg::LogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = { 0 };
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"HBWebServerTra����%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

std::wstring CACHBTra_20Dlg::ModXml(const std::wstring* strUTF8XmlDoc, bool& bMod)
{
	bMod = false;
	// ���Ĵ����URL
	//CURLCode::EncodeW();
	// URL���������
	//CURLCode::DecodeW();
	std::wstring strXml;
	strXml = *strUTF8XmlDoc;

	CString strReport, strDelType;
	//GetDlgItem(IDC_ED_REPORT_NUMBER)->GetWindowTextW(strReport);
	//m_cbDelType.GetWindowTextW(strDelType);
	CString strSql;
	strSql.Format(L"select * from WhiteModifiedParameters");

	std::vector<SWhiteModifiedParameters> vtWhiteModifiedParameters;
	GetDboWhiteModifiedParameters(strSql, vtWhiteModifiedParameters);

	if (vtWhiteModifiedParameters.size() > 0)
	{
		std::wstring strJkid;
		// �Ƚ���
		CXmlReader xmlReader;
		if (xmlReader.Parse(strXml.c_str()))
		{
			if (xmlReader.OpenNode(L"root/head/jkid"))
			{
				xmlReader.GetNodeContent(strJkid);
			}

			// ��OBD�������ݲŴ���
			if (strJkid == L"GC011")
			{
				bMod = true;
				CString strTemp = strXml.c_str();
				std::wstring strCon;
				int ncyds(0);
				bool bIdeReNum(false);
				//1. jylsh varchar2(50) not null ������ˮ��
				if (xmlReader.OpenNode(L"root/body/vehispara/jylsh"))
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						for (UINT i=0; i<vtWhiteModifiedParameters.size(); i++)
						{
							if (strCon.find(vtWhiteModifiedParameters[i].strA) != std::string::npos)
							{
								bIdeReNum = true;
								strReport = vtWhiteModifiedParameters[i].strA.c_str();
								strDelType = vtWhiteModifiedParameters[i].strE.c_str();
								break;
							}
						}

						//if (strCon.find(strReport) != std::string::npos)
						//{
						//	bIdeReNum = true;
						//}
					}
				}
				//3. cyds number(8) not null �����ܵ���
				if (xmlReader.OpenNode(L"root/body/vehispara/cyds") && bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						ncyds = _wtoi(strCon.c_str());
					}
				}
				//6. jqmkd varchar2(4000) null �����ž��Կ��� % 
				if (xmlReader.OpenNode(L"root/body/vehispara/jqmkd")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<jqmkd>%s</jqmkd>", strCon.c_str());
						strTemp.Replace(str, L"<jqmkd></jqmkd>");
					}
				}
				//7. fh varchar2(4000) null ���㸺��ֵ % 
				if (xmlReader.OpenNode(L"root/body/vehispara/fh")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<fh>%s</fh>", strCon.c_str());
						strTemp.Replace(str, L"<fh></fh>");
					}
				}
				//8. qycgqxh varchar2(4000) null ǰ���������ź� mV/mA 
				if (xmlReader.OpenNode(L"root/body/vehispara/qycgqxh")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<qycgqxh>%s</qycgqxh>", strCon.c_str());
						strTemp.Replace(str, L"<qycgqxh></qycgqxh>");
					}
				}
				//9. glkqxs varchar2(4000) null ��������ϵ�����ˣ� 
				if (xmlReader.OpenNode(L"root/body/vehispara/glkqxs")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<glkqxs>%s</glkqxs>", strCon.c_str());
						strTemp.Replace(str, L"<glkqxs></glkqxs>");
					}
				}
				//10. ymkd varchar2(4000) null ���ſ��� % 
				//if (xmlReader.OpenNode(L"root/body/vehispara/ymkd")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<ymkd>%s</ymkd>", strCon.c_str());
				//		strTemp.Replace(str, L"<ymkd></ymkd>");
				//	}
				//}
				//11. cs varchar2(4000) null ���� km/h 
				if (xmlReader.OpenNode(L"root/body/vehispara/cs")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<cs>%s</cs>", strCon.c_str());
						strTemp.Replace(str, L"<cs></cs>");
					}
				}
				//12. fdjzs varchar2(4000) null ������ת�� r/min 
				if (xmlReader.OpenNode(L"root/body/vehispara/fdjzs")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<fdjzs>%s</fdjzs>", strCon.c_str());
						strTemp.Replace(str, L"<fdjzs></fdjzs>");
					}
				}
				//13. jql varchar2(4000) null ������ g/s 
				if (xmlReader.OpenNode(L"root/body/vehispara/jql")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<jql>%s</jql>", strCon.c_str());
						strTemp.Replace(str, L"<jql></jql>");
					}
				}
				//14. jqyl varchar2(4000) null ����ѹ�� kPa 
				if (xmlReader.OpenNode(L"root/body/vehispara/jqyl")&& bIdeReNum)
				{
					xmlReader.GetNodeContent(strCon);
					if (!strCon.empty())
					{
						CString str;
						str.Format(L"<jqyl>%s</jqyl>", strCon.c_str());
						strTemp.Replace(str, L"<jqyl></jqyl>");
					}
				}
				//15. scgl varchar2(4000) null ������������� kw 
				//if (xmlReader.OpenNode(L"root/body/vehispara/scgl")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<scgl>%s</scgl>", strCon.c_str());
				//		strTemp.Replace(str, L"<scgl></scgl>");
				//	}
				//}
				//16. zyyl varchar2(4000) null ��ѹѹ�� kPa 
				//if (xmlReader.OpenNode(L"root/body/vehispara/zyyl")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<zyyl>%s</zyyl>", strCon.c_str());
				//		strTemp.Replace(str, L"<zyyl></zyyl>");
				//	}
				//}
				//17. hyl varchar2(4000) null ������ L/100km 
				//if (xmlReader.OpenNode(L"root/body/vehispara/hyl")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<hyl>%s</hyl>", strCon.c_str());
				//		strTemp.Replace(str, L"<hyl></hyl>");
				//	}
				//}
				//18. no varchar2(4000) null ����������Ũ�� ppm 
				//if (xmlReader.OpenNode(L"root/body/vehispara/no")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<no>%s</no>", strCon.c_str());
				//		strTemp.Replace(str, L"<no></no>");
				//	}
				//}
				//19. nsbsl varchar2(4000) null ���������� L/h 
				//if (xmlReader.OpenNode(L"root/body/vehispara/nsbsl")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<nsbsl>%s</nsbsl>", strCon.c_str());
				//		strTemp.Replace(str, L"<nsbsl></nsbsl>");
				//	}
				//}
				//20. pqwd varchar2(4000) null �����¶� ��
				//if (xmlReader.OpenNode(L"root/body/vehispara/pqwd")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<pqwd>%s</pqwd>", strCon.c_str());
				//		strTemp.Replace(str, L"<pqwd></pqwd>");
				//	}
				//}
				//21. klbjqyc varchar2(4000) null ����������ѹ�� kpa 
				//if (xmlReader.OpenNode(L"root/body/vehispara/klbjqyc")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<klbjqyc>%s</klbjqyc>", strCon.c_str());
				//		strTemp.Replace(str, L"<klbjqyc></klbjqyc>");
				//	}
				//}
				//22. egrkd varchar2(4000) null EGR ���� % 
				//if (xmlReader.OpenNode(L"root/body/vehispara/egrkd")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<egrkd>%s</egrkd>", strCon.c_str());
				//		strTemp.Replace(str, L"<egrkd></egrkd>");
				//	}
				//}
				//23. rypsyl varchar2(4000) null ȼ������ѹ�� bar
				//if (xmlReader.OpenNode(L"root/body/vehispara/rypsyl")&& bIdeReNum)
				//{
				//	xmlReader.GetNodeContent(strCon);
				//	if (!strCon.empty())
				//	{
				//		CString str;
				//		str.Format(L"<rypsyl>%s</rypsyl>", strCon.c_str());
				//		strTemp.Replace(str, L"<rypsyl></rypsyl>");
				//	}
				//}

				if (bIdeReNum)
				{
					// 
					CString strJQMKD(L"<jqmkd>"), strJSFHZ(L"<fh>"), strQYCGQXH(L"<qycgqxh>"), strCS(L"<cs>"), 
						strFDJZS(L"<fdjzs>"), strJQL(L"<jql>"), strJQYL(L"<jqyl>"), strGLKQXS(L"<glkqxs>");
					if (GetSimulationData(strJQMKD, strJSFHZ, strQYCGQXH, strCS, strFDJZS, strJQL, strJQYL, strGLKQXS, ncyds, strDelType))
					{
						strJQMKD.AppendFormat(L"</jqmkd>");
						strTemp.Replace(L"<jqmkd></jqmkd>", strJQMKD);

						strJSFHZ.AppendFormat(L"</fh>");
						strTemp.Replace(L"<fh></fh>", strJSFHZ);

						strQYCGQXH.AppendFormat(L"</qycgqxh>");
						strTemp.Replace(L"<qycgqxh></qycgqxh>", strQYCGQXH);

						strCS.AppendFormat(L"</cs>");
						strTemp.Replace(L"<cs></cs>", strCS);

						strFDJZS.AppendFormat(L"</fdjzs>");
						strTemp.Replace(L"<fdjzs></fdjzs>", strFDJZS);

						strJQL.AppendFormat(L"</jql>");
						strTemp.Replace(L"<jql></jql>", strJQL);

						strJQYL.AppendFormat(L"</jqyl>");
						strTemp.Replace(L"<jqyl></jqyl>", strJQYL);

						strGLKQXS.AppendFormat(L"</glkqxs>");
						strTemp.Replace(L"<glkqxs></glkqxs>", strGLKQXS);

						strXml = strTemp.GetString();
					}

					GetDlgItem(IDC_ED_REPORT_NUMBER)->SetWindowTextW(L"");
					m_cbDelType.SetCurSel(-1);

					// �޸���ɺ�ɾ��
					_ConnectionPtr pConnection;
					if (0x00 == CNHSQLServerDBO::OpenDB(pConnection))
					{
						strSql.Format(L"delete WhiteModifiedParameters where A = '%s'", strReport);

						if (0x00 == CNHSQLServerDBO::ExecuteDML(pConnection, strSql))
						{
						}
					}

					CNHSQLServerDBO::CloseDB(pConnection);
				}
			}
		}
	}
	return strXml;
}

bool CACHBTra_20Dlg::GetSimulationData(CString& strJQMKD, CString& strJSFHZ, CString& strQYCGQXH, CString& strCS, 
	CString& strFDJZS, CString& strJQL, CString& strJQYL, CString& strGLKQXS, const int& ncyds, const CString& strDetType)
{
	for (int i=0; i<ncyds; i++)
	{
		float f;
		int n;
		// ���ٺ�ת��
		if (strDetType == L"��̬")
		{
			n = randEx()%10 + 1;
			f = n/10.0f;
			// 5025
			if (i < 90)
			{
				if (n%2 == 0)
				{
					f = 25.0f - f;
				}
				else
				{
					f = 25.0f + f;
				}
			}
			// 2540
			else
			{
				if (n%2 == 0)
				{
					f = 40.0f - f;
				}
				else
				{
					f = 40.0f + f;
				}
			}
			strCS.AppendFormat(L"%.2f,",f);

			n = randEx()%300 + 1520;
			strFDJZS.AppendFormat(L"%d,",n);

			n = randEx()%2 + 1;
			f = n/100.0f;
			if (n%2 == 0)
			{
				f = 1.00f - f;
			}
			else
			{
				f = 1.00f + f;
			}
			strGLKQXS.AppendFormat(L"%.2f,",f);
		}
		else if (strDetType == L"˫����")
		{
			f = 0.00f;
			strCS.AppendFormat(L"%.2f,",f);

			// �ߵ���
			int nZS = ncyds - 45;
			n = randEx()%200 + 1;
			if (i < nZS)
			{
				if (n%2 == 0)
				{
					n = 2500 - n;
				}
				else
				{
					n = 2500 + n;
				}
			}
			else
			{
				if (n%2 == 0)
				{
					n = 857 - n;
				}
				else
				{
					n = 857 + n;
				}
			}
			strFDJZS.AppendFormat(L"%d,",n);

			n = randEx()%2 + 1;
			f = n/100.0f;
			if (n%2 == 0)
			{
				f = 1.00f - f;
			}
			else
			{
				f = 1.00f + f;
			}
			strGLKQXS.AppendFormat(L"%.2f,",f);
		}
		else if (strDetType == L"���ؼ���")
		{
			n = randEx()%10 + 1;
			f = n/10.0f;
			if (i < (ncyds-2))
			{
				if (n%2 == 0)
				{
					f = 65.0f - f;
				}
				else
				{
					f = 65.0f + f;
				}

				n = randEx()%300 + 2220;
				strFDJZS.AppendFormat(L"%d,",n);
			}
			else
			{
				f = 0.00f;
				n = randEx()%300 + 624;
				strFDJZS.AppendFormat(L"%d,",n);
			}
			strCS.AppendFormat(L"%.2f,",f);
		}
		else if (strDetType == L"���ɼ���")
		{
		}
		else if (strDetType == L"����˲̬")
		{
		}
		//
		n = randEx()%10 + 1;
		if (n%2 == 0)
		{
			f = n + 32.94f;
		}
		else
		{
			f = n /10.0f;
			f = 32.94f + f;
		}
		strJQMKD.AppendFormat(L"%.2f,",f);
		//
		n = randEx()%10 + 1;
		if (n%2 == 0)
		{
			f = 60.00f - n;
		}
		else
		{
			f = n /10.0f;
			f = 60.00f + f;
		}
		strJSFHZ.AppendFormat(L"%.2f,",f);
		//
		n = randEx()%10 + 1;
		if (n%2 == 0)
		{
			f = 30.00 - n;
		}
		else
		{
			f = n /10.0f;
			f = 30.00 + f;
		}
		strQYCGQXH.AppendFormat(L"%.2f,",f);
		//
		n = randEx()%4 + 1;
		if (n%2 == 0)
		{
			f = 2.87f + n;
		}
		else
		{
			f = n /10.0f;
			f = 2.87f + f;
		}
		strJQL.AppendFormat(L"%.2f,",f);
		//
		n = randEx()%10 + 1;
		if (n%2 == 0)
		{
			f = 72.00f - n;
		}
		else
		{
			f = n /10.0f;
			f = 72.00f + f;
		}
		strJQYL.AppendFormat(L"%.2f,",f);
	}

	return true;
}


///////////////////////////������񷽷�////////////////////////////////////////


int __ns1__write(struct soap*, ns1__write *ns1__write_, ns1__writeResponse &ns1__writeResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	std::wstring strXmlDoc2;
	bool bMod;
	strXmlDoc2 = g_pCACHBTraDlg->ModXml(ns1__write_->xmlDoc, bMod);
	ns1__write_->xmlDoc = & strXmlDoc2;

	if (g_pCACHBTraDlg->m_bIsLog && bMod)
	{
		CString strLog;
		strLog.Format(L"%s", ns1__write_->xmlDoc->c_str());

		CNHLogAPI::WriteLogEx(g_pCACHBTraDlg->LogFilePath().c_str(), L"����", L"ns1__write", strLog);
	}

	int nRet = soap_call___ns1__write(&soapClient, g_pCACHBTraDlg->m_chGAURL, NULL, ns1__write_, ns1__writeResponse_);

	if (g_pCACHBTraDlg->m_bIsLog && bMod)
	{
		CString strLog;
		strLog.Format(L"%s", ns1__writeResponse_.return_->c_str());

		CNHLogAPI::WriteLogEx(g_pCACHBTraDlg->LogFilePath().c_str(), L"����", L"ns1__writeResponse_", strLog);
	}

	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__query(struct soap*, ns1__query *ns1__query_, ns1__queryResponse &ns1__queryResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	//std::wstring strXmlDoc2;
	//strXmlDoc2 = g_pCACHBTraDlg->ModXml(ns1__query_->xmlDoc);
	//ns1__query_->xmlDoc = & strXmlDoc2;

	//if (g_pCACHBTraDlg->m_bIsLog)
	//{
	//	CString strLog;
	//	strLog.Format(L"%s->", ns1__query_->xmlDoc->c_str());

	//	CNHLogAPI::WriteLogEx(g_pCACHBTraDlg->LogFilePath().c_str(), L"����", L"ns1__query_", strLog);
	//}

	int nRet = soap_call___ns1__query(&soapClient, g_pCACHBTraDlg->m_chGAURL, NULL, ns1__query_, ns1__queryResponse_);

	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__write_(struct soap*, ns1__write *ns1__write_, ns1__writeResponse &ns1__writeResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	std::wstring strXmlDoc2;
	bool bMod;;
	strXmlDoc2 = g_pCACHBTraDlg->ModXml(ns1__write_->xmlDoc, bMod);
	ns1__write_->xmlDoc = & strXmlDoc2;

	if (g_pCACHBTraDlg->m_bIsLog && bMod)
	{
		CString strLog;
		strLog.Format(L"%s->", ns1__write_->xmlDoc->c_str());

		CNHLogAPI::WriteLogEx(g_pCACHBTraDlg->LogFilePath().c_str(), L"����", L"ns1__write_", strLog);
	}

	int nRet = soap_call___ns1__write_(&soapClient, g_pCACHBTraDlg->m_chGAURL, NULL, ns1__write_, ns1__writeResponse_);

	if (g_pCACHBTraDlg->m_bIsLog && bMod)
	{
		CString strLog;
		strLog.Format(L"%s", ns1__writeResponse_.return_->c_str());

		CNHLogAPI::WriteLogEx(g_pCACHBTraDlg->LogFilePath().c_str(), L"����", L"ns1__writeResponse_", strLog);
	}

	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}

int __ns1__query_(struct soap*, ns1__query *ns1__query_, ns1__queryResponse &ns1__queryResponse_)
{
	struct soap soapClient;
	soap_init(&soapClient);
	soap_set_mode(&soapClient, SOAP_C_UTFSTRING);

	//std::wstring strXmlDoc2;
	//strXmlDoc2 = g_pCACHBTraDlg->ModXml(ns1__query_->xmlDoc);
	//ns1__query_->xmlDoc = & strXmlDoc2;

	//if (g_pCACHBTraDlg->m_bIsLog)
	//{
	//	CString strLog;
	//	strLog.Format(L"%s->", ns1__query_->xmlDoc->c_str());

	//	CNHLogAPI::WriteLogEx(g_pCACHBTraDlg->LogFilePath().c_str(), L"����", L"ns1__query_", strLog);
	//}

	int nRet = soap_call___ns1__query_(&soapClient, g_pCACHBTraDlg->m_chGAURL, NULL, ns1__query_, ns1__queryResponse_);

	//soap_destroy(&soapClient);
	soap_end(&soapClient);
	soap_done(&soapClient);

	return nRet;
}