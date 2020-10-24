#include "StdAfx.h"
#include "NSClientLibAPI.h"
#include "NSSocket.h"

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
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

CNSClientLib& CNSClientLib::GetInstance(void)
{
	static std::auto_ptr<CNSClientLib> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CNSClientLib());
	}
	return *(auto_ptr_instance.get());
}

CNSClientLib::CNSClientLib(void)
{
	srand((unsigned)time(NULL));

	m_bIsUsed = false;
	m_strIP = L"127.0.0.1";
	m_nPort = 8080;
	m_strURL = L"/CtrlCenter/ASM";
		
	wchar_t wchFilePath[MAX_PATH] = {L'\0'};
	if (CNHCommonAPI::GetHLDFilePath(L"Config", L"NSConfig.ini", wchFilePath) == 0x00)
	{
		CSimpleIni si(wchFilePath);
		m_bIsUsed = (si.GetString(L"NSServer", L"IsUsed", L"0") == L"1");
		m_strIP = (LPCTSTR)si.GetString(L"NSServer", L"IP", L"127.0.0.1");
		m_nPort = _wtoi(si.GetString(L"NSServer", L"Port", L"8080"));
		m_strDeviceSN = (LPCTSTR)si.GetString(L"NSServer", L"DeviceSN", L"");
		
		m_strOperatorName = si.GetString(L"NSServer", L"OperatorName", L"");
		m_strOperatorPassword = si.GetString(L"NSServer", L"OperatorPassword", L"");
		m_strDriverName = si.GetString(L"NSServer", L"DriverName", L"");
		m_strDriverPassword = si.GetString(L"NSServer", L"DriverPassword", L"");

		m_strVersion = (LPCTSTR)si.GetString(L"NSServer", L"Version", L"");
		m_strModifyDateTime = (LPCTSTR)si.GetString(L"NSServer", L"ModifyDateTime", L"");
		m_strCertificate = (LPCTSTR)si.GetString(L"NSServer", L"Certificate", L"");
		m_strHardDiskSN = (LPCTSTR)si.GetString(L"NSServer", L"HardDiskSN", L"");
	}
	
	//float fAcc(0);	// 加速度
	//for (int i=1; i<=195; i++)
	//{
	//	m_vtVSTD.push_back(0.00f);
	//}
	//for (int i=1; i<195; i++)
	//{
	//	if (i <= 11)
	//	{
	//		fAcc = 0.0f;
	//	}
	//	else if (i>=12 && i<=15)
	//	{
	//		fAcc = 3.75f;
	//	}
	//	else if (i>=16 && i<=15)
	//	{
	//		fAcc = 3.75f;
	//	}
	//	m_vtVSTD[i] = m_vtVSTD[i-1] + fAcc;
	//}
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(3.7500f);
	m_vtVSTD.push_back(7.5000f);
	m_vtVSTD.push_back(11.2500f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(12.5000f);
	m_vtVSTD.push_back(10.0000f);
	m_vtVSTD.push_back(6.6670f);
	m_vtVSTD.push_back(3.3330f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(3.0000f);
	m_vtVSTD.push_back(6.0000f);
	m_vtVSTD.push_back(9.0000f);
	m_vtVSTD.push_back(12.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(18.4000f);
	m_vtVSTD.push_back(21.8000f);
	m_vtVSTD.push_back(25.2000f);
	m_vtVSTD.push_back(28.6000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(29.2500f);
	m_vtVSTD.push_back(26.5000f);
	m_vtVSTD.push_back(23.7500f);
	m_vtVSTD.push_back(21.0000f);
	m_vtVSTD.push_back(18.2500f);
	m_vtVSTD.push_back(15.5000f);
	m_vtVSTD.push_back(12.7500f);
	m_vtVSTD.push_back(10.0000f);
	m_vtVSTD.push_back(6.6670f);
	m_vtVSTD.push_back(3.3330f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(3.0000f);
	m_vtVSTD.push_back(6.0000f);
	m_vtVSTD.push_back(9.0000f);
	m_vtVSTD.push_back(12.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(15.0000f);
	m_vtVSTD.push_back(17.2200f);
	m_vtVSTD.push_back(19.4400f);
	m_vtVSTD.push_back(21.6700f);
	m_vtVSTD.push_back(23.8900f);
	m_vtVSTD.push_back(26.1100f);
	m_vtVSTD.push_back(28.3300f);
	m_vtVSTD.push_back(30.5500f);
	m_vtVSTD.push_back(32.7800f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(36.8750f);
	m_vtVSTD.push_back(38.7500f);
	m_vtVSTD.push_back(40.6250f);
	m_vtVSTD.push_back(42.5000f);
	m_vtVSTD.push_back(44.3750f);
	m_vtVSTD.push_back(46.2500f);
	m_vtVSTD.push_back(48.1250f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(50.0000f);
	m_vtVSTD.push_back(48.1250f);
	m_vtVSTD.push_back(46.2500f);
	m_vtVSTD.push_back(44.3750f);
	m_vtVSTD.push_back(42.5000f);
	m_vtVSTD.push_back(40.6250f);
	m_vtVSTD.push_back(38.7500f);
	m_vtVSTD.push_back(36.8750f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(35.0000f);
	m_vtVSTD.push_back(33.5000f);
	m_vtVSTD.push_back(32.0000f);
	m_vtVSTD.push_back(28.8570f);
	m_vtVSTD.push_back(25.7140f);
	m_vtVSTD.push_back(22.5710f);
	m_vtVSTD.push_back(19.4280f);
	m_vtVSTD.push_back(16.2850f);
	m_vtVSTD.push_back(13.1420f);
	m_vtVSTD.push_back(10.0000f);
	m_vtVSTD.push_back(6.6670f);
	m_vtVSTD.push_back(3.3330f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	m_vtVSTD.push_back(0.0000f);
	for (int i=0; i<195; i++)
	{
		m_vtVSTDMax.push_back(m_vtVSTD[i]+3.0f);
		m_vtVSTDMin.push_back(m_vtVSTD[i]-3.0f);
	}
}

CNSClientLib::~CNSClientLib(void)
{
}

bool CNSClientLib::SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv)
{
#ifdef DEBUG
	return true;
#endif

	CString strXML = szXML;
	strXML.Replace(L" ", L"%20");	// 转码
	strXML.Replace(L"\r", L"%0d");	// 转码
	strXML.Replace(L"\n", L"%0a");	// 转码

	CString strSend;
	strSend.Format(L" %s?data=%s\r\n", m_strURL.c_str(), strXML);	// 地址前必须有空格

	CNSSocket nsSocket;
	nsSocket.Open(m_strIP.c_str(), m_nPort);
	return nsSocket.SendAndRecvPacket(strSend, strRecv);
}

bool CNSClientLib::OBDGetTime(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDGetTime");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				return true;
			}
		}
	}
	return false;
}

bool CNSClientLib::OBDVerify(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVerify");
	strXML.AppendFormat(L"<Row>");	
	strXML.AppendFormat(L"<User>%s</User>", m_strOperatorName.c_str());
	strXML.AppendFormat(L"<Pwd>%s</Pwd>", m_strOperatorPassword.c_str());
	strXML.AppendFormat(L"<LoginType>%s</LoginType>", L"2");
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				return true;
			}
		}
	}
	return false;
}

bool CNSClientLib::GetOBDVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVehicleList");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	m_vtNetUserInfo.clear();

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"")	// 没有Result节点时有车辆列表
			{
				if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
				{
					do
					{			
						std::map<std::wstring, std::wstring> mapField;	
						xmlReader.EnterNode();
						do
						{
							std::wstring strName, strContent;
							xmlReader.GetNodeName(strName);
							xmlReader.GetNodeContent(strContent);
							mapField[strName] = strContent;
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();

						TESTLOG sTestLog;
						wcscpy_s(sTestLog.wchRunningNumber, mapField[L"OutlookID"].c_str());
						wcscpy_s(sTestLog.wchReportNumber, mapField[L"OutlookID"].c_str());
						wcscpy_s(sTestLog.wchPlateNumber, mapField[L"License"].c_str());

						//0 = 蓝牌
						//1 = 黄牌
						//2 = 白色
						//3 = 黑牌
						//4 = 新能源
						if (mapField[L"LicenseType"] == L"0")
						{
							wcscpy_s(sTestLog.wchPlateType, L"蓝牌");
						}
						else if (mapField[L"LicenseType"] == L"1")
						{
							wcscpy_s(sTestLog.wchPlateType, L"黄牌");
						}
						else if (mapField[L"LicenseType"] == L"3")
						{
							wcscpy_s(sTestLog.wchPlateType, L"黑牌");
						}
						else if (mapField[L"LicenseType"] == L"2")
						{
							wcscpy_s(sTestLog.wchPlateType, L"白色");
						}
						else if (mapField[L"LicenseType"] == L"4")
						{
							wcscpy_s(sTestLog.wchPlateType, L"新能源");
						}
						else
						{
							wcscpy_s(sTestLog.wchPlateType, mapField[L"LicenseType"].c_str());
						}

						//车辆类型
						//0 = 汽油车
						//1 = 柴油车
						//2 = 三轮汽车和低速货车
						//3 = 纯电汽车
						if (mapField[L"VehicleKind"] == L"0")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"汽油车");
						}
						else if (mapField[L"VehicleKind"] == L"1")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"柴油车");
						}
						else if (mapField[L"VehicleKind"] == L"2")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"三轮汽车和低速货车");
						}
						else if (mapField[L"VehicleKind"] == L"3")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"纯电汽车");
						}
						else
						{
							wcscpy_s(sTestLog.wchVehicleType, mapField[L"VehicleKind"].c_str());
						}
						
						wcscpy_s(sTestLog.wchVIN, mapField[L"Vin"].c_str());

						vtTestLog.push_back(sTestLog);
					}
					while (xmlReader.NextSibling());
				}
				return true;
			}
		}
	}
	return false;
}

bool CNSClientLib::GetOBDVehicleInfo(LPCTSTR szVIN, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVehicleInfo");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<License>%s</License>", L"");
	strXML.AppendFormat(L"<LicenseType>%s</LicenseType>", L"");
	strXML.AppendFormat(L"<VIN>%s</VIN>", szVIN);
	strXML.AppendFormat(L"<DateTime>%s</DateTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"")	// 没有Result节点时有车辆信息
			{
				std::wstring strTemp;
				if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
				{
					std::map<std::wstring, std::wstring> mapField;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapField[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();

					TESTLOG sTestLog;
					wcscpy_s(sTestLog.wchPlateNumber, mapField[L"License"].c_str());
					wcscpy_s(sTestLog.wchVIN, mapField[L"VIN"].c_str());
					wcscpy_s(sTestLog.wchRegistrationDate, mapField[L"RegisterDate"].c_str());
															
					wcscpy_s(sTestLog.wchModel, mapField[L"CLXH"].c_str());
					wcscpy_s(sTestLog.wchEngineModel, mapField[L"FDJXH"].c_str());
					wcscpy_s(sTestLog.wchHasOBD, mapField[L"HasOBD"].c_str());

					//0	汽油
					//1	柴油
					//2	液化石油气(LPG)
					//3	天然气(CNG)
					//4	其他
					//6	双燃料
					//8	混合动力
					//9	电
					if (mapField[L"FuelType"] == L"0")
					{
						wcscpy_s(sTestLog.wchFuelType, L"汽油");
					}
					else if (mapField[L"FuelType"] == L"1")
					{
						wcscpy_s(sTestLog.wchFuelType, L"柴油");
					}
					else if (mapField[L"FuelType"] == L"2")
					{
						wcscpy_s(sTestLog.wchFuelType, L"液化石油气(LPG)");
					}
					else if (mapField[L"FuelType"] == L"3")
					{
						wcscpy_s(sTestLog.wchFuelType, L"天然气(CNG)");
					}
					else if (mapField[L"FuelType"] == L"4")
					{
						wcscpy_s(sTestLog.wchFuelType, L"其他");
					}
					else if (mapField[L"FuelType"] == L"6")
					{
						wcscpy_s(sTestLog.wchFuelType, L"双燃料");
					}
					else if (mapField[L"FuelType"] == L"8")
					{
						wcscpy_s(sTestLog.wchFuelType, L"混合动力");
					}
					else if (mapField[L"FuelType"] == L"9")
					{
						wcscpy_s(sTestLog.wchFuelType, L"电");
					}
					else
					{
						wcscpy_s(sTestLog.wchFuelType, mapField[L"FuelType"].c_str());
					}
				}
				return true;
			}
		}
	}
	return false;
}

bool CNSClientLib::Authentication(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"Authentication");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<version>%s</version>", m_strVersion.c_str());
	strXML.AppendFormat(L"<modifydatetime>%s</modifydatetime>", m_strModifyDateTime.c_str());
	strXML.AppendFormat(L"<certificate>%s</certificate>", m_strCertificate.c_str());
	strXML.AppendFormat(L"<Device>%s</Device>", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<hd_seria_number>%s</hd_seria_number>", m_strHardDiskSN.c_str());
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				return true;
			}
		}
	}

	return false;
}

