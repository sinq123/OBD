/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHA503XGasCheckDlg.h
* �ļ���ʶ��
* ժҪ���÷�����ͳһЭ�����ͨ���������������̽�������������(��������������У׼)
        ��403X�Ľ������NOͨ��
*
* ��ǰ�汾��1.0.16
* ���ߣ�Ming
* ������ڣ�2010-10-30
*
* ��ʷ��Ϣ��
*
* �汾��1.0.18
* ���ڣ�2012-10-12
* ���ߣ�Raylee
* ����������ϻ�NHA-501A�豸֧��
*
* �汾��1.0.17
* ���ڣ�2012-10-11
* ���ߣ�Raylee
* ��������ӷ��FGA4100������FLA501����ȪMQW511�豸֧��
*
* �汾��1.0.16
* ���ڣ�2010-10-30
* ���ߣ�Ming
* ���������������������ʱ,������׼ֵ�ĺ�����Լ��
*
* �汾��1.0.15
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* ��ǰ�汾��1.0.14
* ������ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.13
* ������ڣ�2010-07-05
* ���ߣ�Hyh
* ���������ݷ�����Э��3.6�汾,��ӷ����ǰ汾��־λ,���ݲ�ͬ�汾���з����ǵ���
*
* ��ǰ�汾��1.0.12
* ���ڣ�2010-07-01
* ���ߣ�Ming
* ������1)ȥ����ǰ���ⲿ��ֵ�������
*       2)��������CO2��O2�±���ʾ
*	    3)���������ֵ���ú���,��ʼ����ֵ��ʾΪ˫������ֵ
*
* ��ǰ�汾��1.0.11
* ���ڣ�2010-06-29
* ���ߣ�Ming
* ������1)д�������ݵ�������
*		2)����ⲿ��ֵ�������,ͨ���ж�App_Data���Ƿ����LimitOfAnaGasCheck.ini�ļ�,ȷ����ֵ��ȡ��ʽ,����������˫�������ͼ���
*
* ��ǰ�汾��1.0.10
* ������ڣ�2010-06-01
* ���ߣ�Raylee
* ��������׼��DIS�����ж�NO������Ͱ������ǵ�˵�������������ֵ
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* ������1���ص���ʾ��������Ϊ������
*		2���ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-05-12
* ���ߣ�Raylee
* ������дУ׼������ݸ��ûص�����
*
* ��ǰ�汾��1.0.6
* ʱ�䣺2010-04-08
* ���ߣ�Raylee
* ������1���ĵ����궨��־�ͼ��������
*		2����;ֹͣʱ��Ϊ503��ӹر���������
*
* ��ǰ�汾��1.0.5
* ʱ�䣺2010-04-02
* ���ߣ�Raylee
* ������1��Ϊ503��Ӵ�/�رռ����������
*		2������DIS��ASM��VMAS���ּ�ⷽ����ͬ����������Ҫ�󣬷ֱ����������ж�
*		3������NO��ֵ�ĵ�λΪppm
*
* ��ǰ�汾��1.0.4
* ʱ�䣺2010-01-13
* ���ߣ�Cui
* ������1)����Equipment.ini�ļ��ĸ��µ�������
*
* ��ǰ�汾��1.0.3
* ʱ�䣺2010-01-05
* ���ߣ�Cui
* ������1)��ӷ��������ӷ�ʽ������ʶ���Ƿ������������ӣ�����ǣ���Ҫִ���л���������ͨѶģʽ����
*
* ��ǰ�汾��1.0.2
* ʱ�䣺2009-12-23
* ���ߣ�Cui
* ����������������ڲ���ͨѶʱ�����Ρ��˳�����ť�����ĳ���4�ּ��ͬһģʽ
*
* ��ǰ�汾��1.0.1
* ʱ�䣺2009-12-08
* ���ߣ�Cui
* ���������·������߳�

* ��ǰ�汾��1.0.0
* ʱ�䣺2009-12-08
* ���ߣ�Cui
* ��������ʽ����ʹ��
*/
#pragma once
#include "resource.h"
// �Զ���
#include "..\NHDetCommModu\NHDetCommModu.h"
//#include "..\NHDef\NHStructDef.h"

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


// CNHA503XGasCheckDlg �Ի���
class CNHA503XGasCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA503XGasCheckDlg)

public:
	CNHA503XGasCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHA503XGasCheckDlg();

