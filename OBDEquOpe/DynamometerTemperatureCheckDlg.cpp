
// DynamometerTemperatureCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DynamometerTemperatureCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamometerTemperatureCheckDlg 对话框




CDynamometerTemperatureCheckDlg::CDynamometerTemperatureCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDynamometerTemperatureCheckDlg::IDD, pParent)
	, m_pDynThread(NULL)
	, m_bRefreshDataSign(true)
{
	// 启动转速计线程
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

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


// CDynamometerTemperatureCheckDlg 消息处理程序

BOOL CDynamometerTemperatureCheckDlg::OnInitDialog()
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

void CDynamometerTemperatureCheckDlg::OnPaint()
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

void CDynamometerTemperatureCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机温度检查");

	m_lbInfo.SetText(L"正在通讯...");
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		////////////////////////////////////////////////////////// 从配置文件获取南华测功机通讯端口begin

		CString strPath;
		// 获取exe(dll)文件绝对路径
		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
		strPath.ReleaseBuffer();
		// 向上2层
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		// 配置文件路径
		strPath = strPath + L"\\Config\\Equipment.ini";

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
				m_lbInfo.SetText(_T("测功机温度实时检测数据"));
				// 刷新温度
				SetTimer(2, 200, NULL);
			}
			else
			{
				m_lbInfo.SetText(_T("通讯超时"));
			}
		}
		else
		{
			m_lbInfo.SetText(_T("打开通讯端口失败"));
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
	// TODO: 在此添加控件通知处理程序代码

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

