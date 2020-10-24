#include "StdAfx.h"
#include "ATWebServiceLib_API.h"

#include "gsoap\soapGasServiceNewSoapProxy.h"
#include "gsoap\GasServiceNewSoap.nsmap"

std::wstring CATWebServic_API::m_strLogFilePath = L"";

CATWebServic_API::CATWebServic_API(void)
{
	// 默认日志位置
	m_strLogFilePath = LogFilePath().c_str();
}

CATWebServic_API::~CATWebServic_API(void)
{
}

std::wstring CATWebServic_API::LogFilePath(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"AT艾特%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}


DWORD CATWebServic_API::SetLogFilePath(const std::wstring& strLogFilePath)
{
	if (strLogFilePath.empty())
	{
		return 0x01;
	}
	else
	{
		m_strLogFilePath = strLogFilePath;
	}
	return 0x00;
}

// 7注册验证接口
// JczId:检测站编码	ZcTime：登记时间	ZcTime：登记时间
int CATWebServic_API::VehicleRegJudge(const char * pchURL, const std::wstring& strJczId , const std::wstring& strZcTime , 
	const std::wstring& strCarItems, std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__VehicleRegJudge send;
	_ns1__VehicleRegJudgeResponse sret;

	std::wstring m_strZcTime = strZcTime;
	std::wstring m_strJczId = strJczId;
	std::wstring m_strCarItems = strCarItems;

	send.ZcTime = &m_strZcTime;
	send.CarItems = &m_strCarItems;
	send.JczId = &m_strJczId;

	CString strMsg;
	strMsg.Format(L"ZcTime:%s, CarItems:%s, JczId:%s", send.ZcTime->c_str(), send.CarItems->c_str(), send.JczId->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(VehicleRegJudge)", strMsg);

	nRet = toasb.VehicleRegJudge(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.VehicleRegJudgeResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 8上线检测权限判断接口
// jczNo：检测站编号	CarPZ：车牌号	jcxNo：检测线名称	bdrq:标定日期
int CATWebServic_API::OnlineJudge(const char * pchURL, const std::wstring& strjczNo , const std::wstring& strjcxNo , 
	const std::wstring& strbdrq, const std::wstring& strCarPZ, std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__OnlineJudge send;
	_ns1__OnlineJudgeResponse sret;

	std::wstring m_strjczNo = strjczNo;
	std::wstring m_strjcxNo = strjcxNo;
	std::wstring m_strbdrq = strbdrq;
	std::wstring m_strCarPZ = strCarPZ;

	send.jczNo = &m_strjczNo;
	send.jcxNo = &m_strjcxNo;
	send.bdrq = &m_strbdrq;
	send.CarPZ = &m_strCarPZ;

	CString strMsg;
	strMsg.Format(L"jczNo:%s, jcxNo:%s, bdrq:%s, CarPZ:%s", send.jczNo->c_str(), send.jcxNo->c_str(), send.bdrq->c_str(), send.CarPZ->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(OnlineJudge)", strMsg);

	nRet = toasb.OnlineJudge(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.OnlineJudgeResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(OnlineJudge)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 9检测数据接口
// CheckDataItems：检测基本数据	CheckEndDataItems： 检测结果数据
int CATWebServic_API::AddCheckData(const char * pchURL, const std::wstring& strCheckDataItems , const std::wstring& strCheckEndDataItems , 
	std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AddCheckData send;
	_ns1__AddCheckDataResponse sret;

	std::wstring m_strCheckDataItems = strCheckDataItems;
	std::wstring m_strCheckEndDataItems = strCheckEndDataItems;

	send.CheckDataItems = &m_strCheckDataItems;
	send.CheckEndDataItems = &m_strCheckEndDataItems;
	
	CString strMsg;
	strMsg.Format(L"CheckDataItems:%s, CheckEndDataItems:%s", send.CheckDataItems->c_str(), send.CheckEndDataItems->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(AddCheckData)", strMsg);

	nRet = toasb.AddCheckData(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.AddCheckDataResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(AddCheckData)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 10OBD检测数据接口
// ObdDataItems：检测OBD数据
int CATWebServic_API::AddObdData(const char * pchURL, const std::wstring& strObdDataItems, std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AddObdData send;
	_ns1__AddObdDataResponse sret;

	std::wstring m_strObdDataItems = strObdDataItems;

	send.ObdDataItems = &m_strObdDataItems;
	
	CString strMsg;
	strMsg.Format(L"ObdDataItems:%s", send.ObdDataItems->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(AddObdData)", strMsg);

	nRet = toasb.AddObdData(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.AddObdDataResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(AddObdData)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 11OBDIUPR检测数据接口
// IuprDataItems：检测IUPR数据	ObdType： 燃油种类，柴油:CY 汽油：QY
int CATWebServic_API::AddIuprData(const char * pchURL, const std::wstring& strIuprDataItems , const std::wstring& strObdType , 
	std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AddIuprData send;
	_ns1__AddIuprDataResponse sret;

	std::wstring m_strIuprDataItems = strIuprDataItems;
	std::wstring m_strObdType = strObdType;

	send.IuprDataItems = &m_strIuprDataItems;
	send.ObdType = &m_strObdType;
	
	CString strMsg;
	strMsg.Format(L"IuprDataItems:%s, ObdType:%s", send.IuprDataItems->c_str(), send.ObdType->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(AddIuprData)", strMsg);

	nRet = toasb.AddIuprData(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.AddIuprDataResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(AddIuprData)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 12机动车外观检查数据接口
// WgjcDataItems：外观检查数据
int CATWebServic_API::AddWGJCData(const char * pchURL, const std::wstring& strWgjcDataItems, std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AddWGJCData send;
	_ns1__AddWGJCDataResponse sret;

	std::wstring m_strWgjcDataItems = strWgjcDataItems;

	send.WgjcDataItems = &m_strWgjcDataItems;
	
	CString strMsg;
	strMsg.Format(L"WgjcDataItems:%s", send.WgjcDataItems->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(AddWGJCData)", strMsg);

	nRet = toasb.AddWGJCData(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.AddWGJCDataResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(AddWGJCData)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 13检测设备信息上传接口
// addDeviceCheckData：检测设备数据
int CATWebServic_API::AddDeviceCheckData(const char * pchURL, const std::wstring& straddDeviceCheckData, std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AddDeviceCheckData send;
	_ns1__AddDeviceCheckDataResponse sret;

	std::wstring m_straddDeviceCheckData = straddDeviceCheckData;

	send.addDeviceCheckData = &m_straddDeviceCheckData;
	
	CString strMsg;
	strMsg.Format(L"addDeviceCheckData:%s", send.addDeviceCheckData->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(AddDeviceCheckData)", strMsg);

	nRet = toasb.AddDeviceCheckData(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.AddDeviceCheckDataResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(AddDeviceCheckData)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 14设备自检数据上传接口
// DeviceType设备类型 测功机类型：DY 五气分析仪：AN 烟度计：SM  Items
int CATWebServic_API::AddVerify(const char * pchURL, const std::wstring& strDeviceType , const std::wstring& strItems , 
	std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__AddVerify send;
	_ns1__AddVerifyResponse sret;

	std::wstring m_strDeviceType = strDeviceType;
	std::wstring m_strItems = strItems;

	send.DeviceType = &m_strDeviceType;
	send.Items = &m_strItems;
	
	CString strMsg;
	strMsg.Format(L"DeviceType:%s, Items:%s", send.DeviceType->c_str(), send.Items->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(AddVerify)", strMsg);

	nRet = toasb.AddVerify(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.AddVerifyResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(AddVerify)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 15报告打印判断接口
// lsbh：流水号	jczNo：检测站编号
int CATWebServic_API::PrintJudge(const char * pchURL, const std::wstring& strlsbh , const std::wstring& strjczNo , 
	std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__PrintJudge send;
	_ns1__PrintJudgeResponse sret;

	std::wstring m_strlsbh = strlsbh;
	std::wstring m_strjczNo = strjczNo;

	send.lsbh = &m_strlsbh;
	send.jczNo = &m_strjczNo;
	
	CString strMsg;
	strMsg.Format(L"lsbh:%s, jczNo:%s", send.lsbh->c_str(), send.jczNo->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(PrintJudge)", strMsg);

	nRet = toasb.PrintJudge(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.PrintJudgeResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(PrintJudge)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 16报告打印并上传接口
//lsbh：流水号	EncodeReport：检测报告进行转换后编码
int CATWebServic_API::PrintUp(const char * pchURL, const std::wstring& strlsbh , const std::wstring& strencodeReport , 
	std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__PrintUp send;
	_ns1__PrintUpResponse sret;

	std::wstring m_strlsbh = strlsbh;
	std::wstring m_strencodeReport = strencodeReport;

	send.lsbh = &m_strlsbh;
	send.encodeReport = &m_strencodeReport;
	
	CString strMsg;
	strMsg.Format(L"lsbh:%s, encodeReport:%s", send.lsbh->c_str(), send.encodeReport->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(PrintUp)", strMsg);

	nRet = toasb.PrintUp(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.PrintUpResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(PrintUp)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 17检测设备自动锁定接口
// jczNo：检测站编码	devBm：设备唯一码
int CATWebServic_API::LokeJudge(const char * pchURL, const std::wstring& strjczNo  , const std::wstring& strdevBm , 
	std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__LokeJudge send;
	_ns1__LokeJudgeResponse sret;

	std::wstring m_strjczNo = strjczNo;
	std::wstring m_strdevBm = strdevBm;

	send.jczNo = &m_strjczNo;
	send.devBm = &m_strdevBm;
	
	CString strMsg;
	strMsg.Format(L"jczNo:%s, devBm:%s", send.jczNo->c_str(), send.devBm->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(LokeJudge)", strMsg);

	nRet = toasb.LokeJudge(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.LokeJudgeResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(LokeJudge)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}

// 18燃油蒸发排放控制系统数据接口
// rykzItems：燃油蒸发数据参数项
int CATWebServic_API::RyzfControl (const char * pchURL, const std::wstring& strrykzItems, std::wstring& strRetStr)
{
	int nRet(99);
	GasServiceNewSoapProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	_ns1__RyzfControl send;
	_ns1__RyzfControlResponse sret;

	std::wstring m_strrykzItems = strrykzItems;

	send.rykzItems = &m_strrykzItems;
	
	CString strMsg;
	strMsg.Format(L"rykzItems:%s", send.rykzItems->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"发送", L"(RyzfControl)", strMsg);

	nRet = toasb.RyzfControl(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.RyzfControlResult->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(RyzfControl)联网成功", strRetStr.c_str());
	}
	else
	{
		CString str;
		str.Format(L"%d", nRet);
		strRetStr = str.GetString();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"接收", L"(VehicleRegJudge)联网失败", str);
	}

	return nRet;
}



