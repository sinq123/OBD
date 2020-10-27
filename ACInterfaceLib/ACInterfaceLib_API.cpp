#include "StdAfx.h"
#include "ACInterfaceLib_API.h"

#include "gsoap\soapVeptsOutAccessSoapProxy.h"
#include "gsoap\VeptsOutAccessSoap.nsmap"

CACInterfaceLib_API::CACInterfaceLib_API(void)
{
}

CACInterfaceLib_API::~CACInterfaceLib_API(void)
{
}

std::wstring CACInterfaceLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"ACOBD%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CACInterfaceLib_API::QueryObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	VeptsOutAccessSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strxmlDoc = strxmlDoc;
	_ns1__query query;
	_ns1__queryResponse response;
	query.xmlDoc = &m_strxmlDoc;

	nRet = toasb.query(&query, &response);

	CStringW strLog;
	strLog.Format(L"xmlDoc:%s", query.xmlDoc->c_str());
	CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	if (nRet == SOAP_OK)
	{
		strRetStr = response.queryResult->c_str();

		CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CACInterfaceLib_API::QueryObjectOut_(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	VeptsOutAccessSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strxmlDoc = strxmlDoc;
	_ns1__query query;
	_ns1__queryResponse response;
	query.xmlDoc = &m_strxmlDoc;

	nRet = toasb.query_(&query, &response);

	CStringW strLog;
	strLog.Format(L"xmlDoc:%s", query.xmlDoc->c_str());
	CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	if (nRet == SOAP_OK)
	{
		strRetStr = response.queryResult->c_str();
		CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CACInterfaceLib_API::WriteObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	VeptsOutAccessSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__write write;
	_ns1__writeResponse response;
	std::wstring m_strxmlDoc= strxmlDoc;
	write.xmlDoc = &m_strxmlDoc;

	nRet = toasb.write(&write, &response);

	CStringW strLog;
	strLog.Format(L"xmlDoc:%s", write.xmlDoc->c_str());
	CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	if (nRet == SOAP_OK)
	{
		strRetStr = response.writeResult->c_str();
		CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CACInterfaceLib_API::WriteObjectOut_(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	VeptsOutAccessSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__write write;
	_ns1__writeResponse response;
	std::wstring m_strxmlDoc= strxmlDoc;
	write.xmlDoc = &m_strxmlDoc;

	nRet = toasb.write_(&write, &response);

	CStringW strLog;
	strLog.Format(L"xmlDoc:%s", write.xmlDoc->c_str());
	CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	if (nRet == SOAP_OK)
	{
		strRetStr = response.writeResult->c_str();
		CNHLogAPI::WriteLogEx(CACInterfaceLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}