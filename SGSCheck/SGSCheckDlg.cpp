
// SGSCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SGSCheck.h"
#include "SGSCheckDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSGSCheckDlg 对话框

CSGSCheckDlg* g_pSGSCheckDlg(NULL);


CSGSCheckDlg::CSGSCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CSGSCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	//m_nDlgFontSize = nSM_CYSCREEN * 25 / 1024;
	m_nDlgFontSize = 14;
	m_fontDlgFont.CreateFont(m_nDlgFontSize,
		0, 
		0, 
		0, 
		FW_BOLD,
		FALSE, 
		FALSE, 
		FALSE, 
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_MODERN,
		_T("宋体"));
}

CSGSCheckDlg::~CSGSCheckDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CSGSCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_BTN_STATR, m_btnState);
	DDX_Control(pDX, IDC_BTN_RESTATR, m_btnReState);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_ST_EQU_SELF_INS, m_lbEquSelfIns);
	DDX_Control(pDX, IDC_ST_INTERVAL, m_lbInterva);
	DDX_Control(pDX, IDC_ST_GAS_LEAK_HUNNTING, m_lbGasleakhunnting);
	DDX_Control(pDX, IDC_ST_CURRENT_TIME, m_lbCurrentTime);

	DDX_Control(pDX, IDC_ST_BK1, m_BK1);
	DDX_Control(pDX, IDC_ST_BK3, m_BK3);
	DDX_Control(pDX, IDC_CK_QYC_COUST_LOAD, m_chkQYCCoustLoad);
	DDX_Control(pDX, IDC_CK_CYC_COUST_LOAD, m_chkCYCCoustLoad);
	DDX_Control(pDX, IDC_CK_QYC_PLHP, m_chkQYCPLHP);
	DDX_Control(pDX, IDC_CK_CYC_PLHP, m_chkCYCPLHP);
	DDX_Control(pDX, IDC_CK_GAS_CHECH, m_chkGasChech);
	DDX_Control(pDX, IDC_CK_LOW_FOLW_CHECH, m_chkLowFlowChech);
	DDX_Control(pDX, IDC_CK_SMOKE_METER, m_chkSmokeMeter);
	DDX_Control(pDX, IDC_CK_NOX_ANALYZER, m_chkNOxAnalyzeer);
	DDX_Control(pDX, IDC_CK_FLOWMETER, m_chkFlowMeter);
	DDX_Control(pDX, IDC_CK_WEATHER_STATION, m_chkWetherStation);
	DDX_Control(pDX, IDC_CK_TACHOMETER, m_chkTachoMerter);

	DDX_Control(pDX, IDC_ST_SPEEDOMETER, m_stSpeedometer);
	DDX_Control(pDX, IDC_ST_TACHOMETER, m_stTachometer);
}

