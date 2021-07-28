#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CACHBIntLib_API
{
public:
	CACHBIntLib_API(void);
	virtual ~CACHBIntLib_API(void);

private:
	CACHBIntLib_API(const CACHBIntLib_API &);
	CACHBIntLib_API & operator =(const CACHBIntLib_API &);

public:
	static int QueryObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr);

	static int WriteObjectOut(const char * pchURL, const std::wstring& strxmlDoc, std::wstring& strRetStr);

	// »’÷æŒª÷√
	static std::wstring LogFilePath(void);
};