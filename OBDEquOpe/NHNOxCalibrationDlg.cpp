// NHNOxCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHNOxCalibrationDlg.h"

#define CHN_CO2		0x01
#define CHN_NO		0x02
#define CHN_CO2_NO	0x03
#define CHN_NO2		0x04

// CNHNOxCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CNHNOxCalibrationDlg, CDialogZoom)

CNHNOxCalibrationDlg::CNHNOxCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHNOxCalibrationDlg::IDD, pParent)
	
	// CO2测量值,单位:0.001%,放大1000倍,四舍五入
	, m_nCO2MeasuredValue(0)
	// NO测量值,单位:ppm
	, m_nNOMeasuredValue(0)
	// NO2测量值,单位:ppm
	, m_nNO2MeasuredValue(0)
	
	// 标准气CO2标称值,单位:0.001%(放到1000倍)
	, m_nCO2NominalValue(0)
	// 标准气NO标称值,单位:ppm
	, m_nNONominalValue(0)
	// 标准气NO2标称值,单位:ppm
	, m_nNO2NominalValue(0)

	// 校准通道
	, m_nCalChn(CHN_CO2_NO)

	// 是否查询调零状态
	, m_bCheckZeroStatus(true)

	// 步骤
	, m_nStep(0)
	// 操作是否至少完成1次
	, m_bIsOperationFinishedAtLeaseOnce(false)
	// 校准是否Pass
	, m_bIsCalibrationPass(true)

	// 初始化回调函数指针
	, m_pfProcessCtrl(NULL)
{
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

	// 导入参数
	ImportParams();

	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pNOxThread;
	
	// GB3847
	m_nStandardGas1 = 0x09;
	m_nStandardGas2 = 0x0A;
	m_nStandardGas3 = 0x0B;
	m_nStandardGas4 = 0x0C;
}

CNHNOxCalibrationDlg::~CNHNOxCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pAnaThread)
	//{
	//	m_pAnaThread->Close();
	//	m_pAnaThread->QuitThread();
	//	WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	//}
}

void CNHNOxCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);

	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_NO, m_lbNO);
	DDX_Control(pDX, IDC_STATIC_NO2, m_lbNO2);
	DDX_Control(pDX, IDC_STATIC_NOX, m_lbNOx);

	DDX_Control(pDX, IDC_EDIT_CO2_NOMINAL_VALUE, m_edCO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_NO_NOMINAL_VALUE, m_edNONominalValue);
	DDX_Control(pDX, IDC_EDIT_NO2_NOMINAL_VALUE, m_edNO2NominalValue);

	DDX_Control(pDX, IDC_MATHLABLE1, m_mlb1);
	DDX_Control(pDX, IDC_MATHLABLE2, m_mlb2);
	DDX_Control(pDX, IDC_MATHLABLE3, m_mlb3);
	DDX_Control(pDX, IDC_MATHLABLE4, m_mlb4);
	DDX_Control(pDX, IDC_MATHLABLE5, m_mlb5);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
}


BEGIN_MESSAGE_MAP(CNHNOxCalibrationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHNOxCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHNOxCalibrationDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHNOxCalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO_CAL, &CNHNOxCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO2_CAL, &CNHNOxCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CO2_CAL, &CNHNOxCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO_CO2_CAL, &CNHNOxCalibrationDlg::OnBnClickedRadioChnCal)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHNOxCalibrationDlg 消息处理程序

BOOL CNHNOxCalibrationDlg::OnInitDialog()
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

void CNHNOxCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHNOxCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"氮氧分析仪气体校准");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置上下标文本控件
	m_mlb1.SetScriptText(L"NO[10^-^6]");
	m_mlb2.SetScriptText(L"NO_2[10^-^6]");
	m_mlb3.SetScriptText(L"NO[10^-^6]");
	m_mlb4.SetScriptText(L"NO_2[10^-^6]");
	m_mlb5.SetScriptText(L"NOx[10^-^6]");

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	
	// CO2实时值
	m_lbCO2.SetLEDStyle();
	m_lbCO2.SetText(L"--");
	// NO实时值
	m_lbNO.SetLEDStyle();
	m_lbNO.SetText(L"--");
	// NO2实时值
	m_lbNO2.SetLEDStyle();
	m_lbNO2.SetText(L"--");
	// NOx实时值
	m_lbNOx.SetLEDStyle();
	m_lbNOx.SetText(L"--");
	
	m_edCO2NominalValue.AllowNegative(FALSE);
	m_edCO2NominalValue.SetMaxDecimalPlaces(3);

	// 显示校准气标称值历史记录
	CString str;
	str.Format(L"%.3f", m_nCO2NominalValue/1000.0f);
	m_edCO2NominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNONominalValue);
	m_edNONominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNO2NominalValue);
	m_edNO2NominalValue.SetWindowText(str);

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	// 初始化为NO通道校准	
	// 程序模拟鼠标点击
	GetDlgItem(IDC_RADIO_CHN_NO_CAL)->SendMessage(WM_LBUTTONDOWN);
	GetDlgItem(IDC_RADIO_CHN_NO_CAL)->SendMessage(WM_LBUTTONUP);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

	((CButton *)GetDlgItem(IDC_CHECK_MP4))->SetCheck(BST_CHECKED);
}

