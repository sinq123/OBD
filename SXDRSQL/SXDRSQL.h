
// SXDRSQL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSXDRSQLApp:
// �йش����ʵ�֣������ SXDRSQL.cpp
//

class CSXDRSQLApp : public CWinApp
{
public:
	CSXDRSQLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSXDRSQLApp theApp;