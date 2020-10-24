/**
* @file MathLabel.h
*
* ������������CMathLabel��
* 
* @author HuaBei
*/

#pragma once


// CMathLabel

/**
* @brief ���±��ı��ؼ�
*/
class AFX_EXT_CLASS CMathLabel : public CStatic
{
	DECLARE_DYNAMIC(CMathLabel)

public:
	/// ���캯��
	CMathLabel();
	/// ��������
	virtual ~CMathLabel();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

public:
	/// @brief �������±��ı�����
	///
	/// @param strText [in]���±��ı�����
	void SetScriptText(CString strText);// �������±��ı�����
	void ScriptTextOut(HDC hDC, CString data , CRect* drawRect, int justification);


public:
	CString m_strText; ///< ��ʾ���ı�����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};


