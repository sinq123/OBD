#include "StdAfx.h"
#include "PDAServiceLibAPI.h"

#include "gsoap/soapNHOBDProxy.h"
#include "gsoap/NHOBD.nsmap"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

PDAServiceLibAPI& PDAServiceLibAPI::GetInstance(void)
{
	static std::auto_ptr<PDAServiceLibAPI> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new PDAServiceLibAPI());
	}
	return *(auto_ptr_instance.get());
}

PDAServiceLibAPI::PDAServiceLibAPI(void)
{
	m_strURL = "http://127.0.0.1:8080";
}

PDAServiceLibAPI::~PDAServiceLibAPI(void)
{
}

bool PDAServiceLibAPI::DownloadOBDDetList(LPCTSTR szPlateNumber, LPCTSTR szPlateType, std::vector<SDetInfo> &vtDetInfo, std::wstring &strCode, std::wstring &strMsg)
{
	NHOBDProxy webService(m_strURL.c_str(), SOAP_C_UTFSTRING);
	
	CString strTemp;
	CString strXML;
	strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	strXML.AppendFormat(L"<root><body>");
	strXML.AppendFormat(L"<PlateNumber>%s</PlateNumber>", szPlateNumber);
	strXML.AppendFormat(L"<PlateType>%s</PlateType>", szPlateType);
	strXML.AppendFormat(L"</body></root>");

	std::wstring strJkid = L"Q01";
	std::wstring strXml = (LPCTSTR)strXML;
	std::wstring strRet;
	int nRet = webService.queryObjectOut(strJkid, strXml, strRet);
	if (nRet == SOAP_OK)
	{
		strTemp.Format(L"接口[%s]访问成功(%d)\r\n%s\r\n", strJkid.c_str(), nRet, strRet.c_str());
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::DownloadOBDDetList", strTemp);

		// 解析出的字段
		std::wstring strTemp;
		// XML解析
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (xmlReader.OpenNode(L"root/head/message"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (xmlReader.OpenNode(L"root/body/vehinfo"))
			{
				do
				{
					SDetInfo sDetInfo;
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

					sDetInfo.strRunningNumber = mapField[L"RunningNumber"];
					sDetInfo.strPlateNumber = mapField[L"PlateNumber"];
					sDetInfo.strPlateType = mapField[L"PlateType"];
					sDetInfo.strFuelType = mapField[L"FuelType"];
					sDetInfo.strItemApp = mapField[L"ItemApp"];
					sDetInfo.strItemOBD = mapField[L"ItemOBD"];
					sDetInfo.strItemEmi = mapField[L"ItemEmi"];
					vtDetInfo.push_back(sDetInfo);
				}
				while (xmlReader.NextSibling());
			}
		}
		return true;
	}
	else
	{
		strTemp.Format(L"接口[%s]访问失败(%d)\r\n", strJkid.c_str(), nRet);
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::DownloadOBDDetList", strTemp);
		return false;
	}
}

bool PDAServiceLibAPI::UploadOBDDetData(LPCTSTR szRunningNumber, SResultOfOBD &sResultOfOBD, std::wstring &strCode, std::wstring &strMsg)
{
	NHOBDProxy webService(m_strURL.c_str(), SOAP_C_UTFSTRING);
	
	CString strTemp;
	CString strXML;
	strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	strXML.AppendFormat(L"<root><body>");
	strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", szRunningNumber);
	strXML.AppendFormat(L"<Protocol>%s</Protocol>", sResultOfOBD.strOBDProtocol.c_str());
	strXML.AppendFormat(L"<DetBegTime>%s</DetBegTime>", sResultOfOBD.strDetBegTime.c_str());
	strXML.AppendFormat(L"<DetEndTime>%s</DetEndTime>", sResultOfOBD.strDetEndTime.c_str());
	strXML.AppendFormat(L"<ComErrorJudge>%s</ComErrorJudge>", sResultOfOBD.strComErrorJudge.c_str());
	strXML.AppendFormat(L"<ComErrorComment>%s</ComErrorComment>", sResultOfOBD.strComErrorComment.c_str());
	strXML.AppendFormat(L"<DisplayJudge>%s</DisplayJudge>", sResultOfOBD.strDisplayJudge.c_str());
	strXML.AppendFormat(L"<LampStateJudge>%s</LampStateJudge>", sResultOfOBD.strLampStateJudge.c_str());
	strXML.AppendFormat(L"<DTC>%s</DTC>", sResultOfOBD.strDTC.c_str());
	strXML.AppendFormat(L"<DTCMileage>%s</DTCMileage>", sResultOfOBD.strDTCMileage.c_str());
	strXML.AppendFormat(L"<FreezeData>%s</FreezeData>", sResultOfOBD.strFreezeData.c_str());
	strXML.AppendFormat(L"<VIN>%s</VIN>", sResultOfOBD.strVIN.c_str());
	strXML.AppendFormat(L"<OBDType>%s</OBDType>", sResultOfOBD.strOBDType.c_str());
	strXML.AppendFormat(L"<Mileage>%s</Mileage>", sResultOfOBD.strMileage.c_str());
	strXML.AppendFormat(L"<EngineCALID>%s</EngineCALID>", sResultOfOBD.strEngineCALID.c_str());
	strXML.AppendFormat(L"<EngineCVN>%s</EngineCVN>", sResultOfOBD.strEngineCVN.c_str());
	strXML.AppendFormat(L"<PostProcessingCALID>%s</PostProcessingCALID>", sResultOfOBD.strPostProcessingCALID.c_str());
	strXML.AppendFormat(L"<PostProcessingCVN>%s</PostProcessingCVN>", sResultOfOBD.strPostProcessingCVN.c_str());
	strXML.AppendFormat(L"<OtherCALID>%s</OtherCALID>", sResultOfOBD.strOtherCALID.c_str());
	strXML.AppendFormat(L"<OtherCVN>%s</OtherCVN>", sResultOfOBD.strOtherCVN.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_MFT>%s</CheckItemJudge_MFT>", sResultOfOBD.strCheckItemJudge_MFT.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_FST>%s</CheckItemJudge_FST>", sResultOfOBD.strCheckItemJudge_FST.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_CC>%s</CheckItemJudge_CC>", sResultOfOBD.strCheckItemJudge_CC.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_CCH>%s</CheckItemJudge_CCH>", sResultOfOBD.strCheckItemJudge_CCH.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_OS>%s</CheckItemJudge_OS>", sResultOfOBD.strCheckItemJudge_OS.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_OSH>%s</CheckItemJudge_OSH>", sResultOfOBD.strCheckItemJudge_OSH.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_EECS>%s</CheckItemJudge_EECS>", sResultOfOBD.strCheckItemJudge_EECS.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_EGR>%s</CheckItemJudge_EGR>", sResultOfOBD.strCheckItemJudge_EGR.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_SAIS>%s</CheckItemJudge_SAIS>", sResultOfOBD.strCheckItemJudge_SAIS.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_ICM>%s</CheckItemJudge_ICM>", sResultOfOBD.strCheckItemJudge_ICM.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_VVT>%s</CheckItemJudge_VVT>", sResultOfOBD.strCheckItemJudge_VVT.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_DOC>%s</CheckItemJudge_DOC>", sResultOfOBD.strCheckItemJudge_DOC.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_SCR>%s</CheckItemJudge_SCR>", sResultOfOBD.strCheckItemJudge_SCR.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_DPF>%s</CheckItemJudge_DPF>", sResultOfOBD.strCheckItemJudge_DPF.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_ASC>%s</CheckItemJudge_ASC>", sResultOfOBD.strCheckItemJudge_ASC.c_str());
	strXML.AppendFormat(L"<CheckItemJudge_POC>%s</CheckItemJudge_POC>", sResultOfOBD.strCheckItemJudge_POC.c_str());
	strXML.AppendFormat(L"<IUPR_OMCEC>%s</IUPR_OMCEC>", sResultOfOBD.strIUPR_OMCEC.c_str());
	strXML.AppendFormat(L"<IUPR_ICC>%s</IUPR_ICC>", sResultOfOBD.strIUPR_ICC.c_str());
	strXML.AppendFormat(L"<IUPR_CMCCB1>%s</IUPR_CMCCB1>", sResultOfOBD.strIUPR_CMCCB1.c_str());
	strXML.AppendFormat(L"<IUPR_CMCECB1>%s</IUPR_CMCECB1>", sResultOfOBD.strIUPR_CMCECB1.c_str());
	strXML.AppendFormat(L"<IUPR_CMCCB2>%s</IUPR_CMCCB2>", sResultOfOBD.strIUPR_CMCCB2.c_str());
	strXML.AppendFormat(L"<IUPR_CMCECB2>%s</IUPR_CMCECB2>", sResultOfOBD.strIUPR_CMCECB2.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCCB1>%s</IUPR_O2SMCCB1>", sResultOfOBD.strIUPR_O2SMCCB1.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCECB1>%s</IUPR_O2SMCECB1>", sResultOfOBD.strIUPR_O2SMCECB1.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCCB2>%s</IUPR_O2SMCCB2>", sResultOfOBD.strIUPR_O2SMCCB2.c_str());
	strXML.AppendFormat(L"<IUPR_O2SMCECB2>%s</IUPR_O2SMCECB2>", sResultOfOBD.strIUPR_O2SMCECB2.c_str());
	strXML.AppendFormat(L"<IUPR_EGRC>%s</IUPR_EGRC>", sResultOfOBD.strIUPR_EGRC.c_str());
	strXML.AppendFormat(L"<IUPR_EGREC>%s</IUPR_EGREC>", sResultOfOBD.strIUPR_EGREC.c_str());
	strXML.AppendFormat(L"<IUPR_GPFC1>%s</IUPR_GPFC1>", sResultOfOBD.strIUPR_GPFC1.c_str());
	strXML.AppendFormat(L"<IUPR_GPFEC1>%s</IUPR_GPFEC1>", sResultOfOBD.strIUPR_GPFEC1.c_str());
	strXML.AppendFormat(L"<IUPR_GPFC2>%s</IUPR_GPFC2>", sResultOfOBD.strIUPR_GPFC2.c_str());
	strXML.AppendFormat(L"<IUPR_GPFEC2>%s</IUPR_GPFEC2>", sResultOfOBD.strIUPR_GPFEC2.c_str());
	strXML.AppendFormat(L"<IUPR_AMCCC>%s</IUPR_AMCCC>", sResultOfOBD.strIUPR_AMCCC.c_str());
	strXML.AppendFormat(L"<IUPR_AMCEC>%s</IUPR_AMCEC>", sResultOfOBD.strIUPR_AMCEC.c_str());
	strXML.AppendFormat(L"<IUPR_EVAPC>%s</IUPR_EVAPC>", sResultOfOBD.strIUPR_EVAPC.c_str());
	strXML.AppendFormat(L"<IUPR_EVAPEC>%s</IUPR_EVAPEC>", sResultOfOBD.strIUPR_EVAPEC.c_str());
	strXML.AppendFormat(L"<IUPR_NMHCC>%s</IUPR_NMHCC>", sResultOfOBD.strIUPR_NMHCC.c_str());
	strXML.AppendFormat(L"<IUPR_NMHCEC>%s</IUPR_NMHCEC>", sResultOfOBD.strIUPR_NMHCEC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXCC>%s</IUPR_NOXCC>", sResultOfOBD.strIUPR_NOXCC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXCEC>%s</IUPR_NOXCEC>", sResultOfOBD.strIUPR_NOXCEC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXAC>%s</IUPR_NOXAC>", sResultOfOBD.strIUPR_NOXAC.c_str());
	strXML.AppendFormat(L"<IUPR_NOXAEC>%s</IUPR_NOXAEC>", sResultOfOBD.strIUPR_NOXAEC.c_str());
	strXML.AppendFormat(L"<IUPR_PMC>%s</IUPR_PMC>", sResultOfOBD.strIUPR_PMC.c_str());
	strXML.AppendFormat(L"<IUPR_PMEC>%s</IUPR_PMEC>", sResultOfOBD.strIUPR_PMEC.c_str());
	strXML.AppendFormat(L"<IUPR_WSC>%s</IUPR_WSC>", sResultOfOBD.strIUPR_WSC.c_str());
	strXML.AppendFormat(L"<IUPR_WSEC>%s</IUPR_WSEC>", sResultOfOBD.strIUPR_WSEC.c_str());
	strXML.AppendFormat(L"<IUPR_PPC>%s</IUPR_PPC>", sResultOfOBD.strIUPR_PPC.c_str());
	strXML.AppendFormat(L"<IUPR_PPEC>%s</IUPR_PPEC>", sResultOfOBD.strIUPR_PPEC.c_str());
	strXML.AppendFormat(L"<Judge>%s</Judge>", sResultOfOBD.strJudge.c_str());
	strXML.AppendFormat(L"<Operator>%s</Operator>", sResultOfOBD.strOperator.c_str());
	strXML.AppendFormat(L"</body></root>");

	std::wstring strJkid = L"W01";
	std::wstring strXml = (LPCTSTR)strXML;
	std::wstring strRet;
	int nRet = webService.queryObjectOut(strJkid, strXml, strRet);
	if (nRet == SOAP_OK)
	{
		strTemp.Format(L"接口[%s]访问成功(%d)\r\n%s\r\n", strJkid.c_str(), nRet, strRet.c_str());
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::UploadOBDDetData", strTemp);

		return true;
	}
	else
	{
		strTemp.Format(L"接口[%s]访问失败(%d)\r\n", strJkid.c_str(), nRet);
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::UploadOBDDetData", strTemp);
		return false;
	}
}

bool PDAServiceLibAPI::UploadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg)
{
	NHOBDProxy webService(m_strURL.c_str(), SOAP_C_UTFSTRING);
	
	CString strTemp;
	CString strXML;
	strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	strXML.AppendFormat(L"<root><body>");
	strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", szRunningNumber);
	strXML.AppendFormat(L"<Velocity>%s</Velocity>", sOBDRTData.strVelocity.c_str());
	strXML.AppendFormat(L"<CoolantTemp>%s</CoolantTemp>", sOBDRTData.strCoolantTemp.c_str());
	strXML.AppendFormat(L"<EngineRev>%s</EngineRev>", sOBDRTData.strEngineRev.c_str());
	strXML.AppendFormat(L"<SolarTermDoorPosition>%s</SolarTermDoorPosition>", sOBDRTData.strSolarTermDoorPosition.c_str());
	strXML.AppendFormat(L"<ThrottleOpening>%s</ThrottleOpening>", sOBDRTData.strThrottleOpening.c_str());
	strXML.AppendFormat(L"<CalculationLoad>%s</CalculationLoad>", sOBDRTData.strCalculationLoad.c_str());
	strXML.AppendFormat(L"<AirCoefficient>%s</AirCoefficient>", sOBDRTData.strAirCoefficient.c_str());
	strXML.AppendFormat(L"<ForwardSensorV>%s</ForwardSensorV>", sOBDRTData.strForwardSensorV.c_str());
	strXML.AppendFormat(L"<ForwardSensorI>%s</ForwardSensorI>", sOBDRTData.strForwardSensorI.c_str());
	strXML.AppendFormat(L"<MAF>%s</MAF>", sOBDRTData.strMAF.c_str());
	strXML.AppendFormat(L"<MAP>%s</MAP>", sOBDRTData.strMAP.c_str());
	strXML.AppendFormat(L"<EngineOutputPower>%s</EngineOutputPower>", sOBDRTData.strEngineOutputPower.c_str());
	strXML.AppendFormat(L"<ChargeAirPressure>%s</ChargeAirPressure>", sOBDRTData.strChargeAirPressure.c_str());
	strXML.AppendFormat(L"<FuelConsumption>%s</FuelConsumption>", sOBDRTData.strFuelConsumption.c_str());
	strXML.AppendFormat(L"<NOXConcentration>%s</NOXConcentration>", sOBDRTData.strNOXConcentration.c_str());
	strXML.AppendFormat(L"<UreaInjectionVolume>%s</UreaInjectionVolume>", sOBDRTData.strUreaInjectionVolume.c_str());
	strXML.AppendFormat(L"<EGT>%s</EGT>", sOBDRTData.strEGT.c_str());
	strXML.AppendFormat(L"<DPFDifferentialPressure>%s</DPFDifferentialPressure>", sOBDRTData.strDPFDifferentialPressure.c_str());
	strXML.AppendFormat(L"<EGRPosition>%s</EGRPosition>", sOBDRTData.strEGRPosition.c_str());
	strXML.AppendFormat(L"<FuelDeliveryPressure>%s</FuelDeliveryPressure>", sOBDRTData.strFuelDeliveryPressure.c_str());
	strXML.AppendFormat(L"</body></root>");

	std::wstring strJkid = L"W02";
	std::wstring strXml = (LPCTSTR)strXML;
	std::wstring strRet;
	int nRet = webService.queryObjectOut(strJkid, strXml, strRet);
	if (nRet == SOAP_OK)
	{
		strTemp.Format(L"接口[%s]访问成功(%d)\r\n%s\r\n", strJkid.c_str(), nRet, strRet.c_str());
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::UploadOBDRTData", strTemp);

		return true;
	}
	else
	{
		strTemp.Format(L"接口[%s]访问失败(%d)\r\n", strJkid.c_str(), nRet);
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::UploadOBDRTData", strTemp);
		return false;
	}
}

bool PDAServiceLibAPI::DownloadOBDRTData(LPCTSTR szRunningNumber, SOBDRTData &sOBDRTData, std::wstring &strCode, std::wstring &strMsg)
{
	NHOBDProxy webService(m_strURL.c_str(), SOAP_C_UTFSTRING);
	webService.recv_timeout = 1;		//接收 (单位是秒)
	webService.send_timeout = 1;		//发送 (单位是秒)
	webService.connect_timeout = 1;		//连接 (单位是秒)

	CString strTemp;
	CString strXML;
	strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	strXML.AppendFormat(L"<root><body>");
	strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", szRunningNumber);
	strXML.AppendFormat(L"</body></root>");

	std::wstring strJkid = L"Q02";
	std::wstring strXml = (LPCTSTR)strXML;
	std::wstring strRet;
	int nRet = webService.queryObjectOut(strJkid, strXml, strRet);
	if (nRet == SOAP_OK)
	{
		strTemp.Format(L"接口[%s]访问成功(%d)\r\n%s\r\n", strJkid.c_str(), nRet, strRet.c_str());
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::DownloadOBDRTData", strTemp);
		
		// 解析出的字段
		std::wstring strTemp;
		// XML解析
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (xmlReader.OpenNode(L"root/head/message"))
			{
				xmlReader.GetNodeContent(strMsg);
			}
			if (xmlReader.OpenNode(L"root/body/RunningNumber"))
			{
				xmlReader.GetNodeContent(strTemp);
			}
			if (xmlReader.OpenNode(L"root/body/Velocity"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strVelocity = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/Velocity"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strVelocity = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/CoolantTemp"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strCoolantTemp = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/EngineRev"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strEngineRev = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/SolarTermDoorPosition"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strSolarTermDoorPosition = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/ThrottleOpening"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strThrottleOpening = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/CalculationLoad"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strCalculationLoad = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/AirCoefficient"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strAirCoefficient = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/ForwardSensorV"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strForwardSensorV = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/ForwardSensorI"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strForwardSensorI = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/MAF"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strMAF = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/MAP"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strMAP = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/EngineOutputPower"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strEngineOutputPower = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/ChargeAirPressure"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strChargeAirPressure = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/FuelConsumption"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strFuelConsumption = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/NOXConcentration"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strNOXConcentration = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/UreaInjectionVolume"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strUreaInjectionVolume = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/EGT"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strEGT = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/DPFDifferentialPressure"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strDPFDifferentialPressure = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/EGRPosition"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strEGRPosition = strTemp;
			}
			if (xmlReader.OpenNode(L"root/body/FuelDeliveryPressure"))
			{
				xmlReader.GetNodeContent(strTemp);
				sOBDRTData.strFuelDeliveryPressure = strTemp;
			}
		}
		return true;
	}
	else
	{
		strTemp.Format(L"接口[%s]访问失败(%d)\r\n", strJkid.c_str(), nRet);
		CNHLogAPI::WriteLog(LOG_MSG, L"PDAServiceLibAPI::DownloadOBDRTData", strTemp);
		return false;
	}
}
