#include "StdAfx.h"
#include "NHCVarLoadThread.h"

IMPLEMENT_DYNCREATE(CNHCVarLoadThread, CDynThread)

CNHCVarLoadThread::CNHCVarLoadThread(void)
: m_fUpperVelocity(0.0f)
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
, m_TestType(ASM)
, m_bIsSpeedUp(false)
, m_nStatues(1)
{
	ZeroMemory(m_fLugdownPowerAdd, sizeof(m_fLugdownPowerAdd));
	ZeroMemory(m_fAsmPowerAdd, sizeof(m_fAsmPowerAdd));
	ZeroMemory(m_fVmasPowerAdd, sizeof(m_fVmasPowerAdd));
	ZeroMemory(m_nLugdownSlideTime, sizeof(m_nLugdownSlideTime));
	ZeroMemory(m_nAsmSlideTime, sizeof(m_nAsmSlideTime));
	ZeroMemory(m_nVmasSlideTime, sizeof(m_nVmasSlideTime));
}

CNHCVarLoadThread::~CNHCVarLoadThread(void)
{
}
BOOL CNHCVarLoadThread::InitInstance()
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

int CNHCVarLoadThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理

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

BEGIN_MESSAGE_MAP(CNHCVarLoadThread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_VARLOAD_TEST, OnStartVarLoadTest)
	ON_THREAD_MESSAGE(WM_DYN_VARLOAD_TEST_FINISHED, OnFinishVarLoadTest)
END_MESSAGE_MAP()


// CNHCDIWThread 消息处理程序
void CNHCVarLoadThread::StartVarLoadTest_Asyn(const BYTE bTestType,  const HWND hCallBackWnd)
{
	ASSERT(bTestType);
	m_TestType = bTestType;
	if (ASM==bTestType)
	{
		m_fLowerVelocity = 16.1f;
		m_fUpperVelocity = 48.3f;
		m_fMiddleVelocity = 46.7f;
		// 写第一点加载功率指针位置
		WriteVar(85,20);
		// 写开始变载荷滑行速度点
		WriteVar(86,4830);
	}
	else if(VMAS==bTestType)
	{
		m_fLowerVelocity = 16.1f;
		m_fUpperVelocity = 48.3f;
		m_fMiddleVelocity = 46.7f;
		// 写第一点加载功率指针位置
		WriteVar(85,20);
		// 写开始变载荷滑行速度点
		WriteVar(86,4830);
	}
	else
	{
		m_fLowerVelocity = 16.1f;
		m_fUpperVelocity = 72.4f;
		m_fMiddleVelocity = 70.8f;
		// 写第一点加载功率指针位置
		WriteVar(85,5);
		// 写开始变载荷滑行速度点
		WriteVar(86,7240);
	}
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

void CNHCVarLoadThread::OnStartVarLoadTest(WPARAM wParam, LPARAM lParam)
{
	// 重置检测参数
	m_nStatues = 1;
	ZeroMemory(m_fLugdownPowerAdd, sizeof(m_fLugdownPowerAdd));
	ZeroMemory(m_fAsmPowerAdd, sizeof(m_fAsmPowerAdd));
	ZeroMemory(m_fVmasPowerAdd, sizeof(m_fVmasPowerAdd));
	ZeroMemory(m_nLugdownSlideTime, sizeof(m_nLugdownSlideTime));
	ZeroMemory(m_nAsmSlideTime, sizeof(m_nAsmSlideTime));
	ZeroMemory(m_nVmasSlideTime, sizeof(m_nVmasSlideTime));

	m_unVarLoadState = NHC_VARLOAD_STATE_READY;
	VarLoadTest();
}

void CNHCVarLoadThread::OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam)
{
	m_unVarLoadState = NHC_VARLOAD_STATE_FINISHED;
	// 计算变载荷测试结果
	CalcVarLoadResult();
}
void CNHCVarLoadThread::CalcVarLoadResult(void)
{

}
void CNHCVarLoadThread::VarLoadTest(void)
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
			switch(m_TestType)
			{
			case ASM:           // 进行ASM区间滑行
				{
					VarLoadTestOfAsmStep4();
					break;
				}
			case VMAS:          // 进行VMAS区间滑行
				{
					VarLoadTestOfVmasStep4();
					break;
				}
			case LUGDOWN:      // 进行LUGDOWN区间滑行
				{
					VarLoadTestOfLugdownStep4();
					break;
				}
			default:
				break;
			}
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

