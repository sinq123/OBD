
// OBDOperationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OBDOperationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COBDOperationDlg 对话框




COBDOperationDlg::COBDOperationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(COBDOperationDlg::IDD, pParent)
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
	
	m_pAdapterThread = CNHEuqController::GetInstance().m_pOBDThread;
	if (m_pAdapterThread->GetInstrumentModel() == COBDAdapterThread::IM_ZHENGDE)
	{
		//m_pAdapterThread->UnInit();	// 通知结束
		m_pAdapterThread->ReOpen();	// 重新连接
	}

	m_nFuelType = 1;

	::AfxInitRichEdit();
}

COBDOperationDlg::~COBDOperationDlg()
{
	m_fontDlgFont.DeleteObject();
}

void COBDOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_PROTOCOL, m_lbProtocol);
	DDX_Control(pDX, IDC_STATIC_VELOCITY, m_lbVelocity);
	DDX_Control(pDX, IDC_STATIC_REV, m_lbRev);
	DDX_Control(pDX, IDC_STATIC_COOLANT_TEMP, m_lbCoolantTemp);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL, m_cbProtocol);
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_recLog);
}

BEGIN_MESSAGE_MAP(COBDOperationDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &COBDOperationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_VCI_VERSION, &COBDOperationDlg::OnBnClickedBtnVciVersion)
	ON_BN_CLICKED(IDC_BTN_VIN, &COBDOperationDlg::OnBnClickedBtnVIN)
	ON_BN_CLICKED(IDC_BTN_ECU_INFO, &COBDOperationDlg::OnBnClickedBtnEcuInfo)
	ON_BN_CLICKED(IDC_BTN_INIT, &COBDOperationDlg::OnBnClickedBtnInit)
	ON_MESSAGE(WM_OBD_INIT, &COBDOperationDlg::OnInit)
	ON_BN_CLICKED(IDC_BTN_GET_IUPR, &COBDOperationDlg::OnBnClickedBtnGetIupr)
	ON_BN_CLICKED(IDC_BTN_GET_RTDATA, &COBDOperationDlg::OnBnClickedBtnGetRtdata)
	ON_BN_CLICKED(IDC_RADIO_GASOLINE, &COBDOperationDlg::OnBnClickedRadioFuelType)
	ON_BN_CLICKED(IDC_RADIO_DIESEL, &COBDOperationDlg::OnBnClickedRadioFuelType)
	ON_BN_CLICKED(IDC_BTN_LAMP_STATE, &COBDOperationDlg::OnBnClickedBtnLampState)
	ON_BN_CLICKED(IDC_BTN_OBD_TYPE, &COBDOperationDlg::OnBnClickedBtnObdType)
	ON_BN_CLICKED(IDC_BTN_GET_DTC, &COBDOperationDlg::OnBnClickedBtnGetDtc)
END_MESSAGE_MAP()


// COBDOperationDlg 消息处理程序

BOOL COBDOperationDlg::OnInitDialog()
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

void COBDOperationDlg::OnPaint()
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

void COBDOperationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"OBD基本操作");

	m_lbInfo.SetText(L"车辆熄火，接上VCI再点火");
	m_lbInfo.SetTitleStyle();

	m_lbVelocity.SetTitleStyle();
	m_lbRev.SetTitleStyle();
	m_lbCoolantTemp.SetTitleStyle();
	
	for (int i=0; i<11; i++)
	{
		CString strProtocol;
		strProtocol.Format(L"%02d", i);
		int nIndex = m_cbProtocol.AddString(strProtocol);
		m_cbProtocol.SetItemData(nIndex, i);
	}

	((CButton *)GetDlgItem(IDC_RADIO_GASOLINE))->SetCheck(m_nFuelType==1);
	((CButton *)GetDlgItem(IDC_RADIO_DIESEL))->SetCheck(m_nFuelType==2);
}

void COBDOperationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void COBDOperationDlg::WriteLog(LPCTSTR szLog)
{
	m_recLog.SetSel(-1, -1);
	m_recLog.ReplaceSel(szLog);
	m_recLog.LineScroll(1, SB_BOTTOM);
	m_recLog.SetFocus();
}

void COBDOperationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		CString strLog;
		if (m_pAdapterThread->IsOpen())
		{
			if (m_pAdapterThread->IsAdapterOnline())
			{
				strLog.Format(L"VCI在线");
				KillTimer(nIDEvent);
			}
			else
			{
				strLog.Format(L"车辆熄火，接上VCI再点火");
			}
			GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_INIT)->EnableWindow(TRUE);
		}
		else
		{
			strLog.Format(L"打开通讯端口失败");
			
			GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_INIT)->EnableWindow(FALSE);
		}
		m_lbInfo.SetText(strLog);
	}
	else if (2 == nIDEvent)
	{
/*		m_pAdapterThread->GetRTData_Asyn();

		CString strTemp;
		strTemp.Format(L"%.1f", m_pAdapterThread->GetVelocityValue());
		m_lbVelocity.SetText(strTemp);

		strTemp.Format(L"%d", m_pAdapterThread->GetEngineRevValue());
		m_lbRev.SetText(strTemp);

		strTemp.Format(L"%.1f", m_pAdapterThread->GetCoolantTempValue());
		m_lbCoolantTemp.SetText(strTemp);
*/
		std::map<std::wstring, std::wstring> mapData;
		if (m_pAdapterThread->GetRTData(mapData))
		{
			m_lbVelocity.SetText(mapData[L"Velocity"].c_str());
			m_lbRev.SetText(mapData[L"EngineRev"].c_str());
			m_lbCoolantTemp.SetText(mapData[L"CoolantTemp"].c_str());

			CString strLog;
			if (m_nFuelType == 1)
			{
				//strLog.AppendFormat(L"车速:%.1fkm/h,", (float)_wtof(mapData[L"Velocity"].c_str()));
				//strLog.AppendFormat(L"转速:%drpm,", _wtoi(mapData[L"EngineRev"].c_str()));
				//strLog.AppendFormat(L"水温:%.1f℃,", (float)_wtof(mapData[L"CoolantTemp"].c_str()));
				strLog.AppendFormat(L"节气门开度:%.1f%%,", (float)_wtof(mapData[L"SolarTermDoorPosition"].c_str()));
				strLog.AppendFormat(L"计算负荷值:%.1f%%,", (float)_wtof(mapData[L"CalculationLoad"].c_str()));
				strLog.AppendFormat(L"过量空气系数:%.2f,", (float)_wtof(mapData[L"AirCoefficient"].c_str()));
				strLog.AppendFormat(L"前传感器电压:%.2fmV,", (float)_wtof(mapData[L"ForwardSensorV"].c_str()));
				strLog.AppendFormat(L"前传感器电流:%.2fmA,", (float)_wtof(mapData[L"ForwardSensorI"].c_str()));
				strLog.AppendFormat(L"进气量:%.1fg/s,", (float)_wtof(mapData[L"MAF"].c_str()));
				strLog.AppendFormat(L"进气压力:%.1fkpa", (float)_wtof(mapData[L"MAP"].c_str()));
			}
			else if (m_nFuelType == 2)
			{
				//strLog.AppendFormat(L"车速:%.1fkm/h,", (float)_wtof(mapData[L"Velocity"].c_str()));
				//strLog.AppendFormat(L"转速:%drpm,", _wtoi(mapData[L"EngineRev"].c_str()));
				//strLog.AppendFormat(L"水温:%.1f℃,", (float)_wtof(mapData[L"CoolantTemp"].c_str()));
				strLog.AppendFormat(L"油门开度:%.1f%%,", (float)_wtof(mapData[L"ThrottleOpening"].c_str()));
				strLog.AppendFormat(L"进气量:%.1fg/s,", (float)_wtof(mapData[L"MAF"].c_str()));
				strLog.AppendFormat(L"发动机输出功率:%.1fkw,", (float)_wtof(mapData[L"EngineOutputPower"].c_str()));
				strLog.AppendFormat(L"增压压力:%.1fkpa,", (float)_wtof(mapData[L"ChargeAirPressure"].c_str()));
				strLog.AppendFormat(L"耗油量:%.1fL/100km,", (float)_wtof(mapData[L"FuelConsumption"].c_str()));
				strLog.AppendFormat(L"氮氧传感器浓度:%dppm,", _wtoi(mapData[L"NOXConcentration"].c_str()));
				strLog.AppendFormat(L"尿素喷射量:%.1fmL/s,", (float)_wtof(mapData[L"UreaInjectionVolume"].c_str()));
				strLog.AppendFormat(L"排气温度:%.1f℃,", (float)_wtof(mapData[L"EGT"].c_str()));
				strLog.AppendFormat(L"颗粒捕集器压差:%.1fKpa,", (float)_wtof(mapData[L"DPFDifferentialPressure"].c_str()));
				strLog.AppendFormat(L"EGR开度:%.1f%%,", (float)_wtof(mapData[L"EGRPosition"].c_str()));
				strLog.AppendFormat(L"燃油喷射压力:%dbar", _wtoi(mapData[L"FuelDeliveryPressure"].c_str()));
			}
			WriteLog(strLog);
			WriteLog(L"\r\n");
		}
		else
		{
			WriteLog(L"获取实时数据流失败\r\n");
		}
	}
	else if (nIDEvent == 3)
	{
		CString strLog;
		strLog.Format(L"正在建立通讯...%d", m_unInitTimeout);
		m_lbInfo.SetText(strLog);		
		m_unInitTimeout++;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void COBDOperationDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);	

	if (m_pAdapterThread->GetInstrumentModel() == COBDAdapterThread::IM_ZHENGDE)
	{
		m_pAdapterThread->UnInit();	// 通知结束
	}

	OnCancel();
}

