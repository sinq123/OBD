// AnaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AnaDlg.h"

// CAnaDlg 对话框

IMPLEMENT_DYNAMIC(CAnaDlg, CDialogZoom)

// 标定是否Pass
bool CAnaDlg::m_bDemarcationPass = false;

CAnaDlg::CAnaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CAnaDlg::IDD, pParent)
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
}

CAnaDlg::~CAnaDlg()
{	
	m_fontDlgFont.DeleteObject();
}

void CAnaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CAnaDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_GAS_CAL, &CAnaDlg::OnBnClickedButtonGasCal)
	ON_BN_CLICKED(IDC_BUTTON_GAS_CHK, &CAnaDlg::OnBnClickedButtonGasChk)
	ON_BN_CLICKED(IDC_BUTTON_ENV_PARAMS_CAL, &CAnaDlg::OnBnClickedButtonEnvParamsCal)
	ON_BN_CLICKED(IDC_BUTTON_GAS_RESPONSE_TIME, &CAnaDlg::OnBnClickedButtonGasResponseTime)
END_MESSAGE_MAP()


// CAnaDlg 消息处理程序
BOOL CAnaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAnaDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"分析仪(汽油)");
}

void CAnaDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CAnaDlg::OnBnClickedButtonGasCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 删除原有设备校准检查信息文件
	DeleteEquCalChkInfoFile();

	//ShowNHA503XGasCalibrationDlg(AnaCalProcessCtrlCallBack);
	ShowNHAMPGasCalibrationDlg(AnaCalProcessCtrlCallBack);
}

void CAnaDlg::OnBnClickedButtonGasChk()
{
	// TODO: 在此添加控件通知处理程序代码

	// 删除原有设备校准检查信息文件
	DeleteEquCalChkInfoFile();

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// 文件不存在
		return;
	}
	CSimpleIni Equipment_ini(wchPath);
	if (Equipment_ini.GetString(L"AnaParams", L"ChannelNumber", L"5") == L"5")
	{
		ShowNHA503XGasCheckDlg(AnaChkProcessCtrlCallBack);
	}
	else
	{
		ShowNHA403XGasCheckDlg(AnaChkProcessCtrlCallBack);
	}
}

void CAnaDlg::DeleteEquCalChkInfoFile(void)
{
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// 文件不存在
		return;
	}
	DeleteFile(wchPath);
}

DWORD CAnaDlg::CheckEquCalChkInfoFile(void)
{
	// 检查文件是否存在

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}

	// 检查操作是否完成

	CSimpleIni si(wchPath);
	CString str = si.GetString(L"EquCalChkInfo", L"IsOperationFinishedAtLeaseOnce", L"0");
	if (L"0" == str)
	{
		// 操作未完成
		return 0x02;
	}
	else
	{
		// 记录标定是否Pass(后续有用)
		str = si.GetString(L"EquCalChkInfo", L"Judgement", L"0");
		m_bDemarcationPass = (L"通过"==str)?true:false;

		return 0x00;
	}
}

DWORD CAnaDlg::UpdateLineInfoFile(const CString strItem, const CString strItemValue)
{
	ASSERT(L"" != strItem);
	ASSERT(L"" != strItemValue);

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}

	CSimpleIni si(wchPath);
	si.SetString(L"LineInfo", strItem, strItemValue);

	return 0x00;
}

DWORD CAnaDlg::WriteEquCalChkInfo(void)
{
	DWORD dwReturn(0);

	// 写EquCalChkInfo表
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);

	dwReturn = SetDboEquCalChkInfo(sEquCalChkInfo);
	//CNHModeClientDB_SQLite::SetDboEquCalChkInfo(sEquCalChkInfo);
	return dwReturn;
}

DWORD CAnaDlg::WriteDemarcationLog(void)
{
	DWORD dwReturn(0);

	// 写DemarcationLog表
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}

