// LogIn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ACInter.h"
#include "LogIn.h"
#include "afxdialogex.h"


// CLogIn �Ի���

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


// CLogIn ��Ϣ��������


void CLogIn::OnBnClickedOk()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������

	CString strPoss;

	GetDlgItem(IDC_ED_Poss)->GetWindowTextW(strPoss);


	if (m_strDLMM == L"")
	{
		m_strDLMM = L"hunancsjcz13579";
	}

	if (strPoss == m_strDLMM)
	{
		GetDlgItem(IDC_ST_MESSAGE)->SetWindowTextW(L"�������\r\n��ȷ��");
		CDialogEx::OnOK();
	}
	else
	{
		GetDlgItem(IDC_ST_MESSAGE)->SetWindowTextW(L"�������\r\n�ټ�!!");
		Sleep(1000);
		CDialogEx::OnCancel();
	}
	
}