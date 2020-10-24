// NHCPLHPThread.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCPLHP2Thread.h"


// CNHCPLHP2Thread

IMPLEMENT_DYNCREATE(CNHCPLHP2Thread, CDynThread)

CNHCPLHP2Thread::CNHCPLHP2Thread()
: m_unTimerRes(50)
, m_unTimerIdOfPLHP(NULL)
, m_unPLHPState(NHC_PLHP_STATE_NOT_START)
, m_unSlipState(0)
, m_hCallBackWnd(NULL)
, m_dwLastErrorCode(NHC_PLHP_NO_ERROR)
, m_PLHPType(PLHP_ASM)
, m_unTestInterval(200)
, m_fVelocityOfMotorOff(10)
, m_unMiddleTimeOld(0)
, m_unMiddleTimeNew(0)
{
	ZeroMemory(m_unPLHPTime, sizeof(m_unPLHPTime));
	ZeroMemory(m_dRemanenceSumByPower, sizeof(m_dRemanenceSumByPower));
	ZeroMemory(m_dRemanenceSumByForce, sizeof(m_dRemanenceSumByForce));
	ZeroMemory(m_nRemanenceTimes, sizeof(m_nRemanenceTimes));
	ZeroMemory(m_fRemanenceAveByPower, sizeof(m_fRemanenceAveByPower));
	ZeroMemory(m_fRemanenceAveByForce, sizeof(m_fRemanenceAveByForce));
}

CNHCPLHP2Thread::~CNHCPLHP2Thread()
{
}

BOOL CNHCPLHP2Thread::InitInstance()
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

int CNHCPLHP2Thread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理

	// 关闭多媒体定时器
	if (NULL != m_unTimerIdOfPLHP)
	{
		timeKillEvent(m_unTimerIdOfPLHP);
		m_unTimerIdOfPLHP = NULL;
	}

	// 清除多媒体计时器最低分辨率
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCPLHP2Thread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_PLHP_TEST, OnStartPLHPTest)
END_MESSAGE_MAP()


// CNHCPLHP2Thread 消息处理程序

void CNHCPLHP2Thread::TimeProOfPLHP(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCPLHP2Thread* pThread = (CNHCPLHP2Thread*)(dwUser);
	pThread->PLHPTest();
	//TRACE("PLHPTest is executed\n\n");
}

void CNHCPLHP2Thread::StartPLHPTest_Asyn(const enum PLHPType pt, const HWND hCallBackWnd)
{
	m_PLHPType = pt;
	if (PLHP_LUGDOWN == pt)
	{
		// LUGDOWN上限速度
		m_fVelocityOfMotorOff = 94.0f;
	}
	else
	{
		// ASM,VMAS上限速度
		m_fVelocityOfMotorOff = 56.0f;
	}
	m_hCallBackWnd = hCallBackWnd;

	PostThreadMessage(WM_DYN_START_PLHP_TEST, 0, 0);
}