DWORD CAnaDlg::WriteDemarcationResultOfAnaGasCal(void)
{
	DWORD dwReturn(0);

	// 写DemarcationResultOfAnaGasCal表
	DEMARCATIONRESULTOFANAGASCAL sResultOfAnaGasCal;
	ZeroMemory(&sResultOfAnaGasCal, sizeof(DEMARCATIONRESULTOFANAGASCAL));
	GetIniResultOfAnaGasCal(&sResultOfAnaGasCal);

	dwReturn = SetDboDemarcationResultOfAnaGasCal(sResultOfAnaGasCal);
	return dwReturn;
}

DWORD CAnaDlg::WriteDemarcationResultOfAnaGasChk(void)
{
	DWORD dwReturn(0);

	// 写DemarcationResultOfAnaGasChk表
	DEMARCATIONRESULTOFANAGASCHK sResultOfAnaGasChk;
	ZeroMemory(&sResultOfAnaGasChk, sizeof(DEMARCATIONRESULTOFANAGASCHK));
	GetIniResultOfAnaGasChk(&sResultOfAnaGasChk);

	dwReturn = SetDboDemarcationResultOfAnaGasChk(sResultOfAnaGasChk);
	return dwReturn;
}

DWORD CAnaDlg::ReadLineInfo(LINEINFO &sLineInfo)
{
	NHCLIENT sNHClient;
	ZeroMemory(&sNHClient, sizeof(NHCLIENT));
	GetIniNHClient(&sNHClient);

	CString strSql;
	strSql.Format(L"select * from LineInfo where LineNumber = '%s'", sNHClient.wchLineNumber);

	DWORD dwReturn = GetDboLineInfo(strSql, &sLineInfo);

	return dwReturn;
}

DWORD CAnaDlg::WriteLineInfo(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// 基本检查，以防数据被篡改(有限度检查)
	// 判断检测线编号是否为空
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// 检测线编号为空
		// 不写LineInfo
		dwReturn = 0xff;
	}
	else
	{
		// 检测线编号不为空
		// 写LineInfo
		dwReturn = SetDboLineInfo(sLineInfo);
	}

	return dwReturn;
}

DWORD CAnaDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// 基本检查，以防数据被篡改(有限度检查)
	// 判断检测线编号是否为空
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// 检测线编号为空
		// 不写LineInfo.ini
		dwReturn = 0xff;
	}
	else
	{
		SetIniLineInfo(sLineInfo);
		dwReturn = 0x00;
	}

	return dwReturn;
}

DWORD CAnaDlg::WriteAnaGasChkData(void)
{
	// 预检查EquCalChkinfo.ini文件
	// 检查项目：
	// 1)文件是否存在
	// 2)操作是否完成
	if (0x00 != CheckEquCalChkInfoFile())
	{
		// 检查不通过
		return 0xff;
	}
	// 写EquCalChkInfo表
	WriteEquCalChkInfo();
	
	// 写DemarcationLog表
	WriteDemarcationLog();

	// 写DemarcationResultOfAnaGasChk表
	WriteDemarcationResultOfAnaGasChk();

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo分析仪相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 气体检查日期
		ZeroMemory(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate));
		wcscpy_s(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// 标定not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo分析仪相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 气体检查日期
		//ZeroMemory(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate));
		//wcscpy_s(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchAnalyzerChannelCheckDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	CString str = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");

	UpGasCheck(L"13W08");
	UpEqulChk(6);
	if (str.Find(L"4") != -1)
	{
		UpGasCheck13W07();
		UpEqulChk(5);
	}
	else
	{
		UpGasCheck(L"13W05");
		UpEqulChk(3);
	}
	// 本次EquCalChkInfo.ini文件使用完毕，需要删除
	DeleteEquCalChkInfoFile();
	return 0x00;
}

