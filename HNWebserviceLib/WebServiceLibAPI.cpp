#include "StdAfx.h"
#include "WebServiceLibAPI.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "gsoap/soapDataInterfaceSoapProxy.h"
#include "gsoap/DataInterfaceSoap.nsmap"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT
// ���ÿ�
#include "afxwin.h"
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

//ȫ�ֱ���
//WebServiceLibAPI* g_pNWDD(NULL);

WebServiceLibAPI& WebServiceLibAPI::GetInstance(void)
{
	static std::auto_ptr<WebServiceLibAPI> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL)
	{
		auto_ptr_instance.reset(new WebServiceLibAPI());
	}
	return *(auto_ptr_instance.get());
}

WebServiceLibAPI::WebServiceLibAPI(void)
	:m_bUsed(false)
	, m_nState(0)
	, m_nOrder(-1)
	, m_bIsChkPassData(false)
{
	m_strName = L"";
	m_strPassWord = L"";

	wchar_t wchPath[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"Config", L"HNConfig.ini", wchPath);

	CSimpleIni sHN(wchPath);
	m_strURL = sHN.GetString(L"WebService",L"URL",L"http://61.145.229.213:6787/HNGKInterface_NH/DataInterface.asmx?wsdl");
	m_strIP = sHN.GetString(L"WebService",L"IP",L"192.168.1.1");
	m_strPort = sHN.GetString(L"WebService",L"Port",L"9002");
	m_nServerPort = _wtoi(sHN.GetString(L"WebService",L"ServerPort",L"1009"));
	// �Լ��Ƿ�����
	//m_bSelfCheckUsedVideo = _wtoi(sHN.GetString(L"WebService",L"SelfCheckIsUsedVideo",L"1")) == 1 ? true: false;
	m_bUsed = (_wtoi(sHN.GetString(L"WebService",L"IsUsed",L"1")) == 1)? true: false;

	m_nSendTime = _wtoi(sHN.GetString(L"WebService",L"SendTime",L"50"));
	m_bIsUsedSendState = _wtoi(sHN.GetString(L"WebService",L"IsUsedSendState",L"1")) == 1 ? true: false;

	m_bIsChkPassData = _wtoi(sHN.GetString(L"WebService",L"IsChkPassData",L"0")) == 1 ? true: false;
		
	m_sSocketNet.Connect(m_strIP, _wtoi(m_strPort));
	m_unTimerID = timeSetEvent(m_nSendTime, 1, (LPTIMECALLBACK)TimePro, (DWORD_PTR)this, TIME_PERIODIC);
}

WebServiceLibAPI::~WebServiceLibAPI(void)
{
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
		m_nState = 0;
		m_nOrder = -1;
	}
}
//���ú�������
CString WebServiceLibAPI::SetVehicleType(CString strVehicleType)
{
	if (!m_bUsed)
	{
		return L"";
	}
	CString strTemp(L"");
	if(0 == wcscmp(strVehicleType,L"��������"))
	{
		strTemp = L"01";
	}
	else if(0 == wcscmp(strVehicleType,L"С������"))
	{
		strTemp = L"02";
	}
	else if(0 == wcscmp(strVehicleType,L"ʹ������"))
	{
		strTemp = L"03";
	}
	else if(0 == wcscmp(strVehicleType,L"�������"))
	{
		strTemp = L"04";
	}
	else if(0 == wcscmp(strVehicleType,L"��������"))
	{
		strTemp = L"05";
	}
	else if(0 == wcscmp(strVehicleType,L"�⼮����"))
	{
		strTemp = L"06";
	}
	else if(0 == wcscmp(strVehicleType,L"��������Ħ�г�"))
	{
		strTemp = L"07";
	}
	else if(0 == wcscmp(strVehicleType,L"���Ħ�г�"))
	{
		strTemp = L"08";
	}
	else if(0 == wcscmp(strVehicleType,L"ʹ��Ħ�г�"))
	{
		strTemp = L"09";
	}
	else if(0 == wcscmp(strVehicleType,L"���Ħ�г�"))
	{
		strTemp = L"10";
	}
	else if(0 == wcscmp(strVehicleType,L"����Ħ�г�"))
	{
		strTemp = L"11";
	}
	else if(0 == wcscmp(strVehicleType,L"�⼮Ħ�г�"))
	{
		strTemp = L"12";
	}
	else if(0 == wcscmp(strVehicleType,L"ũ�����䳵"))
	{
		strTemp = L"13";
	}
	else if(0 == wcscmp(strVehicleType,L"������"))
	{
		strTemp = L"14";
	}
	else if(0 == wcscmp(strVehicleType,L"�ҳ�"))
	{
		strTemp = L"15";
	}
	else if(0 == wcscmp(strVehicleType,L"��������"))
	{
		strTemp = L"16";
	}
	else if(0 == wcscmp(strVehicleType,L"����Ħ�г�"))
	{
		strTemp = L"17";
	}
	else if(0 == wcscmp(strVehicleType,L"��������"))
	{
		strTemp = L"18";
	}
	else if(0 == wcscmp(strVehicleType,L"����Ħ�г�"))
	{
		strTemp = L"19";
	}
	else if(0 == wcscmp(strVehicleType,L"��ʱ�뾳����"))
	{
		strTemp = L"20";
	}
	else if(0 == wcscmp(strVehicleType,L"��ʱ�뾳Ħ�г�"))
	{
		strTemp = L"21";
	}
	else if(0 == wcscmp(strVehicleType,L"��ʱ��ʻ��"))
	{
		strTemp = L"22";
	}
	else if(0 == wcscmp(strVehicleType,L"��������"))
	{
		strTemp = L"23";
	}
	else if(0 == wcscmp(strVehicleType,L"����Ħ�г�"))
	{
		strTemp = L"24";
	}
	else if(0 == wcscmp(strVehicleType,L"��������Դ��������"))
	{
		strTemp = L"51";
	}
	else if(0 == wcscmp(strVehicleType,L"С������Դ��������"))
	{
		strTemp = L"52";
	}
	else if(0 == wcscmp(strVehicleType,L"����"))
	{
		strTemp = L"99";
	}
	return strTemp;
	//CNHLogAPI::WriteLog(L"������־",L"���ó�������",m_strVehicleType);
}

