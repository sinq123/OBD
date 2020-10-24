/**
* @file MathLabel.h
*
* 本档案定义了CMathLabel类
* 
* @author HuaBei
*/

#pragma once


// CMathLabel

/**
* @brief 上下标文本控件
*/
class AFX_EXT_CLASS CMathLabel : public CStatic
{
	DECLARE_DYNAMIC(CMathLabel)

public:
	/// 构造函数
	CMathLabel();
	/// 析构函数
	virtual ~CMathLabel();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

public:
	/// @brief 设置上下标文本内容
	///
	/// @param strText [in]上下标文本内容
	void SetScriptText(CString strText);// 设置上下标文本内容
	void ScriptTextOut(HDC hDC, CString data , CRect* drawRect, int justification);


public:
	CString m_strText; ///< 显示的文本内容
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};


