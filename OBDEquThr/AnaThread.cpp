// AnaThread.cpp : 实现文件
//

#include "stdafx.h"
#include "AnaThread.h"

// 分析仪流量计通讯模式切换次数
#ifndef MODE_SWITCH_AMOUNT 
#define MODE_SWITCH_AMOUNT 50
#endif


// CAnaThread

IMPLEMENT_DYNCREATE(CAnaThread, CWinThread)

CAnaThread::CAnaThread()
: m_bQuitThread(false)
, m_sHC(0)
, m_fCO(0)
, m_fCO2(0)
, m_fO2(0)
, m_sNO(0)
, m_sNO2(0)
, m_sNOx(0)
, m_usRpm(0)
, m_fOilTemp(0)
, m_usFlux(0)
, m_fLambda(0)
, m_bO2Sensor(false)
, m_bNOSensor(false)
, m_bWaterFull(false)

, m_fET(0.0f)
, m_fAP(0.0f)
, m_fRH(0.0f)

, m_fPef(0)

, m_bStatus(0)

, m_fFluxStd0(0)
, m_fFluxStd20(0)
, m_fFluxAct(0)
, m_fPres(0)
, m_fUDilO2(0)

, m_fHCwt(0)
, m_fCOwt(0)
, m_fCO2wt(0)
, m_fNOwt(0)

, m_sHCct(0)
, m_fCOct(0)
, m_fCO2ct(0)
, m_sNOct(0)
, m_fDilO2(0)
, m_fBenchO2(0)
, m_sGasTemp(0)
, m_fGasFluxStd0(0)
, m_fGasFluxStd20(0)
, m_fExhaustFluxStd0(0)
, m_fExhaustFluxStd20(0)
, m_fGasPres(0)
, m_fGasFluxAct(0)
, m_usVmasRpm(0)
, m_dwAnaWRSta(ANA_WR_SUCCEED)
, m_dwFloWRSta(ANA_WR_SUCCEED)

, m_usTotalHCwt(0)
, m_usTotalCOwt(0)
, m_usTotalCO2wt(0)
, m_usTotalNOwt(0)
, m_fTotalExhaustVolume(0.0f)
, m_fTotalGasVolume(0.0f)

, m_fEnvO2(0)

, m_wMiniStatus(0)

, m_pAna(NULL)

, m_opUsedNow(CAna::ToAnalyzer)
, m_dwStatus(0x00)
{
	// 对象创建时,初始化设备为:503V
	InitializeInstrument(L"NHA503V");
}

CAnaThread::~CAnaThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();
}

BOOL CAnaThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CAnaThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAnaThread, CWinThread)
	ON_THREAD_MESSAGE(WM_ANA_GET_MAJOR_DATA, OnGetMajorData)
	ON_THREAD_MESSAGE(WM_ANA_GET_MAJOR_DATA_N, OnGetMajorDataN)
	ON_THREAD_MESSAGE(WM_ANA_GET_NOX_DATA, OnGetNOxData)
	ON_THREAD_MESSAGE(WM_ANA_GET_MINOR_DATA, OnGetMinorData)
	ON_THREAD_MESSAGE(WM_ANA_GET_PEF, OnGetPef)
	ON_THREAD_MESSAGE(WM_ANA_GET_STATUS, OnGetStatus)
	ON_THREAD_MESSAGE(WM_ANA_GASCALIBRATION, OnGasCalibration)
	ON_THREAD_MESSAGE(WM_ANA_GASCHECK, OnGasCheck)

	ON_THREAD_MESSAGE(WM_ANA_VMAS_REALTIME_DATA, OnGetRealTimeData_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_PDS_WT_DATA, OnGetProcessDataStatus_WT_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_PDS_CT_DATA, OnGetProcessDataStatus_CT_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_TOTAL_WT_DATA, OnGetTotalWeight_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_START_COLLECT_DATA, OnStartCollectData_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_STOP_COLLECT_DATA, OnStopCollectData_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_START_GETENVIRONMENT_O2, OnSatrtGetEnvironmentO2_Flowmeter)
	ON_THREAD_MESSAGE(WM_ANA_VMAS_GET_ENVIRONMENT_O2, OnGetEnvironmentO2_Flowmeter)
END_MESSAGE_MAP()


// CAnaThread 消息处理程序

BOOL CAnaThread::PreTranslateMessage(MSG* pMsg)
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

