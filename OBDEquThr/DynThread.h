/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：DynThread.h
* 文件标识：
* 摘要：测功机线程
*
* 版本：1.0
* 作者：Cui
* 日期：2009-12-10
*
* 历史信息：
*
*
* 版本：1.0.6
* 日期：2014-06-02
* 作者：Qin
* 描述：加入可以恒加速控制模式
*
* 版本：1.0.5
* 日期：2010-08-20
* 作者：Qin
* 描述：加入与设置对应的获取电机最大速度调用函数
*
* 版本：1.0.4
* 日期：2010-07-21
* 作者：Qin
* 描述：加入设置电机最大速度调用函数
*
* 版本：1.0.3
* 日期：2010-07-06
* 作者：Cui
* 描述：加入对马哈测功机的支持

* 版本：1.0.2
* 日期：2010-06-04
* 作者：Qin
* 描述：加入变载荷控制模式调用函数
*
* 版本：1.0.1
* 日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 版本：1.0.0
* 日期：2009-12-10
* 作者：Cui
* 描述：正式发布使用
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaNhc03.h"
#include "MahaASMP.h"

// CDynThread

class AFX_EXT_CLASS CDynThread : public CWinThread
{
	DECLARE_DYNCREATE(CDynThread)

protected:
	CDynThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CDynThread();

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
		// 南华NHC03
		IM_NANHUA_NHC03,
		// 马哈ASM-P
		IM_MAHA_ASMP,
	};

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// 返回打开结果：
	// DYN_OPEN_SUCCEED
	// DYN_OPEN_FAIL
	DWORD Open(const BYTE bPort, const enum InstrumentModel im);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// DYN_CLOSE_SUCCEED
	// DYN_CLOSE_FAIL_WIN32API
	// DYN_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是否打开
	inline bool IsOpen(void) const
	{
		return m_pDyn->IsOpen();
	}
	// 获取串口操作状态
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作start 
	// 取实时数据
	// (out)pfP：功率指针，单位kW，可以为空指针
	// (out)pfF：力指针，单位N，可以为空指针
	// (out)pfV：速度指针，单位km/h，可以为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetRealTimeData(float *const pfP, float *const pfF, float *const pfV);
	inline DWORD GetRealTimeData(void)
	{
		return m_pDyn->GetRealTimeData(&m_fP, &m_fF, &m_fV);
	}

	// 取状态
	// (out)pbs16Status：状态位指针，共16位，数据位对应的含义请参看通讯说明书，可以为空指针（用于通讯测试）
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetStatus(std::bitset<16> *const pbs16Status);
	inline DWORD GetStatus(void)
	{
		return m_pDyn->GetStatus(&m_btStatus);
	}

	// 读变量
	// (in)bVarNumber：要读取的变量号，各个变量对应的变量号请参考通讯说明书
	// (out)pwVarValue：读取到的变量值，含义请参考通讯说明书，不能为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ReadVar(const BYTE bVarNumber, WORD *const pwVarValue);
	inline DWORD ReadVar(const BYTE bVarNumber)
	{
		return m_pDyn->ReadVar(bVarNumber, &(m_wVarValues[bVarNumber]));
	}

	// 写变量
	// (in)bVarNumber：要写入的变量号，各个变量对应的变量号请参考通讯说明书
	// (in)pwVarValue：要写入的变量值，内容含义请参考通讯说明书
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteVar(const BYTE bVarNumber, const WORD wVarValue);

	// 发送控制字
	// (in)bCtrlWord：控制字，控制字内容及代表含义请参考通讯说明书
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD SendCtrlWord(const BYTE bCtrlWord);

	// 升起举升器
	DWORD LiftBeamUp(void);

	// 降下举升器
	DWORD LiftBeamDown(void);

	// 启动电机
	DWORD MotorOn(void);

	// 停止电机
	DWORD MotorOff(void);

	// 退出控制模式
	DWORD ControlModeOff(void);

	// 启动ASM加载模式
	DWORD AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses);

	// 设置恒功率目标值
	// (in)fPTarget：恒功率目标值，单位-kW
	DWORD SetConstantPowerTarget(const float fPTarget);

	// 启动恒功率控制模式
	DWORD ConstantPowerControlModeOn(void);

	// 启动变载荷控制模式
	DWORD VarLoadControlModeOn(void);

	// 设置恒力目标值
	// (in)fFTarget：恒力目标值，单位-N
	DWORD SetConstantForceTarget(const float fFTarget);

	// 启动恒力控制模式
	DWORD ConstantForceControlModeOn(void);

	// 设置恒速度目标值
	// (in)fVTarget：恒速度目标值，单位-km/h
	DWORD SetConstantVelocityTarget(const float fVTarget);

	// 启动恒速度控制
	DWORD ConstantVelocityControlModeOn(void);

	// 设置秒表1开始速度
	// (in)fUpperVelocity：开始速度，单位-km/h
	DWORD SetUpperVelocityOfTimer1(const float fUpperVelocity);

	// 设置秒表1中间速度
	// (in)fMiddleVelocity：中间速度，单位-km/h
	DWORD SetMiddleVelocityOfTimer1(const float fMiddleVelocity);

	// 设置秒表1停止速度
	// (in)fLowerVelocity：停止速度，单位-km/h
	DWORD SetLowerVelocityOfTimer1(const float fLowerVelocity);

	// 启动秒表1
	DWORD SetTimer1(void);

	// 关闭秒表1
	DWORD KillTimer1(void);

	// 获取秒表1时间
	// (out)punTime_MS;时间指针，单位-ms
	DWORD GetTimeOfTimer1(UINT *const punTime_MS);

	// 获取秒表1中间时间
	// (out)punTime_MS;时间指针，单位-ms
	DWORD GetMiddleTimeOfTimer1(UINT *const punTime_MS);

	// 设置秒表2开始速度
	// (in)fUpperVelocity：开始速度，单位-km/h
	DWORD SetUpperVelocityOfTimer2(const float fUpperVelocity);

	// 设置秒表2停止速度
	// (in)fLowerVelocity：停止速度，单位-km/h
	DWORD SetLowerVelocityOfTimer2(const float fLowerVelocity);

	// 设置电机的最大速度
	// (in)fMaxVelocity; 最大速度，单位-km/h
	DWORD SetMotorMaxVelocity(const float fMaxVelocity);

	// 获取电机的最大速度
	// (out)fMaxVelocity; 最大速度，单位-km/h
	DWORD GetMotorMaxVelocity(float *const fMaxVelocity);

	// 启动秒表2
	DWORD SetTimer2(void);

	// 关闭秒表2
	DWORD KillTimer2(void);

	// 获取秒表2时间
	// (out)punTime_MS;时间指针，单位-ms
	DWORD GetTimeOfTimer2(UINT *const punTime_MS);

	// 获取基本惯量
	// (out)pusDIW：基本惯量指针，单位-kg
	DWORD GetDIW(USHORT *const pusDIW);

	// 写入基本惯量
	DWORD SetDIW(const USHORT usDIW);

	//////////////////////////////////////// 驱动阻力控制模式相关begin
	// 写驱动阻力模拟控制模式的车辆基准质量[kg]
	// (in)车辆基准质量,单位-kg
	DWORD WriteVehicleRM(const USHORT usVehicleRM);
	// 写驱动阻力模拟控制模式的Cv
	// 内部将会转化为80km/h对应的值
	// (in)Cv值，50km/h对应的值，单位:kW
	DWORD WriteCv(const float fCv);
	// 写驱动阻力模拟控制模式的Bv
	// (in)Bv值，单位:kW
	DWORD WriteBv(const float fBv);
	// 写驱动阻力模拟控制模式的Av
	// (in)Av值，单位:kW
	DWORD WriteAv(const float fAv);
	// 写驱动阻力模拟控制模式的At
	// (in)At值，单位:kW
	DWORD WriteAt(const float fAt);
	// 写驱动阻力模拟控制模式的Bt
	// (in)Bt值，单位:kW
	DWORD WriteBt(const float fBt);
	// 写驱动阻力模拟控制模式的Ct
	// (in)Ct值，单位:kW
	DWORD WriteCt(const float fCt);
	// 启动驱动阻力模拟控制模式
	DWORD DriveResistanceSimulationControlModeOn(void);
	//////////////////////////////////////// 驱动阻力控制模式相关end

	// 重设行驶距离
	DWORD ResetDistance(void);
	// 读取行驶距离
	// (out)pusDistance:距离指针,单位:米(m)
	DWORD ReadDistance(USHORT *const pusDistance);
	// (out)pfDynMaxPower:测功机最大加载功率[kw]
	DWORD ReadDynMaxPower(float *const pfDynMaxPower);

	////////////////////////////////////// 寄生功率有关begin
	// 读取显示Ap(未去除剩磁力)
	// (out)fAp:功率,单位:kW
	DWORD ReadApWithRemanence(float *const pfAp);
	// 读取显示Bp(未去除剩磁力)
	// (out)fBp:功率,单位:kW
	DWORD ReadBpWithRemanence(float *const pfBp);
	// 读取显示Cp(未去除剩磁力)
	// (out)fCp:功率,单位:kW
	DWORD ReadCpWithRemanence(float *const pfCp);
	// 写入显示Ap(未去除剩磁力)
	// (in)fAp:功率，单位:kW
	DWORD WriteApWithRemanence(const float fAp);
	// 写入显示Bp(未去除剩磁力)
	// (in)fBp:功率，单位:kW
	 DWORD WriteBpWithRemanence(const float fBp);
	// 写入显示Cp(未去除剩磁力)
	// (in)fCp:功率，单位:kW
	DWORD WriteCpWithRemanence(const float fCp);
	// 读取真实Ap(去除剩磁力后)
	// (out)fAp:功率,单位:kW
	DWORD ReadApWithoutRemanence(float *const pfAp);
	// 读取真实Bp(去除剩磁力后)
	// (out)fBp:功率,单位:kW
	DWORD ReadBpWithoutRemanence(float *const pfBp);
	// 读取真实Cp(去除剩磁力后)
	// (out)fCp:功率,单位:kW
	DWORD ReadCpWithoutRemanence(float *const pfCp);
	// 写入真实Ap(去除剩磁力后)
	// (in)fAp:功率，单位:kW
	DWORD WriteApWithoutRemanence(const float fAp);
	// 写入真实Bp(去除剩磁力后)
	// (in)fBp:功率，单位:kW
	DWORD WriteBpWithoutRemanence(const float fBp);
	// 写入真实Cp(去除剩磁力后)
	// (in)fCp:功率，单位:kW
	DWORD WriteCpWithoutRemanence(const float fCp);
	// 设置恒加载步长（分辨率0.01km/h）
	DWORD SetConstantAccStep(const float fStep);
	// 启动恒加载控制模式
	DWORD ConstantAccControlModeOn(void);
	////////////////////////////////////// 寄生功率有关end

	////////////////////////////////////// 响应时间相关Begin
	// 开始加载响应时间控制模式
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ResponseTimeControlModeStart(void);
	// 停止加载响应时间控制模式
	DWORD ResponseTimeControlModeEnd(void);

	// 取过程数据
	// (out)pfF：力指针，单位N，可以为空指针
	// (out)pfV：速度指针，单位[10m/h]，可以为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength=NULL);
	////////////////////////////////////// 响应时间相关End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作end 