void CNHCPLHP2Thread::OnStartPLHPTest(WPARAM wParam, LPARAM lParam)
{
	// 设状态为：就绪准备
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHP2Thread::StartPLHPTest(const enum PLHPType pt, const HWND hCallBackWnd)
{
	m_PLHPType = pt;
	if (PLHP_LUGDOWN == pt)
	{
		// LUGDOWN上限速度
		m_fVelocityOfMotorOff = 94.0f;
	}
	else
	{
		// ASM,VMAS上限速度
		m_fVelocityOfMotorOff = 56.0f;
	}
	m_hCallBackWnd = hCallBackWnd;
	// 设状态为：就绪准备
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHP2Thread::StopPLHPTest(void)
{
	// 停止定时器
	if (NULL != m_unTimerIdOfPLHP)
	{
		timeKillEvent(m_unTimerIdOfPLHP);
		m_unTimerIdOfPLHP = NULL;
	}

	MotorOff();
	KillTimer1();
	KillTimer2();
	ControlModeOff();

	// 设置状态：测试未开始
	m_unPLHPState = NHC_PLHP_STATE_NOT_START;
	m_unSlipState = 0;
}

void CNHCPLHP2Thread::PLHPTest(void)
{
	switch (m_unPLHPState)
	{
	case NHC_PLHP_STATE_READY:
		PLHPTestStep1();  // 就绪准备
		break;
	case NHC_PLHP_STATE_LIFTBEAM_DOWN:
		PLHPTestStep2();  // 降举升器
		break;
	case NHC_PLHP_STATE_ACCELERATION:
		PLHPTestStep3();  // 加速
		break;
	case NHC_PLHP_STATE_SLIP:
		PLHPTestStep4();  // 滑行(计时)
		break;
	default:
		break;
	}
}

void CNHCPLHP2Thread::PLHPTestStep1(void)
{
	// 寄生功率步骤-准备就绪

	// 下面初始化寄生功率测试参数

	// 重设寄生功率滑行时间
	ZeroMemory(m_unPLHPTime, sizeof(m_unPLHPTime));
	// 重设剩磁记录数值
	ZeroMemory(m_dRemanenceSumByPower, sizeof(m_dRemanenceSumByPower));
	ZeroMemory(m_dRemanenceSumByForce, sizeof(m_dRemanenceSumByForce));
	ZeroMemory(m_nRemanenceTimes, sizeof(m_nRemanenceTimes));
	ZeroMemory(m_fRemanenceAveByPower, sizeof(m_fRemanenceAveByPower));
	ZeroMemory(m_fRemanenceAveByForce, sizeof(m_fRemanenceAveByForce));

	// 退出所有控制模式
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// 测功机操作失败
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;
		return;
	}
	// 初始化测功机秒表1
	KillTimer1();
	// 初始化测功机秒表2
	KillTimer2();
	// 降下举升器(必须)
	LiftBeamDown();
	// 查询举升器状态
	GetStatus();
	if (m_btStatus.test(1))
	{
		// 举升器已经完全降下
		// 进入加速了流程
		// 设状态为：加速
		m_unPLHPState = NHC_PLHP_STATE_ACCELERATION;
		// 启动电机，开始加速
		MotorOn();
	}
	else
	{
		// 举升器没有完全降下
		// 进入降下举升器流程
		// 设置状态为：降举升器
		m_unPLHPState = NHC_PLHP_STATE_LIFTBEAM_DOWN;
	}
	// 启动多媒体定时器
	if (NULL != m_unTimerIdOfPLHP)
	{
		timeKillEvent(m_unTimerIdOfPLHP);
		m_unTimerIdOfPLHP = NULL;
	}
	m_unTimerIdOfPLHP = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfPLHP, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfPLHP)
	{
		// 启动多媒体定时器失败
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCPLHP2Thread::PLHPTestStep2(void)
{
	// 寄生功率步骤-降举升器流程

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// 测功机操作失败
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}
		return;
	}
	if(m_btStatus.test(1))
	{
		// 举升器已经降下

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		// 并降间隔改为200ms
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}

		// 启动电机，开始加速
		MotorOn();

		// 进入加速流程
		// 设置状态为：加速
		m_unPLHPState = NHC_PLHP_STATE_ACCELERATION;

		// 重新启动定时器
		m_unTimerIdOfPLHP = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfPLHP, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfPLHP)
		{
			// 启动多媒体定时器失败
			m_unPLHPState = NHC_PLHP_STATE_ERROR;
			m_unSlipState = 0;
			m_dwLastErrorCode = NHC_PLHP_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCPLHP2Thread::PLHPTestStep3(void)
{
	// 寄生功率步骤-加速

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败

		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}
		return;
	}

	if (m_fV > m_fVelocityOfMotorOff)
	{
		// 到达停止速度

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}

		// 以下功能被屏蔽，改为由下一状态关电机
		//// 停止电机
		//MotorOff();

		// 进入滑行流程
		// 设置状态为：滑行，计时
		m_unPLHPState = NHC_PLHP_STATE_SLIP;
		
		// 设置滑行状态：未开始
		// 必须设置
		m_unSlipState = 0;
		// 预执行下一测试状态的第一个滑行步骤操作(m_unSlipState=0)
		// 因为m_unSlipState=0时,ASM和VMAS共有9个测功机操作,LugDown有6个,放在定时器没有运作起来的区域进行操作比较合理
		PLHPTestStep4();

		// 重新启动定时器
		m_unTimerIdOfPLHP = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfPLHP, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfPLHP)
		{
			// 启动多媒体定时器失败
			m_unPLHPState = NHC_PLHP_STATE_ERROR;
			m_unSlipState = 0;
			m_dwLastErrorCode = NHC_PLHP_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCPLHP2Thread::PLHPTestStep4(void)
{
	//寄生功率步骤-滑行(计时)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}
		return;
	}

	if (PLHP_ASM == m_PLHPType)
	{
		// 记录剩磁
		RemanenceRecord_ASM();
		// ASM寄生功率
		PLHPTestStep4_ASM();

	}
	else if (PLHP_VMAS == m_PLHPType)
	{
		// 记录剩磁
		RemanenceRecord_VMAS();
		// VMAS寄生功率
		PLHPTestStep4_VMAS();
	}
	else
	{
		// 记录剩磁
		RemanenceRecord_LUGDOWN();
		// LUGDOWN寄生功率
		PLHPTestStep4_LUGDOWN();
	}
}

