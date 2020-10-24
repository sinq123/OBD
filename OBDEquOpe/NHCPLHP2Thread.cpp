// NHCPLHPThread.cpp : ʵ���ļ�
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

int CNHCPLHP2Thread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����

	// �رն�ý�嶨ʱ��
	if (NULL != m_unTimerIdOfPLHP)
	{
		timeKillEvent(m_unTimerIdOfPLHP);
		m_unTimerIdOfPLHP = NULL;
	}

	// �����ý���ʱ����ͷֱ���
	timeEndPeriod(m_unTimerRes);

	return CDynThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNHCPLHP2Thread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_PLHP_TEST, OnStartPLHPTest)
END_MESSAGE_MAP()


// CNHCPLHP2Thread ��Ϣ�������

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
		// LUGDOWN�����ٶ�
		m_fVelocityOfMotorOff = 94.0f;
	}
	else
	{
		// ASM,VMAS�����ٶ�
		m_fVelocityOfMotorOff = 56.0f;
	}
	m_hCallBackWnd = hCallBackWnd;

	PostThreadMessage(WM_DYN_START_PLHP_TEST, 0, 0);
}

void CNHCPLHP2Thread::OnStartPLHPTest(WPARAM wParam, LPARAM lParam)
{
	// ��״̬Ϊ������׼��
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHP2Thread::StartPLHPTest(const enum PLHPType pt, const HWND hCallBackWnd)
{
	m_PLHPType = pt;
	if (PLHP_LUGDOWN == pt)
	{
		// LUGDOWN�����ٶ�
		m_fVelocityOfMotorOff = 94.0f;
	}
	else
	{
		// ASM,VMAS�����ٶ�
		m_fVelocityOfMotorOff = 56.0f;
	}
	m_hCallBackWnd = hCallBackWnd;
	// ��״̬Ϊ������׼��
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHP2Thread::StopPLHPTest(void)
{
	// ֹͣ��ʱ��
	if (NULL != m_unTimerIdOfPLHP)
	{
		timeKillEvent(m_unTimerIdOfPLHP);
		m_unTimerIdOfPLHP = NULL;
	}

	MotorOff();
	KillTimer1();
	KillTimer2();
	ControlModeOff();

	// ����״̬������δ��ʼ
	m_unPLHPState = NHC_PLHP_STATE_NOT_START;
	m_unSlipState = 0;
}

void CNHCPLHP2Thread::PLHPTest(void)
{
	switch (m_unPLHPState)
	{
	case NHC_PLHP_STATE_READY:
		PLHPTestStep1();  // ����׼��
		break;
	case NHC_PLHP_STATE_LIFTBEAM_DOWN:
		PLHPTestStep2();  // ��������
		break;
	case NHC_PLHP_STATE_ACCELERATION:
		PLHPTestStep3();  // ����
		break;
	case NHC_PLHP_STATE_SLIP:
		PLHPTestStep4();  // ����(��ʱ)
		break;
	default:
		break;
	}
}

void CNHCPLHP2Thread::PLHPTestStep1(void)
{
	// �������ʲ���-׼������

	// �����ʼ���������ʲ��Բ���

	// ����������ʻ���ʱ��
	ZeroMemory(m_unPLHPTime, sizeof(m_unPLHPTime));
	// ����ʣ�ż�¼��ֵ
	ZeroMemory(m_dRemanenceSumByPower, sizeof(m_dRemanenceSumByPower));
	ZeroMemory(m_dRemanenceSumByForce, sizeof(m_dRemanenceSumByForce));
	ZeroMemory(m_nRemanenceTimes, sizeof(m_nRemanenceTimes));
	ZeroMemory(m_fRemanenceAveByPower, sizeof(m_fRemanenceAveByPower));
	ZeroMemory(m_fRemanenceAveByForce, sizeof(m_fRemanenceAveByForce));

	// �˳����п���ģʽ
	if (DYN_WR_SUCCEED != ControlModeOff())
	{
		// �⹦������ʧ��
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;
		return;
	}
	// ��ʼ���⹦�����1
	KillTimer1();
	// ��ʼ���⹦�����2
	KillTimer2();
	// ���¾�����(����)
	LiftBeamDown();
	// ��ѯ������״̬
	GetStatus();
	if (m_btStatus.test(1))
	{
		// �������Ѿ���ȫ����
		// �������������
		// ��״̬Ϊ������
		m_unPLHPState = NHC_PLHP_STATE_ACCELERATION;
		// �����������ʼ����
		MotorOn();
	}
	else
	{
		// ������û����ȫ����
		// ���뽵�¾���������
		// ����״̬Ϊ����������
		m_unPLHPState = NHC_PLHP_STATE_LIFTBEAM_DOWN;
	}
	// ������ý�嶨ʱ��
	if (NULL != m_unTimerIdOfPLHP)
	{
		timeKillEvent(m_unTimerIdOfPLHP);
		m_unTimerIdOfPLHP = NULL;
	}
	m_unTimerIdOfPLHP = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfPLHP, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerIdOfPLHP)
	{
		// ������ý�嶨ʱ��ʧ��
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_START_MMTIMER_FAIL;
		return;
	}
}

void CNHCPLHP2Thread::PLHPTestStep2(void)
{
	// �������ʲ���-������������

	if (DYN_WR_SUCCEED != GetStatus())
	{
		// �⹦������ʧ��
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}
		return;
	}
	if(m_btStatus.test(1))
	{
		// �������Ѿ�����

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		// ���������Ϊ200ms
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}

		// �����������ʼ����
		MotorOn();

		// �����������
		// ����״̬Ϊ������
		m_unPLHPState = NHC_PLHP_STATE_ACCELERATION;

		// ����������ʱ��
		m_unTimerIdOfPLHP = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfPLHP, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfPLHP)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unPLHPState = NHC_PLHP_STATE_ERROR;
			m_unSlipState = 0;
			m_dwLastErrorCode = NHC_PLHP_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCPLHP2Thread::PLHPTestStep3(void)
{
	// �������ʲ���-����

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��

		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}
		return;
	}

	if (m_fV > m_fVelocityOfMotorOff)
	{
		// ����ֹͣ�ٶ�

		// ��ͣ��ʱ����������һ����״̬������Ȼ����������
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}

		// ���¹��ܱ����Σ���Ϊ����һ״̬�ص��
		//// ֹͣ���
		//MotorOff();

		// ���뻬������
		// ����״̬Ϊ�����У���ʱ
		m_unPLHPState = NHC_PLHP_STATE_SLIP;
		
		// ���û���״̬��δ��ʼ
		// ��������
		m_unSlipState = 0;
		// Ԥִ����һ����״̬�ĵ�һ�����в������(m_unSlipState=0)
		// ��Ϊm_unSlipState=0ʱ,ASM��VMAS����9���⹦������,LugDown��6��,���ڶ�ʱ��û������������������в����ȽϺ���
		PLHPTestStep4();

		// ����������ʱ��
		m_unTimerIdOfPLHP = timeSetEvent(m_unTestInterval, m_unTimerRes, (LPTIMECALLBACK)TimeProOfPLHP, (DWORD_PTR)this, TIME_PERIODIC);
		if (NULL == m_unTimerIdOfPLHP)
		{
			// ������ý�嶨ʱ��ʧ��
			m_unPLHPState = NHC_PLHP_STATE_ERROR;
			m_unSlipState = 0;
			m_dwLastErrorCode = NHC_PLHP_START_MMTIMER_FAIL;
			return;
		}
	}
}

