// OpaThread.cpp : 实现文件
//

#include "stdafx.h"
#include "OpaThread.h"


// COpaThread

IMPLEMENT_DYNCREATE(COpaThread, CWinThread)

COpaThread::COpaThread()
: m_bs16AlarmInfo(0x0000)
, m_fN(0.0f)
, m_fK(0.0f)
, m_usRpm(0)
, m_usOilTemp(0)
, m_fOriginalN(0.0f)
, m_fOriginalK(0.0f)
, m_usSmokeTemp(0)
, m_usTubeTemp(0)
, m_usDetectorTemp(0)
, m_usLEDTemp(0)
, m_usBoardTemp(0)
, m_usLeftFanCurrent(0)
, m_usRightFanCurrent(0)
, m_fPowerVoltage(0.0f)
, m_usRealTimeIntensity(0)
, m_usFullShadingIntensity(0)
, m_fMaxN(0.0f)
, m_fMaxK(0.0f)
, m_usMaxRpm(0)
, m_usMaxOilTemp(0)
, m_bStatus(0)
, m_usMin(0)
, m_usSec(0)
, m_fET(0.0f)
, m_fRH(0.0f)
, m_fAP(0.0f)
, m_sNO(0)
, m_sNO2(0)
, m_fCO2(0)
, m_sHC(0)
, m_fCO(0)
, m_fO2(0)
, m_fVersion(0.0f)
, m_est(COpa::FourStroke)
, m_rmt(COpa::Piezoelectric)
, m_fpt(COpa::FilterMode1)
, m_fCorrectedOilTemp(0.0f)
, m_bQuitThread(false)
, m_pOpa(NULL)
, m_dwStatus(0x00)
{
	// 对象创建时,初始化设备为:NHT1L
	InitializeInstrument(IM_NANHUA_NHT1L);
	m_LastOPTime = clock();
}

COpaThread::~COpaThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL COpaThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int COpaThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(COpaThread, CWinThread)
	ON_THREAD_MESSAGE(WM_OPA_CALIBRATE, OnCalibrate)
	ON_THREAD_MESSAGE(WM_OPA_GET_REALTIME_DATA, OnGetRealTimeData)
	ON_THREAD_MESSAGE(WM_OPA_GET_ENV_REALTIME_DATA, OnGetEnvRealTimeData)
	ON_THREAD_MESSAGE(WM_OPA_GET_MAX_VALUE, OnGetMaxValue)
END_MESSAGE_MAP()


// COpaThread 消息处理程序

BOOL COpaThread::PreTranslateMessage(MSG* pMsg)
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

void COpaThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

HANDLE COpaThread::GetComHandle(void)
{
	return m_pOpa->GetComHandle();
}

void COpaThread::AttachComHandle(HANDLE hCom)
{
	m_pOpa->AttachComHandle(hCom);
}

void COpaThread::DetachComHandle(void)
{
	m_pOpa->DetachComHandle();
}

CRITICAL_SECTION COpaThread::GetCriticalSection(void)
{
	return m_pOpa->GetCriticalSection();
}

void COpaThread::AttachCriticalSection(CRITICAL_SECTION cs)
{
	m_pOpa->AttachCriticalSection(cs);
}

void COpaThread::DetachCriticalSection(void)
{
	m_pOpa->DetachCriticalSection();
}

DWORD COpaThread::Open(const BYTE bPort, const enum InstrumentModel im)
{
	InitializeInstrument(im);
	return m_pOpa->Open(bPort);
}

DWORD COpaThread::Close(void)
{
	return m_pOpa->Close();
}

DWORD COpaThread::StopWarmUp(void)
{
	return m_pOpa->StopWarmUp();
}

DWORD COpaThread::Calibrate(void)
{
	return m_pOpa->Calibrate();
}

DWORD COpaThread::ClearMaxValue(void)
{
	return m_pOpa->ClearMaxValue();
}

DWORD COpaThread::SetControlUnitParams(const COpa::ControlUnitParams *const psControlUnitParams)
{
	return m_pOpa->SetControlUnitParams(psControlUnitParams);
}

DWORD COpaThread::SetRH(const float fStdValue)
{
	return m_pOpa->SetRH(fStdValue);
}

DWORD COpaThread::ResetEEPROM(void)
{
	return m_pOpa->ResetEEPROM();
}

DWORD COpaThread::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	return m_pOpa->GetAlarmInfo(pbs16AlarmInfo);
}

DWORD COpaThread::GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp)
{
	return m_pOpa->GetRealTimeData(pfN, pfK, pusRpm, pusOilTemp);
}

DWORD COpaThread::GetMeasureUnitData(float *const pfOriginalN, 
									   float *const pfOriginalK, 
									   USHORT *const pusSmokeTemp, 
									   USHORT *const pusTubeTemp, 
									   USHORT *const pusDetectorTemp, 
									   USHORT *const pusLEDTemp, 
									   USHORT *const pusBoardTemp,
									   USHORT *const pusLeftFanCurrent, 
									   USHORT *const pusRightFanCurrent, 
									   float *const pfPowerVoltage, 
									   USHORT *const pusRealTimeIntensity, 
									   USHORT *const pusFullShadingIntensity)
{
	return m_pOpa->GetMeasureUnitData(pfOriginalN,
		pfOriginalK,
		pusSmokeTemp,
		pusTubeTemp,
		pusDetectorTemp,
		pusLEDTemp,
		pusBoardTemp,
		pusLeftFanCurrent,
		pusRightFanCurrent,
		pfPowerVoltage,
		pusRealTimeIntensity,
		pusFullShadingIntensity);
}

