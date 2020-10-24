// NHCResponseTime2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCResponseTime2Dlg.h"

#include <cmath>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCResponseTime2Dlg 对话框

IMPLEMENT_DYNAMIC(CNHCResponseTime2Dlg, CDialogZoom)

CNHCResponseTime2Dlg::CNHCResponseTime2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCResponseTime2Dlg::IDD, pParent)
	, m_bIsOverSilpOn(false)
	, m_fSpeed_High(56.3f)
	, m_fSpeed_Low(40.0f)
	, m_fLoad_High(1323.0f)
	, m_fLoad_Low(1719.0f)
	, m_fTopSlipSpeed(64.4f)
	, m_usDIW(0)
	, m_nInedxPosition(0)
	, IsCanRefreshCurve(false)
	, nPOINTF(0)
{
	m_lsRTDataOfDynResponseTime.RemoveAll();
	// 启动测功机控制线程
	//m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));
	m_pCNHCResponseTime2Thread = (CNHCResponseTime2Thread*)AfxBeginThread(RUNTIME_CLASS(CNHCResponseTime2Thread));

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
CNHCResponseTime2Dlg::~CNHCResponseTime2Dlg()
{
	m_pCNHCResponseTime2Thread->ControlModeOff();

	m_fontDlgFont.DeleteObject();
	if (NULL != m_pCNHCResponseTime2Thread)
	{
		m_pCNHCResponseTime2Thread->Close();
		m_pCNHCResponseTime2Thread->QuitThread();
		WaitForSingleObject(m_pCNHCResponseTime2Thread->m_hThread, 5000);
	}
}

void CNHCResponseTime2Dlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}
void CNHCResponseTime2Dlg::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, IDC_STATIC_RESPONSE_TIME, m_lbResponse_Time);
	DDX_Control(pDX, IDC_STATIC_STABILIZATION_TIME, m_lbStabilization_Time);


	DDX_Control(pDX, IDC_EDT_SLIP_SPEED, m_edtTopSilpSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEED_HIGH, m_fSpeed_High);
	DDX_Text(pDX, IDC_EDIT_SPEED_LOW, m_fSpeed_Low);
	DDX_Text(pDX, IDC_EDIT_LOAD_HIGH, m_fLoad_High);
	DDX_Text(pDX, IDC_EDIT_LOAD_LOW, m_fLoad_Low);
	DDX_Text(pDX, IDC_EDT_SLIP_SPEED, m_fTopSlipSpeed);
	DDV_MinMaxFloat(pDX, m_fTopSlipSpeed, 5, 100);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCResponseTime2Dlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_START_RESPONSE, &CNHCResponseTime2Dlg::OnBnClickedBtnStartResponse)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHCResponseTime2Dlg::OnBnClickedButtonStop)
	ON_MESSAGE(WM_DYN_SAVE_RESPONSE_TIME_DATA, SaveRealTimeData)
	ON_MESSAGE(WM_DYN_START_RESPONSE_TIME_FINISHED, SaveRealTimeData)
END_MESSAGE_MAP()


// CNHCResponseTime2Dlg 消息处理程序

