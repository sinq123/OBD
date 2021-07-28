#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CSZIntLibbd_New_API
{
public:
	CSZIntLibbd_New_API(void);
	virtual ~CSZIntLibbd_New_API(void);

private:
	CSZIntLibbd_New_API(const CSZIntLibbd_New_API &);
	CSZIntLibbd_New_API & operator =(const CSZIntLibbd_New_API &);

public:
	// 上传检测结果
	static int UploadInspectionResult(const char * pchURL, const std::wstring& strcheckresult, const int& ncheckmethod, std::wstring& strRetStr);

	// 日志位置
	static std::wstring LogFilePath(void);
};