
// SGSCheDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SGSChe.h"
#include "SGSCheDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSGSCheDlg 对话框

#define SCROLLBAR_JUMP 10


CSGSCheDlg::CSGSCheDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CSGSCheDlg::IDD, pParent)
	, m_SelfInsrectionItemsDlg(IDD_SELF_INSRECTION_ITEMS_DIALOG, this)
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

CSGSCheDlg::~CSGSCheDlg()
{
	m_fontDlgFont.DeleteObject();

	::DeleteObject(m_hrPicture);
	m_hrPicture = NULL;

}

void CSGSCheDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_BTN_STATR, m_btnState);
	DDX_Control(pDX, IDC_BTN_RESTATR, m_btnReState);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_ST_SET_INFO_TIMES, m_lbEquSelfIns);

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

	DDX_Control(pDX, IDC_ST_1, m_lb1);
	DDX_Control(pDX, IDC_ST_2, m_lb2);
	DDX_Control(pDX, IDC_ST_3, m_lb3);
	DDX_Control(pDX, IDC_ST_4, m_lb4);
	DDX_Control(pDX, IDC_ST_5, m_lb5);
	DDX_Control(pDX, IDC_ST_6, m_lb6);
	DDX_Control(pDX, IDC_ST_7, m_lb7);
	DDX_Control(pDX, IDC_ST_8, m_lb8);
	DDX_Control(pDX, IDC_ST_9, m_lb9);
	DDX_Control(pDX, IDC_ST_10, m_lb10);
	DDX_Control(pDX, IDC_ST_11, m_lb11);
	DDX_Control(pDX, IDC_ST_12, m_lb12);
	DDX_Control(pDX, IDC_ST_13, m_lb13);
	DDX_Control(pDX, IDC_ST_14, m_lb14);
	DDX_Control(pDX, IDC_ST_15, m_lb15);
	DDX_Control(pDX, IDC_ST_16, m_lb16);
	DDX_Control(pDX, IDC_ST_17, m_lb17);
	DDX_Control(pDX, IDC_ST_18, m_lb18);

	DDX_Control(pDX, IDC_ST_NIULI, m_lbNiuLi);
	DDX_Control(pDX, IDC_ST_GONGLV, m_lbGongLv);
	DDX_Control(pDX, IDC_ST_CO, m_lbCO);
	DDX_Control(pDX, IDC_ST_HC, m_lbHC);
	DDX_Control(pDX, IDC_ST_CO22, m_lbCO2);
	DDX_Control(pDX, IDC_ST_NO, m_lbNO);
	DDX_Control(pDX, IDC_ST_O2, m_lbO2);
	DDX_Control(pDX, IDC_ST_PEF, m_lbPEF);
	DDX_Control(pDX, IDC_ST_NO2, m_lbCO22);
	DDX_Control(pDX, IDC_ST_N, m_lbN);
	DDX_Control(pDX, IDC_ST_CO2, m_lbNO2);
	DDX_Control(pDX, IDC_ST_NS, m_lbNs);

	DDX_Control(pDX, IDC_ST_LLJ_O2, m_lbLLJO2);
	DDX_Control(pDX, IDC_ST_LLJ_BZ, m_lbLLJBZ);
	DDX_Control(pDX, IDC_ST_LLJ_SJ, m_lbLLJSJ);
}

BEGIN_MESSAGE_MAP(CSGSCheDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_STATR, &CSGSCheDlg::OnBnClickedBtnStatr)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSGSCheDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_RESTATR, &CSGSCheDlg::OnBnClickedBtnRestatr)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSGSCheDlg 消息处理程序

BOOL CSGSCheDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 初始化控件
	InitCtrls();
	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);

	LoadSubDlg();
	SetTimer(1, 1000, NULL);

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSGSCheDlg::OnPaint()
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
HCURSOR CSGSCheDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSGSCheDlg::OnBnClickedBtnStatr()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSGSCheDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSGSCheDlg::OnBnClickedBtnRestatr()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSGSCheDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1: // 显示信息
		{
			CString strMsg;
			strMsg.Format(L"%s   %s", m_strSelfInfoTimes, COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			m_lbEquSelfIns.SetWindowTextW(strMsg);
		}
	}
	CDialogZoom::OnTimer(nIDEvent);
}


