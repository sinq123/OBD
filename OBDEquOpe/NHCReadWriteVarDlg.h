/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCReadWriteVarDlg.h
* �ļ���ʶ��
* �������⹦����д�����ӿ�
*
* �汾��1.0
* ���ߣ�Qin
* ���ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.2
* ���ڣ�2015-06-10
* ���ߣ�Sun
* ����������⹦��д����BUG����,д��֮ǰ��д����
*
* �汾��1.0.1
* ���ڣ�2010-07-07
* ���ߣ�Cui
* ���������ձ�̹淶����ģ��
*
* �汾��1.0.0
* ���ڣ�2010-12-08
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
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
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

// CNHCReadWriteVarDlg �Ի���

class CNHCReadWriteVarDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCReadWriteVarDlg)

public:
	CNHCReadWriteVarDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCReadWriteVarDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_READ_WRITE_VAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonReadVar();
	afx_msg void OnBnClickedButtonWriteVar();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();

private:
	CEdit m_edNumberForWrite;
	CEdit m_edNumberForRead;
	// ����ֵ
	CEdit m_edValue;
	CListCtrl m_lcVar;
	// ����������
	CProgressCtrl m_pgRealAllVar;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;
	// ������Ϣ
	CShadeTitle m_shdTitle;

private:
	void InitCtrls(void);
	void SetDlgFont(void);
	
private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);

private:
	// �⹦�������߳�
	CDynThread * m_pDynThread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
};
