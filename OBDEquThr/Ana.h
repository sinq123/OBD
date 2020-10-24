/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：Ana.h
* 文件标识：
* 摘要：分析仪抽象类
*		以纯虚拟函数形式定义分析仪操作接口
*
* 当前版本：1.0.3
* 作者：Hyh
* 完成日期：2010-06-21
*
* 历史信息：
*
* 当前版本：1.0.6
* 完成日期：2014-05-10
* 作者：Lihy
* 描述：添加国产流量计高中低三点校准命令
*
* 当前版本：1.0.5
* 完成日期：2011-03-03
* 作者：Ming
* 描述：分别添加设置HCCOCO2,O2,NOx通道延时值的函数,GetxxxDelayTime_Flowmeter(),SetxxxDelayTime_Flowmeter()和xxxDelayTime_Flowmeter()
*
* 当前版本：1.0.4
* 完成日期：2010-08-26
* 作者：Raylee
* 描述：添加GetTestResultData()函数接口，获取检测结果数据
*
* 当前版本：1.0.3
* 完成日期：2010-06-21
* 作者：Hyh
* 描述：1)修正GetProcessDataStatus_CT_Flowmeter函数
*
* 当前版本：1.0.2
* 完成日期：2010-03-17
* 作者：Cui
* 描述：将大气压力校准函数由APCalibrate改为APCalibration
*
* 当前版本：1.0.1
* 完成日期：2009-12-10
* 作者：Cui
* 描述：修改流量计流量、压力、稀释氧校准函数
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#ifndef ANA_H
#define ANA_H

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// 函数执行结果
#define ANA_OPEN_SUCCEED							0x00  // 打开串口成功
#define ANA_OPEN_FAIL								0x01  // 打开串口失败
#define ANA_CLOSE_SUCCEED							0x00  // 关闭串口成功
#define ANA_CLOSE_FAIL_WIN32_API					0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define ANA_CLOSE_FAIL_NO_OPEN_PORT					0x02  // 关闭串口失败：没有可以关闭的串口
#define ANA_WR_SUCCEED								0x00  // 写读成功
#define ANA_WR_PORT_NOT_OPEN						0x01  // 写读失败：串口未打开
#define ANA_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // 写读失败：读指定数据失败
#define ANA_WR_READ_SPECIFIED_DATA_FAIL				0x03  // 写读失败：读指定数据失败
#define ANA_WR_EQUIP_RETURE_0X15					0x04  // 写读失败：仪器返回错误代码（0x15）
#define ANA_WR_CHECK_SUM_FAIL						0x05  // 写读失败：读出数据校验和失败
#define ANA_WR_UNKNOWN_ERROR						0xFF  // 写读失败：未知错误

class CAna
{
public:
	CAna(void);
	virtual ~CAna(void);

public:
	// 共享串口句柄
	HANDLE GetComHandle(void);
	void AttachComHandle(HANDLE hCom);
	void DetachComHandle(void);
	// 共享临界区
	CRITICAL_SECTION GetCriticalSection(void);
	void AttachCriticalSection(CRITICAL_SECTION cs);
	void DetachCriticalSection(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// ANA_CLOSE_SUCCEED
	// ANA_CLOSE_FAIL_WIN32API
	// ANA_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// 判断串口是或打开
	virtual bool IsOpen(void) const = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作start 
	// 1取主数据
	// (out)psHC,HC浓度指针,单位:ppm,可以为空指针
	// (out)pfCO,CO浓度指针,单位:%,可以为空指针
	// (out)pfCO2,CO2浓度指针,单位:%,可以为空指针
	// (out)pfO2,O2浓度指针,单位:%,可以为空指针
	// (out)pfNO,NO浓度指针,单位:ppm,可以为空指针
	// (out)pusRpm,转速,单位:r/min,可以为空指针
	// (out)pfOilTemp,油温,单位:摄氏度,可以为空指针
	// (out)pusFlux,流量当量,单位:未知,可以为空指针
	// (out)pfLambda,过量空气系数,单位:无,可以为空指针
	// (out)pbO2Sensor,氧传感器是否失效,单位:无,可以为空指针
	// (out)pbNOSensor,NO传感器是否失效,单位:无,可以为空指针
	// (out)pbWaterFull,判断水位是否已满,单位:无（新）
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMajorData(short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL,
		short *const psNO2=NULL,
		short *const psNOx=NULL,
		USHORT *const pusRpm=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusFlux=NULL, 
		float *const pfLambda=NULL,
		bool *const pbO2Sensor=NULL,
		bool *const pbNOSensor=NULL) = 0;

	virtual DWORD GetMajorData(short *const psHC, 
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
		bool *const pbWaterFull) = 0;

	virtual DWORD GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx) = 0;

