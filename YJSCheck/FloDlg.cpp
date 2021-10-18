// FloDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FloDlg.h"

// CFloDlg 对话框

IMPLEMENT_DYNAMIC(CFloDlg, CDialogZoom)

// 标定是否Pass
bool CFloDlg::m_bDemarcationPass = false;

CFloDlg::CFloDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFloDlg::IDD, pParent)
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

CFloDlg::~CFloDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CFloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CFloDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_DILO2_CAL, &CFloDlg::OnBnClickedButtonDilo2Cal)
	ON_BN_CLICKED(IDC_BUTTON_PRES_CAL, &CFloDlg::OnBnClickedButtonPresCal)
	ON_BN_CLICKED(IDC_BUTTON_FLUX_CHK, &CFloDlg::OnBnClickedButtonFluxChk)
	//ON_BN_CLICKED(IDC_BUTTON_FLUX_CAL, &CFloDlg::OnBnClickedButtonFluxCal)
	ON_BN_CLICKED(IDC_BUTTON_DELAYTIME_CFG, &CFloDlg::OnBnClickedButtonDelayTimeCfg)
	ON_BN_CLICKED(IDC_BUTTON_FLO_CAL, &CFloDlg::OnBnClickedButtonFloCal)
END_MESSAGE_MAP()


// CFloDlg 消息处理程序
BOOL CFloDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFloDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"流量计");
}

void CFloDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFloDlg::OnBnClickedButtonDilo2Cal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 删除原有设备校准检查信息文件
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterDilO2CalibrationDlg(FloDilO2CalProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonPresCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 删除原有设备校准检查信息文件
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterPressureCalibrationDlg(FloPresCalProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonFluxChk()
{
	// TODO: 在此添加控件通知处理程序代码

	// 删除原有设备校准检查信息文件
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterFreeFlowCheckDlg(FloFluxChkProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonFluxCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 删除原有设备校准检查信息文件
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterFlowCalibrationDlg(FloFluxCalProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonDelayTimeCfg()
{
	// TODO: 在此添加控件通知处理程序代码

	LoadFlowmeterDelayTimeConfigDlg();
}

//void CFloDlg::DeleteEquCalChkInfoFile(void)
//{
//	// 获取文件路径
//	wchar_t wchPath[MAX_PATH];
//	ZeroMemory(wchPath, sizeof(wchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
//	{
//		// 文件不存在
//		return;
//	}
//	DeleteFile(wchPath);
//}
//
//DWORD CFloDlg::CheckEquCalChkInfoFile(void)
//{
//	// 检查文件是否存在
//
//	// 获取文件路径
//	wchar_t wchPath[MAX_PATH];
//	ZeroMemory(wchPath, sizeof(wchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
//	{
//		// 文件不存在
//		return 0x01;
//	}
//
//	// 检查操作是否完成
//
//	CSimpleIni si(wchPath);
//	CString str = si.GetString(L"EquCalChkInfo", L"IsOperationFinishedAtLeaseOnce", L"0");
//	if (L"0" == str)
//	{
//		// 操作未完成
//		return 0x02;
//	}
//	else
//	{
//		// 记录标定是否Pass(后续有用)
//		str = si.GetString(L"EquCalChkInfo", L"Judgement", L"0");
//		m_bDemarcationPass = (L"通过"==str)?true:false;
//
//		return 0x00;
//	}
//}

DWORD CFloDlg::UpdateLineInfoFile(const CString strItem, const CString strItemValue)
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

DWORD CFloDlg::WriteEquCalChkInfo(void)
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

DWORD CFloDlg::WriteEquCalChkInfoFile(int nDemarcationItem)
{
	// 获取检测线编号和设备型号	
	LINEINFO sLineInfo;
	ZeroMemory(&sLineInfo, sizeof(sLineInfo));
	GetIniLineInfo(&sLineInfo);	

	// 获取操作员
	USERINFO sUserInfo;
	ZeroMemory(&sUserInfo, sizeof(sUserInfo));
	GetIniUserInfo(&sUserInfo);

	// 获取DemarcationLog
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(sDemarcationLog));
	GetIniDemarcationLog(&sDemarcationLog);

	CString strItem;
	if (nDemarcationItem == FLO_FLUXCAL)
	{
		strItem = L"流量计流量校准";
	}
	else if (nDemarcationItem == FLO_FLUXCHK)
	{
		strItem = L"流量计流量检查";
	}
	else if (nDemarcationItem == FLO_DILO2CAL)
	{
		strItem = L"流量计稀释氧校准";
	}
	else if (nDemarcationItem == FLO_PRESCAL)
	{
		strItem = L"流量计压力校准";
	}
	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// 写入EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchFlowmeterNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchFlowmeterModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), strItem, _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	wcsncpy_s(sEquCalChkInfo.wchJudgement, _countof(sEquCalChkInfo.wchJudgement), m_bDemarcationPass ? L"通过" : L"不通过", _countof(sEquCalChkInfo.wchJudgement)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);	// Data、Error暂时不写入，由于每个标定项目的数据格式都不同，所以在写DemarcationResult文件时再写回

	return 0x00;
}

DWORD CFloDlg::WriteDemarcationLogFile(int nDemarcationItem)
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

	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CString strResult;
	if (nDemarcationItem == FLO_FLUXCAL)
	{
		strResult = L"ResultOfFloFluxCal";
	}
	else if (nDemarcationItem == FLO_FLUXCHK)
	{
		strResult = L"ResultOfFloFluxChk";
	}
	else if (nDemarcationItem == FLO_DILO2CAL)
	{
		strResult = L"ResultOfFloDilO2Cal";
	}
	else if (nDemarcationItem == FLO_PRESCAL)
	{
		strResult = L"ResultOfFloPresCal";
	}
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", strResult+L".ini", wchPath))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni si(wchPath);
	CString strDemarcationDate = si.GetString(strResult, L"DemarcationDate", L"");
	CString strPass = si.GetString(strResult, L"Pass", L"");

	// 生成标定编号
	COleDateTime oleDateTime;
	oleDateTime.ParseDateTime(strDemarcationDate);
	CString strDemarcationNumber;
	strDemarcationNumber.Format(L"%s-%s-%s", sStationInfo.wchStationNumber, sLineInfo.wchLineNumber, oleDateTime.Format(L"%Y%m%d%H%M%S"));
	
	// 写入DemarcationLog.ini
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(sDemarcationLog));
	wcsncpy_s(sDemarcationLog.wchDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber), strDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber)-1);
	wcsncpy_s(sDemarcationLog.wchLineNumber, _countof(sDemarcationLog.wchLineNumber), sLineInfo.wchLineNumber, _countof(sDemarcationLog.wchLineNumber)-1);
	wcsncpy_s(sDemarcationLog.wchOperator, _countof(sDemarcationLog.wchOperator), sUserInfo.wchName, _countof(sDemarcationLog.wchOperator)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchFlowmeterNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchFlowmeterModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"流量计", _countof(sDemarcationLog.wchEquipmentName)-1);
	CString strDemarcationItem;
	strDemarcationItem.Format(L"%d", nDemarcationItem);
	wcsncpy_s(sDemarcationLog.wchDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem), strDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem)-1);
	wcsncpy_s(sDemarcationLog.wchDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate), strDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate)-1);
	wcsncpy_s(sDemarcationLog.wchPass, _countof(sDemarcationLog.wchPass), strPass, _countof(sDemarcationLog.wchPass)-1);
	SetIniDemarcationLog(sDemarcationLog);

	return 0x00;
}

