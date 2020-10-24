

#include "stdafx.h"
#include "XPGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXPGroupBox,CButton);

CXPGroupBox::CXPGroupBox()
{
	_titleName = _T("");

	_borderClr = ::GetSysColor(COLOR_3DSHADOW);
	_clientBkClr = ::GetSysColor(COLOR_BTNFACE);

	_titleTextClr = ::GetSysColor(COLOR_WINDOWTEXT);
	_titleBkClr = ::GetSysColor(COLOR_BTNFACE);

	_gboxType = XPGB_FRAME;
	_dwAlignment = SS_LEFT;
}

CXPGroupBox::~CXPGroupBox()
{
}

BEGIN_MESSAGE_MAP(CXPGroupBox, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CXPGroupBox::OnPaint() 
{
	CPaintDC dc(this);

	CRect	rectClient;
	GetClientRect(rectClient);

	// Defalte Rect
	rectClient.DeflateRect(1, 1);

	// Get Text Rect 
	CSize sizeText;
	CRect rectText, rectFrame;
	CRect rectTitle, rectContent;

	CFont *pOldFont = dc.SelectObject(&_font);

	// get Text if need
	if (_titleName.IsEmpty())
	{ 
		GetWindowText(_titleName);
		if (! _titleName.IsEmpty() )
			_titleName = _T(" ") + _titleName + _T(" ");
	}

	if (!_titleName.IsEmpty())
	{
		sizeText = dc.GetTextExtent(_titleName);
	}
	else
	{
		sizeText.cx = 0;
		sizeText.cy = 0;
	}

	if (_gboxType == XPGB_FRAME )		// Frame style
	{
		// Calculate Text Rect 
		switch (_dwAlignment)
		{
		case SS_LEFT:	
			rectText.top = rectClient.top;
			rectText.left = rectClient.left + 10;

			rectText.bottom = rectText.top + sizeText.cy;
			rectText.right = rectText.left + sizeText.cx;
			break;
		case SS_CENTER:	
			rectText.top = rectClient.top;
			rectText.left = rectClient.left + (rectClient.Width() - sizeText.cx) / 2 ;

			rectText.bottom = rectText.top + sizeText.cy;
			rectText.right = rectText.left + sizeText.cx;
			break;
		case SS_RIGHT	:
			rectText.top = rectClient.top;
			rectText.right = rectClient.right -10 ;

			rectText.bottom = rectText.top + sizeText.cy;
			rectText.left = rectText.right - sizeText.cx;
			break;
		}

		//  Calculate Frame rect
		rectFrame.left = rectClient.left;
		rectFrame.top = rectClient.top + sizeText.cy / 2;

		rectFrame.right = rectClient.right;
		rectFrame.bottom = rectFrame.top + rectClient.Height() - sizeText.cy / 2; 

		// Draw Frame border
		CPen penFrame;
		CBrush brushBKFrame(_titleBkClr);

		penFrame.CreatePen(PS_SOLID, 1, _borderClr);

		CPen* pOldPen = dc.SelectObject(&penFrame);
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

		dc.RoundRect(rectFrame, CPoint(10, 10)); 

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush); 

		dc.IntersectClipRect(rectText);
		dc.FillSolidRect(rectText, _titleBkClr);
	}
	else  // Windows Style
	{
		// Calculate Title size
		rectTitle.top = rectClient.top;
		rectTitle.left = rectClient.left ;

		rectTitle.right = rectClient.right;
		rectTitle.bottom = rectClient.top + sizeText.cy + 4;

		// Draw Title round rect
		CPen penFrame;
		CBrush brushBKTitle(_titleBkClr);
		CBrush brushBKContent(_clientBkClr);

		penFrame.CreatePen(PS_SOLID, 1, _borderClr);

		CPen* pOldPen = dc.SelectObject(&penFrame);
		CBrush* pOldBrush = dc.SelectObject(&brushBKTitle);

		dc.RoundRect(rectClient, CPoint(10, 10)); 

		dc.SelectObject(pOldBrush); 

		// Draw content area
		rectContent.left = rectClient.left;
		rectContent.top = rectClient.top + sizeText.cy + 4;

		rectContent.right = rectClient.right;
		rectContent.bottom = rectContent.top + rectClient.Height() - sizeText.cy - 4; 

		pOldBrush = dc.SelectObject(&brushBKContent); 

		dc.Rectangle(rectContent);  

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush); 

		// Calculate Text Rect 
		switch (_dwAlignment)
		{
		case SS_LEFT:	
			rectText.top = rectTitle.top + 2;
			rectText.left = rectTitle.left + 2 ;

			rectText.bottom = rectText.top + sizeText.cy;
			rectText.right = rectText.left + sizeText.cx ;
			break;
		case SS_CENTER:	
			rectText.top = rectTitle.top + 2;
			rectText.left = rectTitle.left + (rectTitle.Width() - sizeText.cx) / 2 ;

			rectText.bottom = rectText.top + sizeText.cy;
			rectText.right = rectText.left + sizeText.cx ;
			break;
		case SS_RIGHT	:
			rectText.top = rectTitle.top + 2;
			rectText.right = rectClient.right - 2  ;

			rectText.bottom = rectText.top + sizeText.cy;
			rectText.left = rectText.right - sizeText.cx;
			break;
		}
	}

	COLORREF clrOldText = dc.SetTextColor(_titleTextClr);
	UINT nMode = dc.SetBkMode(TRANSPARENT);

	dc.DrawText(_titleName, &rectText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP ); //DT_END_ELLIPSIS);

	// restore DC
	dc.SetBkMode(nMode);
	dc.SetTextColor(clrOldText);
	dc.SelectObject(pOldFont);
}