	// 2取辅助数据
	// 其中ET:Environment Temperature(环境温度)缩写
	// 其中AP:Atmospheric Pressure(大气压力)缩写
	// 其中:RH:Relative Humidity(相对湿度)缩写
	// (out)pfET:环境温度指针,单位:摄氏度,不能为空指针
	// (out)pfAP:大气压力指针,单位:kPa,不能为空指针
	// (out)pfRH:相对湿度指针,单位:%,不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMinorData(float *const pfET, float *const pfAP, float *const pfRH) = 0;

	// 3抽空气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DeflateEnvAir(BYTE *const pbResult = NULL) = 0;

	// 4抽管道气(开泵,气样从探头进入)
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DeflatePipeAir(BYTE *const pbResult = NULL) = 0;

	// 5清洗管道(反吹)
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD InverseBlow(BYTE *const pbResult = NULL) = 0;

	// 6停止清洗(停止反吹及停泵)
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD StopPump(BYTE *const pbResult = NULL) = 0;

	// 7检漏
	// (out)pbResult,仪器执行结果指针,0x00-无泄露,0x01-有泄露,0x05-正在检漏,可以为空指针(兼容分析仪通用协议,也可能返回0x06)
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD CheckLeak(BYTE *const pbResult = NULL) = 0;

	// 8调零
	// (out)pbResult,仪器执行结果指针,0x00-调零成功,0x01-调零失败,0x05-正在调零,可以为空指针(兼容分析仪通用协议,也可能返回0x06)
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD Zero(BYTE *const pbResult = NULL) = 0;

	// 9写入校准气浓度
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败(校验和错误),不能为空指针
	// (in)usCalC3H8,C3H8校准值,单位:ppm
	// (in)fCalCO,CO校准值,单位:%
	// (in)fCalCO2,CO2校准值,单位:%
	// (in)usCalNO,NO校准值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)  = 0;
	
