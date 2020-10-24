// FlowmeterRealTimeDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterExternalAgreementCalDlg.h"
#include "afxdialogex.h"

// CFlowmeterExternalCalDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterExternalCalDlg, CDialogZoom)

CFlowmeterExternalCalDlg::CFlowmeterExternalCalDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterExternalCalDlg::IDD, pParent)
{
	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 19 / 1024;
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

	m_pFloThread = (CFlowThread*)AfxBeginThread(RUNTIME_CLASS(CFlowThread));
}

CFlowmeterExternalCalDlg::~CFlowmeterExternalCalDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pFloThread)
	{
		m_pFloThread->Close();
		m_pFloThread->QuitThread();
		WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	}
}

void CFlowmeterExternalCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_FLUX_STD0, m_lbStd0Flux);
	DDX_Control(pDX, IDC_STATIC_FLUX_STD20, m_lbStd20Flux);
	DDX_Control(pDX, IDC_STATIC_FLUX_ACT, m_lbActFlux);
	DDX_Control(pDX, IDC_STATIC_TEMP, m_lbTemp);
	DDX_Control(pDX, IDC_STATIC_PRES, m_lbPres);
	DDX_Control(pDX, IDC_STATIC_UDILO2, m_lbUDilO2);
	DDX_Control(pDX, IDC_EDIT_TP_CALIBRATION_VALUE, m_neTPCalibrationValue);
	DDX_Control(pDX, IDC_EDIT_TP_CALIBRATION_VALUE2, m_nePressureCalibrationValue);
	DDX_Control(pDX, IDC_EDIT_HIGH, m_neHighO2Value);
	DDX_Control(pDX, IDC_EDIT_MIDDLE, m_neMiddleO2Value);
	DDX_Control(pDX, IDC_EDIT_LOW, m_neLowO2Value);
	DDX_Control(pDX, IDC_EDIT_ZERO, m_neZeroO2Value);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_HIGH_FLUX2, m_neStandardValueHighFlux);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_LOW_FLUX, m_neStandardValueLowFlux);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_HIGH_FLUX3, m_neStandardValueSingleFlux);
	DDX_Control(pDX, IDC_RADIO_HIGH, m_rdoHigh);
	DDX_Control(pDX, IDC_RADIO_MIDDLE, m_rdoMiddle);
	DDX_Control(pDX, IDC_RADIO_LOW, m_rdoLow);
}


BEGIN_MESSAGE_MAP(CFlowmeterExternalCalDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterExternalCalDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterExternalCalDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterExternalCalDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_TP, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationTp)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_PRESS, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationPress)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_O2, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationO2)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_HIGH_FLUX, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationHighFlux)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_LOW_FLUX, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationLowFlux)
	ON_BN_CLICKED(IDC_BUTTON_RECOVERY, &CFlowmeterExternalCalDlg::OnBnClickedButtonRecovery)
	ON_BN_CLICKED(IDC_RADIO_HIGH, &CFlowmeterExternalCalDlg::OnBnClickedRadioHigh)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, &CFlowmeterExternalCalDlg::OnBnClickedRadioMiddle)
	ON_BN_CLICKED(IDC_RADIO_LOW, &CFlowmeterExternalCalDlg::OnBnClickedRadioLow)
	ON_BN_CLICKED(IDC_RADIO_ZERO, &CFlowmeterExternalCalDlg::OnBnClickedRadioZero)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_SINGLE_FLUX, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationSingleFlux)
END_MESSAGE_MAP()

BOOL CFlowmeterExternalCalDlg::OnInitDialog()
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


// CFlowmeterExternalCalDlg 消息处理程序

void CFlowmeterExternalCalDlg::InitCtrls(void)
{
	SetDlgFont();
	m_lbTitle.SetTitle(L"外部协议流量计校准");
	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	m_nPoint = 1;
	m_rdoHigh.SetCheck(TRUE);

	// 流量实时值;
	m_lbStd0Flux.SetLEDStyle();
	m_lbStd0Flux.SetText(L"--");
	m_lbStd20Flux.SetLEDStyle();
	m_lbStd20Flux.SetText(L"--");
	m_lbActFlux.SetLEDStyle();
	m_lbActFlux.SetText(L"--");

	// 温度
	m_lbTemp.SetLEDStyle();
	m_lbTemp.SetText(L"--");

	// 压力
	m_lbPres.SetLEDStyle();
	m_lbPres.SetText(L"--");

	// 稀释氧
	m_lbUDilO2.SetLEDStyle();
	m_lbUDilO2.SetText(L"--");

	// 初始化温度校准值为25
	m_neTPCalibrationValue.SetWindowText(L"25");
	// 初始化压力校准值为101kPa
	m_nePressureCalibrationValue.SetWindowText(L"101");
	// 初始化零点稀释氧校准值为101kPa
	m_neZeroO2Value.SetWindowText(L"20.8");

	EnableBasicOperation(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_TP)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_PRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_O2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_SINGLE_FLUX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(false);
}

void CFlowmeterExternalCalDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterExternalCalDlg::OpenSerialPortAndTestCommunication(void)
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

	BYTE bPort(0);
	// 获取通讯端口(从流量计获取)
	bPort = (BYTE)_wtoi(si.GetString(L"FloParams", L"Port", L"4"));

	m_pFloThread->Open(bPort);
	if (m_pFloThread->IsOpen())
	{	
		SetTimer(2,1000,NULL);
		// 就绪
		m_lbInfo.SetText(_T("流量计校准"));
		EnableBasicOperation(true);	
		GetDlgItem(IDC_BUTTON_CALIBRATION_TP)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_PRESS)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_O2)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_SINGLE_FLUX)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(true);
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		EnableBasicOperation(false);
	}
}

void CFlowmeterExternalCalDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterExternalCalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		KillTimer(1);


		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent && m_pFloThread->IsOpen())
	{
		float fFluxStd0;
		float fFluxStd20;
		float fFluxAct;
		float fTemp;
		float fPres;
		float fUDilO2;
		BYTE bResult(0);
		m_pFloThread->GetRealTimeData_Flowmeter(&fFluxStd0,&fFluxStd20,&fFluxAct,&fPres,&fUDilO2,&fTemp,&bResult);
		CString str;
		str.Format(L"%.2f",fFluxStd0);
		m_lbStd0Flux.SetText(str);
		str.Format(L"%.2f",fFluxStd20);
		m_lbStd20Flux.SetText(str);
		str.Format(L"%.2f",fFluxAct);
		m_lbActFlux.SetText(str);
		str.Format(L"%.2f",fPres);
		m_lbPres.SetText(str);
		str.Format(L"%.2f",fUDilO2);
		m_lbUDilO2.SetText(str);
		str.Format(L"%.2f",fTemp);
		m_lbTemp.SetText(str);
	}
	CDialogZoom::OnTimer(nIDEvent);

}


