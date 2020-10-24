// NHCForceCalibrationHeavyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCForceCalibrationHeavyDlg.h"
#include "math.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCForceCalibrationHeavyDlg 对话框

IMPLEMENT_DYNAMIC(CNHCForceCalibrationHeavyDlg, CDialogZoom)

CNHCForceCalibrationHeavyDlg::CNHCForceCalibrationHeavyDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCForceCalibrationHeavyDlg::IDD, pParent)
{
    m_nTestStep = STEP_PREPARE;
	m_nZeroTime = 0;
	m_fCalForce = 0;
	m_nStayTime = 0;
	m_bIsOperationFinishedAtLeaseOnce = false;
	m_nPAUCount = 0;
	m_nCurPAU = NOPAU;

	ZeroMemory(m_fStayF, sizeof(float)*5);
	for (int i=0; i<4; i++)
	{	
		m_bIsPass[i] = true;
		m_bIsCalibrated[i] = false;
	}

	// 启动测功机控制线程
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

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

	m_pfProcessCtrl = NULL;
}

CNHCForceCalibrationHeavyDlg::~CNHCForceCalibrationHeavyDlg()
{	
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCForceCalibrationHeavyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_N, m_lbTorsion);
	DDX_Control(pDX, IDC_STATIC_STEP, m_lbStep);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_RESET, m_lbReset);
}


BEGIN_MESSAGE_MAP(CNHCForceCalibrationHeavyDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_PAU1, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_RADIO_PAU2, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_RADIO_PAU3, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_RADIO_PAU4, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHCForceCalibrationHeavyDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CNHCForceCalibrationHeavyDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDCANCEL, &CNHCForceCalibrationHeavyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNHCForceCalibrationHeavyDlg 消息处理程序

BOOL CNHCForceCalibrationHeavyDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化控件
	InitCtrls();

	// 延时打开串口
	SetTimer(TIMER_OPEN_COM, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCForceCalibrationHeavyDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCForceCalibrationHeavyDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机力校准");

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbReset.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	m_lbTorsion.SetLEDStyle();

	GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
}

void CNHCForceCalibrationHeavyDlg::OpenSerialPortAndTestCommunication(void)
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

	CSimpleIni si(strPath);
	// 获取通讯端口
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));

	// 获取要显示的PAU序号
	bool bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"TR", L"1")));
	m_stPAUShow.bPAU1 = bShow;
	bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"TL", L"1")));
	m_stPAUShow.bPAU2 = bShow;
	bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"BR", L"1")));
	m_stPAUShow.bPAU3 = bShow;
	bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"BL", L"1")));
	m_stPAUShow.bPAU4 = bShow;

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, CDynThread::IM_NANHUA_NHC03))
	{
		std::bitset<16> bit16(0);
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bit16))
		{
			m_lbInfo.SetText(_T("请选择 [力传感器] 进行力校准"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
			
			ResetRadioPau();	
		}
		else
		{
			m_lbInfo.SetText(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	}
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHCForceCalibrationHeavyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_OPEN_COM:
		{
			KillTimer(TIMER_OPEN_COM);
			OpenSerialPortAndTestCommunication();
		}
		break;
	case TIMER_INSTALL_TOOL:
		{
			KillTimer(TIMER_INSTALL_TOOL);
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(_T("请安装好校准工具后点下一步"));
			m_nTestStep = STEP_INTALL_TOOL;
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		}
		break;

	case TIMER_GET_RTDATA:
		{
			m_pDynThread->GetRealTimeData_Asyn(NULL);//获取实时数据
			CString str;
			str.Format(_T("%.1f"), m_pDynThread->GetFValue());
			m_lbTorsion.SetText(str);
		}
		break;

	case TIMER_ZERO_BALANCE:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{
				m_nZeroTime ++;
				if (m_nZeroTime >= 2)//稳定1秒
				{
					m_nTestStep = STEP_PUT_WEIGHT;	// 重柴测功机只需校准一次
					KillTimer(TIMER_ZERO_BALANCE);
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case TIMER_WEIGHT_BALANCE:
		{
			// 判断是否调整完成
			if (m_nStayTime >= 5)// 数组已经填满
			{
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					m_nTestStep = STEP_REMOVE_WEIGHT;
					KillTimer(TIMER_WEIGHT_BALANCE);
					OnBnClickedButtonStart();
				}
			}
			m_fStayF[m_nStayTime%5] = m_pDynThread->GetFValue();
			m_nStayTime++;
		}
		break;

	case TIMER_REMOVE_ALL:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{	
				m_nZeroTime++;
				if (m_nZeroTime >= 2)//稳定1秒
				{
					m_nTestStep = STEP_RESET_ZERO_DONE;
					KillTimer(TIMER_REMOVE_ALL);
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;
	default:
		break;
	}
	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCForceCalibrationHeavyDlg::ClearResult()             //步骤清零
{
	m_lbInfo.StopFlashText();
	m_lbInfo.SetText(_T("力校准前请勿安装校准工具"));
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep1.SetText(L"   安装校准工具");
	m_lbStep2.SetText(L"   放置砝码");
	m_lbStep3.SetText(L"   移走砝码和校准工具");
	m_lbStep4.SetText(L"   完成");

	m_nTestStep = STEP_PREPARE;
	m_nZeroTime = 0;
	m_fCalForce = 0.0f;	
}

void CNHCForceCalibrationHeavyDlg::ResetRadioPau(void)
{	
	GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO_PAU1))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_PAU2))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_PAU3))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_PAU4))->SetCheck(BST_UNCHECKED);
	
	if(m_stPAUShow.bPAU1)
	{
		GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU1)->ShowWindow(FALSE);
	}
	if(m_stPAUShow.bPAU2)
	{
		GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU2)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU2)->ShowWindow(FALSE);
	}
	if(m_stPAUShow.bPAU3)
	{
		GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU3)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU3)->ShowWindow(FALSE);
	}
	if(m_stPAUShow.bPAU4)
	{
		GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU4)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU4)->ShowWindow(FALSE);
	}
}

void CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU1))->GetCheck())
	{
		m_nCurPAU = PAU1;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU2))->GetCheck())
	{
		m_nCurPAU = PAU2;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU3))->GetCheck())
	{
		m_nCurPAU = PAU3;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU4))->GetCheck())
	{
		m_nCurPAU = PAU4;
	}	
	else
	{
		m_nCurPAU = NOPAU;
		return;
	}
	if (m_bIsCalibrated[m_nCurPAU])
	{
		CString strMsg;
		strMsg.Format(L"提示：此[力传感器]已进行过校准！校准结果：%s", m_bIsPass[m_nCurPAU]?L"通过":L"不通过");
		MessageBox(strMsg, _T("力校准"));
	}

	m_pDynThread->WriteVar(231, (WORD)m_nCurPAU);	//设置测功机，选择PAU

	CString strTemp;
	strTemp.Format(L"准备对 [力传感器]-%d 进行校准", m_nCurPAU+1);
	m_lbInfo.SetText(strTemp);
}

void CNHCForceCalibrationHeavyDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nCurPAU == NOPAU)	//没选择PAU
	{
		MessageBox(L"请先选择 [力传感器]！", _T("力校准"));
		return;
	}

	if (m_nTestStep == STEP_PREPARE)
	{	
		m_lbInfo.SetText(_T("力校准前请勿安装校准工具"));

		if (MessageBox(_T("确认开始校准吗?"), _T("力校准"), MB_OKCANCEL) == IDOK)
		{		
			// 回调输出信息
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机力校准开始", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_DYN_FORCE_CAL_START, wchInfo, sizeof(wchInfo));
			}

			ClearResult();		//步骤清零

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
			GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);

			m_fCalForce = (float)m_pDynThread->ReadVar(54)/10.0f;	//取力传感器校准重量(南华)
			m_pDynThread->SendCtrlWord_Asyn('0');					//退出所有控制模式
			m_pDynThread->SendCtrlWord_Asyn('U');					//关闭自动零位调整
			m_pDynThread->SendCtrlWord_Asyn('L');					//力矩零位校准(记录零位)

			SetTimer(TIMER_INSTALL_TOOL, 1500, NULL);
			SetTimer(TIMER_GET_RTDATA, 500, NULL);

			m_lbInfo.SetText(_T("正在调整..."));
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == STEP_INTALL_TOOL)        //校准补偿
	{
		// 判断是否已经安装校准工具
		if (fabs(m_pDynThread->GetFValue()) < 50.0f)
		{
			if (MessageBox(_T("请安装校准工具后点确认!"), _T("力校准"), MB_OKCANCEL) != IDOK)
			{
				m_bIsPass[m_nCurPAU] = false;
				m_nTestStep = STEP_REMOVE_WEIGHT;
				m_lbStep1.SetTextColor(RGB(0, 0, 0));
				m_lbStep1.SetText(L"× 安装校准工具");
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"× 放置砝码");
			}
			OnBnClickedButtonStart();
			return;
		}
	
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"√ 安装校准工具");

		m_lbInfo.SetText(_T("正在调整..."));
		m_pDynThread->SendCtrlWord_Asyn('T');	//打开自动零位调整
		SetTimer(TIMER_ZERO_BALANCE, 500, NULL);
		m_nZeroTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (m_nTestStep == STEP_PUT_WEIGHT)
	{
		m_pDynThread->SendCtrlWord_Asyn('U');	//关闭自动零位调整

		m_lbInfo.SetText(_T("请放置3个砝码后点下一步"));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = STEP_CALIBRATION;
	}
	else if (m_nTestStep == STEP_CALIBRATION)
	{
		// 判断砝码重量是否达到要求正负1000N范围
		if (m_fCalForce > 0.0f)
		{
			if (fabs(m_pDynThread->GetFValue()-m_fCalForce) > 1000.0f)
			{
				MessageBox(_T("砝码重量异常!"), _T("力校准"));
				m_bIsPass[m_nCurPAU] = false;
				m_nTestStep = STEP_REMOVE_WEIGHT;
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"× 放置砝码");
				OnBnClickedButtonStart();
				return;
			}
		}

		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"√ 放置砝码");

		m_lbInfo.SetText(_T("正在调整..."));
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		m_nStayTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('N');	//更新力矩校准数据	
		SetTimer(TIMER_WEIGHT_BALANCE, 1000, NULL);
	}
	else if (m_nTestStep == STEP_REMOVE_WEIGHT)
	{
		m_lbInfo.SetText(_T("请移走砝码和校准工具后点下一步"));	
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = STEP_RESET_ZERO;
	}
	else if (m_nTestStep == STEP_RESET_ZERO)
	{
		if (m_pDynThread->GetFValue() > 50.0f)
		{
			MessageBox(_T("请移走砝码和校准工具,再点击下一步"), _T("力校准"), MB_OK|MB_ICONWARNING);
			return;
		}

        m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"√ 移走砝码和校准工具");

		m_lbInfo.SetText(_T("正在调整..."));
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');	//打开自动零位调整
		SetTimer(TIMER_REMOVE_ALL, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == STEP_RESET_ZERO_DONE)
	{
		m_lbStep4.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(_T("请点击‘完成校准’结束力校准"));
		m_pDynThread->SendCtrlWord_Asyn('V');    //把自动-偏移-调整设为正常
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("完成校准"));
		m_nTestStep = STEP_DONE;

		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == STEP_DONE)
	{
		m_lbInfo.SetText(_T("校准完成"));	
		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"√ 完成");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		
		m_nTestStep = STEP_PREPARE;
		m_bIsCalibrated[m_nCurPAU] = true;

		m_bIsOperationFinishedAtLeaseOnce = true;

		ResetRadioPau();

		//ExportEquCalChkInfo();	// 导出行标结果
		//ExportDemarcationLog();	// 导出标定记录
		ExportDynForceCalibrationResult();	// 导出自定义结果

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机力校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CAL_FINISH, wchInfo, sizeof(wchInfo));
		}
	}

}

void CNHCForceCalibrationHeavyDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nCurPAU == NOPAU)	//没选择PAU
	{
		MessageBox(L"请先选择 [力传感器]！", _T("力校准"));
		return;
	}
	CString strMsg;
	strMsg.Format(L"是否要重置 [力传感器]-%d 参数？", m_nCurPAU+1);
	if (MessageBox(strMsg, _T("力校准"), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	const int nAyyry [4][2]= {{29, 55}, {95, 56}, {96, 91}, {97, 92}};	// PAUx 变量地址{校准因子，零位}
	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar(nAyyry[m_nCurPAU][0], 1280) 
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(nAyyry[m_nCurPAU][1], 960))
	{
		m_lbInfo.SetText(_T("重置完成,请重新进行力校准"));
	}
	else
	{
		m_lbInfo.SetText(_T("重置失败"));
	}
	ClearResult();
	ResetRadioPau();
	m_bIsPass[m_nCurPAU] = true;
	m_bIsCalibrated[m_nCurPAU] = false;
}

void CNHCForceCalibrationHeavyDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	// 没有PAU
	if (m_nPAUCount <= 0)
	{
		OnCancel();
		return;
	}
	// 是否进行过校准
	bool bIsCalibrated = false;
	CString strMsg;	
	for (int i=0; i<m_nPAUCount; i++)
	{
		CString strTemp;
		if (!m_bIsCalibrated[i])
		{
			strTemp.Format(L"[力传感器]-%d 未校准!\n", i+1);
			strMsg += strTemp;
		}
		bIsCalibrated |= m_bIsCalibrated[i];
	}
	// 没有进行校准过
	if (!bIsCalibrated)
	{
		OnCancel();
		return;
	}
	// 没有未校准的PAU
	if (strMsg.IsEmpty())
	{
		OnCancel();
		return;
	}
	// 还有未校准的PAU
	if (MessageBox(strMsg+L"是否要退出？", _T("力校准"), MB_OKCANCEL) == IDOK)
	{
		OnCancel();
	}
}

