#include "StdAfx.h"
#include "NHCVarLoadThread2.h"

IMPLEMENT_DYNCREATE(CNHCVarLoadThread2,CDynThread)

CNHCVarLoadThread2::CNHCVarLoadThread2(void)
: m_nAccType(0)
, m_fUpperVelocity(0.0f)
, m_fUpperVelocityWithOffset(0.0f)
, m_fLowerVelocity(0.0f)
, m_fMiddleVelocity(0.0f)
, m_fLowerVelocityWithOffset(3.0f)
, m_nUpperVelocityOffset(8)
, m_nLowerVelocityOffset(3)
, m_unTimerRes(50)
, m_unTimerIdOfVarLoad(NULL)
, m_unVarLoadState(NHC_VARLOAD_STATE_NOT_START)
, m_hCallBackWnd(NULL) 
, m_dwLastErrorCode(NHC_VARLOAD_NO_ERROR)
, m_unTestInterval(200)
, m_bIsSpeedUp(false)
, m_nStatues(1)
{
	ZeroMemory(m_fPowerAdd, sizeof(m_fPowerAdd));
	ZeroMemory(m_nSlideTime, sizeof(m_nSlideTime));

}

CNHCVarLoadThread2::~CNHCVarLoadThread2(void)
{
}

BOOL CNHCVarLoadThread2::InitInstance()
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

int CNHCVarLoadThread2::ExitInstance()
{
	// 关闭多媒体定时器
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}

	// 清除多媒体计时器最低分辨率
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCVarLoadThread2, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_VARLOAD_TEST, OnStartVarLoadTest)
	ON_THREAD_MESSAGE(WM_DYN_VARLOAD_TEST_FINISHED, OnFinishVarLoadTest)	
END_MESSAGE_MAP()

// CNHCDIWThread 消息处理程序
void CNHCVarLoadThread2::StartVarLoadTest_Asyn(const HWND hCallBackWnd)
{
	m_fLowerVelocity = 8.0f;
	m_fUpperVelocity = 80.5f;
	m_fMiddleVelocity = 78.8f;
	// 写第一点加载功率指针位置
	WriteVar(85, 0);
	// 写开始变载荷滑行速度点
	WriteVar(86, 8050);
	m_hCallBackWnd = hCallBackWnd;

	m_fUpperVelocityWithOffset = m_fUpperVelocity + m_nUpperVelocityOffset;
	m_fLowerVelocityWithOffset = m_fLowerVelocity - m_nLowerVelocityOffset;
	ASSERT(m_fUpperVelocityWithOffset > m_fLowerVelocityWithOffset);
	if (m_fLowerVelocityWithOffset < 10e-6f)
	{
		m_fLowerVelocityWithOffset = 0.0f;
	}

	PostThreadMessage(WM_DYN_START_VARLOAD_TEST, 0, 0);
}

void CNHCVarLoadThread2::OnStartVarLoadTest(WPARAM wParam, LPARAM lParam)
{
	// 重置检测参数
	m_nStatues = 1;
	ZeroMemory(m_fPowerAdd, sizeof(m_fPowerAdd));
	ZeroMemory(m_nSlideTime, sizeof(m_nSlideTime));
	m_unVarLoadState = NHC_VARLOAD_STATE_READY;
	VarLoadTest();
}

void CNHCVarLoadThread2::VarLoadTest(void)
{
	switch (m_unVarLoadState)
	{
	case NHC_VARLOAD_STATE_READY:
		{
			VarLoadTestStep1();
			break;
		}
	case NHC_VARLOAD_STATE_LIFTBEAM_DOWN:
		{
			VarLoadTestStep2();
			break;
		}
	case NHC_VARLOAD_STATE_ACCELERATION:
		{
			VarLoadTestStep3();
			break;
		}
	case NHC_VARLOAD_STATE_SLIP:
		{
			VarLoadTestStep4();	
			break;
		}
	case NHC_VARLOAD_STATE_WAIT_FOR_STOP:
		{
			VarLoadTestStep5();
			break;
		}
	default:
		break;
	}
}

