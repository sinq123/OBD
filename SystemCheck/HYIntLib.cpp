#include "StdAfx.h"
#include "HYIntLib.h"
#include "SystemCheck.h"

//char* CHYIntLib_API::m_pchURL = NULL;
//CStringW CHYIntLib_API::m_strkey = L"";
//CStringW CHYIntLib_API::m_StationNum = L"";
//CStringW CHYIntLib_API::m_LineNum = L"";

CHYIntLib_API::CHYIntLib_API(void)
	:m_pchURL(NULL)
{
	GetConfig();
	GetLogFilePath();
}


CHYIntLib_API::~CHYIntLib_API(void)
{
	if (NULL != m_pchURL)
	{
		free(m_pchURL);
		m_pchURL = NULL;
	}
}

void CHYIntLib_API::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"WebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"NetSet", L"URL", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!strTemp.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(strTemp);
		}
		m_StationNum = si.GetString(L"NetSet", L"StationNum", L"");
		m_LineNum = si.GetString(L"NetSet", L"LineNum", L"");
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		m_strkey = si.GetString(L"UserInfo", L"NetKey", L"");
	}
}

void CHYIntLib_API::GetLogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"衡阳SystemCheck%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strLogFilePath = strLogFilePath;
}


int CHYIntLib_API::ConstLoad_13W01(std::wstring& strRetStr)
{
	int nRet;
	CString strOBDData;

	// 读取DemarcationResultOfDynConstLoad.ini
	DEMARCATIONRESULTOFDYNCONSTLOAD sResultOfDynConstLoad;
	ZeroMemory(&sResultOfDynConstLoad, sizeof(DEMARCATIONRESULTOFDYNCONSTLOAD));
	GetIniResultOfDynConstLoad(&sResultOfDynConstLoad);




	nRet = CHYInterfaceLib_API::ObjectOut(m_pchURL, m_strkey.GetString(), L"13W01", strOBDData.GetString(), strRetStr);
	return nRet;
}

int CHYIntLib_API::PLHP_13W02(std::wstring& strRetStr)
{
	int nRet;


	
	return nRet;
}

int CHYIntLib_API::ConstLoadHeavy_13W03(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::PLHPHeavy_13W04(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::GasCheck_13W05(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::GasCheck_13W06(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::GasCheck_13W07(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::GasCheck_13W08(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::LeakageCheck_13W09(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::YDCalChk_13W010(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::EqulChk_13W011(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::EqulSelfIns_13W012(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}

int CHYIntLib_API::EqulCal_13W014(std::wstring& strRetStr)
{
	int nRet;
	return nRet;
}