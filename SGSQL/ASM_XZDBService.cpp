#include "StdAfx.h"
#include "ASM_XZDBService.h"
#include <cassert>


CASM_XZDBService::CASM_XZDBService(void)
{
}


CASM_XZDBService::~CASM_XZDBService(void)
{
}

DWORD CASM_XZDBService::GetASM_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_XZDB &sASM_XZDB)
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
			FieldToStruct(pRecordset, sASM_XZDB);

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

DWORD CASM_XZDBService::GetASM_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_XZDB> &lsASM_XZDB)
{ 
	assert(NULL != pConnection);
	assert(NULL != pwchSql);

	lsASM_XZDB.clear();
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
			SASM_XZDB sASM_XZDB;
			while (!pRecordset->adoEOF)
			{
				FieldToStruct(pRecordset, sASM_XZDB);
				lsASM_XZDB.push_back(sASM_XZDB);
				pRecordset->MoveNext();
			}
			dwReturn = static_cast<DWORD>(lsASM_XZDB.size());
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

DWORD CASM_XZDBService::SetASM_XZDBData(const _ConnectionPtr &pConnection, const SASM_XZDB &sASM_XZDB)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	_snwprintf_s(wchSql,_countof(wchSql) ,_TRUNCATE, L"SELECT * FROM ASM_XZDB WHERE ID = '%s'", sASM_XZDB.strID.c_str());
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
			StructToField(pRecordset, sASM_XZDB);
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

DWORD CASM_XZDBService::SetASM_XZDBData(const _ConnectionPtr &pConnection, const std::list<SASM_XZDB> &lsASM_XZDB)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	wcsncpy_s(wchSql, _countof(wchSql), L"SELECT * FROM ASM_XZDB", _TRUNCATE);
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
			std::list<SASM_XZDB>::const_iterator iter = lsASM_XZDB.begin();
			for ( ; iter != lsASM_XZDB.end(); iter++)
			{
				pRecordset->AddNew();
				StructToField(pRecordset, *iter);
			}
			pRecordset->Update();
			dwReturn = static_cast<DWORD>(lsASM_XZDB.size());
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

DWORD CASM_XZDBService::SetASM_XZDBData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_XZDB &sASM_XZDB)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	_snwprintf_s(wchSql,_countof(wchSql) ,_TRUNCATE, L"SELECT * FROM ASM_XZDB WHERE ID = '%s'", sASM_XZDB.strID.c_str());
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
			StructToField_NonEmpty(pRecordset, sASM_XZDB);
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

DWORD CASM_XZDBService::FieldToStruct(const _RecordsetPtr &pRecordset, SASM_XZDB &sASM_XZDB)
{
	_variant_t v;
	wchar_t wchBuf[1024] = {L'\0'};

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"ID");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%d", (int)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strID = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701qHC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701qHC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701qCO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701qCO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701qNO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701qNO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701qHC2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701qHC2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701qCO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701qCO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701qNO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701qNO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701HC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701HC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701CO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701CO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701NO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701NO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701HC2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701HC2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701CO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701CO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d1Date20000701NO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd1Date20000701NO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001qHC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001qHC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001qCO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001qCO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001qNO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001qNO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001qHC2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001qHC2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001qNO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001qNO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001HC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001HC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001CO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001CO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001NO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001NO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001HC2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001HC2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001CO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001CO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"d2Date20011001NO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strd2Date20011001NO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qHC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qHC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qCO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qCO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qNO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qNO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qHC2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qHC2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qCO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qCO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qNO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qNO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901HC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901HC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901CO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901CO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901NO5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901NO5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901HC2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901HC2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901qHC5025");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901qHC5025 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901CO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901CO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"zxDate20040901NO2540");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strzxDate20040901NO2540 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"BZSM");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_XZDB.strBZSM = wchBuf;

	return 0x00;
}

