// NHCPLHPGB2018Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCPLHPGB2018Dlg.h"
#include <cmath>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// 显示剩磁相关数据
//#ifndef SHOW_REMANENCE
//#define SHOW_REMANENCE
//#endif

// CNHCPLHPGB2018Dlg 对话框

IMPLEMENT_DYNAMIC(CNHCPLHPGB2018Dlg, CDialogZoom)

CNHCPLHPGB2018Dlg::CNHCPLHPGB2018Dlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCPLHPGB2018Dlg::IDD, pParent)
	//, m_bSerialPort(0)
	, m_usDIW(0)

	, m_dApWithRemanence(0.0)
	, m_dBpWithRemanence(0.0)
	, m_dCpWithRemanence(0.0)
	, m_dApWithoutRemanence(0.0)
	, m_dBpWithoutRemanence(0.0)
	, m_dCpWithoutRemanence(0.0)

	, m_fApWithRemanence(0.0)
	, m_fBpWithRemanence(0.0)
	, m_fCpWithRemanence(0.0)
	, m_fApWithoutRemanence(0.0)
	, m_fBpWithoutRemanence(0.0)
	, m_fCpWithoutRemanence(0.0)

	, m_pmUsed(PLHP_F_MODE)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_pfProcessCtrl(NULL)
	, m_bWriteTimer(false)
	, m_odtStart(COleDateTime::GetCurrentTime())
{
	// 启动测功机控制线程
	m_pNHCPLHPThread = (CNHCPLHPGB2018Thread*)AfxBeginThread(RUNTIME_CLASS(CNHCPLHPGB2018Thread));

    ZeroMemory(m_fNomialVelocity, sizeof(m_fNomialVelocity));
    ZeroMemory(m_fPLHP, sizeof(m_fPLHP));
    ZeroMemory(m_fUpperVelocity, sizeof(m_fUpperVelocity));
    ZeroMemory(m_fLowerVelocity, sizeof(m_fLowerVelocity));

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 20 / 1024;
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

CNHCPLHPGB2018Dlg::~CNHCPLHPGB2018Dlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pNHCPLHPThread)
	{
		m_pNHCPLHPThread->Close();
		m_pNHCPLHPThread->QuitThread();
		WaitForSingleObject(m_pNHCPLHPThread->m_hThread, 5000);
	}
}

void CNHCPLHPGB2018Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_ASM_RANGE, m_rdoAsmRange);
	DDX_Control(pDX, IDC_RADIO_VMAS_RANGE, m_rdoVmasRange);
	DDX_Control(pDX, IDC_RADIO_LUGDOWN_RANGE, m_rdoLugdownRange);
	DDX_Control(pDX, IDC_RADIO_LUGDOWN_RANGE_100, m_rdoLugdownRange_100);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lcResult);
	DDX_Control(pDX, IDC_STATIC_CURVE, m_stcPLHPCurve);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_DIW, m_lbDIW);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lb4);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_F, m_lbF);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_AP, m_lbAp);
	DDX_Control(pDX, IDC_STATIC_BP, m_lbBp);
	DDX_Control(pDX, IDC_STATIC_CP, m_lbCp);
	DDX_Control(pDX, IDC_STATIC_ANOTHER_AP, m_lbAnotherAp);
	DDX_Control(pDX, IDC_STATIC_ANOTHER_BP, m_lbAnotherBp);
	DDX_Control(pDX, IDC_STATIC_ANOTHER_CP, m_lbAnotherCp);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHCPLHPGB2018Dlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCPLHPGB2018Dlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCPLHPGB2018Dlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCPLHPGB2018Dlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCPLHPGB2018Dlg::OnBnClickedButtonMotorOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CNHCPLHPGB2018Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START_TEST, &CNHCPLHPGB2018Dlg::OnBnClickedButtonStartTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCPLHPGB2018Dlg::OnBnClickedButtonStopTest)
	ON_MESSAGE(WM_DYN_PLHP_TEST_FINISHED, OnPLHPTestFinished)
	ON_MESSAGE(WM_DYN_PLHP_SECTION_FINISHED, OnPLHPSectionFinished)
	ON_BN_CLICKED(IDC_RADIO_ASM_RANGE, &CNHCPLHPGB2018Dlg::OnBnClickedRadioAsmRange)
	ON_BN_CLICKED(IDC_RADIO_VMAS_RANGE, &CNHCPLHPGB2018Dlg::OnBnClickedRadioVmasRange)
	ON_BN_CLICKED(IDC_RADIO_LUGDOWN_RANGE, &CNHCPLHPGB2018Dlg::OnBnClickedRadioLugdownRange)
	ON_BN_CLICKED(IDC_RADIO_LUGDOWN_RANGE_100, &CNHCPLHPGB2018Dlg::OnBnClickedRadioLugdownRange100)
END_MESSAGE_MAP()


// CNHCPLHPGB2018Dlg 消息处理程序

BOOL CNHCPLHPGB2018Dlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	// 打开串口，并进行通讯测试
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCPLHPGB2018Dlg::InitCtrls(void)
{
	// 设置对话框字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机寄生功率滑行");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	EnableBasicOperation(false);

	// 设置标签字体颜色
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lb4.SetTextColor(RGB(0, 102, 204));

	// 设置基本惯量显示框风格
	m_lbDIW.SetBkColor(RGB(230, 230, 230));

	// 设置Ap,Bp,Cp显示框风格
	m_lbAp.SetBkColor(RGB(230, 230, 230));
	m_lbAp.SetTextColor(RGB(0, 102, 204));
	m_lbBp.SetBkColor(RGB(230, 230, 230));
	m_lbBp.SetTextColor(RGB(0, 102, 204));
	m_lbCp.SetBkColor(RGB(230, 230, 230));
	m_lbCp.SetTextColor(RGB(0, 102, 204));

	// 设置另一Ap,Bp,Cp显示框风格
	m_lbAnotherAp.SetBkColor(RGB(230, 230, 230));
	m_lbAnotherAp.SetTextColor(RGB(0, 204, 102));
	m_lbAnotherBp.SetBkColor(RGB(230, 230, 230));
	m_lbAnotherBp.SetTextColor(RGB(0, 204, 102));
	m_lbAnotherCp.SetBkColor(RGB(230, 230, 230));
	m_lbAnotherCp.SetTextColor(RGB(0, 204, 102));

#ifndef SHOW_REMANENCE
	m_lbAnotherAp.ShowWindow(SW_HIDE);
	m_lbAnotherBp.ShowWindow(SW_HIDE);
	m_lbAnotherCp.ShowWindow(SW_HIDE);
#endif

	// 设置功率、速度显示框风格
	m_lbF.SetLEDStyle();
	m_lbP.SetLEDStyle();
	m_lbV.SetLEDStyle();
	
	m_lcResult.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//m_lcResult.SetBkColor(RGB(230, 234, 243));
	//m_lcResult.SetTextBkColor(RGB(230, 234, 243));
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcResult.InsertColumn(0, L"速度区间[km/h]",	LVCFMT_LEFT,	170*nSM_CXSCREEN/1280, 0);
	m_lcResult.InsertColumn(1, L"名义速度[km/h]",	LVCFMT_LEFT,	170*nSM_CXSCREEN/1280, 1);
	m_lcResult.InsertColumn(2, L"滑行时间[s]",		LVCFMT_LEFT,	150*nSM_CXSCREEN/1280, 2);
	m_lcResult.InsertColumn(3, L"寄生功率[kW]",		LVCFMT_LEFT,	150*nSM_CXSCREEN/1280, 3);
#ifdef SHOW_REMANENCE
	m_lcResult.InsertColumn(4, L"剩磁[kW]",			LVCFMT_LEFT,	120*nSM_CXSCREEN/1280, 4);
	m_lcResult.InsertColumn(5, L"剩磁[N]",			LVCFMT_LEFT,	100*nSM_CXSCREEN/1280, 5);
#endif

	// 初始化滑行区间ASM区间
	m_rdoAsmRange.SetCheck(BST_CHECKED);
	OnBnClickedRadioAsmRange();
}

