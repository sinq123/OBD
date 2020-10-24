#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>
#include "IntInfo.h"

class AFX_EXT_CLASS CHGBYWebLib_API
{
public:
	CHGBYWebLib_API(void);
	virtual ~CHGBYWebLib_API(void);

private:
	CHGBYWebLib_API(const CHGBYWebLib_API &);
	CHGBYWebLib_API & operator =(const CHGBYWebLib_API &);

public:
	
	static int RSInfo(const char * pchURL, std::wstring& strRetStr, SHGBYMSG& sHGBYMSG,
		const std::wstring strFunctionID = L"", const std::wstring strParams = L"", const std::wstring strKeys = L"");

	// 日志位置
	static std::wstring LogFilePath(void);
	// 设置日志文件
	static DWORD SetLogFilePath(const std::wstring& strLogFilePath);
private:
	// 日志文件
	static std::wstring m_strLogFilePath;
	// 
	static std::wstring DecodeURI(LPCSTR szURIString);
};