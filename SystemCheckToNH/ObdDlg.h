
#pragma once

#include "Resource.h"
#include "SystemCheckToNH.h"

// CObdDlg �Ի���

class CObdDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CObdDlg)

public:
	CObdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CObdDlg();

// �Ի�������
	enum { IDD = IDD_OBD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonObdOperation();

private:
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
};