void CNHCPLHPGB2018Dlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCPLHPGB2018Dlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pNHCPLHPThread->Open(bPort, imDyn))
	{
		// 进行通讯测试
		// 用取基本惯量进行通讯测试
		if (DYN_WR_SUCCEED == m_pNHCPLHPThread->GetDIW(&m_usDIW))
		{
			// 显示基本惯量
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);
			// 获取寄生功率参数,有剩磁
			m_pNHCPLHPThread->ReadApWithRemanence(&m_fApWithRemanence);       //读取Ap
			m_pNHCPLHPThread->ReadBpWithRemanence(&m_fBpWithRemanence);       //读取Bp
			m_pNHCPLHPThread->ReadCpWithRemanence(&m_fCpWithRemanence);       //读取Cp
			// 获取寄生功率参数,无剩磁
			m_pNHCPLHPThread->ReadApWithoutRemanence(&m_fApWithoutRemanence);       //读取Ap
			m_pNHCPLHPThread->ReadBpWithoutRemanence(&m_fBpWithoutRemanence);       //读取Bp
			m_pNHCPLHPThread->ReadCpWithoutRemanence(&m_fCpWithoutRemanence);       //读取Cp

			// 转化ApBpCp
			ConvertApBpCp(false);

			// 显示ApBpCp
			ShowtApBpCp();

			// 显示曲线
			ShowPLHPCurve(true);
			// 显示真实曲线
			ShowAnotherPLHPCurve(true);

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("测功机寄生功率滑行"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
			EnableBasicOperation(true);
			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			EnableBasicOperation(false);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		EnableBasicOperation(false);
	}
}

void CNHCPLHPGB2018Dlg::ShowtApBpCp(void)
{
	CString str;

	// 显示有剩磁参数
	str.Format(L"%.2f", m_fApWithRemanence);
	m_lbAp.SetText(str);
	str.Format(L"%.2f", m_fBpWithRemanence);
	m_lbBp.SetText(str);
	str.Format(L"%.2f", m_fCpWithRemanence);
	m_lbCp.SetText(str);

	// 显示无剩磁参数
	str.Format(L"%.2f", m_fApWithoutRemanence);
	m_lbAnotherAp.SetText(str);
	str.Format(L"%.2f", m_fBpWithoutRemanence);
	m_lbAnotherBp.SetText(str);
	str.Format(L"%.2f", m_fCpWithoutRemanence);
	m_lbAnotherCp.SetText(str);
}

void CNHCPLHPGB2018Dlg::EnableRefreshRealTimeData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(2, 500, NULL);
	}
	else
	{
		KillTimer(2);
	}
}

void CNHCPLHPGB2018Dlg::RefreshRealTimeData(void)
{
	// 刷新主数据
	// 同时进行通讯测试
	// 500毫秒执行一次

	CString str(L"");

	//if (DYN_WR_SUCCEED == m_pNHCPLHPThread->GetRealTimeData_Asyn())
	m_pNHCPLHPThread->GetRealTimeData_Asyn();
	{
		// 通讯成功
		str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
		GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
		GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
	}
	//else
	//{
	//	// 通讯失败
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCPLHPGB2018Dlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(bEnable);
}

void CNHCPLHPGB2018Dlg::RestoreCtrls(void)
{
	m_rdoAsmRange.EnableWindow(TRUE);
	m_rdoVmasRange.EnableWindow(TRUE);
	m_rdoLugdownRange.EnableWindow(TRUE);
	m_rdoLugdownRange_100.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CNHCPLHPGB2018Dlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: 在此添加控件通知处理程序代码
	
	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 检查滚筒是否有转动
	DWORD dw = m_pNHCPLHPThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->GetVValue() > 10e-6)
		{
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,暂时不能升起举升器", L"升举升器", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"升举升器", MB_OK|MB_ICONERROR);
		return;
	}

	m_pNHCPLHPThread->LiftBeamUp();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPGB2018Dlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: 在此添加控件通知处理程序代码
	
	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	m_pNHCPLHPThread->LiftBeamDown();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPGB2018Dlg::OnBnClickedButtonMotorOn()
{
	// TODO: 在此添加控件通知处理程序代码
	
	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 检查滚筒是否有转动
	DWORD dw = m_pNHCPLHPThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->GetVValue() > 0.05f)
		{
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"开电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 发送降举升器命令
	m_pNHCPLHPThread->LiftBeamDown();

	// 检查举升器状态
	dw = m_pNHCPLHPThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->IsTheStatusBitSignaled(0))
		{
			// 举升器已经升起

			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"请降低举升器后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
		else if (!(m_pNHCPLHPThread->IsTheStatusBitSignaled(0)) && !(m_pNHCPLHPThread->IsTheStatusBitSignaled(1)))
		{
			// 举升器正在降下或者正在升起

			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"请等举升器完全降下后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"开电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 进行安全提示
	if (IDYES == MessageBox(L"滚筒即将转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"开电机", MB_YESNO|MB_ICONWARNING))
	{
		m_pNHCPLHPThread->MotorOn();
	}
	else
	{
		MessageBox(L"开电机被取消", L"开电机", MB_OK|MB_ICONWARNING);
	}

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPGB2018Dlg::OnBnClickedButtonMotorOff()
{
	// TODO: 在此添加控件通知处理程序代码
	
	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	m_pNHCPLHPThread->MotorOff();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPGB2018Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (1 == nIDEvent)
	{
		// 打开串口，并测试通讯

		KillTimer(1);
		OpenSerialPortAndTestCommunication();
		//TRACE("OnTimer 1\n");
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
		//TRACE("OnTimer 2\n");
	}
	else if (3 == nIDEvent)
	{
		DisplayPLHPTestProcess();
		//TRACE("OnTimer 3\n");
	}

	CDialogZoom::OnTimer(nIDEvent);
}

// 显示寄生功率测试流程
void CNHCPLHPGB2018Dlg::DisplayPLHPTestProcess(void)
{
	switch (m_pNHCPLHPThread->GetPLHPState())
	{
	case NHC_PLHP_STATE_NOT_START:
		{
		}
		break;
	case NHC_PLHP_STATE_READY:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在初始化测试参数...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
		}
		break;
	case NHC_PLHP_STATE_LIFTBEAM_DOWN:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在降举升器...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
		}
		break;
	case NHC_PLHP_STATE_ACCELERATION:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在加速...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
		}
		break;
	case NHC_PLHP_STATE_SLIP:
		{
			if(m_bWriteTimer)
			{
				m_odtStart = COleDateTime::GetCurrentTime();
				m_bWriteTimer = false;
			}
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在滑行...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// 显示寄生功率滑行时间
			DisplayPLHPSldingTime();
		}
		break;
	case NHC_PLHP_STATE_FINISHED:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试完成");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);

			// 停止刷新寄生功率滑行流程
			KillTimer(3);

			// 以下设置改由接收测试结束消息的函数内处理,即是:OnPLHPTestFinished
			//// 测试全部完成
			//// 计算寄生功率滑行结果
			//CalculatePLHPParams();
			//// 显示寄生功率曲线
			//ShowPLHPCurve();
			//// 写入寄生功率滑行参数到测功机
			//// 恢复控件
			//RestoreCtrls();
			//// 开始刷新数据
			//EnableRefreshRealTimeData(true);
		}
		break;
	case NHC_PLHP_STATE_ERROR:
		{
			if (NHC_PLHP_COMMUNICATION_ERROR == m_pNHCPLHPThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试终止,与测功机通讯超时");
			}
			else if (NHC_PLHP_START_MMTIMER_FAIL == m_pNHCPLHPThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试终止,启动定时器失败");
			}
			//GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			//GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			//GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			// 停止刷新加载滑行流程
			KillTimer(3);
			// 恢复控件
			RestoreCtrls();
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
		}
		break;
	default:
		break;
	}
}

