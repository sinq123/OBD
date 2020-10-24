// NHCDIWThread.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCDIWThread.h"


// CNHCDIWThread

IMPLEMENT_DYNCREATE(CNHCDIWThread, CDynThread)

CNHCDIWThread::CNHCDIWThread()
: m_unLowerSlipingTime(0)
, m_unUpperSlipingTime(0)
, m_fUpperVelocity(0.0f)
, m_fUpperVelocityWithOffset(0.0f)
, m_fLowerVelocity(0.0f)
, m_fLowerVelocityWithOffset(0.0f)
, m_nUpperVelocityOffset(8)
, m_nLowerVelocityOffset(3)
, m_fLowerForce(0.0f)
, m_fUpperForce(0.0f)
, m_unTimerRes(50)
, m_unTimerIdOfDIW(NULL)
, m_unDIWState(NHC_DIW_STATE_NOT_START)
, m_hCallBackWnd(NULL) 
, m_dwLastErrorCode(NHC_DIW_NO_ERROR)
, m_unTestInterval(200)
, m_fAveLowerForce(110.0f)
, m_fAveUpperForce(1110.0f)
, m_dTotalForce(0.0)
, m_unTotalTime(0)
{
}

CNHCDIWThread::~CNHCDIWThread()
{
}

BOOL CNHCDIWThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化

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

	return TRUE;
}

int CNHCDIWThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理

	// 关闭多媒体定时器
	if (NULL != m_unTimerIdOfDIW)
	{
		timeKillEvent(m_unTimerIdOfDIW);
		m_unTimerIdOfDIW = NULL;
	}

	// 清除多媒体计时器最低分辨率
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCDIWThread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_DIW_TEST, OnStartDIWTest)
END_MESSAGE_MAP()


// CNHCDIWThread 消息处理程序
void CNHCDIWThread::StartDIWTest_Asyn(const float fLowerForce, 
								   const float fUpperForce,
								   const float fLowerVelocity, 
								   const float fUpperVelocity, 
								   const HWND hCallBackWnd)
{
	ASSERT(fLowerForce>0.0f && fLowerForce<6000.0f);
	ASSERT(fUpperForce>0.0f && fUpperForce<6000.0f);
	ASSERT(fUpperForce > fLowerForce);
	ASSERT(fLowerVelocity>0.0f && fLowerVelocity<100.0f);
	ASSERT(fUpperVelocity>0.0f && fUpperVelocity<100.0f);
	ASSERT(fUpperVelocity > fLowerVelocity);

	m_fLowerForce = fLowerForce;
	m_fUpperForce = fUpperForce;
	m_fLowerVelocity = fLowerVelocity;
	m_fUpperVelocity = fUpperVelocity;
	m_hCallBackWnd = hCallBackWnd;

	m_fUpperVelocityWithOffset = m_fUpperVelocity + m_nUpperVelocityOffset;
	m_fLowerVelocityWithOffset = m_fLowerVelocity - m_nLowerVelocityOffset;
	ASSERT(m_fUpperVelocityWithOffset > m_fLowerVelocityWithOffset);
	if (m_fLowerVelocityWithOffset < 10e-6f)
	{
		m_fLowerVelocityWithOffset = 0.0f;
	}

	PostThreadMessage(WM_DYN_START_DIW_TEST, 0, 0);
}

void CNHCDIWThread::OnStartDIWTest(WPARAM wParam, LPARAM lParam)
{
	m_unDIWState = NHC_DIW_STATE_READY;
	DIWTest();
}

void CNHCDIWThread::DIWTest(void)
{
	switch (m_unDIWState)
	{
		case NHC_DIW_STATE_READY:
			{
				DIWTestStep1();
				break;
			}
		case NHC_DIW_STATE_LIFTBEAM_DOWN:
			{
				DIWTestStep2();
				break;
			}
		case NHC_DIW_STATE_ACCELERATION_1:
			{
				DIWTestStep3();
				break;
			}
		case NHC_DIW_STATE_SLIP_1:
			{
				DIWTestStep4();
				break;
			}
		case NHC_DIW_STATE_WAIT_FOR_STOP:
			{
				DIWTestStep5();
				break;
			}
		case NHC_DIW_STATE_ACCELERATION_2:
			{
				DIWTestStep6();
				break;
			}
		case NHC_DIW_STATE_SLIP_2:
			{
				DIWTestStep7();
				break;
			}
		default:
			break;
	}
}

