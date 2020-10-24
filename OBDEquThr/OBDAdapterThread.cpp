// OBDAdapterThread.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"

#include "OBDAdapterThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COBDAdapterThread, CWinThread)

COBDAdapterThread::COBDAdapterThread(void)
	: m_bQuitThread(false)
	, m_pAdapter(NULL)
{
	// 对象创建时,初始化设备
	InitializeInstrument(L"ZENYUAN");

	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;
	m_fSolarTermDoorPosition = 0;
	m_fCalculationLoad = 0;
	m_fAirCoefficient = 0;
	m_fForwardSensorV = 0;
	m_fForwardSensorI = 0;
	m_fMAF = 0;
	m_fMAP = 0;
	m_fEngineOutputPower = 0;
	m_fChargeAirPressure = 0;
	m_fFuelConsumption = 0;
	m_nNOXConcentration = 0;
	m_fUreaInjectionVolume = 0;
	m_fEGT = 0;
	m_fDPFDifferentialPressure = 0;
	m_fEGRPosition = 0;
	m_nFuelDeliveryPressure = 0;
	m_nFuelType = 1;

	m_bComPort = 255;
	m_nComBaudRate = CBR_115200;
	m_strServerIP = L"127.0.0.1";
	m_usServerPort = 10055;

	m_LastOPTime = clock();
	m_nGetRTDataTimeInterval = 1000;
}

COBDAdapterThread::~COBDAdapterThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL COBDAdapterThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int COBDAdapterThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(COBDAdapterThread, CWinThread)
	ON_THREAD_MESSAGE(WM_OBD_INIT, OnInit)
	ON_THREAD_MESSAGE(WM_OBD_GET_SYSTEMCHECK, OnGetSystemCheck)
	ON_THREAD_MESSAGE(WM_OBD_GET_RT_DATA, OnGetRTData)
END_MESSAGE_MAP()


// COBDAdapterThread 消息处理程序

BOOL COBDAdapterThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_bQuitThread)
	{
		if(WM_QUIT == pMsg->message)
		{
			return CWinThread::PreTranslateMessage(pMsg);
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return CWinThread::PreTranslateMessage(pMsg);
	}
	//return CWinThread::PreTranslateMessage(pMsg);
}

void COBDAdapterThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

void COBDAdapterThread::InitializeInstrument(const wchar_t *szProtocol)
{
	UninitializeInstrument();

	if (_wcsicmp(szProtocol, L"ZENYUAN") == 0)
	{
		m_pAdapter = new CZenYuanOBD();
		m_emInstrumentModel = IM_ZENYUAN;
	}
	else if (_wcsicmp(szProtocol, L"ZHENGDE") == 0)
	{
		m_pAdapter = new CZhengDeOBD();
		m_emInstrumentModel = IM_ZHENGDE;
	}
	else if (_wcsicmp(szProtocol, L"NHWS") == 0)
	{
		m_pAdapter = new CNanHuaWSOBD();
		m_emInstrumentModel = IM_NHWS;
	}
	else
	{
		// 无匹配
		m_pAdapter = new CZenYuanOBD();
		m_emInstrumentModel = IM_ZENYUAN;
	}
}

COBDAdapterThread::InstrumentModel COBDAdapterThread::GetInstrumentModel(void) const
{
	return m_emInstrumentModel;
}

void COBDAdapterThread::UninitializeInstrument(void)
{
	if (NULL != m_pAdapter)
	{
		delete m_pAdapter;
		m_pAdapter = NULL;
	}
}

DWORD COBDAdapterThread::Open(const BYTE bPort, const int nBaudRate, const wchar_t *szProtocol)
{
	m_bComPort = bPort;
	m_nComBaudRate = nBaudRate;

	InitializeInstrument(szProtocol);
	if (m_emInstrumentModel == IM_ZENYUAN)
	{
		return m_pAdapter->Open(bPort, nBaudRate);
	}
	return COM_OPEN_FAIL;
}

DWORD COBDAdapterThread::Open(std::wstring strIP, const int nPort, const wchar_t *szProtocol)
{
	m_strServerIP = strIP;
	m_usServerPort = nPort;

	InitializeInstrument(szProtocol);
	if (m_emInstrumentModel == IM_ZHENGDE
		|| m_emInstrumentModel == IM_NHWS
		)
	{
		return m_pAdapter->Open(strIP, nPort);
	}
	return COM_OPEN_FAIL;
}

