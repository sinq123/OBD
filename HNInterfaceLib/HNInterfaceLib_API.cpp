#include "StdAfx.h"
#include "HNInterfaceLib_API.h"

#include "soapDataInterfaceSoapProxy.h"
#include "DataInterfaceSoap.nsmap"

CHNInterfaceLib_API::CHNInterfaceLib_API(void)
{
}

CHNInterfaceLib_API::~CHNInterfaceLib_API(void)
{
}

std::wstring CHNInterfaceLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"HN%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CHNInterfaceLib_API:: ObjectOut(const char * pchURL, const std::wstring strmethodName, const std::wstring& strjsonParam, const std::wstring& strresultData, 
		const std::wstring& struserName, const std::wstring& strpassWord, std::wstring& strRetStr)
{
	int nRet(999);
	DataInterfaceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__Common sendCom;
	_ns1__CommonResponse Res;

	std::wstring m_strmethodName = strmethodName;
	std::wstring m_strjsonParam = strjsonParam;
	std::wstring m_strresultData = strresultData;
	std::wstring m_struserName = struserName;
	std::wstring m_strpassWord = strpassWord;

	sendCom.methodName = &m_strmethodName;
	sendCom.jsonParam = &m_strjsonParam;
	sendCom.ResultData = &m_strresultData;
	sendCom.userName = &m_struserName;
	sendCom.passWord = &m_strpassWord;

	CStringW strLog;
	strLog.Format(L"sendCom.jsonParam:%s\r\nsendCom.ResultData:%s", sendCom.jsonParam->c_str(), sendCom.ResultData->c_str());
	CNHLogAPI::WriteLogEx(CHNInterfaceLib_API::LogFilePath().c_str(), L"发送", sendCom.methodName->c_str(), strLog);

	nRet = toasb.Common(&sendCom, Res);

	if (nRet == SOAP_OK)
	{
		strRetStr = Res.CommonResult->c_str();
		CNHLogAPI::WriteLogEx(CHNInterfaceLib_API::LogFilePath().c_str(), L"接受", sendCom.methodName->c_str(), strRetStr.c_str());
	}

	return nRet;
}
