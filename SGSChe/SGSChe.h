
// SGSChe.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Meter.h"
#include "SxGroupBox.h"
#include "Panel.h"
#include "PanelItem.h"

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

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

// CSGSCheApp:
// �йش����ʵ�֣������ SGSChe.cpp
//

class CSGSCheApp : public CWinApp
{
public:
	CSGSCheApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSGSCheApp theApp;