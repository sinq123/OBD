
// YJSocketN.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYJSocketNApp:
// �йش����ʵ�֣������ YJSocketN.cpp
//

class CYJSocketNApp : public CWinApp
{
public:
	CYJSocketNApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CYJSocketNApp theApp;