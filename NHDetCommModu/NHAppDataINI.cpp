// NHAppDataINI.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "NHAppDataINI.h"
#include "assert.h"
#include <fstream>
#include <time.h>
#include <tchar.h>

#include "SimpleIni.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

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

//#ifdef _MANAGED
//#pragma managed(push, off)
//#endif
//
//BOOL APIENTRY DllMain( HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved
//	)
//{
//	return TRUE;
//}
//
//#ifdef _MANAGED
//#pragma managed(pop)
//#endif


// 配置文件版本号
#define VERSION L"1.0.0.20090815"

void GetIniString(const CSimpleIni& ini, const wchar_t* wchSection, const wchar_t* wchKeyword, const wchar_t* wchDefaultValue, wchar_t* wchKeyvalue, const size_t nKeyValueLen)
{
	wcsncpy_s(wchKeyvalue, nKeyValueLen, ini.GetValue(wchSection, wchKeyword, wchDefaultValue), _TRUNCATE);
}

void WriteIniString(CSimpleIni& ini, const wchar_t* wchSection, const wchar_t* wchKeyword, const wchar_t* wchKeyvalue)
{
	ini.SetValue(wchSection, wchKeyword, wchKeyvalue);
}

void GetIniUserInfo(PUSERINFO const pIniUserInfo)
{
	// 获取UserInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath))
	{
		return;
	}
	
	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取UserInfo.ini信息
	GetIniString(ini, L"UserInfo", L"Name", L"", pIniUserInfo->wchName, 50);
	GetIniString(ini, L"UserInfo", L"Password", L"", pIniUserInfo->wchPassword, 50);
	GetIniString(ini, L"UserInfo", L"LicenseSn", L"",pIniUserInfo->wchLicenseSn, 50);
	GetIniString(ini, L"UserInfo", L"LicenseStartDate", L"",pIniUserInfo->wchLicenseStartDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"UserInfo", L"LicenseEndDate", L"", pIniUserInfo->wchLicenseEndDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"UserInfo", L"Role", L"", pIniUserInfo->wchRole, NH_INT_TO_WCHART);
}

void SetIniUserInfo(const USERINFO& iniUserInfo)
{
	// 获取UserInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath, true))
	{
		return;
	}

	CSimpleIni ini(true);

	// 写UserInfo.ini配置文件
	WriteIniString(ini, L"UserInfo", L"Name", iniUserInfo.wchName);
	//WriteIniString(ini, L"UserInfo", L"Password", iniUserInfo.wchPassword);
	WriteIniString(ini, L"UserInfo", L"LicenseSn", iniUserInfo.wchLicenseSn);
	WriteIniString(ini, L"UserInfo", L"LicenseStartDate", iniUserInfo.wchLicenseStartDate);
	WriteIniString(ini, L"UserInfo", L"LicenseEndDate", iniUserInfo.wchLicenseEndDate);
	WriteIniString(ini, L"UserInfo", L"Role", iniUserInfo.wchRole);
	//WriteIniString(ini, L"UserInfo", L"Version", VERSION, wchPath);

	ini.SaveFile(wchPath);
}

//void GetIniBillInfo(PBILLINFO const pIniBillInfo)
//{
//	// 获取BillInfo.ini路径
//	wchar_t wchPath[MAX_PATH];
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"BillInfo.ini", wchPath))
//	{
//		return;
//	}
//	CSimpleIni ini(true);
//	ini.LoadFile(wchPath);
//
//	// 获取BillInfo.ini信息
//	GetIniString(ini, L"BillInfo", L"AutoID", L"", pIniBillInfo->wchAutoID, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"RunningNumber", L"", pIniBillInfo->wchRunningNumber, 50);
//	GetIniString(ini, L"BillInfo", L"PlateNumber", L"", pIniBillInfo->wchPlateNumber, 50);
//	GetIniString(ini, L"BillInfo", L"PlateType", L"", pIniBillInfo->wchPlateType, 50);
//	GetIniString(ini, L"BillInfo", L"TestType", L"", pIniBillInfo->wchTestType, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"IsRetest", L"", pIniBillInfo->wchIsRetest, NH_BOOL_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"IsFuelTypeNeedTest", L"", pIniBillInfo->wchIsFuelTypeNeedTest, NH_BOOL_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"IsFuelType2NeedTest", L"", pIniBillInfo->wchIsFuelType2NeedTest, NH_BOOL_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"BillDate", L"", pIniBillInfo->wchBillDate, NH_TIME_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"Biller", L"", pIniBillInfo->wchBiller, 50);
//	GetIniString(ini, L"BillInfo", L"Shroff", L"", pIniBillInfo->wchShroff, 50);
//	GetIniString(ini, L"BillInfo", L"Driver", L"", pIniBillInfo->wchDriver, 50);
//	GetIniString(ini, L"BillInfo", L"Assessor", L"", pIniBillInfo->wchAssessor, 50);
//	GetIniString(ini, L"BillInfo", L"ChargeState", L"", pIniBillInfo->wchChargeState, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"InitialMoney", L"", pIniBillInfo->wchInitialMoney, NH_FLOAT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"ChangedMoney", L"", pIniBillInfo->wchChangedMoney, NH_FLOAT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"ReceivableMoney", L"", pIniBillInfo->wchReceivableMoney, NH_FLOAT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"ActualMoney", L"", pIniBillInfo->wchActualMoney, NH_FLOAT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"TestCharacter", L"", pIniBillInfo->wchTestCharacter, 50);
//	GetIniString(ini, L"BillInfo", L"TestLocation", L"", pIniBillInfo->wchTestLocation, 50);
//	GetIniString(ini, L"BillInfo", L"TestState", L"", pIniBillInfo->wchTestState, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"LineNumber", L"", pIniBillInfo->wchLineNumber, 50);
//	GetIniString(ini, L"BillInfo", L"StartProcessingTime", L"", pIniBillInfo->wchStartProcessingTime, NH_TIME_TO_WCHART);
//
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodASM", L"", pIniBillInfo->wchNumberOfTestPeriodASM, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodVMAS", L"", pIniBillInfo->wchNumberOfTestPeriodVMAS, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodLUGDOWN", L"", pIniBillInfo->wchNumberOfTestPeriodLUGDOWN, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodDIS", L"", pIniBillInfo->wchNumberOfTestPeriodDIS, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodFSUNHT", L"", pIniBillInfo->wchNumberOfTestPeriodFSUNHT, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodFSUYD", L"", pIniBillInfo->wchNumberOfTestPeriodFSUYD, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodAVFSU", L"", pIniBillInfo->wchNumberOfTestPeriodAVFSU, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriodMI", L"", pIniBillInfo->wchNumberOfTestPeriodMI, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"NumberOfTestPeriod", L"", pIniBillInfo->wchNumberOfTestPeriod, NH_INT_TO_WCHART);
//	GetIniString(ini, L"BillInfo", L"TestPeriodUsed", L"", pIniBillInfo->wchTestPeriodUsed, NH_INT_TO_WCHART);
//
//	GetIniString(ini, L"BillInfo", L"QuitReason", L"", pIniBillInfo->wchQuitReason, 50);
//	GetIniString(ini, L"BillInfo", L"Remark", L"", pIniBillInfo->wchRemark, 1024);
//	GetIniString(ini, L"BillInfo", L"Reserved1", L"", pIniBillInfo->wchReserved1, 50);
//	GetIniString(ini, L"BillInfo", L"Reserved2", L"", pIniBillInfo->wchReserved2, 50);
//	GetIniString(ini, L"BillInfo", L"Reserved3", L"", pIniBillInfo->wchReserved3, 50);
//}
//
//void SetIniBillInfo(const BILLINFO& iniBillInfo)
//{
//	// 获取BillInfo.ini路径
//	wchar_t wchPath[MAX_PATH];
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"BillInfo.ini", wchPath, true))
//	{
//		return;
//	}
//	
//	CSimpleIni ini(true);
//
//	// 写BillInfo.ini配置文件
//	WriteIniString(ini, L"BillInfo", L"AutoID", iniBillInfo.wchAutoID);
//	WriteIniString(ini, L"BillInfo", L"RunningNumber", iniBillInfo.wchRunningNumber);
//	WriteIniString(ini, L"BillInfo", L"PlateNumber", iniBillInfo.wchPlateNumber);
//	WriteIniString(ini, L"BillInfo", L"PlateType", iniBillInfo.wchPlateType);
//	WriteIniString(ini, L"BillInfo", L"TestType", iniBillInfo.wchTestType);
//	WriteIniString(ini, L"BillInfo", L"IsRetest", iniBillInfo.wchIsRetest);
//	WriteIniString(ini, L"BillInfo", L"IsFuelTypeNeedTest", iniBillInfo.wchIsFuelTypeNeedTest);
//	WriteIniString(ini, L"BillInfo", L"IsFuelType2NeedTest", iniBillInfo.wchIsFuelType2NeedTest);
//	WriteIniString(ini, L"BillInfo", L"BillDate", iniBillInfo.wchBillDate);
//	WriteIniString(ini, L"BillInfo", L"Biller", iniBillInfo.wchBiller);
//	WriteIniString(ini, L"BillInfo", L"Shroff", iniBillInfo.wchShroff);
//	WriteIniString(ini, L"BillInfo", L"Driver", iniBillInfo.wchDriver);
//	WriteIniString(ini, L"BillInfo", L"Assessor", iniBillInfo.wchAssessor);
//	WriteIniString(ini, L"BillInfo", L"ChargeState", iniBillInfo.wchChargeState);
//	WriteIniString(ini, L"BillInfo", L"InitialMoney", iniBillInfo.wchInitialMoney);
//	WriteIniString(ini, L"BillInfo", L"ChangedMoney", iniBillInfo.wchChangedMoney);
//	WriteIniString(ini, L"BillInfo", L"ReceivableMoney", iniBillInfo.wchReceivableMoney);
//	WriteIniString(ini, L"BillInfo", L"ActualMoney", iniBillInfo.wchActualMoney);
//	WriteIniString(ini, L"BillInfo", L"TestCharacter", iniBillInfo.wchTestCharacter);
//	WriteIniString(ini, L"BillInfo", L"TestLocation", iniBillInfo.wchTestLocation);
//	WriteIniString(ini, L"BillInfo", L"TestState", iniBillInfo.wchTestState);
//	WriteIniString(ini, L"BillInfo", L"LineNumber", iniBillInfo.wchLineNumber);
//	WriteIniString(ini, L"BillInfo", L"StartProcessingTime", iniBillInfo.wchStartProcessingTime);
//
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodASM", iniBillInfo.wchNumberOfTestPeriodASM);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodVMAS", iniBillInfo.wchNumberOfTestPeriodVMAS);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodLUGDOWN", iniBillInfo.wchNumberOfTestPeriodLUGDOWN);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodDIS", iniBillInfo.wchNumberOfTestPeriodDIS);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodFSUNHT", iniBillInfo.wchNumberOfTestPeriodFSUNHT);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodFSUYD", iniBillInfo.wchNumberOfTestPeriodFSUYD);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodAVFSU", iniBillInfo.wchNumberOfTestPeriodAVFSU);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriodMI", iniBillInfo.wchNumberOfTestPeriodMI);
//	WriteIniString(ini, L"BillInfo", L"NumberOfTestPeriod", iniBillInfo.wchNumberOfTestPeriod);
//	WriteIniString(ini, L"BillInfo", L"TestPeriodUsed", iniBillInfo.wchTestPeriodUsed);
//
//	WriteIniString(ini, L"BillInfo", L"QuitReason", iniBillInfo.wchQuitReason);
//	WriteIniString(ini, L"BillInfo", L"Remark", iniBillInfo.wchRemark);
//	WriteIniString(ini, L"BillInfo", L"Reserved1", iniBillInfo.wchReserved1);
//	WriteIniString(ini, L"BillInfo", L"Reserved2", iniBillInfo.wchReserved2);
//	WriteIniString(ini, L"BillInfo", L"Reserved3", iniBillInfo.wchReserved3);
//
//	ini.SaveFile(wchPath);
//}

void GetIniVehicleInfo(PVEHICLEINFO const pIniVehicleInfo)
{
	// 获取BillInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
	{
		return;
	}
	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取BillInfo.ini信息
	GetIniString(ini, L"VehicleInfo", L"AutoID", L"", pIniVehicleInfo->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"PlateNumber", L"", pIniVehicleInfo->wchPlateNumber, 50);
	GetIniString(ini, L"VehicleInfo", L"PlateType", L"", pIniVehicleInfo->wchPlateType, 50);
	GetIniString(ini, L"VehicleInfo", L"PlateTypeCode", L"", pIniVehicleInfo->wchPlateTypeCode, 50);
	GetIniString(ini, L"VehicleInfo", L"PlateTypeGA", L"", pIniVehicleInfo->wchPlateTypeGA, 50);
	GetIniString(ini, L"VehicleInfo", L"PlateTypeCodeGA", L"", pIniVehicleInfo->wchPlateTypeCodeGA, 50);
	GetIniString(ini, L"VehicleInfo", L"RegistrationDate", L"", pIniVehicleInfo->wchRegistrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"Owner", L"", pIniVehicleInfo->wchOwner, 50);
	GetIniString(ini, L"VehicleInfo", L"Phone", L"", pIniVehicleInfo->wchPhone, 50);
	GetIniString(ini, L"VehicleInfo", L"Address", L"", pIniVehicleInfo->wchAddress, 50);
	GetIniString(ini, L"VehicleInfo", L"UseCharacter", L"", pIniVehicleInfo->wchUseCharacter, 50);
	GetIniString(ini, L"VehicleInfo", L"Manufacturer", L"", pIniVehicleInfo->wchManufacturer, 50);
	GetIniString(ini, L"VehicleInfo", L"Brand", L"", pIniVehicleInfo->wchBrand, 50);
	GetIniString(ini, L"VehicleInfo", L"Model", L"", pIniVehicleInfo->wchModel, 50);
	GetIniString(ini, L"VehicleInfo", L"ProductionDate", L"", pIniVehicleInfo->wchProductionDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"VIN", L"", pIniVehicleInfo->wchVIN, 50);
	GetIniString(ini, L"VehicleInfo", L"ChassisModel", L"", pIniVehicleInfo->wchChassisModel, 50);
	GetIniString(ini, L"VehicleInfo", L"VehicleType", L"", pIniVehicleInfo->wchVehicleType, 50);
	GetIniString(ini, L"VehicleInfo", L"MaximumTotalMass", L"", pIniVehicleInfo->wchMaximumTotalMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"UnladenMass", L"", pIniVehicleInfo->wchUnladenMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"AxleMass", L"", pIniVehicleInfo->wchAxleMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"RatedLoadingMass", L"", pIniVehicleInfo->wchRatedLoadingMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"RatedPassengerCapacity", L"", pIniVehicleInfo->wchRatedPassengerCapacity, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"TyrePressure", L"", pIniVehicleInfo->wchTyrePressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"TravelledDistance", L"", pIniVehicleInfo->wchTravelledDistance, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"EngineManufacturer", L"", pIniVehicleInfo->wchEngineManufacturer, 50);
	GetIniString(ini, L"VehicleInfo", L"EngineModel", L"", pIniVehicleInfo->wchEngineModel, 50);
	GetIniString(ini, L"VehicleInfo", L"EngineNumber", L"", pIniVehicleInfo->wchEngineNumber, 50);
	GetIniString(ini, L"VehicleInfo", L"EngineStroke", L"", pIniVehicleInfo->wchEngineStroke, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"Displacement", L"", pIniVehicleInfo->wchDisplacement, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"NumberOfCylinder", L"", pIniVehicleInfo->wchNumberOfCylinder, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"OilSupplyMode", L"", pIniVehicleInfo->wchOilSupplyMode, 50);
	GetIniString(ini, L"VehicleInfo", L"AirIntakeMode", L"", pIniVehicleInfo->wchAirIntakeMode, 50);
	GetIniString(ini, L"VehicleInfo", L"HasCatalyticConverter", L"", pIniVehicleInfo->wchHasCatalyticConverter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"FuelType", L"", pIniVehicleInfo->wchFuelType, 50);
	GetIniString(ini, L"VehicleInfo", L"FuelMark", L"", pIniVehicleInfo->wchFuelMark, 50);
	GetIniString(ini, L"VehicleInfo", L"DoubleFuel", L"", pIniVehicleInfo->wchDoubleFuel, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"FuelType2", L"", pIniVehicleInfo->wchFuelType2, 50);
	GetIniString(ini, L"VehicleInfo", L"FuelMark2", L"", pIniVehicleInfo->wchFuelMark2, 50);
	GetIniString(ini, L"VehicleInfo", L"RatedRev", L"", pIniVehicleInfo->wchRatedRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"RatedPower", L"", pIniVehicleInfo->wchRatedPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"MaximumNetPower", L"", pIniVehicleInfo->wchMaximumNetPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"GearBoxType", L"", pIniVehicleInfo->wchGearBoxType, 50);
	GetIniString(ini, L"VehicleInfo", L"NumberOfGears", L"", pIniVehicleInfo->wchNumberOfGears, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"DriveType", L"", pIniVehicleInfo->wchDriveType, 50);
	GetIniString(ini, L"VehicleInfo", L"EPSign", L"", pIniVehicleInfo->wchEPSign, 50);
	GetIniString(ini, L"VehicleInfo", L"CertificateNumber", L"", pIniVehicleInfo->wchCertificateNumber, 50);
	GetIniString(ini, L"VehicleInfo", L"IndependentDoubleExhaustPipe", L"", pIniVehicleInfo->wchIndependentDoubleExhaustPipe, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"TCS", L"", pIniVehicleInfo->wchTCS, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"FuelPumpMode", L"", pIniVehicleInfo->wchFuelPumpMode, 50);
	GetIniString(ini, L"VehicleInfo", L"PassengerVehicle", L"", pIniVehicleInfo->wchPassengerVehicle, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"EmissionStandard", L"", pIniVehicleInfo->wchEmissionStandard, 50);
	GetIniString(ini, L"VehicleInfo", L"AnnualTestPeriod", L"", pIniVehicleInfo->wchAnnualTestPeriod, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"HasOBD", L"", pIniVehicleInfo->wchHasOBD, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"OwnerCharacter", L"", pIniVehicleInfo->wchOwnerCharacter, 50);
	GetIniString(ini, L"VehicleInfo", L"OwnerDistrict", L"", pIniVehicleInfo->wchOwnerDistrict, 50);
	GetIniString(ini, L"VehicleInfo", L"LicenseDepartment", L"", pIniVehicleInfo->wchLicenseDepartment, 50);
	GetIniString(ini, L"VehicleInfo", L"LicenseIssueDate", L"", pIniVehicleInfo->wchLicenseIssueDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"LicenseValidDate", L"", pIniVehicleInfo->wchLicenseValidDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"MotorModel", L"", pIniVehicleInfo->wchMotorModel, 50);
	GetIniString(ini, L"VehicleInfo", L"EnergyStorageDeviceModel", L"", pIniVehicleInfo->wchEnergyStorageDeviceModel, 50);
	GetIniString(ini, L"VehicleInfo", L"BatteryCapacity", L"", pIniVehicleInfo->wchBatteryCapacity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"HasDPF", L"", pIniVehicleInfo->wchHasDPF, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"DPFModel", L"", pIniVehicleInfo->wchDPFModel, 50);
	GetIniString(ini, L"VehicleInfo", L"HasSCR", L"", pIniVehicleInfo->wchHasSCR, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleInfo", L"SCRModel", L"", pIniVehicleInfo->wchSCRModel, 50);
	GetIniString(ini, L"VehicleInfo", L"Remark", L"", pIniVehicleInfo->wchRemark, 1024);
	GetIniString(ini, L"VehicleInfo", L"Reserved1", L"", pIniVehicleInfo->wchReserved1, 50);
	GetIniString(ini, L"VehicleInfo", L"Reserved2", L"", pIniVehicleInfo->wchReserved2, 50);
	GetIniString(ini, L"VehicleInfo", L"Reserved3", L"", pIniVehicleInfo->wchReserved3, 50);

}

void SetIniVehicleInfo(const VEHICLEINFO& iniVehicleInfo)
{
	// 获取BillInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取BillInfo.ini信息
	WriteIniString(ini, L"VehicleInfo", L"AutoID", iniVehicleInfo.wchAutoID);
	WriteIniString(ini, L"VehicleInfo", L"PlateNumber", iniVehicleInfo.wchPlateNumber);
	WriteIniString(ini, L"VehicleInfo", L"PlateType", iniVehicleInfo.wchPlateType);
	WriteIniString(ini, L"VehicleInfo", L"PlateTypeCode", iniVehicleInfo.wchPlateTypeCode);
	WriteIniString(ini, L"VehicleInfo", L"PlateTypeGA", iniVehicleInfo.wchPlateTypeGA);
	WriteIniString(ini, L"VehicleInfo", L"PlateTypeCodeGA", iniVehicleInfo.wchPlateTypeCodeGA);
	WriteIniString(ini, L"VehicleInfo", L"RegistrationDate", iniVehicleInfo.wchRegistrationDate);
	WriteIniString(ini, L"VehicleInfo", L"Owner", iniVehicleInfo.wchOwner);
	WriteIniString(ini, L"VehicleInfo", L"Phone", iniVehicleInfo.wchPhone);
	WriteIniString(ini, L"VehicleInfo", L"Address", iniVehicleInfo.wchAddress);
	WriteIniString(ini, L"VehicleInfo", L"UseCharacter", iniVehicleInfo.wchUseCharacter);
	WriteIniString(ini, L"VehicleInfo", L"Manufacturer", iniVehicleInfo.wchManufacturer);
	WriteIniString(ini, L"VehicleInfo", L"Brand", iniVehicleInfo.wchBrand);
	WriteIniString(ini, L"VehicleInfo", L"Model", iniVehicleInfo.wchModel);
	WriteIniString(ini, L"VehicleInfo", L"ProductionDate", iniVehicleInfo.wchProductionDate);
	WriteIniString(ini, L"VehicleInfo", L"VIN", iniVehicleInfo.wchVIN);
	WriteIniString(ini, L"VehicleInfo", L"ChassisModel", iniVehicleInfo.wchChassisModel);
	WriteIniString(ini, L"VehicleInfo", L"VehicleType", iniVehicleInfo.wchVehicleType);
	WriteIniString(ini, L"VehicleInfo", L"MaximumTotalMass", iniVehicleInfo.wchMaximumTotalMass);
	WriteIniString(ini, L"VehicleInfo", L"UnladenMass", iniVehicleInfo.wchUnladenMass);
	WriteIniString(ini, L"VehicleInfo", L"AxleMass", iniVehicleInfo.wchAxleMass);
	WriteIniString(ini, L"VehicleInfo", L"RatedLoadingMass", iniVehicleInfo.wchRatedLoadingMass);
	WriteIniString(ini, L"VehicleInfo", L"RatedPassengerCapacity", iniVehicleInfo.wchRatedPassengerCapacity);
	WriteIniString(ini, L"VehicleInfo", L"TyrePressure", iniVehicleInfo.wchTyrePressure);
	WriteIniString(ini, L"VehicleInfo", L"TravelledDistance", iniVehicleInfo.wchTravelledDistance);
	WriteIniString(ini, L"VehicleInfo", L"EngineManufacturer", iniVehicleInfo.wchEngineManufacturer);
	WriteIniString(ini, L"VehicleInfo", L"EngineModel", iniVehicleInfo.wchEngineModel);
	WriteIniString(ini, L"VehicleInfo", L"EngineNumber", iniVehicleInfo.wchEngineNumber);
	WriteIniString(ini, L"VehicleInfo", L"EngineStroke", iniVehicleInfo.wchEngineStroke);
	WriteIniString(ini, L"VehicleInfo", L"Displacement", iniVehicleInfo.wchDisplacement);
	WriteIniString(ini, L"VehicleInfo", L"NumberOfCylinder", iniVehicleInfo.wchNumberOfCylinder);
	WriteIniString(ini, L"VehicleInfo", L"OilSupplyMode", iniVehicleInfo.wchOilSupplyMode);
	WriteIniString(ini, L"VehicleInfo", L"AirIntakeMode", iniVehicleInfo.wchAirIntakeMode);
	WriteIniString(ini, L"VehicleInfo", L"HasCatalyticConverter", iniVehicleInfo.wchHasCatalyticConverter);
	WriteIniString(ini, L"VehicleInfo", L"FuelType", iniVehicleInfo.wchFuelType);
	WriteIniString(ini, L"VehicleInfo", L"FuelMark", iniVehicleInfo.wchFuelMark);
	WriteIniString(ini, L"VehicleInfo", L"DoubleFuel", iniVehicleInfo.wchDoubleFuel);
	WriteIniString(ini, L"VehicleInfo", L"FuelType2", iniVehicleInfo.wchFuelType2);
	WriteIniString(ini, L"VehicleInfo", L"FuelMark2", iniVehicleInfo.wchFuelMark2);
	WriteIniString(ini, L"VehicleInfo", L"RatedRev", iniVehicleInfo.wchRatedRev);
	WriteIniString(ini, L"VehicleInfo", L"RatedPower", iniVehicleInfo.wchRatedPower);
	WriteIniString(ini, L"VehicleInfo", L"MaximumNetPower", iniVehicleInfo.wchMaximumNetPower);
	WriteIniString(ini, L"VehicleInfo", L"GearBoxType", iniVehicleInfo.wchGearBoxType);
	WriteIniString(ini, L"VehicleInfo", L"NumberOfGears", iniVehicleInfo.wchNumberOfGears);
	WriteIniString(ini, L"VehicleInfo", L"DriveType", iniVehicleInfo.wchDriveType);
	WriteIniString(ini, L"VehicleInfo", L"EPSign", iniVehicleInfo.wchEPSign);
	WriteIniString(ini, L"VehicleInfo", L"CertificateNumber", iniVehicleInfo.wchCertificateNumber);
	WriteIniString(ini, L"VehicleInfo", L"IndependentDoubleExhaustPipe", iniVehicleInfo.wchIndependentDoubleExhaustPipe);
	WriteIniString(ini, L"VehicleInfo", L"TCS", iniVehicleInfo.wchTCS);
	WriteIniString(ini, L"VehicleInfo", L"FuelPumpMode", iniVehicleInfo.wchFuelPumpMode);
	WriteIniString(ini, L"VehicleInfo", L"PassengerVehicle", iniVehicleInfo.wchPassengerVehicle);
	WriteIniString(ini, L"VehicleInfo", L"EmissionStandard", iniVehicleInfo.wchEmissionStandard);
	WriteIniString(ini, L"VehicleInfo", L"AnnualTestPeriod", iniVehicleInfo.wchAnnualTestPeriod);
	WriteIniString(ini, L"VehicleInfo", L"HasOBD", iniVehicleInfo.wchHasOBD);
	WriteIniString(ini, L"VehicleInfo", L"OwnerCharacter", iniVehicleInfo.wchOwnerCharacter);
	WriteIniString(ini, L"VehicleInfo", L"OwnerDistrict", iniVehicleInfo.wchOwnerDistrict);
	WriteIniString(ini, L"VehicleInfo", L"LicenseDepartment", iniVehicleInfo.wchLicenseDepartment);
	WriteIniString(ini, L"VehicleInfo", L"LicenseIssueDate", iniVehicleInfo.wchLicenseIssueDate);
	WriteIniString(ini, L"VehicleInfo", L"LicenseValidDate", iniVehicleInfo.wchLicenseValidDate);
	WriteIniString(ini, L"VehicleInfo", L"MotorModel", iniVehicleInfo.wchMotorModel);
	WriteIniString(ini, L"VehicleInfo", L"EnergyStorageDeviceModel", iniVehicleInfo.wchEnergyStorageDeviceModel);
	WriteIniString(ini, L"VehicleInfo", L"BatteryCapacity", iniVehicleInfo.wchBatteryCapacity);
	WriteIniString(ini, L"VehicleInfo", L"HasDPF", iniVehicleInfo.wchHasDPF);
	WriteIniString(ini, L"VehicleInfo", L"DPFModel", iniVehicleInfo.wchDPFModel);
	WriteIniString(ini, L"VehicleInfo", L"HasSCR", iniVehicleInfo.wchHasSCR);
	WriteIniString(ini, L"VehicleInfo", L"SCRModel", iniVehicleInfo.wchSCRModel);
	WriteIniString(ini, L"VehicleInfo", L"Remark", iniVehicleInfo.wchRemark);
	WriteIniString(ini, L"VehicleInfo", L"Reserved1", iniVehicleInfo.wchReserved1);
	WriteIniString(ini, L"VehicleInfo", L"Reserved2", iniVehicleInfo.wchReserved2);
	WriteIniString(ini, L"VehicleInfo", L"Reserved3", iniVehicleInfo.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniVehicleModel(PVEHICLEMODEL const pIniVehicleModel)
{
	// 获取BillInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleModel.ini", wchPath))
	{
		return;
	}
	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取BillInfo.ini信息
	GetIniString(ini, L"VehicleModel", L"AutoID", L"", pIniVehicleModel->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"Manufacturer", L"", pIniVehicleModel->wchManufacturer, 50);
	GetIniString(ini, L"VehicleModel", L"Brand", L"", pIniVehicleModel->wchBrand, 50);
	GetIniString(ini, L"VehicleModel", L"Model", L"", pIniVehicleModel->wchModel, 50);
	GetIniString(ini, L"VehicleModel", L"ChassisModel", L"", pIniVehicleModel->wchChassisModel, 50);
	GetIniString(ini, L"VehicleModel", L"VehicleType", L"", pIniVehicleModel->wchVehicleType, 50);
	GetIniString(ini, L"VehicleModel", L"MaximumTotalMass", L"", pIniVehicleModel->wchMaximumTotalMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"UnladenMass", L"", pIniVehicleModel->wchUnladenMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"AxleMass", L"", pIniVehicleModel->wchAxleMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"RatedLoadingMass", L"", pIniVehicleModel->wchRatedLoadingMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"RatedPassengerCapacity", L"", pIniVehicleModel->wchRatedPassengerCapacity, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"TyrePressure", L"", pIniVehicleModel->wchTyrePressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"EngineManufacturer", L"", pIniVehicleModel->wchEngineManufacturer, 50);
	GetIniString(ini, L"VehicleModel", L"EngineModel", L"", pIniVehicleModel->wchEngineModel, 50);
	GetIniString(ini, L"VehicleModel", L"Displacement", L"", pIniVehicleModel->wchDisplacement, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"NumberOfCylinder", L"", pIniVehicleModel->wchNumberOfCylinder, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"OilSupplyMode", L"", pIniVehicleModel->wchOilSupplyMode, 50);
	GetIniString(ini, L"VehicleModel", L"AirIntakeMode", L"", pIniVehicleModel->wchAirIntakeMode, 50);
	GetIniString(ini, L"VehicleModel", L"HasCatalyticConverter", L"", pIniVehicleModel->wchHasCatalyticConverter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"FuelType", L"", pIniVehicleModel->wchFuelType, 50);
	GetIniString(ini, L"VehicleModel", L"FuelMark", L"", pIniVehicleModel->wchFuelMark, 50);
	GetIniString(ini, L"VehicleModel", L"DoubleFuel", L"", pIniVehicleModel->wchDoubleFuel, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"FuelType2", L"", pIniVehicleModel->wchFuelType2, 50);
	GetIniString(ini, L"VehicleModel", L"FuelMark2", L"", pIniVehicleModel->wchFuelMark2, 50);
	GetIniString(ini, L"VehicleModel", L"RatedRev", L"", pIniVehicleModel->wchRatedRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"RatedPower", L"", pIniVehicleModel->wchRatedPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"MaximumNetPower", L"", pIniVehicleModel->wchMaximumNetPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"GearBoxType", L"", pIniVehicleModel->wchGearBoxType, 50);
	GetIniString(ini, L"VehicleModel", L"NumberOfGears", L"", pIniVehicleModel->wchNumberOfGears, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"DriveType", L"", pIniVehicleModel->wchDriveType, 50);
	GetIniString(ini, L"VehicleModel", L"EPSign", L"", pIniVehicleModel->wchEPSign, 50);
	GetIniString(ini, L"VehicleModel", L"IndependentDoubleExhaustPipe", L"", pIniVehicleModel->wchIndependentDoubleExhaustPipe, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"TCS", L"", pIniVehicleModel->wchTCS, NH_INT_TO_WCHART);
	GetIniString(ini, L"VehicleModel", L"FuelPumpMode", L"", pIniVehicleModel->wchFuelPumpMode, 50);
	GetIniString(ini, L"VehicleModel", L"Remark", L"", pIniVehicleModel->wchRemark, 1024);
	GetIniString(ini, L"VehicleModel", L"Reserved1", L"", pIniVehicleModel->wchReserved1, 50);
	GetIniString(ini, L"VehicleModel", L"Reserved2", L"", pIniVehicleModel->wchReserved2, 50);
	GetIniString(ini, L"VehicleModel", L"Reserved3", L"", pIniVehicleModel->wchReserved3, 50);

}

void SetIniVehicleModel(const VEHICLEMODEL& iniVehicleModel)
{
	// 获取BillInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleModel.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取BillInfo.ini信息
	WriteIniString(ini, L"VehicleModel", L"AutoID", iniVehicleModel.wchAutoID);
	WriteIniString(ini, L"VehicleModel", L"Manufacturer", iniVehicleModel.wchManufacturer);
	WriteIniString(ini, L"VehicleModel", L"Brand", iniVehicleModel.wchBrand);
	WriteIniString(ini, L"VehicleModel", L"Model", iniVehicleModel.wchModel);
	WriteIniString(ini, L"VehicleModel", L"ChassisModel", iniVehicleModel.wchChassisModel);
	WriteIniString(ini, L"VehicleModel", L"VehicleType", iniVehicleModel.wchVehicleType);
	WriteIniString(ini, L"VehicleModel", L"MaximumTotalMass", iniVehicleModel.wchMaximumTotalMass);
	WriteIniString(ini, L"VehicleModel", L"UnladenMass", iniVehicleModel.wchUnladenMass);
	WriteIniString(ini, L"VehicleModel", L"AxleMass", iniVehicleModel.wchAxleMass);
	WriteIniString(ini, L"VehicleModel", L"RatedLoadingMass", iniVehicleModel.wchRatedLoadingMass);
	WriteIniString(ini, L"VehicleModel", L"RatedPassengerCapacity", iniVehicleModel.wchRatedPassengerCapacity);
	WriteIniString(ini, L"VehicleModel", L"TyrePressure", iniVehicleModel.wchTyrePressure);
	WriteIniString(ini, L"VehicleModel", L"EngineManufacturer", iniVehicleModel.wchEngineManufacturer);
	WriteIniString(ini, L"VehicleModel", L"EngineModel", iniVehicleModel.wchEngineModel);
	WriteIniString(ini, L"VehicleModel", L"Displacement", iniVehicleModel.wchDisplacement);
	WriteIniString(ini, L"VehicleModel", L"NumberOfCylinder", iniVehicleModel.wchNumberOfCylinder);
	WriteIniString(ini, L"VehicleModel", L"OilSupplyMode", iniVehicleModel.wchOilSupplyMode);
	WriteIniString(ini, L"VehicleModel", L"AirIntakeMode", iniVehicleModel.wchAirIntakeMode);
	WriteIniString(ini, L"VehicleModel", L"HasCatalyticConverter", iniVehicleModel.wchHasCatalyticConverter);
	WriteIniString(ini, L"VehicleModel", L"FuelType", iniVehicleModel.wchFuelType);
	WriteIniString(ini, L"VehicleModel", L"FuelMark", iniVehicleModel.wchFuelMark);
	WriteIniString(ini, L"VehicleModel", L"DoubleFuel", iniVehicleModel.wchDoubleFuel);
	WriteIniString(ini, L"VehicleModel", L"FuelType2", iniVehicleModel.wchFuelType2);
	WriteIniString(ini, L"VehicleModel", L"FuelMark2", iniVehicleModel.wchFuelMark2);
	WriteIniString(ini, L"VehicleModel", L"RatedRev", iniVehicleModel.wchRatedRev);
	WriteIniString(ini, L"VehicleModel", L"RatedPower", iniVehicleModel.wchRatedPower);
	WriteIniString(ini, L"VehicleModel", L"MaximumNetPower", iniVehicleModel.wchMaximumNetPower);
	WriteIniString(ini, L"VehicleModel", L"GearBoxType", iniVehicleModel.wchGearBoxType);
	WriteIniString(ini, L"VehicleModel", L"NumberOfGears", iniVehicleModel.wchNumberOfGears);
	WriteIniString(ini, L"VehicleModel", L"DriveType", iniVehicleModel.wchDriveType);
	WriteIniString(ini, L"VehicleModel", L"EPSign", iniVehicleModel.wchEPSign);
	WriteIniString(ini, L"VehicleModel", L"IndependentDoubleExhaustPipe", iniVehicleModel.wchIndependentDoubleExhaustPipe);
	WriteIniString(ini, L"VehicleModel", L"TCS", iniVehicleModel.wchTCS);
	WriteIniString(ini, L"VehicleModel", L"FuelPumpMode", iniVehicleModel.wchFuelPumpMode);
	WriteIniString(ini, L"VehicleModel", L"Remark", iniVehicleModel.wchRemark);
	WriteIniString(ini, L"VehicleModel", L"Reserved1", iniVehicleModel.wchReserved1);
	WriteIniString(ini, L"VehicleModel", L"Reserved2", iniVehicleModel.wchReserved2);
	WriteIniString(ini, L"VehicleModel", L"Reserved3", iniVehicleModel.wchReserved3);

	ini.SaveFile(wchPath);
}


void GetIniLimitOfASM(PLIMITOFASM const pIniLimitOfASM)
{
	// 获取LimitOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfASM.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfASM.ini信息
	GetIniString(ini, L"LimitOfASM", L"HC5025", L"", pIniLimitOfASM->wchHC5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"HC5025_EDP", L"", pIniLimitOfASM->wchHC5025_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"CO5025", L"", pIniLimitOfASM->wchCO5025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"CO5025_EDP", L"", pIniLimitOfASM->wchCO5025_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"NO5025", L"", pIniLimitOfASM->wchNO5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"NO5025_EDP", L"", pIniLimitOfASM->wchNO5025_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"HC2540", L"", pIniLimitOfASM->wchHC2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"HC2540_EDP", L"", pIniLimitOfASM->wchHC2540_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"CO2540", L"", pIniLimitOfASM->wchCO2540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"CO2540_EDP", L"", pIniLimitOfASM->wchCO2540_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"NO2540", L"", pIniLimitOfASM->wchNO2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"NO2540_EDP", L"", pIniLimitOfASM->wchNO2540_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfASM", L"Remark", L"", pIniLimitOfASM->wchRemark, 1024);
	GetIniString(ini, L"LimitOfASM", L"Reserved1", L"", pIniLimitOfASM->wchReserved1, 50);
	GetIniString(ini, L"LimitOfASM", L"Reserved2", L"", pIniLimitOfASM->wchReserved2, 50);
	GetIniString(ini, L"LimitOfASM", L"Reserved3", L"", pIniLimitOfASM->wchReserved3, 50);
}

void SetIniLimitOfASM(const LIMITOFASM& iniLimitOfASM)
{
	// 获取LimitOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfASM.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfASM.ini信息
	WriteIniString(ini, L"LimitOfASM", L"HC5025", iniLimitOfASM.wchHC5025);
	WriteIniString(ini, L"LimitOfASM", L"HC5025_EDP", iniLimitOfASM.wchHC5025_EDP);
	WriteIniString(ini, L"LimitOfASM", L"CO5025", iniLimitOfASM.wchCO5025);
	WriteIniString(ini, L"LimitOfASM", L"CO5025_EDP", iniLimitOfASM.wchCO5025_EDP);
	WriteIniString(ini, L"LimitOfASM", L"NO5025", iniLimitOfASM.wchNO5025);
	WriteIniString(ini, L"LimitOfASM", L"NO5025_EDP", iniLimitOfASM.wchNO5025_EDP);
	WriteIniString(ini, L"LimitOfASM", L"HC2540", iniLimitOfASM.wchHC2540);
	WriteIniString(ini, L"LimitOfASM", L"HC2540_EDP", iniLimitOfASM.wchHC2540_EDP);
	WriteIniString(ini, L"LimitOfASM", L"CO2540", iniLimitOfASM.wchCO2540);
	WriteIniString(ini, L"LimitOfASM", L"CO2540_EDP", iniLimitOfASM.wchCO2540_EDP);
	WriteIniString(ini, L"LimitOfASM", L"NO2540", iniLimitOfASM.wchNO2540);
	WriteIniString(ini, L"LimitOfASM", L"NO2540_EDP", iniLimitOfASM.wchNO2540_EDP);
	
	WriteIniString(ini, L"LimitOfASM", L"Remark", iniLimitOfASM.wchRemark);
	WriteIniString(ini, L"LimitOfASM", L"Reserved1", iniLimitOfASM.wchReserved1);
	WriteIniString(ini, L"LimitOfASM", L"Reserved2", iniLimitOfASM.wchReserved2);
	WriteIniString(ini, L"LimitOfASM", L"Reserved3", iniLimitOfASM.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniLimitOfVMAS(PLIMITOFVMAS const pIniLimitOfVMAS)
{
	// 获取LimitOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfVMAS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfVMAS.ini信息
	GetIniString(ini, L"LimitOfVMAS", L"HC", L"", pIniLimitOfVMAS->wchHC, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"HC_EDP", L"", pIniLimitOfVMAS->wchHC_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"CO", L"", pIniLimitOfVMAS->wchCO, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"CO_EDP", L"", pIniLimitOfVMAS->wchCO_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"NOx", L"", pIniLimitOfVMAS->wchNOx, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"NOx_EDP", L"", pIniLimitOfVMAS->wchNOx_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"HCNOx", L"", pIniLimitOfVMAS->wchHCNOx, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"HCNOx_EDP", L"", pIniLimitOfVMAS->wchHCNOx_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"SelectType", L"", pIniLimitOfVMAS->wchSelectType, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfVMAS", L"Remark", L"", pIniLimitOfVMAS->wchRemark, 1024);
	GetIniString(ini, L"LimitOfVMAS", L"Reserved1", L"", pIniLimitOfVMAS->wchReserved1, 50);
	GetIniString(ini, L"LimitOfVMAS", L"Reserved2", L"", pIniLimitOfVMAS->wchReserved2, 50);
	GetIniString(ini, L"LimitOfVMAS", L"Reserved3", L"", pIniLimitOfVMAS->wchReserved3, 50);
}

void SetIniLimitOfVMAS(const LIMITOFVMAS& iniLimitOfVMAS)
{
	// 获取LimitOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfVMAS.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfVMAS.ini信息
	WriteIniString(ini, L"LimitOfVMAS", L"HC", iniLimitOfVMAS.wchHC);
	WriteIniString(ini, L"LimitOfVMAS", L"HC_EDP", iniLimitOfVMAS.wchHC_EDP);
	WriteIniString(ini, L"LimitOfVMAS", L"CO", iniLimitOfVMAS.wchCO);
	WriteIniString(ini, L"LimitOfVMAS", L"CO_EDP", iniLimitOfVMAS.wchCO_EDP);
	WriteIniString(ini, L"LimitOfVMAS", L"NOx", iniLimitOfVMAS.wchNOx);
	WriteIniString(ini, L"LimitOfVMAS", L"NOx_EDP", iniLimitOfVMAS.wchNOx_EDP);
	WriteIniString(ini, L"LimitOfVMAS", L"HCNOx", iniLimitOfVMAS.wchHCNOx);
	WriteIniString(ini, L"LimitOfVMAS", L"HCNOx_EDP", iniLimitOfVMAS.wchHCNOx_EDP);
	WriteIniString(ini, L"LimitOfVMAS", L"SelectType", iniLimitOfVMAS.wchSelectType);
	WriteIniString(ini, L"LimitOfVMAS", L"Remark", iniLimitOfVMAS.wchRemark);
	WriteIniString(ini, L"LimitOfVMAS", L"Reserved1", iniLimitOfVMAS.wchReserved1);
	WriteIniString(ini, L"LimitOfVMAS", L"Reserved2", iniLimitOfVMAS.wchReserved2);
	WriteIniString(ini, L"LimitOfVMAS", L"Reserved3", iniLimitOfVMAS.wchReserved3);
	ini.SaveFile(wchPath);
}

void GetIniLimitOfLUGDOWN(PLIMITOFLUGDOWN const pIniLimitOfLUGDOWN)
{
	// 获取LimitOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfLUGDOWN.ini", wchPath))
	{
		return;
	}
	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfLUGDOWN.ini信息
	GetIniString(ini, L"LimitOfLUGDOWN", L"K", L"", pIniLimitOfLUGDOWN->wchK, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfLUGDOWN", L"K_EDP", L"", pIniLimitOfLUGDOWN->wchK_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfLUGDOWN", L"N", L"", pIniLimitOfLUGDOWN->wchN, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfLUGDOWN", L"N_EDP", L"", pIniLimitOfLUGDOWN->wchN_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfLUGDOWN", L"NOx", L"", pIniLimitOfLUGDOWN->wchNOx, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfLUGDOWN", L"Remark", L"", pIniLimitOfLUGDOWN->wchRemark, 1024);
	GetIniString(ini, L"LimitOfLUGDOWN", L"Reserved1", L"", pIniLimitOfLUGDOWN->wchReserved1, 50);
	GetIniString(ini, L"LimitOfLUGDOWN", L"Reserved2", L"", pIniLimitOfLUGDOWN->wchReserved2, 50);
	GetIniString(ini, L"LimitOfLUGDOWN", L"Reserved3", L"", pIniLimitOfLUGDOWN->wchReserved3, 50);
}

void SetIniLimitOfLUGDOWN(const LIMITOFLUGDOWN& iniLimitOfLUGDOWN)
{
	// 获取LimitOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfLUGDOWN.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfLUGDOWN.ini信息
	WriteIniString(ini, L"LimitOfLUGDOWN", L"K", iniLimitOfLUGDOWN.wchK);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"K_EDP", iniLimitOfLUGDOWN.wchK_EDP);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"N", iniLimitOfLUGDOWN.wchN);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"N_EDP", iniLimitOfLUGDOWN.wchN_EDP);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"NOx", iniLimitOfLUGDOWN.wchNOx);

	WriteIniString(ini, L"LimitOfLUGDOWN", L"Remark", iniLimitOfLUGDOWN.wchRemark);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"Reserved1", iniLimitOfLUGDOWN.wchReserved1);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"Reserved2", iniLimitOfLUGDOWN.wchReserved2);
	WriteIniString(ini, L"LimitOfLUGDOWN", L"Reserved3", iniLimitOfLUGDOWN.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniLimitOfDIS(PLIMITOFDIS const pIniLimitOfDIS)
{
	// 获取LimitOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfASM.ini信息
	GetIniString(ini, L"LimitOfDIS", L"HCHigh", L"", pIniLimitOfDIS->wchHCHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"HCHigh_EDP", L"", pIniLimitOfDIS->wchHCHigh_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"COHigh", L"", pIniLimitOfDIS->wchCOHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"COHigh_EDP", L"", pIniLimitOfDIS->wchCOHigh_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"HCLow", L"", pIniLimitOfDIS->wchHCLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"HCLow_EDP", L"", pIniLimitOfDIS->wchHCLow_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"COLow", L"", pIniLimitOfDIS->wchCOLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"COLow_EDP", L"", pIniLimitOfDIS->wchCOLow_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"HISLambdaLow", L"", pIniLimitOfDIS->wchLambdaLower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"HISLambdaHigh", L"", pIniLimitOfDIS->wchLambdaUpper, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfDIS", L"Remark", L"", pIniLimitOfDIS->wchRemark, 1024);
	GetIniString(ini, L"LimitOfDIS", L"Reserved1", L"", pIniLimitOfDIS->wchReserved1, 50);
	GetIniString(ini, L"LimitOfDIS", L"Reserved2", L"", pIniLimitOfDIS->wchReserved2, 50);
	GetIniString(ini, L"LimitOfDIS", L"Reserved3", L"", pIniLimitOfDIS->wchReserved3, 50);
}

void SetIniLimitOfDIS(const LIMITOFDIS& iniLimitOfDIS)
{
	// 获取LimitOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfDIS.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfASM.ini信息
	WriteIniString(ini, L"LimitOfDIS", L"HCHigh", iniLimitOfDIS.wchHCHigh);
	WriteIniString(ini, L"LimitOfDIS", L"HCHigh_EDP", iniLimitOfDIS.wchHCHigh_EDP);
	WriteIniString(ini, L"LimitOfDIS", L"COHigh", iniLimitOfDIS.wchCOHigh);
	WriteIniString(ini, L"LimitOfDIS", L"COHigh_EDP", iniLimitOfDIS.wchCOHigh_EDP);
	WriteIniString(ini, L"LimitOfDIS", L"HCLow", iniLimitOfDIS.wchHCLow);
	WriteIniString(ini, L"LimitOfDIS", L"HCLow_EDP", iniLimitOfDIS.wchHCLow_EDP);
	WriteIniString(ini, L"LimitOfDIS", L"COLow", iniLimitOfDIS.wchCOLow);
	WriteIniString(ini, L"LimitOfDIS", L"COLow_EDP", iniLimitOfDIS.wchCOLow_EDP);
	WriteIniString(ini, L"LimitOfDIS", L"HISLambdaLow", iniLimitOfDIS.wchLambdaLower);
	WriteIniString(ini, L"LimitOfDIS", L"HISLambdaHigh", iniLimitOfDIS.wchLambdaUpper);
	WriteIniString(ini, L"LimitOfDIS", L"Remark", iniLimitOfDIS.wchRemark);
	WriteIniString(ini, L"LimitOfDIS", L"Reserved1", iniLimitOfDIS.wchReserved1);
	WriteIniString(ini, L"LimitOfDIS", L"Reserved2", iniLimitOfDIS.wchReserved2);
	WriteIniString(ini, L"LimitOfDIS", L"Reserved3", iniLimitOfDIS.wchReserved3);
	ini.SaveFile(wchPath);
}

void GetIniLimitOfFSUNHT(PLIMITOFFSUNHT const pIniLimitOfFSUNHT)
{
	// 获取LimitOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUNHT.ini", wchPath))
	{
		return;
	}
	
	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfFSUNHT.ini信息
	GetIniString(ini, L"LimitOfFSUNHT", L"K", L"", pIniLimitOfFSUNHT->wchK, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHT", L"K_EDP", L"", pIniLimitOfFSUNHT->wchK_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHT", L"Remark", L"", pIniLimitOfFSUNHT->wchRemark, 1024);
	GetIniString(ini, L"LimitOfFSUNHT", L"Reserved1", L"", pIniLimitOfFSUNHT->wchReserved1, 50);
	GetIniString(ini, L"LimitOfFSUNHT", L"Reserved2", L"", pIniLimitOfFSUNHT->wchReserved2, 50);
	GetIniString(ini, L"LimitOfFSUNHT", L"Reserved3", L"", pIniLimitOfFSUNHT->wchReserved3, 50);
}

void SetIniLimitOfFSUNHT(const LIMITOFFSUNHT& iniLimitOfFSUNHT)
{
	// 获取LimitOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUNHT.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfFSUNHT.ini信息
	WriteIniString(ini, L"LimitOfFSUNHT", L"K", iniLimitOfFSUNHT.wchK);
	WriteIniString(ini, L"LimitOfFSUNHT", L"K_EDP", iniLimitOfFSUNHT.wchK_EDP);
	WriteIniString(ini, L"LimitOfFSUNHT", L"Remark", iniLimitOfFSUNHT.wchRemark);
	WriteIniString(ini, L"LimitOfFSUNHT", L"Reserved1", iniLimitOfFSUNHT.wchReserved1);
	WriteIniString(ini, L"LimitOfFSUNHT", L"Reserved2", iniLimitOfFSUNHT.wchReserved2);
	WriteIniString(ini, L"LimitOfFSUNHT", L"Reserved3", iniLimitOfFSUNHT.wchReserved3);
	ini.SaveFile(wchPath);
}

void GetIniLimitOfFSUNHTOfHeavyVehicle(PLIMITOFFSUNHTOFHEAVYVEHICLE const pIniLimitOfFSUNHTOfHeavyVehicle)
{
	// 获取LimitOfFSUNHTOfHeavyVehicle.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUNHTOfHeavyVehicle.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfFSUNHTOfHeavyVehicle.ini信息
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"TypeApprovalLimit", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchTypeApprovalLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"TypeApprovalLimit_EDP", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchTypeApprovalLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Limit", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Limit_EDP", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Remark", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchRemark, 1024);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Reserved1", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchReserved1, 50);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Reserved2", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchReserved2, 50);
	GetIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Reserved3", L"", pIniLimitOfFSUNHTOfHeavyVehicle->wchReserved3, 50);
}

void SetIniLimitOfFSUNHTOfHeavyVehicle(const LIMITOFFSUNHTOFHEAVYVEHICLE& iniLimitOfFSUNHTOfHeavyVehicle)
{
	// 获取LimitOfFSUNHTOfHeavyVehicle.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUNHTOfHeavyVehicle.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfFSUNHTOfHeavyVehicle.ini信息
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"TypeApprovalLimit", iniLimitOfFSUNHTOfHeavyVehicle.wchTypeApprovalLimit);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"TypeApprovalLimit_EDP", iniLimitOfFSUNHTOfHeavyVehicle.wchTypeApprovalLimit_EDP);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Limit", iniLimitOfFSUNHTOfHeavyVehicle.wchLimit);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Limit_EDP", iniLimitOfFSUNHTOfHeavyVehicle.wchLimit_EDP);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Remark", iniLimitOfFSUNHTOfHeavyVehicle.wchRemark);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Reserved1", iniLimitOfFSUNHTOfHeavyVehicle.wchReserved1);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Reserved2", iniLimitOfFSUNHTOfHeavyVehicle.wchReserved2);
	WriteIniString(ini, L"LimitOfFSUNHTOfHeavyVehicle", L"Reserved3", iniLimitOfFSUNHTOfHeavyVehicle.wchReserved3);
	ini.SaveFile(wchPath);
}

void GetIniLimitOfFSUNHTOfLightVehicle(PLIMITOFFSUNHTOFLIGHTVEHICLE const pIniLimitOfFSUNHTOfLightVehicle)
{
	// 获取LimitOfFSUNHTOfLightVehicle.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUNHTOfLightVehicle.ini", wchPath))
	{
		return;
	}
	
	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfFSUNHTOfLightVehicle.ini信息
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"TypeApprovalLimit", L"", pIniLimitOfFSUNHTOfLightVehicle->wchTypeApprovalLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"TypeApprovalLimit_EDP", L"", pIniLimitOfFSUNHTOfLightVehicle->wchTypeApprovalLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Limit", L"", pIniLimitOfFSUNHTOfLightVehicle->wchLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Limit_EDP", L"", pIniLimitOfFSUNHTOfLightVehicle->wchLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Remark", L"", pIniLimitOfFSUNHTOfLightVehicle->wchRemark, 1024);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Reserved1", L"", pIniLimitOfFSUNHTOfLightVehicle->wchReserved1, 50);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Reserved2", L"", pIniLimitOfFSUNHTOfLightVehicle->wchReserved2, 50);
	GetIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Reserved3", L"", pIniLimitOfFSUNHTOfLightVehicle->wchReserved3, 50);
}

void SetIniLimitOfFSUNHTOfLightVehicle(const LIMITOFFSUNHTOFLIGHTVEHICLE& iniLimitOfFSUNHTOfLightVehicle)
{
	// 获取LimitOfFSUNHTOfLightVehicle.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUNHTOfLightVehicle.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfFSUNHTOfHeavyVehicle.ini信息
	WriteIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"TypeApprovalLimit", iniLimitOfFSUNHTOfLightVehicle.wchTypeApprovalLimit);
	WriteIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"TypeApprovalLimit_EDP", iniLimitOfFSUNHTOfLightVehicle.wchTypeApprovalLimit_EDP);
	WriteIniString(ini, L"LimitOfFSUNHTOfLightVehicle", L"Limit", iniLimitOfFSUNHTOfLightVehicle.wchLimit);
	WriteIniString(ini, L"LimitOfFSUNHTOfLigtVehicle", L"Limit_EDP", iniLimitOfFSUNHTOfLightVehicle.wchLimit_EDP);
	WriteIniString(ini, L"LimitOfFSUNHTOfLigtVehicle", L"Remark", iniLimitOfFSUNHTOfLightVehicle.wchRemark);
	WriteIniString(ini, L"LimitOfFSUNHTOfLigtVehicle", L"Reserved1", iniLimitOfFSUNHTOfLightVehicle.wchReserved1);
	WriteIniString(ini, L"LimitOfFSUNHTOfLigtVehicle", L"Reserved2", iniLimitOfFSUNHTOfLightVehicle.wchReserved2);
	WriteIniString(ini, L"LimitOfFSUNHTOfLigtVehicle", L"Reserved3", iniLimitOfFSUNHTOfLightVehicle.wchReserved3);
	ini.SaveFile(wchPath);
}

void GetIniLimitOfFSUYD(PLIMITOFFSUYD const pIniLimitOfFSUYD)
{
	// 获取LimitOfFSUYD.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUYD.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LimitOfFSUYD.ini信息
	GetIniString(ini, L"LimitOfFSUYD", L"Smoke", L"", pIniLimitOfFSUYD->wchSmoke, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUYD", L"Smoke_EDP", L"", pIniLimitOfFSUYD->wchSmoke_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"LimitOfFSUYD", L"Remark", L"", pIniLimitOfFSUYD->wchRemark, 1024);
	GetIniString(ini, L"LimitOfFSUYD", L"Reserved1", L"", pIniLimitOfFSUYD->wchReserved1, 50);
	GetIniString(ini, L"LimitOfFSUYD", L"Reserved2", L"", pIniLimitOfFSUYD->wchReserved2, 50);
	GetIniString(ini, L"LimitOfFSUYD", L"Reserved3", L"", pIniLimitOfFSUYD->wchReserved3, 50);
}

void SetIniLimitOfFSUYD(const LIMITOFFSUYD& iniLimitOfFSUYD)
{
	// 获取LimitOfFSUYD.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LimitOfFSUYD.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LimitOfFSUYD.ini信息
	WriteIniString(ini, L"LimitOfFSUYD", L"Smoke", iniLimitOfFSUYD.wchSmoke);
	WriteIniString(ini, L"LimitOfFSUYD", L"Smoke_EDP", iniLimitOfFSUYD.wchSmoke_EDP);
	WriteIniString(ini, L"LimitOfFSUYD", L"Remark", iniLimitOfFSUYD.wchRemark);
	WriteIniString(ini, L"LimitOfFSUYD", L"Reserved1", iniLimitOfFSUYD.wchReserved1);
	WriteIniString(ini, L"LimitOfFSUYD", L"Reserved2", iniLimitOfFSUYD.wchReserved2);
	WriteIniString(ini, L"LimitOfFSUYD", L"Reserved3", iniLimitOfFSUYD.wchReserved3);
	ini.SaveFile(wchPath);
}

void GetIniLineInfo(PLINEINFO const pIniLineInfo)
{
	// 获取LineInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取LineInfo.ini信息
	GetIniString(ini, L"LineInfo", L"AutoID", L"", pIniLineInfo->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"LineNumber", L"", pIniLineInfo->wchLineNumber, 50);
	GetIniString(ini, L"LineInfo", L"LineModel", L"", pIniLineInfo->wchLineModel, 50);
	GetIniString(ini, L"LineInfo", L"LineName", L"", pIniLineInfo->wchLineName, 50);
	GetIniString(ini, L"LineInfo", L"LineManufacturer", L"", pIniLineInfo->wchLineManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"LineLicenseSn", L"", pIniLineInfo->wchLineLicenseSn, 50);
	GetIniString(ini, L"LineInfo", L"LineLicenseStartDate", L"", pIniLineInfo->wchLineLicenseStartDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"LineLicenseEndDate", L"", pIniLineInfo->wchLineLicenseEndDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"LineIsLocked", L"", pIniLineInfo->wchLineIsLocked, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"LineState", L"", pIniLineInfo->wchLineState, NH_INT_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"LineStateChangeTime", L"", pIniLineInfo->wchLineStateChangeTime, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"LineRegistrationInfo", L"", pIniLineInfo->wchLineRegistrationInfo, NH_INT_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasDynamometer", L"", pIniLineInfo->wchHasDynamometer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"DynamometerNumber", L"", pIniLineInfo->wchDynamometerNumber, 50);
	GetIniString(ini, L"LineInfo", L"DynamometerModel", L"", pIniLineInfo->wchDynamometerModel, 50);
	GetIniString(ini, L"LineInfo", L"DynamometerManufacturer", L"", pIniLineInfo->wchDynamometerManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"DynamometerCoastDownDate", L"", pIniLineInfo->wchDynamometerCoastDownDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"DynamometerPLHPDate", L"", pIniLineInfo->wchDynamometerPLHPDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"DynamometerForceCalibrationDate", L"", pIniLineInfo->wchDynamometerForceCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"DynamometerForceCheckDate", L"", pIniLineInfo->wchDynamometerForceCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"DynamometerAuthenticationDate", L"", pIniLineInfo->wchDynamometerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasAnalyzer", L"", pIniLineInfo->wchHasAnalyzer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"AnalyzerNumber", L"", pIniLineInfo->wchAnalyzerNumber, 50);
	GetIniString(ini, L"LineInfo", L"AnalyzerModel", L"", pIniLineInfo->wchAnalyzerModel, 50);
	GetIniString(ini, L"LineInfo", L"AnalyzerManufacturer", L"", pIniLineInfo->wchAnalyzerManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"AnalyzerChannelCalibrationDate", L"", pIniLineInfo->wchAnalyzerChannelCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"AnalyzerChannelCheckDate", L"", pIniLineInfo->wchAnalyzerChannelCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"AnalyzerEnvParamCalibrationDate", L"", pIniLineInfo->wchAnalyzerEnvParamCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"AnalyzerAuthenticationDate", L"", pIniLineInfo->wchAnalyzerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasFlowmeter", L"", pIniLineInfo->wchHasFlowmeter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"FlowmeterNumber", L"", pIniLineInfo->wchFlowmeterNumber, 50);
	GetIniString(ini, L"LineInfo", L"FlowmeterModel", L"", pIniLineInfo->wchFlowmeterModel, 50);
	GetIniString(ini, L"LineInfo", L"FlowmeterManufacturer", L"", pIniLineInfo->wchFlowmeterManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"FlowmeterFluxCalibrationDate", L"", pIniLineInfo->wchFlowmeterFluxCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"FlowmeterFluxCheckDate", L"", pIniLineInfo->wchFlowmeterFluxCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"FlowmeterDilO2CalibrationDate", L"", pIniLineInfo->wchFlowmeterDilO2CalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"FlowmeterPressureCalibrationDate", L"", pIniLineInfo->wchFlowmeterPressureCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"FlowmeterAuthenticationDate", L"", pIniLineInfo->wchFlowmeterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasOpacimeter", L"", pIniLineInfo->wchHasOpacimeter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OpacimeterNumber", L"", pIniLineInfo->wchOpacimeterNumber, 50);
	GetIniString(ini, L"LineInfo", L"OpacimeterModel", L"", pIniLineInfo->wchOpacimeterModel, 50);
	GetIniString(ini, L"LineInfo", L"OpacimeterManufacturer", L"", pIniLineInfo->wchOpacimeterManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"OpacimeterChannelCalibrationDate", L"", pIniLineInfo->wchOpacimeterChannelCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OpacimeterChannelCheckDate", L"", pIniLineInfo->wchOpacimeterChannelCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OpacimeterEnvParamCalibrationDate", L"", pIniLineInfo->wchOpacimeterEnvParamCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OpacimeterAuthenticationDate", L"", pIniLineInfo->wchOpacimeterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasSmokemeter", L"", pIniLineInfo->wchHasSmokemeter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"SmokemeterNumber", L"", pIniLineInfo->wchSmokemeterNumber, 50);
	GetIniString(ini, L"LineInfo", L"SmokemeterModel", L"", pIniLineInfo->wchSmokemeterModel, 50);
	GetIniString(ini, L"LineInfo", L"SmokemeterManufacturer", L"", pIniLineInfo->wchSmokemeterManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"SmokemeterCalibrationDate", L"", pIniLineInfo->wchSmokemeterCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"SmokemeterCheckDate", L"", pIniLineInfo->wchSmokemeterCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"SmokemeterAuthenticationDate", L"", pIniLineInfo->wchSmokemeterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasIndependentEnvParamMeasurer", L"", pIniLineInfo->wchHasIndependentEnvParamMeasurer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"EnvParamMeasurerNumber", L"", pIniLineInfo->wchEnvParamMeasurerNumber, 50);
	GetIniString(ini, L"LineInfo", L"EnvParamMeasurerModel", L"", pIniLineInfo->wchEnvParamMeasurerModel, 50);
	GetIniString(ini, L"LineInfo", L"EnvParamMeasurerManufacturer", L"", pIniLineInfo->wchEnvParamMeasurerManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"EnvParamMeasurerCalibrationDate", L"", pIniLineInfo->wchEnvParamMeasurerCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"EnvParamMeasurerCheckDate", L"", pIniLineInfo->wchEnvParamMeasurerCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"EnvParamMeasurerAuthenticationDate", L"", pIniLineInfo->wchEnvParamMeasurerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasIndependentTachometer", L"", pIniLineInfo->wchHasIndependentTachometer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"TachometerNumber", L"", pIniLineInfo->wchTachometerNumber, 50);
	GetIniString(ini, L"LineInfo", L"TachometerModel", L"", pIniLineInfo->wchTachometerModel, 50);
	GetIniString(ini, L"LineInfo", L"TachometerManufacturer", L"", pIniLineInfo->wchTachometerManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"TachometerCalibrationDate", L"", pIniLineInfo->wchTachometerCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"TachometerCheckDate", L"", pIniLineInfo->wchTachometerCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"TachometerAuthenticationDate", L"", pIniLineInfo->wchTachometerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasOBDAdapter", L"", pIniLineInfo->wchHasOBDAdapter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OBDAdapterNumber", L"", pIniLineInfo->wchOBDAdapterNumber, 50);
	GetIniString(ini, L"LineInfo", L"OBDAdapterModel", L"", pIniLineInfo->wchOBDAdapterModel, 50);
	GetIniString(ini, L"LineInfo", L"OBDAdapterManufacturer", L"", pIniLineInfo->wchOBDAdapterManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"OBDAdapterCalibrationDate", L"", pIniLineInfo->wchOBDAdapterCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OBDAdapterCheckDate", L"", pIniLineInfo->wchOBDAdapterCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"OBDAdapterAuthenticationDate", L"", pIniLineInfo->wchOBDAdapterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"HasNOxAnalyzer", L"", pIniLineInfo->wchHasNOxAnalyzer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerNumber", L"", pIniLineInfo->wchNOxAnalyzerNumber, 50);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerModel", L"", pIniLineInfo->wchNOxAnalyzerModel, 50);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerManufacturer", L"", pIniLineInfo->wchNOxAnalyzerManufacturer, 50);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerChannelCalibrationDate", L"", pIniLineInfo->wchNOxAnalyzerChannelCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerChannelCheckDate", L"", pIniLineInfo->wchNOxAnalyzerChannelCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerEnvParamCalibrationDate", L"", pIniLineInfo->wchNOxAnalyzerEnvParamCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"NOxAnalyzerAuthenticationDate", L"", pIniLineInfo->wchNOxAnalyzerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"LineInfo", L"TerminationSign", L"", pIniLineInfo->wchTerminationSign, NH_INT_TO_WCHART);
	//GetIniString(ini, L"LineInfo", L"CertificateAuthentication", L"", pIniLineInfo->wchCertificateAuthentication, 100);
	GetIniString(ini, L"LineInfo", L"Remark", L"", pIniLineInfo->wchRemark, 1024);
	GetIniString(ini, L"LineInfo", L"Reserved1", L"", pIniLineInfo->wchReserved1, 50);
	GetIniString(ini, L"LineInfo", L"Reserved2", L"", pIniLineInfo->wchReserved2, 50);
	GetIniString(ini, L"LineInfo", L"Reserved3", L"", pIniLineInfo->wchReserved3, 50);

}

void SetIniLineInfo(const LINEINFO& iniLineInfo)
{
	// 获取LineInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取LineInfo.ini信息
	WriteIniString(ini, L"LineInfo", L"AutoID", iniLineInfo.wchAutoID);
	WriteIniString(ini, L"LineInfo", L"LineNumber", iniLineInfo.wchLineNumber);
	WriteIniString(ini, L"LineInfo", L"LineModel", iniLineInfo.wchLineModel);
	WriteIniString(ini, L"LineInfo", L"LineName", iniLineInfo.wchLineName);
	WriteIniString(ini, L"LineInfo", L"LineManufacturer", iniLineInfo.wchLineManufacturer);
	WriteIniString(ini, L"LineInfo", L"LineLicenseSn", iniLineInfo.wchLineLicenseSn);
	WriteIniString(ini, L"LineInfo", L"LineLicenseStartDate", iniLineInfo.wchLineLicenseStartDate);
	WriteIniString(ini, L"LineInfo", L"LineLicenseEndDate", iniLineInfo.wchLineLicenseEndDate);
	WriteIniString(ini, L"LineInfo", L"LineIsLocked", iniLineInfo.wchLineIsLocked);
	WriteIniString(ini, L"LineInfo", L"LineState", iniLineInfo.wchLineState);
	WriteIniString(ini, L"LineInfo", L"LineStateChangeTime", iniLineInfo.wchLineStateChangeTime);
	WriteIniString(ini, L"LineInfo", L"LineRegistrationInfo", iniLineInfo.wchLineRegistrationInfo);
	WriteIniString(ini, L"LineInfo", L"HasDynamometer", iniLineInfo.wchHasDynamometer);
	WriteIniString(ini, L"LineInfo", L"DynamometerNumber", iniLineInfo.wchDynamometerNumber);
	WriteIniString(ini, L"LineInfo", L"DynamometerModel", iniLineInfo.wchDynamometerModel);
	WriteIniString(ini, L"LineInfo", L"DynamometerManufacturer", iniLineInfo.wchDynamometerManufacturer);
	WriteIniString(ini, L"LineInfo", L"DynamometerCoastDownDate", iniLineInfo.wchDynamometerCoastDownDate);
	WriteIniString(ini, L"LineInfo", L"DynamometerPLHPDate", iniLineInfo.wchDynamometerPLHPDate);
	WriteIniString(ini, L"LineInfo", L"DynamometerForceCalibrationDate", iniLineInfo.wchDynamometerForceCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"DynamometerForceCheckDate", iniLineInfo.wchDynamometerForceCheckDate);
	WriteIniString(ini, L"LineInfo", L"DynamometerAuthenticationDate", iniLineInfo.wchDynamometerAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasAnalyzer", iniLineInfo.wchHasAnalyzer);
	WriteIniString(ini, L"LineInfo", L"AnalyzerNumber", iniLineInfo.wchAnalyzerNumber);
	WriteIniString(ini, L"LineInfo", L"AnalyzerModel", iniLineInfo.wchAnalyzerModel);
	WriteIniString(ini, L"LineInfo", L"AnalyzerManufacturer", iniLineInfo.wchAnalyzerManufacturer);
	WriteIniString(ini, L"LineInfo", L"AnalyzerChannelCalibrationDate", iniLineInfo.wchAnalyzerChannelCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"AnalyzerChannelCheckDate", iniLineInfo.wchAnalyzerChannelCheckDate);
	WriteIniString(ini, L"LineInfo", L"AnalyzerEnvParamCalibrationDate", iniLineInfo.wchAnalyzerEnvParamCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"AnalyzerAuthenticationDate", iniLineInfo.wchAnalyzerAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasFlowmeter", iniLineInfo.wchHasFlowmeter);
	WriteIniString(ini, L"LineInfo", L"FlowmeterNumber", iniLineInfo.wchFlowmeterNumber);
	WriteIniString(ini, L"LineInfo", L"FlowmeterModel", iniLineInfo.wchFlowmeterModel);
	WriteIniString(ini, L"LineInfo", L"FlowmeterManufacturer", iniLineInfo.wchFlowmeterManufacturer);
	WriteIniString(ini, L"LineInfo", L"FlowmeterFluxCalibrationDate", iniLineInfo.wchFlowmeterFluxCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"FlowmeterFluxCheckDate", iniLineInfo.wchFlowmeterFluxCheckDate);
	WriteIniString(ini, L"LineInfo", L"FlowmeterDilO2CalibrationDate", iniLineInfo.wchFlowmeterDilO2CalibrationDate);
	WriteIniString(ini, L"LineInfo", L"FlowmeterPressureCalibrationDate", iniLineInfo.wchFlowmeterPressureCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"FlowmeterAuthenticationDate", iniLineInfo.wchFlowmeterAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasOpacimeter", iniLineInfo.wchHasOpacimeter);
	WriteIniString(ini, L"LineInfo", L"OpacimeterNumber", iniLineInfo.wchOpacimeterNumber);
	WriteIniString(ini, L"LineInfo", L"OpacimeterModel", iniLineInfo.wchOpacimeterModel);
	WriteIniString(ini, L"LineInfo", L"OpacimeterManufacturer", iniLineInfo.wchOpacimeterManufacturer);
	WriteIniString(ini, L"LineInfo", L"OpacimeterChannelCalibrationDate", iniLineInfo.wchOpacimeterChannelCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"OpacimeterChannelCheckDate", iniLineInfo.wchOpacimeterChannelCheckDate);
	WriteIniString(ini, L"LineInfo", L"OpacimeterEnvParamCalibrationDate", iniLineInfo.wchOpacimeterEnvParamCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"OpacimeterAuthenticationDate", iniLineInfo.wchOpacimeterAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasSmokemeter", iniLineInfo.wchHasSmokemeter);
	WriteIniString(ini, L"LineInfo", L"SmokemeterNumber", iniLineInfo.wchSmokemeterNumber);
	WriteIniString(ini, L"LineInfo", L"SmokemeterModel", iniLineInfo.wchSmokemeterModel);
	WriteIniString(ini, L"LineInfo", L"SmokemeterManufacturer", iniLineInfo.wchSmokemeterManufacturer);
	WriteIniString(ini, L"LineInfo", L"SmokemeterCalibrationDate", iniLineInfo.wchSmokemeterCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"SmokemeterCheckDate", iniLineInfo.wchSmokemeterCheckDate);
	WriteIniString(ini, L"LineInfo", L"SmokemeterAuthenticationDate", iniLineInfo.wchSmokemeterAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasIndependentEnvParamMeasurer", iniLineInfo.wchHasIndependentEnvParamMeasurer);
	WriteIniString(ini, L"LineInfo", L"EnvParamMeasurerNumber", iniLineInfo.wchEnvParamMeasurerNumber);
	WriteIniString(ini, L"LineInfo", L"EnvParamMeasurerModel", iniLineInfo.wchEnvParamMeasurerModel);
	WriteIniString(ini, L"LineInfo", L"EnvParamMeasurerManufacturer", iniLineInfo.wchEnvParamMeasurerManufacturer);
	WriteIniString(ini, L"LineInfo", L"EnvParamMeasurerCalibrationDate", iniLineInfo.wchEnvParamMeasurerCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"EnvParamMeasurerCheckDate", iniLineInfo.wchEnvParamMeasurerCheckDate);
	WriteIniString(ini, L"LineInfo", L"EnvParamMeasurerAuthenticationDate", iniLineInfo.wchEnvParamMeasurerAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasIndependentTachometer", iniLineInfo.wchHasIndependentTachometer);
	WriteIniString(ini, L"LineInfo", L"TachometerNumber", iniLineInfo.wchTachometerNumber);
	WriteIniString(ini, L"LineInfo", L"TachometerModel", iniLineInfo.wchTachometerModel);
	WriteIniString(ini, L"LineInfo", L"TachometerManufacturer", iniLineInfo.wchTachometerManufacturer);
	WriteIniString(ini, L"LineInfo", L"TachometerCalibrationDate", iniLineInfo.wchTachometerCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"TachometerCheckDate", iniLineInfo.wchTachometerCheckDate);
	WriteIniString(ini, L"LineInfo", L"TachometerAuthenticationDate", iniLineInfo.wchTachometerAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasOBDAdapter", iniLineInfo.wchHasOBDAdapter);
	WriteIniString(ini, L"LineInfo", L"OBDAdapterNumber", iniLineInfo.wchOBDAdapterNumber);
	WriteIniString(ini, L"LineInfo", L"OBDAdapterModel", iniLineInfo.wchOBDAdapterModel);
	WriteIniString(ini, L"LineInfo", L"OBDAdapterManufacturer", iniLineInfo.wchOBDAdapterManufacturer);
	WriteIniString(ini, L"LineInfo", L"OBDAdapterCalibrationDate", iniLineInfo.wchOBDAdapterCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"OBDAdapterCheckDate", iniLineInfo.wchOBDAdapterCheckDate);
	WriteIniString(ini, L"LineInfo", L"OBDAdapterAuthenticationDate", iniLineInfo.wchOBDAdapterAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"HasNOxAnalyzer", iniLineInfo.wchHasNOxAnalyzer);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerNumber", iniLineInfo.wchNOxAnalyzerNumber);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerModel", iniLineInfo.wchNOxAnalyzerModel);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerManufacturer", iniLineInfo.wchNOxAnalyzerManufacturer);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerChannelCalibrationDate", iniLineInfo.wchNOxAnalyzerChannelCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerChannelCheckDate", iniLineInfo.wchNOxAnalyzerChannelCheckDate);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerEnvParamCalibrationDate", iniLineInfo.wchNOxAnalyzerEnvParamCalibrationDate);
	WriteIniString(ini, L"LineInfo", L"NOxAnalyzerAuthenticationDate", iniLineInfo.wchNOxAnalyzerAuthenticationDate);
	WriteIniString(ini, L"LineInfo", L"TerminationSign", iniLineInfo.wchTerminationSign);
	//WriteIniString(ini, L"LineInfo", L"CertificateAuthentication", iniLineInfo.wchCertificateAuthentication);
	WriteIniString(ini, L"LineInfo", L"Remark", iniLineInfo.wchRemark);
	WriteIniString(ini, L"LineInfo", L"Reserved1", iniLineInfo.wchReserved1);
	WriteIniString(ini, L"LineInfo", L"Reserved2", iniLineInfo.wchReserved2);
	WriteIniString(ini, L"LineInfo", L"Reserved3", iniLineInfo.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniStationInfo(PSTATIONINFO const pIniStationInfo)
{
	// 获取StationInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取StationInfo.ini信息
	GetIniString(ini, L"StationInfo", L"AutoID", L"", pIniStationInfo->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"StationInfo", L"StationNumber", L"", pIniStationInfo->wchStationNumber, 50);
	GetIniString(ini, L"StationInfo", L"StationFullName", L"", pIniStationInfo->wchStationFullName, 50);
	GetIniString(ini, L"StationInfo", L"StationShortName", L"", pIniStationInfo->wchStationShortName, 50);
	GetIniString(ini, L"StationInfo", L"StationLicenseSn", L"", pIniStationInfo->wchStationLicenseSn, 50);
	GetIniString(ini, L"StationInfo", L"StationLicenseStartDate", L"", pIniStationInfo->wchStationLicenseStartDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"StationInfo", L"StationLicenseEndDate", L"", pIniStationInfo->wchStationLicenseEndDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"StationInfo", L"StationIsLocked", L"", pIniStationInfo->wchStationIsLocked, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"StationInfo", L"StationRegistrationInfo", L"", pIniStationInfo->wchStationRegistrationInfo, NH_INT_TO_WCHART);
	GetIniString(ini, L"StationInfo", L"StationPhone", L"", pIniStationInfo->wchStationPhone, 50);
	GetIniString(ini, L"StationInfo", L"StationFax", L"", pIniStationInfo->wchStationFax, 50);
	GetIniString(ini, L"StationInfo", L"PostCode", L"", pIniStationInfo->wchPostCode, 50);
	GetIniString(ini, L"StationInfo", L"Province", L"", pIniStationInfo->wchProvince, 50);
	GetIniString(ini, L"StationInfo", L"City", L"", pIniStationInfo->wchCity, 50);
	GetIniString(ini, L"StationInfo", L"District", L"", pIniStationInfo->wchDistrict, 50);
	GetIniString(ini, L"StationInfo", L"DetailedAddress", L"", pIniStationInfo->wchDetailedAddress, 50);
	GetIniString(ini, L"StationInfo", L"Remark", L"", pIniStationInfo->wchRemark, 1024);
	GetIniString(ini, L"StationInfo", L"Reserved1", L"", pIniStationInfo->wchReserved1, 50);
	GetIniString(ini, L"StationInfo", L"Reserved2", L"", pIniStationInfo->wchReserved2, 50);
	GetIniString(ini, L"StationInfo", L"Reserved3", L"", pIniStationInfo->wchReserved3, 50);

}

void SetIniStationInfo(const STATIONINFO& iniStationInfo)
{
	// 获取StationInfo.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取StationInfo.ini信息
	WriteIniString(ini, L"StationInfo", L"AutoID", iniStationInfo.wchAutoID);
	WriteIniString(ini, L"StationInfo", L"StationNumber", iniStationInfo.wchStationNumber);
	WriteIniString(ini, L"StationInfo", L"StationFullName", iniStationInfo.wchStationFullName);
	WriteIniString(ini, L"StationInfo", L"StationShortName", iniStationInfo.wchStationShortName);
	WriteIniString(ini, L"StationInfo", L"StationLicenseSn", iniStationInfo.wchStationLicenseSn);
	WriteIniString(ini, L"StationInfo", L"StationLicenseStartDate", iniStationInfo.wchStationLicenseStartDate);
	WriteIniString(ini, L"StationInfo", L"StationLicenseEndDate", iniStationInfo.wchStationLicenseEndDate);
	WriteIniString(ini, L"StationInfo", L"StationIsLocked", iniStationInfo.wchStationIsLocked);
	WriteIniString(ini, L"StationInfo", L"StationRegistrationInfo", iniStationInfo.wchStationRegistrationInfo);
	WriteIniString(ini, L"StationInfo", L"StationPhone", iniStationInfo.wchStationPhone);
	WriteIniString(ini, L"StationInfo", L"StationFax", iniStationInfo.wchStationFax);
	WriteIniString(ini, L"StationInfo", L"PostCode", iniStationInfo.wchPostCode);
	WriteIniString(ini, L"StationInfo", L"Province", iniStationInfo.wchProvince);
	WriteIniString(ini, L"StationInfo", L"City", iniStationInfo.wchCity);
	WriteIniString(ini, L"StationInfo", L"District", iniStationInfo.wchDistrict);
	WriteIniString(ini, L"StationInfo", L"DetailedAddress", iniStationInfo.wchDetailedAddress);
	WriteIniString(ini, L"StationInfo", L"Remark", iniStationInfo.wchRemark);
	WriteIniString(ini, L"StationInfo", L"Reserved1", iniStationInfo.wchReserved1);
	WriteIniString(ini, L"StationInfo", L"Reserved2", iniStationInfo.wchReserved2);
	WriteIniString(ini, L"StationInfo", L"Reserved3", iniStationInfo.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniTestLog(PTESTLOG const pIniTestLog)
{
	// 获取TestLog.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取TestLog.ini信息
	GetIniString(ini, L"TestLog", L"AutoID", L"", pIniTestLog->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"RunningNumber", L"", pIniTestLog->wchRunningNumber, 50);
	GetIniString(ini, L"TestLog", L"ReportNumber", L"", pIniTestLog->wchReportNumber, 50);
	GetIniString(ini, L"TestLog", L"RunningNumberEP", L"", pIniTestLog->wchRunningNumberEP, 50);
	GetIniString(ini, L"TestLog", L"ReportNumberEP", L"", pIniTestLog->wchReportNumberEP, 50);
	GetIniString(ini, L"TestLog", L"ItemApp", L"", pIniTestLog->wchItemApp, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"ItemOBD", L"", pIniTestLog->wchItemOBD, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"ItemEmi", L"", pIniTestLog->wchItemEmi, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"BillDate", L"", pIniTestLog->wchBillDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"Biller", L"", pIniTestLog->wchBiller, 50);
	GetIniString(ini, L"TestLog", L"Shroff", L"", pIniTestLog->wchShroff, 50);
	GetIniString(ini, L"TestLog", L"ChargeState", L"", pIniTestLog->wchChargeState, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"InitialMoney", L"", pIniTestLog->wchInitialMoney, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"ChangedMoney", L"", pIniTestLog->wchChangedMoney, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"ReceivableMoney", L"", pIniTestLog->wchReceivableMoney, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"ActualMoney", L"", pIniTestLog->wchActualMoney, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TestLocation", L"", pIniTestLog->wchTestLocation, 50);
	GetIniString(ini, L"TestLog", L"TestState", L"", pIniTestLog->wchTestState, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsTestFinished", L"", pIniTestLog->wchIsTestFinished, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodASM", L"", pIniTestLog->wchNumberOfTestPeriodASM, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodVMAS", L"", pIniTestLog->wchNumberOfTestPeriodVMAS, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodLUGDOWN", L"", pIniTestLog->wchNumberOfTestPeriodLUGDOWN, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodDIS", L"", pIniTestLog->wchNumberOfTestPeriodDIS, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodFSUNHT", L"", pIniTestLog->wchNumberOfTestPeriodFSUNHT, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodFSUYD", L"", pIniTestLog->wchNumberOfTestPeriodFSUYD, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodAVFSU", L"", pIniTestLog->wchNumberOfTestPeriodAVFSU, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriodMI", L"", pIniTestLog->wchNumberOfTestPeriodMI, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfTestPeriod", L"", pIniTestLog->wchNumberOfTestPeriod, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TestPeriodUsed", L"", pIniTestLog->wchTestPeriodUsed, NH_INT_TO_WCHART);

	GetIniString(ini, L"TestLog", L"QuitReason", L"", pIniTestLog->wchQuitReason, 50);
	GetIniString(ini, L"TestLog", L"TestType", L"", pIniTestLog->wchTestType, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"Pass", L"", pIniTestLog->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"Operator", L"", pIniTestLog->wchOperator, 50);
	GetIniString(ini, L"TestLog", L"Driver", L"", pIniTestLog->wchDriver, 50);
	GetIniString(ini, L"TestLog", L"Assessor", L"", pIniTestLog->wchAssessor, 50);
	GetIniString(ini, L"TestLog", L"IsRetest", L"", pIniTestLog->wchIsRetest, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsFuelTypeNeedTest", L"", pIniTestLog->wchIsFuelTypeNeedTest, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsFuelTypeTestFinished", L"", pIniTestLog->wchIsFuelTypeTestFinished, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsFuelTypeTestPass", L"", pIniTestLog->wchIsFuelTypeTestPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsFuelType2NeedTest", L"", pIniTestLog->wchIsFuelType2NeedTest, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsFuelType2TestFinished", L"", pIniTestLog->wchIsFuelType2TestFinished, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsFuelType2TestPass", L"", pIniTestLog->wchIsFuelType2TestPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TestDate", L"", pIniTestLog->wchTestDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TestStartTime", L"", pIniTestLog->wchTestStartTime, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TestEndTime", L"", pIniTestLog->wchTestEndTime, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TestCharacter", L"", pIniTestLog->wchTestCharacter, 50);
	GetIniString(ini, L"TestLog", L"TestCharacterCode", L"", pIniTestLog->wchTestCharacterCode, 50);
	GetIniString(ini, L"TestLog", L"PrintTimes", L"", pIniTestLog->wchPrintTimes, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsSent", L"", pIniTestLog->wchIsSent, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"IsSentEP", L"", pIniTestLog->wchIsSentEP, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"TestLog", L"PlateNumber", L"", pIniTestLog->wchPlateNumber, 50);
	GetIniString(ini, L"TestLog", L"PlateType", L"", pIniTestLog->wchPlateType, 50);
	GetIniString(ini, L"TestLog", L"PlateTypeCode", L"", pIniTestLog->wchPlateTypeCode, 50);
	GetIniString(ini, L"TestLog", L"PlateTypeGA", L"", pIniTestLog->wchPlateTypeGA, 50);
	GetIniString(ini, L"TestLog", L"PlateTypeCodeGA", L"", pIniTestLog->wchPlateTypeCodeGA, 50);
	GetIniString(ini, L"TestLog", L"RegistrationDate", L"", pIniTestLog->wchRegistrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"Owner", L"", pIniTestLog->wchOwner, 50);
	GetIniString(ini, L"TestLog", L"Phone", L"", pIniTestLog->wchPhone, 50);
	GetIniString(ini, L"TestLog", L"Address", L"", pIniTestLog->wchAddress, 50);
	GetIniString(ini, L"TestLog", L"UseCharacter", L"", pIniTestLog->wchUseCharacter, 50);
	GetIniString(ini, L"TestLog", L"Manufacturer", L"", pIniTestLog->wchManufacturer, 50);
	GetIniString(ini, L"TestLog", L"Brand", L"", pIniTestLog->wchBrand, 50);
	GetIniString(ini, L"TestLog", L"Model", L"", pIniTestLog->wchModel, 50);
	GetIniString(ini, L"TestLog", L"ProductionDate", L"", pIniTestLog->wchProductionDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"VIN", L"", pIniTestLog->wchVIN, 50);
	GetIniString(ini, L"TestLog", L"ChassisModel", L"", pIniTestLog->wchChassisModel, 50);
	GetIniString(ini, L"TestLog", L"VehicleType", L"", pIniTestLog->wchVehicleType, 50);
	GetIniString(ini, L"TestLog", L"MaximumTotalMass", L"", pIniTestLog->wchMaximumTotalMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"UnladenMass", L"", pIniTestLog->wchUnladenMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"AxleMass", L"", pIniTestLog->wchAxleMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"RatedLoadingMass", L"", pIniTestLog->wchRatedLoadingMass, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"RatedPassengerCapacity", L"", pIniTestLog->wchRatedPassengerCapacity, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TyrePressure", L"", pIniTestLog->wchTyrePressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TravelledDistance", L"", pIniTestLog->wchTravelledDistance, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"EngineManufacturer", L"", pIniTestLog->wchEngineManufacturer, 50);
	GetIniString(ini, L"TestLog", L"EngineModel", L"", pIniTestLog->wchEngineModel, 50);
	GetIniString(ini, L"TestLog", L"EngineNumber", L"", pIniTestLog->wchEngineNumber, 50);
	GetIniString(ini, L"TestLog", L"EngineStroke", L"", pIniTestLog->wchEngineStroke, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"Displacement", L"", pIniTestLog->wchDisplacement, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NumberOfCylinder", L"", pIniTestLog->wchNumberOfCylinder, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OilSupplyMode", L"", pIniTestLog->wchOilSupplyMode, 50);
	GetIniString(ini, L"TestLog", L"AirIntakeMode", L"", pIniTestLog->wchAirIntakeMode, 50);
	GetIniString(ini, L"TestLog", L"HasCatalyticConverter", L"", pIniTestLog->wchHasCatalyticConverter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FuelType", L"", pIniTestLog->wchFuelType, 50);
	GetIniString(ini, L"TestLog", L"FuelMark", L"", pIniTestLog->wchFuelMark, 50);
	GetIniString(ini, L"TestLog", L"DoubleFuel", L"", pIniTestLog->wchDoubleFuel, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FuelType2", L"", pIniTestLog->wchFuelType2, 50);
	GetIniString(ini, L"TestLog", L"FuelMark2", L"", pIniTestLog->wchFuelMark2, 50);
	GetIniString(ini, L"TestLog", L"RatedRev", L"", pIniTestLog->wchRatedRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"RatedPower", L"", pIniTestLog->wchRatedPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"MaximumNetPower", L"", pIniTestLog->wchMaximumNetPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"GearBoxType", L"", pIniTestLog->wchGearBoxType, 50);
	GetIniString(ini, L"TestLog", L"NumberOfGears", L"", pIniTestLog->wchNumberOfGears, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DriveType", L"", pIniTestLog->wchDriveType, 50);
	GetIniString(ini, L"TestLog", L"EPSign", L"", pIniTestLog->wchEPSign, 50);
	GetIniString(ini, L"TestLog", L"CertificateNumber", L"", pIniTestLog->wchCertificateNumber, 50);
	GetIniString(ini, L"TestLog", L"IndependentDoubleExhaustPipe", L"", pIniTestLog->wchIndependentDoubleExhaustPipe, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TCS", L"", pIniTestLog->wchTCS, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FuelPumpMode", L"", pIniTestLog->wchFuelPumpMode, 50);
	GetIniString(ini, L"TestLog", L"PassengerVehicle", L"", pIniTestLog->wchPassengerVehicle, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"EmissionStandard", L"", pIniTestLog->wchEmissionStandard, 50);
	GetIniString(ini, L"TestLog", L"AnnualTestPeriod", L"", pIniTestLog->wchAnnualTestPeriod, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasOBD", L"", pIniTestLog->wchHasOBD, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OwnerCharacter", L"", pIniTestLog->wchOwnerCharacter, 50);
	GetIniString(ini, L"TestLog", L"OwnerDistrict", L"", pIniTestLog->wchOwnerDistrict, 50);
	GetIniString(ini, L"TestLog", L"LicenseDepartment", L"", pIniTestLog->wchLicenseDepartment, 50);
	GetIniString(ini, L"TestLog", L"LicenseIssueDate", L"", pIniTestLog->wchLicenseIssueDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"LicenseValidDate", L"", pIniTestLog->wchLicenseValidDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"MotorModel", L"", pIniTestLog->wchMotorModel, 50);
	GetIniString(ini, L"TestLog", L"EnergyStorageDeviceModel", L"", pIniTestLog->wchEnergyStorageDeviceModel, 50);
	GetIniString(ini, L"TestLog", L"BatteryCapacity", L"", pIniTestLog->wchBatteryCapacity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasDPF", L"", pIniTestLog->wchHasDPF, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DPFModel", L"", pIniTestLog->wchDPFModel, 50);
	GetIniString(ini, L"TestLog", L"HasSCR", L"", pIniTestLog->wchHasSCR, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"SCRModel", L"", pIniTestLog->wchSCRModel, 50);

	GetIniString(ini, L"TestLog", L"LineNumber", L"", pIniTestLog->wchLineNumber, 50);
	GetIniString(ini, L"TestLog", L"LineModel", L"", pIniTestLog->wchLineModel, 50);
	GetIniString(ini, L"TestLog", L"LineName", L"", pIniTestLog->wchLineName, 50);
	GetIniString(ini, L"TestLog", L"LineManufacturer", L"", pIniTestLog->wchLineManufacturer, 50);
	GetIniString(ini, L"TestLog", L"LineLicenseSn", L"", pIniTestLog->wchLineLicenseSn, 50);
	GetIniString(ini, L"TestLog", L"LineLicenseStartDate", L"", pIniTestLog->wchLineLicenseStartDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"LineLicenseEndDate", L"", pIniTestLog->wchLineLicenseEndDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"LineIsLocked", L"", pIniTestLog->wchLineIsLocked, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"LineRegistrationInfo", L"", pIniTestLog->wchLineRegistrationInfo, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasDynamometer", L"", pIniTestLog->wchHasDynamometer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DynamometerNumber", L"", pIniTestLog->wchDynamometerNumber, 50);
	GetIniString(ini, L"TestLog", L"DynamometerModel", L"", pIniTestLog->wchDynamometerModel, 50);
	GetIniString(ini, L"TestLog", L"DynamometerManufacturer", L"", pIniTestLog->wchDynamometerManufacturer, 50);
	GetIniString(ini, L"TestLog", L"DynamometerCoastDownDate", L"", pIniTestLog->wchDynamometerCoastDownDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DynamometerPLHPDate", L"", pIniTestLog->wchDynamometerPLHPDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DynamometerForceCalibrationDate", L"", pIniTestLog->wchDynamometerForceCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DynamometerForceCheckDate", L"", pIniTestLog->wchDynamometerForceCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"DynamometerAuthenticationDate", L"", pIniTestLog->wchDynamometerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasAnalyzer", L"", pIniTestLog->wchHasAnalyzer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"AnalyzerNumber", L"", pIniTestLog->wchAnalyzerNumber, 50);
	GetIniString(ini, L"TestLog", L"AnalyzerModel", L"", pIniTestLog->wchAnalyzerModel, 50);
	GetIniString(ini, L"TestLog", L"AnalyzerManufacturer", L"", pIniTestLog->wchAnalyzerManufacturer, 50);
	GetIniString(ini, L"TestLog", L"AnalyzerChannelCalibrationDate", L"", pIniTestLog->wchAnalyzerChannelCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"AnalyzerChannelCheckDate", L"", pIniTestLog->wchAnalyzerChannelCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"AnalyzerEnvParamCalibrationDate", L"", pIniTestLog->wchAnalyzerEnvParamCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"AnalyzerAuthenticationDate", L"", pIniTestLog->wchAnalyzerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasFlowmeter", L"", pIniTestLog->wchHasFlowmeter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FlowmeterNumber", L"", pIniTestLog->wchFlowmeterNumber, 50);
	GetIniString(ini, L"TestLog", L"FlowmeterModel", L"", pIniTestLog->wchFlowmeterModel, 50);
	GetIniString(ini, L"TestLog", L"FlowmeterManufacturer", L"", pIniTestLog->wchFlowmeterManufacturer, 50);
	GetIniString(ini, L"TestLog", L"FlowmeterFluxCalibrationDate", L"", pIniTestLog->wchFlowmeterFluxCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FlowmeterFluxCheckDate", L"", pIniTestLog->wchFlowmeterFluxCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FlowmeterDilO2CalibrationDate", L"", pIniTestLog->wchFlowmeterDilO2CalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FlowmeterPressureCalibrationDate", L"", pIniTestLog->wchFlowmeterPressureCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"FlowmeterAuthenticationDate", L"", pIniTestLog->wchFlowmeterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasOpacimeter", L"", pIniTestLog->wchHasOpacimeter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OpacimeterNumber", L"", pIniTestLog->wchOpacimeterNumber, 50);
	GetIniString(ini, L"TestLog", L"OpacimeterModel", L"", pIniTestLog->wchOpacimeterModel, 50);
	GetIniString(ini, L"TestLog", L"OpacimeterManufacturer", L"", pIniTestLog->wchOpacimeterManufacturer, 50);
	GetIniString(ini, L"TestLog", L"OpacimeterChannelCalibrationDate", L"", pIniTestLog->wchOpacimeterChannelCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OpacimeterChannelCheckDate", L"", pIniTestLog->wchOpacimeterChannelCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OpacimeterEnvParamCalibrationDate", L"", pIniTestLog->wchOpacimeterEnvParamCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OpacimeterAuthenticationDate", L"", pIniTestLog->wchOpacimeterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasSmokemeter", L"", pIniTestLog->wchHasSmokemeter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"SmokemeterNumber", L"", pIniTestLog->wchSmokemeterNumber, 50);
	GetIniString(ini, L"TestLog", L"SmokemeterModel", L"", pIniTestLog->wchSmokemeterModel, 50);
	GetIniString(ini, L"TestLog", L"SmokemeterManufacturer", L"", pIniTestLog->wchSmokemeterManufacturer, 50);
	GetIniString(ini, L"TestLog", L"SmokemeterCalibrationDate", L"", pIniTestLog->wchSmokemeterCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"SmokemeterCheckDate", L"", pIniTestLog->wchSmokemeterCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"SmokemeterAuthenticationDate", L"", pIniTestLog->wchSmokemeterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasIndependentEnvParamMeasurer", L"", pIniTestLog->wchHasIndependentEnvParamMeasurer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"EnvParamMeasurerNumber", L"", pIniTestLog->wchEnvParamMeasurerNumber, 50);
	GetIniString(ini, L"TestLog", L"EnvParamMeasurerModel", L"", pIniTestLog->wchEnvParamMeasurerModel, 50);
	GetIniString(ini, L"TestLog", L"EnvParamMeasurerManufacturer", L"", pIniTestLog->wchEnvParamMeasurerManufacturer, 50);
	GetIniString(ini, L"TestLog", L"EnvParamMeasurerCalibrationDate", L"", pIniTestLog->wchEnvParamMeasurerCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"EnvParamMeasurerCheckDate", L"", pIniTestLog->wchEnvParamMeasurerCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"EnvParamMeasurerAuthenticationDate", L"", pIniTestLog->wchEnvParamMeasurerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasIndependentTachometer", L"", pIniTestLog->wchHasIndependentTachometer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TachometerNumber", L"", pIniTestLog->wchTachometerNumber, 50);
	GetIniString(ini, L"TestLog", L"TachometerModel", L"", pIniTestLog->wchTachometerModel, 50);
	GetIniString(ini, L"TestLog", L"TachometerManufacturer", L"", pIniTestLog->wchTachometerManufacturer, 50);
	GetIniString(ini, L"TestLog", L"TachometerCalibrationDate", L"", pIniTestLog->wchTachometerCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TachometerCheckDate", L"", pIniTestLog->wchTachometerCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"TachometerAuthenticationDate", L"", pIniTestLog->wchTachometerAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasOBDAdapter", L"", pIniTestLog->wchHasOBDAdapter, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OBDAdapterNumber", L"", pIniTestLog->wchOBDAdapterNumber, 50);
	GetIniString(ini, L"TestLog", L"OBDAdapterModel", L"", pIniTestLog->wchOBDAdapterModel, 50);
	GetIniString(ini, L"TestLog", L"OBDAdapterManufacturer", L"", pIniTestLog->wchOBDAdapterManufacturer, 50);
	GetIniString(ini, L"TestLog", L"OBDAdapterCalibrationDate", L"", pIniTestLog->wchOBDAdapterCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OBDAdapterCheckDate", L"", pIniTestLog->wchOBDAdapterCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"OBDAdapterAuthenticationDate", L"", pIniTestLog->wchOBDAdapterAuthenticationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"HasNOxAnalyzer", L"", pIniTestLog->wchHasNOxAnalyzer, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerNumber", L"", pIniTestLog->wchNOxAnalyzerNumber, 50);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerModel", L"", pIniTestLog->wchNOxAnalyzerModel, 50);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerManufacturer", L"", pIniTestLog->wchNOxAnalyzerManufacturer, 50);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerChannelCalibrationDate", L"", pIniTestLog->wchNOxAnalyzerChannelCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerChannelCheckDate", L"", pIniTestLog->wchNOxAnalyzerChannelCheckDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerEnvParamCalibrationDate", L"", pIniTestLog->wchNOxAnalyzerEnvParamCalibrationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"NOxAnalyzerAuthenticationDate", L"", pIniTestLog->wchNOxAnalyzerAuthenticationDate, NH_TIME_TO_WCHART);

	GetIniString(ini, L"TestLog", L"StationNumber", L"", pIniTestLog->wchStationNumber, 50);
	GetIniString(ini, L"TestLog", L"StationFullName", L"", pIniTestLog->wchStationFullName, 50);
	GetIniString(ini, L"TestLog", L"StationShortName", L"", pIniTestLog->wchStationShortName, 50);
	GetIniString(ini, L"TestLog", L"StationLicenseSn", L"", pIniTestLog->wchStationLicenseSn, 50);
	GetIniString(ini, L"TestLog", L"StationLicenseStartDate", L"", pIniTestLog->wchStationLicenseStartDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"StationLicenseEndDate", L"", pIniTestLog->wchStationLicenseEndDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"TestLog", L"StationIsLocked", L"", pIniTestLog->wchStationIsLocked, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"TestLog", L"StationRegistrationInfo", L"", pIniTestLog->wchStationRegistrationInfo, NH_INT_TO_WCHART);
	GetIniString(ini, L"TestLog", L"StationPhone", L"", pIniTestLog->wchStationPhone, 50);
	GetIniString(ini, L"TestLog", L"StationFax", L"", pIniTestLog->wchStationFax, 50);
	GetIniString(ini, L"TestLog", L"PostCode", L"", pIniTestLog->wchPostCode, 50);
	GetIniString(ini, L"TestLog", L"Province", L"", pIniTestLog->wchProvince, 50);
	GetIniString(ini, L"TestLog", L"City", L"", pIniTestLog->wchCity, 50);
	GetIniString(ini, L"TestLog", L"District", L"", pIniTestLog->wchDistrict, 50);
	GetIniString(ini, L"TestLog", L"DetailedAddress", L"", pIniTestLog->wchDetailedAddress, 50);
	GetIniString(ini, L"TestLog", L"Remark", L"", pIniTestLog->wchRemark, 1024);
	GetIniString(ini, L"TestLog", L"Reserved1", L"", pIniTestLog->wchReserved1, 50);
	GetIniString(ini, L"TestLog", L"Reserved2", L"", pIniTestLog->wchReserved2, 50);
	GetIniString(ini, L"TestLog", L"Reserved3", L"", pIniTestLog->wchReserved3, 50);

}

void SetIniTestLog(const TESTLOG& iniTestLog)
{
	// 获取TestLog.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 写TestLog.ini配置文件
	WriteIniString(ini, L"TestLog", L"AutoID", iniTestLog.wchAutoID);
	WriteIniString(ini, L"TestLog", L"RunningNumber", iniTestLog.wchRunningNumber);
	WriteIniString(ini, L"TestLog", L"ReportNumber", iniTestLog.wchReportNumber);
	WriteIniString(ini, L"TestLog", L"RunningNumberEP", iniTestLog.wchRunningNumberEP);
	WriteIniString(ini, L"TestLog", L"ReportNumberEP", iniTestLog.wchReportNumberEP);
	WriteIniString(ini, L"TestLog", L"ItemApp", iniTestLog.wchItemApp);
	WriteIniString(ini, L"TestLog", L"ItemOBD", iniTestLog.wchItemOBD);
	WriteIniString(ini, L"TestLog", L"ItemEmi", iniTestLog.wchItemEmi);
	WriteIniString(ini, L"TestLog", L"BillDate", iniTestLog.wchBillDate);
	WriteIniString(ini, L"TestLog", L"Biller", iniTestLog.wchBiller);
	WriteIniString(ini, L"TestLog", L"Shroff", iniTestLog.wchShroff);
	WriteIniString(ini, L"TestLog", L"ChargeState", iniTestLog.wchChargeState);
	WriteIniString(ini, L"TestLog", L"InitialMoney", iniTestLog.wchInitialMoney);
	WriteIniString(ini, L"TestLog", L"ChangedMoney", iniTestLog.wchChangedMoney);
	WriteIniString(ini, L"TestLog", L"ReceivableMoney", iniTestLog.wchReceivableMoney);
	WriteIniString(ini, L"TestLog", L"ActualMoney", iniTestLog.wchActualMoney);
	WriteIniString(ini, L"TestLog", L"TestLocation", iniTestLog.wchTestLocation);
	WriteIniString(ini, L"TestLog", L"TestState", iniTestLog.wchTestState);
	WriteIniString(ini, L"TestLog", L"IsTestFinished", iniTestLog.wchIsTestFinished);

	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodASM", iniTestLog.wchNumberOfTestPeriodASM);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodVMAS", iniTestLog.wchNumberOfTestPeriodVMAS);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodLUGDOWN", iniTestLog.wchNumberOfTestPeriodLUGDOWN);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodDIS", iniTestLog.wchNumberOfTestPeriodDIS);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodFSUNHT", iniTestLog.wchNumberOfTestPeriodFSUNHT);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodFSUYD", iniTestLog.wchNumberOfTestPeriodFSUYD);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodAVFSU", iniTestLog.wchNumberOfTestPeriodAVFSU);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriodMI", iniTestLog.wchNumberOfTestPeriodMI);
	WriteIniString(ini, L"TestLog", L"NumberOfTestPeriod", iniTestLog.wchNumberOfTestPeriod);
	WriteIniString(ini, L"TestLog", L"TestPeriodUsed", iniTestLog.wchTestPeriodUsed);

	WriteIniString(ini, L"TestLog", L"QuitReason", iniTestLog.wchQuitReason);
	WriteIniString(ini, L"TestLog", L"TestType", iniTestLog.wchTestType);
	WriteIniString(ini, L"TestLog", L"Pass", iniTestLog.wchPass);
	WriteIniString(ini, L"TestLog", L"Operator", iniTestLog.wchOperator);
	WriteIniString(ini, L"TestLog", L"Driver", iniTestLog.wchDriver);
	WriteIniString(ini, L"TestLog", L"Assessor", iniTestLog.wchAssessor);
	WriteIniString(ini, L"TestLog", L"IsRetest", iniTestLog.wchIsRetest);
	WriteIniString(ini, L"TestLog", L"IsFuelTypeNeedTest", iniTestLog.wchIsFuelTypeNeedTest);
	WriteIniString(ini, L"TestLog", L"IsFuelTypeTestFinished", iniTestLog.wchIsFuelTypeTestFinished);
	WriteIniString(ini, L"TestLog", L"IsFuelTypeTestPass", iniTestLog.wchIsFuelTypeTestPass);
	WriteIniString(ini, L"TestLog", L"IsFuelType2NeedTest", iniTestLog.wchIsFuelType2NeedTest);
	WriteIniString(ini, L"TestLog", L"IsFuelType2TestFinished", iniTestLog.wchIsFuelType2TestFinished);
	WriteIniString(ini, L"TestLog", L"IsFuelType2TestPass", iniTestLog.wchIsFuelType2TestPass);
	WriteIniString(ini, L"TestLog", L"TestDate", iniTestLog.wchTestDate);
	WriteIniString(ini, L"TestLog", L"TestStartTime", iniTestLog.wchTestStartTime);
	WriteIniString(ini, L"TestLog", L"TestEndTime", iniTestLog.wchTestEndTime);
	WriteIniString(ini, L"TestLog", L"TestCharacter", iniTestLog.wchTestCharacter);
	WriteIniString(ini, L"TestLog", L"TestCharacterCode", iniTestLog.wchTestCharacterCode);
	WriteIniString(ini, L"TestLog", L"PrintTimes", iniTestLog.wchPrintTimes);
	WriteIniString(ini, L"TestLog", L"IsSent", iniTestLog.wchIsSent);
	WriteIniString(ini, L"TestLog", L"IsSentEP", iniTestLog.wchIsSentEP);

	WriteIniString(ini, L"TestLog", L"PlateNumber", iniTestLog.wchPlateNumber);
	WriteIniString(ini, L"TestLog", L"PlateType", iniTestLog.wchPlateType);
	WriteIniString(ini, L"TestLog", L"PlateTypeCode", iniTestLog.wchPlateTypeCode);
	WriteIniString(ini, L"TestLog", L"PlateTypeGA", iniTestLog.wchPlateTypeGA);
	WriteIniString(ini, L"TestLog", L"PlateTypeCodeGA", iniTestLog.wchPlateTypeCodeGA);
	WriteIniString(ini, L"TestLog", L"RegistrationDate", iniTestLog.wchRegistrationDate);
	WriteIniString(ini, L"TestLog", L"Owner", iniTestLog.wchOwner);
	WriteIniString(ini, L"TestLog", L"Phone", iniTestLog.wchPhone);
	WriteIniString(ini, L"TestLog", L"Address", iniTestLog.wchAddress);
	WriteIniString(ini, L"TestLog", L"UseCharacter", iniTestLog.wchUseCharacter);
	WriteIniString(ini, L"TestLog", L"Manufacturer", iniTestLog.wchManufacturer);
	WriteIniString(ini, L"TestLog", L"Brand", iniTestLog.wchBrand);
	WriteIniString(ini, L"TestLog", L"Model", iniTestLog.wchModel);
	WriteIniString(ini, L"TestLog", L"ProductionDate", iniTestLog.wchProductionDate);
	WriteIniString(ini, L"TestLog", L"VIN", iniTestLog.wchVIN);
	WriteIniString(ini, L"TestLog", L"ChassisModel", iniTestLog.wchChassisModel);
	WriteIniString(ini, L"TestLog", L"VehicleType", iniTestLog.wchVehicleType);
	WriteIniString(ini, L"TestLog", L"MaximumTotalMass", iniTestLog.wchMaximumTotalMass);
	WriteIniString(ini, L"TestLog", L"UnladenMass", iniTestLog.wchUnladenMass);
	WriteIniString(ini, L"TestLog", L"AxleMass", iniTestLog.wchAxleMass);
	WriteIniString(ini, L"TestLog", L"RatedLoadingMass", iniTestLog.wchRatedLoadingMass);
	WriteIniString(ini, L"TestLog", L"RatedPassengerCapacity", iniTestLog.wchRatedPassengerCapacity);
	WriteIniString(ini, L"TestLog", L"TyrePressure", iniTestLog.wchTyrePressure);
	WriteIniString(ini, L"TestLog", L"TravelledDistance", iniTestLog.wchTravelledDistance);
	WriteIniString(ini, L"TestLog", L"EngineManufacturer", iniTestLog.wchEngineManufacturer);
	WriteIniString(ini, L"TestLog", L"EngineModel", iniTestLog.wchEngineModel);
	WriteIniString(ini, L"TestLog", L"EngineNumber", iniTestLog.wchEngineNumber);
	WriteIniString(ini, L"TestLog", L"EngineStroke", iniTestLog.wchEngineStroke);
	WriteIniString(ini, L"TestLog", L"Displacement", iniTestLog.wchDisplacement);
	WriteIniString(ini, L"TestLog", L"NumberOfCylinder", iniTestLog.wchNumberOfCylinder);
	WriteIniString(ini, L"TestLog", L"OilSupplyMode", iniTestLog.wchOilSupplyMode);
	WriteIniString(ini, L"TestLog", L"AirIntakeMode", iniTestLog.wchAirIntakeMode);
	WriteIniString(ini, L"TestLog", L"HasCatalyticConverter", iniTestLog.wchHasCatalyticConverter);
	WriteIniString(ini, L"TestLog", L"FuelType", iniTestLog.wchFuelType);
	WriteIniString(ini, L"TestLog", L"FuelMark", iniTestLog.wchFuelMark);
	WriteIniString(ini, L"TestLog", L"DoubleFuel", iniTestLog.wchDoubleFuel);
	WriteIniString(ini, L"TestLog", L"FuelType2", iniTestLog.wchFuelType2);
	WriteIniString(ini, L"TestLog", L"FuelMark2", iniTestLog.wchFuelMark2);
	WriteIniString(ini, L"TestLog", L"RatedRev", iniTestLog.wchRatedRev);
	WriteIniString(ini, L"TestLog", L"RatedPower", iniTestLog.wchRatedPower);
	WriteIniString(ini, L"TestLog", L"MaximumNetPower", iniTestLog.wchMaximumNetPower);
	WriteIniString(ini, L"TestLog", L"GearBoxType", iniTestLog.wchGearBoxType);
	WriteIniString(ini, L"TestLog", L"NumberOfGears", iniTestLog.wchNumberOfGears);
	WriteIniString(ini, L"TestLog", L"DriveType", iniTestLog.wchDriveType);
	WriteIniString(ini, L"TestLog", L"EPSign", iniTestLog.wchEPSign);
	WriteIniString(ini, L"TestLog", L"CertificateNumber", iniTestLog.wchCertificateNumber);
	WriteIniString(ini, L"TestLog", L"IndependentDoubleExhaustPipe", iniTestLog.wchIndependentDoubleExhaustPipe);
	WriteIniString(ini, L"TestLog", L"TCS", iniTestLog.wchTCS);
	WriteIniString(ini, L"TestLog", L"FuelPumpMode", iniTestLog.wchFuelPumpMode);
	WriteIniString(ini, L"TestLog", L"PassengerVehicle", iniTestLog.wchPassengerVehicle);
	WriteIniString(ini, L"TestLog", L"EmissionStandard", iniTestLog.wchEmissionStandard);
	WriteIniString(ini, L"TestLog", L"AnnualTestPeriod", iniTestLog.wchAnnualTestPeriod);
	WriteIniString(ini, L"TestLog", L"HasOBD", iniTestLog.wchHasOBD);
	WriteIniString(ini, L"TestLog", L"OwnerCharacter", iniTestLog.wchOwnerCharacter);
	WriteIniString(ini, L"TestLog", L"OwnerDistrict", iniTestLog.wchOwnerDistrict);
	WriteIniString(ini, L"TestLog", L"LicenseDepartment", iniTestLog.wchLicenseDepartment);
	WriteIniString(ini, L"TestLog", L"LicenseIssueDate", iniTestLog.wchLicenseIssueDate);
	WriteIniString(ini, L"TestLog", L"LicenseValidDate", iniTestLog.wchLicenseValidDate);
	WriteIniString(ini, L"TestLog", L"MotorModel", iniTestLog.wchMotorModel);
	WriteIniString(ini, L"TestLog", L"EnergyStorageDeviceModel", iniTestLog.wchEnergyStorageDeviceModel);
	WriteIniString(ini, L"TestLog", L"BatteryCapacity", iniTestLog.wchBatteryCapacity);
	WriteIniString(ini, L"TestLog", L"HasDPF", iniTestLog.wchHasDPF);
	WriteIniString(ini, L"TestLog", L"DPFModel", iniTestLog.wchDPFModel);
	WriteIniString(ini, L"TestLog", L"HasSCR", iniTestLog.wchHasSCR);
	WriteIniString(ini, L"TestLog", L"SCRModel", iniTestLog.wchSCRModel);

	WriteIniString(ini, L"TestLog", L"LineNumber", iniTestLog.wchLineNumber);
	WriteIniString(ini, L"TestLog", L"LineModel", iniTestLog.wchLineModel);
	WriteIniString(ini, L"TestLog", L"LineName", iniTestLog.wchLineName);
	WriteIniString(ini, L"TestLog", L"LineManufacturer", iniTestLog.wchLineManufacturer);
	WriteIniString(ini, L"TestLog", L"LineLicenseSn", iniTestLog.wchLineLicenseSn);
	WriteIniString(ini, L"TestLog", L"LineLicenseStartDate", iniTestLog.wchLineLicenseStartDate);
	WriteIniString(ini, L"TestLog", L"LineLicenseEndDate", iniTestLog.wchLineLicenseEndDate);
	WriteIniString(ini, L"TestLog", L"LineIsLocked", iniTestLog.wchLineIsLocked);
	WriteIniString(ini, L"TestLog", L"LineRegistrationInfo", iniTestLog.wchLineRegistrationInfo);
	WriteIniString(ini, L"TestLog", L"HasDynamometer", iniTestLog.wchHasDynamometer);
	WriteIniString(ini, L"TestLog", L"DynamometerNumber", iniTestLog.wchDynamometerNumber);
	WriteIniString(ini, L"TestLog", L"DynamometerModel", iniTestLog.wchDynamometerModel);
	WriteIniString(ini, L"TestLog", L"DynamometerManufacturer", iniTestLog.wchDynamometerManufacturer);
	WriteIniString(ini, L"TestLog", L"DynamometerCoastDownDate", iniTestLog.wchDynamometerCoastDownDate);
	WriteIniString(ini, L"TestLog", L"DynamometerPLHPDate", iniTestLog.wchDynamometerPLHPDate);
	WriteIniString(ini, L"TestLog", L"DynamometerForceCalibrationDate", iniTestLog.wchDynamometerForceCalibrationDate);
	WriteIniString(ini, L"TestLog", L"DynamometerForceCheckDate", iniTestLog.wchDynamometerForceCheckDate);
	WriteIniString(ini, L"TestLog", L"DynamometerAuthenticationDate", iniTestLog.wchDynamometerAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasAnalyzer", iniTestLog.wchHasAnalyzer);
	WriteIniString(ini, L"TestLog", L"AnalyzerNumber", iniTestLog.wchAnalyzerNumber);
	WriteIniString(ini, L"TestLog", L"AnalyzerModel", iniTestLog.wchAnalyzerModel);
	WriteIniString(ini, L"TestLog", L"AnalyzerManufacturer", iniTestLog.wchAnalyzerManufacturer);
	WriteIniString(ini, L"TestLog", L"AnalyzerChannelCalibrationDate", iniTestLog.wchAnalyzerChannelCalibrationDate);
	WriteIniString(ini, L"TestLog", L"AnalyzerChannelCheckDate", iniTestLog.wchAnalyzerChannelCheckDate);
	WriteIniString(ini, L"TestLog", L"AnalyzerEnvParamCalibrationDate", iniTestLog.wchAnalyzerEnvParamCalibrationDate);
	WriteIniString(ini, L"TestLog", L"AnalyzerAuthenticationDate", iniTestLog.wchAnalyzerAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasFlowmeter", iniTestLog.wchHasFlowmeter);
	WriteIniString(ini, L"TestLog", L"FlowmeterNumber", iniTestLog.wchFlowmeterNumber);
	WriteIniString(ini, L"TestLog", L"FlowmeterModel", iniTestLog.wchFlowmeterModel);
	WriteIniString(ini, L"TestLog", L"FlowmeterManufacturer", iniTestLog.wchFlowmeterManufacturer);
	WriteIniString(ini, L"TestLog", L"FlowmeterFluxCalibrationDate", iniTestLog.wchFlowmeterFluxCalibrationDate);
	WriteIniString(ini, L"TestLog", L"FlowmeterFluxCheckDate", iniTestLog.wchFlowmeterFluxCheckDate);
	WriteIniString(ini, L"TestLog", L"FlowmeterDilO2CalibrationDate", iniTestLog.wchFlowmeterDilO2CalibrationDate);
	WriteIniString(ini, L"TestLog", L"FlowmeterPressureCalibrationDate", iniTestLog.wchFlowmeterPressureCalibrationDate);
	WriteIniString(ini, L"TestLog", L"FlowmeterAuthenticationDate", iniTestLog.wchFlowmeterAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasOpacimeter", iniTestLog.wchHasOpacimeter);
	WriteIniString(ini, L"TestLog", L"OpacimeterNumber", iniTestLog.wchOpacimeterNumber);
	WriteIniString(ini, L"TestLog", L"OpacimeterModel", iniTestLog.wchOpacimeterModel);
	WriteIniString(ini, L"TestLog", L"OpacimeterManufacturer", iniTestLog.wchOpacimeterManufacturer);
	WriteIniString(ini, L"TestLog", L"OpacimeterChannelCalibrationDate", iniTestLog.wchOpacimeterChannelCalibrationDate);
	WriteIniString(ini, L"TestLog", L"OpacimeterChannelCheckDate", iniTestLog.wchOpacimeterChannelCheckDate);
	WriteIniString(ini, L"TestLog", L"OpacimeterEnvParamCalibrationDate", iniTestLog.wchOpacimeterEnvParamCalibrationDate);
	WriteIniString(ini, L"TestLog", L"OpacimeterAuthenticationDate", iniTestLog.wchOpacimeterAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasSmokemeter", iniTestLog.wchHasSmokemeter);
	WriteIniString(ini, L"TestLog", L"SmokemeterNumber", iniTestLog.wchSmokemeterNumber);
	WriteIniString(ini, L"TestLog", L"SmokemeterModel", iniTestLog.wchSmokemeterModel);
	WriteIniString(ini, L"TestLog", L"SmokemeterManufacturer", iniTestLog.wchSmokemeterManufacturer);
	WriteIniString(ini, L"TestLog", L"SmokemeterCalibrationDate", iniTestLog.wchSmokemeterCalibrationDate);
	WriteIniString(ini, L"TestLog", L"SmokemeterCheckDate", iniTestLog.wchSmokemeterCheckDate);
	WriteIniString(ini, L"TestLog", L"SmokemeterAuthenticationDate", iniTestLog.wchSmokemeterAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasIndependentEnvParamMeasurer", iniTestLog.wchHasIndependentEnvParamMeasurer);
	WriteIniString(ini, L"TestLog", L"EnvParamMeasurerNumber", iniTestLog.wchEnvParamMeasurerNumber);
	WriteIniString(ini, L"TestLog", L"EnvParamMeasurerModel", iniTestLog.wchEnvParamMeasurerModel);
	WriteIniString(ini, L"TestLog", L"EnvParamMeasurerManufacturer", iniTestLog.wchEnvParamMeasurerManufacturer);
	WriteIniString(ini, L"TestLog", L"EnvParamMeasurerCalibrationDate", iniTestLog.wchEnvParamMeasurerCalibrationDate);
	WriteIniString(ini, L"TestLog", L"EnvParamMeasurerCheckDate", iniTestLog.wchEnvParamMeasurerCheckDate);
	WriteIniString(ini, L"TestLog", L"EnvParamMeasurerAuthenticationDate", iniTestLog.wchEnvParamMeasurerAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasIndependentTachometer", iniTestLog.wchHasIndependentTachometer);
	WriteIniString(ini, L"TestLog", L"TachometerNumber", iniTestLog.wchTachometerNumber);
	WriteIniString(ini, L"TestLog", L"TachometerModel", iniTestLog.wchTachometerModel);
	WriteIniString(ini, L"TestLog", L"TachometerManufacturer", iniTestLog.wchTachometerManufacturer);
	WriteIniString(ini, L"TestLog", L"TachometerCalibrationDate", iniTestLog.wchTachometerCalibrationDate);
	WriteIniString(ini, L"TestLog", L"TachometerCheckDate", iniTestLog.wchTachometerCheckDate);
	WriteIniString(ini, L"TestLog", L"TachometerAuthenticationDate", iniTestLog.wchTachometerAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasOBDAdapter", iniTestLog.wchHasOBDAdapter);
	WriteIniString(ini, L"TestLog", L"OBDAdapterNumber", iniTestLog.wchOBDAdapterNumber);
	WriteIniString(ini, L"TestLog", L"OBDAdapterModel", iniTestLog.wchOBDAdapterModel);
	WriteIniString(ini, L"TestLog", L"OBDAdapterManufacturer", iniTestLog.wchOBDAdapterManufacturer);
	WriteIniString(ini, L"TestLog", L"OBDAdapterCalibrationDate", iniTestLog.wchOBDAdapterCalibrationDate);
	WriteIniString(ini, L"TestLog", L"OBDAdapterCheckDate", iniTestLog.wchOBDAdapterCheckDate);
	WriteIniString(ini, L"TestLog", L"OBDAdapterAuthenticationDate", iniTestLog.wchOBDAdapterAuthenticationDate);
	WriteIniString(ini, L"TestLog", L"HasNOxAnalyzer", iniTestLog.wchHasNOxAnalyzer);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerNumber", iniTestLog.wchNOxAnalyzerNumber);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerModel", iniTestLog.wchNOxAnalyzerModel);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerManufacturer", iniTestLog.wchNOxAnalyzerManufacturer);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerChannelCalibrationDate", iniTestLog.wchNOxAnalyzerChannelCalibrationDate);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerChannelCheckDate", iniTestLog.wchNOxAnalyzerChannelCheckDate);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerEnvParamCalibrationDate", iniTestLog.wchNOxAnalyzerEnvParamCalibrationDate);
	WriteIniString(ini, L"TestLog", L"NOxAnalyzerAuthenticationDate", iniTestLog.wchNOxAnalyzerAuthenticationDate);

	WriteIniString(ini, L"TestLog", L"StationNumber", iniTestLog.wchStationNumber);
	WriteIniString(ini, L"TestLog", L"StationFullName", iniTestLog.wchStationFullName);
	WriteIniString(ini, L"TestLog", L"StationShortName", iniTestLog.wchStationShortName);
	WriteIniString(ini, L"TestLog", L"StationLicenseSn", iniTestLog.wchStationLicenseSn);
	WriteIniString(ini, L"TestLog", L"StationLicenseStartDate", iniTestLog.wchStationLicenseStartDate);
	WriteIniString(ini, L"TestLog", L"StationLicenseEndDate", iniTestLog.wchStationLicenseEndDate);
	WriteIniString(ini, L"TestLog", L"StationIsLocked", iniTestLog.wchStationIsLocked);
	WriteIniString(ini, L"TestLog", L"StationRegistrationInfo", iniTestLog.wchStationRegistrationInfo);
	WriteIniString(ini, L"TestLog", L"StationPhone", iniTestLog.wchStationPhone);
	WriteIniString(ini, L"TestLog", L"StationFax", iniTestLog.wchStationFax);
	WriteIniString(ini, L"TestLog", L"PostCode", iniTestLog.wchPostCode);
	WriteIniString(ini, L"TestLog", L"Province", iniTestLog.wchProvince);
	WriteIniString(ini, L"TestLog", L"City", iniTestLog.wchCity);
	WriteIniString(ini, L"TestLog", L"District", iniTestLog.wchDistrict);
	WriteIniString(ini, L"TestLog", L"DetailedAddress", iniTestLog.wchDetailedAddress);
	WriteIniString(ini, L"TestLog", L"Remark", iniTestLog.wchRemark);
	WriteIniString(ini, L"TestLog", L"Reserved1", iniTestLog.wchReserved1);
	WriteIniString(ini, L"TestLog", L"Reserved2", iniTestLog.wchReserved2);
	WriteIniString(ini, L"TestLog", L"Reserved3", iniTestLog.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniResultOfDIS(PRESULTOFDIS const pIniResultOfDIS)
{
	// 获取ResultOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfDIS.ini信息
	GetIniString(ini, L"ResultOfDIS", L"AutoID", L"", pIniResultOfDIS->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"ReportNumber", L"", pIniResultOfDIS->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfDIS", L"FuelTypeCode", L"", pIniResultOfDIS->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"FuelTypeName", L"", pIniResultOfDIS->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfHCHigh", L"", pIniResultOfDIS->wchLimitOfHCHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfHCHigh_EDP", L"", pIniResultOfDIS->wchLimitOfHCHigh_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfCOHigh", L"", pIniResultOfDIS->wchLimitOfCOHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfCOHigh_EDP", L"", pIniResultOfDIS->wchLimitOfCOHigh_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfHCLow", L"", pIniResultOfDIS->wchLimitOfHCLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfHCLow_EDP", L"", pIniResultOfDIS->wchLimitOfHCLow_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfCOLow", L"", pIniResultOfDIS->wchLimitOfCOLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfCOLow_EDP", L"", pIniResultOfDIS->wchLimitOfCOLow_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"UpperLimitOfLambda", L"", pIniResultOfDIS->wchUpperLimitOfLambda, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LowerLimitOfLambda", L"", pIniResultOfDIS->wchLowerLimitOfLambda, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LimitOfLambda_EDP", L"", pIniResultOfDIS->wchLimitOfLambda_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"RevHigh", L"", pIniResultOfDIS->wchRevHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"OilTemperatureHigh", L"", pIniResultOfDIS->wchOilTemperatureHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LambdaHigh", L"", pIniResultOfDIS->wchLambdaHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"NOHigh", L"", pIniResultOfDIS->wchNOHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"CO2High", L"", pIniResultOfDIS->wchCO2High, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"O2High", L"", pIniResultOfDIS->wchO2High, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"HCHigh", L"", pIniResultOfDIS->wchHCHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"COHigh", L"", pIniResultOfDIS->wchCOHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"PassOfHCHigh", L"", pIniResultOfDIS->wchPassOfHCHigh, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"PassOfCOHigh", L"", pIniResultOfDIS->wchPassOfCOHigh, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"NeedTestLambda", L"", pIniResultOfDIS->wchNeedTestLambda, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"Lambda", L"", pIniResultOfDIS->wchLambda, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"PassOfLambda", L"", pIniResultOfDIS->wchPassOfLambda, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"NeedTestLIS", L"", pIniResultOfDIS->wchNeedTestLIS, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"RevLow", L"", pIniResultOfDIS->wchRevLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"OilTemperatureLow", L"", pIniResultOfDIS->wchOilTemperatureLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"LambdaLow", L"", pIniResultOfDIS->wchLambdaLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"NOLow", L"", pIniResultOfDIS->wchNOLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"CO2Low", L"", pIniResultOfDIS->wchCO2Low, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"O2Low", L"", pIniResultOfDIS->wchO2Low, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"HCLow", L"", pIniResultOfDIS->wchHCLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"COLow", L"", pIniResultOfDIS->wchCOLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"PassOfHCLow", L"", pIniResultOfDIS->wchPassOfHCLow, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"PassOfCOLow", L"", pIniResultOfDIS->wchPassOfCOLow, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"Pass", L"", pIniResultOfDIS->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"EnvironmentalTemperature", L"", pIniResultOfDIS->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"RelativeHumidity", L"", pIniResultOfDIS->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"AtmosphericPressure", L"", pIniResultOfDIS->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"SkipRev", L"", pIniResultOfDIS->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"SkipOilTemperature", L"", pIniResultOfDIS->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDIS", L"Remark", L"", pIniResultOfDIS->wchRemark, 1024);
	GetIniString(ini, L"ResultOfDIS", L"Reserved1", L"", pIniResultOfDIS->wchReserved1, 50);
	GetIniString(ini, L"ResultOfDIS", L"Reserved2", L"", pIniResultOfDIS->wchReserved2, 50);
	GetIniString(ini, L"ResultOfDIS", L"Reserved3", L"", pIniResultOfDIS->wchReserved3, 50);

}

void GetIniResultOfDISEx(SResultOfDIS* const pResultOfDIS)
{
	// 获取ResultOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfDIS.ini信息
	pResultOfDIS->strAutoID = ini.GetValue(L"ResultOfDIS", L"AutoID", L"");
	pResultOfDIS->strReportNumber = ini.GetValue(L"ResultOfDIS", L"ReportNumber", L"");
	pResultOfDIS->strFuelTypeCode = ini.GetValue(L"ResultOfDIS", L"FuelTypeCode", L"");
	pResultOfDIS->strFuelTypeName = ini.GetValue(L"ResultOfDIS", L"FuelTypeName", L"");
	pResultOfDIS->strLimitOfHCHigh = ini.GetValue(L"ResultOfDIS", L"LimitOfHCHigh", L"");
	pResultOfDIS->strLimitOfHCHigh_EDP = ini.GetValue(L"ResultOfDIS", L"LimitOfHCHigh_EDP", L"");
	pResultOfDIS->strLimitOfCOHigh = ini.GetValue(L"ResultOfDIS", L"LimitOfCOHigh", L"");
	pResultOfDIS->strLimitOfCOHigh_EDP = ini.GetValue(L"ResultOfDIS", L"LimitOfCOHigh_EDP", L"");
	pResultOfDIS->strLimitOfHCLow = ini.GetValue(L"ResultOfDIS", L"LimitOfHCLow", L"");
	pResultOfDIS->strLimitOfHCLow_EDP = ini.GetValue(L"ResultOfDIS", L"LimitOfHCLow_EDP", L"");
	pResultOfDIS->strLimitOfCOLow = ini.GetValue(L"ResultOfDIS", L"LimitOfCOLow", L"");
	pResultOfDIS->strLimitOfCOLow_EDP = ini.GetValue(L"ResultOfDIS", L"LimitOfCOLow_EDP", L"");
	pResultOfDIS->strUpperLimitOfLambda = ini.GetValue(L"ResultOfDIS", L"UpperLimitOfLambda", L"");
	pResultOfDIS->strLowerLimitOfLambda = ini.GetValue(L"ResultOfDIS", L"LowerLimitOfLambda", L"");
	pResultOfDIS->strLimitOfLambda_EDP = ini.GetValue(L"ResultOfDIS", L"LimitOfLambda_EDP", L"");
	pResultOfDIS->strRevHigh = ini.GetValue(L"ResultOfDIS", L"RevHigh", L"");
	pResultOfDIS->strOilTemperatureHigh = ini.GetValue(L"ResultOfDIS", L"OilTemperatureHigh", L"");
	pResultOfDIS->strLambdaHigh = ini.GetValue(L"ResultOfDIS", L"LambdaHigh", L"");
	pResultOfDIS->strNOHigh = ini.GetValue(L"ResultOfDIS", L"NOHigh", L"");
	pResultOfDIS->strCO2High = ini.GetValue(L"ResultOfDIS", L"CO2High", L"");
	pResultOfDIS->strO2High = ini.GetValue(L"ResultOfDIS", L"O2High", L"");
	pResultOfDIS->strHCHigh = ini.GetValue(L"ResultOfDIS", L"HCHigh", L"");
	pResultOfDIS->strCOHigh = ini.GetValue(L"ResultOfDIS", L"COHigh", L"");
	pResultOfDIS->strPassOfHCHigh = ini.GetValue(L"ResultOfDIS", L"PassOfHCHigh", L"");
	pResultOfDIS->strPassOfCOHigh = ini.GetValue(L"ResultOfDIS", L"PassOfCOHigh", L"");
	pResultOfDIS->strNeedTestLambda = ini.GetValue(L"ResultOfDIS", L"NeedTestLambda", L"");
	pResultOfDIS->strLambda = ini.GetValue(L"ResultOfDIS", L"Lambda", L"");
	pResultOfDIS->strPassOfLambda = ini.GetValue(L"ResultOfDIS", L"PassOfLambda", L"");
	pResultOfDIS->strNeedTestLIS = ini.GetValue(L"ResultOfDIS", L"NeedTestLIS", L"");
	pResultOfDIS->strRevLow = ini.GetValue(L"ResultOfDIS", L"RevLow", L"");
	pResultOfDIS->strOilTemperatureLow = ini.GetValue(L"ResultOfDIS", L"OilTemperatureLow", L"");
	pResultOfDIS->strLambdaLow = ini.GetValue(L"ResultOfDIS", L"LambdaLow", L"");
	pResultOfDIS->strNOLow = ini.GetValue(L"ResultOfDIS", L"NOLow", L"");
	pResultOfDIS->strCO2Low = ini.GetValue(L"ResultOfDIS", L"CO2Low", L"");
	pResultOfDIS->strO2Low = ini.GetValue(L"ResultOfDIS", L"O2Low", L"");
	pResultOfDIS->strHCLow = ini.GetValue(L"ResultOfDIS", L"HCLow", L"");
	pResultOfDIS->strCOLow = ini.GetValue(L"ResultOfDIS", L"COLow", L"");
	pResultOfDIS->strPassOfHCLow = ini.GetValue(L"ResultOfDIS", L"PassOfHCLow", L"");
	pResultOfDIS->strPassOfCOLow = ini.GetValue(L"ResultOfDIS", L"PassOfCOLow", L"");
	pResultOfDIS->strPass = ini.GetValue(L"ResultOfDIS", L"Pass", L"");
	pResultOfDIS->strEnvironmentalTemperature = ini.GetValue(L"ResultOfDIS", L"EnvironmentalTemperature", L"");
	pResultOfDIS->strRelativeHumidity = ini.GetValue(L"ResultOfDIS", L"RelativeHumidity", L"");
	pResultOfDIS->strAtmosphericPressure = ini.GetValue(L"ResultOfDIS", L"AtmosphericPressure", L"");
	pResultOfDIS->strSkipRev = ini.GetValue(L"ResultOfDIS", L"SkipRev", L"");
	pResultOfDIS->strSkipOilTemperature = ini.GetValue(L"ResultOfDIS", L"SkipOilTemperature", L"");
	pResultOfDIS->strRemark = ini.GetValue(L"ResultOfDIS", L"Remark", L"");
	pResultOfDIS->strReserved1 = ini.GetValue(L"ResultOfDIS", L"Reserved1", L"");
	pResultOfDIS->strReserved2 = ini.GetValue(L"ResultOfDIS", L"Reserved2", L"");
	pResultOfDIS->strReserved3 = ini.GetValue(L"ResultOfDIS", L"Reserved3", L"");
}

void SetIniResultOfDIS(const RESULTOFDIS& iniResultOfDIS)
{
	// 获取ResultOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDIS.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfDIS.ini信息
	WriteIniString(ini, L"ResultOfDIS", L"AutoID", iniResultOfDIS.wchAutoID);
	WriteIniString(ini, L"ResultOfDIS", L"ReportNumber", iniResultOfDIS.wchReportNumber);
	WriteIniString(ini, L"ResultOfDIS", L"FuelTypeCode", iniResultOfDIS.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfDIS", L"FuelTypeName", iniResultOfDIS.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfHCHigh", iniResultOfDIS.wchLimitOfHCHigh);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfHCHigh_EDP", iniResultOfDIS.wchLimitOfHCHigh_EDP);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfCOHigh", iniResultOfDIS.wchLimitOfCOHigh);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfCOHigh_EDP", iniResultOfDIS.wchLimitOfCOHigh_EDP);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfHCLow", iniResultOfDIS.wchLimitOfHCLow);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfHCLow_EDP", iniResultOfDIS.wchLimitOfHCLow_EDP);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfCOLow", iniResultOfDIS.wchLimitOfCOLow);
	WriteIniString(ini, L"ResultOfDIS", L"LimitOfCOLow_EDP", iniResultOfDIS.wchLimitOfCOLow_EDP);
	WriteIniString(ini, L"ResultOfDIS", L"RevHigh", iniResultOfDIS.wchRevHigh);
	WriteIniString(ini, L"ResultOfDIS", L"OilTemperatureHigh", iniResultOfDIS.wchOilTemperatureHigh);
	WriteIniString(ini, L"ResultOfDIS", L"LambdaHigh", iniResultOfDIS.wchLambdaHigh);
	WriteIniString(ini, L"ResultOfDIS", L"NOHigh", iniResultOfDIS.wchNOHigh);
	WriteIniString(ini, L"ResultOfDIS", L"CO2High", iniResultOfDIS.wchCO2High);
	WriteIniString(ini, L"ResultOfDIS", L"O2High", iniResultOfDIS.wchO2High);
	WriteIniString(ini, L"ResultOfDIS", L"HCHigh", iniResultOfDIS.wchHCHigh);
	WriteIniString(ini, L"ResultOfDIS", L"COHigh", iniResultOfDIS.wchCOHigh);
	WriteIniString(ini, L"ResultOfDIS", L"PassOfHCHigh", iniResultOfDIS.wchPassOfHCHigh);
	WriteIniString(ini, L"ResultOfDIS", L"PassOfCOHigh", iniResultOfDIS.wchPassOfCOHigh);
	WriteIniString(ini, L"ResultOfDIS", L"NeedTestLambda", iniResultOfDIS.wchNeedTestLambda);
	WriteIniString(ini, L"ResultOfDIS", L"Lambda", iniResultOfDIS.wchLambda);
	WriteIniString(ini, L"ResultOfDIS", L"PassOfLambda", iniResultOfDIS.wchPassOfLambda);
	WriteIniString(ini, L"ResultOfDIS", L"NeedTestLIS", iniResultOfDIS.wchNeedTestLIS);
	WriteIniString(ini, L"ResultOfDIS", L"RevLow", iniResultOfDIS.wchRevLow);
	WriteIniString(ini, L"ResultOfDIS", L"OilTemperatureLow", iniResultOfDIS.wchOilTemperatureLow);
	WriteIniString(ini, L"ResultOfDIS", L"LambdaLow", iniResultOfDIS.wchLambdaLow);
	WriteIniString(ini, L"ResultOfDIS", L"NOLow", iniResultOfDIS.wchNOLow);
	WriteIniString(ini, L"ResultOfDIS", L"CO2Low", iniResultOfDIS.wchCO2Low);
	WriteIniString(ini, L"ResultOfDIS", L"O2Low", iniResultOfDIS.wchO2Low);
	WriteIniString(ini, L"ResultOfDIS", L"HCLow", iniResultOfDIS.wchHCLow);
	WriteIniString(ini, L"ResultOfDIS", L"COLow", iniResultOfDIS.wchCOLow);
	WriteIniString(ini, L"ResultOfDIS", L"PassOfHCLow", iniResultOfDIS.wchPassOfHCLow);
	WriteIniString(ini, L"ResultOfDIS", L"PassOfCOLow", iniResultOfDIS.wchPassOfCOLow);
	WriteIniString(ini, L"ResultOfDIS", L"Pass", iniResultOfDIS.wchPass);
	WriteIniString(ini, L"ResultOfDIS", L"EnvironmentalTemperature", iniResultOfDIS.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfDIS", L"RelativeHumidity", iniResultOfDIS.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfDIS", L"AtmosphericPressure", iniResultOfDIS.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfDIS", L"SkipRev", iniResultOfDIS.wchSkipRev);
	WriteIniString(ini, L"ResultOfDIS", L"SkipOilTemperature", iniResultOfDIS.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfDIS", L"Remark", iniResultOfDIS.wchRemark);
	WriteIniString(ini, L"ResultOfDIS", L"Reserved1", iniResultOfDIS.wchReserved1);
	WriteIniString(ini, L"ResultOfDIS", L"Reserved2", iniResultOfDIS.wchReserved2);
	WriteIniString(ini, L"ResultOfDIS", L"Reserved3", iniResultOfDIS.wchReserved3);
	
	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfDIS(list<REALTIMEDATAOFDIS>& listRealTimeDataOfDIS)
{
	// 获取RealTimeDataOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	REALTIMEDATAOFDIS iniRealTimeDataOfDIS;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfDIS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfDIS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfDIS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfDIS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfDIS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfDIS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfDIS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfDIS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfDIS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfDIS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfDIS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfDIS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfDIS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfDIS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfDIS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfDIS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfDIS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfDIS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfDIS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfDIS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfDIS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfDIS.wchReserved3, 50);

		listRealTimeDataOfDIS.push_back(iniRealTimeDataOfDIS);
	}

}

void GetIniRealTimeDataOfDISVector(vector<REALTIMEDATAOFDIS>& vRealTimeDataOfDIS)
{
	// 获取RealTimeDataOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	REALTIMEDATAOFDIS iniRealTimeDataOfDIS;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfDIS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfDIS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfDIS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfDIS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfDIS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfDIS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfDIS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfDIS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfDIS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfDIS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfDIS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfDIS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfDIS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfDIS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfDIS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfDIS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfDIS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfDIS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfDIS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfDIS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfDIS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfDIS.wchReserved3, 50);

		vRealTimeDataOfDIS.push_back(iniRealTimeDataOfDIS);
	}
}

void GetIniRealTimeDataOfDISEx(std::vector<SRealTimeDataOfDIS> &vtRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfDIS.ini"*/)
{
	// 获取RealTimeDataOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}
	
	for(int i=1; i<=nTotal; i++)
	{
		SRealTimeDataOfDIS sRTData;
		// 获取RealTimeDataOfDIS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		sRTData.strReportNumber = ini.GetValue(wchSection, L"ReportNumber", L"");
		sRTData.strFuelTypeCode = ini.GetValue(wchSection, L"FuelTypeCode", L"");
		sRTData.strFuelTypeName = ini.GetValue(wchSection, L"FuelTypeName", L"");
		sRTData.strTime = ini.GetValue(wchSection, L"Time", L"");
		sRTData.strSamplingTime = ini.GetValue(wchSection, L"SamplingTime", L"");
		sRTData.strState = ini.GetValue(wchSection, L"State", L"");
		sRTData.strHC = ini.GetValue(wchSection, L"HC", L"");
		sRTData.strCO = ini.GetValue(wchSection, L"CO", L"");
		sRTData.strO2 = ini.GetValue(wchSection, L"O2", L"");
		sRTData.strCO2 = ini.GetValue(wchSection, L"CO2", L"");
		sRTData.strNO = ini.GetValue(wchSection, L"NO", L"");
		sRTData.strEngineRev = ini.GetValue(wchSection, L"EngineRev", L"");
		sRTData.strOilTemperature = ini.GetValue(wchSection, L"OilTemperature", L"");
		sRTData.strLambda = ini.GetValue(wchSection, L"Lambda", L"");
		sRTData.strEnvironmentalTemperature = ini.GetValue(wchSection, L"EnvironmentalTemperature", L"");		
		sRTData.strRelativeHumidity = ini.GetValue(wchSection, L"RelativeHumidity", L"");
		sRTData.strAtmosphericPressure = ini.GetValue(wchSection, L"AtmosphericPressure", L"");	

		// OBD数据流
		sRTData.sOBDRTData.strVelocity = ini.GetValue(wchSection, L"OBDRTData_Velocity", L"");
		sRTData.sOBDRTData.strCoolantTemp = ini.GetValue(wchSection, L"OBDRTData_CoolantTemp", L"");
		sRTData.sOBDRTData.strEngineRev = ini.GetValue(wchSection, L"OBDRTData_EngineRev", L"");
		sRTData.sOBDRTData.strSolarTermDoorPosition = ini.GetValue(wchSection, L"OBDRTData_SolarTermDoorPosition", L"");
		sRTData.sOBDRTData.strThrottleOpening = ini.GetValue(wchSection, L"OBDRTData_ThrottleOpening", L"");
		sRTData.sOBDRTData.strCalculationLoad = ini.GetValue(wchSection, L"OBDRTData_CalculationLoad", L"");
		sRTData.sOBDRTData.strForwardSensorV = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorV", L"");
		sRTData.sOBDRTData.strForwardSensorI = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorI", L"");
		sRTData.sOBDRTData.strAirCoefficient = ini.GetValue(wchSection, L"OBDRTData_AirCoefficient", L"");
		sRTData.sOBDRTData.strMAF = ini.GetValue(wchSection, L"OBDRTData_MAF", L"");
		sRTData.sOBDRTData.strMAP = ini.GetValue(wchSection, L"OBDRTData_MAP", L"");
		sRTData.sOBDRTData.strEngineOutputPower = ini.GetValue(wchSection, L"OBDRTData_EngineOutputPower", L"");
		sRTData.sOBDRTData.strChargeAirPressure = ini.GetValue(wchSection, L"OBDRTData_ChargeAirPressure", L"");
		sRTData.sOBDRTData.strFuelConsumption = ini.GetValue(wchSection, L"OBDRTData_FuelConsumption", L"");
		sRTData.sOBDRTData.strNOXConcentration = ini.GetValue(wchSection, L"OBDRTData_NOXConcentration", L"");
		sRTData.sOBDRTData.strUreaInjectionVolume = ini.GetValue(wchSection, L"OBDRTData_UreaInjectionVolume", L"");
		sRTData.sOBDRTData.strEGT = ini.GetValue(wchSection, L"OBDRTData_EGT", L"");
		sRTData.sOBDRTData.strDPFDifferentialPressure = ini.GetValue(wchSection, L"OBDRTData_DPFDifferentialPressure", L"");
		sRTData.sOBDRTData.strEGRPosition = ini.GetValue(wchSection, L"OBDRTData_EGRPosition", L"");
		sRTData.sOBDRTData.strFuelDeliveryPressure = ini.GetValue(wchSection, L"OBDRTData_FuelDeliveryPressure", L"");

		sRTData.strRemark = ini.GetValue(wchSection, L"Remark", L"");
		sRTData.strReserved1 = ini.GetValue(wchSection, L"Reserved1", L"");
		sRTData.strReserved2 = ini.GetValue(wchSection, L"Reserved2", L"");
		sRTData.strReserved3 = ini.GetValue(wchSection, L"Reserved3", L"");
		vtRTData.push_back(sRTData);
	}
}

void GetIniResultOfASM(PRESULTOFASM const pIniResultOfASM)
{
	// 获取ResultOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfASM.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfASM.ini信息
	GetIniString(ini, L"ResultOfASM", L"AutoID", L"", pIniResultOfASM->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"ReportNumber", L"", pIniResultOfASM->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfASM", L"FuelTypeCode", L"", pIniResultOfASM->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"FuelTypeName", L"", pIniResultOfASM->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfASM", L"LimitOfHC5025", L"", pIniResultOfASM->wchLimitOfHC5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfHC5025_EDP", L"", pIniResultOfASM->wchLimitOfHC5025_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfCO5025", L"", pIniResultOfASM->wchLimitOfCO5025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfCO5025_EDP", L"", pIniResultOfASM->wchLimitOfCO5025_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfNO5025", L"", pIniResultOfASM->wchLimitOfNO5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfNO5025_EDP", L"", pIniResultOfASM->wchLimitOfNO5025_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfHC2540", L"", pIniResultOfASM->wchLimitOfHC2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfHC2540_EDP", L"", pIniResultOfASM->wchLimitOfHC2540_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfCO2540", L"", pIniResultOfASM->wchLimitOfCO2540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfCO2540_EDP", L"", pIniResultOfASM->wchLimitOfCO2540_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfNO2540", L"", pIniResultOfASM->wchLimitOfNO2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"LimitOfNO2540_EDP", L"", pIniResultOfASM->wchLimitOfNO2540_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Rev5025", L"", pIniResultOfASM->wchRev5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"OilTemperature5025", L"", pIniResultOfASM->wchOilTemperature5025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Lambda5025", L"", pIniResultOfASM->wchLambda5025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Power5025", L"", pIniResultOfASM->wchPower5025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"HC5025", L"", pIniResultOfASM->wchHC5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"CO5025", L"", pIniResultOfASM->wchCO5025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"NO5025", L"", pIniResultOfASM->wchNO5025, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"CO25025", L"", pIniResultOfASM->wchCO25025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"O25025", L"", pIniResultOfASM->wchO25025, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"PassOfHC5025", L"", pIniResultOfASM->wchPassOfHC5025, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"PassOfCO5025", L"", pIniResultOfASM->wchPassOfCO5025, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"PassOfNO5025", L"", pIniResultOfASM->wchPassOfNO5025, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"NeedTestMode2540", L"", pIniResultOfASM->wchNeedTestMode2540, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Rev2540", L"", pIniResultOfASM->wchRev2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"OilTemperature2540", L"", pIniResultOfASM->wchOilTemperature2540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Lambda2540", L"", pIniResultOfASM->wchLambda2540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Power2540", L"", pIniResultOfASM->wchPower2540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"HC2540", L"", pIniResultOfASM->wchHC2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"CO2540", L"", pIniResultOfASM->wchCO2540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"NO2540", L"", pIniResultOfASM->wchNO2540, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"CO22540", L"", pIniResultOfASM->wchCO22540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"O22540", L"", pIniResultOfASM->wchO22540, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"PassOfHC2540", L"", pIniResultOfASM->wchPassOfHC2540, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"PassOfCO2540", L"", pIniResultOfASM->wchPassOfCO2540, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"PassOfNO2540", L"", pIniResultOfASM->wchPassOfNO2540, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Pass", L"", pIniResultOfASM->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"EnvironmentalTemperature", L"", pIniResultOfASM->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"RelativeHumidity", L"", pIniResultOfASM->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"AtmosphericPressure", L"", pIniResultOfASM->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"SkipRev", L"", pIniResultOfASM->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"SkipOilTemperature", L"", pIniResultOfASM->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfASM", L"Remark", L"", pIniResultOfASM->wchRemark, 1024);
	GetIniString(ini, L"ResultOfASM", L"Reserved1", L"", pIniResultOfASM->wchReserved1, 50);
	GetIniString(ini, L"ResultOfASM", L"Reserved2", L"", pIniResultOfASM->wchReserved2, 50);
	GetIniString(ini, L"ResultOfASM", L"Reserved3", L"", pIniResultOfASM->wchReserved3, 50);

}

void GetIniResultOfASMEx(SResultOfASM* const pResultOfASM)
{
	// 获取ResultOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfASM.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfASM.ini信息
	pResultOfASM->strAutoID = ini.GetValue(L"ResultOfASM", L"AutoID", L"");
	pResultOfASM->strReportNumber = ini.GetValue(L"ResultOfASM", L"ReportNumber", L"");
	pResultOfASM->strFuelTypeCode = ini.GetValue(L"ResultOfASM", L"FuelTypeCode", L"");
	pResultOfASM->strFuelTypeName = ini.GetValue(L"ResultOfASM", L"FuelTypeName", L"");
	pResultOfASM->strLimitOfHC5025 = ini.GetValue(L"ResultOfASM", L"LimitOfHC5025", L"");
	pResultOfASM->strLimitOfHC5025_EDP = ini.GetValue(L"ResultOfASM", L"LimitOfHC5025_EDP", L"");
	pResultOfASM->strLimitOfCO5025 = ini.GetValue(L"ResultOfASM", L"LimitOfCO5025", L"");
	pResultOfASM->strLimitOfCO5025_EDP = ini.GetValue(L"ResultOfASM", L"LimitOfCO5025_EDP", L"");
	pResultOfASM->strLimitOfNO5025 = ini.GetValue(L"ResultOfASM", L"LimitOfNO5025", L"");
	pResultOfASM->strLimitOfNO5025_EDP = ini.GetValue(L"ResultOfASM", L"LimitOfNO5025_EDP", L"");
	pResultOfASM->strLimitOfHC2540 = ini.GetValue(L"ResultOfASM", L"LimitOfHC2540", L"");
	pResultOfASM->strLimitOfHC2540_EDP = ini.GetValue(L"ResultOfASM", L"LimitOfHC2540_EDP", L"");
	pResultOfASM->strLimitOfCO2540 = ini.GetValue(L"ResultOfASM", L"LimitOfCO2540", L"");
	pResultOfASM->strLimitOfCO2540_EDP = ini.GetValue(L"ResultOfASM", L"LimitOfCO2540_EDP", L"");
	pResultOfASM->strLimitOfNO2540 = ini.GetValue(L"ResultOfASM", L"LimitOfNO2540", L"");
	pResultOfASM->strLimitOfNO2540_EDP = ini.GetValue(L"ResultOfASM", L"LimitOfNO2540_EDP", L"");
	pResultOfASM->strRev5025 = ini.GetValue(L"ResultOfASM", L"Rev5025", L"");
	pResultOfASM->strOilTemperature5025 = ini.GetValue(L"ResultOfASM", L"OilTemperature5025", L"");
	pResultOfASM->strLambda5025 = ini.GetValue(L"ResultOfASM", L"Lambda5025", L"");
	pResultOfASM->strPower5025 = ini.GetValue(L"ResultOfASM", L"Power5025", L"");
	pResultOfASM->strHC5025 = ini.GetValue(L"ResultOfASM", L"HC5025", L"");
	pResultOfASM->strCO5025 = ini.GetValue(L"ResultOfASM", L"CO5025", L"");
	pResultOfASM->strNO5025 = ini.GetValue(L"ResultOfASM", L"NO5025", L"");
	pResultOfASM->strCO25025 = ini.GetValue(L"ResultOfASM", L"CO25025", L"");
	pResultOfASM->strO25025 = ini.GetValue(L"ResultOfASM", L"O25025", L"");
	pResultOfASM->strPassOfHC5025 = ini.GetValue(L"ResultOfASM", L"PassOfHC5025", L"");
	pResultOfASM->strPassOfCO5025 = ini.GetValue(L"ResultOfASM", L"PassOfCO5025", L"");
	pResultOfASM->strPassOfNO5025 = ini.GetValue(L"ResultOfASM", L"PassOfNO5025", L"");
	pResultOfASM->strNeedTestMode2540 = ini.GetValue(L"ResultOfASM", L"NeedTestMode2540", L"");
	pResultOfASM->strRev2540 = ini.GetValue(L"ResultOfASM", L"Rev2540", L"");
	pResultOfASM->strOilTemperature2540 = ini.GetValue(L"ResultOfASM", L"OilTemperature2540", L"");
	pResultOfASM->strLambda2540 = ini.GetValue(L"ResultOfASM", L"Lambda2540", L"");
	pResultOfASM->strPower2540 = ini.GetValue(L"ResultOfASM", L"Power2540", L"");
	pResultOfASM->strHC2540 = ini.GetValue(L"ResultOfASM", L"HC2540", L"");
	pResultOfASM->strCO2540 = ini.GetValue(L"ResultOfASM", L"CO2540", L"");
	pResultOfASM->strNO2540 = ini.GetValue(L"ResultOfASM", L"NO2540", L"");
	pResultOfASM->strCO22540 = ini.GetValue(L"ResultOfASM", L"CO22540", L"");
	pResultOfASM->strO22540 = ini.GetValue(L"ResultOfASM", L"O22540", L"");
	pResultOfASM->strPassOfHC2540 = ini.GetValue(L"ResultOfASM", L"PassOfHC2540", L"");
	pResultOfASM->strPassOfCO2540 = ini.GetValue(L"ResultOfASM", L"PassOfCO2540", L"");
	pResultOfASM->strPassOfNO2540 = ini.GetValue(L"ResultOfASM", L"PassOfNO2540", L"");
	pResultOfASM->strPass = ini.GetValue(L"ResultOfASM", L"Pass", L"");
	pResultOfASM->strEnvironmentalTemperature = ini.GetValue(L"ResultOfASM", L"EnvironmentalTemperature", L"");
	pResultOfASM->strRelativeHumidity = ini.GetValue(L"ResultOfASM", L"RelativeHumidity", L"");
	pResultOfASM->strAtmosphericPressure = ini.GetValue(L"ResultOfASM", L"AtmosphericPressure", L"");
	pResultOfASM->strSkipRev = ini.GetValue(L"ResultOfASM", L"SkipRev", L"");
	pResultOfASM->strSkipOilTemperature = ini.GetValue(L"ResultOfASM", L"SkipOilTemperature", L"");
	pResultOfASM->strRemark = ini.GetValue(L"ResultOfASM", L"Remark", L"");
	pResultOfASM->strReserved1 = ini.GetValue(L"ResultOfASM", L"Reserved1", L"");
	pResultOfASM->strReserved2 = ini.GetValue(L"ResultOfASM", L"Reserved2", L"");
	pResultOfASM->strReserved3 = ini.GetValue(L"ResultOfASM", L"Reserved3", L"");
}

void SetIniResultOfASM(const RESULTOFASM& iniResultOfASM)
{
	// 获取ResultOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfASM.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfASM.ini信息
	WriteIniString(ini, L"ResultOfASM", L"AutoID", iniResultOfASM.wchAutoID);
	WriteIniString(ini, L"ResultOfASM", L"ReportNumber", iniResultOfASM.wchReportNumber);
	WriteIniString(ini, L"ResultOfASM", L"FuelTypeCode", iniResultOfASM.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfASM", L"FuelTypeName", iniResultOfASM.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfHC5025", iniResultOfASM.wchLimitOfHC5025);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfHC5025_EDP", iniResultOfASM.wchLimitOfHC5025_EDP);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfCO5025", iniResultOfASM.wchLimitOfCO5025);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfCO5025_EDP", iniResultOfASM.wchLimitOfCO5025_EDP);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfNO5025", iniResultOfASM.wchLimitOfNO5025);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfNO5025_EDP", iniResultOfASM.wchLimitOfNO5025_EDP);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfHC2540", iniResultOfASM.wchLimitOfHC2540);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfHC2540_EDP", iniResultOfASM.wchLimitOfHC2540_EDP);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfCO2540", iniResultOfASM.wchLimitOfCO2540);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfCO2540_EDP", iniResultOfASM.wchLimitOfCO2540_EDP);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfNO2540", iniResultOfASM.wchLimitOfNO2540);
	WriteIniString(ini, L"ResultOfASM", L"LimitOfNO2540_EDP", iniResultOfASM.wchLimitOfNO2540_EDP);
	WriteIniString(ini, L"ResultOfASM", L"Rev5025", iniResultOfASM.wchRev5025);
	WriteIniString(ini, L"ResultOfASM", L"OilTemperature5025", iniResultOfASM.wchOilTemperature5025);
	WriteIniString(ini, L"ResultOfASM", L"Lambda5025", iniResultOfASM.wchLambda5025);
	WriteIniString(ini, L"ResultOfASM", L"Power5025", iniResultOfASM.wchPower5025);
	WriteIniString(ini, L"ResultOfASM", L"HC5025", iniResultOfASM.wchHC5025);
	WriteIniString(ini, L"ResultOfASM", L"CO5025", iniResultOfASM.wchCO5025);
	WriteIniString(ini, L"ResultOfASM", L"NO5025", iniResultOfASM.wchNO5025);
	WriteIniString(ini, L"ResultOfASM", L"CO25025", iniResultOfASM.wchCO25025);
	WriteIniString(ini, L"ResultOfASM", L"O25025", iniResultOfASM.wchO25025);
	WriteIniString(ini, L"ResultOfASM", L"PassOfHC5025", iniResultOfASM.wchPassOfHC5025);
	WriteIniString(ini, L"ResultOfASM", L"PassOfCO5025", iniResultOfASM.wchPassOfCO5025);
	WriteIniString(ini, L"ResultOfASM", L"PassOfNO5025", iniResultOfASM.wchPassOfNO5025);
	WriteIniString(ini, L"ResultOfASM", L"NeedTestMode2540", iniResultOfASM.wchNeedTestMode2540);
	WriteIniString(ini, L"ResultOfASM", L"Rev2540", iniResultOfASM.wchRev2540);
	WriteIniString(ini, L"ResultOfASM", L"OilTemperature2540", iniResultOfASM.wchOilTemperature2540);
	WriteIniString(ini, L"ResultOfASM", L"Lambda2540", iniResultOfASM.wchLambda2540);
	WriteIniString(ini, L"ResultOfASM", L"Power2540", iniResultOfASM.wchPower2540);
	WriteIniString(ini, L"ResultOfASM", L"HC2540", iniResultOfASM.wchHC2540);
	WriteIniString(ini, L"ResultOfASM", L"CO2540", iniResultOfASM.wchCO2540);
	WriteIniString(ini, L"ResultOfASM", L"NO2540", iniResultOfASM.wchNO2540);
	WriteIniString(ini, L"ResultOfASM", L"CO22540", iniResultOfASM.wchCO22540);
	WriteIniString(ini, L"ResultOfASM", L"O22540", iniResultOfASM.wchO22540);
	WriteIniString(ini, L"ResultOfASM", L"PassOfHC2540", iniResultOfASM.wchPassOfHC2540);
	WriteIniString(ini, L"ResultOfASM", L"PassOfCO2540", iniResultOfASM.wchPassOfCO2540);
	WriteIniString(ini, L"ResultOfASM", L"PassOfNO2540", iniResultOfASM.wchPassOfNO2540);
	WriteIniString(ini, L"ResultOfASM", L"Pass", iniResultOfASM.wchPass);
	WriteIniString(ini, L"ResultOfASM", L"EnvironmentalTemperature", iniResultOfASM.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfASM", L"RelativeHumidity", iniResultOfASM.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfASM", L"AtmosphericPressure", iniResultOfASM.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfASM", L"SkipRev", iniResultOfASM.wchSkipRev);
	WriteIniString(ini, L"ResultOfASM", L"SkipOilTemperature", iniResultOfASM.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfASM", L"Remark", iniResultOfASM.wchRemark);
	WriteIniString(ini, L"ResultOfASM", L"Reserved1", iniResultOfASM.wchReserved1);
	WriteIniString(ini, L"ResultOfASM", L"Reserved2", iniResultOfASM.wchReserved2);
	WriteIniString(ini, L"ResultOfASM", L"Reserved3", iniResultOfASM.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfASM(list<REALTIMEDATAOFASM>& listRealTimeDataOfASM)
{
	// 获取RealTimeDataOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfASM.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfASM iniRealTimeDataOfASM;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfASM.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfASM.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfASM.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfASM.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfASM.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfASM.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfASM.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfASM.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfASM.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfASM.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfASM.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfASM.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"State", L"", iniRealTimeDataOfASM.wchState, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfASM.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfASM.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfASM.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfASM.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfASM.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfASM.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfASM.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfASM.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfASM.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfASM.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfASM.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfASM.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DilutionCorrectionFactor", L"", iniRealTimeDataOfASM.wchDilutionCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IsAdoptedResult", L"", iniRealTimeDataOfASM.wchIsAdoptedResult, NH_BOOL_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfASM.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfASM.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfASM.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfASM.wchReserved3, 50);

		listRealTimeDataOfASM.push_back(iniRealTimeDataOfASM);
	}
}

void GetIniRealTimeDataOfASMVector(vector<REALTIMEDATAOFASM>& vRealTimeDataOfASM)
{
	// 获取RealTimeDataOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfASM.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfASM iniRealTimeDataOfASM;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfASM.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfASM.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfASM.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfASM.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfASM.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfASM.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfASM.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfASM.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfASM.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfASM.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfASM.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfASM.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"State", L"", iniRealTimeDataOfASM.wchState, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfASM.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfASM.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfASM.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfASM.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfASM.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfASM.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfASM.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfASM.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfASM.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfASM.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfASM.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfASM.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DilutionCorrectionFactor", L"", iniRealTimeDataOfASM.wchDilutionCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfASM.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfASM.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfASM.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfASM.wchReserved3, 50);

		vRealTimeDataOfASM.push_back(iniRealTimeDataOfASM);
	}
}

void GetIniRealTimeDataOfASMEx(std::vector<SRealTimeDataOfASM> &vtRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfASM.ini"*/)
{
	// 获取RealTimeDataOfASM.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}


	for(int i=1; i<=nTotal; i++)
	{
		SRealTimeDataOfASM sRTData;
		// 获取RealTimeDataOfASM.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		sRTData.strReportNumber = ini.GetValue(wchSection, L"ReportNumber", L"");
		sRTData.strFuelTypeCode = ini.GetValue(wchSection, L"FuelTypeCode", L"");
		sRTData.strFuelTypeName = ini.GetValue(wchSection, L"FuelTypeName", L"");
		sRTData.strTime = ini.GetValue(wchSection, L"Time", L"");
		sRTData.strSamplingTime = ini.GetValue(wchSection, L"SamplingTime", L"");
		sRTData.strVelocity = ini.GetValue(wchSection, L"Velocity", L"");
		sRTData.strPower = ini.GetValue(wchSection, L"Power", L"");
		sRTData.strForce = ini.GetValue(wchSection, L"Force", L"");		
		sRTData.strIHP = ini.GetValue(wchSection, L"IHP", L"");
		sRTData.strPLHP = ini.GetValue(wchSection, L"PLHP", L"");
		sRTData.strState = ini.GetValue(wchSection, L"State", L"");
		sRTData.strHC = ini.GetValue(wchSection, L"HC", L"");
		sRTData.strCO = ini.GetValue(wchSection, L"CO", L"");
		sRTData.strNO = ini.GetValue(wchSection, L"NO", L"");
		sRTData.strO2 = ini.GetValue(wchSection, L"O2", L"");
		sRTData.strCO2 = ini.GetValue(wchSection, L"CO2", L"");
		sRTData.strEngineRev = ini.GetValue(wchSection, L"EngineRev", L"");
		sRTData.strOilTemperature = ini.GetValue(wchSection, L"OilTemperature", L"");
		sRTData.strLambda = ini.GetValue(wchSection, L"Lambda", L"");
		sRTData.strEnvironmentalTemperature = ini.GetValue(wchSection, L"EnvironmentalTemperature", L"");		
		sRTData.strRelativeHumidity = ini.GetValue(wchSection, L"RelativeHumidity", L"");
		sRTData.strAtmosphericPressure = ini.GetValue(wchSection, L"AtmosphericPressure", L"");
		sRTData.strHumidityCorrectionFactor = ini.GetValue(wchSection, L"HumidityCorrectionFactor", L"");
		sRTData.strDilutionCorrectionFactor = ini.GetValue(wchSection, L"DilutionCorrectionFactor", L"");

		// OBD数据流
		sRTData.sOBDRTData.strVelocity = ini.GetValue(wchSection, L"OBDRTData_Velocity", L"");
		sRTData.sOBDRTData.strCoolantTemp = ini.GetValue(wchSection, L"OBDRTData_CoolantTemp", L"");
		sRTData.sOBDRTData.strEngineRev = ini.GetValue(wchSection, L"OBDRTData_EngineRev", L"");
		sRTData.sOBDRTData.strSolarTermDoorPosition = ini.GetValue(wchSection, L"OBDRTData_SolarTermDoorPosition", L"");
		sRTData.sOBDRTData.strThrottleOpening = ini.GetValue(wchSection, L"OBDRTData_ThrottleOpening", L"");
		sRTData.sOBDRTData.strCalculationLoad = ini.GetValue(wchSection, L"OBDRTData_CalculationLoad", L"");
		sRTData.sOBDRTData.strForwardSensorV = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorV", L"");
		sRTData.sOBDRTData.strForwardSensorI = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorI", L"");
		sRTData.sOBDRTData.strAirCoefficient = ini.GetValue(wchSection, L"OBDRTData_AirCoefficient", L"");
		sRTData.sOBDRTData.strMAF = ini.GetValue(wchSection, L"OBDRTData_MAF", L"");
		sRTData.sOBDRTData.strMAP = ini.GetValue(wchSection, L"OBDRTData_MAP", L"");
		sRTData.sOBDRTData.strEngineOutputPower = ini.GetValue(wchSection, L"OBDRTData_EngineOutputPower", L"");
		sRTData.sOBDRTData.strChargeAirPressure = ini.GetValue(wchSection, L"OBDRTData_ChargeAirPressure", L"");
		sRTData.sOBDRTData.strFuelConsumption = ini.GetValue(wchSection, L"OBDRTData_FuelConsumption", L"");
		sRTData.sOBDRTData.strNOXConcentration = ini.GetValue(wchSection, L"OBDRTData_NOXConcentration", L"");
		sRTData.sOBDRTData.strUreaInjectionVolume = ini.GetValue(wchSection, L"OBDRTData_UreaInjectionVolume", L"");
		sRTData.sOBDRTData.strEGT = ini.GetValue(wchSection, L"OBDRTData_EGT", L"");
		sRTData.sOBDRTData.strDPFDifferentialPressure = ini.GetValue(wchSection, L"OBDRTData_DPFDifferentialPressure", L"");
		sRTData.sOBDRTData.strEGRPosition = ini.GetValue(wchSection, L"OBDRTData_EGRPosition", L"");
		sRTData.sOBDRTData.strFuelDeliveryPressure = ini.GetValue(wchSection, L"OBDRTData_FuelDeliveryPressure", L"");

		sRTData.strRemark = ini.GetValue(wchSection, L"Remark", L"");
		sRTData.strReserved1 = ini.GetValue(wchSection, L"Reserved1", L"");
		sRTData.strReserved2 = ini.GetValue(wchSection, L"Reserved2", L"");
		sRTData.strReserved3 = ini.GetValue(wchSection, L"Reserved3", L"");
		vtRTData.push_back(sRTData);
	}
}

void GetIniResultOfVMAS(PRESULTOFVMAS const pIniResultOfVMAS)
{
	// 获取ResultOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfVMAS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfVMAS.ini信息
	GetIniString(ini, L"ResultOfVMAS", L"AutoID", L"", pIniResultOfVMAS->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"ReportNumber", L"", pIniResultOfVMAS->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfVMAS", L"FuelTypeCode", L"", pIniResultOfVMAS->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"FuelTypeName", L"", pIniResultOfVMAS->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfVMAS", L"NeedTestHCNOx", L"", pIniResultOfVMAS->wchNeedTestHCNOx, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"O2", L"", pIniResultOfVMAS->wchO2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"CO2", L"", pIniResultOfVMAS->wchCO2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"HC", L"", pIniResultOfVMAS->wchHC, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"CO", L"", pIniResultOfVMAS->wchCO, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"NOx", L"", pIniResultOfVMAS->wchNOx, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"HCNOx", L"", pIniResultOfVMAS->wchHCNOx, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfHC", L"", pIniResultOfVMAS->wchLimitOfHC, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfHC_EDP", L"", pIniResultOfVMAS->wchLimitOfHC_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfCO", L"", pIniResultOfVMAS->wchLimitOfCO, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfCO_EDP", L"", pIniResultOfVMAS->wchLimitOfCO_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfNOx", L"", pIniResultOfVMAS->wchLimitOfNOx, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfNOx_EDP", L"", pIniResultOfVMAS->wchLimitOfNOx_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfHCNOx", L"", pIniResultOfVMAS->wchLimitOfHCNOx, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"LimitOfHCNOx_EDP", L"", pIniResultOfVMAS->wchLimitOfHCNOx_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"PassOfHC", L"", pIniResultOfVMAS->wchPassOfHC, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"PassOfCO", L"", pIniResultOfVMAS->wchPassOfCO, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"PassOfNOx", L"", pIniResultOfVMAS->wchPassOfNOx, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"PassOfHCNOx", L"", pIniResultOfVMAS->wchPassOfHCNOx, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"Pass", L"", pIniResultOfVMAS->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"Power50", L"", pIniResultOfVMAS->wchPower50, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"TravelledDistance", L"", pIniResultOfVMAS->wchTravelledDistance, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"OilTemperature", L"", pIniResultOfVMAS->wchOilTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"VolOfGas", L"", pIniResultOfVMAS->wchVolOfGas, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"VolOfExhaust", L"", pIniResultOfVMAS->wchVolOfExhaust, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"EnvironmentalTemperature", L"", pIniResultOfVMAS->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"RelativeHumidity", L"", pIniResultOfVMAS->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"AtmosphericPressure", L"", pIniResultOfVMAS->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"SkipRev", L"", pIniResultOfVMAS->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"SkipOilTemperature", L"", pIniResultOfVMAS->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfVMAS", L"Remark", L"", pIniResultOfVMAS->wchRemark, 1024);
	GetIniString(ini, L"ResultOfVMAS", L"Reserved1", L"", pIniResultOfVMAS->wchReserved1, 50);
	GetIniString(ini, L"ResultOfVMAS", L"Reserved2", L"", pIniResultOfVMAS->wchReserved2, 50);
	GetIniString(ini, L"ResultOfVMAS", L"Reserved3", L"", pIniResultOfVMAS->wchReserved3, 50);

}

void GetIniResultOfVMASEx(SResultOfVMAS* const pResultOfVMAS)
{
	// 获取ResultOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfVMAS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfVMAS.ini信息
	pResultOfVMAS->strAutoID = ini.GetValue(L"ResultOfVMAS", L"AutoID", L"");
	pResultOfVMAS->strReportNumber = ini.GetValue(L"ResultOfVMAS", L"ReportNumber", L"");
	pResultOfVMAS->strFuelTypeCode = ini.GetValue(L"ResultOfVMAS", L"FuelTypeCode", L"");
	pResultOfVMAS->strFuelTypeName = ini.GetValue(L"ResultOfVMAS", L"FuelTypeName", L"");
	pResultOfVMAS->strNeedTestHCNOx = ini.GetValue(L"ResultOfVMAS", L"NeedTestHCNOx", L"");
	pResultOfVMAS->strO2 = ini.GetValue(L"ResultOfVMAS", L"O2", L"");
	pResultOfVMAS->strCO2 = ini.GetValue(L"ResultOfVMAS", L"CO2", L"");
	pResultOfVMAS->strHC = ini.GetValue(L"ResultOfVMAS", L"HC", L"");
	pResultOfVMAS->strCO = ini.GetValue(L"ResultOfVMAS", L"CO", L"");
	pResultOfVMAS->strNOx = ini.GetValue(L"ResultOfVMAS", L"NOx", L"");
	pResultOfVMAS->strHCNOx = ini.GetValue(L"ResultOfVMAS", L"HCNOx", L"");
	pResultOfVMAS->strLimitOfHC = ini.GetValue(L"ResultOfVMAS", L"LimitOfHC", L"");
	pResultOfVMAS->strLimitOfHC_EDP = ini.GetValue(L"ResultOfVMAS", L"LimitOfHC_EDP", L"");
	pResultOfVMAS->strLimitOfCO = ini.GetValue(L"ResultOfVMAS", L"LimitOfCO", L"");
	pResultOfVMAS->strLimitOfCO_EDP = ini.GetValue(L"ResultOfVMAS", L"LimitOfCO_EDP", L"");
	pResultOfVMAS->strLimitOfNOx = ini.GetValue(L"ResultOfVMAS", L"LimitOfNOx", L"");
	pResultOfVMAS->strLimitOfNOx_EDP = ini.GetValue(L"ResultOfVMAS", L"LimitOfNOx_EDP", L"");
	pResultOfVMAS->strLimitOfHCNOx = ini.GetValue(L"ResultOfVMAS", L"LimitOfHCNOx", L"");
	pResultOfVMAS->strLimitOfHCNOx_EDP = ini.GetValue(L"ResultOfVMAS", L"LimitOfHCNOx_EDP", L"");
	pResultOfVMAS->strPassOfHC = ini.GetValue(L"ResultOfVMAS", L"PassOfHC", L"");
	pResultOfVMAS->strPassOfCO = ini.GetValue(L"ResultOfVMAS", L"PassOfCO", L"");
	pResultOfVMAS->strPassOfNOx = ini.GetValue(L"ResultOfVMAS", L"PassOfNOx", L"");
	pResultOfVMAS->strPassOfHCNOx = ini.GetValue(L"ResultOfVMAS", L"PassOfHCNOx", L"");
	pResultOfVMAS->strPass = ini.GetValue(L"ResultOfVMAS", L"Pass", L"");
	pResultOfVMAS->strPower50 = ini.GetValue(L"ResultOfVMAS", L"Power50", L"");
	pResultOfVMAS->strTravelledDistance = ini.GetValue(L"ResultOfVMAS", L"TravelledDistance", L"");
	pResultOfVMAS->strFuleConsumptionPer100KM = ini.GetValue(L"ResultOfVMAS", L"FuleConsumptionPer100KM", L"");
	pResultOfVMAS->strDeviateTime = ini.GetValue(L"ResultOfVMAS", L"DeviateTime", L"");
	pResultOfVMAS->strDeviateCount = ini.GetValue(L"ResultOfVMAS", L"DeviateCount", L"");
	pResultOfVMAS->strLambda = ini.GetValue(L"ResultOfVMAS", L"Lambda", L"");
	pResultOfVMAS->strOilTemperature = ini.GetValue(L"ResultOfVMAS", L"OilTemperature", L"");
	pResultOfVMAS->strVolOfGas = ini.GetValue(L"ResultOfVMAS", L"VolOfGas", L"");
	pResultOfVMAS->strVolOfExhaust = ini.GetValue(L"ResultOfVMAS", L"VolOfExhaust", L"");
	pResultOfVMAS->strEnvironmentalTemperature = ini.GetValue(L"ResultOfVMAS", L"EnvironmentalTemperature", L"");
	pResultOfVMAS->strRelativeHumidity = ini.GetValue(L"ResultOfVMAS", L"RelativeHumidity", L"");
	pResultOfVMAS->strAtmosphericPressure = ini.GetValue(L"ResultOfVMAS", L"AtmosphericPressure", L"");
	pResultOfVMAS->strSkipRev = ini.GetValue(L"ResultOfVMAS", L"SkipRev", L"");
	pResultOfVMAS->strSkipOilTemperature = ini.GetValue(L"ResultOfVMAS", L"SkipOilTemperature", L"");
	pResultOfVMAS->strRemark = ini.GetValue(L"ResultOfVMAS", L"Remark", L"");
	pResultOfVMAS->strReserved1 = ini.GetValue(L"ResultOfVMAS", L"Reserved1", L"");
	pResultOfVMAS->strReserved2 = ini.GetValue(L"ResultOfVMAS", L"Reserved2", L"");
	pResultOfVMAS->strReserved3 = ini.GetValue(L"ResultOfVMAS", L"Reserved3", L"");
}

void SetIniResultOfVMAS(const RESULTOFVMAS& iniResultOfVMAS)
{
	// 获取ResultOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfVMAS.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfVMAS.ini信息
	WriteIniString(ini, L"ResultOfVMAS", L"AutoID", iniResultOfVMAS.wchAutoID);
	WriteIniString(ini, L"ResultOfVMAS", L"ReportNumber", iniResultOfVMAS.wchReportNumber);
	WriteIniString(ini, L"ResultOfVMAS", L"FuelTypeCode", iniResultOfVMAS.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfVMAS", L"FuelTypeName", iniResultOfVMAS.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfVMAS", L"NeedTestHCNOx", iniResultOfVMAS.wchNeedTestHCNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"O2", iniResultOfVMAS.wchO2);
	WriteIniString(ini, L"ResultOfVMAS", L"CO2", iniResultOfVMAS.wchCO2);
	WriteIniString(ini, L"ResultOfVMAS", L"HC", iniResultOfVMAS.wchHC);
	WriteIniString(ini, L"ResultOfVMAS", L"CO", iniResultOfVMAS.wchCO);
	WriteIniString(ini, L"ResultOfVMAS", L"NOx", iniResultOfVMAS.wchNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"HCNOx", iniResultOfVMAS.wchHCNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfHC", iniResultOfVMAS.wchLimitOfHC);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfHC_EDP", iniResultOfVMAS.wchLimitOfHC_EDP);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfCO", iniResultOfVMAS.wchLimitOfCO);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfCO_EDP", iniResultOfVMAS.wchLimitOfCO_EDP);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfNOx", iniResultOfVMAS.wchLimitOfNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfNOx_EDP", iniResultOfVMAS.wchLimitOfNOx_EDP);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfHCNOx", iniResultOfVMAS.wchLimitOfHCNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"LimitOfHCNOx_EDP", iniResultOfVMAS.wchLimitOfHCNOx_EDP);
	WriteIniString(ini, L"ResultOfVMAS", L"PassOfHC", iniResultOfVMAS.wchPassOfHC);
	WriteIniString(ini, L"ResultOfVMAS", L"PassOfCO", iniResultOfVMAS.wchPassOfCO);
	WriteIniString(ini, L"ResultOfVMAS", L"PassOfNOx", iniResultOfVMAS.wchPassOfNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"PassOfHCNOx", iniResultOfVMAS.wchPassOfHCNOx);
	WriteIniString(ini, L"ResultOfVMAS", L"Pass", iniResultOfVMAS.wchPass);
	WriteIniString(ini, L"ResultOfVMAS", L"Power50", iniResultOfVMAS.wchPower50);
	WriteIniString(ini, L"ResultOfVMAS", L"TravelledDistance", iniResultOfVMAS.wchTravelledDistance);
	WriteIniString(ini, L"ResultOfVMAS", L"OilTemperature", iniResultOfVMAS.wchOilTemperature);
	WriteIniString(ini, L"ResultOfVMAS", L"VolOfGas", iniResultOfVMAS.wchVolOfGas);
	WriteIniString(ini, L"ResultOfVMAS", L"VolOfExhaust", iniResultOfVMAS.wchVolOfExhaust);
	WriteIniString(ini, L"ResultOfVMAS", L"EnvironmentalTemperature", iniResultOfVMAS.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfVMAS", L"RelativeHumidity", iniResultOfVMAS.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfVMAS", L"AtmosphericPressure", iniResultOfVMAS.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfVMAS", L"SkipRev", iniResultOfVMAS.wchSkipRev);
	WriteIniString(ini, L"ResultOfVMAS", L"SkipOilTemperature", iniResultOfVMAS.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfVMAS", L"Remark", iniResultOfVMAS.wchRemark);
	WriteIniString(ini, L"ResultOfVMAS", L"Reserved1", iniResultOfVMAS.wchReserved1);
	WriteIniString(ini, L"ResultOfVMAS", L"Reserved2", iniResultOfVMAS.wchReserved2);
	WriteIniString(ini, L"ResultOfVMAS", L"Reserved3", iniResultOfVMAS.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfVMAS(list<REALTIMEDATAOFVMAS>& listRealTimeDataOfVMAS)
{
	// 获取RealTimeDataOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfVMAS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfVMAS iniRealTimeDataOfVMAS;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfVMAS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfVMAS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfVMAS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfVMAS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfVMAS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfVMAS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfVMAS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfVMAS.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfVMAS.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfVMAS.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfVMAS.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfVMAS.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfVMAS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfVMAS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfVMAS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfVMAS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfVMAS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfVMAS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfVMAS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfVMAS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2OfFlowmeter", L"", iniRealTimeDataOfVMAS.wchO2OfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2OfEnvironment", L"", iniRealTimeDataOfVMAS.wchO2OfEnvironment, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HCm", L"", iniRealTimeDataOfVMAS.wchHCm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"COm", L"", iniRealTimeDataOfVMAS.wchCOm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NOm", L"", iniRealTimeDataOfVMAS.wchNOm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2m", L"", iniRealTimeDataOfVMAS.wchO2m, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2m", L"", iniRealTimeDataOfVMAS.wchCO2m, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PressureOfFlowmeter", L"", iniRealTimeDataOfVMAS.wchPressureOfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"TemperatureOfFlowmeter", L"", iniRealTimeDataOfVMAS.wchTemperatureOfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DR", L"", iniRealTimeDataOfVMAS.wchDR, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"ActualFluxOfGas", L"", iniRealTimeDataOfVMAS.wchActualFluxOfGas, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"StandardFluxOfGas", L"", iniRealTimeDataOfVMAS.wchStandardFluxOfGas, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"FluxOfExhaust", L"", iniRealTimeDataOfVMAS.wchFluxOfExhaust, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfVMAS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfVMAS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfVMAS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfVMAS.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DilutionCorrectionFactor", L"", iniRealTimeDataOfVMAS.wchDilutionCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"InertiaSimulationError", L"", iniRealTimeDataOfVMAS.wchInertiaSimulationError, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfVMAS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfVMAS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfVMAS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfVMAS.wchReserved3, 50);

		listRealTimeDataOfVMAS.push_back(iniRealTimeDataOfVMAS);
	}
}

void GetIniRealTimeDataOfVMASVector(vector<REALTIMEDATAOFVMAS>& vRealTimeDataOfVMAS)
{
	// 获取RealTimeDataOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfVMAS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfVMAS iniRealTimeDataOfVMAS;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfVMAS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfVMAS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfVMAS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfVMAS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfVMAS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfVMAS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfVMAS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfVMAS.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfVMAS.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfVMAS.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfVMAS.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfVMAS.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfVMAS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfVMAS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfVMAS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfVMAS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfVMAS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfVMAS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfVMAS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfVMAS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2OfFlowmeter", L"", iniRealTimeDataOfVMAS.wchO2OfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2OfEnvironment", L"", iniRealTimeDataOfVMAS.wchO2OfEnvironment, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HCm", L"", iniRealTimeDataOfVMAS.wchHCm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"COm", L"", iniRealTimeDataOfVMAS.wchCOm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NOm", L"", iniRealTimeDataOfVMAS.wchNOm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2m", L"", iniRealTimeDataOfVMAS.wchO2m, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2m", L"", iniRealTimeDataOfVMAS.wchCO2m, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PressureOfFlowmeter", L"", iniRealTimeDataOfVMAS.wchPressureOfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"TemperatureOfFlowmeter", L"", iniRealTimeDataOfVMAS.wchTemperatureOfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DR", L"", iniRealTimeDataOfVMAS.wchDR, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"ActualFluxOfGas", L"", iniRealTimeDataOfVMAS.wchActualFluxOfGas, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"StandardFluxOfGas", L"", iniRealTimeDataOfVMAS.wchStandardFluxOfGas, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"FluxOfExhaust", L"", iniRealTimeDataOfVMAS.wchFluxOfExhaust, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfVMAS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfVMAS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfVMAS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfVMAS.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DilutionCorrectionFactor", L"", iniRealTimeDataOfVMAS.wchDilutionCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"InertiaSimulationError", L"", iniRealTimeDataOfVMAS.wchInertiaSimulationError, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfVMAS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfVMAS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfVMAS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfVMAS.wchReserved3, 50);

		vRealTimeDataOfVMAS.push_back(iniRealTimeDataOfVMAS);
	}
}

void GetIniRealTimeDataOfVMASEx(std::vector<SRealTimeDataOfVMAS> &vtRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfVMAS.ini"*/)
{
	// 获取RealTimeDataOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	for(int i=1; i<=nTotal; i++)
	{
		SRealTimeDataOfVMAS sRTData;
		// 获取RealTimeDataOfVMAS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		sRTData.strRunningNumber = ini.GetValue(wchSection, L"RunningNumber", L"");
		sRTData.strReportNumber = ini.GetValue(wchSection, L"ReportNumber", L"");
		sRTData.strFuelTypeCode = ini.GetValue(wchSection, L"FuelTypeCode", L"");
		sRTData.strFuelTypeName = ini.GetValue(wchSection, L"FuelTypeName", L"");
		sRTData.strTime = ini.GetValue(wchSection, L"Time", L"");
		sRTData.strSamplingTime = ini.GetValue(wchSection, L"SamplingTime", L"");
		sRTData.strVelocity = ini.GetValue(wchSection, L"Velocity", L"");
		sRTData.strPower = ini.GetValue(wchSection, L"Power", L"");
		sRTData.strForce = ini.GetValue(wchSection, L"Force", L"");
		sRTData.strIHP = ini.GetValue(wchSection, L"IHP", L"");
		sRTData.strPLHP = ini.GetValue(wchSection, L"PLHP", L"");
		sRTData.strHC = ini.GetValue(wchSection, L"HC", L"");
		sRTData.strCO = ini.GetValue(wchSection, L"CO", L"");
		sRTData.strNO = ini.GetValue(wchSection, L"NO", L"");
		sRTData.strNO2 = ini.GetValue(wchSection, L"NO2", L"");
		sRTData.strNOx = ini.GetValue(wchSection, L"NOx", L"");
		sRTData.strO2 = ini.GetValue(wchSection, L"O2", L"");
		sRTData.strCO2 = ini.GetValue(wchSection, L"CO2", L"");
		sRTData.strEngineRev = ini.GetValue(wchSection, L"EngineRev", L"");
		sRTData.strLambda = ini.GetValue(wchSection, L"Lambda", L"");
		sRTData.strOilTemperature = ini.GetValue(wchSection, L"OilTemperature", L"");
		sRTData.strO2OfFlowmeter = ini.GetValue(wchSection, L"O2OfFlowmeter", L"");
		sRTData.strO2OfEnvironment = ini.GetValue(wchSection, L"O2OfEnvironment", L"");
		sRTData.strHCm = ini.GetValue(wchSection, L"HCm", L"");
		sRTData.strCOm = ini.GetValue(wchSection, L"COm", L"");
		sRTData.strNOm = ini.GetValue(wchSection, L"NOm", L"");
		sRTData.strNO2m = ini.GetValue(wchSection, L"NO2m", L"");
		sRTData.strNOxm = ini.GetValue(wchSection, L"NOxm", L"");
		sRTData.strO2m = ini.GetValue(wchSection, L"O2m", L"");
		sRTData.strCO2m = ini.GetValue(wchSection, L"CO2m", L"");
		sRTData.strFuleConsumption = ini.GetValue(wchSection, L"FuleConsumption", L"");
		sRTData.strPressureOfFlowmeter = ini.GetValue(wchSection, L"PressureOfFlowmeter", L"");
		sRTData.strTemperatureOfFlowmeter = ini.GetValue(wchSection, L"TemperatureOfFlowmeter", L"");
		sRTData.strDR = ini.GetValue(wchSection, L"DR", L"");
		sRTData.strActualFluxOfGas = ini.GetValue(wchSection, L"ActualFluxOfGas", L"");
		sRTData.strStandardFluxOfGas = ini.GetValue(wchSection, L"StandardFluxOfGas", L"");
		sRTData.strFluxOfExhaust = ini.GetValue(wchSection, L"FluxOfExhaust", L"");
		sRTData.strEnvironmentalTemperature = ini.GetValue(wchSection, L"EnvironmentalTemperature", L"");
		sRTData.strRelativeHumidity = ini.GetValue(wchSection, L"RelativeHumidity", L"");
		sRTData.strAtmosphericPressure = ini.GetValue(wchSection, L"AtmosphericPressure", L"");
		sRTData.strHumidityCorrectionFactor = ini.GetValue(wchSection, L"HumidityCorrectionFactor", L"");
		sRTData.strDilutionCorrectionFactor = ini.GetValue(wchSection, L"DilutionCorrectionFactor", L"");
		sRTData.strInertiaSimulationError = ini.GetValue(wchSection, L"InertiaSimulationError", L"");
		
		// OBD数据流
		sRTData.sOBDRTData.strVelocity = ini.GetValue(wchSection, L"OBDRTData_Velocity", L"");
		sRTData.sOBDRTData.strCoolantTemp = ini.GetValue(wchSection, L"OBDRTData_CoolantTemp", L"");
		sRTData.sOBDRTData.strEngineRev = ini.GetValue(wchSection, L"OBDRTData_EngineRev", L"");
		sRTData.sOBDRTData.strSolarTermDoorPosition = ini.GetValue(wchSection, L"OBDRTData_SolarTermDoorPosition", L"");
		sRTData.sOBDRTData.strThrottleOpening = ini.GetValue(wchSection, L"OBDRTData_ThrottleOpening", L"");
		sRTData.sOBDRTData.strCalculationLoad = ini.GetValue(wchSection, L"OBDRTData_CalculationLoad", L"");
		sRTData.sOBDRTData.strForwardSensorV = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorV", L"");
		sRTData.sOBDRTData.strForwardSensorI = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorI", L"");
		sRTData.sOBDRTData.strAirCoefficient = ini.GetValue(wchSection, L"OBDRTData_AirCoefficient", L"");
		sRTData.sOBDRTData.strMAF = ini.GetValue(wchSection, L"OBDRTData_MAF", L"");
		sRTData.sOBDRTData.strMAP = ini.GetValue(wchSection, L"OBDRTData_MAP", L"");
		sRTData.sOBDRTData.strEngineOutputPower = ini.GetValue(wchSection, L"OBDRTData_EngineOutputPower", L"");
		sRTData.sOBDRTData.strChargeAirPressure = ini.GetValue(wchSection, L"OBDRTData_ChargeAirPressure", L"");
		sRTData.sOBDRTData.strFuelConsumption = ini.GetValue(wchSection, L"OBDRTData_FuelConsumption", L"");
		sRTData.sOBDRTData.strNOXConcentration = ini.GetValue(wchSection, L"OBDRTData_NOXConcentration", L"");
		sRTData.sOBDRTData.strUreaInjectionVolume = ini.GetValue(wchSection, L"OBDRTData_UreaInjectionVolume", L"");
		sRTData.sOBDRTData.strEGT = ini.GetValue(wchSection, L"OBDRTData_EGT", L"");
		sRTData.sOBDRTData.strDPFDifferentialPressure = ini.GetValue(wchSection, L"OBDRTData_DPFDifferentialPressure", L"");
		sRTData.sOBDRTData.strEGRPosition = ini.GetValue(wchSection, L"OBDRTData_EGRPosition", L"");
		sRTData.sOBDRTData.strFuelDeliveryPressure = ini.GetValue(wchSection, L"OBDRTData_FuelDeliveryPressure", L"");

		sRTData.strRemark = ini.GetValue(wchSection, L"Remark", L"");
		sRTData.strReserved1 = ini.GetValue(wchSection, L"Reserved1", L"");
		sRTData.strReserved2 = ini.GetValue(wchSection, L"Reserved2", L"");
		sRTData.strReserved3 = ini.GetValue(wchSection, L"Reserved3", L"");
		vtRTData.push_back(sRTData);
	}
}

void GetIniResultOfLUGDOWN(PRESULTOFLUGDOWN const pIniResultOfLUGDOWN)
{
	// 获取ResultOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfLUGDOWN.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfLUGDOWN.ini信息
	GetIniString(ini, L"ResultOfLUGDOWN", L"AutoID", L"", pIniResultOfLUGDOWN->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"ReportNumber", L"", pIniResultOfLUGDOWN->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfLUGDOWN", L"FuelTypeCode", L"", pIniResultOfLUGDOWN->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"FuelTypeName", L"", pIniResultOfLUGDOWN->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfLUGDOWN", L"K100", L"", pIniResultOfLUGDOWN->wchK100, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"N100", L"", pIniResultOfLUGDOWN->wchN100, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"NOx100", L"", pIniResultOfLUGDOWN->wchNOx100, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"P100", L"", pIniResultOfLUGDOWN->wchP100, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"EngineRev100", L"", pIniResultOfLUGDOWN->wchEngineRev100, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature100", L"", pIniResultOfLUGDOWN->wchOilTemperature100, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"K90", L"", pIniResultOfLUGDOWN->wchK90, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"N90", L"", pIniResultOfLUGDOWN->wchN90, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"P90", L"", pIniResultOfLUGDOWN->wchP90, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"EngineRev90", L"", pIniResultOfLUGDOWN->wchEngineRev90, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature90", L"", pIniResultOfLUGDOWN->wchOilTemperature90, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"K80", L"", pIniResultOfLUGDOWN->wchK80, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"N80", L"", pIniResultOfLUGDOWN->wchN80, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"NOx80", L"", pIniResultOfLUGDOWN->wchNOx80, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"P80", L"", pIniResultOfLUGDOWN->wchP80, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"EngineRev80", L"", pIniResultOfLUGDOWN->wchEngineRev80, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature80", L"", pIniResultOfLUGDOWN->wchOilTemperature80, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"KLimit", L"", pIniResultOfLUGDOWN->wchKLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"KLimit_EDP", L"", pIniResultOfLUGDOWN->wchKLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"NLimit", L"", pIniResultOfLUGDOWN->wchNLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"NLimit_EDP", L"", pIniResultOfLUGDOWN->wchNLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"NOxLimit", L"", pIniResultOfLUGDOWN->wchNOxLimit, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PowerCorrectionFactor", L"", pIniResultOfLUGDOWN->wchPowerCorrectionFactor, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"CorrectedPower", L"", pIniResultOfLUGDOWN->wchCorrectedPower, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"MinHP", L"", pIniResultOfLUGDOWN->wchMinHP, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"RatedRev", L"", pIniResultOfLUGDOWN->wchRatedRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfK100", L"", pIniResultOfLUGDOWN->wchPassOfK100, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfNOx100", L"", pIniResultOfLUGDOWN->wchPassOfNOx100, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfK90", L"", pIniResultOfLUGDOWN->wchPassOfK90, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfK80", L"", pIniResultOfLUGDOWN->wchPassOfK80, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfNOx80", L"", pIniResultOfLUGDOWN->wchPassOfNOx80, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfP", L"", pIniResultOfLUGDOWN->wchPassOfP, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"PassOfRev", L"", pIniResultOfLUGDOWN->wchPassOfRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"Pass", L"", pIniResultOfLUGDOWN->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"MaxHP", L"", pIniResultOfLUGDOWN->wchMaxHP, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"ActualVelMaxHP", L"", pIniResultOfLUGDOWN->wchActualVelMaxHP, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"CalculatedVelMaxHP", L"", pIniResultOfLUGDOWN->wchCalculatedVelMaxHP, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"MaxRPM", L"", pIniResultOfLUGDOWN->wchMaxRPM, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"IdleRev", L"", pIniResultOfLUGDOWN->wchIdleRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"TotalDuration", L"", pIniResultOfLUGDOWN->wchTotalDuration, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"ModeDuration", L"", pIniResultOfLUGDOWN->wchModeDuration, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature", L"", pIniResultOfLUGDOWN->wchOilTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"EnvironmentalTemperature", L"", pIniResultOfLUGDOWN->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"RelativeHumidity", L"", pIniResultOfLUGDOWN->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"AtmosphericPressure", L"", pIniResultOfLUGDOWN->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"SkipRev", L"", pIniResultOfLUGDOWN->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"SkipOilTemperature", L"", pIniResultOfLUGDOWN->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"SkipRevJudgement", L"", pIniResultOfLUGDOWN->wchSkipRevJudgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"RevPassRange", L"", pIniResultOfLUGDOWN->wchRevPassRange, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfLUGDOWN", L"Remark", L"", pIniResultOfLUGDOWN->wchRemark, 1024);
	GetIniString(ini, L"ResultOfLUGDOWN", L"Reserved1", L"", pIniResultOfLUGDOWN->wchReserved1, 50);
	GetIniString(ini, L"ResultOfLUGDOWN", L"Reserved2", L"", pIniResultOfLUGDOWN->wchReserved2, 50);
	GetIniString(ini, L"ResultOfLUGDOWN", L"Reserved3", L"", pIniResultOfLUGDOWN->wchReserved3, 50);
}

void GetIniResultOfLUGDOWNEx(SResultOfLUGDOWN* const pResultOfLUGDOWN)
{
	// 获取ResultOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfLUGDOWN.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfLUGDOWN.ini信息
	pResultOfLUGDOWN->strAutoID = ini.GetValue(L"ResultOfLUGDOWN", L"AutoID", L"");
	pResultOfLUGDOWN->strReportNumber = ini.GetValue(L"ResultOfLUGDOWN", L"ReportNumber", L"");
	pResultOfLUGDOWN->strFuelTypeCode = ini.GetValue(L"ResultOfLUGDOWN", L"FuelTypeCode", L"");
	pResultOfLUGDOWN->strFuelTypeName = ini.GetValue(L"ResultOfLUGDOWN", L"FuelTypeName", L"");
	pResultOfLUGDOWN->strK100 = ini.GetValue(L"ResultOfLUGDOWN", L"K100", L"");
	pResultOfLUGDOWN->strN100 = ini.GetValue(L"ResultOfLUGDOWN", L"N100", L"");
	pResultOfLUGDOWN->strNOx100 = ini.GetValue(L"ResultOfLUGDOWN", L"NOx100", L"");
	pResultOfLUGDOWN->strP100 = ini.GetValue(L"ResultOfLUGDOWN", L"P100", L"");
	pResultOfLUGDOWN->strEngineRev100 = ini.GetValue(L"ResultOfLUGDOWN", L"EngineRev100", L"");
	pResultOfLUGDOWN->strOilTemperature100 = ini.GetValue(L"ResultOfLUGDOWN", L"OilTemperature100", L"");
	pResultOfLUGDOWN->strK90 = ini.GetValue(L"ResultOfLUGDOWN", L"K90", L"");
	pResultOfLUGDOWN->strN90 = ini.GetValue(L"ResultOfLUGDOWN", L"N90", L"");
	pResultOfLUGDOWN->strP90 = ini.GetValue(L"ResultOfLUGDOWN", L"P90", L"");
	pResultOfLUGDOWN->strEngineRev90 = ini.GetValue(L"ResultOfLUGDOWN", L"EngineRev90", L"");
	pResultOfLUGDOWN->strOilTemperature90 = ini.GetValue(L"ResultOfLUGDOWN", L"OilTemperature90", L"");
	pResultOfLUGDOWN->strK80 = ini.GetValue(L"ResultOfLUGDOWN", L"K80", L"");
	pResultOfLUGDOWN->strN80 = ini.GetValue(L"ResultOfLUGDOWN", L"N80", L"");
	pResultOfLUGDOWN->strNOx80 = ini.GetValue(L"ResultOfLUGDOWN", L"NOx80", L"");
	pResultOfLUGDOWN->strP80 = ini.GetValue(L"ResultOfLUGDOWN", L"P80", L"");
	pResultOfLUGDOWN->strEngineRev80 = ini.GetValue(L"ResultOfLUGDOWN", L"EngineRev80", L"");
	pResultOfLUGDOWN->strOilTemperature80 = ini.GetValue(L"ResultOfLUGDOWN", L"OilTemperature80", L"");
	pResultOfLUGDOWN->strKLimit = ini.GetValue(L"ResultOfLUGDOWN", L"KLimit", L"");
	pResultOfLUGDOWN->strKLimit_EDP = ini.GetValue(L"ResultOfLUGDOWN", L"KLimit_EDP", L"");
	pResultOfLUGDOWN->strNLimit = ini.GetValue(L"ResultOfLUGDOWN", L"NLimit", L"");
	pResultOfLUGDOWN->strNLimit_EDP = ini.GetValue(L"ResultOfLUGDOWN", L"NLimit_EDP", L"");
	pResultOfLUGDOWN->strNOxLimit = ini.GetValue(L"ResultOfLUGDOWN", L"NOxLimit", L"");
	pResultOfLUGDOWN->strPowerCorrectionFactor = ini.GetValue(L"ResultOfLUGDOWN", L"PowerCorrectionFactor", L"");
	pResultOfLUGDOWN->strCorrectedPower = ini.GetValue(L"ResultOfLUGDOWN", L"CorrectedPower", L"");
	pResultOfLUGDOWN->strMinHP = ini.GetValue(L"ResultOfLUGDOWN", L"MinHP", L"");
	pResultOfLUGDOWN->strRatedRev = ini.GetValue(L"ResultOfLUGDOWN", L"RatedRev", L"");
	pResultOfLUGDOWN->strPassOfK100 = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfK100", L"");
	pResultOfLUGDOWN->strPassOfNOx100 = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfNOx100", L"");
	pResultOfLUGDOWN->strPassOfK90 = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfK90", L"");
	pResultOfLUGDOWN->strPassOfK80 = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfK80", L"");
	pResultOfLUGDOWN->strPassOfNOx80 = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfNOx80", L"");
	pResultOfLUGDOWN->strPassOfP = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfP", L"");
	pResultOfLUGDOWN->strPassOfRev = ini.GetValue(L"ResultOfLUGDOWN", L"PassOfRev", L"");
	pResultOfLUGDOWN->strPass = ini.GetValue(L"ResultOfLUGDOWN", L"Pass", L"");
	pResultOfLUGDOWN->strMaxHP = ini.GetValue(L"ResultOfLUGDOWN", L"MaxHP", L"");
	pResultOfLUGDOWN->strActualVelMaxHP = ini.GetValue(L"ResultOfLUGDOWN", L"ActualVelMaxHP", L"");
	pResultOfLUGDOWN->strCalculatedVelMaxHP = ini.GetValue(L"ResultOfLUGDOWN", L"CalculatedVelMaxHP", L"");
	pResultOfLUGDOWN->strMaxRPM = ini.GetValue(L"ResultOfLUGDOWN", L"MaxRPM", L"");
	pResultOfLUGDOWN->strIdleRev = ini.GetValue(L"ResultOfLUGDOWN", L"IdleRev", L"");
	pResultOfLUGDOWN->strTotalDuration = ini.GetValue(L"ResultOfLUGDOWN", L"TotalDuration", L"");
	pResultOfLUGDOWN->strModeDuration = ini.GetValue(L"ResultOfLUGDOWN", L"ModeDuration", L"");
	pResultOfLUGDOWN->strOilTemperature = ini.GetValue(L"ResultOfLUGDOWN", L"OilTemperature", L"");
	pResultOfLUGDOWN->strEnvironmentalTemperature = ini.GetValue(L"ResultOfLUGDOWN", L"EnvironmentalTemperature", L"");
	pResultOfLUGDOWN->strRelativeHumidity = ini.GetValue(L"ResultOfLUGDOWN", L"RelativeHumidity", L"");
	pResultOfLUGDOWN->strAtmosphericPressure = ini.GetValue(L"ResultOfLUGDOWN", L"AtmosphericPressure", L"");
	pResultOfLUGDOWN->strSkipRev = ini.GetValue(L"ResultOfLUGDOWN", L"SkipRev", L"");
	pResultOfLUGDOWN->strSkipOilTemperature = ini.GetValue(L"ResultOfLUGDOWN", L"SkipOilTemperature", L"");
	pResultOfLUGDOWN->strSkipRevJudgement = ini.GetValue(L"ResultOfLUGDOWN", L"SkipRevJudgement", L"");
	pResultOfLUGDOWN->strRevPassRange = ini.GetValue(L"ResultOfLUGDOWN", L"RevPassRange", L"");
	pResultOfLUGDOWN->strRemark = ini.GetValue(L"ResultOfLUGDOWN", L"Remark", L"");
	pResultOfLUGDOWN->strReserved1 = ini.GetValue(L"ResultOfLUGDOWN", L"Reserved1", L"");
	pResultOfLUGDOWN->strReserved2 = ini.GetValue(L"ResultOfLUGDOWN", L"Reserved2", L"");
	pResultOfLUGDOWN->strReserved3 = ini.GetValue(L"ResultOfLUGDOWN", L"Reserved3", L"");
}

void SetIniResultOfLUGDOWN(const RESULTOFLUGDOWN& iniResultOfLUGDOWN)
{
	// 获取ResultOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfLUGDOWN.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfLUGDOWN.ini信息
	WriteIniString(ini, L"ResultOfLUGDOWN", L"AutoID", iniResultOfLUGDOWN.wchAutoID);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"ReportNumber", iniResultOfLUGDOWN.wchReportNumber);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"FuelTypeCode", iniResultOfLUGDOWN.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"FuelTypeName", iniResultOfLUGDOWN.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"K100", iniResultOfLUGDOWN.wchK100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"N100", iniResultOfLUGDOWN.wchN100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"NOx100", iniResultOfLUGDOWN.wchNOx100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"P100", iniResultOfLUGDOWN.wchP100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"EngineRev100", iniResultOfLUGDOWN.wchEngineRev100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature100", iniResultOfLUGDOWN.wchOilTemperature100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"K90", iniResultOfLUGDOWN.wchK90);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"N90", iniResultOfLUGDOWN.wchN90);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"P90", iniResultOfLUGDOWN.wchP90);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"EngineRev90", iniResultOfLUGDOWN.wchEngineRev90);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature90", iniResultOfLUGDOWN.wchOilTemperature90);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"K80", iniResultOfLUGDOWN.wchK80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"N80", iniResultOfLUGDOWN.wchN80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"NOx80", iniResultOfLUGDOWN.wchNOx80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"P80", iniResultOfLUGDOWN.wchP80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"EngineRev80", iniResultOfLUGDOWN.wchEngineRev80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature80", iniResultOfLUGDOWN.wchOilTemperature80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"KLimit", iniResultOfLUGDOWN.wchKLimit);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"KLimit_EDP", iniResultOfLUGDOWN.wchKLimit_EDP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"NLimit", iniResultOfLUGDOWN.wchNLimit);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"NLimit_EDP", iniResultOfLUGDOWN.wchNLimit_EDP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"NOxLimit", iniResultOfLUGDOWN.wchNOxLimit);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PowerCorrectionFactor", iniResultOfLUGDOWN.wchPowerCorrectionFactor);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"CorrectedPower", iniResultOfLUGDOWN.wchCorrectedPower);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"MinHP", iniResultOfLUGDOWN.wchMinHP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"RatedRev", iniResultOfLUGDOWN.wchRatedRev);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfK100", iniResultOfLUGDOWN.wchPassOfK100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfNOx100", iniResultOfLUGDOWN.wchPassOfNOx100);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfK90", iniResultOfLUGDOWN.wchPassOfK90);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfK80", iniResultOfLUGDOWN.wchPassOfK80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfNOx80", iniResultOfLUGDOWN.wchPassOfNOx80);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfP", iniResultOfLUGDOWN.wchPassOfP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"PassOfRev", iniResultOfLUGDOWN.wchPassOfRev);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"Pass", iniResultOfLUGDOWN.wchPass);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"MaxHP", iniResultOfLUGDOWN.wchMaxHP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"ActualVelMaxHP", iniResultOfLUGDOWN.wchActualVelMaxHP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"CalculatedVelMaxHP", iniResultOfLUGDOWN.wchCalculatedVelMaxHP);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"MaxRPM", iniResultOfLUGDOWN.wchMaxRPM);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"IdleRev", iniResultOfLUGDOWN.wchIdleRev);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"TotalDuration", iniResultOfLUGDOWN.wchTotalDuration);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"ModeDuration", iniResultOfLUGDOWN.wchModeDuration);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"OilTemperature", iniResultOfLUGDOWN.wchOilTemperature);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"EnvironmentalTemperature", iniResultOfLUGDOWN.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"RelativeHumidity", iniResultOfLUGDOWN.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"AtmosphericPressure", iniResultOfLUGDOWN.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"SkipRev", iniResultOfLUGDOWN.wchSkipRev);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"SkipOilTemperature", iniResultOfLUGDOWN.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"SkipRevJudgement", iniResultOfLUGDOWN.wchSkipRevJudgement);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"RevPassRange", iniResultOfLUGDOWN.wchRevPassRange);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"Remark", iniResultOfLUGDOWN.wchRemark);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"Reserved1", iniResultOfLUGDOWN.wchReserved1);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"Reserved2", iniResultOfLUGDOWN.wchReserved2);
	WriteIniString(ini, L"ResultOfLUGDOWN", L"Reserved3", iniResultOfLUGDOWN.wchReserved3);
	
	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfLUGDOWN(list<REALTIMEDATAOFLUGDOWN>& listRealTimeDataOfLUGDOWN)
{
	// 获取RealTimeDataOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfLUGDOWN.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfLUGDOWN iniRealTimeDataOfLUGDOWN;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfLUGDOWN.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfLUGDOWN.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfLUGDOWN.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfLUGDOWN.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfLUGDOWN.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfLUGDOWN.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfLUGDOWN.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfLUGDOWN.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfLUGDOWN.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfLUGDOWN.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfLUGDOWN.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfLUGDOWN.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"State", L"", iniRealTimeDataOfLUGDOWN.wchState, 50);
		GetIniString(ini, wchSection, L"K", L"", iniRealTimeDataOfLUGDOWN.wchK, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"N", L"", iniRealTimeDataOfLUGDOWN.wchN, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfLUGDOWN.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfLUGDOWN.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfLUGDOWN.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfLUGDOWN.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfLUGDOWN.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PowerCorrectionFactor", L"", iniRealTimeDataOfLUGDOWN.wchPowerCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfLUGDOWN.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfLUGDOWN.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfLUGDOWN.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO2", L"", iniRealTimeDataOfLUGDOWN.wchNO2, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfLUGDOWN.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfLUGDOWN.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfLUGDOWN.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfLUGDOWN.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfLUGDOWN.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfLUGDOWN.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfLUGDOWN.wchReserved3, 50);

		listRealTimeDataOfLUGDOWN.push_back(iniRealTimeDataOfLUGDOWN);
	}
}

void GetIniRealTimeDataOfLUGDOWNVector(vector<REALTIMEDATAOFLUGDOWN>& vRealTimeDataOfLUGDOWN)
{
	// 获取RealTimeDataOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfLUGDOWN.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfLUGDOWN iniRealTimeDataOfLUGDOWN;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfLUGDOWN.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfLUGDOWN.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfLUGDOWN.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfLUGDOWN.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfLUGDOWN.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfLUGDOWN.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfLUGDOWN.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfLUGDOWN.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfLUGDOWN.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfLUGDOWN.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfLUGDOWN.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfLUGDOWN.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"State", L"", iniRealTimeDataOfLUGDOWN.wchState, 50);
		GetIniString(ini, wchSection, L"K", L"", iniRealTimeDataOfLUGDOWN.wchK, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"N", L"", iniRealTimeDataOfLUGDOWN.wchN, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfLUGDOWN.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfLUGDOWN.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfLUGDOWN.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfLUGDOWN.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfLUGDOWN.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PowerCorrectionFactor", L"", iniRealTimeDataOfLUGDOWN.wchPowerCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfLUGDOWN.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfLUGDOWN.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfLUGDOWN.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO2", L"", iniRealTimeDataOfLUGDOWN.wchNO2, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfLUGDOWN.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfLUGDOWN.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfLUGDOWN.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfLUGDOWN.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfLUGDOWN.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfLUGDOWN.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfLUGDOWN.wchReserved3, 50);

		vRealTimeDataOfLUGDOWN.push_back(iniRealTimeDataOfLUGDOWN);
	}
}

void GetIniRealTimeDataOfLUGDOWNEx(std::vector<SRealTimeDataOfLUGDOWN> &vtRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfLUGDOWN.ini"*/)
{
	// 获取RealTimeDataOfLUGDOWN.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	for(int i=1; i<=nTotal; i++)
	{
		SRealTimeDataOfLUGDOWN sRTData;
		// 获取RealTimeDataOfLUGDOWN.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		sRTData.strRunningNumber = ini.GetValue(wchSection, L"RunningNumber", L"");
		sRTData.strReportNumber = ini.GetValue(wchSection, L"ReportNumber", L"");
		sRTData.strFuelTypeCode = ini.GetValue(wchSection, L"FuelTypeCode", L"");
		sRTData.strFuelTypeName = ini.GetValue(wchSection, L"FuelTypeName", L"");
		sRTData.strTime = ini.GetValue(wchSection, L"Time", L"");
		sRTData.strSamplingTime = ini.GetValue(wchSection, L"SamplingTime", L"");
		sRTData.strVelocity = ini.GetValue(wchSection, L"Velocity", L"");
		sRTData.strPower = ini.GetValue(wchSection, L"Power", L"");
		sRTData.strForce = ini.GetValue(wchSection, L"Force", L"");
		sRTData.strIHP = ini.GetValue(wchSection, L"IHP", L"");
		sRTData.strPLHP = ini.GetValue(wchSection, L"PLHP", L"");
		sRTData.strState = ini.GetValue(wchSection, L"State", L"");
		sRTData.strK = ini.GetValue(wchSection, L"K", L"");
		sRTData.strN = ini.GetValue(wchSection, L"N", L"");
		sRTData.strEngineRev = ini.GetValue(wchSection, L"EngineRev", L"");
		sRTData.strOilTemperature = ini.GetValue(wchSection, L"OilTemperature", L"");
		sRTData.strEnvironmentalTemperature = ini.GetValue(wchSection, L"EnvironmentalTemperature", L"");
		sRTData.strRelativeHumidity = ini.GetValue(wchSection, L"RelativeHumidity", L"");
		sRTData.strAtmosphericPressure = ini.GetValue(wchSection, L"AtmosphericPressure", L"");
		sRTData.strPowerCorrectionFactor = ini.GetValue(wchSection, L"PowerCorrectionFactor", L"");
		sRTData.strCO = ini.GetValue(wchSection, L"CO", L"");
		sRTData.strHC = ini.GetValue(wchSection, L"HC", L"");
		sRTData.strNO = ini.GetValue(wchSection, L"NO", L"");
		sRTData.strNO2 = ini.GetValue(wchSection, L"NO2", L"");
		sRTData.strNOx = ini.GetValue(wchSection, L"NOx", L"");
		sRTData.strCO2 = ini.GetValue(wchSection, L"CO2", L"");
		sRTData.strO2 = ini.GetValue(wchSection, L"O2", L"");
		sRTData.strHumidityCorrectionFactor = ini.GetValue(wchSection, L"HumidityCorrectionFactor", L"");

		// OBD数据流
		sRTData.sOBDRTData.strVelocity = ini.GetValue(wchSection, L"OBDRTData_Velocity", L"");
		sRTData.sOBDRTData.strCoolantTemp = ini.GetValue(wchSection, L"OBDRTData_CoolantTemp", L"");
		sRTData.sOBDRTData.strEngineRev = ini.GetValue(wchSection, L"OBDRTData_EngineRev", L"");
		sRTData.sOBDRTData.strSolarTermDoorPosition = ini.GetValue(wchSection, L"OBDRTData_SolarTermDoorPosition", L"");
		sRTData.sOBDRTData.strThrottleOpening = ini.GetValue(wchSection, L"OBDRTData_ThrottleOpening", L"");
		sRTData.sOBDRTData.strCalculationLoad = ini.GetValue(wchSection, L"OBDRTData_CalculationLoad", L"");
		sRTData.sOBDRTData.strForwardSensorV = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorV", L"");
		sRTData.sOBDRTData.strForwardSensorI = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorI", L"");
		sRTData.sOBDRTData.strAirCoefficient = ini.GetValue(wchSection, L"OBDRTData_AirCoefficient", L"");
		sRTData.sOBDRTData.strMAF = ini.GetValue(wchSection, L"OBDRTData_MAF", L"");
		sRTData.sOBDRTData.strMAP = ini.GetValue(wchSection, L"OBDRTData_MAP", L"");
		sRTData.sOBDRTData.strEngineOutputPower = ini.GetValue(wchSection, L"OBDRTData_EngineOutputPower", L"");
		sRTData.sOBDRTData.strChargeAirPressure = ini.GetValue(wchSection, L"OBDRTData_ChargeAirPressure", L"");
		sRTData.sOBDRTData.strFuelConsumption = ini.GetValue(wchSection, L"OBDRTData_FuelConsumption", L"");
		sRTData.sOBDRTData.strNOXConcentration = ini.GetValue(wchSection, L"OBDRTData_NOXConcentration", L"");
		sRTData.sOBDRTData.strUreaInjectionVolume = ini.GetValue(wchSection, L"OBDRTData_UreaInjectionVolume", L"");
		sRTData.sOBDRTData.strEGT = ini.GetValue(wchSection, L"OBDRTData_EGT", L"");
		sRTData.sOBDRTData.strDPFDifferentialPressure = ini.GetValue(wchSection, L"OBDRTData_DPFDifferentialPressure", L"");
		sRTData.sOBDRTData.strEGRPosition = ini.GetValue(wchSection, L"OBDRTData_EGRPosition", L"");
		sRTData.sOBDRTData.strFuelDeliveryPressure = ini.GetValue(wchSection, L"OBDRTData_FuelDeliveryPressure", L"");

		sRTData.strRemark = ini.GetValue(wchSection, L"Remark", L"");
		sRTData.strReserved1 = ini.GetValue(wchSection, L"Reserved1", L"");
		sRTData.strReserved2 = ini.GetValue(wchSection, L"Reserved2", L"");
		sRTData.strReserved3 = ini.GetValue(wchSection, L"Reserved3", L"");		
		vtRTData.push_back(sRTData);
	}
}

void GetIniResultOfFSUNHT(PRESULTOFFSUNHT const pIniResultOfFSUNHT)
{
	// 获取ResultOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFSUNHT.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFSUNHT.ini信息
	GetIniString(ini, L"ResultOfFSUNHT", L"AutoID", L"", pIniResultOfFSUNHT->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"ReportNumber", L"", pIniResultOfFSUNHT->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfFSUNHT", L"FuelTypeCode", L"", pIniResultOfFSUNHT->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"FuelTypeName", L"", pIniResultOfFSUNHT->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfFSUNHT", L"EngineRev", L"", pIniResultOfFSUNHT->wchEngineRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Value1", L"", pIniResultOfFSUNHT->wchValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Value2", L"", pIniResultOfFSUNHT->wchValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Value3", L"", pIniResultOfFSUNHT->wchValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Value4", L"", pIniResultOfFSUNHT->wchValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Average", L"", pIniResultOfFSUNHT->wchAverage, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Limit", L"", pIniResultOfFSUNHT->wchLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Limit_EDP", L"", pIniResultOfFSUNHT->wchLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Pass", L"", pIniResultOfFSUNHT->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"EnvironmentalTemperature", L"", pIniResultOfFSUNHT->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"RelativeHumidity", L"", pIniResultOfFSUNHT->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"AtmosphericPressure", L"", pIniResultOfFSUNHT->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"OilTemperature", L"", pIniResultOfFSUNHT->wchOilTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"SkipRev", L"", pIniResultOfFSUNHT->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"SkipOilTemperature", L"", pIniResultOfFSUNHT->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUNHT", L"Remark", L"", pIniResultOfFSUNHT->wchRemark, 1024);
	GetIniString(ini, L"ResultOfFSUNHT", L"Reserved1", L"", pIniResultOfFSUNHT->wchReserved1, 50);
	GetIniString(ini, L"ResultOfFSUNHT", L"Reserved2", L"", pIniResultOfFSUNHT->wchReserved2, 50);
	GetIniString(ini, L"ResultOfFSUNHT", L"Reserved3", L"", pIniResultOfFSUNHT->wchReserved3, 50);

}

void GetIniResultOfFSUNHTEx(SResultOfFSUNHT* const pResultOfFSUNHT)
{
	// 获取ResultOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFSUNHT.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFSUNHT.ini信息
	pResultOfFSUNHT->strAutoID = ini.GetValue(L"ResultOfFSUNHT", L"AutoID", L"");
	pResultOfFSUNHT->strReportNumber = ini.GetValue(L"ResultOfFSUNHT", L"ReportNumber", L"");
	pResultOfFSUNHT->strFuelTypeCode = ini.GetValue(L"ResultOfFSUNHT", L"FuelTypeCode", L"");
	pResultOfFSUNHT->strFuelTypeName = ini.GetValue(L"ResultOfFSUNHT", L"FuelTypeName", L"");
	pResultOfFSUNHT->strEngineRev = ini.GetValue(L"ResultOfFSUNHT", L"EngineRev", L"");
	pResultOfFSUNHT->strValue1 = ini.GetValue(L"ResultOfFSUNHT", L"Value1", L"");
	pResultOfFSUNHT->strValue2 = ini.GetValue(L"ResultOfFSUNHT", L"Value2", L"");
	pResultOfFSUNHT->strValue3 = ini.GetValue(L"ResultOfFSUNHT", L"Value3", L"");
	pResultOfFSUNHT->strValue4 = ini.GetValue(L"ResultOfFSUNHT", L"Value4", L"");
	pResultOfFSUNHT->strAverage = ini.GetValue(L"ResultOfFSUNHT", L"Average", L"");
	pResultOfFSUNHT->strLimit = ini.GetValue(L"ResultOfFSUNHT", L"Limit", L"");
	pResultOfFSUNHT->strLimit_EDP = ini.GetValue(L"ResultOfFSUNHT", L"Limit_EDP", L"");
	pResultOfFSUNHT->strPass = ini.GetValue(L"ResultOfFSUNHT", L"Pass", L"");
	pResultOfFSUNHT->strEnvironmentalTemperature = ini.GetValue(L"ResultOfFSUNHT", L"EnvironmentalTemperature", L"");
	pResultOfFSUNHT->strRelativeHumidity = ini.GetValue(L"ResultOfFSUNHT", L"RelativeHumidity", L"");
	pResultOfFSUNHT->strAtmosphericPressure = ini.GetValue(L"ResultOfFSUNHT", L"AtmosphericPressure", L"");
	pResultOfFSUNHT->strOilTemperature = ini.GetValue(L"ResultOfFSUNHT", L"OilTemperature", L"");
	pResultOfFSUNHT->strSkipRev = ini.GetValue(L"ResultOfFSUNHT", L"SkipRev", L"");
	pResultOfFSUNHT->strSkipOilTemperature = ini.GetValue(L"ResultOfFSUNHT", L"SkipOilTemperature", L"");
	pResultOfFSUNHT->strRemark = ini.GetValue(L"ResultOfFSUNHT", L"Remark", L"");
	pResultOfFSUNHT->strReserved1 = ini.GetValue(L"ResultOfFSUNHT", L"Reserved1", L"");
	pResultOfFSUNHT->strReserved2 = ini.GetValue(L"ResultOfFSUNHT", L"Reserved2", L"");
	pResultOfFSUNHT->strReserved3 = ini.GetValue(L"ResultOfFSUNHT", L"Reserved3", L"");
}

void SetIniResultOfFSUNHT(const RESULTOFFSUNHT& iniResultOfFSUNHT)
{
	// 获取ResultOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFSUNHT.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfFSUNHT.ini信息
	WriteIniString(ini, L"ResultOfFSUNHT", L"AutoID", iniResultOfFSUNHT.wchAutoID);
	WriteIniString(ini, L"ResultOfFSUNHT", L"ReportNumber", iniResultOfFSUNHT.wchReportNumber);
	WriteIniString(ini, L"ResultOfFSUNHT", L"FuelTypeCode", iniResultOfFSUNHT.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfFSUNHT", L"FuelTypeName", iniResultOfFSUNHT.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfFSUNHT", L"EngineRev", iniResultOfFSUNHT.wchEngineRev);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Value1", iniResultOfFSUNHT.wchValue1);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Value2", iniResultOfFSUNHT.wchValue2);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Value3", iniResultOfFSUNHT.wchValue3);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Value4", iniResultOfFSUNHT.wchValue4);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Average", iniResultOfFSUNHT.wchAverage);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Limit", iniResultOfFSUNHT.wchLimit);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Limit_EDP", iniResultOfFSUNHT.wchLimit_EDP);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Pass", iniResultOfFSUNHT.wchPass);
	WriteIniString(ini, L"ResultOfFSUNHT", L"EnvironmentalTemperature", iniResultOfFSUNHT.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfFSUNHT", L"RelativeHumidity", iniResultOfFSUNHT.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfFSUNHT", L"AtmosphericPressure", iniResultOfFSUNHT.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfFSUNHT", L"OilTemperature", iniResultOfFSUNHT.wchOilTemperature);
	WriteIniString(ini, L"ResultOfFSUNHT", L"SkipRev", iniResultOfFSUNHT.wchSkipRev);
	WriteIniString(ini, L"ResultOfFSUNHT", L"SkipOilTemperature", iniResultOfFSUNHT.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Remark", iniResultOfFSUNHT.wchRemark);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Reserved1", iniResultOfFSUNHT.wchReserved1);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Reserved2", iniResultOfFSUNHT.wchReserved2);
	WriteIniString(ini, L"ResultOfFSUNHT", L"Reserved3", iniResultOfFSUNHT.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfFSUNHT(list<REALTIMEDATAOFFSUNHT>& listRealTimeDataOfFSUNHT)
{
	// 获取RealTimeDataOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfFSUNHT.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfFSUNHT iniRealTimeDataOfFSUNHT;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfFSUNHT.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfFSUNHT.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfFSUNHT.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"Order", L"", iniRealTimeDataOfFSUNHT.wchOrder, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfFSUNHT.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfFSUNHT.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"K", L"", iniRealTimeDataOfFSUNHT.wchK, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"N", L"", iniRealTimeDataOfFSUNHT.wchN, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfFSUNHT.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfFSUNHT.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfFSUNHT.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfFSUNHT.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfFSUNHT.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfFSUNHT.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfFSUNHT.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfFSUNHT.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfFSUNHT.wchReserved3, 50);

		listRealTimeDataOfFSUNHT.push_back(iniRealTimeDataOfFSUNHT);
	}
}

void GetIniRealTimeDataOfFSUNHTVector(vector<REALTIMEDATAOFFSUNHT>& vRealTimeDataOfFSUNHT)
{
	// 获取RealTimeDataOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfFSUNHT.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	REALTIMEDATAOFFSUNHT iniRealTimeDataOfFSUNHT;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfFSUNHT.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfFSUNHT.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfFSUNHT.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"Order", L"", iniRealTimeDataOfFSUNHT.wchOrder, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfFSUNHT.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfFSUNHT.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"K", L"", iniRealTimeDataOfFSUNHT.wchK, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"N", L"", iniRealTimeDataOfFSUNHT.wchN, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfFSUNHT.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfFSUNHT.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfFSUNHT.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfFSUNHT.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfFSUNHT.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfFSUNHT.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfFSUNHT.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfFSUNHT.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfFSUNHT.wchReserved3, 50);

		vRealTimeDataOfFSUNHT.push_back(iniRealTimeDataOfFSUNHT);
	}
}

void GetIniRealTimeDataOfFSUNHTEx(std::vector<SRealTimeDataOfFSUNHT> &vtRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfFSUNHT.ini"*/)
{
	// 获取RealTimeDataOfFSUNHT.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	for(int i=1; i<=nTotal; i++)
	{
		SRealTimeDataOfFSUNHT sRTData;
		// 获取RealTimeDataOfFSUNHT.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		sRTData.strRunningNumber = ini.GetValue(wchSection, L"RunningNumber", L"");
		sRTData.strReportNumber = ini.GetValue(wchSection, L"ReportNumber", L"");
		sRTData.strOrder = ini.GetValue(wchSection, L"Order", L"");
		sRTData.strTime = ini.GetValue(wchSection, L"Time", L"");
		sRTData.strSamplingTime = ini.GetValue(wchSection, L"SamplingTime", L"");
		sRTData.strK = ini.GetValue(wchSection, L"K", L"");
		sRTData.strN = ini.GetValue(wchSection, L"N", L"");
		sRTData.strEngineRev = ini.GetValue(wchSection, L"EngineRev", L"");
		sRTData.strOilTemperature = ini.GetValue(wchSection, L"OilTemperature", L"");
		sRTData.strEnvironmentalTemperature = ini.GetValue(wchSection, L"EnvironmentalTemperature", L"");
		sRTData.strRelativeHumidity = ini.GetValue(wchSection, L"RelativeHumidity", L"");
		sRTData.strAtmosphericPressure = ini.GetValue(wchSection, L"AtmosphericPressure", L"");

		// OBD数据流
		sRTData.sOBDRTData.strVelocity = ini.GetValue(wchSection, L"OBDRTData_Velocity", L"");
		sRTData.sOBDRTData.strCoolantTemp = ini.GetValue(wchSection, L"OBDRTData_CoolantTemp", L"");
		sRTData.sOBDRTData.strEngineRev = ini.GetValue(wchSection, L"OBDRTData_EngineRev", L"");
		sRTData.sOBDRTData.strSolarTermDoorPosition = ini.GetValue(wchSection, L"OBDRTData_SolarTermDoorPosition", L"");
		sRTData.sOBDRTData.strThrottleOpening = ini.GetValue(wchSection, L"OBDRTData_ThrottleOpening", L"");
		sRTData.sOBDRTData.strCalculationLoad = ini.GetValue(wchSection, L"OBDRTData_CalculationLoad", L"");
		sRTData.sOBDRTData.strForwardSensorV = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorV", L"");
		sRTData.sOBDRTData.strForwardSensorI = ini.GetValue(wchSection, L"OBDRTData_ForwardSensorI", L"");
		sRTData.sOBDRTData.strAirCoefficient = ini.GetValue(wchSection, L"OBDRTData_AirCoefficient", L"");
		sRTData.sOBDRTData.strMAF = ini.GetValue(wchSection, L"OBDRTData_MAF", L"");
		sRTData.sOBDRTData.strMAP = ini.GetValue(wchSection, L"OBDRTData_MAP", L"");
		sRTData.sOBDRTData.strEngineOutputPower = ini.GetValue(wchSection, L"OBDRTData_EngineOutputPower", L"");
		sRTData.sOBDRTData.strChargeAirPressure = ini.GetValue(wchSection, L"OBDRTData_ChargeAirPressure", L"");
		sRTData.sOBDRTData.strFuelConsumption = ini.GetValue(wchSection, L"OBDRTData_FuelConsumption", L"");
		sRTData.sOBDRTData.strNOXConcentration = ini.GetValue(wchSection, L"OBDRTData_NOXConcentration", L"");
		sRTData.sOBDRTData.strUreaInjectionVolume = ini.GetValue(wchSection, L"OBDRTData_UreaInjectionVolume", L"");
		sRTData.sOBDRTData.strEGT = ini.GetValue(wchSection, L"OBDRTData_EGT", L"");
		sRTData.sOBDRTData.strDPFDifferentialPressure = ini.GetValue(wchSection, L"OBDRTData_DPFDifferentialPressure", L"");
		sRTData.sOBDRTData.strEGRPosition = ini.GetValue(wchSection, L"OBDRTData_EGRPosition", L"");
		sRTData.sOBDRTData.strFuelDeliveryPressure = ini.GetValue(wchSection, L"OBDRTData_FuelDeliveryPressure", L"");

		sRTData.strRemark = ini.GetValue(wchSection, L"Remark", L"");
		sRTData.strReserved1 = ini.GetValue(wchSection, L"Reserved1", L"");
		sRTData.strReserved2 = ini.GetValue(wchSection, L"Reserved2", L"");
		sRTData.strReserved3 = ini.GetValue(wchSection, L"Reserved3", L"");
		vtRTData.push_back(sRTData);
	}
}

void GetIniResultOfFSUYD(PRESULTOFFSUYD const pIniResultOfFSUYD)
{
	// 获取ResultOfFSUYD.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFSUYD.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFSUYD.ini信息
	GetIniString(ini, L"ResultOfFSUYD", L"AutoID", L"", pIniResultOfFSUYD->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"ReportNumber", L"", pIniResultOfFSUYD->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfFSUYD", L"FuelTypeCode", L"", pIniResultOfFSUYD->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"FuelTypeName", L"", pIniResultOfFSUYD->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfFSUYD", L"EngineRev", L"", pIniResultOfFSUYD->wchEngineRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Value1", L"", pIniResultOfFSUYD->wchValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Value2", L"", pIniResultOfFSUYD->wchValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Value3", L"", pIniResultOfFSUYD->wchValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Value4", L"", pIniResultOfFSUYD->wchValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Average", L"", pIniResultOfFSUYD->wchAverage, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Limit", L"", pIniResultOfFSUYD->wchLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Limit_EDP", L"", pIniResultOfFSUYD->wchLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Pass", L"", pIniResultOfFSUYD->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"EnvironmentalTemperature", L"", pIniResultOfFSUYD->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"RelativeHumidity", L"", pIniResultOfFSUYD->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"AtmosphericPressure", L"", pIniResultOfFSUYD->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"OilTemperature", L"", pIniResultOfFSUYD->wchOilTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"SkipRev", L"", pIniResultOfFSUYD->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"SkipOilTemperature", L"", pIniResultOfFSUYD->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFSUYD", L"Remark", L"", pIniResultOfFSUYD->wchRemark, 1024);
	GetIniString(ini, L"ResultOfFSUYD", L"Reserved1", L"", pIniResultOfFSUYD->wchReserved1, 50);
	GetIniString(ini, L"ResultOfFSUYD", L"Reserved2", L"", pIniResultOfFSUYD->wchReserved2, 50);
	GetIniString(ini, L"ResultOfFSUYD", L"Reserved3", L"", pIniResultOfFSUYD->wchReserved3, 50);

}


void SetIniResultOfFSUYD(const RESULTOFFSUYD& iniResultOfFSUYD)
{
	// 获取ResultOfFSUYD.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFSUYD.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfFSUYD.ini信息
	WriteIniString(ini, L"ResultOfFSUYD", L"AutoID", iniResultOfFSUYD.wchAutoID);
	WriteIniString(ini, L"ResultOfFSUYD", L"ReportNumber", iniResultOfFSUYD.wchReportNumber);
	WriteIniString(ini, L"ResultOfFSUYD", L"FuelTypeCode", iniResultOfFSUYD.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfFSUYD", L"FuelTypeName", iniResultOfFSUYD.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfFSUYD", L"EngineRev", iniResultOfFSUYD.wchEngineRev);
	WriteIniString(ini, L"ResultOfFSUYD", L"Value1", iniResultOfFSUYD.wchValue1);
	WriteIniString(ini, L"ResultOfFSUYD", L"Value2", iniResultOfFSUYD.wchValue2);
	WriteIniString(ini, L"ResultOfFSUYD", L"Value3", iniResultOfFSUYD.wchValue3);
	WriteIniString(ini, L"ResultOfFSUYD", L"Value4", iniResultOfFSUYD.wchValue4);
	WriteIniString(ini, L"ResultOfFSUYD", L"Average", iniResultOfFSUYD.wchAverage);
	WriteIniString(ini, L"ResultOfFSUYD", L"Limit", iniResultOfFSUYD.wchLimit);
	WriteIniString(ini, L"ResultOfFSUYD", L"Limit_EDP", iniResultOfFSUYD.wchLimit_EDP);
	WriteIniString(ini, L"ResultOfFSUYD", L"Pass", iniResultOfFSUYD.wchPass);
	WriteIniString(ini, L"ResultOfFSUYD", L"EnvironmentalTemperature", iniResultOfFSUYD.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfFSUYD", L"RelativeHumidity", iniResultOfFSUYD.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfFSUYD", L"AtmosphericPressure", iniResultOfFSUYD.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfFSUYD", L"OilTemperature", iniResultOfFSUYD.wchOilTemperature);
	WriteIniString(ini, L"ResultOfFSUYD", L"SkipRev", iniResultOfFSUYD.wchSkipRev);
	WriteIniString(ini, L"ResultOfFSUYD", L"SkipOilTemperature", iniResultOfFSUYD.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfFSUYD", L"Remark", iniResultOfFSUYD.wchRemark);
	WriteIniString(ini, L"ResultOfFSUYD", L"Reserved1", iniResultOfFSUYD.wchReserved1);
	WriteIniString(ini, L"ResultOfFSUYD", L"Reserved2", iniResultOfFSUYD.wchReserved2);
	WriteIniString(ini, L"ResultOfFSUYD", L"Reserved3", iniResultOfFSUYD.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniResultOfAVFSU(PRESULTOFAVFSU const pIniResultOfAVFSU)
{
	// 获取ResultOfAVFSU.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAVFSU.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfAVFSU.ini信息
	GetIniString(ini, L"ResultOfAVFSU", L"AutoID", L"", pIniResultOfAVFSU->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"ReportNumber", L"", pIniResultOfAVFSU->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfAVFSU", L"FuelTypeCode", L"", pIniResultOfAVFSU->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"FuelTypeName", L"", pIniResultOfAVFSU->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfAVFSU", L"EngineRev", L"", pIniResultOfAVFSU->wchEngineRev, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Value1", L"", pIniResultOfAVFSU->wchValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Value2", L"", pIniResultOfAVFSU->wchValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Value3", L"", pIniResultOfAVFSU->wchValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Average", L"", pIniResultOfAVFSU->wchAverage, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Limit", L"", pIniResultOfAVFSU->wchLimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Limit_EDP", L"", pIniResultOfAVFSU->wchLimit_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Pass", L"", pIniResultOfAVFSU->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"EnvironmentalTemperature", L"", pIniResultOfAVFSU->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"RelativeHumidity", L"", pIniResultOfAVFSU->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"AtmosphericPressure", L"", pIniResultOfAVFSU->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"OilTemperature", L"", pIniResultOfAVFSU->wchOilTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"SkipRev", L"", pIniResultOfAVFSU->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"SkipOilTemperature", L"", pIniResultOfAVFSU->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAVFSU", L"Remark", L"", pIniResultOfAVFSU->wchRemark, 1024);
	GetIniString(ini, L"ResultOfAVFSU", L"Reserved1", L"", pIniResultOfAVFSU->wchReserved1, 50);
	GetIniString(ini, L"ResultOfAVFSU", L"Reserved2", L"", pIniResultOfAVFSU->wchReserved2, 50);
	GetIniString(ini, L"ResultOfAVFSU", L"Reserved3", L"", pIniResultOfAVFSU->wchReserved3, 50);

}

void SetIniResultOfAVFSU(const RESULTOFAVFSU& iniResultOfAVFSU)
{
	// 获取ResultOfAVFSU.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAVFSU.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfAVFSU.ini信息
	WriteIniString(ini, L"ResultOfAVFSU", L"AutoID", iniResultOfAVFSU.wchAutoID);
	WriteIniString(ini, L"ResultOfAVFSU", L"ReportNumber", iniResultOfAVFSU.wchReportNumber);
	WriteIniString(ini, L"ResultOfAVFSU", L"FuelTypeCode", iniResultOfAVFSU.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfAVFSU", L"FuelTypeName", iniResultOfAVFSU.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfAVFSU", L"EngineRev", iniResultOfAVFSU.wchEngineRev);
	WriteIniString(ini, L"ResultOfAVFSU", L"Value1", iniResultOfAVFSU.wchValue1);
	WriteIniString(ini, L"ResultOfAVFSU", L"Value2", iniResultOfAVFSU.wchValue2);
	WriteIniString(ini, L"ResultOfAVFSU", L"Value3", iniResultOfAVFSU.wchValue3);
	WriteIniString(ini, L"ResultOfAVFSU", L"Average", iniResultOfAVFSU.wchAverage);
	WriteIniString(ini, L"ResultOfAVFSU", L"Limit", iniResultOfAVFSU.wchLimit);
	WriteIniString(ini, L"ResultOfAVFSU", L"Limit_EDP", iniResultOfAVFSU.wchLimit_EDP);
	WriteIniString(ini, L"ResultOfAVFSU", L"Pass", iniResultOfAVFSU.wchPass);
	WriteIniString(ini, L"ResultOfAVFSU", L"EnvironmentalTemperature", iniResultOfAVFSU.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfAVFSU", L"RelativeHumidity", iniResultOfAVFSU.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfAVFSU", L"AtmosphericPressure", iniResultOfAVFSU.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfAVFSU", L"OilTemperature", iniResultOfAVFSU.wchOilTemperature);
	WriteIniString(ini, L"ResultOfAVFSU", L"SkipRev", iniResultOfAVFSU.wchSkipRev);
	WriteIniString(ini, L"ResultOfAVFSU", L"SkipOilTemperature", iniResultOfAVFSU.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfAVFSU", L"Remark", iniResultOfAVFSU.wchRemark);
	WriteIniString(ini, L"ResultOfAVFSU", L"Reserved1", iniResultOfAVFSU.wchReserved1);
	WriteIniString(ini, L"ResultOfAVFSU", L"Reserved2", iniResultOfAVFSU.wchReserved2);
	WriteIniString(ini, L"ResultOfAVFSU", L"Reserved3", iniResultOfAVFSU.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniResultOfMI(PRESULTOFMI const pIniResultOfMI)
{
	// 获取ResultOfAVFSU.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfMI.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfMI.ini信息
	GetIniString(ini, L"ResultOfMI", L"AutoID", L"", pIniResultOfMI->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"ReportNumber", L"", pIniResultOfMI->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfMI", L"FuelTypeCode", L"", pIniResultOfMI->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"FuelTypeName", L"", pIniResultOfMI->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfMI", L"CO_1", L"", pIniResultOfMI->wchCO_1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"HC_1", L"", pIniResultOfMI->wchHC_1, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"CO2_1", L"", pIniResultOfMI->wchCO2_1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"NO_1", L"", pIniResultOfMI->wchNO_1, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"O2_1", L"", pIniResultOfMI->wchO2_1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"Lambda_1", L"", pIniResultOfMI->wchLambda_1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"OilTemperature_1", L"", pIniResultOfMI->wchOilTemperature_1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"Rev_1", L"", pIniResultOfMI->wchRev_1, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"CO_2", L"", pIniResultOfMI->wchCO_2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"HC_2", L"", pIniResultOfMI->wchHC_2, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"CO2_2", L"", pIniResultOfMI->wchCO2_2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"NO_2", L"", pIniResultOfMI->wchNO_2, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"O2_2", L"", pIniResultOfMI->wchO2_2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"Lambda_2", L"", pIniResultOfMI->wchLambda_2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"OilTemperature_2", L"", pIniResultOfMI->wchOilTemperature_2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"Rev_2", L"", pIniResultOfMI->wchRev_2, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"LimitOfHC", L"", pIniResultOfMI->wchLimitOfHC, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"LimitOfHC_EDP", L"", pIniResultOfMI->wchLimitOfHC_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"LimitOfCO", L"", pIniResultOfMI->wchLimitOfCO, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"LimitOfCO_EDP", L"", pIniResultOfMI->wchLimitOfCO_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"HCAverage", L"", pIniResultOfMI->wchHCAverage, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"COAverage", L"", pIniResultOfMI->wchCOAverage, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"LambdaAverage", L"", pIniResultOfMI->wchLambdaAverage, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"PassOfHC", L"", pIniResultOfMI->wchPassOfHC, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"PassOfCO", L"", pIniResultOfMI->wchPassOfCO, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"Pass", L"", pIniResultOfMI->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"EnvironmentalTemperature", L"", pIniResultOfMI->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"RelativeHumidity", L"", pIniResultOfMI->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"AtmosphericPressure", L"", pIniResultOfMI->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"SkipRev", L"", pIniResultOfMI->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"SkipOilTemperature", L"", pIniResultOfMI->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMI", L"Remark", L"", pIniResultOfMI->wchRemark, 1024);
	GetIniString(ini, L"ResultOfMI", L"Reserved1", L"", pIniResultOfMI->wchReserved1, 50);
	GetIniString(ini, L"ResultOfMI", L"Reserved2", L"", pIniResultOfMI->wchReserved2, 50);
	GetIniString(ini, L"ResultOfMI", L"Reserved3", L"", pIniResultOfMI->wchReserved3, 50);

}

void SetIniResultOfMI(const RESULTOFMI& IniResultOfMI)
{
	// 获取ResultOfAVFSU.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfMI.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfMI.ini信息
	WriteIniString(ini, L"ResultOfMI", L"AutoID", IniResultOfMI.wchAutoID);
	WriteIniString(ini, L"ResultOfMI", L"ReportNumber", IniResultOfMI.wchReportNumber);
	WriteIniString(ini, L"ResultOfMI", L"FuelTypeCode", IniResultOfMI.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfMI", L"FuelTypeName", IniResultOfMI.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfMI", L"CO_1", IniResultOfMI.wchCO_1);
	WriteIniString(ini, L"ResultOfMI", L"HC_1", IniResultOfMI.wchHC_1);
	WriteIniString(ini, L"ResultOfMI", L"CO2_1", IniResultOfMI.wchCO2_1);
	WriteIniString(ini, L"ResultOfMI", L"NO_1", IniResultOfMI.wchNO_1);
	WriteIniString(ini, L"ResultOfMI", L"O2_1", IniResultOfMI.wchO2_1);
	WriteIniString(ini, L"ResultOfMI", L"Lambda_1", IniResultOfMI.wchLambda_1);
	WriteIniString(ini, L"ResultOfMI", L"OilTemperature_1", IniResultOfMI.wchOilTemperature_1);
	WriteIniString(ini, L"ResultOfMI", L"Rev_1", IniResultOfMI.wchRev_1);
	WriteIniString(ini, L"ResultOfMI", L"CO_2", IniResultOfMI.wchCO_2);
	WriteIniString(ini, L"ResultOfMI", L"HC_2", IniResultOfMI.wchHC_2);
	WriteIniString(ini, L"ResultOfMI", L"CO2_2", IniResultOfMI.wchCO2_2);
	WriteIniString(ini, L"ResultOfMI", L"NO_2", IniResultOfMI.wchNO_2);
	WriteIniString(ini, L"ResultOfMI", L"O2_2", IniResultOfMI.wchO2_2);
	WriteIniString(ini, L"ResultOfMI", L"Lambda_2", IniResultOfMI.wchLambda_2);
	WriteIniString(ini, L"ResultOfMI", L"OilTemperature_2", IniResultOfMI.wchOilTemperature_2);
	WriteIniString(ini, L"ResultOfMI", L"Rev_2", IniResultOfMI.wchRev_2);
	WriteIniString(ini, L"ResultOfMI", L"LimitOfHC", IniResultOfMI.wchLimitOfHC);
	WriteIniString(ini, L"ResultOfMI", L"LimitOfHC_EDP", IniResultOfMI.wchLimitOfHC_EDP);
	WriteIniString(ini, L"ResultOfMI", L"LimitOfCO", IniResultOfMI.wchLimitOfCO);
	WriteIniString(ini, L"ResultOfMI", L"LimitOfCO_EDP", IniResultOfMI.wchLimitOfCO_EDP);
	WriteIniString(ini, L"ResultOfMI", L"HCAverage", IniResultOfMI.wchHCAverage);
	WriteIniString(ini, L"ResultOfMI", L"COAverage", IniResultOfMI.wchCOAverage);
	WriteIniString(ini, L"ResultOfMI", L"LambdaAverage", IniResultOfMI.wchLambdaAverage);
	WriteIniString(ini, L"ResultOfMI", L"PassOfHC", IniResultOfMI.wchPassOfHC);
	WriteIniString(ini, L"ResultOfMI", L"PassOfCO", IniResultOfMI.wchPassOfCO);
	WriteIniString(ini, L"ResultOfMI", L"Pass", IniResultOfMI.wchPass);
	WriteIniString(ini, L"ResultOfMI", L"EnvironmentalTemperature", IniResultOfMI.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfMI", L"RelativeHumidity", IniResultOfMI.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfMI", L"AtmosphericPressure", IniResultOfMI.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfMI", L"SkipRev", IniResultOfMI.wchSkipRev);
	WriteIniString(ini, L"ResultOfMI", L"SkipOilTemperature", IniResultOfMI.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfMI", L"Remark", IniResultOfMI.wchRemark);
	WriteIniString(ini, L"ResultOfMI", L"Reserved1", IniResultOfMI.wchReserved1);
	WriteIniString(ini, L"ResultOfMI", L"Reserved2", IniResultOfMI.wchReserved2);
	WriteIniString(ini, L"ResultOfMI", L"Reserved3", IniResultOfMI.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfMI(list<REALTIMEDATAOFMI>& listRealTimeDataOfMI)
{
	// 获取RealTimeDataOfMI.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfMI.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	REALTIMEDATAOFMI iniRealTimeDataOfMI;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfMI.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfMI.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfMI.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfMI.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfMI.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfMI.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfMI.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"State", L"", iniRealTimeDataOfMI.wchState, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfMI.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfMI.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfMI.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfMI.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfMI.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfMI.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfMI.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfMI.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfMI.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfMI.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfMI.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfMI.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfMI.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfMI.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfMI.wchReserved3, 50);

		listRealTimeDataOfMI.push_back(iniRealTimeDataOfMI);
	}
}


void GetIniResultOfMDIS(PRESULTOFMDIS const pIniResultOfMDIS)
{
	// 获取ResultOfMDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfMDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfMDIS.ini信息
	GetIniString(ini, L"ResultOfMDIS", L"AutoID", L"", pIniResultOfMDIS->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"ReportNumber", L"", pIniResultOfMDIS->wchReportNumber, 50);
	GetIniString(ini, L"ResultOfMDIS", L"FuelTypeCode", L"", pIniResultOfMDIS->wchFuelTypeCode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"FuelTypeName", L"", pIniResultOfMDIS->wchFuelTypeName, 50);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfHCHigh", L"", pIniResultOfMDIS->wchLimitOfHCHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfHCHigh_EDP", L"", pIniResultOfMDIS->wchLimitOfHCHigh_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfCOHigh", L"", pIniResultOfMDIS->wchLimitOfCOHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfCOHigh_EDP", L"", pIniResultOfMDIS->wchLimitOfCOHigh_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfHCLow", L"", pIniResultOfMDIS->wchLimitOfHCLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfHCLow_EDP", L"", pIniResultOfMDIS->wchLimitOfHCLow_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfCOLow", L"", pIniResultOfMDIS->wchLimitOfCOLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfCOLow_EDP", L"", pIniResultOfMDIS->wchLimitOfCOLow_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"SkipHISJudgement", L"", pIniResultOfMDIS->wchSkipHISJudgement, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"UpperLimitOfLambda", L"", pIniResultOfMDIS->wchUpperLimitOfLambda, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LowerLimitOfLambda", L"", pIniResultOfMDIS->wchLowerLimitOfLambda, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LimitOfLambda_EDP", L"", pIniResultOfMDIS->wchLimitOfLambda_EDP, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"RevHigh", L"", pIniResultOfMDIS->wchRevHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"OilTemperatureHigh", L"", pIniResultOfMDIS->wchOilTemperatureHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LambdaHigh", L"", pIniResultOfMDIS->wchLambdaHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"NOHigh", L"", pIniResultOfMDIS->wchNOHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"CO2High", L"", pIniResultOfMDIS->wchCO2High, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"CO2HighRound", L"", pIniResultOfMDIS->wchCO2HighRound, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"O2High", L"", pIniResultOfMDIS->wchO2High, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"HCHigh", L"", pIniResultOfMDIS->wchHCHigh, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"HCHighRound", L"", pIniResultOfMDIS->wchHCHighRound, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"COHigh", L"", pIniResultOfMDIS->wchCOHigh, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"COHighAmend", L"", pIniResultOfMDIS->wchCOHighAmend, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"COHighRound", L"", pIniResultOfMDIS->wchCOHighRound, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"PassOfHCHigh", L"", pIniResultOfMDIS->wchPassOfHCHigh, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"PassOfCOHigh", L"", pIniResultOfMDIS->wchPassOfCOHigh, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"NeedTestLambda", L"", pIniResultOfMDIS->wchNeedTestLambda, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"Lambda", L"", pIniResultOfMDIS->wchLambda, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"PassOfLambda", L"", pIniResultOfMDIS->wchPassOfLambda, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"RevLow", L"", pIniResultOfMDIS->wchRevLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"OilTemperatureLow", L"", pIniResultOfMDIS->wchOilTemperatureLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"LambdaLow", L"", pIniResultOfMDIS->wchLambdaLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"NOLow", L"", pIniResultOfMDIS->wchNOLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"CO2Low", L"", pIniResultOfMDIS->wchCO2Low, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"CO2LowRound", L"", pIniResultOfMDIS->wchCO2LowRound, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"O2Low", L"", pIniResultOfMDIS->wchO2Low, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"HCLow", L"", pIniResultOfMDIS->wchHCLow, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"HCLowRound", L"", pIniResultOfMDIS->wchHCLowRound, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"COLow", L"", pIniResultOfMDIS->wchCOLow, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"COLowAmend", L"", pIniResultOfMDIS->wchCOLowAmend, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"COLowRound", L"", pIniResultOfMDIS->wchCOLowRound, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"PassOfHCLow", L"", pIniResultOfMDIS->wchPassOfHCLow, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"PassOfCOLow", L"", pIniResultOfMDIS->wchPassOfCOLow, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"Pass", L"", pIniResultOfMDIS->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"EnvironmentalTemperature", L"", pIniResultOfMDIS->wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"RelativeHumidity", L"", pIniResultOfMDIS->wchRelativeHumidity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"AtmosphericPressure", L"", pIniResultOfMDIS->wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"SkipRev", L"", pIniResultOfMDIS->wchSkipRev, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"SkipOilTemperature", L"", pIniResultOfMDIS->wchSkipOilTemperature, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfMDIS", L"Remark", L"", pIniResultOfMDIS->wchRemark, 1024);
	GetIniString(ini, L"ResultOfMDIS", L"Reserved1", L"", pIniResultOfMDIS->wchReserved1, 50);
	GetIniString(ini, L"ResultOfMDIS", L"Reserved2", L"", pIniResultOfMDIS->wchReserved2, 50);
	GetIniString(ini, L"ResultOfMDIS", L"Reserved3", L"", pIniResultOfMDIS->wchReserved3, 50);

}

void SetIniResultOfMDIS(const RESULTOFMDIS& iniResultOfMDIS)
{
	// 获取ResultOfMDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfMDIS.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 获取ResultOfMDIS.ini信息
	WriteIniString(ini, L"ResultOfMDIS", L"AutoID", iniResultOfMDIS.wchAutoID);
	WriteIniString(ini, L"ResultOfMDIS", L"ReportNumber", iniResultOfMDIS.wchReportNumber);
	WriteIniString(ini, L"ResultOfMDIS", L"FuelTypeCode", iniResultOfMDIS.wchFuelTypeCode);
	WriteIniString(ini, L"ResultOfMDIS", L"FuelTypeName", iniResultOfMDIS.wchFuelTypeName);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfHCHigh", iniResultOfMDIS.wchLimitOfHCHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfHCHigh_EDP", iniResultOfMDIS.wchLimitOfHCHigh_EDP);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfCOHigh", iniResultOfMDIS.wchLimitOfCOHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfCOHigh_EDP", iniResultOfMDIS.wchLimitOfCOHigh_EDP);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfHCLow", iniResultOfMDIS.wchLimitOfHCLow);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfHCLow_EDP", iniResultOfMDIS.wchLimitOfHCLow_EDP);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfCOLow", iniResultOfMDIS.wchLimitOfCOLow);
	WriteIniString(ini, L"ResultOfMDIS", L"LimitOfCOLow_EDP", iniResultOfMDIS.wchLimitOfCOLow_EDP);
	WriteIniString(ini, L"ResultOfMDIS", L"SkipHISJudgement", iniResultOfMDIS.wchSkipHISJudgement);
	WriteIniString(ini, L"ResultOfMDIS", L"RevHigh", iniResultOfMDIS.wchRevHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"OilTemperatureHigh", iniResultOfMDIS.wchOilTemperatureHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"LambdaHigh", iniResultOfMDIS.wchLambdaHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"NOHigh", iniResultOfMDIS.wchNOHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"CO2High", iniResultOfMDIS.wchCO2High);
	WriteIniString(ini, L"ResultOfMDIS", L"CO2HighRound", iniResultOfMDIS.wchCO2HighRound);
	WriteIniString(ini, L"ResultOfMDIS", L"O2High", iniResultOfMDIS.wchO2High);
	WriteIniString(ini, L"ResultOfMDIS", L"HCHigh", iniResultOfMDIS.wchHCHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"HCHighRound", iniResultOfMDIS.wchHCHighRound);
	WriteIniString(ini, L"ResultOfMDIS", L"COHigh", iniResultOfMDIS.wchCOHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"COHighAmend", iniResultOfMDIS.wchCOHighAmend);
	WriteIniString(ini, L"ResultOfMDIS", L"COHighRound", iniResultOfMDIS.wchCOHighRound);
	WriteIniString(ini, L"ResultOfMDIS", L"PassOfHCHigh", iniResultOfMDIS.wchPassOfHCHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"PassOfCOHigh", iniResultOfMDIS.wchPassOfCOHigh);
	WriteIniString(ini, L"ResultOfMDIS", L"NeedTestLambda", iniResultOfMDIS.wchNeedTestLambda);
	WriteIniString(ini, L"ResultOfMDIS", L"Lambda", iniResultOfMDIS.wchLambda);
	WriteIniString(ini, L"ResultOfMDIS", L"PassOfLambda", iniResultOfMDIS.wchPassOfLambda);
	WriteIniString(ini, L"ResultOfMDIS", L"RevLow", iniResultOfMDIS.wchRevLow);
	WriteIniString(ini, L"ResultOfMDIS", L"OilTemperatureLow", iniResultOfMDIS.wchOilTemperatureLow);
	WriteIniString(ini, L"ResultOfMDIS", L"LambdaLow", iniResultOfMDIS.wchLambdaLow);
	WriteIniString(ini, L"ResultOfMDIS", L"NOLow", iniResultOfMDIS.wchNOLow);
	WriteIniString(ini, L"ResultOfMDIS", L"CO2Low", iniResultOfMDIS.wchCO2Low);
	WriteIniString(ini, L"ResultOfMDIS", L"CO2LowRound", iniResultOfMDIS.wchCO2LowRound);
	WriteIniString(ini, L"ResultOfMDIS", L"O2Low", iniResultOfMDIS.wchO2Low);
	WriteIniString(ini, L"ResultOfMDIS", L"HCLow", iniResultOfMDIS.wchHCLow);
	WriteIniString(ini, L"ResultOfMDIS", L"HCLowRound", iniResultOfMDIS.wchHCLowRound);
	WriteIniString(ini, L"ResultOfMDIS", L"COLow", iniResultOfMDIS.wchCOLow);
	WriteIniString(ini, L"ResultOfMDIS", L"COLowAmend", iniResultOfMDIS.wchCOLowAmend);
	WriteIniString(ini, L"ResultOfMDIS", L"COLowRound", iniResultOfMDIS.wchCOLowRound);
	WriteIniString(ini, L"ResultOfMDIS", L"PassOfHCLow", iniResultOfMDIS.wchPassOfHCLow);
	WriteIniString(ini, L"ResultOfMDIS", L"PassOfCOLow", iniResultOfMDIS.wchPassOfCOLow);
	WriteIniString(ini, L"ResultOfMDIS", L"Pass", iniResultOfMDIS.wchPass);
	WriteIniString(ini, L"ResultOfMDIS", L"EnvironmentalTemperature", iniResultOfMDIS.wchEnvironmentalTemperature);
	WriteIniString(ini, L"ResultOfMDIS", L"RelativeHumidity", iniResultOfMDIS.wchRelativeHumidity);
	WriteIniString(ini, L"ResultOfMDIS", L"AtmosphericPressure", iniResultOfMDIS.wchAtmosphericPressure);
	WriteIniString(ini, L"ResultOfMDIS", L"SkipRev", iniResultOfMDIS.wchSkipRev);
	WriteIniString(ini, L"ResultOfMDIS", L"SkipOilTemperature", iniResultOfMDIS.wchSkipOilTemperature);
	WriteIniString(ini, L"ResultOfMDIS", L"Remark", iniResultOfMDIS.wchRemark);
	WriteIniString(ini, L"ResultOfMDIS", L"Reserved1", iniResultOfMDIS.wchReserved1);
	WriteIniString(ini, L"ResultOfMDIS", L"Reserved2", iniResultOfMDIS.wchReserved2);
	WriteIniString(ini, L"ResultOfMDIS", L"Reserved3", iniResultOfMDIS.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfMDIS(list<REALTIMEDATAOFMDIS>& listRealTimeDataOfMDIS)
{
	// 获取RealTimeDataOfMDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfMDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	REALTIMEDATAOFMDIS iniRealTimeDataOfMDIS;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfMDIS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfMDIS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfMDIS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfMDIS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfMDIS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfMDIS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfMDIS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfMDIS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfMDIS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfMDIS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfMDIS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfMDIS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfMDIS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfMDIS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfMDIS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfMDIS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfMDIS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfMDIS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfMDIS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfMDIS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfMDIS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfMDIS.wchReserved3, 50);

		listRealTimeDataOfMDIS.push_back(iniRealTimeDataOfMDIS);
	}
}

void SetIniDemarcationLog(const DEMARCATIONLOG& iniDemarcationLog)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 设置DemarcationLog.ini信息
	WriteIniString(ini, L"DemarcationLog", L"AutoID", iniDemarcationLog.wchAutoID);
	WriteIniString(ini, L"DemarcationLog", L"DemarcationNumber", iniDemarcationLog.wchDemarcationNumber);
	WriteIniString(ini, L"DemarcationLog", L"LineNumber", iniDemarcationLog.wchLineNumber);
	WriteIniString(ini, L"DemarcationLog", L"Operator", iniDemarcationLog.wchOperator);
	WriteIniString(ini, L"DemarcationLog", L"EquipmentNumber", iniDemarcationLog.wchEquipmentNumber);
	WriteIniString(ini, L"DemarcationLog", L"EquipmentModel", iniDemarcationLog.wchEquipmentModel);
	WriteIniString(ini, L"DemarcationLog", L"EquipmentName", iniDemarcationLog.wchEquipmentName);
	WriteIniString(ini, L"DemarcationLog", L"DemarcationItem", iniDemarcationLog.wchDemarcationItem);
	WriteIniString(ini, L"DemarcationLog", L"DemarcationDate", iniDemarcationLog.wchDemarcationDate);
	WriteIniString(ini, L"DemarcationLog", L"Pass", iniDemarcationLog.wchPass);
	WriteIniString(ini, L"DemarcationLog", L"Remark", iniDemarcationLog.wchRemark);
	WriteIniString(ini, L"DemarcationLog", L"Reserved1", iniDemarcationLog.wchReserved1);
	WriteIniString(ini, L"DemarcationLog", L"Reserved2", iniDemarcationLog.wchReserved2);
	WriteIniString(ini, L"DemarcationLog", L"Reserved3", iniDemarcationLog.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniDemarcationLog(PDEMARCATIONLOG const pIniDemarcationLog)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取DemarcationLog.ini信息
	GetIniString(ini, L"DemarcationLog", L"AutoID", L"", pIniDemarcationLog->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"DemarcationLog", L"DemarcationNumber", L"", pIniDemarcationLog->wchDemarcationNumber, 50);
	GetIniString(ini, L"DemarcationLog", L"LineNumber", L"", pIniDemarcationLog->wchLineNumber, 50);
	GetIniString(ini, L"DemarcationLog", L"Operator", L"", pIniDemarcationLog->wchOperator, 50);
	GetIniString(ini, L"DemarcationLog", L"EquipmentNumber", L"", pIniDemarcationLog->wchEquipmentNumber, 50);
	GetIniString(ini, L"DemarcationLog", L"EquipmentModel", L"", pIniDemarcationLog->wchEquipmentModel, 50);
	GetIniString(ini, L"DemarcationLog", L"EquipmentName", L"", pIniDemarcationLog->wchEquipmentName, 50);
	GetIniString(ini, L"DemarcationLog", L"DemarcationItem", L"", pIniDemarcationLog->wchDemarcationItem, NH_INT_TO_WCHART);
	GetIniString(ini, L"DemarcationLog", L"DemarcationDate", L"", pIniDemarcationLog->wchDemarcationDate, NH_TIME_TO_WCHART);
	GetIniString(ini, L"DemarcationLog", L"Pass", L"", pIniDemarcationLog->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"DemarcationLog", L"Remark", L"", pIniDemarcationLog->wchRemark, 1024);
	GetIniString(ini, L"DemarcationLog", L"Reserved1", L"", pIniDemarcationLog->wchReserved1, 50);
	GetIniString(ini, L"DemarcationLog", L"Reserved2", L"", pIniDemarcationLog->wchReserved2, 50);
	GetIniString(ini, L"DemarcationLog", L"Reserved3", L"", pIniDemarcationLog->wchReserved3, 50);

}

void GetIniResultOfAnaGasCal(PDEMARCATIONRESULTOFANAGASCAL const pIniResultOfAnaGasCal)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasCal.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfAnaGasCal.ini信息
	GetIniString(ini, L"ResultOfAnaGasCal", L"AutoID", L"", pIniResultOfAnaGasCal->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"DemarcationNumber", L"", pIniResultOfAnaGasCal->wchDemarcationNumber, 50);

	GetIniString(ini, L"ResultOfAnaGasCal", L"PEFMeasuredValue", L"", pIniResultOfAnaGasCal->wchPEFMeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"SkipHCCalibration", L"", pIniResultOfAnaGasCal->wchSkipHCCalibration, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"C3H8StandardValue", L"", pIniResultOfAnaGasCal->wchC3H8StandardValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"HCMeasuredValue", L"", pIniResultOfAnaGasCal->wchHCMeasuredValue, NH_INT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"SkipCOCalibration", L"", pIniResultOfAnaGasCal->wchSkipCOCalibration, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"COMeasuredValue", L"", pIniResultOfAnaGasCal->wchCOMeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"COStandardValue", L"", pIniResultOfAnaGasCal->wchCOStandardValue, NH_FLOAT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"SkipCO2Calibration", L"", pIniResultOfAnaGasCal->wchSkipCO2Calibration, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"CO2MeasuredValue", L"", pIniResultOfAnaGasCal->wchCO2MeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"CO2StandardValue", L"", pIniResultOfAnaGasCal->wchCO2StandardValue, NH_FLOAT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"SkipNOCalibration", L"", pIniResultOfAnaGasCal->wchSkipNOCalibration, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"NOMeasuredValue", L"", pIniResultOfAnaGasCal->wchNOMeasuredValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"NOStandardValue", L"", pIniResultOfAnaGasCal->wchNOStandardValue, NH_INT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"SkipNO2Calibration", L"", pIniResultOfAnaGasCal->wchSkipNO2Calibration, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"NO2MeasuredValue", L"", pIniResultOfAnaGasCal->wchNO2MeasuredValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"NO2StandardValue", L"", pIniResultOfAnaGasCal->wchNO2StandardValue, NH_INT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"SkipO2Calibration", L"", pIniResultOfAnaGasCal->wchSkipO2Calibration, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"O2MeasuredValue", L"", pIniResultOfAnaGasCal->wchO2MeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasCal", L"O2StandardValue", L"", pIniResultOfAnaGasCal->wchO2StandardValue, NH_FLOAT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"Pass", L"", pIniResultOfAnaGasCal->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasCal", L"Remark", L"", pIniResultOfAnaGasCal->wchRemark, 1024);
	GetIniString(ini, L"ResultOfAnaGasCal", L"Reserved1", L"", pIniResultOfAnaGasCal->wchReserved1, 50);
	GetIniString(ini, L"ResultOfAnaGasCal", L"Reserved2", L"", pIniResultOfAnaGasCal->wchReserved2, 50);
	GetIniString(ini, L"ResultOfAnaGasCal", L"Reserved3", L"", pIniResultOfAnaGasCal->wchReserved3, 50);

}

void GetIniResultOfAnaGasChk(PDEMARCATIONRESULTOFANAGASCHK const pIniResultOfAnaGasChk, wchar_t *pwchSaveFileName/*=L"ResultOfAnaGasChk.ini"*/)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfAnaGasChk.ini信息
	GetIniString(ini, L"ResultOfAnaGasChk", L"AutoID", L"", pIniResultOfAnaGasChk->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"DemarcationNumber", L"", pIniResultOfAnaGasChk->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"", pIniResultOfAnaGasChk->wchPEFMeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"C3H8StandardValue", L"", pIniResultOfAnaGasChk->wchC3H8StandardValue, NH_INT_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"SkipHCJudgement", L"", pIniResultOfAnaGasChk->wchSkipHCJudgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCMeasuredValue", L"", pIniResultOfAnaGasChk->wchHCMeasuredValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCStandardValue", L"", pIniResultOfAnaGasChk->wchHCStandardValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCAE", L"", pIniResultOfAnaGasChk->wchHCAE, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCAELimit", L"", pIniResultOfAnaGasChk->wchHCAELimit, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCAEPass", L"", pIniResultOfAnaGasChk->wchHCAEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCRE", L"", pIniResultOfAnaGasChk->wchHCRE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCRELimit", L"", pIniResultOfAnaGasChk->wchHCRELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCREPass", L"", pIniResultOfAnaGasChk->wchHCREPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"HCPass", L"", pIniResultOfAnaGasChk->wchHCPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"SkipCOJudgement", L"", pIniResultOfAnaGasChk->wchSkipCOJudgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COMeasuredValue", L"", pIniResultOfAnaGasChk->wchCOMeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COStandardValue", L"", pIniResultOfAnaGasChk->wchCOStandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COAE", L"", pIniResultOfAnaGasChk->wchCOAE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COAELimit", L"", pIniResultOfAnaGasChk->wchCOAELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COAEPass", L"", pIniResultOfAnaGasChk->wchCOAEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CORE", L"", pIniResultOfAnaGasChk->wchCORE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CORELimit", L"", pIniResultOfAnaGasChk->wchCORELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COREPass", L"", pIniResultOfAnaGasChk->wchCOREPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"COPass", L"", pIniResultOfAnaGasChk->wchCOPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"SkipCO2Judgement", L"", pIniResultOfAnaGasChk->wchSkipCO2Judgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"", pIniResultOfAnaGasChk->wchCO2MeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2StandardValue", L"", pIniResultOfAnaGasChk->wchCO2StandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2AE", L"", pIniResultOfAnaGasChk->wchCO2AE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2AELimit", L"", pIniResultOfAnaGasChk->wchCO2AELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2AEPass", L"", pIniResultOfAnaGasChk->wchCO2AEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2RE", L"", pIniResultOfAnaGasChk->wchCO2RE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2RELimit", L"", pIniResultOfAnaGasChk->wchCO2RELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2REPass", L"", pIniResultOfAnaGasChk->wchCO2REPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"CO2Pass", L"", pIniResultOfAnaGasChk->wchCO2Pass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"SkipNOJudgement", L"", pIniResultOfAnaGasChk->wchSkipNOJudgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOMeasuredValue", L"", pIniResultOfAnaGasChk->wchNOMeasuredValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOStandardValue", L"", pIniResultOfAnaGasChk->wchNOStandardValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOAE", L"", pIniResultOfAnaGasChk->wchNOAE, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOAELimit", L"", pIniResultOfAnaGasChk->wchNOAELimit, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOAEPass", L"", pIniResultOfAnaGasChk->wchNOAEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NORE", L"", pIniResultOfAnaGasChk->wchNORE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NORELimit", L"", pIniResultOfAnaGasChk->wchNORELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOREPass", L"", pIniResultOfAnaGasChk->wchNOREPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NOPass", L"", pIniResultOfAnaGasChk->wchNOPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"SkipNO2Judgement", L"", pIniResultOfAnaGasChk->wchSkipNO2Judgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"", pIniResultOfAnaGasChk->wchNO2MeasuredValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2StandardValue", L"", pIniResultOfAnaGasChk->wchNO2StandardValue, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2AE", L"", pIniResultOfAnaGasChk->wchNO2AE, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2AELimit", L"", pIniResultOfAnaGasChk->wchNO2AELimit, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2AEPass", L"", pIniResultOfAnaGasChk->wchNO2AEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2RE", L"", pIniResultOfAnaGasChk->wchNO2RE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2RELimit", L"", pIniResultOfAnaGasChk->wchNO2RELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2REPass", L"", pIniResultOfAnaGasChk->wchNO2REPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"NO2Pass", L"", pIniResultOfAnaGasChk->wchNO2Pass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"SkipO2Judgement", L"", pIniResultOfAnaGasChk->wchSkipO2Judgement, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2MeasuredValue", L"", pIniResultOfAnaGasChk->wchO2MeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2StandardValue", L"", pIniResultOfAnaGasChk->wchO2StandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2AE", L"", pIniResultOfAnaGasChk->wchO2AE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2AELimit", L"", pIniResultOfAnaGasChk->wchO2AELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2AEPass", L"", pIniResultOfAnaGasChk->wchO2AEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2RE", L"", pIniResultOfAnaGasChk->wchO2RE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2RELimit", L"", pIniResultOfAnaGasChk->wchO2RELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfAnaGasChk", L"O2REPass", L"", pIniResultOfAnaGasChk->wchO2REPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"Pass", L"", pIniResultOfAnaGasChk->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfAnaGasChk", L"Remark", L"", pIniResultOfAnaGasChk->wchRemark, 1024);
	GetIniString(ini, L"ResultOfAnaGasChk", L"Reserved1", L"", pIniResultOfAnaGasChk->wchReserved1, 50);
	GetIniString(ini, L"ResultOfAnaGasChk", L"Reserved2", L"", pIniResultOfAnaGasChk->wchReserved2, 50);
	GetIniString(ini, L"ResultOfAnaGasChk", L"Reserved3", L"", pIniResultOfAnaGasChk->wchReserved3, 50);

}

void GetIniRealTimeDataOfAnaGasChk(list<DEMARCATIONREALTIMEDATAOFANAGASCHK>& listRealTimeDataOfAnaGasChk)
{
	// 获取DemarcationRealTimeDataOfAnaGasChk.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationRealTimeDataOfAnaGasChk.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	DemarcationRealTimeDataOfAnaGasChk iniDemarcationRealTimeDataOfAnaGasChk;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取DemarcationRealTimeDataOfAnaGasChk.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"DemarcationNumber", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchDemarcationNumber, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"HC", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PEF", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchPEF, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Flux", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchFlux, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"Rev", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniDemarcationRealTimeDataOfAnaGasChk.wchReserved3, 50);

		listRealTimeDataOfAnaGasChk.push_back(iniDemarcationRealTimeDataOfAnaGasChk);
	}
}

void GetIniResultOfOpaCalChk(PDEMARCATIONRESULTOFOPACALCHK const pIniResultOfOpaCalChk)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOpaCalChk.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfOpaCalChk.ini信息
	GetIniString(ini, L"ResultOfOpaCalChk", L"AutoID", L"", pIniResultOfOpaCalChk->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"DemarcationNumber", L"", pIniResultOfOpaCalChk->wchDemarcationNumber, 50);

	GetIniString(ini, L"ResultOfOpaCalChk", L"MeasuredValue1", L"", pIniResultOfOpaCalChk->wchMeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"StandardValue1", L"", pIniResultOfOpaCalChk->wchStandardValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"AE1", L"", pIniResultOfOpaCalChk->wchAE1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"AELimit1", L"", pIniResultOfOpaCalChk->wchAELimit1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"AEPass1", L"", pIniResultOfOpaCalChk->wchAEPass1, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"RE1", L"", pIniResultOfOpaCalChk->wchRE1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"RELimit1", L"", pIniResultOfOpaCalChk->wchRELimit1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"REPass1", L"", pIniResultOfOpaCalChk->wchREPass1, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Linearity1", L"", pIniResultOfOpaCalChk->wchLinearity1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"LinearityLimit1", L"", pIniResultOfOpaCalChk->wchLinearityLimit1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"LinearityPass1", L"", pIniResultOfOpaCalChk->wchLinearityPass1, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Pass1", L"", pIniResultOfOpaCalChk->wchPass1, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfOpaCalChk", L"MeasuredValue2", L"", pIniResultOfOpaCalChk->wchMeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"StandardValue2", L"", pIniResultOfOpaCalChk->wchStandardValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"AE2", L"", pIniResultOfOpaCalChk->wchAE2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"AELimit2", L"", pIniResultOfOpaCalChk->wchAELimit2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"AEPass2", L"", pIniResultOfOpaCalChk->wchAEPass2, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"RE2", L"", pIniResultOfOpaCalChk->wchRE2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"RELimit2", L"", pIniResultOfOpaCalChk->wchRELimit2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"REPass2", L"", pIniResultOfOpaCalChk->wchREPass2, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Linearity2", L"", pIniResultOfOpaCalChk->wchLinearity2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"LinearityLimit2", L"", pIniResultOfOpaCalChk->wchLinearityLimit2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"LinearityPass2", L"", pIniResultOfOpaCalChk->wchLinearityPass2, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Pass2", L"", pIniResultOfOpaCalChk->wchPass2, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfOpaCalChk", L"Pass", L"", pIniResultOfOpaCalChk->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfOpaCalChk", L"Remark", L"", pIniResultOfOpaCalChk->wchRemark, 1024);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Reserved1", L"", pIniResultOfOpaCalChk->wchReserved1, 50);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Reserved2", L"", pIniResultOfOpaCalChk->wchReserved2, 50);
	GetIniString(ini, L"ResultOfOpaCalChk", L"Reserved3", L"", pIniResultOfOpaCalChk->wchReserved3, 50);

}

void GetIniResultOfSmoChk(PDEMARCATIONRESULTOFSMOCHK const pIniResultOfSmoChk)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfSmoChk.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfSmoChk.ini信息
	GetIniString(ini, L"ResultOfSmoChk", L"AutoID", L"", pIniResultOfSmoChk->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"DemarcationNumber", L"", pIniResultOfSmoChk->wchDemarcationNumber, 50);

	GetIniString(ini, L"ResultOfSmoChk", L"RepeatStandardValue", L"", pIniResultOfSmoChk->wchRepeatStandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatMeasuredValue1", L"", pIniResultOfSmoChk->wchRepeatMeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatMeasuredValue2", L"", pIniResultOfSmoChk->wchRepeatMeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatMeasuredValue3", L"", pIniResultOfSmoChk->wchRepeatMeasuredValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatMeasuredValue4", L"", pIniResultOfSmoChk->wchRepeatMeasuredValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatMeasuredValue5", L"", pIniResultOfSmoChk->wchRepeatMeasuredValue5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatErrorValue1", L"", pIniResultOfSmoChk->wchRepeatErrorValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatErrorValue2", L"", pIniResultOfSmoChk->wchRepeatErrorValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatErrorValue3", L"", pIniResultOfSmoChk->wchRepeatErrorValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatErrorValue4", L"", pIniResultOfSmoChk->wchRepeatErrorValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatErrorValue5", L"", pIniResultOfSmoChk->wchRepeatErrorValue5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatAverageValue", L"", pIniResultOfSmoChk->wchRepeatAverageValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"RepeatPass", L"", pIniResultOfSmoChk->wchRepeatPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfSmoChk", L"BasicError1StandardValue", L"", pIniResultOfSmoChk->wchBasicError1StandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError1MeasuredValue1", L"", pIniResultOfSmoChk->wchBasicError1MeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError1MeasuredValue2", L"", pIniResultOfSmoChk->wchBasicError1MeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError1MeasuredValue3", L"", pIniResultOfSmoChk->wchBasicError1MeasuredValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError1AverageValue", L"", pIniResultOfSmoChk->wchBasicError1AverageValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError2StandardValue", L"", pIniResultOfSmoChk->wchBasicError2StandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError2MeasuredValue1", L"", pIniResultOfSmoChk->wchBasicError2MeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError2MeasuredValue2", L"", pIniResultOfSmoChk->wchBasicError2MeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError2MeasuredValue3", L"", pIniResultOfSmoChk->wchBasicError2MeasuredValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError2AverageValue", L"", pIniResultOfSmoChk->wchBasicError2AverageValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicErrorFStandardValue", L"", pIniResultOfSmoChk->wchBasicErrorFStandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicErrorFMeasuredValue1", L"", pIniResultOfSmoChk->wchBasicErrorFMeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicErrorFMeasuredValue2", L"", pIniResultOfSmoChk->wchBasicErrorFMeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicErrorFMeasuredValue3", L"", pIniResultOfSmoChk->wchBasicErrorFMeasuredValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicErrorFAverageValue", L"", pIniResultOfSmoChk->wchBasicErrorFAverageValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError5StandardValue", L"", pIniResultOfSmoChk->wchBasicError5StandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError5MeasuredValue1", L"", pIniResultOfSmoChk->wchBasicError5MeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError5MeasuredValue2", L"", pIniResultOfSmoChk->wchBasicError5MeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError5MeasuredValue3", L"", pIniResultOfSmoChk->wchBasicError5MeasuredValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError5AverageValue", L"", pIniResultOfSmoChk->wchBasicError5AverageValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError6StandardValue", L"", pIniResultOfSmoChk->wchBasicError6StandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError6MeasuredValue1", L"", pIniResultOfSmoChk->wchBasicError6MeasuredValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError6MeasuredValue2", L"", pIniResultOfSmoChk->wchBasicError6MeasuredValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError6MeasuredValue3", L"", pIniResultOfSmoChk->wchBasicError6MeasuredValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicError6AverageValue", L"", pIniResultOfSmoChk->wchBasicError6AverageValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfSmoChk", L"BasicErrorPass", L"", pIniResultOfSmoChk->wchBasicErrorPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfSmoChk", L"Pass", L"", pIniResultOfSmoChk->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfSmoChk", L"Remark", L"", pIniResultOfSmoChk->wchRemark, 1024);
	GetIniString(ini, L"ResultOfSmoChk", L"Reserved1", L"", pIniResultOfSmoChk->wchReserved1, 50);
	GetIniString(ini, L"ResultOfSmoChk", L"Reserved2", L"", pIniResultOfSmoChk->wchReserved2, 50);
	GetIniString(ini, L"ResultOfSmoChk", L"Reserved3", L"", pIniResultOfSmoChk->wchReserved3, 50);

}

// 测功机加载滑行结果<DemarcationResultOfDynConstLoad>
void GetIniResultOfDynConstLoad(PDEMARCATIONRESULTOFDYNCONSTLOAD const pIniResultOfDynConstLoad)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynConstLoad.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfDynConstLoad.ini信息
	GetIniString(ini, L"ResultOfDynConstLoad", L"AutoID", L"", pIniResultOfDynConstLoad->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"DemarcationNumber", L"", pIniResultOfDynConstLoad->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfDynConstLoad", L"DIW", L"", pIniResultOfDynConstLoad->wchDIW, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"LowerVelocity", L"", pIniResultOfDynConstLoad->wchLowerVelocity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"UpperVelocity", L"", pIniResultOfDynConstLoad->wchUpperVelocity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"NomialVelocity", L"", pIniResultOfDynConstLoad->wchNomialVelocity, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"JudgeByTime", L"", pIniResultOfDynConstLoad->wchJudgeByTime, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerMeasuredValue", L"", pIniResultOfDynConstLoad->wchPowerMeasuredValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerStandardValue", L"", pIniResultOfDynConstLoad->wchPowerStandardValue, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerAE", L"", pIniResultOfDynConstLoad->wchPowerAE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerAELimit", L"", pIniResultOfDynConstLoad->wchPowerAELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerAEPass", L"", pIniResultOfDynConstLoad->wchPowerAEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerRE", L"", pIniResultOfDynConstLoad->wchPowerRE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerRELimit", L"", pIniResultOfDynConstLoad->wchPowerRELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerREPass", L"", pIniResultOfDynConstLoad->wchPowerREPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"PowerPass", L"", pIniResultOfDynConstLoad->wchPowerPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"ACDT", L"", pIniResultOfDynConstLoad->wchACDT, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CCDT", L"", pIniResultOfDynConstLoad->wchCCDT, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTAE", L"", pIniResultOfDynConstLoad->wchCDTAE, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTAELimit", L"", pIniResultOfDynConstLoad->wchCDTAELimit, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTAEPass", L"", pIniResultOfDynConstLoad->wchCDTAEPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTRE", L"", pIniResultOfDynConstLoad->wchCDTRE, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTRELimit", L"", pIniResultOfDynConstLoad->wchCDTRELimit, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTREPass", L"", pIniResultOfDynConstLoad->wchCDTREPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"CDTPass", L"", pIniResultOfDynConstLoad->wchCDTPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"Pass", L"", pIniResultOfDynConstLoad->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynConstLoad", L"Remark", L"", pIniResultOfDynConstLoad->wchRemark, 1024);
	GetIniString(ini, L"ResultOfDynConstLoad", L"Reserved1", L"", pIniResultOfDynConstLoad->wchReserved1, 50);
	GetIniString(ini, L"ResultOfDynConstLoad", L"Reserved2", L"", pIniResultOfDynConstLoad->wchReserved2, 50);
	GetIniString(ini, L"ResultOfDynConstLoad", L"Reserved3", L"", pIniResultOfDynConstLoad->wchReserved3, 50);

}

// 测功机加载滑行实时数据<DemarcationRealTimeDataOfDynConstLoad>
void GetIniRealTimeDataOfDynConstLoad(list<DEMARCATIONREALTIMEDATAOFDYNCONSTLOAD>& listRealTimeDataOfDynConstLoad)
{
	// 获取DemarcationRealTimeDataOfDynConstLoad.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationRealTimeDataOfDynConstLoad.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);	
	if(nTotal <= 0)
	{
		return;
	}

	DemarcationRealTimeDataOfDynConstLoad iniDemarcationRealTimeDataOfDynConstLoad;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取DemarcationRealTimeDataOfAnaGasChk.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"DemarcationNumber", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchDemarcationNumber, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"Velocity", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniDemarcationRealTimeDataOfDynConstLoad.wchReserved3, 50);

		listRealTimeDataOfDynConstLoad.push_back(iniDemarcationRealTimeDataOfDynConstLoad);
	}
}

// 测功机力校准结果<DemarcationResultOfDynForceCal>
void GetIniResultOfDynForceCal(PDEMARCATIONRESULTOFDYNFORCECAL const pIniResultOfDynForceCal)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceCal.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfDynForceCal.ini信息
	GetIniString(ini, L"ResultOfDynForceCal", L"AutoID", L"", pIniResultOfDynForceCal->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceCal", L"DemarcationNumber", L"", pIniResultOfDynForceCal->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfDynForceCal", L"Pass", L"", pIniResultOfDynForceCal->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfDynForceCal", L"Remark", L"", pIniResultOfDynForceCal->wchRemark, 1024);
	GetIniString(ini, L"ResultOfDynForceCal", L"Reserved1", L"", pIniResultOfDynForceCal->wchReserved1, 50);
	GetIniString(ini, L"ResultOfDynForceCal", L"Reserved2", L"", pIniResultOfDynForceCal->wchReserved2, 50);
	GetIniString(ini, L"ResultOfDynForceCal", L"Reserved3", L"", pIniResultOfDynForceCal->wchReserved3, 50);
}

// 测功机力检查结果<DemarcationResultOfDynForceChk>
void GetIniResultOfDynForceChk(PDEMARCATIONRESULTOFDYNFORCECHK const pIniResultOfDynForceChk)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceChk.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfDynForceChk.ini信息
	GetIniString(ini, L"ResultOfDynForceChk", L"AutoID", L"", pIniResultOfDynForceChk->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"DemarcationNumber", L"", pIniResultOfDynForceChk->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfDynForceChk", L"WeightCount", L"", pIniResultOfDynForceChk->wchWeightCount, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue1", L"", pIniResultOfDynForceChk->wchStdValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue1", L"", pIniResultOfDynForceChk->wchForwardAvgValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue1", L"", pIniResultOfDynForceChk->wchBackwardAvgValue1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE1", L"", pIniResultOfDynForceChk->wchAE1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE1", L"", pIniResultOfDynForceChk->wchRE1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue2", L"", pIniResultOfDynForceChk->wchStdValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue2", L"", pIniResultOfDynForceChk->wchForwardAvgValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue2", L"", pIniResultOfDynForceChk->wchBackwardAvgValue2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE2", L"", pIniResultOfDynForceChk->wchAE2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE2", L"", pIniResultOfDynForceChk->wchRE2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue3", L"", pIniResultOfDynForceChk->wchStdValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue3", L"", pIniResultOfDynForceChk->wchForwardAvgValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue3", L"", pIniResultOfDynForceChk->wchBackwardAvgValue3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE3", L"", pIniResultOfDynForceChk->wchAE3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE3", L"", pIniResultOfDynForceChk->wchRE3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue4", L"", pIniResultOfDynForceChk->wchStdValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue4", L"", pIniResultOfDynForceChk->wchForwardAvgValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue4", L"", pIniResultOfDynForceChk->wchBackwardAvgValue4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE4", L"", pIniResultOfDynForceChk->wchAE4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE4", L"", pIniResultOfDynForceChk->wchRE4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue5", L"", pIniResultOfDynForceChk->wchStdValue5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue5", L"", pIniResultOfDynForceChk->wchForwardAvgValue5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue5", L"", pIniResultOfDynForceChk->wchBackwardAvgValue5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE5", L"", pIniResultOfDynForceChk->wchAE5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE5", L"", pIniResultOfDynForceChk->wchRE5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue6", L"", pIniResultOfDynForceChk->wchStdValue6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue6", L"", pIniResultOfDynForceChk->wchForwardAvgValue6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue6", L"", pIniResultOfDynForceChk->wchBackwardAvgValue6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE6", L"", pIniResultOfDynForceChk->wchAE6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE6", L"", pIniResultOfDynForceChk->wchRE6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue7", L"", pIniResultOfDynForceChk->wchStdValue7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue7", L"", pIniResultOfDynForceChk->wchForwardAvgValue7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue7", L"", pIniResultOfDynForceChk->wchBackwardAvgValue7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE7", L"", pIniResultOfDynForceChk->wchAE7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE7", L"", pIniResultOfDynForceChk->wchRE7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue8", L"", pIniResultOfDynForceChk->wchStdValue8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue8", L"", pIniResultOfDynForceChk->wchForwardAvgValue8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue8", L"", pIniResultOfDynForceChk->wchBackwardAvgValue8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE8", L"", pIniResultOfDynForceChk->wchAE8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE8", L"", pIniResultOfDynForceChk->wchRE8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue9", L"", pIniResultOfDynForceChk->wchStdValue9, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue9", L"", pIniResultOfDynForceChk->wchForwardAvgValue9, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue9", L"", pIniResultOfDynForceChk->wchBackwardAvgValue9, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE9", L"", pIniResultOfDynForceChk->wchAE9, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE9", L"", pIniResultOfDynForceChk->wchRE9, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"StdValue10", L"", pIniResultOfDynForceChk->wchStdValue10, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"ForwardAvgValue10", L"", pIniResultOfDynForceChk->wchForwardAvgValue10, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"BackwardAvgValue10", L"", pIniResultOfDynForceChk->wchBackwardAvgValue10, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"AE10", L"", pIniResultOfDynForceChk->wchAE10, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"RE10", L"", pIniResultOfDynForceChk->wchRE10, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"Pass", L"", pIniResultOfDynForceChk->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynForceChk", L"Remark", L"", pIniResultOfDynForceChk->wchRemark, 1024);
	GetIniString(ini, L"ResultOfDynForceChk", L"Reserved1", L"", pIniResultOfDynForceChk->wchReserved1, 50);
	GetIniString(ini, L"ResultOfDynForceChk", L"Reserved2", L"", pIniResultOfDynForceChk->wchReserved2, 50);
	GetIniString(ini, L"ResultOfDynForceChk", L"Reserved3", L"", pIniResultOfDynForceChk->wchReserved3, 50);

}

// 测功机寄生功率滑行结果<DemarcationResultOfDynPLHP>
void GetIniResultOfDynPLHP(PDEMARCATIONRESULTOFDYNPLHP const pIniResultOfDynPLHP)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfDynPLHP.ini信息
	GetIniString(ini, L"ResultOfDynPLHP", L"AutoID", L"", pIniResultOfDynPLHP->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"DemarcationNumber", L"", pIniResultOfDynPLHP->wchDemarcationNumber, 50);

	GetIniString(ini, L"ResultOfDynPLHP", L"DIW", L"", pIniResultOfDynPLHP->wchDIW, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPMode", L"", pIniResultOfDynPLHP->wchPLHPMode, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPType", L"", pIniResultOfDynPLHP->wchPLHPType, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"ApWithRemanence", L"", pIniResultOfDynPLHP->wchApWithRemanence, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"BpWithRemanence", L"", pIniResultOfDynPLHP->wchBpWithRemanence, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"CpWithRemanence", L"", pIniResultOfDynPLHP->wchCpWithRemanence, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"ApWithoutRemanence", L"", pIniResultOfDynPLHP->wchApWithoutRemanence, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"BpWithoutRemanence", L"", pIniResultOfDynPLHP->wchBpWithoutRemanence, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"CpWithoutRemanence", L"", pIniResultOfDynPLHP->wchCpWithoutRemanence, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity0", L"", pIniResultOfDynPLHP->wchLowerVelocity0, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity0", L"", pIniResultOfDynPLHP->wchUpperVelocity0, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity0", L"", pIniResultOfDynPLHP->wchNomialVelocity0, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime0", L"", pIniResultOfDynPLHP->wchPLHPTime0, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP0", L"", pIniResultOfDynPLHP->wchPLHP0, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce0", L"", pIniResultOfDynPLHP->wchRemanenceByForce0, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower0", L"", pIniResultOfDynPLHP->wchRemanenceByPower0, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity1", L"", pIniResultOfDynPLHP->wchLowerVelocity1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity1", L"", pIniResultOfDynPLHP->wchUpperVelocity1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity1", L"", pIniResultOfDynPLHP->wchNomialVelocity1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime1", L"", pIniResultOfDynPLHP->wchPLHPTime1, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP1", L"", pIniResultOfDynPLHP->wchPLHP1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce1", L"", pIniResultOfDynPLHP->wchRemanenceByForce1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower1", L"", pIniResultOfDynPLHP->wchRemanenceByPower1, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity2", L"", pIniResultOfDynPLHP->wchLowerVelocity2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity2", L"", pIniResultOfDynPLHP->wchUpperVelocity2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity2", L"", pIniResultOfDynPLHP->wchNomialVelocity2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime2", L"", pIniResultOfDynPLHP->wchPLHPTime2, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP2", L"", pIniResultOfDynPLHP->wchPLHP2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce2", L"", pIniResultOfDynPLHP->wchRemanenceByForce2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower2", L"", pIniResultOfDynPLHP->wchRemanenceByPower2, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity3", L"", pIniResultOfDynPLHP->wchLowerVelocity3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity3", L"", pIniResultOfDynPLHP->wchUpperVelocity3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity3", L"", pIniResultOfDynPLHP->wchNomialVelocity3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime3", L"", pIniResultOfDynPLHP->wchPLHPTime3, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP3", L"", pIniResultOfDynPLHP->wchPLHP3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce3", L"", pIniResultOfDynPLHP->wchRemanenceByForce3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower3", L"", pIniResultOfDynPLHP->wchRemanenceByPower3, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity4", L"", pIniResultOfDynPLHP->wchLowerVelocity4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity4", L"", pIniResultOfDynPLHP->wchUpperVelocity4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity4", L"", pIniResultOfDynPLHP->wchNomialVelocity4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime4", L"", pIniResultOfDynPLHP->wchPLHPTime4, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP4", L"", pIniResultOfDynPLHP->wchPLHP4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce4", L"", pIniResultOfDynPLHP->wchRemanenceByForce4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower4", L"", pIniResultOfDynPLHP->wchRemanenceByPower4, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity5", L"", pIniResultOfDynPLHP->wchLowerVelocity5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity5", L"", pIniResultOfDynPLHP->wchUpperVelocity5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity5", L"", pIniResultOfDynPLHP->wchNomialVelocity5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime5", L"", pIniResultOfDynPLHP->wchPLHPTime5, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP5", L"", pIniResultOfDynPLHP->wchPLHP5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce5", L"", pIniResultOfDynPLHP->wchRemanenceByForce5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower5", L"", pIniResultOfDynPLHP->wchRemanenceByPower5, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity6", L"", pIniResultOfDynPLHP->wchLowerVelocity6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity6", L"", pIniResultOfDynPLHP->wchUpperVelocity6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity6", L"", pIniResultOfDynPLHP->wchNomialVelocity6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime6", L"", pIniResultOfDynPLHP->wchPLHPTime6, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP6", L"", pIniResultOfDynPLHP->wchPLHP6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce6", L"", pIniResultOfDynPLHP->wchRemanenceByForce6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower6", L"", pIniResultOfDynPLHP->wchRemanenceByPower6, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity7", L"", pIniResultOfDynPLHP->wchLowerVelocity7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity7", L"", pIniResultOfDynPLHP->wchUpperVelocity7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity7", L"", pIniResultOfDynPLHP->wchNomialVelocity7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime7", L"", pIniResultOfDynPLHP->wchPLHPTime7, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP7", L"", pIniResultOfDynPLHP->wchPLHP7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce7", L"", pIniResultOfDynPLHP->wchRemanenceByForce7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower7", L"", pIniResultOfDynPLHP->wchRemanenceByPower7, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"LowerVelocity8", L"", pIniResultOfDynPLHP->wchLowerVelocity8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"UpperVelocity8", L"", pIniResultOfDynPLHP->wchUpperVelocity8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"NomialVelocity8", L"", pIniResultOfDynPLHP->wchNomialVelocity8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHPTime8", L"", pIniResultOfDynPLHP->wchPLHPTime8, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"PLHP8", L"", pIniResultOfDynPLHP->wchPLHP8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByForce8", L"", pIniResultOfDynPLHP->wchRemanenceByForce8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"RemanenceByPower8", L"", pIniResultOfDynPLHP->wchRemanenceByPower8, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"Pass", L"", pIniResultOfDynPLHP->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfDynPLHP", L"Remark", L"", pIniResultOfDynPLHP->wchRemark, 1024);
	GetIniString(ini, L"ResultOfDynPLHP", L"Reserved1", L"", pIniResultOfDynPLHP->wchReserved1, 50);
	GetIniString(ini, L"ResultOfDynPLHP", L"Reserved2", L"", pIniResultOfDynPLHP->wchReserved2, 50);
	GetIniString(ini, L"ResultOfDynPLHP", L"Reserved3", L"", pIniResultOfDynPLHP->wchReserved3, 50);

}

// 流量计流量校准结果<DemarcationResultOfFloFluxCal>
void GetIniResultOfFloFluxCal(PDEMARCATIONRESULTOFFLOFLUXCAL const pIniResultOfFloFluxCal)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxCal.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFloFluxCal.ini信息
	GetIniString(ini, L"ResultOfFloFluxCal", L"AutoID", L"", pIniResultOfFloFluxCal->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFloFluxCal", L"DemarcationNumber", L"", pIniResultOfFloFluxCal->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfFloFluxCal", L"Pass", L"", pIniResultOfFloFluxCal->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfFloFluxCal", L"Remark", L"", pIniResultOfFloFluxCal->wchRemark, 1024);
	GetIniString(ini, L"ResultOfFloFluxCal", L"Reserved1", L"", pIniResultOfFloFluxCal->wchReserved1, 50);
	GetIniString(ini, L"ResultOfFloFluxCal", L"Reserved2", L"", pIniResultOfFloFluxCal->wchReserved2, 50);
	GetIniString(ini, L"ResultOfFloFluxCal", L"Reserved3", L"", pIniResultOfFloFluxCal->wchReserved3, 50);

}

// 流量计流量检查结果<DemarcationResultOfFloFluxChk>
void GetIniResultOfFloFluxChk(PDEMARCATIONRESULTOFFLOFLUXCHK const pIniResultOfFloFluxChk)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxChk.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFloFluxChk.ini信息
	GetIniString(ini, L"ResultOfFloFluxChk", L"AutoID", L"", pIniResultOfFloFluxChk->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFloFluxChk", L"DemarcationNumber", L"", pIniResultOfFloFluxChk->wchDemarcationNumber, 50);

	GetIniString(ini, L"ResultOfFloFluxChk", L"Pass", L"", pIniResultOfFloFluxChk->wchPass, NH_BOOL_TO_WCHART);
	GetIniString(ini, L"ResultOfFloFluxChk", L"Remark", L"", pIniResultOfFloFluxChk->wchRemark, 1024);
	GetIniString(ini, L"ResultOfFloFluxChk", L"Reserved1", L"", pIniResultOfFloFluxChk->wchReserved1, 50);
	GetIniString(ini, L"ResultOfFloFluxChk", L"Reserved2", L"", pIniResultOfFloFluxChk->wchReserved2, 50);
	GetIniString(ini, L"ResultOfFloFluxChk", L"Reserved3", L"", pIniResultOfFloFluxChk->wchReserved3, 50);

}

// 流量计稀释氧校准结果<DemarcationResultOfFloDilO2Cal>
void GetIniResultOfFloDilO2Cal(PDEMARCATIONRESULTOFFLODILO2CAL const pIniResultOfFloDilO2Cal)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloDilO2Cal.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFloDilO2Cal.ini信息
	GetIniString(ini, L"ResultOfFloDilO2Cal", L"AutoID", L"", pIniResultOfFloDilO2Cal->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFloDilO2Cal", L"DemarcationNumber", L"", pIniResultOfFloDilO2Cal->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfFloDilO2Cal", L"Pass", L"", pIniResultOfFloDilO2Cal->wchPass, NH_BOOL_TO_WCHART);

	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"StdO2", L"", pIniResultOfFloDilO2Cal->wchStdO2, NH_FLOAT_TO_WCHART);//标准O2
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"O2", L"", pIniResultOfFloDilO2Cal->wchO2, NH_FLOAT_TO_WCHART);//实测O2
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"RO2", L"", pIniResultOfFloDilO2Cal->wchRO2, NH_FLOAT_TO_WCHART);//相对误差
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"EO2", L"", pIniResultOfFloDilO2Cal->wchEO2, NH_FLOAT_TO_WCHART);//绝对误差
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"LowStdO2", L"", pIniResultOfFloDilO2Cal->wchLowStdO2, NH_FLOAT_TO_WCHART);//低浓度标准O2
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"LowO2", L"", pIniResultOfFloDilO2Cal->wchLowO2, NH_FLOAT_TO_WCHART);//低浓度实测O2
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"LowRO2", L"", pIniResultOfFloDilO2Cal->wchLowRO2, NH_FLOAT_TO_WCHART);//低浓度相对误差
	//GetIniString(ini, L"ResultOfFloDilO2Cal", L"LowEO2", L"", pIniResultOfFloDilO2Cal->wchLowEO2, NH_FLOAT_TO_WCHART);//低浓度绝对误差

	GetIniString(ini, L"ResultOfFloDilO2Cal", L"Remark", L"", pIniResultOfFloDilO2Cal->wchRemark, 1024);
	GetIniString(ini, L"ResultOfFloDilO2Cal", L"Reserved1", L"", pIniResultOfFloDilO2Cal->wchReserved1, 50);
	GetIniString(ini, L"ResultOfFloDilO2Cal", L"Reserved2", L"", pIniResultOfFloDilO2Cal->wchReserved2, 50);
	GetIniString(ini, L"ResultOfFloDilO2Cal", L"Reserved3", L"", pIniResultOfFloDilO2Cal->wchReserved3, 50);
}

// 流量计压力校准结果<DemarcationResultOfFloPresCal>
void GetIniResultOfFloPresCal(PDEMARCATIONRESULTOFFLOPRESCAL const pIniResultOfFloPresCal)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloPresCal.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ResultOfFloPresCal.ini信息
	GetIniString(ini, L"ResultOfFloPresCal", L"AutoID", L"", pIniResultOfFloPresCal->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"ResultOfFloPresCal", L"DemarcationNumber", L"", pIniResultOfFloPresCal->wchDemarcationNumber, 50);
	GetIniString(ini, L"ResultOfFloPresCal", L"Pass", L"", pIniResultOfFloPresCal->wchPass, NH_BOOL_TO_WCHART);

	GetIniString(ini, L"ResultOfFloPresCal", L"Remark", L"", pIniResultOfFloPresCal->wchRemark, 1024);
	GetIniString(ini, L"ResultOfFloPresCal", L"Reserved1", L"", pIniResultOfFloPresCal->wchReserved1, 50);
	GetIniString(ini, L"ResultOfFloPresCal", L"Reserved2", L"", pIniResultOfFloPresCal->wchReserved2, 50);
	GetIniString(ini, L"ResultOfFloPresCal", L"Reserved3", L"", pIniResultOfFloPresCal->wchReserved3, 50);
}

void GetIniEquCalChkInfo(PEQUCALCHKINFO const pIniEquCalChkInfo)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ini信息
	GetIniString(ini, L"EquCalChkInfo", L"AutoID", L"", pIniEquCalChkInfo->wchAutoID, (DWORD)(sizeof(pIniEquCalChkInfo->wchAutoID)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"LineNumber", L"", pIniEquCalChkInfo->wchLineNumber, (DWORD)(sizeof(pIniEquCalChkInfo->wchLineNumber)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Operator", L"", pIniEquCalChkInfo->wchOperator, (DWORD)(sizeof(pIniEquCalChkInfo->wchOperator)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"EquipmentNumber", L"", pIniEquCalChkInfo->wchEquipmentNumber, (DWORD)(sizeof(pIniEquCalChkInfo->wchEquipmentNumber)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Equipment", L"", pIniEquCalChkInfo->wchEquipment, (DWORD)(sizeof(pIniEquCalChkInfo->wchEquipment)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Item", L"", pIniEquCalChkInfo->wchItem, (DWORD)(sizeof(pIniEquCalChkInfo->wchItem)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Date", L"", pIniEquCalChkInfo->wchDate, (DWORD)(sizeof(pIniEquCalChkInfo->wchDate)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Data", L"", pIniEquCalChkInfo->wchData, (DWORD)(sizeof(pIniEquCalChkInfo->wchData)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Error", L"", pIniEquCalChkInfo->wchError, (DWORD)(sizeof(pIniEquCalChkInfo->wchError)/sizeof(wchar_t)));
	GetIniString(ini, L"EquCalChkInfo", L"Judgement", L"", pIniEquCalChkInfo->wchJudgement, (DWORD)(sizeof(pIniEquCalChkInfo->wchJudgement)/sizeof(wchar_t)));
}

void SetIniEquCalChkInfo(const EQUCALCHKINFO& iniEquCalChkInfo)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, true))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 设置ini信息
	WriteIniString(ini, L"EquCalChkInfo", L"AutoID", iniEquCalChkInfo.wchAutoID);
	WriteIniString(ini, L"EquCalChkInfo", L"LineNumber", iniEquCalChkInfo.wchLineNumber);
	WriteIniString(ini, L"EquCalChkInfo", L"Operator", iniEquCalChkInfo.wchOperator);
	WriteIniString(ini, L"EquCalChkInfo", L"EquipmentNumber", iniEquCalChkInfo.wchEquipmentNumber);
	WriteIniString(ini, L"EquCalChkInfo", L"Equipment", iniEquCalChkInfo.wchEquipment);
	WriteIniString(ini, L"EquCalChkInfo", L"Item", iniEquCalChkInfo.wchItem);
	WriteIniString(ini, L"EquCalChkInfo", L"Date", iniEquCalChkInfo.wchDate);
	WriteIniString(ini, L"EquCalChkInfo", L"Data", iniEquCalChkInfo.wchData);
	WriteIniString(ini, L"EquCalChkInfo", L"Error", iniEquCalChkInfo.wchError);
	WriteIniString(ini, L"EquCalChkInfo", L"Judgement", iniEquCalChkInfo.wchJudgement);
	
	ini.SaveFile(wchPath);
}

void GetIniSystemLog(PSYSTEMLOG const pIniSystemLog)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemLog.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ini信息
	GetIniString(ini, L"SystemLog", L"AutoID", L"", pIniSystemLog->wchAutoID, (DWORD)(sizeof(pIniSystemLog->wchAutoID)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Client", L"", pIniSystemLog->wchClient, (DWORD)(sizeof(pIniSystemLog->wchClient)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Date", L"", pIniSystemLog->wchDate, (DWORD)(sizeof(pIniSystemLog->wchDate)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"LoginUser", L"", pIniSystemLog->wchLoginUser, (DWORD)(sizeof(pIniSystemLog->wchLoginUser)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Description", L"", pIniSystemLog->wchDescription, (DWORD)(sizeof(pIniSystemLog->wchDescription)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Reason", L"", pIniSystemLog->wchReason, (DWORD)(sizeof(pIniSystemLog->wchReason)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Solution", L"", pIniSystemLog->wchSolution, (DWORD)(sizeof(pIniSystemLog->wchSolution)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Result", L"", pIniSystemLog->wchResult, (DWORD)(sizeof(pIniSystemLog->wchResult)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Remark", L"", pIniSystemLog->wchRemark, (DWORD)(sizeof(pIniSystemLog->wchRemark)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Reserved1", L"", pIniSystemLog->wchReserved1, (DWORD)(sizeof(pIniSystemLog->wchReserved1)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Reserved2", L"", pIniSystemLog->wchReserved2, (DWORD)(sizeof(pIniSystemLog->wchReserved2)/sizeof(wchar_t)));
	GetIniString(ini, L"SystemLog", L"Reserved3", L"", pIniSystemLog->wchReserved3, (DWORD)(sizeof(pIniSystemLog->wchReserved3)/sizeof(wchar_t)));
}

void SetIniSystemLog(const SYSTEMLOG& iniSystemLog)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemLog.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 设置ini信息
	WriteIniString(ini, L"SystemLog", L"AutoID", iniSystemLog.wchAutoID);
	WriteIniString(ini, L"SystemLog", L"Client", iniSystemLog.wchClient);
	WriteIniString(ini, L"SystemLog", L"Date", iniSystemLog.wchDate);
	WriteIniString(ini, L"SystemLog", L"LoginUser", iniSystemLog.wchLoginUser);
	WriteIniString(ini, L"SystemLog", L"Description", iniSystemLog.wchDescription);
	WriteIniString(ini, L"SystemLog", L"Reason", iniSystemLog.wchReason);
	WriteIniString(ini, L"SystemLog", L"Solution", iniSystemLog.wchSolution);
	WriteIniString(ini, L"SystemLog", L"Reason", iniSystemLog.wchReason);
	WriteIniString(ini, L"SystemLog", L"Result", iniSystemLog.wchResult);
	WriteIniString(ini, L"SystemLog", L"Remark", iniSystemLog.wchRemark);
	WriteIniString(ini, L"SystemLog", L"Reserved1", iniSystemLog.wchReserved1);
	WriteIniString(ini, L"SystemLog", L"Reserved2", iniSystemLog.wchReserved2);
	WriteIniString(ini, L"SystemLog", L"Reserved3", iniSystemLog.wchReserved3);

	ini.SaveFile(wchPath);
}

void GetIniSystemParameters(PSYSTEMPARAMETERS const pIniSystemParameters)
{
	assert(NULL != pIniSystemParameters);

	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemParameters.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ini信息
	GetIniString(ini, L"SystemParameters", L"AutoID", L"", pIniSystemParameters->wchAutoID, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"TimesOfPasswordInput", L"", pIniSystemParameters->wchTimesOfPasswordInput, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"TimesOfNoVIDTransmission", L"", pIniSystemParameters->wchTimesOfNoVIDTransmission, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"TimesOfDynamometerCoastDown", L"", pIniSystemParameters->wchTimesOfDynamometerCoastDown, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"ErrorOfDynamometerCoastDown", L"", pIniSystemParameters->wchErrorOfDynamometerCoastDown, NH_FLOAT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"TimesOfDynamometerRevSensorCheck", L"", pIniSystemParameters->wchTimesOfDynamometerRevSensorCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PasswordUnlock", L"", pIniSystemParameters->wchPasswordUnlock, 50);
	GetIniString(ini, L"SystemParameters", L"PasswordLock", L"", pIniSystemParameters->wchPasswordLock, 50);
	GetIniString(ini, L"SystemParameters", L"PeriodOfDynamometerCoastDown", L"", pIniSystemParameters->wchPeriodOfDynamometerCoastDown, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfDynamometerForceCheck", L"", pIniSystemParameters->wchPeriodOfDynamometerForceCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfDynamometerRevSensorCalibration", L"", pIniSystemParameters->wchPeriodOfDynamometerRevSensorCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfEnvParamMeasurerCalibration", L"", pIniSystemParameters->wchPeriodOfEnvParamMeasurerCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfEnvParamMeasurerCheck", L"", pIniSystemParameters->wchPeriodOfEnvParamMeasurerCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfAnalyzerChannelCalibration", L"", pIniSystemParameters->wchPeriodOfAnalyzerChannelCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfAnalyzerChannelCheck", L"", pIniSystemParameters->wchPeriodOfAnalyzerChannelCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfFlowmeterFluxCalibration", L"", pIniSystemParameters->wchPeriodOfFlowmeterFluxCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfFlowmeterFluxCheck", L"", pIniSystemParameters->wchPeriodOfFlowmeterFluxCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfOpacimeterChannelCalibration", L"", pIniSystemParameters->wchPeriodOfOpacimeterChannelCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfOpacimeterChannelCheck", L"", pIniSystemParameters->wchPeriodOfOpacimeterChannelCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfNOxAnalyzerChannelCalibration", L"", pIniSystemParameters->wchPeriodOfNOxAnalyzerChannelCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfNOxAnalyzerChannelCheck", L"", pIniSystemParameters->wchPeriodOfNOxAnalyzerChannelCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfSmokemeterCalibration", L"", pIniSystemParameters->wchPeriodOfSmokemeterCalibration, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"PeriodOfSmokemeterCheck", L"", pIniSystemParameters->wchPeriodOfSmokemeterCheck, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"TestPeriod", L"", pIniSystemParameters->wchTestPeriod, NH_INT_TO_WCHART);
	GetIniString(ini, L"SystemParameters", L"Remark", L"", pIniSystemParameters->wchRemark, 1024);
	GetIniString(ini, L"SystemParameters", L"Reserved1", L"", pIniSystemParameters->wchReserved1, 50);
	GetIniString(ini, L"SystemParameters", L"Reserved2", L"", pIniSystemParameters->wchReserved2, 50);
	GetIniString(ini, L"SystemParameters", L"Reserved3", L"", pIniSystemParameters->wchReserved3, 50);
}

void SetIniSystemParameters(const SYSTEMPARAMETERS& iniSystemParameters)
{
	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemParameters.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 设置ini信息
	WriteIniString(ini, L"SystemParameters", L"AutoID", iniSystemParameters.wchAutoID);
	WriteIniString(ini, L"SystemParameters", L"TimesOfPasswordInput", iniSystemParameters.wchTimesOfPasswordInput);
	WriteIniString(ini, L"SystemParameters", L"TimesOfNoVIDTransmission", iniSystemParameters.wchTimesOfNoVIDTransmission);
	WriteIniString(ini, L"SystemParameters", L"TimesOfDynamometerCoastDown", iniSystemParameters.wchTimesOfDynamometerCoastDown);
	WriteIniString(ini, L"SystemParameters", L"ErrorOfDynamometerCoastDown", iniSystemParameters.wchErrorOfDynamometerCoastDown);
	WriteIniString(ini, L"SystemParameters", L"TimesOfDynamometerRevSensorCheck", iniSystemParameters.wchTimesOfDynamometerRevSensorCheck);
	WriteIniString(ini, L"SystemParameters", L"PasswordUnlock", iniSystemParameters.wchPasswordUnlock);
	WriteIniString(ini, L"SystemParameters", L"PasswordLock", iniSystemParameters.wchPasswordLock);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfDynamometerCoastDown", iniSystemParameters.wchPeriodOfDynamometerCoastDown);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfDynamometerForceCheck", iniSystemParameters.wchPeriodOfDynamometerForceCheck);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfDynamometerRevSensorCalibration", iniSystemParameters.wchPeriodOfDynamometerRevSensorCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfEnvParamMeasurerCalibration", iniSystemParameters.wchPeriodOfEnvParamMeasurerCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfEnvParamMeasurerCheck", iniSystemParameters.wchPeriodOfEnvParamMeasurerCheck);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfAnalyzerChannelCalibration", iniSystemParameters.wchPeriodOfAnalyzerChannelCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfAnalyzerChannelCheck", iniSystemParameters.wchPeriodOfAnalyzerChannelCheck);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfFlowmeterFluxCalibration", iniSystemParameters.wchPeriodOfFlowmeterFluxCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfFlowmeterFluxCheck", iniSystemParameters.wchPeriodOfFlowmeterFluxCheck);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfOpacimeterChannelCalibration", iniSystemParameters.wchPeriodOfOpacimeterChannelCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfOpacimeterChannelCheck", iniSystemParameters.wchPeriodOfOpacimeterChannelCheck);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfNOxAnalyzerChannelCalibration", iniSystemParameters.wchPeriodOfNOxAnalyzerChannelCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfNOxAnalyzerChannelCheck", iniSystemParameters.wchPeriodOfNOxAnalyzerChannelCheck);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfSmokemeterCalibration", iniSystemParameters.wchPeriodOfSmokemeterCalibration);
	WriteIniString(ini, L"SystemParameters", L"PeriodOfSmokemeterCheck", iniSystemParameters.wchPeriodOfSmokemeterCheck);
	WriteIniString(ini, L"SystemParameters", L"TestPeriod", iniSystemParameters.wchTestPeriod);
	WriteIniString(ini, L"SystemParameters", L"Remark", iniSystemParameters.wchRemark);
	WriteIniString(ini, L"SystemParameters", L"Reserved1", iniSystemParameters.wchReserved1);
	WriteIniString(ini, L"SystemParameters", L"Reserved2", iniSystemParameters.wchReserved2);
	WriteIniString(ini, L"SystemParameters", L"Reserved3", iniSystemParameters.wchReserved3);

	ini.SaveFile(wchPath);
}


void  GetIniResultOfOBD(SResultOfOBD* const pResultOfOBD)
{
	assert(NULL != pResultOfOBD);

	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOBD.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 获取ini信息
	pResultOfOBD->strAutoID = ini.GetValue(L"ResultOfOBD", L"AutoID", L"");
	pResultOfOBD->strRunningNumber = ini.GetValue(L"ResultOfOBD", L"RunningNumber", L"");
	pResultOfOBD->strFuelType = ini.GetValue(L"ResultOfOBD", L"FuelType", L"");
	pResultOfOBD->strOperator = ini.GetValue(L"ResultOfOBD", L"Operator", L"");
	pResultOfOBD->strDetTimes = ini.GetValue(L"ResultOfOBD", L"DetTimes", L"");
	pResultOfOBD->strDetBegTime = ini.GetValue(L"ResultOfOBD", L"DetBegTime", L"");
	pResultOfOBD->strDetEndTime = ini.GetValue(L"ResultOfOBD", L"DetEndTime", L"");
	pResultOfOBD->strOBDProtocol = ini.GetValue(L"ResultOfOBD", L"OBDProtocol", L"");
	pResultOfOBD->strComErrorJudge = ini.GetValue(L"ResultOfOBD", L"ComErrorJudge", L"");
	pResultOfOBD->strComErrorComment = ini.GetValue(L"ResultOfOBD", L"ComErrorComment", L"");
	pResultOfOBD->strVIN = ini.GetValue(L"ResultOfOBD", L"VIN", L"");
	pResultOfOBD->strOBDType = ini.GetValue(L"ResultOfOBD", L"OBDType", L"");
	pResultOfOBD->strMileage = ini.GetValue(L"ResultOfOBD", L"Mileage", L"");
	pResultOfOBD->strEngineCALID = ini.GetValue(L"ResultOfOBD", L"EngineCALID", L"");
	pResultOfOBD->strEngineCVN = ini.GetValue(L"ResultOfOBD", L"EngineCVN", L"");
	pResultOfOBD->strPostProcessingCALID = ini.GetValue(L"ResultOfOBD", L"PostProcessingCALID", L"");
	pResultOfOBD->strPostProcessingCVN = ini.GetValue(L"ResultOfOBD", L"PostProcessingCVN", L"");
	pResultOfOBD->strOtherCALID = ini.GetValue(L"ResultOfOBD", L"OtherCALID", L"");
	pResultOfOBD->strOtherCVN = ini.GetValue(L"ResultOfOBD", L"OtherCVN", L"");
	pResultOfOBD->strDisplayJudge = ini.GetValue(L"ResultOfOBD", L"DisplayJudge", L"");
	pResultOfOBD->strDisplayComment = ini.GetValue(L"ResultOfOBD", L"DisplayComment", L"");
	pResultOfOBD->strLampStateJudge = ini.GetValue(L"ResultOfOBD", L"LampStateJudge", L"");
	pResultOfOBD->strLampStateComment = ini.GetValue(L"ResultOfOBD", L"LampStateComment", L"");
	pResultOfOBD->strDTC = ini.GetValue(L"ResultOfOBD", L"DTC", L"");
	pResultOfOBD->strDTCComment = ini.GetValue(L"ResultOfOBD", L"DTCComment", L"");
	pResultOfOBD->strFreezeData = ini.GetValue(L"ResultOfOBD", L"FreezeData", L"");
	pResultOfOBD->strDTCMileage = ini.GetValue(L"ResultOfOBD", L"DTCMileage", L"");
	pResultOfOBD->strDTCJudge = ini.GetValue(L"ResultOfOBD", L"DTCJudge", L"");
	pResultOfOBD->strCheckItemJudge_MFT = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_MFT", L"");
	pResultOfOBD->strCheckItemJudge_FST = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_FST", L"");
	pResultOfOBD->strCheckItemJudge_CC = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_CC", L"");
	pResultOfOBD->strCheckItemJudge_CCH = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_CCH", L"");
	pResultOfOBD->strCheckItemJudge_OS = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_OS", L"");
	pResultOfOBD->strCheckItemJudge_OSH = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_OSH", L"");
	pResultOfOBD->strCheckItemJudge_EECS = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_EECS", L"");
	pResultOfOBD->strCheckItemJudge_EGR = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_EGR", L"");
	pResultOfOBD->strCheckItemJudge_SAIS = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_SAIS", L"");
	pResultOfOBD->strCheckItemJudge_ICM = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_ICM", L"");
	pResultOfOBD->strCheckItemJudge_VVT = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_VVT", L"");
	pResultOfOBD->strCheckItemJudge_DOC = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_DOC", L"");
	pResultOfOBD->strCheckItemJudge_SCR = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_SCR", L"");
	pResultOfOBD->strCheckItemJudge_DPF = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_DPF", L"");
	pResultOfOBD->strCheckItemJudge_ASC = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_ASC", L"");
	pResultOfOBD->strCheckItemJudge_POC = ini.GetValue(L"ResultOfOBD", L"CheckItemJudge_POC", L"");
	pResultOfOBD->strSystemCheckJudge = ini.GetValue(L"ResultOfOBD", L"SystemCheckJudge", L"");
	pResultOfOBD->strIsNeedReCheck = ini.GetValue(L"ResultOfOBD", L"IsNeedReCheck", L"");
	pResultOfOBD->strReCheckItem = ini.GetValue(L"ResultOfOBD", L"ReCheckItem", L"");
	pResultOfOBD->strReCheckResult = ini.GetValue(L"ResultOfOBD", L"ReCheckResult", L"");
	pResultOfOBD->strIUPR_OMCEC = ini.GetValue(L"ResultOfOBD", L"IUPR_OMCEC", L"");
	pResultOfOBD->strIUPR_ICC = ini.GetValue(L"ResultOfOBD", L"IUPR_ICC", L"");
	pResultOfOBD->strIUPR_CMCCB1 = ini.GetValue(L"ResultOfOBD", L"IUPR_CMCCB1", L"");
	pResultOfOBD->strIUPR_CMCECB1 = ini.GetValue(L"ResultOfOBD", L"IUPR_CMCECB1", L"");
	pResultOfOBD->strIUPR_CMCCB2 = ini.GetValue(L"ResultOfOBD", L"IUPR_CMCCB2", L"");
	pResultOfOBD->strIUPR_CMCECB2 = ini.GetValue(L"ResultOfOBD", L"IUPR_CMCECB2", L"");
	pResultOfOBD->strIUPR_O2SMCCB1 = ini.GetValue(L"ResultOfOBD", L"IUPR_O2SMCCB1", L"");
	pResultOfOBD->strIUPR_O2SMCECB1 = ini.GetValue(L"ResultOfOBD", L"IUPR_O2SMCECB1", L"");
	pResultOfOBD->strIUPR_O2SMCCB2 = ini.GetValue(L"ResultOfOBD", L"IUPR_O2SMCCB2", L"");
	pResultOfOBD->strIUPR_O2SMCECB2 = ini.GetValue(L"ResultOfOBD", L"IUPR_O2SMCECB2", L"");
	pResultOfOBD->strIUPR_RO2SMCCB1 = ini.GetValue(L"ResultOfOBD", L"IUPR_RO2SMCCB1", L"");
	pResultOfOBD->strIUPR_RO2SMCECB1 = ini.GetValue(L"ResultOfOBD", L"IUPR_RO2SMCECB1", L"");
	pResultOfOBD->strIUPR_RO2SMCCB2 = ini.GetValue(L"ResultOfOBD", L"IUPR_RO2SMCCB2", L"");
	pResultOfOBD->strIUPR_RO2SMCECB2 = ini.GetValue(L"ResultOfOBD", L"IUPR_RO2SMCECB2", L"");
	pResultOfOBD->strIUPR_EGRC = ini.GetValue(L"ResultOfOBD", L"IUPR_EGRC", L"");
	pResultOfOBD->strIUPR_EGREC = ini.GetValue(L"ResultOfOBD", L"IUPR_EGREC", L"");
	pResultOfOBD->strIUPR_GPFC1 = ini.GetValue(L"ResultOfOBD", L"IUPR_GPFC1", L"");
	pResultOfOBD->strIUPR_GPFEC1 = ini.GetValue(L"ResultOfOBD", L"IUPR_GPFEC1", L"");
	pResultOfOBD->strIUPR_GPFC2 = ini.GetValue(L"ResultOfOBD", L"IUPR_GPFC2", L"");
	pResultOfOBD->strIUPR_GPFEC2 = ini.GetValue(L"ResultOfOBD", L"IUPR_GPFEC2", L"");
	pResultOfOBD->strIUPR_AMCCC = ini.GetValue(L"ResultOfOBD", L"IUPR_AMCCC", L"");
	pResultOfOBD->strIUPR_AMCEC = ini.GetValue(L"ResultOfOBD", L"IUPR_AMCEC", L"");
	pResultOfOBD->strIUPR_EVAPC = ini.GetValue(L"ResultOfOBD", L"IUPR_EVAPC", L"");
	pResultOfOBD->strIUPR_EVAPEC = ini.GetValue(L"ResultOfOBD", L"IUPR_EVAPEC", L"");
	pResultOfOBD->strIUPR_NMHCC = ini.GetValue(L"ResultOfOBD", L"IUPR_NMHCC", L"");
	pResultOfOBD->strIUPR_NMHCEC = ini.GetValue(L"ResultOfOBD", L"IUPR_NMHCEC", L"");
	pResultOfOBD->strIUPR_NOXCC = ini.GetValue(L"ResultOfOBD", L"IUPR_NOXCC", L"");
	pResultOfOBD->strIUPR_NOXCEC = ini.GetValue(L"ResultOfOBD", L"IUPR_NOXCEC", L"");
	pResultOfOBD->strIUPR_NOXAC = ini.GetValue(L"ResultOfOBD", L"IUPR_NOXAC", L"");
	pResultOfOBD->strIUPR_NOXAEC = ini.GetValue(L"ResultOfOBD", L"IUPR_NOXAEC", L"");
	pResultOfOBD->strIUPR_PMC = ini.GetValue(L"ResultOfOBD", L"IUPR_PMC", L"");
	pResultOfOBD->strIUPR_PMEC = ini.GetValue(L"ResultOfOBD", L"IUPR_PMEC", L"");
	pResultOfOBD->strIUPR_WSC = ini.GetValue(L"ResultOfOBD", L"IUPR_WSC", L"");
	pResultOfOBD->strIUPR_WSEC = ini.GetValue(L"ResultOfOBD", L"IUPR_WSEC", L"");
	pResultOfOBD->strIUPR_PPC = ini.GetValue(L"ResultOfOBD", L"IUPR_PPC", L"");
	pResultOfOBD->strIUPR_PPEC = ini.GetValue(L"ResultOfOBD", L"IUPR_PPEC", L"");
	pResultOfOBD->strJudge = ini.GetValue(L"ResultOfOBD", L"Judge", L"");
	pResultOfOBD->strHash = ini.GetValue(L"ResultOfOBD", L"Hash", L"");
	pResultOfOBD->strRemark = ini.GetValue(L"ResultOfOBD", L"Remark", L"");
	pResultOfOBD->strReserved1 = ini.GetValue(L"ResultOfOBD", L"Reserved1", L"");
	pResultOfOBD->strReserved2 = ini.GetValue(L"ResultOfOBD", L"Reserved2", L"");
	pResultOfOBD->strReserved3 = ini.GetValue(L"ResultOfOBD", L"Reserved3", L"");
}

void  SetIniResultOfOBD(const SResultOfOBD& sResultOfOBD)
{

	// 获取ini路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOBD.ini", wchPath, true))
	{
		return;
	}
	
	CSimpleIni ini(true);

	// 设置ini信息
	ini.SetValue(L"ResultOfOBD", L"AutoID", sResultOfOBD.strAutoID.c_str());
	ini.SetValue(L"ResultOfOBD", L"RunningNumber", sResultOfOBD.strRunningNumber.c_str());
	ini.SetValue(L"ResultOfOBD", L"FuelType", sResultOfOBD.strFuelType.c_str());
	ini.SetValue(L"ResultOfOBD", L"Operator", sResultOfOBD.strOperator.c_str());
	ini.SetValue(L"ResultOfOBD", L"DetTimes", sResultOfOBD.strDetTimes.c_str());
	ini.SetValue(L"ResultOfOBD", L"DetBegTime", sResultOfOBD.strDetBegTime.c_str());
	ini.SetValue(L"ResultOfOBD", L"DetEndTime", sResultOfOBD.strDetEndTime.c_str());
	ini.SetValue(L"ResultOfOBD", L"OBDProtocol", sResultOfOBD.strOBDProtocol.c_str());
	ini.SetValue(L"ResultOfOBD", L"ComErrorJudge", sResultOfOBD.strComErrorJudge.c_str());
	ini.SetValue(L"ResultOfOBD", L"ComErrorComment", sResultOfOBD.strComErrorComment.c_str());
	ini.SetValue(L"ResultOfOBD", L"VIN", sResultOfOBD.strVIN.c_str());
	ini.SetValue(L"ResultOfOBD", L"OBDType", sResultOfOBD.strOBDType.c_str());
	ini.SetValue(L"ResultOfOBD", L"Mileage", sResultOfOBD.strMileage.c_str());
	ini.SetValue(L"ResultOfOBD", L"EngineCALID", sResultOfOBD.strEngineCALID.c_str());
	ini.SetValue(L"ResultOfOBD", L"EngineCVN", sResultOfOBD.strEngineCVN.c_str());
	ini.SetValue(L"ResultOfOBD", L"PostProcessingCALID", sResultOfOBD.strPostProcessingCALID.c_str());
	ini.SetValue(L"ResultOfOBD", L"PostProcessingCVN", sResultOfOBD.strPostProcessingCVN.c_str());
	ini.SetValue(L"ResultOfOBD", L"OtherCALID", sResultOfOBD.strOtherCALID.c_str());
	ini.SetValue(L"ResultOfOBD", L"OtherCVN", sResultOfOBD.strOtherCVN.c_str());
	ini.SetValue(L"ResultOfOBD", L"DisplayJudge", sResultOfOBD.strDisplayJudge.c_str());
	ini.SetValue(L"ResultOfOBD", L"DisplayComment", sResultOfOBD.strDisplayComment.c_str());
	ini.SetValue(L"ResultOfOBD", L"LampStateJudge", sResultOfOBD.strLampStateJudge.c_str());
	ini.SetValue(L"ResultOfOBD", L"LampStateComment", sResultOfOBD.strLampStateComment.c_str());
	ini.SetValue(L"ResultOfOBD", L"DTC", sResultOfOBD.strDTC.c_str());
	ini.SetValue(L"ResultOfOBD", L"DTCComment", sResultOfOBD.strDTCComment.c_str());
	ini.SetValue(L"ResultOfOBD", L"FreezeData", sResultOfOBD.strFreezeData.c_str());
	ini.SetValue(L"ResultOfOBD", L"DTCMileage", sResultOfOBD.strDTCMileage.c_str());
	ini.SetValue(L"ResultOfOBD", L"DTCJudge", sResultOfOBD.strDTCJudge.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_MFT", sResultOfOBD.strCheckItemJudge_MFT.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_FST", sResultOfOBD.strCheckItemJudge_FST.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_CC", sResultOfOBD.strCheckItemJudge_CC.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_CCH", sResultOfOBD.strCheckItemJudge_CCH.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_OS", sResultOfOBD.strCheckItemJudge_OS.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_OSH", sResultOfOBD.strCheckItemJudge_OSH.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_EECS", sResultOfOBD.strCheckItemJudge_EECS.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_EGR", sResultOfOBD.strCheckItemJudge_EGR.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_SAIS", sResultOfOBD.strCheckItemJudge_SAIS.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_ICM", sResultOfOBD.strCheckItemJudge_ICM.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_VVT", sResultOfOBD.strCheckItemJudge_VVT.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_DOC", sResultOfOBD.strCheckItemJudge_DOC.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_SCR", sResultOfOBD.strCheckItemJudge_SCR.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_DPF", sResultOfOBD.strCheckItemJudge_DPF.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_ASC", sResultOfOBD.strCheckItemJudge_ASC.c_str());
	ini.SetValue(L"ResultOfOBD", L"CheckItemJudge_POC", sResultOfOBD.strCheckItemJudge_POC.c_str());
	ini.SetValue(L"ResultOfOBD", L"SystemCheckJudge", sResultOfOBD.strSystemCheckJudge.c_str());
	ini.SetValue(L"ResultOfOBD", L"IsNeedReCheck", sResultOfOBD.strIsNeedReCheck.c_str());
	ini.SetValue(L"ResultOfOBD", L"ReCheckItem", sResultOfOBD.strReCheckItem.c_str());
	ini.SetValue(L"ResultOfOBD", L"ReCheckResult", sResultOfOBD.strReCheckResult.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_OMCEC", sResultOfOBD.strIUPR_OMCEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_ICC", sResultOfOBD.strIUPR_ICC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_CMCCB1", sResultOfOBD.strIUPR_CMCCB1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_CMCECB1", sResultOfOBD.strIUPR_CMCECB1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_CMCCB2", sResultOfOBD.strIUPR_CMCCB2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_CMCECB2", sResultOfOBD.strIUPR_CMCECB2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_O2SMCCB1", sResultOfOBD.strIUPR_O2SMCCB1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_O2SMCECB1", sResultOfOBD.strIUPR_O2SMCECB1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_O2SMCCB2", sResultOfOBD.strIUPR_O2SMCCB2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_O2SMCECB2", sResultOfOBD.strIUPR_O2SMCECB2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_RO2SMCCB1", sResultOfOBD.strIUPR_RO2SMCCB1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_RO2SMCECB1", sResultOfOBD.strIUPR_RO2SMCECB1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_RO2SMCCB2", sResultOfOBD.strIUPR_RO2SMCCB2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_RO2SMCECB2", sResultOfOBD.strIUPR_RO2SMCECB2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_EGRC", sResultOfOBD.strIUPR_EGRC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_EGREC", sResultOfOBD.strIUPR_EGREC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_GPFC1", sResultOfOBD.strIUPR_GPFC1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_GPFEC1", sResultOfOBD.strIUPR_GPFEC1.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_GPFC2", sResultOfOBD.strIUPR_GPFC2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_GPFEC2", sResultOfOBD.strIUPR_GPFEC2.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_AMCCC", sResultOfOBD.strIUPR_AMCCC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_AMCEC", sResultOfOBD.strIUPR_AMCEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_EVAPC", sResultOfOBD.strIUPR_EVAPC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_EVAPEC", sResultOfOBD.strIUPR_EVAPEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_NMHCC", sResultOfOBD.strIUPR_NMHCC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_NMHCEC", sResultOfOBD.strIUPR_NMHCEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_NOXCC", sResultOfOBD.strIUPR_NOXCC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_NOXCEC", sResultOfOBD.strIUPR_NOXCEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_NOXAC", sResultOfOBD.strIUPR_NOXAC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_NOXAEC", sResultOfOBD.strIUPR_NOXAEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_PMC", sResultOfOBD.strIUPR_PMC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_PMEC", sResultOfOBD.strIUPR_PMEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_WSC", sResultOfOBD.strIUPR_WSC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_WSEC", sResultOfOBD.strIUPR_WSEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_PPC", sResultOfOBD.strIUPR_PPC.c_str());
	ini.SetValue(L"ResultOfOBD", L"IUPR_PPEC", sResultOfOBD.strIUPR_PPEC.c_str());
	ini.SetValue(L"ResultOfOBD", L"Judge", sResultOfOBD.strJudge.c_str());
	ini.SetValue(L"ResultOfOBD", L"Hash", sResultOfOBD.strHash.c_str());
	ini.SetValue(L"ResultOfOBD", L"Remark", sResultOfOBD.strRemark.c_str());
	ini.SetValue(L"ResultOfOBD", L"Reserved1", sResultOfOBD.strReserved1.c_str());
	ini.SetValue(L"ResultOfOBD", L"Reserved2", sResultOfOBD.strReserved2.c_str());
	ini.SetValue(L"ResultOfOBD", L"Reserved3", sResultOfOBD.strReserved3.c_str());

	ini.SaveFile(wchPath);
}


void GetIniIdleDataOfVMAS(list<REALTIMEDATAOFVMAS>& listIdleDataOfVMAS)
{
	// 获取RealTimeDataOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"IdleDataOfVMAS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfVMAS iniRealTimeDataOfVMAS;

	for(int i=1; i<=nTotal; i++)
	{
		// 获取RealTimeDataOfVMAS.ini信息
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniRealTimeDataOfVMAS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniRealTimeDataOfVMAS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniRealTimeDataOfVMAS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniRealTimeDataOfVMAS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniRealTimeDataOfVMAS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniRealTimeDataOfVMAS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"Velocity", L"", iniRealTimeDataOfVMAS.wchVelocity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Power", L"", iniRealTimeDataOfVMAS.wchPower, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Force", L"", iniRealTimeDataOfVMAS.wchForce, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"IHP", L"", iniRealTimeDataOfVMAS.wchIHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PLHP", L"", iniRealTimeDataOfVMAS.wchPLHP, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HC", L"", iniRealTimeDataOfVMAS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniRealTimeDataOfVMAS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniRealTimeDataOfVMAS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniRealTimeDataOfVMAS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniRealTimeDataOfVMAS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniRealTimeDataOfVMAS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniRealTimeDataOfVMAS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniRealTimeDataOfVMAS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2OfFlowmeter", L"", iniRealTimeDataOfVMAS.wchO2OfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2OfEnvironment", L"", iniRealTimeDataOfVMAS.wchO2OfEnvironment, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HCm", L"", iniRealTimeDataOfVMAS.wchHCm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"COm", L"", iniRealTimeDataOfVMAS.wchCOm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NOm", L"", iniRealTimeDataOfVMAS.wchNOm, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2m", L"", iniRealTimeDataOfVMAS.wchO2m, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2m", L"", iniRealTimeDataOfVMAS.wchCO2m, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"PressureOfFlowmeter", L"", iniRealTimeDataOfVMAS.wchPressureOfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"TemperatureOfFlowmeter", L"", iniRealTimeDataOfVMAS.wchTemperatureOfFlowmeter, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DR", L"", iniRealTimeDataOfVMAS.wchDR, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"ActualFluxOfGas", L"", iniRealTimeDataOfVMAS.wchActualFluxOfGas, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"StandardFluxOfGas", L"", iniRealTimeDataOfVMAS.wchStandardFluxOfGas, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"FluxOfExhaust", L"", iniRealTimeDataOfVMAS.wchFluxOfExhaust, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniRealTimeDataOfVMAS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniRealTimeDataOfVMAS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniRealTimeDataOfVMAS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"HumidityCorrectionFactor", L"", iniRealTimeDataOfVMAS.wchHumidityCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"DilutionCorrectionFactor", L"", iniRealTimeDataOfVMAS.wchDilutionCorrectionFactor, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"InertiaSimulationError", L"", iniRealTimeDataOfVMAS.wchInertiaSimulationError, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniRealTimeDataOfVMAS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniRealTimeDataOfVMAS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniRealTimeDataOfVMAS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniRealTimeDataOfVMAS.wchReserved3, 50);

		listIdleDataOfVMAS.push_back(iniRealTimeDataOfVMAS);
	}
	CNHLogAPI::WriteLog(L"NHAppDataINI", L"GetIniIDleDataOfVMAS", L"获取怠速数据");
}

void SetIniIdleDataOfVMAS(REALTIMEDATAOFVMAS* pIdleDataOfVMAS)
{
	// 获取IdleDataOfVMAS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"IdleDataOfVMAS.ini", wchPath, true))
	{
		return;
	}

	CSimpleIni ini(true);
	int n = 1;
	CString strTime;
	for(; n<= 40; n++)
	{
		strTime.Format(L"%d", n);
		WriteIniString(ini, strTime, L"AutoID", pIdleDataOfVMAS[n-1].wchAutoID);
		WriteIniString(ini, strTime, L"ReportNumber", pIdleDataOfVMAS[n-1].wchReportNumber);
		WriteIniString(ini, strTime, L"FuelTypeCode", pIdleDataOfVMAS[n-1].wchFuelTypeCode);
		WriteIniString(ini, strTime, L"FuelTypeName", pIdleDataOfVMAS[n-1].wchFuelTypeName);
		WriteIniString(ini, strTime, L"O2", pIdleDataOfVMAS[n-1].wchO2);
		WriteIniString(ini, strTime, L"CO2", pIdleDataOfVMAS[n-1].wchCO2);
		WriteIniString(ini, strTime, L"HC", pIdleDataOfVMAS[n-1].wchHC);
		WriteIniString(ini, strTime, L"CO", pIdleDataOfVMAS[n-1].wchCO);
		WriteIniString(ini, strTime, L"NO", pIdleDataOfVMAS[n-1].wchNO);
		WriteIniString(ini, strTime, L"Time", pIdleDataOfVMAS[n-1].wchTime);
		WriteIniString(ini, strTime, L"SamplingTime", pIdleDataOfVMAS[n-1].wchSamplingTime);
		WriteIniString(ini, strTime, L"Velocity", pIdleDataOfVMAS[n-1].wchVelocity);
		WriteIniString(ini, strTime, L"Power", pIdleDataOfVMAS[n-1].wchPower);
		WriteIniString(ini, strTime, L"Force", pIdleDataOfVMAS[n-1].wchForce);
		WriteIniString(ini, strTime, L"IHP", pIdleDataOfVMAS[n-1].wchIHP);
		WriteIniString(ini, strTime, L"PLHP", pIdleDataOfVMAS[n-1].wchPLHP);
		WriteIniString(ini, strTime, L"EngineRev", pIdleDataOfVMAS[n-1].wchEngineRev);
		WriteIniString(ini, strTime, L"Lambda", pIdleDataOfVMAS[n-1].wchLambda);
		WriteIniString(ini, strTime, L"OilTemperature", pIdleDataOfVMAS[n-1].wchOilTemperature);
		WriteIniString(ini, strTime, L"O2OfFlowmeter", pIdleDataOfVMAS[n-1].wchO2OfFlowmeter);
		WriteIniString(ini, strTime, L"O2OfEnvironment", pIdleDataOfVMAS[n-1].wchO2OfEnvironment);
		WriteIniString(ini, strTime, L"HCm", pIdleDataOfVMAS[n-1].wchHCm);
		WriteIniString(ini, strTime, L"COm", pIdleDataOfVMAS[n-1].wchCOm);
		WriteIniString(ini, strTime, L"NOm", pIdleDataOfVMAS[n-1].wchNOm);
		WriteIniString(ini, strTime, L"O2m", pIdleDataOfVMAS[n-1].wchO2m);
		WriteIniString(ini, strTime, L"CO2m", pIdleDataOfVMAS[n-1].wchCO2m);
		WriteIniString(ini, strTime, L"PressureOfFlowmeter", pIdleDataOfVMAS[n-1].wchPressureOfFlowmeter);
		WriteIniString(ini, strTime, L"TemperatureOfFlowmeter", pIdleDataOfVMAS[n-1].wchTemperatureOfFlowmeter);
		WriteIniString(ini, strTime, L"DR", pIdleDataOfVMAS[n-1].wchDR);
		WriteIniString(ini, strTime, L"ActualFluxOfGas", pIdleDataOfVMAS[n-1].wchActualFluxOfGas);
		WriteIniString(ini, strTime, L"StandardFluxOfGas", pIdleDataOfVMAS[n-1].wchStandardFluxOfGas);
		WriteIniString(ini, strTime, L"FluxOfExhaust", pIdleDataOfVMAS[n-1].wchFluxOfExhaust);
		WriteIniString(ini, strTime, L"EnvironmentalTemperature", pIdleDataOfVMAS[n-1].wchEnvironmentalTemperature);
		WriteIniString(ini, strTime, L"RelativeHumidity", pIdleDataOfVMAS[n-1].wchRelativeHumidity);
		WriteIniString(ini, strTime, L"AtmosphericPressure", pIdleDataOfVMAS[n-1].wchAtmosphericPressure);
		WriteIniString(ini, strTime, L"HumidityCorrectionFactor", pIdleDataOfVMAS[n-1].wchHumidityCorrectionFactor);
		WriteIniString(ini, strTime, L"DilutionCorrectionFactor", pIdleDataOfVMAS[n-1].wchDilutionCorrectionFactor);
		WriteIniString(ini, strTime, L"Remark", pIdleDataOfVMAS[n-1].wchRemark);
		WriteIniString(ini, strTime, L"Reserved1", pIdleDataOfVMAS[n-1].wchReserved1);
		WriteIniString(ini, strTime, L"Reserved2", pIdleDataOfVMAS[n-1].wchReserved2);
		WriteIniString(ini, strTime, L"Reserved3", pIdleDataOfVMAS[n-1].wchReserved3);
	}
	WriteIniString(ini, L"Sum", L"Total", strTime);

	ini.SaveFile(wchPath);
}


void GetIniIdleDataOfDIS(list<REALTIMEDATAOFDIS>& listIdleDataOfDIS)
{
	// 获取IdleDataOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"IdleDataOfDIS.ini", wchPath))
	{
		return;
	}

	CSimpleIni ini(true);
	ini.LoadFile(wchPath);

	// 读取总条数
	wchar_t wchTotal[NH_INT_TO_WCHART];
	GetIniString(ini, L"Sum", L"Total", L"0", wchTotal, NH_INT_TO_WCHART);
	int nTotal = _wtoi(wchTotal);
	if(nTotal <= 0)
	{
		return;
	}

	RealTimeDataOfDIS iniIdleDataOfDIS;

	for(int i=1; i<=nTotal; i++)
	{
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i);
		GetIniString(ini, wchSection, L"AutoID", L"", iniIdleDataOfDIS.wchAutoID, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"ReportNumber", L"", iniIdleDataOfDIS.wchReportNumber, 50);
		GetIniString(ini, wchSection, L"FuelTypeCode", L"", iniIdleDataOfDIS.wchFuelTypeCode, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"FuelTypeName", L"", iniIdleDataOfDIS.wchFuelTypeName, 50);
		GetIniString(ini, wchSection, L"Time", L"", iniIdleDataOfDIS.wchTime, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"SamplingTime", L"", iniIdleDataOfDIS.wchSamplingTime, 50);
		GetIniString(ini, wchSection, L"HC", L"", iniIdleDataOfDIS.wchHC, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO", L"", iniIdleDataOfDIS.wchCO, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"O2", L"", iniIdleDataOfDIS.wchO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"CO2", L"", iniIdleDataOfDIS.wchCO2, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"NO", L"", iniIdleDataOfDIS.wchNO, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"EngineRev", L"", iniIdleDataOfDIS.wchEngineRev, NH_INT_TO_WCHART);
		GetIniString(ini, wchSection, L"OilTemperature", L"", iniIdleDataOfDIS.wchOilTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Lambda", L"", iniIdleDataOfDIS.wchLambda, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"EnvironmentalTemperature", L"", iniIdleDataOfDIS.wchEnvironmentalTemperature, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"RelativeHumidity", L"", iniIdleDataOfDIS.wchRelativeHumidity, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"AtmosphericPressure", L"", iniIdleDataOfDIS.wchAtmosphericPressure, NH_FLOAT_TO_WCHART);
		GetIniString(ini, wchSection, L"Remark", L"", iniIdleDataOfDIS.wchRemark, 1024);
		GetIniString(ini, wchSection, L"Reserved1", L"", iniIdleDataOfDIS.wchReserved1, 50);
		GetIniString(ini, wchSection, L"Reserved2", L"", iniIdleDataOfDIS.wchReserved2, 50);
		GetIniString(ini, wchSection, L"Reserved3", L"", iniIdleDataOfDIS.wchReserved3, 50);

		listIdleDataOfDIS.push_back(iniIdleDataOfDIS);
	}
}

void SetIniIdleDataOfDIS(REALTIMEDATAOFDIS* pIdleDataOfDIS)
{
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"IdleDataOfDIS.ini", wchPath, true))
	{
		return;
	}

	CSimpleIni ini(true);
	int n = 1;
	CString strTime;
	for(; n<= 30; n++)
	{
		strTime.Format(L"%d", n);
		WriteIniString(ini, strTime, L"AutoID", pIdleDataOfDIS[n-1].wchAutoID);
		WriteIniString(ini, strTime, L"AtmosphericPressure", pIdleDataOfDIS[n-1].wchAtmosphericPressure);
		WriteIniString(ini, strTime, L"CO", pIdleDataOfDIS[n-1].wchCO);
		WriteIniString(ini, strTime, L"CO2", pIdleDataOfDIS[n-1].wchCO2);
		WriteIniString(ini, strTime, L"EngineRev", pIdleDataOfDIS[n-1].wchEngineRev);
		WriteIniString(ini, strTime, L"EnvironmentalTemperature", pIdleDataOfDIS[n-1].wchEnvironmentalTemperature);
		WriteIniString(ini, strTime, L"FuelTypeCode", pIdleDataOfDIS[n-1].wchFuelTypeCode);
		WriteIniString(ini, strTime, L"FuelTypeName", pIdleDataOfDIS[n-1].wchFuelTypeName);
		WriteIniString(ini, strTime, L"HC", pIdleDataOfDIS[n-1].wchHC);
		WriteIniString(ini, strTime, L"Lambda", pIdleDataOfDIS[n-1].wchLambda);
		WriteIniString(ini, strTime, L"NO", pIdleDataOfDIS[n-1].wchNO);
		WriteIniString(ini, strTime, L"O2", pIdleDataOfDIS[n-1].wchO2);
		WriteIniString(ini, strTime, L"OilTemperature", pIdleDataOfDIS[n-1].wchOilTemperature);
		WriteIniString(ini, strTime, L"RelativeHumidity", pIdleDataOfDIS[n-1].wchRelativeHumidity);
		WriteIniString(ini, strTime, L"Remark", pIdleDataOfDIS[n-1].wchRemark);
		WriteIniString(ini, strTime, L"ReportNumber", pIdleDataOfDIS[n-1].wchReportNumber);
		WriteIniString(ini, strTime, L"Reserved1", pIdleDataOfDIS[n-1].wchReserved1);
		WriteIniString(ini, strTime, L"Reserved2", pIdleDataOfDIS[n-1].wchReserved2);
		WriteIniString(ini, strTime, L"Reserved3", pIdleDataOfDIS[n-1].wchReserved3);
		WriteIniString(ini, strTime, L"SamplingTime", pIdleDataOfDIS[n-1].wchSamplingTime);
		WriteIniString(ini, strTime, L"State", pIdleDataOfDIS[n-1].wchState);
		WriteIniString(ini, strTime, L"Time", pIdleDataOfDIS[n-1].wchTime);
	}
	WriteIniString(ini, L"Sum", L"Total", strTime);

	ini.SaveFile(wchPath);
}

void SetIniRealTimeDataOfDISEx(std::list<SRealTimeDataOfDIS> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfDIS.ini"*/)
{
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath, true))
	{
		return;
	}
	CSimpleIni ini(true);
	wchar_t wchTotal[NH_INT_TO_WCHART];
	wsprintf(wchTotal, L"%d", lsRTData.size());
	WriteIniString(ini, L"Sum", L"Total", wchTotal);
	int i(1);
	for (list<SRealTimeDataOfDIS>::iterator ir=lsRTData.begin(); ir!=lsRTData.end(); ++ir)
	{
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i++);
		WriteIniString(ini, wchSection, L"RunningNumber", ir->strRunningNumber.c_str());
		WriteIniString(ini, wchSection, L"ReportNumber", ir->strReportNumber.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeCode", ir->strFuelTypeCode.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeName", ir->strFuelTypeName.c_str());
		WriteIniString(ini, wchSection, L"Time", ir->strTime.c_str());
		WriteIniString(ini, wchSection, L"SamplingTime", ir->strSamplingTime.c_str());
		WriteIniString(ini, wchSection, L"State", ir->strState.c_str());
		WriteIniString(ini, wchSection, L"HC", ir->strHC.c_str());
		WriteIniString(ini, wchSection, L"CO", ir->strCO.c_str());
		WriteIniString(ini, wchSection, L"O2", ir->strO2.c_str());
		WriteIniString(ini, wchSection, L"CO2", ir->strCO2.c_str());
		WriteIniString(ini, wchSection, L"NO", ir->strNO.c_str());
		WriteIniString(ini, wchSection, L"EngineRev", ir->strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OilTemperature", ir->strOilTemperature.c_str());
		WriteIniString(ini, wchSection, L"Lambda", ir->strLambda.c_str());
		WriteIniString(ini, wchSection, L"EnvironmentalTemperature", ir->strEnvironmentalTemperature.c_str());
		WriteIniString(ini, wchSection, L"RelativeHumidity", ir->strRelativeHumidity.c_str());
		WriteIniString(ini, wchSection, L"AtmosphericPressure", ir->strAtmosphericPressure.c_str());
		//WriteIniString(ini, wchSection, L"sOBDRTData", ir->	sOBDRTData.c_str());
		// OBD数据流
		WriteIniString(ini, wchSection, L"OBDRTData_Velocity", ir->sOBDRTData.strVelocity.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CoolantTemp", ir->sOBDRTData.strCoolantTemp.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineRev", ir->sOBDRTData.strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_SolarTermDoorPosition", ir->sOBDRTData.strSolarTermDoorPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ThrottleOpening", ir->sOBDRTData.strThrottleOpening.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CalculationLoad", ir->sOBDRTData.strCalculationLoad.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorV", ir->sOBDRTData.strForwardSensorV.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorI", ir->sOBDRTData.strForwardSensorI.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_AirCoefficient", ir->sOBDRTData.strAirCoefficient.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAF", ir->sOBDRTData.strMAF.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAP", ir->sOBDRTData.strMAP.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineOutputPower", ir->sOBDRTData.strEngineOutputPower.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ChargeAirPressure", ir->sOBDRTData.strChargeAirPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelConsumption", ir->sOBDRTData.strFuelConsumption.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_NOXConcentration", ir->sOBDRTData.strNOXConcentration.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_UreaInjectionVolume", ir->sOBDRTData.strUreaInjectionVolume.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGT", ir->sOBDRTData.strEGT.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_DPFDifferentialPressure", ir->sOBDRTData.strDPFDifferentialPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGRPosition", ir->sOBDRTData.strEGRPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelDeliveryPressure", ir->sOBDRTData.strFuelDeliveryPressure.c_str());

		WriteIniString(ini, wchSection, L"Remark", ir->strRemark.c_str());
		WriteIniString(ini, wchSection, L"Reserved1", ir->strReserved1.c_str());
		WriteIniString(ini, wchSection, L"Reserved2", ir->strReserved2.c_str());
		WriteIniString(ini, wchSection, L"Reserved3", ir->strReserved3.c_str());
	}
	ini.SaveFile(wchPath);
}

void SetIniRealTimeDataOfVMASEx(std::list<SRealTimeDataOfVMAS> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfVMAS.ini"*/)
{
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath, true))
	{
		return;
	}
	CSimpleIni ini(true);
	wchar_t wchTotal[NH_INT_TO_WCHART];
	wsprintf(wchTotal, L"%d", lsRTData.size());
	WriteIniString(ini, L"Sum", L"Total", wchTotal);
	int i(1);
	for (list<SRealTimeDataOfVMAS>::iterator ir=lsRTData.begin(); ir!=lsRTData.end(); ++ir)
	{
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i++);
		WriteIniString(ini, wchSection, L"RunningNumber", ir->strRunningNumber.c_str());
		WriteIniString(ini, wchSection, L"ReportNumber", ir->strReportNumber.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeCode", ir->strFuelTypeCode.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeName", ir->strFuelTypeName.c_str());
		WriteIniString(ini, wchSection, L"Time", ir->strTime.c_str());
		WriteIniString(ini, wchSection, L"SamplingTime", ir->strSamplingTime.c_str());
		WriteIniString(ini, wchSection, L"Velocity", ir->strVelocity.c_str());
		WriteIniString(ini, wchSection, L"Power", ir->strPower.c_str());
		WriteIniString(ini, wchSection, L"Force", ir->strForce.c_str());
		WriteIniString(ini, wchSection, L"IHP", ir->strIHP.c_str());
		WriteIniString(ini, wchSection, L"PLHP", ir->strPLHP.c_str());
		WriteIniString(ini, wchSection, L"HC", ir->strHC.c_str());
		WriteIniString(ini, wchSection, L"CO", ir->strCO.c_str());
		WriteIniString(ini, wchSection, L"NO", ir->strNO.c_str());
		WriteIniString(ini, wchSection, L"NO2", ir->strNO2.c_str());
		WriteIniString(ini, wchSection, L"NOx", ir->strNOx.c_str());
		WriteIniString(ini, wchSection, L"O2", ir->strO2.c_str());
		WriteIniString(ini, wchSection, L"CO2", ir->strCO2.c_str());
		WriteIniString(ini, wchSection, L"EngineRev", ir->strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"Lambda", ir->strLambda.c_str());
		WriteIniString(ini, wchSection, L"OilTemperature", ir->strOilTemperature.c_str());
		WriteIniString(ini, wchSection, L"O2OfFlowmeter", ir->strO2OfFlowmeter.c_str());
		WriteIniString(ini, wchSection, L"O2OfEnvironment", ir->strO2OfEnvironment.c_str());
		WriteIniString(ini, wchSection, L"HCm", ir->strHCm.c_str());
		WriteIniString(ini, wchSection, L"COm", ir->strCOm.c_str());
		WriteIniString(ini, wchSection, L"NOm", ir->strNOm.c_str());
		WriteIniString(ini, wchSection, L"NO2m", ir->strNO2m.c_str());
		WriteIniString(ini, wchSection, L"NOxm", ir->strNOxm.c_str());
		WriteIniString(ini, wchSection, L"O2m", ir->strO2m.c_str());
		WriteIniString(ini, wchSection, L"CO2m", ir->strCO2m.c_str());
		WriteIniString(ini, wchSection, L"FuleConsumption", ir->strFuleConsumption.c_str());
		WriteIniString(ini, wchSection, L"PressureOfFlowmeter", ir->strPressureOfFlowmeter.c_str());
		WriteIniString(ini, wchSection, L"TemperatureOfFlowmeter", ir->strTemperatureOfFlowmeter.c_str());
		WriteIniString(ini, wchSection, L"DR", ir->strDR.c_str());
		WriteIniString(ini, wchSection, L"ActualFluxOfGas", ir->strActualFluxOfGas.c_str());
		WriteIniString(ini, wchSection, L"StandardFluxOfGas", ir->strStandardFluxOfGas.c_str());
		WriteIniString(ini, wchSection, L"FluxOfExhaust", ir->strFluxOfExhaust.c_str());
		WriteIniString(ini, wchSection, L"EnvironmentalTemperature", ir->strEnvironmentalTemperature.c_str());
		WriteIniString(ini, wchSection, L"RelativeHumidity", ir->strRelativeHumidity.c_str());
		WriteIniString(ini, wchSection, L"AtmosphericPressure", ir->strAtmosphericPressure.c_str());
		WriteIniString(ini, wchSection, L"HumidityCorrectionFactor", ir->strHumidityCorrectionFactor.c_str());
		WriteIniString(ini, wchSection, L"DilutionCorrectionFactor", ir->strDilutionCorrectionFactor.c_str());
		WriteIniString(ini, wchSection, L"InertiaSimulationError", ir->strInertiaSimulationError.c_str());

		//WriteIniString(ini, wchSection, L"sOBDRTData", ir->	sOBDRTData.c_str());
		// OBD数据流
		WriteIniString(ini, wchSection, L"OBDRTData_Velocity", ir->sOBDRTData.strVelocity.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CoolantTemp", ir->sOBDRTData.strCoolantTemp.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineRev", ir->sOBDRTData.strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_SolarTermDoorPosition", ir->sOBDRTData.strSolarTermDoorPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ThrottleOpening", ir->sOBDRTData.strThrottleOpening.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CalculationLoad", ir->sOBDRTData.strCalculationLoad.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorV", ir->sOBDRTData.strForwardSensorV.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorI", ir->sOBDRTData.strForwardSensorI.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_AirCoefficient", ir->sOBDRTData.strAirCoefficient.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAF", ir->sOBDRTData.strMAF.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAP", ir->sOBDRTData.strMAP.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineOutputPower", ir->sOBDRTData.strEngineOutputPower.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ChargeAirPressure", ir->sOBDRTData.strChargeAirPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelConsumption", ir->sOBDRTData.strFuelConsumption.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_NOXConcentration", ir->sOBDRTData.strNOXConcentration.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_UreaInjectionVolume", ir->sOBDRTData.strUreaInjectionVolume.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGT", ir->sOBDRTData.strEGT.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_DPFDifferentialPressure", ir->sOBDRTData.strDPFDifferentialPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGRPosition", ir->sOBDRTData.strEGRPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelDeliveryPressure", ir->sOBDRTData.strFuelDeliveryPressure.c_str());

		WriteIniString(ini, wchSection, L"Remark", ir->strRemark.c_str());
		WriteIniString(ini, wchSection, L"Reserved1", ir->strReserved1.c_str());
		WriteIniString(ini, wchSection, L"Reserved2", ir->strReserved2.c_str());
		WriteIniString(ini, wchSection, L"Reserved3", ir->strReserved3.c_str());
	}
	ini.SaveFile(wchPath);
}

void SetIniRealTimeDataOfFSUNHTEx(std::list<SRealTimeDataOfFSUNHT> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfFSUNHT.ini"*/)
{
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath, true))
	{
		return;
	}
	CSimpleIni ini(true);
	wchar_t wchTotal[NH_INT_TO_WCHART];
	wsprintf(wchTotal, L"%d", lsRTData.size());
	WriteIniString(ini, L"Sum", L"Total", wchTotal);
	int i(1);
	for (list<SRealTimeDataOfFSUNHT>::iterator ir=lsRTData.begin(); ir!=lsRTData.end(); ++ir)
	{
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i++);
		WriteIniString(ini, wchSection, L"RunningNumber", ir->strRunningNumber.c_str());
		WriteIniString(ini, wchSection, L"ReportNumber", ir->strReportNumber.c_str());
		WriteIniString(ini, wchSection, L"Order", ir->strOrder.c_str());
		WriteIniString(ini, wchSection, L"Time", ir->strTime.c_str());
		WriteIniString(ini, wchSection, L"SamplingTime", ir->strSamplingTime.c_str());
		WriteIniString(ini, wchSection, L"K", ir->strK.c_str());
		WriteIniString(ini, wchSection, L"N", ir->strN.c_str());
		WriteIniString(ini, wchSection, L"EngineRev", ir->strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OilTemperature", ir->strOilTemperature.c_str());
		WriteIniString(ini, wchSection, L"EnvironmentalTemperature", ir->strEnvironmentalTemperature.c_str());
		WriteIniString(ini, wchSection, L"RelativeHumidity", ir->strRelativeHumidity.c_str());
		WriteIniString(ini, wchSection, L"AtmosphericPressure", ir->strAtmosphericPressure.c_str());
		//WriteIniString(ini, wchSection, L"sOBDRTData", ir->	sOBDRTData.c_str());
		// OBD数据流
		WriteIniString(ini, wchSection, L"OBDRTData_Velocity", ir->sOBDRTData.strVelocity.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CoolantTemp", ir->sOBDRTData.strCoolantTemp.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineRev", ir->sOBDRTData.strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_SolarTermDoorPosition", ir->sOBDRTData.strSolarTermDoorPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ThrottleOpening", ir->sOBDRTData.strThrottleOpening.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CalculationLoad", ir->sOBDRTData.strCalculationLoad.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorV", ir->sOBDRTData.strForwardSensorV.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorI", ir->sOBDRTData.strForwardSensorI.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_AirCoefficient", ir->sOBDRTData.strAirCoefficient.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAF", ir->sOBDRTData.strMAF.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAP", ir->sOBDRTData.strMAP.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineOutputPower", ir->sOBDRTData.strEngineOutputPower.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ChargeAirPressure", ir->sOBDRTData.strChargeAirPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelConsumption", ir->sOBDRTData.strFuelConsumption.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_NOXConcentration", ir->sOBDRTData.strNOXConcentration.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_UreaInjectionVolume", ir->sOBDRTData.strUreaInjectionVolume.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGT", ir->sOBDRTData.strEGT.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_DPFDifferentialPressure", ir->sOBDRTData.strDPFDifferentialPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGRPosition", ir->sOBDRTData.strEGRPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelDeliveryPressure", ir->sOBDRTData.strFuelDeliveryPressure.c_str());

		WriteIniString(ini, wchSection, L"Remark", ir->strRemark.c_str());
		WriteIniString(ini, wchSection, L"Reserved1", ir->strReserved1.c_str());
		WriteIniString(ini, wchSection, L"Reserved2", ir->strReserved2.c_str());
		WriteIniString(ini, wchSection, L"Reserved3", ir->strReserved3.c_str());
	}
	ini.SaveFile(wchPath);
}

void SetIniRealTimeDataOfLUGDOWNEx(std::list<SRealTimeDataOfLUGDOWN> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfLUGDOWN.ini"*/)
{
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath, true))
	{
		return;
	}
	CSimpleIni ini(true);
	wchar_t wchTotal[NH_INT_TO_WCHART];
	wsprintf(wchTotal, L"%d", lsRTData.size());
	WriteIniString(ini, L"Sum", L"Total", wchTotal);
	int i(1);
	for (list<SRealTimeDataOfLUGDOWN>::iterator ir=lsRTData.begin(); ir!=lsRTData.end(); ++ir)
	{
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i++);
		WriteIniString(ini, wchSection, L"RunningNumber", ir->strRunningNumber.c_str());
		WriteIniString(ini, wchSection, L"ReportNumber", ir->strReportNumber.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeCode", ir->strFuelTypeCode.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeName", ir->strFuelTypeName.c_str());
		WriteIniString(ini, wchSection, L"Time", ir->strTime.c_str());
		WriteIniString(ini, wchSection, L"SamplingTime", ir->strSamplingTime.c_str());
		WriteIniString(ini, wchSection, L"Velocity", ir->strVelocity.c_str());
		WriteIniString(ini, wchSection, L"Power", ir->strPower.c_str());
		WriteIniString(ini, wchSection, L"Force", ir->strForce.c_str());
		WriteIniString(ini, wchSection, L"IHP", ir->strIHP.c_str());
		WriteIniString(ini, wchSection, L"PLHP", ir->strPLHP.c_str());
		WriteIniString(ini, wchSection, L"State", ir->strState.c_str());
		WriteIniString(ini, wchSection, L"K", ir->strK.c_str());
		WriteIniString(ini, wchSection, L"N", ir->strN.c_str());
		WriteIniString(ini, wchSection, L"EngineRev", ir->strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OilTemperature", ir->strOilTemperature.c_str());
		WriteIniString(ini, wchSection, L"EnvironmentalTemperature", ir->strEnvironmentalTemperature.c_str());
		WriteIniString(ini, wchSection, L"RelativeHumidity", ir->strRelativeHumidity.c_str());
		WriteIniString(ini, wchSection, L"AtmosphericPressure", ir->strAtmosphericPressure.c_str());
		WriteIniString(ini, wchSection, L"PowerCorrectionFactor", ir->strPowerCorrectionFactor.c_str());
		WriteIniString(ini, wchSection, L"CO", ir->strCO.c_str());
		WriteIniString(ini, wchSection, L"HC", ir->strHC.c_str());
		WriteIniString(ini, wchSection, L"NO", ir->strNO.c_str());
		WriteIniString(ini, wchSection, L"NO2", ir->strNO2.c_str());
		WriteIniString(ini, wchSection, L"NOx", ir->strNOx.c_str());
		WriteIniString(ini, wchSection, L"CO2", ir->strCO2.c_str());
		WriteIniString(ini, wchSection, L"O2", ir->strO2.c_str());
		WriteIniString(ini, wchSection, L"HumidityCorrectionFactor", ir->strHumidityCorrectionFactor.c_str());

		//WriteIniString(ini, wchSection, L"sOBDRTData", ir->	sOBDRTData.c_str());
		// OBD数据流
		WriteIniString(ini, wchSection, L"OBDRTData_Velocity", ir->sOBDRTData.strVelocity.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CoolantTemp", ir->sOBDRTData.strCoolantTemp.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineRev", ir->sOBDRTData.strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_SolarTermDoorPosition", ir->sOBDRTData.strSolarTermDoorPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ThrottleOpening", ir->sOBDRTData.strThrottleOpening.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CalculationLoad", ir->sOBDRTData.strCalculationLoad.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorV", ir->sOBDRTData.strForwardSensorV.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorI", ir->sOBDRTData.strForwardSensorI.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_AirCoefficient", ir->sOBDRTData.strAirCoefficient.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAF", ir->sOBDRTData.strMAF.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAP", ir->sOBDRTData.strMAP.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineOutputPower", ir->sOBDRTData.strEngineOutputPower.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ChargeAirPressure", ir->sOBDRTData.strChargeAirPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelConsumption", ir->sOBDRTData.strFuelConsumption.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_NOXConcentration", ir->sOBDRTData.strNOXConcentration.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_UreaInjectionVolume", ir->sOBDRTData.strUreaInjectionVolume.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGT", ir->sOBDRTData.strEGT.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_DPFDifferentialPressure", ir->sOBDRTData.strDPFDifferentialPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGRPosition", ir->sOBDRTData.strEGRPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelDeliveryPressure", ir->sOBDRTData.strFuelDeliveryPressure.c_str());

		WriteIniString(ini, wchSection, L"Remark", ir->strRemark.c_str());
		WriteIniString(ini, wchSection, L"Reserved1", ir->strReserved1.c_str());
		WriteIniString(ini, wchSection, L"Reserved2", ir->strReserved2.c_str());
		WriteIniString(ini, wchSection, L"Reserved3", ir->strReserved3.c_str());
	}
	ini.SaveFile(wchPath);
}

void GetIniRealTimeDataOfDISEx(std::list<SRealTimeDataOfDIS> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfDIS.ini"*/)
{
	std::vector<SRealTimeDataOfDIS> vtRTData;
	GetIniRealTimeDataOfDISEx(vtRTData, pwchSaveFileName);
	for(UINT i=0; i<vtRTData.size(); i++)
	{
		lsRTData.push_back(vtRTData[i]);
	}
}
void GetIniRealTimeDataOfVMASEx(std::list<SRealTimeDataOfVMAS> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfVMAS.ini"*/)
{
	std::vector<SRealTimeDataOfVMAS> vtRTData;
	GetIniRealTimeDataOfVMASEx(vtRTData, pwchSaveFileName);
	for(UINT i=0; i<vtRTData.size(); i++)
	{
		lsRTData.push_back(vtRTData[i]);
	}
}
void GetIniRealTimeDataOfFSUNHTEx(std::list<SRealTimeDataOfFSUNHT> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfFSUNHT.ini"*/)
{
	std::vector<SRealTimeDataOfFSUNHT> vtRTData;
	GetIniRealTimeDataOfFSUNHTEx(vtRTData, pwchSaveFileName);
	for(UINT i=0; i<vtRTData.size(); i++)
	{
		lsRTData.push_back(vtRTData[i]);
	}
}
void GetIniRealTimeDataOfLUGDOWNEx(std::list<SRealTimeDataOfLUGDOWN> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfLUGDOWN.ini"*/)
{
	std::vector<SRealTimeDataOfLUGDOWN> vtRTData;
	GetIniRealTimeDataOfLUGDOWNEx(vtRTData, pwchSaveFileName);
	for(UINT i=0; i<vtRTData.size(); i++)
	{
		lsRTData.push_back(vtRTData[i]);
	}
}
void GetIniRealTimeDataOfASMEx(std::list<SRealTimeDataOfASM> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfASM.ini"*/)
{
	std::vector<SRealTimeDataOfASM> vtRTData;
	GetIniRealTimeDataOfASMEx(vtRTData, pwchSaveFileName);
	for(UINT i=0; i<vtRTData.size(); i++)
	{
		lsRTData.push_back(vtRTData[i]);
	}
}

void SetIniResultOfAnaGasChk(const DEMARCATIONRESULTOFANAGASCHK &IniResultOfAnaGasChk, wchar_t *pwchSaveFileName/*=L"ResultOfAnaGasChk.ini"*/)
{
	// 获取LimitOfDIS.ini路径
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath, true))
	{
		return;
	}

	CSimpleIni ini(true);

	// 写ResultOfAnaGasChk.ini信息
	WriteIniString(ini, L"ResultOfAnaGasChk", L"AutoID", IniResultOfAnaGasChk.wchAutoID);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"DemarcationNumber", IniResultOfAnaGasChk.wchDemarcationNumber);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"PEFMeasuredValue", IniResultOfAnaGasChk.wchPEFMeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"C3H8StandardValue", IniResultOfAnaGasChk.wchC3H8StandardValue);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"SkipHCJudgement", IniResultOfAnaGasChk.wchSkipHCJudgement);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCMeasuredValue", IniResultOfAnaGasChk.wchHCMeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCStandardValue", IniResultOfAnaGasChk.wchHCStandardValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCAE", IniResultOfAnaGasChk.wchHCAE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCAELimit", IniResultOfAnaGasChk.wchHCAELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCAEPass", IniResultOfAnaGasChk.wchHCAEPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCRE", IniResultOfAnaGasChk.wchHCRE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCRELimit", IniResultOfAnaGasChk.wchHCRELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCREPass", IniResultOfAnaGasChk.wchHCREPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"HCPass", IniResultOfAnaGasChk.wchHCPass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"SkipCOJudgement", IniResultOfAnaGasChk.wchSkipCOJudgement);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COMeasuredValue", IniResultOfAnaGasChk.wchCOMeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COStandardValue", IniResultOfAnaGasChk.wchCOStandardValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COAE", IniResultOfAnaGasChk.wchCOAE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COAELimit", IniResultOfAnaGasChk.wchCOAELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COAEPass", IniResultOfAnaGasChk.wchCOAEPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CORE", IniResultOfAnaGasChk.wchCORE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CORELimit", IniResultOfAnaGasChk.wchCORELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COREPass", IniResultOfAnaGasChk.wchCOREPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"COPass", IniResultOfAnaGasChk.wchCOPass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"SkipCO2Judgement", IniResultOfAnaGasChk.wchSkipCO2Judgement);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2MeasuredValue", IniResultOfAnaGasChk.wchCO2MeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2StandardValue", IniResultOfAnaGasChk.wchCO2StandardValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2AE", IniResultOfAnaGasChk.wchCO2AE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2AELimit", IniResultOfAnaGasChk.wchCO2AELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2AEPass", IniResultOfAnaGasChk.wchCO2AEPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2RE", IniResultOfAnaGasChk.wchCO2RE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2RELimit", IniResultOfAnaGasChk.wchCO2RELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2REPass", IniResultOfAnaGasChk.wchCO2REPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"CO2Pass", IniResultOfAnaGasChk.wchCO2Pass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"SkipNOJudgement", IniResultOfAnaGasChk.wchSkipNOJudgement);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOMeasuredValue", IniResultOfAnaGasChk.wchNOMeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOStandardValue", IniResultOfAnaGasChk.wchNOStandardValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOAE", IniResultOfAnaGasChk.wchNOAE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOAELimit", IniResultOfAnaGasChk.wchNOAELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOAEPass", IniResultOfAnaGasChk.wchNOAEPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NORE", IniResultOfAnaGasChk.wchNORE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NORELimit", IniResultOfAnaGasChk.wchNORELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOREPass", IniResultOfAnaGasChk.wchNOREPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NOPass", IniResultOfAnaGasChk.wchNOPass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"SkipNO2Judgement", IniResultOfAnaGasChk.wchSkipNO2Judgement);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2MeasuredValue", IniResultOfAnaGasChk.wchNO2MeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2StandardValue", IniResultOfAnaGasChk.wchNO2StandardValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2AE", IniResultOfAnaGasChk.wchNO2AE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2AELimit", IniResultOfAnaGasChk.wchNO2AELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2AEPass", IniResultOfAnaGasChk.wchNO2AEPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2RE", IniResultOfAnaGasChk.wchNO2RE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2RELimit", IniResultOfAnaGasChk.wchNO2RELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2REPass", IniResultOfAnaGasChk.wchNO2REPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"NO2Pass", IniResultOfAnaGasChk.wchNO2Pass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"SkipO2Judgement", IniResultOfAnaGasChk.wchSkipO2Judgement);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2MeasuredValue", IniResultOfAnaGasChk.wchO2MeasuredValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2StandardValue", IniResultOfAnaGasChk.wchO2StandardValue);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2AE", IniResultOfAnaGasChk.wchO2AE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2AELimit", IniResultOfAnaGasChk.wchO2AELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2AEPass", IniResultOfAnaGasChk.wchO2AEPass);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2RE", IniResultOfAnaGasChk.wchO2RE);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2RELimit", IniResultOfAnaGasChk.wchO2RELimit);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"O2REPass", IniResultOfAnaGasChk.wchO2REPass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"Pass", IniResultOfAnaGasChk.wchPass);

	WriteIniString(ini, L"ResultOfAnaGasChk", L"Remark", IniResultOfAnaGasChk.wchRemark);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"Reserved1", IniResultOfAnaGasChk.wchReserved1);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"Reserved2", IniResultOfAnaGasChk.wchReserved2);
	WriteIniString(ini, L"ResultOfAnaGasChk", L"Reserved3", IniResultOfAnaGasChk.wchReserved3);

	ini.SaveFile(wchPath);
}


void SetIniRealTimeDataOfASMEx(std::list<SRealTimeDataOfASM> &lsRTData, wchar_t *pwchSaveFileName/*=L"RealTimeDataOfASM.ini"*/)//导出实时数据
{
	wchar_t wchPath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", pwchSaveFileName, wchPath, true))
	{
		return;
	}
	CSimpleIni ini(true);
	wchar_t wchTotal[NH_INT_TO_WCHART];
	wsprintf(wchTotal, L"%d", lsRTData.size());
	WriteIniString(ini, L"Sum", L"Total", wchTotal);
	int i(1);
	for (list<SRealTimeDataOfASM>::iterator ir=lsRTData.begin(); ir!=lsRTData.end(); ++ir)
	{
		wchar_t wchSection[NH_INT_TO_WCHART];
		wsprintf(wchSection, L"%d", i++);
		WriteIniString(ini, wchSection, L"RunningNumber", ir->strRunningNumber.c_str());
		WriteIniString(ini, wchSection, L"ReportNumber", ir->strReportNumber.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeCode", ir->strFuelTypeCode.c_str());
		WriteIniString(ini, wchSection, L"FuelTypeName", ir->strFuelTypeName.c_str());
		WriteIniString(ini, wchSection, L"Time", ir->strTime.c_str());
		WriteIniString(ini, wchSection, L"SamplingTime", ir->strSamplingTime.c_str());
		WriteIniString(ini, wchSection, L"Velocity", ir->strVelocity.c_str());
		WriteIniString(ini, wchSection, L"Power", ir->strPower.c_str());
		WriteIniString(ini, wchSection, L"Force", ir->strForce.c_str());
		WriteIniString(ini, wchSection, L"IHP", ir->strIHP.c_str());
		WriteIniString(ini, wchSection, L"PLHP", ir->strPLHP.c_str());
		WriteIniString(ini, wchSection, L"State", ir->strState.c_str());
		WriteIniString(ini, wchSection, L"HC", ir->strHC.c_str());
		WriteIniString(ini, wchSection, L"CO", ir->strCO.c_str());
		WriteIniString(ini, wchSection, L"NO", ir->strNO.c_str());
		WriteIniString(ini, wchSection, L"O2", ir->strO2.c_str());
		WriteIniString(ini, wchSection, L"CO2", ir->strCO2.c_str());
		WriteIniString(ini, wchSection, L"EngineRev", ir->strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OilTemperature", ir->strOilTemperature.c_str());
		WriteIniString(ini, wchSection, L"Lambda", ir->strLambda.c_str());
		WriteIniString(ini, wchSection, L"EnvironmentalTemperature", ir->strEnvironmentalTemperature.c_str());
		WriteIniString(ini, wchSection, L"RelativeHumidity", ir->strRelativeHumidity.c_str());
		WriteIniString(ini, wchSection, L"AtmosphericPressure", ir->strAtmosphericPressure.c_str());
		WriteIniString(ini, wchSection, L"HumidityCorrectionFactor", ir->strHumidityCorrectionFactor.c_str());
		WriteIniString(ini, wchSection, L"DilutionCorrectionFactor", ir->strDilutionCorrectionFactor.c_str());
		WriteIniString(ini, wchSection, L"IsAdoptedResult", ir->strIsAdoptedResult.c_str());

		//WriteIniString(ini, wchSection, L"sOBDRTData", ir->	sOBDRTData.c_str());
		// OBD数据流
		WriteIniString(ini, wchSection, L"OBDRTData_Velocity", ir->sOBDRTData.strVelocity.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CoolantTemp", ir->sOBDRTData.strCoolantTemp.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineRev", ir->sOBDRTData.strEngineRev.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_SolarTermDoorPosition", ir->sOBDRTData.strSolarTermDoorPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ThrottleOpening", ir->sOBDRTData.strThrottleOpening.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_CalculationLoad", ir->sOBDRTData.strCalculationLoad.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorV", ir->sOBDRTData.strForwardSensorV.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ForwardSensorI", ir->sOBDRTData.strForwardSensorI.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_AirCoefficient", ir->sOBDRTData.strAirCoefficient.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAF", ir->sOBDRTData.strMAF.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_MAP", ir->sOBDRTData.strMAP.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EngineOutputPower", ir->sOBDRTData.strEngineOutputPower.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_ChargeAirPressure", ir->sOBDRTData.strChargeAirPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelConsumption", ir->sOBDRTData.strFuelConsumption.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_NOXConcentration", ir->sOBDRTData.strNOXConcentration.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_UreaInjectionVolume", ir->sOBDRTData.strUreaInjectionVolume.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGT", ir->sOBDRTData.strEGT.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_DPFDifferentialPressure", ir->sOBDRTData.strDPFDifferentialPressure.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_EGRPosition", ir->sOBDRTData.strEGRPosition.c_str());
		WriteIniString(ini, wchSection, L"OBDRTData_FuelDeliveryPressure", ir->sOBDRTData.strFuelDeliveryPressure.c_str());

		WriteIniString(ini, wchSection, L"Remark", ir->strRemark.c_str());
		WriteIniString(ini, wchSection, L"Reserved1", ir->strReserved1.c_str());
		WriteIniString(ini, wchSection, L"Reserved2", ir->strReserved2.c_str());
		WriteIniString(ini, wchSection, L"Reserved3", ir->strReserved3.c_str());
	}
	ini.SaveFile(wchPath);
}