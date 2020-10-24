// ParaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ParaDlg.h"

// CParaDlg �Ի���

IMPLEMENT_DYNAMIC(CParaDlg, CDialogZoom)

CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParaDlg::IDD, pParent)
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

CParaDlg::~CParaDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CParaDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ENV_PARAMS_CAL, &CParaDlg::OnBnClickedButtonEnvParamsCal)
END_MESSAGE_MAP()


// CParaDlg ��Ϣ�������
BOOL CParaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CParaDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"����������");
}

void CParaDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}


void CParaDlg::OnBnClickedButtonEnvParamsCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	

	LoadParameterEnvParamsCalibrationDlg();
}