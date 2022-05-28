// DieselVehicleEquipmentPrepareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DROBDT.h"
#include "DieselVehicleEquipmentPrepareDlg.h"
#include "afxdialogex.h"


// CDieselVehicleEquipmentPrepareDlg 对话框

IMPLEMENT_DYNAMIC(CDieselVehicleEquipmentPrepareDlg, CDialogZoom)

CDieselVehicleEquipmentPrepareDlg::CDieselVehicleEquipmentPrepareDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDieselVehicleEquipmentPrepareDlg::IDD, pParent)
{
	// 针对1280*1024分辨率进行开发
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		L"宋体");
}

CDieselVehicleEquipmentPrepareDlg::~CDieselVehicleEquipmentPrepareDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CDieselVehicleEquipmentPrepareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_CHK_ADJUST, m_chkAdjust);
	DDX_Control(pDX, IDC_CHK_ZERO, m_chkZero);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_STATIC_PROBETIPS, m_lbProbeTips);
}


BEGIN_MESSAGE_MAP(CDieselVehicleEquipmentPrepareDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_RETRY, &CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnRetry)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_OPA_CAL, &CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnOpaCal)
	ON_BN_CLICKED(IDC_BTN_NOX_ZERO, &CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnNOxZero)
END_MESSAGE_MAP()


// CDieselVehicleEquipmentPrepareDlg 消息处理程序


void CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnRetry()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkAdjust.SetCheck(FALSE);
	m_chkZero.SetCheck(FALSE);

	// 提示信息
	m_lbInfo.SetText(L"正在校准...");
	m_lbProbeTips.FlashText(3);

	m_bOpaCal = false;
	// 检查校准是否完成
	SetTimer(2,1000,NULL);
}

void CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnOpaCal()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkAdjust.SetCheck(FALSE);

	m_bOpaCal = true;
	// 提示信息
	m_lbInfo.SetText(L"正在校准...");
	m_lbProbeTips.FlashText(3);

	// 检查校准是否完成
	SetTimer(2,1000,NULL);
}

void CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnNOxZero()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkZero.SetCheck(FALSE);

	// 提示信息
	m_lbInfo.SetText(L"正在校准...");
	m_lbProbeTips.FlashText(3);

	// 检查校准是否完成
	SetTimer(3,5000,NULL);
}

void CDieselVehicleEquipmentPrepareDlg::OnTimer(UINT_PTR nIDEvent)
{

	switch(nIDEvent)
	{
	case 1: // 默认设备准备好
		{
			KillTimer(1);
			// 提示信息
			m_lbInfo.SetText(L"设备准备完成");

			m_chkAdjust.SetCheck(TRUE);
			m_chkZero.SetCheck(TRUE);

			// 打开所有按钮
			GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
		}
		break;
	case 2: // 重试,不透光校准
		{
			KillTimer(2);

			m_chkAdjust.SetCheck(TRUE);
			// 打开所有按钮
			if (m_bOpaCal)
			{
				// 提示信息
				m_lbInfo.SetText(L"设备准备完成");
				GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
			}
			else
			{
				m_bOpaCal = true;
				SetTimer(3,5000,NULL);
			}
		}
		break;
	case 3: // 重试,不透光校准
		{
			KillTimer(3);

			// 提示信息
			m_lbInfo.SetText(L"设备准备完成");

			m_chkZero.SetCheck(TRUE);
			// 打开所有按钮
			GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
		}
		break;
	default:
		break;
	}
	CDialogZoom::OnTimer(nIDEvent);
}

BOOL CDieselVehicleEquipmentPrepareDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetIniNHClient(&m_sNHClient);
	GetIniLineInfo(&m_sLineInfo);

	// 初始化控件
	InitCtrls();

	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDieselVehicleEquipmentPrepareDlg::InitCtrls()
{
	SetDlgFont();

	m_lbHead.SetTitle(L"柴油车设备准备");

	m_lbInfo.SetText(L"正在通讯..");
	m_lbInfo.SetParameter(L"黑体", 36, RGB(0,0,0), RGB(237,237,237));

	m_lbProbeTips.SetFontName(L"黑体");
	m_lbProbeTips.SetTextColor(RGB(255,0,0));
	m_lbProbeTips.SetFontSize(30);

	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);
	

	if (L'1' == m_sNHClient.wchSystemType[2]
		&& L'1' == m_sLineInfo.wchHasNOxAnalyzer[0]
		)
	{
		m_chkZero.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_NOX_ZERO)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_chkZero.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_NOX_ZERO)->ShowWindow(SW_HIDE);
	}

}

void CDieselVehicleEquipmentPrepareDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}