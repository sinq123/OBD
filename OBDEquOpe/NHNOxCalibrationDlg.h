/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHNOxCalibrationDlg.h
* �ļ���ʶ��
* ժҪ����������������У׼(NO,NO2ͨ��)
*
* �汾��1.0.0
* ���ߣ�Raylee
* ���ڣ�2018-05-14
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.0
* ���ڣ�2018-05-14
* ���ߣ�Raylee
* ����������ing
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


// CNHNOxCalibrationDlg �Ի���

class CNHNOxCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHNOxCalibrationDlg)

public:
	CNHNOxCalibrationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHNOxCalibrationDlg();

// �Ի�������
	enum { IDD = IDD_NHNOXCALDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedRadioChnCal();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

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

	// CO2ʵʱֵ
	CLabel m_lbCO2;
	// NOʵʱֵ
	CLabel m_lbNO;
	// NO2ʵʱֵ
	CLabel m_lbNO2;
	// NOxʵʱֵ
	CLabel m_lbNOx;

	// CO2���ֵ�༭��
	CNumericEdit m_edCO2NominalValue;
	// NO���ֵ�༭��
	CEdit m_edNONominalValue;
	// NO2���ֵ�༭��
	CEdit m_edNO2NominalValue;

	// ���±��ı��ؼ�
	CMathLabel m_mlb1;
	CMathLabel m_mlb2;
	CMathLabel m_mlb3;
	CMathLabel m_mlb4;
	CMathLabel m_mlb5;

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


private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �������
	void ImportParams(void);
	// ����������ʵʱ����
	void ExportGasCalibrationRTData(void);
	// ������������(�ڲ�����)
	void ExportGasCalibrationResult(void);
	
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ����У׼����
	void Process(void);
	// ����У׼����
	void ResetProcess(void);
	// ��ȡ��������ֵ����
	// ����0x00,���ֵ�������Ҫ��
	// ��������,���ֵ���벻��Ҫ��
	DWORD GetNominalValue(void);
	// ��¼���ֵ�������ļ�
	void SaveNominalValueToIniFile(void);
	// ���ʵʱ��ʾ��
	void ClearRTDisplay(void);
	// ��������
	void ZeroProcess(void);
	// У׼����
	void CalibrationProcess(void);



private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// CO2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	int m_nCO2MeasuredValue;
	// NO����ֵ,��λ:ppm
	int m_nNOMeasuredValue;
	// NO2����ֵ,��λ:ppm
	int m_nNO2MeasuredValue;
	 
	// ��׼��CO2���ֵ,��λ:0.001%(�ŵ�1000��)
	int m_nCO2NominalValue;
    // ��׼��NO���ֵ,��λ:ppm
	int m_nNONominalValue;
    // ��׼��NO2���ֵ,��λ:ppm
	int m_nNO2NominalValue;
	
	// У׼����׼
	// JJG668/GB18285/GB3847
	int m_nStandardGas1;
	int m_nStandardGas2;
	int m_nStandardGas3;
	int m_nStandardGas4;
	int m_nStandardGasCur;

	// У׼ͨ��
	//0x01 У׼CO2
	//0x02 У׼NO
	//0x03 У׼NO,CO2
	//0x04 У׼NO2
	int m_nCalChn;

	// �Ƿ��ѯ����״̬
	bool m_bCheckZeroStatus;

	// ����
	int m_nStep;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;
	// У׼����Ƿ�Pass
	bool m_bIsCalibrationPass;

private:
	// �������߳�
	CAnaThread *m_pAnaThread;
};
