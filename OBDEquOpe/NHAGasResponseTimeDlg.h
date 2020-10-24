/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHAGasResponseTime.h
* �ļ���ʶ��
* ժҪ��������������Ӧʱ��
*
* ��ǰ�汾��1.0.0
* ���ߣ�Raylee
* ������ڣ�2019-05-06
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ʱ�䣺2019-05-06
* ���ߣ�Raylee
* ����������ing
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


// CNHAGasResponseTimeDlg �Ի���
class CNHAGasResponseTimeDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHAGasResponseTimeDlg)

public:
	CNHAGasResponseTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHAGasResponseTimeDlg();

// �Ի�������
	enum { IDD = IDD_NHA_GAS_RESPONSE_TIME };

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
	// ������
	void CalculateResult(void);
	// ��ʾ���
	void ShowResult(void);
	// ��ֵ����
	void SetLimit(void);
	// ������Ӧʱ��
	void CalculateRespTimeT90(void);
	void CalculateRespTimeT10(void);

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

	//// ���±��ı��ؼ�
	//CMathLabel m_mlb1;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb2;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb3;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb4;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb5;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb6;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb8;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb12;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb13;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb14;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb15;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb16;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb17;
	//// ���±��ı��ؼ�
	//CMathLabel m_mlb18;
	//CMathLabel m_mlb19;

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
	// ����5
	CLabel m_lbStep5;

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

	// HC�ж���ʾ��
	CLabel m_lbHCJudgement;
	// CO�ж���ʾ��
	CLabel m_lbCOJudgement;
	// CO2�ж���ʾ��
	CLabel m_lbCO2Judgement;
	// NO�ж���ʾ��
	CLabel m_lbNOJudgement;
	// NO2�ж���ʾ��
	CLabel m_lbNO2Judgement;
	// O2�ж���ʾ��
	CLabel m_lbO2Judgement;

	CLabel m_lbHCT90Limit;
	CLabel m_lbCOT90Limit;
	CLabel m_lbCO2T90Limit;
	CLabel m_lbNOT90Limit;
	CLabel m_lbNO2T90Limit;
	CLabel m_lbO2T90Limit;

	CLabel m_lbHCT10Limit;
	CLabel m_lbCOT10Limit;
	CLabel m_lbCO2T10Limit;
	CLabel m_lbNOT10Limit;
	CLabel m_lbNO2T10Limit;
	CLabel m_lbO2T10Limit;

	// HC��Ӧʱ��
	CLabel m_lbHCT90RespTime;
	// CO��Ӧʱ��
	CLabel m_lbCOT90RespTime;
	// CO2��Ӧʱ��
	CLabel m_lbCO2T90RespTime;
	// NO��Ӧʱ��
	CLabel m_lbNOT90RespTime;
	// NO2��Ӧʱ��
	CLabel m_lbNO2T90RespTime;
	// O2��Ӧʱ��
	CLabel m_lbO2T90RespTime;

	// HC��Ӧʱ��
	CLabel m_lbHCT10RespTime;
	// CO��Ӧʱ��
	CLabel m_lbCOT10RespTime;
	// CO2��Ӧʱ��
	CLabel m_lbCO2T10RespTime;
	// NO��Ӧʱ��
	CLabel m_lbNOT10RespTime;
	// NO2��Ӧʱ��
	CLabel m_lbNO2T10RespTime;
	// O2��Ӧʱ��
	CLabel m_lbO2T10RespTime;
	
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
		// Pefֵ
		float fPef;
		// ʱ��
		SYSTEMTIME st;

		RTData(){ZeroMemory(this,sizeof(RTData));}
	};	
	// ��������
	std::vector<RTData> m_vtRTDataT90;
	std::vector<RTData> m_vtRTDataT10;

private:
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
	float m_fHCT90RespTime;
	float m_fCOT90RespTime;
	float m_fCO2T90RespTime;
	float m_fNOT90RespTime;
	float m_fNO2T90RespTime;
	float m_fO2T90RespTime;

	float m_fHCT10RespTime;
	float m_fCOT10RespTime;
	float m_fCO2T10RespTime;
	float m_fNOT10RespTime;
	float m_fNO2T10RespTime;
	float m_fO2T10RespTime;

	// ����Ƿ�ͨ��
	bool m_bIsCheckPass;

};
