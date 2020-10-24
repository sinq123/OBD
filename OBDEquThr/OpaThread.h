/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：OpaThread.h
* 文件标识：
* 摘要：不透光度计线程
*
* 当前版本：1.0
* 作者：Cui,hyh
* 完成日期：2009-12-08
*
* 历史信息：
*
* 版本：1.0.3
* 日期：2012-10-11
* 作者：Raylee
* 描述：添加福立FLB-100，鸣泉MQY-200设备支持
*
* 版本：1.0.2
* 日期：2012-04-26
* 作者：Raylee
* 描述：添加FoFenFTY100支持
*
* 当前版本：1.0.1
* 完成日期：2010-08-26
* 作者：Raylee
* 描述：添加GetTestResultData()函数，发送 0xB1 指令，获取检测结果数据
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaNht6.h"
#include "NanhuaNht1l.h"
#include "NanhuaNHT610.h"
#include "FoFenFTY100.h"
#include "MingQuanMQY200.h"
#include "FuLiFLB100.h"


// COpaThread

class AFX_EXT_CLASS COpaThread : public CWinThread
{
	DECLARE_DYNCREATE(COpaThread)

protected:
	COpaThread();           // 动态创建所使用的受保护的构造函数
	virtual ~COpaThread();

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
		// NHT1L
		IM_NANHUA_NHT1L,
		// NHT6
		IM_NANHUA_NHT6,
		// NHT610
		IM_NANHUA_NHT610,
		// 佛分FTY-100
		IM_FOFEN_FTY100,
		// 福立
		IM_FULI_FLB100,
		// 鸣泉
		IM_MINGQUAN_MQY200,
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
	
	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emInstrumentModel;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作begin
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// NHT1L_OPEN_SUCCEED
	// NHT1L_OPEN_FAIL
	DWORD Open(const BYTE bPort, const enum InstrumentModel im);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// NHT1L_CLOSE_SUCCEED
	// NHT1L_CLOSE_FAIL_WIN32API
	// NHT1L_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	inline bool IsOpen(void) const
	{
		return m_pOpa->IsOpen();
	}

	// 获取串口操作状态
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

	// 取仪器返回的错误码
	inline DWORD GetErrorCodeFromEquipment(void) const
	{
		return m_pOpa->GetErrorCodeFromEquipment();
	}

	// 终止预热
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD StopWarmUp(void);

	// 校准
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD Calibrate(void);

	// 清除最大值
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD ClearMaxValue(void);

	// 设置控制单元参数
	// (in)bParamType：参数类型
	// (out)bParamValue：参数值
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD SetControlUnitParams(const COpa::ControlUnitParams *const psControlUnitParams);
	//DWORD SetControlUnitParams(const BYTE bParamType, const BYTE bParamValue);

	// 设置相对湿度
	// (in)fRHValue：标准值，单位：%
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD SetRH(const float fStdValue);

	// 复位EEPROM参数
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD ResetEEPROM(void);

	// 取仪器报警信息
	// (out)pbs16AlarmInfo：报警信息指针，共16位，数据位对应的含义请参看通讯说明书，不能为空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo);
	inline DWORD GetAlarmInfo(void)
	{
		return GetAlarmInfo(&m_bs16AlarmInfo);
	}

