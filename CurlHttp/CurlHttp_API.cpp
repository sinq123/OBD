#include "stdafx.h"
#include "CurlHttp_API.h"
#include "kutfconv.h"

#define CURL_STATICLIB
#include "libcURL\include\curl.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#ifdef _DEBUG
#pragma comment(linker, "/NODEFAULTLIB:MSVCRT.lib")
#pragma comment(lib, "libcURL/lib/libcurld.lib")
#else
#pragma comment(lib, "libcURL/lib/libcurl.lib")
#endif



static size_t GetPageString(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	//参数userdata是存放数据的指针  其他三个获取内容
	std::string *strHtml = (std::string *)userdata;
	strHtml->append((char*)ptr, size * nmemb);
	return (size * nmemb);
}

CCurlHttp_API::CCurlHttp_API(void)
{
}

CCurlHttp_API::~CCurlHttp_API(void)
{
}

int CCurlHttp_API::CurlPost(const std::wstring &wstrURL, const std::wstring &wstrPostData, std::wstring &wstrRet)
{
	int nRet(99);

	CURL *curl;
	CURLcode nRetCode;
	struct curl_slist *headerlist = NULL;

	std::string strBuf;
	curl = curl_easy_init();    // 初始化

	headerlist = curl_slist_append(headerlist, "Expect:");
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");

	if (curl)
	{
		// 设置地址
		std::string strURL = TCharToANSI(wstrURL.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		// 自定义头
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		// 设置Post的数据
		std::string strPostData = TCharToUTF8(wstrPostData.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostData.c_str());
		// 将返回的html主体数据输出
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBuf);
		// 设置一个回掉函数来获取数据
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetPageString);
		// 设置超时
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10*1000);
		// 执行
		nRetCode = curl_easy_perform(curl);
		// 清理
		curl_easy_cleanup(curl);	
		curl_slist_free_all(headerlist);

		nRet = nRetCode;

		if (nRetCode == CURLE_OK)
		{
			wstrRet = UTF8ToTChar(strBuf.c_str());
		}
		else
		{
			wstrRet = L"调用URL失败";
		}
	}
	else
	{
		wstrRet = L"CURL 初始化失败";
	}
	return nRet;

}

int CCurlHttp_API::CurlGet(const std::wstring &wstrURL, std::wstring &wstrRet)
{
	int nRet(99);

	CURL *curl;
	CURLcode nRetCode;
	curl = curl_easy_init();    // 初始化

	if (curl)
	{
		std::string strBuf;
		// 设置地址
		std::string strURL = TCharToANSI(wstrURL.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		// 将返回的html主体数据输出
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBuf);
		// 设置一个回掉函数来获取数据
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetPageString);
		// 设置超时
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10*1000);
		// 执行
		nRetCode = curl_easy_perform(curl);
		// 清理
		curl_easy_cleanup(curl);

		nRet = nRetCode;
		if (nRetCode == CURLE_OK)
		{
			wstrRet = UTF8ToTChar(strBuf.c_str());
		}
		else
		{
			wstrRet = L"调用URL失败";
		}
	}
	else
	{
		wstrRet = L"CURL 初始化失败";
	}
	return nRet;
}