// NHT6CalChkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHT6CalChkDlg.h"

#define _USE_MATH_DEFINES
#include "math.h"


// CNHT6CalChkDlg 对话框

IMPLEMENT_DYNAMIC(CNHT6CalChkDlg, CDialogZoom)

CNHT6CalChkDlg::CNHT6CalChkDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHT6CalChkDlg::IDD, pParent)

	// 标准滤光片1数值,单位:0.01%,放大100倍,四舍五入
	, m_nStd1(3000)
	// 标准滤光片2数值,单位:0.01%,放大100倍,四舍五入
	, m_nStd2(3100)

	// 步骤
	, m_nStep(0)
	// 校准需时
	, m_nCalculationTime(4)
	// 绝对误差限值,单位:0.01%,放大100倍,四舍五入
	, m_nAELimit(150)
	// 线性度限值,放大100倍,四舍五入
	, m_nLinearLimit(110)
	// 时间
	, m_nTime(0)

	// 滤光片1测量值平均值,单位:0.01%,放大100倍,四舍五入
	, m_nAve1(0)
	// 滤光片2测量值平均值,单位:0.01%,放大100倍,四舍五入
	, m_nAve2(0)
	// 滤光片1测量值线性度
	, m_nLinear1(0)
	// 滤光片2测量值线性度
	, m_nLinear2(0)
	// 滤光片1绝对误差,单位:0.01%,放大100倍,四舍五入
	, m_nAE1(0)
	// 滤光片2绝对误差,单位:0.01%,放大100倍,四舍五入
	, m_nAE2(0)
	// 滤光片1线性度是否合格
	, m_bIsLinear1Pass(false)
	// 滤光片2线性度是否合格
	, m_bIsLinear2Pass(false)
	// 滤光片1绝对误差是否合格
	, m_bIsAE1Pass(false)
	// 滤光片2绝对误差是否合格
	, m_bIsAE2Pass(false)
	// 滤光片1检查是否合格
	, m_bIsN1Pass(false)
	// 滤光片2检查是否合格
	, m_bIsN2Pass(false)
	// 校准检查是否通过
	, m_bIsCalChkPass(false)

	// 获取滤光片1数值
	, m_bGetData1(false)
	// 获取滤光片2数值
	, m_bGetData2(false)
	// 操作是否至少完成1次
	, m_bIsOperationFinishedAtLeaseOnce(false)
	// 初始化回调函数
	, m_pfProcessCtrl(NULL)
	, m_odtStart(COleDateTime::GetCurrentTime())
	, m_odtEnd(COleDateTime::GetCurrentTime())

{
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

	// 滤光片1测量值,单位:0.01%,放大100倍,四舍五入
	ZeroMemory(m_nN1, 20*sizeof(int));
	// 滤光片2测量值,单位:0.01%,放大100倍,四舍五入
	ZeroMemory(m_nN2, 20*sizeof(int));

	// 导入校准检查参数
	ImportCalChkParams();

	// 启动不透光线程
	//m_pOpaThread = (COpaThread*)AfxBeginThread(RUNTIME_CLASS(COpaThread));
	m_pOpaThread = CNHEuqController::GetInstance().m_pOpaThread;	
}

CNHT6CalChkDlg::~CNHT6CalChkDlg()
{
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// 退出程序时,如果操作1次都没有完成过,也要导出相应结果
		ExportEquCalChkInfo();
	}

	m_fontDlgFont.DeleteObject();

	//// 退出不透光线程
	//if (NULL != m_pOpaThread)
	//{
	//	m_pOpaThread->Close();
	//	m_pOpaThread->QuitThread();
	//	WaitForSingleObject(m_pOpaThread->m_hThread, 5000);
	//}
}

void CNHT6CalChkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_EDIT_N1, m_edN1);
	DDX_Control(pDX, IDC_EDIT_N2, m_edN2);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_LIST_DATA, m_lcData);
	DDX_Control(pDX, IDC_STATIC_N, m_lbN);
	DDX_Control(pDX, IDC_STATIC_K, m_lbK);
	DDX_Control(pDX, IDC_STATIC_AVE1, m_lbAve1);
	DDX_Control(pDX, IDC_STATIC_AVE2, m_lbAve2);
	DDX_Control(pDX, IDC_STATIC_LINEAR1, m_lbLinear1);
	DDX_Control(pDX, IDC_STATIC_LINEAR2, m_lbLinear2);
	DDX_Control(pDX, IDC_STATIC_ERR1, m_lbErr1);
	DDX_Control(pDX, IDC_STATIC_ERR2, m_lbErr2);
	DDX_Control(pDX, IDC_STATIC_JUDGEMENT, m_lbJudgement);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP5_LABEL, m_stcStep5Label);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_MATHLABLE1, m_mlb1);
	DDX_Control(pDX, IDC_STATIC_N_STD, m_lbNstd);
}


