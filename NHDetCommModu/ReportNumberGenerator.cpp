// ReportNumberGenerator.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "ReportNumberGenerator.h"
#include <cassert>
#include <fstream>
#include <tchar.h>

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

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


//DWORD __stdcall GenerateReportNumber_SQLite(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber/*=50*/)
//{
//	assert(NULL != pwchReportNumber);
//
//	wchar_t *pwchPath = new wchar_t[MAX_PATH]();
//
//	// 读取检测站信息
//	ZeroMemory(pwchPath, sizeof(pwchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", pwchPath))
//	{
//		// 暂不做任何操作
//	}
//	wchar_t wchStationNumber[MAX_PATH];
//	ZeroMemory(wchStationNumber, sizeof(wchStationNumber));
//	GetPrivateProfileStringW(L"StationInfo", L"StationNumber", L"", wchStationNumber, 10, pwchPath);
//
//	// 读取检测线信息
//	ZeroMemory(pwchPath, sizeof(pwchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", pwchPath))
//	{
//		// 暂不做任何操作
//	}
//	wchar_t wchLineNumber[MAX_PATH];
//	ZeroMemory(wchLineNumber, sizeof(wchLineNumber));
//	GetPrivateProfileStringW(L"LineInfo", L"LineNumber", L"", wchLineNumber, 10, pwchPath);
//
//	// 读系统时间
//	wchar_t wchDateTime[MAX_PATH];
//	ZeroMemory(wchDateTime, sizeof(wchDateTime));
//	SYSTEMTIME stNow;
//	::GetLocalTime(&stNow);
//	wsprintfW(wchDateTime, 
//		L"%02hu%02hu%02hu",
//		stNow.wYear,
//		stNow.wMonth,
//		stNow.wDay);
//
//	// 读顺序号
//	int nAmount(0);
//	if (0x00 == CNHModeClientDB_SQLite::GetDboTestAmount(wchLineNumber, stNow, nAmount))
//	{
//		// 写顺序号
//		++nAmount;
//		CNHModeClientDB_SQLite::SetDboTestAmount(wchLineNumber, stNow, nAmount);
//
//		// 生成包含顺序号的检测报告编号格式，其中顺序号是4位：检测站编号-检测线编号-YYYYMMDD(sn)(sn)(sn)(sn)
//		swprintf(pwchReportNumber, nLengthOfReportNumber, L"%s-%s-%04hu%02hu%02hu%04d", 
//			wchStationNumber, 
//			wchLineNumber, 
//			stNow.wYear, 
//			stNow.wMonth, 
//			stNow.wDay, 
//			nAmount);
//	}
//	else
//	{
//		// 读顺序号失败
//
//		// 生成不包含顺序号的检测报告编号格式，MS代表毫秒，共3位：检测站编号-检测线编号-YYYYMMDDHHMMSS(MS)(MS)(MS)
//		swprintf(pwchReportNumber, nLengthOfReportNumber, L"%s-%s-%04hu%02hu%02hu%02hu%02hu%02hu%03hu", 
//			wchStationNumber, 
//			wchLineNumber, 
//			stNow.wYear, 
//			stNow.wMonth, 
//			stNow.wDay, 
//			stNow.wHour,
//			stNow.wMinute,
//			stNow.wSecond,
//			stNow.wMilliseconds);
//	}
//
//	delete[] pwchPath;
//	pwchPath = NULL;
//
//	return 0x00;
//}

