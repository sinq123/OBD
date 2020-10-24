/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCVarLoadThread.h
* �ļ���ʶ��
* �������ϻ��⹦����Ӧʱ�����
*
* �汾��1.0
* ���ߣ�Qin
* ���ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2010-06-08
* ���ߣ�Qin
* ��������ʽ����ʹ��
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
#pragma comment(lib, "winmm.lib")  // ��װ��ý�嶨ʱ��֧�ֿ�,Ҳ���ֶ���winmm.lib��ӵ�������

///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(begin)
#define NHC_VARLOAD_STATE_NOT_START				0x00	// 0:����δ��ʼ
#define NHC_VARLOAD_STATE_READY					0x01	// 1:׼������
#define NHC_VARLOAD_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
#define NHC_VARLOAD_STATE_ACCELERATION  		0x03	// 3:����
#define NHC_VARLOAD_STATE_SLIP  				0x04	// 4:����
#define NHC_VARLOAD_STATE_WAIT_FOR_STOP			0x05	// 5:�ȴ���Ͳֹͣ
#define NHC_VARLOAD_STATE_FINISHED				0x06	// 6:�������
#define NHC_VARLOAD_STATE_ERROR					0x07	// 7:���Թ��̷�������
///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// �������(begin)
#define NHC_VARLOAD_NO_ERROR					0x00	// 0:û�д�����
#define NHC_VARLOAD_COMMUNICATION_ERROR			0x01	// 1:��⹦��ͨ�Ŵ���
#define NHC_VARLOAD_START_MMTIMER_FAIL			0x02	// 2: ������ý�嶨ʱʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////// �������(end)

// CNHCVarLoadThread

class CNHCVarLoadThread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCVarLoadThread)

protected:
	CNHCVarLoadThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CNHCVarLoadThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	// ��Ӧ������StartVarLoadTest
	afx_msg void OnStartVarLoadTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()


public:
	// ��ʼ���غɲ���(�첽)
	// (in)hCallBackWnd���ص����ھ��
	void StartVarLoadTest_Asyn(const BYTE bTestType, const HWND hCallBackWnd=NULL);

	// ֹͣ���غɲ���
	void StopVarLoadTest(void);

public:
	// ���ر��غɲ���״̬
	inline UINT GetVarLoadState(void) const
	{
		return m_unVarLoadState;
	}
	// ���ر��ػ���ʱ��
	// (in)nSection����0��ʼ��ASM��VMAS��21�Σ�LUGDOWN��36��
	// (in)
	// (return)����Ӧָ���λ���ʱ�䣬��λ-ms
	inline UINT GetVarLoadTime(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<36);
		if (ASM == m_TestType)
		{
			return m_nAsmSlideTime[nSection];
		}
		else if (VMAS == m_TestType)
		{
			return m_nVmasSlideTime[nSection];
		}
		else
		{
			return m_nLugdownSlideTime[nSection];
		}
	}
		// ���ر��ػ��й���
	// (in)nSection����0��ʼ��ASM��VMAS��21�Σ�LUGDOWN��36��
	// (in)
	// (return)����Ӧָ���λ��й��ʣ���λ-KW
	inline float GetVarLoadPower(const int nSection) const
	{
		if (ASM == m_TestType)
		{
			ASSERT(nSection>=0 && nSection<21);
			return m_fAsmPowerAdd[nSection];
		}
		else if (VMAS == m_TestType)
		{
			ASSERT(nSection>=0 && nSection<21);
			return m_fVmasPowerAdd[nSection];
		}
		else
		{
			ASSERT(nSection>=0 && nSection<36);
			return m_fLugdownPowerAdd[nSection];
		}
	}
	// ��������������
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

	// ���������们��ʱ��
	inline UINT GetVarLoadTotalTime(void) const
	{
		if (ASM == m_TestType)
		{
			return m_nAsmSlideTime[20];
		}
		else if (VMAS == m_TestType)
		{
			return m_nVmasSlideTime[20];
		}
		else
		{
			return m_nLugdownSlideTime[35];
		}
	}


private:
	void VarLoadTest(void);
	void VarLoadTestStep1(void);           // ����׼��
	void VarLoadTestStep2(void);           // ��������
	void VarLoadTestStep3(void);           // ����
	void VarLoadTestOfLugdownStep4(void);   // Lugdwon����
	void VarLoadTestOfAsmStep4(void);       // ASM����
	void VarLoadTestOfVmasStep4(void);      // Lugdwon����
	void VarLoadTestStep5(void);           // �ȴ���Ͳֹͣ
	void CalcVarLoadResult(void);          // ������Խ��

