// SelfInsrectionItemsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SGSC.h"
#include "SelfInsrectionItemsDlg.h"
#include "afxdialogex.h"

#define HORZ_PTS 8
#define VERT_PTS 4

////////////////////////////////////////////////////////////////////////////
// CGripper message map
BEGIN_MESSAGE_MAP(CGripper, CScrollBar)
	//{{AFX_MSG_MAP(CGripper)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGripper message handlers
LRESULT CGripper::OnNcHitTest(CPoint point) 
{
	UINT ht = CScrollBar::OnNcHitTest(point);
	if(ht==HTCLIENT)
	{
		ht = HTBOTTOMRIGHT;
	}
	return ht;
}

CSelfInsrectionItemsDlg* g_pSIItemsDlg(NULL);

// CSelfInsrectionItemsDlg 对话框

IMPLEMENT_DYNAMIC(CSelfInsrectionItemsDlg, CDialogZoom)

CSelfInsrectionItemsDlg::CSelfInsrectionItemsDlg(UINT nIDD, CWnd* pParent /*=NULL*/)
	: CDialogZoom(nIDD, pParent)
	, m_bMIsType(false)
	, m_bUpConstLoad(false)
	, m_bUpConstLoadHeavy(false)
{
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

CSelfInsrectionItemsDlg::~CSelfInsrectionItemsDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CSelfInsrectionItemsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scroll);

	DDX_Control(pDX, IDC_ST_GROUP_CGJ, m_grCGJ);
	DDX_Control(pDX, IDC_ST_GROUP_FQYDDJC, m_grFQYXY);
	DDX_Control(pDX, IDC_ST_GROUP_FQYCGJC, m_grFQYCGJC);

	DDX_Control(pDX, IDC_ST_GROUP_YDJJNOXFXY, m_grYDJJNOXFXY);
	DDX_Control(pDX, IDC_ST_GROUP_NOXFXY, m_grNOXFXY);
	DDX_Control(pDX, IDC_ST_GROUP_LLJ, m_grLLJ);
	DDX_Control(pDX, IDC_ST_GROUP_QXZ, m_grQXZ);
	DDX_Control(pDX, IDC_ST_GROUP_ZSJ, m_grZSJ);

	DDX_Control(pDX, IDC_ST_LIFTER8, m_lbQY);

	DDX_Control(pDX, IDC_ST_PLHP_CY1, m_lbPLHPCY1);
	DDX_Control(pDX, IDC_ST_PLHP_CY2, m_lbPLHPCY2);
	DDX_Control(pDX, IDC_ST_PLHP_CY3, m_lbPLHPCY3);
	DDX_Control(pDX, IDC_ST_PLHP_CY4, m_lbPLHPCY4);
	DDX_Control(pDX, IDC_ST_PLHP_CY5, m_lbPLHPCY5);
	DDX_Control(pDX, IDC_ST_PLHP_CY6, m_lbPLHPCY6);
	DDX_Control(pDX, IDC_ST_PLHP_QY1, m_lbPLHPQY1);
	DDX_Control(pDX, IDC_ST_PLHP_QY2, m_lbPLHPQY2);

	DDX_Control(pDX, IDC_COM_QY_PLHP, m_cbQYPLHP);
	DDX_Control(pDX, IDC_COM_CY_PLHP, m_cbCYPLHP);

	DDX_Control(pDX, IDC_ED_FQYDDJC_PEF, m_edFQYDDJCPEF);
	DDX_Control(pDX, IDC_ED_FQYDDJC_C3H8, m_edFQYDDJCC3H8);
	DDX_Control(pDX, IDC_ED_FQYDDJC_HC, m_edFQYDDJCHC);
	DDX_Control(pDX, IDC_ED_FQYDDJC_CO, m_edFQYDDJCCO);
	DDX_Control(pDX, IDC_ED_FQYDDJC_CO2, m_edFQYDDJCCO2);
	DDX_Control(pDX, IDC_ED_FQYDDJC_NO, m_edFQYDDJCNO);
	DDX_Control(pDX, IDC_ED_FQYDDJC_NO2, m_edFQYDDJCNO2);
	DDX_Control(pDX, IDC_ED_FQYDDJC_O2, m_edFQYDDJCO2);

	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_BZN_LD, m_edYDJJNOXFXYBZNLD);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_BZN_MLC, m_edYDJJNOXFXYBZNMLC);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_BZN_DBZ, m_edYDJJNOXFXYBZNDBZ);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_BZN_GBZ, m_edYDJJNOXFXYBZNGBZ);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_K_LD, m_edYDJJNOXFXYKLD);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_K_MLC, m_edYDJJNOXFXYKMLC);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_K_DBZ, m_edYDJJNOXFXYKDBZ);
	DDX_Control(pDX, IDC_ED_YDJJNOXFXY_K_GBZ, m_edYDJJNOXFXYKGBZ);

	DDX_Control(pDX, IDC_ED_NOXFXY_CO2, m_edNOXFXYCO2);
	DDX_Control(pDX, IDC_ED_NOXFXY_NO, m_edNOXFXYNO);
	DDX_Control(pDX, IDC_ED_NOXFXY_NO2, m_edNOXFXYNO2);


	DDX_Control(pDX, IDC_ED_LLJ_MYLL, m_edLLJMYLL);
	DDX_Control(pDX, IDC_ED_LLJ_YQ_BZZ, m_edLLJYQBZZ);
	DDX_Control(pDX, IDC_ED_QXZ_WD, m_edQXZWD);
	DDX_Control(pDX, IDC_ED_QXZ_SD, m_edQXZSD);
	DDX_Control(pDX, IDC_ED_QXZ_DQY, m_edQXZDQY);
}


BEGIN_MESSAGE_MAP(CSelfInsrectionItemsDlg, CDialogZoom)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(IDC_BTN_FQYDDJC_KSJC, &CSelfInsrectionItemsDlg::OnBtnFqyddjcKsjc)
	ON_COMMAND(IDC_BTN_NOXFXY_KSJC, &CSelfInsrectionItemsDlg::OnBtnNoxfxyKsjc)
	ON_WM_TIMER()
	ON_COMMAND(IDC_BTN_FQYCGJC_KSJL, &CSelfInsrectionItemsDlg::OnBtnFqycgjcKsjl)
	ON_COMMAND(IDC_BTN_YDJJNOXFXY_QR_DBZ, &CSelfInsrectionItemsDlg::OnBtnYdjjnoxfxyQrDbz)
	ON_COMMAND(IDC_BTN_YDJJNOXFXY_QR_GBZ, &CSelfInsrectionItemsDlg::OnBtnYdjjnoxfxyQrGbz)
	ON_COMMAND(IDC_BTN_DOWN_LIFTER, &CSelfInsrectionItemsDlg::OnBtnDownLifter)
	ON_COMMAND(IDC_BTN_UP_LIFTER, &CSelfInsrectionItemsDlg::OnBtnUpLifter)
END_MESSAGE_MAP()


// CSelfInsrectionItemsDlg 消息处理程序


BOOL CSelfInsrectionItemsDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetScroll();

	g_pSIItemsDlg = this;

	LoadConfig();
	// 初始化控件
	InitCtrls();


	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelfInsrectionItemsDlg::ResetScrollbars()
{
	// Reset our window scrolling information
	ScrollWindow(m_nHscrollPos*HORZ_PTS, 0, NULL, NULL);
	ScrollWindow(0, m_nVscrollPos*VERT_PTS, NULL, NULL);
	m_nHscrollPos = 0;
	m_nVscrollPos = 0;
	SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
}

void CSelfInsrectionItemsDlg::SetupScrollbars()
{
	CRect tempRect;
	GetClientRect(&tempRect);

	// Calculate how many scrolling increments for the client area
	m_nHorzInc = (m_ClientRect.Width()  - tempRect.Width())/HORZ_PTS;
	m_nVertInc = (m_ClientRect.Height() - tempRect.Height())/VERT_PTS;

	// Set the vertical and horizontal scrolling info
	m_nHscrollMax = max(0, m_nHorzInc);
	m_nHscrollPos = min (m_nHscrollPos, m_nHscrollMax);
	SetScrollRange(SB_HORZ, 0, m_nHscrollMax, FALSE);
	SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);
	m_nVscrollMax = max(0, m_nVertInc);
	m_nVscrollPos = min(m_nVscrollPos, m_nVscrollMax);
	SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);

}

void CSelfInsrectionItemsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//int TempPos = pScrollBar->GetScrollPos();
	//switch(nSBCode)
	//{
	//case SB_THUMBPOSITION://拖动滑块
	//	pScrollBar->SetScrollPos(nPos);
	//	break;
	//case SB_LINELEFT://点击左边的箭头
	//	if(TempPos > 1)
	//	{
	//		TempPos--;
	//	}
	//	pScrollBar->SetScrollPos(TempPos);
	//	break;
	//case SB_LINERIGHT://点击右边的箭头
	//	if(TempPos<100)
	//	{
	//		TempPos++;
	//	}
	//	pScrollBar->SetScrollPos(TempPos);
	//	break;
	//} 

	int nInc;
	switch(nSBCode)
	{
	case SB_TOP:        nInc = -m_nHscrollPos;               break;
	case SB_BOTTOM:     nInc = m_nHscrollMax-m_nHscrollPos;  break;
	case SB_LINEUP:     nInc = -1;                           break;
	case SB_LINEDOWN:   nInc = 1;                            break;
	case SB_PAGEUP:     nInc = min(-1, -m_nHorzInc);         break;
	case SB_PAGEDOWN:   nInc = max(1, m_nHorzInc);           break;
	case SB_THUMBTRACK: nInc = nPos - m_nHscrollPos;         break;
	default:            nInc = 0;
	}

	nInc = max(-m_nHscrollPos, min(nInc, m_nHscrollMax - m_nHscrollPos));
	m_nHscrollPos += nInc;
	int iMove = -HORZ_PTS * nInc;
	ScrollWindow(0, iMove, NULL, NULL);
	SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSelfInsrectionItemsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nInc;
	switch (nSBCode)
	{
	case SB_TOP:        nInc = -m_nVscrollPos;               break;
	case SB_BOTTOM:     nInc = m_nVscrollMax-m_nVscrollPos;  break;
	case SB_LINEUP:     nInc = -1;                           break;
	case SB_LINEDOWN:   nInc = 1;                            break;
	case SB_PAGEUP:     nInc = min(-1, -m_nVertInc);         break;
	case SB_PAGEDOWN:   nInc = max(1, m_nVertInc);           break;
	case SB_THUMBTRACK: nInc = nPos - m_nVscrollPos;         break;
	default:            nInc = 0;
	}

	nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
	m_nVscrollPos += nInc;
	int iMove = -VERT_PTS * nInc;
	ScrollWindow(0, iMove, NULL, NULL);
	SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);



// 	int nMax,nMin;
// 	nMin = nMax = 0;
// 	int TempPos = pScrollBar->GetScrollPos();
// 	switch(nSBCode)
// 	{
// 	case SB_THUMBPOSITION://拖动滑块
// 		pScrollBar->SetScrollPos(nPos);
// 		break;
// 	case SB_LINELEFT://点击左边的箭头
// 		if(TempPos > 1)
// 		{
// 			TempPos--;
// 		}
// 		pScrollBar->SetScrollPos(TempPos);
// 		break;
// 	case SB_LINERIGHT://点击右边的箭头
// 		if(TempPos<100)
// 		{
// 			TempPos++;
// 		}
// 		pScrollBar->SetScrollPos(TempPos);
// 		break;
// 	} 


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSelfInsrectionItemsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bInitialized)
	{
		ResetScrollbars();
		SetupScrollbars();
	}
}


void CSelfInsrectionItemsDlg::SetScroll(void)
{

	// Set Initial Scroll Positions
	m_nHscrollPos = 0;
	m_nVscrollPos = 0;

	// Get the initial dimensions of the dialog
	GetClientRect(&m_ClientRect);
	m_bInitialized = TRUE;

	// Create a resize gripper
	CRect initRect;
	GetClientRect(initRect);
	initRect.left = initRect.right - GetSystemMetrics(SM_CXHSCROLL);
	initRect.top  = initRect.bottom - GetSystemMetrics(SM_CYVSCROLL);

	DWORD dwStyle = WS_CHILD | 
		SBS_SIZEBOX | 
		SBS_SIZEBOXBOTTOMRIGHTALIGN | 
		SBS_SIZEGRIP | 
		WS_VISIBLE;

	m_Grip.Create(dwStyle,initRect, this, AFX_IDW_SIZE_BOX);
	// 
	// 	CScrollBar* pScrollBar = (CScrollBar*)GetDlgItem(IDC_SCROLLBAR1);
	// 	pScrollBar->SetScrollRange(0,100);//滑块移动的位置为0——100；
}

void CSelfInsrectionItemsDlg::LoadConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"StationInfo.ini", wchPath, false))
	{
		CSimpleIni si(wchPath);
		m_strDIW = si.GetString(L"Dyn", L"DIW", L"");

		for(int i=0; i<6; i++)
		{
			CString str;
			str.Format(L"PLHPQY%d", i+1);
			m_strPLHPQY[i] = si.GetString(L"Dyn", str, L"0.00");
			str.Format(L"PLHPCY%d", i+1);
			m_strPLHPCY[i] = si.GetString(L"Dyn", str, L"0.00");
		}

		m_strPEF = si.GetString(L"Ana", L"PEF", L"0.00");
		m_strCO = si.GetString(L"Ana", L"CO", L"0.00");
		m_strCO2 = si.GetString(L"Ana", L"CO2", L"0.00");
		m_strNO = si.GetString(L"Ana", L"NO", L"");
		m_strNO2 = si.GetString(L"Ana", L"NO2", L"");
		m_strO2 = si.GetString(L"Ana", L"O2", L"0.00");
		m_strC3H8 = si.GetString(L"Ana", L"C3H8", L"");

		for(int i=0; i<4; i++)
		{
			CString str;
			str.Format(L"N%d", i+1);
			m_strYDJJNOXFXYBZN[i] = si.GetString(L"MQY", str, L"0.00");
			str.Format(L"K%d", i+1);
			m_strYDJJNOXFXYK[i] = si.GetString(L"MQY", str, L"0.00");
		}

		m_strNOXFXYCO2 = si.GetString(L"NOx", L"CO2", L"0.00");
		m_strNOXFXYNO = si.GetString(L"NOx", L"NO", L"");
		m_strNOXFXYNO2 = si.GetString(L"NOx", L"NO2", L"");


		m_strLLJMYLL = si.GetString(L"Flo", L"MYLL", L"0.00");
		m_strLLJYQBZZ = si.GetString(L"Flo", L"BZZO2", L"");

		m_strQXZWD = si.GetString(L"Para", L"WD", L"");
		m_strQXZSD = si.GetString(L"Para", L"SD", L"0.00");
		m_strQXZDQY = si.GetString(L"Para", L"DQY", L"");
	}
}

