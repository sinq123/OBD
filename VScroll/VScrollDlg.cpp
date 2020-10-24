
// VScrollDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VScroll.h"
#include "VScrollDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SCROLLBAR_JUMP 10
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVScrollDlg �Ի���




CVScrollDlg::CVScrollDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVScrollDlg::IDD, pParent)
	, m_pBtns(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CVScrollDlg::~CVScrollDlg()
{
	if (m_pBtns != NULL)
	{
		
	}
}

void CVScrollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVScrollDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CVScrollDlg ��Ϣ�������

BOOL CVScrollDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if(NULL == m_pBtns)
	{
		int iBtnNum = 10, iMax = 0;
		m_pBtns = new CButton[iBtnNum];
		CString strBtnText = _T("");
		for (int i = 0 ; i < iBtnNum ; i ++)
		{
			strBtnText.Format(_T("Button %.2d"), i);
			m_pBtns[i].Create(strBtnText, WS_VISIBLE|WS_CHILD, CRect(50, 50 + 100*i, 150, 130 + 100*i), this, 3000 + i);
			m_pBtns[i].ShowWindow(SW_SHOW);


			iMax = 130 + 100*i;
		}


		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0;
		info.nMax = iMax;
		info.nPage = 325;//ÿҳ�ĸ߶�Ҳ���Ǵ��ڸ߶ȣ��������ĳ���=nPage/nMax
		info.nPos = 0;
		SetScrollInfo(SB_VERT, &info, TRUE);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVScrollDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVScrollDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVScrollDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVScrollDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
	switch(nSBCode) {
	case SB_BOTTOM:
		{
			ScrollWindow(0, scrollinfo.nPos - scrollinfo.nMax + scrollinfo.nPage);
			scrollinfo.nPos = scrollinfo.nMax - scrollinfo.nPage;
			SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		}
		break;
	case SB_TOP:
		{
			ScrollWindow(0, scrollinfo.nPos);
			scrollinfo.nPos = 0;
			SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		}
		break;
	case SB_LINEUP://�ϼ�ͷ
		{
			int iLeft = scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP)?SCROLLBAR_JUMP:iLeft;
			scrollinfo.nPos -= iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, iJump);
		}
		break;
	case SB_LINEDOWN://�¼�ͷ
		{
			int iLeft = scrollinfo.nMax - scrollinfo.nPage - scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP)?SCROLLBAR_JUMP:iLeft;
			scrollinfo.nPos += iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, 0 - iJump);
		}
		break;
	case SB_PAGEUP://�Ͽհ�
		{
			int iLeft = scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP):iLeft;
			scrollinfo.nPos -= iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, iJump);
		}
		break;
	case SB_PAGEDOWN://�¿հ�
		{
			int iLeft = scrollinfo.nMax - scrollinfo.nPage - scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP):iLeft;
			scrollinfo.nPos += iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, 0 - iJump);
		}
		break;
	case SB_ENDSCROLL://����
		// MessageBox("SB_ENDSCROLL");
		break;
	case SB_THUMBPOSITION://��ק������λ��
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*SCROLLBAR_STEP);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_THUMBTRACK://��ק�����е�λ��
		{
			ScrollWindow(0, scrollinfo.nPos - nPos);
			scrollinfo.nPos = nPos;
			SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		}
		break;
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CVScrollDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
	if(zDelta > 0) { // ���Ϲ�
		int iMulti = zDelta/WHEEL_DELTA;//����
		int iLeft = scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti):iLeft;
		scrollinfo.nPos -= iJump;
		SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
		ScrollWindow(0, iJump);
	}
	else { // ���¹�
		int iMulti = zDelta/(0 - WHEEL_DELTA);//����
		int iLeft = scrollinfo.nMax - scrollinfo.nPage - scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti):iLeft;
		scrollinfo.nPos += iJump;
		SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
		ScrollWindow(0, 0 - iJump);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
