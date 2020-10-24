// ParaThread.cpp : 实现文件
//

#include "stdafx.h"
#include "ParaThread.h"

//#include "ZBase64.h"


//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "../ZipLib/zLib/zlib.h"
#ifdef _DEBUG
#pragma comment(lib, "../ZipLib/zLib/zlibd.lib")
#else
#pragma comment(lib, "../ZipLib/zLib/zlib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif


//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT


// CParaThread

IMPLEMENT_DYNCREATE(CParaThread, CWinThread)

CParaThread::CParaThread()
: m_bQuitThread(false)
, m_pPara(NULL)
, m_fET(0.0f)
, m_fRH(0.0f)
, m_fAP(0.0f)
, m_fOilTemp(0.0f)
, m_usRpm(0)
, m_dwStatus(0x00)
, m_bOpenDB(false)
, m_bWorkerRunning(false)
, m_nWorker(0)
{
	// 初始化ICCID
	strncpy_s(m_chICCID,"00000000000000000000",20);
	m_chICCID[20] = '\0';

	// 对象创建时,初始化设备为:NanhuaPara
	InitializeInstrument(IM_NANHUA_PARA);

	m_hCloseWorker = CreateEvent(NULL, TRUE, FALSE, NULL);

	::InitializeCriticalSection(&m_sCriticalSection);
}

CParaThread::~CParaThread()
{
	// 对象销毁时,卸载设备
	UninitializeInstrument();

	CloseHandle(m_hCloseWorker);

	::DeleteCriticalSection(&m_sCriticalSection);
}

BOOL CParaThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CParaThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CParaThread, CWinThread)
	ON_THREAD_MESSAGE(WM_PARA_GET_ENTIRE_DATA, OnGetEntireData)
	ON_THREAD_MESSAGE(WM_PARA_GET_ENVIRONMENT_PARAMETERS, OnGetEnvironmentParameters)
	ON_THREAD_MESSAGE(WM_PARA_GET_OILTEMP_RPM, OnGetOilTempRpm)
END_MESSAGE_MAP()


// CParaThread 消息处理程序

BOOL CParaThread::PreTranslateMessage(MSG* pMsg)
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

void CParaThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

void CParaThread::InitializeInstrument(const enum InstrumentModel im)
{
	UninitializeInstrument();

	m_emIM = im;

	if (IM_NANHUA_PARA == m_emIM)
	{
		m_pPara = new CNanhuaPara();
	}
	else if (IM_NANHUA_PARA_Socket == m_emIM)
	{
		m_pPara = new CNanhuaSocketPara();
	}
	else if (IM_PARA_DIM == m_emIM)
	{
		m_pPara = new CDigitalThermometer();
	}
	else if (IM_PARA_DIM_NJRJ == m_emIM)
	{
		m_pPara = new CDigitalThermometer_NJRJ();
	}
	else if (IM_PARA_SG506 == m_emIM)
	{
		m_pPara = new CSG506();
	}
	else if (IM_NANHUA_EPCS == m_emIM)
	{
		m_pPara = new CNanhuaEPCS();
	}
	else if (IM_NANHUA_EPCS_4G == m_emIM)
	{
		m_pPara = new CNanhuaEPCS4G();
	}
	else
	{
		// 无匹配
		m_emIM = IM_NANHUA_PARA;
		m_pPara = new CNanhuaPara();
	}
}

void CParaThread::UninitializeInstrument(void)
{
	if (NULL != m_pPara)
	{
		delete m_pPara;
		m_pPara = NULL;
	}
}

DWORD CParaThread::Open(const BYTE bPort, const enum InstrumentModel im)
{
	m_bComPort = bPort;

	InitializeInstrument(im);
	m_dwStatus = m_pPara->Open(bPort);

	if (IM_NANHUA_EPCS_4G==m_emIM && PARA_OPEN_SUCCEED == m_dwStatus)
	{
		char chIMEI[16];
		ZeroMemory(chIMEI, sizeof(chIMEI));

		m_pPara->GetIMEI_ICCID(chIMEI, m_chICCID);

		OpenDB();

		if (IsDBOpen())
		{
			m_bWorkerRunning = true;
			m_nWorker = 0;
			ResetEvent(m_hCloseWorker);

			// 开启工作者线程
			HANDLE hThread;
			DWORD dwThreadID;
			hThread=::CreateThread(NULL,
				0,
				WorkerThread,
				this,
				0,
				&dwThreadID);
		}
	}

	return m_dwStatus;
}

DWORD CParaThread::Open(std::wstring strIP, const int nPort, const enum InstrumentModel im)
{
	m_strServerIP = strIP;
	m_usServerPort = nPort;

	InitializeInstrument(im);
	if (m_emIM == IM_NANHUA_PARA_Socket)
	{
		return m_pPara->Open(strIP, nPort);
	}
	return PARA_OPEN_FAIL;
}

DWORD CParaThread::ReOpen(void)
{
	if (NULL != m_pPara)
	{
		delete m_pPara;
		m_pPara = NULL;
	}
	if (m_emIM == IM_NANHUA_PARA)
	{
		m_pPara = new CNanhuaPara();
		return m_pPara->Open(m_bComPort);
	}
	else if (m_emIM == IM_PARA_DIM)
	{
		m_pPara = new CDigitalThermometer();
		return m_pPara->Open(m_bComPort);
	}
	else if (m_emIM == IM_PARA_DIM_NJRJ)
	{
		m_pPara = new CDigitalThermometer_NJRJ();
		return m_pPara->Open(m_bComPort);
	}
	if (m_emIM == IM_NANHUA_EPCS)
	{
		m_pPara = new CNanhuaEPCS();
		return m_pPara->Open(m_bComPort);
	}
	if (m_emIM == IM_NANHUA_EPCS_4G)
	{
		m_pPara = new CNanhuaEPCS4G();
		return m_pPara->Open(m_bComPort);
	}
	else if (m_emIM == IM_NANHUA_PARA_Socket)
	{
		m_pPara = new CNanhuaSocketPara();
		return m_pPara->Open(m_strServerIP, m_usServerPort);
	}
	return PARA_OPEN_FAIL;
}

DWORD CParaThread::Close(void)
{
	if (m_bWorkerRunning)
	{
		m_bWorkerRunning = false;
		WaitForSingleObject(m_hCloseWorker, INFINITE);
	}

	m_dwStatus = m_pPara->Close();

	CloseDB();

	return m_dwStatus;
}

DWORD CParaThread::EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	m_dwStatus = m_pPara->EnvironmentTemperatureSlopeCalibration(pbResult, fSlope);
	return m_dwStatus;
}

DWORD CParaThread::RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	m_dwStatus = m_pPara->RelativeHumiditySlopeCalibration(pbResult, fSlope);
	return m_dwStatus;
}

DWORD CParaThread::AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	m_dwStatus = m_pPara->AirPressureSlopeCalibration(pbResult, fSlope);
	return m_dwStatus;
}

DWORD CParaThread::OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	m_dwStatus = m_pPara->OilTemperatureColdSideSlopeCalibration(pbResult, fSlope);
	return m_dwStatus;
}

DWORD CParaThread::OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	m_dwStatus = m_pPara->OilTemperatureHotSideSlopeCalibration(pbResult, fSlope);
	return m_dwStatus;
}

DWORD CParaThread::EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	m_dwStatus = m_pPara->EnvironmentTemperatureZeroCalibration(pbResult, fZero);
	return m_dwStatus;
}

DWORD CParaThread::RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	m_dwStatus = m_pPara->RelativeHumidityZeroCalibration(pbResult, fZero);
	return m_dwStatus;
}

DWORD CParaThread::AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	m_dwStatus = m_pPara->AirPressureZeroCalibration(pbResult, fZero);
	return m_dwStatus;
}

DWORD CParaThread::OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	m_dwStatus = m_pPara->OilTemperatureColdSideZeroCalibration(pbResult, fZero);
	return m_dwStatus;
}

DWORD CParaThread::OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	m_dwStatus = m_pPara->OilTemperatureHotSideZeroCalibration(pbResult, fZero);
	return m_dwStatus;
}

DWORD CParaThread::BackupCalibrationParameters(BYTE *const pbResult)
{
	m_dwStatus = m_pPara->BackupCalibrationParameters(pbResult);
	return m_dwStatus;
}

DWORD CParaThread::RecoverCalibrationParameters(BYTE *const pbResult, const CPara::ParaType emParaType)
{
	m_dwStatus = m_pPara->RecoverCalibrationParameters(pbResult, emParaType);
	return m_dwStatus;
}

DWORD CParaThread::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	m_dwStatus = m_pPara->SetStrokeTypeToFourStroke(pbResult);
	return m_dwStatus;
}

DWORD CParaThread::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	m_dwStatus = m_pPara->SetStrokeTypeToTwoStroke(pbResult);
	return m_dwStatus;
}

DWORD CParaThread::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	m_dwStatus = m_pPara->SetFireTypeToSingleFire(pbResult);
	return m_dwStatus;
}

DWORD CParaThread::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	m_dwStatus = m_pPara->SetFireTypeToDoubleFire(pbResult);
	return m_dwStatus;
}

DWORD CParaThread::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	m_dwStatus = m_pPara->GetEntireData(pfET, pfRH, pfAP, pfOilTemp, pusRpm);
	return m_dwStatus;
}

DWORD CParaThread::GetEnvironmentParameters(float *const pfET, 
		float *const pfRH, 
		float *const pfAP)
{
	m_dwStatus = m_pPara->GetEnvironmentParameters(pfET, pfRH, pfAP);
	return m_dwStatus;
}

DWORD CParaThread::GetOilTempRpm(float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	m_dwStatus = m_pPara->GetOilTempRpm(pfOilTemp, pusRpm);
	return m_dwStatus;
}

void CParaThread::GetEntireData_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_PARA_GET_ENTIRE_DATA, 0, (LPARAM)hCallBackWnd);
}

void CParaThread::OnGetEntireData(WPARAM wParam, LPARAM lParam)
{
	m_dwStatus = m_pPara->GetEntireData(&m_fET, &m_fRH, &m_fAP, &m_fOilTemp, &m_usRpm);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_PARA_GET_ENTIRE_DATA, (WPARAM)m_dwStatus, NULL);
	}
}

void CParaThread::GetEnvironmentParameters_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_PARA_GET_ENVIRONMENT_PARAMETERS, 0, (LPARAM)hCallBackWnd);
}

void CParaThread::OnGetEnvironmentParameters(WPARAM wParam, LPARAM lParam)
{
	m_dwStatus = m_pPara->GetEnvironmentParameters(&m_fET,&m_fRH,&m_fAP);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_PARA_GET_ENVIRONMENT_PARAMETERS, (WPARAM)m_dwStatus, NULL);
	}
}

void CParaThread::GetOilTempRpm_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_PARA_GET_OILTEMP_RPM, 0, (LPARAM)hCallBackWnd);
}

void CParaThread::OnGetOilTempRpm(WPARAM wParam, LPARAM lParam)
{
	m_dwStatus = m_pPara->GetOilTempRpm(&m_fOilTemp, &m_usRpm);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_PARA_GET_OILTEMP_RPM, (WPARAM)m_dwStatus, NULL);
	}
}

