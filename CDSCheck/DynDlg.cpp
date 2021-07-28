// DynDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DynDlg.h"

// CDynDlg 对话框

IMPLEMENT_DYNAMIC(CDynDlg, CDialogZoom)

// 标定是否Pass
bool CDynDlg::m_bDemarcationPass = false;

CDynDlg::CDynDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDynDlg::IDD, pParent)
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

CDynDlg::~CDynDlg()
{	
	m_fontDlgFont.DeleteObject();
}

void CDynDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CDynDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_CONST_LOAD, &CDynDlg::OnBnClickedButtonConstLoad)
	ON_BN_CLICKED(IDC_BUTTON_PLHP, &CDynDlg::OnBnClickedButtonPlhp)
	ON_BN_CLICKED(IDC_BUTTON_FORCE_CALIBRATION, &CDynDlg::OnBnClickedButtonForceCalibration)
	ON_BN_CLICKED(IDC_BUTTON_FORCE_CHECK, &CDynDlg::OnBnClickedButtonForceCheck)
	ON_BN_CLICKED(IDC_BUTTON_CONTROL_MODE, &CDynDlg::OnBnClickedButtonControlMode)
	ON_BN_CLICKED(IDC_BUTTON_DIW, &CDynDlg::OnBnClickedButtonDiw)
	ON_BN_CLICKED(IDC_BUTTON_RESPOND_TIME, &CDynDlg::OnBnClickedButtonRespondTime)
	ON_BN_CLICKED(IDC_BUTTON_VAR_LOAD, &CDynDlg::OnBnClickedButtonVarLoad)
	ON_BN_CLICKED(IDC_BUTTON_PLHP_JJF, &CDynDlg::OnBnClickedButtonPlhpJjf)
	ON_BN_CLICKED(IDC_BUTTON_VAR_LOAD_JJF, &CDynDlg::OnBnClickedButtonVarLoadJjf)
END_MESSAGE_MAP()


// CDynDlg 消息处理程序
BOOL CDynDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDynDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"测功机");
}

void CDynDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CDynDlg::OnBnClickedButtonConstLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	DeleteInfoFile();
	ShowNHCConstLoadDlg(DynConstLoadProcessCtrlCallBack);
}

void CDynDlg::OnBnClickedButtonPlhp()
{
	// TODO: 在此添加控件通知处理程序代码

	ShowNHCPLHPGB2018Dlg(DynPLHPProcessCtrlCallBack);
}

void CDynDlg::OnBnClickedButtonForceCalibration()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	CString str = si.GetString(L"DynParams", L"HeavyDyn", L"0");

	if (L"1" == str)
	{
		ShowNHCForceCalibrationHeavyDlg(DynForceCalProcessCtrlCallBack);
	}
	else
	{
		// 根据测功机通讯协议选择南华轻型测功机和马哈轻型测功机力校准接口

		CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
		// 转化为大写
		strDynProtocol.MakeUpper();
		if (L"MAHA" == strDynProtocol)
		{
			// 马哈测功机力校准接口
			ShowMHDForceCalibrationDlg(DynForceCalProcessCtrlCallBack);
		}
		else
		{
			// 南华测功机力校准接口
			ShowNHCForceCalibrationDlg(DynForceCalProcessCtrlCallBack);
		}
	}
}

void CDynDlg::OnBnClickedButtonForceCheck()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	CString str = si.GetString(L"DynParams", L"HeavyDyn", L"0");

	if (L"1" == str)
	{
		ShowNHCForceCheckHeavyDlg(DynForceChkProcessCtrlCallBack);
	}
	else
	{
		ShowNHCForceCheckDlg(DynForceChkProcessCtrlCallBack);
	}
}

void CDynDlg::OnBnClickedButtonControlMode()
{
	// TODO: 在此添加控件通知处理程序代码

	ShowNHCControlModeDlg();
}

void CDynDlg::DeleteInfoFile(void)
{
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// 文件不存在
		return;
	}
	DeleteFile(wchPath);
}