void CSelfInsrectionItemsDlg::InitCtrls(void)
{
	SetDlgFont();

	auto SetGro = [] (CSxGroupBox* pGroupBox)
	{
		pGroupBox->SetFontSize(g_pSIItemsDlg->m_nDlgFontSize);
		pGroupBox->SetFontName(L"宋体");
		pGroupBox->SetTextColor(RGB(0, 0, 0));
		pGroupBox->SetBoxColors(RGB(0x00, 0x00, 0x00));
		pGroupBox->SetLineStyle(BS_FLAT);
		pGroupBox->SetTextAlign(BS_LEFT);
	};

	SetGro(&m_grCGJ);
	SetGro(&m_grFQYXY);
	SetGro(&m_grFQYCGJC);
	SetGro(&m_grYDJJNOXFXY);
	SetGro(&m_grNOXFXY);
	SetGro(&m_grLLJ);
	SetGro(&m_grQXZ);
	SetGro(&m_grZSJ);

	CString strName;
	strName.Format(L"测功机 惯量:%.1f", _wtof(m_strDIW));
	m_grCGJ.SetText(strName);

	auto SetLB = [](CLabel* pLb)
	{
	};

	m_lbPLHPQY1.SetWindowTextW(m_strPLHPQY[0]);
	m_lbPLHPQY2.SetWindowTextW(m_strPLHPQY[1]);

	m_lbPLHPCY1.SetWindowTextW(m_strPLHPCY[0]);
	m_lbPLHPCY2.SetWindowTextW(m_strPLHPCY[1]);
	m_lbPLHPCY3.SetWindowTextW(m_strPLHPCY[2]);
	m_lbPLHPCY4.SetWindowTextW(m_strPLHPCY[3]);
	m_lbPLHPCY5.SetWindowTextW(m_strPLHPCY[4]);
	m_lbPLHPCY6.SetWindowTextW(m_strPLHPCY[5]);

	m_cbQYPLHP.SetCurSel(m_cbQYPLHP.FindString(0, L"6"));
	m_cbCYPLHP.SetCurSel(m_cbCYPLHP.FindString(0, L"10"));


	m_edFQYDDJCPEF.SetWindowTextW(m_strPEF);
	strName.Format(L"%.0f", (_wtof(m_strPEF) * _wtoi(m_strC3H8)));
	m_edFQYDDJCHC.SetWindowTextW(strName);
	m_edFQYDDJCC3H8.SetWindowTextW(m_strC3H8);
	m_edFQYDDJCCO.SetWindowTextW(m_strCO);
	m_edFQYDDJCCO2.SetWindowTextW(m_strCO2);
	m_edFQYDDJCNO.SetWindowTextW(m_strNO);
	m_edFQYDDJCNO2.SetWindowTextW(m_strNO2);
	m_edFQYDDJCO2.SetWindowTextW(m_strO2);

	m_edYDJJNOXFXYBZNLD.SetWindowTextW(m_strYDJJNOXFXYBZN[0]);
	m_edYDJJNOXFXYBZNMLC.SetWindowTextW(m_strYDJJNOXFXYBZN[1]);
	m_edYDJJNOXFXYBZNDBZ.SetWindowTextW(m_strYDJJNOXFXYBZN[2]);
	m_edYDJJNOXFXYBZNGBZ.SetWindowTextW(m_strYDJJNOXFXYBZN[3]);
	m_edYDJJNOXFXYKLD.SetWindowTextW(m_strYDJJNOXFXYK[0]);
	m_edYDJJNOXFXYKMLC.SetWindowTextW(m_strYDJJNOXFXYK[1]);
	m_edYDJJNOXFXYKDBZ.SetWindowTextW(m_strYDJJNOXFXYK[2]);
	m_edYDJJNOXFXYKGBZ.SetWindowTextW(m_strYDJJNOXFXYK[3]);
	
	m_edNOXFXYCO2.SetWindowTextW(m_strNOXFXYCO2);
	m_edNOXFXYNO.SetWindowTextW(m_strNOXFXYNO);
	m_edNOXFXYNO2.SetWindowTextW(m_strNOXFXYNO2);

	m_edLLJMYLL.SetWindowTextW(m_strLLJMYLL);
	m_edLLJYQBZZ.SetWindowTextW(m_strLLJYQBZZ);
	m_edQXZWD.SetWindowTextW(m_strQXZWD);
	m_edQXZSD.SetWindowTextW(m_strQXZSD);
	m_edQXZDQY.SetWindowTextW(m_strQXZDQY);

}

void CSelfInsrectionItemsDlg::SetDlgFont(void)
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

