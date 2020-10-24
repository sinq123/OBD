
// ParameterParamsCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ParameterParamsCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CParameterParamsCheckDlg �Ի���

CParameterParamsCheckDlg::CParameterParamsCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParameterParamsCheckDlg::IDD, pParent)
	, m_pParaThread(NULL)
{
	// �����������߳�
	//m_pParaThread = (CParaThread*)AfxBeginThread(RUNTIME_CLASS(CParaThread));
	m_pParaThread = CNHEuqController::GetInstance().m_pParaThread;

	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
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
		_T("����"));
}

CParameterParamsCheckDlg::~CParameterParamsCheckDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pParaThread)
	//{
	//	m_pParaThread->Close();
	//	m_pParaThread->QuitThread();
	//	WaitForSingleObject(m_pParaThread->m_hThread, 5000);
	//}
}

void CParameterParamsCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);
	DDX_Control(pDX, IDC_STATIC_LABEL5, m_lbLabel5);

	DDX_Control(pDX, IDC_STATIC_ET, m_lbET);
	DDX_Control(pDX, IDC_STATIC_RH, m_lbRH);
	DDX_Control(pDX, IDC_STATIC_AP, m_lbAP);
	DDX_Control(pDX, IDC_STATIC_OT, m_lbOT);
	DDX_Control(pDX, IDC_STATIC_RPM, m_lbRev);
}

BEGIN_MESSAGE_MAP(CParameterParamsCheckDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CParameterParamsCheckDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CParameterParamsCheckDlg ��Ϣ�������

BOOL CParameterParamsCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(1, 1000, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CParameterParamsCheckDlg::OnPaint()
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
	}
	else
	{
		CDialogZoom::OnPaint();
	}
}

void CParameterParamsCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"���������ǲ������");

	m_lbInfo.SetText(L"����ͨѶ...");

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));
	m_lbLabel5.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetTitleStyle();
	m_lbET.SetTitleStyle();
	m_lbRH.SetTitleStyle();
	m_lbAP.SetTitleStyle();
	m_lbOT.SetTitleStyle();
	m_lbRev.SetTitleStyle();
}

void CParameterParamsCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CParameterParamsCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		////////////////////////////////////////////////////////// �������ļ���ȡ�ϻ�������ͨѶ�˿�begin
		CString strPath;
		// ��ȡexe(dll)�ļ�����·��
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		// ����2��
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		// �����ļ�·��
		strPath = strPath + L"\\Config\\Equipment.ini";

		// ȥ�������ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPath);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPath, dwAttributes);

		CSimpleIni si(strPath);
		// ��ȡͨѶ�˿�
		BYTE bPort = (BYTE)_wtoi(si.GetString(L"ParaParams", L"Port", L"7"));
		// ��ȡ������ͨѶЭ������,Ĭ��Ϊ503
		CString strParaProtocol = si.GetString(L"ParaParams", L"Protocol", L"NHP");
		// ת��Ϊ��д
		strParaProtocol.MakeUpper();
		//if (PARA_OPEN_SUCCEED == m_pParaThread->Open(bPort, CParaThread::IM_NANHUA_PARA))
		if (m_pParaThread->IsOpen())
		{
			if (PARA_WR_SUCCEED == m_pParaThread->GetEntireData())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("����������ʵʱ��������"));
				// ˢ��ת��
				SetTimer(2, 1000, NULL);
			}
			else
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			}
		}
		else
		{
			// �򿪴���ʧ��
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		}
	}
	else if (2 == nIDEvent)
	{
		RefreshData();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CParameterParamsCheckDlg::RefreshData(void)
{
	// ˢ������
	CString str(L"");
	float fET(0.0f);
	float fRH(0.0f);
	float fAP(0.0f);
	float fOT(0.0f);
	USHORT usRev(0);

	m_pParaThread->GetEntireData(&fET, &fRH, &fAP, &fOT, &usRev);

	str.Format(L"%.1f", fET);
	m_lbET.SetWindowTextW(str);

	str.Format(L"%.1f", fRH);
	m_lbRH.SetWindowTextW(str);

	str.Format(L"%.1f", fAP);
	m_lbAP.SetWindowTextW(str);

	str.Format(L"%.1f", fOT);
	m_lbOT.SetWindowTextW(str);

	str.Format(L"%d", usRev);
	m_lbRev.SetWindowTextW(str);
}

void CParameterParamsCheckDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	KillTimer(1);
	KillTimer(2);

	OnCancel();
}

