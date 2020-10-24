/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHStructDef.h
* 文件标识：
* 描述：数据结构，对应于数据库NHModeServerDB
*
* 版本：1.0
* 作者：HuaBei
* 日期：2010-01-01
*
* 历史信息：
*
* 版本：1.0.27
* 日期：2014-05-04
* 作者：Lihy
* 描述：在TestLog中增加SRM字段，用于记录是否单次合格
*
* 版本：1.0.26
* 日期：2013-08-02
* 作者：Chc
* 描述：在检测过程结果表添加流水号
*
* 版本：1.0.25
* 日期：2013-05-22
* 作者：Duanjf
* 描述：添加SystemLog相关数据结构
*
* 版本：1.0.24
* 日期：2011-09-28
* 作者：Raylee
* 描述：添加摩托车双怠速相关数据结构
*
* 版本：1.0.23
* 日期：2011-05-02
* 作者：Raylee
* 描述：修正宏NH_TIME_TO_WCHART，定义为30
*
* 版本：1.0.22
* 日期：2011-04-15
* 作者：Ming
* 描述：修改数据结构<VehicleInfo>，添加OwnerCharacter、OwnerDistrict、LicenseDepartment、LicenseIssueDate、LicenseValidDate字段
*		修改数据结构<TestLog>，添加IsSentEP、OwnerCharacter、OwnerDistrict、LicenseDepartment、LicenseIssueDate、LicenseValidDate字段
*
* 版本：1.0.21
* 日期：2011-04-02
* 作者：Raylee
* 描述：修改数据结构<SystemParameters>，添加PeriodOfEnvParamMeasurerCheck字段
*
* 版本：1.0.20
* 日期：2011-01-10
* 作者：Raylee
* 描述：修改数据结构<DemarcationResultOfDynForceChk>
*
* 版本：1.0.19
* 日期：2011-01-05
* 作者：Raylee
* 描述：完善数据结构<DemarcationResultOfDynForceChk>
*
*
* 版本：1.0.18
* 日期：2010-12-11
* 作者：Ming
* 描述：1)在数据结构<BillInfo>中，添加以下成员：wchNumberOfTestPeriodAVFSU、wchNumberOfTestPeriodMI
*		2)在数据结构<TestLog>中，添加以下成员：wchNumberOfTestPeriodAVFSU、wchNumberOfTestPeriodMI
*		3)在数据结构<TestAmount>中，添加以下成员：wchAVFSUAmount、wchMIAmount
*
* 版本：1.0.17
* 日期：2010-12-06
* 作者：Ming
* 描述：1)添加摩托车怠速实时数据<RealTimeDataOfMI>数据结构
*       2)添加摩托车怠速结果数据<ResultOfMI>数据结构
*	    3)添加农用车结果数据<ResultOfAVFSU>数据结构
*
* 版本：1.0.16
* 日期：2010-09-21
* 作者：Ming
* 描述：1)在数据结构<RealTimeDataOfASM>、<RealTimeDataOfLUGDOWN>、<RealTimeDataOfVAMS>中，添加以下成员：wchPLHP、wchIHP
*
* 版本：1.0.15
* 日期：2010-09-20
* 作者：Ming
* 描述：1)在数据结构<VehicleInfo>中，添加以下成员：wchEngineStroke、wchEmissionStandard、wchAnnualTestPeriod、wchHasOBD
*		2)在数据结构<TestLog>中，添加以下成员：wchEngineStroke、wchEmissionStandard、wchAnnualTestPeriod、wchHasOBD、wchLineIPAddress、wchLineHardwareSN
*		3)在数据结构<VehicleModel>中，添加以下成员：wchEngineStroke、wchEmissionStandard、wchHasOBD
*		4)在数据结构<LineInfo>中，添加以下成员：wchLineIPAddress、wchLineHardwareSN
*		5)在数据结构<RealTimeDataOfDIS>和<RealTimeDataOfASM>中，分别添加以下成员：wchState
*
* 版本：1.0.14
* 日期：2010-09-06
* 作者：Ming
* 描述：在数据结构<SystemParameters>中，添加以下成员：wchPeriodOfEnvParamMeasurerCheck、wchTestPeriod
*
* 版本：1.0.13
* 日期：2010-08-13
* 作者：Ming
* 描述：添加不透光实时数据<RealTimeDataOfFSUNHT>数据结构
*
* 版本：1.0.12
* 日期：2010-07-17
* 作者：Ming
* 描述：添加检测次数统计<TestAmount>数据结构
*
* 版本：1.0.11
* 日期：2010-07-11
* 作者：Raylee
* 描述：修正数据结构BillInfo，添加wchTestCharacter
*
* 版本：1.0.10
* 日期：2010-07-07
* 作者：Ming
* 描述：添加数据结构DemarcationRealTimeDataOfAnaGasChk,DemarcationRealTimeDataOfDynConstLoad
*
* 版本：1.0.9
* 日期：2010-05-27
* 作者：Raylee
* 描述：添加数据结构DemarcationResultOfFloFluxCal、DemarcationResultOfFloFluxChk、DemarcationResultOfFloDilO2Cal、DemarcationResultOfFloPresCal
*
* 版本：1.0.8
* 日期：2010-05-14
* 作者：Raylee
* 描述：添加数据结构DemarcationResultOfDynConstLoad、DemarcationResultOfDynForceCal、DemarcationResultOfDynForceChk、DemarcationResultOfDynPLHP
*
* 版本：1.0.7
* 日期：2010-04-23
* 作者：Cui
* 描述：修改ResultOfDIS，添加以下成员：wchLowerLimitOfLambda，wchUpperLimitOfLambda，wchLimitOfLambda_EDP
*
* 版本：1.0.6
* 日期：2010-04-14
* 作者：Raylee
* 描述：改EquCalChkInfo结构Data和Error长度
*
* 版本：1.0.5
* 日期：2010-04-12
* 作者：Raylee
* 描述：1、更改UseInfo结构定义，前缀ch改为wch
*		2、修正StationInfo和TestLog中wchStationLicenseStartDate、wchStationLicenseEndDate定义的长度
*
* 版本：1.0.4
* 日期：2010-04-10
* 作者：Raylee
* 描述：根据数据库表，更改DemarcationResultOfSmoChk
*
* 版本：1.0.3
* 日期：2010-04-10
* 作者：Raylee
* 描述：根据TestLog和VehicleInfo表字段PassengerVehicle，为TESTLOG和VEHICLEINFO添加wchPassengerVehicle
*
* 版本：1.0.2
* 日期：2010-04-08
* 作者：Raylee
* 描述：添加数据结构DemarcationLog、DemarcationResultOfAnaGasCal、DemarcationResultOfAnaGasChk、DemarcationResultOfOpaCalChk、DemarcationResultOfSmoChk
*
* 版本：1.0.1
* 日期：2010-04-06
* 作者：Cui
* 描述：添加TestParametersOfxxx，共7个数据结构(根据数据库NHModeServerDB)

* 版本：1.0.0
* 日期：2010-01-01
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#ifndef NHSTRUCTDEF_H
#define NHSTRUCTDEF_H

#include <string>
#include <vector>
#include <atlstr.h>
typedef unsigned char BYTE;

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "../ZipLib/ZipLib.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/ZipLib_D.lib")
#else
#pragma comment(lib, "../Release/ZipLib.lib")
#endif

//////////////////////////////////////////////////////////////////
#define NH_INT_TO_WCHART 12
#define NH_TIME_TO_WCHART 30
#define NH_BOOL_TO_WCHART 2
#define NH_FLOAT_TO_WCHART 20
//////////////////////////////////////////////////////////////////

// 计算&校验Hash的版本
enum HashVer
{
	HV_V1 = 1,
	HV_V2,
	HV_V3,
	HV_V4,
	HV_V5,
	HV_V6,
	HV_V7,
	HV_V8,
	HV_V9,
	HV_V10,
};

// 系统设置 NHClient.ini
typedef struct NHClient
{
	wchar_t wchSystemType[10];	// 系统类型
	wchar_t wchLineNumber[50];	// 检测线编号
	wchar_t wchVersion[25];		// 版本号

	NHClient(){ZeroMemory(this, sizeof(NHClient));}
}NHCLIENT, *PNHCLIENT;

// 用户信息<UserInfo>
typedef struct UseInfo
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];				// 自动编号
	wchar_t wchName[50];								// 用户名
	wchar_t wchPassword[50];							// 密码
	wchar_t wchLicenseSn[50];						// 许可证编号
	wchar_t wchLicenseStartDate[NH_TIME_TO_WCHART];	// 许可证开始日期
	wchar_t wchLicenseEndDate[NH_TIME_TO_WCHART];	// 许可证结束日期
	wchar_t wchRole[NH_INT_TO_WCHART];				// 角色
	wchar_t wchRemark[1024];							// 备注
	wchar_t wchReserved1[50];						// 保留字段,以后备用
	wchar_t wchReserved2[50];						// 保留字段,以后备用
	wchar_t wchReserved3[50];						// 保留字段,以后备用

	UseInfo(){ZeroMemory(this, sizeof(UseInfo));}
}USERINFO,*PUSERINFO;

// 开单信息<BillInfo>
//typedef struct BillInfo
//{
//	wchar_t wchAutoID[NH_INT_TO_WCHART];
//	wchar_t wchRunningNumber[50];
//	wchar_t wchPlateNumber[50];
//	wchar_t wchPlateType[50];
//	wchar_t wchTestType[NH_INT_TO_WCHART];
//	wchar_t wchIsRetest[NH_BOOL_TO_WCHART];
//	wchar_t wchIsFuelTypeNeedTest[NH_BOOL_TO_WCHART];
//	wchar_t wchIsFuelType2NeedTest[NH_BOOL_TO_WCHART];
//	wchar_t wchBillDate[NH_TIME_TO_WCHART];
//	wchar_t wchBiller[50];
//	wchar_t wchShroff[50];
//	wchar_t wchDriver[50];
//	wchar_t wchAssessor[50];
//	wchar_t wchChargeState[NH_INT_TO_WCHART];
//	wchar_t wchInitialMoney[NH_FLOAT_TO_WCHART];
//	wchar_t wchChangedMoney[NH_FLOAT_TO_WCHART];
//	wchar_t wchReceivableMoney[NH_FLOAT_TO_WCHART];
//	wchar_t wchActualMoney[NH_FLOAT_TO_WCHART];
//	wchar_t wchTestCharacter[50];
//	wchar_t wchTestLocation[50];
//	wchar_t wchTestState[NH_INT_TO_WCHART];
//	wchar_t wchLineNumber[50];
//	wchar_t wchStartProcessingTime[NH_TIME_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodASM[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodVMAS[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodLUGDOWN[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodDIS[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodFSUNHT[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodFSUYD[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodAVFSU[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodMI[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriodMDIS[NH_INT_TO_WCHART];
//	wchar_t wchNumberOfTestPeriod[NH_INT_TO_WCHART];
//	wchar_t wchTestPeriodUsed[NH_INT_TO_WCHART];
//	wchar_t wchQuitReason[50];
//	wchar_t wchRemark[1024];
//	wchar_t wchReserved1[50];
//	wchar_t wchReserved2[50];
//	wchar_t wchReserved3[50];
//
//	BillInfo(){ZeroMemory(this, sizeof(BillInfo));}
//}BILLINFO,*PBILLINFO;

// 车辆信息<VehicleInfo>
typedef struct VehicleInfo
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchPlateNumber[50];
	wchar_t wchPlateType[50];
	wchar_t wchPlateTypeCode[50];
	wchar_t wchPlateTypeGA[50];
	wchar_t wchPlateTypeCodeGA[50];
	wchar_t wchRegistrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOwner[50];
	wchar_t wchPhone[50];
	wchar_t wchAddress[50];
	wchar_t wchUseCharacter[50];
	wchar_t wchManufacturer[50];
	wchar_t wchBrand[50];
	wchar_t wchModel[50];
	wchar_t wchProductionDate[NH_TIME_TO_WCHART];
	wchar_t wchVIN[50];
	wchar_t wchChassisModel[50];
	wchar_t wchVehicleType[50];
	wchar_t wchMaximumTotalMass[NH_INT_TO_WCHART];
	wchar_t wchUnladenMass[NH_INT_TO_WCHART];
	wchar_t wchAxleMass[NH_INT_TO_WCHART];
	wchar_t wchRatedLoadingMass[NH_INT_TO_WCHART];
	wchar_t wchRatedPassengerCapacity[NH_INT_TO_WCHART];
	wchar_t wchTyrePressure[NH_FLOAT_TO_WCHART];
	wchar_t wchTravelledDistance[NH_INT_TO_WCHART];
	wchar_t wchEngineManufacturer[50];
	wchar_t wchEngineModel[50];
	wchar_t wchEngineNumber[50];
	wchar_t wchEngineStroke[NH_INT_TO_WCHART];
	wchar_t wchDisplacement[NH_FLOAT_TO_WCHART];
	wchar_t wchNumberOfCylinder[NH_INT_TO_WCHART];
	wchar_t wchOilSupplyMode[50];
	wchar_t wchAirIntakeMode[50];
	wchar_t wchHasCatalyticConverter[NH_BOOL_TO_WCHART];
	wchar_t wchFuelType[50];
	wchar_t wchFuelMark[50];
	wchar_t wchDoubleFuel[NH_BOOL_TO_WCHART];
	wchar_t wchFuelType2[50];
	wchar_t wchFuelMark2[50];
	wchar_t wchRatedRev[NH_INT_TO_WCHART];
	wchar_t wchRatedPower[NH_FLOAT_TO_WCHART];
	wchar_t wchMaximumNetPower[NH_FLOAT_TO_WCHART];
	wchar_t wchGearBoxType[50];
	wchar_t wchNumberOfGears[NH_INT_TO_WCHART];
	wchar_t wchDriveType[50];
	wchar_t wchEPSign[50];
	wchar_t wchCertificateNumber[50];
	wchar_t wchIndependentDoubleExhaustPipe[NH_BOOL_TO_WCHART];
	wchar_t wchTCS[NH_INT_TO_WCHART];
	wchar_t wchFuelPumpMode[50];
	wchar_t wchPassengerVehicle[NH_BOOL_TO_WCHART];
	wchar_t wchEmissionStandard[50];
	wchar_t wchAnnualTestPeriod[NH_INT_TO_WCHART];
	wchar_t wchHasOBD[NH_BOOL_TO_WCHART];
	wchar_t wchOwnerCharacter[50];
	wchar_t wchOwnerDistrict[50];
	wchar_t wchLicenseDepartment[50];
	wchar_t wchLicenseIssueDate[NH_TIME_TO_WCHART];
	wchar_t wchLicenseValidDate[NH_TIME_TO_WCHART];
	wchar_t wchMotorModel[50];
	wchar_t wchEnergyStorageDeviceModel[50];
	wchar_t wchBatteryCapacity[NH_FLOAT_TO_WCHART];
	wchar_t wchHasDPF[NH_BOOL_TO_WCHART];
	wchar_t wchDPFModel[50];
	wchar_t wchHasSCR[NH_BOOL_TO_WCHART];
	wchar_t wchSCRModel[50];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	VehicleInfo(){ZeroMemory(this, sizeof(VehicleInfo));}
}VEHICLEINFO,*PVEHICLEINFO;

// 车型信息<VehicleModel>
typedef struct VehicleModel
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchManufacturer[50];
	wchar_t wchBrand[50];
	wchar_t wchModel[50];
	wchar_t wchChassisModel[50];
	wchar_t wchVehicleType[50];
	wchar_t wchMaximumTotalMass[NH_INT_TO_WCHART];
	wchar_t wchUnladenMass[NH_INT_TO_WCHART];
	wchar_t wchAxleMass[NH_INT_TO_WCHART];
	wchar_t wchRatedLoadingMass[NH_INT_TO_WCHART];
	wchar_t wchRatedPassengerCapacity[NH_INT_TO_WCHART];
	wchar_t wchTyrePressure[NH_FLOAT_TO_WCHART];
	wchar_t wchEngineManufacturer[50];
	wchar_t wchEngineModel[50];
	wchar_t wchEngineStroke[NH_INT_TO_WCHART];
	wchar_t wchDisplacement[NH_FLOAT_TO_WCHART];
	wchar_t wchNumberOfCylinder[NH_INT_TO_WCHART];
	wchar_t wchOilSupplyMode[50];
	wchar_t wchAirIntakeMode[50];
	wchar_t wchHasCatalyticConverter[NH_BOOL_TO_WCHART];
	wchar_t wchFuelType[50];
	wchar_t wchFuelMark[50];
	wchar_t wchDoubleFuel[NH_BOOL_TO_WCHART];
	wchar_t wchFuelType2[50];
	wchar_t wchFuelMark2[50];
	wchar_t wchRatedRev[NH_INT_TO_WCHART];
	wchar_t wchRatedPower[NH_FLOAT_TO_WCHART];
	wchar_t wchMaximumNetPower[NH_FLOAT_TO_WCHART];
	wchar_t wchGearBoxType[50];
	wchar_t wchNumberOfGears[NH_INT_TO_WCHART];
	wchar_t wchDriveType[50];
	wchar_t wchEPSign[50];
	wchar_t wchIndependentDoubleExhaustPipe[NH_BOOL_TO_WCHART];
	wchar_t wchTCS[NH_INT_TO_WCHART];
	wchar_t wchFuelPumpMode[50];
	wchar_t wchPassengerVehicle[NH_BOOL_TO_WCHART];
	wchar_t wchEmissionStandard[50];
	wchar_t wchHasOBD[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	VehicleModel(){ZeroMemory(this, sizeof(VehicleModel));}
}VEHICLEMODEL,*PVEHICLEMODEL;

// ASM限值信息<LimitOfASM>
typedef struct LimitOfASM
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchVehicleType[50];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchStartRM[NH_INT_TO_WCHART];
	wchar_t wchEndRM[NH_INT_TO_WCHART];
	wchar_t wchHC5025[NH_INT_TO_WCHART];
	wchar_t wchHC5025_EDP[NH_INT_TO_WCHART];
	wchar_t wchCO5025[NH_FLOAT_TO_WCHART];
	wchar_t wchCO5025_EDP[NH_INT_TO_WCHART];
	wchar_t wchNO5025[NH_INT_TO_WCHART];
	wchar_t wchNO5025_EDP[NH_INT_TO_WCHART];
	wchar_t wchHC2540[NH_INT_TO_WCHART];
	wchar_t wchHC2540_EDP[NH_INT_TO_WCHART];
	wchar_t wchCO2540[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2540_EDP[NH_INT_TO_WCHART];
	wchar_t wchNO2540[NH_INT_TO_WCHART];
	wchar_t wchNO2540_EDP[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfASM(){ZeroMemory(this, sizeof(LimitOfASM));}
}LIMITOFASM, *PLIMITOFASM;

// VMAS限值信息<LimitOfVMAS>
typedef struct LimitOfVMAS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchVehicleType[50];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchStartRM[NH_INT_TO_WCHART];
	wchar_t wchEndRM[NH_INT_TO_WCHART];
	wchar_t wchHC[NH_FLOAT_TO_WCHART];
	wchar_t wchHC_EDP[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchCO_EDP[NH_INT_TO_WCHART];
	wchar_t wchNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchNOx_EDP[NH_INT_TO_WCHART];
	wchar_t wchHCNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchHCNOx_EDP[NH_INT_TO_WCHART];
	wchar_t wchSelectType[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfVMAS(){ZeroMemory(this, sizeof(LimitOfVMAS));}
}LIMITOFVMAS, *PLIMITOFVMAS;

// LimitOfLUGDOWN限值信息<LimitOfLimitOfLUGDOWN>
typedef struct LimitOfLUGDOWN
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchVehicleType[50];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchK[NH_FLOAT_TO_WCHART];
	wchar_t wchK_EDP[NH_INT_TO_WCHART];
	wchar_t wchN[NH_FLOAT_TO_WCHART];
	wchar_t wchN_EDP[NH_INT_TO_WCHART];
	wchar_t wchNOx[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfLUGDOWN(){ZeroMemory(this, sizeof(LimitOfLUGDOWN));}
}LIMITOFLUGDOWN, *PLIMITOFLUGDOWN;

// DIS限值信息<LimitOfDIS>
typedef struct LimitOfDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchVehicleType[50];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchHCHigh[NH_INT_TO_WCHART];
	wchar_t wchHCHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchCOHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchCOHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchHCLow[NH_INT_TO_WCHART];
	wchar_t wchHCLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchCOLow[NH_FLOAT_TO_WCHART];
	wchar_t wchCOLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchLambdaLower[NH_FLOAT_TO_WCHART];
	wchar_t wchLambdaUpper[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfDIS(){ZeroMemory(this, sizeof(LimitOfDIS));}
}LIMITOFDIS, *PLIMITOFDIS;

// FSUNHT限值信息<LimitOfFSUNHT>
typedef struct LimitOfFSUNHT
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchIsNaturalAspirate[NH_BOOL_TO_WCHART];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchK[NH_FLOAT_TO_WCHART];
	wchar_t wchK_EDP[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfFSUNHT(){ZeroMemory(this, sizeof(LimitOfFSUNHT));}
}LIMITOFFSUNHT, *PLIMITOFFSUNHT;

// FSUNHTOfHeavyVehicle限值信息<LimitOfFSUNHTOfHeavyVehicle>
typedef struct LimitOfFSUNHTOfHeavyVehicle
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchEngineManufacturer[50];
	wchar_t wchEngineModel[50];
	wchar_t wchMaximumNetPower[NH_FLOAT_TO_WCHART];
	wchar_t wchTypeApprovalLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchTypeApprovalLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfFSUNHTOfHeavyVehicle(){ZeroMemory(this, sizeof(LimitOfFSUNHTOfHeavyVehicle));}
}LIMITOFFSUNHTOFHEAVYVEHICLE, *PLIMITOFFSUNHTOFHEAVYVEHICLE;

// FSUNHTOfLightVehicle限值信息<LimitOfFSUNHTOfLightVehicle>
typedef struct LimitOfFSUNHTOfLightVehicle
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchVehicleManufacturer[50];
	wchar_t wchVehicleModel[50];
	wchar_t wchVehicleName[50];
	wchar_t wchEngineModel[50];
	wchar_t wchEngineManufacturer[50];
	wchar_t wchTypeApprovalLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchTypeApprovalLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfFSUNHTOfLightVehicle(){ZeroMemory(this, sizeof(LimitOfFSUNHTOfLightVehicle));}
}LIMITOFFSUNHTOFLIGHTVEHICLE, *PLIMITOFFSUNHTOFLIGHTVEHICLE;

// FSUYD限值信息<LimitOfFSUYD>
typedef struct LimitOfFSUYD
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchSmoke[NH_FLOAT_TO_WCHART];
	wchar_t wchSmoke_EDP[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfFSUYD(){ZeroMemory(this, sizeof(LimitOfFSUYD));}
}LIMITOFFSUYD, *PLIMITOFFSUYD;