BOOL CNHCResponseTime2Dlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowWindow(SW_MAXIMIZE);
	InitCtrls();


	// 延时打开串口
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CNHCResponseTime2Dlg::InitCtrls(void)
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

	// 初始化曲线坐标系
	InitCurve();
}
void CNHCResponseTime2Dlg::OnBnClickedBtnStartResponse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	// 合理性检查
	if (m_fSpeed_High < m_fSpeed_Low || m_fSpeed_Low < 5.0f || m_fSpeed_High < 5.0f)
	{
		MessageBox(L"输入的速度不合理!设定的[高速点]不能小于设定的[低速点]!");
		return;
	}
	if(m_fTopSlipSpeed > 100.0f || m_fTopSlipSpeed < m_fSpeed_High)
	{
		MessageBox(L"输入的速度不合理!设定的[开始滑行速度]不能小于设定的[高速点]!");
		return;
	}
	BeginWaitCursor();

	// 检查滚筒是否有转动
	DWORD dw = m_pCNHCResponseTime2Thread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pCNHCResponseTime2Thread->GetVValue() > 0.05f)
		{
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"通讯超时", L"关电机", MB_OK|MB_ICONERROR);
		return;
	}

	// 检查举升器状态
	dw = m_pCNHCResponseTime2Thread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pCNHCResponseTime2Thread->IsTheStatusBitSignaled(0))
		{
			// 举升器已经升起

			EndWaitCursor();
			MessageBox(L"请降低举升器后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			m_pCNHCResponseTime2Thread->LiftBeamDown();
			return;
		}
		else if (!(m_pCNHCResponseTime2Thread->IsTheStatusBitSignaled(0)) && !(m_pCNHCResponseTime2Thread->IsTheStatusBitSignaled(1)))
		{
			// 举升器正在降下或者正在升起

			// 发送降举升器命令
			m_pCNHCResponseTime2Thread->LiftBeamDown();

			EndWaitCursor();
			MessageBox(L"请等举升器完全降下后,再开电机", L"开电机", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"通讯超时", L"开电机", MB_OK|MB_ICONERROR);
		return;
	}
	m_pCNHCResponseTime2Thread->ResponseTimeControlModeEnd();

	m_pCNHCResponseTime2Thread->ResponseTimeControlModeStart();

	// 进行安全提示
	if (IDYES == MessageBox(L"滚筒即将转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"开电机", MB_YESNO|MB_ICONWARNING))
	{
		m_lsRTDataOfDynResponseTime.RemoveAll();
		m_stResponseTimeCurve.Clear();
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
		GetDlgItem(IDC_STATIC_LOADFORCE)->SetWindowTextW(L"--");
		GetDlgItem(IDC_STATIC_RESPONSE_TIME)->SetWindowTextW(L"--");
		GetDlgItem(IDC_STATIC_STABILIZATION_TIME)->SetWindowTextW(L"--");

		m_pCNHCResponseTime2Thread->StartResponseTimeTest_Asyn(m_fLoad_High, m_fLoad_Low, m_fSpeed_High, m_fSpeed_Low, m_fTopSlipSpeed,m_hWnd);

		//禁止控件输入
		//m_edtTopSilpSpeed.EnableWindow(0);
		//m_edtSpeed_High.EnableWindow(0);
		//m_edtSpeed_Low.EnableWindow(0);
		//m_edtLoadTraction_High.EnableWindow(0);
		//m_edtLoadTraction_Low.EnableWindow(0);
		ControlEnableWindows(false);

		SetTimer(4,50,NULL);

	}
	else
	{
		MessageBox(L"开电机被取消", L"开电机", MB_OK|MB_ICONWARNING);
	}


	EndWaitCursor();

}

void CNHCResponseTime2Dlg::OnTimer(UINT_PTR nIDEvent)
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
		if (DYN_OPEN_SUCCEED == m_pCNHCResponseTime2Thread->Open(bPort, imDyn))
		{
			// 进行通讯测试
			// 用取状态进行通讯测试

			if (DYN_WR_SUCCEED == m_pCNHCResponseTime2Thread->GetDIW(&m_usDIW))
			//if (DYN_WR_SUCCEED == m_pCNHCResponseTime2Thread->GetStatus())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("测功机响应时间测试"));
				GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

			}
			else
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("通讯超时"));
#ifdef _DEBUG
#else
				GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

				//禁止控件输入
				ControlEnableWindows(false);
				//m_edtTopSilpSpeed.EnableWindow(0);
				//m_edtSpeed_High.EnableWindow(0);
				//m_edtSpeed_Low.EnableWindow(0);
				//m_edtLoadTraction_High.EnableWindow(0);
				//m_edtLoadTraction_Low.EnableWindow(0);
#endif
			}
		}
		else
		{
			// 打开串口失败
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("打开通讯端口失败"));
#ifdef _DEBUG
#else
			GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

			//禁止控件输入
			ControlEnableWindows(false);

			//m_edtTopSilpSpeed.EnableWindow(0);
			//m_edtSpeed_High.EnableWindow(0);
			//m_edtSpeed_Low.EnableWindow(0);
			//m_edtLoadTraction_High.EnableWindow(0);
			//m_edtLoadTraction_Low.EnableWindow(0);
