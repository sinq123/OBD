#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CJNSXInterfaceLib_API
{
public:
	CJNSXInterfaceLib_API(void);
	virtual ~CJNSXInterfaceLib_API(void);

private:
	CJNSXInterfaceLib_API(const CJNSXInterfaceLib_API &);
	CJNSXInterfaceLib_API & operator =(const CJNSXInterfaceLib_API &);

public:
	// Sync() 函数
	static int Sync(const char * pchURL, const std::wstring& strMemo, std::wstring& strRet);
	// GetOtherInspectQueueByDate()函数
	static int GetOtherInspectQueueByDate(const char * pchURL, const std::wstring& strStaDate, const std::wstring& strEndDate, const std::wstring& strQueueType,
		const std::wstring& strMemo, std::wstring& strRet);
	// GetOtherInspectQueueByPlateID()函数
	static int GetOtherInspectQueueByPlateID(const char * pchURL, const std::wstring& strPlateNumber, const std::wstring& strPlateType, const std::wstring& strQueueType,
		const std::wstring& strMemo, std::wstring& strRet);
	// GetInspectQueueByDate()
	static int GetInspectQueueByDate(const char * pchURL, const std::wstring& strStaDate, const std::wstring& strEndDate, const std::wstring& strMemo,
		std::wstring& strRet);
	//GetInspectQueueByPlateID()函数
	static int GetInspectQueueByPlateID(const char * pchURL, const std::wstring& strPlateNumber, const std::wstring& strPlateType, const std::wstring& strMemo,
		std::wstring& strRet);
	// AbortInspect() 函数
	static int AbortInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strReason, const std::wstring& strMemo, std::wstring& strRet);
	// BeginRecord() 函数
	static int BeginRecord(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	// BeginInspect() 函数
	static int BeginInspect(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber,
		const std::wstring& strDriverID, const std::wstring& OperatorID, const std::wstring& strInspectParam ,const std::wstring& strMemo, std::wstring& strRet);
	// TakePhoto() 函数
	static int TakePhoto(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber, const std::wstring& strPicCode, 
		const std::wstring& strMemo, std::wstring& strRet);
	// UploadRealtimeData () 函数
	static int UploadRealtimeData(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strRealtimeData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// StopInspect() 函数
	static int StopInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	//GetLimit() 函数
	static int GetLimit(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	// EndInspect() 函数
	static int EndInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet);
	// EndRecord() 函数
	static int EndRecord(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	// BeginSelfTest() 函数（v3.0 删除）
	static int BeginSelfTest(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strMemo, std::wstring& strRet);
	// UploadSelfTestData() 函数
	static int UploadSelfTestData(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResultData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// EndSelfTest() 函数（v3.0 删除）
	static int EndSelfTest(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet);
	// BeginCalibrate()函数
	static int BeginCalibrate(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strMemo, std::wstring& strRet);
	// UploadCalibrateData() 函数
	static int UploadCalibrateData(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResultData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// EndCalibrate() 函数
	static int EndCalibrate(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet);
	// OBDInspect() 函数（V3.0 新增）
	static int OBDInspect(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strReportNumbet, const std::wstring& strOBDData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// GetInspectParams()函数（V3.0 新增）
	static int GetInspectParams(const char * pchURL, const std::wstring& strMemo, std::wstring& strRet);
	// FuelEvaporationInspect()函数（V3.0 新增）
	static int FuelEvaporationInspect(const char * pchURL, const std::wstring& strChannelID, const std::wstring& strReportNumber, const std::wstring& strRYZFData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// UploadOBDRealtimeData()函数（V3.0 新增）
	static int UploadOBDRealtimeData(const char * pchURL, const std::wstring& strOrder, const std::wstring& strVIN, const std::wstring& strRealtimeData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// OBDTXCheck() 函数（V3.3 新增）
	static int OBDTXCheck(const char * pchURL, const std::wstring& strVIN, const std::wstring& strMemo, std::wstring& strRet);
	
	// 日志位置
	static std::wstring LogFilePath(void);
};