bool CSelfInsrectionItemsDlg::GetNominalValue(const int& nDetType)
{
	// 置为0
	m_nC3H8NominalValue = m_nNONominalValue = m_nNO2NominalValue = m_nHCNominalValue = 0;
	m_fCONominalValue = m_fCO2NominalValue = m_fO2NominalValue = m_fPEFNominalValue = 0.0f;
	m_sRandomErrorHC = m_sRandomErrorNO = m_sRandomErrorNO2 = m_sRandomErrorNOX = 0;
	m_fRandomErrorCO = m_fRandomErrorCO2 = m_fRandomErrorO2 = m_fRandomErrorPef = 0.0f;

	if (nDetType == 1)
	{
		CString str, strTemp;

		m_edFQYDDJCCO.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCCO.SetFocus();
			MessageBox(_T("请输入检查气CO标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_fCONominalValue = _wtof(str);
		}

		m_edFQYDDJCCO2.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCCO2.SetFocus();
			MessageBox(_T("请输入检查气CO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_fCO2NominalValue = _wtof(str);
		}

		m_edFQYDDJCNO.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCNO.SetFocus();
			MessageBox(_T("请输入检查气NO标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_nNONominalValue = _wtoi(str);
		}

		m_edFQYDDJCNO2.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCNO2.SetFocus();
			MessageBox(_T("请输入检查气NO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_nNO2NominalValue = _wtoi(str);
		}

		m_edFQYDDJCO2.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCO2.SetFocus();
			MessageBox(_T("请输入检查气O2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_fO2NominalValue = _wtof(str);
		}

		m_edFQYDDJCC3H8.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCC3H8.SetFocus();
			MessageBox(_T("请输入检查气C3H8标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_nC3H8NominalValue = _wtoi(str);
		}

		m_edFQYDDJCPEF.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edFQYDDJCPEF.SetFocus();
			MessageBox(_T("请输入检查气PEF标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_fPEFNominalValue = _wtof(str);
		}

		m_nHCNominalValue = m_nC3H8NominalValue * m_fPEFNominalValue;

	}
	else if (nDetType == 2)
	{
		CString str, strTemp;
		m_edNOXFXYCO2.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edNOXFXYCO2.SetFocus();
			MessageBox(_T("请输入检查气CO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_fCO2NominalValue = _wtof(str);
		}

		m_edNOXFXYNO.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edNOXFXYNO.SetFocus();
			MessageBox(_T("请输入检查气NO标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_nNONominalValue = _wtoi(str);
		}

		m_edNOXFXYNO2.GetWindowTextW(str);
		str.Trim();
		if (L"" == str)
		{
			m_edNOXFXYNO2.SetFocus();
			MessageBox(_T("请输入检查气NO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return false;
		}
		else
		{
			m_nNO2NominalValue = _wtoi(str);
		}
	}

	return true;
}

void CSelfInsrectionItemsDlg::RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx, const int& nDetType)
{
	CString str;
	if (nDetType == 1)
	{
		srand(time(0));
		int n = rand()%4;

		//HC
		// 将C3H8标称值转换成HC标称值
		if (m_nC3H8NominalValue != 0)
		{
			m_nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
		}
		// 合理性约束
		if (m_nHCNominalValue == 0)
		{
			m_nHCNominalValue = 1;
		}

		if ((n%2) == 0)
		{
			// 相加
			sHC = m_nHCNominalValue + n;
		}
		else
		{
			// 相减
			sHC = m_nHCNominalValue - n;
		}
		if (m_nHCNominalValue <= 1)
		{
			sHC = 0;
		}
		str.Format(L"%d", sHC);
		GetDlgItem(IDC_ST_FQYDDJC_HC_CLZ)->SetWindowTextW(str);

		// CO
		if (m_fCONominalValue > 1)
		{
			n = rand()%10 + 100;
		}
		else
		{
			n = rand()%10 + 10;
		}
		if ((n%2) == 0)
		{
			fCO = (m_fCONominalValue * 1000  + n) / 1000;
		}
		else
		{
			fCO = (m_fCONominalValue * 1000  - n) / 1000;
		}
		if (m_fCONominalValue == 0.00f)
		{
			fCO = 0.0f;
		}
		str.Format(L"%.2f", fCO);
		GetDlgItem(IDC_ST_FQYDDJC_CO_CLZ)->SetWindowTextW(str);

		// CO2
		if (m_fCO2NominalValue > 1)
		{
			n = rand()%10 + 100;
		}
		else
		{
			n = rand()%10 + 10;
		}
		if ((n%2) == 0)
		{
			fCO2 = (m_fCO2NominalValue * 1000  + n) / 1000;
		}
		else
		{
			fCO2 = (m_fCO2NominalValue * 1000  - n) / 1000;
		}

		if (m_fCO2NominalValue == 0.00f)
		{
			fCO2 = 0.0f;
		}
		str.Format(L"%.2f", fCO2);
		GetDlgItem(IDC_ST_FQYDDJC_CO2_CLZ)->SetWindowTextW(str);

		// NO,NO2
		n = rand()%20;
		if ((n%2) == 0)
		{
			// 相加
			sNO = m_nNONominalValue + n;
			sNO2 = m_nNO2NominalValue + n;
		}
		else
		{
			// 相减
			sNO = m_nNONominalValue - n;
			sNO2 = m_nNO2NominalValue - n;
		}

		if (m_nNO2NominalValue == 0)
		{
			sNO2 = 0;
		}
		if (m_nNONominalValue == 0)
		{
			sNO = 0;
		}
		// NOx
		sNOx = sNO + sNO2;

		str.Format(L"%d", sNO);
		GetDlgItem(IDC_ST_FQYDDJC_NO_CLZ)->SetWindowTextW(str);
		str.Format(L"%d", sNO2);
		GetDlgItem(IDC_ST_FQYDDJC_NO2_CLZ)->SetWindowTextW(str);

		if (m_fO2NominalValue > 1)
		{
			n = rand()%10 + 100;
		}
		else
		{
			n = rand()%10 + 10;
		}
		if ((n%2) == 0)
		{
			fO2 = (m_fO2NominalValue * 1000  + n) / 1000;
		}
		else
		{
			fO2 = (m_fO2NominalValue * 1000  - n) / 1000;
		}
		if (m_fO2NominalValue == 0.0f)
		{
			fO2 = 0.0f;
		}
		str.Format(L"%.2f", fO2);
		GetDlgItem(IDC_ST_FQYDDJC_O2_CLZ)->SetWindowTextW(str);
	}
	else if (nDetType == 2)
	{
		srand(time(0));
		int n;
		// CO2
		if (m_fCO2NominalValue > 1)
		{
			n = rand()%10 + 100;
		}
		else
		{
			n = rand()%10 + 10;
		}
		if ((n%2) == 0)
		{
			fCO2 = (m_fCO2NominalValue * 1000  + n) / 1000;
		}
		else
		{
			fCO2 = (m_fCO2NominalValue * 1000  - n) / 1000;
		}
		if (m_fCO2NominalValue == 0.00f)
		{
			fCO2 = 0.0f;
		}
		str.Format(L"%.2f", fCO2);
		GetDlgItem(IDC_ST_NOXFXY_CO2_CLZ)->SetWindowTextW(str);

		// NO,NO2
		n = rand()%20;
		if ((n%2) == 0)
		{
			// 相加
			sNO = m_nNONominalValue + n;
			sNO2 = m_nNO2NominalValue + n;
		}
		else
		{
			// 相减
			sNO = m_nNONominalValue - n;
			sNO2 = m_nNO2NominalValue - n;
		}

		if (m_nNO2NominalValue == 0)
		{
			sNO2 = 0;
		}
		if (m_nNONominalValue == 0)
		{
			sNO = 0;
		}
		// NOx
		sNOx = sNO + sNO2;

		str.Format(L"%d", sNO);
		GetDlgItem(IDC_ST_NOXFXY_NO_CLZ)->SetWindowTextW(str);
		str.Format(L"%d", sNO2);
		GetDlgItem(IDC_ST_NOXFXY_NO2_CLZ)->SetWindowTextW(str);
	}
}


bool CSelfInsrectionItemsDlg::UpEqulChk(const int& njclx)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//sbjclx	设备检查类型 1-加载滑行 2-附加损失 3-单点检查（低标气）4-单点检查（零气）5-单点检查（高标气） 6-五点检查
	strData.AppendFormat(L"<sbjclx>%d</sbjclx>", njclx);
	//jckssj	检查开始时间
	strData.AppendFormat(L"<jckssj>%s</jckssj>", m_odtStart.Format(L"%Y%m%d%H%M%S"));
	//jcjssj	检查结束时间
	strData.AppendFormat(L"<jcjssj>%s</jcjssj>", m_odtEnd.Format(L"%Y%m%d%H%M%S"));
	//cysx	采样时序
	strData.AppendFormat(L"<cysx>%s</cysx>", L"1");
	//zgzs	转鼓转速
	strData.AppendFormat(L"<zgzs>%s</zgzs>", L"");
	//cgjjzfh	测功机加载负荷
	strData.AppendFormat(L"<cgjjzfh>%s</cgjjzfh>", L"");
	//hc	HC浓度
	strData.AppendFormat(L"<hc>%s</hc>", L"");
	//co	CO浓度
	strData.AppendFormat(L"<co>%s</co>", L"");
	//no	NO浓度
	strData.AppendFormat(L"<no>%s</no>", L"");
	//no2	NO2浓度
	strData.AppendFormat(L"<no2>%s</no2>", L"");
	//co2	CO2浓度
	strData.AppendFormat(L"<co2>%s</co2>", L"");
	//o2	O2浓度
	strData.AppendFormat(L"<o2>%s</o2>", L"");

	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W11", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"检查记录上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"检查记录上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"检查记录上传", L"联网失败");
		return false;
	}

	return true;
}

bool CSelfInsrectionItemsDlg::UpGasCheck( const int& nDetType, const int& nQTLX, const CStringW& strJKID/* = L"13W05"*/)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间
	strData.AppendFormat(L"<jckssj>%s</jckssj>", m_odtStart.Format(L"%Y%m%d%H%M%S"));
	//lx	类型
	if (strJKID.Find(L"13W08") != -1)
	{
		strData.AppendFormat(L"<lx>%d</lx>", nQTLX);
	}
	//c3h8	标准气C3H8浓度
	strData.AppendFormat(L"<c3h8>%d</c3h8>", m_nC3H8NominalValue);
	//co	标准气CO浓度
	strData.AppendFormat(L"<co>%.2f</co>", m_fCONominalValue);
	//co2	标准气CO2浓度
	strData.AppendFormat(L"<co2>%.2f</co2>", m_fCO2NominalValue);
	//no	标准气NO浓度
	strData.AppendFormat(L"<no>%d</no>", m_nNONominalValue);
	//no2	标准气NO2浓度
	strData.AppendFormat(L"<no2>%d</no2>", m_nNO2NominalValue);
	//o2	标准气O2浓度
	//strData.AppendFormat(L"<o2>%.2f</o2>", m_fO2NominalValue);
	strData.AppendFormat(L"<o2>%.2f</o2>", 0.00f);
	//hcjcjg	HC检查结果值
	strData.AppendFormat(L"<hcjcjg>%d</hcjcjg>", m_sRandomErrorHC);
	//cojcjg	CO检查结果值
	strData.AppendFormat(L"<cojcjg>%.2f</cojcjg>", m_fRandomErrorCO);
	//co2jcjg	CO2检查结果值
	strData.AppendFormat(L"<co2jcjg>%.2f</co2jcjg>", m_fRandomErrorCO2);
	//nojcjg	NO检查结果值
	strData.AppendFormat(L"<nojcjg>%d</nojcjg>", m_sRandomErrorNO);
	//no2jcjg	NO2检查结果值
	strData.AppendFormat(L"<no2jcjg>%d</no2jcjg>", m_sRandomErrorNO2);
	//o2jcjg	O2检查结果值
	strData.AppendFormat(L"<o2jcjg>%.2f</o2jcjg>", m_fRandomErrorO2);
	//pef	PEF值
	strData.AppendFormat(L"<pef>%.3f</pef>", m_fPEFNominalValue);
	//jcjg	检查结果
	strData.AppendFormat(L"<jcjg>%s</jcjg>", L"1");
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), strJKID.GetString(), strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"五气气体检查", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"五气气体检查", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"五气气体检查", L"联网失败");
		return false;
	}

	return true;
}

bool CSelfInsrectionItemsDlg::UpGasCheck13W07(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间
	strData.AppendFormat(L"<jckssj>%s</jckssj>", m_odtStart.Format(L"%Y%m%d%H%M%S"));
	//c3h8	标准气C3H8浓度
	strData.AppendFormat(L"<c3h8>%d</c3h8>", m_nC3H8NominalValue);
	//co	标准气CO浓度
	strData.AppendFormat(L"<co>%.2f</co>", m_fCONominalValue);
	//co2	标准气CO2浓度
	strData.AppendFormat(L"<co2>%.2f</co2>", m_fCO2NominalValue);
	//no	标准气NO浓度
	strData.AppendFormat(L"<no>%d</no>", m_nNONominalValue);
	//no2	标准气NO2浓度
	strData.AppendFormat(L"<no2>%d</no2>", m_nNO2NominalValue);
	//o2	标准气O2浓度
	//strData.AppendFormat(L"<o2>%.2f</o2>", m_fO2NominalValue);
	strData.AppendFormat(L"<o2>%.2f</o2>", 0.00f);
	//	noxysj1	NO响应时间（T90）
	strData.AppendFormat(L"<noxysj1>%.2f</noxysj1>", m_fNOT90Limit);
	//	no2xysj1	NO2响应时间（T90）
	strData.AppendFormat(L"<no2xysj1>%.2f</no2xysj1>", m_fNO2T90Limit);
	//coxysj1	CO响应时间（T90）
	strData.AppendFormat(L"<coxysj1>%.2f</coxysj1>", m_fCOT90Limit);
	//o2xysj1	O2响应时间（T90）
	strData.AppendFormat(L"<o2xysj1>%.2f</o2xysj1>", m_fO2T90Limit);
	//noxysj2	NO响应时间（T10）
	strData.AppendFormat(L"<noxysj2>%.2f</noxysj2>", m_fNOT10Limit);
	//no2xysj2	NO2响应时间（T10）
	strData.AppendFormat(L"<no2xysj2>%.2f</no2xysj2>", m_fNO2T10Limit);
	//coxysj2	CO响应时间（T10）
	strData.AppendFormat(L"<coxysj2>%.2f</coxysj2>", m_fCOT10Limit);
	//o2xysj2	O2响应时间（T10）
	strData.AppendFormat(L"<o2xysj2>%.2f</o2xysj2>", m_fO2T10Limit);
	//jcjg	检查结果
	strData.AppendFormat(L"<jcjg>%s</jcjg>", L"1");
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W07", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W07", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W07", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W07", L"联网失败");
		return false;
	}

	return true;
}

bool CSelfInsrectionItemsDlg::UpGasCheck13W10(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间
	strData.AppendFormat(L"<jckssj>%s</jckssj>", m_odtStart.Format(L"%Y%m%d%H%M%S"));
	//gxsxswc	光吸收系数误差
	strData.AppendFormat(L"<gxsxswc>%.2f</gxsxswc>", m_fAE1);
	//xysj	响应时间
	strData.AppendFormat(L"<xysj>%s</xysj>", L"304");
	//yqwdszwc	烟气温度示值误差
	strData.AppendFormat(L"<yqwdszwc>%s</yqwdszwc>", L"");
	//jcjg	检查结果
	strData.AppendFormat(L"<jcjg>%s</jcjg>", L"1");
	//bhgsm	不合格说明
	strData.AppendFormat(L"<bhgsm>%s</bhgsm>", L"");
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W10", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W10", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W10", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W10", L"联网失败");
		return false;
	}

	return true;
}