void COBDOperationDlg::OnBnClickedBtnVciVersion()
{
	CString strTemp;
	std::wstring strVer;
	if (m_pAdapterThread->GetSoftwareVer(strVer))
	{
		WriteLog(L"获取VIC版本成功\r\n");
		strTemp.Format(L"%s\r\n", strVer.c_str());
		WriteLog(strTemp);
	}
	else
	{
		WriteLog(L"获取VIC版本失败\r\n");
	}
}

void COBDOperationDlg::OnBnClickedBtnVIN()
{
	std::wstring strVIN;
	std::wstring strVINHex;
	int nCode = 0;
	CString strTemp;
	if (m_pAdapterThread->GetInstrumentModel() == COBDAdapterThread::IM_ZHENGDE)
	{
		if (m_pAdapterThread->GetVIN(strVIN, strVINHex, &nCode))
		{
			strTemp.Format(L"获取VIN码成功(%d)\r\n", nCode);
			strTemp.AppendFormat(L"%s\r\n", strVIN.c_str());
			WriteLog(strTemp);
		}
		else
		{
			if (nCode == 2)
			{
				strTemp.Format(L"获取VIN码成功，非标准格式(%d)\r\n", nCode);
				strTemp.AppendFormat(L"字符：%s\r\n", strVIN.c_str());
				strTemp.AppendFormat(L"十六进制数据：%s\r\n", strVINHex.c_str());
				WriteLog(strTemp);
			}
			else
			{
				strTemp.Format(L"获取VIN码失败(%d)\r\n", nCode);
				WriteLog(strTemp);
			}
		}
	}
	else
	{
		if (m_pAdapterThread->GetVIN(strVIN, strVINHex))
		{
			WriteLog(L"获取VIN码成功\r\n");
			strTemp.Format(L"%s\r\n", strVIN.c_str());
			WriteLog(strTemp);
		}
		else
		{
			WriteLog(L"获取VIN码失败\r\n");
		}
	}
}

