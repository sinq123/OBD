// NHConfigINI.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "assert.h"
#include <fstream>
#include <time.h>
#include <tchar.h>
#include "NHConfigINI.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

//#ifdef _MANAGED
//#pragma managed(push, off)
//#endif
//
//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//					 )
//{
//    return TRUE;
//}
//
//#ifdef _MANAGED
//#pragma managed(pop)
//#endif

// 配置文件版本号
#define VERSION L"1.0.0.20090815"

void GetIniNHClient(PNHCLIENT const pIniNHClient)
{
	// 获取NHClient.ini路径
	wchar_t wchPathNHClient[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPathNHClient))
	{
		return;
	}
	// 获取NHClient.ini信息
	GetPrivateProfileString(L"System", L"SystemType", L"", pIniNHClient->wchSystemType, 10, wchPathNHClient);
	GetPrivateProfileString(L"System", L"LineNumber", L"",pIniNHClient->wchLineNumber, 50, wchPathNHClient);
	GetPrivateProfileString(L"System", L"Version", L"", pIniNHClient->wchVersion, 25, wchPathNHClient);

}

void SetIniNHClient(const NHCLIENT& iniNHClient)
{
}