/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FlowmeterRealTimeDataDlg.h
* �ļ���ʶ��
* ժҪ��������ʵʱ������ʾ
*
* ��ǰ�汾��1.0
* ���ߣ�Lihy
* ������ڣ�2014-07-18
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.0
* ������ڣ�2014-07-18
* ���ߣ�Lihy
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
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
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


// CFlowmeterRealTimeDataDlg �Ի���

class CFlowmeterRealTimeDataDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterRealTimeDataDlg)

public:
	CFlowmeterRealTimeDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlowmeterRealTimeDataDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLOWMETER_REALTIME_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	// ������
	CShadeTitle m_lbTitle;
	// ��Ϣ��ʾ��
	CLabel m_lbInfo;

	// ��׼����
	CLabel m_lbStd0Flux;
	CLabel m_lbStd20Flux;
	CLabel m_lbActFlux;
	// �¶�
	CLabel m_lbTemp;
	// ѹ��
	CLabel m_lbPres;
	// ϡ����
	CLabel m_lbUDilO2;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �Ƿ������������(���ط����)
	void EnableBasicOperation(bool bEnable=true);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �������߳�
	CAnaThread *m_pFloThread;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFanOn();
	afx_msg void OnBnClickedButtonFanOff();
	afx_msg void OnBnClickedButtonQuit();
};
