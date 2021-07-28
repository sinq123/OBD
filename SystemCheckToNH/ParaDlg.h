
#pragma once

#include "Resource.h"
#include "SystemCheckToNH.h"

// CParaDlg 对话框

class CParaDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CParaDlg)

public:
	CParaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaDlg();

// 对话框数据
	enum { IDD = IDD_PARA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonEnvParamsCal();

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
