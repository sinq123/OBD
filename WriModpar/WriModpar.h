
// WriModpar.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

// ���ÿ�(MFC��չDLL)
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

// CWriModparApp:
// �йش����ʵ�֣������ WriModpar.cpp
//

class CWriModparApp : public CWinApp
{
public:
	CWriModparApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWriModparApp theApp;