//void CNHCPLHP2Thread::PLHPTestStep5(void)
//{
//	// 寄生功率步骤-滑行(等待滚筒停下来)
//
//	if (DYN_WR_SUCCEED != GetRealTimeData())
//	{
//		// 执行测功机操作失败
//
//		m_unPLHPState = 7;
//		m_unSlipState = 0;
//		m_dwLastErrorCode = 0x01;
//
//		// 停止定时器
//		if (NULL != m_unTimerIdOfPLHP)
//		{
//			timeKillEvent(m_unTimerIdOfPLHP);
//			m_unTimerIdOfPLHP = NULL;
//		}
//		return;
//	}
//
//	if (m_fV < 0.05f)
//	{
//		// 滚筒已经停下来
//		// 结束测试
//
//		// 停止定时器
//		if (NULL != m_unTimerIdOfPLHP)
//		{
//			timeKillEvent(m_unTimerIdOfPLHP);
//			m_unTimerIdOfPLHP = NULL;
//		}
//
//		// 设置状态为：测试全部结束
//		m_unPLHPState = 6;
//	}
//}

void CNHCPLHP2Thread::PLHPTestStep4_ASM(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 准备滑行

			// 启动秒表2(54~46)
			SetUpperVelocityOfTimer2(54);
			SetLowerVelocityOfTimer2(46);
			SetTimer2();
			// 启动秒表1(48~33~17)
			SetUpperVelocityOfTimer1(48);
			SetMiddleVelocityOfTimer1(33);
			SetLowerVelocityOfTimer1(17);
			SetTimer1();
			// 关电机
			MotorOff();

			// 进入下一滑行状态(51~48)
			m_unSlipState = 1;

			break;
		}
	case 1:
		{
			// 51~48

			// 获取第一段滑行时间(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);

			if (m_fV < 48.0f)
			{
				// 进入下一滑行状态(48~45)
				m_unSlipState = 2;
			}
			break;
		}
	case 2:
		{
			// 48~45

			// 获取第一段滑行时间(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);
			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 46.0f)
			{
				// 第1段标记
				const UINT unSection(0);
				// 计算第1段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第一段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, WPARAM(unSection), 0);
				}
				// 重新设置秒表2时间段(40~24),并重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(40);
				SetLowerVelocityOfTimer2(24);
				SetTimer2();

				// 进入下一滑行状态(45~40)
				m_unSlipState = 3;
			}
			break;
		}
	case 3:
		{
			// 45~40

			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 40.0f)
			{
				// 进入下一滑行状态(40~33)
				m_unSlipState = 4;
			}
			break;
		}
	case 4:
		{
			// 40~33

			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);
			// 获取第三段滑行时间(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);

			if (m_fV < 33.0f)
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 暂存48~33滑行时间,为了算出第四段滑行时间(33~17)
				GetMiddleTimeOfTimer1(&m_unMiddleTimeOld);
				// 重新设置Timer1中间速度为32
				SetMiddleVelocityOfTimer1(32);

				// 进入下一滑行状态(33~32)
				m_unSlipState = 5;
			}
			break;
		}
	case 5:
		{
			// 33~32

			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);
			// 获取第三段滑行时间(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);
			// 获取第四段滑行时间(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;

			if (m_fV < 32.0f)
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);

				// 第2段标记
				const UINT unSection(1);
				// 计算第2段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第二段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 进入下一滑行状态(32~24)
				m_unSlipState = 6;
			}
			break;
		}
	case 6:
		{
			// 32~24

			// 获取第三段滑行时间(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);
			// 获取第四段滑行时间(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;

			if (m_fV < 24.0f)
			{
				// 第3段标记
				const UINT unSection(2);
				// 计算第3段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;

				// 发消息到主线程窗口通知滑行计时第三段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				// 重新设置秒表2时间段(19~13),并重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(19);
				SetLowerVelocityOfTimer2(13);
				SetTimer2();

				// 进入下一滑行状态(24~19)
				m_unSlipState = 7;
			}
			break;
		}
	case 7:
		{
			// 24~19

			// 获取第四段滑行时间(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;

			if (m_fV < 19.0f)
			{
				// 进入下一滑行状态(19~17)
				m_unSlipState = 8;
			}
			break;
		}
	case 8:
		{
			// 19~17

			// 获取第四段滑行时间(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;
			// 获取第五段滑行时间(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 17.0f)
			{
				// 第4段标记
				const UINT unSection(3);
				// 计算第4段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第四段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 秒表1使用完毕，停止
				KillTimer1();

				// 进入下一滑行状态(17~13)
				m_unSlipState = 9;
			}
			break;
		}
	case 9:
		{
			// 17~13

			// 获取第五段滑行时间(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 13.0f)
			{
				// 第5段标记
				const UINT unSection(4);
				// 计算第5段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第五段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 秒表2使用完毕，停止
				KillTimer2();

				// 停止定时器
				if (NULL != m_unTimerIdOfPLHP)
				{
					timeKillEvent(m_unTimerIdOfPLHP);
					m_unTimerIdOfPLHP = NULL;
				}
				
				// 滑行状态完成
				// 设置状态为：测试完成
				m_unPLHPState = NHC_PLHP_STATE_FINISHED;

				// 测试完成
				// 发消息到主线程窗口通知滑行计时已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_TEST_FINISHED, 0, 0);
					m_hCallBackWnd = NULL;
				}
			}
			break;
		}
	default:
		break;
	}
}

