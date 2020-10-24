
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
	GetLogFilePath();

#ifndef CS_NO_YZ
	if (!UpIntLogIn())
	{
		return FALSE;
	}
#endif

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
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		m_strkey = si.GetString(L"UserInfo", L"NetKey", L"");
		m_strName = si.GetString(L"UserInfo", L"Name", L"");
	}
}

void CSystemCheckApp::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"����SystemCheck%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

bool CSystemCheckApp::UpIntLogIn(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�û���ϢINI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"��¼�ϴ�", L"�û���ϢINI����");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp;

	strTemp = si.GetString(L"UserInfo", L"Name", L"GXCZ3");
	CString strSql;

	strSql.Format(L"SELECT * from userInfo WHERE Name = '%s'", strTemp);
	USERINFO sUserInfo;
	if (0x01 != GetDboUserInfo(strSql.GetString(), &sUserInfo))
	{
		// ��ȡ���α�֤������
		CString strMsg;
		strMsg.Format(L"��������ʧ�ܻ�û�иõ�¼Ա%s", strTemp);
		MessageBox(NULL, strMsg, L"", MB_ICONWARNING|MB_OK);
		return false;
	}
	// ��ȡ����תMD5����
	strTemp.Format(L"%s", sUserInfo.wchPassword);
	std::string smd(CW2A(strTemp.GetString()));
	MD5 md5(smd);
	smd = md5.md5();
	// ��д
	transform(smd.begin(), smd.end(), smd.begin(), ::toupper);
	// Сд
	//transform(smd.begin(), smd.end(), smd.begin(), ::tolower);

	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><login>");
	//���վ���
	strData.AppendFormat(L"<jczbh>%s</jczbh>", m_StationNum);
	// ����߱��
	strData.AppendFormat(L"<jcxbh>%s</jcxbh>", m_LineNum);
	// ��¼�ʺ�
	strTemp = si.GetString(L"UserInfo", L"Name", L"GXCZ3");
	// �������ĵ�¼���õ�¼��������������
	if (!IsChinese(strTemp))
	{
		strData.AppendFormat(L"<dlzh>%s</dlzh>", strTemp);
	}
	else
	{
		strData.AppendFormat(L"<dlzh>%s</dlzh>", sUserInfo.wchLicenseSn);
	}
	// ����Ϊ32�ĵ�¼����MD5�ַ���
	strTemp = smd.c_str();
	strData.AppendFormat(L"<dlkl>%s</dlkl>", strTemp);

	strData.AppendFormat(L"</login></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"11Q01", strData.GetString(), strRet);

#ifdef CS_NO_YZ
	nRet = 0;
	CString strDataRet;
	strDataRet.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	strDataRet.AppendFormat(L"<root><head><code>1</code><message>��¼�ɹ�!</message><rownum>1</rownum></head>");
	strDataRet.AppendFormat(L"<body><list id=\"0\"><data>0BF83F050621830D35E94732137C9566</data></list></body></root>");
	strRet = strDataRet;
#endif
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
					si.SetString(L"UserInfo", L"NetKey", strContent.c_str());
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

bool CSystemCheckApp::UpIntLogOut(void)
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


int CSystemCheckApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpIntLogOut();


	return CWinApp::ExitInstance();
}

bool CSystemCheckApp::IsChinese(CString& strChi)
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