DWORD CDynDlg::WriteEquCalChkInfoFile(int nDemarcationItem)
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
	if (nDemarcationItem == DYN_CL)
	{
		strItem = L"测功机加载滑行";
	}
	else if (nDemarcationItem == DYN_PLHP)
	{
		strItem = L"测功机寄生功率滑行";
	}
	else if (nDemarcationItem == DYN_FORCE_CAL)
	{
		strItem = L"测功机力校准";
	}
	else if (nDemarcationItem == DYN_FORCE_CHK)
	{
		strItem = L"测功机力检查";
	}
	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// 写入EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchDynamometerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchDynamometerModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), strItem, _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	wcsncpy_s(sEquCalChkInfo.wchJudgement, _countof(sEquCalChkInfo.wchJudgement), m_bDemarcationPass ? L"通过" : L"不通过", _countof(sEquCalChkInfo.wchJudgement)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);	// Data、Error暂时不写入，由于每个标定项目的数据格式都不同，所以在写DemarcationResult文件时再写回

	return 0x00;
}

DWORD CDynDlg::WriteEquCalChkInfo(void)
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

DWORD CDynDlg::WriteDemarcationLogFile(int nDemarcationItem)
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
	if (nDemarcationItem == DYN_CL)
	{
		strResult = L"ResultOfDynConstLoad";
	}
	else if (nDemarcationItem == DYN_PLHP)
	{
		strResult = L"ResultOfDynPLHP";
	}
	else if (nDemarcationItem == DYN_FORCE_CAL)
	{
		strResult = L"ResultOfDynForceCal";
	}
	else if (nDemarcationItem == DYN_FORCE_CHK)
	{
		strResult = L"ResultOfDynForceChk";
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
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchDynamometerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchDynamometerModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"底盘测功机", _countof(sDemarcationLog.wchEquipmentName)-1);
	CString strDemarcationItem;
	strDemarcationItem.Format(L"%d", nDemarcationItem);
	wcsncpy_s(sDemarcationLog.wchDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem), strDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem)-1);
	wcsncpy_s(sDemarcationLog.wchDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate), strDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate)-1);
	wcsncpy_s(sDemarcationLog.wchPass, _countof(sDemarcationLog.wchPass), strPass, _countof(sDemarcationLog.wchPass)-1);
	SetIniDemarcationLog(sDemarcationLog);

	return 0x00;
}

DWORD CDynDlg::WriteDemarcationLog(void)
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

