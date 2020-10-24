// Panel/Panel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Panel.h"
#include "PanelItem.h"

// CPanel
#define VERTICAL_WIDTH 15

IMPLEMENT_DYNAMIC(CPanel, CWnd)

CPanel::CPanel()
{

}

CPanel::~CPanel()
{
}

//����һ��Panel
int CPanel::InsertPanel(CPanelItem* pItem, int pos)
{
	if (pItem)
	{
		if (IsPanelExist(pItem))
		{
			return -1;
		}
		if (pos >= (int)mItemMap.size() || pos <0)
		{
			return AddPanel(pItem);
		}
		else
		{
			std::vector<CPanelItem*>::iterator it =mItemMap.begin();
			std::vector<CPanelItem*>::iterator ed = mItemMap.end();
			int j=0;
			for (; it != ed; ++it, ++j)
			{
				if (j == pos)
				{
					mItemMap.insert(it, 1, pItem);
					//�޸ĺ���ĳ��ڵ�ID
					for (int i=j; i<(int)mItemMap.size(); ++i)
					{
						mItemMap[i]->SetID(mItemMap[i]->GetID() + 1);
					}
					break;
				}
			}
			//pItem->SetParent(this);
		}
		LayoutPanel();
	}
	return -1;
}

//���һ��Panel
int CPanel::AddPanel(CPanelItem* pItem)
{
	if(pItem && IsPanelExist(pItem) == FALSE)
	{
		mItemMap.push_back(pItem);
		pItem->SetID((int)mItemMap.size()-1);
		LayoutPanel();
		return pItem->GetID();
	}
	return -1;
}

//ɾ��һ��Panel
BOOL	CPanel::RemovePanel(CPanelItem* pItem)
{
	if (pItem)
	{
		return RemovePanel(pItem->GetID());
	}
	return FALSE;
}

//ɾ��һ��Panel
BOOL	CPanel::RemovePanel(int panelID)
{
	BOOL bModify = FALSE;
	std::vector<CPanelItem*>::iterator it =mItemMap.begin();
	std::vector<CPanelItem*>::iterator ed = mItemMap.end();
	int j=0;
	for (; it != ed; ++it, ++j)
	{
		if (j == panelID)
		{
			(*it)->SetParent(NULL);
			(*it)->SetID(-1);
			//��СID
			for (size_t ii=j; ii<mItemMap.size(); ii++)
			{
				mItemMap[ii]->SetID(mItemMap[ii]->GetID() -1);
			}
			mItemMap.erase(it);
			bModify = TRUE;
			break;
		}
	}
	if (bModify)
	{
		LayoutPanel();
	}
	return TRUE;
}

//�ж�һ��PanelItem�Ƿ����
BOOL CPanel::IsPanelExist(CPanelItem* pItem)
{
	if (pItem)
	{
		for (size_t i=0; i<mItemMap.size(); i++)
		{
			if (mItemMap[i] == pItem)
			{
				ASSERT(FALSE && "attemp to add a panel twice CPanel::IsPanelExist");
				return TRUE;
			}
		}
	}
	return FALSE;
}
//�ж�һ��PanelItem�Ƿ����
BOOL CPanel::IsPanelExist(int panelId)
{
	for (size_t i=0; i<mItemMap.size(); i++)
	{
		if (mItemMap[i]->GetID() == panelId)
		{
			ASSERT(FALSE && "attemp to add a panel twice CPanel::IsPanelExist");
			return TRUE;
		}
	}
	return FALSE;
}

void	CPanel::LayoutPanel(void)
{
	//��ͷ��β���޸�λ�ã���������Լ���λ�ã���ô����������
	CRect selfRect;
	GetClientRect(&selfRect);
	CSize childSize = CalcPanelSize();
	BOOL bShowHScroll = FALSE, bShowVScroll = FALSE;
	INT maxWidth = selfRect.Width();
	INT maxHeight = selfRect.Height();
	//�ж��Ƿ���ʾ������
	//if (selfRect.Width() < childSize.cx)
	//{
	//	bShowHScroll = TRUE;
	//	maxWidth -= 25;
	//}
	if (selfRect.Height() < childSize.cy)
	{
		bShowVScroll = TRUE;
		maxWidth -= VERTICAL_WIDTH;
	}
	INT currX = 0, currY = bShowVScroll ? -mScrollBarV.GetScrollPos() : 0;
	for (size_t i=0; i<mItemMap.size(); i++)
	{
		CSize cSize = mItemMap[i]->GetPanelSize();
		mItemMap[i]->MoveWindow(currX, currY, maxWidth, cSize.cy, TRUE);
		currY += cSize.cy;
	}
	if (mScrollBarV.m_hWnd)
	{
		if (bShowVScroll)
		{
			SCROLLINFO info;
			info.cbSize = sizeof(SCROLLINFO);     
			info.fMask = SIF_ALL;     
			info.nMin = 0;     
			info.nMax = childSize.cy; 
			info.nPage = selfRect.Height(); 
			info.nPos = mScrollBarV.GetScrollPos();    
			info.nTrackPos = 2; 
			mScrollBarV.SetScrollInfo(&info);
			mScrollBarV.MoveWindow(maxWidth, 0, selfRect.Width() , selfRect.Height());
			mScrollBarV.EnableScrollBar(ESB_ENABLE_BOTH);
			mScrollBarV.ShowScrollBar(TRUE);
		}
		else
		{
			mScrollBarV.ShowScrollBar(FALSE);
		}
	}
	//if (mScrollBarH.m_hWnd)
	//{
	//	if (bShowHScroll)
	//	{
	//		mScrollBarH.MoveWindow(0,maxHeight, selfRect.Width(), selfRect.Height());
	//		mScrollBarH.ShowWindow(SW_SHOW);
	//	}
	//	else
	//	{
	//		mScrollBarH.ShowWindow(SW_HIDE);
	//	}
	//}
}

