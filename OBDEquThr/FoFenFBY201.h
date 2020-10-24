/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FoFenFBY201.h
* 文件标识：
* 摘要：
*		
*
* 版本：1.0.1
* 作者：Raylee
* 日期：2012-03-19
*
* 历史信息：
*
*
* 版本：1.0.1
* 日期：2012-06-25
* 作者：Raylee
* 描述：GetTestState函数兼容两个版本，
*		1）会单字节的返回0x82/0x41；
*		2）按照说明书协议返回0x02 XX XX。
*
* 版本：1.0.0
* 日期：2012-03-19
* 作者：Raylee
* 描述：正式发布使用
*/

#pragma once

#ifndef FOFEN_FBY201_H
#define FOFEN_FBY201_H

#include "Smo.h"

class CFoFenFBY201 : public CSmo
{
public:
	CFoFenFBY201(void);
	~CFoFenFBY201(void);

public:
	// 打开串口
	DWORD Open(BYTE bPort);
	// 关闭串口
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;

	///@brief 烟度自动校准      //append for qin      
	///发<9B> 返回：<06>
	DWORD AutoAdj(BYTE *const pbResult=NULL);
	////@brief退出检测
	///发<9D> 返回:成功<06>，失败<15>
	DWORD QuitAutoTest(BYTE *const pbResult);
	///@brief 烟度自动读数      //append for qin      
	///发<9C> 返回：<06>，失败<15>,检测完成<94>
	DWORD AutoReadData(BYTE *const pbResult);
	///@brief 烟度计复位            
	///发<9A> 返回：<06>，失败<15>
	DWORD Reset(BYTE *const pbResult=NULL);
	///@brief 开始自动检测          
	///发<92> 返回：第一次 <93>，第二次 <95>， 第三次 <95>，第四次 <94>，失败<15>
	DWORD StartAutoTest  (BYTE *const pbResult=NULL);    
	///@brief 取第一次数据          
	/// 发<96> 返回：成功<数据>，失败<15>
	DWORD GetData1(BYTE *const pbResult, float *const pfData1);
	///@brief 取第二、三、四次数据
	///发<97> 返回：成功<数据>，失败<15>
	DWORD GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4);
	///@brief 取检测状态
	///@param hCallBackWnd 返回信息句柄
	DWORD GetTestState(BYTE *const pbResult, BYTE *const pbState); 
	///@brief 启动实时检测模式
	///@param hCallBackWnd 返回信息句柄
    DWORD StartRealTest  (BYTE *const pbResult);      
	////@brief 开启响应测量模式,开启后踩脚踏开关才有效(紧在实时模式下)
	///@param hCallBackWnd 返回信息句柄
    DWORD AllowTest(BYTE *const pbResult);  
	///@brief 监听一次检测是否完成,监听脚踏开关是否松开
	///@param hCallBackWnd 返回信息句柄
    DWORD GetSign(BYTE *const pbResult);
	///@brief 烟度计硬复位            
	///发<9D> 返回：<98>，失败<15>
	DWORD HardReset(BYTE *const pbResult=NULL);

private:
	// 连续执行发送与接收串口数据，bStopOnError：串口通讯失败时停止向下执行
	DWORD WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const DWORD dwSleepBeforeRead=0);
	// 计算前n位的校验和
	BYTE CheckSum(BYTE *const p, const UINT un);
private:
	// 串口句柄
	HANDLE m_hComm;
	//客户信息临界保护量
	CRITICAL_SECTION cInfoSection;
};
#endif