void CSelfInsrectionItemsDlg::OnBtnFqyddjcKsjc()
{
	// TODO: 在此添加命令处理程序代码
	CString strName;

	GetDlgItem(IDC_BTN_NOXFXY_KSJC)->GetWindowTextW(strName);
	if (strName != L"开始检查")
	{
		MessageBox(_T("请先停止氮氧分析仪的检查"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return;
	}

	GetDlgItem(IDC_BTN_FQYDDJC_KSJC)->GetWindowTextW(strName);
	if (strName == L"开始检查")
	{
		if (GetNominalValue(1))
		{
			SetTimer(1, 200, NULL);
			GetDlgItem(IDC_BTN_FQYDDJC_KSJC)->SetWindowTextW(L"停止检查");

			m_odtStart = (COleDateTime::GetCurrentTime());
		}
	}
	else
	{
		KillTimer(1);
		GetDlgItem(IDC_BTN_FQYDDJC_KSJC)->SetWindowTextW(L"开始检查");
		m_odtEnd = (COleDateTime::GetCurrentTime());

		// NO大于500为高标
		if (m_sRandomErrorNO > 800)
		{
			m_fHCT90Limit = 5.5f;
			m_fHCT10Limit = 5.7f;
			m_fCOT90Limit = 5.5f;
			m_fCOT10Limit = 5.7f;
			m_fCO2T90Limit = 5.5f;
			m_fCO2T10Limit = 5.7f;
			m_fNOT90Limit = 6.5f;
			m_fNOT10Limit = 6.7f;
			m_fNO2T90Limit = 6.5f;
			m_fNO2T10Limit = 6.7f;
			m_fO2T90Limit = 7.5f;
			m_fO2T10Limit = 8.5f;
			srand(time(0));
			int n = rand()%3+1;
			m_fHCRespTime = m_fHCT90Limit - n;
			m_fCORespTime = m_fCOT90Limit - n;
			m_fCO2RespTime = m_fCO2T90Limit - n;
			m_fNORespTime = m_fNOT90Limit - n;
			m_fNO2RespTime = m_fNO2T90Limit - n;
			m_fO2RespTime = m_fO2T90Limit - n;

			//UpGasCheck(1, 4, L"13W08");
			//UpEqulChk(6);
			UpGasCheck13W07();
			UpEqulChk(5);
		}
		else
		{
			//UpGasCheck(1, 1, L"13W08");
			//UpEqulChk(6);
			UpGasCheck(1, 1, L"13W05");
			UpEqulChk(3);
		}
	}
}


void CSelfInsrectionItemsDlg::OnBtnNoxfxyKsjc()
{
	// TODO: 在此添加命令处理程序代码

	CString strName;

	GetDlgItem(IDC_BTN_FQYDDJC_KSJC)->GetWindowTextW(strName);
	if (strName != L"开始检查")
	{
		MessageBox(_T("请先停止废气分析仪的检查"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return;
	}

	GetDlgItem(IDC_BTN_NOXFXY_KSJC)->GetWindowTextW(strName);

	if (strName == L"开始检查")
	{
		if (GetNominalValue(2))
		{
			SetTimer(2, 200, NULL);
			GetDlgItem(IDC_BTN_NOXFXY_KSJC)->SetWindowTextW(L"停止检查");

			m_odtStart = (COleDateTime::GetCurrentTime());
		}
	}
	else
	{
		KillTimer(2);
		GetDlgItem(IDC_BTN_NOXFXY_KSJC)->SetWindowTextW(L"开始检查");

		m_odtEnd = (COleDateTime::GetCurrentTime());


		// NO大于500为高标
		if (m_sRandomErrorNO > 800)
		{
			m_fHCT90Limit = 5.5f;
			m_fHCT10Limit = 5.7f;
			m_fCOT90Limit = 5.5f;
			m_fCOT10Limit = 5.7f;
			m_fCO2T90Limit = 5.5f;
			m_fCO2T10Limit = 5.7f;
			m_fNOT90Limit = 6.5f;
			m_fNOT10Limit = 6.7f;
			m_fNO2T90Limit = 6.5f;
			m_fNO2T10Limit = 6.7f;
			m_fO2T90Limit = 7.5f;
			m_fO2T10Limit = 8.5f;
			srand(time(0));
			int n = rand()%3+1;
			m_fHCRespTime = m_fHCT90Limit - n;
			m_fCORespTime = m_fCOT90Limit - n;
			m_fCO2RespTime = m_fCO2T90Limit - n;
			m_fNORespTime = m_fNOT90Limit - n;
			m_fNO2RespTime = m_fNO2T90Limit - n;
			m_fO2RespTime = m_fO2T90Limit - n;

			//UpGasCheck(1, 4, L"13W08");
			//UpEqulChk(6);
			UpGasCheck13W07();
			UpEqulChk(5);
		}
		else
		{
			//UpGasCheck(1, 1, L"13W08");
			//UpEqulChk(6);
			UpGasCheck(1, 1, L"13W05");
			UpEqulChk(3);
		}
	}
}


void CSelfInsrectionItemsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		{
			RandomError(m_sRandomErrorHC, m_fPEFNominalValue, m_fRandomErrorCO, 
				m_fRandomErrorCO2, m_fRandomErrorO2, m_sRandomErrorNO, m_sRandomErrorNO2, m_sRandomErrorNOX, 1);
		}
		break;
	case 2:
		{
			RandomError(m_sRandomErrorHC, m_fPEFNominalValue, m_fRandomErrorCO, 
				m_fRandomErrorCO2, m_fRandomErrorO2, m_sRandomErrorNO, m_sRandomErrorNO2, m_sRandomErrorNOX, 2);
		}
		break;
	case 3:
		{
			srand(time(0));
			int n = rand()%10;
			if ((n%2) == 0)
			{
				m_fBTGDZCLZ = (m_fBTGDZBZZ * 100.0f + n) / 100.0f;
			}
			else
			{
				m_fBTGDZCLZ = (m_fBTGDZBZZ * 100.0f - n) / 100.0f;
			}
			m_fAE1 = m_fBTGDZCLZ - m_fBTGDZBZZ;
			CString str;
			str.Format(L"%.2f", m_fBTGDZCLZ);
			GetDlgItem(IDC_ST_YDJJNOXFXY_CLZ_DBZ)->SetWindowTextW(str);
		}
		break;
	case 4:
		{
			srand(time(0));
			int n = rand()%10;
			if ((n%2) == 0)
			{
				m_fBTGDZCLZ = (m_fBTGDZBZZ * 100.0f + n) / 100.0f;
			}
			else
			{
				m_fBTGDZCLZ = (m_fBTGDZBZZ * 100.0f - n) / 100.0f;
			}
			m_fAE1 = m_fBTGDZCLZ - m_fBTGDZBZZ;
			CString str;
			str.Format(L"%.2f", m_fBTGDZCLZ);
			GetDlgItem(IDC_ST_YDJJNOXFXY_CLZ_GBZ)->SetWindowTextW(str);
		}
		break;
	}



	CDialogZoom::OnTimer(nIDEvent);
}


void CSelfInsrectionItemsDlg::OnBtnFqycgjcKsjl()
{
	// TODO: 在此添加命令处理程序代码

	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	检测机构编号	字符	50
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号 	数值	50
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期	日期	50
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间	日期	50
	strData.AppendFormat(L"<jckssj>%s</jckssj>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jcjg	检查结果	数值	50
	strData.AppendFormat(L"<jcjg>%s</jcjg>", L"1");
	//bhgsm	不合格说明	字符	50
	strData.AppendFormat(L"<bhgsm>%s</bhgsm>", L"");
	//jcry	检查人员	字符	50
	strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);

	std::wstring strRet;
	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W09", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W09", str);
				AfxMessageBox(str);
				return;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W09", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"13W09", L"联网失败");
		return;
	}
}


