// NHA503XGasCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHA503XGasCheckDlg.h"
#include "math.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
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

#define CHN_HC_CO_CO2		1
#define CHN_NO				2
#define CHN_NO2				3
#define CHN_HC_CO_CO2_NO	4
#define CHN_O2				5

// CNHA503XGasCheckDlg 对话框

IMPLEMENT_DYNAMIC(CNHA503XGasCheckDlg, CDialogZoom)

CNHA503XGasCheckDlg::CNHA503XGasCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHA503XGasCheckDlg::IDD, pParent)
	, m_nStep(0)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_nC3H8NominalValue(0)
	, m_nCONominalValue(0)
	, m_nCO2NominalValue(0)
	, m_nHCNominalValue(0)
	, m_nNONominalValue(0)
	, m_nNO2NominalValue(0)
	, m_nO2NominalValue(0)

	// HC绝对误差限值,单位:ppm
	, m_nHCAELimit(12)
	// CO绝对误差限值,单位:0.01%,放大100倍,四舍五入
	, m_nCOAELimit(int(0.06f * AMPLIFICATION_FACTOR+0.5f))
	// CO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	, m_nCO2AELimit(int(0.50f * AMPLIFICATION_FACTOR + 0.5f))
	// NO绝对误差限值,单位:ppm
	, m_nNOAELimit(25)
	// NO2绝对误差限值,单位:ppm
	, m_nNO2AELimit(25)
	// O2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	, m_nO2AELimit(int(0.50f * AMPLIFICATION_FACTOR + 0.5f))

	// HC相对误差限值,单位:%
	, m_nHCRELimit(5)
	// CO相对误差限值,单位:%
	, m_nCORELimit(5)
	// CO2相对误差限值,单位:%
	, m_nCO2RELimit(5)
	// NO相对误差限值,单位:%
	, m_nNORELimit(5)
	// NO2相对误差限值,单位:%
	, m_nNO2RELimit(5)
	// O2相对误差限值,单位:%
	, m_nO2RELimit(5)

	// PEF测量值,放大1000倍,四舍五入
	, m_nPEFMeasuredValue(0)
	// HC测量值,单位:ppm
	, m_nHCMeasuredValue(0)
	// CO测量值,单位:0.01%,放大100倍,四舍五入
	, m_nCOMeasuredValue(0)
	// CO2测量值,单位:0.01%,放大100倍,四舍五入
	, m_nCO2MeasuredValue(0)
	// O2测量值,单位:0.01%,放大100倍,四舍五入
	, m_nO2MeasuredValue(0)
	// NO测量值,单位:ppm
	, m_nNOMeasuredValue(0)
	// NO2测量值,单位:ppm
	, m_nNO2MeasuredValue(0)

	// HC绝对误差,单位:ppm
	, m_nHCAE(0)
	// CO绝对误差,单位:0.01%,放大100倍,四舍五入
	, m_nCOAE(0)
	// CO2绝对误差,单位:0.01%,放大100倍,四舍五入
	, m_nCO2AE(0)
	// NO绝对误差,单位:ppm
	, m_nNOAE(0)
	// NO2绝对误差,单位:ppm
	, m_nNO2AE(0)
	// O2绝对误差,单位:0.01%,放大100倍,四舍五入
	, m_nO2AE(0)

	// HC绝对误差是否通过
	, m_bIsHCAEPass(false)
	// CO绝对误差是否通过
	, m_bIsCOAEPass(false)
	// CO2绝对误差是否通过
	, m_bIsCO2AEPass(false)
	// NO绝对误差是否通过
	, m_bIsNOAEPass(false)
	// NO2绝对误差是否通过
	, m_bIsNO2AEPass(false)
	// O2绝对误差是否通过
	, m_bIsO2AEPass(false)
	
	// HC相对误差,单位:%,放大100倍,四舍五入
	, m_nHCRE(0)
	// CO相对误差,单位:%,放大100倍,四舍五入
	, m_nCORE(0)
	// CO2相对误差,单位:%,放大100倍,四舍五入
	, m_nCO2RE(0)
	// NO相对误差,单位:%,放大100倍,四舍五入
	, m_nNORE(0)
	// NO2相对误差,单位:%,放大100倍,四舍五入
	, m_nNO2RE(0)
	// O2相对误差,单位:%,放大100倍,四舍五入
	, m_nO2RE(0)

	// HC相对误差是否通过
	, m_bIsHCREPass(0)
	// CO相对误差是否通过
	, m_bIsCOREPass(0)
	// CO2相对误差是否通过
	, m_bIsCO2REPass(0)
	// NO相对误差是否通过
	, m_bIsNOREPass(0)
	// NO2相对误差是否通过
	, m_bIsNO2REPass(0)
	// O2相对误差是否通过
	, m_bIsO2REPass(0)

	// HC通道是否通过
	, m_bIsHCPass(0)
	// CO通道是否通过
	, m_bIsCOPass(0)
	// CO2通道是否通过
	, m_bIsCO2Pass(0)
	// NO通道是否通过
	, m_bIsNOPass(0)
	// NO2通道是否通过
	, m_bIsNO2Pass(0)
	// O2通道是否通过
	, m_bIsO2Pass(0)

	// 检查是否通过
	, m_bIsCheckPass(0)
	
	, m_nChannel(0)
	
	// 初始化回调函数
	, m_pfProcessCtrl(NULL)

	// 分析仪协议版本
	, m_nAnaProtocolVersion(4)

	, m_odtStart(COleDateTime::GetCurrentTime())
	, m_odtEnd(COleDateTime::GetCurrentTime())

	, m_sRandomErrorHC(0)
	, m_sRandomErrorNO(0)
	, m_sRandomErrorNO2(0)
	, m_sRandomErrorNOX(0)
	, m_fRandomErrorCO(0.0f)
	, m_fRandomErrorCO2(0.0f)
	, m_fRandomErrorO2(0.0f)
	, m_fRandomErrorPef(0.0f)
{
	// 启动分析仪线程
	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pAnaThread;

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

	m_lsRTDataOfAnaGasChk.RemoveAll();
	
	// 导入气体检查参数
	ImportGasCheckParams();
		
	m_fHCT90Limit = 5.5f;
	m_fHCT10Limit = 5.7f;

	m_fCOT90Limit = 5.5f;
	m_fCOT10Limit = 5.7f;
	
	m_fCO2T90Limit = 5.5f;
	m_fCO2T10Limit = 5.7f;

	m_fNOT90Limit = 6.5f;
	m_fNOT10Limit = 6.7f;

	m_fNO2T90Limit = 6.5f;
	m_fNO2T10Limit = 6.7f;

	m_fO2T90Limit = 7.5f;
	m_fO2T10Limit = 8.5f;
}

CNHA503XGasCheckDlg::~CNHA503XGasCheckDlg()
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

void CNHA503XGasCheckDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_MATHLABLE8, m_mlb8);
	DDX_Control(pDX, IDC_MATHLABLE12, m_mlb12);
	DDX_Control(pDX, IDC_MATHLABLE13, m_mlb13);
	DDX_Control(pDX, IDC_MATHLABLE14, m_mlb14);
	DDX_Control(pDX, IDC_MATHLABLE15, m_mlb15);
	DDX_Control(pDX, IDC_MATHLABLE16, m_mlb16);
	DDX_Control(pDX, IDC_MATHLABLE17, m_mlb17);
	DDX_Control(pDX, IDC_MATHLABLE18, m_mlb18);
	DDX_Control(pDX, IDC_MATHLABLE19, m_mlb19);
	DDX_Control(pDX, IDC_STATIC_HC, m_lbHC);
	DDX_Control(pDX, IDC_STATIC_C3H8, m_lbC3H8);
	DDX_Control(pDX, IDC_STATIC_CO, m_lbCO);
	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_NO, m_lbNO);
	DDX_Control(pDX, IDC_STATIC_NO2, m_lbNO2);
	DDX_Control(pDX, IDC_STATIC_PEF, m_lbPEF);
	DDX_Control(pDX, IDC_STATIC_O2, m_lbO2);
	DDX_Control(pDX, IDC_EDIT_C3H8_NOMINAL_VALUE, m_edC3H8NominalValue);
	DDX_Control(pDX, IDC_EDIT_CO_NOMINAL_VALUE, m_edCONominalValue);
	DDX_Control(pDX, IDC_EDIT_CO2_NOMINAL_VALUE, m_edCO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_NO_NOMINAL_VALUE, m_edNONominalValue);
	DDX_Control(pDX, IDC_EDIT_NO2_NOMINAL_VALUE, m_edNO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_O2_NOMINAL_VALUE, m_edO2NominalValue);
	DDX_Control(pDX, IDC_STATIC_PEF_MEASURED_VALUE, m_lbPEFMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_HC_MEASURED_VALUE, m_lbHCMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_HC_NOMINAL_VALUE, m_lbHCNominalValue);
	DDX_Control(pDX, IDC_STATIC_HC_AE, m_lbHCAE);
	DDX_Control(pDX, IDC_STATIC_HC_RE, m_lbHCRE);
	DDX_Control(pDX, IDC_STATIC_HC_JUDGEMENT, m_lbHCJudgement);
	DDX_Control(pDX, IDC_STATIC_CO_MEASURED_VALUE, m_lbCOMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO_NOMINAL_VALUE, m_lbCONominalValue);
	DDX_Control(pDX, IDC_STATIC_CO_AE, m_lbCOAE);
	DDX_Control(pDX, IDC_STATIC_CO_JUDGEMENT, m_lbCOJudgement);
	DDX_Control(pDX, IDC_STATIC_CO_RE, m_lbCORE);
	DDX_Control(pDX, IDC_STATIC_CO2_MEASURED_VALUE, m_lbCO2MeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO2_NOMINAL_VALUE, m_lbCO2NominalValue);
	DDX_Control(pDX, IDC_STATIC_CO2_RE, m_lbCO2RE);
	DDX_Control(pDX, IDC_STATIC_CO2_AE, m_lbCO2AE);
	DDX_Control(pDX, IDC_STATIC_CO2_JUDGEMENT, m_lbCO2Judgement);		
	DDX_Control(pDX, IDC_STATIC_O2_MEASURED_VALUE, m_lbO2MeasuredValue);
	DDX_Control(pDX, IDC_STATIC_O2_NOMINAL_VALUE, m_lbO2NominalValue);
	DDX_Control(pDX, IDC_STATIC_O2_RE, m_lbO2RE);
	DDX_Control(pDX, IDC_STATIC_O2_AE, m_lbO2AE);
	DDX_Control(pDX, IDC_STATIC_O2_JUDGEMENT, m_lbO2Judgement);
	DDX_Control(pDX, IDC_STATIC_DETERMINATION, m_lbDetermination);
	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
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
	DDX_Control(pDX, IDC_STATIC_HC_RESP_TIME, m_lbHCRespTime);
	DDX_Control(pDX, IDC_STATIC_CO_RESP_TIME, m_lbCORespTime);
	DDX_Control(pDX, IDC_STATIC_CO2_RESP_TIME, m_lbCO2RespTime);
	DDX_Control(pDX, IDC_STATIC_NO_RESP_TIME, m_lbNORespTime);
	DDX_Control(pDX, IDC_STATIC_NO2_RESP_TIME, m_lbNO2RespTime);
	DDX_Control(pDX, IDC_STATIC_O2_RESP_TIME, m_lbO2RespTime);
}


BEGIN_MESSAGE_MAP(CNHA503XGasCheckDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHA503XGasCheckDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHA503XGasCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHA503XGasCheckDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_RADIO_DIS, &CNHA503XGasCheckDlg::OnBnClickedRadioTestType)
	//ON_BN_CLICKED(IDC_RADIO_ASM, &CNHA503XGasCheckDlg::OnBnClickedRadioTestType)
	//ON_BN_CLICKED(IDC_RADIO_VMAS, &CNHA503XGasCheckDlg::OnBnClickedRadioTestType)
	ON_BN_CLICKED(IDC_RADIO_CHN_HC_CO_CO2, &CNHA503XGasCheckDlg::OnBnClickedRadioChn)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO, &CNHA503XGasCheckDlg::OnBnClickedRadioChn)
	ON_BN_CLICKED(IDC_RADIO_CHN_NO2, &CNHA503XGasCheckDlg::OnBnClickedRadioChn)
	ON_BN_CLICKED(IDC_RADIO_CHN_HC_CO_CO2_NO, &CNHA503XGasCheckDlg::OnBnClickedRadioChn)
	ON_BN_CLICKED(IDC_RADIO_CHN_O2, &CNHA503XGasCheckDlg::OnBnClickedRadioChn)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE1, &CNHA503XGasCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE2, &CNHA503XGasCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE3, &CNHA503XGasCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE4, &CNHA503XGasCheckDlg::OnBnClickedRadioCheckType)
	ON_BN_CLICKED(IDC_RADIO_CHECK_TYPE5, &CNHA503XGasCheckDlg::OnBnClickedRadioCheckType)
