// NHDailyDemarcationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YJSCheck.h"
#include "NHDailyDemarcationDlg.h"
#include "afxdialogex.h"


// CNHDailyDemarcationDlg �Ի���

IMPLEMENT_DYNAMIC(CNHDailyDemarcationDlg, CDialogZoom)

CNHDailyDemarcationDlg::CNHDailyDemarcationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHDailyDemarcationDlg::IDD, pParent)
{
	// �豸У׼�����Ϣ
	ZeroMemory(&m_sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	// �������Ϣ
	ZeroMemory(&m_sLineInfo, sizeof(LINEINFO));

	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		_T("����"));
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


// CNHDailyDemarcationDlg ��Ϣ�������


void CNHDailyDemarcationDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


BOOL CNHDailyDemarcationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_vtID.push_back(IDC_STATIC_1);
	m_vtID.push_back(IDC_STATIC_2);
	m_vtID.push_back(IDC_STATIC_3);
	m_vtID.push_back(IDC_STATIC_4);
	m_vtID.push_back(IDC_STATIC_5);
	m_vtID.push_back(IDC_STATIC_6);
	m_vtID.push_back(IDC_STATIC_7);
	m_vtID.push_back(IDC_STATIC_8);

	/*
	  ��ʼ���ؼ�
	*/
	InitCtrls();

	/*
	  �����ʾ����
	*/
	ShowWindow(SW_MAXIMIZE);

	LoadSubDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHDailyDemarcationDlg::InitCtrls()
{
	/*
	  ��ʼ������
	*/
	SetDlgFont();

	m_lbTitle.SetTitle(L"�ճ��궨��Ŀ");
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
	// ��ȡϵͳ����
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
			// ������λ�ü���
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
			// ������λ�ü���
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
			// ������λ�ü���
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
			// ������λ�ü���
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
			// ������λ�ü���
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
			// ������λ�ü���
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
			// ������λ�ü���
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
			// ������λ�ü���
			return;
		}
		GetDlgItem(m_vtID[nIndex++])->GetWindowRect(rt);
		m_ObdDlg.Create(CObdDlg::IDD, this);
		m_ObdDlg.MoveWindow(rt);
		m_ObdDlg.ShowWindow(SW_SHOW);
	}
}
