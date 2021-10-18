#pragma once


// CNHSystemCheckDlg 对话框

class CNHSystemCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHSystemCheckDlg)

public:
	CNHSystemCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHSystemCheckDlg();

// 对话框数据
	enum { IDD = IDD_NH_SYSTEM_CHECK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CShadeTitle m_lbTitle;

private:
	void InitCtrls();// 初始化控件
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
public:
	afx_msg void OnBnClickedBtnSystemChechSelf();
	afx_msg void OnBnClickedBtnLeakUpload();
};
