// NHCVarLoadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "math.h"
#include "NHCVarLoadDlg.h"

// 自定义
#include "..\NHDetCommModu\NHDetCommModu.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// 自用库
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT
// CNHCVarLoadDlg 对话框

IMPLEMENT_DYNAMIC(CNHCVarLoadDlg, CDialogZoom)

CNHCVarLoadDlg::CNHCVarLoadDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CNHCVarLoadDlg::IDD, pParent)
, m_ListReal(8)
, m_TestType(LUGDOWN)
, m_usDIW(907)
, m_fNominalTimeOfHigh(0.00)
, m_fNominalTimeOfLow(0.00)
, m_fRealityTimeOfHigh(0.00)
, m_fRealityTimeOfLow(0.00)
, m_fErrOfHigh(0.00)
, m_fErrOfLow(0.00)
, m_bResult(false)
{
	// 启动测功机控制线程
	m_pNHCVarLoadThread = (CNHCVarLoadThread*)AfxBeginThread(RUNTIME_CLASS(CNHCVarLoadThread));

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
}

CNHCVarLoadDlg::~CNHCVarLoadDlg()
{
	if (NULL != m_pNHCVarLoadThread)
	{
		m_pNHCVarLoadThread->Close();
		m_pNHCVarLoadThread->QuitThread();
		WaitForSingleObject(m_pNHCVarLoadThread->m_hThread, 5000);
	}
}

void CNHCVarLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_lbSpeed);
	DDX_Control(pDX, IDC_STATIC_POWER, m_lbPower);
	DDX_Control(pDX, IDC_STATIC_2, m_lbTime_Rel2);
	DDX_Control(pDX, IDC_STATIC_3, m_lbTime_Rel3);
	DDX_Control(pDX, IDC_STATIC_TIP, m_lbMsg);
	DDX_Control(pDX, IDC_STATIC_4, m_lbTime_Result);
	DDX_Control(pDX, IDC_STATIC_6, m_lbTime_Err2);
	DDX_Control(pDX, IDC_STATIC_7, m_lbTime_Err3);
	DDX_Control(pDX, IDC_STATIC_10, m_lbDIW);
	DDX_Control(pDX, IDC_LIST_REAL, m_ListReal);
	DDX_Control(pDX, IDC_STATIC_TIME2, m_lbTime2);
	DDX_Control(pDX, IDC_STATIC_TIME3, m_lbTime3);
	DDX_Control(pDX, IDC_RADIO1, m_CtrlRadio1);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCVarLoadDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_STARTTEST, &CNHCVarLoadDlg::OnBnClickedButtonStarttest)
	ON_BN_CLICKED(IDC_RADIO1, &CNHCVarLoadDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CNHCVarLoadDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CNHCVarLoadDlg::OnBnClickedRadio3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCVarLoadDlg::OnBnClickedButtonStopTest)
END_MESSAGE_MAP()


// CNHCVarLoadDlg 消息处理程序

