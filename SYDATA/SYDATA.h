
// SYDATA.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSYDATAApp:
// �йش����ʵ�֣������ SYDATA.cpp
//

class CSYDATAApp : public CWinApp
{
public:
	CSYDATAApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSYDATAApp theApp;