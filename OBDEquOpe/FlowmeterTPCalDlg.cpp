// FlowmeterTPCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterTPCalDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterTPCalDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterTPCalDlg, CDialogZoom)

CFlowmeterTPCalDlg::CFlowmeterTPCalDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CFlowmeterTPCalDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)
, m_bIsCalibrationPass(true)
, m_fTPCalValue(101.0f)
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

CFlowmeterTPCalDlg::~CFlowmeterTPCalDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterTPCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_STATIC_TP, m_lbTP);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);

	DDX_Control(pDX, IDC_EDIT_TP_CALIBRATION_VALUE, m_neTPCalibrationValue);
}


BEGIN_MESSAGE_MAP(CFlowmeterTPCalDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterTPCalDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterTPCalDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterTPCalDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterTPCalDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterTPCalDlg::OnBnClickedButtonFanOff)
END_MESSAGE_MAP()


// CFlowmeterTPCalDlg 消息处理程序

BOOL CFlowmeterTPCalDlg::OnInitDialog()
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


void CFlowmeterTPCalDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterTPCalDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"流量计温度校准");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// 温度实时值
	m_lbTP.SetLEDStyle();
	m_lbTP.SetText(L"--");

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);

	// 初始化温度校准值为25
	// 校准值编辑框
	m_neTPCalibrationValue.AllowNegative(FALSE);
	m_neTPCalibrationValue.SetMaxDecimalPlaces(1);
	m_neTPCalibrationValue.AllowNegative(1);
	m_neTPCalibrationValue.SetWindowText(L"25");

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	EnableBasicOperation(false);
}

void CFlowmeterTPCalDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("流量计温度校准"));
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
					m_lbInfo.SetText(_T("流量计温度校准"));
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

void CFlowmeterTPCalDlg::OnTimer(UINT_PTR nIDEvent)
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

void CFlowmeterTPCalDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	// 停止刷新实时数据
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterTPCalDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	// 重置流程
	ResetProcess();

	m_lbInfo.SetText(_T("温度校准停止"));

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计温度校准停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_TPCAL_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CFlowmeterTPCalDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码

	Process();
}

void CFlowmeterTPCalDlg::Process(void)
{
	if (0 == m_nStep)
	{
				
		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计温度校准开始", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_TPCAL_START, wchInfo, sizeof(wchInfo));
		}

		// 重置流程
		ResetProcess();

		m_lbInfo.SetText(_T("请输入温度校准值,然后下一步"));
		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查校准值输入start
		if (0x00 != GetCalibrationValue())
		{
			// 校准值输入不符合要求
			return;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查校准值输入stop

		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		BeginWaitCursor();

		// 停止刷新实时数据
		RefreshRTData(false);

		if (0x00 == CalibrationProcess())
		{
			m_bIsCalibrationPass = true;

			// 校准成功
			m_lbInfo.SetText(_T("温度校准成功"));
		}
		else
		{
			m_bIsCalibrationPass = false;

			// 校准失败
			m_lbInfo.SetText(_T("温度校准失败"));
		}

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();

		// 标记操作已经至少完成1次
		m_bIsOperationFinishedAtLeaseOnce = true;

		// 导出结果
		ExportFloTPCalResult();
		
		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计温度校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_TPCAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));

		//m_lbInfo.SetText(_T("温度校准完成"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		// 重置步骤
		m_nStep = 0;
	}
}


void CFlowmeterTPCalDlg::ResetProcess(void)
{
	// 步骤显示框
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);

	// 步骤置零
	m_nStep = 0;
	// 校准结果置初始化值
	m_bIsCalibrationPass = true;

	// 温度校准值
	m_fTPCalValue = 101.0f;
}

DWORD CFlowmeterTPCalDlg::GetCalibrationValue(void)
{
	// 获取校准值并检查
	CString strTPCalibrationValue;
	m_neTPCalibrationValue.GetWindowText(strTPCalibrationValue);
	strTPCalibrationValue.Trim();
	if (L"" == strTPCalibrationValue)
	{
		m_neTPCalibrationValue.SetFocus();
		MessageBox(_T("请输入温度校准值"), _T("温度校准"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// 校准值,单位:℃
	m_fTPCalValue = (float)_wtof(strTPCalibrationValue);
	// 格式化为2为小数
	m_fTPCalValue = int(m_fTPCalValue * 100.0f + 0.5f) / 100.0f;

	if (m_fTPCalValue>150.0f || m_fTPCalValue<-50.0f)
	{
		m_neTPCalibrationValue.SetFocus();
		MessageBox(_T("温度校准值不在合理范围(-50℃~150℃)"), _T("温度校准"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

DWORD CFlowmeterTPCalDlg::CalibrationProcess(void)
{
	// 温度过程由以下步骤组成：
	// 1)关闭风机
	// 2)设置温度校准值
	// 3)温度校准

	m_pFloThread->RunFan_Flowmeter(false);

	BYTE bResult(0);
	m_pFloThread->SetCalibrationTemp_Flowmeter(&bResult, m_fTPCalValue);
	if (0x06 == bResult)
	{
		// 写入温度校准值成功

		BYTE bCalResult(0);
		m_pFloThread->TempCalibration_Flowmeter(&bResult, &bCalResult);
		if (0x06==bResult && 0x00==bCalResult)
		{
			// 温度校准成功

			return 0x00;
		}
		else
		{
			// 温度校准失败

			return 0x01;
		}
	}
	else
	{
		// 写入温度校准值失败

		return 0x01;
	}
}

void CFlowmeterTPCalDlg::ExportFloTPCalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloTPCal]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	strData.Format(L"标准值:%.2f℃", m_fTPCalValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 误差
	CString strError;
	strError.Format(L"绝对误差:0.01℃");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 是否通过
	str.Format(L"\r\nPass=%s", m_bIsCalibrationPass ? L"1" : L"0");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloTPCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CFlowmeterTPCalDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterTPCalDlg::DisplayRTData(void)
{
	m_pFloThread->GetRealTimeData_Flowmeter_Aysn();

	float f = m_pFloThread->GetVmasRTDGasTempValue();
	CString str;
	str.Format(L"%.2f", f);
	m_lbTP.SetText(str);
}

void CFlowmeterTPCalDlg::ClearRTDisplay(void)
{
	m_lbTP.SetText(L"--");
}

void CFlowmeterTPCalDlg::OnBnClickedButtonFanOn()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterTPCalDlg::OnBnClickedButtonFanOff()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterTPCalDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterTPCalDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}