DWORD CAnaDlg::WriteAnaGasCalData(void)
{
	// 获取检测站编号
	STATIONINFO sStationInfo;	
	ZeroMemory(&sStationInfo, sizeof(sStationInfo));
	GetIniStationInfo(&sStationInfo);

	// 获取检测线编号和设备型号	
	LINEINFO sLineInfo;
	ZeroMemory(&sLineInfo, sizeof(sLineInfo));
	GetIniLineInfo(&sLineInfo);	
	
	// 获取操作员
	USERINFO sUserInfo;
	ZeroMemory(&sUserInfo, sizeof(sUserInfo));
	GetIniUserInfo(&sUserInfo);

	// 获取标定结果
	DEMARCATIONRESULTOFANAGASCAL sResultOfAnaGasCal;
	ZeroMemory(&sResultOfAnaGasCal, sizeof(sResultOfAnaGasCal));
	GetIniResultOfAnaGasCal(&sResultOfAnaGasCal);

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasCal.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}

	CSimpleIni si(wchPath);
	CString strDemarcationDate = si.GetString(L"ResultOfAnaGasCal", L"DemarcationDate", L"");

	// 生成标定编号
	COleDateTime oleDateTime;
	oleDateTime.ParseDateTime(strDemarcationDate);
	CString strDemarcationNumber;
	strDemarcationNumber.Format(L"%s-%s-%s", sStationInfo.wchStationNumber, sLineInfo.wchLineNumber, oleDateTime.Format(L"%Y%m%d%H%M%S"));
	wcsncpy_s(sResultOfAnaGasCal.wchDemarcationNumber, _countof(sResultOfAnaGasCal.wchDemarcationNumber), strDemarcationNumber, _countof(sResultOfAnaGasCal.wchDemarcationNumber)-1);
	
	// 写DemarcationNumber到ResultOfAnaGasCal.ini
	si.SetString(L"ResultOfAnaGasCal", L"DemarcationNumber", strDemarcationNumber);

	// 写入DemarcationLog.ini	
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(sDemarcationLog));
	wcsncpy_s(sDemarcationLog.wchDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber), strDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber)-1);
	wcsncpy_s(sDemarcationLog.wchLineNumber, _countof(sDemarcationLog.wchLineNumber), sLineInfo.wchLineNumber, _countof(sDemarcationLog.wchLineNumber)-1);
	wcsncpy_s(sDemarcationLog.wchOperator, _countof(sDemarcationLog.wchOperator), sUserInfo.wchName, _countof(sDemarcationLog.wchOperator)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchAnalyzerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchAnalyzerModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"废气分析仪", _countof(sDemarcationLog.wchEquipmentName)-1);
	CString strDemarcationItem;
	strDemarcationItem.Format(L"%d", ANA_GAS_CAL);
	wcsncpy_s(sDemarcationLog.wchDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem), strDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem)-1);
	wcsncpy_s(sDemarcationLog.wchDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate), strDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate)-1);
	wcsncpy_s(sDemarcationLog.wchPass, _countof(sDemarcationLog.wchPass), sResultOfAnaGasCal.wchPass, _countof(sDemarcationLog.wchPass)-1);
	SetIniDemarcationLog(sDemarcationLog);

	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// 写入EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchAnalyzerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchAnalyzerModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), L"分析仪气体校准", _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	CString strData,strError;
	strData.Format(L"校准气值:");
	strError.Format(L"测量值:");
	if (sResultOfAnaGasCal.wchSkipHCCalibration[0] == L'0')
	{
		strData.AppendFormat(L"[C3H8]:%s,", sResultOfAnaGasCal.wchC3H8StandardValue);
		strError.AppendFormat(L"[HC]:%s,", sResultOfAnaGasCal.wchHCMeasuredValue);
	}
	if (sResultOfAnaGasCal.wchSkipCOCalibration[0] == L'0')
	{
		strData.AppendFormat(L"[CO]:%s,", sResultOfAnaGasCal.wchCOStandardValue);
		strError.AppendFormat(L"[CO]:%s,", sResultOfAnaGasCal.wchCOMeasuredValue);
	}	
	if (sResultOfAnaGasCal.wchSkipCO2Calibration[0] == L'0')
	{
		strData.AppendFormat(L"[CO2]:%s,", sResultOfAnaGasCal.wchCO2StandardValue);
		strError.AppendFormat(L"[CO2]:%s,", sResultOfAnaGasCal.wchCO2MeasuredValue);
	}
	if (sResultOfAnaGasCal.wchSkipNOCalibration[0] == L'0')
	{
		strData.AppendFormat(L"[NO]:%s,", sResultOfAnaGasCal.wchNOStandardValue);
		strError.AppendFormat(L"[NO]:%s,", sResultOfAnaGasCal.wchNOMeasuredValue);
	}
	if (sResultOfAnaGasCal.wchSkipNO2Calibration[0] == L'0')
	{
		strData.AppendFormat(L"[NO2]:%s,", sResultOfAnaGasCal.wchNO2StandardValue);
		strError.AppendFormat(L"[NO2]:%s,", sResultOfAnaGasCal.wchNO2MeasuredValue);
	}
	strData.TrimRight(L",");
	strError.TrimRight(L",");
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	wcsncpy_s(sEquCalChkInfo.wchJudgement, _countof(sEquCalChkInfo.wchJudgement), m_bDemarcationPass ? L"通过" : L"不通过", _countof(sEquCalChkInfo.wchJudgement)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();

	// 写DemarcationLog表
	WriteDemarcationLog();
	
	// 写DemarcationResultOfAnaGasCal表
	WriteDemarcationResultOfAnaGasCal();

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo分析仪相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 气体检查日期
		ZeroMemory(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate));
		wcscpy_s(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo分析仪相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 气体检查日期
		//ZeroMemory(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate));
		//wcscpy_s(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchAnalyzerChannelCalibrationDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfAnaGasCal.ini");
	return 0x00;
}