DWORD CDynDlg::WriteDemarcationResultOfDynConstLoadFile(void)
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

	// 写DemarcationNumber到ResultOfDynConstLoad.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynConstLoad.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfDynConstLoad_ini(wchPath);
	ResultOfDynConstLoad_ini.SetString(L"ResultOfDynConstLoad", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfDynConstLoad.ini
	DEMARCATIONRESULTOFDYNCONSTLOAD sResultOfDynConstLoad;
	ZeroMemory(&sResultOfDynConstLoad, sizeof(DEMARCATIONRESULTOFDYNCONSTLOAD));
	GetIniResultOfDynConstLoad(&sResultOfDynConstLoad);
	
	// 合并数据
	CString strData,strError;
	strData.Format(L"ACDT=%sms,CCDT=%sms", sResultOfDynConstLoad.wchACDT, sResultOfDynConstLoad.wchCCDT);
	strError.Format(L"CDTAE=%sms,CDTRE=%s%%", sResultOfDynConstLoad.wchCDTAE, sResultOfDynConstLoad.wchCDTRE);

	// 写到EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

// 写DemarcationResultOfDynConstLoad表
DWORD CDynDlg::WriteDemarcationResultOfDynConstLoad(void)
{
	DWORD dwReturn(0);

	// 写ResultOfDynConstLoad表
	DEMARCATIONRESULTOFDYNCONSTLOAD sResultOfDynConstLoad;
	ZeroMemory(&sResultOfDynConstLoad, sizeof(DEMARCATIONRESULTOFDYNCONSTLOAD));
	GetIniResultOfDynConstLoad(&sResultOfDynConstLoad);

	dwReturn = SetDboDemarcationResultOfDynConstLoad(sResultOfDynConstLoad);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynConstLoad(sResultOfDynConstLoad);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynPLHPFile(void)
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

	// 写DemarcationNumber到ResultOfDynPLHP.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfDynPLHP_ini(wchPath);
	ResultOfDynPLHP_ini.SetString(L"ResultOfDynPLHP", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfDynPLHP.ini
	DEMARCATIONRESULTOFDYNPLHP sResultOfDynPLHP;
	ZeroMemory(&sResultOfDynPLHP, sizeof(DEMARCATIONRESULTOFDYNPLHP));
	GetIniResultOfDynPLHP(&sResultOfDynPLHP);
	
	// 合并数据
	CString strData,strError;
	if (wcscmp(sResultOfDynPLHP.wchPLHPType, L"1") == 0)	//	ASM寄生功率
	{
		strData.Format(L"PLHP40:%s,PLHP25:%s", sResultOfDynPLHP.wchPLHP1, sResultOfDynPLHP.wchPLHP3);
	}
	else if (wcscmp(sResultOfDynPLHP.wchPLHPType, L"2") == 0)	//	VMAS寄生功率
	{
		strData.Format(L"PLHP40:%s,PLHP24:%s", sResultOfDynPLHP.wchPLHP1, sResultOfDynPLHP.wchPLHP3);
	}
	else if (wcscmp(sResultOfDynPLHP.wchPLHPType, L"3") == 0)	//	LUGDOWN寄生功率
	{
		strData.Format(L"PLHP80:%s,PLHP72:%s,PLHP64:%s,PLHP56:%s,PLHP48:%s,PLHP40:%s", 
			sResultOfDynPLHP.wchPLHP1, sResultOfDynPLHP.wchPLHP2, sResultOfDynPLHP.wchPLHP3, sResultOfDynPLHP.wchPLHP4, sResultOfDynPLHP.wchPLHP5, sResultOfDynPLHP.wchPLHP6);
	}
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

// 写DemarcationResultOfDynPLHP表
DWORD CDynDlg::WriteDemarcationResultOfDynPLHP(void)
{
	DWORD dwReturn(0);

	// 写ResultOfDynPLHP表
	DEMARCATIONRESULTOFDYNPLHP sResultOfDynPLHP;
	ZeroMemory(&sResultOfDynPLHP, sizeof(DEMARCATIONRESULTOFDYNPLHP));
	GetIniResultOfDynPLHP(&sResultOfDynPLHP);

	dwReturn = SetDboDemarcationResultOfDynPLHP(sResultOfDynPLHP);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynPLHP(sResultOfDynPLHP);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynForceCalFile(void)
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

	// 写DemarcationNumber到ResultOfDynForceCal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceCal.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfDynForceCal_ini(wchPath);
	ResultOfDynForceCal_ini.SetString(L"ResultOfDynForceCal", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfDynForceCal.ini
	DEMARCATIONRESULTOFDYNFORCECAL sResultOfDynForceCal;
	ZeroMemory(&sResultOfDynForceCal, sizeof(DEMARCATIONRESULTOFDYNFORCECAL));
	GetIniResultOfDynForceCal(&sResultOfDynForceCal);
	
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

// 写DemarcationResultOfDynForceCal表
DWORD CDynDlg::WriteDemarcationResultOfDynForceCal(void)
{
	DWORD dwReturn(0);

	// 写ResultOfDynConstLoad表
	DEMARCATIONRESULTOFDYNFORCECAL sResultOfDynForceCal;
	ZeroMemory(&sResultOfDynForceCal, sizeof(DEMARCATIONRESULTOFDYNFORCECAL));
	GetIniResultOfDynForceCal(&sResultOfDynForceCal);

	dwReturn = SetDboDemarcationResultOfDynForceCal(sResultOfDynForceCal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynForceCal(sResultOfDynForceCal);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynForceChkFile(void)
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

	// 写DemarcationNumber到ResultOfDynForceChk.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceChk.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}
	CSimpleIni ResultOfDynForceChk_ini(wchPath);
	ResultOfDynForceChk_ini.SetString(L"ResultOfDynForceChk", L"DemarcationNumber", strDemarcationNumber);

	// 读取DemarcationResultOfDynForceChk.ini
	DEMARCATIONRESULTOFDYNFORCECHK sResultOfDynForceChk;
	ZeroMemory(&sResultOfDynForceChk, sizeof(DEMARCATIONRESULTOFDYNFORCECHK));
	GetIniResultOfDynForceChk(&sResultOfDynForceChk);
	
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

// 写DemarcationResultOfDynForceChk表
DWORD CDynDlg::WriteDemarcationResultOfDynForceChk(void)
{
	DWORD dwReturn(0);

	// 写ResultOfDynForceChk表
	DEMARCATIONRESULTOFDYNFORCECHK sResultOfDynForceChk;
	ZeroMemory(&sResultOfDynForceChk, sizeof(DEMARCATIONRESULTOFDYNFORCECHK));
	GetIniResultOfDynForceChk(&sResultOfDynForceChk);

	dwReturn = SetDboDemarcationResultOfDynForceChk(sResultOfDynForceChk);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynForceChk(sResultOfDynForceChk);
	return dwReturn;
}

DWORD CDynDlg::ReadLineInfo(LINEINFO &sLineInfo)
{
	NHCLIENT sNHClient;
	ZeroMemory(&sNHClient, sizeof(NHCLIENT));
	GetIniNHClient(&sNHClient);

	CString strSql;
	strSql.Format(L"select * from LineInfo where LineNumber = '%s'", sNHClient.wchLineNumber);

	DWORD dwReturn = GetDboLineInfo(strSql, &sLineInfo);

	return dwReturn;
}

DWORD CDynDlg::WriteLineInfo(LINEINFO &sLineInfo)
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

DWORD CDynDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
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

DWORD CDynDlg::WriteDynConstLoadData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(DYN_CL);
	WriteEquCalChkInfoFile(DYN_CL);
	WriteDemarcationResultOfDynConstLoadFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();
	
	// 写DemarcationLog表
	WriteDemarcationLog();

	// 写DemarcationResultOfDynConstLoad表
	WriteDemarcationResultOfDynConstLoad();

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo测功机相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 加载滑行日期
		ZeroMemory(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate));
		wcscpy_s(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate)/sizeof(wchar_t), str);

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

		//// 更新sLineInfo测功机相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 加载滑行日期
		//ZeroMemory(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate));
		//wcscpy_s(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerCoastDownDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynConstLoad.ini");

	// 上传部分
	bool bRet(false);
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	si.SetString(L"SystemCheck", L"JYY", theApp.m_sUserInfo.wchName);
	si.SetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");

	CString str = si.GetString(L"LowerRangeLoadConst", L"RangeLoadConstType", L"1");
	SHBMsg sHBMsg;
	if (L"3" == str) // LUGDOWN
	{
		CZYHttp_PAI::GetInstance().SetDieConstLoad(sHBMsg);
	}
	else // 
	{
		CZYHttp_PAI::GetInstance().SetGasConstLoad(sHBMsg);
	}
	
	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"上传成功");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"上传失败：%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}

	return 0x00;
}

DWORD CDynDlg::WriteDynPLHPData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(DYN_PLHP);
	WriteEquCalChkInfoFile(DYN_PLHP);
	WriteDemarcationResultOfDynPLHPFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();
	
	// 写DemarcationLog表
	WriteDemarcationLog();
	
	// 写DemarcationResultOfDynPLHP表
	WriteDemarcationResultOfDynPLHP();

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo测功机相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 寄生功率滑行日期
		ZeroMemory(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate));
		wcscpy_s(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate)/sizeof(wchar_t), str);

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

		//// 更新sLineInfo测功机相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 寄生功率滑行日期
		//ZeroMemory(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate));
		//wcscpy_s(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerPLHPDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynPLHP.ini");

	// 上传部分
	bool bRet(false);
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	si.SetString(L"SystemCheck", L"JYY", theApp.m_sUserInfo.wchName);
	si.SetString(L"SystemCheck", L"JYYSFZH", theApp.m_sUserInfo.wchPassword);

	CString str = si.GetString(L"ResultOfDynPLHP", L"PLHPType", L"0");
	SHBMsg sHBMsg;
	if (L"3" == str) // 柴油
	{
		CZYHttp_PAI::GetInstance().SetDiePLHP(sHBMsg);
	}
	else // 汽油
	{
		CZYHttp_PAI::GetInstance().SetGasPLHP(sHBMsg);
	}

	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"上传成功");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"上传失败：%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}

	return 0x00;
}

