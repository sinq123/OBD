
// SGSC.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Meter.h"
#include "SxGroupBox.h"
#include "Panel.h"
#include "PanelItem.h"
#include "md5.h"
#include <algorithm>

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

// CSGSCApp:
// 有关此类的实现，请参阅 SGSC.cpp
//

class CSGSCApp : public CWinApp
{
public:
	CSGSCApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现

	DECLARE_MESSAGE_MAP()

public:
	char* m_pchURL;
	CStringW m_strkey;
	CStringW m_StationNum;
	CStringW m_LineNum;
	CStringW m_strName;
	CStringW m_strPassword;
	// 获取配置信息
	void GetConfig(void);
	// 生成日志文件
	CStringW m_strIntLogFilePath;
	void GetLogFilePath(void);

	bool UpIntLogIn(void);
	bool UpIntLogOut(void);
	bool IsChinese(CString& strChi);
};

extern CSGSCApp theApp;