void CAnaThread::InitializeInstrument(const wchar_t *szProtocol)
{
	UninitializeInstrument();
	
	if (_wcsicmp(szProtocol, L"NHA503") == 0
		|| _wcsicmp(szProtocol, L"503") == 0
		)
	{
		m_pAna = new CNanhuaNha503();
		m_emInstrumentModel = IM_NANHUA_NHA503;
	}
	else if (_wcsicmp(szProtocol, L"NHA503V") == 0
			|| _wcsicmp(szProtocol, L"503V") == 0
			)
	{
		m_pAna = new CNanhuaNha503V();
		m_emInstrumentModel = IM_NANHUA_NHA503V;
	}
	else if (_wcsicmp(szProtocol, L"ML100") == 0)
	{
		m_pAna = new CMingQuanML100();
		m_emInstrumentModel = IM_MINGQUAN_ML100;
	}
	else if (_wcsicmp(szProtocol, L"MQW511") == 0)
	{
		m_pAna = new CMingQuanMQW511();
		m_emInstrumentModel = IM_MINGQUAN_MQW511;
	}
	else if (_wcsicmp(szProtocol, L"MQW50A") == 0)
	{
		m_pAna = new CMingQuanMQW50A();
		m_emInstrumentModel = IM_MINGQUAN_MQW50A;
	}
	else if (_wcsicmp(szProtocol, L"FLA501") == 0)
	{
		m_pAna = new CFuLiFLA501();
		m_emInstrumentModel = IM_FULI_FLA501;
	}
	else if (_wcsicmp(szProtocol, L"FLA502") == 0)
	{
		m_pAna = new CFuLiFLA502();
		m_emInstrumentModel = IM_FULI_FLA502;
	}
	else if (_wcsicmp(szProtocol, L"FLV1000") == 0)
	{
		m_pAna = new CFuLiFLV1000();
		m_emInstrumentModel = IM_FULI_FLV1000;
	}
	else if (_wcsicmp(szProtocol, L"FGA4100") == 0)
	{
		m_pAna = new CFoFenFGA4100();
		m_emInstrumentModel = IM_FOFEN_FGA4100;
	}
	else if (_wcsicmp(szProtocol, L"NHA501A") == 0
			|| _wcsicmp(szProtocol, L"501A") == 0
			)
	{
		m_pAna = new CNanhuaNha501A();
		m_emInstrumentModel = IM_NANHUA_NHA501A;
	}
	else if (_wcsicmp(szProtocol, L"NHA506I") == 0
			|| _wcsicmp(szProtocol, L"NHA605I") == 0
			|| _wcsicmp(szProtocol, L"506I") == 0
			|| _wcsicmp(szProtocol, L"605I") == 0
			)
	{
		m_pAna = new CNanhuaNha506I();
		m_emInstrumentModel = IM_NANHUA_NHA506I;
	}
	else if (_wcsicmp(szProtocol, L"NHA509") == 0
			|| _wcsicmp(szProtocol, L"NHA509En") == 0
			)
	{
		m_pAna = new CNanhuaNha509Encrypt();
		m_emInstrumentModel = IM_NANHUA_NHA509EN;
	}
	else if (_wcsicmp(szProtocol, L"NHA610") == 0
			|| _wcsicmp(szProtocol, L"NHA610En") == 0
			)
	{
		m_pAna = new CNanhuaNha610Encrypt();
		m_emInstrumentModel = IM_NANHUA_NHA610EN;
	}
	else if (_wcsicmp(szProtocol, L"NHA100") == 0)
	{
		m_pAna = new CNanhuaNha100();
		m_emInstrumentModel = IM_NANHUA_NHA100;
	}
	else
	{
		// 无匹配
		m_pAna = new CNanhuaNha509Encrypt();
		m_emInstrumentModel = IM_NANHUA_NHA509EN;
	}
}

void CAnaThread::UninitializeInstrument(void)
{
	if (NULL != m_pAna)
	{
		delete m_pAna;
		m_pAna = NULL;
	}
}

void CAnaThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

HANDLE CAnaThread::GetComHandle(void)
{
	return m_pAna->GetComHandle();
}

void CAnaThread::AttachComHandle(HANDLE hCom, const wchar_t *szProtocol) 
{
	InitializeInstrument(szProtocol);
	m_pAna->AttachComHandle(hCom); 
}

void CAnaThread::DetachComHandle(void)
{
	m_pAna->DetachComHandle();
}

CRITICAL_SECTION CAnaThread::GetCriticalSection(void)
{
	return m_pAna->GetCriticalSection();
}

