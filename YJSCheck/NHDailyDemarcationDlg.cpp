// NHDailyDemarcationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YJSCheck.h"
#include "NHDailyDemarcationDlg.h"
#include "afxdialogex.h"


// CNHDailyDemarcationDlg 对话框

IMPLEMENT_DYNAMIC(CNHDailyDemarcationDlg, CDialogZoom)

CNHDailyDemarcationDlg::CNHDailyDemarcationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHDailyDemarcationDlg::IDD, pParent)
{
	// 设备校准检查信息
	ZeroMemory(&m_sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	// 检测线信息
	ZeroMemory(&m_sLineInfo, sizeof(LINEINFO));

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 25 / 1024;
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

CNHDailyDemarcationDlg::~CNHDailyDemarcationDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CNHDailyDemarcationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHDailyDemarcationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CNHDailyDemarcationDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CNHDailyDemarcationDlg 消息处理程序


void CNHDailyDemarcationDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


BOOL CNHDailyDemarcationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_vtID.push_back(IDC_STATIC_1);
	m_vtID.push_back(IDC_STATIC_2);
	m_vtID.push_back(IDC_STATIC_3);
	m_vtID.push_back(IDC_STATIC_4);
	m_vtID.push_back(IDC_STATIC_5);
	m_vtID.push_back(IDC_STATIC_6);
	m_vtID.push_back(IDC_STATIC_7);
	m_vtID.push_back(IDC_STATIC_8);

	/*
	  初始化控件
	*/
	InitCtrls();

	/*
	  最大化显示窗口
	*/
	ShowWindow(SW_MAXIMIZE);

	LoadSubDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHDailyDemarcationDlg::InitCtrls()
{
	/*
	  初始化字体
	*/
	SetDlgFont();

	m_lbTitle.SetTitle(L"日常标定项目");
}

void CNHDailyDemarcationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHDailyDemarcationDlg::LoadSubDlg(void)
{
	int  nIndex(0);
	// 获取系统类型
	NHCLIENT SiniNHClient;
	GetIniNHClient(&SiniNHClient);
	
	LINEINFO sLineInfo;
	ZeroMemory(&sLineInfo, sizeof(LINEINFO));
	GetIniLineInfo(&sLineInfo);

	CRect rt;
	if (L'1' == SiniNHClient.wchSystemType[0]  /* ASM */
	|| L'1' == SiniNHClient.wchSystemType[1]   /* VMAS */
	|| L'1' == SiniNHClient.wchSystemType[2])   /* LUGDOWN */
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);	
		m_DynDlg.Create(CDynDlg::IDD, this);
		m_DynDlg.MoveWindow(rt);
		m_DynDlg.ShowWindow(SW_SHOW);
	}

	if (L'1' == SiniNHClient.wchSystemType[0]  /* ASM */
	|| L'1' == SiniNHClient.wchSystemType[1]   /* VMAS */
	|| L'1' == SiniNHClient.wchSystemType[3])  /* DIS */
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_AnaDlg.Create(CAnaDlg::IDD, this);
		m_AnaDlg.MoveWindow(rt);
		m_AnaDlg.ShowWindow(SW_SHOW);
	}
	if (L'1' == SiniNHClient.wchSystemType[1]) /* VMAS */
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_FloDlg.Create(CFloDlg::IDD, this);
		m_FloDlg.MoveWindow(rt);
		m_FloDlg.ShowWindow(SW_SHOW);
	}
	if (L'1' == SiniNHClient.wchSystemType[2]  /* LUGDOWN */
		&& L'1' == sLineInfo.wchHasNOxAnalyzer[0]
		)
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_NOxDlg.Create(CNOxDlg::IDD, this);
		m_NOxDlg.MoveWindow(rt);
		m_NOxDlg.ShowWindow(SW_SHOW);
	}
	if (L'1' == SiniNHClient.wchSystemType[2]  /* LUGDOWN */
	|| L'1' == SiniNHClient.wchSystemType[4])  /* FSUNHT */
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_OpaDlg.Create(COpaDlg::IDD, this);
		m_OpaDlg.MoveWindow(rt);
		m_OpaDlg.ShowWindow(SW_SHOW);
	}
	if (L'1' == SiniNHClient.wchSystemType[5]) /* FSUYD */
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_SmoDlg.Create(CSmoDlg::IDD, this);
		m_SmoDlg.MoveWindow(rt);
		m_SmoDlg.ShowWindow(SW_SHOW);
	}

	LINEINFO SLineInfo;
	ZeroMemory(&SLineInfo, sizeof(LINEINFO));
	GetIniLineInfo(&SLineInfo);
	if (L'1' == SLineInfo.wchHasIndependentEnvParamMeasurer[0])
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}

		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_ParaDlg.Create(CParaDlg::IDD, this);
		m_ParaDlg.MoveWindow(rt);
		m_ParaDlg.ShowWindow(SW_SHOW);
	}

	if (L'1' == SLineInfo.wchHasOBDAdapter[0])
	{
		if (nIndex >= m_vtID.size())
		{
			// 界面无位置加载
			return;
		}
		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_ObdDlg.Create(CObdDlg::IDD, this);
		m_ObdDlg.MoveWindow(rt);
		m_ObdDlg.ShowWindow(SW_SHOW);
	}
}