void CNHCVarLoadDlg::OnBnClickedButtonStarttest()
{
	// TODO: 在此添加控件通知处理程序代码

	// 停止刷新数据
	EnableRefreshRealTimeData(false);

	// 检查滚筒是否有转动
	DWORD dw = m_pNHCVarLoadThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCVarLoadThread->GetVValue() > 0.05f)
		{
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"滚筒正在转动,请等滚筒停止再开始测试", L"变载荷滑行", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"通讯超时", L"变载荷滑行", MB_OK|MB_ICONERROR);
		return;
	}
	// 安全提示，测功机滚筒将要转动
	if (IDYES == MessageBox(L"测试过程滚筒将会高速转动!\n\n人和车不能在滚筒上面!\n\n请确认是否已经安全!", L"变载荷滑行", MB_YESNO|MB_ICONWARNING))
	{
		// 禁用开始按钮
		GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(false);
		// 禁用退出按钮
		GetDlgItem(IDOK)->EnableWindow(false);	
		// 开始刷新检测过程
		SetTimer(3, 50, NULL);
		if(LUGDOWN == m_TestType)
		{
			for(int i=0;i<9;i++)
			{
				m_ListReal.SetItemText(i,1,L"");
				m_ListReal.SetItemText(i,3,L"");
				m_ListReal.SetItemText(i,5,L"");
				m_ListReal.SetItemText(i,7,L"");

			}
		}
		else
		{
			for(int i=0;i<6;i++)
			{
				m_ListReal.SetItemText(i,1,L"");
				m_ListReal.SetItemText(i,3,L"");
				m_ListReal.SetItemText(i,5,L"");
				m_ListReal.SetItemText(i,7,L"");

			}
		}
		// 开始变载荷测试
		m_pNHCVarLoadThread->StartVarLoadTest_Asyn(m_TestType,m_hWnd);
	}
	else
	{
		// 开始刷新数据
		EnableRefreshRealTimeData(true);
		MessageBox(L"测试被取消", L"变载荷滑行", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();

}

BOOL CNHCVarLoadDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListReal.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	int nHeight = nSM_CYSCREEN * 30 / 1024;
	m_SmallImageList.Create(1,nHeight,0,0,0);
	m_ListReal.SetImageList(&m_SmallImageList, LVSIL_SMALL);
	CStringArray Head;
	CByteArray   Cols;

	Head.Add(L"速度");
	Cols.Add(12);       
	Head.Add(L"时间");
	Cols.Add(13);       
	Head.Add(L"速度");
	Cols.Add(12);       
	Head.Add(L"时间");
	Cols.Add(13);       
	Head.Add(L"速度");
	Cols.Add(12);       
	Head.Add(L"时间");
	Cols.Add(13);       
	Head.Add(L"速度");
	Cols.Add(12);       
	Head.Add(L"时间");
	Cols.Add(13);       
	m_ListReal.InitCtrl(&Head, &Cols);
	Head.RemoveAll();
	Cols.RemoveAll();
	InitCtrls();

	m_CtrlRadio1.SetCheck(TRUE);
	m_shdTitle.SetTitle(L"测功机变载荷滑行");

	ShowWindow(SW_MAXIMIZE);

	SetDlgFont();

	// 打开串口，并进行通讯测试
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCVarLoadDlg::InitCtrls(void)
{
	CStringArray Head;
	m_lbMsg.SetText(L"变载荷滑行测试");
	m_lbMsg.SetTitleStyle();

	m_lbSpeed.SetLEDStyle();
	m_lbPower.SetLEDStyle();
	m_lbTime_Rel2.SetLEDStyle();
	m_lbTime_Rel3.SetLEDStyle();


	m_lbTime_Result.SetLEDStyle();

	m_lbTime_Err2.SetLEDStyle();
	m_lbTime_Err3.SetLEDStyle();
	m_lbTime2.SetLEDStyle();
	m_lbTime3.SetLEDStyle();
	m_lbDIW.SetLEDStyle();

	m_lbMsg.SetBkColor(RGB(0,0,0));

	for (int i=0;i<8;i++)
	{
		m_ListReal.SetColumnWidth(i,85);
	}

	if (LUGDOWN == m_TestType)
	{

		///////////////////////////////////////////////////公制/////////////////////////////////////////////////////////////
		Head.RemoveAll();
		Head.Add(L"72.4");Head.Add(L"");Head.Add(L"57.9");Head.Add(L"");Head.Add(L"43.4");Head.Add(L"");Head.Add(L"29.0");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"70.8");Head.Add(L"");Head.Add(L"56.3");Head.Add(L"");Head.Add(L"41.8");Head.Add(L"");Head.Add(L"27.4");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"69.2");Head.Add(L"");Head.Add(L"54.7");Head.Add(L"");Head.Add(L"40.2");Head.Add(L"");Head.Add(L"25.7");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"67.6");Head.Add(L"");Head.Add(L"53.1");Head.Add(L"");Head.Add(L"38.6");Head.Add(L"");Head.Add(L"24.1");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"66.0");Head.Add(L"");Head.Add(L"51.5");Head.Add(L"");Head.Add(L"37.0");Head.Add(L"");Head.Add(L"22.5");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"64.4");Head.Add(L"");Head.Add(L"49.9");Head.Add(L"");Head.Add(L"35.4");Head.Add(L"");Head.Add(L"20.9");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"62.8");Head.Add(L"");Head.Add(L"48.3");Head.Add(L"");Head.Add(L"33.8");Head.Add(L"");Head.Add(L"19.3");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"61.1");Head.Add(L"");Head.Add(L"46.7");Head.Add(L"");Head.Add(L"32.2");Head.Add(L"");Head.Add(L"17.7");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"59.5");Head.Add(L"");Head.Add(L"45.1");Head.Add(L"");Head.Add(L"30.6");Head.Add(L"");Head.Add(L"16.1");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();

		for (int line=1;line<9;line=line+2)
		{
			m_ListReal.SetItemBackgndColor(RGB(234, 234, 234),line,0);
			m_ListReal.SetItemBackgndColor(RGB(237, 237, 237),line,1);
			m_ListReal.SetItemBackgndColor(RGB(205, 205, 205),line-1,0);
			m_ListReal.SetItemBackgndColor(RGB(210, 210, 210),line-1,1);

			m_ListReal.SetItemBackgndColor(RGB(240, 240, 240),line,2);
			m_ListReal.SetItemBackgndColor(RGB(243, 243, 243),line,3);

			m_ListReal.SetItemBackgndColor(RGB(215, 215, 215),line-1,2);
			m_ListReal.SetItemBackgndColor(RGB(220, 220, 220),line-1,3);
		}

		for (int line=1;line<9;line=line+2)
		{
			m_ListReal.SetItemBackgndColor(RGB(246, 246, 246),line,4);
			m_ListReal.SetItemBackgndColor(RGB(249, 249, 249),line,5);
			m_ListReal.SetItemBackgndColor(RGB(225, 225, 225),line-1,4);
			m_ListReal.SetItemBackgndColor(RGB(230, 230, 230),line-1,5);

			m_ListReal.SetItemBackgndColor(RGB(252, 252, 252),line,6);
			m_ListReal.SetItemBackgndColor(RGB(255, 255, 255),line,7);

			m_ListReal.SetItemBackgndColor(RGB(235, 235, 235),line-1,6);
			m_ListReal.SetItemBackgndColor(RGB(240, 240, 240),line-1,7);
		}
		m_ListReal.SetBkColor(RGB(243, 243, 243));
		m_lbTime_Rel2.SetText(L"--");
		m_lbTime_Rel3.SetText(L"--");
		m_lbTime_Result.SetText(L"--");
		m_lbTime_Err2.SetText(L"--");
		m_lbTime_Err3.SetText(L"--");
		m_lbTime2.SetText(L"--");
		m_lbTime3.SetText(L"--");
		GetDlgItem(IDC_STATIC_V_2)->SetWindowText(L"72.4-16.1");
		////////////////////更改///////////////////////////////////////////////////////
	}
	else
	{
		///////////////////////////////////////////////////VMAS ASM/////////////////////////////////////////////////////////////
		Head.RemoveAll();
		Head.Add(L"48.3");Head.Add(L"");Head.Add(L"38.6");Head.Add(L"");Head.Add(L"29.0");Head.Add(L"");Head.Add(L"19.3");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"46.7");Head.Add(L"");Head.Add(L"37.0");Head.Add(L"");Head.Add(L"27.4");Head.Add(L"");Head.Add(L"17.7");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"45.1");Head.Add(L"");Head.Add(L"35.4");Head.Add(L"");Head.Add(L"25.7");Head.Add(L"");Head.Add(L"16.1");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"43.4");Head.Add(L"");Head.Add(L"33.8");Head.Add(L"");Head.Add(L"24.1");Head.Add(L"");Head.Add(L"");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"41.8");Head.Add(L"");Head.Add(L"32.2");Head.Add(L"");Head.Add(L"22.5");Head.Add(L"");Head.Add(L"");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();
		Head.Add(L"40.2");Head.Add(L"");Head.Add(L"30.6");Head.Add(L"");Head.Add(L"20.9");Head.Add(L"");Head.Add(L"");Head.Add(L"");
		m_ListReal.AddItem(&Head);
		Head.RemoveAll();

		for (int line=1;line<7;line=line+2)
		{
			m_ListReal.SetItemBackgndColor(RGB(234, 234, 234),line,0);
			m_ListReal.SetItemBackgndColor(RGB(237, 237, 237),line,1);
			m_ListReal.SetItemBackgndColor(RGB(205, 205, 205),line-1,0);
			m_ListReal.SetItemBackgndColor(RGB(210, 210, 210),line-1,1);

			m_ListReal.SetItemBackgndColor(RGB(240, 240, 240),line,2);
			m_ListReal.SetItemBackgndColor(RGB(243, 243, 243),line,3);

			m_ListReal.SetItemBackgndColor(RGB(215, 215, 215),line-1,2);
			m_ListReal.SetItemBackgndColor(RGB(220, 220, 220),line-1,3);
		}

		for (int line=1;line<6;line=line+2)
		{
			m_ListReal.SetItemBackgndColor(RGB(246, 246, 246),line,4);
			m_ListReal.SetItemBackgndColor(RGB(249, 249, 249),line,5);
			m_ListReal.SetItemBackgndColor(RGB(225, 225, 225),line-1,4);
			m_ListReal.SetItemBackgndColor(RGB(230, 230, 230),line-1,5);

			m_ListReal.SetItemBackgndColor(RGB(252, 252, 252),line,6);
			m_ListReal.SetItemBackgndColor(RGB(255, 255, 255),line,7);

			m_ListReal.SetItemBackgndColor(RGB(235, 235, 235),line-1,6);
			m_ListReal.SetItemBackgndColor(RGB(240, 240, 240),line-1,7);
		}
		m_ListReal.SetBkColor(RGB(243, 243, 243));
		m_lbTime_Rel2.SetText(L"--");
		m_lbTime_Rel3.SetText(L"--");
		m_lbTime_Result.SetText(L"--");
		m_lbTime_Err2.SetText(L"--");
		m_lbTime_Err3.SetText(L"--");
		m_lbTime2.SetText(L"--");
		m_lbTime3.SetText(L"--");
		GetDlgItem(IDC_STATIC_V_2)->SetWindowText(L"--");
	}
}
void CNHCVarLoadDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ListReal.DeleteAllItems();
	GetDlgItem(IDC_STATIC_V_2)->SetWindowTextW(L"72.4-16.1");
	m_TestType = LUGDOWN;
	InitCtrls();
}

void CNHCVarLoadDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListReal.DeleteAllItems();
	GetDlgItem(IDC_STATIC_V_2)->SetWindowTextW(L"--");
	m_TestType = VMAS;
	InitCtrls();
}

void CNHCVarLoadDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListReal.DeleteAllItems();

	GetDlgItem(IDC_STATIC_V_2)->SetWindowTextW(L"--");

	m_TestType = ASM;
	InitCtrls();
}

void CNHCVarLoadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		// 打开串口，并测试通讯
		KillTimer(1);
		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}
	else if (3 == nIDEvent)
	{
		DisplayVarLoadTestProcess();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCVarLoadDlg::DisplayVarLoadTestProcess(void)
{
	switch (m_pNHCVarLoadThread->GetVarLoadState())
	{
	case NHC_VARLOAD_STATE_NOT_START:
		{
		}
		break;
	case NHC_VARLOAD_STATE_READY:
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"正在初始化测试参数...");
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
		}
		break;
	case NHC_VARLOAD_STATE_LIFTBEAM_DOWN:
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"正在降举升器...");
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
		}
		break;
	case NHC_VARLOAD_STATE_ACCELERATION:
		{
			EnableRefreshRealTimeData(false);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"正在加速...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCVarLoadThread->GetPValue());
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCVarLoadThread->GetVValue());
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
		}
		break;
	case NHC_VARLOAD_STATE_SLIP:
		{
			EnableRefreshRealTimeData(false);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"正在进行变载荷测试...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCVarLoadThread->GetPValue());
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCVarLoadThread->GetVValue());
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
			// 显示变载滑行时间
			DisplayVarLoadSldingTime();
		}
		break;
	case NHC_VARLOAD_STATE_WAIT_FOR_STOP:
		{
			EnableRefreshRealTimeData(true);
		}
	case NHC_VARLOAD_STATE_FINISHED:
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"测试完成");
			GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(TRUE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);

			// 停止刷新寄生功率滑行流程
			KillTimer(3);

			// 计算寄变载荷滑行结果
			CalculateVarLoadParams();

			// 显示数据
			ShowResult();
		}
		break;
	case NHC_VARLOAD_STATE_ERROR:
		{
			if (NHC_VARLOAD_COMMUNICATION_ERROR == m_pNHCVarLoadThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"测试终止,与测功机通讯超时");
			}
			else if (NHC_VARLOAD_START_MMTIMER_FAIL == m_pNHCVarLoadThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"测试终止,启动定时器失败");
			}
			//GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(L"--");
			//GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			//GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
			// 停止刷新加载滑行流程
			KillTimer(3);
			// 开始刷新数据
			EnableRefreshRealTimeData(true);
		}
		break;
	default:
		break;
	}
}
void CNHCVarLoadDlg::ShowResult(void)
{
	CString strTemp;
	strTemp.Format(L"%.2f",m_fNominalTimeOfHigh);
	GetDlgItem(IDC_STATIC_TIME2)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fNominalTimeOfLow);
	GetDlgItem(IDC_STATIC_TIME3)->SetWindowText(strTemp);

	strTemp.Format(L"%.2f",m_fRealityTimeOfHigh);
	GetDlgItem(IDC_STATIC_2)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fRealityTimeOfLow);
	GetDlgItem(IDC_STATIC_3)->SetWindowText(strTemp);

	strTemp.Format(L"%.2f",m_fErrOfHigh);
	GetDlgItem(IDC_STATIC_6)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fErrOfLow);
	GetDlgItem(IDC_STATIC_7)->SetWindowText(strTemp);

	if ((UINT)fabs(m_fErrOfHigh) > 4 || (UINT)fabs(m_fErrOfLow) > 4)
	{
		GetDlgItem(IDC_STATIC_4)->SetWindowText(L"不合格");
	}
	else
	{
		GetDlgItem(IDC_STATIC_4)->SetWindowText(L"合格");
	}


}
void CNHCVarLoadDlg::CalculateVarLoadParams()
{
	if (LUGDOWN == m_TestType)
	{
		m_fRealityTimeOfHigh = m_pNHCVarLoadThread->GetVarLoadTotalTime() / 1000.0f;
		m_fRealityTimeOfLow  = m_fRealityTimeOfHigh - (m_pNHCVarLoadThread->GetVarLoadTime(15) / 1000.0f);
		if (m_usDIW == 907)
		{
			m_fNominalTimeOfHigh = 15.53f;
			m_fNominalTimeOfLow = 6.41f;
		}
		else
		{
			m_fNominalTimeOfLow = (float)(m_usDIW*707)/100000 ;
			m_fNominalTimeOfHigh = (float)(m_usDIW*1713)/100000 ;

		}
		//72.4~16.1误差
		m_fErrOfHigh = ((m_fRealityTimeOfHigh- m_fNominalTimeOfHigh) / m_fNominalTimeOfHigh)*100;
		m_fErrOfLow =  ((m_fRealityTimeOfLow  - m_fNominalTimeOfLow) / m_fNominalTimeOfLow)*100;

	}
	else
	{
		m_fRealityTimeOfHigh = 0.00f;
		m_fNominalTimeOfHigh = 0.00f;
		m_fErrOfHigh = 0.00f;
		m_fRealityTimeOfLow = m_pNHCVarLoadThread->GetVarLoadTotalTime() / 1000.0f;
		if (m_usDIW == 907)
		{
			m_fNominalTimeOfLow = 6.41f;
		}
		else
		{
			m_fNominalTimeOfLow = (float)(m_usDIW*707)/100000 ;
		}
		//48.3~16.1误差
		m_fErrOfLow = ((m_fRealityTimeOfLow- m_fNominalTimeOfLow) / m_fNominalTimeOfLow)*100;
	}
}
void CNHCVarLoadDlg::DisplayVarLoadSldingTime(void)
{
	// 要注意,单位ms－>s

	if (ASM == m_TestType)
	{
		// 刷新ASM变载荷滑行实时数据
		DisplayVarLoadSldingTime_ASM();
	}
	else if (VMAS == m_TestType)
	{
		// 刷新VMAS变载荷滑行实时数据
		DisplayVarLoadSldingTime_VMAS();
	}
	else
	{
		// 刷新LUGDOWN变载荷滑行实时数据
		DisplayVarLoadSldingTime_LUGDOWN();
	}
}
void CNHCVarLoadDlg::DisplayVarLoadSldingTime_ASM(void)
{
	CString str(L"");
	if (m_pNHCVarLoadThread->GetVValue() >=46.7f)
	{
		// 刷新第1段滑行时间(48.3，固定是0秒)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(0)/1000.0f);
		m_ListReal.SetItemText(0, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=45.1f)
	{
		// 刷新第2段滑行时间(48.3~46.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(1)/1000.0f);
		m_ListReal.SetItemText(1, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=43.4f)
	{
		// 刷新第3段滑行时间(46.7~43.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(2)/1000.0f);
		m_ListReal.SetItemText(2, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=41.8f)
	{
		// 刷新第4段滑行时间(43.4~41.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(3)/1000.0f);
		m_ListReal.SetItemText(3, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=40.2f)
	{
		// 刷新第2段滑行时间(41.8~40.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(4)/1000.0f);
		m_ListReal.SetItemText(4, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=38.6f)
	{
		// 刷新第3段滑行时间(40.2~38.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(5)/1000.0f);
		m_ListReal.SetItemText(5, 1, str);

	}
	else if (m_pNHCVarLoadThread->GetVValue()>=37.0f)
	{
		// 刷新第4段滑行时间(38.6~37)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(6)/1000.0f);
		m_ListReal.SetItemText(0, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=35.4f)
	{
		// 刷新第4段滑行时间(37~35.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(7)/1000.0f);
		m_ListReal.SetItemText(1, 3, str);

	}
	else if (m_pNHCVarLoadThread->GetVValue()>=33.8f)
	{
		// 刷新第5段滑行时间(35.4~33.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(8)/1000.0f);
		m_ListReal.SetItemText(2, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=32.2f)
	{
		// 刷新第5段滑行时间(33.8~32.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(9)/1000.0f);
		m_ListReal.SetItemText(3, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=30.6f)
	{
		// 刷新第5段滑行时间(32.2~30.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(10)/1000.0f);
		m_ListReal.SetItemText(4, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=29.0f)
	{
		// 刷新第5段滑行时间(30.6~29)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(11)/1000.0f);
		m_ListReal.SetItemText(5, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=27.4f)
	{
		// 刷新第5段滑行时间(29~27.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(12)/1000.0f);
		m_ListReal.SetItemText(0, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=25.7f)
	{
		// 刷新第5段滑行时间(27.4~25.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(13)/1000.0f);
		m_ListReal.SetItemText(1, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=24.1f)
	{
		// 刷新第5段滑行时间(25.7~24.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(14)/1000.0f);
		m_ListReal.SetItemText(2, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=22.5f)
	{
		// 刷新第5段滑行时间(24.1~22.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(15)/1000.0f);
		m_ListReal.SetItemText(3, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=20.9f)
	{
		// 刷新第5段滑行时间(22.5~20.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(16)/1000.0f);
		m_ListReal.SetItemText(4, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=19.3f)
	{
		// 刷新第5段滑行时间(20.9~19.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(17)/1000.0f);
		m_ListReal.SetItemText(5, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=17.7f)
	{
		// 刷新第5段滑行时间(19.3~17.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(18)/1000.0f);
		m_ListReal.SetItemText(0, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=16.1f)
	{
		// 刷新第5段滑行时间(17.7~16.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(19)/1000.0f);
		m_ListReal.SetItemText(1, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=11.0f)
	{
		// 刷新第5段滑行时间(19~13)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(20)/1000.0f);
		m_ListReal.SetItemText(2, 7, str);
	}

}

void CNHCVarLoadDlg::DisplayVarLoadSldingTime_VMAS(void)
{
	CString str(L"");
	if (m_pNHCVarLoadThread->GetVValue() >=46.7f)
	{
		// 刷新第1段滑行时间(48.3，固定是0秒)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(0)/1000.0f);
		m_ListReal.SetItemText(0, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=45.1f)
	{
		// 刷新第2段滑行时间(48.3~46.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(1)/1000.0f);
		m_ListReal.SetItemText(1, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=43.4f)
	{
		// 刷新第3段滑行时间(46.7~43.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(2)/1000.0f);
		m_ListReal.SetItemText(2, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=41.8f)
	{
		// 刷新第4段滑行时间(43.4~41.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(3)/1000.0f);
		m_ListReal.SetItemText(3, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=40.2f)
	{
		// 刷新第2段滑行时间(41.8~40.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(4)/1000.0f);
		m_ListReal.SetItemText(4, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=38.6f)
	{
		// 刷新第3段滑行时间(40.2~38.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(5)/1000.0f);
		m_ListReal.SetItemText(5, 1, str);

	}
	else if (m_pNHCVarLoadThread->GetVValue()>=37.0f)
	{
		// 刷新第4段滑行时间(38.6~37)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(6)/1000.0f);
		m_ListReal.SetItemText(0, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=35.4f)
	{
		// 刷新第4段滑行时间(37~35.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(7)/1000.0f);
		m_ListReal.SetItemText(1, 3, str);

	}
	else if (m_pNHCVarLoadThread->GetVValue()>=33.8f)
	{
		// 刷新第5段滑行时间(35.4~33.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(8)/1000.0f);
		m_ListReal.SetItemText(2, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=32.2f)
	{
		// 刷新第5段滑行时间(33.8~32.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(9)/1000.0f);
		m_ListReal.SetItemText(3, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=30.6f)
	{
		// 刷新第5段滑行时间(32.2~30.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(10)/1000.0f);
		m_ListReal.SetItemText(4, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=29.0f)
	{
		// 刷新第5段滑行时间(30.6~29)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(11)/1000.0f);
		m_ListReal.SetItemText(5, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=27.4f)
	{
		// 刷新第5段滑行时间(29~27.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(12)/1000.0f);
		m_ListReal.SetItemText(0, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=25.7f)
	{
		// 刷新第5段滑行时间(27.4~25.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(13)/1000.0f);
		m_ListReal.SetItemText(1, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=24.1f)
	{
		// 刷新第5段滑行时间(25.7~24.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(14)/1000.0f);
		m_ListReal.SetItemText(2, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=22.5f)
	{
		// 刷新第5段滑行时间(24.1~22.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(15)/1000.0f);
		m_ListReal.SetItemText(3, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=20.9f)
	{
		// 刷新第5段滑行时间(22.5~20.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(16)/1000.0f);
		m_ListReal.SetItemText(4, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=19.3f)
	{
		// 刷新第5段滑行时间(20.9~19.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(17)/1000.0f);
		m_ListReal.SetItemText(5, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=17.7f)
	{
		// 刷新第5段滑行时间(19.3~17.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(18)/1000.0f);
		m_ListReal.SetItemText(0, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=16.1f)
	{
		// 刷新第5段滑行时间(17.7~16.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(19)/1000.0f);
		m_ListReal.SetItemText(1, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=11.0f)
	{
		// 刷新第5段滑行时间(19~13)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(20)/1000.0f);
		m_ListReal.SetItemText(2, 7, str);
	}
}

void CNHCVarLoadDlg::DisplayVarLoadSldingTime_LUGDOWN(void)
{
	CString str(L"");
	if (m_pNHCVarLoadThread->GetVValue() >=72.4f)
	{
		// 刷新第1段滑行时间(72.4固定是0秒)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(0)/1000.0f);
		m_ListReal.SetItemText(0, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=69.2f)
	{
		// 刷新第2段滑行时间(72.4~70.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(1)/1000.0f);
		m_ListReal.SetItemText(1, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=67.6f)
	{
		// 刷新第3段滑行时间(70.8~69.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(2)/1000.0f);
		m_ListReal.SetItemText(2, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=66.0f)
	{
		// 刷新第4段滑行时间(70.8~69.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(3)/1000.0f);
		m_ListReal.SetItemText(3, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=64.4f)
	{
		// 刷新第5段滑行时间(69.2~66.0)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(4)/1000.0f);
		m_ListReal.SetItemText(4, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=62.8f)
	{
		// 刷新第6段滑行时间(66.0~64.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(5)/1000.0f);
		m_ListReal.SetItemText(5, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=61.1f)
	{
		// 刷新第7段滑行时间(64.4~62.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(6)/1000.0f);
		m_ListReal.SetItemText(6, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=59.5f)
	{
		// 刷新第8段滑行时间(62.8~61.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(7)/1000.0f);
		m_ListReal.SetItemText(7, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=57.9f)
	{
		// 刷新第9段滑行时间(61.1~59.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(8)/1000.0f);
		m_ListReal.SetItemText(8, 1, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=56.3f)
	{
		// 刷新第10段滑行时间(59.5~57.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(9)/1000.0f);
		m_ListReal.SetItemText(0, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=54.7f)
	{
		// 刷新第11段滑行时间(57.9~56.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(10)/1000.0f);
		m_ListReal.SetItemText(1, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=53.1f)
	{
		// 刷新第12段滑行时间(56.3~54.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(11)/1000.0f);
		m_ListReal.SetItemText(2, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=51.5f)
	{
		// 刷新第13段滑行时间(54.7~53.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(12)/1000.0f);
		m_ListReal.SetItemText(3, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=49.9f)
	{
		// 刷新第14段滑行时间(53.1~51.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(13)/1000.0f);
		m_ListReal.SetItemText(4, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=48.3f)
	{
		// 刷新第15段滑行时间(51.5~53.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(14)/1000.0f);
		m_ListReal.SetItemText(5, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=46.7f)
	{
		// 刷新第16段滑行时间(53.1~84.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(15)/1000.0f);
		m_ListReal.SetItemText(6, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=45.1f)
	{
		// 刷新第17段滑行时间(48.3~46.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(16)/1000.0f);
		m_ListReal.SetItemText(7, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue() >=43.4f)
	{
		// 刷新第18段滑行时间(46.7~45.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(17)/1000.0f);
		m_ListReal.SetItemText(8, 3, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=41.8f)
	{
		// 刷新第19段滑行时间(45.1~43.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(18)/1000.0f);
		m_ListReal.SetItemText(0, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=40.2f)
	{
		// 刷新第20段滑行时间(43.4~41.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(19)/1000.0f);
		m_ListReal.SetItemText(1, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=38.6f)
	{
		// 刷新第21段滑行时间(41.8~40.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(20)/1000.0f);
		m_ListReal.SetItemText(2, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=37.0f)
	{
		// 刷新第22段滑行时间(40.2~38.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(21)/1000.0f);
		m_ListReal.SetItemText(3, 5, str);

	}
	else if (m_pNHCVarLoadThread->GetVValue()>=35.4f)
	{
		// 刷新第23段滑行时间(38.6~37)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(22)/1000.0f);
		m_ListReal.SetItemText(4, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=33.8f)
	{
		// 刷新第24段滑行时间(37~35.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(23)/1000.0f);
		m_ListReal.SetItemText(5, 5, str);

	}
	else if (m_pNHCVarLoadThread->GetVValue()>=32.2f)
	{
		// 刷新第25段滑行时间(35.4~33.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(24)/1000.0f);
		m_ListReal.SetItemText(6, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=30.6f)
	{
		// 刷新第26段滑行时间(33.8~32.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(25)/1000.0f);
		m_ListReal.SetItemText(7, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=29.0f)
	{
		// 刷新第27段滑行时间(32.2~30.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(26)/1000.0f);
		m_ListReal.SetItemText(8, 5, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=27.4f)
	{
		// 刷新第28段滑行时间(30.6~29)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(27)/1000.0f);
		m_ListReal.SetItemText(0, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=25.7f)
	{
		// 刷新第29段滑行时间(29~27.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(28)/1000.0f);
		m_ListReal.SetItemText(1, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=24.1f)
	{
		// 刷新第30段滑行时间(27.4~25.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(29)/1000.0f);
		m_ListReal.SetItemText(2, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=22.5f)
	{
		// 刷新第31段滑行时间(25.7~24.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(30)/1000.0f);
		m_ListReal.SetItemText(3, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=20.9f)
	{
		// 刷新第32段滑行时间(24.1~22.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(31)/1000.0f);
		m_ListReal.SetItemText(4, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=19.3f)
	{
		// 刷新第33段滑行时间(22.5~20.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(32)/1000.0f);
		m_ListReal.SetItemText(5, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=17.7f)
	{
		// 刷新第34段滑行时间(20.9~19.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(33)/1000.0f);
		m_ListReal.SetItemText(6, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=16.10f)
	{
		// 刷新第35段滑行时间(19.3~17.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(34)/1000.0f);
		m_ListReal.SetItemText(7, 7, str);
	}
	else if (m_pNHCVarLoadThread->GetVValue()>=11.0f)
	{
		// 刷新第36段滑行时间(17.7~16.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread->GetVarLoadTime(35)/1000.0f);
		m_ListReal.SetItemText(8, 7, str);
	}
}


void CNHCVarLoadDlg::EnableRefreshRealTimeData(bool bEnable)
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
void CNHCVarLoadDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}
void CNHCVarLoadDlg::RefreshRealTimeData(void)
{
	// 刷新主数据
	// 同时进行通讯测试
	// 500毫秒执行一次

	CString str(L"");

	//if (DYN_WR_SUCCEED == m_pNHCVarLoadThread->GetRealTimeData_Asyn())
	m_pNHCVarLoadThread->GetRealTimeData_Asyn();
	{
		// 通讯成功
		str.Format(L"%.2f", m_pNHCVarLoadThread->GetPValue());
		GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCVarLoadThread->GetVValue());
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
	}
	//else
	//{
	//	// 通讯失败
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
	//}
}

void CNHCVarLoadDlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pNHCVarLoadThread->Open(bPort, CDynThread::IM_NANHUA_NHC03))
	{
		// 进行通讯测试
		// 用取基本惯量进行通讯测试
		if (DYN_WR_SUCCEED == m_pNHCVarLoadThread->GetDIW(&m_usDIW))
		{
			// 显示基本惯量
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_10)->SetWindowTextW(str);


			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("测功机变载荷滑行测试"));
			GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
	}
}

void CNHCVarLoadDlg::OnBnClickedButtonStopTest()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("停止"));
	GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);	
	GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);

	m_pNHCVarLoadThread ->StopVarLoadTest();
	KillTimer(3);
	// 开始刷新数据
	EnableRefreshRealTimeData(true);
}
