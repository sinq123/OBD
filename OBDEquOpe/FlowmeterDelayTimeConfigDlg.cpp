// FlowmeterDelayTimeConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterDelayTimeConfigDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterDelayTimeConfigDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterDelayTimeConfigDlg, CDialogZoom)

CFlowmeterDelayTimeConfigDlg::CFlowmeterDelayTimeConfigDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CFlowmeterDelayTimeConfigDlg::IDD, pParent)
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

CFlowmeterDelayTimeConfigDlg::~CFlowmeterDelayTimeConfigDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterDelayTimeConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_ED_HCCOCO2_VALUE, m_neHCCOCO2Value);
	DDX_Control(pDX, IDC_ED_O2_VALUE, m_neO2Value);
	DDX_Control(pDX, IDC_ED_NOX_VALUE, m_neNOxValue);
}


BEGIN_MESSAGE_MAP(CFlowmeterDelayTimeConfigDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterDelayTimeConfigDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BTN_HCCOCO2_READ, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Read)
	ON_BN_CLICKED(IDC_BTN_O2_READ, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Read)
	ON_BN_CLICKED(IDC_BTN_NOX_READ, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxRead)
	ON_BN_CLICKED(IDC_BTN_HCCOCO2_WRITE, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Write)
	ON_BN_CLICKED(IDC_BTN_O2_WRITE, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Write)
	ON_BN_CLICKED(IDC_BTN_NOX_WRITE, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxWrite)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_CONFIG, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnDefaultConfig)
END_MESSAGE_MAP()


// CFlowmeterDelayTimeConfigDlg 消息处理程序

BOOL CFlowmeterDelayTimeConfigDlg::OnInitDialog()
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

void CFlowmeterDelayTimeConfigDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterDelayTimeConfigDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"流量计通道延时设置");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	m_neHCCOCO2Value.AllowNegative(FALSE);
	m_neHCCOCO2Value.SetMaxDecimalPlaces(0);
	m_neHCCOCO2Value.SetLimitText(2);
	m_neO2Value.AllowNegative(FALSE);
	m_neO2Value.SetMaxDecimalPlaces(0);
	m_neO2Value.SetLimitText(2);
	m_neNOxValue.AllowNegative(FALSE);
	m_neNOxValue.SetMaxDecimalPlaces(0);
	m_neNOxValue.SetLimitText(2);
}

void CFlowmeterDelayTimeConfigDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterDelayTimeConfigDlg::OpenSerialPortAndTestCommunication(void)
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

					// 就绪
					m_lbInfo.SetText(_T("流量计通道延时设置"));
					// 位于实时通讯界面
					EnableOperation(true);
				}
				break;
			case 0x05:
				{
					// 通讯正常，但暂未切换到流量计操作模式
					// 原因：流量计忙

					// 就绪
					m_lbInfo.SetText(_T("流量计通道延时设置"));
					// 位于实时通讯界面
					EnableOperation(true);
				}
				break;
			case 0x15:
				{
					// 通讯错误：返回0x15
					m_lbInfo.SetText(_T("通讯错误"));
					EnableOperation(false);
				}
				break;
			default:
				{
					// 通讯错误：不可识别代码
					m_lbInfo.SetText(L"通讯错误");
					EnableOperation(false);
				}
				break;
			}
		}
		else
		{
			// 通讯失败
			m_lbInfo.SetText(L"通讯超时");
			EnableOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		EnableOperation(false);
	}

#ifdef _DEBUG
	EnableOperation(true);