bool CParaThread::DT_ASM(const TESTLOG &sTestLog, const SResultOfASM &sResultOfASM)
{
	if (IM_NANHUA_EPCS_4G==m_emIM && IsDBOpen())
	{
		CString strJson;

		strJson += L"{\"TestLog\":[{";
		strJson.AppendFormat(L"\"RunningNumber\": \"%s\",", sTestLog.wchRunningNumber);
		strJson.AppendFormat(L"\"ReportNumber\": \"%s\",", sTestLog.wchReportNumber);
		strJson.AppendFormat(L"\"BillDate\": \"%s\",", sTestLog.wchBillDate);
		strJson.AppendFormat(L"\"Biller\": \"%s\",", sTestLog.wchBiller);
		strJson.AppendFormat(L"\"Shroff\": \"%s\",", sTestLog.wchShroff);
		strJson.AppendFormat(L"\"ChargeState\": \"%s\",", sTestLog.wchChargeState);
		strJson.AppendFormat(L"\"InitialMoney\": \"%.2f\",", _wtof(sTestLog.wchInitialMoney));
		strJson.AppendFormat(L"\"ChangedMoney\": \"%.2f\",", _wtof(sTestLog.wchChangedMoney));
		strJson.AppendFormat(L"\"ReceivableMoney\": \"%.2f\",", _wtof(sTestLog.wchReceivableMoney));
		strJson.AppendFormat(L"\"ActualMoney\": \"%.2f\",", _wtof(sTestLog.wchActualMoney));
		strJson.AppendFormat(L"\"TestLocation\": \"%s\",", sTestLog.wchTestLocation);
		strJson.AppendFormat(L"\"TestState\": \"%s\",", sTestLog.wchTestState);
		strJson.AppendFormat(L"\"IsTestFinished\": \"%s\",", sTestLog.wchIsTestFinished);
		strJson.AppendFormat(L"\"NumberOfTestPeriodASM\": \"%s\",", sTestLog.wchNumberOfTestPeriodASM);
		strJson.AppendFormat(L"\"NumberOfTestPeriodVMAS\": \"%s\",", sTestLog.wchNumberOfTestPeriodVMAS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodLUGDOWN\": \"%s\",", sTestLog.wchNumberOfTestPeriodLUGDOWN);
		strJson.AppendFormat(L"\"NumberOfTestPeriodDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUNHT\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUNHT);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUYD\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUYD);
		strJson.AppendFormat(L"\"NumberOfTestPeriodAVFSU\": \"%s\",", sTestLog.wchNumberOfTestPeriodAVFSU);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMI\": \"%s\",", sTestLog.wchNumberOfTestPeriodMI);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodMDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriod\": \"%s\",", sTestLog.wchNumberOfTestPeriod);
		strJson.AppendFormat(L"\"TestPeriodUsed\": \"%s\",", sTestLog.wchTestPeriodUsed);
		strJson.AppendFormat(L"\"QuitReason\": \"%s\",", sTestLog.wchQuitReason);
		strJson.AppendFormat(L"\"TestType\": \"%s\",", sTestLog.wchTestType);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sTestLog.wchPass);
		strJson.AppendFormat(L"\"Operator\": \"%s\",", sTestLog.wchOperator);
		strJson.AppendFormat(L"\"Driver\": \"%s\",", sTestLog.wchDriver);
		strJson.AppendFormat(L"\"Assessor\": \"%s\",", sTestLog.wchAssessor);
		strJson.AppendFormat(L"\"IsRetest\": \"%s\",", sTestLog.wchIsRetest);
		//strJson.AppendFormat(L"\"EntryPCIP\": \"%s\",", sTestLog.wchEntryPCIP);
		strJson.AppendFormat(L"\"IsFuelTypeNeedTest\": \"%s\",", sTestLog.wchIsFuelTypeNeedTest);
		strJson.AppendFormat(L"\"IsFuelTypeTestFinished\": \"%s\",", sTestLog.wchIsFuelTypeTestFinished);
		strJson.AppendFormat(L"\"IsFuelTypeTestPass\": \"%s\",", sTestLog.wchIsFuelTypeTestPass);
		strJson.AppendFormat(L"\"IsFuelType2NeedTest\": \"%s\",", sTestLog.wchIsFuelType2NeedTest);
		strJson.AppendFormat(L"\"IsFuelType2TestFinished\": \"%s\",", sTestLog.wchIsFuelType2TestFinished);
		strJson.AppendFormat(L"\"IsFuelType2TestPass\": \"%s\",", sTestLog.wchIsFuelType2TestPass);
		strJson.AppendFormat(L"\"TestDate\": \"%s\",", sTestLog.wchTestDate);
		strJson.AppendFormat(L"\"TestStartTime\": \"%s\",", sTestLog.wchTestStartTime);
		strJson.AppendFormat(L"\"TestEndTime\": \"%s\",", sTestLog.wchTestEndTime);
		strJson.AppendFormat(L"\"TestCharacter\": \"%s\",", sTestLog.wchTestCharacter);
		//strJson.AppendFormat(L"\"AuditTimes\": \"%s\",", sTestLog.wchAuditTimes);
		//strJson.AppendFormat(L"\"RecentAuditTime\": \"%s\",", sTestLog.wchRecentAuditTime);
		strJson.AppendFormat(L"\"PrintTimes\": \"%s\",", sTestLog.wchPrintTimes);
		strJson.AppendFormat(L"\"RecentPrintReportTime\": \"%s\",", sTestLog.wchRecentPrintReportTime);
		//strJson.AppendFormat(L"\"PrintInvoiceTimes\": \"%s\",", sTestLog.wchPrintInvoiceTimes);
		//strJson.AppendFormat(L"\"RecentPrintInvoiceTime\": \"%s\",", sTestLog.wchRecentPrintInvoiceTime);
		//strJson.AppendFormat(L"\"PrintEPSignTimes\": \"%s\",", sTestLog.wchPrintEPSignTimes);
		//strJson.AppendFormat(L"\"RecentPrintEPSignTime\": \"%s\",", sTestLog.wchRecentPrintEPSignTime);
		//strJson.AppendFormat(L"\"PrintCertificateTimes\": \"%s\",", sTestLog.wchPrintCertificateTimes);
		//strJson.AppendFormat(L"\"RecentPrintCertificateTime\": \"%s\",", sTestLog.wchRecentPrintCertificateTime);
		//strJson.AppendFormat(L"\"IsTestValid\": \"%s\",", sTestLog.wchIsTestValid);
		strJson.AppendFormat(L"\"IsSent\": \"%s\",", sTestLog.wchIsSent);
		strJson.AppendFormat(L"\"IsSentEP\": \"%s\",", sTestLog.wchIsSentEP);
		strJson.AppendFormat(L"\"SRM\": \"%s\",", sTestLog.wchSRM);
		//strJson.AppendFormat(L"\"Harmony\": \"%s\",", sTestLog.wchHarmony);
		strJson.AppendFormat(L"\"PlateNumber\": \"%s\",", sTestLog.wchPlateNumber);
		strJson.AppendFormat(L"\"PlateType\": \"%s\",", sTestLog.wchPlateType);
		strJson.AppendFormat(L"\"RegistrationDate\": \"%s\",", sTestLog.wchRegistrationDate);
		strJson.AppendFormat(L"\"Owner\": \"%s\",", sTestLog.wchOwner);
		strJson.AppendFormat(L"\"Phone\": \"%s\",", sTestLog.wchPhone);
		strJson.AppendFormat(L"\"Address\": \"%s\",", sTestLog.wchAddress);
		strJson.AppendFormat(L"\"UseCharacter\": \"%s\",", sTestLog.wchUseCharacter);
		strJson.AppendFormat(L"\"Manufacturer\": \"%s\",", sTestLog.wchManufacturer);
		strJson.AppendFormat(L"\"Brand\": \"%s\",", sTestLog.wchBrand);
		strJson.AppendFormat(L"\"Model\": \"%s\",", sTestLog.wchModel);
		strJson.AppendFormat(L"\"ProductionDate\": \"%s\",", sTestLog.wchProductionDate);
		strJson.AppendFormat(L"\"VIN\": \"%s\",", sTestLog.wchVIN);
		strJson.AppendFormat(L"\"ChassisModel\": \"%s\",", sTestLog.wchChassisModel);
		strJson.AppendFormat(L"\"VehicleType\": \"%s\",", sTestLog.wchVehicleType);
		strJson.AppendFormat(L"\"MaximumTotalMass\": \"%s\",", sTestLog.wchMaximumTotalMass);
		strJson.AppendFormat(L"\"UnladenMass\": \"%s\",", sTestLog.wchUnladenMass);
		strJson.AppendFormat(L"\"AxleMass\": \"%s\",", sTestLog.wchAxleMass);
		strJson.AppendFormat(L"\"RatedLoadingMass\": \"%s\",", sTestLog.wchRatedLoadingMass);
		strJson.AppendFormat(L"\"RatedPassengerCapacity\": \"%s\",", sTestLog.wchRatedPassengerCapacity);
		strJson.AppendFormat(L"\"TyrePressure\": \"%.2f\",", _wtof(sTestLog.wchTyrePressure));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%s\",", sTestLog.wchTravelledDistance);
		strJson.AppendFormat(L"\"EngineManufacturer\": \"%s\",", sTestLog.wchEngineManufacturer);
		strJson.AppendFormat(L"\"EngineModel\": \"%s\",", sTestLog.wchEngineModel);
		strJson.AppendFormat(L"\"EngineNumber\": \"%s\",", sTestLog.wchEngineNumber);
		strJson.AppendFormat(L"\"EngineStroke\": \"%s\",", sTestLog.wchEngineStroke);
		strJson.AppendFormat(L"\"Displacement\": \"%.2f\",", _wtof(sTestLog.wchDisplacement));
		strJson.AppendFormat(L"\"NumberOfCylinder\": \"%s\",", sTestLog.wchNumberOfCylinder);
		strJson.AppendFormat(L"\"OilSupplyMode\": \"%s\",", sTestLog.wchOilSupplyMode);
		strJson.AppendFormat(L"\"AirIntakeMode\": \"%s\",", sTestLog.wchAirIntakeMode);
		strJson.AppendFormat(L"\"HasCatalyticConverter\": \"%s\",", sTestLog.wchHasCatalyticConverter);
		strJson.AppendFormat(L"\"FuelType\": \"%s\",", sTestLog.wchFuelType);
		strJson.AppendFormat(L"\"FuelMark\": \"%s\",", sTestLog.wchFuelMark);
		strJson.AppendFormat(L"\"DoubleFuel\": \"%s\",", sTestLog.wchDoubleFuel);
		strJson.AppendFormat(L"\"FuelType2\": \"%s\",", sTestLog.wchFuelType2);
		strJson.AppendFormat(L"\"FuelMark2\": \"%s\",", sTestLog.wchFuelMark2);
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sTestLog.wchRatedRev);
		strJson.AppendFormat(L"\"RatedPower\": \"%.2f\",", _wtof(sTestLog.wchRatedPower));
		strJson.AppendFormat(L"\"MaximumNetPower\": \"%.2f\",", _wtof(sTestLog.wchMaximumNetPower));
		strJson.AppendFormat(L"\"GearBoxType\": \"%s\",", sTestLog.wchGearBoxType);
		strJson.AppendFormat(L"\"NumberOfGears\": \"%s\",", sTestLog.wchNumberOfGears);
		strJson.AppendFormat(L"\"DriveType\": \"%s\",", sTestLog.wchDriveType);
		strJson.AppendFormat(L"\"EPSign\": \"%s\",", sTestLog.wchEPSign);
		strJson.AppendFormat(L"\"CertificateNumber\": \"%s\",", sTestLog.wchCertificateNumber);
		strJson.AppendFormat(L"\"IndependentDoubleExhaustPipe\": \"%s\",", sTestLog.wchIndependentDoubleExhaustPipe);
		strJson.AppendFormat(L"\"TCS\": \"%s\",", sTestLog.wchTCS);
		strJson.AppendFormat(L"\"FuelPumpMode\": \"%s\",", sTestLog.wchFuelPumpMode);
		strJson.AppendFormat(L"\"PassengerVehicle\": \"%s\",", sTestLog.wchPassengerVehicle);
		strJson.AppendFormat(L"\"EmissionStandard\": \"%s\",", sTestLog.wchEmissionStandard);
		strJson.AppendFormat(L"\"AnnualTestPeriod\": \"%s\",", sTestLog.wchAnnualTestPeriod);
		strJson.AppendFormat(L"\"HasOBD\": \"%s\",", sTestLog.wchHasOBD);
		strJson.AppendFormat(L"\"OwnerCharacter\": \"%s\",", sTestLog.wchOwnerCharacter);
		strJson.AppendFormat(L"\"OwnerDistrict\": \"%s\",", sTestLog.wchOwnerDistrict);
		strJson.AppendFormat(L"\"LicenseDepartment\": \"%s\",", sTestLog.wchLicenseDepartment);
		strJson.AppendFormat(L"\"LicenseIssueDate\": \"%s\",", sTestLog.wchLicenseIssueDate);
		strJson.AppendFormat(L"\"LicenseValidDate\": \"%s\"", sTestLog.wchLicenseValidDate);

		strJson.AppendFormat(L"\"LineNumber\": \"%s\"", sTestLog.wchLineNumber);
		strJson.AppendFormat(L"\"LineModel\": \"%s\"", sTestLog.wchLineModel);
		strJson.AppendFormat(L"\"LineName\": \"%s\"", sTestLog.wchLineName);
		strJson.AppendFormat(L"\"LineManufacturer\": \"%s\"", sTestLog.wchLineManufacturer);
		strJson.AppendFormat(L"\"LineLicenseSn\": \"%s\"", sTestLog.wchLineLicenseSn);
		strJson.AppendFormat(L"\"LineLicenseStartDate\": \"%s\"", sTestLog.wchLineLicenseStartDate);
		strJson.AppendFormat(L"\"LineLicenseEndDate\": \"%s\"", sTestLog.wchLineLicenseEndDate);
		strJson.AppendFormat(L"\"LineIsLocked\": \"%s\"", sTestLog.wchLineIsLocked);
		strJson.AppendFormat(L"\"LineRegistrationInfo\": \"%s\"", sTestLog.wchLineRegistrationInfo);
		strJson.AppendFormat(L"\"LineIPAddress\": \"%s\"", sTestLog.wchLineIPAddress);
		strJson.AppendFormat(L"\"LineHardwareSN\": \"%s\"", sTestLog.wchLineHardwareSN);
		strJson.AppendFormat(L"\"HasDynamometer\": \"%s\"", sTestLog.wchHasDynamometer);
		strJson.AppendFormat(L"\"DynamometerNumber\": \"%s\"", sTestLog.wchDynamometerNumber);
		strJson.AppendFormat(L"\"DynamometerModel\": \"%s\"", sTestLog.wchDynamometerModel);
		strJson.AppendFormat(L"\"DynamometerManufacturer\": \"%s\"", sTestLog.wchDynamometerManufacturer);
		strJson.AppendFormat(L"\"DynamometerCoastDownDate\": \"%s\"", sTestLog.wchDynamometerCoastDownDate);
		strJson.AppendFormat(L"\"DynamometerPLHPDate\": \"%s\"", sTestLog.wchDynamometerPLHPDate);
		strJson.AppendFormat(L"\"DynamometerForceCalibrationDate\": \"%s\"", sTestLog.wchDynamometerForceCalibrationDate);
		strJson.AppendFormat(L"\"DynamometerForceCheckDate\": \"%s\"", sTestLog.wchDynamometerForceCheckDate);
		strJson.AppendFormat(L"\"DynamometerAuthenticationDate\": \"%s\"", sTestLog.wchDynamometerAuthenticationDate);
		strJson.AppendFormat(L"\"HasAnalyzer\": \"%s\"", sTestLog.wchHasAnalyzer);
		strJson.AppendFormat(L"\"AnalyzerNumber\": \"%s\"", sTestLog.wchAnalyzerNumber);
		strJson.AppendFormat(L"\"AnalyzerModel\": \"%s\"", sTestLog.wchAnalyzerModel);
		strJson.AppendFormat(L"\"AnalyzerManufacturer\": \"%s\"", sTestLog.wchAnalyzerManufacturer);
		strJson.AppendFormat(L"\"AnalyzerChannelCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerChannelCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerChannelCheckDate\": \"%s\"", sTestLog.wchAnalyzerChannelCheckDate);
		strJson.AppendFormat(L"\"AnalyzerEnvParamCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerAuthenticationDate\": \"%s\"", sTestLog.wchAnalyzerAuthenticationDate);
		strJson.AppendFormat(L"\"HasFlowmeter\": \"%s\"", sTestLog.wchHasFlowmeter);
		strJson.AppendFormat(L"\"FlowmeterNumber\": \"%s\"", sTestLog.wchFlowmeterNumber);
		strJson.AppendFormat(L"\"FlowmeterModel\": \"%s\"", sTestLog.wchFlowmeterModel);
		strJson.AppendFormat(L"\"FlowmeterManufacturer\": \"%s\"", sTestLog.wchFlowmeterManufacturer);
		strJson.AppendFormat(L"\"FlowmeterFluxCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterFluxCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterFluxCheckDate\": \"%s\"", sTestLog.wchFlowmeterFluxCheckDate);
		strJson.AppendFormat(L"\"FlowmeterDilO2CalibrationDate\": \"%s\"", sTestLog.wchFlowmeterDilO2CalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterPressureCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterPressureCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterAuthenticationDate\": \"%s\"", sTestLog.wchFlowmeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasOpacimeter\": \"%s\"", sTestLog.wchHasOpacimeter);
		strJson.AppendFormat(L"\"OpacimeterNumber\": \"%s\"", sTestLog.wchOpacimeterNumber);
		strJson.AppendFormat(L"\"OpacimeterModel\": \"%s\"", sTestLog.wchOpacimeterModel);
		strJson.AppendFormat(L"\"OpacimeterManufacturer\": \"%s\"", sTestLog.wchOpacimeterManufacturer);
		strJson.AppendFormat(L"\"OpacimeterChannelCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterChannelCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterChannelCheckDate\": \"%s\"", sTestLog.wchOpacimeterChannelCheckDate);
		strJson.AppendFormat(L"\"OpacimeterEnvParamCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterAuthenticationDate\": \"%s\"", sTestLog.wchOpacimeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasSmokemeter\": \"%s\"", sTestLog.wchHasSmokemeter);
		strJson.AppendFormat(L"\"SmokemeterNumber\": \"%s\"", sTestLog.wchSmokemeterNumber);
		strJson.AppendFormat(L"\"SmokemeterModel\": \"%s\"", sTestLog.wchSmokemeterModel);
		strJson.AppendFormat(L"\"SmokemeterManufacturer\": \"%s\"", sTestLog.wchSmokemeterManufacturer);
		strJson.AppendFormat(L"\"SmokemeterCalibrationDate\": \"%s\"", sTestLog.wchSmokemeterCalibrationDate);
		strJson.AppendFormat(L"\"SmokemeterCheckDate\": \"%s\"", sTestLog.wchSmokemeterCheckDate);
		strJson.AppendFormat(L"\"SmokemeterAuthenticationDate\": \"%s\"", sTestLog.wchSmokemeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentEnvParamMeasurer\": \"%s\"", sTestLog.wchHasIndependentEnvParamMeasurer);
		strJson.AppendFormat(L"\"EnvParamMeasurerNumber\": \"%s\"", sTestLog.wchEnvParamMeasurerNumber);
		strJson.AppendFormat(L"\"EnvParamMeasurerModel\": \"%s\"", sTestLog.wchEnvParamMeasurerModel);
		strJson.AppendFormat(L"\"EnvParamMeasurerManufacturer\": \"%s\"", sTestLog.wchEnvParamMeasurerManufacturer);
		strJson.AppendFormat(L"\"EnvParamMeasurerCalibrationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCalibrationDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerCheckDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCheckDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerAuthenticationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentTachometer\": \"%s\"", sTestLog.wchHasIndependentTachometer);
		strJson.AppendFormat(L"\"TachometerNumber\": \"%s\"", sTestLog.wchTachometerNumber);
		strJson.AppendFormat(L"\"TachometerModel\": \"%s\"", sTestLog.wchTachometerModel);
		strJson.AppendFormat(L"\"TachometerManufacturer\": \"%s\"", sTestLog.wchTachometerManufacturer);
		strJson.AppendFormat(L"\"TachometerCalibrationDate\": \"%s\"", sTestLog.wchTachometerCalibrationDate);
		strJson.AppendFormat(L"\"TachometerCheckDate\": \"%s\"", sTestLog.wchTachometerCheckDate);
		strJson.AppendFormat(L"\"TachometerAuthenticationDate\": \"%s\"", sTestLog.wchTachometerAuthenticationDate);
		strJson.AppendFormat(L"\"StationNumber\": \"%s\"", sTestLog.wchStationNumber);
		strJson.AppendFormat(L"\"StationFullName\": \"%s\"", sTestLog.wchStationFullName);
		strJson.AppendFormat(L"\"StationShortName\": \"%s\"", sTestLog.wchStationShortName);
		strJson.AppendFormat(L"\"StationLicenseSn\": \"%s\"", sTestLog.wchStationLicenseSn);
		strJson.AppendFormat(L"\"StationLicenseStartDate\": \"%s\"", sTestLog.wchStationLicenseStartDate);
		strJson.AppendFormat(L"\"StationLicenseEndDate\": \"%s\"", sTestLog.wchStationLicenseEndDate);
		strJson.AppendFormat(L"\"StationIsLocked\": \"%s\"", sTestLog.wchStationIsLocked);
		strJson.AppendFormat(L"\"StationRegistrationInfo\": \"%s\"", sTestLog.wchStationRegistrationInfo);
		strJson.AppendFormat(L"\"StationPhone\": \"%s\"", sTestLog.wchStationPhone);
		strJson.AppendFormat(L"\"StationFax\": \"%s\"", sTestLog.wchStationFax);
		strJson.AppendFormat(L"\"PostCode\": \"%s\"", sTestLog.wchPostCode);
		strJson.AppendFormat(L"\"Province\": \"%s\"", sTestLog.wchProvince);
		strJson.AppendFormat(L"\"City\": \"%s\"", sTestLog.wchCity);
		strJson.AppendFormat(L"\"District\": \"%s\"", sTestLog.wchDistrict);
		strJson.AppendFormat(L"\"DetailedAddress\": \"%s\"", sTestLog.wchDetailedAddress);

		strJson += L"}],";

		strJson += L"\"ResultOfASM\":[{";
		strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", sResultOfASM.strFuelTypeCode.c_str());
		strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", sResultOfASM.strFuelTypeName.c_str());
		strJson.AppendFormat(L"\"LimitOfHC5025\": \"%s\",", sResultOfASM.strLimitOfHC5025.c_str());
		strJson.AppendFormat(L"\"LimitOfHC5025_EDP\": \"%s\",", sResultOfASM.strLimitOfHC5025_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfCO5025\": \"%.2f\",", _wtof(sResultOfASM.strLimitOfCO5025.c_str()));
		strJson.AppendFormat(L"\"LimitOfCO5025_EDP\": \"%s\",", sResultOfASM.strLimitOfCO5025_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfNO5025\": \"%s\",", sResultOfASM.strLimitOfNO5025.c_str());
		strJson.AppendFormat(L"\"LimitOfNO5025_EDP\": \"%s\",", sResultOfASM.strLimitOfNO5025_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfHC2540\": \"%s\",", sResultOfASM.strLimitOfHC2540.c_str());
		strJson.AppendFormat(L"\"LimitOfHC2540_EDP\": \"%s\",", sResultOfASM.strLimitOfHC2540_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfCO2540\": \"%.2f\",", _wtof(sResultOfASM.strLimitOfCO2540.c_str()));
		strJson.AppendFormat(L"\"LimitOfCO2540_EDP\": \"%s\",", sResultOfASM.strLimitOfCO2540_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfNO2540\": \"%s\",", sResultOfASM.strLimitOfNO2540.c_str());
		strJson.AppendFormat(L"\"LimitOfNO2540_EDP\": \"%s\",", sResultOfASM.strLimitOfNO2540_EDP.c_str());
		strJson.AppendFormat(L"\"Rev5025\": \"%s\",", sResultOfASM.strRev5025.c_str());
		strJson.AppendFormat(L"\"OilTemperature5025\": \"%s\",", sResultOfASM.strOilTemperature5025.c_str());
		strJson.AppendFormat(L"\"Lambda5025\": \"%s\",", sResultOfASM.strLambda5025.c_str());
		strJson.AppendFormat(L"\"Power5025\": \"%s\",", sResultOfASM.strPower5025.c_str());
		strJson.AppendFormat(L"\"HC5025\": \"%s\",", sResultOfASM.strHC5025.c_str());
		strJson.AppendFormat(L"\"CO5025\": \"%s\",", sResultOfASM.strCO5025.c_str());
		strJson.AppendFormat(L"\"NO5025\": \"%s\",", sResultOfASM.strNO5025.c_str());
		strJson.AppendFormat(L"\"CO25025\": \"%s\",", sResultOfASM.strCO25025.c_str());
		strJson.AppendFormat(L"\"O25025\": \"%s\",", sResultOfASM.strO25025.c_str());
		strJson.AppendFormat(L"\"PassOfHC5025\": \"%s\",", sResultOfASM.strPassOfHC5025.c_str());
		strJson.AppendFormat(L"\"PassOfCO5025\": \"%s\",", sResultOfASM.strPassOfCO5025.c_str());
		strJson.AppendFormat(L"\"PassOfNO5025\": \"%s\",", sResultOfASM.strPassOfNO5025.c_str());
		strJson.AppendFormat(L"\"NeedTestMode2540\": \"%s\",", sResultOfASM.strNeedTestMode2540.c_str());
		strJson.AppendFormat(L"\"Rev2540\": \"%s\",", sResultOfASM.strRev2540.c_str());
		strJson.AppendFormat(L"\"OilTemperature2540\": \"%.2f\",", _wtof(sResultOfASM.strOilTemperature2540.c_str()));
		strJson.AppendFormat(L"\"Lambda2540\": \"%.2f\",", _wtof(sResultOfASM.strLambda2540.c_str()));
		strJson.AppendFormat(L"\"Power2540\": \"%.2f\",", _wtof(sResultOfASM.strPower2540.c_str()));
		strJson.AppendFormat(L"\"HC2540\": \"%s\",", sResultOfASM.strHC2540.c_str());
		strJson.AppendFormat(L"\"CO2540\": \"%.2f\",", _wtof(sResultOfASM.strCO2540.c_str()));
		strJson.AppendFormat(L"\"NO2540\": \"%s\",", sResultOfASM.strNO2540.c_str());
		strJson.AppendFormat(L"\"CO22540\": \"%.2f\",", _wtof(sResultOfASM.strCO22540.c_str()));
		strJson.AppendFormat(L"\"O22540\": \"%.2f\",", _wtof(sResultOfASM.strO22540.c_str()));
		strJson.AppendFormat(L"\"PassOfHC2540\": \"%s\",", sResultOfASM.strPassOfHC2540.c_str());
		strJson.AppendFormat(L"\"PassOfCO2540\": \"%s\",", sResultOfASM.strPassOfCO2540.c_str());
		strJson.AppendFormat(L"\"PassOfNO2540\": \"%s\",", sResultOfASM.strPassOfNO2540.c_str());
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sResultOfASM.strPass.c_str());
		strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(sResultOfASM.strEnvironmentalTemperature.c_str()));
		strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(sResultOfASM.strRelativeHumidity.c_str()));
		strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(sResultOfASM.strAtmosphericPressure.c_str()));
		strJson.AppendFormat(L"\"SkipRev\": \"%s\",", sResultOfASM.strSkipRev.c_str());
		strJson.AppendFormat(L"\"SkipOilTemperature\": \"%s\"", sResultOfASM.strSkipOilTemperature.c_str());
		strJson += L"}],";

		SResultOfASM sResultOfASMTemp;
		sResultOfASMTemp = sResultOfASM;
		std::vector<SRealTimeDataOfASM> vtRTData;
		vtRTData = sResultOfASMTemp.RTDataUpZip();

		strJson += L"\"RealTimeDataOfASM\":[";

		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strJson += L"{";
			strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", vtRTData[i].strFuelTypeCode.c_str());
			strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", vtRTData[i].strFuelTypeName.c_str());
			strJson.AppendFormat(L"\"Time\": \"%s\",", vtRTData[i].strTime.c_str());
			strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", vtRTData[i].strSamplingTime.c_str());
			strJson.AppendFormat(L"\"Velocity\": \"%.2f\",", _wtof(vtRTData[i].strVelocity.c_str()));
			strJson.AppendFormat(L"\"Power\": \"%.2f\",", _wtof(vtRTData[i].strPower.c_str()));
			strJson.AppendFormat(L"\"Force\": \"%.2f\",", _wtof(vtRTData[i].strForce.c_str()));
			strJson.AppendFormat(L"\"IHP\": \"%.2f\",", _wtof(vtRTData[i].strIHP.c_str()));
			strJson.AppendFormat(L"\"PLHP\": \"%.2f\",", _wtof(vtRTData[i].strPLHP.c_str()));
			strJson.AppendFormat(L"\"State\": \"%s\",", vtRTData[i].strState.c_str());
			strJson.AppendFormat(L"\"HC\": \"%s\",", vtRTData[i].strHC.c_str());
			strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(vtRTData[i].strCO.c_str()));
			strJson.AppendFormat(L"\"NO\": \"%s\",", vtRTData[i].strNO.c_str());
			strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(vtRTData[i].strO2.c_str()));
			strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(vtRTData[i].strCO2.c_str()));
			strJson.AppendFormat(L"\"EngineRev\": \"%s\",", vtRTData[i].strEngineRev.c_str());
			strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(vtRTData[i].strOilTemperature.c_str()));
			strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(vtRTData[i].strLambda.c_str()));
			strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
			strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(vtRTData[i].strRelativeHumidity.c_str()));
			strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(vtRTData[i].strAtmosphericPressure.c_str()));
			strJson.AppendFormat(L"\"HumidityCorrectionFactor\": \"%.2f\",", _wtof(vtRTData[i].strHumidityCorrectionFactor.c_str()));
			strJson.AppendFormat(L"\"DilutionCorrectionFactor\": \"%.2f\",", _wtof(vtRTData[i].strDilutionCorrectionFactor.c_str()));
			strJson.AppendFormat(L"\"IsAdoptedResult\": \"%s\"", vtRTData[i].strIsAdoptedResult.c_str());

			//OBD数据
			strJson.AppendFormat(L"\"OBDRTData_EngineRev\": \"%s\",", vtRTData[i].sOBDRTData.strEngineRev.c_str());
			strJson.AppendFormat(L"\"OBDRTData_Velocity\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strVelocity.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CoolantTemp\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCoolantTemp.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_SolarTermDoorPosition\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ThrottleOpening\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strThrottleOpening.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CalculationLoad\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCalculationLoad.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorV\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorV.c_str()));			
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorI\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorI.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_AirCoefficient\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strAirCoefficient.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAF\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAF.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAP\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAP.c_str()));
			//以下检测没保存
			strJson.AppendFormat(L"\"OBDRTData_EngineOutputPower\": \"%s\",", vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());
			strJson.AppendFormat(L"\"OBDRTData_ChargeAirPressure\": \"%s\",", vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());
			strJson.AppendFormat(L"\"OBDRTData_FuelConsumption\": \"%s\",", vtRTData[i].sOBDRTData.strFuelConsumption.c_str());
			strJson.AppendFormat(L"\"OBDRTData_NOXConcentration\": \"%s\",", vtRTData[i].sOBDRTData.strNOXConcentration.c_str());
			strJson.AppendFormat(L"\"OBDRTData_UreaInjectionVolume\": \"%s\",", vtRTData[i].sOBDRTData.strUreaInjectionVolume.c_str());
			strJson.AppendFormat(L"\"OBDRTData_EGT\": \"%s\",", vtRTData[i].sOBDRTData.strEGT.c_str());
			strJson.AppendFormat(L"\"OBDRTData_DPFDifferentialPressure\": \"%s\",", vtRTData[i].sOBDRTData.strDPFDifferentialPressure.c_str());
			strJson.AppendFormat(L"\"OBDRTData_EGRPosition\": \"%s\",", vtRTData[i].sOBDRTData.strEGRPosition.c_str());
			strJson.AppendFormat(L"\"OBDRTData_FuelDeliveryPressure\": \"%s\",", vtRTData[i].sOBDRTData.strFuelDeliveryPressure.c_str());
			strJson += L"},";
		}
		// 删除最后一个"，"
		if(vtRTData.size()>0)
		{
			strJson.Delete(strJson.GetLength()-1);
		}

		//for (list<REALTIMEDATAOFASM>::iterator ir=listRealTimeDataOfASM.begin(); ir!=listRealTimeDataOfASM.end(); ir++)
		//{
		//	strJson += L"{";
		//	strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", ir->wchFuelTypeCode);
		//	strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", ir->wchFuelTypeName);
		//	strJson.AppendFormat(L"\"Time\": \"%s\",", ir->wchTime);
		//	strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", ir->wchSamplingTime);
		//	strJson.AppendFormat(L"\"Velocity\": \"%.2f\",", _wtof(ir->wchVelocity));
		//	strJson.AppendFormat(L"\"Power\": \"%.2f\",", _wtof(ir->wchPower));
		//	strJson.AppendFormat(L"\"Force\": \"%.2f\",", _wtof(ir->wchForce));
		//	strJson.AppendFormat(L"\"IHP\": \"%.2f\",", _wtof(ir->wchIHP));
		//	strJson.AppendFormat(L"\"PLHP\": \"%.2f\",", _wtof(ir->wchPLHP));
		//	strJson.AppendFormat(L"\"State\": \"%s\",", ir->wchState);
		//	strJson.AppendFormat(L"\"HC\": \"%s\",", ir->wchHC);
		//	strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(ir->wchCO));
		//	strJson.AppendFormat(L"\"NO\": \"%s\",", ir->wchNO);
		//	strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(ir->wchO2));
		//	strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(ir->wchCO2));
		//	strJson.AppendFormat(L"\"EngineRev\": \"%s\",", ir->wchEngineRev);
		//	strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(ir->wchOilTemperature));
		//	strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(ir->wchLambda));
		//	strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(ir->wchEnvironmentalTemperature));
		//	strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(ir->wchRelativeHumidity));
		//	strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(ir->wchAtmosphericPressure));
		//	strJson.AppendFormat(L"\"HumidityCorrectionFactor\": \"%.2f\",", _wtof(ir->wchHumidityCorrectionFactor));
		//	strJson.AppendFormat(L"\"DilutionCorrectionFactor\": \"%.2f\",", _wtof(ir->wchDilutionCorrectionFactor));
		//	strJson.AppendFormat(L"\"IsAdoptedResult\": \"%s\"", ir->wchIsAdoptedResult);
		//	strJson += L"},";
		//}
		//// 删除最后一个"，"
		//if(listRealTimeDataOfASM.size()>0)
		//{
		//	strJson.Delete(strJson.GetLength()-1);
		//}
		strJson += L"]}";

		char *pch(NULL);
		pch = CNHCommonAPI::UnicodeToANSI(strJson);

		unsigned long nLen = strlen(pch);

		BYTE *szBuf = new BYTE[nLen]();
		int nRet = compress(szBuf, &nLen, (BYTE*)pch, nLen);

		ZBase64 zBase64;
		std::string strBufBase64 = zBase64.EncodeRaw(szBuf, nLen);

		DataTrans((BYTE*)strBufBase64.c_str(), strBufBase64.length());

		if(szBuf != NULL)
		{
			delete[] szBuf;
		}

		if (NULL != pch)
		{
			free(pch);
			pch = NULL;
		}
	}

	return true;
}

