/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaPara.h
* 文件标识：
* 摘要：参数测量仪实体类
*
* 当前版本：1.0.2
* 作者：Hyh
* 完成日期：2010-06-08
*
* 历史信息：
*
*
* 当前版本：1.0.2
* 完成日期：2010-06-08
* 作者：Hyh
* 描述：1)添加点火方式和冲程数设置虚函数
*
* 版本：1.0.1
* 日期：2010-05-27
* 作者：Cui
* 描述：修改GetEntireData、GetEnvironmentParameters、GetOilTempRpm函数体，使可以接受NULL指针
*
* 当前版本：1.0.0
* 完成日期：2010-05-11
* 作者：Hyh
* 描述：正式发布使用
*/
#pragma once

#ifndef NANHUAPARA_H
#define NANHUAPARA_H

#include "Para.h"

class CNanhuaPara : public CPara
{
public:
	CNanhuaPara(void);
	~CNanhuaPara(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);
	DWORD Open(std::wstring strIP, const int nPort) { return PARA_OPEN_FAIL;}

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

protected:
	// CRC Calculation
	void CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW);
	// Write Buffer
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);
	// Read Comm Buffer
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);

protected:
	// 执行恢复参数命令
	bool ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2);

public:
	// 温度斜率校准
	DWORD EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// 湿度斜率校准
	DWORD RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// 大气压斜率校准
	DWORD AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// 油温冷端斜率校准
	DWORD OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// 油温热端斜率校准
	DWORD OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// 温度零位校准
	DWORD EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// 湿度零位校准
	DWORD RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// 大气压零位校准
	DWORD AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// 油温冷端零位校准
	DWORD OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// 油温热端零位校准
	DWORD OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// 备份校准参数
	DWORD BackupCalibrationParameters(BYTE *const pbResult);

	// 恢复校准参数
	DWORD RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType);

	// 设置冲程为4
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL);

	// 设置冲程为2
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL);

	// 设置点火方式为单次
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL);

	// 设置点火方式为二次
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL);

	// 读全部数据
	DWORD GetEntireData(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL);

	// 读环境参数
	DWORD GetEnvironmentParameters(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL);

	// 读转速油温
	DWORD GetOilTempRpm(float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL);

	// 数据传输
	DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

	// 获取IMEI和ICCID
	DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID);

protected:
	// 串口句柄
	HANDLE m_hComm;
	// 临界保护量
	CRITICAL_SECTION m_sCriticalSection;
};
#endif