void CNHNOxCalibrationDlg::ImportParams(void)
{
	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// 去除配置文件只读属性
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);
	CString str;
	
	str = si.GetString(L"AnaCal", L"CO2NominalValue", L"12");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6f);
	// 放大1000倍,并四舍五入
	m_nCO2NominalValue = (int)(f * 1000 + 0.5f);

	str = si.GetString(L"AnaCal", L"NONominalValue", L"5000");
	m_nNONominalValue = _wtoi(str);

	str = si.GetString(L"AnaCal", L"NO2NominalValue", L"3000");
	m_nNO2NominalValue = _wtoi(str);
}

void CNHNOxCalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	GetDlgItem(IDC_BUTTON_START)->GetWindowText(strTemp);
	if (strTemp == L"开始")
	{
		// 禁用按钮
		GetDlgItem(IDC_CHECK_MP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MP1)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CHN_CO2_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO2_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO_CO2_CAL)->EnableWindow(FALSE);
	}

	if (GetKeyState(VK_RMENU)<0 && GetKeyState('S')<0)
	{
		// 重置流程
		ResetProcess();
		// 如果Alt+S,则跳过调零(右边Alt键,s键不分大小写)
		m_nStep = 1;
	}

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体校准开始", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CAL_START, wchInfo, sizeof(wchInfo));
	}

	Process();
}

void CNHNOxCalibrationDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);

	// 重置流程
	ResetProcess();

	m_lbInfo.SetText(_T("气体检查停止"));

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	
	// 启用按钮
	{
		GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_CHN_CO2_CAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO_CAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO2_CAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO_CO2_CAL)->EnableWindow(TRUE);
	}

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体校准停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CAL_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHNOxCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CNHNOxCalibrationDlg::OnBnClickedRadioChnCal()
{
	if (((CButton *)GetDlgItem(IDC_RADIO_CHN_NO_CAL))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = CHN_NO;

		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_NO2_CAL))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = CHN_NO2;

		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(TRUE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_NO_CO2_CAL))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = CHN_CO2_NO;

		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CO2_CAL))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = CHN_CO2;

		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
}

void CNHNOxCalibrationDlg::ClearRTDisplay(void)
{
	// CO2实时值
	m_lbCO2.SetText(L"--");
	// NO2实时值
	m_lbNO2.SetText(L"--");
	// NO实时值
	m_lbNO.SetText(L"--");
	// NOx实时值
	m_lbNOx.SetText(L"--");
}

void CNHNOxCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		CalibrationProcess();
	}
	else if (2 == nIDEvent)
	{
		KillTimer(2);

		OpenSerialPortAndTestCommunication();
	}
	else if (3 == nIDEvent)
	{
		ZeroProcess();
	}
	else if (4 == nIDEvent)
	{
		// 延时结束，开始查询调零结果
		KillTimer(4);
		SetTimer(3, 1000, NULL);

		// 延时结束,重新开启"停止"按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	}
	else if (5 == nIDEvent)
	{
		// 用于延时刷新数据
		KillTimer(5);

		Process();
	}
	else if (6 == nIDEvent)
	{
		// 延时结束，开始查询校准过程
		KillTimer(6);
		SetTimer(1, 1000, NULL);

		// 延时结束,重新开启"停止"按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	}


	CDialogZoom::OnTimer(nIDEvent);
}

void CNHNOxCalibrationDlg::CalibrationProcess(void)
{
	BYTE bResult(0);
	short sNO(0);
	short sNO2(0);
	float fCO2(0);

	if (ANA_WR_SUCCEED == m_pAnaThread->GasCalibration(&bResult, m_nCalChn, NULL, NULL, &fCO2, NULL, &sNO, &sNO2))
	{
		ASSERT(0x00==bResult || 0x01==bResult || 0x05==bResult);
		if (0x05 == bResult)
		{
			// 正在校准
			m_lbInfo.SetText(L"正在校准...");

			CString str;
			
			str.Format(L"%.2f", fCO2);
			m_lbCO2.SetText(str);
				
			// CO2测量值,单位:0.001%,放大1000倍,四舍五入
			if (fCO2 < 0.0f)
			{
				fCO2 = 0.0f;
			}
			m_nCO2MeasuredValue = (int)(fCO2 * 1000.0f + 0.5f);
			
			str.Format(L"%hd", sNO);
			m_lbNO.SetText(str);
				
			// NO测量值,单位:ppm
			if (sNO < 0)
			{
				sNO = 0;
			}
			m_nNOMeasuredValue = sNO;

			str.Format(L"%hd", sNO2);
			m_lbNO2.SetText(str);

			// NO2测量值,单位:ppm
			if (sNO2 < 0)
			{
				sNO2 = 0;
			}
			m_nNO2MeasuredValue = sNO2;
		}
		else if (0x00 == bResult)
		{
			// 校准成功
			KillTimer(1);

			m_bIsCalibrationPass = true;

			// 清楚实时数据显示
			ClearRTDisplay();

			// TODO:记录校准结果

			// 标记操作已经至少完成1次
			m_bIsOperationFinishedAtLeaseOnce = true;

			// 导出校准结果
			ExportGasCalibrationResult();

			// 回调输出信息
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体校准完成", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_ANA_GAS_CAL_FINISH, wchInfo, sizeof(wchInfo));
			}

			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));

			m_lbInfo.SetText(_T("气体校准成功"));

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
			
			if (((CButton *)GetDlgItem(IDC_CHECK_MP3))->GetCheck()==BST_CHECKED
				&& m_nStandardGasCur==m_nStandardGas4)
			{
				m_nStep = 1;
				m_nStandardGasCur = m_nStandardGas3;
				Process();
			}
			else if (((CButton *)GetDlgItem(IDC_CHECK_MP2))->GetCheck()==BST_CHECKED
				&& (m_nStandardGasCur==m_nStandardGas4||m_nStandardGasCur==m_nStandardGas3))
			{
				m_nStep = 1;
				m_nStandardGasCur = m_nStandardGas2;
				Process();
			}
			else if (((CButton *)GetDlgItem(IDC_CHECK_MP1))->GetCheck()==BST_CHECKED
				&& (m_nStandardGasCur==m_nStandardGas4||m_nStandardGasCur==m_nStandardGas3||m_nStandardGasCur==m_nStandardGas2))
			{
				m_nStep = 1;
				m_nStandardGasCur = m_nStandardGas1;
				Process();
			}
			else
			{
				// 重置步骤
				m_nStep = 0;

				// 启用按钮
				{
					GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
					GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
					GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

					GetDlgItem(IDC_RADIO_CHN_CO2_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_NO_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_NO2_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_NO_CO2_CAL)->EnableWindow(TRUE);
				}
			}
		}
		else if (0x01 == bResult)
		{
			// 校准失败
			KillTimer(1);

			m_bIsCalibrationPass = false;

			// 清楚实时数据显示
			ClearRTDisplay();

			// TODO:记录校准结果

			// 标记操作已经至少完成1次
			m_bIsOperationFinishedAtLeaseOnce = true;

			// 导出校准结果
			ExportGasCalibrationResult();

			// 回调输出信息
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体校准完成", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_ANA_GAS_CAL_FINISH, wchInfo, sizeof(wchInfo));
			}

			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));

			m_lbInfo.SetText(_T("气体校准失败"));

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
			
			// 启用按钮
			{
				GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

				GetDlgItem(IDC_RADIO_CHN_CO2_CAL)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_NO_CAL)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_NO2_CAL)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_NO_CO2_CAL)->EnableWindow(TRUE);
			}

			// 重置步骤
			m_nStep = 0;
		}
	}
	else
	{
		KillTimer(1);
		m_lbInfo.SetText(_T("校准失败"));

		// 重置流程
		ResetProcess();

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		
		// 启用按钮
		{
			GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

			GetDlgItem(IDC_RADIO_CHN_CO2_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_NO_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_NO2_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_NO_CO2_CAL)->EnableWindow(TRUE);
		}
	}
}

