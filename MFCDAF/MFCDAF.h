
// MFCDAF.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCDAFApp:
// �йش����ʵ�֣������ MFCDAF.cpp
//

class CMFCDAFApp : public CWinApp
{
public:
	CMFCDAFApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCDAFApp theApp;