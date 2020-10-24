// NHCPLHPThread.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCPLHPGB2018Thread.h"


// CNHCPLHPGB2018Thread

IMPLEMENT_DYNCREATE(CNHCPLHPGB2018Thread, CDynThread)

CNHCPLHPGB2018Thread::CNHCPLHPGB2018Thread()
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
	ZeroMemory(m_unPLHPTime10, sizeof(m_unPLHPTime10));
	ZeroMemory(m_dRemanenceSumByPower, sizeof(m_dRemanenceSumByPower));
	ZeroMemory(m_dRemanenceSumByForce, sizeof(m_dRemanenceSumByForce));
	ZeroMemory(m_nRemanenceTimes, sizeof(m_nRemanenceTimes));
	ZeroMemory(m_fRemanenceAveByPower, sizeof(m_fRemanenceAveByPower));
	ZeroMemory(m_fRemanenceAveByForce, sizeof(m_fRemanenceAveByForce));
}

CNHCPLHPGB2018Thread::~CNHCPLHPGB2018Thread()
{
}

BOOL CNHCPLHPGB2018Thread::InitInstance()
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

int CNHCPLHPGB2018Thread::ExitInstance()
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

BEGIN_MESSAGE_MAP(CNHCPLHPGB2018Thread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_PLHP_TEST, OnStartPLHPTest)
END_MESSAGE_MAP()


// CNHCPLHPGB2018Thread 消息处理程序

void CNHCPLHPGB2018Thread::TimeProOfPLHP(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCPLHPGB2018Thread* pThread = (CNHCPLHPGB2018Thread*)(dwUser);
	pThread->PLHPTest();
	//TRACE("PLHPTest is executed\n\n");
}

void CNHCPLHPGB2018Thread::StartPLHPTest_Asyn(const enum PLHPType pt, const HWND hCallBackWnd)
{
	m_PLHPType = pt;
	if (PLHP_LUGDOWN == pt)
	{
		// LUGDOWN上限速度
		m_fVelocityOfMotorOff = 82.0f;
	}
	else if(PLHP_LUGDOWN_100 == pt)
	{
		// LUGDOWN上限速度
		m_fVelocityOfMotorOff = 102.0f;
	
	}
	else
	{
		// ASM,VMAS上限速度
		m_fVelocityOfMotorOff = 62.0f;
	}
	m_hCallBackWnd = hCallBackWnd;

	PostThreadMessage(WM_DYN_START_PLHP_TEST, 0, 0);
}

