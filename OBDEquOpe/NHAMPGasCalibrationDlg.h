/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHAMPGasCalibrationDlg.h
* �ļ���ʶ��
* ժҪ������У׼(������������)
*
* ��ǰ�汾��1.0.10
* ���ߣ�Hyh
* ������ڣ�2010-07-05
*
* ��ʷ��Ϣ��
*
* �汾��1.0.13
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* ��ǰ�汾��1.0.12
* ������ڣ�2010-09-29
* ���ߣ�Raylee
* ������1���޸Ľ���������С
*		2�����CO2��O2�ؼ����±���ʾ
*
* ��ǰ�汾��1.0.11
* ������ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.10
* ������ڣ�2010-07-05
* ���ߣ�Hyh
* ���������ݷ�����Э��3.6�汾,��ӷ����ǰ汾��־λ,���ݲ�ͬ�汾���з����ǵ���
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-07-01
* ���ߣ�Ming
* ������ȥ��ExportEquCalChkInfo��ExportDemarcationLog�������������Ͷ���
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-05-26
* ���ߣ�Raylee
* ������ֻ��������У׼�������
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* ������1���ص���ʾУ׼������ΪУ׼���
*		2���ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.6
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ���������ù��̿��ƻص�����������Ӧ����״̬
*
* ��ǰ�汾��1.0.5
* ������ڣ�2010-05-12
* ���ߣ�Raylee
* ������дУ׼������ݸ��ûص�����
*
* ��ǰ�汾��1.0.4
* ʱ�䣺2010-04-09
* ���ߣ�Raylee
* �������ĵ����궨��־�ͼ��������
*
* ��ǰ�汾��1.0.3
* ʱ�䣺2010-01-13
* ���ߣ�Cui
* ������1)����Equipment.ini�ļ��ĸ��µ�������
*
* ��ǰ�汾��1.0.2
* ʱ�䣺2010-01-05
* ���ߣ�Cui
* ������1)��ӷ��������ӷ�ʽ������ʶ���Ƿ������������ӣ�����ǣ���Ҫִ���л���������ͨѶģʽ����
*       2)�޸Ľ������塰��������ֵ���á�Ϊ��У׼�����ֵ���á�
*
* ��ǰ�汾��1.0.1
* ������ڣ�2009-12-08
* ���ߣ�Cui
* ���������·������߳�
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-11-26
* ���ߣ�Cui
* ��������ʽ����ʹ��
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


// CNHAMPGasCalibrationDlg �Ի���

class CNHAMPGasCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHAMPGasCalibrationDlg)

public:
	CNHAMPGasCalibrationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHAMPGasCalibrationDlg();

// �Ի�������
	enum { IDD = IDD_NHA_MP_CAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedRadioStandard();
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
	// NOʵʱֵ
	CLabel m_lbNO;
	// NO2ʵʱֵ
	CLabel m_lbNO2;
	// PEFʵʱֵ
	CLabel m_lbPEF;

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

	// ���±��ı��ؼ�
	CMathLabel m_mlb1;
	CMathLabel m_mlb2;
	CMathLabel m_mlb3;
	CMathLabel m_mlb4;
	CMathLabel m_mlb5;
	CMathLabel m_mlb6;
	CMathLabel m_mlb7;
	CMathLabel m_mlb8;
	CMathLabel m_mlb9;
	CMathLabel m_mlb10;

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
	// PEF����ֵ,�Ŵ�1000��,��������
	int m_nPEFMeasuredValue;
	// HC����ֵ,��λ:ppm
	int m_nHCMeasuredValue;
	// CO����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	int m_nCOMeasuredValue;
	// CO2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	int m_nCO2MeasuredValue;
	// O2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	int m_nO2MeasuredValue;
	// NO����ֵ,��λ:ppm
	int m_nNOMeasuredValue;
	// NO2����ֵ,��λ:ppm
	int m_nNO2MeasuredValue;

    // ��׼��C3H8���ֵ,��λ:ppm
	int m_nC3H8NominalValue;
    // ��׼��CO���ֵ,��λ:0.001%(�ŵ�1000��)
	int m_nCONominalValue;
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
	//0x01 У׼HC,CO,CO2
	//0x02 У׼NO
	//0x04 У׼NO2
	//0x03 У׼HC,CO,CO2,NO
	//0x05 У׼HC,CO,CO2,NO2
	//0x06 У׼NO,NO2
	//0x07 У׼HC,CO,CO2,NO,NO2
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