DWORD COpaThread::GetMaxValue(float *const pfMaxN, float *const pfMaxK, USHORT *const pusMaxRpm, USHORT *const pusMaxOilTemp)
{
	return m_pOpa->GetMaxValue(pfMaxN, pfMaxK, pusMaxRpm, pusMaxOilTemp);
}

DWORD COpaThread::GetStatus(BYTE *const pbStatus)
{
	return m_pOpa->GetStatus(pbStatus);
}

DWORD COpaThread::SetStatus(const BYTE bStatus)
{
	return m_pOpa->SetStatus(bStatus);
}

DWORD COpaThread::GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec)
{
	return m_pOpa->GetWarmUpTime(pusMin, pusSec);
}

DWORD COpaThread::GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP)
{
	return m_pOpa->GetEnvParams(pfET, pfRH, pfAP);
}

DWORD COpaThread::GetVersion(float *const pfVersion)
{
	return m_pOpa->GetVersion(pfVersion);
}

DWORD COpaThread::GetControlUnitParams(enum COpa::EngineStrokeType *const pest, enum COpa::RpmMeasurementType *const purmt, enum COpa::FilterParamType *const pfpt)
{
	return m_pOpa->GetControlUnitParams(pest, purmt, pfpt);
}

DWORD COpaThread::GetCorrectedOilTemp(float *const pfCorrectedOilTemp)
{
	return m_pOpa->GetCorrectedOilTemp(pfCorrectedOilTemp);
}

DWORD COpaThread::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return m_pOpa->GetTestResultData(pfValueK1, pfValueK2, pfValueK3, psLowRpm, pfValueKAvg);
}

void COpaThread::Calibrate_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_OPA_CALIBRATE, 0, (LPARAM)hCallBackWnd);
}

void COpaThread::OnCalibrate(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = m_pOpa->Calibrate();
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OPA_CALIBRATE, (WPARAM)dw, 0);
	}
}

void COpaThread::GetRealTimeData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_OPA_GET_REALTIME_DATA, 0, (LPARAM)hCallBackWnd);
}

void COpaThread::GetEnvRealTimeData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_OPA_GET_ENV_REALTIME_DATA, 0, (LPARAM)hCallBackWnd);
}

void COpaThread::OnGetRealTimeData(WPARAM wParam, LPARAM lParam)
{
	DWORD dw;
	if (GetInstrumentModel() != IM_NANHUA_NHT610)
	{
		dw = m_pOpa->GetRealTimeData(&m_fN,&m_fK,&m_usRpm,&m_usOilTemp);
	}
	else
	{
		if (clock()-m_LastOPTime > 500)	// 强制在底层限制采样命令的发送间隔
		{
			dw = m_pOpa->GetRealTimeData(&m_fN,&m_fK,&m_usRpm,&m_usOilTemp,
									&m_sNO, &m_sNO2, &m_fCO2, 
									&m_sHC, &m_fCO, &m_fO2);
			m_LastOPTime = clock();
		}
	}
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OPA_GET_REALTIME_DATA, (WPARAM)dw, 0);
	}
}

void COpaThread::OnGetEnvRealTimeData(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = m_pOpa->GetEnvParams(&m_fET, &m_fRH, &m_fAP);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OPA_GET_ENV_REALTIME_DATA, (WPARAM)dw, 0);
	}
}

void COpaThread::GetMaxValue_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_OPA_GET_MAX_VALUE, 0, (LPARAM)hCallBackWnd);
}

void COpaThread::OnGetMaxValue(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = m_pOpa->GetMaxValue(&m_fMaxN, &m_fMaxK, &m_usMaxRpm, &m_usMaxOilTemp);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_OPA_GET_MAX_VALUE, (WPARAM)dw, 0);
	}
}

void COpaThread::InitializeInstrument(const enum InstrumentModel im)
{
	UninitializeInstrument();
	
	m_emInstrumentModel = im;
	if(IM_NANHUA_NHT1L == im)
	{
		m_pOpa = new CNanhuaNht1L();
	}
	else if(IM_NANHUA_NHT6 == im)
	{
		m_pOpa = new CNanhuaNht6();
	}
	else if(IM_NANHUA_NHT610 == im)
	{
		m_pOpa = new CNanhuaNHT610();
	}
	else if(IM_FOFEN_FTY100 == im)
	{
		m_pOpa = new CFoFenFTY100();
	}
	else if(IM_FULI_FLB100 == im)
	{
		m_pOpa = new CFuLiFLB100();
	}
	else if(IM_MINGQUAN_MQY200 == im)
	{
		m_pOpa = new CMingQuanMQY200();
	}
	else
	{
		// 无匹配
		m_pOpa = new CNanhuaNht1L();
	}
}

void COpaThread::UninitializeInstrument(void)
{
	if (NULL != m_pOpa)
	{
		delete m_pOpa;
		m_pOpa = NULL;
	}
}