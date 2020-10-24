// NHT1LCalibrationCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHT1LCalibrationCheckDlg.h"
#include "math.h"

// CNHT1LCalibrationCheckDlg 对话框

IMPLEMENT_DYNAMIC(CNHT1LCalibrationCheckDlg, CDialogZoom)

CNHT1LCalibrationCheckDlg::CNHT1LCalibrationCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHT1LCalibrationCheckDlg::IDD, pParent)
{
	// 启动不透光线程
	m_pOpaThread = (COpaThread*)AfxBeginThread(RUNTIME_CLASS(COpaThread));

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

	ZeroMemory(m_nN1, sizeof(int)*24);
	ZeroMemory(m_nN2, sizeof(int)*24);

	m_nTestStep = STEP_PREPARE;
	m_nTime = 0;
	m_nAELimit = 150;
	m_nLinearLimit = 110;
	m_nStdN1 = 0;
	m_nStdN2 = 0;

	m_bIsLinear1Pass = true;	
	m_bIsLinear2Pass = true;	
	m_bIsAE1Pass = true;		
	m_bIsAE2Pass = true;		
	m_bIsN1Pass = true;	
	m_bIsN2Pass = true;
	m_bIsCalChkPass = true;
	m_bIsOperationFinishedAtLeaseOnce = false;
}

CNHT1LCalibrationCheckDlg::~CNHT1LCalibrationCheckDlg()
{	
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// 退出程序时,如果操作1次都没有完成过,也要导出相应结果
		ExportCalChkResult();
	}
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pOpaThread)
	{
		m_pOpaThread->Close();
		m_pOpaThread->QuitThread();
		WaitForSingleObject(m_pOpaThread->m_hThread, 5000);
	}
}

void CNHT1LCalibrationCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LB1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LB2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LB3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_N, m_lbN);
	DDX_Control(pDX, IDC_STATIC_K, m_lbK);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResult);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_ML1, m_mlK);
	DDX_Control(pDX, IDC_EDIT_STDN1, m_edStdN1);
	DDX_Control(pDX, IDC_EDIT_STDN2, m_edStdN2);
	DDX_Control(pDX, IDC_STATIC_AVG1, m_lbAvg1);
	DDX_Control(pDX, IDC_STATIC_AVG2, m_lbAvg2);
	DDX_Control(pDX, IDC_STATIC_LINEAR1, m_lbLinear1);
	DDX_Control(pDX, IDC_STATIC_LINEAR2, m_lbLinear2);
	DDX_Control(pDX, IDC_STATIC_ERR1, m_lbError1);
	DDX_Control(pDX, IDC_STATIC_ERR2, m_lbError2);
	DDX_Control(pDX, IDC_STATIC_JUDGEMENT, m_lbJudgement);
}


