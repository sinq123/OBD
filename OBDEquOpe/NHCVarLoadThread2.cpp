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

int CNHCVarLoadThread2::ExitInstance()
{
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

BEGIN_MESSAGE_MAP(CNHCVarLoadThread2, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_VARLOAD_TEST, OnStartVarLoadTest)
	ON_THREAD_MESSAGE(WM_DYN_VARLOAD_TEST_FINISHED, OnFinishVarLoadTest)	
END_MESSAGE_MAP()

// CNHCDIWThread ��Ϣ�������
void CNHCVarLoadThread2::StartVarLoadTest_Asyn(const HWND hCallBackWnd)
{
	m_fLowerVelocity = 8.0f;
	m_fUpperVelocity = 80.5f;
	m_fMiddleVelocity = 78.8f;
	// д��һ����ع���ָ��λ��
	WriteVar(85, 0);
	// д��ʼ���غɻ����ٶȵ�
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
	// ���ü�����
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
		
		// ����������Ϊ������٣���ʼ����
		if (0 == m_nAccType)
		{
			MotorOn();
		}	
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

void CNHCVarLoadThread2::TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCVarLoadThread2* pThread = (CNHCVarLoadThread2*)(dwUser);
	pThread->VarLoadTest();
	//TRACE("DIWTest is executed\n\n");
}

void CNHCVarLoadThread2::VarLoadTestStep2(void)
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

		// ����������Ϊ������٣���ʼ����
		if (0 == m_nAccType)
		{
			MotorOn();
		}

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

void CNHCVarLoadThread2::VarLoadTestStep3(void)
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
		// ����������Ϊ������٣�ֹͣ���
		if (0 == m_nAccType)
		{
			MotorOff();
		}		
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

void CNHCVarLoadThread2::VarLoadTestStep4(void)
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
		switch(m_nStatues)														//�ж�Ҫ���Ե��ٶȶ�
		{
		case 1://3.7
			if( m_fV < 80.5)									//��һ���ٶ����80.5mp/h
			{
				m_nStatues++;
				m_fPowerAdd[m_nStatues] = m_fP;
			} 
			break;
		case 2://����4.4kw
			if(m_fV<78.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)77.20);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[1]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 3://����5.1kw
			if(m_fV<77.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)75.60);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[2]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 4://����5.9kw
			if(m_fV<75.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)74.00);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[3]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 5://����6.6kw
			if(m_fV<74.00)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)72.40);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[4]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 6://����7.4kw
			if(m_fV<72.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)70.80);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[5]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 7://����5.9kw
			if(m_fV<70.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)69.20);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[6]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 8://����7.4kw
			if(m_fV<69.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)67.60);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[7]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 9://����8.8kw
			if(m_fV<67.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)66.00);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[8]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 10://����10.3kw
			if(m_fV<66.00)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)64.40);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[9]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 11://����11.8kw
			if(m_fV<64.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)62.80);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[10]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 12://����13.2kw
			if(m_fV<62.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)61.10);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[11]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 13://����14.7kw
			if(m_fV<61.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)59.50);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[12]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 14://����15.4kw
			if(m_fV<59.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)57.90);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[13]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;		
		case 15://����16.2kw
			if(m_fV<57.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)56.30);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[14]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 16://����16.9kw
			if(m_fV<56.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)54.70);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[15]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 17://����17.6kw
			if(m_fV<54.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)53.10);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[16]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 18://����18.4kw
			if(m_fV<53.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)51.50);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[17]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 19://����17.6kw
			if(m_fV<51.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)49.90);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[18]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 20://����16.9kw
			if(m_fV<49.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)48.30);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[19]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 21://����16.2kw
			if(m_fV<48.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)46.70);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[20]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 22://����15.4kw
			if(m_fV<46.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)45.10);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[21]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 23://����14.7kw
			if(m_fV<45.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)43.40);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[22]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 24://����13.2kw
			if(m_fV<43.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)41.80);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[23]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 25://����11.8kw
			if(m_fV<41.8)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)40.2);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[24]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 26://����10.3kw
			if(m_fV<40.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)38.60);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[25]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 27://����11.0kw
			if(m_fV<38.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)37.00);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[26]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 28://����11.8kw
			if(m_fV<36.7)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)35.40);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[27]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 29://����12.5kw
			if(m_fV<35.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)33.80);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[28]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;		
		case 30://����13.2kw
			if(m_fV<33.80)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)32.20);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[29]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 31://����12.5kw
			if(m_fV<32.20)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)30.60);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[30]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 32://����11.8kw
			if(m_fV<30.60)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)29.00);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[31]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 33://����11.0kw
			if(m_fV<29.00)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)27.40);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[32]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 34://����10.3kw
			if(m_fV<27.40)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)25.70);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[33]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 35://����8.8kw
			if(m_fV<25.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)24.10);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[34]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 36://����7.4kw
			if(m_fV<24.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)22.50);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[35]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 37://����8.1kw
			if(m_fV<22.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)20.90);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[36]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 38://����8.8kw
			if(m_fV<20.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)19.30);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[37]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 39://����8.1kw
			if(m_fV<19.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)17.70);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[38]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 40://����7.4kw
			if(m_fV<17.70)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)16.10);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[39]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 41://����6.6kw
			if(m_fV<16.10)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)14.50);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[40]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 42://����5.9kw
			if(m_fV<14.50)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)12.90);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[41]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 43://����5.1kw
			if(m_fV<12.90)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)11.30);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[42]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 44://����4.4kw
			if(m_fV<11.30)
			{
				m_nStatues++;
				SetMiddleVelocityOfTimer1((float)9.70);	//�������1�м��ٶ�
				GetMiddleTimeOfTimer1(&m_nSlideTime[43]);
				m_fPowerAdd[m_nStatues] = m_fP;
			}
			break;
		case 45://����3.7kw
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
				// 80.5~8.0��ʱ��
				GetTimeOfTimer1(&m_nSlideTime[45]);
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

void CNHCVarLoadThread2::VarLoadTestStep5(void)
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

void CNHCVarLoadThread2::OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam)
{
	m_unVarLoadState = NHC_VARLOAD_STATE_FINISHED;
	// ������غɲ��Խ��
	//CalcVarLoadResult();
}

void CNHCVarLoadThread2::StopVarLoadTest(void)
{
	// ֹͣ��ʱ��
	if (NULL != m_unTimerIdOfVarLoad)
	{
		timeKillEvent(m_unTimerIdOfVarLoad);
		m_unTimerIdOfVarLoad = NULL;
	}
	// ����������Ϊ������٣�ֹͣ���
	if (0 == m_nAccType)
	{
		MotorOff();
	}
	KillTimer1();
	ControlModeOff();

	// ����״̬������δ��ʼ
	m_unTimerIdOfVarLoad = NHC_VARLOAD_STATE_NOT_START;
	// ������Խ������
	m_nStatues = 1;
	ZeroMemory(m_fPowerAdd, sizeof(m_fPowerAdd));
	ZeroMemory(m_nSlideTime, sizeof(m_nSlideTime));
}
