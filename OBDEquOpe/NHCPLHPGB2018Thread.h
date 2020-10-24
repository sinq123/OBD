/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCPLHPGB2018Thread.h
* �ļ���ʶ��
* ժҪ���⹦���������ʻ����߳�
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-11-24
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.3
* ������ڣ�2013-07-31
* ���ߣ�Chc
* �������޸��������ʳ��ָ�����BUG
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2009-12-22
* ���ߣ�Cui
* ����������ע�ͺʹ����ʽ
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-11-24
* ���ߣ�Cui
* ��������ʽ����ʹ��
*
*
*/

#pragma once

// �Զ���
#include "..\NHDetCommModu\NHDetCommModu.h"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")  // ��װ��ý�嶨ʱ��֧�ֿ�,Ҳ���ֶ���winmm.lib��ӵ������С�

///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(begin)
#define NHC_PLHP_STATE_NOT_START				0x00	// 0:����δ��ʼ
#define NHC_PLHP_STATE_READY					0x01	// 1:׼������(��ʼ�����Բ���)
#define NHC_PLHP_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
#define NHC_PLHP_STATE_ACCELERATION				0x03	// 3:����
#define NHC_PLHP_STATE_SLIP						0x04	// 4:����
#define NHC_PLHP_STATE_FINISHED					0x05	// 5:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
#define NHC_PLHP_STATE_ERROR					0x06	// 6:���Թ��̷�������
///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// �������(begin)
#define NHC_PLHP_NO_ERROR						0x00	// û�д�����
#define NHC_PLHP_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
#define NHC_PLHP_START_MMTIMER_FAIL				0x02	// ������ý�嶨ʱʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////// �������(end)

// CNHCPLHPGB2018Thread

class CNHCPLHPGB2018Thread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCPLHPGB2018Thread)

protected:
	CNHCPLHPGB2018Thread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CNHCPLHPGB2018Thread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// ������������
	enum PLHPType
	{
		PLHP_ASM,
		PLHP_VMAS,
		PLHP_LUGDOWN,
		PLHP_LUGDOWN_100,//��ʼ�ٶ�100
	};

public:
	// ��ʼ��������(�첽)
	// (in)pt�������������ͣ�ASM��VMAS��LUGDOWN��
	// (in)rm��ʣ�ż�¼ģʽ
	// (in)hCallBackWnd���ص�����
	void StartPLHPTest_Asyn(const enum PLHPType pt, const HWND hCallBackWnd=NULL);

	// ֹͣ��������
	void StopPLHPTest(void);

public:
	// ���ؼ�������״̬
	inline UINT GetPLHPState(void) const
	{
		return m_unPLHPState;
	}

	// ���ؼ������ʻ���ʱ��
	// (in)nSection����0��ʼ��ASM��VMAS��5�Σ�LUGDOWN��9��
	// (return)����Ӧָ���λ���ʱ�䣬��λ-ms
	inline UINT GetPLHPTime(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<10);

		return m_unPLHPTime[nSection];
	}

	// ��������������
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

	// ���ظ����ٶ������ʣ��ƽ��ֵ(����ģʽ)
	// (in)nSection����0��ʼ��ASM��VMAS��5�Σ�LUGDOWN��9��
	// (return)����Ӧָ���ٶ������ʣ��ƽ��ֵ����λ:kW
	inline float GetRemanenceAveByPower(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<10);

		return m_fRemanenceAveByPower[nSection];
	}

	// ���ظ����ٶ������ʣ��ƽ��ֵ(��ģʽ)
	// (in)nSection����0��ʼ��ASM��VMAS��5�Σ�LUGDOWN��9��
	// (return)����Ӧָ���ٶ������ʣ��ƽ��ֵ����λ:N
	inline float GetRemanenceAveByForce(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<10);

		return m_fRemanenceAveByForce[nSection];
	}

private:
	// ��Ӧ������StartPLHPTest_Asyn
	afx_msg void OnStartPLHPTest(WPARAM wParam, LPARAM lParam);

private:
	// �˺�����ʱ������
	// ��ʼ��������(ͬ��)
	void StartPLHPTest(const enum PLHPType pt, const HWND hCallBackWnd=NULL);

