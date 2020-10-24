// SelfInsrectionItemsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SGSChe.h"
#include "SelfInsrectionItemsDlg.h"
#include "afxdialogex.h"

#define HORZ_PTS 8
#define VERT_PTS 4

////////////////////////////////////////////////////////////////////////////
// CGripper message map
BEGIN_MESSAGE_MAP(CGripper, CScrollBar)
	//{{AFX_MSG_MAP(CGripper)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGripper message handlers
LRESULT CGripper::OnNcHitTest(CPoint point) 
{
	UINT ht = CScrollBar::OnNcHitTest(point);
	if(ht==HTCLIENT)
	{
		ht = HTBOTTOMRIGHT;
	}
	return ht;
}

// CSelfInsrectionItemsDlg 对话框

IMPLEMENT_DYNAMIC(CSelfInsrectionItemsDlg, CDialogZoom)

CSelfInsrectionItemsDlg::CSelfInsrectionItemsDlg(UINT nIDD,CWnd* pParent /*=NULL*/)
	: CDialogZoom(nIDD, pParent)
{
	m_bInitialized = FALSE;
}

CSelfInsrectionItemsDlg::~CSelfInsrectionItemsDlg()
{
}

void CSelfInsrectionItemsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_scroll);
}


BEGIN_MESSAGE_MAP(CSelfInsrectionItemsDlg, CDialogZoom)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CSelfInsrectionItemsDlg 消息处理程序


BOOL CSelfInsrectionItemsDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetScroll();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelfInsrectionItemsDlg::SetScroll(void)
{
	// Set Initial Scroll Positions
	m_nHscrollPos = 0;
	m_nVscrollPos = 0;

	// Get the initial dimensions of the dialog
	GetClientRect(&m_ClientRect);
	m_bInitialized = TRUE;

	// Create a resize gripper
	CRect initRect;
	GetClientRect(initRect);
	initRect.left = initRect.right - GetSystemMetrics(SM_CXHSCROLL);
	initRect.top  = initRect.bottom - GetSystemMetrics(SM_CYVSCROLL);

	DWORD dwStyle = WS_CHILD | 
		SBS_SIZEBOX | 
		SBS_SIZEBOXBOTTOMRIGHTALIGN | 
		SBS_SIZEGRIP | 
		WS_VISIBLE;

	m_Grip.Create(dwStyle,initRect, this, AFX_IDW_SIZE_BOX);
// 
// 	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR1);
// 	pScrollBar->SetScrollRange(0,100);//滑块移动的位置为0——100；
}

void CSelfInsrectionItemsDlg::ResetScrollbars()
{
	// Reset our window scrolling information
	ScrollWindow(m_nHscrollPos*HORZ_PTS, 0, NULL, NULL);
	ScrollWindow(0, m_nVscrollPos*VERT_PTS, NULL, NULL);
	m_nHscrollPos = 0;
	m_nVscrollPos = 0;
	SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
}


void CSelfInsrectionItemsDlg::SetupScrollbars()
{
	CRect tempRect;
	GetClientRect(&tempRect);

	// Calculate how many scrolling increments for the client area
	m_nHorzInc = (m_ClientRect.Width()  - tempRect.Width())/HORZ_PTS;
	m_nVertInc = (m_ClientRect.Height() - tempRect.Height())/VERT_PTS;

	// Set the vertical and horizontal scrolling info
	m_nHscrollMax = max(0, m_nHorzInc);
	m_nHscrollPos = min (m_nHscrollPos, m_nHscrollMax);
	SetScrollRange(SB_HORZ, 0, m_nHscrollMax, FALSE);
	SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);
	m_nVscrollMax = max(0, m_nVertInc);
	m_nVscrollPos = min(m_nVscrollPos, m_nVscrollMax);
	SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);

}

void CSelfInsrectionItemsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int TempPos = pScrollBar->GetScrollPos();
	switch(nSBCode)
	{
	case SB_THUMBPOSITION://拖动滑块
		pScrollBar->SetScrollPos(nPos);
		break;
	case SB_LINELEFT://点击左边的箭头
		if(TempPos > 1)
		{
			TempPos--;
		}
		pScrollBar->SetScrollPos(TempPos);
		break;
	case SB_LINERIGHT://点击右边的箭头
		if(TempPos<100)
		{
			TempPos++;
		}
		pScrollBar->SetScrollPos(TempPos);
		break;
	} 
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSelfInsrectionItemsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nInc;
	switch (nSBCode)
	{
	case SB_TOP:        nInc = -m_nVscrollPos;               break;
	case SB_BOTTOM:     nInc = m_nVscrollMax-m_nVscrollPos;  break;
	case SB_LINEUP:     nInc = -1;                           break;
	case SB_LINEDOWN:   nInc = 1;                            break;
	case SB_PAGEUP:     nInc = min(-1, -m_nVertInc);         break;
	case SB_PAGEDOWN:   nInc = max(1, m_nVertInc);           break;
	case SB_THUMBTRACK: nInc = nPos - m_nVscrollPos;         break;
	default:            nInc = 0;
	}

	nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
	m_nVscrollPos += nInc;
	int iMove = -VERT_PTS * nInc;
	ScrollWindow(0, iMove, NULL, NULL);
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);



// 	int nMax,nMin;
// 	nMin = nMax = 0;
// 	int TempPos = pScrollBar->GetScrollPos();
// 	switch(nSBCode)
// 	{
// 	case SB_THUMBPOSITION://拖动滑块
// 		pScrollBar->SetScrollPos(nPos);
// 		break;
// 	case SB_LINELEFT://点击左边的箭头
// 		if(TempPos > 1)
// 		{
// 			TempPos--;
// 		}
// 		pScrollBar->SetScrollPos(TempPos);
// 		break;
// 	case SB_LINERIGHT://点击右边的箭头
// 		if(TempPos<100)
// 		{
// 			TempPos++;
// 		}
// 		pScrollBar->SetScrollPos(TempPos);
// 		break;
// 	} 


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSelfInsrectionItemsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bInitialized)
	{
		ResetScrollbars();
		SetupScrollbars();
	}
}