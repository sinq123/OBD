// DieselVehicleEquipmentPrepareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DROBDT.h"
#include "DieselVehicleEquipmentPrepareDlg.h"
#include "afxdialogex.h"


// CDieselVehicleEquipmentPrepareDlg �Ի���

IMPLEMENT_DYNAMIC(CDieselVehicleEquipmentPrepareDlg, CDialogZoom)

CDieselVehicleEquipmentPrepareDlg::CDieselVehicleEquipmentPrepareDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDieselVehicleEquipmentPrepareDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
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
		L"����");
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


// CDieselVehicleEquipmentPrepareDlg ��Ϣ�������


void CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnRetry()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkAdjust.SetCheck(FALSE);
	m_chkZero.SetCheck(FALSE);

	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"����У׼...");
	m_lbProbeTips.FlashText(3);

	m_bOpaCal = false;
	// ���У׼�Ƿ����
	SetTimer(2,1000,NULL);
}

void CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnOpaCal()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkAdjust.SetCheck(FALSE);

	m_bOpaCal = true;
	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"����У׼...");
	m_lbProbeTips.FlashText(3);

	// ���У׼�Ƿ����
	SetTimer(2,1000,NULL);
}

void CDieselVehicleEquipmentPrepareDlg::OnBnClickedBtnNOxZero()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkZero.SetCheck(FALSE);

	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"����У׼...");
	m_lbProbeTips.FlashText(3);

	// ���У׼�Ƿ����
	SetTimer(3,5000,NULL);
}

void CDieselVehicleEquipmentPrepareDlg::OnTimer(UINT_PTR nIDEvent)
{

	switch(nIDEvent)
	{
	case 1: // Ĭ���豸׼����
		{
			KillTimer(1);
			// ��ʾ��Ϣ
			m_lbInfo.SetText(L"�豸׼�����");

			m_chkAdjust.SetCheck(TRUE);
			m_chkZero.SetCheck(TRUE);

			// �����а�ť
			GetDlgItem(IDC_BTN_OPA_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_NOX_ZERO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
		}
		break;
	case 2: // ����,��͸��У׼
		{
			KillTimer(2);

			m_chkAdjust.SetCheck(TRUE);
			// �����а�ť
			if (m_bOpaCal)
			{
				// ��ʾ��Ϣ
				m_lbInfo.SetText(L"�豸׼�����");
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
	case 3: // ����,��͸��У׼
		{
			KillTimer(3);

			// ��ʾ��Ϣ
			m_lbInfo.SetText(L"�豸׼�����");

			m_chkZero.SetCheck(TRUE);
			// �����а�ť
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	GetIniNHClient(&m_sNHClient);
	GetIniLineInfo(&m_sLineInfo);

	// ��ʼ���ؼ�
	InitCtrls();

	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDieselVehicleEquipmentPrepareDlg::InitCtrls()
{
	SetDlgFont();

	m_lbHead.SetTitle(L"���ͳ��豸׼��");

	m_lbInfo.SetText(L"����ͨѶ..");
	m_lbInfo.SetParameter(L"����", 36, RGB(0,0,0), RGB(237,237,237));

	m_lbProbeTips.SetFontName(L"����");
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