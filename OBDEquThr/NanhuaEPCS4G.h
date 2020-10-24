/*
* Copyright (c) 2018,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaEPCS.h
* 文件标识：
* 摘要：参数测量仪服务实体类
*
* 当前版本：1.0
* 作者：Hyh
* 完成日期：2018-03-10
*/
#pragma once

#ifndef NANHUAEPCS4G_H
#define NANHUAEPCS4G_H

#include "Para.h"

class CNanhuaEPCS4G : public CPara
{
public:
	CNanhuaEPCS4G(void);
	~CNanhuaEPCS4G(void);

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
	DWORD DataTrans(BYTE *const pbResult,const BYTE bCom[], const BYTE * pbDataBuf, const UINT nDatalen);

	// 进入传输数据模式
	DWORD EnterDTMode(BYTE *const pbResult);

	// 传输数据
	DWORD TransData(BYTE *const pbResult, const BYTE * pbDataBuf, const UINT nDatalen);

	// 退出传输数据模式
	DWORD ExitDTMode(BYTE *const pbResult);

	// 获取IMEI和ICCID
	DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID);

	// 传输数据
	DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

protected:
	// 串口句柄
	HANDLE m_hComm;
	// 临界保护量
	CRITICAL_SECTION m_sCriticalSection;
};
#endif