// NHCDIWThread.cpp : ʵ���ļ�
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

int CNHCDIWThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����

	// �رն�ý�嶨ʱ��
	if (NULL != m_unTimerIdOfDIW)
	{
		timeKillEvent(m_unTimerIdOfDIW);
		m_unTimerIdOfDIW = NULL;
	}

	// �����ý���ʱ����ͷֱ���
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCDIWThread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_DIW_TEST, OnStartDIWTest)
END_MESSAGE_MAP()


// CNHCDIWThread ��Ϣ�������
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
	// ����1:׼������

	// ������Խ������
	m_unLowerSlipingTime = 0;
	m_unUpperSlipingTime = 0;
	m_fAveLowerForce = -1.0f;  // ��ʼ��Ϊ��ֵ,��ʾδ���й���¼
	m_fAveUpperForce = -1.0f;  // ��ʼ��Ϊ��ֵ,��ʾδ���й���¼
	m_dTotalForce = 0.0;
	m_unTotalTime = 0;

	// �˳����п���ģʽ
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// �⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;
		return;
	}
	// ��ʼ���⹦�����1
	KillTimer1();
	// �������1��ʼ�ٶ�
	SetUpperVelocityOfTimer1(m_fUpperVelocity);
	// �������1ֹͣ�ٶ�
	SetLowerVelocityOfTimer1(m_fLowerVelocity);
	// ���ú���Ŀ��ֵ(С������)
	SetConstantForceTarget(m_fLowerForce);
	// ���¾�����(����)
	LiftBeamDown();
	// ��ѯ������״̬
	GetStatus();
	if (m_btStatus.test(1))
	{
		// �������Ѿ���ȫ����
		// �����������
		// ��״̬Ϊ������
		m_unDIWState = NHC_DIW_STATE_ACCELERATION_1;
		// �����������ʼ����
		MotorOn();
	}
	else
	{
		// ������û����ȫ����
		// ���뽵�¾���������
		// ����״̬Ϊ����������
		m_unDIWState = NHC_DIW_STATE_LIFTBEAM_DOWN;
	}
	// ������ý�嶨ʱ��
	if (NULL != m_unTimerIdOfDIW)
	{
		timeKillEvent(m_unTimerIdOfDIW);
		m_unTimerIdOfDIW = NULL;
	}
	m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfDIW)
	{
		// ������ý�嶨ʱ��ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCDIWThread::DIWTestStep2(void)
{
	// ����2:��������

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// �⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if(m_btStatus.test(1))
	{
		// �������Ѿ�����

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// �����������ʼ����
		MotorOn();

		// �����������
		// ����״̬Ϊ������
		m_unDIWState = NHC_DIW_STATE_ACCELERATION_1;

		// ����������ʱ��
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep3(void)
{
	// ����3:����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// ����ֹͣ�ٶ�

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// �������1
		SetTimer1();
		// ֹͣ���
		MotorOff();
		// ������������ģʽ
		ConstantForceControlModeOn();

		// ���뻬������
		// ����״̬Ϊ�����У���ʱ�׶�
		m_unDIWState = NHC_DIW_STATE_SLIP_1;

		// ����������ʱ��
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep4(void)
{
	// ����4:����(С������)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// ���ڼ��ؼ�ʱ�׶�
		// ʵʱȡ���ʱ��
		GetTimeOfTimer1(&(m_unLowerSlipingTime));

		// �Ѽ�������(С������)
		if (m_fV>m_fLowerVelocity && m_fV<m_fUpperVelocity)
		{
			m_dTotalForce += m_fF;
			m_unTotalTime++;

			// ���������ƽ��ֵ(С������)
			ASSERT(0 != m_unTotalTime);
			m_fAveLowerForce = float(m_dTotalForce/m_unTotalTime);
			// ����һλС��,������������
			m_fAveLowerForce = int(m_fAveLowerForce*10.0f + 0.5f) / 10.0f;
		}
	}
	else
	{
		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		//// ���������ƽ��ֵ(С������)
		//ASSERT(0 != m_unTotalTime);
		//m_fAveLowerForce = float(m_dTotalForce/m_unTotalTime);
		//// ����һλС��,������������
		//m_fAveLowerForce = int(m_fAveLowerForce*10.0f + 0.5f) / 10.0f;

		// �����ܼ������ͼ�¼����
		m_dTotalForce = 0.0;
		m_unTotalTime = 0;
 
		// ֹͣ���1
		KillTimer1();
		// �˳����п���ģʽ
		ControlModeOff();

		// ����״̬Ϊ���ȴ���Ͳֹͣ
		m_unDIWState = NHC_DIW_STATE_WAIT_FOR_STOP;

		// ����������ʱ��
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep5(void)
{
	// ����5:�ȴ���Ͳֹͣ

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV <= 0.0f)
	{
		// ��Ͳ�Ѿ���������ٴμ��ٵİ�ȫ�ٶ�(�����ڹ�Ͳ����ʱ,�����!!!!)
		// ������һ���ٽ׶�
		// ��ʼ����һ�׶β���

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// ���ú���Ŀ��ֵ(�������)
		SetConstantForceTarget(m_fUpperForce);

		// �����������ʼ����
		MotorOn();

		// ����״̬Ϊ������
		m_unDIWState = NHC_DIW_STATE_ACCELERATION_2;

		// ����������ʱ��
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep6(void)
{
	// ����6:����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// ����ֹͣ�ٶ�

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		// �������1
		SetTimer1();
		// ֹͣ���
		MotorOff();
		// ������������ģʽ
		ConstantForceControlModeOn();

		// ���뻬������
		// ����״̬Ϊ�����У���ʱ�׶�
		m_unDIWState = NHC_DIW_STATE_SLIP_2;

		// ����������ʱ��
		m_unTimerIdOfDIW = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfDIW, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfDIW)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unDIWState = NHC_DIW_STATE_ERROR;
			m_dwLastErrorCode = NHC_DIW_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCDIWThread::DIWTestStep7(void)
{
	// ����7:����(�������)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unDIWState = NHC_DIW_STATE_ERROR;
		m_dwLastErrorCode = NHC_DIW_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}
		return;
	}
	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// ���ڼ��ؼ�ʱ�׶�
		// ʵʱȡ���ʱ��
		GetTimeOfTimer1(&(m_unUpperSlipingTime));

		// �Ѽ�������(�������)
		if (m_fV>m_fLowerVelocity && m_fV<m_fUpperVelocity)
		{
			m_dTotalForce += m_fF;
			m_unTotalTime++;

			// ���������ƽ��ֵ(С������)
			ASSERT(0 != m_unTotalTime);
			m_fAveUpperForce = float(m_dTotalForce/m_unTotalTime);
			// ����һλС��,������������
			m_fAveUpperForce = int(m_fAveUpperForce*10.0f + 0.5f) / 10.0f;
		}
	}
	else
	{
		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfDIW)
		{
			timeKillEvent(m_unTimerIdOfDIW);
			m_unTimerIdOfDIW = NULL;
		}

		//// ���������ƽ��ֵ(С������)
		//ASSERT(0 != m_unTotalTime);
		//m_fAveUpperForce = float(m_dTotalForce/m_unTotalTime);
		//// ����һλС��,������������
		//m_fAveUpperForce = int(m_fAveUpperForce*10.0f + 0.5f) / 10.0f;

		// �����ܼ������ͼ�¼����
		m_dTotalForce = 0.0;
		m_unTotalTime = 0;

		// ֹͣ���1
		KillTimer1();
		// �˳����п���ģʽ
		ControlModeOff();

		// ����״̬Ϊ���������
		m_unDIWState = NHC_DIW_STATE_FINISHED;
	}
}

void CNHCDIWThread::StopDIWTest(void)
{
	// ֹͣ��ʱ��
	if (NULL != m_unTimerIdOfDIW)
	{
		timeKillEvent(m_unTimerIdOfDIW);
		m_unTimerIdOfDIW = NULL;
	}

	MotorOff();
	KillTimer1();
	ControlModeOff();

	// ����״̬������δ��ʼ
	m_unDIWState = NHC_DIW_STATE_NOT_START;
	// ������Խ������
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