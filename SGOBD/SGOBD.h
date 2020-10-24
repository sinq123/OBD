
// SGOBD.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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
// �йش����ʵ�֣������ SGOBD.cpp
//

class CSGOBDApp : public CWinApp
{
public:
	CSGOBDApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

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
	// ��ȡ������Ϣ
	void GetConfig(void);
	// ������־�ļ�
	CStringW m_strIntLogFilePath;
	CStringW m_strDBLogFilePath;
	void GetLogFilePath(void);

	bool UpIntLogIn(void);
	bool UpIntLogOut(void);
	bool IsChinese(CString& strChi);
};

extern CSGOBDApp theApp;