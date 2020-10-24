#include "StdAfx.h"
#include "NHCResponseTime2Thread.h"

IMPLEMENT_DYNCREATE(CNHCResponseTime2Thread, CDynThread)

CNHCResponseTime2Thread::CNHCResponseTime2Thread(void)
	: m_unTimerRes(50)
	, m_unTimerIdOfResponseTime(NULL)
	, m_unResponseTimeState(NHC_RT_STATE_NOT_START)
	, m_unTestInterval(200)
	, m_hCallBackWnd(NULL)
	, m_nElapsedTime(0)
	, m_fLoad_High(1323.0f)
	, m_fLoad_Low(1719.0f)
	, m_fSpeed_High(56.3f)
	, m_fSpeed_Low(40.0f)
	, m_fTopSlipSpeed(64.4f)
	, bIsSaveDate(false)
{
}

CNHCResponseTime2Thread::~CNHCResponseTime2Thread(void)
{
}

BOOL CNHCResponseTime2Thread::InitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 设置多媒体计时器最低分辨率
	TIMECAPS tc;
	if (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(TIMECAPS)))
	{
		// 获取系统支持分辨率，并选择适当的分辨率(>=50ms)
		m_unTimerRes = min(max(tc.wPeriodMin, 50), tc.wPeriodMax);
	}
	else
	{
		// 获取系统支持分辨率出错，直接设为50ms
		m_unTimerRes = 50;
	}
	timeBeginPeriod(m_unTimerRes);

	return CDynThread::InitInstance();
}

int CNHCResponseTime2Thread::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 关闭多媒体定时器
	if (NULL != m_unTimerIdOfResponseTime)
	{
		timeKillEvent(m_unTimerIdOfResponseTime);
		m_unTimerIdOfResponseTime = NULL;
	}

	// 清除多媒体计时器最低分辨率
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCResponseTime2Thread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_RESPONSE_TIME_TEST, OnStartResponseTimeTest)
END_MESSAGE_MAP()

void CNHCResponseTime2Thread::OnStartResponseTimeTest(WPARAM wParam, LPARAM lParam)
{
	// 设置状态为准备
	m_unResponseTimeState = NHC_RT_STATE_READY;
	ResponseTimeTest();
}

void CNHCResponseTime2Thread::StartResponseTimeTest_Asyn(const float fF_High, const float fF_Low, const float fF_SpeedHigh, const float fF_SpeedLow, const float fF_TopSpeed, const HWND hCallBackWnd)
{
	m_fLoad_High = fF_High;
	m_fLoad_Low = fF_Low;
	m_fSpeed_High = fF_SpeedHigh;
	m_fSpeed_Low = fF_SpeedLow;
	m_fTopSlipSpeed = fF_TopSpeed;

	m_hCallBackWnd = hCallBackWnd;

	PostThreadMessage(WM_DYN_START_RESPONSE_TIME_TEST, 0, 0);
}

void CNHCResponseTime2Thread::ResponseTimeTest(void)
{
	switch (m_unResponseTimeState)
	{
	case NHC_RT_STATE_READY:
		ResponseTimeTestStep1();  // 就绪准备
		break;
	case NHC_RT_STATE_LIFTBEAM_DOWN:
		ResponseTimeTestStep2();  // 降举升器
		break;
	case NHC_RT_STATE_ACCELERATION:
		ResponseTimeTestStep3();  // 加速
		break;
	case NHC_RT_STATE_SLIP:
		ResponseTimeTestStep4();  // 滑行(加载计时阶段)
		break;
	case NHC_RT_STATE_BRAKE_High:
		ResponseTimeTestStep5();  // 高速点加载力
		break;
	case NHC_RT_STATE_BRAKE_Low:
		ResponseTimeTestStep6();  // 低速点加载力
		break;
	case NHC_RT_STATE_STABLE:
		ResponseTimeTestStep7();  // 稳定
		break;
	default:
		break;
	}

}

