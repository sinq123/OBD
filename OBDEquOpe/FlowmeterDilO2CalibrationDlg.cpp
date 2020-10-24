// FlowmeterDilO2CalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterDilO2CalibrationDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterDilO2CalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterDilO2CalibrationDlg, CDialogZoom)

CFlowmeterDilO2CalibrationDlg::CFlowmeterDilO2CalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterDilO2CalibrationDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)
, m_bIsCalibrationPass(true)
, m_pfProcessCtrl(NULL)
{
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

	//m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pFloThread = CNHEuqController::GetInstance().m_pFloThread;
}

CFlowmeterDilO2CalibrationDlg::~CFlowmeterDilO2CalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterDilO2CalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_T, m_lbT);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);

	DDX_Control(pDX, IDC_STATIC_DIL_O2, m_lbDilO2);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);

	DDX_Control(pDX, IDC_EDIT_HIGH, m_neHighO2Value);
	DDX_Control(pDX, IDC_EDIT_MIDDLE, m_neMiddleO2Value);
	DDX_Control(pDX, IDC_EDIT_LOW, m_neLowO2Value);

	DDX_Control(pDX, IDC_RADIO1, m_rdo1);
	DDX_Control(pDX, IDC_RADIO2, m_rdo2);
	DDX_Control(pDX, IDC_RADIO_HIGH, m_rdoHigh);
	DDX_Control(pDX, IDC_RADIO_MIDDLE, m_rdoMiddle);
	DDX_Control(pDX, IDC_RADIO_LOW, m_rdoLow);
	DDX_Control(pDX, IDC_STATIC_BOX, m_rdoBox);
}


BEGIN_MESSAGE_MAP(CFlowmeterDilO2CalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_RADIO1, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO_HIGH, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioHigh)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioMiddle)
	ON_BN_CLICKED(IDC_RADIO_LOW, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioLow)
END_MESSAGE_MAP()


// CFlowmeterDilO2CalibrationDlg 消息处理程序

BOOL CFlowmeterDilO2CalibrationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);

	// 延时打开串口
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFlowmeterDilO2CalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterDilO2CalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_nType = 1;
	m_nPoint = 1;
	m_rdo1.SetCheck(TRUE);
	m_rdoHigh.SetCheck(TRUE);
	ShowEntrance();

#ifdef _DEBUG
	m_neHighO2Value.SetWindowTextW(L"20.8");
	m_neMiddleO2Value.SetWindowTextW(L"4.52");
	m_neLowO2Value.SetWindowTextW(L"1.13");
#endif

	m_neHighO2Value.AllowNegative(FALSE);
	m_neHighO2Value.SetMaxDecimalPlaces(0);
	//m_neHighO2Value.SetLimitText(2);
	m_neMiddleO2Value.AllowNegative(FALSE);
	m_neMiddleO2Value.SetMaxDecimalPlaces(0);
	//m_neMiddleO2Value.SetLimitText(2);
	m_neLowO2Value.AllowNegative(FALSE);
	m_neLowO2Value.SetMaxDecimalPlaces(0);
	//m_neLowO2Value.SetLimitText(2);

	m_lbTitle.SetTitle(L"流量计稀释氧校准");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));

	// 稀释氧实时值
	m_lbDilO2.SetLEDStyle();
	m_lbDilO2.SetText(L"--");

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	EnableBasicOperation(false);
}