BOOL CXPGroupBox::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	dwStyle |= BS_ICON;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CXPGroupBox::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_ICON;
	return CButton::PreCreateWindow(cs);
}

void CXPGroupBox::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();

	//modified the style to avoid text overlap when press tab 
	ModifyStyle(0, BS_ICON);

	// Get Defalut Font 
	CFont* cf = GetFont();
	if (cf !=NULL)
	{
		cf->GetObject(sizeof(_lFont), &_lFont);
	}
	else
	{
		GetObject(GetStockObject(SYSTEM_FONT),sizeof(_lFont), &_lFont);
	}

	ReconstructFont();
}

void CXPGroupBox::ReconstructFont()
{
	_font.DeleteObject();
	BOOL bCreated = _font.CreateFontIndirect(&_lFont);

	ASSERT(bCreated);
}

void CXPGroupBox::UpdateSurface()
{
	CRect (rc);
	GetWindowRect(rc);
	RedrawWindow();

	GetParent()->ScreenToClient(rc);
	GetParent()->InvalidateRect(rc, TRUE);
	GetParent()->UpdateWindow();
}

CXPGroupBox& CXPGroupBox::SetXPGroupStyle(XPGroupBoxStyle eStyle) 
{
	_gboxType = eStyle;
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetFont(LOGFONT lf)
{
	CopyMemory(&_lFont, &lf, sizeof(_lFont));
	ReconstructFont();
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetFontBold(BOOL bBold)
{
	_lFont.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetFontName(const CString& strFont, BYTE byCharSet)
{
	_lFont.lfCharSet = byCharSet;

	_tcscpy(_lFont.lfFaceName,strFont);
	ReconstructFont();
	UpdateSurface();

	return *this;
}

CXPGroupBox& CXPGroupBox::SetFontUnderline(BOOL bSet)
{
	_lFont.lfUnderline = bSet;
	ReconstructFont();
	UpdateSurface();

	return *this;
}

CXPGroupBox& CXPGroupBox::SetFontItalic(BOOL bSet)
{
	_lFont.lfItalic = bSet;
	ReconstructFont();
	UpdateSurface();

	return *this;	
}

CXPGroupBox& CXPGroupBox::SetFontSize(int nSize)
{
	CFont cf;
	LOGFONT lf;

	cf.CreatePointFont(nSize * 10, _lFont.lfFaceName);
	cf.GetLogFont(&lf);

	_lFont.lfHeight = lf.lfHeight;
	_lFont.lfWidth  = lf.lfWidth;

	ReconstructFont();
	UpdateSurface();

	return *this;
}

CXPGroupBox& CXPGroupBox::SetBorderColor(COLORREF clrBorder)
{
	_borderClr = clrBorder;
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetCatptionTextColor(COLORREF clrText) 
{
	_titleTextClr = clrText;
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetBackgroundColor(COLORREF clrBKClient)
{
	_titleBkClr = clrBKClient;
	_clientBkClr = clrBKClient;
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetBackgroundColor(COLORREF clrBKTilte,  COLORREF clrBKClient)
{
	_titleBkClr = clrBKTilte;
	_clientBkClr = clrBKClient;
	UpdateSurface();
	return *this;
}

CXPGroupBox& CXPGroupBox::SetText(LPCTSTR lpszText)
{
	if (IsWindow(this->GetSafeHwnd())) 
	{
		_titleName = lpszText;
		_titleName = _T(" ") + _titleName + _T(" ");
		UpdateSurface();
	}

	return *this;
}

CXPGroupBox& CXPGroupBox::SetAlignment(DWORD dwType)
{
	switch (dwType)
	{
	default	: ASSERT(false);
	case SS_LEFT:	
		_dwAlignment = SS_LEFT;
		break;
	case	SS_CENTER:	
		_dwAlignment = SS_CENTER;
		break;
	case	SS_RIGHT	:
		_dwAlignment = SS_RIGHT;
		break;
	}
	UpdateSurface();

	return *this;
}