BEGIN_MESSAGE_MAP(CNHT1LCalibrationCheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHT1LCalibrationCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHT1LCalibrationCheckDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CNHT1LCalibrationCheckDlg 消息处理程序

BOOL CNHT1LCalibrationCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化控件
	InitCtrls();

	// 延时打开串口
	SetTimer(TIMER_OPEN_COM, 500, NULL);

	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHT1LCalibrationCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHT1LCalibrationCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	//设置输入小数位数,不可以为负数
	m_edStdN1.AllowNegative(FALSE);
	m_edStdN2.AllowNegative(FALSE);
	m_edStdN1.SetMaxDecimalPlaces();
	m_edStdN2.SetMaxDecimalPlaces();

	// 设置上下标文本控件
	m_mlK.SetScriptText(L"K[m^-^1]:");
	// N原始实时值
	m_lbN.SetLEDStyle();
	m_lbN.SetText(L"--");
	// K原始实时值
	m_lbK.SetLEDStyle();
	m_lbK.SetText(L"--");
	// 设置标签颜色
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lbAvg1.SetBkColor(RGB(230, 230, 230));	
	m_lbAvg2.SetBkColor(RGB(230, 230, 230));
	m_lbLinear1.SetBkColor(RGB(230, 230, 230));
	m_lbLinear2.SetBkColor(RGB(230, 230, 230));
	m_lbError1.SetBkColor(RGB(230, 230, 230));
    m_lbError2.SetBkColor(RGB(230, 230, 230));
	m_lbJudgement.SetBkColor(RGB(230, 230, 230));

	m_lbTitle.SetTitle(_T("不透光度计校准检查"));
	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	//列表
	m_lstResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstResult.InsertColumn(0, _T("时间[s]"), 0, 110);
	m_lstResult.InsertColumn(1, _T("滤光片1[%]"), 0, 150);
	m_lstResult.InsertColumn(2, _T("滤光片2[%]"), 0, 150);
	m_lstResult.InsertColumn(3, _T(""), 0, 50);
	m_lstResult.InsertColumn(4, _T("时间[s]"), 0, 110);
	m_lstResult.InsertColumn(5, _T("滤光片1[%]"), 0, 150);
	m_lstResult.InsertColumn(6, _T("滤光片2[%]"), 0, 150);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	for (int i=0; i<10; i++)
	{
		CString str;
		str.Format(_T("%.1f"), (i+1)*0.5);
		m_lstResult.InsertItem(i, str);

		str.Format(_T("%.1f"), (i+11)*0.5);
		m_lstResult.SetItemText(i, 4, str);
	}

	//读取配置
	CString strPathOfDataExchange;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// 向上2层
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// 数据交换目录路径
	strPathOfDataExchange = strPathOfDataExchange + L"\\Config";

	CSimpleIni iniDemarcation(strPathOfDataExchange + L"\\Demarcation.ini");
	m_edStdN1.SetWindowText(iniDemarcation.GetString(L"OpaDem", L"StdN1"));
	m_edStdN2.SetWindowText(iniDemarcation.GetString(L"OpaDem", L"StdN2"));
}

void CNHT1LCalibrationCheckDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"OpaParams", L"Port", L"2"));
	if (OPA_OPEN_SUCCEED == m_pOpaThread->Open(bPort, COpaThread::IM_NANHUA_NHT1L))
	{
		BYTE bStatusValue = 0;
		if (OPA_WR_SUCCEED == m_pOpaThread->GetStatus(&bStatusValue)) //0，预热，1，实时检测，2，校准，3控制单元与测量单元通讯错误
		{
			// 通讯成功
			switch (bStatusValue)
			{
			case 0x00:
				{
					// 正在预热
					m_lbInfo.SetText(L"正在预热...");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
					if (MessageBox(_T("是否退出预热?"), _T("不透光度计校准"), MB_YESNO)==IDYES)
					{
						m_pOpaThread->StopWarmUp();		//停止预热
						m_lbInfo.SetText(L"正在退出预热...");
						SetTimer(TIMER_GET_STATUS, 1000, NULL);
					}
				}
				break;
			case 0x01:
				{
					// 实时测试方式,正好不用切换测试方式
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

					// 就绪
					m_lbInfo.SetText(L"不透光度计校准检查");
				}
				break;
			case 0x03:	
				{
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

					// 通讯错误
					m_lbInfo.SetText(L"控制单元与测量单元通讯错误");
				}
				break;
			default:
				{
					// 通讯错误
					m_lbInfo.SetText(L"通讯错误");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
				}
				break;
			}
		}
		else
		{
			m_lbInfo.SetText(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	}
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHT1LCalibrationCheckDlg::OnTimer(UINT_PTR nIDEvent)
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
	case TIMER_GET_STATUS:
		{
			BYTE bStatus(0);
			if (OPA_WR_SUCCEED == m_pOpaThread->GetStatus(&bStatus))
			{
				if (1 == bStatus)
				{
					// 就绪
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					
					m_lbInfo.SetText(L"不透光度计校准检查");

					KillTimer(TIMER_GET_STATUS);
				}
			}
		}
		break;
	case TIMER_WAIT_FOR_CAL:
		{
			KillTimer(TIMER_WAIT_FOR_CAL);
			m_nTestStep = STEP_INPUT_N;
			m_lbInfo.SetText(_T("请输入2块滤光片示值后点下一步"));
			m_lbStep1.SetTextColor(RGB(0, 0, 0));
			m_lbStep1.SetText(L"√ 不透光度计校准");
			m_lbStep2.SetTextColor(RGB(0, 0, 255));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		}
		break;
	case TIMER_GET_REAL_TIME_VAL:
		{
			m_pOpaThread->GetRealTimeData_Asyn(NULL); 

			CString strN, strK;
			float fN(0.0f), fK(0.0f);
			fN = m_pOpaThread->GetRealTimeNValue();
			fK = m_pOpaThread->GetRealTimeKValue();
			strN.Format(_T("%.2f"), fN);
			strK.Format(_T("%.2f"), fK);
			m_lbN.SetText(strN);
			m_lbK.SetText(strK);
		}
		break;
	case TIMER_MEASURE_N1:
		{
			if (m_nTime < 20)
			{
				CString str;
				float f = m_pOpaThread->GetRealTimeNValue();
				m_nN1[m_nTime] = (int)(f * 100 + 0.5f);

				str.Format(_T("%.2f"), f);

				if (m_nTime < 10)
				{
					m_lstResult.SetItemText(m_nTime, 1, str);
				}
				else
				{
					m_lstResult.SetItemText(m_nTime-10, 5, str);
				}

				m_nTime++;
				str.Format(_T("正在获取测量数据....%d"), 10-m_nTime/2);
				m_lbInfo.SetText(str);
			}
			else
			{
				KillTimer(TIMER_MEASURE_N1);
				m_nTime = 0;
				m_nTestStep = STEP_MEASURE_N2;
				OnBnClickedButtonStart();
			}
		}
		break;
	case TIMER_MEASURE_N2:
		{
			if (m_nTime < 20)
			{
				CString str;
				float f = m_pOpaThread->GetRealTimeNValue();
				m_nN2[m_nTime] = (int)(f * 100 + 0.5f);

				str.Format(_T("%.2f"), f);

				if (m_nTime < 10)
				{
					m_lstResult.SetItemText(m_nTime, 2, str);
				}
				else
				{
					m_lstResult.SetItemText(m_nTime-10, 6, str);
				}

				m_nTime++;
				str.Format(_T("正在获取测量数据....%d"), 10-m_nTime/2);
				m_lbInfo.SetText(str);
			}
			else
			{
				KillTimer(TIMER_MEASURE_N2);
				m_nTime = 0;
				m_nTestStep = STEP_DONE;
				OnBnClickedButtonStart();
			}
		}
		break;
	default:
		break;
	}
	CDialogZoom::OnTimer(nIDEvent);
}

