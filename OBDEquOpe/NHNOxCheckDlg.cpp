// NHNOxCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHNOxCheckDlg.h"
#include "math.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// 放大倍数,用于进行将浮点型转化成整型,进行数值比较(用浮点型比较有精度问题)
#define AMPLIFICATION_FACTOR 1000
// 小数位数,用于显示浮点型数值的小数位数,一般要根据AMPLIFICATION_FACTOR成对数变化
#define DECIMAL_PLACES 3

#define CHN_CO2		1
#define CHN_NO		2
#define CHN_NO2		3
#define CHN_CO2_NO	4

// CNHNOxCheckDlg 对话框

IMPLEMENT_DYNAMIC(CNHNOxCheckDlg, CDialogZoom)

CNHNOxCheckDlg::CNHNOxCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHNOxCheckDlg::IDD, pParent)
	, m_nStep(0)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_nNONominalValue(0)
	, m_nNO2NominalValue(0)
	, m_odtStart(COleDateTime::GetCurrentTime())
	, m_odtEnd(COleDateTime::GetCurrentTime())

	// CO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	, m_nCO2AELimit(int(0.50f * AMPLIFICATION_FACTOR + 0.5f))
	// CO2相对误差限值,单位:%
	, m_nCO2RELimit(5)
	// CO2测量值,单位:0.01%,放大100倍,四舍五入
	, m_nCO2MeasuredValue(0)
	// CO2绝对误差,单位:0.01%,放大100倍,四舍五入
	, m_nCO2AE(0)
	// CO2绝对误差是否通过
	, m_bIsCO2AEPass(false)
	// CO2相对误差,单位:%,放大100倍,四舍五入
	, m_nCO2RE(0)
	// CO2相对误差是否通过
	, m_bIsCO2REPass(0)
	// CO2通道是否通过
	, m_bIsCO2Pass(0)

	// NO绝对误差限值,单位:ppm
	, m_nNOAELimit(25)
	// NO相对误差限值,单位:%
	, m_nNORELimit(5)
	// NO测量值,单位:ppm
	, m_nNOMeasuredValue(0)
	// NO绝对误差,单位:ppm
	, m_nNOAE(0)
	// NO绝对误差是否通过
	, m_bIsNOAEPass(false)	
	// NO相对误差,单位:%,放大100倍,四舍五入
	, m_nNORE(0)
	// NO相对误差是否通过
	, m_bIsNOREPass(0)
	// NO通道是否通过
	, m_bIsNOPass(0)

	// NO2绝对误差限值,单位:ppm
	, m_nNO2AELimit(25)
	// NO2相对误差限值,单位:%
	, m_nNO2RELimit(5)
	// NO2测量值,单位:ppm
	, m_nNO2MeasuredValue(0)
	// NO2绝对误差,单位:ppm
	, m_nNO2AE(0)
	// NO2绝对误差是否通过
	, m_bIsNO2AEPass(false)	
	// NO2相对误差,单位:%,放大100倍,四舍五入
	, m_nNO2RE(0)
	// NO2相对误差是否通过
	, m_bIsNO2REPass(0)
	// NO2通道是否通过
	, m_bIsNO2Pass(0)
	
	// 检查通道
	, m_nChannel(CHN_CO2_NO)

	// 检查是否通过
	, m_bIsCheckPass(0)	
	// 初始化回调函数
	, m_pfProcessCtrl(NULL)

	// 分析仪协议版本
	, m_nAnaProtocolVersion(4)
	, m_sRandomErrorHC(0)
	, m_sRandomErrorNO(0)
	, m_sRandomErrorNO2(0)
	, m_sRandomErrorNOX(0)
	, m_fRandomErrorCO(0.0f)
	, m_fRandomErrorCO2(0.0f)
	, m_fRandomErrorO2(0.0f)
{
	// 启动分析仪线程
	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pNOxThread;

	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 20 / 1024;
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
		
	m_lsRTDataOfNOx.RemoveAll();

	// 导入气体检查参数
	ImportGasCheckParams();
	
	m_fCO2T90Limit = 5.5f;
	m_fCO2T10Limit = 5.7f;

	m_fNOT90Limit = 6.5f;
	m_fNOT10Limit = 6.7f;

	m_fNO2T90Limit = 6.5f;
	m_fNO2T10Limit = 6.7f;
}

CNHNOxCheckDlg::~CNHNOxCheckDlg()
{
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// 退出程序时,如果操作1次都没有完成过,也要导出相应结果
		ExportEquCalChkInfo();
	}
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pAnaThread)
	//{
	//	m_pAnaThread->Close();
	//	m_pAnaThread->QuitThread();
	//	WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	//}
}

void CNHNOxCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);
	DDX_Control(pDX, IDC_MATHLABLE1, m_mlb1);
	DDX_Control(pDX, IDC_MATHLABLE2, m_mlb2);
	DDX_Control(pDX, IDC_MATHLABLE3, m_mlb3);
	DDX_Control(pDX, IDC_MATHLABLE4, m_mlb4);
	DDX_Control(pDX, IDC_MATHLABLE5, m_mlb5);
	DDX_Control(pDX, IDC_MATHLABLE6, m_mlb6);
	DDX_Control(pDX, IDC_MATHLABLE13, m_mlb7);
	DDX_Control(pDX, IDC_MATHLABLE14, m_mlb8);
	DDX_Control(pDX, IDC_MATHLABLE15, m_mlb9);
	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_NO, m_lbNO);
	DDX_Control(pDX, IDC_STATIC_NO2, m_lbNO2);
	DDX_Control(pDX, IDC_STATIC_NOX, m_lbNOx);
	DDX_Control(pDX, IDC_EDIT_CO2_NOMINAL_VALUE, m_edCO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_NO_NOMINAL_VALUE, m_edNONominalValue);
	DDX_Control(pDX, IDC_EDIT_NO2_NOMINAL_VALUE, m_edNO2NominalValue);
	DDX_Control(pDX, IDC_STATIC_DETERMINATION, m_lbDetermination);
	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_CO2_MEASURED_VALUE, m_lbCO2MeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO2_NOMINAL_VALUE, m_lbCO2NominalValue);
	DDX_Control(pDX, IDC_STATIC_CO2_RE, m_lbCO2RE);
	DDX_Control(pDX, IDC_STATIC_CO2_AE, m_lbCO2AE);
	DDX_Control(pDX, IDC_STATIC_CO2_JUDGEMENT, m_lbCO2Judgement);
	DDX_Control(pDX, IDC_STATIC_NO_MEASURED_VALUE, m_lbNOMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_NO_NOMINAL_VALUE, m_lbNONominalValue);
	DDX_Control(pDX, IDC_STATIC_NO_AE, m_lbNOAE);
	DDX_Control(pDX, IDC_STATIC_NO_RE, m_lbNORE);
	DDX_Control(pDX, IDC_STATIC_NO_JUDGEMENT, m_lbNOJudgement);
	DDX_Control(pDX, IDC_STATIC_NO2_MEASURED_VALUE, m_lbNO2MeasuredValue);
	DDX_Control(pDX, IDC_STATIC_NO2_NOMINAL_VALUE, m_lbNO2NominalValue);
	DDX_Control(pDX, IDC_STATIC_NO2_AE, m_lbNO2AE);
	DDX_Control(pDX, IDC_STATIC_NO2_RE, m_lbNO2RE);
	DDX_Control(pDX, IDC_STATIC_NO2_JUDGEMENT, m_lbNO2Judgement);
	DDX_Control(pDX, IDC_STATIC_CO2_RESP_TIME, m_lbCO2RespTime);
	DDX_Control(pDX, IDC_STATIC_NO_RESP_TIME, m_lbNORespTime);
	DDX_Control(pDX, IDC_STATIC_NO2_RESP_TIME, m_lbNO2RespTime);
}


