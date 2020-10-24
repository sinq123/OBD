/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaNha509Encrypt.h
* 文件标识：
* 摘要：分析仪503V通讯类，继承于抽象类CAna
*       包含VMAS流量计通讯接口
*
* 当前版本：1.0.6
* 作者：Hyh
* 完成日期：2010-07-05
*
* 历史信息：
*
* 当前版本：1.0.9
* 完成日期：2011-03-03
* 作者：Ming
* 描述：分别添加设置HCCOCO2,O2,NOx通道延时值的函数,GetxxxDelayTime_Flowmeter(),SetxxxDelayTime_Flowmeter()和xxxDelayTime_Flowmeter()
*
* 当前版本：1.0.8
* 完成日期：2010-08-26
* 作者：Raylee
* 描述：添加GetTestResultData()函数，获取检测结果数据
*
* 当前版本：1.0.7
* 完成日期：2010-08-26
* 作者：Ming
* 描述：1)将收数据超时时间设为2.5s,原为2s
*
* 当前版本：1.0.6
* 完成日期：2010-07-05
* 作者：Hyh
* 描述：为兼容3.6版本分析仪协议,历史机器取命令返回慢,现将收数据超时时间设为2s,原为1s
*
* 当前版本：1.0.5
* 完成日期：2010-06-21
* 作者：Hyh
* 描述：1)修正GetProcessDataStatus_CT_Flowmeter函数
*
* 当前版本：1.0.4
* 完成日期：2010-03-17
* 作者：Cui
* 描述：将大气压力校准函数由APCalibrate改为APCalibration
*
* 当前版本：1.0.3
* 完成日期：2009-12-21
* 作者：Cui
* 描述：1)修改流量计取环境氧气函数：GetEnvironmentO2_Flowmeter
*       2)添加流量计读校准数据函数：ReadCalibrationData_Flowmeter
*
* 当前版本：1.0.2
* 完成日期：2009-12-10
* 作者：Cui
* 描述：修改流量计流量、压力、稀释氧校准函数
*
* 当前版本：1.0.1
* 完成日期：2009-12-10
* 作者：Cui
* 描述：添加流量计读写函数WriteAndReadData_Flowmeter
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#ifndef NANHUA_NHA509_Encrypt_H
#define NANHUA_NHA509_Encrypt_H

#include "Ana.h"

class CNanhuaNha509Encrypt : public CAna
{
public:
	CNanhuaNha509Encrypt(void);
	~CNanhuaNha509Encrypt(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// NHC_CLOSE_SUCCEED
	// NHC_CLOSE_FAIL_WIN32API
	// NHC_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;
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
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetMajorData(short *const psHC=NULL, 
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
		bool *const pbNOSensor=NULL);

	DWORD GetMajorData(short *const psHC=NULL, 
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
		bool *const pbNOSensor=NULL,
		bool *const pbWaterLevel=NULL);

	DWORD GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx);

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
	DWORD GetMinorData(float *const pfET, float *const pfAP, float *const pfRH);

	// 3抽空气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflateEnvAir(BYTE *const pbResult = NULL);

	// 4抽管道气(开泵,气样从探头进入)
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflatePipeAir(BYTE *const pbResult = NULL);

	// 5清洗管道(反吹)
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD InverseBlow(BYTE *const pbResult = NULL);

	// 6停止清洗(停止反吹及停泵)
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StopPump(BYTE *const pbResult = NULL);

	// 7检漏
	// (out)pbResult,仪器执行结果指针,0x00-无泄露,0x01-有泄露,0x05-正在检漏,可以为空指针(兼容分析仪通用协议,也可能返回0x06)
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CheckLeak(BYTE *const pbResult = NULL);

