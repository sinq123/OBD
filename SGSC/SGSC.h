
// SGSC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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
#include "md5.h"
#include <algorithm>

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

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

// CSGSCApp:
// �йش����ʵ�֣������ SGSC.cpp
//

class CSGSCApp : public CWinApp
{
public:
	CSGSCApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	char* m_pchURL;
	CStringW m_strkey;
	CStringW m_StationNum;
	CStringW m_LineNum;
	CStringW m_strName;
	CStringW m_strPassword;
	// ��ȡ������Ϣ
	void GetConfig(void);
	// ������־�ļ�
	CStringW m_strIntLogFilePath;
	void GetLogFilePath(void);

	bool UpIntLogIn(void);
	bool UpIntLogOut(void);
	bool IsChinese(CString& strChi);
};

extern CSGSCApp theApp;