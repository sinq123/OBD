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
	else if (IM_XIN_CHENG == im)
	{
		m_pDyn = new CXinChengDyn;
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

// 设置IGBT的pwm波占空比(%)
// 设置pwm占空比为50%：
// XSI +0x00+0x20+0x7a+0x44+ 和校验码＋C
DWORD CDynThread::XC_SetConstantIGBT(const float fpwm)
{
	return m_pDyn->XC_SetConstantIGBT(fpwm);
}

// 设置选用的力通道（通道1，通道2，通道3）
// 默认值为通道1：如设置为通道1/2/3，该条命令直接用第一个数据字节来表示选用的通道。
// XSI +0+0+0+1/2/3+ 和校验码＋C
DWORD CDynThread::XC_SetConstantForceChannel(const int nChannel)
{
	return m_pDyn->XC_SetConstantForceChannel(nChannel);
}

// 力清零
//对目前设置使用的通道进行清零：
// XSZ+0+0+0+0+ 和校验码＋C
DWORD CDynThread::XC_SetConstantForceClearing(void)
{
	return m_pDyn->XC_SetConstantForceClearing();
}

// 清除按键值
DWORD CDynThread::XC_ClearKeyValue(void)
{
	return m_pDyn->XC_ClearKeyValue();
}

// 设置滚筒直径(cm)：
// 如滚筒直径为14.736cm：
//XCS +0xa8+0xc6+0x6b+0x41+ 和校验码＋C
DWORD CDynThread::XC_SetConstantRollerDiameter(const int nRollerDim)
{
	return m_pDyn->XC_SetConstantRollerDiameter(nRollerDim);
}

// 设置编码器脉冲数(每圈)
// 如编码器脉冲数为1000：
//XCS +0x00+0x00+0x7a+0x44+ 和校验码＋C
DWORD CDynThread::XC_SetConstantEncoderPulNum(const int nEnPulNum)
{
	return m_pDyn->XC_SetConstantEncoderPulNum(nEnPulNum);
}

// 设置通道1力标定系数b[0]：force=b[0]*(ad_value-c[0])
// 设置系数b[0]=1158.4：
// XCA +0xcd+0xcc+0x90+0x44+ 和校验码＋C
DWORD CDynThread::XC_SetConstantForceCailFac_B1(const float fb)
{
	return m_pDyn->XC_SetConstantForceCailFac_B1(fb);
}

//设置通道1力标定系数c[0]：	
// 设置系数c[0]=0.01：
//XCB +0x0a+0xd7+0x23+0x3c+ 和校验码＋C
DWORD CDynThread::XC_SetConstantForceCailFac_C1(const float fc)
{
	return m_pDyn->XC_SetConstantForceCailFac_C1(fc);
}

//设置通道2力标定系数b[1]：	
//设置系数b[1]=1158.4：
//XCC +0xcd+0xcc+0x90+0x44+ 和校验码＋C
DWORD CDynThread::XC_SetConstantForceCailFac_B2(const float fb)
{
	return m_pDyn->XC_SetConstantForceCailFac_B2(fb);
}

//设置通道2力标定系数c[1]：	
//设置系数c[1]=0.01：
//XCD +0x0a+0xd7+0x23+0x3c + 和校验码＋C
DWORD CDynThread::XC_SetConstantForceCailFac_C2(const float fc)
{
	return m_pDyn->XC_SetConstantForceCailFac_B2(fc);
}

//设置通道3力标定系数b[2]：	
//设置系数b[2]=1158.4：
//XCE +0xcd+0xcc+0x90+0x44 + 和校验码＋C
DWORD CDynThread::XC_SetConstantForceCailFac_B3(const float fb)
{
	return m_pDyn->XC_SetConstantForceCailFac_B2(fb);
}

//设置通道3力标定系数c[2]：	
//设置系数c[2]=0.01：
//XCF +0x0a+0xd7+0x23+0x3c  +和校验码＋C
DWORD CDynThread::XC_SetConstantForceCailFac_C3(const float fc)
{
	return m_pDyn->XC_SetConstantForceCailFac_C3(fc);
}

//设置恒速控制PID控制比例系数kp：	
//设置系数c[2]=0.01：
//XCH +0x0a+0xd7+0x23+0x3c  +和校验码＋C
DWORD CDynThread::XC_SetConstantSpeedControl_P(const float fkp)
{
	return m_pDyn->XC_SetConstantSpeedControl_P(fkp);
}

//设置恒速控制PID控制积分系数ki：	
//设置系数c[2]=0.01：
//XCI +0x0a+0xd7+0x23+0x3c +和校验码＋C
DWORD CDynThread::XC_SetConstantSpeedControl_I(const float fki)
{
	return m_pDyn->XC_SetConstantSpeedControl_I(fki);
}

//设置恒速控制PID控制微分系数kd：	
//设置系数c[2]=0.01：
//XCJ+0x0a+0xd7+0x23+0x3c +和校验码＋C
DWORD CDynThread::XC_SetConstantSpeedControl_D(const float fkd)
{
	return m_pDyn->XC_SetConstantSpeedControl_D(fkd);
}

//设置恒力控制PID控制比例系数kp_force：	
//设置系数c[2]=0.01：
//XCK +0x0a+0xd7+0x23+0x3c  +和校验码＋C
DWORD CDynThread::XC_SetConstantForceControl_P(const float fkp)
{
	return m_pDyn->XC_SetConstantForceControl_P(fkp);
}

//设置恒力控制PID控制比例系数 ki_force：	
//设置系数c[2]=0.01：
//XCL +0x0a+0xd7+0x23+0x3c +和校验码＋C
DWORD CDynThread::XC_SetConstantForceControl_I(const float fki)
{
	return m_pDyn->XC_SetConstantForceControl_I(fki);
}

//设置恒力控制PID控制比例系数 kd_force：	
//设置系数c[2]=0.01：
//XCM +0x0a+0xd7+0x23+0x3c +和校验码＋C
DWORD CDynThread::XC_SetConstantForceControl_D(const float fkd)
{
	return m_pDyn->XC_SetConstantForceControl_D(fkd);
}

//启动恒速度控制	XBS + 0+0+0+0+ 和校验码 ＋ C
//启动恒扭距控制	XBF + 0+0+0+0+ 和校验码 ＋ C
//启动恒功率控制	XBP + 0+0+ 0+0+ 和校验码 ＋ C

//启动输出设定pwm值	XBI + 0+0+ 0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_StartSetPWN(const float fPWN)
{
	return m_pDyn->XC_StartSetPWN(fPWN);
}

//停止控制,pwm输出0	XBS + 0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_StopPWN(void)
{
	return m_pDyn->XC_StopPWN();
}

//起动电机	XEB + 0+0+0+0+ 和校验码 ＋ C
//停止电机	XES + 0+0+0+0+ 和校验码 ＋ C
//打开指定继电器（LH1/LH2/LH3）	（LH1/LH2/LH3）分别对应数据1/2/3：如打开继电器LH1: XEO + 0+0+0+1+ 和校验码 ＋ C
DWORD CDynThread::XC_OpenRelay(const int nLH)
{
	return m_pDyn->XC_OpenRelay(nLH);
}

//关闭指定继电器（LH1/LH2/LH3）	（LH1/LH2/LH3）分别对应数据1/2/3：如打开继电器LH3: XEF + 0+0+0+3+ 和校验码 ＋ C
DWORD CDynThread::XC_CloseRelay(const int nLH)
{
	return m_pDyn->XC_CloseRelay(nLH);
}

//举升器上升	XJU+ 0+0+0+0+ 和校验码 ＋ C
//举升器下降	XJD +0+0+0+0+ 和校验码 ＋ C
//进入标定状态	XDF +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_EnterCaliSta(void)
{
	return m_pDyn->XC_EnterCaliSta();
}

//退出标定状态	 XDS +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_ExitCaliSta(void)
{
	return m_pDyn->XC_ExitCaliSta();
}

//读取环境参数	 XRT +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_ReadEnvPar(float *const pfET/*=NULL*/, float *const pfAP/*=NULL*/, float *const pfRH/*=NULL*/)
{
	return m_pDyn->XC_ReadEnvPar(pfET, pfAP, pfRH);
}

//读取力标定系数b,c	XRF +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_ReadForCalFac(float *const pfb/*=NULL*/, float *const pfc/*=NULL*/, float *const pfb1/*=NULL*/, float *const pfc1/*=NULL*/, float *const pfb2/*=NULL*/, float *const pfc2/*=NULL*/)
{
	return m_pDyn->XC_ReadForCalFac(pfb, pfc, pfb1, pfc1, pfb2, pfc2);
}

//读取力通道	XRC +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_ReadForCha(int *const pnCha/*=NULL*/)
{
	return m_pDyn->XC_ReadForCha(pnCha);
}

//读取恒速PID系数	XRs +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_ReadConSpeed(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	return m_pDyn->XC_ReadConSpeed(pfP, pfI, pfD);
}

