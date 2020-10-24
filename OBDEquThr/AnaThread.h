/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：AnaThread.h
* 文件标识：
* 摘要：分析仪线程
*
* 当前版本：1.0.5
* 作者：Hyh
* 完成日期：2010-06-21
*
* 历史信息：
*
* 当前版本：1.0.23
* 完成日期：2014-07-18
* 作者：Lihy
* 描述：添加获取流量计所有实时数据指令
*
* 当前版本：1.0.22
* 完成日期：2014-06-26
* 作者：Lihy
* 描述：添加获取微状态指令，用于判断VMAS检测过程中分析仪是否堵塞(微状态中高位中的第4个2进制位用于判断，1为堵塞，0为正常)
*
* 当前版本：1.0.21
* 完成日期：2014-05-10
* 作者：Lihy
* 描述：添加国产流量计高中低三点校准命令
*
* 版本：1.0.20
* 日期：2013-1-18
* 作者：Wyq
* 描述：增加读取气体温度实时数值
*
* 版本：1.0.19
* 日期：2012-10-12
* 作者：Raylee
* 描述：添加南华NHA-501A设备支持
*
* 版本：1.0.18
* 日期：2012-10-11
* 作者：Raylee
* 描述：添加佛分FGA4100，福立FLA501，鸣泉MQW511设备支持
*
* 当前版本：1.0.7
* 完成日期：2011-03-03
* 作者：Ming
* 描述：分别添加设置HCCOCO2,O2,NOx通道延时值的函数,GetxxxDelayTime_Flowmeter(),SetxxxDelayTime_Flowmeter()和xxxDelayTime_Flowmeter()
*
* 当前版本：1.0.6
* 完成日期：2010-08-26
* 作者：Raylee
* 描述：添加GetTestResultData()函数，获取检测结果数据
*
* 当前版本：1.0.5
* 完成日期：2010-06-21
* 作者：Hyh
* 描述：1)修正GetProcessDataStatus_CT_Flowmeter函数
*
* 当前版本：1.0.4
* 完成日期：2010-03-17
* 作者：Cui
* 描述：将大气压力校准函数由APCalibrate改为APCalibration
*
* 当前版本：1.0.3
* 完成日期：2010-02-01
* 作者：Cui,hyh
* 描述：函数Open()切换通讯模式修改,原切换次数为1,现切换次数为50
*
* 当前版本：1.0.2
* 完成日期：2009-12-31
* 作者：Cui
* 描述：函数Open，添加形参bool bConnectToFlowmeter(标记分析仪是否有连接流量计)
*
* 当前版本：1.0.1
* 完成日期：2009-12-10
* 作者：Cui
* 描述：修改流量计流量、压力、稀释氧校准函数
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaNha503V.h"
#include "NanhuaNha503.h"
#include "NanhuaNha501A.h"
#include "NanhuaNha506I.h"
#include "NanhuaNha509Encrypt.h"
#include "NanhuaNha610Encrypt.h"
#include "NanhuaNha100.h"
#include "MingQuanML100.h"
#include "MingQuanMQW511.h"
#include "MingQuanMQW50A.h"
#include "FuLiFLA501.h"
#include "FuLiFLA502.h"
#include "FuLiFLV1000.h"
#include "FoFenFGA4100.h"
// CAnaThread

class AFX_EXT_CLASS CAnaThread : public CWinThread
{
	DECLARE_DYNCREATE(CAnaThread)

protected:
	CAnaThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CAnaThread();

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
		// 单独的503
		IM_NANHUA_NHA503,
		// 可与Vmas连接的503
		IM_NANHUA_NHA503V,
		// 公开协议的501A
		IM_NANHUA_NHA501A,
		// 内部通讯协议的506,605
		IM_NANHUA_NHA506I,
		// 佛分FGA-4100
		IM_FOFEN_FGA4100,
		// 福立FLA-501
		IM_FULI_FLA501,
		// 福立FLA-502
		IM_FULI_FLA502,
		// 福立FLV1000
		IM_FULI_FLV1000,
		// 鸣泉MQW-511
		IM_MINGQUAN_MQW511,
		// 鸣泉MQW-50A
		IM_MINGQUAN_MQW50A,
		// 鸣泉ML-100
		IM_MINGQUAN_ML100,
		// NHA509
		IM_NANHUA_NHA509,
		// NHA509加密
		IM_NANHUA_NHA509EN,
		// NHA610加密
		IM_NANHUA_NHA610EN,
		// NHA100
		IM_NANHUA_NHA100,
	};

