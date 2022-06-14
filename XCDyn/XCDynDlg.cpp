
// XCDynDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XCDyn.h"
#include "XCDynDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXCDynDlg 对话框




CXCDynDlg::CXCDynDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CXCDynDlg::IDD, pParent)
	, m_nDlgFontSize(45)
{
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// 针对1280*1024分辨率进行开发
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
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
		L"宋体");

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


// CXCDynDlg 消息处理程序

BOOL CXCDynDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitCtrls();

	// 延时打开串口
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXCDynDlg::OnPaint()
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

	m_lbInfo.SetText(L"正在通讯...");
	m_lbInfo.SetTitleStyle();

	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->SetWindowText(L"降举升器");
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->SetWindowText(L"关电机");
}


void CXCDynDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginWaitCursor();
	CString strName;
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->GetWindowText(strName);

	if (L"升举升器" == strName)
	{
		m_pDynThread->LiftBeamUp();

		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->SetWindowText(L"降举升器");
	}
	else if (L"降举升器" == strName)
	{
		m_pDynThread->LiftBeamDown();
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->SetWindowText(L"升举升器");
	}

	EndWaitCursor();
}


void CXCDynDlg::OnBnClickedButtonMotorOn()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginWaitCursor();
	CString strName;
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->GetWindowText(strName);

	if (L"开电机" == strName)
	{
		m_pDynThread->MotorOn();

		GetDlgItem(IDC_BUTTON_MOTOR_ON)->SetWindowText(L"关电机");
	}
	else if (L"关电机" == strName)
	{
		m_pDynThread->MotorOff();

		GetDlgItem(IDC_BUTTON_MOTOR_ON)->SetWindowText(L"开电机");
	}

	EndWaitCursor();
}


void CXCDynDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	strPath = strPath + L"\\Config\\Equ.ini";

	// 去除配置文件只读属性
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);

	// 获取通讯端口
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// 转化为大写
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
		// 用取状态进行通讯测试
		if (DYN_WR_SUCCEED == m_pDynThread->XC_StopPWN())
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("测功机常用操作"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
	}
}
