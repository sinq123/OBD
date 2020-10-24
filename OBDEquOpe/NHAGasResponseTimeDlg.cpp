// NHAGasResponseTimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHAGasResponseTimeDlg.h"
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

// CNHAGasResponseTimeDlg 对话框

IMPLEMENT_DYNAMIC(CNHAGasResponseTimeDlg, CDialogZoom)

CNHAGasResponseTimeDlg::CNHAGasResponseTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHAGasResponseTimeDlg::IDD, pParent)
	, m_nStep(0)

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

	// 检查是否通过
	, m_bIsCheckPass(0)
	
	// 初始化回调函数
	, m_pfProcessCtrl(NULL)

	// 分析仪协议版本
	, m_nAnaProtocolVersion(4)
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
}

CNHAGasResponseTimeDlg::~CNHAGasResponseTimeDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CNHAGasResponseTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);
	DDX_Control(pDX, IDC_STATIC_HC, m_lbHC);
	DDX_Control(pDX, IDC_STATIC_C3H8, m_lbC3H8);
	DDX_Control(pDX, IDC_STATIC_CO, m_lbCO);
	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_NO, m_lbNO);
	DDX_Control(pDX, IDC_STATIC_NO2, m_lbNO2);
	DDX_Control(pDX, IDC_STATIC_PEF, m_lbPEF);
	DDX_Control(pDX, IDC_STATIC_O2, m_lbO2);
	
	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP5_LABEL, m_stcStep5Label);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);

	DDX_Control(pDX, IDC_STATIC_HC_JUDGEMENT, m_lbHCJudgement);
	DDX_Control(pDX, IDC_STATIC_CO_JUDGEMENT, m_lbCOJudgement);
	DDX_Control(pDX, IDC_STATIC_CO2_JUDGEMENT, m_lbCO2Judgement);
	DDX_Control(pDX, IDC_STATIC_NO_JUDGEMENT, m_lbNOJudgement);
	DDX_Control(pDX, IDC_STATIC_NO2_JUDGEMENT, m_lbNO2Judgement);
	DDX_Control(pDX, IDC_STATIC_O2_JUDGEMENT, m_lbO2Judgement);

	DDX_Control(pDX, IDC_STATIC_HC_T90_LIMIT, m_lbHCT90Limit);
	DDX_Control(pDX, IDC_STATIC_CO_T90_LIMIT, m_lbCOT90Limit);
	DDX_Control(pDX, IDC_STATIC_CO2_T90_LIMIT, m_lbCO2T90Limit);
	DDX_Control(pDX, IDC_STATIC_NO_T90_LIMIT, m_lbNOT90Limit);
	DDX_Control(pDX, IDC_STATIC_NO2_T90_LIMIT, m_lbNO2T90Limit);
	DDX_Control(pDX, IDC_STATIC_O2_T90_LIMIT, m_lbO2T90Limit);

	DDX_Control(pDX, IDC_STATIC_HC_RESP_TIME_T90, m_lbHCT90RespTime);
	DDX_Control(pDX, IDC_STATIC_CO_RESP_TIME_T90, m_lbCOT90RespTime);
	DDX_Control(pDX, IDC_STATIC_CO2_RESP_TIME_T90, m_lbCO2T90RespTime);
	DDX_Control(pDX, IDC_STATIC_NO_RESP_TIME_T90, m_lbNOT90RespTime);
	DDX_Control(pDX, IDC_STATIC_NO2_RESP_TIME_T90, m_lbNO2T90RespTime);
	DDX_Control(pDX, IDC_STATIC_O2_RESP_TIME_T90, m_lbO2T90RespTime);

	DDX_Control(pDX, IDC_STATIC_HC_T10_LIMIT, m_lbHCT10Limit);
	DDX_Control(pDX, IDC_STATIC_CO_T10_LIMIT, m_lbCOT10Limit);
	DDX_Control(pDX, IDC_STATIC_CO2_T10_LIMIT, m_lbCO2T10Limit);
	DDX_Control(pDX, IDC_STATIC_NO_T10_LIMIT, m_lbNOT10Limit);
	DDX_Control(pDX, IDC_STATIC_NO2_T10_LIMIT, m_lbNO2T10Limit);
	DDX_Control(pDX, IDC_STATIC_O2_T10_LIMIT, m_lbO2T10Limit);

	DDX_Control(pDX, IDC_STATIC_HC_RESP_TIME_T10, m_lbHCT10RespTime);
	DDX_Control(pDX, IDC_STATIC_CO_RESP_TIME_T10, m_lbCOT10RespTime);
	DDX_Control(pDX, IDC_STATIC_CO2_RESP_TIME_T10, m_lbCO2T10RespTime);
	DDX_Control(pDX, IDC_STATIC_NO_RESP_TIME_T10, m_lbNOT10RespTime);
	DDX_Control(pDX, IDC_STATIC_NO2_RESP_TIME_T10, m_lbNO2T10RespTime);
	DDX_Control(pDX, IDC_STATIC_O2_RESP_TIME_T10, m_lbO2T10RespTime);
}