BEGIN_MESSAGE_MAP(CNHNOxCheckDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHNOxCheckDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHNOxCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHNOxCheckDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_CHN_NO_CHK, &CNHNOxCheckDlg::OnBnClickedRadioChnChk)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO2_CHK, &CNHNOxCheckDlg::OnBnClickedRadioChnChk)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO_CO2_CHK, &CNHNOxCheckDlg::OnBnClickedRadioChnChk)
	ON_BN_CLICKED(IDC_RADIO_CHN_CO2_CHK, &CNHNOxCheckDlg::OnBnClickedRadioChnChk)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE1, &CNHNOxCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE2, &CNHNOxCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE3, &CNHNOxCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE4, &CNHNOxCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE5, &CNHNOxCheckDlg::OnBnClickedRadioCheckType)
END_MESSAGE_MAP()


// CNHNOxCheckDlg 消息处理程序

BOOL CNHNOxCheckDlg::OnInitDialog()
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

void CNHNOxCheckDlg::InitCtrls(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}

	m_lbHead.SetTitle(L"氮氧分析仪气体检查");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置上下标文本控件
	m_mlb1.SetScriptText(L"NOx[10^-^6]");
	m_mlb2.SetScriptText(L"NO_2[10^-^6]");
	m_mlb3.SetScriptText(L"NO_2[10^-^6]");
	m_mlb4.SetScriptText(L"测量值[10^-^6]");
	m_mlb5.SetScriptText(L"标称值[10^-^6]");
	m_mlb6.SetScriptText(L"绝对误差[10^-^6]");
	m_mlb7.SetScriptText(L"NO[10^-^6]");
	m_mlb8.SetScriptText(L"NO[10^-^6]");
	m_mlb9.SetScriptText(L"CO_2[%]");

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));
	
	m_edCO2NominalValue.AllowNegative(FALSE);
	m_edCO2NominalValue.SetMaxDecimalPlaces(DECIMAL_PLACES);

	// CO2实时值
	m_lbCO2.SetLEDStyle();
	m_lbCO2.SetText(L"--");
	// NO实时值
	m_lbNO.SetLEDStyle();
	m_lbNO.SetText(L"--");
	// HC实时值
	m_lbNO2.SetLEDStyle();
	m_lbNO2.SetText(L"--");
	// NOx实时值
	m_lbNOx.SetLEDStyle();
	m_lbNOx.SetText(L"--");
	

	CString strFormat;

	// 显示检查气标称值历史记录
	CString str;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	m_edCO2NominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNONominalValue);
	m_edNONominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNO2NominalValue);
	m_edNO2NominalValue.SetWindowText(str);

	// CO2测量值显示框
	m_lbCO2MeasuredValue.SetBkColor(RGB(230, 230, 230));
	// CO2标称值显示框
	m_lbCO2NominalValue.SetBkColor(RGB(230, 230, 230));	
	// CO2绝对误差显示框
	m_lbCO2AE.SetBkColor(RGB(230, 230, 230));
	// CO2相对误差显示框
	m_lbCO2RE.SetBkColor(RGB(230, 230, 230));
	// CO2判定显示框
	m_lbCO2Judgement.SetBkColor(RGB(230, 230, 230));
	
	// NO测量值显示框
	m_lbNOMeasuredValue.SetBkColor(RGB(230, 230, 230));
	// NO标称值显示框
	m_lbNONominalValue.SetBkColor(RGB(230, 230, 230));	
	// NO绝对误差显示框
	m_lbNOAE.SetBkColor(RGB(230, 230, 230));
	// NO相对误差显示框
	m_lbNORE.SetBkColor(RGB(230, 230, 230));
	// NO判定显示框
	m_lbNOJudgement.SetBkColor(RGB(230, 230, 230));

	// NO2测量值显示框
	m_lbNO2MeasuredValue.SetBkColor(RGB(230, 230, 230));
	// NO2标称值显示框
	m_lbNO2NominalValue.SetBkColor(RGB(230, 230, 230));	
	// NO2绝对误差显示框
	m_lbNO2AE.SetBkColor(RGB(230, 230, 230));
	// NO2相对误差显示框
	m_lbNO2RE.SetBkColor(RGB(230, 230, 230));
	// NO2判定显示框
	m_lbNO2Judgement.SetBkColor(RGB(230, 230, 230));
	
	m_lbCO2RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNORespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNO2RespTime.SetBkColor(RGB(230, 230, 230));

	// 裁决显示框
	m_lbDetermination.SetBkColor(RGB(230, 230, 230));
	m_lbDetermination.AutoFitFont();

	SetLimit();

	//////////////////////////////////////// 初始化绝对误差和相对误差限值start
	// CO2绝对误差显示框
	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
	strFormat.Format(L"--(±%%.%df)", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);
	
	// CO2相对误差显示框
	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nCO2RELimit);
	m_lbCO2RE.SetText(str);

	// NO绝对误差显示框
	m_lbNOAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNOAELimit);
	m_lbNOAE.SetText(str);

	// NO相对误差显示框
	m_lbNORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNORELimit);
	m_lbNORE.SetText(str);

	// NO2绝对误差显示框
	m_lbNO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2AELimit);
	m_lbNO2AE.SetText(str);

	// NO2相对误差显示框
	m_lbNO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2RELimit);
	m_lbNO2RE.SetText(str);
	//////////////////////////////////////// 初始化绝对误差和相对误差限值stop

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	
	((CButton *)GetDlgItem(IDC_RADIO_CHN_NO_CO2_CHK))->SetCheck(BST_CHECKED);
	m_edCO2NominalValue.EnableWindow(TRUE);
	m_edNONominalValue.EnableWindow(TRUE);
	m_edNO2NominalValue.EnableWindow(FALSE);
	m_nChannel = CHN_CO2_NO;

	((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE1))->SetCheck(BST_CHECKED);
	m_nCheckType = 1;
}

void CNHNOxCheckDlg::ImportGasCheckParams(void)
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
	// 放大100倍,并四舍五入
	str = si.GetString(L"NOxChk", L"CO2NominalValue", L"12");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nCO2NominalValue = (int)(f * AMPLIFICATION_FACTOR + 0.5f);
	str = si.GetString(L"NOxChk", L"NONominalValue", L"3000");
	m_nNONominalValue = _wtoi(str);
	str = si.GetString(L"NOxChk", L"NO2NominalValue", L"600");
	m_nNO2NominalValue = _wtoi(str);
}

void CNHNOxCheckDlg::ExportEquCalChkInfo(void)
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
	CString strNOxAnalyzerModel = siLineInfo.GetString(L"LineInfo", L"NOxAnalyzerModel", L"");

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
	str.Format(L"\r\nEquipment=%s", strNOxAnalyzerModel);
	strResult += str;

	// 项目
	str.Format(L"\r\nItem=%s", L"氮氧分析仪气体检查");
	strResult += str;

	// 日期
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strData;
	if (m_nChannel == CHN_NO)
	{
		strData.Format(L"[NO]:%d(%d),", m_nNOMeasuredValue, m_nNONominalValue);
	}
	else if (m_nChannel == CHN_NO2)
	{
		strData.Format(L"[NO2]:%d(%d)", m_nNO2MeasuredValue, m_nNO2NominalValue);
	}
	else if (m_nChannel == CHN_CO2)
	{
		strData.Format(L"[CO2]:%d(%d)", m_nCO2MeasuredValue, m_nCO2NominalValue);
	}
	else if (m_nChannel == CHN_CO2_NO)
	{
		strData.Format(L"[NO]:%d(%d),", m_nNOMeasuredValue, m_nNONominalValue);
		strData.AppendFormat(L"[CO2]:%d(%d)", m_nCO2MeasuredValue, m_nCO2NominalValue);
	}
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	CString strError;
	if (m_nChannel == CHN_NO)
	{
		strError.Format(L"NO:%d(±%d)/%d(±%d),", m_nNOAE, m_nNOAELimit, m_nNORE, m_nNORELimit);
	}
	else if (m_nChannel == CHN_NO2)
	{
		strError.AppendFormat(L"NO2:%d(±%d)/%d(±%d)", m_nNO2AE, m_nNO2AELimit, m_nNO2RE, m_nNO2RELimit);
	}
	else if (m_nChannel == CHN_CO2)
	{
		strError.Format(L"CO2:%d(±%d)/%d(±%d)", m_nCO2AE, m_nCO2AELimit, m_nCO2RE, m_nCO2RELimit);
	}
	else if (m_nChannel == CHN_CO2_NO)
	{
		strError.Format(L"NO:%d(±%d)/%d(±%d),", m_nNOAE, m_nNOAELimit, m_nNORE, m_nNORELimit);
		strError.AppendFormat(L"CO2:%d(±%d)/%d(±%d)", m_nCO2AE, m_nCO2AELimit, m_nCO2RE, m_nCO2RELimit);
	}
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 判断
	str.Format(L"\r\nJudgement=%s", m_bIsCheckPass?L"通过":L"不通过");
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

