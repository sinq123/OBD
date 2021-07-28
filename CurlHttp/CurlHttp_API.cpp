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
	//����userdata�Ǵ�����ݵ�ָ��  ����������ȡ����
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
	curl = curl_easy_init();    // ��ʼ��

	headerlist = curl_slist_append(headerlist, "Expect:");
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");

	if (curl)
	{
		// ���õ�ַ
		std::string strURL = TCharToANSI(wstrURL.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		// �Զ���ͷ
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		// ����Post������
		std::string strPostData = TCharToUTF8(wstrPostData.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostData.c_str());
		// �����ص�html�����������
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBuf);
		// ����һ���ص���������ȡ����
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetPageString);
		// ���ó�ʱ
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10*1000);
		// ִ��
		nRetCode = curl_easy_perform(curl);
		// ����
		curl_easy_cleanup(curl);	
		curl_slist_free_all(headerlist);

		nRet = nRetCode;

		if (nRetCode == CURLE_OK)
		{
			wstrRet = UTF8ToTChar(strBuf.c_str());
		}
		else
		{
			wstrRet = L"����URLʧ��";
		}
	}
	else
	{
		wstrRet = L"CURL ��ʼ��ʧ��";
	}
	return nRet;

}

int CCurlHttp_API::CurlGet(const std::wstring &wstrURL, std::wstring &wstrRet)
{
	int nRet(99);

	CURL *curl;
	CURLcode nRetCode;
	curl = curl_easy_init();    // ��ʼ��

	if (curl)
	{
		std::string strBuf;
		// ���õ�ַ
		std::string strURL = TCharToANSI(wstrURL.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		// �����ص�html�����������
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBuf);
		// ����һ���ص���������ȡ����
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetPageString);
		// ���ó�ʱ
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10*1000);
		// ִ��
		nRetCode = curl_easy_perform(curl);
		// ����
		curl_easy_cleanup(curl);

		nRet = nRetCode;
		if (nRetCode == CURLE_OK)
		{
			wstrRet = UTF8ToTChar(strBuf.c_str());
		}
		else
		{
			wstrRet = L"����URLʧ��";
		}
	}
	else
	{
		wstrRet = L"CURL ��ʼ��ʧ��";
	}
	return nRet;
}