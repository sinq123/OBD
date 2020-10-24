/**
* @file LinePlot.h
*
* ���ĵ�������CPlotData��CLinePlot��
*
* @author ������
*/

// LinePlot.h : header file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  CLinePlot, CPlotData - a simple 2D graph control
//
//  Author: Paul Grenz
//  Email:  pgrenz@irlabs.com
//
//  You may freely use or modify this code provided this
//  message is included in all derived versions.
//
//  History - 2004/10/28 Initial release to codeguru.com
//
//
//  This class implements a simple 2D graph control which supports
//  multiple plots, locking and unlocking of axes, printing, and 
//  zooming.
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEPLOT_H__7752C288_1A29_4D99_9777_DE3596070E6A__INCLUDED_)
#define AFX_LINEPLOT_H__7752C288_1A29_4D99_9777_DE3596070E6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///  ������������,������Rect
typedef struct t_FLOATRECT
{
	float left;
	float top;
	float right;
	float bottom;
}
FLOATRECT;

#include <afxtempl.h>
#include <vector>
#include "MemDC.h"

/// ֪ͨ��Ϣ
#define NM_PLOT_SEL_CHANGE      (WM_USER+100)
#define NM_PLOT_LIMITS_CHANGE   (WM_USER+101)
#define NM_PLOT_MOUSE_MOVE      (WM_USER+102)

///  ����־
#define PLOT_OVER_NOTHING (0)
#define PLOT_OVER_KEY     (1)
#define PLOT_OVER_DATA    (2)
#define PLOT_OVER_X_MIN   (4)
#define PLOT_OVER_X_MAX   (8)
#define PLOT_OVER_Y_MIN   (16)
#define PLOT_OVER_Y_MAX   (32)
#define PLOT_OVER_X_LOCK  (64)
#define PLOT_OVER_Y_LOCK  (128)

//  id of the edit box.
#define ID_EDT_DATA_ENTRY       (WM_USER+10)

/////////////////////////////////////////////////////////////////////////////
//  CPlotData class
/////////////////////////////////////////////////////////////////////////////

/**
* @brief ͼ������
*/
class  CPlotData
{
	//  constants.
private:
	/// ���߻�����ʽ.
	enum enumPlotStyle
	{
		nStyleLine    = 0,///< ����
		nStyleBar     = 1,///< ��״
	};

	// Construction/Destruction
public:
	CPlotData(CString szName, COLORREF crColor, int nStyle, POINTF *pptData, UINT uiPointCount);///< ��ָ����С���鴴��
	CPlotData(CString szName, COLORREF crColor, int nStyle, std::vector<POINTF> *pvecData);///< �ɶ�̬���鴴��
	CPlotData(const CPlotData &pd);
	virtual ~CPlotData();

	// Attributes
public:
	CString           m_szName;///< ����
	int               m_nStyle;///< ��ʽ
	std::vector<POINTF>      m_pptData;///< ��������
	
	POINT             *m_pptScreen;///<  Ϊ"polyline" ��ͼ����׼��������.
	UINT              m_uiPointCount;///< ���ݵ�����
	UINT			  m_uiMaxScreenPoint;///< ��Ļ��������������
	COLORREF          m_crColor;///< ��ɫ
	bool              m_oIsDirty;///< �Ƿ��Ѿ�����
	CRect             m_rcKeyArea;///< Key����
	float             m_eXDataMin;///< X����Сֵ
	float             m_eXDataMax;///< X�����ֵ
	float             m_eYDataMin;///< Y����Сֵ
	float             m_eYDataMax;///< Y�����ֵ
	float             m_eYDataMean;///< Y��ƽ��ֵ

	//  methods.
public:
	void              Clear();///< ����
	bool              UpdateDatas(POINTF *pptData, UINT uiPointCount);///< ��������
	bool              UpdateDatas(const std::vector<POINTF> *pvecData);
	bool			  AddData(POINTF ptData);///< ���һ������
	void			  SetPtScreenSize(UINT uiCount);///< ������Ļ���������С

	//  operators
public:
	CPlotData&        operator=(const CPlotData& pd);

};

/////////////////////////////////////////////////////////////////////////////
//  CLinePlot control
/////////////////////////////////////////////////////////////////////////////

#define LINE_PLOT_UNDO_SIZE (50) // ���˵Ĳ���

/**
* @brief 2Dͼ�ο�����
*
* ���Ի��������ͣ������ͽ�������ϵ����ӡ������
*/
class  CLinePlot : public CWnd
{
	//  constants.
public:
	enum enumPlotStyle
	{
		LpLine    = 0,
		LpBar     = 1,
	};


	// Construction/Destruction
public:
	/// ���캯��
	CLinePlot();
	/// ��������
	virtual ~CLinePlot();

	//  variables
public:
	CString           m_szName;
	CString           m_szXCaption;
	CString           m_szYCaption;
	int               m_nXIntervals;
	int               m_nYIntervals;
	UINT              m_uiXPrecision;
	UINT              m_uiYPrecision;
	float             m_eXMin;
	float             m_eXMax;
	float             m_eYMin;
	float             m_eYMax;
	FLOATRECT         m_prcUndoLimits[LINE_PLOT_UNDO_SIZE];
	int               m_nUndoIndex;
	float             m_eXRatio;
	float             m_eYRatio;
	int               m_nCursor;
	UINT              m_uiMouseOver;
	CEdit             m_edtDataEntry;
	int               m_nSelected;
	int               m_nMouseOverKey;
	CRect             m_rcZoom;
	bool              m_oIsMouseDown;
	bool              m_oHasPassedOverPlot;
	bool              m_oIsXLocked;
	bool              m_oIsYLocked;
	CRect             m_rcXMin;
	CRect             m_rcXMax;
	CRect             m_rcYMin;
	CRect             m_rcYMax;
	CRect             m_rcXLock;
	CRect             m_rcYLock;
	CRect             m_rcKeyArea;
	CRect             m_rcPlotArea;

