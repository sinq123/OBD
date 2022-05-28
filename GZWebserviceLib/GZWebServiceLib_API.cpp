#include "StdAfx.h"
#include "GZWebServiceLib_API.h"

#include "gsoap\soapDeviceSwapIfaceImplServiceSoapBindingProxy.h"
#include "gsoap\DeviceSwapIfaceImplServiceSoapBinding.nsmap"

std::wstring CGZWebServic_API::m_strLogFilePath = L"";

CGZWebServic_API::CGZWebServic_API(void)
{
}

CGZWebServic_API::~CGZWebServic_API(void)
{
}

std::wstring CGZWebServic_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"GZSCheck%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CGZWebServic_API::cgjLSPSelfcheck(const char * pchURL, const bool& bDieselOil, const std::wstring& strAvitualTime, const std::wstring& strArealTime,
		const std::wstring& strBvitualTime, const std::wstring& strBrealTime, const std::wstring& strCvitualTime, const std::wstring& strCrealTime,
		const std::wstring& strDvitualTime, const std::wstring& strDrealTime, const  std::wstring& strregistCode, const std::wstring& strALSpower, 
		const std::wstring& strBLSpower, const std::wstring& strCLSpower, const std::wstring& strDLSpower, const std::wstring& strcheckResult, 
		const std::wstring& strcheckTimeStart, const std::wstring& strcheckTimeEnd, const std::wstring& strremark, std::wstring& strRetStr)
{
	int nRet(99);

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strregistCode = strregistCode;
	std::wstring m_strcheckResult = strcheckResult;
	std::wstring m_strcheckTimeStart = strcheckTimeStart;
	std::wstring m_strcheckTimeEnd = strcheckTimeEnd;
	std::wstring m_strremark = strremark;

	ns1__cgjLSPSelfcheck send;
	ns1__cgjLSPSelfcheckResponse sres;

	std::wstring strF;

	send.registCode = &m_strregistCode;
	if (bDieselOil)
	{
		send.AvitualTime = (float)_wtof(strAvitualTime.c_str());
		send.ArealTime = (float)_wtof(strArealTime.c_str());
		send.BvitualTime = (float)_wtof(strBvitualTime.c_str());
		send.BrealTime = (float)_wtof(strBrealTime.c_str());
		send.ALSpower = (float)_wtof(strALSpower.c_str());
		send.BLSpower = (float)_wtof(strBLSpower.c_str());
	}
	send.CvitualTime = (float)_wtof(strCvitualTime.c_str());
	send.CrealTime = (float)_wtof(strCrealTime.c_str());
	send.DvitualTime = (float)_wtof(strDvitualTime.c_str());
	send.DrealTime = (float)_wtof(strDrealTime.c_str());
	send.CLSpower = (float)_wtof(strCLSpower.c_str());
	send.DLSpower = (float)_wtof(strDLSpower.c_str());
	send.checkResult = &m_strcheckResult;
	send.checkTimeStart = &m_strcheckTimeStart;
	send.checkTimeEnd = &m_strcheckTimeEnd;
	send.remark = &m_strremark;


	nRet = toasb.cgjLSPSelfcheck(&send, sres);

	if (nRet == SOAP_OK)
	{
		strRetStr = L"上传成功";
		if (!sres.return_)
		{
			strRetStr = L"上传失败";
		}
		
	}
	else
	{
		strRetStr.append(L"失败%d", nRet);
	}

	return nRet;
}