DWORD CASM_XZDBService::StructToField(const _RecordsetPtr &pRecordset, const SASM_XZDB &sASM_XZDB)
{
	std::wstring str;
	_variant_t v;

	str = sASM_XZDB.strID;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"ID");

	str = sASM_XZDB.strd1Date20000701qHC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701qHC5025");

	str = sASM_XZDB.strd1Date20000701qCO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701qCO5025");

	str = sASM_XZDB.strd1Date20000701qNO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701qNO5025");

	str = sASM_XZDB.strd1Date20000701qHC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701qHC2540");

	str = sASM_XZDB.strd1Date20000701qCO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701qCO2540");

	str = sASM_XZDB.strd1Date20000701qNO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701qNO2540");

	str = sASM_XZDB.strd1Date20000701HC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701HC5025");

	str = sASM_XZDB.strd1Date20000701CO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701CO5025");

	str = sASM_XZDB.strd1Date20000701NO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701NO5025");

	str = sASM_XZDB.strd1Date20000701HC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701HC2540");

	str = sASM_XZDB.strd1Date20000701CO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701CO2540");

	str = sASM_XZDB.strd1Date20000701NO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d1Date20000701NO2540");

	str = sASM_XZDB.strd2Date20011001qHC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001qHC5025");

	str = sASM_XZDB.strd2Date20011001qCO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001qCO5025");

	str = sASM_XZDB.strd2Date20011001qNO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001qNO5025");

	str = sASM_XZDB.strd2Date20011001qHC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001qHC2540");

	str = sASM_XZDB.strd2Date20011001qCO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001qCO2540");

	str = sASM_XZDB.strd2Date20011001qNO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001qNO2540");

	str = sASM_XZDB.strd2Date20011001HC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001HC5025");

	str = sASM_XZDB.strd2Date20011001CO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001CO5025");

	str = sASM_XZDB.strd2Date20011001NO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001NO5025");

	str = sASM_XZDB.strd2Date20011001HC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001HC2540");

	str = sASM_XZDB.strd2Date20011001CO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001CO2540");

	str = sASM_XZDB.strd2Date20011001NO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"d2Date20011001NO2540");

	str = sASM_XZDB.strzxDate20040901qHC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901qHC5025");

	str = sASM_XZDB.strzxDate20040901qCO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901qCO5025");

	str = sASM_XZDB.strzxDate20040901qNO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901qNO5025");

	str = sASM_XZDB.strzxDate20040901qHC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901qHC2540");

	str = sASM_XZDB.strzxDate20040901qCO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901qCO2540");

	str = sASM_XZDB.strzxDate20040901qNO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901qNO2540");

	str = sASM_XZDB.strzxDate20040901HC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901HC5025");

	str = sASM_XZDB.strzxDate20040901CO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901CO5025");

	str = sASM_XZDB.strzxDate20040901NO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901NO5025");

	str = sASM_XZDB.strzxDate20040901HC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901HC2540");

	str = sASM_XZDB.strzxDate20040901CO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901CO2540");

	str = sASM_XZDB.strzxDate20040901NO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"zxDate20040901NO2540");

	str = sASM_XZDB.strBZSM;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"BZSM");

	return 0x00;
}

DWORD CASM_XZDBService::StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_XZDB &sASM_XZDB)
{
	std::wstring str;
	_variant_t v;

	str = sASM_XZDB.strID;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"ID");
	}
	str = sASM_XZDB.strd1Date20000701qHC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701qHC5025");
	}
	str = sASM_XZDB.strd1Date20000701qCO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701qCO5025");
	}
	str = sASM_XZDB.strd1Date20000701qNO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701qNO5025");
	}
	str = sASM_XZDB.strd1Date20000701qHC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701qHC2540");
	}
	str = sASM_XZDB.strd1Date20000701qCO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701qCO2540");
	}
	str = sASM_XZDB.strd1Date20000701qNO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701qNO2540");
	}
	str = sASM_XZDB.strd1Date20000701HC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701HC5025");
	}
	str = sASM_XZDB.strd1Date20000701CO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701CO5025");
	}
	str = sASM_XZDB.strd1Date20000701NO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701NO5025");
	}
	str = sASM_XZDB.strd1Date20000701HC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701HC2540");
	}
	str = sASM_XZDB.strd1Date20000701CO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701CO2540");
	}
	str = sASM_XZDB.strd1Date20000701NO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d1Date20000701NO2540");
	}
	str = sASM_XZDB.strd2Date20011001qHC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001qHC5025");
	}
	str = sASM_XZDB.strd2Date20011001qCO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001qCO5025");
	}
	str = sASM_XZDB.strd2Date20011001qNO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001qNO5025");
	}
	str = sASM_XZDB.strd2Date20011001qHC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001qHC2540");
	}
	str = sASM_XZDB.strd2Date20011001qCO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001qCO2540");
	}
	str = sASM_XZDB.strd2Date20011001qNO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001qNO2540");
	}
	str = sASM_XZDB.strd2Date20011001HC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001HC5025");
	}
	str = sASM_XZDB.strd2Date20011001CO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001CO5025");
	}
	str = sASM_XZDB.strd2Date20011001NO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001NO5025");
	}
	str = sASM_XZDB.strd2Date20011001HC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001HC2540");
	}
	str = sASM_XZDB.strd2Date20011001CO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001CO2540");
	}
	str = sASM_XZDB.strd2Date20011001NO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"d2Date20011001NO2540");
	}
	str = sASM_XZDB.strzxDate20040901qHC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901qHC5025");
	}
	str = sASM_XZDB.strzxDate20040901qCO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901qCO5025");
	}
	str = sASM_XZDB.strzxDate20040901qNO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901qNO5025");
	}
	str = sASM_XZDB.strzxDate20040901qHC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901qHC2540");
	}
	str = sASM_XZDB.strzxDate20040901qCO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901qCO2540");
	}
	str = sASM_XZDB.strzxDate20040901qNO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901qNO2540");
	}
	str = sASM_XZDB.strzxDate20040901HC5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901HC5025");
	}
	str = sASM_XZDB.strzxDate20040901CO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901CO5025");
	}
	str = sASM_XZDB.strzxDate20040901NO5025;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901NO5025");
	}
	str = sASM_XZDB.strzxDate20040901HC2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901HC2540");
	}
	str = sASM_XZDB.strzxDate20040901CO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901CO2540");
	}
	str = sASM_XZDB.strzxDate20040901NO2540;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"zxDate20040901NO2540");
	}
	str = sASM_XZDB.strBZSM;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"BZSM");
	}

	return 0x00;
}