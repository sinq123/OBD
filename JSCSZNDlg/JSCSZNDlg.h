
// JSCSZNDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CJSCSZNDlgApp:
// �йش����ʵ�֣������ JSCSZNDlg.cpp
//

class CJSCSZNDlgApp : public CWinApp
{
public:
	CJSCSZNDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJSCSZNDlgApp theApp;