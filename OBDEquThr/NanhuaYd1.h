/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaYd1.h
* 文件标识：
* 摘要：烟度计YD1通讯类，继承于抽象类CSmo
*		依照烟度计YD1通讯协议进行编写
*       具体请参考转烟度计YD1通讯协议说明书
*		
*
* 当前版本：1.0.1
* 作者：hyh
* 完成日期：2010-01-18
*
* 历史信息：
*
*
* 当前版本：1.0.1
* 完成日期：2010-01-18
* 作者：hyh
* 描述：1)修改GetData1234()和GetData1()函数
*       添加if(str.length() >= 14)和if(str.length() >= 4)防止出错
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#ifndef NANHUA_YD1_H
#define NANHUA_YD1_H

#include "Smo.h"

class CNanhuaYd1 : public CSmo
{
public:
	CNanhuaYd1(void);
	~CNanhuaYd1(void);

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