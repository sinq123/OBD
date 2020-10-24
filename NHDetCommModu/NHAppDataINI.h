/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHAppDataINI.h
* 文件标识：
* 摘    要：App_Data数据配置文件基本操作,数据结构格式定义
*
* 当前版本：1.0
* 作    者：HB
* 完成日期：2010-07-14
*
*
* 历史信息：
*
* 版本：1.0.19
* 日期：2013-02-28
* 作者：Chc
* 描述：增加ASM、VMAS、DIS、LUGDOWN的以vector方式读取过程数据函数GetIniRealTimeDataOfXXXXXXVector
*
* 版本：1.0.18
* 日期：2012-08-13
* 作者：Wu
* 描述：1)函数SetIniUserInfo,添加写入用户密码功能
*		2)函数GetIniUserInfo,添加获取用户密码功能
*
* 版本：1.0.17
* 日期：2012-07-05
* 作者：Raylee
* 描述：修正GetIniLiteString()读取ini时指定的缓冲区长度为wchar_t类型长度
*
* 版本：1.0.16
* 日期：2011-09-28
* 作者：Raylee
* 描述：添加摩托车双怠速相关结构的ini读写操作
*
* 版本：1.0.15
* 日期：2011-08-24
* 作者：hemincong
* 描述：使用inilite读写ini文件
*
* 版本：1.0.14
* 日期：2011-06-16
* 作者：Sjming
* 描述：由于CNHReadIni类不稳定，现恢复使用API函数读取Ini文件
*
* 版本：1.0.13
* 日期：2011-06-01
* 作者：hemincong
* 描述：对大部分GetIni函数改用CNHReadIni读取
*
* 版本：1.0.12
* 日期：2011-05-26
* 作者：Raylee
* 描述：1）添加FSUNHT和FSUYD结果ini对保留字段的操作
*		2）修正读取Remark字段缓冲区长度为1024
*
* 版本：1.0.11
* 日期：2011-05-18
* 作者：Ming
* 描述：函数GetIniRealTimeDataOfxxx，添加采样时间SamplingTime的读取操作
*
* 版本：1.0.10
* 日期：2011-04-15
* 作者：Ming
* 描述：GetIniVehicleInfo/SetIniVehicleInfo函数添加对OwnerCharacter、OwnerDistrict、LicenseDepartment、LicenseIssueDate和LicenseValidDate的读写操作
*		GetDboTestLog/SetDboTestLog函数添加对IsSentEP、OwnerCharacter、OwnerDistrict、LicenseDepartment、LicenseIssueDate和LicenseValidDate的读写操作
*
* 版本：1.0.9
* 日期：2011-04-02
* 作者：Raylee
* 描述：GetIniSystemParameters/SetIniSystemParameters函数添加读写PeriodOfDynamometerForceCheck操作
*
* 版本：1.0.8
* 作者：Ming
* 日期：2011-02-23
* 描述：修正函数GetIniResultOfFloFluxChk()读错ResultOfFloFluxChk项的Bug
*
* 版本：1.0.7
* 作者：Raylee
* 日期：2011-01-10
* 描述：根据数据结构的改变，修改函数GetIniResultOfDynForceChk()
*
* 版本：1.0.6
* 作者：Ming
* 日期：2011-01-10
* 描述：1)函数GetIniTestLog和SetIniTestLog,添加成员EmissionStandard、EngineStroke、AnnualTestPeriod、HasOBD的获取和写入
*
* 版本：1.0.5
* 作者：Raylee
* 日期：2011-01-05
* 描述：修改函数GetIniResultOfDynForceChk()
*
* 版本：1.0.4
* 作者：Ming
* 日期：2010-12-11
* 描述：1)函数GetIniBillInfo、GetIniTestLog,分别添加成员NumberOfTestPeriodAVFSU(检测周期内的AVFSU检测次数)和NumberOfTestPeriodMI(检测周期内的MI检测次数)的获取
*		2)函数SetIniBillInfo、SetIniTestLog,分别添加成员NumberOfTestPeriodAVFSU(检测周期内的AVFSU检测次数)和NumberOfTestPeriodMI(检测周期内的MI检测次数)的写入
*		3)添加函数SetIniResultOfAVFSU、GetIniResultOfAVFSU 农用车自由加速结果数据的获取和写入
*		4)添加函数GetIniRealTimeDataOfMI、SetIniResultOfMI、GetIniResultOfMI 摩托车怠速结果数据的获取和写入，实时数据的获取
*
* 版本：1.0.4
* 作者：Ming
* 日期：2010-09-21
* 描述：1)函数GetIniRealTimeDataOfASM、GetIniRealTimeDataOfLUGDOWN和GetIniRealTimeDataOfVMAS,分别添加成员IHP(指示功率)和PLHP(寄生功率)的获取
*
* 版本：1.0.3
* 作者：Ming
* 日期：2010-09-20
* 描述：1)函数GetIniVehicleInfo和SetIniVehicleInfo,添加成员EmissionStandard、EngineStroke、AnnualTestPeriod、HasOBD的获取和写入
*
* 版本：1.0.2
* 作者：Ming
* 日期：2010-09-07
* 描述：1)函数GetIniSystemParameters和SetIniSystemParameters,添加成员PeriodOfEnvParamMeasurerCheck和TestPeriod的获取和写入
*		2)修正函数SetIniSystemParameters部分字段填写错误的bug
*
* 版本：1.0.1
* 作者：Ming
* 日期：2010-08-13
* 描述：添加获取不透光自由加速实时数据函数GetIniRealTimeDataOfFSUNHT.
*/