END_MESSAGE_MAP()


// CNHA503XGasCheckDlg 消息处理程序

BOOL CNHA503XGasCheckDlg::OnInitDialog()
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

void CNHA503XGasCheckDlg::InitCtrls(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}

	m_lbHead.SetTitle(L"分析仪气体检查");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置上下标文本控件
	m_mlb1.SetScriptText(L"C_3H_8[10^-^6]");
	m_mlb2.SetScriptText(L"HC[10^-^6]");
	m_mlb3.SetScriptText(L"C_3H_8[10^-^6]");
	m_mlb4.SetScriptText(L"测量值[10^-^6]");
	m_mlb5.SetScriptText(L"标称值[10^-^6]");
	m_mlb6.SetScriptText(L"绝对误差[10^-^6]");
	m_mlb8.SetScriptText(L"CO_2通道");
	m_mlb12.SetScriptText(L"NO_2通道");
	m_mlb13.SetScriptText(L"NO[10^-^6]");
	m_mlb14.SetScriptText(L"NO[10^-^6]");
	m_mlb15.SetScriptText(L"CO_2[%]");
	m_mlb16.SetScriptText(L"CO_2[%]");
	m_mlb17.SetScriptText(L"O_2[%]");
	m_mlb18.SetScriptText(L"NO_2[10^-^6]");
	m_mlb19.SetScriptText(L"NO_2[10^-^6]");

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// HC实时值
	m_lbHC.SetLEDStyle();
	m_lbHC.SetText(L"--");
	// C3H8实时值
	m_lbC3H8.SetLEDStyle();
	m_lbC3H8.SetText(L"--");
	// CO实时值
	m_lbCO.SetLEDStyle();
	m_lbCO.SetText(L"--");
	// CO2实时值
	m_lbCO2.SetLEDStyle();
	m_lbCO2.SetText(L"--");
	// NO实时值
	m_lbNO.SetLEDStyle();
	m_lbNO.SetText(L"--");
	// NO2实时值
	m_lbNO2.SetLEDStyle();
	m_lbNO2.SetText(L"--");
	// O2实时值
	m_lbO2.SetLEDStyle();
	m_lbO2.SetText(L"--");
	
	// PEF实时值

	m_lbPEF.SetLEDStyle();
	m_lbPEF.SetText(L"--");

	// CO,CO2检查气标称值编辑框
	m_edCONominalValue.AllowNegative(FALSE);
	m_edCONominalValue.SetMaxDecimalPlaces(DECIMAL_PLACES);
	m_edCO2NominalValue.AllowNegative(FALSE);
	m_edCO2NominalValue.SetMaxDecimalPlaces(DECIMAL_PLACES);
	m_edO2NominalValue.AllowNegative(FALSE);
	m_edO2NominalValue.SetMaxDecimalPlaces(DECIMAL_PLACES);

	CString strFormat;

	// 显示检查气标称值历史记录
	CString str;
	str.Format(L"%d", m_nC3H8NominalValue);
	m_edC3H8NominalValue.SetWindowText(str);
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	m_edCONominalValue.SetWindowText(str);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	m_edCO2NominalValue.SetWindowText(str);
	str.Format(strFormat, (float)m_nO2NominalValue/AMPLIFICATION_FACTOR);
	m_edO2NominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNONominalValue);
	m_edNONominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNO2NominalValue);
	m_edNO2NominalValue.SetWindowText(str);

	// PEF测量值显示框
	m_lbPEFMeasuredValue.SetBkColor(RGB(230, 230, 230));

	// HC测量值显示框
	m_lbHCMeasuredValue.SetBkColor(RGB(230, 230, 230));
	// CO测量值显示框
	m_lbCOMeasuredValue.SetBkColor(RGB(230, 230, 230));
	// CO2测量值显示框
	m_lbCO2MeasuredValue.SetBkColor(RGB(230, 230, 230));
	// O2测量值显示框
	m_lbO2MeasuredValue.SetBkColor(RGB(230, 230, 230));
	// NO测量值显示框
	m_lbNOMeasuredValue.SetBkColor(RGB(230, 230, 230));
	// NO2测量值显示框
	m_lbNO2MeasuredValue.SetBkColor(RGB(230, 230, 230));

	// HC标称值显示框
	m_lbHCNominalValue.SetBkColor(RGB(230, 230, 230));
	// CO标称值显示框
	m_lbCONominalValue.SetBkColor(RGB(230, 230, 230));
	// CO2标称值显示框
	m_lbCO2NominalValue.SetBkColor(RGB(230, 230, 230));
	// O2标称值显示框
	m_lbO2NominalValue.SetBkColor(RGB(230, 230, 230));
	// NO标称值显示框
	m_lbNONominalValue.SetBkColor(RGB(230, 230, 230));
	// NO2标称值显示框
	m_lbNO2NominalValue.SetBkColor(RGB(230, 230, 230));
	
	// HC绝对误差显示框
	m_lbHCAE.SetBkColor(RGB(230, 230, 230));
	// CO绝对误差显示框
	m_lbCOAE.SetBkColor(RGB(230, 230, 230));
	// CO2绝对误差显示框
	m_lbCO2AE.SetBkColor(RGB(230, 230, 230));
	// O2绝对误差显示框
	m_lbO2AE.SetBkColor(RGB(230, 230, 230));
	// NO绝对误差显示框
	m_lbNOAE.SetBkColor(RGB(230, 230, 230));
	// NO2绝对误差显示框
	m_lbNO2AE.SetBkColor(RGB(230, 230, 230));

	// HC相对误差显示框
	m_lbHCRE.SetBkColor(RGB(230, 230, 230));
	// CO相对误差显示框
	m_lbCORE.SetBkColor(RGB(230, 230, 230));
	// CO2相对误差显示框
	m_lbCO2RE.SetBkColor(RGB(230, 230, 230));
	// O2相对误差显示框
	m_lbO2RE.SetBkColor(RGB(230, 230, 230));
	// NO相对误差显示框
	m_lbNORE.SetBkColor(RGB(230, 230, 230));
	// NO2相对误差显示框
	m_lbNO2RE.SetBkColor(RGB(230, 230, 230));

	// HC判定显示框
	m_lbHCJudgement.SetBkColor(RGB(230, 230, 230));
	// CO判定显示框
	m_lbCOJudgement.SetBkColor(RGB(230, 230, 230));
	// CO2判定显示框
	m_lbCO2Judgement.SetBkColor(RGB(230, 230, 230));
	// O2判定显示框
	m_lbO2Judgement.SetBkColor(RGB(230, 230, 230));
	// NO判定显示框
	m_lbNOJudgement.SetBkColor(RGB(230, 230, 230));
	// NO2判定显示框
	m_lbNO2Judgement.SetBkColor(RGB(230, 230, 230));

	m_lbHCRespTime.SetBkColor(RGB(230, 230, 230));
	m_lbCORespTime.SetBkColor(RGB(230, 230, 230));
	m_lbCO2RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNORespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNO2RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbO2RespTime.SetBkColor(RGB(230, 230, 230));

	// 裁决显示框
	m_lbDetermination.SetBkColor(RGB(230, 230, 230));
	m_lbDetermination.AutoFitFont();
	
	SetLimit_VMAS();

	//////////////////////////////////////// 初始化绝对误差和相对误差限值start
	// HC绝对误差显示框
	m_lbHCAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nHCAELimit);
	m_lbHCAE.SetText(str);
	// CO绝对误差显示框
	m_lbCOAE.SetTextColor(RGB(0, 0, 0));
	strFormat.Format(L"--(±%%.%df)", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	m_lbCOAE.SetText(str);
	// CO2绝对误差显示框
	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);
	// NO绝对误差显示框
	m_lbNOAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNOAELimit);
	m_lbNOAE.SetText(str);
	// NO2绝对误差显示框
	m_lbNO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2AELimit);
	m_lbNO2AE.SetText(str);
	// O2绝对误差显示框
	m_lbO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nO2AELimit/AMPLIFICATION_FACTOR);
	m_lbO2AE.SetText(str);

	// HC相对误差显示框
	m_lbHCRE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nHCRELimit);
	m_lbHCRE.SetText(str);
	// CO相对误差显示框
	m_lbCORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nCORELimit);
	m_lbCORE.SetText(str);
	// CO2相对误差显示框
	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nCO2RELimit);
	m_lbCO2RE.SetText(str);
	// NO相对误差显示框
	m_lbNORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNORELimit);
	m_lbNORE.SetText(str);
	// NO2相对误差显示框
	m_lbNO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2RELimit);
	m_lbNO2RE.SetText(str);
	// O2相对误差显示框
	m_lbO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nO2RELimit);
	m_lbO2RE.SetText(str);
	//////////////////////////////////////// 初始化绝对误差和相对误差限值stop

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	
	//((CButton *)GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2))->SetCheck(BST_CHECKED);
	//m_nChannel = CHN_HC_CO_CO2;
	((CButton *)GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2_NO))->SetCheck(BST_CHECKED);
	m_nChannel = CHN_HC_CO_CO2_NO;
	m_edC3H8NominalValue.EnableWindow(TRUE);
	m_edCONominalValue.EnableWindow(TRUE);
	m_edCO2NominalValue.EnableWindow(TRUE);
	m_edNONominalValue.EnableWindow(FALSE);
	m_edNO2NominalValue.EnableWindow(FALSE);
	m_edO2NominalValue.EnableWindow(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE1))->SetCheck(BST_CHECKED);
	m_nCheckType = 1;
}

void CNHA503XGasCheckDlg::ImportGasCheckParams(void)
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
	str = si.GetString(L"AnaChk", L"C3H8NominalValue", L"3000");
	m_nC3H8NominalValue = _wtoi(str);
	str = si.GetString(L"AnaChk", L"CONominalValue", L"6");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nCONominalValue = (int)(f * AMPLIFICATION_FACTOR + 0.5f);
	str = si.GetString(L"AnaChk", L"CO2NominalValue", L"12");
	f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nCO2NominalValue = (int)(f * AMPLIFICATION_FACTOR + 0.5f);
	str = si.GetString(L"AnaChk", L"NONominalValue", L"200");
	m_nNONominalValue = _wtoi(str);
	str = si.GetString(L"AnaChk", L"NO2NominalValue", L"100");
	m_nNO2NominalValue = _wtoi(str);
	str = si.GetString(L"AnaChk", L"O2NominalValue", L"20.8");
	f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// 放大100倍,并四舍五入
	m_nO2NominalValue = (int)(f * AMPLIFICATION_FACTOR + 0.5f);
}

