
// SYSQL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSYSQLApp:
// �йش����ʵ�֣������ SYSQL.cpp
//

class CSYSQLApp : public CWinApp
{
public:
	CSYSQLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSYSQLApp theApp;