void CFlowmeterDilO2CalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
	const CString strConnection = si.GetString(L"AnaParams", L"ConnectToFlo", L"0");
	const bool bAnaConnectToFlo = (L"0" != strConnection); // 分析仪是否连接到流量计
	BYTE bPort(0);
	if (bAnaConnectToFlo)
	{
		// 获取通讯端口(从分析仪获取)
		bPort = (BYTE)_wtoi(si.GetString(L"AnaParams", L"Port", L"3"));
	}
	else
	{
		// 获取通讯端口(从流量计获取)
		bPort = (BYTE)_wtoi(si.GetString(L"FloParams", L"Port", L"4"));
	}
	// 获取分析仪通讯协议类型,默认为503
	CString strAnaProtocol = si.GetString(L"AnaParams", L"Protocol", L"503");
	// 转化为大写
	strAnaProtocol.MakeUpper();
	CAnaThread::InstrumentModel im;
	if (L"503" == strAnaProtocol)
	{
		im = CAnaThread::IM_NANHUA_NHA503;
	}
	else if (L"503V" == strAnaProtocol)
	{
		im = CAnaThread::IM_NANHUA_NHA503V;
	}
	else
	{
		// 其它不能被识别类型,统一作为503
		im = CAnaThread::IM_NANHUA_NHA503;
	}
	bool bIsFloOk(false);
	//if (ANA_OPEN_SUCCEED == m_pFloThread->Open(bPort, strAnaProtocol, true))
	if (m_pFloThread->IsOpen())
	{
		// 测试通讯
		BYTE bResult(0);
		if (ANA_WR_SUCCEED == m_pFloThread->SetOperationMode(CAna::ToFlowmeter, &bResult))
		{
			// 通讯成功
			switch (bResult)
			{
			case 0x06:
				{
					// 成功切换到流量计操作模式

					// 刷新实时数据
					RefreshRTData(true);

					// 就绪
					m_lbInfo.SetText(_T("流量计稀释氧校准"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					EnableBasicOperation(true);
				}
				break;
			case 0x05:
				{
					// 通讯正常，但暂未切换到流量计操作模式
					// 原因：流量计忙

					// 刷新实时数据
					RefreshRTData(true);

					// 就绪
					m_lbInfo.SetText(_T("流量计稀释氧校准"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					EnableBasicOperation(true);
				}
				break;
			case 0x15:
				{
					// 通讯错误：返回0x15
					m_lbInfo.SetText(_T("通讯错误"));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					EnableBasicOperation(false);
				}
				break;
			default:
				{
					// 通讯错误：不可识别代码
					m_lbInfo.SetText(L"通讯错误");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					EnableBasicOperation(false);
				}
				break;
			}
		}
		else
		{
			// 通讯失败
			m_lbInfo.SetText(L"通讯超时");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
			EnableBasicOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(false);
	}
}

void CFlowmeterDilO2CalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		DisplayRTData();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	// 停止刷新实时数据
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	// 重置流程
	ResetProcess();

	m_lbInfo.SetText(_T("稀释氧校准停止"));
	
	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计稀释氧校准完成", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_DILO2CAL_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计稀释氧校准开始", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_DILO2CAL_START, wchInfo, sizeof(wchInfo));
	}

	if(m_nType == 1)
	{
		Process();
	}
	else
	{
		Process1();
	}
}

void CFlowmeterDilO2CalibrationDlg::Process(void)
{
	if (0 == m_nStep)
	{
		// 重置流程
		ResetProcess();
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("通入校准气体，稳定后点击下一步"));
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(L"下一步");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_rdo1.ShowWindow(FALSE);
		m_rdo2.ShowWindow(FALSE);
		m_rdoBox.ShowWindow(FALSE);

		m_nStep = 1;
	}
	else if(1 == m_nStep)
	{
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		BeginWaitCursor();

		// 停止刷新实时数据
		RefreshRTData(false);

		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("正在校准.."));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_fValue = 20.8f;

		if (0x00 == CalibrationProcess())
		{
			m_bIsCalibrationPass = true;

			// 校准成功
			m_lbInfo.SetText(_T("稀释氧校准成功"));
		}
		else
		{
			m_bIsCalibrationPass = false;

			// 校准失败
			m_lbInfo.SetText(_T("稀释氧校准失败"));
		}
		//校准完成
		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));

		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		m_rdo1.ShowWindow(TRUE);
		m_rdo2.ShowWindow(TRUE);
		m_rdoBox.ShowWindow(TRUE);

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();

		// 标记操作已经至少完成1次
		m_bIsOperationFinishedAtLeaseOnce = true;

		// 导出结果
		ExportFloDilO2CalResult();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计稀释氧校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_DILO2CAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		// 重置步骤
		m_nStep = 0;
	}
}

