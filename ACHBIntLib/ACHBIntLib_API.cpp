#include "StdAfx.h"
#include "ACHBIntLib_API.h"

#include "gsoap\soapWSServiceImplServiceSoapBindingProxy.h"
#include "gsoap\WSServiceImplServiceSoapBinding.nsmap"

CACHBIntLib_API::CACHBIntLib_API(void)
{
}

CACHBIntLib_API::~CACHBIntLib_API(void)
{
}

std::wstring CACHBIntLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"ACHBOBD%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CACHBIntLib_API::QueryObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	WSServiceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strxmlDoc = strxmlDoc;
	ns1__query query;
	ns1__queryResponse response;
	query.xmlDoc = &m_strxmlDoc;

	nRet = toasb.query(&query, response);

	CStringW strLog;
	strLog.Format(L"xmlDoc:%s", query.xmlDoc->c_str());
	CNHLogAPI::WriteLogEx(CACHBIntLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	if (nRet == SOAP_OK)
	{
		strRetStr = response.return_->c_str();

		CNHLogAPI::WriteLogEx(CACHBIntLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CACHBIntLib_API::WriteObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	WSServiceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__write write;
	ns1__writeResponse response;
	std::wstring m_strxmlDoc= strxmlDoc;
	write.xmlDoc = &m_strxmlDoc;

	nRet = toasb.write(&write, response);

	CStringW strLog;
	strLog.Format(L"xmlDoc:%s", write.xmlDoc->c_str());
	CNHLogAPI::WriteLogEx(CACHBIntLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	if (nRet == SOAP_OK)
	{
		strRetStr = response.return_->c_str();
		CNHLogAPI::WriteLogEx(CACHBIntLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}
	else
	{
		CString strLog;
		strLog.Format(L"%d 失败", nRet);
		CNHLogAPI::WriteLogEx(CACHBIntLib_API::LogFilePath().c_str(), L"接收", L"", strLog);
	}
	return nRet;
}
