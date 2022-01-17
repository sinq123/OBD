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


//��ʱ�ı��ĺ��塰�����Ϊ�����롱
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

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT





CXZHttp_API::CXZHttp_API(void)
{
	// ģ������
	m_strModName = L"XZHttp";

	// ���������ļ�
	LoadConfig();

	// ������־�ļ�
	GenLogFile();

	// ��ʼ������
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
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetHLDFilePath(L"LOG", L"", wchLogFileFolderPath, true);

	// ɾ��lDays��ǰ�������죩����־�ļ�
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

	// ���ɽ�����־�ļ�·��
	// ��������־�ļ�
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
	// ��ȡ�����ļ�
	wchar_t wchFilePath[MAX_PATH] = {L'\0'};
	CNHCommonAPI::GetHLDFilePath(L"Config", L"XZConfig.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	// URL��ַ
	m_wstrURL = si.GetString(L"NetConfig", L"WURL", L"");
	// ����������
	m_wstrJCZBH = si.GetString(L"NetConfig", L"jyjgbh", L"");
	// �û���
	m_wstrUserName = si.GetString(L"NetConfig", L"WURL", L"");
	// ����
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
	// ���վ���
	root["TsNo"] = Json::Value(TCharToUTF8(m_wstrJCZBH.c_str()));
	// �ܳ�
	root["Token"] = Json::Value(TCharToUTF8(m_wstrPassWord.c_str()));
	// ��������
	root["serviceCode"] = Json::Value(TCharToUTF8(strServiceCode.c_str()));
	// ʱ��
	root["ReqTime"] = Json::Value(TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S")));
	// ����
	root["ReqBody"] = Json::Value(ReqBody);

	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, wstrURL.c_str(), strServiceCode.c_str(), wstrPostData.c_str());


	return CCurlHttp_API::CurlPost(wstrURL, wstrPostData, wstrRet);
}

// ����Token
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
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��ȡTokrn", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			m_wstrPassWord = JsonValueToCString(root["access_token"]).GetString();
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��ȡToken", L"���ز���", wstrRet.c_str());
		return false;
	}

	return true;
}

//OBD ��������Ϣ	ODS_A7_OBD_INSPECTION_INFO
bool CXZHttp_API::OBDItemInfoA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{
	bool bRet(false);

	CString strURL;

	Json::Value root;

	//COMMUNICATION_NORMAL	OBDͨ���Ƿ�ɹ�
	root["COMMUNICATION_NORMAL"] = TCharToUTF8(L"Y");
	//FAILURE_REASON	���ɹ�ԭ��-1�ӿ��� 2�Ҳ����ӿ� 3���Ӻ���ͨ��
	root["FAILURE_REASON"] = TCharToUTF8(L"");
	//FAULT_CODE_NUM	����������
	root["FAULT_CODE_NUM"] = TCharToUTF8(L"0");
	//FAULT_INDICATOR_NORMAL	OBD����ָʾ���Ƿ�����
	root["FAULT_INDICATOR_NORMAL"] = TCharToUTF8(L"Y");
	//FAULT_LIGHT	���ϵ��Ƿ����
	root["FAULT_LIGHT"] = TCharToUTF8(L"N");
	//MODEL_REQUIREMENTS	����OBDҪ��*-1:OBD��,5:NO OBD,6:EOBD,29:CN-OBD-6
	root["MODEL_REQUIREMENTS"] = TCharToUTF8(L"1");
	//READY_STATE_UNDONE	����״̬δ�����
	root["READY_STATE_UNDONE"] = TCharToUTF8(L"0");
	//RESULT	�����-0-���ϸ�1-�ϸ�
	root["RESULT"] = TCharToUTF8(L"1");
	//TERMINAL_COMMUNICATION_NORMAL	Զ���ŷŹ������ն�ͨ���Ƿ�����
	root["TERMINAL_COMMUNICATION_NORMAL"] = TCharToUTF8(L"Y");
	//TOTAL_O_DOMETER	�ۻ���ʻ���
	root["TOTAL_O_DOMETER"] = TCharToUTF8(L"0");
	//VIN	����ʶ�����
	root["VIN"] = TCharToUTF8(sResultData.strVIN.c_str());


	std::wstring wstrRet;
	int nRet = Cutl(strURL.GetString(), root, L"ODS_A7_OBD_INSPECTION_INFO", wstrRet);

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"ODS_A7_OBD_INSPECTION_INFO", L"���ز���", wstrRet.c_str());
	// �����ɹ�
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
//OBD�����������Ƶ�Ԫ��	ODS_A8_CONTROL_LEVEL_CHECK_ITEM
bool CXZHttp_API::OBDItemControlA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{
	return true;
}
//OBD �������������룩	ODS_A9_ERROR_CODE_CHECK
bool CXZHttp_API::OBDItemErrorA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD�����������ͳ�δ������Ŀ��	ODS_A10_PETROL_CAR_UNREADY_PROJECT
bool CXZHttp_API::OBDItemPetrolCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD�����������ͳ�δ������Ŀ��	ODS_A11_DIESEL_CAR_UNREADY_PROJECT
bool CXZHttp_API::OBDItemDieselCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD��������IUPR��	ODS_A12_IUPR_CHECK_ITEM
bool CXZHttp_API::OBDItemIUPRA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//���ͳ�OBD�����������ʵʱ���ݣ�	ODS_A13_REALTIME_DATA_PETROL_CAR_CHECK
bool CXZHttp_API::OBDItemPetrolCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//���ͳ�OBD�����������ʵʱ���ݣ�	ODS_A14_REALTIME_DATA_DIESEL_CAR_CHECK
bool CXZHttp_API::OBDItemDieselCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//OBD�����Ϣ	ODS_B2_OBD_CHECK_INFO
bool CXZHttp_API::OBDItemInfoB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//�����Ƶ�Ԫ��Ϣ	ODS_B3_CHECK_CONTROL_INFO
bool CXZHttp_API::OBDItemControlB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
//����������Ϣ	ODS_B4_CHECK_FAULT_CODE
bool CXZHttp_API::OBDItemErrorB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{return true;
}
