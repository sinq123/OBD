/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：Opa.h
* 文件标识：
* 摘要：不透光度计抽象类
*		以纯虚拟函数形式定义不透光度计操作接口
*
* 当前版本：1.0
* 作者：Cui,hyh
* 完成日期：2009-12-08
*
* 历史信息：
*
* 当前版本：1.0.1
* 完成日期：2010-08-26
* 作者：Raylee
* 描述：添加GetTestResultData()函数，获取检测数据
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#ifndef OPA_H
#define OPA_H

// 函数执行结果
#define OPA_OPEN_SUCCEED						0x00  // 打开串口成功
#define OPA_OPEN_FAIL							0x01  // 打开串口失败
#define OPA_CLOSE_SUCCEED						0x00  // 关闭串口成功
#define OPA_CLOSE_FAIL_WIN32_API				0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define OPA_CLOSE_FAIL_NO_OPEN_PORT				0x02  // 关闭串口失败：没有可以关闭的串口
#define OPA_WR_SUCCEED							0x00  // 写读成功
#define OPA_WR_PORT_NOT_OPEN					0x01  // 写读失败：串口未打开
#define OPA_WR_READ_SPECIFIED_DATA_FAIL			0x02  // 写读失败：读指定数据失败
#define OPA_WR_WRITE_SPECIFIED_DATA_FAIL		0x03  // 写读失败：写指定数据失败
#define OPA_WR_EQUIP_RETURE_0X15				0x04  // 写读失败：仪器返回错误代码（0x15）
#define OPA_WR_CHECK_SUM_FAIL					0x05  // 写读失败：读出数据校验和失败
#define OPA_WR_UNKNOWN_ERROR					0xFF  // 写读失败：未知错误

#include <bitset>

class COpa
{
public:
	COpa(void);
	virtual ~COpa(void);

public:
	// 参数类型
	enum ParamType
	{
		// 发动机冲程
		EngineStroke = 0x01,		
		// 转速测量方式
		RpmMeasurement = 0x02,
		// 滤波参数
		FilterParam = 0x03,
		// 恢复出厂设置
		RestoreFactorySetting = 0x04
	};
	// 冲程类型
	enum EngineStrokeType
	{
		// 4冲程
		FourStroke = 0x01,
		// 2冲程
		TwoStroke = 0x02,
	};
	enum RpmMeasurementType
	{
		// 压电式
		Piezoelectric = 0x01,
		// 光电式
		Photoelectric = 0x02,
		// 电池式
		BatteryType = 0x03,
	};
	// 滤波参数
	enum FilterParamType
	{
		// 滤波模式1
		FilterMode1 = 0x01,
		// 滤波模式2
		FilterMode2 = 0x02,
		// 无滤波
		NoFilter = 0x03
	};
	struct ControlUnitParams
	{
		ParamType pt;
		union ParamValue
		{
			EngineStrokeType est;
			RpmMeasurementType rmt;
			FilterParamType fpt;
			enum {NoParam = 0x00};
		} pv;
	};

public:
	// 共享串口句柄
	HANDLE GetComHandle(void);
	void AttachComHandle(HANDLE hCom);
	void DetachComHandle(void);
	// 共享临界区
	CRITICAL_SECTION GetCriticalSection(void);
	void AttachCriticalSection(CRITICAL_SECTION cs);
	void DetachCriticalSection(void);

	// 已知光吸收系数K，求L(mm)长度通道下的不透光度N
	float K2N(const float fK, const unsigned short usL);

	// 已知L(mm)长度通道下的不透光度N，求光吸收系数K
	float N2K(const float fN, const unsigned short usL);

	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// OPA_OPEN_SUCCEED
	// OPA_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// OPA_CLOSE_SUCCEED
	// OPA_CLOSE_FAIL_WIN32_API
	// OPA_CLOSE_FAIL_NO_OPEN_PORT
	virtual DWORD Close(void) = 0;

	// 判断串口是或打开
	virtual bool IsOpen(void) const = 0;

	// 取仪器返回的错误码
	virtual DWORD GetErrorCodeFromEquipment(void) const = 0;

	// 终止预热
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD StopWarmUp(void) = 0;

	// 校准
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD Calibrate(void) = 0;

	// 清除最大值
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD ClearMaxValue(void) = 0;

	// 设置控制单元参数
	// (in)bParamType：参数类型
	// (out)bParamValue：参数值
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	// DWORD SetControlUnitParams(const enum ParamType pt);
	// DWORD SetControlUnitParams(const enum ParamType pt, const EngineStrokeType est);
	// DWORD SetControlUnitParams(const enum ParamType pt, const RpmMeasurementType rmt);
	// DWORD SetControlUnitParams(const enum ParamType pt, const FilterParamType fpt);
	virtual DWORD SetControlUnitParams(const ControlUnitParams *const psControlUnitParams) = 0;
	virtual DWORD SetControlUnitParams(const BYTE bParamType, const BYTE bParamValue) = 0;

	// 设置相对湿度
	// (in)fRHValue：标准值，单位：%
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD SetRH(const float fStdValue) = 0;

	// 复位EEPROM参数
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD ResetEEPROM(void) = 0;

	// 取仪器报警信息
	// (out)pbs16AlarmInfo：报警信息指针，共16位，数据位对应的含义请参看通讯说明书，不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo) = 0;

