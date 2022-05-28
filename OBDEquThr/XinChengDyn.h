
#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef XinChengDyn_H
#define XinChengDyn_H

#include "Dyn.h"
#include <stdint.h>

class CXinChengDyn: public CDyn
{
public:
	CXinChengDyn(void);
	~CXinChengDyn(void);


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
	DWORD GetStatus(std::bitset<16> *const pbs16Status = NULL){return DYN_NOT_SUPPORT;}

	// 读变量
	// (in)bVarNumber：要读取的变量号，各个变量对应的变量号请参考通讯说明书
	// (out)pwVarValue：读取到的变量值，含义请参考通讯说明书，不能为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ReadVar(const BYTE bVarNumber, WORD *const pwVarValue){return DYN_NOT_SUPPORT;}

	// 写变量
	// (in)bVarNumber：要写入的变量号，各个变量对应的变量号请参考通讯说明书
	// (in)pwVarValue：要写入的变量值，内容含义请参考通讯说明书
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteVar(const BYTE bVarNumber, const WORD wVarValue){return DYN_NOT_SUPPORT;}

	// 发送控制字
	// (in)bCtrlWord：控制字，控制字内容及代表含义请参考通讯说明书
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD SendCtrlWord(const BYTE bCtrlWord){return DYN_NOT_SUPPORT;}

	// 升起举升器
	DWORD LiftBeamUp(void);

	// 降下举升器
	DWORD LiftBeamDown(void);

	// 启动电机
	DWORD MotorOn(void);

	// 停止电机
	DWORD MotorOff(void);

	// 退出控制模式
	DWORD ControlModeOff(void){return DYN_NOT_SUPPORT;}

