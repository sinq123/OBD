
// SystemCheckToNHDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemCheckToNH.h"
#include "SystemCheckToNHDlg.h"
#include "afxdialogex.h"
#include "NHDailyDemarcationDlg.h"
#include "NHSystemCheckDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSystemCheckToNHDlg �Ի���




CSystemCheckToNHDlg::CSystemCheckToNHDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CSystemCheckToNHDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

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

CSystemCheckToNHDlg::~CSystemCheckToNHDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CSystemCheckToNHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_EMISSION_TEST, m_ibEmissionTest);
	DDX_Control(pDX, IDC_BTN_DAILY_DEMARCATION, m_ibDailyDemarcation);
	DDX_Control(pDX, IDC_BTN_DATA_MANAGEMENT, m_ibDataMangement);
	DDX_Control(pDX, IDC_BTN_SYSTEM_MANAGEMENT, m_ibSystemConfig);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_ssVersion);
	DDX_Control(pDX, IDC_STATIC_NH_LOGO, m_iwNHLogo);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_stTitle);
	DDX_Control(pDX, IDC_STATIC_EP_LOGO, m_iwEPLogo);
	DDX_Control(pDX, IDC_BTN_TRAINING_PRESENTATION, m_ibTrainingPresentation);
	DDX_Control(pDX, IDC_BTN_SYSTEM_CHECK, m_ibSystemCheck);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_ST_LN_SN, m_lbLineNumberAndEquipmentLicense);
	DDX_Control(pDX, IDC_ST_TIME, m_lbTime);
	DDX_Control(pDX, IDC_ST_SIGN, m_lbSign);
}

BEGIN_MESSAGE_MAP(CSystemCheckToNHDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DAILY_DEMARCATION, &CSystemCheckToNHDlg::OnBnClickedBtnDailyDemarcation)
	ON_BN_CLICKED(IDC_BTN_EMISSION_TEST, &CSystemCheckToNHDlg::OnBnClickedBtnEmissionTest)
	ON_BN_CLICKED(IDC_BTN_DATA_MANAGEMENT, &CSystemCheckToNHDlg::OnBnClickedBtnDataManagement)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_MANAGEMENT, &CSystemCheckToNHDlg::OnBnClickedBtnSystemManagement)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_CHECK, &CSystemCheckToNHDlg::OnBnClickedBtnSystemCheck)
	ON_BN_CLICKED(IDC_BTN_TRAINING_PRESENTATION, &CSystemCheckToNHDlg::OnBnClickedBtnTrainingPresentation)
	ON_BN_CLICKED(IDCANCEL, &CSystemCheckToNHDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSystemCheckToNHDlg ��Ϣ�������

BOOL CSystemCheckToNHDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);
	// ˢ��ʱ��
	SetTimer(1, 1000, NULL);

	// ��ʼ��
	CNHEuqController::GetInstance().Init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSystemCheckToNHDlg::OnPaint()
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
HCURSOR CSystemCheckToNHDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSystemCheckToNHDlg::OnBnClickedBtnDailyDemarcation()
{
	CNHDailyDemarcationDlg dlg;
	const DWORD dwReturn = (DWORD)dlg.DoModal();
}

void CSystemCheckToNHDlg::OnBnClickedBtnEmissionTest()
{
}

void CSystemCheckToNHDlg::OnBnClickedBtnDataManagement()
{
}

void CSystemCheckToNHDlg::OnBnClickedBtnSystemManagement()
{
}

void CSystemCheckToNHDlg::OnBnClickedBtnSystemCheck()
{
	GetLocalTime(&m_sCurrencyTime);
	SetDboLineInfoLineState(m_sIniNHClient.wchLineNumber, EQU_SYSTEMCHECK, &m_sCurrencyTime);

	CNHSystemCheckDlg dlg;
	const DWORD dwReturn = (DWORD)dlg.DoModal();

	GetLocalTime(&m_sCurrencyTime);
	SetDboLineInfoLineState(m_sIniNHClient.wchLineNumber, EQU_FREE, &m_sCurrencyTime);
}

void CSystemCheckToNHDlg::OnBnClickedBtnTrainingPresentation()
{
}

void CSystemCheckToNHDlg::OnBnClickedCancel()
{
	// ����ʼ��
	CNHEuqController::GetInstance().UnInit();

	OnCancel();
}