void COBDOperationDlg::OnBnClickedBtnEcuInfo()
{
	std::wstring strVer;
	
	int nCode = 0;
	std::map<std::wstring, std::wstring> mapData;
	CString strTemp;
	if (m_pAdapterThread->GetInstrumentModel() == COBDAdapterThread::IM_ZHENGDE)
	{
		if (m_pAdapterThread->GetECUInfo(mapData, &nCode))
		{
			WriteLog(L"获取ECU信息成功\r\n");
			strTemp.Format(L"%s,%s    (%d)\r\n", mapData[L"EngineCALID"].c_str(), mapData[L"EngineCVN"].c_str(), nCode);
			WriteLog(strTemp);
		}
		else
		{
			WriteLog(L"获取ECU信息失败\r\n");
			strTemp.Format(L"--    (%d)\r\n", nCode);
			WriteLog(strTemp);
		}
	}
	else
	{
		if (m_pAdapterThread->GetECUInfo(mapData))
		{			
			WriteLog(L"获取ECU信息成功\r\n");
			strTemp.Format(L"%s,%s\r\n", mapData[L"EngineCALID"].c_str(), mapData[L"EngineCVN"].c_str());
			WriteLog(strTemp);
		}
		else
		{
			WriteLog(L"获取ECU信息失败\r\n");
		}
	}
}


void COBDOperationDlg::OnBnClickedBtnLampState()
{
	CString strTemp;
	
	bool bIsOn = false;
	if (m_pAdapterThread->GetLampState(bIsOn))
	{			
		WriteLog(L"获取故障指示灯状态成功\r\n");
		strTemp.Format(L"%s\r\n", bIsOn?L"点亮":L"熄灭");
		WriteLog(strTemp);
	}
	else
	{
		WriteLog(L"获取故障指示灯状态失败\r\n");
	}
}


void COBDOperationDlg::OnBnClickedBtnObdType()
{
	CString strTemp;
	std::wstring strType;
	if (m_pAdapterThread->GetOBDType(strType))
	{			
		WriteLog(L"获取OBD型式检验要求成功\r\n");
		strTemp.Format(L"%s\r\n", strType.c_str());
		WriteLog(strTemp);
	}
	else
	{
		WriteLog(L"获取OBD型式检验要求失败\r\n");
	}
}


void COBDOperationDlg::OnBnClickedBtnGetDtc()
{
	CString strTemp;
	std::vector<SDTCData> vtDTC;
	if (m_pAdapterThread->GetDTC(vtDTC))
	{			
		WriteLog(L"获取故障码成功\r\n");
		if (vtDTC.size() == 0)
		{
			WriteLog(L"无故障码\r\n");
		}
		else
		{
			CString strDTC;
			for (UINT i=0; i<vtDTC.size(); i++)
			{
				//DWORD dwCode = vtDTC[i].dwCode;
				//if (dwCode != 0x00)
				//{
				//	BYTE bType = (dwCode&0xC000)>>14;
				//	WORD wValue = dwCode&0x3FFF;
				//	switch (bType)
				//	{
				//	case 0:	strDTC.AppendFormat(L"P%04X,", wValue);	break;
				//	case 1:	strDTC.AppendFormat(L"C%04X,", wValue);	break;
				//	case 2:	strDTC.AppendFormat(L"B%04X,", wValue);	break;
				//	case 3:	strDTC.AppendFormat(L"U%04X,", wValue);	break;
				//	default: strDTC.AppendFormat(L"%04X,", dwCode);break;
				//	}
				//}
				strDTC.AppendFormat(L"[%s],%s\r\n", vtDTC[i].strCode.c_str(), vtDTC[i].strComment.c_str());
			}
			WriteLog(strDTC + L"\r\n");
		}
	}
	else
	{
		WriteLog(L"获取故障码失败\r\n");
	}
}

