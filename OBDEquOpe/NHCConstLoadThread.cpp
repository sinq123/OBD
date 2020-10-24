#include "StdAfx.h"
#include "NHCConstLoadThread.h"

IMPLEMENT_DYNCREATE(CNHCConstLoadThread, CDynThread)

CNHCConstLoadThread::CNHCConstLoadThread(void)
: m_unConstLoadTime(0)
, m_fUpperVelocity(0.0f)
, m_fLowerVelocity(0.0f)
, m_fLoadPower(0.0f)
, m_unTimerRes(50)
, m_unConstLoadState(NHC_CL_STATE_NOT_START)
, m_unTimerIdOfConstLoad(NULL)
, m_nUpperVelocityOffset(2)
, m_nLowerVelocityOffset(3)
, m_hCallBackWnd(NULL)
, m_dwLastErrorCode(NHC_CL_NO_ERROR)
, m_unTestInterval(200)
{

}

CNHCConstLoadThread::~CNHCConstLoadThread(void)
{
}

BOOL CNHCConstLoadThread::InitInstance()
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

int CNHCConstLoadThread::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 关闭多媒体定时器
	if (NULL != m_unTimerIdOfConstLoad)
	{
		timeKillEvent(m_unTimerIdOfConstLoad);
		m_unTimerIdOfConstLoad = NULL;
	}

	// 清除多媒体计时器最低分辨率
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCConstLoadThread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_CONST_LOAD_TEST, OnStartConstLoadTest)
END_MESSAGE_MAP()

void CNHCConstLoadThread::StartConstLoadTest_Asyn(const float fUpperVelocity, const float fLowerVelocity, const float fLoadPower, const HWND hCallBackWnd)
{
	ASSERT(fUpperVelocity > fLowerVelocity);
	ASSERT(fUpperVelocity>0.0f && fUpperVelocity<100.0f);
	ASSERT(fLowerVelocity>0.0f);
	ASSERT(fLoadPower>0.0f && fLoadPower<99.0f);

	m_fUpperVelocity = fUpperVelocity;
	m_fLowerVelocity = fLowerVelocity;
	m_fLoadPower = fLoadPower;
	m_hCallBackWnd = hCallBackWnd;

	PostThreadMessage(WM_DYN_START_CONST_LOAD_TEST, 0, 0);
}

void CNHCConstLoadThread::OnStartConstLoadTest(WPARAM wParam, LPARAM lParam)
{
	// 设状态为：就绪准备
	m_unConstLoadState = NHC_CL_STATE_READY;
	ConstLoadTest();
}

void CNHCConstLoadThread::StartConstLoadTest(const float fUpperVelocity, const float fLowerVelocity, const float fLoadPower, const HWND hCallBackWnd)
{
	ASSERT(fUpperVelocity > fLowerVelocity);
	ASSERT(fUpperVelocity>0.0f && fUpperVelocity<100.0f);
	ASSERT(fLowerVelocity>0.0f);
	ASSERT(fLoadPower>0.0f && fLoadPower<99.0f);

	m_fUpperVelocity = fUpperVelocity;
	m_fLowerVelocity = fLowerVelocity;
	m_fLoadPower = fLoadPower;
	m_hCallBackWnd = hCallBackWnd;

	// 设状态为：就绪准备
	m_unConstLoadState = NHC_CL_STATE_READY;
	ConstLoadTest();
}

void CNHCConstLoadThread::ConstLoadTest(void)
{
	switch (m_unConstLoadState)
	{
	case NHC_CL_STATE_READY:
		ConstLoadTestStep1();  // 就绪准备
		break;
	case NHC_CL_STATE_LIFTBEAM_DOWN:
		ConstLoadTestStep2();  // 降举升器
		break;
	case NHC_CL_STATE_ACCELERATION:
		ConstLoadTestStep3();  // 加速
		break;
	case NHC_CL_STATE_SLIP:
		ConstLoadTestStep4();  // 滑行(加载计时阶段)
		break;
	default:
		break;
	}
}