void CAnaThread::AttachCriticalSection(CRITICAL_SECTION cs)
{
	m_pAna->AttachCriticalSection(cs);
}

void CAnaThread::DetachCriticalSection(void)
{
	m_pAna->DetachCriticalSection();
}

DWORD CAnaThread::Open(const BYTE bPort, const wchar_t *szProtocol, const bool bConnectToFlowmeter/*=false*/)
{
	InitializeInstrument(szProtocol);

	DWORD dw = m_pAna->Open(bPort);

	if (bConnectToFlowmeter)
	{
		// 连接流量计
		// 初始化为分析仪通讯模式
		if (ANA_OPEN_SUCCEED == dw)
		{
			m_opUsedNow = CAna::ToFlowmeter;
			if (0x00 != SetAnalyzerOperationMode())
			{
				// TODO:切换不成功,暂不作任何操作
			}
		}
	}
	m_dwStatus=dw;
	return dw;
}

DWORD CAnaThread::Close(void)
{
	m_dwStatus = m_pAna->Close();
	return m_dwStatus;
}

// 1取主数据
DWORD CAnaThread::GetMajorData(short *const psHC, 
							float *const pfCO, 
							float *const pfCO2, 
							float *const pfO2, 
							short *const psNO,
							short *const psNO2,
							short *const psNOx,
							USHORT *const pusRpm, 
							float *const pfOilTemp, 
							USHORT *const pusFlux, 
							float *const pfLambda,
							bool *const pbO2Sensor,
							bool *const pbNOSensor)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetMajorData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx, pusRpm, pfOilTemp, pusFlux, pfLambda, pbO2Sensor, pbNOSensor);
	return m_dwStatus;
}

DWORD CAnaThread::GetMajorData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO,
		short *const psNO2,
		short *const psNOx,
		USHORT *const pusRpm, 
		float *const pfOilTemp, 
		USHORT *const pusFlux, 
		float *const pfLambda,
		bool *const pbO2Sensor,
		bool *const pbNOSensor,
		bool *const pbWaterFull)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetMajorData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx, pusRpm, pfOilTemp, pusFlux, pfLambda, pbO2Sensor, pbNOSensor, pbWaterFull);
	return m_dwStatus;
}

DWORD CAnaThread::GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetNOxData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx);
	return m_dwStatus;
}
// 2取辅助数据
// ET:Environment Temperature缩写
// AP:Atmospheric Pressure缩写
// RH:Relative Humidity缩写
DWORD CAnaThread::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetMinorData(pfET,pfAP,pfRH);
	return m_dwStatus;
}

// 3抽空气
DWORD CAnaThread::DeflateEnvAir(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DeflateEnvAir(pbResult);
	return m_dwStatus;
}

// 4抽管道气(开泵,气样从探头进入)
DWORD CAnaThread::DeflatePipeAir(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DeflatePipeAir(pbResult);
	return m_dwStatus;
}

// 5清洗管道(反吹)
DWORD CAnaThread::InverseBlow(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->InverseBlow(pbResult);
	return m_dwStatus;
}

// 6停止清洗(停止反吹及停泵)
DWORD CAnaThread::StopPump(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->StopPump(pbResult);
	return m_dwStatus;
}

// 7检漏
DWORD CAnaThread::CheckLeak(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->CheckLeak(pbResult);
	return m_dwStatus;
}

// 8调零
DWORD CAnaThread::Zero(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->Zero(pbResult);
	return m_dwStatus;
}

// 9写入校准气浓度
DWORD CAnaThread::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
	const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationGasConcentration(pbResult,nPoint,usCalC3H8,fCalCO,fCalCO2,usCalNO,usCalNO2);
	return m_dwStatus;
}

// 10校准
DWORD CAnaThread::GasCalibration(BYTE *const pbResult, const int nChanel, 
								 short *const psHC, 
								 float *const pfCO, 
								 float *const pfCO2, 
								 float *const pfO2, 
								 short *const psNO, 
								 short *const psNO2, 
								 float *const pfPef)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GasCalibration(pbResult,nChanel,psHC,pfCO,pfCO2,pfO2,psNO,psNO2,pfPef);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationNO2Concentration(pbResult,usNO2);
	return m_dwStatus;
}

DWORD CAnaThread::NO2Calibration(BYTE *const pbResult, short *const psNO2/*=NULL*/)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->NO2Calibration(pbResult, psNO2);
	return m_dwStatus;
}

