
// SGSCheck.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "XPGroupBox.h"
#include "Meter.h"
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
// CSGSCheckApp:
// �йش����ʵ�֣������ SGSCheck.cpp
//

class CSGSCheckApp : public CWinApp
{
public:
	CSGSCheckApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSGSCheckApp theApp;