	// 10校准
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,0x05-正在校准,不能为空指针
	// (out)psHC,HC值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO,CO值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO2,CO2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfO2,O2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)psNO,NO值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfPef,Pef值指针,单位:无,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GasCalibration(BYTE *const pbResult, const int nChanel, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL,  
		short *const psNO2=NULL,
		float *const pfPef=NULL) = 0;

	// 写入NO2校准气浓度
	// (in)usNO2,NO2校准值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2)  = 0;

	// 校准NO2
	// (out)psNO2,NO2值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD NO2Calibration(BYTE *const pbResult, short *const psNO2=NULL) = 0;

	// 11写入检查气浓度
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败(校验和错误),不能为空指针
	// (in)uChkC3H8:C3H8检查值,单位:ppm
	// (in)fChkCO:CO检查值,单位:%
	// (in)fChkCO2:CO2检查值,单位:%
	// (in)usChkNO:NO检查值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO) = 0;

	// 12自动检查
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,0x05-正在检查,不能为空指针
	// (out)psHC,HC值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO,CO值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO2,CO2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfO2,O2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)psNO,NO值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfPef,Pef值指针,单位:无,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GasCheck(BYTE *const pbResult, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		float *const pfPef=NULL) = 0;

	// 13取PEF
	// (out)pfPef,Pef值指针，单位：无，不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetPef(float *const pfPef) = 0;

	// 14清除NO老化标志
	// 503特有
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD ClearNoSign(BYTE *const pbResult = NULL) = 0;

	// 15设置调零气体为空气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetZeroAirTypeToEnvAir(BYTE *const pbResult = NULL) = 0;

	// 16设置调零气体为零气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetZeroAirTypeToZeroAir(BYTE *const pbResult = NULL) = 0;

	// 17设置冲程为4
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL) = 0;

	// 18设置冲程为2
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL) = 0;

	// 19设置点火方式为单次
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) = 0;

	// 20设置点火方式为二次
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL) = 0;

	// 21允许自动调零
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD EnableAutoZero(BYTE *const pbResult = NULL) = 0;

	// 22禁止自动调零
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DisableAutoZero(BYTE *const pbResult = NULL) = 0;

	// 23允许键盘操作

	// 24禁止键盘操作

	// 25LCD开

	// 26LCD关

	// 27环境温度校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,不能为空指针
	// (in)fCalET:环境温度校准值,单位:摄氏度
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD ETCalibration(BYTE *const pbResult, const float fCalET) = 0;

	// 28室温校准

	// 29油温校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,不能为空指针
	// (in)fCalOT:油温校准值,单位:摄氏度
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD OTCalibration(BYTE *const pbResult, const float fCalOT) = 0;

	// 30环境湿度校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,不能为空指针
	// (in)fCalRH:相对湿度校准值,单位-%
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD RHCalibration(BYTE *const pbResult, const float fCalRH) = 0;

	// 31大气压力校准
	// 返回函数执行结果：
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,不能为空指针
	// (in)fCalAP:大气压力校准值,单位:kPa
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD APCalibration(BYTE *const pbResult, const float fCalAP) = 0;

	// 32检查气开
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD OpenCheckGas(BYTE *const pbResult = NULL) = 0;

	// 33检查气关
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD CloseCheckGas(BYTE *const pbResult = NULL) = 0;

	// 34取仪器状态
	// (out)pbStatus,仪器状态指针,0x00-就绪(未开泵),0x01-正在检测(已开泵),0x02-正在预热,0x03-正在检漏,等等(具体状态码含义需要查阅通讯协议),不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetStatus(BYTE *const pbStatus) = 0;

	// 35取预热剩余时间
	// (out)pusSecondsLeft:预热剩余时间指针,应答[AA][BB],剩余时间=[AA]*256+[BB]秒;不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetWarmUpTimeLeft(USHORT *const pusSecondsLeft) = 0;

	// 36设置燃料类型为汽油
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToGas(BYTE *const pbResult = NULL) = 0;

	// 37设置燃料类型为液化石油气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToLPG(BYTE *const pbResult = NULL) = 0;

	// 38设置燃料类型为天然气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToNG(BYTE *const pbResult = NULL) = 0;

	// 39设置燃料类型为乙醇汽油
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToGasohol(BYTE *const pbResult = NULL) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作end

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 操作模式相关start
public:
	// 操作模式
	enum OperationMode
	{
		// 分析仪通讯模式
		ToAnalyzer,
		// 流量计通讯模式
		ToFlowmeter
	};

public:
	//// 获取当前使用的操作模式
	//virtual DWORD GetOperationModeUsedNow(void) const = 0;

	// 操作模式转换
	// (in)op:操作模式,ToAnalyzer-分析仪通讯模式,ToFlowmeter-流量计通讯模式
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetOperationMode(const enum OperationMode op, BYTE *const pbResult=NULL) = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 操作模式相关stop

