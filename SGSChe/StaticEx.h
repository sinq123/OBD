#pragma once

/*************************************************************************************
作者：方远
日期：2009-07-21
说明：面板的头，也可以自绘成图片，目前使用的是自绘一个带
边界的矩形。
网址：http://www.cnitblog.com/fangyuanning/
*************************************************************************************/

class CPanelItem;
// CStaticEx

class CStaticEx : public CStatic
{
	DECLARE_DYNAMIC(CStaticEx)

public:
	CStaticEx(void);
	virtual ~CStaticEx(void);

public:
	void SetTextColor(COLORREF color) { m_clrCaption = color;}
	COLORREF GetTextColor(void) { return m_clrCaption; }
	void SetExtend(BOOL ext) { m_isExtend = ext; }
	BOOL getExtend(void) { return m_isExtend; }
	void	ExchangeExtend(void) { BOOL tmp = !m_isExtend ;m_isExtend= tmp ; }
	void SetText(const CString & name) { m_Text = name; }
	void	SetBackgroundColor(COLORREF color) 
	{
		if (m_bkColor != color)
		{
			m_bkColor = color; 
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = m_bkColor;
			logBrush.lbHatch = HS_CROSS;
			if (m_bkBrush.m_hObject)
			{
				m_bkBrush.DeleteObject();
			}
			m_bkBrush.CreateBrushIndirect(&logBrush);
		}
	}
	void	SetBorderColor(COLORREF color) 
	{
		if (m_borderColor != color)
		{
			m_borderColor = color;
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = m_borderColor;
			logBrush.lbHatch = HS_CROSS;
			m_bkBorderBrush.CreateBrushIndirect(&logBrush);
		}
	}
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	DECLARE_MESSAGE_MAP()

protected:
	COLORREF m_clrCaption;
	COLORREF m_bkColor;
	COLORREF m_borderColor;
	CBrush m_bkBrush;
	CBrush m_bkBorderBrush;
	BOOL m_isExtend;
	CString m_Text;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


