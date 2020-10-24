#pragma once

/*************************************************************************************
作者：方远
日期：2009-07-21
说明：面板的管理类，这个类可以管理任意多的对话框项（CPanelItem）
实现了动态调整Panel位置，和调整动画
网址：http://www.cnitblog.com/fangyuanning/
*************************************************************************************/

#include <vector>
// CPanel

class CPanelItem;
class CPanel : public CWnd
{
	DECLARE_DYNAMIC(CPanel)

public:
	CPanel();
	virtual ~CPanel();

public:
	//插入一个Panel
	int InsertPanel(CPanelItem* pItem, int pos);
	//添加一个Panel
	int AddPanel(CPanelItem* pItem);
	//删除一个Panel
	BOOL	RemovePanel(CPanelItem* pItem);
	//删除一个Panel
	BOOL	RemovePanel(int panelID);
	//判断一个PanelItem是否存在
	BOOL IsPanelExist(CPanelItem* pItem);
	//判断一个PanelItem是否存在
	BOOL IsPanelExist(int panelId);
	//布局函数
	void	LayoutPanel(void);
	//创建自己的函数
	BOOL	Create(const CRect& rect, CWnd* pParent);
	//计算子窗口占据的大小
	CSize CalcPanelSize(void);
	//重绘子窗口
	void RedrawChild();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual void PostNcDestroy()
	{
		delete this; //删除对象本身
	}

protected:
	typedef std::vector<CPanelItem*> PanelItemMap;
	PanelItemMap mItemMap;
	CScrollBar mScrollBarV;
	CScrollBar mScrollBarH;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(	UINT nFlags,short zDelta,CPoint pt);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