void CNHT1LCalibrationCheckDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_nTestStep)            
	{
	case STEP_PREPARE://步骤0:开始检测
		{
				ClearResult();

				m_lbStep1.SetTextColor(RGB(0, 0, 255));
				m_lbInfo.SetText(_T("正在校准..."));
				GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDCANCEL)->EnableWindow(FALSE); // 禁用退出按钮
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

				m_pOpaThread->Calibrate();	//发送校准指令
				SetTimer(TIMER_WAIT_FOR_CAL, 5000, NULL);	//等5s
		}
		break;
	case STEP_INPUT_N:
		{
			//开始实时取N、K的值
			SetTimer(TIMER_GET_REAL_TIME_VAL, 400, NULL);

			CString strStdN1, strStdN2;
			m_edStdN1.GetWindowText(strStdN1);
			m_edStdN2.GetWindowText(strStdN2);
			if (strStdN1.IsEmpty() || strStdN2.IsEmpty())
			{
				MessageBox(_T("请输入2块滤光片示值后点下一步"), _T("不透光度计校准"), MB_OK+MB_ICONINFORMATION);
				return;
			}
			
			float f = (float)_wtof(strStdN1);
			ASSERT(f > -10e-6f);
			
			m_nStdN1 = (int)(f * 100 + 0.5f);	// 放大100倍,并四舍五入
			if (m_nStdN1 == 0)
			{	
				MessageBox(_T("滤光片1示值不能为0"), _T("不透光度计校准"), MB_OK+MB_ICONINFORMATION);
				m_edStdN1.SetFocus();
				return;
			}

			f = (float)_wtof(strStdN2);
			ASSERT(f > -10e-6f);
			
			m_nStdN2 = (int)(f * 100 + 0.5f);	// 放大100倍,并四舍五入
			if (m_nStdN2 == 0)
			{			
				MessageBox(_T("滤光片2示值不能为0"), _T("不透光度计校准"), MB_OK+MB_ICONINFORMATION);
				m_edStdN2.SetFocus();
				return;
			}

			m_edStdN1.SetReadOnly(TRUE);
			m_edStdN2.SetReadOnly(TRUE);

			// 保存标准值
			SaveStdNVal();

			//下一步提示信息
			m_lbInfo.SetText(L"请插入滤光片1");
			m_lbStep2.SetTextColor(RGB(0, 0, 0));
			m_lbStep2.SetText(L"√ 填写标准滤光片数值");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			
			m_nTestStep = STEP_MEASURE_N1;
		}
		break;
	case STEP_MEASURE_N1:
		{
			if (MessageBox(_T("请插入滤光片1，点“确定”开始获取测量数据！"), _T("不透光度计校准"), MB_OKCANCEL) != IDOK)
			{
				return;
			}
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			m_nTime = 0;
			SetTimer(TIMER_MEASURE_N1, 500, NULL);                //获取10秒N值
			m_lbInfo.SetText(_T("正在获取测量数据..."));
		}
		break;
	case STEP_MEASURE_N2:
		{
			m_lbStep3.SetTextColor(RGB(0, 0, 0));
			m_lbStep3.SetText(L"√ 标准滤光片1检查");
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			m_lbInfo.SetText(L"请插入滤光片2");
			if (MessageBox(_T("请插入滤光片2，点“确定”开始获取测量数据！"), _T("不透光度计校准"), MB_OKCANCEL) != IDOK)
			{
				return;
			}
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			m_nTime = 0;
			SetTimer(TIMER_MEASURE_N2, 500, NULL);                //获取10秒N值
			m_lbInfo.SetText(_T("正在获取测量数据..."));
		}
		break;
	case STEP_DONE:
		{
			m_lbStep4.SetTextColor(RGB(0, 0, 0));
			m_lbStep4.SetText(L"√ 标准滤光片2检查");
			m_lbStep5.SetTextColor(RGB(0, 0, 255));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			MessageBox(_T("校准检查完成，请移走滤光片！"), _T("不透光度计校准"));
			
			// 计算结果
			CalcResult();

			m_lbStep5.SetTextColor(RGB(0, 0, 0));
			m_lbStep5.SetText(L"√ 计算结果");
			m_lbInfo.SetText(_T("校准检查完成"));

			m_nTestStep = STEP_PREPARE;
			KillTimer(TIMER_GET_REAL_TIME_VAL);

			m_bIsOperationFinishedAtLeaseOnce = true;

			// 导出检查细节
			ExportCalChkDetail();
			// 导出检查结果
			ExportCalChkResult();

			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		}
		break;
	default:
		break;
	}
}

