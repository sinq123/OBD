#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CHNInterfaceLib_API
{
public:
	CHNInterfaceLib_API(void);
	virtual ~CHNInterfaceLib_API(void);

private:
	CHNInterfaceLib_API(const CHNInterfaceLib_API &);
	CHNInterfaceLib_API & operator =(const CHNInterfaceLib_API &);

public:
	static int ObjectOut(const char * pchURL, const std::wstring strmethodName, const std::wstring& strjsonParam, const std::wstring& strresultData, 
		const std::wstring& struserName, const std::wstring& strpassWord, std::wstring& strRetStr);

		// »’÷æŒª÷√
	static std::wstring LogFilePath(void);

};