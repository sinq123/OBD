// CFlowThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EquThr.h"
#include "FlowThread.h"


// CFlowThread

IMPLEMENT_DYNCREATE(CFlowThread, CWinThread)

CFlowThread::CFlowThread()
: m_bQuitThread(false)
, m_fFluxStd0(0)
, m_fFluxStd20(0)
, m_fFluxAct(0)
, m_fPres(0)
, m_fUDilO2(0)
, m_fGasTemp(0)
, m_pFlow(NULL)
, m_dwStatus(0x00)
{
	InitializeInstrument();
}

CFlowThread::~CFlowThread()
{
	// ��������ʱ,ж���豸
	UninitializeInstrument();
}

BOOL CFlowThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CFlowThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

// CFlowThread ��Ϣ�������

BOOL CFlowThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
}

void CFlowThread::InitializeInstrument()
{
	UninitializeInstrument();
	m_pFlow = new CNanhuaNHF1();

	
}

void CFlowThread::UninitializeInstrument(void)
{
	if (NULL != m_pFlow)
	{
		delete m_pFlow;
		m_pFlow = NULL;
	}
}

void CFlowThread::QuitThread()
{
	m_bQuitThread = true;
	PostThreadMessage(WM_QUIT, 0, 0);
}

DWORD CFlowThread::Open(const BYTE bPort)
{
	InitializeInstrument();
	DWORD dw = m_pFlow->Open(bPort);
	return dw;
}

DWORD CFlowThread::Close(void)
{
	m_dwStatus = m_pFlow->Close();
	return m_dwStatus;
}

bool CFlowThread::IsOpen(void) const
{
	return m_pFlow->IsOpen();
}




// 1.��ȡ������ʵʱ����
DWORD CFlowThread::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult)
{
	
	m_dwStatus = m_pFlow->GetRealTimeData_Flowmeter(pfFluxStd0, pfFluxStd20, pfFluxAct, pfPres, pfUDilO2, pfGasTemp, pbResult);
	return m_dwStatus;
}

// 2.��ȡ״̬
DWORD CFlowThread::Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau)
{	
	m_dwStatus = m_pFlow->Getstatus_Flowmeter(pbResult,pbMachineStau,pbFanStau,pbFlowStau);
	return m_dwStatus;
}

// 3.���ط��
DWORD CFlowThread::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult)
{
	
	m_dwStatus = m_pFlow->RunFan_Flowmeter(bRun, pbResult);
	return m_dwStatus;
}

// 4.���ñ�׼����
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult)
{	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_Zero(pbResult);
	return m_dwStatus;
}

// 5.���ñ�׼ѹ��
DWORD CFlowThread::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	
	m_dwStatus = m_pFlow->SetCalibrationPres_Flowmeter(pbResult,fCalPres);
	return m_dwStatus;
}

// 6.�����¶�
DWORD CFlowThread::SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp)
{
	
	m_dwStatus = m_pFlow->SetCalibrationTemp_Flowmeter(pbResult,fCalTemp);
	return m_dwStatus;
}

// 7.���ñ�׼���� ע:fCalFlux���׼����20��״̬�� 
DWORD CFlowThread::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CNanhuaNHF1::FluxCalibrationMode cm, const float fCalFlux)
{
	
	m_dwStatus = m_pFlow->SetCalibrationFlux_Flowmeter(pbResult,cm,fCalFlux);
	return m_dwStatus;
}

// 8.���øߵ��׼����(���ù���������)
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_High(pbResult,fCalDilO2);
	return m_dwStatus;
}

// 9.�����е��׼����(���ù���������)
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_Middle(pbResult,fCalDilO2);
	return m_dwStatus;
}

// 10.���õ͵��׼����(���ù���������)
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_Low(pbResult,fCalDilO2);
	return m_dwStatus;
}

// 11.�ָ���������
DWORD CFlowThread::RestoreFactorySettings_Flowmeter(BYTE *const pbResult)
{
	
	m_dwStatus = m_pFlow->RestoreFactorySettings_Flowmeter(pbResult);
	return m_dwStatus;
}



