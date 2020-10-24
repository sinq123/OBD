/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�ParameterEnvParamsCalibrationDlg.h
* �ļ���ʶ��
* �����������������Դ����������Ǽ����´�����У׼
*
* �汾��1.0
* ���ߣ�Ming
* ���ڣ�2010-08-27
*
* ��ʷ��Ϣ��
*
* �汾��1.0.1
* ���ڣ�2011-05-04
* ���ߣ�Ming
* ��������ӻָ�Ĭ�����ð�ť�����ò�������
*
* �汾��1.0.0
* ���ڣ�2010-08-27
* ���ߣ�Ming
* ������beta
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
#pragma comment( lib, "..\\Debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\Release\\OBDEquThr.lib" )
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


// CParameterEnvParamsCalibrationDlg �Ի���

class CParameterEnvParamsCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CParameterEnvParamsCalibrationDlg)

public:
	CParameterEnvParamsCalibrationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParameterEnvParamsCalibrationDlg();

// �Ի�������
	enum { IDD = IDD_PARAMETER_ENVPARAMS_CALIBRATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedButtonEtCal();
	afx_msg void OnBnClickedButtonRhCal();
	afx_msg void OnBnClickedButtonApCal();
	afx_msg void OnBnClickedButtonOtCal();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonDefaultConfig();
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
	// ����4
	CLabel m_lbLabel4;

	// �����¶Ȳ���ֵ�༭��
	CEdit m_edETMea;
	// ���ʪ�Ȳ���ֵ�༭��
	CEdit m_edRHMea;
	// ����ѹ������ֵ�༭��
	CEdit m_edAPMea;
	// ���²���ֵ�༭��
	CEdit m_edOTMea;

	// �����¶ȱ�׼ֵ�༭��
	CNumericEdit m_neETStd;
	// ���ʪ�ȱ�׼ֵ�༭��
	CNumericEdit m_neRHStd;
	// ����ѹ����׼ֵ�༭��
	CNumericEdit m_neAPStd;
	// ���±�׼ֵ�༭��
	CNumericEdit m_neOTStd;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ˢ������
	void RefreshData(void);
	// ʹˢ������
	void EnableRefreshData(bool bEnable = true);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ����ˢ�±�־
	bool m_bRefreshDataSign;

private:
	// �����������߳�
	CParaThread *m_pParaThread;
};