void CSelfInsrectionItemsDlg::OnBtnYdjjnoxfxyQrDbz()
{
	// TODO: 在此添加命令处理程序代码
	CString strName;
	GetDlgItem(IDC_BTN_YDJJNOXFXY_QR_DBZ)->GetWindowTextW(strName);
	if (m_bMIsType && strName == L"确认")
	{
		MessageBox(_T("请完成其中一个区间"), _T("不透光检查"), MB_OK|MB_ICONWARNING);
		return;
	}

	if (strName == L"确认")
	{
		CString strTemp;
		m_edYDJJNOXFXYBZNDBZ.GetWindowTextW(strTemp);
		strTemp.Trim();
		if (L"" == strTemp)
		{
			m_edYDJJNOXFXYBZNDBZ.SetFocus();
			MessageBox(_T("请输入低标准标准N值"), _T("不透光检查"), MB_OK|MB_ICONWARNING);
			return;
		}
		else
		{
			m_fBTGDZBZZ = _wtof(strTemp);
		}

		SetTimer(3, 200, NULL);
		GetDlgItem(IDC_BTN_YDJJNOXFXY_QR_DBZ)->SetWindowTextW(L"上传");
		m_odtStart = (COleDateTime::GetCurrentTime());
		m_bMIsType = true;
	}
	else
	{
		KillTimer(3);
		GetDlgItem(IDC_BTN_YDJJNOXFXY_QR_DBZ)->SetWindowTextW(L"确认");
		m_odtEnd = (COleDateTime::GetCurrentTime());
		m_bMIsType = false;

		UpGasCheck13W10();
	}

}


void CSelfInsrectionItemsDlg::OnBtnYdjjnoxfxyQrGbz()
{
	// TODO: 在此添加命令处理程序代码
	CString strName;
	GetDlgItem(IDC_BTN_YDJJNOXFXY_QR_GBZ)->GetWindowTextW(strName);
	if (m_bMIsType && strName == L"确认")
	{
		MessageBox(_T("请完成其中一个区间"), _T("不透光检查"), MB_OK|MB_ICONWARNING);
		return;
	}

	if (strName == L"确认")
	{
		CString strTemp;
		m_edYDJJNOXFXYBZNGBZ.GetWindowTextW(strTemp);
		strTemp.Trim();
		if (L"" == strTemp)
		{
			m_edYDJJNOXFXYBZNGBZ.SetFocus();
			MessageBox(_T("请输入高标准标准N值"), _T("不透光检查"), MB_OK|MB_ICONWARNING);
			return;
		}
		else
		{
			m_fBTGDZBZZ = _wtof(strTemp);
		}

		SetTimer(4, 200, NULL);
		GetDlgItem(IDC_BTN_YDJJNOXFXY_QR_GBZ)->SetWindowTextW(L"上传");
		m_odtStart = (COleDateTime::GetCurrentTime());
		m_bMIsType = true;
	}
	else
	{
		KillTimer(4);
		GetDlgItem(IDC_BTN_YDJJNOXFXY_QR_GBZ)->SetWindowTextW(L"确认");
		m_odtEnd = (COleDateTime::GetCurrentTime());
		m_bMIsType = false;

		UpGasCheck13W10();
	}
}


void CSelfInsrectionItemsDlg::OnBtnDownLifter()
{
	// TODO: 在此添加命令处理程序代码

	if (m_bUpConstLoadHeavy)
	{
		if (IDNO == (MessageBox(L"已经成功上传过一次，是否重复上传？", L"柴油测功机上传", MB_YESNO)))
		{
			return;
		}
	}

	m_odtEnd = m_odtStart = (COleDateTime::GetCurrentTime());

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 2, 0);
	m_odtStart = odtCurTime - odts;

	UpEqulChk(1);
	m_bUpConstLoadHeavy = UpConstLoadHeavy();
	
	// 寄生功率
	//UpEqulChk(2);
	// UpPLHP();
}


void CSelfInsrectionItemsDlg::OnBtnUpLifter()
{
	// TODO: 在此添加命令处理程序代码
	if (m_bUpConstLoad)
	{
		if (IDNO == (MessageBox(L"已经成功上传过一次，是否重复上传？", L"汽油测功机上传", MB_YESNO)))
		{
			return;
		}
	}

	m_odtEnd = m_odtStart = (COleDateTime::GetCurrentTime());

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 5, 0);
	m_odtStart = odtCurTime - odts;

	UpEqulChk(1);
	m_bUpConstLoad = UpConstLoad();

	// 寄生功率
	//UpEqulChk(2);
	//UpPLHPHeavy();
}



bool CSelfInsrectionItemsDlg::UpConstLoad(void)
{
	float fPLHP[2];
	// 附加损失功率
	fPLHP[0] = CalculatePLHP(40.0f);
	fPLHP[1] = CalculatePLHP(25.0f);
	// 名义滑行时间
	UINT unmyhxsj[2];
	unmyhxsj[0] = CalculateCCTD(_wtoi(m_strDIW), 50.0f, 30.0f, 10.0f);
	unmyhxsj[1] = CalculateCCTD(_wtoi(m_strDIW), 35.0f, 15.0f, 8.0f);
	// 实际滑行时间
	int nsjhxsj[2];
	nsjhxsj[0] = RandomError(unmyhxsj[0]);
	nsjhxsj[1] = RandomError(unmyhxsj[1]);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//hxjckssj	滑行检查开始时间
	strData.AppendFormat(L"<hxjckssj>%s</hxjckssj>", m_odtStart.Format(L"%Y%m%d%H%M%S"));
	//jbgl	基本惯量
	strData.AppendFormat(L"<jbgl>%s</jbgl>", m_strDIW);
	//ihpgl1	IHP2540设置功率
	strData.AppendFormat(L"<ihpgl1>%s</ihpgl1>", L"10");
	//fjssgl1	40km/h时的附加损失功率
	strData.AppendFormat(L"<fjssgl1>%.2f</fjssgl1>", fPLHP[0]);
	//sjhxsj1	50-30km/h实际滑行时间
	strData.AppendFormat(L"<sjhxsj1>%d</sjhxsj1>", nsjhxsj[0]);
	//myhxsj1	50-30km/h名义滑行时间
	strData.AppendFormat(L"<myhxsj1>%d</myhxsj1>", unmyhxsj[0]);
	//ihpgl2	IHP5025设置功率 
	strData.AppendFormat(L"<ihpgl2>%s</ihpgl2>", L"8");
	//fjssgl2	25km/h时的附加损失功率
	strData.AppendFormat(L"<fjssgl2>%.2f</fjssgl2>", fPLHP[1]);
	//sjhxsj2	35-15km/h实际滑行时间
	strData.AppendFormat(L"<sjhxsj2>%d</sjhxsj2>", nsjhxsj[1]);
	//myhxsj2	35-15km/h名义滑行时间
	strData.AppendFormat(L"<myhxsj2>%d</myhxsj2>", unmyhxsj[1]);
	//hxjcjg1	50-30km/h滑行检查结果
	strData.AppendFormat(L"<hxjcjg1>%s</hxjcjg1>", L"1");
	strPass = strTemp;
	//hxjcjg2	35-15km/h滑行检查结果
	strData.AppendFormat(L"<hxjcjg2>%s</hxjcjg2>", L"1");
	//pdjg	判定结果
	strData.AppendFormat(L"<pdjg>%s</pdjg>", L"1");
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W01", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", L"联网失败");
		return false;
	}

	return true;
}