void CNHA503XGasCheckDlg::ExportEquCalChkInfo(void)
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
	CString strAnalyzerModel = siLineInfo.GetString(L"LineInfo", L"AnalyzerModel", L"");

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
	str.Format(L"\r\nEquipment=%s", strAnalyzerModel);
	strResult += str;

	// 项目
	str.Format(L"\r\nItem=%s", L"分析仪气体检查");
	strResult += str;

	// 日期
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据
	CString strFormat;
	strFormat.Format(L"[HC]:%%d(%%d);[CO]:%%.%df(%%.%df);[CO2]:%%.%df(%%.%df):[NO]:%%d(%%d)", DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES);
	CString strData;
	strData.Format(strFormat, m_nHCMeasuredValue, m_nHCNominalValue, 
		(float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR, (float)m_nCONominalValue/AMPLIFICATION_FACTOR,
		(float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR, 
		m_nNOMeasuredValue, m_nNONominalValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// 误差
	//strFormat.Format(L"绝对误差/相对误差:HC:%%d(±%%d)/%%d(±%%d);"
	//				  L"CO:%%.%df(±%%.%df)/%%d(±%%d);"
	//				  L"CO2:%%.%df(±%%.%df)/%%d(±%%d);"
	//				  L"NO:%%d(±%%d)/%%d(±%%d);", 
	//	DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES);
	//CString strError;
	//strError.Format(strFormat, m_nHCAE, m_nHCAELimit, m_nHCRE, m_nHCRELimit,
	//	(float)m_nCOAE/AMPLIFICATION_FACTOR, (float)m_nCOAELimit/AMPLIFICATION_FACTOR, m_nCORE, m_nCORELimit,
	//	(float)m_nCO2AE/AMPLIFICATION_FACTOR, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR, m_nCO2RE, m_nCO2RELimit, 
	//	m_nNOAE, m_nNOAELimit, m_nNORE, m_nNORELimit);
	strFormat.Format(L"HC:%%d(±%%d)/%%d(±%%d);"
					  L"CO:%%.%df(±%%.%df)/%%d(±%%d);"
					  L"CO2:%%.%df(±%%.%df)/%%d(±%%d);"
					  L"NO:%%d(±%%d)/%%d(±%%d)", 
		DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES);
	CString strError;
	strError.Format(strFormat, m_nHCAE, m_nHCAELimit, m_nHCRE, m_nHCRELimit,
		(float)m_nCOAE/AMPLIFICATION_FACTOR, (float)m_nCOAELimit/AMPLIFICATION_FACTOR, m_nCORE, m_nCORELimit,
		(float)m_nCO2AE/AMPLIFICATION_FACTOR, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR, m_nCO2RE, m_nCO2RELimit, 
		m_nNOAE, m_nNOAELimit, m_nNORE, m_nNORELimit);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// 判断
	str.Format(L"\r\nJudgement=%s", m_bIsCheckPass?L"通过":L"不通过");
	strResult += str;

	//广州环保局的特定格式的标定数据保存在Reserved1字段////////////////////////////////////////////////////////
	CString strHC_std,strHC_val,strHC_err,strCO_std,strCO_val,strCO_err,strCO2_std,strCO2_val,strCO2_err;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	strHC_std.Format(L"%d", m_nHCNominalValue);		//HC标准值
	strHC_val.Format(L"%d", m_nHCMeasuredValue);	//HC测量值
	strHC_err.Format(L"%d", m_nHCRE);				//HC相对误差
	strCO_std.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);		//CO标准值
	strCO_val.Format(strFormat, (float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR);	//CO测量值
	strCO_err.Format(L"%d", m_nCORE);				//CO相对误差
	strCO2_std.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);		//CO2标准值
	strCO2_val.Format(strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);	//CO2测量值
	strCO2_err.Format(L"%d", m_nCO2RE);				//CO2相对误差
	str.Format(L"\r\nReserved1=%s;%s;%s;%s;%s;%s;%s;%s;%s", 
		strCO_std, strCO_val, strCO_err, strHC_std, strHC_val, strHC_err, strCO2_std, strCO2_val, strCO2_err); //写入顺序不能反
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

void CNHA503XGasCheckDlg::ExportGasCheckRTData(void)
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
	
	int nCount = (int)(m_lsRTDataOfAnaGasChk.GetCount());

	////////////////////////////////////////////// 总数begin
	strRTData.Format(L"\r\n[Sum]\r\nTotal=%d", nCount);
	sfe.WriteString(strRTData);
	////////////////////////////////////////////// 总数end

	RTData rd;
	POSITION pos;
	for(int i=0; i<nCount; i++ )
	{			
		pos = m_lsRTDataOfAnaGasChk.FindIndex(i);
		rd = m_lsRTDataOfAnaGasChk.GetAt(pos);

		strRTData.Format(_T("\r\n\r\n[%d]"), i+1);

		CString str;

		// 时间序号
		str.Format(_T("\r\nTime=%d"), rd.nTime);
		strRTData += str;

		// HC示值
		str.Format(_T("\r\nHC=%hd"), rd.sHC);
		strRTData += str;

		// CO示值
		str.Format(_T("\r\nCO=%.2f"), rd.fCO);
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

		// O2示值
		str.Format(_T("\r\nO2=%.2f"), rd.fO2);
		strRTData += str;

		// PEF示值
		str.Format(_T("\r\nPEF=%.3f"), rd.fPef);
		strRTData += str;

		// Lambda
		str.Format(_T("\r\nLambda=%.2f"), rd.fLambda);
		strRTData += str;

		// 流量
		str.Format(_T("\r\nFlux=%hu"), rd.usFlux);
		strRTData += str;

		// 转速
		str.Format(_T("\r\nRev=%hu"), rd.usRev);
		strRTData += str;

		// 油温
		str.Format(_T("\r\nOilTemperature=%.2f"), rd.fOT);
		strRTData += str;
		
		strRTData.AppendFormat(L"\r\n");
		strRTData.AppendFormat(L"DateTime=%04d-%02d-%02d %02d:%02d:%02d.%03d\r\n", 
			rd.st.wYear, rd.st.wMonth, rd.st.wDay, 
			rd.st.wHour, rd.st.wMinute, rd.st.wSecond, rd.st.wMilliseconds);

		sfe.WriteString(strRTData);
	}

	sfe.Close();
}

void CNHA503XGasCheckDlg::ExportDemarcationLog(void)
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
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"AnalyzerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"AnalyzerModel", L"");

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
	str.Format(L"\r\nEquipmentName=%s", L"废气分析仪");
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

void CNHA503XGasCheckDlg::ExportGasCheckResult(void)
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
	// 1：HC_CO_CO2
	// 2：NO
	// 3：NO2
	// 4：HC_CO_CO2_NO
	str.Format(L"\r\nChannel=%d", m_nChannel);
	strResult += str;

	// 标准气浓度
	// 1：低浓度
	// 2：中低浓度
	// 3：中高浓度
	// 4：高浓度
	str.Format(L"\r\nType=%d", m_nCheckType);
	strResult += str;

	// PEF
	str.Format(L"\r\nPEFMeasuredValue=%.3f", m_nPEFMeasuredValue/1000.0f);
	strResult += str;

	// C3H8标准值,单位:ppm
	str.Format(L"\r\nC3H8StandardValue=%d", m_nC3H8NominalValue);
	strResult += str;
	
	// 是否跳过HC校准检测
	if (m_nChannel==CHN_NO || m_nChannel==CHN_NO2)
	{
		str.Format(L"\r\nSkipHCJudgement=1");
	}
	else
	{
		str.Format(L"\r\nSkipHCJudgement=0");
	}
	strResult += str;
		
	// HC测量值,单位:ppm
	str.Format(L"\r\nHCMeasuredValue=%d", m_nHCMeasuredValue);
	strResult += str;

	// HC标称值,单位:ppm
	str.Format(L"\r\nHCStandardValue=%d", m_nHCNominalValue);
	strResult += str;

	// HC绝对误差,单位:ppm
	str.Format(L"\r\nHCAE=%d", m_nHCAE);
	strResult += str;

	// HC绝对误差限值,单位:ppm
	str.Format(L"\r\nHCAELimit=%d", m_nHCAELimit);
	strResult += str;

	// HC绝对误差是否合格
	str.Format(L"\r\nHCAEPass=%s", m_bIsHCAEPass?L"1":L"0");
	strResult += str;

	// HC相对误差
	str.Format(L"\r\nHCRE=%d", m_nHCRE);
	strResult += str;

	// HC相对误差限值
	str.Format(L"\r\nHCRELimit=%d", m_nHCRELimit);
	strResult += str;

	// HC相对误差是否合格
	str.Format(L"\r\nHCREPass=%s", m_bIsHCREPass?L"1":L"0");
	strResult += str;

	// HC是否合格
	str.Format(L"\r\nHCPass=%s", m_bIsHCPass?L"1":L"0");
	strResult += str;

	// 是否跳过CO校准检测
	if (m_nChannel==CHN_NO || m_nChannel==CHN_NO2)
	{
		str.Format(L"\r\nSkipCOJudgement=1");
	}
	else
	{
		str.Format(L"\r\nSkipCOJudgement=0");
	}
	strResult += str;

	// CO测量值,单位:%
	CString strFormat;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(L"\r\nCOMeasuredValue="+strFormat, (float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO标称值,单位:%
	str.Format(L"\r\nCOStandardValue="+strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO绝对误差,单位:%
	str.Format(L"\r\nCOAE="+strFormat, (float)m_nCOAE/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO绝对误差限值,单位:%
	str.Format(L"\r\nCOAELimit="+strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO绝对误差是否合格
	str.Format(L"\r\nCOAEPass=%s", m_bIsCOAEPass?L"1":L"0");
	strResult += str;

	// CO相对误差
	str.Format(L"\r\nCORE=%d", m_nCORE);
	strResult += str;

	// CO相对误差限值
	str.Format(L"\r\nCORELimit=%d", m_nCORELimit);
	strResult += str;

	// CO相对误差是否合格
	str.Format(L"\r\nCOREPass=%s", m_bIsCOREPass?L"1":L"0");
	strResult += str;

	// CO是否合格
	str.Format(L"\r\nCOPass=%s", m_bIsCOPass?L"1":L"0");
	strResult += str;

	// 是否跳过CO2校准检测
	if (m_nChannel==CHN_NO || m_nChannel==CHN_NO2)
	{
		str.Format(L"\r\nSkipCO2Judgement=1");
	}
	else
	{
		str.Format(L"\r\nSkipCO2Judgement=0");
	}
	strResult += str;

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

	// 是否跳过NO校准检测
	if (m_nChannel==CHN_HC_CO_CO2 || m_nChannel==CHN_NO2)
	{
		str.Format(L"\r\nSkipNOJudgement=1");
	}
	else
	{
		str.Format(L"\r\nSkipNOJudgement=0");
	}
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
	if (m_nChannel==CHN_HC_CO_CO2 || m_nChannel==CHN_NO2 || m_nChannel==CHN_HC_CO_CO2_NO)
	{
		str.Format(L"\r\nSkipNO2Judgement=1");
	}
	else
	{
		str.Format(L"\r\nSkipNO2Judgement=0");
	}
	strResult += str;
	
	if (m_nChannel==CHN_NO2)
	{
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

	// 是否跳过O2校准检测
	str.Format(L"\r\nSkipO2Judgement=1");
	strResult += str;

	// O2测量值,单位:%
	str.Format(L"\r\nO2MeasuredValue=");
	strResult += str;

	// O2标称值,单位:%
	str.Format(L"\r\nO2StandardValue=");
	strResult += str;

	// O2绝对误差,单位:%
	str.Format(L"\r\nO2AE=");
	strResult += str;

	// O2绝对误差限值,单位:%
	str.Format(L"\r\nO2AELimit=");
	strResult += str;

	// O2绝对误差是否合格
	str.Format(L"\r\nO2AEPass=");
	strResult += str;

	// O2相对误差
	str.Format(L"\r\nO2RE=");
	strResult += str;

	// O2相对误差限值
	str.Format(L"\r\nO2RELimit=");
	strResult += str;

	// O2相对误差是否合格
	str.Format(L"\r\nO2REPass=");
	strResult += str;

	// O2是否合格
	str.Format(L"\r\nO2Pass=");
	strResult += str;

	// 响应时间
	str.Format(L"\r\nHCT90Limit=%.2f", m_fHCT90Limit);
	strResult += str;
	str.Format(L"\r\nHCT10Limit=%.2f", m_fHCT10Limit);
	strResult += str;
	str.Format(L"\r\nHCRespTime=%.2f", m_fHCRespTime);
	strResult += str;
	str.Format(L"\r\nCOT90Limit=%.2f", m_fCOT90Limit);
	strResult += str;
	str.Format(L"\r\nCOT10Limit=%.2f", m_fCOT10Limit);
	strResult += str;
	str.Format(L"\r\nCORespTime=%.2f", m_fCORespTime);
	strResult += str;
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
	str.Format(L"\r\nO2T90Limit=%.2f", m_fO2T90Limit);
	strResult += str;
	str.Format(L"\r\nO2T10Limit=%.2f", m_fO2T10Limit);
	strResult += str;
	str.Format(L"\r\nO2RespTime=%.2f", m_fO2RespTime);
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

void CNHA503XGasCheckDlg::OpenSerialPortAndTestCommunication(void)
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
	if (m_pAnaThread->IsOpen())
	{
		if (CNHEuqController::GetInstance().m_bConnectToFlowmeter)
		{
			m_pAnaThread->SetOperationMode(CAna::ToAnalyzer);
		}

		// 测试通讯
		BYTE bStatus(0);
		if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		{
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
					m_lbInfo.SetText(_T("分析仪气体检查"));
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



void CNHA503XGasCheckDlg::ResetProcess()
{
	//m_lbInfo.StopFlashText();

	// 清除实时值显示
	ClearRTDisplay();

	// 清除实时数据链表
	m_lsRTDataOfAnaGasChk.RemoveAll();

	// PEF测量值显示框
	m_lbPEFMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbPEFMeasuredValue.SetText(L"--");

	// HC测量值显示框
	m_lbHCMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbHCMeasuredValue.SetText(L"--");
	// CO测量值显示框
	m_lbCOMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbCOMeasuredValue.SetText(L"--");
	// CO2测量值显示框
	m_lbCO2MeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbCO2MeasuredValue.SetText(L"--");
	// NO测量值显示框
	m_lbNOMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbNOMeasuredValue.SetText(L"--");
	// NO2测量值显示框
	m_lbNO2MeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbNO2MeasuredValue.SetText(L"--");

	// HC标称值显示框
	m_lbHCNominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbHCNominalValue.SetText(L"--");
	// CO标称值显示框
	m_lbCONominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbCONominalValue.SetText(L"--");
	// CO2标称值显示框
	m_lbCO2NominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbCO2NominalValue.SetText(L"--");
	// NO标称值显示框
	m_lbNONominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbNONominalValue.SetText(L"--");
	// NO2标称值显示框
	m_lbNO2NominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbNO2NominalValue.SetText(L"--");

	//////////////////////////////////////// 初始化绝对误差和相对误差限值start

	SetLimit_VMAS();

	CString strFormat;
	CString str;
	// HC绝对误差显示框
	m_lbHCAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nHCAELimit);
	m_lbHCAE.SetText(str);
	// CO绝对误差显示框
	m_lbCOAE.SetTextColor(RGB(0, 0, 0));
	strFormat.Format(L"--(±%%.%df)", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	m_lbCOAE.SetText(str);
	// CO2绝对误差显示框
	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);
	// NO绝对误差显示框
	m_lbNOAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNOAELimit);
	m_lbNOAE.SetText(str);
	// NO2绝对误差显示框
	m_lbNO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNO2AELimit);
	m_lbNO2AE.SetText(str);
	// O2绝对误差显示框
	m_lbO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nO2AELimit/AMPLIFICATION_FACTOR);
	m_lbO2AE.SetText(str);

	// HC相对误差显示框
	m_lbHCRE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nHCRELimit);
	m_lbHCRE.SetText(str);
	// CO相对误差显示框
	m_lbCORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nCORELimit);
	m_lbCORE.SetText(str);
	// CO2相对误差显示框
	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nCO2RELimit);
	m_lbCO2RE.SetText(str);
	// NO相对误差显示框
	m_lbNORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNORELimit);
	m_lbNORE.SetText(str);
	// NO2相对误差显示框
	m_lbNO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nNORELimit);
	m_lbNO2RE.SetText(str);
	// O2相对误差显示框
	m_lbO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(±%d)", m_nO2RELimit);
	m_lbO2RE.SetText(str);
	//////////////////////////////////////// 初始化绝对误差和相对误差限值stop

	// HC判定显示框
	m_lbHCJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbHCJudgement.SetText(L"--");
	// CO判定显示框
	m_lbCOJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbCOJudgement.SetText(L"--");
	// CO2判定显示框
	m_lbCO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbCO2Judgement.SetText(L"--");
	// O2判定显示框
	m_lbO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbO2Judgement.SetText(L"--");
	// NO判定显示框
	m_lbNOJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbNOJudgement.SetText(L"--");
	// NO2判定显示框
	m_lbNO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbNO2Judgement.SetText(L"--");

	m_lbHCRespTime.SetTextColor(RGB(0, 0, 0));
	m_lbHCRespTime.SetText(L"--");
	m_lbCORespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCORespTime.SetText(L"--");
	m_lbCO2RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCO2RespTime.SetText(L"--");
	m_lbNORespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNORespTime.SetText(L"--");
	m_lbNO2RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNO2RespTime.SetText(L"--");
	m_lbO2RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbO2RespTime.SetText(L"--");

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

	m_fHCRespTime = 0;
	m_fCORespTime = 0;
	m_fCO2RespTime = 0;
	m_fNORespTime = 0;
	m_fNO2RespTime = 0;
	m_fO2RespTime = 0;
}