/**********************************�ŷż�ⲿ��****************************************/
//��ʼ��Ⲣ¼��BeginRecord
bool WebServiceLibAPI::StartDetAndVideo()
{
	if (!m_bUsed)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);
	CNHLogAPI::WriteLog(_T("������־"), _T("BeginRecord���͵�ַ��"),m_strURL);

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	if(reader.parse(str,root,true))
	{
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		LINEINFO sLineinfo;
		GetIniLineInfo(&sLineinfo);
		root["LineNo"] = TCharToANSI(sLineinfo.wchLineNumber);
		root["License"] = TCharToANSI(sTestLog.wchPlateNumber);
		CString strPlateType(L"");
		if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"1";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		root["LicenseType"] = TCharToANSI(strPlateType);
		root["LicenseCode"] = TCharToANSI(SetVehicleType(sTestLog.wchReserved1));//�� GA24.7
		CString strTestType(L"");
		// �ϻ���1:ASM,2:VMAS,3:LUGDOWN,4:DIS,5:FSUNHT,6:FSUYD,7:AVFSU,8:MI,9:MDIS
		// ƽ̨��1:DIS,2:ASM,3:VMAS,4:LUGDOWN,5:FSUNHT
		if(0 == wcscmp(L"4",sTestLog.wchTestType))
		{
			strTestType = L"1";
		}
		else if(0 == wcscmp(L"1",sTestLog.wchTestType))
		{
			strTestType = L"2";
		}
		else if(0 == wcscmp(L"2",sTestLog.wchTestType))
		{
			strTestType = L"3";
		}
		else if(0 == wcscmp(L"3",sTestLog.wchTestType))
		{
			strTestType = L"4";
		}
		else if(0 == wcscmp(L"5",sTestLog.wchTestType))
		{
			strTestType = L"5";
		}
		root["TestType"] = TCharToANSI(strTestType);
		root["VIN"] = TCharToANSI(sTestLog.wchVIN);
	}
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(root.toStyledString().c_str());
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	strjsonParam.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonParam.Replace(L"\n", L"");//ȥ������
	//strjsonParam.Replace(L" ", L"");//ȥ�ո�

	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"BeginRecord";
	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;

	CNHLogAPI::WriteLog(_T("������־"), _T("BeginRecord����"),send.jsonParam->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("BeginRecord"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		//if(reader1.parse(TCharToANSI(resp.CommonResult->c_str()),root1))
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("BeginRecord�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("BeginRecordʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("BeginRecord"),_T("���ýӿ�ʧ��"));
		return false;
	}
	return true;
}
//������⣬ֹͣ¼��EndRecordAndUpload
bool WebServiceLibAPI::EndDetAndVideo(int nTestType)
{
	if (!m_bUsed)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);

	/********************************************************************/

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);
	LINEINFO sLineInfo;
	GetIniLineInfo(&sLineInfo);

	USERINFO SUserInfo;
	GetIniUserInfo(&SUserInfo);

	if(reader.parse(str,root,true))
	{
		root["LineNo"] = TCharToANSI(sLineInfo.wchLineNumber);
		root["License"] = TCharToANSI(sTestLog.wchPlateNumber);
		//root["LicenseCode"] = TCharToANSI(m_strVehicleType);
		root["LicenseCode"] = TCharToANSI(SetVehicleType(sTestLog.wchReserved1));
		root["VIN"] = TCharToANSI(sTestLog.wchVIN);

		CString strPlateType(L"");
		if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"1";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		root["LicenseType"] = TCharToANSI(strPlateType);
		//root["DataType"] = TCharToANSI(L"0");
		CString strTestCharacter;
		if(0 == wcscmp(sTestLog.wchTestCharacter,L"���"))
		{
			strTestCharacter = L"1";
		}
		else if(0 == wcscmp(sTestLog.wchTestCharacter,L"��ע�ᳵ��"))
		{
			strTestCharacter = L"2";
		}
		else if(0 == wcscmp(sTestLog.wchTestCharacter,L"��س�ת��"))
		{
			strTestCharacter = L"3";
		}
		else if(0 == wcscmp(sTestLog.wchTestCharacter,L"ʵ��Ա�"))
		{
			strTestCharacter = L"4";
		}
		else
		{
			strTestCharacter = L"1";
		}
		root["TestMethod"] = TCharToANSI(strTestCharacter);
		CString str(L"");
		// �ϻ���1:ASM,2:VMAS,3:LUGDOWN,4:DIS,5:FSUNHT,6:FSUYD,7:AVFSU,8:MI,9:MDIS
		// ƽ̨��1:DIS,2:ASM,3:VMAS,4:LUGDOWN,5:FSUNHT
		if(_wtoi(sTestLog.wchTestType) == 1)
		{
			str = L"2";
		}
		else if(_wtoi(sTestLog.wchTestType) == 2)
		{
			str = L"3";
		}
		else if(_wtoi(sTestLog.wchTestType) == 3)
		{
			str = L"4";
		}
		else if(_wtoi(sTestLog.wchTestType) == 4)
		{
			str = L"1";
		}
		else if(_wtoi(sTestLog.wchTestType) == 5)
		{
			str = L"5";
		}
		else if(_wtoi(sTestLog.wchTestType) == 6)
		{
			str = L"6";
		}
		root["TestType"] = TCharToANSI(str);
		root["TestTimes"] = TCharToANSI(sTestLog.wchNumberOfTestPeriod);
		//root["TestPerson"] = TCharToANSI(sTestLog.wchOperator);
		root["TestPerson"] = TCharToANSI(SUserInfo.wchName);
		root["TsBeginTime"] = TCharToANSI(sTestLog.wchTestStartTime);
		root["TsEndTime"] = TCharToANSI(sTestLog.wchTestEndTime);
		root["Judge"] = TCharToANSI(sTestLog.wchPass);
		root["DrivePerson"] = TCharToANSI(sTestLog.wchDriver);
		root["ResponseTime"] = TCharToANSI(L"500");// ������Ӧʱ�䣬�Ժ���Ϊ��λ
	}
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(root.toStyledString().c_str());
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	strjsonParam.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonParam.Replace(L"\n", L"");//ȥ������
	//strjsonParam.Replace(L" ", L"");//ȥ�ո�
	/********************************************************************/

	// �������
	CString strXml(L"<Result_Data>");
	switch(nTestType)
	{
		case ASM://�˷�������
		//{
		//	RESULTOFASM sResultOfASM;
		//	GetIniResultOfASM(&sResultOfASM);
		//	strXml.AppendFormat(L"<TEMPERATURE>%.1f</TEMPERATURE>",_wtof(sResultOfASM.wchEnvironmentalTemperature));
		//	strXml.AppendFormat(L"<PRESSURE>%.1f</PRESSURE>",_wtof(sResultOfASM.wchAtmosphericPressure));
		//	strXml.AppendFormat(L"<RH>%.1f</RH>",_wtof(sResultOfASM.wchRelativeHumidity));
		//	strXml.AppendFormat(L"<HC5025Limit>%d</HC5025Limit>",_wtoi(sResultOfASM.wchLimitOfHC5025));
		//	strXml.AppendFormat(L"<CO5025Limit>%.2f</CO5025Limit>",_wtof(sResultOfASM.wchLimitOfCO5025));
		//	strXml.AppendFormat(L"<NO5025Limit>%d</NO5025Limit>",_wtoi(sResultOfASM.wchLimitOfNO5025));
		//	strXml.AppendFormat(L"<HC2540Limit>%d</HC2540Limit>",_wtoi(sResultOfASM.wchLimitOfHC2540));
		//	strXml.AppendFormat(L"<CO2540Limit>%.2f</CO2540Limit>",_wtof(sResultOfASM.wchLimitOfCO2540));
		//	strXml.AppendFormat(L"<NO2540Limit>%d</NO2540Limit>",_wtoi(sResultOfASM.wchLimitOfNO2540));
		//	strXml.AppendFormat(L"<HC5025>%d</HC5025>",_wtoi(sResultOfASM.wchHC5025));
		//	strXml.AppendFormat(L"<CO5025>%.2f</CO5025>",_wtof(sResultOfASM.wchCO5025));
		//	strXml.AppendFormat(L"<NO5025>%d</NO5025>",_wtoi(sResultOfASM.wchNO5025));
		//	strXml.AppendFormat(L"<HC2540>%d</HC2540>",_wtoi(sResultOfASM.wchHC2540));
		//	strXml.AppendFormat(L"<CO2540>%.2f</CO2540>",_wtof(sResultOfASM.wchCO2540));
		//	strXml.AppendFormat(L"<NO2540>%d</NO2540>",_wtoi(sResultOfASM.wchNO2540));
		//	strXml.AppendFormat(L"<HC5025Judge>%d</HC5025Judge>",_wtoi(sResultOfASM.wchPassOfHC5025));
		//	strXml.AppendFormat(L"<CO5025Judge>%d</CO5025Judge>",_wtoi(sResultOfASM.wchPassOfCO5025));
		//	strXml.AppendFormat(L"<NO5025Judge>%d</NO5025Judge>",_wtoi(sResultOfASM.wchPassOfNO5025));
		//	strXml.AppendFormat(L"<HC2540Judge>%d</HC2540Judge>",_wtoi(sResultOfASM.wchPassOfHC2540));
		//	strXml.AppendFormat(L"<CO2540Judge>%d</CO2540Judge>",_wtoi(sResultOfASM.wchPassOfCO2540));
		//	strXml.AppendFormat(L"<NO2540Judge>%d</NO2540Judge>",_wtoi(sResultOfASM.wchPassOfNO2540));
		//}
		break;

		case DIS:
		{
			RESULTOFDIS SResultOfDIS;
			GetIniResultOfDIS(&SResultOfDIS);
			strXml.AppendFormat(L"<TEMPERATURE>%.1f</TEMPERATURE>",_wtof(SResultOfDIS.wchEnvironmentalTemperature));
			strXml.AppendFormat(L"<PRESSURE>%.1f</PRESSURE>",_wtof(SResultOfDIS.wchAtmosphericPressure));
			strXml.AppendFormat(L"<RH>%.1f</RH>",_wtof(SResultOfDIS.wchRelativeHumidity));
			strXml.AppendFormat(L"<OLITEMP>%.2f</OLITEMP>",_wtof(SResultOfDIS.wchOilTemperatureHigh));

			VEHICLEINFO SVehicleInfo;
			GetIniVehicleInfo(&SVehicleInfo);
			CString strTemp;
			//////////////////// ���޴߻�ת����
			bool bHasCatalyticConverter = false;
			strTemp = SVehicleInfo.wchHasCatalyticConverter;
			if(L"1" == strTemp)
			{
				bHasCatalyticConverter = true;
			}
			/////////////////////// ���ͷ�ʽ
			strTemp = SVehicleInfo.wchOilSupplyMode;

			//if(strTemp.Find(L"�ջ�") != -1 && bHasCatalyticConverter)
			//{
				strXml.AppendFormat(L"<LambdaUp>%.2f</LambdaUp>",_wtof(SResultOfDIS.wchLowerLimitOfLambda));//�˴�ƽ̨�Ƿ���
				strXml.AppendFormat(L"<LambdaDown>%.2f</LambdaDown>",_wtof(SResultOfDIS.wchUpperLimitOfLambda));
				strXml.AppendFormat(L"<Lambda>%.2f</Lambda>",_wtof(SResultOfDIS.wchLambda));
				strXml.AppendFormat(L"<LambdaJudge>%d</LambdaJudge>",_wtoi(SResultOfDIS.wchPassOfLambda));
			//}
			//else
			//{
				//strXml.AppendFormat(L"<LambdaUp></LambdaUp>");
				//strXml.AppendFormat(L"<LambdaDown></LambdaDown>");
				//strXml.AppendFormat(L"<Lambda></Lambda>");
				//strXml.AppendFormat(L"<LambdaJudge></LambdaJudge>");
			//}
			//strXml.AppendFormat(L"<Lambda>%.2f</Lambda>",_wtof(SResultOfDIS.wchLambda));
			
			strXml.AppendFormat(L"<LSCOLimit>%.2f</LSCOLimit>",_wtof(SResultOfDIS.wchLimitOfCOLow));
			strXml.AppendFormat(L"<LSCOResult>%.2f</LSCOResult>",_wtof(SResultOfDIS.wchCOLow));
			strXml.AppendFormat(L"<LSCOJudge>%d</LSCOJudge>",_wtoi(SResultOfDIS.wchPassOfCOLow));
			strXml.AppendFormat(L"<LSHCLimit>%d</LSHCLimit>",_wtoi(SResultOfDIS.wchLimitOfHCLow));
			strXml.AppendFormat(L"<LSHCResult>%d</LSHCResult>",_wtoi(SResultOfDIS.wchHCLow));
			strXml.AppendFormat(L"<LSHCJudge>%d</LSHCJudge>",_wtoi(SResultOfDIS.wchPassOfHCLow));
			strXml.AppendFormat(L"<HSCOLimit>%.2f</HSCOLimit>",_wtof(SResultOfDIS.wchLimitOfCOHigh));
			strXml.AppendFormat(L"<HSCOResult>%.2f</HSCOResult>",_wtof(SResultOfDIS.wchCOHigh));
			strXml.AppendFormat(L"<HSCOJudge>%d</HSCOJudge>",_wtoi(SResultOfDIS.wchPassOfCOHigh));
			strXml.AppendFormat(L"<HSHCLimit>%d</HSHCLimit>",_wtoi(SResultOfDIS.wchLimitOfHCHigh));
			strXml.AppendFormat(L"<HSHCResult>%d</HSHCResult>",_wtoi(SResultOfDIS.wchHCHigh));
			strXml.AppendFormat(L"<HSHCJudge>%d</HSHCJudge>",_wtoi(SResultOfDIS.wchPassOfHCHigh));
			strXml.AppendFormat(L"<Gdszs>%d</Gdszs>",_wtoi(SResultOfDIS.wchRevHigh));
			strXml.AppendFormat(L"<Dszs>%d</Dszs>",_wtoi(SResultOfDIS.wchRevLow));
		}
		break;
		
		case VMAS:
		{
			RESULTOFVMAS SResultOfVmas;
			GetIniResultOfVMAS(&SResultOfVmas);

			strXml.AppendFormat(L"<TEMPERATURE>%.1f</TEMPERATURE>",_wtof(SResultOfVmas.wchEnvironmentalTemperature));
			strXml.AppendFormat(L"<PRESSURE>%.1f</PRESSURE>",_wtof(SResultOfVmas.wchAtmosphericPressure));
			strXml.AppendFormat(L"<RH>%.1f</RH>",_wtof(SResultOfVmas.wchRelativeHumidity));
			
			strXml.AppendFormat(L"<HCLimit>%.2f</HCLimit>",_wtof(SResultOfVmas.wchLimitOfHC));
			strXml.AppendFormat(L"<COLimit>%.2f</COLimit>",_wtof(SResultOfVmas.wchLimitOfCO));
			strXml.AppendFormat(L"<NOxLimit>%.2f</NOxLimit>",_wtof(SResultOfVmas.wchLimitOfNOx));

			strXml.AppendFormat(L"<HC>%.2f</HC>",_wtof(SResultOfVmas.wchHC));
			strXml.AppendFormat(L"<CO>%.2f</CO>",_wtof(SResultOfVmas.wchCO));
			strXml.AppendFormat(L"<NOx>%.2f</NOx>",_wtof(SResultOfVmas.wchNOx));

			strXml.AppendFormat(L"<HCJudge>%d</HCJudge>",_wtoi(SResultOfVmas.wchPassOfHC));
			strXml.AppendFormat(L"<COJudge>%d</COJudge>",_wtoi(SResultOfVmas.wchPassOfCO));
			strXml.AppendFormat(L"<NOxJudge>%d</NOxJudge>",_wtoi(SResultOfVmas.wchPassOfNOx));

			strXml.AppendFormat(L"<Odometer>%.3f</Odometer>",_wtof(SResultOfVmas.wchTravelledDistance));
			strXml.AppendFormat(L"<ExtractGas>%d</ExtractGas>",_wtoi(SResultOfVmas.wchVolOfExhaust));
			strXml.AppendFormat(L"<DivergeCount>%d</DivergeCount>",0);//���������е�ƫ�����
			strXml.AppendFormat(L"<DivergeSeconds>%d</DivergeSeconds>",0);//���������е�ƫ��ʱ��
			strXml.AppendFormat(L"<TestSecond>%d</TestSecond>",195);//����ʱ��
			strXml.AppendFormat(L"<EO2>%.2f</EO2>",_wtof(SResultOfVmas.wchO2));//���������е� O2 Ũ��
			strXml.AppendFormat(L"<SettingDLoad>%.2f</SettingDLoad>",_wtof(SResultOfVmas.wchPower50));//�⹦���趨����
		}
		break;

		case LUGDOWN:
		{
			RESULTOFLUGDOWN sResultOfLugdowm;
			GetIniResultOfLUGDOWN(&sResultOfLugdowm);
			TESTLOG sTestLog;
			GetIniTestLog(&sTestLog);
			strXml.AppendFormat(L"<TEMPERATURE>%.1f</TEMPERATURE>",_wtof(sResultOfLugdowm.wchEnvironmentalTemperature));
			strXml.AppendFormat(L"<PRESSURE>%.1f</PRESSURE>",_wtof(sResultOfLugdowm.wchAtmosphericPressure));
			strXml.AppendFormat(L"<RH>%.1f</RH>",_wtof(sResultOfLugdowm.wchRelativeHumidity));
			strXml.AppendFormat(L"<SmokeKLimit>%.2f</SmokeKLimit>",_wtof(sResultOfLugdowm.wchKLimit));
			strXml.AppendFormat(L"<K100>%.2f</K100>",_wtof(sResultOfLugdowm.wchK100));
			strXml.AppendFormat(L"<K80>%.2f</K80>",_wtof(sResultOfLugdowm.wchK80));
			//strXml.AppendFormat(L"<SmokeNLimit>%.2f</SmokeNLimit>",_wtof(sResultOfLugdowm.wchNLimit));
			strXml.AppendFormat(L"<SmokeNLimit>%s</SmokeNLimit>",L"40");
			strXml.AppendFormat(L"<N100>%.2f</N100>",_wtof(sResultOfLugdowm.wchN100));
			strXml.AppendFormat(L"<N80>%.2f</N80>",_wtof(sResultOfLugdowm.wchN80));
			strXml.AppendFormat(L"<MaxPowerLimit>%.2f</MaxPowerLimit>",_wtof(sTestLog.wchRatedPower)*0.40f);
			strXml.AppendFormat(L"<NOxLimit>%.2f</NOxLimit>",_wtof(sResultOfLugdowm.wchNOxLimit));//NOx �ŷ���ֵ
			strXml.AppendFormat(L"<MaxPower>%.2f</MaxPower>",_wtof(sResultOfLugdowm.wchCorrectedPower));
			strXml.AppendFormat(L"<NOx80>%d</NOx80>",_wtoi(sResultOfLugdowm.wchNOx80));
			strXml.AppendFormat(L"<NOx100>%d</NOx100>",_wtoi(sResultOfLugdowm.wchNOx100));
			strXml.AppendFormat(L"<SpeedUp>%d</SpeedUp>",_wtoi(sResultOfLugdowm.wchRatedRev));//�������ת������
			strXml.AppendFormat(L"<SpeedDown>%d</SpeedDown>",_wtoi(sResultOfLugdowm.wchRatedRev));//�������ת������
			strXml.AppendFormat(L"<ERatedSpeed>%d</ERatedSpeed>",_wtoi(sResultOfLugdowm.wchRatedRev));
			strXml.AppendFormat(L"<MaxPowerESpeed>%d</MaxPowerESpeed>",_wtoi(sResultOfLugdowm.wchMaxRPM));//ʵ������ֱ߹��ʷ�����ת��
			strXml.AppendFormat(L"<Result>%d</Result>",_wtoi(sResultOfLugdowm.wchPass));
		}
		break;

		case FSUNHT:
		{
			RESULTOFFSUNHT sResultOfFSUNHT;
			GetIniResultOfFSUNHT(&sResultOfFSUNHT);
			strXml.AppendFormat(L"<RatedSpeed>%d</RatedSpeed>",_wtoi(sTestLog.wchRatedRev));
			strXml.AppendFormat(L"<RealSpeed>%d</RealSpeed>",_wtoi(sResultOfFSUNHT.wchEngineRev));
			strXml.AppendFormat(L"<SmokeKLimit>%.2f</SmokeKLimit>",_wtof(sResultOfFSUNHT.wchLimit));
			strXml.AppendFormat(L"<SmokeK1>%.2f</SmokeK1>",_wtof(sResultOfFSUNHT.wchValue2));
			strXml.AppendFormat(L"<SmokeK2>%.2f</SmokeK2>",_wtof(sResultOfFSUNHT.wchValue3));
			strXml.AppendFormat(L"<SmokeK3>%.2f</SmokeK3>",_wtof(sResultOfFSUNHT.wchValue4));
			strXml.AppendFormat(L"<SmokeKAvg>%.2f</SmokeKAvg>",_wtof(sResultOfFSUNHT.wchAverage));
			//float fLimitK = _wtof(sResultOfFSUNHT.wchLimit);
			//float fLimitN = 100 * (1 - exp(fLimitK * (-430/1000.0f)));
			strXml.AppendFormat(L"<SmokeNLimit>%s</SmokeNLimit>",L"40");
			float fK1 = _wtof(sResultOfFSUNHT.wchValue2);
			float fK2 = _wtof(sResultOfFSUNHT.wchValue3);
			float fK3 = _wtof(sResultOfFSUNHT.wchValue4);
			float fN1 = 100 * (1 - exp(fK1 * (-430/1000.0f)));
			float fN2 = 100 * (1 - exp(fK2 * (-430/1000.0f)));
			float fN3 = 100 * (1 - exp(fK3 * (-430/1000.0f)));
			strXml.AppendFormat(L"<SmokeN1>%.2f</SmokeN1>",fN1);
			strXml.AppendFormat(L"<SmokeN2>%.2f</SmokeN2>",fN2);
			strXml.AppendFormat(L"<SmokeN3>%.2f</SmokeN3>",fN3);
			float fKAverage = _wtof(sResultOfFSUNHT.wchAverage);
			float fNAverage = 100 * (1 - exp(fKAverage * (-430/1000.0f)));
			strXml.AppendFormat(L"<SmokeNAvg>%.2f</SmokeNAvg>",fNAverage);
			strXml.AppendFormat(L"<Speed1>%.2f</Speed1>",_wtof(sResultOfFSUNHT.wchEngineRev2));
			strXml.AppendFormat(L"<Speed2>%.2f</Speed2>",_wtof(sResultOfFSUNHT.wchEngineRev3));
			strXml.AppendFormat(L"<Speed3>%.2f</Speed3>",_wtof(sResultOfFSUNHT.wchEngineRev4));
			strXml.AppendFormat(L"<TEMPERATURE>%.1f</TEMPERATURE>",_wtof(sResultOfFSUNHT.wchEnvironmentalTemperature));
			strXml.AppendFormat(L"<PRESSURE>%.1f</PRESSURE>",_wtof(sResultOfFSUNHT.wchAtmosphericPressure));
			strXml.AppendFormat(L"<RH>%.1f</RH>",_wtof(sResultOfFSUNHT.wchRelativeHumidity));
			strXml.AppendFormat(L"<Result>%d</Result>",_wtoi(sResultOfFSUNHT.wchPass));
		}
		break;

		default:
		break;
	}
	strXml = strXml + L"</Result_Data>" ;

	/********************************************************************/

	//��������
	CString strSourceData(L"<Source_Data>"); // �뼶
	CString strSourceData2(L"<Source_Data>"); // ���뼶
	CString strTemp(L"");
	switch(nTestType)
	{
		case DIS:
		{
			list<REALTIMEDATAOFDIS> listRealTimeDataOFDis;
			GetIniRealTimeDataOfDIS(listRealTimeDataOFDis);
			for(list<REALTIMEDATAOFDIS>::iterator ir=listRealTimeDataOFDis.begin(); ir!=listRealTimeDataOFDis.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
				strTemp.Empty();
				//0-70%�ת�١�1-�ߵ���׼����2-�ߵ��ټ�⡢3-����׼����4-���ټ��
				// ��ⲽ�����: 0-���δ��ʼ; 1-�ȳ�; 2-��һ������ͬ��;3-�ߵ��ٲ���; 4-�ڶ�������ͬ��; 5-���ٲ���; 6-���;7-�ߵ���ת���ٵȴ�; 8-���
				if(_wtoi(ir->wchReserved2) == 7)
				{
					strTemp = L"1";
				}
				else if(_wtoi(ir->wchReserved2) == 8)
				{
					strTemp = L"0";
				}
				else
				{
					strTemp.Format(L"%d",_wtoi(ir->wchReserved2) -1);
				}
				strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strTemp.Empty();
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<HC>%s</HC>",ir->wchHC);
				strSourceData.AppendFormat(L"<CO>%s</CO>",ir->wchCO);
				strSourceData.AppendFormat(L"<O2>%s</O2>",ir->wchO2);
				strSourceData.AppendFormat(L"<CO2>%s</CO2>",ir->wchCO2);
				strSourceData.AppendFormat(L"<LANBDAMP>%s</LANBDAMP>",ir->wchLambda);
				strSourceData.AppendFormat(L"<RATE>%s</RATE>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>",_wtoi(ir->wchReserved1) == 1 ? L"0" : L"1");//���ݱ�־:0-������1-����
				strSourceData.AppendFormat(L"<OILTEMP>%s</OILTEMP>",ir->wchOilTemperature);
				strSourceData.AppendFormat(L"</Data>");
				

				// ���뼶���� (�Ѽ�¼)
				//0-70%�ת�١�1-�ߵ���׼����2-�ߵ��ټ�⡢3-����׼����4-���ټ��
				if(_wtoi(ir->wchReserved2) == 7)
				{
					strTemp = L"1";
				}
				else if(_wtoi(ir->wchReserved2) == 8)
				{
					strTemp = L"0";
				}
				else
				{
					strTemp.Format(L"%d",_wtoi(ir->wchReserved2) -1);
				}
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",_wtoi(ir->wchReserved1) == 1 ? L"0" : L"1");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"HC");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchHC);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",_wtoi(ir->wchReserved1) == 1 ? L"0" : L"1");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"CO");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchCO);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",_wtoi(ir->wchReserved1) == 1 ? L"0" : L"1");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"O2");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchO2);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",_wtoi(ir->wchReserved1) == 1 ? L"0" : L"1");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"CO2");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchCO2);
				strSourceData2.AppendFormat(L"</Data>");

				strTemp.Empty();
			}
		}
		break;
		
		case VMAS:
		{
			// ��ȡRealTimeDataOfVMAS.ini·��
			wchar_t wchPath[MAX_PATH];	
			if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfVMAS.ini", wchPath))
			{
			}
			CSimpleIni sRealTimeDataOFVMAS(wchPath);
			// ��ȡ������
			int nTotal = _wtoi(sRealTimeDataOFVMAS.GetString(L"Sum",L"Total",L"0"));	

			list<REALTIMEDATAOFVMAS> listRealTimeDataOFVMAS;
			GetIniRealTimeDataOfVMAS(listRealTimeDataOFVMAS);

			for(list<REALTIMEDATAOFVMAS>::iterator ir=listRealTimeDataOFVMAS.begin(); ir!=listRealTimeDataOFVMAS.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
				strTemp.Empty();
				//0-�豸׼����1-���ǰ����׼����2-195 ������
				CString strState;
				if(nTotal > 195)
				{
					int nTime;
					nTime = nTotal - 195;
					if(_wtoi(ir->wchTime) > nTime)
					{
						strState = L"2";
					}
					else
					{
						strState = L"1";
					}
				}
				else
				{
					strState = L"2";
				}
				strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<HCREAL>%s</HCREAL>",ir->wchHC);//��ֵӦ��������ǰ��ֵ
				strSourceData.AppendFormat(L"<COREAL>%s</COREAL>",ir->wchCO);//��ֵӦ��������ǰ��ֵ
				strSourceData.AppendFormat(L"<CO2REAL>%s</CO2REAL>",ir->wchCO2);//��ֵӦ��������ǰ��ֵ
				strSourceData.AppendFormat(L"<NOREAL>%s</NOREAL>",ir->wchNO);//��ֵӦ��������ǰ��ֵ
				strSourceData.AppendFormat(L"<ENVIROO2REAL>%s</ENVIROO2REAL>",ir->wchO2OfEnvironment);
				//
				strSourceData.AppendFormat(L"<ANALYSISO2REAL>%s</ANALYSISO2REAL>",ir->wchO2);
				strSourceData.AppendFormat(L"<FLOWO2REAL>%s</FLOWO2REAL>",ir->wchO2OfFlowmeter);
				strSourceData.AppendFormat(L"<HCQUALITY>%s</HCQUALITY>",ir->wchHCm);//��ֵӦ��������ǰ��ֵ
				strSourceData.AppendFormat(L"<COQUALITY>%s</COQUALITY>",ir->wchCOm);//��ֵӦ��������ǰ��ֵ
				strSourceData.AppendFormat(L"<NOXQUALITY>%s</NOXQUALITY>",ir->wchNOm);//��ֵӦ��������ǰ��ֵ
				//
				strSourceData.AppendFormat(L"<STANDARDSPEED>%s</STANDARDSPEED>",ir->wchVelocity);
				strSourceData.AppendFormat(L"<REALSPEED>%s</REALSPEED>",ir->wchVelocity);
				strSourceData.AppendFormat(L"<TORQUE>%s</TORQUE>",ir->wchForce);
				strSourceData.AppendFormat(L"<ROALPOWER>%s</ROALPOWER>",ir->wchPower);
				strSourceData.AppendFormat(L"<TEMPERATURE>%s</TEMPERATURE>",ir->wchEnvironmentalTemperature);
				strSourceData.AppendFormat(L"<RELATIVEHUMIDITY>%s</RELATIVEHUMIDITY>",ir->wchRelativeHumidity);
				strSourceData.AppendFormat(L"<AIRPRESSURE>%s</AIRPRESSURE>",ir->wchAtmosphericPressure);
				strSourceData.AppendFormat(L"<FLOWTEMPERATURE>%s</FLOWTEMPERATURE>",ir->wchTemperatureOfFlowmeter);
				strSourceData.AppendFormat(L"<FLOWPRESSURE>%s</FLOWPRESSURE>",ir->wchPressureOfFlowmeter);
				strSourceData.AppendFormat(L"<REALVOLUME>%s</REALVOLUME>",ir->wchActualFluxOfGas);
				strSourceData.AppendFormat(L"<STANDARDVOLUME>%s</STANDARDVOLUME>",ir->wchStandardFluxOfGas);
				strSourceData.AppendFormat(L"<HUMIDITYCORRECT>%s</HUMIDITYCORRECT>",ir->wchHumidityCorrectionFactor);
				strSourceData.AppendFormat(L"<DILUTIONCORRECT>%s</DILUTIONCORRECT>",ir->wchDilutionCorrectionFactor);
				strSourceData.AppendFormat(L"<DILUTIONRATE>%s</DILUTIONRATE>",ir->wchDR);
				strSourceData.AppendFormat(L"<ANALYSISPRESSURE>%s</ANALYSISPRESSURE>",ir->wchPressureOfFlowmeter);
				strSourceData.AppendFormat(L"<GASREALFLOW>%s</GASREALFLOW>",ir->wchFluxOfExhaust);
				strSourceData.AppendFormat(L"<LAMBDA>%s</LAMBDA>",ir->wchLambda);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData.AppendFormat(L"<DLOAD>%s</DLOAD>",ir->wchIHP);//���̲⹦���غ�
				strSourceData.AppendFormat(L"<HCREALXX>%s</HCREALXX>",ir->wchHC);//HC Ũ��ֵ:pm��ϡ��������Ũ��
				strSourceData.AppendFormat(L"<COREALXX>%s</COREALXX>",ir->wchCO);//CO Ũ��ֵ:%��ϡ��������Ũ��
				strSourceData.AppendFormat(L"<NOxREALXX>%s</NOxREALXX>",ir->wchNO);//NOx Ũ��ֵ:pm��ϡ��������Ũ��
				strSourceData.AppendFormat(L"</Data>");
				
				// ���뼶���� (�Ѽ�¼)
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"REALVOLUME");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchActualFluxOfGas);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"FLOWPRESSURE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchPressureOfFlowmeter);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"FLOWTEMPERATURE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchTemperatureOfFlowmeter);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"ANALYSISO2REAL");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchO2);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"FLOWO2REAL");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchO2OfFlowmeter);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"CO2REAL");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchCO2);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"COREAL");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchCO);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"HCREAL");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchHC);
				strSourceData2.AppendFormat(L"</Data>");

				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"NOREAL");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchNO);
				strSourceData2.AppendFormat(L"</Data>");
			}

		}
		break;

		case LUGDOWN:
		{

			list<REALTIMEDATAOFLUGDOWN> listRealTimeDataOfLUGDOWN;
			GetIniRealTimeDataOfLUGDOWN(listRealTimeDataOfLUGDOWN);

			int n100(0);
			for(list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN.begin(); ir!=listRealTimeDataOfLUGDOWN.end(); ++ir)
			{
				int nState = _wtoi(ir->wchState);
				if (3== nState)
				{
					n100++;
				}
				else if (3 < nState)
				{
					break;
				}
			}
			CString strTemp(L"");
			for(list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN.begin(); ir!=listRealTimeDataOfLUGDOWN.end(); ++ir)
			{

				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Mid(0,19));//ȥ������
				strTemp.Empty();

				if(_wtoi(ir->wchState)<=2)
				{
					strTemp = L"0";
				}
				else if(_wtoi(ir->wchState)==3 &&n100>12)//��¼100%�׶�11��ǰ����Ϊ%100�ָ�
				{
					n100--;
					strTemp = L"1";
				}
				else if(_wtoi(ir->wchState)==3)
				{
					strTemp = L"2";
				}
				else if(_wtoi(ir->wchState)==4)
				{
					strTemp = L"3";
				}
				else if(_wtoi(ir->wchState)==5)
				{
					strTemp = L"4";
				}
				else if(_wtoi(ir->wchState)>5)//�����ݲ���¼
				{
					strTemp = L"4";
				}
				strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<REALSPEED>%s</REALSPEED>",ir->wchVelocity);//ʵʱ����/ת�����ٶ�km/h
				strSourceData.AppendFormat(L"<TORQUE>%s</TORQUE>",ir->wchForce);//Ť��N
				strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);//������ת��r/min
				strSourceData.AppendFormat(L"<DLOAD>%s</DLOAD>",ir->wchPower);//�⹦���غ�kW
				strSourceData.AppendFormat(L"<OPACITYSMOKE>%s</OPACITYSMOKE>",ir->wchN);//��͸���%
				strSourceData.AppendFormat(L"<LIGHTABSORB>%s</LIGHTABSORB>",ir->wchK);//������ϵ��k/m -1
				strSourceData.AppendFormat(L"<NOx>%d</NOx>",_wtoi(ir->wchNO)+_wtoi(ir->wchNO2));//��������10 -6
				strSourceData.AppendFormat(L"<CO2>%s</CO2>",ir->wchCO2);//������̼%
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData.AppendFormat(L"</Data>");

			}
			
			// ���뼶���� (�Ѽ�¼)
			list<REALTIMEDATAOFLUGDOWN> listRealTimeDataOfLUGDOWN100ms;
			//GetIniRealTimeDataOfLUGDOWN100ms(listRealTimeDataOfLUGDOWN100ms);
			GetIniRealTimeDataOfLUGDOWN(listRealTimeDataOfLUGDOWN100ms);
			int n100ms(0);
			for(list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN100ms.begin(); ir!=listRealTimeDataOfLUGDOWN100ms.end(); ++ir)
			{
				int nState = _wtoi(ir->wchState);
				if (3== nState)
				{
					n100ms++;
				}
				else if (3 < nState)
				{
					break;
				}
			}

			for(list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN100ms.begin(); ir!=listRealTimeDataOfLUGDOWN100ms.end(); ++ir)
			{
				if(_wtoi(ir->wchState)<=2)
				{
					strTemp = L"0";
				}
				else if(_wtoi(ir->wchState)==3 &&n100ms>120)//��¼100%�׶�11��ǰ����Ϊ%100�ָ�
				{
					n100ms--;
					strTemp = L"1";
				}
				else if(_wtoi(ir->wchState)==3)
				{
					strTemp = L"2";
				}
				else if(_wtoi(ir->wchState)==4)
				{
					strTemp = L"3";
				}
				else if(_wtoi(ir->wchState)==5)
				{
					strTemp = L"4";
				}
				else if(_wtoi(ir->wchState)>5)//�����ݲ���¼
				{
					strTemp = L"4";
				}
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"LIGHTABSORB");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchK);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"OPACITYSMOKE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchN);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"TORQUE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchForce);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"REALSPEED");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>", ir->wchVelocity);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"TEMPERATURE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchEnvironmentalTemperature);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"RH");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchRelativeHumidity);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"PRESSURE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>",ir->wchAtmosphericPressure);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"NOx");
				strSourceData2.AppendFormat(L"<VALUE>%d</VALUE>",_wtoi(ir->wchNO)+_wtoi(ir->wchNO2));
				strSourceData2.AppendFormat(L"</Data>");
			}
		}
		break;

		case FSUNHT:
		{
			list<REALTIMEDATAOFFSUNHT> listRealTimeDataOfFSUNHT;
			GetIniRealTimeDataOfFSUNHT(listRealTimeDataOfFSUNHT);

			for(list<REALTIMEDATAOFFSUNHT>::iterator ir = listRealTimeDataOfFSUNHT.begin();ir != listRealTimeDataOfFSUNHT.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
				strTemp.Empty();
				strSourceData.AppendFormat(L"<WORKTYPE>%d</WORKTYPE>",_wtoi(ir->wchOrder)-3);
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<OPACITYSMOKE>%s</OPACITYSMOKE>",ir->wchN);
				strSourceData.AppendFormat(L"<LIGHTABSORB>%s</LIGHTABSORB>",ir->wchK);
				strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����   ����׶���ɲ��ϴ���ʼ�ź�
				strSourceData.AppendFormat(L"</Data>");
				
				// ���뼶���� (�Ѽ�¼)
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%d</WORKTYPE>",_wtoi(ir->wchOrder)-3);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����   ����׶���ɲ��ϴ���ʼ�ź�
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"OPACITYSMOKE");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>", ir->wchN);
				strSourceData2.AppendFormat(L"</Data>");
				
				strSourceData2.AppendFormat(L"<Data>");
				strSourceData2.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",ir->wchSamplingTime);
				strSourceData2.AppendFormat(L"<WORKTYPE>%d</WORKTYPE>",_wtoi(ir->wchOrder)-3);
				strSourceData2.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData2.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����   ����׶���ɲ��ϴ���ʼ�ź�
				strSourceData2.AppendFormat(L"<ITEM>%s</ITEM>",L"LIGHTABSORB");
				strSourceData2.AppendFormat(L"<VALUE>%s</VALUE>", ir->wchK);
				strSourceData2.AppendFormat(L"</Data>");
			}
		}
		break;
	}
	strSourceData = strSourceData + L"</Source_Data>";
	strSourceData2 = strSourceData2 + L"</Source_Data>";

	/********************************************************************/
	//OBD��������
	const char* strOBD = "{}";
	Json::Value vOBD;
	Json::Value v;

	CString strReportNumber;	
	strReportNumber.Format(L"%s",sTestLog.wchReportNumber);

	if(reader.parse(strOBD,vOBD,true))
	{
		switch(nTestType)
		{
		case DIS:
			{
				CString strSQL = L"SELECT * FROM ResultOfDIS WHERE ReportNumber = '" + strReportNumber+"'";
				std::vector<SRealTimeDataOfDIS> vtRTData;
				SResultOfDIS sResultOfDIS;
				GetDboResultOfDIS_Zip(strSQL, &sResultOfDIS);
				vtRTData = sResultOfDIS.RTDataUpZip();

				for (UINT i=0; i<vtRTData.size(); i++)
				{
					v["DATATIME"] = TCharToANSI(vtRTData[i].strSamplingTime.c_str());//����ʱ��
					v["OBDDEVICEID"] = TCharToANSI(sLineInfo.wchOBDAdapterNumber);//OBD �豸 ID
					v["RAO"] = TCharToANSI(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str());//�����ž��Կ���
					v["CLD"] = TCharToANSI(vtRTData[i].sOBDRTData.strCalculationLoad.c_str());//���㸺��ֵ
					v["LAMBDA"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//��������ϵ��
					v["SPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strVelocity.c_str());//����
					v["ESPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineRev.c_str());//������ת��
					v["AIRINPUT"] = TCharToANSI(vtRTData[i].sOBDRTData.strMAF.c_str());//������
					v["AIRINPUTPRESSURE"] = TCharToANSI(vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());//����ѹ��
					v["EDLOAD"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());//�������������
					v["FRONTO2SENSORSIGNALDY"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//ǰ���������źţ���ѹ��mV
					v["FRONTO2SENSORSIGNALDL"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//ǰ���������źţ�������ma

					v["FRONTO2SENSORSIGNAL"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//ǰ���������ź�

					vOBD["body"].append(v);
				}
			}
			break;

		case ASM:
			{
			}
			break;

		case VMAS:
			{
				CString strSQL = L"SELECT * FROM ResultOfVMAS WHERE ReportNumber = '" + strReportNumber+"'";
				std::vector<SRealTimeDataOfVMAS> vtRTData;
				SResultOfVMAS sResultOfVMAS;
				GetDboResultOfVMAS_Zip(strSQL, &sResultOfVMAS);
				vtRTData = sResultOfVMAS.RTDataUpZip();

				for (UINT i=0; i<vtRTData.size(); i++)
				{
					v["DATATIME"] = TCharToANSI(vtRTData[i].strSamplingTime.c_str());//����ʱ��
					v["OBDDEVICEID"] = TCharToANSI(sLineInfo.wchOBDAdapterNumber);//OBD �豸 ID
					v["RAO"] = TCharToANSI(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str());//�����ž��Կ���
					v["CLD"] = TCharToANSI(vtRTData[i].sOBDRTData.strCalculationLoad.c_str());//���㸺��ֵ
					v["LAMBDA"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//��������ϵ��
					v["SPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strVelocity.c_str());//����
					v["ESPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineRev.c_str());//������ת��
					v["AIRINPUT"] = TCharToANSI(vtRTData[i].sOBDRTData.strMAF.c_str());//������
					v["AIRINPUTPRESSURE"] = TCharToANSI(vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());//����ѹ��
					v["EDLOAD"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());//�������������
					v["FRONTO2SENSORSIGNAL"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//ǰ���������ź�
					v["FRONTO2SENSORSIGNALDY"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//ǰ���������źţ���ѹ��mV
					v["FRONTO2SENSORSIGNALDL"] = TCharToANSI(vtRTData[i].sOBDRTData.strAirCoefficient.c_str());//ǰ���������źţ�������ma
					
					vOBD["body"].append(v);
				}
			}
			break;

		case LUGDOWN:
			{
				CString strSQL = L"SELECT * FROM ResultOfLUGDOWN WHERE ReportNumber = '" + strReportNumber+"'";
				std::vector<SRealTimeDataOfLUGDOWN> vtRTData;
				SResultOfLUGDOWN sResultOfLUGDOWN;
				GetDboResultOfLUGDOWN_Zip(strSQL, &sResultOfLUGDOWN);
				vtRTData = sResultOfLUGDOWN.RTDataUpZip();

				for (UINT i=0; i<vtRTData.size(); i++)
				{
					v["DATATIME"] = TCharToANSI(vtRTData[i].strSamplingTime.c_str());//����ʱ��
					v["OBDDEVICEID"] = TCharToANSI(sLineInfo.wchOBDAdapterNumber);//OBD �豸 ID
					v["AGO"] = TCharToANSI(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str());//���ſ���
					v["SPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strVelocity.c_str());//����
					v["DLOAD"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());//�������������
					v["ESPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineRev.c_str());//������ת��
					v["AIRINPUT"] = TCharToANSI(vtRTData[i].sOBDRTData.strMAF.c_str());//������
					v["SCP"] = TCharToANSI(vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());//��ѹѹ��
					v["OILWEAR"] = TCharToANSI(vtRTData[i].sOBDRTData.strFuelConsumption.c_str());//������
					v["NOMIC"] = TCharToANSI(vtRTData[i].sOBDRTData.strNOXConcentration.c_str());//����������Ũ��
					v["UREAJET"] = TCharToANSI(vtRTData[i].sOBDRTData.strUreaInjectionVolume.c_str());//����������
					v["GASTEMP"] = TCharToANSI(vtRTData[i].sOBDRTData.strEGT.c_str());//�����¶�
					v["DCPD"] = TCharToANSI(vtRTData[i].sOBDRTData.strDPFDifferentialPressure.c_str());//����������ѹ��
					v["EGRO"] = TCharToANSI(vtRTData[i].sOBDRTData.strEGRPosition.c_str());//EGR ����
					v["OILJETP"] = TCharToANSI(vtRTData[i].sOBDRTData.strFuelDeliveryPressure.c_str());//ȼ������ѹ��
					
					vOBD["body"].append(v);
				}
			}
			break;

		case FSUNHT:
			{
				CString strSQL = L"SELECT * FROM ResultOfFSUNHT WHERE ReportNumber = '" + strReportNumber+"'";
				std::vector<SRealTimeDataOfFSUNHT> vtRTData;
				SResultOfFSUNHT sResultOfFSUNHT;
				GetDboResultOfFSUNHT_Zip(strSQL, &sResultOfFSUNHT);
				vtRTData = sResultOfFSUNHT.RTDataUpZip();

				for (UINT i=0; i<vtRTData.size(); i++)
				{
					v["DATATIME"] = TCharToANSI(vtRTData[i].strSamplingTime.c_str());//����ʱ��
					v["OBDDEVICEID"] = TCharToANSI(sLineInfo.wchOBDAdapterNumber);//OBD �豸 ID
					v["AGO"] = TCharToANSI(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str());//���ſ���
					v["SPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strVelocity.c_str());//����
					v["DLOAD"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());//�������������
					v["ESPEED"] = TCharToANSI(vtRTData[i].sOBDRTData.strEngineRev.c_str());//������ת��
					v["AIRINPUT"] = TCharToANSI(vtRTData[i].sOBDRTData.strMAF.c_str());//������
					v["SCP"] = TCharToANSI(vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());//��ѹѹ��
					v["OILWEAR"] = TCharToANSI(vtRTData[i].sOBDRTData.strFuelConsumption.c_str());//������
					v["NOMIC"] = TCharToANSI(vtRTData[i].sOBDRTData.strNOXConcentration.c_str());//����������Ũ��
					v["UREAJET"] = TCharToANSI(vtRTData[i].sOBDRTData.strUreaInjectionVolume.c_str());//����������
					v["GASTEMP"] = TCharToANSI(vtRTData[i].sOBDRTData.strEGT.c_str());//�����¶�
					v["DCPD"] = TCharToANSI(vtRTData[i].sOBDRTData.strDPFDifferentialPressure.c_str());//����������ѹ��
					v["EGRO"] = TCharToANSI(vtRTData[i].sOBDRTData.strEGRPosition.c_str());//EGR ����
					v["OILJETP"] = TCharToANSI(vtRTData[i].sOBDRTData.strFuelDeliveryPressure.c_str());//ȼ������ѹ��
					
					vOBD["body"].append(v);
				}
			}
			break;
		}
	}
	
	//wchar_t *pwchOBD=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(vOBD).c_str());
	
	CString strOBDSourceData = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(vOBD).c_str());
	strOBDSourceData.Replace(L"\r\n", L"");//ȥ���س�����
	strOBDSourceData.Replace(L"\n", L"");//ȥ������
	//strOBDSourceData.Replace(L" ", L"");//ȥ�ո�
	strOBDSourceData = strOBDSourceData.Mid(8,strOBDSourceData.GetLength());//ȥ��{"body":
	strOBDSourceData = strOBDSourceData.Mid(0,strOBDSourceData.GetLength()-1);//ȥ��}
	/********************************************************************/

	
	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"EndRecordAndUpload";

	//strOBDSourceData = L"["+strOBDSourceData+L"]";

	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrResultData = strXml.GetString();
	std::wstring wstrSourceData = strSourceData.GetString();
	std::wstring wstrSourceData2 = strSourceData2.GetString();//��������(����)
	std::wstring wstrOBDSourceData = strOBDSourceData.GetString();//OBD��������
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.ResultData = &wstrResultData;
	send.SourceData = &wstrSourceData;
	send.SourceDataMS = &wstrSourceData2;//��������(����)
	send.OBDSourceData = &wstrOBDSourceData;//OBD��������
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;

	CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload����jsonParam:"),send.jsonParam->c_str());

	CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload���͹�������:"),send.SourceData->c_str());

	CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload���ͽ������:"),send.ResultData->c_str());

	CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload����OBD��������:"),send.OBDSourceData->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUploadʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("EndRecordAndUpload"),_T("���ýӿ�ʧ��"));
		return false;
	}
	return true;
}
//�ϴ���������UploadSourceData (û��)
bool WebServiceLibAPI::UploadProcessData(int nTestType)
{
	if (!m_bUsed)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);;

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	if(reader.parse(str,root,true))
	{
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		CString str(L"");
		if(_wtoi(sTestLog.wchTestType) == 1)
		{
			str = L"2";
		}
		else if(_wtoi(sTestLog.wchTestType) == 2)
		{
			str = L"3";
		}
		else if(_wtoi(sTestLog.wchTestType) == 3)
		{
			str = L"4";
		}
		else if(_wtoi(sTestLog.wchTestType) == 4)
		{
			str = L"1";
		}
		else if(_wtoi(sTestLog.wchTestType) == 5)
		{
			str = L"5";
		}
		else if(_wtoi(sTestLog.wchTestType) == 6)
		{
			str = L"6";
		}
		root["TestType"] = TCharToANSI(str);
		root["License"] = TCharToANSI(sTestLog.wchPlateNumber);
		CString strPlateType(L"");
		if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"1";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		root["LicenseType"] = TCharToANSI(strPlateType);
		//root["LicenseCode"] = TCharToANSI(m_strVehicleType);
		root["LicenseCode"] = TCharToANSI(SetVehicleType(sTestLog.wchReserved1));
	}
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(root.toStyledString().c_str());
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());

	CString strSourceData(L"<Source_Data>");
	CString strTemp(L"");
	switch(nTestType)
	{
		case DIS:
		{
			list<REALTIMEDATAOFDIS> listRealTimeDataOFDis;
			GetIniRealTimeDataOfDIS(listRealTimeDataOFDis);
			for(list<REALTIMEDATAOFDIS>::iterator ir=listRealTimeDataOFDis.begin(); ir!=listRealTimeDataOFDis.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
				strTemp.Empty();
				//0-70%�ת�١�1-�ߵ���׼����2-�ߵ��ټ�⡢3-����׼����4-���ټ��
				// ��ⲽ�����: 0-���δ��ʼ; 1-�ȳ�; 2-��һ������ͬ��;3-�ߵ��ٲ���; 4-�ڶ�������ͬ��; 5-���ٲ���; 6-���;7-�ߵ���ת���ٵȴ�; 8-���
				if(_wtoi(ir->wchReserved2) == 7)
				{
					strTemp = L"1";
				}
				else if(_wtoi(ir->wchReserved2) == 8)
				{
					strTemp = L"0";
				}
				else
				{
					strTemp.Format(L"%d",_wtoi(ir->wchReserved2) -1);
				}
				strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strTemp.Empty();
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<HC>%s</HC>",ir->wchHC);
				strSourceData.AppendFormat(L"<CO>%s</CO>",ir->wchCO);
				strSourceData.AppendFormat(L"<O2>%s</O2>",ir->wchO2);
				strSourceData.AppendFormat(L"<CO2>%s</CO2>",ir->wchCO2);
				strSourceData.AppendFormat(L"<LANBDAMP>%s</LANBDAMP>",ir->wchLambda);
				strSourceData.AppendFormat(L"<RATE>%s</RATE>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>",_wtoi(ir->wchReserved1) == 1 ? L"0" : L"1");//���ݱ�־:0-������1-����
				strSourceData.AppendFormat(L"</Data>");
			}
		}
		break;

		case ASM:
		//{
		//	list<REALTIMEDATAOFASM> listRealTimeDataOFASM;
		//	GetIniRealTimeDataOfASM(listRealTimeDataOFASM);

		//	for(list<REALTIMEDATAOFASM>::iterator ir=listRealTimeDataOFASM.begin(); ir!=listRealTimeDataOFASM.end(); ++ir)
		//	{
		//		strSourceData.AppendFormat(L"<Data>");
		//		strTemp.Format(L"%s", ir->wchSamplingTime);
		//		//strTemp.Replace(L"-",NULL);
		//		//strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp);
		//		strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
		//		strTemp.Empty();
		//		if(_wtoi(ir->wchTime)>0 && _wtoi(ir->wchTime) <= 90)
		//		{
		//			strTemp = L"1";
		//		}
		//		else if(_wtoi(ir->wchTime)>90 && _wtoi(ir->wchTime) <= 180)
		//		{
		//			strTemp = L"2";
		//		}
		//		strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
		//		strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
		//		strSourceData.AppendFormat(L"<REALSPEED>%s</REALSPEED>",ir->wchVelocity);
		//		strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);
		//		strSourceData.AppendFormat(L"<TORQUE>%s</TORQUE>",ir->wchForce);
		//		strSourceData.AppendFormat(L"<REALLOADPOWER>%s</REALLOADPOWER>",ir->wchPower);
		//		strSourceData.AppendFormat(L"<DLOAD>%.2f</DLOAD>",_wtof(ir->wchForce)/9.8);
		//		strSourceData.AppendFormat(L"<COTEST>%s</COTEST>",ir->wchCO);
		//		strSourceData.AppendFormat(L"<CO2TEST>%s</CO2TEST>",ir->wchCO2);
		//		strSourceData.AppendFormat(L"<HCTEST>%s</HCTEST>",ir->wchHC);
		//		strSourceData.AppendFormat(L"<NOTEST>%s</NOTEST>",ir->wchNO);
		//		strSourceData.AppendFormat(L"<O2TEST>%s</O2TEST>",ir->wchO2);
		//		strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-����
		//		strSourceData.AppendFormat(L"</Data>");
		//	}
		//}
		break;

		case VMAS:
		{
			// ��ȡRealTimeDataOfVMAS.ini·��
			wchar_t wchPath[MAX_PATH];	
			if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfVMAS.ini", wchPath))
			{
			}
			CSimpleIni sRealTimeDataOFVMAS(wchPath);
			// ��ȡ������
			int nTotal = _wtoi(sRealTimeDataOFVMAS.GetString(L"Sum",L"Total",L"0"));	

			list<REALTIMEDATAOFVMAS> listRealTimeDataOFVMAS;
			GetIniRealTimeDataOfVMAS(listRealTimeDataOFVMAS);

			for(list<REALTIMEDATAOFVMAS>::iterator ir=listRealTimeDataOFVMAS.begin(); ir!=listRealTimeDataOFVMAS.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
				strTemp.Empty();
				//0-�豸׼����1-���ǰ����׼����2-195 ������
				CString strState;
				if(nTotal > 195)
				{
					int nTime;
					nTime = nTotal - 195;
					if(_wtoi(ir->wchTime) > nTime)
					{
						strState = L"2";
					}
					else
					{
						strState = L"1";
					}
				}
				else
				{
					strState = L"2";
				}
				strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strState);
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<HCREAL>%s</HCREAL>",ir->wchHC);
				strSourceData.AppendFormat(L"<COREAL>%s</COREAL>",ir->wchCO);
				strSourceData.AppendFormat(L"<CO2REAL>%s</CO2REAL>",ir->wchCO2);
				strSourceData.AppendFormat(L"<NOREAL>%s</NOREAL>",ir->wchNO);
				strSourceData.AppendFormat(L"<ENVIROO2REAL>%s</ENVIROO2REAL>",ir->wchO2OfEnvironment);
				//
				strSourceData.AppendFormat(L"<ANALYSISO2REAL>%s</ANALYSISO2REAL>",ir->wchO2);
				strSourceData.AppendFormat(L"<FLOWO2REAL>%s</FLOWO2REAL>",ir->wchO2OfFlowmeter);
				strSourceData.AppendFormat(L"<HCQUALITY>%s</HCQUALITY>",ir->wchHCm);
				strSourceData.AppendFormat(L"<COQUALITY>%s</COQUALITY>",ir->wchCOm);
				strSourceData.AppendFormat(L"<NOXQUALITY>%s</NOXQUALITY>",ir->wchNOm);
				//
				strSourceData.AppendFormat(L"<STANDARDSPEED>%s</STANDARDSPEED>",ir->wchVelocity);
				strSourceData.AppendFormat(L"<REALSPEED>%s</REALSPEED>",ir->wchVelocity);
				strSourceData.AppendFormat(L"<TORQUE>%s</TORQUE>",ir->wchForce);
				strSourceData.AppendFormat(L"<ROALPOWER>%s</ROALPOWER>",ir->wchPower);
				strSourceData.AppendFormat(L"<TEMPERATURE>%s</TEMPERATURE>",ir->wchEnvironmentalTemperature);
				strSourceData.AppendFormat(L"<RELATIVEHUMIDITY>%s</RELATIVEHUMIDITY>",ir->wchRelativeHumidity);
				strSourceData.AppendFormat(L"<AIRPRESSURE>%s</AIRPRESSURE>",ir->wchAtmosphericPressure);
				strSourceData.AppendFormat(L"<FLOWTEMPERATURE>%s</FLOWTEMPERATURE>",ir->wchTemperatureOfFlowmeter);
				strSourceData.AppendFormat(L"<FLOWPRESSURE>%s</FLOWPRESSURE>",ir->wchPressureOfFlowmeter);
				strSourceData.AppendFormat(L"<REALVOLUME>%s</REALVOLUME>",ir->wchActualFluxOfGas);
				strSourceData.AppendFormat(L"<STANDARDVOLUME>%s</STANDARDVOLUME>",ir->wchStandardFluxOfGas);
				strSourceData.AppendFormat(L"<HUMIDITYCORRECT>%s</HUMIDITYCORRECT>",ir->wchHumidityCorrectionFactor);
				strSourceData.AppendFormat(L"<DILUTIONCORRECT>%s</DILUTIONCORRECT>",ir->wchDilutionCorrectionFactor);
				strSourceData.AppendFormat(L"<DILUTIONRATE>%s</DILUTIONRATE>",ir->wchDR);
				strSourceData.AppendFormat(L"<ANALYSISPRESSURE>%s</ANALYSISPRESSURE>",ir->wchPressureOfFlowmeter);
				strSourceData.AppendFormat(L"<GASREALFLOW>%s</GASREALFLOW>",ir->wchFluxOfExhaust);
				strSourceData.AppendFormat(L"<LAMBDA>%s</LAMBDA>",ir->wchLambda);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>",L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData.AppendFormat(L"</Data>");
			}

		}
		break;

		case LUGDOWN:
		{
			list<REALTIMEDATAOFLUGDOWN> listRealTimeDataOfLUGDOWN;
			GetIniRealTimeDataOfLUGDOWN(listRealTimeDataOfLUGDOWN);
			int n100(0);
			for(list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN.begin(); ir!=listRealTimeDataOfLUGDOWN.end(); ++ir)
			{
				int nState = _wtoi(ir->wchState);
				if (3== nState)
				{
					n100++;
				}
				else if (3 < nState)
				{
					break;
				}
			}

			CString strTemp(L"");
			for(list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN.begin(); ir!=listRealTimeDataOfLUGDOWN.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Mid(0,19));//ȥ������
				strTemp.Empty();
				if(_wtoi(ir->wchState)<=2)
				{
					strTemp = L"0";
				}
				else if(_wtoi(ir->wchState)==3 &&n100>12)//��¼100%�׶�11��ǰ����Ϊ%100�ָ�
				{
					n100--;
					strTemp = L"1";
				}
				else if(_wtoi(ir->wchState)==3)
				{
					strTemp = L"2";
				}
				else if(_wtoi(ir->wchState)==4)
				{
					strTemp = L"3";
				}
				else if(_wtoi(ir->wchState)==5||_wtoi(ir->wchState)>5)
				{
					strTemp = L"4";
				}
				strSourceData.AppendFormat(L"<WORKTYPE>%s</WORKTYPE>",strTemp);
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<REALSPEED>%s</REALSPEED>",ir->wchVelocity);
				strSourceData.AppendFormat(L"<TORQUE>%s</TORQUE>",ir->wchForce);
				strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<DLOAD>%s</DLOAD>",ir->wchPower);
				strSourceData.AppendFormat(L"<LIGHTABSORB>%s</LIGHTABSORB>",ir->wchK);
				strSourceData.AppendFormat(L"<NOx>%d</NOx>",_wtoi(ir->wchNO)+_wtoi(ir->wchNO2));
				strSourceData.AppendFormat(L"<CO2>%s</CO2>",ir->wchCO2);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����
				strSourceData.AppendFormat(L"</Data>");
			}
		}
		break;

		case FSUNHT:
		{
			list<REALTIMEDATAOFFSUNHT> listRealTimeDataOfFSUNHT;
			GetIniRealTimeDataOfFSUNHT(listRealTimeDataOfFSUNHT);

			for(list<REALTIMEDATAOFFSUNHT>::iterator ir = listRealTimeDataOfFSUNHT.begin();ir != listRealTimeDataOfFSUNHT.end(); ++ir)
			{
				strSourceData.AppendFormat(L"<Data>");
				strTemp.Format(L"%s", ir->wchSamplingTime);
				strSourceData.AppendFormat(L"<ALLSEQUENCE>%s</ALLSEQUENCE>",strTemp.Left(19));
				strTemp.Empty();
				strSourceData.AppendFormat(L"<WORKTYPE>%d</WORKTYPE>",_wtoi(ir->wchOrder)-3);
				strSourceData.AppendFormat(L"<SAMPLESEQUENCE>%s</SAMPLESEQUENCE>",ir->wchTime);
				strSourceData.AppendFormat(L"<OPACITYSMOKE>%s</OPACITYSMOKE>",ir->wchK);
				strSourceData.AppendFormat(L"<ENGINESPEED>%s</ENGINESPEED>",ir->wchEngineRev);
				strSourceData.AppendFormat(L"<DATASIGN>%s</DATASIGN>", L"0");//���ݱ�־:0-������1-���2-Ʈ�㣬3-����   ����׶���ɲ��ϴ���ʼ�ź�
				strSourceData.AppendFormat(L"</Data>");
			}
		}
		break;
	}
	strSourceData = strSourceData + L"</Source_Data>";

	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"UploadSourceData";
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrSourceData = strSourceData.GetString();
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.SourceData = &wstrSourceData;
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;

	CNHLogAPI::WriteLog(_T("������־"), _T("UploadSourceData����"),send.jsonParam->c_str());
	CNHLogAPI::WriteLog(_T("������־"), _T("UploadSourceData��������"),send.SourceData->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("UploadSourceData"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("UploadSourceData�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("UploadSourceDataʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("UploadSourceData"),_T("���ýӿ�ʧ��"));
		return false;
	}
	return true;
}
/*****************************************************************************/

//�ϴ�OBD�������UploadOBD
bool WebServiceLibAPI::UploadOBDTestData()
{
	if (!m_bUsed)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);

	LINEINFO sLineinfo;
	GetIniLineInfo(&sLineinfo);

	USERINFO SUserInfo;
	GetIniUserInfo(&SUserInfo);


	TESTLOG sTestLog2;
	GetIniTestLog(&sTestLog2);

	VEHICLEINFO SVehicleInfo;
	GetIniVehicleInfo(&SVehicleInfo);

	SResultOfOBD sResultOfOBD;
	GetIniResultOfOBD(&sResultOfOBD);
	CString strSql;
	strSql.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", sTestLog2.wchRunningNumber);
	GetDboResultOfOBD(strSql ,&sResultOfOBD);
	//

	TESTLOG sTestLog;
	wchar_t wchSql[SQLMAX];
	wsprintf(wchSql, L"select * from TestLog where RunningNumber = '%s'",  sTestLog2.wchRunningNumber);
	GetDboTestLog(wchSql, &(sTestLog));

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	if(reader.parse(str,root,true))
	{
		root["LineNo"] = TCharToANSI(sLineinfo.wchLineNumber);
		root["License"] = TCharToANSI(sTestLog.wchPlateNumber);
		CString strPlateType(L"");
		if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"1";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		root["LicenseType"] = TCharToANSI(strPlateType);
		//root["LicenseCode"] = TCharToANSI(m_strVehicleType);//�� GA24.7
		root["LicenseCode"] = TCharToANSI(SetVehicleType(sTestLog.wchReserved1));//�� GA24.7
		CString strFuelType(L"");
		strFuelType.Format(L"%s",sTestLog.wchFuelType);
		//if(strFuelType.Find(L"��") != -1)
		if(L"1" == strPlateType || L"5" == strPlateType)//1-���͡����硢��Һ������ 2-���͡���硢��Һ������
		{
			strFuelType = L"1";
		}
		else
		{
			strFuelType = L"2";
		}
		root["Type"] = TCharToANSI(strFuelType);
		root["TestTimes"] = TCharToANSI(L"1");
		//root["TestPerson"] = TCharToANSI(sTestLog.wchOperator);
		root["TestPerson"] = TCharToANSI(SUserInfo.wchName);
		
		COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 1, 32);
		odt1 = odt - odts;

		root["TsBeginTime"] = TCharToANSI(odt1.Format(L"%Y-%m-%d %H:%M:%S"));
		root["TsEndTime"] = TCharToANSI(odt.Format(L"%Y-%m-%d %H:%M:%S"));
		//root["ECU_CALID"] = TCharToANSI(sResultOfOBD.strEngineCALID.c_str());
		//root["ECU_CVN"] = TCharToANSI(sResultOfOBD.strEngineCVN.c_str());
		//root["SCR_CALID"] = TCharToANSI(sResultOfOBD.strPostProcessingCALID.c_str());
		//root["SCR_CVN"] = TCharToANSI(sResultOfOBD.strPostProcessingCVN.c_str());
		//root["OCU_CALID"] = TCharToANSI(sResultOfOBD.strOtherCALID.c_str());
		//root["OCU_CVN"] = TCharToANSI(sResultOfOBD.strOtherCVN.c_str());
		//root["VIN"] = TCharToANSI(sResultOfOBD.strVIN.c_str());
		root["VIN"] = TCharToANSI(sTestLog.wchVIN);
	}
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(root.toStyledString().c_str());
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	strjsonParam.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonParam.Replace(L"\n", L"");//ȥ������
	//strjsonParam.Replace(L" ", L"");//ȥ�ո�

	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"UploadOBD";

	// �������
	const char* strRet = "{}";
	Json::Value vResult;
	CString strTemp;
	if(reader.parse(strRet,vResult,true))
	{
		if (wcscmp(sTestLog.wchItemOBD, L"4") == 0)
		{
			vResult["vin"] = TCharToANSI(sTestLog.wchVIN);// ����ʶ�����
			if(0 != wcscmp(L"", sResultOfOBD.strOBDType.c_str()))
			{
				if(0 == wcscmp(L"EOBD", sResultOfOBD.strOBDType.c_str()))
				{
					strTemp = L"1";
				}
				else if(0 == wcscmp(L"OBDII", sResultOfOBD.strOBDType.c_str()))
				{
					strTemp = L"2";
				}
				else if(0 == wcscmp(L"CN-OBD-6", sResultOfOBD.strOBDType.c_str()))
				{
					strTemp = L"3";
				}
				else if(0 == wcscmp(L"JOBD", sResultOfOBD.strOBDType.c_str()))
				{
					strTemp = L"4";
				}
				else if(0 == wcscmp(L"EOBD+OBDII", sResultOfOBD.strOBDType.c_str()))
				{
					strTemp = L"5";
				}
				else if(0 == wcscmp(L"����", sResultOfOBD.strOBDType.c_str()))
				{
					strTemp = L"6";
				}
			}
			else
			{
				strTemp = L"3";
			}
			vResult["vehOBDRequire"] = TCharToANSI(strTemp);// ���� OBD Ҫ��:1��EOBD��2��OBDII��3��CN-OBD-6,4-JOBD��5-EOBD+OBDII��6-����
			vResult["odometer"] = TCharToANSI(sResultOfOBD.strMileage.c_str());// �ۻ���ʻ���
			
			strTemp = (0==wcscmp(L"1",sResultOfOBD.strLampStateJudge.c_str()) ? L"Y":L"N");
			vResult["faultIndicatorStatus"] = TCharToANSI(strTemp);// OBD ����ָʾ���Ƿ����� :Y/N
			strTemp = (0==wcscmp(L"1",sResultOfOBD.strComErrorJudge.c_str()) ? L"Y":L"N");
			vResult["communicationStatus"] = TCharToANSI(strTemp);// OBD ͨ���Ƿ�ɹ���Y/N			
			vResult["communicationFailReason"] = TCharToANSI(sResultOfOBD.strComErrorComment.c_str());// ���ɹ�ԭ��:1���ӿ���2���Ҳ����ӿ�3�����Ӻ���ͨ��
			strTemp = (0==wcscmp(L"1", sResultOfOBD.strComErrorJudge.c_str()) ? L"Y":L"N");
			vResult["remoteEmissionDeviceCommunicationStatus"] = TCharToANSI(strTemp);// Զ���ŷŹ������ն�ͨ���Ƿ�����Y/N
			strTemp = (0==wcscmp(L"2", sResultOfOBD.strLampStateJudge.c_str()) ? L"Y":L"N");
			vResult["faultLightStatus"] = TCharToANSI(sResultOfOBD.strLampStateJudge.c_str());// ���ϵ��Ƿ����Y/N			
			strTemp = (0==wcscmp(L"2",sResultOfOBD.strDTCJudge.c_str()) ? L"Y":L"N");
			vResult["isExistsFaultCode"] = TCharToANSI(strTemp);// �Ƿ���ڹ�����Y/N
			
			CString strMFT, strFST,  strCC, strCCH, strOS, strOSH, strEECS, strEGR, strSAIS, strICM, strVVT, strDOC, strSCR, strDPF, strASC, strPOC;
			CString strIsFinish;
			strMFT = sResultOfOBD.strCheckItemJudge_MFT.c_str();
			strFST = sResultOfOBD.strCheckItemJudge_FST.c_str();
			strCC = sResultOfOBD.strCheckItemJudge_CC.c_str();
			strCCH = sResultOfOBD.strCheckItemJudge_CCH.c_str();
			strOS = sResultOfOBD.strCheckItemJudge_OS.c_str();
			strOSH = sResultOfOBD.strCheckItemJudge_OSH.c_str();
			strEECS = sResultOfOBD.strCheckItemJudge_EECS.c_str();
			strEGR = sResultOfOBD.strCheckItemJudge_EGR.c_str();
			strSAIS = sResultOfOBD.strCheckItemJudge_SAIS.c_str();
			strICM = sResultOfOBD.strCheckItemJudge_ICM.c_str();
			strVVT = sResultOfOBD.strCheckItemJudge_VVT.c_str();
			strDOC = sResultOfOBD.strCheckItemJudge_DOC.c_str();
			strSCR = sResultOfOBD.strCheckItemJudge_SCR.c_str();
			strDPF = sResultOfOBD.strCheckItemJudge_DPF.c_str();
			strASC = sResultOfOBD.strCheckItemJudge_ASC.c_str();
			strPOC = sResultOfOBD.strCheckItemJudge_POC.c_str();
			if(L"2" == strMFT || L"2" == strFST || L"2" == strCC || L"2" == strCCH
		    || L"2" == strOS || L"2" == strOSH || L"2" == strEECS || L"2" == strEGR
			|| L"2" == strSAIS || L"2" == strICM || L"2" == strVVT || L"2" == strDOC
			|| L"2" == strSCR || L"2" == strDPF || L"2" == strASC || L"2" == strPOC)
			{
				strIsFinish = L"Y";
			}
			else
			{
				strIsFinish = L"N";
			}

			vResult["isExistsReadyStatusUndone"] = TCharToANSI(strIsFinish);// �Ƿ��о���״̬δ�����Y/N

			// ���Ƶ�Ԫ
			{
				
				Json::Value v1;
				v1["no"] = TCharToANSI(L"1");//���
				v1["name"] = TCharToANSI(L"���������Ƶ�Ԫ");//���Ƶ�Ԫ���ƣ������ܳ�
				v1["CALID"] = TCharToANSI(sResultOfOBD.strEngineCALID.c_str());//���Ƶ�Ԫ CALID
				v1["CVN"] = TCharToANSI(sResultOfOBD.strEngineCVN.c_str());//���Ƶ�Ԫ CVN
				vResult["controlUnit"].append(v1);
				
				Json::Value v2;
				v2["no"] = TCharToANSI(L"2");//���
				v2["name"] = TCharToANSI(L"������Ƶ�Ԫ");//���Ƶ�Ԫ���ƣ������ܳ�
				v2["CALID"] = TCharToANSI(sResultOfOBD.strPostProcessingCALID.c_str());//���Ƶ�Ԫ CALID
				v2["CVN"] = TCharToANSI(sResultOfOBD.strPostProcessingCVN.c_str());//���Ƶ�Ԫ CVN
				vResult["controlUnit"].append(v2);
				
				Json::Value v3;
				v3["no"] = TCharToANSI(L"3");//���
				v3["name"] = TCharToANSI(L"�������Ƶ�Ԫ");//���Ƶ�Ԫ���ƣ������ܳ�
				v3["CALID"] = TCharToANSI(sResultOfOBD.strOtherCALID.c_str());//���Ƶ�Ԫ CALID
				v3["CVN"] = TCharToANSI(sResultOfOBD.strOtherCVN.c_str());//���Ƶ�Ԫ CVN
				vResult["controlUnit"].append(v3);
			}
			// ������
			{
				Json::Value v;
				v["no"] = TCharToANSI(L"1");//���
				//v["code"] = TCharToANSI(L"��");//���ϴ���
				//v["message"] = TCharToANSI(L"��");//������Ϣ
				//v["odometer"] = TCharToANSI(L"0");//��������ʻ���
				v["code"] = TCharToANSI(sResultOfOBD.strDTC.c_str());//���ϴ���
				v["message"] = TCharToANSI(sResultOfOBD.strDTCComment.c_str());//������Ϣ
				v["odometer"] = TCharToANSI(sResultOfOBD.strDTCMileage.c_str());//��������ʻ���

				vResult["faultCode"].append(v);
			}
			// δ������Ŀ
			{
				Json::Value v;
				//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����				
				if(L"2" != strEGR)
				{
					v["no"] = TCharToANSI(L"1");//���
					v["indicatorDescription"] = TCharToANSI(L"5");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
					v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
					if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_EGR.c_str()))
					{
						//strTemp = L"δ����";
						strTemp = L"N";
					}
					else
					{
						//strTemp = L"�Ѿ���";
						strTemp = L"Y";
					}
					v["readyStatus"] = TCharToANSI(strTemp);//����״̬
					vResult["readyStatusUndone"].append(v);
				}
				if(0 == wcscmp(L"����", sResultOfOBD.strFuelType.c_str()))
				{
					// ���ͣ�SCR��POC��DOC,MFT��DPF��EGR
					if(L"2" != strSCR)
					{
						v["no"] = TCharToANSI(L"2");//���
						v["indicatorDescription"] = TCharToANSI(L"1");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_SCR.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}

					if(L"2" != strPOC)
					{
						v["no"] = TCharToANSI(L"3");//���
						v["indicatorDescription"] = TCharToANSI(L"2");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_POC.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}

					if(L"2" != strDOC)
					{
						v["no"] = TCharToANSI(L"4");//���
						v["indicatorDescription"] = TCharToANSI(L"2");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_DOC.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}

					if(L"2" != strDPF)
					{
						v["no"] = TCharToANSI(L"5");//���
						v["indicatorDescription"] = TCharToANSI(L"4");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_DPF.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}

					if(L"2" != strMFT)
					{
						v["no"] = TCharToANSI(L"6");//���
						v["indicatorDescription"] = TCharToANSI(L"6");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_MFT.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}
				}
				else
				{
					// ���ͣ�CC��OS��OSH��EGR
					if(L"2" != strCC)
					{
						v["no"] = TCharToANSI(L"2");//���
						v["indicatorDescription"] = TCharToANSI(L"6");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"CC");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_CC.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}
					
					if(L"2" != strOS)
					{
						v["no"] = TCharToANSI(L"3");//���
						v["indicatorDescription"] = TCharToANSI(L"6");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"OS");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_OS.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}
					
					if(L"2" != strOSH)
					{
						v["no"] = TCharToANSI(L"4");//���
						v["indicatorDescription"] = TCharToANSI(L"6");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
						v["OtherIndicatorDescription"] = TCharToANSI(L"OSH");//�����������������
						if(0 == wcscmp(L"2", sResultOfOBD.strCheckItemJudge_OSH.c_str()))
						{
							//strTemp = L"δ����";
							strTemp = L"N";
						}
						else
						{
							//strTemp = L"�Ѿ���";
							strTemp = L"Y";
						}
						v["readyStatus"] = TCharToANSI(strTemp);//����״̬
						vResult["readyStatusUndone"].append(v);
					}
				}
			}
			// IUPR
			{
				/*�����Ŀ����:1-NMHC �߻�����2-NOX �߻�����3-NOX ��������4-������������
				5-������������6-ERG �� VVT��7-��ѹѹ����8-�߻����� 1��9-�߻����� 2��
				10-ǰ���������� 1��11-ǰ���������� 2��12-������������ 1��13-������������ 2��
				14-EVAP��15-EGR ��VVT��16-GPF �� 1��17-GPF �� 2��18-���ο�������ϵͳ
				*/
				CString strJccs,strFhcs,strRate;
				float fRate;
				for(int i = 1; i <= 18; i ++)
				{
					Json::Value v;

					strTemp.Format(L"%d", i);
					v["no"] = TCharToANSI(strTemp);//���
					v["item"] = TCharToANSI(strTemp);//�����Ŀ����
					
					switch(i)
					{
					case 1://NMHC �߻���
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_NMHCC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_NMHCEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 2://NOX �߻���
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_NOXCC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_NOXCEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 3://NOX ������
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_NOXAC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_NOXAEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 4://����������
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_PMC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_PMEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 5://����������
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_WSC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_WSEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 6://ERG �� VVT
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_EVAPC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_EVAPEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 7://��ѹѹ��
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_PPC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_PPEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 8://�߻�����
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_CMCCB1.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_CMCECB1.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 9://�߻����� 
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_CMCCB2.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_CMCECB2.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 10://ǰ����������
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_O2SMCCB1.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_O2SMCECB1.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 11://ǰ���������� 
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_O2SMCCB2.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_O2SMCECB2.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 12://������������
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_RO2SMCCB1.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_RO2SMCECB1.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 13://������������
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_RO2SMCCB2.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_RO2SMCECB2.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 14://EVAP
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_EVAPC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_EVAPEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 15://EGR ��VVT
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_EGRC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_EGREC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 16://GPF ��
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_GPFC1.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_GPFEC1.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 17://GPF ��
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_GPFC2.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_GPFEC2.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					case 18://���ο�������ϵͳ
						{
							strJccs.Format(L"%s", sResultOfOBD.strIUPR_AMCCC.c_str());
							strFhcs.Format(L"%s", sResultOfOBD.strIUPR_AMCEC.c_str());
							if(L"��֧��" == strJccs || L"" == strJccs)
							{
								strJccs.Format(L"0");
							}
							if(L"��֧��" == strFhcs || L"" == strFhcs)
							{
								strFhcs.Format(L"0");
							}
							if(0 != _wtoi(strFhcs))
							{
								fRate = _wtoi(strJccs)/(_wtoi(strFhcs))*1.0f;
							}
							else
							{
								fRate = 0.0f;
							}
							strRate.Format(L"%.2f", fRate);
							v["finishCount"] = TCharToANSI(strJccs);//�����ɴ���
							v["matchCount"] = TCharToANSI(strFhcs);//���ϼ����������
							v["IUPRRate"] = TCharToANSI(strRate);//IUPR ��
							v["igniteCount"] = TCharToANSI(sResultOfOBD.strIUPR_ICC.c_str());//������
						}
						break;
					}
					vResult["IUPR"].append(v);// IUPR
				}
			}
			// ����֡
			{
				Json::Value v;
				v["no"] = TCharToANSI(L"1");//���
				v["ff"] = TCharToANSI(sResultOfOBD.strFreezeData.c_str());//����֡
				v["ds"] = TCharToANSI(sResultOfOBD.strFreezeData.c_str());//������
				vResult["freezeFrame"].append(v);
			}

			vResult["obdDeviceId"] = TCharToANSI(sLineinfo.wchOBDAdapterNumber);// OBD �豸 ID

			bool bNeedTest = IsNeedTestOBD(SVehicleInfo);
			if(bNeedTest)
			{
				vResult["Result"] = TCharToANSI(sResultOfOBD.strJudge.c_str());//0-���ϸ�1-�ϸ� 2-��� 3-�� OBD
			}
			else
			{
				vResult["Result"] = TCharToANSI(L"3");//0-���ϸ�1-�ϸ� 2-��� 3-�� OBD			
			}
		
		}
		else 
		{
			// ��obd
			vResult["vin"] = TCharToANSI(L"");// ����ʶ�����
			vResult["vehOBDRequire"] = TCharToANSI(L"");// ���� OBD Ҫ��:1��EOBD��2��OBDII��3��CN-OBD-6
			vResult["odometer"] = TCharToANSI(L"");// �ۻ���ʻ���
			vResult["faultIndicatorStatus"] = TCharToANSI(L"");// OBD ����ָʾ���Ƿ����� :Y/N
			vResult["communicationStatus"] = TCharToANSI(L"");// OBD ͨ���Ƿ�ɹ���Y/N
			vResult["communicationFailReason"] = TCharToANSI(L"");// ���ɹ�ԭ��:1���ӿ���2���Ҳ����ӿ�3�����Ӻ���ͨ��4������
			vResult["remoteEmissionDeviceCommunicationStatus"] = TCharToANSI(L"");// Զ���ŷŹ������ն�ͨ���Ƿ�����Y/N
			vResult["faultLightStatus"] = TCharToANSI(L"");// ���ϵ��Ƿ����Y/N
			vResult["isExistsFaultCode"] = TCharToANSI(L"");// �Ƿ���ڹ�����Y/N
			vResult["isExistsReadyStatusUndone"] = TCharToANSI(L"");// �Ƿ��о���״̬δ�����Y/N

			// controlUnit���Ƶ�Ԫ
			{
				Json::Value v;
				v["no"] = TCharToANSI(L"");//���
				v["name"] = TCharToANSI(L"");//���Ƶ�Ԫ���ƣ������ܳ�
				v["CALID"] = TCharToANSI(L"");//���Ƶ�Ԫ CALID
				v["CVN"] = TCharToANSI(L"");//���Ƶ�Ԫ CVN
				vResult["controlUnit"].append(v);// controlUnit
			}
			// faultCode������
			{
				Json::Value v;
				v["no"] = TCharToANSI(L"");//���
				v["code"] = TCharToANSI(L"");//���ϴ���
				v["message"] = TCharToANSI(L"");//������Ϣ
				v["odometer"] = TCharToANSI(L"");//��������ʻ���

				vResult["faultCode"].append(v);
			}
			// readyStatusUndoneδ������Ŀ
			{
				Json::Value v;
				v["no"] = TCharToANSI(L"");//���
				v["indicatorDescription"] = TCharToANSI(L"");//�������������:1-SCR��2-POC��3-DOC��4-DPF��5-EGR��6-����
				v["OtherIndicatorDescription"] = TCharToANSI(L"");//�����������������
				v["readyStatus"] = TCharToANSI(L"");//����״̬

				vResult["readyStatusUndone"].append(v);
			}
			// IUPR
			{
				Json::Value v;
				/*�����Ŀ����:1-NMHC �߻�����2-NOX �߻�����3-NOX ��������4-������������
				5-������������6-ERG �� VVT��7-��ѹѹ����8-�߻����� 1��9-�߻����� 2��
				10-ǰ���������� 1��11-ǰ���������� 2��12-������������ 1��13-������������ 2��
				14-EVAP��15-EGR ��VVT��16-GPF �� 1��17-GPF �� 2��18-���ο�������ϵͳ
				*/
				v["no"] = TCharToANSI(L"");//���
				v["item"] = TCharToANSI(L"");//�����Ŀ����
				v["finishCount"] = TCharToANSI(L"");//�����ɴ���
				v["matchCount"] = TCharToANSI(L"");//���ϼ����������
				v["IUPRRate"] = TCharToANSI(L"");//IUPR ��
				v["igniteCount"] = TCharToANSI(L"");//������

				vResult["IUPR"].append(v);// IUPR
			}
			// freezeFrame
			{
				Json::Value v;
				v["no"] = TCharToANSI(L"");//���
				v["ff"] = TCharToANSI(L"");//����֡
				v["ds"] = TCharToANSI(L"");//������
				
				vResult["freezeFrame"].append(v);// freezeFrame
			}
			vResult["obdDeviceId"] = TCharToANSI(sLineinfo.wchOBDAdapterNumber);//OBD �豸 ID
			if(_wtoi(SVehicleInfo.wchHasOBD) == 0)
			{
				strTemp = L"3";
			}
			else
			{
				strTemp = L"2";
			}
			vResult["Result"] = TCharToANSI(strTemp);//0-���ϸ�1-�ϸ� 2-��� 3-�� OBD
		}

	}
	Json::FastWriter jsonWriterRet;
	//wchar_t *pwchResult=CNHCommonAPI::ANSIToUnicode();
	CString strjsonResult = CNHCommonAPI::ANSIToUnicode(jsonWriterRet.write(vResult).c_str());
	strjsonResult.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonResult.Replace(L"\n", L"");//ȥ������
	//strjsonResult.Replace(L" ", L"");//ȥ�ո�

	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrResultData = strjsonResult.GetString();
	//std::wstring wstrSourceData = strSourceData.GetString();
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.ResultData = &wstrResultData;
	//send.SourceData = &wstrSourceData;
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;

	CNHLogAPI::WriteLog(_T("������־"), _T("UploadOBD����"),send.jsonParam->c_str());

	CNHLogAPI::WriteLog(_T("������־"), _T("UploadOBD���ͽ������:"),send.ResultData->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("UploadOBD"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		//if(reader1.parse(TCharToANSI(resp.CommonResult->c_str()),root1))
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("UploadOBD�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("UploadOBDʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("UploadOBD"),_T("���ýӿ�ʧ��"));
		return false;
	}
	return true;
}
//�ϴ�ȼ����������UploadFuelEvaporation
bool WebServiceLibAPI::UploadFuelEvaporationData(bool bAirInlet, bool bAirOutlet, bool bPass)
{
	if (!m_bUsed)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);;

	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);
	LINEINFO sLineinfo;
	GetIniLineInfo(&sLineinfo);

	USERINFO SUserInfo;
	GetIniUserInfo(&SUserInfo);
	 //�������
	CString strXml(L"<Result_Data>");
	
	strXml.AppendFormat(L"<OILENTRYTEST>%s</OILENTRYTEST>",bAirInlet?L"1":L"0");//���Ϳڲ��� :0-���ϸ�1-�ϸ�
	strXml.AppendFormat(L"<FUELTANKCAPTEST>%s</FUELTANKCAPTEST>",bAirOutlet?L"1":L"0");//����ǲ��� :0-���ϸ�1-�ϸ�
	strXml.AppendFormat(L"<RESULT>%s</RESULT>",bPass?L"1":L"0");//����ж� :0-���ϸ�1-�ϸ�

	strXml = strXml + L"</Result_Data>" ;

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	if(reader.parse(str,root,true))
	{
		root["LineNo"] = TCharToANSI(sLineinfo.wchLineNumber);
		root["License"] = TCharToANSI(sTestLog.wchPlateNumber);
		CString strPlateType(L"");
		if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"1";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		root["LicenseType"] = TCharToANSI(strPlateType);
		//root["LicenseCode"] = TCharToANSI(m_strVehicleType);//�� GA24.7
		root["LicenseCode"] = TCharToANSI(SetVehicleType(sTestLog.wchReserved1));//�� GA24.7
		//CString strFuelType(L"");
		//strFuelType.Format(L"%s",sTestLog.wchFuelType);
		//if(strFuelType.Find(L"��") != -1)
		//{
		//	strFuelType = L"1";
		//}
		//else
		//{
		//	strFuelType = L"2";
		//}
		//root["Type"] = TCharToANSI(strFuelType);
		root["TestTimes"] = TCharToANSI(sTestLog.wchNumberOfTestPeriod);
		//root["TestPerson"] = TCharToANSI(sTestLog.wchOperator);
		root["TestPerson"] = TCharToANSI(SUserInfo.wchName);
		root["TsBeginTime"] = TCharToANSI(sTestLog.wchTestStartTime);
		root["TsEndTime"] = TCharToANSI(sTestLog.wchTestEndTime);
	}
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	strjsonParam.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonParam.Replace(L"\n", L"");//ȥ������
	//strjsonParam.Replace(L" ", L"");//ȥ�ո�

	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"UploadFuelEvaporation";
	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();
	std::wstring wstrResultData = strXml.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.ResultData = &wstrResultData;
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;

	CNHLogAPI::WriteLog(_T("������־"), _T("UploadFuelEvaporation����"),send.jsonParam->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("UploadFuelEvaporation"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		//if(reader1.parse(TCharToANSI(resp.CommonResult->c_str()),root1))
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("UploadFuelEvaporation�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("UploadFuelEvaporationʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("UploadFuelEvaporation"),_T("���ýӿ�ʧ��"));
		return false;
	}
	return true;
}

/*******************************�Լ첿��******************************************/
//�Լ쿪ʼ��¼��BeginSelfTestRecord
bool WebServiceLibAPI::BeginSelfTestRecord(CString strTestType)
{
	if (!m_bUsed/* || !m_bSelfCheckUsedVideo*/)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);;

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	if(reader.parse(str,root,true))
	{
		LINEINFO sLineInfo;
		GetIniLineInfo(&sLineInfo);
		root["LineNo"] = TCharToANSI(sLineInfo.wchLineNumber);
		/*�Լ����ͣ�
		31:���ػ��м��(������):����Ƶ�ʣ�ÿ�쿪���Լ�
		32:�������ʻ��м��(������):����Ƶ�ʣ����ػ��м������ͨ������Ҫ��������
		33:�������ʻ��м��(������):����Ƶ�ʣ����ػ��м������ͨ������Ҫ��������
		34�����ػ��м��(������):����Ƶ�ʣ�ÿ�쿪���Լ�
		42:й©���������:����Ƶ�ʣ�ÿ�쿪���Լ�
		43:�����ǵ�������:����Ƶ�ʣ�ÿ�쿪���Լ�
		44:�����������:����Ƶ�ʣ�������ĵͱ�����鲻ͨ�����������ͨ���߱���У׼�����
		45:�����������̼��:����Ƶ�ʣ�ÿ�쿪���Լ�
		*46:���������Ի����:����Ƶ�ʣ������鲻ͨ��ʱ����Ҫ�������Ի������飬Ȼ����������
		51:NOX���������������:����Ƶ�ʣ�ÿ�쿪���Լ�
		52:NOX�����ǵ�������:����Ƶ�ʣ�ÿ�쿪���Լ�
		53:NOX�����������:����Ƶ�ʣ�������ĵͱ�����鲻ͨ�����������ͨ���߱���У׼�����
		*54:NOx������ת���ʺ˲飺����Ƶ�ʣ�7 ����Ȼ��
		*55:NOx���������Ի����:����Ƶ�ʣ������鲻ͨ��ʱ����Ҫ�������Ի������飬Ȼ����������
		61:�����Ƽ��������:����Ƶ�ʣ�ÿ�쿪���Լ�
		71:�̶ȼƼ��������:����Ƶ�ʣ�ÿ�쿪���Լ�
		*72:�̶ȼƲ�͸��ȼ��:����Ƶ�ʣ�ÿ�μ��ǰ���
		81:ת�ټƼ��:����Ƶ�ʣ��������
		91:�����������:����Ƶ�ʣ��������
		*/
		root["TestType"] = TCharToANSI(strTestType);
	}

	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(root.toStyledString().c_str());
	Json::FastWriter jsonWriter;
	 //wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	strjsonParam.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonParam.Replace(L"\n", L"");//ȥ������
	//strjsonParam.Replace(L" ", L"");//ȥ�ո�

	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"BeginSelfTestRecord";
	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;
	
	CNHLogAPI::WriteLog(_T("������־"), _T("BeginSelfTestRecord���ͷ�����"),send.methodName->c_str());
	CNHLogAPI::WriteLog(_T("������־"), _T("BeginSelfTestRecord����jsonParam"),send.jsonParam->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("BeginSelfTestRecord����"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		//if(reader1.parse(TCharToANSI(resp.CommonResult->c_str()),root1))
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("BeginSelfTestRecord�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T("BeginSelfTestRecordʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T("BeginSelfTestRecord"),_T("���ýӿ�ʧ��"));
		return false;
	}
	return true;
}
/* �Լ������ֹͣ¼�� EndSelfTestRecordAndUpload
* strTestType: �����Ŀ
* strResult: �����
* strXml: �ϴ�xml��Ϣ ,�б�����ʱ�ļ��ģ��˲�����NULL
*/
bool WebServiceLibAPI::EndSelfTestRecordAndUpload(CString strTestType, bool bResult, CString strResultData, CString strProcessData)
{
	if (!m_bUsed/* || !m_bSelfCheckUsedVideo*/)
	{
		return true;
	}
	DataInterfaceSoapProxy webService;
	soap_set_mode(&webService, SOAP_C_UTFSTRING);
	webService.soap_endpoint = CNHCommonAPI::UnicodeToANSI(m_strURL);;

	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;
	
	CString strResultXml;
	
	if(reader.parse(str,root,true))
	{
		LINEINFO sLineInfo;
		GetIniLineInfo(&sLineInfo);
		root["LineNo"] = TCharToANSI(sLineInfo.wchLineNumber);
		/*�Լ����ͣ�
		31:���ػ��м��(������):����Ƶ�ʣ�ÿ�쿪���Լ�
		32:�������ʻ��м��(������):����Ƶ�ʣ����ػ��м������ͨ������Ҫ��������
		33:�������ʻ��м��(������):����Ƶ�ʣ����ػ��м������ͨ������Ҫ��������
		34�����ػ��м��(������):����Ƶ�ʣ�ÿ�쿪���Լ�
		42:й©���������:����Ƶ�ʣ�ÿ�쿪���Լ�
		43:�����ǵ�������:����Ƶ�ʣ�ÿ�쿪���Լ�
		44:�����������:����Ƶ�ʣ�������ĵͱ�����鲻ͨ�����������ͨ���߱���У׼�����
		45:�����������̼��:����Ƶ�ʣ�ÿ�쿪���Լ�
		*46:���������Ի����:����Ƶ�ʣ������鲻ͨ��ʱ����Ҫ�������Ի������飬Ȼ����������
		51:NOX���������������:����Ƶ�ʣ�ÿ�쿪���Լ�
		52:NOX�����ǵ�������:����Ƶ�ʣ�ÿ�쿪���Լ�
		53:NOX�����������:����Ƶ�ʣ�������ĵͱ�����鲻ͨ�����������ͨ���߱���У׼�����
		*54:NOx������ת���ʺ˲飺����Ƶ�ʣ�7 ����Ȼ��
		*55:NOx���������Ի����:����Ƶ�ʣ������鲻ͨ��ʱ����Ҫ�������Ի������飬Ȼ����������
		61:�����Ƽ��������:����Ƶ�ʣ�ÿ�쿪���Լ�
		71:�̶ȼƼ��������:����Ƶ�ʣ�ÿ�쿪���Լ�
		*72:�̶ȼƲ�͸��ȼ��:����Ƶ�ʣ�ÿ�μ��ǰ���
		81:ת�ټƼ��:����Ƶ�ʣ��������
		91:�����������:����Ƶ�ʣ��������
		*/
		root["TestType"] = TCharToANSI(strTestType);

		strResultXml.Format(L"<Result_Data><Data>");
		CString strJudge;
		switch(_wtoi(strTestType))
		{
		case 31://���ػ��м��(������)
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 34://���ػ��м��(������):����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 32://�������ʻ��м��:������:����Ƶ�ʣ����ػ��м������ͨ������Ҫ��������
			{
				DEMARCATIONRESULTOFDYNPLHP sResultOfDYNPLHP;
				GetIniResultOfDynPLHP(&sResultOfDYNPLHP);
				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 32);
				odt1 = odt - odts;

				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<SLIDE5030>%s</SLIDE5030>",sResultOfDYNPLHP.wchPLHPTime0);
				strResultXml.AppendFormat(L"<LOSS40>%s</LOSS40>",sResultOfDYNPLHP.wchPLHP0);
				strResultXml.AppendFormat(L"<SPEED50>%s</SPEED50>",sResultOfDYNPLHP.wchNomialVelocity0);
				strResultXml.AppendFormat(L"<SPEED30>%s</SPEED30>",sResultOfDYNPLHP.wchNomialVelocity1);
				strResultXml.AppendFormat(L"<LOSS40JUDGE>%s</LOSS40JUDGE>",L"1");
				strResultXml.AppendFormat(L"<SLIDE3515>%s</SLIDE3515>",sResultOfDYNPLHP.wchPLHPTime1);
				strResultXml.AppendFormat(L"<LOSS25>%s</LOSS25>",sResultOfDYNPLHP.wchPLHP1);
				strResultXml.AppendFormat(L"<SLIDE3515NOMINAL>%s</SLIDE3515NOMINAL>",sResultOfDYNPLHP.wchNomialVelocity2);
				strResultXml.AppendFormat(L"<SLIDE3515POWER>%s</SLIDE3515POWER>",sResultOfDYNPLHP.wchNomialVelocity3);
				strResultXml.AppendFormat(L"<LOSS25JUDGE>%s</LOSS25JUDGE>",L"1");
				strResultXml.AppendFormat(L"<BASEINERTIA>%s</BASEINERTIA>",sResultOfDYNPLHP.wchDIW);
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOfDYNPLHP.wchPass);

				strJudge.Format(L"%s",sResultOfDYNPLHP.wchPass);
			}
			break;

		case 33://�������ʻ��м��:������:����Ƶ�ʣ����ػ��м������ͨ������Ҫ��������
			{
				DEMARCATIONRESULTOFDYNPLHP sResultOfDYNPLHP;
				GetIniResultOfDynPLHP(&sResultOfDYNPLHP);
				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 32);
				odt1 = odt - odts;

				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));

				strResultXml.AppendFormat(L"<SLIDE10080>%s</SLIDE10080>",sResultOfDYNPLHP.wchPLHPTime0);//100-80km/h ʵ�ʻ���ʱ��
				strResultXml.AppendFormat(L"<LOSS90>%s</LOSS90>",sResultOfDYNPLHP.wchPLHP0);//90km/hʱ�ĸ�����ʧ����
				strResultXml.AppendFormat(L"<LOSS90JUDGE>%s</LOSS90JUDGE>",L"1");//90km/h ʱ������ʧ���:0-���ϸ�1-�ϸ�
				strResultXml.AppendFormat(L"<SLIDE9070>%s</SLIDE9070>",sResultOfDYNPLHP.wchPLHPTime1);//90-70km/h ʵ�ʻ���ʱ��
				strResultXml.AppendFormat(L"<LOSS80>%s</LOSS80>",sResultOfDYNPLHP.wchPLHP1);//80km/hʱ�ĸ�����ʧ����
				strResultXml.AppendFormat(L"<LOSS80JUDGE>%s</LOSS80JUDGE>",L"1");//80km/h ʱ������ʧ���
				strResultXml.AppendFormat(L"<SLIDE8060>%s</SLIDE8060>",sResultOfDYNPLHP.wchPLHPTime2);//80-60km/h ʵ�ʻ���ʱ��
				strResultXml.AppendFormat(L"<LOSS70>%s</LOSS70>",sResultOfDYNPLHP.wchPLHP2);//70km/h ʱ�ĸ�����ʧ����
				strResultXml.AppendFormat(L"<LOSS70JUDGE>%s</LOSS70JUDGE>",L"1");//70km/h ʱ������ʧ���
				strResultXml.AppendFormat(L"<SLIDE7050>%s</SLIDE7050>",sResultOfDYNPLHP.wchPLHPTime3);//70-50km/h ʵ�ʻ���ʱ��
				strResultXml.AppendFormat(L"<LOSS60>%s</LOSS60>",sResultOfDYNPLHP.wchPLHP3);//60km/h ʱ�ĸ�����ʧ����
				strResultXml.AppendFormat(L"<LOSS60JUDGE>%s</LOSS60JUDGE>",L"1");//60km/h ʱ������ʧ���
				strResultXml.AppendFormat(L"<SLIDE6040>%s</SLIDE6040>",sResultOfDYNPLHP.wchPLHPTime4);//60-40km/h ʵ�ʻ���ʱ��
				strResultXml.AppendFormat(L"<LOSS50>%s</LOSS50>",sResultOfDYNPLHP.wchPLHP4);//50km/h ʱ�ĸ�����ʧ����
				strResultXml.AppendFormat(L"<LOSS50JUDGE>%s</LOSS50JUDGE>",L"1");//50km/h ʱ������ʧ���
				strResultXml.AppendFormat(L"<SLIDE5030>%s</SLIDE5030>",sResultOfDYNPLHP.wchPLHPTime5);
				strResultXml.AppendFormat(L"<LOSS40>%s</LOSS40>",sResultOfDYNPLHP.wchPLHP5);
				strResultXml.AppendFormat(L"<LOSS40JUDGE>%s</LOSS40JUDGE>",L"1");
				strResultXml.AppendFormat(L"<SLIDE4020>%s</SLIDE4020>",sResultOfDYNPLHP.wchPLHPTime6);
				strResultXml.AppendFormat(L"<LOSS30>%s</LOSS30>",sResultOfDYNPLHP.wchPLHP6);
				strResultXml.AppendFormat(L"<LOSS30JUDGE>%s</LOSS30JUDGE>",L"1");
				strResultXml.AppendFormat(L"<SLIDE3010>%s</SLIDE3010>",sResultOfDYNPLHP.wchPLHPTime7);
				strResultXml.AppendFormat(L"<LOSS20>%s</LOSS20>",sResultOfDYNPLHP.wchPLHP7);
				strResultXml.AppendFormat(L"<LOSS20JUDGE>%s</LOSS20JUDGE>",L"1");
				strResultXml.AppendFormat(L"<SPEED100>%s</SPEED100>",L"98.0");
				strResultXml.AppendFormat(L"<SPEED90>%s</SPEED90>",sResultOfDYNPLHP.wchNomialVelocity0);
				strResultXml.AppendFormat(L"<SPEED80>%s</SPEED80>",sResultOfDYNPLHP.wchNomialVelocity1);
				strResultXml.AppendFormat(L"<SPEED70>%s</SPEED70>",sResultOfDYNPLHP.wchNomialVelocity2);
				strResultXml.AppendFormat(L"<SPEED60>%s</SPEED60>",sResultOfDYNPLHP.wchNomialVelocity3);
				strResultXml.AppendFormat(L"<SPEED50>%s</SPEED50>",sResultOfDYNPLHP.wchNomialVelocity4);
				strResultXml.AppendFormat(L"<SPEED40>%s</SPEED40>",sResultOfDYNPLHP.wchNomialVelocity5);
				strResultXml.AppendFormat(L"<SPEED30>%s</SPEED30>",sResultOfDYNPLHP.wchNomialVelocity6);
				strResultXml.AppendFormat(L"<SPEED20>%s</SPEED20>",sResultOfDYNPLHP.wchNomialVelocity7);
				strResultXml.AppendFormat(L"<SPEED10>%s</SPEED10>",sResultOfDYNPLHP.wchNomialVelocity8);
				strResultXml.AppendFormat(L"<BASEINERTIA>%s</BASEINERTIA>",sResultOfDYNPLHP.wchDIW);
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOfDYNPLHP.wchPass);
				
				strJudge.Format(L"%s",sResultOfDYNPLHP.wchPass);
			}
			break;

		case 42:// �����ǣ�й©���������:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				wchar_t wchPath[MAX_PATH];
				
				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 0);
				odt1 = odt - odts;
				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));//�������
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));//��鿪ʼʱ��
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));//������ʱ��
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",bResult ? L"1":L"0");//�����
				if(bResult)
				{
					strResultXml.AppendFormat(L"<FAILREMARK>%s</FAILREMARK>",L"");//���ϸ�˵��:ȡ��ϵͳ��������
				}
				else
				{
					strResultXml.AppendFormat(L"<FAILREMARK>%s</FAILREMARK>",L"ȡ��ϵͳ");//���ϸ�˵��:ȡ��ϵͳ��������
				}

				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 43:// �����ǣ���������:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				//DEMARCATIONRESULTOFANAGASCAL sResultOFANAGASCAL;
				//ZeroMemory(&sResultOFANAGASCAL, sizeof(DEMARCATIONRESULTOFANAGASCAL));
				//GetIniResultOfAnaGasCal(&sResultOFANAGASCAL);

				//DEMARCATIONRESULTOFANAGASCHK sResultOFANAGASCHK;
				//ZeroMemory(&sResultOFANAGASCHK, sizeof(DEMARCATIONRESULTOFANAGASCHK));
				//GetIniResultOfAnaGasChk(&sResultOFANAGASCHK);

				//COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				//COleDateTimeSpan odts;
				//odts.SetDateTimeSpan(0, 0, 1, 32);
				//odt1 = odt - odts;
				//strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				//strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				//strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));
				//
				//int nType;
				//if(_wtoi(sResultOFANAGASCAL.wchC3H8StandardValue) <= 1)//����
				//{
				//	nType = 4;
				//	
				//}
				//else if(_wtoi(sResultOFANAGASCAL.wchC3H8StandardValue) <= 200)//�ͱ���
				//{
				//	nType = 3;
				//}
				//else//�߱���
				//{
				//	nType = 5;					
				//}
				//switch(nType)
				//{
				//case 3:// ����
				//	{
				//		strResultXml.AppendFormat(L"<LC3H8>%s</LC3H8>",sResultOFANAGASCAL.wchC3H8StandardValue);
				//		strResultXml.AppendFormat(L"<LCO>%s</LCO>",sResultOFANAGASCAL.wchCOStandardValue);
				//		strResultXml.AppendFormat(L"<LC02>%s</LC02>",sResultOFANAGASCAL.wchCO2StandardValue);
				//		strResultXml.AppendFormat(L"<LNO>%s</LNO>",sResultOFANAGASCAL.wchNO2StandardValue);
				//		strResultXml.AppendFormat(L"<LO2>%s</LO2>",sResultOFANAGASCAL.wchO2StandardValue);
				//		strResultXml.AppendFormat(L"<LHCVALUE>%s</LHCVALUE>",sResultOFANAGASCAL.wchHCMeasuredValue);
				//		strResultXml.AppendFormat(L"<LCOVALUE>%s</LCOVALUE>",sResultOFANAGASCAL.wchCOMeasuredValue);
				//		strResultXml.AppendFormat(L"<LCO2VALUE>%s</LCO2VALUE>",sResultOFANAGASCHK.wchCO2MeasuredValue);
				//		strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				//		strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				//		strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",sResultOFANAGASCHK.wchPass);
				//								
				//		strResultXml.AppendFormat(L"<ZC3H8>%s</ZC3H8>",L"");
				//		strResultXml.AppendFormat(L"<ZCO>%s</ZCO>",L"");
				//		strResultXml.AppendFormat(L"<ZC02>%s</ZC02>",L"");
				//		strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",L"");
				//		strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",L"");
				//		strResultXml.AppendFormat(L"<ZHCVALUE>%s</ZHCVALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZCOVALUE>%s</ZCOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZCO2VALUE>%s</ZCO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",L"");
				//		
				//		strResultXml.AppendFormat(L"<HC3H8>%s</HC3H8>",L"");
				//		strResultXml.AppendFormat(L"<HCO>%s</HCO>",L"");
				//		strResultXml.AppendFormat(L"<HC02>%s</HC02>",L"");
				//		strResultXml.AppendFormat(L"<HNO>%s</HNO>",L"");
				//		strResultXml.AppendFormat(L"<HO2>%s</HO2>",L"");
				//		strResultXml.AppendFormat(L"<HHCVALUE>%s</HHCVALUE>",L"");
				//		strResultXml.AppendFormat(L"<HCOVALUE>%s</HCOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<HCO2VALUE>%s</HCO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"");
				//	}
				//	break;
				//case 4://����
				//	{
				//		strResultXml.AppendFormat(L"<LC3H8>%s</LC3H8>",L"");
				//		strResultXml.AppendFormat(L"<LCO>%s</LCO>",L"");
				//		strResultXml.AppendFormat(L"<LC02>%s</LC02>",L"");
				//		strResultXml.AppendFormat(L"<LNO>%s</LNO>",L"");
				//		strResultXml.AppendFormat(L"<LO2>%s</LO2>",L"");
				//		strResultXml.AppendFormat(L"<LHCVALUE>%s</LHCVALUE>",L"");
				//		strResultXml.AppendFormat(L"<LCOVALUE>%s</LCOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<LCO2VALUE>%s</LCO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",L"");

				//		strResultXml.AppendFormat(L"<ZC3H8>%s</ZC3H8>",sResultOFANAGASCAL.wchC3H8StandardValue);
				//		strResultXml.AppendFormat(L"<ZCO>%s</ZCO>",sResultOFANAGASCAL.wchCOStandardValue);
				//		strResultXml.AppendFormat(L"<ZC02>%s</ZC02>",sResultOFANAGASCAL.wchCO2StandardValue);
				//		strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",sResultOFANAGASCAL.wchNO2StandardValue);
				//		strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",sResultOFANAGASCAL.wchO2StandardValue);
				//		strResultXml.AppendFormat(L"<ZHCVALUE>%s</ZHCVALUE>",sResultOFANAGASCAL.wchHCMeasuredValue);
				//		strResultXml.AppendFormat(L"<ZCOVALUE>%s</ZCOVALUE>",sResultOFANAGASCAL.wchCOMeasuredValue);
				//		strResultXml.AppendFormat(L"<ZCO2VALUE>%s</ZCO2VALUE>",sResultOFANAGASCHK.wchCO2MeasuredValue);
				//		strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				//		strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				//		strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",sResultOFANAGASCHK.wchPass);

				//		strResultXml.AppendFormat(L"<HC3H8>%s</HC3H8>",L"");
				//		strResultXml.AppendFormat(L"<HCO>%s</HCO>",L"");
				//		strResultXml.AppendFormat(L"<HC02>%s</HC02>",L"");
				//		strResultXml.AppendFormat(L"<HNO>%s</HNO>",L"");
				//		strResultXml.AppendFormat(L"<HO2>%s</HO2>",L"");
				//		strResultXml.AppendFormat(L"<HHCVALUE>%s</HHCVALUE>",L"");
				//		strResultXml.AppendFormat(L"<HCOVALUE>%s</HCOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<HCO2VALUE>%s</HCO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"");
				//	}
				//	break;
				//case 5:// ����
				//	{
				//		strResultXml.AppendFormat(L"<LC3H8>%s</LC3H8>",L"");
				//		strResultXml.AppendFormat(L"<LCO>%s</LCO>",L"");
				//		strResultXml.AppendFormat(L"<LC02>%s</LC02>",L"");
				//		strResultXml.AppendFormat(L"<LNO>%s</LNO>",L"");
				//		strResultXml.AppendFormat(L"<LO2>%s</LO2>",L"");
				//		strResultXml.AppendFormat(L"<LHCVALUE>%s</LHCVALUE>",L"");
				//		strResultXml.AppendFormat(L"<LCOVALUE>%s</LCOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<LCO2VALUE>%s</LCO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",L"");

				//		strResultXml.AppendFormat(L"<ZC3H8>%s</ZC3H8>",L"");
				//		strResultXml.AppendFormat(L"<ZCO>%s</ZCO>",L"");
				//		strResultXml.AppendFormat(L"<ZC02>%s</ZC02>",L"");
				//		strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",L"");
				//		strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",L"");
				//		strResultXml.AppendFormat(L"<ZHCVALUE>%s</ZHCVALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZCOVALUE>%s</ZCOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZCO2VALUE>%s</ZCO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",L"");
				//		strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",L"");

				//		strResultXml.AppendFormat(L"<HC3H8>%s</HC3H8>",sResultOFANAGASCAL.wchC3H8StandardValue);
				//		strResultXml.AppendFormat(L"<HCO>%s</HCO>",sResultOFANAGASCAL.wchCOStandardValue);
				//		strResultXml.AppendFormat(L"<HC02>%s</HC02>",sResultOFANAGASCAL.wchCO2StandardValue);
				//		strResultXml.AppendFormat(L"<HNO>%s</HNO>",sResultOFANAGASCAL.wchNO2StandardValue);
				//		strResultXml.AppendFormat(L"<HO2>%s</HO2>",sResultOFANAGASCAL.wchO2StandardValue);
				//		strResultXml.AppendFormat(L"<HHCVALUE>%s</HHCVALUE>",sResultOFANAGASCAL.wchHCMeasuredValue);
				//		strResultXml.AppendFormat(L"<HCOVALUE>%s</HCOVALUE>",sResultOFANAGASCAL.wchCOMeasuredValue);
				//		strResultXml.AppendFormat(L"<HCO2VALUE>%s</HCO2VALUE>",sResultOFANAGASCHK.wchCO2MeasuredValue);
				//		strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				//		strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				//		strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",sResultOFANAGASCHK.wchPass);
				//	}
				//	break;				
				//}
				//
				//strResultXml.AppendFormat(L"<T90CO>%s</T90CO>",L"2");
				//strResultXml.AppendFormat(L"<T10CO>%s</T10CO>",L"2");
				//strResultXml.AppendFormat(L"<T90NO>%s</T90NO>",L"2");
				//strResultXml.AppendFormat(L"<T10NO>%s</T10NO>",L"2");
				//strResultXml.AppendFormat(L"<T90O2>%s</T90O2>",L"2");
				//strResultXml.AppendFormat(L"<T10O2>%s</T10O2>",L"2");

				//strResultXml.AppendFormat(L"<HCREDIFFER>%s</HCREDIFFER>",sResultOFANAGASCHK.wchHCRE);
				//strResultXml.AppendFormat(L"<HCABDIFFER>%s</HCABDIFFER>",sResultOFANAGASCHK.wchHCAE);
				//strResultXml.AppendFormat(L"<COREDIFFER>%s</COREDIFFER>",sResultOFANAGASCHK.wchCORE);
				//strResultXml.AppendFormat(L"<COABDIFFER>%s</COABDIFFER>",sResultOFANAGASCHK.wchCOAE);
				//strResultXml.AppendFormat(L"<CO2REDIFFER>%s</CO2REDIFFER>",sResultOFANAGASCHK.wchCO2RE);
				//strResultXml.AppendFormat(L"<CO2ABDIFFER>%s</CO2ABDIFFER>",sResultOFANAGASCHK.wchCO2AE);
				//strResultXml.AppendFormat(L"<NOREDIFFER>%s</NOREDIFFER>",sResultOFANAGASCHK.wchNO2RE);
				//strResultXml.AppendFormat(L"<NOABDIFFER>%s</NOABDIFFER>",sResultOFANAGASCHK.wchNO2AE);
				//strResultXml.AppendFormat(L"<O2REDIFFER>%s</O2REDIFFER>",sResultOFANAGASCHK.wchO2RE);
				//strResultXml.AppendFormat(L"<O2ABDIFFER>%s</O2ABDIFFER>",sResultOFANAGASCHK.wchO2AE);
				//strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOFANAGASCHK.wchPass);
				//strResultXml.AppendFormat(L"<PEF>%s</PEF>",sResultOFANAGASCHK.wchPEFMeasuredValue);

				//// ��������
				//Json::Value vTemp;
				//if(reader.parse(str,vTemp,true))
				//{
				//	CString strTemp;
				//	list<DEMARCATIONREALTIMEDATAOFANAGASCHK> listRealTimeDataOfAnaGasChk;
				//	GetIniRealTimeDataOfAnaGasChk(listRealTimeDataOfAnaGasChk);
				//	for(list<DEMARCATIONREALTIMEDATAOFANAGASCHK>::iterator ir=listRealTimeDataOfAnaGasChk.begin(); ir!=listRealTimeDataOfAnaGasChk.end(); ++ir)
				//	{
				//		Json::Value v;
				//		if(_wtoi(ir->wchHC) <= 1)//����
				//		{
				//			strTemp = L"4";
				//		}
				//		else if(_wtoi(ir->wchHC) <= 200)//�ͱ���
				//		{
				//			strTemp = L"3";
				//		}
				//		else//�߱���
				//		{
				//			strTemp = L"5";
				//		}
				//		v["testType"] = TCharToANSI(strTemp);//�������:1-���ػ��� 2-������ʧ 3-�����飨�ͱ�����
				//		//4-�����飨������5-�����飨�߱����� 6-����� 7���̶ȼƼ��
				//		v["beginTime"] = TCharToANSI(ir->wchReserved1);//��鿪ʼʱ���ʽΪ YYYY-MM-DD 24h:mm:ss
				//		v["endTime"] = TCharToANSI(ir->wchReserved1);//������ʱ��
				//		v["sequence"] = TCharToANSI(ir->wchTime);//����ʱ��:���룬�� 1 ��ʼ��ÿ������ 1
				//		v["rollSpeed"] = TCharToANSI(L"");//ת���ٶ�,r/min �⹦�����
				//		v["DLoad"] = TCharToANSI(L"");//�⹦�����ظ���,kw���⹦�����
				//		v["HC"] = TCharToANSI(ir->wchHC);//HC Ũ��,10-6�������Ǽ��
				//		v["CO"] = TCharToANSI(ir->wchCO);//CO Ũ��,%�������Ǽ��
				//		v["NO"] = TCharToANSI(ir->wchNO);//NO Ũ��,10-6�������Ǽ�飬NOx �����Ǽ��
				//		v["NO2"] = TCharToANSI(ir->wchNO2);//NO2 Ũ��,10-6�������Ǽ�飬NOx �����Ǽ��
				//		v["CO2"] = TCharToANSI(ir->wchCO2);//CO2 Ũ��,%�������Ǽ�飬NOx �����Ǽ��
				//		v["O2"] = TCharToANSI(ir->wchO2);//O2 Ũ��,%�������Ǽ��
				//		v["N"] = TCharToANSI(L"");//��͸���̶�,m-1���̶ȼƼ��
				//		v["K"] = TCharToANSI(strTemp);//������ϵ��

				//		vTemp["body"].append(v);
				//	}
				//}
				//Json::FastWriter jsonWriter;
				//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(vTemp).c_str());
				//
				//strProcessData = pwch;
				//strProcessData.Replace(L"\r\n", L"");//ȥ���س�����
				//strProcessData.Replace(L"\n", L"");//ȥ������
				//strProcessData.Replace(L" ", L"");//ȥ�ո�
				//strProcessData.Replace(L"{\"body\":",L"");// ȥ��{"body":
				//strProcessData = strProcessData.Mid(0,strProcessData.GetLength()-1);//ȥ��}

				strResultXml.AppendFormat(L"%s", strResultData);
				strJudge.Format(L"%s",bResult? L"1":L"0");
			}
			break;

		case 44:// �����ǣ������:����Ƶ�ʣ�������ĵͱ�����鲻ͨ�����������ͨ���߱���У׼�����
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				strJudge.Format(L"%s",bResult? L"1":L"0");
			}
			break;

		case 45://�����ǣ������̼��:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				DEMARCATIONRESULTOFANAGASCHK sResultOFANAGASCHK;
				ZeroMemory(&sResultOFANAGASCHK, sizeof(DEMARCATIONRESULTOFANAGASCHK));
				GetIniResultOfAnaGasChk(&sResultOFANAGASCHK);

				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 0);
				odt1 = odt - odts;

				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<O2STANDARD>%s</O2STANDARD>",L"20.8");//�������̱�ֵ
				strResultXml.AppendFormat(L"<O2MEASURE>%s</O2MEASURE>",L"20.7");//�������̲���ֵ
				strResultXml.AppendFormat(L"<O2DIFFER>%s</O2DIFFER>",L"0.1");//�����������
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",L"1");//�����:0 -���ϸ�1-�ϸ�

				strJudge.Format(L"%s",bResult? L"1":L"0");
			}
			break;

		case 46://������:���Ի����:����Ƶ�ʣ������鲻ͨ��ʱ����Ҫ�������Ի������飬Ȼ���������� 
			{

				strResultXml.AppendFormat(L"%s", strResultData);
				strJudge.Format(L"%s",bResult? L"1":L"0");
			}
			break;

		case 51:// NOX�����ǣ����������:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				DEMARCATIONRESULTOFANAGASCHK sResultOFANAGASCHK;
				ZeroMemory(&sResultOFANAGASCHK, sizeof(DEMARCATIONRESULTOFANAGASCHK));
				GetIniResultOfAnaGasChk(&sResultOFANAGASCHK);

				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 32);
				odt1 = odt - odts;
				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<N02>%s</N02>",sResultOFANAGASCHK.wchNO2StandardValue);
				strResultXml.AppendFormat(L"<NO>%s</NO>",sResultOFANAGASCHK.wchNOStandardValue);
				strResultXml.AppendFormat(L"<O2>%s</O2>",sResultOFANAGASCHK.wchO2StandardValue);
				strResultXml.AppendFormat(L"<NO2JUDG>%s</NO2JUDG>",sResultOFANAGASCHK.wchNO2MeasuredValue);
				strResultXml.AppendFormat(L"<NOJUDGE>%s</NOJUDGE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				strResultXml.AppendFormat(L"<O2JUDGE>%s</O2JUDGE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				
				strResultXml.AppendFormat(L"<NO2REDIFFER>%s</NO2REDIFFER>",sResultOFANAGASCHK.wchNO2RE);
				strResultXml.AppendFormat(L"<NO2ABDIFFER>%s</NO2ABDIFFER>",sResultOFANAGASCHK.wchNO2AE);
				strResultXml.AppendFormat(L"<NOREDIFFER>%s</NOREDIFFER>",sResultOFANAGASCHK.wchNORE);
				strResultXml.AppendFormat(L"<NOABDIFFER>%s</NOABDIFFER>",sResultOFANAGASCHK.wchNOAE);
				strResultXml.AppendFormat(L"<O2REDIFFER>%s</O2REDIFFER>",sResultOFANAGASCHK.wchO2RE);
				strResultXml.AppendFormat(L"<O2ABDIFFER>%s</O2ABDIFFER>",sResultOFANAGASCHK.wchO2AE);
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOFANAGASCHK.wchPass);

				strJudge.Format(L"%s",sResultOFANAGASCHK.wchPass);
			}
			break;

		case 52:// NOX�����ǣ���������:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				DEMARCATIONRESULTOFANAGASCAL sResultOFANAGASCAL;
				ZeroMemory(&sResultOFANAGASCAL, sizeof(DEMARCATIONRESULTOFANAGASCAL));
				GetIniResultOfAnaGasCal(&sResultOFANAGASCAL);

				DEMARCATIONRESULTOFANAGASCHK sResultOFANAGASCHK;
				ZeroMemory(&sResultOFANAGASCHK, sizeof(DEMARCATIONRESULTOFANAGASCHK));
				GetIniResultOfAnaGasChk(&sResultOFANAGASCHK);

				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 32);
				odt1 = odt - odts;
				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));
				
				int nType;
				if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 1)//����:<1
				{
					nType = 1;					
				}
				else if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 400)//�ͱ���:300
				{
					nType = 2;
				}
				else if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 1000)//�еͱ���:900
				{
					nType = 3;
				}
				else if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 2000)//�иߵͱ���:1800
				{
					nType = 4;
				}
				else//�߱���:3000
				{
					nType = 5;					
				}
				
				//if(nType <= 3)
				{
					// ��Ũ��
					strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",sResultOFANAGASCAL.wchNO2StandardValue);
					strResultXml.AppendFormat(L"<HNO>%s</HNO>",sResultOFANAGASCAL.wchNOStandardValue);
					strResultXml.AppendFormat(L"<HO2>%s</HO2>",sResultOFANAGASCAL.wchO2StandardValue);
					strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",sResultOFANAGASCAL.wchNO2MeasuredValue);
					strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",sResultOFANAGASCAL.wchNOMeasuredValue);
					strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",sResultOFANAGASCAL.wchO2MeasuredValue);
					strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",sResultOFANAGASCHK.wchPass);

					// ��Ũ��
					strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
					strResultXml.AppendFormat(L"<LNO>%s</LNO>",sResultOFANAGASCHK.wchNOStandardValue);
					strResultXml.AppendFormat(L"<LO2>%s</LO2>",sResultOFANAGASCHK.wchO2StandardValue);
					strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
					strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
					strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
					strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",sResultOFANAGASCHK.wchPass);
				}
				//else
				//{
				//	// ��Ũ��
				//	strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",sResultOFANAGASCAL.wchNO2StandardValue);
				//	strResultXml.AppendFormat(L"<HNO>%s</HNO>",sResultOFANAGASCAL.wchNOStandardValue);
				//	strResultXml.AppendFormat(L"<HO2>%s</HO2>",sResultOFANAGASCAL.wchO2StandardValue);
				//	strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",sResultOFANAGASCAL.wchNO2MeasuredValue);
				//	strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",sResultOFANAGASCAL.wchNOMeasuredValue);
				//	strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",sResultOFANAGASCAL.wchO2MeasuredValue);
				//	strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"1");

				//	// ��Ũ��
				//	strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
				//	strResultXml.AppendFormat(L"<LNO>%s</LNO>",sResultOFANAGASCHK.wchNOStandardValue);
				//	strResultXml.AppendFormat(L"<LO2>%s</LO2>",sResultOFANAGASCHK.wchO2StandardValue);
				//	strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
				//	strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				//	strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				//	strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",sResultOFANAGASCHK.wchPass);
				//}

				strResultXml.AppendFormat(L"<T90NOX>%s</T90NOX>",L"1");
				strResultXml.AppendFormat(L"<T10NOX>%s</T10NOX>",L"1");
				strResultXml.AppendFormat(L"<NO2REDIFFER>%s</NO2REDIFFER>",sResultOFANAGASCHK.wchNO2RE);
				strResultXml.AppendFormat(L"<NO2ABDIFFER>%s</NO2ABDIFFER>",sResultOFANAGASCHK.wchNO2AE);
				strResultXml.AppendFormat(L"<NOREDIFFER>%s</NOREDIFFER>",sResultOFANAGASCHK.wchNORE);
				strResultXml.AppendFormat(L"<NOABDIFFER>%s</NOABDIFFER>",sResultOFANAGASCHK.wchNOAE);
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOFANAGASCHK.wchPass);

				strJudge.Format(L"%s",sResultOFANAGASCHK.wchPass);
			}
			break;

		case 53:// NOX�����ǣ��������:����Ƶ�ʣ�������ĵͱ�����鲻ͨ�����������ͨ���߱���У׼�����
			{
				DEMARCATIONRESULTOFANAGASCHK sResultOFANAGASCHK;
				ZeroMemory(&sResultOFANAGASCHK, sizeof(DEMARCATIONRESULTOFANAGASCHK));
				GetIniResultOfAnaGasChk(&sResultOFANAGASCHK);

				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 32);
				odt1 = odt - odts;
				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));

				int nType;
				if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 1)//����:<1
				{
					nType = 1;					
				}
				else if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 400)//�ͱ���:300
				{
					nType = 2;
				}
				else if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 1000)//�еͱ���:900
				{
					nType = 3;
				}
				else if(_wtoi(sResultOFANAGASCHK.wchNOStandardValue) <= 2000)//�иߵͱ���:1800
				{
					nType = 4;
				}
				else//�߱���:3000
				{
					nType = 5;					
				}

				switch(nType)
				{
				case 1:
					{
						//����
						strResultXml.AppendFormat(L"<ZNO2>%s</ZNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
						strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",sResultOFANAGASCHK.wchNOStandardValue);
						strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",sResultOFANAGASCHK.wchO2StandardValue);
						strResultXml.AppendFormat(L"<ZNO2VALUE>%s</ZNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
						strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
						strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
						strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",sResultOFANAGASCHK.wchPass);

						//��Ũ��
						strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",L"");
						strResultXml.AppendFormat(L"<LNO>%s</LNO>",L"");
						strResultXml.AppendFormat(L"<LO2>%s</LO2>",L"");
						strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",L"");
						strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",L"");

						//�е�Ũ��
						strResultXml.AppendFormat(L"<MLNO2>%s</MLNO2>",L"");
						strResultXml.AppendFormat(L"<MLNO>%s</MLNO>",L"");
						strResultXml.AppendFormat(L"<MLO2>%s</MLO2>",L"");
						strResultXml.AppendFormat(L"<MLNO2VALUE>%s</MLNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLNOVALUE>%s</MLNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MLO2VALUE>%s</MLO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLJUDGE>%s</MLJUDGE>",L"");

						// �и�Ũ��
						strResultXml.AppendFormat(L"<MHNO2>%s</MHNO2>",L"");
						strResultXml.AppendFormat(L"<MHNO>%s</MHNO>",L"");
						strResultXml.AppendFormat(L"<MHO2>%s</MHO2>",L"");
						strResultXml.AppendFormat(L"<MHNO2VALUE>%s</MHNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHNOVALUE>%s</MHNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MHO2VALUE>%s</MHO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHJUDGE>%s</MHJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",L"");
						strResultXml.AppendFormat(L"<HNO>%s</HNO>",L"");
						strResultXml.AppendFormat(L"<HO2>%s</HO2>",L"");
						strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",L"");
						strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"");
					}
					break;
				case 2:
					{
						//����
						strResultXml.AppendFormat(L"<ZNO2>%s</ZNO2>",L"");
						strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",L"");
						strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",L"");
						strResultXml.AppendFormat(L"<ZNO2VALUE>%s</ZNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",L"");
						strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
						strResultXml.AppendFormat(L"<LNO>%s</LNO>",sResultOFANAGASCHK.wchNOStandardValue);
						strResultXml.AppendFormat(L"<LO2>%s</LO2>",sResultOFANAGASCHK.wchO2StandardValue);
						strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
						strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
						strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
						strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",sResultOFANAGASCHK.wchPass);

						//�е�Ũ��
						strResultXml.AppendFormat(L"<MLNO2>%s</MLNO2>",L"");
						strResultXml.AppendFormat(L"<MLNO>%s</MLNO>",L"");
						strResultXml.AppendFormat(L"<MLO2>%s</MLO2>",L"");
						strResultXml.AppendFormat(L"<MLNO2VALUE>%s</MLNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLNOVALUE>%s</MLNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MLO2VALUE>%s</MLO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLJUDGE>%s</MLJUDGE>",L"");

						// �и�Ũ��
						strResultXml.AppendFormat(L"<MHNO2>%s</MHNO2>",L"");
						strResultXml.AppendFormat(L"<MHNO>%s</MHNO>",L"");
						strResultXml.AppendFormat(L"<MHO2>%s</MHO2>",L"");
						strResultXml.AppendFormat(L"<MHNO2VALUE>%s</MHNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHNOVALUE>%s</MHNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MHO2VALUE>%s</MHO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHJUDGE>%s</MHJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",L"");
						strResultXml.AppendFormat(L"<HNO>%s</HNO>",L"");
						strResultXml.AppendFormat(L"<HO2>%s</HO2>",L"");
						strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",L"");
						strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"");
					}
					break;
				case 3:
					{
						//����
						strResultXml.AppendFormat(L"<ZNO2>%s</ZNO2>",L"");
						strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",L"");
						strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",L"");
						strResultXml.AppendFormat(L"<ZNO2VALUE>%s</ZNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",L"");
						strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",L"");
						strResultXml.AppendFormat(L"<LNO>%s</LNO>",L"");
						strResultXml.AppendFormat(L"<LO2>%s</LO2>",L"");
						strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",L"");
						strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",L"");

						//�е�Ũ��
						strResultXml.AppendFormat(L"<MLNO2>%s</MLNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
						strResultXml.AppendFormat(L"<MLNO>%s</MLNO>",sResultOFANAGASCHK.wchNOStandardValue);
						strResultXml.AppendFormat(L"<MLO2>%s</MLO2>",sResultOFANAGASCHK.wchO2StandardValue);
						strResultXml.AppendFormat(L"<MLNO2VALUE>%s</MLNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
						strResultXml.AppendFormat(L"<MLNOVALUE>%s</MLNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
						strResultXml.AppendFormat(L"<MLO2VALUE>%s</MLO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
						strResultXml.AppendFormat(L"<MLJUDGE>%s</MLJUDGE>",sResultOFANAGASCHK.wchPass);

						// �и�Ũ��
						strResultXml.AppendFormat(L"<MHNO2>%s</MHNO2>",L"");
						strResultXml.AppendFormat(L"<MHNO>%s</MHNO>",L"");
						strResultXml.AppendFormat(L"<MHO2>%s</MHO2>",L"");
						strResultXml.AppendFormat(L"<MHNO2VALUE>%s</MHNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHNOVALUE>%s</MHNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MHO2VALUE>%s</MHO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHJUDGE>%s</MHJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",L"");
						strResultXml.AppendFormat(L"<HNO>%s</HNO>",L"");
						strResultXml.AppendFormat(L"<HO2>%s</HO2>",L"");
						strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",L"");
						strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"");
					}
					break;
				case 4:
					{
						//����
						strResultXml.AppendFormat(L"<ZNO2>%s</ZNO2>",L"");
						strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",L"");
						strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",L"");
						strResultXml.AppendFormat(L"<ZNO2VALUE>%s</ZNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",L"");
						strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",L"");
						strResultXml.AppendFormat(L"<LNO>%s</LNO>",L"");
						strResultXml.AppendFormat(L"<LO2>%s</LO2>",L"");
						strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",L"");
						strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",L"");

						//�е�Ũ��
						strResultXml.AppendFormat(L"<MLNO2>%s</MLNO2>",L"");
						strResultXml.AppendFormat(L"<MLNO>%s</MLNO>",L"");
						strResultXml.AppendFormat(L"<MLO2>%s</MLO2>",L"");
						strResultXml.AppendFormat(L"<MLNO2VALUE>%s</MLNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLNOVALUE>%s</MLNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MLO2VALUE>%s</MLO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLJUDGE>%s</MLJUDGE>",L"");

						// �и�Ũ��
						strResultXml.AppendFormat(L"<MHNO2>%s</MHNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
						strResultXml.AppendFormat(L"<MHNO>%s</MHNO>",sResultOFANAGASCHK.wchNOStandardValue);
						strResultXml.AppendFormat(L"<MHO2>%s</MHO2>",sResultOFANAGASCHK.wchO2StandardValue);
						strResultXml.AppendFormat(L"<MHNO2VALUE>%s</MHNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
						strResultXml.AppendFormat(L"<MHNOVALUE>%s</MHNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
						strResultXml.AppendFormat(L"<MHO2VALUE>%s</MHO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
						strResultXml.AppendFormat(L"<MHJUDGE>%s</MHJUDGE>",sResultOFANAGASCHK.wchPass);

						//��Ũ��
						strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",L"");
						strResultXml.AppendFormat(L"<HNO>%s</HNO>",L"");
						strResultXml.AppendFormat(L"<HO2>%s</HO2>",L"");
						strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",L"");
						strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",L"");
						strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",L"");
					}
					break;
				case 5:
					{
						//����
						strResultXml.AppendFormat(L"<ZNO2>%s</ZNO2>",L"");
						strResultXml.AppendFormat(L"<ZNO>%s</ZNO>",L"");
						strResultXml.AppendFormat(L"<ZO2>%s</ZO2>",L"");
						strResultXml.AppendFormat(L"<ZNO2VALUE>%s</ZNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZNOVALUE>%s</ZNOVALUE>",L"");
						strResultXml.AppendFormat(L"<ZO2VALUE>%s</ZO2VALUE>",L"");
						strResultXml.AppendFormat(L"<ZJUDGE>%s</ZJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",L"");
						strResultXml.AppendFormat(L"<LNO>%s</LNO>",L"");
						strResultXml.AppendFormat(L"<LO2>%s</LO2>",L"");
						strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",L"");
						strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",L"");
						strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",L"");

						//�е�Ũ��
						strResultXml.AppendFormat(L"<MLNO2>%s</MLNO2>",L"");
						strResultXml.AppendFormat(L"<MLNO>%s</MLNO>",L"");
						strResultXml.AppendFormat(L"<MLO2>%s</MLO2>",L"");
						strResultXml.AppendFormat(L"<MLNO2VALUE>%s</MLNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLNOVALUE>%s</MLNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MLO2VALUE>%s</MLO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MLJUDGE>%s</MLJUDGE>",L"");

						// �и�Ũ��
						strResultXml.AppendFormat(L"<MHNO2>%s</MHNO2>",L"");
						strResultXml.AppendFormat(L"<MHNO>%s</MHNO>",L"");
						strResultXml.AppendFormat(L"<MHO2>%s</MHO2>",L"");
						strResultXml.AppendFormat(L"<MHNO2VALUE>%s</MHNO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHNOVALUE>%s</MHNOVALUE>",L"");
						strResultXml.AppendFormat(L"<MHO2VALUE>%s</MHO2VALUE>",L"");
						strResultXml.AppendFormat(L"<MHJUDGE>%s</MHJUDGE>",L"");

						//��Ũ��
						strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
						strResultXml.AppendFormat(L"<HNO>%s</HNO>",sResultOFANAGASCHK.wchNOStandardValue);
						strResultXml.AppendFormat(L"<HO2>%s</HO2>",sResultOFANAGASCHK.wchO2StandardValue);
						strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
						strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
						strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
						strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",sResultOFANAGASCHK.wchPass);
					}
					break;
				}

				if(nType == 2)
				{
					//��Ũ��ʱ���
					strResultXml.AppendFormat(L"<NO2REDIFFER>%s</NO2REDIFFER>",sResultOFANAGASCHK.wchNO2RE);
					strResultXml.AppendFormat(L"<NO2ABDIFFER>%s</NO2ABDIFFER>",sResultOFANAGASCHK.wchNO2AE);
					strResultXml.AppendFormat(L"<NOREDIFFER>%s</NOREDIFFER>",sResultOFANAGASCHK.wchNORE);
					strResultXml.AppendFormat(L"<NOABDIFFER>%s</NOABDIFFER>",sResultOFANAGASCHK.wchNOAE);
					strResultXml.AppendFormat(L"<O2REDIFFER>%s</O2REDIFFER>",sResultOFANAGASCHK.wchO2RE);
					strResultXml.AppendFormat(L"<O2ABDIFFER>%s</O2ABDIFFER>",sResultOFANAGASCHK.wchO2AE);
				}
				else
				{
					strResultXml.AppendFormat(L"<NO2REDIFFER>%s</NO2REDIFFER>",L"");
					strResultXml.AppendFormat(L"<NO2ABDIFFER>%s</NO2ABDIFFER>",L"");
					strResultXml.AppendFormat(L"<NOREDIFFER>%s</NOREDIFFER>",L"");
					strResultXml.AppendFormat(L"<NOABDIFFER>%s</NOABDIFFER>",L"");
					strResultXml.AppendFormat(L"<O2REDIFFER>%s</O2REDIFFER>",L"");
					strResultXml.AppendFormat(L"<O2ABDIFFER>%s</O2ABDIFFER>",L"");
				}
				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOFANAGASCHK.wchPass);

				strJudge.Format(L"%s",sResultOFANAGASCHK.wchPass);
			}
			break;

		case 54://NOx������:ת���ʺ˲飺����Ƶ�ʣ�7 ����Ȼ��
			{
			}
			break;

		case 55://NOx������:���Ի����:����Ƶ�ʣ������鲻ͨ��ʱ����Ҫ�������Ի������飬Ȼ����������
			{
				DEMARCATIONRESULTOFANAGASCHK sResultOFANAGASCHK;
				ZeroMemory(&sResultOFANAGASCHK, sizeof(DEMARCATIONRESULTOFANAGASCHK));
				GetIniResultOfAnaGasChk(&sResultOFANAGASCHK);

				COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 1, 32);
				odt1 = odt - odts;
				strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));

				//��Ũ��
				strResultXml.AppendFormat(L"<HNO2>%s</HNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
				strResultXml.AppendFormat(L"<HNO>%s</HNO>",sResultOFANAGASCHK.wchNOStandardValue);
				strResultXml.AppendFormat(L"<HO2>%s</HO2>",sResultOFANAGASCHK.wchO2StandardValue);
				strResultXml.AppendFormat(L"<HNO2VALUE>%s</HNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
				strResultXml.AppendFormat(L"<HNOVALUE>%s</HNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				strResultXml.AppendFormat(L"<HO2VALUE>%s</HO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				strResultXml.AppendFormat(L"<HJUDGE>%s</HJUDGE>",sResultOFANAGASCHK.wchPass);

				//��Ũ��
				strResultXml.AppendFormat(L"<LNO2>%s</LNO2>",sResultOFANAGASCHK.wchNO2StandardValue);
				strResultXml.AppendFormat(L"<LNO>%s</LNO>",sResultOFANAGASCHK.wchNOStandardValue);
				strResultXml.AppendFormat(L"<LO2>%s</LO2>",sResultOFANAGASCHK.wchO2StandardValue);
				strResultXml.AppendFormat(L"<LNO2VALUE>%s</LNO2VALUE>",sResultOFANAGASCHK.wchNO2MeasuredValue);
				strResultXml.AppendFormat(L"<LNOVALUE>%s</LNOVALUE>",sResultOFANAGASCHK.wchNOMeasuredValue);
				strResultXml.AppendFormat(L"<LO2VALUE>%s</LO2VALUE>",sResultOFANAGASCHK.wchO2MeasuredValue);
				strResultXml.AppendFormat(L"<LJUDGE>%s</LJUDGE>",sResultOFANAGASCHK.wchPass);

				strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",sResultOFANAGASCHK.wchPass);

				strJudge.Format(L"%s",sResultOFANAGASCHK.wchPass);
			}
			break;

		case 61:// ������:���������:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				//COleDateTime odt(COleDateTime::GetCurrentTime()),odt1;
				//COleDateTimeSpan odts;
				//odts.SetDateTimeSpan(0, 0, 1, 32);
				//odt1 = odt - odts;

				//DEMARCATIONRESULTOFFLODILO2CAL sResultOfFLODILO2CAL;
				//GetIniResultOfFloDilO2Cal(&sResultOfFLODILO2CAL);
				//strResultXml.AppendFormat(L"<BDDATE>%s</BDDATE>",odt.Format(L"%Y-%m-%d"));
				//strResultXml.AppendFormat(L"<BEGINTIME>%s</BEGINTIME>",odt1.Format(L"%Y-%m-%d %H:%M:%S"));
				//strResultXml.AppendFormat(L"<ENDTIME>%s</ENDTIME>",odt.Format(L"%Y-%m-%d %H:%M:%S"));

				//strResultXml.AppendFormat(L"<O2UPSTANDARD>%s</O2UPSTANDARD>",L"20.88");//���������̱�ֵ
				//strResultXml.AppendFormat(L"<O2UPMEASURE>%s</O2UPMEASURE>",L"20.85");//���������̲���ֵ
				//strResultXml.AppendFormat(L"<O2UPDIFFER>%s</O2UPDIFFER>",L"0.03");//�������������
				//strResultXml.AppendFormat(L"<O2DOWNSTANDARD>%s</O2DOWNSTANDARD>",L"20.88");//���������̱�ֵ
				//strResultXml.AppendFormat(L"<O2DOWNMEASURE>%s</O2DOWNMEASURE>",L"20.85");//���������̲���ֵ
				//strResultXml.AppendFormat(L"<O2DOWNDIFFER>%s</O2DOWNDIFFER>",L"0.03");//�������������
				//strResultXml.AppendFormat(L"<TEMPSTANDARD>%s</TEMPSTANDARD>",L"");//�¶ȱ�ֵ
				//strResultXml.AppendFormat(L"<TEMPSURE>%s</TEMPSURE>",L"");//�¶Ȳ���ֵ
				//strResultXml.AppendFormat(L"<TEMPDIFFER>%s</TEMPDIFFER>",L"");//�¶����
				//strResultXml.AppendFormat(L"<JUDGE>%s</JUDGE>",L"1");//�����
				
				strResultXml.AppendFormat(L"%s", strResultData);
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 71:// �̶ȼ�:���������:����Ƶ�ʣ�ÿ�쿪���Լ�
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				strResultXml.AppendFormat(L"<FAILREMARK>%s</FAILREMARK>", L"");//���ϸ�˵��
				
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 72:// �̶ȼ�:��͸��ȼ��:����Ƶ�ʣ�ÿ�μ��ǰ���
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 81:// ת�ټƣ����������:����Ƶ�ʣ��������
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		case 91:// ����վ�����������:����Ƶ�ʣ��������
			{
				strResultXml.AppendFormat(L"%s", strResultData);
				
				strJudge.Format(L"%s",bResult ? L"1":L"0");
			}
			break;

		default:
			break;
		}

		strResultXml.AppendFormat(L"<TESTPERSON>%s</TESTPERSON>",sUserInfo.wchName);//�����
		strResultXml.AppendFormat(L"</Data></Result_Data>");
		root["Judge"] = TCharToANSI(strJudge);
	}
	
	////wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(root.toStyledString().c_str());
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	CString strjsonParam = CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	strjsonParam = L"["+strjsonParam+L"]";
	strjsonParam.Replace(L"\r\n", L"");//ȥ���س�����
	strjsonParam.Replace(L"\n", L"");//ȥ������
	//strjsonParam.Replace(L" ", L"");//ȥ�ո�

	_ns1__Common send;
	_ns1__CommonResponse resp;
	CString strmethodName = L"EndSelfTestRecordAndUpload";
	std::wstring wstrmethodName = strmethodName.GetString();
	std::wstring wstrJsonParam = strjsonParam.GetString();
	std::wstring wstrName = m_strName.GetString();
	std::wstring wstrPassWord = m_strPassWord.GetString();
	std::wstring wstrResultXml = strResultXml.GetString();
	if(0 < strProcessData.GetLength())
	{
		strProcessData = L"["+strProcessData+L"]";
	}
	strProcessData.Replace(L"\r\n", L"");//ȥ���س�����
	strProcessData.Replace(L"\n", L"");//ȥ������
	//strProcessData.Replace(L" ", L"");//ȥ�ո�
	std::wstring wstrSourceData = strProcessData.GetString();

	send.methodName = &wstrmethodName;
	send.jsonParam = &wstrJsonParam;
	send.ResultData = &wstrResultXml;
	send.SourceData = &wstrSourceData;
	send.userName = &wstrName;
	send.passWord = &wstrPassWord;
	
	CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload���ͷ�����"),send.methodName->c_str());
	CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload����jsonParam"),send.jsonParam->c_str());
	CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload����ResultData"),send.ResultData->c_str());
	CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload����SourceData"),send.SourceData->c_str());

	if( SOAP_OK == webService.Common(&send,resp))
	{
		CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload"),resp.CommonResult->c_str());

		CXmlReader xmlReader;
		std::wstring wstrTMP;
		if(xmlReader.Parse(resp.CommonResult->c_str()))
		{
			xmlReader.OpenNode(L"string");
			xmlReader.GetNodeContent(wstrTMP);
		}

		Json::Reader reader1;
		Json::Value root1;
		if(reader1.parse(TCharToANSI(wstrTMP.c_str()),root1))
		{
			CString strSuccess = KANSIToUTF16(root1["Success"].asString().c_str());
			CString strMessage = KANSIToUTF16(root1["Message"].asString().c_str());
			CString strData = KANSIToUTF16(root1["Data"].asString().c_str());

			if(L"true" == strSuccess)
			{
				CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload�ɹ�"), strMessage);
			}
			else
			{
				CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUploadʧ��"), strMessage+L"--"+strData);
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(_T("������־"), _T(" EndSelfTestRecordAndUpload"),_T("���ýӿ�ʧ��"));
		return false;
	}
	
	return true;
}

/*****************************************************************************/
// �ϱ���ʼ����״̬  �˽ӿ�socket���ӱ�����UTF����
bool WebServiceLibAPI::ESToEP(int nNumber, int nOrder)
{
    if (!m_bUsed)
	{
		return true;
	}
	if(!m_bIsUsedSendState)
	{
		return true;
	}
	//CNHNet nhNet;
	//nhNet.ConnectServer(m_strIP, _wtoi(m_strPort));

	const char* str = "{}";
	Json::Reader reader;
	Json::Value root;

	if(reader.parse(str,root,true))
	{
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		LINEINFO sLineinfo;
		GetIniLineInfo(&sLineinfo);
		root["LineNo"] = TCharToUTF8(sLineinfo.wchLineNumber);
		//root["License"] = TCharToANSI(sTestLog.wchPlateNumber);
		root["License"] = TCharToUTF8(sTestLog.wchPlateNumber);
		CString strPlateType(L"");
		if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"1";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		root["LicenseType"] = TCharToUTF8(strPlateType);
		//root["LicenseCode"] = TCharToANSI(m_strVehicleType);//�� GA24.7
		root["LicenseCode"] = TCharToUTF8(SetVehicleType(sTestLog.wchReserved1));//�� GA24.7
		CString strTestType(L"");
		if(0 == wcscmp(L"4",sTestLog.wchTestType))
		{
			strTestType = L"1";
		}
		else if(0 == wcscmp(L"2",sTestLog.wchTestType))
		{
			strTestType = L"3";
		}
		else if(0 == wcscmp(L"3",sTestLog.wchTestType))
		{
			strTestType = L"4";
		}
		else if(0 == wcscmp(L"5",sTestLog.wchTestType))
		{
			strTestType = L"5";
		}
		root["TestType"] = TCharToUTF8(strTestType);
		CString strVIN;
		strVIN.Format(L"%s",sTestLog.wchVIN);
		root["VIN"] = TCharToUTF8(strVIN);
		SYSTEMTIME oleSamplingTime;
		::GetLocalTime(&oleSamplingTime);
		CString strDate;
		strDate.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d",oleSamplingTime.wYear,oleSamplingTime.wMonth,oleSamplingTime.wDay,oleSamplingTime.wHour,oleSamplingTime.wMinute,oleSamplingTime.wSecond,oleSamplingTime.wMilliseconds);
		root["WorkTypeTime"] = TCharToUTF8(strDate);
		//root["WorkType"] = TCharToANSI(L"1");
		CString strOrder;
		//strOrder.Format(L"%d", nOrder);
		//CNHLogAPI::WriteLog(_T("������־"), _T("����ͨ��0"),strOrder);
		if(nOrder < 0)
		{
			nOrder = 0;
		}
		strOrder.Format(L"%d", nOrder);
		//CNHLogAPI::WriteLog(_T("������־"), _T("����ͨ��1"),strOrder);
		root["WorkType"] = TCharToUTF8(strOrder);
	}
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	
	CString strBuf,strHead;
	if (nNumber == 1)
	{
		strHead =L"##B##";
		strBuf.Format(L"%s%s",strHead, CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str()));
	}
	else
	{
		m_nState = 0;
		m_nOrder = -1;
	   strHead =L"##END##";
	   strBuf.Format(L"%s",strHead);
	}
	strBuf.Replace(L"\r\n", L"");//ȥ���س�����
	strBuf.Replace(L"\n", L"");//ȥ������
	//strBuf.Replace(L" ", L"");//ȥ�ո�

	int nLen = WideCharToMultiByte(CP_ACP, 0, strBuf, -1, NULL, 0, NULL, NULL);
	char *pchBuf = new char[nLen+1]();
	WideCharToMultiByte(CP_ACP, 0, strBuf, -1, pchBuf, nLen, NULL, NULL);
	
	if (NULL != pchBuf)
	{
		m_sSocketNet.SendToServer((char *)pchBuf, nLen);
	}
	delete[] pchBuf;
	pchBuf = NULL;
	
	return true;
}
// �����ּ��������������
void WebServiceLibAPI::RecvData(unsigned long sIP,SOCKET sClient,char * pData,unsigned long DataLength)
{
    Json::Reader reader;
	Json::Value root;
	CString strLicense,strLicenseCode,strLicenseType,strTestType,strVIN;
	if(reader.parse(pData,root))
	{
	    strLicense = KANSIToUTF16(root["License"].asString().c_str());
		strLicenseCode =  KANSIToUTF16(root["LicenseCode"].asString().c_str());
	    strLicenseType = KANSIToUTF16(root["LicenseType"].asString().c_str());
	    strTestType = KANSIToUTF16(root["TestType"].asString().c_str());
		strVIN = KANSIToUTF16(root["VIN"].asString().c_str());
	}

	const char* str = "{}";
	Json::Reader write;
	Json::Value v;

	if(write.parse(str,v,true))
	{
	   TESTLOG sTestLog;
	   GetIniTestLog(&sTestLog);
	   LINEINFO sLineinfo;
	   GetIniLineInfo(&sLineinfo);

	   Json::Value CurrentData;
	   CString strTemp;
	   if (L"4" == strTestType)
	   {
		 if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved2) == 7)
		 {
		 	strTemp = L"1";
		 }
		 else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved2) == 8)
		 {
			strTemp = L"0";
		 }
		 else
		 {
			strTemp.Format(L"%d",_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved2) -1);
		 }

		 CurrentData["Item"] = TCharToANSI(L"HC");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchHC);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"CO");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchCO);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"O2");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchCO);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"CO2");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchCO);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);
	   }

	   if (L"5" == strTestType)
	   {
	     strTemp.Format(L"%d",_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchOrder)-3);
		 CurrentData["Item"] = TCharToANSI(L"OPACITYSMOKE");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchK);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);
	   }

	   if (L"2" == strTestType)
	   {
	     CString strState;
		 
		 CurrentData["Item"] = TCharToANSI(L"REALVOLUME");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchActualFluxOfGas);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"FLOWPRESSURE");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchPressureOfFlowmeter);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"FLOWTEMPERATURE");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTemperatureOfFlowmeter);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"ANALYSISO2REAL");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchO2);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"FLOWO2REAL");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchO2OfFlowmeter);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"CO2REAL");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchCO2);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"COREAL");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchCO);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"HCREAL");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchHC);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"NOREAL");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchNO);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(L"2");

		 v["CurrentData"].append(CurrentData);
	   }

	   if (L"3" == strTestType)
	   {
		if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)<=2)
		{
			strTemp = L"0";
		}
		else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)==3)
		{
			strTemp = L"2";
		}
		else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)==4)
		{
			strTemp = L"3";
		}
		else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)==5||_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)>5)
		{
			strTemp = L"4";
		}

		 CurrentData["Item"] = TCharToANSI(L"LIGHTABSORB");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchK);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"TORQUE");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchForce);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"REALSPEED");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchVelocity);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"TEMPERATURE");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchEnvironmentalTemperature);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"RH");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchRelativeHumidity);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);

		 CurrentData["Item"] = TCharToANSI(L"PRESSURE");
		 CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchAtmosphericPressure);
		 CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
		 CurrentData["DataSign"] = TCharToANSI(L"0");
		 CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
		 CurrentData["WorkType"] = TCharToANSI(strTemp);

		 v["CurrentData"].append(CurrentData);
	   }

	   //WebServiceLibAPI::GetInstance().SetVehicleType(sTestLog.wchReserved1);
	   v["License"] = TCharToANSI(sTestLog.wchPlateNumber);
	   //v["LicenseCode"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_strVehicleType);
	   v["LicenseCode"] = TCharToANSI(WebServiceLibAPI::GetInstance().SetVehicleType(sTestLog.wchReserved1));
	   CString strPlateType(L"");
	   if(0 == wcscmp(L"����",sTestLog.wchPlateType))
	   {
			strPlateType = L"1";
	   }
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"2";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"3";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"4";
		}
		else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
		{
			strPlateType = L"5";
		}
		else if(0 == wcscmp(L"������",sTestLog.wchPlateType))
		{
			strPlateType = L"6";
		}
		v["LicenseType"] = TCharToANSI(strPlateType);

		CString strTestType(L"");
		if(0 == wcscmp(L"4",sTestLog.wchTestType))
		{
			strTestType = L"1";
		}
		else if(0 == wcscmp(L"2",sTestLog.wchTestType))
		{
			strTestType = L"3";
		}
		else if(0 == wcscmp(L"3",sTestLog.wchTestType))
		{
			strTestType = L"4";
		}
		else if(0 == wcscmp(L"5",sTestLog.wchTestType))
		{
			strTestType = L"5";
		}
		v["TestType"] = TCharToANSI(strTestType);
	}
	Json::FastWriter jsonWriter;
	//wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
	//char szSend[4079];
	//CString strSend;
	//strSend = ANSIToTChar(jsonWriter.write(root).c_str());
	//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strSend,-1,szSend,sizeof(szSend),NULL,NULL);

	//std::string strSend(jsonWriter.write(root).c_str());
	WebServiceLibAPI Net;
	Net.m_IocpModeSvr.SendMsg(sClient, (char*)jsonWriter.write(root).c_str(), jsonWriter.write(root).length());
}