#endif
		}
	}
	else if(4 == nIDEvent)
	{
		CString str;
		str.Format(L"%.2f", m_pCNHCResponseTime2Thread->GetRealTimeFDate(NHC_RT_SAMPLING_NUM-1));
		GetDlgItem(IDC_STATIC_LOADFORCE)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pCNHCResponseTime2Thread->GetRealTimeVDate(NHC_RT_SAMPLING_NUM-1));
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);

		switch (m_pCNHCResponseTime2Thread->GetResponseTimeState())
		{
		case NHC_RT_STATE_READY:
			m_lbMsg.SetWindowText(L"正在初始化测试参数...");
			break;
		case NHC_RT_STATE_LIFTBEAM_DOWN:
			m_lbMsg.SetWindowText(L"正在降举升器...");
			break;
		case NHC_RT_STATE_ACCELERATION:
			m_lbMsg.SetWindowText(L"正在加速...");
			break;
		case NHC_RT_STATE_SLIP:
			m_lbMsg.SetWindowText(L"正在滑行...");
			break;
		case NHC_RT_STATE_BRAKE_High:
			m_lbMsg.SetWindowText(L"到达设定的速度(高速点)");
			break;
		case NHC_RT_STATE_BRAKE_Low:
			m_lbMsg.SetWindowText(L"到达设定的速度(低速点)");
			break;
		case NHC_RT_STATE_FINISHED:
			m_lbMsg.SetWindowText(L"测试完成");
			// 停止秒表
			KillTimer(4);
			KillTimer(3);
			IsCanRefreshCurve =false;

			m_pCNHCResponseTime2Thread->SetConstantForceTarget(1000.0f);
			m_pCNHCResponseTime2Thread->ConstantForceControlModeOn();
			// 数据处理
			DataAnalysis();
			//m_edtTopSilpSpeed.EnableWindow(1);
			//m_edtSpeed_High.EnableWindow(1);
			//m_edtSpeed_Low.EnableWindow(1);
			//m_edtLoadTraction_High.EnableWindow(1);
			//m_edtLoadTraction_Low.EnableWindow(1);
			ControlEnableWindows(true);

			break;
		case NHC_RT_STATE_ERROR:
			if (NHC_RT_COMMUNICATION_ERROR == m_pCNHCResponseTime2Thread->GetLastErrorCode())
			{
				m_lbMsg.SetWindowText(L"测试终止,与测功机通讯超时");
			}
			else if (NHC_RT_START_MMTIMER_FAIL == m_pCNHCResponseTime2Thread->GetLastErrorCode())
			{
				m_lbMsg.SetWindowText(L"测试终止,启动定时器失败");
			}
			m_pCNHCResponseTime2Thread->SetConstantForceTarget(1000.0f);
			m_pCNHCResponseTime2Thread->ConstantForceControlModeOn();
			KillTimer(4);
			break;
		default:
			break;
		}
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCResponseTime2Dlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_lbMsg.SetWindowText(L"停止测试");
	// 退出所有控制模式
	m_pCNHCResponseTime2Thread->ResponseTimeControlModeEnd();
	m_pCNHCResponseTime2Thread->ControlModeOff();
	m_bIsOverSilpOn = false;
	//m_edtTopSilpSpeed.EnableWindow(1);
	//m_edtSpeed_High.EnableWindow(1);
	//m_edtSpeed_Low.EnableWindow(1);
	//m_edtLoadTraction_High.EnableWindow(1);
	//m_edtLoadTraction_Low.EnableWindow(1);
	ControlEnableWindows(true);
}

void CNHCResponseTime2Dlg::ControlEnableWindows(bool bEnable)
{
	m_edtTopSilpSpeed.EnableWindow(bEnable);
	m_edtSpeed_High.EnableWindow(bEnable);
	m_edtSpeed_Low.EnableWindow(bEnable);
	m_edtLoadTraction_High.EnableWindow(bEnable);
	m_edtLoadTraction_Low.EnableWindow(bEnable);
}

void CNHCResponseTime2Dlg::InitCurve()
{
	m_stResponseTimeCurve.m_szXCaption = _T("时间(ms)");
	m_stResponseTimeCurve.m_szYCaption = _T("Focre(N)");
	m_stResponseTimeCurve.m_nXIntervals = 10;
	m_stResponseTimeCurve.m_nYIntervals = 8;
	m_stResponseTimeCurve.m_uiXPrecision = 1;
	m_stResponseTimeCurve.m_uiYPrecision = 0;
	m_stResponseTimeCurve.m_eXMin = 0.0f;
	m_stResponseTimeCurve.m_eXMax = 5.0f;
	m_stResponseTimeCurve.m_eYMin = 0.0f;
	m_stResponseTimeCurve.m_eYMax = 500.0f;
	m_stResponseTimeCurve.Create(CRect(0, 0, 1, 1), this, ID_CTRL_PLOT_SCAN);

	CRect rcClient;
	GetDlgItem(IDC_STATIC_CURVE)->GetWindowRect(&rcClient);
	m_stResponseTimeCurve.MoveWindow(rcClient.left,rcClient.top, rcClient.Width(), rcClient.Height());

	m_stResponseTimeCurve.SetBkColor(RGB(0,0,0));
	m_stResponseTimeCurve.SetFrameworkColor(RGB(255,255,255));
	m_stResponseTimeCurve.Clear();
}

void CNHCResponseTime2Dlg::RefreshCurve()
{
	POSITION POSITIONpos;
	SRTData sRTData;
	// 开始位置
	POSITIONpos = m_lsRTDataOfDynResponseTime.FindIndex(m_nInedxPosition);
	// 更新曲线 一次画 NHC_RT_SAMPLING_NUM个点
	for( int i = 0; i< NHC_RT_SAMPLING_NUM; i++)
	{
		sRTData = m_lsRTDataOfDynResponseTime.GetNext(POSITIONpos);
		POINTF pLForce = {(float)sRTData.nTime*10, (float)sRTData.fForce};
		m_stResponseTimeCurve.AddData(nPOINTF,pLForce);
	}
	// 记录结束位置
	m_nInedxPosition = m_lsRTDataOfDynResponseTime.GetCount();
}