private:
	static void CALLBACK TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// �������� �� ASM 1��VMAS 2��LUGDOWN 3
	BYTE m_TestType;
	// ��ʼ��ʱ�ٶȣ����ٵ㣬��λ:km/h
	float m_fUpperVelocity;
	// �ص���ĸ��ٶȵ㣬����ʼ��ʱ�ٶȼ���ƫ�ƣ���λ:km/h
	float m_fUpperVelocityWithOffset;
	// ֹͣ��ʱ�ٶȣ����ٵ㣬��λ:km/h
	float m_fLowerVelocity;
	// ֹͣȡ���ʱ���ٶȵ㣬����ֹͣȡ���ʱ���ٶȵ��ȥƫ�ƣ���λ:km/h
	float m_fLowerVelocityWithOffset;
	// ����м��ٶȵ㣬��λ:km/h
	float m_fMiddleVelocity;
	// ���ٶȵ�ƫ��,��λ:km/h
	const int m_nUpperVelocityOffset;
	// ���ٶȵ�ƫ��,��λ:km/h
	const int m_nLowerVelocityOffset;

	// �Ƿ�ﵽ���ٶȵ�
	bool m_bIsSpeedUp;
	// ���ڽ��б��غɻ��е��ٶȶ�,��1��ʼ��45
	int m_nStatues;

	// LUGDOWN���غɻ���ÿ�εļ��ع���
	// ���ݴ�m_fLugdownPowerAdd[1]��ʼΪ��һ��
	float m_fLugdownPowerAdd[36];

	// ASM���غɻ���ÿ�εļ��ع���
	// ���ݴ�m_fAsmPowerAdd[1]��ʼΪ��һ��
	float m_fAsmPowerAdd[21];

	// VMAS���غɻ���ÿ�εļ��ع���
	// ���ݴ�m_fVmasPowerAdd[1]��ʼΪ��һ��
	float m_fVmasPowerAdd[21];

	// LUGDOWN���غ�ÿ�εĻ���ʱ�� 
	// ���ݴ�m_nLugdownSlideTime[0]��ʼΪ��һ��
	// m_nLugdownSlideTime[35]���һ�����ܻ���ʱ��
	UINT m_nLugdownSlideTime[36];

	// ASM���غ�ÿ�εĻ���ʱ��
	// ���ݴ�m_nAsmSlideTime[0]��ʼΪ��һ��
	// m_nAsmSlideTime[20]���һ�����ܻ���ʱ��
	UINT m_nAsmSlideTime[21];

	// VMAS���غ�ÿ�εĻ���ʱ��
	// ���ݴ�m_nVmasSlideTime[0]��ʼΪ��һ��
	// m_nVmasSlideTime[20]���һ�����ܻ���ʱ��
	UINT m_nVmasSlideTime[21];

	// ��ʱ���ֱ���,��λ:ms
	UINT m_unTimerRes;
	// ��ý�嶨ʱ��
	UINT m_unTimerIdOfVarLoad;

	// ���غɲ���״̬(�궨��)
	// NHC_VARLOAD_STATE_NOT_START				0x00	// 0:����δ��ʼ
	// NHC_VARLOAD_STATE_READY					0x01	// 1:׼������
	// NHC_VARLOAD_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
	// NHC_VARLOAD_STATE_ACCELERATION			0x03	// 3:����
	// NHC_VARLOAD_STATE_SLIP					0x04	// 4:����(���غɲ�����)
	// NHC_VARLOAD_STATE_WAIT_FOR_STOP			0x05	// 5:�ȴ���Ͳֹͣ
	// NHC_VARLOAD_STATE_FINISHED				0x06	// 6:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
	// NHC_VARLOAD_STATE_ERROR					0x07	// 7:���Թ��̷�������
	UINT m_unVarLoadState;

	// �ص����ھ��
	HWND m_hCallBackWnd; 

	// �������
	// NHC_VARLOAD_NO_ERROR						0x00	// û�д�����
	// NHC_VARLOAD_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
	// NHC_VARLOAD_START_MMTIMER_FAIL			0x02	// ������ý�嶨ʱʧ��
	DWORD m_dwLastErrorCode;

	// ���Լ������λ:ms
	const UINT m_unTestInterval;
};