void  WebServiceLibAPI::TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	WebServiceLibAPI *pThis = (WebServiceLibAPI *)dwUser;

	if(pThis->m_nState != 0)
	{
		pThis->ESToEP(pThis->m_nState, pThis->m_nOrder);
		//CString str;
		//str.Format(L"״̬�룺%d", pThis->m_nOrder);
		//CNHLogAPI::WriteLog(_T("������־"), _T("����ͨ��"),str);
	}

	TRACE("TimePro running...\n");
}

bool WebServiceLibAPI::IsNeedTestOBD(const VehicleInfo &sVehicleInfo)
{

	//��Ҫ����OBD����������
	//a,�������ͳ�2011.7.1����������OBDϵͳ
	//b,�������ͳ�2013.7.1����������OBDϵͳ
	//b,���ͳ�2018.1.1����������OBDϵͳ

	// ��OBD�������OBD
	if (sVehicleInfo.wchHasOBD[0] != L'1')
	{
		return false;
	}
	
	int nMaximumTotalMass = _wtoi(sVehicleInfo.wchMaximumTotalMass);

	COleDateTime odtProductionDate;
	odtProductionDate.ParseDateTime(sVehicleInfo.wchRegistrationDate);
	COleDateTime odtTime1, odtTime2, odtTime3;
	odtTime1.ParseDateTime(L"2011-07-01 00:00:00");
	odtTime2.ParseDateTime(L"2013-07-01 00:00:00");
	odtTime3.ParseDateTime(L"2018-01-01 00:00:00");
	
	bool bDieselVehicle = (wcscmp(sVehicleInfo.wchFuelType, L"����")==0);

	if (bDieselVehicle)
	{
		if (odtProductionDate < odtTime3)
		{
			return false;
		}
	}
	else
	{
		if (nMaximumTotalMass > 3500)
		{
			if (odtProductionDate < odtTime2)
			{
				return false;
			}
		}
		else
		{
			if (odtProductionDate < odtTime1)
			{
				return false;
			}
		}
	}

	return true;

}