#ifndef NHAPPDATAINI_H
#define NHAPPDATAINI_H

// 备注：各类型转换成wchar_t对照表
// int wchar_t[11] 有符号
// short wchar_t[6] 有符号
// time wchar_t[19] 精确到秒 2008-08-08 23:59:59

#include "NHDef.h"
#include "NHStructDef.h"
#include <list>
#include <vector>
using std::list;
using std::vector;

// App_Data
// 用户信息 UserInfo.ini

void GetIniUserInfo(PUSERINFO const pIniUserInfo);
void SetIniUserInfo(const USERINFO& iniUserInfo);

// 开单信息<BillInfo>
//void GetIniBillInfo(PBILLINFO const pIniBillInfo);
//void SetIniBillInfo(const BILLINFO& iniBillInfo);

// 车辆信息<VehicleInfo>
void GetIniVehicleInfo(PVEHICLEINFO const pIniVehicleInfo);
void SetIniVehicleInfo(const VEHICLEINFO& iniVehicleInfo);

// 车型信息<VehicleInfo>
void GetIniVehicleModel(PVEHICLEMODEL const pIniVehicleModel);
void SetIniVehicleModel(const VEHICLEMODEL& iniVehicleModel);

// ASM限值信息<LimitOfASM>
void GetIniLimitOfASM(PLIMITOFASM const pIniLimitOfASM);
void SetIniLimitOfASM(const LIMITOFASM& iniLimitOfASM);

// VMAS限值信息<LimitOfVMAS>
void GetIniLimitOfVMAS(PLIMITOFVMAS const pIniLimitOfVMAS);
void SetIniLimitOfVMAS(const LIMITOFVMAS& iniLimitOfVMAS);

// LimitOfLUGDOWN限值信息<LimitOfLimitOfLUGDOWN>
void GetIniLimitOfLUGDOWN(PLIMITOFLUGDOWN const pIniLimitOfLUGDOWN);
void SetIniLimitOfLUGDOWN(const LIMITOFLUGDOWN& iniLimitOfLUGDOWN);

// DIS限值信息<LimitOfDIS>
void GetIniLimitOfDIS(PLIMITOFDIS const pIniLimitOfDIS);
void SetIniLimitOfDIS(const LIMITOFDIS& iniLimitOfDIS);

// FSUNHT限值信息<LimitOfFSUNHT>
void GetIniLimitOfFSUNHT(PLIMITOFFSUNHT const pIniLimitOfFSUNHT);
void SetIniLimitOfFSUNHT(const LIMITOFFSUNHT& iniLimitOfFSUNHT);

// FSUNHTOfHeavyVehicle限值信息<LimitOfFSUNHTOfHeavyVehicle>
void GetIniLimitOfFSUNHTOfHeavyVehicle(PLIMITOFFSUNHTOFHEAVYVEHICLE const pIniLimitOfFSUNHTOfHeavyVehicle);
void SetIniLimitOfFSUNHTOfHeavyVehicle(const LIMITOFFSUNHTOFHEAVYVEHICLE& iniLimitOfFSUNHTOfHeavyVehicle);