void CAnaDlg::OnBnClickedButtonEnvParamsCal()
{
	// TODO: 在此添加控件通知处理程序代码

	LoadNHA503XEnvParamsCalibrationDlg();
}

DWORD  CAnaDlg::AnaCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_ANA_GAS_CAL_START:
	case NH_ANA_GAS_CAL_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_ANA_GAS_CAL_FINISH:
		{
			WriteAnaGasCalData();
		}
		break;
	default:
		{
			return 0xff;
		}
		break;
	}
	return 0x00;
}

DWORD  CAnaDlg::AnaChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_ANA_GAS_CHK_START:
	case NH_ANA_GAS_CHK_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_ANA_GAS_CHK_FINISH:
		{
			WriteAnaGasChkData();
		}
		break;
	default:
		{
			return 0xff;
		}
		break;
	}
	return 0x00;
}

void CAnaDlg::OnBnClickedButtonGasResponseTime()
{
	LoadNHAGasResponseTimeDlg();
}

bool CAnaDlg::UpGasCheck(const CStringW& strJKID/* = L"13W05"*/)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// 读取加载滑行结果文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// 文件不存在
		AfxMessageBox(L"分析仪其他检查文件INI不在");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"分析仪上传", L"分析仪其他检查文件INI不在");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	if (strJKID.Find(L"13W08") != -1)
	{
		//lx	类型
		strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
		strData.AppendFormat(L"<lx>%s</lx>", strTemp);
	}
	//c3h8	标准气C3H8浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0");
	strData.AppendFormat(L"<c3h8>%d</c3h8>", _wtoi(strTemp));
	//co	标准气CO浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00");
	strData.AppendFormat(L"<co>%.2f</co>", _wtof(strTemp));
	//co2	标准气CO2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00");
	strData.AppendFormat(L"<co2>%.2f</co2>", _wtof(strTemp));
	//no	标准气NO浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0");
	strData.AppendFormat(L"<no>%d</no>", _wtoi(strTemp));
	//no2	标准气NO2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0");
	strData.AppendFormat(L"<no2>%d</no2>", _wtoi(strTemp));
	//o2	标准气O2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00");
	strData.AppendFormat(L"<o2>%.2f</o2>", _wtof(strTemp));
	//hcjcjg	HC检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0");
	strData.AppendFormat(L"<hcjcjg>%d</hcjcjg>", _wtoi(strTemp));
	//cojcjg	CO检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00");
	strData.AppendFormat(L"<cojcjg>%.2f</cojcjg>", _wtof(strTemp));
	//co2jcjg	CO2检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<co2jcjg>%.2f</co2jcjg>", _wtof(strTemp));
	//nojcjg	NO检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0");
	strData.AppendFormat(L"<nojcjg>%d</nojcjg>", _wtoi(strTemp));
	//no2jcjg	NO2检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"0");
	strData.AppendFormat(L"<no2jcjg>%d</no2jcjg>", _wtoi(strTemp));
	//o2jcjg	O2检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<o2jcjg>%.2f</o2jcjg>", _wtof(strTemp));
	//pef	PEF值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3");
	strData.AppendFormat(L"<pef>%.3f</pef>", _wtof(strTemp));
	//jcjg	检查结果
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), strJKID.GetString(), strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", L"联网失败");
		return false;
	}

	return true;
}