bool CParaThread::DT_VMAS(const TESTLOG &sTestLog, const SResultOfVMAS &sResultOfVMAS)
{
	if (IM_NANHUA_EPCS_4G==m_emIM && IsDBOpen())
	{
		CString strJson;

		strJson += L"{\"TestLog\":[{";
		strJson.AppendFormat(L"\"RunningNumber\": \"%s\",", sTestLog.wchRunningNumber);
		strJson.AppendFormat(L"\"ReportNumber\": \"%s\",", sTestLog.wchReportNumber);
		strJson.AppendFormat(L"\"BillDate\": \"%s\",", sTestLog.wchBillDate);
		strJson.AppendFormat(L"\"Biller\": \"%s\",", sTestLog.wchBiller);
		strJson.AppendFormat(L"\"Shroff\": \"%s\",", sTestLog.wchShroff);
		strJson.AppendFormat(L"\"ChargeState\": \"%s\",", sTestLog.wchChargeState);
		strJson.AppendFormat(L"\"InitialMoney\": \"%.2f\",", _wtof(sTestLog.wchInitialMoney));
		strJson.AppendFormat(L"\"ChangedMoney\": \"%.2f\",", _wtof(sTestLog.wchChangedMoney));
		strJson.AppendFormat(L"\"ReceivableMoney\": \"%.2f\",", _wtof(sTestLog.wchReceivableMoney));
		strJson.AppendFormat(L"\"ActualMoney\": \"%.2f\",", _wtof(sTestLog.wchActualMoney));
		strJson.AppendFormat(L"\"TestLocation\": \"%s\",", sTestLog.wchTestLocation);
		strJson.AppendFormat(L"\"TestState\": \"%s\",", sTestLog.wchTestState);
		strJson.AppendFormat(L"\"IsTestFinished\": \"%s\",", sTestLog.wchIsTestFinished);
		strJson.AppendFormat(L"\"NumberOfTestPeriodASM\": \"%s\",", sTestLog.wchNumberOfTestPeriodASM);
		strJson.AppendFormat(L"\"NumberOfTestPeriodVMAS\": \"%s\",", sTestLog.wchNumberOfTestPeriodVMAS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodLUGDOWN\": \"%s\",", sTestLog.wchNumberOfTestPeriodLUGDOWN);
		strJson.AppendFormat(L"\"NumberOfTestPeriodDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUNHT\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUNHT);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUYD\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUYD);
		strJson.AppendFormat(L"\"NumberOfTestPeriodAVFSU\": \"%s\",", sTestLog.wchNumberOfTestPeriodAVFSU);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMI\": \"%s\",", sTestLog.wchNumberOfTestPeriodMI);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodMDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriod\": \"%s\",", sTestLog.wchNumberOfTestPeriod);
		strJson.AppendFormat(L"\"TestPeriodUsed\": \"%s\",", sTestLog.wchTestPeriodUsed);
		strJson.AppendFormat(L"\"QuitReason\": \"%s\",", sTestLog.wchQuitReason);
		strJson.AppendFormat(L"\"TestType\": \"%s\",", sTestLog.wchTestType);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sTestLog.wchPass);
		strJson.AppendFormat(L"\"Operator\": \"%s\",", sTestLog.wchOperator);
		strJson.AppendFormat(L"\"Driver\": \"%s\",", sTestLog.wchDriver);
		strJson.AppendFormat(L"\"Assessor\": \"%s\",", sTestLog.wchAssessor);
		strJson.AppendFormat(L"\"IsRetest\": \"%s\",", sTestLog.wchIsRetest);
		//strJson.AppendFormat(L"\"EntryPCIP\": \"%s\",", sTestLog.wchEntryPCIP);
		strJson.AppendFormat(L"\"IsFuelTypeNeedTest\": \"%s\",", sTestLog.wchIsFuelTypeNeedTest);
		strJson.AppendFormat(L"\"IsFuelTypeTestFinished\": \"%s\",", sTestLog.wchIsFuelTypeTestFinished);
		strJson.AppendFormat(L"\"IsFuelTypeTestPass\": \"%s\",", sTestLog.wchIsFuelTypeTestPass);
		strJson.AppendFormat(L"\"IsFuelType2NeedTest\": \"%s\",", sTestLog.wchIsFuelType2NeedTest);
		strJson.AppendFormat(L"\"IsFuelType2TestFinished\": \"%s\",", sTestLog.wchIsFuelType2TestFinished);
		strJson.AppendFormat(L"\"IsFuelType2TestPass\": \"%s\",", sTestLog.wchIsFuelType2TestPass);
		strJson.AppendFormat(L"\"TestDate\": \"%s\",", sTestLog.wchTestDate);
		strJson.AppendFormat(L"\"TestStartTime\": \"%s\",", sTestLog.wchTestStartTime);
		strJson.AppendFormat(L"\"TestEndTime\": \"%s\",", sTestLog.wchTestEndTime);
		strJson.AppendFormat(L"\"TestCharacter\": \"%s\",", sTestLog.wchTestCharacter);
		//strJson.AppendFormat(L"\"AuditTimes\": \"%s\",", sTestLog.wchAuditTimes);
		//strJson.AppendFormat(L"\"RecentAuditTime\": \"%s\",", sTestLog.wchRecentAuditTime);
		strJson.AppendFormat(L"\"PrintTimes\": \"%s\",", sTestLog.wchPrintTimes);
		strJson.AppendFormat(L"\"RecentPrintReportTime\": \"%s\",", sTestLog.wchRecentPrintReportTime);
		//strJson.AppendFormat(L"\"PrintInvoiceTimes\": \"%s\",", sTestLog.wchPrintInvoiceTimes);
		//strJson.AppendFormat(L"\"RecentPrintInvoiceTime\": \"%s\",", sTestLog.wchRecentPrintInvoiceTime);
		//strJson.AppendFormat(L"\"PrintEPSignTimes\": \"%s\",", sTestLog.wchPrintEPSignTimes);
		//strJson.AppendFormat(L"\"RecentPrintEPSignTime\": \"%s\",", sTestLog.wchRecentPrintEPSignTime);
		//strJson.AppendFormat(L"\"PrintCertificateTimes\": \"%s\",", sTestLog.wchPrintCertificateTimes);
		//strJson.AppendFormat(L"\"RecentPrintCertificateTime\": \"%s\",", sTestLog.wchRecentPrintCertificateTime);
		//strJson.AppendFormat(L"\"IsTestValid\": \"%s\",", sTestLog.wchIsTestValid);
		strJson.AppendFormat(L"\"IsSent\": \"%s\",", sTestLog.wchIsSent);
		strJson.AppendFormat(L"\"IsSentEP\": \"%s\",", sTestLog.wchIsSentEP);
		strJson.AppendFormat(L"\"SRM\": \"%s\",", sTestLog.wchSRM);
		//strJson.AppendFormat(L"\"Harmony\": \"%s\",", sTestLog.wchHarmony);
		strJson.AppendFormat(L"\"PlateNumber\": \"%s\",", sTestLog.wchPlateNumber);
		strJson.AppendFormat(L"\"PlateType\": \"%s\",", sTestLog.wchPlateType);
		strJson.AppendFormat(L"\"RegistrationDate\": \"%s\",", sTestLog.wchRegistrationDate);
		strJson.AppendFormat(L"\"Owner\": \"%s\",", sTestLog.wchOwner);
		strJson.AppendFormat(L"\"Phone\": \"%s\",", sTestLog.wchPhone);
		strJson.AppendFormat(L"\"Address\": \"%s\",", sTestLog.wchAddress);
		strJson.AppendFormat(L"\"UseCharacter\": \"%s\",", sTestLog.wchUseCharacter);
		strJson.AppendFormat(L"\"Manufacturer\": \"%s\",", sTestLog.wchManufacturer);
		strJson.AppendFormat(L"\"Brand\": \"%s\",", sTestLog.wchBrand);
		strJson.AppendFormat(L"\"Model\": \"%s\",", sTestLog.wchModel);
		strJson.AppendFormat(L"\"ProductionDate\": \"%s\",", sTestLog.wchProductionDate);
		strJson.AppendFormat(L"\"VIN\": \"%s\",", sTestLog.wchVIN);
		strJson.AppendFormat(L"\"ChassisModel\": \"%s\",", sTestLog.wchChassisModel);
		strJson.AppendFormat(L"\"VehicleType\": \"%s\",", sTestLog.wchVehicleType);
		strJson.AppendFormat(L"\"MaximumTotalMass\": \"%s\",", sTestLog.wchMaximumTotalMass);
		strJson.AppendFormat(L"\"UnladenMass\": \"%s\",", sTestLog.wchUnladenMass);
		strJson.AppendFormat(L"\"AxleMass\": \"%s\",", sTestLog.wchAxleMass);
		strJson.AppendFormat(L"\"RatedLoadingMass\": \"%s\",", sTestLog.wchRatedLoadingMass);
		strJson.AppendFormat(L"\"RatedPassengerCapacity\": \"%s\",", sTestLog.wchRatedPassengerCapacity);
		strJson.AppendFormat(L"\"TyrePressure\": \"%.2f\",", _wtof(sTestLog.wchTyrePressure));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%s\",", sTestLog.wchTravelledDistance);
		strJson.AppendFormat(L"\"EngineManufacturer\": \"%s\",", sTestLog.wchEngineManufacturer);
		strJson.AppendFormat(L"\"EngineModel\": \"%s\",", sTestLog.wchEngineModel);
		strJson.AppendFormat(L"\"EngineNumber\": \"%s\",", sTestLog.wchEngineNumber);
		strJson.AppendFormat(L"\"EngineStroke\": \"%s\",", sTestLog.wchEngineStroke);
		strJson.AppendFormat(L"\"Displacement\": \"%.2f\",", _wtof(sTestLog.wchDisplacement));
		strJson.AppendFormat(L"\"NumberOfCylinder\": \"%s\",", sTestLog.wchNumberOfCylinder);
		strJson.AppendFormat(L"\"OilSupplyMode\": \"%s\",", sTestLog.wchOilSupplyMode);
		strJson.AppendFormat(L"\"AirIntakeMode\": \"%s\",", sTestLog.wchAirIntakeMode);
		strJson.AppendFormat(L"\"HasCatalyticConverter\": \"%s\",", sTestLog.wchHasCatalyticConverter);
		strJson.AppendFormat(L"\"FuelType\": \"%s\",", sTestLog.wchFuelType);
		strJson.AppendFormat(L"\"FuelMark\": \"%s\",", sTestLog.wchFuelMark);
		strJson.AppendFormat(L"\"DoubleFuel\": \"%s\",", sTestLog.wchDoubleFuel);
		strJson.AppendFormat(L"\"FuelType2\": \"%s\",", sTestLog.wchFuelType2);
		strJson.AppendFormat(L"\"FuelMark2\": \"%s\",", sTestLog.wchFuelMark2);
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sTestLog.wchRatedRev);
		strJson.AppendFormat(L"\"RatedPower\": \"%.2f\",", _wtof(sTestLog.wchRatedPower));
		strJson.AppendFormat(L"\"MaximumNetPower\": \"%.2f\",", _wtof(sTestLog.wchMaximumNetPower));
		strJson.AppendFormat(L"\"GearBoxType\": \"%s\",", sTestLog.wchGearBoxType);
		strJson.AppendFormat(L"\"NumberOfGears\": \"%s\",", sTestLog.wchNumberOfGears);
		strJson.AppendFormat(L"\"DriveType\": \"%s\",", sTestLog.wchDriveType);
		strJson.AppendFormat(L"\"EPSign\": \"%s\",", sTestLog.wchEPSign);
		strJson.AppendFormat(L"\"CertificateNumber\": \"%s\",", sTestLog.wchCertificateNumber);
		strJson.AppendFormat(L"\"IndependentDoubleExhaustPipe\": \"%s\",", sTestLog.wchIndependentDoubleExhaustPipe);
		strJson.AppendFormat(L"\"TCS\": \"%s\",", sTestLog.wchTCS);
		strJson.AppendFormat(L"\"FuelPumpMode\": \"%s\",", sTestLog.wchFuelPumpMode);
		strJson.AppendFormat(L"\"PassengerVehicle\": \"%s\",", sTestLog.wchPassengerVehicle);
		strJson.AppendFormat(L"\"EmissionStandard\": \"%s\",", sTestLog.wchEmissionStandard);
		strJson.AppendFormat(L"\"AnnualTestPeriod\": \"%s\",", sTestLog.wchAnnualTestPeriod);
		strJson.AppendFormat(L"\"HasOBD\": \"%s\",", sTestLog.wchHasOBD);
		strJson.AppendFormat(L"\"OwnerCharacter\": \"%s\",", sTestLog.wchOwnerCharacter);
		strJson.AppendFormat(L"\"OwnerDistrict\": \"%s\",", sTestLog.wchOwnerDistrict);
		strJson.AppendFormat(L"\"LicenseDepartment\": \"%s\",", sTestLog.wchLicenseDepartment);
		strJson.AppendFormat(L"\"LicenseIssueDate\": \"%s\",", sTestLog.wchLicenseIssueDate);
		strJson.AppendFormat(L"\"LicenseValidDate\": \"%s\"", sTestLog.wchLicenseValidDate);

		strJson.AppendFormat(L"\"LineNumber\": \"%s\"", sTestLog.wchLineNumber);
		strJson.AppendFormat(L"\"LineModel\": \"%s\"", sTestLog.wchLineModel);
		strJson.AppendFormat(L"\"LineName\": \"%s\"", sTestLog.wchLineName);
		strJson.AppendFormat(L"\"LineManufacturer\": \"%s\"", sTestLog.wchLineManufacturer);
		strJson.AppendFormat(L"\"LineLicenseSn\": \"%s\"", sTestLog.wchLineLicenseSn);
		strJson.AppendFormat(L"\"LineLicenseStartDate\": \"%s\"", sTestLog.wchLineLicenseStartDate);
		strJson.AppendFormat(L"\"LineLicenseEndDate\": \"%s\"", sTestLog.wchLineLicenseEndDate);
		strJson.AppendFormat(L"\"LineIsLocked\": \"%s\"", sTestLog.wchLineIsLocked);
		strJson.AppendFormat(L"\"LineRegistrationInfo\": \"%s\"", sTestLog.wchLineRegistrationInfo);
		strJson.AppendFormat(L"\"LineIPAddress\": \"%s\"", sTestLog.wchLineIPAddress);
		strJson.AppendFormat(L"\"LineHardwareSN\": \"%s\"", sTestLog.wchLineHardwareSN);
		strJson.AppendFormat(L"\"HasDynamometer\": \"%s\"", sTestLog.wchHasDynamometer);
		strJson.AppendFormat(L"\"DynamometerNumber\": \"%s\"", sTestLog.wchDynamometerNumber);
		strJson.AppendFormat(L"\"DynamometerModel\": \"%s\"", sTestLog.wchDynamometerModel);
		strJson.AppendFormat(L"\"DynamometerManufacturer\": \"%s\"", sTestLog.wchDynamometerManufacturer);
		strJson.AppendFormat(L"\"DynamometerCoastDownDate\": \"%s\"", sTestLog.wchDynamometerCoastDownDate);
		strJson.AppendFormat(L"\"DynamometerPLHPDate\": \"%s\"", sTestLog.wchDynamometerPLHPDate);
		strJson.AppendFormat(L"\"DynamometerForceCalibrationDate\": \"%s\"", sTestLog.wchDynamometerForceCalibrationDate);
		strJson.AppendFormat(L"\"DynamometerForceCheckDate\": \"%s\"", sTestLog.wchDynamometerForceCheckDate);
		strJson.AppendFormat(L"\"DynamometerAuthenticationDate\": \"%s\"", sTestLog.wchDynamometerAuthenticationDate);
		strJson.AppendFormat(L"\"HasAnalyzer\": \"%s\"", sTestLog.wchHasAnalyzer);
		strJson.AppendFormat(L"\"AnalyzerNumber\": \"%s\"", sTestLog.wchAnalyzerNumber);
		strJson.AppendFormat(L"\"AnalyzerModel\": \"%s\"", sTestLog.wchAnalyzerModel);
		strJson.AppendFormat(L"\"AnalyzerManufacturer\": \"%s\"", sTestLog.wchAnalyzerManufacturer);
		strJson.AppendFormat(L"\"AnalyzerChannelCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerChannelCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerChannelCheckDate\": \"%s\"", sTestLog.wchAnalyzerChannelCheckDate);
		strJson.AppendFormat(L"\"AnalyzerEnvParamCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerAuthenticationDate\": \"%s\"", sTestLog.wchAnalyzerAuthenticationDate);
		strJson.AppendFormat(L"\"HasFlowmeter\": \"%s\"", sTestLog.wchHasFlowmeter);
		strJson.AppendFormat(L"\"FlowmeterNumber\": \"%s\"", sTestLog.wchFlowmeterNumber);
		strJson.AppendFormat(L"\"FlowmeterModel\": \"%s\"", sTestLog.wchFlowmeterModel);
		strJson.AppendFormat(L"\"FlowmeterManufacturer\": \"%s\"", sTestLog.wchFlowmeterManufacturer);
		strJson.AppendFormat(L"\"FlowmeterFluxCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterFluxCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterFluxCheckDate\": \"%s\"", sTestLog.wchFlowmeterFluxCheckDate);
		strJson.AppendFormat(L"\"FlowmeterDilO2CalibrationDate\": \"%s\"", sTestLog.wchFlowmeterDilO2CalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterPressureCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterPressureCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterAuthenticationDate\": \"%s\"", sTestLog.wchFlowmeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasOpacimeter\": \"%s\"", sTestLog.wchHasOpacimeter);
		strJson.AppendFormat(L"\"OpacimeterNumber\": \"%s\"", sTestLog.wchOpacimeterNumber);
		strJson.AppendFormat(L"\"OpacimeterModel\": \"%s\"", sTestLog.wchOpacimeterModel);
		strJson.AppendFormat(L"\"OpacimeterManufacturer\": \"%s\"", sTestLog.wchOpacimeterManufacturer);
		strJson.AppendFormat(L"\"OpacimeterChannelCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterChannelCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterChannelCheckDate\": \"%s\"", sTestLog.wchOpacimeterChannelCheckDate);
		strJson.AppendFormat(L"\"OpacimeterEnvParamCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterAuthenticationDate\": \"%s\"", sTestLog.wchOpacimeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasSmokemeter\": \"%s\"", sTestLog.wchHasSmokemeter);
		strJson.AppendFormat(L"\"SmokemeterNumber\": \"%s\"", sTestLog.wchSmokemeterNumber);
		strJson.AppendFormat(L"\"SmokemeterModel\": \"%s\"", sTestLog.wchSmokemeterModel);
		strJson.AppendFormat(L"\"SmokemeterManufacturer\": \"%s\"", sTestLog.wchSmokemeterManufacturer);
		strJson.AppendFormat(L"\"SmokemeterCalibrationDate\": \"%s\"", sTestLog.wchSmokemeterCalibrationDate);
		strJson.AppendFormat(L"\"SmokemeterCheckDate\": \"%s\"", sTestLog.wchSmokemeterCheckDate);
		strJson.AppendFormat(L"\"SmokemeterAuthenticationDate\": \"%s\"", sTestLog.wchSmokemeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentEnvParamMeasurer\": \"%s\"", sTestLog.wchHasIndependentEnvParamMeasurer);
		strJson.AppendFormat(L"\"EnvParamMeasurerNumber\": \"%s\"", sTestLog.wchEnvParamMeasurerNumber);
		strJson.AppendFormat(L"\"EnvParamMeasurerModel\": \"%s\"", sTestLog.wchEnvParamMeasurerModel);
		strJson.AppendFormat(L"\"EnvParamMeasurerManufacturer\": \"%s\"", sTestLog.wchEnvParamMeasurerManufacturer);
		strJson.AppendFormat(L"\"EnvParamMeasurerCalibrationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCalibrationDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerCheckDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCheckDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerAuthenticationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentTachometer\": \"%s\"", sTestLog.wchHasIndependentTachometer);
		strJson.AppendFormat(L"\"TachometerNumber\": \"%s\"", sTestLog.wchTachometerNumber);
		strJson.AppendFormat(L"\"TachometerModel\": \"%s\"", sTestLog.wchTachometerModel);
		strJson.AppendFormat(L"\"TachometerManufacturer\": \"%s\"", sTestLog.wchTachometerManufacturer);
		strJson.AppendFormat(L"\"TachometerCalibrationDate\": \"%s\"", sTestLog.wchTachometerCalibrationDate);
		strJson.AppendFormat(L"\"TachometerCheckDate\": \"%s\"", sTestLog.wchTachometerCheckDate);
		strJson.AppendFormat(L"\"TachometerAuthenticationDate\": \"%s\"", sTestLog.wchTachometerAuthenticationDate);
		strJson.AppendFormat(L"\"StationNumber\": \"%s\"", sTestLog.wchStationNumber);
		strJson.AppendFormat(L"\"StationFullName\": \"%s\"", sTestLog.wchStationFullName);
		strJson.AppendFormat(L"\"StationShortName\": \"%s\"", sTestLog.wchStationShortName);
		strJson.AppendFormat(L"\"StationLicenseSn\": \"%s\"", sTestLog.wchStationLicenseSn);
		strJson.AppendFormat(L"\"StationLicenseStartDate\": \"%s\"", sTestLog.wchStationLicenseStartDate);
		strJson.AppendFormat(L"\"StationLicenseEndDate\": \"%s\"", sTestLog.wchStationLicenseEndDate);
		strJson.AppendFormat(L"\"StationIsLocked\": \"%s\"", sTestLog.wchStationIsLocked);
		strJson.AppendFormat(L"\"StationRegistrationInfo\": \"%s\"", sTestLog.wchStationRegistrationInfo);
		strJson.AppendFormat(L"\"StationPhone\": \"%s\"", sTestLog.wchStationPhone);
		strJson.AppendFormat(L"\"StationFax\": \"%s\"", sTestLog.wchStationFax);
		strJson.AppendFormat(L"\"PostCode\": \"%s\"", sTestLog.wchPostCode);
		strJson.AppendFormat(L"\"Province\": \"%s\"", sTestLog.wchProvince);
		strJson.AppendFormat(L"\"City\": \"%s\"", sTestLog.wchCity);
		strJson.AppendFormat(L"\"District\": \"%s\"", sTestLog.wchDistrict);
		strJson.AppendFormat(L"\"DetailedAddress\": \"%s\"", sTestLog.wchDetailedAddress);

		strJson += L"}],";

		strJson += L"\"ResultOfVMAS\":[{";
		strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", sResultOfVMAS.strFuelTypeCode.c_str());
		strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", sResultOfVMAS.strFuelTypeName.c_str());
		strJson.AppendFormat(L"\"NeedTestHCNOx\": \"%s\",", sResultOfVMAS.strNeedTestHCNOx.c_str());
		strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(sResultOfVMAS.strO2.c_str()));
		strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(sResultOfVMAS.strCO2.c_str()));
		strJson.AppendFormat(L"\"HC\": \"%.2f\",", _wtof(sResultOfVMAS.strHC.c_str()));
		strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(sResultOfVMAS.strCO.c_str()));
		strJson.AppendFormat(L"\"NOx\": \"%.2f\",", _wtof(sResultOfVMAS.strNOx.c_str()));
		strJson.AppendFormat(L"\"HCNOx\": \"%.2f\",", _wtof(sResultOfVMAS.strHCNOx.c_str()));
		strJson.AppendFormat(L"\"LimitOfHC\": \"%.2f\",", _wtof(sResultOfVMAS.strLimitOfHC.c_str()));
		strJson.AppendFormat(L"\"LimitOfHC_EDP\": \"%s\",", sResultOfVMAS.strLimitOfHC_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfCO\": \"%.2f\",", _wtof(sResultOfVMAS.strLimitOfCO.c_str()));
		strJson.AppendFormat(L"\"LimitOfCO_EDP\": \"%s\",", sResultOfVMAS.strLimitOfCO_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfNOx\": \"%.2f\",", _wtof(sResultOfVMAS.strLimitOfNOx.c_str()));
		strJson.AppendFormat(L"\"LimitOfNOx_EDP\": \"%s\",", sResultOfVMAS.strLimitOfNOx_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfHCNOx\": \"%.2f\",", _wtof(sResultOfVMAS.strLimitOfHCNOx.c_str()));
		strJson.AppendFormat(L"\"LimitOfHCNOx_EDP\": \"%s\",", sResultOfVMAS.strLimitOfHCNOx_EDP.c_str());
		strJson.AppendFormat(L"\"PassOfHC\": \"%s\",", sResultOfVMAS.strPassOfHC.c_str());
		strJson.AppendFormat(L"\"PassOfCO\": \"%s\",", sResultOfVMAS.strPassOfCO.c_str());
		strJson.AppendFormat(L"\"PassOfNOx\": \"%s\",", sResultOfVMAS.strPassOfNOx.c_str());
		strJson.AppendFormat(L"\"PassOfHCNOx\": \"%s\",", sResultOfVMAS.strPassOfHCNOx.c_str());
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sResultOfVMAS.strPass.c_str());
		strJson.AppendFormat(L"\"Power50\": \"%.2f\",", _wtof(sResultOfVMAS.strPower50.c_str()));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%.2f\",", _wtof(sResultOfVMAS.strTravelledDistance.c_str()));
		strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(sResultOfVMAS.strOilTemperature.c_str()));
		strJson.AppendFormat(L"\"VolOfGas\": \"%.2f\",", _wtof(sResultOfVMAS.strVolOfGas.c_str()));
		strJson.AppendFormat(L"\"VolOfExhaust\": \"%.2f\",", _wtof(sResultOfVMAS.strVolOfExhaust.c_str()));
		strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(sResultOfVMAS.strEnvironmentalTemperature.c_str()));
		strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(sResultOfVMAS.strRelativeHumidity.c_str()));
		strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(sResultOfVMAS.strAtmosphericPressure.c_str()));
		strJson.AppendFormat(L"\"SkipRev\": \"%s\",", sResultOfVMAS.strSkipRev.c_str());
		strJson.AppendFormat(L"\"SkipOilTemperature\": \"%s\"", sResultOfVMAS.strSkipOilTemperature.c_str());
		strJson += L"}],";

		SResultOfVMAS sResultOfVMASTemp;
		sResultOfVMASTemp = sResultOfVMAS;
		std::vector<SRealTimeDataOfVMAS> vtRTData;
		vtRTData = sResultOfVMASTemp.RTDataUpZip();

		strJson += L"\"RealTimeDataOfVMAS\":[";
		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strJson += L"{";
			strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", vtRTData[i].strFuelTypeCode.c_str());
			strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", vtRTData[i].strFuelTypeName.c_str());
			strJson.AppendFormat(L"\"Time\": \"%s\",", vtRTData[i].strTime.c_str());
			strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", vtRTData[i].strSamplingTime.c_str());
			strJson.AppendFormat(L"\"Velocity\": \"%.2f\",", _wtof(vtRTData[i].strVelocity.c_str()));
			strJson.AppendFormat(L"\"Power\": \"%.2f\",", _wtof(vtRTData[i].strPower.c_str()));
			strJson.AppendFormat(L"\"Force\": \"%.2f\",", _wtof(vtRTData[i].strForce.c_str()));
			strJson.AppendFormat(L"\"IHP\": \"%.2f\",", _wtof(vtRTData[i].strIHP.c_str()));
			strJson.AppendFormat(L"\"PLHP\": \"%.2f\",", _wtof(vtRTData[i].strPLHP.c_str()));
			strJson.AppendFormat(L"\"HC\": \"%s\",", vtRTData[i].strHC.c_str());
			strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(vtRTData[i].strCO.c_str()));
			strJson.AppendFormat(L"\"NO\": \"%s\",", vtRTData[i].strNO.c_str());
			strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(vtRTData[i].strO2.c_str()));
			strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(vtRTData[i].strCO2.c_str()));
			strJson.AppendFormat(L"\"EngineRev\": \"%s\",", vtRTData[i].strEngineRev.c_str());
			strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(vtRTData[i].strLambda.c_str()));
			strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(vtRTData[i].strOilTemperature.c_str()));
			strJson.AppendFormat(L"\"O2OfFlowmeter\": \"%.2f\",", _wtof(vtRTData[i].strO2OfFlowmeter.c_str()));
			strJson.AppendFormat(L"\"O2OfEnvironment\": \"%.2f\",", _wtof(vtRTData[i].strO2OfEnvironment.c_str()));
			strJson.AppendFormat(L"\"HCm\": \"%.2f\",", _wtof(vtRTData[i].strHCm.c_str()));
			strJson.AppendFormat(L"\"COm\": \"%.2f\",", _wtof(vtRTData[i].strCOm.c_str()));
			strJson.AppendFormat(L"\"NOm\": \"%.2f\",", _wtof(vtRTData[i].strNOm.c_str()));
			strJson.AppendFormat(L"\"O2m\": \"%.2f\",", _wtof(vtRTData[i].strO2m.c_str()));
			strJson.AppendFormat(L"\"CO2m\": \"%.2f\",", _wtof(vtRTData[i].strCO2m.c_str()));
			strJson.AppendFormat(L"\"PressureOfFlowmeter\": \"%.2f\",", _wtof(vtRTData[i].strPressureOfFlowmeter.c_str()));
			strJson.AppendFormat(L"\"TemperatureOfFlowmeter\": \"%.2f\",", _wtof(vtRTData[i].strTemperatureOfFlowmeter.c_str()));
			strJson.AppendFormat(L"\"DR\": \"%.2f\",", _wtof(vtRTData[i].strDR.c_str()));
			strJson.AppendFormat(L"\"ActualFluxOfGas\": \"%.2f\",", _wtof(vtRTData[i].strActualFluxOfGas.c_str()));
			strJson.AppendFormat(L"\"StandardFluxOfGas\": \"%.2f\",", _wtof(vtRTData[i].strStandardFluxOfGas.c_str()));
			strJson.AppendFormat(L"\"FluxOfExhaust\": \"%.2f\",", _wtof(vtRTData[i].strFluxOfExhaust.c_str()));
			strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
			strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(vtRTData[i].strRelativeHumidity.c_str()));
			strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(vtRTData[i].strAtmosphericPressure.c_str()));
			strJson.AppendFormat(L"\"HumidityCorrectionFactor\": \"%.2f\",", _wtof(vtRTData[i].strHumidityCorrectionFactor.c_str()));
			strJson.AppendFormat(L"\"DilutionCorrectionFactor\": \"%.2f\",", _wtof(vtRTData[i].strDilutionCorrectionFactor.c_str()));
			strJson.AppendFormat(L"\"InertiaSimulationError\": \"%.2f\"", _wtof(vtRTData[i].strInertiaSimulationError.c_str()));

			//OBD数据
			strJson.AppendFormat(L"\"OBDRTData_EngineRev\": \"%s\",", vtRTData[i].sOBDRTData.strEngineRev.c_str());
			strJson.AppendFormat(L"\"OBDRTData_Velocity\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strVelocity.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CoolantTemp\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCoolantTemp.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_SolarTermDoorPosition\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ThrottleOpening\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strThrottleOpening.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CalculationLoad\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCalculationLoad.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorV\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorV.c_str()));			
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorI\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorI.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_AirCoefficient\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strAirCoefficient.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAF\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAF.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAP\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAP.c_str()));
			//以下检测没保存
			strJson.AppendFormat(L"\"OBDRTData_EngineOutputPower\": \"%s\",", vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());
			strJson.AppendFormat(L"\"OBDRTData_ChargeAirPressure\": \"%s\",", vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());
			strJson.AppendFormat(L"\"OBDRTData_FuelConsumption\": \"%s\",", vtRTData[i].sOBDRTData.strFuelConsumption.c_str());
			strJson.AppendFormat(L"\"OBDRTData_NOXConcentration\": \"%s\",", vtRTData[i].sOBDRTData.strNOXConcentration.c_str());
			strJson.AppendFormat(L"\"OBDRTData_UreaInjectionVolume\": \"%s\",", vtRTData[i].sOBDRTData.strUreaInjectionVolume.c_str());
			strJson.AppendFormat(L"\"OBDRTData_EGT\": \"%s\",", vtRTData[i].sOBDRTData.strEGT.c_str());
			strJson.AppendFormat(L"\"OBDRTData_DPFDifferentialPressure\": \"%s\",", vtRTData[i].sOBDRTData.strDPFDifferentialPressure.c_str());
			strJson.AppendFormat(L"\"OBDRTData_EGRPosition\": \"%s\",", vtRTData[i].sOBDRTData.strEGRPosition.c_str());
			strJson.AppendFormat(L"\"OBDRTData_FuelDeliveryPressure\": \"%s\",", vtRTData[i].sOBDRTData.strFuelDeliveryPressure.c_str());

			strJson += L"},";
		}
		// 删除最后一个"，"
		if(vtRTData.size()>0)
		{
			strJson.Delete(strJson.GetLength()-1);
		}

		//for (std::list<REALTIMEDATAOFVMAS>::iterator ir=listRealTimeDataOfVMAS.begin(); ir!=listRealTimeDataOfVMAS.end(); ir++)
		//{
		//	strJson += L"{";
		//	strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", ir->wchFuelTypeCode);
		//	strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", ir->wchFuelTypeName);
		//	strJson.AppendFormat(L"\"Time\": \"%s\",", ir->wchTime);
		//	strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", ir->wchSamplingTime);
		//	strJson.AppendFormat(L"\"Velocity\": \"%.2f\",", _wtof(ir->wchVelocity));
		//	strJson.AppendFormat(L"\"Power\": \"%.2f\",", _wtof(ir->wchPower));
		//	strJson.AppendFormat(L"\"Force\": \"%.2f\",", _wtof(ir->wchForce));
		//	strJson.AppendFormat(L"\"IHP\": \"%.2f\",", _wtof(ir->wchIHP));
		//	strJson.AppendFormat(L"\"PLHP\": \"%.2f\",", _wtof(ir->wchPLHP));
		//	strJson.AppendFormat(L"\"HC\": \"%s\",", ir->wchHC);
		//	strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(ir->wchCO));
		//	strJson.AppendFormat(L"\"NO\": \"%s\",", ir->wchNO);
		//	strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(ir->wchO2));
		//	strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(ir->wchCO2));
		//	strJson.AppendFormat(L"\"EngineRev\": \"%s\",", ir->wchEngineRev);
		//	strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(ir->wchLambda));
		//	strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(ir->wchOilTemperature));
		//	strJson.AppendFormat(L"\"O2OfFlowmeter\": \"%.2f\",", _wtof(ir->wchO2OfFlowmeter));
		//	strJson.AppendFormat(L"\"O2OfEnvironment\": \"%.2f\",", _wtof(ir->wchO2OfEnvironment));
		//	strJson.AppendFormat(L"\"HCm\": \"%.2f\",", _wtof(ir->wchHCm));
		//	strJson.AppendFormat(L"\"COm\": \"%.2f\",", _wtof(ir->wchCOm));
		//	strJson.AppendFormat(L"\"NOm\": \"%.2f\",", _wtof(ir->wchNOm));
		//	strJson.AppendFormat(L"\"O2m\": \"%.2f\",", _wtof(ir->wchO2m));
		//	strJson.AppendFormat(L"\"CO2m\": \"%.2f\",", _wtof(ir->wchCO2m));
		//	strJson.AppendFormat(L"\"PressureOfFlowmeter\": \"%.2f\",", _wtof(ir->wchPressureOfFlowmeter));
		//	strJson.AppendFormat(L"\"TemperatureOfFlowmeter\": \"%.2f\",", _wtof(ir->wchTemperatureOfFlowmeter));
		//	strJson.AppendFormat(L"\"DR\": \"%.2f\",", _wtof(ir->wchDR));
		//	strJson.AppendFormat(L"\"ActualFluxOfGas\": \"%.2f\",", _wtof(ir->wchActualFluxOfGas));
		//	strJson.AppendFormat(L"\"StandardFluxOfGas\": \"%.2f\",", _wtof(ir->wchStandardFluxOfGas));
		//	strJson.AppendFormat(L"\"FluxOfExhaust\": \"%.2f\",", _wtof(ir->wchFluxOfExhaust));
		//	strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(ir->wchEnvironmentalTemperature));
		//	strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(ir->wchRelativeHumidity));
		//	strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(ir->wchAtmosphericPressure));
		//	strJson.AppendFormat(L"\"HumidityCorrectionFactor\": \"%.2f\",", _wtof(ir->wchHumidityCorrectionFactor));
		//	strJson.AppendFormat(L"\"DilutionCorrectionFactor\": \"%.2f\",", _wtof(ir->wchDilutionCorrectionFactor));
		//	strJson.AppendFormat(L"\"InertiaSimulationError\": \"%.2f\"", _wtof(ir->wchInertiaSimulationError));
		//	strJson += L"},";
		//}
		//// 删除最后一个"，"
		//if(listRealTimeDataOfVMAS.size()>0)
		//{
		//	strJson.Delete(strJson.GetLength()-1);
		//}
		strJson += L"]}";

		char *pch(NULL);
		pch = CNHCommonAPI::UnicodeToANSI(strJson);

		unsigned long nLen = strlen(pch);

		BYTE *szBuf = new BYTE[nLen]();
		int nRet = compress(szBuf, &nLen, (BYTE*)pch, nLen);

		ZBase64 zBase64;
		std::string strBufBase64 = zBase64.EncodeRaw(szBuf, nLen);

		DataTrans((BYTE*)strBufBase64.c_str(), strBufBase64.length());

		if(szBuf != NULL)
		{
			delete[] szBuf;
		}

		if (NULL != pch)
		{
			free(pch);
			pch = NULL;
		}
	}

	return true;
}

