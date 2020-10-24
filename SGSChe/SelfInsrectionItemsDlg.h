#pragma once


// CSelfInsrectionItemsDlg 对话框

class CGripper : public CScrollBar
{
	// Construction
public:
	CGripper(){};
	// Implementation
public:
	virtual ~CGripper(){};
	// Generated message map functions
protected:
	//{{AFX_MSG(CGripper)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CSelfInsrectionItemsDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CSelfInsrectionItemsDlg)

public:
	CSelfInsrectionItemsDlg(UINT nIDD, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelfInsrectionItemsDlg();

// 对话框数据
	//enum { IDD = IDD_SELF_INSRECTION_ITEMS_DIALOG };

	void SetupScrollbars();
	void ResetScrollbars();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	CScrollBar m_scroll;

	int  m_nHorzInc, m_nVertInc,
		m_nVscrollMax, m_nHscrollMax,
		m_nVscrollPos, m_nHscrollPos;
	CBrush   m_transbrush;
	CRect m_ClientRect;
	BOOL m_bInitialized;
	CGripper m_Grip;

private:
	// 设置滚动条
	void SetScroll(void);
};