bool CAnaDlg::UpEqulChk(const int& njclx)
{
	COleDateTime odtStart, odtEnd;

	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// 读取加载滑行结果文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// 文件不存在
		AfxMessageBox(L"分析仪其他检查文件INI不在");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"分析仪上传", L"分析仪其他检查文件INI不在");
		return false;
	}
	CSimpleIni si(wchPath);

	CString strTemp;

	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!odtStart.ParseDateTime(strTemp))
	{
		odtStart = COleDateTime::GetCurrentTime();
	}

	strTemp = si.GetString(L"ResultOfAnaGasChk", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!odtEnd.ParseDateTime(strTemp))
	{
		odtEnd = COleDateTime::GetCurrentTime();
	}

	CStringW strData, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//sbjclx	设备检查类型 1-加载滑行 2-附加损失 3-单点检查（低标气）4-单点检查（零气）5-单点检查（高标气） 6-五点检查
	strData.AppendFormat(L"<sbjclx>%d</sbjclx>", njclx);
	//jckssj	检查开始时间
	strData.AppendFormat(L"<jckssj>%s</jckssj>", odtStart.Format(L"%Y%m%d%H%M%S"));
	//jcjssj	检查结束时间
	strData.AppendFormat(L"<jcjssj>%s</jcjssj>", odtEnd.Format(L"%Y%m%d%H%M%S"));
	//cysx	采样时序
	strData.AppendFormat(L"<cysx>%s</cysx>", L"1");
	//zgzs	转鼓转速
	strData.AppendFormat(L"<zgzs>%s</zgzs>", L"");
	//cgjjzfh	测功机加载负荷
	strData.AppendFormat(L"<cgjjzfh>%s</cgjjzfh>", L"");
	//hc	HC浓度
	strData.AppendFormat(L"<hc>%s</hc>", L"");
	//co	CO浓度
	strData.AppendFormat(L"<co>%s</co>", L"");
	//no	NO浓度
	strData.AppendFormat(L"<no>%s</no>", L"");
	//no2	NO2浓度
	strData.AppendFormat(L"<no2>%s</no2>", L"");
	//co2	CO2浓度
	strData.AppendFormat(L"<co2>%s</co2>", L"");
	//o2	O2浓度
	strData.AppendFormat(L"<o2>%s</o2>", L"");

	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W11", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"检查记录上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"检查记录上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"检查记录上传", L"联网失败");
		return false;
	}

	return true;
}

