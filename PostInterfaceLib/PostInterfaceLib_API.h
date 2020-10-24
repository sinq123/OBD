#pragma once

#include <afxinet.h>
#include <string>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#define IE_AGENT _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)")

// 操作成功
#define SUCCESS 0
// 操作失败
#define FAILURE 1
// 操作超时
#define OUTTIME 2

class AFX_EXT_CLASS CPOSTInterfaceLib_API
{
public:
	CPOSTInterfaceLib_API(LPCTSTR strAgent = IE_AGENT);
	virtual ~CPOSTInterfaceLib_API(void);

private:
	CPOSTInterfaceLib_API(const CPOSTInterfaceLib_API &);
	CPOSTInterfaceLib_API & operator =(const CPOSTInterfaceLib_API &);

public:
	static int HttpGet(const std::wstring& strUrl, const std::wstring& strPostData, std::wstring& strRet);
	static int HttpPost(const std::wstring& strUrl, const std::wstring& strPostData, std::wstring& strRet);

	// 日志位置
	static std::wstring LogFilePath(void);

private:
	static int ExecteRequest(const std::wstring& strMethod, const std::wstring& strUrl, const std::wstring& strPostData, std::wstring& strRet);
	static std::string ws2s(const std::wstring &wstr);
	static std::wstring s2ws(const std::string& str);

};