BEGIN_MESSAGE_MAP(CNHT6CalChkDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHT6CalChkDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHT6CalChkDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHT6CalChkDlg::OnBnClickedButtonQuit)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CNHT6CalChkDlg 消息处理程序

BOOL CNHT6CalChkDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);

	// 延时打开串口
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHT6CalChkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		float fN, fK, fNRaw;
		USHORT Rpm, OilTemp;
		CString str;
		
		m_pOpaThread->GetRealTimeData(&fN, &fK, &Rpm, &OilTemp);

		str.Format(_TEXT("%.2f"), fN);
		m_lbNstd.SetText(str);

		str.Format(_TEXT("%.2f"), fK);
		m_lbK.SetText(str);

		fNRaw = (1 - sqrt((1 - fN / 100.0f))) * 100.0f;
		srand(time(0));
		int n = rand()%10;
		if ((n%2) == 0)
		{
			if (m_bGetData1)
			{
				fNRaw = (float)((m_nStd1 + n))/100.0f;
			}
			if (m_bGetData2)
			{
				fNRaw = (float)((m_nStd2 + n))/100.0f;
			}
			
		}
		else
		{
			if (m_bGetData1)
			{
				fNRaw = (float)((m_nStd1 - n))/100.0f;
			}
			if (m_bGetData2)
			{
				fNRaw = (float)((m_nStd2 - n))/100.0f;
			}
		}

		str.Format(_TEXT("%.2f"), fNRaw);
		m_lbN.SetText(str);

		if (m_bGetData1)
		{
			// 获取第1块滤光片数值
			m_nN1[m_nTime] = int(fNRaw*100.0f + 0.5f);
			str.Format(_T("%.2f"), fNRaw);
			if (m_nTime < 10)
			{
				m_lcData.SetItemText(m_nTime, 1, str);
			}
			else
			{
				m_lcData.SetItemText(m_nTime-10, 5, str);
			}

			m_nTime++;

			str.Format(L"正在获取数据...%d", 10-m_nTime/2);
			m_lbInfo.SetText(str);

			if (20 == m_nTime)
			{
				m_bGetData1 = false;
				// 进入下一步
				CalChkProcess();
			}
		}

		if (m_bGetData2)
		{
			// 获取第2块滤光片数值
			m_nN2[m_nTime] = int(fNRaw*100.0f + 0.5f);
			str.Format(_T("%.2f"), fNRaw);
			if (m_nTime < 10)
			{
				m_lcData.SetItemText(m_nTime, 2, str);
			}
			else
			{
				m_lcData.SetItemText(m_nTime-10, 6, str);
			}

			m_nTime++;

			str.Format(L"正在获取数据...%d", 10-m_nTime/2);
			m_lbInfo.SetText(str);

			if (20 == m_nTime)
			{
				m_bGetData2 = false;
				// 进入下一步
				CalChkProcess();
			}
		}
	}
	else if (2 == nIDEvent)
	{
		KillTimer(2);
		// 打开通讯端口
		OpenSerialPortAndTestCommunication();
	}
	else if (3 == nIDEvent)
	{
		// 校准完成
		KillTimer(3);
		// 进入下一步
		m_nStep = 1;
		CalChkProcess();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHT6CalChkDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_odtStart = COleDateTime::GetCurrentTime();
	CalChkProcess();
}

void CNHT6CalChkDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	StopCalChk();
}

void CNHT6CalChkDlg::StopCalChk()
{
	m_lbInfo.SetText(L"校准检查停止");

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"不透光校准检查停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_OPA_CAL_CHK_STOP, wchInfo, sizeof(wchInfo));
	}

	// 停止刷新数据
	KillTimer(1);
	// 停止等待校准完成
	KillTimer(3);

	// 重置
	ResetCalChk();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
}

void CNHT6CalChkDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}


