#pragma once

/*************************************************************************************
作者：方远
日期：2009-07-21
说明：面板的子窗体，一个面板可以包含任意多个子窗体，每个
子窗口是一个对话框，对应一个对话框的模板ID，调用创建函数
后，加入CPanel类就可以了，以后面板的位置和大小都有CPanel控制
每个对话框必须从这个类派生，虽然可以直接使用这个类，但是要
响应对话框上控件的函数，所以还是派生一个子类实现更好。
网址：http://www.cnitblog.com/fangyuanning/
*************************************************************************************/

#include "StaticEx.h"
// CPanelItem
class CPanel;
class CPanelItem : public CDialog
{
	DECLARE_DYNAMIC(CPanelItem)

public:
	CPanelItem();
	virtual ~CPanelItem();

public:
	//获取面板的大小，用来计算面板的位置
	virtual CSize GetPanelSize(void);
	//获取ID
	int	GetID(void) { return mID; }
	//设置ID
	void	SetID(int id) { mID = id; }
	//创建函数
	virtual BOOL Create(UINT nIDTemplate, const CString& name, COLORREF bkcolor, CPanel* pParentWnd, INT nHeadHeight = 20 );
	//重绘头
	void RedrawHead(void);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PostNcDestroy()
	{
		delete this; //删除对象本身
	}
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	int mID;
	//Panel的头
	CStaticEx mPanelHead;
	//窗口显示的名字
	CString mItemName;
	//面板对话框的大小
	CSize	mPanelSize;
	//面板动画大小
	CSize mPanelAniSize;
	//Panel头的高度
	INT mHeadHeight;
	//面板指针
	CPanel* mPanelParent;
	//动画ID
	INT mTimerID;
	BOOL m_IsInit;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(	UINT nFlags,CPoint point );
	afx_msg void OnHeadDBClick(void);
	void OnTimer(UINT nIDEvent) ;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


