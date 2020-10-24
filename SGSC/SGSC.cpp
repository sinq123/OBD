
// SGSC.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SGSC.h"
#include "SGSCDlg.h"
#include "RJYZ.h"
#include "LoginDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSGSCApp

BEGIN_MESSAGE_MAP(CSGSCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSGSCApp ����

CSGSCApp::CSGSCApp()
	: m_pchURL(NULL)
	, m_strkey(L"")
	, m_StationNum(L"")
	, m_LineNum(L"")
	, m_strIntLogFilePath(L"")
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSGSCApp ����

CSGSCApp theApp;


// CSGSCApp ��ʼ��

BOOL CSGSCApp::InitInstance()
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

	// ֤����֤
	std::wstring strMsg, strDLMM;
	if (!CRJYZ::IsYZ(strMsg, strDLMM))
	{
		MessageBox(NULL,strMsg.c_str(), L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	// ��¼��ȡ�û���������
	CLoginDlg LogInDlg;
	if (LogInDlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	GetConfig();
	GetLogFilePath();
	// ƽ̨��¼��֤
	if (!UpIntLogIn())
	{
		return FALSE;
	}

	CSGSCDlg dlg;
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


void CSGSCApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"WebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"NetSet", L"URL", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!strTemp.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(strTemp);
		}
		m_StationNum = si.GetString(L"NetSet", L"StationNum", L"");
		m_LineNum = si.GetString(L"NetSet", L"LineNum", L"");
	}
}

void CSGSCApp::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"���SGSC%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

bool CSGSCApp::UpIntLogIn(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	std::string smd(CW2A(m_strPassword.GetString()));
	MD5 md5(smd);
	smd = md5.md5();
	// ��д
	transform(smd.begin(), smd.end(), smd.begin(), ::toupper);
	// Сд
	//transform(smd.begin(), smd.end(), smd.begin(), ::tolower);

	CStringW strData, strTemp;

	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><login>");
	//���վ���
	strData.AppendFormat(L"<jczbh>%s</jczbh>", m_StationNum);
	// ����߱��
	strData.AppendFormat(L"<jcxbh>%s</jcxbh>", m_LineNum);
	// ��¼�ʺ�
	strData.AppendFormat(L"<dlzh>%s</dlzh>", m_strName);
	// ����Ϊ32�ĵ�¼����MD5�ַ���
	strTemp = smd.c_str();
	strData.AppendFormat(L"<dlkl>%s</dlkl>", strTemp);

	strData.AppendFormat(L"</login></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"11Q01", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"��¼�ϴ�", str);
				MessageBox(NULL, str, L"", MB_ICONWARNING|MB_OK);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"��¼�ϴ�", str);

				if (xmlReader.OpenNode(L"root/body/list/data"))
				{
					xmlReader.GetNodeContent(strContent);
					m_strkey = strContent.c_str();
				}
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"��¼�ϴ�", L"����ʧ��");
		MessageBox(NULL, L"����ʧ��", L"", MB_ICONWARNING|MB_OK);
		return false;
	}
	return true;
}

bool CSGSCApp::UpIntLogOut(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><logout>");
	strData.AppendFormat(L"</logout></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"11Q02", strData.GetString(), strRet);

	return true;
}


int CSGSCApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpIntLogOut();


	return CWinApp::ExitInstance();
}

bool CSGSCApp::IsChinese(CString& strChi)
{
	strChi.Trim();
	bool bRet(false);
	int nLen = strChi.GetLength();
	for(int i = 0;i<= nLen; i++)
	{
		int unicode = (int)strChi.GetAt(i);
		if (unicode <= '9' && unicode >= '0')
		{
		}
		else if (unicode <= 'z' && unicode >= 'a')
		{
		}
		else if (unicode <= 'Z' && unicode >= 'A')
		{
		}
		else if (unicode > 255)
		{
			bRet = true;
			break;
		}
		else
		{
		}

	}

	return bRet;
}