//读取恒力PID系数	XRf +0+0+0+0+ 和校验码 ＋ C	
DWORD CDynThread::XC_ReadConForce(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	return m_pDyn->XC_ReadConForce(pfP, pfI, pfD);
}

//读取滚筒直径和编码器脉冲	 XRS +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_ReadRollerAndEncoder(int *const pnDia/*=NULL*/, int *const pnPul/*=NULL*/)
{
	return m_pDyn->XC_ReadRollerAndEncoder(pnDia, pnPul);
}

//将3个通道的标定系数固化到ROM中	 XMA +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_SolCalFac(void)
{
	return m_pDyn->XC_SolCalFac();
}

//将滚筒直径和编码器脉冲固化到ROM中	XMS +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_SolRollerAndEncoder(void)
{
	return m_pDyn->XC_SolRollerAndEncoder();
}

//将力通道的选择固化到ROM中	XMC +0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_SolForCha(void)
{
	return m_pDyn->XC_SolForCha();
}

//将恒速控制的PID系数固化到ROM中	XMD + 0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_SolConSheepPID(void)
{
	return m_pDyn->XC_SolConSheepPID();
}

//将恒力控制的PID系数固化到ROM中	XME + 0+0+0+0+ 和校验码 ＋ C
DWORD CDynThread::XC_SolConForcePID(void)
{
	return m_pDyn->XC_SolConForcePID();
}