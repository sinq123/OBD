/**
* @file Meter.h
*
* 本文档定义了CMeter类
*
* @author HuaBei
*/

#if !defined(AFX_METER_H__D5802279_6502_4453_BE21_58604877AD39__INCLUDED_)
#define AFX_METER_H__D5802279_6502_4453_BE21_58604877AD39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Meter.h : header file
//

#ifndef ROUND
#define ROUND(x) (int)((x) + 0.5 - (double)((x) < 0))
#endif

#define BOUNDARY_POINTS 200		///< 对表盘圆划分的块数
#define TABNUM 5				///< 彩色刻度数量
/////////////////////////////////////////////////////////////////////////////
// CMeter window

/**
* @brief CMeter图表类
*
* 画带指针的表
*/
class CMeter : public CStatic
{
// Construction
public:
	// 构造函数
	CMeter();

// Attributes
public:

// Operations
public:
	// 析构函数
	virtual ~CMeter();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeter)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRange(double dMin, double dMax);					///< 设置仪表最大值最小值
	void SetUnits(CString &strUnits);							///< 设置单位	
	void SetTitle(CString Title);								///< 设置标题
	void SetTickScale(int nTickScale);							///< 设置每刻度与实际数据的比例尺
	void SetTicks(int nTicks);									///< 设置主刻度（分几大格）
	void SetSubTicks(int nSubTicks);							///< 设置子刻度（每大格分为几小格）
	void UpdateNeedle(double dValue);							///< 显示指针变化

	void SetValueDecimals(int nDecimals);						///< 设置数值显示精度（小数点后位数）
	void SetScaleDecimals(int nDecimals);						///< 设置刻度显示精度（小数点后位数）
	void SetAngleRange(int nStartAngleDeg, int nEndAngleDeg);	///< 表盘圆弧起始终止角度

	void SetNeedleColor (COLORREF colorNeedle);					///< 设置指针颜色
	void SetColorTick(BOOL bColorTick = FALSE);					///< 是否显示彩色刻度	
	void SetFrameBorder(UINT borderWidth,COLORREF crBorder);	///< 设置外框边框大小及颜色
	void SetFrameColor (COLORREF colorFrame);					///< 设置外框背景颜色
	void SetMeterBkColor(COLORREF colorMeterBk);				///< 设置表盘背景颜色
	void SetValueColor(COLORREF colorValue);				    ///< 设置显示值颜色
	void SetWindowColor(COLORREF colorWindow);				    ///<  window背景色
	
	double m_dCurrentValue;			///< 当前值
	double m_dCurrentTick;			///< 当前刻度
	double m_dMaxValue;				///< 最大值
	double m_dMinValue;				///< 最小值
	

	// Generated message map functions
protected:
	void DrawNode(CDC *pDC);									///< 画指针上的点
	void DrawValue(CDC *pDC);									///< 画显示值
	void DrawNeedle(CDC *pDC);									///< 画指针
	void DrawMeterBackground(CDC *pDC, CRect &rect);			///< 画表盘背景
	void ReconstructControl();									///< 重建表盘	

	double m_dLeftAngleRad;						///< 起始角度
	double m_dRightAngleRad;					///< 终止角度
	int m_nCenterRadius;						///< 中心园半径大小
	int m_nRadiusFrame;							///< 仪表盘边框半径

	int m_nScaleDecimals;	///< 刻度显示精度
	int m_nValueDecimals;	///< 数值显示精度	
	int m_nStartAngleDeg;	///< 仪表盘圆弧起始角度
	int m_nEndAngleDeg;		///< 仪表盘圆弧终止角度
	int m_nTicks;			///< 刻度数
	int m_nSubTicks;		///< 分刻度数
	int	m_borderWidth;		///< 外框边框大小
	int m_iOnBand;			///< 上圆盘边框大小
	int m_nTickScale;		///< 每刻度与实际数据的比例尺	

	CPoint m_ptMeterCenter;						///< 仪表中心
	CPoint m_pointBoundary[BOUNDARY_POINTS];	///< 边界点，用于绘制刻度
	
	CRect m_rectCtrl;			///< 控件区域
	CRect m_rectValue;			///< 显示数值区域
	CRgn m_rgnBoundary;			///< 多边形区域

	CBitmap *m_pBitmapOldBackground ;
	CBitmap m_bitmapBackground ;
	CDC		m_dcBackground;
	CFont	m_font;				///< 显示文字字体	

	COLORREF m_colorWindow;		///< window背景色
	COLORREF m_colorHighlight;	///< 高光色
	COLORREF m_colorShadow;		///< 阴影色
	COLORREF m_colorButton;		///< 按钮颜色
	COLORREF m_colorText;		///< 显示文本颜色
	COLORREF m_colorNeedle;		///< 指针颜色
	COLORREF m_colorTable[5];	///< 彩色刻度颜色组
	COLORREF m_crBorder;		///< 外框边框颜色
	COLORREF m_colorFrame;		///< 外框背景颜色
	COLORREF m_colorMeterBk;	///< 表盘背景颜色
	COLORREF m_colorTicks;		///< 表盘刻度颜色
	COLORREF m_colorTickFont;	///< 表盘刻度值颜色
	COLORREF m_colorValue;	    ///< 显示值颜色

	BOOL	m_bColorTick;		///< 是否显示彩色刻度
	
	CString	m_strUnits;			///< 显示单位
	CString m_strTitle;			///< 显示标题

	
	//{{AFX_MSG(CMeter)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METER_H__D5802279_6502_4453_BE21_58604877AD39__INCLUDED_)
