
// CS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "..\LogIn\LogIn.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\Debug\\LogIn_D.lib" )
#else
#pragma comment( lib, "..\\Release\\LogIn.lib" )
#endif

#include "resource.h"		// ������


// CCSApp:
// �йش����ʵ�֣������ CS.cpp
//

class CCSApp : public CWinApp
{
public:
	CCSApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCSApp theApp;