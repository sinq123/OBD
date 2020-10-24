
// ACSCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ACSCheck.h"
#include "ACSCheckDlg.h"
#include "afxdialogex.h"
#include "HAT601Dlg.h"
#include "NHA509Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACSCheckDlg �Ի���




CACSCheckDlg::CACSCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CACSCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CACSCheckDlg::~CACSCheckDlg()
{
	// ����ʼ��
	CNHEuqController::GetInstance().UnInit();
}

void CACSCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CACSCheckDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_NHA509, &CACSCheckDlg::OnBnClickedBtnNha509)
	ON_BN_CLICKED(IDC_BTN_NHT601, &CACSCheckDlg::OnBnClickedBtnNht601)
END_MESSAGE_MAP()


// CACSCheckDlg ��Ϣ�������

BOOL CACSCheckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��ʼ��
	CNHEuqController::GetInstance().Init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CACSCheckDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CACSCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CACSCheckDlg::OnBnClickedBtnNha509()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CNHA509Dlg dlg;
	dlg.DoModal();
}


void CACSCheckDlg::OnBnClickedBtnNht601()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
