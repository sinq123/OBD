
// XCDyn.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXCDynApp:
// �йش����ʵ�֣������ XCDyn.cpp
//

class CXCDynApp : public CWinApp
{
public:
	CXCDynApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXCDynApp theApp;