
// HttpServerTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHttpServerTestApp:
// �йش����ʵ�֣������ HttpServerTest.cpp
//

class CHttpServerTestApp : public CWinApp
{
public:
	CHttpServerTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHttpServerTestApp theApp;