// FSUNHTOfLightVehicle限值信息<LimitOfFSUNHTOfLightVehicle>
void GetIniLimitOfFSUNHTOfLightVehicle(PLIMITOFFSUNHTOFLIGHTVEHICLE const pIniLimitOfFSUNHTOfLightVehicle);
void SetIniLimitOfFSUNHTOfLightVehicle(const LIMITOFFSUNHTOFLIGHTVEHICLE& iniLimitOfFSUNHTOfLightVehicle);

// FSUYD限值信息<LimitOfFSUYD>
void GetIniLimitOfFSUYD(PLIMITOFFSUYD const pIniLimitOfFSUYD);
void SetIniLimitOfFSUYD(const LIMITOFFSUYD& iniLimitOfFSUYD);

// 检测线信息<LineInfo>
void GetIniLineInfo(PLINEINFO const pIniLineInfo);
void SetIniLineInfo(const LINEINFO& iniLineInfo);

// 检测站信息<StationInfo>
void GetIniStationInfo(PSTATIONINFO const pIniStationInfo);
void SetIniStationInfo(const STATIONINFO& iniStationInfo);

// 排放检测日志信息<TestLog>
void GetIniTestLog(PTESTLOG const pIniTestLog);
void SetIniTestLog(const TESTLOG& iniTestLog);

// DIS检测结果<ResultOfDIS>
void GetIniResultOfDIS(PRESULTOFDIS const pIniResultOfDIS);
void GetIniResultOfDISEx(SResultOfDIS* const pResultOfDIS);
void SetIniResultOfDIS(const RESULTOFDIS& iniResultOfDIS);