bool CParaThread::DT_LUGDOWN(const TESTLOG &sTestLog, const SResultOfLUGDOWN &sResultOfLUGDOWN)
{
	if (IM_NANHUA_EPCS_4G==m_emIM && IsDBOpen())
	{
		CString strJson;

		strJson += L"{\"TestLog\":[{";
		strJson.AppendFormat(L"\"RunningNumber\": \"%s\",", sTestLog.wchRunningNumber);
		strJson.AppendFormat(L"\"ReportNumber\": \"%s\",", sTestLog.wchReportNumber);
		strJson.AppendFormat(L"\"BillDate\": \"%s\",", sTestLog.wchBillDate);
		strJson.AppendFormat(L"\"Biller\": \"%s\",", sTestLog.wchBiller);
		strJson.AppendFormat(L"\"Shroff\": \"%s\",", sTestLog.wchShroff);
		strJson.AppendFormat(L"\"ChargeState\": \"%s\",", sTestLog.wchChargeState);
		strJson.AppendFormat(L"\"InitialMoney\": \"%.2f\",", _wtof(sTestLog.wchInitialMoney));
		strJson.AppendFormat(L"\"ChangedMoney\": \"%.2f\",", _wtof(sTestLog.wchChangedMoney));
		strJson.AppendFormat(L"\"ReceivableMoney\": \"%.2f\",", _wtof(sTestLog.wchReceivableMoney));
		strJson.AppendFormat(L"\"ActualMoney\": \"%.2f\",", _wtof(sTestLog.wchActualMoney));
		strJson.AppendFormat(L"\"TestLocation\": \"%s\",", sTestLog.wchTestLocation);
		strJson.AppendFormat(L"\"TestState\": \"%s\",", sTestLog.wchTestState);
		strJson.AppendFormat(L"\"IsTestFinished\": \"%s\",", sTestLog.wchIsTestFinished);
		strJson.AppendFormat(L"\"NumberOfTestPeriodASM\": \"%s\",", sTestLog.wchNumberOfTestPeriodASM);
		strJson.AppendFormat(L"\"NumberOfTestPeriodVMAS\": \"%s\",", sTestLog.wchNumberOfTestPeriodVMAS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodLUGDOWN\": \"%s\",", sTestLog.wchNumberOfTestPeriodLUGDOWN);
		strJson.AppendFormat(L"\"NumberOfTestPeriodDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUNHT\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUNHT);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUYD\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUYD);
		strJson.AppendFormat(L"\"NumberOfTestPeriodAVFSU\": \"%s\",", sTestLog.wchNumberOfTestPeriodAVFSU);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMI\": \"%s\",", sTestLog.wchNumberOfTestPeriodMI);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodMDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriod\": \"%s\",", sTestLog.wchNumberOfTestPeriod);
		strJson.AppendFormat(L"\"TestPeriodUsed\": \"%s\",", sTestLog.wchTestPeriodUsed);
		strJson.AppendFormat(L"\"QuitReason\": \"%s\",", sTestLog.wchQuitReason);
		strJson.AppendFormat(L"\"TestType\": \"%s\",", sTestLog.wchTestType);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sTestLog.wchPass);
		strJson.AppendFormat(L"\"Operator\": \"%s\",", sTestLog.wchOperator);
		strJson.AppendFormat(L"\"Driver\": \"%s\",", sTestLog.wchDriver);
		strJson.AppendFormat(L"\"Assessor\": \"%s\",", sTestLog.wchAssessor);
		strJson.AppendFormat(L"\"IsRetest\": \"%s\",", sTestLog.wchIsRetest);
		//strJson.AppendFormat(L"\"EntryPCIP\": \"%s\",", sTestLog.wchEntryPCIP);
		strJson.AppendFormat(L"\"IsFuelTypeNeedTest\": \"%s\",", sTestLog.wchIsFuelTypeNeedTest);
		strJson.AppendFormat(L"\"IsFuelTypeTestFinished\": \"%s\",", sTestLog.wchIsFuelTypeTestFinished);
		strJson.AppendFormat(L"\"IsFuelTypeTestPass\": \"%s\",", sTestLog.wchIsFuelTypeTestPass);
		strJson.AppendFormat(L"\"IsFuelType2NeedTest\": \"%s\",", sTestLog.wchIsFuelType2NeedTest);
		strJson.AppendFormat(L"\"IsFuelType2TestFinished\": \"%s\",", sTestLog.wchIsFuelType2TestFinished);
		strJson.AppendFormat(L"\"IsFuelType2TestPass\": \"%s\",", sTestLog.wchIsFuelType2TestPass);
		strJson.AppendFormat(L"\"TestDate\": \"%s\",", sTestLog.wchTestDate);
		strJson.AppendFormat(L"\"TestStartTime\": \"%s\",", sTestLog.wchTestStartTime);
		strJson.AppendFormat(L"\"TestEndTime\": \"%s\",", sTestLog.wchTestEndTime);
		strJson.AppendFormat(L"\"TestCharacter\": \"%s\",", sTestLog.wchTestCharacter);
		//strJson.AppendFormat(L"\"AuditTimes\": \"%s\",", sTestLog.wchAuditTimes);
		//strJson.AppendFormat(L"\"RecentAuditTime\": \"%s\",", sTestLog.wchRecentAuditTime);
		strJson.AppendFormat(L"\"PrintTimes\": \"%s\",", sTestLog.wchPrintTimes);
		strJson.AppendFormat(L"\"RecentPrintReportTime\": \"%s\",", sTestLog.wchRecentPrintReportTime);
		//strJson.AppendFormat(L"\"PrintInvoiceTimes\": \"%s\",", sTestLog.wchPrintInvoiceTimes);
		//strJson.AppendFormat(L"\"RecentPrintInvoiceTime\": \"%s\",", sTestLog.wchRecentPrintInvoiceTime);
		//strJson.AppendFormat(L"\"PrintEPSignTimes\": \"%s\",", sTestLog.wchPrintEPSignTimes);
		//strJson.AppendFormat(L"\"RecentPrintEPSignTime\": \"%s\",", sTestLog.wchRecentPrintEPSignTime);
		//strJson.AppendFormat(L"\"PrintCertificateTimes\": \"%s\",", sTestLog.wchPrintCertificateTimes);
		//strJson.AppendFormat(L"\"RecentPrintCertificateTime\": \"%s\",", sTestLog.wchRecentPrintCertificateTime);
		//strJson.AppendFormat(L"\"IsTestValid\": \"%s\",", sTestLog.wchIsTestValid);
		strJson.AppendFormat(L"\"IsSent\": \"%s\",", sTestLog.wchIsSent);
		strJson.AppendFormat(L"\"IsSentEP\": \"%s\",", sTestLog.wchIsSentEP);
		strJson.AppendFormat(L"\"SRM\": \"%s\",", sTestLog.wchSRM);
		//strJson.AppendFormat(L"\"Harmony\": \"%s\",", sTestLog.wchHarmony);
		strJson.AppendFormat(L"\"PlateNumber\": \"%s\",", sTestLog.wchPlateNumber);
		strJson.AppendFormat(L"\"PlateType\": \"%s\",", sTestLog.wchPlateType);
		strJson.AppendFormat(L"\"RegistrationDate\": \"%s\",", sTestLog.wchRegistrationDate);
		strJson.AppendFormat(L"\"Owner\": \"%s\",", sTestLog.wchOwner);
		strJson.AppendFormat(L"\"Phone\": \"%s\",", sTestLog.wchPhone);
		strJson.AppendFormat(L"\"Address\": \"%s\",", sTestLog.wchAddress);
		strJson.AppendFormat(L"\"UseCharacter\": \"%s\",", sTestLog.wchUseCharacter);
		strJson.AppendFormat(L"\"Manufacturer\": \"%s\",", sTestLog.wchManufacturer);
		strJson.AppendFormat(L"\"Brand\": \"%s\",", sTestLog.wchBrand);
		strJson.AppendFormat(L"\"Model\": \"%s\",", sTestLog.wchModel);
		strJson.AppendFormat(L"\"ProductionDate\": \"%s\",", sTestLog.wchProductionDate);
		strJson.AppendFormat(L"\"VIN\": \"%s\",", sTestLog.wchVIN);
		strJson.AppendFormat(L"\"ChassisModel\": \"%s\",", sTestLog.wchChassisModel);
		strJson.AppendFormat(L"\"VehicleType\": \"%s\",", sTestLog.wchVehicleType);
		strJson.AppendFormat(L"\"MaximumTotalMass\": \"%s\",", sTestLog.wchMaximumTotalMass);
		strJson.AppendFormat(L"\"UnladenMass\": \"%s\",", sTestLog.wchUnladenMass);
		strJson.AppendFormat(L"\"AxleMass\": \"%s\",", sTestLog.wchAxleMass);
		strJson.AppendFormat(L"\"RatedLoadingMass\": \"%s\",", sTestLog.wchRatedLoadingMass);
		strJson.AppendFormat(L"\"RatedPassengerCapacity\": \"%s\",", sTestLog.wchRatedPassengerCapacity);
		strJson.AppendFormat(L"\"TyrePressure\": \"%.2f\",", _wtof(sTestLog.wchTyrePressure));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%s\",", sTestLog.wchTravelledDistance);
		strJson.AppendFormat(L"\"EngineManufacturer\": \"%s\",", sTestLog.wchEngineManufacturer);
		strJson.AppendFormat(L"\"EngineModel\": \"%s\",", sTestLog.wchEngineModel);
		strJson.AppendFormat(L"\"EngineNumber\": \"%s\",", sTestLog.wchEngineNumber);
		strJson.AppendFormat(L"\"EngineStroke\": \"%s\",", sTestLog.wchEngineStroke);
		strJson.AppendFormat(L"\"Displacement\": \"%.2f\",", _wtof(sTestLog.wchDisplacement));
		strJson.AppendFormat(L"\"NumberOfCylinder\": \"%s\",", sTestLog.wchNumberOfCylinder);
		strJson.AppendFormat(L"\"OilSupplyMode\": \"%s\",", sTestLog.wchOilSupplyMode);
		strJson.AppendFormat(L"\"AirIntakeMode\": \"%s\",", sTestLog.wchAirIntakeMode);
		strJson.AppendFormat(L"\"HasCatalyticConverter\": \"%s\",", sTestLog.wchHasCatalyticConverter);
		strJson.AppendFormat(L"\"FuelType\": \"%s\",", sTestLog.wchFuelType);
		strJson.AppendFormat(L"\"FuelMark\": \"%s\",", sTestLog.wchFuelMark);
		strJson.AppendFormat(L"\"DoubleFuel\": \"%s\",", sTestLog.wchDoubleFuel);
		strJson.AppendFormat(L"\"FuelType2\": \"%s\",", sTestLog.wchFuelType2);
		strJson.AppendFormat(L"\"FuelMark2\": \"%s\",", sTestLog.wchFuelMark2);
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sTestLog.wchRatedRev);
		strJson.AppendFormat(L"\"RatedPower\": \"%.2f\",", _wtof(sTestLog.wchRatedPower));
		strJson.AppendFormat(L"\"MaximumNetPower\": \"%.2f\",", _wtof(sTestLog.wchMaximumNetPower));
		strJson.AppendFormat(L"\"GearBoxType\": \"%s\",", sTestLog.wchGearBoxType);
		strJson.AppendFormat(L"\"NumberOfGears\": \"%s\",", sTestLog.wchNumberOfGears);
		strJson.AppendFormat(L"\"DriveType\": \"%s\",", sTestLog.wchDriveType);
		strJson.AppendFormat(L"\"EPSign\": \"%s\",", sTestLog.wchEPSign);
		strJson.AppendFormat(L"\"CertificateNumber\": \"%s\",", sTestLog.wchCertificateNumber);
		strJson.AppendFormat(L"\"IndependentDoubleExhaustPipe\": \"%s\",", sTestLog.wchIndependentDoubleExhaustPipe);
		strJson.AppendFormat(L"\"TCS\": \"%s\",", sTestLog.wchTCS);
		strJson.AppendFormat(L"\"FuelPumpMode\": \"%s\",", sTestLog.wchFuelPumpMode);
		strJson.AppendFormat(L"\"PassengerVehicle\": \"%s\",", sTestLog.wchPassengerVehicle);
		strJson.AppendFormat(L"\"EmissionStandard\": \"%s\",", sTestLog.wchEmissionStandard);
		strJson.AppendFormat(L"\"AnnualTestPeriod\": \"%s\",", sTestLog.wchAnnualTestPeriod);
		strJson.AppendFormat(L"\"HasOBD\": \"%s\",", sTestLog.wchHasOBD);
		strJson.AppendFormat(L"\"OwnerCharacter\": \"%s\",", sTestLog.wchOwnerCharacter);
		strJson.AppendFormat(L"\"OwnerDistrict\": \"%s\",", sTestLog.wchOwnerDistrict);
		strJson.AppendFormat(L"\"LicenseDepartment\": \"%s\",", sTestLog.wchLicenseDepartment);
		strJson.AppendFormat(L"\"LicenseIssueDate\": \"%s\",", sTestLog.wchLicenseIssueDate);
		strJson.AppendFormat(L"\"LicenseValidDate\": \"%s\"", sTestLog.wchLicenseValidDate);

		strJson.AppendFormat(L"\"LineNumber\": \"%s\"", sTestLog.wchLineNumber);
		strJson.AppendFormat(L"\"LineModel\": \"%s\"", sTestLog.wchLineModel);
		strJson.AppendFormat(L"\"LineName\": \"%s\"", sTestLog.wchLineName);
		strJson.AppendFormat(L"\"LineManufacturer\": \"%s\"", sTestLog.wchLineManufacturer);
		strJson.AppendFormat(L"\"LineLicenseSn\": \"%s\"", sTestLog.wchLineLicenseSn);
		strJson.AppendFormat(L"\"LineLicenseStartDate\": \"%s\"", sTestLog.wchLineLicenseStartDate);
		strJson.AppendFormat(L"\"LineLicenseEndDate\": \"%s\"", sTestLog.wchLineLicenseEndDate);
		strJson.AppendFormat(L"\"LineIsLocked\": \"%s\"", sTestLog.wchLineIsLocked);
		strJson.AppendFormat(L"\"LineRegistrationInfo\": \"%s\"", sTestLog.wchLineRegistrationInfo);
		strJson.AppendFormat(L"\"LineIPAddress\": \"%s\"", sTestLog.wchLineIPAddress);
		strJson.AppendFormat(L"\"LineHardwareSN\": \"%s\"", sTestLog.wchLineHardwareSN);
		strJson.AppendFormat(L"\"HasDynamometer\": \"%s\"", sTestLog.wchHasDynamometer);
		strJson.AppendFormat(L"\"DynamometerNumber\": \"%s\"", sTestLog.wchDynamometerNumber);
		strJson.AppendFormat(L"\"DynamometerModel\": \"%s\"", sTestLog.wchDynamometerModel);
		strJson.AppendFormat(L"\"DynamometerManufacturer\": \"%s\"", sTestLog.wchDynamometerManufacturer);
		strJson.AppendFormat(L"\"DynamometerCoastDownDate\": \"%s\"", sTestLog.wchDynamometerCoastDownDate);
		strJson.AppendFormat(L"\"DynamometerPLHPDate\": \"%s\"", sTestLog.wchDynamometerPLHPDate);
		strJson.AppendFormat(L"\"DynamometerForceCalibrationDate\": \"%s\"", sTestLog.wchDynamometerForceCalibrationDate);
		strJson.AppendFormat(L"\"DynamometerForceCheckDate\": \"%s\"", sTestLog.wchDynamometerForceCheckDate);
		strJson.AppendFormat(L"\"DynamometerAuthenticationDate\": \"%s\"", sTestLog.wchDynamometerAuthenticationDate);
		strJson.AppendFormat(L"\"HasAnalyzer\": \"%s\"", sTestLog.wchHasAnalyzer);
		strJson.AppendFormat(L"\"AnalyzerNumber\": \"%s\"", sTestLog.wchAnalyzerNumber);
		strJson.AppendFormat(L"\"AnalyzerModel\": \"%s\"", sTestLog.wchAnalyzerModel);
		strJson.AppendFormat(L"\"AnalyzerManufacturer\": \"%s\"", sTestLog.wchAnalyzerManufacturer);
		strJson.AppendFormat(L"\"AnalyzerChannelCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerChannelCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerChannelCheckDate\": \"%s\"", sTestLog.wchAnalyzerChannelCheckDate);
		strJson.AppendFormat(L"\"AnalyzerEnvParamCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerAuthenticationDate\": \"%s\"", sTestLog.wchAnalyzerAuthenticationDate);
		strJson.AppendFormat(L"\"HasFlowmeter\": \"%s\"", sTestLog.wchHasFlowmeter);
		strJson.AppendFormat(L"\"FlowmeterNumber\": \"%s\"", sTestLog.wchFlowmeterNumber);
		strJson.AppendFormat(L"\"FlowmeterModel\": \"%s\"", sTestLog.wchFlowmeterModel);
		strJson.AppendFormat(L"\"FlowmeterManufacturer\": \"%s\"", sTestLog.wchFlowmeterManufacturer);
		strJson.AppendFormat(L"\"FlowmeterFluxCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterFluxCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterFluxCheckDate\": \"%s\"", sTestLog.wchFlowmeterFluxCheckDate);
		strJson.AppendFormat(L"\"FlowmeterDilO2CalibrationDate\": \"%s\"", sTestLog.wchFlowmeterDilO2CalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterPressureCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterPressureCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterAuthenticationDate\": \"%s\"", sTestLog.wchFlowmeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasOpacimeter\": \"%s\"", sTestLog.wchHasOpacimeter);
		strJson.AppendFormat(L"\"OpacimeterNumber\": \"%s\"", sTestLog.wchOpacimeterNumber);
		strJson.AppendFormat(L"\"OpacimeterModel\": \"%s\"", sTestLog.wchOpacimeterModel);
		strJson.AppendFormat(L"\"OpacimeterManufacturer\": \"%s\"", sTestLog.wchOpacimeterManufacturer);
		strJson.AppendFormat(L"\"OpacimeterChannelCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterChannelCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterChannelCheckDate\": \"%s\"", sTestLog.wchOpacimeterChannelCheckDate);
		strJson.AppendFormat(L"\"OpacimeterEnvParamCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterAuthenticationDate\": \"%s\"", sTestLog.wchOpacimeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasSmokemeter\": \"%s\"", sTestLog.wchHasSmokemeter);
		strJson.AppendFormat(L"\"SmokemeterNumber\": \"%s\"", sTestLog.wchSmokemeterNumber);
		strJson.AppendFormat(L"\"SmokemeterModel\": \"%s\"", sTestLog.wchSmokemeterModel);
		strJson.AppendFormat(L"\"SmokemeterManufacturer\": \"%s\"", sTestLog.wchSmokemeterManufacturer);
		strJson.AppendFormat(L"\"SmokemeterCalibrationDate\": \"%s\"", sTestLog.wchSmokemeterCalibrationDate);
		strJson.AppendFormat(L"\"SmokemeterCheckDate\": \"%s\"", sTestLog.wchSmokemeterCheckDate);
		strJson.AppendFormat(L"\"SmokemeterAuthenticationDate\": \"%s\"", sTestLog.wchSmokemeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentEnvParamMeasurer\": \"%s\"", sTestLog.wchHasIndependentEnvParamMeasurer);
		strJson.AppendFormat(L"\"EnvParamMeasurerNumber\": \"%s\"", sTestLog.wchEnvParamMeasurerNumber);
		strJson.AppendFormat(L"\"EnvParamMeasurerModel\": \"%s\"", sTestLog.wchEnvParamMeasurerModel);
		strJson.AppendFormat(L"\"EnvParamMeasurerManufacturer\": \"%s\"", sTestLog.wchEnvParamMeasurerManufacturer);
		strJson.AppendFormat(L"\"EnvParamMeasurerCalibrationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCalibrationDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerCheckDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCheckDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerAuthenticationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentTachometer\": \"%s\"", sTestLog.wchHasIndependentTachometer);
		strJson.AppendFormat(L"\"TachometerNumber\": \"%s\"", sTestLog.wchTachometerNumber);
		strJson.AppendFormat(L"\"TachometerModel\": \"%s\"", sTestLog.wchTachometerModel);
		strJson.AppendFormat(L"\"TachometerManufacturer\": \"%s\"", sTestLog.wchTachometerManufacturer);
		strJson.AppendFormat(L"\"TachometerCalibrationDate\": \"%s\"", sTestLog.wchTachometerCalibrationDate);
		strJson.AppendFormat(L"\"TachometerCheckDate\": \"%s\"", sTestLog.wchTachometerCheckDate);
		strJson.AppendFormat(L"\"TachometerAuthenticationDate\": \"%s\"", sTestLog.wchTachometerAuthenticationDate);
		strJson.AppendFormat(L"\"StationNumber\": \"%s\"", sTestLog.wchStationNumber);
		strJson.AppendFormat(L"\"StationFullName\": \"%s\"", sTestLog.wchStationFullName);
		strJson.AppendFormat(L"\"StationShortName\": \"%s\"", sTestLog.wchStationShortName);
		strJson.AppendFormat(L"\"StationLicenseSn\": \"%s\"", sTestLog.wchStationLicenseSn);
		strJson.AppendFormat(L"\"StationLicenseStartDate\": \"%s\"", sTestLog.wchStationLicenseStartDate);
		strJson.AppendFormat(L"\"StationLicenseEndDate\": \"%s\"", sTestLog.wchStationLicenseEndDate);
		strJson.AppendFormat(L"\"StationIsLocked\": \"%s\"", sTestLog.wchStationIsLocked);
		strJson.AppendFormat(L"\"StationRegistrationInfo\": \"%s\"", sTestLog.wchStationRegistrationInfo);
		strJson.AppendFormat(L"\"StationPhone\": \"%s\"", sTestLog.wchStationPhone);
		strJson.AppendFormat(L"\"StationFax\": \"%s\"", sTestLog.wchStationFax);
		strJson.AppendFormat(L"\"PostCode\": \"%s\"", sTestLog.wchPostCode);
		strJson.AppendFormat(L"\"Province\": \"%s\"", sTestLog.wchProvince);
		strJson.AppendFormat(L"\"City\": \"%s\"", sTestLog.wchCity);
		strJson.AppendFormat(L"\"District\": \"%s\"", sTestLog.wchDistrict);
		strJson.AppendFormat(L"\"DetailedAddress\": \"%s\"", sTestLog.wchDetailedAddress);

		strJson += L"}],";

		strJson += L"\"ResultOfLUGDOWN\":[{";
		strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", sResultOfLUGDOWN.strFuelTypeCode.c_str());
		strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", sResultOfLUGDOWN.strFuelTypeName.c_str());
		strJson.AppendFormat(L"\"K100\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strK100.c_str()));
		strJson.AppendFormat(L"\"N100\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strN100.c_str()));
		strJson.AppendFormat(L"\"P100\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strP100.c_str()));
		strJson.AppendFormat(L"\"NOx100\": \"%s\",", sResultOfLUGDOWN.strNOx100.c_str());
		strJson.AppendFormat(L"\"EngineRev100\": \"%s\",", sResultOfLUGDOWN.strEngineRev100.c_str());
		strJson.AppendFormat(L"\"OilTemperature100\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strOilTemperature100.c_str()));
		strJson.AppendFormat(L"\"K90\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strK90.c_str()));
		strJson.AppendFormat(L"\"N90\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strN90.c_str()));
		strJson.AppendFormat(L"\"P90\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strP90.c_str()));
		strJson.AppendFormat(L"\"EngineRev90\": \"%s\",", sResultOfLUGDOWN.strEngineRev90.c_str());
		strJson.AppendFormat(L"\"OilTemperature90\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strOilTemperature90.c_str()));
		strJson.AppendFormat(L"\"K80\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strK80.c_str()));
		strJson.AppendFormat(L"\"N80\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strN80.c_str()));
		strJson.AppendFormat(L"\"P80\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strP80.c_str()));
		strJson.AppendFormat(L"\"NOx80\": \"%s\",", sResultOfLUGDOWN.strNOx80.c_str());
		strJson.AppendFormat(L"\"EngineRev80\": \"%s\",", sResultOfLUGDOWN.strEngineRev80.c_str());
		strJson.AppendFormat(L"\"OilTemperature80\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strOilTemperature80.c_str()));
		strJson.AppendFormat(L"\"KLimit\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strKLimit.c_str()));
		strJson.AppendFormat(L"\"KLimit_EDP\": \"%s\",", sResultOfLUGDOWN.strKLimit_EDP.c_str());
		strJson.AppendFormat(L"\"NLimit\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strNLimit.c_str()));
		strJson.AppendFormat(L"\"NLimit_EDP\": \"%s\",", sResultOfLUGDOWN.strNLimit_EDP.c_str());
		strJson.AppendFormat(L"\"NOxLimit\": \"%s\",", sResultOfLUGDOWN.strNOxLimit.c_str());
		strJson.AppendFormat(L"\"PowerCorrectionFactor\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strPowerCorrectionFactor.c_str()));
		strJson.AppendFormat(L"\"CorrectedPower\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strCorrectedPower.c_str()));
		strJson.AppendFormat(L"\"MinHP\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strMinHP.c_str()));
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sResultOfLUGDOWN.strRatedRev.c_str());
		strJson.AppendFormat(L"\"PassOfK100\": \"%s\",", sResultOfLUGDOWN.strPassOfK100.c_str());
		strJson.AppendFormat(L"\"PassOfNOx100\": \"%s\",", sResultOfLUGDOWN.strPassOfNOx100.c_str());
		strJson.AppendFormat(L"\"PassOfK90\": \"%s\",", sResultOfLUGDOWN.strPassOfK90.c_str());
		strJson.AppendFormat(L"\"PassOfK80\": \"%s\",", sResultOfLUGDOWN.strPassOfK80.c_str());
		strJson.AppendFormat(L"\"PassOfNOx80\": \"%s\",", sResultOfLUGDOWN.strPassOfNOx80.c_str());
		strJson.AppendFormat(L"\"PassOfP\": \"%s\",", sResultOfLUGDOWN.strPassOfP.c_str());
		strJson.AppendFormat(L"\"PassOfRev\": \"%s\",", sResultOfLUGDOWN.strPassOfRev.c_str());
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sResultOfLUGDOWN.strPass.c_str());
		strJson.AppendFormat(L"\"MaxHP\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strMaxHP.c_str()));
		strJson.AppendFormat(L"\"ActualVelMaxHP\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strActualVelMaxHP.c_str()));
		strJson.AppendFormat(L"\"CalculatedVelMaxHP\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strCalculatedVelMaxHP.c_str()));
		strJson.AppendFormat(L"\"MaxRPM\": \"%s\",", sResultOfLUGDOWN.strMaxRPM.c_str());
		strJson.AppendFormat(L"\"IdleRev\": \"%s\",", sResultOfLUGDOWN.strIdleRev.c_str());
		strJson.AppendFormat(L"\"TotalDuration\": \"%s\",", sResultOfLUGDOWN.strTotalDuration.c_str());
		strJson.AppendFormat(L"\"ModeDuration\": \"%s\",", sResultOfLUGDOWN.strModeDuration.c_str());
		strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", (sResultOfLUGDOWN.strOilTemperature.c_str()));
		strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strEnvironmentalTemperature.c_str()));
		strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strRelativeHumidity.c_str()));
		strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(sResultOfLUGDOWN.strAtmosphericPressure.c_str()));
		strJson.AppendFormat(L"\"SkipRev\": \"%s\",", sResultOfLUGDOWN.strSkipRev.c_str());
		strJson.AppendFormat(L"\"SkipOilTemperature\": \"%s\",", sResultOfLUGDOWN.strSkipOilTemperature.c_str());
		strJson.AppendFormat(L"\"SkipRevJudgement\": \"%s\",", sResultOfLUGDOWN.strSkipRevJudgement.c_str());
		strJson.AppendFormat(L"\"RevPassRange\": \"%s\"", sResultOfLUGDOWN.strRevPassRange.c_str());
		strJson += L"}],";

		SResultOfLUGDOWN sResultOfLUGDOWNTemp;
		sResultOfLUGDOWNTemp = sResultOfLUGDOWN;
		std::vector<SRealTimeDataOfLUGDOWN> vtRTData;
		vtRTData = sResultOfLUGDOWNTemp.RTDataUpZip();

		strJson += L"\"RealTimeDataOfLUGDOWN\":[";

		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strJson += L"{";
			strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", vtRTData[i].strFuelTypeCode.c_str());
			strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", vtRTData[i].strFuelTypeName.c_str());
			strJson.AppendFormat(L"\"Time\": \"%s\",", vtRTData[i].strTime.c_str());
			strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", vtRTData[i].strSamplingTime.c_str());
			strJson.AppendFormat(L"\"Velocity\": \"%.2f\",", _wtof(vtRTData[i].strVelocity.c_str()));
			strJson.AppendFormat(L"\"Power\": \"%.2f\",", _wtof(vtRTData[i].strPower.c_str()));
			strJson.AppendFormat(L"\"Force\": \"%.2f\",", _wtof(vtRTData[i].strForce.c_str()));
			strJson.AppendFormat(L"\"IHP\": \"%.2f\",", _wtof(vtRTData[i].strIHP.c_str()));
			strJson.AppendFormat(L"\"PLHP\": \"%.2f\",", _wtof(vtRTData[i].strPLHP.c_str()));
			strJson.AppendFormat(L"\"State\": \"%s\",", vtRTData[i].strState.c_str());
			strJson.AppendFormat(L"\"K\": \"%.2f\",", _wtof(vtRTData[i].strK.c_str()));
			strJson.AppendFormat(L"\"N\": \"%.2f\",", _wtof(vtRTData[i].strN.c_str()));
			strJson.AppendFormat(L"\"EngineRev\": \"%s\",", vtRTData[i].strEngineRev.c_str());
			strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(vtRTData[i].strOilTemperature.c_str()));
			strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
			strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(vtRTData[i].strRelativeHumidity.c_str()));
			strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(vtRTData[i].strAtmosphericPressure.c_str()));
			strJson.AppendFormat(L"\"PowerCorrectionFactor\": \"%.2f\"", _wtof(vtRTData[i].strPowerCorrectionFactor.c_str()));
			strJson.AppendFormat(L"\"HC\": \"%s\",", vtRTData[i].strHC.c_str());
			strJson.AppendFormat(L"\"NO\": \"%s\",", vtRTData[i].strNO.c_str());
			strJson.AppendFormat(L"\"NO2\": \"%s\",", vtRTData[i].strNO2.c_str());
			strJson.AppendFormat(L"\"CO\": \"%.2f\"", _wtof(vtRTData[i].strCO.c_str()));
			strJson.AppendFormat(L"\"CO2\": \"%.2f\"", _wtof(vtRTData[i].strCO2.c_str()));
			strJson.AppendFormat(L"\"O2\": \"%.2f\"", _wtof(vtRTData[i].strO2.c_str()));
			strJson.AppendFormat(L"\"HumidityCorrectionFactor\": \"%.2f\"", _wtof(vtRTData[i].strHumidityCorrectionFactor.c_str()));

			//OBD数据
			strJson.AppendFormat(L"\"OBDRTData_EngineRev\": \"%s\",", vtRTData[i].sOBDRTData.strEngineRev.c_str());
			strJson.AppendFormat(L"\"OBDRTData_Velocity\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strVelocity.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CoolantTemp\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCoolantTemp.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_SolarTermDoorPosition\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ThrottleOpening\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strThrottleOpening.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CalculationLoad\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCalculationLoad.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorV\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorV.c_str()));			
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorI\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorI.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_AirCoefficient\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strAirCoefficient.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAF\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAF.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAP\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAP.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_EngineOutputPower\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strEngineOutputPower.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ChargeAirPressure\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strChargeAirPressure.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_FuelConsumption\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strFuelConsumption.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_NOXConcentration\": \"%s\",", vtRTData[i].sOBDRTData.strNOXConcentration.c_str());
			strJson.AppendFormat(L"\"OBDRTData_UreaInjectionVolume\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_EGT\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strEGT.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_DPFDifferentialPressure\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_EGRPosition\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strEGRPosition.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_FuelDeliveryPressure\": \"%s\",", vtRTData[i].sOBDRTData.strFuelDeliveryPressure.c_str());

			strJson += L"},";
		}
		// 删除最后一个"，"
		if(vtRTData.size()>0)
		{
			strJson.Delete(strJson.GetLength()-1);
		}

		//for (std::list<REALTIMEDATAOFLUGDOWN>::iterator ir=listRealTimeDataOfLUGDOWN.begin(); ir!=listRealTimeDataOfLUGDOWN.end(); ir++)
		//{
		//	strJson += L"{";
		//	strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", ir->wchFuelTypeCode);
		//	strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", ir->wchFuelTypeName);
		//	strJson.AppendFormat(L"\"Time\": \"%s\",", ir->wchTime);
		//	strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", ir->wchSamplingTime);
		//	strJson.AppendFormat(L"\"Velocity\": \"%.2f\",", _wtof(ir->wchVelocity));
		//	strJson.AppendFormat(L"\"Power\": \"%.2f\",", _wtof(ir->wchPower));
		//	strJson.AppendFormat(L"\"Force\": \"%.2f\",", _wtof(ir->wchForce));
		//	strJson.AppendFormat(L"\"IHP\": \"%.2f\",", _wtof(ir->wchIHP));
		//	strJson.AppendFormat(L"\"PLHP\": \"%.2f\",", _wtof(ir->wchPLHP));
		//	strJson.AppendFormat(L"\"State\": \"%s\",", ir->wchState);
		//	strJson.AppendFormat(L"\"K\": \"%.2f\",", _wtof(ir->wchK));
		//	strJson.AppendFormat(L"\"N\": \"%.2f\",", _wtof(ir->wchN));
		//	strJson.AppendFormat(L"\"EngineRev\": \"%s\",", ir->wchEngineRev);
		//	strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(ir->wchOilTemperature));
		//	strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(ir->wchEnvironmentalTemperature));
		//	strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(ir->wchRelativeHumidity));
		//	strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(ir->wchAtmosphericPressure));
		//	strJson.AppendFormat(L"\"PowerCorrectionFactor\": \"%.2f\"", _wtof(ir->wchPowerCorrectionFactor));
		//	strJson += L"},";
		//}
		//// 删除最后一个"，"
		//if(listRealTimeDataOfLUGDOWN.size()>0)
		//{
		//	strJson.Delete(strJson.GetLength()-1);
		//}
		strJson += L"]}";

		char *pch(NULL);
		pch = CNHCommonAPI::UnicodeToANSI(strJson);

		unsigned long nLen = strlen(pch);

		BYTE *szBuf = new BYTE[nLen]();
		int nRet = compress(szBuf, &nLen, (BYTE*)pch, nLen);

		ZBase64 zBase64;
		std::string strBufBase64 = zBase64.EncodeRaw(szBuf, nLen);

		DataTrans((BYTE*)strBufBase64.c_str(), strBufBase64.length());

		if(szBuf != NULL)
		{
			delete[] szBuf;
		}

		if (NULL != pch)
		{
			free(pch);
			pch = NULL;
		}
	}

	return true;
}

bool CParaThread::DT_DIS(const TESTLOG &sTestLog, const SResultOfDIS &sResultOfDIS)
{
	if (IM_NANHUA_EPCS_4G==m_emIM && IsDBOpen())
	{
		CString strJson;

		strJson += L"{\"TestLog\":[{";
		strJson.AppendFormat(L"\"RunningNumber\": \"%s\",", sTestLog.wchRunningNumber);
		strJson.AppendFormat(L"\"ReportNumber\": \"%s\",", sTestLog.wchReportNumber);
		strJson.AppendFormat(L"\"BillDate\": \"%s\",", sTestLog.wchBillDate);
		strJson.AppendFormat(L"\"Biller\": \"%s\",", sTestLog.wchBiller);
		strJson.AppendFormat(L"\"Shroff\": \"%s\",", sTestLog.wchShroff);
		strJson.AppendFormat(L"\"ChargeState\": \"%s\",", sTestLog.wchChargeState);
		strJson.AppendFormat(L"\"InitialMoney\": \"%.2f\",", _wtof(sTestLog.wchInitialMoney));
		strJson.AppendFormat(L"\"ChangedMoney\": \"%.2f\",", _wtof(sTestLog.wchChangedMoney));
		strJson.AppendFormat(L"\"ReceivableMoney\": \"%.2f\",", _wtof(sTestLog.wchReceivableMoney));
		strJson.AppendFormat(L"\"ActualMoney\": \"%.2f\",", _wtof(sTestLog.wchActualMoney));
		strJson.AppendFormat(L"\"TestLocation\": \"%s\",", sTestLog.wchTestLocation);
		strJson.AppendFormat(L"\"TestState\": \"%s\",", sTestLog.wchTestState);
		strJson.AppendFormat(L"\"IsTestFinished\": \"%s\",", sTestLog.wchIsTestFinished);
		strJson.AppendFormat(L"\"NumberOfTestPeriodASM\": \"%s\",", sTestLog.wchNumberOfTestPeriodASM);
		strJson.AppendFormat(L"\"NumberOfTestPeriodVMAS\": \"%s\",", sTestLog.wchNumberOfTestPeriodVMAS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodLUGDOWN\": \"%s\",", sTestLog.wchNumberOfTestPeriodLUGDOWN);
		strJson.AppendFormat(L"\"NumberOfTestPeriodDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUNHT\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUNHT);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUYD\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUYD);
		strJson.AppendFormat(L"\"NumberOfTestPeriodAVFSU\": \"%s\",", sTestLog.wchNumberOfTestPeriodAVFSU);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMI\": \"%s\",", sTestLog.wchNumberOfTestPeriodMI);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodMDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriod\": \"%s\",", sTestLog.wchNumberOfTestPeriod);
		strJson.AppendFormat(L"\"TestPeriodUsed\": \"%s\",", sTestLog.wchTestPeriodUsed);
		strJson.AppendFormat(L"\"QuitReason\": \"%s\",", sTestLog.wchQuitReason);
		strJson.AppendFormat(L"\"TestType\": \"%s\",", sTestLog.wchTestType);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sTestLog.wchPass);
		strJson.AppendFormat(L"\"Operator\": \"%s\",", sTestLog.wchOperator);
		strJson.AppendFormat(L"\"Driver\": \"%s\",", sTestLog.wchDriver);
		strJson.AppendFormat(L"\"Assessor\": \"%s\",", sTestLog.wchAssessor);
		strJson.AppendFormat(L"\"IsRetest\": \"%s\",", sTestLog.wchIsRetest);
		//strJson.AppendFormat(L"\"EntryPCIP\": \"%s\",", sTestLog.wchEntryPCIP);
		strJson.AppendFormat(L"\"IsFuelTypeNeedTest\": \"%s\",", sTestLog.wchIsFuelTypeNeedTest);
		strJson.AppendFormat(L"\"IsFuelTypeTestFinished\": \"%s\",", sTestLog.wchIsFuelTypeTestFinished);
		strJson.AppendFormat(L"\"IsFuelTypeTestPass\": \"%s\",", sTestLog.wchIsFuelTypeTestPass);
		strJson.AppendFormat(L"\"IsFuelType2NeedTest\": \"%s\",", sTestLog.wchIsFuelType2NeedTest);
		strJson.AppendFormat(L"\"IsFuelType2TestFinished\": \"%s\",", sTestLog.wchIsFuelType2TestFinished);
		strJson.AppendFormat(L"\"IsFuelType2TestPass\": \"%s\",", sTestLog.wchIsFuelType2TestPass);
		strJson.AppendFormat(L"\"TestDate\": \"%s\",", sTestLog.wchTestDate);
		strJson.AppendFormat(L"\"TestStartTime\": \"%s\",", sTestLog.wchTestStartTime);
		strJson.AppendFormat(L"\"TestEndTime\": \"%s\",", sTestLog.wchTestEndTime);
		strJson.AppendFormat(L"\"TestCharacter\": \"%s\",", sTestLog.wchTestCharacter);
		//strJson.AppendFormat(L"\"AuditTimes\": \"%s\",", sTestLog.wchAuditTimes);
		//strJson.AppendFormat(L"\"RecentAuditTime\": \"%s\",", sTestLog.wchRecentAuditTime);
		strJson.AppendFormat(L"\"PrintTimes\": \"%s\",", sTestLog.wchPrintTimes);
		strJson.AppendFormat(L"\"RecentPrintReportTime\": \"%s\",", sTestLog.wchRecentPrintReportTime);
		//strJson.AppendFormat(L"\"PrintInvoiceTimes\": \"%s\",", sTestLog.wchPrintInvoiceTimes);
		//strJson.AppendFormat(L"\"RecentPrintInvoiceTime\": \"%s\",", sTestLog.wchRecentPrintInvoiceTime);
		//strJson.AppendFormat(L"\"PrintEPSignTimes\": \"%s\",", sTestLog.wchPrintEPSignTimes);
		//strJson.AppendFormat(L"\"RecentPrintEPSignTime\": \"%s\",", sTestLog.wchRecentPrintEPSignTime);
		//strJson.AppendFormat(L"\"PrintCertificateTimes\": \"%s\",", sTestLog.wchPrintCertificateTimes);
		//strJson.AppendFormat(L"\"RecentPrintCertificateTime\": \"%s\",", sTestLog.wchRecentPrintCertificateTime);
		//strJson.AppendFormat(L"\"IsTestValid\": \"%s\",", sTestLog.wchIsTestValid);
		strJson.AppendFormat(L"\"IsSent\": \"%s\",", sTestLog.wchIsSent);
		strJson.AppendFormat(L"\"IsSentEP\": \"%s\",", sTestLog.wchIsSentEP);
		strJson.AppendFormat(L"\"SRM\": \"%s\",", sTestLog.wchSRM);
		//strJson.AppendFormat(L"\"Harmony\": \"%s\",", sTestLog.wchHarmony);
		strJson.AppendFormat(L"\"PlateNumber\": \"%s\",", sTestLog.wchPlateNumber);
		strJson.AppendFormat(L"\"PlateType\": \"%s\",", sTestLog.wchPlateType);
		strJson.AppendFormat(L"\"RegistrationDate\": \"%s\",", sTestLog.wchRegistrationDate);
		strJson.AppendFormat(L"\"Owner\": \"%s\",", sTestLog.wchOwner);
		strJson.AppendFormat(L"\"Phone\": \"%s\",", sTestLog.wchPhone);
		strJson.AppendFormat(L"\"Address\": \"%s\",", sTestLog.wchAddress);
		strJson.AppendFormat(L"\"UseCharacter\": \"%s\",", sTestLog.wchUseCharacter);
		strJson.AppendFormat(L"\"Manufacturer\": \"%s\",", sTestLog.wchManufacturer);
		strJson.AppendFormat(L"\"Brand\": \"%s\",", sTestLog.wchBrand);
		strJson.AppendFormat(L"\"Model\": \"%s\",", sTestLog.wchModel);
		strJson.AppendFormat(L"\"ProductionDate\": \"%s\",", sTestLog.wchProductionDate);
		strJson.AppendFormat(L"\"VIN\": \"%s\",", sTestLog.wchVIN);
		strJson.AppendFormat(L"\"ChassisModel\": \"%s\",", sTestLog.wchChassisModel);
		strJson.AppendFormat(L"\"VehicleType\": \"%s\",", sTestLog.wchVehicleType);
		strJson.AppendFormat(L"\"MaximumTotalMass\": \"%s\",", sTestLog.wchMaximumTotalMass);
		strJson.AppendFormat(L"\"UnladenMass\": \"%s\",", sTestLog.wchUnladenMass);
		strJson.AppendFormat(L"\"AxleMass\": \"%s\",", sTestLog.wchAxleMass);
		strJson.AppendFormat(L"\"RatedLoadingMass\": \"%s\",", sTestLog.wchRatedLoadingMass);
		strJson.AppendFormat(L"\"RatedPassengerCapacity\": \"%s\",", sTestLog.wchRatedPassengerCapacity);
		strJson.AppendFormat(L"\"TyrePressure\": \"%.2f\",", _wtof(sTestLog.wchTyrePressure));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%s\",", sTestLog.wchTravelledDistance);
		strJson.AppendFormat(L"\"EngineManufacturer\": \"%s\",", sTestLog.wchEngineManufacturer);
		strJson.AppendFormat(L"\"EngineModel\": \"%s\",", sTestLog.wchEngineModel);
		strJson.AppendFormat(L"\"EngineNumber\": \"%s\",", sTestLog.wchEngineNumber);
		strJson.AppendFormat(L"\"EngineStroke\": \"%s\",", sTestLog.wchEngineStroke);
		strJson.AppendFormat(L"\"Displacement\": \"%.2f\",", _wtof(sTestLog.wchDisplacement));
		strJson.AppendFormat(L"\"NumberOfCylinder\": \"%s\",", sTestLog.wchNumberOfCylinder);
		strJson.AppendFormat(L"\"OilSupplyMode\": \"%s\",", sTestLog.wchOilSupplyMode);
		strJson.AppendFormat(L"\"AirIntakeMode\": \"%s\",", sTestLog.wchAirIntakeMode);
		strJson.AppendFormat(L"\"HasCatalyticConverter\": \"%s\",", sTestLog.wchHasCatalyticConverter);
		strJson.AppendFormat(L"\"FuelType\": \"%s\",", sTestLog.wchFuelType);
		strJson.AppendFormat(L"\"FuelMark\": \"%s\",", sTestLog.wchFuelMark);
		strJson.AppendFormat(L"\"DoubleFuel\": \"%s\",", sTestLog.wchDoubleFuel);
		strJson.AppendFormat(L"\"FuelType2\": \"%s\",", sTestLog.wchFuelType2);
		strJson.AppendFormat(L"\"FuelMark2\": \"%s\",", sTestLog.wchFuelMark2);
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sTestLog.wchRatedRev);
		strJson.AppendFormat(L"\"RatedPower\": \"%.2f\",", _wtof(sTestLog.wchRatedPower));
		strJson.AppendFormat(L"\"MaximumNetPower\": \"%.2f\",", _wtof(sTestLog.wchMaximumNetPower));
		strJson.AppendFormat(L"\"GearBoxType\": \"%s\",", sTestLog.wchGearBoxType);
		strJson.AppendFormat(L"\"NumberOfGears\": \"%s\",", sTestLog.wchNumberOfGears);
		strJson.AppendFormat(L"\"DriveType\": \"%s\",", sTestLog.wchDriveType);
		strJson.AppendFormat(L"\"EPSign\": \"%s\",", sTestLog.wchEPSign);
		strJson.AppendFormat(L"\"CertificateNumber\": \"%s\",", sTestLog.wchCertificateNumber);
		strJson.AppendFormat(L"\"IndependentDoubleExhaustPipe\": \"%s\",", sTestLog.wchIndependentDoubleExhaustPipe);
		strJson.AppendFormat(L"\"TCS\": \"%s\",", sTestLog.wchTCS);
		strJson.AppendFormat(L"\"FuelPumpMode\": \"%s\",", sTestLog.wchFuelPumpMode);
		strJson.AppendFormat(L"\"PassengerVehicle\": \"%s\",", sTestLog.wchPassengerVehicle);
		strJson.AppendFormat(L"\"EmissionStandard\": \"%s\",", sTestLog.wchEmissionStandard);
		strJson.AppendFormat(L"\"AnnualTestPeriod\": \"%s\",", sTestLog.wchAnnualTestPeriod);
		strJson.AppendFormat(L"\"HasOBD\": \"%s\",", sTestLog.wchHasOBD);
		strJson.AppendFormat(L"\"OwnerCharacter\": \"%s\",", sTestLog.wchOwnerCharacter);
		strJson.AppendFormat(L"\"OwnerDistrict\": \"%s\",", sTestLog.wchOwnerDistrict);
		strJson.AppendFormat(L"\"LicenseDepartment\": \"%s\",", sTestLog.wchLicenseDepartment);
		strJson.AppendFormat(L"\"LicenseIssueDate\": \"%s\",", sTestLog.wchLicenseIssueDate);
		strJson.AppendFormat(L"\"LicenseValidDate\": \"%s\"", sTestLog.wchLicenseValidDate);

		strJson.AppendFormat(L"\"LineNumber\": \"%s\"", sTestLog.wchLineNumber);
		strJson.AppendFormat(L"\"LineModel\": \"%s\"", sTestLog.wchLineModel);
		strJson.AppendFormat(L"\"LineName\": \"%s\"", sTestLog.wchLineName);
		strJson.AppendFormat(L"\"LineManufacturer\": \"%s\"", sTestLog.wchLineManufacturer);
		strJson.AppendFormat(L"\"LineLicenseSn\": \"%s\"", sTestLog.wchLineLicenseSn);
		strJson.AppendFormat(L"\"LineLicenseStartDate\": \"%s\"", sTestLog.wchLineLicenseStartDate);
		strJson.AppendFormat(L"\"LineLicenseEndDate\": \"%s\"", sTestLog.wchLineLicenseEndDate);
		strJson.AppendFormat(L"\"LineIsLocked\": \"%s\"", sTestLog.wchLineIsLocked);
		strJson.AppendFormat(L"\"LineRegistrationInfo\": \"%s\"", sTestLog.wchLineRegistrationInfo);
		strJson.AppendFormat(L"\"LineIPAddress\": \"%s\"", sTestLog.wchLineIPAddress);
		strJson.AppendFormat(L"\"LineHardwareSN\": \"%s\"", sTestLog.wchLineHardwareSN);
		strJson.AppendFormat(L"\"HasDynamometer\": \"%s\"", sTestLog.wchHasDynamometer);
		strJson.AppendFormat(L"\"DynamometerNumber\": \"%s\"", sTestLog.wchDynamometerNumber);
		strJson.AppendFormat(L"\"DynamometerModel\": \"%s\"", sTestLog.wchDynamometerModel);
		strJson.AppendFormat(L"\"DynamometerManufacturer\": \"%s\"", sTestLog.wchDynamometerManufacturer);
		strJson.AppendFormat(L"\"DynamometerCoastDownDate\": \"%s\"", sTestLog.wchDynamometerCoastDownDate);
		strJson.AppendFormat(L"\"DynamometerPLHPDate\": \"%s\"", sTestLog.wchDynamometerPLHPDate);
		strJson.AppendFormat(L"\"DynamometerForceCalibrationDate\": \"%s\"", sTestLog.wchDynamometerForceCalibrationDate);
		strJson.AppendFormat(L"\"DynamometerForceCheckDate\": \"%s\"", sTestLog.wchDynamometerForceCheckDate);
		strJson.AppendFormat(L"\"DynamometerAuthenticationDate\": \"%s\"", sTestLog.wchDynamometerAuthenticationDate);
		strJson.AppendFormat(L"\"HasAnalyzer\": \"%s\"", sTestLog.wchHasAnalyzer);
		strJson.AppendFormat(L"\"AnalyzerNumber\": \"%s\"", sTestLog.wchAnalyzerNumber);
		strJson.AppendFormat(L"\"AnalyzerModel\": \"%s\"", sTestLog.wchAnalyzerModel);
		strJson.AppendFormat(L"\"AnalyzerManufacturer\": \"%s\"", sTestLog.wchAnalyzerManufacturer);
		strJson.AppendFormat(L"\"AnalyzerChannelCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerChannelCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerChannelCheckDate\": \"%s\"", sTestLog.wchAnalyzerChannelCheckDate);
		strJson.AppendFormat(L"\"AnalyzerEnvParamCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerAuthenticationDate\": \"%s\"", sTestLog.wchAnalyzerAuthenticationDate);
		strJson.AppendFormat(L"\"HasFlowmeter\": \"%s\"", sTestLog.wchHasFlowmeter);
		strJson.AppendFormat(L"\"FlowmeterNumber\": \"%s\"", sTestLog.wchFlowmeterNumber);
		strJson.AppendFormat(L"\"FlowmeterModel\": \"%s\"", sTestLog.wchFlowmeterModel);
		strJson.AppendFormat(L"\"FlowmeterManufacturer\": \"%s\"", sTestLog.wchFlowmeterManufacturer);
		strJson.AppendFormat(L"\"FlowmeterFluxCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterFluxCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterFluxCheckDate\": \"%s\"", sTestLog.wchFlowmeterFluxCheckDate);
		strJson.AppendFormat(L"\"FlowmeterDilO2CalibrationDate\": \"%s\"", sTestLog.wchFlowmeterDilO2CalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterPressureCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterPressureCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterAuthenticationDate\": \"%s\"", sTestLog.wchFlowmeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasOpacimeter\": \"%s\"", sTestLog.wchHasOpacimeter);
		strJson.AppendFormat(L"\"OpacimeterNumber\": \"%s\"", sTestLog.wchOpacimeterNumber);
		strJson.AppendFormat(L"\"OpacimeterModel\": \"%s\"", sTestLog.wchOpacimeterModel);
		strJson.AppendFormat(L"\"OpacimeterManufacturer\": \"%s\"", sTestLog.wchOpacimeterManufacturer);
		strJson.AppendFormat(L"\"OpacimeterChannelCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterChannelCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterChannelCheckDate\": \"%s\"", sTestLog.wchOpacimeterChannelCheckDate);
		strJson.AppendFormat(L"\"OpacimeterEnvParamCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterAuthenticationDate\": \"%s\"", sTestLog.wchOpacimeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasSmokemeter\": \"%s\"", sTestLog.wchHasSmokemeter);
		strJson.AppendFormat(L"\"SmokemeterNumber\": \"%s\"", sTestLog.wchSmokemeterNumber);
		strJson.AppendFormat(L"\"SmokemeterModel\": \"%s\"", sTestLog.wchSmokemeterModel);
		strJson.AppendFormat(L"\"SmokemeterManufacturer\": \"%s\"", sTestLog.wchSmokemeterManufacturer);
		strJson.AppendFormat(L"\"SmokemeterCalibrationDate\": \"%s\"", sTestLog.wchSmokemeterCalibrationDate);
		strJson.AppendFormat(L"\"SmokemeterCheckDate\": \"%s\"", sTestLog.wchSmokemeterCheckDate);
		strJson.AppendFormat(L"\"SmokemeterAuthenticationDate\": \"%s\"", sTestLog.wchSmokemeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentEnvParamMeasurer\": \"%s\"", sTestLog.wchHasIndependentEnvParamMeasurer);
		strJson.AppendFormat(L"\"EnvParamMeasurerNumber\": \"%s\"", sTestLog.wchEnvParamMeasurerNumber);
		strJson.AppendFormat(L"\"EnvParamMeasurerModel\": \"%s\"", sTestLog.wchEnvParamMeasurerModel);
		strJson.AppendFormat(L"\"EnvParamMeasurerManufacturer\": \"%s\"", sTestLog.wchEnvParamMeasurerManufacturer);
		strJson.AppendFormat(L"\"EnvParamMeasurerCalibrationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCalibrationDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerCheckDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCheckDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerAuthenticationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentTachometer\": \"%s\"", sTestLog.wchHasIndependentTachometer);
		strJson.AppendFormat(L"\"TachometerNumber\": \"%s\"", sTestLog.wchTachometerNumber);
		strJson.AppendFormat(L"\"TachometerModel\": \"%s\"", sTestLog.wchTachometerModel);
		strJson.AppendFormat(L"\"TachometerManufacturer\": \"%s\"", sTestLog.wchTachometerManufacturer);
		strJson.AppendFormat(L"\"TachometerCalibrationDate\": \"%s\"", sTestLog.wchTachometerCalibrationDate);
		strJson.AppendFormat(L"\"TachometerCheckDate\": \"%s\"", sTestLog.wchTachometerCheckDate);
		strJson.AppendFormat(L"\"TachometerAuthenticationDate\": \"%s\"", sTestLog.wchTachometerAuthenticationDate);
		strJson.AppendFormat(L"\"StationNumber\": \"%s\"", sTestLog.wchStationNumber);
		strJson.AppendFormat(L"\"StationFullName\": \"%s\"", sTestLog.wchStationFullName);
		strJson.AppendFormat(L"\"StationShortName\": \"%s\"", sTestLog.wchStationShortName);
		strJson.AppendFormat(L"\"StationLicenseSn\": \"%s\"", sTestLog.wchStationLicenseSn);
		strJson.AppendFormat(L"\"StationLicenseStartDate\": \"%s\"", sTestLog.wchStationLicenseStartDate);
		strJson.AppendFormat(L"\"StationLicenseEndDate\": \"%s\"", sTestLog.wchStationLicenseEndDate);
		strJson.AppendFormat(L"\"StationIsLocked\": \"%s\"", sTestLog.wchStationIsLocked);
		strJson.AppendFormat(L"\"StationRegistrationInfo\": \"%s\"", sTestLog.wchStationRegistrationInfo);
		strJson.AppendFormat(L"\"StationPhone\": \"%s\"", sTestLog.wchStationPhone);
		strJson.AppendFormat(L"\"StationFax\": \"%s\"", sTestLog.wchStationFax);
		strJson.AppendFormat(L"\"PostCode\": \"%s\"", sTestLog.wchPostCode);
		strJson.AppendFormat(L"\"Province\": \"%s\"", sTestLog.wchProvince);
		strJson.AppendFormat(L"\"City\": \"%s\"", sTestLog.wchCity);
		strJson.AppendFormat(L"\"District\": \"%s\"", sTestLog.wchDistrict);
		strJson.AppendFormat(L"\"DetailedAddress\": \"%s\"", sTestLog.wchDetailedAddress);

		strJson += L"}],";


		strJson += L"\"ResultOfDIS\":[{";

		strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", sResultOfDIS.strFuelTypeCode.c_str());
		strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", sResultOfDIS.strFuelTypeName.c_str());
		strJson.AppendFormat(L"\"LimitOfHCHigh\": \"%s\",", sResultOfDIS.strLimitOfHCHigh.c_str());
		strJson.AppendFormat(L"\"LimitOfHCHigh_EDP\": \"%s\",", sResultOfDIS.strLimitOfHCHigh_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfCOHigh\": \"%.2f\",", _wtof(sResultOfDIS.strLimitOfCOHigh.c_str()));
		strJson.AppendFormat(L"\"LimitOfCOHigh_EDP\": \"%s\",", sResultOfDIS.strLimitOfCOHigh_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfHCLow\": \"%s\",", sResultOfDIS.strLimitOfHCLow.c_str());
		strJson.AppendFormat(L"\"LimitOfHCLow_EDP\": \"%s\",", sResultOfDIS.strLimitOfHCLow_EDP.c_str());
		strJson.AppendFormat(L"\"LimitOfCOLow\": \"%.2f\",", _wtof(sResultOfDIS.strLimitOfCOLow.c_str()));
		strJson.AppendFormat(L"\"LimitOfCOLow_EDP\": \"%s\",", sResultOfDIS.strLimitOfCOLow_EDP.c_str());
		strJson.AppendFormat(L"\"LowerLimitOfLambda\": \"%.2f\",", _wtof(sResultOfDIS.strLowerLimitOfLambda.c_str()));
		strJson.AppendFormat(L"\"UpperLimitOfLambda\": \"%.2f\",", _wtof(sResultOfDIS.strUpperLimitOfLambda.c_str()));
		strJson.AppendFormat(L"\"LimitOfLambda_EDP\": \"%s\",", sResultOfDIS.strLimitOfLambda_EDP.c_str());
		strJson.AppendFormat(L"\"RevHigh\": \"%s\",", sResultOfDIS.strRevHigh.c_str());
		strJson.AppendFormat(L"\"OilTemperatureHigh\": \"%.2f\",", _wtof(sResultOfDIS.strOilTemperatureHigh.c_str()));
		strJson.AppendFormat(L"\"LambdaHigh\": \"%.2f\",", _wtof(sResultOfDIS.strLambdaHigh.c_str()));
		strJson.AppendFormat(L"\"NOHigh\": \"%s\",", sResultOfDIS.strNOHigh.c_str());
		strJson.AppendFormat(L"\"CO2High\": \"%.2f\",", _wtof(sResultOfDIS.strCO2High.c_str()));
		strJson.AppendFormat(L"\"O2High\": \"%.2f\",", _wtof(sResultOfDIS.strO2High.c_str()));
		strJson.AppendFormat(L"\"HCHigh\": \"%s\",", sResultOfDIS.strHCHigh.c_str());
		strJson.AppendFormat(L"\"COHigh\": \"%.2f\",", _wtof(sResultOfDIS.strCOHigh.c_str()));
		strJson.AppendFormat(L"\"PassOfHCHigh\": \"%s\",", sResultOfDIS.strPassOfHCHigh.c_str());
		strJson.AppendFormat(L"\"PassOfCOHigh\": \"%s\",", sResultOfDIS.strPassOfCOHigh.c_str());
		strJson.AppendFormat(L"\"NeedTestLambda\": \"%s\",", sResultOfDIS.strNeedTestLambda.c_str());
		strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(sResultOfDIS.strLambda.c_str()));
		strJson.AppendFormat(L"\"PassOfLambda\": \"%s\",", sResultOfDIS.strPassOfLambda.c_str());
		strJson.AppendFormat(L"\"NeedTestLIS\": \"%s\",", sResultOfDIS.strNeedTestLIS.c_str());
		strJson.AppendFormat(L"\"RevLow\": \"%s\",", sResultOfDIS.strRevLow.c_str());
		strJson.AppendFormat(L"\"OilTemperatureLow\": \"%.2f\",", _wtof(sResultOfDIS.strOilTemperatureLow.c_str()));
		strJson.AppendFormat(L"\"LambdaLow\": \"%.2f\",", _wtof(sResultOfDIS.strLambdaLow.c_str()));
		strJson.AppendFormat(L"\"NOLow\": \"%s\",", sResultOfDIS.strNOLow.c_str());
		strJson.AppendFormat(L"\"CO2Low\": \"%.2f\",", _wtof(sResultOfDIS.strCO2Low.c_str()));
		strJson.AppendFormat(L"\"O2Low\": \"%.2f\",", _wtof(sResultOfDIS.strO2Low.c_str()));
		strJson.AppendFormat(L"\"HCLow\": \"%s\",", sResultOfDIS.strHCLow.c_str());
		strJson.AppendFormat(L"\"COLow\": \"%.2f\",", _wtof(sResultOfDIS.strCOLow.c_str()));
		strJson.AppendFormat(L"\"PassOfHCLow\": \"%s\",", sResultOfDIS.strPassOfHCLow.c_str());
		strJson.AppendFormat(L"\"PassOfCOLow\": \"%s\",", sResultOfDIS.strPassOfCOLow.c_str());
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sResultOfDIS.strPass.c_str());
		strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(sResultOfDIS.strEnvironmentalTemperature.c_str()));
		strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(sResultOfDIS.strRelativeHumidity.c_str()));
		strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(sResultOfDIS.strAtmosphericPressure.c_str()));
		strJson.AppendFormat(L"\"SkipRev\": \"%s\",", sResultOfDIS.strSkipRev.c_str());
		strJson.AppendFormat(L"\"SkipOilTemperature\": \"%s\"", sResultOfDIS.strSkipOilTemperature.c_str());
		strJson += L"}],";

		SResultOfDIS sResultOfDISTemp;
		sResultOfDISTemp = sResultOfDIS;
		std::vector<SRealTimeDataOfDIS> vtRTData;
		vtRTData = sResultOfDISTemp.RTDataUpZip();
		strJson += L"\"RealTimeDataOfDIS\":[";

		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strJson += L"{";
			strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", vtRTData[i].strFuelTypeCode.c_str());
			strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", vtRTData[i].strFuelTypeName.c_str());
			strJson.AppendFormat(L"\"Time\": \"%s\",", vtRTData[i].strTime.c_str());
			strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", vtRTData[i].strSamplingTime.c_str());
			strJson.AppendFormat(L"\"State\": \"%s\",", vtRTData[i].strState.c_str());
			strJson.AppendFormat(L"\"HC\": \"%s\",", vtRTData[i].strHC.c_str());
			strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(vtRTData[i].strCO.c_str()));
			strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(vtRTData[i].strO2.c_str()));
			strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(vtRTData[i].strCO2.c_str()));
			strJson.AppendFormat(L"\"NO\": \"%s\",", vtRTData[i].strNO.c_str());
			strJson.AppendFormat(L"\"EngineRev\": \"%s\",", vtRTData[i].strEngineRev.c_str());
			strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(vtRTData[i].strOilTemperature.c_str()));
			strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(vtRTData[i].strLambda.c_str()));
			strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(vtRTData[i].strEnvironmentalTemperature.c_str()));
			strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(vtRTData[i].strRelativeHumidity.c_str()));
			strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\"", _wtof(vtRTData[i].strAtmosphericPressure.c_str()));

			//OBD数据
			strJson.AppendFormat(L"\"OBDRTData_EngineRev\": \"%s\",", vtRTData[i].sOBDRTData.strEngineRev.c_str());
			strJson.AppendFormat(L"\"OBDRTData_Velocity\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strVelocity.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CoolantTemp\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCoolantTemp.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_SolarTermDoorPosition\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ThrottleOpening\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strThrottleOpening.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_CalculationLoad\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strCalculationLoad.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorV\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorV.c_str()));			
			strJson.AppendFormat(L"\"OBDRTData_ForwardSensorI\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strForwardSensorI.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_AirCoefficient\": \"%.2f\",", _wtof(vtRTData[i].sOBDRTData.strAirCoefficient.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAF\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAF.c_str()));
			strJson.AppendFormat(L"\"OBDRTData_MAP\": \"%.1f\",", _wtof(vtRTData[i].sOBDRTData.strMAP.c_str()));
			//以下检测没保存
			strJson.AppendFormat(L"\"OBDRTData_EngineOutputPower\": \"%s\",", vtRTData[i].sOBDRTData.strEngineOutputPower.c_str());
			strJson.AppendFormat(L"\"OBDRTData_ChargeAirPressure\": \"%s\",", vtRTData[i].sOBDRTData.strChargeAirPressure.c_str());
			strJson.AppendFormat(L"\"OBDRTData_FuelConsumption\": \"%s\",", vtRTData[i].sOBDRTData.strFuelConsumption.c_str());
			strJson.AppendFormat(L"\"OBDRTData_NOXConcentration\": \"%s\",", vtRTData[i].sOBDRTData.strNOXConcentration.c_str());
			strJson.AppendFormat(L"\"OBDRTData_UreaInjectionVolume\": \"%s\",", vtRTData[i].sOBDRTData.strUreaInjectionVolume.c_str());
			strJson.AppendFormat(L"\"OBDRTData_EGT\": \"%s\",", vtRTData[i].sOBDRTData.strEGT.c_str());
			strJson.AppendFormat(L"\"OBDRTData_DPFDifferentialPressure\": \"%s\",", vtRTData[i].sOBDRTData.strDPFDifferentialPressure.c_str());
			strJson.AppendFormat(L"\"OBDRTData_EGRPosition\": \"%s\",", vtRTData[i].sOBDRTData.strEGRPosition.c_str());
			strJson.AppendFormat(L"\"OBDRTData_FuelDeliveryPressure\": \"%s\",", vtRTData[i].sOBDRTData.strFuelDeliveryPressure.c_str());

			strJson += L"},";
		}
		// 删除最后一个"，"
		if(vtRTData.size()>0)
		{
			strJson.Delete(strJson.GetLength()-1);
		}
		//for (std::list<REALTIMEDATAOFDIS>::iterator ir=listRealTimeDataOfDIS.begin(); ir!=listRealTimeDataOfDIS.end(); ir++)
		//{
		//	strJson += L"{";
		//	strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", ir->wchFuelTypeCode);
		//	strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", ir->wchFuelTypeName);
		//	strJson.AppendFormat(L"\"Time\": \"%s\",", ir->wchTime);
		//	strJson.AppendFormat(L"\"SamplingTime\": \"%s\",", ir->wchSamplingTime);
		//	strJson.AppendFormat(L"\"State\": \"%s\",", ir->wchState);
		//	strJson.AppendFormat(L"\"HC\": \"%s\",", ir->wchHC);
		//	strJson.AppendFormat(L"\"CO\": \"%.2f\",", _wtof(ir->wchCO));
		//	strJson.AppendFormat(L"\"O2\": \"%.2f\",", _wtof(ir->wchO2));
		//	strJson.AppendFormat(L"\"CO2\": \"%.2f\",", _wtof(ir->wchCO2));
		//	strJson.AppendFormat(L"\"NO\": \"%s\",", ir->wchNO);
		//	strJson.AppendFormat(L"\"EngineRev\": \"%s\",", ir->wchEngineRev);
		//	strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(ir->wchOilTemperature));
		//	strJson.AppendFormat(L"\"Lambda\": \"%.2f\",", _wtof(ir->wchLambda));
		//	strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(ir->wchEnvironmentalTemperature));
		//	strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(ir->wchRelativeHumidity));
		//	strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\"", _wtof(ir->wchAtmosphericPressure));
		//	strJson += L"},";
		//}
		//// 删除最后一个"，"
		//if(listRealTimeDataOfDIS.size()>0)
		//{
		//	strJson.Delete(strJson.GetLength()-1);
		//}
		strJson += L"]}";

		char *pch(NULL);
		pch = CNHCommonAPI::UnicodeToANSI(strJson);

		unsigned long nLen = strlen(pch);

		BYTE *szBuf = new BYTE[nLen]();
		int nRet = compress(szBuf, &nLen, (BYTE*)pch, nLen);

		ZBase64 zBase64;
		std::string strBufBase64 = zBase64.EncodeRaw(szBuf, nLen);

		DataTrans((BYTE*)strBufBase64.c_str(), strBufBase64.length());

		if(szBuf != NULL)
		{
			delete[] szBuf;
		}

		if (NULL != pch)
		{
			free(pch);
			pch = NULL;
		}
	}

	return true;
}