void CSystemCheckToNHDlg::InitCtrls(void)
{
	SetDlgFont();

	m_ibTrainingPresentation.SetImage(IDR_PNG_TRAINING_PRESENTATION, L"PNG");
	m_ibTrainingPresentation.SetTitle(L"��ѵ��ʾ");
	m_ibTrainingPresentation.SetDescribe(L"  ��ѵ��ʾ");

	m_ibSystemCheck.SetImage(IDR_PNG_SYSTEM_CHECK, L"PNG");
	m_ibSystemCheck.SetTitle(L"ϵͳ�Լ�");
	m_ibSystemCheck.SetDescribe(L"  �豸�ճ����");

	m_ibEmissionTest.SetImage(IDR_PNG_EMISSION_TEST, L"PNG");
	m_ibEmissionTest.SetTitle(L"�ŷż��");
	m_ibEmissionTest.SetDescribe(L"  �ŷż��");
		
	m_ibDailyDemarcation.SetImage(IDR_PNG_DAILY_DEMARCATION, L"PNG");
	m_ibDailyDemarcation.SetTitle(L"�ճ��궨");
	m_ibDailyDemarcation.SetDescribe(L"  �豸�궨/���/ά�ޱ���");

	m_ibDataMangement.SetImage(IDR_PNG_DATA_MANAGEMENT, L"PNG");
	m_ibDataMangement.SetTitle(L"���ݹ���");
	m_ibDataMangement.SetDescribe(L"  ��ѯ/ͳ�Ʊ�������");

	m_ibSystemConfig.SetImage(IDR_PNG_SYSTEM_CONFIG, L"PNG");
	m_ibSystemConfig.SetTitle(L"ϵͳ����");
	m_ibSystemConfig.SetDescribe(L"  ����ϵͳ�����в���");

	CString strSign;
	strSign.Format(L"EDS V%s[%s]", MAIN_MINOR_VERSION, ENG_PRO_NAME);
	SetWindowText(strSign);
	strSign.Format(L"%s", ENG_PRO_NAME);
	m_lbSign.SetBkTransparent();
	m_lbSign.SetFontEx(L"Verdana", m_nDlgFontSize);
	m_lbSign.SetTextEx(strSign, RGB(5, 119, 16));

	CString strVersion;
	m_iwNHLogo.SetImage(IDR_PNG_NH_LOGO, L"PNG");
	strVersion.Format(L"�ŷż��ϵͳ V%s ��ɽ���ϻ������ɷ����޹�˾����", MAIN_MINOR_VERSION);

	m_ssVersion.SetReverse(1);
	m_ssVersion.SetText(strVersion);
	m_iwEPLogo.SetImage(IDR_PNG_EP_LOGO, L"PNG");

	m_btnExit.SetIcon(IDI_ICON_SHUT_DOWN);

	COleDateTime odtCurrentTime = COleDateTime::GetCurrentTime();
	CString strCurrentTime(L"");
	strCurrentTime.Format(L"%d��%02d��%02d�� %02dʱ%02d��%02d��", 
		odtCurrentTime.GetYear(), 
		odtCurrentTime.GetMonth(), 
		odtCurrentTime.GetDay(), 
		odtCurrentTime.GetHour(), 
		odtCurrentTime.GetMinute(), 
		odtCurrentTime.GetSecond());
		m_lbTime.SetText(strCurrentTime);

	LINEINFO sLineInfo;
	GetIniLineInfo(&sLineInfo);
	CString str(L"");
	str.Format(L"����߱��:%s �豸��֤���:%s", 
		sLineInfo.wchLineNumber, 
		sLineInfo.wchLineLicenseSn);
	m_lbLineNumberAndEquipmentLicense.SetText(str);

	STATIONINFO sStationInfo;
	GetIniStationInfo(&sStationInfo);

	GetIniNHClient(&m_sIniNHClient);
	// �������� NHClient.ini
	wchar_t wchPath[MAX_PATH] = {0};
	CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath);
	CSimpleIni NHClient_ini(wchPath);
	CString strAppTitle = NHClient_ini.GetString(L"System", L"AppTitle", L"");
	if (strAppTitle.IsEmpty())
	{
		strAppTitle = sStationInfo.wchStationFullName;
	}
	// ���NHClient.ini��û�ж��壬�Ͷ����ݿ��StationInfo.StationFullName
	m_stTitle.SetTitle(strAppTitle);
}

void CSystemCheckToNHDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CSystemCheckToNHDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case 1:
		{
			COleDateTime odtCurrentTime = COleDateTime::GetCurrentTime();
			CString strCurrentTime(L"");
			strCurrentTime.Format(L"%d��%02d��%02d�� %02dʱ%02d��%02d��", 
				odtCurrentTime.GetYear(), 
				odtCurrentTime.GetMonth(), 
				odtCurrentTime.GetDay(), 
				odtCurrentTime.GetHour(), 
				odtCurrentTime.GetMinute(), 
				odtCurrentTime.GetSecond());
			m_lbTime.SetText(strCurrentTime);
		}
		break;

	default:
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}