	// 取实时数据
	// (out)pfN：不透光度值指针，单位：%，不能为空指针
	// (out)pfK：光吸收系数值指针，单位：m-1，不能为空指针
	// (out)pusRpm：转速值指针，单位：r/min，不能为空指针
	// (out)pusOilTemp：油温值指针，单位：摄氏度，不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp) = 0;
		
	// 取实时数据(增加氮氧检测)
	// (out)pfN：不透光度值指针，单位：%，不能为空指针
	// (out)pfK：光吸收系数值指针，单位：m-1，不能为空指针
	// (out)pusRpm：转速值指针，单位：r/min，不能为空指针
	// (out)pusOilTemp：油温值指针，单位：摄氏度，不能为空指针
	// (out)pfNO：NO浓度指针,单位:ppm,不能为空指针
	// (out)pfNO2：NO2浓度指针,单位:ppm,不能为空指针
	// (out)pfCO2：CO2浓度指针,单位:%,不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp,
						short *const psNO, short *const psNO2, float *const pfCO2, 
						short *const psHC, float *const pfCO, float *const pfO2) = 0;

	// 取测量单元内部数据
	// (out)pfOriginalN：原始不透光度值指针，单位：%，可以为空指针
	// (out)pfOriginalK：原始光吸收系数值指针，单位：m-1，可以为空指针
	// (out)pusSmokeTemp：排烟温度值指针，单位：摄氏度，可以为空指针
	// (out)pusTubeTemp：测量管壁油温值指针，单位：摄氏度，可以为空指针
	// (out)pusDetectorTemp：检测器值指针，单位：摄氏度，可以为空指针
	// (out)pusLEDTemp：发光源值指针，单位：摄氏度，可以为空指针
	// (out)pusBoardTemp：内部温度值指针，单位：摄氏度，可以为空指针
	// (out)pusLeftFanCurrent：左风扇电流值指针，单位：未知，可以为空指针
	// (out)pusRightFanCurrent：右风扇电流值指针，单位：未知，可以为空指针
	// (out)pfPowerVoltage：电源电压值指针，单位：未知，可以为空指针
	// (out)pusRealTimeIntensity：实时光强值指针，单位：未知，可以为空指针
	// (out)pusFullShadingIntensity：全暗光强值指针，单位：未知，可以为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMeasureUnitData(float *const pfOriginalN=NULL, 
		float *const pfOriginalK=NULL, 
		USHORT *const pusSmokeTemp=NULL, 
		USHORT *const pusTubeTemp=NULL, 
		USHORT *const pusDetectorTemp=NULL, 
		USHORT *const pusLEDTemp=NULL, 
		USHORT *const pusBoardTemp=NULL, 
		USHORT *const pusLeftFanCurrent=NULL, 
		USHORT *const pusRightFanCurrent=NULL, 
		float *const pfPowerVoltage=NULL, 
		USHORT *const pusRealTimeIntensity=NULL, 
		USHORT *const pusFullShadingIntensity=NULL) = 0;

	// 取最大值
	// (out)pfMaxN：最大不透光度值指针，单位：%，可以为空指针
	// (out)pfMaxK：最大光吸收系数值指针，单位：m-1，可以为空指针
	// (out)pusMaxRpm：最大转速值指针，单位：r/min，可以为空指针
	// (out)pusMaxOilTemp：最大油温值指针，单位：摄氏度，可以为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMaxValue(float *const pfMaxN=NULL, float *const pfMaxK=NULL, USHORT *const pusMaxRpm=NULL, USHORT *const pusMaxOilTemp=NULL) = 0;

	// 取仪器状态
	// (out)pbStatus：仪器状态值指针（状态值含义参考说明书）不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetStatus(BYTE *const pbStatus) = 0;

	// 设仪器状态
	// (int)bStatus：仪器状态值指针（状态值含义参考说明书）
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD SetStatus(const BYTE bStatus) = 0;

	// 取预热剩余时间
	// (out)pusMin：分钟值指针，不能未空指针
	// (out)pusSec：秒钟值指针，不能未空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec) = 0;

	// 取环境参数
	// (out)pfET：环境温度值指针，单位：摄氏度，不能为空指针
	// (out)pfRH：相对湿度值指针，单位：%，不能为空指针
	// (out)pfAP：大气压力值指针，单位：kPa，不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP) = 0;

	// 取版本信息
	// (out)pfVersion：版本值指针，不能是空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetVersion(float *const pfVersion) = 0;

	// 取控制单元参数
	// (out)pbEngineStroke：发动机冲程
	// (out)pbRpmMeasureType：转速测量方式
	// (out)pbFilterConstant：滤波参数
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt) = 0;
	virtual DWORD GetControlUnitParams(BYTE *const pbEngineStroke, BYTE *const pbRpmMeasureType, BYTE *const pbFilterConstant) = 0;

	// 取补偿油温
	// (out)pfCorrectedOilTemp：补偿油温值指针，单位：摄氏度，不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetCorrectedOilTemp(float *const pfCorrectedOilTemp) = 0;

	// 取手动操作检测的数据
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm ,float *const pfValueKAvg) = 0;
		
	//进入灵敏度校准状态
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD EntrySensitivityCalStatus(void) = 0;

	//灵敏度校准
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD ProceedSensitivityCal(float const pfValue) = 0;

	//退出灵敏度校准状态
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD QuitSensitivityCalStatus(void) = 0;

protected:
	// 串口句柄
	HANDLE m_hComm;
	// 临界保护量
	CRITICAL_SECTION cInfoSection;
};
#endif
