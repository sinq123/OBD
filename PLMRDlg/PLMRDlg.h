
// PLMRDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPLMRDlgApp:
// �йش����ʵ�֣������ PLMRDlg.cpp
//

class CPLMRDlgApp : public CWinApp
{
public:
	CPLMRDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPLMRDlgApp theApp;