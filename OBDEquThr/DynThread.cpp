// DynThread.cpp : 实现文件
//

#include "stdafx.h"
#include "DynThread.h"


// CDynThread

IMPLEMENT_DYNCREATE(CDynThread, CWinThread)

CDynThread::CDynThread()
: m_bQuitThread(false)
, m_fP(0.0f)
, m_fF(0.0f)
, m_fV(0.0f)
, m_dwWRSta(DYN_WR_SUCCEED)
, m_btStatus(0x0000)
, m_pDyn(NULL)
, m_dwStatus(0x00)
{
	ZeroMemory(m_wVarValues, sizeof(m_wVarValues));
	ZeroMemory(m_wVarValuesForWrite, sizeof(m_wVarValuesForWrite));

	// 对象创建时,初始化设备为:NHC03
	InitializeInstrument(IM_NANHUA_NHC03);
}

CDynThread::~CDynThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL CDynThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CDynThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDynThread, CWinThread)
	ON_THREAD_MESSAGE(WM_DYN_GET_REALTIME_DATA, OnGetRealTimeData)
	ON_THREAD_MESSAGE(WM_DYN_GET_STATUS, OnGetStatus)
	ON_THREAD_MESSAGE(WM_DYN_READ_VAR, OnReadVar)
	ON_THREAD_MESSAGE(WM_DYN_WRITE_VAR, OnWriteVar)
	ON_THREAD_MESSAGE(WM_DYN_SEND_CONTROL_WORD, OnSendCtrlWord)
END_MESSAGE_MAP()


// CDynThread 消息处理程序

BOOL CDynThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_bQuitThread)
	{
		if (WM_QUIT == pMsg->message)
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
}

void CDynThread::QuitThread()
{
	m_bQuitThread = true;
	PostThreadMessage(WM_QUIT, 0, 0);
}

void CDynThread::InitializeInstrument(const enum InstrumentModel im)
{
	UninitializeInstrument();

	if (IM_NANHUA_NHC03 == im)
	{
		m_pDyn = new CNanhuaNhc03;
	}
	else if (IM_MAHA_ASMP == im)
	{
		m_pDyn = new CMahaASMP;
	}
	else
	{
		// 无匹配
		m_pDyn = new CNanhuaNhc03;
	}
}

void CDynThread::UninitializeInstrument(void)
{
	if (NULL != m_pDyn)
	{
		delete m_pDyn;
		m_pDyn = NULL;
	}
}

DWORD CDynThread::Open(const BYTE bPort, const enum InstrumentModel im)
{
	InitializeInstrument(im);

	return m_pDyn->Open(bPort);
}

DWORD CDynThread::Close(void)
{
	return m_pDyn->Close();
}

//bool CDynThread::IsOpen(void) const
//{
//	return m_pDyn->IsOpen();
//}

DWORD CDynThread::GetRealTimeData(float *const pfP, float *const pfF, float *const pfV)
{
	m_dwStatus = m_pDyn->GetRealTimeData(pfP, pfF, pfV);
	return m_dwStatus;
}

DWORD CDynThread::GetStatus(std::bitset<16> *const pbs16Status)
{
	return m_pDyn->GetStatus(pbs16Status);
}

DWORD CDynThread::ReadVar(const BYTE bVarNumber, WORD *const pwVarValue)
{
	return m_pDyn->ReadVar(bVarNumber, pwVarValue);
}

DWORD CDynThread::WriteVar(const BYTE bVarNumber, const WORD wVarValue)
{
	return m_pDyn->WriteVar(bVarNumber, wVarValue);
}

DWORD CDynThread::SendCtrlWord(const BYTE bCtrlWord)
{
	return m_pDyn->SendCtrlWord(bCtrlWord);
}

DWORD CDynThread::LiftBeamUp(void)
{
	return m_pDyn->LiftBeamUp();
}

DWORD CDynThread::LiftBeamDown(void)
{
	return  m_pDyn->LiftBeamDown();
}

DWORD CDynThread::MotorOn(void)
{
	return  m_pDyn->MotorOn();
}

DWORD CDynThread::MotorOff(void)
{
	return  m_pDyn->MotorOff();
}

DWORD CDynThread::ControlModeOff(void)
{
	return  m_pDyn->ControlModeOff();
}

DWORD CDynThread::AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses)
{
	return  m_pDyn->AsmRegulatorOn(fTargetPower, fAsmSpeed, fTireLosses);
}

DWORD CDynThread::SetConstantPowerTarget(const float fPTarget)
{
	return  m_pDyn->SetConstantPowerTarget(fPTarget);
}

DWORD CDynThread::ConstantPowerControlModeOn(void)
{
	return  m_pDyn->ConstantPowerControlModeOn();
}

DWORD CDynThread::VarLoadControlModeOn(void)
{
	return  m_pDyn->VarLoadControlModeOn();
}

DWORD CDynThread::SetConstantForceTarget(const float fFTarget)
{
	return  m_pDyn->SetConstantForceTarget(fFTarget);
}

