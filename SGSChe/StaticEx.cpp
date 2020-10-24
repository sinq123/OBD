// Panel/StaticEx.cpp : ʵ���ļ�
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
	//���濪ʼ���
	//����ʹ�����dc������dc��Ȼ��ʹ�ã����ᵼ�²�ͣ�ĵ������������
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

	if(m_Text.IsEmpty() == FALSE)      //д�ı�
	{
		COLORREF clrOldColor = dcMem.SetTextColor(m_clrCaption);
		int nOldMode = dcMem.SetBkMode(TRANSPARENT);
		HGDIOBJ hOldFont = dcMem.SelectObject(GetFont()->GetSafeHandle());
		dcMem.DrawText(m_Text.GetString(), -1, &rectOuter, DT_SINGLELINE| DT_CENTER |DT_VCENTER);
		dcMem.SetTextColor(clrOldColor);
		dcMem.SetBkMode(nOldMode);
		dcMem.SelectObject(hOldFont);
	}

	//���Ӽ���
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

	return TRUE;   //����Ǳ����
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



// CStaticEx ��Ϣ�������


BOOL CStaticEx::PreTranslateMessage(MSG* pMsg)
{
	return CStatic::PreTranslateMessage(pMsg);
}
