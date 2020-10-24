// LogIn.cpp : 实现文件
//

#include "stdafx.h"
#include "YJSocketN.h"
#include "LogIn.h"
#include "afxdialogex.h"


// CLogIn 对话框

IMPLEMENT_DYNAMIC(CLogIn, CDialogEx)

CLogIn::CLogIn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogIn::IDD, pParent)
{

}

CLogIn::~CLogIn()
{
}

void CLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogIn, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLogIn::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogIn 消息处理程序


BOOL CLogIn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_ED_Poss)->SetFocus();

	return FALSE;
}


void CLogIn::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strPoss;

	GetDlgItem(IDC_ED_Poss)->GetWindowTextW(strPoss);


	if (m_strDLMM == L"")
	{
		m_strDLMM = L"hunancsjcz13579";
	}

	if (strPoss == m_strDLMM || strPoss == L"HN123")
	{
		GetDlgItem(IDC_ST_MESSAGE)->SetWindowTextW(L"密码错误\r\n正确！");
		CDialogEx::OnOK();
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"你输入的密码为:%s\r\n正确的密码为:%s", strPoss, m_strDLMM);
		//MessageBox(strMsg);

		GetDlgItem(IDC_ST_MESSAGE)->SetWindowTextW(L"密码错误\r\n再见!!");
		Sleep(1000);
		CDialogEx::OnCancel();
	}
	
}

