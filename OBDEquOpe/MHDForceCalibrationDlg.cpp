// MHDForceCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MHDForceCalibrationDlg.h"
#include <cmath>

// CMHDForceCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CMHDForceCalibrationDlg, CDialogZoom)

CMHDForceCalibrationDlg::CMHDForceCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CMHDForceCalibrationDlg::IDD, pParent)
	, m_nTestStep(0)
	, m_nZeroTime(0)
	, m_fCalForce(0)
	, m_nStayTime(0)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_bIsPass(true)
	, m_pfProcessCtrl(NULL)
{
	// 针对1280*1024分辨率进行开发
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		L"宋体");

	// 启动测功机线程
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	ZeroMemory(m_fStayF, sizeof(m_fStayF));
}

CMHDForceCalibrationDlg::~CMHDForceCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	// 停止测功机线程
	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CMHDForceCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_N, m_lbTorsion);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_STATIC_STEP6, m_lbStep6);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_STEP, m_lbStep);
	DDX_Control(pDX, IDC_STATIC_RESET, m_lbReset);
}


BEGIN_MESSAGE_MAP(CMHDForceCalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMHDForceCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMHDForceCalibrationDlg::OnBnClickedButtonReset)
	//ON_MESSAGE(WM_DYN_READ_VAR,OnReadVar)
	ON_BN_CLICKED(IDCANCEL, &CMHDForceCalibrationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMHDForceCalibrationDlg 消息处理程序

BOOL CMHDForceCalibrationDlg::OnInitDialog()
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

void CMHDForceCalibrationDlg::OpenSerialPortAndTestCommunication(void)
{
	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	// 配置文件路径
	strPath = strPath + L"\\Config\\Equipment.ini";

	CSimpleIni si(strPath);
	// 获取通讯端口
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, CDynThread::IM_MAHA_ASMP))
	{
		// 用获取校准力m_fCalForce测试通讯
		// 顺便把校准力记录下来
		WORD w(0);
		if (DYN_WR_SUCCEED == m_pDynThread->ReadVar(88, &w))
		{
			m_fCalForce = w / 10.0f;

			m_lbInfo.SetText(L"力校准前请勿安装校准工具");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			m_lbInfo.SetText(L"通讯超时");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(L"打开通讯端口失败");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	}
}

void CMHDForceCalibrationDlg::EnableRefreshRealTimeData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(3, 500, NULL);
	}
	else
	{
		KillTimer(3);
	}
}

void CMHDForceCalibrationDlg::RefreshRealTimeData(void)
{
	m_pDynThread->GetRealTimeData_Asyn(NULL);
	CString str;
	str.Format(L"%.1f", m_pDynThread->GetFValue());
	m_lbTorsion.SetText(str);
}

void CMHDForceCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			OpenSerialPortAndTestCommunication();
		}
		break;

	case 2:
		{
			KillTimer(2);
			m_nTestStep = 1;
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(L"请安装好校准工具后点下一步");
			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		}
		break;

	case 3:
		{
			RefreshRealTimeData();
		}
		break;

	case 4:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{
				m_nZeroTime++;
				// 稳定1秒
				if (m_nZeroTime >= 2)
				{
					KillTimer(4);
					m_nTestStep = 2;
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case 5:
		{
			// 判断是否调整完成
			if (m_nStayTime >= 5)
			{
				// 数组已经填满
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					KillTimer(5);
					m_nTestStep = 4;
					OnBnClickedButtonStart();
				}
			}
			m_fStayF[m_nStayTime%5] = m_pDynThread->GetFValue();
			m_nStayTime++;
		}
		break;

	case 6:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{	
				m_nZeroTime++;
				// 稳定1秒
				if (m_nZeroTime >= 2)
				{
					KillTimer(6);
					m_nTestStep = 6;
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case 7:
		{
			// 判断是否调整完成
			if (m_nStayTime >= 5)
			{
				// 数组已经填满
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					KillTimer(7);
					m_nTestStep = 8;
					OnBnClickedButtonStart();
				}
			}
			m_fStayF[m_nStayTime%5] = m_pDynThread->GetFValue();
			m_nStayTime++;
		}
		break;

	case 8:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{	
				m_nZeroTime++;
				// 稳定1秒
				if (m_nZeroTime >= 2)
				{
					KillTimer(8);
					m_nTestStep = 10;
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
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CMHDForceCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CMHDForceCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机力校准");

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbReset.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetText(L"正在通讯...");
	m_lbInfo.SetTitleStyle();

	m_lbTorsion.SetLEDStyle();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
}

void CMHDForceCalibrationDlg::ClearResult()
{
	m_lbInfo.StopFlashText();
	m_lbInfo.SetText(L"力校准前请勿安装校准工具");
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep5.SetTextColor(RGB(0, 0, 0));
	m_lbStep6.SetTextColor(RGB(0, 0, 0));

	m_lbStep1.SetText(L"   安装校准工具");
	m_lbStep2.SetText(L"   放置砝码");
	m_lbStep3.SetText(L"   移走砝码");
	m_lbStep4.SetText(L"   放置砝码");
	m_lbStep5.SetText(L"   移走砝码和校准工具");
	m_lbStep6.SetText(L"   完成");

	m_nTestStep = 0;
	m_nZeroTime = 0;
	m_fCalForce = 0;	
	m_bIsPass = true;
}

void CMHDForceCalibrationDlg::OnBnClickedButtonStart()
{
	if (m_nTestStep == 0)
	{	
		m_lbInfo.SetText(L"力校准前请勿安装校准工具");

		if (MessageBox(L"确定开始校准吗?", L"力校准", MB_OKCANCEL) == IDOK)
		{
			// 回调输出信息
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机力校准开始", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_DYN_FORCE_CAL_START, wchInfo, sizeof(wchInfo));
			}

			// 步骤清零
			ClearResult();

			BeginWaitCursor();
			EnableRefreshRealTimeData(false);

			// 退出所有控制模式
			m_pDynThread->SendCtrlWord('0');
			// 关闭自动零位调整
			m_pDynThread->SendCtrlWord('U');
			// 力矩零位校准(记录零位)
			m_pDynThread->SendCtrlWord('L');

			EnableRefreshRealTimeData(true);
			EndWaitCursor();

			SetTimer(2, 1500, NULL);

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"下一步");

			m_lbInfo.SetText(L"正在调整...");
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == 1)
	{
		// 校准补偿
		// 判断是否已经安装校准工具
		if (fabs(m_pDynThread->GetFValue()) < 50.0f)
		{
			if (MessageBox(L"请安装校准工具后点确定", L"力校准", MB_OKCANCEL) != IDOK)
			{
				m_nTestStep = 8;
				m_bIsPass = false;
				m_lbStep1.SetTextColor(RGB(0, 0, 0));
				m_lbStep1.SetText(L"× 安装校准工具");
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"× 放置砝码");
				m_lbStep3.SetTextColor(RGB(0, 0, 0));
				m_lbStep3.SetText(L"× 移走砝码");
				m_lbStep4.SetTextColor(RGB(0, 0, 0));
				m_lbStep4.SetText(L"× 放置砝码");
			}
			OnBnClickedButtonStart();
			return;
		}

		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 打开自动零位调整
		m_pDynThread->SendCtrlWord('T');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"√ 安装校准工具");
		m_lbInfo.SetText(L"正在调整...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nZeroTime = 0;
		SetTimer(4, 500, NULL);
	}
	else if (m_nTestStep == 2)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 关闭自动零位调整
		m_pDynThread->SendCtrlWord('U');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"请放置5个砝码后点下一步");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 3;
	}
	else if (m_nTestStep == 3)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 校准力矩
		m_pDynThread->SendCtrlWord('M');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

        m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"√ 放置砝码");
		m_lbInfo.SetText(L"正在调整...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nStayTime = 0;
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		SetTimer(5, 1000, NULL);
	}
	else if (m_nTestStep == 4)
	{
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"请移走所有砝码后点下一步");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 5;
	}
	else if (m_nTestStep == 5)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 打开自动零位调整
		m_pDynThread->SendCtrlWord('T');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

        m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"√ 移走砝码");
		m_lbInfo.SetText(L"正在调整...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nZeroTime = 0;
		SetTimer(6, 500, NULL);
	}
	else if (m_nTestStep == 6)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 关闭自动零位调整
		m_pDynThread->SendCtrlWord('U');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbInfo.SetText(L"请放置5个砝码后点下一步");
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 7;
	}
	else if (m_nTestStep == 7)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 更新力矩校准数据	
		m_pDynThread->SendCtrlWord('N');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"√ 放置砝码");
		m_lbInfo.SetText(L"正在调整...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nStayTime = 0;
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		SetTimer(7, 1000, NULL);
	}
	else if (m_nTestStep == 8)
	{
		m_lbStep5.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"请移走砝码和校准工具后点下一步");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 9;
	}
	else if (m_nTestStep == 9)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 打开自动零位调整
		m_pDynThread->SendCtrlWord('T');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

        m_lbStep5.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetText(L"√ 移走砝码和校准工具");
		m_lbInfo.SetText(L"正在调整...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nZeroTime = 0;
		SetTimer(8, 500, NULL);
	}
	else if (m_nTestStep == 10)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// 把自动-偏移-调整设为正常
		m_pDynThread->SendCtrlWord('V');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep6.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"请点击‘完成校准’结束力校准");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"完成校准");

		m_nTestStep = 11;
		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == 11)
	{
		m_lbStep6.SetTextColor(RGB(0, 0, 0));
		m_lbStep6.SetText(L"√ 完成");
		m_lbInfo.SetText(L"校准完成");	
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	
		m_nTestStep = 0;

		// 记录操作完成
		m_bIsOperationFinishedAtLeaseOnce = true;

		// 导出结果数据
		ExportDynForceCalibrationResult();

		// 回调函数获取校准信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机力校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CAL_FINISH, wchInfo, sizeof(wchInfo));
		}
	}
}