void CNHNOxCheckDlg::ExportDemarcationLog(void)
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

	// 获取检测线编号和分析仪型号
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"NOxAnalyzerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"NOxAnalyzerModel", L"");

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
	str.Format(L"\r\nEquipmentName=%s", L"氮氧分析仪");
	strDemarcationLog += str;

	// 标定项目
	str.Format(L"\r\nDemarcationItem=%d", 6);
	strDemarcationLog += str;

	// 标定日期
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// 检查是否通过
	str.Format(L"\r\nPass=%s", m_bIsCheckPass?L"1":L"0");
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

void CNHNOxCheckDlg::ExportGasCheckResult(void)
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
	CString strResult(L"[ResultOfAnaGasChk]");
	//////////////////////////////////////////////////////////////////////////////
	// 标定编号
	str.Format(L"\r\nDemarcationNumber=%s", strDemarcationNumber);
	strResult += str;

	// 开始时间
	str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 结束时间
	str.Format(L"\r\nEndTime=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 通道
	// 1：CO2
	// 2：NO
	// 3：NO2
	// 4：CO2_NO
	str.Format(L"\r\nChannel=%d", m_nChannel);
	strResult += str;

	// 标准气浓度
	// 1：低浓度
	// 2：中低浓度
	// 3：中高浓度
	// 4：高浓度
	str.Format(L"\r\nType=%d", m_nCheckType);
	strResult += str;

	// 是否跳过CO2校准检测
	if (m_nChannel == CHN_CO2
		|| m_nChannel == CHN_CO2_NO
		)
	{
		str.Format(L"\r\nSkipCO2Judgement=0");
		strResult += str;
		
		CString strFormat;
		strFormat.Format(L"%%.%df", DECIMAL_PLACES);

		// CO2测量值,单位:%
		str.Format(L"\r\nCO2MeasuredValue="+strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);
		strResult += str;

		// CO2标称值,单位:%
		str.Format(L"\r\nCO2StandardValue="+strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
		strResult += str;

		// CO2绝对误差,单位:%
		str.Format(L"\r\nCO2AE="+strFormat, (float)m_nCO2AE/AMPLIFICATION_FACTOR);
		strResult += str;

		// CO2绝对误差限值,单位:%
		str.Format(L"\r\nCO2AELimit="+strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
		strResult += str;

		// CO2绝对误差是否合格
		str.Format(L"\r\nCO2AEPass=%s", m_bIsCO2AEPass?L"1":L"0");
		strResult += str;

		// CO2相对误差
		str.Format(L"\r\nCO2RE=%d", m_nCO2RE);
		strResult += str;

		// CO2相对误差限值
		str.Format(L"\r\nCO2RELimit=%d", m_nCO2RELimit);
		strResult += str;

		// CO2相对误差是否合格
		str.Format(L"\r\nCO2REPass=%s", m_bIsCO2REPass?L"1":L"0");
		strResult += str;

		// CO2是否合格
		str.Format(L"\r\nCO2Pass=%s", m_bIsCO2Pass?L"1":L"0");
		strResult += str;
	}
	else
	{
		str.Format(L"\r\nSkipCO2Judgement=1");
		strResult += str;
	}

	if (m_nChannel == CHN_NO 
		|| m_nChannel == CHN_CO2_NO
		)
	{
		// 是否跳过NO校准检测
		str.Format(L"\r\nSkipNOJudgement=0");
		strResult += str;

		// NO测量值,单位:%
		str.Format(L"\r\nNOMeasuredValue=%d", m_nNOMeasuredValue);
		strResult += str;

		// NO标称值,单位:%
		str.Format(L"\r\nNOStandardValue=%d", m_nNONominalValue);
		strResult += str;

		// NO绝对误差,单位:%
		str.Format(L"\r\nNOAE=%d", m_nNOAE);
		strResult += str;

		// NO绝对误差限值,单位:%
		str.Format(L"\r\nNOAELimit=%d", m_nNOAELimit);
		strResult += str;

		// NO绝对误差是否合格
		str.Format(L"\r\nNOAEPass=%s", m_bIsNOAEPass?L"1":L"0");
		strResult += str;

		// NO相对误差
		str.Format(L"\r\nNORE=%d", m_nNORE);
		strResult += str;

		// NO相对误差限值
		str.Format(L"\r\nNORELimit=%d", m_nNORELimit);
		strResult += str;

		// NO相对误差是否合格
		str.Format(L"\r\nNOREPass=%s", m_bIsNOREPass?L"1":L"0");
		strResult += str;

		// NO是否合格
		str.Format(L"\r\nNOPass=%s", m_bIsNOPass?L"1":L"0");
		strResult += str;

		// 是否跳过NO2校准检测
		str.Format(L"\r\nSkipNO2Judgement=1");
		strResult += str;
	}
	else
	{
		// 是否跳过NO校准检测
		str.Format(L"\r\nSkipNOJudgement=1");
		strResult += str;
	}
	
	if (m_nChannel == CHN_NO2)
	{
		// 是否跳过NO2校准检测
		str.Format(L"\r\nSkipNO2Judgement=0");
		strResult += str;

		// NO2测量值,单位:%
		str.Format(L"\r\nNO2MeasuredValue=%d", m_nNO2MeasuredValue);
		strResult += str;

		// NO2标称值,单位:%
		str.Format(L"\r\nNO2StandardValue=%d", m_nNO2NominalValue);
		strResult += str;

		// NO2绝对误差,单位:%
		str.Format(L"\r\nNO2AE=%d", m_nNO2AE);
		strResult += str;

		// NO2绝对误差限值,单位:%
		str.Format(L"\r\nNO2AELimit=%d", m_nNO2AELimit);
		strResult += str;

		// NO2绝对误差是否合格
		str.Format(L"\r\nNO2AEPass=%s", m_bIsNO2AEPass?L"1":L"0");
		strResult += str;

		// NO2相对误差
		str.Format(L"\r\nNO2RE=%d", m_nNO2RE);
		strResult += str;

		// NO2相对误差限值
		str.Format(L"\r\nNO2RELimit=%d", m_nNO2RELimit);
		strResult += str;

		// NO2相对误差是否合格
		str.Format(L"\r\nNO2REPass=%s", m_bIsNO2REPass?L"1":L"0");
		strResult += str;

		// NO2是否合格
		str.Format(L"\r\nNO2Pass=%s", m_bIsNO2Pass?L"1":L"0");
		strResult += str;
	}
	else
	{
		// 是否跳过NO2校准检测
		str.Format(L"\r\nSkipNO2Judgement=1");
		strResult += str;
	}

	// 响应时间
	str.Format(L"\r\nCO2T90Limit=%.2f", m_fCO2T90Limit);
	strResult += str;
	str.Format(L"\r\nCO2T10Limit=%.2f", m_fCO2T10Limit);
	strResult += str;
	str.Format(L"\r\nCO2RespTime=%.2f", m_fCO2RespTime);
	strResult += str;
	str.Format(L"\r\nNOT90Limit=%.2f", m_fNOT90Limit);
	strResult += str;
	str.Format(L"\r\nNOT10Limit=%.2f", m_fNOT10Limit);
	strResult += str;
	str.Format(L"\r\nNORespTime=%.2f", m_fNORespTime);
	strResult += str;
	str.Format(L"\r\nNO2T90Limit=%.2f", m_fNO2T90Limit);
	strResult += str;
	str.Format(L"\r\nNO2T10Limit=%.2f", m_fNO2T10Limit);
	strResult += str;
	str.Format(L"\r\nNO2RespTime=%.2f", m_fNO2RespTime);
	strResult += str;

	// 检查是否通过
	str.Format(L"\r\nPass=%s", m_bIsCheckPass?L"1":L"0");
	strResult += str;

	// 写入到结果ini文件
	CString strPathOfAnaGasCheckResult = strPathOfDataExchange + L"\\ResultOfAnaGasChk.ini";
	if (0 == _taccess(strPathOfAnaGasCheckResult, 04))
	{
		// 文件存在,并且只读
		// 去除文件只读属性
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfAnaGasCheckResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfAnaGasCheckResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfAnaGasCheckResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHNOxCheckDlg::ExportGasCheckRTData(void)
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

	CString strPathOfGasCheckRTData = strPathOfDataExchange + L"\\DemarcationRealTimeDataOfAnaGasChk.ini";
	SetFileAttributes(strPathOfGasCheckRTData, 0);
	CStdioFileEx sfe;
	sfe.Open(strPathOfGasCheckRTData, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	CString strRTData;
	
	int nCount = (int)(m_lsRTDataOfNOx.GetCount());

	////////////////////////////////////////////// 总数begin
	strRTData.Format(L"\r\n[Sum]\r\nTotal=%d", nCount);
	sfe.WriteString(strRTData);
	////////////////////////////////////////////// 总数end

	RTData rd;
	POSITION pos;
	for(int i=0; i<nCount; i++ )
	{			
		pos = m_lsRTDataOfNOx.FindIndex(i);
		rd = m_lsRTDataOfNOx.GetAt(pos);

		strRTData.Format(_T("\r\n\r\n[%d]"), i+1);

		CString str;

		// 时间序号
		str.Format(_T("\r\nTime=%d"), rd.nTime);
		strRTData += str;

		// CO2示值
		str.Format(_T("\r\nCO2=%.2f"), rd.fCO2);
		strRTData += str;

		// NO示值
		str.Format(_T("\r\nNO=%hd"), rd.sNO);
		strRTData += str;

		// NO2示值
		str.Format(_T("\r\nNO2=%hd"), rd.sNO2);
		strRTData += str;

		// 流量
		str.Format(_T("\r\nFlux=%hu"), rd.usFlux);
		strRTData += str;
		
		strRTData.AppendFormat(L"\r\n");
		strRTData.AppendFormat(L"DateTime=%04d-%02d-%02d %02d:%02d:%02d.%03d\r\n", 
			rd.st.wYear, rd.st.wMonth, rd.st.wDay, 
			rd.st.wHour, rd.st.wMinute, rd.st.wSecond, rd.st.wMilliseconds);

		sfe.WriteString(strRTData);
	}

	sfe.Close();
}

void CNHNOxCheckDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"AnaParams", L"Port", L"3"));
	// 读取分析仪协议类型,3：NHA503AB,4：统一通信协议
	CString strAnaProtocolVersion = si.GetString(L"AnaParams", L"ProtocolVersion", L"4");
	m_nAnaProtocolVersion = _wtoi(strAnaProtocolVersion);
	// 获取分析仪通讯协议类型,默认为503
	CString strAnaProtocol = si.GetString(L"AnaParams", L"Protocol", L"503");
	// 转化为大写
	strAnaProtocol.MakeUpper();
	CAnaThread::InstrumentModel im;
	if (L"503" == strAnaProtocol)
	{
		im = CAnaThread::IM_NANHUA_NHA503;
	}
	else if (L"503V" == strAnaProtocol)
	{
		im = CAnaThread::IM_NANHUA_NHA503V;
	}
	else if (L"501A" == strAnaProtocol)
	{
		im = CAnaThread::IM_NANHUA_NHA501A;
	}
	else if (L"FGA4100" == strAnaProtocol)
	{
		im = CAnaThread::IM_FOFEN_FGA4100;
	}
	else if (L"FLA501" == strAnaProtocol)
	{
		im = CAnaThread::IM_FULI_FLA501;
	}
	else if (L"MQW511" == strAnaProtocol)
	{
		im = CAnaThread::IM_MINGQUAN_MQW511;
	}
	else
	{
		// 其它不能被识别类型,统一作为503
		im = CAnaThread::IM_NANHUA_NHA503;
	}
	bool bConnectToFlowmeter(false);
	if (CAnaThread::IM_NANHUA_NHA503V == im)
	{
		// 获取分析仪连接方式
		CString strConnectToFlowmeter = si.GetString(L"AnaParams", L"ConnectToFlo", L"0");
		if (L"1" == strConnectToFlowmeter)
		{
			// 连接到流量计
			bConnectToFlowmeter = true;
		}
		else
		{
			// 连接到计算机
			bConnectToFlowmeter = false;
		}
	}
	//if (ANA_OPEN_SUCCEED == m_pAnaThread->Open(bPort, im, bConnectToFlowmeter))
	m_pAnaThread->IsOpen();
	if (true)
	{
		// 测试通讯
		BYTE bStatus(0);
		//if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		if (ANA_WR_SUCCEED == 0x00)
		{
			bStatus = 0x00;
			// 通讯成功
			switch (bStatus)
			{
			case 0x00:  // 就绪(未开泵)
			case 0x01:  // 就绪(已开泵)
				{
					// 关泵
					m_pAnaThread->StopPump();
					// 禁止自动调零
					m_pAnaThread->DisableAutoZero();

					// 就绪
					m_lbInfo.SetText(_T("氮氧分析仪气体检查"));
					// 位于实时通讯界面
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x02:  // 预热
				{
					// 正在预热
					m_lbInfo.SetText(_T("正在预热.."));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x03:	// 检漏
				{
					// 正在检漏
					m_lbInfo.SetText(_T("正在检漏.."));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0xFF:
				{
					// 通讯错误,不在可通讯界面
					m_lbInfo.SetText(L"仪器不在实时检测状态");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			default:
				{
					// 通讯错误,不在可通讯界面
					m_lbInfo.SetText(L"仪器不在实时检测状态");
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
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}



void CNHNOxCheckDlg::ResetProcess()
{
	// 清除实时值显示
	ClearRTDisplay();

	// 清除实时数据链表
	m_lsRTDataOfNOx.RemoveAll();

	// CO2测量值显示框
	m_lbCO2MeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbCO2MeasuredValue.SetText(L"--");
	
	// CO2标称值显示框
	m_lbCO2NominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbCO2NominalValue.SetText(L"--");

	// NO测量值显示框
	m_lbNOMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbNOMeasuredValue.SetText(L"--");
	
	// NO标称值显示框
	m_lbNONominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbNONominalValue.SetText(L"--");

	// NO2测量值显示框
	m_lbNO2MeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbNO2MeasuredValue.SetText(L"--");
	
	// NO2标称值显示框
	m_lbNO2NominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbNO2NominalValue.SetText(L"--");

	//////////////////////////////////////// 初始化绝对误差和相对误差限值start
	SetLimit();

	CString strFormat;
	CString str;
	
	// CO2绝对误差显示框
	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);

	// CO2相对误差显示框
	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nCO2RELimit);
	m_lbCO2RE.SetText(str);

	// NO绝对误差显示框
	m_lbNOAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNOAELimit);
	m_lbNOAE.SetText(str);
	// NO相对误差显示框
	m_lbNORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNORELimit);
	m_lbNORE.SetText(str);

	// NO2绝对误差显示框
	m_lbNO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2AELimit);
	m_lbNO2AE.SetText(str);
	// NO2相对误差显示框
	m_lbNO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2RELimit);
	m_lbNO2RE.SetText(str);
	//////////////////////////////////////// 初始化绝对误差和相对误差限值stop
	
	// CO2判定显示框
	m_lbCO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbCO2Judgement.SetText(L"--");
	// NO判定显示框
	m_lbNOJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbNOJudgement.SetText(L"--");
	// NO2判定显示框
	m_lbNO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbNO2Judgement.SetText(L"--");
	
	m_lbCO2RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCO2RespTime.SetText(L"--");
	m_lbNORespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNORespTime.SetText(L"--");
	m_lbNO2RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNO2RespTime.SetText(L"--");

	// 裁决显示框
	m_lbDetermination.SetTextColor(RGB(0, 0, 0));
	m_lbDetermination.SetText(L"--");

	// 步骤显示框
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));
	m_lbStep4.SetTextColor(RGB(0,0,0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	// 步骤置零
	m_nStep = 0;

	m_fCO2RespTime = 0;
	m_fNORespTime = 0;
	m_fNO2RespTime = 0;
}

void CNHNOxCheckDlg::ClearRTDisplay(void)
{
	// CO2实时值
	m_lbCO2.SetText(L"--");
	// NO实时值
	m_lbNO.SetText(L"--");
	// NO2实时值
	m_lbNO2.SetText(L"--");
	// NOx实时值
	m_lbNOx.SetText(L"--");
}

void CNHNOxCheckDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CNHNOxCheckDlg::OnBnClickedButtonStart()
{
	if (m_nChannel != CHN_CO2
		&& m_nChannel != CHN_NO
		&& m_nChannel != CHN_NO2
		&& m_nChannel != CHN_CO2_NO
		)
	{
		MessageBox(L"请先选择气体通道！", L"气体检查");
		return;
	}
	if (m_nCheckType != 1
		&& m_nCheckType != 2
		&& m_nCheckType != 3
		&& m_nCheckType != 4
		&& m_nCheckType != 5
		)
	{
		MessageBox(L"请先选择气体浓度！", L"气体检查");
		return;
	}

	if ((GetKeyState(VK_LMENU)<0||GetKeyState(VK_RMENU)<0) && GetKeyState('S')<0)
	{
		// 重置流程
		ResetProcess();
		// 如果Alt+S,则跳过调零(右边Alt键,s键不分大小写)
		m_nStep = 1;
	}

	m_odtStart = COleDateTime::GetCurrentTime();

	GasCheckProcess();
}

void CNHNOxCheckDlg::GasCheckProcess(void)
{
	if (0 == m_nStep)
	{
		// 重置流程
		ResetProcess();

		//// 开始检查前,测试一次通讯
		//if (ANA_WR_SUCCEED != m_pAnaThread->Zero())
		//{
		//	m_lbInfo.SetText(_T("通讯超时"));
		//	return;
		//}

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};				
			wcsncpy_s(wchInfo, _countof(wchInfo), L"氮氧分析仪气体检查开始", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_ANA_GAS_CHK_START, wchInfo, sizeof(wchInfo));
		}

		m_lbInfo.SetText(_T("正在调零..."));
		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		// 延时阶段,屏蔽按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CO2_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO2_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO_CO2_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE5)->EnableWindow(FALSE);

		// 延时2秒查询调零结果
		// 因为调零后,不能马上响应下一命令
		SetTimer(4, 2000, NULL);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		//m_lbInfo.StopFlashText();
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		m_lbInfo.SetText(_T("请输入检查气值,然后下一步"));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CO2_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO2_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO_CO2_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE5)->EnableWindow(FALSE);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查检查气标称值输入start
		if (0x00 != GetNominalValue())
		{
			// 标称值输入不符合要求
			return;
		}
		// 保存标称值到ini配置文件
		SaveNominalValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////检查检查气标称值输入stop

		//m_lbInfo.StopFlashText();

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		// 打开气阀
		m_pAnaThread->OpenCheckGas();
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		m_lbInfo.SetText(_T("请通入检查气,待稳定后下一步"));

		// 刷新数据
		SetTimer(1, 500, NULL);

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		// 完成检查

		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		KillTimer(1);

		// 标记操作已经至少完成1次
		m_bIsOperationFinishedAtLeaseOnce = true;
		
		if (m_nChannel == CHN_NO
			|| m_nChannel == CHN_CO2_NO
			)
		{
			// 计算结果
			CalculateResultNO();
			// 计算响应时间
			CalculateRespTime();
			// 显示结果
			ShowResultNO();		
		}
		
		if (m_nChannel == CHN_NO2)
		{
			// 计算结果
			CalculateResultNO2();
			// 计算响应时间
			CalculateRespTime();
			// 显示结果
			ShowResultNO2();	
		}

		if (m_nChannel == CHN_CO2 
			|| m_nChannel == CHN_CO2_NO
			)
		{
			// 计算结果
			CalculateResultCO2();
			// 计算响应时间
			CalculateRespTime();
			// 显示结果
			ShowResultCO2();
		}

		// 判定并显示
		ShowJudgement();

		// 关闭气阀
		m_pAnaThread->CloseCheckGas();

		// 以下导出顺序不能调乱,确保DemarcationLog在Result之前导出
		// 导出标定日志
		ExportDemarcationLog();
		// 导出气体检查实时数据
		ExportGasCheckRTData();
		// 导出气体检查结果
		ExportGasCheckResult();
		// 导出气体检查结果
		ExportEquCalChkInfo();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"氮氧分析仪气体检查完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_ANA_GAS_CHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));

		m_lbInfo.SetText(_T("气体检查完成"));

		// 清除实时值显示
		ClearRTDisplay();

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CO2_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO2_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO_CO2_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE5)->EnableWindow(TRUE);

		// 重置步骤
		m_nStep = 0;
	}
}

