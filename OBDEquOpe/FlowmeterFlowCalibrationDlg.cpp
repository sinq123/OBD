// FlowmeterFlowCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterFlowCalibrationDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterFlowCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterFlowCalibrationDlg, CDialogZoom)

CFlowmeterFlowCalibrationDlg::CFlowmeterFlowCalibrationDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CFlowmeterFlowCalibrationDlg::IDD, pParent)
// 步骤
, m_nStep(0)
// 操作是否至少完成1次
, m_bIsOperationFinishedAtLeaseOnce(false)
// 校准结果是否Pass
, m_bIsPass(false)
// 高流量标准值,单位:L/s
, m_fStandardValueOfHighFlux(0.0f)
// 低流量标准值,单位:L/s
, m_fStandardValueOfLowFlux(0.0f)
, m_fMaximumZeroOffsetValue(36.7f)
, m_pfProcessCtrl(NULL)
, m_bDomestic(false)
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

	// 导入参数
	ImportParams();

	//m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pFloThread = CNHEuqController::GetInstance().m_pFloThread;
}

CFlowmeterFlowCalibrationDlg::~CFlowmeterFlowCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterFlowCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_STATIC_FLUX, m_lbFlux);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP5_LABEL, m_stcStep5Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_HIGH_FLUX, m_neStandardValueHighFlux);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_LOW_FLUX, m_neStandardValueLowFlux);
}


BEGIN_MESSAGE_MAP(CFlowmeterFlowCalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_HIGH_FLUX, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationHighFlux)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_LOW_FLUX, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationLowFlux)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_RADIO1, &CFlowmeterFlowCalibrationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFlowmeterFlowCalibrationDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CFlowmeterFlowCalibrationDlg 消息处理程序

BOOL CFlowmeterFlowCalibrationDlg::OnInitDialog()
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

void CFlowmeterFlowCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterFlowCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"流量计流量校准");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// 流量实时值;
	m_lbFlux.SetLEDStyle();
	m_lbFlux.SetText(L"--");

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	// 界面按钮
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	EnableBasicOperation(false);
}

void CFlowmeterFlowCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
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
	else if (3 == nIDEvent)
	{
		KillTimer(3);

		// 继续流量
		Process();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterFlowCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("流量计流量校准"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
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
					m_lbInfo.SetText(_T("流量计流量校准"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
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
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
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
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
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
			GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
			EnableBasicOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(false);
	}

#ifdef _DEBUG
	// 刷新实时数据
	RefreshRTData(true);

	// 就绪
	m_lbInfo.SetText(_T("流量计流量校准"));
	// 位于实时通讯界面
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
	EnableBasicOperation(true);
#endif

}

void CFlowmeterFlowCalibrationDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterFlowCalibrationDlg::DisplayRTData(void)
{
	m_pFloThread->GetRealTimeData_Flowmeter_Aysn();

	float f = m_pFloThread->GetVmasRTDFluxStd20Value();
	// 零偏移值约束
	if (f < m_fMaximumZeroOffsetValue)
	{
		f = 0.0f;
	}
	CString str;
	str.Format(L"%.2f", f);
	m_lbFlux.SetText(str);
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);

	Process();

}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	// 关闭流量分析仪风机
	m_pFloThread->RunFan_Flowmeter(false);

	// 重置流程
	ResetProcess();

	m_lbInfo.SetText(_T("流量校准停止"));
		
	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计流量校准停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_FLUXCAL_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);

	EnableBasicOperation(true);
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	// 停止刷新实时数据
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationHighFlux()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取高流量标准值
	if (0x00 != GetStandardValueOfHighFlux())
	{
		return;
	}

	BeginWaitCursor();

	// 停止刷新数据
	RefreshRTData(false);

	// 写入校准值并校准
	BYTE bResult(0x00);
	CAna::FluxCalibrationMode nFCM;
	CString strFCM(L"");
	CString strMsg(L"");
	if (m_bDomestic)
	{
		nFCM = CAna::SinglePointFlowCal;
		strFCM.Format(L"单点流量");
	}
	else
	{
		nFCM = CAna::HighFlowCal;
		strFCM.Format(L"高流量");
	}

	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, nFCM, m_fStandardValueOfHighFlux);
	if (0x06 == bResult)
	{
		// 写入高/单点流量标准值成功

		BYTE bCalResult(0);
		
		m_pFloThread->FluxCalibration_Flowmeter(&bResult, &bCalResult, nFCM);
		if (0x06==bResult && 0x00==bCalResult)
		{
			// 高/单点流量校准成功
			strMsg.Format(L"%s校准成功", strFCM);
			MessageBox(strMsg, L"流量校准", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			// 高/单点流量校准失败
			strMsg.Format(L"%s校准失败", strFCM);
			MessageBox(strMsg, L"流量校准", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		// 写入高/单点流量标准值失败
		strMsg.Format(L"%s校准失败：写入%s标准值失败", strFCM, strFCM);
		MessageBox(strMsg, L"流量校准", MB_OK|MB_ICONWARNING);
	}

	// 开始刷新数据
	RefreshRTData(true);

	EndWaitCursor();
}

DWORD CFlowmeterFlowCalibrationDlg::GetStandardValueOfHighFlux(void)
{
	// 获取标准值并检查
	CString strStandardValue;
	m_neStandardValueHighFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueHighFlux.SetFocus();
		if (m_bDomestic)
		{
			MessageBox(_T("请输入单点流量标准值"), _T("流量校准"), MB_OK|MB_ICONWARNING);
		}
		else
		{
			MessageBox(_T("请输入高流量标准值"), _T("流量校准"), MB_OK|MB_ICONWARNING);
		}
		return 0x01;
	}
	// 流量标准值,单位:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// 格式化为2为小数，并四舍五入
	m_fStandardValueOfHighFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// 合理性约束:通用高流量(50L/s~500L/s) 南华单点(50L/s~235L/s)
	if (m_bDomestic)
	{
		if ((m_fStandardValueOfHighFlux<50.0f || m_fStandardValueOfHighFlux>235.0f))
		{
			m_neStandardValueHighFlux.SetFocus();
			MessageBox(_T("单点流量标准值不在合理范围(50L/s~235L/s)"), _T("流量校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	else
	{
		if (m_fStandardValueOfHighFlux<50.0f || m_fStandardValueOfHighFlux>500.0f)
		{
			m_neStandardValueHighFlux.SetFocus();
			MessageBox(_T("高流量标准值不在合理范围(50L/s~500L/s)"), _T("流量校准"), MB_OK|MB_ICONWARNING);

			return 0x01;
		}
	}
	return 0x00;
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationLowFlux()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取低流量标准值
	if (0x00 != GetStandardValueOfLowFlux())
	{
		return;
	}

	BeginWaitCursor();

	// 停止刷新数据
	RefreshRTData(false);

	// 写入校准值并校准
	BYTE bResult(0x00);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CAna::LowFlowCal, m_fStandardValueOfLowFlux);
	if (0x06 == bResult)
	{
		// 写入低流量标准值成功

		BYTE bCalResult(0);
		m_pFloThread->FluxCalibration_Flowmeter(&bResult, &bCalResult, CAna::LowFlowCal);
		if (0x06==bResult && 0x00==bCalResult)
		{
			// 低流量校准成功

			MessageBox(L"低流量校准成功", L"流量校准", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			// 低流量校准失败

			MessageBox(L"低流量校准失败", L"流量校准", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		// 写入低流量标准值失败

		MessageBox(L"低流量校准失败：写入低流量标准值失败", L"流量校准", MB_OK|MB_ICONWARNING);
	}

	// 开始刷新数据
	RefreshRTData(true);

	EndWaitCursor();
}

DWORD CFlowmeterFlowCalibrationDlg::GetStandardValueOfLowFlux(void)
{
	// 获取标准值并检查
	CString strStandardValue;
	m_neStandardValueLowFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueLowFlux.SetFocus();
		MessageBox(_T("请输入低流量标准值"), _T("流量校准"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// 流量标准值,单位:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// 格式化为2为小数，并四舍五入
	m_fStandardValueOfLowFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// 合理性约束(0L/s~500L/s)
	if (m_fStandardValueOfLowFlux<0.0f || m_fStandardValueOfLowFlux>500.0f)
	{
		m_neStandardValueLowFlux.SetFocus();
		MessageBox(_T("低流量标准值不在合理范围(0L/s~500L/s)"), _T("流量校准"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CFlowmeterFlowCalibrationDlg::Process(void)
{
	if (0 == m_nStep)
	{
		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计流量校准开始", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCAL_START, wchInfo, sizeof(wchInfo));
		}
		
		// 连接标准流量计

		// 重置流程
		ResetProcess();

		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("请连接标准流量计,然后下一步"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(true);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		// 打开风机

		BeginWaitCursor();

		// 停止刷新实时数据
		RefreshRTData(false);

		// 打开流量分析仪风机
		m_pFloThread->RunFan_Flowmeter(true);

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();
		
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("正在打开风机..."));

		// 延时2秒自动进入下一步
		SetTimer(3, 2000, NULL);

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(true);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		// 高流量校准 或 单点流量校准

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		if (m_bDomestic)
		{
			m_lbInfo.SetText(_T("请进行单点流量校准,然后下一步"));
			m_nStep = 4;
		}
		else
		{
			m_lbInfo.SetText(_T("请进行高流量校准,然后下一步"));
			m_nStep = 3;
		}
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(true);
	}
	else if (3 == m_nStep)
	{
		// 低流量校准
		BeginWaitCursor();
		
		// 关闭流量分析仪风机
		m_pFloThread->RunFan_Flowmeter(false);
		
		EndWaitCursor();

		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("请进行低流量校准,然后下一步"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		m_nStep = 4;
	}
	else if (4 == m_nStep)
	{
		BeginWaitCursor();

		// 停止刷新实时数据
		RefreshRTData(false);

		// 关闭流量分析仪风机
		m_pFloThread->RunFan_Flowmeter(false);

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();

		if (m_bDomestic)
		{
			m_stcStep3Label.ShowWindow(SW_SHOW);
			m_lbStep3.SetTextColor(RGB(0, 0, 0));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));
		}
		else
		{
			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));
			m_lbStep5.SetTextColor(RGB(0, 0, 255));
			m_stcStep5Label.ShowWindow(SW_SHOW);
			m_lbStep5.SetTextColor(RGB(0, 0, 0));
		}

		m_lbInfo.SetText(_T("校准完成"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		// 导出结果
		ExportFloFluxCalResult();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计流量校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		// 步骤置零
		m_nStep = 0;
	}
}

void CFlowmeterFlowCalibrationDlg::ResetProcess(void)
{
	// 步骤显示框
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));
	m_lbStep4.SetTextColor(RGB(0,0,0));
	m_lbStep5.SetTextColor(RGB(0,0,0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);
	
	// 步骤置零
	m_nStep = 0;
	// 校准结果置初始化值
	m_bIsPass = true;
}

void CFlowmeterFlowCalibrationDlg::ImportParams(void)
{
	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CString strEquipmentPath = strPath + L"\\Config\\Equipment.ini";
	CSimpleIni sio(strEquipmentPath);
	CString str = sio.GetString(L"FloParams", L"MaximumZeroOffsetValue", L"36.7");
	float f = (float)_wtof(str);
	// 格式化两位小数
	m_fMaximumZeroOffsetValue = (int)(f * 100.0f +0.5f) / 100.0f;
	ASSERT(m_fMaximumZeroOffsetValue > -10e-6);
	// 合理性约束(0L/s~36.7L/s)
	if (m_fMaximumZeroOffsetValue < 0.0f)
	{
		m_fMaximumZeroOffsetValue = 0.0f;
	}
	if (m_fMaximumZeroOffsetValue > 36.7f)
	{
		m_fMaximumZeroOffsetValue = 36.7f;
	}
}

void CFlowmeterFlowCalibrationDlg::ExportFloFluxCalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloFluxCal]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;
	
	// 数据
	CString strData;
	strData.Format(L"高流量标准值:%.2fL/s,低流量标准值%.2fL/s", m_fStandardValueOfHighFlux, m_fStandardValueOfLowFlux);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 是否通过
	str.Format(L"\r\nPass=%s", m_bIsPass ? L"1" : L"0");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOn()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOff()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterFlowCalibrationDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterFlowCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CFlowmeterFlowCalibrationDlg::ShowEntrance()
{
	m_bDomestic = false;
	m_lbStep3.SetWindowTextW(L"高流量校准");
	m_lbStep4.SetWindowTextW(L"低流量校准");
	m_lbStep5.ShowWindow(SW_SHOW);
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);


	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_STANDARD_VALUE_LOW_FLUX)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_LFC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_HFC)->SetWindowText(L"高流量校准");
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->SetWindowText(L"高流量校准");
	m_neStandardValueHighFlux.SetWindowText(L"");
	m_neStandardValueLowFlux.SetWindowText(L"");
}

void CFlowmeterFlowCalibrationDlg::ShowDomestic()
{
	m_bDomestic = true;
	m_lbStep4.SetWindowTextW(L"完成校准");
	m_lbStep3.SetWindowTextW(L"单点流量校准");
	m_lbStep5.ShowWindow(SW_HIDE);
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_STANDARD_VALUE_LOW_FLUX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_LFC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_HFC)->SetWindowText(L"单点流量校准");
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->SetWindowText(L"单点流量校准");
	m_neStandardValueHighFlux.SetWindowText(L"");
	m_neStandardValueLowFlux.SetWindowText(L"");
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowEntrance();
}


void CFlowmeterFlowCalibrationDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowDomestic();
}