void CNHT1LCalibrationCheckDlg::SaveStdNVal(void)
{	
	//写入配置
	CString strPathOfDataExchange;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// 向上2层
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// 数据交换目录路径
	strPathOfDataExchange = strPathOfDataExchange + L"\\Config";

	CSimpleIni iniDemarcation(strPathOfDataExchange + L"\\Demarcation.ini");
	
	CString strStdN1;
	strStdN1.Format(L"%.2f", m_nStdN1/100.0);
	iniDemarcation.SetString(L"OpaDem", L"StdN1", strStdN1);	//保存滤光片1标准检查值
	
	CString strStdN2;
	strStdN2.Format(L"%.2f", m_nStdN2/100.0);
	iniDemarcation.SetString(L"OpaDem", L"StdN2", strStdN2);	//保存滤光片2标准检查值
}

void CNHT1LCalibrationCheckDlg::CalcResult(void)
{
	for (int i=0; i<20; i++)
	{
		m_nN1[20] += m_nN1[i];
		m_nN2[20] += m_nN2[i];
	}
	m_nN1[20] /= 20;		//N1平均值
	m_nN2[20] /= 20;		//N2平均值

	for (int i=0; i<20; i++)
	{
		m_nN1[21] += (int)pow((float)(m_nN1[i]-m_nN1[20]), 2);           //平方
		m_nN2[21] += (int)pow((float)(m_nN2[i]-m_nN2[20]), 2);           //平方
	}
	m_nN1[21] = (int)sqrt((float)m_nN1[21]) / 20;             //开方再除以20,得到线性度值
	m_nN2[21] = (int)sqrt((float)m_nN2[21]) / 20;             //开方再除以20,得到线性度值
	
	CString strTemp;
	strTemp.Format(L"%.2f", m_nN1[20]/100.0);	//显示N1平均值
	m_lbAvg1.SetText(strTemp);

	strTemp.Format(L"%.2f", m_nN2[20]/100.0);	//显示N2平均值
	m_lbAvg2.SetText(strTemp);
	
	if (abs(m_nN1[21]) > m_nLinearLimit)				//显示N1线性度
	{
		m_lbLinear1.SetTextColor(RGB(255, 0, 0));
		m_bIsLinear1Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN1[21]/100.0);	
	m_lbLinear1.SetText(strTemp);

	if (abs(m_nN2[21]) > m_nLinearLimit)				//显示N2线性度
	{
		m_lbLinear2.SetTextColor(RGB(255, 0, 0));
		m_bIsLinear2Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN2[21]/100.0);	
	m_lbLinear2.SetText(strTemp);

	m_nN1[23] = m_nN1[20] - m_nStdN1;			//显示N1误差
	if (abs(m_nN1[23]) > m_nLinearLimit)
	{
		m_lbError1.SetTextColor(RGB(255, 0, 0));	
		m_bIsAE1Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN1[23]/100.0);
	m_lbError1.SetText(strTemp);

	m_nN2[23] = m_nN2[20] - m_nStdN2;			//显示N2误差
	if (abs(m_nN2[23]) > m_nLinearLimit)
	{
		m_lbError2.SetTextColor(RGB(255, 0, 0));	
		m_bIsAE2Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN2[23]/100.0);
	m_lbError2.SetText(strTemp);

	m_bIsN1Pass = m_bIsLinear1Pass && m_bIsAE1Pass;
	m_bIsN2Pass = m_bIsLinear2Pass && m_bIsAE2Pass;

	if (m_bIsN1Pass && m_bIsN2Pass)
	{		
		m_lbJudgement.SetTextColor(RGB(0, 0, 0));
		m_lbJudgement.SetText(_T("通过"));
	}
	else
	{
		m_lbJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbJudgement.SetText(_T("不通过"));
	}
}