BEGIN_MESSAGE_MAP(CNHAGasResponseTimeDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHAGasResponseTimeDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHAGasResponseTimeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHAGasResponseTimeDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHAGasResponseTimeDlg 消息处理程序

BOOL CNHAGasResponseTimeDlg::OnInitDialog()
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

void CNHAGasResponseTimeDlg::InitCtrls(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}

	m_lbHead.SetTitle(L"分析仪响应时间");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置上下标文本控件
	//m_mlb1.SetScriptText(L"C_3H_8[10^-^6]");
	//m_mlb2.SetScriptText(L"HC[10^-^6]");
	//m_mlb3.SetScriptText(L"C_3H_8[10^-^6]");
	//m_mlb4.SetScriptText(L"测量值[10^-^6]");
	//m_mlb5.SetScriptText(L"标称值[10^-^6]");
	//m_mlb6.SetScriptText(L"绝对误差[10^-^6]");
	//m_mlb8.SetScriptText(L"CO_2通道");
	//m_mlb12.SetScriptText(L"NO_2通道");
	//m_mlb13.SetScriptText(L"NO[10^-^6]");
	//m_mlb14.SetScriptText(L"NO[10^-^6]");
	//m_mlb15.SetScriptText(L"CO_2[%]");
	//m_mlb16.SetScriptText(L"CO_2[%]");
	//m_mlb17.SetScriptText(L"O_2[%]");
	//m_mlb18.SetScriptText(L"NO_2[10^-^6]");
	//m_mlb19.SetScriptText(L"NO_2[10^-^6]");

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	//m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	//m_lbLabel3.SetTextColor(RGB(0, 102, 204));
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
	
	CString strFormat;

	// HC判定显示框
	m_lbHCJudgement.SetBkColor(RGB(230, 230, 230));
	// CO判定显示框
	m_lbCOJudgement.SetBkColor(RGB(230, 230, 230));
	// CO2判定显示框
	m_lbCO2Judgement.SetBkColor(RGB(230, 230, 230));
	// NO判定显示框
	m_lbNOJudgement.SetBkColor(RGB(230, 230, 230));
	// NO2判定显示框
	m_lbNO2Judgement.SetBkColor(RGB(230, 230, 230));
	// O2判定显示框
	m_lbO2Judgement.SetBkColor(RGB(230, 230, 230));

	m_lbHCT90Limit.SetBkColor(RGB(230, 230, 230));
	m_lbCOT90Limit.SetBkColor(RGB(230, 230, 230));
	m_lbCO2T90Limit.SetBkColor(RGB(230, 230, 230));
	m_lbNOT90Limit.SetBkColor(RGB(230, 230, 230));
	m_lbNO2T90Limit.SetBkColor(RGB(230, 230, 230));
	m_lbO2T90Limit.SetBkColor(RGB(230, 230, 230));

	m_lbHCT90RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbCOT90RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbCO2T90RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNOT90RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNO2T90RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbO2T90RespTime.SetBkColor(RGB(230, 230, 230));

	m_lbHCT10Limit.SetBkColor(RGB(230, 230, 230));
	m_lbCOT10Limit.SetBkColor(RGB(230, 230, 230));
	m_lbCO2T10Limit.SetBkColor(RGB(230, 230, 230));
	m_lbNOT10Limit.SetBkColor(RGB(230, 230, 230));
	m_lbNO2T10Limit.SetBkColor(RGB(230, 230, 230));
	m_lbO2T10Limit.SetBkColor(RGB(230, 230, 230));

	m_lbHCT10RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbCOT10RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbCO2T10RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNOT10RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbNO2T10RespTime.SetBkColor(RGB(230, 230, 230));
	m_lbO2T10RespTime.SetBkColor(RGB(230, 230, 230));
		
	SetLimit();

	//////////////////////////////////////// 初始化绝对误差和相对误差限值start

	//////////////////////////////////////// 初始化绝对误差和相对误差限值stop

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

void CNHAGasResponseTimeDlg::ExportEquCalChkInfo(void)
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
	str.Format(L"\r\nItem=%s", L"分析仪气体响应时间");
	strResult += str;

	// 日期
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// 数据

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

void CNHAGasResponseTimeDlg::ExportGasCheckRTData(void)
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

	CString strPathOfGasCheckRTData = strPathOfDataExchange + L"\\DemarcationRealTimeDataOfAnaGasResponseTime.ini";
	SetFileAttributes(strPathOfGasCheckRTData, 0);
	CStdioFileEx sfe;
	sfe.Open(strPathOfGasCheckRTData, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	
	CString strRTData;	
	////////////////////////////////////////////// 总数begin
	strRTData.AppendFormat(L"\r\n");
	strRTData.AppendFormat(L"[Sum]\r\n");
	strRTData.AppendFormat(L"Total=%d\r\n", m_vtRTDataT90.size());
	sfe.WriteString(strRTData);
	////////////////////////////////////////////// 总数end

	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		strRTData.AppendFormat(_T("\r\n\r\n"));
		strRTData.AppendFormat(_T("[%d]\r\n"), i+1);
		// 时间序号
		strRTData.AppendFormat(_T("Time=%d\r\n"), m_vtRTDataT90[i].nTime);
		// HC示值
		strRTData.AppendFormat(_T("HC=%hd\r\n"), m_vtRTDataT90[i].sHC);
		// CO示值
		strRTData.AppendFormat(_T("CO=%.2f\r\n"), m_vtRTDataT90[i].fCO);
		// CO2示值
		strRTData.AppendFormat(_T("CO2=%.2f\r\n"), m_vtRTDataT90[i].fCO2);
		// NO示值
		strRTData.AppendFormat(_T("NO=%hd\r\n"), m_vtRTDataT90[i].sNO);
		// NO2示值
		strRTData.AppendFormat(_T("NO2=%hd\r\n"), m_vtRTDataT90[i].sNO2);
		// O2示值
		strRTData.AppendFormat(_T("O2=%.2f\r\n"), m_vtRTDataT90[i].fO2);
		// 绝对时间
		strRTData.AppendFormat(L"DateTime=%04d-%02d-%02d %02d:%02d:%02d.%03d\r\n", 
			m_vtRTDataT90[i].st.wYear, m_vtRTDataT90[i].st.wMonth, m_vtRTDataT90[i].st.wDay, 
			m_vtRTDataT90[i].st.wHour, m_vtRTDataT90[i].st.wMinute, m_vtRTDataT90[i].st.wSecond, m_vtRTDataT90[i].st.wMilliseconds);

		sfe.WriteString(strRTData);
	}

	sfe.Close();
}

