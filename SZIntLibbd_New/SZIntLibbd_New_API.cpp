#include "StdAfx.h"
#include "SZIntLibbd_New_API.h"

#include "gsoap\soapIBdServiceServiceSoapBindingProxy.h"
#include "gsoap\IBdServiceServiceSoapBinding.nsmap"

CSZIntLibbd_New_API::CSZIntLibbd_New_API(void)
{
}

CSZIntLibbd_New_API::~CSZIntLibbd_New_API(void)
{
}

std::wstring CSZIntLibbd_New_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"JSCXZNBD%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CSZIntLibbd_New_API::UploadInspectionResult(const char * pchURL, const std::wstring& strcheckresult, const int& ncheckmethod, std::wstring& strRetStr)
{
	int nRet(999);
	IBdServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__UploadBDResult send;
	ns1__UploadBDResultResponse sRes;

	std::wstring m_strcheckresult(strcheckresult);

	send.checkresult = &m_strcheckresult;
	send.action = ncheckmethod;

	CStringW strLog;
	strLog.Format(L"上传标定checkresult:%s, action:%d", send.checkresult->c_str(), send.action);
	CNHLogAPI::WriteLogEx(CSZIntLibbd_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.UploadBDResult(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLibbd_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}