void CNHT1LCalibrationCheckDlg::ClearResult()
{
	ZeroMemory(m_nN1, sizeof(int)*24);
	ZeroMemory(m_nN2, sizeof(int)*24);

	m_edStdN1.SetReadOnly(FALSE);
	m_edStdN2.SetReadOnly(FALSE);

	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep5.SetTextColor(RGB(0, 0, 0));

	m_lbStep1.SetText(L"   不透光度计校准");
	m_lbStep2.SetText(L"   填写标准滤光片数值");
	m_lbStep3.SetText(L"   标准滤光片1检查");	
	m_lbStep4.SetText(L"   标准滤光片2检查");	
	m_lbStep5.SetText(L"   计算结果");

	m_lbN.SetText(_T("--")); 
	m_lbK.SetText(_T("--")); 

	m_lbAvg1.SetText(_T("--"));                 
	m_lbAvg2.SetText(_T("--"));                

	m_lbLinear1.SetText(_T("--"));       
	m_lbLinear1.SetTextColor(RGB(0, 0, 0));

	m_lbLinear2.SetText(_T("--"));        
	m_lbLinear2.SetTextColor(RGB(0, 0, 0));

	m_lbError1.SetText(_T("--"));   
	m_lbError1.SetTextColor(RGB(0, 0, 0));

	m_lbError2.SetText(_T("--"));      
	m_lbError2.SetTextColor(RGB(0, 0, 0));
	
	m_lbJudgement.SetText(_T("--"));  
	m_lbJudgement.SetTextColor(RGB(0, 0, 0));

	for (int i =0; i<10; i++)
	{
		m_lstResult.SetItemText(i, 1, _T(""));
		m_lstResult.SetItemText(i, 2, _T(""));
		m_lstResult.SetItemText(i, 5, _T(""));
		m_lstResult.SetItemText(i, 6, _T(""));
	}

	m_bIsLinear1Pass = true;	
	m_bIsLinear2Pass = true;	
	m_bIsAE1Pass = true;		
	m_bIsAE2Pass = true;		
	m_bIsN1Pass = true;	
	m_bIsN2Pass = true;
	m_bIsCalChkPass = true;

	m_nTestStep = STEP_PREPARE;
	m_nTime = 0;
}

