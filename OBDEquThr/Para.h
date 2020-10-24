/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：Para.h
* 文件标识：
* 摘要：参数测量仪抽象类
*		以纯虚拟函数形式定义参数测量仪操作接口
*
* 当前版本：1.0
* 作者：Hyh
* 完成日期：2010-05-11
*
* 历史信息：
*
*
* 当前版本：1.0.1
* 完成日期：2010-06-08
* 作者：Hyh
* 描述：1)添加点火方式和冲程数设置虚函数
*
* 当前版本：1.0.0
* 完成日期：2010-05-11
* 作者：Hyh
* 描述：正式发布使用
*/
#pragma once


#ifndef PARA_H
#define PARA_H

#include <string>

// 函数执行结果
#define PARA_OPEN_SUCCEED							0x00  // 打开串口成功
#define PARA_OPEN_FAIL								0x01  // 打开串口失败
#define PARA_CLOSE_SUCCEED							0x00  // 关闭串口成功
#define PARA_CLOSE_FAIL_WIN32_API					0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define PARA_CLOSE_FAIL_NO_OPEN_PORT				0x02  // 关闭串口失败：没有可以关闭的串口
#define PARA_WR_SUCCEED								0x00  // 写读成功
#define PARA_WR_PORT_NOT_OPEN						0x01  // 写读失败：串口未打开
#define PARA_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // 写读失败：读指定数据失败
#define PARA_WR_READ_SPECIFIED_DATA_FAIL			0x03  // 写读失败：读指定数据失败
#define PARA_WR_EQUIP_RETURE_0X15					0x04  // 写读失败：仪器返回错误代码（0x15）
#define PARA_WR_CHECK_SUM_FAIL						0x05  // 写读失败：读出数据校验和失败
#define PARA_WR_UNKNOWN_ERROR						0xFF  // 写读失败：未知错误

class CPara
{
public:
	CPara(void);
	virtual ~CPara(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// PARA_OPEN_SUCCEED
	// PARA_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;
	virtual DWORD Open(std::wstring strIP, const int nPort) = 0;

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// PARA_CLOSE_SUCCEED
	// PARA_CLOSE_FAIL_WIN32API
	// PARA_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// 判断串口是或打开
	virtual bool IsOpen(void) const = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

public:
	enum ParaType
	{
		// 所有参数
		AllParameters,
		// 环境温度参数
		EnvironmentTemperaturePara,
		// 相对湿度参数
		RelativeHumidityPara,
		// 大气压力参数
		AirPressurePara,
		// 油温参数
		OilTemperaturePara
	};

public:
	// 温度斜率校准
	virtual DWORD EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// 湿度斜率校准
	virtual DWORD RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// 大气压斜率校准
	virtual DWORD AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// 油温冷端斜率校准
	virtual DWORD OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// 油温热端斜率校准
	virtual DWORD OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// 温度零位校准
	virtual DWORD EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// 湿度零位校准
	virtual DWORD RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// 大气压零位校准
	virtual DWORD AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// 油温冷端零位校准
	virtual DWORD OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// 油温热端零位校准
	virtual DWORD OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// 备份校准参数
	virtual DWORD BackupCalibrationParameters(BYTE *const pbResult) = 0;

	// 恢复校准参数
	virtual DWORD RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType) = 0;

	// 设置冲程为4
	virtual DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL) = 0;

	// 设置冲程为2
	virtual DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL) = 0;

	// 设置点火方式为单次
	virtual DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) = 0;

	// 设置点火方式为二次
	virtual DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL) = 0;

	// 读全部数据
	virtual DWORD GetEntireData(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL) = 0;

	// 读环境参数
	virtual DWORD GetEnvironmentParameters(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL) = 0;

	// 读转速油温
	virtual DWORD GetOilTempRpm(float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL) = 0;

	// 数据传输
	virtual DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen) = 0;

		// 获取IMEI和ICCID
	virtual DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID) = 0;
};
#endif