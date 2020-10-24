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
	// TODO: �ڴ�ִ���������̳߳�ʼ��

	// ���ö�ý���ʱ����ͷֱ���
	TIMECAPS tc;
	if (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(TIMECAPS)))
	{
		// ��ȡϵͳ֧�ֱַ��ʣ���ѡ���ʵ��ķֱ���(>=50ms)
		m_unTimerRes = min(max(tc.wPeriodMin, 50), tc.wPeriodMax);
	}
	else
	{
		// ��ȡϵͳ֧�ֱַ��ʳ���ֱ����Ϊ50ms
		m_unTimerRes = 50;
	}
	timeBeginPeriod(m_unTimerRes);

	return TRUE;
}

int CNHCVarLoadThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����

	// �رն�ý�嶨ʱ��
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}

	// �����ý���ʱ����ͷֱ���
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCVarLoadThread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_VARLOAD_TEST, OnStartVarLoadTest)
	ON_THREAD_MESSAGE(WM_DYN_VARLOAD_TEST_FINISHED, OnFinishVarLoadTest)
END_MESSAGE_MAP()


// CNHCDIWThread ��Ϣ�������
void CNHCVarLoadThread::StartVarLoadTest_Asyn(const BYTE bTestType,  const HWND hCallBackWnd)
{
	ASSERT(bTestType);
	m_TestType = bTestType;
	if (ASM==bTestType)
	{
		m_fLowerVelocity = 16.1f;
		m_fUpperVelocity = 48.3f;
		m_fMiddleVelocity = 46.7f;
		// д��һ����ع���ָ��λ��
		WriteVar(85,20);
		// д��ʼ���غɻ����ٶȵ�
		WriteVar(86,4830);
	}
	else if(VMAS==bTestType)
	{
		m_fLowerVelocity = 16.1f;
		m_fUpperVelocity = 48.3f;
		m_fMiddleVelocity = 46.7f;
		// д��һ����ع���ָ��λ��
		WriteVar(85,20);
		// д��ʼ���غɻ����ٶȵ�
		WriteVar(86,4830);
	}
	else
	{
		m_fLowerVelocity = 16.1f;
		m_fUpperVelocity = 72.4f;
		m_fMiddleVelocity = 70.8f;
		// д��һ����ع���ָ��λ��
		WriteVar(85,5);
		// д��ʼ���غɻ����ٶȵ�
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
	// ���ü�����
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
	// ������غɲ��Խ��
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
			case ASM:           // ����ASM���们��
				{
					VarLoadTestOfAsmStep4();
					break;
				}
			case VMAS:          // ����VMAS���们��
				{
					VarLoadTestOfVmasStep4();
					break;
				}
			case LUGDOWN:      // ����LUGDOWN���们��
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
	// ����1:׼������

	// ������Խ������

	// �˳����п���ģʽ
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// �⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;
		return;
	}
	// ��ʼ���⹦�����1
	KillTimer1();
	// �������1��ʼ�ٶ�
	SetUpperVelocityOfTimer1(m_fUpperVelocity);
	// �������1ֹͣ�ٶ�
	SetLowerVelocityOfTimer1(m_fLowerVelocity);
	// �������1�м��ٶ�
	SetMiddleVelocityOfTimer1(m_fMiddleVelocity);
	// ���¾�����(����)
	LiftBeamDown();
	// ��ѯ������״̬
	GetStatus();
	if (m_btStatus.test(1))
	{
		// �������Ѿ���ȫ����
		// �����������
		// ��״̬Ϊ������
		m_unVarLoadState = NHC_VARLOAD_STATE_ACCELERATION;
		// �����������ʼ����
		MotorOn();
	}
	else
	{
		// ������û����ȫ����
		// ���뽵�¾���������
		// ����״̬Ϊ����������
		m_unVarLoadState = NHC_VARLOAD_STATE_LIFTBEAM_DOWN;
	}
	// ������ý�嶨ʱ��
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}
	m_unTimerIdOfVarLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfVarLoad, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfVarLoad)
	{
		// ������ý�嶨ʱ��ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCVarLoadThread::VarLoadTestStep2(void)
{
	// ����2:��������

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// �⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if(m_btStatus.test(1))
	{
		// �������Ѿ�����

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}

		// �����������ʼ����
		MotorOn();

		// �����������
		// ����״̬Ϊ������
		m_unVarLoadState = NHC_VARLOAD_STATE_ACCELERATION;

		// ����������ʱ��
		m_unTimerIdOfVarLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfVarLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfVarLoad)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
			m_dwLastErrorCode = NHC_VARLOAD_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCVarLoadThread::VarLoadTestStep3(void)
{
	// ����3:����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// ����ֹͣ�ٶ�
		m_bIsSpeedUp = true;
		//��һ��
		m_nStatues=1; 

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}

		// �������1
		SetTimer1();
		// ֹͣ���
		MotorOff();
		// �������غɿ���ģʽ
		VarLoadControlModeOn(); 
		// ���뻬������
		// ����״̬Ϊ�����У���ʱ�׶�
		m_unVarLoadState = NHC_VARLOAD_STATE_SLIP;

		// ����������ʱ��
		m_unTimerIdOfVarLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfVarLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfVarLoad)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
			m_dwLastErrorCode = NHC_VARLOAD_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCVarLoadThread::VarLoadTestOfLugdownStep4(void)
{
	// ����4:����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// ���ڼ��ؼ�ʱ�׶�
		// ʵʱȡ���ʱ��
		switch(m_nStatues)                                                         //�ж�Ҫ���Ե��ٶȶ�
		{
		case 1://3.7
			if( m_fV < 72.22)                                           //��һ���ٶ����72.2mp/h
			{
				m_nStatues++;
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://����5.9kw
			if(m_fV<70.61)                     
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)69.20);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[1]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://����7.4kw 
			if(m_fV<69.00)                
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)67.60);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[2]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://����8.8kw
			if(m_fV<67.40)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)65.98);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[3]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://����10.3kw
			if(m_fV<65.78)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)64.37);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[4]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://����11.8kw
			if(m_fV<64.17)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)62.76);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[5]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://����13.2kw
			if (m_fV<62.56)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)61.15);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[6]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://����14.7kw 
			if (m_fV<60.95)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)59.54);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[7]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://����15.4kw
			if(m_fV<59.34)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)57.93);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[8]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://����16.2kw
			if(m_fV<57.73)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)56.33);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[9]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://����16.9kw
			if(m_fV<56.13)               
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)54.72);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[10]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://����17.6kw
			if(m_fV<54.52)                
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)53.11);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[11]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://����18.4kw
			if (m_fV<52.91)                
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)51.50);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[12]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://����17.6kw
			if (m_fV<51.30)              
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)49.89);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[13]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 15://����16.9kw
			if (m_fV < 49.69f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)48.28);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[14]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://����16.2kw
			if (m_fV < 48.08f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)46.67);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[15]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://����15.4kw
			if (m_fV < 46.47f)           
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.06);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[16]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://����14.7kw
			if(m_fV < 44.86f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.45);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[17]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://����13.2kw
			if (m_fV < 43.25f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.84);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[18]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://����11.8kw
			if (m_fV < 41.64f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.23);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[19]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 21://����10.3kw
			if (m_fV<40.03f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.62);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[20]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 22://����11.0kw
			if (m_fV < 38.42f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.01);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[21]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 23://����11.8kw
			if (m_fV < 36.81f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[22]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 24://����12.5kw 
			if (m_fV < 35.20f)      
			{ 
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[23]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 25://����13.2kw
			if (m_fV < 33.60f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.19);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[24]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 26://����12.5kw
			if ( m_fV < 31.99f)    
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.58);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[25]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 27://����11.8kw
			if (m_fV < 30.38f)      
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)28.97);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[26]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 28://����11.0kw 
			if (m_fV < 28.77f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.36);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[27]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 29://����10.3kw
			if (m_fV < 27.16f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.75);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[28]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 30://����8.8kw
			if (m_fV < 25.55f)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.14);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[29]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 31://����7.4kw
			if (m_fV < 23.94f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.53);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[30]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 32://����8.1kw
			if (m_fV < 22.33f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.92);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[31]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 33://����8.8kw
			if (m_fV < 20.72f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.31);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[32]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 34://����8.1kw
			if (m_fV < 19.11f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nLugdownSlideTime[33]);
				m_fLugdownPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 35://����7.4kw
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
				// 72.4~16.1��ʱ��
				GetTimeOfTimer1(&m_nLugdownSlideTime[35]);
				// ֹͣ���1
				KillTimer1();
				// �˳����Կ���ģʽ
				ControlModeOff();
				// ����״̬�ȴ�����
				m_unVarLoadState = NHC_VARLOAD_STATE_WAIT_FOR_STOP;
			}
		default:break;
		}
	}
}

