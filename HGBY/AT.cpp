
// AT.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AT.h"
#include "ATDlg.h"
#include "RJYZ.h"
#include "LogIn.h"

#include <locale>//ͷ�ļ�
#include <fstream>
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
	, m_strPathLog(L"")
	, m_strLogNameN(L"")
	, m_strLogNameF(L"")
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

#ifndef _DEBUG
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
#endif
	GetLogFilePath();
	GetConfig();
	GetKey();
	CHGBYWebLib_API::SetLogFilePath(m_strIntLogFilePath.GetString());

	CATDlg dlg;
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
	if (!(_taccess(m_strPathLog, 06)))
	{
		CString strPath;
		strPath.Format(L"%s\\%s%s%s.log", m_strPathLog, m_strLogNameN, COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"), m_strLogNameF);

		DeleteFile(strPath);
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
	strFileName.Format(L"HGBY%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

void CATApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"HGWebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strURL = si.GetString(L"WebServer", L"URL", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!m_strURL.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(m_strURL);
		}
		m_strStationNum = si.GetString(L"WebServer", L"LineCode", L"");
		m_strLineNum = si.GetString(L"WebServer", L"StnId", L"");
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strUserName = si.GetString(L"UserInfo", L"Name", L"");
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetCDFilePath(L"Path.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strPathLog = si.GetString(L"Path", L"PathLog", L"");
		m_strLogNameN = si.GetString(L"Path", L"LogNameN", L"");
		m_strLogNameF = si.GetString(L"Path", L"LogNameF", L"");
	}
}

void CATApp::GetKey(void)
{

	// �ж�·���Ƿ����,�����Խ��ж�д
	if (!(_taccess(m_strPathLog, 06)))
	{
		CString strPath;
		strPath.Format(L"%s\\%s%s%s.log", m_strPathLog, m_strLogNameN, COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"), m_strLogNameF);

		CStdioFileEx sfe;
		// ��֤�򿪳ɹ�
		if (sfe.Open(strPath, CFile::modeReadWrite|CFile::typeText))
		{
			CString strLog, strTemp;
			while (sfe.ReadString(strTemp))
			{
				if (strTemp.Find(L"RSInfo����	�ӿ�:Login") != -1)
				{
					strLog = strTemp;
				}
			}
			int nN = strLog.Find(L"<body>");
			int nF = strLog.Find(L"</body>");
			strTemp = strLog.Mid(nN+6, nF-nN-6);

			CXmlReader xmlReader;
			if (xmlReader.Parse(strTemp))
			{

				std::wstring strCon;
				if (xmlReader.OpenNode(L"Token/TokenInfo"))
				{
					xmlReader.GetNodeAttribute(L"TokenGuid", strCon);
					m_strKey = strCon.c_str();
				}
			}

		}
		sfe.Close();
	}
}
