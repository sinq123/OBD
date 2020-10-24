// CFlowThread.cpp : 实现文件
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
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL CFlowThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CFlowThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

// CFlowThread 消息处理程序

BOOL CFlowThread::PreTranslateMessage(MSG* pMsg)
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




// 1.获取流量计实时数据
DWORD CFlowThread::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult)
{
	
	m_dwStatus = m_pFlow->GetRealTimeData_Flowmeter(pfFluxStd0, pfFluxStd20, pfFluxAct, pfPres, pfUDilO2, pfGasTemp, pbResult);
	return m_dwStatus;
}

// 2.获取状态
DWORD CFlowThread::Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau)
{	
	m_dwStatus = m_pFlow->Getstatus_Flowmeter(pbResult,pbMachineStau,pbFanStau,pbFlowStau);
	return m_dwStatus;
}

// 3.开关风机
DWORD CFlowThread::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult)
{
	
	m_dwStatus = m_pFlow->RunFan_Flowmeter(bRun, pbResult);
	return m_dwStatus;
}

// 4.设置标准氧气
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult)
{	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_Zero(pbResult);
	return m_dwStatus;
}

// 5.设置标准压力
DWORD CFlowThread::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	
	m_dwStatus = m_pFlow->SetCalibrationPres_Flowmeter(pbResult,fCalPres);
	return m_dwStatus;
}

// 6.设置温度
DWORD CFlowThread::SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp)
{
	
	m_dwStatus = m_pFlow->SetCalibrationTemp_Flowmeter(pbResult,fCalTemp);
	return m_dwStatus;
}

// 7.设置标准流量 注:fCalFlux需标准化到20℃状态下 
DWORD CFlowThread::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CNanhuaNHF1::FluxCalibrationMode cm, const float fCalFlux)
{
	
	m_dwStatus = m_pFlow->SetCalibrationFlux_Flowmeter(pbResult,cm,fCalFlux);
	return m_dwStatus;
}

// 8.设置高点标准氧气(适用国产流量计)
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_High(pbResult,fCalDilO2);
	return m_dwStatus;
}

// 9.设置中点标准氧气(适用国产流量计)
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_Middle(pbResult,fCalDilO2);
	return m_dwStatus;
}

// 10.设置低点标准氧气(适用国产流量计)
DWORD CFlowThread::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	
	m_dwStatus = m_pFlow->SetCalibrationDilO2_Flowmeter_Low(pbResult,fCalDilO2);
	return m_dwStatus;
}

// 11.恢复出厂设置
DWORD CFlowThread::RestoreFactorySettings_Flowmeter(BYTE *const pbResult)
{
	
	m_dwStatus = m_pFlow->RestoreFactorySettings_Flowmeter(pbResult);
	return m_dwStatus;
}



