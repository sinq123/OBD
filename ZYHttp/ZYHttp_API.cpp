#include "stdafx.h"
#include "ZYHttp_API.h"

#include "..\JsonnLib\JsonnLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\JsonnLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\JsonnLib.lib")
#endif

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


CZYHttp_PAI::CZYHttp_PAI(void)
{
	// ģ������
	m_strModName = L"ZYHttp";

	// ���������ļ�
	LoadConfig();

	// ������־�ļ�
	GenLogFile();

	// ��ʼ������
	InitData();
}

CZYHttp_PAI::~CZYHttp_PAI(void)
{
}

CZYHttp_PAI& CZYHttp_PAI::GetInstance(void)
{
	static std::auto_ptr<CZYHttp_PAI> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CZYHttp_PAI());
	}
	return *(auto_ptr_instance.get());
}



bool CZYHttp_PAI::SetGasConstLoad(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����ػ����ļ�INI����";
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

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���50km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-mm-dd hh24:mi:ss)
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	if (otd1 > otd2)
	{
		root["data"]["jckssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	else
	{
		root["data"]["jckssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	//6	jbgl	��������	String	�ɿ�	��λ��kg
	strTemp = si.GetString(L"LowerRangeLoadConst", L"DIW", L"0");
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());
	//7	ihp5025	IHP5025���ع���	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"PowerStandardValue", L"8")));
	root["data"]["ihp5025"] = TCharToUTF8(strTemp.GetString());
	//8	fjssgl25	25km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"InternalDamage", L"0.2")));
	root["data"]["fjssgl25"] = TCharToUTF8(strTemp.GetString());
	//9	sjhxsj25	35-15km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"ACDT", L"")));
	root["data"]["sjhxsj25"] = TCharToUTF8(strTemp.GetString());
	//10	myhxsj25	35-15km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"CCDT", L"")));
	root["data"]["myhxsj25"] = TCharToUTF8(strTemp.GetString());
	//11	hxjcjg25	35-15km/h���м����	String	�ɿ�	1-�ϸ�2-���ϸ�
	strTemp.Format(L"%s", L"1");
	root["data"]["hxjcjg25"] = TCharToUTF8(strTemp.GetString());
	//12	ihp2540	IHP2540���ع���	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"HigherRangeLoadConst", L"PowerStandardValue", L"10")));
	root["data"]["ihp2540"] = TCharToUTF8(strTemp.GetString());
	//13	fjssgl40	40km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"HigherRangeLoadConst", L"InternalDamage", L"0.2")));
	root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
	//14	sjhxsj40	50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"HigherRangeLoadConst", L"ACDT", L"")));
	root["data"]["sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//15	myhxsj40	50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"HigherRangeLoadConst", L"CCDT", L"")));
	root["data"]["myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//16	hxjcjg40	50-30km/h���м����	String	�ɿ�	1-�ϸ�2-���ϸ�
	strTemp.Format(L"%s", L"1");
	root["data"]["hxjcjg40"] = TCharToUTF8(strTemp.GetString());
	//17	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	strTemp.Format(L"%s", L"1");
	root["data"]["jcpdjg"] = TCharToUTF8(strTemp.GetString());
	//18	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	strTemp.Format(L"%s", L"");
	root["data"]["jcbtgyy"] = TCharToUTF8(strTemp.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y43��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y43", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y43��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetGasPLHP(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���50km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-mm-dd hh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ(yyyy-mm-dd hh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jcjssj"] = TCharToUTF8(strTemp.GetString());
	//6	jbgl	��������	String	�ɿ�	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());

	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");

		if ((strTemp.Find(L"20") != -1)
			|| (strTemp.Find(L"15") != -1))
		{

			//7	fjssgl25	25km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl25"] = TCharToUTF8(strTemp.GetString());
			//8	sjhxsj25	35-15km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj25"] = TCharToUTF8(strTemp.GetString());
			//9	fjssjcjg25	25km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg25"] = TCharToUTF8(L"1");
		}
		if (strTemp.Find(L"30") != -1)
		{
			//10	fjssgl40	40km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С���������λ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
			//11	sjhxsj40	50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj40"] = TCharToUTF8(strTemp.GetString());
			//12	fjssjcjg40	40km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg40"] = TCharToUTF8(L"1");
		}
	}

	//13	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//14	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y44��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y44", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y44��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetDieConstLoad(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����ػ����ļ�INI����";
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

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���100km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-mm-ddhh24:mi:ss)
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ(yyyy-mm-ddhh24:mi:ss)
	if (otd1 > otd2)
	{
		root["data"]["jckssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	else
	{
		root["data"]["jckssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	//6	jbgl	��������	String	�ɿ�	��λΪkg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"LowerRangeLoadConst", L"DIW", L"0")));
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());
	//7	ihp30sjhxsj90	IHP30kw 100-80km/hʵ�ʻ���ʱ��	String	�ɿ� ��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT300", L"0")));
	root["data"]["ihp30sjhxsj90"] = TCharToUTF8(strTemp.GetString());
	//8	ihp30myhxsj90	IHP30kw 100-80km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT300", L"0")));
	root["data"]["ihp30myhxsj90"] = TCharToUTF8(strTemp.GetString());
	//9	ihp30jcpdjg90	IHP30kw 100-80km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg90"] = TCharToUTF8(L"1");
	//10	ihp30sjhxsj80	IHP30kw 90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT301", L"0")));
	root["data"]["ihp30sjhxsj80"] = TCharToUTF8(strTemp.GetString());
	//11	ihp30myhxsj80	IHP30kw 90-70km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT301", L"0")));
	root["data"]["ihp30myhxsj80"] = TCharToUTF8(strTemp.GetString());
	//12	ihp30jcpdjg80	IHP30kw 90-70km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg80"] = TCharToUTF8(L"1");
	//13	ihp30sjhxsj70	IHP30kw 80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT302", L"0")));
	root["data"]["ihp30sjhxsj70"] = TCharToUTF8(strTemp.GetString());
	//14	ihp30myhxsj70	IHP30kw 80-60km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT302", L"0")));
	root["data"]["ihp30myhxsj70"] = TCharToUTF8(strTemp.GetString());
	//15	ihp30jcpdjg70	IHP30kw 80-60km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg70"] = TCharToUTF8(L"1");
	//16	ihp30sjhxsj60	IHP30kw 70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT303", L"0")));
	root["data"]["ihp30sjhxsj60"] = TCharToUTF8(strTemp.GetString());
	//17	ihp30myhxsj60	IHP30kw 70-50km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT303", L"0")));
	root["data"]["ihp30myhxsj60"] = TCharToUTF8(strTemp.GetString());
	//18	ihp30jcpdjg60	IHP30kw 70-50km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg60"] = TCharToUTF8(L"1");
	//19	ihp30sjhxsj50	IHP30kw 60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT304", L"0")));
	root["data"]["ihp30sjhxsj50"] = TCharToUTF8(strTemp.GetString());
	//20	ihp30myhxsj50	IHP30kw 60-40km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT304", L"0")));
	root["data"]["ihp30myhxsj50"] = TCharToUTF8(strTemp.GetString());
	//21	ihp30jcpdjg50	IHP30kw 60-40km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg50"] = TCharToUTF8(L"1");
	//22	ihp30sjhxsj40	IHP30kw 50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT305", L"0")));
	root["data"]["ihp30sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//23	ihp30myhxsj40	IHP30kw 50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT305", L"0")));
	root["data"]["ihp30myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//24	ihp30jcpdjg40	IHP30kw 50-30km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg40"] = TCharToUTF8(L"1");
	//25	ihp30sjhxsj30	IHP30kw 40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT306", L"0")));
	root["data"]["ihp30sjhxsj30"] = TCharToUTF8(strTemp.GetString());
	//26	ihp30myhxsj30	IHP30kw 40-20km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT306", L"0")));
	root["data"]["ihp30myhxsj30"] = TCharToUTF8(strTemp.GetString());
	//27	ihp30jcpdjg30	IHP30kw 40-20km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg30"] = TCharToUTF8(L"1");
	//28	ihp30sjhxsj20	IHP30kw 30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT307", L"0")));
	root["data"]["ihp30sjhxsj20"] = TCharToUTF8(strTemp.GetString());
	//29	ihp30myhxsj20	IHP30kw 30-10km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT307", L"0")));
	root["data"]["ihp30myhxsj20"] = TCharToUTF8(strTemp.GetString());
	//30	ihp30jcpdjg20	IHP30kw 30-10km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg20"] = TCharToUTF8(L"1");
	//31	ihp20sjhxsj90	IHP20kw 100-80km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT200", L"0")));
	root["data"]["ihp20sjhxsj90"] = TCharToUTF8(strTemp.GetString());
	//32	ihp20myhxsj90	IHP20kw 100-80km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT200", L"0")));
	root["data"]["ihp20myhxsj90"] = TCharToUTF8(strTemp.GetString());
	//33	Ihp20jcpdjg90	IHP20kw 100-80km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg90"] = TCharToUTF8(L"1");
	//34	ihp20sjhxsj80	IHP20kw 90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT201", L"0")));
	root["data"]["ihp20sjhxsj80"] = TCharToUTF8(strTemp.GetString());
	//35	ihp20myhxsj80	IHP20kw 90-70km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT201", L"0")));
	root["data"]["ihp20myhxsj80"] = TCharToUTF8(strTemp.GetString());
	//36	Ihp20jcpdjg80	IHP20kw 90-70km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg80"] = TCharToUTF8(L"1");
	//37	ihp20sjhxsj70	IHP20kw 80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT202", L"0")));
	root["data"]["ihp20sjhxsj70"] = TCharToUTF8(strTemp.GetString());
	//38	ihp20myhxsj70	IHP20kw 80-60km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT202", L"0")));
	root["data"]["ihp20myhxsj70"] = TCharToUTF8(strTemp.GetString());
	//39	Ihp20jcpdjg70	IHP20kw 80-60km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg70"] = TCharToUTF8(L"1");
	//40	ihp20sjhxsj60	IHP20kw 70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT203", L"0")));
	root["data"]["ihp20sjhxsj60"] = TCharToUTF8(strTemp.GetString());
	//41	ihp20myhxsj60	IHP20kw 70-50km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT203", L"0")));
	root["data"]["ihp20myhxsj60"] = TCharToUTF8(strTemp.GetString());
	//42	Ihp20jcpdjg60	IHP20kw 70-50km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg60"] = TCharToUTF8(L"1");
	//43	ihp20sjhxsj50	IHP20kw 60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT204", L"0")));
	root["data"]["ihp20sjhxsj50"] = TCharToUTF8(strTemp.GetString());
	//44	ihp20myhxsj50	IHP20kw 60-40km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT204", L"0")));
	root["data"]["ihp20myhxsj50"] = TCharToUTF8(strTemp.GetString());
	//45	Ihp20jcpdjg50	IHP20kw 60-40km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg50"] = TCharToUTF8(L"1");
	//46	ihp20sjhxsj40	IHP20kw 50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT205", L"0")));
	root["data"]["ihp20sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//47	ihp20myhxsj40	IHP20kw 50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT205", L"0")));
	root["data"]["ihp20myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//48	Ihp20cpdjg40	IHP20kw 50-30km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	// Э����� Ӧ��ΪIhp20jcpdjg40
	root["data"]["Ihp20jcpdjg40"] = TCharToUTF8(L"1");
	//49	ihp20sjhxsj30	IHP20kw 40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT206", L"0")));
	root["data"]["ihp20sjhxsj30"] = TCharToUTF8(strTemp.GetString());
	//50	ihp20myhxsj30	IHP20kw 40-20km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT206", L"0")));
	root["data"]["ihp20myhxsj30"] = TCharToUTF8(strTemp.GetString());
	//51	Ihp20jcpdjg30	IHP20kw 40-20km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg30"] = TCharToUTF8(L"1");
	//52	ihp20sjhxsj20	IHP20kw 30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT207", L"0")));
	root["data"]["ihp20sjhxsj20"] = TCharToUTF8(strTemp.GetString());
	//53	ihp20myhxsj20	IHP20kw 30-10km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT207", L"0")));
	root["data"]["ihp20myhxsj20"] = TCharToUTF8(strTemp.GetString());
	//54	Ihp20jcpdjg20	IHP20kw 30-10km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg20"] = TCharToUTF8(L"1");
	//55	ihp10sjhxsj90	IHP10kw 100-80km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT100", L"0")));
	root["data"]["ihp10sjhxsj90"] = TCharToUTF8(strTemp.GetString());
	//56	ihp10myhxsj90	IHP10kw 100-80km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT100", L"0")));
	root["data"]["ihp10myhxsj90"] = TCharToUTF8(strTemp.GetString());
	//57	ihp10jcpdjg90	IHP10kw 100-80km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg90"] = TCharToUTF8(L"1");
	//58	ihp10sjhxsj80	IHP10kw 90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT101", L"0")));
	root["data"]["ihp10sjhxsj80"] = TCharToUTF8(strTemp.GetString());
	//59	ihp10myhxsj80	IHP10kw 90-70km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT101", L"0")));
	root["data"]["ihp10myhxsj80"] = TCharToUTF8(strTemp.GetString());
	//60	ihp10jcpdjg80	IHP10kw 90-70km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg80"] = TCharToUTF8(L"1");
	//61	ihp10sjhxsj70	IHP10kw 80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT102", L"0")));
	root["data"]["ihp10sjhxsj70"] = TCharToUTF8(strTemp.GetString());
	//62	ihp10myhxsj70	IHP10kw 80-60km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT102", L"0")));
	root["data"]["ihp10myhxsj70"] = TCharToUTF8(strTemp.GetString());
	//63	ihp10jcpdjg70	IHP10kw 80-60km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg70"] = TCharToUTF8(L"1");
	//64	ihp10sjhxsj60	IHP10kw 70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT103", L"0")));
	root["data"]["ihp10sjhxsj60"] = TCharToUTF8(strTemp.GetString());
	//65	ihp10myhxsj60	IHP10kw 70-50km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT103", L"0")));
	root["data"]["ihp10myhxsj60"] = TCharToUTF8(strTemp.GetString());
	//66	ihp10jcpdjg60	IHP10kw 70-50km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg60"] = TCharToUTF8(L"1");
	//67	ihp10sjhxsj50	IHP10kw 60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT104", L"0")));
	root["data"]["ihp10sjhxsj50"] = TCharToUTF8(strTemp.GetString());
	//68	ihp10myhxsj50	IHP10kw 60-40km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT104", L"0")));
	root["data"]["ihp10myhxsj50"] = TCharToUTF8(strTemp.GetString());
	//69	ihp10jcpdjg50	IHP10kw 60-40km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg50"] = TCharToUTF8(L"1");
	//70	ihp10sjhxsj40	IHP10kw 50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT105", L"0")));
	root["data"]["ihp10sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//71	ihp10myhxsj40	IHP10kw 50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT105", L"0")));
	root["data"]["ihp10myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//72	ihp10jcpdjg40	IHP10kw 50-30km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg40"] = TCharToUTF8(L"1");
	//73	ihp10sjhxsj30	IHP10kw 40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT106", L"0")));
	root["data"]["ihp10sjhxsj30"] = TCharToUTF8(strTemp.GetString());
	//74	ihp10myhxsj30	IHP10kw 40-20km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT106", L"0")));
	root["data"]["ihp10myhxsj30"] = TCharToUTF8(strTemp.GetString());
	//75	ihp10jcpdjg30	IHP10kw 40-20km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg30"] = TCharToUTF8(L"1");
	//76	ihp10sjhxsj20	IHP10kw 30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT107", L"0")));
	root["data"]["ihp10sjhxsj20"] = TCharToUTF8(strTemp.GetString());
	//77	ihp10myhxsj20	IHP10kw 30-10km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT107", L"0")));
	root["data"]["ihp10myhxsj20"] = TCharToUTF8(strTemp.GetString());
	//78	ihp10jcpdjg20	IHP10kw 30-10km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg20"] = TCharToUTF8(L"1");
	//79	fjssgl90	90km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP0", L"0")));
	root["data"]["fjssgl90"] = TCharToUTF8(strTemp.GetString());
	//80	fjssgl80	80km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP1", L"0")));
	root["data"]["fjssgl80"] = TCharToUTF8(strTemp.GetString());
	//81	fjssgl70	70km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP2", L"0")));
	root["data"]["fjssgl70"] = TCharToUTF8(strTemp.GetString());
	//82	fjssgl60	60km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP3", L"0")));
	root["data"]["fjssgl60"] = TCharToUTF8(strTemp.GetString());
	//83	fjssgl50	50km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP4", L"0")));
	root["data"]["fjssgl50"] = TCharToUTF8(strTemp.GetString());
	//84	fjssgl40	40km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP5", L"0")));
	root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
	//85	fjssgl30	30km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP6", L"0")));
	root["data"]["fjssgl30"] = TCharToUTF8(strTemp.GetString());
	//86	fjssgl20	20km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP7", L"0")));
	root["data"]["fjssgl20"] = TCharToUTF8(strTemp.GetString());
	//87	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//88	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y45��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y45", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y45��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetDiePLHP(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	//String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���100km/h��ʼ��ʱ�䣬ʱ���ʽ	(yyyy-mm-ddhh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	�ɿ�	ʱ���ʽ	(yyyy-mm-ddhh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jcjssj"] = TCharToUTF8(strTemp.GetString());
	//6	jbgl	��������	String	�ɿ�	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());

	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");

		if (strTemp.Find(L"80") != -1)
		{
			//7	fjssgl90	90km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl90"] = TCharToUTF8(strTemp.GetString());
			//8	sjhxsj90	100-80km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj90"] = TCharToUTF8(strTemp.GetString());
			//9	fjssjcjg90	90km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg90"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"70") != -1)
		{
			//10	fjssgl80	80km/h������ʧ����	String	�ɿ�	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl80"] = TCharToUTF8(strTemp.GetString());
			//11	sjhxsj80	90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj80"] = TCharToUTF8(strTemp.GetString());
			//12	fjssjcjg80	80km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg80"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"60") != -1)
		{
			//13	fjssgl70	70km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl70"] = TCharToUTF8(strTemp.GetString());
			//14	sjhxsj70	80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��msС���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj70"] = TCharToUTF8(strTemp.GetString());
			//15	fjssjcjg70	70km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg70"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"50") != -1)
		{
			//16	fjssgl60	60km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl60"] = TCharToUTF8(strTemp.GetString());
			//17	sjhxsj60	70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj60"] = TCharToUTF8(strTemp.GetString());
			//18	fjssjcjg60	60km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg60"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"40") != -1)
		{
			//19	fjssgl50	50km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl50"] = TCharToUTF8(strTemp.GetString());
			//20	sjhxsj50	60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj50"] = TCharToUTF8(strTemp.GetString());
			//21	fjssjcjg50	50km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg50"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"30") != -1)
		{
			//22	fjssgl40	40km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
			//23	sjhxsj40	50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj40"] = TCharToUTF8(strTemp.GetString());
			//24	fjssjcjg40	40km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg40"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"20") != -1)
		{
			//25	fjssgl30	30km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl30"] = TCharToUTF8(strTemp.GetString());
			//26	sjhxsj30	40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj30"] = TCharToUTF8(strTemp.GetString());
			//27	fjssjcjg30	30km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg30"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"10") != -1)
		{
			//28	fjssgl20	20km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl20"] = TCharToUTF8(strTemp.GetString());
			//29	sjhxsj20	30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj20"] = TCharToUTF8(strTemp.GetString());
			//30	fjssjcjg20	20km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg20"] = TCharToUTF8(L"1");
		}
	}
	//31	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//32	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y46��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y46", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y46��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetGasCheck(const CStringW& strjclx, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ͨ����ʼ��ʱ���ʽ(yyyy-mm-dd hh24:mm:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ (yyyy-mm-ddhh24:mi:ss)
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	jclx	�������	String	���ɿ�	1-�����顢2-�����
	root["data"]["jclx"] = TCharToUTF8(strjclx);
	//7	qtlx	��������	String	���ɿ�	1-��Ũ�ȡ�2-�е�Ũ�ȡ�3-�и�Ũ�ȡ�4-��Ũ�ȡ�5-���
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
	root["data"]["qtlx"] = TCharToUTF8(strTemp.GetString());
	//8	c3h8	��׼��C3H8Ũ��	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0")));
	root["data"]["c3h8"] = TCharToUTF8(strTemp.GetString());
	//9	bzco	��׼��COŨ��	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00")));
	root["data"]["bzco"] = TCharToUTF8(strTemp.GetString());
	root["data"]["co"] = TCharToUTF8(strTemp.GetString());
	//10	bzco2	��׼��CO2Ũ��	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	root["data"]["bzco2"] = TCharToUTF8(strTemp.GetString());
	root["data"]["co2"] = TCharToUTF8(strTemp.GetString());
	//11	bznox	��׼��NOxŨ��	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	root["data"]["bznox"] = TCharToUTF8(strTemp.GetString());
	root["data"]["nox"] = TCharToUTF8(strTemp.GetString());
	//12	bzo2	��׼��O2Ũ��	String	�ɿ�	��λ��%	С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00")));
	root["data"]["bzo2"] = TCharToUTF8(strTemp.GetString());
	root["data"]["o2"] = TCharToUTF8(strTemp.GetString());
	//13	hcjcjg	HC�����ֵ	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0")));
	root["data"]["hcjcjg"] = TCharToUTF8(strTemp.GetString());
	//14	cojcjg	CO�����ֵ	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00")));
	root["data"]["cojcjg"] = TCharToUTF8(strTemp.GetString());
	//15	co2jcjg	CO2�����ֵ	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	root["data"]["co2jcjg"] = TCharToUTF8(strTemp.GetString());
	//16	noxjcjg	NOx�����ֵ	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	root["data"]["noxjcjg"] = TCharToUTF8(strTemp.GetString());
	//17	o2jcjg	O2�����ֵ	String	�ɿ�	��λ��%С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00")));
	root["data"]["o2jcjg"] = TCharToUTF8(strTemp.GetString());
	//18	pef	PEFֵ	String	���ɿ�	С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3")));
	root["data"]["pef"] = TCharToUTF8(strTemp.GetString());
	//19	hcjcpdjg	HC����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["hcjcpdjg"] = TCharToUTF8(L"1");
	//20	cojcpdjg	CO����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["cojcpdjg"] = TCharToUTF8(L"1");
	//21	co2jcpdjg	CO2����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["co2jcpdjg"] = TCharToUTF8(L"1");
	//22	noxjcpdjg	NOx����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["noxjcpdjg"] = TCharToUTF8(L"1");
	//23	o2jcpdjg	O2����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�	ע������������Ϊ5-���ʱ������ɿ�
	root["data"]["o2jcpdjg"] = TCharToUTF8(L"1");
	//24	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//25	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y47��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y47", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y47��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetDieCheck(const CStringW& strjclx, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	// �Ƿ������ж� 1.���� 0.������
	CStringW strJudgeMent[3];
	// CO2
	strJudgeMent[0] = si.GetString(L"ResultOfAnaGasChk", L"SkipCO2Judgement", L"0");
	// NO
	strJudgeMent[1] = si.GetString(L"ResultOfAnaGasChk", L"SkipNOJudgement", L"0");
	// NO2
	strJudgeMent[2] = si.GetString(L"ResultOfAnaGasChk", L"SkipNO2Judgement", L"0");


	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ͨ����ʼ��ʱ���ʽ(yyyy-mm-ddhh24:mi:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ	(yyyy-mm-ddhh24:mi:ss)
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	jclx	�������	String	���ɿ�	1-�����顢2-�����
	root["data"]["jclx"] = TCharToUTF8(strjclx);
	//7	qtlx	��������	String	���ɿ�	1-��Ũ�ȡ�2-�е�Ũ�ȡ�3-�и�Ũ�ȡ�4-��Ũ�ȡ�5-���
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
	root["data"]["qtlx"] = TCharToUTF8(strTemp.GetString());
	//8	co2	��׼��CO2Ũ��	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	if (strJudgeMent[0] == L"1")
	{
		root["data"]["co2"] = TCharToUTF8(L"");
		root["data"]["bzco2"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["co2"] = TCharToUTF8(strTemp.GetString());
		root["data"]["bzco2"] = TCharToUTF8(strTemp.GetString());
	}
	//9	no	��׼��NOŨ��	String	���ɿ�	��λ��10-6����
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	if (strJudgeMent[1] == L"1")
	{
		root["data"]["no"] = TCharToUTF8(L"");
		root["data"]["no2"] = TCharToUTF8(L"");
		root["data"]["bzno"] = TCharToUTF8(L"");
		root["data"]["bzno2"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["no"] = TCharToUTF8(strTemp.GetString());
		root["data"]["no2"] = TCharToUTF8(strTemp.GetString());
		root["data"]["bzno"] = TCharToUTF8(strTemp.GetString());
		root["data"]["bzno2"] = TCharToUTF8(strTemp.GetString());
	}
	//10	no2	��׼��NO2Ũ��	String	���ɿ�	��λ��10-6������
	//strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0")));
	//if (strJudgeMent[2] == L"1")
	//{
	//	root["data"]["no2"] = TCharToUTF8(L"");
	//}
	//else
	//{
	//	if (_wtoi(strTemp) > 1)
	//	{
	//		root["data"]["no2"] = TCharToUTF8(strTemp.GetString());
	//	}
	//	else
	//	{
	//		strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	//		root["data"]["no2"] = TCharToUTF8(strTemp.GetString());
	//	}
	//}
	//11	o2	��׼��O2Ũ��	String	�ɿ�	��λ��%С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"")));
	root["data"]["o2"] = TCharToUTF8(strTemp.GetString());
	//12	co2jcjg	CO2�����ֵ	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	if (strJudgeMent[0] == L"1")
	{
		root["data"]["co2jcjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["co2jcjg"] = TCharToUTF8(strTemp.GetString());
	}
	//13	nojcjg	NO�����ֵ	String	���ɿ�	��λ��10-6����
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	if (strJudgeMent[1] == L"1")
	{
		root["data"]["nojcjg"] = TCharToUTF8(L"");
		root["data"]["no2jcjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["nojcjg"] = TCharToUTF8(strTemp.GetString());
		root["data"]["no2jcjg"] = TCharToUTF8(strTemp.GetString());
	}
	//14	no2jcjg	NO2�����ֵ	String	���ɿ�	��λ��10-6����
	//strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"0")));
	//if (strJudgeMent[2] == L"1")
	//{
	//	root["data"]["no2jcjg"] = TCharToUTF8(L"");
	//}
	//else
	//{
	//	root["data"]["no2jcjg"] = TCharToUTF8(strTemp.GetString());
	//}
	//15	o2jcjg	O2�����ֵ	String	�ɿ�	��λ��%	С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"")));
	root["data"]["o2jcjg"] = TCharToUTF8(strTemp.GetString());
	//16	co2jcpdjg	CO2����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	if (strJudgeMent[0] == L"1")
	{
		root["data"]["co2jcpdjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["co2jcpdjg"] = TCharToUTF8(L"1");
	}
	//17	nojcpdjg	NO����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	if (strJudgeMent[1] == L"1")
	{
		root["data"]["nojcpdjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["nojcpdjg"] = TCharToUTF8(L"1");
	}
	//18	no2jcpdjg	NO2����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	if (strJudgeMent[2] == L"1")
	{
		root["data"]["no2jcpdjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["no2jcpdjg"] = TCharToUTF8(L"1");
	}
	//19	o2jcpdjg	O2����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�	ע������������Ϊ5-���ʱ������ɿ�
	root["data"]["o2jcpdjg"] = TCharToUTF8(L"");
	//20	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//21	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y48��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y48", root, wstrRet);

#ifdef _DEBUG
	nRet = 0;
	wstrRet = L"{\"code\":200,\"msg\":\"�����ɹ�\"}";
#endif

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y48��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetLeakCheck(const CStringW& sblx, const CStringW& jyy, const CStringW& jyysfzh,SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	root["data"]["jyy"] = TCharToUTF8(jyy.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	root["data"]["jyysfzh"] = TCharToUTF8(jyysfzh.GetString());
	//4	sblx	�豸����	String	���ɿ�	H02-����������	H03-������������� 
	root["data"]["sblx"] = TCharToUTF8(sblx.GetString());
	//5	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���Ǵ�ͨ������ʼ��ʱ���ʽ	(yyyy-mm-dd hh24:mi:ss)
	root["data"]["jckssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ	(yyyy-mm-dd hh24:mi:ss)
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//7	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//8	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y49��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y49", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y49��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetOpaCheck(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOpaCalChk.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"��͸�����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	strTemp = si.GetString(L"ResultOfOpaCalChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	bz30	30%�˹�Ƭ��ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue1", L"0.00")));
	root["data"]["bz30"] = TCharToUTF8(strTemp.GetString());
	//7	clz30	30%�˹�Ƭ����ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue1", L"0.00")));
	root["data"]["clz30"] = TCharToUTF8(strTemp.GetString());
	//8	jcjg30	30%�˹�Ƭ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcjg30"] = TCharToUTF8(L"1");
	//9	bz50	50%�˹�Ƭ��ֵ	String	�ɿ�	�˹�Ƭ2�����б�ֵ����
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue2", L"0.00")));
	root["data"]["bz50"] = TCharToUTF8(strTemp.GetString());
	//10	clz50	50%�˹�Ƭ����ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue2", L"0.00")));
	root["data"]["clz50"] = TCharToUTF8(strTemp.GetString());
	//11	jcjg50	50%�˹�Ƭ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcjg50"] = TCharToUTF8(L"1");
	//12	bz70	70%�˹�Ƭ��ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue3", L"0.00")));
	root["data"]["bz70"] = TCharToUTF8(strTemp.GetString());
	//13	clz70	70%�˹�Ƭ����ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue3", L"0.00")));
	root["data"]["clz70"] = TCharToUTF8(strTemp.GetString());
	//14	jcjg70	70%�˹�Ƭ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcjg70"] = TCharToUTF8(L"1");
	//15	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//16	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y51��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y51", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y51��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetParaCheck(const SEnvPara& sEnvPara, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"UpResultOfEnvParamsINI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = sEnvPara.JYY.c_str();
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = sEnvPara.JYYSFZH.c_str();
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jckssj"] = TCharToUTF8(sEnvPara.odtbegin.Format(L"%Y-%m-%d %H:%M:%S"));
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jcjssj"] = TCharToUTF8(sEnvPara.odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
	//6	txjcjg	ͨѶ�����	String	�ɿ�	1 �ɹ�	0 ʧ��
	root["data"]["txjcjg"] = TCharToUTF8(L"1");
	//7	hjwd	�����¶ȣ����϶ȣ�	String	�ɿ�	С�������һλ
	root["data"]["hjwd"] = TCharToUTF8(sEnvPara.hjwd.c_str());
	//8	scwd	ʵ���¶ȶ��������϶ȣ�	String	�ɿ�	С�������һλ
	root["data"]["scwd"] = TCharToUTF8(sEnvPara.scwd.c_str());
	//9	wdjcpdjg	�¶ȼ���ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["wdjcpdjg"] = TCharToUTF8(L"1");
	//10	hjsd	����ʪ�ȣ�%��	String	�ɿ�	С�������һλ
	root["data"]["hjsd"] = TCharToUTF8(sEnvPara.hjsd.c_str());
	//11	scsd	ʵ��ʪ�ȶ���	String	�ɿ�	С�������һλ
	root["data"]["scsd"] = TCharToUTF8(sEnvPara.scsd.c_str());
	//12	sdjcpdjg	ʪ�ȼ���ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["sdjcpdjg"] = TCharToUTF8(L"1");
	//13	hjdqy	��������ѹkPa	String	�ɿ�	С�������һλ
	root["data"]["hjdqy"] = TCharToUTF8(sEnvPara.hjdqy.c_str());
	//14	scdqy	ʵ�����ѹ����kPa	String	�ɿ�	С�������һλ
	root["data"]["scdqy"] = TCharToUTF8(sEnvPara.scdqy.c_str());
	//15	dqyjcpdjg	����ѹ����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["dqyjcpdjg"] = TCharToUTF8(L"1");
	//16	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//17	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y52��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y52", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y52��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetSpeCheck(const STachometer& sTachometer, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfTachometer.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"UpResultOfEnvParamsINI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(sTachometer.JYY.c_str());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = sTachometer.JYYSFZH.c_str();
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jckssj"] = TCharToUTF8(sTachometer.odtbegin.Format(L"%Y-%m-%d %H:%M:%S"));
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jcjssj"] = TCharToUTF8(sTachometer.odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
	//6	bzzs1	1000r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	root["data"]["bzzs1"] = TCharToUTF8(sTachometer.bzzs1.c_str());
	//7	sc1	1000r/minʵ�����	String	�ɿ�	С�������һλ
	root["data"]["sc1"] = TCharToUTF8(sTachometer.sc1.c_str());
	//8	pd1	1000r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd1"] = TCharToUTF8(L"1");
	//9	bzzs2	1500r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	root["data"]["bzzs2"] = TCharToUTF8(sTachometer.bzzs2.c_str());
	//10	sc2	1500r/minʵ�����	String	�ɿ�	С�������һλ
	root["data"]["sc2"] = TCharToUTF8(sTachometer.sc2.c_str());
	//11	pd2	1500r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd2"] = TCharToUTF8(L"1");
	//12	bzzs3	2000r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	root["data"]["bzzs3"] = TCharToUTF8(sTachometer.bzzs3.c_str());
	//13	sc3	2000r/minʵ�����	String	�ɿ�	С�������һλ
	root["data"]["sc3"] = TCharToUTF8(sTachometer.sc3.c_str());
	//14	pd3	2000r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd3"] = TCharToUTF8(L"1");
	//15	bzzs4	2500r/min��ѧת�ٱ����	String	�ɿ�	
	root["data"]["bzzs4"] = TCharToUTF8(sTachometer.bzzs4.c_str());
	//16	sc4	2500r/minʵ�����	String	�ɿ�	
	root["data"]["sc4"] = TCharToUTF8(sTachometer.sc4.c_str());
	//17	pd4	2500r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd4"] = TCharToUTF8(L"1");
	//18	bzzs5	3000r/min��ѧת�ٱ����	String	�ɿ�	
	root["data"]["bzzs5"] = TCharToUTF8(sTachometer.bzzs5.c_str());
	//19	sc5	3000r/minʵ�����	String	�ɿ�	
	root["data"]["sc5"] = TCharToUTF8(sTachometer.sc5.c_str());
	//20	pd5	3000r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd5"] = TCharToUTF8(L"1");
	//21	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//22	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y53��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y53", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y53��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			//���ؽ��code
			CString strCode;
			strCode.Format(L"%d", root["code"].asInt());
			sHBMsg.code = strCode.GetString();

			sHBMsg.msg = UTF8ToTChar(root["msg"].asString().c_str());

			sHBMsg.data = UTF8ToTChar(root["data"].asString().c_str());

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}


int CZYHttp_PAI::WriteCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet)
{

	root["jkid"] = Json::Value(TCharToUTF8(wstrJkid.c_str()));
	root["yhxlh"] = Json::Value(TCharToUTF8(wstryhxlh.c_str()));
	root["jyjgbh"] = Json::Value(TCharToUTF8(wstrjyjgbh.c_str()));

	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, wstrWURL.c_str(), wstrJkid.c_str(), wstrPostData.c_str());


	return CCurlHttp_API::CurlPost(wstrWURL, wstrPostData, wstrRet);
}

int CZYHttp_PAI::QueryCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet)
{

	root["jkid"] = Json::Value(TCharToUTF8(wstrJkid.c_str()));
	root["yhxlh"] = Json::Value(TCharToUTF8(wstryhxlh.c_str()));
	root["jyjgbh"] = Json::Value(TCharToUTF8(wstrjyjgbh.c_str()));

	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, wstrWURL.c_str(), wstrJkid.c_str(), wstrPostData.c_str());

	return CCurlHttp_API::CurlPost(wstrQURL, wstrPostData, wstrRet);
}

void CZYHttp_PAI::GenLogFile(void)
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

void CZYHttp_PAI::InitData(void)
{
}

void CZYHttp_PAI::LoadConfig(void)
{
	// ��ȡ�����ļ�
	wchar_t wchFilePath[MAX_PATH] = {L'\0'};
	CNHCommonAPI::GetHLDFilePath(L"Config", L"ZYConfig.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	// URLд���ַ
	wstrWURL = si.GetString(L"NetConfig", L"WURL", L"");
	// URL��ѯ��ַ
	wstrQURL = si.GetString(L"NetConfig", L"QURL", L"");
	// �������к�
	wstryhxlh = si.GetString(L"NetConfig", L"yhxlh", L"");
	// ����������
	wstrjyjgbh = si.GetString(L"NetConfig", L"jyjgbh", L"");
	// ����߱��
	wstrjcxdh = si.GetString(L"NetConfig", L"jcxdh", L"");

}