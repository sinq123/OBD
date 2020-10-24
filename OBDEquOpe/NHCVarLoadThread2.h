/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCVarLoadThread2.h
* 文件标识：
* 描述：南华测功机变载荷滑行（HJ/T290-2006）测试控制线程
*
* 版本：1.0
* 作者：hemincong
* 日期：2010-09-29
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2010-09-29
* 作者：hemincong
* 描述：正式发布使用
*/

#pragma once

// 自定义
#include "..\NHDetCommModu\NHDetCommModu.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")  // 安装多媒体定时器支持库,也可手动将winmm.lib添加到工程中

///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(begin)
#define NHC_VARLOAD_STATE_NOT_START				0x00	// 0:测试未开始
#define NHC_VARLOAD_STATE_READY					0x01	// 1:准备就绪
#define NHC_VARLOAD_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
#define NHC_VARLOAD_STATE_ACCELERATION  		0x03	// 3:加速
#define NHC_VARLOAD_STATE_SLIP  				0x04	// 4:滑行
#define NHC_VARLOAD_STATE_WAIT_FOR_STOP			0x05	// 5:等待滚筒停止
#define NHC_VARLOAD_STATE_FINISHED				0x06	// 6:测试完成
#define NHC_VARLOAD_STATE_ERROR					0x07	// 7:测试过程发生错误
///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(begin)
#define NHC_VARLOAD_NO_ERROR					0x00	// 0:没有错误发生
#define NHC_VARLOAD_COMMUNICATION_ERROR			0x01	// 1:与测功机通信错误
#define NHC_VARLOAD_START_MMTIMER_FAIL			0x02	// 2: 启动多媒体定时失败
/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(end)

class CNHCVarLoadThread2 :
	public CDynThread
{
	DECLARE_DYNCREATE(CNHCVarLoadThread2)

protected:
	DECLARE_MESSAGE_MAP()
public:
	CNHCVarLoadThread2(void);
	~CNHCVarLoadThread2(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	// 响应函数：StartVarLoadTest
	afx_msg void OnStartVarLoadTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam);

public:
	// 开始变载荷测试(异步)
	// (in)hCallBackWnd：回调窗口句柄
	void StartVarLoadTest_Asyn(const HWND hCallBackWnd=NULL);
	// 停止变载荷测试
	void StopVarLoadTest(void);

	// 返回变载荷测试状态
	inline UINT GetVarLoadState(void) const
	{
		return m_unVarLoadState;
	}

	// 返回变载滑行时间
	// (in)nSection：从0开始，到45，有46段
	// (in)
	// (return)：对应指定段滑行时间，单位-ms
	inline UINT GetVarLoadTime(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<46);
		return m_nSlideTime[nSection];
	}

	// 返回总区间滑行时间
	inline UINT GetVarLoadTotalTime(void) const
	{
		return m_nSlideTime[45];
	}

	// 返回变载滑行功率
	// (in)nSection：从0开始
	// (in)
	// (return)：对应指定段滑行功率，单位-KW
	inline float GetVarLoadPower(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<46);
		return m_fPowerAdd[nSection];	
	}

	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}
		
	inline void SetAccType(const int AccType) 
	{
		m_nAccType = AccType;
	}

private:
	void VarLoadTest(void);
	void VarLoadTestStep1(void);        // 就绪准备
	void VarLoadTestStep2(void);        // 降举升器
	void VarLoadTestStep3(void);		// 加速
	void VarLoadTestStep4(void);		// 滑行
	void VarLoadTestStep5(void);        // 等待滚筒停止

private:
	static void CALLBACK TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	//加速类型 0为电机加速 1为汽车加速
	int m_nAccType;
	// 开始计时速度，高速点，单位:km/h
	float m_fUpperVelocity;
	// 关电机的高速度点，即开始计时速度加上偏移，单位:km/h
	float m_fUpperVelocityWithOffset;
	// 停止计时速度，低速点，单位:km/h
	float m_fLowerVelocity;
	// 停止取秒表时间速度点，即是停止取秒表时间速度点减去偏移，单位:km/h
	float m_fLowerVelocityWithOffset;
	// 秒表中间速度点，单位:km/h
	float m_fMiddleVelocity;
	// 高速度点偏移,单位:km/h
	const int m_nUpperVelocityOffset;
	// 低速度点偏移,单位:km/h
	const int m_nLowerVelocityOffset;

	// 是否达到高速度点
	bool m_bIsSpeedUp;
	// 正在进行变载荷滑行的速度段,从1开始到45
	int m_nStatues;

	// 变载荷滑行每段的加载功率
	// 数据从m_fPowerAdd[1]开始为第一组
	float m_fPowerAdd[46];

	// 变载荷每段的滑行时间 
	// 数据从m_nSlideTime[0]开始为第一组
	// m_nSlideTime[46]最后一组是总滑行时间
	UINT m_nSlideTime[46];

	// 定时器分辨率,单位:ms
	UINT m_unTimerRes;
	// 多媒体定时器
	UINT m_unTimerIdOfVarLoad;

	// 变载荷测试状态(宏定义)
	//#define NHC_VARLOAD_STATE_NOT_START				0x00	// 0:测试未开始
	//#define NHC_VARLOAD_STATE_READY					0x01	// 1:准备就绪
	//#define NHC_VARLOAD_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
	//#define NHC_VARLOAD_STATE_ACCELERATION  			0x03	// 3:加速
	//#define NHC_VARLOAD_STATE_SLIP  					0x04	// 4:滑行
	//#define NHC_VARLOAD_STATE_WAIT_FOR_STOP			0x05	// 5:等待滚筒停止
	//#define NHC_VARLOAD_STATE_FINISHED				0x06	// 6:测试完成
	//#define NHC_VARLOAD_STATE_ERROR					0x07	// 7:测试过程发生错误
	UINT m_unVarLoadState;

	// 回调窗口句柄
	HWND m_hCallBackWnd; 

	// 错误代码
	// NHC_VARLOAD_NO_ERROR								0x00	// 没有错误发生
	// NHC_VARLOAD_COMMUNICATION_ERROR					0x01	// 与测功机通信错误
	// NHC_VARLOAD_START_MMTIMER_FAIL					0x02	// 启动多媒体定时失败
	DWORD m_dwLastErrorCode;

	// 测试间隔，单位:ms
	const UINT m_unTestInterval;
};