DWORD COBDAdapterThread::ReOpen(void)
{
	if (m_emInstrumentModel == IM_ZENYUAN)
	{
		if (NULL != m_pAdapter)
		{
			delete m_pAdapter;
			m_pAdapter = NULL;
		}
		m_pAdapter = new CZenYuanOBD();
		return m_pAdapter->Open(m_bComPort, m_nComBaudRate);
	}
	else if (m_emInstrumentModel == IM_ZHENGDE)
	{
		//if (NULL != m_pAdapter)
		//{
		//	delete m_pAdapter;
		//	m_pAdapter = NULL;
		//}
		//m_pAdapter = new CZhengDeOBD();
		//return m_pAdapter->Open(m_strServerIP, m_usServerPort);

		if (IsOpen())
		{
			return COM_OPEN_SUCCEED;
		}
	}
	return COM_OPEN_FAIL;
}

DWORD COBDAdapterThread::Close(void)
{
	DWORD dw = 0x00;
	if (NULL != m_pAdapter)
	{
		dw = m_pAdapter->Close();
	}
	UninitializeInstrument();
	return dw;
}

bool COBDAdapterThread::IsOpen(void) const
{
	if (m_pAdapter == NULL)
	{
		return false;
	}

	return m_pAdapter->IsOpen();
}

// 初始化适配器
bool COBDAdapterThread::Init(DWORD &dwProtocol, const int nFuelType)
{
	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;
	m_nFuelType = nFuelType;

	return m_pAdapter->Init(dwProtocol, nFuelType);
}

void COBDAdapterThread::Init_Asyn(DWORD dwProtocol, const int nFuelType, HWND hCallBackWnd)
{
	m_nFuelType = nFuelType;
	PostThreadMessage(WM_OBD_INIT, dwProtocol, (LPARAM)hCallBackWnd);
}

void COBDAdapterThread::OnInit(WPARAM wParam, LPARAM lParam)
{
	DWORD dwProtocol = (DWORD)wParam;
	bool bRet = Init(dwProtocol, m_nFuelType);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OBD_INIT, (WPARAM)bRet, (LPARAM)dwProtocol);
	}
}

bool COBDAdapterThread::IsAdapterOnline(void)
{
	return m_pAdapter->IsAdapterOnline();
}

bool COBDAdapterThread::GetDTC(std::vector<SDTCData> &vtDTC)
{
	return m_pAdapter->GetDTC(vtDTC);
}

bool COBDAdapterThread::GetLampState(bool &bIsOn)
{
	return m_pAdapter->GetLampState(bIsOn);
}

bool COBDAdapterThread::GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem)
{
	return m_pAdapter->GetSystemCheck(sSystemCheckItem);
}

void COBDAdapterThread::GetSystemCheck_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_OBD_GET_SYSTEMCHECK, 0, (LPARAM)hCallBackWnd);
}

void COBDAdapterThread::OnGetSystemCheck(WPARAM wParam, LPARAM lParam)
{
	memset(&m_sSystemCheckItem, 0, sizeof(SOBDSystemCheckItem));
	bool bRet = GetSystemCheck(m_sSystemCheckItem);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OBD_GET_SYSTEMCHECK, (WPARAM)bRet, m_vtDTC.size());
	}
}

bool COBDAdapterThread::ClearDTC(void)
{
	return m_pAdapter->ClearDTC();
}

bool COBDAdapterThread::GetRTData(void)
{
	return m_pAdapter->GetRTData(m_nRev, m_fV, m_fTemp);
}

bool COBDAdapterThread::GetRTData(std::map<std::wstring, std::wstring> &mapData)
{
	return m_pAdapter->GetRTData(mapData);
}

void COBDAdapterThread::GetRTData_Asyn(HWND hCallBackWnd)
{
	if ((clock()-m_LastOPTime) > m_nGetRTDataTimeInterval)	// 强制在底层限制采样命令的发送间隔
	{
		m_LastOPTime = clock();
		//TRACE("异步访问请求\r\n");

		PostThreadMessage(WM_OBD_GET_RT_DATA, 0, (LPARAM)hCallBackWnd);
	}
}

