/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCDIWThread.h
* �ļ���ʶ��
* ժҪ���⹦�������������в���
*
* ��ǰ�汾��1.0.1
* ���ߣ�Qin
* ������ڣ�2009-11-22
*
* ��ʷ��Ϣ��
*
* �汾��1.0.1
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
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

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
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
#include "NHCDIWThread.h"

// CNHCDIWDlg �Ի���

class CNHCDIWDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCDIWDlg)

public:
	CNHCDIWDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCDIWDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_DIW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStartTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonWriteDiw();

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ˢ��ʵʱ����
	void EnableRefreshRealTimeData(bool bEnable=true);
	void RefreshRealTimeData(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ��ʾ����������������
	void DisplayDIWTestProcess(void);
	// �����������
	void CalculateDIW(void);

private:
	//////////////////////////////////////////////////////////////////////////////////�ؼ�����begin
	// ������Ϣ
	CShadeTitle m_shdTitle;
	// ��ʾ��Ϣ��ʾ��
	CLabel m_lbInfo;
	// ������ʾ��
	CLabel m_lbP;
	// ����ʾ��
	CLabel m_lbF;
	// �ٶ���ʾ��
	CLabel m_lbV;
	// ��ǩ1(ʵʱֵ)
	CLabel m_lb1;
	// ��ǩ2
	CLabel m_lb2;
	// ��ǩ3
	CLabel m_lb3;
	// ��ǩ4
	CLabel m_lb4;
	// ����ʱ����ʾ��(С������)
	CLabel m_lbLowerSlipingTime;
	// ����ʱ����ʾ��(�������)
	CLabel m_lbUpperSlipingTime;
	// ʵ��ƽ����������ʾ��(С������)
	CLabel m_lbAveLowerForce;
	// ʵ��ƽ����������ʾ��(�������)
	CLabel m_lbAveUpperForce;
	// ������������ֵ��ʾ��
	CLabel m_lbMeasuredDIW;
	CEdit m_edLowerForce;
	CEdit m_edUpperForce;
	CEdit m_edLowerVelocity;
	CEdit m_edUpperVelocity;
	//////////////////////////////////////////////////////////////////////////////////�ؼ�����en

private:
	// �⹦�����غɿ����߳�
	CNHCDIWThread *m_pNHCDIWThread;
	// ��ƻ�����������λ-kg
	USHORT m_usDIW;
	// ʵ���������,��λ:kg
	USHORT m_usMeasuredDIW;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// С������,��λ-N
	float m_fLowerForce;
	// �������,��λ-N
	float m_fUpperForce;
	// ��������,���ٵ�,��λ:km/h
	float m_fLowerVelocity;
	// ��������,���ٵ�,��λ:km/h
	float m_fUpperVelocity;

	// ����ʱ��(С������),��λ:ms
	UINT m_unLowerSlipingTime;
	// ����ʱ��(�������),��λ:ms
	UINT m_unUpperSlipingTime;
	// ʵ��С������ƽ��ֵ,��λ-N
	float m_fAveLowerForce;
	// ʵ��������ƽ��ֵ,��λ-N
	float m_fAveUpperForce;

	// �ݴ�Ap,Bp,Cp,������:39,40,41
	WORD m_wAp;
	WORD m_wBp;
	WORD m_wCp;
};
