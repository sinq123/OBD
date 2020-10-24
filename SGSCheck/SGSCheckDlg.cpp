
// SGSCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SGSCheck.h"
#include "SGSCheckDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSGSCheckDlg �Ի���

CSGSCheckDlg* g_pSGSCheckDlg(NULL);


CSGSCheckDlg::CSGSCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CSGSCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	//m_nDlgFontSize = nSM_CYSCREEN * 25 / 1024;
	m_nDlgFontSize = 14;
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

CSGSCheckDlg::~CSGSCheckDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CSGSCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_BTN_STATR, m_btnState);
	DDX_Control(pDX, IDC_BTN_RESTATR, m_btnReState);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_ST_EQU_SELF_INS, m_lbEquSelfIns);
	DDX_Control(pDX, IDC_ST_INTERVAL, m_lbInterva);
	DDX_Control(pDX, IDC_ST_GAS_LEAK_HUNNTING, m_lbGasleakhunnting);
	DDX_Control(pDX, IDC_ST_CURRENT_TIME, m_lbCurrentTime);

	DDX_Control(pDX, IDC_ST_BK1, m_BK1);
	DDX_Control(pDX, IDC_ST_BK3, m_BK3);
	DDX_Control(pDX, IDC_CK_QYC_COUST_LOAD, m_chkQYCCoustLoad);
	DDX_Control(pDX, IDC_CK_CYC_COUST_LOAD, m_chkCYCCoustLoad);
	DDX_Control(pDX, IDC_CK_QYC_PLHP, m_chkQYCPLHP);
	DDX_Control(pDX, IDC_CK_CYC_PLHP, m_chkCYCPLHP);
	DDX_Control(pDX, IDC_CK_GAS_CHECH, m_chkGasChech);
	DDX_Control(pDX, IDC_CK_LOW_FOLW_CHECH, m_chkLowFlowChech);
	DDX_Control(pDX, IDC_CK_SMOKE_METER, m_chkSmokeMeter);
	DDX_Control(pDX, IDC_CK_NOX_ANALYZER, m_chkNOxAnalyzeer);
	DDX_Control(pDX, IDC_CK_FLOWMETER, m_chkFlowMeter);
	DDX_Control(pDX, IDC_CK_WEATHER_STATION, m_chkWetherStation);
	DDX_Control(pDX, IDC_CK_TACHOMETER, m_chkTachoMerter);

	DDX_Control(pDX, IDC_ST_SPEEDOMETER, m_stSpeedometer);
	DDX_Control(pDX, IDC_ST_TACHOMETER, m_stTachometer);
}