void CSGSCheDlg::InitCtrls(void)
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
	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"StationInfo.ini", wchPath, false))
	{
		CSimpleIni si(wchPath);
		m_strSelfInfoTimes = si.GetString(L"StationInfo", L"Name", L"｜  设备自检  |  |  堵住但氮氧分析仪取样探头进气口后点击\"开始检漏\"按钮进行检漏  |");
	}
	m_lbEquSelfIns.SetFontEx(_T("宋体"), 20);
	m_lbEquSelfIns.SetWindowTextW(m_strSelfInfoTimes);

	auto SetCHK = [] (CButtonST* pChk)
	{
		pChk->SetIcon(IDI_ICON_CHK_BOX_ON, IDI_ICON_CHK_BOX_OFF);
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,255));
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));

		pChk->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(105,105,105));
		pChk->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(105,105,105));
		pChk->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(105,105,105));

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
	m_stSpeedometer.SetFrameColor(RGB(105,105,105));	// 设置外框背景颜色
	m_stSpeedometer.SetMeterBkColor(RGB(105,105,105));	//设置表盘背景颜色
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
	m_stTachometer.SetFrameColor(RGB(105,105,105));	// 设置外框背景颜色
	m_stTachometer.SetMeterBkColor(RGB(105,105,105));	//设置表盘背景颜色
	m_stTachometer.SetNeedleColor(RGB(0,255,0)); //设置指针颜色
	m_stTachometer.SetAngleRange(140, 310);
	m_stTachometer.UpdateNeedle(0);


	auto SetLbT = [] (CLabel* plb)
	{
		plb->SetBkTransparent();
		plb->SetTextColor(RGB(255,255,255));
		plb->AutoFitFont();
	};
	SetLbT(&m_lb1);
	SetLbT(&m_lb2);
	SetLbT(&m_lb3);
	SetLbT(&m_lb4);
	SetLbT(&m_lb5);
	SetLbT(&m_lb6);
	SetLbT(&m_lb7);
	SetLbT(&m_lb8);
	SetLbT(&m_lb9);
	SetLbT(&m_lb10);
	SetLbT(&m_lb11);
	SetLbT(&m_lb12);
	SetLbT(&m_lb13);
	SetLbT(&m_lb14);
	SetLbT(&m_lb15);
	SetLbT(&m_lb16);
	SetLbT(&m_lb17);
	SetLbT(&m_lb18);

	auto SetLb = [] (CLabel* plb)
	{
		plb->SetBkColor(COLOR_BTNFACE);
		plb->SetTextColor(RGB(255,255,255));
		plb->AutoFitFont();
	};

	SetLb(&m_lbNiuLi);
	SetLb(&m_lbGongLv);
	SetLb(&m_lbCO);
	SetLb(&m_lbHC);
	SetLb(&m_lbNO);
	SetLb(&m_lbCO2);
	SetLb(&m_lbO2);
	SetLb(&m_lbPEF);
	SetLb(&m_lbNO2);
	SetLb(&m_lbCO22);
	SetLb(&m_lbN);
	SetLb(&m_lbNs);
	SetLb(&m_lbLLJO2);
	SetLb(&m_lbLLJBZ);
	SetLb(&m_lbLLJSJ);

	m_lbNiuLi.SetWindowTextW(L"0");
	m_lbHC.SetWindowTextW(L"0");
	m_lbNO.SetWindowTextW(L"0");
	m_lbNO2.SetWindowTextW(L"0");
	m_lbGongLv.SetWindowTextW(L"0.00");
	m_lbCO.SetWindowTextW(L"0.00");
	m_lbCO2.SetWindowTextW(L"0.00");
	m_lbO2.SetWindowTextW(L"0.00");
	m_lbN.SetWindowTextW(L"0.00");
	m_lbNs.SetWindowTextW(L"0.00");
	m_lbCO22.SetWindowTextW(L"0.00");
	m_lbPEF.SetWindowTextW(L"0.000");

	m_lbLLJO2.SetWindowTextW(L"0.00");
	m_lbLLJBZ.SetWindowTextW(L"0.0");
	m_lbLLJSJ.SetWindowTextW(L"0.0");

}

void CSGSCheDlg::SetDlgFont(void)
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


void CSGSCheDlg::LoadSubDlg(void)
{
	CRect rect;
	GetDlgItem(IDC_ST_RECT_DETITEM)->GetWindowRect(rect);	
	ScreenToClient(&rect); 

	m_SelfInsrectionItemsDlg.Create(IDD_SELF_INSRECTION_ITEMS_DIALOG, this);
	m_SelfInsrectionItemsDlg.MoveWindow(rect);
	m_SelfInsrectionItemsDlg.SetupScrollbars();
	m_SelfInsrectionItemsDlg.ShowWindow(SW_SHOW);

}

HBRUSH CSGSCheDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogZoom::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	
	//switch(nCtlColor)
	//{
	//case IDC_ST_PICTURE_BK_1:
	//	{
	//		m_hrPicture = ::CreateSolidBrush(RGB(255,255,255));
	//		hbr = (HBRUSH)m_hrPicture;
	//	}
	//	break;
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
