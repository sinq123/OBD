
// HttpClientTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHttpClientTestApp:
// �йش����ʵ�֣������ HttpClientTest.cpp
//

class CHttpClientTestApp : public CWinApp
{
public:
	CHttpClientTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHttpClientTestApp theApp;