
// AT.cpp : 定义应用程序的类行为。
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


// CATApp 构造

CATApp::CATApp()
	: m_pchURL(NULL)
	, m_strURL(L"")
	, m_strStationNum(L"")
	, m_strLineNum(L"")
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CATApp 对象

CATApp theApp;


// CATApp 初始化

BOOL CATApp::InitInstance()
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


	// 创建一个隐藏的窗口，以便控制主对话框在任务栏的隐藏和显示
	CWnd m_wndOwner;
	if (m_wndOwner.m_hWnd == NULL)
	{
		LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
		if (!m_wndOwner.CreateEx(0, pstrOwnerClass, _T(""),  //创建一个隐藏的弹出样式的窗口
			WS_POPUP, CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL, 0))
			return FALSE;
	}

	//CATDlg dlg;
	CATDlg dlg(&m_wndOwner);     // 以隐藏的窗口作为父窗口
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


	// 销毁隐藏的父窗口
	if (m_wndOwner.m_hWnd != NULL)
		m_wndOwner.DestroyWindow();

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CATApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (NULL != m_pchURL)
	{
		free(m_pchURL);
		m_pchURL = NULL;
	}

	return CWinApp::ExitInstance();
}

void CATApp::GetLogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"AT艾特%s.log", odtNow.Format(L"%Y-%m-%d"));

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