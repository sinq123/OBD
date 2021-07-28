
// WriModpar_2.0.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

// 自用库(MFC扩展DLL)
#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\ACInterfaceLib\ACInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACInterfaceLib.lib")
#endif

#include "..\ACHBIntLib\ACHBIntLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACHBIntLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACHBIntLib.lib")
#endif

// CWriModpar_20App:
// 有关此类的实现，请参阅 WriModpar_2.0.cpp
//

class CWriModpar_20App : public CWinApp
{
public:
	CWriModpar_20App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CWriModpar_20App theApp;