DWORD CFloDlg::WriteDemarcationLog(void)
{
	DWORD dwReturn(0);

	// 写DemarcationLog表
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}


DWORD CFloDlg::WriteDemarcationResultOfFloFluxChk(void)
{
	DWORD dwReturn(0);

	// 写DemarcationResultOfFloFluxChk表
	DEMARCATIONRESULTOFFLOFLUXCHK sResultOfFloFluxChk;
	ZeroMemory(&sResultOfFloFluxChk, sizeof(DEMARCATIONRESULTOFFLOFLUXCHK));
	GetIniResultOfFloFluxChk(&sResultOfFloFluxChk);

	dwReturn = SetDboDemarcationResultOfFloFluxChk(sResultOfFloFluxChk);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloFluxChk(sResultOfFloFluxChk);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloFluxChkFile(void)
{
	// 读DemarcationLog.ini的DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// 写DemarcationNumber到ResultOfFloFluxChk.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxChk.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfFloFluxChk_ini(wchPath);
	ResultOfFloFluxChk_ini.SetString(L"ResultOfFloFluxChk", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfFloFluxChk.ini
	DEMARCATIONRESULTOFFLOFLUXCHK sResultOfFloFluxChk;
	ZeroMemory(&sResultOfFloFluxChk, sizeof(DEMARCATIONRESULTOFFLOFLUXCHK));
	GetIniResultOfFloFluxChk(&sResultOfFloFluxChk);
	
	// 合并数据
	CString strData,strError;
	strData.Format(L"");
	strError.Format(L"");

	// 写到EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

DWORD CFloDlg::WriteDemarcationResultOfFloFluxCal(void)
{
	DWORD dwReturn(0);

	// 写DemarcationResultOfFloFluxCal表
	DEMARCATIONRESULTOFFLOFLUXCAL sResultOfFloFluxCal;
	ZeroMemory(&sResultOfFloFluxCal, sizeof(DEMARCATIONRESULTOFFLOFLUXCAL));
	GetIniResultOfFloFluxCal(&sResultOfFloFluxCal);

	dwReturn = SetDboDemarcationResultOfFloFluxCal(sResultOfFloFluxCal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloFluxCal(sResultOfFloFluxCal);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloFluxCalFile(void)
{
	// 读DemarcationLog.ini的DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// 写DemarcationNumber到ResultOfFloFluxCal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxCal.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfFloFluxCal_ini(wchPath);
	ResultOfFloFluxCal_ini.SetString(L"ResultOfFloFluxCal", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfFloFluxCal.ini
	DEMARCATIONRESULTOFFLOFLUXCAL sResultOfFloFluxCal;
	ZeroMemory(&sResultOfFloFluxCal, sizeof(DEMARCATIONRESULTOFFLOFLUXCAL));
	GetIniResultOfFloFluxCal(&sResultOfFloFluxCal);
	
	// 合并数据
	CString strData,strError;
	strData.Format(L"");
	strError.Format(L"");

	// 写到EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

DWORD CFloDlg::WriteDemarcationResultOfFloDilO2Cal(void)
{	
	DWORD dwReturn(0);

	// 写DemarcationResultOfFloDilO2Cal表
	DEMARCATIONRESULTOFFLODILO2CAL sResultOfFloDilO2Cal;
	ZeroMemory(&sResultOfFloDilO2Cal, sizeof(DEMARCATIONRESULTOFFLODILO2CAL));
	GetIniResultOfFloDilO2Cal(&sResultOfFloDilO2Cal);

	dwReturn = SetDboDemarcationResultOfFloDilO2Cal(sResultOfFloDilO2Cal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloDilO2Cal(sResultOfFloDilO2Cal);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloDilO2CalFile(void)
{
	// 读DemarcationLog.ini的DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// 写DemarcationNumber到ResultOfFloDilO2Cal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloDilO2Cal.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfFloDilO2Cal_ini(wchPath);
	ResultOfFloDilO2Cal_ini.SetString(L"ResultOfFloDilO2Cal", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfFloDilO2Cal.ini
	DEMARCATIONRESULTOFFLODILO2CAL sResultOfFloDilO2Cal;
	ZeroMemory(&sResultOfFloDilO2Cal, sizeof(DEMARCATIONRESULTOFFLODILO2CAL));
	GetIniResultOfFloDilO2Cal(&sResultOfFloDilO2Cal);
	
	// 合并数据
	CString strData,strError;
	strData.Format(L"");
	strError.Format(L"");

	// 写到EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

DWORD CFloDlg::WriteDemarcationResultOfFloPresCal(void)
{
	DWORD dwReturn(0);

	// 写DemarcationResultOfFloPresCal表
	DEMARCATIONRESULTOFFLOPRESCAL sResultOfFloPresCal;
	ZeroMemory(&sResultOfFloPresCal, sizeof(DEMARCATIONRESULTOFFLOPRESCAL));
	GetIniResultOfFloPresCal(&sResultOfFloPresCal);

	dwReturn = SetDboDemarcationResultOfFloPresCal(sResultOfFloPresCal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloPresCal(sResultOfFloPresCal);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloPresCalFile(void)
{
	// 读DemarcationLog.ini的DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// 写DemarcationNumber到ResultOfFloPresCal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloPresCal.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfFloPresCal_ini(wchPath);
	ResultOfFloPresCal_ini.SetString(L"ResultOfFloPresCal", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfFloPresCal.ini
	DEMARCATIONRESULTOFFLOPRESCAL sResultOfFloPresCal;
	ZeroMemory(&sResultOfFloPresCal, sizeof(DEMARCATIONRESULTOFFLOPRESCAL));
	GetIniResultOfFloPresCal(&sResultOfFloPresCal);
	
	// 合并数据
	CString strData,strError;
	strData.Format(L"");
	strError.Format(L"");

	// 写到EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

DWORD CFloDlg::ReadLineInfo(LINEINFO &sLineInfo)
{
	NHCLIENT sNHClient;
	ZeroMemory(&sNHClient, sizeof(NHCLIENT));
	GetIniNHClient(&sNHClient);

	CString strSql;
	strSql.Format(L"select * from LineInfo where LineNumber = '%s'", sNHClient.wchLineNumber);

	ZeroMemory(&sLineInfo, sizeof(LINEINFO));
	DWORD dwReturn = GetDboLineInfo(strSql, &sLineInfo);

	return dwReturn;
}

DWORD CFloDlg::WriteLineInfo(LINEINFO &sLineInfo)
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

DWORD CFloDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
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

DWORD CFloDlg::WriteFloDilO2CalData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(FLO_DILO2CAL);
	WriteEquCalChkInfoFile(FLO_DILO2CAL);
	WriteDemarcationResultOfFloDilO2CalFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();

	// 写DemarcationLog表
	WriteDemarcationLog();

	// 数据库还没有相应表，先屏蔽此段代码
	//// 写DemarcationResultOfFloDilO2Cal表
	//if (0x00 != WriteDemarcationResultOfFloDilO2Cal())
	//{
	//	// 写DemarcationResultOfFloDilO2Cal表失败
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo流量计相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate));
		wcscpy_s(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// 标定 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo流量计相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate));
		//wcscpy_s(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterDilO2CalibrationDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

DWORD CFloDlg::WriteFloPresCalData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(FLO_PRESCAL);
	WriteEquCalChkInfoFile(FLO_PRESCAL);
	WriteDemarcationResultOfFloPresCalFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();

	// 写DemarcationLog表
	WriteDemarcationLog();

	// 数据库还没有相应表，先屏蔽此段代码
	//// 写DemarcationResultOfFloPresCal表
	//if (0x00 != WriteDemarcationResultOfFloPresCal())
	//{
	//	// 写DemarcationResultOfFloPresCal表失败
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo流量计相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate));
		wcscpy_s(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// 标定 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo流量计相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate));
		//wcscpy_s(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterPressureCalibrationDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

DWORD CFloDlg::WriteFloFluxChkData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(FLO_FLUXCHK);
	WriteEquCalChkInfoFile(FLO_FLUXCHK);
	WriteDemarcationResultOfFloFluxChkFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();

	// 写DemarcationLog表
	WriteDemarcationLog();
	
	// 数据库还没有相应表，先屏蔽此段代码
	//// 写DemarcationResultOfFloFluxChk表
	//if (0x00 != WriteDemarcationResultOfFloFluxChk())
	//{
	//	// 写DemarcationResultOfFloFluxChk表失败
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo流量计相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate));
		wcscpy_s(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// 标定 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo流量计相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate));
		//wcscpy_s(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterFluxCheckDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

DWORD CFloDlg::WriteFloFluxCalData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(FLO_FLUXCAL);
	WriteEquCalChkInfoFile(FLO_FLUXCAL);
	WriteDemarcationResultOfFloFluxCalFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();

	// 写DemarcationLog表
	WriteDemarcationLog();

	// 数据库还没有相应表，先屏蔽此段代码
	//// 写DemarcationResultOfFloFluxCal表
	//if (0x00 != WriteDemarcationResultOfFloFluxCal())
	//{
	//	// 写DemarcationResultOfFloFluxCal表失败
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo流量计相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate));
		wcscpy_s(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo流量计相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate));
		//wcscpy_s(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterFluxCalibrationDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

// 流量校准过程控制回调函数
DWORD CFloDlg::FloFluxCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf /*= NULL*/, const LONG lBufLen /*= 0*/)
{
	switch (dwStatus)
	{
	case NH_FLO_FLUXCAL_START:
	case NH_FLO_FLUXCAL_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_FLO_FLUXCAL_FINISH:
		{
			WriteFloFluxCalData();
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

// 流量检查过程控制回调函数
DWORD CFloDlg::FloFluxChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf /*= NULL*/, const LONG lBufLen /*= 0*/)
{
	switch (dwStatus)
	{
	case NH_FLO_FLUXCHK_START:
	case NH_FLO_FLUXCHK_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_FLO_FLUXCHK_FINISH:
		{
			WriteFloFluxChkData();
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

// 压力校准过程控制回调函数
DWORD CFloDlg::FloPresCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf /*= NULL*/, const LONG lBufLen /*= 0*/)
{
	switch (dwStatus)
	{
	case NH_FLO_PRESCAL_START:
	case NH_FLO_PRESCAL_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_FLO_PRESCAL_FINISH:
		{
			WriteFloPresCalData();
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

// 稀释氧校准过程控制回调函数
DWORD CFloDlg::FloDilO2CalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf /*= NULL*/, const LONG lBufLen /*= 0*/)
{
	switch (dwStatus)
	{
	case NH_FLO_DILO2CAL_START:
	case NH_FLO_DILO2CAL_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_FLO_DILO2CAL_FINISH:
		{
			WriteFloDilO2CalData();
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

void CFloDlg::OnBnClickedButtonFloCal()
{
	LoadFlowmeterFlowCalibrationDlg();
}