bool CNSClientLib::GetTime(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"GetTime");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				std::wstring strDateTime;
				if (xmlReader.OpenNode(L"Message/Respond/Row/DateTime"))
				{
					xmlReader.GetNodeContent(strDateTime);
				}
				COleDateTime odtNow;
				if (odtNow.ParseDateTime(strDateTime.c_str()))
				{
					SYSTEMTIME st;
					st.wYear = odtNow.GetYear();
					st.wMonth = odtNow.GetMonth();
					st.wDay = odtNow.GetDay();
					st.wHour = odtNow.GetHour();
					st.wMinute = odtNow.GetMinute();
					st.wSecond = odtNow.GetSecond();
					st.wMilliseconds = 0;
					// 本机
					if (SetLocalTime(&st))
					{
						CNHLogAPI::WriteLog(L"时间同步", L"设置本地时间", L"设置本地时间成功");
					}
					else
					{
						CNHLogAPI::WriteLog(L"时间同步", L"设置本地时间", L"设置本地时间失败");
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::GetUsers(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"GetUsers");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	m_vtNetUserInfo.clear();

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"")	// 没有Result节点时有用户列表
			{
				if (xmlReader.OpenNode(L"Message/Respond/Row/User"))
				{
					do
					{			
						std::map<std::wstring, std::wstring> mapField;	
						xmlReader.EnterNode();
						do
						{
							std::wstring strName, strContent;
							xmlReader.GetNodeName(strName);
							xmlReader.GetNodeContent(strContent);
							mapField[strName] = strContent;
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();

						USERINFO sUserInfo;
						wcscpy_s(sUserInfo.wchLicenseSn, mapField[L"Account"].c_str());
						wcscpy_s(sUserInfo.wchName, mapField[L"Name"].c_str());
						wcscpy_s(sUserInfo.wchRole, mapField[L"UserType"].c_str());
						m_vtNetUserInfo.push_back(sUserInfo);
					}
					while (xmlReader.NextSibling());
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::EmiVerify(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"Verify");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<User>%s</User>", m_strOperatorName.c_str());
	strXML.AppendFormat(L"<Pwd>%s</Pwd>", m_strOperatorPassword.c_str());
	strXML.AppendFormat(L"<User2>%s</User2>", m_strDriverName.c_str());
	strXML.AppendFormat(L"<Pwd2>%s</Pwd2>", m_strDriverPassword.c_str());
	strXML.AppendFormat(L"<LoginType>%d</LoginType>", 0);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::DemVerify(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"Verify");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<User>%s</User>", m_strOperatorName.c_str());
	strXML.AppendFormat(L"<Pwd>%s</Pwd>", m_strOperatorPassword.c_str());
	strXML.AppendFormat(L"<User2>%s</User2>", m_strDriverName.c_str());
	strXML.AppendFormat(L"<Pwd2>%s</Pwd2>", m_strDriverPassword.c_str());
	strXML.AppendFormat(L"<LoginType>%d</LoginType>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::UserExit(LPCTSTR szOperator, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"UserExit");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<User>%s</User>", szOperator);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::GetEvnSet(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"GetEvnSet");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				std::wstring strTemp;
				if (xmlReader.OpenNode(L"Message/Respond/evnset/TEMPL"))
				{
					// 温度低限值
					xmlReader.GetNodeContent(strTemp);
				}
				if (xmlReader.OpenNode(L"Message/Respond/evnset/TEMPH"))
				{
					// 温度高限值
					xmlReader.GetNodeContent(strTemp);
				}
				if (xmlReader.OpenNode(L"Message/Respond/evnset/HUML"))
				{
					// 湿度低限值
					xmlReader.GetNodeContent(strTemp);
				}
				if (xmlReader.OpenNode(L"Message/Respond/evnset/HUMH"))
				{
					// 湿度高限值
					xmlReader.GetNodeContent(strTemp);
				}
				if (xmlReader.OpenNode(L"Message/Respond/evnset/ATML"))
				{
					// 大气压低限值
					xmlReader.GetNodeContent(strTemp);
				}
				if (xmlReader.OpenNode(L"Message/Respond/evnset/ATMH"))
				{
					// 大气压高限值
					xmlReader.GetNodeContent(strTemp);
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::GetVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"VehicleList");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	m_vtNetUserInfo.clear();

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
#ifndef DEBUG
		if (xmlReader.Parse(strRecv.c_str()))
#else
		if (xmlReader.LoadFile(L"XML\\VehicleList.xml"))
#endif
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"")	// 没有Result节点时有车辆列表
			{
				if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
				{
					do
					{			
						std::map<std::wstring, std::wstring> mapField;	
						xmlReader.EnterNode();
						do
						{
							std::wstring strName, strContent;
							xmlReader.GetNodeName(strName);
							xmlReader.GetNodeContent(strContent);
							mapField[strName] = strContent;
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();

						TESTLOG sTestLog;
						wcscpy_s(sTestLog.wchRunningNumber, mapField[L"OutlookID"].c_str());
						wcscpy_s(sTestLog.wchReportNumber, mapField[L"OutlookID"].c_str());
						wcscpy_s(sTestLog.wchPlateNumber, mapField[L"License"].c_str());

						//0 = 蓝牌
						//1 = 黄牌					
						//2 = 白色
						//3 = 黑牌
						//4 = 新能源
						if (mapField[L"LicenseType"] == L"0")
						{
							wcscpy_s(sTestLog.wchPlateType, L"蓝牌");
						}
						else if (mapField[L"LicenseType"] == L"1")
						{
							wcscpy_s(sTestLog.wchPlateType, L"黄牌");
						}
						else if (mapField[L"LicenseType"] == L"3")
						{
							wcscpy_s(sTestLog.wchPlateType, L"黑牌");
						}
						else if (mapField[L"LicenseType"] == L"2")
						{
							wcscpy_s(sTestLog.wchPlateType, L"白色");
						}
						else if (mapField[L"LicenseType"] == L"4")
						{
							wcscpy_s(sTestLog.wchPlateType, L"新能源");
						}
						else
						{
							wcscpy_s(sTestLog.wchPlateType, mapField[L"LicenseType"].c_str());
						}

						//车辆类型
						//0 = 汽油车
						//1 = 柴油车
						//2 = 三轮汽车和低速货车
						//3 = 纯电汽车
						if (mapField[L"VehicleKind"] == L"0")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"汽油车");
						}
						else if (mapField[L"VehicleKind"] == L"1")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"柴油车");
						}
						else if (mapField[L"VehicleKind"] == L"2")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"三轮汽车和低速货车");
						}
						else if (mapField[L"VehicleKind"] == L"3")
						{
							wcscpy_s(sTestLog.wchVehicleType, L"纯电汽车");
						}
						else
						{
							wcscpy_s(sTestLog.wchVehicleType, mapField[L"VehicleKind"].c_str());
						}

						// 编号及检测方法名称
						//A	双怠速
						//B	稳态工况
						//C	简易瞬态
						//E	滤纸烟度
						//F	自由加速
						//G	加载减速
						if (mapField[L"CheckMethod"] == L"A")
						{
							wcscpy_s(sTestLog.wchTestType, L"4");
						}
						else if (mapField[L"CheckMethod"] == L"B")
						{
							wcscpy_s(sTestLog.wchTestType, L"1");
						}
						else if (mapField[L"CheckMethod"] == L"C")
						{
							wcscpy_s(sTestLog.wchTestType, L"2");
						}
						else if (mapField[L"CheckMethod"] == L"E")
						{
							wcscpy_s(sTestLog.wchTestType, L"6");
						}
						else if (mapField[L"CheckMethod"] == L"F")
						{
							wcscpy_s(sTestLog.wchTestType, L"5");
						}
						else if (mapField[L"CheckMethod"] == L"G")
						{
							wcscpy_s(sTestLog.wchTestType, L"3");
						}
						else
						{
							wcscpy_s(sTestLog.wchTestType, mapField[L"CheckMethod"].c_str());
						}

						//0	定期检验
						//1	注册登记检验
						//2	抽检
						//3	异地年检
						//7	实验对比
						//8	监督抽检不合格复检
						//9	外地车转入检测
						if (mapField[L"CheckType"] == L"0")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"定期检验");
						}
						else if (mapField[L"CheckType"] == L"1")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"注册登记检验");
						}
						else if (mapField[L"CheckType"] == L"2")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"抽检");
						}
						else if (mapField[L"CheckType"] == L"3")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"异地年检");
						}
						else if (mapField[L"CheckType"] == L"7")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"实验对比");
						}
						else if (mapField[L"CheckType"] == L"8")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"监督抽检不合格复检");
						}
						else if (mapField[L"CheckType"] == L"9")
						{
							wcscpy_s(sTestLog.wchTestCharacter, L"外地车转入检测");
						}
						else
						{
							wcscpy_s(sTestLog.wchTestCharacter, mapField[L"CheckType"].c_str());
						}
						wcscpy_s(sTestLog.wchTestCharacterCode, mapField[L"CheckType"].c_str());

						//0	汽油
						//1	柴油
						//2	液化石油气(LPG)
						//3	天然气(CNG)
						//4	其他
						//6	双燃料
						//8	混合动力
						//9	电
						if (mapField[L"FuelType"] == L"0")
						{
							wcscpy_s(sTestLog.wchFuelType, L"汽油");
						}
						else if (mapField[L"FuelType"] == L"1")
						{
							wcscpy_s(sTestLog.wchFuelType, L"柴油");
						}
						else if (mapField[L"FuelType"] == L"2")
						{
							wcscpy_s(sTestLog.wchFuelType, L"液化石油气(LPG)");
						}
						else if (mapField[L"FuelType"] == L"3")
						{
							wcscpy_s(sTestLog.wchFuelType, L"天然气(CNG)");
						}
						else if (mapField[L"FuelType"] == L"4")
						{
							wcscpy_s(sTestLog.wchFuelType, L"其他");
						}
						else if (mapField[L"FuelType"] == L"6")
						{
							wcscpy_s(sTestLog.wchFuelType, L"双燃料");
						}
						else if (mapField[L"FuelType"] == L"8")
						{
							wcscpy_s(sTestLog.wchFuelType, L"混合动力");
						}
						else if (mapField[L"FuelType"] == L"9")
						{
							wcscpy_s(sTestLog.wchFuelType, L"电");
						}
						else
						{
							wcscpy_s(sTestLog.wchFuelType, mapField[L"FuelType"].c_str());
						}

						vtTestLog.push_back(sTestLog);
					}
					while (xmlReader.NextSibling());
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::GetVehicleInfo(LPCTSTR szOutlookID, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"VehicleInfo");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<OutlookID>%s</OutlookID>", szOutlookID);
	strXML.AppendFormat(L"<License>%s</License>", L"");
	strXML.AppendFormat(L"<LicenseType>%s</LicenseType>", L"");
	strXML.AppendFormat(L"<VIN>%s</VIN>", L"");
	strXML.AppendFormat(L"<DateTime>%s</DateTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
#ifndef DEBUG
		if (xmlReader.Parse(strRecv.c_str()))
#else
		if (xmlReader.LoadFile(L"XML\\VehicleInfo.xml"))
#endif
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"")	// 没有Result节点时有车辆信息
			{
				std::wstring strTemp;
				if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
				{
					std::map<std::wstring, std::wstring> mapField;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapField[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();
					
					// 记录OutlookID
					m_strOutlookID = szOutlookID;

					wcscpy_s(sTestLog.wchRunningNumber, m_strOutlookID.c_str());
					wcscpy_s(sTestLog.wchReportNumber, m_strOutlookID.c_str());
					wcscpy_s(sTestLog.wchPlateNumber, mapField[L"License"].c_str());
					wcscpy_s(sTestLog.wchVIN, mapField[L"Vin"].c_str());
					wcscpy_s(sTestLog.wchRegistrationDate, mapField[L"RegisterDate"].c_str());
					wcscpy_s(sTestLog.wchProductionDate, mapField[L"RegisterDate"].c_str());
					
					//车辆类型。分以下几种：
					//K1 大型客车
					//K2 中型客车
					//K3 小型客车
					//K4 微型客车
					//H1 重型货车
					//H2 中型货车
					//H3 轻型货车
					//H4 微型货车
					//H5 低速货车
					//M1 三轮摩托车
					//M2 二轮摩托车
					//N1 三轮汽车
					if (mapField[L"VehicleType"] == L"K1")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"大型客车");
					}
					else if (mapField[L"VehicleType"] == L"K2")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"中型客车");
					}
					else if (mapField[L"VehicleType"] == L"K3")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"小型客车");
					}
					else if (mapField[L"VehicleType"] == L"K4")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"微型客车");
					}
					else if (mapField[L"VehicleType"] == L"H1")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"重型货车");
					}
					else if (mapField[L"VehicleType"] == L"H2")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"中型货车");
					}
					else if (mapField[L"VehicleType"] == L"H3")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"轻型货车");
					}
					else if (mapField[L"VehicleType"] == L"H4")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"微型货车");
					}
					else if (mapField[L"VehicleType"] == L"H5")
					{
						wcscpy_s(sTestLog.wchVehicleType, L"低速货车");
					}
					else
					{
						wcscpy_s(sTestLog.wchVehicleType, mapField[L"VehicleType"].c_str());
					}

					wcscpy_s(sTestLog.wchModel, mapField[L"Model"].c_str());

					//变速箱形式。分以下几种：
					//0 = 手动
					//1 = 自动
					//2 = 手自一体
					if (mapField[L"GearBoxType"] == L"0")
					{
						wcscpy_s(sTestLog.wchGearBoxType, L"手动");
					}
					else if (mapField[L"GearBoxType"] == L"1")
					{
						wcscpy_s(sTestLog.wchGearBoxType, L"自动");
					}
					else if (mapField[L"GearBoxType"] == L"2")
					{
						wcscpy_s(sTestLog.wchGearBoxType, L"手自一体");
					}
					else
					{
						wcscpy_s(sTestLog.wchGearBoxType, mapField[L"GearBoxType"].c_str());
					}
					
					//进气方式。分以下几种：
					//0 = 自然进气
					//1 = 涡轮增压
					//2 = 机械增压
					if (mapField[L"AdmissionMode"] == L"0")
					{
						wcscpy_s(sTestLog.wchAirIntakeMode, L"自然进气");
					}
					else if (mapField[L"AdmissionMode"] == L"1")
					{
						wcscpy_s(sTestLog.wchAirIntakeMode, L"涡轮增压");
					}
					else if (mapField[L"AdmissionMode"] == L"2")
					{
						wcscpy_s(sTestLog.wchAirIntakeMode, L"机械增压");
					}
					else
					{
						wcscpy_s(sTestLog.wchAirIntakeMode, mapField[L"AdmissionMode"].c_str());
					}

					wcscpy_s(sTestLog.wchDisplacement, mapField[L"Volume"].c_str());
					wcscpy_s(sTestLog.wchTravelledDistance, mapField[L"Odometer"].c_str());
					
					//0	汽油
					//1	柴油
					//2	液化石油气(LPG)
					//3	天然气(CNG)
					//4	其他
					//6	双燃料
					//8	混合动力
					//9	电
					if (mapField[L"FuelType"] == L"0")
					{
						wcscpy_s(sTestLog.wchFuelType, L"汽油");
					}
					else if (mapField[L"FuelType"] == L"1")
					{
						wcscpy_s(sTestLog.wchFuelType, L"柴油");
					}
					else if (mapField[L"FuelType"] == L"2")
					{
						wcscpy_s(sTestLog.wchFuelType, L"液化石油气(LPG)");
					}
					else if (mapField[L"FuelType"] == L"3")
					{
						wcscpy_s(sTestLog.wchFuelType, L"天然气(CNG)");
					}
					else if (mapField[L"FuelType"] == L"4")
					{
						wcscpy_s(sTestLog.wchFuelType, L"其他");
					}
					else if (mapField[L"FuelType"] == L"6")
					{
						wcscpy_s(sTestLog.wchFuelType, L"双燃料");
					}
					else if (mapField[L"FuelType"] == L"8")
					{
						wcscpy_s(sTestLog.wchFuelType, L"混合动力");
					}
					else if (mapField[L"FuelType"] == L"9")
					{
						wcscpy_s(sTestLog.wchFuelType, L"电");
					}
					else
					{
						wcscpy_s(sTestLog.wchFuelType, mapField[L"FuelType"].c_str());
					}

					//供油方式。分以下几种：
					//0 = 化油器
					//1 = 开环电喷
					//2 = 闭环电喷
					//3 = 直喷
					if (mapField[L"SupplyMode"] == L"0")
					{
						wcscpy_s(sTestLog.wchOilSupplyMode, L"化油器");
					}
					else if (mapField[L"SupplyMode"] == L"1")
					{
						wcscpy_s(sTestLog.wchOilSupplyMode, L"开环电喷");
					}
					else if (mapField[L"SupplyMode"] == L"2")
					{
						wcscpy_s(sTestLog.wchOilSupplyMode, L"闭环电喷");
					}
					else if (mapField[L"SupplyMode"] == L"3")
					{
						wcscpy_s(sTestLog.wchOilSupplyMode, L"直喷");
					}
					else
					{
						wcscpy_s(sTestLog.wchOilSupplyMode, mapField[L"SupplyMode"].c_str());
					}

					wcscpy_s(sTestLog.wchRatedRev, mapField[L"RatedRev"].c_str());
					wcscpy_s(sTestLog.wchRatedPower, mapField[L"RatedPower"].c_str());

					//驱动方式。分以下几种：
					//0 = 前驱
					//1 = 后驱
					//2 = 四驱
					//3 = 全时四驱
					if (mapField[L"DriveMode"] == L"0")
					{
						wcscpy_s(sTestLog.wchDriveType, L"前驱");
					}
					else if (mapField[L"DriveMode"] == L"1")
					{
						wcscpy_s(sTestLog.wchDriveType, L"后驱");
					}
					else if (mapField[L"DriveMode"] == L"2")
					{
						wcscpy_s(sTestLog.wchDriveType, L"四驱");
					}
					else if (mapField[L"DriveMode"] == L"3")
					{
						wcscpy_s(sTestLog.wchDriveType, L"全时四驱");
					}
					else
					{
						wcscpy_s(sTestLog.wchDriveType, mapField[L"DriveMode"].c_str());
					}

					wcscpy_s(sTestLog.wchOwner, mapField[L"Owner"].c_str());
					wcscpy_s(sTestLog.wchPhone, mapField[L"Phone"].c_str());
					wcscpy_s(sTestLog.wchAddress, mapField[L"Address"].c_str());
					wcscpy_s(sTestLog.wchMaximumTotalMass, mapField[L"MaxMass"].c_str());
					swprintf_s(sTestLog.wchUnladenMass, L"%d", max(_wtoi(mapField[L"RefMass"].c_str())-100, 750));
					
					wcscpy_s(sTestLog.wchHasCatalyticConverter, mapField[L"HasPurge"].c_str());

					if (mapField[L"IsEFI"] == L"1")
					{
					}

					//K 客车
					//H 货车
					//Q 牵引车
					//Z 专项作业车
					//D 电车
					//M 摩托车
					//N 三轮车
					//T 拖拉机
					//J 轮式机械
					//G 全挂车
					//B 半挂车
					//X 其他
					if (mapField[L"CarOrTruck"] == L"K")
					{
						wcscpy_s(sTestLog.wchRatedPassengerCapacity, mapField[L"MaxLoad"].c_str());
					}
					else
					{
						wcscpy_s(sTestLog.wchRatedLoadingMass, mapField[L"MaxLoad"].c_str());
					}

					wcscpy_s(sTestLog.wchNumberOfCylinder, mapField[L"Cylinder"].c_str());
					
					// 编号及检测方法名称
					//A	双怠速
					//B	稳态工况
					//C	简易瞬态
					//E	滤纸烟度
					//F	自由加速
					//G	加载减速
					if (mapField[L"YellowToGreen"] == L"A")
					{
						wcscpy_s(sTestLog.wchTestType, L"4");
					}
					else if (mapField[L"YellowToGreen"] == L"B")
					{
						wcscpy_s(sTestLog.wchTestType, L"1");
					}
					else if (mapField[L"YellowToGreen"] == L"C")
					{
						wcscpy_s(sTestLog.wchTestType, L"2");
					}
					else if (mapField[L"YellowToGreen"] == L"E")
					{
						wcscpy_s(sTestLog.wchTestType, L"6");
					}
					else if (mapField[L"YellowToGreen"] == L"F")
					{
						wcscpy_s(sTestLog.wchTestType, L"5");
					}
					else if (mapField[L"YellowToGreen"] == L"G")
					{
						wcscpy_s(sTestLog.wchTestType, L"3");
					}
					else
					{
						wcscpy_s(sTestLog.wchTestType, mapField[L"CheckMethod"].c_str());
					}

					//车辆满足的排放标准
					//0：国零
					//1：国I
					//2：国II
					//3：国III
					//4：国IV
					//5：国V
					//6：国VI
					if (mapField[L"StandardID"] == L"0")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国零");
					}
					else if (mapField[L"StandardID"] == L"1")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国I");
					}
					else if (mapField[L"StandardID"] == L"2")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国II");
					}
					else if (mapField[L"StandardID"] == L"3")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国III");
					}
					else if (mapField[L"StandardID"] == L"4")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国IV");
					}
					else if (mapField[L"StandardID"] == L"5")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国V");
					}
					else if (mapField[L"StandardID"] == L"6")
					{
						wcscpy_s(sTestLog.wchEmissionStandard, L"国VI");
					}
					else
					{
						wcscpy_s(sTestLog.wchTestType, mapField[L"StandardID"].c_str());
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::GetLimit(LPCTSTR szOutlookID, MapData &mapLimit, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"VehicleInfo");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<OutlookID>%s</OutlookID>", szOutlookID);
	strXML.AppendFormat(L"<License>%s</License>", L"");
	strXML.AppendFormat(L"<LicenseType>%s</LicenseType>", L"");
	strXML.AppendFormat(L"<VIN>%s</VIN>", L"");
	strXML.AppendFormat(L"<DateTime>%s</DateTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"")	// 没有Result节点时有限值
			{
				std::wstring strTemp;
				if (xmlReader.OpenNode(L"Message/Respond/Row/LimitValue"))
				{	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapLimit[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::GetLimit(LIMITOFASM &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	MapData mapLimit;
	bool bRet = GetLimit(m_strOutlookID.c_str(), mapLimit, strResult, strMsg);
	if (bRet)
	{
		if (mapLimit[L"IsUsed"] == L"1")
		{
			wcscpy_s(sLimit.wchHC5025, mapLimit[L"HC5025"].c_str());
			wcscpy_s(sLimit.wchCO5025, mapLimit[L"CO5025"].c_str());
			wcscpy_s(sLimit.wchNO5025, mapLimit[L"NO5025"].c_str());
			wcscpy_s(sLimit.wchHC2540, mapLimit[L"HC2540"].c_str());
			wcscpy_s(sLimit.wchCO2540, mapLimit[L"CO2540"].c_str());
			wcscpy_s(sLimit.wchNO2540, mapLimit[L"NO2540"].c_str());
			wcscpy_s(sLimit.wchReserved1, L"获取联网限值成功");	// 成功标识
		}
	}
	return bRet;
}

bool CNSClientLib::GetLimit(LIMITOFVMAS &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	MapData mapLimit;
	bool bRet = GetLimit(m_strOutlookID.c_str(), mapLimit, strResult, strMsg);
	if (bRet)
	{
		if (mapLimit[L"IsUsed"] == L"1")
		{
			wcscpy_s(sLimit.wchHC, mapLimit[L"VmasHC"].c_str());
			wcscpy_s(sLimit.wchCO, mapLimit[L"VmasCO"].c_str());
			wcscpy_s(sLimit.wchNOx, mapLimit[L"VmasNOX"].c_str());
			wcscpy_s(sLimit.wchReserved1, L"获取联网限值成功");	// 成功标识
		}
	}
	return bRet;
}

bool CNSClientLib::GetLimit(LIMITOFDIS &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	MapData mapLimit;
	bool bRet = GetLimit(m_strOutlookID.c_str(), mapLimit, strResult, strMsg);
	if (bRet)
	{
		if (mapLimit[L"IsUsed"] == L"1")
		{
			wcscpy_s(sLimit.wchHCHigh, mapLimit[L"HighIdleHC"].c_str());
			wcscpy_s(sLimit.wchCOHigh, mapLimit[L"HighIdleCO"].c_str());
			wcscpy_s(sLimit.wchHCLow, mapLimit[L"LowIdleHC"].c_str());
			wcscpy_s(sLimit.wchCOLow, mapLimit[L"LowIdleCO"].c_str());
			swprintf_s(sLimit.wchLambdaLower, L"%.2f", 0.95f);
			swprintf_s(sLimit.wchLambdaUpper, L"%.2f", 1.05f);
			wcscpy_s(sLimit.wchReserved1, L"获取联网限值成功");	// 成功标识
		}
	}
	return bRet;
}

bool CNSClientLib::GetLimit(LIMITOFFSUNHT &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	MapData mapLimit;
	bool bRet = GetLimit(m_strOutlookID.c_str(), mapLimit, strResult, strMsg);
	if (bRet)
	{
		if (mapLimit[L"IsUsed"] == L"1")
		{
			wcscpy_s(sLimit.wchK, mapLimit[L"FASmokeK"].c_str());
			wcscpy_s(sLimit.wchReserved1, L"获取联网限值成功");	// 成功标识
		}
	}
	return bRet;
}

bool CNSClientLib::GetLimit(LIMITOFLUGDOWN &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	MapData mapLimit;
	bool bRet = GetLimit(m_strOutlookID.c_str(), mapLimit, strResult, strMsg);
	if (bRet)
	{
		if (mapLimit[L"IsUsed"] == L"1")
		{
			wcscpy_s(sLimit.wchK, mapLimit[L"SmokeK"].c_str());
			wcscpy_s(sLimit.wchNOx, mapLimit[L"SmokeNO"].c_str());
			wcscpy_s(sLimit.wchReserved1, L"获取联网限值成功");	// 成功标识
		}
	}
	return bRet;
}

bool CNSClientLib::StartTest(NHTestType emTestType, std::wstring &strResult, std::wstring &strMsg)
{
	// 已经开始检测的需要先停止
	if (m_mapTesting[m_strOutlookID] == L"1")
	{
		switch (emTestType)
		{
		case ASM:
			{
				std::wstring strResultStop, strMsgStop, strReportNumberStop;
				SResultOfASM sResultData;
				std::vector<SRealTimeDataOfASM> vtRTData;
				if (UploadASM(sResultData, vtRTData, strResultStop, strMsgStop, strReportNumberStop))
				{
					m_mapTesting[m_strOutlookID] = L"";
				}
			}
			break;
		case VMAS:
			{
				std::wstring strResultStop, strMsgStop, strReportNumberStop;
				SResultOfVMAS sResultData;
				std::vector<SRealTimeDataOfVMAS> vtRTData;
				if (UploadVMAS(sResultData, vtRTData, strResultStop, strMsgStop, strReportNumberStop))
				{
					m_mapTesting[m_strOutlookID] = L"";
				}
			}
			break;	
		case LUGDOWN:
			{
				std::wstring strResultStop, strMsgStop, strReportNumberStop;
				SResultOfLUGDOWN sResultData;
				std::vector<SRealTimeDataOfLUGDOWN> vtRTData;
				if (UploadLUGDOWN(sResultData, vtRTData, strResultStop, strMsgStop, strReportNumberStop))
				{
					m_mapTesting[m_strOutlookID] = L"";
				}
			}
			break;
		case DIS:
			{
				std::wstring strResultStop, strMsgStop, strReportNumberStop;
				SResultOfDIS sResultData;
				std::vector<SRealTimeDataOfDIS> vtRTData;
				if (UploadDIS(sResultData, vtRTData, strResultStop, strMsgStop, strReportNumberStop))
				{
					m_mapTesting[m_strOutlookID] = L"";
				}
			}
			break;
		case FSUNHT:
			{
				std::wstring strResultStop, strMsgStop, strReportNumberStop;
				SResultOfFSUNHT sResultData;
				std::vector<SRealTimeDataOfFSUNHT> vtRTData;
				if (UploadFSUNHT(sResultData, vtRTData, strResultStop, strMsgStop, strReportNumberStop))
				{
					m_mapTesting[m_strOutlookID] = L"";
				}
			}
			break;
		}
	}

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"StartTest");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				// 记录已经开始检测
				m_mapTesting[m_strOutlookID] = L"1";
			}
		}
		return true;
	}
	return false;
}
					//上传瞬态数据
bool CNSClientLib::UploadVMAS(SResultOfVMAS &sResultData, std::vector<SRealTimeDataOfVMAS> &vtRTData, 
						std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"UploadGasVmasResult");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<Temperature>%.1f</Temperature>", (float)_wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strXML.AppendFormat(L"<AirPressure>%.1f</AirPressure>", (float)_wtof(sResultData.strAtmosphericPressure.c_str()));
	strXML.AppendFormat(L"<Humidity>%.1f</Humidity>", (float)_wtof(sResultData.strRelativeHumidity.c_str()));
	strXML.AppendFormat(L"<AmbientCO>%.2f</AmbientCO>", rand()%2/100.0f);
	strXML.AppendFormat(L"<AmbientCO2>%.2f</AmbientCO2>", rand()%3/100.0f);
	strXML.AppendFormat(L"<AmbientHC>%d</AmbientHC>", rand()%5);
	strXML.AppendFormat(L"<AmbientNO>%d</AmbientNO>", rand()%5);
	strXML.AppendFormat(L"<AmbientO2>%.2f</AmbientO2>", 20.5f+rand()%50/100.0f);
	strXML.AppendFormat(L"<BackgroundCO>%.2f</BackgroundCO>", rand()%2/100.0f);
	strXML.AppendFormat(L"<BackgroundCO2>%.2f</BackgroundCO2>", rand()%3/100.0f);
	strXML.AppendFormat(L"<BackgroundHC>%d</BackgroundHC>", rand()%5);
	strXML.AppendFormat(L"<BackgroundNO>%d</BackgroundNO>", rand()%5);
	strXML.AppendFormat(L"<BackgroundO2>%.2f</BackgroundO2>", 20.5f+rand()%50/100.0f);
	strXML.AppendFormat(L"<ResidualHC>%d</ResidualHC>", rand()%5);
	strXML.AppendFormat(L"<TestTime>%d</TestTime>", 195);
	strXML.AppendFormat(L"<AirFlowAll>%.2f</AirFlowAll>", (float)_wtof(sResultData.strVolOfGas.c_str())/1000*60); // L/s -> m3/min
	strXML.AppendFormat(L"<CO>%.2f</CO>", (float)_wtof(sResultData.strCO.c_str()));			//平台取值及显示是2位小数
	strXML.AppendFormat(L"<CO2>%.2f</CO2>", (float)_wtof(sResultData.strCO2.c_str()));
	strXML.AppendFormat(L"<HC>%.2f</HC>", (float)_wtof(sResultData.strHC.c_str()));         //平台取值及显示是2位小数
	strXML.AppendFormat(L"<NO>%.2f</NO>", (float)_wtof(sResultData.strNOx.c_str()));        //平台取值1位小数，显示是2位小数
	strXML.AppendFormat(L"<NOX>%.2f</NOX>", (float)_wtof(sResultData.strNOx.c_str()));        //平台取值1位小数，显示是2位小数(协议没有)
	strXML.AppendFormat(L"<NOx>%.2f</NOx>", (float)_wtof(sResultData.strNOx.c_str()));        //平台取值1位小数，显示是2位小数(协议没有)
	strXML.AppendFormat(L"<Power>%.2f</Power>", (float)_wtof(sResultData.strPower50.c_str()));

	// 计算Lambda
	float fLambda = 0;
	for (UINT i=0; i<vtRTData.size(); i++)
	{
		fLambda += (float)_wtof(vtRTData[i].strLambda.c_str());
	}
	fLambda /= vtRTData.size();
	strXML.AppendFormat(L"<Lambda>%.2f</Lambda>", fLambda);

	strXML.AppendFormat(L"<TestKilomter>%.3f</TestKilomter>", (float)_wtof(sResultData.strTravelledDistance.c_str()));
	
	CString strStartTime, strEndTime;
	if (vtRTData.size() >= 1)
	{
		strStartTime = vtRTData[0].strSamplingTime.c_str();
		strEndTime = vtRTData[vtRTData.size()-1].strSamplingTime.c_str();
	}
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", strStartTime);
	strXML.AppendFormat(L"<EndTime>%s</EndTime>", strEndTime);
	
	// 计算速度偏离次数和时间
	std::vector<int> vtVWarning;
	for (UINT i=0; i<vtRTData.size()&&i<m_vtVSTDMax.size()&&i<m_vtVSTDMin.size(); i++)
	{
		if ((float)_wtof(vtRTData[i].strVelocity.c_str()) > m_vtVSTDMax[i]
			|| (float)_wtof(vtRTData[i].strVelocity.c_str()) < m_vtVSTDMin[i]
			)
		{
			vtVWarning.push_back(1);
		}
		else
		{
			vtVWarning.push_back(0);
		}
	}
	int nPLCS(0), nPLSJ(0);
	for (UINT i=0; i<vtVWarning.size(); i++)
	{
		if (i >= 1)
		{
			if (vtVWarning[i-1] == 0
				&& vtVWarning[i] == 1
				)
			{
				nPLCS++;
			}
		}
		nPLSJ += vtVWarning[i];
	}
	strXML.AppendFormat(L"<plcs>%d</plcs>", nPLCS);
	strXML.AppendFormat(L"<plsj>%d</plsj>", nPLSJ);

	if (vtRTData.size() == 0)
	{
		// 没有数据，是停止的调用
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 9);
	}
	else
	{
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 0);
	}
	strXML.AppendFormat(L"<AdjustZero>%d</AdjustZero>", 1);
	strXML.AppendFormat(L"<Result>%s</Result>", sResultData.strPass.c_str());
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	for (UINT i=0; i<vtRTData.size(); i++)
	{
		strXML.AppendFormat(L"<Row>");		
		strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", i+1);

		//工况类型。
		//0：15km/h等速检测中
		//1：32km/h等速检测中
		//2：50km/h等速检测中
		//3：35km/h等速检测中
		//4：怠速检测中
		//5：加速过程
		//6：减速过程
		int nDataType(0);
		if (i>=15 && i<23)
		{
			nDataType = 0;
		}
		else if (i>=61 && i<85)
		{
			nDataType = 1;
		}
		else if (i>=143 && i<155)
		{
			nDataType = 2;
		}
		else if (i>=163 && i<176)
		{
			nDataType = 3;
		}
		else if (i < 11
			|| (i>=28 && i<49)
			|| (i>=96 && i<117)
			|| (i>=188 && i<195)
			)
		{
			nDataType = 4;
		}
		else if ((i>=11 && i<15)
				|| (i>=49 && i<61)
				|| (i>=117 && i<143)
				)
		{
			nDataType = 5;
		}
		else if ((i>=23 && i<28)
				|| (i>=85 && i<96)
				|| (i>=155 && i<163)
				|| (i>=176 && i<188)
				)
		{
			nDataType = 6;
		}
		strXML.AppendFormat(L"<DataType>%d</DataType>", nDataType);

		float fDCF = (float)_wtof(vtRTData[i].strDilutionCorrectionFactor.c_str());
		float fHCF = (float)_wtof(vtRTData[i].strHumidityCorrectionFactor.c_str());
		strXML.AppendFormat(L"<HC>%.2f</HC>", (float)_wtof(vtRTData[i].strHC.c_str()));                     //平台取值及显示是2位小数
		strXML.AppendFormat(L"<HCDF>%.1f</HCDF>", (float)_wtof(vtRTData[i].strHC.c_str())*fDCF);
		strXML.AppendFormat(L"<CO>%.2f</CO>", (float)_wtof(vtRTData[i].strCO.c_str()));                     //平台取值及显示是2位小数
		strXML.AppendFormat(L"<CODF>%.2f</CODF>", (float)_wtof(vtRTData[i].strCO.c_str())*fDCF);
		strXML.AppendFormat(L"<NOX>%.2f</NOX>", (float)_wtof(vtRTData[i].strNOx.c_str()));                  //平台取值1位小数，显示是2位小数
		strXML.AppendFormat(L"<NODFK>%.1f</NODFK>", (float)_wtof(vtRTData[i].strNOx.c_str())*fDCF*fHCF);
		strXML.AppendFormat(L"<CO2>%.2f</CO2>", (float)_wtof(vtRTData[i].strCO2.c_str()));
		strXML.AppendFormat(L"<O2>%.2f</O2>", (float)_wtof(vtRTData[i].strO2.c_str()));
		strXML.AppendFormat(L"<AirFlowO2>%.2f</AirFlowO2>", (float)_wtof(vtRTData[i].strO2OfFlowmeter.c_str()));
		strXML.AppendFormat(L"<Rev>%s</Rev>", vtRTData[i].strEngineRev.c_str());
		strXML.AppendFormat(L"<JYWD>%.1f</JYWD>", (float)_wtof(vtRTData[i].strOilTemperature.c_str()));
		strXML.AppendFormat(L"<Velocity>%.2f</Velocity>", (float)_wtof(vtRTData[i].strVelocity.c_str()));
		strXML.AppendFormat(L"<Torque>%.2f</Torque>", (float)_wtof(vtRTData[i].strForce.c_str()));
		strXML.AppendFormat(L"<Power>%.2f</Power>", (float)_wtof(vtRTData[i].strPower.c_str()));
		strXML.AppendFormat(L"<Temperature>%.1f</Temperature>", (float)_wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
		strXML.AppendFormat(L"<Humidity>%.1f</Humidity>", (float)_wtof(vtRTData[i].strRelativeHumidity.c_str()));
		strXML.AppendFormat(L"<AirPressure>%.1f</AirPressure>", (float)_wtof(vtRTData[i].strAtmosphericPressure.c_str()));
		strXML.AppendFormat(L"<HumCoefficient>%.2f</HumCoefficient>", fHCF);
		strXML.AppendFormat(L"<DilutionCoefficient>%.2f</DilutionCoefficient>", fDCF);
		strXML.AppendFormat(L"<AirFlowO2>%.2f</AirFlowO2>", (float)_wtof(vtRTData[i].strO2OfFlowmeter.c_str()));
		strXML.AppendFormat(L"<BJO2>%.2f</BJO2>", (float)_wtof(vtRTData[i].strO2OfEnvironment.c_str()));
		strXML.AppendFormat(L"<DilutionProportion>%.2f</DilutionProportion>", (float)_wtof(vtRTData[i].strDR.c_str()));
		strXML.AppendFormat(L"<FlowTemperature>%.1f</FlowTemperature>", (float)_wtof(vtRTData[i].strTemperatureOfFlowmeter.c_str()));
		strXML.AppendFormat(L"<FlowPressure>%.1f</FlowPressure>", (float)_wtof(vtRTData[i].strPressureOfFlowmeter.c_str()));
		strXML.AppendFormat(L"<AirFlow>%.2f</AirFlow>", (float)_wtof(vtRTData[i].strActualFluxOfGas.c_str()));
		strXML.AppendFormat(L"<BZLL>%.2f</BZLL>", (float)_wtof(vtRTData[i].strFluxOfExhaust.c_str()));
		strXML.AppendFormat(L"<LowFlowFlag>%d</LowFlowFlag>", 0);
		strXML.AppendFormat(L"</Row>");
	}
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				if (xmlReader.OpenNode(L"Message"))
				{
					xmlReader.GetNodeAttribute(L"ReportNum", strReportNumber);
				}
			}
		}
		return true;
	}
	return false;
}
	
