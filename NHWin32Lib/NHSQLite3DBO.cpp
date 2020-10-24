// NHSQLite3DBO.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "NHSQLite3DBO.h"
#include <cassert>
#include <io.h>
#include <malloc.h>

#include "NHCommonAPI.h"

DWORD CNHSQLite3DBO::OpenDB(CppSQLite3DB &db)
{
	DWORD dwReturn(0x00);

	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"DB", L"NHModeClientDB.db", wchPath);

	char *pchPath(NULL);
	pchPath = CNHCommonAPI::UnicodeToANSI(wchPath);

	try
	{
		db.open(pchPath);
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);
		dwReturn = 0x01;
	}

	if (NULL != pchPath)
	{
		free(pchPath);
		pchPath = NULL;
	}

	return dwReturn;
}

DWORD CNHSQLite3DBO::OpenDB(CppSQLite3DB &db, const wchar_t *const pwchDBPath)
{
	assert(NULL != pwchDBPath);

	DWORD dwReturn(0x00);

	char *pchPath(NULL);
	pchPath = CNHCommonAPI::UnicodeToANSI(pwchDBPath);

	try
	{
		db.open(pchPath);
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);
		dwReturn = 0x01;
	}

	if (NULL != pchPath)
	{
		free(pchPath);
		pchPath = NULL;
	}

	return dwReturn;
}

DWORD CNHSQLite3DBO::CloseDB(CppSQLite3DB &db)
{
	DWORD dwReturn(0x00);

	try
	{
		db.close();
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);
		dwReturn = 0x01;
	}

	return dwReturn;
}

DWORD CNHSQLite3DBO::OpenQuery(CppSQLite3DB &db, CppSQLite3Query &query, const wchar_t *const pwchSQL)
{
	assert(NULL != pwchSQL);

	DWORD dwReturn(0x00);

	char *pchSQL(NULL);
	pchSQL = CNHCommonAPI::UnicodeToUTF8(pwchSQL);
	try
	{
		query = db.execQuery(pchSQL);
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);
		dwReturn = 0x01;
	}
	if (NULL != pchSQL)
	{
		free(pchSQL);
		pchSQL = NULL;
	}

	return dwReturn;
}

DWORD CNHSQLite3DBO::CloseQuery(CppSQLite3Query &query)
{
	DWORD dwReturn(0x00);

	try
	{
		query.finalize();
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);
		dwReturn = 0x01;
	}

	return dwReturn;
}

DWORD CNHSQLite3DBO::ExecuteDML(CppSQLite3DB &db, const wchar_t *const pwchSQL, int *const pnRowsInvolved/*=NULL*/)
{
	assert(NULL != pwchSQL);

	DWORD dwReturn(0x00);

	char *pUtf8SQL(NULL);
	pUtf8SQL = CNHCommonAPI::UnicodeToUTF8(pwchSQL);
	try
	{
		const int nRowsInvolved = db.execDML(pUtf8SQL);
		if (NULL != pnRowsInvolved)
		{
			*pnRowsInvolved = nRowsInvolved;
		}
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);
		dwReturn = 0x01;
	}

	if (NULL != pUtf8SQL)
	{
		free(pUtf8SQL);
		pUtf8SQL = NULL;
	}

	return dwReturn;
}

DWORD CNHSQLite3DBO::GetFieldValue(CppSQLite3Query &query, wchar_t *const pwchFieldValue, const int nFieldValueLength, const wchar_t *const pwchFieldName)
{
	assert(NULL != pwchFieldValue);
	assert(NULL != pwchFieldName);

	DWORD dwReturn(0x00);

	wchar_t *pwch(NULL);
	char *pchFieldName(NULL);
	pchFieldName = CNHCommonAPI::UnicodeToANSI(pwchFieldName);
	try
	{
		pwch = CNHCommonAPI::UTF8ToUnicode(query.fieldValue(pchFieldName));
		wcsncpy_s(pwchFieldValue, nFieldValueLength, pwch, nFieldValueLength-1);
	}
	catch (CppSQLite3Exception &e)
	{
		OutputDBErrMsg(e);

		dwReturn =  0x01;
	}

	if (NULL != pwch)
	{
		free(pwch);
		pwch = NULL;
	}
	if (NULL != pchFieldName)
	{
		free(pchFieldName);
		pchFieldName = NULL;
	}

	return dwReturn;
}

void CNHSQLite3DBO::OutputDBErrMsg(CppSQLite3Exception &e)
{
	const int nErrMsgLength(MAX_PATH*5);
	char *pchErrMsg = new char[nErrMsgLength]();
	_snprintf_s(pchErrMsg, nErrMsgLength/sizeof(char), _TRUNCATE, "错误编号:%d  错误信息:%s",
		e.errorCode(),
		e.errorMessage());

	// ANSIToUnicode
	wchar_t *pwchErrMsg(NULL);
	pwchErrMsg = CNHCommonAPI::ANSIToUnicode(pchErrMsg);

	// 输出错误信息到输出窗口
	OutputDebugStringW(L"\n");
	OutputDebugStringW(pwchErrMsg);
	OutputDebugStringW(L"\n");

	if (NULL != pchErrMsg)
	{
		delete[] pchErrMsg;
		pchErrMsg = NULL;
	}
	if (NULL != pwchErrMsg)
	{
		free(pwchErrMsg);
		pwchErrMsg = NULL;
	}
}
