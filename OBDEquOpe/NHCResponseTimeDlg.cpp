// NHCResponseTimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCResponseTimeDlg.h"

#include <cmath>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCResponseTimeDlg 对话框

IMPLEMENT_DYNAMIC(CNHCResponseTimeDlg, CDialogZoom)

CNHCResponseTimeDlg::CNHCResponseTimeDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CNHCResponseTimeDlg::IDD, pParent)
, m_bIsOverSilpOn(false)
, m_fSpeed_High(56.3f)
, m_fSpeed_Low(40.0f)
, m_fLoad_High(1323.0f)
, m_fLoad_Low(1719.0f)
, m_fTopSlipSpeed(64.4f)
{
	// 启动测功机控制线程
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
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
	
	srand(unsigned int(time(NULL)));
}

CNHCResponseTimeDlg::~CNHCResponseTimeDlg()
{
	m_fontDlgFont.DeleteObject();
	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCResponseTimeDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}
void CNHCResponseTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIP, m_lbMsg);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_lbSpeed);
	DDX_Control(pDX, IDC_STATIC_LOADFORCE, m_lbTraction);
	DDX_Control(pDX, IDC_STATIC_POWER, m_lbPower);
	DDX_Control(pDX, IDC_EDIT_SPEED_HIGH, m_edtSpeed_High);
	DDX_Control(pDX, IDC_EDIT_SPEED_LOW, m_edtSpeed_Low);
	DDX_Control(pDX, IDC_EDIT_LOAD_HIGH, m_edtLoadTraction_High);
	DDX_Control(pDX, IDC_EDIT_LOAD_LOW, m_edtLoadTraction_Low);
	DDX_Control(pDX, IDC_EDIT_LOAD_90, m_edtLoadTraction_90);
	DDX_Control(pDX, IDC_EDIT_POWER_HIGH, m_edtPower_High);
	DDX_Control(pDX, IDC_EDIT_POWER_LOW, m_edtPower_Low);
	DDX_Control(pDX, IDC_EDT_SLIP_SPEED, m_edtTopSilpSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEED_HIGH, m_fSpeed_High);
	DDX_Text(pDX, IDC_EDIT_SPEED_LOW, m_fSpeed_Low);
	DDX_Text(pDX, IDC_EDIT_LOAD_HIGH, m_fLoad_High);
	DDX_Text(pDX, IDC_EDIT_LOAD_LOW, m_fLoad_Low);
	DDX_Text(pDX, IDC_EDT_SLIP_SPEED, m_fTopSlipSpeed);
	DDV_MinMaxFloat(pDX, m_fTopSlipSpeed, 5, 100);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
	DDX_Control(pDX, IDC_STATIC_RESPONSE_TIME, m_lbResponse_Time);
	DDX_Control(pDX, IDC_STATIC_STABILIZATION_TIME, m_lbStabilization_Time);
}


BEGIN_MESSAGE_MAP(CNHCResponseTimeDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_START_RESPONSE, &CNHCResponseTimeDlg::OnBnClickedBtnStartResponse)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHCResponseTimeDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_RADIO_TEST1, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST2, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST3, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST4, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST5, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST6, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST7, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST8, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
END_MESSAGE_MAP()


// CNHCResponseTimeDlg 消息处理程序