void CNHCPLHP2Thread::PLHPTestStep4_VMAS(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 准备滑行

			// 启动秒表2(51~45)
			SetUpperVelocityOfTimer2(54);
			SetLowerVelocityOfTimer2(46);
			SetTimer2();
			// 启动秒表1(48~33~16)
			SetUpperVelocityOfTimer1(48);
			SetMiddleVelocityOfTimer1(32);
			SetLowerVelocityOfTimer1(16);
			SetTimer1();
			// 关电机
			MotorOff();
			
			// 进入下一滑行状态(51~48)
			m_unSlipState = 1;

			break;
		}
	case 1:
		{
			// 51~48

			//获取第一段滑行时间(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);

			if (m_fV < 48.0f)
			{
				// 进入下一滑行状态(48~45)
				m_unSlipState = 2;
			}
			break;
		}
	case 2:
		{
			// 48~45

			//获取第一段滑行时间(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);
			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 46.0f)
			{
				// 第1段标记
				const UINT unSection(0);
				// 计算第1段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第一段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 重新设置秒表2时间段(40~24),并重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(40);
				SetLowerVelocityOfTimer2(24);
				SetTimer2();

				// 进入下一滑行状态(45~40)
				m_unSlipState = 3;
			}
			break;
		}
	case 3:
		{
			// 45~40

			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 40.0f)
			{
				// 进入下一滑行状态(40~32)
				m_unSlipState = 4;
			}
			break;
		}
	case 4:
		{
			// 40~32

			// 获取第二段滑行时间(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);
			// 获取第三段滑行时间(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);

			if (m_fV < 32.0f)
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第二段滑行时间(48~32)
				GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);

				// 第2段标记
				const UINT unSection(1);
				// 计算第2段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第二段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 进入下一滑行状态(32~24)
				m_unSlipState = 5;
			}
			break;
		}
	case 5:
		{
			// 32~24

			// 获取第三段滑行时间(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);
			// 获取四段滑行时间(32~16)
			GetTimeOfTimer1(&m_unMiddleTimeOld);
			m_unPLHPTime[3] = m_unMiddleTimeOld - m_unPLHPTime[1];

			if (m_fV < 24.0f)
			{
				// 第3段标记
				const UINT unSection(2);
				// 计算第3段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第三段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 重新设置秒表2时间段(19~13),并重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(19);
				SetLowerVelocityOfTimer2(13);
				SetTimer2();

				// 进入下一滑行状态(24~19)
				m_unSlipState = 6;
			}
			break;
		}
	case 6:
		{
			// 24~19

			// 获取四段滑行时间(32~16)
			GetTimeOfTimer1(&m_unMiddleTimeOld);
			m_unPLHPTime[3] = m_unMiddleTimeOld - m_unPLHPTime[1];

			if (m_fV < 19.0f)
			{
				// 进入下一滑行状态(19~16)
				m_unSlipState = 7;
			}
			break;
		}
	case 7:
		{
			//  19~16

			// 获取四段滑行时间(32~16)
			GetTimeOfTimer1(&m_unMiddleTimeOld);
			m_unPLHPTime[3] = m_unMiddleTimeOld - m_unPLHPTime[1];
			// 获取第五段滑行时间(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 16.0f)
			{
				// 第4段标记
				const UINT unSection(3);
				// 计算第4段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第四段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 秒表1使用完毕，停止
				KillTimer1();

				// 进入下一滑行状态(16~13)
				m_unSlipState = 8;
			}
			break;
		}
	case 8:
		{
			// 16~13

			// 获取第五段滑行时间(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 13.0f)
			{
				// 第5段标记
				const UINT unSection(4);
				// 计算第5段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第五段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 秒表2使用完毕，停止
				KillTimer2();

				// 停止定时器
				if (NULL != m_unTimerIdOfPLHP)
				{
					timeKillEvent(m_unTimerIdOfPLHP);
					m_unTimerIdOfPLHP = NULL;
				}

				// 滑行状态完成
				// 设置状态为：测试完成
				m_unPLHPState = NHC_PLHP_STATE_FINISHED;

				// 测试完成
				// 发消息到主线程窗口通知滑行计时已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_TEST_FINISHED, 0, 0);
					m_hCallBackWnd = NULL;
				}
			}
			break;
		}
	default:
		break;
	}
}

