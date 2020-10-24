/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCPLHPGB2018Thread.h
* 文件标识：
* 摘要：测功机寄生功率滑行线程
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-11-24
*
* 历史信息：
*
* 当前版本：1.0.3
* 完成日期：2013-07-31
* 作者：Chc
* 描述：修复寄生功率出现负数的BUG
*
* 当前版本：1.0.2
* 完成日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 当前版本：1.0.1
* 完成日期：2009-12-22
* 作者：Cui
* 描述：整理注释和代码格式
*
* 当前版本：1.0.0
* 完成日期：2009-11-24
* 作者：Cui
* 描述：正式发布使用
*
*
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
#pragma comment(lib, "winmm.lib")  // 安装多媒体定时器支持库,也可手动将winmm.lib添加到工程中。

///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(begin)
#define NHC_PLHP_STATE_NOT_START				0x00	// 0:测试未开始
#define NHC_PLHP_STATE_READY					0x01	// 1:准备就绪(初始化测试参数)
#define NHC_PLHP_STATE_LIFTBEAM_DOWN			0x02	// 2:降举升器
#define NHC_PLHP_STATE_ACCELERATION				0x03	// 3:加速
#define NHC_PLHP_STATE_SLIP						0x04	// 4:滑行
#define NHC_PLHP_STATE_FINISHED					0x05	// 5:测试完成(到达滑行区间低速度点,滚筒可能还在转)
#define NHC_PLHP_STATE_ERROR					0x06	// 6:测试过程发生错误
///////////////////////////////////////////////////////////////////////////////////////////////////测试状态(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(begin)
#define NHC_PLHP_NO_ERROR						0x00	// 没有错误发生
#define NHC_PLHP_COMMUNICATION_ERROR			0x01	// 与测功机通信错误
#define NHC_PLHP_START_MMTIMER_FAIL				0x02	// 启动多媒体定时失败
/////////////////////////////////////////////////////////////////////////////////////////////////// 错误代码(end)

// CNHCPLHPGB2018Thread

class CNHCPLHPGB2018Thread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCPLHPGB2018Thread)

protected:
	CNHCPLHPGB2018Thread();           // 动态创建所使用的受保护的构造函数
	virtual ~CNHCPLHPGB2018Thread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// 寄生功率类型
	enum PLHPType
	{
		PLHP_ASM,
		PLHP_VMAS,
		PLHP_LUGDOWN,
		PLHP_LUGDOWN_100,//起始速度100
	};

public:
	// 开始寄生功率(异步)
	// (in)pt：寄生功率类型（ASM，VMAS，LUGDOWN）
	// (in)rm：剩磁记录模式
	// (in)hCallBackWnd：回调窗口
	void StartPLHPTest_Asyn(const enum PLHPType pt, const HWND hCallBackWnd=NULL);

	// 停止寄生功率
	void StopPLHPTest(void);

public:
	// 返回寄生功率状态
	inline UINT GetPLHPState(void) const
	{
		return m_unPLHPState;
	}

	// 返回寄生功率滑行时间
	// (in)nSection：从0开始，ASM和VMAS有5段，LUGDOWN有9段
	// (return)：对应指定段滑行时间，单位-ms
	inline UINT GetPLHPTime(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<10);

		return m_unPLHPTime[nSection];
	}

	// 返回最近错误代码
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

	// 返回各个速度区间的剩磁平均值(功率模式)
	// (in)nSection：从0开始，ASM和VMAS有5段，LUGDOWN有9段
	// (return)：对应指定速度区间的剩磁平均值，单位:kW
	inline float GetRemanenceAveByPower(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<10);

		return m_fRemanenceAveByPower[nSection];
	}

	// 返回各个速度区间的剩磁平均值(力模式)
	// (in)nSection：从0开始，ASM和VMAS有5段，LUGDOWN有9段
	// (return)：对应指定速度区间的剩磁平均值，单位:N
	inline float GetRemanenceAveByForce(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<10);

		return m_fRemanenceAveByForce[nSection];
	}

private:
	// 响应函数：StartPLHPTest_Asyn
	afx_msg void OnStartPLHPTest(WPARAM wParam, LPARAM lParam);