	// 取实时数据
	// (out)pfN：不透光度值指针，单位：%，不能为空指针
	// (out)pfK：光吸收系数值指针，单位：m-1，不能为空指针
	// (out)pusRpm：转速值指针，单位：r/min，不能为空指针
	// (out)pusOilTemp：油温值指针，单位：摄氏度，不能为空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp);
	inline DWORD GetRealTimeData(void)
	{
		return GetRealTimeData(&m_fN, &m_fK, &m_usRpm, &m_usOilTemp);
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
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
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
	inline DWORD GetMeasureUnitData(void)
	{
		return GetMeasureUnitData(&m_fOriginalN,
			&m_fOriginalK,
			&m_usSmokeTemp,
			&m_usTubeTemp,
			&m_usDetectorTemp,
			&m_usLEDTemp,
			&m_usBoardTemp,
			&m_usLeftFanCurrent,
			&m_usRightFanCurrent,
			&m_fPowerVoltage,
			&m_usRealTimeIntensity,
			&m_usFullShadingIntensity);
	}

	// 取最大值
	// (out)pfMaxN：最大不透光度值指针，单位：%，可以为空指针
	// (out)pfMaxK：最大光吸收系数值指针，单位：m-1，可以为空指针
	// (out)pusMaxRpm：最大转速值指针，单位：r/min，可以为空指针
	// (out)pusMaxOilTemp：最大油温值指针，单位：摄氏度，可以为空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetMaxValue(float *const pfMaxN, float *const pfMaxK, USHORT *const pusMaxRpm, USHORT *const pusMaxOilTemp);
	inline DWORD GetMaxValue(void)
	{
		return GetMaxValue(&m_fMaxN, &m_fMaxK, &m_usMaxRpm, &m_usMaxOilTemp);
	}

	// 取仪器状态
	// (out)pbStatus：仪器状态值指针（0，预热，1，实时检测，2，校准，3控制单元与测量单元通信错误）不能为空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);
	inline DWORD GetStatus(void)
	{
		return GetStatus(&m_bStatus);
	}

	// 设仪器状态
	// (in)bStatus：仪器状态值
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD SetStatus(const BYTE bStatus);

	// 取预热剩余时间
	// (out)pusMin：分钟值指针，不能未空指针
	// (out)pusSec：秒钟值指针，不能未空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec);
	inline DWORD GetWarmUpTime(void)
	{
		return GetWarmUpTime(&m_usMin, &m_usSec);
	}

	// 取环境参数
	// (out)pfET：环境温度值指针，单位：摄氏度，不能为空指针
	// (out)pfRH：相对湿度值指针，单位：%，不能为空指针
	// (out)pfAP：大气压力值指针，单位：kPa，不能为空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP);
	inline DWORD GetEnvParams(void)
	{
		return GetEnvParams(&m_fET, &m_fRH, &m_fAP);
	}
	// 取环境温度值,单位:摄氏度
	inline float GetEnvDataETValue(void) const
	{
		return m_fET;
	}
	// 取大气压力值,单位:kPa
	inline float GetEnvDataAPValue(void) const
	{
		return m_fAP;
	}
	// 取相对湿度值,单位:%
	inline float GetEnvDataRHValue(void) const
	{
		return m_fRH;
	}
	// 取版本信息
	// (out)pfVersion：版本值指针，不能是空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetVersion(float *const pfVersion);
	inline DWORD GetVersion(void)
	{
		return GetVersion(&m_fVersion);
	}

	// 取控制单元参数
	// (out)pbEngineStroke：发动机冲程
	// (out)pbRpmMeasureType：转速测量方式
	// (out)pbFilterConstant：滤波参数
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetControlUnitParams(enum COpa::EngineStrokeType *const pest, enum COpa::RpmMeasurementType *const prmt, enum COpa::FilterParamType *const pfpt);
	inline DWORD GetControlUnitParams(void)
	{
		return GetControlUnitParams(&m_est, &m_rmt, &m_fpt);
	}

	// 取补偿油温
	// (out)pfCorrectedOilTemp：补偿油温值指针，单位：摄氏度，不能为空指针
	// 返回函数执行结果：
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetCorrectedOilTemp(float *const pfCorrectedOilTemp);
	inline DWORD GetCorrectedOilTemp(void)
	{
		return GetCorrectedOilTemp(&m_fCorrectedOilTemp);
	}
		
	// 取手动操作检测的数据
	// 返回函数执行结果：
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器基本操作（异步）begin 
	// 对应同步Calibrate
	void Calibrate_Asyn(HWND hCallBackWnd=NULL);
	// 对应同步GetRealTimeData
	void GetRealTimeData_Asyn(HWND hCallBackWnd=NULL);
	// 对应同步GetEnvParams
	void GetEnvRealTimeData_Asyn(HWND hCallBackWnd=NULL);
	// 取最大值
	void GetMaxValue_Asyn(HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器基本操作（异步）end 

private:
	// 响应异步操作函数
	afx_msg void OnCalibrate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetEnvRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMaxValue(WPARAM wParam, LPARAM lParam);

public:
	// 指定报警信息位是否处于有信号状态
	// (in)bAlarmInfoBitNumber：报警信息位号，具体每个报警信息位的含义请查看通讯协议，取值范围只能是0~15
	// return：如果该状态为1，返回true，否则，返回false
	inline bool IsTheAlarmInfoBitSignaled(const BYTE bAlarmInfoBitNumber) const
	{
		ASSERT(bAlarmInfoBitNumber < 16);

		return m_bs16AlarmInfo.test(bAlarmInfoBitNumber);
	}
	// 获取最大不透光度值，单位：%
	inline float GetMaxNValue(void) const
	{
		return m_fMaxN;
	}
	// 获取最大光吸收系数值，单位：m-1
	inline float GetMaxKValue(void) const
	{
		return m_fMaxK;
	}
	// 获取最大转速值,单位:r/min
	inline USHORT GetMaxRpmValue(void) const
	{
		return m_usMaxRpm;
	}
	// 获取最大油温值,单位:摄氏度
	inline USHORT GetMaxOilTempValue(void) const
	{
		return m_usMaxOilTemp;
	}
	// 获取实时不透光度值,单位:%
	inline float GetRealTimeNValue(void) const
	{
		return m_fN;
	}
	// 获取实时光吸收系数值,单位:m-1
	inline float GetRealTimeKValue(void) const
	{
		return m_fK;
	}
	// 获取实时转速值,单位:r/min
	inline USHORT GetRealTimeRpmValue(void) const
	{
		return m_usRpm;
	}
	// 获取实时油温值,单位:摄氏度
	inline USHORT GetRealTimeOilTempValue(void) const
	{
		return m_usOilTemp;
	}
	// 获取仪器状态值
	inline BYTE GetStatusValue(void) const
	{
		return m_bStatus;
	}

	inline short GetNOValue(void) const
	{
		return m_sNO;
	}
	inline short GetNO2Value(void) const
	{
		return m_sNO2;
	}
	inline float GetCO2Value(void) const
	{
		return m_fCO2;
	}
	inline short GetHCValue(void) const
	{
		return m_sHC;
	}
	inline float GetCOValue(void) const
	{
		return m_fCO;
	}
	inline float GetO2Value(void) const
	{
		return m_fO2;
	}

public:
	// 退出线程操作
	void QuitThread(void);

private:
	// 初始化设备
	void InitializeInstrument(const enum InstrumentModel im);
	// 卸载设备
	void UninitializeInstrument(void);

private:
	// 报警信息
	std::bitset<16> m_bs16AlarmInfo;

	// 不透光度,单位:%
	float m_fN;
	// 光吸收系数,单位:m-1
	float m_fK;
	// 转速,单位:r/min
	USHORT m_usRpm;
	// 油温,单位:摄氏度
	USHORT m_usOilTemp;

	short m_sNO;
	short m_sNO2;
	float m_fCO2;
	short m_sHC;
	float m_fCO;
	float m_fO2;

	// 原始不透光度值,单位:%
	float m_fOriginalN;
	// 原始光吸收系数值,单位:m-1
	float m_fOriginalK;
	// 排烟温度值,单位:摄氏度
	USHORT m_usSmokeTemp;
	// 测量管壁油温值指针，单位：摄氏度
	USHORT m_usTubeTemp;
	// 检测器值指针，单位：摄氏度
	USHORT m_usDetectorTemp;
	// 发光源值指针，单位：摄氏度
	USHORT m_usLEDTemp;
	// 内部温度值指针，单位：摄氏度
	USHORT m_usBoardTemp;
	// 左风扇电流值指针，单位：未知
	USHORT m_usLeftFanCurrent;
	// 右风扇电流值指针，单位：未知
	USHORT m_usRightFanCurrent;
	// 电源电压值指针，单位：未知
	float m_fPowerVoltage;
	// 实时光强值指针，单位：未知
	USHORT m_usRealTimeIntensity;
	// 全暗光强值指针，单位：未知
	USHORT m_usFullShadingIntensity;

	// 最大不透光度值，单位：%
	float m_fMaxN;
	// 最大光吸收系数值，单位：m-1
	float m_fMaxK;
	// 最大转速值,单位:r/min
	USHORT m_usMaxRpm;
	// 最大油温值,单位:摄氏度
	USHORT m_usMaxOilTemp;

	// 仪器状态值
	// 0，预热，1，实时检测，2，校准，3控制单元与测量单元通信错误
	BYTE m_bStatus;

	// 分
	USHORT m_usMin;
	// 秒
	USHORT m_usSec;

	// 环境温度,单位:摄氏度
	float m_fET;
	// 相对湿度,单位:%
	float m_fRH;
	// 大气压力,单位:kPa
	float m_fAP;

	// 版本
	float m_fVersion;

	// 发动机冲程
	enum COpa::EngineStrokeType m_est;
	// 转速测量方式
	enum COpa::RpmMeasurementType m_rmt;
	// 滤波参数
	enum COpa::FilterParamType m_fpt;

	// 补偿温度值
	float m_fCorrectedOilTemp;

	// 上次操作时刻
	clock_t m_LastOPTime;

private:
	// NHT1L操作类
	COpa* m_pOpa;
	// 设备型号
	InstrumentModel m_emInstrumentModel;
	// 线程退出标志
	bool m_bQuitThread;
	// 串口操作状态
	DWORD m_dwStatus;
};


