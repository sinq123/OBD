/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaNHF1.h
* 文件标识：
* 摘要：流量计NHF1通讯类
*
*
* 当前版本：
* 作者：
* 完成日期：
*
*/

#pragma once

#ifndef NANHUA_NHF1_H
#define NANHUA_NHF1_H

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


class CNanhuaNHF1
{
public:
	CNanhuaNHF1(void);
	~CNanhuaNHF1(void);

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
		bool bNeedReadChkSum=true);

	// 计算前n位的校验和
	// 相加，然后取最低字节位
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	BYTE CheckSum_Flowmeter(const BYTE *const pb, const UINT un);

	//计算读校验是否为真
	bool CheckReadSum_Flowmeter(const BYTE *const pb, const UINT un);

public:
	
	////////////////////////////////////////////////////////////////////////////////////////////////////流量计校准操作
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL

	// 1.获取流量计实时数据
	// pfFluxStd0:0℃标准流量，L/s
	// pfFluxStd20:20℃标准流量，L/s
	// pfFluxAct:实际流量，L/s
	// (out)pbResult:仪器操作结果指针；0x31-成功，0x30-失败
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, 
		float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	// 2.获取状态
	DWORD Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau);
	
	// 3.开关风机
	// (in)bRun:开关风机参数；true-开风机，false-关风机
	// (out)pbResult:仪器操作结果指针；0x06-设置模式成功，0x05-设置模式失败(流量计忙)， 0x15-设置模式失败(错误命令)
	//               如果函数执行过程错误，返回0xFF
	//				 可以为NULL
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);

	// 4.稀释氧空气调零
	DWORD SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult);


	// 5.设置标准压力
	// 返回函数执行结果：
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);

	// 6.流量计温度校准：注意：进口流量计机芯不支持
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);

	// 7.流量高/低/单量程标定
	// 返回函数执行结果：
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux);

	// 8.稀释氧高量程标定
	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);

	// 9.稀释氧中量程标定
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);

	// 10.稀释氧低量程标定
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);
	
	// 11.恢复出厂设置
	DWORD RestoreFactorySettings_Flowmeter(BYTE *const pbResult);

protected:
	// 串口句柄
	HANDLE m_hComm;
	// 临界保护量
	CRITICAL_SECTION cInfoSection;
};
#endif