// InfoTestingPerSonnel.cpp : 实现文件
//

#include "stdafx.h"
#include "CDSCheck.h"
#include "InfoTestingPerSonnel.h"
#include "afxdialogex.h"


// CInfoTestingPerSonnel 对话框

IMPLEMENT_DYNAMIC(CInfoTestingPerSonnel, CDialogEx)

CInfoTestingPerSonnel::CInfoTestingPerSonnel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfoTestingPerSonnel::IDD, pParent)
{

}

CInfoTestingPerSonnel::~CInfoTestingPerSonnel()
{
}

void CInfoTestingPerSonnel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoTestingPerSonnel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInfoTestingPerSonnel::OnBnClickedOk)
	ON_EN_CHANGE(IDC_ED_JYY, &CInfoTestingPerSonnel::OnEnChangeEdJyy)
END_MESSAGE_MAP()


// CInfoTestingPerSonnel 消息处理程序


void CInfoTestingPerSonnel::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查用户名密码
	CString strjyy;
	GetDlgItem(IDC_ED_JYY)->GetWindowText(strjyy);
	if(strjyy.IsEmpty())
	{
		AfxMessageBox(L"请输入检验员");
		GetDlgItem(IDC_ED_JYY)->SetFocus();
		return;
	}

	CString strjyysfzh;
	GetDlgItem(IDC_ED_JYYSFZH)->GetWindowText(strjyysfzh);
	if(strjyysfzh.IsEmpty())
	{
		AfxMessageBox(L"请输入检验员身份证号");
		GetDlgItem(IDC_ED_JYYSFZH)->SetFocus();
		return;
	}

	CDialogEx::OnOK();
}


void CInfoTestingPerSonnel::OnEnChangeEdJyy()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strjyy;
	GetDlgItem(IDC_ED_JYY)->GetWindowText(strjyy);
	if(!strjyy.IsEmpty())
	{
		CString strSql;
		strSql.Format(L"select * from UserInfo where Name = '%s'",strjyy);
		USERINFO SUserInfo;

		int nLogin = GetDboUserInfo(strSql, &SUserInfo);

		if (1 == nLogin)
		{
			theApp.m_sUserInfo = SUserInfo;
			GetDlgItem(IDC_ED_JYYSFZH)->SetWindowText(SUserInfo.wchPassword);

		}
	}

}