//void WebServiceLibAPI::StartNHServer(void)
//{
//	int nPort(10012);
//
//	nPort = _wtoi(m_strPort.c_str());
//	if (nPort<1000 || nPort>65535)
//	{
//		nPort = 10012;
//	}
//
//	// ���������߳�
//	m_NHNetServer.Run(nPort);
//	m_NHNetServer.SetOnReadCallBack(DataSolutionCallBack);
//}
//
//void WebServiceLibAPI::StopNHServer(void)
//{
//	m_NHNetServer.Stop();
//}
//void WebServiceLibAPI::DataSolutionCallBack(const wchar_t* pwchIP, const BYTE* pBytes, const size_t size)
//{
//	// ����IP
//	g_pNWDD->m_strClientIP = pwchIP;
//
//	char* data = (char *)pBytes;
//
//	// ��������
//	char* pchRevData = new char[size+1]();
//	ZeroMemory(pchRevData, size+1);
//	strncpy_s(pchRevData, size+1, data, _TRUNCATE);
//
//	// ��psession���ͻ��˻ط�����;
//
//	wchar_t* pwchData = CNHCommonAPI::ANSIToUnicode(pchRevData);
//
//	// ��¼��־
//	CNHLogAPI::WriteLog(_T("������־"), _T("������"),pchRevData);
//
//	if (pwchData != NULL)
//	{
//		free(pwchData);
//		pwchData = NULL;
//	}
//
//	
//    Json::Reader reader;
//	Json::Value root;
//	CString strLicense,strLicenseCode,strLicenseType,strTestType,strVIN;
//	if(reader.parse(pchRevData,root))
//	{
//	    strLicense = KANSIToUTF16(root["License"].asString().c_str());
//		strLicenseCode =  KANSIToUTF16(root["LicenseCode"].asString().c_str());
//	    strLicenseType = KANSIToUTF16(root["LicenseType"].asString().c_str());
//	    strTestType = KANSIToUTF16(root["TestType"].asString().c_str());
//		strVIN = KANSIToUTF16(root["VIN"].asString().c_str());
//	}
//
//	if(L"" != strLicense)
//	{
//		const char* str = "{}";
//		Json::Reader write;
//		Json::Value v;
//
//		if(write.parse(str,v,true))
//		{
//			TESTLOG sTestLog;
//			GetIniTestLog(&sTestLog);
//			LINEINFO sLineinfo;
//			GetIniLineInfo(&sLineinfo);
//
//			Json::Value CurrentData;
//			CString strTemp;
//			if (L"4" == strTestType)
//			{
//				if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved2) == 7)
//				{
//					strTemp = L"1";
//				}
//				else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved2) == 8)
//				{
//					strTemp = L"0";
//				}
//				else
//				{
//					strTemp.Format(L"%d",_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved2) -1);
//				}
//
//				CurrentData["Item"] = TCharToANSI(L"HC");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchHC);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"CO");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchCO);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"O2");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchCO);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"CO2");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchCO);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchReserved1) == 1 ? L"0" : L"1");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfDIS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//			}
//
//			if (L"5" == strTestType)
//			{
//				strTemp.Format(L"%d",_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchOrder)-3);
//				CurrentData["Item"] = TCharToANSI(L"OPACITYSMOKE");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchK);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfFSUNHT->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//			}
//
//			if (L"2" == strTestType)
//			{
//				CString strState;
//
//				CurrentData["Item"] = TCharToANSI(L"REALVOLUME");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchActualFluxOfGas);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"FLOWPRESSURE");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchPressureOfFlowmeter);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"FLOWTEMPERATURE");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTemperatureOfFlowmeter);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"ANALYSISO2REAL");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchO2);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"FLOWO2REAL");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchO2OfFlowmeter);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"CO2REAL");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchCO2);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"COREAL");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchCO);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"HCREAL");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchHC);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"NOREAL");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchNO);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfVMAS->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(L"2");
//
//				v["CurrentData"].append(CurrentData);
//			}
//
//			if (L"3" == strTestType)
//			{
//				if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)<=2)
//				{
//					strTemp = L"0";
//				}
//				else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)==3)
//				{
//					strTemp = L"2";
//				}
//				else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)==4)
//				{
//					strTemp = L"3";
//				}
//				else if(_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)==5||_wtoi(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchState)>5)
//				{
//					strTemp = L"4";
//				}
//
//				CurrentData["Item"] = TCharToANSI(L"LIGHTABSORB");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchK);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"TORQUE");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchForce);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"REALSPEED");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchVelocity);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"TEMPERATURE");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchEnvironmentalTemperature);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"RH");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchRelativeHumidity);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//
//				CurrentData["Item"] = TCharToANSI(L"PRESSURE");
//				CurrentData["Value"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchAtmosphericPressure);
//				CurrentData["Timestamp"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchSamplingTime);
//				CurrentData["DataSign"] = TCharToANSI(L"0");
//				CurrentData["SampleSequence"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_RealTimeDataOfLUGDOWN->wchTime);
//				CurrentData["WorkType"] = TCharToANSI(strTemp);
//
//				v["CurrentData"].append(CurrentData);
//			}
//
//			//WebServiceLibAPI::GetInstance().SetVehicleType(sTestLog.wchReserved1);
//			v["License"] = TCharToANSI(sTestLog.wchPlateNumber);
//			//v["LicenseCode"] = TCharToANSI(WebServiceLibAPI::GetInstance().m_strVehicleType);
//			v["LicenseCode"] = TCharToANSI(WebServiceLibAPI::GetInstance().SetVehicleType(sTestLog.wchReserved1));
//			CString strPlateType(L"");
//			if(0 == wcscmp(L"����",sTestLog.wchPlateType))
//			{
//				strPlateType = L"1";
//			}
//			else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
//			{
//				strPlateType = L"2";
//			}
//			else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
//			{
//				strPlateType = L"3";
//			}
//			else if(0 == wcscmp(L"����",sTestLog.wchPlateType))
//			{
//				strPlateType = L"4";
//			}
//			v["LicenseType"] = TCharToANSI(strPlateType);
//
//			CString strTestType(L"");
//			if(0 == wcscmp(L"4",sTestLog.wchTestType))
//			{
//				strTestType = L"1";
//			}
//			else if(0 == wcscmp(L"2",sTestLog.wchTestType))
//			{
//				strTestType = L"3";
//			}
//			else if(0 == wcscmp(L"3",sTestLog.wchTestType))
//			{
//				strTestType = L"4";
//			}
//			else if(0 == wcscmp(L"5",sTestLog.wchTestType))
//			{
//				strTestType = L"5";
//			}
//			v["TestType"] = TCharToANSI(strTestType);
//		}
//		Json::FastWriter jsonWriter;
//		wchar_t *pwch=CNHCommonAPI::ANSIToUnicode(jsonWriter.write(root).c_str());
//				
//		CString strBuf;
//		strBuf.Format(L"%s",pwch);
//		int nLen = WideCharToMultiByte(CP_ACP, 0, strBuf, -1, NULL, 0, NULL, NULL);
//		char *pchBuf = new char[nLen+1]();
//		WideCharToMultiByte(CP_ACP, 0, strBuf, -1, pchBuf, nLen, NULL, NULL);
//		m_NHNetServer.Send(pwchIP, (BYTE *)pchBuf, nLen);
//
//		delete[] pchBuf;
//		pchBuf = NULL;
//	}
//
//	// �ͷŶ��ڴ�
//	if (NULL != pchRevData)
//	{
//		delete[] pchRevData;
//		pchRevData = NULL;
//	}
//}