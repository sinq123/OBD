
// SGOBD.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SGOBD.h"
#include "SGOBDDlg.h"
#include "LogIn.h"
#include "RJYZ.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSGOBDApp

BEGIN_MESSAGE_MAP(CSGOBDApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSGOBDApp 构造

CSGOBDApp::CSGOBDApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSGOBDApp 对象

CSGOBDApp theApp;


// CSGOBDApp 初始化

BOOL CSGOBDApp::InitInstance()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, m_pszAppName);
	if (ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, L"程序已经运行", L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	GetConfig();
	GetLogFilePath();

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

	//// 平台登录认证
	//if (!UpIntLogIn())
	//{
	//	return FALSE;
	//}

	CSGOBDDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

void CSGOBDApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"WebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strURL = si.GetString(L"NetSet", L"URL", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!m_strURL.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(m_strURL);
		}
		m_StationNum = si.GetString(L"NetSet", L"StationNum", L"");
		m_LineNum = si.GetString(L"NetSet", L"LineNum", L"");

		m_strName = si.GetString(L"NetSet", L"Name", L"");
		m_strPassword = si.GetString(L"NetSet", L"Password", L"");
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"SGDB.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		m_strSourceDB = si.GetString(L"Database", L"Source", L"(Local)");
		m_strCatalogDB = si.GetString(L"Database", L"Catalog", L"NHModeServerDB_GB2018");
		m_strUserIDDB = si.GetString(L"Database", L"UserID", L"sa");
		m_strPasswordDB = si.GetString(L"Database", L"Password", L"123");
	}
}

void CSGOBDApp::GetLogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName, strFileName2;
	strFileName.Format(L"曙光OBD%s.log", odtNow.Format(L"%Y-%m-%d"));
	strFileName2.Format(L"曙光DB%s.log", odtNow.Format(L"%Y-%m-%d"));
	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;

	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName2;
	m_strDBLogFilePath = strLogFilePath;
}

bool CSGOBDApp::UpIntLogIn(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	std::string smd(CW2A(m_strPassword.GetString()));
	MD5 md5(smd);
	smd = md5.md5();
	// 大写
	transform(smd.begin(), smd.end(), smd.begin(), ::toupper);
	// 小写
	//transform(smd.begin(), smd.end(), smd.begin(), ::tolower);

	CStringW strData, strTemp;

	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><login>");
	//检测站编号
	strData.AppendFormat(L"<jczbh>%s</jczbh>", m_StationNum);
	// 检测线编号
	strData.AppendFormat(L"<jcxbh>%s</jcxbh>", m_LineNum);
	// 登录帐号
	strData.AppendFormat(L"<dlzh>%s</dlzh>", m_strName);
	// 长度为32的登录口令MD5字符串
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
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"登录上传", str);
				MessageBox(NULL, str, L"", MB_ICONWARNING|MB_OK);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"登录上传", str);

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
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"登录上传", L"联网失败");
		MessageBox(NULL, L"联网失败", L"", MB_ICONWARNING|MB_OK);
		return false;
	}
	return true;
}

bool CSGOBDApp::UpIntLogOut(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><logout>");
	strData.AppendFormat(L"</logout></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"11Q02", strData.GetString(), strRet);

	return true;
}


int CSGOBDApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	//UpIntLogOut();


	return CWinApp::ExitInstance();
}

bool CSGOBDApp::IsChinese(CString& strChi)
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