// 11写入检查气浓度
DWORD CAnaThread::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCheckGasConcentration(pbResult,usChkC3H8,fChkCO,fChkCO2,usChkNO);
	return m_dwStatus;
}

// 12自动检查
DWORD CAnaThread::GasCheck(BYTE *const pbResult, 
						   short *const psHC, 
						   float *const pfCO, 
						   float *const pfCO2, 
						   float *const pfO2, 
						   short *const psNO, 
						   float *const pfPef)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GasCheck(pbResult,psHC,pfCO,pfCO2,pfO2,psNO,pfPef);
	return m_dwStatus;
}

// 13取PEF
DWORD CAnaThread::GetPef(float *const pfPef)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetPef(pfPef);
	return m_dwStatus;
}

// 14清除NO老化标志
// 503特有
DWORD CAnaThread::ClearNoSign(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->ClearNoSign(pbResult);
	return m_dwStatus;
}

// 15设置调零气体为空气
DWORD CAnaThread::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetZeroAirTypeToEnvAir(pbResult);
	return m_dwStatus;
}

// 16设置调零气体为零气
DWORD CAnaThread::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetZeroAirTypeToZeroAir(pbResult);
	return m_dwStatus;
}

// 17设置冲程为4
DWORD CAnaThread::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetStrokeTypeToFourStroke(pbResult);
	return m_dwStatus;
}

// 18设置冲程为2
DWORD CAnaThread::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetStrokeTypeToTwoStroke(pbResult);
	return m_dwStatus;
}

// 19设置点火方式为单次
DWORD CAnaThread::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetFireTypeToSingleFire(pbResult);
	return m_dwStatus;
}

// 20设置点火方式为二次
DWORD CAnaThread::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetFireTypeToDoubleFire(pbResult);
	return m_dwStatus;
}

// 21允许自动调零
DWORD CAnaThread::EnableAutoZero(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->EnableAutoZero(pbResult);
	return m_dwStatus;
}

// 22禁止自动调零
DWORD CAnaThread::DisableAutoZero(BYTE *const pbResult)
{
	m_dwStatus = SetAnalyzerOperationMode();
	if (0x00 != m_dwStatus)
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DisableAutoZero(pbResult);
	return m_dwStatus;
}

// 23允许键盘操作

// 24禁止键盘操作

// 25LCD开

// 26LCD关

// 27环境温度校准
DWORD CAnaThread::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->ETCalibration(pbResult,fCalET);
	return m_dwStatus;
}

// 28室温校准

// 29油温校准
DWORD CAnaThread::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->OTCalibration(pbResult,fCalOT);
	return m_dwStatus;
}

// 30环境湿度校准
DWORD CAnaThread::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->RHCalibration(pbResult,fCalRH);
	return m_dwStatus;
}

// 31大气压力校准
DWORD CAnaThread::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->APCalibration(pbResult,fCalAP);
	return m_dwStatus;
}

// 32检查气开
DWORD CAnaThread::OpenCheckGas(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->OpenCheckGas(pbResult);
	return m_dwStatus;
}

// 33检查气关
DWORD CAnaThread::CloseCheckGas(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->CloseCheckGas(pbResult);
	return m_dwStatus;
}

// 34取仪器状态
DWORD CAnaThread::GetStatus(BYTE *const pbStatus)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetStatus(pbStatus);
	return m_dwStatus;
}

// 35取预热剩余时间
DWORD CAnaThread::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetWarmUpTimeLeft(pusSecondsLeft);
	return m_dwStatus;
}

// 36设置燃料类型为汽油
DWORD CAnaThread::SetFuelTypeToGas(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetFuelTypeToGas(pbResult);
	return m_dwStatus;
}

// 37设置燃料类型为液化石油气
DWORD CAnaThread::SetFuelTypeToLPG(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetFuelTypeToLPG(pbResult);
	return m_dwStatus;
}

// 38设置燃料类型为天然气
DWORD CAnaThread::SetFuelTypeToNG(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetFuelTypeToNG(pbResult);
	return m_dwStatus;
}