void CNHCPLHPGB2018Dlg::DisplayPLHPSldingTime(void)
{
	// 要注意,单位ms－>s

	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM寄生功率
		DisplayPLHPSldingTime_ASM();
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS寄生功率
		DisplayPLHPSldingTime_VMAS();
	}
	else
	{
		// LUGDOWN寄生功率
		DisplayPLHPSldingTime_LUGDOWN();
	}
}

void CNHCPLHPGB2018Dlg::DisplayPLHPSldingTime_ASM(void)
{
	CString str(L"");
	if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[0])
	{
		// 刷新第1段滑行时间(65~55)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[1])
	{
		// 刷新第2段滑行时间(55~45)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[2])
	{
		// 刷新第3段滑行时间(45~35)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[3])
	{
		// 刷新第4段滑行时间(35~25)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[4])
	{
		// 刷新第5段滑行时间(25~15)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[5])
	{
		// 刷新第6段滑行时间(15~5)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(5)/1000.0f);
		m_lcResult.SetItemText(5, 2, str);
	}
}

void CNHCPLHPGB2018Dlg::DisplayPLHPSldingTime_VMAS(void)
{
	CString str(L"");
	if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[0])
	{
		// 刷新第1段滑行时间(65~55)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[1])
	{
		// 刷新第2段滑行时间(55~45)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[2])
	{
		// 刷新第3段滑行时间(45~35)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[3])
	{
		// 刷新第4段滑行时间(35~25)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[4])
	{
		// 刷新第5段滑行时间(25~15)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[5])
	{
		// 刷新第6段滑行时间(15~5)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(5)/1000.0f);
		m_lcResult.SetItemText(5, 2, str);
	}
}

void CNHCPLHPGB2018Dlg::DisplayPLHPSldingTime_LUGDOWN(void)
{
	CString str(L"");
	if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[0])
	{
		// 刷新第1段滑行时间(80~60)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[1])
	{
		// 刷新第2段滑行时间(70~50)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[2])
	{
		// 刷新第3段滑行时间(60~40)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[3])
	{
		// 刷新第4段滑行时间(50~20)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[4])
	{
		// 刷新第5段滑行时间(40~20)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[5])
	{
		// 刷新第6段滑行时间(30~10)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(5)/1000.0f);
		m_lcResult.SetItemText(5, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[6])
	{
		// 刷新第7段滑行时间(25~15)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(6)/1000.0f);
		m_lcResult.SetItemText(6, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue() >= m_fLowerVelocity[7])
	{
		// 刷新第8段滑行时间(15~5)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(7)/1000.0f);
		m_lcResult.SetItemText(7, 2, str);
	}
}

void CNHCPLHPGB2018Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	
	EnableRefreshRealTimeData(false);

	OnCancel();
}

void CNHCPLHPGB2018Dlg::OnBnClickedButtonStartTest()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	CString str(L"");

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 检查滚筒是否有转动
	DWORD dw = m_pNHCPLHPThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->GetVValue() > 0.1f)
		{
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开始测试", L"寄生功率", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"寄生功率", MB_OK|MB_ICONERROR);
		return;
	}

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机寄生功率滑行开始", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_PLHP_START, wchInfo, sizeof(wchInfo));
	}

	// 安全提示，测功机滚筒将要转动
	if (IDYES == MessageBox(L"测试过程滚筒将会高速转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"寄生功率", MB_YESNO|MB_ICONWARNING))
	{
		m_rdoAsmRange.EnableWindow(FALSE);
		m_rdoVmasRange.EnableWindow(FALSE);
		m_rdoLugdownRange.EnableWindow(FALSE);
		m_rdoLugdownRange_100.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// 禁止基本操作
		EnableBasicOperation(false);
		// 清除检测结果
		ClearPLHPResult();
		// 禁止退出测试界面
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		// 开始刷新检测过程
		m_bWriteTimer = true;
		SetTimer(3, 200, NULL);
		// 寄生功率类型
		CNHCPLHPGB2018Thread::PLHPType pt;
		if (BST_CHECKED == m_rdoAsmRange.GetCheck())
		{
			// ASM寄生功率
			pt = CNHCPLHPGB2018Thread::PLHP_ASM;
		}
		else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
		{
			// VMAS寄生功率
			pt = CNHCPLHPGB2018Thread::PLHP_VMAS;
		}
		else if(BST_CHECKED == m_rdoLugdownRange.GetCheck())
		{
			// LUGDOWN寄生功率
			pt = CNHCPLHPGB2018Thread::PLHP_LUGDOWN;
		}
		else
		{		
			// LUGDOWN寄生功率
			pt = CNHCPLHPGB2018Thread::PLHP_LUGDOWN_100;
		}
		m_pNHCPLHPThread->StartPLHPTest_Asyn(pt, m_hWnd);
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		MessageBox(L"测试被取消", L"寄生功率", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
}

void CNHCPLHPGB2018Dlg::OnBnClickedButtonStopTest()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试被用户停止");

	KillTimer(3);

	m_pNHCPLHPThread->StopPLHPTest();

	RestoreCtrls();
	m_bWriteTimer = false;
	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机寄生功率滑行停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_PLHP_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHCPLHPGB2018Dlg::Polynom_berechnen(double *const pdXArray, double *const pdYArray, const int nAmount, double *const pdResult, const int grad)
{
	ASSERT(NULL != pdXArray);
	ASSERT(NULL != pdYArray);
	ASSERT(NULL != pdResult);
	ASSERT(nAmount> 0);

	register int i(0), j(0), k(0);
	double fakt(0.0);
	double hv(0.0);
	double alpha[5][6];
	double sum_x[6];
	ZeroMemory(alpha, sizeof(alpha));
	ZeroMemory(sum_x, sizeof(sum_x));

	// pdResult initialisieren
	for (i=0; i<=grad; i++)
	{
		pdResult[i] = 0.0;
	}

	/* Berechnung der rechten Seiten der Gleichungen */
	for (i = 0; i <= (grad + 1); i++)
	{
		for (j = 0; j <= grad+1; j++)
			alpha[i][j] = 0;
	}

	for (j = 0; j <= grad; j++)
	{
		for (i = 0; i < nAmount; i++)
		{
			hv = pdYArray[i];

			if (j > 0)
				hv *= pow (pdXArray[i], j);

			alpha[j][grad+1] += hv;
		}
	}

	/* Berechnung der pdResult-Matrix */
	alpha[0][0] = nAmount;

	for (i = 0; i < (2 * grad); i++)
		sum_x[i] = 0;

	for (i = 0; i < nAmount; i++)
	{
		for (j = 0; j < (2 * grad); j++)
			sum_x[j] += pow (pdXArray[i], j+1);
	}

	for (i = 0; i <= grad; i++)
	{
		for (j = 0; j<= grad; j++)
		{
			if (i == 0 && j == 0)
				continue;

			alpha[i][j] = sum_x[i+j-1];
		}
	}

	/* Gau?scher Algorithmus */
	for (k = 0; k <= grad; k++)
	{
		for (i = 0; i <= grad; i++)
		{
			if (i == k)
				continue;

			fakt = alpha[i][k];
			for (j = k; j <= (grad + 1); j++)
			{
				alpha[i][j] = (- fakt / alpha[k][k])
					* alpha[k][j] + alpha[i][j];
			}
		}
	}

	/* pdResult berechnen */
	for (i = 0; i <= grad; i++)
		pdResult[i] = alpha[i][grad+1] / alpha[i][i];
}

float CNHCPLHPGB2018Dlg::CalculatePLHP(const float fUpperVelocity, const float fLowerVelocity, const UINT unSlidingTime)
{
	ASSERT(fUpperVelocity > 0.0f);
	ASSERT(fLowerVelocity > 0.0f);
	ASSERT(fUpperVelocity >= fLowerVelocity);
	ASSERT(unSlidingTime > 0);

	float f = m_usDIW * (fUpperVelocity*fUpperVelocity - fLowerVelocity*fLowerVelocity) / (2.0f * unSlidingTime * 3.6f * 3.6f);
	ASSERT(f > 0.0f);
	return (f * 100.0f + 0.5f) / 100.0f;
}

LRESULT CNHCPLHPGB2018Dlg::OnPLHPTestFinished(WPARAM wParam, LPARAM lParam)
{
	// 设置操作完成标志
	m_bIsOperationFinishedAtLeaseOnce = true;

	///////////////////////////测试全部完成，进行测试数据处理begin
	// 计算寄生功率滑行结果
	CalculatePLHPParams();
	// 显示寄生功率曲线
	ShowPLHPCurve(true);
	// 显示真实寄生功率曲线
	ShowAnotherPLHPCurve(true);
	// 显示ApBpCp
	ShowtApBpCp();
	// 写入寄生功率滑行参数到测功机
	WritePLHPParamsToDyn();

	//ExportEquCalChkInfo();	// 导出寄生功率测试结果
	//ExportDemarcationLog();	// 导出寄生功率测试记录
	ExportDynPLHPResult();	// 导出寄生功率测试结果

	// 恢复控件
	RestoreCtrls();
	// 开始刷新数据
	EnableRefreshRealTimeData(true);
	///////////////////////////测试全部完成，进行测试数据处理end
	
	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机寄生功率滑行完成", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_PLHP_FINISH, wchInfo, sizeof(wchInfo));
	}

	return 0;
}

LRESULT CNHCPLHPGB2018Dlg::OnPLHPSectionFinished(WPARAM wParam, LPARAM lParam)
{
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM寄生功率
		PLHPSectionFinished_ASM((UINT)wParam);
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS寄生功率
		PLHPSectionFinished_VMAS((UINT)wParam);
	}
	else
	{
		// LUGDOWN寄生功率
		PLHPSectionFinished_LUGDOWN((UINT)wParam);
	}
	return 0;
}

void CNHCPLHPGB2018Dlg::PLHPSectionFinished_ASM(UINT unSection)
{
	CString str;
	// 显示滑行时间
	str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
	m_lcResult.SetItemText(unSection, 2, str);
	// 显示寄生功率
	m_fPLHP[unSection] = CalculatePLHP(m_fUpperVelocity[unSection], m_fLowerVelocity[unSection], m_pNHCPLHPThread->GetPLHPTime(unSection));
	str.Format(L"%.2f",m_fPLHP[unSection]);
	m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
	// 显示剩磁
	str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
	m_lcResult.SetItemText(unSection, 4, str);
	str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
	m_lcResult.SetItemText(unSection, 5, str);
#endif

}

void CNHCPLHPGB2018Dlg::PLHPSectionFinished_VMAS(UINT unSection)
{
	CString str;
	// 显示滑行时间
	str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
	m_lcResult.SetItemText(unSection, 2, str);
	// 显示寄生功率
	m_fPLHP[unSection] = CalculatePLHP(m_fUpperVelocity[unSection], m_fLowerVelocity[unSection], m_pNHCPLHPThread->GetPLHPTime(unSection));
	str.Format(L"%.2f",m_fPLHP[unSection]);
	m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
	// 显示剩磁
	str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
	m_lcResult.SetItemText(unSection, 4, str);
	str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
	m_lcResult.SetItemText(unSection, 5, str);
#endif

}

void CNHCPLHPGB2018Dlg::PLHPSectionFinished_LUGDOWN(UINT unSection)
{
	CString str;
	// 显示滑行时间
	str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
	m_lcResult.SetItemText(unSection, 2, str);
	// 显示寄生功率
	m_fPLHP[unSection] = CalculatePLHP(m_fUpperVelocity[unSection], m_fLowerVelocity[unSection], m_pNHCPLHPThread->GetPLHPTime(unSection));
	str.Format(L"%.2f",m_fPLHP[unSection]);
	m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
	// 显示剩磁
	str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
	m_lcResult.SetItemText(unSection, 4, str);
	str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
	m_lcResult.SetItemText(unSection, 5, str);
#endif

}

void CNHCPLHPGB2018Dlg::OnBnClickedRadioAsmRange()
{	
	// 高速点
	m_fUpperVelocity[0] = 60.0f;
	m_fUpperVelocity[1] = 50.0f;
	m_fUpperVelocity[2] = 35.0f;
	m_fUpperVelocity[3] = 22.0f;
	m_fUpperVelocity[4] = 0.0f;
	m_fUpperVelocity[5] = 0.0f;
	m_fUpperVelocity[6] = 0.0f;
	m_fUpperVelocity[7] = 0.0f;
	m_fUpperVelocity[8] = 0.0f;

	// 低速点
	m_fLowerVelocity[0] = 40.0f;
	m_fLowerVelocity[1] = 30.0f;
	m_fLowerVelocity[2] = 15.0f;
	m_fLowerVelocity[3] = 8.0f;
	m_fLowerVelocity[4] = 0.0f;
	m_fLowerVelocity[5] = 0.0f;
	m_fLowerVelocity[6] = 0.0f;
	m_fLowerVelocity[7] = 0.0f;
	m_fLowerVelocity[8] = 0.0f;

	// 名义速度
	m_fNomialVelocity[0] = (m_fUpperVelocity[0]+m_fLowerVelocity[0])/2;
	m_fNomialVelocity[1] = (m_fUpperVelocity[1]+m_fLowerVelocity[1])/2;
	m_fNomialVelocity[2] = (m_fUpperVelocity[2]+m_fLowerVelocity[2])/2;
	m_fNomialVelocity[3] = (m_fUpperVelocity[3]+m_fLowerVelocity[3])/2;
	m_fNomialVelocity[4] = (m_fUpperVelocity[4]+m_fLowerVelocity[4])/2;
	m_fNomialVelocity[5] = (m_fUpperVelocity[5]+m_fLowerVelocity[5])/2;
	m_fNomialVelocity[6] = (m_fUpperVelocity[6]+m_fLowerVelocity[6])/2;
	m_fNomialVelocity[7] = (m_fUpperVelocity[7]+m_fLowerVelocity[7])/2;
	m_fNomialVelocity[8] = (m_fUpperVelocity[8]+m_fLowerVelocity[8])/2;

	m_lcResult.DeleteAllItems();
	
	for (int i=0; i<4; i++)
	{
		CString strTemp;
		strTemp.Format(L"%.1f~%.1f", m_fUpperVelocity[i], m_fLowerVelocity[i]);
		m_lcResult.InsertItem(i, strTemp);
		strTemp.Format(L"%.1f", m_fNomialVelocity[i]);
		m_lcResult.SetItemText(i, 1, strTemp);
	}
}

void CNHCPLHPGB2018Dlg::OnBnClickedRadioVmasRange()
{
	
	// 高速点
	m_fUpperVelocity[0] = 60.0f;
	m_fUpperVelocity[1] = 50.0f;
	m_fUpperVelocity[2] = 35.0f;
	m_fUpperVelocity[3] = 20.0f;
	m_fUpperVelocity[4] = 0.0f;
	m_fUpperVelocity[5] = 0.0f;
	m_fUpperVelocity[6] = 0.0f;
	m_fUpperVelocity[7] = 0.0f;
	m_fUpperVelocity[8] = 0.0f;

	// 低速点
	m_fLowerVelocity[0] = 40.0f;
	m_fLowerVelocity[1] = 30.0f;
	m_fLowerVelocity[2] = 15.0f;
	m_fLowerVelocity[3] = 10.0f;
	m_fLowerVelocity[4] = 0.0f;
	m_fLowerVelocity[5] = 0.0f;
	m_fLowerVelocity[6] = 0.0f;
	m_fLowerVelocity[7] = 0.0f;
	m_fLowerVelocity[8] = 0.0f;

	// 名义速度
	m_fNomialVelocity[0] = (m_fUpperVelocity[0]+m_fLowerVelocity[0])/2;
	m_fNomialVelocity[1] = (m_fUpperVelocity[1]+m_fLowerVelocity[1])/2;
	m_fNomialVelocity[2] = (m_fUpperVelocity[2]+m_fLowerVelocity[2])/2;
	m_fNomialVelocity[3] = (m_fUpperVelocity[3]+m_fLowerVelocity[3])/2;
	m_fNomialVelocity[4] = (m_fUpperVelocity[4]+m_fLowerVelocity[4])/2;
	m_fNomialVelocity[5] = (m_fUpperVelocity[5]+m_fLowerVelocity[5])/2;
	m_fNomialVelocity[6] = (m_fUpperVelocity[6]+m_fLowerVelocity[6])/2;
	m_fNomialVelocity[7] = (m_fUpperVelocity[7]+m_fLowerVelocity[7])/2;
	m_fNomialVelocity[8] = (m_fUpperVelocity[8]+m_fLowerVelocity[8])/2;

	m_lcResult.DeleteAllItems();
	
	for (int i=0; i<4; i++)
	{
		CString strTemp;
		strTemp.Format(L"%.1f~%.1f", m_fUpperVelocity[i], m_fLowerVelocity[i]);
		m_lcResult.InsertItem(i, strTemp);
		strTemp.Format(L"%.1f", m_fNomialVelocity[i]);
		m_lcResult.SetItemText(i, 1, strTemp);
	}
}

void CNHCPLHPGB2018Dlg::OnBnClickedRadioLugdownRange()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// 高速点
	m_fUpperVelocity[0] = 80.0f;
	m_fUpperVelocity[1] = 70.0f;
	m_fUpperVelocity[2] = 60.0f;
	m_fUpperVelocity[3] = 50.0f;
	m_fUpperVelocity[4] = 40.0f;
	m_fUpperVelocity[5] = 30.0f;
	m_fUpperVelocity[6] = 0.0f;
	m_fUpperVelocity[7] = 0.0f;
	m_fUpperVelocity[8] = 0.0f;

	// 低速点
	m_fLowerVelocity[0] = 60.0f;
	m_fLowerVelocity[1] = 50.0f;
	m_fLowerVelocity[2] = 40.0f;
	m_fLowerVelocity[3] = 30.0f;
	m_fLowerVelocity[4] = 20.0f;
	m_fLowerVelocity[5] = 10.0f;
	m_fLowerVelocity[6] = 0.0f;
	m_fLowerVelocity[7] = 0.0f;
	m_fLowerVelocity[8] = 0.0f;

	// 名义速度
	m_fNomialVelocity[0] = (m_fUpperVelocity[0]+m_fLowerVelocity[0])/2;
	m_fNomialVelocity[1] = (m_fUpperVelocity[1]+m_fLowerVelocity[1])/2;
	m_fNomialVelocity[2] = (m_fUpperVelocity[2]+m_fLowerVelocity[2])/2;
	m_fNomialVelocity[3] = (m_fUpperVelocity[3]+m_fLowerVelocity[3])/2;
	m_fNomialVelocity[4] = (m_fUpperVelocity[4]+m_fLowerVelocity[4])/2;
	m_fNomialVelocity[5] = (m_fUpperVelocity[5]+m_fLowerVelocity[5])/2;
	m_fNomialVelocity[6] = (m_fUpperVelocity[6]+m_fLowerVelocity[6])/2;
	m_fNomialVelocity[7] = (m_fUpperVelocity[7]+m_fLowerVelocity[7])/2;
	m_fNomialVelocity[8] = (m_fUpperVelocity[8]+m_fLowerVelocity[8])/2;

	m_lcResult.DeleteAllItems();
	
	for (int i=0; i<6; i++)
	{
		CString strTemp;
		strTemp.Format(L"%.1f~%.1f", m_fUpperVelocity[i], m_fLowerVelocity[i]);
		m_lcResult.InsertItem(i, strTemp);
		strTemp.Format(L"%.1f", m_fNomialVelocity[i]);
		m_lcResult.SetItemText(i, 1, strTemp);
	}
}

void CNHCPLHPGB2018Dlg::OnBnClickedRadioLugdownRange100()
{
	// 高速点
	m_fUpperVelocity[0] = 100.0f;
	m_fUpperVelocity[1] = 90.0f;
	m_fUpperVelocity[2] = 80.0f;
	m_fUpperVelocity[3] = 70.0f;
	m_fUpperVelocity[4] = 60.0f;
	m_fUpperVelocity[5] = 50.0f;
	m_fUpperVelocity[6] = 40.0f;
	m_fUpperVelocity[7] = 30.0f;
	m_fUpperVelocity[8] = 0.0f;

	// 低速点
	m_fLowerVelocity[0] = 80.0f;
	m_fLowerVelocity[1] = 70.0f;
	m_fLowerVelocity[2] = 60.0f;
	m_fLowerVelocity[3] = 50.0f;
	m_fLowerVelocity[4] = 40.0f;
	m_fLowerVelocity[5] = 30.0f;
	m_fLowerVelocity[6] = 20.0f;
	m_fLowerVelocity[7] = 10.0f;
	m_fLowerVelocity[8] = 0.0f;

	// 名义速度
	m_fNomialVelocity[0] = (m_fUpperVelocity[0]+m_fLowerVelocity[0])/2;
	m_fNomialVelocity[1] = (m_fUpperVelocity[1]+m_fLowerVelocity[1])/2;
	m_fNomialVelocity[2] = (m_fUpperVelocity[2]+m_fLowerVelocity[2])/2;
	m_fNomialVelocity[3] = (m_fUpperVelocity[3]+m_fLowerVelocity[3])/2;
	m_fNomialVelocity[4] = (m_fUpperVelocity[4]+m_fLowerVelocity[4])/2;
	m_fNomialVelocity[5] = (m_fUpperVelocity[5]+m_fLowerVelocity[5])/2;
	m_fNomialVelocity[6] = (m_fUpperVelocity[6]+m_fLowerVelocity[6])/2;
	m_fNomialVelocity[7] = (m_fUpperVelocity[7]+m_fLowerVelocity[7])/2;
	m_fNomialVelocity[8] = (m_fUpperVelocity[8]+m_fLowerVelocity[8])/2;

	m_lcResult.DeleteAllItems();
	
	for (int i=0; i<8; i++)
	{
		CString strTemp;
		strTemp.Format(L"%.1f~%.1f", m_fUpperVelocity[i], m_fLowerVelocity[i]);
		m_lcResult.InsertItem(i, strTemp);
		strTemp.Format(L"%.1f", m_fNomialVelocity[i]);
		m_lcResult.SetItemText(i, 1, strTemp);
	}
}


void CNHCPLHPGB2018Dlg::CalculatePLHPParams(void)
{
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM
		CalculatePLHPParams_ASM();
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS
		CalculatePLHPParams_VMAS();
	}
	else 
	{
		// LUGDOWN
		CalculatePLHPParams_LUGDOWN();
	}

	// 转化ApBpCp
	ConvertApBpCp(true);
}

void CNHCPLHPGB2018Dlg::CalculatePLHPParams_ASM(void)
{
	double *pdXArray = new double[4]();
	double *pdYArray = new double[4]();
	double *pdResult = new double[4]();

	// 第1组数据
	pdXArray[0] = 0.0;
	pdYArray[0] = 0.0;
	// 第2组数据(第3段:35~15)
	pdXArray[1] = m_fNomialVelocity[2];
	pdYArray[1] = m_fPLHP[2];
	// 第3组数据(第2段:50~30)
	pdXArray[2] = m_fNomialVelocity[1];
	pdYArray[2] = m_fPLHP[1];
	// 第4组数据(第1段:60~40)
	pdXArray[3] = m_fNomialVelocity[0];
	pdYArray[3] = m_fPLHP[0];

	Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
	m_dApWithRemanence = pdResult[1];
	m_dBpWithRemanence = pdResult[2];
	m_dCpWithRemanence = pdResult[3];

	if (PLHP_NORMAL_MODE == m_pmUsed)
	{
		// 平常模式
		m_dApWithoutRemanence = m_dApWithRemanence;
		m_dBpWithoutRemanence = m_dBpWithRemanence;
		m_dCpWithoutRemanence = m_dCpWithRemanence;
	}
	else if (PLHP_F_MODE == m_pmUsed)
	{
		// 力模式

		// 重新设置拟合数据

		// 第1组数据
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// 第2组数据(第3段:35~15)
		pdXArray[1] = m_fNomialVelocity[2];
		pdYArray[1] = m_fPLHP[2] - (m_pNHCPLHPThread->GetRemanenceAveByForce(4)*m_fNomialVelocity[2]/3.6f/1000.0f);
		// 第3组数据(第2段:50~30)
		pdXArray[2] = m_fNomialVelocity[1];
		pdYArray[2] = m_fPLHP[1] - (m_pNHCPLHPThread->GetRemanenceAveByForce(2)*m_fNomialVelocity[1]/3.6f/1000.0f);
		// 第4组数据(第1段:60~40)
		pdXArray[3] = m_fNomialVelocity[0];
		pdYArray[3] = m_fPLHP[0] - (m_pNHCPLHPThread->GetRemanenceAveByForce(0)*m_fNomialVelocity[0]/3.6f/1000.0f);
		// 拟合结果
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}
	else
	{
		// 功率模式

		// 重新设置拟合数据

		// 第1组数据
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// 第2组数据(第3段:35~15)
		pdXArray[1] = m_fNomialVelocity[2];
		pdYArray[1] = m_fPLHP[2] - m_pNHCPLHPThread->GetRemanenceAveByPower(2);
		// 第3组数据(第2段:50~30)
		pdXArray[2] = m_fNomialVelocity[1];
		pdYArray[2] = m_fPLHP[1] - m_pNHCPLHPThread->GetRemanenceAveByPower(1);
		// 第4组数据(第1段:60~40)
		pdXArray[3] = m_fNomialVelocity[0];
		pdYArray[3] = m_fPLHP[0] - m_pNHCPLHPThread->GetRemanenceAveByPower(0);
		// 拟合结果
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}

	delete[] pdXArray;
	delete[] pdYArray;
	delete[] pdResult;
}

void CNHCPLHPGB2018Dlg::CalculatePLHPParams_VMAS(void)
{
	// 和ASM一样
	CalculatePLHPParams_ASM();
}

void CNHCPLHPGB2018Dlg::CalculatePLHPParams_LUGDOWN(void)
{
	double *pdXArray = new double[4]();
	double *pdYArray = new double[4]();
	double *pdResult = new double[4]();

	// 第2组数据，用第5段数据(第5段:30~10)
	const int nArray2(4);
	// 第3组数据，用第3段数据(第3段:50~30)
	const int nArray3(2);
	// 第4组数据，用第1段数据(第1段:80~60)
	const int nArray4(0);

	// 第1组数据
	pdXArray[0] = 0.0;
	pdYArray[0] = 0.0;
	// 第2组数据(第5段:30~10)
	pdXArray[1] = m_fNomialVelocity[nArray2];
	pdYArray[1] = m_fPLHP[nArray2];
	// 第3组数据(第3段:50~30)
	pdXArray[2] = m_fNomialVelocity[nArray3];
	pdYArray[2] = m_fPLHP[nArray3];
	// 第4组数据(第1段:80~60)
	pdXArray[3] = m_fNomialVelocity[nArray4];
	pdYArray[3] = m_fPLHP[nArray4];

	Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
	m_dApWithRemanence = pdResult[1];
	m_dBpWithRemanence = pdResult[2];
	m_dCpWithRemanence = pdResult[3];

	if (PLHP_NORMAL_MODE == m_pmUsed)
	{
		// 平常模式
		m_dApWithoutRemanence = m_dApWithRemanence;
		m_dBpWithoutRemanence = m_dBpWithRemanence;
		m_dCpWithoutRemanence = m_dCpWithRemanence;
	}
	else if (PLHP_F_MODE == m_pmUsed)
	{
		// 力模式

		// 重新设置拟合数据

		// 第1组数据
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// 第2组数据(第5段:30~10)
		pdXArray[1] = m_fNomialVelocity[nArray2];
		pdYArray[1] = m_fPLHP[nArray2] - (m_pNHCPLHPThread->GetRemanenceAveByForce(nArray2)*m_fNomialVelocity[nArray2]/3.6f/1000.0f);
		// 第3组数据(第3段:50~30)
		pdXArray[2] = m_fNomialVelocity[nArray3];
		pdYArray[2] = m_fPLHP[nArray3] - (m_pNHCPLHPThread->GetRemanenceAveByForce(nArray3)*m_fNomialVelocity[nArray3]/3.6f/1000.0f);
		// 第4组数据(第1段:80~60)
		pdXArray[3] = m_fNomialVelocity[nArray4];
		pdYArray[3] = m_fPLHP[nArray4] - (m_pNHCPLHPThread->GetRemanenceAveByForce(nArray4)*m_fNomialVelocity[nArray4]/3.6f/1000.0f);
		// 拟合结果
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}
	else
	{
		// 功率模式

		// 重新设置拟合数据

		// 第1组数据
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// 第2组数据(第5段:30~10)
		pdXArray[1] = m_fNomialVelocity[nArray2];
		pdYArray[1] = m_fPLHP[nArray2] - m_pNHCPLHPThread->GetRemanenceAveByPower(nArray2);
		// 第3组数据(第3段:50~30)
		pdXArray[2] = m_fNomialVelocity[nArray3];
		pdYArray[2] = m_fPLHP[nArray3] - m_pNHCPLHPThread->GetRemanenceAveByPower(nArray3);
		// 第4组数据(第1段:80~60)
		pdXArray[3] = m_fNomialVelocity[nArray4];
		pdYArray[3] = m_fPLHP[nArray4] - m_pNHCPLHPThread->GetRemanenceAveByPower(nArray4);
		// 拟合结果
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}

	delete[] pdXArray;
	delete[] pdYArray;
	delete[] pdResult;
}

void CNHCPLHPGB2018Dlg::ConvertApBpCp(bool bConvertToPower)
{
	if (bConvertToPower)
	{
		// 转化成功率(乘以80.5)

		// 真实Ap(无剩磁)
		m_fApWithoutRemanence = (float)(m_dApWithoutRemanence*80.5);
		// 保留2位小数，并四舍五入
		if (m_fApWithoutRemanence >= 0.0f)
		{
			// 正数
			m_fApWithoutRemanence = int(m_fApWithoutRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// 负数
			m_fApWithoutRemanence = int(m_fApWithoutRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// 真实Bp(无剩磁)
		m_fBpWithoutRemanence = (float)(m_dBpWithoutRemanence*80.5*80.5);
		// 保留2位小数，并四舍五入
		if (m_fBpWithoutRemanence >= 0.0f)
		{
			// 正数
			m_fBpWithoutRemanence = (int)(m_fBpWithoutRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// 负数
			m_fBpWithoutRemanence = (int)(m_fBpWithoutRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// 真实Cp(无剩磁)
		m_fCpWithoutRemanence = (float)(m_dCpWithoutRemanence*80.5*80.5*80.5);
		// 保留2位小数，并四舍五入
		if (m_fCpWithoutRemanence >= 0.0f)
		{
			// 正数
			m_fCpWithoutRemanence = (int)(m_fCpWithoutRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// 负数
			m_fCpWithoutRemanence = (int)(m_fCpWithoutRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// 显示Ap(有剩磁)
		m_fApWithRemanence = (float)(m_dApWithRemanence*80.5);
		// 保留2位小数，并四舍五入
		if (m_fApWithRemanence >= 0.0f)
		{
			// 正数
			m_fApWithRemanence = (int)(m_fApWithRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// 负数
			m_fApWithRemanence = (int)(m_fApWithRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// 显示Bp(有剩磁)
		m_fBpWithRemanence = (float)(m_dBpWithRemanence*80.5*80.5);
		// 保留2位小数，并四舍五入
		if (m_fBpWithRemanence >= 0.0f)
		{
			// 正数
			m_fBpWithRemanence = (int)(m_fBpWithRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// 负数
			m_fBpWithRemanence = (int)(m_fBpWithRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// 显示Cp(有剩磁)
		m_fCpWithRemanence = (float)(m_dCpWithRemanence*80.5*80.5*80.5);
		// 保留2位小数，并四舍五入
		if (m_fCpWithRemanence >= 0.0f)
		{
			// 正数
			m_fCpWithRemanence = (int)(m_fCpWithRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// 负数
			m_fCpWithRemanence = (int)(m_fCpWithRemanence * 100.0f - 0.5f) / 100.0f;
		}
	}
	else
	{
		// 转化成一般参数(除以80.5)

		// 获取寄生功率参数,有剩磁
		m_dApWithRemanence = m_fApWithRemanence / 80.5;
		m_dBpWithRemanence = m_fBpWithRemanence / 80.5 / 80.5;
		m_dCpWithRemanence = m_fCpWithRemanence / 80.5 / 80.5 / 80.5;
		// 获取寄生功率参数,无剩磁
		m_dApWithoutRemanence = m_fApWithoutRemanence / 80.5;
		m_dBpWithoutRemanence = m_fBpWithoutRemanence / 80.5 / 80.5;
		m_dCpWithoutRemanence = m_fCpWithoutRemanence / 80.5 / 80.5 / 80.5;
	}
}

void CNHCPLHPGB2018Dlg::ShowPLHPCurve(bool bShow)
{
	if (bShow)
	{
		m_stcPLHPCurve.SetParams(m_dCpWithRemanence, m_dBpWithRemanence, m_dApWithRemanence);
	}
	m_stcPLHPCurve.ShowCurve(bShow);
}

void CNHCPLHPGB2018Dlg::ShowAnotherPLHPCurve(bool bShow)
{
#ifdef SHOW_REMANENCE
	if (bShow)
	{
		m_stcPLHPCurve.SetAnotherParams(m_dCpWithoutRemanence, m_dBpWithoutRemanence, m_dApWithoutRemanence);
	}
	m_stcPLHPCurve.ShowAnotherCurve(bShow);
#endif
}

void CNHCPLHPGB2018Dlg::WritePLHPParamsToDyn(void)
{
	// Ap、Bp、Cp可能出现负数情况
	// 南华测功机和马哈测功机对负数作不同处理

	//////////////////////////////////////////////////////////////////////////////////////马哈测功机特殊处理begin
	// 1)如果Cp小于0，置为0
	// 2)如果Ap和Bp同时大于零，才进行写变量操作
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
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// 转化为大写
	strDynProtocol.MakeUpper();
	if (L"MAHA" == strDynProtocol)
	{
		if (m_fCpWithoutRemanence < 0.0f)
		{
			m_fCpWithoutRemanence = 0.0f;
		}
		if (m_fApWithoutRemanence<=0.0f || m_fBpWithoutRemanence<=0.0f)
		{
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////马哈测功机特殊处理end

	// 写真实Ap(无剩磁)
	m_pNHCPLHPThread->WriteApWithoutRemanence(m_fApWithoutRemanence);
	// 写真实Bp(无剩磁)
	m_pNHCPLHPThread->WriteBpWithoutRemanence(m_fBpWithoutRemanence);
	// 写真实Cp(无剩磁)
	m_pNHCPLHPThread->WriteCpWithoutRemanence(m_fCpWithoutRemanence);

	// 写显示Ap(有剩磁)
	m_pNHCPLHPThread->WriteApWithRemanence(m_fApWithRemanence);
	// 写显示Bp(有剩磁)
	m_pNHCPLHPThread->WriteBpWithRemanence(m_fBpWithRemanence);
	// 写显示Cp(有剩磁)
	m_pNHCPLHPThread->WriteCpWithRemanence(m_fCpWithRemanence);
}

void CNHCPLHPGB2018Dlg::ClearPLHPResult(void)
{
	// 清除参数
	m_lbAp.SetText(L"--");
	m_lbBp.SetText(L"--");
	m_lbCp.SetText(L"--");
	m_lbAnotherAp.SetText(L"--");
	m_lbAnotherBp.SetText(L"--");
	m_lbAnotherCp.SetText(L"--");
	// 清除寄生功率曲线
	ShowPLHPCurve(false);
	// 清除真实寄生功率曲线
	ShowAnotherPLHPCurve(false);
	// 清除寄生功率:m_fPLHP
	ZeroMemory(m_fPLHP, sizeof(m_fPLHP));
	// 清除ListCtrl数据
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM
		OnBnClickedRadioAsmRange();
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS
		OnBnClickedRadioVmasRange();
	} 
	else if (BST_CHECKED == m_rdoLugdownRange.GetCheck())
	{
		// LUGDOWN
		OnBnClickedRadioLugdownRange();
	}
	else
	{
		// LUGDOWN
		OnBnClickedRadioLugdownRange100();
	}
}

/*
void CNHCPLHPGB2018Dlg::ExportEquCalChkInfo()
{
	// 导出检测结果到数据交换目录

	CString strPathOfDataExchange;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// 向上2层
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// 数据交换目录路径
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// 判断路径是否存在,并可以进行读写
	if (_taccess(strPathOfDataExchange, 06))
	{
		// 路径不存在

		// 创建路径
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// 创建路径失败,返回,不保存
			return;
		}
	}

	// 获取检测线编号和设备型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strDynamometerModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

	// 获取操作员
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	CString str;
	CString strResult(L"[EquCalChkInfo]");

	// 操作是否完成过至少一次
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce ? L"1" : L"0");
	strResult += str;

	// 检测线编号
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strResult += str;

	// 操作员
	str.Format(L"\r\nOperator=%s", strOperator);
	strResult += str;

	// 设备
	str.Format(L"\r\nEquipment=%s", strDynamometerModel);
	strResult += str;

	// 项目
	str.Format(L"\r\nItem=%s", L"测功机寄生功率滑行");
	strResult += str;

	// 日期
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		strData.Format(L"PLHP40:%.2f,PLHP25:%.2f", m_fPLHP[1], m_fPLHP[3]);
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		strData.Format(L"PLHP40:%.2f,PLHP24:%.2f", m_fPLHP[1], m_fPLHP[3]);
	}
	else if (BST_CHECKED == m_rdoLugdownRange.GetCheck())
	{
		strData.Format(L"PLHP80:%.2f,PLHP72:%.2f,PLHP64:%.2f,PLHP56:%.2f,PLHP48:%.2f,PLHP40:%.2f", m_fPLHP[1], m_fPLHP[2], m_fPLHP[3], m_fPLHP[4], m_fPLHP[5], m_fPLHP[6]);
	}
	else
	{
		// 默认为ASM范围
		strData.Format(L"PLHP40:%.2f,PLHP25:%.2f", m_fPLHP[1], m_fPLHP[3]);
	}
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 误差
	CString strError(L"");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 判断
	str.Format(L"\r\nJudgement=%s", L"通过");
	strResult += str;

	// 写入到结果ini文件
	CString strPathOfEquCalChkInfo = strPathOfDataExchange + L"\\EquCalChkInfo.ini";
	if (0 == _taccess(strPathOfEquCalChkInfo, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfEquCalChkInfo);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfEquCalChkInfo, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfEquCalChkInfo, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}
*/

/*
void CNHCPLHPGB2018Dlg::ExportDemarcationLog(void)
{	
	CString strPathOfDataExchange;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// 向上2层
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// 数据交换目录路径
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// 判断路径是否存在,并可以进行读写
	if (_taccess(strPathOfDataExchange, 06))
	{
		// 路径不存在

		// 创建路径
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// 创建路径失败,返回
			return;
		}
	}

	// 获取检测站编号
	CString strPathOfStationInfoFile = strPathOfDataExchange + L"\\StationInfo.ini";
	CSimpleIni siStationInfo(strPathOfStationInfoFile);
	CString strStationNumber = siStationInfo.GetString(L"StationInfo", L"StationNumber", L"");

	// 获取检测线编号和设备型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"DynamometerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

	// 获取操作员
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString str;
	CString strDemarcationLog(L"[DemarcationLog]");

	// 标定编号
	str.Format(L"\r\nDemarcationNumber=%s-%s-%s", strStationNumber, strLineNumber, odt.Format(L"%Y%m%d%H%M%S"));
	strDemarcationLog += str;

	// 检测线编号
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strDemarcationLog += str;

	// 操作员
	str.Format(L"\r\nOperator=%s", strOperator);
	strDemarcationLog += str;

	// 设备编号
	str.Format(L"\r\nEquipmentNumber=%s", strEquipmentNumber);
	strDemarcationLog += str;

	// 设备型号
	str.Format(L"\r\nEquipmentModel=%s", strEquipmentModel);
	strDemarcationLog += str;

	// 设备名称
	str.Format(L"\r\nEquipmentName=%s", L"底盘测功机");
	strDemarcationLog += str;

	// 标定项目
	str.Format(L"\r\nDemarcationItem=%d", DYN_PLHP);
	strDemarcationLog += str;

	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// 检查是否通过
	str.Format(L"\r\nPass=%s", L"1");
	strDemarcationLog += str;

	// 写入到结果ini文件
	CString strPathOfDemarcationLog = strPathOfDataExchange + L"\\DemarcationLog.ini";
	if (0 == _taccess(strPathOfDemarcationLog, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfDemarcationLog);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfDemarcationLog, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfDemarcationLog, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strDemarcationLog);
	sfe.Close();
}
*/

void CNHCPLHPGB2018Dlg::ExportDynPLHPResult(void)
{	
	CString str;
	CString strResult(L"[ResultOfDynPLHP]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 记录开始时间
	str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 记录结束时间
	str.Format(L"\r\nEndTime=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 基本惯量
	str.Format(L"\r\nDIW=%d", m_usDIW);
	strResult += str;

	//	PLHPMode	int	Checked
	CString strPLHPMode(L"1");
	if (PLHP_NORMAL_MODE == m_pmUsed)
	{
		strPLHPMode = L"1";
	}
	else if (PLHP_F_MODE == m_pmUsed)
	{
		strPLHPMode = L"2";
	}
	else
	{
		strPLHPMode = L"3";
	}
	str.Format(L"\r\nPLHPMode=%s", strPLHPMode);
	strResult += str;

	//  PLHPType	int	Checked
	CString strPLHPType(L"1");
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM寄生功率
		strPLHPType = L"1";
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS寄生功率
		strPLHPType = L"2";
	}
	else
	{
		// LUGDOWN寄生功率
		strPLHPType = L"3";
	}
	str.Format(L"\r\nPLHPType=%s", strPLHPType);
	strResult += str;

	//	ApWithRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nApWithRemanence=%.2f", m_fApWithRemanence);
	strResult += str;

	//	BpWithRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nBpWithRemanence=%.2f", m_fBpWithRemanence);
	strResult += str;

	//	CpWithRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nCpWithRemanence=%.2f", m_fCpWithRemanence);
	strResult += str;

	//	ApWithoutRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nApWithoutRemanence=%.2f", m_fApWithoutRemanence);
	strResult += str;

	//	BpWithoutRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nBpWithoutRemanence=%.2f", m_fBpWithoutRemanence);
	strResult += str;

	//	CpWithoutRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nCpWithoutRemanence=%.2f", m_fCpWithoutRemanence);
	strResult += str;

	for (int i=0; i<9; i++)
	{
		str.Format(L"\r\nLowerVelocity%d=%.2f", i, m_fLowerVelocity[i]);
		strResult += str;

		str.Format(L"\r\nUpperVelocity%d=%.2f", i, m_fUpperVelocity[i]);
		strResult += str;

		str.Format(L"\r\nNomialVelocity%d=%.2f", i, m_fNomialVelocity[i]);
		strResult += str;

		str.Format(L"\r\nPLHPTime%d=%u", i, m_pNHCPLHPThread->GetPLHPTime(i));
		strResult += str;

		str.Format(L"\r\nPLHP%d=%.2f", i, m_fPLHP[i]);
		strResult += str;

		str.Format(L"\r\nRemanenceByForce%d=%.2f", i, m_pNHCPLHPThread->GetRemanenceAveByForce(i));
		strResult += str;

		str.Format(L"\r\nRemanenceByPower%d=%.2f", i, m_pNHCPLHPThread->GetRemanenceAveByPower(i));
		strResult += str;
	}
	bool bPass = true;
	if (L"1" == strPLHPType
		|| L"2" == strPLHPType
		)
	{
		if (m_fPLHP[1] > 2.5f
			|| m_fPLHP[2] > 2.5f
			)
		{
			// 功率超2.5kw，不合格
			bPass = false;
		}
	}

	str.Format(L"\r\nPass=%s", L"1");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCPLHPGB2018Dlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

