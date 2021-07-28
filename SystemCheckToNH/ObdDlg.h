
#pragma once

#include "Resource.h"
#include "SystemCheckToNH.h"

// CObdDlg 对话框

class CObdDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CObdDlg)

public:
	CObdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CObdDlg();

// 对话框数据
	enum { IDD = IDD_OBD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonObdOperation();

private:
	CShadeTitle m_lbTitle;

private:
	// 初始化控件
	void InitCtrls();
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
};