public:
	// 共享串口句柄
	HANDLE GetComHandle(void);
	void AttachComHandle(HANDLE hCom, const wchar_t *szProtocol);
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
	// (in)bPort：串口号(1~255)
	// (in)im：仪器类型
	// (in)bConnectToFlowmeter：是否有连接流量计
	// 返回打开结果：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort, const wchar_t *szProtocol, const bool bConnectToFlowmeter=false);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// ANA_CLOSE_SUCCEED
	// ANA_CLOSE_FAIL_WIN32API
	// ANA_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	inline bool IsOpen(void) const
	{
		return m_pAna->IsOpen();
	}
	// 获取串口操作状态
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪部分基本操作begin
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 同步操作begin
	// 1取主数据
	// (out)psHC,HC浓度指针,单位:ppm,可以为空指针
	// (out)pfCO,CO浓度指针,单位:%,可以为空指针
	// (out)pfCO2,CO2浓度指针,单位:%,可以为空指针
	// (out)pfO2,O2浓度指针,单位:%,可以为空指针
	// (out)pfNO,NO浓度指针,单位:ppm,可以为空指针
	// (out)pusRpm,转速,单位:r/min,可以为空指针
	// (out)pfOilTemp,油温,单位:摄氏度,可以为空指针
	// (out)pusFlux,流量当量,单位:未知,可以为空指针
	// (out)pfLambda,过量空气系数,单位:无,可以为空指针
	// (out)pbO2Sensor,氧传感器是否失效,单位:无,可以为空指针
	// (out)pbNOSensor,NO传感器是否失效,单位:无,可以为空指针
	// (out)pbWaterFull,判断水位是否已满,单位:无（新）
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetMajorData(short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL,
		short *const psNO2=NULL,
		short *const psNOx=NULL,
		USHORT *const pusRpm=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusFlux=NULL, 
		float *const pfLambda=NULL,
		bool *const pbO2Sensor=NULL,
		bool *const pbNOSensor=NULL);

	DWORD GetMajorData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO,
		short *const psNO2,
		short *const psNOx,
		USHORT *const pusRpm, 
		float *const pfOilTemp, 
		USHORT *const pusFlux, 
		float *const pfLambda,
		bool *const pbO2Sensor,
		bool *const pbNOSensor,
		bool *const pbWaterFull);
	
	DWORD GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx);

	// 2取辅助数据
	// 其中ET:Environment Temperature(环境温度)缩写
	// 其中AP:Atmospheric Pressure(大气压力)缩写
	// 其中:RH:Relative Humidity(相对湿度)缩写
	// (out)pfET:环境温度指针,单位:摄氏度,不能为空指针
	// (out)pfAP:大气压力指针,单位:kPa,不能为空指针
	// (out)pfRH:相对湿度指针,单位:%,不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetMinorData(float *const pfET, float *const pfAP, float *const pfRH);

	// 3抽空气
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflateEnvAir(BYTE *const pbResult = NULL);

	// 4抽管道气(开泵,气样从探头进入)
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflatePipeAir(BYTE *const pbResult = NULL);

	// 5清洗管道(反吹)
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD InverseBlow(BYTE *const pbResult = NULL);

	// 6停止清洗(停止反吹及停泵)
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StopPump(BYTE *const pbResult = NULL);

	// 7检漏
	// (out)pbResult,仪器执行结果指针,0x00-无泄露,0x01-有泄露,0x05-正在检漏,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CheckLeak(BYTE *const pbResult = NULL);

	// 8调零
	// (out)pbResult,仪器执行结果指针,0x00-调零成功,0x01-调零失败,0x05-正在调零,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD Zero(BYTE *const pbResult = NULL);

	// 9写入校准气浓度
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败(校验和错误),如果函数执行过程错误,被置为0xFF,不能为空指针
	// (in)usCalC3H8,C3H8校准值,单位:ppm
	// (in)fCalCO,CO校准值,单位:%
	// (in)fCalCO2,CO2校准值,单位:%
	// (in)usCalNO,NO校准值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2);

	// 10校准
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,0x05-正在校准,如果函数执行过程错误,被置为0xFF,不能为空指针
	// (out)psHC,HC值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO,CO值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO2,CO2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfO2,O2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)psNO,NO值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfPef,Pef值指针,单位:无,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GasCalibration(BYTE *const pbResult, const int nChanel, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		short *const psNO2=NULL, 
		float *const pfPef=NULL);
	
	// 写入NO2校准气浓度
	// (in)usNO2,NO2校准值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2);

	// 校准NO2
	// (out)psNO2,NO2值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD NO2Calibration(BYTE *const pbResult, short *const psNO2=NULL);

	// 11写入检查气浓度
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败(校验和错误),如果函数执行过程错误,被置为0xFF,不能为空指针
	// (in)uChkC3H8:C3H8检查值,单位:ppm
	// (in)fChkCO:CO检查值,单位:%
	// (in)fChkCO2:CO2检查值,单位:%
	// (in)usChkNO:NO检查值,单位:ppm
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO);

	// 12自动检查
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,0x05-正在检查,如果函数执行过程错误,被置为0xFF,不能为空指针
	// (out)psHC,HC值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO,CO值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfCO2,CO2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfO2,O2值指针,单位:%,如果函数执行过程错误,被置为0,可以为空指针
	// (out)psNO,NO值指针,单位:ppm,如果函数执行过程错误,被置为0,可以为空指针
	// (out)pfPef,Pef值指针,单位:无,如果函数执行过程错误,被置为0,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GasCheck(BYTE *const pbResult, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		float *const pfPef=NULL);

	// 13取PEF
	// (out)pfPef,Pef值指针，单位：无，不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetPef(float *const pfPef);

	// 14清除NO老化标志
	// 503特有
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ClearNoSign(BYTE *const pbResult = NULL);

	// 15设置调零气体为空气
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToEnvAir(BYTE *const pbResult = NULL);

	// 16设置调零气体为零气
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToZeroAir(BYTE *const pbResult = NULL);

	// 17设置冲程为4
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL);

	// 18设置冲程为2
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL)  ;

	// 19设置点火方式为单次
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) ;

	// 20设置点火方式为二次
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL);

	// 21允许自动调零
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD EnableAutoZero(BYTE *const pbResult = NULL);

	// 22禁止自动调零
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DisableAutoZero(BYTE *const pbResult = NULL);

	// 23允许键盘操作

	// 24禁止键盘操作

	// 25LCD开

	// 26LCD关

	// 27环境温度校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,不能为空指针
	// (in)fCalET:环境温度校准值,单位:摄氏度
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ETCalibration(BYTE *const pbResult, const float fCalET);

	// 28室温校准

	// 29油温校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,不能为空指针
	// (in)fCalOT:油温校准值,单位:摄氏度
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OTCalibration(BYTE *const pbResult, const float fCalOT);

	// 30环境湿度校准
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,不能为空指针
	// (in)fCalRH:相对湿度校准值,单位-%
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD RHCalibration(BYTE *const pbResult, const float fCalRH);

	// 31大气压力校准
	// 返回函数执行结果：
	// (out)pbResult:仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,不能为空指针
	// (in)fCalAP:大气压力校准值,单位:kPa
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD APCalibration(BYTE *const pbResult, const float fCalAP);

	// 32检查气开
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OpenCheckGas(BYTE *const pbResult = NULL);

	// 33检查气关
	// (out)pbResult,仪器执行结果指针,0x00-成功,0x01-失败,如果函数执行过程错误,被置为0xFF,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CloseCheckGas(BYTE *const pbResult = NULL);

	// 34取仪器状态
	// (out)pbStatus,仪器状态指针,0x00-就绪(未开泵),0x01-正在检测(已开泵),0x02-正在预热,0x03-正在检漏,如果函数执行过程错误,被置为0xFF,不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);

	// 35取预热剩余时间
	// (out)pusSecondsLeft:预热剩余时间指针,应答[AA][BB],剩余时间=[AA]*256+[BB]秒;如果函数执行过程错误,被置为0xFFFF;不能为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetWarmUpTimeLeft(USHORT *const pusSecondsLeft);

	// 36设置燃料类型为汽油
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToGas(BYTE *const pbResult = NULL);

	// 37设置燃料类型为液化石油气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToLPG(BYTE *const pbResult = NULL);

	// 38设置燃料类型为天然气
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToNG(BYTE *const pbResult = NULL);

	// 39设置燃料类型为乙醇汽油
	// (out)pbResult,仪器执行结果指针,一般返回0x06,可以为空指针
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToGasohol(BYTE *const pbResult = NULL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 同步操作end


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 异步操作begin
	// 异步获取分析仪主数据
	void GetMajorData_Asyn(HWND hCallBackWnd=NULL);
	// 异步获取分析仪主数据(新)
	void GetMajorData_Asyn_N(HWND hCallBackWnd=NULL);
	// 异步获取NOx数据
	void GetNOxData_Asyn(HWND hCallBackWnd=NULL);
	// 异步获取分析仪辅助数据
	void GetMinorData_Asyn(HWND hCallBackWnd=NULL);
	//  异步获取Pef
	void GetPef_Asyn(HWND hCallBackWnd=NULL);
	// 异步获取仪器状态
	void GetStatus_Aysn(HWND hCallBackWnd=NULL);
	// 异步分析仪校准
	void GasCalibration_Aysn(const int nChanel, HWND hCallBackWnd=NULL);
	// 异步气体检查
	void GasCheck_Aysn(HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 异步操作end
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪部分基本操作begin

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计部分基本操作begin
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 同步操作begin
	// 获取当前使用的操作模式
	inline DWORD GetOperationModeUsedNow(void) const
	{
		return m_opUsedNow;
	}

	// 操作模式转换
	// (in)op:操作模式,ToAnalyzer-分析仪通讯模式,ToFlowmeter-流量计通讯模式
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetOperationMode(const enum CAna::OperationMode op, BYTE *const pbResult=NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行状态begin
	// 取命令执行状态(判断流量计命令是否已经执行完毕)
	// 主要用于判断稀释氧校准等需要等待的命令是否已经执行完毕
	// (out)pbResult：仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	// 一般只会返回0x06，因为这是即时命令
	// (out)pwProcessStatus：命令执行状态指针，0-无命令正在执行，非0-有命令正在执行
	// (out)pwProcess：处于执行状态的命令字指针
	// (return)
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetProcessStatus_Flowmeter(BYTE *const pbResult=NULL, WORD *const pwProcessStatus=NULL, WORD *const pwProcess=NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行状态end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////收集数据相关begin
	// 开始收集数据
	// (int)wCollectionTime:收集时间,单位:s(秒)。VMAS有限制，最大只能是600秒。
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StartCollectData_Flowmeter(const WORD wCollectionTime=195, WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL); 

	// 停止收集数据
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StopCollectData_Flowmeter(WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL);  


	// 读过程数据和状态(质量模式)
	// (in)wCntStart:过程数据开始序号，只能是1~600
	// (in)wCntLength:过程数据长度，只能是1~600
	// (out)pfHCwt:HC质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfCOwt:CO质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfCO2wt:CO2质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfNOwt:NO质量指针；单位：毫克(mg)。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL);

	// 读过程数据和状态(浓度模式) 
	// (in)wCntStart:过程数据开始序号，只能是1~600
	// (in)wCntLength:过程数据长度，只能是1~600
	// (out)psHCct:HC浓度指针,单位:ppm。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psCOct:CO浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psCO2ct:CO2浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psNOct:NO浓度指针,单位:ppm。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfDilO2:稀释氧浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfBenchO2:分析仪氧浓度指针,单位:%。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)psGasTemp;稀释气体温度,单位:摄氏度。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasFluxStd0:稀释气体0℃标准流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasFluxStd20:稀释气体20℃标准流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfExhaustFluxStd0:废气0℃流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfExhaustFluxStd20:废气20℃流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasPres:稀释气体压力,单位:kPa。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pfGasFluxAct:稀释气体实际流量,单位:L/s。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pusRpm:发动机转速,单位:r/min。不能为NULL，所指向的内存大小必须大于等于wCntLength
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct=NULL, 
		float *const pfCOct=NULL, 
		float *const pfCO2ct=NULL, 
		short *const psNOct=NULL, 
		float *const pfDilO2=NULL, 
		float *const pfBenchO2=NULL, 
		short *const psGasTemp=NULL, 
		float *const pfGasFluxStd0=NULL, 
		float *const pfGasFluxStd20=NULL, 
		float *const pfExhaustFluxStd0=NULL, 
		float *const pfExhaustFluxStd20=NULL, 
		float *const pfGasPres=NULL, 
		float *const pfGasFluxAct=NULL, 
		USHORT *const pusRpm=NULL,
		WORD *const pwMiniStatus=NULL,
		const BYTE bDataMode=0x01,
		BYTE *const pbResult=NULL);

	// 获取总克数(mg) 
	// (out)pusHCwt,HC总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pusCOwt,CO总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pusCO2wt,CO2总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pusNOwt,NO总质量指针,单位:mg(毫克)。不能为NULL
	// (out)pfExhaustVolume:废气体积,单位:L。不能为NULL
	// (out)pfGasVolume:稀释气体体积,单位:L。不能为NULL
	// (out)pwMiniStatus:状态，具体含义请参考通讯协议。可以为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL);

	// 获取已经收集数据的数量
	// (out)pwCorrectedDataNumber:已经收集数据个数指针；如果函数执行错误，则返回0x00，不能为NULL
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult=NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////收集数据相关end


	// 获取流量计实时数据
	// pfFluxStd0:0℃标准流量，L/s
	// pfFluxStd20:20℃标准流量，L/s
	// pfFluxAct:实际流量，L/s
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	// 获取流量计全部实时数据
	// pfFluxStd0:0℃标准流量，L/s
	// pfFluxStd20:20℃标准流量，L/s
	// pfFluxAct:实际流量，L/s
	// (out)pbResult:仪器操作结果指针；0x06-成功，0x05-失败(流量计忙)， 0x15-失败(错误命令)
	//               如果函数执行过程错误，返回0x00
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	// 设置标准流量 注:fCalFlux需标准化到20℃状态下 Mark By LiangQW@20160721
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CAna::FluxCalibrationMode cm, const float fCalFlux);
	// 流量校准 //返回校准结果pbCalResult：成功：低流量0,高流量2,单点1 Mark By LiangQW@20160722
	DWORD FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum CAna::FluxCalibrationMode cm);

	// 设置标准压力
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);
	// 压力校准 
	DWORD PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	//暂时取消：原因：进口流量计不支持 Mark By LiangQW@20160721
	// 设置温度
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);
	// 温度校准 
	DWORD TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// 设置标准氧气
	DWORD SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2);
	// 氧气校准 
	DWORD DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// 设置高点标准氧气(适用国产流量计)
	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);
	// 高点氧气校准 
	DWORD DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// 设置中点标准氧气(适用国产流量计)
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);
	// 中点氧气校准 
	DWORD DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// 设置低点标准氧气(适用国产流量计)
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);
	// 低点氧气校准 
	DWORD DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// 开始采集环境氧
	DWORD SatrtGetEnvironmentO2_Flowmeter(void);
	// bResult 0x00时获取环境氧成功,其他状态待定
	DWORD GetEnvironmentO2_Flowmeter(BYTE *const pbResult,float *const pfEnvO2);

	// 设置HC/CO/CO2通道延时值
	DWORD GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// 设置O2通道延时值
	DWORD GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// 设置NOx通道延时值
	DWORD GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// 开关风机
	// (in)bRun:开关风机参数；true-开风机，false-关风机
	// (out)pbResult:仪器操作结果指针；0x06-设置模式成功，0x05-设置模式失败(流量计忙)， 0x15-设置模式失败(错误命令)
	//               如果函数执行过程错误，返回0xFF
	//				 可以为NULL
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 同步操作end

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 异步操作begin
	//取流量计实时数据
	void GetRealTimeData_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// 取过程数据,质量模式(异步模式只可以取一个数据)
	void GetProcessDataStatus_WT_Flowmeter_Aysn(const WORD wCntStart, HWND hCallBackWnd=NULL);
	// 取过程数据,浓度模式(异步模式只可以取一个数据)
	void GetProcessDataStatus_CT_Flowmeter_Aysn(const WORD wCntStart, HWND hCallBackWnd=NULL);
	// 取总质量
	void GetTotalWeight_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// 开始采集数据
	void StartCollectData_Flowmeter_Aysn(const WORD wCollectionTime=195,HWND hCallBackWnd=NULL);
	// 停止采集
	void StopCollectData_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// 开始采集环境氧
	void SatrtGetEnvironmentO2_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// 取环境氧
	void GetEnvironmentO2_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 异步操作end
	
	// 获取手动检测的数据
	// 返回函数执行结果：
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetTestResultData(short *const psHCHigh=NULL, 
		float *const pfCOHigh=NULL, 
		float *const pfCO2High=NULL, 
		float *const pfO2High=NULL, 
		short *const psNOHigh=NULL,
		USHORT *const pusRpmHigh=NULL, 
		float *const pfOilTempHigh=NULL, 
		float *const pfLambdaHigh=NULL,
		short *const psHCLow=NULL, 
		float *const pfCOLow=NULL, 
		float *const pfCO2Low=NULL, 
		float *const pfO2Low=NULL, 
		short *const psNOLow=NULL,
		USHORT *const pusRpmLow=NULL, 
		float *const pfOilTempLow=NULL, 
		float *const pfLambdaLow=NULL);

	DWORD SetAnalyzerOperationMode(void);
	DWORD SetFlowmeterOperationMode(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计部分基本操作end

private:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////分析仪响应异步操作函数begin
	afx_msg void OnGetMajorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMajorDataN(WPARAM wParam, LPARAM lParam); // 新的取数据协议（增加水位判断）
	afx_msg void OnGetNOxData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMinorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetPef(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGasCalibration(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGasCheck(WPARAM wParam, LPARAM lParam);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////分析仪响应异步操作函数end


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量计响应异步操作函数begin
	afx_msg void OnGetRealTimeData_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetProcessDataStatus_WT_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetProcessDataStatus_CT_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetTotalWeight_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStartCollectData_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopCollectData_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSatrtGetEnvironmentO2_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetEnvironmentO2_Flowmeter(WPARAM wParam, LPARAM lParam);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量计响应异步操作函数end


public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////分析仪部分发布数据begin
	// 取分析仪主数据,HC值,单位:ppm
	inline short GetMajorDataHCValue(void) const
	{
		return m_sHC;
	}
	// 取分析仪主数据,CO值,单位:%
	inline float GetMajorDataCOValue(void) const
	{
		return m_fCO;
	}
	// 取分析仪主数据,CO2值,单位:%
	inline float GetMajorDataCO2Value(void) const
	{
		return m_fCO2;
	}
	// 取分析仪主数据,O2值,单位:%
	inline float GetMajorDataO2Value(void) const
	{
		return m_fO2;
	}
	// 取分析仪主数据,NO值,单位:ppm
	inline short GetMajorDataNOValue(void) const
	{
		return m_sNO;
	}
	// 取分析仪主数据,NO2值,单位:ppm
	inline short GetMajorDataNO2Value(void) const
	{
		return m_sNO2;
	}
	// 取分析仪主数据,NOx值,单位:ppm
	inline short GetMajorDataNOxValue(void) const
	{
		return m_sNOx;
	}
	// 取分析仪主数据,转速值,单位:r/min
	inline USHORT GetMajorDataRpmValue(void) const
	{
		return m_usRpm;
	}
	// 取分析仪主数据,油温值,单位:摄氏度
	inline float GetMajorDataOilTempValue(void) const
	{
		return m_fOilTemp;
	}
	// 取分析仪主数据,流量当量值,单位:无
	inline USHORT GetMajorDataFluxValue(void) const
	{
		return m_usFlux;
	}
	// 取分析仪主数据,过量空气系数,单位:无
	inline float GetMajorDataLambdaValue(void) const
	{
		return m_fLambda;
	}
	// 取Pef值,单位:无
	inline float GetPefValue(void) const
	{
		return m_fPef;
	}
	// 取状态值
	inline USHORT GetStatusValue(void) const
	{
		return m_bStatus;
	}
	// 取分析仪副数据,环境温度值,单位:摄氏度
	inline float GetMinorDataETValue(void) const
	{
		return m_fET;
	}
	// 取分析仪副数据,大气压力值,单位:kPa
	inline float GetMinorDataAPValue(void) const
	{
		return m_fAP;
	}
	// 取分析仪副数据,相对湿度值,单位:%
	inline float GetMinorDataRHValue(void) const
	{
		return m_fRH;
	}
	inline bool GetMajorDataWaterFull(void) const
	{
		return m_bWaterFull;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////分析仪部分发布数据end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量计部分发布数据begin
	// 取流量计实时数据,流量值,单位:scfm(标准立方英尺/分钟)
	inline float GetVmasRTDFluxStd0Value(void) const
	{
		return m_fFluxStd0;
	}
	inline float GetVmasRTDFluxStd20Value(void) const
	{
		return m_fFluxStd20;
	}
	inline float GetVmasRTDFluxActValue(void) const
	{
		return m_fFluxAct;
	}
	// 取流量计实时数据,压力值,单位:kPa
	inline float GetVmasRTDPresValue(void) const
	{
		return m_fPres;
	}
	// 取流量计实时数据,未修正稀释氧值,单位:%
	inline float GetVmasRTDUDilO2Value(void) const
	{
		return m_fUDilO2;
	}
	// 取流量计实时数据,稀释气体温度值,单位:摄氏度
	inline float GetVmasRTDGasTempValue(void) const
	{
		return m_fGasTemp;
	}
	// 取过程数据,HC质量值,单位:mg
	inline float GetVmasPDSHCwtValue(void) const
	{
		return m_fHCwt;
	}
	// 取过程数据,CO质量值,单位:mg
	inline float GetVmasPDSCOwtValue(void) const
	{
		return m_fCOwt;
	}
	// 取过程数据,CO2质量值,单位:mg
	inline float GetVmasPDSCO2wtValue(void) const
	{
		return m_fCO2wt;
	}
	// 取过程数据,NO质量值,单位:mg
	inline float GetVmasPDSNOwtValue(void) const
	{
		return m_fNOwt;
	}
	// 取过程数据,HC浓度值,单位:ppm
	inline short GetVmasPDSHCctValue(void) const
	{
		return m_sHCct;
	}
	// 取过程数据,CO浓度值,单位:%
	inline float GetVmasPDSCOctValue(void) const
	{
		return m_fCOct;
	}
	// 取过程数据,CO2浓度值,单位:%
	inline float GetVmasPDSCO2ctValue(void) const
	{
		return m_fCO2ct;
	}
	// 取过程数据,NO浓度值,单位:ppm
	inline short GetVmasPDSNOctValue(void) const
	{
		return m_sNOct;
	}
	// 取过程数据,稀释O2浓度值,单位:%
	inline float GetVmasPDSDilO2Value(void) const
	{
		return m_fDilO2;
	}
	// 取过程数据,分析仪O2浓度值,单位:%
	inline float GetVmasPDSBenchO2Value(void) const
	{
		return m_fBenchO2;
	}
	// 取过程数据,稀释气体温度值,单位:摄氏度
	inline short GetVmasPDSGasTempValue(void) const
	{
		return m_sGasTemp;
	}
	// 取过程数据,稀释气体标准流量值,单位:scfm
	inline float GetVmasPDSGasFluxStd0Value(void) const
	{
		return m_fGasFluxStd0;
	}
	inline float GetVmasPDSGasFluxStd20Value(void) const
	{
		return m_fGasFluxStd20;
	}
	// 取过程数据,废气标准流量值,单位:scfm
	inline float GetVmasPDSExhaustFluxStd0Value(void) const
	{
		return m_fExhaustFluxStd0;
	}
	inline float GetVmasPDSExhaustFluxStd20Value(void) const
	{
		return m_fExhaustFluxStd20;
	}
	// 取过程数据,稀释气体压力值,单位:kPa
	inline float GetVmasPDSGasPresValue(void) const
	{
		return m_fGasPres;
	}
	// 取过程数据,稀释气体实际流量值,单位:ascm
	inline float GetVmasPDSGasFluxActValue(void) const
	{
		return m_fGasFluxAct;
	}
	// 取过程数据,发动机转速值,单位:rpm
	inline USHORT GetVmasPDSRpmValue(void) const
	{
		return m_usVmasRpm;
	}
	// 取HC总克数,单位:mg
	inline USHORT GetVmasTotalHCwtValue(void) const
	{
		return m_usTotalHCwt;
	}
	// 取CO总克数,单位:mg
	inline USHORT GetVmasTotalCOwtValue(void) const
	{
		return m_usTotalCOwt;
	}
	// 取CO2总克数,单位:mg
	inline USHORT GetVmasTotalCO2wtValue(void) const
	{
		return m_usTotalCO2wt;
	}
	// 取NO总克数,单位:mg
	inline USHORT GetVmasTotalNOwtValue(void) const
	{
		return m_usTotalNOwt;
	}
	// 取废气总体积,单位:L
	inline float GetVmasTotalExhaustVolume(void) const
	{
		return m_fTotalExhaustVolume;
	}
	// 取稀释气体总体积,单位:L
	inline float GetVmasTotalGasVolume(void) const
	{
		return m_fTotalGasVolume;
	}
	// 取环境氧,单位:mg
	inline float GetVmasEnvironmentO2Value(void) const
	{
		return m_fEnvO2;
	}

	//取微状态
	inline WORD GetMiniStatus(void) const
	{
		return m_wMiniStatus;
	}
	// 获取分析仪读写状态
	inline DWORD GetAnaWRSta(void) const
	{
		return m_dwAnaWRSta;
	}
	// 获取流量计读写状态
	inline DWORD GetFloWRSta(void) const
	{
		return m_dwFloWRSta;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////流量计部分发布数据begin


public:
	// 退出线程操作
	void QuitThread(void);

private:
	// 初始化设备
	void InitializeInstrument(const wchar_t *szProtocol);
	// 卸载设备
	void UninitializeInstrument(void);

private:
	// Ana操作类
	CAna* m_pAna;
	// 设备型号
	InstrumentModel m_emInstrumentModel;
	// 线程退出标志
	bool m_bQuitThread;

protected:
	//////////////////////////////////////////////////////////分析仪部分成员变量begin
	// 主数据
	// HC浓度值,单位:ppm
	short  m_sHC;
	// CO浓度值,单位:%
	float  m_fCO;
	// CO2浓度值,单位:%
	float  m_fCO2;
	// O2浓度值,单位:%
	float  m_fO2;
	// NO浓度值,单位:%
	short  m_sNO;
	// NO2浓度值,单位:%
	short  m_sNO2;
	// NOx浓度值,单位:%
	short  m_sNOx;
	// 转速,单位:r/min
	USHORT  m_usRpm;
	// 油温,单位:摄氏度
	float  m_fOilTemp;
	// 流量当量值(其实是压力值,压力值越大,流量越小),单位:无
	USHORT  m_usFlux;
	// 过量空气系数,单位:无
	float  m_fLambda;
	// 氧传感器是否老化
	bool m_bO2Sensor;
	// 氮氧传感器是否老化
	bool m_bNOSensor;
	// 水位是否已满
	bool m_bWaterFull;
	// 辅助数据
	// 环境温度(Environment Temperature),单位:摄氏度
	float m_fET;
	// 大气压力(Atmospheric Pressure),单位:kPa
	float m_fAP;
	// 相对湿度(Relative Humidity),单位:%
	float m_fRH;

	// pef,单位:无
	float m_fPef;

	// 状态
	// 0x00-就绪(未开泵)
	// 0x01-正在检测(已开泵)
	// 0x02-正在预热
	// 0x03-正在检漏
	BYTE m_bStatus;
	//////////////////////////////////////////////////////////分析仪部分成员变量end

	//////////////////////////////////////////////////////////流量计部分成员变量begin
	// 流量计实时数据
	// 流量值,单位:scfm(标准立方英尺/分钟)
	float  m_fFluxStd0;
	float  m_fFluxStd20;
	float  m_fFluxAct;
	// 压力值,单位:kPa
	float  m_fPres;
	// 未修正稀释氧,单位:%
	float  m_fUDilO2;
	// 稀释气体温度,单位:摄氏度
	float m_fGasTemp; 

	// 质量过程数据
	// HC质量值,单位:mg
	float m_fHCwt; 
	// CO质量值,单位:mg
	float m_fCOwt; 
	// CO2质量值,单位:mg
	float m_fCO2wt; 
	// NO质量值,单位:mg
	float m_fNOwt;

	// 浓度过程数据
	// HC浓度值,单位:ppm
	short m_sHCct;
	// CO浓度值,单位:%
	float m_fCOct; 
	// CO2浓度值,单位:%
	float m_fCO2ct; 
	// NO浓度值,单位:ppm
	short m_sNOct; 
	// 稀释氧浓度值,单位:%
	float m_fDilO2; 
	// 分析仪氧浓度值,单位:%
	float m_fBenchO2; 
	// 稀释气体温度,单位:摄氏度
	short m_sGasTemp; 
	// 稀释气体标准流量值,单位:scfm(标准立方英尺/分钟)
	float m_fGasFluxStd0; 
	float m_fGasFluxStd20; 
	// 废气标准流量值,单位:scfm(标准立方英尺/分钟)
	float m_fExhaustFluxStd0;
	float m_fExhaustFluxStd20;
	// 稀释气体压力值,单位:kPa
	float  m_fGasPres; 
	// 稀释气体实际流量值,单位:acfm(标准立方英尺/分钟)
	float  m_fGasFluxAct;
	// 从流量计取回来的转速值,最终来自分析仪,单位:r/min
	USHORT  m_usVmasRpm;

	// 总克数
	// HC总质量,单位:mg
	USHORT m_usTotalHCwt;
	// CO总质量,单位:mg
	USHORT m_usTotalCOwt;
	// CO2总质量,单位:mg
	USHORT m_usTotalCO2wt;
	// NO总质量,单位:mg
	USHORT m_usTotalNOwt;
	// 废气总体积,单位:L
	float m_fTotalExhaustVolume;
	// 稀释气体总体积,单位:L
	float m_fTotalGasVolume;

	// 环境氧
	float m_fEnvO2;

	// 微状态,用于收集数据相关命令:51,52
	WORD m_wMiniStatus;
	DWORD m_dwAnaWRSta; // 分析仪读写状态
	DWORD m_dwFloWRSta; // 流量计读写状态
	//////////////////////////////////////////////////////////流量计部分成员变量begin

	// 目前正在通讯的仪器
	enum CAna::OperationMode m_opUsedNow;
	// 串口操作状态
	DWORD m_dwStatus;
};


