
// DRSXLN.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDRSXLNApp:
// �йش����ʵ�֣������ DRSXLN.cpp
//

class CDRSXLNApp : public CWinApp
{
public:
	CDRSXLNApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDRSXLNApp theApp;