void COBDOperationDlg::OnBnClickedBtnGetIupr()
{
	int nCode = 0;
	CString strTemp;
	std::map<std::wstring, std::wstring> mapData;
	if (m_pAdapterThread->GetIUPR(mapData, &nCode))
	{
		CString strData;
		strData.AppendFormat(L"		监视完成次数	监视条件符合次数	IUPR率\r\n");
		int nNumerator = 0, nDenominator = 0; 
		if (m_nFuelType == 1)
		{
			//strData.AppendFormat(L"OMCEC:%s,", mapData[L"OMCEC"].c_str());
			//strData.AppendFormat(L"ICC:%s,", mapData[L"ICC"].c_str());
			nNumerator = _wtoi(mapData[L"CMCECB1"].c_str());
			nDenominator = _wtoi(mapData[L"CMCCB1"].c_str());
			strData.AppendFormat(L"催化器组1:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"CMCECB2"].c_str());
			nDenominator = _wtoi(mapData[L"CMCCB2"].c_str());
			strData.AppendFormat(L"催化器组2:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);

			nNumerator = _wtoi(mapData[L"O2SMCECB1"].c_str());
			nDenominator = _wtoi(mapData[L"O2SMCCB1"].c_str());
			strData.AppendFormat(L"前氧传感器组1:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);

			nNumerator = _wtoi(mapData[L"O2SMCECB2"].c_str());
			nDenominator = _wtoi(mapData[L"O2SMCCB2"].c_str());
			strData.AppendFormat(L"前氧传感器组2:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"RO2SMCECB1"].c_str());
			nDenominator = _wtoi(mapData[L"RO2SMCCB1"].c_str());
			strData.AppendFormat(L"后氧传感器组1:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"RO2SMCECB2"].c_str());
			nDenominator = _wtoi(mapData[L"RO2SMCCB2"].c_str());
			strData.AppendFormat(L"后氧传感器组2:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"EVAPEC"].c_str());
			nDenominator = _wtoi(mapData[L"EVAPC"].c_str());
			strData.AppendFormat(L"EVAP:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"EGREC"].c_str());
			nDenominator = _wtoi(mapData[L"EGRC"].c_str());
			strData.AppendFormat(L"EGR和/或VVT:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"GPFEC1"].c_str());
			nDenominator = _wtoi(mapData[L"GPFC1"].c_str());
			strData.AppendFormat(L"GPF组1:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"GPFEC2"].c_str());
			nDenominator = _wtoi(mapData[L"GPFC2"].c_str());
			strData.AppendFormat(L"GPF组2:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"AMCEC"].c_str());
			nDenominator = _wtoi(mapData[L"AMCCC"].c_str());
			strData.AppendFormat(L"二次空气喷射系统:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
		}
		else if (m_nFuelType == 2)
		{
			nNumerator = _wtoi(mapData[L"NMHCEC"].c_str());
			nDenominator = _wtoi(mapData[L"NMHCC"].c_str());
			strData.AppendFormat(L"NMHC催化器:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
								
			nNumerator = _wtoi(mapData[L"NOXCEC"].c_str());
			nDenominator = _wtoi(mapData[L"NOXCC"].c_str());
			strData.AppendFormat(L"NOX催化器:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"NOXAEC"].c_str());
			nDenominator = _wtoi(mapData[L"NOXAC"].c_str());
			strData.AppendFormat(L"NOX吸附器:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"PMEC"].c_str());
			nDenominator = _wtoi(mapData[L"PMC"].c_str());
			strData.AppendFormat(L"PM捕集器:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
								
			nNumerator = _wtoi(mapData[L"WSEC"].c_str());
			nDenominator = _wtoi(mapData[L"WSC"].c_str());
			strData.AppendFormat(L"废气传感器:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
								
			nNumerator = _wtoi(mapData[L"EGREC"].c_str());
			nDenominator = _wtoi(mapData[L"EGRC"].c_str());
			strData.AppendFormat(L"EGR和/或VVT:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
				
			nNumerator = _wtoi(mapData[L"PPEC"].c_str());
			nDenominator = _wtoi(mapData[L"PPC"].c_str());
			strData.AppendFormat(L"增压压力:%d	%d	%.2f\r\n", nDenominator, nNumerator, nDenominator==0?0:1.0f*nNumerator/nDenominator);
		}
		strTemp.Format(L"%s", strData);

		WriteLog(L"获取IUPR数据成功\r\n");
		WriteLog(strTemp);
	}
	else
	{
		WriteLog(L"获取IUPR数据失败\r\n");
	}
}

void COBDOperationDlg::OnBnClickedBtnInit()
{
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);

	CString strProtocol;
	m_cbProtocol.GetWindowText(strProtocol);

	m_pAdapterThread->Init_Asyn(_wtoi(strProtocol), m_nFuelType, m_hWnd);
	SetTimer(3, 1000, NULL);
	m_unInitTimeout = 1;

	GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_INIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_VIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ECU_INFO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_GET_IUPR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LAMP_STATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_OBD_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_GET_DTC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_GET_RTDATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_GET_RTDATA)->SetWindowText(L"读取数据流");

	m_lbVelocity.SetText(L"--");
	m_lbRev.SetText(L"--");
	m_lbCoolantTemp.SetText(L"--");
}

LRESULT COBDOperationDlg::OnInit(WPARAM wParam, LPARAM lParam)
{
	KillTimer(3);
	CString strLog;
	if ((bool)wParam)
	{
		DWORD dwRet = (DWORD)lParam;
		strLog.Format(L"建立通讯成功，协议版本%02d", dwRet);
		CString strProtocol;
		strProtocol.Format(L"%02d-%s", dwRet, m_pAdapterThread->GetOBDProtocolName(dwRet).c_str());
		m_lbProtocol.SetText(strProtocol);
		
		//SetTimer(2, 1000, NULL);

		GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_VIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ECU_INFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GET_IUPR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LAMP_STATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_OBD_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GET_DTC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GET_RTDATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_INIT)->EnableWindow(TRUE);
	}
	else
	{
		strLog.Format(L"失败，熄火12秒后重试");

		GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_VIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ECU_INFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GET_IUPR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_LAMP_STATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_OBD_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GET_DTC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GET_RTDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_INIT)->EnableWindow(TRUE);
	}
	m_lbInfo.SetText(strLog);

	return 0x00;
}


void COBDOperationDlg::OnBnClickedBtnGetRtdata()
{
	CString strText;
	GetDlgItem(IDC_BTN_GET_RTDATA)->GetWindowText(strText);
	if (strText == L"停止")
	{
		KillTimer(2);
		GetDlgItem(IDC_BTN_INIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_GASOLINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DIESEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_VIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ECU_INFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GET_IUPR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LAMP_STATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_OBD_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GET_DTC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GET_RTDATA)->SetWindowText(L"读取数据流");
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		m_lbVelocity.SetText(L"--");
		m_lbRev.SetText(L"--");
		m_lbCoolantTemp.SetText(L"--");
	}
	else
	{
		SetTimer(2, 1000, NULL);
		GetDlgItem(IDC_BTN_INIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_GASOLINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DIESEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_VCI_VERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_VIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ECU_INFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GET_IUPR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_LAMP_STATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_OBD_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GET_DTC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GET_RTDATA)->SetWindowText(L"停止");
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	}
}

void COBDOperationDlg::OnBnClickedRadioFuelType()
{
	if (((CButton *)GetDlgItem(IDC_RADIO_GASOLINE))->GetCheck() == BST_CHECKED)
	{
		m_nFuelType = 1;
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_DIESEL))->GetCheck() == BST_CHECKED)
	{
		m_nFuelType = 2;
	}
}