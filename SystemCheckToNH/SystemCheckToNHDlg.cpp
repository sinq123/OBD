
// SystemCheckToNHDlg.cpp : 实现文件
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


// CSystemCheckToNHDlg 对话框




CSystemCheckToNHDlg::CSystemCheckToNHDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CSystemCheckToNHDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
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
		_T("宋体"));
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


// CSystemCheckToNHDlg 消息处理程序

BOOL CSystemCheckToNHDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 初始化控件
	InitCtrls();

	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);
	// 刷新时间
	SetTimer(1, 1000, NULL);

	// 初始化
	CNHEuqController::GetInstance().Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSystemCheckToNHDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogZoom::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// 反初始化
	CNHEuqController::GetInstance().UnInit();

	OnCancel();
}

void CSystemCheckToNHDlg::InitCtrls(void)
{
	SetDlgFont();

	m_ibTrainingPresentation.SetImage(IDR_PNG_TRAINING_PRESENTATION, L"PNG");
	m_ibTrainingPresentation.SetTitle(L"培训演示");
	m_ibTrainingPresentation.SetDescribe(L"  培训演示");

	m_ibSystemCheck.SetImage(IDR_PNG_SYSTEM_CHECK, L"PNG");
	m_ibSystemCheck.SetTitle(L"系统自检");
	m_ibSystemCheck.SetDescribe(L"  设备日常检查");

	m_ibEmissionTest.SetImage(IDR_PNG_EMISSION_TEST, L"PNG");
	m_ibEmissionTest.SetTitle(L"排放检测");
	m_ibEmissionTest.SetDescribe(L"  排放检测");
		
	m_ibDailyDemarcation.SetImage(IDR_PNG_DAILY_DEMARCATION, L"PNG");
	m_ibDailyDemarcation.SetTitle(L"日常标定");
	m_ibDailyDemarcation.SetDescribe(L"  设备标定/检查/维修保养");

	m_ibDataMangement.SetImage(IDR_PNG_DATA_MANAGEMENT, L"PNG");
	m_ibDataMangement.SetTitle(L"数据管理");
	m_ibDataMangement.SetDescribe(L"  查询/统计本机数据");

	m_ibSystemConfig.SetImage(IDR_PNG_SYSTEM_CONFIG, L"PNG");
	m_ibSystemConfig.SetTitle(L"系统管理");
	m_ibSystemConfig.SetDescribe(L"  配置系统的运行参数");

	CString strSign;
	strSign.Format(L"EDS V%s[%s]", MAIN_MINOR_VERSION, ENG_PRO_NAME);
	SetWindowText(strSign);
	strSign.Format(L"%s", ENG_PRO_NAME);
	m_lbSign.SetBkTransparent();
	m_lbSign.SetFontEx(L"Verdana", m_nDlgFontSize);
	m_lbSign.SetTextEx(strSign, RGB(5, 119, 16));

	CString strVersion;
	m_iwNHLogo.SetImage(IDR_PNG_NH_LOGO, L"PNG");
	strVersion.Format(L"排放检测系统 V%s 佛山市南华仪器股份有限公司研制", MAIN_MINOR_VERSION);

	m_ssVersion.SetReverse(1);
	m_ssVersion.SetText(strVersion);
	m_iwEPLogo.SetImage(IDR_PNG_EP_LOGO, L"PNG");

	m_btnExit.SetIcon(IDI_ICON_SHUT_DOWN);

	COleDateTime odtCurrentTime = COleDateTime::GetCurrentTime();
	CString strCurrentTime(L"");
	strCurrentTime.Format(L"%d年%02d月%02d日 %02d时%02d分%02d秒", 
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
	str.Format(L"检测线编号:%s 设备认证编号:%s", 
		sLineInfo.wchLineNumber, 
		sLineInfo.wchLineLicenseSn);
	m_lbLineNumberAndEquipmentLicense.SetText(str);

	STATIONINFO sStationInfo;
	GetIniStationInfo(&sStationInfo);

	GetIniNHClient(&m_sIniNHClient);
	// 程序标题读 NHClient.ini
	wchar_t wchPath[MAX_PATH] = {0};
	CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath);
	CSimpleIni NHClient_ini(wchPath);
	CString strAppTitle = NHClient_ini.GetString(L"System", L"AppTitle", L"");
	if (strAppTitle.IsEmpty())
	{
		strAppTitle = sStationInfo.wchStationFullName;
	}
	// 如果NHClient.ini中没有定义，就读数据库的StationInfo.StationFullName
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 1:
		{
			COleDateTime odtCurrentTime = COleDateTime::GetCurrentTime();
			CString strCurrentTime(L"");
			strCurrentTime.Format(L"%d年%02d月%02d日 %02d时%02d分%02d秒", 
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