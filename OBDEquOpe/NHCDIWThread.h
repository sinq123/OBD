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
#define NHC_DIW_STATE_NOT_START				0x00	// 0:测试未开始
#define NHC_DIW_STATE_READY					0x01	// 1:准备就绪
#define NHC_DIW_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
#define NHC_DIW_STATE_ACCELERATION_1		0x03	// 3:加速，第一次
#define NHC_DIW_STATE_SLIP_1				0x04	// 4:滑行(小加载力)
#define NHC_DIW_STATE_WAIT_FOR_STOP			0x05	// 5:等待滚筒停止
#define NHC_DIW_STATE_ACCELERATION_2		0x06	// 6:加速，第二次
#define NHC_DIW_STATE_SLIP_2				0x07	// 7:滑行(大加载力)
#define NHC_DIW_STATE_FINISHED				0x08	// 8:测试完成(到达滑行区间低速度点,滚筒可能还在转)
#define NHC_DIW_STATE_ERROR					0x09	// 9:测试过程发生错误
///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(begin)
#define NHC_DIW_NO_ERROR					0x00	// 0:没有错误发生
#define NHC_DIW_COMMUNICATION_ERROR			0x01	// 1:与测功机通信错误
#define NHC_DIW_START_MMTIMER_FAIL			0x02	// 2: 启动多媒体定时失败
/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(end)

// CNHCDIWThread

class CNHCDIWThread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCDIWThread)

protected:
	CNHCDIWThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CNHCDIWThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()


public:
	// 开始基本惯量测试(异步)
	// (in)fLowerForce:小加载力，单位:N
	// (in)fUpperForce:大加载力，单位:N
	// (in)fLowerVelocity:低速点，停止测试速度，单位:km/h
	// (in)fUpperVelocity:高速点，开始测试速度，单位:km/h
	// (in)hCallBackWnd：回调窗口句柄
	void StartDIWTest_Asyn(const float fLowerForce, 
		const float fUpperForce,
		const float fLowerVelocity, 
		const float fUpperVelocity, 
		const HWND hCallBackWnd=NULL);

	// 停止基本惯量测试
	void StopDIWTest(void);

public:
	// 返回基本惯量测试状态
	inline UINT GetDIWState(void) const
	{
		return m_unDIWState;
	}

	// 返回小加载力滑行时间
	// 单位-ms
	inline UINT GetLowerSlipingTime(void) const
	{
		return m_unLowerSlipingTime;
	}

	// 返回大加载力滑行时间
	// 单位-ms
	inline UINT GetUpperSlipingTime(void) const
	{
		return m_unUpperSlipingTime;
	}

	// 返回平均加载力(小加载力)
	// 单位-N
	inline float GetAveLowerForce(void) const
	{
		return m_fAveLowerForce;
	}

	// 返回平均加载力(大加载力)
	// 单位-N
	inline float GetAveUpperForce(void) const
	{
		return m_fAveUpperForce;
	}

	// 返回最近错误代码
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

private:
	// 响应函数：StartDIWTest_Asyn
	afx_msg void OnStartDIWTest(WPARAM wParam, LPARAM lParam);

private:
	void DIWTest(void);
	void DIWTestStep1(void);  // 就绪准备
	void DIWTestStep2(void);  // 降举升器
	void DIWTestStep3(void);  // 加速
	void DIWTestStep4(void);  // 滑行(小加载力)
	void DIWTestStep5(void);  // 等待滚筒停止
	void DIWTestStep6(void);  // 加速
	void DIWTestStep7(void);  // 滑行(大加载力)

private:
	static void CALLBACK TimeProOfDIW(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// 小加载力滑行时间，单位:毫秒(ms)
	UINT m_unLowerSlipingTime;
	// 大加载力滑行时间，单位:毫秒(ms)
	UINT m_unUpperSlipingTime;

	// 实测平均加载力(小加载力),单位:N
	float m_fAveLowerForce;
	// 实测平均加载力(大加载力),单位:N
	float m_fAveUpperForce;

	// 测试阶段加载力总数(用于求加载力平均值)
	double m_dTotalForce;
	// 测试阶段加载力记录次数(用于求加载力平均值)
	UINT m_unTotalTime;

	// 开始计时速度，高速点，单位:km/h
	float m_fUpperVelocity;
	// 关电机的高速度点，即开始计时速度加上偏移，单位:km/h
	float m_fUpperVelocityWithOffset;
	// 停止计时速度，低速点，单位:km/h
	float m_fLowerVelocity;
	// 停止取秒表时间速度点，即是停止取秒表时间速度点减去偏移，单位:km/h
	float m_fLowerVelocityWithOffset;
	// 高速度点偏移,单位:km/h
	const int m_nUpperVelocityOffset;
	// 低速度点偏移,单位:km/h
	const int m_nLowerVelocityOffset;

	// 小加载力,单位:N
	float m_fLowerForce;
	// 大加载力,单位:N
	float m_fUpperForce;

	// 定时器分辨率,单位:ms
	UINT m_unTimerRes;
	// 多媒体定时器
	UINT m_unTimerIdOfDIW;

	// 基本惯量测试状态(宏定义)
	// NHC_DIW_STATE_NOT_START				0x00	// 0:测试未开始
	// NHC_DIW_STATE_READY					0x01	// 1:准备就绪
	// NHC_DIW_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
	// NHC_DIW_STATE_ACCELERATION_1			0x03	// 3:加速，第一次
	// NHC_DIW_STATE_SLIP_1					0x04	// 4:滑行(小加载力)
	// NHC_DIW_STATE_WAIT_FOR_STOP			0x05	// 5:等待滚筒停止
	// NHC_DIW_STATE_ACCELERATION_2			0x06	// 6:加速，第二次
	// NHC_DIW_STATE_SLIP_2					0x07	// 7:滑行(大加载力)
	// NHC_DIW_STATE_FINISHED				0x08	// 8:测试完成(到达滑行区间低速度点,滚筒可能还在转)
	// NHC_DIW_STATE_ERROR					0x09	// 9:测试过程发生错误
	UINT m_unDIWState;

	// 回调窗口句柄
	HWND m_hCallBackWnd; 

	// 错误代码
	// NHC_DIW_NO_ERROR						0x00	// 没有错误发生
	// NHC_DIW_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
	// NHC_DIW_START_MMTIMER_FAIL			0x02	// 启动多媒体定时失败
	DWORD m_dwLastErrorCode;

	// 测试间隔，单位:ms
	const UINT m_unTestInterval;
};