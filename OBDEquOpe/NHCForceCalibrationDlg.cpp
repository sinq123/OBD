// NHCForceCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCForceCalibrationDlg.h"
#include "math.h"

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCForceCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CNHCForceCalibrationDlg, CDialogZoom)

CNHCForceCalibrationDlg::CNHCForceCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCForceCalibrationDlg::IDD, pParent)
{
    m_nTestStep = 0;
	m_nZeroTime = 0;
	m_fCalForce = 0;
	m_nStayTime = 0;
	m_bIsOperationFinishedAtLeaseOnce = false;
	m_bIsPass = true;

	ZeroMemory(m_fStayF, sizeof(float)*5);

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

CNHCForceCalibrationDlg::~CNHCForceCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCForceCalibrationDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CNHCForceCalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHCForceCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CNHCForceCalibrationDlg::OnBnClickedButtonReset)
	ON_MESSAGE(WM_DYN_READ_VAR,OnReadVar)
END_MESSAGE_MAP()


// CNHCForceCalibrationDlg 消息处理程序

BOOL CNHCForceCalibrationDlg::OnInitDialog()
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

void CNHCForceCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, CDynThread::IM_NANHUA_NHC03))
	{
		std::bitset<16> bit16(0);
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bit16))
		{
			m_lbInfo.SetText(_T("力校准前请勿安装校准工具"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
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

void CNHCForceCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
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
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(_T("请安装好校准工具后点下一步"));
			m_nTestStep = 1;
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		}
		break;

	case 3:
		{
			m_pDynThread->GetRealTimeData_Asyn(NULL);//获取实时数据
			CString str;
			str.Format(_T("%.1f"), m_pDynThread->GetFValue());
			m_lbTorsion.SetText(str);
		}
		break;

	case 4:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1)
			{
				m_nZeroTime ++;
				if (m_nZeroTime >= 2)//稳定1秒
				{
					m_nTestStep = 2;
					KillTimer(4);
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
			if (m_nStayTime >= 5)// 数组已经填满
			{
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					m_nTestStep = 4;
					KillTimer(5);
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
				if (m_nZeroTime >= 2)//稳定1秒
				{
					m_nTestStep = 6;
					KillTimer(6);
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
			if (m_nStayTime >= 5)// 数组已经填满
			{
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					m_nTestStep = 8;
					KillTimer(7);
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
				if (m_nZeroTime >= 2)//稳定1秒
				{
					m_nTestStep = 10;
					KillTimer(8);
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


void CNHCForceCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCForceCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机力校准");

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbReset.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	m_lbTorsion.SetLEDStyle();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	strPath = strPath + L"\\Config\\Demarcation.ini";

	CSimpleIni si(strPath);
	m_strWeightCount = si.GetString(L"DynForceCal", L"WeightCount", L"3");
}

void CNHCForceCalibrationDlg::ClearResult()             //步骤清零
{
	m_lbInfo.StopFlashText();
	m_lbInfo.SetText(_T("力校准前请勿安装校准工具"));
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

void CNHCForceCalibrationDlg::OnBnClickedButtonStart()
{
	if (m_nTestStep == 0)
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

			ClearResult();             //步骤清零

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

			m_pDynThread->ReadVar_Asyn(54, m_hWnd);// 取力传感器校准重量(南华)
			m_pDynThread->SendCtrlWord_Asyn('0');//退出所有控制模式
			m_pDynThread->SendCtrlWord_Asyn('U');//关闭自动零位调整
			m_pDynThread->SendCtrlWord_Asyn('L');//力矩零位校准(记录零位)

			m_lbInfo.SetText(_T("正在调整..."));
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == 1)        //校准补偿
	{
		// 判断是否已经安装校准工具
		if (fabs(m_pDynThread->GetFValue()) < 50.0f)
		{
			if (MessageBox(_T("请安装校准工具后点确认!"), _T("力校准"), MB_OKCANCEL) != IDOK)
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
	
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"√ 安装校准工具");

		m_lbInfo.SetText(_T("正在调整..."));
		m_pDynThread->SendCtrlWord_Asyn('T');//打开自动零位调整
		SetTimer(4, 500, NULL);
		m_nZeroTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (m_nTestStep == 2)        
	{
		m_lbInfo.SetText(_T("请放置")+m_strWeightCount+_T("个砝码后点下一步"));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		m_pDynThread->SendCtrlWord_Asyn('U');//关闭自动零位调整
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 3;
	}
	else if (m_nTestStep == 3)
	{
		// 判断砝码重量是否达到要求正负1000N范围
		if (m_fCalForce > 0.0f)
		{
			if (fabs(m_pDynThread->GetFValue()-m_fCalForce) > 1000.0f)
			{
				MessageBox(_T("砝码重量异常!"), _T("力校准"));
				m_bIsPass = false;
				m_nTestStep = 8;
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"× 放置砝码");
				m_lbStep3.SetTextColor(RGB(0, 0, 0));
				m_lbStep3.SetText(L"× 移走砝码");
				m_lbStep4.SetTextColor(RGB(0, 0, 0));
				m_lbStep4.SetText(L"× 放置砝码");
				OnBnClickedButtonStart();
				return;
			}
		}

        m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"√ 放置砝码");

		m_pDynThread->SendCtrlWord_Asyn('M');//校准力矩
		m_lbInfo.SetText(_T("正在调整..."));
		m_nStayTime = 0;
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		SetTimer(5, 1000, NULL);
	}
	else if (m_nTestStep == 4)
	{
		m_lbInfo.SetText(_T("请移走所有砝码后点下一步"));	
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 5;
	}
	else if (m_nTestStep == 5)
	{
        m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"√ 移走砝码");

		m_lbInfo.SetText(_T("正在调整..."));		
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');//打开自动零位调整
		SetTimer(6, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == 6)
	{
		m_pDynThread->SendCtrlWord_Asyn('U');//关闭自动零位调整

		m_lbInfo.SetText(_T("请放置")+m_strWeightCount+_T("个砝码后点下一步"));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 7;
	}
	else if (m_nTestStep == 7)
	{
		// 判断砝码重量是否达到要求正负1000N范围
		if (m_fCalForce > 0.0f)
		{
			if (fabs(m_pDynThread->GetFValue()-m_fCalForce) > 1000.0f)
			{
				MessageBox(_T("砝码重量异常!"), _T("力校准"));
				m_bIsPass = false;
				m_nTestStep = 8;
				m_lbStep4.SetTextColor(RGB(0, 0, 0));
				m_lbStep4.SetText(L"× 放置砝码");
				OnBnClickedButtonStart();
				return;
			}
		}

		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"√ 放置砝码");

		m_lbInfo.SetText(_T("正在调整..."));
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		m_nStayTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('N');//更新力矩校准数据	
		SetTimer(7, 1000, NULL);
	}
	else if (m_nTestStep == 8)
	{
		m_lbInfo.SetText(_T("请移走砝码和校准工具后点下一步"));	
		m_lbStep5.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 9;
	}
	else if (m_nTestStep == 9)
	{
		if (m_pDynThread->GetFValue() > 50.0f)
		{
			MessageBox(_T("请移走砝码和校准工具,再点击下一步"), _T("力校准"), MB_OK|MB_ICONWARNING);
			return;
		}

        m_lbStep5.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetText(L"√ 移走砝码和校准工具");

		m_lbInfo.SetText(_T("正在调整..."));
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');//打开自动零位调整
		SetTimer(8, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == 10)
	{
		m_lbStep6.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(_T("请点击‘完成校准’结束力校准"));
		m_pDynThread->SendCtrlWord_Asyn('V');    //把自动-偏移-调整设为正常
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("完成校准"));
		m_nTestStep = 11;

		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == 11)
	{
		m_lbInfo.SetText(_T("校准完成"));	
		m_lbStep6.SetTextColor(RGB(0, 0, 0));
		m_lbStep6.SetText(L"√ 完成");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	
		m_nTestStep = 0;
		//KillTimer(3);

		m_bIsOperationFinishedAtLeaseOnce = true;	//记录操作完成

		//ExportEquCalChkInfo();	// 导出行标结果
		//ExportDemarcationLog();	// 导出标定记录
		ExportDynForceCalibrationResult();	// 导出自定义结果

		// 回调函数获取校准信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"测功机力校准完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CAL_FINISH, wchInfo, sizeof(wchInfo));
		}
	}
}



void CNHCForceCalibrationDlg::OnBnClickedButtonReset()
{
	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar(29, 1000) 
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(55, 127)
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(56, 250))
	{
		m_lbInfo.SetText(_T("重置完成,请重新进行力校准"));
	}
	else
	{
		m_lbInfo.SetText(_T("重置失败"));
	}
	ClearResult();
}

LRESULT  CNHCForceCalibrationDlg::OnReadVar(WPARAM bSucess, LPARAM lParam)          //获取测功机状态
{
	if (bSucess != DYN_WR_SUCCEED)
	{
		m_lbInfo.SetText(_T("与测功机通讯超时"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
	}
	else
	{
		//if ((int)lParam == 6)
		//{
		//	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		//	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
		//}
		if ((int)lParam == 54)// 取得力传感器校准重量(南华)
		{
			m_fCalForce = (float)m_pDynThread->GetVarValue(54)/10.0f;
			SetTimer(2, 1500, NULL);
			SetTimer(3, 500, NULL);
		}
	}
	return 0;
}

/*
void CNHCForceCalibrationDlg::ExportEquCalChkInfo(void)
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
void CNHCForceCalibrationDlg::ExportDemarcationLog(void)
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
	
void CNHCForceCalibrationDlg::ExportDynForceCalibrationResult(void)
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

void CNHCForceCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}
