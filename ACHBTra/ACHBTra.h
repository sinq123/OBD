
// ACHBTra.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CACHBTraApp:
// �йش����ʵ�֣������ ACHBTra.cpp
//

class CACHBTraApp : public CWinApp
{
public:
	CACHBTraApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CACHBTraApp theApp;