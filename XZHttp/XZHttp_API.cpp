#include "stdafx.h"
#include "XZHttp_API.h"



#include "..\RapidjsonLib\RapidjsonLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\RapidjsonLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\RapidjsonLib.lib")
#endif

#include "..\CurlHttp\CurlHttp.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\CurlHttp_D.lib")
#else
#pragma comment(lib, "..\\Release\\CurlHttp.lib")
#endif


//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MYLib_D.lib")
#else
#pragma comment( lib, "..\\Release\\MYLib.lib" )
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT





CXZHttp_API::CXZHttp_API(void)
{
	// 模块名称
	m_strModName = L"XZHttp";

	// 加载配置文件
	LoadConfig();

	// 生成日志文件
	GenLogFile();

	// 初始化数据
	InitData();
}

CXZHttp_API::~CXZHttp_API(void)
{
}

CXZHttp_API& CXZHttp_API::GetInstance(void)
{
	static std::auto_ptr<CXZHttp_API> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CXZHttp_API());
	}
	return *(auto_ptr_instance.get());
}



void CXZHttp_API::GenLogFile(void)
{
	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetHLDFilePath(L"LOG", L"", wchLogFileFolderPath, true);

	// 删除lDays天前（含当天）的日志文件
	COleDateTime odtNow(COleDateTime::GetCurrentTime());
	const long lDays(30);
	COleDateTime odtDelLog = odtNow - COleDateTimeSpan(lDays, 0, 0, 0);
	SYSTEMTIME stDelLog;
	ZeroMemory(&stDelLog, sizeof(SYSTEMTIME));
	stDelLog.wYear = (WORD)odtDelLog.GetYear();
	stDelLog.wMonth = (WORD)odtDelLog.GetMonth();
	stDelLog.wDay = (WORD)odtDelLog.GetDay();
	stDelLog.wHour = 23;
	stDelLog.wMinute = 59;
	stDelLog.wSecond = 59;
	CNHLogAPI::DeleteLogEx(wchLogFileFolderPath, stDelLog);

	// 生成今天日志文件路径
	// 主程序日志文件
	CStringW strFileName;
	strFileName.Format(L"%s_%s.log", odtNow.Format(L"%Y-%m-%d"), m_strModName);
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;

}

void CXZHttp_API::InitData(void)
{
}

void CXZHttp_API::LoadConfig(void)
{
	// 读取配置文件
	wchar_t wchFilePath[MAX_PATH] = {L'\0'};
	CNHCommonAPI::GetHLDFilePath(L"Config", L"XZConfig.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	// URL地址
	m_wstrURL = si.GetString(L"NetConfig", L"WURL", L"");
	// 检验机构编号
	m_wstrJCZBH = si.GetString(L"NetConfig", L"jyjgbh", L"");
	// 用户名
	m_wstrUserName = si.GetString(L"NetConfig", L"WURL", L"");
	// 密码
	m_wstrPassWord = si.GetString(L"NetConfig", L"jyjgbh", L"");
}

CString CXZHttp_API::JsonValueToCString(Json::Value root)
{
	CString strRet(L"");

	if (root.type() == Json::nullValue)
	{
		strRet.Format(L"%s", L"");
	}
	if (root.type() == Json::stringValue)
	{
		strRet.Format(L"%s", UTF8ToTChar(root.asString().c_str()));
	}
	else if (root.type() == Json::booleanValue)
	{
		strRet.Format(L"%s", UTF8ToTChar(root.asString().c_str()));
	}
	else if (root.type() == Json::realValue)
	{
		strRet.Format(L"%f", root.asDouble());
	}
	else if (root.type() == Json::uintValue)
	{
		strRet.Format(L"%hu", root.asUInt());
	}
	else if (root.type() == Json::intValue)
	{
		strRet.Format(L"%d", root.asInt());
	}
	else if (root.type() == Json::arrayValue || root.type() == Json::objectValue)
	{
		Json::StyledWriter sw;
		strRet.Format(L"%s", UTF8ToTChar(sw.write(root).c_str()));
	}

	return strRet;
}

int CXZHttp_API::Cutl(const std::wstring wstrURL, const Json::Value &ReqBody, const std::wstring strServiceCode, std::wstring& wstrRet)
{
	Json::Value root;
	// 检测站编号
	root["TsNo"] = Json::Value(TCharToUTF8(m_wstrJCZBH.c_str()));
	// 密匙
	root["Token"] = Json::Value(TCharToUTF8(m_wstrPassWord.c_str()));
	// 数据类型
	root["serviceCode"] = Json::Value(TCharToUTF8(strServiceCode.c_str()));
	// 时间
	root["ReqTime"] = Json::Value(TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S")));
	// 数据
	root["ReqBody"] = Json::Value(ReqBody);

	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, wstrURL.c_str(), strServiceCode.c_str(), wstrPostData.c_str());


	return CCurlHttp_API::CurlPost(wstrURL, wstrPostData, wstrRet);
}

// 请求Token
bool CXZHttp_API::GetToken(void)
{
	CString strURL;
	
	Json::Value root;
	// username
	root["username"] = Json::Value(TCharToUTF8(m_wstrUserName.c_str()));
	// password
	root["password"] = Json::Value(TCharToUTF8(m_wstrPassWord.c_str()));


	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, strURL, L"", wstrPostData.c_str());

	std::wstring wstrRet;
	int nRet = CCurlHttp_API::CurlPost(strURL.GetString(), wstrPostData, wstrRet);

	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"获取Tokrn", L"返回参数", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			m_wstrPassWord = JsonValueToCString(root["access_token"]).GetString();
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"获取Token", L"返回参数", wstrRet.c_str());
		return false;
	}

	return true;
}

