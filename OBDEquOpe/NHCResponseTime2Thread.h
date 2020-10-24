#pragma once
// 自定义
//#include "..\NHDef\NHDef.h"
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
#define NHC_RT_STATE_NOT_START				0x00	// 0:测试未开始
#define NHC_RT_STATE_READY					0x01	// 1:准备就绪
#define NHC_RT_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
#define NHC_RT_STATE_ACCELERATION			0x03	// 3:加速
#define NHC_RT_STATE_SLIP					0x04	// 4:滑行
#define NHC_RT_STATE_BRAKE_High				0x05	// 5:高速点加载力
#define NHC_RT_STATE_BRAKE_Low				0x06	// 6:低速点加载力
#define NHC_RT_STATE_STABLE					0x07	// 7:稳定
#define NHC_RT_STATE_FINISHED				0x08	// 8:测试完成(到达滑行区间低速度点,滚筒可能还在转)
#define NHC_RT_STATE_ERROR					0x09	// 9:测试过程发生错误
///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(begin)
#define NHC_RT_NO_ERROR						0x00	// 没有错误发生
#define NHC_RT_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
#define NHC_RT_START_MMTIMER_FAIL			0x02	// 启动多媒体定时失败
/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(end)

#define NHC_RT_SAMPLING_NUM					10		// 采样数量
class CNHCResponseTime2Thread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCResponseTime2Thread)
public:
	CNHCResponseTime2Thread(void);
	~CNHCResponseTime2Thread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP();
private:
	afx_msg void OnStartResponseTimeTest(WPARAM wParam, LPARAM lParam);

private:
	void ResponseTimeTest(void);
	void ResponseTimeTestStep1(void); // 就绪准备
	void ResponseTimeTestStep2(void); // 降举升器
	void ResponseTimeTestStep3(void); // 加速			速度达到64.4 km/h  开始滑行
	void ResponseTimeTestStep4(void); // 滑行			速度到达56.3 km/h  高速点加载力
	void ResponseTimeTestStep5(void); // 高速点加载力	速度到达40   km/h  低速点加载力
	void ResponseTimeTestStep6(void); // 低速点加载力
	void ResponseTimeTestStep7(void); // 稳定
	//取实时数据
	void RefreshRealTimeData(void);
public:
	// fF_High 制动力高点
	// fF_Low  制动力低点
	// (in)hCallBackWnd：回调窗口
	void StartResponseTimeTest_Asyn(const float fF_High, const float fF_Low, const float fF_SpeedHigh, const float fF_SpeedLow, const float fF_TopSpeed,const HWND hCallBackWnd);

	void EndResponseTimeTest();

	///////// 响应时间测试设定参数////Begin
	// 高速点加载力
	float m_fLoad_High;
	// 低速点加载力
	float m_fLoad_Low;
	// 高速点速度
	float m_fSpeed_High;
	// 低速点速度
	float m_fSpeed_Low;
	// 开始滑行速度
	float m_fTopSlipSpeed;
	///////// 响应时间测试设定参数////End

	float m_f_ResT_V[NHC_RT_SAMPLING_NUM];
	float m_f_ResT_F[NHC_RT_SAMPLING_NUM];


private:
	// 定时器分辨率
	UINT m_unTimerRes;

	// 定载荷滑行定时器
	UINT m_unTimerIdOfResponseTime;
	static void CALLBACK TimeProOfResponseTime(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	bool bIsSaveDate;

public:
	// 响应时间测试状态,
	// NHC_RT_STATE_NOT_START				0x00	// 0:测试未开始
	// NHC_RT_STATE_READY					0x01	// 1:准备就绪
	// NHC_RT_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
	// NHC_RT_STATE_ACCELERATION			0x03	// 3:加速
	// NHC_RT_STATE_SLIP					0x04	// 4:滑行
	// NHC_RT_STATE_BRAKE_High				0x05	// 5:高速点加载力
	// NHC_RT_STATE_BRAKE_Low				0x06	// 6:低速点加载力
	// NHC_RT_STATE_STABLE					0x07	// 7:稳定
	// NHC_RT_STATE_FINISHED				0x08	// 8:测试完成(到达滑行区间低速度点,滚筒可能还在转)
	// NHC_RT_STATE_ERROR					0x09	// 9:测试过程发生错误
	UINT m_unResponseTimeState;
	// 错误代码
	// NHC_RT_NO_ERROR						0x00	// 没有错误发生
	// NHC_RT_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
	// NHC_RT_START_MMTIMER_FAIL			0x02	// 启动多媒体定时失败
	DWORD m_dwLastErrorCode;

	// 回调窗口句柄
	HWND m_hCallBackWnd; 

	// 测试间隔，单位-ms
	const UINT m_unTestInterval;
	// 流逝时间
	int m_nElapsedTime;

public:
	// 返回响应时间状态
	inline UINT GetResponseTimeState(void) const
	{
		return m_unResponseTimeState;
	}
	// 返回最近错误代码
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}
	// 返回速度实时数据
	inline float GetRealTimeVDate(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<NHC_RT_SAMPLING_NUM);

		return m_f_ResT_V[nSection];
	}
	// 返回力实时数据
	inline float GetRealTimeFDate(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<NHC_RT_SAMPLING_NUM);

		return m_f_ResT_F[nSection];
	}
};
