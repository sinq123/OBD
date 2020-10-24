/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHNOxCheckDlg.h
* �ļ���ʶ��
* ժҪ������������������(NO,NO2ͨ��)
*
* ��ǰ�汾��1.0.0
* ���ߣ�Raylee
* ������ڣ�2018-05-16
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2018-05-16
* ���ߣ�Raylee
* ����������ing
*
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


// CNHNOxCheckDlg �Ի���
class CNHNOxCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHNOxCheckDlg)

public:
	CNHNOxCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHNOxCheckDlg();

// �Ի�������
	enum { IDD = IDD_NHNOXCHECKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedRadioChnChk();
	afx_msg void OnBnClickedRadioCheckType();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�begin
	// ������������������
	void ImportGasCheckParams(void);
	// ������������(�б궨��)
	void ExportEquCalChkInfo(void);
	// �����궨��־(�ڲ�����)
	void ExportDemarcationLog(void);
	// ������������(�ڲ�����)
	void ExportGasCheckResult(void);
	// ����������ʵʱ����
	void ExportGasCheckRTData(void);
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
	void CalculateResultCO2(void);
	void CalculateResultNO(void);
	void CalculateResultNO2(void);
	// ��ʾ���
	void ShowResultCO2(void);
	void ShowResultNO(void);
	void ShowResultNO2(void);
	void ShowJudgement(void);
	// ��ֵ����
	void SetLimit(void);
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
	CMathLabel m_mlb7;
	// ���±��ı��ؼ�
	CMathLabel m_mlb8;
	// ���±��ı��ؼ�
	CMathLabel m_mlb9;

	// CO2ʵʱֵ
	CLabel m_lbCO2;
	// NOʵʱֵ
	CLabel m_lbNO;
	// NO2ʵʱֵ
	CLabel m_lbNO2;
	// NOxʵʱֵ
	CLabel m_lbNOx;

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

	// CO2���ֵ�༭��
	CNumericEdit m_edCO2NominalValue;
	// NO���ֵ�༭��
	CEdit m_edNONominalValue;
	// NO2���ֵ�༭��
	CEdit m_edNO2NominalValue;
		
	// CO2����ֵ��ʾ��
	CLabel m_lbCO2MeasuredValue;
	// NO����ֵ��ʾ��
	CLabel m_lbNOMeasuredValue;
	// NO2����ֵ��ʾ��
	CLabel m_lbNO2MeasuredValue;
	
	// CO2���ֵ��ʾ��
	CLabel m_lbCO2NominalValue;
	// NO���ֵ��ʾ��
	CLabel m_lbNONominalValue;
	// NO2���ֵ��ʾ��
	CLabel m_lbNO2NominalValue;

	// CO2���������ʾ��
	CLabel m_lbCO2AE;
	// NO���������ʾ��
	CLabel m_lbNOAE;
	// NO2���������ʾ��
	CLabel m_lbNO2AE;
		
	// CO2��������ʾ��
	CLabel m_lbCO2RE;
	// NO��������ʾ��
	CLabel m_lbNORE;
	// NO2��������ʾ��
	CLabel m_lbNO2RE;
	
	// CO2�ж���ʾ��
	CLabel m_lbCO2Judgement;
	// NO�ж���ʾ��
	CLabel m_lbNOJudgement;
	// NO2�ж���ʾ��
	CLabel m_lbNO2Judgement;
	
	// CO2��Ӧʱ��
	CLabel m_lbCO2RespTime;
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

	// ���ͨ��
	// 1��CO2
	// 2��NO
	// 3��NO2
	// 4��CO2,NO
	int m_nChannel;
	// ��׼��Ũ��
	// 1����Ũ��
	// 2���е�Ũ��
	// 3���и�Ũ��
	// 4����Ũ��
	int m_nCheckType;
	
	// ʵʱ��������ṹ
	struct RTData
	{
		// ʱ�����
		int nTime;
		// CO2ֵ,��λ:0.01%
		float fCO2;
		// NOֵ,��λ:ppm
		short sNO;
		// NO2ֵ,��λ:ppm
		short sNO2;
		// ����
		USHORT usFlux;
		// ʱ��
		SYSTEMTIME st;

		RTData(){ZeroMemory(this,sizeof(RTData));}
	};	
	// ��������
	CList<RTData, RTData> m_lsRTDataOfNOx;

private:
    // ��׼��CO2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCO2NominalValue;
	// ��׼��NO���ֵ,��λ:ppm
	int m_nNONominalValue;
	// ��׼��NO2���ֵ,��λ:ppm
	int m_nNO2NominalValue;

private:	
	// CO2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AELimit;
	// CO2��������ֵ,��λ:%
	int m_nCO2RELimit;
	// NO���������ֵ,��λ:ppm
	int m_nNOAELimit;
	// NO��������ֵ,��λ:%
	int m_nNORELimit;
	// NO2���������ֵ,��λ:ppm
	int m_nNO2AELimit;
	// NO��������ֵ,��λ:%
	int m_nNO2RELimit;
	
	// ��Ӧʱ��Ҫ��
	float m_fCO2T90Limit;
	float m_fCO2T10Limit;
	float m_fNOT90Limit;
	float m_fNOT10Limit;
	float m_fNO2T90Limit;
	float m_fNO2T10Limit;

private:
	// CO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2MeasuredValue;
	// CO2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AE;
	// CO2��������Ƿ�ͨ��
	bool m_bIsCO2AEPass;
	// CO2������,��λ:%,�Ŵ�100��,��������
	int m_nCO2RE;
	// CO2�������Ƿ�ͨ��
	bool m_bIsCO2REPass;
	// CO2ͨ���Ƿ�ͨ��
	bool m_bIsCO2Pass;

	// NO����ֵ,��λ:ppm
	int m_nNOMeasuredValue;
	// NO�������,��λ:ppm
	int m_nNOAE;
	// NO��������Ƿ�ͨ��
	bool m_bIsNOAEPass;
	// NO������,��λ:%,�Ŵ�100��,��������
	int m_nNORE;
	// NO�������Ƿ�ͨ��
	bool m_bIsNOREPass;
	// NOͨ���Ƿ�ͨ��
	bool m_bIsNOPass;

	// NO2����ֵ,��λ:ppm
	int m_nNO2MeasuredValue;
	// NO2�������,��λ:ppm
	int m_nNO2AE;
	// NO2��������Ƿ�ͨ��
	bool m_bIsNO2AEPass;
	// NO2������,��λ:%,�Ŵ�100��,��������
	int m_nNO2RE;
	// NO2�������Ƿ�ͨ��
	bool m_bIsNO2REPass;
	// NO2ͨ���Ƿ�ͨ��
	bool m_bIsNO2Pass;
	
	// ��Ӧʱ��
	float m_fCO2RespTime;
	float m_fNORespTime;
	float m_fNO2RespTime;

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

	void RandomError(short& sHC, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx);
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;

	void Precomputation(int& nMeasuredValue, const int& nNominalValue, const int& nAELimit);
};