	std::vector<CPlotData> m_lstPlotData; ///< ����������������

	// Methods
public:
	BOOL              Create(const RECT& rect, CWnd* pwndParent, UINT uiID);
	int               AddLine(CString szName, COLORREF crColor, enumPlotStyle nStyle, POINTF *pptData, UINT uiPointCount);
	int               AddLine(CString szName, COLORREF crColor, enumPlotStyle nStyle, std::vector<POINTF> *pvecData);
	int               AddLine(const CPlotData &PlotData);
	void              Clear();
	bool              Remove(int nIndex);
	void              Refresh();
	int               Count() { return int(m_lstPlotData.size()); };
	void              Print();
	int               Selected() { return m_nSelected; };

	//  operators
public:
	CPlotData &       operator[](UINT unIndex);

	//  notifications
private:
	void              EmitSelectionChanged();
	void              EmitLimitsChanged();
	void              EmitMouseMove();

	//  sub-attribute get/set
public:
	CString           GetName(int nIndex);
	bool              SetName(int nIndex, CString szName);
	std::vector<POINTF> *      GetDatas(int nIndex);
	bool              SetDatas(int nIndex, std::vector<POINTF>* pvecData);
	bool              SetDatas(int nIndex, POINTF *pptData, UINT uiPointCount);
	bool			  AddData(int nIndex, POINTF ptData);
	COLORREF          GetColor(int nIndex);
	/// @brief ����������ɫ
	///
	/// @param nIndex [in]��������
	/// @param crColor [in]������ɫ
	/// @retval true ִ�гɹ�
	bool              SetColor(int nIndex, COLORREF crColor);// ����������ɫ
	enumPlotStyle     GetStyle(int nIndex);
	bool              SetStyle(int nIndex, enumPlotStyle nStyle);
	UINT              GetPointCount(int nIndex);

	//  attributes get/set
public:
	void              SetName(CString &szName) { m_szName = szName; };
	CString           GetName() { return m_szName; };
	void              SetXCaption(CString &szXCaption) { m_szXCaption = szXCaption; };
	CString           GetXCaption() { return m_szXCaption; };
	void              SetYCaption(CString &szYCaption) { m_szYCaption = szYCaption; };
	CString           GetYCaption() { return m_szYCaption; };
	void              SetXPrecision(UINT &uiXPrecision) { m_uiXPrecision = uiXPrecision; };
	UINT              GetXPrecision() { return m_uiXPrecision; };
	void              SetYPrecision(UINT &uiYPrecision) { m_uiYPrecision = uiYPrecision; };
	UINT              GetYPrecision() { return m_uiYPrecision; };
	void              SetXIntervals(int &nXIntervals) { m_nXIntervals = nXIntervals; };
	int               GetXIntervals() { return m_nXIntervals; };
	void              SetYIntervals(int &nYIntervals) { m_nYIntervals = nYIntervals; };
	int               GetYIntervals() { return m_nYIntervals; };
	bool              SetXMin(float &eXMin);
	float             GetXMin() { return m_eXMin; };
	bool              SetXMax(float &eXMax);
	float             GetXMax() { return m_eXMax; };
	bool              SetYMin(float &eYMin);
	float             GetYMin() { return m_eYMin; };
	bool              SetYMax(float &eYMax);
	float             GetYMax() { return m_eYMax; };
	bool              SetIsXLocked(bool &oLock);
	bool              GetIsXLocked() { return m_oIsXLocked; };
	bool              SetIsYLocked(bool &oLock);
	bool              GetIsYLocked() { return m_oIsYLocked; };
	/// @brief ���ñ�����ɫ
	///
	/// @param crColor [in]������ɫ
	void              SetBkColor(COLORREF crColor);// ���ñ�����ɫ
	/// @brief ���ÿ��������ɫ
	///
	/// @param crColor [in]������ɫ
	void              SetFrameworkColor(COLORREF crColor);// ���ÿ��������ɫ
	/// @brief ����������ɫ
	///
	/// @param crColor [in]������ɫ
	void              SetGridColor(COLORREF crColor);//����������ɫ

	//  functions.
private:
	COLORREF          m_crBkColor;
	COLORREF          m_crFrameworkColor;
	COLORREF          m_crGridColor;
	CRect             DrawFramework(CDC *pdc, CRect rcBounds, int nFontHeight, bool oIsPrinting);
	void              DrawData(CDC *pDC, CRect rcBounds, bool oIsPrinting);
	void              DrawPlotData(CDC* pDC, UINT uiIndex, CRect &rcBounds);
	void              HideEditWindow();
	void              AddToZoomUndo(FLOATRECT &rcLimits);
	FLOATRECT         GetLastZoomUndo();

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinePlot)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CLinePlot)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPLOT_H__7752C288_1A29_4D99_9777_DE3596070E6A__INCLUDED_)
