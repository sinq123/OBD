/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FlowThread.h
* 文件标识：
* 摘要：流量计线程
*
* 当前版本：
* 作者：
* 完成日期：
*
*/
#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"
#include "NanhuaNHF1.h"
// FlowThread

class AFX_EXT_CLASS CFlowThread : public CWinThread
{
	DECLARE_DYNCREATE(CFlowThread)

protected:
	CFlowThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CFlowThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:

	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// (in)im：仪器类型
	// (in)bConnectToFlowmeter：是否有连接流量计
	// 返回打开结果：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// ANA_CLOSE_SUCCEED
	// ANA_CLOSE_FAIL_WIN32API
	// ANA_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;


	// 1.获取流量计实时数据
	// pfFluxStd0:0℃标准流量，L/s
	// pfFluxStd20:20℃标准流量，L/s
	// pfFluxAct:实际流量，L/s
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	// 2.获取状态
	DWORD Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau);

	// 3.开关风机
	// (in)bRun:开关风机参数；true-开风机，false-关风机
	// (out)pbResult:仪器操作结果指针；0x06-设置模式成功，0x05-设置模式失败(流量计忙)， 0x15-设置模式失败(错误命令)
	//               如果函数执行过程错误，返回0xFF
	//				 可以为NULL
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);

	// 4.设置标准氧气
	DWORD SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult);

	// 5.设置标准压力
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);

	// 6.设置温度
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);

	// 7.设置标准流量 注:fCalFlux需标准化到20℃状态下 Mark By LiangQW@20160721
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CNanhuaNHF1::FluxCalibrationMode cm, const float fCalFlux);

	// 8.设置高点标准氧气(适用国产流量计)
	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);

	// 9.设置中点标准氧气(适用国产流量计)
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);

	// 10.设置低点标准氧气(适用国产流量计)
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);

	// 11.恢复出厂设置
	DWORD RestoreFactorySettings_Flowmeter(BYTE *const pbResult);



	
public:
	// 退出线程操作
	void QuitThread(void);

private:
	// 初始化设备
	void InitializeInstrument(void);
	// 卸载设备
	void UninitializeInstrument(void);

private:
	// 外部协议流量计
	CNanhuaNHF1* m_pFlow;
	// 线程退出标志
	bool m_bQuitThread;

protected:
	// 流量值,单位:scfm(标准立方英尺/分钟)
	float m_fFluxStd0;
	float m_fFluxStd20;
	float m_fFluxAct;
	// 压力值,单位:kPa
	float  m_fPres;
	// 未修正稀释氧,单位:%
	float  m_fUDilO2;
	// 稀释气体温度,单位:摄氏度
	float m_fGasTemp; 
	
	// 串口操作状态
	DWORD m_dwStatus;
};