	// 8调零
	// (out)pbResult,仪器执行结果指针,0x00-调零成功,0x01-调零失败,0x05-正在调零,可以为空指针(兼容分析仪通用协议,也可能返回0x06)
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD Zero(BYTE *const pbResult = NULL);

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
	DWORD SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2);

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
	DWORD GasCalibration(BYTE *const pbResult, const int nChanel, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		short *const psNO2=NULL, 
		float *const pfPef=NULL);
	
	// 写入NO2校准气浓度
	// (in)usNO2,NO2校准值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2);

	// 校准NO2
	// (out)psNO2,NO2值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD NO2Calibration(BYTE *const pbResult, short *const psNO2=NULL);

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
	DWORD SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO);

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
	DWORD GasCheck(BYTE *const pbResult, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		float *const pfPef=NULL);

	// 13取PEF
	// (out)pfPef,Pef值指针，单位：无，不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetPef(float *const pfPef);

	// 14清除NO老化标志
	// 503特有
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ClearNoSign(BYTE *const pbResult = NULL);

	// 15设置调零气体为空气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToEnvAir(BYTE *const pbResult = NULL);

	// 16设置调零气体为零气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToZeroAir(BYTE *const pbResult = NULL);

	// 17设置冲程为4
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL);

	// 18设置冲程为2
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL);

	// 19设置点火方式为单次
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL);

	// 20设置点火方式为二次
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL);

	// 21允许自动调零
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD EnableAutoZero(BYTE *const pbResult = NULL);

	// 22禁止自动调零
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DisableAutoZero(BYTE *const pbResult = NULL);

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
	DWORD ETCalibration(BYTE *const pbResult, const float fCalET);

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
	DWORD OTCalibration(BYTE *const pbResult, const float fCalOT);

	// 30环境湿度校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,不能为空指针
	// (in)fCalRH:相对湿度校准值,单位-%
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD RHCalibration(BYTE *const pbResult, const float fCalRH);

	// 31大气压力校准
	// 返回函数执行结果：
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,不能为空指针
	// (in)fCalAP:大气压力校准值,单位:kPa
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD APCalibration(BYTE *const pbResult, const float fCalAP);

	// 32检查气开
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OpenCheckGas(BYTE *const pbResult = NULL);

	// 33检查气关
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CloseCheckGas(BYTE *const pbResult = NULL);

	// 34取仪器状态
	// (out)pbStatus,仪器状态指针,0x00-就绪(未开泵),0x01-正在检测(已开泵),0x02-正在预热,0x03-正在检漏,等等(具体状态码含义需要查阅通讯协议),不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);

	// 35取预热剩余时间
	// (out)pusSecondsLeft:预热剩余时间指针,应答[AA][BB],剩余时间=[AA]*256+[BB]秒;不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetWarmUpTimeLeft(USHORT *const pusSecondsLeft);

	// 36设置燃料类型为汽油
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToGas(BYTE *const pbResult = NULL);

	// 37设置燃料类型为液化石油气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToLPG(BYTE *const pbResult = NULL);

	// 38设置燃料类型为天然气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToNG(BYTE *const pbResult = NULL);

	// 39设置燃料类型为乙醇汽油
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToGasohol(BYTE *const pbResult = NULL);

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
	DWORD SetOperationMode(const enum OperationMode op, BYTE *const pbResult=NULL);

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
	DWORD GetProcessStatus_Flowmeter(BYTE *const pbResult=NULL, WORD *const pwProcessStatus=NULL, WORD *const pwProcess=NULL);
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
	DWORD StartCollectData_Flowmeter(const WORD wCollectionTime=195, WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL); 

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
	DWORD StopCollectData_Flowmeter(WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL);

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
	DWORD GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL);

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
	DWORD GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
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
		BYTE *const pbResult=NULL);

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
	DWORD GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL);

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
	DWORD GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult=NULL);
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
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);
	
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
	DWORD GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量校准相关begin
	// TODO:需要继续完善
	// 设置标准流量
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux);

	// TODO:需要继续完善
	// 流量校准 
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm);
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
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);

	// TODO:需要继续完善
	// 压力校准 
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////压力校准相关end
	
	//流量计温度校准：注意：进口流量计机芯不支持
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);
	DWORD TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////稀释氧校准相关begin
	// TODO:需要继续完善
	// 设置标准氧气
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2);

	// 氧气校准
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);
	DWORD DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);
	DWORD DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);
	DWORD DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////稀释氧校准相关end
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通道延时校准相关begin
	// 设置HC/CO/CO2通道延时值
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// 设置O2通道延时值
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// 设置NOx通道延时值
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通道延时校准相关end

	// TODO:需要继续完善
	// 开始采集环境氧
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SatrtGetEnvironmentO2_Flowmeter(void);
	DWORD GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2);
  
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
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);

protected:
	// 写读串口数据
	// 写入相应的命令，读取对应的返回数据
	// 并根据需要，分别对写入和读取的数据进行校验和验证
	// (in)pbWriteBuffer：要写入的数据内容指针，可以为空指针，如果为空指针，则不进行写数据操作
	// (in)dwWriteLength：要写入的数据长度（字节数），当pbWriteBuffer为空指针时，这个参数被忽略
	// (out)pbReadBuffer：要读取的数据内容指针，不能为空指针
	// (in)dwReadLength：要读取的数据长度（字节数）
	// (in)bNeedWriteChkSum：true，需要写校验，false：不需要，当pbWriteBuffer为空指针时，这个参数被忽略
	// (in)bNeedReadChkSum：true，需要读校验，false：不需要
	// (in)bPurgeComm,是否重置串口操作,true,重置串口操作(执行PurgeComm函数),false,不重置串口操作(不执行PurgeComm函数)
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *const pbWriteBuffer, 
		const DWORD dwWriteLength, 
		BYTE *const pbReadBuffer, 
		const DWORD dwReadLength, 
		const bool bNeedWriteChkSum=true, 
		const bool bNeedReadChkSum=true, 
		const bool bPurgeComm=true);

	// 计算前n位的校验和
	// 相加，然后取最低字节位
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	BYTE CheckSum(const BYTE *const pb, const UINT un);

	// 只返回06单字节结果的命令格式
	// 这种命令一定也是单字节
	// 绝大部分分析仪命令都是属于这种命令格式
	// (in)bCmd,单字节命令
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult = NULL);

	// 只返回00,01,05这3种单字节结果中的其中一种的命令格式,如果是05,可以带后续数据
	// 调零和检漏命令属于这种命令格式
	// 校准和自动检查命令也属于这种命令格式,而且当返回05时,带有后续数据
	// (in)bCmd,单字节命令
	// (out)pbResult,仪器执行结果指针,0x00-操作成功,0x01-操作失败,0x05-正在操作;如果没有后续数据,则可以为空指针,如果有后续数据,则不能为空指针
	// (out)psHC,HC浓度值指针,单位-ppm
	// (out)pfCO,CO浓度值指针,单位-%
	// (out)pfCO2,CO2浓度值指针,单位-%
	// (out)pfO2,O2浓度值指针,单位-%
	// (out)psNO,NO浓度值指针,单位-ppm
	// (out)pfPef,Pef值指针
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult = NULL);
	DWORD CmdFormat_Return00_01_05(const BYTE bCmd, 
		BYTE *const pbResult, 
		short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		float *const pfPef);

