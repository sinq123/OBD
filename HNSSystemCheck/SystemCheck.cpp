
// SystemCheck.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SystemCheck.h"
#include "SystemCheckDlg.h"
#include "RJYZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSystemCheckApp

BEGIN_MESSAGE_MAP(CSystemCheckApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSystemCheckApp ����

CSystemCheckApp::CSystemCheckApp()
	: m_pchURL(NULL)
	, m_StationNum(L"")
	, m_LicenseCode(L"")
	, m_LineNum(L"")
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSystemCheckApp ����

CSystemCheckApp theApp;


// CSystemCheckApp ��ʼ��

BOOL CSystemCheckApp::InitInstance()
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

#ifndef CS_NO_YZ
	std::wstring strMsg, strDLMM;
	if (!CRJYZ::IsYZ(strMsg, strDLMM))
	{
		MessageBox(NULL,strMsg.c_str(), L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}

#endif
	GetConfig();
	if (!GetAccessToken())
	{
		MessageBox(NULL, L"��ȡ����ʧ��", L"����", MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	CSystemCheckDlg dlg;
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

void CSystemCheckApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"ZZWDConfig.ini", wchPath))
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
		m_StationNum = si.GetString(L"WebService", L"StationNumber", L"");
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		m_LineNum = si.GetString(L"System", L"LineNumber", L"");
	}
}

bool CSystemCheckApp::GetAccessToken(void)
{
	bool bRet(false);

	CHNSY_API::SetLogFilePath(L"");

	if (!m_StationNum.IsEmpty())
	{
		std::wstring strRetStr;
		
		int nRet = CHNSY_API::getAccessToken(m_pchURL, m_LineNum.GetString(), strRetStr);

		if (nRet == 0)
		{
			CXmlReader xmlReader;
			if (xmlReader.Parse(strRetStr.c_str()))
			{
				std::wstring strCode, strAccessToken;
				// 1 result
				if (xmlReader.OpenNode(L"root/result"))
				{
					xmlReader.GetNodeContent(strCode);
				}

				if (strCode.find(L"1") != std::string::npos)
				{
					if (xmlReader.OpenNode(L"root/info/accessToken"))
					{
						xmlReader.GetNodeContent(strAccessToken);
					}
					if (strAccessToken != L"")
					{
						m_LicenseCode = strAccessToken.c_str();
						bRet = true;
					}
				}
			}
		}
	}

	return bRet;
}

int CSystemCheckApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	//UpIntLogOut();

	return CWinApp::ExitInstance();
}

