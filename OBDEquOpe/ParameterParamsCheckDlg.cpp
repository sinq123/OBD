
// ParameterParamsCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParameterParamsCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CParameterParamsCheckDlg 对话框

CParameterParamsCheckDlg::CParameterParamsCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParameterParamsCheckDlg::IDD, pParent)
	, m_pParaThread(NULL)
{
	// 启动测量仪线程
	//m_pParaThread = (CParaThread*)AfxBeginThread(RUNTIME_CLASS(CParaThread));
	m_pParaThread = CNHEuqController::GetInstance().m_pParaThread;

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


// CParameterParamsCheckDlg 消息处理程序

BOOL CParameterParamsCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

	InitCtrls();

	// 延时打开串口
	SetTimer(1, 1000, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CParameterParamsCheckDlg::OnPaint()
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
	}
	else
	{
		CDialogZoom::OnPaint();
	}
}

void CParameterParamsCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"参数测量仪参数检查");

	m_lbInfo.SetText(L"正在通讯...");

	// 设置标签颜色
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		////////////////////////////////////////////////////////// 从配置文件获取南华测量仪通讯端口begin
		CString strPath;
		// 获取exe(dll)文件绝对路径
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		// 向上2层
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		// 配置文件路径
		strPath = strPath + L"\\Config\\Equipment.ini";

		// 去除配置文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPath);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPath, dwAttributes);

		CSimpleIni si(strPath);
		// 获取通讯端口
		BYTE bPort = (BYTE)_wtoi(si.GetString(L"ParaParams", L"Port", L"7"));
		// 获取分析仪通讯协议类型,默认为503
		CString strParaProtocol = si.GetString(L"ParaParams", L"Protocol", L"NHP");
		// 转化为大写
		strParaProtocol.MakeUpper();
		//if (PARA_OPEN_SUCCEED == m_pParaThread->Open(bPort, CParaThread::IM_NANHUA_PARA))
		if (m_pParaThread->IsOpen())
		{
			if (PARA_WR_SUCCEED == m_pParaThread->GetEntireData())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("参数测量仪实时测量数据"));
				// 刷新转速
				SetTimer(2, 1000, NULL);
			}
			else
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			}
		}
		else
		{
			// 打开串口失败
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
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
	// 刷新数据
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
	// TODO: 在此添加控件通知处理程序代码

	KillTimer(1);
	KillTimer(2);

	OnCancel();
}

