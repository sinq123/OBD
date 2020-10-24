/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：Dyn.h
* 文件标识：
* 摘要：测功机抽象类
*		以纯虚拟函数形式定义测功机操作接口
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-12-09
*
* 历史信息：
*
* 版本：1.0.5
* 日期：2014-06-02
* 作者：Qin
* 描述：加入可以恒加速控制模式

* 当前版本：1.0.4
* 日期：2010-08-20
* 作者：Qin
* 描述：加入与设置对应的获取电机最大速度调用函数
*
* 当前版本：1.0.3
* 日期：2010-07-21
* 作者：Qin
* 描述：加入设置电机最大速度调用接口
*
* 当前版本：1.0.2
* 完成日期：2010-06-04
* 作者：Qin
* 描述：加入变载荷控制模式调用接口
*
* 当前版本：1.0.1
* 完成日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”

* 当前版本：1.0.0
* 完成日期：2009-12-09
* 作者：Cui
* 描述：正式发布使用
*/

#pragma once


#ifndef DYN_H
#define DYN_H

// 函数执行结果
#define DYN_OPEN_SUCCEED							0x00  // 打开串口成功
#define DYN_OPEN_FAIL								0x01  // 打开串口失败
#define DYN_CLOSE_SUCCEED							0x00  // 关闭串口成功
#define DYN_CLOSE_FAIL_WIN32API						0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define DYN_CLOSE_FAIL_NOOPENPORT					0x02  // 关闭串口失败：没有可以关闭的串口
#define DYN_WR_SUCCEED								0x00  // 写读成功
#define DYN_WR_PORT_NOT_OPEN						0x01  // 写读失败：串口未打开
#define DYN_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // 写读失败：写指定数据失败
#define DYN_WR_READ_SPECIFIED_DATA_FAIL				0x03  // 写读失败：读指定数据失败
#define DYN_WR_EQUIP_RETURE_0X15					0x04  // 写读失败：仪器返回错误代码（0x15）
#define DYN_WR_CHECK_SUM_FAIL						0x05  // 写读失败：读出数据校验和失败
#define DYN_NOT_SUPPORT								0x06  // 不支持的操作
#define DYN_WR_UNKNOWN_ERROR						0xFF  // 写读失败：未知错误

#include <bitset>

class CDyn
{
public:
	CDyn(void);
	virtual ~CDyn(void);

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// 返回打开结果：
	// DYN_OPEN_SUCCEED
	// DYN_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// DYN_CLOSE_SUCCEED
	// DYN_CLOSE_FAIL_WIN32API
	// DYN_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// 判断串口是否打开
	virtual bool IsOpen(void) const = 0;

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
	virtual DWORD GetRealTimeData(float *const pfP=NULL, float *const pfF=NULL, float *const pfV=NULL) = 0;

	// 取状态
	// (out)pbs16Status：状态位指针，共16位，数据位对应的含义请参看通讯说明书，可以为空指针（用于通讯测试）
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD GetStatus(std::bitset<16> *const pbs16Status = NULL) = 0;

	// 读变量
	// (in)bVarNumber：要读取的变量号，各个变量对应的变量号请参考通讯说明书
	// (out)pwVarValue：读取到的变量值，含义请参考通讯说明书，不能为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD ReadVar(const BYTE bVarNumber, WORD *const pwVarValue) = 0;

	// 写变量
	// (in)bVarNumber：要写入的变量号，各个变量对应的变量号请参考通讯说明书
	// (in)pwVarValue：要写入的变量值，内容含义请参考通讯说明书
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD WriteVar(const BYTE bVarNumber, const WORD wVarValue) = 0;

	// 发送控制字
	// (in)bCtrlWord：控制字，控制字内容及代表含义请参考通讯说明书
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD SendCtrlWord(const BYTE bCtrlWord) = 0;

	// 升起举升器
	virtual DWORD LiftBeamUp(void) = 0;

	// 降下举升器
	virtual DWORD LiftBeamDown(void) = 0;

	// 启动电机
	virtual DWORD MotorOn(void) = 0;

	// 停止电机
	virtual DWORD MotorOff(void) = 0;

	// 退出控制模式
	virtual DWORD ControlModeOff(void) = 0;

