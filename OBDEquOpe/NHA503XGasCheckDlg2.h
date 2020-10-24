/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHA503XGasCheckDlg2.h
* �ļ���ʶ��
* ժҪ���÷�����ͳһЭ�����ͨ���������������̽���������(����������У׼)
        ��NHA503XGasCheckDlg�Ľ�,ʹ��������ֵ�����ж�,ֻ�����������ݺͽ������
*
* ��ǰ�汾��1.0.4
* ���ߣ�Ming
* ������ڣ�2010-10-30
*
* ��ʷ��Ϣ��
*
* �汾��1.0.6
* ���ڣ�2012-10-12
* ���ߣ�Raylee
* ����������ϻ�NHA-501A�豸֧��
*
* �汾��1.0.5
* ���ڣ�2012-10-11
* ���ߣ�Raylee
* ��������ӷ��FGA4100������FLA501����ȪMQW511�豸֧��
*
* �汾��1.0.4
* ���ڣ�2010-10-30
* ���ߣ�Ming
* ���������������������ʱ,������׼ֵ�ĺ�����Լ��
*
* �汾��1.0.3
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-07-05
* ���ߣ�Hyh
* ���������ݷ�����Э��3.6�汾,��ӷ����ǰ汾��־λ,���ݲ�ͬ�汾���з����ǵ���
*
* ��ǰ�汾��1.0.0
* ʱ�䣺2010-07-01
* ���ߣ�Ming
* ��������ʽ����ʹ��
*/
#pragma once
#include "resource.h"
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


// CNHA503XGasCheckDlg2 �Ի���
class CNHA503XGasCheckDlg2 : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA503XGasCheckDlg2)

public:
	CNHA503XGasCheckDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHA503XGasCheckDlg2();

