
// CSTra.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "..\GZWebserviceLib\GZWebserviceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\GZWebLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\GZWebLib.lib")
#endif

#include "..\ACInterfaceLib\ACInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACInterfaceLib.lib")
#endif

// CCSTraApp:
// 有关此类的实现，请参阅 CSTra.cpp
//

class CCSTraApp : public CWinApp
{
public:
	CCSTraApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCSTraApp theApp;