/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCVarLoadThread2.h
* �ļ���ʶ��
* �������ϻ��⹦�����غɻ��У�HJ/T290-2006�����Կ����߳�
*
* �汾��1.0
* ���ߣ�hemincong
* ���ڣ�2010-09-29
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2010-09-29
* ���ߣ�hemincong
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

class CNHCVarLoadThread2 :
	public CDynThread
{
	DECLARE_DYNCREATE(CNHCVarLoadThread2)

protected:
	DECLARE_MESSAGE_MAP()
public:
	CNHCVarLoadThread2(void);
	~CNHCVarLoadThread2(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	// ��Ӧ������StartVarLoadTest
	afx_msg void OnStartVarLoadTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFinishVarLoadTest(WPARAM wParam, LPARAM lParam);

public:
	// ��ʼ���غɲ���(�첽)
	// (in)hCallBackWnd���ص����ھ��
	void StartVarLoadTest_Asyn(const HWND hCallBackWnd=NULL);
	// ֹͣ���غɲ���
	void StopVarLoadTest(void);

	// ���ر��غɲ���״̬
	inline UINT GetVarLoadState(void) const
	{
		return m_unVarLoadState;
	}

	// ���ر��ػ���ʱ��
	// (in)nSection����0��ʼ����45����46��
	// (in)
	// (return)����Ӧָ���λ���ʱ�䣬��λ-ms
	inline UINT GetVarLoadTime(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<46);
		return m_nSlideTime[nSection];
	}

	// ���������们��ʱ��
	inline UINT GetVarLoadTotalTime(void) const
	{
		return m_nSlideTime[45];
	}

	// ���ر��ػ��й���
	// (in)nSection����0��ʼ
	// (in)
	// (return)����Ӧָ���λ��й��ʣ���λ-KW
	inline float GetVarLoadPower(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<46);
		return m_fPowerAdd[nSection];	
	}

	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}
		
	inline void SetAccType(const int AccType) 
	{
		m_nAccType = AccType;
	}

private:
	void VarLoadTest(void);
	void VarLoadTestStep1(void);        // ����׼��
	void VarLoadTestStep2(void);        // ��������
	void VarLoadTestStep3(void);		// ����
	void VarLoadTestStep4(void);		// ����
	void VarLoadTestStep5(void);        // �ȴ���Ͳֹͣ

private:
	static void CALLBACK TimeProOfVarLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	//�������� 0Ϊ������� 1Ϊ��������
	int m_nAccType;
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

	// ���غɻ���ÿ�εļ��ع���
	// ���ݴ�m_fPowerAdd[1]��ʼΪ��һ��
	float m_fPowerAdd[46];

	// ���غ�ÿ�εĻ���ʱ�� 
	// ���ݴ�m_nSlideTime[0]��ʼΪ��һ��
	// m_nSlideTime[46]���һ�����ܻ���ʱ��
	UINT m_nSlideTime[46];

	// ��ʱ���ֱ���,��λ:ms
	UINT m_unTimerRes;
	// ��ý�嶨ʱ��
	UINT m_unTimerIdOfVarLoad;

	// ���غɲ���״̬(�궨��)
	//#define NHC_VARLOAD_STATE_NOT_START				0x00	// 0:����δ��ʼ
	//#define NHC_VARLOAD_STATE_READY					0x01	// 1:׼������
	//#define NHC_VARLOAD_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
	//#define NHC_VARLOAD_STATE_ACCELERATION  			0x03	// 3:����
	//#define NHC_VARLOAD_STATE_SLIP  					0x04	// 4:����
	//#define NHC_VARLOAD_STATE_WAIT_FOR_STOP			0x05	// 5:�ȴ���Ͳֹͣ
	//#define NHC_VARLOAD_STATE_FINISHED				0x06	// 6:�������
	//#define NHC_VARLOAD_STATE_ERROR					0x07	// 7:���Թ��̷�������
	UINT m_unVarLoadState;

	// �ص����ھ��
	HWND m_hCallBackWnd; 

	// �������
	// NHC_VARLOAD_NO_ERROR								0x00	// û�д�����
	// NHC_VARLOAD_COMMUNICATION_ERROR					0x01	// ��⹦��ͨ�Ŵ���
	// NHC_VARLOAD_START_MMTIMER_FAIL					0x02	// ������ý�嶨ʱʧ��
	DWORD m_dwLastErrorCode;

	// ���Լ������λ:ms
	const UINT m_unTestInterval;
};

