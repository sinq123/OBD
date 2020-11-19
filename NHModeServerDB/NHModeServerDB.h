/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHModeServerDB.h
* 文件标识：
* 描述：NHModeServerDB数据库基本操作,返回值逻辑：除注明返回值逻辑的函数外，其它函数遵循如下返回规则：
*		Getxxx：执行成功返回0x01，无对应记录返回0x00，异常返回0xff；Setxxx：成功返回0x00，异常返回0xff
*
* 版本：1.0
* 作者：HuaBei
* 日期：2010-07-14
*
* 历史信息：
*
* 版本：1.0.34
* 日期：2014-06-20
* 作者：Lihy
* 描述：在GetDboRealTimeDataOfLUGDOWN()函数中增加读取PowerCorrectionFactor字段
*
* 版本：1.0.33
* 日期：2014-05-04
* 作者：Lihy
* 描述：在GetDboTestLog()函数中增加读取SRM字段
*
* 版本：1.0.32
* 日期：2013-08-19
* 作者：Chc
* 描述：修改过程数据读取不取ReportNumber和RunningNumber的BUG
*
* 版本：1.0.31
* 日期：2013-06-22
* 作者：Chc
* 描述：修改过程数据读取和写入压缩函数内存泄露的BUG
*
* 版本：1.0.30
* 日期：2013-07-12
* 作者：Duanjf
* 描述：SetDboSystemLog系统日志SetDboLineLock检测线锁止表操作
*
* 版本：1.0.29
* 日期：2013-06-22
* 作者：Chc
* 描述：加入稳态、简易瞬态、加载减速、双怠速、摩托车怠速和摩托车双怠速的过程数据读取和写入压缩函数
*
* 版本：1.0.28
* 日期：2013-04-02
* 作者：Chc
* 描述：1)在获取FSNHT限值函数中，如果获取限值失败或空，限值都设置为最宽松限值，限值尾数都相应加1，以便区分限值的来源
		2)修改DIS获取默认限值的错误
		3)增加用户权限的读取和写入函数
		4)修改摩托车双怠速结果数据的读取和写入函数
*
* 版本：1.0.27
* 日期：2013-01-28
* 作者：Chc
* 描述：1)增加读取白名单函数GetDboWhiteList
*		2)增加读取重复性函数GetDboRepeat
*
* 版本：1.0.26
* 日期：2013-01-05
* 作者：Lihy
* 描述：修正GetDboXXX函数,使输出日期格式为XXXX-XX-XX XX:XX:XX
*
* 版本：1.0.25
* 日期：2012-10-08
* 作者：Chc
* 描述：修正GetDboTestLog函数,使输出日期格式为XXXX-XX-XX XX:XX:XX
*
* 版本：1.0.24
* 日期：2012-10-08
* 作者：wuyq
* 描述：修正SetDboResultOfDIS函数,使数据库中O2保存正确的数据(之前保存的数据是CO2的)
*
*
*版本：1.0.23
*日期：2010-05-19
*作者：Modb
*描述：1)类NHModeServerDB,函数GetDboBillInfo、GetDboTestLog、SetDboBillInfo、SetTestLog添加接口wchNumberOfTestPeriodMDIS
	   2)类NHModeServerDB,函数GetDboTestAmount、SetDboTestAmount添加接口wchMDISAmount
	   3)类NHModeServerDB,添加获取摩托车双怠速结果数据函数GetDboResultOfMDIS
	   4)类NHModeServerDB,添加写入摩托车双怠速结果数据函数SetDboResultOfMDIS
	   5)类NHModeServerDB,添加写入摩托车双怠速实时数据函数SetDborealTimeDataOfMDIS

