
// CDSCheck.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
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

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\OBDEquOpe\EquOpe.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDEquOpe_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDEquOpe.lib")
#endif


#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
#endif

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "..\ZYHttp\ZYHttp.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ZYHttp_D.lib")
#else
#pragma comment(lib, "..\\Release\\ZYHttp.lib")
#endif

// CCDSCheckApp:
// 有关此类的实现，请参阅 CDSCheck.cpp
//

class CCDSCheckApp : public CWinApp
{
public:
	CCDSCheckApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现

	DECLARE_MESSAGE_MAP()


public:
	USERINFO m_sUserInfo;
};

extern CCDSCheckApp theApp;