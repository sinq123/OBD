/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHA403XGasCheckDlg.h
* �ļ���ʶ��
* ժҪ���÷�����ͳһЭ�����ͨ���������������̽�������������(��������������У׼)
        ��������503ϵ�з�����,��Ϊ503�����ǵ���,ͨ��������������ͨ�����ǲ�һ��
        (������ͳһЭ����Ҫ��503������Э������,��ִ��������Щ��һ��)
*
* ��ǰ�汾��1.0.15
* ���ߣ�Ming
* ������ڣ�2010-10-30
*
* ��ʷ��Ϣ��
*
* �汾��1.0.15
* ���ڣ�2010-10-30
* ���ߣ�Ming
* ���������������������ʱ,������׼ֵ�ĺ�����Լ��
*
* �汾��1.0.14
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* ��ǰ�汾��1.0.13
* ������ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.12
* ������ڣ�2010-07-05
* ���ߣ�Hyh
* ���������ݷ�����Э��3.6�汾,��ӷ����ǰ汾��־λ,���ݲ�ͬ�汾���з����ǵ���
*
* ��ǰ�汾��1.0.11
* ���ڣ�2010-07-01
* ���ߣ�Ming
* ������1����������CO2��O2�±���ʾ
*
* ��ǰ�汾��1.0.10
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* ������1���ص���ʾ��������Ϊ������
*		2���ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-05-12
* ���ߣ�Raylee
* ������дУ׼������ݸ��ûص�����
*
*
* ��ǰ�汾��1.0.7
* ʱ�䣺2010-04-08
* ���ߣ�Raylee
* �������ĵ����궨��־�ͼ��������
*
* ��ǰ�汾��1.0.6
* ʱ�䣺2010-01-13
* ���ߣ�Cui
* ������1)����Equipment.ini�ļ��ĸ��µ�������
*
* ��ǰ�汾��1.0.5
* ʱ�䣺2010-01-05
* ���ߣ�Cui
* ������1)��ӷ��������ӷ�ʽ������ʶ���Ƿ������������ӣ�����ǣ���Ҫִ���л���������ͨѶģʽ����
*
* ��ǰ�汾��1.0.4
* ʱ�䣺2009-12-08
* ���ߣ�Cui
* ���������·������߳�
*
* ��ǰ�汾��1.0.3
* ʱ�䣺2009-11-23
* ���ߣ�Raylee
* �������ĺ���ExportGasCheckDetail���ѱ궨���ݱ��浽ini��	
*		����afx_msg void OnSysCommand(UINT nID, LPARAM lParam);λ��
*
* ��ǰ�汾��1.0.2
* ʱ�䣺2009-11-20
* ���ߣ�Raylee
* �������ĺ���ExportGasCheckResult���ѱ궨���ݱ�����Reserved1�ֶ�
*
* ��ǰ�汾��1.0.1
* ʱ�䣺2009-09-21
* ���ߣ�Cui
* �������������ϵ�ppm��Ϊ10-6������ʵ�����±���Ӿ�Ч��
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-08-15
* ���ߣ�Cui
* ����������ͨ����顱���ʸ�Ϊ�������顱������ʽʹ��
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

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
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

// CNHA403XGasCheckDlg �Ի���

class CNHA403XGasCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA403XGasCheckDlg)

public:
	CNHA403XGasCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHA403XGasCheckDlg();