void CNHCConstLoadThread::ConstLoadTestStep1(void)
{
	// 定载荷步骤-准备就绪

	// 下面初始化定载荷测试参数

	m_fUpperVelocityWithOffset = m_fUpperVelocity + m_nUpperVelocityOffset;
	m_fLowerVelocityWithOffset = m_fLowerVelocity - m_nLowerVelocityOffset;
	ASSERT(m_fUpperVelocityWithOffset > m_fLowerVelocityWithOffset);
	if (m_fLowerVelocityWithOffset < 10e-6f)
	{
		m_fLowerVelocityWithOffset = 0.0f;
	}

	// 重设定载荷滑行时间
	m_unConstLoadTime = 0;

	// 退出所有控制模式
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// 测功机操作失败
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;
		return;
	}
	// 初始化测功机秒表1
	KillTimer1();
	// 设置秒表1开始速度
	SetUpperVelocityOfTimer1(m_fUpperVelocity);
	// 设置秒表1停止速度
	SetLowerVelocityOfTimer1(m_fLowerVelocity);
	// 设置恒功率目标值
	SetConstantPowerTarget(m_fLoadPower);
	// 降下举升器(必须)
	LiftBeamDown();
	// 查询举升器状态
	GetStatus();
	if (m_btStatus.test(1))
	{
		// 举升器已经完全降下
		// 进入加速了流程
		// 设状态为：加速
		m_unConstLoadState = NHC_CL_STATE_ACCELERATION;
		// 启动电机，开始加速
		MotorOn();
	}
	else
	{
		// 举升器没有完全降下
		// 进入降下举升器流程
		// 设置状态为：降举升器
		m_unConstLoadState = NHC_CL_STATE_LIFTBEAM_DOWN;
	}
	// 启动多媒体定时器
	if (NULL != m_unTimerIdOfConstLoad)
	{
		timeKillEvent(m_unTimerIdOfConstLoad);
		m_unTimerIdOfConstLoad = NULL;
	}
	m_unTimerIdOfConstLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfConstLoad, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfConstLoad)
	{
		// 启动多媒体定时器失败
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCConstLoadThread::ConstLoadTestStep2(void)
{
	// 定载荷步骤-降举升器流程

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// 测功机操作失败
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}
		return;
	}
	if (m_btStatus.test(1))
	{
		// 举升器已经降下

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}

		// 启动电机，开始加速
		MotorOn();

		// 进入加速流程
		// 设置状态为：加速
		m_unConstLoadState = NHC_CL_STATE_ACCELERATION;

		// 重新启动定时器
		m_unTimerIdOfConstLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfConstLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfConstLoad)
		{
			// 启动多媒体定时器失败
			m_unConstLoadState = NHC_CL_STATE_ERROR;
			m_dwLastErrorCode = NHC_CL_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCConstLoadThread::ConstLoadTestStep3(void)
{
	// 定载荷步骤-加速

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}
		return;
	}

	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// 到达停止速度

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}

		// 停止电机
		MotorOff();
		// 启动秒表1
		SetTimer1();
		// 启动恒功率控制模式
		ConstantPowerControlModeOn();

		// 进入滑行流程
		// 设置状态为：滑行，加载计时阶段
		m_unConstLoadState = NHC_CL_STATE_SLIP;

		// 重新启动定时器
		m_unTimerIdOfConstLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfConstLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfConstLoad)
		{
			// 启动多媒体定时器失败
			m_unConstLoadState = NHC_CL_STATE_ERROR;
			m_dwLastErrorCode = NHC_CL_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCConstLoadThread::ConstLoadTestStep4(void)
{
	// 定载荷步骤-滑行(加载计时)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}
		return;
	}

	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// 处于加载计时阶段
		// 实时取秒表时间
		GetTimeOfTimer1(&(m_unConstLoadTime));
	}
	else if (m_fV < 5)
	{
		// 停止定时器
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}

		// 停止秒表1
		KillTimer1();
		// 退出所有控制模式
		ControlModeOff();

		// 设置状态为：测试完成
		m_unConstLoadState = NHC_CL_STATE_FINISHED;

		// 测试完成
		// 发消息到主线程窗口通知滑行计时已经完成
		if (NULL != m_hCallBackWnd)
		{
			::PostMessage(m_hCallBackWnd, WM_DYN_CONST_LOAD_TEST_FINISHED, 0, 0);
			m_hCallBackWnd = NULL;
		}
	}
}

void CNHCConstLoadThread::StopConstLoadTest(void)
{
	// 停止定时器
	if (NULL != m_unTimerIdOfConstLoad)
	{
		timeKillEvent(m_unTimerIdOfConstLoad);
		m_unTimerIdOfConstLoad = NULL;
	}

	MotorOff();
	KillTimer1();
	ControlModeOff();

	// 设置状态：测试未开始
	m_unConstLoadState = NHC_CL_STATE_NOT_START;
}

void CNHCConstLoadThread::TimeProOfConstLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCConstLoadThread* pThread = reinterpret_cast<CNHCConstLoadThread*>(dwUser);
	pThread->ConstLoadTest();
	//TRACE("ConstLoadTest is executed\n\n");
}
