/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCResponseTimeDlg.h
* �ļ���ʶ��
* �������⹦����Ӧʱ�����
*
* �汾��1.0
* ���ߣ�Qin
* ���ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
* �汾��1.0.1
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��

* �汾��1.0.0
* ���ڣ�2009-12-08
* ���ߣ�Qin
* ��������ʽ����ʹ��
*/
#pragma once

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
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
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

// CNHCResponseTimeDlg �Ի���

class CNHCResponseTimeDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCResponseTimeDlg)

public:
	CNHCResponseTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCResponseTimeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_RESPONSE_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedBtnStartResponse();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedRadioTest();

public:
	virtual BOOL OnInitDialog();

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ˢ��ʵʱ����
	void EnableRefreshRealTimeData(bool bEnable=true);
	// ˢ��ʵʱ����
	void RefreshRealTimeData(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// ��ʾ����
	CShadeTitle m_shdTitle;
	// ��ʾ�ٶ�
	CLabel m_lbSpeed;
	// ��ʾ��
	CLabel m_lbTraction;
	// ��ʾ����
	CLabel m_lbPower;
	// ��ʾ��Ϣ
	CLabel m_lbMsg;
	// ���ٵ��趨
	CEdit m_edtSpeed_High;
	// ���ٵ��趨
	CEdit m_edtSpeed_Low;
	// ���ٵ�������趨
	CEdit m_edtLoadTraction_Low;
	// ���ٵ�������趨
	CEdit m_edtLoadTraction_High;
	// 90%Ŀ�������
	CEdit m_edtLoadTraction_90;
	// ���ٵ���ع����趨
	CEdit m_edtPower_High;
	// ���ٵ���ع����趨
	CEdit m_edtPower_Low;
	// ��ʼ�����ٶȵ��趨
	CEdit m_edtTopSilpSpeed;
	// ��Ӧʱ��
	CLabel m_lbResponse_Time;
	// �ȶ�ʱ��
	CLabel m_lbStabilization_Time;

private:
	// �Ƿ񵽴ﻬ���ٶ��趨��
	bool m_bIsOverSilpOn;
	// �⹦���߳�
	CDynThread * m_pDynThread;

private:
	// ��Ӧʱ������趨����
	float m_fSpeed_High;
	float m_fSpeed_Low;
	float m_fLoad_High;
	float m_fLoad_Low;
	float m_fTopSlipSpeed;

};