void CNHAGasResponseTimeDlg::ExportDemarcationLog(void)
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

void CNHAGasResponseTimeDlg::ExportGasCheckResult(void)
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
	
	// 检查是否通过
	str.Format(L"\r\nPass=%s", m_bIsCheckPass?L"1":L"0");
	strResult += str;

	// 写入到结果ini文件
	CString strPathOfAnaGasCheckResult = strPathOfDataExchange + L"\\ResultOfAnaGasResponseTime.ini";
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

void CNHAGasResponseTimeDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("分析仪响应时间"));
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



void CNHAGasResponseTimeDlg::ResetProcess()
{
	//m_lbInfo.StopFlashText();

	// 清除实时值显示
	ClearRTDisplay();

	// 清除实时数据链表
	m_vtRTDataT90.clear();
	m_vtRTDataT10.clear();

	//////////////////////////////////////// 初始化绝对误差和相对误差限值start

	SetLimit();

	CString strFormat;
	CString str;

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
	// NO判定显示框
	m_lbNOJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbNOJudgement.SetText(L"--");
	// NO2判定显示框
	m_lbNO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbNO2Judgement.SetText(L"--");
	// O2判定显示框
	m_lbO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbO2Judgement.SetText(L"--");
	
	m_lbHCT90RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbHCT90RespTime.SetText(L"--");
	m_lbCOT90RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCOT90RespTime.SetText(L"--");
	m_lbCO2T90RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCO2T90RespTime.SetText(L"--");
	m_lbNOT90RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNOT90RespTime.SetText(L"--");
	m_lbNO2T90RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNO2T90RespTime.SetText(L"--");
	m_lbO2T90RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbO2T90RespTime.SetText(L"--");
	
	m_lbHCT10RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbHCT10RespTime.SetText(L"--");
	m_lbCOT10RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCOT10RespTime.SetText(L"--");
	m_lbCO2T10RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbCO2T10RespTime.SetText(L"--");
	m_lbNOT10RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNOT10RespTime.SetText(L"--");
	m_lbNO2T10RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbNO2T10RespTime.SetText(L"--");
	m_lbO2T10RespTime.SetTextColor(RGB(0, 0, 0));
	m_lbO2T10RespTime.SetText(L"--");
	
	// 步骤显示框
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));
	m_lbStep4.SetTextColor(RGB(0,0,0));
	m_lbStep5.SetTextColor(RGB(0,0,0));

	// 步骤标签
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	// 步骤置零
	m_nStep = 0;

	m_fHCT90RespTime = 0;
	m_fCOT90RespTime = 0;
	m_fCO2T90RespTime = 0;
	m_fNOT90RespTime = 0;
	m_fNO2T90RespTime = 0;
	m_fO2T90RespTime = 0;

	m_fHCT10RespTime = 0;
	m_fCOT10RespTime = 0;
	m_fCO2T10RespTime = 0;
	m_fNOT10RespTime = 0;
	m_fNO2T10RespTime = 0;
	m_fO2T10RespTime = 0;
}

