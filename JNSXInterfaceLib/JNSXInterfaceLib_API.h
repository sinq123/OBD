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
	// Sync() ����
	static int Sync(const char * pchURL, const std::wstring& strMemo, std::wstring& strRet);
	// GetOtherInspectQueueByDate()����
	static int GetOtherInspectQueueByDate(const char * pchURL, const std::wstring& strStaDate, const std::wstring& strEndDate, const std::wstring& strQueueType,
		const std::wstring& strMemo, std::wstring& strRet);
	// GetOtherInspectQueueByPlateID()����
	static int GetOtherInspectQueueByPlateID(const char * pchURL, const std::wstring& strPlateNumber, const std::wstring& strPlateType, const std::wstring& strQueueType,
		const std::wstring& strMemo, std::wstring& strRet);
	// GetInspectQueueByDate()
	static int GetInspectQueueByDate(const char * pchURL, const std::wstring& strStaDate, const std::wstring& strEndDate, const std::wstring& strMemo,
		std::wstring& strRet);
	//GetInspectQueueByPlateID()����
	static int GetInspectQueueByPlateID(const char * pchURL, const std::wstring& strPlateNumber, const std::wstring& strPlateType, const std::wstring& strMemo,
		std::wstring& strRet);
	// AbortInspect() ����
	static int AbortInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strReason, const std::wstring& strMemo, std::wstring& strRet);
	// BeginRecord() ����
	static int BeginRecord(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	// BeginInspect() ����
	static int BeginInspect(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber,
		const std::wstring& strDriverID, const std::wstring& OperatorID, const std::wstring& strInspectParam ,const std::wstring& strMemo, std::wstring& strRet);
	// TakePhoto() ����
	static int TakePhoto(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber, const std::wstring& strPicCode, 
		const std::wstring& strMemo, std::wstring& strRet);
	// UploadRealtimeData () ����
	static int UploadRealtimeData(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strRealtimeData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// StopInspect() ����
	static int StopInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	//GetLimit() ����
	static int GetLimit(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	// EndInspect() ����
	static int EndInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet);
	// EndRecord() ����
	static int EndRecord(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet);
	// BeginSelfTest() ������v3.0 ɾ����
	static int BeginSelfTest(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strMemo, std::wstring& strRet);
	// UploadSelfTestData() ����
	static int UploadSelfTestData(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResultData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// EndSelfTest() ������v3.0 ɾ����
	static int EndSelfTest(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet);
	// BeginCalibrate()����
	static int BeginCalibrate(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strMemo, std::wstring& strRet);
	// UploadCalibrateData() ����
	static int UploadCalibrateData(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResultData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// EndCalibrate() ����
	static int EndCalibrate(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet);
	// OBDInspect() ������V3.0 ������
	static int OBDInspect(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strReportNumbet, const std::wstring& strOBDData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// GetInspectParams()������V3.0 ������
	static int GetInspectParams(const char * pchURL, const std::wstring& strMemo, std::wstring& strRet);
	// FuelEvaporationInspect()������V3.0 ������
	static int FuelEvaporationInspect(const char * pchURL, const std::wstring& strChannelID, const std::wstring& strReportNumber, const std::wstring& strRYZFData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// UploadOBDRealtimeData()������V3.0 ������
	static int UploadOBDRealtimeData(const char * pchURL, const std::wstring& strOrder, const std::wstring& strVIN, const std::wstring& strRealtimeData, 
		const std::wstring& strMemo, std::wstring& strRet);
	// OBDTXCheck() ������V3.3 ������
	static int OBDTXCheck(const char * pchURL, const std::wstring& strVIN, const std::wstring& strMemo, std::wstring& strRet);
	
	// ��־λ��
	static std::wstring LogFilePath(void);
};