// 39设置燃料类型为乙醇汽油
DWORD CAnaThread::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	if (0x00 != SetAnalyzerOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetFuelTypeToGasohol(pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::SetOperationMode(enum CAna::OperationMode op, BYTE *const pbResult)
{
	BYTE bResult(0);
	m_dwStatus = m_pAna->SetOperationMode(op, &bResult);
	if (NULL != pbResult)
	{
		*pbResult = bResult;
	}
	if (0x06 == bResult)
	{
		// 流量计返回:设置模式成功
		m_opUsedNow = op;
	}

	return m_dwStatus;

	//return m_pAna->SetOperationMode(op, pbResult);
}

DWORD CAnaThread::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	m_dwStatus = m_pAna->GetProcessStatus_Flowmeter(pbResult, pwProcessStatus, pwProcess);
	return m_dwStatus;
}

// 开关风机
DWORD CAnaThread::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->RunFan_Flowmeter(bRun, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::StartCollectData_Flowmeter(const WORD wCollectionTime, WORD *const pwMiniStatus, BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->StartCollectData_Flowmeter(wCollectionTime, pwMiniStatus, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::StopCollectData_Flowmeter(WORD *const pwMiniStatus, BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->StopCollectData_Flowmeter(pwMiniStatus, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetCollectedDataNumber_Flowmeter(pwCorrectedDataNumber, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetRealTimeData_Flowmeter(pfFluxStd0, pfFluxStd20, pfFluxAct, pfPres, pfUDilO2, pfGasTemp, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}

	m_dwStatus = m_pAna->GetRealTimeData_Flowmeter_ALL(pwStatus0,pwStatus1,pwStatus2,pwStatus3,pwStatus4,pwStatus5,pwStatus6,pwStatus7,pwStatus8,pwStatus9,pwStatus10,pwStatus11,pwStatus12,pwStatus13,pwStatus14,pwStatus15,
		pwStatus16,pwStatus17,pwStatus18,pwStatus19,pwStatus20,pwStatus21,pwStatus22,pwStatus23,pwStatus24,pwStatus25,pfFluxStd0,pfFluxStd20,pfFluxAct, pfPres, pfUDilO2, pfGasTemp, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus,
		BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetProcessDataStatus_WT_Flowmeter(wCntStart, wCntLength, pfHCwt, pfCOwt, pfCO2wt, pfNOwt, pwMiniStatus, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
														   const WORD wCntLength,
														   short *const psHCct, 
														   float *const pfCOct, 
														   float *const pfCO2ct, 
														   short *const psNOct, 
														   float *const pfDilO2, 
														   float *const pfBenchO2, 
														   short *const psGasTemp, 
														   float *const pfGasFluxStd0, 
														   float *const pfGasFluxStd20, 
														   float *const pfExhaustFluxStd0, 
														   float *const pfExhaustFluxStd20, 
														   float *const pfGasPres, 
														   float *const pfGasFluxAct, 
														   USHORT *const pusRpm,
														   WORD *const pwMiniStatus,
														   const BYTE bDataMode,
														   BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetProcessDataStatus_CT_Flowmeter(wCntStart,
		wCntLength,
		psHCct, 
		pfCOct, 
		pfCO2ct, 
		psNOct, 
		pfDilO2, 
		pfBenchO2, 
		psGasTemp, 
		pfGasFluxStd0, 
		pfGasFluxStd20, 
		pfExhaustFluxStd0, 
		pfExhaustFluxStd20, 
		pfGasPres, 
		pfGasFluxAct, 
		pusRpm,
		pwMiniStatus,
		bDataMode,
		pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus,
		BYTE *const pbResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetTotalWeight_Flowmeter(pusHCwt,pusCOwt,pusCO2wt,pusNOwt, pfExhaustVolume, pfGasVolume, pwMiniStatus, pbResult);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CAna::FluxCalibrationMode cm, const float fCalFlux)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationFlux_Flowmeter(pbResult,cm,fCalFlux);
	return m_dwStatus;
}

DWORD CAnaThread::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum CAna::FluxCalibrationMode cm)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->FluxCalibration_Flowmeter(pbResult, pbCalResult, cm);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationPres_Flowmeter(pbResult,fCalPres);
	return m_dwStatus;
}

DWORD CAnaThread::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->PresCalibration_Flowmeter(pbResult, pbCalResult);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationTemp_Flowmeter(pbResult,fCalTemp);
	return m_dwStatus;
}

DWORD CAnaThread::TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->TempCalibration_Flowmeter(pbResult, pbCalResult);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationDilO2_Flowmeter(pbResult,fCalDilO2);
	return m_dwStatus;
}

DWORD CAnaThread::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DilO2Calibration_Flowmeter(pbResult, pbCalResult, bDataRequestMode);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationDilO2_Flowmeter_High(pbResult,fCalDilO2);
	return m_dwStatus;
}

DWORD CAnaThread::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DilO2Calibration_Flowmeter_High(pbResult, pbCalResult, bDataRequestMode);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationDilO2_Flowmeter_Middle(pbResult,fCalDilO2);
	return m_dwStatus;
}

DWORD CAnaThread::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DilO2Calibration_Flowmeter_Middle(pbResult, pbCalResult, bDataRequestMode);
	return m_dwStatus;
}

DWORD CAnaThread::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetCalibrationDilO2_Flowmeter_Low(pbResult,fCalDilO2);
	return m_dwStatus;
}

