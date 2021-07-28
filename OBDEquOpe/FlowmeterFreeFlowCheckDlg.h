/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FlowmeterFreeFlowCheckDlg.h
* �ļ���ʶ��
* ժҪ��Sensor������VMAS�����������(����Ҫ��׼������)
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-12-22
*
* ��ʷ��Ϣ��
*
* �汾��1.0.11
* ���ڣ�2011-06-24
* ���ߣ�Sjming
* ������ʵʱֵ�����ƫ��ֵ��Լ��
*
* �汾��1.0.10
* ���ڣ�2011-02-23
* ���ߣ�Ming
* ��������������ʱ����ӵ�����׼ֵ�Ͳ���ֵ��
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
* ������1)�޸�Ϊ�����ڽ����޸�������׼ֵ
*       2)�������
*
* ��ǰ�汾��1.0.1
* ������ڣ�2009-12-22
* ���ߣ�Cui
* ��������ӻ�������(���ط��)
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


// CFlowmeterFreeFlowCheckDlg �Ի���

class CFlowmeterFreeFlowCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterFreeFlowCheckDlg)

public:
	CFlowmeterFreeFlowCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlowmeterFreeFlowCheckDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLOWMETER_FREE_FLOW_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonStart();
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

	// ����1
	CLabel m_lbStep1;
	// ����2
	CLabel m_lbStep2;
	// ����3
	CLabel m_lbStep3;
	// ����4
	CLabel m_lbStep4;

	// ������׼ֵ�༭��
	CNumericEdit m_neFreeFlowStandardValue;

	// ����ֵ��ʾ��
	CLabel m_lbMeauredValue;
	// ��������ʾ��
	CLabel m_lbRE;
	// ��������ֵ��ʾ��
	CLabel m_lbRELimit;
	// �ж������ʾ��
	CLabel m_lbJudgement;

	// �����б��
	CListCtrl m_lcData;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �������
	void ImportParams(void);
	// ����ʵʱ����
	void ExportFloFluxChkRTData(void);
	// �������
	void ExportFloFluxChkResult(void);

private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �������
	void Process(void);
	// ���ü������
	void ResetProcess(void);
	// ��ȡ��׼ֵ����
	// ����0x00,��׼ֵ�������Ҫ��
	// ��������,��׼ֵ���벻��Ҫ��
	DWORD GetStandardValue(void);
	// ��¼��׼ֵ�������ļ�
	void SaveStandardValueToIniFile(void);
	// ���ʵʱ��ʾ��
	void ClearRTDisplay(void);
	// ˢ��ʵʱ����
	// (in)bRefresh:true,ˢ��ʵʱ����,false,ֹͣˢ��ʵʱ����
	void RefreshRTData(bool bRefresh = true);
	// ��ʾʵʱ����
	void DisplayRTData(void);
	// ������
	void CalculateResult(void);
	// ��ʾ���
	void DisplayResult(void);
	// ������
	void ClearResult(void);
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
	// ������Ƿ�Pass
	bool m_bIsPass;
	// ���ݻ�ȡ����
	int m_nTimesOfGettingData;
	// 20������ʵʱ����,��λ:0.01L/s(�Ŵ�100��)
	int m_nFluxRTData[20];
	// ���̲���ֵ(20������ƽ��ֵ),��λ:0.01L/s(�Ŵ�100��)
	int m_nFluxMeasuredValue;
	// ����������׼ֵ,��λ:0.01L/s(�Ŵ�100��)
	int m_nFreeFlowStandardValue;
	// ������,��λ:%
	int m_nRE;
	// ��������ֵ,��λ:%
	const int m_nRELimit;
	// ������ƫ��ֵ����λ:L/s
	float m_fMaximumZeroOffsetValue;

private:
	// �������߳�
	CAnaThread *m_pFloThread;
};
