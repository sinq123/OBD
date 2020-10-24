// LogIn.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LogIn.h"


#include "LogInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern AFX_EXTENSION_MODULE LogInDLL;

DWORD AFX_EXT_API ShowLogIn(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/* = NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(LogInDLL.hResource);

	CLogInDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}