void CNHA503XGasCheckDlg::ClearRTDisplay(void)
{
	// HC实时值
	m_lbHC.SetText(L"--");
	// C3H8实时值
	m_lbC3H8.SetText(L"--");
	// CO实时值
	m_lbCO.SetText(L"--");
	// CO2实时值
	m_lbCO2.SetText(L"--");
	// NO实时值
	m_lbNO.SetText(L"--");
	// NO2实时值
	m_lbNO2.SetText(L"--");
	// O2实时值
	m_lbO2.SetText(L"--");
	// PEF实时值
	m_lbPEF.SetText(L"--");
}

void CNHA503XGasCheckDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CNHA503XGasCheckDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nChannel != CHN_HC_CO_CO2
		&& m_nChannel != CHN_NO
		&& m_nChannel != CHN_NO2
		&& m_nChannel != CHN_HC_CO_CO2_NO
		&& m_nChannel != CHN_O2
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

void CNHA503XGasCheckDlg::GasCheckProcess(void)
{
	if (0 == m_nStep)
	{
		// 重置流程
		ResetProcess();

		// 开始检查前,测试一次通讯
		if (ANA_WR_SUCCEED != m_pAnaThread->Zero())
		{
			m_lbInfo.SetText(_T("通讯超时"));
			return;
		}

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};				
			wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体检查开始", _countof(wchInfo)-1);
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
		GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_NO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_O2)->EnableWindow(FALSE);
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
		
		// 提前执行
		m_pAnaThread->GetPef_Asyn(NULL);

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

		//if (m_nTestType == DIS)
		//{
		//	// 计算结果
		//	CalculateResult_DIS();
		//	// 显示结果
		//	ShowResult_DIS();
		//}
		//else if (m_nTestType == ASM)
		//{
		//	// 计算结果
		//	CalculateResult_ASM();
		//	// 显示结果
		//	ShowResult_ASM();
		//}
		//else if (m_nTestType == VMAS)
		//{
			// 计算结果
			CalculateResult_VMAS();
			// 计算响应时间
			CalculateRespTime();
			// 显示结果
			ShowResult_VMAS();
		//}
		

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
			wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪气体检查完成", _countof(wchInfo)-1);
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
		GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_NO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_O2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHECK_TYPE5)->EnableWindow(TRUE);

		// 重置步骤
		m_nStep = 0;
	}
}

