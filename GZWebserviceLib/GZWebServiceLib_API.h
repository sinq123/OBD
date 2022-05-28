#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CGZWebServic_API
{
public:
	CGZWebServic_API(void);
	virtual ~CGZWebServic_API(void);

private:
	CGZWebServic_API(const CGZWebServic_API &);
	CGZWebServic_API & operator =(const CGZWebServic_API &);

public:
	// 测功机加载滑行
	static int cgjLSPSelfcheck(const char * pchURL, const bool& bDieselOil, const std::wstring& strAvitualTime, const std::wstring& strArealTime,
		const std::wstring& strBvitualTime, const std::wstring& strBrealTime, const std::wstring& strCvitualTime, const std::wstring& strCrealTime,
		const std::wstring& strDvitualTime, const std::wstring& strDrealTime, const std::wstring& strregistCode, const std::wstring& strALSpower, 
		const std::wstring& strBLSpower, const std::wstring& strCLSpower, const std::wstring& strDLSpower, const std::wstring& strcheckResult, 
		const std::wstring& strcheckTimeStart, const std::wstring& strcheckTimeEnd, const std::wstring& strremark, std::wstring& strRetStr);

	static int cgjSelfcheck(const char * pchURL, const std::wstring& strregistCode, const std::wstring strHVitualTime, const std::wstring strHrealTime,
		const std::wstring& strLvitualTime, const std::wstring strLrealTime, const std::wstring strHpower, const std::wstring strLpower,
		const std::wstring& strcheckResult, const std::wstring strcheckTimeStart, const std::wstring strcheckTimeEnd, const std::wstring strremark,
		std::wstring& strRetStr);

	static int cgjLASelfcheck(const char * pchURL, const std::wstring& strregistCode, const std::wstring strAvitualTime, const std::wstring strArealTime,
		const std::wstring& strBvitualTime, const std::wstring strBrealTime, const std::wstring strCvitualTime, const std::wstring strCrealTime,
		const std::wstring& strDvitualTime, const std::wstring strDrealTime, const std::wstring strcheckResult, const std::wstring strcheckTimeStart,
		const std::wstring strcheckTimeEnd, const std::wstring strremark, std::wstring& strRetStr);


	static int getSatate(const char * pchURL, const std::wstring& strregistCode, std::wstring& strRetStr);

	// 日志位置
	static std::wstring LogFilePath(void);
	// 设置日志文件
	static DWORD SetLogFilePath(const std::wstring& strLogFilePath);
private:
	// 日志文件
	static std::wstring m_strLogFilePath;
};