
// VScrollDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VScroll.h"
#include "VScrollDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SCROLLBAR_JUMP 10
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CVScrollDlg 对话框




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


// CVScrollDlg 消息处理程序

BOOL CVScrollDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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
		info.nPage = 325;//每页的高度也就是窗口高度，滚动条的长度=nPage/nMax
		info.nPos = 0;
		SetScrollInfo(SB_VERT, &info, TRUE);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVScrollDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	case SB_LINEUP://上箭头
		{
			int iLeft = scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP)?SCROLLBAR_JUMP:iLeft;
			scrollinfo.nPos -= iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, iJump);
		}
		break;
	case SB_LINEDOWN://下箭头
		{
			int iLeft = scrollinfo.nMax - scrollinfo.nPage - scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP)?SCROLLBAR_JUMP:iLeft;
			scrollinfo.nPos += iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, 0 - iJump);
		}
		break;
	case SB_PAGEUP://上空白
		{
			int iLeft = scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP):iLeft;
			scrollinfo.nPos -= iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, iJump);
		}
		break;
	case SB_PAGEDOWN://下空白
		{
			int iLeft = scrollinfo.nMax - scrollinfo.nPage - scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP):iLeft;
			scrollinfo.nPos += iJump;
			SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
			ScrollWindow(0, 0 - iJump);
		}
		break;
	case SB_ENDSCROLL://结束
		// MessageBox("SB_ENDSCROLL");
		break;
	case SB_THUMBPOSITION://拖拽的最终位置
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*SCROLLBAR_STEP);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_THUMBTRACK://拖拽过程中的位置
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
	if(zDelta > 0) { // 向上滚
		int iMulti = zDelta/WHEEL_DELTA;//倍数
		int iLeft = scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti):iLeft;
		scrollinfo.nPos -= iJump;
		SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
		ScrollWindow(0, iJump);
	}
	else { // 向下滚
		int iMulti = zDelta/(0 - WHEEL_DELTA);//倍数
		int iLeft = scrollinfo.nMax - scrollinfo.nPage - scrollinfo.nPos, iJump = (iLeft > SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti)?(SCROLLBAR_JUMP*SCROLLBAR_JUMP*iMulti):iLeft;
		scrollinfo.nPos += iJump;
		SetScrollInfo(SB_VERT, &scrollinfo, TRUE);
		ScrollWindow(0, 0 - iJump);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