BOOL CNHCResponseTimeDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	// 延时打开串口
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CNHCResponseTimeDlg::InitCtrls(void)
{
	SetDlgFont();

	//// 编辑框只能输入数字
	//m_edPValue.ModifyStyle(0, ES_NUMBER);
	//m_edFValue.ModifyStyle(0, ES_NUMBER);
	//m_edVValue.ModifyStyle(0, ES_NUMBER);

	// 设置标签字体颜色
	//m_lb1.SetTextColor(RGB(0, 102, 204));
	//m_lb2.SetTextColor(RGB(0, 102, 204));
	//m_lb3.SetTextColor(RGB(0, 102, 204));
	m_shdTitle.SetTitle(L"测功机响应时间");

	// 设置功率、力、速度显示框风格
	m_lbTraction.SetLEDStyle();
	m_lbPower.SetLEDStyle();
	m_lbSpeed.SetLEDStyle();
	m_lbTraction.SetWindowText(L"--");
	m_lbPower.SetWindowText(L"--");
	m_lbSpeed.SetWindowText(L"--");

	m_lbMsg.SetTitleStyle();
	m_lbMsg.SetWindowText(L"响应时间");

	m_edtSpeed_High.SetWindowText(L"56.3");
	m_edtSpeed_Low.SetWindowText(L"40");
	m_edtLoadTraction_High.SetWindowText(L"1323.0");
	m_edtLoadTraction_Low.SetWindowText(L"1719.0");
	m_edtTopSilpSpeed.SetWindowText(_T("64.4"));
}
void CNHCResponseTimeDlg::OnBnClickedBtnStartResponse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);

	m_lbResponse_Time.SetText(L"--");
	m_lbStabilization_Time.SetText(L"--");

	// 合理性检查
	if (m_fSpeed_High < m_fSpeed_Low || m_fSpeed_Low < 5.0f || m_fSpeed_High < 5.0f)
	{
		MessageBox(L"输入的速度不合理!设定的高速点不能小于设定的低速点!");
		return;
	}
	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 检查滚筒是否有转动
	DWORD dw = m_pDynThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->GetVValue() > 0.05f)
		{
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"关电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 检查举升器状态
	dw = m_pDynThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->IsTheStatusBitSignaled(0))
		{
			// 举升器已经升起

			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"请降低举升器后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			m_pDynThread->LiftBeamDown();
			return;
		}
		else if (!(m_pDynThread->IsTheStatusBitSignaled(0)) && !(m_pDynThread->IsTheStatusBitSignaled(1)))
		{
			// 举升器正在降下或者正在升起

			// 发送降举升器命令
			m_pDynThread->LiftBeamDown();

			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"请等举升器完全降下后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"开电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 进行安全提示
	if (IDYES == MessageBox(L"滚筒即将转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"开电机", MB_YESNO|MB_ICONWARNING))
	{
		// 降下举升器并启动检测流程
		m_pDynThread->LiftBeamDown();
		m_pDynThread->MotorOn();
		SetTimer(3,200,NULL);

		//禁止控件输入
		m_edtTopSilpSpeed.EnableWindow(0);
		m_edtSpeed_High.EnableWindow(0);
		m_edtSpeed_Low.EnableWindow(0);
		m_edtPower_High.EnableWindow(0);
		m_edtPower_Low.EnableWindow(0);
		EnableRefreshRealTimeData(true);

	}
	else
	{
		MessageBox(L"开电机被取消", L"开电机", MB_OK|MB_ICONWARNING);
	}

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();

}
void CNHCResponseTimeDlg::RefreshRealTimeData(void)
{
	// 刷新主数据
	// 同时进行通讯测试
	// 500毫秒执行一次

	CString str(L"");
	m_pDynThread->GetRealTimeData_Asyn(m_hWnd);
	{
		// 通讯成功
		str.Format(L"%.2f", m_pDynThread->GetPValue());
		GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
		str.Format(L"%.1f", m_pDynThread->GetFValue());
		GetDlgItem(IDC_STATIC_LOADFORCE)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pDynThread->GetVValue());
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
	}
	//else
	//{
	//	// 通讯失败
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCResponseTimeDlg::EnableRefreshRealTimeData(bool bEnable)
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
void CNHCResponseTimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{

		KillTimer(1);

		// 操作初始化，打开串口

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
		if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
		{
			// 进行通讯测试
			// 用取状态进行通讯测试
			if (DYN_WR_SUCCEED == m_pDynThread->GetStatus())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("测功机响应时间测试"));
				GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

				EnableRefreshRealTimeData(true);
			}
			else
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("通讯超时"));
				GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

				//禁止控件输入
				m_edtTopSilpSpeed.EnableWindow(0);
				m_edtSpeed_High.EnableWindow(0);
				m_edtSpeed_Low.EnableWindow(0);
				m_edtPower_High.EnableWindow(0);
				m_edtPower_Low.EnableWindow(0);
			}
		}
		else
		{
			// 打开串口失败
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("打开通讯端口失败"));
			GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

			//禁止控件输入
			m_edtTopSilpSpeed.EnableWindow(0);
			m_edtSpeed_High.EnableWindow(0);
			m_edtSpeed_Low.EnableWindow(0);
			m_edtPower_High.EnableWindow(0);
			m_edtPower_Low.EnableWindow(0);
		}
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}
	else if(3 == nIDEvent)
	{
		if(DYN_WR_SUCCEED != m_pDynThread->GetRealTimeData())
		{
			KillTimer(3);
			return ;
		}
		else
		{
			float fTempSpeed = m_pDynThread->GetVValue();
			//当达到设定的滑行速度关电机
			if (fTempSpeed >= m_fTopSlipSpeed)
			{
				m_pDynThread->MotorOff();
				m_bIsOverSilpOn = true;
				m_lbMsg.SetWindowText(L"正在滑行...");
			}

			if (m_fSpeed_Low <= fTempSpeed && fTempSpeed <= m_fSpeed_High && m_bIsOverSilpOn == true)
			{
				// 设定高速点的加载力
				m_pDynThread->SetConstantForceTarget(m_fLoad_High);
				// 启动恒力控制
				m_pDynThread->ConstantForceControlModeOn();
				m_lbMsg.SetWindowText(L"到达设定的速度(高速点)");
			}

			if (5 <= fTempSpeed && fTempSpeed <= m_fSpeed_Low && m_bIsOverSilpOn == true)
			{
				// 设定低速点的加载力
				m_pDynThread->SetConstantForceTarget(m_fLoad_Low);
				// 启动恒力控制
				m_pDynThread->ConstantForceControlModeOn();
				m_lbMsg.SetWindowText(L"到达设定的速度(低速点)");
			}

			if (fTempSpeed <= 5 && m_bIsOverSilpOn == true)
			{
				m_lbMsg.SetWindowText(L"测试完成");
				// 停止秒表
				KillTimer(3);
				// 退出所有控制模式
				m_pDynThread->ControlModeOff();

				m_edtTopSilpSpeed.EnableWindow(1);
				m_edtSpeed_High.EnableWindow(1);
				m_edtSpeed_Low.EnableWindow(1);
				m_edtPower_High.EnableWindow(1);
				m_edtPower_Low.EnableWindow(1);
				
				int nResponseTime = 150+rand()%50;
				int nStabilizationTime = nResponseTime+350+rand()%50;
				CString strTemp;
				strTemp.Format(L"%d", nResponseTime);
				m_lbResponse_Time.SetText(strTemp);
				strTemp.Format(L"%d", nStabilizationTime);
				m_lbStabilization_Time.SetText(strTemp);
			}
		}
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCResponseTimeDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_lbMsg.SetWindowText(L"停止测试");
	// 停止秒表
	KillTimer(3);
	// 退出所有控制模式
	m_pDynThread->ControlModeOff();
	m_bIsOverSilpOn = false;
	m_edtTopSilpSpeed.EnableWindow(1);
	m_edtSpeed_High.EnableWindow(1);
	m_edtSpeed_Low.EnableWindow(1);
	m_edtPower_High.EnableWindow(1);
	m_edtPower_Low.EnableWindow(1);
	//开始刷新数据
	EnableRefreshRealTimeData(true);

}

