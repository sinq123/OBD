
// ACHBTra_2.0.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CACHBTra_20App:
// �йش����ʵ�֣������ ACHBTra_2.0.cpp
//

class CACHBTra_20App : public CWinApp
{
public:
	CACHBTra_20App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CACHBTra_20App theApp;