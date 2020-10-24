#include "StdAfx.h"
#include "BJCLXXBService.h"
#include <cassert>


CBJCLXXBService::CBJCLXXBService(void)
{
}


CBJCLXXBService::~CBJCLXXBService(void)
{
}

DWORD CBJCLXXBService::GetBJCLXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SBJCLXXB &sBJCLXXB)
{
	assert(NULL != pConnection);
	assert(NULL != pwchSql);

	DWORD dwReturn(0x00);
	_RecordsetPtr pRecordset(NULL);
	if (0x00 != OpenQuery(pConnection, pRecordset, pwchSql))
	{
		// 打开查询失败

		dwReturn = 0xFFFFFFFF;
	}
	else
	{
		// 打开查询成功

		if (!pRecordset->adoEOF)
		{
			FieldToStruct(pRecordset, sBJCLXXB);

			dwReturn = 0x01;
		}
		else
		{
			dwReturn = 0x00;
		}
	}

	if (0x00 != CloseQuery(pRecordset))
	{
		// 关闭查询失败

		dwReturn = 0xFFFFFFFF;
	}

	return dwReturn;  
}

DWORD CBJCLXXBService::GetBJCLXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SBJCLXXB> &lsBJCLXXB)
{ 
	assert(NULL != pConnection);
	assert(NULL != pwchSql);

	lsBJCLXXB.clear();
	DWORD dwReturn(0x00);
	_RecordsetPtr pRecordset(NULL);
	if (0x00 != OpenQuery(pConnection, pRecordset, pwchSql))
	{
		// 打开查询失败

		dwReturn = 0xFFFFFFFF;
	}
	else
	{
		// 打开查询成功

		try
		{
			SBJCLXXB sBJCLXXB;
			while (!pRecordset->adoEOF)
			{
				FieldToStruct(pRecordset, sBJCLXXB);
				lsBJCLXXB.push_back(sBJCLXXB);
				pRecordset->MoveNext();
			}
			dwReturn = static_cast<DWORD>(lsBJCLXXB.size());
		}
		catch (_com_error& e)
		{
			OutputDBErrMsg(e);
			dwReturn = 0xFFFFFFFF;
		}
	}

	if (0x00 != CloseQuery(pRecordset))
	{
		// 关闭查询失败

		dwReturn = 0xFFFFFFFF;
	}

	return dwReturn;  
}

DWORD CBJCLXXBService::SetBJCLXXBData(const _ConnectionPtr &pConnection, const SBJCLXXB &sBJCLXXB)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	_snwprintf_s(wchSql,_countof(wchSql) ,_TRUNCATE, L"SELECT * FROM BJCLXXB WHERE JCBH = '%s'", sBJCLXXB.strJCBH.c_str());
	_RecordsetPtr pRecordset(NULL);
	if (0x00 != OpenQuery(pConnection, pRecordset, wchSql))
	{
		// 打开查询失败

		dwReturn = 0xFFFFFFFF;
	}
	else
	{
		// 打开查询成功

		try
		{
			// 有则改，无则加
			if (pRecordset->adoEOF) 
			{
				pRecordset->AddNew();
			}
			StructToField(pRecordset, sBJCLXXB);
			pRecordset->Update();
			dwReturn = 0x01;
		}
		catch (_com_error& e)
		{
			OutputDBErrMsg(e);
			dwReturn = 0xFFFFFFFF;
		}
	}

	if (0x00 != CloseQuery(pRecordset))
	{
		// 关闭查询失败

		dwReturn = 0xFFFFFFFF;
	}

	return dwReturn;  
}

DWORD CBJCLXXBService::SetBJCLXXBData(const _ConnectionPtr &pConnection, const std::list<SBJCLXXB> &lsBJCLXXB)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	wcsncpy_s(wchSql, _countof(wchSql), L"SELECT * FROM BJCLXXB", _TRUNCATE);
	_RecordsetPtr pRecordset(NULL);
	if (0x00 != OpenQuery(pConnection, pRecordset, wchSql))
	{
		// 打开查询失败

		dwReturn = 0xFFFFFFFF;
	}
	else
	{
		// 打开查询成功

		try
		{
			std::list<SBJCLXXB>::const_iterator iter = lsBJCLXXB.begin();
			for ( ; iter != lsBJCLXXB.end(); iter++)
			{
				pRecordset->AddNew();
				StructToField(pRecordset, *iter);
			}
			pRecordset->Update();
			dwReturn = static_cast<DWORD>(lsBJCLXXB.size());
		}
		catch (_com_error& e)
		{
			OutputDBErrMsg(e);
			dwReturn = 0xFFFFFFFF;
		}
	}

	if  (0x00 != CloseQuery(pRecordset))
	{
		// 关闭查询失败

		dwReturn = 0xFFFFFFFF;
	}

	return dwReturn;  
}