	// 启动ASM加载模式
	virtual DWORD AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses) = 0;

	// 设置恒功率目标值
	// (in)fPTarget：恒功率目标值，单位-kW
	virtual DWORD SetConstantPowerTarget(const float fPTarget) = 0;

	// 启动恒功率控制模式
	virtual DWORD ConstantPowerControlModeOn(void) = 0;

	// 启动变载荷控制模式
	virtual DWORD VarLoadControlModeOn(void) = 0;

	// 设置恒力目标值
	// (in)fFTarget：恒力目标值，单位-N
	virtual DWORD SetConstantForceTarget(const float fFTarget) = 0;

	// 启动恒力控制模式
	virtual DWORD ConstantForceControlModeOn(void) = 0;

	// 设置恒速度目标值
	// (in)fVTarget：恒速度目标值，单位-km/h
	virtual DWORD SetConstantVelocityTarget(const float fVTarget) = 0;

	// 启动恒速度控制
	virtual DWORD ConstantVelocityControlModeOn(void) = 0;

	// 设置秒表1开始速度
	// (in)fUpperVelocity：开始速度，单位-km/h
	virtual DWORD SetUpperVelocityOfTimer1(const float fUpperVelocity) = 0;

	// 设置秒表1中间速度
	// (in)fMiddleVelocity：中间速度，单位-km/h
	virtual DWORD SetMiddleVelocityOfTimer1(const float fMiddleVelocity) = 0;

	// 设置秒表1停止速度
	// (in)fLowerVelocity：停止速度，单位-km/h
	virtual DWORD SetLowerVelocityOfTimer1(const float fLowerVelocity) = 0;

	// 设置测功机电机最大速度
	// (in)fMaxVelocity; 最大速度，单位-km/h
	virtual DWORD SetMotorMaxVelocity(const float fMaxVelocity) = 0;

	// 获取测功机电机最大速度
	// (out)fMaxVelocity; 最大速度，单位-km/h
    virtual DWORD GetMotorMaxVelocity(float *const fMaxVelocity) = 0;

	// 启动秒表1
	virtual DWORD SetTimer1(void) = 0;

	// 关闭秒表1
	virtual DWORD KillTimer1(void) = 0;

	// 获取秒表1时间
	// (out)punTime_MS;时间指针，单位-ms
	virtual DWORD GetTimeOfTimer1(UINT *const punTime_MS) = 0;

	// 获取秒表1中间时间
	// (out)punTime_MS;时间指针，单位-ms
	virtual DWORD GetMiddleTimeOfTimer1(UINT *const punTime_MS) = 0;

	// 设置秒表2开始速度
	// (in)fUpperVelocity：开始速度，单位-km/h
	virtual DWORD SetUpperVelocityOfTimer2(const float fUpperVelocity) = 0;

	// 设置秒表2停止速度
	// (in)fLowerVelocity：停止速度，单位-km/h
	virtual DWORD SetLowerVelocityOfTimer2(const float fLowerVelocity) = 0;

	// 启动秒表2
	virtual DWORD SetTimer2(void) = 0;

	// 关闭秒表2
	virtual DWORD KillTimer2(void) = 0;

	// 获取秒表2时间
	// (out)punTime_MS;时间指针，单位-ms
	virtual DWORD GetTimeOfTimer2(UINT *const punTime_MS) = 0;

	// 获取基本惯量
	// (out)pusDIW：基本惯量指针，单位-kg
	virtual DWORD GetDIW(USHORT *const pusDIW) = 0;

	// 写入基本惯量
	virtual DWORD SetDIW(const USHORT usDIW) = 0;

	//////////////////////////////////////// 驱动阻力控制模式相关begin
	// 写驱动阻力模拟控制模式的车辆基准质量[kg]
	// (in)车辆基准质量,单位-kg
	virtual DWORD WriteVehicleRM(const USHORT usVehicleRM) = 0;
	// 写驱动阻力模拟控制模式的Cv
	// 内部将会转化为80km/h对应的值
	// (in)Cv值，50km/h对应的值，单位:kW
	virtual DWORD WriteCv(const float fCv) = 0;
	// 写驱动阻力模拟控制模式的Bv
	// (in)Bv值，单位:kW
	virtual DWORD WriteBv(const float fBv) = 0;
	// 写驱动阻力模拟控制模式的Av
	// (in)Av值，单位:kW
	virtual DWORD WriteAv(const float fAv) = 0;
	// 写驱动阻力模拟控制模式的At
	// (in)At值，单位:kW
	virtual DWORD WriteAt(const float fAt) = 0;
	// 写驱动阻力模拟控制模式的Bt
	// (in)Bt值，单位:kW
	virtual DWORD WriteBt(const float fBt) = 0;
	// 写驱动阻力模拟控制模式的Ct
	// (in)Ct值，单位:kW
	virtual DWORD WriteCt(const float fCt) = 0;
	// 启动驱动阻力模拟控制模式
	virtual DWORD DriveResistanceSimulationControlModeOn(void) = 0;
	//////////////////////////////////////// 驱动阻力控制模式相关end

	// 重设行驶距离
	virtual DWORD ResetDistance(void) = 0;
	// 读取行驶距离
	// (out)pusDistance:距离指针,单位:米(m)
	virtual DWORD ReadDistance(USHORT *const pusDistance) = 0;


	////////////////////////////////////// 寄生功率有关begin
	// 读取显示Ap(未去除剩磁力)
	// (out)fAp:功率,单位:kW
	virtual DWORD ReadApWithRemanence(float *const pfAp) = 0;
	// 读取显示Bp(未去除剩磁力)
	// (out)fBp:功率,单位:kW
	virtual DWORD ReadBpWithRemanence(float *const pfBp) = 0;
	// 读取显示Cp(未去除剩磁力)
	// (out)fCp:功率,单位:kW
	virtual DWORD ReadCpWithRemanence(float *const pfCp) = 0;
	// 写入显示Ap(未去除剩磁力)
	// (in)fAp:功率，单位:kW
	virtual DWORD WriteApWithRemanence(const float fAp) = 0;
	// 写入显示Bp(未去除剩磁力)
	// (in)fBp:功率，单位:kW
	virtual DWORD WriteBpWithRemanence(const float fBp) = 0;
	// 写入显示Cp(未去除剩磁力)
	// (in)fCp:功率，单位:kW
	virtual DWORD WriteCpWithRemanence(const float fCp) = 0;
	// 读取真实Ap(去除剩磁力后)
	// (out)fAp:功率,单位:kW
	virtual DWORD ReadApWithoutRemanence(float *const pfAp) = 0;
	// 读取真实Bp(去除剩磁力后)
	// (out)fBp:功率,单位:kW
	virtual DWORD ReadBpWithoutRemanence(float *const pfBp) = 0;
	// 读取真实Cp(去除剩磁力后)
	// (out)fCp:功率,单位:kW
	virtual DWORD ReadCpWithoutRemanence(float *const pfCp) = 0;
	// 写入真实Ap(去除剩磁力后)
	// (in)fAp:功率，单位:kW
	virtual DWORD WriteApWithoutRemanence(const float fAp) = 0;
	// 写入真实Bp(去除剩磁力后)
	// (in)fBp:功率，单位:kW
	virtual DWORD WriteBpWithoutRemanence(const float fBp) = 0;
	// 写入真实Cp(去除剩磁力后)
	// (in)fCp:功率，单位:kW
	virtual DWORD WriteCpWithoutRemanence(const float fCp) = 0;
	// 设置恒加载步长（分辨率0.01km/h）
	virtual DWORD SetConstantAccStep(const float fStep) = 0;
	// 启动恒加载控制模式
	virtual DWORD ConstantAccControlModeOn(void) = 0;
	////////////////////////////////////// 寄生功率有关end

	////////////////////////////////////// 响应时间相关Begin
	// 开始加载响应时间控制模式
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD ResponseTimeControlModeStart(void)=0;
	// 停止加载响应时间控制模式
	virtual DWORD ResponseTimeControlModeEnd(void)=0;

	// 取过程数据
	// (out)pfF：力指针，单位N，可以为空指针
	// (out)pfV：速度指针，单位[10m/h]，可以为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength=NULL)=0;
	////////////////////////////////////// 响应时间相关End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作end 
};

#endif
