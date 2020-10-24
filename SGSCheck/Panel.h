#pragma once

/*************************************************************************************
���ߣ���Զ
���ڣ�2009-07-21
˵�������Ĺ����࣬�������Թ��������ĶԻ����CPanelItem��
ʵ���˶�̬����Panelλ�ã��͵�������
��ַ��http://www.cnitblog.com/fangyuanning/
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
	//����һ��Panel
	int InsertPanel(CPanelItem* pItem, int pos);
	//���һ��Panel
	int AddPanel(CPanelItem* pItem);
	//ɾ��һ��Panel
	BOOL	RemovePanel(CPanelItem* pItem);
	//ɾ��һ��Panel
	BOOL	RemovePanel(int panelID);
	//�ж�һ��PanelItem�Ƿ����
	BOOL IsPanelExist(CPanelItem* pItem);
	//�ж�һ��PanelItem�Ƿ����
	BOOL IsPanelExist(int panelId);
	//���ֺ���
	void	LayoutPanel(void);
	//�����Լ��ĺ���
	BOOL	Create(const CRect& rect, CWnd* pParent);
	//�����Ӵ���ռ�ݵĴ�С
	CSize CalcPanelSize(void);
	//�ػ��Ӵ���
	void RedrawChild();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual void PostNcDestroy()
	{
		delete this; //ɾ��������
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


