// FlowmeterFreeFlowCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FlowmeterFreeFlowCheckDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterFreeFlowCheckDlg 对话框

IMPLEMENT_DYNAMIC(CFlowmeterFreeFlowCheckDlg, CDialogZoom)

CFlowmeterFreeFlowCheckDlg::CFlowmeterFreeFlowCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterFreeFlowCheckDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)
, m_bIsPass(true)
, m_nTimesOfGettingData(0)
, m_nFluxMeasuredValue(0)
, m_nFreeFlowStandardValue(101)
, m_nRE(0)
, m_nRELimit(10)
, m_fMaximumZeroOffsetValue(36.7f)
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

	ZeroMemory(m_nFluxRTData, sizeof(m_nFluxRTData));

	// 导入参数
	ImportParams();

	//m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pFloThread = CNHEuqController::GetInstance().m_pFloThread;
}

CFlowmeterFreeFlowCheckDlg::~CFlowmeterFreeFlowCheckDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterFreeFlowCheckDlg::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);

	DDX_Control(pDX, IDC_EDIT_FREE_FLOW_STANDARD_VALUE, m_neFreeFlowStandardValue);
	DDX_Control(pDX, IDC_STATIC_MEASURED_VALUE, m_lbMeauredValue);
	DDX_Control(pDX, IDC_STATIC_RE, m_lbRE);
	DDX_Control(pDX, IDC_STATIC_RE_LIMIT, m_lbRELimit);
	DDX_Control(pDX, IDC_STATIC_JUDGEMENT, m_lbJudgement);
	DDX_Control(pDX, IDC_LIST_DATA, m_lcData);
}


BEGIN_MESSAGE_MAP(CFlowmeterFreeFlowCheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOff)
END_MESSAGE_MAP()


// CFlowmeterFreeFlowCheckDlg 消息处理程序

BOOL CFlowmeterFreeFlowCheckDlg::OnInitDialog()
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


void CFlowmeterFreeFlowCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterFreeFlowCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"流量计流量检查");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// 流量实时值
	m_lbFlux.SetLEDStyle();
	m_lbFlux.SetText(L"--");

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	// 初始化流量标准值
	CString str;
	str.Format(L"%.2f", m_nFreeFlowStandardValue/100.0f);
	m_neFreeFlowStandardValue.SetWindowText(str);
	// 初始化为不能在界面编辑标准值(可以从配置文件修改)
	m_neFreeFlowStandardValue.EnableWindow(FALSE);

	// 测量值显示框
	m_lbMeauredValue.SetBkColor(RGB(230, 230, 230));
	// 相对误差显示框
	m_lbRE.SetBkColor(RGB(230, 230, 230));
	// 相对误差限值显示框
	m_lbRELimit.SetBkColor(RGB(230, 230, 230));
	// 判定结果显示框
	m_lbJudgement.SetBkColor(RGB(230, 230, 230));

	m_lcData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcData.InsertColumn(0,	L"时间[s]",		LVCFMT_LEFT,	100*nSM_CXSCREEN/1280,	0);
	m_lcData.InsertColumn(1,	L"流量[L/s]",	LVCFMT_LEFT,	180*nSM_CXSCREEN/1280,	1);
	// 分隔
	m_lcData.InsertColumn(2,	L"",			LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	2);
	m_lcData.InsertColumn(3,	L"时间[s]",		LVCFMT_LEFT,	100*nSM_CXSCREEN/1280,	3);
	m_lcData.InsertColumn(4,	L"流量[L/s]",	LVCFMT_LEFT,	180*nSM_CXSCREEN/1280,	4);

	for(int i=0; i<10; i++)
	{
		str.Format(L"%d", i+1);
		m_lcData.InsertItem(i, str);

		str.Format(L"%d", i+11);
		m_lcData.SetItemText(i, 3, str);
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	EnableBasicOperation(false);
}

void CFlowmeterFreeFlowCheckDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("流量计流量检查"));
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
					m_lbInfo.SetText(_T("流量计流量检查"));
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

