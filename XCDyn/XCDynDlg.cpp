
// XCDynDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XCDyn.h"
#include "XCDynDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXCDynDlg �Ի���




CXCDynDlg::CXCDynDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CXCDynDlg::IDD, pParent)
	, m_nDlgFontSize(45)
{
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
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
		L"����");

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CXCDynDlg::~CXCDynDlg(void)
{
	m_fontDlgFont.DeleteObject();

	if (m_pDynThread != NULL)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}

}

void CXCDynDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

}

BEGIN_MESSAGE_MAP(CXCDynDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CXCDynDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CXCDynDlg::OnBnClickedButtonMotorOn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CXCDynDlg ��Ϣ�������

BOOL CXCDynDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXCDynDlg::OnPaint()
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
		CDialogZoom::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CXCDynDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CXCDynDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CXCDynDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbInfo.SetText(L"����ͨѶ...");
	m_lbInfo.SetTitleStyle();

	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->SetWindowText(L"��������");
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->SetWindowText(L"�ص��");
}


void CXCDynDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();
	CString strName;
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->GetWindowText(strName);

	if (L"��������" == strName)
	{
		m_pDynThread->LiftBeamUp();

		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->SetWindowText(L"��������");
	}
	else if (L"��������" == strName)
	{
		m_pDynThread->LiftBeamDown();
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->SetWindowText(L"��������");
	}

	EndWaitCursor();
}


void CXCDynDlg::OnBnClickedButtonMotorOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();
	CString strName;
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->GetWindowText(strName);

	if (L"�����" == strName)
	{
		m_pDynThread->MotorOn();

		GetDlgItem(IDC_BUTTON_MOTOR_ON)->SetWindowText(L"�ص��");
	}
	else if (L"�ص��" == strName)
	{
		m_pDynThread->MotorOff();

		GetDlgItem(IDC_BUTTON_MOTOR_ON)->SetWindowText(L"�����");
	}

	EndWaitCursor();
}


void CXCDynDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			OpenSerialPortAndTestCommunication();
		}
		break;
	default:{}break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CXCDynDlg::OpenSerialPortAndTestCommunication(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Equ.ini";

	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);

	// ��ȡͨѶ�˿�
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// ת��Ϊ��д
	strDynProtocol.MakeUpper();
	CDynThread::InstrumentModel imDyn;
	if (L"NANHUA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}
	else if (L"MAHA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_MAHA_ASMP;
	}
	else if (L"XINCHENG" == strDynProtocol)
	{
		imDyn = CDynThread::IM_XIN_CHENG;
	}
	else
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}


	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
	{
		// ��ȡ״̬����ͨѶ����
		if (DYN_WR_SUCCEED == m_pDynThread->XC_StopPWN())
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("�⹦�����ò���"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
	}
}
