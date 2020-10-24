// ObdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ObdDlg.h"

// CObdDlg 对话框

IMPLEMENT_DYNAMIC(CObdDlg, CDialogZoom)

CObdDlg::CObdDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CObdDlg::IDD, pParent)
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


// CObdDlg 消息处理程序
BOOL CObdDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CObdDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"车载诊断系统");
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