DWORD CAnaThread::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->DilO2Calibration_Flowmeter_Low(pbResult, pbCalResult, bDataRequestMode);
	return m_dwStatus;
}

DWORD CAnaThread::SatrtGetEnvironmentO2_Flowmeter()
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SatrtGetEnvironmentO2_Flowmeter();
	return m_dwStatus;
}

DWORD CAnaThread::GetEnvironmentO2_Flowmeter(BYTE *const pbResult,float *const pfEnvO2)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetEnvironmentO2_Flowmeter(pbResult,pfEnvO2);
	return m_dwStatus;
}

DWORD CAnaThread::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetHCCOCO2DelayTime_Flowmeter(pbResult, pfDelayTime);
	return m_dwStatus;
}

DWORD CAnaThread::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetHCCOCO2DelayTime_Flowmeter(pbResult, fDelayTime);
	return m_dwStatus;
}

DWORD CAnaThread::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->HCCOCO2DelayTime_Flowmeter(pbResult, pbCalResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetO2DelayTime_Flowmeter(pbResult, pfDelayTime);
	return m_dwStatus;
}

DWORD CAnaThread::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetO2DelayTime_Flowmeter(pbResult, fDelayTime);
	return m_dwStatus;
}

DWORD CAnaThread::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->O2DelayTime_Flowmeter(pbResult, pbCalResult);
	return m_dwStatus;
}

DWORD CAnaThread::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->GetNOxDelayTime_Flowmeter(pbResult, pfDelayTime);
	return m_dwStatus;
}

DWORD CAnaThread::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->SetNOxDelayTime_Flowmeter(pbResult, fDelayTime);
	return m_dwStatus;
}

DWORD CAnaThread::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	if (0x00 != SetFlowmeterOperationMode())
	{
		return ANA_WR_UNKNOWN_ERROR;
	}
	m_dwStatus = m_pAna->NOxDelayTime_Flowmeter(pbResult, pbCalResult);
	return m_dwStatus;
}

void CAnaThread::GetMajorData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GET_MAJOR_DATA, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetMajorData(WPARAM wParam, LPARAM lParam)
{
	m_dwAnaWRSta = GetMajorData(&m_sHC, &m_fCO, &m_fCO2, &m_fO2, &m_sNO, &m_sNO2, &m_sNOx, &m_usRpm, &m_fOilTemp, &m_usFlux, &m_fLambda);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GET_MAJOR_DATA, (WPARAM)m_dwAnaWRSta, 0);
	}
}
///////////
void CAnaThread::GetMajorData_Asyn_N(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GET_MAJOR_DATA_N, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetNOxData(WPARAM wParam, LPARAM lParam)
{
	m_dwAnaWRSta = GetNOxData(&m_sHC, &m_fCO, &m_fCO2, &m_fO2, &m_sNO, &m_sNO2, &m_sNOx);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GET_NOX_DATA, (WPARAM)m_dwAnaWRSta, 0);
	}
}

void CAnaThread::OnGetMajorDataN(WPARAM wParam, LPARAM lParam)
{
	m_dwAnaWRSta = GetMajorData(&m_sHC, &m_fCO, &m_fCO2, &m_fO2, &m_sNO, &m_sNO2, &m_sNOx, &m_usRpm, &m_fOilTemp, &m_usFlux, &m_fLambda, &m_bO2Sensor, &m_bNOSensor, &m_bWaterFull);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GET_MAJOR_DATA_N, (WPARAM)m_dwAnaWRSta, 0);
	}
}


void CAnaThread::GetNOxData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GET_NOX_DATA, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::GetMinorData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GET_MINOR_DATA, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetMinorData(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = GetMinorData(&m_fET,&m_fAP,&m_fRH);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GET_MINOR_DATA, (WPARAM)dw, 0);
	}
}

void CAnaThread::GetPef_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GET_PEF, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetPef(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = GetPef(&m_fPef);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GET_PEF, (WPARAM)dw, 0);
	}
}