DWORD CBJCLXXBService::SetBJCLXXBData_NonEmpty(const _ConnectionPtr &pConnection, const SBJCLXXB &sBJCLXXB)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	_snwprintf_s(wchSql,_countof(wchSql) ,_TRUNCATE, L"SELECT * FROM BJCLXXB WHERE JCBH = '%s'", sBJCLXXB.strJCBH.c_str());
	_RecordsetPtr pRecordset(NULL);
	if (0x00 != OpenQuery(pConnection, pRecordset, wchSql))
	{
		// 打开查询失败

		dwReturn = 0xFFFFFFFF;
	}
	else
	{
		// 打开查询成功

		try
		{
			// 有则改，无则加
			if (pRecordset->adoEOF) 
			{
				pRecordset->AddNew();
			}
			StructToField_NonEmpty(pRecordset, sBJCLXXB);
			pRecordset->Update();
			dwReturn = 0x01;
		}
		catch (_com_error& e)
		{
			OutputDBErrMsg(e);
			dwReturn = 0xFFFFFFFF;
		}
	}

	if (0x00 != CloseQuery(pRecordset))
	{
		// 关闭查询失败

		dwReturn = 0xFFFFFFFF;
	}

	return dwReturn;  
}

DWORD CBJCLXXBService::FieldToStruct(const _RecordsetPtr &pRecordset, SBJCLXXB &sBJCLXXB)
{
	_variant_t v;
	wchar_t wchBuf[1024] = {L'\0'};

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCBH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCBH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCCS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCCS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"PZLX");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strPZLX = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCCLPH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCCLPH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CLXHBH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCLXHBH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CCRQ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		DATE dt = static_cast<DATE>(v);
		SYSTEMTIME st;
		VariantTimeToSystemTime(dt, &st);
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%04hu-%02hu-%02hu %02hu:%02hu:%02hu", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCCRQ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"DJRQ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		DATE dt = static_cast<DATE>(v);
		SYSTEMTIME st;
		VariantTimeToSystemTime(dt, &st);
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%04hu-%02hu-%02hu %02hu:%02hu:%02hu", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strDJRQ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"FDJH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strFDJH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CJH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCJH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CZ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCZ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CZDH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCZDH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CZDZ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCZDZ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"LCBDS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strLCBDS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"HBBZ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strHBBZ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"SYQK");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strSYQK = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCBJ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCBJ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCZT");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCZT = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"QRJCFF");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strQRJCFF = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"RYLX");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strRYLX = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"GYFF");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strGYFF = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"BSXLX");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strBSXLX = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CLLX");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strCLLX = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"ZBZL");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strZBZL = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"ZDZZL");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strZDZZL = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"FDJPL");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strFDJPL = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"FDJSCS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strFDJSCS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCZH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCZH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"DCZZ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strDCZZ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"FDJEDGL");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strFDJEDGL = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"FDJEDZS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strFDJEDZS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"PQGSL");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strPQGSL = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"PFBZ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strPFBZ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"ZZCS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strZZCS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"QGS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strQGS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"QDXS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strQDXS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"QDLQY");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strQDLQY = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCYH");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCYH = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"DWS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strDWS = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"HDZK");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strHDZK = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JCRQ");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		DATE dt = static_cast<DATE>(v);
		SYSTEMTIME st;
		VariantTimeToSystemTime(dt, &st);
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%04hu-%02hu-%02hu %02hu:%02hu:%02hu", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJCRQ = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"JQXS");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sBJCLXXB.strJQXS = wchBuf;

	return 0x00;
}

DWORD CBJCLXXBService::StructToField(const _RecordsetPtr &pRecordset, const SBJCLXXB &sBJCLXXB)
{
	std::wstring str;
	_variant_t v;

	str = sBJCLXXB.strJCBH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCBH");

	str = sBJCLXXB.strJCCS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCCS");

	str = sBJCLXXB.strPZLX;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"PZLX");

	str = sBJCLXXB.strJCCLPH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCCLPH");

	str = sBJCLXXB.strCLXHBH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CLXHBH");

	str = sBJCLXXB.strCCRQ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
		v.ChangeType(VT_DATE);
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CCRQ");

	str = sBJCLXXB.strDJRQ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
		v.ChangeType(VT_DATE);
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"DJRQ");

	str = sBJCLXXB.strFDJH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"FDJH");

	str = sBJCLXXB.strCJH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CJH");

	str = sBJCLXXB.strCZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CZ");

	str = sBJCLXXB.strCZDH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CZDH");

	str = sBJCLXXB.strCZDZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CZDZ");

	str = sBJCLXXB.strLCBDS;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"LCBDS");

	str = sBJCLXXB.strHBBZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"HBBZ");

	str = sBJCLXXB.strSYQK;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"SYQK");

	str = sBJCLXXB.strJCBJ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCBJ");

	str = sBJCLXXB.strJCZT;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCZT");

	str = sBJCLXXB.strQRJCFF;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"QRJCFF");

	str = sBJCLXXB.strRYLX;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"RYLX");

	str = sBJCLXXB.strGYFF;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"GYFF");

	str = sBJCLXXB.strBSXLX;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"BSXLX");

	str = sBJCLXXB.strCLLX;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CLLX");

	str = sBJCLXXB.strZBZL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"ZBZL");

	str = sBJCLXXB.strZDZZL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"ZDZZL");

	str = sBJCLXXB.strFDJPL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"FDJPL");

	str = sBJCLXXB.strFDJSCS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"FDJSCS");

	str = sBJCLXXB.strJCZH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCZH");

	str = sBJCLXXB.strDCZZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"DCZZ");

	str = sBJCLXXB.strFDJEDGL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"FDJEDGL");

	str = sBJCLXXB.strFDJEDZS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"FDJEDZS");

	str = sBJCLXXB.strPQGSL;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"PQGSL");

	str = sBJCLXXB.strPFBZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"PFBZ");

	str = sBJCLXXB.strZZCS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"ZZCS");

	str = sBJCLXXB.strQGS;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"QGS");

	str = sBJCLXXB.strQDXS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"QDXS");

	str = sBJCLXXB.strQDLQY;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"QDLQY");

	str = sBJCLXXB.strJCYH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCYH");

	str = sBJCLXXB.strDWS;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"DWS");

	str = sBJCLXXB.strHDZK;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"HDZK");

	str = sBJCLXXB.strJCRQ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
		v.ChangeType(VT_DATE);
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JCRQ");

	str = sBJCLXXB.strJQXS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"JQXS");
	return 0x00;
}

