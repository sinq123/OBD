
// NHOBDPDA.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNHOBDPDAApp:
// �йش����ʵ�֣������ NHOBDPDA.cpp
//

class CNHOBDPDAApp : public CWinApp
{
public:
	CNHOBDPDAApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNHOBDPDAApp theApp;