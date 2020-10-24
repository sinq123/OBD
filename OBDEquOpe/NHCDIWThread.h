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
#define NHC_DIW_STATE_NOT_START				0x00	// 0:����δ��ʼ
#define NHC_DIW_STATE_READY					0x01	// 1:׼������
#define NHC_DIW_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
#define NHC_DIW_STATE_ACCELERATION_1		0x03	// 3:���٣���һ��
#define NHC_DIW_STATE_SLIP_1				0x04	// 4:����(С������)
#define NHC_DIW_STATE_WAIT_FOR_STOP			0x05	// 5:�ȴ���Ͳֹͣ
#define NHC_DIW_STATE_ACCELERATION_2		0x06	// 6:���٣��ڶ���
#define NHC_DIW_STATE_SLIP_2				0x07	// 7:����(�������)
#define NHC_DIW_STATE_FINISHED				0x08	// 8:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
#define NHC_DIW_STATE_ERROR					0x09	// 9:���Թ��̷�������
///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// �������(begin)
#define NHC_DIW_NO_ERROR					0x00	// 0:û�д�����
#define NHC_DIW_COMMUNICATION_ERROR			0x01	// 1:��⹦��ͨ�Ŵ���
#define NHC_DIW_START_MMTIMER_FAIL			0x02	// 2: ������ý�嶨ʱʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////// �������(end)

// CNHCDIWThread

class CNHCDIWThread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCDIWThread)

protected:
	CNHCDIWThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CNHCDIWThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()


public:
	// ��ʼ������������(�첽)
	// (in)fLowerForce:С����������λ:N
	// (in)fUpperForce:�����������λ:N
	// (in)fLowerVelocity:���ٵ㣬ֹͣ�����ٶȣ���λ:km/h
	// (in)fUpperVelocity:���ٵ㣬��ʼ�����ٶȣ���λ:km/h
	// (in)hCallBackWnd���ص����ھ��
	void StartDIWTest_Asyn(const float fLowerForce, 
		const float fUpperForce,
		const float fLowerVelocity, 
		const float fUpperVelocity, 
		const HWND hCallBackWnd=NULL);

	// ֹͣ������������
	void StopDIWTest(void);

public:
	// ���ػ�����������״̬
	inline UINT GetDIWState(void) const
	{
		return m_unDIWState;
	}

	// ����С����������ʱ��
	// ��λ-ms
	inline UINT GetLowerSlipingTime(void) const
	{
		return m_unLowerSlipingTime;
	}

	// ���ش����������ʱ��
	// ��λ-ms
	inline UINT GetUpperSlipingTime(void) const
	{
		return m_unUpperSlipingTime;
	}

	// ����ƽ��������(С������)
	// ��λ-N
	inline float GetAveLowerForce(void) const
	{
		return m_fAveLowerForce;
	}

	// ����ƽ��������(�������)
	// ��λ-N
	inline float GetAveUpperForce(void) const
	{
		return m_fAveUpperForce;
	}

	// ��������������
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

private:
	// ��Ӧ������StartDIWTest_Asyn
	afx_msg void OnStartDIWTest(WPARAM wParam, LPARAM lParam);

private:
	void DIWTest(void);
	void DIWTestStep1(void);  // ����׼��
	void DIWTestStep2(void);  // ��������
	void DIWTestStep3(void);  // ����
	void DIWTestStep4(void);  // ����(С������)
	void DIWTestStep5(void);  // �ȴ���Ͳֹͣ
	void DIWTestStep6(void);  // ����
	void DIWTestStep7(void);  // ����(�������)

private:
	static void CALLBACK TimeProOfDIW(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// С����������ʱ�䣬��λ:����(ms)
	UINT m_unLowerSlipingTime;
	// �����������ʱ�䣬��λ:����(ms)
	UINT m_unUpperSlipingTime;

	// ʵ��ƽ��������(С������),��λ:N
	float m_fAveLowerForce;
	// ʵ��ƽ��������(�������),��λ:N
	float m_fAveUpperForce;

	// ���Խ׶μ���������(�����������ƽ��ֵ)
	double m_dTotalForce;
	// ���Խ׶μ�������¼����(�����������ƽ��ֵ)
	UINT m_unTotalTime;

	// ��ʼ��ʱ�ٶȣ����ٵ㣬��λ:km/h
	float m_fUpperVelocity;
	// �ص���ĸ��ٶȵ㣬����ʼ��ʱ�ٶȼ���ƫ�ƣ���λ:km/h
	float m_fUpperVelocityWithOffset;
	// ֹͣ��ʱ�ٶȣ����ٵ㣬��λ:km/h
	float m_fLowerVelocity;
	// ֹͣȡ���ʱ���ٶȵ㣬����ֹͣȡ���ʱ���ٶȵ��ȥƫ�ƣ���λ:km/h
	float m_fLowerVelocityWithOffset;
	// ���ٶȵ�ƫ��,��λ:km/h
	const int m_nUpperVelocityOffset;
	// ���ٶȵ�ƫ��,��λ:km/h
	const int m_nLowerVelocityOffset;

	// С������,��λ:N
	float m_fLowerForce;
	// �������,��λ:N
	float m_fUpperForce;

	// ��ʱ���ֱ���,��λ:ms
	UINT m_unTimerRes;
	// ��ý�嶨ʱ��
	UINT m_unTimerIdOfDIW;

	// ������������״̬(�궨��)
	// NHC_DIW_STATE_NOT_START				0x00	// 0:����δ��ʼ
	// NHC_DIW_STATE_READY					0x01	// 1:׼������
	// NHC_DIW_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
	// NHC_DIW_STATE_ACCELERATION_1			0x03	// 3:���٣���һ��
	// NHC_DIW_STATE_SLIP_1					0x04	// 4:����(С������)
	// NHC_DIW_STATE_WAIT_FOR_STOP			0x05	// 5:�ȴ���Ͳֹͣ
	// NHC_DIW_STATE_ACCELERATION_2			0x06	// 6:���٣��ڶ���
	// NHC_DIW_STATE_SLIP_2					0x07	// 7:����(�������)
	// NHC_DIW_STATE_FINISHED				0x08	// 8:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
	// NHC_DIW_STATE_ERROR					0x09	// 9:���Թ��̷�������
	UINT m_unDIWState;

	// �ص����ھ��
	HWND m_hCallBackWnd; 

	// �������
	// NHC_DIW_NO_ERROR						0x00	// û�д�����
	// NHC_DIW_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
	// NHC_DIW_START_MMTIMER_FAIL			0x02	// ������ý�嶨ʱʧ��
	DWORD m_dwLastErrorCode;

	// ���Լ������λ:ms
	const UINT m_unTestInterval;
};