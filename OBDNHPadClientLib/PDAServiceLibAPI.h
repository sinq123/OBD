/*
* Copyright (c) 2009,��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PDAServiceLibAPI.h
* �ļ���ʶ��
* ժҪ��WebService�����ӿڷ�װ
*
* �汾��1.0.0
* ���ߣ�Raylee
* ���ڣ�2019-03-05
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.0
* ���ڣ�2019-03-05
* ���ߣ�Raylee
* ����������ing
*/
#pragma once

#include <memory>
#include <string>
#include <map>
#include <windows.h>

#include "../NHDetCommModu/NHDetCommModu.h"

struct SDetInfo
{
	std::wstring strRunningNumber;
	std::wstring strPlateNumber;
	std::wstring strPlateType;
	std::wstring strFuelType;
	std::wstring strItemApp;
	std::wstring strItemOBD;
	std::wstring strItemEmi;
};

class AFX_EXT_CLASS PDAServiceLibAPI
{
public:
	static PDAServiceLibAPI& GetInstance(void);

private:
	PDAServiceLibAPI(void);
	~PDAServiceLibAPI(void);
	PDAServiceLibAPI(const PDAServiceLibAPI &other);
	PDAServiceLibAPI& operator=(const PDAServiceLibAPI &other);
	friend class std::auto_ptr<PDAServiceLibAPI>;

public:
	// ����WebService��ַ
	inline void SetUrl(LPCTSTR szUrl) {	m_strURL = CStringA(szUrl);	}

	inline LPCTSTR GetRunningNumber(void) {	return m_strRunningNumber.c_str();	}
	inline void SetRunningNumber(LPCTSTR szRunningNumber) {	m_strRunningNumber = szRunningNumber;	}
	
public:
	bool DownloadOBDDetList(LPCTSTR szPlateNumber, LPCTSTR szPlateType, std::vector<SDetInfo> &vtDetInfo, std::wstring &strCode, std::wstring &strMsg);
	bool UploadOBDDetData(LPCTSTR szRunningNumber, SResultOfOBD &sResultOfOBD, std::wstring &strCode, std::wstring &strMsg);
	bool UploadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg);
	bool DownloadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg);

private:
	// WebService��ַ
	std::string m_strURL;
	// ����ǰ��ˮ��
	std::wstring m_strRunningNumber;
};

