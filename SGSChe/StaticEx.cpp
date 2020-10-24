// Panel/StaticEx.cpp : 实现文件
//

#include "stdafx.h"
#include "StaticEx.h"


// CStaticEx

IMPLEMENT_DYNAMIC(CStaticEx, CStatic)

CStaticEx::CStaticEx()
{
	m_clrCaption = 0;
	m_isExtend = TRUE;
	SetBackgroundColor(RGB(255,0,0));
	SetBorderColor(RGB(0,0,0));
}

CStaticEx::~CStaticEx()
{
}

void CStaticEx::OnPaint()
{
	//下面开始描绘
	//必须使用这个dc，其他dc虽然能使用，但会导致不停的调用这个函数。
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	CBitmap *pOldBitmap =0;
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bkBmp;
	bkBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = dcMem.SelectObject(&bkBmp);
	CBrush defaultBk;
	defaultBk.Attach((HBRUSH) (COLOR_BTNFACE+1));
	dcMem.FillRect(&rect, &defaultBk);
	defaultBk.Detach();
	CRect rectOuter = rect;
	rectOuter.DeflateRect(CSize(0,2));
	CRect rectInner = rectOuter;
	rectInner.DeflateRect(CSize(1,1));
	dcMem.FillRect(&rectOuter, &m_bkBorderBrush);
	dcMem.FillRect(&rectInner, &m_bkBrush);

	if(m_Text.IsEmpty() == FALSE)      //写文本
	{
		COLORREF clrOldColor = dcMem.SetTextColor(m_clrCaption);
		int nOldMode = dcMem.SetBkMode(TRANSPARENT);
		HGDIOBJ hOldFont = dcMem.SelectObject(GetFont()->GetSafeHandle());
		dcMem.DrawText(m_Text.GetString(), -1, &rectOuter, DT_SINGLELINE| DT_CENTER |DT_VCENTER);
		dcMem.SetTextColor(clrOldColor);
		dcMem.SetBkMode(nOldMode);
		dcMem.SelectObject(hOldFont);
	}

	//画加减号
	CString szExtend = _T("");
	if (getExtend())
	{
		szExtend = _T("-  ");
	}
	else
	{
		szExtend = _T("+  ");
	}
	COLORREF clrOldColor = dcMem.SetTextColor(m_clrCaption);
	int nOldMode = dcMem.SetBkMode(TRANSPARENT);
	dcMem.DrawText(szExtend.GetString(), -1, &rectOuter, DT_SINGLELINE| DT_VCENTER | DT_RIGHT);
	dcMem.SetTextColor(clrOldColor);
	dcMem.SetBkMode(nOldMode);
	dc.BitBlt(rect.left, rect.top, rect.right, rect.bottom, &dcMem, 0, 0, SRCCOPY);    
	dcMem.SelectObject(pOldBitmap);

	VERIFY(dcMem.DeleteDC());
}

BOOL CStaticEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;   //这个是必须的
	//    return CStatic::OnEraseBkgnd(pDC);
}

void CStaticEx::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CStaticEx 消息处理程序


BOOL CStaticEx::PreTranslateMessage(MSG* pMsg)
{
	return CStatic::PreTranslateMessage(pMsg);
}
