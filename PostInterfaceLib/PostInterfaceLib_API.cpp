#include "StdAfx.h"
#include "PostInterfaceLib_API.h"

#define BUFFER_SIZE 1024

#define NORMAL_CONNECT INTERNET_FLAG_KEEP_CONNECTION
#define SECURE_CONNECT NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define NORMAL_REQUEST INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE
#define SECURE_REQUEST NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID

CPOSTInterfaceLib_API::CPOSTInterfaceLib_API(LPCTSTR strAgent/* = IE_AGENT*/)
{

}

CPOSTInterfaceLib_API::~CPOSTInterfaceLib_API()
{

}

std::wstring CPOSTInterfaceLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"新疆OBD%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

int CPOSTInterfaceLib_API::ExecteRequest(const std::wstring& strMethod, const std::wstring& strUrl, const std::wstring& strPostData, std::wstring& strRet)
{

	CInternetSession *m_pSession;
	CHttpConnection *m_pConnection;
	CHttpFile *m_pFile;

	m_pSession = new CInternetSession(IE_AGENT);
	m_pConnection = NULL;
	m_pFile = NULL;

	CString strServer, strObject;
	DWORD dwServiceType;
	INTERNET_PORT nPort;
	AfxParseURL(strUrl.c_str(), dwServiceType, strServer, strObject, nPort);

	CNHLogAPI::WriteLogEx(CPOSTInterfaceLib_API::LogFilePath().c_str(), L"发送", strUrl.c_str(), strPostData.c_str());

	if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
	{
		CString str;
		strRet = L"URL为空或不包含一个已知的Internet服务类型";
		str.Format(L"代号(%d, %x)\r\n%s", dwServiceType, dwServiceType, strRet.c_str());
		CNHLogAPI::WriteLogEx(CPOSTInterfaceLib_API::LogFilePath().c_str(), L"错误", L"", str);
		// 释放参数
		if (true)
		{
			if (m_pFile != NULL)
			{
				m_pFile->Close();
				delete m_pFile;
				m_pFile = NULL;
			}
			if (m_pConnection != NULL)
			{
				m_pConnection->Close();
				delete m_pConnection;
				m_pConnection = NULL;
			}
			if (m_pSession != NULL)
			{
				m_pSession->Close();
				delete m_pSession;
				m_pSession = NULL;
			}
		}

		return FAILURE;
	}

	try
	{
		m_pConnection = m_pSession->GetHttpConnection(strServer, dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT, nPort);
		m_pFile = m_pConnection->OpenRequest(strMethod.c_str(), strObject, NULL, 1, NULL, NULL, (dwServiceType == AFX_INET_SERVICE_HTTP? NORMAL_REQUEST : SECURE_REQUEST));

		m_pFile->AddRequestHeaders(L"Accept: *,*/*");
		m_pFile->AddRequestHeaders(L"Accept-Languahe: zh-cn");
		m_pFile->AddRequestHeaders(L"Content-Yype: application/x-www-from-urlencoded");
		m_pFile->AddRequestHeaders(L"Accept-Encoding: gzip, deflate");

		m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)strPostData.c_str(), strPostData == L"" ? 0: strPostData.length());
		m_pFile->WriteString((LPCTSTR)strPostData.c_str());
		m_pFile->EndRequest();
		DWORD dwRet;
		m_pFile->QueryInfoStatusCode(dwRet);
		if (dwRet == HTTP_STATUS_OK)
		{
			//char szchars[BUFFER_SIZE + 1] = {0};
			//std::string strRawRet = "";
			//UINT nRet = 0;
			//while ((nRet = m_pFile->Read((void*)szchars, BUFFER_SIZE)) > 0)
			//{
			//	szchars[nRet] = '\0';
			//	strRawRet += szchars;
			//	memset(szchars, 0, BUFFER_SIZE + 1);
			//}
			CString str, strTemp;
			UINT nRet = 0;
			while ((nRet = m_pFile->ReadString(str)) > 0)
			{
				strTemp += str;
			}

			strRet = strTemp.GetString();
		}

		CNHLogAPI::WriteLogEx(CPOSTInterfaceLib_API::LogFilePath().c_str(), L"返回", L"", strRet.c_str());
	}
	catch (CInternetException* e)
	{


		CString str;
		DWORD dwErrorCOde = e->m_dwError;
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		e->Delete();

		DWORD dwError = GetLastError();
		str.Format(L"IE代码%d,%x \r\n%s; 错误代码%d,%x", dwErrorCOde, dwErrorCOde,
			szError, dwError, dwError);

		// 释放参数
		if (true)
		{
			if (m_pFile != NULL)
			{
				m_pFile->Close();
				delete m_pFile;
				m_pFile = NULL;
			}
			if (m_pConnection != NULL)
			{
				m_pConnection->Close();
				delete m_pConnection;
				m_pConnection = NULL;
			}
			if (m_pSession != NULL)
			{
				m_pSession->Close();
				delete m_pSession;
				m_pSession = NULL;
			}
		}
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CPOSTInterfaceLib_API::LogFilePath().c_str(), L"错误", L"", str);

		if (ERROR_INTERNET_TIMEOUT == dwErrorCOde)
		{
			return OUTTIME;
		}
		else
		{
			return FAILURE;
		}

	}

	// 释放参数
	if (true)
	{
		if (m_pFile != NULL)
		{
			m_pFile->Close();
			delete m_pFile;
			m_pFile = NULL;
		}
		if (m_pConnection != NULL)
		{
			m_pConnection->Close();
			delete m_pConnection;
			m_pConnection = NULL;
		}
		if (m_pSession != NULL)
		{
			m_pSession->Close();
			delete m_pSession;
			m_pSession = NULL;
		}
	}

	return SUCCESS;
}


int CPOSTInterfaceLib_API::HttpGet(const std::wstring& strUrl, const std::wstring& strPostData, std::wstring& strRet)
{
	return ExecteRequest(L"GET", strUrl, strPostData, strRet);
}

int CPOSTInterfaceLib_API::HttpPost(const std::wstring& strUrl, const std::wstring& strPostData, std::wstring& strRet)
{
	return ExecteRequest(L"POST", strUrl, strPostData, strRet);
}

std::string CPOSTInterfaceLib_API::ws2s(const std::wstring &wstr)
{
	std::string curLocale = setlocale(LC_ALL, NULL);	// curLocale = "C"
	setlocale(LC_ALL, "chs");
	const wchar_t *pwchSource = wstr.c_str();
	size_t sizeDest = sizeof(wchar_t) * (wstr.size() + 1);
	char *pchDest = new char[sizeDest];
	memset(pchDest, 0, sizeDest);
	size_t sizeTran(0);
	wcstombs_s(&sizeTran, pchDest, sizeDest, pwchSource, _TRUNCATE);
	std::string strResult = pchDest;
	delete []pchDest;
	setlocale(LC_ALL, curLocale.c_str());
	return strResult;
}

std::wstring CPOSTInterfaceLib_API::s2ws(const std::string& str)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char *pchSource = str.c_str();
	size_t sizeDest = str.size() + 1;
	wchar_t *pwchDest = new wchar_t[sizeDest];
	wmemset(pwchDest, 0, sizeDest);
	size_t sizeTran(0);
	mbstowcs_s(&sizeTran, pwchDest, sizeDest, pchSource, _TRUNCATE);
	std::wstring wstrResult = pwchDest;
	delete []pwchDest;
	setlocale(LC_ALL, curLocale.c_str());
	return wstrResult;
}