void CNHCPLHPGB2018Thread::OnStartPLHPTest(WPARAM wParam, LPARAM lParam)
{
	// 设状态为：就绪准备
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHPGB2018Thread::StartPLHPTest(const enum PLHPType pt, const HWND hCallBackWnd)
{
	m_PLHPType = pt;
	if (PLHP_LUGDOWN == pt)
	{
		// LUGDOWN上限速度
		m_fVelocityOfMotorOff = 82.0f;
	}
	else if(PLHP_LUGDOWN_100 == pt)
	{
		// LUGDOWN上限速度
		m_fVelocityOfMotorOff = 102.0f;	
	}
	else
	{
		// ASM,VMAS上限速度
		m_fVelocityOfMotorOff = 62.0f;
	}
	m_hCallBackWnd = hCallBackWnd;
	// 设状态为：就绪准备
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHPGB2018Thread::StopPLHPTest(void)
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

void CNHCPLHPGB2018Thread::PLHPTest(void)
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

void CNHCPLHPGB2018Thread::PLHPTestStep1(void)
{
	// 寄生功率步骤-准备就绪

	// 下面初始化寄生功率测试参数

	// 重设寄生功率滑行时间
	ZeroMemory(m_unPLHPTime, sizeof(m_unPLHPTime));
	ZeroMemory(m_unPLHPTime10, sizeof(m_unPLHPTime10));
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

void CNHCPLHPGB2018Thread::PLHPTestStep2(void)
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

void CNHCPLHPGB2018Thread::PLHPTestStep3(void)
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

void CNHCPLHPGB2018Thread::PLHPTestStep4(void)
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
	else if (PLHP_LUGDOWN == m_PLHPType)
	{
		// 记录剩磁
		RemanenceRecord_LUGDOWN();
		// LUGDOWN寄生功率
		PLHPTestStep4_LUGDOWN();
	}
	else 
	{
		// 记录剩磁
		RemanenceRecord_LUGDOWN_100();
		// LUGDOWN寄生功率
		PLHPTestStep4_LUGDOWN_100();
	}
}

void CNHCPLHPGB2018Thread::PLHPTestStep4_ASM(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 启动秒表1(60~40)
			SetUpperVelocityOfTimer1(60);
			SetLowerVelocityOfTimer1(40);
			SetTimer1();

			// 启动秒表2(50~30)
			SetUpperVelocityOfTimer2(50);
			SetLowerVelocityOfTimer2(30);
			SetTimer2();

			// 关电机
			MotorOff();
			// 进入下一滑行状态
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 60~40
			if (m_fV >= 50.0f)
			{
				// 获取第一段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
			}
			else
			{
				// 获取第1段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// 进入下一滑行状态(50~30)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 50~30
			if (m_fV >= 40.0f)
			{
				// 获取第1段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
			}
			else
			{
				// 获取第1段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// 第1段标记
				const UINT unSection(0);
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

				//设置第3阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(35);
				SetLowerVelocityOfTimer1(15);
				SetTimer1();

				// 进入下一滑行状态(50~30)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 50~30
			if (m_fV >= 35.0f)
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);


				// 进入下一滑行状态(35~15)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 35~15
			if (m_fV >= 30.0f)
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);

				// 第2段标记
				const UINT unSection(1);
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

				//设置第4阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(22);
				SetLowerVelocityOfTimer2(8);
				SetTimer2();

				// 进入下一滑行状态(35~15)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 22~8
			if (m_fV >= 22.0f)
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// 进入下一滑行状态
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 35~15
			if (m_fV >= 15.0f)
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// 第3段标记
				const UINT unSection(2);
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
				
				//秒表1使用完毕，停止
				KillTimer1();

				// 进入下一滑行状态
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 22~8
			if (m_fV >= 8.0f)
			{
				// 获取第4段滑行时间(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// 获取第4段滑行时间(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// 第4段标记
				const UINT unSection(3);
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

				//秒表2使用完毕，停止
				KillTimer2();

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

void CNHCPLHPGB2018Thread::PLHPTestStep4_VMAS(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 启动秒表1(60~40)
			SetUpperVelocityOfTimer1(60);
			SetLowerVelocityOfTimer1(40);
			SetTimer1();

			// 启动秒表2(50~30)
			SetUpperVelocityOfTimer2(50);
			SetLowerVelocityOfTimer2(30);
			SetTimer2();

			// 关电机
			MotorOff();
			// 进入下一滑行状态
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 60~40
			if (m_fV >= 50.0f)
			{
				// 获取第一段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
			}
			else
			{
				// 获取第1段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// 进入下一滑行状态(50~30)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 50~30
			if (m_fV >= 40.0f)
			{
				// 获取第1段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
			}
			else
			{
				// 获取第1段滑行时间(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// 第1段标记
				const UINT unSection(0);
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

				//设置第3阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(35);
				SetLowerVelocityOfTimer1(15);
				SetTimer1();

				// 进入下一滑行状态(50~30)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 50~30
			if (m_fV >= 35.0f)
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);


				// 进入下一滑行状态(35~15)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 35~15
			if (m_fV >= 30.0f)
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// 获取第2段滑行时间(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);

				// 第2段标记
				const UINT unSection(1);
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
				

				//设置第4阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(20);
				SetLowerVelocityOfTimer2(10);
				SetTimer2();

				// 进入下一滑行状态(35~15)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 20~8
			if (m_fV >= 20.0f)
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// 进入下一滑行状态
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 35~15
			if (m_fV >= 15.0f)
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// 获取第3段滑行时间(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// 获取第4段滑行时间(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// 第3段标记
				const UINT unSection(2);
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
				
				//秒表1使用完毕，停止
				KillTimer1();

				// 进入下一滑行状态
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 20~10
			if (m_fV >= 10.0f)
			{
				// 获取第4段滑行时间(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// 获取第4段滑行时间(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// 第4段标记
				const UINT unSection(3);
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

				//秒表2使用完毕，停止
				KillTimer2();

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

void CNHCPLHPGB2018Thread::PLHPTestStep4_LUGDOWN(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 启动秒表1(80~70)
			SetUpperVelocityOfTimer1(80);
			SetLowerVelocityOfTimer1(70);
			SetTimer1();

			// 启动秒表2(70~60)
			SetUpperVelocityOfTimer2(70);
			SetLowerVelocityOfTimer2(60);
			SetTimer2();

			// 关电机
			MotorOff();
			// 进入下一滑行状态
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 100~90
			if (m_fV >= 70.0f)
			{
				// 获取第1段滑行时间(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
			}
			else
			{
				// 获取第1段滑行时间(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				// 获取第1段滑行时间(70~60)
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				
				//设置第2阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(60);
				SetLowerVelocityOfTimer1(50);
				SetTimer1();

				// 进入下一滑行状态(70~60)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 70~60
			if (m_fV >= 60.0f)
			{
				// 获取第1段滑行时间
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
			}
			else
			{
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80

				// 第1段标记
				const UINT unSection(0);
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

				//设置第2阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(50);
				SetLowerVelocityOfTimer2(40);
				SetTimer2();

				// 进入下一滑行状态(60~50)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 70~50
			if (m_fV >= 50.0f)
			{
				// 获取第2段滑行时间(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
			}
			else
			{
				// 获取第2段滑行时间(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70

				// 第2段标记
				const UINT unSection(1);
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

				//设置第3阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(40);
				SetLowerVelocityOfTimer1(30);
				SetTimer1();

				// 进入下一滑行状态(50~40)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 50~40
			if (m_fV >= 40.0f)
			{
				// 获取滑行时间(50~40)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
			}
			else
			{				
				// 获取滑行时间(50~40)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60

				// 第3段标记
				const UINT unSection(2);
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

				//设置第4阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(30);
				SetLowerVelocityOfTimer2(20);
				SetTimer2();

				// 进入下一滑行状态(40~30)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 40~30
			if (m_fV >= 30.0f)
			{
				// 获取滑行时间(40~30)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
			}
			else
			{				
				// 获取滑行时间(40~30)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50

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
				// 发消息到主线程窗口通知滑行计时第五段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				//设置第5阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(20);
				SetLowerVelocityOfTimer1(10);
				SetTimer1();


				// 进入下一滑行状态(30~20)
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 30~20
			if (m_fV >= 20.0f)
			{
				// 获取滑行时间(30~20)
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
				m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT40
			}
			else
			{
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				GetTimeOfTimer1(&m_unPLHPTime10[6]);
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
				m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT40
				
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
				// 发消息到主线程窗口通知滑行计时第六段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				
				//秒表2使用完成，关闭
				KillTimer2();

				// 进入下一滑行状态(20~10)
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 20~10
			// 获取滑行时间(20~10)
			// 最后一段
			GetTimeOfTimer1(&m_unPLHPTime10[6]);
			m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT20

			if (m_fV < 10.0f)
			{
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
				// 发消息到主线程窗口通知滑行计时第七段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				//秒表2使用完毕，停止
				KillTimer2();
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

void CNHCPLHPGB2018Thread::PLHPTestStep4_LUGDOWN_100(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// 启动秒表1(100~90)
			SetUpperVelocityOfTimer1(100);
			SetLowerVelocityOfTimer1(90);
			SetTimer1();

			// 启动秒表2(90~80)
			SetUpperVelocityOfTimer2(90);
			SetLowerVelocityOfTimer2(80);
			SetTimer2();

			// 关电机
			MotorOff();
			// 进入下一滑行状态
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 100~90
			if (m_fV >= 90.0f)
			{
				// 获取第1段滑行时间(100~90)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
			}
			else
			{
				// 获取第1段滑行时间(100~90)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				// 获取第1段滑行时间(90~80)
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				
				//设置第2阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(80);
				SetLowerVelocityOfTimer1(70);
				SetTimer1();

				// 进入下一滑行状态(90~80)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 90~80
			if (m_fV >= 80.0f)
			{
				// 获取第1段滑行时间
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
			}
			else
			{
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80

				// 第1段标记
				const UINT unSection(0);
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

				//设置第2阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(70);
				SetLowerVelocityOfTimer2(60);
				SetTimer2();

				// 进入下一滑行状态(80~70)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 80~70
			if (m_fV >= 70.0f)
			{
				// 获取第2段滑行时间(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
			}
			else
			{
				// 获取第2段滑行时间(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70

				// 第2段标记
				const UINT unSection(1);
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

				//设置第3阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(60);
				SetLowerVelocityOfTimer1(50);
				SetTimer1();

				// 进入下一滑行状态(70~60)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 70~60
			if (m_fV >= 60.0f)
			{
				// 获取滑行时间(70~60)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
			}
			else
			{				
				// 获取滑行时间(70~60)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60

				// 第3段标记
				const UINT unSection(2);
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

				//设置第4阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(50);
				SetLowerVelocityOfTimer2(40);
				SetTimer2();

				// 进入下一滑行状态(60~50)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 60~50
			if (m_fV >= 50.0f)
			{
				// 获取滑行时间(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
			}
			else
			{				
				// 获取滑行时间(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50

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
				// 发消息到主线程窗口通知滑行计时第五段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				//设置第5阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(40);
				SetLowerVelocityOfTimer1(30);
				SetTimer1();


				// 进入下一滑行状态(50~40)
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 50~40
			if (m_fV >= 40.0f)
			{
				// 获取滑行时间(50~40)
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
				m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT40
			}
			else
			{
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				GetTimeOfTimer1(&m_unPLHPTime10[6]);
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
				m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT40
				
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
				// 发消息到主线程窗口通知滑行计时第六段已经完成
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				
				//设置第6阶段秒表2速度，重新启动
				KillTimer2();
				SetUpperVelocityOfTimer2(30);
				SetLowerVelocityOfTimer2(20);
				SetTimer2();

				// 进入下一滑行状态(40~30)
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 40~30
			if (m_fV >= 30.0f)
			{
				// 获取滑行时间(40~30)
				GetTimeOfTimer1(&m_unPLHPTime10[6]);
				m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT40
				m_unPLHPTime[6] = m_unPLHPTime10[6]+m_unPLHPTime10[7];	//ACDT30
			}
			else
			{
				GetTimeOfTimer1(&m_unPLHPTime10[6]);
				GetTimeOfTimer2(&m_unPLHPTime10[7]);
				m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT40
				m_unPLHPTime[6] = m_unPLHPTime10[6]+m_unPLHPTime10[7];	//ACDT30
				
				// 第6段标记
				const UINT unSection(5);
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

				//设置第7阶段秒表1速度，重新启动
				KillTimer1();
				SetUpperVelocityOfTimer1(20);
				SetLowerVelocityOfTimer1(10);
				SetTimer1();

				// 进入下一滑行状态(30~20)
				m_unSlipState = 8;
			}
		}
		break;
	case 8:
		{
			// 30~20
			if (m_fV >= 20.0f)
			{
				// 获取滑行时间(30~20)
				GetTimeOfTimer2(&m_unPLHPTime10[7]);
				m_unPLHPTime[6] = m_unPLHPTime10[6]+m_unPLHPTime10[7];	//ACDT30
				m_unPLHPTime[7] = m_unPLHPTime10[7]+m_unPLHPTime10[8];	//ACDT20
			}
			else
			{
				GetTimeOfTimer2(&m_unPLHPTime10[7]);
				GetTimeOfTimer1(&m_unPLHPTime10[8]);
				m_unPLHPTime[6] = m_unPLHPTime10[6]+m_unPLHPTime10[7];	//ACDT30
				m_unPLHPTime[7] = m_unPLHPTime10[7]+m_unPLHPTime10[8];	//ACDT20
				
				// 第7段标记
				const UINT unSection(6);
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

				// 进入下一滑行状态(20~10)
				m_unSlipState = 9;
			}
		}
		break;
	case 9:
		{
			// 20~10

			// 获取滑行时间(20~10)
			// 最后一段
			GetTimeOfTimer1(&m_unPLHPTime10[8]);
			m_unPLHPTime[7] = m_unPLHPTime10[7]+m_unPLHPTime10[8];	//ACDT20

			if (m_fV < 10.0f)
			{
				// 第8段标记
				const UINT unSection(7);
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

				//秒表2使用完毕，停止
				KillTimer2();

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

void CNHCPLHPGB2018Thread::RemanenceRecord_ASM(void)
{
	// ASM剩磁记录
	
	if (m_fV > 60.0f)
	{
		// 未进入有效区间范围，不记录剩磁
	}
	else if (m_fV >= 50.0f)
	{
		// 第1段(60~40)
		RemanenceRecordOfSection1();
	}
	else if (m_fV >= 40.0f)
	{
		// 第1段(60~40)
		RemanenceRecordOfSection1();
		// 第2段(50~30)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 35.0f)
	{
		// 第2段(50~30)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 30.0f)
	{
		// 第2段(50~30)
		RemanenceRecordOfSection2();
		// 第3段(35~15)
		RemanenceRecordOfSection3();
	}
	else if (m_fV >= 15.0f)
	{
		// 第3段(35~15)
		RemanenceRecordOfSection3();
	}
}

void CNHCPLHPGB2018Thread::RemanenceRecord_VMAS(void)
{
	// VMAS剩磁记录
	
	RemanenceRecord_ASM();
}

void CNHCPLHPGB2018Thread::RemanenceRecord_LUGDOWN(void)
{
	// LUGDOWN剩磁记录

	if (m_fV > 80.0f)
	{
		// 未进入有效区间范围，不记录剩磁
	}
	else if (m_fV >= 70.0f)
	{
		// 第1段(80~60)
		RemanenceRecordOfSection1();
	}
	else if (m_fV >= 60.0f)
	{
		// 第1段(80~60)
		RemanenceRecordOfSection1();
		// 第2段(70~50)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 50.0f)
	{
		// 第2段(70~50)
		RemanenceRecordOfSection2();
		// 第3段(60~40)
		RemanenceRecordOfSection3();
	}
	else if (m_fV >= 40.0f)
	{
		// 第3段(60~40)
		RemanenceRecordOfSection3();
		// 第4段(50~30)
		RemanenceRecordOfSection4();
	}
	else if (m_fV >= 30.0f)
	{
		// 第4段(50~30)
		RemanenceRecordOfSection4();
		// 第5段(40~20)
		RemanenceRecordOfSection5();
	}
	else if (m_fV >= 20.0f)
	{
		// 第5段(40~20)
		RemanenceRecordOfSection5();
		// 第6段(30~10)
		RemanenceRecordOfSection6();
	}
	else if (m_fV >= 10.0f)
	{
		// 第6段(30~10)
		RemanenceRecordOfSection6();
	}

}

void CNHCPLHPGB2018Thread::RemanenceRecord_LUGDOWN_100(void)
{
	// LUGDOWN剩磁记录

	if (m_fV > 100.0f)
	{
		// 未进入有效区间范围，不记录剩磁
	}
	else if (m_fV >= 90.0f)
	{
		// 第1段(100~80)
		RemanenceRecordOfSection1();
	}
	else if (m_fV >= 80.0f)
	{
		// 第1段(100~80)
		RemanenceRecordOfSection1();
		// 第2段(90~70)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 70.0f)
	{
		// 第2段(90~70)
		RemanenceRecordOfSection2();
		// 第3段(80~60)
		RemanenceRecordOfSection3();
	}
	else if (m_fV >= 60.0f)
	{
		// 第3段(80~60)
		RemanenceRecordOfSection3();
		// 第4段(70~50)
		RemanenceRecordOfSection4();
	}
	else if (m_fV >= 50.0f)
	{
		// 第4段(70~50)
		RemanenceRecordOfSection4();
		// 第5段(60~40)
		RemanenceRecordOfSection5();
	}
	else if (m_fV >= 40.0f)
	{
		// 第5段(60~40)
		RemanenceRecordOfSection5();
		// 第6段(50~30)
		RemanenceRecordOfSection6();
	}
	else if (m_fV >= 30.0f)
	{
		// 第6段(50~30)
		RemanenceRecordOfSection6();
		// 第7段(40~20)
		RemanenceRecordOfSection7();
	}
	else if (m_fV >= 20.0f)
	{
		// 第7段(40~20)
		RemanenceRecordOfSection7();
		// 第8段(30~10)
		RemanenceRecordOfSection8();
	}
	else if (m_fV >= 10.0f)
	{
		// 第8段(30~10)
		RemanenceRecordOfSection8();
	}
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection1(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[0] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[0] += m_fF;
	m_nRemanenceTimes[0]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection2(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[1] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[1] += m_fF;
	m_nRemanenceTimes[1]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection3(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[2] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[2] += m_fF;
	m_nRemanenceTimes[2]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection4(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[3] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[3] += m_fF;
	m_nRemanenceTimes[3]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection5(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[4] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[4] += m_fF;
	m_nRemanenceTimes[4]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection6(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[5] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[5] += m_fF;
	m_nRemanenceTimes[5]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection7(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[6] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[6] += m_fF;
	m_nRemanenceTimes[6]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection8(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[7] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[7] += m_fF;
	m_nRemanenceTimes[7]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection9(void)
{
	// 剩磁记录-功率模式
	m_dRemanenceSumByPower[8] += m_fP;
	// 剩磁记录-力模式
	// TODO:确认是否有溢出
	m_dRemanenceSumByForce[8] += m_fF;
	m_nRemanenceTimes[8]++;
}
