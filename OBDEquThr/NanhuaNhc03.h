/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaNhc03.h
* 文件标识：
* 摘要：南华测功机NHC03系列通讯类，继承与CDyn
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-12-09
*
* 历史信息：
*
*
* 当前版本：1.0.6
* 日期：2017-07-26
* 作者：Qin
* 描述：根据关于测功机升级秒表计时范围的说明进行升级
*
* 当前版本：1.0.5
* 日期：2010-08-20
* 作者：Qin
* 描述：加入与设置对应的获取电机最大速度调用函数
*
* 当前版本：1.0.4
* 日期：2010-07-21
* 作者：Qin
* 描述：加入设置电机最大速度调用函数
*
* 版本：1.0.3
* 日期：2010-06-09
* 作者：Cui
* 描述：修正以下Ap、Bp、Cp的读写函数：
* 	DWORD ReadApWithRemanence(float *const pfAp);
* 	DWORD ReadBpWithRemanence(float *const pfBp);
* 	DWORD ReadCpWithRemanence(float *const pfCp);
* 	DWORD WriteApWithRemanence(const float fAp);
* 	DWORD WriteBpWithRemanence(const float fBp);
* 	DWORD WriteCpWithRemanence(const float fCp);
* 	DWORD ReadApWithoutRemanence(float *const pfAp);
* 	DWORD ReadBpWithoutRemanence(float *const pfBp);
* 	DWORD ReadCpWithoutRemanence(float *const pfCp);
* 	DWORD WriteApWithoutRemanence(const float fAp);
* 	DWORD WriteBpWithoutRemanence(const float fBp);
* 	DWORD WriteCpWithoutRemanence(const float fCp);
*
* 当前版本：1.0.2
* 完成日期：2010-06-04
* 作者：Qin
* 描述：加入变载荷控制模式调用函数
*
* 当前版本：1.0.1
* 完成日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 当前版本：1.0.0
* 完成日期：2009-12-09
* 作者：Cui
* 描述：正式发布使用
*/

#pragma once

#ifndef NANHUA_NHC03_H
#define NANHUA_NHC03_H

#include "Dyn.h"

class CNanhuaNhc03 : public CDyn
{
public:
	CNanhuaNhc03(void);
	~CNanhuaNhc03(void);

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// 返回打开结果：
	// DYN_OPEN_SUCCEED
	// DYN_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// DYN_CLOSE_SUCCEED
	// DYN_CLOSE_FAIL_WIN32API
	// DYN_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是否打开
	bool IsOpen(void) const;

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
	DWORD GetRealTimeData(float *const pfP=NULL, float *const pfF=NULL, float *const pfV=NULL);

	// 取状态
	// (out)pbs16Status：状态位指针，共16位，数据位对应的含义请参看通讯说明书，可以为空指针（用于通讯测试）
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetStatus(std::bitset<16> *const pbs16Status = NULL);

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

	// 设置恒加载步长（分辨率0.01km/h）
	DWORD SetConstantAccStep(const float fStep);

	// 启动恒加载控制模式
	DWORD ConstantAccControlModeOn(void);

	// 设置恒力目标值
	// (in)fFTarget：恒力目标值，单位-N
	DWORD SetConstantForceTarget(const float fFTarget);

	// 启动恒力控制模式
	DWORD ConstantForceControlModeOn(void);

	//　启动变载荷模式
	DWORD VarLoadControlModeOn(void);

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
	// (out)fMaxVelocity;最大速度 单位-km/h
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

	// 设置基本惯量
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
	////////////////////////////////////// 寄生功率有关end

	////////////////////////////////////// 响应时间有关begin
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
	DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength =10);
	////////////////////////////////////// 响应时间有关end

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作end 

private:
	// 写读串口数据
	// 写入相应的命令，读取对应的返回数据
	// 并根据需要，分别对写入和读取的数据进行校验和验证
	// (in)pbWriteBuffer：要写入的数据内容指针，不能为空指针
	// (in)dwWriteLength：要写入的数据长度
	// (out)pbReadBuffer：要读取的数据内容指针，不能为空指针
	// (in)dwReadLength：要读取的数据长度
	// (in)bNeedWriteChkSum：true，需要写校验，false：不需要
	// (in)bNeedReadChkSum：true，需要读校验，false：不需要
	// 返回，函数执行结果：
	// NHC_WR_FUNC_SUCCEED
	// NHC_WR_FUNC_PORT_NOT_OPEN
	// NHC_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHC_WR_FUNC_EQUIP_RETURE_0X15
	// NHC_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const bool bNeedWriteChkSum=true, const bool bNeedReadChkSum=true);

	// 计算校验和
	// 相加，然后取反加一
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	BYTE CheckSum(const BYTE *const p, const UINT un);

private:
	// 转换float与word，用于Ap，Bp，Cp的写读
	static void ConvertFloatToWord(const float fSource, WORD *const pwDestination);
	static void ConvertWordToFloat(const WORD wSource, float *const pfDestination);

private:
	// 串口句柄
	HANDLE m_hComm;
	//客户信息临界保护量
	CRITICAL_SECTION cInfoSection;
};

#endif