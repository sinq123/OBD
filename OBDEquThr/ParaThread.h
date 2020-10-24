/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：ParaThread.h
* 文件标识：
* 摘要：参数测量仪线程
*
* 当前版本：1.0.1
* 作者：Hyh
* 完成日期：2010-06-08
*
* 历史信息：
*
* 当前版本：1.0.1
* 完成日期：2010-06-08
* 作者：Hyh
* 描述：1)添加点火方式和冲程数设置
*
* 当前版本：1.0.0
* 完成日期：2010-05-12
* 作者：Hyh
* 描述：正式发布使用
*/
#pragma once
#include "..\NHDetCommModu\NHDetCommModu.h"
#include "..\NHWin32Lib\NHWin32Lib.h"

#include "NanhuaPara.h"
#include "NanhuaEPCS.h"
#include "NanhuaEPCS4G.h"
#include "NanhuaSocketPara.h"
#include "DigitalThermometer.h"
#include "DigitalThermometer_NJRJ.h"
#include "SG506.h"

// CParaThread
class AFX_EXT_CLASS CParaThread : public CWinThread
{
	DECLARE_DYNCREATE(CParaThread)

protected:
	CParaThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CParaThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()

public:
	// 仪器型号
	enum InstrumentModel
	{
		// PARA
		IM_NANHUA_PARA = 0,
		IM_PARA_DIM,
		IM_PARA_DIM_NJRJ,
		IM_PARA_SG506,
		IM_NANHUA_EPCS,
		//支持写入版本
		IM_NANHUA_EPCS_4G,
		// 南华参数仪，socket获取
		IM_NANHUA_PARA_Socket,
	};

public:
	// 打开串口
	DWORD Open(const BYTE bPort, const enum InstrumentModel im);
	DWORD Open(std::wstring strIP, const int nPort, const enum InstrumentModel im);
	DWORD ReOpen(void);

	// 关闭串口
	DWORD Close(void);

	// 判断串口是或打开
	inline bool IsOpen(void) const
	{
		return m_pPara->IsOpen();
	}
	
	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emIM;
	}

	// 获取串口操作状态
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
public:
	// 退出线程操作
	void QuitThread(void);

public:
	// 获取实时数据-温度
	inline float GetETValue(void) const
	{
		return m_fET;
	}
	// 获取实时数据-湿度
	inline float GetRHValue(void) const
	{
		return m_fRH;
	}
	// 获取实时数据-大气压
	inline float GetAPValue(void) const
	{
		return m_fAP;
	}
	// 获取实时数据-油温
	inline float GetOilTempValue(void) const
	{
		return m_fOilTemp;
	}
	// 获取实时数据-转速
	inline USHORT GetRpmValue(void) const
	{
		return m_usRpm;
	}

public:
	// 同步操作
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
	DWORD RecoverCalibrationParameters(BYTE *const pbResult, const CPara::ParaType emParaType);

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

	// DT 
	bool DT_ASM(const TESTLOG &sTestLog, const SResultOfASM &sResultOfASM);
	bool DT_VMAS(const TESTLOG &sTestLog, const SResultOfVMAS &sResultOfVMAS);
	bool DT_LUGDOWN(const TESTLOG &sTestLog, const SResultOfLUGDOWN &sResultOfLUGDOWN);
	bool DT_DIS(const TESTLOG &sTestLog, const SResultOfDIS &sResultOfDIS);
	bool DT_FSUNHT(const TESTLOG &sTestLog, const SResultOfFSUNHT &sResultOfFSUNHT);
	bool DT_FSUYD(const TESTLOG &sTestLog, const RESULTOFFSUYD &sResultOfFSUYD);

public:
	// 异步函数
	// 读全部数据
	void GetEntireData_Asyn(HWND hCallBackWnd=NULL);

	// 读环境参数
	void GetEnvironmentParameters_Asyn(HWND hCallBackWnd=NULL);

	// 读转速油温
	void GetOilTempRpm_Asyn(HWND hCallBackWnd=NULL);

	// DT命令
	void DTCommand_Asyn(const BYTE * pbDataBuf, const UINT nDatalen);

private:
	// 初始化设备
	void InitializeInstrument(const enum InstrumentModel im);
	// 卸载设备
	void UninitializeInstrument(void);

private:
	// 响应异步操作函数
	afx_msg void OnGetEntireData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetEnvironmentParameters(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetOilTempRpm(WPARAM wParam, LPARAM lParam);

private:
	// 异步SQLite线程
	static DWORD __stdcall WorkerThread(LPVOID pParam);

private:
	// 打开SQLite3数据库
	DWORD OpenDB(void);
	// 关闭SQLite3数据库
	DWORD CloseDB(void);
	// 检查SQLite3数据库
	void CheckDB(void);
	// 判断串口是或打开
	inline bool IsDBOpen(void) const
	{
		return m_bOpenDB;
	}

	// 发送数据
	void  DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

private:
	// 对象
	CPara* m_pPara;
	// 线程退出标志
	bool m_bQuitThread;

private:
	// 温度,湿度,大气压,油温
	float m_fET,m_fRH,m_fAP,m_fOilTemp;
	// 转速
	USHORT m_usRpm;
	// 串口操作状态
	DWORD m_dwStatus;

private:
	enum InstrumentModel m_emIM;
	// SQLite3数据库
	CppSQLite3DB m_db;
	// SQLite3数据库是否打开
	bool m_bOpenDB;
	// ICCID
	char m_chICCID[21];

	// 临界保护量
	CRITICAL_SECTION m_sCriticalSection;
	// 是否退出工作者线程
	bool m_bWorkerRunning;
	HANDLE m_hCloseWorker;
	// 计数器
	int m_nWorker;
		
	// 串口号
	BYTE m_bComPort;
	// 串口波特率
	int m_nComBaudRate;

	// 服务器IP地址
	std::wstring m_strServerIP;
	// 服务器端口号
	unsigned short m_usServerPort;
};


