#include "StdAfx.h"
#include "NanHuaWSOBD.h"

#include <vector>
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

#include "../NHDetCommModu/NHDetCommModu.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\OBDNHPadClientLib\NHPadClientLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHPadClientLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHPadClientLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

CNanHuaWSOBD::CNanHuaWSOBD(void)
{
	srand(unsigned int(time(NULL)));

	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;
	m_LastOPTime = clock();
	m_nGetRTDataTimeInterval = 500;
}

CNanHuaWSOBD::~CNanHuaWSOBD(void)
{
	// 对象销毁时,关闭串口资源
	Close();
}

//DWORD CNanHuaWSOBD::Open(BYTE bPort, const int nBaudRate)
DWORD CNanHuaWSOBD::Open(std::wstring strIP, const int nPort)
{
	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;

	assert(0 != nPort);

	CString strURL;
	strURL.Format(L"http://%s:%d", strIP.c_str(), nPort);
	PDAServiceLibAPI::GetInstance().SetUrl(strURL);
	
	SOBDRTData sOBDRTData;
	std::wstring strCode, strMsg;
	if (PDAServiceLibAPI::GetInstance().DownloadOBDRTData(L"123456789", sOBDRTData, strCode, strMsg))
	{
		CNHLogAPI::WriteLog(L"CNanHuaWSOBD::Open", L"连接成功", strURL);
		return COM_OPEN_SUCCEED;
	}
	else
	{
		CNHLogAPI::WriteLog(L"CNanHuaWSOBD::Open", L"连接失败", strURL);
		Close();
		return COM_OPEN_FAIL;
	}
}

DWORD CNanHuaWSOBD::Close(void)
{
	return true;
}

bool CNanHuaWSOBD::IsOpen(void) const
{
	return true;
}

bool CNanHuaWSOBD::Init(DWORD &dwProtocol, const int nFuelType)
{
	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;

	m_nFuelType = nFuelType;


	return true;
}


bool CNanHuaWSOBD::GetInitState(OBDInitState &enState, DWORD &dwProtocol)
{
	return false;
}

bool CNanHuaWSOBD::IsAdapterOnline(void)
{
	return true;
}

bool CNanHuaWSOBD::GetDTC(std::vector<SDTCData> &vtDTC)
{
	return false;
}

bool CNanHuaWSOBD::GetLampState(bool &bIsOn)
{
	return false;
}

bool CNanHuaWSOBD::GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem)
{
	return false;
}

bool CNanHuaWSOBD::ClearDTC(void)
{
	return false;
}

bool CNanHuaWSOBD::GetRTData(int &nRev, float &fV, float &fTemp)
{
	std::map<std::wstring, std::wstring> mapData;
	bool bRet = GetRTData(mapData);
	if (bRet)
	{
		m_fV = (float)_wtof(mapData[L"Velocity"].c_str());
		m_fTemp = (float)_wtof(mapData[L"CoolantTemp"].c_str());
		m_nRev = _wtoi(mapData[L"EngineRev"].c_str());

		nRev = m_nRev;
		fV = m_fV;
		fTemp = m_fTemp;

		return true;
	}
	return false;
}