//OBD 检查基本信息	ODS_A7_OBD_INSPECTION_INFO
bool CXZHttp_API::OBDItemInfoA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{
	bool bRet(false);

	CString strURL;

	Json::Value root;

	//COMMUNICATION_NORMAL	OBD通信是否成功
	root["COMMUNICATION_NORMAL"] = TCharToUTF8(L"Y");
	//FAILURE_REASON	不成功原因-1接口损坏 2找不到接口 3连接后不能通信
	root["FAILURE_REASON"] = TCharToUTF8(L"");
	//FAULT_CODE_NUM	故障码数量
	root["FAULT_CODE_NUM"] = TCharToUTF8(L"0");
	//FAULT_INDICATOR_NORMAL	OBD故障指示器是否正常
	root["FAULT_INDICATOR_NORMAL"] = TCharToUTF8(L"Y");
	//FAULT_LIGHT	故障灯是否点亮
	root["FAULT_LIGHT"] = TCharToUTF8(L"N");
	//MODEL_REQUIREMENTS	车型OBD要求*-1:OBDⅡ,5:NO OBD,6:EOBD,29:CN-OBD-6
	root["MODEL_REQUIREMENTS"] = TCharToUTF8(L"1");
	//READY_STATE_UNDONE	就绪状态未完成项
	root["READY_STATE_UNDONE"] = TCharToUTF8(L"0");
	//RESULT	检查结果-0-不合格、1-合格
	root["RESULT"] = TCharToUTF8(L"1");
	//TERMINAL_COMMUNICATION_NORMAL	远程排放管理车载终端通信是否正常
	root["TERMINAL_COMMUNICATION_NORMAL"] = TCharToUTF8(L"Y");
	//TOTAL_O_DOMETER	累积行驶里程
	root["TOTAL_O_DOMETER"] = TCharToUTF8(L"0");
	//VIN	车辆识别代号
	root["VIN"] = TCharToUTF8(sResultData.strVIN.c_str());


	std::wstring wstrRet;
	int nRet = Cutl(strURL.GetString(), root, L"ODS_A7_OBD_INSPECTION_INFO", wstrRet);

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"ODS_A7_OBD_INSPECTION_INFO", L"返回参数", wstrRet.c_str());
	// 联网成功
	if (nRet == 0)
	{
		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}
//OBD检查数据项（控制单元）	ODS_A8_CONTROL_LEVEL_CHECK_ITEM
bool CXZHttp_API::OBDItemControlA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{
	return true;
}
//OBD 检查数据项（故障码）	ODS_A9_ERROR_CODE_CHECK
bool CXZHttp_API::OBDItemErrorA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD检查数据项（汽油车未就绪项目）	ODS_A10_PETROL_CAR_UNREADY_PROJECT
bool CXZHttp_API::OBDItemPetrolCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD检查数据项（柴油车未就绪项目）	ODS_A11_DIESEL_CAR_UNREADY_PROJECT
bool CXZHttp_API::OBDItemDieselCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD检查数据项（IUPR）	ODS_A12_IUPR_CHECK_ITEM
bool CXZHttp_API::OBDItemIUPRA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//汽油车OBD检查过程数据项（实时数据）	ODS_A13_REALTIME_DATA_PETROL_CAR_CHECK
bool CXZHttp_API::OBDItemPetrolCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//柴油车OBD检查过程数据项（实时数据）	ODS_A14_REALTIME_DATA_DIESEL_CAR_CHECK
bool CXZHttp_API::OBDItemDieselCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD检查信息	ODS_B2_OBD_CHECK_INFO
bool CXZHttp_API::OBDItemInfoB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//检查控制单元信息	ODS_B3_CHECK_CONTROL_INFO
bool CXZHttp_API::OBDItemControlB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//检查故障码信息	ODS_B4_CHECK_FAULT_CODE
bool CXZHttp_API::OBDItemErrorB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
