// GasolineVehicleEquipmentPrepareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YJOBDT.h"
#include "GasolineVehicleEquipmentPrepareDlg.h"
#include "afxdialogex.h"


// CGasolineVehicleEquipmentPrepareDlg �Ի���

IMPLEMENT_DYNAMIC(CGasolineVehicleEquipmentPrepareDlg, CDialogZoom)

	CGasolineVehicleEquipmentPrepareDlg::CGasolineVehicleEquipmentPrepareDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CGasolineVehicleEquipmentPrepareDlg::IDD, pParent)
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

CGasolineVehicleEquipmentPrepareDlg::~CGasolineVehicleEquipmentPrepareDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CGasolineVehicleEquipmentPrepareDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CGasolineVehicleEquipmentPrepareDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RETRY, &CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnRetry)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ANA_ZERO, &CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnAnaZero)
	ON_BN_CLICKED(IDC_BTN_ANA_CHK_EC, &CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnAnaChkEc)
	ON_BN_CLICKED(IDC_BTN_ANA_CHK_RC, &CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnAnaChkRc)
	ON_BN_CLICKED(IDC_BTN_FLO_CHK_O2, &CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnFloChkO2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGasolineVehicleEquipmentPrepareDlg ��Ϣ�������

BOOL CGasolineVehicleEquipmentPrepareDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ���ؼ�
	InitCtrls();

	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGasolineVehicleEquipmentPrepareDlg::OnTimer(UINT_PTR nIDEvent)
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
			// ��ʾ��Ϣ
			m_lbInfo.SetText(L"�豸׼�����");

			GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
		}
		break;
	case 2: //����
		{
			KillTimer(2);
			m_chkZero.SetCheck(TRUE);
			if (!m_bRet)
			{
				// ��ʾ��Ϣ
				m_lbInfo.SetText(L"�豸׼�����");
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
			}
			else
			{
				SetTimer(3, 1000, NULL);
			}
		}
		break;
	case 3: //���ڻ����������
		{
			KillTimer(3);
			m_chkEC.SetCheck(TRUE);
			if (!m_bRet)
			{
				// ��ʾ��Ϣ
				m_lbInfo.SetText(L"�豸׼�����");
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
			}
			else
			{
				SetTimer(4, 1000, NULL);
			}
		}
		break;
	case 4: //��������������
		{
			KillTimer(4);
			m_chkRC.SetCheck(TRUE);

			if (!m_bFlo && m_bRet)
			{
				m_bRet = false;
			}

			if (!m_bRet )
			{
				// ��ʾ��Ϣ
				m_lbInfo.SetText(L"�豸׼�����");
				GetDlgItem(IDC_BTN_RETRY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(TRUE);
			}
			else
			{
				SetTimer(5, 1000, NULL);
			}
		}
		break;
	case 5: //������
		{
			KillTimer(5);
			m_chkFlowmeterCheckO2.SetCheck(TRUE);
			// ��ʾ��Ϣ
			m_lbInfo.SetText(L"�豸׼�����");
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

void CGasolineVehicleEquipmentPrepareDlg::InitCtrls()
{
	// ���öԻ�������
	SetDlgFont();

	// ��������
	m_lbHead.SetTitle(L"���ͳ��豸׼��");

	m_lbInfo.SetText(L"����ͨѶ..");
	m_lbInfo.SetParameter(L"����", 36, RGB(0,0,0), RGB(237,237,237));

	m_lbProbeTips.SetFontName(L"����");
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

void CGasolineVehicleEquipmentPrepareDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CGasolineVehicleEquipmentPrepareDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();
}

void CGasolineVehicleEquipmentPrepareDlg::OnClose()
{
	CDialogZoom::OnClose();
}

void CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnRetry()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkZero.SetCheck(FALSE);
	m_chkEC.SetCheck(FALSE);
	m_chkRC.SetCheck(FALSE);
	m_chkFlowmeterCheckO2.SetCheck(FALSE);
	m_lbProbeTips.FlashText(3);

	m_bRet = true;

	SetTimer(2, 5000, NULL);
}

void CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnAnaZero()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkZero.SetCheck(FALSE);

	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"���ڵ���...");

	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"���ڵ���...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(2, 5000, NULL);
}

void CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnAnaChkEc()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkEC.SetCheck(FALSE);
	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"���ڻ����������...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(3, 1000, NULL);
}

void CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnAnaChkRc()
{
	// �����ɱ�־
	m_chkRC.SetCheck(FALSE);

	// �������а�ť
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"��������������...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(4, 1000, NULL);
}

void CGasolineVehicleEquipmentPrepareDlg::OnBnClickedBtnFloChkO2()
{
	// �������а�ť
	GetDlgItem(IDC_BTN_ANA_ZERO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_EC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANA_CHK_RC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLO_CHK_O2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RETRY)->EnableWindow(FALSE);

	// �����ɱ�־
	m_chkFlowmeterCheckO2.SetCheck(FALSE);

	// ��ʾ��Ϣ
	m_lbInfo.SetText(L"���������̼��...");
	m_lbProbeTips.FlashText(3);

	m_bRet = false;

	SetTimer(4, 5000, NULL);
}