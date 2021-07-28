/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FlowmeterFlowCalibrationDlg.h
* �ļ���ʶ��
* ժҪ��Sensor������VMAS����У׼(��Ҫ��׼������)
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-12-22
*
* ��ʷ��Ϣ��
*
* �汾��1.0.10
* ���ڣ�2011-06-24
* ���ߣ�Sjming
* ������1��ʵʱֵ�����ƫ��ֵ��Լ��
*		2��ͨѶ����ʱ�������ο��ط����ť��������ʱ�Զ��ط��
*
* �汾��1.0.9
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.8
* ���ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-05-27
* ���ߣ�Raylee
* ������ʹ���µı궨���ݵ�����ʽ
*
* ��ǰ�汾��1.0.6
* ������ڣ�2010-05-22
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* ��ǰ�汾��1.0.5
* ������ڣ�2010-05-14
* ���ߣ�Raylee
* ������д�궨���ݸ��ûص�����
*
* ��ǰ�汾��1.0.4
* ʱ�䣺2010-02-02
* ���ߣ�Cui
* ������1)�����������߳�Open����ʹ�ã������ӵ�������ʵ��bConnectToFlowmeter��Ϊtrue
*
* ��ǰ�汾��1.0.3
* ʱ�䣺2010-01-13
* ���ߣ�Cui
* ������1)����Equipment.ini�ļ��ĸ��µ�������
*
* ��ǰ�汾��1.0.2
* ������ڣ�2009-12-23
* ���ߣ�Cui
* ������1)��У׼���̽���С�޸�
*       2)�������
*
* ��ǰ�汾��1.0.1
* ������ڣ�2009-12-22
* ���ߣ�Cui
* ��������ӻ�������(���ط����)
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-22
* ���ߣ�Cui
* ��������ʽ����ʹ��
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

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
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


// CFlowmeterFlowCalibrationDlg �Ի���

class CFlowmeterFlowCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterFlowCalibrationDlg)

public:
	CFlowmeterFlowCalibrationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlowmeterFlowCalibrationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLOWMETER_FLOW_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonCalibrationHighFlux();
	afx_msg void OnBnClickedButtonCalibrationLowFlux();
	afx_msg void OnBnClickedButtonFanOn();
	afx_msg void OnBnClickedButtonFanOff();

private:
	// ������
	CShadeTitle m_lbTitle;
	// ��Ϣ��ʾ��
	CLabel m_lbInfo;
	// ����1
	CLabel m_lbLabel1;
	// ����2
	CLabel m_lbLabel2;
	// ����3
	CLabel m_lbLabel3;
	// ����4
	CLabel m_lbLabel4;

	// ����ʵʱֵ��ʾ��
	CLabel m_lbFlux;

	// ����1��ǩ
	CStatic m_stcStep1Label;
	// ����2��ǩ
	CStatic m_stcStep2Label;
	// ����3��ǩ
	CStatic m_stcStep3Label;
	// ����4��ǩ
	CStatic m_stcStep4Label;
	// ����5��ǩ
	CStatic m_stcStep5Label;

	// ����1
	CLabel m_lbStep1;
	// ����2
	CLabel m_lbStep2;
	// ����3
	CLabel m_lbStep3;
	// ����4
	CLabel m_lbStep4;
	// ����5
	CLabel m_lbStep5;

	// ��������׼ֵ�༭��
	CNumericEdit m_neStandardValueHighFlux;
	// ��������׼ֵ�༭��
	CNumericEdit m_neStandardValueLowFlux;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	//��ʾ�ؼ�
	void ShowEntrance(void);
	void ShowDomestic(void);

private:
	// �������
	void ImportParams(void);
	// ����ʵʱ����
	void ExportFloFluxCalRTData(void);
	// �������
	void ExportFloFluxCalResult(void);

private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ˢ��ʵʱ����
	// (in)bRefresh:true,ˢ��ʵʱ����,false,ֹͣˢ��ʵʱ����
	void RefreshRTData(bool bRefresh = true);
	// ��ʾʵʱ����
	void DisplayRTData(void);
	// ����
	void Process(void);
	// ��������
	void ResetProcess(void);
	// ��ȡ��������׼ֵ����
	// ����0x00,��׼ֵ�������Ҫ��
	// ��������,��׼ֵ���벻��Ҫ��
	DWORD GetStandardValueOfHighFlux(void);
	// ��ȡ��������׼ֵ����
	// ����0x00,��׼ֵ�������Ҫ��
	// ��������,��׼ֵ���벻��Ҫ��
	DWORD GetStandardValueOfLowFlux(void);
	// �Ƿ������������(���ط����)
	void EnableBasicOperation(bool bEnable=true);

	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ����
	int m_nStep;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;
	// У׼����Ƿ�Pass
	bool m_bIsPass;
	// ��������׼ֵ,��λ:L/s
	float m_fStandardValueOfHighFlux;
	// ��������׼ֵ,��λ:L/s
	float m_fStandardValueOfLowFlux;
	// ������ƫ��ֵ����λ:L/s
	float m_fMaximumZeroOffsetValue;
	// �ϻ�Sensor
	bool m_bDomestic;
private:
	// �������߳�
	CAnaThread *m_pFloThread;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
