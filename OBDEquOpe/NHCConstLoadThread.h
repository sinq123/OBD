/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCConstLoadThread.h
* �ļ���ʶ��
* ժҪ���⹦�����ػ��п����߳�
*
* �汾��1.0
* ���ߣ�Cui
* ���ڣ�2009-12-10
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.1
* ���ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* �汾��1.0.0
* ���ڣ�2009-12-10
* ���ߣ�Cui
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
#define NHC_CL_STATE_NOT_START				0x00	// 0:����δ��ʼ
#define NHC_CL_STATE_READY					0x01	// 1:׼������
#define NHC_CL_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
#define NHC_CL_STATE_ACCELERATION			0x03	// 3:����
#define NHC_CL_STATE_SLIP					0x04	// 4:����
#define NHC_CL_STATE_FINISHED				0x05	// 5:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
#define NHC_CL_STATE_ERROR					0x06	// 6:���Թ��̷�������
///////////////////////////////////////////////////////////////////////////////////////////////////����״̬(end)

/////////////////////////////////////////////////////////////////////////////////////////////////// �������(begin)
#define NHC_CL_NO_ERROR						0x00	// û�д�����
#define NHC_CL_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
#define NHC_CL_START_MMTIMER_FAIL			0x02	// ������ý�嶨ʱʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////// �������(end)

//class CNHCConstLoadThread : public CNHCCtrlThread
class CNHCConstLoadThread : public CDynThread
{
	DECLARE_DYNCREATE(CNHCConstLoadThread)

public:
	CNHCConstLoadThread(void);
	~CNHCConstLoadThread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// ��ʼ���غ�(�첽)
	// (in)fUpperVelocity:���ٵ㣬��ʼ�����ٶȣ���λ-km/h
	// (in)fLowerVelocity:���ٵ㣬ֹͣ�����ٶȣ���λ-km/h
	// (in)fLoadPower:���ع��ʣ���λ-kW
	// (in)hCallBackWnd���ص����ھ��
	void StartConstLoadTest_Asyn(const float fUpperVelocity, const float fLowerVelocity, const float fLoadPower, const HWND hCallBackWnd=NULL);

	// ֹͣ���غ�
	void StopConstLoadTest(void);

public:
	// ���ض��غ�״̬
	inline UINT GetConstLoadState(void) const
	{
		return m_unConstLoadState;
	}

	// ���ض��غɻ���ʱ��
	// ��λ-ms
	inline UINT GetConstLoadTime(void) const
	{
		return m_unConstLoadTime;
	}

	// ��������������
	inline DWORD GetLastErrorCode(void) const
	{
		return m_dwLastErrorCode;
	}

private:
	// ��Ӧ������StartConstLoadTest_Asyn
	afx_msg void OnStartConstLoadTest(WPARAM wParam, LPARAM lParam);

private:
	// �˺�����ʱ������
	// ��ʼ���غ�(ͬ��)
	// (in)fUpperVelocity:���ٵ㣬��ʼ�����ٶȣ���λ-km/h
	// (in)fLowerVelocity:���ٵ㣬ֹͣ�����ٶȣ���λ-km/h
	// (in)fLoadPower:���ع��ʣ���λ-kW
	void StartConstLoadTest(const float fUpperVelocity, const float fLowerVelocity, const float fLoadPower, const HWND hCallBackWnd=NULL);

private:
	void ConstLoadTest(void);
	void ConstLoadTestStep1(void);  // ����׼��
	void ConstLoadTestStep2(void);  // ��������
	void ConstLoadTestStep3(void);  // ����
	void ConstLoadTestStep4(void);  // ����(���ؼ�ʱ�׶�)

private:
	// ���غɻ���ʱ�䣬��λ-����(ms)
	UINT m_unConstLoadTime;

private:
	// ���غɿ�ʼ��ʱ�ٶȣ����ٵ㣬��λ-km/h
	float m_fUpperVelocity;
	// ���غɹص���ĸ��ٶȵ㣬���Ƕ��غɸ��ٵ����ƫ�ƣ���λ-km/h
	float m_fUpperVelocityWithOffset;
	// ���غ�ֹͣ��ʱ�ٶȣ����ٵ㣬��λ-km/h
	float m_fLowerVelocity;
	// ���غ�ֹͣȡ���ʱ���ٶȣ����Ƕ��غɵ��ٵ��ȥƫ�ƣ���λ-km/h
	float m_fLowerVelocityWithOffset;
	// ���غɼ��ع��ʣ���λ-kW
	float m_fLoadPower;

private:
	// ��ʱ���ֱ���
	UINT m_unTimerRes;

	// ���غɻ��ж�ʱ��
	UINT m_unTimerIdOfConstLoad;
	static void CALLBACK TimeProOfConstLoad(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:
	// ���غɲ���״̬,
	// NHC_CL_STATE_NOT_START				0x00	// 0:����δ��ʼ
	// NHC_CL_STATE_READY					0x01	// 1:׼������
	// NHC_CL_STATE_LIFTBEAM_DOWN			0x02	// 2:��������
	// NHC_CL_STATE_ACCELERATION			0x03	// 3:����
	// NHC_CL_STATE_SLIP					0x04	// 4:����
	// NHC_CL_STATE_FINISHED				0x05	// 5:�������(���ﻬ��������ٶȵ�,��Ͳ���ܻ���ת)
	// NHC_CL_STATE_ERROR					0x06	// 6:���Թ��̷�������
	UINT m_unConstLoadState;

	// ���ٶȵ�ƫ��
	const int m_nUpperVelocityOffset;
	// ���ٶȵ�ƫ��
	const int m_nLowerVelocityOffset;

	// �ص����ھ��
	HWND m_hCallBackWnd; 

	// �������
	// NHC_CL_NO_ERROR						0x00	// û�д�����
	// NHC_CL_COMMUNICATION_ERROR			0x01	// ��⹦��ͨ�Ŵ���
	// NHC_CL_START_MMTIMER_FAIL			0x02	// ������ý�嶨ʱʧ��
	DWORD m_dwLastErrorCode;

	// ���Լ������λ-ms
	const UINT m_unTestInterval;
};
