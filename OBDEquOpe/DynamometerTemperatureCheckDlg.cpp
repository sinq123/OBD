
// DynamometerTemperatureCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DynamometerTemperatureCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamometerTemperatureCheckDlg �Ի���




CDynamometerTemperatureCheckDlg::CDynamometerTemperatureCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDynamometerTemperatureCheckDlg::IDD, pParent)
	, m_pDynThread(NULL)
	, m_bRefreshDataSign(true)
{
	// ����ת�ټ��߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

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

CDynamometerTemperatureCheckDlg::~CDynamometerTemperatureCheckDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CDynamometerTemperatureCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_FORCE_TEMP, m_lbForceTemperature);
	DDX_Control(pDX, IDC_STATIC_BEARING_TEMP, m_lbBearingTemperature);
}

BEGIN_MESSAGE_MAP(CDynamometerTemperatureCheckDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDynamometerTemperatureCheckDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDynamometerTemperatureCheckDlg ��Ϣ�������

BOOL CDynamometerTemperatureCheckDlg::OnInitDialog()
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

void CDynamometerTemperatureCheckDlg::OnPaint()
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

void CDynamometerTemperatureCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦���¶ȼ��");

	m_lbInfo.SetText(L"����ͨѶ...");
	m_lbInfo.SetTitleStyle();

	m_lbForceTemperature.SetLEDStyle();
	m_lbBearingTemperature.SetLEDStyle();
}

void CDynamometerTemperatureCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CDynamometerTemperatureCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		////////////////////////////////////////////////////////// �������ļ���ȡ�ϻ��⹦��ͨѶ�˿�begin

		CString strPath;
		// ��ȡexe(dll)�ļ�����·��
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		// ����2��
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		// �����ļ�·��
		strPath = strPath + L"\\Config\\Equipment.ini";

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
		//else if (L"VAMAGRSR576" == strDynProtocol)
		//{
		//	imDyn = CDynThread::IM_VAMAG_RSR576;
		//}
		else
		{
			imDyn = CDynThread::IM_NANHUA_NHC03;
		}

		if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
		{
			std::bitset<16> bit16(0);
			if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bit16))
			{
				m_lbInfo.SetText(_T("�⹦���¶�ʵʱ�������"));
				// ˢ���¶�
				SetTimer(2, 200, NULL);
			}
			else
			{
				m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			}
		}
		else
		{
			m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		}		
	}
	else if (2 == nIDEvent)
	{
		RefreshData(m_bRefreshDataSign);
		m_bRefreshDataSign = !m_bRefreshDataSign;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CDynamometerTemperatureCheckDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	KillTimer(1);
	KillTimer(2);

	OnCancel();
}

void CDynamometerTemperatureCheckDlg::RefreshData(bool bSign/* = true*/)
{
	float fTemp(0.0f);
	CString str(L"");
	if (bSign)
	{
		m_pDynThread->ReadVar(222);
		fTemp = (float)(m_pDynThread->GetVarValue(222)/1.0f - 123.0f);
		str.Format(L"%.1f", fTemp);
		m_lbForceTemperature.SetText(str);
	}
	else
	{
		m_pDynThread->ReadVar(224);
		fTemp = (float)(m_pDynThread->GetVarValue(224)/1.0f - 123.0f);
		str.Format(L"%.1f", fTemp);
		m_lbBearingTemperature.SetText(str);
	}
}

