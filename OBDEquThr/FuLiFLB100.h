/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FuLiFLB100.h
* 文件标识：
* 摘要：福立不透光度计FLB-100通讯类，继承于抽象类COpa
*		
*
* 版本：1.0
* 作者：Raylee
* 完成日期：2012-10-11
*
* 历史信息：
*
* 版本：1.0.0
* 作者：Raylee
* 日期：2012-10-11
* 描述：发布使用
*/

#pragma once

#ifndef FULI_FLB100_H
#define FULI_FLB100_H

#include "Opa.h"

class CFuLiFLB100 : public COpa
{
public:
	CFuLiFLB100(void);
	~CFuLiFLB100(void);

public:
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// OPA_OPEN_SUCCEED
	// OPA_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// OPA_CLOSE_SUCCEED
	// OPA_CLOSE_FAIL_WIN32API
	// OPA_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;

	// 取仪器返回的错误码
	DWORD GetErrorCodeFromEquipment(void) const;

	// 终止预热
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD StopWarmUp(void);

	// 校准
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD Calibrate(void);

	// 清除最大值
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD ClearMaxValue(void);

	// 设置控制单元参数
	// (in)bParamType：参数类型
	// (out)bParamValue：参数值
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	//DWORD SetControlUnitParams(const enum ParamType pt);
	//DWORD SetControlUnitParams(const enum ParamType pt, const EngineStrokeType est);
	//DWORD SetControlUnitParams(const enum ParamType pt, const RpmMeasurementType rmt);
	//DWORD SetControlUnitParams(const enum ParamType pt, const FilterParamType fpt);
	DWORD SetControlUnitParams(const ControlUnitParams *const psControlUnitParams);
	DWORD SetControlUnitParams(const BYTE bParamType, const BYTE bParamValue);

	// 设置相对湿度
	// (in)fRHValue：标准值，单位：%
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD SetRH(const float fStdValue);

	// 复位EEPROM参数
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD ResetEEPROM(void);

	// 取仪器报警信息
	// (out)pbs16AlarmInfo：报警信息指针，共16位，数据位对应的含义请参看通讯说明书，不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo);

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
	DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp);
	DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp,
						short *const psNO, short *const psNO2, float *const pfCO2, 
						short *const psHC, float *const pfCO, float *const pfO2)
	{
		return OPA_WR_EQUIP_RETURE_0X15;
	}

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
	DWORD GetMeasureUnitData(float *const pfOriginalN=NULL, 
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
		USHORT *const pusFullShadingIntensity=NULL);

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
	DWORD GetMaxValue(float *const pfMaxN=NULL, float *const pfMaxK=NULL, USHORT *const pusMaxRpm=NULL, USHORT *const pusMaxOilTemp=NULL);

	// 取仪器状态
	// (out)pbStatus：仪器状态值指针（0，预热，1，实时检测，2，校准，3控制单元与测量单元通信错误）不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);

	// 设仪器状态
	// (int)bStatus：仪器状态值指针（状态值含义参考说明书）
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD SetStatus(const BYTE bStatus);

	// 取预热剩余时间
	// (out)pusMin：分钟值指针，不能未空指针
	// (out)pusSec：秒钟值指针，不能未空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec);

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
	DWORD GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP);

	// 取版本信息
	// (out)pfVersion：版本值指针，不能是空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetVersion(float *const pfVersion);

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
	DWORD GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt);
	DWORD GetControlUnitParams(BYTE *const pbEngineStroke, BYTE *const pbRpmMeasureType, BYTE *const pbFilterConstant);

	// 取补偿油温
	// (out)pfCorrectedOilTemp：补偿油温值指针，单位：摄氏度，不能为空指针
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetCorrectedOilTemp(float *const pfCorrectedOilTemp);
		
	// 取手动操作检测的数据
	// 返回函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg);
	
	//进入灵敏度校准状态
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD EntrySensitivityCalStatus(void){return OPA_WR_EQUIP_RETURE_0X15;}

	//灵敏度校准
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD ProceedSensitivityCal(float const pfValue){return OPA_WR_EQUIP_RETURE_0X15;}

	//退出灵敏度校准状态
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD QuitSensitivityCalStatus(void){return OPA_WR_EQUIP_RETURE_0X15;}

protected:
	// 写读串口数据
	// 写入相应的命令，读取对应的返回数据
	// 并根据需要，分别对写入和读取的数据进行校验和验证
	// (in)pbWriteBuffer：要写入的数据内容，不能为空指针
	// (in)dwWriteLength：要写入的数据长度
	// (out)pbReadBuffer：要读取的数据内容，不能为空指针
	// (in)dwReadLength：要读取的数据长度
	// (in)bNeedWriteChkSum：true，需要写校验，false：不需要
	// (in)bNeedReadChkSum：true，需要读校验，false：不需要
	// 返回，函数执行结果：
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum=true, bool bNeedReadChkSum=true);

	// 计算校验和
	// 相加，然后取反加一
	// (in)p：要进行校验的数据内容
	// (in)un：要进行校验的数据长度
	// 返回校验和
	BYTE CheckSum(BYTE *p, UINT un);
protected:
	//// 串口句柄
	//HANDLE m_hComm;
	// 状态码:0,预热状态;1,实时检测状态;2,校准状态;3,控制单元与测量单元通信错误;
	BYTE m_bStatus;
	// 最后一个非法或错误指令的状态码
	BYTE m_bLastErrorCode;
private:
	////客户信息临界保护量
	//CRITICAL_SECTION cInfoSection;
};
#endif