int CGZWebServic_API::cgjSelfcheck(const char * pchURL, const std::wstring& strregistCode, const std::wstring strHVitualTime, const std::wstring strHrealTime,
		const std::wstring& strLvitualTime, const std::wstring strLrealTime, const std::wstring strHpower, const std::wstring strLpower,
		const std::wstring& strcheckResult, const std::wstring strcheckTimeStart, const std::wstring strcheckTimeEnd, const std::wstring strremark,
		std::wstring& strRetStr)
{
	int nRet(99);

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strregistCode = strregistCode;
	std::wstring m_strcheckResult = strcheckResult;
	std::wstring m_strcheckTimeStart = strcheckTimeStart;
	std::wstring m_strcheckTimeEnd = strcheckTimeEnd;
	std::wstring m_strremark = strremark;

	ns1__cgjSelfcheck send;
	ns1__cgjSelfcheckResponse sres;

	send.registCode = &m_strregistCode;

	send.HVitualTime = (float)_wtof(strHVitualTime.c_str());
	send.HrealTime = (float)_wtof(strHrealTime.c_str());
	send.LvitualTime = (float)_wtof(strLvitualTime.c_str());
	send.LrealTime = (float)_wtof(strLrealTime.c_str());
	send.Hpower = (float)_wtof(strHpower.c_str());
	send.Lpower = (float)_wtof(strLpower.c_str());

	send.checkResult = &m_strcheckResult;
	send.checkTimeStart = &m_strcheckTimeStart;
	send.checkTimeEnd = &m_strcheckTimeEnd;
	send.remark = &m_strremark;


	nRet = toasb.cgjSelfcheck(&send, sres);

	if (nRet == SOAP_OK)
	{
		strRetStr = L"上传成功";
		if (!sres.return_)
		{
			strRetStr = L"上传失败";
		}
		
	}
	else
	{
		strRetStr.append(L"失败%d", nRet);
	}

	return nRet;
}

int CGZWebServic_API::cgjLASelfcheck(const char * pchURL, const std::wstring& strregistCode, const std::wstring strAvitualTime, const std::wstring strArealTime,
		const std::wstring& strBvitualTime, const std::wstring strBrealTime, const std::wstring strCvitualTime, const std::wstring strCrealTime,
		const std::wstring& strDvitualTime, const std::wstring strDrealTime, const std::wstring strcheckResult, const std::wstring strcheckTimeStart,
		const std::wstring strcheckTimeEnd, const std::wstring strremark, std::wstring& strRetStr)
{
	int nRet(99);

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strregistCode = strregistCode;
	std::wstring m_strcheckResult = strcheckResult;
	std::wstring m_strcheckTimeStart = strcheckTimeStart;
	std::wstring m_strcheckTimeEnd = strcheckTimeEnd;
	std::wstring m_strremark = strremark;

	ns1__cgjLASelfcheck send;
	ns1__cgjLASelfcheckResponse sres;

	send.registCode = &m_strregistCode;

	send.AvitualTime = (float)_wtof(strAvitualTime.c_str());
	send.ArealTime = (float)_wtof(strArealTime.c_str());
	send.BvitualTime = (float)_wtof(strBvitualTime.c_str());
	send.BrealTime = (float)_wtof(strBrealTime.c_str());
	send.CvitualTime = (float)_wtof(strCvitualTime.c_str());
	send.CrealTime = (float)_wtof(strCrealTime.c_str());
	send.DvitualTime = (float)_wtof(strDvitualTime.c_str());
	send.DrealTime = (float)_wtof(strDrealTime.c_str());

	send.checkResult = &m_strcheckResult;
	send.checkTimeStart = &m_strcheckTimeStart;
	send.checkTimeEnd = &m_strcheckTimeEnd;
	send.remark = &m_strremark;


	nRet = toasb.cgjLASelfcheck(&send, sres);

	if (nRet == SOAP_OK)
	{
		strRetStr = L"上传成功";
		if (!sres.return_)
		{
			strRetStr = L"上传失败";
		}
		
	}
	else
	{
		strRetStr.append(L"失败%d", nRet);
	}

	return nRet;
}

int CGZWebServic_API::getSatate(const char * pchURL, const std::wstring& strregistCode, std::wstring& strRetStr)
{
	int nRet(99);

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	std::wstring m_strregistCode = strregistCode;


	ns1__getSatate send;
	ns1__getSatateResponse sres;

	send.registCode = &m_strregistCode;
	//CNHLogAPI::WriteLogEx(CGZWebServic_API::LogFilePath().c_str(), L"发送", L"getSatate", send.registCode->c_str());

	nRet = toasb.getSatate(&send, sres);

	if (nRet == SOAP_OK)
	{

		CString str;

		str.Format(L"%s,%s,%s", sres.return_->businessId->c_str(), sres.return_->methodId->c_str(),sres.return_->state->c_str());
		strRetStr = str.GetString();
		
	}
	else
	{
		CString str;
		str.Format(L"失败%d", nRet);
		strRetStr = str.GetString();
	}
	//CNHLogAPI::WriteLogEx(CGZWebServic_API::LogFilePath().c_str(), L"返回", L"getSatate",strRetStr.c_str());
	return nRet;
}

DWORD CGZWebServic_API::SetLogFilePath(const std::wstring& strLogFilePath)
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