private:
	// 此函数暂时不开放
	// 开始寄生功率(同步)
	void StartPLHPTest(const enum PLHPType pt, const HWND hCallBackWnd=NULL);

private:
	// 寄生功率测试
	void PLHPTest(void);
	void PLHPTestStep1(void);  // 就绪准备
	void PLHPTestStep2(void);  // 降举升器
	void PLHPTestStep3(void);  // 加速
	void PLHPTestStep4(void);  // 滑行(计时)

	inline void PLHPTestStep4_ASM(void);
	inline void PLHPTestStep4_VMAS(void);
	inline void PLHPTestStep4_LUGDOWN(void);
	inline void PLHPTestStep4_LUGDOWN_100(void);

	// 剩磁记录
	inline void RemanenceRecord_ASM(void);
	inline void RemanenceRecord_VMAS(void);
	inline void RemanenceRecord_LUGDOWN(void);
	inline void RemanenceRecord_LUGDOWN_100(void);
	inline void RemanenceRecordOfSection1(void);
	inline void RemanenceRecordOfSection2(void);
	inline void RemanenceRecordOfSection3(void);
	inline void RemanenceRecordOfSection4(void);
	inline void RemanenceRecordOfSection5(void);
	inline void RemanenceRecordOfSection6(void);
	inline void RemanenceRecordOfSection7(void);
	inline void RemanenceRecordOfSection8(void);
	inline void RemanenceRecordOfSection9(void);

private:
	// 定时器分辨率
	UINT m_unTimerRes;
	// 寄生功率滑行定时器
	UINT m_unTimerIdOfPLHP;
	static void CALLBACK TimeProOfPLHP(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// 寄生功率测试状态,
	// NHC_PLHP_STATE_NOT_START					0x00	// 0:测试未开始
	// NHC_PLHP_STATE_READY						0x01	// 1:准备就绪(初始化测试参数)
	// NHC_PLHP_STATE_LIFTBEAM_DOWN				0x02	// 2:降举升器
	// NHC_PLHP_STATE_ACCELERATION				0x03	// 3:加速
	// NHC_PLHP_STATE_SLIP						0x04	// 4:滑行
	// NHC_PLHP_STATE_FINISHED					0x05	// 5:测试完成(到达滑行区间低速度点,滚筒可能还在转)
	// NHC_PLHP_STATE_ERROR						0x06	// 6:测试过程发生错误
	UINT m_unPLHPState;

	// 滑行状态
	// 0-未开始
	// 内部使用
	UINT m_unSlipState;

	// 回调窗口句柄
	HWND m_hCallBackWnd; 

	// 错误代码
	// NHC_PLHP_NO_ERROR						0x00	// 没有错误发生
	// NHC_PLHP_COMMUNICATION_ERROR				0x01	// 与测功机通信错误
	// NHC_PLHP_START_MMTIMER_FAIL				0x02	// 启动多媒体定时失败
	DWORD m_dwLastErrorCode;

private:
	// 寄生功率类型
	enum PLHPType m_PLHPType;
	// 测试间隔，单位-ms
	const UINT m_unTestInterval;
	// 停电机速度
	float m_fVelocityOfMotorOff;
	// 滑行时间辅助中间变量，用于LUGDOWN计时
	// 单位-ms
	UINT m_unMiddleTimeOld;
	UINT m_unMiddleTimeNew;
	// 寄生功率各速度段时间	
	// 单位-ms
	UINT m_unPLHPTime[9];
	UINT m_unPLHPTime10[9];	// 每10km/h记录一个时间
	 // 寄生功率各速度段剩磁累计值,功率模式,单位-kW
	double m_dRemanenceSumByPower[9];
	 // 寄生功率各速度段剩磁累计值,力模式,单位-N
	double m_dRemanenceSumByForce[9];
	 // 寄生功率各速度段获取剩磁累计次数
	int m_nRemanenceTimes[9];
	 // 寄生功率各速度段剩磁平均值,功率模式,单位-kW
	float m_fRemanenceAveByPower[9];
	 // 寄生功率各速度段剩磁平均值,力模式,单位-N
	float m_fRemanenceAveByForce[9];
};