void CNHT1LCalibrationCheckDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor c;
	KillTimer(TIMER_GET_REAL_TIME_VAL);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始"));
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	m_lbInfo.SetText(_T("校准检查停止"));
	Sleep(400);
	ClearResult();
}

void CNHT1LCalibrationCheckDlg::ExportCalChkDetail(void)
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

	// 获取检测线编号和仪器型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strOpacimeterNumber = siLineInfo.GetString(L"LineInfo", L"OpacimeterNumber", L"");
	CString strOpacimeterModel = siLineInfo.GetString(L"LineInfo", L"OpacimeterModel", L"");

	// 获取操作员
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	// 获取当前时刻
	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString str;
	CString strDetail(L"[DetailOfOpaCalChk]");

	// 标定编号
	str.Format(L"\r\nDemarcationNumber=%s-%s-%s", strStationNumber, strLineNumber, odt.Format(L"%Y%m%d%H%M%S"));
	strDetail += str;

	// 检测线编号
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strDetail += str;

	// 操作员
	str.Format(L"\r\nOperator=%s", strOperator);
	strDetail += str;

	// 设备编号
	str.Format(L"\r\nEquipmentNumber=%s", strOpacimeterNumber);
	strDetail += str;

	// 设备型号
	str.Format(L"\r\nEquipmentModel=%s", strOpacimeterModel);
	strDetail += str;

	// 设备名称
	str.Format(L"\r\nEquipmentName=%s", L"不透光度计");
	strDetail += str;

	// 标定项目
	str.Format(L"\r\nDemarcationItem=%d", 7);
	strDetail += str;

	// 标定日期
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDetail += str;

	////////////////////////////////////////////////////////////////////////////////滤光片1细节begin
	// 滤光片1测量值平均值,单位:0.01%
	str.Format(L"\r\nMeasuredValue1=%.2f", m_nN1[20]/100.0);
	strDetail += str;

	// 标准滤光片1数值,单位:0.01%
	str.Format(L"\r\nStandardValue1=%.2f", m_nStdN1/100.0);
	strDetail += str;

	// 滤光片1绝对误差,单位:0.01%
	str.Format(L"\r\nAbsoluteError1=%.2f", m_nN1[23]/100.0);
	strDetail += str;
	
	// 滤光片1相对误差,单位:0.01%
	str.Format(L"\r\nRelativeError1=%.2f", m_nN1[23]/m_nStdN1*100.0);
	strDetail += str;
	
	// 滤光片1绝对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nAbsoluteErrorLimit1=%.2f", m_nAELimit/100.0);
	strDetail += str;

	// 滤光片1相对误差限值
	str.Format(L"\r\nRelativeErrorLimit1=");
	strDetail += str;

	// 滤光片1绝对误差是否合格
	str.Format(L"\r\nAbsoluteErrorPass1=%s", m_bIsAE1Pass ? L"1" : L"0");
	strDetail += str;

	// 滤光片1相对误差是否合格
	str.Format(L"\r\nRelativeErrorPass1=");
	strDetail += str;

	// 滤光片1测量值线性度
	str.Format(L"\r\nLinearity1=%.2f", m_nN1[21]/100.0);
	strDetail += str;

	// 滤光片1测量值线性度限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearityLimit1=%.2f", m_nLinearLimit/100.0);
	strDetail += str;

	// 滤光片1线性度是否合格
	str.Format(L"\r\nLinearityPass1=%s", m_bIsLinear1Pass ? L"1" : L"0");
	strDetail += str;

	// 滤光片1是否合格
	str.Format(L"\r\nPass1=%s", m_bIsN1Pass ? L"1" : L"0");
	strDetail += str;
	////////////////////////////////////////////////////////////////////////////////滤光片1细节end


	////////////////////////////////////////////////////////////////////////////////滤光片2细节begin
	// 滤光片2测量值平均值,单位:0.01%
	str.Format(L"\r\nMeasuredValue2=%.2f", m_nN2[20]/100.0);
	strDetail += str;

	// 标准滤光片2数值,单位:0.01%
	str.Format(L"\r\nStandardValue2=%.2f", m_nStdN2/100.0);
	strDetail += str;

	// 滤光片2绝对误差,单位:0.01%
	str.Format(L"\r\nAbsoluteError2=%.2f", m_nN2[23]/100.0);
	strDetail += str;
	
	// 滤光片2相对误差,单位:0.01%,四舍五入
	str.Format(L"\r\nRelativeError2=%.2f", m_nN2[23]/m_nStdN2*100.0);
	strDetail += str;
	
	// 滤光片2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nAbsoluteErrorLimit2=%.2f", m_nAELimit/100.0);
	strDetail += str;

	// 滤光片2相对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nRelativeErrorLimit2=");
	strDetail += str;

	// 滤光片2绝对误差是否合格
	str.Format(L"\r\nAbsoluteErrorPass2=%s", m_bIsAE2Pass ? L"1" : L"0");
	strDetail += str;

	// 滤光片2相对误差是否合格
	str.Format(L"\r\nRelativeErrorPass2=");
	strDetail += str;

	// 滤光片2测量值线性度,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearity2=%.2f", m_nN2[21]/100.0);
	strDetail += str;

	// 滤光片2测量值线性度限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearityLimit2=%.2f", m_nLinearLimit/100.0);
	strDetail += str;

	// 滤光片2线性度是否合格
	str.Format(L"\r\nLinearityPass2=%s", m_bIsLinear2Pass ? L"1" : L"0");
	strDetail += str;

	// 滤光片2是否合格
	str.Format(L"\r\nPass2=%s", m_bIsN2Pass ? L"1" : L"0");
	strDetail += str;
	////////////////////////////////////////////////////////////////////////////////滤光片2细节end

	CString strIsPass;
	m_lbJudgement.GetWindowText(strIsPass);
	// 校准检查是否通过
	str.Format(L"\r\nPass=%s", (m_bIsN1Pass&&m_bIsN2Pass) ? L"1" : L"0");
	strDetail += str;

	// 写入到结果ini文件
	CString strPathOfAnaGasCheckDetail = strPathOfDataExchange + L"\\DetailOfOpaCalChk.ini";
	if (0 == _taccess(strPathOfAnaGasCheckDetail, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfAnaGasCheckDetail);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfAnaGasCheckDetail, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfAnaGasCheckDetail, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strDetail);
	sfe.Close();
}

