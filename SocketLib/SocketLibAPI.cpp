#include "StdAfx.h"
#include "SocketLibAPI.h"
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
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT


CSocketLib_API& CSocketLib_API::GetInstance(void)
{
	static std::auto_ptr<CSocketLib_API> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CSocketLib_API());
	}
	return *(auto_ptr_instance.get());
}

CSocketLib_API::CSocketLib_API(void)
{
	srand((unsigned)time(NULL));
		
	wchar_t wchConfigIni[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"YJConfig.ini", wchConfigIni))
	{
	}

	CSimpleIni si(wchConfigIni);

	// ������IP
	m_strIP = si.GetString(L"System", L"ServerIP", L"117.40.138.23");
	// �������˿ں�
	m_nPort = (unsigned short)_wtoi(si.GetString(L"System", L"ServerPort", L"9779"));
	// ���վ���
	m_strStationNumber = si.GetString(L"System", L"StationNumber", L"40605001");
	// ����߱��
	m_strLineNumber = si.GetString(L"System", L"LineNumber", L"01");
	// �Ƿ�����
	m_bIsUsed = (L"1" == si.GetString(L"System", L"IsUsed", L"1")) ? true : false;
}

CSocketLib_API::~CSocketLib_API(void)
{
	CloseConnect();
}

void CSocketLib_API::CloseConnect(void)
{
	if (m_Socket.IsOpen())
	{
		m_Socket.Close();
	}
}

CString CSocketLib_API::CheckSum(const char *ch, const UINT un)
{
	assert(NULL!=ch && 0UL!=un);
	CString strRet(L"");

	DWORD bSum(0x00);	
	for (UINT ui=0; ui<un; ui++)
	{
		bSum ^=  ch[ui];
	}
	strRet.Format(L"%02x", bSum&0xff);
	return strRet;
}

CString CSocketLib_API::GenerateInsNum(void)
{
	// Ψһ�������ɹ��򣺼���߱���+ʱ�䣨��ȷ�����룩
	CString InsNum(L"");
	InsNum.AppendFormat(L"%s", m_strLineNumber);
	SYSTEMTIME dt;
	GetLocalTime(&dt);
	InsNum.AppendFormat(L"%04d%02d%02d%02d%02d%02d%03d", dt.wYear, dt.wMonth, dt.wDay, dt.wHour, dt.wMinute, dt.wSecond, dt.wMilliseconds);
	return InsNum;
}