void CNHCVarLoadThread::VarLoadTestOfAsmStep4(void)
{
	// ����4:����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// ���ڼ��ؼ�ʱ�׶�
		// ʵʱȡ���ʱ��
		switch(m_nStatues)                                                         //�ж�Ҫ���Ե��ٶȶ�
		{
		case 1://3.7
			if( m_fV < 48.3)                                           //��һ���ٶ����48.3mp/h
			{
				m_nStatues++;
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://����15.4kw
			if (m_fV < 46.47f)           
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.06);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[1]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://����14.7kw
			if(m_fV < 44.86f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.45);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[2]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://����13.2kw
			if (m_fV < 43.25f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.84);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[3]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://����11.8kw
			if (m_fV < 41.64f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.23);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[4]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://����10.3kw
			if (m_fV<40.03f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.62);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[5]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://����11.0kw
			if (m_fV < 38.42f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.01);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[6]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://����11.8kw
			if (m_fV < 36.81f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[7]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://����12.5kw 
			if (m_fV < 35.20f)      
			{ 
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[8]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://����13.2kw
			if (m_fV < 33.60f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.19);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[9]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://����12.5kw
			if ( m_fV < 31.99f)    
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.58);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[10]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://����11.8kw
			if (m_fV < 30.38f)      
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)28.97);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[11]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://����11.0kw 
			if (m_fV < 28.77f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.36);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[12]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://����10.3kw
			if (m_fV < 27.16f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.75);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[13]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 15://����8.8kw
			if (m_fV < 25.55f)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.14);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[14]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://����7.4kw
			if (m_fV < 23.94f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.53);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[15]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://����8.1kw
			if (m_fV < 22.33f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.92);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[16]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://����8.8kw
			if (m_fV < 20.72f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.31);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[17]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://����8.1kw
			if (m_fV < 19.11f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nAsmSlideTime[18]);
				m_fAsmPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://����7.4kw
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
				//48.3~16.1ʱ��
				GetTimeOfTimer1(&m_nAsmSlideTime[20]);
				//ֹͣ���1
				KillTimer1();
				// �˳����Կ���ģʽ
				ControlModeOff();
				// ����״̬�ȴ�����
				m_unVarLoadState = NHC_VARLOAD_STATE_WAIT_FOR_STOP;
			}
		default:break;
		}
	}
}