DWORD CDynDlg::WriteDynForceCalData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(DYN_FORCE_CAL);
	WriteEquCalChkInfoFile(DYN_FORCE_CAL);
	WriteDemarcationResultOfDynForceCalFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();
	
	// 写DemarcationLog表
	WriteDemarcationLog();
	
	// 写DemarcationResultOfDynForceCal表
	WriteDemarcationResultOfDynForceCal();
	
	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo测功机相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 力校准日期
		ZeroMemory(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate));
		wcscpy_s(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate)/sizeof(wchar_t), str);

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

		//// 更新sLineInfo测功机相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 力校准日期
		//ZeroMemory(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate));
		//wcscpy_s(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerForceCalibrationDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynForceCal.ini");
	return 0x00;
}

DWORD CDynDlg::WriteDynForceChkData(void)
{
	// 先合成ini文件
	WriteDemarcationLogFile(DYN_FORCE_CHK);
	WriteEquCalChkInfoFile(DYN_FORCE_CHK);
	WriteDemarcationResultOfDynForceChkFile();

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();
	
	// 写DemarcationLog表
	WriteDemarcationLog();
	
	// 写DemarcationResultOfDynForceChk表
	WriteDemarcationResultOfDynForceChk();
	
	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo测功机相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 力检查日期
		ZeroMemory(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate));
		wcscpy_s(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate)/sizeof(wchar_t), str);

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

		//// 更新sLineInfo测功机相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 力检查日期
		//ZeroMemory(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate));
		//wcscpy_s(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerForceCheckDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynForceChk.ini");
	return 0x00;
}

