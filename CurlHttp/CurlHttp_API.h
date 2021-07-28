#pragma once

#include <memory>
#include <string>


class AFX_EXT_CLASS CCurlHttp_API
{
public:
	CCurlHttp_API(void);
	virtual ~CCurlHttp_API(void);

private:
	CCurlHttp_API(const CCurlHttp_API &);
	CCurlHttp_API & operator = (const CCurlHttp_API &);



public:
	// post方法
	static int CurlPost(const std::wstring &wstrURL, const std::wstring &wstrPostData, std::wstring &wstrRet);
	// get方法
	static int CurlGet(const std::wstring &wstrURL, std::wstring &wstrRet);


};