void CNHCPLHP2Thread::PLHPTestStep4(void)
{
	//�������ʲ���-����(��ʱ)

	if (DYN_WR_SUCCEED != GetRealTimeData())
	{
		// ִ�в⹦������ʧ��
		m_unPLHPState = NHC_PLHP_STATE_ERROR;
		m_unSlipState = 0;
		m_dwLastErrorCode = NHC_PLHP_COMMUNICATION_ERROR;

		// ֹͣ��ʱ��
		if (NULL != m_unTimerIdOfPLHP)
		{
			timeKillEvent(m_unTimerIdOfPLHP);
			m_unTimerIdOfPLHP = NULL;
		}
		return;
	}

	if (PLHP_ASM == m_PLHPType)
	{
		// ��¼ʣ��
		RemanenceRecord_ASM();
		// ASM��������
		PLHPTestStep4_ASM();

	}
	else if (PLHP_VMAS == m_PLHPType)
	{
		// ��¼ʣ��
		RemanenceRecord_VMAS();
		// VMAS��������
		PLHPTestStep4_VMAS();
	}
	else
	{
		// ��¼ʣ��
		RemanenceRecord_LUGDOWN();
		// LUGDOWN��������
		PLHPTestStep4_LUGDOWN();
	}
}

//void CNHCPLHP2Thread::PLHPTestStep5(void)
//{
//	// �������ʲ���-����(�ȴ���Ͳͣ����)
//
//	if (DYN_WR_SUCCEED != GetRealTimeData())
//	{
//		// ִ�в⹦������ʧ��
//
//		m_unPLHPState = 7;
//		m_unSlipState = 0;
//		m_dwLastErrorCode = 0x01;
//
//		// ֹͣ��ʱ��
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
//		// ��Ͳ�Ѿ�ͣ����
//		// ��������
//
//		// ֹͣ��ʱ��
//		if (NULL != m_unTimerIdOfPLHP)
//		{
//			timeKillEvent(m_unTimerIdOfPLHP);
//			m_unTimerIdOfPLHP = NULL;
//		}
//
//		// ����״̬Ϊ������ȫ������
//		m_unPLHPState = 6;
//	}
//}

