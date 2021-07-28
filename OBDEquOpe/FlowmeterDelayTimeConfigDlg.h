/*
* Copyright (c) 2011,��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�FlowmeterDelayTimeConfigDlg.h
* �ļ���ʶ��
* ժҪ��Sensor������ͨ����ʱ����
*
* ��ǰ�汾��1.0
* ���ߣ�Ming
* ������ڣ�2009-12-22
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2011-03-011
* ���ߣ�Ming
* ��������ӻָ�Ĭ�����ù���
*
* ��ǰ�汾��1.0.0
* ������ڣ�2011-03-03
* ���ߣ�Ming
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


// CFlowmeterDelayTimeConfigDlg �Ի���

class CFlowmeterDelayTimeConfigDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterDelayTimeConfigDlg)

public:
	CFlowmeterDelayTimeConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlowmeterDelayTimeConfigDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLOWMETER_DELAYTIME_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedBtnHccoco2Read();
	afx_msg void OnBnClickedBtnO2Read();
	afx_msg void OnBnClickedBtnNoxRead();
	afx_msg void OnBnClickedBtnHccoco2Write();
	afx_msg void OnBnClickedBtnO2Write();
	afx_msg void OnBnClickedBtnNoxWrite();
	afx_msg void OnBnClickedBtnDefaultConfig();

private:
	// ������
	CShadeTitle m_lbTitle;
	// ��Ϣ��ʾ��
	CLabel m_lbInfo;

	// ����ʵʱֵ��ʾ��
	CLabel m_lbFlux;

	// HC/CO/CO2У׼ֵ�༭��
	CNumericEdit m_neHCCOCO2Value;
	// O2У׼ֵ�༭��
	CNumericEdit m_neO2Value;
	// NOxУ׼ֵ�༭��
	CNumericEdit m_neNOxValue;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �Ƿ��������
	void EnableOperation(bool bEnable=true);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �������߳�
	CAnaThread *m_pFloThread;
};
