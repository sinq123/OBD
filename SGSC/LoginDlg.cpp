// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SGSC.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	//m_nDlgFontSize = nSM_CYSCREEN * 15 / 1024;
	m_nDlgFontSize = 22;
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
		L"΢���ź�");
}

CLoginDlg::~CLoginDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_USER_NAME, m_stUserName);
	DDX_Control(pDX, IDC_ST_PASSWORD, m_stPassword);
	DDX_Control(pDX, IDC_ED_USER_NAME, m_edUserName);
	DDX_Control(pDX, IDC_ED_PASSWORD, m_edPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������


void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strUserName, strPassword;
	m_edUserName.GetWindowTextW(strUserName);
	m_edPassword.GetWindowTextW(strPassword);

	if (L"" == strUserName)
	{
		MessageBox(L"������[�û���]��", L"�û���¼", MB_OK|MB_ICONERROR);
		m_edUserName.SetFocus();
		return;
	}

	if (L"" == strPassword)
	{
		MessageBox(L"������[����]��", L"�û���¼", MB_OK|MB_ICONERROR);
		m_edPassword.SetFocus();
		return;
	}

	theApp.m_strName = strUserName;
	theApp.m_strPassword = strPassword;



	CDialogEx::OnOK();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ���ؼ�
	InitCtrls();

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLoginDlg::InitCtrls(void)
{
	SetDlgFont();

	m_stUserName.SetBkTransparent();
	m_stPassword.SetBkTransparent();

#if defined(_DEBUG)
	m_edUserName.SetWindowText(L"admin");
	m_edPassword.SetWindowText(L"123");
#else
	m_edUserName.SetFocus();
#endif

	CString strTitle(L"");
	strTitle.Format(L"%s", L"SGSC��¼");
	SetWindowText(strTitle);
}

void CLoginDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}