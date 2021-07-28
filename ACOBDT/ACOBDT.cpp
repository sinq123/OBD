
// ACOBDT.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ACOBDT.h"
#include "ACOBDTDlg.h"
#include "RJYZ.h"
#include "LogIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACOBDTApp

BEGIN_MESSAGE_MAP(CACOBDTApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CACOBDTApp ����

CACOBDTApp::CACOBDTApp()
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


// Ψһ��һ�� CACOBDTApp ����

CACOBDTApp theApp;


// CACOBDTApp ��ʼ��

BOOL CACOBDTApp::InitInstance()
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

	//std::wstring strMsg, strDLMM;
	//if (!CRJYZ::IsYZ(strMsg, strDLMM))
	//{
	//	MessageBox(NULL,strMsg.c_str(), L"", MB_ICONWARNING|MB_OK);
	//	return FALSE;
	//}

	//CLogIn LogInDlg;
	//LogInDlg.m_strDLMM = strDLMM.c_str();
	//if (LogInDlg.DoModal() == IDCANCEL)
	//{
	//	return FALSE;
	//}

	GetLogFilePath();
	GetConfig();

	CACOBDTDlg dlg;
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

void CACOBDTApp::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"ACOBDT%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

void CACOBDTApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"ACWebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strURL = si.GetString(L"EPB", L"WebAddress", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!m_strURL.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(m_strURL);
		}
		m_strStationNum = si.GetString(L"EPB", L"JCZBH", L"");
		m_strLineNum = si.GetString(L"EPB", L"JKXLH", L"");
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

}

CString CACOBDTApp::DecodeURI(LPCSTR szURIString)
{
	std::string strSource(szURIString);
	int nSrcLen = strSource.size();

	std::string strDestA;

	int nIndex(0);
	while (nIndex < nSrcLen)
	{
		if (strSource.at(nIndex) == '%'
			&& nIndex+2 <= nSrcLen)
		{
			std::string strValue = strSource.substr(nIndex, 3);
			int nValue(0);
			sscanf_s(strValue.c_str(), "%%%x", &nValue);
			strDestA += (char)nValue;
			nIndex += 3;
		}
		else if (strSource.at(nIndex) == '+')
		{
			strDestA += ' ';
			nIndex++;
		}
		else
		{
			strDestA += (char)strSource.at(nIndex);
			nIndex++;
		}
	}
	CString strDest(UTF8ToTChar(strDestA.c_str()));
	return strDest;
}

CString CACOBDTApp::DecodeURI(LPCWSTR szURIString)
{
	return DecodeURI(KUTF16ToANSI(szURIString));
}