void CFlowmeterFreeFlowCheckDlg::OnTimer(UINT_PTR nIDEvent)
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
		// 获取20秒数据

		float f = m_pFloThread->GetVmasRTDFluxStd20Value();
		// 放大100倍，并四舍五入
		m_nFluxRTData[m_nTimesOfGettingData] = (int)(f * 100.0f + 0.5f);
		CString str;
		str.Format(L"%.2f", m_nFluxRTData[m_nTimesOfGettingData]/100.0f);
		if (m_nTimesOfGettingData < 10)
		{
			m_lcData.SetItemText(m_nTimesOfGettingData, 1, str);
		}
		else
		{
			m_lcData.SetItemText(m_nTimesOfGettingData-10, 4, str);
		}
		m_nTimesOfGettingData++;
		if (20 == m_nTimesOfGettingData)
		{
			// 20秒数据获取完毕
			KillTimer(3);

			// 继续执行流程
			Process();
		}
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	// 停止刷新实时数据
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// 关闭流量分析仪风机
	m_pFloThread->RunFan_Flowmeter(FALSE);

	// 重置流程
	ResetProcess();

	m_lbInfo.SetText(_T("流量检查停止"));
	
	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计流量检查停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_FLUXCHK_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码

	Process();
}

void CFlowmeterFreeFlowCheckDlg::Process(void)
{
	if (0 == m_nStep)
	{				
		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计流量检查开始", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCHK_START, wchInfo, sizeof(wchInfo));
		}

		// 拆去所有集气管

		// 重置流程
		ResetProcess();

		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("请拆去所有集气管,然后下一步"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		// 打开风机,等待气流稳定

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查标准值输入start
		if (0x00 != GetStandardValue())
		{
			// 标准值输入不符合要求
			return;
		}
		// 记录标准值到配置文件
		SaveStandardValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查标准值输入stop


		BeginWaitCursor();

		// 停止刷新实时数据
		RefreshRTData(false);

		// 打开流量分析仪风机
		m_pFloThread->RunFan_Flowmeter(TRUE);

		// 重启刷新实时数据
		RefreshRTData(true);

		EndWaitCursor();
		
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("请等待气流稳定，然后下一步"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		// 记录20S稳定气流读数

		// 数据获取次数置零
		m_nTimesOfGettingData = 0;

		// 获取20秒数据
		SetTimer(3, 1000, NULL);

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("正在获取数据..."));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		// 计算结果
		
		// 关闭流量分析仪风机
		m_pFloThread->RunFan_Flowmeter(FALSE);

		CalculateResult();
		DisplayResult();

		// 标记操作已经至少完成1次
		m_bIsOperationFinishedAtLeaseOnce = true;

		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("检查完成"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		// 导出气体检查结果
		ExportFloFluxChkResult();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"流量计流量检查完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));

		// 步骤置零
		m_nStep = 0;
	}
}

void CFlowmeterFreeFlowCheckDlg::ResetProcess(void)
{
	// 停止获取数据定时器
	KillTimer(3);

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
	
	// 清除结果
	ClearResult();

	// 步骤置零
	m_nStep = 0;
	// 检查结果置初始化值
	m_bIsPass = true;
	// 数据获取次数置零
	m_nTimesOfGettingData = 0;
	// 清除获取数据
	ZeroMemory(m_nFluxRTData, sizeof(m_nFluxRTData));
	// 清除测量值
	m_nFluxMeasuredValue = 0;
	// 清除相对误差
	m_nRE = 0;
}

DWORD CFlowmeterFreeFlowCheckDlg::GetStandardValue(void)
{
	// 获取标准值并检查
	CString strFreeFlowStandardValue;
	m_neFreeFlowStandardValue.GetWindowText(strFreeFlowStandardValue);
	strFreeFlowStandardValue.Trim();
	if (L"" == strFreeFlowStandardValue)
	{
		m_neFreeFlowStandardValue.SetFocus();
		MessageBox(_T("请输入流量标准值"), _T("流量检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// 流量标准值,单位:0.01L/s(放大100倍)
	float f = (float)_wtof(strFreeFlowStandardValue);
	ASSERT(f > -10e-6f);
	// 格式化为2为小数，并四舍五入
	m_nFreeFlowStandardValue = int(f * 100.0f + 0.5f);
	// 合理性约束(50L/s~500L/s)
	if (m_nFreeFlowStandardValue<5000 || m_nFreeFlowStandardValue>50000)
	{
		m_neFreeFlowStandardValue.SetFocus();
		MessageBox(_T("流量标准值不在合理范围(50L/s~500L/s)"), _T("流量检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CFlowmeterFreeFlowCheckDlg::SaveStandardValueToIniFile(void)
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
	str.Format(L"%.2f", m_nFreeFlowStandardValue/100.0f);
	si.SetString(L"FloDem", L"FreeFlowStandardValue", str);
}

void CFlowmeterFreeFlowCheckDlg::ImportParams(void)
{
	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	CString strDemarcationPath = strPath + L"\\Config\\Demarcation.ini";
	// 去除配置文件只读属性
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strDemarcationPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strDemarcationPath, dwAttributes);

	CSimpleIni si(strDemarcationPath);
	CString str;
	str = si.GetString(L"FloDem", L"FreeFlowStandardValue", L"");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nFreeFlowStandardValue = (int)(f * 100.0f + 0.5f);
	// 合理性约束(50L/s~500L/s)
	if (m_nFreeFlowStandardValue < 5000)
	{
		m_nFreeFlowStandardValue = 5000;
	}
	if (m_nFreeFlowStandardValue > 50000)
	{
		m_nFreeFlowStandardValue = 50000;
	}

	CString strEquipmentPath = strPath + L"\\Config\\Equipment.ini";
	CSimpleIni sio(strEquipmentPath);
	str = sio.GetString(L"FloParams", L"MaximumZeroOffsetValue", L"36.7");
	f = (float)_wtof(str);
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


void CFlowmeterFreeFlowCheckDlg::ExportFloFluxChkResult()
{
	CString str;
	CString strResult(L"[ResultOfFloFluxChk]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	strData.Format(L"标准值:%.2fL/s,测量值%.2fL/s", m_nFreeFlowStandardValue/100.0f, m_nFluxMeasuredValue/100.0f);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 标准值
	str.Format(L"\r\nFreeFlowStandardValue=%.2f", m_nFreeFlowStandardValue/100.0f);
	strResult += str;
	// 测量值
	str.Format(L"\r\nFluxMeasuredValue=%.2f", m_nFluxMeasuredValue/100.0f);
	strResult += str;

	// 误差
	CString strError;
	strError.Format(L"相对误差:%d%%(±%d%%)", m_nRE, m_nRELimit);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 是否通过
	str.Format(L"\r\nPass=%s", m_bIsPass ? L"1" : L"0");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxChk.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CFlowmeterFreeFlowCheckDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterFreeFlowCheckDlg::DisplayRTData(void)
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

void CFlowmeterFreeFlowCheckDlg::ClearRTDisplay(void)
{
	m_lbFlux.SetText(L"--");
}

void CFlowmeterFreeFlowCheckDlg::CalculateResult(void)
{
	// 计算平均值
	int nSum(0);
	for (int i=0; i<20; i++)
	{
		nSum += m_nFluxRTData[i];
	}
	m_nFluxMeasuredValue = (int)(nSum / 20.0f + 0.5f);

	// 计算误差
	m_nRE = (m_nFluxMeasuredValue - m_nFreeFlowStandardValue) * 100 / m_nFreeFlowStandardValue;

	if (abs(m_nRE) > m_nRELimit)
	{
		// Failed
		m_bIsPass = false;
	}
	else
	{
		// Succeed
		m_bIsPass = true;
	}
}

void CFlowmeterFreeFlowCheckDlg::DisplayResult(void)
{
	CString str;

	// 测量值显示框
	str.Format(L"%.2f", m_nFluxMeasuredValue/100.0f);
	m_lbMeauredValue.SetText(str);

	// 相对误差显示框
	str.Format(L"%d", m_nRE);
	m_lbRE.SetText(str);

	// 相对误差限值显示框
	str.Format(L"±%d", m_nRELimit);
	m_lbRELimit.SetText(str);

	// 判定结果显示框
	if (m_bIsPass)
	{
		m_lbJudgement.SetText(L"通过");

		m_lbRE.SetTextColor(RGB(0, 128, 0));
		m_lbJudgement.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbJudgement.SetText(L"不通过");

		m_lbRE.SetTextColor(RGB(255, 0, 0));
		m_lbJudgement.SetTextColor(RGB(255, 0, 0));
	}
}

void CFlowmeterFreeFlowCheckDlg::ClearResult(void)
{
	// 清空列表
	for (int i=0; i<10; i++)
	{
		m_lcData.SetItemText(i, 1, L"");

		m_lcData.SetItemText(i, 4, L"");
	}

	// 测量值显示框
	m_lbMeauredValue.SetText(L"--");
	m_lbMeauredValue.SetTextColor(RGB(0, 0, 0));

	// 相对误差显示框
	m_lbRE.SetText(L"--");
	m_lbRE.SetTextColor(RGB(0, 0, 0));

	// 相对误差限值显示框
	m_lbRELimit.SetText(L"--");
	m_lbRELimit.SetTextColor(RGB(0, 0, 0));

	// 判定结果显示框
	m_lbJudgement.SetText(L"--");
	m_lbJudgement.SetTextColor(RGB(0, 0, 0));
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOn()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOff()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterFreeFlowCheckDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterFreeFlowCheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}