* 版本：1.0.22
* 日期：2011-10-10
* 作者：hemincong
* 描述：.vt = VT_NULL;改为.ChangeType(VT_NULL);
*
* 版本：1.0.21
* 日期：2011-05-19
* 作者：Ming
* 描述：1)添加函数SetDboTestLogIsSentEP()
*
* 版本：1.0.20
* 日期：2011-05-17
* 作者：Raylee
* 描述：添加Sampling字段的相应操作
*
* 版本：1.0.19
* 日期：2011-04-28
* 作者：Liangzs
* 描述：1）统一NHModeServerDB和NHModeClientDB两个库的接口（包括函数名，参数列表，返回值），除注明返回值逻辑的函数外，
*		其它函数遵循如下返回规则：Getxxx：执行成功返回0x01，无对应记录返回0x00，异常返回0xff；Setxxx：成功返回0x00，
*		异常返回0xff
*		2）每个函数加上注释，特别是加上返回值的注释
*
* 版本：1.0.18
* 日期：2011-04-25
* 作者：Raylee
* 描述：1）修正SetTestLog函数，以RunningNumber为主键插入
*		2）注释数据库超时设置，使用ADO默认设置
*
* 版本：1.0.17
* 日期：2011-04-15
* 作者：Ming
* 描述：函数GetDboVehicleInfo,SetDboVehicleInfo,添加读取和写入字段OwnerCharacter、OwnerDistrict、LicenseDepartment、LicenseIssueDate和LicenseValidDate
*		函数GetDboTestLog,SetDboTestLog,添加读取和写入字段IsSentEP、OwnerCharacter、OwnerDistrict、LicenseDepartment、LicenseIssueDate和LicenseValidDate
*
* 版本：1.0.16
* 日期：2011-04-07
* 作者：Ming
* 描述：添加函数GetDboStationTestAmount,SetDboStationTestAmount,实现生成检测站顺序号
*
* 版本：1.0.15
* 日期：2011-04-02
* 作者：Raylee
* 描述：GetDboSystemParameters函数添加读取PeriodOfDynamometerForceCheck字段
*
* 版本：1.0.14
* 日期：2011-03-11
* 作者：Raylee
* 描述：修正GetDboSystemParameters函数没有读取PeriodOfEnvParamMeasurerCheck字段的bug
*
* 版本：1.0.13
* 日期：2011-01-20
* 作者：Ming
* 描述：1)添加获取实时数据函数GetDboRealTimeDataOfASM()、GetDboRealTimeDataOfLUGDOWN()、GetDboRealTimeDataOfDIS()、GetDboRealTimeDataOfVMAS()
*		2)添加函数SetDboTestLogIsSent()
*
* 版本：1.0.12
* 日期：2011-01-17
* 作者：Raylee
* 描述：修正GetDboSystemParameters()函数没有读取TestPeriod字段的bug
*
* 版本：1.0.11
* 日期：2011-01-15
* 作者：Raylee
* 描述：添加函数GetDboResultOfVMAS()
*
* 版本：1.0.10
* 日期：2011-01-10
* 作者：Ming
* 描述：1)在获取ASM、LUGDOWN、VMAS、DIS、FSUYD限值函数中，如果获取限值失败或空，限值都设置为最宽松限值，限值尾数都相应加1，以便区分限值的来源
*		2)函数GetDboTestLog，SetDboTestLog，SetDboVehicleInfo添加四个个接口wchEngineStroke、wchEmissionStandard、wchAnnualTestPeriod、wchHasOBD
*
* 版本：1.0.9
* 日期：2010-12-30
* 作者：Ming
* 描述：添加设置检测线状态函数SetDboLineInfoLineState
*
* 版本：1.0.8
* 日期：2010-12-27
* 作者：Raylee
* 描述：修正GetDboVehicleModel函数没有读取PassengerVehicle的bug
*
* 版本：1.0.7
* 日期：2010-12-11
* 作者：Ming
* 描述：1)函数GetDboBillInfo、GetDboTestLog、SetDboBillInfo、SetTestLog添加接口wchNumberOfTestPeriodAVFSU和wchNumberOfTestPeriodMI	
*		2)函数GetDboTestAmount、SetDboTestAmount添加接口wchMIAmount和wchAVFSUAmount
*		3)添加获取农用车自由加速结果数据函数GetDboResultOfAVFSU
*		4)添加写入农用车自由加速结果数据函数SetDboResultOfAVFSU
*		5)添加获取摩托车怠速结果数据函数GetDboResultOfMI
*		6)添加写入摩托车怠速结果数据函数SetDboResultOfMI
*		7)添加写入摩托车怠速实时数据函数SetDborealTimeDataOfMI
*
* 版本：1.0.6
* 日期：2010-11-30
* 作者：Raylee
* 描述：添加UpdateTestPhoto()，写拍照时间接口
*
* 版本：1.0.5
* 日期：2010-10-30
* 作者：Raylee
* 描述：优化检测实时数据的插入，提高速度
*
* 版本：1.0.4
* 日期：2010-10-08
* 作者：Raylee
* 描述：修正SetDboResultOfASM函数，Lambda5025做转换时，_wtoi改为_wtof
*
* 版本：1.0.3
* 日期：2010-09-20
* 作者：Ming
* 描述：函数GetDboVehicleInfo添加四个个接口wchEngineStroke、wchEmissionStandard、wchAnnualTestPeriod、wchHasOBD
*
* 版本：1.0.2
* 日期：2010-09-14
* 作者：Qin
* 描述：函数SetDboBillInfoTestState加入两个接口wchLineNumber(检测线编号)与StartTime(改变检测状态时间)
*
* 版本：1.0.1
* 日期：2010-07-29
* 作者：CYX
* 描述：修正函数GetDboResultOfASM读取LimitOfCO2540_EDP字段的Bug
*
* 版本：1.0.0
* 日期：2010-07-14
* 作者：hyh, Ming
* 描述：正式发布使用
*/


#ifndef NHMODESERVERDB_H
#define NHMODESERVERDB_H

// 由于是导出文件,所以要重新定义
#ifndef SQLMAX
#define SQLMAX 1024 // sql 语句最大输入字符数
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#include <list>
using std::list;

