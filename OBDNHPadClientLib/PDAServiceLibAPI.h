/*
* Copyright (c) 2009,佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：PDAServiceLibAPI.h
* 文件标识：
* 摘要：WebService联网接口封装
*
* 版本：1.0.0
* 作者：Raylee
* 日期：2019-03-05
*
* 历史信息：
*
*
* 版本：1.0.0
* 日期：2019-03-05
* 作者：Raylee
* 描述：开发ing
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
	// 设置WebService地址
	inline void SetUrl(LPCTSTR szUrl) {	m_strURL = CStringA(szUrl);	}

	inline LPCTSTR GetRunningNumber(void) {	return m_strRunningNumber.c_str();	}
	inline void SetRunningNumber(LPCTSTR szRunningNumber) {	m_strRunningNumber = szRunningNumber;	}
	
public:
	bool DownloadOBDDetList(LPCTSTR szPlateNumber, LPCTSTR szPlateType, std::vector<SDetInfo> &vtDetInfo, std::wstring &strCode, std::wstring &strMsg);
	bool UploadOBDDetData(LPCTSTR szRunningNumber, SResultOfOBD &sResultOfOBD, std::wstring &strCode, std::wstring &strMsg);
	bool UploadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg);
	bool DownloadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg);

private:
	// WebService地址
	std::string m_strURL;
	// 处理当前流水号
	std::wstring m_strRunningNumber;
};