void CNHAGasResponseTimeDlg::ClearRTDisplay(void)
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

void CNHAGasResponseTimeDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CNHAGasResponseTimeDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码

	if ((GetKeyState(VK_LMENU)<0||GetKeyState(VK_RMENU)<0) && GetKeyState('S')<0)
	{
		// 重置流程
		ResetProcess();
		// 如果Alt+S,则跳过调零(右边Alt键,s键不分大小写)
		m_nStep = 1;
	}

	GasCheckProcess();
}

void CNHAGasResponseTimeDlg::GasCheckProcess(void)
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
			wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪响应时间开始", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_ANA_GAS_CHK_START, wchInfo, sizeof(wchInfo));
		}

		m_lbInfo.SetText(_T("正在调零..."));
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));

		// 延时阶段,屏蔽按钮
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		// 延时2秒查询调零结果
		// 因为调零后,不能马上响应下一命令
		SetTimer(4, 2000, NULL);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));
	
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		m_lbInfo.SetText(_T("请选择气体通道,然后下一步"));
	
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		

		bool bCheckHC = (((CButton *)GetDlgItem(IDC_CHECK_HC))->GetCheck() == BST_CHECKED);
		bool bCheckNO = (((CButton *)GetDlgItem(IDC_CHECK_NO))->GetCheck() == BST_CHECKED);
		bool bCheckNO2 = (((CButton *)GetDlgItem(IDC_CHECK_NO2))->GetCheck() == BST_CHECKED);
		bool bCheckCO = (((CButton *)GetDlgItem(IDC_CHECK_CO))->GetCheck() == BST_CHECKED);
		bool bCheckCO2 = (((CButton *)GetDlgItem(IDC_CHECK_CO2))->GetCheck() == BST_CHECKED);
		bool bCheckO2 = (((CButton *)GetDlgItem(IDC_CHECK_O2))->GetCheck() == BST_CHECKED);
		if (bCheckHC ||	bCheckNO ||	bCheckNO2 || bCheckCO || bCheckCO2 || bCheckO2)
		{
			m_nStep = 2;
		}
		else
		{
			return;
		}
	}
	
	if (2 == m_nStep)
	{
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
		SetTimer(90, 500, NULL);

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));
		// 关闭气阀
		m_pAnaThread->CloseCheckGas();
		KillTimer(90);

		// 测量
		m_pAnaThread->DeflatePipeAir();

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("下一步"));
		m_lbInfo.SetText(_T("正在排气,待稳定后下一步"));
		
		// 刷新数据
		SetTimer(10, 500, NULL);

		m_nStep = 4;
	}
	else if (4 == m_nStep)
	{
		// 完成检查
		// 关泵
		m_pAnaThread->StopPump();

		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetTextColor(RGB(0, 0, 255));

		KillTimer(90);
		KillTimer(10);
		
		// 计算结果
		//CalculateResult();
		// 计算响应时间
		CalculateRespTimeT90();
		CalculateRespTimeT10();
		// 显示结果
		ShowResult();

		// 以下导出顺序不能调乱,确保DemarcationLog在Result之前导出
		// 导出标定日志
		ExportDemarcationLog();
		// 导出响应时间实时数据
		ExportGasCheckRTData();
		// 导出响应时间结果
		ExportGasCheckResult();
		// 导出响应时间结果
		ExportEquCalChkInfo();

		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪响应时间完成", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_ANA_GAS_CHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep5Label.ShowWindow(SW_SHOW);
		m_lbStep5.SetTextColor(RGB(0, 0, 0));

		m_lbInfo.SetText(_T("响应时间完成"));

		// 清除实时值显示
		ClearRTDisplay();

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

		// 重置步骤
		m_nStep = 0;
	}
}

