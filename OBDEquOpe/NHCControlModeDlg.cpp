// NHCControlModeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCControlModeDlg.h"


// CNHCControlModeDlg 对话框

IMPLEMENT_DYNAMIC(CNHCControlModeDlg, CDialogZoom)

CNHCControlModeDlg::CNHCControlModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCControlModeDlg::IDD, pParent)
	,m_fNominalV(100.0f)
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
}

CNHCControlModeDlg::~CNHCControlModeDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCControlModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_P_VALUE, m_nePValue);
	DDX_Control(pDX, IDC_EDIT_F_VALUE, m_neFValue);
	DDX_Control(pDX, IDC_EDIT_V_VALUE, m_neVValue);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_F, m_lbF);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHCControlModeDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_POWER_ON, &CNHCControlModeDlg::OnBnClickedButtonConstantPowerOn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_FORCE_ON, &CNHCControlModeDlg::OnBnClickedButtonConstantForceOn)
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_VELOCITY_ON, &CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCControlModeDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCControlModeDlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCControlModeDlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCControlModeDlg::OnBnClickedButtonMotorOff)
	ON_BN_CLICKED(IDC_BUTTON_CONTROL_MODE_OFF, &CNHCControlModeDlg::OnBnClickedButtonControlModeOff)
	ON_BN_CLICKED(IDCANCEL, &CNHCControlModeDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_DYN_GET_REALTIME_DATA, OnGetRealTimeData)
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_VELOCITY_ON2, &CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn2)
END_MESSAGE_MAP()


// CNHCControlModeDlg 消息处理程序

BOOL CNHCControlModeDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	// 延时打开串口
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCControlModeDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCControlModeDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机控制模式");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);

	// 设置功率、力、速度显示框风格
	m_lbP.SetLEDStyle();
	m_lbF.SetLEDStyle();
	m_lbV.SetLEDStyle();

	//// 编辑框只能输入数字
	//m_edPValue.ModifyStyle(0, ES_NUMBER);
	//m_edFValue.ModifyStyle(0, ES_NUMBER);
	//m_edVValue.ModifyStyle(0, ES_NUMBER);

	// 设置标签字体颜色
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
}

void CNHCControlModeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCControlModeDlg::OpenSerialPortAndTestCommunication(void)
{
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
			m_pDynThread->GetMotorMaxVelocity(&m_fNominalV);
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("测功机控制模式"));
			GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
	}
}

LRESULT CNHCControlModeDlg::OnGetRealTimeData(WPARAM wParam, LPARAM lParam)
{
	if (DYN_WR_SUCCEED != (DWORD)wParam)
	{
		TRACE("取实时数据失败(通讯超时)\n");
	}

	return 0;
}