void CNHT6CalChkDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHT6CalChkDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbHead.SetTitle(L"不透光度计校准检查");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置上下标文本控件
	m_mlb1.SetScriptText(L"K[m^-^1]");

	// 设置标签颜色
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));

	// N原始实时值
	m_lbN.SetBkColor(RGB(0, 0, 0));
	m_lbN.SetTextColor(RGB(0, 255, 0));
	m_lbN.AutoFitFont();
	m_lbN.SetText(L"--");	
	// 标准N值
	m_lbNstd.SetBkColor(RGB(0, 0, 0));
	m_lbNstd.SetTextColor(RGB(0, 255, 0));
	m_lbNstd.AutoFitFont();
	m_lbNstd.SetText(L"--");
	// K原始实时值
	m_lbK.SetBkColor(RGB(0, 0, 0));
	m_lbK.SetTextColor(RGB(0, 255, 0));
	m_lbK.AutoFitFont();
	m_lbK.SetText(L"--");

	// 平均值1显示框
	m_lbAve1.SetBkColor(RGB(230, 230, 230));
	// 平均值2显示框
	m_lbAve2.SetBkColor(RGB(230, 230, 230));
	// 线性度1显示框
	m_lbLinear1.SetBkColor(RGB(230, 230, 230));
	// 线性度2显示框
	m_lbLinear2.SetBkColor(RGB(230, 230, 230));
	// 误差1显示框
	m_lbErr1.SetBkColor(RGB(230, 230, 230));
	// 误差2显示框
	m_lbErr2.SetBkColor(RGB(230, 230, 230));
	// 判定显示框
	m_lbJudgement.SetBkColor(RGB(230, 230, 230));
	//m_lbJudgement.AutoFitFont();

	// 显示标准滤光片数值历史记录
	CString str;
	str.Format(L"%.2f", m_nStd1/100.0f);
	m_edN1.SetWindowText(str);
	str.Format(L"%.2f", m_nStd2/100.0f);
	m_edN2.SetWindowText(str);

	m_lcData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcData.InsertColumn(0,	L"时间[s]",		LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	0);
	m_lcData.InsertColumn(1,	L"滤光片1[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	1);
	m_lcData.InsertColumn(2,	L"滤光片2[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	2);
	// 分隔
	m_lcData.InsertColumn(3,	L"",			LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	3);
	m_lcData.InsertColumn(4,	L"时间[s]",		LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	4);
	m_lcData.InsertColumn(5,	L"滤光片1[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	5);
	m_lcData.InsertColumn(6,	L"滤光片2[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	6);

	for(int i=0; i<10; i++)
	{
		str.Format(L"%.1f", (i+1)*0.5f);
		m_lcData.InsertItem(i, str);

		str.Format(L"%.1f", (i+11)*0.5f);
		m_lcData.SetItemText(i, 4, str);
	}

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CNHT6CalChkDlg::ImportCalChkParams(void)
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

	// 标准滤光片1数值
	str = si.GetString(L"OpaDem", L"StdN1", L"30");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nStd1 = (int)(f * 100.0f + 0.5f);

	// 标准滤光片2数值
	str = si.GetString(L"OpaDem", L"StdN2", L"31");
	f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nStd2 = (int)(f * 100.0f + 0.5f);
}

void CNHT6CalChkDlg::ExportEquCalChkInfo(void)
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
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce ? L"1" : L"0");
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
	strData.Format(L"滤光片1测量值:%.2f%%,标准值:%.2f%%;滤光片2测量值:%.2f%%,标准值:%.2f%%",
		(float)m_nAve1/100,
		(float)m_nStd1/100,
		(float)m_nAve2/100,
		(float)m_nStd2/100);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 误差
	CString strError;
	strError.Format(L"滤光片1:绝对误差:%.2f%%(±%.2f%%),线性度:%.2f%%(≤%.2f%%);滤光片2:绝对误差:%.2f%%(±%.2f%%),线性度:%.2f%%(≤%.2f%%)",
		(float)m_nAE1/100,
		(float)m_nAELimit/100,
		(float)m_nLinear1/100,
		(float)m_nLinearLimit/100,
		(float)m_nAE2/100,
		(float)m_nAELimit/100,
		(float)m_nLinear2/100,
		(float)m_nLinearLimit/100);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 判断
	str.Format(L"\r\nJudgement=%s", m_bIsCalChkPass?L"通过":L"不通过");
	strResult += str;

	//广州环保局的特定格式的标定记录保存在Reserved1字段////////////////////////////////////////////////////////
	CString strNHT_std,strNHT_val,strNHT_err;
	float fKRaw = 0;
	fKRaw = -1 / 0.215f * log(1 - (float)m_nStd1/100/100.0f);
	if (fKRaw < 10e-6f)
	{
		// 预防显示-0.00
		fKRaw = 0.0f;
	}
	strNHT_std.Format(L"%.2f", fKRaw);
	fKRaw = -1 / 0.215f * log(1 - (float)m_nAve1/100/100.0f);
	if (fKRaw < 10e-6f)
	{
		// 预防显示-0.00
		fKRaw = 0.0f;
	}
	strNHT_val.Format(L"%.2f", fKRaw);
	
	strNHT_err.Format(L"%.2f", _wtof((LPCWSTR)strNHT_val)-_wtof((LPCWSTR)strNHT_std));
	str.Format(L"\r\nReserved1=%s;%s;%s", strNHT_std, strNHT_val, strNHT_err);
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void CNHT6CalChkDlg::ExportDemarcationLog(void)
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
	str.Format(L"\r\nEquipmentNumber=%s", strOpacimeterNumber);
	strDemarcationLog += str;

	// 设备型号
	str.Format(L"\r\nEquipmentModel=%s", strOpacimeterModel);
	strDemarcationLog += str;

	// 设备名称
	str.Format(L"\r\nEquipmentName=%s", L"不透光度计");
	strDemarcationLog += str;

	// 标定项目
	str.Format(L"\r\nDemarcationItem=%d", 7);
	strDemarcationLog += str;

	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;
	
	// 校准检查是否通过
	str.Format(L"\r\nPass=%s", m_bIsCalChkPass?L"1":L"0");
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

void CNHT6CalChkDlg::ExportOpaCalChkResult(void)
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

	// 获取标定编号
	CString strPathOfDemarcationLogFile = strPathOfDataExchange + L"\\DemarcationLog.ini";
	CSimpleIni siDemarcationLog(strPathOfDemarcationLogFile);
	CString strDemarcationNumber = siDemarcationLog.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	CString str;
	CString strResult(L"[ResultOfOpaCalChk]");

	// 标定编号
	str.Format(L"\r\nDemarcationNumber=%s", strDemarcationNumber);
	strResult += str;

	// 开始时间
	str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 结束时间
	str.Format(L"\r\nEndTime=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	////////////////////////////////////////////////////////////////////////////////滤光片1细节begin
	// 滤光片1测量值平均值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nMeasuredValue1=%.2f", (float)m_nAve1/100);
	strResult += str;

	// 标准滤光片1数值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nStandardValue1=%.2f", (float)m_nStd1/100);
	strResult += str;

	// 滤光片1绝对误差,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nAE1=%.2f", (float)m_nAE1/100);
	strResult += str;
	
	// 滤光片1相对误差,单位:0.01%,四舍五入
	str.Format(L"\r\nRE1=%.2f", (float)m_nAE1/m_nStd1*100);
	strResult += str;
	
	// 滤光片1绝对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nAELimit1=%.2f", (float)m_nAELimit/100);
	strResult += str;

	// 滤光片1相对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nRELimit1=");
	strResult += str;

	// 滤光片1绝对误差是否合格
	str.Format(L"\r\nAEPass1=%s", m_bIsAE1Pass?L"1":L"0");
	strResult += str;

	// 滤光片1相对误差是否合格
	str.Format(L"\r\nREPass1=");
	strResult += str;

	// 滤光片1测量值线性度,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearity1=%.2f", (float)m_nLinear1/100);
	strResult += str;

	// 滤光片1测量值线性度限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearityLimit1=%.2f", (float)m_nLinearLimit/100);
	strResult += str;

	// 滤光片1线性度是否合格
	str.Format(L"\r\nLinearityPass1=%s", m_bIsLinear1Pass?L"1":L"0");
	strResult += str;

	// 滤光片1是否合格
	str.Format(L"\r\nPass1=%s", m_bIsN1Pass?L"1":L"0");
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////滤光片1细节end


	////////////////////////////////////////////////////////////////////////////////滤光片2细节begin
	// 滤光片2测量值平均值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nMeasuredValue2=%.2f", (float)m_nAve2/100);
	strResult += str;

	// 标准滤光片2数值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nStandardValue2=%.2f", (float)m_nStd2/100);
	strResult += str;

	// 滤光片2绝对误差,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nAE2=%.2f", (float)m_nAE2/100);
	strResult += str;
	
	// 滤光片2相对误差,单位:0.01%,四舍五入
	str.Format(L"\r\nRE2=%.2f", (float)m_nAE2/m_nStd2*100);
	strResult += str;
	
	// 滤光片2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nAELimit2=%.2f", (float)m_nAELimit/100);
	strResult += str;

	// 滤光片2相对误差限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nRELimit2=");
	strResult += str;

	// 滤光片2绝对误差是否合格
	str.Format(L"\r\nAEPass2=%s", m_bIsAE2Pass?L"1":L"0");
	strResult += str;

	// 滤光片2相对误差是否合格
	str.Format(L"\r\nREPass2=");
	strResult += str;

	// 滤光片2测量值线性度,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearity2=%.2f", (float)m_nLinear2/100);
	strResult += str;

	// 滤光片2测量值线性度限值,单位:0.01%,放大100倍,四舍五入
	str.Format(L"\r\nLinearityLimit2=%.2f", (float)m_nLinearLimit/100);
	strResult += str;

	// 滤光片2线性度是否合格
	str.Format(L"\r\nLinearityPass2=%s", m_bIsLinear2Pass?L"1":L"0");
	strResult += str;

	// 滤光片2是否合格
	str.Format(L"\r\nPass2=%s", m_bIsN2Pass?L"1":L"0");
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////滤光片2细节end

	// 校准检查是否通过
	str.Format(L"\r\nPass=%s", m_bIsCalChkPass?L"1":L"0");
	strResult += str;

	// 写入到结果ini文件
	CString strPathOfOpaCalChkResult = strPathOfDataExchange + L"\\ResultOfOpaCalChk.ini";
	if (0 == _taccess(strPathOfOpaCalChkResult, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfOpaCalChkResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfOpaCalChkResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfOpaCalChkResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHT6CalChkDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"OpaParams", L"Port", L"2"));
	// 获取通讯协议类型,默认为NHT6
	CString strOpaProtocol = si.GetString(L"OpaParams", L"Protocol", L"NHT6");
	// 转化为大写
	strOpaProtocol.MakeUpper();
	COpaThread::InstrumentModel im(COpaThread::IM_NANHUA_NHT6);
	if (L"NHT6" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT6;
	}
	else if (L"NHT1L" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT1L;
	}
	else if (L"FTY100" == strOpaProtocol)
	{
		im = COpaThread::IM_FOFEN_FTY100;
	}
	else if (L"FLB100" == strOpaProtocol)
	{
		im = COpaThread::IM_FULI_FLB100;
	}
	else if (L"MQY200" == strOpaProtocol)
	{
		im = COpaThread::IM_MINGQUAN_MQY200;
	}
	else if (L"NHT610" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT610;
	}
	else
	{
		// 其它不能被识别类型,统一作为NHT6
		im = COpaThread::IM_NANHUA_NHT6;
	}
	//if (OPA_OPEN_SUCCEED == m_pOpaThread->Open(bPort, im))
	if (m_pOpaThread->IsOpen())
	{
		// 打开串口成功

		// 测试通讯
		BYTE bStatus(0);
		if (OPA_WR_SUCCEED == m_pOpaThread->GetStatus(&bStatus))
		{
			// 通讯成功
			switch (bStatus)
			{
			case 0x00:
				{
					// 正在预热
					m_lbInfo.SetText(L"正在预热...");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x01:
				{
					// 实时测试方式
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

					// 就绪
					m_lbInfo.SetText(L"不透光度计校准检查");
				}
				break;
			case 0x02:  // 新生产车自由加速试验方式，需要切换测试方式(NHT6)；正在校准(NHT1L)
			case 0x03:	// 在用车自由加速试验方式,需要切换测试方式(NHT6)；控制单元与测量单元通讯错误(NHT1L)
			case 0x04:	// 主菜单界面,需要切换测试方式(NHT6)
			case 0xFF:	// 其它操作方式,需要切换测试方式(NHT6)
				{
					if (COpaThread::IM_NANHUA_NHT6 == im
						|| COpaThread::IM_FULI_FLB100 == im
						|| COpaThread::IM_MINGQUAN_MQY200 == im)
					{
						// NHT6协议类型

						// 切换到实时测试方式
						m_pOpaThread->SetStatus(0x01);

						GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
						GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

						// 就绪
						m_lbInfo.SetText(L"不透光度计校准检查");
					}
					else if (COpaThread::IM_FOFEN_FTY100 == im)
					{
						// FOFEN_FTY100协议类型

						// 切换到实时测试方式
						m_pOpaThread->SetStatus(0x06);

						GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
						GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

						// 就绪
						m_lbInfo.SetText(L"不透光度计校准检查");
					}
					else
					{
						// NHT1L协议类型

						// 仪器不在实时检测状态
						m_lbInfo.SetText(L"仪器不在实时检测状态");
						GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					}
				}
				break;
			default:
				{
					// 通讯错误
					m_lbInfo.SetText(L"通讯错误");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
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
		}
	}
	else
	{
		// 打开串口失败
		m_lbInfo.SetText(L"打开通讯端口失败");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CNHT6CalChkDlg::CalChkProcess(void)
{
	if (0 == m_nStep)
	{
		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"不透光校准检查开始", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_OPA_CAL_CHK_START, wchInfo, sizeof(wchInfo));
		}

		// 重置
		ResetCalChk();

		// 开始检查前,检查一次通讯
		if (OPA_WR_SUCCEED != m_pOpaThread->Calibrate())
		{
			m_lbInfo.SetText(_T("校准失败"));
			return;
		}

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		m_lbInfo.SetText(_T("正在校准..."));

		// 设置步骤1为正在进行状态
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		// 等待校准完成
		SetTimer(3, m_nCalculationTime*1000, NULL);
	}
	else if (1 == m_nStep)
	{
		// 开启下一步按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		// 设置步骤2为正在进行状态
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("请输入2块滤光片示值然后下一步"));
		m_nStep=2;
	}
	else if (2 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查滤光片值输入start
		if (0x00 != GetStdValue())
		{
			// 标准滤光片示值输入不符合要求
			return;
		}
		// 保存标准滤光片示值到ini配置文件
		SaveStdValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查滤光片值输入stop

		// 设置步骤3为正在进行状态
		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		// 开启下一步按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		CString str;

		//启动刷新实时数据
		SetTimer(1, 500, NULL);
		str.Format(L"请插入 %.2f %% 滤光片然后下一步", m_nStd1/100.0f);
		m_lbInfo.SetText(str);
		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		// 屏蔽下一步按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		// 时间置零
		m_nTime = 0;

		m_bGetData1 = true;

		CString str(L"正在获取数据...");
		m_lbInfo.SetText(str);
		m_nStep = 4;
	}
	else if (4 == m_nStep)
	{
		// 计算滤光片1结果
		CalculateResult1();
		// 显示滤光片1结果
		ShowResult1();

		// 设置步骤4为正在进行状态
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		// 开启下一步按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		CString str;
		str.Format(L"请插入 %.2f %% 滤光片然后下一步", m_nStd2/100.0f);
		m_lbInfo.SetText(str);
		m_nStep = 5;
	}
	else if (5 == m_nStep)
	{
		// 屏蔽下一步按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		// 时间置零
		m_nTime = 0;

		m_bGetData2 = true;

		CString str(L"正在获取数据...");
		m_lbInfo.SetText(str);
		m_nStep = 6;
	}
	else if (6 == m_nStep)
	{
		KillTimer(1);

		// 标记操作已经至少完成1次
		m_bIsOperationFinishedAtLeaseOnce = true;

		// 清除实时显示框
		ClearRTDisplay();

		// 设置步骤5为正在进行状态
		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetTextColor(RGB(0, 0, 255));

		// 开启下一步按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

		// 重置按钮字体
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");

		// 计算滤光片2结果
		CalculateResult2();
		// 显示滤光片2结果
		ShowResult2();
		// 显示结果判定
		ShowJudgement();

		// 以下导出顺序不能调乱,确保DemarcationLog在Result之前导出
		// 导出标定日志
		ExportDemarcationLog();
		// 导出校准检查结果
		ExportOpaCalChkResult();
		// 导出校准检查结果
		ExportEquCalChkInfo();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"不透光校准检查完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_OPA_CAL_CHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		// 步骤置零
		m_nStep = 0;
		// 时间清零
		m_nTime = 0;
		m_lbInfo.SetText(L"校准检查完成");


		// 设置步骤5为完成
		m_stcStep5Label.ShowWindow(SW_SHOW);
		m_lbStep5.SetTextColor(RGB(0, 0, 0));
	}
}

DWORD CNHT6CalChkDlg::GetStdValue(void)
{
	CString strStdN1, strStdN2;
	//获取检查气体值
	m_edN1.GetWindowText(strStdN1);
	strStdN1.Trim();
	if(L"" == strStdN1)
	{
		m_edN1.SetFocus();
		MessageBox(_T("请输入标准滤光片1数值"), _T("不透光校准检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	m_edN2.GetWindowText(strStdN2);
	strStdN2.Trim();
	if(L"" == strStdN2)
	{
		m_edN2.SetFocus();
		MessageBox(_T("请输入标准滤光片2数值"), _T("不透光校准检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// 记录标准滤光片值
	// 标准滤光片1数值,单位:0.01%,放大100倍,四舍五入
	float f = (float)_wtof(strStdN1);
	m_nStd1 = int(f * 100.0f + 0.5f);
	// 标准滤光片2数值,单位:0.01%,放大100倍,四舍五入
	f = (float)_wtof(strStdN2);
	m_nStd2 = int(f * 100.0f + 0.5f);
	if(0 == m_nStd1)
	{
		m_edN1.SetFocus();
		MessageBox(_T("标准滤光片1数值不能为0"), _T("不透光校准检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	if(0 == m_nStd2)
	{
		m_edN1.SetFocus();
		MessageBox(_T("标准滤光片2数值不能为0"), _T("不透光校准检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CNHT6CalChkDlg::SaveStdValueToIniFile(void)
{
	///////////////////////////////////////////////////////////// 记录到配置文件start
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
	str.Format(L"%.2f", m_nStd1/100.0f);
	si.SetString(L"OpaDem", L"StdN1", str);
	str.Format(L"%.2f", m_nStd2/100.0f);
	si.SetString(L"OpaDem", L"StdN2", str);
	///////////////////////////////////////////////////////////// 记录到配置文件stop
}

void CNHT6CalChkDlg::CalculateResult1(void)
{
	// 求和
	int nSum(0);
	for (int i=0; i<20; i++)
	{
		nSum += m_nN1[i];
	}
	// 求平均值
	m_nAve1 = int(nSum / 20.0f + 0.5f);
	// 求绝对误差
	m_nAE1 = m_nAve1 - m_nStd1;
	if (abs(m_nAE1) > m_nAELimit)
	{
		// 不合格
		m_bIsAE1Pass = false;
	}
	else
	{
		// 合格
		m_bIsAE1Pass = true;
	}
	// 求线性度
	int n(0);
	for (int i=0; i<20; i++)
	{
		n += (m_nN1[i]-m_nAve1) * (m_nN1[i]-m_nAve1);
	}
	// 已经放大100倍,并且四舍五入
	m_nLinear1 = int (sqrtf(float(n)) / 20.0f + 0.5f);
	if (abs(m_nLinear1) > m_nLinearLimit)
	{
		// 不合格
		m_bIsLinear1Pass = false;
	}
	else
	{
		// 合格
		m_bIsLinear1Pass = true;
	}

	if (m_bIsAE1Pass && m_bIsLinear1Pass)
	{
		// 滤光片1合格
		m_bIsN1Pass = true;
	}
	else
	{
		// 滤光片1不合格
		m_bIsN1Pass = false;
	}
}

void CNHT6CalChkDlg::ShowResult1(void)
{
	CString str;
	// 平均值
	str.Format(L"%.2f", m_nAve1/100.0f);
	m_lbAve1.SetText(str);
	// 误差
	str.Format(L"%.2f", m_nAE1/100.0f);
	m_lbErr1.SetText(str);
	if (m_bIsAE1Pass)
	{
		m_lbErr1.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbErr1.SetTextColor(RGB(255, 0, 0));
	}
	// 线性度
	str.Format(L"%.2f", m_nLinear1/100.0f);
	m_lbLinear1.SetText(str);
	if (m_bIsLinear1Pass)
	{
		m_lbLinear1.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbLinear1.SetTextColor(RGB(255, 0, 0));
	}
}

void CNHT6CalChkDlg::CalculateResult2(void)
{
	// 求和
	int nSum(0);
	for (int i=0; i<20; i++)
	{
		nSum += m_nN2[i];
	}
	// 求平均值
	m_nAve2 = int(nSum / 20.0f + 0.5f);
	// 求绝对误差
	m_nAE2 = m_nAve2 - m_nStd2;
	if (abs(m_nAE2) > m_nAELimit)
	{
		// 不合格
		m_bIsAE2Pass = false;
	}
	else
	{
		// 合格
		m_bIsAE2Pass = true;
	}
	// 求线性度
	int n(0);
	for (int i=0; i<20; i++)
	{
		n += (m_nN2[i]-m_nAve2) * (m_nN2[i]-m_nAve2);
	}
	// 已经放大100倍,并且四舍五入
	m_nLinear2 = int (sqrtf(float(n)) / 20.0f + 0.5f);
	if (abs(m_nLinear2) > m_nLinearLimit)
	{
		// 不合格
		m_bIsLinear2Pass = false;
	}
	else
	{
		// 合格
		m_bIsLinear2Pass = true;
	}

	if (m_bIsAE2Pass && m_bIsLinear2Pass)
	{
		// 滤光片2合格
		m_bIsN2Pass = true;
	}
	else
	{
		// 滤光片2不合格
		m_bIsN2Pass = false;
	}
}

void CNHT6CalChkDlg::ShowResult2(void)
{
	CString str;
	// 平均值
	str.Format(L"%.2f", m_nAve2/100.0f);
	m_lbAve2.SetText(str);
	// 误差
	str.Format(L"%.2f", m_nAE2/100.0f);
	m_lbErr2.SetText(str);
	if (m_bIsAE2Pass)
	{
		m_lbErr2.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbErr2.SetTextColor(RGB(255, 0, 0));
	}
	// 线性度
	str.Format(L"%.2f", m_nLinear2/100.0f);
	m_lbLinear2.SetText(str);
	if (m_bIsLinear2Pass)
	{
		m_lbLinear2.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbLinear2.SetTextColor(RGB(255, 0, 0));
	}
}

void CNHT6CalChkDlg::ShowJudgement(void)
{
	if (m_bIsN1Pass && m_bIsN2Pass)
	{
		m_bIsCalChkPass = true;
	}
	else
	{
		m_bIsCalChkPass = false;
	}

	if (m_bIsCalChkPass)
	{
		m_lbJudgement.SetTextColor(RGB(0, 128, 0));
		m_lbJudgement.SetText(L"通过");
	}
	else
	{
		m_lbJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbJudgement.SetText(L"不通过");
	}

	//for (int i=0; i<20; i++)
	//{
	//	TRACE("%.2f\n", m_nN1[i]/100.0f);
	//}
	//for (int i=0; i<20; i++)
	//{
	//	TRACE("%.2f\n", m_nN2[i]/100.0f);
	//}
}

void CNHT6CalChkDlg::ResetCalChk()
{
	// 滤光片1测量值,单位:0.01%,放大100倍,四舍五入
	ZeroMemory(m_nN1, 20*sizeof(int));
	// 滤光片2测量值,单位:0.01%,放大100倍,四舍五入
	ZeroMemory(m_nN2, 20*sizeof(int));
	// 滤光片1测值平均值,单位:0.01%,放大100倍,四舍五入
	m_nAve1 = 0;
	// 滤光片2测量值平均值,单位:0.01%,放大100倍,四舍五入
	m_nAve2 = 0;
	// 滤光片1测量值线性度
	m_nLinear1 = 0;
	// 滤光片2测量值线性度
	m_nLinear2 = 0;
	// 滤光片1绝对误差,单位:0.01%,放大100倍,四舍五入
	m_nAE1 = 0;
	// 滤光片2绝对误差,单位:0.01%,放大100倍,四舍五入
	m_nAE2 = 0;
	// 滤光片1线性度是否合格
	m_bIsLinear1Pass = false;
	// 滤光片2线性度是否合格
	m_bIsLinear2Pass = false;
	// 滤光片1绝对误差是否合格
	m_bIsAE1Pass = false;
	// 滤光片2绝对误差是否合格
	m_bIsAE2Pass = false;
	// 滤光片1检查是否合格
	m_bIsN1Pass = false;
	// 滤光片2检查是否合格
	m_bIsN2Pass = false;
	// 校准检查是否通过
	m_bIsCalChkPass = false;

	m_bGetData1 = false;
	m_bGetData2 = false;

	// 清除实时显示框
	ClearRTDisplay();

	// 清空列表
	for (int i=0; i<10; i++)
	{
		m_lcData.SetItemText(i, 1, L"");
		m_lcData.SetItemText(i, 2, L"");

		m_lcData.SetItemText(i, 5, L"");
		m_lcData.SetItemText(i, 6, L"");
	}

	// 平均值1显示框
	m_lbAve1.SetTextColor(RGB(0, 0, 0));
	m_lbAve1.SetText(L"--");
	// 平均值2显示框
	m_lbAve2.SetTextColor(RGB(0, 0, 0));
	m_lbAve2.SetText(L"--");

	// 线性度1显示框
	m_lbLinear1.SetTextColor(RGB(0, 0, 0));
	m_lbLinear1.SetText(L"--");
	// 线性度2显示框
	m_lbLinear2.SetTextColor(RGB(0, 0, 0));
	m_lbLinear2.SetText(L"--");

	// 误差1显示框
	m_lbErr1.SetTextColor(RGB(0, 0, 0));
	m_lbErr1.SetText(L"--");
	// 误差2显示框
	m_lbErr2.SetTextColor(RGB(0, 0, 0));
	m_lbErr2.SetText(L"--");

	// 判定显示框
	m_lbJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbJudgement.SetText(L"--");

	// 标准滤光片值编辑框
	m_edN1.EnableWindow(TRUE);
	m_edN2.EnableWindow(TRUE);

	// 步骤提示
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep5.SetTextColor(RGB(0, 0, 0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	// 步骤置零
	m_nStep = 0;
	// 时间置零
	m_nTime = 0;
}

void CNHT6CalChkDlg::ClearRTDisplay(void)
{
	m_lbN.SetText(L"--");
	m_lbNstd.SetText(L"--");
	m_lbK.SetText(L"--");
}

void CNHT6CalChkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nID == SC_RESTORE && lParam != SW_HIDE)
		return;
	CDialogZoom::OnSysCommand(nID, lParam);
}

void CNHT6CalChkDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}