DWORD CDynThread::ConstantForceControlModeOn(void)
{
	return  m_pDyn->ConstantForceControlModeOn();
}

DWORD CDynThread::SetConstantVelocityTarget(const float fVTarget)
{
	return  m_pDyn->SetConstantVelocityTarget(fVTarget);
}

DWORD CDynThread::ConstantVelocityControlModeOn(void)
{
	return  m_pDyn->ConstantVelocityControlModeOn();
}

DWORD CDynThread::SetUpperVelocityOfTimer1(const float fUpperVelocity)
{
	return  m_pDyn->SetUpperVelocityOfTimer1(fUpperVelocity);
}

DWORD CDynThread::SetMiddleVelocityOfTimer1(const float fMiddleVelocity)
{
	return  m_pDyn->SetMiddleVelocityOfTimer1(fMiddleVelocity);
}

DWORD CDynThread::SetLowerVelocityOfTimer1(const float fLowerVelocity)
{
	return  m_pDyn->SetLowerVelocityOfTimer1(fLowerVelocity);
}

DWORD CDynThread::SetTimer1(void)
{
	return  m_pDyn->SetTimer1();
}

DWORD CDynThread::KillTimer1(void)
{
	return  m_pDyn->KillTimer1();
}

DWORD CDynThread::GetTimeOfTimer1(UINT *const punTime_MS)
{
	return  m_pDyn->GetTimeOfTimer1(punTime_MS);
}

DWORD CDynThread::GetMiddleTimeOfTimer1(UINT *const punTime_MS)
{
	return  m_pDyn->GetMiddleTimeOfTimer1(punTime_MS);
}

DWORD CDynThread::SetUpperVelocityOfTimer2(const float fUpperVelocity)
{
	return  m_pDyn->SetUpperVelocityOfTimer2(fUpperVelocity);
}

DWORD CDynThread::SetLowerVelocityOfTimer2(const float fLowerVelocity)
{
	return  m_pDyn->SetLowerVelocityOfTimer2(fLowerVelocity);
}

DWORD CDynThread::SetMotorMaxVelocity(const float fMaxVelocity)
{
	return m_pDyn->SetMotorMaxVelocity(fMaxVelocity);
}

DWORD CDynThread::GetMotorMaxVelocity(float *const fMaxVelocity)
{
	return m_pDyn->GetMotorMaxVelocity(fMaxVelocity);
}

DWORD CDynThread::SetTimer2(void)
{
	return  m_pDyn->SetTimer2();
}

DWORD CDynThread::KillTimer2(void)
{
	return  m_pDyn->KillTimer2();
}

DWORD CDynThread::GetTimeOfTimer2(UINT *const punTime_MS)
{
	return  m_pDyn->GetTimeOfTimer2(punTime_MS);
}

DWORD CDynThread::GetDIW(USHORT *const pusDIW)
{
	return  m_pDyn->GetDIW(pusDIW);
}

DWORD CDynThread::SetDIW(const USHORT usDIW)
{
	return  m_pDyn->SetDIW(usDIW);
}

DWORD CDynThread::WriteVehicleRM(const USHORT usVehicleRM)
{
	return  m_pDyn->WriteVehicleRM(usVehicleRM);
}

DWORD CDynThread::WriteCv(const float fCv)
{
	return  m_pDyn->WriteCv(fCv);
}

DWORD CDynThread::WriteBv(const float fBv)
{
	return  m_pDyn->WriteBv(fBv);
}

DWORD CDynThread::WriteAv(const float fAv)
{
	return  m_pDyn->WriteAv(fAv);
}

DWORD CDynThread::WriteAt(const float fAt)
{
	return  m_pDyn->WriteAt(fAt);
}

DWORD CDynThread::WriteBt(const float fBt)
{
	return  m_pDyn->WriteBt(fBt);
}

DWORD CDynThread::WriteCt(const float fCt)
{
	return  m_pDyn->WriteCt(fCt);
}

DWORD CDynThread::DriveResistanceSimulationControlModeOn(void)
{
	return  m_pDyn->DriveResistanceSimulationControlModeOn();
}


DWORD CDynThread::ResetDistance(void)
{
	return  m_pDyn->ResetDistance();
}

DWORD CDynThread::ReadDistance(USHORT *const pusDistance)
{
	return  m_pDyn->ReadDistance(pusDistance);
}



DWORD CDynThread::ReadApWithRemanence(float *const pfAp)
{
	return  m_pDyn->ReadApWithRemanence(pfAp);
}

DWORD CDynThread::ReadBpWithRemanence(float *const pfBp)
{
	return  m_pDyn->ReadBpWithRemanence(pfBp);
}

DWORD CDynThread::ReadCpWithRemanence(float *const pfCp)
{
	return  m_pDyn->ReadCpWithRemanence(pfCp);
}

DWORD CDynThread::WriteApWithRemanence(const float fAp)
{
	return  m_pDyn->WriteApWithRemanence(fAp);
}