void CNHCVarLoadThread2::VarLoadTestStep1(void)
{
	// 步骤1:准备就绪
	// 重设测试结果数据
	// 退出所有控制模式
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// 测功机操作失败
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;
		return;
	}
	// 初始化测功机秒表1
	KillTimer1();
	// 设置秒表1开始速度
	SetUpperVelocityOfTimer1(m_fUpperVelocity);
	// 设置秒表1停止速度
	SetLowerVelocityOfTimer1(m_fLowerVelocity);
	// 设置秒表1中间速度
	SetMiddleVelocityOfTimer1(m_fMiddleVelocity);
	// 降下举升器(必须)
	LiftBeamDown();
	// 查询举升器状态
	GetStatus();
	if (m_btStatus.test(1))
	{
		// 举升器已经完全降下
		// 进入加速流程
		// 设状态为：加速
		m_unVarLoadState = NHC_VARLOAD_STATE_ACCELERATION;
		
		// 若加速类型为电机加速，开始加速
		if (0 == m_nAccType)
		{
			MotorOn();
		}	
	}
	else
	{
		// 举升器没有完全降下
		// 进入降下举升器流程
		// 设置状态为：降举升器
		m_unVarLoadState = NHC_VARLOAD_STATE_LIFTBEAM_DOWN;
	}
	// 启动多媒体定时器
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}
	m_unTimerIdOfVarLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfVarLoad, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfVarLoad)
	{
		// 启动多媒体定时器失败
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCVarLoadThread2::TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCVarLoadThread2* pThread = (CNHCVarLoadThread2*)(dwUser);
	pThread->VarLoadTest();
	//TRACE("DIWTest is executed\n\n");
}

void CNHCVarLoadThread2::VarLoadTestStep2(void)
{
	// 步骤2:降举升器
	if (DYN_WR_SUCCEED != GetStatus())
	{
		// 测功机操作失败
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;
		// 停止定时器
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if(m_btStatus.test(1))
	{
		// 举升器已经降下
		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}

		// 若加速类型为电机加速，开始加速
		if (0 == m_nAccType)
		{
			MotorOn();
		}

		// 进入加速流程
		// 设置状态为：加速
		m_unVarLoadState = NHC_VARLOAD_STATE_ACCELERATION;

		// 重新启动定时器
		m_unTimerIdOfVarLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfVarLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfVarLoad)
		{
			// 启动多媒体定时器失败
			m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
			m_dwLastErrorCode = NHC_VARLOAD_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCVarLoadThread2::VarLoadTestStep3(void)
{
	// 步骤3:加速
	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// 到达停止速度
		m_bIsSpeedUp = true;
		//第一段
		m_nStatues=1; 

		// 暂停定时器，设置下一测试状态参数，然后重新启动
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}

		// 启动秒表1
		SetTimer1();
		// 若加速类型为电机加速，停止电机
		if (0 == m_nAccType)
		{
			MotorOff();
		}		
		// 启动变载荷控制模式
		VarLoadControlModeOn(); 
		// 进入滑行流程
		// 设置状态为：滑行，计时阶段
		m_unVarLoadState = NHC_VARLOAD_STATE_SLIP;

		// 重新启动定时器
		m_unTimerIdOfVarLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfVarLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfVarLoad)
		{
			// 启动多媒体定时器失败
			m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
			m_dwLastErrorCode = NHC_VARLOAD_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCVarLoadThread2::VarLoadTestStep4(void)
{
	// 步骤4:滑行

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// 停止定时器
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// 处于加载计时阶段
		// 实时取秒表时间
		switch(m_nStatues)														//判断要测试的速度段
		{
		case 1://3.7
			if( m_fV < 80.5)									//第一段速度起点80.5mp/h
			{
				m_nStatues++;
				m_fPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://加载4.4kw
			if(m_fV<78.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)77.20);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[1]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://加载5.1kw
			if(m_fV<77.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)75.60);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[2]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://加载5.9kw
			if(m_fV<75.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)74.00);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[3]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://加载6.6kw
			if(m_fV<74.00)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)72.40);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[4]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://加载7.4kw
			if(m_fV<72.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)70.80);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[5]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://加载5.9kw
			if(m_fV<70.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)69.20);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[6]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://加载7.4kw
			if(m_fV<69.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)67.60);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[7]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://加载8.8kw
			if(m_fV<67.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)66.00);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[8]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://加载10.3kw
			if(m_fV<66.00)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)64.40);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[9]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://加载11.8kw
			if(m_fV<64.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)62.80);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[10]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://加载13.2kw
			if(m_fV<62.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)61.10);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[11]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://加载14.7kw
			if(m_fV<61.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)59.50);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[12]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://加载15.4kw
			if(m_fV<59.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)57.90);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[13]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;		
		case 15://加载16.2kw
			if(m_fV<57.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)56.30);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[14]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://加载16.9kw
			if(m_fV<56.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)54.70);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[15]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://加载17.6kw
			if(m_fV<54.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)53.10);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[16]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://加载18.4kw
			if(m_fV<53.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)51.50);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[17]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://加载17.6kw
			if(m_fV<51.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)49.90);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[18]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://加载16.9kw
			if(m_fV<49.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)48.30);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[19]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 21://加载16.2kw
			if(m_fV<48.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)46.70);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[20]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 22://加载15.4kw
			if(m_fV<46.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.10);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[21]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 23://加载14.7kw
			if(m_fV<45.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.40);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[22]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 24://加载13.2kw
			if(m_fV<43.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.80);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[23]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 25://加载11.8kw
			if(m_fV<41.8)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.2);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[24]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 26://加载10.3kw
			if(m_fV<40.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.60);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[25]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 27://加载11.0kw
			if(m_fV<38.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.00);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[26]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 28://加载11.8kw
			if(m_fV<36.7)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[27]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 29://加载12.5kw
			if(m_fV<35.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[28]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;		
		case 30://加载13.2kw
			if(m_fV<33.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.20);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[29]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 31://加载12.5kw
			if(m_fV<32.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.60);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[30]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 32://加载11.8kw
			if(m_fV<30.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)29.00);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[31]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 33://加载11.0kw
			if(m_fV<29.00)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.40);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[32]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 34://加载10.3kw
			if(m_fV<27.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.70);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[33]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 35://加载8.8kw
			if(m_fV<25.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.10);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[34]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 36://加载7.4kw
			if(m_fV<24.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.50);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[35]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 37://加载8.1kw
			if(m_fV<22.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.90);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[36]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 38://加载8.8kw
			if(m_fV<20.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.30);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[37]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 39://加载8.1kw
			if(m_fV<19.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[38]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 40://加载7.4kw
			if(m_fV<17.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)16.10);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[39]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 41://加载6.6kw
			if(m_fV<16.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)14.50);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[40]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 42://加载5.9kw
			if(m_fV<14.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)12.90);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[41]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 43://加载5.1kw
			if(m_fV<12.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)11.30);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[42]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 44://加载4.4kw
			if(m_fV<11.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)9.70);	//设置秒表1中间速度
				GetMiddleTimeOfTimer1(&m_nSlideTime[43]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 45://加载3.7kw
			if(m_fV<9.70)
			{
				m_nStatues++;
				GetMiddleTimeOfTimer1(&m_nSlideTime[44]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 46:
			if (m_fV < 8.00)
			{
				// 80.5~8.0的时间
				GetTimeOfTimer1(&m_nSlideTime[45]);
				// 停止秒表1
				KillTimer1();
				// 退出所以控制模式
				ControlModeOff();
				// 设置状态等待滑行
				m_unVarLoadState = NHC_VARLOAD_STATE_WAIT_FOR_STOP;
			}
		default:break;
		}
	}
}

