// ReportNumberGenerator.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ReportNumberGenerator.h"
#include <cassert>
#include <fstream>
#include <tchar.h>

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
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

//�ָ���ĺ���Ϊ���
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
//	// ��ȡ���վ��Ϣ
//	ZeroMemory(pwchPath, sizeof(pwchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", pwchPath))
//	{
//		// �ݲ����κβ���
//	}
//	wchar_t wchStationNumber[MAX_PATH];
//	ZeroMemory(wchStationNumber, sizeof(wchStationNumber));
//	GetPrivateProfileStringW(L"StationInfo", L"StationNumber", L"", wchStationNumber, 10, pwchPath);
//
//	// ��ȡ�������Ϣ
//	ZeroMemory(pwchPath, sizeof(pwchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", pwchPath))
//	{
//		// �ݲ����κβ���
//	}
//	wchar_t wchLineNumber[MAX_PATH];
//	ZeroMemory(wchLineNumber, sizeof(wchLineNumber));
//	GetPrivateProfileStringW(L"LineInfo", L"LineNumber", L"", wchLineNumber, 10, pwchPath);
//
//	// ��ϵͳʱ��
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
//	// ��˳���
//	int nAmount(0);
//	if (0x00 == CNHModeClientDB_SQLite::GetDboTestAmount(wchLineNumber, stNow, nAmount))
//	{
//		// д˳���
//		++nAmount;
//		CNHModeClientDB_SQLite::SetDboTestAmount(wchLineNumber, stNow, nAmount);
//
//		// ���ɰ���˳��ŵļ�ⱨ���Ÿ�ʽ������˳�����4λ�����վ���-����߱��-YYYYMMDD(sn)(sn)(sn)(sn)
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
//		// ��˳���ʧ��
//
//		// ���ɲ�����˳��ŵļ�ⱨ���Ÿ�ʽ��MS������룬��3λ�����վ���-����߱��-YYYYMMDDHHMMSS(MS)(MS)(MS)
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

	// ��ȡ���վ��Ϣ
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", pwchPath))
	{
		// �ݲ����κβ���
	}
	wchar_t wchStationNumber[MAX_PATH];
	ZeroMemory(wchStationNumber, sizeof(wchStationNumber));
	GetPrivateProfileStringW(L"StationInfo", L"StationNumber", L"", wchStationNumber, 10, pwchPath);

	// ��ȡ�������Ϣ
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", pwchPath))
	{
		// �ݲ����κβ���
	}
	wchar_t wchLineNumber[MAX_PATH];
	ZeroMemory(wchLineNumber, sizeof(wchLineNumber));
	GetPrivateProfileStringW(L"LineInfo", L"LineNumber", L"", wchLineNumber, 10, pwchPath);

	// ��ȡ�����Ź���
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", pwchPath))
	{
		// �ݲ����κβ���
	}
	wchar_t wchDistrictCode[MAX_PATH] = {0};
	GetPrivateProfileStringW(L"ReportNumber", L"DistrictCode", L"440605", wchDistrictCode, 10, pwchPath);
	wchar_t wchStationCode[MAX_PATH] = {0};
	GetPrivateProfileStringW(L"ReportNumber", L"StationCode", L"01", wchStationCode, 10, pwchPath);

	// ��ϵͳʱ��
	wchar_t wchDateTime[MAX_PATH];
	ZeroMemory(wchDateTime, sizeof(wchDateTime));
	SYSTEMTIME stNow;
	::GetLocalTime(&stNow);
	wsprintfW(wchDateTime, 
		L"%02hu%02hu%02hu",
		stNow.wYear,
		stNow.wMonth,
		stNow.wDay);

	// ������ͳ�ƽṹ
	TESTAMOUNT sTestAmount;
	ZeroMemory(&sTestAmount, sizeof(sTestAmount));
	if (0x00 == GetDboTestAmount(wchStationNumber, stNow, &sTestAmount))
	{
		// д˳���
		int nAmount = _wtoi(sTestAmount.wchAllAmount);
		nAmount++;
		swprintf_s(sTestAmount.wchAllAmount, NH_INT_TO_WCHART, L"%d", nAmount);
		SetDboTestAmount(wchStationNumber, stNow, sTestAmount);

		// ���ɰ���˳��ŵļ�ⱨ���Ÿ�ʽ������˳�����4λ��������������-�������˳���-YYYYMMDDHHMMSS(sn)
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
		// ��˳���ʧ��

		// ���ɲ�����˳��ŵļ�ⱨ���Ÿ�ʽ��MS������룬��4λ��������������-�������˳���-YYYYMMDDHHMMSS(MS)
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

	// ��ȡ���վ��Ϣ
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", pwchPath))
	{
		// �ݲ����κβ���
	}
	wchar_t wchStationNumber[MAX_PATH];
	ZeroMemory(wchStationNumber, sizeof(wchStationNumber));
	GetPrivateProfileStringW(L"StationInfo", L"StationNumber", L"", wchStationNumber, 10, pwchPath);

	// ��ȡ�������Ϣ
	ZeroMemory(pwchPath, sizeof(pwchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", pwchPath))
	{
		// �ݲ����κβ���
	}
	wchar_t wchLineNumber[MAX_PATH];
	ZeroMemory(wchLineNumber, sizeof(wchLineNumber));
	GetPrivateProfileStringW(L"LineInfo", L"LineNumber", L"", wchLineNumber, 10, pwchPath);

	// ��ϵͳʱ��
	wchar_t wchDateTime[MAX_PATH];
	ZeroMemory(wchDateTime, sizeof(wchDateTime));
	SYSTEMTIME stNow;
	::GetLocalTime(&stNow);
	wsprintfW(wchDateTime, 
		L"%02hu%02hu%02hu",
		stNow.wYear,
		stNow.wMonth,
		stNow.wDay);

	// ������ͳ�ƽṹ
	TESTAMOUNT sTestAmount;
	ZeroMemory(&sTestAmount, sizeof(sTestAmount));
	if (0x00 == GetDboStationTestAmount(stNow, &sTestAmount))
	{
		// д˳���
		int nAmount = _wtoi(sTestAmount.wchAllAmount);
		nAmount++;
		swprintf_s(sTestAmount.wchAllAmount, NH_INT_TO_WCHART, L"%d", nAmount);
		SetDboStationTestAmount(stNow, sTestAmount);

		// ���ɰ���˳��ŵļ�ⱨ���Ÿ�ʽ������˳�����4λ�����վ���-����߱��-YYYYMMDD(sn)(sn)(sn)(sn)
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
		// ��˳���ʧ��

		// ���ɲ�����˳��ŵļ�ⱨ���Ÿ�ʽ��MS������룬��3λ�����վ���-����߱��-YYYYMMDDHHMMSS(MS)(MS)(MS)
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