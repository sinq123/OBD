
// HNSYDLG.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHNSYDLGApp:
// �йش����ʵ�֣������ HNSYDLG.cpp
//

class CHNSYDLGApp : public CWinApp
{
public:
	CHNSYDLGApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHNSYDLGApp theApp;