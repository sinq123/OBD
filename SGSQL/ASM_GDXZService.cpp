#include "StdAfx.h"
#include "ASM_GDXZService.h"
#include <cassert>


CASM_GDXZService::CASM_GDXZService(void)
{
}


CASM_GDXZService::~CASM_GDXZService(void)
{
}

DWORD CASM_GDXZService::GetASM_GDXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_GDXZ &sASM_GDXZ)
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
			FieldToStruct(pRecordset, sASM_GDXZ);

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

DWORD CASM_GDXZService::GetASM_GDXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_GDXZ> &lsASM_GDXZ)
{ 
	assert(NULL != pConnection);
	assert(NULL != pwchSql);

	lsASM_GDXZ.clear();
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
			SASM_GDXZ sASM_GDXZ;
			while (!pRecordset->adoEOF)
			{
				FieldToStruct(pRecordset, sASM_GDXZ);
				lsASM_GDXZ.push_back(sASM_GDXZ);
				pRecordset->MoveNext();
			}
			dwReturn = static_cast<DWORD>(lsASM_GDXZ.size());
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

DWORD CASM_GDXZService::SetASM_GDXZData(const _ConnectionPtr &pConnection, const SASM_GDXZ &sASM_GDXZ)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	_snwprintf_s(wchSql,_countof(wchSql) ,_TRUNCATE, L"SELECT * FROM ASM_GDXZ WHERE ID = '%s'", sASM_GDXZ.strID.c_str());
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
			StructToField(pRecordset, sASM_GDXZ);
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

DWORD CASM_GDXZService::SetASM_GDXZData(const _ConnectionPtr &pConnection, const std::list<SASM_GDXZ> &lsASM_GDXZ)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	wcsncpy_s(wchSql, _countof(wchSql), L"SELECT * FROM ASM_GDXZ", _TRUNCATE);
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
			std::list<SASM_GDXZ>::const_iterator iter = lsASM_GDXZ.begin();
			for ( ; iter != lsASM_GDXZ.end(); iter++)
			{
				pRecordset->AddNew();
				StructToField(pRecordset, *iter);
			}
			pRecordset->Update();
			dwReturn = static_cast<DWORD>(lsASM_GDXZ.size());
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

DWORD CASM_GDXZService::SetASM_GDXZData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_GDXZ &sASM_GDXZ)
{
	assert(NULL != pConnection);

	DWORD dwReturn(0x00);
	wchar_t wchSql[1024] = {L'\0'};
	ZeroMemory(wchSql, sizeof(wchSql));
	_snwprintf_s(wchSql,_countof(wchSql) ,_TRUNCATE, L"SELECT * FROM ASM_GDXZ WHERE ID = '%s'", sASM_GDXZ.strID.c_str());
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
			StructToField_NonEmpty(pRecordset, sASM_GDXZ);
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

DWORD CASM_GDXZService::FieldToStruct(const _RecordsetPtr &pRecordset, SASM_GDXZ &sASM_GDXZ)
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
	sASM_GDXZ.strID = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CO25");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_GDXZ.strCO25 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"HC25");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_GDXZ.strHC25 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"NO25");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_GDXZ.strNO25 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"CO40");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_GDXZ.strCO40 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"HC40");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_GDXZ.strHC40 = wchBuf;

	v.ChangeType(VT_NULL);
	ZeroMemory(wchBuf, sizeof(wchBuf));
	GetFieldValue(pRecordset, v, L"NO40");
	if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	{
		_snwprintf_s(wchBuf, _countof(wchBuf), _TRUNCATE, L"%s", (wchar_t*)(_bstr_t)v);
	}
	else
	{
		wcsncpy_s(wchBuf, _countof(wchBuf), L"", _TRUNCATE);
	}
	sASM_GDXZ.strNO40 = wchBuf;

	return 0x00;
}

DWORD CASM_GDXZService::StructToField(const _RecordsetPtr &pRecordset, const SASM_GDXZ &sASM_GDXZ)
{
	std::wstring str;
	_variant_t v;

	str = sASM_GDXZ.strID;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"ID");

	str = sASM_GDXZ.strCO25;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CO25");

	str = sASM_GDXZ.strHC25;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"HC25");

	str = sASM_GDXZ.strNO25;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"NO25");

	str = sASM_GDXZ.strCO40;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"CO40");

	str = sASM_GDXZ.strHC40;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"HC40");

	str = sASM_GDXZ.strNO40;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();
	}
	else
	{
		v.ChangeType(VT_EMPTY);
	}
	PutFieldValue(pRecordset, v, L"NO40");

	return 0x00;
}

DWORD CASM_GDXZService::StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_GDXZ &sASM_GDXZ)
{
	std::wstring str;
	_variant_t v;

	str = sASM_GDXZ.strID;
	if (L"" != str)
	{
		v = static_cast<int>(_wtoi(str.c_str()));

		PutFieldValue(pRecordset, v, L"ID");
	}
	str = sASM_GDXZ.strCO25;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CO25");
	}
	str = sASM_GDXZ.strHC25;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"HC25");
	}
	str = sASM_GDXZ.strNO25;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"NO25");
	}
	str = sASM_GDXZ.strCO40;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"CO40");
	}
	str = sASM_GDXZ.strHC40;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"HC40");
	}
	str = sASM_GDXZ.strNO40;
	if (L"" != str)
	{
		v = (_variant_t)str.c_str();

		PutFieldValue(pRecordset, v, L"NO40");
	}
	return 0x00;
}