void CNHCDIWThread::DIWTestStep1(void)
{
	// 步骤1:准备就绪

	// 重设测试结果数据
	m_unLowerSlipingTime = 0;
	m_unUpperSlipingTime = 0;
	m_fAveLowerForce = -1.0f;  // 初始化为负值,表示未进行过记录
	m_fAveUpperForce = -1.0f;  // 初始化为负值,表示未进行过记录
	m_dTotalForce = 0.0;
	m_unTotalTime = 0;

	// 退出所有控制模式
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// 测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;
		return;
	}
	// 初始化测功机秒表1
	KillTimer1();
	// 设置秒表1开始速度
	SetUpperVelocityOfTimer1(m_fUpperVelocity);
	// 设置秒表1停止速度
	SetLowerVelocityOfTimer1(m_fLowerVelocity);
	// 设置恒力目标值(小加载力)
	SetConstantForceTarget(m_fLowerForce);
	// 降下举升器(必须)
	LiftBeamDown();
	// 查询举升器状态
	GetStatus();
	if (m_btStatus.test(1))
	{
		// 举升器已经完全降下
		// 进入加速流程
		// 设状态为：加速
		m_unDIWState = NHC_DIW_STATE_ACCELERATION_1;
		// 启动电机，开始加速
		MotorOn();
	}
	else
	{
		// 举升器没有完全降下
		// 进入降下举升器流程
		// 设置状态为：降举升器
		m_unDIWState = NHC_DIW_STATE_LIFTBEAM_DOWN;
	}
	// 启动多媒体定时器
	if (NULL != m_unTimerIdOfDIW)
	{
		timeKillEvent(m_unTimerIdOfDIW);
		m_unTimerIdOfDIW = NULL;
	}
	m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfDIW)
	{
		// 启动多媒体定时器失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCDIWThread::DIWTestStep2(void)
{
	// 步骤2:降举升器

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// 测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if(m_btStatus.test(1))
	{
		// 举升器已经降下

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// 启动电机，开始加速
		MotorOn();

		// 进入加速流程
		// 设置状态为：加速
		m_unDIWState = NHC_DIW_STATE_ACCELERATION_1;

		// 重新启动定时器
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// 启动多媒体定时器失败
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep3(void)
{
	// 步骤3:加速

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// 到达停止速度

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// 启动秒表1
		SetTimer1();
		// 停止电机
		MotorOff();
		// 启动恒力控制模式
		ConstantForceControlModeOn();

		// 进入滑行流程
		// 设置状态为：滑行，计时阶段
		m_unDIWState = NHC_DIW_STATE_SLIP_1;

		// 重新启动定时器
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// 启动多媒体定时器失败
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep4(void)
{
	// 步骤4:滑行(小加载力)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// 处于加载计时阶段
		// 实时取秒表时间
		GetTimeOfTimer1(&(m_unLowerSlipingTime));

		// 搜集加载力(小加载力)
		if (m_fV>m_fLowerVelocity && m_fV<m_fUpperVelocity)
		{
			m_dTotalForce += m_fF;
			m_unTotalTime++;

			// 计算加载力平均值(小加载力)
			ASSERT(0 != m_unTotalTime);
			m_fAveLowerForce = float(m_dTotalForce/m_unTotalTime);
			// 保留一位小数,并且四舍五入
			m_fAveLowerForce = int(m_fAveLowerForce*10.0f + 0.5f) / 10.0f;
		}
	}
	else
	{
		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		//// 计算加载力平均值(小加载力)
		//ASSERT(0 != m_unTotalTime);
		//m_fAveLowerForce = float(m_dTotalForce/m_unTotalTime);
		//// 保留一位小数,并且四舍五入
		//m_fAveLowerForce = int(m_fAveLowerForce*10.0f + 0.5f) / 10.0f;

		// 重设总加载力和记录次数
		m_dTotalForce = 0.0;
		m_unTotalTime = 0;
 
		// 停止秒表1
		KillTimer1();
		// 退出所有控制模式
		ControlModeOff();

		// 设置状态为：等待滚筒停止
		m_unDIWState = NHC_DIW_STATE_WAIT_FOR_STOP;

		// 重新启动定时器
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// 启动多媒体定时器失败
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep5(void)
{
	// 步骤5:等待滚筒停止

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV <= 0.0f)
	{
		// 滚筒已经到达可以再次加速的安全速度(不能在滚筒高速时,开电机!!!!)
		// 进入下一加速阶段
		// 初始化下一阶段参数

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// 设置恒力目标值(大加载力)
		SetConstantForceTarget(m_fUpperForce);

		// 启动电机，开始加速
		MotorOn();

		// 设置状态为：加速
		m_unDIWState = NHC_DIW_STATE_ACCELERATION_2;

		// 重新启动定时器
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// 启动多媒体定时器失败
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep6(void)
{
	// 步骤6:加速

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// 到达停止速度

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// 启动秒表1
		SetTimer1();
		// 停止电机
		MotorOff();
		// 启动恒力控制模式
		ConstantForceControlModeOn();

		// 进入滑行流程
		// 设置状态为：滑行，计时阶段
		m_unDIWState = NHC_DIW_STATE_SLIP_2;

		// 重新启动定时器
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// 启动多媒体定时器失败
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep7(void)
{
	// 步骤7:滑行(大加载力)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// 处于加载计时阶段
		// 实时取秒表时间
		GetTimeOfTimer1(&(m_unUpperSlipingTime));

		// 搜集加载力(大加载力)
		if (m_fV>m_fLowerVelocity && m_fV<m_fUpperVelocity)
		{
			m_dTotalForce += m_fF;
			m_unTotalTime++;

			// 计算加载力平均值(小加载力)
			ASSERT(0 != m_unTotalTime);
			m_fAveUpperForce = float(m_dTotalForce/m_unTotalTime);
			// 保留一位小数,并且四舍五入
			m_fAveUpperForce = int(m_fAveUpperForce*10.0f + 0.5f) / 10.0f;
		}
	}
	else
	{
		// 停止定时器
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		//// 计算加载力平均值(小加载力)
		//ASSERT(0 != m_unTotalTime);
		//m_fAveUpperForce = float(m_dTotalForce/m_unTotalTime);
		//// 保留一位小数,并且四舍五入
		//m_fAveUpperForce = int(m_fAveUpperForce*10.0f + 0.5f) / 10.0f;

		// 重设总加载力和记录次数
		m_dTotalForce = 0.0;
		m_unTotalTime = 0;

		// 停止秒表1
		KillTimer1();
		// 退出所有控制模式
		ControlModeOff();

		// 设置状态为：测试完成
		m_unDIWState = NHC_DIW_STATE_FINISHED;
	}
}

void CNHCDIWThread::StopDIWTest(void)
{
	// 停止定时器
	if (NULL != m_unTimerIdOfDIW)
	{
		timeKillEvent(m_unTimerIdOfDIW);
		m_unTimerIdOfDIW = NULL;
	}

	MotorOff();
	KillTimer1();
	ControlModeOff();

	// 设置状态：测试未开始
	m_unDIWState = NHC_DIW_STATE_NOT_START;
	// 重设测试结果数据
	m_unLowerSlipingTime = 0;
	m_unUpperSlipingTime = 0;
	m_fAveLowerForce = 110.0f;
	m_fAveUpperForce = 1110.0f;
	m_dTotalForce = 0.0;
	m_unTotalTime = 0;
}

void CNHCDIWThread::TimeProOfDIW(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCDIWThread* pThread = (CNHCDIWThread*)(dwUser);
	pThread->DIWTest();
	//TRACE("DIWTest is executed\n\n");
}