#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CHYInterfaceLib_API
{
public:
	CHYInterfaceLib_API(void);
	virtual ~CHYInterfaceLib_API(void);

private:
	CHYInterfaceLib_API(const CHYInterfaceLib_API &);
	CHYInterfaceLib_API & operator =(const CHYInterfaceLib_API &);

public:
	static int ObjectOut(const char * pchURL, const std::wstring& strkey, const std::wstring& strjkid, 
		const std::wstring& strxmlDoc, std::wstring& strRetStr);

	// ��־λ��
	static std::wstring LogFilePath(void);
	// ������־�ļ�
	static DWORD SetLogFilePath(const std::wstring& strLogFilePath);
private:
	// ��־�ļ�
	static std::wstring m_strLogFilePath;

};