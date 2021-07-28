// InfoTestingPerSonnel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDSCheck.h"
#include "InfoTestingPerSonnel.h"
#include "afxdialogex.h"


// CInfoTestingPerSonnel �Ի���

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


// CInfoTestingPerSonnel ��Ϣ�������


void CInfoTestingPerSonnel::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ����û�������
	CString strjyy;
	GetDlgItem(IDC_ED_JYY)->GetWindowText(strjyy);
	if(strjyy.IsEmpty())
	{
		AfxMessageBox(L"���������Ա");
		GetDlgItem(IDC_ED_JYY)->SetFocus();
		return;
	}

	CString strjyysfzh;
	GetDlgItem(IDC_ED_JYYSFZH)->GetWindowText(strjyysfzh);
	if(strjyysfzh.IsEmpty())
	{
		AfxMessageBox(L"���������Ա���֤��");
		GetDlgItem(IDC_ED_JYYSFZH)->SetFocus();
		return;
	}

	CDialogEx::OnOK();
}


void CInfoTestingPerSonnel::OnEnChangeEdJyy()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
