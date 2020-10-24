// NHCPLHPThread.cpp : ʵ���ļ�
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

int CNHCPLHPGB2018Thread::ExitInstance()
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

BEGIN_MESSAGE_MAP(CNHCPLHPGB2018Thread, CDynThread)
	ON_THREAD_MESSAGE(WM_DYN_START_PLHP_TEST, OnStartPLHPTest)
END_MESSAGE_MAP()


// CNHCPLHPGB2018Thread ��Ϣ�������

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
		// LUGDOWN�����ٶ�
		m_fVelocityOfMotorOff = 82.0f;
	}
	else if(PLHP_LUGDOWN_100 == pt)
	{
		// LUGDOWN�����ٶ�
		m_fVelocityOfMotorOff = 102.0f;
	
	}
	else
	{
		// ASM,VMAS�����ٶ�
		m_fVelocityOfMotorOff = 62.0f;
	}
	m_hCallBackWnd = hCallBackWnd;

	PostThreadMessage(WM_DYN_START_PLHP_TEST, 0, 0);
}

void CNHCPLHPGB2018Thread::OnStartPLHPTest(WPARAM wParam, LPARAM lParam)
{
	// ��״̬Ϊ������׼��
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHPGB2018Thread::StartPLHPTest(const enum PLHPType pt, const HWND hCallBackWnd)
{
	m_PLHPType = pt;
	if (PLHP_LUGDOWN == pt)
	{
		// LUGDOWN�����ٶ�
		m_fVelocityOfMotorOff = 82.0f;
	}
	else if(PLHP_LUGDOWN_100 == pt)
	{
		// LUGDOWN�����ٶ�
		m_fVelocityOfMotorOff = 102.0f;	
	}
	else
	{
		// ASM,VMAS�����ٶ�
		m_fVelocityOfMotorOff = 62.0f;
	}
	m_hCallBackWnd = hCallBackWnd;
	// ��״̬Ϊ������׼��
	m_unPLHPState = NHC_PLHP_STATE_READY;
	m_unSlipState = 0;
	PLHPTest();
}

void CNHCPLHPGB2018Thread::StopPLHPTest(void)
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

void CNHCPLHPGB2018Thread::PLHPTest(void)
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

void CNHCPLHPGB2018Thread::PLHPTestStep1(void)
{
	// �������ʲ���-׼������

	// �����ʼ���������ʲ��Բ���

	// ����������ʻ���ʱ��
	ZeroMemory(m_unPLHPTime, sizeof(m_unPLHPTime));
	ZeroMemory(m_unPLHPTime10, sizeof(m_unPLHPTime10));
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

void CNHCPLHPGB2018Thread::PLHPTestStep2(void)
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

void CNHCPLHPGB2018Thread::PLHPTestStep3(void)
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

void CNHCPLHPGB2018Thread::PLHPTestStep4(void)
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
	else if (PLHP_LUGDOWN == m_PLHPType)
	{
		// ��¼ʣ��
		RemanenceRecord_LUGDOWN();
		// LUGDOWN��������
		PLHPTestStep4_LUGDOWN();
	}
	else 
	{
		// ��¼ʣ��
		RemanenceRecord_LUGDOWN_100();
		// LUGDOWN��������
		PLHPTestStep4_LUGDOWN_100();
	}
}

void CNHCPLHPGB2018Thread::PLHPTestStep4_ASM(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// �������1(60~40)
			SetUpperVelocityOfTimer1(60);
			SetLowerVelocityOfTimer1(40);
			SetTimer1();

			// �������2(50~30)
			SetUpperVelocityOfTimer2(50);
			SetLowerVelocityOfTimer2(30);
			SetTimer2();

			// �ص��
			MotorOff();
			// ������һ����״̬
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 60~40
			if (m_fV >= 50.0f)
			{
				// ��ȡ��һ�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
			}
			else
			{
				// ��ȡ��1�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// ������һ����״̬(50~30)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 50~30
			if (m_fV >= 40.0f)
			{
				// ��ȡ��1�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
			}
			else
			{
				// ��ȡ��1�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// ��1�α��
				const UINT unSection(0);
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

				//���õ�3�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(35);
				SetLowerVelocityOfTimer1(15);
				SetTimer1();

				// ������һ����״̬(50~30)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 50~30
			if (m_fV >= 35.0f)
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);


				// ������һ����״̬(35~15)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 35~15
			if (m_fV >= 30.0f)
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);

				// ��2�α��
				const UINT unSection(1);
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

				//���õ�4�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(22);
				SetLowerVelocityOfTimer2(8);
				SetTimer2();

				// ������һ����״̬(35~15)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 22~8
			if (m_fV >= 22.0f)
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// ������һ����״̬
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 35~15
			if (m_fV >= 15.0f)
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// ��3�α��
				const UINT unSection(2);
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
				
				//���1ʹ����ϣ�ֹͣ
				KillTimer1();

				// ������һ����״̬
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 22~8
			if (m_fV >= 8.0f)
			{
				// ��ȡ��4�λ���ʱ��(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// ��ȡ��4�λ���ʱ��(22~8)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// ��4�α��
				const UINT unSection(3);
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

				//���2ʹ����ϣ�ֹͣ
				KillTimer2();

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

void CNHCPLHPGB2018Thread::PLHPTestStep4_VMAS(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// �������1(60~40)
			SetUpperVelocityOfTimer1(60);
			SetLowerVelocityOfTimer1(40);
			SetTimer1();

			// �������2(50~30)
			SetUpperVelocityOfTimer2(50);
			SetLowerVelocityOfTimer2(30);
			SetTimer2();

			// �ص��
			MotorOff();
			// ������һ����״̬
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 60~40
			if (m_fV >= 50.0f)
			{
				// ��ȡ��һ�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
			}
			else
			{
				// ��ȡ��1�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// ������һ����״̬(50~30)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 50~30
			if (m_fV >= 40.0f)
			{
				// ��ȡ��1�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
			}
			else
			{
				// ��ȡ��1�λ���ʱ��(60~40)
				GetTimeOfTimer1(&m_unPLHPTime[0]);
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);

				// ��1�α��
				const UINT unSection(0);
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

				//���õ�3�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(35);
				SetLowerVelocityOfTimer1(15);
				SetTimer1();

				// ������һ����״̬(50~30)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 50~30
			if (m_fV >= 35.0f)
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);


				// ������һ����״̬(35~15)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 35~15
			if (m_fV >= 30.0f)
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
			}
			else
			{
				// ��ȡ��2�λ���ʱ��(50~30)
				GetTimeOfTimer2(&m_unPLHPTime[1]);
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);

				// ��2�α��
				const UINT unSection(1);
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
				

				//���õ�4�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(20);
				SetLowerVelocityOfTimer2(10);
				SetTimer2();

				// ������һ����״̬(35~15)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 20~8
			if (m_fV >= 20.0f)
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// ������һ����״̬
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 35~15
			if (m_fV >= 15.0f)
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// ��ȡ��3�λ���ʱ��(35~15)
				GetTimeOfTimer1(&m_unPLHPTime[2]);
				// ��ȡ��4�λ���ʱ��(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// ��3�α��
				const UINT unSection(2);
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
				
				//���1ʹ����ϣ�ֹͣ
				KillTimer1();

				// ������һ����״̬
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 20~10
			if (m_fV >= 10.0f)
			{
				// ��ȡ��4�λ���ʱ��(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);
			}
			else
			{
				// ��ȡ��4�λ���ʱ��(20~10)
				GetTimeOfTimer2(&m_unPLHPTime[3]);

				// ��4�α��
				const UINT unSection(3);
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

				//���2ʹ����ϣ�ֹͣ
				KillTimer2();

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

void CNHCPLHPGB2018Thread::PLHPTestStep4_LUGDOWN(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// �������1(80~70)
			SetUpperVelocityOfTimer1(80);
			SetLowerVelocityOfTimer1(70);
			SetTimer1();

			// �������2(70~60)
			SetUpperVelocityOfTimer2(70);
			SetLowerVelocityOfTimer2(60);
			SetTimer2();

			// �ص��
			MotorOff();
			// ������һ����״̬
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 100~90
			if (m_fV >= 70.0f)
			{
				// ��ȡ��1�λ���ʱ��(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
			}
			else
			{
				// ��ȡ��1�λ���ʱ��(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				// ��ȡ��1�λ���ʱ��(70~60)
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				
				//���õ�2�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(60);
				SetLowerVelocityOfTimer1(50);
				SetTimer1();

				// ������һ����״̬(70~60)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 70~60
			if (m_fV >= 60.0f)
			{
				// ��ȡ��1�λ���ʱ��
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

				// ��1�α��
				const UINT unSection(0);
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

				//���õ�2�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(50);
				SetLowerVelocityOfTimer2(40);
				SetTimer2();

				// ������һ����״̬(60~50)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 70~50
			if (m_fV >= 50.0f)
			{
				// ��ȡ��2�λ���ʱ��(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
			}
			else
			{
				// ��ȡ��2�λ���ʱ��(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70

				// ��2�α��
				const UINT unSection(1);
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

				//���õ�3�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(40);
				SetLowerVelocityOfTimer1(30);
				SetTimer1();

				// ������һ����״̬(50~40)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 50~40
			if (m_fV >= 40.0f)
			{
				// ��ȡ����ʱ��(50~40)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
			}
			else
			{				
				// ��ȡ����ʱ��(50~40)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60

				// ��3�α��
				const UINT unSection(2);
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

				//���õ�4�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(30);
				SetLowerVelocityOfTimer2(20);
				SetTimer2();

				// ������һ����״̬(40~30)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 40~30
			if (m_fV >= 30.0f)
			{
				// ��ȡ����ʱ��(40~30)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
			}
			else
			{				
				// ��ȡ����ʱ��(40~30)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50

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
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				//���õ�5�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(20);
				SetLowerVelocityOfTimer1(10);
				SetTimer1();


				// ������һ����״̬(30~20)
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 30~20
			if (m_fV >= 20.0f)
			{
				// ��ȡ����ʱ��(30~20)
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
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				
				//���2ʹ����ɣ��ر�
				KillTimer2();

				// ������һ����״̬(20~10)
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 20~10
			// ��ȡ����ʱ��(20~10)
			// ���һ��
			GetTimeOfTimer1(&m_unPLHPTime10[6]);
			m_unPLHPTime[5] = m_unPLHPTime10[5]+m_unPLHPTime10[6];	//ACDT20

			if (m_fV < 10.0f)
			{
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
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ���߶��Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				//���2ʹ����ϣ�ֹͣ
				KillTimer2();
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

void CNHCPLHPGB2018Thread::PLHPTestStep4_LUGDOWN_100(void)
{
	switch (m_unSlipState)
	{
	case 0:
		{
			// �������1(100~90)
			SetUpperVelocityOfTimer1(100);
			SetLowerVelocityOfTimer1(90);
			SetTimer1();

			// �������2(90~80)
			SetUpperVelocityOfTimer2(90);
			SetLowerVelocityOfTimer2(80);
			SetTimer2();

			// �ص��
			MotorOff();
			// ������һ����״̬
			m_unSlipState = 1;
		}
		break;
	case 1:
		{
			// 100~90
			if (m_fV >= 90.0f)
			{
				// ��ȡ��1�λ���ʱ��(100~90)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
			}
			else
			{
				// ��ȡ��1�λ���ʱ��(100~90)
				GetTimeOfTimer1(&m_unPLHPTime10[0]);
				// ��ȡ��1�λ���ʱ��(90~80)
				GetTimeOfTimer2(&m_unPLHPTime10[1]);
				m_unPLHPTime[0] = m_unPLHPTime10[0]+m_unPLHPTime10[1];	//ACDT90
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				
				//���õ�2�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(80);
				SetLowerVelocityOfTimer1(70);
				SetTimer1();

				// ������һ����״̬(90~80)
				m_unSlipState = 2;
			}
		}
		break;
	case 2:
		{
			// 90~80
			if (m_fV >= 80.0f)
			{
				// ��ȡ��1�λ���ʱ��
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

				// ��1�α��
				const UINT unSection(0);
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

				//���õ�2�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(70);
				SetLowerVelocityOfTimer2(60);
				SetTimer2();

				// ������һ����״̬(80~70)
				m_unSlipState = 3;
			}
		}
		break;
	case 3:
		{
			// 80~70
			if (m_fV >= 70.0f)
			{
				// ��ȡ��2�λ���ʱ��(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
			}
			else
			{
				// ��ȡ��2�λ���ʱ��(80~70)
				GetTimeOfTimer1(&m_unPLHPTime10[2]);
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[1] = m_unPLHPTime10[1]+m_unPLHPTime10[2];	//ACDT80
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70

				// ��2�α��
				const UINT unSection(1);
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

				//���õ�3�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(60);
				SetLowerVelocityOfTimer1(50);
				SetTimer1();

				// ������һ����״̬(70~60)
				m_unSlipState = 4;
			}
		}
		break;
	case 4:
		{
			// 70~60
			if (m_fV >= 60.0f)
			{
				// ��ȡ����ʱ��(70~60)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
			}
			else
			{				
				// ��ȡ����ʱ��(70~60)
				GetTimeOfTimer2(&m_unPLHPTime10[3]);
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[2] = m_unPLHPTime10[2]+m_unPLHPTime10[3];	//ACDT70
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60

				// ��3�α��
				const UINT unSection(2);
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

				//���õ�4�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(50);
				SetLowerVelocityOfTimer2(40);
				SetTimer2();

				// ������һ����״̬(60~50)
				m_unSlipState = 5;
			}
		}
		break;
	case 5:
		{
			// 60~50
			if (m_fV >= 50.0f)
			{
				// ��ȡ����ʱ��(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50
			}
			else
			{				
				// ��ȡ����ʱ��(60~50)
				GetTimeOfTimer1(&m_unPLHPTime10[4]);
				GetTimeOfTimer2(&m_unPLHPTime10[5]);
				m_unPLHPTime[3] = m_unPLHPTime10[3]+m_unPLHPTime10[4];	//ACDT60
				m_unPLHPTime[4] = m_unPLHPTime10[4]+m_unPLHPTime10[5];	//ACDT50

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
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}

				//���õ�5�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(40);
				SetLowerVelocityOfTimer1(30);
				SetTimer1();


				// ������һ����״̬(50~40)
				m_unSlipState = 6;
			}
		}
		break;
	case 6:
		{
			// 50~40
			if (m_fV >= 40.0f)
			{
				// ��ȡ����ʱ��(50~40)
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
				// ����Ϣ�����̴߳���֪ͨ���м�ʱ�������Ѿ����
				if (NULL != m_hCallBackWnd)
				{
					::PostMessage(m_hCallBackWnd, WM_DYN_PLHP_SECTION_FINISHED, (WPARAM)unSection, 0);
				}
				
				//���õ�6�׶����2�ٶȣ���������
				KillTimer2();
				SetUpperVelocityOfTimer2(30);
				SetLowerVelocityOfTimer2(20);
				SetTimer2();

				// ������һ����״̬(40~30)
				m_unSlipState = 7;
			}
		}
		break;
	case 7:
		{
			// 40~30
			if (m_fV >= 30.0f)
			{
				// ��ȡ����ʱ��(40~30)
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
				
				// ��6�α��
				const UINT unSection(5);
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

				//���õ�7�׶����1�ٶȣ���������
				KillTimer1();
				SetUpperVelocityOfTimer1(20);
				SetLowerVelocityOfTimer1(10);
				SetTimer1();

				// ������һ����״̬(30~20)
				m_unSlipState = 8;
			}
		}
		break;
	case 8:
		{
			// 30~20
			if (m_fV >= 20.0f)
			{
				// ��ȡ����ʱ��(30~20)
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
				
				// ��7�α��
				const UINT unSection(6);
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

				// ������һ����״̬(20~10)
				m_unSlipState = 9;
			}
		}
		break;
	case 9:
		{
			// 20~10

			// ��ȡ����ʱ��(20~10)
			// ���һ��
			GetTimeOfTimer1(&m_unPLHPTime10[8]);
			m_unPLHPTime[7] = m_unPLHPTime10[7]+m_unPLHPTime10[8];	//ACDT20

			if (m_fV < 10.0f)
			{
				// ��8�α��
				const UINT unSection(7);
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

				//���2ʹ����ϣ�ֹͣ
				KillTimer2();

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

void CNHCPLHPGB2018Thread::RemanenceRecord_ASM(void)
{
	// ASMʣ�ż�¼
	
	if (m_fV > 60.0f)
	{
		// δ������Ч���䷶Χ������¼ʣ��
	}
	else if (m_fV >= 50.0f)
	{
		// ��1��(60~40)
		RemanenceRecordOfSection1();
	}
	else if (m_fV >= 40.0f)
	{
		// ��1��(60~40)
		RemanenceRecordOfSection1();
		// ��2��(50~30)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 35.0f)
	{
		// ��2��(50~30)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 30.0f)
	{
		// ��2��(50~30)
		RemanenceRecordOfSection2();
		// ��3��(35~15)
		RemanenceRecordOfSection3();
	}
	else if (m_fV >= 15.0f)
	{
		// ��3��(35~15)
		RemanenceRecordOfSection3();
	}
}

void CNHCPLHPGB2018Thread::RemanenceRecord_VMAS(void)
{
	// VMASʣ�ż�¼
	
	RemanenceRecord_ASM();
}

void CNHCPLHPGB2018Thread::RemanenceRecord_LUGDOWN(void)
{
	// LUGDOWNʣ�ż�¼

	if (m_fV > 80.0f)
	{
		// δ������Ч���䷶Χ������¼ʣ��
	}
	else if (m_fV >= 70.0f)
	{
		// ��1��(80~60)
		RemanenceRecordOfSection1();
	}
	else if (m_fV >= 60.0f)
	{
		// ��1��(80~60)
		RemanenceRecordOfSection1();
		// ��2��(70~50)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 50.0f)
	{
		// ��2��(70~50)
		RemanenceRecordOfSection2();
		// ��3��(60~40)
		RemanenceRecordOfSection3();
	}
	else if (m_fV >= 40.0f)
	{
		// ��3��(60~40)
		RemanenceRecordOfSection3();
		// ��4��(50~30)
		RemanenceRecordOfSection4();
	}
	else if (m_fV >= 30.0f)
	{
		// ��4��(50~30)
		RemanenceRecordOfSection4();
		// ��5��(40~20)
		RemanenceRecordOfSection5();
	}
	else if (m_fV >= 20.0f)
	{
		// ��5��(40~20)
		RemanenceRecordOfSection5();
		// ��6��(30~10)
		RemanenceRecordOfSection6();
	}
	else if (m_fV >= 10.0f)
	{
		// ��6��(30~10)
		RemanenceRecordOfSection6();
	}

}

void CNHCPLHPGB2018Thread::RemanenceRecord_LUGDOWN_100(void)
{
	// LUGDOWNʣ�ż�¼

	if (m_fV > 100.0f)
	{
		// δ������Ч���䷶Χ������¼ʣ��
	}
	else if (m_fV >= 90.0f)
	{
		// ��1��(100~80)
		RemanenceRecordOfSection1();
	}
	else if (m_fV >= 80.0f)
	{
		// ��1��(100~80)
		RemanenceRecordOfSection1();
		// ��2��(90~70)
		RemanenceRecordOfSection2();
	}
	else if (m_fV >= 70.0f)
	{
		// ��2��(90~70)
		RemanenceRecordOfSection2();
		// ��3��(80~60)
		RemanenceRecordOfSection3();
	}
	else if (m_fV >= 60.0f)
	{
		// ��3��(80~60)
		RemanenceRecordOfSection3();
		// ��4��(70~50)
		RemanenceRecordOfSection4();
	}
	else if (m_fV >= 50.0f)
	{
		// ��4��(70~50)
		RemanenceRecordOfSection4();
		// ��5��(60~40)
		RemanenceRecordOfSection5();
	}
	else if (m_fV >= 40.0f)
	{
		// ��5��(60~40)
		RemanenceRecordOfSection5();
		// ��6��(50~30)
		RemanenceRecordOfSection6();
	}
	else if (m_fV >= 30.0f)
	{
		// ��6��(50~30)
		RemanenceRecordOfSection6();
		// ��7��(40~20)
		RemanenceRecordOfSection7();
	}
	else if (m_fV >= 20.0f)
	{
		// ��7��(40~20)
		RemanenceRecordOfSection7();
		// ��8��(30~10)
		RemanenceRecordOfSection8();
	}
	else if (m_fV >= 10.0f)
	{
		// ��8��(30~10)
		RemanenceRecordOfSection8();
	}
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection1(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[0] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[0] += m_fF;
	m_nRemanenceTimes[0]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection2(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[1] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[1] += m_fF;
	m_nRemanenceTimes[1]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection3(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[2] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[2] += m_fF;
	m_nRemanenceTimes[2]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection4(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[3] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[3] += m_fF;
	m_nRemanenceTimes[3]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection5(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[4] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[4] += m_fF;
	m_nRemanenceTimes[4]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection6(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[5] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[5] += m_fF;
	m_nRemanenceTimes[5]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection7(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[6] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[6] += m_fF;
	m_nRemanenceTimes[6]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection8(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[7] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[7] += m_fF;
	m_nRemanenceTimes[7]++;
}

void CNHCPLHPGB2018Thread::RemanenceRecordOfSection9(void)
{
	// ʣ�ż�¼-����ģʽ
	m_dRemanenceSumByPower[8] += m_fP;
	// ʣ�ż�¼-��ģʽ
	// TODO:ȷ���Ƿ������
	m_dRemanenceSumByForce[8] += m_fF;
	m_nRemanenceTimes[8]++;
}