#endif
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CFlowmeterDelayTimeConfigDlg::EnableOperation(bool bEnable)
{
	GetDlgItem(IDC_BTN_HCCOCO2_WRITE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_O2_WRITE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_NOX_WRITE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_HCCOCO2_READ)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_O2_READ)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_NOX_READ)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DEFAULT_CONFIG)->EnableWindow(bEnable);
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Read()
{
	// TODO: 在此添加控件通知处理程序代码
	float fHCCOCO2_DelayTime(0.0f);
	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->GetHCCOCO2DelayTime_Flowmeter(&bResult, &fHCCOCO2_DelayTime))
	{
		if (0x06 == bResult)
		{
			CString str;
			str.Format(L"%.2f", fHCCOCO2_DelayTime);
			GetDlgItem(IDC_ED_HCCOCO2_VALUE)->SetWindowTextW(str);
		}
		else
		{
			MessageBoxW(L"读取HC/CO/CO2通道延时值失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"读取HC/CO/CO2通道延时值失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Read()
{
	// TODO: 在此添加控件通知处理程序代码
	float fO2_DelayTime(0.0f);
	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->GetO2DelayTime_Flowmeter(&bResult, &fO2_DelayTime))
	{
		if (0x06 == bResult)
		{
			CString str;
			str.Format(L"%.2f", fO2_DelayTime);
			GetDlgItem(IDC_ED_O2_VALUE)->SetWindowTextW(str);
		}
		else
		{
			MessageBoxW(L"读取O2通道延时值失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"读取O2通道延时值失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxRead()
{
	// TODO: 在此添加控件通知处理程序代码
	float fNOx_DelayTime(0.0f);
	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->GetNOxDelayTime_Flowmeter(&bResult, &fNOx_DelayTime))
	{
		if (0x06 == bResult)
		{
			CString str;
			str.Format(L"%.2f", fNOx_DelayTime);
			GetDlgItem(IDC_ED_NOX_VALUE)->SetWindowTextW(str);
		}
		else
		{
			MessageBoxW(L"读取NOx通道延时值失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"读取NOx通道延时值失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Write()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_neHCCOCO2Value.GetWindowTextW(str);
	float fDelayTime = (float)_wtof(str);
	if (fDelayTime<10e-6 || fDelayTime>15.0f)
	{
		MessageBoxW(L"延时值应在0～15之间取值", L"通道延时设置", MB_OK|MB_ICONWARNING);
		m_neHCCOCO2Value.SetFocus();
		return;
	}

	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->SetHCCOCO2DelayTime_Flowmeter(&bResult, fDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->HCCOCO2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 == bCalResult)
				{
					MessageBoxW(L"HC/CO/CO2通道延时值写入成功", L"通道延时设置", MB_OK);
				}
				else
				{
					MessageBoxW(L"HC/CO/CO2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
				}
			}
			else
			{
				MessageBoxW(L"HC/CO/CO2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
			}
		}
		else
		{
			MessageBoxW(L"HC/CO/CO2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"HC/CO/CO2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Write()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_neO2Value.GetWindowTextW(str);
	float fDelayTime = (float)_wtof(str);
	if (fDelayTime<10e-6 || fDelayTime>15.0f)
	{
		MessageBoxW(L"延时值应在0～15之间取值", L"通道延时设置", MB_OK|MB_ICONWARNING);
		m_neO2Value.SetFocus();
		return;
	}

	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->SetO2DelayTime_Flowmeter(&bResult, fDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->O2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 == bCalResult)
				{
					MessageBoxW(L"O2通道延时值写入成功", L"通道延时设置", MB_OK);
				}
				else
				{
					MessageBoxW(L"O2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
				}
			}
			else
			{
				MessageBoxW(L"O2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
			}
		}
		else
		{
			MessageBoxW(L"O2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"O2通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_neNOxValue.GetWindowTextW(str);
	float fDelayTime = (float)_wtof(str);
	if (fDelayTime<10e-6 || fDelayTime>15.0f)
	{
		MessageBoxW(L"延时值应在0～15之间取值", L"通道延时设置", MB_OK|MB_ICONWARNING);
		m_neNOxValue.SetFocus();
		return;
	}

	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->SetNOxDelayTime_Flowmeter(&bResult, fDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->NOxDelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 == bCalResult)
				{
					MessageBoxW(L"NOx通道延时值写入成功", L"通道延时设置", MB_OK);
				}
				else
				{
					MessageBoxW(L"NOx通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
				}
			}
			else
			{
				MessageBoxW(L"NOx通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
			}
		}
		else
		{
			MessageBoxW(L"NOx通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"NOx通道延时值写入失败", L"通道延时设置", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnDefaultConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK != MessageBoxW(L"确定现在恢复通道延时值默认设置？", L"通道延时设置", MB_OKCANCEL|MB_ICONWARNING))
	{
		return;
	}

	// 通道延时默认值初始化
	float fHCCOCO2DelayTime(5.0f);
	float fO2DelayTime(6.0f);
	float fNOxDelayTime(7.0f);

	bool bIsOK_HCCOCO2(true);
	bool bIsOK_O2(true);
	bool bIsOK_NOx(true);

	BYTE bResult(0);
	// 写HC/CO/CO2通道延时默认值
	if (ANA_WR_SUCCEED == m_pFloThread->SetHCCOCO2DelayTime_Flowmeter(&bResult, fHCCOCO2DelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->HCCOCO2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 != bCalResult)
				{
					bIsOK_HCCOCO2 = false;
				}
			}
			else
			{
				bIsOK_HCCOCO2 = false;
			}
		}
		else
		{
			bIsOK_HCCOCO2 = false;
		}
	}
	else
	{
		bIsOK_HCCOCO2 = false;
	}

	// 写O2通道延时默认值
	if (ANA_WR_SUCCEED == m_pFloThread->SetO2DelayTime_Flowmeter(&bResult, fO2DelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->O2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 != bCalResult)
				{
					bIsOK_O2 = false;
				}
			}
			else
			{
				bIsOK_O2 = false;
			}
		}
		else
		{
			bIsOK_O2 = false;
		}
	}
	else
	{
		bIsOK_O2 = false;
	}

	// 写NOx通道延时默认值
	if (ANA_WR_SUCCEED == m_pFloThread->SetNOxDelayTime_Flowmeter(&bResult, fNOxDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->NOxDelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 != bCalResult)
				{
					bIsOK_NOx = false;
				}
			}
			else
			{
				bIsOK_NOx = false;
			}
		}
		else
		{
			bIsOK_NOx = false;
		}
	}
	else
	{
		bIsOK_NOx = false;
	}

	CString str(L"");
	if (!bIsOK_HCCOCO2)
	{
		str += L"● HC/CO/CO2通道延时默认值写入失败\n\n";
	}
	if (!bIsOK_O2)
	{
		str += L"● O2通道延时默认值写入失败\n\n";
	}
	if (!bIsOK_NOx)
	{
		str += L"● NOx通道延时默认值写入失败\n\n";
	}

	if (bIsOK_HCCOCO2 && bIsOK_O2 && bIsOK_NOx)
	{
		MessageBoxW(L"通道延时默认值设置成功", L"通道延时设置", MB_OK);
	}
	else
	{
		MessageBoxW(str, L"通道延时设置", MB_OK|MB_ICONWARNING);
	}	
}