void CFlowmeterDilO2CalibrationDlg::Process1(void)
{
	if (0 == m_nStep)
	{
		// 重置流程
		ResetProcess();
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("通入校准气体，稳定后点击下一步"));
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(L"下一步");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_rdo1.ShowWindow(FALSE);
		m_rdo2.ShowWindow(FALSE);
		m_rdoBox.ShowWindow(FALSE);

		m_nStep = 1;
	}
	else if(1 == m_nStep)
	{
		if(m_nPoint == 1)
		{
			CString str;
			m_neHighO2Value.GetWindowTextW(str);
			float fHighValue = (float)_wtof(str);
			if (fHighValue<18.0f || fHighValue>21.0f)
			{
				MessageBoxW(L"高点校准应在18～21之间取值", L"流量计校准", MB_OK|MB_ICONWARNING);
				m_neHighO2Value.SetFocus();
				return;
			}
		}
		else if(m_nPoint == 2)
		{
			CString str;
			m_neMiddleO2Value.GetWindowTextW(str);
			float fMiddleValue = (float)_wtof(str);
			if (fMiddleValue<4.5f || fMiddleValue>11.0f)
			{
				MessageBoxW(L"中点校准应在4.5～11之间取值", L"流量计校准", MB_OK|MB_ICONWARNING);
				m_neMiddleO2Value.SetFocus();
				return;
			}
		}
		else if(m_nPoint == 3)
		{
			CString str;
			m_neLowO2Value.GetWindowTextW(str);
			float fLowValue = (float)_wtof(str);
			if (fLowValue<0.5f || fLowValue>2.0f)
			{
				MessageBoxW(L"低点校准应在0.5～2之间取值", L"流量计校准", MB_OK|MB_ICONWARNING);
				m_neLowO2Value.SetFocus();
				return;
			}
		}

		ShowEntrance();

		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		BeginWaitCursor();

		// 停止刷新实时数据
		RefreshRTData(false);

		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("正在校准.."));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		if(m_nPoint == 1)
		{
			if (0x00 == CalibrationProcess_High())
			{
				m_bIsCalibrationPass = true;

				// 校准成功
				m_lbInfo.SetText(_T("稀释氧校准成功"));
			}
			else
			{
				m_bIsCalibrationPass = false;

				// 校准失败
				m_lbInfo.SetText(_T("稀释氧校准失败"));
			}
		}

		else if(m_nPoint == 2)
		{
			if (0x00 == CalibrationProcess_Middle())
			{
				m_bIsCalibrationPass = true;

				// 校准成功
				m_lbInfo.SetText(_T("稀释氧校准成功"));
			}
			else
			{
				m_bIsCalibrationPass = false;

				// 校准失败
				m_lbInfo.SetText(_T("稀释氧校准失败"));
			}
		}

		else if(m_nPoint == 3)
		{
			if (0x00 == CalibrationProcess_Low())
			{
				m_bIsCalibrationPass = true;

				// 校准成功
				m_lbInfo.SetText(_T("稀释氧校准成功"));
			}
			else
			{
				m_bIsCalibrationPass = false;

				// 校准失败
				m_lbInfo.SetText(_T("稀释氧校准失败"));
			}
		}

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));

		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		ShowDomestic();

		m_rdo1.ShowWindow(TRUE);
		m_rdo2.ShowWindow(TRUE);
		m_rdoBox.ShowWindow(TRUE);

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();

		// 标记操作已经至少完成1次
		m_bIsOperationFinishedAtLeaseOnce = true;

		// 导出结果
		ExportFloDilO2CalResult();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计稀释氧校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_DILO2CAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		// 重置步骤
		m_nStep = 0;
	}
}

