
// ACSCheck.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

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

#include "..\NHSYInterfaceLib\NHSYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHSYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHSYInterfaceLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
#endif

#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
#endif

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CACSCheckApp:
// 有关此类的实现，请参阅 ACSCheck.cpp
//

class CACSCheckApp : public CWinApp
{
public:
	CACSCheckApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
	char* m_pchURL;
	CStringW m_strkey;
	CStringW m_StationNum;
	CStringW m_LineNum;
	CStringW m_strName;
	CStringW m_RegistCode;
	CStringW m_strVersion;
	// 获取配置信息
	void GetConfig(void);
	// 生成日志文件
	CStringW m_strIntLogFilePath;
	void GetLogFilePath(void);
	bool GetIntAcc(void);
	bool UpTobbh(void);

};

extern CACSCheckApp theApp;