DWORD  CDynDlg::DynConstLoadProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_DYN_CL_START_LOW:
	case NH_DYN_CL_START_HIGH:
	case NH_DYN_CL_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_DYN_CL_FINISH:
		{
			WriteDynConstLoadData();
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

DWORD  CDynDlg::DynPLHPProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_DYN_PLHP_START:
	case NH_DYN_PLHP_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_DYN_PLHP_FINISH:
		{
			WriteDynPLHPData();
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

DWORD  CDynDlg::DynForceCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_DYN_FORCE_CAL_START:
	case NH_DYN_FORCE_CAL_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_DYN_FORCE_CAL_FINISH:
		{
			WriteDynForceCalData();
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

DWORD  CDynDlg::DynForceChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_DYN_FORCE_CHK_START:
	case NH_DYN_FORCE_CHK_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_DYN_FORCE_CHK_FINISH:
		{
			WriteDynForceChkData();
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


void CDynDlg::OnBnClickedButtonDiw()
{
	ShowNHCDIWDlg();
}


void CDynDlg::OnBnClickedButtonRespondTime()
{
	ShowNHCResponseTime();
}


void CDynDlg::OnBnClickedButtonVarLoad()
{
	ShowNHCVarLoadDlg2();
}


void CDynDlg::OnBnClickedButtonPlhpJjf()
{
	ShowNHCPLHP2Dlg(DynPLHPProcessCtrlCallBack);
}


void CDynDlg::OnBnClickedButtonVarLoadJjf()
{
	ShowNHCVarLoadDlg();
}
