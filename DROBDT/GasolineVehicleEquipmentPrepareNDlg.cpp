// GasolineVehicleEquipmentPrepareNDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DROBDT.h"
#include "GasolineVehicleEquipmentPrepareNDlg.h"
#include "afxdialogex.h"


// CGasolineVehicleEquipmentPrepareNDlg 对话框
IMPLEMENT_DYNAMIC(CGasolineVehicleEquipmentPrepareNDlg, CDialogZoom)

CGasolineVehicleEquipmentPrepareNDlg::CGasolineVehicleEquipmentPrepareNDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CGasolineVehicleEquipmentPrepareNDlg::IDD, pParent)
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

CGasolineVehicleEquipmentPrepareNDlg::~CGasolineVehicleEquipmentPrepareNDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CGasolineVehicleEquipmentPrepareNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_TIP, m_lbInfo);
	DDX_Control(pDX, IDC_CHK_ZERO, m_chkZero);
	DDX_Control(pDX, IDC_CHK_EC, m_chkEC);
	DDX_Control(pDX, IDC_CHK_RC, m_chkRC);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_CHK_FLOWMETER_CHECK_O2, m_chkFlowmeterCheckO2);
	DDX_Control(pDX, IDC_STATIC_PROBETIPS, m_lbProbeTips);
}


BEGIN_MESSAGE_MAP(CGasolineVehicleEquipmentPrepareNDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RETRY, &CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnRetry)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ANA_ZERO, &CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnAnaZero)
	ON_BN_CLICKED(IDC_BTN_ANA_CHK_EC, &CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnAnaChkEc)
	ON_BN_CLICKED(IDC_BTN_ANA_CHK_RC, &CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnAnaChkRc)
	ON_BN_CLICKED(IDC_BTN_FLO_CHK_O2, &CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnFloChkO2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGasolineVehicleEquipmentPrepareNDlg 消息处理程序

BOOL CGasolineVehicleEquipmentPrepareNDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化控件
	InitCtrls();

	SetTimer(1, 200, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGasolineVehicleEquipmentPrepareNDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);

			m_chkZero.SetCheck(TRUE);
			m_chkEC.SetCheck(TRUE);
			m_chkRC.SetCheck(TRUE);
			m_chkFlowmeterCheckO2.SetCheck(TRUE);
			// 提示信息
			m_lbInfo.SetText(L"设备准备完成");

			GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
		}
		break;
	case 2: //调零
		{
			KillTimer(2);
			m_chkZero.SetCheck(TRUE);
			if (!m_bRet)
			{
				// 提示信息
				m_lbInfo.SetText(L"设备准备完成");
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
			}
			else
			{
				m_lbInfo.SetText(L"正在环境空气检查...");
	m_lbProbeTips.FlashText(3);
				SetTimer(3, 1000, NULL);
			}
		}
		break;
	case 3: //正在环境空气检查
		{
			KillTimer(3);
			m_chkEC.SetCheck(TRUE);
			if (!m_bRet)
			{
				// 提示信息
				m_lbInfo.SetText(L"设备准备完成");
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
			}
			else
			{
				m_lbInfo.SetText(L"正在气体残留检查...");
	m_lbProbeTips.FlashText(3);
				SetTimer(4, 1000, NULL);
			}
		}
		break;
	case 4: //正在气体残留检查
		{
			KillTimer(4);
			m_chkRC.SetCheck(TRUE);

			if (!m_bFlo && m_bRet)
			{
				m_bRet = false;
			}

			if (!m_bRet )
			{
				// 提示信息
				m_lbInfo.SetText(L"设备准备完成");
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
			}
			else
			{
				m_lbInfo.SetText(L"正在氧量程检查...");
				m_lbProbeTips.FlashText(3);
				SetTimer(5, 1000, NULL);
			}
		}
		break;
	case 5: //氧量程
		{
			KillTimer(5);
			m_chkFlowmeterCheckO2.SetCheck(TRUE);
			// 提示信息
			m_lbInfo.SetText(L"设备准备完成");
			GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
		}
	default:
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CGasolineVehicleEquipmentPrepareNDlg::InitCtrls()
{
	// 设置对话框字体
	SetDlgFont();

	// 基本设置
	m_lbHead.SetTitle(L"汽油车设备准备");
	
	m_lbInfo.SetText(L"正在通讯..");
	m_lbInfo.SetParameter(L"黑体", 36, RGB(0,0,0), RGB(237,237,237));

	m_lbProbeTips.SetFontName(L"黑体");
	m_lbProbeTips.SetTextColor(RGB(255,0,0));
	m_lbProbeTips.SetFontSize(30);

	GetIniNHClient(&m_sNHClient);

	if (L'1' == m_sNHClient.wchSystemType[1])
	{
		m_chkFlowmeterCheckO2.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_FLO_CHK_O2)->ShowWindow(SW_SHOW);

		m_bFlo = true;
	}
	else
	{
		m_chkFlowmeterCheckO2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_FLO_CHK_O2)->ShowWindow(SW_HIDE);

		m_bFlo = false;
	}

	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
}

void CGasolineVehicleEquipmentPrepareNDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CGasolineVehicleEquipmentPrepareNDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();
}

void CGasolineVehicleEquipmentPrepareNDlg::OnClose()
{
	CDialogZoom::OnClose();
}

void CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnRetry()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkZero.SetCheck(FALSE);
	m_chkEC.SetCheck(FALSE);
	m_chkRC.SetCheck(FALSE);
	m_chkFlowmeterCheckO2.SetCheck(FALSE);
	m_lbProbeTips.FlashText(3);

	m_bRet = true;

	m_lbInfo.SetText(L"正在调零...");
	m_lbProbeTips.FlashText(3);
	SetTimer(2, 5000, NULL);
}

void CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnAnaZero()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkZero.SetCheck(FALSE);

	// 提示信息
	m_lbInfo.SetText(L"正在调零...");

	// 提示信息
	m_lbInfo.SetText(L"正在调零...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(2, 5000, NULL);
}

void CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnAnaChkEc()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkEC.SetCheck(FALSE);
	// 提示信息
	m_lbInfo.SetText(L"正在环境空气检查...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(3, 1000, NULL);
}

void CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnAnaChkRc()
{
	// 清除完成标志
	m_chkRC.SetCheck(FALSE);

	// 禁用所有按钮
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 提示信息
	m_lbInfo.SetText(L"正在气体残留检查...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(4, 1000, NULL);
}

void CGasolineVehicleEquipmentPrepareNDlg::OnBnClickedBtnFloChkO2()
{
	// 禁用所有按钮
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// 清除完成标志
	m_chkFlowmeterCheckO2.SetCheck(FALSE);

	// 提示信息
	m_lbInfo.SetText(L"正在氧量程检查...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(5, 5000, NULL);
}
