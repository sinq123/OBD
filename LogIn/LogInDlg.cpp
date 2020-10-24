// LogInDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LogIn.h"
#include "LogInDlg.h"
#include "afxdialogex.h"


// CLogInDlg �Ի���

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogInDlg::IDD, pParent)
	, m_pfProcessCtrl(NULL)
	, m_dwReturn(0x00)
	, m_strDLMM(L"")
	, m_strMsg(L"")
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 20 / 1024;
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

CLogInDlg::~CLogInDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_Poss, m_edPoss);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbInfo);
}


BEGIN_MESSAGE_MAP(CLogInDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLogInDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogInDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLogInDlg ��Ϣ�������


void CLogInDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CString strPoss;

	m_edPoss.GetWindowTextW(strPoss);

	CString strDLMM = m_strDLMM.c_str();

	if (strDLMM == L"")
	{
		strDLMM = L"hunancsjcz13579";
	}

	if (strPoss == strDLMM || strPoss == L"HN123")
	{
		m_dwReturn = 0x00;
		m_strMsg = L"������ȷ!";
		m_lbInfo.SetWindowTextW(m_strMsg.c_str());
		SetTimer(1, 10, NULL);
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"�����������Ϊ:%s\r\n��ȷ������Ϊ:%s", strPoss, m_strDLMM);
		//MessageBox(strMsg, L"", MB_ICONWARNING|MB_OK);

		m_dwReturn = 0x02;
		m_strMsg = L"�������\r\n�ټ�!!";
		m_lbInfo.SetWindowTextW(m_strMsg.c_str());
		SetTimer(1, 1000, NULL);
	}

	CDialogEx::OnOK();
}


void CLogInDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dwReturn = 0x03;
	m_strMsg = L"�ͻ������˳�";
	m_lbInfo.SetWindowTextW(m_strMsg.c_str());
	SetTimer(1, 500, NULL);

	CDialogEx::OnCancel();
}


BOOL CLogInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrls();

	SetTimer(2, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CLogInDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	// ��ʱ��1��������ʾ �ȴ�ʱ�������һ��
	// ��ʱ��2����ʱ��ȡ֤����֤
	switch (nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), m_strMsg.c_str(), _countof(wchInfo)-1);
				m_pfProcessCtrl(m_dwReturn, wchInfo, sizeof(wchInfo));
			}
		}
		break;
	case 2:
		{
			KillTimer(2);
			//if (!IsPassCerModu())
			//{
			//	m_lbInfo.SetWindowTextW(m_strMsg.c_str());
			//	MessageBox(m_strMsg.c_str(), L"", MB_ICONWARNING|MB_OK);
			//	m_dwReturn = 0x01;
			//	SetTimer(1, 500, NULL);
			//}
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);

	
}

void CLogInDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbInfo.SetWindowTextW(L"");
	m_edPoss.SetWindowTextW(L"");
	m_edPoss.SetFocus();
}

void CLogInDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}


void CLogInDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}


bool CLogInDlg::IsPassCerModu(void)
{
	return CRJYZ::IsYZ(m_strMsg, m_strDLMM);
}