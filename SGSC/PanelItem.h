#pragma once

/*************************************************************************************
���ߣ���Զ
���ڣ�2009-07-21
˵���������Ӵ��壬һ�������԰����������Ӵ��壬ÿ��
�Ӵ�����һ���Ի��򣬶�Ӧһ���Ի����ģ��ID�����ô�������
�󣬼���CPanel��Ϳ����ˣ��Ժ�����λ�úʹ�С����CPanel����
ÿ���Ի��������������������Ȼ����ֱ��ʹ������࣬����Ҫ
��Ӧ�Ի����Ͽؼ��ĺ��������Ի�������һ������ʵ�ָ��á�
��ַ��http://www.cnitblog.com/fangyuanning/
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
	//��ȡ���Ĵ�С��������������λ��
	virtual CSize GetPanelSize(void);
	//��ȡID
	int	GetID(void) { return mID; }
	//����ID
	void	SetID(int id) { mID = id; }
	//��������
	virtual BOOL Create(UINT nIDTemplate, const CString& name, COLORREF bkcolor, CPanel* pParentWnd, INT nHeadHeight = 20 );
	//�ػ�ͷ
	void RedrawHead(void);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PostNcDestroy()
	{
		delete this; //ɾ��������
	}
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

protected:
	int mID;
	//Panel��ͷ
	CStaticEx mPanelHead;
	//������ʾ������
	CString mItemName;
	//���Ի���Ĵ�С
	CSize	mPanelSize;
	//��嶯����С
	CSize mPanelAniSize;
	//Panelͷ�ĸ߶�
	INT mHeadHeight;
	//���ָ��
	CPanel* mPanelParent;
	//����ID
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


