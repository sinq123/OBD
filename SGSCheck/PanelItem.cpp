// Panel/CPanelItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PanelItem.h"
//����Ҳ�������ļ�����ô�޸ĳ��Լ���Դ�ļ���·��
#include "resource.h"
#include "Panel.h"

#define PANEL_HEAD_HEIGHT 20
#define PANEL_HEAD_ID 10999
#define PANEL_ANI_STEP 30
// CPanelItem

IMPLEMENT_DYNAMIC(CPanelItem, CDialog)

CPanelItem::CPanelItem()
{
	m_IsInit = FALSE;
	mPanelSize = CSize(400,400);
	mPanelAniSize = mPanelSize;
	mPanelParent = 0;
	mTimerID = -1;
}

CPanelItem::~CPanelItem()
{
}

CSize CPanelItem::GetPanelSize(void)
{
	return mPanelAniSize;
}

BOOL CPanelItem::Create(UINT nIDTemplate , const CString& name, COLORREF bkcolor, CPanel* pParentWnd, INT nHeadHeight)
{
	//�����ڲ���Ϊ��
	ASSERT(pParentWnd);
	BOOL bRet = CDialog::Create(nIDTemplate, pParentWnd);
	ASSERT(bRet);
	CRect rect;
	GetClientRect(&rect);
	//����ʱ�Ĵ�С�ǶԻ���Ĵ�С
	mPanelSize = CSize(rect.Width(), rect.Height());
	//����Head�ĸ߶�
	mPanelSize.cy += nHeadHeight;
	mPanelAniSize = mPanelSize;
	mHeadHeight = nHeadHeight;
	//��������
	bRet = mPanelHead.Create(name, SS_CENTER | SS_NOTIFY | WS_VISIBLE | WS_CHILD, CRect(0,0 , rect.Width(), PANEL_HEAD_HEIGHT), this, PANEL_HEAD_ID);
	ASSERT(bRet);
	mPanelHead.SetFont(GetFont(), TRUE);
	mPanelHead.SetText(name);
	mPanelHead.SetBackgroundColor(bkcolor);
	mPanelHead.SetExtend(TRUE);
	mPanelParent = pParentWnd;
	m_IsInit = TRUE;
	return bRet;
}


BEGIN_MESSAGE_MAP(CPanelItem, CDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_STN_DBLCLK(PANEL_HEAD_ID, OnHeadDBClick)
END_MESSAGE_MAP()



// CPanelItem ��Ϣ�������



void CPanelItem::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_IsInit)
	{
		mPanelHead.MoveWindow(0,0,cx,PANEL_HEAD_HEIGHT,TRUE);
		mPanelHead.Invalidate(TRUE);
	}
}

BOOL CPanelItem::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		SetFocus();
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelItem::OnLButtonDblClk(	UINT nFlags,CPoint point )
{
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPanelItem::OnHeadDBClick(void)
{
	mPanelHead.ExchangeExtend();
	if(mPanelParent)
	{
		mPanelParent->LayoutPanel();
	}
	if(mTimerID<0)
	{
		static INT tid = 0;
		mTimerID = ++tid;
		SetTimer(mTimerID, 50,0);
	}
	
}

//�ػ�ͷ
void CPanelItem::RedrawHead(void)
{
	mPanelHead.Invalidate(TRUE);
}

void CPanelItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPanelItem::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == mTimerID)
	{
		//��������
		if ( mPanelHead.getExtend() )
		{
			if (mPanelAniSize.cy < mPanelSize.cy)
			{
				mPanelAniSize.cy += PANEL_ANI_STEP;
				if (mPanelAniSize.cy > mPanelSize.cy)
				{
					mPanelAniSize.cy  = mPanelSize.cy;
				}
				if(mPanelParent)
				{
					mPanelParent->LayoutPanel();
				}
			}
			else if (mTimerID > 0)
			{
				KillTimer(mTimerID);
				mTimerID = -1;
			}
		}
		//����
		else
		{
			if (mPanelAniSize.cy > PANEL_HEAD_HEIGHT)
			{
				mPanelAniSize.cy -= PANEL_ANI_STEP;
				if (mPanelAniSize.cy < PANEL_HEAD_HEIGHT)
				{
					mPanelAniSize.cy = PANEL_HEAD_HEIGHT;
				}
				if(mPanelParent)
				{
					mPanelParent->LayoutPanel();
				}
			}
			else if (mTimerID > 0)
			{
				KillTimer(mTimerID);
				mTimerID = -1;
			}
		}
	}
	CWnd::OnTimer(nIDEvent);
}