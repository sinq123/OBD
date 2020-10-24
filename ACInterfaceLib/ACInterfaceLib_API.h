#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CACInterfaceLib_API
{
public:
	CACInterfaceLib_API(void);
	virtual ~CACInterfaceLib_API(void);

private:
	CACInterfaceLib_API(const CACInterfaceLib_API &);
	CACInterfaceLib_API & operator =(const CACInterfaceLib_API &);

public:
	static int QueryObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr);
	static int QueryObjectOut_(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr);

	static int WriteObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr);
	static int WriteObjectOut_(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr);

	// »’÷æŒª÷√
	static std::wstring LogFilePath(void);
};