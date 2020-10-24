
// ACSCheck.cpp : 定义应用程序的类行为。
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


// CACSCheckApp 构造

CACSCheckApp::CACSCheckApp()
	: m_pchURL(NULL)
	, m_strkey(L"")
	, m_StationNum(L"")
	, m_LineNum(L"")
	, m_strIntLogFilePath(L"")
	, m_RegistCode(L"")
	, m_strVersion(L"")
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CACSCheckApp 对象

CACSCheckApp theApp;


// CACSCheckApp 初始化

BOOL CACSCheckApp::InitInstance()
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
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"邵阳ACSCheck%s.log", odtNow.Format(L"%Y-%m-%d"));

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
					wstrContent = L"获取失败：" + wstrContent;
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
				MessageBox(NULL, L"解析失败", L"", MB_ICONWARNING|MB_OK);
				return false;
			}
		}
		else
		{
			MessageBox(NULL, L"联网失败", L"", MB_ICONWARNING|MB_OK);
			return false;
		}
	}
	else
	{
		MessageBox(NULL, L"获取联网检测参数有空值", L"", MB_ICONWARNING|MB_OK);
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
					wstrContent = L"获取失败：" + wstrContent;
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
				MessageBox(NULL, L"解析失败", L"", MB_ICONWARNING|MB_OK);
				return false;
			}
		}
		else
		{
			MessageBox(NULL, L"联网失败", L"", MB_ICONWARNING|MB_OK);
			return false;
		}
}