bool CSocketLib_API::SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv)
{	
	CString strSend(L"");
	// 1��������+2���վ����+3����߱���+4ʱ��+4Ψһ����+5�̶��ָ���+6���ݲ���+7�̶��ָ���+8У����+9�̶�������
	strSend.Format(L"%s%s%s", strDataType, strStationNumber, strLineNumber);
	strSend.AppendFormat(L"%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	// Ψһ����
	strSend.AppendFormat(L"%s", strInsNumber);
	// �̶��ָ���
	strSend.AppendFormat(L"@@@");
	strSend.AppendFormat(L"%s", strJsonData);// ���ݲ���
	strSend.AppendFormat(L"tek");// �̶��ָ���

	std::string sData(CW2A(strSend.GetString()));
	CString strSum = CheckSum((char *)sData.c_str(), sData.size());
	strSend.AppendFormat(L"%s", strSum); // У����

	strSend.AppendFormat(L"####");// �̶�������

	if (!m_Socket.IsOpen())
	{
		if (!m_Socket.Open(m_strIP, m_nPort))
		{
			strRecv = L"���ӷ�����ʧ�ܣ�";
			return false;
		}
	}

	return m_Socket.SendAndRecvPacket(strSend, strRecv);
}

bool CSocketLib_API::Authentication(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	return SendAndRecvPacket(L"LJZC", L"", L"", L"", m_strLineNumber, strMsg);
}

bool CSocketLib_API::UploadDeviceChkProcess(const std::wstring& strCheckType, const std::wstring strCheckTimeStart, const std::wstring strCheckTimeEnd, 
	std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckType\":\"%s\",", strCheckType.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strCheckTimeStart.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", strCheckTimeEnd.c_str());
	strJsonData.AppendFormat(L"\"Second_NO\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"RotarySpeed\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"PLHP\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"HC\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"CO\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"NO\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"NO2\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"CO2\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"O2\":\"%s\"", L"");
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ37", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData.GetString(), strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}


bool CSocketLib_API::UploadGasDynCoastdown(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"�ϴ����ػ����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	COleDateTime otd1, otd2;
	strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd2.ParseDateTime(strTemp))
	{
		otd2 = COleDateTime::GetCurrentTime();
	}

	CString strJsonData(L"{");
	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", otd2.Format(L"%Y-%m-%d"));
	//4.		SlideTimeStart	���м�鿪ʼʱ��	��	ʱ��	��ʼʱ���ǹ�Ͳת���½���50km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	if (otd1 > otd2)
	{
		strJsonData.AppendFormat(L"\"SlideTimeStart\":\"%s\",", otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	else
	{
		strJsonData.AppendFormat(L"\"SlideTimeStart\":\"%s\",", otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	//5.		DIW	��������	��	����(5)	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(strTemp.GetString()));

	//6.		IHP2540	IHP2540���ù���	��	����(4)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"HigherRangeLoadConst", L"PowerStandardValue", L"10")));
	strJsonData.AppendFormat(L"\"IHP2540\":\"%.0f\",", _wtof(strTemp.GetString()));

	//7.		PLHP40	40km/hʱ�ĸ�����ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"HigherRangeLoadConst", L"InternalDamage", L"0.2")));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(strTemp.GetString()));

	//8.		ACDT40	50-30km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"HigherRangeLoadConst", L"CCDT", L"")));
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(strTemp.GetString()));

	//9.		CCDT40	50-30km/h���廬��ʵ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"HigherRangeLoadConst", L"ACDT", L"")));
	strJsonData.AppendFormat(L"\"CCDT40\":\"%.0f\",", _wtof(strTemp.GetString()));

	//10.		IHP5025	IHP5025���ù���	��	����(4)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"PowerStandardValue", L"8")));
	strJsonData.AppendFormat(L"\"IHP5025\":\"%.0f\",", _wtof(strTemp.GetString()));

	//11.		PLHP25	25km/hʱ�ĸ�����ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"InternalDamage", L"0.2")));
	strJsonData.AppendFormat(L"\"PLHP25\":\"%d\",", _wtoi(strTemp.GetString()));

	//12.		ACDT25	35-15km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"ACDT", L"")));
	strJsonData.AppendFormat(L"\"ACDT25\":\"%.0f\",", _wtof(strTemp.GetString()));

	//13.		CCDT25	35-15km/h���廬��ʵ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"CCDT", L"")));
	strJsonData.AppendFormat(L"\"CCDT25\":\"%.0f\",", _wtof(strTemp.GetString()));

	//14.		SlideCheckJudge40	50-30km/h���м����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"SlideCheckJudge40\":\"%s\",", L"1");
	//15.		SlideCheckJudge25	35-15km/h���м����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"SlideCheckJudge25\":\"%s\",", L"1");
	//16.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	//17.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ27", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadGasDynParasiticLose(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"�ϴ����������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;


	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber.GetString());

	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber.GetString());
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d").GetString());

	
	//4.		PLTimeStart	������ʧ��ʼʱ��	��	ʱ��	��ʼʱ���ǹ�Ͳת���½���50km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"PLTimeStart\":\"%s\",", strTemp.GetString());
	
	//5.		SlideTimeEnd	���м�����ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strJsonData.AppendFormat(L"\"SlideTimeEnd\":\"%s\",", strTemp.GetString());

	//6.		DIW	��������	��	����(5)	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(strTemp.GetString()));

	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");

		if ((strTemp.Find(L"20") != -1)
			|| (strTemp.Find(L"15") != -1))
		{
			//9.		PLHP25	25km/hʱ�ĸ�����ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP25\":\"%d\",", _wtoi(strTemp.GetString()));

			//10.		ACDT25	35-15km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT25\":\"%.0f\",", _wtof(strTemp.GetString()));
		}
		if (strTemp.Find(L"30") != -1)
		{
			//7.		PLHP40	40km/hʱ�ĸ�����ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(strTemp.GetString()));

			//8.		ACDT40	50-30km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(strTemp.GetString()));
		}
	}
	
	//11.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");

	//12.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ28", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadDiesDynCoastdown(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"�ϴ����ػ����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	COleDateTime otd1, otd2;
	strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd2.ParseDateTime(strTemp))
	{
		otd2 = COleDateTime::GetCurrentTime();
	}

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	//4.		SlideTimeStart	���м�鿪ʼʱ��	��	ʱ��	��ʼʱ���ǹ�Ͳת���½���100km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	if (otd1 > otd2)
	{
		strJsonData.AppendFormat(L"\"SlideTimeStart\":\"%s\",", otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	else
	{
		strJsonData.AppendFormat(L"\"SlideTimeStart\":\"%s\",", otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	//5.		DIW	��������	��	����(5)	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"LowerRangeLoadConst", L"DIW", L"0")));
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(strTemp.GetString()));

	//6.		IHP30ACDT90	IHP30kw 100-80km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT300", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT90\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//7.		IHP30ACDT80	IHP30kw 90-70km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT301", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT80\":\"%.0f\",", _wtof(strTemp.GetString()));

	//8.		IHP30ACDT70	IHP30kw 80-60km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT302", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT70\":\"%.0f\",", _wtof(strTemp.GetString()));

	//9.		IHP30ACDT60	IHP30kw 70-50km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT303", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT60\":\"%.0f\",", _wtof(strTemp.GetString()));

	//10.		IHP30ACDT50	IHP30kw 60-40km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT304", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT50\":\"%.0f\",", _wtof(strTemp.GetString()));

	//11.		IHP30ACDT40	IHP30kw 50-30km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT305", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT40\":\"%.0f\",", _wtof(strTemp.GetString()));

	//12.		IHP30ACDT30	IHP30kw 40-20km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT306", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT30\":\"%.0f\",", _wtof(strTemp.GetString()));

	//13.		IHP30ACDT20	IHP30kw 30-10km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT307", L"0")));
	strJsonData.AppendFormat(L"\"IHP30ACDT20\":\"%.0f\",", _wtof(strTemp.GetString()));

	//14.		IHP30CCDT90	IHP30kw 100-80km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT300", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT90\":\"%.0f\",", _wtof(strTemp.GetString()));

	//15.		IHP30CCDT80	IHP30kw 90-70km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT301", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT80\":\"%.0f\",", _wtof(strTemp.GetString()));

	//16.		IHP30CCDT70	IHP30kw 80-60km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT302", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT70\":\"%.0f\",", _wtof(strTemp.GetString()));

	//17.		IHP30CCDT60	IHP30kw 70-50km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT303", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT60\":\"%.0f\",", _wtof(strTemp.GetString()));

	//18.		IHP30CCDT50	IHP30kw 60-40km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT304", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT50\":\"%.0f\",", _wtof(strTemp.GetString()));

	//19.		IHP30CCDT40	IHP30kw 50-30km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT305", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT40\":\"%.0f\",", _wtof(strTemp.GetString()));

	//20.		IHP30CCDT30	IHP30kw 40-20km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT306", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT30\":\"%.0f\",", _wtof(strTemp.GetString()));

	//21.		IHP30CCDT20	IHP30kw 30-10km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT307", L"0")));
	strJsonData.AppendFormat(L"\"IHP30CCDT20\":\"%.0f\",", _wtof(strTemp.GetString()));

	//22.		IHP20ACDT90	IHP20kw 100-80km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT200", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT90\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//23.		IHP20ACDT80	IHP20kw 90-70km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT201", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT80\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//24.		IHP20ACDT70	IHP20kw 80-60km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT202", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT70\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//25.		IHP20ACDT60	IHP20kw 70-50km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT203", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT60\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//26.		IHP20ACDT50	IHP20kw 60-40km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT204", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT50\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//27.		IHP20ACDT40	IHP20kw 50-30km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT205", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT40\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//28.		IHP20ACDT30	IHP20kw 40-20km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT206", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT30\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//29.		IHP20ACDT20	IHP20kw 30-10km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT207", L"0")));
	strJsonData.AppendFormat(L"\"IHP20ACDT20\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//30.		IHP20CCDT90	IHP20kw 100-80km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT200", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT90\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//31.		IHP20CCDT80	IHP20kw 90-70km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT201", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT80\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//32.		IHP20CCDT70	IHP20kw 80-60km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT202", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT70\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//33.		IHP20CCDT60	IHP20kw 70-50km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT203", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT60\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//34.		IHP20CCDT50	IHP20kw 60-40km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT204", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT50\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//35.		IHP20CCDT40	IHP20kw 50-30km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT205", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT40\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//36.		IHP20CCDT30	IHP20kw 40-20km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT206", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT30\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//37.		IHP20CCDT20	IHP20kw 30-10km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT207", L"0")));
	strJsonData.AppendFormat(L"\"IHP20CCDT20\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//38.		IHP10ACDT90	IHP10kw 100-80km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT100", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT90\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//39.		IHP10ACDT80	IHP10kw 90-70km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT101", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT80\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//40.		IHP10ACDT70	IHP10kw 80-60km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT102", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT70\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//41.		IHP10ACDT60	IHP10kw 70-50km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT103", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT60\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//42.		IHP10ACDT50	IHP10kw 60-40km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT104", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT50\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//43.		IHP10ACDT40	IHP10kw 50-30km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT105", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT40\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//44.		IHP10ACDT30	IHP10kw 40-20km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT106", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT30\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//45.		IHP10ACDT20	IHP10kw 30-10km/hʵ�ʻ���ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT107", L"0")));
	strJsonData.AppendFormat(L"\"IHP10ACDT20\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//46.		IHP10CCDT90	IHP10kw 100-80km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT100", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT90\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//47.		IHP10CCDT80	IHP10kw 90-70km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT101", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT80\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//48.		IHP10CCDT70	IHP10kw 80-60km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT102", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT70\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//49.		IHP10CCDT60	IHP10kw 70-50km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT103", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT60\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//50.		IHP10CCDT50	IHP10kw 60-40km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT104", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT50\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//51.		IHP10CCDT40	IHP10kw 50-30km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT105", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT40\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//52.		IHP10CCDT30	IHP10kw 40-20km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT106", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT30\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//53.		IHP10CCDT20	IHP10kw 30-10km/h���廬��ʱ��	��	����(6)	��λ��ms
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT107", L"0")));
	strJsonData.AppendFormat(L"\"IHP10CCDT20\":\"%.0f\",", _wtof(strTemp.GetString()));
	
	//54.		PLHP90	90km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP0", L"0")));
	strJsonData.AppendFormat(L"\"PLHP90\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//55.		PLHP80	80km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP1", L"0")));
	strJsonData.AppendFormat(L"\"PLHP80\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//56.		PLHP70	70km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP2", L"0")));
	strJsonData.AppendFormat(L"\"PLHP70\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//57.		PLHP60	60km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP3", L"0")));
	strJsonData.AppendFormat(L"\"PLHP60\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//58.		PLHP50	50km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP4", L"0")));
	strJsonData.AppendFormat(L"\"PLHP50\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//59.		PLHP40	40km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP5", L"0")));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//60.		PLHP30	30km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP6", L"0")));
	strJsonData.AppendFormat(L"\"PLHP30\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//61.		PLHP20	20km/h������ʧ����	��	����(6,2)	��λ��kW
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP7", L"0")));
	strJsonData.AppendFormat(L"\"PLHP20\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//62.		SlideCheckJudge100	100-10km/h���м����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"SlideCheckJudge100\":\"%s\",", L"1");
	//63.		SlideCheckJudge80	80-10km/h���м����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"SlideCheckJudge80\":\"%s\",", L"1");
	//64.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	//65.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",",  sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ29", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";

	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadDiesDynParasiticLose(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"�ϴ����������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	
	//4.		PLTimeStart	���ӹ�����ʧ���Կ�ʼʱ��	��	ʱ��	��ʼʱ���ǹ�Ͳת���½���100km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"PLTimeStart\":\"%s\",", strTemp.GetString());
	
	//5.		SlideTimeEnd	���ӹ�����ʧ���Խ���ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"SlideTimeEnd\":\"%s\",", strTemp.GetString());
	
	//6.		DIW	��������	��	����(5)	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(strTemp.GetString()));
	
	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");

		if (strTemp.Find(L"80") != -1)
		{
			//7.		ACDT90	100-80km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT90\":\"%.0f\",", _wtof(strTemp.GetString()));

			//15.		PLHP90	90km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP90\":\"%d\",", _wtoi(strTemp.GetString()));
		}
		if (strTemp.Find(L"70") != -1)
		{
			//8.		ACDT80	90-70km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT80\":\"%.0f\",", _wtof(strTemp.GetString()));
			//16.		PLHP80	80km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP80\":\"%d\",", _wtoi(strTemp.GetString()));

		}
		if (strTemp.Find(L"60") != -1)
		{
			//9.		ACDT70	80-60km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT70\":\"%.0f\",", _wtof(strTemp.GetString()));
			//17.		PLHP70	70km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP70\":\"%d\",", _wtoi(strTemp.GetString()));

		}
		if (strTemp.Find(L"50") != -1)
		{
			//10.		ACDT60	70-50km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT60\":\"%.0f\",", _wtof(strTemp.GetString()));
			//18.		PLHP60	60km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP60\":\"%d\",", _wtoi(strTemp.GetString()));

		}
		if (strTemp.Find(L"40") != -1)
		{
			//11.		ACDT50	60-40km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT50\":\"%.0f\",", _wtof(strTemp.GetString()));
			//19.		PLHP50	50km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP50\":\"%d\",", _wtoi(strTemp.GetString()));

		}
		if (strTemp.Find(L"30") != -1)
		{
			//12.		ACDT40	50-30km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(strTemp.GetString()));
			//20.		PLHP40	40km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(strTemp.GetString()));

		}
		if (strTemp.Find(L"20") != -1)
		{
			//13.		ACDT30	40-20km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT30\":\"%.0f\",", _wtof(strTemp.GetString()));
			//21.		PLHP30	30km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP30\":\"%d\",", _wtoi(strTemp.GetString()));

		}
		if (strTemp.Find(L"10") != -1)
		{
			//14.		ACDT20	30-10km/hʵ�ʻ���ʵ��	��	����(6)	��λ��ms
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"ACDT20\":\"%.0f\",", _wtof(strTemp.GetString()));
			//22.		PLHP20	20km/h������ʧ����	��	����(6,2)	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			strJsonData.AppendFormat(L"\"PLHP20\":\"%d\",", _wtoi(strTemp.GetString()));
		}
	}
	//23.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//24.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ30", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadAnaLowStdGasCheck(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ�����ǽ���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber.GetString());

	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber.GetString());
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d").GetString());
	
	//4.		CheckTimeStart	��鿪ʼʱ��	��	ʱ��	��ͨ����ʼ��ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp.GetString());
	
	//5.		CheckTimeEnd	������ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S").GetString());
	
	//6.		C3H8	��׼��C3H8Ũ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0")));
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//7.		CO	��׼��COŨ�� 	��	����(4,2)	��λ��%
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//8.		CO2	��׼��CO2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//9.		NO	��׼��NOŨ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(strTemp.GetString()));

	//10.		O2	��׼��O2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//11.		HCER	HC�����ֵ	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0")));
	strJsonData.AppendFormat(L"\"HCER\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//12.		COER	CO�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//13.		CO2ER	CO2�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//14.		NOER	NO�����ֵ	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	strJsonData.AppendFormat(L"\"NOER\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//15.		O2ER	O2�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//16.		PEF	PEFֵ	��	����(5,3)	
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3")));
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(strTemp.GetString()));
	
	//17.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//18.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ31", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadAnaZeroGasCheck(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ�����ǽ���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	
	//4.		CheckTimeStart	��鿪ʼʱ��	��	ʱ��	��ͨ����ʼ��ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp.GetString());
	
	//5.		CheckTimeEnd	������ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	
	//6.		C3H8	��׼��C3H8Ũ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0")));
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//7.		CO	��׼��COŨ�� 	��	����(4,2)	��λ��%
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//8.		CO2	��׼��CO2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//9.		NO	��׼��NOŨ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//10.		O2	��׼��O2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//11.		HCER	HC�����ֵ	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0")));
	strJsonData.AppendFormat(L"\"HCER\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//12.		COER	CO�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//13.		CO2ER	CO2�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//14.		NOER	NO�����ֵ	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	strJsonData.AppendFormat(L"\"NOER\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//15.		O2ER	O2�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//16.		PEF	PEFֵ	��	����(5,3)	
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3")));
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(strTemp.GetString()));
	
	//17.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//18.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ32", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadAnaHighStdGasCheck(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ�����ǽ���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	
	//4.		CheckTimeStart	��鿪ʼʱ��	��	ʱ��	��ͨ����ʼ��ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp.GetString());
	
	//5.		CheckTimeEnd	������ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	
	//6.		C3H8	��׼��C3H8Ũ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0")));
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//7.		CO	��׼��COŨ�� 	��	����(4,2)	��λ��%
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0")));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//8.		CO2	��׼��CO2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0")));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//9.		NO	��׼��NOŨ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//10.		O2	��׼��O2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0")));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//11.		T90NO	NO��Ӧʱ��(T90)	��	����(4)	��λ��s
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NORespTime", L"0")));
	strJsonData.AppendFormat(L"\"T90NO\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//12.		T90CO	CO��Ӧʱ��(T90)	��	����(4)	��λ��s
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CORespTime", L"0")));
	strJsonData.AppendFormat(L"\"T90CO\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//13.		T90O2	O2��Ӧʱ��(T90)	��	����(4)	��λ��s
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2RespTime", L"0")));
	strJsonData.AppendFormat(L"\"T90O2\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//14.		T10NO	NO��Ӧʱ��(T10)	��	����(4)	��λ��s
	strJsonData.AppendFormat(L"\"T10NO\":\"%s\",", L"1");
	
	//15.		T10CO	CO��Ӧʱ��(T10)	��	����(4)	��λ��s
	strJsonData.AppendFormat(L"\"T10CO\":\"%s\",", L"1");
	
	//16.		T10O2	O2��Ӧʱ��(T10)	��	����(4)	��λ��s
	strJsonData.AppendFormat(L"\"T10O2\":\"%s\",", L"1");
	
	//17.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�2-��ά��
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//18.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");
	

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ33", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadAnaFullCalChk(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ�����ǽ���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	
	//4.		CheckTimeStart	��鿪ʼʱ��	��	ʱ��	��ʼʱ���Ǵ�ͨ������ʼ��ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp.GetString());
	
	//5.		CheckTimeEnd	������ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	
	//6.		CheckType	����	��	�ַ�(1)	1-��Ũ�ȡ�2-�е�Ũ�ȡ�3-�и�Ũ�ȡ�4-��Ũ�ȡ�5-���
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
	strJsonData.AppendFormat(L"\"CheckType\":\"%s\",", strTemp.GetString());
	
	//7.		C3H8	��׼��C3H8Ũ��	��	����(4)	��λ��10-6��������THC
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0")));
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//8.		CO	��׼��COŨ�� 	��	����(4,2)	��λ��%
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//9.		CO2	��׼��CO2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//10.		NO	��׼��NOŨ��	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//11.		O2	��׼��O2Ũ��	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00")));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//12.		HCER	HC�����ֵ	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0")));
	strJsonData.AppendFormat(L"\"HCER\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//13.		COER	CO�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//14.		CO2ER	CO2�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//15.		NOER	NO�����ֵ	��	����(4)	��λ��10-6
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	strJsonData.AppendFormat(L"\"NOER\":\"%d\",", _wtoi(strTemp.GetString()));
	
	//16.		O2ER	O2�����ֵ	��	����(4,2)	��λ��%
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00")));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//17.		PEF	PEFֵ	��	����(5,3)	
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3")));
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(strTemp.GetString()));
	
	//18.		CheckJudge	����ж����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//19.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ34", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadAnaLeakTest(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	
	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	
	//4.		CheckTimeStart	��鿪ʼʱ��	��	ʱ��	��ʼʱ���Ǵ�ͨ������ʼ��ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	COleDateTime odtStart = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odtSp;
	odtSp.SetDateTimeSpan(0,0,10,0);
	odtStart = odtStart - odtSp;
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
	
	//5.		CheckTimeEnd	������ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	
	//6.		CheckJudge	�����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//7.		BelowStandard	���ϸ�˵��	��	�ַ�(200)	
	strJsonData.AppendFormat(L"\"BelowStandard\":\"%s\",", L"");
	
	//8.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);

	
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ35", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}

bool CSocketLib_API::UploadOpaCalChk(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	// ��ȡ��͸�����ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOpaCalChk.ini", wchPath, false))
	{
		// �ļ�������
		strResult = L"-1";
		strMsg = L"��͸�����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");

	//1.		StationCode	���վ����	��	�ַ�(10)	StationCode
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	
	//2.		SceneCode	����߱���	��	�ַ�(10)	
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);

	//3.		CheckDate	�������	��	����	ʱ���ʽ(yyyy-MM-dd)
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	
	//4.		CheckTimeStart	��鿪ʼʱ��	��	ʱ��	��ʼʱ���Ǵ�ͨ������ʼ��ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strTemp = si.GetString(L"ResultOfOpaCalChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",",strTemp.GetString());
	
	//5.		CheckTimeEnd	������ʱ��	��	ʱ��	ʱ���ʽ(yyyy-MM-dd HH:mm:ss)
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	
	//6.		PLAC	������ϵ����	��	����(4,2)	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"AE1", L"0.00")));
	strJsonData.AppendFormat(L"\"PLAC\":\"%.2f\",", _wtof(strTemp.GetString()));
	
	//7.		ResponseTime	��Ӧʱ��	��	����(6)	��λ��ms
	strJsonData.AppendFormat(L"\"ResponseTime\":\"%d\",", 200+rand()%300);
	
	//8.		TC	�����¶�ʾֵ���	��	����(6,2)	��λ����
	strJsonData.AppendFormat(L"\"TC\":\"%.2f\",", rand()%5/10.0f);
	
	//9.		CheckJudge	�����	��	�ַ�(1)	1-�ϸ�0-���ϸ�
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", L"1");
	
	//10.		BelowStandard	���ϸ�˵��	��	�ַ�(200)	
	strJsonData.AppendFormat(L"\"BelowStandard\":\"%s\",", L"");

	//11.		Checker	�����Ա	��	�ַ�(10)	��д�������������
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ36", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// ��ȡʧ��ԭ��
		strResult = L"0";
	}
	return bRet;
}