// �Ի�������
	enum { IDD = IDD_NHA503XGASCHECKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	//afx_msg void OnBnClickedRadioTestType();
	afx_msg void OnBnClickedRadioChn();
	afx_msg void OnBnClickedRadioCheckType();
	DECLARE_MESSAGE_MAP()

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
	//void CalculateResult_DIS(void);
	//void CalculateResult_ASM(void);
	void CalculateResult_VMAS(void);
	// ��ʾ���
	//void ShowResult_DIS(void);	
	//void ShowResult_ASM(void);
	void ShowResult_VMAS(void);
	// ��ֵ����
	//void SetLimit_DIS(void);
	//void SetLimit_ASM(void);
	void SetLimit_VMAS(void);
	// ������Ӧʱ��
	void CalculateRespTime(void);

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
	CMathLabel m_mlb8;
	// ���±��ı��ؼ�
	CMathLabel m_mlb12;
	// ���±��ı��ؼ�
	CMathLabel m_mlb13;
	// ���±��ı��ؼ�
	CMathLabel m_mlb14;
	// ���±��ı��ؼ�
	CMathLabel m_mlb15;
	// ���±��ı��ؼ�
	CMathLabel m_mlb16;
	// ���±��ı��ؼ�
	CMathLabel m_mlb17;
	// ���±��ı��ؼ�
	CMathLabel m_mlb18;
	CMathLabel m_mlb19;

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
	// NO2ʵʱֵ
	CLabel m_lbNO2;
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
	// NO2���ֵ�༭��
	CEdit m_edNO2NominalValue;
	// O2���ֵ�༭��
	CNumericEdit m_edO2NominalValue;

	// PEF����ֵ��ʾ��
	CLabel m_lbPEFMeasuredValue;
	
	// HC����ֵ��ʾ��
	CLabel m_lbHCMeasuredValue;
	// CO����ֵ��ʾ��
	CLabel m_lbCOMeasuredValue;
	// CO2����ֵ��ʾ��
	CLabel m_lbCO2MeasuredValue;
	// O2����ֵ��ʾ��
	CLabel m_lbO2MeasuredValue;
	// NO����ֵ��ʾ��
	CLabel m_lbNOMeasuredValue;
	// NO2����ֵ��ʾ��
	CLabel m_lbNO2MeasuredValue;

	// HC���ֵ��ʾ��
	CLabel m_lbHCNominalValue;
	// CO���ֵ��ʾ��
	CLabel m_lbCONominalValue;
	// CO2���ֵ��ʾ��
	CLabel m_lbCO2NominalValue;
	// O2���ֵ��ʾ��
	CLabel m_lbO2NominalValue;
	// NO���ֵ��ʾ��
	CLabel m_lbNONominalValue;
	// NO2���ֵ��ʾ��
	CLabel m_lbNO2NominalValue;

	// HC���������ʾ��
	CLabel m_lbHCAE;
	// CO���������ʾ��
	CLabel m_lbCOAE;
	// CO2���������ʾ��
	CLabel m_lbCO2AE;
	// O2���������ʾ��
	CLabel m_lbO2AE;
	// NO���������ʾ��
	CLabel m_lbNOAE;
	// NO2���������ʾ��
	CLabel m_lbNO2AE;
		
	// HC��������ʾ��
	CLabel m_lbHCRE;
	// CO��������ʾ��
	CLabel m_lbCORE;
	// CO2��������ʾ��
	CLabel m_lbCO2RE;
	// O2��������ʾ��
	CLabel m_lbO2RE;
	// NO��������ʾ��
	CLabel m_lbNORE;
	// NO2��������ʾ��
	CLabel m_lbNO2RE;
	
	// HC�ж���ʾ��
	CLabel m_lbHCJudgement;
	// CO�ж���ʾ��
	CLabel m_lbCOJudgement;
	// CO2�ж���ʾ��
	CLabel m_lbCO2Judgement;
	// O2�ж���ʾ��
	CLabel m_lbO2Judgement;
	// NO�ж���ʾ��
	CLabel m_lbNOJudgement;
	// NO2�ж���ʾ��
	CLabel m_lbNO2Judgement;

	// HC��Ӧʱ��
	CLabel m_lbHCRespTime;
	// CO��Ӧʱ��
	CLabel m_lbCORespTime;
	// CO2��Ӧʱ��
	CLabel m_lbCO2RespTime;
	// O2��Ӧʱ��
	CLabel m_lbO2RespTime;
	// NO��Ӧʱ��
	CLabel m_lbNORespTime;
	// NO2��Ӧʱ��
	CLabel m_lbNO2RespTime;
	
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
	// ͨ��
	// 1��HC_CO_CO2
	// 2��NO
	// 3��NO2
	// 4��HC_CO_CO2_NO
	int m_nChannel;
	// ��׼��Ũ��
	// 1����Ũ��
	// 2���е�Ũ��
	// 3���и�Ũ��
	// 4����Ũ��
	int m_nCheckType;

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
		// NO2ֵ,��λ:ppm
		short sNO2;
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
		// ʱ��
		SYSTEMTIME st;

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
	// ��׼��NO2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nNO2NominalValue;
	// ��׼��HC���ֵ,��λ:ppm,
	// ��C3H8���ֵ*PEF�õ�
	int m_nHCNominalValue;
    // ��׼��O2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nO2NominalValue;

private:
	// HC���������ֵ,��λ:ppm
	int m_nHCAELimit;
	// CO���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOAELimit;
	// CO2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AELimit;
	// NO���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNOAELimit;
	// NO2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNO2AELimit;
	// O2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nO2AELimit;

	// HC��������ֵ,��λ:%
	int m_nHCRELimit;
	// CO��������ֵ,��λ:%
	int m_nCORELimit;
	// CO2��������ֵ,��λ:%
	int m_nCO2RELimit;
	// NO��������ֵ,��λ:%
	int m_nNORELimit;
	// NO2��������ֵ,��λ:%
	int m_nNO2RELimit;
	// O2��������ֵ,��λ:%
	int m_nO2RELimit;
	
	// ��Ӧʱ��Ҫ��
	float m_fHCT90Limit;
	float m_fHCT10Limit;
	float m_fCOT90Limit;
	float m_fCOT10Limit;	
	float m_fCO2T90Limit;
	float m_fCO2T10Limit;
	float m_fNOT90Limit;
	float m_fNOT10Limit;
	float m_fNO2T90Limit;
	float m_fNO2T10Limit;
	float m_fO2T90Limit;
	float m_fO2T10Limit;

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
	// NO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNO2MeasuredValue;
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
	// NO2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nNO2AE;
	// O2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nO2AE;

	// HC��������Ƿ�ͨ��
	bool m_bIsHCAEPass;
	// CO��������Ƿ�ͨ��
	bool m_bIsCOAEPass;
	// CO2��������Ƿ�ͨ��
	bool m_bIsCO2AEPass;
	// NO��������Ƿ�ͨ��
	bool m_bIsNOAEPass;
	// NO2��������Ƿ�ͨ��
	bool m_bIsNO2AEPass;
	// O2��������Ƿ�ͨ��
	bool m_bIsO2AEPass;

	// HC������,��λ:%
	int m_nHCRE;
	// CO������,��λ:%,�Ŵ�100��,��������
	int m_nCORE;
	// CO2������,��λ:%,�Ŵ�100��,��������
	int m_nCO2RE;
	// NO������,��λ:%,�Ŵ�100��,��������
	int m_nNORE;
	// NO2������,��λ:%,�Ŵ�100��,��������
	int m_nNO2RE;
	// O2������,��λ:%,�Ŵ�100��,��������
	int m_nO2RE;

	// HC�������Ƿ�ͨ��
	bool m_bIsHCREPass;
	// CO�������Ƿ�ͨ��
	bool m_bIsCOREPass;
	// CO2�������Ƿ�ͨ��
	bool m_bIsCO2REPass;
	// NO�������Ƿ�ͨ��
	bool m_bIsNOREPass;
	// NO2�������Ƿ�ͨ��
	bool m_bIsNO2REPass;
	// O2�������Ƿ�ͨ��
	bool m_bIsO2REPass;

	// HCͨ���Ƿ�ͨ��
	bool m_bIsHCPass;
	// COͨ���Ƿ�ͨ��
	bool m_bIsCOPass;
	// CO2ͨ���Ƿ�ͨ��
	bool m_bIsCO2Pass;
	// NOͨ���Ƿ�ͨ��
	bool m_bIsNOPass;
	// NO2ͨ���Ƿ�ͨ��
	bool m_bIsNO2Pass;
	// O2ͨ���Ƿ�ͨ��
	bool m_bIsO2Pass;
	
	// ��Ӧʱ��
	float m_fHCRespTime;
	float m_fCORespTime;
	float m_fCO2RespTime;
	float m_fNORespTime;
	float m_fNO2RespTime;
	float m_fO2RespTime;

	// ����Ƿ�ͨ��
	bool m_bIsCheckPass;

private:
	short m_sRandomErrorHC;
	short m_sRandomErrorNO;
	short m_sRandomErrorNO2;
	short m_sRandomErrorNOX;
	float m_fRandomErrorCO;
	float m_fRandomErrorCO2;
	float m_fRandomErrorO2;
	float m_fRandomErrorPef;

	void RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx);
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;
};