void CNHCResponseTime2Dlg::DataAnalysis()
{
	m_lbMsg.SetWindowText(L"数据处理中");
	CString str;

	POSITION POSITIONpos;
	SRTData sRTData;
	POSITIONpos = m_lsRTDataOfDynResponseTime.GetHeadPosition();
	// 标志位
	bool bT1(false),bT2(false),bT3(false);
	// 制动时间 t1 t2 t3
	int nt1(0),nt2(0),nt3(0);
	// 制动力 b1,c1,c2  根据JJF1221定义
	float fb1,fc1,fc2;
	// 高速点制动力
	fb1 = m_fLoad_High;
	// 低速点制动力
	fc1 = m_fLoad_Low;
	fc2= (fc1-fb1)*90/100.0f + fb1;
	int nTime(0);
	// 是否上升阶跃
	bool IsUpPhase;
	if(fb1 > fc1)
	{
		IsUpPhase = false;
	}
	else
	{
		IsUpPhase = true;
	}
	while(!bT1 || !bT2 || !bT3)
	{
		sRTData = m_lsRTDataOfDynResponseTime.GetNext(POSITIONpos);
		if(IsUpPhase)
		{
			if( sRTData.fForce >= fb1 && sRTData.fVelocity <= 40.0f && !bT1)
			{
				// t1
				nt1 = sRTData.nTime*10;
				bT1 = true;
			}
			if( sRTData.fForce >= fc2 && sRTData.fVelocity <= 40.0f && !bT2)
			{
				// t2
				nt2 = sRTData.nTime*10;
				bT2 = true;
			}
		}
		else
		{
			if( sRTData.fForce <= fb1 && sRTData.fVelocity <= 40.0f && !bT1)
			{
				// t1
				nt1 = sRTData.nTime*10;
				bT1 = true;
			}
			if( sRTData.fForce <= fc2 && sRTData.fVelocity <= 40.0f && !bT2)
			{
				// t2
				nt2 = sRTData.nTime*10;
				bT2 = true;
			}
		}
		if(sRTData.fForce > fc1*98/100.0f && sRTData.fForce < fc1*102/100.0f && !bT3)
		{
			nTime+=10;
			if(nTime >= 300)
			{
				// t3
				nt3 = sRTData.nTime*10;
				bT3 = true;
			}
		}
		else
		{
			nTime = 0;
		}
	}

	str.Format(L"st1=%d,st2=%d,st3=%d",nt1,nt2,nt3);
	CNHLogAPI::WriteLog(LOG_MSG,L"DataAnalysis",str);

	m_lbMsg.SetWindowText(L"测试完成");

	CString strResponseTime,strStabilizationTime;
	strResponseTime.Format(L"%d",nt2-nt1);
	m_lbResponse_Time.SetWindowText(strResponseTime);
	strStabilizationTime.Format(L"%d",nt3-nt1);
	m_lbStabilization_Time.SetWindowText(strStabilizationTime);
}

LRESULT CNHCResponseTime2Dlg::SaveRealTimeData(WPARAM wParam, LPARAM lParam)
{ 
	SRTData sRTData;
	CString str(L"");

	for(int i = 0;i< NHC_RT_SAMPLING_NUM;i++)
	{
		sRTData.nTime = (int)(m_lsRTDataOfDynResponseTime.GetCount())+1;
		sRTData.fForce = m_pCNHCResponseTime2Thread->GetRealTimeFDate(i);
		sRTData.fVelocity =m_pCNHCResponseTime2Thread->GetRealTimeVDate(i);

		str.Format(L"nTime=%d   fVelocity=%.1f   fForce=%.1f",sRTData.nTime,sRTData.fVelocity,sRTData.fForce);
		CNHLogAPI::WriteLog(LOG_MSG,L"RefreshRealTimeData",str);

		m_lsRTDataOfDynResponseTime.AddTail(sRTData);
	}

	if(IsCanRefreshCurve)
	{
		RefreshCurve();
	}
	else
	{
		m_stResponseTimeCurve.Clear();
		sRTData = m_lsRTDataOfDynResponseTime.GetHead();

		POINTF pTemp = {0, sRTData.fForce};
		nPOINTF = m_stResponseTimeCurve.AddLine(_T("力"), RGB(238, 173, 14) , CLinePlot::LpLine, &pTemp, 1);
		m_nInedxPosition = 0;

		RefreshCurve();
	}
	IsCanRefreshCurve = true;
	return 0;
}