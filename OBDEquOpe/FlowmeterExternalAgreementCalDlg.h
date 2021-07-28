/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FlowmeterExternalCalibrationDlg.h.h
* �ļ���ʶ��
* ժҪ���ⲿЭ��������У׼
*
* ��ǰ�汾��
* ���ߣ�
* ������ڣ�
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

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif



//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include "resource.h"		// ������
#include "afxwin.h"


// CFlowmeterExternalCalDlg �Ի���

class CFlowmeterExternalCalDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterExternalCalDlg)

public:
	CFlowmeterExternalCalDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlowmeterExternalCalDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLOWMETER_EXTERNAL_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFanOn();
	afx_msg void OnBnClickedButtonFanOff();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonCalibrationTp();
	afx_msg void OnBnClickedButtonCalibrationPress();
	afx_msg void OnBnClickedButtonCalibrationO2();
	afx_msg void OnBnClickedButtonCalibrationHighFlux();
	afx_msg void OnBnClickedButtonCalibrationLowFlux();
	afx_msg void OnBnClickedButtonCalibrationSingleFlux();
	afx_msg void OnBnClickedButtonRecovery();
	afx_msg void OnBnClickedRadioHigh();
	afx_msg void OnBnClickedRadioMiddle();
	afx_msg void OnBnClickedRadioLow();
	afx_msg void OnBnClickedRadioZero();
;
private:
	// ������
	CShadeTitle m_lbTitle;
	// ��Ϣ��ʾ��
	CLabel m_lbInfo;
	// ��׼������ʾ��
	CLabel m_lbStd0Flux;
	CLabel m_lbStd20Flux;
	CLabel m_lbActFlux;
	// �¶���ʾ��
	CLabel m_lbTemp;
	// ѹ����ʾ��
	CLabel m_lbPres;
	// ϡ������ʾ��
	CLabel m_lbUDilO2;
	// ϡ����ѡ��
	CButton m_rdoHigh;
	CButton m_rdoMiddle;
	CButton m_rdoLow;
	// ϡ����У׼ֵ�༭��,��
	CNumericEdit m_neHighO2Value;
	CNumericEdit m_neMiddleO2Value;
	CNumericEdit m_neLowO2Value;
	CNumericEdit m_neZeroO2Value;
	// �¶�У׼ֵ�༭��,��
	CNumericEdit m_neTPCalibrationValue;
	// ѹ��У׼ֵ�༭��,kPa
	CNumericEdit m_nePressureCalibrationValue;
	// �ߵ�������׼ֵ�༭��
	CNumericEdit m_neStandardValueHighFlux;
	// �͵�������׼ֵ�༭��
	CNumericEdit m_neStandardValueLowFlux;
	// ����������׼ֵ�༭��
	CNumericEdit m_neStandardValueSingleFlux;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �Ƿ������������(���ط����)
	void EnableBasicOperation(bool bEnable=true);

private:
	// ˢ��ʵʱ����
	// (in)bRefresh:true,ˢ��ʵʱ����,false,ֹͣˢ��ʵʱ����
	void RefreshRTData(bool bRefresh = true);

	// �����¶�У׼���
	void ExportFloTPCalResult(void);
	// ��ȡ�¶�У׼ֵ����
	DWORD GetTpCalibrationValue(void);
	// �¶�У׼����
	DWORD TpCalibrationProcess(void);
	
	// ����ѹ��У׼���
	void ExportFloPresCalResult(void);
	// ��ȡѹ��У׼ֵ����
	DWORD GetPresCalibrationValue(void);
	// ѹ��У׼����
	DWORD PresCalibrationProcess(void);

	// ����ϡ����У׼���
	void ExportFloDilO2CalResult(void);
	// У׼����
	DWORD DilO2CalibrationProcess(void);
	DWORD CalibrationProcess_High(void);
	DWORD CalibrationProcess_Middle(void);
	DWORD CalibrationProcess_Low(void);

	// �������
	void ImportParams(void);
	// �������
	void ExportFloFluxCalResult(CString strName,float fValue,BOOL bPass);
	// ��ȡ�ߵ�������׼ֵ����
	DWORD GetStandardValueOfHighFlux(void);
	// ��ȡ�͵�������׼ֵ����
	DWORD GetStandardValueOfLowFlux(void);
	// ��ȡ����������׼ֵ����
	DWORD GetStandardValueOfSingleFlux(void);

	// �ָ���������
	DWORD RestoreFactorySettings(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �¶�У׼ֵ
	float m_fTPCalValue;
	// ѹ��У׼ֵ
	float m_fPressureCalibrationValue;
	// �¶�У׼����Ƿ�Pass
	bool m_bIsTpCalibrationPass;
	// ѹ��У׼����Ƿ�Pass
	bool m_bIsPresCalibrationPass;

	//У׼��(1:�� 2:�� 3:�� 4:��)
	int m_nPoint;
	// ϡ����У׼����Ƿ�Pass
	bool m_bIsO2CalibrationPass;
	
	// ����У׼����Ƿ�Pass
	bool m_bIsFlowCalibrationPass;
	// �ߵ�������׼ֵ,��λ:L/s
	float m_fStandardValueOfHighFlux;
	// ����������׼ֵ,��λ:L/s
	float m_fStandardValueOfSingleFlux;
	// �͵�������׼ֵ,��λ:L/s
	float m_fStandardValueOfLowFlux;
	// ������ƫ��ֵ����λ:L/s
	float m_fMaximumZeroOffsetValue;

	// �������߳�
	CFlowThread *m_pFloThread;
};