BEGIN_MESSAGE_MAP(CSGSCheckDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSGSCheckDlg ��Ϣ�������

BOOL CSGSCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	g_pSGSCheckDlg = this;
	// ��ʼ���ؼ�
	InitCtrls();
	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);
	
	// ˢ��ʱ��
	CString strCurrentTime(L"");
	strCurrentTime.Format(L"��ǰʱ��:%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	m_lbCurrentTime.SetText(strCurrentTime);
	SetTimer(1, 1000, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSGSCheckDlg::OnPaint()
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
HCURSOR CSGSCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSGSCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case 1:
		{
			CString strCurrentTime(L"");
			strCurrentTime.Format(L"��ǰʱ��:%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			m_lbCurrentTime.SetText(strCurrentTime);
		}
		break;

	default:
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}


void CSGSCheckDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogZoom::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CSGSCheckDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogZoom::OnClose();
}

void CSGSCheckDlg::InitCtrls(void)
{
	SetDlgFont();
	//m_btnExit.SetFlat(FALSE);

	auto SetBTN = [] (CButtonST* pBTN)
	{
		//�����ڰ�ť��ʱ�ı���ɫ 
		pBTN->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(128,128,128));
		//��ͨ״̬ʱ�ı���ɫ
		pBTN->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(128,128,128));
		//��ť�����º�ı���ɫ 
		pBTN->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(128,128,128));
		//�����ڰ�ť��ʱ��ǰ��ɫ
		pBTN->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,255));
		//��ͨ״̬ʱ��ǰ��ɫ 
		pBTN->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
		//��ť�����º��ǰ��ɫ
		pBTN->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));

		pBTN->SetFont(&g_pSGSCheckDlg->m_fontDlgFont);
	};
	SetBTN(&m_btnExit);
	SetBTN(&m_btnState);
	SetBTN(&m_btnReState);
	SetBTN(&m_btnStop);

	auto SetLB = [] (CLabel* plb)
	{
		plb->SetTextColor(RGB(255,255,255));
		plb->SetBkColor(RGB(128,128,128));
	};

	SetLB(&m_lbEquSelfIns);
	SetLB(&m_lbInterva);
	SetLB(&m_lbGasleakhunnting);
	SetLB(&m_lbCurrentTime);
	m_lbInterva.SetWindowTextW(L"");

	//m_BK1.SetBackgroundColor(RGB(0,0,0),RGB(0,0,0));

	

	auto SetCHK = [] (CButtonST* pChk)
	{
		pChk->SetIcon(IDI_ICON_CHK_BOX_ON, IDI_ICON_CHK_BOX_OFF);
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,255,255));
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255,255,255));
		pChk->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255,255,255));

		pChk->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(192,192,192));
		pChk->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(192,192,192));
		pChk->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(192,192,192));

		pChk->SetFont(&g_pSGSCheckDlg->m_fontDlgFont);
		pChk->DrawBorder(FALSE);
	};

	SetCHK(&m_chkQYCCoustLoad);
	SetCHK(&m_chkCYCCoustLoad);
	SetCHK(&m_chkQYCPLHP);
	SetCHK(&m_chkCYCPLHP);
	SetCHK(&m_chkGasChech);
	SetCHK(&m_chkLowFlowChech);
	SetCHK(&m_chkSmokeMeter);
	SetCHK(&m_chkNOxAnalyzeer);
	SetCHK(&m_chkFlowMeter);
	SetCHK(&m_chkWetherStation);
	SetCHK(&m_chkTachoMerter);


	auto SetPGB = [] (CXPGroupBox* pPGB)
	{
		pPGB->SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
		pPGB->SetBackgroundColor(RGB(192,192,192));
	};
	SetPGB(&m_BK1);
	SetPGB(&m_BK3);


	// ���ٱ�
	CString str = _T("km/h");
	m_stSpeedometer.SetUnits(str);						// ���õ�λ
	m_stSpeedometer.SetTitle(_T(""));					// ����
	m_stSpeedometer.SetRange(0,90);					// ���ÿ̶ȷ�Χ
	m_stSpeedometer.SetTicks(9);						// ���ÿ̶���
	m_stSpeedometer.SetTickScale(1);					// ���ñ�����
	m_stSpeedometer.SetValueDecimals(1);				// ����С��λ
	m_stSpeedometer.SetSubTicks(10);				// �����ӿ̶ȣ�ÿ����Ϊ��С��
	m_stSpeedometer.SetValueColor(RGB(255,255,255));	// ������ʾֵ��ɫ
	m_stSpeedometer.SetFrameColor(RGB(192,192,192));	// ������򱳾���ɫ
	m_stSpeedometer.SetMeterBkColor(RGB(192,192,192));	//���ñ��̱�����ɫ
	//m_stSpeedometer.SetFrameBorder(1000, RGB(255,255,255));
	m_stSpeedometer.SetNeedleColor(RGB(0,255,0)); //����ָ����ɫ
	m_stSpeedometer.UpdateNeedle(0);
	
	// ת�ٱ�
	str = _T("x1000r/min");
	m_stTachometer.SetUnits(str);						// ���õ�λ
	m_stTachometer.SetTitle(_T(""));
	m_stTachometer.SetRange(0,4);					// ���ÿ̶ȷ�Χ
	m_stTachometer.SetTicks(4);						// ���ÿ̶���
	m_stTachometer.SetTickScale(1);					// ���ñ�����
	m_stTachometer.SetValueDecimals(1);				// ����С��λ
	m_stTachometer.SetValueColor(RGB(58, 248, 250));	// ������ʾֵ��ɫ
	m_stTachometer.SetSubTicks(10);				// �����ӿ̶ȣ�ÿ����Ϊ��С��
	m_stTachometer.SetValueColor(RGB(255,255,255));	// ������ʾֵ��ɫ
	m_stTachometer.SetFrameColor(RGB(192,192,192));	// ������򱳾���ɫ
	m_stTachometer.SetMeterBkColor(RGB(192,192,192));	//���ñ��̱�����ɫ
	m_stTachometer.SetNeedleColor(RGB(0,255,0)); //����ָ����ɫ
	m_stTachometer.SetAngleRange(140, 310);
	m_stTachometer.UpdateNeedle(0);

	//����CPanel
	if (true)
	{
		CRect rect;
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_ST_BK2);
		pWnd->GetWindowRect(&rect);  
		//GetDlgItem(IDC_ST_BK2)->GetClientRect(&rect);
		//�������
		CPanel* panel = new CPanel();
		panel->Create(&rect, this);

		CPanelItem* pItem1 = new CPanelItem();
		pItem1->Create(IDD_SELF_INSRECTION_ITEMS_DIALOG, _T(""), RGB(74,141,186), panel, 0);
		pItem1->ShowWindow(SW_SHOW);
		panel->AddPanel(pItem1);
	}
}

void CSGSCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);

		TCHAR tchs[20] = {0};
		GetClassName(pWnd->GetSafeHwnd(), tchs, 20);
		if (_tcscmp(tchs, _T("ComboBox")) == 0)
		{
			((CComboBox *)pWnd)->SetItemHeight(-1, 25);
		}

		pWnd = pWnd->GetNextWindow();
	}
}