bool CNanHuaWSOBD::GetRTData(std::map<std::wstring, std::wstring> &mapData)
{
	if ((clock()-m_LastOPTime) > m_nGetRTDataTimeInterval)	// 强制在底层限制采样命令的发送间隔
	{
		TRACE("超过时间间隔，访问数据\r\n");
		m_LastOPTime = clock();

		SOBDRTData sOBDRTData;
		std::wstring strCode, strMsg;
		if (PDAServiceLibAPI::GetInstance().DownloadOBDRTData(PDAServiceLibAPI::GetInstance().GetRunningNumber(), sOBDRTData, strCode, strMsg))
		{
			mapData.clear();
			if (strCode == L"1")
			{
				mapData[L"Velocity"] = sOBDRTData.strVelocity;
				mapData[L"CoolantTemp"] = sOBDRTData.strCoolantTemp;
				mapData[L"EngineRev"] = sOBDRTData.strEngineRev;
				mapData[L"SolarTermDoorPosition"] = sOBDRTData.strSolarTermDoorPosition;
				mapData[L"ThrottleOpening"] = sOBDRTData.strThrottleOpening;
				mapData[L"CalculationLoad"] = sOBDRTData.strCalculationLoad;
				mapData[L"ForwardSensorV"] = sOBDRTData.strForwardSensorV;
				mapData[L"ForwardSensorI"] = sOBDRTData.strForwardSensorI;
				mapData[L"AirCoefficient"] = sOBDRTData.strAirCoefficient;
				mapData[L"MAF"] = sOBDRTData.strMAF;
				mapData[L"MAP"] = sOBDRTData.strMAP;
				mapData[L"EngineOutputPower"] = sOBDRTData.strEngineOutputPower;
				mapData[L"ChargeAirPressure"] = sOBDRTData.strChargeAirPressure;
				mapData[L"FuelConsumption"] = sOBDRTData.strFuelConsumption;
				mapData[L"NOXConcentration"] = sOBDRTData.strNOXConcentration;
				mapData[L"UreaInjectionVolume"] = sOBDRTData.strUreaInjectionVolume;
				mapData[L"EGT"] = sOBDRTData.strEGT;
				mapData[L"DPFDifferentialPressure"] = sOBDRTData.strDPFDifferentialPressure;
				mapData[L"EGRPosition"] = sOBDRTData.strEGRPosition;
				mapData[L"FuelDeliveryPressure"] = sOBDRTData.strFuelDeliveryPressure;

				m_fV = (float)_wtof(mapData[L"Velocity"].c_str());
				m_fTemp = (float)_wtof(mapData[L"CoolantTemp"].c_str());
				m_nRev = _wtoi(mapData[L"EngineRev"].c_str());
				m_mapRTData = mapData;
				return true;
			}
			else
			{
				m_fV = 0;
				m_fTemp = 0;
				m_nRev = 0;
				m_mapRTData.clear();
			}
		}
		else
		{
			m_fV = 0;
			m_fTemp = 0;
			m_nRev = 0;
			m_mapRTData.clear();
		}
	}
	else
	{
		TRACE("未到时间间隔，发缓存数据\r\n");
		mapData = m_mapRTData;
		return true;
	}
	return false;
}

bool CNanHuaWSOBD::GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	return false;
}

bool CNanHuaWSOBD::GetDTCMileage(DWORD &dwMileage)
{
	return false;
}

bool CNanHuaWSOBD::GetFreezeData(std::wstring &strData)
{
	return false;
}

bool CNanHuaWSOBD::GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode/*=NULL*/)
{
	return false;
}

bool CNanHuaWSOBD::GetOBDType(std::wstring &strType)
{
	return false;
}

bool CNanHuaWSOBD::GetMileage(DWORD &dwMileage)
{
	return false;
}

bool CNanHuaWSOBD::GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	return false;
}

bool CNanHuaWSOBD::GetSoftwareVer(std::wstring &strVer)
{
	return false;
}

bool CNanHuaWSOBD::UnInit(void)
{
	return false;
}

std::wstring CNanHuaWSOBD::GetOBDProtocolName(DWORD dwProtocol)
{
	switch (dwProtocol)
	{
	case 1:		return L"KWP2000 FastInit";
	case 2:		return L"KWP2000 5baudInit";
	case 3:		return L"ISO9141-2 5baudInit";
	case 4:		return L"标准CAN ISO15765";
	case 5:		return L"扩展CAN ISO15765";
	case 6:		return L"VPW";
	case 7:		return L"PWM";
	case 8:		return L"J1939";
	case 9:		return L"KWP1281";
	case 10:	return L"Audi CAN";
	default:	return L"未知协议";
	}
}