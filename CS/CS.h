
// CS.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "..\LogIn\LogIn.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\Debug\\LogIn_D.lib" )
#else
#pragma comment( lib, "..\\Release\\LogIn.lib" )
#endif

#include "resource.h"		// 主符号


// CCSApp:
// 有关此类的实现，请参阅 CS.cpp
//

class CCSApp : public CWinApp
{
public:
	CCSApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCSApp theApp;