void CNHCVarLoadThread2::VarLoadTestStep5(void)
{
	// 步骤5:等待滚筒停止
	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// 执行测功机操作失败
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;
		// 停止定时器
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}

	if (m_fV < 2.0f)
	{
		// 暂停定时器
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		// 设置状态为：完成
		m_unVarLoadState = NHC_VARLOAD_STATE_FINISHED;

		//发送完成实验
		PostThreadMessage(WM_DYN_VARLOAD_TEST_FINISHED, 0, 0);
	}
}

void CNHCVarLoadThread2::OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam)
{
	m_unVarLoadState = NHC_VARLOAD_STATE_FINISHED;
	// 计算变载荷测试结果
	//CalcVarLoadResult();
}

void CNHCVarLoadThread2::StopVarLoadTest(void)
{
	// 停止定时器
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}
	// 若加速类型为电机加速，停止电机
	if (0 == m_nAccType)
	{
		MotorOff();
	}
	KillTimer1();
	ControlModeOff();

	// 设置状态：测试未开始
	m_unTimerIdOfVarLoad = NHC_VARLOAD_STATE_NOT_START;
	// 重设测试结果数据
	m_nStatues = 1;
	ZeroMemory(m_fPowerAdd, sizeof(m_fPowerAdd));
	ZeroMemory(m_nSlideTime, sizeof(m_nSlideTime));
}