void CNHNOxCalibrationDlg::ZeroProcess(void)
{
	if (m_bCheckZeroStatus)
	{
		// 用状态命令查询调零过程

		BYTE bStatus(0);
		//if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		if (ANA_WR_SUCCEED == 0x00)
		{
			bStatus = 0x10;
			if (0x05 == bStatus)
			{
				// 正在调零
				m_lbInfo.SetText(L"正在调零...");
			}
			else if (0x10 == bStatus)
			{
				// 调零成功
				KillTimer(3);

				// 延时阶段,屏蔽按钮
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// 延时3秒
				// 因为收到调零结果后,调零指令还没有结束
				SetTimer(5, 3000, NULL);
			}
			else if (0x11 == bStatus)
			{
				// 调零失败(照样进入下一步)
				KillTimer(3);

				// 延时阶段,屏蔽按钮
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// 延时3秒
				// 因为收到调零结果后,调零指令还没有结束
				SetTimer(5, 3000, NULL);
			}
			else
			{
				// 返回未定义代码
				//KillTimer(3);
				TRACE("返回未定义代码\n");
			}
		}
		else
		{
			KillTimer(3);
			m_lbInfo.SetText(_T("调零失败"));

			// 重置流程
			ResetProcess();

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		// 用调零命令查询调零过程

		BYTE bResult;
		//if (ANA_WR_SUCCEED == m_pAnaThread->Zero(&bResult))
		if (ANA_WR_SUCCEED == 0x00)
		{
			bResult = 0x00;
			ASSERT(0x00==bResult || 0x01==bResult || 0x05==bResult);
			if (0x05 == bResult)
			{
				// 正在调零
				m_lbInfo.SetText(L"正在调零...");
			}
			else if (0x00 == bResult)
			{
				// 调零成功
				KillTimer(3);

				// 延时阶段,屏蔽按钮
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// 延时3秒
				// 因为收到调零结果后,调零指令还没有结束
				SetTimer(5, 3000, NULL);
			}
			else if (0x01 == bResult)
			{
				// 调零失败(照样进入下一步)
				KillTimer(3);

				// 延时阶段,屏蔽按钮
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// 延时3秒
				// 因为收到调零结果后,调零指令还没有结束
				SetTimer(5, 3000, NULL);
			}
		}
		else
		{
			KillTimer(3);
			m_lbInfo.SetText(_T("调零失败"));

			// 重置流程
			ResetProcess();

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
}

void CNHNOxCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
	// 读取分析仪协议类型,3：NHA503AB,4：统一通信协议
	CString strAnaProtocolVersion = si.GetString(L"AnaParams", L"ProtocolVersion", L"4");
	if (L"3" == strAnaProtocolVersion)
	{
		m_bCheckZeroStatus = false;
	}
	else
	{
		m_bCheckZeroStatus = true;
	}
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
	m_pAnaThread->IsOpen();
	if (true)
	{
		// 测试通讯
		BYTE bStatus(0);
		//if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		if (ANA_WR_SUCCEED == 0x00)
		{
			bStatus = 0x00;
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
					m_lbInfo.SetText(_T("分析仪气体校准"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x02:  // 预热
				{
					// 正在预热
					m_lbInfo.SetText(_T("正在预热.."));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x03:	// 检漏
				{
					// 正在检漏
					m_lbInfo.SetText(_T("正在检漏.."));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0xFF:
				{
					// 通讯错误,不在可通讯界面
					m_lbInfo.SetText(L"仪器不在实时检测状态");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			default:
				{
					// 通讯错误,不在可通讯界面
					m_lbInfo.SetText(L"仪器不在实时检测状态");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
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
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CNHNOxCalibrationDlg::Process(void)
{
	if (0 == m_nStep)
	{
		// 重置流程
		ResetProcess();

		//// 开始校准前,测试一次通讯
		//if (ANA_WR_SUCCEED != m_pAnaThread->Zero())
		//{
		//	m_lbInfo.SetText(_T("通讯超时"));
		//	return;
		//}

		m_lbInfo.SetText(_T("正在调零..."));
		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		// 延时阶段,屏蔽按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		// 延时2秒查询调零结果
		// 因为调零后,不能马上响应下一命令
		SetTimer(4, 2000, NULL);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		if (m_nStandardGasCur == m_nStandardGas4)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"填写4号校准气值");
			m_lbStep3.SetText(L"通入4号校准气");
			m_lbStep4.SetText(L"4号气校准");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}
		else if (m_nStandardGasCur == m_nStandardGas3)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"填写3号校准气值");
			m_lbStep3.SetText(L"通入3号校准气");
			m_lbStep4.SetText(L"3号气校准");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}
		else if (m_nStandardGasCur == m_nStandardGas2)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"填写2号校准气值");
			m_lbStep3.SetText(L"通入2号校准气");
			m_lbStep4.SetText(L"2号气校准");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}
		else if (m_nStandardGasCur == m_nStandardGas1)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"填写1号校准气值");
			m_lbStep3.SetText(L"通入1号校准气");
			m_lbStep4.SetText(L"1号气校准");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}

		//m_lbInfo.StopFlashText();
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		m_lbInfo.SetText(_T("请输入校准气值,然后下一步"));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查校准气标称值输入start
		if (0x00 != GetNominalValue())
		{
			// 标称值输入不符合要求
			return;
		}
		// 保存标称值到ini配置文件
		SaveNominalValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查校准气标称值输入stop

		//m_lbInfo.StopFlashText();

		// 写入校准气浓度
		//0x01 校准HC,CO,CO2
		//0x02 校准NO
		//0x04 校准NO2
		//0x03 校准HC,CO,CO2,NO
		//0x05 校准HC,CO,CO2,NO2
		//0x06 校准NO,NO2
		//0x07 校准HC,CO,CO2,NO,NO2
		BYTE bResult(0);
		if (m_nCalChn == CHN_NO)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, 0, m_nNONominalValue, 0);
		}
		else if (m_nCalChn == CHN_NO2)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, 0, 0, m_nNO2NominalValue);
		}
		else if (m_nCalChn == CHN_CO2_NO)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, m_nCO2NominalValue/1000.0f, m_nNONominalValue, 0);
		}
		else if (m_nCalChn == CHN_CO2)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, m_nCO2NominalValue/1000.0f, 0, 0);
		}

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		m_lbInfo.SetText(_T("请通入校准气,然后下一步"));

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		// 延时阶段,屏蔽按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		// 分析仪第一次接受校准命令后，返回结果估计要3秒后
		// 所以以下命令读取返回数据时肯定超时，但分析仪已经接受了校准命令，进入校准流程。
		BYTE bResult(0);	
		m_pAnaThread->GasCalibration(&bResult, m_nCalChn);

		// 延时3秒后开始查询校准过程
		SetTimer(6, 3000, NULL);
	}
}

