// NHA503XEnvParamsCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHA503XEnvParamsCalibrationDlg.h"


// CNHA503XEnvParamsCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CNHA503XEnvParamsCalibrationDlg, CDialogZoom)

CNHA503XEnvParamsCalibrationDlg::CNHA503XEnvParamsCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHA503XEnvParamsCalibrationDlg::IDD, pParent)
	, m_pAnaThread(NULL)
	, m_bRefreshDataSign(true)
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

	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pAnaThread;
}

CNHA503XEnvParamsCalibrationDlg::~CNHA503XEnvParamsCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pAnaThread)
	//{
	//	m_pAnaThread->Close();
	//	m_pAnaThread->QuitThread();
	//	WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	//}
}

void CNHA503XEnvParamsCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_EDIT_ET_MEA, m_edETMea);
	DDX_Control(pDX, IDC_EDIT_RH_MEA, m_edRHMea);
	DDX_Control(pDX, IDC_EDIT_AP_MEA, m_edAPMea);
	DDX_Control(pDX, IDC_EDIT_OT_MEA, m_edOTMea);

	DDX_Control(pDX, IDC_EDIT_ET_STD, m_neETStd);
	DDX_Control(pDX, IDC_EDIT_RH_STD, m_neRHStd);
	DDX_Control(pDX, IDC_EDIT_AP_STD, m_neAPStd);
	DDX_Control(pDX, IDC_EDIT_OT_STD, m_neOTStd);
}


BEGIN_MESSAGE_MAP(CNHA503XEnvParamsCalibrationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ET_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonEtCal)
	ON_BN_CLICKED(IDC_BUTTON_RH_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonRhCal)
	ON_BN_CLICKED(IDC_BUTTON_AP_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonApCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonOtCal)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonQuit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHA503XEnvParamsCalibrationDlg 消息处理程序

