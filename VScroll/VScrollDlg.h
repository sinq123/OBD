
// VScrollDlg.h : 头文件
//

#pragma once


// CVScrollDlg 对话框
class CVScrollDlg : public CDialogEx
{
// 构造
public:
	CVScrollDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CVScrollDlg(void);
// 对话框数据
	enum { IDD = IDD_VSCROLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton* m_pBtns;

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
