#include "StdAfx.h"
#include "SZIntLib_New_API.h"

#include "gsoap\soapIVehicleServiceServiceSoapBindingProxy.h"
#include "gsoap\IVehicleServiceServiceSoapBinding.nsmap"

CSZIntLib_New_API::CSZIntLib_New_API(void)
{
}

CSZIntLib_New_API::~CSZIntLib_New_API(void)
{
}

std::wstring CSZIntLib_New_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"JSCXZN%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CSZIntLib_New_API::LoginServer(const char * pchURL, const std::wstring& strunitid, const std::wstring& strlineid, 
		const std::wstring& strusername, const std::wstring& pwd, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strunitid(strunitid);
	std::wstring m_strlineid(strlineid);
	std::wstring m_strusername(strusername);
	std::wstring m_pwd(pwd);

	ns1__LoginServer send;
	ns1__LoginServerResponse sRes;

	send.unitid = &m_strunitid;
	send.lineid = &m_strlineid;
	send.username = &m_strusername;
	send.pwd = &m_pwd;

	CStringW strLog;
	strLog.Format(L"机构编号unitid:%s, 检测线编号lineid:%s, 用户名username:%s, 密码pwd:%s", send.unitid->c_str(), 
		send.lineid->c_str(), send.username->c_str(), send.pwd->c_str());
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.LoginServer(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CSZIntLib_New_API::GetBaseTypeInfo(const char * pchURL, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__GetBaseTypeInfo send;
	ns1__GetBaseTypeInfoResponse sRes;

	CStringW strLog;
	strLog.Format(L"获取字典");
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetBaseTypeInfo(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CSZIntLib_New_API::Vehiclelogin(const char * pchURL, const std::wstring& strXmlDoc, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__vehiclelogin send;
	ns1__vehicleloginResponse sRes;

	std::wstring m_strXmlDoc(strXmlDoc);

	send.vehiclexml = &m_strXmlDoc;

	CStringW strLog;
	strLog.Format(L"车辆检测登录vehiclexml:%s", send.vehiclexml->c_str());
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.vehiclelogin(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CSZIntLib_New_API::GetCheckList(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__GetCheckList send;
	ns1__GetCheckListResponse sRes;

	std::wstring m_strtoken(strtoken);
	std::wstring m_strunitid(strunitid);

	send.token = &m_strtoken;
	send.unitid = &m_strunitid;

	CStringW strLog;
	strLog.Format(L"获取待检测列表token:%s, unitid:%s", send.token->c_str(), send.unitid->c_str());
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetCheckList(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CSZIntLib_New_API::getVehicle(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid, const std::wstring& strVIN, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__GetVehicle send;
	ns1__GetVehicleResponse sRes;

	std::wstring m_strtoken(strtoken);
	std::wstring m_strunitid(strunitid);
	std::wstring m_strVIN(strVIN);

	send.token = &m_strtoken;
	send.unitid = &m_strunitid;
	send.vin = &m_strVIN;

	CStringW strLog;
	strLog.Format(L"获取对应的车辆信息token:%s, unitid:%s, vin:%s", send.token->c_str(), send.unitid->c_str(), send.vin->c_str());
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetVehicle(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CSZIntLib_New_API::BeginCheck(const char * pchURL, const std::wstring& strunitid, const std::wstring& strlineid,
		const std::wstring& strVIN, const std::wstring& strPlate, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__BeginCheck send;
	ns1__BeginCheckResponse sRes;

	std::wstring m_strunitid(strunitid);
	std::wstring m_strlineid(strlineid);
	std::wstring m_strVIN(strVIN);
	std::wstring m_strPlate(strPlate);

	send.unitid = &m_strunitid;
	send.lineid = &m_strlineid;
	send.vin = &m_strVIN;
	send.plate = &m_strPlate;

	CStringW strLog;
	strLog.Format(L"开始上线检测unitid:%s, lineid:%s, vin:%s, plate:%s", 
		send.unitid->c_str(), send.lineid->c_str(), send.vin->c_str(), send.plate->c_str());
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.BeginCheck(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}

int CSZIntLib_New_API::UploadInspectionResult(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid,
		const std::wstring& strcheckresult, const int& ncheckmethod, std::wstring& strRetStr)
{
	int nRet(999);
	IVehicleServiceServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__UploadInspectionResult send;
	ns1__UploadInspectionResultResponse sRes;

	std::wstring m_strtoken(strtoken);
	std::wstring m_strunitid(strunitid);
	std::wstring m_strcheckresult(strcheckresult);

	send.token = &m_strtoken;
	send.unitid = &m_strunitid;
	send.checkresult = &m_strcheckresult;
	send.checkmethod = ncheckmethod;

	CStringW strLog;
	strLog.Format(L"上传检测结果token:%s, unitid:%s, checkresult:%s, checkmethod:%d", 
		send.token->c_str(), send.unitid->c_str(), send.checkresult->c_str(), send.checkmethod);
	CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.UploadInspectionResult(&send, sRes);

	if (nRet == SOAP_OK)
	{
		strRetStr = sRes.return_->c_str();

		CNHLogAPI::WriteLogEx(CSZIntLib_New_API::LogFilePath().c_str(), L"接收", L"", strRetStr.c_str());
	}

	return nRet;
}