void CMHDForceCalibrationDlg::OnBnClickedButtonReset()
{
	if (IDOK != MessageBox(L"确定重置力校准参数吗?\r\n\r\n重置力校准参数后,需要重新进行力校准", L"力校准", MB_OKCANCEL|MB_ICONINFORMATION))
	{
		return;
	}

	BeginWaitCursor();
	EnableRefreshRealTimeData(false);

	// 打开写保护
	m_pDynThread->WriteVar(0, 9864);

	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar(29, 1000) 
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(55, 127)
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(56, 250))
	{
		m_lbInfo.SetText(L"重置完成,请重新进行力校准");
	}
	else
	{
		m_lbInfo.SetText(L"重置失败");
	}
	ClearResult();

	// 关闭写保护
	m_pDynThread->WriteVar(0, 0);

	EnableRefreshRealTimeData(true);
	EndWaitCursor();
}

//LRESULT  CMHDForceCalibrationDlg::OnReadVar(WPARAM bSucess, LPARAM lParam)          //获取测功机状态
//{
//	if (bSucess != DYN_WR_SUCCEED)
//	{
//		m_lbInfo.SetText(L"与测功机通讯超时");
//		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
//		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
//		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
//	}
//	else
//	{
//		if ((int)lParam == 54)// 取得力传感器校准重量(南华)
//		{
//			m_fCalForce = (float)m_pDynThread->GetVarValue(54)/10.0f;
//		}
//	}
//	return 0;
//}

void CMHDForceCalibrationDlg::ExportDynForceCalibrationResult(void)
{	
	CString str;
	CString strResult(L"[ResultOfDynForceCal]");
	
	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 校准是否通过
	str.Format(L"\r\nPass=%s", m_bIsPass?L"1":L"0");
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

void CMHDForceCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CMHDForceCalibrationDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	EnableRefreshRealTimeData(false);

	OnCancel();
}
