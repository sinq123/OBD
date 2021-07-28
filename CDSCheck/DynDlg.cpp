// DynDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DynDlg.h"

// CDynDlg �Ի���

IMPLEMENT_DYNAMIC(CDynDlg, CDialogZoom)

// �궨�Ƿ�Pass
bool CDynDlg::m_bDemarcationPass = false;

CDynDlg::CDynDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDynDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
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
		_T("����"));
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


// CDynDlg ��Ϣ�������
BOOL CDynDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDynDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DeleteInfoFile();
	ShowNHCConstLoadDlg(DynConstLoadProcessCtrlCallBack);
}

void CDynDlg::OnBnClickedButtonPlhp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ShowNHCPLHPGB2018Dlg(DynPLHPProcessCtrlCallBack);
}

void CDynDlg::OnBnClickedButtonForceCalibration()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// ��ʱ������������
	}
	CSimpleIni si(wchPath);

	CString str = si.GetString(L"DynParams", L"HeavyDyn", L"0");

	if (L"1" == str)
	{
		ShowNHCForceCalibrationHeavyDlg(DynForceCalProcessCtrlCallBack);
	}
	else
	{
		// ���ݲ⹦��ͨѶЭ��ѡ���ϻ����Ͳ⹦����������Ͳ⹦����У׼�ӿ�

		CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
		// ת��Ϊ��д
		strDynProtocol.MakeUpper();
		if (L"MAHA" == strDynProtocol)
		{
			// ����⹦����У׼�ӿ�
			ShowMHDForceCalibrationDlg(DynForceCalProcessCtrlCallBack);
		}
		else
		{
			// �ϻ��⹦����У׼�ӿ�
			ShowNHCForceCalibrationDlg(DynForceCalProcessCtrlCallBack);
		}
	}
}

void CDynDlg::OnBnClickedButtonForceCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// ��ʱ������������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ShowNHCControlModeDlg();
}

void CDynDlg::DeleteInfoFile(void)
{
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		return;
	}
	DeleteFile(wchPath);
}


DWORD CDynDlg::WriteEquCalChkInfoFile(int nDemarcationItem)
{
	// ��ȡ����߱�ź��豸�ͺ�	
	LINEINFO sLineInfo;
	ZeroMemory(&sLineInfo, sizeof(sLineInfo));
	GetIniLineInfo(&sLineInfo);	

	// ��ȡ����Ա
	USERINFO sUserInfo;
	ZeroMemory(&sUserInfo, sizeof(sUserInfo));
	GetIniUserInfo(&sUserInfo);

	// ��ȡDemarcationLog
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(sDemarcationLog));
	GetIniDemarcationLog(&sDemarcationLog);

	CString strItem;
	if (nDemarcationItem == DYN_CL)
	{
		strItem = L"�⹦�����ػ���";
	}
	else if (nDemarcationItem == DYN_PLHP)
	{
		strItem = L"�⹦���������ʻ���";
	}
	else if (nDemarcationItem == DYN_FORCE_CAL)
	{
		strItem = L"�⹦����У׼";
	}
	else if (nDemarcationItem == DYN_FORCE_CHK)
	{
		strItem = L"�⹦�������";
	}
	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchDynamometerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchDynamometerModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), strItem, _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	wcsncpy_s(sEquCalChkInfo.wchJudgement, _countof(sEquCalChkInfo.wchJudgement), m_bDemarcationPass ? L"ͨ��" : L"��ͨ��", _countof(sEquCalChkInfo.wchJudgement)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);	// Data��Error��ʱ��д�룬����ÿ���궨��Ŀ�����ݸ�ʽ����ͬ��������дDemarcationResult�ļ�ʱ��д��

	return 0x00;
}

