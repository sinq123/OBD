
// JNSX.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CJNSXApp:
// �йش����ʵ�֣������ JNSX.cpp
//

class CJNSXApp : public CWinApp
{
public:
	CJNSXApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJNSXApp theApp;