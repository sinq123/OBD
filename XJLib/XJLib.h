
// XJLib.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXJLibApp:
// �йش����ʵ�֣������ XJLib.cpp
//

class CXJLibApp : public CWinApp
{
public:
	CXJLibApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXJLibApp theApp;