void CNHCPLHP2Thread::PLHPTestStep4_ASM(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// ׼������

			// �������2(54~46)
			SetUpperVelocityOfTimer2(54);
			SetLowerVelocityOfTimer2(46);
			SetTimer2();
			// �������1(48~33~17)
			SetUpperVelocityOfTimer1(48);
			SetMiddleVelocityOfTimer1(33);
			SetLowerVelocityOfTimer1(17);
			SetTimer1();
			// �ص��
			MotorOff();

			// ������һ����״̬(51~48)
			m_unSlipState = 1;

			break;
		}
	case 1:
		{
			// 51~48

			// ��ȡ��һ�λ���ʱ��(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);

			if (m_fV < 48.0f)
			{
				// ������һ����״̬(48~45)
				m_unSlipState = 2;
			}
			break;
		}
	case 2:
		{
			// 48~45

			// ��ȡ��һ�λ���ʱ��(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);
			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 46.0f)
			{
				// ��1�α��
				const UINT unSection(0);
				// �����1��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ��һ���Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, WPARAM(unSection), 0);
				}
				// �����������2ʱ���(40~24),����������
				KillTimer2();
				SetUpperVelocityOfTimer2(40);
				SetLowerVelocityOfTimer2(24);
				SetTimer2();

				// ������һ����״̬(45~40)
				m_unSlipState = 3;
			}
			break;
		}
	case 3:
		{
			// 45~40

			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 40.0f)
			{
				// ������һ����״̬(40~33)
				m_unSlipState = 4;
			}
			break;
		}
	case 4:
		{
			// 40~33

			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);
			// ��ȡ�����λ���ʱ��(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);

			if (m_fV < 33.0f)
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// �ݴ�48~33����ʱ��,Ϊ��������Ķλ���ʱ��(33~17)
				GetMiddleTimeOfTimer1(&m_unMiddleTimeOld);
				// ��������Timer1�м��ٶ�Ϊ32
				SetMiddleVelocityOfTimer1(32);

				// ������һ����״̬(33~32)
				m_unSlipState = 5;
			}
			break;
		}
	case 5:
		{
			// 33~32

			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);
			// ��ȡ�����λ���ʱ��(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);
			// ��ȡ���Ķλ���ʱ��(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;

			if (m_fV < 32.0f)
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);

				// ��2�α��
				const UINT unSection(1);
				// �����2��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�ڶ����Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ����״̬(32~24)
				m_unSlipState = 6;
			}
			break;
		}
	case 6:
		{
			// 32~24

			// ��ȡ�����λ���ʱ��(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);
			// ��ȡ���Ķλ���ʱ��(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;

			if (m_fV < 24.0f)
			{
				// ��3�α��
				const UINT unSection(2);
				// �����3��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;

				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				// �����������2ʱ���(19~13),����������
				KillTimer2();
				SetUpperVelocityOfTimer2(19);
				SetLowerVelocityOfTimer2(13);
				SetTimer2();

				// ������һ����״̬(24~19)
				m_unSlipState = 7;
			}
			break;
		}
	case 7:
		{
			// 24~19

			// ��ȡ���Ķλ���ʱ��(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;

			if (m_fV < 19.0f)
			{
				// ������һ����״̬(19~17)
				m_unSlipState = 8;
			}
			break;
		}
	case 8:
		{
			// 19~17

			// ��ȡ���Ķλ���ʱ��(33~17)
			GetTimeOfTimer1(&m_unPLHPTime[3]);
			m_unPLHPTime[3]  -= m_unMiddleTimeOld;
			// ��ȡ����λ���ʱ��(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 17.0f)
			{
				// ��4�α��
				const UINT unSection(3);
				// �����4��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ���Ķ��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ���1ʹ����ϣ�ֹͣ
				KillTimer1();

				// ������һ����״̬(17~13)
				m_unSlipState = 9;
			}
			break;
		}
	case 9:
		{
			// 17~13

			// ��ȡ����λ���ʱ��(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 13.0f)
			{
				// ��5�α��
				const UINT unSection(4);
				// �����5��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ���2ʹ����ϣ�ֹͣ
				KillTimer2();

				// ֹͣ��ʱ��
				if (NULL != m_unTimerIdOfPLHP)
				{
					timeKillEvent(m_unTimerIdOfPLHP);
					m_unTimerIdOfPLHP = NULL;
				}
				
				// ����״̬���
				// ����״̬Ϊ���������
				m_unPLHPState = NHC_PLHP_STATE_FINISHED;

				// �������
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�Ѿ����
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
			// ׼������

			// �������2(51~45)
			SetUpperVelocityOfTimer2(54);
			SetLowerVelocityOfTimer2(46);
			SetTimer2();
			// �������1(48~33~16)
			SetUpperVelocityOfTimer1(48);
			SetMiddleVelocityOfTimer1(32);
			SetLowerVelocityOfTimer1(16);
			SetTimer1();
			// �ص��
			MotorOff();
			
			// ������һ����״̬(51~48)
			m_unSlipState = 1;

			break;
		}
	case 1:
		{
			// 51~48

			//��ȡ��һ�λ���ʱ��(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);

			if (m_fV < 48.0f)
			{
				// ������һ����״̬(48~45)
				m_unSlipState = 2;
			}
			break;
		}
	case 2:
		{
			// 48~45

			//��ȡ��һ�λ���ʱ��(51~45)
			GetTimeOfTimer2(&m_unPLHPTime[0]);
			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 46.0f)
			{
				// ��1�α��
				const UINT unSection(0);
				// �����1��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ��һ���Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// �����������2ʱ���(40~24),����������
				KillTimer2();
				SetUpperVelocityOfTimer2(40);
				SetLowerVelocityOfTimer2(24);
				SetTimer2();

				// ������һ����״̬(45~40)
				m_unSlipState = 3;
			}
			break;
		}
	case 3:
		{
			// 45~40

			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);

			if (m_fV < 40.0f)
			{
				// ������һ����״̬(40~32)
				m_unSlipState = 4;
			}
			break;
		}
	case 4:
		{
			// 40~32

			// ��ȡ�ڶ��λ���ʱ��(48~32)
			//GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);
			GetTimeOfTimer1(&m_unPLHPTime[1]);
			// ��ȡ�����λ���ʱ��(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);

			if (m_fV < 32.0f)
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ�ڶ��λ���ʱ��(48~32)
				GetMiddleTimeOfTimer1(&m_unPLHPTime[1]);

				// ��2�α��
				const UINT unSection(1);
				// �����2��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�ڶ����Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ����״̬(32~24)
				m_unSlipState = 5;
			}
			break;
		}
	case 5:
		{
			// 32~24

			// ��ȡ�����λ���ʱ��(40~24)
			GetTimeOfTimer2(&m_unPLHPTime[2]);
			// ��ȡ�Ķλ���ʱ��(32~16)
			GetTimeOfTimer1(&m_unMiddleTimeOld);
			m_unPLHPTime[3] = m_unMiddleTimeOld - m_unPLHPTime[1];

			if (m_fV < 24.0f)
			{
				// ��3�α��
				const UINT unSection(2);
				// �����3��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// �����������2ʱ���(19~13),����������
				KillTimer2();
				SetUpperVelocityOfTimer2(19);
				SetLowerVelocityOfTimer2(13);
				SetTimer2();

				// ������һ����״̬(24~19)
				m_unSlipState = 6;
			}
			break;
		}
	case 6:
		{
			// 24~19

			// ��ȡ�Ķλ���ʱ��(32~16)
			GetTimeOfTimer1(&m_unMiddleTimeOld);
			m_unPLHPTime[3] = m_unMiddleTimeOld - m_unPLHPTime[1];

			if (m_fV < 19.0f)
			{
				// ������һ����״̬(19~16)
				m_unSlipState = 7;
			}
			break;
		}
	case 7:
		{
			//  19~16

			// ��ȡ�Ķλ���ʱ��(32~16)
			GetTimeOfTimer1(&m_unMiddleTimeOld);
			m_unPLHPTime[3] = m_unMiddleTimeOld - m_unPLHPTime[1];
			// ��ȡ����λ���ʱ��(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 16.0f)
			{
				// ��4�α��
				const UINT unSection(3);
				// �����4��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ���Ķ��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ���1ʹ����ϣ�ֹͣ
				KillTimer1();

				// ������һ����״̬(16~13)
				m_unSlipState = 8;
			}
			break;
		}
	case 8:
		{
			// 16~13

			// ��ȡ����λ���ʱ��(19~13)
			GetTimeOfTimer2(&m_unPLHPTime[4]);

			if (m_fV < 13.0f)
			{
				// ��5�α��
				const UINT unSection(4);
				// �����5��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ���2ʹ����ϣ�ֹͣ
				KillTimer2();

				// ֹͣ��ʱ��
				if (NULL != m_unTimerIdOfPLHP)
				{
					timeKillEvent(m_unTimerIdOfPLHP);
					m_unTimerIdOfPLHP = NULL;
				}

				// ����״̬���
				// ����״̬Ϊ���������
				m_unPLHPState = NHC_PLHP_STATE_FINISHED;

				// �������
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�Ѿ����
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
			// �������1(92~84)
			SetUpperVelocityOfTimer1(92);
			//SetMiddleVelocityOfTimer1(84);
			SetLowerVelocityOfTimer1(84);
			SetTimer1();

			// �������2(84~76)
			SetUpperVelocityOfTimer2(84);
			SetLowerVelocityOfTimer2(76);
			SetTimer2();

			// �ص��
			MotorOff();
			// ������һ����״̬
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 92~84

			if (m_fV >= 84.0f)
			{
				// ��ȡ��һ�λ���ʱ��(92~84)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ��һ�λ���ʱ��(92~84)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// �ݴ滬��ʱ��(92~84)
				m_unMiddleTimeOld = m_unPLHPTime[0];

				// ��1�α��
				const UINT unSection(0);
				// �����1��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ��һ���Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}


				//���õ����׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(76);
				SetLowerVelocityOfTimer1(68);
				SetTimer1();

				// ������һ����״̬(84~76)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 84~76

			if (m_fV >= 76.0f)
			{
				// ��ȡ�ڶ��λ���ʱ��
				//GetTimeOfTimer1(&m_unMiddleTimeNew);
				//m_unPLHPTime[1] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				GetTimeOfTimer2(&m_unPLHPTime[1]);
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ�ڶ��λ���ʱ��(84~76)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				//m_unPLHPTime[1] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~76)
				m_unMiddleTimeOld += m_unPLHPTime[1];

				// ��2�α��
				const UINT unSection(1);
				// �����2��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�ڶ����Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ�����1�м��ٶ�(68)
				//SetMiddleVelocityOfTimer1(68);

				//���õ��Ľ׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(68);
				SetLowerVelocityOfTimer2(60);
				SetTimer2();

				// ������һ����״̬(76~68)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 76~68

			if (m_fV >= 68.0f)
			{
				// ��ȡ�����λ���ʱ��(76~68)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				//m_unPLHPTime[2] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ�����λ���ʱ��(76~68)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				//m_unPLHPTime[2] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~68)
				m_unMiddleTimeOld += m_unPLHPTime[2];

				// ��3�α��
				const UINT unSection(2);
				// �����3��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ�����1�м��ٶ�(60)
				//SetMiddleVelocityOfTimer1(60);

				//���õ���׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(60);
				SetLowerVelocityOfTimer1(52);
				SetTimer1();

				// ������һ����״̬(68~60)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 68~60

			if (m_fV >= 60.0f)
			{
				// ��ȡ�Ķλ���ʱ��(68~60)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
				//m_unPLHPTime[3] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ���Ķλ���ʱ��(68~60)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
				//m_unPLHPTime[3] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~60)
				m_unMiddleTimeOld += m_unPLHPTime[3];

				// ��4�α��
				const UINT unSection(3);
				// �����4��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ���Ķ��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ�����1�м��ٶ�(52)
				//SetMiddleVelocityOfTimer1(52);

				//���õ����׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(52);
				SetLowerVelocityOfTimer2(44);
				SetTimer2();

				// ������һ����״̬(60~52)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 60~52

			if (m_fV >= 52.0f)
			{
				// ��ȡ����λ���ʱ��(60~52)
				GetTimeOfTimer1(&m_unPLHPTime[4]);
				//m_unPLHPTime[4] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ����λ���ʱ��(60~52)
				GetTimeOfTimer1(&m_unPLHPTime[4]);
				//m_unPLHPTime[4] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~52)
				m_unMiddleTimeOld += m_unPLHPTime[4];

				// ��5�α��
				const UINT unSection(4);
				// �����5��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ�����1�м��ٶ�(44)
				//SetMiddleVelocityOfTimer1(44);

				//���õ��߽׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(44);
				SetLowerVelocityOfTimer1(36);
				SetTimer1();


				// ������һ����״̬(52~44)
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 52~44

			if (m_fV >= 44.0f)
			{
				// ��ȡ�����λ���ʱ��(52~44)
				GetTimeOfTimer2(&m_unPLHPTime[5]);
				//m_unPLHPTime[5] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ�����λ���ʱ��(52~44)
				GetTimeOfTimer2(&m_unPLHPTime[5]);
				//m_unPLHPTime[5] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~44)
				m_unMiddleTimeOld += m_unPLHPTime[5];

				// ��6�α��
				const UINT unSection(5);
				// �����6��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ�����1�м��ٶ�(36)
				//SetMiddleVelocityOfTimer1(36);

				//���õڰ˽׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(36);
				SetLowerVelocityOfTimer2(28);
				SetTimer2();

				// ������һ����״̬(44~36)
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 44~36

			if (m_fV >= 36.0f)
			{
				// ��ȡ���߶λ���ʱ��(44~36)
				GetTimeOfTimer1(&m_unPLHPTime[6]);
				//m_unPLHPTime[6] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ���߶λ���ʱ��(44~36)
				GetTimeOfTimer1(&m_unPLHPTime[6]);
				//m_unPLHPTime[6] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~36)
				m_unMiddleTimeOld += m_unPLHPTime[6];

				// ��7�α��
				const UINT unSection(6);
				// �����7��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ���߶��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ������һ�����1�м��ٶ�(28)
				//SetMiddleVelocityOfTimer1(28);

				//���õھŽ׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(28);
				SetLowerVelocityOfTimer1(20);
				SetTimer1();

				// ������һ����״̬(36~28)
				m_unSlipState = 8;
			}
		}
		break;
	case 8:
		{
			// 36~28

			if (m_fV >= 28.0f)
			{
				// ��ȡ�ڰ˶λ���ʱ��(36~28)
				GetTimeOfTimer2(&m_unPLHPTime[7]);
				//m_unPLHPTime[7] = m_unMiddleTimeNew - m_unMiddleTimeOld;
			}
			else
			{
				// ��ȡ���յ��м�ʱ��,��Ϊ�м�ʱ�䲻��ʵʱ����
				// ��ȡ�ڰ˶λ���ʱ��(36~28)
				GetTimeOfTimer2(&m_unPLHPTime[7]);
				//m_unPLHPTime[7] = m_unMiddleTimeNew - m_unMiddleTimeOld;
				// �ݴ滬��ʱ��(92~28)
				m_unMiddleTimeOld += m_unPLHPTime[7];

				// ��8�α��
				const UINT unSection(7);
				// �����8��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�ڰ˶��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				// ���2ʹ����ɣ��ر�
				KillTimer2();

				// ������һ����״̬(28~20)
				m_unSlipState = 9;
			}
		}
		break;
	case 9:
		{
			// 28~20

			// ��ȡ�ھŶλ���ʱ��(28~20)
			// ���һ��
			GetTimeOfTimer1(&m_unPLHPTime[8]);
			//m_unPLHPTime[8] = m_unMiddleTimeNew - m_unMiddleTimeOld;

			if (m_fV < 20.0f)
			{
				// ��9�α��
				const UINT unSection(8);
				// �����9��ʣ��ƽ��ֵ,��ʽ��Ϊ2λС��������������
				// ����ģʽ
				m_fRemanenceAveByPower[unSection] = (float)(m_dRemanenceSumByPower[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByPower[unSection] >= 0.0f);
				m_fRemanenceAveByPower[unSection] = (int)(m_fRemanenceAveByPower[unSection]*100.0f+0.5f) / 100.0f;
				// ��ģʽ
				m_fRemanenceAveByForce[unSection] = (float)(m_dRemanenceSumByForce[unSection]/m_nRemanenceTimes[unSection]);
				ASSERT(m_fRemanenceAveByForce[unSection] >= 0.0f);
				m_fRemanenceAveByForce[unSection] = (int)(m_fRemanenceAveByForce[unSection]*100.0f+0.5f) / 100.0f;
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�ھŶ��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				
				// ֹͣ��ʱ��
				if (NULL != m_unTimerIdOfPLHP)
				{
					timeKillEvent(m_unTimerIdOfPLHP);
					m_unTimerIdOfPLHP = NULL;
				}

				// ����
				m_unMiddleTimeOld = m_unMiddleTimeNew = 0;
				// ���1ʹ����ϣ�ֹͣ
				KillTimer1();

				// ����״̬���
				// ����״̬Ϊ���������
				m_unPLHPState = NHC_PLHP_STATE_FINISHED;

				// �������
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�Ѿ����
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
	// ASMʣ�ż�¼

	//if (m_fV<=51.0f && m_fV>=48.0f)
	if(m_fV > 54.0f)
	{
		// δ������Ч���䷶Χ������¼ʣ��
	}
	else if (m_fV>=48.0f)
	{
		// ��1��(54~46)
		RemanenceRecordOfSection1();
	}
	else if (m_fV>=46.0f)
	{
		// ��1��(54~46)
		RemanenceRecordOfSection1();
		// ��2��(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=40.0f)
	{
		// ��2��(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=33.0f)
	{
		// ��2��(48~32)
		RemanenceRecordOfSection2();
		// ��3��(40~24)
		RemanenceRecordOfSection3();
	}
	else if (m_fV>=32.0f)
	{
		// ��2��(48~32)
		RemanenceRecordOfSection2();
		// ��3��(40~24)
		RemanenceRecordOfSection3();
		// ��4��(33~17)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=24.0f)
	{
		// ��3��(40~24)
		RemanenceRecordOfSection3();
		// ��4��(33~17)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=19.0f)
	{
		// ��4��(33~17)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=17.0f)
	{
		// ��4��(33~17)
		RemanenceRecordOfSection4();
		// ��5��(19~13)
		RemanenceRecordOfSection5();
	}
	else if (m_fV>= 13.0f)
	{
		// ��5��(19~13)
		RemanenceRecordOfSection5();
	}
}

void CNHCPLHP2Thread::RemanenceRecord_VMAS(void)
{
	// VMASʣ�ż�¼

	//if (m_fV<=51.0f && m_fV>=48.0f)
	if (m_fV > 54.0f)
	{
		// δ������Ч���䷶Χ������¼ʣ��
	}
	else if (m_fV>=48.0f)
	{
		// ��1��(51~45)
		RemanenceRecordOfSection1();
	}
	else if (m_fV>=46.0f)
	{
		// ��1��(51~45)
		RemanenceRecordOfSection1();
		// ��2��(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=40.0f)
	{
		// ��2��(48~32)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=32.0f)
	{
		// ��2��(48~32)
		RemanenceRecordOfSection2();
		// ��3��(40~24)
		RemanenceRecordOfSection3();
	}
	else if (m_fV>=24.0f)
	{
		// ��3��(40~24)
		RemanenceRecordOfSection3();
		// ��4��(32~16)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=19.0f)
	{
		// ��4��(32~16)
		RemanenceRecordOfSection4();
	}
	else if(m_fV>=16.0f)
	{
		// ��4��(32~16)
		RemanenceRecordOfSection4();
		// ��5��(19~13)
		RemanenceRecordOfSection5();
	}
	else if (m_fV>=13.0f)
	{
		// ��5��(19~13)
		RemanenceRecordOfSection5();
	}
}

void CNHCPLHP2Thread::RemanenceRecord_LUGDOWN(void)
{
	// LUGDOWNʣ�ż�¼

	//if (m_fV<=92.0f && m_fV>=84.0f)
	if (m_fV > 92.0f)
	{
		// δ������Ч���䷶Χ������¼ʣ��
	}
	else if (m_fV>=84.0f)
	{
		// ��1��(92~84)
		RemanenceRecordOfSection1();
	}
	else if (m_fV>=76.0f)
	{
		// ��2��(84~76)
		RemanenceRecordOfSection2();
	}
	else if (m_fV>=68.0f)
	{
		// ��3��(76~68)
		RemanenceRecordOfSection3();
	}
	else if (m_fV>=60.0f)
	{
		// ��4��(68~60)
		RemanenceRecordOfSection4();
	}
	else if (m_fV>=52.0f)
	{
		// ��5��(60~52)
		RemanenceRecordOfSection5();
	}
	else if (m_fV>=44.0f)
	{
		// ��6��(52~44)
		RemanenceRecordOfSection6();
	}
	else if (m_fV>=36.0f)
	{
		// ��7��(44~36)
		RemanenceRecordOfSection7();
	}
	else if (m_fV>=28.0f)
	{
		// ��8��(36~28)
		RemanenceRecordOfSection8();
	}
	else if (m_fV>=20.0f)
	{
		// ��9��(28~20)
		RemanenceRecordOfSection9();
	}
}

void CNHCPLHP2Thread::RemanenceRecordOfSection1(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[0] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[0] += m_fF;
	m_nRemanenceTimes[0]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection2(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[1] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[1] += m_fF;
	m_nRemanenceTimes[1]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection3(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[2] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[2] += m_fF;
	m_nRemanenceTimes[2]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection4(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[3] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[3] += m_fF;
	m_nRemanenceTimes[3]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection5(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[4] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[4] += m_fF;
	m_nRemanenceTimes[4]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection6(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[5] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[5] += m_fF;
	m_nRemanenceTimes[5]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection7(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[6] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[6] += m_fF;
	m_nRemanenceTimes[6]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection8(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[7] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[7] += m_fF;
	m_nRemanenceTimes[7]++;
}

void CNHCPLHP2Thread::RemanenceRecordOfSection9(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[8] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[8] += m_fF;
	m_nRemanenceTimes[8]++;
}