bool CAnaDlg::UpGasCheck13W06(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// 读取加载滑行结果文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// 文件不存在
		AfxMessageBox(L"分析仪检查文件INI不在");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"分析仪上传", L"分析仪检查文件INI不在");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	//c3h8	标准气C3H8浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0");
	strData.AppendFormat(L"<c3h8>%d</c3h8>", _wtoi(strTemp));
	//co	标准气CO浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00");
	strData.AppendFormat(L"<co>%.2f</co>", _wtof(strTemp));
	//co2	标准气CO2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00");
	strData.AppendFormat(L"<co2>%.2f</co2>", _wtof(strTemp));
	//no	标准气NO浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0");
	strData.AppendFormat(L"<no>%d</no>", _wtoi(strTemp));
	//no2	标准气NO2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0");
	strData.AppendFormat(L"<no2>%d</no2>", _wtoi(strTemp));
	//o2	标准气O2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00");
	strData.AppendFormat(L"<o2>%.2f</o2>", _wtof(strTemp));
	//hcjcjg	HC检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0");
	strData.AppendFormat(L"<hcjcjg>%d</hcjcjg>", _wtoi(strTemp));
	//cojcjg	CO检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00");
	strData.AppendFormat(L"<cojcjg>%.2f</cojcjg>", _wtof(strTemp));
	//co2jcjg	CO2检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<co2jcjg>%.2f</co2jcjg>", _wtof(strTemp));
	//nojcjg	NO检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0");
	strData.AppendFormat(L"<nojcjg>%d</nojcjg>", _wtoi(strTemp));
	//no2jcjg	NO2检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"0");
	strData.AppendFormat(L"<no2jcjg>%d</no2jcjg>", _wtoi(strTemp));
	//o2jcjg	O2检查结果值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<o2jcjg>%.2f</o2jcjg>", _wtof(strTemp));
	//pef	PEF值
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3");
	strData.AppendFormat(L"<pef>%.3f</pef>", _wtof(strTemp));
	//jcjg	检查结果
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W06", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", L"联网失败");
		return false;
	}

	return true;
}

bool CAnaDlg::UpGasCheck13W07(void)
{
	// 设定联网配置文件日志
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// 读取加载滑行结果文件
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// 文件不存在
		AfxMessageBox(L"分析仪检查文件INI不在");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"分析仪上传", L"分析仪检查文件INI不在");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	检测机构编号
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	检测线编号
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	检查日期
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	检查开始时间
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	//c3h8	标准气C3H8浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0");
	strData.AppendFormat(L"<c3h8>%d</c3h8>", _wtoi(strTemp));
	//co	标准气CO浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00");
	strData.AppendFormat(L"<co>%.2f</co>", _wtof(strTemp));
	//co2	标准气CO2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00");
	strData.AppendFormat(L"<co2>%.2f</co2>", _wtof(strTemp));
	//no	标准气NO浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0");
	strData.AppendFormat(L"<no>%d</no>", _wtoi(strTemp));
	//no2	标准气NO2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0");
	strData.AppendFormat(L"<no2>%d</no2>", _wtoi(strTemp));
	//o2	标准气O2浓度
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00");
	strData.AppendFormat(L"<o2>%.2f</o2>", _wtof(strTemp));
	//	noxysj1	NO响应时间（T90）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOT90Limit", L"0.00");
	strData.AppendFormat(L"<noxysj1>%.2f</noxysj1>", _wtof(strTemp));
	//	no2xysj1	NO2响应时间（T90）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2T90Limit", L"0.00");
	strData.AppendFormat(L"<no2xysj1>%.2f</no2xysj1>", _wtof(strTemp));
	//coxysj1	CO响应时间（T90）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COT90Limit", L"0.00");
	strData.AppendFormat(L"<coxysj1>%.2f</coxysj1>", _wtof(strTemp));
	//o2xysj1	O2响应时间（T90）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2T90Limit", L"0.00");
	strData.AppendFormat(L"<o2xysj1>%.2f</o2xysj1>", _wtof(strTemp));
	//noxysj2	NO响应时间（T10）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOT10Limit", L"0.00");
	strData.AppendFormat(L"<noxysj2>%.2f</noxysj2>", _wtof(strTemp));
	//no2xysj2	NO2响应时间（T10）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2T10Limit", L"0.00");
	strData.AppendFormat(L"<no2xysj2>%.2f</no2xysj2>", _wtof(strTemp));
	//coxysj2	CO响应时间（T10）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COT10Limit", L"0.00");
	strData.AppendFormat(L"<coxysj2>%.2f</coxysj2>", _wtof(strTemp));
	//o2xysj2	O2响应时间（T10）
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2T10Limit", L"0.00");
	strData.AppendFormat(L"<o2xysj2>%.2f</o2xysj2>", _wtof(strTemp));
	//jcjg	检查结果
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
	//jcry	检查人员
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W07", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, 上传失败", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"加载滑行上传", L"联网失败");
		return false;
	}

	return true;
}