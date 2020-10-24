// NHCDIWDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCDIWDlg.h"
#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif
// CNHCDIWDlg 对话框

IMPLEMENT_DYNAMIC(CNHCDIWDlg, CDialogZoom)

CNHCDIWDlg::CNHCDIWDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCDIWDlg::IDD, pParent)
	, m_fLowerForce(0.0f)
	, m_fUpperForce(0.0f)
	, m_fLowerVelocity(0.0f)
	, m_fUpperVelocity(0.0f)
	, m_unLowerSlipingTime(0)
	, m_unUpperSlipingTime(0)
	, m_usDIW(0)
	, m_usMeasuredDIW(0)
	, m_fAveLowerForce(110.0f)
	, m_fAveUpperForce(1110.0f)
	, m_wAp(0)
	, m_wBp(0)
	, m_wCp(0)
{
	// 启动测功机控制线程
	m_pNHCDIWThread = (CNHCDIWThread*)AfxBeginThread(RUNTIME_CLASS(CNHCDIWThread));

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

	// 从配置文件获取加载力和滑行区间
	////////////////////////////////////////////////////////// 从配置文件获取加载力和滑行区间begin
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
	str = si.GetString(L"DynDIW", L"LowerForce", L"110");
	m_fLowerForce = (float)_tstof(str);
	str = si.GetString(L"DynDIW", L"UpperForce", L"1110");
	m_fUpperForce = (float)_tstof(str);
	ASSERT(m_fUpperForce>m_fLowerForce);
	str = si.GetString(L"DynDIW", L"LowerVelocity", L"16");
	m_fLowerVelocity = (float)_tstof(str);
	str = si.GetString(L"DynDIW", L"UpperVelocity", L"48");
	m_fUpperVelocity = (float)_tstof(str);
	ASSERT(m_fUpperVelocity>m_fLowerVelocity);
	////////////////////////////////////////////////////////// 从配置文件获取加载力和滑行区间end
	
	//// 写死参数（发给沈阳）
	//m_fLowerForce = 110;
	//m_fUpperForce = 1110;
	//m_fLowerVelocity = 16.0f;
	//m_fUpperVelocity = 48.0f;
}

CNHCDIWDlg::~CNHCDIWDlg()
{
	m_fontDlgFont.DeleteObject();

	// 重新恢复寄生功率参数
	if (0 != m_wAp)
	{
		m_pNHCDIWThread->WriteVar(39, m_wAp);
	}
	if (0 != m_wBp)
	{
		m_pNHCDIWThread->WriteVar(40, m_wBp);
	}
	if (0 != m_wCp)
	{
		m_pNHCDIWThread->WriteVar(41, m_wCp);
	}

	if (NULL != m_pNHCDIWThread)
	{
		m_pNHCDIWThread->Close();
		m_pNHCDIWThread->QuitThread();
		WaitForSingleObject(m_pNHCDIWThread->m_hThread, 5000);
	}
}

void CNHCDIWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_F, m_lbF);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_EDIT_LOWER_FORCE, m_edLowerForce);
	DDX_Control(pDX, IDC_EDIT_UPPER_FORCE, m_edUpperForce);
	DDX_Control(pDX, IDC_EDIT_LOWER_VELOCITY, m_edLowerVelocity);
	DDX_Control(pDX, IDC_EDIT_UPPER_VELOCITY, m_edUpperVelocity);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_LOWER_SLIPINGTIME, m_lbLowerSlipingTime);
	DDX_Control(pDX, IDC_STATIC_UPPER_SLIPINGTIME, m_lbUpperSlipingTime);
	DDX_Control(pDX, IDC_STATIC_AVE_LOWER_FORCE, m_lbAveLowerForce);
	DDX_Control(pDX, IDC_STATIC_AVE_UPPER_FORCE, m_lbAveUpperForce);
	DDX_Control(pDX, IDC_STATIC_MEASURED_DIW, m_lbMeasuredDIW);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lb4);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCDIWDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_TEST, &CNHCDIWDlg::OnBnClickedButtonStartTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCDIWDlg::OnBnClickedButtonStopTest)
	ON_BN_CLICKED(IDCANCEL, &CNHCDIWDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DIW, &CNHCDIWDlg::OnBnClickedButtonWriteDiw)
END_MESSAGE_MAP()


// CNHCDIWDlg 消息处理程序