bool CNSClientLib::UploadASM(SResultOfASM &sResultData, std::vector<SRealTimeDataOfASM> &vtRTData, 
				std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"UploadGasolineResult");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<Temperature>%.1f</Temperature>", (float)_wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strXML.AppendFormat(L"<AirPressure>%.1f</AirPressure>", (float)_wtof(sResultData.strAtmosphericPressure.c_str()));
	strXML.AppendFormat(L"<Humidity>%.1f</Humidity>", (float)_wtof(sResultData.strRelativeHumidity.c_str()));
	strXML.AppendFormat(L"<AmbientCO>%.2f</AmbientCO>", rand()%2/100.0f);
	strXML.AppendFormat(L"<AmbientCO2>%.2f</AmbientCO2>", rand()%3/100.0f);
	strXML.AppendFormat(L"<AmbientHC>%d</AmbientHC>", rand()%5);
	strXML.AppendFormat(L"<AmbientNO>%d</AmbientNO>", rand()%5);
	strXML.AppendFormat(L"<AmbientO2>%.2f</AmbientO2>", 20.5f+rand()%50/100.0f);
	strXML.AppendFormat(L"<BackgroundCO>%.2f</BackgroundCO>", rand()%2/100.0f);
	strXML.AppendFormat(L"<BackgroundCO2>%.2f</BackgroundCO2>", rand()%3/100.0f);
	strXML.AppendFormat(L"<BackgroundHC>%d</BackgroundHC>", rand()%5);
	strXML.AppendFormat(L"<BackgroundNO>%d</BackgroundNO>", rand()%5);
	strXML.AppendFormat(L"<BackgroundO2>%.2f</BackgroundO2>", 20.5f+rand()%50/100.0f);
	strXML.AppendFormat(L"<ResidualHC>%d</ResidualHC>", rand()%5);
	strXML.AppendFormat(L"<CO5025>%.2f</CO5025>", (float)_wtof(sResultData.strCO5025.c_str()));
	strXML.AppendFormat(L"<HC5025>%.0f</HC5025>", (float)_wtof(sResultData.strHC5025.c_str()));
	strXML.AppendFormat(L"<NO5025>%.0f</NO5025>", (float)_wtof(sResultData.strNO5025.c_str()));
	strXML.AppendFormat(L"<Power5025>%.2f</Power5025>", (float)_wtof(sResultData.strPower5025.c_str()));
	if (_wtoi(sResultData.strRev5025.c_str()) == 0)
	{
		strXML.AppendFormat(L"<Rev5025></Rev5025>");
	}
	else
	{
		strXML.AppendFormat(L"<Rev5025>%.0f</Rev5025>", (float)_wtof(sResultData.strRev5025.c_str()));
	}
	strXML.AppendFormat(L"<Lambda5025>%.2f</Lambda5025>", (float)_wtof(sResultData.strLambda5025.c_str()));
	if (sResultData.strNeedTestMode2540 == L"1")
	{
		strXML.AppendFormat(L"<CO2540>%.2f</CO2540>", (float)_wtof(sResultData.strCO2540.c_str()));
		strXML.AppendFormat(L"<HC2540>%.0f</HC2540>", (float)_wtof(sResultData.strHC2540.c_str()));
		strXML.AppendFormat(L"<NO2540>%.0f</NO2540>", (float)_wtof(sResultData.strNO2540.c_str()));
		strXML.AppendFormat(L"<Power2540>%.2f</Power2540>", (float)_wtof(sResultData.strPower2540.c_str()));
		if (_wtoi(sResultData.strRev2540.c_str()) == 0)
		{
			strXML.AppendFormat(L"<Rev5025></Rev5025>");
		}
		else
		{
			strXML.AppendFormat(L"<Rev2540>%.0f</Rev2540>", (float)_wtof(sResultData.strRev2540.c_str()));
		}
		strXML.AppendFormat(L"<Lambda2540>%.2f</Lambda2540>", (float)_wtof(sResultData.strLambda2540.c_str()));
	}
	else
	{
		// 没有做2540需要传空值
		strXML.AppendFormat(L"<CO2540></CO2540>");
		strXML.AppendFormat(L"<HC2540></HC2540>");
		strXML.AppendFormat(L"<NO2540></NO2540>");
		strXML.AppendFormat(L"<Power2540></Power2540>");
		strXML.AppendFormat(L"<Rev2540></Rev2540>");
		strXML.AppendFormat(L"<Lambda2540></Lambda2540>");
	}

	CString strStartTime, strEndTime;
	if (vtRTData.size() >= 1)
	{
		strStartTime = vtRTData[0].strSamplingTime.c_str();
		strEndTime = vtRTData[vtRTData.size()-1].strSamplingTime.c_str();
	}
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", strStartTime);
	strXML.AppendFormat(L"<EndTime>%s</EndTime>", strEndTime);

	if (vtRTData.size() == 0)
	{
		// 没有数据，是停止的调用
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 9);
	}
	else
	{
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 0);
	}
	strXML.AppendFormat(L"<AdjustZero>%d</AdjustZero>", 1);

	//检测过程标识（例：如5025快速工况结束检测，该值传1,；以此类推。）
	//0：准备
	//1.5025快速工况
	//2.5025工况
	//3.2540快速工况
	//4.2540工况
	int nCheckProcess(0);
	if (vtRTData.size() <= 20)
	{
		nCheckProcess = 1;
	}
	else if (vtRTData.size()>20 && vtRTData.size()<=90)
	{
		nCheckProcess = 2;
	}
	else if (vtRTData.size()>90 && vtRTData.size()<=110)
	{
		nCheckProcess = 3;
	}
	else if (vtRTData.size()>110)
	{
		nCheckProcess = 4;
	}
	strXML.AppendFormat(L"<CheckProcess>%d</CheckProcess>", nCheckProcess);

	strXML.AppendFormat(L"<Result>%s</Result>", sResultData.strPass.c_str());
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	for (UINT i=0; i<vtRTData.size(); i++)
	{
		// 0) 未开始
		// 1) 初始化(检查设备通讯等)
		// 2) 降举升器
		// 3) 5秒稳定车速(5025)
		// 4) 10秒数据同步(5025)
		// 5) 10秒快速工况(5025)
		// 6) 65秒检测(5025)
		// 7) 5秒稳定车速(2540)
		// 8) 10秒数据同步(2540)
		// 9) 10秒快速工况(2540)
		// 10) 65秒检测(2540)
		// 11) 减速停车
		int nState = _wtoi(vtRTData[i].strState.c_str());
		if (nState == 3
			|| nState == 4
			|| nState == 5
			|| nState == 6
			|| nState == 7
			|| nState == 8
			|| nState == 9
			|| nState == 10
			)
		{
			strXML.AppendFormat(L"<Row>");		
			strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", i+1);

			//工况类型。
			//0：加速过程或速度/转速稳定过程
			//1.5025快速工况（正常结束累计20条记录）
			//2.5025工况（正常结束累计90条记录）
			//3.2540快速工况（此过程累计20条记录）
			//4.2540工况（此过程累计90条记录）
			int nDataType(0);
			if (nState==3 || nState==7)
			{
				nDataType = 0;
			}
			else if (nState==4)
			{
				nDataType = 0;
			}
			else if (/*nState==4 || */nState==5)
			{
				nDataType = 1;
			}
			else if (nState == 6)
			{
				nDataType = 2;
			}
			else if (nState==8)
			{
				nDataType = 0;
			}
			else if (/*nState==8 || */nState==9)
			{
				nDataType = 3;
			}
			else if (nState == 10)
			{
				nDataType = 4;
			}
			strXML.AppendFormat(L"<DataType>%d</DataType>", nDataType);

			float fDCF = (float)_wtof(vtRTData[i].strDilutionCorrectionFactor.c_str());
			float fHCF = (float)_wtof(vtRTData[i].strHumidityCorrectionFactor.c_str());
			strXML.AppendFormat(L"<Rev>%s</Rev>", vtRTData[i].strEngineRev.c_str());
			strXML.AppendFormat(L"<JYWD>%.1f</JYWD>", (float)_wtof(vtRTData[i].strOilTemperature.c_str()));
			strXML.AppendFormat(L"<Velocity>%.2f</Velocity>", (float)_wtof(vtRTData[i].strVelocity.c_str()));
			strXML.AppendFormat(L"<Force>%.0f</Force>", (float)_wtof(vtRTData[i].strForce.c_str()));
			strXML.AppendFormat(L"<Power>%.2f</Power>", (float)_wtof(vtRTData[i].strPower.c_str()));
			strXML.AppendFormat(L"<DynamometerLoad>%.2f</DynamometerLoad>", (float)_wtof(vtRTData[i].strIHP.c_str()));
			strXML.AppendFormat(L"<HC>%.0f</HC>", (float)_wtof(vtRTData[i].strHC.c_str()));
			strXML.AppendFormat(L"<CO>%.2f</CO>", (float)_wtof(vtRTData[i].strCO.c_str()));
			strXML.AppendFormat(L"<NO>%.0f</NO>", (float)_wtof(vtRTData[i].strNO.c_str()));
			strXML.AppendFormat(L"<O2>%.2f</O2>", (float)_wtof(vtRTData[i].strO2.c_str()));
			strXML.AppendFormat(L"<CO2>%.2f</CO2>", (float)_wtof(vtRTData[i].strCO2.c_str()));
			strXML.AppendFormat(L"<XCO>%.2f</XCO>", (float)_wtof(vtRTData[i].strCO.c_str())*fDCF);
			strXML.AppendFormat(L"<XHC>%.0f</XHC>", (float)_wtof(vtRTData[i].strHC.c_str())*fDCF);
			strXML.AppendFormat(L"<XNO>%.0f</XNO>", (float)_wtof(vtRTData[i].strNO.c_str())*fDCF*fHCF);
			strXML.AppendFormat(L"<WD>%.1f</WD>", (float)_wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
			strXML.AppendFormat(L"<SD>%.1f</SD>", (float)_wtof(vtRTData[i].strRelativeHumidity.c_str()));
			strXML.AppendFormat(L"<DQY>%.1f</DQY>", (float)_wtof(vtRTData[i].strAtmosphericPressure.c_str()));
			strXML.AppendFormat(L"<KH>%.2f</KH>", fHCF);
			strXML.AppendFormat(L"<DF>%.2f</DF>", fDCF);
			strXML.AppendFormat(L"<LowFlowFlag>%d</LowFlowFlag>", 0);
			strXML.AppendFormat(L"</Row>");
		}
	}
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				if (xmlReader.OpenNode(L"Message"))
				{
					xmlReader.GetNodeAttribute(L"ReportNum", strReportNumber);
				}
			}
		}
		return true;
	}
	return false;
}
	
