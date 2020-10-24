/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：SmoThread.h
* 文件标识：
* 摘要：烟度计线程
*
* 当前版本：1.0.1
* 作者：hyh
* 完成日期：2010-01-14
*
* 历史信息：
*
*
* 版本：1.0.2
* 日期：2012-04-26
* 作者：Raylee
* 描述：添加FoFenFBY201支持
*
* 当前版本：1.0.1
* 完成日期：2010-01-14
* 作者：hyh
* 描述：将OnRepeatCheck和OnRealCheck的硬复位改为软复位
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaYd1.h"
#include "FoFenFBY201.h"

// CSmoThread

class AFX_EXT_CLASS CSmoThread : public CWinThread
{
	DECLARE_DYNCREATE(CSmoThread)

protected:
	CSmoThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSmoThread();

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
		// YD1
		IM_NANHUA_YD1,
		// 佛分滤纸式烟度计
		IM_FOFEN_FBY201
	};

public:
	//同步操作
		// 打开串口
	 DWORD Open(const BYTE bPort, const enum InstrumentModel im);

	// 关闭串口
	DWORD Close(void);

	// 判断串口是或打开
	inline bool IsOpen(void) const
	{
		return m_pSmo->IsOpen();
	}
	///@brief 烟度自动校准      //append for qin      
	///发<9B> 返回：<06>
	DWORD AutoAdj(BYTE *const pbResult=NULL);
	///@brief 烟度自动读数      //append for qin      
	///发<9C> 返回：<06>，失败<15>,检测完成<94>
	DWORD AutoReadData(BYTE *const pbResult);
	///@brief 烟度计复位            
	///发<9A> 返回：<06>，失败<15>
	DWORD Reset(BYTE *const pbResult=NULL);
	////@brief退出检测
	///发<9D> 返回:成功<06>，失败<15>
	DWORD QuitAutoTest(BYTE *const pbResult);
	///@brief 开始自动检测          
	///发<92> 返回：第一次 <93>，第二次 <95>， 第三次 <95>，第四次 <94>，失败<15>
	DWORD StartAutoTest(BYTE *const pbResult=NULL);    
	///@brief 取第一次数据          
	/// 发<96> 返回：成功<数据>，失败<15>
	DWORD GetData1(BYTE *const pbResult, float *const pfData1);
	///@brief 取第二、三、四次数据
	///发<97> 返回：成功<数据>，失败<15>
	DWORD GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4);
	///@brief 取检测状态
	DWORD GetTestState(BYTE *const pbResult, BYTE *const pbState); 
	///@brief 启动实时检测模式
    DWORD StartRealTest(BYTE *const pbResult=NULL);      
	////@brief 开启响应测量模式,开启后踩脚踏开关才有效(紧在实时模式下)
    DWORD AllowTest(BYTE *const pbResult);  
	///@brief 监听一次检测是否完成,监听脚踏开关是否松开
    DWORD GetSign(BYTE *const pbResult);  
	///@brief 烟度计复位  只针对新的芯片(而且存在疑问,暂时弃用)          
	///发<9D> 返回：<98>，失败<15>
	DWORD HardReset(BYTE *const pbResult=NULL);

	//异步操作

	//重复性检查
	 void OnRepeatCheck(WPARAM wParam, LPARAM lParam);
	 //示值误差检查
	 void OnRealCheck(WPARAM wParam, LPARAM lParam);

public:
	// 异步函数
	 void Reset_Asyn(HWND hCallBackWnd=NULL);
	 void GetData1234_Asyn(HWND hCallBackWnd=NULL);
private:
	// 响应异步操作函数
	afx_msg void OnReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetData1234(WPARAM wParam, LPARAM lParam);

private:
	float m_fData1,m_fData2,m_fData3,m_fData4;

public:
	// 获取实时数据Data1
	inline float GetData1Value(void) const
	{
		return m_fData1;
	}
	// 获取实时数据Data2
	inline float GetData2Value(void) const
	{
		return m_fData2;
	}
	// 获取实时数据Data3
	inline float GetData3Value(void) const
	{
		return m_fData3;
	}
	// 获取实时数据Data4
	inline float GetData4Value(void) const
	{
		return m_fData4;
	}


public:
	// 退出线程操作
	void QuitThread(void);

private:
	// 初始化设备
	void InitializeInstrument(const enum InstrumentModel im);
	// 卸载设备
	void UninitializeInstrument(void);

public:
	float m_fRepatData[6],m_fRealData[6][4];
	// 是否终止检测
	BOOL m_bIsQuit;

private:
	CSmo* m_pSmo;
	// 线程退出标志
	bool m_bQuitThread;
};