BOOL CNHA503XEnvParamsCalibrationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);

	// 延时打开串口
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHA503XEnvParamsCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHA503XEnvParamsCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"分析仪环境参数及油温校准");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// 测量值编辑框
	m_edETMea.SetReadOnly(TRUE);
	m_edRHMea.SetReadOnly(TRUE);
	m_edAPMea.SetReadOnly(TRUE);
	m_edOTMea.SetReadOnly(TRUE);

	// 校准值编辑框
	m_neETStd.AllowNegative(FALSE);
	m_neETStd.SetMaxDecimalPlaces(1);
	m_neRHStd.AllowNegative(FALSE);
	m_neRHStd.SetMaxDecimalPlaces(1);
	m_neAPStd.AllowNegative(FALSE);
	m_neAPStd.SetMaxDecimalPlaces(1);
	m_neOTStd.AllowNegative(FALSE);
	m_neOTStd.SetMaxDecimalPlaces(1);

	GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonEtCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdET;
	GetDlgItem(IDC_EDIT_ET_STD)->GetWindowTextW(strStdET);
	if (L"" == strStdET)
	{
		MessageBox(L"请输入环境温度标准值", L"环境温度校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdET = static_cast<float>(_wtof(strStdET));
	if (fStdET<-50.0f || fStdET>200.0f)
	{
		MessageBox(L"环境温度标准值不在合理范围\r\n请输入合理的环境参数标准值", L"环境温度校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->ETCalibration(&bResult, fStdET);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"环境温度标准成功", L"环境温度校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"环境温度标准失败", L"环境温度校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonRhCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdRH;
	GetDlgItem(IDC_EDIT_RH_STD)->GetWindowTextW(strStdRH);
	if (L"" == strStdRH)
	{
		MessageBox(L"请输入相对湿度标准值", L"相对湿度校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdRH = static_cast<float>(_wtof(strStdRH));
	if (fStdRH<0.0f || fStdRH>100.0f)
	{
		MessageBox(L"相对湿度标准值不在合理范围\r\n请输入合理的相对湿度标准值", L"相对湿度校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->RHCalibration(&bResult, fStdRH);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"相对湿度标准成功", L"相对湿度校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"相对湿度标准失败", L"相对湿度校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonApCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdAP;
	GetDlgItem(IDC_EDIT_AP_STD)->GetWindowTextW(strStdAP);
	if (L"" == strStdAP)
	{
		MessageBox(L"请输入大气压力标准值", L"大气压力校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdAP = static_cast<float>(_wtof(strStdAP));
	if (fStdAP<0.0f || fStdAP>200.0f)
	{
		MessageBox(L"大气压力标准值不在合理范围\r\n请输入合理的大气压力标准值", L"大气压力校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->APCalibration(&bResult, fStdAP);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"大气压力标准成功", L"大气压力校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"大气压力标准失败", L"大气压力校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonOtCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdOT;
	GetDlgItem(IDC_EDIT_OT_STD)->GetWindowTextW(strStdOT);
	if (L"" == strStdOT)
	{
		MessageBox(L"请输入油温标准值", L"油温校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdOT = static_cast<float>(_wtof(strStdOT));
	if (fStdOT<-50.0f || fStdOT>200.0f)
	{
		MessageBox(L"油温标准值不在合理范围\r\n请输入合理的油温标准值", L"油温校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->OTCalibration(&bResult, fStdOT);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"油温标准成功", L"油温校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"油温标准失败", L"油温校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CNHA503XEnvParamsCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"AnaParams", L"Port", L"3"));
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
	bool bConnectToFlowmeter(false);
	if (CAnaThread::IM_NANHUA_NHA503V == im)
	{
		// 获取分析仪连接方式
		CString strConnectToFlowmeter = si.GetString(L"AnaParams", L"ConnectToFlo", L"0");
		if (L"1" == strConnectToFlowmeter)
		{
			// 连接到流量计
			bConnectToFlowmeter = true;
		}
		else
		{
			// 连接到计算机
			bConnectToFlowmeter = false;
		}
	}
	//if (ANA_OPEN_SUCCEED == m_pAnaThread->Open(bPort, im, bConnectToFlowmeter))
	if (m_pAnaThread->IsOpen())
	{
		if (CNHEuqController::GetInstance().m_bConnectToFlowmeter)
		{
			m_pAnaThread->SetOperationMode(CAna::ToAnalyzer);
		}

		// 测试通讯
		BYTE bStatus(0);
		if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		{
			// 通讯成功
			switch (bStatus)
			{
			case 0x00:  // 就绪(未开泵)
			case 0x01:  // 就绪(已开泵)
				{
					// 关泵
					m_pAnaThread->StopPump();
					// 禁止自动调零
					m_pAnaThread->DisableAutoZero();

					// 就绪
					m_lbInfo.SetText(_T("分析仪环境参数与油温校准"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

					// 开始刷新数据
					EnableRefreshData(true);
				}
				break;
			case 0x02:  // 预热
				{
					// 正在预热
					m_lbInfo.SetText(_T("正在预热.."));
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x03:	// 检漏
				{
					// 正在检漏
					m_lbInfo.SetText(_T("正在检漏.."));
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0xFF:
				{
					// 通讯错误,不在可通讯界面
					m_lbInfo.SetText(L"仪器不在实时检测状态");
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			default:
				{
					// 通讯错误,不在可通讯界面
					m_lbInfo.SetText(L"仪器不在实时检测状态");
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			}
		}
		else
		{
			// 通讯失败
			m_lbInfo.SetText(L"通讯超时");
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 1:
		{
			RefreshData();
		}
		break;
	case 2:
		{
			KillTimer(2);
			OpenSerialPortAndTestCommunication();
		}
		break;
	default:
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}


void CNHA503XEnvParamsCalibrationDlg::RefreshData(void)
{
	// 轮流刷新主数据和副数据
	if (m_bRefreshDataSign)
	{
		m_pAnaThread->GetMinorData_Asyn();
	}
	else
	{
		m_pAnaThread->GetMajorData_Asyn();
	}
	m_bRefreshDataSign = !m_bRefreshDataSign;

	float fET = m_pAnaThread->GetMinorDataETValue();
	float fRH = m_pAnaThread->GetMinorDataRHValue();
	float fAP = m_pAnaThread->GetMinorDataAPValue();
	float fOT = m_pAnaThread->GetMajorDataOilTempValue();

	CString str;
	
	str.Format(L"%.1f", fET);
	m_edETMea.SetWindowTextW(str);

	str.Format(L"%.1f", fRH);
	m_edRHMea.SetWindowTextW(str);

	str.Format(L"%.1f", fAP);
	m_edAPMea.SetWindowTextW(str);

	str.Format(L"%.1f", fOT);
	m_edOTMea.SetWindowTextW(str);
}

void CNHA503XEnvParamsCalibrationDlg::EnableRefreshData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(1, 1000, NULL);
	}
	else
	{
		KillTimer(1);
	}
}