//�����Լ��ĺ���
BOOL	CPanel::Create(const CRect& rect, CWnd* pParent)
{
	BOOL bRet = FALSE;
	//�ظ�����
	if (m_hWnd != NULL)
	{
		return TRUE;
	}
	//�����Լ�
	CString strClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH) (COLOR_BTNFACE+1), 0);
	bRet = CWnd::Create(strClassName, _T(""),WS_CHILD | WS_VISIBLE, rect, pParent, 11);
	ASSERT(bRet);
	//����������
	//bRet = mScrollBarH.Create(SBS_HORZ | SBS_BOTTOMALIGN | WS_CHILD, CRect(5, 5, 100, 30), this, 100);
	//ASSERT(bRet);
	bRet = mScrollBarV.Create(SBS_VERT | SBS_RIGHTALIGN | WS_CHILD, CRect(5, 5, 100, 30), this, 200);
	ASSERT(bRet);
	return bRet;
}

//�����Ӵ���ռ�ݵĴ�С
CSize CPanel::CalcPanelSize(void)
{
	CSize sz(0,0);
	for (size_t i=0; i<mItemMap.size(); i++)
	{
		CSize panelSize = mItemMap[i]->GetPanelSize();
		if (sz.cx < panelSize.cx)
		{
			sz.cx = panelSize.cx;
		}
		if (panelSize.cy > 0)
		{
			sz.cy += panelSize.cy;
		}
	}
	return sz;
}


BEGIN_MESSAGE_MAP(CPanel, CWnd)
	ON_WM_SIZE()
	ON_WM_VSCROLL( )
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CPanel ��Ϣ�������



void CPanel::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (m_hWnd)
	{
		LayoutPanel();
	}
}

BOOL CPanel::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		SetFocus();
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CPanel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar && pScrollBar->IsWindowVisible())
	{
		int nMin,nMax,nCurpos, oldPos;
		pScrollBar->GetScrollRange(&nMin,&nMax);  //ȡ�ù�������Χ
		nCurpos=pScrollBar->GetScrollPos();          //ȡ�õ�ǰֵ
		oldPos = nCurpos;
		switch(nSBCode)                 //���������Ϣ
		{
		case SB_LINEUP:                 //����ļ�ͷ������
		case SB_PAGEUP:
			nCurpos=nCurpos-20;
			break;   
		case SB_LINEDOWN:
		case SB_PAGEDOWN:
			nCurpos += 20;
			break;    //���ҵļ�ͷ������
		case SB_THUMBTRACK:         //����϶�
			nCurpos=nPos;
			break;
		}
		if(nCurpos>nMax)  
		{
			nCurpos=nMax;
		}

		if (oldPos != nCurpos)
		{
			pScrollBar->SetScrollPos(nCurpos);  //������Ϣ
			LayoutPanel();
			RedrawChild();
		}
	}
}

//ע������������������Ӵ��ڻ����Լ��н����ʱ��Żᱻ����
 BOOL CPanel::OnMouseWheel(	UINT nFlags,short zDelta,CPoint pt)
 {
	 if (zDelta > 0)
	 {
		OnVScroll(SB_LINEUP, 0, &mScrollBarV);
	 }
	 else
	 {
		OnVScroll(SB_LINEDOWN, 0, &mScrollBarV);
	 }
	 return CWnd::OnMouseWheel(nFlags, zDelta, pt);
 }

 void CPanel::RedrawChild()
 {
	for (size_t i=0; i<mItemMap.size(); i++)
	{
		mItemMap[i]->RedrawHead();
	}
 }