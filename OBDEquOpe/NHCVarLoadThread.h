/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCVarLoadThread.h
* 文件标识：
* 描述：南华测功机响应时间测试
*
* 版本：1.0
* 作者：Qin
* 日期：2010-06-08
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2010-06-08
* 作者：Qin
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

// CNHCVarLoadThread

class CNHCVarLoadThread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCVarLoadThread)

protected:
	CNHCVarLoadThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CNHCVarLoadThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	// 响应函数：StartVarLoadTest
	afx_msg void OnStartVarLoadTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()


public:
	// 开始变载荷测试(异步)
	// (in)hCallBackWnd：回调窗口句柄
	void StartVarLoadTest_Asyn(const BYTE bTestType, const HWND hCallBackWnd=NULL);

	// 停止变载荷测试
	void StopVarLoadTest(void);

public:
	// 返回变载荷测试状态
	inline UINT GetVarLoadState(void) const
	{
		return m_unVarLoadState;
	}
	// 返回变载滑行时间
	// (in)nSection：从0开始，ASM和VMAS有21段，LUGDOWN有36段
	// (in)
	// (return)：对应指定段滑行时间，单位-ms
	inline UINT GetVarLoadTime(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<36);
		if (ASM == m_TestType)
		{
			return m_nAsmSlideTime[nSection];
		}
		else if (VMAS == m_TestType)
		{
			return m_nVmasSlideTime[nSection];
		}
		else
		{
			return m_nLugdownSlideTime[nSection];
		}
	}
		// 返回变载滑行功率
	// (in)nSection：从0开始，ASM和VMAS有21段，LUGDOWN有36段
	// (in)
	// (return)：对应指定段滑行功率，单位-KW
	inline float GetVarLoadPower(const int nSection) const
	{
		if (ASM == m_TestType)
		{
			ASSERT(nSection>=0 && nSection<21);
			return m_fAsmPowerAdd[nSection];
		}
		else if (VMAS == m_TestType)
		{
			ASSERT(nSection>=0 && nSection<21);
			return m_fVmasPowerAdd[nSection];
		}
		else
		{
			ASSERT(nSection>=0 && nSection<36);
			return m_fLugdownPowerAdd[nSection];
		}
	}
	// 返回最近错误代码
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

	// 返回总区间滑行时间
	inline UINT GetVarLoadTotalTime(void) const
	{
		if (ASM == m_TestType)
		{
			return m_nAsmSlideTime[20];
		}
		else if (VMAS == m_TestType)
		{
			return m_nVmasSlideTime[20];
		}
		else
		{
			return m_nLugdownSlideTime[35];
		}
	}


private:
	void VarLoadTest(void);
	void VarLoadTestStep1(void);           // 就绪准备
	void VarLoadTestStep2(void);           // 降举升器
	void VarLoadTestStep3(void);           // 加速
	void VarLoadTestOfLugdownStep4(void);   // Lugdwon滑行
	void VarLoadTestOfAsmStep4(void);       // ASM滑行
	void VarLoadTestOfVmasStep4(void);      // Lugdwon滑行
	void VarLoadTestStep5(void);           // 等待滚筒停止
	void CalcVarLoadResult(void);          // 计算测试结果

private:
	static void CALLBACK TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// 测试类型 ， ASM 1；VMAS 2；LUGDOWN 3
	BYTE m_TestType;
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

	// LUGDOWN变载荷滑行每段的加载功率
	// 数据从m_fLugdownPowerAdd[1]开始为第一组
	float m_fLugdownPowerAdd[36];

	// ASM变载荷滑行每段的加载功率
	// 数据从m_fAsmPowerAdd[1]开始为第一组
	float m_fAsmPowerAdd[21];

	// VMAS变载荷滑行每段的加载功率
	// 数据从m_fVmasPowerAdd[1]开始为第一组
	float m_fVmasPowerAdd[21];

	// LUGDOWN变载荷每段的滑行时间 
	// 数据从m_nLugdownSlideTime[0]开始为第一组
	// m_nLugdownSlideTime[35]最后一组是总滑行时间
	UINT m_nLugdownSlideTime[36];

	// ASM变载荷每段的滑行时间
	// 数据从m_nAsmSlideTime[0]开始为第一组
	// m_nAsmSlideTime[20]最后一组是总滑行时间
	UINT m_nAsmSlideTime[21];

	// VMAS变载荷每段的滑行时间
	// 数据从m_nVmasSlideTime[0]开始为第一组
	// m_nVmasSlideTime[20]最后一组是总滑行时间
	UINT m_nVmasSlideTime[21];

	// 定时器分辨率,单位:ms
	UINT m_unTimerRes;
	// 多媒体定时器
	UINT m_unTimerIdOfVarLoad;

	// 变载荷测试状态(宏定义)
	// NHC_VARLOAD_STATE_NOT_START				0x00	// 0:测试未开始
	// NHC_VARLOAD_STATE_READY					0x01	// 1:准备就绪
	// NHC_VARLOAD_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
	// NHC_VARLOAD_STATE_ACCELERATION			0x03	// 3:加速
	// NHC_VARLOAD_STATE_SLIP					0x04	// 4:滑行(变载荷测试中)
	// NHC_VARLOAD_STATE_WAIT_FOR_STOP			0x05	// 5:等待滚筒停止
	// NHC_VARLOAD_STATE_FINISHED				0x06	// 6:测试完成(到达滑行区间低速度点,滚筒可能还在转)
	// NHC_VARLOAD_STATE_ERROR					0x07	// 7:测试过程发生错误
	UINT m_unVarLoadState;

	// 回调窗口句柄
	HWND m_hCallBackWnd; 

	// 错误代码
	// NHC_VARLOAD_NO_ERROR						0x00	// 没有错误发生
	// NHC_VARLOAD_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
	// NHC_VARLOAD_START_MMTIMER_FAIL			0x02	// 启动多媒体定时失败
	DWORD m_dwLastErrorCode;

	// 测试间隔，单位:ms
	const UINT m_unTestInterval;
};