bool CParaThread::DT_FSUNHT(const TESTLOG &sTestLog, const SResultOfFSUNHT &sResultOfFSUNHT)
{
	if (IM_NANHUA_EPCS_4G==m_emIM && IsDBOpen())
	{
		CString strJson;

		strJson += L"{\"TestLog\":[{";
		strJson.AppendFormat(L"\"RunningNumber\": \"%s\",", sTestLog.wchRunningNumber);
		strJson.AppendFormat(L"\"ReportNumber\": \"%s\",", sTestLog.wchReportNumber);
		strJson.AppendFormat(L"\"BillDate\": \"%s\",", sTestLog.wchBillDate);
		strJson.AppendFormat(L"\"Biller\": \"%s\",", sTestLog.wchBiller);
		strJson.AppendFormat(L"\"Shroff\": \"%s\",", sTestLog.wchShroff);
		strJson.AppendFormat(L"\"ChargeState\": \"%s\",", sTestLog.wchChargeState);
		strJson.AppendFormat(L"\"InitialMoney\": \"%.2f\",", _wtof(sTestLog.wchInitialMoney));
		strJson.AppendFormat(L"\"ChangedMoney\": \"%.2f\",", _wtof(sTestLog.wchChangedMoney));
		strJson.AppendFormat(L"\"ReceivableMoney\": \"%.2f\",", _wtof(sTestLog.wchReceivableMoney));
		strJson.AppendFormat(L"\"ActualMoney\": \"%.2f\",", _wtof(sTestLog.wchActualMoney));
		strJson.AppendFormat(L"\"TestLocation\": \"%s\",", sTestLog.wchTestLocation);
		strJson.AppendFormat(L"\"TestState\": \"%s\",", sTestLog.wchTestState);
		strJson.AppendFormat(L"\"IsTestFinished\": \"%s\",", sTestLog.wchIsTestFinished);
		strJson.AppendFormat(L"\"NumberOfTestPeriodASM\": \"%s\",", sTestLog.wchNumberOfTestPeriodASM);
		strJson.AppendFormat(L"\"NumberOfTestPeriodVMAS\": \"%s\",", sTestLog.wchNumberOfTestPeriodVMAS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodLUGDOWN\": \"%s\",", sTestLog.wchNumberOfTestPeriodLUGDOWN);
		strJson.AppendFormat(L"\"NumberOfTestPeriodDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUNHT\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUNHT);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUYD\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUYD);
		strJson.AppendFormat(L"\"NumberOfTestPeriodAVFSU\": \"%s\",", sTestLog.wchNumberOfTestPeriodAVFSU);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMI\": \"%s\",", sTestLog.wchNumberOfTestPeriodMI);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodMDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriod\": \"%s\",", sTestLog.wchNumberOfTestPeriod);
		strJson.AppendFormat(L"\"TestPeriodUsed\": \"%s\",", sTestLog.wchTestPeriodUsed);
		strJson.AppendFormat(L"\"QuitReason\": \"%s\",", sTestLog.wchQuitReason);
		strJson.AppendFormat(L"\"TestType\": \"%s\",", sTestLog.wchTestType);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sTestLog.wchPass);
		strJson.AppendFormat(L"\"Operator\": \"%s\",", sTestLog.wchOperator);
		strJson.AppendFormat(L"\"Driver\": \"%s\",", sTestLog.wchDriver);
		strJson.AppendFormat(L"\"Assessor\": \"%s\",", sTestLog.wchAssessor);
		strJson.AppendFormat(L"\"IsRetest\": \"%s\",", sTestLog.wchIsRetest);
		//strJson.AppendFormat(L"\"EntryPCIP\": \"%s\",", sTestLog.wchEntryPCIP);
		strJson.AppendFormat(L"\"IsFuelTypeNeedTest\": \"%s\",", sTestLog.wchIsFuelTypeNeedTest);
		strJson.AppendFormat(L"\"IsFuelTypeTestFinished\": \"%s\",", sTestLog.wchIsFuelTypeTestFinished);
		strJson.AppendFormat(L"\"IsFuelTypeTestPass\": \"%s\",", sTestLog.wchIsFuelTypeTestPass);
		strJson.AppendFormat(L"\"IsFuelType2NeedTest\": \"%s\",", sTestLog.wchIsFuelType2NeedTest);
		strJson.AppendFormat(L"\"IsFuelType2TestFinished\": \"%s\",", sTestLog.wchIsFuelType2TestFinished);
		strJson.AppendFormat(L"\"IsFuelType2TestPass\": \"%s\",", sTestLog.wchIsFuelType2TestPass);
		strJson.AppendFormat(L"\"TestDate\": \"%s\",", sTestLog.wchTestDate);
		strJson.AppendFormat(L"\"TestStartTime\": \"%s\",", sTestLog.wchTestStartTime);
		strJson.AppendFormat(L"\"TestEndTime\": \"%s\",", sTestLog.wchTestEndTime);
		strJson.AppendFormat(L"\"TestCharacter\": \"%s\",", sTestLog.wchTestCharacter);
		//strJson.AppendFormat(L"\"AuditTimes\": \"%s\",", sTestLog.wchAuditTimes);
		//strJson.AppendFormat(L"\"RecentAuditTime\": \"%s\",", sTestLog.wchRecentAuditTime);
		strJson.AppendFormat(L"\"PrintTimes\": \"%s\",", sTestLog.wchPrintTimes);
		strJson.AppendFormat(L"\"RecentPrintReportTime\": \"%s\",", sTestLog.wchRecentPrintReportTime);
		//strJson.AppendFormat(L"\"PrintInvoiceTimes\": \"%s\",", sTestLog.wchPrintInvoiceTimes);
		//strJson.AppendFormat(L"\"RecentPrintInvoiceTime\": \"%s\",", sTestLog.wchRecentPrintInvoiceTime);
		//strJson.AppendFormat(L"\"PrintEPSignTimes\": \"%s\",", sTestLog.wchPrintEPSignTimes);
		//strJson.AppendFormat(L"\"RecentPrintEPSignTime\": \"%s\",", sTestLog.wchRecentPrintEPSignTime);
		//strJson.AppendFormat(L"\"PrintCertificateTimes\": \"%s\",", sTestLog.wchPrintCertificateTimes);
		//strJson.AppendFormat(L"\"RecentPrintCertificateTime\": \"%s\",", sTestLog.wchRecentPrintCertificateTime);
		//strJson.AppendFormat(L"\"IsTestValid\": \"%s\",", sTestLog.wchIsTestValid);
		strJson.AppendFormat(L"\"IsSent\": \"%s\",", sTestLog.wchIsSent);
		strJson.AppendFormat(L"\"IsSentEP\": \"%s\",", sTestLog.wchIsSentEP);
		strJson.AppendFormat(L"\"SRM\": \"%s\",", sTestLog.wchSRM);
		//strJson.AppendFormat(L"\"Harmony\": \"%s\",", sTestLog.wchHarmony);
		strJson.AppendFormat(L"\"PlateNumber\": \"%s\",", sTestLog.wchPlateNumber);
		strJson.AppendFormat(L"\"PlateType\": \"%s\",", sTestLog.wchPlateType);
		strJson.AppendFormat(L"\"RegistrationDate\": \"%s\",", sTestLog.wchRegistrationDate);
		strJson.AppendFormat(L"\"Owner\": \"%s\",", sTestLog.wchOwner);
		strJson.AppendFormat(L"\"Phone\": \"%s\",", sTestLog.wchPhone);
		strJson.AppendFormat(L"\"Address\": \"%s\",", sTestLog.wchAddress);
		strJson.AppendFormat(L"\"UseCharacter\": \"%s\",", sTestLog.wchUseCharacter);
		strJson.AppendFormat(L"\"Manufacturer\": \"%s\",", sTestLog.wchManufacturer);
		strJson.AppendFormat(L"\"Brand\": \"%s\",", sTestLog.wchBrand);
		strJson.AppendFormat(L"\"Model\": \"%s\",", sTestLog.wchModel);
		strJson.AppendFormat(L"\"ProductionDate\": \"%s\",", sTestLog.wchProductionDate);
		strJson.AppendFormat(L"\"VIN\": \"%s\",", sTestLog.wchVIN);
		strJson.AppendFormat(L"\"ChassisModel\": \"%s\",", sTestLog.wchChassisModel);
		strJson.AppendFormat(L"\"VehicleType\": \"%s\",", sTestLog.wchVehicleType);
		strJson.AppendFormat(L"\"MaximumTotalMass\": \"%s\",", sTestLog.wchMaximumTotalMass);
		strJson.AppendFormat(L"\"UnladenMass\": \"%s\",", sTestLog.wchUnladenMass);
		strJson.AppendFormat(L"\"AxleMass\": \"%s\",", sTestLog.wchAxleMass);
		strJson.AppendFormat(L"\"RatedLoadingMass\": \"%s\",", sTestLog.wchRatedLoadingMass);
		strJson.AppendFormat(L"\"RatedPassengerCapacity\": \"%s\",", sTestLog.wchRatedPassengerCapacity);
		strJson.AppendFormat(L"\"TyrePressure\": \"%.2f\",", _wtof(sTestLog.wchTyrePressure));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%s\",", sTestLog.wchTravelledDistance);
		strJson.AppendFormat(L"\"EngineManufacturer\": \"%s\",", sTestLog.wchEngineManufacturer);
		strJson.AppendFormat(L"\"EngineModel\": \"%s\",", sTestLog.wchEngineModel);
		strJson.AppendFormat(L"\"EngineNumber\": \"%s\",", sTestLog.wchEngineNumber);
		strJson.AppendFormat(L"\"EngineStroke\": \"%s\",", sTestLog.wchEngineStroke);
		strJson.AppendFormat(L"\"Displacement\": \"%.2f\",", _wtof(sTestLog.wchDisplacement));
		strJson.AppendFormat(L"\"NumberOfCylinder\": \"%s\",", sTestLog.wchNumberOfCylinder);
		strJson.AppendFormat(L"\"OilSupplyMode\": \"%s\",", sTestLog.wchOilSupplyMode);
		strJson.AppendFormat(L"\"AirIntakeMode\": \"%s\",", sTestLog.wchAirIntakeMode);
		strJson.AppendFormat(L"\"HasCatalyticConverter\": \"%s\",", sTestLog.wchHasCatalyticConverter);
		strJson.AppendFormat(L"\"FuelType\": \"%s\",", sTestLog.wchFuelType);
		strJson.AppendFormat(L"\"FuelMark\": \"%s\",", sTestLog.wchFuelMark);
		strJson.AppendFormat(L"\"DoubleFuel\": \"%s\",", sTestLog.wchDoubleFuel);
		strJson.AppendFormat(L"\"FuelType2\": \"%s\",", sTestLog.wchFuelType2);
		strJson.AppendFormat(L"\"FuelMark2\": \"%s\",", sTestLog.wchFuelMark2);
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sTestLog.wchRatedRev);
		strJson.AppendFormat(L"\"RatedPower\": \"%.2f\",", _wtof(sTestLog.wchRatedPower));
		strJson.AppendFormat(L"\"MaximumNetPower\": \"%.2f\",", _wtof(sTestLog.wchMaximumNetPower));
		strJson.AppendFormat(L"\"GearBoxType\": \"%s\",", sTestLog.wchGearBoxType);
		strJson.AppendFormat(L"\"NumberOfGears\": \"%s\",", sTestLog.wchNumberOfGears);
		strJson.AppendFormat(L"\"DriveType\": \"%s\",", sTestLog.wchDriveType);
		strJson.AppendFormat(L"\"EPSign\": \"%s\",", sTestLog.wchEPSign);
		strJson.AppendFormat(L"\"CertificateNumber\": \"%s\",", sTestLog.wchCertificateNumber);
		strJson.AppendFormat(L"\"IndependentDoubleExhaustPipe\": \"%s\",", sTestLog.wchIndependentDoubleExhaustPipe);
		strJson.AppendFormat(L"\"TCS\": \"%s\",", sTestLog.wchTCS);
		strJson.AppendFormat(L"\"FuelPumpMode\": \"%s\",", sTestLog.wchFuelPumpMode);
		strJson.AppendFormat(L"\"PassengerVehicle\": \"%s\",", sTestLog.wchPassengerVehicle);
		strJson.AppendFormat(L"\"EmissionStandard\": \"%s\",", sTestLog.wchEmissionStandard);
		strJson.AppendFormat(L"\"AnnualTestPeriod\": \"%s\",", sTestLog.wchAnnualTestPeriod);
		strJson.AppendFormat(L"\"HasOBD\": \"%s\",", sTestLog.wchHasOBD);
		strJson.AppendFormat(L"\"OwnerCharacter\": \"%s\",", sTestLog.wchOwnerCharacter);
		strJson.AppendFormat(L"\"OwnerDistrict\": \"%s\",", sTestLog.wchOwnerDistrict);
		strJson.AppendFormat(L"\"LicenseDepartment\": \"%s\",", sTestLog.wchLicenseDepartment);
		strJson.AppendFormat(L"\"LicenseIssueDate\": \"%s\",", sTestLog.wchLicenseIssueDate);
		strJson.AppendFormat(L"\"LicenseValidDate\": \"%s\"", sTestLog.wchLicenseValidDate);

		strJson.AppendFormat(L"\"LineNumber\": \"%s\"", sTestLog.wchLineNumber);
		strJson.AppendFormat(L"\"LineModel\": \"%s\"", sTestLog.wchLineModel);
		strJson.AppendFormat(L"\"LineName\": \"%s\"", sTestLog.wchLineName);
		strJson.AppendFormat(L"\"LineManufacturer\": \"%s\"", sTestLog.wchLineManufacturer);
		strJson.AppendFormat(L"\"LineLicenseSn\": \"%s\"", sTestLog.wchLineLicenseSn);
		strJson.AppendFormat(L"\"LineLicenseStartDate\": \"%s\"", sTestLog.wchLineLicenseStartDate);
		strJson.AppendFormat(L"\"LineLicenseEndDate\": \"%s\"", sTestLog.wchLineLicenseEndDate);
		strJson.AppendFormat(L"\"LineIsLocked\": \"%s\"", sTestLog.wchLineIsLocked);
		strJson.AppendFormat(L"\"LineRegistrationInfo\": \"%s\"", sTestLog.wchLineRegistrationInfo);
		strJson.AppendFormat(L"\"LineIPAddress\": \"%s\"", sTestLog.wchLineIPAddress);
		strJson.AppendFormat(L"\"LineHardwareSN\": \"%s\"", sTestLog.wchLineHardwareSN);
		strJson.AppendFormat(L"\"HasDynamometer\": \"%s\"", sTestLog.wchHasDynamometer);
		strJson.AppendFormat(L"\"DynamometerNumber\": \"%s\"", sTestLog.wchDynamometerNumber);
		strJson.AppendFormat(L"\"DynamometerModel\": \"%s\"", sTestLog.wchDynamometerModel);
		strJson.AppendFormat(L"\"DynamometerManufacturer\": \"%s\"", sTestLog.wchDynamometerManufacturer);
		strJson.AppendFormat(L"\"DynamometerCoastDownDate\": \"%s\"", sTestLog.wchDynamometerCoastDownDate);
		strJson.AppendFormat(L"\"DynamometerPLHPDate\": \"%s\"", sTestLog.wchDynamometerPLHPDate);
		strJson.AppendFormat(L"\"DynamometerForceCalibrationDate\": \"%s\"", sTestLog.wchDynamometerForceCalibrationDate);
		strJson.AppendFormat(L"\"DynamometerForceCheckDate\": \"%s\"", sTestLog.wchDynamometerForceCheckDate);
		strJson.AppendFormat(L"\"DynamometerAuthenticationDate\": \"%s\"", sTestLog.wchDynamometerAuthenticationDate);
		strJson.AppendFormat(L"\"HasAnalyzer\": \"%s\"", sTestLog.wchHasAnalyzer);
		strJson.AppendFormat(L"\"AnalyzerNumber\": \"%s\"", sTestLog.wchAnalyzerNumber);
		strJson.AppendFormat(L"\"AnalyzerModel\": \"%s\"", sTestLog.wchAnalyzerModel);
		strJson.AppendFormat(L"\"AnalyzerManufacturer\": \"%s\"", sTestLog.wchAnalyzerManufacturer);
		strJson.AppendFormat(L"\"AnalyzerChannelCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerChannelCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerChannelCheckDate\": \"%s\"", sTestLog.wchAnalyzerChannelCheckDate);
		strJson.AppendFormat(L"\"AnalyzerEnvParamCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerAuthenticationDate\": \"%s\"", sTestLog.wchAnalyzerAuthenticationDate);
		strJson.AppendFormat(L"\"HasFlowmeter\": \"%s\"", sTestLog.wchHasFlowmeter);
		strJson.AppendFormat(L"\"FlowmeterNumber\": \"%s\"", sTestLog.wchFlowmeterNumber);
		strJson.AppendFormat(L"\"FlowmeterModel\": \"%s\"", sTestLog.wchFlowmeterModel);
		strJson.AppendFormat(L"\"FlowmeterManufacturer\": \"%s\"", sTestLog.wchFlowmeterManufacturer);
		strJson.AppendFormat(L"\"FlowmeterFluxCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterFluxCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterFluxCheckDate\": \"%s\"", sTestLog.wchFlowmeterFluxCheckDate);
		strJson.AppendFormat(L"\"FlowmeterDilO2CalibrationDate\": \"%s\"", sTestLog.wchFlowmeterDilO2CalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterPressureCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterPressureCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterAuthenticationDate\": \"%s\"", sTestLog.wchFlowmeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasOpacimeter\": \"%s\"", sTestLog.wchHasOpacimeter);
		strJson.AppendFormat(L"\"OpacimeterNumber\": \"%s\"", sTestLog.wchOpacimeterNumber);
		strJson.AppendFormat(L"\"OpacimeterModel\": \"%s\"", sTestLog.wchOpacimeterModel);
		strJson.AppendFormat(L"\"OpacimeterManufacturer\": \"%s\"", sTestLog.wchOpacimeterManufacturer);
		strJson.AppendFormat(L"\"OpacimeterChannelCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterChannelCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterChannelCheckDate\": \"%s\"", sTestLog.wchOpacimeterChannelCheckDate);
		strJson.AppendFormat(L"\"OpacimeterEnvParamCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterAuthenticationDate\": \"%s\"", sTestLog.wchOpacimeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasSmokemeter\": \"%s\"", sTestLog.wchHasSmokemeter);
		strJson.AppendFormat(L"\"SmokemeterNumber\": \"%s\"", sTestLog.wchSmokemeterNumber);
		strJson.AppendFormat(L"\"SmokemeterModel\": \"%s\"", sTestLog.wchSmokemeterModel);
		strJson.AppendFormat(L"\"SmokemeterManufacturer\": \"%s\"", sTestLog.wchSmokemeterManufacturer);
		strJson.AppendFormat(L"\"SmokemeterCalibrationDate\": \"%s\"", sTestLog.wchSmokemeterCalibrationDate);
		strJson.AppendFormat(L"\"SmokemeterCheckDate\": \"%s\"", sTestLog.wchSmokemeterCheckDate);
		strJson.AppendFormat(L"\"SmokemeterAuthenticationDate\": \"%s\"", sTestLog.wchSmokemeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentEnvParamMeasurer\": \"%s\"", sTestLog.wchHasIndependentEnvParamMeasurer);
		strJson.AppendFormat(L"\"EnvParamMeasurerNumber\": \"%s\"", sTestLog.wchEnvParamMeasurerNumber);
		strJson.AppendFormat(L"\"EnvParamMeasurerModel\": \"%s\"", sTestLog.wchEnvParamMeasurerModel);
		strJson.AppendFormat(L"\"EnvParamMeasurerManufacturer\": \"%s\"", sTestLog.wchEnvParamMeasurerManufacturer);
		strJson.AppendFormat(L"\"EnvParamMeasurerCalibrationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCalibrationDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerCheckDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCheckDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerAuthenticationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentTachometer\": \"%s\"", sTestLog.wchHasIndependentTachometer);
		strJson.AppendFormat(L"\"TachometerNumber\": \"%s\"", sTestLog.wchTachometerNumber);
		strJson.AppendFormat(L"\"TachometerModel\": \"%s\"", sTestLog.wchTachometerModel);
		strJson.AppendFormat(L"\"TachometerManufacturer\": \"%s\"", sTestLog.wchTachometerManufacturer);
		strJson.AppendFormat(L"\"TachometerCalibrationDate\": \"%s\"", sTestLog.wchTachometerCalibrationDate);
		strJson.AppendFormat(L"\"TachometerCheckDate\": \"%s\"", sTestLog.wchTachometerCheckDate);
		strJson.AppendFormat(L"\"TachometerAuthenticationDate\": \"%s\"", sTestLog.wchTachometerAuthenticationDate);
		strJson.AppendFormat(L"\"StationNumber\": \"%s\"", sTestLog.wchStationNumber);
		strJson.AppendFormat(L"\"StationFullName\": \"%s\"", sTestLog.wchStationFullName);
		strJson.AppendFormat(L"\"StationShortName\": \"%s\"", sTestLog.wchStationShortName);
		strJson.AppendFormat(L"\"StationLicenseSn\": \"%s\"", sTestLog.wchStationLicenseSn);
		strJson.AppendFormat(L"\"StationLicenseStartDate\": \"%s\"", sTestLog.wchStationLicenseStartDate);
		strJson.AppendFormat(L"\"StationLicenseEndDate\": \"%s\"", sTestLog.wchStationLicenseEndDate);
		strJson.AppendFormat(L"\"StationIsLocked\": \"%s\"", sTestLog.wchStationIsLocked);
		strJson.AppendFormat(L"\"StationRegistrationInfo\": \"%s\"", sTestLog.wchStationRegistrationInfo);
		strJson.AppendFormat(L"\"StationPhone\": \"%s\"", sTestLog.wchStationPhone);
		strJson.AppendFormat(L"\"StationFax\": \"%s\"", sTestLog.wchStationFax);
		strJson.AppendFormat(L"\"PostCode\": \"%s\"", sTestLog.wchPostCode);
		strJson.AppendFormat(L"\"Province\": \"%s\"", sTestLog.wchProvince);
		strJson.AppendFormat(L"\"City\": \"%s\"", sTestLog.wchCity);
		strJson.AppendFormat(L"\"District\": \"%s\"", sTestLog.wchDistrict);
		strJson.AppendFormat(L"\"DetailedAddress\": \"%s\"", sTestLog.wchDetailedAddress);

		strJson += L"}],";


		strJson += L"\"ResultOfFSUNHT\":[{";
		strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", sResultOfFSUNHT.strFuelTypeCode.c_str());
		strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", sResultOfFSUNHT.strFuelTypeName.c_str());
		strJson.AppendFormat(L"\"EngineRev\": \"%s\",", sResultOfFSUNHT.strEngineRev.c_str());
		strJson.AppendFormat(L"\"Value1\": \"%.2f\",", _wtof(sResultOfFSUNHT.strValue1.c_str()));
		strJson.AppendFormat(L"\"Value2\": \"%.2f\",", _wtof(sResultOfFSUNHT.strValue2.c_str()));
		strJson.AppendFormat(L"\"Value3\": \"%.2f\",", _wtof(sResultOfFSUNHT.strValue3.c_str()));
		strJson.AppendFormat(L"\"Value4\": \"%.2f\",", _wtof(sResultOfFSUNHT.strValue4.c_str()));
		strJson.AppendFormat(L"\"Average\": \"%.2f\",", _wtof(sResultOfFSUNHT.strAverage.c_str()));
		strJson.AppendFormat(L"\"Limit\": \"%.2f\",", _wtof(sResultOfFSUNHT.strLimit.c_str()));
		strJson.AppendFormat(L"\"Limit_EDP\": \"%s\",", sResultOfFSUNHT.strLimit_EDP.c_str());
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sResultOfFSUNHT.strPass.c_str());
		strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(sResultOfFSUNHT.strEnvironmentalTemperature.c_str()));
		strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(sResultOfFSUNHT.strRelativeHumidity.c_str()));
		strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(sResultOfFSUNHT.strAtmosphericPressure.c_str()));
		strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(sResultOfFSUNHT.strOilTemperature.c_str()));
		strJson.AppendFormat(L"\"SkipRev\": \"%s\",", sResultOfFSUNHT.strSkipRev.c_str());
		strJson.AppendFormat(L"\"SkipOilTemperature\": \"%s\"", sResultOfFSUNHT.strSkipOilTemperature.c_str());
		strJson += L"}]}";

		char *pch(NULL);
		pch = CNHCommonAPI::UnicodeToANSI(strJson);

		unsigned long nLen = strlen(pch);

		BYTE *szBuf = new BYTE[nLen]();
		int nRet = compress(szBuf, &nLen, (BYTE*)pch, nLen);

		ZBase64 zBase64;
		std::string strBufBase64 = zBase64.EncodeRaw(szBuf, nLen);

		DataTrans((BYTE*)strBufBase64.c_str(), strBufBase64.length());

		if(szBuf != NULL)
		{
			delete[] szBuf;
		}

		if (NULL != pch)
		{
			free(pch);
			pch = NULL;
		}
	}

	return true;
}

