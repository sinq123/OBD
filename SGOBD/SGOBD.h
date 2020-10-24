
// SGOBD.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <algorithm>

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\SGSQL\SGSQL.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\SGSQL_D.lib")
#else
#pragma comment(lib, "..\\Release\\SGSQL.lib")
#endif

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

// CSGOBDApp:
// 有关此类的实现，请参阅 SGOBD.cpp
//

class CSGOBDApp : public CWinApp
{
public:
	CSGOBDApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现

	DECLARE_MESSAGE_MAP()
public:
	CStringW m_strURL;
	char* m_pchURL;
	CStringW m_strkey;
	CStringW m_StationNum;
	CStringW m_LineNum;
	CStringW m_strName;
	CStringW m_strPassword;

	CStringW m_strSourceDB;
	CStringW m_strCatalogDB;
	CStringW m_strUserIDDB;
	CStringW m_strPasswordDB;
	// 获取配置信息
	void GetConfig(void);
	// 生成日志文件
	CStringW m_strIntLogFilePath;
	CStringW m_strDBLogFilePath;
	void GetLogFilePath(void);

	bool UpIntLogIn(void);
	bool UpIntLogOut(void);
	bool IsChinese(CString& strChi);
};

extern CSGOBDApp theApp;