void CNHNOxCalibrationDlg::ResetProcess(void)
{
	// 步骤显示框
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));
	m_lbStep4.SetTextColor(RGB(0,0,0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	// 步骤置零
	m_nStep = 0;
	// 校准结果置初始化值
	m_bIsCalibrationPass = true;
	
	// CO2测量值,单位:0.001%,放大1000倍,四舍五入
	m_nCO2MeasuredValue = 0;
	// NO测量值,单位:ppm
	m_nNOMeasuredValue = 0;
	// NO2测量值,单位:ppm
	m_nNO2MeasuredValue = 0;

	// 先校准4号气
	m_nStandardGasCur = m_nStandardGas4;
}

DWORD CNHNOxCalibrationDlg::GetNominalValue(void)
{
	CString strCO2, strNO, strNO2;

	//获取气体值并检查
	if (m_nCalChn==CHN_CO2 || m_nCalChn==CHN_CO2_NO)
	{
		GetDlgItem(IDC_EDIT_CO2_NOMINAL_VALUE)->GetWindowText(strCO2);
		strCO2.Trim();
		if (L"" == strCO2)
		{
			m_edCO2NominalValue.SetFocus();
			MessageBox(_T("请输入校准气CO2标称值"), _T("气体校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// 标准气CO2标称值,单位:0.001%(放到1000倍)
		float f = (float)_wtof(strCO2);
		m_nCO2NominalValue = int(f * 1000.0f + 0.5f);
		if (m_nCO2NominalValue <= 0)
		{
			m_edCO2NominalValue.SetFocus();
			MessageBox(_T("校准气CO2标称值不能为0"), _T("气体校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}

	if (m_nCalChn==CHN_NO || m_nCalChn==CHN_CO2_NO)
	{
		GetDlgItem(IDC_EDIT_NO_NOMINAL_VALUE)->GetWindowText(strNO);
		strNO.Trim();
		if (L"" == strNO)
		{
			m_edNONominalValue.SetFocus();
			MessageBox(_T("请输入校准气NO标称值"), _T("气体校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// 标准气NO标称值,单位:ppm
		m_nNONominalValue = _wtoi(strNO);
		if (m_nNONominalValue <= 0)
		{
			m_edNONominalValue.SetFocus();
			MessageBox(_T("校准气NO标称值不能为0"), _T("气体校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	
	if (m_nCalChn==CHN_NO2)
	{
		GetDlgItem(IDC_EDIT_NO2_NOMINAL_VALUE)->GetWindowText(strNO2);
		strNO2.Trim();
		if (L"" == strNO2)
		{
			m_edNO2NominalValue.SetFocus();
			MessageBox(_T("请输入校准气NO2标称值"), _T("气体校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// 标准气NO2标称值,单位:ppm
		m_nNO2NominalValue = _wtoi(strNO2);
		if (m_nNO2NominalValue <= 0)
		{
			m_edNO2NominalValue.SetFocus();
			MessageBox(_T("校准气NO2标称值不能为0"), _T("气体校准"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	
	return 0x00;
}

void CNHNOxCalibrationDlg::SaveNominalValueToIniFile(void)
{
	///////////////////////////////////////////////////////////// 记录到配置文件start
	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// 去除配置文件只读属性
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);
	CString str;
	str.Format(L"%.3f", m_nCO2NominalValue/1000.0f);
	si.SetString(L"AnaCal", L"CO2NominalValue", str);
	str.Format(L"%d", m_nNONominalValue);
	si.SetString(L"AnaCal", L"NONominalValue", str);
	str.Format(L"%d", m_nNO2NominalValue);
	si.SetString(L"AnaCal", L"NO2NominalValue", str);
	///////////////////////////////////////////////////////////// 记录到配置文件stop
}

void CNHNOxCalibrationDlg::ExportGasCalibrationRTData()
{
	// 目前为空，预留
}

void CNHNOxCalibrationDlg::ExportGasCalibrationResult()
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

	CString str;
	CString strResult(L"[ResultOfAnaGasCal]");
//////////////////////////////////////////////////////////////////////////////
	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 是否跳过HC通道校准
	str.Format(L"\r\nSkipHCCalibration=%s", L"1");
	strResult += str;

	// 是否跳过CO通道校准
	str.Format(L"\r\nSkipCOCalibration=%s", L"1");
	strResult += str;
	
	// 是否跳过CO2通道校准
	str.Format(L"\r\nSkipCO2Calibration=%s", (m_nCalChn==CHN_NO || m_nCalChn==CHN_NO2)?L"1":L"0");
	strResult += str;

	// CO2标称值,单位:%
	str.Format(L"\r\nCO2StandardValue=%.3f", m_nCO2NominalValue/1000.0f);
	strResult += str;

	// CO2测量值,单位:%
	str.Format(L"\r\nCO2MeasuredValue=%.3f", m_nCO2MeasuredValue/1000.0f);
	strResult += str;

	// 是否跳过NO通道校准
	str.Format(L"\r\nSkipNOCalibration=%s", (m_nCalChn==CHN_CO2 || m_nCalChn==CHN_NO2)?L"1":L"0");
	strResult += str;

	// NO标称值,单位:%
	str.Format(L"\r\nNOStandardValue=%d", m_nNONominalValue);
	strResult += str;

	// NO测量值,单位:%
	str.Format(L"\r\nNOMeasuredValue=%d", m_nNOMeasuredValue);
	strResult += str;

	// 是否跳过NO2通道校准
	str.Format(L"\r\nSkipNO2Calibration=%s", (m_nCalChn!=CHN_NO2)?L"1":L"0");
	strResult += str;

	// NO标称值,单位:%
	str.Format(L"\r\nNO2StandardValue=%d", m_nNO2NominalValue);
	strResult += str;

	// NO测量值,单位:%
	str.Format(L"\r\nNO2MeasuredValue=%d", m_nNO2MeasuredValue);
	strResult += str;

	// 是否跳过O2通道校准
	str.Format(L"\r\nSkipO2Calibration=%s", L"1");
	strResult += str;
	
	// 校准是否通过
	str.Format(L"\r\nPass=%s", m_bIsCalibrationPass?L"1":L"0");
	strResult += str;

	// 写入到结果ini文件
	CString strPathOfAnaGasCalibrationResult = strPathOfDataExchange + L"\\ResultOfAnaGasCal.ini";
	if (0 == _taccess(strPathOfAnaGasCalibrationResult, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfAnaGasCalibrationResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfAnaGasCalibrationResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfAnaGasCalibrationResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHNOxCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}