bool CParaThread::DT_FSUYD(const TESTLOG &sTestLog, const RESULTOFFSUYD &sResultOfFSUYD)
{
	if (IM_NANHUA_EPCS_4G==m_emIM && IsDBOpen())
	{
		CString strJson;

		strJson += L"{\"TestLog\":[{";

		strJson.AppendFormat(L"\"RunningNumber\": \"%s\",", sTestLog.wchRunningNumber);
		strJson.AppendFormat(L"\"ReportNumber\": \"%s\",", sTestLog.wchReportNumber);
		strJson.AppendFormat(L"\"BillDate\": \"%s\",", sTestLog.wchBillDate);
		strJson.AppendFormat(L"\"Biller\": \"%s\",", sTestLog.wchBiller);
		strJson.AppendFormat(L"\"Shroff\": \"%s\",", sTestLog.wchShroff);
		strJson.AppendFormat(L"\"ChargeState\": \"%s\",", sTestLog.wchChargeState);
		strJson.AppendFormat(L"\"InitialMoney\": \"%.2f\",", _wtof(sTestLog.wchInitialMoney));
		strJson.AppendFormat(L"\"ChangedMoney\": \"%.2f\",", _wtof(sTestLog.wchChangedMoney));
		strJson.AppendFormat(L"\"ReceivableMoney\": \"%.2f\",", _wtof(sTestLog.wchReceivableMoney));
		strJson.AppendFormat(L"\"ActualMoney\": \"%.2f\",", _wtof(sTestLog.wchActualMoney));
		strJson.AppendFormat(L"\"TestLocation\": \"%s\",", sTestLog.wchTestLocation);
		strJson.AppendFormat(L"\"TestState\": \"%s\",", sTestLog.wchTestState);
		strJson.AppendFormat(L"\"IsTestFinished\": \"%s\",", sTestLog.wchIsTestFinished);
		strJson.AppendFormat(L"\"NumberOfTestPeriodASM\": \"%s\",", sTestLog.wchNumberOfTestPeriodASM);
		strJson.AppendFormat(L"\"NumberOfTestPeriodVMAS\": \"%s\",", sTestLog.wchNumberOfTestPeriodVMAS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodLUGDOWN\": \"%s\",", sTestLog.wchNumberOfTestPeriodLUGDOWN);
		strJson.AppendFormat(L"\"NumberOfTestPeriodDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUNHT\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUNHT);
		strJson.AppendFormat(L"\"NumberOfTestPeriodFSUYD\": \"%s\",", sTestLog.wchNumberOfTestPeriodFSUYD);
		strJson.AppendFormat(L"\"NumberOfTestPeriodAVFSU\": \"%s\",", sTestLog.wchNumberOfTestPeriodAVFSU);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMI\": \"%s\",", sTestLog.wchNumberOfTestPeriodMI);
		strJson.AppendFormat(L"\"NumberOfTestPeriodMDIS\": \"%s\",", sTestLog.wchNumberOfTestPeriodMDIS);
		strJson.AppendFormat(L"\"NumberOfTestPeriod\": \"%s\",", sTestLog.wchNumberOfTestPeriod);
		strJson.AppendFormat(L"\"TestPeriodUsed\": \"%s\",", sTestLog.wchTestPeriodUsed);
		strJson.AppendFormat(L"\"QuitReason\": \"%s\",", sTestLog.wchQuitReason);
		strJson.AppendFormat(L"\"TestType\": \"%s\",", sTestLog.wchTestType);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sTestLog.wchPass);
		strJson.AppendFormat(L"\"Operator\": \"%s\",", sTestLog.wchOperator);
		strJson.AppendFormat(L"\"Driver\": \"%s\",", sTestLog.wchDriver);
		strJson.AppendFormat(L"\"Assessor\": \"%s\",", sTestLog.wchAssessor);
		strJson.AppendFormat(L"\"IsRetest\": \"%s\",", sTestLog.wchIsRetest);
		//strJson.AppendFormat(L"\"EntryPCIP\": \"%s\",", sTestLog.wchEntryPCIP);
		strJson.AppendFormat(L"\"IsFuelTypeNeedTest\": \"%s\",", sTestLog.wchIsFuelTypeNeedTest);
		strJson.AppendFormat(L"\"IsFuelTypeTestFinished\": \"%s\",", sTestLog.wchIsFuelTypeTestFinished);
		strJson.AppendFormat(L"\"IsFuelTypeTestPass\": \"%s\",", sTestLog.wchIsFuelTypeTestPass);
		strJson.AppendFormat(L"\"IsFuelType2NeedTest\": \"%s\",", sTestLog.wchIsFuelType2NeedTest);
		strJson.AppendFormat(L"\"IsFuelType2TestFinished\": \"%s\",", sTestLog.wchIsFuelType2TestFinished);
		strJson.AppendFormat(L"\"IsFuelType2TestPass\": \"%s\",", sTestLog.wchIsFuelType2TestPass);
		strJson.AppendFormat(L"\"TestDate\": \"%s\",", sTestLog.wchTestDate);
		strJson.AppendFormat(L"\"TestStartTime\": \"%s\",", sTestLog.wchTestStartTime);
		strJson.AppendFormat(L"\"TestEndTime\": \"%s\",", sTestLog.wchTestEndTime);
		strJson.AppendFormat(L"\"TestCharacter\": \"%s\",", sTestLog.wchTestCharacter);
		//strJson.AppendFormat(L"\"AuditTimes\": \"%s\",", sTestLog.wchAuditTimes);
		//strJson.AppendFormat(L"\"RecentAuditTime\": \"%s\",", sTestLog.wchRecentAuditTime);
		strJson.AppendFormat(L"\"PrintTimes\": \"%s\",", sTestLog.wchPrintTimes);
		strJson.AppendFormat(L"\"RecentPrintReportTime\": \"%s\",", sTestLog.wchRecentPrintReportTime);
		//strJson.AppendFormat(L"\"PrintInvoiceTimes\": \"%s\",", sTestLog.wchPrintInvoiceTimes);
		//strJson.AppendFormat(L"\"RecentPrintInvoiceTime\": \"%s\",", sTestLog.wchRecentPrintInvoiceTime);
		//strJson.AppendFormat(L"\"PrintEPSignTimes\": \"%s\",", sTestLog.wchPrintEPSignTimes);
		//strJson.AppendFormat(L"\"RecentPrintEPSignTime\": \"%s\",", sTestLog.wchRecentPrintEPSignTime);
		//strJson.AppendFormat(L"\"PrintCertificateTimes\": \"%s\",", sTestLog.wchPrintCertificateTimes);
		//strJson.AppendFormat(L"\"RecentPrintCertificateTime\": \"%s\",", sTestLog.wchRecentPrintCertificateTime);
		//strJson.AppendFormat(L"\"IsTestValid\": \"%s\",", sTestLog.wchIsTestValid);
		strJson.AppendFormat(L"\"IsSent\": \"%s\",", sTestLog.wchIsSent);
		strJson.AppendFormat(L"\"IsSentEP\": \"%s\",", sTestLog.wchIsSentEP);
		strJson.AppendFormat(L"\"SRM\": \"%s\",", sTestLog.wchSRM);
		//strJson.AppendFormat(L"\"Harmony\": \"%s\",", sTestLog.wchHarmony);
		strJson.AppendFormat(L"\"PlateNumber\": \"%s\",", sTestLog.wchPlateNumber);
		strJson.AppendFormat(L"\"PlateType\": \"%s\",", sTestLog.wchPlateType);
		strJson.AppendFormat(L"\"RegistrationDate\": \"%s\",", sTestLog.wchRegistrationDate);
		strJson.AppendFormat(L"\"Owner\": \"%s\",", sTestLog.wchOwner);
		strJson.AppendFormat(L"\"Phone\": \"%s\",", sTestLog.wchPhone);
		strJson.AppendFormat(L"\"Address\": \"%s\",", sTestLog.wchAddress);
		strJson.AppendFormat(L"\"UseCharacter\": \"%s\",", sTestLog.wchUseCharacter);
		strJson.AppendFormat(L"\"Manufacturer\": \"%s\",", sTestLog.wchManufacturer);
		strJson.AppendFormat(L"\"Brand\": \"%s\",", sTestLog.wchBrand);
		strJson.AppendFormat(L"\"Model\": \"%s\",", sTestLog.wchModel);
		strJson.AppendFormat(L"\"ProductionDate\": \"%s\",", sTestLog.wchProductionDate);
		strJson.AppendFormat(L"\"VIN\": \"%s\",", sTestLog.wchVIN);
		strJson.AppendFormat(L"\"ChassisModel\": \"%s\",", sTestLog.wchChassisModel);
		strJson.AppendFormat(L"\"VehicleType\": \"%s\",", sTestLog.wchVehicleType);
		strJson.AppendFormat(L"\"MaximumTotalMass\": \"%s\",", sTestLog.wchMaximumTotalMass);
		strJson.AppendFormat(L"\"UnladenMass\": \"%s\",", sTestLog.wchUnladenMass);
		strJson.AppendFormat(L"\"AxleMass\": \"%s\",", sTestLog.wchAxleMass);
		strJson.AppendFormat(L"\"RatedLoadingMass\": \"%s\",", sTestLog.wchRatedLoadingMass);
		strJson.AppendFormat(L"\"RatedPassengerCapacity\": \"%s\",", sTestLog.wchRatedPassengerCapacity);
		strJson.AppendFormat(L"\"TyrePressure\": \"%.2f\",", _wtof(sTestLog.wchTyrePressure));
		strJson.AppendFormat(L"\"TravelledDistance\": \"%s\",", sTestLog.wchTravelledDistance);
		strJson.AppendFormat(L"\"EngineManufacturer\": \"%s\",", sTestLog.wchEngineManufacturer);
		strJson.AppendFormat(L"\"EngineModel\": \"%s\",", sTestLog.wchEngineModel);
		strJson.AppendFormat(L"\"EngineNumber\": \"%s\",", sTestLog.wchEngineNumber);
		strJson.AppendFormat(L"\"EngineStroke\": \"%s\",", sTestLog.wchEngineStroke);
		strJson.AppendFormat(L"\"Displacement\": \"%.2f\",", _wtof(sTestLog.wchDisplacement));
		strJson.AppendFormat(L"\"NumberOfCylinder\": \"%s\",", sTestLog.wchNumberOfCylinder);
		strJson.AppendFormat(L"\"OilSupplyMode\": \"%s\",", sTestLog.wchOilSupplyMode);
		strJson.AppendFormat(L"\"AirIntakeMode\": \"%s\",", sTestLog.wchAirIntakeMode);
		strJson.AppendFormat(L"\"HasCatalyticConverter\": \"%s\",", sTestLog.wchHasCatalyticConverter);
		strJson.AppendFormat(L"\"FuelType\": \"%s\",", sTestLog.wchFuelType);
		strJson.AppendFormat(L"\"FuelMark\": \"%s\",", sTestLog.wchFuelMark);
		strJson.AppendFormat(L"\"DoubleFuel\": \"%s\",", sTestLog.wchDoubleFuel);
		strJson.AppendFormat(L"\"FuelType2\": \"%s\",", sTestLog.wchFuelType2);
		strJson.AppendFormat(L"\"FuelMark2\": \"%s\",", sTestLog.wchFuelMark2);
		strJson.AppendFormat(L"\"RatedRev\": \"%s\",", sTestLog.wchRatedRev);
		strJson.AppendFormat(L"\"RatedPower\": \"%.2f\",", _wtof(sTestLog.wchRatedPower));
		strJson.AppendFormat(L"\"MaximumNetPower\": \"%.2f\",", _wtof(sTestLog.wchMaximumNetPower));
		strJson.AppendFormat(L"\"GearBoxType\": \"%s\",", sTestLog.wchGearBoxType);
		strJson.AppendFormat(L"\"NumberOfGears\": \"%s\",", sTestLog.wchNumberOfGears);
		strJson.AppendFormat(L"\"DriveType\": \"%s\",", sTestLog.wchDriveType);
		strJson.AppendFormat(L"\"EPSign\": \"%s\",", sTestLog.wchEPSign);
		strJson.AppendFormat(L"\"CertificateNumber\": \"%s\",", sTestLog.wchCertificateNumber);
		strJson.AppendFormat(L"\"IndependentDoubleExhaustPipe\": \"%s\",", sTestLog.wchIndependentDoubleExhaustPipe);
		strJson.AppendFormat(L"\"TCS\": \"%s\",", sTestLog.wchTCS);
		strJson.AppendFormat(L"\"FuelPumpMode\": \"%s\",", sTestLog.wchFuelPumpMode);
		strJson.AppendFormat(L"\"PassengerVehicle\": \"%s\",", sTestLog.wchPassengerVehicle);
		strJson.AppendFormat(L"\"EmissionStandard\": \"%s\",", sTestLog.wchEmissionStandard);
		strJson.AppendFormat(L"\"AnnualTestPeriod\": \"%s\",", sTestLog.wchAnnualTestPeriod);
		strJson.AppendFormat(L"\"HasOBD\": \"%s\",", sTestLog.wchHasOBD);
		strJson.AppendFormat(L"\"OwnerCharacter\": \"%s\",", sTestLog.wchOwnerCharacter);
		strJson.AppendFormat(L"\"OwnerDistrict\": \"%s\",", sTestLog.wchOwnerDistrict);
		strJson.AppendFormat(L"\"LicenseDepartment\": \"%s\",", sTestLog.wchLicenseDepartment);
		strJson.AppendFormat(L"\"LicenseIssueDate\": \"%s\",", sTestLog.wchLicenseIssueDate);
		strJson.AppendFormat(L"\"LicenseValidDate\": \"%s\"", sTestLog.wchLicenseValidDate);

		strJson.AppendFormat(L"\"LineNumber\": \"%s\"", sTestLog.wchLineNumber);
		strJson.AppendFormat(L"\"LineModel\": \"%s\"", sTestLog.wchLineModel);
		strJson.AppendFormat(L"\"LineName\": \"%s\"", sTestLog.wchLineName);
		strJson.AppendFormat(L"\"LineManufacturer\": \"%s\"", sTestLog.wchLineManufacturer);
		strJson.AppendFormat(L"\"LineLicenseSn\": \"%s\"", sTestLog.wchLineLicenseSn);
		strJson.AppendFormat(L"\"LineLicenseStartDate\": \"%s\"", sTestLog.wchLineLicenseStartDate);
		strJson.AppendFormat(L"\"LineLicenseEndDate\": \"%s\"", sTestLog.wchLineLicenseEndDate);
		strJson.AppendFormat(L"\"LineIsLocked\": \"%s\"", sTestLog.wchLineIsLocked);
		strJson.AppendFormat(L"\"LineRegistrationInfo\": \"%s\"", sTestLog.wchLineRegistrationInfo);
		strJson.AppendFormat(L"\"LineIPAddress\": \"%s\"", sTestLog.wchLineIPAddress);
		strJson.AppendFormat(L"\"LineHardwareSN\": \"%s\"", sTestLog.wchLineHardwareSN);
		strJson.AppendFormat(L"\"HasDynamometer\": \"%s\"", sTestLog.wchHasDynamometer);
		strJson.AppendFormat(L"\"DynamometerNumber\": \"%s\"", sTestLog.wchDynamometerNumber);
		strJson.AppendFormat(L"\"DynamometerModel\": \"%s\"", sTestLog.wchDynamometerModel);
		strJson.AppendFormat(L"\"DynamometerManufacturer\": \"%s\"", sTestLog.wchDynamometerManufacturer);
		strJson.AppendFormat(L"\"DynamometerCoastDownDate\": \"%s\"", sTestLog.wchDynamometerCoastDownDate);
		strJson.AppendFormat(L"\"DynamometerPLHPDate\": \"%s\"", sTestLog.wchDynamometerPLHPDate);
		strJson.AppendFormat(L"\"DynamometerForceCalibrationDate\": \"%s\"", sTestLog.wchDynamometerForceCalibrationDate);
		strJson.AppendFormat(L"\"DynamometerForceCheckDate\": \"%s\"", sTestLog.wchDynamometerForceCheckDate);
		strJson.AppendFormat(L"\"DynamometerAuthenticationDate\": \"%s\"", sTestLog.wchDynamometerAuthenticationDate);
		strJson.AppendFormat(L"\"HasAnalyzer\": \"%s\"", sTestLog.wchHasAnalyzer);
		strJson.AppendFormat(L"\"AnalyzerNumber\": \"%s\"", sTestLog.wchAnalyzerNumber);
		strJson.AppendFormat(L"\"AnalyzerModel\": \"%s\"", sTestLog.wchAnalyzerModel);
		strJson.AppendFormat(L"\"AnalyzerManufacturer\": \"%s\"", sTestLog.wchAnalyzerManufacturer);
		strJson.AppendFormat(L"\"AnalyzerChannelCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerChannelCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerChannelCheckDate\": \"%s\"", sTestLog.wchAnalyzerChannelCheckDate);
		strJson.AppendFormat(L"\"AnalyzerEnvParamCalibrationDate\": \"%s\"", sTestLog.wchAnalyzerEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"AnalyzerAuthenticationDate\": \"%s\"", sTestLog.wchAnalyzerAuthenticationDate);
		strJson.AppendFormat(L"\"HasFlowmeter\": \"%s\"", sTestLog.wchHasFlowmeter);
		strJson.AppendFormat(L"\"FlowmeterNumber\": \"%s\"", sTestLog.wchFlowmeterNumber);
		strJson.AppendFormat(L"\"FlowmeterModel\": \"%s\"", sTestLog.wchFlowmeterModel);
		strJson.AppendFormat(L"\"FlowmeterManufacturer\": \"%s\"", sTestLog.wchFlowmeterManufacturer);
		strJson.AppendFormat(L"\"FlowmeterFluxCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterFluxCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterFluxCheckDate\": \"%s\"", sTestLog.wchFlowmeterFluxCheckDate);
		strJson.AppendFormat(L"\"FlowmeterDilO2CalibrationDate\": \"%s\"", sTestLog.wchFlowmeterDilO2CalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterPressureCalibrationDate\": \"%s\"", sTestLog.wchFlowmeterPressureCalibrationDate);
		strJson.AppendFormat(L"\"FlowmeterAuthenticationDate\": \"%s\"", sTestLog.wchFlowmeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasOpacimeter\": \"%s\"", sTestLog.wchHasOpacimeter);
		strJson.AppendFormat(L"\"OpacimeterNumber\": \"%s\"", sTestLog.wchOpacimeterNumber);
		strJson.AppendFormat(L"\"OpacimeterModel\": \"%s\"", sTestLog.wchOpacimeterModel);
		strJson.AppendFormat(L"\"OpacimeterManufacturer\": \"%s\"", sTestLog.wchOpacimeterManufacturer);
		strJson.AppendFormat(L"\"OpacimeterChannelCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterChannelCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterChannelCheckDate\": \"%s\"", sTestLog.wchOpacimeterChannelCheckDate);
		strJson.AppendFormat(L"\"OpacimeterEnvParamCalibrationDate\": \"%s\"", sTestLog.wchOpacimeterEnvParamCalibrationDate);
		strJson.AppendFormat(L"\"OpacimeterAuthenticationDate\": \"%s\"", sTestLog.wchOpacimeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasSmokemeter\": \"%s\"", sTestLog.wchHasSmokemeter);
		strJson.AppendFormat(L"\"SmokemeterNumber\": \"%s\"", sTestLog.wchSmokemeterNumber);
		strJson.AppendFormat(L"\"SmokemeterModel\": \"%s\"", sTestLog.wchSmokemeterModel);
		strJson.AppendFormat(L"\"SmokemeterManufacturer\": \"%s\"", sTestLog.wchSmokemeterManufacturer);
		strJson.AppendFormat(L"\"SmokemeterCalibrationDate\": \"%s\"", sTestLog.wchSmokemeterCalibrationDate);
		strJson.AppendFormat(L"\"SmokemeterCheckDate\": \"%s\"", sTestLog.wchSmokemeterCheckDate);
		strJson.AppendFormat(L"\"SmokemeterAuthenticationDate\": \"%s\"", sTestLog.wchSmokemeterAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentEnvParamMeasurer\": \"%s\"", sTestLog.wchHasIndependentEnvParamMeasurer);
		strJson.AppendFormat(L"\"EnvParamMeasurerNumber\": \"%s\"", sTestLog.wchEnvParamMeasurerNumber);
		strJson.AppendFormat(L"\"EnvParamMeasurerModel\": \"%s\"", sTestLog.wchEnvParamMeasurerModel);
		strJson.AppendFormat(L"\"EnvParamMeasurerManufacturer\": \"%s\"", sTestLog.wchEnvParamMeasurerManufacturer);
		strJson.AppendFormat(L"\"EnvParamMeasurerCalibrationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCalibrationDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerCheckDate\": \"%s\"", sTestLog.wchEnvParamMeasurerCheckDate);
		strJson.AppendFormat(L"\"EnvParamMeasurerAuthenticationDate\": \"%s\"", sTestLog.wchEnvParamMeasurerAuthenticationDate);
		strJson.AppendFormat(L"\"HasIndependentTachometer\": \"%s\"", sTestLog.wchHasIndependentTachometer);
		strJson.AppendFormat(L"\"TachometerNumber\": \"%s\"", sTestLog.wchTachometerNumber);
		strJson.AppendFormat(L"\"TachometerModel\": \"%s\"", sTestLog.wchTachometerModel);
		strJson.AppendFormat(L"\"TachometerManufacturer\": \"%s\"", sTestLog.wchTachometerManufacturer);
		strJson.AppendFormat(L"\"TachometerCalibrationDate\": \"%s\"", sTestLog.wchTachometerCalibrationDate);
		strJson.AppendFormat(L"\"TachometerCheckDate\": \"%s\"", sTestLog.wchTachometerCheckDate);
		strJson.AppendFormat(L"\"TachometerAuthenticationDate\": \"%s\"", sTestLog.wchTachometerAuthenticationDate);
		strJson.AppendFormat(L"\"StationNumber\": \"%s\"", sTestLog.wchStationNumber);
		strJson.AppendFormat(L"\"StationFullName\": \"%s\"", sTestLog.wchStationFullName);
		strJson.AppendFormat(L"\"StationShortName\": \"%s\"", sTestLog.wchStationShortName);
		strJson.AppendFormat(L"\"StationLicenseSn\": \"%s\"", sTestLog.wchStationLicenseSn);
		strJson.AppendFormat(L"\"StationLicenseStartDate\": \"%s\"", sTestLog.wchStationLicenseStartDate);
		strJson.AppendFormat(L"\"StationLicenseEndDate\": \"%s\"", sTestLog.wchStationLicenseEndDate);
		strJson.AppendFormat(L"\"StationIsLocked\": \"%s\"", sTestLog.wchStationIsLocked);
		strJson.AppendFormat(L"\"StationRegistrationInfo\": \"%s\"", sTestLog.wchStationRegistrationInfo);
		strJson.AppendFormat(L"\"StationPhone\": \"%s\"", sTestLog.wchStationPhone);
		strJson.AppendFormat(L"\"StationFax\": \"%s\"", sTestLog.wchStationFax);
		strJson.AppendFormat(L"\"PostCode\": \"%s\"", sTestLog.wchPostCode);
		strJson.AppendFormat(L"\"Province\": \"%s\"", sTestLog.wchProvince);
		strJson.AppendFormat(L"\"City\": \"%s\"", sTestLog.wchCity);
		strJson.AppendFormat(L"\"District\": \"%s\"", sTestLog.wchDistrict);
		strJson.AppendFormat(L"\"DetailedAddress\": \"%s\"", sTestLog.wchDetailedAddress);

		strJson += L"}],";

		strJson += L"\"ResultOfFSUYD\":[{";
		strJson.AppendFormat(L"\"FuelTypeCode\": \"%s\",", sResultOfFSUYD.wchFuelTypeCode);
		strJson.AppendFormat(L"\"FuelTypeName\": \"%s\",", sResultOfFSUYD.wchFuelTypeName);
		strJson.AppendFormat(L"\"EngineRev\": \"%s\",", sResultOfFSUYD.wchEngineRev);
		strJson.AppendFormat(L"\"Value1\": \"%.2f\",", _wtof(sResultOfFSUYD.wchValue1));
		strJson.AppendFormat(L"\"Value2\": \"%.2f\",", _wtof(sResultOfFSUYD.wchValue2));
		strJson.AppendFormat(L"\"Value3\": \"%.2f\",", _wtof(sResultOfFSUYD.wchValue3));
		strJson.AppendFormat(L"\"Value4\": \"%.2f\",", _wtof(sResultOfFSUYD.wchValue4));
		strJson.AppendFormat(L"\"Average\": \"%.2f\",", _wtof(sResultOfFSUYD.wchAverage));
		strJson.AppendFormat(L"\"Limit\": \"%.2f\",", _wtof(sResultOfFSUYD.wchLimit));
		strJson.AppendFormat(L"\"Limit_EDP\": \"%s\",", sResultOfFSUYD.wchLimit_EDP);
		strJson.AppendFormat(L"\"Pass\": \"%s\",", sResultOfFSUYD.wchPass);
		strJson.AppendFormat(L"\"EnvironmentalTemperature\": \"%.2f\",", _wtof(sResultOfFSUYD.wchEnvironmentalTemperature));
		strJson.AppendFormat(L"\"RelativeHumidity\": \"%.2f\",", _wtof(sResultOfFSUYD.wchRelativeHumidity));
		strJson.AppendFormat(L"\"AtmosphericPressure\": \"%.2f\",", _wtof(sResultOfFSUYD.wchAtmosphericPressure));
		strJson.AppendFormat(L"\"OilTemperature\": \"%.2f\",", _wtof(sResultOfFSUYD.wchOilTemperature));
		strJson.AppendFormat(L"\"SkipRev\": \"%s\",", sResultOfFSUYD.wchSkipRev);
		strJson.AppendFormat(L"\"SkipOilTemperature\": \"%s\"", sResultOfFSUYD.wchSkipOilTemperature);
		strJson += L"}]}";

		char *pch(NULL);
		pch = CNHCommonAPI::UnicodeToANSI(strJson);

		unsigned long nLen = strlen(pch);

		BYTE *szBuf = new BYTE[nLen]();
		int nRet = compress(szBuf, &nLen, (BYTE*)pch, nLen);

		ZBase64 zBase64;
		std::string strBufBase64 = zBase64.EncodeRaw(szBuf, nLen);

		DataTrans((BYTE*)strBufBase64.c_str(), strBufBase64.length());

		if(szBuf != NULL)
		{
			delete[] szBuf;
		}

		if (NULL != pch)
		{
			free(pch);
			pch = NULL;
		}
	}

	return true;
}