	// 启动ASM加载模式
	DWORD AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses){return DYN_NOT_SUPPORT;}

	// 设置恒功率目标值
	// (in)fPTarget：恒功率目标值，单位-kW
	DWORD SetConstantPowerTarget(const float fPTarget);

	// 启动恒功率控制模式
	DWORD ConstantPowerControlModeOn(void);

	// 启动变载荷控制模式
	DWORD VarLoadControlModeOn(void){return DYN_NOT_SUPPORT;}

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
	DWORD SetUpperVelocityOfTimer1(const float fUpperVelocity){return DYN_NOT_SUPPORT;}

	// 设置秒表1中间速度
	// (in)fMiddleVelocity：中间速度，单位-km/h
	DWORD SetMiddleVelocityOfTimer1(const float fMiddleVelocity){return DYN_NOT_SUPPORT;}

	// 设置秒表1停止速度
	// (in)fLowerVelocity：停止速度，单位-km/h
	DWORD SetLowerVelocityOfTimer1(const float fLowerVelocity){return DYN_NOT_SUPPORT;}

	// 设置测功机电机最大速度
	// (in)fMaxVelocity; 最大速度，单位-km/h
	DWORD SetMotorMaxVelocity(const float fMaxVelocity){return DYN_NOT_SUPPORT;}

	// 获取测功机电机最大速度
	// (out)fMaxVelocity; 最大速度，单位-km/h
    DWORD GetMotorMaxVelocity(float *const fMaxVelocity){return DYN_NOT_SUPPORT;}

	// 启动秒表1
	DWORD SetTimer1(void){return DYN_NOT_SUPPORT;}

	// 关闭秒表1
	DWORD KillTimer1(void){return DYN_NOT_SUPPORT;}

	// 获取秒表1时间
	// (out)punTime_MS;时间指针，单位-ms
	DWORD GetTimeOfTimer1(UINT *const punTime_MS){return DYN_NOT_SUPPORT;}

	// 获取秒表1中间时间
	// (out)punTime_MS;时间指针，单位-ms
	DWORD GetMiddleTimeOfTimer1(UINT *const punTime_MS){return DYN_NOT_SUPPORT;}

	// 设置秒表2开始速度
	// (in)fUpperVelocity：开始速度，单位-km/h
	DWORD SetUpperVelocityOfTimer2(const float fUpperVelocity){return DYN_NOT_SUPPORT;}

	// 设置秒表2停止速度
	// (in)fLowerVelocity：停止速度，单位-km/h
	DWORD SetLowerVelocityOfTimer2(const float fLowerVelocity){return DYN_NOT_SUPPORT;}

	// 启动秒表2
	DWORD SetTimer2(void){return DYN_NOT_SUPPORT;}

	// 关闭秒表2
	DWORD KillTimer2(void){return DYN_NOT_SUPPORT;}

	// 获取秒表2时间
	// (out)punTime_MS;时间指针，单位-ms
	DWORD GetTimeOfTimer2(UINT *const punTime_MS){return DYN_NOT_SUPPORT;}

	// 获取基本惯量
	// (out)pusDIW：基本惯量指针，单位-kg
	DWORD GetDIW(USHORT *const pusDIW){return DYN_NOT_SUPPORT;}

	// 写入基本惯量
	DWORD SetDIW(const USHORT usDIW){return DYN_NOT_SUPPORT;}

	//////////////////////////////////////// 驱动阻力控制模式相关begin
	// 写驱动阻力模拟控制模式的车辆基准质量[kg]
	// (in)车辆基准质量,单位-kg
	DWORD WriteVehicleRM(const USHORT usVehicleRM){return DYN_NOT_SUPPORT;}
	// 写驱动阻力模拟控制模式的Cv
	// 内部将会转化为80km/h对应的值
	// (in)Cv值，50km/h对应的值，单位:kW
	DWORD WriteCv(const float fCv){return DYN_NOT_SUPPORT;}
	// 写驱动阻力模拟控制模式的Bv
	// (in)Bv值，单位:kW
	DWORD WriteBv(const float fBv){return DYN_NOT_SUPPORT;}
	// 写驱动阻力模拟控制模式的Av
	// (in)Av值，单位:kW
	DWORD WriteAv(const float fAv){return DYN_NOT_SUPPORT;}
	// 写驱动阻力模拟控制模式的At
	// (in)At值，单位:kW
	DWORD WriteAt(const float fAt){return DYN_NOT_SUPPORT;}
	// 写驱动阻力模拟控制模式的Bt
	// (in)Bt值，单位:kW
	DWORD WriteBt(const float fBt){return DYN_NOT_SUPPORT;}
	// 写驱动阻力模拟控制模式的Ct
	// (in)Ct值，单位:kW
	DWORD WriteCt(const float fCt){return DYN_NOT_SUPPORT;}
	// 启动驱动阻力模拟控制模式
	DWORD DriveResistanceSimulationControlModeOn(void){return DYN_NOT_SUPPORT;}
	//////////////////////////////////////// 驱动阻力控制模式相关end

	// 重设行驶距离
	DWORD ResetDistance(void){return DYN_NOT_SUPPORT;}
	// 读取行驶距离
	// (out)pusDistance:距离指针,单位:米(m)
	DWORD ReadDistance(USHORT *const pusDistance){return DYN_NOT_SUPPORT;}


	////////////////////////////////////// 寄生功率有关begin
	// 读取显示Ap(未去除剩磁力)
	// (out)fAp:功率,单位:kW
	DWORD ReadApWithRemanence(float *const pfAp){return DYN_NOT_SUPPORT;}
	// 读取显示Bp(未去除剩磁力)
	// (out)fBp:功率,单位:kW
	DWORD ReadBpWithRemanence(float *const pfBp){return DYN_NOT_SUPPORT;}
	// 读取显示Cp(未去除剩磁力)
	// (out)fCp:功率,单位:kW
	DWORD ReadCpWithRemanence(float *const pfCp){return DYN_NOT_SUPPORT;}
	// 写入显示Ap(未去除剩磁力)
	// (in)fAp:功率，单位:kW
	DWORD WriteApWithRemanence(const float fAp){return DYN_NOT_SUPPORT;}
	// 写入显示Bp(未去除剩磁力)
	// (in)fBp:功率，单位:kW
	DWORD WriteBpWithRemanence(const float fBp){return DYN_NOT_SUPPORT;}
	// 写入显示Cp(未去除剩磁力)
	// (in)fCp:功率，单位:kW
	DWORD WriteCpWithRemanence(const float fCp){return DYN_NOT_SUPPORT;}
	// 读取真实Ap(去除剩磁力后)
	// (out)fAp:功率,单位:kW
	DWORD ReadApWithoutRemanence(float *const pfAp){return DYN_NOT_SUPPORT;}
	// 读取真实Bp(去除剩磁力后)
	// (out)fBp:功率,单位:kW
	DWORD ReadBpWithoutRemanence(float *const pfBp){return DYN_NOT_SUPPORT;}
	// 读取真实Cp(去除剩磁力后)
	// (out)fCp:功率,单位:kW
	DWORD ReadCpWithoutRemanence(float *const pfCp){return DYN_NOT_SUPPORT;}
	// 写入真实Ap(去除剩磁力后)
	// (in)fAp:功率，单位:kW
	DWORD WriteApWithoutRemanence(const float fAp){return DYN_NOT_SUPPORT;}
	// 写入真实Bp(去除剩磁力后)
	// (in)fBp:功率，单位:kW
	DWORD WriteBpWithoutRemanence(const float fBp){return DYN_NOT_SUPPORT;}
	// 写入真实Cp(去除剩磁力后)
	// (in)fCp:功率，单位:kW
	DWORD WriteCpWithoutRemanence(const float fCp){return DYN_NOT_SUPPORT;}
	// 设置恒加载步长（分辨率0.01km/h）
	DWORD SetConstantAccStep(const float fStep){return DYN_NOT_SUPPORT;}
	// 启动恒加载控制模式
	DWORD ConstantAccControlModeOn(void){return DYN_NOT_SUPPORT;}
	////////////////////////////////////// 寄生功率有关end

	////////////////////////////////////// 响应时间相关Begin
	// 开始加载响应时间控制模式
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ResponseTimeControlModeStart(void){return DYN_NOT_SUPPORT;}
	// 停止加载响应时间控制模式
	DWORD ResponseTimeControlModeEnd(void){return DYN_NOT_SUPPORT;}

	// 取过程数据
	// (out)pfF：力指针，单位N，可以为空指针
	// (out)pfV：速度指针，单位[10m/h]，可以为空指针
	// 返回函数执行结果：
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength=NULL){return DYN_NOT_SUPPORT;}
	////////////////////////////////////// 响应时间相关End

	///////////////////////////////////// 新城的控制设置Begin

	//设置恒速度值(km/h) 	设置速度为70.0km/h:
	//XSS+0x00+0x00+0x8c+0x42+ 和校验码＋C

	//设置恒扭力值 (N)	设置扭力为1000.5N:
	//XSF +0x00+0x20+0x7a+0x44+ 和校验码＋C

	//设置恒功率值(kw) 	设置功率为10.1kw:
	//XSP +0x9a+0x99+0x21+0x41+ 和校验码＋C


	// 设置IGBT的pwm波占空比(%)
	// 设置pwm占空比为50%：
	// XSI +0x00+0x20+0x7a+0x44+ 和校验码＋C
	DWORD XC_SetConstantIGBT(const float fpwm);

	// 设置选用的力通道（通道1，通道2，通道3）
	// 默认值为通道1：如设置为通道1/2/3，该条命令直接用第一个数据字节来表示选用的通道。
	// XSI +0+0+0+1/2/3+ 和校验码＋C
	DWORD XC_SetConstantForceChannel(const int nChannel);

	// 力清零
	//对目前设置使用的通道进行清零：
	// XSZ+0+0+0+0+ 和校验码＋C
	DWORD XC_SetConstantForceClearing(void);

	// 清除按键值
	DWORD XC_ClearKeyValue(void);

	// 设置滚筒直径(cm)：
	// 如滚筒直径为14.736cm：
	//XCS +0xa8+0xc6+0x6b+0x41+ 和校验码＋C
	DWORD XC_SetConstantRollerDiameter(const int nRollerDim);

	// 设置编码器脉冲数(每圈)
	// 如编码器脉冲数为1000：
	//XCS +0x00+0x00+0x7a+0x44+ 和校验码＋C
	DWORD XC_SetConstantEncoderPulNum(const int nEnPulNum);

	// 设置通道1力标定系数b[0]：force=b[0]*(ad_value-c[0])
	// 设置系数b[0]=1158.4：
	// XCA +0xcd+0xcc+0x90+0x44+ 和校验码＋C
	DWORD XC_SetConstantForceCailFac_B1(const float fb);

	//设置通道1力标定系数c[0]：	
	// 设置系数c[0]=0.01：
	//XCB +0x0a+0xd7+0x23+0x3c+ 和校验码＋C
	DWORD XC_SetConstantForceCailFac_C1(const float fc);

	//设置通道2力标定系数b[1]：	
	//设置系数b[1]=1158.4：
	//XCC +0xcd+0xcc+0x90+0x44+ 和校验码＋C
	DWORD XC_SetConstantForceCailFac_B2(const float fb);

	//设置通道2力标定系数c[1]：	
	//设置系数c[1]=0.01：
	//XCD +0x0a+0xd7+0x23+0x3c + 和校验码＋C
	DWORD XC_SetConstantForceCailFac_C2(const float fc);

	//设置通道3力标定系数b[2]：	
	//设置系数b[2]=1158.4：
	//XCE +0xcd+0xcc+0x90+0x44 + 和校验码＋C
	DWORD XC_SetConstantForceCailFac_B3(const float fb);

	//设置通道3力标定系数c[2]：	
	//设置系数c[2]=0.01：
	//XCF +0x0a+0xd7+0x23+0x3c  +和校验码＋C
	DWORD XC_SetConstantForceCailFac_C3(const float fc);

	//设置恒速控制PID控制比例系数kp：	
	//设置系数c[2]=0.01：
	//XCH +0x0a+0xd7+0x23+0x3c  +和校验码＋C
	DWORD XC_SetConstantSpeedControl_P(const float fkp);

	//设置恒速控制PID控制积分系数ki：	
	//设置系数c[2]=0.01：
	//XCI +0x0a+0xd7+0x23+0x3c +和校验码＋C
	DWORD XC_SetConstantSpeedControl_I(const float fki);

	//设置恒速控制PID控制微分系数kd：	
	//设置系数c[2]=0.01：
	//XCJ+0x0a+0xd7+0x23+0x3c +和校验码＋C
	DWORD XC_SetConstantSpeedControl_D(const float fkd);

	//设置恒力控制PID控制比例系数kp_force：	
	//设置系数c[2]=0.01：
	//XCK +0x0a+0xd7+0x23+0x3c  +和校验码＋C
	DWORD XC_SetConstantForceControl_P(const float fkp);

	//设置恒力控制PID控制比例系数 ki_force：	
	//设置系数c[2]=0.01：
	//XCL +0x0a+0xd7+0x23+0x3c +和校验码＋C
	DWORD XC_SetConstantForceControl_I(const float fki);

	//设置恒力控制PID控制比例系数 kd_force：	
	//设置系数c[2]=0.01：
	//XCM +0x0a+0xd7+0x23+0x3c +和校验码＋C
	DWORD XC_SetConstantForceControl_D(const float fkd);

	//启动恒速度控制	XBS + 0+0+0+0+ 和校验码 ＋ C
	//启动恒扭距控制	XBF + 0+0+0+0+ 和校验码 ＋ C
	//启动恒功率控制	XBP + 0+0+ 0+0+ 和校验码 ＋ C

	//启动输出设定pwm值	XBI + 0+0+ 0+0+ 和校验码 ＋ C
	DWORD XC_StartSetPWN(const float fPWN);

	//停止控制,pwm输出0	XBS + 0+0+0+0+ 和校验码 ＋ C
	DWORD XC_StopPWN(void);

	//起动电机	XEB + 0+0+0+0+ 和校验码 ＋ C
	//停止电机	XES + 0+0+0+0+ 和校验码 ＋ C

	//打开指定继电器（LH1/LH2/LH3）	（LH1/LH2/LH3）分别对应数据1/2/3：如打开继电器LH1: XEO + 0+0+0+1+ 和校验码 ＋ C
	DWORD XC_OpenRelay(const int nLH);

	//关闭指定继电器（LH1/LH2/LH3）	（LH1/LH2/LH3）分别对应数据1/2/3：如打开继电器LH3: XEF + 0+0+0+3+ 和校验码 ＋ C
	DWORD XC_CloseRelay(const int nLH);

	//举升器上升	XJU+ 0+0+0+0+ 和校验码 ＋ C
	//举升器下降	XJD +0+0+0+0+ 和校验码 ＋ C
	//进入标定状态	XDF +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_EnterCaliSta(void);

	//退出标定状态	 XDS +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_ExitCaliSta(void);

	//读取环境参数	 XRT +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_ReadEnvPar(float *const pfET=NULL, float *const pfAP=NULL, float *const pfRH=NULL);

	//读取力标定系数b,c	XRF +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_ReadForCalFac(float *const pfb=NULL, float *const pfc=NULL, float *const pfb1=NULL, float *const pfc1=NULL, float *const pfb2=NULL, float *const pfc2=NULL);

	//读取力通道	XRC +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_ReadForCha(int *const pnCha=NULL);

	//读取恒速PID系数	XRs +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_ReadConSpeed(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL);

	//读取恒力PID系数	XRf +0+0+0+0+ 和校验码 ＋ C	
	DWORD XC_ReadConForce(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL);

	//读取滚筒直径和编码器脉冲	 XRS +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_ReadRollerAndEncoder(int *const pnDia=NULL, int *const pnPul=NULL);

	//将3个通道的标定系数固化到ROM中	 XMA +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_SolCalFac(void);

	//将滚筒直径和编码器脉冲固化到ROM中	XMS +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_SolRollerAndEncoder(void);

	//将力通道的选择固化到ROM中	XMC +0+0+0+0+ 和校验码 ＋ C
	DWORD XC_SolForCha(void);

	//将恒速控制的PID系数固化到ROM中	XMD + 0+0+0+0+ 和校验码 ＋ C
	DWORD XC_SolConSheepPID(void);

	//将恒力控制的PID系数固化到ROM中	XME + 0+0+0+0+ 和校验码 ＋ C
	DWORD XC_SolConForcePID(void);

	///////////////////////////////////// 新城的控制设置End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器操作end 

private:
	// 计算校验和
	// 相加
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	BYTE CheckSum(const BYTE *const p, const UINT un);

	// float转16进制
	// 简单点写就是printf("%x\n",*(unsigned int *)&fvalue);
	unsigned int flo_hex(float fvalue);
	//十进制数转成十六进制   IEEE754单精度
	uint32_t data2hex(uint16_t data);

	// 十六进制数转浮点数通用方法
	/**
	* 将unsigned char数组转换成long long数值
	* {0x00 0x00 0x20 0x00}转换之后得到8192
	*
	* @param str 数组
	* @param len str数组长度
	* @returns 0 on error
	*/
	LONGLONG ReadCharToLL(unsigned char* str, int len);
	float Hex2Float(unsigned long long number);
	
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
private:
	// 串口句柄
	HANDLE m_hComm;
	//客户信息临界保护量
	CRITICAL_SECTION cInfoSection;

};

#endif