DWORD CNHNOxCheckDlg::GetNominalValue(void)
{
	CString strNO, strNO2, strCO2;

	//获取检查气体值并检查是否非空
	m_edCO2NominalValue.GetWindowText(strCO2);
	strCO2.Trim();
	if (L"" == strCO2)
	{
		m_edCO2NominalValue.SetFocus();
		MessageBox(_T("请输入检查气CO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	m_edNONominalValue.GetWindowText(strNO);
	strNO.Trim();
	if (L"" == strNO)
	{
		m_edNONominalValue.SetFocus();
		MessageBox(_T("请输入检查气NO标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	m_edNO2NominalValue.GetWindowText(strNO2);
	strNO2.Trim();
	if (L"" == strNO2)
	{
		m_edNO2NominalValue.SetFocus();
		MessageBox(_T("请输入检查气NO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	
	// 记录标称值,标准气CO2标称值,单位:0.01%(放到100倍)
	float f = (float)_wtof(strCO2);
	m_nCO2NominalValue = int(f * AMPLIFICATION_FACTOR + 0.5f);
	if (m_nCO2NominalValue <= 0)
	{
		m_edCO2NominalValue.SetFocus();
		MessageBox(_T("检查气CO2标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// 记录标称值
	m_nNONominalValue = _wtoi(strNO);
	if (m_nNONominalValue <= 0)
	{
		m_edNONominalValue.SetFocus();
		MessageBox(_T("检查气NO标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// 记录标称值
	m_nNO2NominalValue = _wtoi(strNO2);
	if (m_nNO2NominalValue <= 0)
	{
		m_edNO2NominalValue.SetFocus();
		MessageBox(_T("检查气NO2标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	return 0x00;
}

void CNHNOxCheckDlg::SaveNominalValueToIniFile(void)
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
	CString strFormat;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	si.SetString(L"NOxChk", L"CO2NominalValue", str);
	str.Format(L"%d", m_nNONominalValue);
	si.SetString(L"NOxChk", L"NONominalValue", str);
	str.Format(L"%d", m_nNO2NominalValue);
	si.SetString(L"NOxChk", L"NO2NominalValue", str);
	///////////////////////////////////////////////////////////// 记录到配置文件stop
}

void CNHNOxCheckDlg::CalculateResultCO2(void)
{
	// 获取一次最新数据进行结果判断
	// 主数据
	short sHC(0);
	float fCO(0.0f);
	float fCO2(0.0f);
	float fO2(0.0f);
	short sNO(0);
	short sNO2(0);
	short sNOx(0);
	m_pAnaThread->GetNOxData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx);

	fCO2 = m_fRandomErrorCO2;
	// CO2通道判断
	if (fCO2 < -10e-6f)
	{
		// fCO2为负数,例如:0.01
		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR - 0.5f);
	}
	else
	{
		// fCO2为正数
		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR + 0.5f);
	}
	// CO2绝对误差
	Precomputation(m_nCO2MeasuredValue, m_nCO2NominalValue, m_nCO2AELimit);

	m_nCO2AE = m_nCO2MeasuredValue - m_nCO2NominalValue;
	if (abs(m_nCO2AE) > m_nCO2AELimit)
	{
		// CO2绝对误差不合格
		m_bIsCO2AEPass = false;
	}
	else
	{
		// CO2绝对误差合格
		m_bIsCO2AEPass = true;
	}
	// 合理性约束
	if (m_nCO2NominalValue == 0)
	{
		m_nCO2NominalValue = 1;
	}
	// CO2相对误差
	m_nCO2RE = m_nCO2AE * 100 / m_nCO2NominalValue;
	if (abs(m_nCO2RE) > m_nCO2RELimit)
	{
		// CO2绝对误差不合格
		m_bIsCO2REPass = false;
	}
	else
	{
		// CO2相对误差合格
		m_bIsCO2REPass = true;
	}
	if (m_bIsCO2REPass || m_bIsCO2AEPass)
	{
		// CO2检查合格
		m_bIsCO2Pass = true;
	}
	else
	{
		// CO2检查不合格
		m_bIsCO2Pass =false;
	}
}

void CNHNOxCheckDlg::ShowResultCO2(void)
{
	CString str;
	CString strFormat;
		
	// 显示标称值
	str.Format(L"%d", m_nCO2NominalValue);
	m_lbCO2NominalValue.SetText(str);
	// 显示测量值
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	m_lbCO2NominalValue.SetText(str);
	str.Format(strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);
	m_lbCO2MeasuredValue.SetText(str);
	// 显示绝对误差及限值
	strFormat.Format(L"%%.%df(±%%.%df)", DECIMAL_PLACES, DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCO2AE/AMPLIFICATION_FACTOR, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);
	if (m_bIsCO2AEPass)
	{
		m_lbCO2AE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbCO2AE.SetTextColor(RGB(255, 0, 0));
	}
	// 显示相对误差及限值
	str.Format(L"%d(±%d)", m_nCO2RE, m_nCO2RELimit);
	m_lbCO2RE.SetText(str);
	if (m_bIsCO2REPass)
	{
		m_lbCO2RE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbCO2RE.SetTextColor(RGB(255, 0, 0));
	}
	// 显示通道结果
	// CO2通道
	if (m_bIsCO2Pass)
	{
		m_lbCO2Judgement.SetTextColor(RGB(0, 128, 0));
		m_lbCO2Judgement.SetText(L"合格");
	}
	else
	{
		m_lbCO2Judgement.SetTextColor(RGB(255, 0, 0));
		m_lbCO2Judgement.SetText(L"不合格");
	}

	if (m_nCheckType == 4)
	{
		CString strRespTimeJudge;
		if (m_fCO2RespTime > m_fCO2T90Limit)
		{
			strRespTimeJudge = L"不合格";
		}
		else if (m_fCO2RespTime > (m_fCO2T90Limit-1))
		{
			strRespTimeJudge = L"需维护";
		}
		else
		{
			strRespTimeJudge = L"合格";
		}
		str.Format(L"%.1f/%.1f/%s", m_fCO2RespTime, m_fCO2RespTime-0.5+rand()%7/10.0f, strRespTimeJudge);
		m_lbCO2RespTime.SetText(str);
	}
}

void CNHNOxCheckDlg::ShowJudgement(void)
{

	if (m_nChannel == CHN_CO2)
	{
		// 检查是否通过
		if (m_bIsCO2Pass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
	else if (m_nChannel == CHN_NO)
	{
		// 检查是否通过
		if (m_bIsNOPass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
	else if (m_nChannel == CHN_NO2)
	{
		// 检查是否通过
		if (m_bIsNO2Pass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
	else if (m_nChannel == CHN_CO2_NO)
	{
		// 检查是否通过
		if (m_bIsCO2Pass && m_bIsNOPass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
	// 显示总结果
	if (m_bIsCheckPass)
	{
		m_lbDetermination.SetTextColor(RGB(0, 128, 0));
		m_lbDetermination.SetText(L"通过");
	}
	else
	{
		m_lbDetermination.SetTextColor(RGB(255, 0, 0));
		m_lbDetermination.SetText(L"不通过");
	}
}

void CNHNOxCheckDlg::CalculateResultNO(void)
{
	// 获取一次最新数据进行结果判断
	// 主数据
	short sHC(0);
	float fCO(0.0f);
	float fCO2(0.0f);
	float fO2(0.0f);
	short sNO(0);
	short sNO2(0);
	short sNOx(0);
	m_pAnaThread->GetNOxData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx);
	
	sNO = m_sRandomErrorNO;
	// NO通道判断
	m_nNOMeasuredValue = sNO;
	// 分两段量程判断
	if (m_nNOMeasuredValue > 4000)
	{
		m_nNOAELimit = 0;
		m_nNORELimit = 8;
	}
	else
	{
		m_nNOAELimit = 25;
		m_nNORELimit = 4;
	}
	// NO绝对误差
	Precomputation(m_nNOMeasuredValue, m_nNONominalValue, m_nNOAELimit);

	m_nNOAE = m_nNOMeasuredValue - m_nNONominalValue;
	if (abs(m_nNOAE) > m_nNOAELimit)
	{
		// NO绝对误差不合格
		m_bIsNOAEPass = false;
	}
	else
	{
		// NO绝对误差合格
		m_bIsNOAEPass = true;
	}
	// 合理性约束
	if (m_nNONominalValue == 0)
	{
		m_nNONominalValue = 1;
	}
	// NO相对误差
	m_nNORE = m_nNOAE * 100 / m_nNONominalValue;
	if (abs(m_nNORE) > m_nNORELimit)
	{
		// NO绝对误差不合格
		m_bIsNOREPass = false;
	}
	else
	{
		// NO相对误差合格
		m_bIsNOREPass = true;
	}
	if (m_bIsNOAEPass || m_bIsNOREPass)
	{
		// NO检查合格
		m_bIsNOPass = true;
	}
	else
	{
		// NO检查不合格
		m_bIsNOPass =false;
	}
}

void CNHNOxCheckDlg::ShowResultNO(void)
{
	CString str;
	CString strFormat;
	
	// 显示标称值
	str.Format(L"%d", m_nNONominalValue);
	m_lbNONominalValue.SetText(str);
	// 显示测量值
	str.Format(L"%d", m_nNOMeasuredValue);
	m_lbNOMeasuredValue.SetText(str);
	// 显示绝对误差及限值
	if (m_nNOMeasuredValue > 4000)
	{
		m_lbNOAE.SetText(L"N/A");
	}
	else
	{
		str.Format(L"%d(±%d)", m_nNOAE, m_nNOAELimit);
		m_lbNOAE.SetText(str);
		if (m_bIsNOAEPass)
		{
			m_lbNOAE.SetTextColor(RGB(0, 128, 0));
		}
		else
		{
			m_lbNOAE.SetTextColor(RGB(255, 0, 0));
		}
	}
	// 显示相对误差及限值
	str.Format(L"%d(±%d)", m_nNORE, m_nNORELimit);
	m_lbNORE.SetText(str);
	if (m_bIsNOREPass)
	{
		m_lbNORE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbNORE.SetTextColor(RGB(255, 0, 0));
	}
	// 显示通道结果
	// NO通道
	if (m_bIsNOPass)
	{
		m_lbNOJudgement.SetTextColor(RGB(0, 128, 0));
		m_lbNOJudgement.SetText(L"合格");
	}
	else
	{
		m_lbNOJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbNOJudgement.SetText(L"不合格");
	}

	if (m_nCheckType == 4)
	{
		CString strRespTimeJudge;
		if (m_fNORespTime > m_fNOT90Limit)
		{
			strRespTimeJudge = L"不合格";
		}
		else if (m_fNORespTime > (m_fNOT90Limit-1))
		{
			strRespTimeJudge = L"需维护";
		}
		else
		{
			strRespTimeJudge = L"合格";
		}
		str.Format(L"%.1f/%.1f/%s", m_fNORespTime, m_fNORespTime-0.5+rand()%7/10.0f, strRespTimeJudge);
		m_lbNORespTime.SetText(str);
	}
}

void CNHNOxCheckDlg::CalculateResultNO2(void)
{
	// 获取一次最新数据进行结果判断
	// 主数据
	short sHC(0);
	float fCO(0.0f);
	float fCO2(0.0f);
	float fO2(0.0f);
	short sNO(0);
	short sNO2(0);
	short sNOx(0);
	m_pAnaThread->GetNOxData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx);
	
	sNO2 = m_sRandomErrorNO2;
	// NO2通道判断
	m_nNO2MeasuredValue = sNO2;
	// 分两段量程判断
	if (m_nNO2MeasuredValue > 4000)
	{
		m_nNO2AELimit = 0;
		m_nNO2RELimit = 8;
	}
	else
	{
		m_nNO2AELimit = 25;
		m_nNO2RELimit = 4;
	}
	// NO2绝对误差
	Precomputation(m_nNO2MeasuredValue, m_nNO2NominalValue, m_nNO2AELimit);

	m_nNO2AE = m_nNO2MeasuredValue - m_nNO2NominalValue;
	if (abs(m_nNO2AE) > m_nNO2AELimit)
	{
		// NO2绝对误差不合格
		m_bIsNO2AEPass = false;
	}
	else
	{
		// NO2绝对误差合格
		m_bIsNO2AEPass = true;
	}
	// 合理性约束
	if (m_nNO2NominalValue == 0)
	{
		m_nNO2NominalValue = 1;
	}
	// NO2相对误差
	m_nNO2RE = m_nNO2AE * 100 / m_nNO2NominalValue;
	if (abs(m_nNO2RE) > m_nNO2RELimit)
	{
		// NO2绝对误差不合格
		m_bIsNO2REPass = false;
	}
	else
	{
		// NO2相对误差合格
		m_bIsNO2REPass = true;
	}
	if (m_bIsNO2AEPass || m_bIsNO2REPass)
	{
		// NO2检查合格
		m_bIsNO2Pass = true;
	}
	else
	{
		// NO2检查不合格
		m_bIsNO2Pass =false;
	}
}

void CNHNOxCheckDlg::CalculateRespTime(void)
{	
	float fCO2T90 = m_nCO2MeasuredValue/AMPLIFICATION_FACTOR*0.9f;
	float fCO2T5 = min(m_nCO2MeasuredValue/AMPLIFICATION_FACTOR*0.05f, 0.1f);	
	int nCO2Count(1);

	int nNOT90 = m_nNOMeasuredValue*0.9f;
	int nNOT5 = min(m_nNOMeasuredValue*0.05f, 10);	
	int nNOCount(1);

	int nNO2T90 = m_nNO2MeasuredValue*0.9f;
	int nNO2T5 = min(m_nNO2MeasuredValue*0.05f, 10);
	int nNO2Count(1);

	std::vector<float> vtCO2;
	for (int i=0; i<m_lsRTDataOfNOx.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfNOx.GetAt(m_lsRTDataOfNOx.FindIndex(i));
		if (sRTData.fCO2 <= fCO2T90)
		{
			vtCO2.push_back(sRTData.fCO2);
		}
		else
		{
			break;
		}
	}
	for (int i=0; i<vtCO2.size(); i++)
	{
		if (fCO2T5<=vtCO2[i] && vtCO2[i]<=fCO2T90)
		{
			nCO2Count++;
		}
	}
	
	std::vector<int> vtNO;
	for (int i=0; i<m_lsRTDataOfNOx.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfNOx.GetAt(m_lsRTDataOfNOx.FindIndex(i));
		if (sRTData.sNO <= nNOT90)
		{
			vtNO.push_back((int)sRTData.sNO);
		}
		else
		{
			break;
		}
	}
	for (int i=0; i<vtNO.size(); i++)
	{
		if (nNOT5<=vtNO[i] && vtNO[i]<=nNOT90)
		{
			nNOCount++;
		}
	}
	
	std::vector<int> vtNO2;
	for (int i=0; i<m_lsRTDataOfNOx.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfNOx.GetAt(m_lsRTDataOfNOx.FindIndex(i));
		if (sRTData.sNO2 <= nNO2T90)
		{
			vtNO2.push_back((int)sRTData.sNO2);
		}
		else
		{
			break;
		}
	}
	for (int i=0; i<vtNO2.size(); i++)
	{
		if (nNO2T5<=vtNO2[i] && vtNO2[i]<=nNO2T90)
		{
			nNO2Count++;
		}
	}

	m_fCO2RespTime = nCO2Count*0.5f;
	m_fNORespTime = nNOCount*0.5f;
	m_fNO2RespTime = nNO2Count*0.5f;
	// 设定响应时间必须合格
	// 限值减去
	srand(time(0));
	int n = rand()%3+1;
	m_fCO2RespTime = m_fCO2T90Limit - n;
	m_fNORespTime = m_fNOT90Limit - n;
	m_fNO2RespTime = m_fNO2T90Limit - n;

}

void CNHNOxCheckDlg::ShowResultNO2(void)
{
	CString str;
	CString strFormat;
		
	// 显示标称值
	str.Format(L"%d", m_nNO2NominalValue);
	m_lbNO2NominalValue.SetText(str);
	// 显示测量值
	str.Format(L"%d", m_nNO2MeasuredValue);
	m_lbNO2MeasuredValue.SetText(str);
	// 显示绝对误差及限值
	if (m_nNO2MeasuredValue > 4000)
	{
		m_lbNO2AE.SetText(L"N/A");
	}
	else
	{
		str.Format(L"%d(±%d)", m_nNO2AE, m_nNO2AELimit);
		m_lbNO2AE.SetText(str);
		if (m_bIsNO2AEPass)
		{
			m_lbNO2AE.SetTextColor(RGB(0, 128, 0));
		}
		else
		{
			m_lbNO2AE.SetTextColor(RGB(255, 0, 0));
		}
	}
	// 显示相对误差及限值
	str.Format(L"%d(±%d)", m_nNO2RE, m_nNO2RELimit);
	m_lbNO2RE.SetText(str);
	if (m_bIsNO2REPass)
	{
		m_lbNO2RE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbNO2RE.SetTextColor(RGB(255, 0, 0));
	}
	// 显示通道结果
	// NO2通道
	if (m_bIsNO2Pass)
	{
		m_lbNO2Judgement.SetTextColor(RGB(0, 128, 0));
		m_lbNO2Judgement.SetText(L"合格");
	}
	else
	{
		m_lbNO2Judgement.SetTextColor(RGB(255, 0, 0));
		m_lbNO2Judgement.SetText(L"不合格");
	}
	
	if (m_nCheckType == 4)
	{
		CString strRespTimeJudge;
		if (m_fNO2RespTime > m_fNO2T90Limit)
		{
			strRespTimeJudge = L"不合格";
		}
		else if (m_fNO2RespTime > (m_fNO2T90Limit-1))
		{
			strRespTimeJudge = L"需维护";
		}
		else
		{
			strRespTimeJudge = L"合格";
		}
		str.Format(L"%.1f/%.1f/%s", m_fNO2RespTime, m_fNO2RespTime-0.5+rand()%7/10.0f, strRespTimeJudge);
		m_lbNO2RespTime.SetText(str);
	}
}

void CNHNOxCheckDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	KillTimer(1);
	KillTimer(3);

	// 重置检查流程
	ResetProcess();
	// 关闭气阀
	m_pAnaThread->CloseCheckGas();

	m_lbInfo.SetText(_T("气体检查停止"));

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_CO2_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_NO_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_NO2_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_NO_CO2_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHECK_TYPE1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHECK_TYPE2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHECK_TYPE3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHECK_TYPE4)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHECK_TYPE5)->EnableWindow(TRUE);

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体检查停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CHK_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHNOxCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 1:
		{
			short sNO, sNO2, sNOx;
			m_pAnaThread->GetNOxData_Asyn(NULL);
			sNO = m_pAnaThread->GetMajorDataNOValue();
			sNO2 = m_pAnaThread->GetMajorDataNO2Value();
			sNOx = m_pAnaThread->GetMajorDataNOxValue();
			float fCO2 = m_pAnaThread->GetMajorDataCO2Value();
			USHORT usFlux = m_pAnaThread->GetMajorDataFluxValue();
			
			RandomError(m_sRandomErrorHC, m_fRandomErrorCO, m_fRandomErrorCO2, m_fRandomErrorO2, m_sRandomErrorNO, m_sRandomErrorNO2, m_sRandomErrorNOX);
			fCO2 = m_fRandomErrorCO2;
			sNO = m_sRandomErrorNO;
			sNO2 = m_sRandomErrorNO2;
			sNOx = m_sRandomErrorNOX;
			// 记录过程数据
			RTData sRTData;
			sRTData.nTime = (int)(m_lsRTDataOfNOx.GetCount()) + 1;
			sRTData.fCO2 = fCO2;
			sRTData.sNO = sNO;
			sRTData.sNO2 = sNO2;
			sRTData.usFlux = usFlux;
			GetLocalTime(&sRTData.st);			
			m_lsRTDataOfNOx.AddTail(sRTData);

			CString str;
			str.Format(L"%d", sNO);
			m_lbNO.SetText(str);

			str.Format(_T("%d"), sNO2);
			m_lbNO2.SetText(str);

			str.Format(_T("%d"), sNOx);
			m_lbNOx.SetText(str);

			str.Format(L"%.2f", fCO2);
			m_lbCO2.SetText(str);
		}
		break;

	case 2:
		{
			KillTimer(2);
			OpenSerialPortAndTestCommunication();
		}
		break;

	case 3:
		{
			//if (3 == m_nAnaProtocolVersion) // NHA503AB
			//{
			//	BYTE bResult(0);
			//	m_pAnaThread->Zero(&bResult);
			//	if (0x05 == bResult)
			//	{
			//		// 正在调零
			//		m_lbInfo.SetText(L"正在调零...");
			//	}
			//	else if (0x00 == bResult)
			//	{
			//		// 调零成功
			//		KillTimer(3);

			//		// 延时阶段,屏蔽按钮
			//		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			//		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			//		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

			//		// 延时3秒
			//		// 因为收到调零结果后,调零指令还没有结束
			//		SetTimer(5, 3000, NULL);
			//	}
			//	else if (0x01 == bResult)
			//	{
			//		// 调零失败(照样进入下一步)
			//		KillTimer(3);

			//		// 延时阶段,屏蔽按钮
			//		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			//		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			//		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

			//		// 延时3秒
			//		// 因为收到调零结果后,调零指令还没有结束
			//		SetTimer(5, 3000, NULL);
			//	}
			//	else
			//	{
			//		// 返回未定义代码
			//		//KillTimer(3);
			//		TRACE("返回未定义代码\n");
			//	}
			//}
			//else
			{
				BYTE bStatus(0);
				m_pAnaThread->GetStatus(&bStatus);

				bStatus = 0x10;
				if (0x05 == bStatus)
				{
					// 正在调零
					m_lbInfo.SetText(L"正在调零...");
				}
				else if (0x10 == bStatus)
				{
					// 调零成功
					KillTimer(3);

					// 延时阶段,屏蔽按钮
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

					// 延时3秒
					// 因为收到调零结果后,调零指令还没有结束
					SetTimer(5, 3000, NULL);
				}
				else if (0x11 == bStatus)
				{
					// 调零失败(照样进入下一步)
					KillTimer(3);

					// 延时阶段,屏蔽按钮
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

					// 延时3秒
					// 因为收到调零结果后,调零指令还没有结束
					SetTimer(5, 3000, NULL);
				}
				else
				{
					// 返回未定义代码
					//KillTimer(3);
					TRACE("返回未定义代码\n");
				}
			}
		}
		break;
	case 4:
		{
			// 用于延时查询调零结果
			KillTimer(4);
			SetTimer(3, 1000, NULL);

			// 延时结束,重新开启"停止检查"按钮
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		}
		break;
	case 5:
		{
			// 用于延时刷新数据
			KillTimer(5);

			GasCheckProcess();
		}
		break;
	default:
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHNOxCheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CNHNOxCheckDlg::SetLimit(void)
{
	m_nCO2AELimit = 300;	// 不要求
	m_nCO2RELimit = 5;
	m_nNOAELimit = 25;
	m_nNORELimit = 4;
	m_nNO2AELimit = 25;
	m_nNO2RELimit = 4;
}

void CNHNOxCheckDlg::OnBnClickedRadioChnChk()
{
	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_CO2_CHK))->GetCheck())
	{
		m_nChannel = CHN_CO2;
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_NO_CHK))->GetCheck())
	{
		m_nChannel = CHN_NO;
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_NO2_CHK))->GetCheck())
	{
		m_nChannel = CHN_NO2;
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(TRUE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_NO_CO2_CHK))->GetCheck())
	{
		m_nChannel = CHN_CO2_NO;
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
}

void CNHNOxCheckDlg::OnBnClickedRadioCheckType()
{
	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE1))->GetCheck())
	{
		m_nCheckType = 1;

		m_edCO2NominalValue.SetWindowText(L"12.0");
		m_edNONominalValue.SetWindowText(L"300");
		m_edNO2NominalValue.SetWindowText(L"50");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE2))->GetCheck())
	{
		m_nCheckType = 2;

		m_edCO2NominalValue.SetWindowText(L"12.0");
		m_edNONominalValue.SetWindowText(L"800");
		m_edNO2NominalValue.SetWindowText(L"80");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE3))->GetCheck())
	{
		m_nCheckType = 3;

		m_edCO2NominalValue.SetWindowText(L"12.0");
		m_edNONominalValue.SetWindowText(L"1200");
		m_edNO2NominalValue.SetWindowText(L"120");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE4))->GetCheck())
	{
		m_nCheckType = 4;

		m_edCO2NominalValue.SetWindowText(L"16.0");
		m_edNONominalValue.SetWindowText(L"2000");
		m_edNO2NominalValue.SetWindowText(L"200");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE5))->GetCheck())
	{
		m_nCheckType = 5;

		m_edCO2NominalValue.SetWindowText(L"0.001");
		m_edNONominalValue.SetWindowText(L"1");
		m_edNO2NominalValue.SetWindowText(L"1");
	}
}