DWORD CBJCLXXBService::StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SBJCLXXB &sBJCLXXB)
{
	std::wstring str;
	_variant_t v;

	str = sBJCLXXB.strJCBH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCBH");
	}
	str = sBJCLXXB.strJCCS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCCS");
	}
	str = sBJCLXXB.strPZLX;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"PZLX");
	}
	str = sBJCLXXB.strJCCLPH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCCLPH");
	}
	str = sBJCLXXB.strCLXHBH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CLXHBH");
	}
	str = sBJCLXXB.strCCRQ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
		v.ChangeType(VT_DATE);

		PutFieldValue(pRecordset, v, L"CCRQ");
	}
	str = sBJCLXXB.strDJRQ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
		v.ChangeType(VT_DATE);

		PutFieldValue(pRecordset, v, L"DJRQ");
	}
	str = sBJCLXXB.strFDJH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"FDJH");
	}
	str = sBJCLXXB.strCJH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CJH");
	}
	str = sBJCLXXB.strCZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CZ");
	}
	str = sBJCLXXB.strCZDH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CZDH");
	}
	str = sBJCLXXB.strCZDZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CZDZ");
	}
	str = sBJCLXXB.strLCBDS;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"LCBDS");
	}
	str = sBJCLXXB.strHBBZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"HBBZ");
	}
	str = sBJCLXXB.strSYQK;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"SYQK");
	}
	str = sBJCLXXB.strJCBJ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCBJ");
	}
	str = sBJCLXXB.strJCZT;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCZT");
	}
	str = sBJCLXXB.strQRJCFF;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"QRJCFF");
	}
	str = sBJCLXXB.strRYLX;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"RYLX");
	}
	str = sBJCLXXB.strGYFF;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"GYFF");
	}
	str = sBJCLXXB.strBSXLX;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"BSXLX");
	}
	str = sBJCLXXB.strCLLX;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CLLX");
	}
	str = sBJCLXXB.strZBZL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"ZBZL");
	}
	str = sBJCLXXB.strZDZZL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"ZDZZL");
	}
	str = sBJCLXXB.strFDJPL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"FDJPL");
	}
	str = sBJCLXXB.strFDJSCS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"FDJSCS");
	}
	str = sBJCLXXB.strJCZH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCZH");
	}
	str = sBJCLXXB.strDCZZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"DCZZ");
	}
	str = sBJCLXXB.strFDJEDGL;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"FDJEDGL");
	}
	str = sBJCLXXB.strFDJEDZS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"FDJEDZS");
	}
	str = sBJCLXXB.strPQGSL;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"PQGSL");
	}
	str = sBJCLXXB.strPFBZ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"PFBZ");
	}
	str = sBJCLXXB.strZZCS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"ZZCS");
	}
	str = sBJCLXXB.strQGS;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"QGS");
	}
	str = sBJCLXXB.strQDXS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"QDXS");
	}
	str = sBJCLXXB.strQDLQY;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"QDLQY");
	}
	str = sBJCLXXB.strJCYH;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JCYH");
	}
	str = sBJCLXXB.strDWS;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"DWS");
	}
	str = sBJCLXXB.strHDZK;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"HDZK");
	}
	str = sBJCLXXB.strJCRQ;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
		v.ChangeType(VT_DATE);

		PutFieldValue(pRecordset, v, L"JCRQ");
	}
	str = sBJCLXXB.strJQXS;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"JQXS");
	}

	return 0x00;
}