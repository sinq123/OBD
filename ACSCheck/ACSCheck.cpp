
// ACSCheck.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ACSCheck.h"
#include "ACSCheckDlg.h"
#include "RJYZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACSCheckApp

BEGIN_MESSAGE_MAP(CACSCheckApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CACSCheckApp ����

CACSCheckApp::CACSCheckApp()
	: m_pchURL(NULL)
	, m_strkey(L"")
	, m_StationNum(L"")
	, m_LineNum(L"")
	, m_strIntLogFilePath(L"")
	, m_RegistCode(L"")
	, m_strVersion(L"")
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CACSCheckApp ����

CACSCheckApp theApp;


// CACSCheckApp ��ʼ��

BOOL CACSCheckApp::InitInstance()
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
#endif
	m_RegistCode.Format(L"%s", this->m_lpCmdLine);
	GetConfig();
	GetLogFilePath();

	CHNSY_API::SetLogFilePath(m_strIntLogFilePath.GetString());
#ifndef _DEBUG
	if (!GetIntAcc())
	{
		return FALSE;
	}
#else
	m_RegistCode = L"123456789";
#endif
	CACSCheckDlg dlg;
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

void CACSCheckApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"WebInfo.ini", wchPath, false))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"WebService", L"URL", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!strTemp.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(strTemp);
		}
		if (m_RegistCode.IsEmpty())
		{
			m_RegistCode = si.GetString(L"WebService", L"LicenseCode", L"");
		}
		else
		{
			si.SetString(L"WebService", L"LicenseCode", m_RegistCode);
		}
		m_strVersion = si.GetString(L"WebService", L"Version", L"5.0.3.0");

		m_StationNum = si.GetString(L"WebService", L"StationNum", L"");
		m_LineNum= si.GetString(L"WebService", L"LineNum", L"");
	}

	

}

void CACSCheckApp::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"����ACSCheck%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

bool CACSCheckApp::GetIntAcc(void)
{
	if (m_pchURL != NULL && !m_StationNum.IsEmpty())
	{
		std::wstring strRet;
		int nRet = CHNSY_API::getAccessToken(m_pchURL, m_LineNum.GetString(), strRet);

		if (nRet == 0)
		{
			CXmlReader xmlReader;
			if (xmlReader.Parse(strRet.c_str()))
			{
				std::wstring wstrCode, wstrContent;
				if (xmlReader.OpenNode(L"root/result"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}

				if (xmlReader.OpenNode(L"root/info"))
				{
					xmlReader.GetNodeContent(wstrContent);
				}

				if (wstrCode != L"1")
				{
					wstrContent = L"��ȡʧ�ܣ�" + wstrContent;
					MessageBox(NULL, wstrContent.c_str(), L"", MB_ICONWARNING|MB_OK);
					return false;
				}
				else
				{
					if (xmlReader.OpenNode(L"root/info/accessToken"))
					{
						xmlReader.GetNodeContent(wstrContent);
						m_RegistCode = wstrContent.c_str();
					}

					return true;
				}
			}
			else
			{
				MessageBox(NULL, L"����ʧ��", L"", MB_ICONWARNING|MB_OK);
				return false;
			}
		}
		else
		{
			MessageBox(NULL, L"����ʧ��", L"", MB_ICONWARNING|MB_OK);
			return false;
		}
	}
	else
	{
		MessageBox(NULL, L"��ȡ�����������п�ֵ", L"", MB_ICONWARNING|MB_OK);
		return false;
	}
}

bool CACSCheckApp::UpTobbh(void)
{
	std::wstring strRet;
	int nRet = CHNSY_API::gkrjbbh(m_pchURL, m_RegistCode.GetString(), m_strVersion.GetString(), strRet);

	if (nRet == 0)
		{
			CXmlReader xmlReader;
			if (xmlReader.Parse(strRet.c_str()))
			{
				std::wstring wstrCode, wstrContent;
				if (xmlReader.OpenNode(L"root/result"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}

				if (xmlReader.OpenNode(L"root/info"))
				{
					xmlReader.GetNodeContent(wstrContent);
				}

				if (wstrCode != L"1")
				{
					wstrContent = L"��ȡʧ�ܣ�" + wstrContent;
					MessageBox(NULL, wstrContent.c_str(), L"", MB_ICONWARNING|MB_OK);
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				MessageBox(NULL, L"����ʧ��", L"", MB_ICONWARNING|MB_OK);
				return false;
			}
		}
		else
		{
			MessageBox(NULL, L"����ʧ��", L"", MB_ICONWARNING|MB_OK);
			return false;
		}
}
