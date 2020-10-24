#include "StdAfx.h"
#include "JNSXInterfaceLib_API.h"

#include "gsoap\soapServiceSoapProxy.h"
#include "gsoap\ServiceSoap.nsmap"

CJNSXInterfaceLib_API::CJNSXInterfaceLib_API(void)
{
}

CJNSXInterfaceLib_API::~CJNSXInterfaceLib_API(void)
{
}

std::wstring CJNSXInterfaceLib_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"JNSX%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}


int CJNSXInterfaceLib_API::Sync(const char * pchURL, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__Sync sendSync;
	_ns1__SyncResponse Response;

	std::wstring m_strMemo(strMemo);
	sendSync.Memo = &m_strMemo;

	nRet = toasb.Sync(&sendSync, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.SyncResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();

		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"GetOtherInspectQueueByDate", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::GetOtherInspectQueueByDate(const char * pchURL, const std::wstring& strStaDate, const std::wstring& strEndDate, const std::wstring& strQueueType,
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__GetOtherInspectQueueByDate send;
	_ns1__GetOtherInspectQueueByDateResponse Response;

	std::wstring m_strStaData(strStaDate);
	std::wstring m_strEndData(strEndDate);
	std::wstring m_strQueueType(strQueueType);
	std::wstring m_strMemo(strMemo);

	send.FromDate = &m_strStaData;
	send.ToDate = &m_strEndData;
	send.QueueType = &m_strQueueType;
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"FromDate:%s, QueueType:%s", send.FromDate->c_str(), send.QueueType->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetOtherInspectQueueByDate(&send, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.GetOtherInspectQueueByDateResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();

		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"GetOtherInspectQueueByDate", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::GetOtherInspectQueueByPlateID(const char * pchURL, const std::wstring& strPlateNumber, const std::wstring& strPlateType, const std::wstring& strQueueType,
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__GetOtherInspectQueueByPlateID send;
	_ns1__GetOtherInspectQueueByPlateIDResponse Response;

	std::wstring m_strPlateNumber(strPlateNumber);
	std::wstring m_strPlateType(strPlateType);
	std::wstring m_strQueueType(strQueueType);
	std::wstring m_strMemo(strMemo);

	send.PlateID = &m_strPlateNumber;
	send.PlateType = _wtoi(m_strPlateType.c_str());
	send.QueueType = &m_strQueueType;
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"PlateID:%s, QueueType:%d, PlateType:%s", send.PlateID->c_str(), send.PlateType, send.QueueType->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetOtherInspectQueueByPlateID(&send, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.GetOtherInspectQueueByPlateIDResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"返回", L"GetOtherInspectQueueByPlateID", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::GetInspectQueueByDate(const char * pchURL, const std::wstring& strStaDate, const std::wstring& strEndDate, const std::wstring& strMemo,
		std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__GetInspectQueueByDate send;
	_ns1__GetInspectQueueByDateResponse Response;

	std::wstring m_strStaData(strStaDate);
	std::wstring m_strEndData(strEndDate);
	std::wstring m_strMemo(strMemo);

	send.FromDate = &m_strStaData;
	send.ToDate = &m_strEndData;
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"FromDate:%s, ToDate:%s", send.FromDate->c_str(), send.ToDate->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetInspectQueueByDate(&send, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.GetInspectQueueByDateResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"返回", L"GetInspectQueueByDate", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::GetInspectQueueByPlateID(const char * pchURL, const std::wstring& strPlateNumber, const std::wstring& strPlateType, const std::wstring& strMemo,
		std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__GetInspectQueueByPlateID send;
	_ns1__GetInspectQueueByPlateIDResponse Response;

	std::wstring m_strPlateNumber(strPlateNumber);
	std::wstring m_strPlateType(strPlateType);
	std::wstring m_strMemo(strMemo);

	send.PlateID = &m_strPlateNumber;
	send.PlateType = _wtoi(m_strPlateType.c_str());
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"PlateID:%s, PlateType:%d", send.PlateID->c_str(), send.PlateType);
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetInspectQueueByPlateID(&send, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.GetInspectQueueByPlateIDResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"GetInspectQueueByPlateID", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::AbortInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strReason, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AbortInspect send;
	_ns1__AbortInspectResponse Response;

	std::wstring m_strReportNumber(strReportNumber);
	std::wstring m_strReason(strReason);
	std::wstring m_strMemo(strMemo);

	send.InspectID = &m_strReportNumber;
	send.Reason = _wtoi(m_strReason.c_str());
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"InspectID:%s, Reason:%d", send.InspectID->c_str(), send.Reason);
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);


	nRet = toasb.AbortInspect(&send, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.AbortInspectResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"AbortInspect", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::BeginRecord(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__BeginRecord send;
	_ns1__BeginRecordResponse Response;

	std::wstring m_strLineNUmber(strLineNUmber);
	std::wstring m_strReportNumber(strReportNumber);

	send.LineID = _wtol(m_strLineNUmber.c_str());
	send.InspectID = &m_strReportNumber;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, InspectID:%s", send.LineID, send.InspectID->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.BeginRecord(&send, Response);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Response.BeginRecordResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"BeginRecord", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::BeginInspect(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber,
		const std::wstring& strDriverID, const std::wstring& OperatorID, const std::wstring& strInspectParam ,const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__BeginInspect send;
	_ns1__BeginInspectResponse Res;

	std::wstring m_strLineNUmber(strLineNUmber);
	std::wstring m_strReportNumber(strReportNumber);
	std::wstring m_strDriverID(strDriverID);
	std::wstring m_OperatorID(OperatorID);
	std::wstring m_strInspectParam(strInspectParam);

	send.LineID = _wtol(m_strLineNUmber.c_str());
	send.InspectID = &m_strReportNumber;
	send.DriverID = _wtol(m_strDriverID.c_str());
	send.OperatorID = _wtol(m_OperatorID.c_str());
	send.InspectParam = &m_strInspectParam;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, InspectID:%s, DriverID:%d, OperatorID:%d, InspectParam:%s", send.LineID, send.InspectID->c_str(), send.DriverID,
		send.OperatorID, send.InspectParam->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.BeginInspect(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.BeginInspectResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"BeginInspect", L"", strRet.c_str());
	}
	return nRet;

}

int CJNSXInterfaceLib_API::TakePhoto(const char * pchURL, const std::wstring& strLineNUmber, const std::wstring& strReportNumber, const std::wstring& strPicCode, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__TakePhoto send;
	_ns1__TakePhotoResponse Res;

	std::wstring m_strLineNUmber(strLineNUmber);
	std::wstring m_strReportNumber(strReportNumber);
	std::wstring m_strPicCode(strPicCode);

	send.LineID = _wtol(m_strLineNUmber.c_str());
	send.InspectID = &m_strReportNumber;
	send.PicCode = _wtoi(m_strPicCode.c_str());

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, InspectID:%s, PicCode:%d", send.LineID, send.InspectID->c_str(), send.PicCode);
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.TakePhoto(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.TakePhotoResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"TakePhoto", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::UploadRealtimeData(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strRealtimeData, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__UploadRealtimeData send;
	_ns1__UploadRealtimeDataResponse Res;

	std::wstring m_strReportNumber(strReportNumber);
	std::wstring m_strRealtimeData(strRealtimeData);

	send.InspectID = &m_strReportNumber;
	send.RealtimeData = &m_strRealtimeData;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"InspectID:%s, RealtimeData:%s", send.InspectID->c_str(), send.RealtimeData->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.UploadRealtimeData(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.UploadRealtimeDataResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"UploadRealtimeData", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::StopInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__StopInspect send;
	_ns1__StopInspectResponse Res;

	std::wstring m_strReportNumber(strReportNumber);

	send.InspectID = &m_strReportNumber;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"InspectID:%s", send.InspectID->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.StopInspect(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.StopInspectResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"StopInspect", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::GetLimit(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__GetLimit send;
	_ns1__GetLimitResponse Res;

	std::wstring m_strReportNumber(strReportNumber);

	send.InspectID = &m_strReportNumber;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"InspectID:%s", send.InspectID->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetLimit(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.GetLimitResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"GetLimit", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::EndInspect(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__EndInspect send;
	_ns1__EndInspectResponse Res;

	std::wstring m_strReportNumber(strReportNumber);
	std::wstring m_strResult(strResult);

	send.InspectID = &m_strReportNumber;
	send.Result = &m_strResult;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"InspectID:%s, m_strResult:%s", send.InspectID->c_str(), send.Result->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.EndInspect(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.EndInspectResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"EndInspect", L"", strRet.c_str());
	}
	return nRet;

}

int CJNSXInterfaceLib_API::EndRecord(const char * pchURL, const std::wstring& strReportNumber, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__EndRecord send;
	_ns1__EndRecordResponse Res;

	std::wstring m_strReportNumber(strReportNumber);

	send.InspectID = &m_strReportNumber;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"InspectID:%s", send.InspectID->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.EndRecord(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.EndRecordResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"EndRecord", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::BeginSelfTest(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strMemo, std::wstring& strRet)
{
	return 99;
}

int CJNSXInterfaceLib_API::UploadSelfTestData(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResultData, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__UploadSelfTestData send;
	_ns1__UploadSelfTestDataResponse Res;

	std::wstring m_strLineNumber(strLineNumber);
	std::wstring m_strResultData(strResultData);

	send.LineID = _wtol(strLineNumber.c_str());
	send.ResultData = &m_strResultData;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, ResultData:%s", send.LineID, send.ResultData->c_str() );
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.UploadSelfTestData(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.UploadSelfTestDataResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"UploadSelfTestData", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::EndSelfTest(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	return 99;
}

int CJNSXInterfaceLib_API::BeginCalibrate(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__BeginCalibrate send;
	_ns1__BeginCalibrateResponse Res;

	std::wstring m_strLineNumber(strLineNumber);

	send.LineID = _wtol(m_strLineNumber.c_str());

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d", send.LineID);
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.BeginCalibrate(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.BeginCalibrateResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"BeginCalibrate", L"", strRet.c_str());
	}
	return nRet;

}

int CJNSXInterfaceLib_API::UploadCalibrateData(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResultData, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__UploadCalibrateData send;
	_ns1__UploadCalibrateDataResponse Res;

	std::wstring m_strLineNumber(strLineNumber);
	std::wstring m_strResultData(strResultData);
	send.LineID = _wtol(m_strLineNumber.c_str());
	send.ResultData = &m_strResultData;
	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, ResultData:%s, ", send.LineID, send.ResultData->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.UploadCalibrateData(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.UploadCalibrateDataResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"UploadCalibrateData", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::EndCalibrate(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strResult, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__EndCalibrate send;
	_ns1__EndCalibrateResponse Res;

	std::wstring m_strLineNumber(strLineNumber);
	std::wstring m_strResult(strResult);

	send.LineID = _wtol(m_strLineNumber.c_str());
	send.Result = &m_strResult;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, Result:%s, ", send.LineID, send.Result->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.EndCalibrate(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.EndCalibrateResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"EndCalibrate", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::OBDInspect(const char * pchURL, const std::wstring& strLineNumber, const std::wstring& strReportNumbet, const std::wstring& strOBDData, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__OBDInspect send;
	_ns1__OBDInspectResponse Res;

	std::wstring m_strLineNumber(strLineNumber);
	std::wstring m_strReportNumbet(strReportNumbet);
	std::wstring m_strOBDData(strOBDData);

	send.LineID = _wtol(m_strLineNumber.c_str());
	send.InspectID = &m_strReportNumbet;
	send.sOBDData = &m_strOBDData;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"LineID:%d, InspectID:%s, sOBDData:%s ", send.LineID, send.InspectID->c_str(), send.sOBDData->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.OBDInspect(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.OBDInspectResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"OBDInspect", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::GetInspectParams(const char * pchURL, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__GetInspectParams send;
	_ns1__GetInspectParamsResponse Res;

	std::wstring m_strMemo(strMemo);

	CString strLog;
	strLog.Format(L"");
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.GetInspectParams(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.GetInspectParamsResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"GetInspectParams", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::FuelEvaporationInspect(const char * pchURL, const std::wstring& strChannelID, const std::wstring& strReportNumber, const std::wstring& strRYZFData, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__FuelEvaporationInspect send;
	_ns1__FuelEvaporationInspectResponse Res;

	std::wstring m_strReportNumber(strReportNumber);
	std::wstring m_strChannelID(strChannelID);
	std::wstring m_strRYZFData(strRYZFData);

	send.ChannelID = _wtol(m_strChannelID.c_str());
	send.InspectID = &m_strReportNumber;
	send.RYZFData = &m_strRYZFData;

	std::wstring m_strMemo(strMemo);
	send.Memo = &m_strMemo;

	CString strLog;
	strLog.Format(L"ChannelID:%d, InspectID:%s, RYZFData:%s", send.ChannelID, send.InspectID->c_str(), send.RYZFData->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.FuelEvaporationInspect(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.FuelEvaporationInspectResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"FuelEvaporationInspect", L"", strRet.c_str());
	}
	return nRet;
}

int CJNSXInterfaceLib_API::UploadOBDRealtimeData(const char * pchURL, const std::wstring& strOrder, const std::wstring& strVIN, const std::wstring& strRealtimeData, 
		const std::wstring& strMemo, std::wstring& strRet)
{
	return 99;
}

int CJNSXInterfaceLib_API::OBDTXCheck(const char * pchURL, const std::wstring& strVIN, const std::wstring& strMemo, std::wstring& strRet)
{
	int nRet(99);
	
	ServiceSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__OBDTXCheck send;
	_ns1__OBDTXCheckResponse Res;

	std::wstring m_strVIN(strVIN);
	send.VIN = &m_strVIN;

	CString strLog;
	strLog.Format(L"VIN:%s", send.VIN->c_str());
	CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"发送", L"", strLog);

	nRet = toasb.OBDTXCheck(&send, Res);

	if (nRet == SOAP_OK)
	{
		ns1__RetValue ns1__RetValue;
		ns1__RetValue = *Res.OBDTXCheckResult;
		// 拼接返回的字符串
		// TipMessage\n 本参数的功能是向用户呈现由环保局下发的各种通知 Xml 格式字符串
		// ErrNum\n int 型，如果函数执行成功，此值为 0，否则为错误代号
		// ErrMsg\n string 型。如果函数执行成功，其值可能为空字符串或者含有重要提示 信息
		// InternalErrNum\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常代 号
		// InternalErrMsg\n 如果函数执行过程中出现内部异常（意外错误），则此值为内部异常消 息内容
		// Value\n 如果函数执行成功，则此值为函数返回的其它值，string 型。执行失败 则返回空字符串或其它信息 （注意：执行失败后此值的内容应尽量忽略， 请勿与执行成功时的返回值混淆）
		// Memo\n 留给将来扩展其它功能时用。如果无特殊说明，请直接传入空字符串。
		CString str;
		str.Format(L"%s\n%d\n%s\n%d\n%s\n%s\n%s\n,", ns1__RetValue.TipMessage->c_str(), ns1__RetValue.ErrNum, ns1__RetValue.ErrMsg->c_str(), 
			ns1__RetValue.InternalErrNum, ns1__RetValue.InternalErrMsg->c_str(), ns1__RetValue.Value->c_str(), ns1__RetValue.Memo->c_str());
		// 拼接完成，返回
		strRet = str.GetString();
		CNHLogAPI::WriteLogEx(CJNSXInterfaceLib_API::LogFilePath().c_str(), L"OBDTXCheck", L"", strRet.c_str());
	}
	return nRet;
}