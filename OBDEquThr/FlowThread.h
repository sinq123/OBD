/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FlowThread.h
* �ļ���ʶ��
* ժҪ���������߳�
*
* ��ǰ�汾��
* ���ߣ�
* ������ڣ�
*
*/
#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"
#include "NanhuaNHF1.h"
// FlowThread

class AFX_EXT_CLASS CFlowThread : public CWinThread
{
	DECLARE_DYNCREATE(CFlowThread)

protected:
	CFlowThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFlowThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:

	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// (in)im����������
	// (in)bConnectToFlowmeter���Ƿ�������������
	// ���ش򿪽����
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// ANA_CLOSE_SUCCEED
	// ANA_CLOSE_FAIL_WIN32API
	// ANA_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����ǻ��
	bool IsOpen(void) const;


	// 1.��ȡ������ʵʱ����
	// pfFluxStd0:0���׼������L/s
	// pfFluxStd20:20���׼������L/s
	// pfFluxAct:ʵ��������L/s
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	// 2.��ȡ״̬
	DWORD Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau);

	// 3.���ط��
	// (in)bRun:���ط��������true-�������false-�ط��
	// (out)pbResult:�����������ָ�룻0x06-����ģʽ�ɹ���0x05-����ģʽʧ��(������æ)�� 0x15-����ģʽʧ��(��������)
	//               �������ִ�й��̴��󣬷���0xFF
	//				 ����ΪNULL
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);

	// 4.���ñ�׼����
	DWORD SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult);

	// 5.���ñ�׼ѹ��
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);

	// 6.�����¶�
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);

	// 7.���ñ�׼���� ע:fCalFlux���׼����20��״̬�� Mark By LiangQW@20160721
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CNanhuaNHF1::FluxCalibrationMode cm, const float fCalFlux);

	// 8.���øߵ��׼����(���ù���������)
	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);

	// 9.�����е��׼����(���ù���������)
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);

	// 10.���õ͵��׼����(���ù���������)
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);

	// 11.�ָ���������
	DWORD RestoreFactorySettings_Flowmeter(BYTE *const pbResult);



	
public:
	// �˳��̲߳���
	void QuitThread(void);

private:
	// ��ʼ���豸
	void InitializeInstrument(void);
	// ж���豸
	void UninitializeInstrument(void);

private:
	// �ⲿЭ��������
	CNanhuaNHF1* m_pFlow;
	// �߳��˳���־
	bool m_bQuitThread;

protected:
	// ����ֵ,��λ:scfm(��׼����Ӣ��/����)
	float m_fFluxStd0;
	float m_fFluxStd20;
	float m_fFluxAct;
	// ѹ��ֵ,��λ:kPa
	float  m_fPres;
	// δ����ϡ����,��λ:%
	float  m_fUDilO2;
	// ϡ�������¶�,��λ:���϶�
	float m_fGasTemp; 
	
	// ���ڲ���״̬
	DWORD m_dwStatus;
};


