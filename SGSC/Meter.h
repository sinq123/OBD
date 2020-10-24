/**
* @file Meter.h
*
* ���ĵ�������CMeter��
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

#define BOUNDARY_POINTS 200		///< �Ա���Բ���ֵĿ���
#define TABNUM 5				///< ��ɫ�̶�����
/////////////////////////////////////////////////////////////////////////////
// CMeter window

/**
* @brief CMeterͼ����
*
* ����ָ��ı�
*/
class CMeter : public CStatic
{
// Construction
public:
	// ���캯��
	CMeter();

// Attributes
public:

// Operations
public:
	// ��������
	virtual ~CMeter();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeter)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRange(double dMin, double dMax);					///< �����Ǳ����ֵ��Сֵ
	void SetUnits(CString &strUnits);							///< ���õ�λ	
	void SetTitle(CString Title);								///< ���ñ���
	void SetTickScale(int nTickScale);							///< ����ÿ�̶���ʵ�����ݵı�����
	void SetTicks(int nTicks);									///< �������̶ȣ��ּ����
	void SetSubTicks(int nSubTicks);							///< �����ӿ̶ȣ�ÿ����Ϊ��С��
	void UpdateNeedle(double dValue);							///< ��ʾָ��仯

	void SetValueDecimals(int nDecimals);						///< ������ֵ��ʾ���ȣ�С�����λ����
	void SetScaleDecimals(int nDecimals);						///< ���ÿ̶���ʾ���ȣ�С�����λ����
	void SetAngleRange(int nStartAngleDeg, int nEndAngleDeg);	///< ����Բ����ʼ��ֹ�Ƕ�

	void SetNeedleColor (COLORREF colorNeedle);					///< ����ָ����ɫ
	void SetColorTick(BOOL bColorTick = FALSE);					///< �Ƿ���ʾ��ɫ�̶�	
	void SetFrameBorder(UINT borderWidth,COLORREF crBorder);	///< �������߿��С����ɫ
	void SetFrameColor (COLORREF colorFrame);					///< ������򱳾���ɫ
	void SetMeterBkColor(COLORREF colorMeterBk);				///< ���ñ��̱�����ɫ
	void SetValueColor(COLORREF colorValue);				    ///< ������ʾֵ��ɫ
	void SetWindowColor(COLORREF colorWindow);				    ///<  window����ɫ
	
	double m_dCurrentValue;			///< ��ǰֵ
	double m_dCurrentTick;			///< ��ǰ�̶�
	double m_dMaxValue;				///< ���ֵ
	double m_dMinValue;				///< ��Сֵ
	

	// Generated message map functions
protected:
	void DrawNode(CDC *pDC);									///< ��ָ���ϵĵ�
	void DrawValue(CDC *pDC);									///< ����ʾֵ
	void DrawNeedle(CDC *pDC);									///< ��ָ��
	void DrawMeterBackground(CDC *pDC, CRect &rect);			///< �����̱���
	void ReconstructControl();									///< �ؽ�����	

	double m_dLeftAngleRad;						///< ��ʼ�Ƕ�
	double m_dRightAngleRad;					///< ��ֹ�Ƕ�
	int m_nCenterRadius;						///< ����԰�뾶��С
	int m_nRadiusFrame;							///< �Ǳ��̱߿�뾶

	int m_nScaleDecimals;	///< �̶���ʾ����
	int m_nValueDecimals;	///< ��ֵ��ʾ����	
	int m_nStartAngleDeg;	///< �Ǳ���Բ����ʼ�Ƕ�
	int m_nEndAngleDeg;		///< �Ǳ���Բ����ֹ�Ƕ�
	int m_nTicks;			///< �̶���
	int m_nSubTicks;		///< �̶ֿ���
	int	m_borderWidth;		///< ���߿��С
	int m_iOnBand;			///< ��Բ�̱߿��С
	int m_nTickScale;		///< ÿ�̶���ʵ�����ݵı�����	

	CPoint m_ptMeterCenter;						///< �Ǳ�����
	CPoint m_pointBoundary[BOUNDARY_POINTS];	///< �߽�㣬���ڻ��ƿ̶�
	
	CRect m_rectCtrl;			///< �ؼ�����
	CRect m_rectValue;			///< ��ʾ��ֵ����
	CRgn m_rgnBoundary;			///< ���������

	CBitmap *m_pBitmapOldBackground ;
	CBitmap m_bitmapBackground ;
	CDC		m_dcBackground;
	CFont	m_font;				///< ��ʾ��������	

	COLORREF m_colorWindow;		///< window����ɫ
	COLORREF m_colorHighlight;	///< �߹�ɫ
	COLORREF m_colorShadow;		///< ��Ӱɫ
	COLORREF m_colorButton;		///< ��ť��ɫ
	COLORREF m_colorText;		///< ��ʾ�ı���ɫ
	COLORREF m_colorNeedle;		///< ָ����ɫ
	COLORREF m_colorTable[5];	///< ��ɫ�̶���ɫ��
	COLORREF m_crBorder;		///< ���߿���ɫ
	COLORREF m_colorFrame;		///< ��򱳾���ɫ
	COLORREF m_colorMeterBk;	///< ���̱�����ɫ
	COLORREF m_colorTicks;		///< ���̶̿���ɫ
	COLORREF m_colorTickFont;	///< ���̶̿�ֵ��ɫ
	COLORREF m_colorValue;	    ///< ��ʾֵ��ɫ

	BOOL	m_bColorTick;		///< �Ƿ���ʾ��ɫ�̶�
	
	CString	m_strUnits;			///< ��ʾ��λ
	CString m_strTitle;			///< ��ʾ����

	
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