DWORD CParaThread::WorkerThread(LPVOID lpParam)
{
	CParaThread* pParaThread=(CParaThread*)lpParam;

	while (pParaThread->m_bWorkerRunning)
	{
		pParaThread->m_nWorker++;

		if ((pParaThread->m_nWorker%6)==0)
		{
			// 进入临界区域
			::EnterCriticalSection(&pParaThread->m_sCriticalSection);

			int nF0(0);
			int nF2Len(0);
			char* pchF2Data = NULL;

			char chSql[SQLMAX];
			sprintf_s(chSql, SQLMAX, "select F0,F2 from T1 order by F0 limit 0,1;");

			CppSQLite3Statement stat;
			stat=pParaThread->m_db.compileStatement(chSql);

			CppSQLite3Query rs=stat.execQuery();

			if(!rs.eof())
			{
				nF0 = rs.getIntField(0);

				const unsigned char * pData=rs.getBlobField(1,nF2Len);
				if (nF2Len > 0)
				{
					pchF2Data = new char[nF2Len+1]();
					memcpy_s(pchF2Data, nF2Len, pData, nF2Len);
				}
			}
			rs.finalize();
			stat.finalize();

			if (nF0 > 0)
			{
				if (nF2Len > 0)
				{
					if(PARA_WR_SUCCEED == pParaThread->m_pPara->DataTrans((BYTE*)pchF2Data, nF2Len))
					{
						sprintf_s(chSql, SQLMAX, "delete from T1 where F0 = %d;", nF0);
						pParaThread->m_db.execDML(chSql);
					}
				}
				else
				{
					sprintf_s(chSql, SQLMAX, "delete from T1 where F0 = %d;", nF0);
					pParaThread->m_db.execDML(chSql);
				}
			}

			if (pchF2Data != NULL)
			{
				delete[] pchF2Data;
				pchF2Data = NULL;
			}

			// 离开临界区域
			::LeaveCriticalSection(&pParaThread->m_sCriticalSection);
		}

		Sleep(500);
	}
	
	SetEvent(pParaThread->m_hCloseWorker);

	return 0;
}


