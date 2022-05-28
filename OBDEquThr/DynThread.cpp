// DynThread.cpp : ʵ���ļ�
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

	// ���󴴽�ʱ,��ʼ���豸Ϊ:NHC03
	InitializeInstrument(IM_NANHUA_NHC03);
}

CDynThread::~CDynThread()
{
	// ��������ʱ,ж���豸
	UninitializeInstrument();
}

BOOL CDynThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CDynThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDynThread, CWinThread)
	ON_THREAD_MESSAGE(WM_DYN_GET_REALTIME_DATA, OnGetRealTimeData)
	ON_THREAD_MESSAGE(WM_DYN_GET_STATUS, OnGetStatus)
	ON_THREAD_MESSAGE(WM_DYN_READ_VAR, OnReadVar)
	ON_THREAD_MESSAGE(WM_DYN_WRITE_VAR, OnWriteVar)
	ON_THREAD_MESSAGE(WM_DYN_SEND_CONTROL_WORD, OnSendCtrlWord)
END_MESSAGE_MAP()


// CDynThread ��Ϣ�������

BOOL CDynThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

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
		// ��ƥ��
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

// ����IGBT��pwm��ռ�ձ�(%)
// ����pwmռ�ձ�Ϊ50%��
// XSI +0x00+0x20+0x7a+0x44+ ��У���룫C
DWORD CDynThread::XC_SetConstantIGBT(const float fpwm)
{
	return m_pDyn->XC_SetConstantIGBT(fpwm);
}

// ����ѡ�õ���ͨ����ͨ��1��ͨ��2��ͨ��3��
// Ĭ��ֵΪͨ��1��������Ϊͨ��1/2/3����������ֱ���õ�һ�������ֽ�����ʾѡ�õ�ͨ����
// XSI +0+0+0+1/2/3+ ��У���룫C
DWORD CDynThread::XC_SetConstantForceChannel(const int nChannel)
{
	return m_pDyn->XC_SetConstantForceChannel(nChannel);
}

// ������
//��Ŀǰ����ʹ�õ�ͨ���������㣺
// XSZ+0+0+0+0+ ��У���룫C
DWORD CDynThread::XC_SetConstantForceClearing(void)
{
	return m_pDyn->XC_SetConstantForceClearing();
}

// �������ֵ
DWORD CDynThread::XC_ClearKeyValue(void)
{
	return m_pDyn->XC_ClearKeyValue();
}

// ���ù�Ͳֱ��(cm)��
// ���Ͳֱ��Ϊ14.736cm��
//XCS +0xa8+0xc6+0x6b+0x41+ ��У���룫C
DWORD CDynThread::XC_SetConstantRollerDiameter(const int nRollerDim)
{
	return m_pDyn->XC_SetConstantRollerDiameter(nRollerDim);
}

// ���ñ�����������(ÿȦ)
// �������������Ϊ1000��
//XCS +0x00+0x00+0x7a+0x44+ ��У���룫C
DWORD CDynThread::XC_SetConstantEncoderPulNum(const int nEnPulNum)
{
	return m_pDyn->XC_SetConstantEncoderPulNum(nEnPulNum);
}

// ����ͨ��1���궨ϵ��b[0]��force=b[0]*(ad_value-c[0])
// ����ϵ��b[0]=1158.4��
// XCA +0xcd+0xcc+0x90+0x44+ ��У���룫C
DWORD CDynThread::XC_SetConstantForceCailFac_B1(const float fb)
{
	return m_pDyn->XC_SetConstantForceCailFac_B1(fb);
}

//����ͨ��1���궨ϵ��c[0]��	
// ����ϵ��c[0]=0.01��
//XCB +0x0a+0xd7+0x23+0x3c+ ��У���룫C
DWORD CDynThread::XC_SetConstantForceCailFac_C1(const float fc)
{
	return m_pDyn->XC_SetConstantForceCailFac_C1(fc);
}

//����ͨ��2���궨ϵ��b[1]��	
//����ϵ��b[1]=1158.4��
//XCC +0xcd+0xcc+0x90+0x44+ ��У���룫C
DWORD CDynThread::XC_SetConstantForceCailFac_B2(const float fb)
{
	return m_pDyn->XC_SetConstantForceCailFac_B2(fb);
}

//����ͨ��2���궨ϵ��c[1]��	
//����ϵ��c[1]=0.01��
//XCD +0x0a+0xd7+0x23+0x3c + ��У���룫C
DWORD CDynThread::XC_SetConstantForceCailFac_C2(const float fc)
{
	return m_pDyn->XC_SetConstantForceCailFac_B2(fc);
}

//����ͨ��3���궨ϵ��b[2]��	
//����ϵ��b[2]=1158.4��
//XCE +0xcd+0xcc+0x90+0x44 + ��У���룫C
DWORD CDynThread::XC_SetConstantForceCailFac_B3(const float fb)
{
	return m_pDyn->XC_SetConstantForceCailFac_B2(fb);
}

//����ͨ��3���궨ϵ��c[2]��	
//����ϵ��c[2]=0.01��
//XCF +0x0a+0xd7+0x23+0x3c  +��У���룫C
DWORD CDynThread::XC_SetConstantForceCailFac_C3(const float fc)
{
	return m_pDyn->XC_SetConstantForceCailFac_C3(fc);
}

//���ú��ٿ���PID���Ʊ���ϵ��kp��	
//����ϵ��c[2]=0.01��
//XCH +0x0a+0xd7+0x23+0x3c  +��У���룫C
DWORD CDynThread::XC_SetConstantSpeedControl_P(const float fkp)
{
	return m_pDyn->XC_SetConstantSpeedControl_P(fkp);
}