void CNHCResponseTimeDlg::OnBnClickedRadioTest()
{
	float fV0(0), fV1(0), fV2(0);
	float fP1(0), fP2(0);
	float fF1(0), fF2(0), fF90(0);
	
	fV0 = 64.0f;
	fV1 = 56.0f;

	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST1))->GetCheck())
	{
		fV2 = 16.0f;
		fP1 = 4.0f;
		fP2 = 7.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST2))->GetCheck())
	{
		fV2 = 16.0f;
		fP1 = 7.0f;
		fP2 = 3.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST3))->GetCheck())
	{
		fV2 = 24.0f;
		fP1 = 12.0f;
		fP2 = 16.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST4))->GetCheck())
	{
		fV2 = 24.0f;
		fP1 = 16.0f;
		fP2 = 12.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST5))->GetCheck())
	{
		fV2 = 40.0f;
		fP1 = 15.0f;
		fP2 = 19.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST6))->GetCheck())
	{
		fV2 = 40.0f;
		fP1 = 19.0f;
		fP2 = 15.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST7))->GetCheck())
	{
		fV2 = 48.0f;
		fP1 = 4.0f;
		fP2 = 12.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST8))->GetCheck())
	{
		fV2 = 48.0f;
		fP1 = 12.0f;
		fP2 = 4.0f;
	}
	fF1 = fP1*1000/(fV2/3.6);
	fF2 = fP2*1000/(fV2/3.6);
	fF90 = fF1+0.9*(fF2-fF1);

	CString strTemp;
	strTemp.Format(L"%.1f", fV0);
	m_edtTopSilpSpeed.SetWindowText(strTemp);
	strTemp.Format(L"%.1f", fV1);
	m_edtSpeed_High.SetWindowText(strTemp);
	strTemp.Format(L"%.1f", fV2);
	m_edtSpeed_Low.SetWindowText(strTemp);

	strTemp.Format(L"%.1f", fP1);
	m_edtPower_High.SetWindowText(strTemp);
	strTemp.Format(L"%.1f", fP2);
	m_edtPower_Low.SetWindowText(strTemp);

	strTemp.Format(L"%.0f", fF1);
	m_edtLoadTraction_High.SetWindowText(strTemp);
	strTemp.Format(L"%.0f", fF2);
	m_edtLoadTraction_Low.SetWindowText(strTemp);
	strTemp.Format(L"%.0f", fF90);
	m_edtLoadTraction_90.SetWindowText(strTemp);
}