// �Ի�������
	enum { IDD = IDD_NHA503XGASCHECKDLG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�begin
	// ������������������
	void ImportGasCheckParams(void);
	// �����ⲿ��ֵ����
	void ImportGasCheckLimit(void);
	// ����������ʵʱ����
	void ExportGasCheckRTData(void);
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
	// ���±��ı��ؼ�
	CMathLabel m_mlb11;
	// ���±��ı��ؼ�
	CMathLabel m_mlb12;

	// HCʵʱֵ
	CLabel m_lbHC;
	// C3H8ʵʱֵ
	CLabel m_lbC3H8;
	// COʵʱֵ
	CLabel m_lbCO;
	// CO2ʵʱֵ
	CLabel m_lbCO2;
	// NOʵʱֵ
	CLabel m_lbNO;
	// O2ʵʱֵ
	CLabel m_lbO2;
	// PEFʵʱֵ
	CLabel m_lbPEF;

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

	// C3H8���ֵ�༭��
	CEdit m_edC3H8NominalValue;
	// CO���ֵ�༭��
	CNumericEdit m_edCONominalValue;
	// CO2���ֵ�༭��
	CNumericEdit m_edCO2NominalValue;
	// NO���ֵ�༭��
	CEdit m_edNONominalValue;

	// PEF����ֵ��ʾ��
	CLabel m_lbPEFMeasuredValue;
	
	// HC����ֵ��ʾ��
	CLabel m_lbHCMeasuredValue;
	// CO����ֵ��ʾ��
	CLabel m_lbCOMeasuredValue;
	// CO2����ֵ��ʾ��
	CLabel m_lbCO2MeasuredValue;
	// NO����ֵ��ʾ��
	CLabel m_lbNOMeasuredValue;

	// HC���ֵ��ʾ��
	CLabel m_lbHCNominalValue;
	// CO���ֵ��ʾ��
	CLabel m_lbCONominalValue;
	// CO2���ֵ��ʾ��
	CLabel m_lbCO2NominalValue;
	// NO���ֵ��ʾ��
	CLabel m_lbNONominalValue;

	// HC���������ʾ��
	CLabel m_lbHCAE;
	// CO���������ʾ��
	CLabel m_lbCOAE;
	// CO2���������ʾ��
	CLabel m_lbCO2AE;
	// NO���������ʾ��
	CLabel m_lbNOAE;

	
	// HC��������ʾ��
	CLabel m_lbHCRE;
	// CO��������ʾ��
	CLabel m_lbCORE;
	// CO2��������ʾ��
	CLabel m_lbCO2RE;
	// NO��������ʾ��
	CLabel m_lbNORE;
	
	// HC�ж���ʾ��
	CLabel m_lbHCJudgement;
	// CO�ж���ʾ��
	CLabel m_lbCOJudgement;
	// CO2�ж���ʾ��
	CLabel m_lbCO2Judgement;
	// NO�ж���ʾ��
	CLabel m_lbNOJudgement;
	
	// �þ���ʾ��
	CLabel m_lbDetermination;

private:
	// �������߳�
	CAnaThread *m_pAnaThread;
	
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ������Э��汾
	int m_nAnaProtocolVersion;

private:
	// ����
	int m_nStep;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
	// ʵʱ��������ṹ
	struct RTData
	{
		// ʱ�����
		int nTime;
		// HCֵ,��λ:ppm
		short sHC;
		// COֵ,��λ:0.01%
		float fCO;
		// CO2ֵ,��λ:0.01%
		float fCO2;
		// O2ֵ,��λ:%
		float fO2;
		// NOֵ,��λ:ppm
		short sNO;
		// ת��,��λ:r/min
		USHORT usRev;
		// ����,��λ:��
		float fOT;
		// ����
		USHORT usFlux;
		// Lambda
		float fLambda;
		// Pef
		float fPef;
		// �����¶�,��λ:��
		float fET;
		// ���ʪ��,��λ:%
		float fRH;
		// ����ѹ��,��λ:kPa
		float fAP;

		RTData(){ZeroMemory(this,sizeof(RTData));}
	};	
	// ��������
	CList<RTData, RTData> m_lsRTDataOfAnaGasChk;

private:
    // ��׼��C3H8���ֵ,��λ:ppm
	int m_nC3H8NominalValue;
    // ��׼��CO���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCONominalValue;
    // ��׼��CO2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCO2NominalValue;
	// ��׼��NO���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nNONominalValue;
	// ��׼��HC���ֵ,��λ:ppm,
	// ��C3H8���ֵ*PEF�õ�
	int m_nHCNominalValue;

private:
	// HC���������ֵ,��λ:ppm
	int m_nHCAELimit;
	// CO���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOAELimit;
	// CO2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AELimit;
	// NO���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNOAELimit;

	// HC��������ֵ,��λ:%
	int m_nHCRELimit;
	// CO��������ֵ,��λ:%
	int m_nCORELimit;
	// CO2��������ֵ,��λ:%
	int m_nCO2RELimit;
	// NO��������ֵ,��λ:%
	int m_nNORELimit;

private:
	// PEF����ֵ,�Ŵ�1000��,��������
	int m_nPEFMeasuredValue;
	// HC����ֵ,��λ:ppm
	int m_nHCMeasuredValue;
	// CO����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOMeasuredValue;
	// CO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2MeasuredValue;
	// NO����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNOMeasuredValue;
	// O2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nO2MeasuredValue;

	// HC�������,��λ:ppm
	int m_nHCAE;
	// CO�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOAE;
	// CO2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AE;
	// NO�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nNOAE;

	// HC��������Ƿ�ͨ��
	bool m_bIsHCAEPass;
	// CO��������Ƿ�ͨ��
	bool m_bIsCOAEPass;
	// CO2��������Ƿ�ͨ��
	bool m_bIsCO2AEPass;
	// NO��������Ƿ�ͨ��
	bool m_bIsNOAEPass;

	// HC������,��λ:%
	int m_nHCRE;
	// CO������,��λ:%,�Ŵ�100��,��������
	int m_nCORE;
	// CO2������,��λ:%,�Ŵ�100��,��������
	int m_nCO2RE;
	// NO������,��λ:%,�Ŵ�100��,��������
	int m_nNORE;

	// HC�������Ƿ�ͨ��
	bool m_bIsHCREPass;
	// CO�������Ƿ�ͨ��
	bool m_bIsCOREPass;
	// CO2�������Ƿ�ͨ��
	bool m_bIsCO2REPass;
	// NO�������Ƿ�ͨ��
	bool m_bIsNOREPass;

	// HCͨ���Ƿ�ͨ��
	bool m_bIsHCPass;
	// COͨ���Ƿ�ͨ��
	bool m_bIsCOPass;
	// CO2ͨ���Ƿ�ͨ��
	bool m_bIsCO2Pass;
	// NOͨ���Ƿ�ͨ��
	bool m_bIsNOPass;

	// ����Ƿ�ͨ��
	bool m_bIsCheckPass;

public:
	afx_msg void OnStnClickedStaticNoRe();
};