BEGIN_MESSAGE_MAP(CSGSCheckDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSGSCheckDlg 消息处理程序

BOOL CSGSCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	g_pSGSCheckDlg = this;
	// 初始化控件
	InitCtrls();
	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);
	
	// 刷新时间
	CString strCurrentTime(L"");
	strCurrentTime.Format(L"当前时间:%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	m_lbCurrentTime.SetText(strCurrentTime);
	SetTimer(1, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSGSCheckDlg::OnPaint()
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
		CDialogZoom::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSGSCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSGSCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 1:
		{
			CString strCurrentTime(L"");
			strCurrentTime.Format(L"当前时间:%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			m_lbCurrentTime.SetText(strCurrentTime);
		}
		break;

	default:
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}


void CSGSCheckDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogZoom::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CSGSCheckDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogZoom::OnClose();
}

void CSGSCheckDlg::InitCtrls(void)
{
	SetDlgFont();
	//m_btnExit.SetFlat(FALSE);

	auto SetBTN = [] (CButtonST* pBTN)
	{
		//鼠标放在按钮内时的背景色 
		pBTN->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(128,128,128));
		//普通状态时的背景色
		pBTN->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(128,128,128));
		//按钮被按下后的背景色 
		pBTN->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(128,128,128));
		//鼠标放在按钮内时的前景色
		pBTN->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,255));
		//普通状态时的前景色 
		pBTN->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
		//按钮被按下后的前景色
		pBTN->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));

		pBTN->SetFont(&g_pSGSCheckDlg->m_fontDlgFont);
	};
	SetBTN(&m_btnExit);
	SetBTN(&m_btnState);
	SetBTN(&m_btnReState);
	SetBTN(&m_btnStop);

	auto SetLB = [] (CLabel* plb)
	{
		plb->SetTextColor(RGB(255,255,255));
		plb->SetBkColor(RGB(128,128,128));
	};

	SetLB(&m_lbEquSelfIns);
	SetLB(&m_lbInterva);
	SetLB(&m_lbGasleakhunnting);
	SetLB(&m_lbCurrentTime);
	m_lbInterva.SetWindowTextW(L"");

	//m_BK1.SetBackgroundColor(RGB(0,0,0),RGB(0,0,0));

	

	auto SetCHK = [] (CButtonST* pChk)
	{
		pChk->SetIcon(IDI_ICON_CHK_BOX_ON, IDI_ICON_CHK_BOX_OFF);
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,255));
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));

		pChk->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(192,192,192));
		pChk->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(192,192,192));
		pChk->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(192,192,192));

		pChk->SetFont(&g_pSGSCheckDlg->m_fontDlgFont);
		pChk->DrawBorder(FALSE);
	};

	SetCHK(&m_chkQYCCoustLoad);
	SetCHK(&m_chkCYCCoustLoad);
	SetCHK(&m_chkQYCPLHP);
	SetCHK(&m_chkCYCPLHP);
	SetCHK(&m_chkGasChech);
	SetCHK(&m_chkLowFlowChech);
	SetCHK(&m_chkSmokeMeter);
	SetCHK(&m_chkNOxAnalyzeer);
	SetCHK(&m_chkFlowMeter);
	SetCHK(&m_chkWetherStation);
	SetCHK(&m_chkTachoMerter);


	auto SetPGB = [] (CXPGroupBox* pPGB)
	{
		pPGB->SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
		pPGB->SetBackgroundColor(RGB(192,192,192));
	};
	SetPGB(&m_BK1);
	SetPGB(&m_BK3);


	// 车速表
	CString str = _T("km/h");
	m_stSpeedometer.SetUnits(str);						// 设置单位
	m_stSpeedometer.SetTitle(_T(""));					// 标题
	m_stSpeedometer.SetRange(0,90);					// 设置刻度范围
	m_stSpeedometer.SetTicks(9);						// 设置刻度数
	m_stSpeedometer.SetTickScale(1);					// 设置比例尺
	m_stSpeedometer.SetValueDecimals(1);				// 设置小数位
	m_stSpeedometer.SetSubTicks(10);				// 设置子刻度（每大格分为几小格）
	m_stSpeedometer.SetValueColor(RGB(255,255,255));	// 设置显示值颜色
	m_stSpeedometer.SetFrameColor(RGB(192,192,192));	// 设置外框背景颜色
	m_stSpeedometer.SetMeterBkColor(RGB(192,192,192));	//设置表盘背景颜色
	//m_stSpeedometer.SetFrameBorder(1000, RGB(255,255,255));
	m_stSpeedometer.SetNeedleColor(RGB(0,255,0)); //设置指针颜色
	m_stSpeedometer.UpdateNeedle(0);
	
	// 转速表
	str = _T("x1000r/min");
	m_stTachometer.SetUnits(str);						// 设置单位
	m_stTachometer.SetTitle(_T(""));
	m_stTachometer.SetRange(0,4);					// 设置刻度范围
	m_stTachometer.SetTicks(4);						// 设置刻度数
	m_stTachometer.SetTickScale(1);					// 设置比例尺
	m_stTachometer.SetValueDecimals(1);				// 设置小数位
	m_stTachometer.SetValueColor(RGB(58, 248, 250));	// 设置显示值颜色
	m_stTachometer.SetSubTicks(10);				// 设置子刻度（每大格分为几小格）
	m_stTachometer.SetValueColor(RGB(255,255,255));	// 设置显示值颜色
	m_stTachometer.SetFrameColor(RGB(192,192,192));	// 设置外框背景颜色
	m_stTachometer.SetMeterBkColor(RGB(192,192,192));	//设置表盘背景颜色
	m_stTachometer.SetNeedleColor(RGB(0,255,0)); //设置指针颜色
	m_stTachometer.SetAngleRange(140, 310);
	m_stTachometer.UpdateNeedle(0);

	//加载CPanel
	if (true)
	{
		CRect rect;
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_ST_BK2);
		pWnd->GetWindowRect(&rect);  
		//GetDlgItem(IDC_ST_BK2)->GetClientRect(&rect);
		//创建面板
		CPanel* panel = new CPanel();
		panel->Create(&rect, this);

		CPanelItem* pItem1 = new CPanelItem();
		pItem1->Create(IDD_SELF_INSRECTION_ITEMS_DIALOG, _T(""), RGB(74,141,186), panel, 0);
		pItem1->ShowWindow(SW_SHOW);
		panel->AddPanel(pItem1);
	}
}

void CSGSCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);

		TCHAR tchs[20] = {0};
		GetClassName(pWnd->GetSafeHwnd(), tchs, 20);
		if (_tcscmp(tchs, _T("ComboBox")) == 0)
		{
			((CComboBox *)pWnd)->SetItemHeight(-1, 25);
		}

		pWnd = pWnd->GetNextWindow();
	}
}