private:
	// 只返回06,00,01这3种单字节结果中的其中一种的命令格式
	// 写入校准气命令和写入检查气命令属于这种命令格式
	// 环境温度,油温,大气压力,相对湿度校准也属于这种命令格式
	// (in)bCmd,单字节命令
	// (out)pbResult,仪器执行结果指针,0x00-操作成功,0x01-操作失败,不能为空指针
	// usC3H8,C3H8浓度值,单位-ppm
	// fCO,CO浓度值,单位-%
	// fCO2,CO2浓度值,单位-%
	// usNO,NO浓度值,单位-ppm
	// fValue:环境温度(或者油温或者大气压力或者相对压力)校准值
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO);
	DWORD CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue);

private:
	// 只返回00,01这2种单字节结果中的其中一种的命令格式,只限于503B协议
	// 写入校准气命令和写入检查气命令属于这种命令格式
	// (in)bCmd,单字节命令
	// (out)pbResult,仪器执行结果指针,0x00-操作成功,0x01-操作失败(校验和失败),不能为空指针
	// usC3H8,C3H8浓度值,单位-ppm
	// fCO,CO浓度值,单位-%
	// fCO2,CO2浓度值,单位-%
	// usNO,NO浓度值,单位-ppm
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO);

	// 只返回00,01,02这3种单字节结果中的其中一种的命令格式,只限于503B协议
	// 环境温度校准,油温校准,大气压力校准,相对湿度校准命令属于这种命令格式
	// (in)bCmd,单字节命令
	// (out)pbResult,仪器执行结果指针,0x00-操作成功,0x01-操作失败,0x02-校验和失败,不能为空指针
	// fValue:环境温度(或者油温或者大气压力或者相对压力)校准值
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue);

protected:
	// 写读串口数据(用于与流量计通讯)
	// 写入相应的命令，读取对应的返回数据
	// 并根据需要，分别对写入和读取的数据进行校验和验证
	// (in)pbWriteBuffer：要写入的数据内容指针，可以为空指针，如果为空指针，则不进行写数据操作
	// (in)dwWriteLength：要写入的数据长度（字节数），当pbWriteBuffer为空指针时，这个参数被忽略
	// (out)pbReadBuffer：要读取的数据内容指针，不能为空指针
	// (in)dwReadLength：要读取的数据长度（字节数）
	// (in)bNeedWriteChkSum：true，需要写校验，false：不需要，当pbWriteBuffer为空指针时，这个参数被忽略
	// (in)bNeedReadChkSum：true，需要读校验，false：不需要
	// (in)bPurgeComm,是否重置串口操作,true,重置串口操作(执行PurgeComm函数),false,不重置串口操作(不执行PurgeComm函数)
	// 返回，函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
		const DWORD dwWriteLength, 
		BYTE *const pbReadBuffer, 
		const DWORD dwReadLength, 
		const bool bNeedWriteChkSum=true, 
		const bool bNeedReadChkSum=true);

	// 计算前n位的校验和
	// 相加，然后取最低字节位
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	WORD CheckSum_Flowmeter(const BYTE *const pb, const UINT un);

	// 读校准数据(Read calibration data)
	// 并根据需要，分别对写入和读取的数据进行校验和验证
	// (out)pbResult：仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)，0x15-失败(错误命令)
	// (in)bCalMode：校准模式，参数含义请参考流量计说明书(其中0x03代表取环境氧)
	// (in)pbReadBuf：读缓冲
	// (in)dwBytesToRead：读取字节数量
	// (return)
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead);
	
	// 获取手动检测的数据
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetTestResultData(short *const psHCHigh=NULL, 
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
		float *const pfLambdaLow=NULL);
	
	DWORD Authentication(void);
	
	UINT m_unTimerID;
	static void CALLBACK TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

protected:
	//// 串口句柄
	//HANDLE m_hComm;
	//// 临界保护量
	//CRITICAL_SECTION cInfoSection;
	//// 目前正在通讯的仪器
	//enum OperationMode m_opUsedNow;
};
#endif