void CAnaThread::GetStatus_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GET_STATUS, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetStatus(WPARAM wParam, LPARAM lParam)
{
	DWORD dw =  GetStatus(&m_bStatus);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GET_STATUS, (WPARAM)dw, 0);
	}
}

void CAnaThread::GasCalibration_Aysn(const int nChanel, HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GASCALIBRATION, nChanel, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGasCalibration(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult;
	DWORD dw = GasCalibration(&bResult, wParam);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GASCALIBRATION, (WPARAM)dw, (LPARAM)bResult);
	}
}

void CAnaThread::GetRealTimeData_Flowmeter_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_REALTIME_DATA, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetRealTimeData_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult(0);
	m_dwFloWRSta = GetRealTimeData_Flowmeter(&m_fFluxStd0, &m_fFluxStd20, &m_fFluxAct, &m_fPres, &m_fUDilO2, &m_fGasTemp, &bResult);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_VMAS_REALTIME_DATA, (WPARAM)m_dwFloWRSta, (LPARAM)bResult);
	}
}

void CAnaThread::GetProcessDataStatus_WT_Flowmeter_Aysn(const WORD wCntStart, HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_PDS_WT_DATA, (WPARAM)wCntStart, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetProcessDataStatus_WT_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult(0);
	WORD wCntStart = (WORD)wParam;
	HWND hCallbackWnd = (HWND)lParam;
	DWORD dw = GetProcessDataStatus_WT_Flowmeter(wCntStart, 1, &m_fHCwt, &m_fCOwt, &m_fCO2wt, &m_fNOwt, &m_wMiniStatus, &bResult);

	if (ANA_WR_SUCCEED != dw)
	{
		bResult = ANA_WR_UNKNOWN_ERROR;
	}

	if (NULL != hCallbackWnd)
	{
		::PostMessage(hCallbackWnd, WM_ANA_VMAS_PDS_WT_DATA, (WPARAM)wCntStart, (LPARAM)bResult);
	}
}

void CAnaThread::GetProcessDataStatus_CT_Flowmeter_Aysn(const WORD wCntStart,HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_PDS_CT_DATA, (WPARAM)wCntStart, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetProcessDataStatus_CT_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult(0);
	WORD wCntStart = (WORD)wParam;
	HWND hCallbackWnd = (HWND)lParam;
	DWORD dw = GetProcessDataStatus_CT_Flowmeter(wCntStart, 
		1, 
		&m_sHCct, 
		&m_fCOct, 
		&m_fCO2ct, 
		&m_sNOct, 
		&m_fDilO2, 
		&m_fBenchO2, 
		&m_sGasTemp, 
		&m_fGasFluxStd0,
		&m_fGasFluxStd20,
		&m_fExhaustFluxStd0,
		&m_fExhaustFluxStd20,
		&m_fGasPres,
		&m_fGasFluxAct,
		&m_usVmasRpm, 
		&m_wMiniStatus, 
		0x01,
		&bResult);

	if (ANA_WR_SUCCEED != dw)
	{
		bResult = ANA_WR_UNKNOWN_ERROR;
	}

	if (NULL != hCallbackWnd)
	{
		::PostMessage(hCallbackWnd, WM_ANA_VMAS_PDS_CT_DATA, (WPARAM)wCntStart, (LPARAM)bResult);
	}
}

void CAnaThread::GetTotalWeight_Flowmeter_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_TOTAL_WT_DATA, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetTotalWeight_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult(0);
	HWND hCallbackWnd = (HWND)lParam;
	DWORD dw = GetTotalWeight_Flowmeter(&m_usTotalHCwt, &m_usTotalCOwt, &m_usTotalCO2wt, &m_usTotalNOwt, &m_fTotalExhaustVolume, &m_fTotalGasVolume, &m_wMiniStatus, &bResult);
	if (NULL != hCallbackWnd)
	{
		::PostMessage(hCallbackWnd, WM_ANA_VMAS_TOTAL_WT_DATA, (WPARAM)dw, (LPARAM)bResult);
	}
}

void CAnaThread::StartCollectData_Flowmeter_Aysn(const WORD wCollectionTime,HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_START_COLLECT_DATA, (WPARAM)wCollectionTime, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnStartCollectData_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult(0);
	HWND hCallbackWnd = (HWND)lParam;
	DWORD dw = StartCollectData_Flowmeter(WORD(wParam), &m_wMiniStatus, &bResult);
	if (NULL != hCallbackWnd)
	{
		::PostMessage(hCallbackWnd, WM_ANA_VMAS_START_COLLECT_DATA, (WPARAM)dw, (LPARAM)bResult);
	}
}