DWORD GenerateReportNumber_SQLServer(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber/*=50*/)
{
	assert(NULL != pwchReportNumber);

	wchar_t *pwchPath = new wchar_t[MAX_PATH]();

	// 读取检测站信息
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", pwchPath))
	{
		// 暂不做任何操作
	}
	wchar_t wchStationNumber[MAX_PATH];
	ZeroMemory(wchStationNumber, sizeof(wchStationNumber));
	GetPrivateProfileStringW(L"StationInfo", L"StationNumber", L"", wchStationNumber, 10, pwchPath);

	// 读取检测线信息
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", pwchPath))
	{
		// 暂不做任何操作
	}
	wchar_t wchLineNumber[MAX_PATH];
	ZeroMemory(wchLineNumber, sizeof(wchLineNumber));
	GetPrivateProfileStringW(L"LineInfo", L"LineNumber", L"", wchLineNumber, 10, pwchPath);

	// 读取报告编号规则
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", pwchPath))
	{
		// 暂不做任何操作
	}
	wchar_t wchDistrictCode[MAX_PATH] = {0};
	GetPrivateProfileStringW(L"ReportNumber", L"DistrictCode", L"440605", wchDistrictCode, 10, pwchPath);
	wchar_t wchStationCode[MAX_PATH] = {0};
	GetPrivateProfileStringW(L"ReportNumber", L"StationCode", L"01", wchStationCode, 10, pwchPath);

	// 读系统时间
	wchar_t wchDateTime[MAX_PATH];
	ZeroMemory(wchDateTime, sizeof(wchDateTime));
	SYSTEMTIME stNow;
	::GetLocalTime(&stNow);
	wsprintfW(wchDateTime, 
		L"%02hu%02hu%02hu",
		stNow.wYear,
		stNow.wMonth,
		stNow.wDay);

	// 检测次数统计结构
	TESTAMOUNT sTestAmount;
	ZeroMemory(&sTestAmount, sizeof(sTestAmount));
	if (0x00 == GetDboTestAmount(wchStationNumber, stNow, &sTestAmount))
	{
		// 写顺序号
		int nAmount = _wtoi(sTestAmount.wchAllAmount);
		nAmount++;
		swprintf_s(sTestAmount.wchAllAmount, NH_INT_TO_WCHART, L"%d", nAmount);
		SetDboTestAmount(wchStationNumber, stNow, sTestAmount);

		// 生成包含顺序号的检测报告编号格式，其中顺序号是4位：行政区划代号-检验机构顺序号-YYYYMMDDHHMMSS(sn)
		swprintf(pwchReportNumber, nLengthOfReportNumber, L"%s%s%02hu%02hu%02hu%02hu%02hu%02hu%04d", 
			wchDistrictCode, 
			wchStationCode, 
			stNow.wYear%100, 
			stNow.wMonth, 
			stNow.wDay, 
			stNow.wHour,
			stNow.wMinute,
			stNow.wSecond,
			nAmount);
	}
	else
	{
		// 读顺序号失败

		// 生成不包含顺序号的检测报告编号格式，MS代表毫秒，共4位：行政区划代号-检验机构顺序号-YYYYMMDDHHMMSS(MS)
		swprintf(pwchReportNumber, nLengthOfReportNumber, L"%s%s%02hu%02hu%02hu%02hu%02hu%02hu%04hu", 
			wchDistrictCode, 
			wchStationCode, 
			stNow.wYear%100, 
			stNow.wMonth, 
			stNow.wDay, 
			stNow.wHour,
			stNow.wMinute,
			stNow.wSecond,
			stNow.wMilliseconds);
	}

	delete[] pwchPath;
	pwchPath = NULL;

	return 0x00;
}

DWORD GenerateReportNumber_StationTestAmount(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber/*=50*/)
{
	assert(NULL != pwchReportNumber);

	wchar_t *pwchPath = new wchar_t[MAX_PATH]();

	// 读取检测站信息
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", pwchPath))
	{
		// 暂不做任何操作
	}
	wchar_t wchStationNumber[MAX_PATH];
	ZeroMemory(wchStationNumber, sizeof(wchStationNumber));
	GetPrivateProfileStringW(L"StationInfo", L"StationNumber", L"", wchStationNumber, 10, pwchPath);

	// 读取检测线信息
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", pwchPath))
	{
		// 暂不做任何操作
	}
	wchar_t wchLineNumber[MAX_PATH];
	ZeroMemory(wchLineNumber, sizeof(wchLineNumber));
	GetPrivateProfileStringW(L"LineInfo", L"LineNumber", L"", wchLineNumber, 10, pwchPath);

	// 读系统时间
	wchar_t wchDateTime[MAX_PATH];
	ZeroMemory(wchDateTime, sizeof(wchDateTime));
	SYSTEMTIME stNow;
	::GetLocalTime(&stNow);
	wsprintfW(wchDateTime, 
		L"%02hu%02hu%02hu",
		stNow.wYear,
		stNow.wMonth,
		stNow.wDay);

	// 检测次数统计结构
	TESTAMOUNT sTestAmount;
	ZeroMemory(&sTestAmount, sizeof(sTestAmount));
	if (0x00 == GetDboStationTestAmount(stNow, &sTestAmount))
	{
		// 写顺序号
		int nAmount = _wtoi(sTestAmount.wchAllAmount);
		nAmount++;
		swprintf_s(sTestAmount.wchAllAmount, NH_INT_TO_WCHART, L"%d", nAmount);
		SetDboStationTestAmount(stNow, sTestAmount);

		// 生成包含顺序号的检测报告编号格式，其中顺序号是4位：检测站编号-检测线编号-YYYYMMDD(sn)(sn)(sn)(sn)
		swprintf(pwchReportNumber, nLengthOfReportNumber, L"%s-%s-%04hu%02hu%02hu%04d", 
			wchStationNumber, 
			wchLineNumber, 
			stNow.wYear, 
			stNow.wMonth, 
			stNow.wDay, 
			nAmount);
	}
	else
	{
		// 读顺序号失败

		// 生成不包含顺序号的检测报告编号格式，MS代表毫秒，共3位：检测站编号-检测线编号-YYYYMMDDHHMMSS(MS)(MS)(MS)
		swprintf(pwchReportNumber, nLengthOfReportNumber, L"%s-%s-%04hu%02hu%02hu%02hu%02hu%02hu%03hu", 
			wchStationNumber, 
			wchLineNumber, 
			stNow.wYear, 
			stNow.wMonth, 
			stNow.wDay, 
			stNow.wHour,
			stNow.wMinute,
			stNow.wSecond,
			stNow.wMilliseconds);
	}

	delete[] pwchPath;
	pwchPath = NULL;

	return 0x00;
}