DWORD CParaThread::OpenDB(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	DWORD dwRet = CNHCommonAPI::GetHLDFilePath(L"App_Data", L"tmp.dat", wchPath);
	if (0x00==dwRet || 0x02==dwRet)
	{
		dwRet = CNHSQLite3DBO::OpenDB(m_db, wchPath);
		if (0x00 == dwRet)
		{
			m_bOpenDB = true;
		}
		else
		{
			m_bOpenDB = false;
		}

		CheckDB();
	}
	else
	{
		m_bOpenDB = false;
	}

	return dwRet;
}

DWORD CParaThread::CloseDB(void)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	DWORD dwRet(0x00);
	if (IsDBOpen())
	{
		dwRet = CNHSQLite3DBO::CloseDB(m_db);
		m_bOpenDB = false;
	}

	// 离开临界区域
	::LeaveCriticalSection(&m_sCriticalSection);

	return  dwRet;
}

void CParaThread::CheckDB(void)
{
	if (IsDBOpen())
	{
		try
		{
			bool bCreateTable(false);

			CppSQLite3Query q;
			if(0x00==CNHSQLite3DBO::OpenQuery(m_db, q, L"select name from sqlite_master where type='table' and name = 'T1';"))
			{
				if (q.eof())
				{
					bCreateTable = true;			
				}
			}
			if (0x00 != CNHSQLite3DBO::CloseQuery(q))
			{
			}

			if (bCreateTable)
			{
				CString strSQL = L"create table T1(F0 integer PRIMARY KEY autoincrement,F1 datetime, F2 image);";

				if (0x00 != CNHSQLite3DBO::ExecuteDML(m_db, strSQL))
				{
				}
			}

			bool bF0(false),bF1(false),bF2(false);
			if(0x00==CNHSQLite3DBO::OpenQuery(m_db,q,L"PRAGMA table_info('T1');"))
			{
				while (!q.eof())
				{
					wchar_t wchName[50];
					wchar_t wchType[50];
					wchar_t wchPK[50];

					CNHSQLite3DBO::GetFieldValue(q, wchName, 50, L"name");
					CNHSQLite3DBO::GetFieldValue(q, wchType, 50, L"type");
					CNHSQLite3DBO::GetFieldValue(q, wchPK, 50, L"pk");

					if (wcscmp(wchName, L"F0") && wcscmp(wchType, L"integer") && wcscmp(wchPK, L"1"))
					{
						bF0 = true;
					}

					if (wcscmp(wchName, L"F1") && wcscmp(wchType, L"datetime") && wcscmp(wchPK, L"0"))
					{
						bF1 = true;
					}

					if (wcscmp(wchName, L"F2") && wcscmp(wchType, L"image")  && wcscmp(wchPK, L"0"))
					{
						bF2 = true;
					}

					q.nextRow();
				}
			}
			if (0x00 != CNHSQLite3DBO::CloseQuery(q))
			{
			}

			if (!bF0 || !bF1 || !bF2)
			{
				CString strSQL = L"DROP TABLE T1;";
				if (0x00 != CNHSQLite3DBO::ExecuteDML(m_db, strSQL))
				{
				}

				strSQL = L"create table T1(F0 integer PRIMARY KEY autoincrement,F1 datetime, F2 image);";
				if (0x00 != CNHSQLite3DBO::ExecuteDML(m_db, strSQL))
				{
				}
			}

			if (0x00 != CNHSQLite3DBO::ExecuteDML(m_db, L"DELETE FROM T1 WHERE date('now', '-2 day') >= date(F1);"))
			{
			}	
		}
		catch (CppSQLite3Exception &e)
		{
			//WriteDatabaseError(e);
		}
	}
}

void  CParaThread::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 数据包大小[Byte]
	const unsigned int g_unPackSize = 950;
	// 数据包包头大小[Byte]
	const unsigned int g_unPackHeaderSize = 20;
	// 请求编号QN大小
	const unsigned int g_unQNSize = 15;
	// 总包数PCNT大小
	const unsigned int g_unPCNTSize = 2;
	// 包号PNO大小
	const unsigned int g_unPNOSize = 2;
	// 数据包的容量
	const unsigned int g_unPackLoad = g_unPackSize - g_unPackHeaderSize - g_unQNSize - g_unPCNTSize-g_unPNOSize;
	// 总包数
	const unsigned int g_unMaxSum = 99;
	// 总容量
	const unsigned int g_unMaxLoad = g_unPackLoad * g_unMaxSum;

	if (nDatalen <= g_unMaxLoad)
	{
		int nPCNT = 0;
		if((nDatalen%g_unPackLoad)==0)
		{
			nPCNT = nDatalen/g_unPackLoad;
		}
		else
		{
			nPCNT = nDatalen/g_unPackLoad + 1;
		}

		SYSTEMTIME st;
		GetSystemTime(&st);
		char chNow[18];
		sprintf_s(chNow, "%04d%02d%02d%02d%02d%02d%02d",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);

		int nPos = 0;
		for (int i=1; i<=nPCNT; i++)
		{
			BYTE* pbPackData = new BYTE[g_unPackSize]();
			int nPackLen(0);

			for(int j=0; j<g_unPackHeaderSize; j++)
			{
				pbPackData[j] = m_chICCID[j]; 
			}

			for(int j=0; j<g_unQNSize; j++)
			{
				pbPackData[g_unPackHeaderSize+j] = chNow[j+2]; 
			}

			char chPCNT[3];
			sprintf_s(chPCNT, "%02d",nPCNT);
			pbPackData[g_unPackHeaderSize+g_unQNSize] = chPCNT[0];
			pbPackData[g_unPackHeaderSize+g_unQNSize+1] = chPCNT[1];

			char chPNO[3];
			sprintf_s(chPNO, "%02d",i);
			pbPackData[g_unPackHeaderSize+g_unQNSize+g_unPCNTSize] = chPNO[0];
			pbPackData[g_unPackHeaderSize+g_unQNSize+g_unPCNTSize+1] = chPNO[1];

			nPackLen = g_unPackHeaderSize+g_unQNSize+g_unPCNTSize+g_unPNOSize;
			for(int j=0; j<g_unPackLoad; j++)
			{
				if(nPos < (int)nDatalen)
				{
					pbPackData[g_unPackHeaderSize+g_unQNSize+g_unPCNTSize+g_unPNOSize+j] = pbDataBuf[nPos];
					nPos++;
					nPackLen++;
				}
				else
				{
					break;
				}
			}

			char chSql[SQLMAX];
			sprintf_s(chSql, SQLMAX, "INSERT INTO T1 (F0, F1, F2) VALUES(NULL, datetime('now', 'localtime'), ?)");

			CppSQLite3Statement stat;
			stat=m_db.compileStatement(chSql);

			stat.bind(1,(const unsigned char *)pbPackData,nPackLen);
			stat.execDML();
			stat.finalize();//释放分配的stat空间

			if (pbPackData != NULL)
			{
				delete[] pbPackData;
				pbPackData = NULL;
			}
		}
	}

	// 离开临界区域
	::LeaveCriticalSection(&m_sCriticalSection);
}