BOOL CNHCDIWDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	// 延时打开串口
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCDIWDlg::InitCtrls(void)
{
	SetDlgFont();
	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	m_shdTitle.SetTitle(L"测功机基本惯量测试");

	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);

	// 设置功率、力、速度显示框风格
	m_lbP.SetLEDStyle();
	m_lbF.SetLEDStyle();
	m_lbV.SetLEDStyle();

	// 设置编辑框属性(只读,只输入数字)
	m_edLowerForce.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edUpperForce.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edLowerVelocity.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edUpperVelocity.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edLowerForce.ModifyStyle(0, ES_NUMBER);
	m_edUpperForce.ModifyStyle(0, ES_NUMBER);
	m_edLowerVelocity.ModifyStyle(0, ES_NUMBER);
	m_edUpperVelocity.ModifyStyle(0, ES_NUMBER);

	// 显示测试参数
	CString str;
	str.Format(L"%.0f", m_fLowerForce);
	m_edLowerForce.SetWindowTextW(str);
	GetDlgItem(IDC_STATIC_LOWER_FORCE)->SetWindowTextW(str);
	str.Format(L"%.0f", m_fUpperForce);
	m_edUpperForce.SetWindowTextW(str);
	GetDlgItem(IDC_STATIC_UPPER_FORCE)->SetWindowTextW(str);
	str.Format(L"%.0f", m_fLowerVelocity);
	m_edLowerVelocity.SetWindowTextW(str);
	str.Format(L"%.0f", m_fUpperVelocity);
	m_edUpperVelocity.SetWindowTextW(str);

	// 设置标签字体颜色
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lb4.SetTextColor(RGB(0, 102, 204));

	// 设置测试结果风格
	m_lbLowerSlipingTime.SetBkColor(RGB(230, 230, 230));
	m_lbUpperSlipingTime.SetBkColor(RGB(230, 230, 230));
	m_lbAveLowerForce.SetBkColor(RGB(230, 230, 230));
	m_lbAveUpperForce.SetBkColor(RGB(230, 230, 230));
	m_lbMeasuredDIW.SetBkColor(RGB(230, 230, 230));
}

void CNHCDIWDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCDIWDlg::EnableRefreshRealTimeData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(2, 500, NULL);
	}
	else
	{
		KillTimer(2);
	}
}

void CNHCDIWDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if (1 == nIDEvent)
	{
		// 操作初始化，打开串口
		KillTimer(1);
		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}
	else if (3 == nIDEvent)
	{
		DisplayDIWTestProcess();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCDIWDlg::RefreshRealTimeData(void)
{
	// 刷新主数据
	// 同时进行通讯测试
	// 500毫秒执行一次

	CString str(L"");
	//if (DYN_WR_SUCCEED == m_pNHCDIWThread->GetRealTimeData())
	m_pNHCDIWThread->GetRealTimeData_Asyn(NULL);
	{
		// 通讯成功
		str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
		GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
		GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
	}
	//else
	//{
	//	// 通讯失败
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCDIWDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// 转化为大写
	strDynProtocol.MakeUpper();
	CDynThread::InstrumentModel imDyn;
	if (L"NANHUA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}
	else if (L"MAHA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_MAHA_ASMP;
	}
	//else if (L"VAMAGRSR576" == strDynProtocol)
	//{
	//	imDyn = CDynThread::IM_VAMAG_RSR576;
	//}
	else
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}

	if (DYN_OPEN_SUCCEED == m_pNHCDIWThread->Open(bPort, imDyn))
	{
		// 进行通讯测试
		// 用取状态进行通讯测试
		if (DYN_WR_SUCCEED == m_pNHCDIWThread->GetDIW(&m_usDIW))
		{
			// 基本惯量不能算入寄生功率
			// 取出寄生功率参数,暂存起来,退出程序时重新写入
			m_pNHCDIWThread->ReadVar(39, &m_wAp);
			m_pNHCDIWThread->ReadVar(40, &m_wBp);
			m_pNHCDIWThread->ReadVar(41, &m_wCp);
			// 清零参数
			m_pNHCDIWThread->WriteVar(39, 0);
			m_pNHCDIWThread->WriteVar(40, 0);
			m_pNHCDIWThread->WriteVar(41, 0);

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("基本惯量测试"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
			// 显示标称基本惯量
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);
	}
}

void CNHCDIWDlg::DisplayDIWTestProcess(void)
{
	switch (m_pNHCDIWThread->GetDIWState())
	{
	case NHC_DIW_STATE_NOT_START:
		{
			// 测试未开始
			break;
		}
	case NHC_DIW_STATE_READY:
		{
			// 准备就绪
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在初始化测试参数...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			break;
		}
	case NHC_DIW_STATE_LIFTBEAM_DOWN:
		{
			// 降举升器
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在降举升器...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			break;
		}
	case NHC_DIW_STATE_ACCELERATION_1:
		{
			// 加速
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在加速...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			break;
		}
	case NHC_DIW_STATE_SLIP_1:
		{
			// 滑行(小加载力)
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在滑行...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// 显示滑行时间，单位ms－>s
			m_unLowerSlipingTime = m_pNHCDIWThread->GetLowerSlipingTime();
			str.Format(L"%.3f", m_unLowerSlipingTime / 1000.0f);
			GetDlgItem(IDC_STATIC_LOWER_SLIPINGTIME)->SetWindowTextW(str);
			// 显示加载力平均值(小加载力)
			m_fAveLowerForce = m_pNHCDIWThread->GetAveLowerForce();
			if (m_fAveLowerForce > 10e-6f)
			{
				// 不为负数时,才显示
				str.Format(L"%.1f", m_fAveLowerForce);
				GetDlgItem(IDC_STATIC_AVE_LOWER_FORCE)->SetWindowTextW(str);
			}
			break;
		}
	case NHC_DIW_STATE_WAIT_FOR_STOP:
		{
			// 等待滚筒停止
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在滑行...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			break;
		}
	case NHC_DIW_STATE_ACCELERATION_2:
		{
			// 加速
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在加速...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			break;
		}
	case NHC_DIW_STATE_SLIP_2:
		{
			// 滑行(大加载力)
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"正在滑行...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// 显示滑行时间，单位ms－>s
			m_unUpperSlipingTime = m_pNHCDIWThread->GetUpperSlipingTime();
			str.Format(L"%.3f", m_unUpperSlipingTime / 1000.0f);
			GetDlgItem(IDC_STATIC_UPPER_SLIPINGTIME)->SetWindowTextW(str);
			// 显示加载力平均值(大加载力)
			m_fAveUpperForce = m_pNHCDIWThread->GetAveUpperForce();
			if (m_fAveUpperForce > 10e-6f)
			{
				// 不为负数时,才显示
				str.Format(L"%.1f", m_fAveUpperForce);
				GetDlgItem(IDC_STATIC_AVE_UPPER_FORCE)->SetWindowTextW(str);
			}
			break;
		}
	case NHC_DIW_STATE_FINISHED:
		{
			// 完成

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试完成");
			// 停止显示流程
			KillTimer(3);
			// 计算实测基本惯量
			CalculateDIW();
			// 显示实测基本惯量
			CString str;
			str.Format(L"%hu", m_usMeasuredDIW);
			GetDlgItem(IDC_STATIC_MEASURED_DIW)->SetWindowText(str);
			//开始刷新数据
			EnableRefreshRealTimeData(true);
			// 启用开始测试
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			// 禁用停止测试
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			// 启用写入
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
			// 启用退出测试界面
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			break;
		}
	case NHC_DIW_STATE_ERROR:
		{
			// 测试过程发生错误

			if (NHC_DIW_COMMUNICATION_ERROR == m_pNHCDIWThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试终止,与测功机通讯超时");
			}
			else if (NHC_DIW_START_MMTIMER_FAIL == m_pNHCDIWThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试终止,启动定时器失败");
			}
			// 停止显示流程
			KillTimer(3);
			//开始刷新数据
			EnableRefreshRealTimeData(true);
			// 启用开始测试
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			// 禁用停止测试
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			// 启用写入
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
			// 启用退出测试界面
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			break;
		}
	default:
		{
			break;
		}
	}
}

void CNHCDIWDlg::CalculateDIW(void)
{
	float f = (m_fAveUpperForce-m_fAveLowerForce) * (m_unLowerSlipingTime/1000.0f) * (m_unUpperSlipingTime/1000.0f) / ((m_fUpperVelocity-m_fLowerVelocity)/3.6f) / ((m_unLowerSlipingTime-m_unUpperSlipingTime)/1000.0f);
	// 四舍五入
	m_usMeasuredDIW = USHORT(f + 0.5f);
}

void CNHCDIWDlg::OnBnClickedButtonStartTest()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	CString str(L"");

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 检查滚筒是否有转动
	DWORD dw = m_pNHCDIWThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCDIWThread->GetVValue() > 0.05f)
		{
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开始测试", L"基本惯量", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"基本惯量", MB_OK|MB_ICONERROR);
		return;
	}

	// 安全提示，测功机滚筒将要转动
	if (IDYES == MessageBox(L"测试过程滚筒将会高速转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"基本惯量", MB_YESNO|MB_ICONWARNING))
	{
		// 禁止开始测试
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		// 启用停止测试
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// 禁用写入
		GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);
		// 禁止退出测试界面
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

		// 清除结果框数据
		m_lbLowerSlipingTime.SetText(L"--");
		m_lbUpperSlipingTime.SetText(L"--");
		m_lbAveLowerForce.SetText(L"--");
		m_lbAveUpperForce.SetText(L"--");
		m_lbMeasuredDIW.SetText(L"--");

		// 开始刷新检测过程
		SetTimer(3, 200, NULL);
		m_pNHCDIWThread->StartDIWTest_Asyn(m_fLowerForce, m_fUpperForce, m_fLowerVelocity, m_fUpperVelocity, m_hWnd);
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		MessageBox(L"测试被取消", L"基本惯量", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
	////////////////////////////////////////写基本惯量测试的日志
	CString strBZDIW;//标准值
	CString strCSDIW;//测试值
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcscpy_s(sEquCalChkInfo.wchItem, L"测功机基本惯量测试");
	GetDlgItem(IDC_STATIC_DIW)->GetWindowTextW(strBZDIW);
	GetDlgItem(IDC_STATIC_DIW)->GetWindowTextW(strCSDIW);
	CString strData;
	strData.Format(L"标准值%c,测试值%c",strBZDIW,strCSDIW);
	wcscpy_s(sEquCalChkInfo.wchData, strData);
	//// 写EquCalChkInfo表
	SetDboEquCalChkInfo(sEquCalChkInfo);
	
}

void CNHCDIWDlg::OnBnClickedButtonStopTest()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"测试被用户停止");

	// 停止显示测试流程
	KillTimer(3);

	m_pNHCDIWThread->StopDIWTest();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	// 启用开始测试
	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
	// 禁用停止测试
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	// 启用写入
	GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
	// 启用退出测试界面
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHCDIWDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	EnableRefreshRealTimeData(false);

	OnCancel();
}

void CNHCDIWDlg::OnBnClickedButtonWriteDiw()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	CString str;
	GetDlgItem(IDC_EDIT_DIW_VALUE_TO_WRITE)->GetWindowText(str);
	// 判断是否有输入写入值
	if (L"" == str)
	{
		MessageBox(L"写入值不能为空!", L"基本惯量", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_DIW_VALUE_TO_WRITE)->SetFocus();
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		return;
	}
	USHORT usNewDIW = (USHORT)_tstoi(str);
	if (usNewDIW<800 || usNewDIW>1000)
	{
		// 提示写入值不在一般范围
		str.Format(L"将要写入的基本惯量值为 %hukg,不在一般范围内(800~1000kg)\n\n是否继续写入?", usNewDIW);
		if (IDNO == MessageBox(str, L"基本惯量", MB_YESNO|MB_ICONWARNING))
		{
			MessageBox(L"写入操作被取消", L"基本惯量", MB_OK|MB_ICONINFORMATION);
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			return;
		}
	}
	// 写入基本惯量
	if (DYN_WR_SUCCEED != m_pNHCDIWThread->SetDIW(usNewDIW))
	{
		MessageBox(L"写入基本惯量失败", L"基本惯量", MB_OK|MB_ICONERROR);
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		return;
	}
	// 先保存旧的基本惯量
	USHORT usOldDIW = m_usDIW;
	// 读出基本惯量,并更新到显示框
	m_pNHCDIWThread->GetDIW(&m_usDIW);
	str.Format(L"%hu", m_usDIW);
	GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);

	// 开始刷新数据
	EnableRefreshRealTimeData(true);
	EndWaitCursor();

	if (usOldDIW != usNewDIW)
	{
		// 提示需要进行寄生功率测试
		MessageBox(L"标称基本惯量已经改变,需要重新进行寄生功率测试", L"基本惯量", MB_OK|MB_ICONINFORMATION);
	}
}