void CAnaThread::StopCollectData_Flowmeter_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_STOP_COLLECT_DATA, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnStopCollectData_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult(0);
	HWND hCallbackWnd = (HWND)lParam;
	DWORD dw = StopCollectData_Flowmeter(&m_wMiniStatus, &bResult);
	if (NULL != hCallbackWnd)
	{
		::PostMessage(hCallbackWnd, WM_ANA_VMAS_STOP_COLLECT_DATA, (WPARAM)dw, (LPARAM)bResult);
	}
}

void CAnaThread::SatrtGetEnvironmentO2_Flowmeter_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_START_GETENVIRONMENT_O2, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnSatrtGetEnvironmentO2_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	DWORD dw = SatrtGetEnvironmentO2_Flowmeter();
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_VMAS_START_GETENVIRONMENT_O2, (WPARAM)dw,0);
	}
}

void CAnaThread::GetEnvironmentO2_Flowmeter_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_VMAS_START_GETENVIRONMENT_O2, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGetEnvironmentO2_Flowmeter(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult;
	DWORD dw = GetEnvironmentO2_Flowmeter(&bResult,&m_fEnvO2);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_VMAS_START_GETENVIRONMENT_O2, (WPARAM)dw,(LPARAM)bResult);
	}
}


void CAnaThread::GasCheck_Aysn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_ANA_GASCHECK, 0, (LPARAM)hCallBackWnd);
}

void CAnaThread::OnGasCheck(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult;
	DWORD dw = GasCheck(&bResult);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_ANA_GASCHECK, (WPARAM)dw, (LPARAM)bResult);
	}
}

DWORD CAnaThread::SetAnalyzerOperationMode(void)
{
	DWORD dwSuccess(0xff);
	if (CAna::ToAnalyzer != GetOperationModeUsedNow())
	{
		for (int i=0; i<MODE_SWITCH_AMOUNT; i++)
		{
			BYTE bResult(0);
			SetOperationMode(CAna::ToAnalyzer, &bResult);

			if (0x05 == bResult)
			{
				// 延时后重新设置分析仪操作模式
				Sleep(50);
			}
			else
			{
				dwSuccess = 0x00;
				break;
			}
		}
	}
	else
	{
		dwSuccess = 0x00;
	}

	m_dwStatus = dwSuccess;
	return m_dwStatus;
}

DWORD CAnaThread::SetFlowmeterOperationMode(void)
{
	DWORD dwSuccess(0xff);
	if (CAna::ToFlowmeter != GetOperationModeUsedNow())
	{
		for (int i=0; i<MODE_SWITCH_AMOUNT; i++)
		{
			BYTE bResult(0);
			SetOperationMode(CAna::ToFlowmeter, &bResult);

			if (0x05 == bResult)
			{
				// 延时后重新设置流量计通讯模式
				Sleep(50);
			}
			else
			{
				dwSuccess = 0x00;
				break;
			}
		}
	}
	else
	{
		dwSuccess = 0x00;
	}
	m_dwStatus = dwSuccess;
	return m_dwStatus;
}

// 获取手动检测的数据
DWORD CAnaThread::GetTestResultData(short *const psHCHigh/*=NULL*/, 
		float *const pfCOHigh/*=NULL*/, 
		float *const pfCO2High/*=NULL*/, 
		float *const pfO2High/*=NULL*/, 
		short *const psNOHigh/*=NULL*/,
		USHORT *const pusRpmHigh/*=NULL*/, 
		float *const pfOilTempHigh/*=NULL*/, 
		float *const pfLambdaHigh/*=NULL*/,
		short *const psHCLow/*=NULL*/, 
		float *const pfCOLow/*=NULL*/, 
		float *const pfCO2Low/*=NULL*/, 
		float *const pfO2Low/*=NULL*/, 
		short *const psNOLow/*=NULL*/,
		USHORT *const pusRpmLow/*=NULL*/, 
		float *const pfOilTempLow/*=NULL*/, 
		float *const pfLambdaLow/*=NULL*/)
{
	m_dwStatus = m_pAna->GetTestResultData(psHCHigh, pfCOHigh, pfCO2High, pfO2High, psNOHigh, pusRpmHigh, pfOilTempHigh, pfLambdaHigh
							, psHCLow, pfCOLow, pfCO2Low, pfO2Low, psNOLow, pusRpmLow, pfOilTempLow, pfLambdaLow);
	return m_dwStatus;
}