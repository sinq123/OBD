
// SystemCheck.cpp : 定义应用程序的类行为。
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


// CSystemCheckApp 构造

CSystemCheckApp::CSystemCheckApp()
	: m_pchURL(NULL)
	, m_strkey(L"")
	, m_StationNum(L"")
	, m_LineNum(L"")
	, m_strIntLogFilePath(L"")
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSystemCheckApp 对象

CSystemCheckApp theApp;


// CSystemCheckApp 初始化

BOOL CSystemCheckApp::InitInstance()
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
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"衡阳SystemCheck%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

bool CSystemCheckApp::UpIntLogIn(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath, false))
	{
		// 文件不存在
		AfxMessageBox(L"用户信息INI不在");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"登录上传", L"用户信息INI不在");
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
		// 获取两次保证都可以
		CString strMsg;
		strMsg.Format(L"连接数据失败或没有该登录员%s", strTemp);
		MessageBox(NULL, strMsg, L"", MB_ICONWARNING|MB_OK);
		return false;
	}
	// 获取密码转MD5密文
	strTemp.Format(L"%s", sUserInfo.wchPassword);
	std::string smd(CW2A(strTemp.GetString()));
	MD5 md5(smd);
	smd = md5.md5();
	// 大写
	transform(smd.begin(), smd.end(), smd.begin(), ::toupper);
	// 小写
	//transform(smd.begin(), smd.end(), smd.begin(), ::tolower);

	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><login>");
	//检测站编号
	strData.AppendFormat(L"<jczbh>%s</jczbh>", m_StationNum);
	// 检测线编号
	strData.AppendFormat(L"<jcxbh>%s</jcxbh>", m_LineNum);
	// 登录帐号
	strTemp = si.GetString(L"UserInfo", L"Name", L"GXCZ3");
	// 不是中文登录，用登录名，不是用其他
	if (!IsChinese(strTemp))
	{
		strData.AppendFormat(L"<dlzh>%s</dlzh>", strTemp);
	}
	else
	{
		strData.AppendFormat(L"<dlzh>%s</dlzh>", sUserInfo.wchLicenseSn);
	}
	// 长度为32的登录口令MD5字符串
	strTemp = smd.c_str();
	strData.AppendFormat(L"<dlkl>%s</dlkl>", strTemp);

	strData.AppendFormat(L"</login></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"11Q01", strData.GetString(), strRet);

#ifdef CS_NO_YZ
	nRet = 0;
	CString strDataRet;
	strDataRet.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	strDataRet.AppendFormat(L"<root><head><code>1</code><message>登录成功!</message><rownum>1</rownum></head>");
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
					si.SetString(L"UserInfo", L"NetKey", strContent.c_str());
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

bool CSystemCheckApp::UpIntLogOut(void)
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


int CSystemCheckApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
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
