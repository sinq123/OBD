/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCConstLoadThread.h
* 文件标识：
* 摘要：测功机加载滑行控制线程
*
* 版本：1.0
* 作者：Cui
* 日期：2009-12-10
*
* 历史信息：
*
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
#define NHC_CL_STATE_NOT_START				0x00	// 0:测试未开始
#define NHC_CL_STATE_READY					0x01	// 1:准备就绪
#define NHC_CL_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
#define NHC_CL_STATE_ACCELERATION			0x03	// 3:加速
#define NHC_CL_STATE_SLIP					0x04	// 4:滑行
#define NHC_CL_STATE_FINISHED				0x05	// 5:测试完成(到达滑行区间低速度点,滚筒可能还在转)
#define NHC_CL_STATE_ERROR					0x06	// 6:测试过程发生错误
///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(begin)
#define NHC_CL_NO_ERROR						0x00	// 没有错误发生
#define NHC_CL_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
#define NHC_CL_START_MMTIMER_FAIL			0x02	// 启动多媒体定时失败
/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(end)

//class CNHCConstLoadThread : public CNHCCtrlThread
class CNHCConstLoadThread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCConstLoadThread)

public:
	CNHCConstLoadThread(void);
	~CNHCConstLoadThread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// 开始定载荷(异步)
	// (in)fUpperVelocity:高速点，开始测试速度，单位-km/h
	// (in)fLowerVelocity:低速点，停止测试速度，单位-km/h
	// (in)fLoadPower:加载功率，单位-kW
	// (in)hCallBackWnd：回调窗口句柄
	void StartConstLoadTest_Asyn(const float fUpperVelocity, const float fLowerVelocity, const float fLoadPower, const HWND hCallBackWnd=NULL);

	// 停止定载荷
	void StopConstLoadTest(void);

public:
	// 返回定载荷状态
	inline UINT GetConstLoadState(void) const
	{
		return m_unConstLoadState;
	}

	// 返回定载荷滑行时间
	// 单位-ms
	inline UINT GetConstLoadTime(void) const
	{
		return m_unConstLoadTime;
	}

	// 返回最近错误代码
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

private:
	// 响应函数：StartConstLoadTest_Asyn
	afx_msg void OnStartConstLoadTest(WPARAM wParam, LPARAM lParam);

private:
	// 此函数暂时不开放
	// 开始定载荷(同步)
	// (in)fUpperVelocity:高速点，开始测试速度，单位-km/h
	// (in)fLowerVelocity:低速点，停止测试速度，单位-km/h
	// (in)fLoadPower:加载功率，单位-kW
	void StartConstLoadTest(const float fUpperVelocity, const float fLowerVelocity, const float fLoadPower, const HWND hCallBackWnd=NULL);

private:
	void ConstLoadTest(void);
	void ConstLoadTestStep1(void);  // 就绪准备
	void ConstLoadTestStep2(void);  // 降举升器
	void ConstLoadTestStep3(void);  // 加速
	void ConstLoadTestStep4(void);  // 滑行(加载计时阶段)

private:
	// 定载荷滑行时间，单位-毫秒(ms)
	UINT m_unConstLoadTime;

private:
	// 定载荷开始计时速度，高速点，单位-km/h
	float m_fUpperVelocity;
	// 定载荷关电机的高速度点，即是定载荷高速点加上偏移，单位-km/h
	float m_fUpperVelocityWithOffset;
	// 定载荷停止计时速度，低速点，单位-km/h
	float m_fLowerVelocity;
	// 定载荷停止取秒表时间速度，即是定载荷低速点减去偏移，单位-km/h
	float m_fLowerVelocityWithOffset;
	// 定载荷加载功率，单位-kW
	float m_fLoadPower;

private:
	// 定时器分辨率
	UINT m_unTimerRes;

	// 定载荷滑行定时器
	UINT m_unTimerIdOfConstLoad;
	static void CALLBACK TimeProOfConstLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// 定载荷测试状态,
	// NHC_CL_STATE_NOT_START				0x00	// 0:测试未开始
	// NHC_CL_STATE_READY					0x01	// 1:准备就绪
	// NHC_CL_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
	// NHC_CL_STATE_ACCELERATION			0x03	// 3:加速
	// NHC_CL_STATE_SLIP					0x04	// 4:滑行
	// NHC_CL_STATE_FINISHED				0x05	// 5:测试完成(到达滑行区间低速度点,滚筒可能还在转)
	// NHC_CL_STATE_ERROR					0x06	// 6:测试过程发生错误
	UINT m_unConstLoadState;

	// 高速度点偏移
	const int m_nUpperVelocityOffset;
	// 低速度点偏移
	const int m_nLowerVelocityOffset;

	// 回调窗口句柄
	HWND m_hCallBackWnd; 

	// 错误代码
	// NHC_CL_NO_ERROR						0x00	// 没有错误发生
	// NHC_CL_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
	// NHC_CL_START_MMTIMER_FAIL			0x02	// 启动多媒体定时失败
	DWORD m_dwLastErrorCode;

	// 测试间隔，单位-ms
	const UINT m_unTestInterval;
};
