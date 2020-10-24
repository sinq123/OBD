#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CZKYTInterfaceLib_API
{
public:
	CZKYTInterfaceLib_API(void);
	virtual ~CZKYTInterfaceLib_API(void);

private:
	CZKYTInterfaceLib_API(const CZKYTInterfaceLib_API &);
	CZKYTInterfaceLib_API & operator =(const CZKYTInterfaceLib_API &);

public:
	static int getSqm(const char * pchURL, const std::wstring& strjcxbh, std::wstring& strRetStr);

	static int jdcjk(const char * pchURL, const std::wstring& strsqm, const std::wstring& strjklx, const std::wstring& strjhsj, std::wstring& strRetStr);

	// »’÷æŒª÷√
	static std::wstring LogFilePath(void);
};