void COBDAdapterThread::OnGetRTData(WPARAM wParam, LPARAM lParam)
{
	std::map<std::wstring, std::wstring> mapData;
	bool bRet = GetRTData(mapData);
	if (bRet)
	{
		m_fV = (float)_wtof(mapData[L"Velocity"].c_str());
		m_fTemp = (float)_wtof(mapData[L"CoolantTemp"].c_str());
		m_nRev = _wtoi(mapData[L"EngineRev"].c_str());
		m_fSolarTermDoorPosition = (float)_wtof(mapData[L"SolarTermDoorPosition"].c_str());
		m_fCalculationLoad = (float)_wtof(mapData[L"CalculationLoad"].c_str());
		m_fForwardSensorV = (float)_wtof(mapData[L"ForwardSensorV"].c_str());
		m_fForwardSensorI = (float)_wtof(mapData[L"ForwardSensorI"].c_str());
		m_fAirCoefficient = (float)_wtof(mapData[L"AirCoefficient"].c_str());
		m_fMAF = (float)_wtof(mapData[L"MAF"].c_str());
		m_fMAP = (float)_wtof(mapData[L"MAP"].c_str());
		m_fEngineOutputPower = (float)_wtof(mapData[L"EngineOutputPower"].c_str());
		m_fChargeAirPressure = (float)_wtof(mapData[L"ChargeAirPressure"].c_str());
		m_fFuelConsumption = (float)_wtof(mapData[L"FuelConsumption"].c_str());
		m_nNOXConcentration = _wtoi(mapData[L"NOXConcentration"].c_str());
		m_fUreaInjectionVolume = (float)_wtof(mapData[L"UreaInjectionVolume"].c_str());
		m_fEGT = (float)_wtof(mapData[L"EGT"].c_str());
		m_fDPFDifferentialPressure = (float)_wtof(mapData[L"DPFDifferentialPressure"].c_str());
		m_fEGRPosition = (float)_wtof(mapData[L"EGRPosition"].c_str());
		m_nFuelDeliveryPressure = _wtoi(mapData[L"FuelDeliveryPressure"].c_str());
	}
	else
	{
		m_fV = 0;
		m_fTemp = 0;
		m_nRev = 0;
		m_fSolarTermDoorPosition = 0;
		m_fCalculationLoad = 0;
		m_fAirCoefficient = 0;
		m_fForwardSensorV = 0;
		m_fForwardSensorI = 0;
		m_fMAF = 0;
		m_fMAP = 0;
		m_fEngineOutputPower = 0;
		m_fChargeAirPressure = 0;
		m_fFuelConsumption = 0;
		m_nNOXConcentration = 0;
		m_fUreaInjectionVolume = 0;
		m_fEGT = 0;
		m_fDPFDifferentialPressure = 0;
		m_fEGRPosition = 0;
		m_nFuelDeliveryPressure = 0;
	}
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OBD_GET_RT_DATA, (WPARAM)bRet, (LPARAM)0);
	}
}

bool COBDAdapterThread::GetDTCMileage(DWORD &dwMileage)
{
	return m_pAdapter->GetDTCMileage(dwMileage);
}

bool COBDAdapterThread::GetFreezeData(std::wstring &strData)
{
	return m_pAdapter->GetFreezeData(strData);
}

bool COBDAdapterThread::GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode/*=NULL*/)
{
	return m_pAdapter->GetVIN(strVIN, strVINHex, pnCode);
}

bool COBDAdapterThread::GetOBDType(std::wstring &strType)
{
	return m_pAdapter->GetOBDType(strType);
}

bool COBDAdapterThread::GetMileage(DWORD &dwMileage)
{
	return m_pAdapter->GetMileage(dwMileage);
}

bool COBDAdapterThread::GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	return m_pAdapter->GetECUInfo(mapData, pnCode);
}

bool COBDAdapterThread::GetSoftwareVer(std::wstring &strVer)
{
	return m_pAdapter->GetSoftwareVer(strVer);
}

bool COBDAdapterThread::GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	return m_pAdapter->GetIUPR(mapData, pnCode);
}

bool COBDAdapterThread::UnInit(void)
{
	return m_pAdapter->UnInit();
}

std::wstring COBDAdapterThread::GetOBDProtocolName(DWORD dwProtocol)
{
	return m_pAdapter->GetOBDProtocolName(dwProtocol);
}
