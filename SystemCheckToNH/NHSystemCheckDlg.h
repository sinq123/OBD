#pragma once


// CNHSystemCheckDlg �Ի���

class CNHSystemCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHSystemCheckDlg)

public:
	CNHSystemCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHSystemCheckDlg();

// �Ի�������
	enum { IDD = IDD_NH_SYSTEM_CHECK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CShadeTitle m_lbTitle;

private:
	void InitCtrls();// ��ʼ���ؼ�
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
public:
	afx_msg void OnBnClickedBtnSystemChechSelf();
	afx_msg void OnBnClickedBtnLeakUpload();
};
