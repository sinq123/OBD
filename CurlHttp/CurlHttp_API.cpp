#include "stdafx.h"
#include "CurlHttp_API.h"
#include "kutfconv.h"
#include <iostream>

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

static size_t DownFile(void * ptr, size_t size, size_t nmemb, FILE *stream)
{
	//����stream�Ǵ�����ݵ�ָ��  ����������ȡ����
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

CCurlHttp_API::CCurlHttp_API(void)
{
}

CCurlHttp_API::~CCurlHttp_API(void)
{
}

int CCurlHttp_API::download_file(const std::wstring &wstrURL, const std::wstring &wstrFilePath)
{
	int nRet(99);

	CURL *curl;
	FILE *fp;
	CURLcode nRetCode;
	/*   ����curl_global_init()��ʼ��libcurl  */
	nRetCode = curl_global_init(CURL_GLOBAL_ALL);

	if (CURLE_OK != nRetCode)
	{
		printf("init libcurl failed.");
		curl_global_cleanup();
		return -1;
	}

	/*  ����curl_easy_init()�����õ� easy interface��ָ��  */
	curl = curl_easy_init();

	if (curl)
	{
		std::string strFilePath = TCharToANSI(wstrFilePath.c_str());
		fopen_s(&fp, strFilePath.c_str(),"wb");
		// ���õ�ַ
		std::string strURL = TCharToANSI(wstrURL.c_str());
		nRetCode = curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		if (nRetCode != CURLE_OK)
		{
			fclose(fp);
			curl_easy_cleanup(curl);
			return -1;
		}
		/*  ����curl_easy_setopt()���õĴ���ѡ�ʵ�ֻص�����������û��ض�����  */
		nRetCode = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownFile);
		if (nRetCode != CURLE_OK)
		{
			fclose(fp);
			curl_easy_cleanup(curl);
			return -1;
		}
		/*  ����curl_easy_setopt()���õĴ���ѡ�ʵ�ֻص�����������û��ض�����  */
		nRetCode = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		if (nRetCode != CURLE_OK)
		{
			fclose(fp);
			curl_easy_cleanup(curl);
			return -1;
		}

		nRetCode = curl_easy_perform(curl);
		// ����curl_easy_perform()������ɴ�������
		fclose(fp);
		/* Check for errors */
		if(nRetCode != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(nRetCode));
			curl_easy_cleanup(curl);
			return -1;
		}

		curl_easy_cleanup(curl);
		// ����curl_easy_cleanup()�ͷ��ڴ�
		curl_global_cleanup();
	}
	else
	{
		//wstrRet = L"CURL ��ʼ��ʧ��";
	}

	return nRet;
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
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json; charset=utf-8");

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