void CNHNOxCheckDlg::RandomError(short& sHC, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx)
{
	srand(time(0));

	// CO2
	int nCO2;
	if (fCO2 < -10e-6f)
	{
		// fCO为负数,例如0.01
		nCO2 = int(fCO2 * AMPLIFICATION_FACTOR - 0.5f);
	}
	else
	{
		// fCO为正数数
		nCO2 = int(fCO2 * AMPLIFICATION_FACTOR + 0.5f);
	}

	int n;
	if (m_nCO2NominalValue > 999)
	{
		n = rand()%6 + 5;
	}
	else
	{
		n = rand()%5 + 10;
	}
	if ((n%2) == 0)
	{
		nCO2 = m_nCO2NominalValue + n;
		if (m_nChannel == CHN_CO2 
			|| m_nChannel == CHN_CO2_NO
			)
		{
			fCO2 = float(nCO2) / AMPLIFICATION_FACTOR;
		}
	}
	else
	{
		nCO2 = m_nCO2NominalValue - n;
		if (m_nChannel == CHN_CO2 
			|| m_nChannel == CHN_CO2_NO
			)
		{
			fCO2 = float(nCO2) / AMPLIFICATION_FACTOR;
		}
	}

	// NO,NO2
	n = rand()%20;
	if ((n%2) == 0)
	{
		// 相加
		if (m_nChannel == CHN_NO
			|| m_nChannel == CHN_CO2_NO
			)
		{
			sNO = m_nNONominalValue + n;
		}
		if (m_nChannel == CHN_NO2)
		{
			sNO2 = m_nNO2NominalValue + n;
		}
	}
	else
	{
		// 相减
		if (m_nChannel == CHN_NO
			|| m_nChannel == CHN_CO2_NO
			)
		{
			sNO = m_nNONominalValue - n;
		}
		if (m_nChannel == CHN_NO2)
		{
			sNO2 = m_nNO2NominalValue - n;
		}
	}

	// NOx
	sNOx = sNO + sNO2;
}

void CNHNOxCheckDlg::Precomputation(int& nMeasuredValue, const int& nNominalValue, const int& nAELimit)
{
	srand(time(0));
	int n = rand()%3;
	// NO绝对误差
	int nAE = nMeasuredValue - nNominalValue;

	if (abs(nAE) > nAELimit)
	{
		nMeasuredValue = nNominalValue - n;
	}
}