void CNHCResponseTime2Thread::ResponseTimeTestStep1(void)
{

	// 退出所有控制模式
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// 测功机操作失败
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_COMMUNICATION_ERROR;
		return;
	}
	// 降下举升器(必须)
	LiftBeamDown();
	// 查询举升器状态
	GetStatus();
	if (m_btStatus.test(1))
	{
		// 举升器已经完全降下
		// 进入加速了流程
		// 设状态为：加速
		m_unResponseTimeState = NHC_RT_STATE_ACCELERATION;
		// 启动电机，开始加速
		MotorOn();
	}
	else
	{
		// 举升器没有完全降下
		// 进入降下举升器流程
		// 设置状态为：降举升器
		m_unResponseTimeState = NHC_RT_STATE_LIFTBEAM_DOWN;
	}
	// 启动多媒体定时器
	if (NULL != m_unTimerIdOfResponseTime)
	{
		timeKillEvent(m_unTimerIdOfResponseTime);
		m_unTimerIdOfResponseTime = NULL;
	}
	m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfResponseTime)
	{
		// 启动多媒体定时器失败
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCResponseTime2Thread::ResponseTimeTestStep2(void)
{
	// 降举升器流程

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// 测功机操作失败
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}
		return;
	}
	if (m_btStatus.test(1))
	{
		// 举升器已经降下

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// 启动电机，开始加速
		MotorOn();

		// 开始加载响应时间控制模式
		ResponseTimeControlModeStart();
		// 进入加速流程
		// 设置状态为：加速
		m_unResponseTimeState = NHC_RT_STATE_ACCELERATION;

		// 重新启动定时器
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// 启动多媒体定时器失败
			m_unResponseTimeState = NHC_RT_STATE_ERROR;
			m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCResponseTime2Thread::ResponseTimeTestStep3(void)
{
	RefreshRealTimeData();

	if (m_f_ResT_V[NHC_RT_SAMPLING_NUM-1] > m_fTopSlipSpeed)
	{
		// 到达停止速度

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// 停止电机
		MotorOff();

		// 进入滑行流程
		// 设置状态为：滑行
		m_unResponseTimeState = NHC_RT_STATE_SLIP;

		// 重新启动定时器
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// 启动多媒体定时器失败
			m_unResponseTimeState = NHC_RT_STATE_ERROR;
			m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCResponseTime2Thread::ResponseTimeTestStep4(void)
{
	RefreshRealTimeData();

	if(m_f_ResT_V[NHC_RT_SAMPLING_NUM-1] <= m_fSpeed_High)
	{
		bIsSaveDate = true;
		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// 设置状态为: 高速点加载力
		m_unResponseTimeState = NHC_RT_STATE_BRAKE_High;
		// 高速点加载力
		SetConstantForceTarget(m_fLoad_High);
		ConstantForceControlModeOn();

		// 重新启动定时器
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// 启动多媒体定时器失败
			m_unResponseTimeState = NHC_RT_STATE_ERROR;
			m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
			return;
		}
	}
}
void CNHCResponseTime2Thread::ResponseTimeTestStep5(void)
{
	RefreshRealTimeData();

	if(m_f_ResT_V[NHC_RT_SAMPLING_NUM-1] <= m_fSpeed_Low)
	{
		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// 设置状态为: 低速点加载力
		m_unResponseTimeState = NHC_RT_STATE_BRAKE_Low;
		// 低速点加载力
		SetConstantForceTarget(m_fLoad_Low);
		ConstantForceControlModeOn();

		m_nElapsedTime = 0;

		// 重新启动定时器  刷新间隔
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// 启动多媒体定时器失败
			m_unResponseTimeState = NHC_RT_STATE_ERROR;
			m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
			return;
		}
	}
}
void CNHCResponseTime2Thread::ResponseTimeTestStep6(void)
{
	RefreshRealTimeData();

	if(m_f_ResT_F[NHC_RT_SAMPLING_NUM-1] > m_fLoad_Low*98/100 && m_f_ResT_F[NHC_RT_SAMPLING_NUM-1] < m_fLoad_Low*102/100)
	{
		m_nElapsedTime++;
	}
	else
	{
		m_nElapsedTime = 0;
	}

	if(m_nElapsedTime > 5)
	{
		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}
		//设置状态 稳定
		m_unResponseTimeState = NHC_RT_STATE_STABLE;
		// 重新启动定时器
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// 启动多媒体定时器失败
			m_unResponseTimeState = NHC_RT_STATE_ERROR;
			m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
			return;
		}
	}
}
void CNHCResponseTime2Thread::ResponseTimeTestStep7(void)
{
	ResponseTimeControlModeEnd();
	ControlModeOff();
	bIsSaveDate =false;
	// 停止定时器
	if (NULL != m_unTimerIdOfResponseTime)
	{
		timeKillEvent(m_unTimerIdOfResponseTime);
		m_unTimerIdOfResponseTime = NULL;
	}

	m_unResponseTimeState = NHC_RT_STATE_FINISHED;

	// 发消息到主线程窗口通知滑行计时已经完成
	if (NULL != m_hCallBackWnd)
	{
		//::PostMessage(m_hCallBackWnd, WM_DYN_RESPONSE_TIME_TEST_FINISHED, 0, 0);
		m_hCallBackWnd = NULL;
	}
}

void CNHCResponseTime2Thread::TimeProOfResponseTime(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCResponseTime2Thread* pThread = reinterpret_cast<CNHCResponseTime2Thread*>(dwUser);
	pThread->ResponseTimeTest();
	//TRACE("ResponseTimeTest is executed\n\n");
}

void CNHCResponseTime2Thread::RefreshRealTimeData(void)
{
	if (DYN_WR_SUCCEED != GetResponseTimeData(m_f_ResT_V,m_f_ResT_F,NHC_RT_SAMPLING_NUM))
	{
		// 执行测功机操作失败
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}
		return;
	}
	if(bIsSaveDate)
	{
		 //发消息到主线程窗口通知保存实时数据
		if (NULL != m_hCallBackWnd)
		{
			::PostMessage(m_hCallBackWnd, WM_DYN_SAVE_RESPONSE_TIME_DATA, 0, 0);
		}
	}
}