void CNHCPLHP2Thread::PLHPTestStep4_LUGDOWN(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 启动秒表1(92~84)
			SetUpperVelocityOfTimer1(92);
			//SetMiddleVelocityOfTimer1(84);
			SetLowerVelocityOfTimer1(84);
			SetTimer1();

			// 启动秒表2(84~76)
			SetUpperVelocityOfTimer2(84);
			SetLowerVelocityOfTimer2(76);
			SetTimer2();

			// 关电机
			MotorOff();
			// 进入下一滑行状态
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 92~84

			if (m_fV >= 84.0f)
			{
				// 获取第一段滑行时间(92~84)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第一段滑行时间(92~84)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 暂存滑行时间(92~84)
				m_unMiddleTimeOld = m_unPLHPTime[0];

				// 第1段标记
				const UINT unSection(0);
				// 计算第1段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第一段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}


				//设置第三阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(76);
				SetLowerVelocityOfTimer1(68);
				SetTimer1();

				// 进入下一滑行状态(84~76)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 84~76

			if (m_fV >= 76.0f)
			{
				// 获取第二段滑行时间
				//GetTimeOfTimer1(&m_unMiddleTimeNew);
				//m_unPLHPTime[1] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				GetTimeOfTimer2(&m_unPLHPTime[1]);
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第二段滑行时间(84~76)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				//m_unPLHPTime[1] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~76)
				m_unMiddleTimeOld += m_unPLHPTime[1];

				// 第2段标记
				const UINT unSection(1);
				// 计算第2段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第二段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 设置下一段秒表1中间速度(68)
				//SetMiddleVelocityOfTimer1(68);

				//设置第四阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(68);
				SetLowerVelocityOfTimer2(60);
				SetTimer2();

				// 进入下一滑行状态(76~68)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 76~68

			if (m_fV >= 68.0f)
			{
				// 获取第三段滑行时间(76~68)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				//m_unPLHPTime[2] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第三段滑行时间(76~68)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				//m_unPLHPTime[2] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~68)
				m_unMiddleTimeOld += m_unPLHPTime[2];

				// 第3段标记
				const UINT unSection(2);
				// 计算第3段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第三段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 设置下一段秒表1中间速度(60)
				//SetMiddleVelocityOfTimer1(60);

				//设置第五阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(60);
				SetLowerVelocityOfTimer1(52);
				SetTimer1();

				// 进入下一滑行状态(68~60)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 68~60

			if (m_fV >= 60.0f)
			{
				// 获取四段滑行时间(68~60)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
				//m_unPLHPTime[3] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第四段滑行时间(68~60)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
				//m_unPLHPTime[3] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~60)
				m_unMiddleTimeOld += m_unPLHPTime[3];

				// 第4段标记
				const UINT unSection(3);
				// 计算第4段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第四段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 设置下一段秒表1中间速度(52)
				//SetMiddleVelocityOfTimer1(52);

				//设置第六阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(52);
				SetLowerVelocityOfTimer2(44);
				SetTimer2();

				// 进入下一滑行状态(60~52)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 60~52

			if (m_fV >= 52.0f)
			{
				// 获取第五段滑行时间(60~52)
				GetTimeOfTimer1(&m_unPLHPTime[4]);
				//m_unPLHPTime[4] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第五段滑行时间(60~52)
				GetTimeOfTimer1(&m_unPLHPTime[4]);
				//m_unPLHPTime[4] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~52)
				m_unMiddleTimeOld += m_unPLHPTime[4];

				// 第5段标记
				const UINT unSection(4);
				// 计算第5段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第五段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 设置下一段秒表1中间速度(44)
				//SetMiddleVelocityOfTimer1(44);

				//设置第七阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(44);
				SetLowerVelocityOfTimer1(36);
				SetTimer1();


				// 进入下一滑行状态(52~44)
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 52~44

			if (m_fV >= 44.0f)
			{
				// 获取第六段滑行时间(52~44)
				GetTimeOfTimer2(&m_unPLHPTime[5]);
				//m_unPLHPTime[5] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第六段滑行时间(52~44)
				GetTimeOfTimer2(&m_unPLHPTime[5]);
				//m_unPLHPTime[5] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~44)
				m_unMiddleTimeOld += m_unPLHPTime[5];

				// 第6段标记
				const UINT unSection(5);
				// 计算第6段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第六段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 设置下一段秒表1中间速度(36)
				//SetMiddleVelocityOfTimer1(36);

				//设置第八阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(36);
				SetLowerVelocityOfTimer2(28);
				SetTimer2();

				// 进入下一滑行状态(44~36)
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 44~36

			if (m_fV >= 36.0f)
			{
				// 获取第七段滑行时间(44~36)
				GetTimeOfTimer1(&m_unPLHPTime[6]);
				//m_unPLHPTime[6] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第七段滑行时间(44~36)
				GetTimeOfTimer1(&m_unPLHPTime[6]);
				//m_unPLHPTime[6] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~36)
				m_unMiddleTimeOld += m_unPLHPTime[6];

				// 第7段标记
				const UINT unSection(6);
				// 计算第7段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第七段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 设置下一段秒表1中间速度(28)
				//SetMiddleVelocityOfTimer1(28);

				//设置第九阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(28);
				SetLowerVelocityOfTimer1(20);
				SetTimer1();

				// 进入下一滑行状态(36~28)
				m_unSlipState = 8;
			}
		}
		break;
	case 8:
		{
			// 36~28

			if (m_fV >= 28.0f)
			{
				// 获取第八段滑行时间(36~28)
				GetTimeOfTimer2(&m_unPLHPTime[7]);
				//m_unPLHPTime[7] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// 获取最终的中间时间,因为中间时间不是实时更新
				// 获取第八段滑行时间(36~28)
				GetTimeOfTimer2(&m_unPLHPTime[7]);
				//m_unPLHPTime[7] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// 暂存滑行时间(92~28)
				m_unMiddleTimeOld += m_unPLHPTime[7];

				// 第8段标记
				const UINT unSection(7);
				// 计算第8段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第八段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// 秒表2使用完成，关闭
				KillTimer2();

				// 进入下一滑行状态(28~20)
				m_unSlipState = 9;
			}
		}
		break;
	case 9:
		{
			// 28~20

			// 获取第九段滑行时间(28~20)
			// 最后一段
			GetTimeOfTimer1(&m_unPLHPTime[8]);
			//m_unPLHPTime[8] = m_unMiddleTimeNew - m_unMiddleTimeOld;

			if (m_fV < 20.0f)
			{
				// 第9段标记
				const UINT unSection(8);
				// 计算第9段剩磁平均值,格式化为2位小数，并四舍五入
				// 功率模式
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// 力模式
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// 发消息到主线程窗口通知滑行计时第九段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				
				// 停止定时器
				if (NULL != m_unTimerIdOfPLHP)
				{
					timeKillEvent(m_unTimerIdOfPLHP);
					m_unTimerIdOfPLHP = NULL;
				}

				// 置零
				m_unMiddleTimeOld = m_unMiddleTimeNew = 0;
				// 秒表1使用完毕，停止
				KillTimer1();

				// 滑行状态完成
				// 设置状态为：测试完成
				m_unPLHPState = NHC_PLHP_STATE_FINISHED;

				// 测试完成
				// 发消息到主线程窗口通知滑行计时已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_TEST_FINISHED, 0, 0);
					m_hCallBackWnd = NULL;
				}
			}
		}
		break;
	default:
		break;
	}
}

void CNHCPLHP2Thread::RemanenceRecord_ASM(void)
{
	// ASM剩磁记录

	//if (m_fV<=51.0f && m_fV>=48.0f)
	if(m_fV > 54.0f)
	{
		// 未进入有效区间范围，不记录剩磁
	}
	else if (m_fV>=48.0f)
	{
		// 第1段(54~46)
		RemanenceRecordOfSection1();
	}
	else if (m_fV>=46.0f)
	{
		// 第1段(54~46)
		RemanenceRecordOfSection1();
		// 第2段(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=40.0f)
	{
		// 第2段(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=33.0f)
	{
		// 第2段(48~32)
		RemanenceRecordOfSection2();
		// 第3段(40~24)
		RemanenceRecordOfSection3();
	}
	else if (m_fV>=32.0f)
	{
		// 第2段(48~32)
		RemanenceRecordOfSection2();
		// 第3段(40~24)
		RemanenceRecordOfSection3();
		// 第4段(33~17)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=24.0f)
	{
		// 第3段(40~24)
		RemanenceRecordOfSection3();
		// 第4段(33~17)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=19.0f)
	{
		// 第4段(33~17)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=17.0f)
	{
		// 第4段(33~17)
		RemanenceRecordOfSection4();
		// 第5段(19~13)
		RemanenceRecordOfSection5();
	}
	else if (m_fV>= 13.0f)
	{
		// 第5段(19~13)
		RemanenceRecordOfSection5();
	}
}

void CNHCPLHP2Thread::RemanenceRecord_VMAS(void)
{
	// VMAS剩磁记录

	//if (m_fV<=51.0f && m_fV>=48.0f)
	if (m_fV > 54.0f)
	{
		// 未进入有效区间范围，不记录剩磁
	}
	else if (m_fV>=48.0f)
	{
		// 第1段(51~45)
		RemanenceRecordOfSection1();
	}
	else if (m_fV>=46.0f)
	{
		// 第1段(51~45)
		RemanenceRecordOfSection1();
		// 第2段(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=40.0f)
	{
		// 第2段(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=32.0f)
	{
		// 第2段(48~32)
		RemanenceRecordOfSection2();
		// 第3段(40~24)
		RemanenceRecordOfSection3();
	}
	else if (m_fV>=24.0f)
	{
		// 第3段(40~24)
		RemanenceRecordOfSection3();
		// 第4段(32~16)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=19.0f)
	{
		// 第4段(32~16)
		RemanenceRecordOfSection4();
	}
	else if(m_fV>=16.0f)
	{
		// 第4段(32~16)
		RemanenceRecordOfSection4();
		// 第5段(19~13)
		RemanenceRecordOfSection5();
	}
	else if (m_fV>=13.0f)
	{
		// 第5段(19~13)
		RemanenceRecordOfSection5();
	}
}

void CNHCPLHP2Thread::RemanenceRecord_LUGDOWN(void)
{
	// LUGDOWN剩磁记录

	//if (m_fV<=92.0f && m_fV>=84.0f)
	if (m_fV > 92.0f)
	{
		// 未进入有效区间范围，不记录剩磁
	}
	else if (m_fV>=84.0f)
	{
		// 第1段(92~84)
		RemanenceRecordOfSection1();
	}
	else if (m_fV>=76.0f)
	{
		// 第2段(84~76)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=68.0f)
	{
		// 第3段(76~68)
		RemanenceRecordOfSection3();
	}
	else if (m_fV>=60.0f)
	{
		// 第4段(68~60)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=52.0f)
	{
		// 第5段(60~52)
		RemanenceRecordOfSection5();
	}
	else if (m_fV>=44.0f)
	{
		// 第6段(52~44)
		RemanenceRecordOfSection6();
	}
	else if (m_fV>=36.0f)
	{
		// 第7段(44~36)
		RemanenceRecordOfSection7();
	}
	else if (m_fV>=28.0f)
	{
		// 第8段(36~28)
		RemanenceRecordOfSection8();
	}
	else if (m_fV>=20.0f)
	{
		// 第9段(28~20)
		RemanenceRecordOfSection9();
	}
}

void CNHCPLHP2Thread::RemanenceRecordOfSection1(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[0] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[0] += m_fF;
	m_nRemanenceTimes[0]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection2(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[1] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[1] += m_fF;
	m_nRemanenceTimes[1]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection3(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[2] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[2] += m_fF;
	m_nRemanenceTimes[2]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection4(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[3] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[3] += m_fF;
	m_nRemanenceTimes[3]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection5(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[4] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[4] += m_fF;
	m_nRemanenceTimes[4]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection6(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[5] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[5] += m_fF;
	m_nRemanenceTimes[5]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection7(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[6] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[6] += m_fF;
	m_nRemanenceTimes[6]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection8(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[7] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[7] += m_fF;
	m_nRemanenceTimes[7]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection9(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[8] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[8] += m_fF;
	m_nRemanenceTimes[8]++;
}
