/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCResponseTime2Dlg.h
* �ļ���ʶ��
* �������⹦����Ӧʱ�����
*
* �汾��1.0
* ���ߣ�zhangwf
* ���ڣ�2016-09-14
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


#define ID_CTRL_PLOT_SCAN    WM_USER+1010

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
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
#include "LinePlot.h"
#include "NHCResponseTime2Thread.h"

// CNHCResponseTime2Dlg �Ի���

class CNHCResponseTime2Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCResponseTime2Dlg)

public:
	CNHCResponseTime2Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCResponseTime2Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_RESPONSE_TIME2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedBtnStartResponse();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();

public:
	virtual BOOL OnInitDialog();

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

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
	// ��ʼ�����ٶȵ��趨
	CEdit m_edtTopSilpSpeed;
	// �Ƿ񵽴ﻬ���ٶ��趨��
	bool m_bIsOverSilpOn;
	// ��Ӧʱ��
	CLabel m_lbResponse_Time;
	// �ȶ�ʱ��
	CLabel m_lbStabilization_Time ;

private:
	// �⹦���߳�
	//CDynThread * m_pDynThread;
	// �⹦�������߳�
	CNHCResponseTime2Thread * m_pCNHCResponseTime2Thread;

private:
	// ʵʱ��������ṹ
	struct SRTData
	{
		// ʱ�����
		int nTime;
		// ��Ͳ���ٶ�,��λ:km/h
		float fVelocity;
		// ��,��λ:N
		float fForce;

		SRTData(){ZeroMemory(this, sizeof(SRTData));}
	};	
	// ��������
	CList<SRTData, SRTData> m_lsRTDataOfDynResponseTime;

	// ���߻���λ��
	int m_nInedxPosition;

private:
	// ��Ӧʱ������趨����
	float m_fSpeed_High;
	float m_fSpeed_Low;
	float m_fLoad_High;
	float m_fLoad_Low;
	float m_fTopSlipSpeed;


	// ������������λ-kg
	USHORT m_usDIW;

private:
	// ��ʼ������
	void InitCurve();

	// ˢ������
	void RefreshCurve();

	void ControlEnableWindows(bool bEnable =1);
	// ������ʾ��
	POINTF  m_n_tempPoint[1], m_n_showPoint;
	// �ߵ���ɫ
	COLORREF m_crColor;

	bool IsCanRefreshCurve;
	int nPOINTF;

protected:
	CLinePlot m_stResponseTimeCurve;


private:
	// ���ݷ���
	void DataAnalysis();
	afx_msg LRESULT SaveRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResponseTimeTestFinished(WPARAM wParam, LPARAM lParam);
};