void CFlowmeterExternalCalDlg::OnBnClickedButtonFanOn()
{
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(true,&bResult);
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonFanOff()
{
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(false,&bResult);
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonQuit()
{
	KillTimer(2);
	OnCancel();
}


DWORD CFlowmeterExternalCalDlg::GetTpCalibrationValue(void)
{
	// 获取校准值并检查
	CString strTPCalibrationValue;
	m_neTPCalibrationValue.GetWindowText(strTPCalibrationValue);
	strTPCalibrationValue.Trim();
	if (L"" == strTPCalibrationValue)
	{
		m_neTPCalibrationValue.SetFocus();
		m_lbInfo.SetText(_T("请输入温度校准值"));
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

DWORD CFlowmeterExternalCalDlg::TpCalibrationProcess(void)
{
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(false,&bResult);
	m_pFloThread->SetCalibrationTemp_Flowmeter(&bResult, m_fTPCalValue);
	if (0x31 == bResult)
	{
		// 温度校准值成功
		return 0x00;	
	}
	else
	{
		// 温度校准值失败
		return 0x01;
	}

}

void CFlowmeterExternalCalDlg::ExportFloTPCalResult()
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
	str.Format(L"\r\nPass=%s", m_bIsTpCalibrationPass ? L"1" : L"0");
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

void CFlowmeterExternalCalDlg::RefreshRTData(bool bRefresh)
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


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationTp()
{
	if (0x00 != GetTpCalibrationValue())
	{
		// 校准值输入不符合要求
		return;
	}

	BeginWaitCursor();

	// 停止刷新实时数据
	RefreshRTData(false);

	if (0x00 == TpCalibrationProcess())
	{
		m_bIsTpCalibrationPass = true;
		// 校准成功
		m_lbInfo.SetText(_T("温度校准成功"));
	}
	else
	{
		m_bIsTpCalibrationPass = false;
		// 校准失败
		m_lbInfo.SetText(_T("温度校准失败"));
	}

	// 重启刷新实时数据
	RefreshRTData(true);
	EndWaitCursor();

	// 导出结果
	ExportFloTPCalResult();
}



DWORD CFlowmeterExternalCalDlg::GetPresCalibrationValue(void)
{
	// 获取校准值并检查
	CString strPressureCalibrationValue;
	m_nePressureCalibrationValue.GetWindowText(strPressureCalibrationValue);
	strPressureCalibrationValue.Trim();
	if (L"" == strPressureCalibrationValue)
	{
		m_nePressureCalibrationValue.SetFocus();
		m_lbInfo.SetText(_T("请输入压力校准值"));
		return 0x01;
	}
	// 校准值,单位:kPa
	m_fPressureCalibrationValue = (float)_wtof(strPressureCalibrationValue);
	// 格式化为2为小数
	m_fPressureCalibrationValue = int(m_fPressureCalibrationValue * 100.0f + 0.5f) / 100.0f;
	if (m_fPressureCalibrationValue>150.0f || m_fPressureCalibrationValue<50.0f)
	{
		m_nePressureCalibrationValue.SetFocus();
		MessageBox(_T("压力校准值不在合理范围(50kPa~150kPa)"), _T("压力校准"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

DWORD CFlowmeterExternalCalDlg::PresCalibrationProcess(void)
{
	// 压力过程由以下步骤组成：
	// 1)关闭风机
	// 2)设置压力校准值
	// 3)压力校准
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(false,&bResult);
	m_pFloThread->SetCalibrationPres_Flowmeter(&bResult, m_fPressureCalibrationValue);
	if (0x31 == bResult)
	{
		// 写入压力校准值成功
		return 0x00;
	}
	else
	{
		// 写入压力校准值失败
		return 0x01;
	}
}

void CFlowmeterExternalCalDlg::ExportFloPresCalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloPresCal]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	strData.Format(L"标准值:%.2fkPa", m_fPressureCalibrationValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 误差
	CString strError;
	strError.Format(L"绝对误差:0.01kPa");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 是否通过
	str.Format(L"\r\nPass=%s", m_bIsPresCalibrationPass ? L"1" : L"0");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloPresCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}



void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationPress()
{
	if (0x00 != GetPresCalibrationValue())
	{
		// 校准值输入不符合要求
		return;
	}

	BeginWaitCursor();

	// 停止刷新实时数据
	RefreshRTData(false);

	if (0x00 == PresCalibrationProcess())
	{
		m_bIsPresCalibrationPass = true;

		// 校准成功
		m_lbInfo.SetText(_T("压力校准成功"));
	}
	else
	{
		m_bIsPresCalibrationPass = false;

		// 校准失败
		m_lbInfo.SetText(_T("压力校准失败"));
	}

	// 重启刷新实时数据
	RefreshRTData(true);

	EndWaitCursor();

	// 导出结果
	ExportFloPresCalResult();
}




DWORD CFlowmeterExternalCalDlg::CalibrationProcess_High(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	CString str;
	m_neHighO2Value.GetWindowTextW(str);
	float fHighValue = (float)_wtof(str);
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_High(&bResult, fHighValue);
	if (0x31 == bResult)
	{
		// 稀释氧校准完成
		return 0x00;
	}
	else
	{
		// 稀释氧校准失败
		return 0x01;
	}
}

DWORD CFlowmeterExternalCalDlg::CalibrationProcess_Middle(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	CString str;
	m_neMiddleO2Value.GetWindowTextW(str);
	float fMiddleValue = (float)_wtof(str);
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Middle(&bResult, fMiddleValue);
	if (0x31 == bResult)
	{
		// 稀释氧校准完成
		return 0x00;
	}
	else
	{
		// 稀释氧校准失败
		return 0x01;
	}
}

DWORD CFlowmeterExternalCalDlg::CalibrationProcess_Low(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	CString str;
	m_neLowO2Value.GetWindowTextW(str);
	float fLowValue = (float)_wtof(str);
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Low(&bResult, fLowValue);
	if (0x31 == bResult)
	{
		// 稀释氧校准完成
		return 0x00;
	}
	else
	{
		// 稀释氧校准失败
		return 0x01;
	}


}

void CFlowmeterExternalCalDlg::ExportFloDilO2CalResult()
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
	str.Format(L"\r\nPass=%s", m_bIsO2CalibrationPass ? L"1" : L"0");
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



DWORD CFlowmeterExternalCalDlg::DilO2CalibrationProcess(void)
{
	// 稀释氧过程由以下步骤组成：
	// 1)设置稀释氧校准值
	// 2)稀释氧校准
	// 3)等待氧气校准完成
	// 4)获取校准结果

	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Zero(&bResult);
	if (0x31 == bResult)
	{
		
		// 稀释氧校准成功
		return 0x00;
			
	}
	else
	{
		// 写入稀释氧校准失败
		return 0x01;
	}
}

void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationO2()
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


	BeginWaitCursor();

	// 停止刷新实时数据
	RefreshRTData(false);

	EnableBasicOperation(false);

	if(m_nPoint == 1)
	{
		if (0x00 == CalibrationProcess_High())
		{
			m_bIsO2CalibrationPass = true;

			// 校准成功
			m_lbInfo.SetText(_T("高点稀释氧校准成功"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// 校准失败
			m_lbInfo.SetText(_T("高点稀释氧校准失败"));
		}
	}

	else if(m_nPoint == 2)
	{
		if (0x00 == CalibrationProcess_Middle())
		{
			m_bIsO2CalibrationPass = true;

			// 校准成功
			m_lbInfo.SetText(_T("中点稀释氧校准成功"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// 校准失败
			m_lbInfo.SetText(_T("中点稀释氧校准失败"));
		}
	}

	else if(m_nPoint == 3)
	{
		if (0x00 == CalibrationProcess_Low())
		{
			m_bIsO2CalibrationPass = true;

			// 校准成功
			m_lbInfo.SetText(_T("低点稀释氧校准成功"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// 校准失败
			m_lbInfo.SetText(_T("低点稀释氧校准失败"));
		}
	}
	else if(m_nPoint == 4)
	{
		if (0x00 == DilO2CalibrationProcess())
		{
			m_bIsO2CalibrationPass = true;

			// 校准成功
			m_lbInfo.SetText(_T("零点稀释氧校准成功"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// 校准失败
			m_lbInfo.SetText(_T("零点稀释氧校准失败"));
		}
	}

	EnableBasicOperation(true);

	// 重启刷新实时数据
	RefreshRTData(true);
	EndWaitCursor();

	// 导出结果
	ExportFloDilO2CalResult();	
}

DWORD CFlowmeterExternalCalDlg::GetStandardValueOfLowFlux(void)
{
	// 获取标准值并检查
	CString strStandardValue;
	m_neStandardValueLowFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueLowFlux.SetFocus();
		m_lbInfo.SetText(_T("请输入低流量校准值"));
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
DWORD CFlowmeterExternalCalDlg::GetStandardValueOfSingleFlux(void)
{
	// 获取标准值并检查
	CString strStandardValue;
	m_neStandardValueSingleFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueSingleFlux.SetFocus();
		m_lbInfo.SetText(_T("请输入单点流量校准值"));
		return 0x01;
	}
	// 流量标准值,单位:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// 格式化为2为小数，并四舍五入
	m_fStandardValueOfSingleFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// 合理性约束(0L/s~500L/s)
	if ((m_fStandardValueOfSingleFlux<50.0f || m_fStandardValueOfSingleFlux>235.0f))
	{
		m_neStandardValueSingleFlux.SetFocus();
		MessageBox(_T("单点流量标准值不在合理范围(50L/s~235L/s)"), _T("流量校准"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}





DWORD CFlowmeterExternalCalDlg::GetStandardValueOfHighFlux(void)
{
	// 获取标准值并检查
	CString strStandardValue;
	m_neStandardValueHighFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueHighFlux.SetFocus();
		m_lbInfo.SetText(_T("请输入高流量校准值"));
		return 0x01;
	}
	// 流量标准值,单位:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// 格式化为2为小数，并四舍五入
	m_fStandardValueOfHighFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// 合理性约束:通用高流量(50L/s~500L/s) 南华单点(50L/s~235L/s)
	if (m_fStandardValueOfHighFlux<50.0f || m_fStandardValueOfHighFlux>500.0f)
	{
		m_neStandardValueHighFlux.SetFocus();
		MessageBox(_T("高流量标准值不在合理范围(50L/s~500L/s)"), _T("流量校准"), MB_OK|MB_ICONWARNING);

		return 0x01;
	}
	
	return 0x00;
}


void CFlowmeterExternalCalDlg::ImportParams(void)
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

void CFlowmeterExternalCalDlg::ExportFloFluxCalResult(CString strName,float fValue,BOOL bPass)
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
	strData.Format(L"%s校准值:%.2fL/s",strName, fValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 是否通过
	str.Format(L"\r\nPass=%s", bPass ? L"1" : L"0");
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


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationSingleFlux()
{
	
	// 获取单点流量标准值
	if (0x00 != GetStandardValueOfSingleFlux())
	{
		return;
	}

	BeginWaitCursor();

	// 停止刷新数据
	RefreshRTData(false);

	CString strName(L"");
	strName.Format(L"单点流量");

	// 写入校准值并校准
	BYTE bResult(0);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CNanhuaNHF1::SinglePointFlowCal, m_fStandardValueOfSingleFlux);
	if (0x31 == bResult)
	{
		// 单点流量校准成功
		m_lbInfo.SetText(L"单点流量校准成功");
		m_bIsFlowCalibrationPass = true;
	}
	else
	{

		// 单点流量校准失败
		m_lbInfo.SetText(L"单点流量校准成功");
		m_bIsFlowCalibrationPass = false;
	}
	// 导出结果
	ExportFloFluxCalResult(strName,m_fStandardValueOfSingleFlux,m_bIsFlowCalibrationPass);
	// 开始刷新数据
	RefreshRTData(true);
	EndWaitCursor();

	
	
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationHighFlux()
{
	// 获取高流量标准值
	if (0x00 != GetStandardValueOfHighFlux())
	{
		return;
	}

	BeginWaitCursor();

	// 停止刷新数据
	RefreshRTData(false);

	CString strName(L"");
	strName.Format(L"高流量");

	// 写入校准值并校准
	BYTE bResult(0);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CNanhuaNHF1::HighFlowCal, m_fStandardValueOfHighFlux);
	if (0x31 == bResult)
	{
		// 高点流量校准成功
		m_lbInfo.SetText(L"高流量校准成功");
		m_bIsFlowCalibrationPass = true;
		
	}
	else
	{
		// 高点流量标准值失败
		m_lbInfo.SetText(L"高流量校准失败");
		m_bIsFlowCalibrationPass = false;
	}
	// 导出结果
	ExportFloFluxCalResult(strName,m_fStandardValueOfHighFlux,m_bIsFlowCalibrationPass);
	// 开始刷新数据
	RefreshRTData(true);

	EndWaitCursor();
	
	
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationLowFlux()
{

	// 获取低流量标准值
	if (0x00 != GetStandardValueOfLowFlux())
	{
		return;
	}

	BeginWaitCursor();

	// 停止刷新数据
	RefreshRTData(false);

	CString strName(L"");
	strName.Format(L"低流量");
	// 写入校准值并校准
	BYTE bResult(0x00);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CNanhuaNHF1::LowFlowCal, m_fStandardValueOfLowFlux);
	if (0x31 == bResult)
	{
		// 低流量校准成功
		m_lbInfo.SetText(L"低流量校准成功");
		m_bIsFlowCalibrationPass = true;
	}
	else
	{
		// 低流量校准失败
		m_lbInfo.SetText(L"低流量校准失败");
		m_bIsFlowCalibrationPass = false;
	}
	// 导出结果
	ExportFloFluxCalResult(strName,m_fStandardValueOfLowFlux,m_bIsFlowCalibrationPass);
	// 开始刷新数据
	RefreshRTData(true);

	EndWaitCursor();
	
	
}


DWORD CFlowmeterExternalCalDlg::RestoreFactorySettings(void)
{

	BYTE bResult(0);
	m_pFloThread->RestoreFactorySettings_Flowmeter(&bResult);
	if (0x31 == bResult)
	{
		// 温度校准值成功
		return 0x00;	
	}
	else
	{
		// 温度校准值失败
		return 0x01;
	}
}

void CFlowmeterExternalCalDlg::OnBnClickedButtonRecovery()
{
	if (0x00 == RestoreFactorySettings())
	{
		m_lbInfo.SetText(_T("恢复出厂设置成功"));
	}
	else
	{
		m_lbInfo.SetText(_T("恢复出厂设置失败"));
	}
}

void CFlowmeterExternalCalDlg::OnBnClickedRadioHigh()
{
	m_nPoint = 1;
}


void CFlowmeterExternalCalDlg::OnBnClickedRadioMiddle()
{
	m_nPoint = 2;
}


void CFlowmeterExternalCalDlg::OnBnClickedRadioLow()
{
	m_nPoint = 3;
}

void CFlowmeterExternalCalDlg::OnBnClickedRadioZero()
{
	m_nPoint = 4;
}