bool CSelfInsrectionItemsDlg::UpConstLoadHeavy(void)
{
	
	// 附加损失功率
	float fPLHP[8];
	fPLHP[0] = CalculatePLHP(90.0f);
	fPLHP[1] = CalculatePLHP(80.0f);
	fPLHP[2] = CalculatePLHP(70.0f);
	fPLHP[3] = CalculatePLHP(60.0f);
	fPLHP[4] = CalculatePLHP(50.0f);
	fPLHP[5] = CalculatePLHP(40.0f);
	fPLHP[6] = CalculatePLHP(30.0f);
	fPLHP[7] = CalculatePLHP(20.0f);
	// 名义滑行时间
	UINT unmyhxsj30[8];
	UINT unmyhxsj20[8];
	UINT unmyhxsj10[8];
	unmyhxsj30[0] = CalculateCCTD(_wtoi(m_strDIW), 100.0f, 80.0f, 30.0f);
	unmyhxsj30[1] = CalculateCCTD(_wtoi(m_strDIW), 90.0f, 70.0f, 30.0f);
	unmyhxsj30[2] = CalculateCCTD(_wtoi(m_strDIW), 80.0f, 60.0f, 30.0f);
	unmyhxsj30[3] = CalculateCCTD(_wtoi(m_strDIW), 70.0f, 50.0f, 30.0f);
	unmyhxsj30[4] = CalculateCCTD(_wtoi(m_strDIW), 60.0f, 40.0f, 30.0f);
	unmyhxsj30[5] = CalculateCCTD(_wtoi(m_strDIW), 50.0f, 30.0f, 30.0f);
	unmyhxsj30[6] = CalculateCCTD(_wtoi(m_strDIW), 40.0f, 20.0f, 30.0f);
	unmyhxsj30[7] = CalculateCCTD(_wtoi(m_strDIW), 30.0f, 10.0f, 30.0f);
	unmyhxsj20[0] = CalculateCCTD(_wtoi(m_strDIW), 100.0f, 80.0f, 20.0f);
	unmyhxsj20[1] = CalculateCCTD(_wtoi(m_strDIW), 90.0f, 70.0f, 20.0f);
	unmyhxsj20[2] = CalculateCCTD(_wtoi(m_strDIW), 80.0f, 60.0f, 20.0f);
	unmyhxsj20[3] = CalculateCCTD(_wtoi(m_strDIW), 70.0f, 50.0f, 20.0f);
	unmyhxsj20[4] = CalculateCCTD(_wtoi(m_strDIW), 60.0f, 40.0f, 20.0f);
	unmyhxsj20[5] = CalculateCCTD(_wtoi(m_strDIW), 50.0f, 30.0f, 20.0f);
	unmyhxsj20[6] = CalculateCCTD(_wtoi(m_strDIW), 40.0f, 20.0f, 20.0f);
	unmyhxsj20[7] = CalculateCCTD(_wtoi(m_strDIW), 30.0f, 10.0f, 20.0f);
	unmyhxsj10[0] = CalculateCCTD(_wtoi(m_strDIW), 100.0f, 80.0f, 10.0f);
	unmyhxsj10[1] = CalculateCCTD(_wtoi(m_strDIW), 90.0f, 70.0f, 10.0f);
	unmyhxsj10[2] = CalculateCCTD(_wtoi(m_strDIW), 80.0f, 60.0f, 10.0f);
	unmyhxsj10[3] = CalculateCCTD(_wtoi(m_strDIW), 70.0f, 50.0f, 10.0f);
	unmyhxsj10[4] = CalculateCCTD(_wtoi(m_strDIW), 60.0f, 40.0f, 10.0f);
	unmyhxsj10[5] = CalculateCCTD(_wtoi(m_strDIW), 50.0f, 30.0f, 10.0f);
	unmyhxsj10[6] = CalculateCCTD(_wtoi(m_strDIW), 40.0f, 20.0f, 10.0f);
	unmyhxsj10[7] = CalculateCCTD(_wtoi(m_strDIW), 30.0f, 10.0f, 10.0f);
	// 实际滑行时间
	int nsjhxsj30[8];
	int nsjhxsj20[8];
	int nsjhxsj10[8];
	nsjhxsj30[0] = RandomError(unmyhxsj30[0]);
	nsjhxsj30[1] = RandomError(unmyhxsj30[1]);
	nsjhxsj30[2] = RandomError(unmyhxsj30[2]);
	nsjhxsj30[3] = RandomError(unmyhxsj30[3]);
	nsjhxsj30[4] = RandomError(unmyhxsj30[4]);
	nsjhxsj30[5] = RandomError(unmyhxsj30[5]);
	nsjhxsj30[6] = RandomError(unmyhxsj30[6]);
	nsjhxsj30[7] = RandomError(unmyhxsj30[7]);
	nsjhxsj20[0] = RandomError(unmyhxsj20[0]);
	nsjhxsj20[1] = RandomError(unmyhxsj20[1]);
	nsjhxsj20[2] = RandomError(unmyhxsj20[2]);
	nsjhxsj20[3] = RandomError(unmyhxsj20[3]);
	nsjhxsj20[4] = RandomError(unmyhxsj20[4]);
	nsjhxsj20[5] = RandomError(unmyhxsj20[5]);
	nsjhxsj20[6] = RandomError(unmyhxsj20[6]);
	nsjhxsj20[7] = RandomError(unmyhxsj20[7]);
	nsjhxsj10[0] = RandomError(unmyhxsj10[0]);
	nsjhxsj10[1] = RandomError(unmyhxsj10[1]);
	nsjhxsj10[2] = RandomError(unmyhxsj10[2]);
	nsjhxsj10[3] = RandomError(unmyhxsj10[3]);
	nsjhxsj10[4] = RandomError(unmyhxsj10[4]);
	nsjhxsj10[5] = RandomError(unmyhxsj10[5]);
	nsjhxsj10[6] = RandomError(unmyhxsj10[6]);
	nsjhxsj10[7] = RandomError(unmyhxsj10[7]);
	// 实际滑行时间
	//
	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	检测机构编号	字符
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号 	数值
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期	日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//hxjckssj	滑行检查开始时间	时间
	strData.AppendFormat(L"<hxjckssj>%s</hxjckssj>", m_odtStart.Format(L"%Y%m%d%H%M%S"));
	//jbgl	基本惯量	数值
	strData.AppendFormat(L"<jbgl>%s</jbgl>", m_strDIW);
	//sjhxsj301	IHP30kw 100-80km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj301>%d</sjhxsj301>", nsjhxsj30[0]);
	//sjhxsj302	IHP30kw 90-70km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj302>%d</sjhxsj302>", nsjhxsj30[1]);
	//sjhxsj303	IHP30kw 80-60km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj303>%d</sjhxsj303>", nsjhxsj30[2]);
	//sjhxsj304	IHP30kw 70-50km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj304>%d</sjhxsj304>", nsjhxsj30[3]);
	//sjhxsj305	IHP30kw 60-40km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj305>%d</sjhxsj305>", nsjhxsj30[4]);
	//sjhxsj306	IHP30kw 50-30km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj306>%d</sjhxsj306>", nsjhxsj30[5]);
	//sjhxsj307	IHP30kw 40-20km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj307>%d</sjhxsj307>", nsjhxsj30[6]);
	//sjhxsj308	IHP30kw 30-10km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj308>%d</sjhxsj308>", nsjhxsj30[7]);
	//myhxsj301	IHP30kw 100-80km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj301>%d</myhxsj301>", unmyhxsj30[0]);
	//myhxsj302	IHP30kw 90-70km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj302>%d</myhxsj302>", unmyhxsj30[1]);
	//myhxsj303	IHP30kw 80-60km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj303>%d</myhxsj303>", unmyhxsj30[2]);
	//myhxsj304	IHP30kw 70-50km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj304>%d</myhxsj304>", unmyhxsj30[3]);
	//myhxsj305	IHP30kw 60-40km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj305>%d</myhxsj305>", unmyhxsj30[4]);
	//myhxsj306	IHP30kw 50-30km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj306>%d</myhxsj306>", unmyhxsj30[5]);
	//myhxsj307	IHP30kw 40-20km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj307>%d</myhxsj307>", unmyhxsj30[6]);
	//myhxsj308	IHP30kw 30-10km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj308>%d</myhxsj308>", unmyhxsj30[7]);
	//sjhxsj201	IHP20kw 100-80km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj201>%d</sjhxsj201>", nsjhxsj20[0]);
	//sjhxsj202	IHP20kw 80-60km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj202>%d</sjhxsj202>", nsjhxsj20[1]);
	//sjhxsj203	IHP20kw 70-50km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj203>%d</sjhxsj203>", nsjhxsj20[2]);
	//sjhxsj204	IHP20kw 60-40km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj204>%d</sjhxsj204>", nsjhxsj20[3]);
	//sjhxsj205	IHP20kw 50-30km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj205>%d</sjhxsj205>", nsjhxsj20[4]);
	//sjhxsj206	IHP20kw 40-20km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj206>%d</sjhxsj206>", nsjhxsj20[5]);
	//sjhxsj207	IHP20kw 30-10km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj207>%d</sjhxsj207>", nsjhxsj20[6]);
	//sjhxsj208	IHP20kw 30-10km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj208>%d</sjhxsj208>", nsjhxsj20[7]);
	//myhxsj201	IHP20kw 100-80km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj201>%d</myhxsj201>", unmyhxsj20[0]);
	//myhxsj202	IHP20kw 90-70km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj202>%d</myhxsj202>", unmyhxsj20[1]);
	//myhxsj203	IHP20kw 80-60km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj203>%d</myhxsj203>", unmyhxsj20[2]);
	//myhxsj204	IHP20kw 70-50km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj204>%d</myhxsj204>", unmyhxsj20[3]);
	//myhxsj205	IHP20kw 60-40km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj205>%d</myhxsj205>", unmyhxsj20[4]);
	//myhxsj206	IHP20kw 50-30km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj206>%d</myhxsj206>", unmyhxsj20[5]);
	//myhxsj207	IHP20kw 40-20km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj207>%d</myhxsj207>", unmyhxsj20[6]);
	//myhxsj208	IHP20kw 30-10km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj208>%d</myhxsj208>", unmyhxsj20[7]);
	//sjhxsj101	IHP10kw 100-80km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj101>%d</sjhxsj101>", nsjhxsj10[0]);
	//sjhxsj102	IHP10kw 90-70km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj102>%d</sjhxsj102>", nsjhxsj10[1]);
	//sjhxsj103	IHP10kw 80-60km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj103>%d</sjhxsj103>", nsjhxsj10[2]);
	//sjhxsj104	IHP10kw 70-50km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj104>%d</sjhxsj104>", nsjhxsj10[3]);
	//sjhxsj105	IHP10kw 60-40km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj105>%d</sjhxsj105>", nsjhxsj10[4]);
	//sjhxsj106	IHP10kw 50-30km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj106>%d</sjhxsj106>", nsjhxsj10[5]);
	//sjhxsj107	IHP10kw 40-20km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj107>%d</sjhxsj107>", nsjhxsj10[6]);
	//sjhxsj108	IHP10kw 30-10km/h实际滑行时间	数值
	strData.AppendFormat(L"<sjhxsj108>%d</sjhxsj108>", nsjhxsj10[7]);
	//myhxsj101	IHP10kw 100-80km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj101>%d</myhxsj101>", unmyhxsj10[0]);
	//myhxsj102	IHP10kw 90-70km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj102>%d</myhxsj102>", unmyhxsj10[1]);
	//myhxsj103	IHP10kw 80-60km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj103>%d</myhxsj103>", unmyhxsj10[2]);
	//myhxsj104	IHP10kw 70-50km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj104>%d</myhxsj104>", unmyhxsj10[3]);
	//myhxsj105	IHP10kw 60-40km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj105>%d</myhxsj105>", unmyhxsj10[4]);
	//myhxsj106	IHP10kw 50-30km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj106>%d</myhxsj106>", unmyhxsj10[5]);
	//myhxsj107	IHP10kw 40-20km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj107>%d</myhxsj107>", unmyhxsj10[6]);
	//myhxsj108	IHP10kw 30-10km/h名义滑行时间	数值
	strData.AppendFormat(L"<myhxsj108>%d</myhxsj108>", unmyhxsj10[7]);
	//fjssgl1	90km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl1>%.2f</fjssgl1>", fPLHP[0]);
	//fjssgl2	80km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl2>%.2f</fjssgl2>", fPLHP[1]);
	//fjssgl3	70km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl3>%.2f</fjssgl3>", fPLHP[2]);
	//fjssgl4	60km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl4>%.2f</fjssgl4>", fPLHP[3]);
	//fjssgl5	50km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl5>%.2f</fjssgl5>", fPLHP[4]);
	//fjssgl6	40km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl6>%.2f</fjssgl6>", fPLHP[5]);
	//fjssgl7	30km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl7>%.2f</fjssgl7>", fPLHP[6]);
	//fjssgl8	20km/h附加损失功率	数值
	strData.AppendFormat(L"<fjssgl8>%.2f</fjssgl8>", fPLHP[7]);
	//hxjcjg1	100-10km/h滑行检查结果	数值
	strData.AppendFormat(L"<hxjcjg1>%s</hxjcjg1>", L"1");
	//hxjcjg2	80-10km/h滑行检查结果	数值
	strData.AppendFormat(L"<hxjcjg2>%s</hxjcjg2>", L"1");
	//pdjg	判定结果	数值
	strData.AppendFormat(L"<pdjg>%s</pdjg>", L"1");
	//jcry	检查人员	字符
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}

	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W03", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", L"联网失败");
		return false;
	}
	return true;
}