DWORD CDynDlg::WriteEquCalChkInfo(void)
{
	DWORD dwReturn(0);

	// дEquCalChkInfo��
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);

	dwReturn = SetDboEquCalChkInfo(sEquCalChkInfo);
	//CNHModeClientDB_SQLite::SetDboEquCalChkInfo(sEquCalChkInfo);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationLogFile(int nDemarcationItem)
{		
	// ��ȡ���վ���
	STATIONINFO sStationInfo;	
	ZeroMemory(&sStationInfo, sizeof(sStationInfo));
	GetIniStationInfo(&sStationInfo);

	// ��ȡ����߱�ź��豸�ͺ�	
	LINEINFO sLineInfo;
	ZeroMemory(&sLineInfo, sizeof(sLineInfo));
	GetIniLineInfo(&sLineInfo);	
	
	// ��ȡ����Ա
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
		// �ļ�������
		return 0x01;
	}
	CSimpleIni si(wchPath);
	CString strDemarcationDate = si.GetString(strResult, L"DemarcationDate", L"");
	CString strPass = si.GetString(strResult, L"Pass", L"");

	// ���ɱ궨���
	COleDateTime oleDateTime;
	oleDateTime.ParseDateTime(strDemarcationDate);
	CString strDemarcationNumber;
	strDemarcationNumber.Format(L"%s-%s-%s", sStationInfo.wchStationNumber, sLineInfo.wchLineNumber, oleDateTime.Format(L"%Y%m%d%H%M%S"));
	
	// д��DemarcationLog.ini
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(sDemarcationLog));
	wcsncpy_s(sDemarcationLog.wchDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber), strDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber)-1);
	wcsncpy_s(sDemarcationLog.wchLineNumber, _countof(sDemarcationLog.wchLineNumber), sLineInfo.wchLineNumber, _countof(sDemarcationLog.wchLineNumber)-1);
	wcsncpy_s(sDemarcationLog.wchOperator, _countof(sDemarcationLog.wchOperator), sUserInfo.wchName, _countof(sDemarcationLog.wchOperator)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchDynamometerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchDynamometerModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"���̲⹦��", _countof(sDemarcationLog.wchEquipmentName)-1);
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

	// дDemarcationLog��
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynConstLoadFile(void)
{
	// ��DemarcationLog.ini��DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// дDemarcationNumber��ResultOfDynConstLoad.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfDynConstLoad_ini(wchPath);
	ResultOfDynConstLoad_ini.SetString(L"ResultOfDynConstLoad", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfDynConstLoad.ini
	DEMARCATIONRESULTOFDYNCONSTLOAD sResultOfDynConstLoad;
	ZeroMemory(&sResultOfDynConstLoad, sizeof(DEMARCATIONRESULTOFDYNCONSTLOAD));
	GetIniResultOfDynConstLoad(&sResultOfDynConstLoad);
	
	// �ϲ�����
	CString strData,strError;
	strData.Format(L"ACDT=%sms,CCDT=%sms", sResultOfDynConstLoad.wchACDT, sResultOfDynConstLoad.wchCCDT);
	strError.Format(L"CDTAE=%sms,CDTRE=%s%%", sResultOfDynConstLoad.wchCDTAE, sResultOfDynConstLoad.wchCDTRE);

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

// дDemarcationResultOfDynConstLoad��
DWORD CDynDlg::WriteDemarcationResultOfDynConstLoad(void)
{
	DWORD dwReturn(0);

	// дResultOfDynConstLoad��
	DEMARCATIONRESULTOFDYNCONSTLOAD sResultOfDynConstLoad;
	ZeroMemory(&sResultOfDynConstLoad, sizeof(DEMARCATIONRESULTOFDYNCONSTLOAD));
	GetIniResultOfDynConstLoad(&sResultOfDynConstLoad);

	dwReturn = SetDboDemarcationResultOfDynConstLoad(sResultOfDynConstLoad);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynConstLoad(sResultOfDynConstLoad);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynPLHPFile(void)
{
	// ��DemarcationLog.ini��DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// дDemarcationNumber��ResultOfDynPLHP.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfDynPLHP_ini(wchPath);
	ResultOfDynPLHP_ini.SetString(L"ResultOfDynPLHP", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfDynPLHP.ini
	DEMARCATIONRESULTOFDYNPLHP sResultOfDynPLHP;
	ZeroMemory(&sResultOfDynPLHP, sizeof(DEMARCATIONRESULTOFDYNPLHP));
	GetIniResultOfDynPLHP(&sResultOfDynPLHP);
	
	// �ϲ�����
	CString strData,strError;
	if (wcscmp(sResultOfDynPLHP.wchPLHPType, L"1") == 0)	//	ASM��������
	{
		strData.Format(L"PLHP40:%s,PLHP25:%s", sResultOfDynPLHP.wchPLHP1, sResultOfDynPLHP.wchPLHP3);
	}
	else if (wcscmp(sResultOfDynPLHP.wchPLHPType, L"2") == 0)	//	VMAS��������
	{
		strData.Format(L"PLHP40:%s,PLHP24:%s", sResultOfDynPLHP.wchPLHP1, sResultOfDynPLHP.wchPLHP3);
	}
	else if (wcscmp(sResultOfDynPLHP.wchPLHPType, L"3") == 0)	//	LUGDOWN��������
	{
		strData.Format(L"PLHP80:%s,PLHP72:%s,PLHP64:%s,PLHP56:%s,PLHP48:%s,PLHP40:%s", 
			sResultOfDynPLHP.wchPLHP1, sResultOfDynPLHP.wchPLHP2, sResultOfDynPLHP.wchPLHP3, sResultOfDynPLHP.wchPLHP4, sResultOfDynPLHP.wchPLHP5, sResultOfDynPLHP.wchPLHP6);
	}
	strError.Format(L"");

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

// дDemarcationResultOfDynPLHP��
DWORD CDynDlg::WriteDemarcationResultOfDynPLHP(void)
{
	DWORD dwReturn(0);

	// дResultOfDynPLHP��
	DEMARCATIONRESULTOFDYNPLHP sResultOfDynPLHP;
	ZeroMemory(&sResultOfDynPLHP, sizeof(DEMARCATIONRESULTOFDYNPLHP));
	GetIniResultOfDynPLHP(&sResultOfDynPLHP);

	dwReturn = SetDboDemarcationResultOfDynPLHP(sResultOfDynPLHP);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynPLHP(sResultOfDynPLHP);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynForceCalFile(void)
{
	// ��DemarcationLog.ini��DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// дDemarcationNumber��ResultOfDynForceCal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceCal.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfDynForceCal_ini(wchPath);
	ResultOfDynForceCal_ini.SetString(L"ResultOfDynForceCal", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfDynForceCal.ini
	DEMARCATIONRESULTOFDYNFORCECAL sResultOfDynForceCal;
	ZeroMemory(&sResultOfDynForceCal, sizeof(DEMARCATIONRESULTOFDYNFORCECAL));
	GetIniResultOfDynForceCal(&sResultOfDynForceCal);
	
	// �ϲ�����
	CString strData,strError;
	strData.Format(L"");
	strError.Format(L"");

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

// дDemarcationResultOfDynForceCal��
DWORD CDynDlg::WriteDemarcationResultOfDynForceCal(void)
{
	DWORD dwReturn(0);

	// дResultOfDynConstLoad��
	DEMARCATIONRESULTOFDYNFORCECAL sResultOfDynForceCal;
	ZeroMemory(&sResultOfDynForceCal, sizeof(DEMARCATIONRESULTOFDYNFORCECAL));
	GetIniResultOfDynForceCal(&sResultOfDynForceCal);

	dwReturn = SetDboDemarcationResultOfDynForceCal(sResultOfDynForceCal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfDynForceCal(sResultOfDynForceCal);
	return dwReturn;
}

DWORD CDynDlg::WriteDemarcationResultOfDynForceChkFile(void)
{
	// ��DemarcationLog.ini��DemarcationNumber
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni DemarcationLog_ini(wchPath);
	CString strDemarcationNumber = DemarcationLog_ini.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	// дDemarcationNumber��ResultOfDynForceChk.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceChk.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfDynForceChk_ini(wchPath);
	ResultOfDynForceChk_ini.SetString(L"ResultOfDynForceChk", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfDynForceChk.ini
	DEMARCATIONRESULTOFDYNFORCECHK sResultOfDynForceChk;
	ZeroMemory(&sResultOfDynForceChk, sizeof(DEMARCATIONRESULTOFDYNFORCECHK));
	GetIniResultOfDynForceChk(&sResultOfDynForceChk);
	
	// �ϲ�����
	CString strData,strError;
	strData.Format(L"");
	strError.Format(L"");

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);
	wcsncpy_s(sEquCalChkInfo.wchData, _countof(sEquCalChkInfo.wchData), strData, _countof(sEquCalChkInfo.wchData)-1);
	wcsncpy_s(sEquCalChkInfo.wchError, _countof(sEquCalChkInfo.wchError), strError, _countof(sEquCalChkInfo.wchError)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	return 0x00;
}

// дDemarcationResultOfDynForceChk��
DWORD CDynDlg::WriteDemarcationResultOfDynForceChk(void)
{
	DWORD dwReturn(0);

	// дResultOfDynForceChk��
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

	// ������飬�Է����ݱ��۸�(���޶ȼ��)
	// �жϼ���߱���Ƿ�Ϊ��
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// ����߱��Ϊ��
		// ��дLineInfo
		dwReturn = 0xff;
	}
	else
	{
		// ����߱�Ų�Ϊ��
		// дLineInfo
		dwReturn = SetDboLineInfo(sLineInfo);
	}

	return dwReturn;
}

DWORD CDynDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// ������飬�Է����ݱ��۸�(���޶ȼ��)
	// �жϼ���߱���Ƿ�Ϊ��
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// ����߱��Ϊ��
		// ��дLineInfo.ini
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
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(DYN_CL);
	WriteEquCalChkInfoFile(DYN_CL);
	WriteDemarcationResultOfDynConstLoadFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();
	
	// дDemarcationLog��
	WriteDemarcationLog();

	// дDemarcationResultOfDynConstLoad��
	WriteDemarcationResultOfDynConstLoad();

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�⹦���������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// ���ػ�������
		ZeroMemory(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate));
		wcscpy_s(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// �궨not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo�⹦���������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// ���ػ�������
		//ZeroMemory(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate));
		//wcscpy_s(sLineInfo.wchDynamometerCoastDownDate, sizeof(sLineInfo.wchDynamometerCoastDownDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerCoastDownDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynConstLoad.ini");

	// �ϴ�����
	bool bRet(false);
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// ��ʱ������������
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
		AfxMessageBox(L"�ϴ��ɹ�");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"�ϴ�ʧ�ܣ�%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}

	return 0x00;
}

DWORD CDynDlg::WriteDynPLHPData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(DYN_PLHP);
	WriteEquCalChkInfoFile(DYN_PLHP);
	WriteDemarcationResultOfDynPLHPFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();
	
	// дDemarcationLog��
	WriteDemarcationLog();
	
	// дDemarcationResultOfDynPLHP��
	WriteDemarcationResultOfDynPLHP();

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�⹦���������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// �������ʻ�������
		ZeroMemory(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate));
		wcscpy_s(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else		
	{
		// �궨 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo�⹦���������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// �������ʻ�������
		//ZeroMemory(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate));
		//wcscpy_s(sLineInfo.wchDynamometerPLHPDate, sizeof(sLineInfo.wchDynamometerPLHPDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerPLHPDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynPLHP.ini");

	// �ϴ�����
	bool bRet(false);
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// ��ʱ������������
	}
	CSimpleIni si(wchPath);

	si.SetString(L"SystemCheck", L"JYY", theApp.m_sUserInfo.wchName);
	si.SetString(L"SystemCheck", L"JYYSFZH", theApp.m_sUserInfo.wchPassword);

	CString str = si.GetString(L"ResultOfDynPLHP", L"PLHPType", L"0");
	SHBMsg sHBMsg;
	if (L"3" == str) // ����
	{
		CZYHttp_PAI::GetInstance().SetDiePLHP(sHBMsg);
	}
	else // ����
	{
		CZYHttp_PAI::GetInstance().SetGasPLHP(sHBMsg);
	}

	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"�ϴ��ɹ�");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"�ϴ�ʧ�ܣ�%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}

	return 0x00;
}

DWORD CDynDlg::WriteDynForceCalData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(DYN_FORCE_CAL);
	WriteEquCalChkInfoFile(DYN_FORCE_CAL);
	WriteDemarcationResultOfDynForceCalFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();
	
	// дDemarcationLog��
	WriteDemarcationLog();
	
	// дDemarcationResultOfDynForceCal��
	WriteDemarcationResultOfDynForceCal();
	
	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�⹦���������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// ��У׼����
		ZeroMemory(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate));
		wcscpy_s(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// �궨 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo�⹦���������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// ��У׼����
		//ZeroMemory(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate));
		//wcscpy_s(sLineInfo.wchDynamometerForceCalibrationDate, sizeof(sLineInfo.wchDynamometerForceCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerForceCalibrationDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfDynForceCal.ini");
	return 0x00;
}

DWORD CDynDlg::WriteDynForceChkData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(DYN_FORCE_CHK);
	WriteEquCalChkInfoFile(DYN_FORCE_CHK);
	WriteDemarcationResultOfDynForceChkFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();
	
	// дDemarcationLog��
	WriteDemarcationLog();
	
	// дDemarcationResultOfDynForceChk��
	WriteDemarcationResultOfDynForceChk();
	
	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�⹦���������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// ���������
		ZeroMemory(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate));
		wcscpy_s(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else		
	{
		// �궨 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo�⹦���������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// ���������
		//ZeroMemory(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate));
		//wcscpy_s(sLineInfo.wchDynamometerForceCheckDate, sizeof(sLineInfo.wchDynamometerForceCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchDynamometerForceCheckDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
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