public:
	// 流量校准模式
	enum FluxCalibrationMode
	{
		// 低流量校准
		LowFlowCal,
		// 高流量校准
		HighFlowCal,
		// 单点流量校准
		SinglePointFlowCal
	};

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 与流量计联用时需要用到的操作start

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行状态begin
	// 取命令执行状态(判断流量计命令是否已经执行完毕)
	// 主要用于判断稀释氧校准等需要等待的命令是否已经执行完毕
	// (out)pbResult：仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	// 一般只会返回0x06，因为这是即时命令
	// (out)pwProcessStatus：命令执行状态指针，0-无命令正在执行，非0-有命令正在执行
	// (out)pwProcess：处于执行状态的命令字指针
	// (return)
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetProcessStatus_Flowmeter(BYTE *const pbResult=NULL, WORD *const pwProcessStatus=NULL, WORD *const pwProcess=NULL) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行状态end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////收集数据相关begin
	// 开始收集数据
	// (int)wCollectionTime:收集时间,单位:s(秒)。VMAS有限制，最大只能是600秒。
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD StartCollectData_Flowmeter(const WORD wCollectionTime=195, WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL) = 0; 

	// 停止收集数据
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD StopCollectData_Flowmeter(WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL) = 0;  

	// 读过程数据和状态(质量模式)
	// (in)wCntStart:过程数据开始序号，只能是1~600
	// (in)wCntLength:过程数据长度，只能是1~600
	// (out)pfHCwt:HC质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfCOwt:CO质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfCO2wt:CO2质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfNOwt:NO质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL) = 0;

	// 读过程数据和状态(浓度模式) 
	// (in)wCntStart:过程数据开始序号，只能是1~600
	// (in)wCntLength:过程数据长度，只能是1~600
	// (out)psHCct:HC浓度指针,单位:ppm。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psCOct:CO浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psCO2ct:CO2浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psNOct:NO浓度指针,单位:ppm。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfDilO2:稀释氧浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfBenchO2:分析仪氧浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psGasTemp;稀释气体温度,单位:摄氏度。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasFluxStd0:稀释气体0℃标准流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasFluxStd20:稀释气体20℃标准流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfExhaustFluxStd0:废气0℃流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfExhaustFluxStd20:废气20℃流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasPres:稀释气体压力,单位:kPa。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasFluxAct:稀释气体实际流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pusRpm:发动机转速,单位:r/min。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct=NULL, 
		float *const pfCOct=NULL, 
		float *const pfCO2ct=NULL, 
		short *const psNOct=NULL, 
		float *const pfDilO2=NULL, 
		float *const pfBenchO2=NULL, 
		short *const psGasTemp=NULL, 
		float *const pfGasFluxStd0=NULL, 
		float *const pfGasFluxStd20=NULL, 
		float *const pfExhaustFluxStd0=NULL, 
		float *const pfExhaustFluxStd20=NULL, 
		float *const pfGasPres=NULL, 
		float *const pfGasFluxAct=NULL, 
		USHORT *const pusRpm=NULL,
		WORD *const pwMiniStatus=NULL,
		const BYTE bDataMode=0x01,
		BYTE *const pbResult=NULL) = 0;

	// 获取总克数(mg) 
	// (out)pusHCwt,HC总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pusCOwt,CO总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pusCO2wt,CO2总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pusNOwt,NO总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pfExhaustVolume:废气体积,单位:L。不能为NULL
	// (out)pfGasVolume:稀释气体体积,单位:L。不能为NULL
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL) = 0;

	// 获取已经收集数据的数量
	// (out)pwCorrectedDataNumber:已经收集数据个数指针；如果函数执行错误，则返回0x00，不能为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult=NULL) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////收集数据相关end


	// 获取流量计实时数据
	// pfFluxStd0:0℃标准流量，L/s
	// pfFluxStd20:20℃标准流量，L/s
	// pfFluxAct:实际流量，L/s
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL) = 0;

	// 获取流量计全部实时数据
	// pfFluxStd0:0℃标准流量，L/s
	// pfFluxStd20:20℃标准流量，L/s
	// pfFluxAct:实际流量，L/s
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL) = 0;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量校准相关begin
	// TODO:需要继续完善
	// 设置标准流量
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux) = 0;

	// TODO:需要继续完善
	// 流量校准 
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量校准相关end


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////压力校准相关begin
	// TODO:需要继续完善
	// 设置标准压力
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres) = 0;

	// TODO:需要继续完善
	// 压力校准 
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////压力校准相关end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////温度校准相关begin
	// TODO:需要继续完善
	// 设置标准温度
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp) = 0;

	// TODO:需要继续完善
	// 温度校准 
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////温度校准相关end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////稀释氧校准相关begin
	// TODO:需要继续完善
	// 设置标准氧气
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2) = 0;

	// 氧气校准
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;

	virtual DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2) = 0;
	virtual DWORD DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;
	virtual DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2) = 0;
	virtual DWORD DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;
	virtual DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2) = 0;
	virtual DWORD DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////稀释氧校准相关end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通道延时校准相关begin
	// 设置HC/CO/CO2通道延时值
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime) = 0;
	virtual DWORD SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime) = 0;
	virtual DWORD HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;

	// 设置O2通道延时值
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime) = 0;
	virtual DWORD SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime) = 0;
	virtual DWORD O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;

	// 设置NOx通道延时值
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime) = 0;
	virtual DWORD SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime) = 0;
	virtual DWORD NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通道延时校准相关end

	// TODO:需要继续完善
	// 开始采集环境氧
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SatrtGetEnvironmentO2_Flowmeter(void) = 0;
	virtual DWORD GetEnvironmentO2_Flowmeter(BYTE *const pbResult,float *const pfEnvO2) = 0;
  
	// 开关风机
	// (in)bRun:开关风机参数；true-开风机，false-关风机
	// (out)pbResult:仪器操作结果指针；0x06-设置模式成功，0x05-设置模式失败(流量计忙)， 0x15-设置模式失败(错误命令)
	//               如果函数执行过程错误，返回0xFF
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL) = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 与流量计联用时需要用到的操作end

	// 获取手动检测的数据
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetTestResultData(short *const psHCHigh=NULL, 
		float *const pfCOHigh=NULL, 
		float *const pfCO2High=NULL, 
		float *const pfO2High=NULL, 
		short *const psNOHigh=NULL,
		USHORT *const pusRpmHigh=NULL, 
		float *const pfOilTempHigh=NULL, 
		float *const pfLambdaHigh=NULL,
		short *const psHCLow=NULL, 
		float *const pfCOLow=NULL, 
		float *const pfCO2Low=NULL, 
		float *const pfO2Low=NULL, 
		short *const psNOLow=NULL,
		USHORT *const pusRpmLow=NULL, 
		float *const pfOilTempLow=NULL, 
		float *const pfLambdaLow=NULL) = 0;

protected:
	// 实际流量转0℃标准流量
	float ActFlux2StdFlux0(const float fActFlux, const float fTemp, const float fPres);
	// 实际流量转20℃标准流量
	float ActFlux2StdFlux20(const float fActFlux, const float fTemp, const float fPres);
	// 20℃标准流量转实际流量
	float StdFlux202ActFlux(const float fStdFlux20, const float fTemp, const float fPres);
	// 20℃标准流量转0℃标准流量
	float StdFlux202StdFlux0(const float fStdFlux20, const float fTemp, const float fPres);
	
	// 写串口数据
	// pbWriteBuffer：要写入的数据内容，不能为空指针
	// dwNumberOfBytesToWrite：要写入的数据长度
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);

	// 读串口数据
	// pbReadBuffer：要读取的数据内容，不能为空指针
	// dwNumberOfBytesToRead：要读取的数据长度
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);

protected:
	// 串口句柄
	HANDLE m_hComm;
	// 临界保护量
	CRITICAL_SECTION cInfoSection;

	// 记录操作模式
	enum OperationMode m_enMode;
};
#endif