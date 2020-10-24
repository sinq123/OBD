#pragma once
// �Զ���
//#include "..\NHDef\NHDef.h"
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
#define NHC_RT_STATE_NOT_START				0x00	// 0:����δ��ʼ
#define NHC_RT_STATE_READY					0x01	// 1:׼������
#define NHC_RT_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
#define NHC_RT_STATE_ACCELERATION			0x03	// 3:����
#define NHC_RT_STATE_SLIP					0x04	// 4:����
#define NHC_RT_STATE_BRAKE_High				0x05	// 5:���ٵ������
#define NHC_RT_STATE_BRAKE_Low				0x06	// 6:���ٵ������
#define NHC_RT_STATE_STABLE					0x07	// 7:�ȶ�
#define NHC_RT_STATE_FINISHED				0x08	// 8:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
#define NHC_RT_STATE_ERROR					0x09	// 9:���Թ��̷�������
///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// �������(begin)
#define NHC_RT_NO_ERROR						0x00	// û�д�����
#define NHC_RT_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
#define NHC_RT_START_MMTIMER_FAIL			0x02	// ������ý�嶨ʱʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////// �������(end)

#define NHC_RT_SAMPLING_NUM					10		// ��������
class CNHCResponseTime2Thread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCResponseTime2Thread)
public:
	CNHCResponseTime2Thread(void);
	~CNHCResponseTime2Thread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP();
private:
	afx_msg void OnStartResponseTimeTest(WPARAM wParam, LPARAM lParam);

private:
	void ResponseTimeTest(void);
	void ResponseTimeTestStep1(void); // ����׼��
	void ResponseTimeTestStep2(void); // ��������
	void ResponseTimeTestStep3(void); // ����			�ٶȴﵽ64.4 km/h  ��ʼ����
	void ResponseTimeTestStep4(void); // ����			�ٶȵ���56.3 km/h  ���ٵ������
	void ResponseTimeTestStep5(void); // ���ٵ������	�ٶȵ���40   km/h  ���ٵ������
	void ResponseTimeTestStep6(void); // ���ٵ������
	void ResponseTimeTestStep7(void); // �ȶ�
	//ȡʵʱ����
	void RefreshRealTimeData(void);
public:
	// fF_High �ƶ����ߵ�
	// fF_Low  �ƶ����͵�
	// (in)hCallBackWnd���ص�����
	void StartResponseTimeTest_Asyn(const float fF_High, const float fF_Low, const float fF_SpeedHigh, const float fF_SpeedLow, const float fF_TopSpeed,const HWND hCallBackWnd);

	void EndResponseTimeTest();

	///////// ��Ӧʱ������趨����////Begin
	// ���ٵ������
	float m_fLoad_High;
	// ���ٵ������
	float m_fLoad_Low;
	// ���ٵ��ٶ�
	float m_fSpeed_High;
	// ���ٵ��ٶ�
	float m_fSpeed_Low;
	// ��ʼ�����ٶ�
	float m_fTopSlipSpeed;
	///////// ��Ӧʱ������趨����////End

	float m_f_ResT_V[NHC_RT_SAMPLING_NUM];
	float m_f_ResT_F[NHC_RT_SAMPLING_NUM];


private:
	// ��ʱ���ֱ���
	UINT m_unTimerRes;

	// ���غɻ��ж�ʱ��
	UINT m_unTimerIdOfResponseTime;
	static void CALLBACK TimeProOfResponseTime(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	bool bIsSaveDate;

public:
	// ��Ӧʱ�����״̬,
	// NHC_RT_STATE_NOT_START				0x00	// 0:����δ��ʼ
	// NHC_RT_STATE_READY					0x01	// 1:׼������
	// NHC_RT_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
	// NHC_RT_STATE_ACCELERATION			0x03	// 3:����
	// NHC_RT_STATE_SLIP					0x04	// 4:����
	// NHC_RT_STATE_BRAKE_High				0x05	// 5:���ٵ������
	// NHC_RT_STATE_BRAKE_Low				0x06	// 6:���ٵ������
	// NHC_RT_STATE_STABLE					0x07	// 7:�ȶ�
	// NHC_RT_STATE_FINISHED				0x08	// 8:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
	// NHC_RT_STATE_ERROR					0x09	// 9:���Թ��̷�������
	UINT m_unResponseTimeState;
	// �������
	// NHC_RT_NO_ERROR						0x00	// û�д�����
	// NHC_RT_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
	// NHC_RT_START_MMTIMER_FAIL			0x02	// ������ý�嶨ʱʧ��
	DWORD m_dwLastErrorCode;

	// �ص����ھ��
	HWND m_hCallBackWnd; 

	// ���Լ������λ-ms
	const UINT m_unTestInterval;
	// ����ʱ��
	int m_nElapsedTime;

public:
	// ������Ӧʱ��״̬
	inline UINT GetResponseTimeState(void) const
	{
		return m_unResponseTimeState;
	}
	// ��������������
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}
	// �����ٶ�ʵʱ����
	inline float GetRealTimeVDate(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<NHC_RT_SAMPLING_NUM);

		return m_f_ResT_V[nSection];
	}
	// ������ʵʱ����
	inline float GetRealTimeFDate(const int nSection) const
	{
		ASSERT(nSection>=0 && nSection<NHC_RT_SAMPLING_NUM);

		return m_f_ResT_F[nSection];
	}
};