private:
	// �������ʲ���
	void PLHPTest(void);
	void PLHPTestStep1(void);  // ����׼��
	void PLHPTestStep2(void);  // ��������
	void PLHPTestStep3(void);  // ����
	void PLHPTestStep4(void);  // ����(��ʱ)

	inline void PLHPTestStep4_ASM(void);
	inline void PLHPTestStep4_VMAS(void);
	inline void PLHPTestStep4_LUGDOWN(void);
	inline void PLHPTestStep4_LUGDOWN_100(void);

	// ʣ�ż�¼
	inline void RemanenceRecord_ASM(void);
	inline void RemanenceRecord_VMAS(void);
	inline void RemanenceRecord_LUGDOWN(void);
	inline void RemanenceRecord_LUGDOWN_100(void);
	inline void RemanenceRecordOfSection1(void);
	inline void RemanenceRecordOfSection2(void);
	inline void RemanenceRecordOfSection3(void);
	inline void RemanenceRecordOfSection4(void);
	inline void RemanenceRecordOfSection5(void);
	inline void RemanenceRecordOfSection6(void);
	inline void RemanenceRecordOfSection7(void);
	inline void RemanenceRecordOfSection8(void);
	inline void RemanenceRecordOfSection9(void);

private:
	// ��ʱ���ֱ���
	UINT m_unTimerRes;
	// �������ʻ��ж�ʱ��
	UINT m_unTimerIdOfPLHP;
	static void CALLBACK TimeProOfPLHP(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// �������ʲ���״̬,
	// NHC_PLHP_STATE_NOT_START					0x00	// 0:����δ��ʼ
	// NHC_PLHP_STATE_READY						0x01	// 1:׼������(��ʼ�����Բ���)
	// NHC_PLHP_STATE_LIFTBEAM_DOWN				0x02	// 2:��������
	// NHC_PLHP_STATE_ACCELERATION				0x03	// 3:����
	// NHC_PLHP_STATE_SLIP						0x04	// 4:����
	// NHC_PLHP_STATE_FINISHED					0x05	// 5:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
	// NHC_PLHP_STATE_ERROR						0x06	// 6:���Թ��̷�������
	UINT m_unPLHPState;

	// ����״̬
	// 0-δ��ʼ
	// �ڲ�ʹ��
	UINT m_unSlipState;

	// �ص����ھ��
	HWND m_hCallBackWnd; 

	// �������
	// NHC_PLHP_NO_ERROR						0x00	// û�д�����
	// NHC_PLHP_COMMUNICATION_ERROR				0x01	// ��⹦��ͨ�Ŵ���
	// NHC_PLHP_START_MMTIMER_FAIL				0x02	// ������ý�嶨ʱʧ��
	DWORD m_dwLastErrorCode;

private:
	// ������������
	enum PLHPType m_PLHPType;
	// ���Լ������λ-ms
	const UINT m_unTestInterval;
	// ͣ����ٶ�
	float m_fVelocityOfMotorOff;
	// ����ʱ�丨���м����������LUGDOWN��ʱ
	// ��λ-ms
	UINT m_unMiddleTimeOld;
	UINT m_unMiddleTimeNew;
	// �������ʸ��ٶȶ�ʱ��	
	// ��λ-ms
	UINT m_unPLHPTime[9];
	UINT m_unPLHPTime10[9];	// ÿ10km/h��¼һ��ʱ��
	 // �������ʸ��ٶȶ�ʣ���ۼ�ֵ,����ģʽ,��λ-kW
	double m_dRemanenceSumByPower[9];
	 // �������ʸ��ٶȶ�ʣ���ۼ�ֵ,��ģʽ,��λ-N
	double m_dRemanenceSumByForce[9];
	 // �������ʸ��ٶȶλ�ȡʣ���ۼƴ���
	int m_nRemanenceTimes[9];
	 // �������ʸ��ٶȶ�ʣ��ƽ��ֵ,����ģʽ,��λ-kW
	float m_fRemanenceAveByPower[9];
	 // �������ʸ��ٶȶ�ʣ��ƽ��ֵ,��ģʽ,��λ-N
	float m_fRemanenceAveByForce[9];
};


