
// ACOBDT.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// 自用库(MFC扩展DLL)
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

#include "..\ACHBIntLib\ACHBIntLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACHBIntLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACHBIntLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT


#ifndef Up_OBD_RealTime
//#define Up_OBD_RealTime
#endif

// CACOBDTApp:
// 有关此类的实现，请参阅 ACOBDT.cpp
//

class CACOBDTApp : public CWinApp
{
public:
	CACOBDTApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
	// 生成日志文件
	CStringW m_strIntLogFilePath;
	void GetLogFilePath(void);

	char* m_pchURL;
	CStringW m_strURL;
	CStringW m_strStationNum;
	CStringW m_strLineNum;
	CStringW m_strDetectLSBH;
	COleDateTime m_odtDemarcationDate;
	// 获取配置信息
	void GetConfig(void);

	CString DecodeURI(LPCSTR szURIString);
	CString DecodeURI(LPCWSTR szURIString);

};

extern CACOBDTApp theApp;