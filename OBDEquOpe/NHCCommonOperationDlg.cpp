// NHCCommonOperationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCCommonOperationDlg.h"

//#include "bitset"

// CNHCCommonOperationDlg 对话框

IMPLEMENT_DYNAMIC(CNHCCommonOperationDlg, CDialogZoom)

CNHCCommonOperationDlg::CNHCCommonOperationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCCommonOperationDlg::IDD, pParent)
	, m_nDlgFontSize(45)
{
	// 启动测功机线程
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
}

CNHCCommonOperationDlg::~CNHCCommonOperationDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCCommonOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCCommonOperationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCCommonOperationDlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCCommonOperationDlg::OnBnClickedButtonMotorOff)
	ON_BN_CLICKED(IDCANCEL, &CNHCCommonOperationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_STATE, &CNHCCommonOperationDlg::OnBnClickedButtonViewState)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHCCommonOperationDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CNHCCommonOperationDlg 消息处理程序

BOOL CNHCCommonOperationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrls();

	// 延时打开串口
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCCommonOperationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCCommonOperationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbInfo.SetText(L"正在通讯...");
	m_lbInfo.SetTitleStyle();

	m_shdTitle.SetTitle(L"测功机常用操作");

	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(FALSE);
}

void CNHCCommonOperationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCCommonOperationDlg::OpenSerialPortAndTestCommunication(void)
{
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
		// 用取状态进行通讯测试
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus())
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("测功机常用操作"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(FALSE);
	}
}

void CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 检查滚筒是否有转动
	DWORD dw = m_pDynThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->GetVValue() > 0.05f)
		{
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,暂时不能升起举升器", L"升举升器", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"通讯超时", L"升举升器", MB_OK|MB_ICONERROR);
		return;
	}

	m_pDynThread->LiftBeamUp();

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	m_pDynThread->LiftBeamDown();

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonMotorOn()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 检查滚筒是否有转动
	DWORD dw = m_pDynThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->GetVValue() > 0.05f)
		{
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"通讯超时", L"关电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 检查举升器状态
	//std::bitset<16> bs;
	dw = m_pDynThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->IsTheStatusBitSignaled(0))
		{
			// 举升器已经升起

			EndWaitCursor();
			MessageBox(L"请降低举升器后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
		else if (!(m_pDynThread->IsTheStatusBitSignaled(0)) && !(m_pDynThread->IsTheStatusBitSignaled(1)))
		{
			// 举升器正在降下或者正在升起(到顶和到底信号都没有被触发)

			// 发送降举升器命令
			m_pDynThread->LiftBeamDown();
			EndWaitCursor();
			MessageBox(L"请等举升器完全降下后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"通讯超时", L"开电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 进行安全提示
	if (IDYES == MessageBox(L"滚筒即将转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"开电机", MB_YESNO|MB_ICONWARNING))
	{
		m_pDynThread->LiftBeamDown();
		m_pDynThread->MotorOn();
	}
	else
	{
		MessageBox(L"开电机被取消", L"开电机", MB_OK|MB_ICONWARNING);
	}

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonMotorOff()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	m_pDynThread->MotorOff();

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CNHCCommonOperationDlg::OnBnClickedButtonViewState()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 检查举升器状态
	std::bitset<16> bs;
	if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bs))
	{
		EndWaitCursor();
		CString str(L"");
		str.Format(L"左边高位右边低位:\n\n%S\n----------------\nFEDCBA9876543210\n", bs.to_string().c_str());
		MessageBox(str, L"查看状态", MB_OK);
		return;
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"通讯超时", L"查看状态", MB_OK|MB_ICONERROR);
		return;
	}

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}