public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器基本操作（异步）begin 
	// 对应同步GetRealTimeData
	void GetRealTimeData_Asyn(HWND hCallBackWnd=NULL);
	// 对应同步GetStatus
	void GetStatus_Asyn(HWND hCallBackWnd=NULL);
	// 对应同步ReadVar
	void ReadVar_Asyn(const BYTE bVarNumber, HWND hCallBackWnd=NULL);
	// 对应同步WriteVar
	void WriteVar_Asyn(const BYTE bVarNumber, const WORD wVarValue, HWND hCallBackWnd=NULL);
	// 对应同步SendCtrlWord
	void SendCtrlWord_Asyn(const BYTE bCtrlWord, HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器基本操作（异步）end 

public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////发布数据begin
	// 获取功率实时值
	// return：功率实时值，单位-kW
	inline float GetPValue(void) const
	{
		return m_fP;
	}
	// 获取力实时值
	// return：力实时值，单位-N
	inline float GetFValue(void) const
	{
		return m_fF;
	}
	// 获取速度实时值
	// return：速度实时值，单位-km/h
	inline float GetVValue(void) const
	{
		return m_fV;
	}
	// 获取读写状态
	inline DWORD GetWRSta(void) const
	{
		return m_dwWRSta;
	}
	// 指定状态位是否处于有信号状态
	// (in)bStatusBitNumber：状态位号，具体每个状态位的含义请查看通讯协议，取值范围只能是0~15
	// return：如果该状态为1，返回true，否则，返回false
	inline bool IsTheStatusBitSignaled(const BYTE bStatusBitNumber) const
	{
		ASSERT(bStatusBitNumber < 16);

		return m_btStatus.test(bStatusBitNumber);
	}
	// 获取指定变量值
	// (in)bVarNumber：变量号
	// return：变量值
	// 变量号和变量值对应的含义请查看通讯协议
	inline WORD GetVarValue(const BYTE bVarNumber) const
	{
		return m_wVarValues[bVarNumber];
	}
	//inline float GetDynMaxPower() const
	//{
	//	return m_wVarValues[68]/100.0f;
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////发布数据end


private:
	// 响应异步操作函数
	afx_msg void OnGetRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReadVar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWriteVar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSendCtrlWord(WPARAM wParam, LPARAM lParam);

public:
	// 退出线程操作
	void QuitThread(void);

private:
	// 初始化设备
	void InitializeInstrument(const enum InstrumentModel im);
	// 卸载设备
	void UninitializeInstrument(void);

protected:
	float m_fP;
	float m_fF;
	float m_fV;
	std::bitset<16> m_btStatus;
	WORD m_wVarValues[250];
	// 写入变量,异步时的中间变量
	WORD m_wVarValuesForWrite[250];
	DWORD m_dwWRSta; // 读写状态

private:
	// 测功机操作类
	CDyn *m_pDyn;
	// 线程退出标志
	bool m_bQuitThread;
	// 串口操作状态
	DWORD m_dwStatus;
};


