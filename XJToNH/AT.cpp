
// AT.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AT.h"
#include "ATDlg.h"
#include "RJYZ.h"
#include "LogIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CATApp

BEGIN_MESSAGE_MAP(CATApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CATApp ����

CATApp::CATApp()
	: m_pchURL(NULL)
	, m_strURL(L"")
	, m_strStationNum(L"")
	, m_strLineNum(L"")
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CATApp ����

CATApp theApp;


// CATApp ��ʼ��

BOOL CATApp::InitInstance()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, m_pszAppName);
	if (ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, L"�����Ѿ�����", L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	std::wstring strMsg, strDLMM;
	if (!CRJYZ::IsYZ(strMsg, strDLMM))
	{
		MessageBox(NULL,strMsg.c_str(), L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	CLogIn LogInDlg;
	LogInDlg.m_strDLMM = strDLMM.c_str();
	if (LogInDlg.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	GetLogFilePath();
	GetConfig();


	// ����һ�����صĴ��ڣ��Ա�������Ի����������������غ���ʾ
	CWnd m_wndOwner;
	if (m_wndOwner.m_hWnd == NULL)
	{
		LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
		if (!m_wndOwner.CreateEx(0, pstrOwnerClass, _T(""),  //����һ�����صĵ�����ʽ�Ĵ���
			WS_POPUP, CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL, 0))
			return FALSE;
	}

	//CATDlg dlg;
	CATDlg dlg(&m_wndOwner);     // �����صĴ�����Ϊ������
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}


	// �������صĸ�����
	if (m_wndOwner.m_hWnd != NULL)
		m_wndOwner.DestroyWindow();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CATApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���

	if (NULL != m_pchURL)
	{
		free(m_pchURL);
		m_pchURL = NULL;
	}

	return CWinApp::ExitInstance();
}

void CATApp::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"AT����%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

void CATApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"XJConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strURL = si.GetString(L"NetConfig", L"URL", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!m_strURL.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(m_strURL);
		}

		m_strKey = si.GetString(L"NetConfig", L"Key", L"");
	}

	m_odtDemarcationDate = COleDateTime::GetCurrentTime();
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;
		strTemp = si.GetString(L"DemarcationLog", L"DemarcationDate", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));

		if(!m_odtDemarcationDate.ParseDateTime(strTemp))
		{
			m_odtDemarcationDate = COleDateTime::GetCurrentTime();
		}
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		m_strLineNum = si.GetString(L"LineInfo", L"LineNumber", L"");
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		m_strStationNum = si.GetString(L"StationInfo", L"StationNumber", L"");
	}
}