//extern "C"
//{
	// 用户信息<UserInfo> UserInfo表的所有操作

	// 获取用户信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboUserInfo(const wchar_t wchSql[SQLMAX], PUSERINFO const pDboUserInfo);
	// 设置用户信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboUserInfo(const USERINFO &dboUserInfo);
	// 删除用户信息
	// 该操作未实现
	DWORD DelDboUserInfo(const wchar_t wchSql[SQLMAX]);
	// 获取用户权限
	// 返回：
	// 0x01：获取成功
	// 0x02：获取失败
	// 0xff：数据库异常
	DWORD GetDboUserPermission(const wchar_t *const pwchName, std::string &strPermission);
	// 写权限数据入数据库
	// 返回：
	// 0x00：写入成功
	// 0xff：数据库异常
	DWORD SetDboUserPermission(const wchar_t *const pwchName, const std::string &strPermission);
	

	// 开单信息<BillInfo> BillInfo表的所有操作
	// 获取开单信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	//DWORD GetDboBillInfo(const wchar_t wchSql[SQLMAX], PBILLINFO const pDboBillInfo);
	// 设置开单信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	//DWORD SetDboBillInfo(const BILLINFO &dboBillInfo);
	// 设置开单信息中的检测状态，开始处理时间，检测线编号
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	//DWORD SetDboBillInfoTestState(const wchar_t wchRunningNumber[50], const short siTestState,const wchar_t wchLineNumber[50] = NULL, const SYSTEMTIME *pstProcessingTime = NULL);
	// 设置开单信息中的检测状态和检测线编号
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboBillInfoSelectState(const wchar_t wchRunningNumber[50], const wchar_t wchLineNumber[50]);
	// 程序异常退出时重置在检车辆状态为待检
	// 返回：
	// 0x00：执行成功
	// 0xff：数据库异常
	DWORD UpdateDboBillInfoPowerFailure(const wchar_t wchLineNumber[50]);
	// 删除检测完成的开单信息
	// 返回：
	// 0x00：删除成功
	// 0xff：数据库异常
	DWORD DelDboBillInfoTestFinished(const wchar_t wchRunningNumber[50]);
	// 删除开单信息
	// 返回：
	// 0x00：删除成功
	// 0xff：数据库异常
	DWORD DelDboBillInfo(const wchar_t wchSql[SQLMAX]);
	// 车辆信息<VehicleInfo> Vehicle表的所有操作
	// 获取车辆信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboVehicleInfo(const wchar_t wchSql[SQLMAX], PVEHICLEINFO const pDboVehicleInfo);
	// 设置车辆信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboVehicleInfo(const VEHICLEINFO &dboVehicleInfo);

	// 车型信息<VehicleModel> VehicleModel表的所有操作
	// 获取车型信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboVehicleModel(const wchar_t wchSql[SQLMAX], PVEHICLEMODEL const pDboVehicleModel);
	// 设置车型信息
	// 返回：
	// 0x00：修改成功
	// 0x01: 添加成功
	// 0xff：数据库异常
	DWORD SetDboVehicleModel(const VEHICLEMODEL &dboVehicleModel);

	// ASM限值信息<LimitOfASM> LimitOfASM表的所有操作
	// 获取ASM限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfASM(const wchar_t wchSql[SQLMAX], PLIMITOFASM const pDboLimitOfASM);

	// Vmas限值信息<LimitOfVMAS> LimitOfVMAS表的所有操作
	// 获取VMAS限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfVMAS(const wchar_t wchSql[SQLMAX], PLIMITOFVMAS const pDboLimitOfVMAS);

	// LimitOfLUGDOWN限值信息<LimitOfLimitOfLUGDOWN> LimitOfLimitOfLUGDOWN表的所有操作
	// 获取LUGDOWN限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfLUGDOWN(const wchar_t wchSql[SQLMAX], PLIMITOFLUGDOWN const pDboLimitOfLUGDOWN);

	// DIS限值信息<LimitOfDIS> LimitOfDIS表的所有操作
	// 获取DIS限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfDIS(const wchar_t wchSql[SQLMAX], PLIMITOFDIS const pDboLimitOfDIS);

	// FSUNHT限值信息<LimitOfFSUNHT> LimitOfFSUNHT表的所有操作
	// 获取FSNHT限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfFSUNHT(const wchar_t wchSql[SQLMAX], PLIMITOFFSUNHT const pDboLimitOfFSUNHT);

	// FSUNHTOfHeavyVehicle限值信息<LimitOfFSUNHTOfHeavyVehicle> LimitOfFSUNHTOfHeavyVehicle表的所有操作
	// 获取FSUNHTOfHeavyVehicle限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfFSUNHTOfHeavyVehicle(const wchar_t wchSql[SQLMAX], PLIMITOFFSUNHTOFHEAVYVEHICLE const pDboLimitOfFSUNHTOfHeavyVehicle);

	// FSUNHTOfLightVehicle限值信息<LimitOfFSUNHTOfLightVehicle> LimitOfFSUNHTOfLightVehicle表的所有操作
	// 获取FSUNHTOfLightVehicle限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfFSUNHTOfLightVehicle(const wchar_t wchSql[SQLMAX], PLIMITOFFSUNHTOFLIGHTVEHICLE const pDboLimitOfFSUNHTOfLightVehicle);

	// FSUYD限值信息<LimitOfFSUYD> LimitOfFSUYD表的所有操作
	// 获取FSUYD限值信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLimitOfFSUYD(const wchar_t wchSql[SQLMAX], PLIMITOFFSUYD const pDboLimitOfFSUYD);

	// 检测线信息<LineInfo> LineInfo表的所有操作
	// 获取检测线信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboLineInfo(const wchar_t wchSql[SQLMAX], PLINEINFO const pDboLineInfo);
	// 设置检测线信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboLineInfo(const LINEINFO &dboLineInfo);
	// 设置检测线状态和检测线状态变更时间
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboLineInfoLineState(const wchar_t wchLineNumber[50], const short siLineState, const SYSTEMTIME *pstChangeTime);

	// 检测站信息<StationInfo> StationInfo表的所有操作
	// 获取检测站信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboStationInfo(const wchar_t wchSql[SQLMAX], PSTATIONINFO const pDboStationInfo);
	// 设置检测站信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboStationInfo(const STATIONINFO& dboStationInfo);

	// 排放检测日志信息<TestLog> TestLog表的所有操作
	// 获取检测日志信息
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestLog(const wchar_t wchSql[SQLMAX], PTESTLOG const pDboTestLog);
	// 设置检测日志信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestLog(const TESTLOG &dboTestLog);
	// 设置检测日志信息中的是否传送字段
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestLogIsSent(const wchar_t wchRunningNumber[50], const bool bIsSent=false);
	// 设置检测日志信息中的是否传送环保数据字段
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestLogIsSentEP(const wchar_t wchRunningNumber[50], const bool bIsSentEP=false);
	// 设置检测日志中的检测状态，开始处理时间，检测线编号
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestLogTestState(const wchar_t wchRunningNumber[50], const short siTestState,const wchar_t wchLineNumber[50] = NULL, const SYSTEMTIME *pstProcessingTime = NULL);

	// DIS检测数据<ResultOfDIS> ResultOfDIS表的所有操作
	// 获取DIS检测结果
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfDIS(const wchar_t wchSql[SQLMAX], PRESULTOFDIS const pDboResultOfDIS);
	DWORD GetDboResultOfDIS_Zip(const wchar_t wchSQL[SQLMAX], SResultOfDIS* const pResultOfDIS);
	// 设置DIS检测结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfDIS(const RESULTOFDIS &dboResultOfDIS);
	DWORD SetDboResultOfDIS_Zip(const SResultOfDIS &sResultOfDIS);

	// DIS real time data<RealTimeDataOfDIS> RealTimeDataOfDIS表的所有操作
	// 获取DIS实时数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfDIS(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	DWORD GetDboRealTimeDataOfDIS_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	// 设置DIS实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboRealTimeDataOfDIS(list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	DWORD SetDboRealTimeDataOfDIS_Zip(list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	// ASM result data<ResultOfASM> ResultOfASM表的所有操作
	// 获取ASM结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfASM(const wchar_t wchSql[SQLMAX], PRESULTOFASM const pDboResultOfASM);
	DWORD GetDboResultOfASM_Zip(const wchar_t wchSQL[SQLMAX], SResultOfASM* const pResultOfASM);
	// 设置ASM结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfASM(const RESULTOFASM &dboResultOfASM);
	DWORD SetDboResultOfASM_Zip(const SResultOfASM &sResultOfASM);

	// ASM real time data<RealTimeDataOfASM> RealTimeDataOfASM表的所有操作
	// 获取ASM实时数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfASM(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);
	DWORD GetDboRealTimeDataOfASM_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);
	// 设置ASM实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboRealTimeDataOfASM(list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);
	DWORD SetDboRealTimeDataOfASM_Zip(list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);

	// VMAS result data<ResultOfVMAS> ResultOfVMAS表的所有操作
	// 获取VMAS结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfVMAS(const wchar_t wchSql[SQLMAX], PRESULTOFVMAS const pDboResultOfVMAS);
	DWORD GetDboResultOfVMAS_Zip(const wchar_t wchSQL[SQLMAX], SResultOfVMAS* const pResultOfVMAS);
	// 设置VMAS结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfVMAS(const RESULTOFVMAS &dboResultOfVMAS);
	DWORD SetDboResultOfVMAS_Zip(const SResultOfVMAS &sResultOfVMAS);

	// VMAS real time data<RealTimeDataOfVMAS> RealTimeDataOfVMAS表的所有操作
	// 获取VMAS实时数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfVMAS(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	DWORD GetDboRealTimeDataOfVMAS_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	// 设置VMAS实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboRealTimeDataOfVMAS(list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	DWORD SetDboRealTimeDataOfVMAS_Zip(list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	// LUGDOWN result data<ResultOfLUGDOWN> ResultOfLUGDOWN表的所有操作
	// 获取LUGDOWN结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfLUGDOWN(const wchar_t wchSql[SQLMAX], PRESULTOFLUGDOWN const pDboResultOfLUGDOWN);
	DWORD GetDboResultOfLUGDOWN_Zip(const wchar_t wchSQL[SQLMAX], SResultOfLUGDOWN* const pResultOfLUGDOWN);
	// 设置LUGDOWN结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfLUGDOWN(const RESULTOFLUGDOWN &dboResultOfLUGDOWN);
	DWORD SetDboResultOfLUGDOWN_Zip(const SResultOfLUGDOWN &sResultOfLUGDOWN);

	// LUGDOWN real time data<RealTimeDataOfLUGDOWN> RealTimeDataOfLUGDOWN表的所有操作
	// 获取LUGDOWN实时数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfLUGDOWN(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	DWORD GetDboRealTimeDataOfLUGDOWN_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	// 设置LUGDOWN实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboRealTimeDataOfLUGDOWN(list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	DWORD SetDboRealTimeDataOfLUGDOWN_Zip(list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	// FSUNHT检测数据<ResultOfFSUNHT> ResultOfFSUNHT表的所有操作
	// 获取FSUNHT结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfFSUNHT(const wchar_t wchSql[SQLMAX], PRESULTOFFSUNHT const pDboResultOfFSUNHT);
	DWORD GetDboResultOfFSUNHT_Zip(const wchar_t wchSQL[SQLMAX], SResultOfFSUNHT* const pResultOfFSUNHT);
	// 设置FSUNHT结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfFSUNHT(const RESULTOFFSUNHT &dboResultOfFSUNHT);
	DWORD SetDboResultOfFSUNHT_Zip(const SResultOfFSUNHT &sResultOfFSUNHT);


	// FSUNHT real time data<RealTimeDataOfFSUNHT> RealTimeDataOfFSUNHT表的所有操作
	// 获取FSUNHT实时数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfFSUNHT(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);
	DWORD GetDboRealTimeDataOfFSUNHT_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);
	// 设置FSUNHT实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboRealTimeDataOfFSUNHT(list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);
	DWORD SetDboRealTimeDataOfFSUNHT_Zip(list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);

	// FSUYD检测数据<ResultOfFSUYD> ResultOfFSUYD表的所有操作
	// 获取FSUYD结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfFSUYD(const wchar_t wchSql[SQLMAX], PRESULTOFFSUYD const pDboResultOfFSUYD);
	// 设置FSUYD结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfFSUYD(const RESULTOFFSUYD &dboResultOfFSUYD);

	// AVFSU检测数据<ResultOfAVFSU> ResultOfAVFSU表的所有操作
	// 获取AVFSU结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfAVFSU(const wchar_t wchSql[SQLMAX], PRESULTOFAVFSU const pDboResultOfAVFSU);
	// 设置AVFSU结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfAVFSU(const RESULTOFAVFSU &dboResultOfAVFSU);

	// MI检测数据<ResultOfMI> ResultOfMI表的所有操作
	// 获取MI结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfMI(const wchar_t wchSql[SQLMAX], PRESULTOFMI const pDboResultOfMI);
	// 设置MI结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfMI(const RESULTOFMI &dboResultOfMI);

	// MI real time data<RealTimeDataOfMI> RealTimeDataOfMI表的所有操作
	// 设置MI实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfMI(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	DWORD GetDboRealTimeDataOfMI_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	DWORD SetDboRealTimeDataOfMI(list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	DWORD SetDboRealTimeDataOfMI_Zip(list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	// MDIS检测数据<ResultOfMDIS> ResultOfMDIS表的所有操作
	// 获取MDIS结果数据
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboResultOfMDIS(const wchar_t wchSql[SQLMAX], PRESULTOFMDIS const pDboResultOfMDIS);
	// 设置MDIS结果数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboResultOfMDIS(const RESULTOFMDIS &dboResultOfMDIS);

	// MDIS real time data<RealTimeDataOfMI> RealTimeDataOfMDIS表的所有操作
	// 设置MDIS实时数据
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD GetDboRealTimeDataOfMDIS(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	DWORD GetDboRealTimeDataOfMDIS_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	DWORD SetDboRealTimeDataOfMDIS(list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	DWORD SetDboRealTimeDataOfMDIS_Zip(list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	// 设备校准检查信息<EquCalChkInfo> EquCalChkInfo表的所有操作
	DWORD SetDboEquCalChkInfo(const EQUCALCHKINFO &dboEquCalChkInfo);

	// 检测线认证信息<LineInfo>
	// return value
	// 0xff : Database exception
	// 0x00 : No record get
	// 0x01 : Get first record successful
	// 0x02 : Get CertificateAuthentication failed
	DWORD GetDboCertificateOfLineInfo(wchar_t const * pwchLineNumber, BYTE * const pbCertificateAuthentication, const UINT uiCALength, long& lDataSize);
	// return value
	// 0xff : Database exception
	// 0x00 : Update successful
	// 0x01 : LineNumber error
	DWORD SetDboCertificateOfLineInfo(wchar_t const * pwchLineNumber, BYTE * const pbCertificateAuthentication, const UINT uiCALength);

	// 系统参数<SystemParameters>
	// 获取系统参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboSystemParameters(const wchar_t wchSql[SQLMAX], PSYSTEMPARAMETERS const pDboSystemParameters);
	// 设置系统参数
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboSystemParameters(const SYSTEMPARAMETERS &dboSystemParameters);

	// ASM检测参数<TestParametersOfASM>
	// 获取ASM检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfASM(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFASM const pDboTestParametersOfASM);

	// VMAS检测参数<TestParametersOfVMAS>
	// 获取VMAS检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfVMAS(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFVMAS const pDboTestParametersOfVMAS);

	// 重型LUGDOWN检测参数<TestParametersOfLUGDOWNHEAVY>
	// 获取LUGDOWNHEAVY检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfLUGDOWNHEAVY(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFLUGDOWNHEAVY const pDboTestParametersOfLUGDOWNHEAVY);

	// 轻型LUGDOWN检测参数<TestParametersOfLUGDOWNLIGHT>
	// 获取LUGDOWNLIGHT检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfLUGDOWNLIGHT(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFLUGDOWNLIGHT const pDboTestParametersOfLUGDOWNLIGHT);

	// 双怠速检测参数<TestParametersOfDIS>
	// 获取DIS检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfDIS(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFDIS const pDboTestParametersOfDIS);
	// 设置DIS检测参数
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestParametersOfDIS(const TESTPARAMETERSOFDIS &dboTestParametersOfDIS);

	// 不透光自由加速检测参数<TestParametersOfFSUNHT>
	// 获取FSUNHT检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfFSUNHT(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFFSUNHT const pDboTestParametersOfFSUNHT);
	// 设置FSUNHT检测参数
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestParametersOfFSUNHT(const TESTPARAMETERSOFFSUNHT &dboTestParametersOfFSUNHT);

	// 滤纸式自由加速检测参数<TestParametersOfFSUYD>
	// 获取FSUYD检测参数
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestParametersOfFSUYD(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFFSUYD const pDboTestParametersOfFSUYD);
	// 设置FSUYD检测参数
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboTestParametersOfFSUYD(const TESTPARAMETERSOFFSUYD &dboTestParametersOfFSUYD);

	// 标定日志<DemarcationLog>
	// 设置标定日志信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationLog(const DEMARCATIONLOG &dboDemarcationLog);

	// 分析仪气体校准结果<DemarcationResultOfAnaGasCal>
	// 设置分析仪气体校准结果信息
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfAnaGasCal(const DEMARCATIONRESULTOFANAGASCAL &dboResultOfAnaGasCal);

	// 分析仪气体检查结果<DemarcationResultOfAnaGasChk>
	// 设置分析仪气体检查结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfAnaGasChk(const DEMARCATIONRESULTOFANAGASCHK &dboResultOfAnaGasChk);

	// 不透光度计校准检查结果<DemarcationResultOfOpaCalChk>
	// 设置不透光度计校准检查结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfOpaCalChk(const DEMARCATIONRESULTOFOPACALCHK &dboResultOfOpaCalChk);
	
	// 滤纸式烟度计检查结果<DemarcationResultOfSmoChk>
	// 设置滤纸式烟度计检查结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfSmoChk(const DEMARCATIONRESULTOFSMOCHK &dboResultOfSmoChk);
	
	// 测功机加载滑行结果<DemarcationResultOfDynConstLoad>
	// 设置测功机加载滑行结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfDynConstLoad(const DEMARCATIONRESULTOFDYNCONSTLOAD &dboResultOfDynConstLoad);

	// 测功机力校准结果<DemarcationResultOfDynForceCal>
	// 设置测功机力校准结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfDynForceCal(const DEMARCATIONRESULTOFDYNFORCECAL &dboResultOfDynForceCal);
	
	// 测功机力检查结果<DemarcationResultOfDynForceChk>
	// 设置测功机力检查结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfDynForceChk(const DEMARCATIONRESULTOFDYNFORCECHK &dboResultOfDynForceChk);

	// 测功机寄生功率滑行结果<DemarcationResultOfDynPLHP>
	// 设置测功机寄生功率滑行结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfDynPLHP(const DEMARCATIONRESULTOFDYNPLHP &dboResultOfDynPLHP);
	
	// 流量计流量校准结果<DemarcationResultOfFloFluxCal>
	// 设置流量计流量校准结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfFloFluxCal(const DemarcationResultOfFloFluxCal &dboResultOfFloFluxCal);

	// 流量计流量检查结果<DemarcationResultOfFloFluxChk>
	// 设置流量计流量检查结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfFloFluxChk(const DemarcationResultOfFloFluxChk &dboResultOfFloFluxChk);

	// 流量计稀释氧校准结果<DemarcationResultOfFloDilO2Cal>
	// 设置流量计稀释氧校准结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfFloDilO2Cal(const DemarcationResultOfFloDilO2Cal &dboResultOfFloDilO2Cal);

	// 流量计压力校准结果<DemarcationResultOfFloPresCal>
	// 设置流量计压力校准结果
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDemarcationResultOfFloPresCal(const DemarcationResultOfFloPresCal &dboResultOfFloPresCal);

	// 检测图片<TestPhoto>
	// 获取检测图片
	// 返回：
	// 0x01：获取成功
	// 0x02：获取失败
	// 0xff：数据库异常
	DWORD GetDboTestPhoto(const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, UINT *const puiActualLengthInBytes, const BYTE bPhotoNumber = 5);
	DWORD GetDboTestPhoto(const wchar_t *const pwchRunningNumber, const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, UINT *const puiActualLengthInBytes, const BYTE bPhotoNumber = 5);
	// 写图片数据入数据库
	// 返回：
	// 0x00：写入成功
	// 0xff：数据库异常
	DWORD SetDboTestPhoto(const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, const BYTE bPhotoNumber = 5);
	DWORD SetDboTestPhoto(const wchar_t *const pwchRunningNumber, const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, const BYTE bPhotoNumber = 5);
	// 更新图片
	// 返回：
	// 0x00：更新成功
	// 0xff：数据库异常
	DWORD UpdateTestPhoto(const wchar_t *const pwchReportNumber, const wchar_t *const pwchFieldName, const LPSYSTEMTIME lpst);
	DWORD UpdateTestPhoto(const wchar_t *const pwchRunningNumber, const wchar_t *const pwchReportNumber, const wchar_t *const pwchFieldName, const LPSYSTEMTIME lpst);

	// 读检测数量<TestAmount>
	// 如果没有检测线编号和日期对应的记录，将会自动创建一条对应记录，并将数量置为0
	// (in)pwchLineNumber,检测线编号
	// (in)st,日期时间
	// (in)nAmount,数量变量引用
	// (reutrn)
	// 0x00:正常
	// 非0x00:异常
	DWORD GetDboTestAmount(const wchar_t * const pwchLineNumber, const SYSTEMTIME &st, PTESTAMOUNT const pDboTestAmount);
	
	// 写检测数量<TestAmount>
	// (in)nAmount,数量变量
	// (reutrn)
	// 0x00:正常
	// 非0x00:异常
	DWORD SetDboTestAmount(const wchar_t * const pwchLineNumber, const SYSTEMTIME &st, const TESTAMOUNT &dboTestAmount);

	// 读检测数量<StationTestAmount>
	// 返回：
	// 0x00:正常
	// 0xff:异常
	DWORD GetDboStationTestAmount(const SYSTEMTIME &st, PTESTAMOUNT const pDboStationTestAmount);
	
	// 写检测数量<StationTestAmount>
	// 返回：
	// 0x00:正常
	// 非0x00:异常
	DWORD SetDboStationTestAmount(const SYSTEMTIME &st, const TESTAMOUNT &dboStationTestAmount);

	// 删除历史数据
	// (in)st：要删除的历史数据的时间点，不包括该时间点
	// 返回：
	// 0x00:正常
	// 非0x00:异常
	DWORD DelDboHistoryData(const SYSTEMTIME &st);

	// 读白名单<T1>
	// (in)pwchPlateNumber,车牌号码
	// (in)pwchPlateType,车牌类型
	// (reutrn)
	// 0x00:正常 在白名单里
	// 非0x00:异常 不在白名单里
	DWORD GetDboWhiteList(const wchar_t * const pwchPlateNumber, const wchar_t * const pwchPlateType);

	// 读重复性<T2>
	// (in)pwchPlateNumber,车牌号码
	// (in)pwchPlateType,车牌类型
	// (in)pwchTestType,检测类型
	// (out)pwchReportNumber,报告编号
	// (reutrn)
	// 0x00:正常 在重复性里
	// 非0x00:异常 不在重复性里
	DWORD GetDboRepeat(const wchar_t * const pwchPlateNumber, const wchar_t * const pwchPlateType, const wchar_t * const pwchTestType, wchar_t * const pwchReportNumber);
	DWORD GetDboRepeat(const wchar_t * const pwchPlateNumber, const wchar_t * const pwchPlateType, const wchar_t * const pwchTestType, wchar_t * const pwchRunningNumber, wchar_t * const pwchReportNumber);


	//写日常运行日志
	//strMessage:日志时间描述
	//Operator:用户
	//OpTime:操作时间
	//Work:操作项目 1是日常运行操作,2是仪器校准,3是仪器自检
	//Pass:是否通过 1是通过,0是不通过
	// 返回：
	// 0x00:正常
	// 非0x00:异常
	DWORD SetDboRunningLog(const wchar_t * strMessage,wchar_t * Operator,const wchar_t * OpTime,wchar_t * Work,wchar_t * Pass);

	// 系统日志<SystemLog> SystemLog表的所有操作
	//reason: 1:运行记录 2:校准记录 3:故障记录 4:维修记录
	DWORD SetDboSystemLog(const SYSTEMLOG &dboSystemLog);

	//锁止检测线
	//bLock:true时,锁止检测线
	//bLock:false时,解锁检测线
	 DWORD SetDboLineLock(const wchar_t wchLineNumber[50], bool bLock=false);
	 // 删除RealTimeData
	// 返回：
	// 0x00：删除成功
	// 0xff：数据库异常
	DWORD DelDboRealTimeData(const wchar_t wchSql[SQLMAX]);

	// 依据ReportNumber查找TestLog中的RunningNumber
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestLogRunningNumber(const wchar_t * const pwchReportNumber, wchar_t * const pwchRunningNumber);

	// 依据RunningNumber查找TestLog中的ReportNumber
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboTestLogReportNumber(const wchar_t * const pwchRunningNumber, wchar_t * const pwchReportNumber);

	// 数据库版本<DBVersion>
	// 获取数据库版本
	// 返回：
	// 0x00：无对应记录
	// 0x01：获取成功
	// 0xff：数据库异常
	DWORD GetDboDBVersion(SDBVersion& sDBVersion);
	// 设置数据库版本
	// 返回：
	// 0x00：设置成功
	// 0xff：数据库异常
	DWORD SetDboDBVersion(const SDBVersion& sDBVersion);
	//设置数字温度计数据
	DWORD SetDboRealTimeDataOfDIM(const REALTIMEDATAOFDIM &sRealTimeDataOfDIM);
	//设置数字温度计日志
	DWORD SetDboLogOfDIM(const LOGOFDIM &sLogOfDIM);
	//获取数字温度计数据
	DWORD GetDboRealTimeDataOfDIM(const wchar_t wchSql[SQLMAX], PREALTIMEDATAOFDIM const pRealTimeDataOfDIM);
	//获取数字温度计日志
	DWORD GetDboLogOfDIM(const wchar_t wchSql[SQLMAX], PLOGOFDIM const pLogOfDIM);
	//删除数字温度计数据
	DWORD DelDboRealTimeDataOfDIM(const wchar_t wchSql[SQLMAX]);
	//删除数字温度计日志
	DWORD DelDboLogOfDIM(const wchar_t wchSql[SQLMAX]);
	//同步服务器时间到本机
	DWORD SynDBTime(void);
	DWORD SynDBTimeWithMilliseconds(void);
	//获取环境参数
	//返回：0x00 无数据记录;0x01 有数据记录;0xff 错误
	DWORD GetEPDataFromDBNew(wchar_t *const pwchEquNum=NULL, 
		float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL, 
		PREALTIMEDATAOFDIM const pRealTimeDataOfDIM=NULL
		);

	//获取OBD检测数据
	DWORD GetDboResultOfOBD(const wchar_t wchSql[SQLMAX], SResultOfOBD* const pResultOfOBD);
	//设置OBD检测数据
	DWORD SetDboResultOfOBD(const SResultOfOBD &sResultOfOBD);
	
	DWORD GetDboResultOfAppearance(const wchar_t wchSql[SQLMAX], SResultOfAppearance* const pResultOfAppearance);
	DWORD SetDboResultOfAppearance(const SResultOfAppearance &sResultOfAppearance);

	//获取燃油蒸发系统检测数据
	DWORD GetDboResultOfEECS(const wchar_t wchSql[SQLMAX], SResultOfEECS* const pResultOfEECS);
	//设置燃油蒸发系统检测数据
	DWORD SetDboResultOfEECS(const SResultOfEECS &sResultOfEECS);

	//获取林格曼烟度结果数据
	DWORD GetDboResultOfRingelmann(const wchar_t wchSql[SQLMAX], SResultOfRingelmann* const pResultOfRingelmann);
	//设置林格曼烟度结果数据
	DWORD SetDboResultOfRingelmann(const SResultOfRingelmann &sResultOfRingelmann);

	// 外检拍照/录像表操作
	DWORD GetDboAppPhotoInfo(const wchar_t wchSql[SQLMAX], std::vector<SAppPhotoInfo> &vtAppPhotoInfo);
	DWORD SetDboAppPhotoInfo(const std::vector<SAppPhotoInfo> &vtAppPhotoInfo);

	// 拍照/录像数据表操作
	DWORD GetDboTestPhotoData(const wchar_t wchSql[SQLMAX], std::vector<STestPhotoData> &vtTestPhotoData);
	DWORD SetDboTestPhotoData(const std::vector<STestPhotoData> &vtTestPhotoData);

	// 获取白名单修改的参数
	DWORD GetDboWhiteModifiedParameters(const wchar_t wchSql[SQLMAX], SWhiteModifiedParameters* const pWhiteModifiedParameters);
	DWORD GetDboWhiteModifiedParameters(const wchar_t wchSql[SQLMAX], std::vector<SWhiteModifiedParameters> & vtWhiteModifiedParameters);
	// 设置白名单修改的参数
	DWORD SetDboWhiteModifiedParameters(const SWhiteModifiedParameters &sWhiteModifiedParameters);

#endif