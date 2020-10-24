/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：DigitalThermometer_NJRJ.h
* 文件标识：
* 摘要：南京睿君电子科技有限公司DPH-103(CMC号：苏制01100013)数字式参数测量仪实体类
* 当前版本：1.0.0
* 完成日期：2015-12-11
* 作者：LiangQW
* 描述：正式发布使用
* 仅支持GetEnvironmentParameters,其余返回PARA_WR_UNKNOWN_ERROR
*/
#pragma once
#ifndef DIGITALTHERMOMETER_NJRJ_H
#define DIGITALTHERMOMETER_NJRJ_H
#include "Para.h"
class CDigitalThermometer_NJRJ : public CPara
{
public:
	CDigitalThermometer_NJRJ(void);
	~CDigitalThermometer_NJRJ(void);

public:
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// (return)：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);
	DWORD Open(std::wstring strIP, const int nPort) { return PARA_OPEN_FAIL;}

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// (return)：
	// NHC_CLOSE_SUCCEED
	// NHC_CLOSE_FAIL_WIN32API
	// NHC_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;

protected:
	// CRC Calculation
	void CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW);
	// Write Buffer
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);
	// Read Comm Buffer
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);
	// 执行恢复参数命令
	bool ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2);

protected:
	// 串口句柄
	HANDLE m_hComm;
	// 串口操作临界区
	CRITICAL_SECTION m_sCriticalSection;

protected:
	float m_fET;
	float m_fAP;
	float m_fRH;
	int m_nFlagET;
	int m_nFlagRH;
	int m_nFlagAP;
	DWORD m_dwReadIntervalTimeout;// 读间隔超时
	DWORD m_dwReadTotalTimeoutMultiplier;// 读时间系数
	DWORD m_dwReadTotalTimeoutConstant;// 读时间超时
	DWORD m_dwWriteTotalTimeoutMultiplier;// 写时间系数
	DWORD m_dwWriteTotalTimeoutConstant;// 写时间超时
	COMMTIMEOUTS m_sCommTimeouts;

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
};
#endif