/*
void CNHCForceCalibrationHeavyDlg::ExportEquCalChkInfo(void)
{
	// 导出检测结果到数据交换目录

	CString strPathOfDataExchange;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// 向上2层
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// 数据交换目录路径
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// 判断路径是否存在,并可以进行读写
	if (_taccess(strPathOfDataExchange, 06))
	{
		// 路径不存在

		// 创建路径
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// 创建路径失败,返回,不保存
			return;
		}
	}

	// 获取检测线编号和分析仪型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strDynamometerModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

	// 获取操作员
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	CString str;
	CString strResult(L"[EquCalChkInfo]");

	// 操作是否完成过至少一次
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce ? L"1" : L"0");
	strResult += str;

	// 检测线编号
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strResult += str;

	// 操作员
	str.Format(L"\r\nOperator=%s", strOperator);
	strResult += str;

	// 设备
	str.Format(L"\r\nEquipment=%s", strDynamometerModel);
	strResult += str;

	// 项目
	str.Format(L"\r\nItem=%s", L"测功机力校准");
	strResult += str;

	// 日期
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	str.Format(L"\r\nData=");
	strResult += str;

	// 误差
	CString strError(L"");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 判断
	str.Format(L"\r\nJudgement=%s", m_bIsPass ? L"通过" : L"不通过");
	strResult += str;

	// 写入到结果ini文件
	CString strPathOfEquCalChkInfo = strPathOfDataExchange + L"\\EquCalChkInfo.ini";
	if (0 == _taccess(strPathOfEquCalChkInfo, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfEquCalChkInfo);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfEquCalChkInfo, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfEquCalChkInfo, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}
*/

/*	
void CNHCForceCalibrationHeavyDlg::ExportDemarcationLog(void)
{	
	CString strPathOfDataExchange;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// 向上2层
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// 数据交换目录路径
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// 判断路径是否存在,并可以进行读写
	if (_taccess(strPathOfDataExchange, 06))
	{
		// 路径不存在

		// 创建路径
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// 创建路径失败,返回
			return;
		}
	}

	// 获取检测站编号
	CString strPathOfStationInfoFile = strPathOfDataExchange + L"\\StationInfo.ini";
	CSimpleIni siStationInfo(strPathOfStationInfoFile);
	CString strStationNumber = siStationInfo.GetString(L"StationInfo", L"StationNumber", L"");

	// 获取检测线编号和设备型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"DynamometerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

	// 获取操作员
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString str;
	CString strDemarcationLog(L"[DemarcationLog]");

	// 标定编号
	str.Format(L"\r\nDemarcationNumber=%s-%s-%s", strStationNumber, strLineNumber, odt.Format(L"%Y%m%d%H%M%S"));
	strDemarcationLog += str;

	// 检测线编号
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strDemarcationLog += str;

	// 操作员
	str.Format(L"\r\nOperator=%s", strOperator);
	strDemarcationLog += str;

	// 设备编号
	str.Format(L"\r\nEquipmentNumber=%s", strEquipmentNumber);
	strDemarcationLog += str;

	// 设备型号
	str.Format(L"\r\nEquipmentModel=%s", strEquipmentModel);
	strDemarcationLog += str;

	// 设备名称
	str.Format(L"\r\nEquipmentName=%s", L"底盘测功机");
	strDemarcationLog += str;

	// 标定项目
	str.Format(L"\r\nDemarcationItem=%d", DYN_FORCE_CAL);
	strDemarcationLog += str;

	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// 检查是否通过
	str.Format(L"\r\nPass=%s", m_bIsPass?L"1":L"0");
	strDemarcationLog += str;

	// 写入到结果ini文件
	CString strPathOfDemarcationLog = strPathOfDataExchange + L"\\DemarcationLog.ini";
	if (0 == _taccess(strPathOfDemarcationLog, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfDemarcationLog);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfDemarcationLog, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfDemarcationLog, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strDemarcationLog);
	sfe.Close();
}
*/
	
void CNHCForceCalibrationHeavyDlg::ExportDynForceCalibrationResult(void)
{		
	// 每校准一个PAU就导出一个日志
	CString str;
	CString strResult(L"[ResultOfDynForceCal]");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 校准是否通过
	str.Format(L"\r\nPass=%s", m_bIsPass[m_nCurPAU]?L"1":L"0");
	strResult += str;

	// 写入到结果ini文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCForceCalibrationHeavyDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

