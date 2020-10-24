// ObdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ObdDlg.h"

// CObdDlg �Ի���

IMPLEMENT_DYNAMIC(CObdDlg, CDialogZoom)

CObdDlg::CObdDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CObdDlg::IDD, pParent)
{
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

CObdDlg::~CObdDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CObdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CObdDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_OBD_OPERATION, &CObdDlg::OnBnClickedButtonObdOperation)
END_MESSAGE_MAP()


// CObdDlg ��Ϣ�������
BOOL CObdDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CObdDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"�������ϵͳ");
}

void CObdDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CObdDlg::OnBnClickedButtonObdOperation()
{
	LoadOBDOperationDlg();
}
