#include "StdAfx.h"
#include "HYInterfaceLib_API.h"

#include "gsoap\soapExternalAccessSoapProxy.h"
#include "gsoap\ExternalAccessSoap.nsmap"

std::wstring CHYInterfaceLib_API::m_strLogFilePath = L"";

CHYInterfaceLib_API::CHYInterfaceLib_API(void)
{
}

CHYInterfaceLib_API::~CHYInterfaceLib_API(void)
{
}

std::wstring CHYInterfaceLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"衡阳OBD%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

DWORD CHYInterfaceLib_API::SetLogFilePath(const std::wstring& strLogFilePath)
{
	if (strLogFilePath.empty())
	{
		return 0x01;
	}
	else
	{
		m_strLogFilePath = strLogFilePath;
	}
	return 0x00;
}

int CHYInterfaceLib_API::ObjectOut(const char * pchURL, const std::wstring& strkey, const std::wstring& strjkid,
	const std::wstring& strxmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	ExternalAccessSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strkey= strkey;
	std::wstring m_strjkid= strjkid;
	std::wstring m_strxmlDoc= strxmlDoc;

	_ns1__wsInvokeInterface send;
	_ns1__wsInvokeInterfaceResponse sres;

	send.KEY = &m_strkey;
	send.JKID = &m_strjkid;
	send.XML_ = &m_strxmlDoc;

	if (m_strLogFilePath.empty())
	{
		CNHLogAPI::WriteLogEx(CHYInterfaceLib_API::LogFilePath().c_str(), L"发送", send.JKID->c_str(), send.XML_->c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", send.JKID->c_str(), send.XML_->c_str());
	}


	nRet = toasb.wsInvokeInterface(&send, sres);

	if (nRet == SOAP_OK)
	{
		strRetStr = sres.wsInvokeInterfaceResult->c_str();

		if (m_strLogFilePath.empty())
		{
			CNHLogAPI::WriteLogEx(CHYInterfaceLib_API::LogFilePath().c_str(), L"返回", send.JKID->c_str(), strRetStr.c_str());
		}
		else
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"返回", send.JKID->c_str(), strRetStr.c_str());
		}
	}

	return nRet;
}
