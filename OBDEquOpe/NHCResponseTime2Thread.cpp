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
	// TODO: �ڴ����ר�ô����/����û���

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

	return CDynThread::InitInstance();
}

int CNHCResponseTime2Thread::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���

	// �رն�ý�嶨ʱ��
	if (NULL != m_unTimerIdOfResponseTime)
	{
		timeKillEvent(m_unTimerIdOfResponseTime);
		m_unTimerIdOfResponseTime = NULL;
	}

	// �����ý���ʱ����ͷֱ���
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCResponseTime2Thread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_RESPONSE_TIME_TEST, OnStartResponseTimeTest)
END_MESSAGE_MAP()

void CNHCResponseTime2Thread::OnStartResponseTimeTest(WPARAM wParam, LPARAM lParam)
{
	// ����״̬Ϊ׼��
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
		ResponseTimeTestStep1();  // ����׼��
		break;
	case NHC_RT_STATE_LIFTBEAM_DOWN:
		ResponseTimeTestStep2();  // ��������
		break;
	case NHC_RT_STATE_ACCELERATION:
		ResponseTimeTestStep3();  // ����
		break;
	case NHC_RT_STATE_SLIP:
		ResponseTimeTestStep4();  // ����(���ؼ�ʱ�׶�)
		break;
	case NHC_RT_STATE_BRAKE_High:
		ResponseTimeTestStep5();  // ���ٵ������
		break;
	case NHC_RT_STATE_BRAKE_Low:
		ResponseTimeTestStep6();  // ���ٵ������
		break;
	case NHC_RT_STATE_STABLE:
		ResponseTimeTestStep7();  // �ȶ�
		break;
	default:
		break;
	}

}

void CNHCResponseTime2Thread::ResponseTimeTestStep1(void)
{

	// �˳����п���ģʽ
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// �⹦������ʧ��
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_COMMUNICATION_ERROR;
		return;
	}
	// ���¾�����(����)
	LiftBeamDown();
	// ��ѯ������״̬
	GetStatus();
	if (m_btStatus.test(1))
	{
		// �������Ѿ���ȫ����
		// �������������
		// ��״̬Ϊ������
		m_unResponseTimeState = NHC_RT_STATE_ACCELERATION;
		// �����������ʼ����
		MotorOn();
	}
	else
	{
		// ������û����ȫ����
		// ���뽵�¾���������
		// ����״̬Ϊ����������
		m_unResponseTimeState = NHC_RT_STATE_LIFTBEAM_DOWN;
	}
	// ������ý�嶨ʱ��
	if (NULL != m_unTimerIdOfResponseTime)
	{
		timeKillEvent(m_unTimerIdOfResponseTime);
		m_unTimerIdOfResponseTime = NULL;
	}
	m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfResponseTime)
	{
		// ������ý�嶨ʱ��ʧ��
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCResponseTime2Thread::ResponseTimeTestStep2(void)
{
	// ������������

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// �⹦������ʧ��
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}
		return;
	}
	if (m_btStatus.test(1))
	{
		// �������Ѿ�����

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// �����������ʼ����
		MotorOn();

		// ��ʼ������Ӧʱ�����ģʽ
		ResponseTimeControlModeStart();
		// �����������
		// ����״̬Ϊ������
		m_unResponseTimeState = NHC_RT_STATE_ACCELERATION;

		// ����������ʱ��
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// ������ý�嶨ʱ��ʧ��
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
		// ����ֹͣ�ٶ�

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// ֹͣ���
		MotorOff();

		// ���뻬������
		// ����״̬Ϊ������
		m_unResponseTimeState = NHC_RT_STATE_SLIP;

		// ����������ʱ��
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// ������ý�嶨ʱ��ʧ��
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
		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// ����״̬Ϊ: ���ٵ������
		m_unResponseTimeState = NHC_RT_STATE_BRAKE_High;
		// ���ٵ������
		SetConstantForceTarget(m_fLoad_High);
		ConstantForceControlModeOn();

		// ����������ʱ��
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// ������ý�嶨ʱ��ʧ��
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
		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}

		// ����״̬Ϊ: ���ٵ������
		m_unResponseTimeState = NHC_RT_STATE_BRAKE_Low;
		// ���ٵ������
		SetConstantForceTarget(m_fLoad_Low);
		ConstantForceControlModeOn();

		m_nElapsedTime = 0;

		// ����������ʱ��  ˢ�¼��
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// ������ý�嶨ʱ��ʧ��
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
		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}
		//����״̬ �ȶ�
		m_unResponseTimeState = NHC_RT_STATE_STABLE;
		// ����������ʱ��
		m_unTimerIdOfResponseTime = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfResponseTime, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfResponseTime)
		{
			// ������ý�嶨ʱ��ʧ��
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
	// ֹͣ��ʱ��
	if (NULL != m_unTimerIdOfResponseTime)
	{
		timeKillEvent(m_unTimerIdOfResponseTime);
		m_unTimerIdOfResponseTime = NULL;
	}

	m_unResponseTimeState = NHC_RT_STATE_FINISHED;

	// ����Ϣ�����̴߳���֪ͨ���м�ʱ�Ѿ����
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
		// ִ�в⹦������ʧ��
		m_unResponseTimeState = NHC_RT_STATE_ERROR;
		m_dwLastErrorCode = NHC_RT_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfResponseTime)
		{
			timeKillEvent(m_unTimerIdOfResponseTime);
			m_unTimerIdOfResponseTime = NULL;
		}
		return;
	}
	if(bIsSaveDate)
	{
		 //����Ϣ�����̴߳���֪ͨ����ʵʱ����
		if (NULL != m_hCallBackWnd)
		{
			::PostMessage(m_hCallBackWnd, WM_DYN_SAVE_RESPONSE_TIME_DATA, 0, 0);
		}
	}
}