void CNHCVarLoadThread::VarLoadTestOfVmasStep4(void)
{
	// ����4:����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// ���ڼ��ؼ�ʱ�׶�
		// ʵʱȡ���ʱ��
		switch(m_nStatues)                                                         //�ж�Ҫ���Ե��ٶȶ�
		{
		case 1://3.7
			if( m_fV < 48.3)                                           //��һ���ٶ����48.3mp/h
			{
				m_nStatues++;
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://����15.4kw
			if (m_fV < 46.47f)           
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.06);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[1]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://����14.7kw
			if(m_fV < 44.86f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.45);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[2]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://����13.2kw
			if (m_fV < 43.25f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.84);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[3]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://����11.8kw
			if (m_fV < 41.64f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.23);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[4]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://����10.3kw
			if (m_fV<40.03f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.62);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[5]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://����11.0kw
			if (m_fV < 38.42f)          
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.01);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[6]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://����11.8kw
			if (m_fV < 36.81f)         
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[7]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://����12.5kw 
			if (m_fV < 35.20f)      
			{ 
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[8]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://����13.2kw
			if (m_fV < 33.60f)       
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.19);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[9]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://����12.5kw
			if ( m_fV < 31.99f)    
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.58);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[10]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://����11.8kw
			if (m_fV < 30.38f)      
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)28.97);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[11]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://����11.0kw 
			if (m_fV < 28.77f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.36);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[12]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://����10.3kw
			if (m_fV < 27.16f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.75);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[13]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 15://����8.8kw
			if (m_fV < 25.55f)             
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.14);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[14]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://����7.4kw
			if (m_fV < 23.94f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.53);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[15]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://����8.1kw
			if (m_fV < 22.33f)            
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.92);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[16]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://����8.8kw
			if (m_fV < 20.72f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.31);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[17]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://����8.1kw
			if (m_fV < 19.11f)        
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);    //�������1�м��ٶ� 
				GetMiddleTimeOfTimer1(&m_nVmasSlideTime[18]);
				m_fVmasPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://����7.4kw
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
				//48.3~16.1ʱ��
				GetTimeOfTimer1(&m_nVmasSlideTime[20]);
				//ֹͣ���1
				KillTimer1();
				// �˳����Կ���ģʽ
				ControlModeOff();
				// ����״̬�ȴ�����
				m_unVarLoadState = NHC_VARLOAD_STATE_WAIT_FOR_STOP;
			}
		default:break;
		}
	}
}

void CNHCVarLoadThread::VarLoadTestStep5(void)
{
	// ����5:�ȴ���Ͳֹͣ

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unVarLoadState = NHC_VARLOAD_STATE_ERROR;
		m_dwLastErrorCode = NHC_VARLOAD_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		return;
	}

	if (m_fV < 2.0f)
	{
		// ��ͣ��ʱ��
		if (NULL != m_unTimerIdOfVarLoad)
		{
			timeKillEvent(m_unTimerIdOfVarLoad);
			m_unTimerIdOfVarLoad = NULL;
		}
		// ����״̬Ϊ�����
		m_unVarLoadState = NHC_VARLOAD_STATE_FINISHED;

		//�������ʵ��
		PostThreadMessage(WM_DYN_VARLOAD_TEST_FINISHED, 0, 0);
	}
}

void CNHCVarLoadThread::StopVarLoadTest(void)
{
	// ֹͣ��ʱ��
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}

	MotorOff();
	KillTimer1();
	ControlModeOff();

	// ����״̬������δ��ʼ
	m_unTimerIdOfVarLoad = NHC_VARLOAD_STATE_NOT_START;
	// ������Խ������
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