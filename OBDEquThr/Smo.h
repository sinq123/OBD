/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：Smo.h
* 文件标识：
* 摘要：烟度计抽象类
*		以纯虚拟函数形式定义烟度计操作接口
*
* 当前版本：1.0
* 作者：Cui,hyh
* 完成日期：2009-12-08
*
* 历史信息：
*
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#ifndef SMO_H
#define SMO_H

// 函数执行结果
#define SMO_OPEN_SUCCEED						0x00  // 打开串口成功
#define SMO_OPEN_FAIL							0x01  // 打开串口失败
#define SMO_CLOSE_SUCCEED						0x00  // 关闭串口成功
#define SMO_CLOSE_FAIL_WIN32_API				0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define SMO_CLOSE_FAIL_NO_OPEN_PORT				0x02  // 关闭串口失败：没有可以关闭的串口
#define SMO_WR_SUCCEED							0x00  // 写读成功
#define SMO_WR_PORT_NOT_OPEN					0x01  // 写读失败：串口未打开
#define SMO_WR_READ_SPECIFIED_DATA_FAIL			0x02  // 写读失败：读指定数据失败
#define SMO_WR_WRITE_SPECIFIED_DATA_FAIL		0x03  // 写读失败：写指定数据失败
#define SMO_WR_EQUIP_RETURE_0X15				0x04  // 写读失败：仪器返回错误代码（0x15）
#define SMO_WR_CHECK_SUM_FAIL					0x05  // 写读失败：读出数据校验和失败
#define SMO_WR_UNKNOWN_ERROR					0xFF  // 写读失败：未知错误

class CSmo
{
public:
	CSmo(void);
	virtual ~CSmo(void);

public:
	// 打开串口
	virtual DWORD Open(BYTE bPort) = 0;
	// 关闭串口
	virtual DWORD Close(void) = 0;

	// 判断串口是或打开
	virtual bool IsOpen(void) const = 0;

	///@brief 烟度自动校准 
	///发<9B> 返回：<06>
	virtual DWORD AutoAdj(BYTE *const pbResult=NULL) = 0;
	////@brief退出检测
	///发<9D> 返回:成功<06>，失败<15>
	virtual DWORD QuitAutoTest(BYTE *const pbResult) = 0;
	///@brief 烟度自动读数      //append for qin      
	///发<9C> 返回：<06>，失败<15>,检测完成<94>
	virtual DWORD AutoReadData(BYTE *const pbResult) = 0;
	///@brief 烟度计复位            
	///发<9A> 返回：<06>，失败<15>
	virtual DWORD Reset(BYTE *const pbResult=NULL) = 0;
	///@brief 开始自动检测          
	///发<92> 返回：第一次 <93>，第二次 <95>， 第三次 <95>，第四次 <94>，失败<15>
	virtual DWORD StartAutoTest(BYTE *const pbResult=NULL) = 0;    
	///@brief 取第一次数据          
	/// 发<96> 返回：成功<数据>，失败<15>
	virtual DWORD GetData1(BYTE *const pbResult, float *const pfData1) = 0;
	///@brief 取第二、三、四次数据
	///发<97> 返回：成功<数据>，失败<15>
	virtual DWORD GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4) = 0;
	///@brief 取检测状态
	///@param hCallBackWnd 返回信息句柄
	virtual DWORD GetTestState(BYTE *const pbResult, BYTE *const pbState) = 0; 
	///@brief 启动实时检测模式
	///@param hCallBackWnd 返回信息句柄
    virtual DWORD StartRealTest  (BYTE *const pbResult) = 0;      
	////@brief 开启响应测量模式,开启后踩脚踏开关才有效(紧在实时模式下)
	///@param hCallBackWnd 返回信息句柄
    virtual DWORD AllowTest(BYTE *const pbResult) = 0;  
	///@brief 监听一次检测是否完成,监听脚踏开关是否松开
	///@param hCallBackWnd 返回信息句柄
    virtual DWORD GetSign(BYTE *const pbResult) = 0;
	///@brief 烟度计硬复位            
	///发<9D> 返回：<98>，失败<15>
	virtual DWORD HardReset(BYTE *const pbResult=NULL) = 0;
};
#endif