//���ú��ٿ���PID���ƻ���ϵ��ki��	
//����ϵ��c[2]=0.01��
//XCI +0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CDynThread::XC_SetConstantSpeedControl_I(const float fki)
{
	return m_pDyn->XC_SetConstantSpeedControl_I(fki);
}

//���ú��ٿ���PID����΢��ϵ��kd��	
//����ϵ��c[2]=0.01��
//XCJ+0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CDynThread::XC_SetConstantSpeedControl_D(const float fkd)
{
	return m_pDyn->XC_SetConstantSpeedControl_D(fkd);
}

//���ú�������PID���Ʊ���ϵ��kp_force��	
//����ϵ��c[2]=0.01��
//XCK +0x0a+0xd7+0x23+0x3c  +��У���룫C
DWORD CDynThread::XC_SetConstantForceControl_P(const float fkp)
{
	return m_pDyn->XC_SetConstantForceControl_P(fkp);
}

//���ú�������PID���Ʊ���ϵ�� ki_force��	
//����ϵ��c[2]=0.01��
//XCL +0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CDynThread::XC_SetConstantForceControl_I(const float fki)
{
	return m_pDyn->XC_SetConstantForceControl_I(fki);
}

//���ú�������PID���Ʊ���ϵ�� kd_force��	
//����ϵ��c[2]=0.01��
//XCM +0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CDynThread::XC_SetConstantForceControl_D(const float fkd)
{
	return m_pDyn->XC_SetConstantForceControl_D(fkd);
}

//�������ٶȿ���	XBS + 0+0+0+0+ ��У���� �� C
//������Ť�����	XBF + 0+0+0+0+ ��У���� �� C
//�����㹦�ʿ���	XBP + 0+0+ 0+0+ ��У���� �� C

//��������趨pwmֵ	XBI + 0+0+ 0+0+ ��У���� �� C
DWORD CDynThread::XC_StartSetPWN(const float fPWN)
{
	return m_pDyn->XC_StartSetPWN(fPWN);
}

//ֹͣ����,pwm���0	XBS + 0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_StopPWN(void)
{
	return m_pDyn->XC_StopPWN();
}

//�𶯵��	XEB + 0+0+0+0+ ��У���� �� C
//ֹͣ���	XES + 0+0+0+0+ ��У���� �� C
//��ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH1: XEO + 0+0+0+1+ ��У���� �� C
DWORD CDynThread::XC_OpenRelay(const int nLH)
{
	return m_pDyn->XC_OpenRelay(nLH);
}

//�ر�ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH3: XEF + 0+0+0+3+ ��У���� �� C
DWORD CDynThread::XC_CloseRelay(const int nLH)
{
	return m_pDyn->XC_CloseRelay(nLH);
}

//����������	XJU+ 0+0+0+0+ ��У���� �� C
//�������½�	XJD +0+0+0+0+ ��У���� �� C
//����궨״̬	XDF +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_EnterCaliSta(void)
{
	return m_pDyn->XC_EnterCaliSta();
}

//�˳��궨״̬	 XDS +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_ExitCaliSta(void)
{
	return m_pDyn->XC_ExitCaliSta();
}

//��ȡ��������	 XRT +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_ReadEnvPar(float *const pfET/*=NULL*/, float *const pfAP/*=NULL*/, float *const pfRH/*=NULL*/)
{
	return m_pDyn->XC_ReadEnvPar(pfET, pfAP, pfRH);
}

//��ȡ���궨ϵ��b,c	XRF +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_ReadForCalFac(float *const pfb/*=NULL*/, float *const pfc/*=NULL*/, float *const pfb1/*=NULL*/, float *const pfc1/*=NULL*/, float *const pfb2/*=NULL*/, float *const pfc2/*=NULL*/)
{
	return m_pDyn->XC_ReadForCalFac(pfb, pfc, pfb1, pfc1, pfb2, pfc2);
}

//��ȡ��ͨ��	XRC +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_ReadForCha(int *const pnCha/*=NULL*/)
{
	return m_pDyn->XC_ReadForCha(pnCha);
}

//��ȡ����PIDϵ��	XRs +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_ReadConSpeed(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	return m_pDyn->XC_ReadConSpeed(pfP, pfI, pfD);
}

//��ȡ����PIDϵ��	XRf +0+0+0+0+ ��У���� �� C	
DWORD CDynThread::XC_ReadConForce(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	return m_pDyn->XC_ReadConForce(pfP, pfI, pfD);
}

//��ȡ��Ͳֱ���ͱ���������	 XRS +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_ReadRollerAndEncoder(int *const pnDia/*=NULL*/, int *const pnPul/*=NULL*/)
{
	return m_pDyn->XC_ReadRollerAndEncoder(pnDia, pnPul);
}

//��3��ͨ���ı궨ϵ���̻���ROM��	 XMA +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_SolCalFac(void)
{
	return m_pDyn->XC_SolCalFac();
}

//����Ͳֱ���ͱ���������̻���ROM��	XMS +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_SolRollerAndEncoder(void)
{
	return m_pDyn->XC_SolRollerAndEncoder();
}

//����ͨ����ѡ��̻���ROM��	XMC +0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_SolForCha(void)
{
	return m_pDyn->XC_SolForCha();
}

//�����ٿ��Ƶ�PIDϵ���̻���ROM��	XMD + 0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_SolConSheepPID(void)
{
	return m_pDyn->XC_SolConSheepPID();
}

//���������Ƶ�PIDϵ���̻���ROM��	XME + 0+0+0+0+ ��У���� �� C
DWORD CDynThread::XC_SolConForcePID(void)
{
	return m_pDyn->XC_SolConForcePID();
}