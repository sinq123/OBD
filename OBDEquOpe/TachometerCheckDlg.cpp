
// TachometerCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TachometerCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTachometerCheckDlg �Ի���




CTachometerCheckDlg::CTachometerCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CTachometerCheckDlg::IDD, pParent)
	, m_pTacThread(NULL)
{
	// ����ת�ټ��߳�
	m_pTacThread = (CTacThread*)AfxBeginThread(RUNTIME_CLASS(CTacThread));

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

CTachometerCheckDlg::~CTachometerCheckDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pTacThread)
	{
		m_pTacThread->Close();
		m_pTacThread->QuitThread();
		WaitForSingleObject(m_pTacThread->m_hThread, 5000);
	}
}

void CTachometerCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_REV, m_lbRev);
}

BEGIN_MESSAGE_MAP(CTachometerCheckDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CTachometerCheckDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTachometerCheckDlg ��Ϣ�������

BOOL CTachometerCheckDlg::OnInitDialog()
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

void CTachometerCheckDlg::OnPaint()
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

void CTachometerCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"ת�ټ�ת�ټ��");

	m_lbInfo.SetText(L"����ͨѶ...");
	m_lbInfo.SetTitleStyle();

	m_lbRev.SetTitleStyle();
}

void CTachometerCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CTachometerCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		////////////////////////////////////////////////////////// �������ļ���ȡ�ϻ��⹦��ͨѶ�˿�begin
		CString strPath;
		// ��ȡexe�ļ�����·��
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
		CString strSerialPort = si.GetString(L"TacParams", L"Port", L"0");
		BYTE bSerialPort = BYTE(_tstoi(strSerialPort));
		CString strProtocol = si.GetString(L"TacParams", L"Protocol", L"RPM5300");
		////////////////////////////////////////////////////////// �������ļ���ȡ�ϻ��⹦��ͨѶ�˿�end
		if (TAC_OPEN_SUCCEED == m_pTacThread->Open(bSerialPort, strProtocol))
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ת�ټ�RPM5300����"));
			// ˢ��ת��
			SetTimer(2, 1000, NULL);
		}
		else
		{
			// �򿪴���ʧ��
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		}
	}
	else if (2 == nIDEvent)
	{
		//DWORD dwbegin = GetTickCount();
		m_pTacThread->GetRev();
		//DWORD dwend  = GetTickCount() - dwbegin;
		//TRACE("%u\n", dwend);

		CString str;
		str.Format(L"%d", m_pTacThread->GetRevValue());
		m_lbRev.SetText(str);
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CTachometerCheckDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	KillTimer(1);
	KillTimer(2);

	OnCancel();
}

