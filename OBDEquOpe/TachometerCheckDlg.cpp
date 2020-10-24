
// TachometerCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TachometerCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTachometerCheckDlg 对话框




CTachometerCheckDlg::CTachometerCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CTachometerCheckDlg::IDD, pParent)
	, m_pTacThread(NULL)
{
	// 启动转速计线程
	m_pTacThread = (CTacThread*)AfxBeginThread(RUNTIME_CLASS(CTacThread));

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


// CTachometerCheckDlg 消息处理程序

BOOL CTachometerCheckDlg::OnInitDialog()
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

void CTachometerCheckDlg::OnPaint()
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

void CTachometerCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"转速计转速检查");

	m_lbInfo.SetText(L"正在通讯...");
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		////////////////////////////////////////////////////////// 从配置文件获取南华测功机通讯端口begin
		CString strPath;
		// 获取exe文件绝对路径
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
		CString strSerialPort = si.GetString(L"TacParams", L"Port", L"0");
		BYTE bSerialPort = BYTE(_tstoi(strSerialPort));
		CString strProtocol = si.GetString(L"TacParams", L"Protocol", L"RPM5300");
		////////////////////////////////////////////////////////// 从配置文件获取南华测功机通讯端口end
		if (TAC_OPEN_SUCCEED == m_pTacThread->Open(bSerialPort, strProtocol))
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("转速计RPM5300读数"));
			// 刷新转速
			SetTimer(2, 1000, NULL);
		}
		else
		{
			// 打开串口失败
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
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
	// TODO: 在此添加控件通知处理程序代码

	KillTimer(1);
	KillTimer(2);

	OnCancel();
}