// MDIS限值信息<LimitOfMDIS>
typedef struct LimitOfMDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchVehicleType[50];
	wchar_t wchEngineStroke[50];
	wchar_t wchStartDate[NH_TIME_TO_WCHART];
	wchar_t wchEndDate[NH_TIME_TO_WCHART];
	wchar_t wchHCHigh[NH_INT_TO_WCHART];
	wchar_t wchHCHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchCOHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchCOHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchHCLow[NH_INT_TO_WCHART];
	wchar_t wchHCLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchCOLow[NH_FLOAT_TO_WCHART];
	wchar_t wchCOLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchSkipHISJudgement[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LimitOfMDIS(){ZeroMemory(this, sizeof(LimitOfMDIS));}
}LIMITOFMDIS, *PLIMITOFMDIS;

// 检测线信息<LineInfo>
typedef struct LineInfo
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchLineNumber[50];
	wchar_t wchLineModel[50];
	wchar_t wchLineName[50];
	wchar_t wchLineManufacturer[50];
	wchar_t wchLineLicenseSn[50];
	wchar_t wchLineLicenseStartDate[NH_TIME_TO_WCHART];
	wchar_t wchLineLicenseEndDate[NH_TIME_TO_WCHART];
	wchar_t wchLineIsLocked[NH_BOOL_TO_WCHART];
	wchar_t wchLineState[NH_INT_TO_WCHART];
	wchar_t wchLineStateChangeTime[NH_TIME_TO_WCHART];
	wchar_t wchLineRegistrationInfo[NH_INT_TO_WCHART];
	wchar_t wchLineIPAddress[50];
	wchar_t wchLineHardwareSN[50];
	wchar_t wchHasDynamometer[NH_BOOL_TO_WCHART];
	wchar_t wchDynamometerNumber[50];
	wchar_t wchDynamometerModel[50];
	wchar_t wchDynamometerManufacturer[50];
	wchar_t wchDynamometerCoastDownDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerPLHPDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerForceCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerForceCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasAnalyzer[NH_BOOL_TO_WCHART];
	wchar_t wchAnalyzerNumber[50];
	wchar_t wchAnalyzerModel[50];
	wchar_t wchAnalyzerManufacturer[50];
	wchar_t wchAnalyzerChannelCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchAnalyzerChannelCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchAnalyzerEnvParamCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchAnalyzerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasFlowmeter[NH_BOOL_TO_WCHART];
	wchar_t wchFlowmeterNumber[50];
	wchar_t wchFlowmeterModel[50];
	wchar_t wchFlowmeterManufacturer[50];
	wchar_t wchFlowmeterFluxCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterFluxCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterDilO2CalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterPressureCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasOpacimeter[NH_BOOL_TO_WCHART];
	wchar_t wchOpacimeterNumber[50];
	wchar_t wchOpacimeterModel[50];
	wchar_t wchOpacimeterManufacturer[50];
	wchar_t wchOpacimeterChannelCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOpacimeterChannelCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchOpacimeterEnvParamCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOpacimeterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasSmokemeter[NH_BOOL_TO_WCHART];
	wchar_t wchSmokemeterNumber[50];
	wchar_t wchSmokemeterModel[50];
	wchar_t wchSmokemeterManufacturer[50];
	wchar_t wchSmokemeterCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchSmokemeterCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchSmokemeterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasIndependentEnvParamMeasurer[NH_BOOL_TO_WCHART];
	wchar_t wchEnvParamMeasurerNumber[50];
	wchar_t wchEnvParamMeasurerModel[50];
	wchar_t wchEnvParamMeasurerManufacturer[50];
	wchar_t wchEnvParamMeasurerCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchEnvParamMeasurerCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchEnvParamMeasurerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasIndependentTachometer[NH_BOOL_TO_WCHART];
	wchar_t wchTachometerNumber[50];
	wchar_t wchTachometerModel[50];
	wchar_t wchTachometerManufacturer[50];
	wchar_t wchTachometerCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchTachometerCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchTachometerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasOBDAdapter[NH_BOOL_TO_WCHART];
	wchar_t wchOBDAdapterNumber[50];
	wchar_t wchOBDAdapterModel[50];
	wchar_t wchOBDAdapterManufacturer[50];
	wchar_t wchOBDAdapterCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOBDAdapterCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchOBDAdapterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasNOxAnalyzer[NH_BOOL_TO_WCHART];
	wchar_t wchNOxAnalyzerNumber[50];
	wchar_t wchNOxAnalyzerModel[50];
	wchar_t wchNOxAnalyzerManufacturer[50];
	wchar_t wchNOxAnalyzerChannelCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchNOxAnalyzerChannelCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchNOxAnalyzerEnvParamCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchNOxAnalyzerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchTerminationSign[NH_INT_TO_WCHART];
	wchar_t wchCertificateAuthentication[1024];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	LineInfo(){ZeroMemory(this, sizeof(LineInfo));}
}LINEINFO, *PLINEINFO;

// 检测站信息<StationInfo>
typedef struct StationInfo
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchStationNumber[50];
	wchar_t wchStationFullName[50];
	wchar_t wchStationShortName[50];
	wchar_t wchStationLicenseSn[50];
	wchar_t wchStationLicenseStartDate[NH_TIME_TO_WCHART];
	wchar_t wchStationLicenseEndDate[NH_TIME_TO_WCHART];
	wchar_t wchStationIsLocked[NH_BOOL_TO_WCHART];
	wchar_t wchStationRegistrationInfo[NH_INT_TO_WCHART];
	wchar_t wchStationPhone[50];
	wchar_t wchStationFax[50];
	wchar_t wchPostCode[50];
	wchar_t wchProvince[50];
	wchar_t wchCity[50];
	wchar_t wchDistrict[50];
	wchar_t wchDetailedAddress[50];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	StationInfo(){ZeroMemory(this, sizeof(StationInfo));}
}STATIONINFO, *PSTATIONINFO;