void CNHAGasResponseTimeDlg::CalculateRespTimeT90(void)
{
	int nHCT90 = 0;
	int nHCT5 = 0;	
	int nHCCount(1);
	
	float fCOT90 = 0;
	float fCOT5 = 0;	
	int nCOCount(1);
	
	float fCO2T90 = 0;
	float fCO2T5 = 0;	
	int nCO2Count(1);

	int nNOT90 = 0;
	int nNOT5 = 0;
	int nNOCount(1);

	int nNO2T90 = 0;
	int nNO2T5 = 0;
	int nNO2Count(1);
	
	float fO2T90 = 0;
	float fO2T5 = 0;
	int nO2Count(1);
	
	if (m_vtRTDataT90.size() >= 1)
	{
		RTData sRTData = m_vtRTDataT90[m_vtRTDataT90.size()-1];

		nHCT90 = sRTData.sHC*0.90f;
		nHCT5 = min(sRTData.sHC*0.05f, 10);

		fCOT90 = sRTData.fCO*0.90f;
		fCOT5 = min(sRTData.fCO*0.05f, 0.1f);	
		
		fCO2T90 = sRTData.fCO2*0.90f;
		fCO2T5 = min(sRTData.fCO2*0.05f, 0.1f);

		nNOT90 = sRTData.sNO*0.90f;
		nNOT5 = min(sRTData.sNO*0.05f, 10);

		nNO2T90 = sRTData.sNO2*0.90f;
		nNO2T5 = min(sRTData.sNO2*0.05f, 10);
		
		fO2T90 = sRTData.fO2*0.90f;
		fO2T5 = min(sRTData.fO2*0.05f, 0.1f);
	}

	std::vector<int> vtHC;
	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT90[i];
		if (sRTData.sHC <= nHCT90)
		{
			vtHC.push_back((int)sRTData.sHC);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtHC.size(); i++)
	{
		if (nHCT5<=vtHC[i] && vtHC[i]<=nHCT90)
		{
			nHCCount++;
		}
	}

	std::vector<float> vtCO;
	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT90[i];
		if (sRTData.fCO <= fCOT90)
		{
			vtCO.push_back(sRTData.fCO);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtCO.size(); i++)
	{
		if (fCOT5<=vtCO[i] && vtCO[i]<=fCOT90)
		{
			nCOCount++;
		}
	}

	std::vector<float> vtCO2;
	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT90[i];
		if (sRTData.fCO2 <= fCO2T90)
		{
			vtCO2.push_back(sRTData.fCO2);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtCO2.size(); i++)
	{
		if (fCO2T5<=vtCO2[i] && vtCO2[i]<=fCO2T90)
		{
			nCO2Count++;
		}
	}
	
	std::vector<int> vtNO;
	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT90[i];
		if (sRTData.sNO <= nNOT90)
		{
			vtNO.push_back((int)sRTData.sNO);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtNO.size(); i++)
	{
		if (nNOT5<=vtNO[i] && vtNO[i]<=nNOT90)
		{
			nNOCount++;
		}
	}
	
	std::vector<int> vtNO2;
	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT90[i];
		if (sRTData.sNO2 <= nNO2T90)
		{
			vtNO2.push_back((int)sRTData.sNO2);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtNO2.size(); i++)
	{
		if (nNO2T5<=vtNO2[i] && vtNO2[i]<=nNO2T90)
		{
			nNO2Count++;
		}
	}

	std::vector<float> vtO2;
	for (UINT i=0; i<m_vtRTDataT90.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT90[i];
		if (sRTData.fO2 <= fO2T90)
		{
			vtO2.push_back(sRTData.fO2);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtO2.size(); i++)
	{
		if (fO2T5<=vtO2[i] && vtO2[i]<=fO2T90)
		{
			nO2Count++;
		}
	}

	m_fHCT90RespTime = nHCCount*0.5f;
	m_fCOT90RespTime = nCOCount*0.5f;
	m_fCO2T90RespTime = nCO2Count*0.5f;
	m_fNOT90RespTime = nNOCount*0.5f;
	m_fNO2T90RespTime = nNO2Count*0.5f;
	m_fO2T90RespTime = nO2Count*0.5f;
}

void CNHAGasResponseTimeDlg::CalculateRespTimeT10(void)
{
	int nHCT95 = 0;
	int nHCT10 = 0;	
	int nHCCount(1);
	
	float fCOT95 = 0;
	float fCOT10 = 0;	
	int nCOCount(1);
	
	float fCO2T95 = 0;
	float fCO2T10 = 0;	
	int nCO2Count(1);

	int nNOT95 = 0;
	int nNOT10 = 0;	
	int nNOCount(1);

	int nNO2T95 = 0;
	int nNO2T10 = 0;
	int nNO2Count(1);
	
	float fO2T95 = 0;
	float fO2T10 = 0;	
	int nO2Count(1);

	if (m_vtRTDataT10.size() >= 1)
	{
		RTData sRTData = m_vtRTDataT10[0];

		nHCT95 = sRTData.sHC*0.95f;
		nHCT10 = sRTData.sHC*0.1f;

		fCOT95 = sRTData.fCO*0.95f;
		fCOT10 = sRTData.fCO*0.1f;
		
		fCO2T95 = sRTData.fCO2*0.95f;
		fCO2T10 = sRTData.fCO2*0.1f;

		nNOT95 = sRTData.sNO*0.95f;
		nNOT10 = sRTData.sNO*0.1f;

		nNO2T95 = sRTData.sNO2*0.95f;
		nNO2T10 = sRTData.sNO2*0.1f;
		
		fO2T95 = sRTData.fO2*0.95f;
		fO2T10 = sRTData.fO2*0.1f;
	}

	std::vector<int> vtHC;
	for (UINT i=0; i<m_vtRTDataT10.size(); i++)
	{
		// 只算下降阶段
		RTData sRTData = m_vtRTDataT10[i];
		if (sRTData.sHC >= nHCT10)
		{
			vtHC.push_back((int)sRTData.sHC);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtHC.size(); i++)
	{
		if (nHCT10<=vtHC[i] && vtHC[i]<=nHCT95)
		{
			nHCCount++;
		}
	}

	std::vector<float> vtCO;
	for (UINT i=0; i<m_vtRTDataT10.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT10[i];
		if (sRTData.fCO >= fCOT10)
		{
			vtCO.push_back(sRTData.fCO);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtCO.size(); i++)
	{
		if (fCOT10<=vtCO[i] && vtCO[i]<=fCOT95)
		{
			nCOCount++;
		}
	}

	std::vector<float> vtCO2;
	for (UINT i=0; i<m_vtRTDataT10.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT10[i];
		if (sRTData.fCO2 >= fCO2T10)
		{
			vtCO2.push_back(sRTData.fCO2);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtCO2.size(); i++)
	{
		if (fCO2T10<=vtCO2[i] && vtCO2[i]<=fCO2T95)
		{
			nCO2Count++;
		}
	}
	
	std::vector<int> vtNO;
	for (UINT i=0; i<m_vtRTDataT10.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT10[i];
		if (sRTData.sNO >= nNOT10)
		{
			vtNO.push_back((int)sRTData.sNO);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtNO.size(); i++)
	{
		if (nNOT10<=vtNO[i] && vtNO[i]<=nNOT95)
		{
			nNOCount++;
		}
	}
	
	std::vector<int> vtNO2;
	for (UINT i=0; i<m_vtRTDataT10.size(); i++)
	{
		// 只算上升阶段
		RTData sRTData = m_vtRTDataT10[i];
		if (sRTData.sNO2 >= nNO2T10)
		{
			vtNO2.push_back((int)sRTData.sNO2);
		}
		else
		{
			break;
		}
	}
	for (UINT i=0; i<vtNO2.size(); i++)
	{
		if (nNO2T10<=vtNO2[i] && vtNO2[i]<=nNO2T95)
		{
			nNO2Count++;
		}
	}

	m_fHCT10RespTime = nHCCount*0.5f;
	m_fCOT10RespTime = nCOCount*0.5f;
	m_fCO2T10RespTime = nCO2Count*0.5f;
	m_fNOT10RespTime = nNOCount*0.5f;
	m_fNO2T10RespTime = nNO2Count*0.5f;
}

void CNHAGasResponseTimeDlg::ShowResult(void)
{
	CString strTemp;
	
	bool bCheckHC = (((CButton *)GetDlgItem(IDC_CHECK_HC))->GetCheck() == BST_CHECKED);
	bool bCheckNO = (((CButton *)GetDlgItem(IDC_CHECK_NO))->GetCheck() == BST_CHECKED);
	bool bCheckNO2 = (((CButton *)GetDlgItem(IDC_CHECK_NO2))->GetCheck() == BST_CHECKED);
	bool bCheckCO = (((CButton *)GetDlgItem(IDC_CHECK_CO))->GetCheck() == BST_CHECKED);
	bool bCheckCO2 = (((CButton *)GetDlgItem(IDC_CHECK_CO2))->GetCheck() == BST_CHECKED);
	bool bCheckO2 = (((CButton *)GetDlgItem(IDC_CHECK_O2))->GetCheck() == BST_CHECKED);

	if (bCheckHC)
	{
		strTemp.Format(L"%.1f", m_fHCT90RespTime);
		m_lbHCT90RespTime.SetText(strTemp);
		strTemp.Format(L"%.1f", m_fHCT10RespTime);
		m_lbHCT10RespTime.SetText(strTemp);
		m_bIsHCPass = (m_fHCT90RespTime<=m_fHCT90Limit)&&(m_fHCT10RespTime<=m_fHCT10Limit);
		m_lbHCJudgement.SetText(m_bIsHCPass?L"合格":L"不合格");
	}

	if (bCheckCO)
	{
		strTemp.Format(L"%.1f", m_fCOT90RespTime);
		m_lbCOT90RespTime.SetText(strTemp);
		strTemp.Format(L"%.1f", m_fCOT10RespTime);
		m_lbCOT10RespTime.SetText(strTemp);
		m_bIsCOPass = (m_fCOT90RespTime<=m_fCOT90Limit)&&(m_fCOT10RespTime<=m_fCOT10Limit);
		m_lbCOJudgement.SetText(m_bIsCOPass?L"合格":L"不合格");	
	}

	if (bCheckCO2)
	{
		strTemp.Format(L"%.1f", m_fCO2T90RespTime);
		m_lbCO2T90RespTime.SetText(strTemp);
		strTemp.Format(L"%.1f", m_fCO2T10RespTime);
		m_lbCO2T10RespTime.SetText(strTemp);
		m_bIsCO2Pass = (m_fCO2T90RespTime<=m_fCO2T90Limit)&&(m_fCO2T10RespTime<=m_fCO2T10Limit);
		m_lbCO2Judgement.SetText(m_bIsCO2Pass?L"合格":L"不合格");	
	}

	if (bCheckNO)
	{
		strTemp.Format(L"%.1f", m_fNOT90RespTime);
		m_lbNOT90RespTime.SetText(strTemp);
		strTemp.Format(L"%.1f", m_fNOT10RespTime);
		m_lbNOT10RespTime.SetText(strTemp);
		m_bIsNOPass = (m_fNOT90RespTime<=m_fNOT90Limit)&&(m_fNOT10RespTime<=m_fNOT10Limit);
		m_lbNOJudgement.SetText(m_bIsNOPass?L"合格":L"不合格");	
	}

	if (bCheckNO2)
	{
		strTemp.Format(L"%.1f", m_fNO2T90RespTime);
		m_lbNO2T90RespTime.SetText(strTemp);
		strTemp.Format(L"%.1f", m_fNO2T10RespTime);
		m_lbNO2T10RespTime.SetText(strTemp);
		m_bIsNO2Pass = (m_fNO2T90RespTime<=m_fNO2T90Limit)&&(m_fNO2T10RespTime<=m_fNO2T10Limit);
		m_lbNO2Judgement.SetText(m_bIsNO2Pass?L"合格":L"不合格");	
	}

	if (bCheckO2)
	{
		strTemp.Format(L"%.1f", m_fO2T90RespTime);
		m_lbO2T90RespTime.SetText(strTemp);
		strTemp.Format(L"%.1f", m_fO2T10RespTime);
		m_lbO2T10RespTime.SetText(strTemp);
		m_bIsO2Pass = (m_fO2T90RespTime<=m_fO2T90Limit)&&(m_fO2T10RespTime<=m_fO2T10Limit);	
		m_lbO2Judgement.SetText(m_bIsO2Pass?L"合格":L"不合格");
	}
}

void CNHAGasResponseTimeDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	
	KillTimer(90);
	KillTimer(10);

	KillTimer(3);

	// 重置检查流程
	ResetProcess();
	// 关闭气阀
	m_pAnaThread->CloseCheckGas();
	m_pAnaThread->StopPump();

	m_lbInfo.SetText(_T("响应时间停止"));

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"分析仪响应时间停止", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CHK_STOP, wchInfo, sizeof(wchInfo));
	}
}
void CNHAGasResponseTimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 90:
		{
			m_pAnaThread->GetMajorData_Asyn(NULL);

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

			// 记录过程数据
			RTData sRTData;
			sRTData.nTime = m_vtRTDataT90.size() + 1;
			sRTData.sHC = sHC;
			sRTData.fCO = fCO;
			sRTData.fCO2 = fCO2;
			sRTData.fO2 = fO2;
			sRTData.sNO = sNO;
			sRTData.sNO2 = sNO2;
			sRTData.fPef = fPef;
			GetLocalTime(&sRTData.st);
			m_vtRTDataT90.push_back(sRTData);

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
	case 10:
		{
			m_pAnaThread->GetMajorData_Asyn(NULL);

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

			// 记录过程数据
			RTData sRTData;
			sRTData.nTime = m_vtRTDataT10.size() + 1;
			sRTData.sHC = sHC;
			sRTData.fCO = fCO;
			sRTData.fCO2 = fCO2;
			sRTData.fO2 = fO2;
			sRTData.sNO = sNO;
			sRTData.sNO2 = sNO2;
			sRTData.fPef = fPef;
			GetLocalTime(&sRTData.st);
			m_vtRTDataT10.push_back(sRTData);

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

void CNHAGasResponseTimeDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CNHAGasResponseTimeDlg::SetLimit(void)
{
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

	CString strLimit;
	strLimit.Format(L"≤%.1f", m_fHCT90Limit);
	m_lbHCT90Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fCOT90Limit);
	m_lbCOT90Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fCO2T90Limit);
	m_lbCO2T90Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fNOT90Limit);
	m_lbNOT90Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fNO2T90Limit);
	m_lbNO2T90Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fO2T90Limit);
	m_lbO2T90Limit.SetText(strLimit);

	strLimit.Format(L"≤%.1f", m_fHCT10Limit);
	m_lbHCT10Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fCOT10Limit);
	m_lbCOT10Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fCO2T10Limit);
	m_lbCO2T10Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fNOT10Limit);
	m_lbNOT10Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fNO2T10Limit);
	m_lbNO2T10Limit.SetText(strLimit);
	strLimit.Format(L"≤%.1f", m_fO2T10Limit);
	m_lbO2T10Limit.SetText(strLimit);
}