// �Ի�������
	enum { IDD = IDD_NHA403XGASCHECKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

private:
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�begin
	// ������������������
	void ImportGasCheckParams(void);
	// ������������(�б궨��)
	void ExportEquCalChkInfo(void);
	// ����������ʵʱ����
	void ExportGasCheckRTData(void);
	// �����궨��־(�ڲ�����)
	void ExportDemarcationLog(void);
	// ������������(�ڲ�����)
	void ExportGasCheckResult(void);
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�end

	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ����������
	void GasCheckProcess(void);
	// ���ü������
	void ResetProcess(void); 
	// ���ʵʱ��ʾ��
	void ClearRTDisplay(void);
	// ��ȡ��������ֵ����
	// ����0x00,���ֵ�������Ҫ��
	// ��������,���ֵ���벻��Ҫ��
	DWORD GetNominalValue(void);
	// ��¼���ֵ�������ļ�
	void SaveNominalValueToIniFile(void);
	// ������
	void CalculateResult(void);
	// ��ʾ���
	void ShowResult(void);

private:
	// �������߳�
	CAnaThread * m_pAnaThread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ������
	CShadeTitle m_lbHead;
	// �����
	CLabel m_lbInfo;
	// ʵʱֵ����
	CLabel m_lbLabel1;
	// ���ֵ����
	CLabel m_lbLabel2;
	// ������
	CLabel m_lbLabel3;
	// ��鲽�����
	CLabel m_lbLabel4;

	// HCʵʱֵ
	CLabel m_lbHC;
	// C3H8ʵʱֵ
	CLabel m_lbC3H8;
	// COʵʱֵ
	CLabel m_lbCO;
	// CO2ʵʱֵ
	CLabel m_lbCO2;
	// O2ʵʱֵ
	CLabel m_lbO2;
	// PEFʵʱֵ
	CLabel m_lbPEF;

	// C3H8���ֵ�༭��
	CEdit m_edC3H8NominalValue;
	// CO���ֵ�༭��
	CNumericEdit m_edCONominalValue;
	// CO2���ֵ�༭��
	CNumericEdit m_edCO2NominalValue;

	// ����1
	CLabel m_lbStep1;
	// ����2
	CLabel m_lbStep2;
	// ����3
	CLabel m_lbStep3;
	// ����4
	CLabel m_lbStep4;

	// ����1��ǩ
	CStatic m_stcStep1Label;
	// ����2��ǩ
	CStatic m_stcStep2Label;
	// ����3��ǩ
	CStatic m_stcStep3Label;
	// ����4��ǩ
	CStatic m_stcStep4Label;

	// PEF����ֵ��ʾ��
	CLabel m_lbPEFMeasuredValue;
	
	// HC����ֵ��ʾ��
	CLabel m_lbHCMeasuredValue;
	// CO����ֵ��ʾ��
	CLabel m_lbCOMeasuredValue;
	// CO2����ֵ��ʾ��
	CLabel m_lbCO2MeasuredValue;

	// HC���ֵ��ʾ��
	CLabel m_lbHCNominalValue;
	// CO���ֵ��ʾ��
	CLabel m_lbCONominalValue;
	// CO2���ֵ��ʾ��
	CLabel m_lbCO2NominalValue;

	// HC���������ʾ��
	CLabel m_lbHCAE;
	// CO���������ʾ��
	CLabel m_lbCOAE;
	// CO2���������ʾ��
	CLabel m_lbCO2AE;

	// HC��������ʾ��
	CLabel m_lbHCRE;
	// CO��������ʾ��
	CLabel m_lbCORE;
	// CO2��������ʾ��
	CLabel m_lbCO2RE;

	// HC�ж���ʾ��
	CLabel m_lbHCJudgement;
	// CO�ж���ʾ��
	CLabel m_lbCOJudgement;
	// CO2�ж���ʾ��
	CLabel m_lbCO2Judgement;

	// �þ���ʾ��
	CLabel m_lbDetermination;

	// ���±��ı��ؼ�
	CMathLabel m_mlb1;
	// ���±��ı��ؼ�
	CMathLabel m_mlb2;
	// ���±��ı��ؼ�
	CMathLabel m_mlb3;
	// ���±��ı��ؼ�
	CMathLabel m_mlb4;
	// ���±��ı��ؼ�
	CMathLabel m_mlb5;
	// ���±��ı��ؼ�
	CMathLabel m_mlb6;
	// ���±��ı��ؼ�
	CMathLabel m_mlb7;
	// ���±��ı��ؼ�
	CMathLabel m_mlb8;
	// ���±��ı��ؼ�
	CMathLabel m_mlb9;
	// ���±��ı��ؼ�
	CMathLabel m_mlb10;

private:
	// ������Э��汾
	int m_nAnaProtocolVersion;

private:
	// ����
	int m_nStep;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
    // ��׼��C3H8���ֵ,��λ:ppm
	int m_nC3H8NominalValue;
    // ��׼��CO���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCONominalValue;
    // ��׼��CO2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCO2NominalValue;
	// ��׼��HC���ֵ,��λ:ppm,
	// ��C3H8���ֵ*PEF�õ�
	int m_nHCNominalValue;

private:
	// HC���������ֵ,��λ:ppm
	const int m_nHCAELimit;
	// CO���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	const int m_nCOAELimit;
	// CO2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	const int m_nCO2AELimit;

	// HC��������ֵ,��λ:%
	const int m_nHCRELimit;
	// CO��������ֵ,��λ:%
	const int m_nCORELimit;
	// CO2��������ֵ,��λ:%
	const int m_nCO2RELimit;

private:
	// PEF����ֵ,�Ŵ�1000��,��������
	int m_nPEFMeasuredValue;
	// HC����ֵ,��λ:ppm
	int m_nHCMeasuredValue;
	// CO����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOMeasuredValue;
	// CO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2MeasuredValue;
	// O2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nO2MeasuredValue;

	// HC�������,��λ:ppm
	int m_nHCAE;
	// CO�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOAE;
	// CO2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AE;

	// HC��������Ƿ�ͨ��
	bool m_bIsHCAEPass;
	// CO��������Ƿ�ͨ��
	bool m_bIsCOAEPass;
	// CO2��������Ƿ�ͨ��
	bool m_bIsCO2AEPass;

	// HC������,��λ:%
	int m_nHCRE;
	// CO������,��λ:%,�Ŵ�100��,��������
	int m_nCORE;
	// CO2������,��λ:%,�Ŵ�100��,��������
	int m_nCO2RE;

	// HC�������Ƿ�ͨ��
	bool m_bIsHCREPass;
	// CO�������Ƿ�ͨ��
	bool m_bIsCOREPass;
	// CO2�������Ƿ�ͨ��
	bool m_bIsCO2REPass;

	// HCͨ���Ƿ�ͨ��
	bool m_bIsHCPass;
	// COͨ���Ƿ�ͨ��
	bool m_bIsCOPass;
	// CO2ͨ���Ƿ�ͨ��
	bool m_bIsCO2Pass;

	// ����Ƿ�ͨ��
	bool m_bIsCheckPass;

};