DWORD CDynThread::WriteBpWithRemanence(const float fBp)
{
	return  m_pDyn->WriteBpWithRemanence(fBp);
}

DWORD CDynThread::WriteCpWithRemanence(const float fCp)
{
	return  m_pDyn->WriteCpWithRemanence(fCp);
}

DWORD CDynThread::ReadApWithoutRemanence(float *const pfAp)
{
	return  m_pDyn->ReadApWithoutRemanence(pfAp);
}

DWORD CDynThread::ReadBpWithoutRemanence(float *const pfBp)
{
	return  m_pDyn->ReadBpWithoutRemanence(pfBp);
}

DWORD CDynThread::ReadCpWithoutRemanence(float *const pfCp)
{
	return  m_pDyn->ReadCpWithoutRemanence(pfCp);
}

DWORD CDynThread::WriteApWithoutRemanence(const float fAp)
{
	return  m_pDyn->WriteApWithoutRemanence(fAp);
}

DWORD CDynThread::WriteBpWithoutRemanence(const float fBp)
{
	return  m_pDyn->WriteBpWithoutRemanence(fBp);
}

DWORD CDynThread::WriteCpWithoutRemanence(const float fCp)
{
	return  m_pDyn->WriteCpWithoutRemanence(fCp);
}


void CDynThread::GetRealTimeData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_DYN_GET_REALTIME_DATA, 0, (LPARAM)hCallBackWnd);
}

void CDynThread::OnGetRealTimeData(WPARAM wParam, LPARAM lParam)
{
	m_dwWRSta = m_pDyn->GetRealTimeData(&m_fP, &m_fF, &m_fV);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_DYN_GET_REALTIME_DATA, (WPARAM)m_dwWRSta, 0);
	}
}

void CDynThread::GetStatus_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_DYN_GET_STATUS, 0, (LPARAM)hCallBackWnd);
}

void CDynThread::OnGetStatus(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = m_pDyn->GetStatus(&m_btStatus);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_DYN_GET_STATUS, (WPARAM)dw, 0);
	}
}

void CDynThread::ReadVar_Asyn(const BYTE bVarNumber, HWND hCallBackWnd)
{
	PostThreadMessage(WM_DYN_READ_VAR, WPARAM(bVarNumber), (LPARAM)hCallBackWnd);
}

void CDynThread::OnReadVar(WPARAM wParam, LPARAM lParam)
{
	BYTE bVarNumber = (BYTE)wParam;
	DWORD dw = m_pDyn->ReadVar(bVarNumber, &m_wVarValues[bVarNumber]);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_DYN_READ_VAR, (WPARAM)dw, (LPARAM)bVarNumber);
	}
}

void CDynThread::WriteVar_Asyn(const BYTE bVarNumber, const WORD wVarValue, HWND hCallBackWnd)
{
	m_wVarValuesForWrite[bVarNumber] = wVarValue;
	PostThreadMessage(WM_DYN_WRITE_VAR, WPARAM(bVarNumber), (LPARAM)hCallBackWnd);
}

void CDynThread::OnWriteVar(WPARAM wParam, LPARAM lParam)
{
	BYTE bVarNumber = (BYTE)wParam;
	DWORD dw = m_pDyn->WriteVar(bVarNumber, m_wVarValuesForWrite[bVarNumber]);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_DYN_WRITE_VAR, (WPARAM)dw, 0);
	}
}

void CDynThread::SendCtrlWord_Asyn(const BYTE bCtrlWord, HWND hCallBackWnd)
{
	PostThreadMessage(WM_DYN_SEND_CONTROL_WORD, WPARAM(bCtrlWord), (LPARAM)hCallBackWnd);
}

void CDynThread::OnSendCtrlWord(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = m_pDyn->SendCtrlWord((BYTE)wParam);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_DYN_SEND_CONTROL_WORD, (WPARAM)dw, 0);
	}
}

DWORD CDynThread::SetConstantAccStep(const float fStep)
{
	return  m_pDyn->SetConstantAccStep(fStep);
}

DWORD CDynThread::ConstantAccControlModeOn(void)
{
	return  m_pDyn->ConstantAccControlModeOn();
}

DWORD CDynThread::ReadDynMaxPower(float *const pfDynMaxPower)
{
	DWORD dw = m_pDyn->ReadVar(68, &m_wVarValues[68]);
	if (DYN_WR_SUCCEED == dw)
	{
		*pfDynMaxPower = (float)m_wVarValues[68]/100;
	}
	else
	{
		*pfDynMaxPower = 0.0f;
	}
	return dw;
}
DWORD CDynThread::ResponseTimeControlModeStart(void)
{
	return  m_pDyn->ResponseTimeControlModeStart();
}

DWORD CDynThread::ResponseTimeControlModeEnd(void)
{
	return  m_pDyn->ResponseTimeControlModeEnd();
}
DWORD CDynThread::GetResponseTimeData( float *const pfV, float *const pfF, const int nLength)
{
	return m_pDyn->GetResponseTimeData(pfV, pfF,nLength);
}