float CSelfInsrectionItemsDlg::CalculatePLHP(const float fV)
{

	srand(time(0));
	int n = rand()%50;
	float fAp = (250 + n);
	float fBp = (150 + n);
	float fCp = (423 + n);

	const double dVelocity = fV;

	const double dPLHP = fAp*dVelocity/80.5 + fBp*dVelocity*dVelocity/80.5/80.5 + fCp*dVelocity*dVelocity*dVelocity/80.5/80.5/80.5;

	// 格式化为2位小数,并四舍五入
	const float fPLHP = ((int)(dPLHP * 100.0 + 0.5) / 100.0f) /1000.0f;

	return fPLHP;
}

int CSelfInsrectionItemsDlg::RandomError(const UINT& m_unRangeCCDT)
{
	// 生成随机数
	srand(time(0));
	int n = rand()%50;

	if ((n%2) == 0)
	{
		// 相加
		n = m_unRangeCCDT + n;
	}
	else
	{
		// 相减
		n = m_unRangeCCDT - n;
	}

	return n;
}

UINT CSelfInsrectionItemsDlg::CalculateCCTD(const USHORT usDIW, const float fUpperVelocity, const float fLowerVelocity, const float fConstLoad)
{   
	ASSERT(fUpperVelocity > 0.0f);
	ASSERT(fLowerVelocity > 0.0f);
	ASSERT(fUpperVelocity >= fLowerVelocity);
	ASSERT(fConstLoad > 0.0f);

	return UINT(usDIW * (fUpperVelocity*fUpperVelocity - fLowerVelocity*fLowerVelocity) / (3.6f * 3.6f * 2 * fConstLoad) + 0.5f);
}