bool CNSClientLib::UploadLUGDOWN(SResultOfLUGDOWN &sResultData, std::vector<SRealTimeDataOfLUGDOWN> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"UploadDieselResult");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<Temperature>%.1f</Temperature>", (float)_wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strXML.AppendFormat(L"<AirPressure>%.1f</AirPressure>", (float)_wtof(sResultData.strAtmosphericPressure.c_str()));
	strXML.AppendFormat(L"<Humidity>%.1f</Humidity>", (float)_wtof(sResultData.strRelativeHumidity.c_str()));
	strXML.AppendFormat(L"<CalcVelMaxHP>%.2f</CalcVelMaxHP>", (float)_wtof(sResultData.strCalculatedVelMaxHP.c_str()));
	strXML.AppendFormat(L"<RealVelMaxHP>%.2f</RealVelMaxHP>", (float)_wtof(sResultData.strActualVelMaxHP.c_str()));
	strXML.AppendFormat(L"<RealMaxPower>%.2f</RealMaxPower>", (float)_wtof(sResultData.strMaxHP.c_str()));
	strXML.AppendFormat(L"<MaxPower>%.2f</MaxPower>", (float)_wtof(sResultData.strCorrectedPower.c_str()));
	strXML.AppendFormat(L"<DSFDJRpm>%.0f</DSFDJRpm>", (float)_wtof(sResultData.strIdleRev.c_str()));
	strXML.AppendFormat(L"<Rev100>%.0f</Rev100>", (float)_wtof(sResultData.strEngineRev100.c_str()));
	strXML.AppendFormat(L"<K100>%.2f</K100>", (float)_wtof(sResultData.strK100.c_str()));
	strXML.AppendFormat(L"<K80>%.2f</K80>", (float)_wtof(sResultData.strK80.c_str()));
	strXML.AppendFormat(L"<NOX>%.0f</NOX>", (float)_wtof(sResultData.strNOx80.c_str()));

	CString strStartTime, strEndTime;
	if (vtRTData.size() >= 1)
	{
		strStartTime = vtRTData[0].strSamplingTime.c_str();
		strEndTime = vtRTData[vtRTData.size()-1].strSamplingTime.c_str();
	}
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", strStartTime);
	strXML.AppendFormat(L"<EndTime>%s</EndTime>", strEndTime);

	if (vtRTData.size() == 0)
	{
		// 没有数据，是停止的调用
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 9);
	}
	else
	{
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 0);
	}
	strXML.AppendFormat(L"<Result>%s</Result>", sResultData.strPass.c_str());
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	for (UINT i=0; i<vtRTData.size(); i++)
	{
		strXML.AppendFormat(L"<Row>");		
		strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", i+1);

		// 检测状态,0:未开始;1:低负荷运转;2:功率扫描;3:VelMaxHP加载;4:90%;5:80%;6:减载停车;7:结束;8:检测中断	
		int nState = _wtoi(vtRTData[i].strState.c_str());

		//工况类型。
		//0：功率扫描中
		//1：恢复到100%VelMaxHP过程中
		//2：100%VelMaxHP点检测过程
		//3：90%VelMaxHP点检测过程
		//4：80%VelMaxHP点检测过程
		//注意：90%速度点的数据不要！！！
		int nDataType(0);
		if (nState == 0
			|| nState == 1
			|| nState == 2
			)
		{
			nDataType = 0;
		}
		else if (nState == 3)
		{
			nDataType = 2;
		}
		else if (nState == 5)
		{
			nDataType = 4;
		}
		strXML.AppendFormat(L"<DataType>%d</DataType>", nDataType);

		strXML.AppendFormat(L"<SmokeK>%.1f</SmokeK>", (float)_wtof(vtRTData[i].strK.c_str()));
		strXML.AppendFormat(L"<NOX>%.0f</NOX>", (float)_wtof(vtRTData[i].strNOx.c_str()));
		strXML.AppendFormat(L"<Rev>%s</Rev>", vtRTData[i].strEngineRev.c_str());
		strXML.AppendFormat(L"<Smoke>%.2f</Smoke>", (float)_wtof(vtRTData[i].strN.c_str()));
		strXML.AppendFormat(L"<JYWD>%.1f</JYWD>", (float)_wtof(vtRTData[i].strOilTemperature.c_str()));
		strXML.AppendFormat(L"<Velocity>%.2f</Velocity>", (float)_wtof(vtRTData[i].strVelocity.c_str()));
		strXML.AppendFormat(L"<Force>%.0f</Force>", (float)_wtof(vtRTData[i].strForce.c_str()));
		strXML.AppendFormat(L"<Power>%.2f</Power>", (float)_wtof(vtRTData[i].strPower.c_str()));
		strXML.AppendFormat(L"<CO2>%.2f</CO2>", (float)_wtof(vtRTData[i].strCO2.c_str()));
		strXML.AppendFormat(L"<WD>%.1f</WD>", (float)_wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
		strXML.AppendFormat(L"<SD>%.1f</SD>", (float)_wtof(vtRTData[i].strRelativeHumidity.c_str()));
		strXML.AppendFormat(L"<DQY>%.1f</DQY>", (float)_wtof(vtRTData[i].strAtmosphericPressure.c_str()));
		strXML.AppendFormat(L"<PCof>%.3f</PCof>", (float)_wtof(vtRTData[i].strPowerCorrectionFactor.c_str()));
		strXML.AppendFormat(L"<LowFlowFlag>%d</LowFlowFlag>", 0);
		strXML.AppendFormat(L"</Row>");
	}
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				if (xmlReader.OpenNode(L"Message"))
				{
					xmlReader.GetNodeAttribute(L"ReportNum", strReportNumber);
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::UploadDIS(SResultOfDIS &sResultData, std::vector<SRealTimeDataOfDIS> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"UploadDIdleResult");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<Temperature>%.1f</Temperature>", (float)_wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strXML.AppendFormat(L"<AirPressure>%.1f</AirPressure>", (float)_wtof(sResultData.strAtmosphericPressure.c_str()));
	strXML.AppendFormat(L"<Humidity>%.1f</Humidity>", (float)_wtof(sResultData.strRelativeHumidity.c_str()));
	strXML.AppendFormat(L"<OilTemperature>%.1f</OilTemperature>", (float)_wtof(sResultData.strOilTemperatureHigh.c_str()));
	strXML.AppendFormat(L"<backgroundCO>%.2f</backgroundCO>", rand()%2/100.0f);
	strXML.AppendFormat(L"<backgroundCO2>%.2f</backgroundCO2>", rand()%3/100.0f);
	strXML.AppendFormat(L"<backgroundHC>%d</backgroundHC>", rand()%5);
	strXML.AppendFormat(L"<backgroundNO>%d</backgroundNO>", rand()%5);
	strXML.AppendFormat(L"<ResidualHC>%d</ResidualHC>", rand()%5);
	strXML.AppendFormat(L"<LowIdleCO>%.2f</LowIdleCO>", (float)_wtof(sResultData.strCOLow.c_str()));		//平台取值及显示是2位小数
	strXML.AppendFormat(L"<LowIdleHC>%.0f</LowIdleHC>", (float)_wtof(sResultData.strHCLow.c_str()));
	strXML.AppendFormat(L"<HighIdleCO>%.2f</HighIdleCO>", (float)_wtof(sResultData.strCOHigh.c_str()));		//平台取值及显示是2位小数
	strXML.AppendFormat(L"<HighIdleHC>%.0f</HighIdleHC>", (float)_wtof(sResultData.strHCHigh.c_str()));
	strXML.AppendFormat(L"<Lambda>%.2f</Lambda>", (float)_wtof(sResultData.strLambda.c_str()));
	strXML.AppendFormat(L"<LowRev>%.0f</LowRev>", (float)_wtof(sResultData.strRevLow.c_str()));
	strXML.AppendFormat(L"<HighRev>%.0f</HighRev>", (float)_wtof(sResultData.strRevHigh.c_str()));

	CString strStartTime, strEndTime;
	if (vtRTData.size() >= 1)
	{
		strStartTime = vtRTData[0].strSamplingTime.c_str();
		strEndTime = vtRTData[vtRTData.size()-1].strSamplingTime.c_str();
	}
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", strStartTime);
	strXML.AppendFormat(L"<EndTime>%s</EndTime>", strEndTime);

	if (vtRTData.size() == 0)
	{
		// 没有数据，是停止的调用
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 9);
	}
	else
	{
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 0);
	}
	strXML.AppendFormat(L"<AdjustZero>%d</AdjustZero>", 1);	
	strXML.AppendFormat(L"<Result>%s</Result>", sResultData.strPass.c_str());
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	for (UINT i=0; i<vtRTData.size(); i++)
	{
		strXML.AppendFormat(L"<Row>");		
		strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", i+1);
		
		//工况类型。
		//0：低怠速检测中
		//1：高怠速检测中
		int nDataType(0);
		if (i < 45)
		{
			nDataType = 1;
		}
		else
		{
			nDataType = 0;
		}
		strXML.AppendFormat(L"<DataType>%d</DataType>", nDataType);
		strXML.AppendFormat(L"<Rev>%s</Rev>", vtRTData[i].strEngineRev.c_str());
		strXML.AppendFormat(L"<CO>%.2f</CO>", (float)_wtof(vtRTData[i].strCO.c_str()));		//平台取值及显示是2位小数
		strXML.AppendFormat(L"<HC>%.0f</HC>", (float)_wtof(vtRTData[i].strHC.c_str()));
		strXML.AppendFormat(L"<O2>%.2f</O2>", (float)_wtof(vtRTData[i].strO2.c_str()));
		strXML.AppendFormat(L"<CO2>%.2f</CO2>", (float)_wtof(vtRTData[i].strCO2.c_str()));
		strXML.AppendFormat(L"<Lambda>%.2f</Lambda>", (float)_wtof(vtRTData[i].strLambda.c_str()));
		strXML.AppendFormat(L"<JYWD>%.1f</JYWD>", (float)_wtof(vtRTData[i].strOilTemperature.c_str()));
		strXML.AppendFormat(L"<LowFlowFlag>%d</LowFlowFlag>", 0);
		strXML.AppendFormat(L"</Row>");
	}
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				if (xmlReader.OpenNode(L"Message"))
				{
					xmlReader.GetNodeAttribute(L"ReportNum", strReportNumber);
				}
			}
		}
		return true;
	}
	return false;
}
					//上传自由加速数据