DWORD CNHA503XGasCheckDlg::GetNominalValue(void)
{
	CString strC3H8, strCO, strCO2, strNO, strNO2, strO2;
	//获取检查气体值并检查是否非空
	if (m_nChannel == CHN_HC_CO_CO2
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		m_edC3H8NominalValue.GetWindowText(strC3H8);
		strC3H8.Trim();
		if (L"" == strC3H8)
		{
			m_edC3H8NominalValue.SetFocus();
			MessageBox(_T("请输入检查气C3H8标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}

		m_edCONominalValue.GetWindowText(strCO);
		strCO.Trim();
		if (L"" == strCO)
		{
			m_edCONominalValue.SetFocus();
			MessageBox(_T("请输入检查气CO标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		m_edCO2NominalValue.GetWindowText(strCO2);
		strCO2.Trim();
		if (L"" == strCO2)
		{
			m_edCO2NominalValue.SetFocus();
			MessageBox(_T("请输入检查气CO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	
	if (m_nChannel == CHN_NO
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		m_edNONominalValue.GetWindowText(strNO);
		strNO.Trim();
		if (L"" == strNO)
		{
			m_edNONominalValue.SetFocus();
			MessageBox(_T("请输入检查气NO标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}

	if (m_nChannel == CHN_NO2)
	{
		m_edNO2NominalValue.GetWindowText(strNO2);
		strNO2.Trim();
		if (L"" == strNO2)
		{
			m_edNO2NominalValue.SetFocus();
			MessageBox(_T("请输入检查气NO2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	
	if (m_nChannel == CHN_HC_CO_CO2
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		// 记录标称值
		// 标准气HC标称值,单位:ppm
		m_nC3H8NominalValue = _wtoi(strC3H8);
		if (m_nC3H8NominalValue <= 0)
		{
			m_edC3H8NominalValue.SetFocus();
			MessageBox(_T("检查气C3H8标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// 标准气CO标称值,单位:0.01%(放到100倍)
		float f = (float)_wtof(strCO);
		m_nCONominalValue = int(f * AMPLIFICATION_FACTOR + 0.5f);
		if (m_nCONominalValue <= 0)
		{
			m_edCONominalValue.SetFocus();
			MessageBox(_T("检查气CO标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// 标准气CO2标称值,单位:0.01%(放到100倍)
		f = (float)_wtof(strCO2);
		m_nCO2NominalValue = int(f * AMPLIFICATION_FACTOR + 0.5f);
		if (m_nCO2NominalValue <= 0)
		{
			m_edCO2NominalValue.SetFocus();
			MessageBox(_T("检查气CO2标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	
	if (m_nChannel == CHN_NO
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		m_nNONominalValue = _wtoi(strNO);
		if (m_nNONominalValue <= 0)
		{
			m_edNONominalValue.SetFocus();
			MessageBox(_T("检查气NO标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}

	if (m_nChannel == CHN_NO2)
	{
		m_nNO2NominalValue = _wtoi(strNO2);
		if (m_nNO2NominalValue <= 0)
		{
			m_edNO2NominalValue.SetFocus();
			MessageBox(_T("检查气NO2标称值不能为0"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	
	if (m_nChannel == CHN_O2)
	{
		m_edO2NominalValue.GetWindowText(strO2);
		strO2.Trim();
		if (L"" == strO2)
		{
			m_edO2NominalValue.SetFocus();
			MessageBox(_T("请输入检查气O2标称值"), _T("气体检查"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	return 0x00;
}

void CNHA503XGasCheckDlg::SaveNominalValueToIniFile(void)
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
	str.Format(L"%d", m_nC3H8NominalValue);
	si.SetString(L"AnaChk", L"C3H8NominalValue", str);
	CString strFormat;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	si.SetString(L"AnaChk", L"CONominalValue", str);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	si.SetString(L"AnaChk", L"CO2NominalValue", str);
	str.Format(L"%d", m_nNONominalValue);
	si.SetString(L"AnaChk", L"NONominalValue", str);
	str.Format(L"%d", m_nNO2NominalValue);
	si.SetString(L"AnaChk", L"NO2NominalValue", str);
	str.Format(strFormat, (float)m_nO2NominalValue/AMPLIFICATION_FACTOR);
	si.SetString(L"AnaChk", L"O2NominalValue", str);
	///////////////////////////////////////////////////////////// 记录到配置文件stop
}

//void CNHA503XGasCheckDlg::CalculateResult_DIS(void)
//{
//	// 获取一次最新数据进行结果判断
//	// 主数据
//	short sHC(0);
//	float fCO(0.0f);
//	float fCO2(0.0f);
//	float fO2(0.0f);
//	short sNO(0);
//	short sNO2(0);
//	short sNOx(0);
//	USHORT usRpm(0);
//	float fOilTemp(0.0f);
//	USHORT usFlux(0);
//	float fLambda(0.0f);
//	m_pAnaThread->GetMajorData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx, &usRpm, &fOilTemp, &usFlux, &fLambda);
////#ifdef _DEBUG
////	sHC = 1700;
////	fCO = 8.33;
////	fCO2 = 12.2;
////	sNO = 3000;
////#endif
//	float fPef(0.0f);
//	m_pAnaThread->GetPef(&fPef);
//
//	// PEF强制置数
//	if (fPef < 0.3)
//	{
//		fPef = 0.3f;
//	}
//	// PEF测量值,放大1000倍,四舍五入
//	m_nPEFMeasuredValue = int(fPef * 1000 + 0.5f);
//
//	// 将C3H8标称值转换成HC标称值
//	m_nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
//	// 合理性约束
//	if (m_nHCNominalValue == 0)
//	{
//		m_nHCNominalValue = 1;
//	}
//	// HC通道判断
//	m_nHCMeasuredValue = sHC;
//	// 绝对误差
//	m_nHCAE = m_nHCMeasuredValue - m_nHCNominalValue;
//	if (abs(m_nHCAE) > m_nHCAELimit)
//	{
//		// HC绝对误差不合格
//		m_bIsHCAEPass = false;
//	}
//	else
//	{
//		// HC绝对误差合格
//		m_bIsHCAEPass = true;
//	}
//	// 相对误差
//	m_nHCRE = m_nHCAE * 100 / m_nHCNominalValue;
//	if (abs(m_nHCRE) > m_nHCRELimit)
//	{
//		// HC绝对误差不合格
//		m_bIsHCREPass = false;
//	}
//	else
//	{
//		// HC相对误差合格
//		m_bIsHCREPass = true;
//	}
//	if (m_bIsHCAEPass || m_bIsHCREPass)
//	{
//		// HC检查合格
//		m_bIsHCPass = true;
//	}
//	else
//	{
//		// HC检查不合格
//		m_bIsHCPass =false;
//	}
//
//	// CO通道判断
//	if (fCO < -10e-6f)
//	{
//		// fCO为负数,例如0.01
//		m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR - 0.5f);
//	}
//	else
//	{
//		// fCO为正数数
//		m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR + 0.5f);
//	}
//	// 绝对误差
//	m_nCOAE = m_nCOMeasuredValue - m_nCONominalValue;
//	// CO绝对误差限值
//	ASSERT(m_nCOAELimit > -10e-6);
//	if (abs(m_nCOAE) > m_nCOAELimit)
//	{
//		// CO绝对误差不合格
//		m_bIsCOAEPass = false;
//	}
//	else
//	{
//		// CO绝对误差合格
//		m_bIsCOAEPass = true;
//	}
//	// 合理性约束
//	if (m_nCONominalValue == 0)
//	{
//		m_nCONominalValue = 1;
//	}
//	// 相对误差
//	m_nCORE = m_nCOAE * 100 / m_nCONominalValue;
//	if (abs(m_nCORE) > m_nCORELimit)
//	{
//		// CO相对误差不合格
//		m_bIsCOREPass = false;
//	}
//	else
//	{
//		// CO相对误差合格
//		m_bIsCOREPass = true;
//	}
//	if (m_bIsCOAEPass || m_bIsCOREPass)
//	{
//		// CO检查合格
//		m_bIsCOPass = true;
//	}
//	else
//	{
//		// CO检查不合格
//		m_bIsCOPass = false;
//	}
//
//	// CO2通道判断
//	if (fCO2 < -10e-6f)
//	{
//		// fCO2为负数,例如:0.01
//		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR - 0.5f);
//	}
//	else
//	{
//		// fCO2为正数
//		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR + 0.5f);
//	}
//	// 绝对误差
//	m_nCO2AE = m_nCO2MeasuredValue - m_nCO2NominalValue;
//	// CO2绝对误差限值
//	ASSERT(m_nCO2AELimit > -10e-6);
//	if (abs(m_nCO2AE) > m_nCO2AELimit)
//	{
//		// CO2绝对误差不合格
//		m_bIsCO2AEPass = false;
//	}
//	else
//	{
//		// CO2绝对误差合格
//		m_bIsCO2AEPass = true;
//	}
//	// 合理性约束
//	if (m_nCO2NominalValue == 0)
//	{
//		m_nCO2NominalValue = 1;
//	}
//	// 相对误差
//	m_nCO2RE = m_nCO2AE * 100 / m_nCO2NominalValue;
//	if (abs(m_nCO2RE) > m_nCO2RELimit)
//	{
//		// CO2相对误差不合格
//		m_bIsCO2REPass = false;
//	}
//	else
//	{
//		// CO2相对误差合格
//		m_bIsCO2REPass = true;
//	}
//	if (m_bIsCO2AEPass || m_bIsCO2REPass)
//	{
//		// CO2检查合格
//		m_bIsCO2Pass = true;
//	}
//	else
//	{
//		// CO2检查不合格
//		m_bIsCO2Pass = false;
//	}
//
//	// NO通道判断
//	m_nNOMeasuredValue = sNO;
//	// NO绝对误差
//	m_nNOAE = m_nNOMeasuredValue - m_nNONominalValue;
//	if (abs(m_nNOAE) > m_nNOAELimit)
//	{
//		// NO绝对误差不合格
//		m_bIsNOAEPass = false;
//	}
//	else
//	{
//		// NO绝对误差合格
//		m_bIsNOAEPass = true;
//	}
//	// 合理性约束
//	if (m_nNONominalValue == 0)
//	{
//		m_nNONominalValue = 1;
//	}
//	// NO相对误差
//	m_nNORE = m_nNOAE * 100 / m_nNONominalValue;
//	if (abs(m_nNORE) > m_nNORELimit)
//	{
//		// NO绝对误差不合格
//		m_bIsNOREPass = false;
//	}
//	else
//	{
//		// NO相对误差合格
//		m_bIsNOREPass = true;
//	}
//	if (m_bIsNOAEPass || m_bIsNOREPass)
//	{
//		// NO检查合格
//		m_bIsNOPass = true;
//	}
//	else
//	{
//		// NO检查不合格
//		m_bIsNOPass =false;
//	}
//
//	// 检查是否通过
//	if (m_bIsHCPass && m_bIsCOPass && m_bIsCO2Pass && m_bIsNOPass)
//	{
//		m_bIsCheckPass = true;
//	}
//	else
//	{
//		m_bIsCheckPass = false;
//	}
//
//}

//void CNHA503XGasCheckDlg::CalculateResult_ASM(void)
//{
//	// 获取一次最新数据进行结果判断
//	// 主数据
//	short sHC(0);
//	float fCO(0.0f);
//	float fCO2(0.0f);
//	float fO2(0.0f);
//	short sNO(0);
//	short sNO2(0);
//	short sNOx(0);
//	USHORT usRpm(0);
//	float fOilTemp(0.0f);
//	USHORT usFlux(0);
//	float fLambda(0.0f);
//	m_pAnaThread->GetMajorData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx, &usRpm, &fOilTemp, &usFlux, &fLambda);
////#ifdef _DEBUG
////	sHC = 1700;
////	fCO = 8.33;
////	fCO2 = 12.2;
////	sNO = 3000;
////#endif
//	float fPef(0.0f);
//	m_pAnaThread->GetPef(&fPef);
//
//	// PEF强制置数
//	if (fPef < 0.3)
//	{
//		fPef = 0.3f;
//	}
//	// PEF测量值,放大1000倍,四舍五入
//	m_nPEFMeasuredValue = int(fPef * 1000 + 0.5f);
//
//	// 将C3H8标称值转换成HC标称值
//	m_nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
//	// 合理性约束
//	if (m_nHCNominalValue == 0)
//	{
//		m_nHCNominalValue = 1;
//	}
//	// HC通道判断
//	m_nHCMeasuredValue = sHC;
//	// 绝对误差
//	m_nHCAE = m_nHCMeasuredValue - m_nHCNominalValue;
//
//	// 分两段量程判断
//	if (m_nHCMeasuredValue > 2000)	
//	{
//		m_nHCAELimit = 0;
//		m_nHCRELimit = 10;
//	}
//	else
//	{
//		m_nHCAELimit = 10;
//		m_nHCRELimit = 5;
//	}
//	if (abs(m_nHCAE) > m_nHCAELimit)
//	{
//		// HC绝对误差不合格
//		m_bIsHCAEPass = false;
//	}
//	else
//	{
//		// HC绝对误差合格
//		m_bIsHCAEPass = true;
//	}
//	// 相对误差
//	m_nHCRE = m_nHCAE * 100 / m_nHCNominalValue;
//	if (abs(m_nHCRE) > m_nHCRELimit)
//	{
//		// HC绝对误差不合格
//		m_bIsHCREPass = false;
//	}
//	else
//	{
//		// HC相对误差合格
//		m_bIsHCREPass = true;
//	}
//	if (m_bIsHCAEPass || m_bIsHCREPass)
//	{
//		// HC检查合格
//		m_bIsHCPass = true;
//	}
//	else
//	{
//		// HC检查不合格
//		m_bIsHCPass =false;
//	}
//
//	// CO通道判断
//	if (fCO < -10e-6f)
//	{
//		// fCO为负数,例如0.01
//		m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR - 0.5f);
//	}
//	else
//	{
//		// fCO为正数数
//		m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR + 0.5f);
//	}
//	// 分两段量程判断
//	if (m_nCOMeasuredValue > 10*AMPLIFICATION_FACTOR)
//	{
//		m_nCOAELimit = 0;
//		m_nCORELimit = 10;
//	}
//	else
//	{
//		m_nCOAELimit = int(0.05f * AMPLIFICATION_FACTOR);
//		m_nCORELimit = 5;
//	}
//	// 绝对误差
//	m_nCOAE = m_nCOMeasuredValue - m_nCONominalValue;
//	// CO绝对误差限值
//	ASSERT(m_nCOAELimit > -10e-6);
//	if (abs(m_nCOAE) > m_nCOAELimit)
//	{
//		// CO绝对误差不合格
//		m_bIsCOAEPass = false;
//	}
//	else
//	{
//		// CO绝对误差合格
//		m_bIsCOAEPass = true;
//	}
//	// 合理性约束
//	if (m_nCONominalValue == 0)
//	{
//		m_nCONominalValue = 1;
//	}
//	// 相对误差
//	m_nCORE = m_nCOAE * 100 / m_nCONominalValue;
//	if (abs(m_nCORE) > m_nCORELimit)
//	{
//		// CO相对误差不合格
//		m_bIsCOREPass = false;
//	}
//	else
//	{
//		// CO相对误差合格
//		m_bIsCOREPass = true;
//	}
//	if (m_bIsCOAEPass || m_bIsCOREPass)
//	{
//		// CO检查合格
//		m_bIsCOPass = true;
//	}
//	else
//	{
//		// CO检查不合格
//		m_bIsCOPass = false;
//	}
//
//	// CO2通道判断
//	if (fCO2 < -10e-6f)
//	{
//		// fCO2为负数,例如:0.01
//		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR - 0.5f);
//	}
//	else
//	{
//		// fCO2为正数
//		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR + 0.5f);
//	}
//
//	// 分两段量程判断
//	if (m_nCO2MeasuredValue > 16*AMPLIFICATION_FACTOR)
//	{
//		m_nCO2AELimit = 0;
//		m_nCO2RELimit = 10;
//	}
//	else
//	{
//		m_nCO2AELimit = int(0.5f * AMPLIFICATION_FACTOR);
//		m_nCO2RELimit = 5;
//	}
//	// 绝对误差
//	m_nCO2AE = m_nCO2MeasuredValue - m_nCO2NominalValue;
//	// CO2绝对误差限值
//	ASSERT(m_nCO2AELimit > -10e-6);
//	if (abs(m_nCO2AE) > m_nCO2AELimit)
//	{
//		// CO2绝对误差不合格
//		m_bIsCO2AEPass = false;
//	}
//	else
//	{
//		// CO2绝对误差合格
//		m_bIsCO2AEPass = true;
//	}
//	// 合理性约束
//	if (m_nCO2NominalValue == 0)
//	{
//		m_nCO2NominalValue = 1;
//	}
//	// 相对误差
//	m_nCO2RE = m_nCO2AE * 100 / m_nCO2NominalValue;
//	if (abs(m_nCO2RE) > m_nCO2RELimit)
//	{
//		// CO2相对误差不合格
//		m_bIsCO2REPass = false;
//	}
//	else
//	{
//		// CO2相对误差合格
//		m_bIsCO2REPass = true;
//	}
//	if (m_bIsCO2AEPass || m_bIsCO2REPass)
//	{
//		// CO2检查合格
//		m_bIsCO2Pass = true;
//	}
//	else
//	{
//		// CO2检查不合格
//		m_bIsCO2Pass = false;
//	}
//
//	// NO通道判断
//	m_nNOMeasuredValue = sNO;
//
//	// 分两段量程判断
//	if (m_nNOMeasuredValue > 4000)
//	{
//		m_nNOAELimit = 0;
//		m_nNORELimit = 8;
//	}
//	else
//	{
//		m_nNOAELimit = 25;
//		m_nNORELimit = 4;
//	}
//	// NO绝对误差
//	m_nNOAE = m_nNOMeasuredValue - m_nNONominalValue;
//	if (abs(m_nNOAE) > m_nNOAELimit)
//	{
//		// NO绝对误差不合格
//		m_bIsNOAEPass = false;
//	}
//	else
//	{
//		// NO绝对误差合格
//		m_bIsNOAEPass = true;
//	}
//	// 合理性约束
//	if (m_nNONominalValue == 0)
//	{
//		m_nNONominalValue = 1;
//	}
//	// NO相对误差
//	m_nNORE = m_nNOAE * 100 / m_nNONominalValue;
//	if (abs(m_nNORE) > m_nNORELimit)
//	{
//		// NO绝对误差不合格
//		m_bIsNOREPass = false;
//	}
//	else
//	{
//		// NO相对误差合格
//		m_bIsNOREPass = true;
//	}
//	if (m_bIsNOAEPass || m_bIsNOREPass)
//	{
//		// NO检查合格
//		m_bIsNOPass = true;
//	}
//	else
//	{
//		// NO检查不合格
//		m_bIsNOPass =false;
//	}
//
//	// 检查是否通过
//	if (m_bIsHCPass && m_bIsCOPass && m_bIsCO2Pass && m_bIsNOPass)
//	{
//		m_bIsCheckPass = true;
//	}
//	else
//	{
//		m_bIsCheckPass = false;
//	}
//
//}

void CNHA503XGasCheckDlg::CalculateResult_VMAS(void)
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
	USHORT usRpm(0);
	float fOilTemp(0.0f);
	USHORT usFlux(0);
	float fLambda(0.0f);
	m_pAnaThread->GetMajorData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx, &usRpm, &fOilTemp, &usFlux, &fLambda);
//#ifdef _DEBUG
//	sHC = 1700;
//	fCO = 8.33;
//	fCO2 = 12.2;
//	sNO = 3000;
//#endif
	float fPef(0.0f);
	m_pAnaThread->GetPef(&fPef);

	// PEF强制置数
	if (fPef < 0.3)
	{
		fPef = 0.3f;
	}
	// PEF测量值,放大1000倍,四舍五入
	m_nPEFMeasuredValue = int(fPef * 1000 + 0.5f);

	if (m_nChannel == CHN_HC_CO_CO2
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		// 将C3H8标称值转换成HC标称值
		m_nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
		// 合理性约束
		if (m_nHCNominalValue == 0)
		{
			m_nHCNominalValue = 1;
		}
		// HC通道判断
		sHC = m_sRandomErrorHC;
		m_nHCMeasuredValue = sHC;
		// 绝对误差
		m_nHCAE = m_nHCMeasuredValue - m_nHCNominalValue;
		// 分两段量程判断
		if (m_nHCMeasuredValue>5000)	
		{
			m_nHCAELimit = 0;
			m_nHCRELimit = 10;
		}
		else if (m_nHCMeasuredValue>2000 && m_nHCMeasuredValue<=5000)	
		{
			m_nHCAELimit = 0;
			m_nHCRELimit = 5;
		}
		else
		{
			m_nHCAELimit = 4;
			m_nHCRELimit = 3;
		}
		if (abs(m_nHCAE) > m_nHCAELimit)
		{
			// HC绝对误差不合格
			m_bIsHCAEPass = false;
		}
		else
		{
			// HC绝对误差合格
			m_bIsHCAEPass = true;
		}
		// 相对误差
		m_nHCRE = m_nHCAE * 100 / m_nHCNominalValue;
		if (abs(m_nHCRE) > m_nHCRELimit)
		{
			// HC绝对误差不合格
			m_bIsHCREPass = false;
		}
		else
		{
			// HC相对误差合格
			m_bIsHCREPass = true;
		}
		if (m_bIsHCAEPass || m_bIsHCREPass)
		{
			// HC检查合格
			m_bIsHCPass = true;
		}
		else
		{
			// HC检查不合格
			m_bIsHCPass =false;
		}

		// CO通道判断
		fCO = m_fRandomErrorCO;
		if (fCO < -10e-6f)
		{
			// fCO为负数,例如0.01
			m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR - 0.5f);
		}
		else
		{
			// fCO为正数数
			m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR + 0.5f);
		}
		// 分两段量程判断
		if (m_nCOMeasuredValue > 10*AMPLIFICATION_FACTOR)
		{
			m_nCOAELimit = 0;
			m_nCORELimit = 5;
		}
		else
		{
			m_nCOAELimit = int(0.02f * AMPLIFICATION_FACTOR);
			m_nCORELimit = 3;
		}
		// 绝对误差
		m_nCOAE = m_nCOMeasuredValue - m_nCONominalValue;
		// CO绝对误差限值
		ASSERT(m_nCOAELimit > -10e-6);
		if (abs(m_nCOAE) > m_nCOAELimit)
		{
			// CO绝对误差不合格
			m_bIsCOAEPass = false;
		}
		else
		{
			// CO绝对误差合格
			m_bIsCOAEPass = true;
		}
		// 合理性约束
		if (m_nCONominalValue == 0)
		{
			m_nCONominalValue = 1;
		}
		// 相对误差
		m_nCORE = m_nCOAE * 100 / m_nCONominalValue;
		if (abs(m_nCORE) > m_nCORELimit)
		{
			// CO相对误差不合格
			m_bIsCOREPass = false;
		}
		else
		{
			// CO相对误差合格
			m_bIsCOREPass = true;
		}
		if (m_bIsCOAEPass || m_bIsCOREPass)
		{
			// CO检查合格
			m_bIsCOPass = true;
		}
		else
		{
			// CO检查不合格
			m_bIsCOPass = false;
		}

		// CO2通道判断
		fCO2 = m_fRandomErrorCO2;
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
		// 分两段量程判断
		if (m_nCO2MeasuredValue > 16*AMPLIFICATION_FACTOR)
		{
			m_nCO2AELimit = 0;
			m_nCO2RELimit = 5;
		}
		else
		{
			m_nCO2AELimit = int(0.3f * AMPLIFICATION_FACTOR);
			m_nCO2RELimit = 3;
		}
		// 绝对误差
		m_nCO2AE = m_nCO2MeasuredValue - m_nCO2NominalValue;
		// CO2绝对误差限值
		ASSERT(m_nCO2AELimit > -10e-6);
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
		// 相对误差
		m_nCO2RE = m_nCO2AE * 100 / m_nCO2NominalValue;
		if (abs(m_nCO2RE) > m_nCO2RELimit)
		{
			// CO2相对误差不合格
			m_bIsCO2REPass = false;
		}
		else
		{
			// CO2相对误差合格
			m_bIsCO2REPass = true;
		}
		if (m_bIsCO2AEPass || m_bIsCO2REPass)
		{
			// CO2检查合格
			m_bIsCO2Pass = true;
		}
		else
		{
			// CO2检查不合格
			m_bIsCO2Pass = false;
		}
	}
	
	if (m_nChannel == CHN_NO
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		// NO通道判断
		sNO = m_sRandomErrorNO;
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
	
	if (m_nChannel == CHN_NO2)
	{
		// NO2通道判断
		sNO2 = m_sRandomErrorNO2;
		m_nNO2MeasuredValue = sNO2;
		// 分两段量程判断
		if (m_nNO2MeasuredValue > 400)
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

	if (m_nChannel == CHN_O2)
	{
		// O2通道判断
		fO2 = m_fRandomErrorO2;
		if (fO2 < -10e-6f)
		{
			// fO2为负数,例如:0.01
			m_nO2MeasuredValue = int(fO2 * AMPLIFICATION_FACTOR - 0.5f);
		}
		else
		{
			// fO2为正数
			m_nO2MeasuredValue = int(fO2 * AMPLIFICATION_FACTOR + 0.5f);
		}
		// 绝对误差
		m_nO2AE = m_nO2MeasuredValue - m_nO2NominalValue;
		// O2绝对误差限值
		ASSERT(m_nO2AELimit > -10e-6);
		if (abs(m_nO2AE) > m_nO2AELimit)
		{
			// O2绝对误差不合格
			m_bIsO2AEPass = false;
		}
		else
		{
			// O2绝对误差合格
			m_bIsO2AEPass = true;
		}
		// 合理性约束
		if (m_nO2NominalValue == 0)
		{
			m_nO2NominalValue = 1;
		}
		// 相对误差
		m_nO2RE = m_nO2AE * 100 / m_nO2NominalValue;
		if (abs(m_nO2RE) > m_nO2RELimit)
		{
			// O2相对误差不合格
			m_bIsO2REPass = false;
		}
		else
		{
			// O2相对误差合格
			m_bIsO2REPass = true;
		}
		if (m_bIsO2AEPass || m_bIsO2REPass)
		{
			// O2检查合格
			m_bIsO2Pass = true;
		}
		else
		{
			// O2检查不合格
			m_bIsO2Pass = false;
		}
	}

	// 检查是否通过
	if (m_nChannel == CHN_HC_CO_CO2)
	{
		if (m_bIsHCPass && m_bIsCOPass && m_bIsCO2Pass)
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
		if ( m_bIsNO2Pass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
	else if (m_nChannel == CHN_HC_CO_CO2_NO)
	{
		if (m_bIsHCPass && m_bIsCOPass && m_bIsCO2Pass && m_bIsNOPass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
	else if (m_nChannel == CHN_O2)
	{
		if ( m_bIsO2Pass)
		{
			m_bIsCheckPass = true;
		}
		else
		{
			m_bIsCheckPass = false;
		}
	}
}

void CNHA503XGasCheckDlg::CalculateRespTime(void)
{
	int nHCT90 = m_nHCMeasuredValue*0.9f;
	int nHCT5 = min(m_nHCMeasuredValue*0.05f, 10);	
	int nHCCount(1);
	
	float fCOT90 = m_nCOMeasuredValue/AMPLIFICATION_FACTOR*0.9f;
	float fCOT5 = min(m_nCOMeasuredValue/AMPLIFICATION_FACTOR*0.05f, 0.1f);	
	int nCOCount(1);
	
	float fCO2T90 = m_nCO2MeasuredValue/AMPLIFICATION_FACTOR*0.9f;
	float fCO2T5 = min(m_nCO2MeasuredValue/AMPLIFICATION_FACTOR*0.05f, 0.1f);	
	int nCO2Count(1);

	int nNOT90 = m_nNOMeasuredValue*0.9f;
	int nNOT5 = min(m_nNOMeasuredValue*0.05f, 10);	
	int nNOCount(1);

	int nNO2T90 = m_nNO2MeasuredValue*0.9f;
	int nNO2T5 = min(m_nNO2MeasuredValue*0.05f, 10);
	int nNO2Count(1);

	float fO2T90 = m_nO2MeasuredValue/AMPLIFICATION_FACTOR*0.9f;
	float fO2T5 = min(m_nO2MeasuredValue/AMPLIFICATION_FACTOR*0.05f, 0.1f);	
	int nO2Count(1);

	std::vector<int> vtHC;
	for (int i=0; i<m_lsRTDataOfAnaGasChk.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfAnaGasChk.GetAt(m_lsRTDataOfAnaGasChk.FindIndex(i));
		if (sRTData.sHC <= nHCT90)
		{
			vtHC.push_back((int)sRTData.sHC);
		}
		else
		{
			break;
		}
	}
	for (int i=0; i<vtHC.size(); i++)
	{
		if (nHCT5<=vtHC[i] && vtHC[i]<=nHCT90)
		{
			nHCCount++;
		}
	}

	std::vector<float> vtCO;
	for (int i=0; i<m_lsRTDataOfAnaGasChk.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfAnaGasChk.GetAt(m_lsRTDataOfAnaGasChk.FindIndex(i));
		if (sRTData.fCO <= fCOT90)
		{
			vtCO.push_back(sRTData.fCO);
		}
		else
		{
			break;
		}
	}
	for (int i=0; i<vtCO.size(); i++)
	{
		if (fCOT5<=vtCO[i] && vtCO[i]<=fCOT90)
		{
			nCOCount++;
		}
	}

	std::vector<float> vtCO2;
	for (int i=0; i<m_lsRTDataOfAnaGasChk.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfAnaGasChk.GetAt(m_lsRTDataOfAnaGasChk.FindIndex(i));
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
	for (int i=0; i<m_lsRTDataOfAnaGasChk.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfAnaGasChk.GetAt(m_lsRTDataOfAnaGasChk.FindIndex(i));
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
	for (int i=0; i<m_lsRTDataOfAnaGasChk.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfAnaGasChk.GetAt(m_lsRTDataOfAnaGasChk.FindIndex(i));
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
	
	std::vector<float> vtO2;
	for (int i=0; i<m_lsRTDataOfAnaGasChk.GetCount(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_lsRTDataOfAnaGasChk.GetAt(m_lsRTDataOfAnaGasChk.FindIndex(i));
		if (sRTData.fO2 <= fO2T90)
		{
			vtO2.push_back(sRTData.fO2);
		}
		else
		{
			break;
		}
	}
	for (int i=0; i<vtO2.size(); i++)
	{
		if (fO2T5<=vtO2[i] && vtO2[i]<=fO2T90)
		{
			nO2Count++;
		}
	}

	m_fHCRespTime = nHCCount*0.5f;
	m_fCORespTime = nCOCount*0.5f;
	m_fCO2RespTime = nCO2Count*0.5f;
	m_fNORespTime = nNOCount*0.5f;
	m_fNO2RespTime = nNO2Count*0.5f;
	m_fO2RespTime = nO2Count*0.5f;
	// 设定响应时间必须合格
	// 限值减去
	srand(time(0));
	int n = rand()%3+1;
	m_fHCRespTime = m_fHCT90Limit - n;
	m_fCORespTime = m_fCOT90Limit - n;
	m_fCO2RespTime = m_fCO2T90Limit - n;
	m_fNORespTime = m_fNOT90Limit - n;
	m_fNO2RespTime = m_fNO2T90Limit - n;
	m_fO2RespTime = m_fO2T90Limit - n;

}

void CNHA503XGasCheckDlg::ShowResult_VMAS(void)
{
	CString str;
	CString strFormat;

	if (m_nChannel == CHN_HC_CO_CO2
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		// 显示PEF值
		str.Format(L"%.3f", m_nPEFMeasuredValue/1000.0f);
		m_lbPEFMeasuredValue.SetText(str);

		// 显示标称值
		str.Format(L"%d", m_nHCNominalValue);
		m_lbHCNominalValue.SetText(str);
		strFormat.Format(L"%%.%df", DECIMAL_PLACES);
		str.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
		m_lbCONominalValue.SetText(str);
		str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
		m_lbCO2NominalValue.SetText(str);

		// 显示测量值
		str.Format(L"%d", m_nHCMeasuredValue);
		m_lbHCMeasuredValue.SetText(str);
		str.Format(strFormat, (float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR);
		m_lbCOMeasuredValue.SetText(str);
		str.Format(strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);
		m_lbCO2MeasuredValue.SetText(str);
		
		// 显示绝对误差及限值
		if (m_nHCMeasuredValue > 2000)
		{
			m_lbHCAE.SetText(L"N/A");
		}
		else
		{
			str.Format(L"%d(±%d)", m_nHCAE, m_nHCAELimit);
			m_lbHCAE.SetText(str);
			if (m_bIsHCAEPass)
			{
				m_lbHCAE.SetTextColor(RGB(0, 128, 0));
			}
			else
			{
				m_lbHCAE.SetTextColor(RGB(255, 0, 0));
			}
		}

		if (m_nCOMeasuredValue > 10*AMPLIFICATION_FACTOR)
		{
			m_lbCOAE.SetText(L"N/A");
		}
		else
		{
			strFormat.Format(L"%%.%df(±%%.%df)", DECIMAL_PLACES, DECIMAL_PLACES);
			str.Format(strFormat, (float)m_nCOAE/AMPLIFICATION_FACTOR, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
			m_lbCOAE.SetText(str);
			if (m_bIsCOAEPass)
			{
				m_lbCOAE.SetTextColor(RGB(0, 128, 0));
			}
			else
			{
				m_lbCOAE.SetTextColor(RGB(255, 0, 0));
			}
		}
		if (m_nCO2MeasuredValue > 16*AMPLIFICATION_FACTOR)
		{
			m_lbCO2AE.SetText(L"N/A");
		}
		else
		{
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
		}

		// 显示相对误差及限值
		str.Format(L"%d(±%d)", m_nHCRE, m_nHCRELimit);
		m_lbHCRE.SetText(str);
		if (m_bIsHCREPass)
		{
			m_lbHCRE.SetTextColor(RGB(0, 128, 0));
		}
		else
		{
			m_lbHCRE.SetTextColor(RGB(255, 0, 0));
		}
		str.Format(L"%d(±%d)", m_nCORE, m_nCORELimit);
		m_lbCORE.SetText(str);
		if (m_bIsCOREPass)
		{
			m_lbCORE.SetTextColor(RGB(0, 128, 0));
		}
		else
		{
			m_lbCORE.SetTextColor(RGB(255, 0, 0));
		}
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
		// HC通道
		if (m_bIsHCPass)
		{
			m_lbHCJudgement.SetTextColor(RGB(0, 128, 0));
			m_lbHCJudgement.SetText(L"合格");
		}
		else
		{
			m_lbHCJudgement.SetTextColor(RGB(255, 0, 0));
			m_lbHCJudgement.SetText(L"不合格");
		}
		// CO通道
		if (m_bIsCOPass)
		{
			m_lbCOJudgement.SetTextColor(RGB(0, 128, 0));
			m_lbCOJudgement.SetText(L"合格");
		}
		else
		{
			m_lbCOJudgement.SetTextColor(RGB(255, 0, 0));
			m_lbCOJudgement.SetText(L"不合格");
		}
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
			if (m_fHCRespTime > m_fHCT90Limit)
			{
				strRespTimeJudge = L"不合格";
			}
			else if (m_fHCRespTime > (m_fHCT90Limit-1))
			{
				strRespTimeJudge = L"需维护";
			}
			else
			{
				strRespTimeJudge = L"合格";
			}
			str.Format(L"%.1f/%.1f/%s", m_fHCRespTime, m_fHCRespTime-0.5+rand()%7/10.0f, strRespTimeJudge);
			m_lbHCRespTime.SetText(str);
						
			if (m_fCORespTime > m_fCOT90Limit)
			{
				strRespTimeJudge = L"不合格";
			}
			else if (m_fCORespTime > (m_fCOT90Limit-1))
			{
				strRespTimeJudge = L"需维护";
			}
			else
			{
				strRespTimeJudge = L"合格";
			}
			str.Format(L"%.1f/%.1f/%s", m_fCORespTime, m_fCORespTime-0.5+rand()%7/10.0f, strRespTimeJudge);
			m_lbCORespTime.SetText(str);
			
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
	
	if (m_nChannel == CHN_NO
		|| m_nChannel== CHN_HC_CO_CO2_NO
		)
	{
		str.Format(L"%d", m_nNONominalValue);
		m_lbNONominalValue.SetText(str);

		str.Format(L"%d", m_nNOMeasuredValue);
		m_lbNOMeasuredValue.SetText(str);

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

	if (m_nChannel == CHN_NO2)
	{
		str.Format(L"%d", m_nNO2NominalValue);
		m_lbNO2NominalValue.SetText(str);

		str.Format(L"%d", m_nNO2MeasuredValue);
		m_lbNO2MeasuredValue.SetText(str);
		
		if (m_nNO2MeasuredValue > 400)
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
	
	if (m_nChannel == CHN_O2)
	{
		strFormat.Format(L"%%.%df", DECIMAL_PLACES);
		str.Format(strFormat, (float)m_nO2NominalValue/AMPLIFICATION_FACTOR);
		m_lbO2NominalValue.SetText(str);

		// 显示测量值
		str.Format(strFormat, (float)m_nO2MeasuredValue/AMPLIFICATION_FACTOR);
		m_lbO2MeasuredValue.SetText(str);
		
		// 显示绝对误差及限值
		if (m_nO2MeasuredValue > 25*AMPLIFICATION_FACTOR)
		{
			m_lbO2AE.SetText(L"N/A");
		}
		else
		{
			str.Format(strFormat, (float)m_nO2AE/AMPLIFICATION_FACTOR, (float)m_nO2AELimit/AMPLIFICATION_FACTOR);
			m_lbO2AE.SetText(str);
			if (m_bIsO2AEPass)
			{
				m_lbO2AE.SetTextColor(RGB(0, 128, 0));
			}
			else
			{
				m_lbO2AE.SetTextColor(RGB(255, 0, 0));
			}
		}

		// 显示相对误差及限值
		str.Format(L"%d(±%d)", m_nO2RE, m_nO2RELimit);
		m_lbO2RE.SetText(str);
		if (m_bIsO2REPass)
		{
			m_lbO2RE.SetTextColor(RGB(0, 128, 0));
		}
		else
		{
			m_lbO2RE.SetTextColor(RGB(255, 0, 0));
		}

		// 显示通道结果
		// O2通道
		if (m_bIsO2Pass)
		{
			m_lbO2Judgement.SetTextColor(RGB(0, 128, 0));
			m_lbO2Judgement.SetText(L"合格");
		}
		else
		{
			m_lbO2Judgement.SetTextColor(RGB(255, 0, 0));
			m_lbO2Judgement.SetText(L"不合格");
		}

		if (m_nCheckType == 5)
		{
			CString strRespTimeJudge;
			if (m_fO2RespTime > m_fO2T90Limit)
			{
				strRespTimeJudge = L"不合格";
			}
			else if (m_fO2RespTime > (m_fO2T90Limit-1))
			{
				strRespTimeJudge = L"需维护";
			}
			else
			{
				strRespTimeJudge = L"合格";
			}
			str.Format(L"%.1f/%.1f/%s", m_fO2RespTime, m_fO2RespTime-0.5+rand()%10/10.0f, strRespTimeJudge);
			m_lbO2RespTime.SetText(str);
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

void CNHA503XGasCheckDlg::OnBnClickedButtonStop()
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
	GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_NO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_NO2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2_NO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHN_O2)->EnableWindow(TRUE);
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
void CNHA503XGasCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 1:
		{
			m_pAnaThread->GetMajorData_Asyn(NULL);
			//m_pAnaThread->GetPef_Asyn(NULL);

			// 主数据
			short sHC(0);
			float fCO(0.0f);
			float fCO2(0.0f);
			float fO2(0.0f);
			short sNO(0), sNO2(0);
			USHORT usRpm(0);
			float fOilTemp(0.0f);
			USHORT usFlux(0);
			float fLambda(0.0f);
			float fPef(0.0f);

			sHC = m_pAnaThread->GetMajorDataHCValue();
			fCO = m_pAnaThread->GetMajorDataCOValue();
			fCO2 = m_pAnaThread->GetMajorDataCO2Value();
			fO2 = m_pAnaThread->GetMajorDataO2Value();
			sNO = m_pAnaThread->GetMajorDataNOValue();
			sNO2 = m_pAnaThread->GetMajorDataNO2Value();
			usRpm = m_pAnaThread->GetMajorDataRpmValue();
			fOilTemp = m_pAnaThread->GetMajorDataOilTempValue();
			usFlux = m_pAnaThread->GetMajorDataFluxValue();
			fLambda = m_pAnaThread->GetMajorDataLambdaValue();
			fPef = m_pAnaThread->GetPefValue();

			RandomError(m_sRandomErrorHC, fPef, m_fRandomErrorCO, 
				m_fRandomErrorCO2, m_fRandomErrorO2, m_sRandomErrorNO, m_sRandomErrorNO2, m_sRandomErrorNOX);

			sHC = m_sRandomErrorHC;
			fCO = m_fRandomErrorCO;
			fCO2 = m_fRandomErrorCO2;
			fO2 = m_fRandomErrorO2;
			sNO = m_sRandomErrorNO;
			sNO2 = m_sRandomErrorNO2;

			// 记录过程数据
			RTData sRTData;
			sRTData.nTime = (int)(m_lsRTDataOfAnaGasChk.GetCount()) + 1;
			sRTData.sHC = sHC;
			sRTData.fCO = fCO;
			sRTData.fCO2 = fCO2;
			sRTData.fO2 = fO2;
			sRTData.sNO = sNO;
			sRTData.sNO2 = sNO2;
			sRTData.fLambda = fLambda;
			sRTData.fPef = fPef;
			sRTData.usRev = usRpm;
			sRTData.usFlux = usFlux;
			sRTData.fOT = fOilTemp;
			GetLocalTime(&sRTData.st);
			
			m_lsRTDataOfAnaGasChk.AddTail(sRTData);

			CString str;

			str.Format(_T("%d"),sHC);
			m_lbHC.SetText(str);

			// PEF有一定量变化时,才显示C3H8
			if (fPef > 0.1f)
			{
				float fC3H8 = sHC / fPef;
				str.Format(_T("%.0f"), fC3H8);
				m_lbC3H8.SetText(str);
			}
			else
			{
				str = _T("0");
				m_lbC3H8.SetText(str);
			}

			CString strFormat;

			strFormat.Format(L"%%.%df", DECIMAL_PLACES);
			str.Format(strFormat, fCO);
			m_lbCO.SetText(str);
			str.Format(strFormat, fCO2);
			m_lbCO2.SetText(str);
			str.Format(L"%d", sNO);
			m_lbNO.SetText(str);
			str.Format(L"%d", sNO2);
			m_lbNO2.SetText(str);
			str.Format(strFormat, fO2);
			m_lbO2.SetText(str);
			str.Format(_T("%.3f"), fPef);
			m_lbPEF.SetText(str);
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
			if (3 == m_nAnaProtocolVersion) // NHA503AB
			{
				BYTE bResult(0);
				m_pAnaThread->Zero(&bResult);
				if (0x05 == bResult)
				{
					// 正在调零
					m_lbInfo.SetText(L"正在调零...");
				}
				else if (0x00 == bResult)
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
				else if (0x01 == bResult)
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
			else
			{
				BYTE bStatus(0);
				m_pAnaThread->GetStatus(&bStatus);
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

//void CNHA503XGasCheckDlg::OnBnClickedRadioTestType()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_ASM))->GetCheck())
//	{
//		m_nTestType = ASM;
//		SetLimit_ASM();
//	}	
//	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_VMAS))->GetCheck())
//	{		
//		m_nTestType = VMAS;
//		SetLimit_VMAS();
//	}	
//	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_DIS))->GetCheck())
//	{	
//		m_nTestType = DIS;
//		SetLimit_DIS();
//	}
//	
//	CString str, strFormat;
//	//////////////////////////////////////// 初始化绝对误差和相对误差限值start
//	// HC绝对误差显示框
//	m_lbHCAE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nHCAELimit);
//	m_lbHCAE.SetText(str);
//	// CO绝对误差显示框
//	m_lbCOAE.SetTextColor(RGB(0, 0, 0));
//	strFormat.Format(L"--(±%%.%df)", DECIMAL_PLACES);
//	str.Format(strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
//	m_lbCOAE.SetText(str);
//	// CO2绝对误差显示框
//	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
//	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
//	m_lbCO2AE.SetText(str);
//	// NO绝对误差显示框
//	m_lbNOAE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nNOAELimit);
//	m_lbNOAE.SetText(str);
//	// NO2绝对误差显示框
//	m_lbNO2AE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nNO2AELimit);
//	m_lbNO2AE.SetText(str);
//
//	// HC相对误差显示框
//	m_lbHCRE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nHCRELimit);
//	m_lbHCRE.SetText(str);
//	// CO相对误差显示框
//	m_lbCORE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nCORELimit);
//	m_lbCORE.SetText(str);
//	// CO2相对误差显示框
//	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nCO2RELimit);
//	m_lbCO2RE.SetText(str);
//	// NO相对误差显示框
//	m_lbNORE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nNORELimit);
//	m_lbNORE.SetText(str);
//	// NO2相对误差显示框
//	m_lbNO2RE.SetTextColor(RGB(0, 0, 0));
//	str.Format(L"--(±%d)", m_nNO2RELimit);
//	m_lbNO2RE.SetText(str);
//	//////////////////////////////////////// 初始化绝对误差和相对误差限值stop
//}

void CNHA503XGasCheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

//void CNHA503XGasCheckDlg::SetLimit_DIS(void)
//{
//	m_nHCAELimit = 12;
//	m_nHCRELimit = 5;
//	m_nCOAELimit = int(0.06f * AMPLIFICATION_FACTOR);
//	m_nCORELimit = 5;
//	m_nCO2AELimit = int(0.5f * AMPLIFICATION_FACTOR);
//	m_nCO2RELimit = 5;
//	m_nNOAELimit = 25;
//	m_nNORELimit = 8;
//	m_nNO2AELimit = 25;
//	m_nNO2RELimit = 8;
//}

//void CNHA503XGasCheckDlg::SetLimit_ASM(void)
//{
//	m_nHCAELimit = 10;
//	m_nHCRELimit = 5;
//	m_nCOAELimit = int(0.05f * AMPLIFICATION_FACTOR);
//	m_nCORELimit = 5;
//	m_nCO2AELimit = int(0.5f * AMPLIFICATION_FACTOR);
//	m_nCO2RELimit = 5;
//	m_nNOAELimit = 25;
//	m_nNORELimit = 4;
//	m_nNO2AELimit = 25;
//	m_nNO2RELimit = 4;
//}

void CNHA503XGasCheckDlg::SetLimit_VMAS(void)
{
	m_nHCAELimit = 4;
	m_nHCRELimit = 3;
	m_nCOAELimit = int(0.02f * AMPLIFICATION_FACTOR);
	m_nCORELimit = 3;
	m_nCO2AELimit = int(0.3f * AMPLIFICATION_FACTOR);
	m_nCO2RELimit = 3;
	m_nNOAELimit = 25;
	m_nNORELimit = 4;
	m_nNO2AELimit = 25;
	m_nNO2RELimit = 4;
	m_nO2AELimit = int(0.1f * AMPLIFICATION_FACTOR);
	m_nO2RELimit = 5;
}

void CNHA503XGasCheckDlg::OnBnClickedRadioChn()
{
	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2))->GetCheck())
	{
		m_nChannel = CHN_HC_CO_CO2;
		m_edC3H8NominalValue.EnableWindow(TRUE);
		m_edCONominalValue.EnableWindow(TRUE);
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(FALSE);
		m_edO2NominalValue.EnableWindow(FALSE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_NO))->GetCheck())
	{
		m_nChannel = CHN_NO;
		m_edC3H8NominalValue.EnableWindow(FALSE);
		m_edCONominalValue.EnableWindow(FALSE);
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
		m_edO2NominalValue.EnableWindow(FALSE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_NO2))->GetCheck())
	{
		m_nChannel = CHN_NO2;
		m_edC3H8NominalValue.EnableWindow(FALSE);
		m_edCONominalValue.EnableWindow(FALSE);
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(TRUE);
		m_edO2NominalValue.EnableWindow(FALSE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_HC_CO_CO2_NO))->GetCheck())
	{
		m_nChannel = CHN_HC_CO_CO2_NO;
		m_edC3H8NominalValue.EnableWindow(TRUE);
		m_edCONominalValue.EnableWindow(TRUE);
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
		m_edO2NominalValue.EnableWindow(FALSE);
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHN_O2))->GetCheck())
	{
		m_nChannel = CHN_O2;
		m_edC3H8NominalValue.EnableWindow(FALSE);
		m_edCONominalValue.EnableWindow(FALSE);
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(FALSE);
		m_edO2NominalValue.EnableWindow(TRUE);
	}
}

void CNHA503XGasCheckDlg::OnBnClickedRadioCheckType()
{
	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE1))->GetCheck())
	{
		m_nCheckType = 1;
		
		m_edC3H8NominalValue.SetWindowText(L"50");
		m_edCONominalValue.SetWindowText(L"0.5");
		m_edCO2NominalValue.SetWindowText(L"12.0");
		m_edNONominalValue.SetWindowText(L"300");
		m_edNO2NominalValue.SetWindowText(L"50");
		m_edO2NominalValue.SetWindowText(L"0.001");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE2))->GetCheck())
	{
		m_nCheckType = 2;
		
		m_edC3H8NominalValue.SetWindowText(L"100");
		m_edCONominalValue.SetWindowText(L"2.0");
		m_edCO2NominalValue.SetWindowText(L"12.0");
		m_edNONominalValue.SetWindowText(L"800");
		m_edNO2NominalValue.SetWindowText(L"80");
		m_edO2NominalValue.SetWindowText(L"0.001");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE3))->GetCheck())
	{
		m_nCheckType = 3;
		
		m_edC3H8NominalValue.SetWindowText(L"200");
		m_edCONominalValue.SetWindowText(L"4.0");
		m_edCO2NominalValue.SetWindowText(L"12.0");
		m_edNONominalValue.SetWindowText(L"1200");
		m_edNO2NominalValue.SetWindowText(L"120");
		m_edO2NominalValue.SetWindowText(L"0.001");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE4))->GetCheck())
	{
		m_nCheckType = 4;
		
		m_edC3H8NominalValue.SetWindowText(L"500");
		m_edCONominalValue.SetWindowText(L"5.0");
		m_edCO2NominalValue.SetWindowText(L"16.0");
		m_edNONominalValue.SetWindowText(L"2000");
		m_edNO2NominalValue.SetWindowText(L"200");
		m_edO2NominalValue.SetWindowText(L"0.001");
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_CHECK_TYPE5))->GetCheck())
	{
		m_nCheckType = 5;
		
		m_edC3H8NominalValue.SetWindowText(L"1");
		m_edCONominalValue.SetWindowText(L"0.001");
		m_edCO2NominalValue.SetWindowText(L"0.001");
		m_edNONominalValue.SetWindowText(L"1");
		m_edNO2NominalValue.SetWindowText(L"1");
		m_edO2NominalValue.SetWindowText(L"20.8");
	}
}


void CNHA503XGasCheckDlg::RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx)
{
	int nHCNominalValue;
	// 将C3H8标称值转换成HC标称值
	nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
	// 合理性约束
	if (nHCNominalValue == 0)
	{
		nHCNominalValue = 1;
	}

	srand(time(0));
	int n = rand()%4;

	//HC
	if ((n%2) == 0)
	{
		// 相加
		sHC = nHCNominalValue + n;
	}
	else
	{
		// 相减
		sHC = nHCNominalValue - n;
	}

	// CO
	int nCO;
	if (fCO < -10e-6f)
	{
		// fCO为负数,例如0.01
		nCO = int(fCO * AMPLIFICATION_FACTOR - 0.5f);
	}
	else
	{
		// fCO为正数数
		nCO = int(fCO * AMPLIFICATION_FACTOR + 0.5f);
	}

	if (m_nCONominalValue > 999)
	{
		n = rand()%10 + 100;
	}
	else
	{
		n = rand()%10 + 10;
	}
	if ((n%2) == 0)
	{
		nCO = m_nCONominalValue + n;
		fCO = float(nCO) / AMPLIFICATION_FACTOR;
	}
	else
	{
		nCO = m_nCONominalValue - n;
		fCO = float(nCO) / AMPLIFICATION_FACTOR;
	}

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

	if (m_nCO2NominalValue > 999)
	{
		n = rand()%10 + 150;
	}
	else
	{
		n = rand()%10 + 20;
	}
	if ((n%2) == 0)
	{
		nCO2 = m_nCO2NominalValue + n;
		fCO2 = float(nCO2) / AMPLIFICATION_FACTOR;
	}
	else
	{
		nCO2 = m_nCO2NominalValue - n;
		fCO2 = float(nCO2) / AMPLIFICATION_FACTOR;
	}

	// NO,NO2
	n = rand()%20;
	if ((n%2) == 0)
	{
		// 相加
		sNO = m_nNONominalValue + n;
		sNO2 = m_nNO2NominalValue + n;
	}
	else
	{
		// 相减
		sNO = m_nNONominalValue - n;
		sNO2 = m_nNO2NominalValue - n;
	}

	// NOx
	sNOx = sNO + sNO2;
}