void CNHT1LCalibrationCheckDlg::ExportCalChkResult(void)
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

	// 获取检测线编号和不透光型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"OpacimeterModel", L"");

	// 获取操作员
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	CString str;
	CString strResult(L"[EquCalChkInfo]");

	// 操作是否完成过至少一次
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce?L"1":L"0");
	strResult += str;

	// 检测线编号
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strResult += str;

	// 操作员
	str.Format(L"\r\nOperator=%s", strOperator);
	strResult += str;

	// 设备
	str.Format(L"\r\nEquipment=%s", strEquipmentModel);
	strResult += str;

	// 项目
	str.Format(L"\r\nItem=%s", L"不透光度计校准检查");
	strResult += str;

	// 日期
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	strData.Format(L"滤光片1:测量值:%.2f%%,标准值:%.2f%%;滤光片2:测量值:%.2f%%,标准值:%.2f%%",
		m_nN1[20]/100.0, m_nStdN1/100.0, m_nN2[20]/100.0, m_nStdN2/100.0);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 误差
	CString strError;
	strError.Format(L"滤光片1:绝对误差:%.2f%%(±%.2f%%),线性度:%.2f%%(≤%.2f%%);滤光片2:绝对误差:%.2f%%(±%.2f%%),线性度:%.2f%%(≤%.2f%%)",
		 m_nN1[23]/100.0,
		(float)m_nAELimit/100,
		m_nN1[21]/100.0,
		(float)m_nLinearLimit/100,
		m_nN2[23]/100.0,
		(float)m_nAELimit/100,
		m_nN2[21]/100.0,
		(float)m_nLinearLimit/100);
	if (!m_bIsOperationFinishedAtLeaseOnce)
	{
		strError = L"滤光片1:绝对误差:0%(±1.5%),线性度:0%(≤1.1%);滤光片2:绝对误差:0%(±1.5%),线性度:0%(≤1.1%)";
	}
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 判断
	str.Format(L"\r\nJudgement=%s", (m_bIsN1Pass&&m_bIsN2Pass) ? L"通过" : L"不通过");
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