void CNHCControlModeDlg::RefreshRealTimeData(void)
{
	// 刷新主数据
	// 同时进行通讯测试
	// 500毫秒执行一次

	CString str(L"");
	//if (DYN_WR_SUCCEED == m_pDynThread->GetRealTimeData())
	m_pDynThread->GetRealTimeData_Asyn(m_hWnd);
	{
		// 通讯成功
		str.Format(L"%.2f", m_pDynThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.1f", m_pDynThread->GetFValue());
		GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pDynThread->GetVValue());
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

void CNHCControlModeDlg::EnableRefreshRealTimeData(bool bEnable)
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

void CNHCControlModeDlg::EnableBasicOperation(bool bEnable)
{
	if (bEnable)
	{
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
	}
}


void CNHCControlModeDlg::OnBnClickedButtonConstantPowerOn()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查目标功率值
	CString str(L"");
	GetDlgItem(IDC_EDIT_P_VALUE)->GetWindowText(str);
	if (L"" == str)
	{
		MessageBoxW(L"恒功率目标值不能为空", L"恒功率", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_P_VALUE)->SetFocus();
		return;
	}
	float fPValue = (float)_wtof(str);
	if (fPValue<0.0f || fPValue>50.0f)
	{
		MessageBoxW(L"请输入正确的恒功率目标值(0~50kW)", L"恒功率", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_P_VALUE)->SetFocus();
		return;
	}

	BeginWaitCursor();
	// 停止刷新数据
	EnableRefreshRealTimeData(false);
	// 设置目标值
	if (DYN_WR_SUCCEED != m_pDynThread->SetConstantPowerTarget(fPValue))
	{
		EndWaitCursor();
		MessageBoxW(L"设置恒功率目标值超时", L"恒功率", MB_OK|MB_ICONERROR);
		return;
	}
	// 启动恒功率
	//if (DYN_WR_SUCCEED != m_pDynThread->SendCtrlWord('7'))ConstantPowerControlModeOn
	if (DYN_WR_SUCCEED != m_pDynThread->ConstantPowerControlModeOn())
	{
		MessageBoxW(L"启动恒功率控制模式超时", L"恒功率", MB_OK|MB_ICONERROR);
	}
	else
	{
		str.Format(L"正在运行恒功率控制模式:%.2fkW", fPValue);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(str);
	}
	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	// 禁止进行基本操作
	EnableBasicOperation(false);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonConstantForceOn()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查目标力值
	CString str(L"");
	GetDlgItem(IDC_EDIT_F_VALUE)->GetWindowText(str);
	if (L"" == str)
	{
		MessageBoxW(L"恒力目标值不能为空", L"恒力", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_F_VALUE)->SetFocus();
		return;
	}
	float fFValue = (float)_wtof(str);
	if (fFValue<0.0f || fFValue>8000.0f)
	{
		MessageBoxW(L"请输入正确的恒力目标值(0~8000.0N)", L"恒力", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_F_VALUE)->SetFocus();
		return;
	}

	BeginWaitCursor();
	// 停止刷新数据
	EnableRefreshRealTimeData(false);
	// 设置目标值
	if (DYN_WR_SUCCEED != m_pDynThread->SetConstantForceTarget(fFValue))
	{
		EndWaitCursor();
		MessageBoxW(L"设置恒力目标值超时", L"恒力", MB_OK|MB_ICONERROR);
		return;
	}
	// 启动恒力
	if (DYN_WR_SUCCEED != m_pDynThread->ConstantForceControlModeOn())
	{
		MessageBoxW(L"启动恒力控制模式超时", L"恒力", MB_OK|MB_ICONERROR);
	}
	else
	{
		str.Format(L"正在运行恒力控制模式:%.1fN", fFValue);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(str);
	}
	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	// 禁止进行基本操作
	EnableBasicOperation(false);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查目标速度值
	CString str(L"");
	GetDlgItem(IDC_EDIT_V_VALUE)->GetWindowText(str);
	if (L"" == str)
	{
		MessageBoxW(L"恒速度目标值不能为空", L"恒速度", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE)->SetFocus();
		return;
	}
	float fVValue = (float)_wtof(str);
	if (fVValue<0.0f || fVValue>100.0f)
	{
		MessageBoxW(L"请输入正确的恒速度目标值(0~100.0km/h)", L"恒速度", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE)->SetFocus();
		return;
	}

	BeginWaitCursor();
	// 停止刷新数据
	EnableRefreshRealTimeData(false);
	// 设置目标值
	if (DYN_WR_SUCCEED != m_pDynThread->SetConstantVelocityTarget(fVValue))
	{
		EndWaitCursor();
		MessageBoxW(L"设置恒速度目标值超时", L"恒速度", MB_OK|MB_ICONERROR);
		return;
	}
	// 启动恒速度
	if (DYN_WR_SUCCEED != m_pDynThread->ConstantVelocityControlModeOn())
	{
		MessageBoxW(L"启动恒速度控制模式超时", L"恒速度", MB_OK|MB_ICONERROR);
	}
	else
	{
		str.Format(L"正在运行恒速度控制模式:%.1fkm/h", fVValue);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(str);
	}
	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	// 禁止进行基本操作
	EnableBasicOperation(false);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: 在此添加控件通知处理程序代码

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
			MessageBox(L"滚筒正在转动,暂时不能升起举升器", L"升举升器", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"升举升器", MB_OK|MB_ICONERROR);
		return;
	}

	m_pDynThread->LiftBeamUp();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	m_pDynThread->LiftBeamDown();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonMotorOn()
{
	// TODO: 在此添加控件通知处理程序代码

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
		m_pDynThread->LiftBeamDown();
		m_pDynThread->MotorOn();
	}
	else
	{
		MessageBox(L"开电机被取消", L"开电机", MB_OK|MB_ICONWARNING);
	}

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonMotorOff()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	m_pDynThread->MotorOff();

	// 开始刷新数据
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonControlModeOff()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	m_pDynThread->ControlModeOff();
	///恢复测功机电机标称速度
	m_pDynThread->SetMotorMaxVelocity(m_fNominalV);

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("已停止所有控制模式"));

	// 可以进行基本操作
	EnableBasicOperation(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 停止所有控制模式
	m_pDynThread->ControlModeOff();

	///恢复测功机电机标称速度
	m_pDynThread->SetMotorMaxVelocity(m_fNominalV);

	OnCancel();
}

void CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn2()
{
	// TODO: 在此添加控件通知处理程序代码

	int nVValue(0);
	// 目标速度值
	CString str(L"");
	EnableRefreshRealTimeData(false);
	GetDlgItem(IDC_EDIT_V_VALUE2)->GetWindowText(str);
	float fVValue = (float)_wtof(str);
	if (m_pDynThread->GetVValue() != 0.0f)
	{
		MessageBoxW(L"滚筒还在转动,待测功机滚筒完全停止后再启动", L"恒速度", MB_OK|MB_ICONERROR);
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		return;
	}
	else
	{
		MessageBoxW(L"滚筒开始转动,请确认测功机滚筒旁不能有人和杂物！", L"安全警告", MB_OK|MB_ICONERROR);
	}
	if (L"" == str)
	{
		MessageBoxW(L"恒速度目标值不能为空", L"恒速度", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE2)->SetFocus();
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		return;
	}
	if (fVValue<0.0f || fVValue>100.0f)
	{
		MessageBoxW(L"请输入正确的恒速度目标值(0~100.0km/h)", L"恒速度", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE2)->SetFocus();
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		return;
	}
	else
	{
		// 设置目标值
		if (DYN_WR_SUCCEED != m_pDynThread->SetMotorMaxVelocity(fVValue))
		{
			EndWaitCursor();
			MessageBoxW(L"设置恒速度目标值超时", L"恒速度", MB_OK|MB_ICONERROR);
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			return;
		}
	}
	BeginWaitCursor();
	// 开始刷新数据
	EnableRefreshRealTimeData(true);
	///启动电机
	m_pDynThread->MotorOn();
}
