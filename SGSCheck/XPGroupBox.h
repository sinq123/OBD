
#if !defined(AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_)
#define AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CXPGroupBox : public CButton
{
	DECLARE_DYNAMIC(CXPGroupBox);

public:
	CXPGroupBox();
	virtual ~CXPGroupBox();

	enum XPGroupBoxStyle { XPGB_FRAME = 0, XPGB_WINDOW = 1 };

	// Operations
public:
	virtual CXPGroupBox&	SetBorderColor(COLORREF clrBorder);
	virtual CXPGroupBox&	SetCatptionTextColor(COLORREF clrText);
	virtual CXPGroupBox& SetBackgroundColor(COLORREF clrBKClient);
	virtual CXPGroupBox&	SetBackgroundColor(COLORREF clrBKTilte,  COLORREF clrBKClient);
	virtual CXPGroupBox&	SetXPGroupStyle(XPGroupBoxStyle eStyle); 

	virtual CXPGroupBox& SetText(LPCTSTR lpszTitle);
	virtual CXPGroupBox& SetFontBold(BOOL bBold);
	virtual CXPGroupBox& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual CXPGroupBox& SetFontUnderline(BOOL bSet);
	virtual CXPGroupBox& SetFontItalic(BOOL bSet);
	virtual CXPGroupBox& SetFontSize(int nSize);
	virtual CXPGroupBox& SetFont(LOGFONT lf);

	virtual CXPGroupBox& SetAlignment(DWORD dwType);

	// Overrides
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();

protected:
	void UpdateSurface();
	void ReconstructFont();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	CString				_titleName;
	DWORD				_dwAlignment;
	LOGFONT				_lFont;
	CFont				_font;

	COLORREF			_borderClr;
	COLORREF			_titleBkClr;
	COLORREF			_clientBkClr;
	COLORREF			_titleTextClr;

	XPGroupBoxStyle		_gboxType;
};

#endif // !defined(AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_)