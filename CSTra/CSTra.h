
// CSTra.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

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
// �йش����ʵ�֣������ CSTra.cpp
//

class CCSTraApp : public CWinApp
{
public:
	CCSTraApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCSTraApp theApp;