// DIS real time data<RealTimeDataOfDIS>
void GetIniRealTimeDataOfDIS(list<REALTIMEDATAOFDIS>& listRealTimeDataOfDIS);
void GetIniRealTimeDataOfDISVector(vector<REALTIMEDATAOFDIS>& vRealTimeDataOfDIS);
void GetIniRealTimeDataOfDISEx(std::vector<SRealTimeDataOfDIS> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfDIS.ini");

// ASM test result<ResultOfASM>
void GetIniResultOfASM(PRESULTOFASM const pIniResultOfASM);
void GetIniResultOfASMEx(SResultOfASM* const pResultOfASM);
void SetIniResultOfASM(const RESULTOFASM& iniResultOfASM);

// ASM real time data<RealTimeDataOfASM>
void GetIniRealTimeDataOfASM(list<REALTIMEDATAOFASM>& listRealTimeDataOfASM);
void GetIniRealTimeDataOfASMVector(vector<REALTIMEDATAOFASM>& vRealTimeDataOfASM);
void GetIniRealTimeDataOfASMEx(std::vector<SRealTimeDataOfASM> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfASM.ini");

// VMAS test result<ResultOfVMAS>
void GetIniResultOfVMAS(PRESULTOFVMAS const pIniResultOfVMAS);
void GetIniResultOfVMASEx(SResultOfVMAS* const pResultOfVMAS);
void SetIniResultOfVMAS(const RESULTOFVMAS& iniResultOfVMAS);

// VMAS real time data<RealTimeDataOfVMAS>
void GetIniRealTimeDataOfVMAS(list<REALTIMEDATAOFVMAS>& listRealTimeDataOfVMAS);
void GetIniRealTimeDataOfVMASVector(vector<REALTIMEDATAOFVMAS>& vRealTimeDataOfVMAS);
void GetIniRealTimeDataOfVMASEx(std::vector<SRealTimeDataOfVMAS> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfVMAS.ini");

// LUGDOWN test result<ResultOfLUGDOWN>
void GetIniResultOfLUGDOWN(PRESULTOFLUGDOWN const pIniResultOfLUGDOWN);
void GetIniResultOfLUGDOWNEx(SResultOfLUGDOWN* const pResultOfLUGDOWN);
void SetIniResultOfLUGDOWN(const RESULTOFLUGDOWN& iniResultOfLUGDOWN);

// LUGDOWN real time data<RealTimeDataOfLUGDOWN>
void GetIniRealTimeDataOfLUGDOWN(list<REALTIMEDATAOFLUGDOWN>& listRealTimeDataOfLUGDOWN);
void GetIniRealTimeDataOfLUGDOWNVector(vector<REALTIMEDATAOFLUGDOWN>& vRealTimeDataOfLUGDOWN);
void GetIniRealTimeDataOfLUGDOWNEx(std::vector<SRealTimeDataOfLUGDOWN> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfLUGDOWN.ini");

// FSUNHT test result<ResultOfFSUNHT>
void GetIniResultOfFSUNHT(PRESULTOFFSUNHT const pIniResultOfFSUNHT);
void GetIniResultOfFSUNHTEx(SResultOfFSUNHT* const pResultOfFSUNHT);
void SetIniResultOfFSUNHT(const RESULTOFFSUNHT& iniResultOfFSUNHT);

// FSUNHT real time data<RealTimeDataOfFSUNHT>
void GetIniRealTimeDataOfFSUNHT(list<REALTIMEDATAOFFSUNHT>& listRealTimeDataOfFSUNHT);
void GetIniRealTimeDataOfFSUNHTVector(vector<REALTIMEDATAOFFSUNHT>& vRealTimeDataOfFSUNHT);
void GetIniRealTimeDataOfFSUNHTEx(std::vector<SRealTimeDataOfFSUNHT> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfFSUNHT.ini");

// FSUYD test result<ResultOfFSUYD>
void GetIniResultOfFSUYD(PRESULTOFFSUYD const pIniResultOfFSUYD);
void SetIniResultOfFSUYD(const RESULTOFFSUYD& iniResultOfFSUYD);

// AVFSU test result<ResultOfAVFSU>
void GetIniResultOfAVFSU(PRESULTOFAVFSU const pIniResultOfAVFSU);
void SetIniResultOfAVFSU(const RESULTOFAVFSU& iniResultOfAVFSU);

// MI test result<ResultOfMI>
void GetIniResultOfMI(PRESULTOFMI const pIniResultOfMI);
void SetIniResultOfMI(const RESULTOFMI& iniResultOfMI);

// MI real time data<RealTimeDataOfMI>
void GetIniRealTimeDataOfMI(list<REALTIMEDATAOFMI>& listRealTimeDataOfMI);

// MDIS test result<ResultOfMDIS>
void GetIniResultOfMDIS(PRESULTOFMDIS const pIniResultOfMDIS);
void SetIniResultOfMDIS(const RESULTOFMDIS& iniResultOfMDIS);

// MDIS real time data<RealTimeDataOfMDIS>
void GetIniRealTimeDataOfMDIS(list<REALTIMEDATAOFMDIS>& listRealTimeDataOfMDIS);

// 设备校准检查信息<EquCalChkInfo>
void GetIniEquCalChkInfo(PEQUCALCHKINFO const pIniEquCalChkInfo);
void SetIniEquCalChkInfo(const EQUCALCHKINFO& iniEquCalChkInfo);

// 设备系统日志<SystemLog>
void GetIniSystemLog(PSYSTEMLOG const pIniSystemLog);
void SetIniSystemLog(const SYSTEMLOG& iniSystemLog);

// 标定日志信息<DemarcationLog>
void GetIniDemarcationLog(PDEMARCATIONLOG const pIniDemarcationLog);
void SetIniDemarcationLog(const DEMARCATIONLOG& iniDemarcationLog);

// 分析仪气体校准结果<DemarcationResultOfAnaGasCal>
void GetIniResultOfAnaGasCal(PDEMARCATIONRESULTOFANAGASCAL const pIniResultOfAnaGasCal);

// 分析仪气体检查结果<DemarcationResultOfAnaGasChk>
void GetIniResultOfAnaGasChk(PDEMARCATIONRESULTOFANAGASCHK const pIniResultOfAnaGasChk, wchar_t *pwchSaveFileName=L"ResultOfAnaGasChk.ini");
void GetIniRealTimeDataOfAnaGasChk(list<DEMARCATIONREALTIMEDATAOFANAGASCHK>& listRealTimeDataOfAnaGasChk);

// 不透光度计校准检查结果<DemarcationResultOfOpaCalChk>
void GetIniResultOfOpaCalChk(PDEMARCATIONRESULTOFOPACALCHK const pIniResultOfOpaCalChk);

// 滤纸式烟度计检查结果<DemarcationResultOfSmoChk>
void GetIniResultOfSmoChk(PDEMARCATIONRESULTOFSMOCHK const pIniResultOfSmoChk);

// 测功机加载滑行结果<DemarcationResultOfDynConstLoad>
void GetIniResultOfDynConstLoad(PDEMARCATIONRESULTOFDYNCONSTLOAD const pIniResultOfDynConstLoad);
void GetIniRealTimeDataOfDynConstLoad(list<DEMARCATIONREALTIMEDATAOFDYNCONSTLOAD>& listRealTimeDataOfDynConstLoad);

// 测功机力校准结果<DemarcationResultOfDynForceCal>
void GetIniResultOfDynForceCal(PDEMARCATIONRESULTOFDYNFORCECAL const pIniResultOfDynForceCal);

// 测功机力检查结果<DemarcationResultOfDynForceChk>
void GetIniResultOfDynForceChk(PDEMARCATIONRESULTOFDYNFORCECHK const pIniResultOfDynForceChk);

// 测功机寄生功率滑行结果<DemarcationResultOfDynPLHP>
void GetIniResultOfDynPLHP(PDEMARCATIONRESULTOFDYNPLHP const pIniResultOfDynPLHP);

// 流量计流量校准结果<DemarcationResultOfFloFluxCal>
void GetIniResultOfFloFluxCal(PDEMARCATIONRESULTOFFLOFLUXCAL const pIniResultOfFloFluxCal);

// 流量计流量检查结果<DemarcationResultOfFloFluxChk>
void GetIniResultOfFloFluxChk(PDEMARCATIONRESULTOFFLOFLUXCHK const pIniResultOfFloFluxChk);

// 流量计稀释氧校准结果<DemarcationResultOfFloDilO2Cal>
void GetIniResultOfFloDilO2Cal(PDEMARCATIONRESULTOFFLODILO2CAL const pIniResultOfFloDilO2Cal);

// 流量计压力校准结果<DemarcationResultOfFloPresCal>
void GetIniResultOfFloPresCal(PDEMARCATIONRESULTOFFLOPRESCAL const pIniResultOfFloPresCal);

// 系统参数<SystemParameters>
void GetIniSystemParameters(PSYSTEMPARAMETERS const pIniSystemParameters);
void SetIniSystemParameters(const SYSTEMPARAMETERS& iniSystemParameters);

// OBD检测结果<ResultOfOBD>
void  GetIniResultOfOBD(SResultOfOBD* const pResultOfOBD);
void  SetIniResultOfOBD(const SResultOfOBD& sResultOfOBD);

// VMAS idle data<RealTimeDataOfVMAS>
void GetIniIdleDataOfVMAS(list<REALTIMEDATAOFVMAS>& listIdleDataOfVMAS);
void SetIniIdleDataOfVMAS(REALTIMEDATAOFVMAS* pIdleDataOfVMAS);

// DIS idle data<RealTimeDataOfDIS>
void GetIniIdleDataOfDIS(list<REALTIMEDATAOFDIS>& listIdleDataOfDIS);
void SetIniIdleDataOfDIS(REALTIMEDATAOFDIS* pIdleDataOfDIS);

void SetIniRealTimeDataOfDISEx(std::list<SRealTimeDataOfDIS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfDIS.ini");//导出实时数据
void SetIniRealTimeDataOfVMASEx(std::list<SRealTimeDataOfVMAS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfVMAS.ini");//导出实时数据
void SetIniRealTimeDataOfFSUNHTEx(std::list<SRealTimeDataOfFSUNHT> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfFSUNHT.ini");//导出实时数据
void SetIniRealTimeDataOfLUGDOWNEx(std::list<SRealTimeDataOfLUGDOWN> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfLUGDOWN.ini");//导出实时数据
void SetIniRealTimeDataOfASMEx(std::list<SRealTimeDataOfASM> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfASM.ini");//导出实时数据


void GetIniRealTimeDataOfDISEx(std::list<SRealTimeDataOfDIS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfDIS.ini");//读实时数据
void GetIniRealTimeDataOfVMASEx(std::list<SRealTimeDataOfVMAS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfVMAS.ini");//读实时数据
void GetIniRealTimeDataOfFSUNHTEx(std::list<SRealTimeDataOfFSUNHT> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfFSUNHT.ini");//读实时数据
void GetIniRealTimeDataOfLUGDOWNEx(std::list<SRealTimeDataOfLUGDOWN> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfLUGDOWN.ini");//读实时数据
void GetIniRealTimeDataOfASMEx(std::list<SRealTimeDataOfASM> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfASM.ini");//读实时数据

// 分析仪气体检查结果<ResultOfAnaGasChk>
void SetIniResultOfAnaGasChk(const DEMARCATIONRESULTOFANAGASCHK &IniResultOfAnaGasChk, wchar_t *pwchSaveFileName=L"ResultOfAnaGasChk.ini");
#endif