// 排放检测日志信息<TestLog>
typedef struct TestLog
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchRunningNumberEP[50];
	wchar_t wchReportNumberEP[50];
	wchar_t wchItemApp[NH_INT_TO_WCHART];	// 外观检验(0-免除检测,1-等待检测,2-准备检测,3-正在检测,4-完成检测,NULL-免除检测) 
	wchar_t wchItemOBD[NH_INT_TO_WCHART];	// OBD检测(0-免除检测,1-等待检测,2-准备检测,3-正在检测,4-完成检测,NULL-免除检测) 
	wchar_t wchItemEmi[NH_INT_TO_WCHART];	// 排放检测(0-免除检测,1-等待检测,2-准备检测,3-正在检测,4-完成检测,NULL-免除检测)
	wchar_t wchBillDate[NH_TIME_TO_WCHART];
	wchar_t wchBiller[50];
	wchar_t wchShroff[50];
	wchar_t wchChargeState[NH_INT_TO_WCHART];
	wchar_t wchInitialMoney[NH_FLOAT_TO_WCHART];
	wchar_t wchChangedMoney[NH_FLOAT_TO_WCHART];
	wchar_t wchReceivableMoney[NH_FLOAT_TO_WCHART];
	wchar_t wchActualMoney[NH_FLOAT_TO_WCHART];
	wchar_t wchTestLocation[50];
	wchar_t wchTestState[NH_INT_TO_WCHART];
	wchar_t wchIsTestFinished[NH_BOOL_TO_WCHART];
	wchar_t wchNumberOfTestPeriodASM[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodVMAS[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodLUGDOWN[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodDIS[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodFSUNHT[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodFSUYD[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodAVFSU[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodMI[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriodMDIS[NH_INT_TO_WCHART];
	wchar_t wchNumberOfTestPeriod[NH_INT_TO_WCHART];
	wchar_t wchTestPeriodUsed[NH_INT_TO_WCHART];
	wchar_t wchQuitReason[50];	 
	wchar_t wchTestType[NH_INT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchOperator[50];
	wchar_t wchDriver[50];
	wchar_t wchAssessor[50];
	wchar_t wchStartProcessingTime[NH_TIME_TO_WCHART];
	wchar_t wchIsRetest[NH_BOOL_TO_WCHART];
	wchar_t wchIsFuelTypeNeedTest[NH_BOOL_TO_WCHART];
	wchar_t wchIsFuelTypeTestFinished[NH_BOOL_TO_WCHART];
	wchar_t wchIsFuelTypeTestPass[NH_BOOL_TO_WCHART];
	wchar_t wchIsFuelType2NeedTest[NH_BOOL_TO_WCHART];
	wchar_t wchIsFuelType2TestFinished[NH_BOOL_TO_WCHART];
	wchar_t wchIsFuelType2TestPass[NH_BOOL_TO_WCHART];
	wchar_t wchTestDate[NH_TIME_TO_WCHART];
	wchar_t wchTestStartTime[NH_TIME_TO_WCHART];
	wchar_t wchTestEndTime[NH_TIME_TO_WCHART];
	wchar_t wchTestCharacter[50];
	wchar_t wchTestCharacterCode[50];
	wchar_t wchPrintTimes[NH_INT_TO_WCHART];
	wchar_t wchRecentPrintReportTime[NH_TIME_TO_WCHART];
	wchar_t wchIsSent[NH_BOOL_TO_WCHART];
	wchar_t wchIsSentEP[NH_BOOL_TO_WCHART];
	wchar_t wchPlateNumber[50];
	wchar_t wchPlateType[50];
	wchar_t wchPlateTypeCode[50];
	wchar_t wchPlateTypeGA[50];
	wchar_t wchPlateTypeCodeGA[50];
	wchar_t wchRegistrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOwner[50];
	wchar_t wchPhone[50];
	wchar_t wchAddress[50];
	wchar_t wchUseCharacter[50];
	wchar_t wchManufacturer[50];
	wchar_t wchBrand[50];
	wchar_t wchModel[50];
	wchar_t wchProductionDate[NH_TIME_TO_WCHART];
	wchar_t wchVIN[50];
	wchar_t wchChassisModel[50];
	wchar_t wchVehicleType[50];
	wchar_t wchMaximumTotalMass[NH_INT_TO_WCHART];
	wchar_t wchUnladenMass[NH_INT_TO_WCHART];
	wchar_t wchAxleMass[NH_INT_TO_WCHART];
	wchar_t wchRatedLoadingMass[NH_INT_TO_WCHART];
	wchar_t wchRatedPassengerCapacity[NH_INT_TO_WCHART];
	wchar_t wchTyrePressure[NH_FLOAT_TO_WCHART];
	wchar_t wchTravelledDistance[NH_INT_TO_WCHART];
	wchar_t wchEngineManufacturer[50];
	wchar_t wchEngineModel[50];
	wchar_t wchEngineNumber[50];
	wchar_t wchEngineStroke[NH_INT_TO_WCHART];
	wchar_t wchDisplacement[NH_FLOAT_TO_WCHART];
	wchar_t wchNumberOfCylinder[NH_INT_TO_WCHART];
	wchar_t wchOilSupplyMode[50];
	wchar_t wchAirIntakeMode[50];
	wchar_t wchHasCatalyticConverter[NH_BOOL_TO_WCHART];
	wchar_t wchFuelType[50];
	wchar_t wchFuelMark[50];
	wchar_t wchDoubleFuel[NH_BOOL_TO_WCHART];
	wchar_t wchFuelType2[50];
	wchar_t wchFuelMark2[50];
	wchar_t wchRatedRev[NH_INT_TO_WCHART];
	wchar_t wchRatedPower[NH_FLOAT_TO_WCHART];
	wchar_t wchMaximumNetPower[NH_FLOAT_TO_WCHART];
	wchar_t wchGearBoxType[50];
	wchar_t wchNumberOfGears[NH_INT_TO_WCHART];
	wchar_t wchDriveType[50];
	wchar_t wchEPSign[50];
	wchar_t wchCertificateNumber[50];
	wchar_t wchIndependentDoubleExhaustPipe[NH_BOOL_TO_WCHART];
	wchar_t wchTCS[NH_INT_TO_WCHART];
	wchar_t wchFuelPumpMode[50];
	wchar_t wchPassengerVehicle[NH_BOOL_TO_WCHART];
	wchar_t wchEmissionStandard[50];
	wchar_t wchAnnualTestPeriod[NH_INT_TO_WCHART];
	wchar_t wchHasOBD[NH_BOOL_TO_WCHART];
	wchar_t wchOwnerCharacter[50];
	wchar_t wchOwnerDistrict[50];
	wchar_t wchLicenseDepartment[50];
	wchar_t wchLicenseIssueDate[NH_TIME_TO_WCHART];
	wchar_t wchLicenseValidDate[NH_TIME_TO_WCHART];
	wchar_t wchMotorModel[50];
	wchar_t wchEnergyStorageDeviceModel[50];
	wchar_t wchBatteryCapacity[NH_FLOAT_TO_WCHART];
	wchar_t wchHasDPF[NH_BOOL_TO_WCHART];
	wchar_t wchDPFModel[50];
	wchar_t wchHasSCR[NH_BOOL_TO_WCHART];
	wchar_t wchSCRModel[50];
	wchar_t wchLineNumber[50];
	wchar_t wchLineModel[50];
	wchar_t wchLineName[50];
	wchar_t wchLineManufacturer[50];
	wchar_t wchLineLicenseSn[50];
	wchar_t wchLineLicenseStartDate[NH_TIME_TO_WCHART];
	wchar_t wchLineLicenseEndDate[NH_TIME_TO_WCHART];
	wchar_t wchLineIsLocked[NH_BOOL_TO_WCHART];
	wchar_t wchLineRegistrationInfo[NH_INT_TO_WCHART];
	wchar_t wchLineIPAddress[50];
	wchar_t wchLineHardwareSN[50];
	wchar_t wchHasDynamometer[NH_BOOL_TO_WCHART];
	wchar_t wchDynamometerNumber[50];
	wchar_t wchDynamometerModel[50];
	wchar_t wchDynamometerManufacturer[50];
	wchar_t wchDynamometerCoastDownDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerPLHPDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerForceCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerForceCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchDynamometerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasAnalyzer[NH_BOOL_TO_WCHART];
	wchar_t wchAnalyzerNumber[50];
	wchar_t wchAnalyzerModel[50];
	wchar_t wchAnalyzerManufacturer[50];
	wchar_t wchAnalyzerChannelCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchAnalyzerChannelCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchAnalyzerEnvParamCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchAnalyzerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasFlowmeter[NH_BOOL_TO_WCHART];
	wchar_t wchFlowmeterNumber[50];
	wchar_t wchFlowmeterModel[50];
	wchar_t wchFlowmeterManufacturer[50];
	wchar_t wchFlowmeterFluxCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterFluxCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterDilO2CalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterPressureCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchFlowmeterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasOpacimeter[NH_BOOL_TO_WCHART];
	wchar_t wchOpacimeterNumber[50];
	wchar_t wchOpacimeterModel[50];
	wchar_t wchOpacimeterManufacturer[50];
	wchar_t wchOpacimeterChannelCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOpacimeterChannelCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchOpacimeterEnvParamCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOpacimeterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasSmokemeter[NH_BOOL_TO_WCHART];
	wchar_t wchSmokemeterNumber[50];
	wchar_t wchSmokemeterModel[50];
	wchar_t wchSmokemeterManufacturer[50];
	wchar_t wchSmokemeterCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchSmokemeterCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchSmokemeterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasIndependentEnvParamMeasurer[NH_BOOL_TO_WCHART];
	wchar_t wchEnvParamMeasurerNumber[50];
	wchar_t wchEnvParamMeasurerModel[50];
	wchar_t wchEnvParamMeasurerManufacturer[50];
	wchar_t wchEnvParamMeasurerCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchEnvParamMeasurerCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchEnvParamMeasurerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasIndependentTachometer[NH_BOOL_TO_WCHART];
	wchar_t wchTachometerNumber[50];
	wchar_t wchTachometerModel[50];
	wchar_t wchTachometerManufacturer[50];
	wchar_t wchTachometerCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchTachometerCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchTachometerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasOBDAdapter[NH_BOOL_TO_WCHART];
	wchar_t wchOBDAdapterNumber[50];
	wchar_t wchOBDAdapterModel[50];
	wchar_t wchOBDAdapterManufacturer[50];
	wchar_t wchOBDAdapterCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchOBDAdapterCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchOBDAdapterAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchHasNOxAnalyzer[NH_BOOL_TO_WCHART];
	wchar_t wchNOxAnalyzerNumber[50];
	wchar_t wchNOxAnalyzerModel[50];
	wchar_t wchNOxAnalyzerManufacturer[50];
	wchar_t wchNOxAnalyzerChannelCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchNOxAnalyzerChannelCheckDate[NH_TIME_TO_WCHART];
	wchar_t wchNOxAnalyzerEnvParamCalibrationDate[NH_TIME_TO_WCHART];
	wchar_t wchNOxAnalyzerAuthenticationDate[NH_TIME_TO_WCHART];
	wchar_t wchStationNumber[50];
	wchar_t wchStationFullName[50];
	wchar_t wchStationShortName[50];
	wchar_t wchStationLicenseSn[50];
	wchar_t wchStationLicenseStartDate[NH_TIME_TO_WCHART];
	wchar_t wchStationLicenseEndDate[NH_TIME_TO_WCHART];
	wchar_t wchStationIsLocked[NH_BOOL_TO_WCHART];
	wchar_t wchStationRegistrationInfo[NH_INT_TO_WCHART];
	wchar_t wchStationPhone[50];
	wchar_t wchStationFax[50];
	wchar_t wchPostCode[50];
	wchar_t wchProvince[50];
	wchar_t wchCity[50];
	wchar_t wchDistrict[50];
	wchar_t wchDetailedAddress[50];
	wchar_t wchSRM[2];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestLog(){ZeroMemory(this, sizeof(TestLog));}
}TESTLOG,*PTESTLOG;

// 检测次数统计<TestAmount>
typedef struct TestAmount
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchLineNumber[50];
	wchar_t wchTestDate[NH_TIME_TO_WCHART];
	wchar_t wchASMAmount[NH_INT_TO_WCHART];
	wchar_t wchVMASAmount[NH_INT_TO_WCHART];
	wchar_t wchLUGDOWNAmount[NH_INT_TO_WCHART];
	wchar_t wchDISAmount[NH_INT_TO_WCHART];
	wchar_t wchFSUNHTAmount[NH_INT_TO_WCHART];
	wchar_t wchFSUYDAmount[NH_INT_TO_WCHART];
	wchar_t wchAVFSUAmount[NH_INT_TO_WCHART];
	wchar_t wchMIAmount[NH_INT_TO_WCHART];
	wchar_t wchLUGDOWNHeavyAmount[NH_INT_TO_WCHART];
	wchar_t wchLUGDOWNLightAmount[NH_INT_TO_WCHART];
	wchar_t wchAllAmount[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestAmount(){ZeroMemory(this, sizeof(TestAmount));}
}TESTAMOUNT,*PTESTAMOUNT;

struct SOBDRTData
{
	std::wstring strVelocity;					//车速[km/h]
	std::wstring strCoolantTemp;				//冷却液温度[℃]
	std::wstring strEngineRev;					//发动机转速[rpm]
	std::wstring strSolarTermDoorPosition;		//节气门开度[%]
	std::wstring strThrottleOpening;			//油门开度[%]
	std::wstring strCalculationLoad;			//计算负荷值[%]
	std::wstring strForwardSensorV;				//前传感器电压(mV) 
	std::wstring strForwardSensorI;				//前传感器电流(mA)
	std::wstring strAirCoefficient;				//过量空气系数(λ) 
	std::wstring strMAF;						//进气量(g/s)
	std::wstring strMAP;						//进气压力(kpa)
	std::wstring strEngineOutputPower;			//发动机输出功率(kw)
	std::wstring strChargeAirPressure;			//增压压力[kpa]
	std::wstring strFuelConsumption;			//耗油量[L/100km]
	std::wstring strNOXConcentration;			//氮氧传感器浓度[ppm]
	std::wstring strUreaInjectionVolume;		//尿素喷射量[mL/s]
	std::wstring strEGT;						//排气温度[℃]
	std::wstring strDPFDifferentialPressure;	//颗粒捕集器压差[Kpa]
	std::wstring strEGRPosition;				//EGR开度[%]
	std::wstring strFuelDeliveryPressure;		//燃油喷射压力[bar]
};

// OBD检测结果<ResultOfOBD>
struct SResultOfOBD
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;

	// FuelType
	std::wstring strFuelType;	
	// Operator
	std::wstring strOperator;	
	// DetTimes
	std::wstring strDetTimes;	
	// DetBegTime
	std::wstring strDetBegTime;
	// DetEndTime
	std::wstring strDetEndTime;
	// OBDProtocol
	std::wstring strOBDProtocol;
	// ComErrorJudge(0:同NULL；1:合格；2:不合格)
	std::wstring strComErrorJudge;
	// ComErrorComment
	std::wstring strComErrorComment;	
	// VIN
	std::wstring strVIN;
	// OBDType
	std::wstring strOBDType;
	// Mileage
	std::wstring strMileage;
	// EngineCALID
	std::wstring strEngineCALID;
	// EngineCVN
	std::wstring strEngineCVN;
	// PostProcessingCALID
	std::wstring strPostProcessingCALID;
	// PostProcessingCVN
	std::wstring strPostProcessingCVN;
	// OtherCALID
	std::wstring strOtherCALID;
	// OtherCVN
	std::wstring strOtherCVN;
	// DisplayJudge
	std::wstring strDisplayJudge;
	// DisplayComment
	std::wstring strDisplayComment;
	// LampStateJudge
	std::wstring strLampStateJudge;
	// LampStateComment
	std::wstring strLampStateComment;
	// DTC
	std::wstring strDTC;
	// DTCComment
	std::wstring strDTCComment;
	// FreezeData
	std::wstring strFreezeData;
	// DTCMileage
	std::wstring strDTCMileage;
	// DTCJudge
	std::wstring strDTCJudge;
	// CheckItemJudge_MFT
	std::wstring strCheckItemJudge_MFT;
	// CheckItemJudge_FST
	std::wstring strCheckItemJudge_FST;
	// CheckItemJudge_CC (催化转换器 Catalyst Converter)
	std::wstring strCheckItemJudge_CC;
	// CheckItemJudge_CCH (催化转换器加热器 Catalyst Converter Heater)
	std::wstring strCheckItemJudge_CCH;
	// CheckItemJudge_OS (氧传感器 Oxygen Sensor)
	std::wstring strCheckItemJudge_OS;
	// CheckItemJudge_OSH (氧传感器加热器 Oxygen Sensor Heater)
	std::wstring strCheckItemJudge_OSH;
	// CheckItemJudge_EECS (蒸发排放控制系统 Evaporation Emissions Control System)
	std::wstring strCheckItemJudge_EECS;
	// CheckItemJudge_EGR (排气再循环系统 Exhaust Gas Recirculation)
	std::wstring strCheckItemJudge_EGR;
	// CheckItemJudge_SAIS (二次空气喷射系统 Second Air-Injection System)
	std::wstring strCheckItemJudge_SAIS;
	// CheckItemJudge_ICM (综合部件监控 Integrated Component Monitoring)
	std::wstring strCheckItemJudge_ICM;
	// CheckItemJudge_VVT (可变气门VVT)
	std::wstring strCheckItemJudge_VVT;
	// CheckItemJudge_DOC (氧化型催化转化器 Diesel Oxidation Catalyst)
	std::wstring strCheckItemJudge_DOC;
	// CheckItemJudge_SCR (选择性催化还原器 Selective Catalytic Reduction)
	std::wstring strCheckItemJudge_SCR;
	// CheckItemJudge_DPF (颗粒捕集/过滤器 Diesel Particulate Filter)
	std::wstring strCheckItemJudge_DPF;
	// CheckItemJudge_ASC (氨逃逸催化器 Ammonia Slip Catalytic)
	std::wstring strCheckItemJudge_ASC;
	// CheckItemJudge_POC (颗粒物催化氧化器 Particle Oxidation Catalyst)
	std::wstring strCheckItemJudge_POC;
	// SystemCheckJudge
	std::wstring strSystemCheckJudge;
	// IsNeedReCheck
	std::wstring strIsNeedReCheck;
	// ReCheckItem
	std::wstring strReCheckItem;
	// ReCheckResult
	std::wstring strReCheckResult;
	// IUPR_OMCEC
	std::wstring strIUPR_OMCEC;
	// IUPR_ICC
	std::wstring strIUPR_ICC;
	// IUPR_CMCCB1
	std::wstring strIUPR_CMCCB1;
	// IUPR_CMCECB1
	std::wstring strIUPR_CMCECB1;
	// IUPR_CMCCB2
	std::wstring strIUPR_CMCCB2;
	// IUPR_CMCECB2
	std::wstring strIUPR_CMCECB2;
	// IUPR_O2SMCCB1
	std::wstring strIUPR_O2SMCCB1;
	// IUPR_O2SMCECB1
	std::wstring strIUPR_O2SMCECB1;
	// IUPR_O2SMCCB2
	std::wstring strIUPR_O2SMCCB2;
	// IUPR_O2SMCECB2
	std::wstring strIUPR_O2SMCECB2;
	// IUPR_RO2SMCCB1
	std::wstring strIUPR_RO2SMCCB1;
	// IUPR_RO2SMCECB1
	std::wstring strIUPR_RO2SMCECB1;
	// IUPR_RO2SMCCB2
	std::wstring strIUPR_RO2SMCCB2;
	// IUPR_RO2SMCECB2
	std::wstring strIUPR_RO2SMCECB2;
	// IUPR_EGRC
	std::wstring strIUPR_EGRC;
	// IUPR_EGREC
	std::wstring strIUPR_EGREC;
	// GPF监视完成次数，组1
	std::wstring strIUPR_GPFC1;
	// GPF监视条件符合次数，组1
	std::wstring strIUPR_GPFEC1;
	// GPF监视完成次数，组2
	std::wstring strIUPR_GPFC2;
	// GPF监视条件符合次数，组2
	std::wstring strIUPR_GPFEC2;
	// IUPR_AMCCC
	std::wstring strIUPR_AMCCC;
	// IUPR_AMCEC
	std::wstring strIUPR_AMCEC;
	// IUPR_EVAPC
	std::wstring strIUPR_EVAPC;
	// IUPR_EVAPEC
	std::wstring strIUPR_EVAPEC;
	// IUPR_NMHCC
	std::wstring strIUPR_NMHCC;
	// IUPR_NMHCEC
	std::wstring strIUPR_NMHCEC;
	// IUPR_NOXCC
	std::wstring strIUPR_NOXCC;
	// IUPR_NOXCEC
	std::wstring strIUPR_NOXCEC;
	// IUPR_NOXAC
	std::wstring strIUPR_NOXAC;
	// IUPR_NOXAEC
	std::wstring strIUPR_NOXAEC;
	// IUPR_PMC
	std::wstring strIUPR_PMC;
	// IUPR_PMEC
	std::wstring strIUPR_PMEC;
	// IUPR_WSC
	std::wstring strIUPR_WSC;
	// IUPR_WSEC
	std::wstring strIUPR_WSEC;
	// IUPR_PPC
	std::wstring strIUPR_PPC;
	// IUPR_PPEC
	std::wstring strIUPR_PPEC;
	// Judge
	std::wstring strJudge;
	// 校验Hash
	std::wstring strHash;

	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;

};

// 燃油蒸发系统检测结果<ResultOfEECS>
struct SResultOfEECS
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;

	// Operator
	std::wstring strOperator;	
	// DetTimes
	std::wstring strDetTimes;	
	// DetBegTime
	std::wstring strDetBegTime;
	// DetEndTime
	std::wstring strDetEndTime;
	// OilinInitPress
	std::wstring strOilinInitPress;
	// OilinLostPress
	std::wstring strOilinLostPress;
	// OilinPass
	std::wstring strOilinPass;
	// CapInitPress
	std::wstring strCapInitPress;
	// CapLostPress
	std::wstring strCapLostPress;
	// CapPass
	std::wstring strCapPass;
	// Pass
	std::wstring strPass;

	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;

};

// 林格曼烟度结果<ResultOfRingelmann>
struct SResultOfRingelmann
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;

	// Operator
	std::wstring strOperator;	
	// DetTimes
	std::wstring strDetTimes;	
	// DetBegTime
	std::wstring strDetBegTime;
	// DetEndTime
	std::wstring strDetEndTime;
	// HasVisibleSmoke
	std::wstring strHasVisibleSmoke;
	// RingelmannNumber
	std::wstring strRingelmannNumber;
	// Pass
	std::wstring strPass;
	// Hash
	std::wstring strHash;

	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;

};

#ifndef MAX_APP_ITEM
#define MAX_APP_ITEM	20
#endif
// 外检表结构
struct SResultOfAppearance
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strOperator;
	std::wstring strDetTimes;
	std::wstring strDetBegTime;
	std::wstring strDetEndTime;
	std::wstring strItemName[MAX_APP_ITEM];
	std::wstring strItemJudge[MAX_APP_ITEM];
	std::wstring strItemIsVeto[MAX_APP_ITEM];
	std::wstring strItemRemark[MAX_APP_ITEM];
	std::wstring strPass;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};


// 此结构体数据以XML格式存入SResultOfVMAS的二进制字段中
struct SRealTimeDataOfDIS
{
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strTime;
	std::wstring strSamplingTime;
	std::wstring strState;
	std::wstring strHC;
	std::wstring strCO;
	std::wstring strO2;
	std::wstring strCO2;
	std::wstring strNO;
	std::wstring strEngineRev;
	std::wstring strOilTemperature;
	std::wstring strLambda;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	SOBDRTData sOBDRTData;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};

struct SResultOfDIS
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strLimitOfHCHigh;
	std::wstring strLimitOfHCHigh_EDP;
	std::wstring strLimitOfCOHigh;
	std::wstring strLimitOfCOHigh_EDP;
	std::wstring strLimitOfHCLow;
	std::wstring strLimitOfHCLow_EDP;
	std::wstring strLimitOfCOLow;
	std::wstring strLimitOfCOLow_EDP;
	std::wstring strLowerLimitOfLambda;
	std::wstring strUpperLimitOfLambda;
	std::wstring strLimitOfLambda_EDP;
	std::wstring strRevHigh;
	std::wstring strOilTemperatureHigh;
	std::wstring strLambdaHigh;
	std::wstring strNOHigh;
	std::wstring strCO2High;
	std::wstring strO2High;
	std::wstring strHCHigh;
	std::wstring strCOHigh;
	std::wstring strPassOfHCHigh;
	std::wstring strPassOfCOHigh;
	std::wstring strNeedTestLambda;
	std::wstring strLambda;
	std::wstring strPassOfLambda;
	std::wstring strNeedTestLIS;
	std::wstring strRevLow;
	std::wstring strOilTemperatureLow;
	std::wstring strLambdaLow;
	std::wstring strNOLow;
	std::wstring strCO2Low;
	std::wstring strO2Low;
	std::wstring strHCLow;
	std::wstring strCOLow;
	std::wstring strPassOfHCLow;
	std::wstring strPassOfCOLow;
	std::wstring strPass;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strSkipRev;
	std::wstring strSkipOilTemperature;
	// RTDataZip，过程数据，以XML格式封装，zip算法压缩
	std::string strRTDataZip;
	// Hash数据校验
	std::wstring strHash;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;

	// 压缩过程数据
	void RTDataZip(std::vector<SRealTimeDataOfDIS> &vtRTData)
	{
		std::wstring strXML;
		strXML.append(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.append(L"<DISRTData>");
		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strXML.append(L"<Row>");
			strXML.append(L"<RunningNumber>" + vtRTData[i].strRunningNumber + L"</RunningNumber>");
			strXML.append(L"<ReportNumber>" + vtRTData[i].strReportNumber + L"</ReportNumber>");
			strXML.append(L"<FuelTypeCode>" + vtRTData[i].strFuelTypeCode + L"</FuelTypeCode>");
			strXML.append(L"<FuelTypeName>" + vtRTData[i].strFuelTypeName + L"</FuelTypeName>");
			strXML.append(L"<Time>" + vtRTData[i].strTime + L"</Time>");
			strXML.append(L"<SamplingTime>" + vtRTData[i].strSamplingTime + L"</SamplingTime>");
			strXML.append(L"<State>" + vtRTData[i].strState + L"</State>");
			strXML.append(L"<HC>" + vtRTData[i].strHC + L"</HC>");
			strXML.append(L"<CO>" + vtRTData[i].strCO + L"</CO>");
			strXML.append(L"<O2>" + vtRTData[i].strO2 + L"</O2>");
			strXML.append(L"<CO2>" + vtRTData[i].strCO2 + L"</CO2>");
			strXML.append(L"<NO>" + vtRTData[i].strNO + L"</NO>");
			strXML.append(L"<EngineRev>" + vtRTData[i].strEngineRev + L"</EngineRev>");
			strXML.append(L"<OilTemperature>" + vtRTData[i].strOilTemperature + L"</OilTemperature>");
			strXML.append(L"<Lambda>" + vtRTData[i].strLambda + L"</Lambda>");
			strXML.append(L"<EnvironmentalTemperature>" + vtRTData[i].strEnvironmentalTemperature + L"</EnvironmentalTemperature>");
			strXML.append(L"<RelativeHumidity>" + vtRTData[i].strRelativeHumidity + L"</RelativeHumidity>");
			strXML.append(L"<AtmosphericPressure>" + vtRTData[i].strAtmosphericPressure + L"</AtmosphericPressure>");
			
			// 汽油车的OBD数据流
			strXML.append(L"<OBDRTData_Velocity>" + vtRTData[i].sOBDRTData.strVelocity + L"</OBDRTData_Velocity>");
			strXML.append(L"<OBDRTData_CoolantTemp>" + vtRTData[i].sOBDRTData.strCoolantTemp + L"</OBDRTData_CoolantTemp>");
			strXML.append(L"<OBDRTData_EngineRev>" + vtRTData[i].sOBDRTData.strEngineRev + L"</OBDRTData_EngineRev>");
			strXML.append(L"<OBDRTData_SolarTermDoorPosition>" + vtRTData[i].sOBDRTData.strSolarTermDoorPosition + L"</OBDRTData_SolarTermDoorPosition>");
			strXML.append(L"<OBDRTData_ThrottleOpening>" + vtRTData[i].sOBDRTData.strThrottleOpening + L"</OBDRTData_ThrottleOpening>");
			strXML.append(L"<OBDRTData_CalculationLoad>" + vtRTData[i].sOBDRTData.strCalculationLoad + L"</OBDRTData_CalculationLoad>");
			strXML.append(L"<OBDRTData_ForwardSensorV>" + vtRTData[i].sOBDRTData.strForwardSensorV + L"</OBDRTData_ForwardSensorV>");
			strXML.append(L"<OBDRTData_ForwardSensorI>" + vtRTData[i].sOBDRTData.strForwardSensorI + L"</OBDRTData_ForwardSensorI>");
			strXML.append(L"<OBDRTData_AirCoefficient>" + vtRTData[i].sOBDRTData.strAirCoefficient + L"</OBDRTData_AirCoefficient>");
			strXML.append(L"<OBDRTData_MAF>" + vtRTData[i].sOBDRTData.strMAF + L"</OBDRTData_MAF>");
			strXML.append(L"<OBDRTData_MAP>" + vtRTData[i].sOBDRTData.strMAP + L"</OBDRTData_MAP>");

			strXML.append(L"<Remark>" + vtRTData[i].strRemark + L"</Remark>");
			strXML.append(L"<Reserved1>" + vtRTData[i].strReserved1 + L"</Reserved1>");
			strXML.append(L"<Reserved2>" + vtRTData[i].strReserved2 + L"</Reserved2>");
			strXML.append(L"<Reserved3>" + vtRTData[i].strReserved3 + L"</Reserved3>");
			strXML.append(L"</Row>");
		}
		strXML.append(L"</DISRTData>");
				
		std::string strUtf8XML = TCharToUTF8(strXML.c_str());
		CZipLib zip;
		strRTDataZip = zip.ZipData(strUtf8XML.data());
	}

	// 解压过程数据
	std::vector<SRealTimeDataOfDIS> RTDataUpZip(void)
	{
		std::vector<SRealTimeDataOfDIS> vtRTData;

		CZipLib zip;		
		std::wstring strXML = (LPCTSTR)UTF8ToTChar(zip.UnZipData(strRTDataZip).c_str());
		
		CXmlReader xmlReader;
		if (xmlReader.Parse(strXML.c_str()))
		{
			if (xmlReader.OpenNode(L"DISRTData/Row"))
			{
				do
				{			
					std::map<std::wstring, std::wstring> mapString;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapString[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();

					SRealTimeDataOfDIS sRTData;
					sRTData.strRunningNumber = mapString[L"RunningNumber"];
					sRTData.strReportNumber = mapString[L"ReportNumber"];
					sRTData.strFuelTypeCode = mapString[L"FuelTypeCode"];
					sRTData.strFuelTypeName = mapString[L"FuelTypeName"];
					sRTData.strTime = mapString[L"Time"];
					sRTData.strSamplingTime = mapString[L"SamplingTime"];
					sRTData.strState = mapString[L"State"];
					sRTData.strHC = mapString[L"HC"];
					sRTData.strCO = mapString[L"CO"];
					sRTData.strO2 = mapString[L"O2"];
					sRTData.strCO2 = mapString[L"CO2"];
					sRTData.strNO = mapString[L"NO"];
					sRTData.strEngineRev = mapString[L"EngineRev"];
					sRTData.strOilTemperature = mapString[L"OilTemperature"];
					sRTData.strLambda = mapString[L"Lambda"];
					sRTData.strEnvironmentalTemperature = mapString[L"EnvironmentalTemperature"];
					sRTData.strRelativeHumidity = mapString[L"RelativeHumidity"];
					sRTData.strAtmosphericPressure = mapString[L"AtmosphericPressure"];
					
					// 汽油车的OBD数据流
					sRTData.sOBDRTData.strVelocity = mapString[L"OBDRTData_Velocity"];
					sRTData.sOBDRTData.strCoolantTemp = mapString[L"OBDRTData_CoolantTemp"];
					sRTData.sOBDRTData.strEngineRev = mapString[L"OBDRTData_EngineRev"];
					sRTData.sOBDRTData.strSolarTermDoorPosition = mapString[L"OBDRTData_SolarTermDoorPosition"];
					sRTData.sOBDRTData.strThrottleOpening = mapString[L"OBDRTData_ThrottleOpening"];
					sRTData.sOBDRTData.strCalculationLoad = mapString[L"OBDRTData_CalculationLoad"];
					sRTData.sOBDRTData.strForwardSensorV = mapString[L"OBDRTData_ForwardSensorV"];
					sRTData.sOBDRTData.strForwardSensorI = mapString[L"OBDRTData_ForwardSensorI"];
					sRTData.sOBDRTData.strAirCoefficient = mapString[L"OBDRTData_AirCoefficient"];
					sRTData.sOBDRTData.strMAF = mapString[L"OBDRTData_MAF"];
					sRTData.sOBDRTData.strMAP = mapString[L"OBDRTData_MAP"];

					sRTData.strRemark = mapString[L"Remark"];
					sRTData.strReserved1 = mapString[L"Reserved1"];
					sRTData.strReserved2 = mapString[L"Reserved2"];
					sRTData.strReserved3 = mapString[L"Reserved3"];
					vtRTData.push_back(sRTData);
				}
				while (xmlReader.NextSibling());
			}
		}		
		return vtRTData;
	}
			
	// 计算Hash
	std::wstring CalcHash(const enum HashVer emVer)
	{
		CString strXML;
		strXML.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.AppendFormat(L"<DISResultData>");
		if (emVer >= HV_V1)
		{
			strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", strRunningNumber.c_str());
			strXML.AppendFormat(L"<ReportNumber>%s</ReportNumber>", strReportNumber.c_str());
			strXML.AppendFormat(L"<LimitOfHCHigh>%d</LimitOfHCHigh>", _wtoi(strLimitOfHCHigh.c_str()));
			strXML.AppendFormat(L"<LimitOfCOHigh>%.4f</LimitOfCOHigh>", (float)_wtof(strLimitOfCOHigh.c_str()));
			strXML.AppendFormat(L"<LimitOfHCLow>%d</LimitOfHCLow>", _wtoi(strLimitOfHCLow.c_str()));
			strXML.AppendFormat(L"<LimitOfCOLow>%.4f</LimitOfCOLow>", (float)_wtof(strLimitOfCOLow.c_str()));
			strXML.AppendFormat(L"<LowerLimitOfLambda>%.4f</LowerLimitOfLambda>", (float)_wtof(strLowerLimitOfLambda.c_str()));
			strXML.AppendFormat(L"<UpperLimitOfLambda>%.4f</UpperLimitOfLambda>", (float)_wtof(strUpperLimitOfLambda.c_str()));
			strXML.AppendFormat(L"<RevHigh>%d</RevHigh>", _wtoi(strRevHigh.c_str()));
			strXML.AppendFormat(L"<HCHigh>%d</HCHigh>", _wtoi(strHCHigh.c_str()));
			strXML.AppendFormat(L"<COHigh>%.4f</COHigh>", (float)_wtof(strCOHigh.c_str()));
			strXML.AppendFormat(L"<PassOfHCHigh>%s</PassOfHCHigh>", strPassOfHCHigh.c_str());
			strXML.AppendFormat(L"<PassOfCOHigh>%s</PassOfCOHigh>", strPassOfCOHigh.c_str());
			strXML.AppendFormat(L"<NeedTestLambda>%s</NeedTestLambda>", strNeedTestLambda.c_str());
			strXML.AppendFormat(L"<Lambda>%.4f</Lambda>", (float)_wtof(strLambda.c_str()));
			strXML.AppendFormat(L"<PassOfLambda>%s</PassOfLambda>", strPassOfLambda.c_str());
			strXML.AppendFormat(L"<NeedTestLIS>%s</NeedTestLIS>", strNeedTestLIS.c_str());
			strXML.AppendFormat(L"<HCLow>%d</HCLow>", _wtoi(strHCLow.c_str()));
			strXML.AppendFormat(L"<COLow>%.4f</COLow>", (float)_wtof(strCOLow.c_str()));
			strXML.AppendFormat(L"<PassOfHCLow>%s</PassOfHCLow>", strPassOfHCLow.c_str());
			strXML.AppendFormat(L"<PassOfCOLow>%s</PassOfCOLow>", strPassOfCOLow.c_str());
			strXML.AppendFormat(L"<Pass>%s</Pass>", strPass.c_str());
			strXML.AppendFormat(L"<EnvironmentalTemperature>%.4f</EnvironmentalTemperature>", (float)_wtof(strEnvironmentalTemperature.c_str()));
			strXML.AppendFormat(L"<RelativeHumidity>%.4f</RelativeHumidity>", (float)_wtof(strRelativeHumidity.c_str()));
			strXML.AppendFormat(L"<AtmosphericPressure>%.4f</AtmosphericPressure>", (float)_wtof(strAtmosphericPressure.c_str()));
		}
		if (emVer >= HV_V2)
		{
			// 预留增加字段
		}
		strXML.AppendFormat(L"</DISResultData>");

		std::string strUtf8XML = TCharToUTF8(strXML);
		MD5 hash(strUtf8XML.c_str());
		
		std::wstring strHash = (LPCTSTR)UTF8ToTChar(hash.md5().c_str());
		return strHash;
	}

	// 更新Hash
	void UpdateHash(void)
	{
		enum HashVer emVer = HV_V1;	// 用版本号控制需要验证的字段
		CString strTemp;
		strTemp.Format(L"%d,%s", emVer, CalcHash(emVer).c_str());	//版本号和hash一起保存
		this->strHash = (LPCTSTR)strTemp;
	}

	// 校验Hash
	bool CheckHash(void)
	{
		CString strTemp = this->strHash.c_str();

		// 分解版本号和hash
		enum HashVer emVer = (HashVer)_wtoi(strTemp.Mid(0, strTemp.Find(L",")));	
		std::wstring strHashSave = (LPCTSTR)strTemp.Mid(strTemp.Find(L",")+1);

		std::wstring strHashCalc = CalcHash(emVer);		
		if (_wcsicmp(strHashSave.c_str(), strHashCalc.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

// DIS检测结果<ResultOfDIS>
typedef struct ResultOfDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchLimitOfHCHigh[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCOHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCOHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCLow[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCOLow[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCOLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchLowerLimitOfLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperLimitOfLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfLambda_EDP[NH_INT_TO_WCHART];
	wchar_t wchRevHigh[NH_INT_TO_WCHART];
	wchar_t wchOilTemperatureHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchLambdaHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchNOHigh[NH_INT_TO_WCHART];
	wchar_t wchCO2High[NH_FLOAT_TO_WCHART];
	wchar_t wchO2High[NH_FLOAT_TO_WCHART];
	wchar_t wchHCHigh[NH_INT_TO_WCHART];
	wchar_t wchCOHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHCHigh[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCOHigh[NH_BOOL_TO_WCHART];
	wchar_t wchNeedTestLambda[NH_BOOL_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfLambda[NH_BOOL_TO_WCHART];
	wchar_t wchNeedTestLIS[NH_BOOL_TO_WCHART];
	wchar_t wchRevLow[NH_INT_TO_WCHART];
	wchar_t wchOilTemperatureLow[NH_FLOAT_TO_WCHART];
	wchar_t wchLambdaLow[NH_FLOAT_TO_WCHART];
	wchar_t wchNOLow[NH_INT_TO_WCHART];
	wchar_t wchCO2Low[NH_FLOAT_TO_WCHART];
	wchar_t wchO2Low[NH_FLOAT_TO_WCHART];
	wchar_t wchHCLow[NH_INT_TO_WCHART];
	wchar_t wchCOLow[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHCLow[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCOLow[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfDIS(){ZeroMemory(this, sizeof(ResultOfDIS));}
}RESULTOFDIS, *PRESULTOFDIS;

// DIS real time data<RealTimeDataOfDIS>
typedef struct RealTimeDataOfDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchState[50];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	RealTimeDataOfDIS(){ZeroMemory(this, sizeof(RealTimeDataOfDIS));}
}REALTIMEDATAOFDIS, *PREALTIMEDATAOFDIS;

// MDIS real time data<RealTimeDataOfMDIS>
typedef struct RealTimeDataOfMDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchState[50];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	RealTimeDataOfMDIS(){ZeroMemory(this, sizeof(RealTimeDataOfMDIS));}
}REALTIMEDATAOFMDIS, *PREALTIMEDATAOFMDIS;

// MDIS检测结果<ResultOfMDIS>
typedef struct ResultOfMDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchLimitOfHCHigh[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCOHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCOHigh_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCLow[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCOLow[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCOLow_EDP[NH_INT_TO_WCHART];
	wchar_t wchSkipHISJudgement[NH_BOOL_TO_WCHART];
	wchar_t wchLowerLimitOfLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperLimitOfLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfLambda_EDP[NH_INT_TO_WCHART];
	wchar_t wchRevHigh[NH_INT_TO_WCHART];
	wchar_t wchOilTemperatureHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchLambdaHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchNOHigh[NH_INT_TO_WCHART];
	wchar_t wchCO2High[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2HighRound[NH_FLOAT_TO_WCHART];
	wchar_t wchO2High[NH_FLOAT_TO_WCHART];
	wchar_t wchHCHigh[NH_INT_TO_WCHART];
	wchar_t wchHCHighRound[NH_INT_TO_WCHART];
	wchar_t wchCOHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchCOHighAmend[NH_FLOAT_TO_WCHART];
	wchar_t wchCOHighRound[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHCHigh[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCOHigh[NH_BOOL_TO_WCHART];
	wchar_t wchNeedTestLambda[NH_BOOL_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfLambda[NH_BOOL_TO_WCHART];
	wchar_t wchRevLow[NH_INT_TO_WCHART];
	wchar_t wchOilTemperatureLow[NH_FLOAT_TO_WCHART];
	wchar_t wchLambdaLow[NH_FLOAT_TO_WCHART];
	wchar_t wchNOLow[NH_INT_TO_WCHART];
	wchar_t wchCO2Low[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2LowRound[NH_FLOAT_TO_WCHART];
	wchar_t wchO2Low[NH_FLOAT_TO_WCHART];
	wchar_t wchHCLow[NH_INT_TO_WCHART];
	wchar_t wchHCLowRound[NH_INT_TO_WCHART];
	wchar_t wchCOLow[NH_FLOAT_TO_WCHART];
	wchar_t wchCOLowAmend[NH_FLOAT_TO_WCHART];
	wchar_t wchCOLowRound[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHCLow[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCOLow[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfMDIS(){ZeroMemory(this, sizeof(ResultOfMDIS));}
}RESULTOFMDIS, *PRESULTOFMDIS;

// DIS检测结果<ResultOfMI>
typedef struct ResultOfMI
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchLimitOfHC[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHC_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCO[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCO_EDP[NH_INT_TO_WCHART];
	wchar_t wchRev_1[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature_1[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda_1[NH_FLOAT_TO_WCHART];
	wchar_t wchNO_1[NH_INT_TO_WCHART];
	wchar_t wchCO2_1[NH_FLOAT_TO_WCHART];
	wchar_t wchO2_1[NH_FLOAT_TO_WCHART];
	wchar_t wchHC_1[NH_INT_TO_WCHART];
	wchar_t wchCO_1[NH_FLOAT_TO_WCHART];
	wchar_t wchRev_2[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature_2[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda_2[NH_FLOAT_TO_WCHART];
	wchar_t wchNO_2[NH_INT_TO_WCHART];
	wchar_t wchCO2_2[NH_FLOAT_TO_WCHART];
	wchar_t wchO2_2[NH_FLOAT_TO_WCHART];
	wchar_t wchHC_2[NH_INT_TO_WCHART];
	wchar_t wchCO_2[NH_FLOAT_TO_WCHART];
	wchar_t wchHCAverage[NH_INT_TO_WCHART];
	wchar_t wchCOAverage[NH_FLOAT_TO_WCHART];
	wchar_t wchLambdaAverage[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHC[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCO[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfMI(){ZeroMemory(this, sizeof(ResultOfMI));}
}RESULTOFMI, *PRESULTOFMI;

// MI real time data<RealTimeDataOfMI>
typedef struct RealTimeDataOfMI
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchState[50];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	RealTimeDataOfMI(){ZeroMemory(this, sizeof(RealTimeDataOfMI));}
}REALTIMEDATAOFMI, *PREALTIMEDATAOFMI;

// 此结构体数据以XML格式存入SResultOfASM的二进制字段中
struct SRealTimeDataOfASM
{
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strTime;
	std::wstring strSamplingTime;
	std::wstring strVelocity;
	std::wstring strPower;
	std::wstring strForce;
	std::wstring strIHP;
	std::wstring strPLHP;
	std::wstring strState;
	std::wstring strHC;
	std::wstring strCO;
	std::wstring strNO;
	std::wstring strO2;
	std::wstring strCO2;
	std::wstring strEngineRev;
	std::wstring strOilTemperature;
	std::wstring strLambda;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strHumidityCorrectionFactor;
	std::wstring strDilutionCorrectionFactor;
	std::wstring strIsAdoptedResult;
	SOBDRTData sOBDRTData;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};

struct SResultOfASM
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strLimitOfHC5025;
	std::wstring strLimitOfHC5025_EDP;
	std::wstring strLimitOfCO5025;
	std::wstring strLimitOfCO5025_EDP;
	std::wstring strLimitOfNO5025;
	std::wstring strLimitOfNO5025_EDP;
	std::wstring strLimitOfHC2540;
	std::wstring strLimitOfHC2540_EDP;
	std::wstring strLimitOfCO2540;
	std::wstring strLimitOfCO2540_EDP;
	std::wstring strLimitOfNO2540;
	std::wstring strLimitOfNO2540_EDP;
	std::wstring strRev5025;
	std::wstring strOilTemperature5025;
	std::wstring strLambda5025;
	std::wstring strPower5025;
	std::wstring strHC5025;
	std::wstring strCO5025;
	std::wstring strNO5025;
	std::wstring strCO25025;
	std::wstring strO25025;
	std::wstring strPassOfHC5025;
	std::wstring strPassOfCO5025;
	std::wstring strPassOfNO5025;
	std::wstring strNeedTestMode2540;
	std::wstring strRev2540;
	std::wstring strOilTemperature2540;
	std::wstring strLambda2540;
	std::wstring strPower2540;
	std::wstring strHC2540;
	std::wstring strCO2540;
	std::wstring strNO2540;
	std::wstring strCO22540;
	std::wstring strO22540;
	std::wstring strPassOfHC2540;
	std::wstring strPassOfCO2540;
	std::wstring strPassOfNO2540;
	std::wstring strPass;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strSkipRev;
	std::wstring strSkipOilTemperature;
	// RTDataZip，过程数据，以XML格式封装，zip算法压缩
	std::string strRTDataZip;
	// Hash数据校验
	std::wstring strHash;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;

	// 压缩过程数据
	void RTDataZip(std::vector<SRealTimeDataOfASM> &vtRTData)
	{
		std::wstring strXML;
		strXML.append(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.append(L"<ASMRTData>");
		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strXML.append(L"<Row>");
			strXML.append(L"<RunningNumber>" + vtRTData[i].strRunningNumber + L"</RunningNumber>");
			strXML.append(L"<ReportNumber>" + vtRTData[i].strReportNumber + L"</ReportNumber>");
			strXML.append(L"<FuelTypeCode>" + vtRTData[i].strFuelTypeCode + L"</FuelTypeCode>");
			strXML.append(L"<FuelTypeName>" + vtRTData[i].strFuelTypeName + L"</FuelTypeName>");
			strXML.append(L"<Time>" + vtRTData[i].strTime + L"</Time>");
			strXML.append(L"<SamplingTime>" + vtRTData[i].strSamplingTime + L"</SamplingTime>");
			strXML.append(L"<Velocity>" + vtRTData[i].strVelocity + L"</Velocity>");
			strXML.append(L"<Power>" + vtRTData[i].strPower + L"</Power>");
			strXML.append(L"<Force>" + vtRTData[i].strForce + L"</Force>");
			strXML.append(L"<IHP>" + vtRTData[i].strIHP + L"</IHP>");
			strXML.append(L"<PLHP>" + vtRTData[i].strPLHP + L"</PLHP>");
			strXML.append(L"<State>" + vtRTData[i].strState + L"</State>");
			strXML.append(L"<HC>" + vtRTData[i].strHC + L"</HC>");
			strXML.append(L"<CO>" + vtRTData[i].strCO + L"</CO>");
			strXML.append(L"<NO>" + vtRTData[i].strNO + L"</NO>");
			strXML.append(L"<O2>" + vtRTData[i].strO2 + L"</O2>");
			strXML.append(L"<CO2>" + vtRTData[i].strCO2 + L"</CO2>");
			strXML.append(L"<EngineRev>" + vtRTData[i].strEngineRev + L"</EngineRev>");
			strXML.append(L"<OilTemperature>" + vtRTData[i].strOilTemperature + L"</OilTemperature>");
			strXML.append(L"<Lambda>" + vtRTData[i].strLambda + L"</Lambda>");
			strXML.append(L"<EnvironmentalTemperature>" + vtRTData[i].strEnvironmentalTemperature + L"</EnvironmentalTemperature>");
			strXML.append(L"<RelativeHumidity>" + vtRTData[i].strRelativeHumidity + L"</RelativeHumidity>");
			strXML.append(L"<AtmosphericPressure>" + vtRTData[i].strAtmosphericPressure + L"</AtmosphericPressure>");
			strXML.append(L"<HumidityCorrectionFactor>" + vtRTData[i].strHumidityCorrectionFactor + L"</HumidityCorrectionFactor>");
			strXML.append(L"<DilutionCorrectionFactor>" + vtRTData[i].strDilutionCorrectionFactor + L"</DilutionCorrectionFactor>");
			strXML.append(L"<IsAdoptedResult>" + vtRTData[i].strIsAdoptedResult + L"</IsAdoptedResult>");
			
			// 汽油车的OBD数据流
			strXML.append(L"<OBDRTData_Velocity>" + vtRTData[i].sOBDRTData.strVelocity + L"</OBDRTData_Velocity>");
			strXML.append(L"<OBDRTData_CoolantTemp>" + vtRTData[i].sOBDRTData.strCoolantTemp + L"</OBDRTData_CoolantTemp>");
			strXML.append(L"<OBDRTData_EngineRev>" + vtRTData[i].sOBDRTData.strEngineRev + L"</OBDRTData_EngineRev>");
			strXML.append(L"<OBDRTData_SolarTermDoorPosition>" + vtRTData[i].sOBDRTData.strSolarTermDoorPosition + L"</OBDRTData_SolarTermDoorPosition>");
			strXML.append(L"<OBDRTData_ThrottleOpening>" + vtRTData[i].sOBDRTData.strThrottleOpening + L"</OBDRTData_ThrottleOpening>");
			strXML.append(L"<OBDRTData_CalculationLoad>" + vtRTData[i].sOBDRTData.strCalculationLoad + L"</OBDRTData_CalculationLoad>");
			strXML.append(L"<OBDRTData_ForwardSensorV>" + vtRTData[i].sOBDRTData.strForwardSensorV + L"</OBDRTData_ForwardSensorV>");
			strXML.append(L"<OBDRTData_ForwardSensorI>" + vtRTData[i].sOBDRTData.strForwardSensorI + L"</OBDRTData_ForwardSensorI>");
			strXML.append(L"<OBDRTData_AirCoefficient>" + vtRTData[i].sOBDRTData.strAirCoefficient + L"</OBDRTData_AirCoefficient>");
			strXML.append(L"<OBDRTData_MAF>" + vtRTData[i].sOBDRTData.strMAF + L"</OBDRTData_MAF>");
			strXML.append(L"<OBDRTData_MAP>" + vtRTData[i].sOBDRTData.strMAP + L"</OBDRTData_MAP>");

			strXML.append(L"<Remark>" + vtRTData[i].strRemark + L"</Remark>");
			strXML.append(L"<Reserved1>" + vtRTData[i].strReserved1 + L"</Reserved1>");
			strXML.append(L"<Reserved2>" + vtRTData[i].strReserved2 + L"</Reserved2>");
			strXML.append(L"<Reserved3>" + vtRTData[i].strReserved3 + L"</Reserved3>");
			strXML.append(L"</Row>");
		}
		strXML.append(L"</ASMRTData>");
				
		std::string strUtf8XML = TCharToUTF8(strXML.c_str());
		CZipLib zip;
		strRTDataZip = zip.ZipData(strUtf8XML.data());
	}

	// 解压过程数据
	std::vector<SRealTimeDataOfASM> RTDataUpZip(void)
	{
		std::vector<SRealTimeDataOfASM> vtRTData;

		CZipLib zip;		
		std::wstring strXML = (LPCTSTR)UTF8ToTChar(zip.UnZipData(strRTDataZip).c_str());
		
		CXmlReader xmlReader;
		if (xmlReader.Parse(strXML.c_str()))
		{
			if (xmlReader.OpenNode(L"ASMRTData/Row"))
			{
				do
				{			
					std::map<std::wstring, std::wstring> mapString;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapString[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();

					SRealTimeDataOfASM sRTData;
					sRTData.strRunningNumber = mapString[L"RunningNumber"];
					sRTData.strReportNumber = mapString[L"ReportNumber"];
					sRTData.strFuelTypeCode = mapString[L"FuelTypeCode"];
					sRTData.strFuelTypeName = mapString[L"FuelTypeName"];
					sRTData.strTime = mapString[L"Time"];
					sRTData.strSamplingTime = mapString[L"SamplingTime"];
					sRTData.strVelocity = mapString[L"Velocity"];
					sRTData.strPower = mapString[L"Power"];
					sRTData.strForce = mapString[L"Force"];
					sRTData.strIHP = mapString[L"IHP"];
					sRTData.strPLHP = mapString[L"PLHP"];
					sRTData.strState = mapString[L"State"];
					sRTData.strHC = mapString[L"HC"];
					sRTData.strCO = mapString[L"CO"];
					sRTData.strNO = mapString[L"NO"];
					sRTData.strO2 = mapString[L"O2"];
					sRTData.strCO2 = mapString[L"CO2"];
					sRTData.strEngineRev = mapString[L"EngineRev"];
					sRTData.strOilTemperature = mapString[L"OilTemperature"];
					sRTData.strLambda = mapString[L"Lambda"];
					sRTData.strEnvironmentalTemperature = mapString[L"EnvironmentalTemperature"];
					sRTData.strRelativeHumidity = mapString[L"RelativeHumidity"];
					sRTData.strAtmosphericPressure = mapString[L"AtmosphericPressure"];
					sRTData.strHumidityCorrectionFactor = mapString[L"HumidityCorrectionFactor"];
					sRTData.strDilutionCorrectionFactor = mapString[L"DilutionCorrectionFactor"];
					sRTData.strIsAdoptedResult = mapString[L"IsAdoptedResult"];
					
					// 汽油车的OBD数据流
					sRTData.sOBDRTData.strVelocity = mapString[L"OBDRTData_Velocity"];
					sRTData.sOBDRTData.strCoolantTemp = mapString[L"OBDRTData_CoolantTemp"];
					sRTData.sOBDRTData.strEngineRev = mapString[L"OBDRTData_EngineRev"];
					sRTData.sOBDRTData.strSolarTermDoorPosition = mapString[L"OBDRTData_SolarTermDoorPosition"];
					sRTData.sOBDRTData.strThrottleOpening = mapString[L"OBDRTData_ThrottleOpening"];
					sRTData.sOBDRTData.strCalculationLoad = mapString[L"OBDRTData_CalculationLoad"];
					sRTData.sOBDRTData.strForwardSensorV = mapString[L"OBDRTData_ForwardSensorV"];
					sRTData.sOBDRTData.strForwardSensorI = mapString[L"OBDRTData_ForwardSensorI"];
					sRTData.sOBDRTData.strAirCoefficient = mapString[L"OBDRTData_AirCoefficient"];
					sRTData.sOBDRTData.strMAF = mapString[L"OBDRTData_MAF"];
					sRTData.sOBDRTData.strMAP = mapString[L"OBDRTData_MAP"];

					sRTData.strRemark = mapString[L"Remark"];
					sRTData.strReserved1 = mapString[L"Reserved1"];
					sRTData.strReserved2 = mapString[L"Reserved2"];
					sRTData.strReserved3 = mapString[L"Reserved3"];
					vtRTData.push_back(sRTData);
				}
				while (xmlReader.NextSibling());
			}
		}		
		return vtRTData;
	}

	// 计算Hash
	std::wstring CalcHash(const enum HashVer emVer)
	{
		CString strXML;
		strXML.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.AppendFormat(L"<ASMResultData>");
		if (emVer >= HV_V1)
		{
			strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", strRunningNumber.c_str());
			strXML.AppendFormat(L"<ReportNumber>%s</ReportNumber>", strReportNumber.c_str());
			strXML.AppendFormat(L"<LimitOfHC5025>%d</LimitOfHC5025>", _wtoi(strLimitOfHC5025.c_str()));
			strXML.AppendFormat(L"<LimitOfCO5025>%.4f</LimitOfCO5025>", (float)_wtof(strLimitOfCO5025.c_str()));
			strXML.AppendFormat(L"<LimitOfNO5025>%d</LimitOfNO5025>", _wtoi(strLimitOfNO5025.c_str()));
			strXML.AppendFormat(L"<LimitOfHC2540>%d</LimitOfHC2540>", _wtoi(strLimitOfHC2540.c_str()));
			strXML.AppendFormat(L"<LimitOfCO2540>%.4f</LimitOfCO2540>", (float)_wtof(strLimitOfCO2540.c_str()));
			strXML.AppendFormat(L"<LimitOfNO2540>%d</LimitOfNO2540>", _wtoi(strLimitOfNO2540.c_str()));
			strXML.AppendFormat(L"<HC5025>%d</HC5025>", _wtoi(strHC5025.c_str()));
			strXML.AppendFormat(L"<CO5025>%.4f</CO5025>", (float)_wtof(strCO5025.c_str()));
			strXML.AppendFormat(L"<NO5025>%d</NO5025>", _wtoi(strNO5025.c_str()));
			strXML.AppendFormat(L"<CO25025>%.4f</CO25025>", (float)_wtof(strCO25025.c_str()));
			strXML.AppendFormat(L"<O25025>%.4f</O25025>", (float)_wtof(strO25025.c_str()));
			strXML.AppendFormat(L"<PassOfHC5025>%s</PassOfHC5025>", strPassOfHC5025.c_str());
			strXML.AppendFormat(L"<PassOfCO5025>%s</PassOfCO5025>", strPassOfCO5025.c_str());
			strXML.AppendFormat(L"<PassOfNO5025>%s</PassOfNO5025>", strPassOfNO5025.c_str());
			strXML.AppendFormat(L"<HC2540>%d</HC2540>", _wtoi(strHC2540.c_str()));
			strXML.AppendFormat(L"<CO2540>%.4f</CO2540>", (float)_wtof(strCO2540.c_str()));
			strXML.AppendFormat(L"<NO2540>%d</NO2540>", _wtoi(strNO2540.c_str()));
			strXML.AppendFormat(L"<CO22540>%.4f</CO22540>", (float)_wtof(strCO22540.c_str()));
			strXML.AppendFormat(L"<O22540>%.4f</O22540>", (float)_wtof(strO22540.c_str()));
			strXML.AppendFormat(L"<PassOfHC2540>%s</PassOfHC2540>", strPassOfHC2540.c_str());
			strXML.AppendFormat(L"<PassOfCO2540>%s</PassOfCO2540>", strPassOfCO2540.c_str());
			strXML.AppendFormat(L"<PassOfNO2540>%s</PassOfNO2540>", strPassOfNO2540.c_str());
			strXML.AppendFormat(L"<Pass>%s</Pass>", strPass.c_str());
			strXML.AppendFormat(L"<EnvironmentalTemperature>%.4f</EnvironmentalTemperature>", (float)_wtof(strEnvironmentalTemperature.c_str()));
			strXML.AppendFormat(L"<RelativeHumidity>%.4f</RelativeHumidity>", (float)_wtof(strRelativeHumidity.c_str()));
			strXML.AppendFormat(L"<AtmosphericPressure>%.4f</AtmosphericPressure>", (float)_wtof(strAtmosphericPressure.c_str()));
		}
		if (emVer >= HV_V2)
		{
			// 预留增加字段
		}
		strXML.AppendFormat(L"</ASMResultData>");

		std::string strUtf8XML = TCharToUTF8(strXML);
		MD5 hash(strUtf8XML.c_str());
		
		std::wstring strHash = (LPCTSTR)UTF8ToTChar(hash.md5().c_str());
		return strHash;
	}

	// 更新Hash
	void UpdateHash(void)
	{
		enum HashVer emVer = HV_V1;	// 用版本号控制需要验证的字段
		CString strTemp;
		strTemp.Format(L"%d,%s", emVer, CalcHash(emVer).c_str());	//版本号和hash一起保存
		this->strHash = (LPCTSTR)strTemp;
	}

	// 校验Hash
	bool CheckHash(void)
	{
		CString strTemp = this->strHash.c_str();

		// 分解版本号和hash
		enum HashVer emVer = (HashVer)_wtoi(strTemp.Mid(0, strTemp.Find(L",")));	
		std::wstring strHashSave = (LPCTSTR)strTemp.Mid(strTemp.Find(L",")+1);

		std::wstring strHashCalc = CalcHash(emVer);		
		if (_wcsicmp(strHashSave.c_str(), strHashCalc.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

// ASM result data<ResultOfASM>
typedef struct ResultOfASM
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchLimitOfHC5025[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHC5025_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCO5025[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCO5025_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfNO5025[NH_INT_TO_WCHART];
	wchar_t wchLimitOfNO5025_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHC2540[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHC2540_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCO2540[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCO2540_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfNO2540[NH_INT_TO_WCHART];
	wchar_t wchLimitOfNO2540_EDP[NH_INT_TO_WCHART];
	wchar_t wchRev5025[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature5025[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda5025[NH_FLOAT_TO_WCHART];
	wchar_t wchPower5025[NH_FLOAT_TO_WCHART];
	wchar_t wchHC5025[NH_INT_TO_WCHART];
	wchar_t wchCO5025[NH_FLOAT_TO_WCHART];
	wchar_t wchNO5025[NH_INT_TO_WCHART];
	wchar_t wchCO25025[NH_FLOAT_TO_WCHART];
	wchar_t wchO25025[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHC5025[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCO5025[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfNO5025[NH_BOOL_TO_WCHART];
	wchar_t wchNeedTestMode2540[NH_BOOL_TO_WCHART];
	wchar_t wchRev2540[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature2540[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda2540[NH_FLOAT_TO_WCHART];
	wchar_t wchPower2540[NH_FLOAT_TO_WCHART];
	wchar_t wchHC2540[NH_INT_TO_WCHART];
	wchar_t wchCO2540[NH_FLOAT_TO_WCHART];
	wchar_t wchNO2540[NH_INT_TO_WCHART];
	wchar_t wchCO22540[NH_FLOAT_TO_WCHART];
	wchar_t wchO22540[NH_FLOAT_TO_WCHART];
	wchar_t wchPassOfHC2540[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCO2540[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfNO2540[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfASM(){ZeroMemory(this, sizeof(ResultOfASM));}
}RESULTOFASM, *PRESULTOFASM;


// ASM real time data<RealTimeDataOfASM>
typedef struct RealTimeDataOfASM
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchPower[NH_FLOAT_TO_WCHART];
	wchar_t wchForce[NH_FLOAT_TO_WCHART];
	wchar_t wchIHP[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHP[NH_FLOAT_TO_WCHART];
	wchar_t wchState[50];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchHumidityCorrectionFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchDilutionCorrectionFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchIsAdoptedResult[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	RealTimeDataOfASM(){ZeroMemory(this, sizeof(RealTimeDataOfASM));}
}REALTIMEDATAOFASM, *PREALTIMEDATAOFASM;

// VMAS result data<ResultOfVMAS>
typedef struct ResultOfVMAS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchNeedTestHCNOx[NH_BOOL_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchHC[NH_FLOAT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchHCNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfHC[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfHC_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfCO[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfCO_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfNOx_EDP[NH_INT_TO_WCHART];
	wchar_t wchLimitOfHCNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchLimitOfHCNOx_EDP[NH_INT_TO_WCHART];
	wchar_t wchPassOfHC[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfCO[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfNOx[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfHCNOx[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchPower50[NH_FLOAT_TO_WCHART];
	wchar_t wchTravelledDistance[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchVolOfGas[NH_FLOAT_TO_WCHART];
	wchar_t wchVolOfExhaust[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfVMAS(){ZeroMemory(this, sizeof(ResultOfVMAS));}
}RESULTOFVMAS, *PRESULTOFVMAS;

// 此结构体数据以XML格式存入SResultOfVMAS的二进制字段中
struct SRealTimeDataOfVMAS
{
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strTime;
	std::wstring strSamplingTime;
	std::wstring strVelocity;
	std::wstring strPower;
	std::wstring strForce;
	std::wstring strIHP;
	std::wstring strPLHP;
	std::wstring strHC;
	std::wstring strCO;
	std::wstring strNO;
	std::wstring strNO2;
	std::wstring strNOx;
	std::wstring strO2;
	std::wstring strCO2;
	std::wstring strEngineRev;
	std::wstring strLambda;
	std::wstring strOilTemperature;
	std::wstring strO2OfFlowmeter;
	std::wstring strO2OfEnvironment;
	std::wstring strHCm;
	std::wstring strCOm;
	std::wstring strNOm;
	std::wstring strNO2m;
	std::wstring strNOxm;
	std::wstring strO2m;
	std::wstring strCO2m;
	std::wstring strFuleConsumption;
	std::wstring strPressureOfFlowmeter;
	std::wstring strTemperatureOfFlowmeter;
	std::wstring strDR;
	std::wstring strActualFluxOfGas;
	std::wstring strStandardFluxOfGas;
	std::wstring strFluxOfExhaust;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strHumidityCorrectionFactor;
	std::wstring strDilutionCorrectionFactor;
	std::wstring strInertiaSimulationError;
	SOBDRTData sOBDRTData;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};

struct SResultOfVMAS
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strNeedTestHCNOx;
	std::wstring strO2;
	std::wstring strCO2;
	std::wstring strHC;
	std::wstring strCO;
	std::wstring strNOx;
	std::wstring strHCNOx;
	std::wstring strLimitOfHC;
	std::wstring strLimitOfHC_EDP;
	std::wstring strLimitOfCO;
	std::wstring strLimitOfCO_EDP;
	std::wstring strLimitOfNOx;
	std::wstring strLimitOfNOx_EDP;
	std::wstring strLimitOfHCNOx;
	std::wstring strLimitOfHCNOx_EDP;
	std::wstring strPassOfHC;
	std::wstring strPassOfCO;
	std::wstring strPassOfNOx;
	std::wstring strPassOfHCNOx;
	std::wstring strPass;
	std::wstring strPower50;
	std::wstring strTravelledDistance;
	std::wstring strFuleConsumptionPer100KM;
	std::wstring strDeviateTime;
	std::wstring strDeviateCount;
	std::wstring strLambda;
	std::wstring strOilTemperature;
	std::wstring strVolOfGas;
	std::wstring strVolOfExhaust;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strSkipRev;
	std::wstring strSkipOilTemperature;
	// RTDataZip，过程数据，以XML格式封装，zip算法压缩
	std::string strRTDataZip;
	// Hash数据校验
	std::wstring strHash;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
	
	// 压缩过程数据
	void RTDataZip(std::vector<SRealTimeDataOfVMAS> &vtRTData)
	{
		std::wstring strXML;
		strXML.append(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.append(L"<VMASRTData>");
		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strXML.append(L"<Row>");
			strXML.append(L"<RunningNumber>" + vtRTData[i].strRunningNumber + L"</RunningNumber>");
			strXML.append(L"<ReportNumber>" + vtRTData[i].strReportNumber + L"</ReportNumber>");
			strXML.append(L"<FuelTypeCode>" + vtRTData[i].strFuelTypeCode + L"</FuelTypeCode>");
			strXML.append(L"<FuelTypeName>" + vtRTData[i].strFuelTypeName + L"</FuelTypeName>");
			strXML.append(L"<Time>" + vtRTData[i].strTime + L"</Time>");
			strXML.append(L"<SamplingTime>" + vtRTData[i].strSamplingTime + L"</SamplingTime>");
			strXML.append(L"<Velocity>" + vtRTData[i].strVelocity + L"</Velocity>");
			strXML.append(L"<Power>" + vtRTData[i].strPower + L"</Power>");
			strXML.append(L"<Force>" + vtRTData[i].strForce + L"</Force>");
			strXML.append(L"<IHP>" + vtRTData[i].strIHP + L"</IHP>");
			strXML.append(L"<PLHP>" + vtRTData[i].strPLHP + L"</PLHP>");
			strXML.append(L"<HC>" + vtRTData[i].strHC + L"</HC>");
			strXML.append(L"<CO>" + vtRTData[i].strCO + L"</CO>");
			strXML.append(L"<NO>" + vtRTData[i].strNO + L"</NO>");
			strXML.append(L"<NO2>" + vtRTData[i].strNO2 + L"</NO2>");
			strXML.append(L"<NOx>" + vtRTData[i].strNOx + L"</NOx>");
			strXML.append(L"<O2>" + vtRTData[i].strO2 + L"</O2>");
			strXML.append(L"<CO2>" + vtRTData[i].strCO2 + L"</CO2>");
			strXML.append(L"<EngineRev>" + vtRTData[i].strEngineRev + L"</EngineRev>");
			strXML.append(L"<Lambda>" + vtRTData[i].strLambda + L"</Lambda>");
			strXML.append(L"<OilTemperature>" + vtRTData[i].strOilTemperature + L"</OilTemperature>");
			strXML.append(L"<O2OfFlowmeter>" + vtRTData[i].strO2OfFlowmeter + L"</O2OfFlowmeter>");
			strXML.append(L"<O2OfEnvironment>" + vtRTData[i].strO2OfEnvironment + L"</O2OfEnvironment>");
			strXML.append(L"<HCm>" + vtRTData[i].strHCm + L"</HCm>");
			strXML.append(L"<COm>" + vtRTData[i].strCOm + L"</COm>");
			strXML.append(L"<NOm>" + vtRTData[i].strNOm + L"</NOm>");
			strXML.append(L"<NO2m>" + vtRTData[i].strNO2m + L"</NO2m>");
			strXML.append(L"<NOxm>" + vtRTData[i].strNOxm + L"</NOxm>");
			strXML.append(L"<O2m>" + vtRTData[i].strO2m + L"</O2m>");
			strXML.append(L"<CO2m>" + vtRTData[i].strCO2m + L"</CO2m>");
			strXML.append(L"<FuleConsumption>" + vtRTData[i].strFuleConsumption + L"</FuleConsumption>");
			strXML.append(L"<PressureOfFlowmeter>" + vtRTData[i].strPressureOfFlowmeter + L"</PressureOfFlowmeter>");
			strXML.append(L"<TemperatureOfFlowmeter>" + vtRTData[i].strTemperatureOfFlowmeter + L"</TemperatureOfFlowmeter>");
			strXML.append(L"<DR>" + vtRTData[i].strDR + L"</DR>");
			strXML.append(L"<ActualFluxOfGas>" + vtRTData[i].strActualFluxOfGas + L"</ActualFluxOfGas>");
			strXML.append(L"<StandardFluxOfGas>" + vtRTData[i].strStandardFluxOfGas + L"</StandardFluxOfGas>");
			strXML.append(L"<FluxOfExhaust>" + vtRTData[i].strFluxOfExhaust + L"</FluxOfExhaust>");
			strXML.append(L"<EnvironmentalTemperature>" + vtRTData[i].strEnvironmentalTemperature + L"</EnvironmentalTemperature>");
			strXML.append(L"<RelativeHumidity>" + vtRTData[i].strRelativeHumidity + L"</RelativeHumidity>");
			strXML.append(L"<AtmosphericPressure>" + vtRTData[i].strAtmosphericPressure + L"</AtmosphericPressure>");
			strXML.append(L"<HumidityCorrectionFactor>" + vtRTData[i].strHumidityCorrectionFactor + L"</HumidityCorrectionFactor>");
			strXML.append(L"<DilutionCorrectionFactor>" + vtRTData[i].strDilutionCorrectionFactor + L"</DilutionCorrectionFactor>");
			strXML.append(L"<InertiaSimulationError>" + vtRTData[i].strInertiaSimulationError + L"</InertiaSimulationError>");
			
			// 汽油车的OBD数据流
			strXML.append(L"<OBDRTData_Velocity>" + vtRTData[i].sOBDRTData.strVelocity + L"</OBDRTData_Velocity>");
			strXML.append(L"<OBDRTData_CoolantTemp>" + vtRTData[i].sOBDRTData.strCoolantTemp + L"</OBDRTData_CoolantTemp>");
			strXML.append(L"<OBDRTData_EngineRev>" + vtRTData[i].sOBDRTData.strEngineRev + L"</OBDRTData_EngineRev>");
			strXML.append(L"<OBDRTData_SolarTermDoorPosition>" + vtRTData[i].sOBDRTData.strSolarTermDoorPosition + L"</OBDRTData_SolarTermDoorPosition>");
			strXML.append(L"<OBDRTData_ThrottleOpening>" + vtRTData[i].sOBDRTData.strThrottleOpening + L"</OBDRTData_ThrottleOpening>");
			strXML.append(L"<OBDRTData_CalculationLoad>" + vtRTData[i].sOBDRTData.strCalculationLoad + L"</OBDRTData_CalculationLoad>");
			strXML.append(L"<OBDRTData_ForwardSensorV>" + vtRTData[i].sOBDRTData.strForwardSensorV + L"</OBDRTData_ForwardSensorV>");
			strXML.append(L"<OBDRTData_ForwardSensorI>" + vtRTData[i].sOBDRTData.strForwardSensorI + L"</OBDRTData_ForwardSensorI>");
			strXML.append(L"<OBDRTData_AirCoefficient>" + vtRTData[i].sOBDRTData.strAirCoefficient + L"</OBDRTData_AirCoefficient>");
			strXML.append(L"<OBDRTData_MAF>" + vtRTData[i].sOBDRTData.strMAF + L"</OBDRTData_MAF>");
			strXML.append(L"<OBDRTData_MAP>" + vtRTData[i].sOBDRTData.strMAP + L"</OBDRTData_MAP>");

			strXML.append(L"<Remark>" + vtRTData[i].strRemark + L"</Remark>");
			strXML.append(L"<Reserved1>" + vtRTData[i].strReserved1 + L"</Reserved1>");
			strXML.append(L"<Reserved2>" + vtRTData[i].strReserved2 + L"</Reserved2>");
			strXML.append(L"<Reserved3>" + vtRTData[i].strReserved3 + L"</Reserved3>");
			strXML.append(L"</Row>");
		}
		strXML.append(L"</VMASRTData>");
				
		std::string strUtf8XML = TCharToUTF8(strXML.c_str());
		CZipLib zip;
		strRTDataZip = zip.ZipData(strUtf8XML.data());
	}

	// 解压过程数据
	std::vector<SRealTimeDataOfVMAS> RTDataUpZip(void)
	{
		std::vector<SRealTimeDataOfVMAS> vtRTData;

		CZipLib zip;		
		std::wstring strXML = (LPCTSTR)UTF8ToTChar(zip.UnZipData(strRTDataZip).c_str());
		
		CXmlReader xmlReader;
		if (xmlReader.Parse(strXML.c_str()))
		{
			if (xmlReader.OpenNode(L"VMASRTData/Row"))
			{
				do
				{			
					std::map<std::wstring, std::wstring> mapString;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapString[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();

					SRealTimeDataOfVMAS sRTData;
					sRTData.strRunningNumber = mapString[L"RunningNumber"];
					sRTData.strReportNumber = mapString[L"ReportNumber"];
					sRTData.strFuelTypeCode = mapString[L"FuelTypeCode"];
					sRTData.strFuelTypeName = mapString[L"FuelTypeName"];
					sRTData.strTime = mapString[L"Time"];
					sRTData.strSamplingTime = mapString[L"SamplingTime"];
					sRTData.strVelocity = mapString[L"Velocity"];
					sRTData.strPower = mapString[L"Power"];
					sRTData.strForce = mapString[L"Force"];
					sRTData.strIHP = mapString[L"IHP"];
					sRTData.strPLHP = mapString[L"PLHP"];
					sRTData.strHC = mapString[L"HC"];
					sRTData.strCO = mapString[L"CO"];
					sRTData.strNO = mapString[L"NO"];
					sRTData.strNO2 = mapString[L"NO2"];
					sRTData.strNOx = mapString[L"NOx"];
					sRTData.strO2 = mapString[L"O2"];
					sRTData.strCO2 = mapString[L"CO2"];
					sRTData.strEngineRev = mapString[L"EngineRev"];
					sRTData.strLambda = mapString[L"Lambda"];
					sRTData.strOilTemperature = mapString[L"OilTemperature"];
					sRTData.strO2OfFlowmeter = mapString[L"O2OfFlowmeter"];
					sRTData.strO2OfEnvironment = mapString[L"O2OfEnvironment"];
					sRTData.strHCm = mapString[L"HCm"];
					sRTData.strCOm = mapString[L"COm"];
					sRTData.strNOm = mapString[L"NOm"];
					sRTData.strNO2m = mapString[L"NO2m"];
					sRTData.strNOxm = mapString[L"NOxm"];
					sRTData.strO2m = mapString[L"O2m"];
					sRTData.strCO2m = mapString[L"CO2m"];
					sRTData.strFuleConsumption = mapString[L"FuleConsumption"];
					sRTData.strPressureOfFlowmeter = mapString[L"PressureOfFlowmeter"];
					sRTData.strTemperatureOfFlowmeter = mapString[L"TemperatureOfFlowmeter"];
					sRTData.strDR = mapString[L"DR"];
					sRTData.strActualFluxOfGas = mapString[L"ActualFluxOfGas"];
					sRTData.strStandardFluxOfGas = mapString[L"StandardFluxOfGas"];
					sRTData.strFluxOfExhaust = mapString[L"FluxOfExhaust"];
					sRTData.strEnvironmentalTemperature = mapString[L"EnvironmentalTemperature"];
					sRTData.strRelativeHumidity = mapString[L"RelativeHumidity"];
					sRTData.strAtmosphericPressure = mapString[L"AtmosphericPressure"];
					sRTData.strHumidityCorrectionFactor = mapString[L"HumidityCorrectionFactor"];
					sRTData.strDilutionCorrectionFactor = mapString[L"DilutionCorrectionFactor"];
					sRTData.strInertiaSimulationError = mapString[L"InertiaSimulationError"];

					// 汽油车的OBD数据流
					sRTData.sOBDRTData.strVelocity = mapString[L"OBDRTData_Velocity"];
					sRTData.sOBDRTData.strCoolantTemp = mapString[L"OBDRTData_CoolantTemp"];
					sRTData.sOBDRTData.strEngineRev = mapString[L"OBDRTData_EngineRev"];
					sRTData.sOBDRTData.strSolarTermDoorPosition = mapString[L"OBDRTData_SolarTermDoorPosition"];
					sRTData.sOBDRTData.strThrottleOpening = mapString[L"OBDRTData_ThrottleOpening"];
					sRTData.sOBDRTData.strCalculationLoad = mapString[L"OBDRTData_CalculationLoad"];
					sRTData.sOBDRTData.strForwardSensorV = mapString[L"OBDRTData_ForwardSensorV"];
					sRTData.sOBDRTData.strForwardSensorI = mapString[L"OBDRTData_ForwardSensorI"];
					sRTData.sOBDRTData.strAirCoefficient = mapString[L"OBDRTData_AirCoefficient"];
					sRTData.sOBDRTData.strMAF = mapString[L"OBDRTData_MAF"];
					sRTData.sOBDRTData.strMAP = mapString[L"OBDRTData_MAP"];

					sRTData.strRemark = mapString[L"Remark"];
					sRTData.strReserved1 = mapString[L"Reserved1"];
					sRTData.strReserved2 = mapString[L"Reserved2"];
					sRTData.strReserved3 = mapString[L"Reserved3"];
					vtRTData.push_back(sRTData);
				}
				while (xmlReader.NextSibling());
			}
		}		
		return vtRTData;
	}
	
	// 计算Hash
	std::wstring CalcHash(const enum HashVer emVer)
	{
		CString strXML;
		strXML.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.AppendFormat(L"<VMASResultData>");
		if (emVer >= HV_V1)
		{
			strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", strRunningNumber.c_str());
			strXML.AppendFormat(L"<ReportNumber>%s</ReportNumber>", strReportNumber.c_str());
			strXML.AppendFormat(L"<O2>%.4f</O2>", (float)_wtof(strO2.c_str()));
			strXML.AppendFormat(L"<CO2>%.4f</CO2>", (float)_wtof(strCO2.c_str()));
			strXML.AppendFormat(L"<HC>%.4f</HC>", (float)_wtof(strHC.c_str()));
			strXML.AppendFormat(L"<CO>%.4f</CO>", (float)_wtof(strCO.c_str()));
			strXML.AppendFormat(L"<NOx>%.4f</NOx>", (float)_wtof(strNOx.c_str()));
			strXML.AppendFormat(L"<LimitOfHC>%.4f</LimitOfHC>", (float)_wtof(strLimitOfHC.c_str()));
			strXML.AppendFormat(L"<LimitOfCO>%.4f</LimitOfCO>", (float)_wtof(strLimitOfCO.c_str()));
			strXML.AppendFormat(L"<LimitOfNOx>%.4f</LimitOfNOx>", (float)_wtof(strLimitOfNOx.c_str()));
			strXML.AppendFormat(L"<PassOfHC>%s</PassOfHC>", strPassOfHC.c_str());
			strXML.AppendFormat(L"<PassOfCO>%s</PassOfCO>", strPassOfCO.c_str());
			strXML.AppendFormat(L"<PassOfNOx>%s</PassOfNOx>", strPassOfNOx.c_str());
			strXML.AppendFormat(L"<Power50>%.4f</Power50>", (float)_wtof(strPower50.c_str()));
			strXML.AppendFormat(L"<TravelledDistance>%.4f</TravelledDistance>", (float)_wtof(strTravelledDistance.c_str()));
			strXML.AppendFormat(L"<VolOfGas>%.4f</VolOfGas>", (float)_wtof(strVolOfGas.c_str()));
			strXML.AppendFormat(L"<VolOfExhaust>%.4f</VolOfExhaust>", (float)_wtof(strVolOfExhaust.c_str()));
			strXML.AppendFormat(L"<Pass>%s</Pass>", strPass.c_str());
			strXML.AppendFormat(L"<EnvironmentalTemperature>%.4f</EnvironmentalTemperature>", (float)_wtof(strEnvironmentalTemperature.c_str()));
			strXML.AppendFormat(L"<RelativeHumidity>%.4f</RelativeHumidity>", (float)_wtof(strRelativeHumidity.c_str()));
			strXML.AppendFormat(L"<AtmosphericPressure>%.4f</AtmosphericPressure>", (float)_wtof(strAtmosphericPressure.c_str()));
		}
		if (emVer >= HV_V2)
		{
			// 预留增加字段
		}
		strXML.AppendFormat(L"</VMASResultData>");

		std::string strUtf8XML = TCharToUTF8(strXML);
		MD5 hash(strUtf8XML.c_str());
		
		std::wstring strHash = (LPCTSTR)UTF8ToTChar(hash.md5().c_str());
		return strHash;
	}

	// 更新Hash
	void UpdateHash(void)
	{
		enum HashVer emVer = HV_V1;	// 用版本号控制需要验证的字段
		CString strTemp;
		strTemp.Format(L"%d,%s", emVer, CalcHash(emVer).c_str());	//版本号和hash一起保存
		this->strHash = (LPCTSTR)strTemp;
	}

	// 校验Hash
	bool CheckHash(void)
	{
		CString strTemp = this->strHash.c_str();

		// 分解版本号和hash
		enum HashVer emVer = (HashVer)_wtoi(strTemp.Mid(0, strTemp.Find(L",")));	
		std::wstring strHashSave = (LPCTSTR)strTemp.Mid(strTemp.Find(L",")+1);

		std::wstring strHashCalc = CalcHash(emVer);		
		if (_wcsicmp(strHashSave.c_str(), strHashCalc.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

// VMAS real time data<RealTimeDataOfVMAS>
typedef struct RealTimeDataOfVMAS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchPower[NH_FLOAT_TO_WCHART];
	wchar_t wchForce[NH_FLOAT_TO_WCHART];
	wchar_t wchIHP[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHP[NH_FLOAT_TO_WCHART];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchO2OfFlowmeter[NH_FLOAT_TO_WCHART];
	wchar_t wchO2OfEnvironment[NH_FLOAT_TO_WCHART];
	wchar_t wchHCm[NH_FLOAT_TO_WCHART];
	wchar_t wchCOm[NH_FLOAT_TO_WCHART];
	wchar_t wchNOm[NH_FLOAT_TO_WCHART];
	wchar_t wchO2m[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2m[NH_FLOAT_TO_WCHART];
	wchar_t wchFuleConsumption[NH_FLOAT_TO_WCHART];
	wchar_t wchPressureOfFlowmeter[NH_FLOAT_TO_WCHART];
	wchar_t wchTemperatureOfFlowmeter[NH_FLOAT_TO_WCHART];
	wchar_t wchDR[NH_FLOAT_TO_WCHART];
	wchar_t wchActualFluxOfGas[NH_FLOAT_TO_WCHART];
	wchar_t wchStandardFluxOfGas[NH_FLOAT_TO_WCHART];
	wchar_t wchFluxOfExhaust[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchHumidityCorrectionFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchDilutionCorrectionFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchInertiaSimulationError[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	RealTimeDataOfVMAS(){ZeroMemory(this, sizeof(RealTimeDataOfVMAS));}
}REALTIMEDATAOFVMAS, *PREALTIMEDATAOFVMAS;

// LugDown result data<ResultOfLUGDOWN>
typedef struct ResultOfLUGDOWN
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchK100[NH_FLOAT_TO_WCHART];
	wchar_t wchN100[NH_FLOAT_TO_WCHART];
	wchar_t wchP100[NH_FLOAT_TO_WCHART];
	wchar_t wchNOx100[NH_INT_TO_WCHART];
	wchar_t wchEngineRev100[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature100[NH_FLOAT_TO_WCHART];
	wchar_t wchK90[NH_FLOAT_TO_WCHART];
	wchar_t wchN90[NH_FLOAT_TO_WCHART];
	wchar_t wchP90[NH_FLOAT_TO_WCHART];
	wchar_t wchEngineRev90[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature90[NH_FLOAT_TO_WCHART];
	wchar_t wchK80[NH_FLOAT_TO_WCHART];
	wchar_t wchN80[NH_FLOAT_TO_WCHART];
	wchar_t wchP80[NH_FLOAT_TO_WCHART];
	wchar_t wchNOx80[NH_INT_TO_WCHART];
	wchar_t wchEngineRev80[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature80[NH_FLOAT_TO_WCHART];
	wchar_t wchKLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchKLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchNLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchNLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchNOxLimit[NH_INT_TO_WCHART];
	wchar_t wchPowerCorrectionFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchCorrectedPower[NH_FLOAT_TO_WCHART];
	wchar_t wchMinHP[NH_FLOAT_TO_WCHART];
	wchar_t wchRatedRev[NH_INT_TO_WCHART];
	wchar_t wchPassOfK100[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfNOx100[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfK90[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfK80[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfNOx80[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfP[NH_BOOL_TO_WCHART];
	wchar_t wchPassOfRev[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchMaxHP[NH_FLOAT_TO_WCHART];
	wchar_t wchActualVelMaxHP[NH_FLOAT_TO_WCHART];
	wchar_t wchCalculatedVelMaxHP[NH_FLOAT_TO_WCHART];
	wchar_t wchMaxRPM[NH_INT_TO_WCHART];
	wchar_t wchIdleRev[NH_INT_TO_WCHART];
	wchar_t wchTotalDuration[NH_INT_TO_WCHART];
	wchar_t wchModeDuration[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchSkipRevJudgement[NH_BOOL_TO_WCHART];
	wchar_t wchRevPassRange[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfLUGDOWN(){ZeroMemory(this, sizeof(ResultOfLUGDOWN));}
}RESULTOFLUGDOWN, *PRESULTOFLUGDOWN;

// 此结构体数据以XML格式存入SResultOfLUGDOWN的二进制字段中
struct SRealTimeDataOfLUGDOWN
{
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strTime;
	std::wstring strSamplingTime;
	std::wstring strVelocity;
	std::wstring strPower;
	std::wstring strForce;
	std::wstring strIHP;
	std::wstring strPLHP;
	std::wstring strState;
	std::wstring strK;
	std::wstring strN;
	std::wstring strEngineRev;
	std::wstring strOilTemperature;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strPowerCorrectionFactor;

	std::wstring strCO;
	std::wstring strHC;
	std::wstring strNO;
	std::wstring strNO2;
	std::wstring strNOx;
	std::wstring strCO2;
	std::wstring strO2;
	std::wstring strHumidityCorrectionFactor;

	SOBDRTData sOBDRTData;

	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;	
};

struct SResultOfLUGDOWN
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strK100;
	std::wstring strN100;
	std::wstring strP100;
	std::wstring strNOx100;
	std::wstring strEngineRev100;
	std::wstring strOilTemperature100;
	std::wstring strK90;
	std::wstring strN90;
	std::wstring strP90;
	std::wstring strEngineRev90;
	std::wstring strOilTemperature90;
	std::wstring strK80;
	std::wstring strN80;
	std::wstring strP80;
	std::wstring strNOx80;
	std::wstring strEngineRev80;
	std::wstring strOilTemperature80;
	std::wstring strKLimit;
	std::wstring strKLimit_EDP;
	std::wstring strNLimit;
	std::wstring strNLimit_EDP;
	std::wstring strNOxLimit;
	std::wstring strPowerCorrectionFactor;
	std::wstring strCorrectedPower;
	std::wstring strMinHP;
	std::wstring strRatedRev;
	std::wstring strPassOfK100;
	std::wstring strPassOfNOx100;
	std::wstring strPassOfK90;
	std::wstring strPassOfK80;
	std::wstring strPassOfNOx80;
	std::wstring strPassOfP;
	std::wstring strPassOfRev;
	std::wstring strPass;
	std::wstring strMaxHP;
	std::wstring strActualVelMaxHP;
	std::wstring strCalculatedVelMaxHP;
	std::wstring strMaxRPM;
	std::wstring strIdleRev;
	std::wstring strTotalDuration;
	std::wstring strModeDuration;
	std::wstring strOilTemperature;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strSkipRev;
	std::wstring strSkipOilTemperature;
	std::wstring strSkipRevJudgement;
	std::wstring strRevPassRange;
	// RTDataZip，过程数据，以XML格式封装，zip算法压缩
	std::string strRTDataZip;
	// Hash数据校验
	std::wstring strHash;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
	
	// 压缩过程数据
	void RTDataZip(std::vector<SRealTimeDataOfLUGDOWN> &vtRTData)
	{
		std::wstring strXML;
		strXML.append(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.append(L"<LUGDOWNRTData>");
		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strXML.append(L"<Row>");
			strXML.append(L"<RunningNumber>" + vtRTData[i].strRunningNumber + L"</RunningNumber>");
			strXML.append(L"<ReportNumber>" + vtRTData[i].strReportNumber + L"</ReportNumber>");
			strXML.append(L"<FuelTypeCode>" + vtRTData[i].strFuelTypeCode + L"</FuelTypeCode>");
			strXML.append(L"<FuelTypeName>" + vtRTData[i].strFuelTypeName + L"</FuelTypeName>");
			strXML.append(L"<Time>" + vtRTData[i].strTime + L"</Time>");
			strXML.append(L"<SamplingTime>" + vtRTData[i].strSamplingTime + L"</SamplingTime>");
			strXML.append(L"<Velocity>" + vtRTData[i].strVelocity + L"</Velocity>");
			strXML.append(L"<Power>" + vtRTData[i].strPower + L"</Power>");
			strXML.append(L"<Force>" + vtRTData[i].strForce + L"</Force>");
			strXML.append(L"<IHP>" + vtRTData[i].strIHP + L"</IHP>");
			strXML.append(L"<PLHP>" + vtRTData[i].strPLHP + L"</PLHP>");
			strXML.append(L"<State>" + vtRTData[i].strState + L"</State>");
			strXML.append(L"<K>" + vtRTData[i].strK + L"</K>");
			strXML.append(L"<N>" + vtRTData[i].strN + L"</N>");
			strXML.append(L"<EngineRev>" + vtRTData[i].strEngineRev + L"</EngineRev>");
			strXML.append(L"<OilTemperature>" + vtRTData[i].strOilTemperature + L"</OilTemperature>");
			strXML.append(L"<EnvironmentalTemperature>" + vtRTData[i].strEnvironmentalTemperature + L"</EnvironmentalTemperature>");
			strXML.append(L"<RelativeHumidity>" + vtRTData[i].strRelativeHumidity + L"</RelativeHumidity>");
			strXML.append(L"<AtmosphericPressure>" + vtRTData[i].strAtmosphericPressure + L"</AtmosphericPressure>");
			strXML.append(L"<PowerCorrectionFactor>" + vtRTData[i].strPowerCorrectionFactor + L"</PowerCorrectionFactor>");
			strXML.append(L"<CO>" + vtRTData[i].strCO + L"</CO>");
			strXML.append(L"<HC>" + vtRTData[i].strHC + L"</HC>");
			strXML.append(L"<NO>" + vtRTData[i].strNO + L"</NO>");
			strXML.append(L"<NO2>" + vtRTData[i].strNO2 + L"</NO2>");
			strXML.append(L"<NOx>" + vtRTData[i].strNOx + L"</NOx>");
			strXML.append(L"<CO2>" + vtRTData[i].strCO2 + L"</CO2>");
			strXML.append(L"<O2>" + vtRTData[i].strO2 + L"</O2>");
			strXML.append(L"<HumidityCorrectionFactor>" + vtRTData[i].strHumidityCorrectionFactor + L"</HumidityCorrectionFactor>");
			
			// 柴油车的OBD数据流
			strXML.append(L"<OBDRTData_Velocity>" + vtRTData[i].sOBDRTData.strVelocity + L"</OBDRTData_Velocity>");
			strXML.append(L"<OBDRTData_CoolantTemp>" + vtRTData[i].sOBDRTData.strCoolantTemp + L"</OBDRTData_CoolantTemp>");
			strXML.append(L"<OBDRTData_EngineRev>" + vtRTData[i].sOBDRTData.strEngineRev + L"</OBDRTData_EngineRev>");			
			strXML.append(L"<OBDRTData_SolarTermDoorPosition>" + vtRTData[i].sOBDRTData.strSolarTermDoorPosition + L"</OBDRTData_SolarTermDoorPosition>");
			strXML.append(L"<OBDRTData_ThrottleOpening>" + vtRTData[i].sOBDRTData.strThrottleOpening + L"</OBDRTData_ThrottleOpening>");
			strXML.append(L"<OBDRTData_MAF>" + vtRTData[i].sOBDRTData.strMAF + L"</OBDRTData_MAF>");
			strXML.append(L"<OBDRTData_MAP>" + vtRTData[i].sOBDRTData.strMAP + L"</OBDRTData_MAP>");
			strXML.append(L"<OBDRTData_EngineOutputPower>" + vtRTData[i].sOBDRTData.strEngineOutputPower + L"</OBDRTData_EngineOutputPower>");
			strXML.append(L"<OBDRTData_ChargeAirPressure>" + vtRTData[i].sOBDRTData.strChargeAirPressure + L"</OBDRTData_ChargeAirPressure>");
			strXML.append(L"<OBDRTData_FuelConsumption>" + vtRTData[i].sOBDRTData.strFuelConsumption + L"</OBDRTData_FuelConsumption>");
			strXML.append(L"<OBDRTData_NOXConcentration>" + vtRTData[i].sOBDRTData.strNOXConcentration + L"</OBDRTData_NOXConcentration>");
			strXML.append(L"<OBDRTData_UreaInjectionVolume>" + vtRTData[i].sOBDRTData.strUreaInjectionVolume + L"</OBDRTData_UreaInjectionVolume>");
			strXML.append(L"<OBDRTData_EGT>" + vtRTData[i].sOBDRTData.strEGT + L"</OBDRTData_EGT>");
			strXML.append(L"<OBDRTData_DPFDifferentialPressure>" + vtRTData[i].sOBDRTData.strDPFDifferentialPressure + L"</OBDRTData_DPFDifferentialPressure>");
			strXML.append(L"<OBDRTData_EGRPosition>" + vtRTData[i].sOBDRTData.strEGRPosition + L"</OBDRTData_EGRPosition>");
			strXML.append(L"<OBDRTData_FuelDeliveryPressure>" + vtRTData[i].sOBDRTData.strFuelDeliveryPressure + L"</OBDRTData_FuelDeliveryPressure>");

			strXML.append(L"<Remark>" + vtRTData[i].strRemark + L"</Remark>");
			strXML.append(L"<Reserved1>" + vtRTData[i].strReserved1 + L"</Reserved1>");
			strXML.append(L"<Reserved2>" + vtRTData[i].strReserved2 + L"</Reserved2>");
			strXML.append(L"<Reserved3>" + vtRTData[i].strReserved3 + L"</Reserved3>");
			strXML.append(L"</Row>");
		}
		strXML.append(L"</LUGDOWNRTData>");
						
		std::string strUtf8XML = TCharToUTF8(strXML.c_str());
		CZipLib zip;
		strRTDataZip = zip.ZipData(strUtf8XML.data());
	}

	// 解压过程数据
	std::vector<SRealTimeDataOfLUGDOWN> RTDataUpZip(void)
	{
		std::vector<SRealTimeDataOfLUGDOWN> vtRTData;

		CZipLib zip;		
		std::wstring strXML = (LPCTSTR)UTF8ToTChar(zip.UnZipData(strRTDataZip).c_str());
		
		CXmlReader xmlReader;
		if (xmlReader.Parse(strXML.c_str()))
		{
			if (xmlReader.OpenNode(L"LUGDOWNRTData/Row"))
			{
				do
				{			
					std::map<std::wstring, std::wstring> mapString;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapString[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();

					SRealTimeDataOfLUGDOWN sRTData;
					sRTData.strRunningNumber = mapString[L"RunningNumber"];
					sRTData.strReportNumber = mapString[L"ReportNumber"];
					sRTData.strFuelTypeCode = mapString[L"FuelTypeCode"];
					sRTData.strFuelTypeName = mapString[L"FuelTypeName"];
					sRTData.strTime = mapString[L"Time"];
					sRTData.strSamplingTime = mapString[L"SamplingTime"];
					sRTData.strVelocity = mapString[L"Velocity"];
					sRTData.strPower = mapString[L"Power"];
					sRTData.strForce = mapString[L"Force"];
					sRTData.strIHP = mapString[L"IHP"];
					sRTData.strPLHP = mapString[L"PLHP"];
					sRTData.strState = mapString[L"State"];
					sRTData.strK = mapString[L"K"];
					sRTData.strN = mapString[L"N"];
					sRTData.strEngineRev = mapString[L"EngineRev"];
					sRTData.strOilTemperature = mapString[L"OilTemperature"];
					sRTData.strEnvironmentalTemperature = mapString[L"EnvironmentalTemperature"];
					sRTData.strRelativeHumidity = mapString[L"RelativeHumidity"];
					sRTData.strAtmosphericPressure = mapString[L"AtmosphericPressure"];
					sRTData.strPowerCorrectionFactor = mapString[L"PowerCorrectionFactor"];
					sRTData.strCO = mapString[L"CO"];
					sRTData.strHC = mapString[L"HC"];
					sRTData.strNO = mapString[L"NO"];
					sRTData.strNO2 = mapString[L"NO2"];
					sRTData.strNOx = mapString[L"NOx"];
					sRTData.strCO2 = mapString[L"CO2"];
					sRTData.strO2 = mapString[L"O2"];
					sRTData.strHumidityCorrectionFactor = mapString[L"HumidityCorrectionFactor"];
					
					// 柴油车的OBD数据流
					sRTData.sOBDRTData.strVelocity = mapString[L"OBDRTData_Velocity"];
					sRTData.sOBDRTData.strCoolantTemp = mapString[L"OBDRTData_CoolantTemp"];
					sRTData.sOBDRTData.strEngineRev = mapString[L"OBDRTData_EngineRev"];
					sRTData.sOBDRTData.strSolarTermDoorPosition = mapString[L"OBDRTData_SolarTermDoorPosition"];
					sRTData.sOBDRTData.strThrottleOpening = mapString[L"OBDRTData_ThrottleOpening"];
					sRTData.sOBDRTData.strMAF = mapString[L"OBDRTData_MAF"];
				    sRTData.sOBDRTData.strMAP = mapString[L"OBDRTData_MAP"];
					sRTData.sOBDRTData.strEngineOutputPower = mapString[L"OBDRTData_EngineOutputPower"];
					sRTData.sOBDRTData.strChargeAirPressure = mapString[L"OBDRTData_ChargeAirPressure"];
					sRTData.sOBDRTData.strFuelConsumption = mapString[L"OBDRTData_FuelConsumption"];
					sRTData.sOBDRTData.strNOXConcentration = mapString[L"OBDRTData_NOXConcentration"];
					sRTData.sOBDRTData.strUreaInjectionVolume = mapString[L"OBDRTData_UreaInjectionVolume"];
					sRTData.sOBDRTData.strEGT = mapString[L"OBDRTData_EGT"];
					sRTData.sOBDRTData.strDPFDifferentialPressure = mapString[L"OBDRTData_DPFDifferentialPressure"];
					sRTData.sOBDRTData.strEGRPosition = mapString[L"OBDRTData_EGRPosition"];
					sRTData.sOBDRTData.strFuelDeliveryPressure = mapString[L"OBDRTData_FuelDeliveryPressure"];

					sRTData.strRemark = mapString[L"Remark"];
					sRTData.strReserved1 = mapString[L"Reserved1"];
					sRTData.strReserved2 = mapString[L"Reserved2"];
					sRTData.strReserved3 = mapString[L"Reserved3"];
					vtRTData.push_back(sRTData);
				}
				while (xmlReader.NextSibling());
			}
		}		
		return vtRTData;
	}
	
	// 计算Hash
	std::wstring CalcHash(const enum HashVer emVer)
	{
		CString strXML;
		strXML.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.AppendFormat(L"<LUGDOWNResultData>");
		if (emVer >= HV_V1)
		{
			strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", strRunningNumber.c_str());
			strXML.AppendFormat(L"<ReportNumber>%s</ReportNumber>", strReportNumber.c_str());
			strXML.AppendFormat(L"<K100>%.4f</K100>", (float)_wtof(strK100.c_str()));
			strXML.AppendFormat(L"<N100>%.4f</N100>", (float)_wtof(strN100.c_str()));
			strXML.AppendFormat(L"<P100>%.4f</P100>", (float)_wtof(strP100.c_str()));
			strXML.AppendFormat(L"<NOx100>%d</NOx100>", _wtoi(strNOx100.c_str()));
			strXML.AppendFormat(L"<EngineRev100>%d</EngineRev100>", _wtoi(strEngineRev100.c_str()));
			strXML.AppendFormat(L"<K80>%.4f</K80>", (float)_wtof(strK80.c_str()));
			strXML.AppendFormat(L"<N80>%.4f</N80>", (float)_wtof(strN80.c_str()));
			strXML.AppendFormat(L"<P80>%.4f</P80>", (float)_wtof(strP80.c_str()));
			strXML.AppendFormat(L"<NOx80>%d</NOx80>", _wtoi(strNOx80.c_str()));
			strXML.AppendFormat(L"<KLimit>%.4f</KLimit>", (float)_wtof(strKLimit.c_str()));
			strXML.AppendFormat(L"<NLimit>%.4f</NLimit>", (float)_wtof(strNLimit.c_str()));
			strXML.AppendFormat(L"<NOxLimit>%d</NOxLimit>", _wtoi(strNOxLimit.c_str()));
			strXML.AppendFormat(L"<PowerCorrectionFactor>%.4f</PowerCorrectionFactor>", (float)_wtof(strPowerCorrectionFactor.c_str()));
			strXML.AppendFormat(L"<CorrectedPower>%.4f</CorrectedPower>", (float)_wtof(strCorrectedPower.c_str()));
			strXML.AppendFormat(L"<MinHP>%.4f</MinHP>", (float)_wtof(strMinHP.c_str()));
			strXML.AppendFormat(L"<PassOfK100>%s</PassOfK100>", strPassOfK100.c_str());
			strXML.AppendFormat(L"<PassOfK80>%s</PassOfK80>", strPassOfK80.c_str());
			strXML.AppendFormat(L"<PassOfNOx80>%s</PassOfNOx80>", strPassOfNOx80.c_str());
			strXML.AppendFormat(L"<PassOfP>%s</PassOfP>", strPassOfP.c_str());
			strXML.AppendFormat(L"<Pass>%s</Pass>", strPass.c_str());
			strXML.AppendFormat(L"<EnvironmentalTemperature>%.4f</EnvironmentalTemperature>", (float)_wtof(strEnvironmentalTemperature.c_str()));
			strXML.AppendFormat(L"<RelativeHumidity>%.4f</RelativeHumidity>", (float)_wtof(strRelativeHumidity.c_str()));
			strXML.AppendFormat(L"<AtmosphericPressure>%.4f</AtmosphericPressure>", (float)_wtof(strAtmosphericPressure.c_str()));
		}
		if (emVer >= HV_V2)
		{
			// 预留增加字段
		}
		strXML.AppendFormat(L"</LUGDOWNResultData>");

		std::string strUtf8XML = TCharToUTF8(strXML);
		MD5 hash(strUtf8XML.c_str());
		
		std::wstring strHash = (LPCTSTR)UTF8ToTChar(hash.md5().c_str());
		return strHash;
	}

	// 更新Hash
	void UpdateHash(void)
	{
		enum HashVer emVer = HV_V1;	// 用版本号控制需要验证的字段
		CString strTemp;
		strTemp.Format(L"%d,%s", emVer, CalcHash(emVer).c_str());	//版本号和hash一起保存
		this->strHash = (LPCTSTR)strTemp;
	}

	// 校验Hash
	bool CheckHash(void)
	{
		CString strTemp = this->strHash.c_str();

		// 分解版本号和hash
		enum HashVer emVer = (HashVer)_wtoi(strTemp.Mid(0, strTemp.Find(L",")));	
		std::wstring strHashSave = (LPCTSTR)strTemp.Mid(strTemp.Find(L",")+1);

		std::wstring strHashCalc = CalcHash(emVer);		
		if (_wcsicmp(strHashSave.c_str(), strHashCalc.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

// LUGDOWN real time data<RealTimeDataOfLUGDOWN>
typedef struct RealTimeDataOfLUGDOWN
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchPower[NH_FLOAT_TO_WCHART];
	wchar_t wchForce[NH_FLOAT_TO_WCHART];
	wchar_t wchIHP[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHP[NH_FLOAT_TO_WCHART];
	wchar_t wchState[50];
	wchar_t wchK[NH_FLOAT_TO_WCHART];
	wchar_t wchN[NH_FLOAT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerCorrectionFactor[NH_FLOAT_TO_WCHART];

	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchNO2[NH_INT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchHumidityCorrectionFactor[NH_FLOAT_TO_WCHART];

	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];
	
	RealTimeDataOfLUGDOWN(){ZeroMemory(this, sizeof(RealTimeDataOfLUGDOWN));}
}REALTIMEDATAOFLUGDOWN, *PREALTIMEDATAOFLUGDOWNM;

// 此结构体数据以XML格式存入SResultOfFSUNHT的二进制字段中
struct SRealTimeDataOfFSUNHT
{
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strOrder;
	std::wstring strTime;
	std::wstring strSamplingTime;
	std::wstring strK;
	std::wstring strN;
	std::wstring strEngineRev;
	std::wstring strOilTemperature;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	SOBDRTData sOBDRTData;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};

struct SResultOfFSUNHT
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strReportNumber;
	std::wstring strFuelTypeCode;
	std::wstring strFuelTypeName;
	std::wstring strEngineRev;
	std::wstring strValue1;
	std::wstring strValue2;
	std::wstring strValue3;
	std::wstring strValue4;
	std::wstring strAverage;
	std::wstring strLimit;
	std::wstring strLimit_EDP;
	std::wstring strPass;
	std::wstring strEnvironmentalTemperature;
	std::wstring strRelativeHumidity;
	std::wstring strAtmosphericPressure;
	std::wstring strOilTemperature;
	std::wstring strSkipRev;
	std::wstring strSkipOilTemperature;
	// RTDataZip，过程数据，以XML格式封装，zip算法压缩
	std::string strRTDataZip;
	// Hash数据校验
	std::wstring strHash;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
	
	
	// 压缩过程数据
	void RTDataZip(std::vector<SRealTimeDataOfFSUNHT> &vtRTData)
	{
		std::wstring strXML;
		strXML.append(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.append(L"<FSUNHTRTData>");
		for (UINT i=0; i<vtRTData.size(); i++)
		{
			strXML.append(L"<Row>");
			strXML.append(L"<RunningNumber>" + vtRTData[i].strRunningNumber + L"</RunningNumber>");
			strXML.append(L"<ReportNumber>" + vtRTData[i].strReportNumber + L"</ReportNumber>");
			strXML.append(L"<Order>" + vtRTData[i].strOrder + L"</Order>");
			strXML.append(L"<Time>" + vtRTData[i].strTime + L"</Time>");
			strXML.append(L"<SamplingTime>" + vtRTData[i].strSamplingTime + L"</SamplingTime>");
			strXML.append(L"<K>" + vtRTData[i].strK + L"</K>");
			strXML.append(L"<N>" + vtRTData[i].strN + L"</N>");
			strXML.append(L"<EngineRev>" + vtRTData[i].strEngineRev + L"</EngineRev>");
			strXML.append(L"<OilTemperature>" + vtRTData[i].strOilTemperature + L"</OilTemperature>");
			strXML.append(L"<EnvironmentalTemperature>" + vtRTData[i].strEnvironmentalTemperature + L"</EnvironmentalTemperature>");
			strXML.append(L"<RelativeHumidity>" + vtRTData[i].strRelativeHumidity + L"</RelativeHumidity>");
			strXML.append(L"<AtmosphericPressure>" + vtRTData[i].strAtmosphericPressure + L"</AtmosphericPressure>");
			
			// 柴油车的OBD数据流
			strXML.append(L"<OBDRTData_Velocity>" + vtRTData[i].sOBDRTData.strVelocity + L"</OBDRTData_Velocity>");
			strXML.append(L"<OBDRTData_CoolantTemp>" + vtRTData[i].sOBDRTData.strCoolantTemp + L"</OBDRTData_CoolantTemp>");
			strXML.append(L"<OBDRTData_EngineRev>" + vtRTData[i].sOBDRTData.strEngineRev + L"</OBDRTData_EngineRev>");
			strXML.append(L"<OBDRTData_SolarTermDoorPosition>" + vtRTData[i].sOBDRTData.strSolarTermDoorPosition + L"</OBDRTData_SolarTermDoorPosition>");	
			strXML.append(L"<OBDRTData_ThrottleOpening>" + vtRTData[i].sOBDRTData.strThrottleOpening + L"</OBDRTData_ThrottleOpening>");		
			strXML.append(L"<OBDRTData_MAF>" + vtRTData[i].sOBDRTData.strMAF + L"</OBDRTData_MAF>");
			strXML.append(L"<OBDRTData_MAP>" + vtRTData[i].sOBDRTData.strMAP + L"</OBDRTData_MAP>");
			strXML.append(L"<OBDRTData_EngineOutputPower>" + vtRTData[i].sOBDRTData.strEngineOutputPower + L"</OBDRTData_EngineOutputPower>");
			strXML.append(L"<OBDRTData_ChargeAirPressure>" + vtRTData[i].sOBDRTData.strChargeAirPressure + L"</OBDRTData_ChargeAirPressure>");
			strXML.append(L"<OBDRTData_FuelConsumption>" + vtRTData[i].sOBDRTData.strFuelConsumption + L"</OBDRTData_FuelConsumption>");
			strXML.append(L"<OBDRTData_NOXConcentration>" + vtRTData[i].sOBDRTData.strNOXConcentration + L"</OBDRTData_NOXConcentration>");
			strXML.append(L"<OBDRTData_UreaInjectionVolume>" + vtRTData[i].sOBDRTData.strUreaInjectionVolume + L"</OBDRTData_UreaInjectionVolume>");
			strXML.append(L"<OBDRTData_EGT>" + vtRTData[i].sOBDRTData.strEGT + L"</OBDRTData_EGT>");
			strXML.append(L"<OBDRTData_DPFDifferentialPressure>" + vtRTData[i].sOBDRTData.strDPFDifferentialPressure + L"</OBDRTData_DPFDifferentialPressure>");
			strXML.append(L"<OBDRTData_EGRPosition>" + vtRTData[i].sOBDRTData.strEGRPosition + L"</OBDRTData_EGRPosition>");
			strXML.append(L"<OBDRTData_FuelDeliveryPressure>" + vtRTData[i].sOBDRTData.strFuelDeliveryPressure + L"</OBDRTData_FuelDeliveryPressure>");

			strXML.append(L"<Remark>" + vtRTData[i].strRemark + L"</Remark>");
			strXML.append(L"<Reserved1>" + vtRTData[i].strReserved1 + L"</Reserved1>");
			strXML.append(L"<Reserved2>" + vtRTData[i].strReserved2 + L"</Reserved2>");
			strXML.append(L"<Reserved3>" + vtRTData[i].strReserved3 + L"</Reserved3>");
			strXML.append(L"</Row>");
		}
		strXML.append(L"</FSUNHTRTData>");
						
		std::string strUtf8XML = TCharToUTF8(strXML.c_str());
		CZipLib zip;
		strRTDataZip = zip.ZipData(strUtf8XML.data());
	}

	// 解压过程数据
	std::vector<SRealTimeDataOfFSUNHT> RTDataUpZip(void)
	{
		std::vector<SRealTimeDataOfFSUNHT> vtRTData;

		CZipLib zip;		
		std::wstring strXML = (LPCTSTR)UTF8ToTChar(zip.UnZipData(strRTDataZip).c_str());
		
		CXmlReader xmlReader;
		if (xmlReader.Parse(strXML.c_str()))
		{
			if (xmlReader.OpenNode(L"FSUNHTRTData/Row"))
			{
				do
				{			
					std::map<std::wstring, std::wstring> mapString;	
					xmlReader.EnterNode();
					do
					{
						std::wstring strName, strContent;
						xmlReader.GetNodeName(strName);
						xmlReader.GetNodeContent(strContent);
						mapString[strName] = strContent;
					}
					while (xmlReader.NextSibling());
					xmlReader.ExitNode();

					SRealTimeDataOfFSUNHT sRTData;
					sRTData.strRunningNumber = mapString[L"RunningNumber"];
					sRTData.strReportNumber = mapString[L"ReportNumber"];
					sRTData.strTime = mapString[L"Time"];
					sRTData.strOrder = mapString[L"Order"];
					sRTData.strSamplingTime = mapString[L"SamplingTime"];
					sRTData.strK = mapString[L"K"];
					sRTData.strN = mapString[L"N"];
					sRTData.strEngineRev = mapString[L"EngineRev"];
					sRTData.strOilTemperature = mapString[L"OilTemperature"];
					sRTData.strEnvironmentalTemperature = mapString[L"EnvironmentalTemperature"];
					sRTData.strRelativeHumidity = mapString[L"RelativeHumidity"];
					sRTData.strAtmosphericPressure = mapString[L"AtmosphericPressure"];
					
					// 柴油车的OBD数据流
					sRTData.sOBDRTData.strVelocity = mapString[L"OBDRTData_Velocity"];
					sRTData.sOBDRTData.strCoolantTemp = mapString[L"OBDRTData_CoolantTemp"];
					sRTData.sOBDRTData.strEngineRev = mapString[L"OBDRTData_EngineRev"];
					sRTData.sOBDRTData.strSolarTermDoorPosition = mapString[L"OBDRTData_SolarTermDoorPosition"];
					sRTData.sOBDRTData.strThrottleOpening = mapString[L"OBDRTData_ThrottleOpening"];
					sRTData.sOBDRTData.strMAF = mapString[L"OBDRTData_MAF"];
				    sRTData.sOBDRTData.strMAP = mapString[L"OBDRTData_MAP"];
					sRTData.sOBDRTData.strEngineOutputPower = mapString[L"OBDRTData_EngineOutputPower"];
					sRTData.sOBDRTData.strChargeAirPressure = mapString[L"OBDRTData_ChargeAirPressure"];
					sRTData.sOBDRTData.strFuelConsumption = mapString[L"OBDRTData_FuelConsumption"];
					sRTData.sOBDRTData.strNOXConcentration = mapString[L"OBDRTData_NOXConcentration"];
					sRTData.sOBDRTData.strUreaInjectionVolume = mapString[L"OBDRTData_UreaInjectionVolume"];
					sRTData.sOBDRTData.strEGT = mapString[L"OBDRTData_EGT"];
					sRTData.sOBDRTData.strDPFDifferentialPressure = mapString[L"OBDRTData_DPFDifferentialPressure"];
					sRTData.sOBDRTData.strEGRPosition = mapString[L"OBDRTData_EGRPosition"];
					sRTData.sOBDRTData.strFuelDeliveryPressure = mapString[L"OBDRTData_FuelDeliveryPressure"];

					sRTData.strRemark = mapString[L"Remark"];
					sRTData.strReserved1 = mapString[L"Reserved1"];
					sRTData.strReserved2 = mapString[L"Reserved2"];
					sRTData.strReserved3 = mapString[L"Reserved3"];
					vtRTData.push_back(sRTData);
				}
				while (xmlReader.NextSibling());
			}
		}		
		return vtRTData;
	}
		
	// 计算Hash
	std::wstring CalcHash(const enum HashVer emVer)
	{
		CString strXML;
		strXML.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		strXML.AppendFormat(L"<FSUNHTResultData>");
		if (emVer >= HV_V1)
		{
			strXML.AppendFormat(L"<RunningNumber>%s</RunningNumber>", strRunningNumber.c_str());
			strXML.AppendFormat(L"<ReportNumber>%s</ReportNumber>", strReportNumber.c_str());
			strXML.AppendFormat(L"<Limit>%.4f</Limit>", (float)_wtof(strLimit.c_str()));
			strXML.AppendFormat(L"<EngineRev>%d</EngineRev>", _wtoi(strEngineRev.c_str()));
			strXML.AppendFormat(L"<Value1>%.4f</Value1>", (float)_wtof(strValue1.c_str()));
			strXML.AppendFormat(L"<Value2>%.4f</Value2>", (float)_wtof(strValue2.c_str()));
			strXML.AppendFormat(L"<Value3>%.4f</Value3>", (float)_wtof(strValue3.c_str()));
			strXML.AppendFormat(L"<Value4>%.4f</Value4>", (float)_wtof(strValue4.c_str()));
			strXML.AppendFormat(L"<Average>%.4f</Average>", (float)_wtof(strAverage.c_str()));
			strXML.AppendFormat(L"<Pass>%s</Pass>", strPass.c_str());
			strXML.AppendFormat(L"<EnvironmentalTemperature>%.4f</EnvironmentalTemperature>", (float)_wtof(strEnvironmentalTemperature.c_str()));
			strXML.AppendFormat(L"<RelativeHumidity>%.4f</RelativeHumidity>", (float)_wtof(strRelativeHumidity.c_str()));
			strXML.AppendFormat(L"<AtmosphericPressure>%.4f</AtmosphericPressure>", (float)_wtof(strAtmosphericPressure.c_str()));
		}
		if (emVer >= HV_V2)
		{
			// 预留增加字段
		}
		strXML.AppendFormat(L"</FSUNHTResultData>");

		std::string strUtf8XML = TCharToUTF8(strXML);
		MD5 hash(strUtf8XML.c_str());
		
		std::wstring strHash = (LPCTSTR)UTF8ToTChar(hash.md5().c_str());
		return strHash;
	}

	// 更新Hash
	void UpdateHash(void)
	{
		enum HashVer emVer = HV_V1;	// 用版本号控制需要验证的字段
		CString strTemp;
		strTemp.Format(L"%d,%s", emVer, CalcHash(emVer).c_str());	//版本号和hash一起保存
		this->strHash = (LPCTSTR)strTemp;
	}

	// 校验Hash
	bool CheckHash(void)
	{
		CString strTemp = this->strHash.c_str();

		// 分解版本号和hash
		enum HashVer emVer = (HashVer)_wtoi(strTemp.Mid(0, strTemp.Find(L",")));	
		std::wstring strHashSave = (LPCTSTR)strTemp.Mid(strTemp.Find(L",")+1);

		std::wstring strHashCalc = CalcHash(emVer);		
		if (_wcsicmp(strHashSave.c_str(), strHashCalc.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct SAppPhotoInfo
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strCode;
	std::wstring strName;
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};

struct STestPhotoData
{
	std::wstring strAutoID;
	std::wstring strRunningNumber;
	std::wstring strCode;
	std::wstring strName;
	std::wstring strFileTime;
	std::wstring strFilePath;
	std::string strFileData;	// 二进制字段
	std::wstring strRemark;
	std::wstring strReserved1;
	std::wstring strReserved2;
	std::wstring strReserved3;
};

// FSUNHT result data<ResultOfFSUNHT>
typedef struct ResultOfFSUNHT
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchAverage[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];
	wchar_t wchEngineRev2[NH_INT_TO_WCHART];
	wchar_t wchEngineRev3[NH_INT_TO_WCHART];
	wchar_t wchEngineRev4[NH_INT_TO_WCHART];

	ResultOfFSUNHT(){ZeroMemory(this, sizeof(ResultOfFSUNHT));}
}RESULTOFFSUNHT, *PRESULTOFFSUNHT;

typedef struct RealTimeDataOfFSUNHT
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchOrder[NH_INT_TO_WCHART];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchSamplingTime[50];
	wchar_t wchK[NH_FLOAT_TO_WCHART];
	wchar_t wchN[NH_FLOAT_TO_WCHART];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];
	
	RealTimeDataOfFSUNHT(){ZeroMemory(this, sizeof(RealTimeDataOfFSUNHT));}
}REALTIMEDATAOFFSUNHT, *PREALTIMEDATAOFFSUNHT;

// FSUYD result data<ResultOfFSUYD>
typedef struct ResultOfFSUYD
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchAverage[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfFSUYD(){ZeroMemory(this, sizeof(ResultOfFSUYD));}
}RESULTOFFSUYD, *PRESULTOFFSUYD;

// AVFSU result data<ResultOfAVFSU>
typedef struct ResultOfAVFSU
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchReportNumber[50];
	wchar_t wchFuelTypeCode[NH_INT_TO_WCHART];
	wchar_t wchFuelTypeName[50];
	wchar_t wchEngineRev[NH_INT_TO_WCHART];
	wchar_t wchValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchAverage[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchLimit_EDP[NH_INT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipRev[NH_BOOL_TO_WCHART];
	wchar_t wchSkipOilTemperature[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	ResultOfAVFSU(){ZeroMemory(this, sizeof(ResultOfAVFSU));}
}RESULTOFAVFSU, *PRESULTOFAVFSU;

// 设备校准检查信息<EquCalChkInfo>
typedef struct EquCalChkInfo
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchLineNumber[50];
	wchar_t wchOperator[50];
	wchar_t wchEquipmentNumber[50];
	wchar_t wchEquipment[50];
	wchar_t wchItem[50];
	wchar_t wchDate[NH_TIME_TO_WCHART];
	wchar_t wchData[1024];		//对应数据表字段，改100为1024
	wchar_t wchError[1024];		//对应数据表字段，改100为1024
	wchar_t wchJudgement[50];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	EquCalChkInfo(){ZeroMemory(this, sizeof(EquCalChkInfo));}
}EQUCALCHKINFO, *PEQUCALCHKINFO;

// 标定日志<DemarcationLog>
typedef struct DemarcationLog
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchLineNumber[50];
	wchar_t wchOperator[50];
	wchar_t wchEquipmentNumber[50];
	wchar_t wchEquipmentModel[50];
	wchar_t wchEquipmentName[50];
	wchar_t wchDemarcationItem[NH_INT_TO_WCHART];
	wchar_t wchDemarcationDate[NH_TIME_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationLog(){ZeroMemory(this, sizeof(DemarcationLog));}
}DEMARCATIONLOG, *PDEMARCATIONLOG;

// 系统日志<SystemLog>
typedef struct SystemLog
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchClient[50];
	wchar_t wchDate[NH_TIME_TO_WCHART];
	wchar_t wchLoginUser[50];
	wchar_t wchDescription[50];
	wchar_t wchReason[50];
	wchar_t wchSolution[50];
	wchar_t wchResult[50];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	SystemLog(){ZeroMemory(this, sizeof(SystemLog));}
}SYSTEMLOG, *PSYSTEMLOG;

// 分析仪气体校准结果<DemarcationResultOfAnaGasCal>
typedef struct DemarcationResultOfAnaGasCal
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchPEFMeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipHCCalibration[NH_BOOL_TO_WCHART];
	wchar_t wchC3H8StandardValue[NH_INT_TO_WCHART];
	wchar_t wchHCMeasuredValue[NH_INT_TO_WCHART];
	wchar_t wchSkipCOCalibration[NH_BOOL_TO_WCHART];
	wchar_t wchCOStandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchCOMeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipCO2Calibration[NH_BOOL_TO_WCHART];
	wchar_t wchCO2StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2MeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t wchSkipNOCalibration[NH_BOOL_TO_WCHART];
	wchar_t wchNOStandardValue[NH_INT_TO_WCHART];
	wchar_t wchNOMeasuredValue[NH_INT_TO_WCHART];
	wchar_t wchSkipNO2Calibration[NH_BOOL_TO_WCHART];
	wchar_t wchNO2StandardValue[NH_INT_TO_WCHART];
	wchar_t wchNO2MeasuredValue[NH_INT_TO_WCHART];
	wchar_t wchSkipO2Calibration[NH_BOOL_TO_WCHART];
	wchar_t wchO2StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchO2MeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfAnaGasCal(){ZeroMemory(this, sizeof(DemarcationResultOfAnaGasCal));}
}DEMARCATIONRESULTOFANAGASCAL, *PDEMARCATIONRESULTOFANAGASCAL;

// 分析仪气体检查结果<DemarcationResultOfAnaGasChk>
typedef struct DemarcationResultOfAnaGasChk
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t	wchDemarcationNumber[50];
	wchar_t	wchPEFMeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchC3H8StandardValue[NH_INT_TO_WCHART];
	wchar_t	wchSkipHCJudgement[NH_BOOL_TO_WCHART];
	wchar_t	wchHCMeasuredValue[NH_INT_TO_WCHART];
	wchar_t	wchHCStandardValue[NH_INT_TO_WCHART];
	wchar_t	wchHCAE[NH_INT_TO_WCHART];
	wchar_t	wchHCAELimit[NH_INT_TO_WCHART];
	wchar_t	wchHCAEPass[NH_BOOL_TO_WCHART];
	wchar_t	wchHCRE[NH_FLOAT_TO_WCHART];
	wchar_t	wchHCRELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchHCREPass[NH_BOOL_TO_WCHART];
	wchar_t	wchHCPass[NH_BOOL_TO_WCHART];
	wchar_t	wchSkipCOJudgement[NH_BOOL_TO_WCHART];
	wchar_t	wchCOMeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchCOStandardValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchCOAE[NH_FLOAT_TO_WCHART];
	wchar_t	wchCOAELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchCOAEPass[NH_BOOL_TO_WCHART];
	wchar_t	wchCORE[NH_FLOAT_TO_WCHART];
	wchar_t	wchCORELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchCOREPass[NH_BOOL_TO_WCHART];
	wchar_t	wchCOPass[NH_BOOL_TO_WCHART];
	wchar_t	wchSkipCO2Judgement[NH_BOOL_TO_WCHART];
	wchar_t	wchCO2MeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchCO2StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchCO2AE[NH_FLOAT_TO_WCHART];
	wchar_t	wchCO2AELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchCO2AEPass[NH_BOOL_TO_WCHART];
	wchar_t	wchCO2RE[NH_FLOAT_TO_WCHART];
	wchar_t	wchCO2RELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchCO2REPass[NH_BOOL_TO_WCHART];
	wchar_t	wchCO2Pass[NH_BOOL_TO_WCHART];
	wchar_t	wchSkipNOJudgement[NH_BOOL_TO_WCHART];
	wchar_t	wchNOMeasuredValue[NH_INT_TO_WCHART];
	wchar_t	wchNOStandardValue[NH_INT_TO_WCHART];
	wchar_t	wchNOAE[NH_INT_TO_WCHART];
	wchar_t	wchNOAELimit[NH_INT_TO_WCHART];
	wchar_t	wchNOAEPass[NH_BOOL_TO_WCHART];
	wchar_t	wchNORE[NH_FLOAT_TO_WCHART];
	wchar_t	wchNORELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchNOREPass[NH_BOOL_TO_WCHART];
	wchar_t	wchNOPass[NH_BOOL_TO_WCHART];
	wchar_t	wchSkipNO2Judgement[NH_BOOL_TO_WCHART];
	wchar_t	wchNO2MeasuredValue[NH_INT_TO_WCHART];
	wchar_t	wchNO2StandardValue[NH_INT_TO_WCHART];
	wchar_t	wchNO2AE[NH_INT_TO_WCHART];
	wchar_t	wchNO2AELimit[NH_INT_TO_WCHART];
	wchar_t	wchNO2AEPass[NH_BOOL_TO_WCHART];
	wchar_t	wchNO2RE[NH_FLOAT_TO_WCHART];
	wchar_t	wchNO2RELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchNO2REPass[NH_BOOL_TO_WCHART];
	wchar_t	wchNO2Pass[NH_BOOL_TO_WCHART];
	wchar_t	wchSkipO2Judgement[NH_BOOL_TO_WCHART];
	wchar_t	wchO2MeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchO2StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t	wchO2AE[NH_FLOAT_TO_WCHART];
	wchar_t	wchO2AELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchO2AEPass[NH_BOOL_TO_WCHART];
	wchar_t	wchO2RE[NH_FLOAT_TO_WCHART];
	wchar_t	wchO2RELimit[NH_FLOAT_TO_WCHART];
	wchar_t	wchO2REPass[NH_BOOL_TO_WCHART];
	wchar_t	wchO2Pass[NH_BOOL_TO_WCHART];
	wchar_t	wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfAnaGasChk(){ZeroMemory(this, sizeof(DemarcationResultOfAnaGasChk));}
}DEMARCATIONRESULTOFANAGASCHK, *PDEMARCATIONRESULTOFANAGASCHK;

// 分析仪气体检查实时数据<DemarcationRealTimeDataOfAnaGasChk>
typedef struct DemarcationRealTimeDataOfAnaGasChk
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchHC[NH_INT_TO_WCHART];
	wchar_t wchCO[NH_FLOAT_TO_WCHART];
	wchar_t wchCO2[NH_FLOAT_TO_WCHART];
	wchar_t wchNO[NH_INT_TO_WCHART];
	wchar_t wchNO2[NH_INT_TO_WCHART];
	wchar_t wchO2[NH_FLOAT_TO_WCHART];
	wchar_t wchPEF[NH_FLOAT_TO_WCHART];
	wchar_t wchLambda[NH_FLOAT_TO_WCHART];
	wchar_t wchFlux[NH_INT_TO_WCHART];
	wchar_t wchRev[NH_INT_TO_WCHART];
	wchar_t wchOilTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationRealTimeDataOfAnaGasChk(){ZeroMemory(this, sizeof(DemarcationRealTimeDataOfAnaGasChk));}
}DEMARCATIONREALTIMEDATAOFANAGASCHK, *PDEMARCATIONREALTIMEDATAOFANAGASCHK;

// 不透光度计校准检查结果<DemarcationResultOfOpaCalChk>
typedef struct DemarcationResultOfOpaCalChk
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchMeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchStandardValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchAE1[NH_FLOAT_TO_WCHART];
	wchar_t wchAELimit1[NH_FLOAT_TO_WCHART];
	wchar_t wchAEPass1[NH_BOOL_TO_WCHART];
	wchar_t wchRE1[NH_FLOAT_TO_WCHART];
	wchar_t wchRELimit1[NH_FLOAT_TO_WCHART];
	wchar_t wchREPass1[NH_BOOL_TO_WCHART];
	wchar_t wchLinearity1[NH_FLOAT_TO_WCHART];
	wchar_t wchLinearityLimit1[NH_FLOAT_TO_WCHART];
	wchar_t wchLinearityPass1[NH_BOOL_TO_WCHART];
	wchar_t wchPass1[NH_BOOL_TO_WCHART];
	wchar_t wchMeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchStandardValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchAE2[NH_FLOAT_TO_WCHART];
	wchar_t wchAELimit2[NH_FLOAT_TO_WCHART];
	wchar_t wchAEPass2[NH_BOOL_TO_WCHART];
	wchar_t wchRE2[NH_FLOAT_TO_WCHART];
	wchar_t wchRELimit2[NH_FLOAT_TO_WCHART];
	wchar_t wchREPass2[NH_BOOL_TO_WCHART];
	wchar_t wchLinearity2[NH_FLOAT_TO_WCHART];
	wchar_t wchLinearityLimit2[NH_FLOAT_TO_WCHART];
	wchar_t wchLinearityPass2[NH_BOOL_TO_WCHART];
	wchar_t wchPass2[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfOpaCalChk(){ZeroMemory(this, sizeof(DemarcationResultOfOpaCalChk));}
}DEMARCATIONRESULTOFOPACALCHK, *PDEMARCATIONRESULTOFOPACALCHK;

// 烟度计检查结果<DemarcationResultOfSmoChk>
typedef struct DemarcationResultOfSmoChk
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchRepeatStandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatMeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatMeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatMeasuredValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatMeasuredValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatMeasuredValue5[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatErrorValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatErrorValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatErrorValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatErrorValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatErrorValue5[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatAverageValue[NH_FLOAT_TO_WCHART];
	wchar_t wchRepeatPass[NH_BOOL_TO_WCHART];
	wchar_t wchBasicError1StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError1MeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError1MeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError1MeasuredValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError1AverageValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError2StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError2MeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError2MeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError2MeasuredValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError2AverageValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicErrorFStandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicErrorFMeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicErrorFMeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicErrorFMeasuredValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicErrorFAverageValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError5StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError5MeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError5MeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError5MeasuredValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError5AverageValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError6StandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError6MeasuredValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError6MeasuredValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError6MeasuredValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicError6AverageValue[NH_FLOAT_TO_WCHART];
	wchar_t wchBasicErrorPass[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfSmoChk(){ZeroMemory(this, sizeof(DemarcationResultOfSmoChk));}
}DEMARCATIONRESULTOFSMOCHK, *PDEMARCATIONRESULTOFSMOCHK;

// 测功机加载滑行结果<DemarcationResultOfDynConstLoad>
typedef struct DemarcationResultOfDynConstLoad
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchDIW[NH_INT_TO_WCHART];
	wchar_t wchLowerVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchJudgeByTime[NH_BOOL_TO_WCHART];
	wchar_t wchPowerMeasuredValue[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerStandardValue[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerAE[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerAELimit[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerAEPass[NH_BOOL_TO_WCHART];
	wchar_t wchPowerRE[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerRELimit[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerREPass[NH_BOOL_TO_WCHART];
	wchar_t wchPowerPass[NH_BOOL_TO_WCHART];
	wchar_t wchACDT[NH_INT_TO_WCHART];
	wchar_t wchCCDT[NH_INT_TO_WCHART];
	wchar_t wchCDTAE[NH_INT_TO_WCHART];
	wchar_t wchCDTAELimit[NH_INT_TO_WCHART];
	wchar_t wchCDTAEPass[NH_BOOL_TO_WCHART];
	wchar_t wchCDTRE[NH_FLOAT_TO_WCHART];
	wchar_t wchCDTRELimit[NH_FLOAT_TO_WCHART];
	wchar_t wchCDTREPass[NH_BOOL_TO_WCHART];
	wchar_t wchCDTPass[NH_BOOL_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfDynConstLoad(){ZeroMemory(this, sizeof(DemarcationResultOfDynConstLoad));}
}DEMARCATIONRESULTOFDYNCONSTLOAD, *PDEMARCATIONRESULTOFDYNCONSTLOAD;

// 测功机加载滑行实时数据<DemarcationRealTimeDataOfDynConstLoad>
typedef struct DemarcationRealTimeDataOfDynConstLoad
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchTime[NH_INT_TO_WCHART];
	wchar_t wchVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchPower[NH_FLOAT_TO_WCHART];
	wchar_t wchForce[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationRealTimeDataOfDynConstLoad(){ZeroMemory(this, sizeof(DemarcationRealTimeDataOfDynConstLoad));}
}DEMARCATIONREALTIMEDATAOFDYNCONSTLOAD, *PDEMARCATIONREALTIMEDATAOFDYNCONSTLOAD;

// 测功机力校准结果<DemarcationResultOfDynForceCal>
typedef struct DemarcationResultOfDynForceCal
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];

	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfDynForceCal(){ZeroMemory(this, sizeof(DemarcationResultOfDynForceCal));}
}DEMARCATIONRESULTOFDYNFORCECAL, *PDEMARCATIONRESULTOFDYNFORCECAL;

// 测功机力检查结果<DemarcationResultOfDynForceChk>
typedef struct DemarcationResultOfDynForceChk
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t	wchWeightCount[NH_INT_TO_WCHART];
	wchar_t wchStdValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue1[NH_FLOAT_TO_WCHART];
	wchar_t wchAE1[NH_FLOAT_TO_WCHART];
	wchar_t wchRE1[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue2[NH_FLOAT_TO_WCHART];
	wchar_t wchAE2[NH_FLOAT_TO_WCHART];
	wchar_t wchRE2[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue3[NH_FLOAT_TO_WCHART];
	wchar_t wchAE3[NH_FLOAT_TO_WCHART];
	wchar_t wchRE3[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue4[NH_FLOAT_TO_WCHART];
	wchar_t wchAE4[NH_FLOAT_TO_WCHART];
	wchar_t wchRE4[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue5[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue5[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue5[NH_FLOAT_TO_WCHART];
	wchar_t wchAE5[NH_FLOAT_TO_WCHART];
	wchar_t wchRE5[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue6[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue6[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue6[NH_FLOAT_TO_WCHART];
	wchar_t wchAE6[NH_FLOAT_TO_WCHART];
	wchar_t wchRE6[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue7[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue7[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue7[NH_FLOAT_TO_WCHART];
	wchar_t wchAE7[NH_FLOAT_TO_WCHART];
	wchar_t wchRE7[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue8[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue8[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue8[NH_FLOAT_TO_WCHART];
	wchar_t wchAE8[NH_FLOAT_TO_WCHART];
	wchar_t wchRE8[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue9[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue9[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue9[NH_FLOAT_TO_WCHART];
	wchar_t wchAE9[NH_FLOAT_TO_WCHART];
	wchar_t wchRE9[NH_FLOAT_TO_WCHART];
	wchar_t wchStdValue10[NH_FLOAT_TO_WCHART];
	wchar_t wchForwardAvgValue10[NH_FLOAT_TO_WCHART];
	wchar_t wchBackwardAvgValue10[NH_FLOAT_TO_WCHART];
	wchar_t wchAE10[NH_FLOAT_TO_WCHART];
	wchar_t wchRE10[NH_FLOAT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfDynForceChk(){ZeroMemory(this, sizeof(DemarcationResultOfDynForceChk));}
}DEMARCATIONRESULTOFDYNFORCECHK, *PDEMARCATIONRESULTOFDYNFORCECHK;

// 测功机寄生功率滑行结果<DemarcationResultOfDynPLHP>
typedef struct DemarcationResultOfDynPLHP
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];
	wchar_t wchDIW[NH_INT_TO_WCHART];
	wchar_t wchPLHPMode[NH_INT_TO_WCHART];
	wchar_t wchPLHPType[NH_INT_TO_WCHART];
	wchar_t wchApWithRemanence[NH_FLOAT_TO_WCHART];
	wchar_t wchBpWithRemanence[NH_FLOAT_TO_WCHART];
	wchar_t wchCpWithRemanence[NH_FLOAT_TO_WCHART];
	wchar_t wchApWithoutRemanence[NH_FLOAT_TO_WCHART];
	wchar_t wchBpWithoutRemanence[NH_FLOAT_TO_WCHART];
	wchar_t wchCpWithoutRemanence[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity0[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity0[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity0[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime0[NH_INT_TO_WCHART];
	wchar_t wchPLHP0[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce0[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower0[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity1[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity1[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity1[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime1[NH_INT_TO_WCHART];
	wchar_t wchPLHP1[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce1[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower1[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity2[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity2[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity2[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime2[NH_INT_TO_WCHART];
	wchar_t wchPLHP2[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce2[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower2[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity3[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity3[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity3[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime3[NH_INT_TO_WCHART];
	wchar_t wchPLHP3[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce3[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower3[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity4[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity4[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity4[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime4[NH_INT_TO_WCHART];
	wchar_t wchPLHP4[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce4[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower4[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity5[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity5[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity5[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime5[NH_INT_TO_WCHART];
	wchar_t wchPLHP5[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce5[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower5[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity6[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity6[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity6[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime6[NH_INT_TO_WCHART];
	wchar_t wchPLHP6[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce6[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower6[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity7[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity7[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity7[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime7[NH_INT_TO_WCHART];
	wchar_t wchPLHP7[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce7[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower7[NH_FLOAT_TO_WCHART];
	wchar_t wchLowerVelocity8[NH_FLOAT_TO_WCHART];
	wchar_t wchUpperVelocity8[NH_FLOAT_TO_WCHART];
	wchar_t wchNomialVelocity8[NH_FLOAT_TO_WCHART];
	wchar_t wchPLHPTime8[NH_INT_TO_WCHART];
	wchar_t wchPLHP8[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByForce8[NH_FLOAT_TO_WCHART];
	wchar_t wchRemanenceByPower8[NH_FLOAT_TO_WCHART];
	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfDynPLHP(){ZeroMemory(this, sizeof(DemarcationResultOfDynPLHP));}
}DEMARCATIONRESULTOFDYNPLHP, *PDEMARCATIONRESULTOFDYNPLHP;

// 流量计流量校准结果<DemarcationResultOfFloFluxCal>
typedef struct DemarcationResultOfFloFluxCal
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];

	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfFloFluxCal(){ZeroMemory(this, sizeof(DemarcationResultOfFloFluxCal));}
}DEMARCATIONRESULTOFFLOFLUXCAL, *PDEMARCATIONRESULTOFFLOFLUXCAL;

// 流量计流量检查结果<DemarcationResultOfFloFluxChk>
typedef struct DemarcationResultOfFloFluxChk
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];

	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfFloFluxChk(){ZeroMemory(this, sizeof(DemarcationResultOfFloFluxChk));}
}DEMARCATIONRESULTOFFLOFLUXCHK, *PDEMARCATIONRESULTOFFLOFLUXCHK;

// 流量计稀释氧校准结果<DemarcationResultOfFloDilO2Cal>
typedef struct DemarcationResultOfFloDilO2Cal
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];

	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfFloDilO2Cal(){ZeroMemory(this, sizeof(DemarcationResultOfFloDilO2Cal));}
}DEMARCATIONRESULTOFFLODILO2CAL, *PDEMARCATIONRESULTOFFLODILO2CAL;

// 流量计压力校准结果<DemarcationResultOfFloPresCal>
typedef struct DemarcationResultOfFloPresCal
{
	wchar_t	wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchDemarcationNumber[50];

	wchar_t wchPass[NH_BOOL_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	DemarcationResultOfFloPresCal(){ZeroMemory(this, sizeof(DemarcationResultOfFloPresCal));}
}DEMARCATIONRESULTOFFLOPRESCAL, *PDEMARCATIONRESULTOFFLOPRESCAL;

// 系统参数<SystemParameters>
typedef struct SystemParameters
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchTimesOfPasswordInput[NH_INT_TO_WCHART];
	wchar_t wchTimesOfNoVIDTransmission[NH_INT_TO_WCHART];
	wchar_t wchTimesOfDynamometerCoastDown[NH_INT_TO_WCHART];
	wchar_t wchErrorOfDynamometerCoastDown[NH_FLOAT_TO_WCHART];
	wchar_t wchTimesOfDynamometerRevSensorCheck[NH_INT_TO_WCHART];
	wchar_t wchPasswordUnlock[50];
	wchar_t wchPasswordLock[50];
	wchar_t wchPeriodOfDynamometerCoastDown[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfDynamometerForceCheck[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfDynamometerRevSensorCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfEnvParamMeasurerCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfEnvParamMeasurerCheck[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfAnalyzerChannelCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfAnalyzerChannelCheck[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfFlowmeterFluxCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfFlowmeterFluxCheck[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfOpacimeterChannelCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfOpacimeterChannelCheck[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfNOxAnalyzerChannelCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfNOxAnalyzerChannelCheck[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfSmokemeterCalibration[NH_INT_TO_WCHART];
	wchar_t wchPeriodOfSmokemeterCheck[NH_INT_TO_WCHART];
	wchar_t wchTestPeriod[NH_INT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	SystemParameters(){ZeroMemory(this, sizeof(SystemParameters));}
}SYSTEMPARAMETERS, *PSYSTEMPARAMETERS;

// ASM检测参数<TestParametersOfASM>
typedef struct TestParametersOfASM
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchLoadParamOfASM5025[NH_FLOAT_TO_WCHART];
	wchar_t wchLoadParamOfASM2540[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfGAS[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfLPG[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfCNG[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfASM(){ZeroMemory(this, sizeof(TestParametersOfASM));}
}TESTPARAMETERSOFASM, *PTESTPARAMETERSOFASM;

// VMAS检测参数<TestParametersOfVMAS>
typedef struct TestParametersOfVMAS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchLoad_0_750[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_750_850[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_850_1020[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_1020_1250[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_1250_1470[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_1470_1700[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_1700_1930[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_1930_2150[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_2150_2380[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_2380_2610[NH_FLOAT_TO_WCHART];
	wchar_t wchLoad_2610_3500[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfGAS[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfLPG[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfCNG[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfVMAS(){ZeroMemory(this, sizeof(TestParametersOfVMAS));}
}TESTPARAMETERSOFVMAS, *PTESTPARAMETERSOFVMAS;

// 重型LUGDOWN检测参数<TestParametersOfLUGDOWNHEAVY>
typedef struct TestParametersOfLUGDOWNHEAVY
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchMaxOfLowLoadPower[NH_FLOAT_TO_WCHART];
	wchar_t wchRollerVelocityChangeRate[NH_FLOAT_TO_WCHART];
	wchar_t wchVehicleVelocityScanRange[NH_FLOAT_TO_WCHART];
	wchar_t wchSamplingTimeOfPowerScan[NH_FLOAT_TO_WCHART];
	wchar_t wchSamplingTimeOfOpacityTest[NH_FLOAT_TO_WCHART];
	wchar_t wchStableTimeOfPowerScan[NH_FLOAT_TO_WCHART];
	wchar_t wchStableTimeOfOpacityTest[NH_FLOAT_TO_WCHART];
	wchar_t wchIntervalOfTorque[NH_FLOAT_TO_WCHART];
	wchar_t wchIntervalOfVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerDiscountFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfLUGDOWNHEAVY(){ZeroMemory(this, sizeof(TestParametersOfLUGDOWNHEAVY));}
}TESTPARAMETERSOFLUGDOWNHEAVY, *PTESTPARAMETERSOFLUGDOWNHEAVY;

// 轻型LUGDOWN检测参数<TestParametersOfLUGDOWNLIGHT>
typedef struct TestParametersOfLUGDOWNLIGHT
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchMaxOfLowLoadPower[NH_FLOAT_TO_WCHART];
	wchar_t wchRollerVelocityChangeRate[NH_FLOAT_TO_WCHART];
	wchar_t wchVehicleVelocityScanRange[NH_FLOAT_TO_WCHART];
	wchar_t wchSamplingTimeOfPowerScan[NH_FLOAT_TO_WCHART];
	wchar_t wchSamplingTimeOfOpacityTest[NH_FLOAT_TO_WCHART];
	wchar_t wchStableTimeOfPowerScan[NH_FLOAT_TO_WCHART];
	wchar_t wchStableTimeOfOpacityTest[NH_FLOAT_TO_WCHART];
	wchar_t wchIntervalOfTorque[NH_FLOAT_TO_WCHART];
	wchar_t wchIntervalOfVelocity[NH_FLOAT_TO_WCHART];
	wchar_t wchPowerDiscountFactor[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfLUGDOWNLIGHT(){ZeroMemory(this, sizeof(TestParametersOfLUGDOWNLIGHT));}
}TESTPARAMETERSOFLUGDOWNLIGHT, *PTESTPARAMETERSOFLUGDOWNLIGHT;

// 双怠速检测参数<TestParametersOfDIS>
typedef struct TestParametersOfDIS
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchHISRevOfHeavyVehicle[NH_INT_TO_WCHART];
	wchar_t wchHISRevRangeOfHeavyVehicle[NH_INT_TO_WCHART];
	wchar_t wchHISRevOfLightVehicle[NH_INT_TO_WCHART];
	wchar_t wchHISRevRangeOfLightVehicle[NH_INT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfGAS[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfLPG[NH_FLOAT_TO_WCHART];
	wchar_t wchMinOfCOCO2OfCNG[NH_FLOAT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfDIS(){ZeroMemory(this, sizeof(TestParametersOfDIS));}
}TESTPARAMETERSOFDIS, *PTESTPARAMETERSOFDIS;

// 不透光自由加速检测参数<TestParametersOfFSUNHT>
typedef struct TestParametersOfFSUNHT
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRevFactor[NH_INT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfFSUNHT(){ZeroMemory(this, sizeof(TestParametersOfFSUNHT));}
}TESTPARAMETERSOFFSUNHT, *PTESTPARAMETERSOFFSUNHT;

// 滤纸式自由加速检测参数<TestParametersOfFSUYD>
typedef struct TestParametersOfFSUYD
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRevFactor[NH_INT_TO_WCHART];
	wchar_t wchOilTempLimit[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];

	TestParametersOfFSUYD(){ZeroMemory(this, sizeof(TestParametersOfFSUYD));}
}TESTPARAMETERSOFFSUYD, *PTESTPARAMETERSOFFSUYD;

// T2表参数
typedef struct T2
{
	wchar_t wchA[50];
	wchar_t wchB[50];
	wchar_t wchVIN[50];
	wchar_t wchTestTpe[NH_INT_TO_WCHART];
	wchar_t wchASM_HC5025[NH_FLOAT_TO_WCHART];
	wchar_t wchASM_CO5025[NH_FLOAT_TO_WCHART];
	wchar_t wchASM_NO5025[NH_FLOAT_TO_WCHART];
	wchar_t wchASM_HC2540[NH_FLOAT_TO_WCHART];
	wchar_t wchASM_CO2540[NH_FLOAT_TO_WCHART];
	wchar_t wchASM_NO2540[NH_FLOAT_TO_WCHART];
	wchar_t wchDIS_COHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchDIS_HCHigh[NH_FLOAT_TO_WCHART];
	wchar_t wchDIS_COLow[NH_FLOAT_TO_WCHART];
	wchar_t wchDIS_HCLow[NH_FLOAT_TO_WCHART];
	wchar_t wchDIS_Lambda[NH_FLOAT_TO_WCHART];
	wchar_t wchVMAS_HC[NH_FLOAT_TO_WCHART];
	wchar_t wchVMAS_CO[NH_FLOAT_TO_WCHART];
	wchar_t wchVMAS_NOx[NH_FLOAT_TO_WCHART];
	wchar_t wchVMAS_HCNOx[NH_FLOAT_TO_WCHART];
	wchar_t wchNHT_Value[NH_FLOAT_TO_WCHART];
	wchar_t wchFSUYD_Value[NH_FLOAT_TO_WCHART];
	wchar_t wchLUGDOWN_K100[NH_FLOAT_TO_WCHART];
	wchar_t wchLUGDOWN_K90[NH_FLOAT_TO_WCHART];
	wchar_t wchLUGDOWN_K80[NH_FLOAT_TO_WCHART];
	wchar_t wchLUGDOWN_CorrectedPower[NH_FLOAT_TO_WCHART];
	wchar_t wchLUGDOWN_EngineRev[NH_FLOAT_TO_WCHART];

	T2(){ZeroMemory(this, sizeof(T2));}
}*PT2;

// 数据库结构版本<DBVersion>
struct SDBVersion
{
	std::wstring strVersion;
	BYTE *pbProVer;
	std::wstring strRemark;

	// 以下字段不和数据库字段对应，指示图片缓冲区大小
	unsigned int nProVerSize;

	SDBVersion()
	{
		pbProVer = NULL;

		nProVerSize = 0;
	}
	
	~SDBVersion()
	{
		if (pbProVer != NULL)
		{
			delete pbProVer;
			pbProVer = NULL;
		}
	}

	void SetProVerData(const BYTE *pbPhotoData ,const unsigned int nSize)
	{
		SetPhotoData(&pbProVer, pbPhotoData, nSize);
		nProVerSize = nSize;
	}

private:
	// 拷贝数据到Photo字段，内存动态申请，析构时自动释放
	inline void SetPhotoData(BYTE **pbPhotoBuf, const BYTE *pbPhotoData, const unsigned int nSize)
	{
		if (nSize != 0)
		{
			if (*pbPhotoBuf != NULL)
			{
				delete *pbPhotoBuf;
				*pbPhotoBuf = NULL;
			}
			*pbPhotoBuf = new BYTE[nSize];
			memset(*pbPhotoBuf, 0, nSize);
			memcpy(*pbPhotoBuf, pbPhotoData, nSize);
		}
	}
};

typedef struct RealTimeDataOfDIM
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchEquNum[50];
	wchar_t wchSamplingTime[NH_TIME_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];
	RealTimeDataOfDIM(){ZeroMemory(this, sizeof(RealTimeDataOfDIM));}
}REALTIMEDATAOFDIM, *PREALTIMEDATAOFDIM;

typedef struct LogOfDIM
{
	wchar_t wchAutoID[NH_INT_TO_WCHART];
	wchar_t wchRunningNumber[50];
	wchar_t wchEquNum[50];
	wchar_t wchOperator[50];
	wchar_t wchSamplingTime[NH_TIME_TO_WCHART];
	wchar_t wchEnvironmentalTemperature[NH_FLOAT_TO_WCHART];
	wchar_t wchRelativeHumidity[NH_FLOAT_TO_WCHART];
	wchar_t wchAtmosphericPressure[NH_FLOAT_TO_WCHART];
	wchar_t wchRemark[1024];
	wchar_t wchReserved1[50];
	wchar_t wchReserved2[50];
	wchar_t wchReserved3[50];
	LogOfDIM(){ZeroMemory(this, sizeof(LogOfDIM));}
}LOGOFDIM, *PLOGOFDIM;
#endif