void CFlowmeterDilO2CalibrationDlg::ResetProcess(void)
{
	// 步骤显示框
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);

	// 步骤置零
	m_nStep = 0;
	// 校准结果置初始化值
	m_bIsCalibrationPass = true;
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter(&bResult, 28.5f);
	if (0x06 == bResult)
	{
		// 写入稀释氧校准值成功

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// 稀释氧校准完成

			// 一般不可能到这个位置，因为会返回0x05(正在忙)

			 if (0x00 == bCalResult)
			 {
				 // 校准成功
				 return 0x00;
			 }
			 else
			 {
				 // 校准失败
				 return 0x01;
			 }
		}
		else if (0x05 == bResult)
		{
			// 等待校准执行完成

			WORD wProcessStatus(0);
			WORD wProcess(0);
			// 避免进入死循环，只查询100次
			for (int i=0; i<100; i++)
			{
				bResult = 0;
				wProcessStatus = 0;
				wProcess = 0;
				m_pFloThread->GetProcessStatus_Flowmeter(&bResult, &wProcessStatus, &wProcess);
				if (0x00 == wProcessStatus)
				{
					// 校准命令执行完成
					break;
				}
				else
				{
					// 小等一会^_^
					ASSERT(0x36 == wProcess);
					Sleep(100);
				}
			}
			// 取校准结果，用长命令模式
			bResult = 0;
			bCalResult = 0;
			m_pFloThread->DilO2Calibration_Flowmeter(&bResult, &bCalResult, true);
			if (0x06==bResult && 0x00==bCalResult)
			{
				// 校准成功
				return 0x00;
			}
			else
			{
				// 校准失败
				return 0x01;
			}
		}
		else
		{
			// 校准失败
			return 0x01;
		}
	}
	else
	{
		// 写入稀释氧校准值失败

		return 0x01;
	}
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess_High(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	CString str;
	m_neHighO2Value.GetWindowTextW(str);
	float fHighValue = (float)_wtof(str);
	m_fValue = fHighValue;
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_High(&bResult, fHighValue);
	if (0x06 == bResult)
	{
		// 写入稀释氧校准值成功

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter_High(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// 稀释氧校准完成

			return 0x00;
		}
		else
		{
			// 校准失败
			return 0x01;
		}
	}
	else
	{
		// 写入稀释氧校准值失败

		return 0x01;
	}
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess_Middle(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	CString str;
	m_neMiddleO2Value.GetWindowTextW(str);
	float fMiddleValue = (float)_wtof(str);
	m_fValue = fMiddleValue;
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Middle(&bResult, fMiddleValue);
	if (0x06 == bResult)
	{
		// 写入稀释氧校准值成功

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter_Middle(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// 稀释氧校准完成

			return 0x00;
		}
		else
		{
			// 校准失败
			return 0x01;
		}
	}
	else
	{
		// 写入稀释氧校准值失败

		return 0x01;
	}
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess_Low(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	CString str;
	m_neLowO2Value.GetWindowTextW(str);
	float fLowValue = (float)_wtof(str);
	m_fValue = fLowValue;
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Low(&bResult, fLowValue);
	if (0x06 == bResult)
	{
		// 写入稀释氧校准值成功

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter_Low(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// 稀释氧校准完成

			return 0x00;
		}
		else
		{
			// 校准失败
			return 0x01;
		}
	}
	else
	{
		// 写入稀释氧校准值失败

		return 0x01;
	}
}

void CFlowmeterDilO2CalibrationDlg::ExportFloDilO2CalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloDilO2Cal]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	strData.Format(L"标准值:%.2fkPa", 20.8f);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 是否通过
	str.Format(L"\r\nPass=%s", m_bIsCalibrationPass ? L"1" : L"0");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloDilO2Cal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CFlowmeterDilO2CalibrationDlg::RefreshRTData(bool bRefresh)
{
	if (bRefresh)
	{
		SetTimer(2, 1000, NULL);
	}
	else
	{
		KillTimer(2);
	}
}

void CFlowmeterDilO2CalibrationDlg::DisplayRTData(void)
{
	m_pFloThread->GetRealTimeData_Flowmeter_Aysn();

	float f = m_pFloThread->GetVmasRTDUDilO2Value();
	CString str;
	str.Format(L"%.2f", f);
	m_lbDilO2.SetText(str);
}

void CFlowmeterDilO2CalibrationDlg::ClearRTDisplay(void)
{
	m_lbDilO2.SetText(L"--");
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOn()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOff()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterDilO2CalibrationDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterDilO2CalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowEntrance();
	m_nType = 1;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowDomestic();
	m_nType = 2;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioHigh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPoint = 1;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioMiddle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPoint = 2;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioLow()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPoint = 3;
}

void CFlowmeterDilO2CalibrationDlg::ShowEntrance()
{
	m_rdoHigh.ShowWindow(FALSE);
	m_rdoMiddle.ShowWindow(FALSE);
	m_rdoLow.ShowWindow(FALSE);

	m_neHighO2Value.ShowWindow(FALSE);
	m_neMiddleO2Value.ShowWindow(FALSE);
	m_neLowO2Value.ShowWindow(FALSE);

	m_lbT.ShowWindow(FALSE);
}

void CFlowmeterDilO2CalibrationDlg::ShowDomestic()
{
	m_rdoHigh.ShowWindow(TRUE);
	m_rdoMiddle.ShowWindow(TRUE);
	m_rdoLow.ShowWindow(TRUE);

	m_neHighO2Value.ShowWindow(TRUE);
	m_neMiddleO2Value.ShowWindow(TRUE);
	m_neLowO2Value.ShowWindow(TRUE);

	m_lbT.ShowWindow(TRUE);
}