bool CNSClientLib::UploadFSUNHT(SResultOfFSUNHT &sResultData, std::vector<SRealTimeDataOfFSUNHT> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"UploadDieselFAResult");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<Temperature>%.1f</Temperature>", (float)_wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strXML.AppendFormat(L"<AirPressure>%.1f</AirPressure>", (float)_wtof(sResultData.strAtmosphericPressure.c_str()));
	strXML.AppendFormat(L"<Humidity>%.1f</Humidity>", (float)_wtof(sResultData.strRelativeHumidity.c_str()));
	strXML.AppendFormat(L"<SmokeK1>%.2f</SmokeK1>", (float)_wtof(sResultData.strValue2.c_str()));
	strXML.AppendFormat(L"<SmokeK2>%.2f</SmokeK2>", (float)_wtof(sResultData.strValue3.c_str()));
	strXML.AppendFormat(L"<SmokeK3>%.2f</SmokeK3>", (float)_wtof(sResultData.strValue4.c_str()));
	strXML.AppendFormat(L"<SmokeAvg>%.2f</SmokeAvg>", (float)_wtof(sResultData.strAverage.c_str()));
	strXML.AppendFormat(L"<IdleRev>%.0f</IdleRev>", (float)_wtof(sResultData.strEngineRev.c_str()));

	CString strStartTime, strEndTime;
	if (vtRTData.size() >= 1)
	{
		strStartTime = vtRTData[0].strSamplingTime.c_str();
		strEndTime = vtRTData[vtRTData.size()-1].strSamplingTime.c_str();
	}
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", strStartTime);
	strXML.AppendFormat(L"<EndTime>%s</EndTime>", strEndTime);

	if (vtRTData.size() == 0)
	{
		// 没有数据，是停止的调用
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 9);
	}
	else
	{
		strXML.AppendFormat(L"<StopReason>%d</StopReason>", 0);
	}
	strXML.AppendFormat(L"<Result>%s</Result>", sResultData.strPass.c_str());
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	for (UINT i=0; i<vtRTData.size(); i++)
	{
		strXML.AppendFormat(L"<Row>");		
		strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", i+1);

		int nDataType = _wtoi(vtRTData[i].strOrder.c_str());
		strXML.AppendFormat(L"<DataType>%d</DataType>", nDataType);
		strXML.AppendFormat(L"<IdleRev>%s</IdleRev>", vtRTData[i].strEngineRev.c_str());
		strXML.AppendFormat(L"<SmokeK>%.2f</SmokeK>", (float)_wtof(vtRTData[i].strK.c_str()));
		strXML.AppendFormat(L"<JYWD>%.1f</JYWD>", (float)_wtof(vtRTData[i].strOilTemperature.c_str()));
		strXML.AppendFormat(L"<LowFlowFlag>%d</LowFlowFlag>", 0);
		strXML.AppendFormat(L"</Row>");
	}
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				if (xmlReader.OpenNode(L"Message"))
				{
					xmlReader.GetNodeAttribute(L"ReportNum", strReportNumber);
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::UploadOBD(SResultOfOBD &sResultOfOBD,
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber)
{
	CString str, strBuf, strFuelType;


	COleDateTime odtNow = COleDateTime::GetCurrentTime();
	COleDateTime oleDateTime;
	oleDateTime.ParseDateTime(sResultOfOBD.strDetBegTime.c_str());
	CString strTime = oleDateTime.Format(L"%H:%M:%S");
	CString strDate = oleDateTime.Format(L"%Y-%m-%d");
	
	strFuelType.AppendFormat(L"%s",sResultOfOBD.strFuelType.c_str());
	CString strXML;
	if (L"柴油" == strFuelType)
	{
		
		strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadDieselResult");

		//外观检测不需要上传

		strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");

		strXML.AppendFormat(L"<Result><Row><Info>");
		strXML.AppendFormat(L"<VIN>%s</VIN>",sResultOfOBD.strVIN == L"" ? L"0":sResultOfOBD.strVIN.c_str());
		strXML.AppendFormat(L"<XSJYOBDYQ>%s</XSJYOBDYQ>",sResultOfOBD.strOBDType == L"" ? L"0":sResultOfOBD.strOBDType.c_str());
		strXML.AppendFormat(L"<LJXSLC>%s</LJXSLC>",sResultOfOBD.strMileage == L"" ? L"0":sResultOfOBD.strMileage.c_str());
		strXML.AppendFormat(L"<FDJKZCVN>%s</FDJKZCVN>",sResultOfOBD.strEngineCVN == L"" ? L"0":sResultOfOBD.strEngineCVN.c_str());		
		strXML.AppendFormat(L"<HCLKZCALID>%s</HCLKZCALID>",sResultOfOBD.strPostProcessingCALID == L"" ? L"0":sResultOfOBD.strPostProcessingCALID.c_str());
		strXML.AppendFormat(L"<HCLKZCVN>%s</HCLKZCVN>",sResultOfOBD.strPostProcessingCVN == L"" ? L"0":sResultOfOBD.strPostProcessingCVN.c_str());
		strXML.AppendFormat(L"<QTKZCALID>%s</QTKZCALID>",sResultOfOBD.strOtherCALID == L"" ? L"0":sResultOfOBD.strOtherCALID.c_str());
		strXML.AppendFormat(L"<QTKZCVN>%s</QTKZCVN>",sResultOfOBD.strOtherCVN == L"" ? L"0":sResultOfOBD.strOtherCVN.c_str());
		strXML.AppendFormat(L"<JCSJ>%s %s</JCSJ>",strDate,strTime);
		strXML.AppendFormat(L"<GZZSQ>%s</GZZSQ>",sResultOfOBD.strLampStateJudge == L"" ? L"0":sResultOfOBD.strLampStateJudge.c_str());
		strXML.AppendFormat(L"<TXQK>%s</TXQK>",sResultOfOBD.strComErrorJudge == L"" ? L"0":sResultOfOBD.strComErrorJudge.c_str());
		strXML.AppendFormat(L"<GZZSQBJ>%s</GZZSQBJ>",sResultOfOBD.strLampStateJudge == L"" ? L"0":sResultOfOBD.strLampStateJudge.c_str());
		strXML.AppendFormat(L"<GZDMGZXX>%s</GZDMGZXX>",sResultOfOBD.strDTC == L"" ? L"0":sResultOfOBD.strDTC.c_str());
		strXML.AppendFormat(L"<JXZTWWCXM>0</JXZTWWCXM>");
		strXML.AppendFormat(L"<MILXSLC>%s</MILXSLC>",sResultOfOBD.strDTCMileage == L"" ? L"0":sResultOfOBD.strDTCMileage.c_str());
		strXML.AppendFormat(L"<JCJG>%s</JCJG>",sResultOfOBD.strJudge == L"" ? L"0":sResultOfOBD.strJudge.c_str());
		strXML.AppendFormat(L"<SFXYFJ>%s</SFXYFJ>",sResultOfOBD.strIsNeedReCheck == L"" ? L"0":sResultOfOBD.strIsNeedReCheck.c_str());
		strXML.AppendFormat(L"<FJNR>%s</FJNR>",sResultOfOBD.strReCheckItem == L"" ? L"0":sResultOfOBD.strReCheckItem.c_str());
		strXML.AppendFormat(L"<FJJG>%s</FJJG>",sResultOfOBD.strReCheckResult == L"" ? L"0":sResultOfOBD.strReCheckResult.c_str());
		strXML.AppendFormat(L"<TXJCJG>%s</TXJCJG>",sResultOfOBD.strComErrorJudge == L"" ? L"0":sResultOfOBD.strComErrorJudge.c_str());
		strXML.AppendFormat(L"<ZTBYZ>1</ZTBYZ>");
		strXML.AppendFormat(L"</Info></Row></Result>");
				
		strXML.AppendFormat(L"<FaultCode><Row><Info>");
		strXML.AppendFormat(L"<DQGZM>%s</DQGZM>",sResultOfOBD.strDTC.c_str());
		strXML.AppendFormat(L"<WJGZM>%s</WJGZM>",L"");
		strXML.AppendFormat(L"</Info></Row></FaultCode>");
			
		strXML.AppendFormat(L"<Readiness><Row><Info>",L"0");
		strXML.AppendFormat(L"<GZMGS>%s</GZMGS>",sResultOfOBD.strLampStateJudge == L"" ? L"0":sResultOfOBD.strLampStateJudge.c_str());
		strXML.AppendFormat(L"<GZDZT>%s</GZDZT>",sResultOfOBD.strCheckItemJudge_MFT == L"" ? L"0":sResultOfOBD.strCheckItemJudge_MFT.c_str());
		strXML.AppendFormat(L"<SHJC>%s</SHJC>",sResultOfOBD.strCheckItemJudge_FST == L"" ? L"0":sResultOfOBD.strCheckItemJudge_FST.c_str());
		strXML.AppendFormat(L"<RYXTJC>%s</RYXTJC>",sResultOfOBD.strCheckItemJudge_ICM == L"" ? L"0":sResultOfOBD.strCheckItemJudge_ICM.c_str());
		strXML.AppendFormat(L"<ZHBJJC>%s</ZHBJJC>");
		strXML.AppendFormat(L"<NMHCCHJJC>%s</NMHCCHJJC>",L"0");
		strXML.AppendFormat(L"<NOHCLJC>%s</NOHCLJC>",L"0");
		strXML.AppendFormat(L"<ZYYLXTJC>%s</ZYYLXTJC>",L"0");
		strXML.AppendFormat(L"<FQCGQJC>%s</FQCGQJC>",L"0");
		strXML.AppendFormat(L"<PMJC>%s</PMJC>",sResultOfOBD.strCheckItemJudge_POC == L"" || sResultOfOBD.strCheckItemJudge_POC == L"" ? L"0":sResultOfOBD.strCheckItemJudge_POC.c_str());
		strXML.AppendFormat(L"<EGRHVVTXTJC>%s</EGRHVVTXTJC>",sResultOfOBD.strCheckItemJudge_VVT == L"" ? L"0":sResultOfOBD.strCheckItemJudge_VVT.c_str());
		strXML.AppendFormat(L"</Info></Row></Readiness>"); 
						
		strXML.AppendFormat(L"<IUPR><Row><Info>"); 
		strXML.AppendFormat(L"<NMHCWCCS>%s</NMHCWCCS>",sResultOfOBD.strIUPR_NMHCC == L"不支持" ||sResultOfOBD.strIUPR_NMHCC == L"" ? L"0":sResultOfOBD.strIUPR_NMHCC.c_str());
		strXML.AppendFormat(L"<NMHCFHCS>%s</NMHCFHCS>",sResultOfOBD.strIUPR_NMHCEC == L"不支持"||sResultOfOBD.strIUPR_NMHCEC == L"" ? L"0":sResultOfOBD.strIUPR_NMHCEC.c_str());
		strXML.AppendFormat(L"<NMHCIUPRL>%.2f</NMHCIUPRL>",sResultOfOBD.strIUPR_NMHCEC == L""||sResultOfOBD.strIUPR_NMHCC == L""||sResultOfOBD.strIUPR_NMHCC == L"0"||sResultOfOBD.strIUPR_NMHCC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_NMHCEC.c_str())/_wtof(sResultOfOBD.strIUPR_NMHCC.c_str()));
		strXML.AppendFormat(L"<NOXCHQWCCS>%s</NOXCHQWCCS>",sResultOfOBD.strIUPR_NOXCC == L"" ? L"0":sResultOfOBD.strIUPR_NOXCC.c_str());
		strXML.AppendFormat(L"<NOXCHQFHCS>%s</NOXCHQFHCS>",sResultOfOBD.strIUPR_NOXCEC == L"" ? L"0":sResultOfOBD.strIUPR_NOXCEC.c_str());
		strXML.AppendFormat(L"<NOXCHQIUPRL>%.2f</NOXCHQIUPRL>",sResultOfOBD.strIUPR_NOXCEC == L""||sResultOfOBD.strIUPR_NOXCC == L""||sResultOfOBD.strIUPR_NOXCC == L"0"||sResultOfOBD.strIUPR_NOXCC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_NOXCEC.c_str())/_wtof(sResultOfOBD.strIUPR_NOXCC.c_str()));
		strXML.AppendFormat(L"<NOXXFQWCCS>%s</NOXXFQWCCS>",sResultOfOBD.strIUPR_NOXAC == L"" ? L"0":sResultOfOBD.strIUPR_NOXAC.c_str());
		strXML.AppendFormat(L"<NOXXFQFHCS>%s</NOXXFQFHCS>",sResultOfOBD.strIUPR_NOXAEC == L"" ? L"0":sResultOfOBD.strIUPR_NOXAEC.c_str());
		strXML.AppendFormat(L"<NOXXFQIUPRL>%.2f</NOXXFQIUPRL>",sResultOfOBD.strIUPR_NOXAEC == L""||sResultOfOBD.strIUPR_NOXAC == L""||sResultOfOBD.strIUPR_NOXAC == L"0"||sResultOfOBD.strIUPR_NOXAC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_NOXAEC.c_str())/_wtof(sResultOfOBD.strIUPR_NOXAC.c_str()));
		strXML.AppendFormat(L"<PMWCCS>%s</PMWCCS>",sResultOfOBD.strIUPR_PMC == L"" ? L"0":sResultOfOBD.strIUPR_PMC.c_str());
		strXML.AppendFormat(L"<PMFHCS>%s</PMFHCS>",sResultOfOBD.strIUPR_PMEC == L"" ? L"0":sResultOfOBD.strIUPR_PMEC.c_str());
		strXML.AppendFormat(L"<PMIUPRL>%.2f</PMIUPRL>",sResultOfOBD.strIUPR_PMEC == L""||sResultOfOBD.strIUPR_PMC == L""||sResultOfOBD.strIUPR_PMC == L"0"||sResultOfOBD.strIUPR_PMC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_PMEC.c_str())/_wtof(sResultOfOBD.strIUPR_PMC.c_str()));
		strXML.AppendFormat(L"<FQCGQWCCS>%s</FQCGQWCCS>",sResultOfOBD.strIUPR_WSC == L"" ? L"0":sResultOfOBD.strIUPR_WSC.c_str());
		strXML.AppendFormat(L"<FQCGQFHCS>%s</FQCGQFHCS>",sResultOfOBD.strIUPR_WSEC == L"" ? L"0":sResultOfOBD.strIUPR_WSEC.c_str());						
		strXML.AppendFormat(L"<FQCGQIUPRL>%.2f</FQCGQIUPRL>",sResultOfOBD.strIUPR_WSEC == L""||sResultOfOBD.strIUPR_WSC == L""||sResultOfOBD.strIUPR_WSC == L"0"||sResultOfOBD.strIUPR_WSC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_WSEC.c_str())/_wtof(sResultOfOBD.strIUPR_WSC.c_str()));
		strXML.AppendFormat(L"<EGRHVVTWCCS>%s</EGRHVVTWCCS>",sResultOfOBD.strIUPR_EGRC == L"" ? L"0":sResultOfOBD.strIUPR_EGRC.c_str());
		strXML.AppendFormat(L"<EGRHVVTFHCS>%s</EGRHVVTFHCS>",sResultOfOBD.strIUPR_EGREC == L"" ? L"0":sResultOfOBD.strIUPR_EGREC.c_str());
		strXML.AppendFormat(L"<EGRHVVTIUPRL>%.2f</EGRHVVTIUPRL>",sResultOfOBD.strIUPR_EGREC == L""||sResultOfOBD.strIUPR_EGRC == L""||sResultOfOBD.strIUPR_EGRC == L"0"||sResultOfOBD.strIUPR_EGRC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_EGREC.c_str())/_wtof(sResultOfOBD.strIUPR_EGRC.c_str()));
		strXML.AppendFormat(L"<ZYYLWCCS>%s</ZYYLWCCS>",sResultOfOBD.strIUPR_PPC == L"" ? L"0":sResultOfOBD.strIUPR_PPC.c_str());
		strXML.AppendFormat(L"<ZYYLFHCS>%s</ZYYLFHCS>",sResultOfOBD.strIUPR_PPEC == L"" ? L"0":sResultOfOBD.strIUPR_PPEC.c_str());
		strXML.AppendFormat(L"<ZYYLIUPRL>%.2f</ZYYLIUPRL>",sResultOfOBD.strIUPR_PPEC == L""||sResultOfOBD.strIUPR_PPC == L""||sResultOfOBD.strIUPR_PPC == L"0"||sResultOfOBD.strIUPR_PPC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_PPEC.c_str())/_wtof(sResultOfOBD.strIUPR_PPC.c_str()));
		strXML.AppendFormat(L"</Info></Row></IUPR>");		
	}
	else
	{		
		strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", m_strDeviceSN.c_str(), m_strOutlookID.c_str());
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadGasResult");
		//外观检测不需要上传

		strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");
	
		strXML.AppendFormat(L"<Result><Row><Info>");
		strXML.AppendFormat(L"<VIN>%s</VIN>",sResultOfOBD.strVIN == L"" ? L"0":sResultOfOBD.strVIN.c_str());
		strXML.AppendFormat(L"<XSJYOBDYQ>%s</XSJYOBDYQ>",sResultOfOBD.strOBDType == L"不支持" || sResultOfOBD.strOBDType == L"" ? L"0":sResultOfOBD.strOBDType.c_str());
		strXML.AppendFormat(L"<LJXSLC>%s</LJXSLC>",sResultOfOBD.strMileage == L"不支持"||sResultOfOBD.strMileage == L"" ? L"0":sResultOfOBD.strMileage.c_str());
		strXML.AppendFormat(L"<FDJKZCALID>%s</FDJKZCALID>",sResultOfOBD.strEngineCALID == L"不支持" ||sResultOfOBD.strEngineCALID == L"" ? L"0":sResultOfOBD.strEngineCALID.c_str());
		strXML.AppendFormat(L"<FDJKZCVN>%s</FDJKZCVN>",sResultOfOBD.strEngineCVN == L"不支持" ||sResultOfOBD.strEngineCVN == L"" ? L"0":sResultOfOBD.strEngineCVN.c_str());			
		strXML.AppendFormat(L"<HCLKZCALID>%s</HCLKZCALID>",sResultOfOBD.strPostProcessingCALID == L"不支持"||sResultOfOBD.strPostProcessingCALID == L"" ? L"0":sResultOfOBD.strPostProcessingCALID.c_str());
		strXML.AppendFormat(L"<HCLKZCVN>%s</HCLKZCVN>",sResultOfOBD.strPostProcessingCVN == L"不支持" || sResultOfOBD.strPostProcessingCVN == L"" ? L"0":sResultOfOBD.strPostProcessingCVN.c_str());
		strXML.AppendFormat(L"<QTKZCALID>%s</QTKZCALID>",sResultOfOBD.strOtherCALID == L"不支持" || sResultOfOBD.strOtherCALID == L"" ? L"0":sResultOfOBD.strOtherCALID.c_str());
		strXML.AppendFormat(L"<QTKZCVN>%s</QTKZCVN>",sResultOfOBD.strOtherCVN == L"" ? L"0":sResultOfOBD.strOtherCVN.c_str());
		strXML.AppendFormat(L"<JCSJ>%s %s</JCSJ>",strDate,strTime);	
		strXML.AppendFormat(L"<GZZSQ>%s</GZZSQ>",sResultOfOBD.strLampStateJudge == L"" ? L"0":sResultOfOBD.strLampStateJudge.c_str());
		strXML.AppendFormat(L"<TXQK>%s</TXQK>",sResultOfOBD.strComErrorJudge == L"" ? L"0":sResultOfOBD.strComErrorJudge.c_str());
		strXML.AppendFormat(L"<GZZSQBJ>%s</GZZSQBJ>",sResultOfOBD.strLampStateJudge == L"" ? L"0":sResultOfOBD.strLampStateJudge.c_str());
		strXML.AppendFormat(L"<GZDMGZXX>%s</GZDMGZXX>",sResultOfOBD.strDTC == L"" ? L"0":sResultOfOBD.strDTC.c_str());
		strXML.AppendFormat(L"<JXZTWWCXM>0</JXZTWWCXM>");
		strXML.AppendFormat(L"<MILXSLC>%s</MILXSLC>",sResultOfOBD.strDTCMileage == L"" ? L"0":sResultOfOBD.strDTCMileage.c_str());
		strXML.AppendFormat(L"<JCJG>%s</JCJG>",sResultOfOBD.strJudge == L"" ? L"0":sResultOfOBD.strJudge.c_str());
		strXML.AppendFormat(L"<SFXYFJ>%s</SFXYFJ>",sResultOfOBD.strIsNeedReCheck == L"" ? L"0":sResultOfOBD.strIsNeedReCheck.c_str());
		strXML.AppendFormat(L"<FJNR>%s</FJNR>",sResultOfOBD.strReCheckItem == L"" ? L"0":sResultOfOBD.strReCheckItem.c_str());
		strXML.AppendFormat(L"<FJJG>%s</FJJG>",sResultOfOBD.strReCheckResult == L"" ? L"0":sResultOfOBD.strReCheckResult.c_str());
		strXML.AppendFormat(L"<TXJCJG>%s</TXJCJG>",sResultOfOBD.strComErrorJudge == L"" ? L"0":sResultOfOBD.strComErrorJudge.c_str());
		strXML.AppendFormat(L"<ZTBYZ>1</ZTBYZ>");
		strXML.AppendFormat(L"</Info></Row></Result>");			
	
		strXML.AppendFormat(L"<FaultCode><Row><Info>");
		strXML.AppendFormat(L"<DQGZM>%s</DQGZM>",sResultOfOBD.strDTC == L"" ? L"0":sResultOfOBD.strDTC.c_str());
		strXML.AppendFormat(L"<WJGZM>%s</WJGZM>",L"0");
		strXML.AppendFormat(L"</Info></Row></FaultCode>");
				
		strXML.AppendFormat(L"<Readiness><Row><Info>");
		strXML.AppendFormat(L"<GZMGS>%s</GZMGS>",L"0");
		strXML.AppendFormat(L"<GZDZT>%s</GZDZT>",sResultOfOBD.strLampStateJudge == L"" ? L"0":sResultOfOBD.strLampStateJudge.c_str());
		strXML.AppendFormat(L"<SHJC>%s</SHJC>",sResultOfOBD.strCheckItemJudge_MFT == L"" ? L"0":sResultOfOBD.strCheckItemJudge_MFT.c_str());
		strXML.AppendFormat(L"<RYXTJC>%s</RYXTJC>",sResultOfOBD.strCheckItemJudge_FST == L"" ? L"0":sResultOfOBD.strCheckItemJudge_FST.c_str());
		strXML.AppendFormat(L"<ZHBJJC>%s</ZHBJJC>",sResultOfOBD.strCheckItemJudge_ICM == L"" ? L"0":sResultOfOBD.strCheckItemJudge_ICM.c_str());
		strXML.AppendFormat(L"<CHJJC>%s</CHJJC>",sResultOfOBD.strCheckItemJudge_CC == L"" ? L"0":sResultOfOBD.strCheckItemJudge_CC.c_str());
		strXML.AppendFormat(L"<JRSCHJJC>%s</JRSCHJJC>",sResultOfOBD.strCheckItemJudge_CCH == L"" ? L"0":sResultOfOBD.strCheckItemJudge_CCH.c_str());
		strXML.AppendFormat(L"<ZFXTJC>%s</ZFXTJC>",sResultOfOBD.strCheckItemJudge_EECS == L"" ? L"0":sResultOfOBD.strCheckItemJudge_EECS.c_str());
		strXML.AppendFormat(L"<ECKQJC>%s</ECKQJC>",sResultOfOBD.strCheckItemJudge_SAIS == L"" ? L"0":sResultOfOBD.strCheckItemJudge_SAIS.c_str());
		strXML.AppendFormat(L"<YCGQJC>%s</YCGQJC>",sResultOfOBD.strCheckItemJudge_OS == L"" ? L"0":sResultOfOBD.strCheckItemJudge_OS.c_str());
		strXML.AppendFormat(L"<YCGQJRQXTJC>%s</YCGQJRQXTJC>",sResultOfOBD.strCheckItemJudge_OSH == L"" ? L"0":sResultOfOBD.strCheckItemJudge_OSH.c_str());
		strXML.AppendFormat(L"<EGRHVVTXTJC>%s</EGRHVVTXTJC>",sResultOfOBD.strCheckItemJudge_EGR == L"" ? L"0":sResultOfOBD.strCheckItemJudge_EGR.c_str());
		strXML.AppendFormat(L"</Info></Row></Readiness>");						
	
		strXML.AppendFormat(L"<IUPR><Row><Info>");
		strXML.AppendFormat(L"<CHQZ1WCCS>%s</CHQZ1WCCS>",sResultOfOBD.strIUPR_CMCCB1 == L"不支持" || sResultOfOBD.strIUPR_CMCCB1 == L"" ? L"0":sResultOfOBD.strIUPR_CMCCB1.c_str());
		strXML.AppendFormat(L"<CHQZ1FHCS>%s</CHQZ1FHCS>",sResultOfOBD.strIUPR_CMCECB1 == L"不支持" || sResultOfOBD.strIUPR_CMCECB1 == L"" ? L"0":sResultOfOBD.strIUPR_CMCECB1.c_str());
		strXML.AppendFormat(L"<CHQZ1IUPRL>%.2f</CHQZ1IUPRL>",sResultOfOBD.strIUPR_CMCECB1 == L""|| sResultOfOBD.strIUPR_CMCCB1 == L""|| sResultOfOBD.strIUPR_CMCCB1 == L"0"|| sResultOfOBD.strIUPR_CMCCB1 == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str())/_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()));
		strXML.AppendFormat(L"<CHQZ2WCCS>%s</CHQZ2WCCS>",sResultOfOBD.strIUPR_CMCCB2 == L"不支持" || sResultOfOBD.strIUPR_CMCCB2 == L"" ? L"0":sResultOfOBD.strIUPR_CMCCB2.c_str());
		strXML.AppendFormat(L"<CHQZ2FHCS>%s</CHQZ2FHCS>",sResultOfOBD.strIUPR_CMCECB2 == L"不支持" || sResultOfOBD.strIUPR_CMCECB2 == L"" ? L"0":sResultOfOBD.strIUPR_CMCECB2.c_str());
		strXML.AppendFormat(L"<CHQZ2IUPRL>%.2f</CHQZ2IUPRL>",sResultOfOBD.strIUPR_CMCECB2 == L""||sResultOfOBD.strIUPR_CMCCB2 == L""||sResultOfOBD.strIUPR_CMCCB2 == L"0"||sResultOfOBD.strIUPR_CMCCB2 == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_CMCECB2.c_str())/_wtof(sResultOfOBD.strIUPR_CMCCB2.c_str()));
		strXML.AppendFormat(L"<QYCGQZ1WCCS>%s</QYCGQZ1WCCS>",sResultOfOBD.strIUPR_O2SMCCB1 == L"不支持" || sResultOfOBD.strIUPR_O2SMCCB1 == L"" ? L"0":sResultOfOBD.strIUPR_O2SMCCB1.c_str());					
		strXML.AppendFormat(L"<QYCGQZ1FHCS>%s</QYCGQZ1FHCS>",sResultOfOBD.strIUPR_O2SMCECB1 == L"不支持" || sResultOfOBD.strIUPR_O2SMCECB1 == L"" ? L"0":sResultOfOBD.strIUPR_O2SMCECB1.c_str());
		strXML.AppendFormat(L"<QYCGQZ1IUPRL>%.2f</QYCGQZ1IUPRL>",sResultOfOBD.strIUPR_O2SMCECB1 == L""||sResultOfOBD.strIUPR_O2SMCCB1 == L""||sResultOfOBD.strIUPR_O2SMCCB1 == L"0"||sResultOfOBD.strIUPR_O2SMCCB1 == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_O2SMCECB1.c_str())/_wtof(sResultOfOBD.strIUPR_O2SMCCB1.c_str()));
		strXML.AppendFormat(L"<QYCGQZ2WCCS>%s</QYCGQZ2WCCS>",sResultOfOBD.strIUPR_O2SMCCB2 == L"" ? L"0":sResultOfOBD.strIUPR_O2SMCCB2.c_str());
		strXML.AppendFormat(L"<QYCGQZ2FHCS>%s</QYCGQZ2FHCS>",sResultOfOBD.strIUPR_O2SMCECB2 == L"" ? L"0":sResultOfOBD.strIUPR_O2SMCECB2.c_str());
		strXML.AppendFormat(L"<QYCGQZ2IUPRL>%.2f</QYCGQZ2IUPRL>",sResultOfOBD.strIUPR_O2SMCECB2 == L""||sResultOfOBD.strIUPR_O2SMCCB2 == L""||sResultOfOBD.strIUPR_O2SMCCB2 == L"0"||sResultOfOBD.strIUPR_O2SMCCB2 == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_O2SMCECB2.c_str())/_wtof(sResultOfOBD.strIUPR_O2SMCCB2.c_str()));
		strXML.AppendFormat(L"<HYCGQZ1WCCS>%s</HYCGQZ1WCCS>",sResultOfOBD.strIUPR_O2SMCECB2 == L"" ? L"0":sResultOfOBD.strIUPR_O2SMCECB2.c_str());
		strXML.AppendFormat(L"<HYCGQZ1FHCS>%s</HYCGQZ1FHCS>",L"0");
		strXML.AppendFormat(L"<HYCGQZ1IUPRL>%s</HYCGQZ1IUPRL>",L"0");
		strXML.AppendFormat(L"<HYCGQZ2WCCS>%s</HYCGQZ2WCCS>",L"0");
		strXML.AppendFormat(L"<HYCGQZ2FHCS>%s</HYCGQZ2FHCS>",L"0");
		strXML.AppendFormat(L"<HYCGQZ2IUPRL>%s</HYCGQZ2IUPRL>",L"0");
		strXML.AppendFormat(L"<EVAPWCCS>%s</EVAPWCCS>",sResultOfOBD.strIUPR_EVAPC == L"不支持" || sResultOfOBD.strIUPR_EVAPC == L"" ? L"0":sResultOfOBD.strIUPR_EVAPC.c_str());
		strXML.AppendFormat(L"<EVAPFHCS>%s</EVAPFHCS>",sResultOfOBD.strIUPR_EVAPEC == L"不支持" || sResultOfOBD.strIUPR_EVAPEC == L"" ? L"0":sResultOfOBD.strIUPR_EVAPEC.c_str());
		strXML.AppendFormat(L"<EVAPIUPRL>%.2f</EVAPIUPRL>", sResultOfOBD.strIUPR_EVAPEC == L""||sResultOfOBD.strIUPR_EVAPC == L""||sResultOfOBD.strIUPR_EVAPC == L"0"||sResultOfOBD.strIUPR_EVAPC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str())/_wtof(sResultOfOBD.strIUPR_EVAPC.c_str()));
		strXML.AppendFormat(L"<EGRHVVTWCCS>%s</EGRHVVTWCCS>",sResultOfOBD.strIUPR_EGRC == L"" || sResultOfOBD.strIUPR_EGRC == L"不支持" ? L"0":sResultOfOBD.strIUPR_EGRC.c_str());
		strXML.AppendFormat(L"<EGRHVVTFHCS>%s</EGRHVVTFHCS>",sResultOfOBD.strIUPR_EGREC == L"" ||sResultOfOBD.strIUPR_EGREC == L"不支持"? L"0":sResultOfOBD.strIUPR_EGREC.c_str());
		strXML.AppendFormat(L"<EGRHVVTIUPRL>%.2f</EGRHVVTIUPRL>",sResultOfOBD.strIUPR_EGREC == L""||sResultOfOBD.strIUPR_EGRC == L""||sResultOfOBD.strIUPR_EGRC == L"0"||sResultOfOBD.strIUPR_EGRC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_EGREC.c_str())/_wtof(sResultOfOBD.strIUPR_EGRC.c_str()));
		strXML.AppendFormat(L"<GPFZ1WCCS>%s</GPFZ1WCCS>",sResultOfOBD.strIUPR_GPFC1 == L"" || sResultOfOBD.strIUPR_GPFC1 == L"不支持" ? L"0":sResultOfOBD.strIUPR_GPFC1.c_str());
		strXML.AppendFormat(L"<GPFZ1FHCS>%s</GPFZ1FHCS>",sResultOfOBD.strIUPR_GPFEC1 == L""|| sResultOfOBD.strIUPR_GPFEC1 == L"不支持" ? L"0":sResultOfOBD.strIUPR_GPFEC1.c_str());
		strXML.AppendFormat(L"<GPFZ1IUPRL>%.2f</GPFZ1IUPRL>",sResultOfOBD.strIUPR_GPFEC1 == L""||sResultOfOBD.strIUPR_GPFC1 == L""||sResultOfOBD.strIUPR_GPFC1 == L"0"||sResultOfOBD.strIUPR_GPFC1 == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str())/_wtof(sResultOfOBD.strIUPR_GPFC1.c_str()));
		strXML.AppendFormat(L"<GPFZ2WCCS>%s</GPFZ2WCCS>", sResultOfOBD.strIUPR_GPFC2 == L""|| sResultOfOBD.strIUPR_GPFC2 == L"不支持" ? L"0":sResultOfOBD.strIUPR_GPFC2.c_str());			
		strXML.AppendFormat(L"<GPFZ2FHCS>%s</GPFZ2FHCS>",sResultOfOBD.strIUPR_GPFEC2 == L""|| sResultOfOBD.strIUPR_GPFEC2 == L"不支持" ? L"0":sResultOfOBD.strIUPR_GPFEC2.c_str());
		strXML.AppendFormat(L"<GPFZ2IUPRL>%.2f</GPFZ2IUPRL>",sResultOfOBD.strIUPR_GPFEC2 == L""||sResultOfOBD.strIUPR_GPFC2 == L""||sResultOfOBD.strIUPR_GPFC2 == L"0"||sResultOfOBD.strIUPR_GPFC2 == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str())/_wtof(sResultOfOBD.strIUPR_GPFC2.c_str()));
		strXML.AppendFormat(L"<ECKQPSWCCS>%s</ECKQPSWCCS>",sResultOfOBD.strIUPR_AMCCC == L""|| sResultOfOBD.strIUPR_AMCCC == L"不支持" ? L"0":sResultOfOBD.strIUPR_AMCCC.c_str());
		strXML.AppendFormat(L"<ECKQPSFHCS>%s</ECKQPSFHCS>",sResultOfOBD.strIUPR_AMCEC == L""|| sResultOfOBD.strIUPR_AMCEC == L"不支持" ? L"0":sResultOfOBD.strIUPR_AMCEC.c_str());
		strXML.AppendFormat(L"<ECKQPSIUPRL>%.2f</ECKQPSIUPRL>",sResultOfOBD.strIUPR_AMCEC == L""||sResultOfOBD.strIUPR_AMCCC == L""||sResultOfOBD.strIUPR_AMCCC == L"0"||sResultOfOBD.strIUPR_AMCCC == L"不支持" ? 0.0:_wtof(sResultOfOBD.strIUPR_AMCEC.c_str())/_wtof(sResultOfOBD.strIUPR_AMCCC.c_str()));
		strXML.AppendFormat(L"</Info></Row></IUPR>");
	}
	strXML.AppendFormat(L"</Request></Message>");
	strXML.Replace(L"不支持",L"0");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
				if (xmlReader.OpenNode(L"Message"))
				{
					xmlReader.GetNodeAttribute(L"ReportNum", strReportNumber);
				}
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::DynCoastdown(std::wstring &strResult, std::wstring &strMsg)
{
	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"Coastdown");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<ACDT40>%.0f</ACDT40>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"ACDT1", L""))*1000);
	strXML.AppendFormat(L"<ACDT24>%.0f</ACDT24>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"ACDT2", L""))*1000);
	float fPLHP40 = 0.3f+rand()%10/100.0f;
	strXML.AppendFormat(L"<PLHP40>%.2f</PLHP40>", fPLHP40);
	float fPLHP24 = 0.1f+rand()%10/100.0f;
	strXML.AppendFormat(L"<PLHP24>%.2f</PLHP24>", fPLHP24);
	strXML.AppendFormat(L"<CCDT40>%.0f</CCDT40>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"CCDT1", L""))*1000);
	strXML.AppendFormat(L"<CCDT24>%.0f</CCDT24>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"CCDT2", L""))*1000);
	strXML.AppendFormat(L"<IHP40>%.1f</IHP40>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"ConstLoadPower1", L""))-fPLHP40);
	strXML.AppendFormat(L"<IHP24>%.1f</IHP24>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"ConstLoadPower2", L""))-fPLHP24);
	strXML.AppendFormat(L"<DIW>%.0f</DIW>", (float)_wtof(sio.GetString(L"DynamometerSystemCheck", L"DIW", L"")));
	strXML.AppendFormat(L"<Result40>%d</Result40>", 1);
	strXML.AppendFormat(L"<Result24>%d</Result24>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	strXML.AppendFormat(L"<Row>");
	//strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", 1);
	//strXML.AppendFormat(L"<Torque>%.2f</Torque>", 1);
	//strXML.AppendFormat(L"<Velocity>%.2f</Velocity>", 1);
	//strXML.AppendFormat(L"<Power>%.2f</Power>", 1);
	//strXML.AppendFormat(L"<Force>%.2f</Force>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::DynParasiticLose(std::wstring &strResult, std::wstring &strMsg)
{
	DEMARCATIONRESULTOFDYNPLHP sResultOfDynPLHP;
	ZeroMemory(&sResultOfDynPLHP, sizeof(DEMARCATIONRESULTOFDYNPLHP));
	GetIniResultOfDynPLHP(&sResultOfDynPLHP);

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"ParasiticLose");
	strXML.AppendFormat(L"<Result>");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));

	strXML.AppendFormat(L"<ACDT40>%.0f</ACDT40>", (float)_wtof(sResultOfDynPLHP.wchPLHPTime1));
	strXML.AppendFormat(L"<ACDT24>%.0f</ACDT24>", (float)_wtof(sResultOfDynPLHP.wchPLHPTime3));
	strXML.AppendFormat(L"<PLHP40>%.2f</PLHP40>", (float)_wtof(sResultOfDynPLHP.wchPLHP1));
	strXML.AppendFormat(L"<PLHP24>%.2f</PLHP24>", (float)_wtof(sResultOfDynPLHP.wchPLHP3));
	strXML.AppendFormat(L"<DIW>%d</DIW>", _wtoi(sResultOfDynPLHP.wchDIW));

	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Result>");
	strXML.AppendFormat(L"<ProcessData>");
	strXML.AppendFormat(L"<Row>");
	//strXML.AppendFormat(L"<TimeCount>%d</TimeCount>", 1);
	//strXML.AppendFormat(L"<Torque>%.2f</Torque>", 1);
	//strXML.AppendFormat(L"<Velocity>%.2f</Velocity>", 1);
	//strXML.AppendFormat(L"<Power>%.2f</Power>", 1);
	//strXML.AppendFormat(L"<Force>%.2f</Force>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</ProcessData>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::DynTorqueCal(std::wstring &strResult, std::wstring &strMsg)
{
	DEMARCATIONRESULTOFDYNFORCECHK sResultOfDynForceChk;
	ZeroMemory(&sResultOfDynForceChk, sizeof(DEMARCATIONRESULTOFDYNFORCECHK));
	GetIniResultOfDynForceChk(&sResultOfDynForceChk);

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"TorqueCal");

	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", (float)_wtof(sResultOfDynForceChk.wchStdValue1));
	strXML.AppendFormat(L"<Check>%.2f</Check>", (float)_wtof(sResultOfDynForceChk.wchForwardAvgValue1));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");

	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", (float)_wtof(sResultOfDynForceChk.wchStdValue1));
	strXML.AppendFormat(L"<Check>%.2f</Check>", (float)_wtof(sResultOfDynForceChk.wchBackwardAvgValue1));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");

	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", (float)_wtof(sResultOfDynForceChk.wchStdValue2));
	strXML.AppendFormat(L"<Check>%.2f</Check>", (float)_wtof(sResultOfDynForceChk.wchForwardAvgValue2));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");

	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", (float)_wtof(sResultOfDynForceChk.wchStdValue2));
	strXML.AppendFormat(L"<Check>%.2f</Check>", (float)_wtof(sResultOfDynForceChk.wchBackwardAvgValue2));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");

	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", (float)_wtof(sResultOfDynForceChk.wchStdValue3));
	strXML.AppendFormat(L"<Check>%.2f</Check>", (float)_wtof(sResultOfDynForceChk.wchForwardAvgValue3));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");

	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", (float)_wtof(sResultOfDynForceChk.wchStdValue3));
	strXML.AppendFormat(L"<Check>%.2f</Check>", (float)_wtof(sResultOfDynForceChk.wchBackwardAvgValue3));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");

	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::DynSpeedCal(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"SpeedCal");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));	
	strXML.AppendFormat(L"<Nominal>%.2f</Nominal>", 40.0f);
	strXML.AppendFormat(L"<Check>%.2f</Check>", 39.75f+rand()%50/100.0f);
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::AnaLeakTest(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"AnalyzerLeakTest");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::AnaO2RangeCheck(const float fO2, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"AnalyzerO2RangeCheck");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<jckssj>%s</jckssj>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	
	float fStdO2 = 20.8f;
	strXML.AppendFormat(L"<o2lcbz>%.2f</o2lcbz>", fStdO2);
	strXML.AppendFormat(L"<o2lcclz>%.2f</o2lcclz>", fO2);
	strXML.AppendFormat(L"<o2lcwc>%.2f</o2lcwc>", (fO2-fStdO2)/fStdO2*100.0f);
	strXML.AppendFormat(L"<jcjg>%d</jcjg>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::AnaCalCheck(std::wstring &strResult, std::wstring &strMsg)
{
	DEMARCATIONRESULTOFANAGASCHK sResultOfAnaGasChk;
	ZeroMemory(&sResultOfAnaGasChk, sizeof(DEMARCATIONRESULTOFANAGASCHK));
	GetIniResultOfAnaGasChk(&sResultOfAnaGasChk);

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"AnalyzerCalCheck");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));

	strXML.AppendFormat(L"<HCTag>%.0f</HCTag>", (float)_wtof(sResultOfAnaGasChk.wchHCStandardValue));
	strXML.AppendFormat(L"<COTag>%.3f</COTag>", (float)_wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strXML.AppendFormat(L"<CO2Tag>%.2f</CO2Tag>", (float)_wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strXML.AppendFormat(L"<NOTag>%.0f</NOTag>", (float)_wtof(sResultOfAnaGasChk.wchNOStandardValue));
	strXML.AppendFormat(L"<CheckHCTag>%.0f</CheckHCTag>", (float)_wtof(sResultOfAnaGasChk.wchHCStandardValue));
	strXML.AppendFormat(L"<CheckCOTag>%.3f</CheckCOTag>", (float)_wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strXML.AppendFormat(L"<CheckCO2Tag>%.2f</CheckCO2Tag>", (float)_wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strXML.AppendFormat(L"<CheckNOTag>%.0f</CheckNOTag>", (float)_wtof(sResultOfAnaGasChk.wchNOStandardValue));
	strXML.AppendFormat(L"<CheckO2Tag>%.2f</CheckO2Tag>", (float)_wtof(sResultOfAnaGasChk.wchO2StandardValue));
	strXML.AppendFormat(L"<HCCheckResult>%.0f</HCCheckResult>", (float)_wtof(sResultOfAnaGasChk.wchHCMeasuredValue));
	strXML.AppendFormat(L"<COCheckResult>%.3f</COCheckResult>", (float)_wtof(sResultOfAnaGasChk.wchCOMeasuredValue));
	strXML.AppendFormat(L"<CO2CheckResult>%.2f</CO2CheckResult>", (float)_wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strXML.AppendFormat(L"<NOCheckResult>%.0f</NOCheckResult>", (float)_wtof(sResultOfAnaGasChk.wchNOMeasuredValue));
	strXML.AppendFormat(L"<O2CheckResult>%.2f</O2CheckResult>", (float)_wtof(sResultOfAnaGasChk.wchO2MeasuredValue));

	strXML.AppendFormat(L"<HCT90>%.1f</HCT90>", 3.5f+rand()%10/10.0f);
	strXML.AppendFormat(L"<COT90>%.1f</COT90>", 3.5f+rand()%10/10.0f);
	strXML.AppendFormat(L"<CO2T90>%.1f</CO2T90>", 3.5f+rand()%10/10.0f);
	strXML.AppendFormat(L"<NOT90>%.1f</NOT90>", 4.5f+rand()%10/10.0f);
	strXML.AppendFormat(L"<O2T90>%.1f</O2T90>", 5.5f+rand()%10/10.0f);
	strXML.AppendFormat(L"<HCT10>%.1f</HCT10>", 3.7f+rand()%10/10.0f);
	strXML.AppendFormat(L"<COT10>%.1f</COT10>", 3.7f+rand()%10/10.0f);
	strXML.AppendFormat(L"<CO2T10>%.1f</CO2T10>", 3.7f+rand()%10/10.0f);
	strXML.AppendFormat(L"<NOT10>%.1f</NOT10>", 4.7f+rand()%10/10.0f);
	strXML.AppendFormat(L"<O2T10>%.1f</O2T10>", 6.5f+rand()%10/10.0f);

	strXML.AppendFormat(L"<PEF>%.3f</PEF>", (float)_wtof(sResultOfAnaGasChk.wchPEFMeasuredValue));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::AnaLowStandardGasCheck(std::wstring &strResult, std::wstring &strMsg)
{
	DEMARCATIONRESULTOFANAGASCHK sResultOfAnaGasChk;
	ZeroMemory(&sResultOfAnaGasChk, sizeof(DEMARCATIONRESULTOFANAGASCHK));
	GetIniResultOfAnaGasChk(&sResultOfAnaGasChk);

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"LowStandardGasCheck");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<jckssj>%s</jckssj>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strXML.AppendFormat(L"<c3h8nd>%.2f</c3h8nd>", (float)_wtof(sResultOfAnaGasChk.wchC3H8StandardValue));
	strXML.AppendFormat(L"<cond>%.2f</cond>", (float)_wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strXML.AppendFormat(L"<co2nd>%.2f</co2nd>", (float)_wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strXML.AppendFormat(L"<nond>%.2f</nond>", (float)_wtof(sResultOfAnaGasChk.wchNOStandardValue));
	strXML.AppendFormat(L"<o2nd>%.2f</o2nd>", (float)_wtof(sResultOfAnaGasChk.wchO2StandardValue));
	strXML.AppendFormat(L"<hcjcz>%.2f</hcjcz>", (float)_wtof(sResultOfAnaGasChk.wchHCMeasuredValue));
	strXML.AppendFormat(L"<cojcz>%.2f</cojcz>", (float)_wtof(sResultOfAnaGasChk.wchCOMeasuredValue));
	strXML.AppendFormat(L"<co2jcz>%.2f</co2jcz>", (float)_wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strXML.AppendFormat(L"<nojcz>%.2f</nojcz>", (float)_wtof(sResultOfAnaGasChk.wchNOMeasuredValue));
	strXML.AppendFormat(L"<o2jcz>%.2f</o2jcz>", (float)_wtof(sResultOfAnaGasChk.wchO2MeasuredValue));
	strXML.AppendFormat(L"<pef>%.2f</pef>", (float)_wtof(sResultOfAnaGasChk.wchPEFMeasuredValue));
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::AnaFullCal(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"AnalyzerFullCal");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));

	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::OpaCalChk(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"SmokemeterCal");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<StartTime>%s</StartTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));

	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::SelfcheckResult(float fET, float fRH, float fAP, std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"SelfcheckResult");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<SelfcheckTime>%s</SelfcheckTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strXML.AppendFormat(L"<Temperature>%.2f</Temperature>", fET);
	strXML.AppendFormat(L"<Humidity>%.2f</Humidity>", fRH);
	strXML.AppendFormat(L"<Atmosphere>%.2f</Atmosphere>", fAP);	
	strXML.AppendFormat(L"<BackgroundTemperature>%.2f</BackgroundTemperature>", fET);
	strXML.AppendFormat(L"<BackgroundHumidity>%.2f</BackgroundHumidity>", fRH);
	strXML.AppendFormat(L"<BackgroundAtmosphere>%.2f</BackgroundAtmosphere>", fAP);	
	strXML.AppendFormat(L"<TemperatureErrorRange>%.2f</TemperatureErrorRange>", fET);
	strXML.AppendFormat(L"<HumidityErrorRange>%.2f</HumidityErrorRange>", fRH);
	strXML.AppendFormat(L"<AtmosphereErrorRange>%.2f</AtmosphereErrorRange>", fAP);
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}

bool CNSClientLib::PreheatResult(std::wstring &strResult, std::wstring &strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", m_strDeviceSN.c_str());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"PreheatResult");
	strXML.AppendFormat(L"<Row>");
	strXML.AppendFormat(L"<PreheatTime>%s</PreheatTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strXML.AppendFormat(L"<Analyzer>%d</Analyzer>", 1);
	strXML.AppendFormat(L"<Dynamometer>%d</Dynamometer>", 1);
	strXML.AppendFormat(L"<Flow>%d</Flow>", 1);
	strXML.AppendFormat(L"<Smoker>%d</Smoker>", 1);
	strXML.AppendFormat(L"<Result>%d</Result>", 1);
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");
	
	std::wstring strRecv;
	if (SendAndRecvPacket(strXML, strRecv))
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (strResult == L"1")
			{
			}
		}
		return true;
	}
	return false;
}