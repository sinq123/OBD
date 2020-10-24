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

int CNHCConstLoadThread::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���

	// �رն�ý�嶨ʱ��
	if (NULL != m_unTimerIdOfConstLoad)
	{
		timeKillEvent(m_unTimerIdOfConstLoad);
		m_unTimerIdOfConstLoad = NULL;
	}

	// �����ý���ʱ����ͷֱ���
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
	// ��״̬Ϊ������׼��
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

	// ��״̬Ϊ������׼��
	m_unConstLoadState = NHC_CL_STATE_READY;
	ConstLoadTest();
}

void CNHCConstLoadThread::ConstLoadTest(void)
{
	switch (m_unConstLoadState)
	{
	case NHC_CL_STATE_READY:
		ConstLoadTestStep1();  // ����׼��
		break;
	case NHC_CL_STATE_LIFTBEAM_DOWN:
		ConstLoadTestStep2();  // ��������
		break;
	case NHC_CL_STATE_ACCELERATION:
		ConstLoadTestStep3();  // ����
		break;
	case NHC_CL_STATE_SLIP:
		ConstLoadTestStep4();  // ����(���ؼ�ʱ�׶�)
		break;
	default:
		break;
	}
}

void CNHCConstLoadThread::ConstLoadTestStep1(void)
{
	// ���غɲ���-׼������

	// �����ʼ�����غɲ��Բ���

	m_fUpperVelocityWithOffset = m_fUpperVelocity + m_nUpperVelocityOffset;
	m_fLowerVelocityWithOffset = m_fLowerVelocity - m_nLowerVelocityOffset;
	ASSERT(m_fUpperVelocityWithOffset > m_fLowerVelocityWithOffset);
	if (m_fLowerVelocityWithOffset < 10e-6f)
	{
		m_fLowerVelocityWithOffset = 0.0f;
	}

	// ���趨�غɻ���ʱ��
	m_unConstLoadTime = 0;

	// �˳����п���ģʽ
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// �⹦������ʧ��
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;
		return;
	}
	// ��ʼ���⹦�����1
	KillTimer1();
	// �������1��ʼ�ٶ�
	SetUpperVelocityOfTimer1(m_fUpperVelocity);
	// �������1ֹͣ�ٶ�
	SetLowerVelocityOfTimer1(m_fLowerVelocity);
	// ���ú㹦��Ŀ��ֵ
	SetConstantPowerTarget(m_fLoadPower);
	// ���¾�����(����)
	LiftBeamDown();
	// ��ѯ������״̬
	GetStatus();
	if (m_btStatus.test(1))
	{
		// �������Ѿ���ȫ����
		// �������������
		// ��״̬Ϊ������
		m_unConstLoadState = NHC_CL_STATE_ACCELERATION;
		// �����������ʼ����
		MotorOn();
	}
	else
	{
		// ������û����ȫ����
		// ���뽵�¾���������
		// ����״̬Ϊ����������
		m_unConstLoadState = NHC_CL_STATE_LIFTBEAM_DOWN;
	}
	// ������ý�嶨ʱ��
	if (NULL != m_unTimerIdOfConstLoad)
	{
		timeKillEvent(m_unTimerIdOfConstLoad);
		m_unTimerIdOfConstLoad = NULL;
	}
	m_unTimerIdOfConstLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfConstLoad, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfConstLoad)
	{
		// ������ý�嶨ʱ��ʧ��
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCConstLoadThread::ConstLoadTestStep2(void)
{
	// ���غɲ���-������������

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// �⹦������ʧ��
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}
		return;
	}
	if (m_btStatus.test(1))
	{
		// �������Ѿ�����

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}

		// �����������ʼ����
		MotorOn();

		// �����������
		// ����״̬Ϊ������
		m_unConstLoadState = NHC_CL_STATE_ACCELERATION;

		// ����������ʱ��
		m_unTimerIdOfConstLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfConstLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfConstLoad)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unConstLoadState = NHC_CL_STATE_ERROR;
			m_dwLastErrorCode = NHC_CL_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCConstLoadThread::ConstLoadTestStep3(void)
{
	// ���غɲ���-����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}
		return;
	}

	if (m_fV > m_fUpperVelocityWithOffset)
	{
		// ����ֹͣ�ٶ�

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}

		// ֹͣ���
		MotorOff();
		// �������1
		SetTimer1();
		// �����㹦�ʿ���ģʽ
		ConstantPowerControlModeOn();

		// ���뻬������
		// ����״̬Ϊ�����У����ؼ�ʱ�׶�
		m_unConstLoadState = NHC_CL_STATE_SLIP;

		// ����������ʱ��
		m_unTimerIdOfConstLoad = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfConstLoad, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfConstLoad)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unConstLoadState = NHC_CL_STATE_ERROR;
			m_dwLastErrorCode = NHC_CL_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCConstLoadThread::ConstLoadTestStep4(void)
{
	// ���غɲ���-����(���ؼ�ʱ)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unConstLoadState = NHC_CL_STATE_ERROR;
		m_dwLastErrorCode = NHC_CL_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}
		return;
	}

	if (m_fV > m_fLowerVelocityWithOffset)
	{
		// ���ڼ��ؼ�ʱ�׶�
		// ʵʱȡ���ʱ��
		GetTimeOfTimer1(&(m_unConstLoadTime));
	}
	else if (m_fV < 5)
	{
		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfConstLoad)
		{
			timeKillEvent(m_unTimerIdOfConstLoad);
			m_unTimerIdOfConstLoad = NULL;
		}

		// ֹͣ���1
		KillTimer1();
		// �˳����п���ģʽ
		ControlModeOff();

		// ����״̬Ϊ���������
		m_unConstLoadState = NHC_CL_STATE_FINISHED;

		// �������
		// ����Ϣ�����̴߳���֪ͨ���м�ʱ�Ѿ����
		if (NULL != m_hCallBackWnd)
		{
			::PostMessage(m_hCallBackWnd, WM_DYN_CONST_LOAD_TEST_FINISHED, 0, 0);
			m_hCallBackWnd = NULL;
		}
	}
}

void CNHCConstLoadThread::StopConstLoadTest(void)
{
	// ֹͣ��ʱ��
	if (NULL != m_unTimerIdOfConstLoad)
	{
		timeKillEvent(m_unTimerIdOfConstLoad);
		m_unTimerIdOfConstLoad = NULL;
	}

	MotorOff();
	KillTimer1();
	ControlModeOff();

	// ����״̬������δ��ʼ
	m_unConstLoadState = NHC_CL_STATE_NOT_START;
}

void CNHCConstLoadThread::TimeProOfConstLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNHCConstLoadThread* pThread = reinterpret_cast<CNHCConstLoadThread*>(dwUser);
	pThread->ConstLoadTest();
	//TRACE("ConstLoadTest is executed\n\n");
}
