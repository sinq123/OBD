#include "StdAfx.h"
#include "ZKYTInterfaceLib_API.h"

#include "gsoap\soapDeviceSwapIfaceImplServiceSoapBindingProxy.h"
#include "gsoap\soap.nsmap"

CZKYTInterfaceLib_API::CZKYTInterfaceLib_API(void)
{
}

CZKYTInterfaceLib_API::~CZKYTInterfaceLib_API(void)
{
}

std::wstring CZKYTInterfaceLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"ZKYTWH%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CZKYTInterfaceLib_API::getSqm(const char * pchURL, const std::wstring& strjcxbh, std::wstring& strRetStr)
{
	int nRet(999);
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strjcxbh(strjcxbh);

	ns1__getSqm send;
	ns1__getSqmResponse sRes;

	send.jcxbh = &m_strjcxbh;

	CStringW strLog;
	strLog.Format(L"jcxbh:%s", send.jcxbh->c_str());
	CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.getSqm(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}


int CZKYTInterfaceLib_API::jdcjk(const char * pchURL, const std::wstring& strsqm, const std::wstring& strjklx, const std::wstring& strjhsj, std::wstring& strRetStr)
{
	int nRet(999);
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strsqm(strsqm);
	std::wstring m_strjklx(strjklx);
	std::wstring m_strjhsj(strjhsj);
	ns1__jdcjk send;
	ns1__jdcjkResponse sRes;

	send.sqm = &m_strsqm;
	send.jklx = &m_strjklx;
	send.jhsj = &m_strjhsj;

	CStringW strLog;
	strLog.Format(L"sqm:%s, jklx:%s, jhsj:%s", send.sqm->c_str(), send.jklx->c_str(), send.jhsj->c_str());
	CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.jdcjk(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}