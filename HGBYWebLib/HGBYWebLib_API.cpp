#include "StdAfx.h"
#include "HGBYWebLib_API.h"

#include "gsoap\soapVESLinkStd2018SoapProxy.h"
#include "gsoap\VESLinkStd2018Soap.nsmap"

std::wstring CHGBYWebLib_API::m_strLogFilePath = L"";

CHGBYWebLib_API::CHGBYWebLib_API(void)
{
	// 默认日志位置
	m_strLogFilePath = LogFilePath().c_str();
}

CHGBYWebLib_API::~CHGBYWebLib_API(void)
{
}

std::wstring CHGBYWebLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"AT艾特%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}


DWORD CHGBYWebLib_API::SetLogFilePath(const std::wstring& strLogFilePath)
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


int CHGBYWebLib_API::RSInfo(const char * pchURL, std::wstring& strRetStr, SHGBYMSG& sHGBYMSG,
		const std::wstring strFunctionID/* = L""*/, const std::wstring strParams/* = L""*/, const std::wstring strKeys/* = L""*/)
{
	int nRet(99);
	VESLinkStd2018SoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__RSInfo send;
	_ns1__RSInfoResponse sret;


	std::wstring m_strFunctionID = strFunctionID;
	std::wstring m_strParams = strParams;
	std::wstring m_strKeys = strKeys;


	send.szFunctionID = &m_strFunctionID;
	send.szParams = &m_strParams;
	send.szKeys = &m_strKeys;


	CString strMsg;
	strMsg.Format(L"FunctionID:%s, Params:%s, Keys:%s", send.szFunctionID->c_str(), send.szParams->c_str(), send.szKeys->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(RSInfo)", strMsg);

	nRet = toasb.RSInfo(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.RSInfoResult->c_str();
		if (strRetStr.find(L"%") != -1)
		{
			strRetStr = DecodeURI(KUTF16ToANSI(strRetStr.c_str()));
		}
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(RSInfo)联网成功", strRetStr.c_str());

		// 预先解析
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRetStr.c_str()))
		{
			std::wstring strCode, strContent, str;

			if (xmlReader.OpenNode(L"response/head/code"))
			{
				xmlReader.GetNodeContent(sHGBYMSG.strcode);
				
			}

			if (xmlReader.OpenNode(L"root/head/message"))
			{
				xmlReader.GetNodeContent(sHGBYMSG.strmessage);
			}

			CString strBody = strRetStr.c_str();
			//strBody = strBody.MakeLower();
			int nBegin = strBody.Find(L"<body>");
			int nEnd = strBody.Find(L"</body>");
			sHGBYMSG.strbody = strBody.Mid(nBegin+6, nEnd-nBegin-6);
		}

	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(RSInfo)联网失败", str);
	}

	return nRet;

}

std::wstring CHGBYWebLib_API::DecodeURI(LPCSTR szURIString)
{
	std::string strSource(szURIString);
	int nSrcLen = strSource.size();

	std::string strDestA;

	int nIndex(0);
	while (nIndex < nSrcLen)
	{
		if (strSource.at(nIndex) == '%'
			&& nIndex+2 <= nSrcLen)
		{
			std::string strValue = strSource.substr(nIndex, 3);
			int nValue(0);
			sscanf(strValue.c_str(), "%%%x", &nValue);
			strDestA += (char)nValue;
			nIndex += 3;
		}
		else if (strSource.at(nIndex) == '+')
		{
			strDestA += ' ';
			nIndex++;
		}
		else
		{
			strDestA += (char)strSource.at(nIndex);
			nIndex++;
		}
	}
	CString strDest(UTF8ToTChar(strDestA.c_str()));
	return strDest.GetString();
}
