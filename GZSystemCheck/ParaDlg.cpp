// ParaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParaDlg.h"

// CParaDlg 对话框

IMPLEMENT_DYNAMIC(CParaDlg, CDialogZoom)

CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParaDlg::IDD, pParent)
{
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


// CParaDlg 消息处理程序
BOOL CParaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CParaDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"参数测量仪");
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
	// TODO: 在此添加控件通知处理程序代码	

	LoadParameterEnvParamsCalibrationDlg();
}