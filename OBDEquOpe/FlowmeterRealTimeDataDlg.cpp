// FlowmeterRealTimeDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterRealTimeDataDlg.h"
#include "afxdialogex.h"


// CFlowmeterRealTimeDataDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterRealTimeDataDlg, CDialogZoom)

CFlowmeterRealTimeDataDlg::CFlowmeterRealTimeDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterRealTimeDataDlg::IDD, pParent)
{
	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 40 / 1024;
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

CFlowmeterRealTimeDataDlg::~CFlowmeterRealTimeDataDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterRealTimeDataDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CFlowmeterRealTimeDataDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterRealTimeDataDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()

BOOL CFlowmeterRealTimeDataDlg::OnInitDialog()
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


// CFlowmeterRealTimeDataDlg 消息处理程序

void CFlowmeterRealTimeDataDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"流量计实时数据");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();


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

	EnableBasicOperation(false);
}

void CFlowmeterRealTimeDataDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterRealTimeDataDlg::OpenSerialPortAndTestCommunication(void)
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
					SetTimer(2,1000,NULL);

					// 就绪
					m_lbInfo.SetText(_T("流量计实时数据"));

					EnableBasicOperation(true);
				}
				break;
			case 0x05:
				{
					// 通讯正常，但暂未切换到流量计操作模式
					// 原因：流量计忙
					SetTimer(2,1000,NULL);
					// 就绪
					m_lbInfo.SetText(_T("流量计实时数据"));

					EnableBasicOperation(true);
				}
				break;
			case 0x15:
				{
					// 通讯错误：返回0x15
					m_lbInfo.SetText(_T("通讯错误"));

					EnableBasicOperation(false);
				}
				break;
			default:
				{
					// 通讯错误：不可识别代码
					m_lbInfo.SetText(L"通讯错误");

					EnableBasicOperation(false);
				}
				break;
			}
		}
		else
		{
			// 通讯失败
			m_lbInfo.SetText(L"通讯超时");

			EnableBasicOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));

		EnableBasicOperation(false);
	}
}

void CFlowmeterRealTimeDataDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterRealTimeDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		WORD status0;
		WORD status1;
		WORD status2;
		WORD status3;
		WORD status4;
		WORD status5;
		WORD status6;
		WORD status7;
		WORD status8;
		WORD status9;
		WORD status10;
		WORD status11;
		WORD status12;
		WORD status13;
		WORD status14;
		WORD status15;
		WORD status16;
		WORD status17;
		WORD status18;
		WORD status19;
		WORD status20;
		WORD status21;
		WORD status22;
		WORD status23;
		WORD status24;
		WORD status25;
		float fFluxStd0;
		float fFluxStd20;
		float fFluxAct;
		float fTemp;
		float fPres;
		float fUDilO2;

		m_pFloThread->GetRealTimeData_Flowmeter_ALL(&status0,&status1,&status2,&status3,&status4,&status5,&status6,&status7,&status8,&status9,&status10,&status11,&status12,&status13,&status14,&status15,
		&status16,&status17,&status18,&status19,&status20,&status21,&status22,&status23,&status24,&status25,&fFluxStd0,&fFluxStd20,&fFluxAct,&fPres,&fUDilO2,&fTemp);
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


void CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOn()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFloThread->RunFan_Flowmeter(true);
}


void CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOff()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFloThread->RunFan_Flowmeter(false);
}


void CFlowmeterRealTimeDataDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(2);
	OnCancel();
}
