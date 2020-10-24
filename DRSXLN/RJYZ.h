#pragma once

#include <string>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

class CRJYZ
{
public:
	CRJYZ();
	virtual ~CRJYZ();

private:
	CRJYZ(const CRJYZ &);
	CRJYZ & operator =(const CRJYZ &);

public:
	static bool IsYZ(std::wstring &strMsg, std::wstring &strDLMM );
};
