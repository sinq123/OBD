/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCCommonOperationDlg.h
* �ļ���ʶ��
* �������⹦�����ò���
*
* �汾��1.0
* ���ߣ�Qin
* ���ڣ�2010-06-17
*
* ��ʷ��Ϣ��
*
* �汾��1.0.2
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.1
* ���ڣ�2010-07-07
* ���ߣ�Cui
* ������1)���ձ�̹淶����ģ��
*       2)�Ľ�״̬��ʾ
*
* �汾��1.0.0
* ���ڣ�2010-06-17
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

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include "resource.h"		// ������

// CNHCCommonOperationDlg �Ի���

class CNHCCommonOperationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCCommonOperationDlg)

public:
	CNHCCommonOperationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCCommonOperationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_COMMON_OPERATION };

protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonViewState();
	afx_msg void OnBnClickedButtonQuit();

private:
	// ��ʾ��Ϣ
	CLabel m_lbInfo;
	// ����
	CShadeTitle m_shdTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);

private:
	// �⹦���߳�
	CDynThread *m_pDynThread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
};