void CNHCVarLoadThread::VarLoadTestStep1(void)
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
		// 启动电机，开始加速
		MotorOn();
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

void CNHCVarLoadThread::VarLoadTestStep2(void)
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

		// 启动电机，开始加速
		MotorOn();

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

void CNHCVarLoadThread::VarLoadTestStep3(void)
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
		// 停止电机
		MotorOff();
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

void CNHCVarLoadThread::VarLoadTestOfLugdownStep4(void)
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
		switch(m_nStatues)                                                         //判断要测试的速度段
		{
		case 1://3.7
			if( m_fV < 72.22)                                           //第一段速度起点72.2mp/h
			{
				m_nStatues++;
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://加载5.9kw
			if(m_fV<70.61)                     
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)69.20);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[1]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://加载7.4kw 
			if(m_fV<69.00)                
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)67.60);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[2]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://加载8.8kw
			if(m_fV<67.40)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)65.98);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[3]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://加载10.3kw
			if(m_fV<65.78)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)64.37);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[4]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://加载11.8kw
			if(m_fV<64.17)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)62.76);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[5]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://加载13.2kw
			if (m_fV<62.56)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)61.15);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[6]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://加载14.7kw 
			if (m_fV<60.95)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)59.54);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[7]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://加载15.4kw
			if(m_fV<59.34)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)57.93);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[8]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://加载16.2kw
			if(m_fV<57.73)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)56.33);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[9]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://加载16.9kw
			if(m_fV<56.13)               
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)54.72);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[10]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://加载17.6kw
			if(m_fV<54.52)                
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)53.11);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[11]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://加载18.4kw
			if (m_fV<52.91)                
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)51.50);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[12]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://加载17.6kw
			if (m_fV<51.30)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)49.89);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[13]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 15://加载16.9kw
			if (m_fV < 49.69f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)48.28);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[14]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://加载16.2kw
			if (m_fV < 48.08f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)46.67);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[15]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://加载15.4kw
			if (m_fV < 46.47f)           
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.06);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[16]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://加载14.7kw
			if(m_fV < 44.86f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.45);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[17]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://加载13.2kw
			if (m_fV < 43.25f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.84);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[18]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://加载11.8kw
			if (m_fV < 41.64f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.23);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[19]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 21://加载10.3kw
			if (m_fV<40.03f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.62);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[20]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 22://加载11.0kw
			if (m_fV < 38.42f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.01);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[21]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 23://加载11.8kw
			if (m_fV < 36.81f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[22]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 24://加载12.5kw 
			if (m_fV < 35.20f)      
			{ 
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[23]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 25://加载13.2kw
			if (m_fV < 33.60f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.19);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[24]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 26://加载12.5kw
			if ( m_fV < 31.99f)    
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.58);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[25]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 27://加载11.8kw
			if (m_fV < 30.38f)      
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)28.97);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[26]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 28://加载11.0kw 
			if (m_fV < 28.77f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.36);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[27]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 29://加载10.3kw
			if (m_fV < 27.16f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.75);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[28]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 30://加载8.8kw
			if (m_fV < 25.55f)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.14);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[29]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 31://加载7.4kw
			if (m_fV < 23.94f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.53);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[30]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 32://加载8.1kw
			if (m_fV < 22.33f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.92);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[31]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 33://加载8.8kw
			if (m_fV < 20.72f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.31);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[32]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 34://加载8.1kw
			if (m_fV < 19.11f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[33]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 35://加载7.4kw
			if (m_fV < 17.50f)         
			{
				m_nStatues++; 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[34]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 36:
			if (m_fV < 15.89f)
			{
				// 72.4~16.1的时间
				GetTimeOfTimer1(&m_nLugdownSlideTime[35]);
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

void CNHCVarLoadThread::VarLoadTestOfAsmStep4(void)
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
		switch(m_nStatues)                                                         //判断要测试的速度段
		{
		case 1://3.7
			if( m_fV < 48.3)                                           //第一段速度起点48.3mp/h
			{
				m_nStatues++;
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://加载15.4kw
			if (m_fV < 46.47f)           
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.06);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[1]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://加载14.7kw
			if(m_fV < 44.86f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.45);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[2]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://加载13.2kw
			if (m_fV < 43.25f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.84);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[3]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://加载11.8kw
			if (m_fV < 41.64f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.23);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[4]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://加载10.3kw
			if (m_fV<40.03f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.62);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[5]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://加载11.0kw
			if (m_fV < 38.42f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.01);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[6]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://加载11.8kw
			if (m_fV < 36.81f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[7]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://加载12.5kw 
			if (m_fV < 35.20f)      
			{ 
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[8]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://加载13.2kw
			if (m_fV < 33.60f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.19);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[9]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://加载12.5kw
			if ( m_fV < 31.99f)    
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.58);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[10]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://加载11.8kw
			if (m_fV < 30.38f)      
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)28.97);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[11]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://加载11.0kw 
			if (m_fV < 28.77f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.36);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[12]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://加载10.3kw
			if (m_fV < 27.16f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.75);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[13]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 15://加载8.8kw
			if (m_fV < 25.55f)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.14);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[14]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://加载7.4kw
			if (m_fV < 23.94f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.53);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[15]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://加载8.1kw
			if (m_fV < 22.33f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.92);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[16]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://加载8.8kw
			if (m_fV < 20.72f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.31);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[17]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://加载8.1kw
			if (m_fV < 19.11f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[18]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://加载7.4kw
			if (m_fV < 17.50f)         
			{
				m_nStatues++;
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[19]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 21:
			if (m_fV < 15.92f)
			{
				//48.3~16.1时间
				GetTimeOfTimer1(&m_nAsmSlideTime[20]);
				//停止秒表1
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

void CNHCVarLoadThread::VarLoadTestOfVmasStep4(void)
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
		switch(m_nStatues)                                                         //判断要测试的速度段
		{
		case 1://3.7
			if( m_fV < 48.3)                                           //第一段速度起点48.3mp/h
			{
				m_nStatues++;
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://加载15.4kw
			if (m_fV < 46.47f)           
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.06);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[1]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://加载14.7kw
			if(m_fV < 44.86f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.45);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[2]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://加载13.2kw
			if (m_fV < 43.25f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.84);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[3]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://加载11.8kw
			if (m_fV < 41.64f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.23);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[4]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://加载10.3kw
			if (m_fV<40.03f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.62);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[5]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://加载11.0kw
			if (m_fV < 38.42f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.01);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[6]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://加载11.8kw
			if (m_fV < 36.81f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[7]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://加载12.5kw 
			if (m_fV < 35.20f)      
			{ 
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[8]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://加载13.2kw
			if (m_fV < 33.60f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.19);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[9]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://加载12.5kw
			if ( m_fV < 31.99f)    
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.58);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[10]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://加载11.8kw
			if (m_fV < 30.38f)      
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)28.97);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[11]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://加载11.0kw 
			if (m_fV < 28.77f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.36);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[12]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://加载10.3kw
			if (m_fV < 27.16f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.75);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[13]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 15://加载8.8kw
			if (m_fV < 25.55f)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.14);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[14]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://加载7.4kw
			if (m_fV < 23.94f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.53);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[15]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://加载8.1kw
			if (m_fV < 22.33f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.92);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[16]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://加载8.8kw
			if (m_fV < 20.72f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.31);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[17]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://加载8.1kw
			if (m_fV < 19.11f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);    //设置秒表1中间速度 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[18]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://加载7.4kw
			if (m_fV < 17.50f)         
			{
				m_nStatues++;
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[19]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 21:
			if (m_fV < 15.92f)
			{
				//48.3~16.1时间
				GetTimeOfTimer1(&m_nVmasSlideTime[20]);
				//停止秒表1
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

void CNHCVarLoadThread::VarLoadTestStep5(void)
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

void CNHCVarLoadThread::StopVarLoadTest(void)
{
	// 停止定时器
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}

	MotorOff();
	KillTimer1();
	ControlModeOff();

	// 设置状态：测试未开始
	m_unTimerIdOfVarLoad = NHC_VARLOAD_STATE_NOT_START;
	// 重设测试结果数据
	m_nStatues = 1;
	ZeroMemory(m_fLugdownPowerAdd, sizeof(m_fLugdownPowerAdd));
	ZeroMemory(m_fAsmPowerAdd, sizeof(m_fAsmPowerAdd));
	ZeroMemory(m_fVmasPowerAdd, sizeof(m_fVmasPowerAdd));
	ZeroMemory(m_nLugdownSlideTime, sizeof(m_nLugdownSlideTime));
	ZeroMemory(m_nAsmSlideTime, sizeof(m_nAsmSlideTime));
	ZeroMemory(m_nVmasSlideTime, sizeof(m_nVmasSlideTime));
}

void CNHCVarLoadThread::TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCVarLoadThread* pThread = (CNHCVarLoadThread*)(dwUser);
	pThread->VarLoadTest();
	//TRACE("DIWTest is executed\n\n");
}