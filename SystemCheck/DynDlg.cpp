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

	CString str = si.GetString(L"LowerRangeLoadConst", L"RangeLoadConstType", L"1");
	if (L"3" == str) // LUGDOWN
	{
		bRet = UpConstLoadHeavy();
	}
	else // 
	{
		bRet = UpConstLoad();
	}
	if (bRet)
	{
		bRet = UpEqulChk(1);
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

	CString str = si.GetString(L"ResultOfDynPLHP", L"PLHPType", L"0");
	if (L"3" == str) // ����
	{
		bRet = UpPLHPHeavy();
	}
	else // ����
	{
		bRet = UpPLHP();
	}
	bRet = UpEqulChk(2);

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


bool CDynDlg::UpConstLoad(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�ϴ����ػ����ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"�ϴ����ػ����ļ�INI����");
		return false;
	}
	CSimpleIni si(wchPath);

	CString strLowerRangeLoadConstFinish, strHigherRangeLoadConstFinish;

	strLowerRangeLoadConstFinish = si.GetString(L"LowerRangeLoadConst", L"LowerRangeLoadConstFinish", L"0");
	strHigherRangeLoadConstFinish = si.GetString(L"HigherRangeLoadConst", L"HigherRangeLoadConstFinish", L"0");

	if (strLowerRangeLoadConstFinish != L"1" 
		|| strHigherRangeLoadConstFinish != L"1"
		)
	{
		//AfxMessageBox(L"����һ������δ���");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����һ������δ���");
		return false;
	}

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	���������
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	����߱�� 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	�������
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//hxjckssj	���м�鿪ʼʱ��
	// �ϴ����ʱ����ĸ�
	COleDateTime otd1, otd2;
	strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd2.ParseDateTime(strTemp))
	{
		otd2 = COleDateTime::GetCurrentTime();
	}
	if (otd1 > otd2)
	{
		strData.AppendFormat(L"<hxjckssj>%s</hxjckssj>", otd2.Format(L"%Y%m%d%H%M%S"));
	}
	else
	{
		strData.AppendFormat(L"<hxjckssj>%s</hxjckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	}
	//jbgl	��������
	strTemp = si.GetString(L"LowerRangeLoadConst", L"DIW", L"0");
	strData.AppendFormat(L"<jbgl>%s</jbgl>", strTemp);
	//ihpgl1	IHP2540���ù���
	strTemp = si.GetString(L"HigherRangeLoadConst", L"PowerStandardValue", L"10");
	strData.AppendFormat(L"<ihpgl1>%s</ihpgl1>", strTemp);
	//fjssgl1	40km/hʱ�ĸ�����ʧ����
	strTemp = si.GetString(L"HigherRangeLoadConst", L"InternalDamage", L"0.2");
	strData.AppendFormat(L"<fjssgl1>%s</fjssgl1>", strTemp);
	//sjhxsj1	50-30km/hʵ�ʻ���ʱ��
	strTemp = si.GetString(L"HigherRangeLoadConst", L"ACDT", L"");
	strData.AppendFormat(L"<sjhxsj1>%s</sjhxsj1>", strTemp);
	//myhxsj1	50-30km/h���廬��ʱ��
	strTemp = si.GetString(L"HigherRangeLoadConst", L"CCDT", L"");
	strData.AppendFormat(L"<myhxsj1>%s</myhxsj1>", strTemp);
	//ihpgl2	IHP5025���ù���
	strTemp = si.GetString(L"LowerRangeLoadConst", L"PowerStandardValue", L"8");
	strData.AppendFormat(L"<ihpgl2>%s</ihpgl2>", strTemp);
	//fjssgl2	25km/hʱ�ĸ�����ʧ����
	strTemp = si.GetString(L"LowerRangeLoadConst", L"InternalDamage", L"0.2");
	strData.AppendFormat(L"<fjssgl2>%s</fjssgl2>", strTemp);
	//sjhxsj2	35-15km/hʵ�ʻ���ʱ��
	strTemp = si.GetString(L"LowerRangeLoadConst", L"ACDT", L"");
	strData.AppendFormat(L"<sjhxsj2>%s</sjhxsj2>", strTemp);
	//myhxsj2	35-15km/h���廬��ʱ��
	strTemp = si.GetString(L"LowerRangeLoadConst", L"CCDT", L"");
	strData.AppendFormat(L"<myhxsj2>%s</myhxsj2>", strTemp);
	//hxjcjg1	50-30km/h���м����
	strTemp = si.GetString(L"HigherRangeLoadConst", L"Pass", L"1");
	strData.AppendFormat(L"<hxjcjg1>%s</hxjcjg1>", strTemp);
	strPass = strTemp;
	//hxjcjg2	35-15km/h���м����
	strTemp = si.GetString(L"LowerRangeLoadConst", L"Pass", L"1");
	strData.AppendFormat(L"<hxjcjg2>%s</hxjcjg2>", strTemp);
	//pdjg	�ж����
	if (strPass == L"1"
		&& strTemp == L"1"
		)
	{
		strData.AppendFormat(L"<pdjg>%s</pdjg>", L"1");
	}
	else
	{
		strData.AppendFormat(L"<pdjg>%s</pdjg>", L"0");
	}
	//jcry	�����Ա
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

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W01", strData.GetString(), strRet);

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
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����ʧ��");
		return false;
	}

	return true;
}

bool CDynDlg::UpConstLoadHeavy(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�ϴ����ػ����ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"�ϴ����ػ����ļ�INI����");
		return false;
	}
	CSimpleIni si(wchPath);

	CString strLowerRangeLoadConstFinish, strHigherRangeLoadConstFinish;

	strLowerRangeLoadConstFinish = si.GetString(L"LowerRangeLoadConst", L"LowerRangeLoadConstFinish", L"0");
	strHigherRangeLoadConstFinish = si.GetString(L"HigherRangeLoadConst", L"HigherRangeLoadConstFinish", L"0");

	if (strLowerRangeLoadConstFinish != L"1" 
		|| strHigherRangeLoadConstFinish != L"1"
		)
	{
		//AfxMessageBox(L"����һ������δ���");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����һ������δ���");
		return false;
	}

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	���������	�ַ�
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	����߱�� 	��ֵ
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	�������	����
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//hxjckssj	���м�鿪ʼʱ��	ʱ��
	COleDateTime otd1, otd2;
	strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd2.ParseDateTime(strTemp))
	{
		otd2 = COleDateTime::GetCurrentTime();
	}
	if (otd1 > otd2)
	{
		strData.AppendFormat(L"<hxjckssj>%s</hxjckssj>", otd2.Format(L"%Y%m%d%H%M%S"));
	}
	else
	{
		strData.AppendFormat(L"<hxjckssj>%s</hxjckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	}
	//jbgl	��������	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"DIW", L"0");
	strData.AppendFormat(L"<jbgl>%s</jbgl>", strTemp);

	//sjhxsj301	IHP30kw 100-80km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT300", L"0");
	strData.AppendFormat(L"<sjhxsj301>%s</sjhxsj301>", strTemp);
	//sjhxsj302	IHP30kw 90-70km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT301", L"0");
	strData.AppendFormat(L"<sjhxsj302>%s</sjhxsj302>", strTemp);
	//sjhxsj303	IHP30kw 80-60km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT302", L"0");
	strData.AppendFormat(L"<sjhxsj303>%s</sjhxsj303>", strTemp);
	//sjhxsj304	IHP30kw 70-50km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT303", L"0");
	strData.AppendFormat(L"<sjhxsj304>%s</sjhxsj304>", strTemp);
	//sjhxsj305	IHP30kw 60-40km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT304", L"0");
	strData.AppendFormat(L"<sjhxsj305>%s</sjhxsj305>", strTemp);
	//sjhxsj306	IHP30kw 50-30km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT305", L"0");
	strData.AppendFormat(L"<sjhxsj306>%s</sjhxsj306>", strTemp);
	//sjhxsj307	IHP30kw 40-20km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT306", L"0");
	strData.AppendFormat(L"<sjhxsj307>%s</sjhxsj307>", strTemp);
	//sjhxsj308	IHP30kw 30-10km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT307", L"0");
	strData.AppendFormat(L"<sjhxsj308>%s</sjhxsj308>", strTemp);
	//myhxsj301	IHP30kw 100-80km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT300", L"0");
	strData.AppendFormat(L"<myhxsj301>%s</myhxsj301>", strTemp);
	//myhxsj302	IHP30kw 90-70km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT301", L"0");
	strData.AppendFormat(L"<myhxsj302>%s</myhxsj302>", strTemp);
	//myhxsj303	IHP30kw 80-60km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT302", L"0");
	strData.AppendFormat(L"<myhxsj303>%s</myhxsj303>", strTemp);
	//myhxsj304	IHP30kw 70-50km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT303", L"0");
	strData.AppendFormat(L"<myhxsj304>%s</myhxsj304>", strTemp);
	//myhxsj305	IHP30kw 60-40km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT304", L"0");
	strData.AppendFormat(L"<myhxsj305>%s</myhxsj305>", strTemp);
	//myhxsj306	IHP30kw 50-30km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT305", L"0");
	strData.AppendFormat(L"<myhxsj306>%s</myhxsj306>", strTemp);
	//myhxsj307	IHP30kw 40-20km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT306", L"0");
	strData.AppendFormat(L"<myhxsj307>%s</myhxsj307>", strTemp);
	//myhxsj308	IHP30kw 30-10km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT307", L"0");
	strData.AppendFormat(L"<myhxsj308>%s</myhxsj308>", strTemp);
	//sjhxsj201	IHP20kw 100-80km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT200", L"0");
	strData.AppendFormat(L"<sjhxsj201>%s</sjhxsj201>", strTemp);
	//sjhxsj202	IHP20kw 80-60km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT201", L"0");
	strData.AppendFormat(L"<sjhxsj202>%s</sjhxsj202>", strTemp);
	//sjhxsj203	IHP20kw 70-50km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT202", L"0");
	strData.AppendFormat(L"<sjhxsj203>%s</sjhxsj203>", strTemp);
	//sjhxsj204	IHP20kw 60-40km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT203", L"0");
	strData.AppendFormat(L"<sjhxsj204>%s</sjhxsj204>", strTemp);
	//sjhxsj205	IHP20kw 50-30km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT204", L"0");
	strData.AppendFormat(L"<sjhxsj205>%s</sjhxsj205>", strTemp);
	//sjhxsj206	IHP20kw 40-20km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT205", L"0");
	strData.AppendFormat(L"<sjhxsj206>%s</sjhxsj206>", strTemp);
	//sjhxsj207	IHP20kw 30-10km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT206", L"0");
	strData.AppendFormat(L"<sjhxsj207>%s</sjhxsj207>", strTemp);
	//sjhxsj208	IHP20kw 30-10km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT207", L"0");
	strData.AppendFormat(L"<sjhxsj208>%s</sjhxsj208>", strTemp);
	//myhxsj201	IHP20kw 100-80km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT200", L"0");
	strData.AppendFormat(L"<myhxsj201>%s</myhxsj201>", strTemp);
	//myhxsj202	IHP20kw 90-70km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT201", L"0");
	strData.AppendFormat(L"<myhxsj202>%s</myhxsj202>", strTemp);
	//myhxsj203	IHP20kw 80-60km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT202", L"0");
	strData.AppendFormat(L"<myhxsj203>%s</myhxsj203>", strTemp);
	//myhxsj204	IHP20kw 70-50km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT203", L"0");
	strData.AppendFormat(L"<myhxsj204>%s</myhxsj204>", strTemp);
	//myhxsj205	IHP20kw 60-40km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT204", L"0");
	strData.AppendFormat(L"<myhxsj205>%s</myhxsj205>", strTemp);
	//myhxsj206	IHP20kw 50-30km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT205", L"0");
	strData.AppendFormat(L"<myhxsj206>%s</myhxsj206>", strTemp);
	//myhxsj207	IHP20kw 40-20km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT206", L"0");
	strData.AppendFormat(L"<myhxsj207>%s</myhxsj207>", strTemp);
	//myhxsj208	IHP20kw 30-10km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT207", L"0");
	strData.AppendFormat(L"<myhxsj208>%s</myhxsj208>", strTemp);
	//sjhxsj101	IHP10kw 100-80km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT100", L"0");
	strData.AppendFormat(L"<sjhxsj101>%s</sjhxsj101>", strTemp);
	//sjhxsj102	IHP10kw 90-70km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT101", L"0");
	strData.AppendFormat(L"<sjhxsj102>%s</sjhxsj102>", strTemp);
	//sjhxsj103	IHP10kw 80-60km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT102", L"0");
	strData.AppendFormat(L"<sjhxsj103>%s</sjhxsj103>", strTemp);
	//sjhxsj104	IHP10kw 70-50km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT103", L"0");
	strData.AppendFormat(L"<sjhxsj104>%s</sjhxsj104>", strTemp);
	//sjhxsj105	IHP10kw 60-40km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT104", L"0");
	strData.AppendFormat(L"<sjhxsj105>%s</sjhxsj105>", strTemp);
	//sjhxsj106	IHP10kw 50-30km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT105", L"0");
	strData.AppendFormat(L"<sjhxsj106>%s</sjhxsj106>", strTemp);
	//sjhxsj107	IHP10kw 40-20km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT106", L"0");
	strData.AppendFormat(L"<sjhxsj107>%s</sjhxsj107>", strTemp);
	//sjhxsj108	IHP10kw 30-10km/hʵ�ʻ���ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT107", L"0");
	strData.AppendFormat(L"<sjhxsj108>%s</sjhxsj108>", strTemp);
	//myhxsj101	IHP10kw 100-80km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT100", L"0");
	strData.AppendFormat(L"<myhxsj101>%s</myhxsj101>", strTemp);
	//myhxsj102	IHP10kw 90-70km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT101", L"0");
	strData.AppendFormat(L"<myhxsj102>%s</myhxsj102>", strTemp);
	//myhxsj103	IHP10kw 80-60km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT102", L"0");
	strData.AppendFormat(L"<myhxsj103>%s</myhxsj103>", strTemp);
	//myhxsj104	IHP10kw 70-50km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT103", L"0");
	strData.AppendFormat(L"<myhxsj104>%s</myhxsj104>", strTemp);
	//myhxsj105	IHP10kw 60-40km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT104", L"0");
	strData.AppendFormat(L"<myhxsj105>%s</myhxsj105>", strTemp);
	//myhxsj106	IHP10kw 50-30km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT105", L"0");
	strData.AppendFormat(L"<myhxsj106>%s</myhxsj106>", strTemp);
	//myhxsj107	IHP10kw 40-20km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT106", L"0");
	strData.AppendFormat(L"<myhxsj107>%s</myhxsj107>", strTemp);
	//myhxsj108	IHP10kw 30-10km/h���廬��ʱ��	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT107", L"0");
	strData.AppendFormat(L"<myhxsj108>%s</myhxsj108>", strTemp);
	//fjssgl1	90km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP0", L"0");
	strData.AppendFormat(L"<fjssgl1>%s</fjssgl1>", strTemp);
	//fjssgl2	80km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP1", L"0");
	strData.AppendFormat(L"<fjssgl2>%s</fjssgl2>", strTemp);
	//fjssgl3	70km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP2", L"0");
	strData.AppendFormat(L"<fjssgl3>%s</fjssgl3>", strTemp);
	//fjssgl4	60km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP3", L"0");
	strData.AppendFormat(L"<fjssgl4>%s</fjssgl4>", strTemp);
	//fjssgl5	50km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP4", L"0");
	strData.AppendFormat(L"<fjssgl5>%s</fjssgl5>", strTemp);
	//fjssgl6	40km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP5", L"0");
	strData.AppendFormat(L"<fjssgl6>%s</fjssgl6>", strTemp);
	//fjssgl7	30km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP6", L"0");
	strData.AppendFormat(L"<fjssgl7>%s</fjssgl7>", strTemp);
	//fjssgl8	20km/h������ʧ����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP7", L"0");
	strData.AppendFormat(L"<fjssgl8>%s</fjssgl8>", strTemp);
	//hxjcjg1	100-10km/h���м����	��ֵ
	strTemp = si.GetString(L"HigherRangeLoadConst", L"Pass", L"1");
	strData.AppendFormat(L"<hxjcjg1>%s</hxjcjg1>", strTemp);
	strPass = strTemp;
	//hxjcjg2	80-10km/h���м����	��ֵ
	strTemp = si.GetString(L"LowerRangeLoadConst", L"Pass", L"1");
	strData.AppendFormat(L"<hxjcjg2>%s</hxjcjg2>", strTemp);
	//pdjg	�ж����	��ֵ
	if (strPass == L"1"
		&& strTemp == L"1"
		)
	{
		strData.AppendFormat(L"<pdjg>%s</pdjg>", L"1");
	}
	else
	{
		strData.AppendFormat(L"<pdjg>%s</pdjg>", L"0");
	}
	//jcry	�����Ա	�ַ�
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

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W03", strData.GetString(), strRet);

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
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����ʧ��");
		return false;
	}
	return true;
}

bool CDynDlg::UpPLHP(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�ϴ����ػ����ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���������ϴ�", L"�ϴ����������ļ�INI����");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	���������
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	����߱�� 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	�������
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//fjsssj	������ʧ��ʼʱ��
	COleDateTime otd;
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd.ParseDateTime(strTemp))
	{
		otd = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<fjsssj>%s</fjsssj>", otd.Format(L"%Y%m%d%H%M%S"));
	//hxjcjssj	���м�����ʱ��
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd.ParseDateTime(strTemp))
	{
		otd = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<hxjcjssj>%s</hxjcjssj>", otd.Format(L"%Y%m%d%H%M%S"));
	//jbgl	��������
	strTemp = si.GetString(L"ResultOfDynPLHP", L"DIW", L"0");
	strData.AppendFormat(L"<jbgl>%s</jbgl>", strTemp);

	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
		if (strTemp == L"30")
		{
			//sjhxsj1	50-30km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			strData.AppendFormat(L"<sjhxsj1>%s</sjhxsj1>", strTemp);
			//fjssgl1	40km/hʱ�ĸ�����ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			strData.AppendFormat(L"<fjssgl1>%s</fjssgl1>", strTemp);
		}
		if (strTemp == L"20" || strTemp == L"15")
		{
			//sjhxsj2	35-15km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			strData.AppendFormat(L"<sjhxsj2>%s</sjhxsj2>", strTemp);
			//fjssgl2	25km/hʱ������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			strData.AppendFormat(L"<fjssgl2>%s</fjssgl2>", strTemp);
		}
	}
	//odjg	�ж����
	strTemp = si.GetString(L"ResultOfDynPLHP", L"Pass", L"1");
	strData.AppendFormat(L"<odjg>%s</odjg>", strTemp);
	//jcry	�����Ա
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

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W02", strData.GetString(), strRet);

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
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����ʧ��");
		return false;
	}

	return true;
}

bool CDynDlg::UpPLHPHeavy(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�ϴ����ػ����ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���������ϴ�", L"�ϴ����������ļ�INI����");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	���������
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	����߱�� 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	�������
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//fjsssj	������ʧ��ʼʱ��
	COleDateTime otd;
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd.ParseDateTime(strTemp))
	{
		otd = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<fjsssj>%s</fjsssj>", otd.Format(L"%Y%m%d%H%M%S"));
	//hxjcjssj	���м�����ʱ��
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd.ParseDateTime(strTemp))
	{
		otd = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<hxjcjssj>%s</hxjcjssj>", otd.Format(L"%Y%m%d%H%M%S"));

	float fV820(0.0f), fPL(0.0f);
	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
		if (strTemp == L"60")
		{
			//sjhxsj1	80-60km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fV820 += _wtof(strTemp);
			strData.AppendFormat(L"<sjhxsj1>%s</sjhxsj1>", strTemp);
			//fjssgl2	70km/h������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fPL += _wtof(strTemp);
			strData.AppendFormat(L"<fjssgl2>%s</fjssgl2>", strTemp);
		}
		if (strTemp == L"50")
		{
			//sjhxsj2	70-50km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fV820 += _wtof(strTemp);
			strData.AppendFormat(L"<sjhxsj2>%s</sjhxsj2>", strTemp);
			//fjssgl3	60km/h������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fPL += _wtof(strTemp);
			strData.AppendFormat(L"<fjssgl3>%s</fjssgl3>", strTemp);
		}
		if (strTemp == L"40")
		{
			//sjhxsj3	60-40km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fV820 += _wtof(strTemp);
			strData.AppendFormat(L"<sjhxsj3>%s</sjhxsj3>", strTemp);
			//fjssgl4	50km/h������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fPL += _wtof(strTemp);
			strData.AppendFormat(L"<fjssgl4>%s</fjssgl4>", strTemp);
		}
		if (strTemp == L"30")
		{
			//sjhxsj4	50-30km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fV820 += _wtof(strTemp);
			strData.AppendFormat(L"<sjhxsj4>%s</sjhxsj4>", strTemp);
			//fjssgl5	40km/h������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fPL += _wtof(strTemp);
			strData.AppendFormat(L"<fjssgl5>%s</fjssgl5>", strTemp);
		}
		if (strTemp == L"20")
		{
			//sjhxsj5	40-20km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fV820 += _wtof(strTemp);
			strData.AppendFormat(L"<sjhxsj5>%s</sjhxsj5>", strTemp);
			//fjssgl6	30km/h������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fPL += _wtof(strTemp);
			strData.AppendFormat(L"<fjssgl6>%s</fjssgl6>", strTemp);
		}
		if (strTemp == L"10")
		{
			//sjhxsj6	30-10km/hʵ�ʻ���ʱ��
			str.Format(L"PLHPTime%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fV820 += _wtof(strTemp);
			strData.AppendFormat(L"<sjhxsj6>%s</sjhxsj6>", strTemp);
			//fjssgl7	20km/h������ʧ����
			str.Format(L"PLHP%d", i);
			strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");
			fPL += _wtof(strTemp);
			strData.AppendFormat(L"<fjssgl7>%s</fjssgl7>", strTemp);
		}
	}
	//sjhxsj7	20-80km/hʵ�ʻ���ʱ��
	strTemp.Format(L"%.0f", fV820);
	strData.AppendFormat(L"<sjhxsj7>%s</sjhxsj7>", strTemp);
	//fjssgl1	80km/h������ʧ����
	strTemp.Format(L"%.0f", fPL);
	strData.AppendFormat(L"<fjssgl1>%s</fjssgl1>", strTemp);

	//jbgl	��������
	strTemp = si.GetString(L"ResultOfDynPLHP", L"DIW", L"0");
	strData.AppendFormat(L"<jbgl>%s</jbgl>", strTemp);

	//odjg	�ж����
	strTemp = si.GetString(L"ResultOfDynPLHP", L"Pass", L"1");
	strData.AppendFormat(L"<odjg>%s</odjg>", strTemp);
	//jcry	�����Ա
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

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W04", strData.GetString(), strRet);

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
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����ʧ��");
		return false;
	}

	return true;
}

bool CDynDlg::UpEqulChk(const int& njclx)
{
	COleDateTime odtStart, odtEnd;
	switch(njclx)
	{
	case 1:
		{
			// ���ػ���
			// ��ȡ���ػ��н���ļ�
			wchar_t wchPath[MAX_PATH];
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
			{
				// �ļ�������
				AfxMessageBox(L"�ϴ����ػ����ļ�INI����");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"�ϴ����ػ����ļ�INI����");
				return false;
			}
			CSimpleIni si(wchPath);

			COleDateTime otd1, otd2;
			CString strTemp;
			strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			if (!otd1.ParseDateTime(strTemp))
			{
				otd1 = COleDateTime::GetCurrentTime();
			}
			strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			if (!otd2.ParseDateTime(strTemp))
			{
				otd2 = COleDateTime::GetCurrentTime();
			}
			if (otd1 > otd2)
			{
				odtStart = otd2;
			}
			else
			{
				odtStart = otd1;
			}

			strTemp = si.GetString(L"LowerRangeLoadConst", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			if (!otd1.ParseDateTime(strTemp))
			{
				otd1 = COleDateTime::GetCurrentTime();
			}
			strTemp = si.GetString(L"HigherRangeLoadConst", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			if (!otd2.ParseDateTime(strTemp))
			{
				otd2 = COleDateTime::GetCurrentTime();
			}
			if (otd1 > otd2)
			{
				odtEnd = otd2;
			}
			else
			{
				odtEnd = otd1;
			}
		}
		break;
	case 2:
		{
			// ��ȡ���ػ��н���ļ�
			wchar_t wchPath[MAX_PATH];
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
			{
				// �ļ�������
				AfxMessageBox(L"�ϴ����ػ����ļ�INI����");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���������ϴ�", L"�ϴ����������ļ�INI����");
				return false;
			}
			CSimpleIni si(wchPath);

			CString strTemp;
			strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			if (!odtStart.ParseDateTime(strTemp))
			{
				odtStart = COleDateTime::GetCurrentTime();
			}

			strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			if (!odtEnd.ParseDateTime(strTemp))
			{
				odtEnd = COleDateTime::GetCurrentTime();
			}
		}
		break;
	default:
		{
			AfxMessageBox(L"δ֪�ļ������");
			CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"����¼�ϴ�", L"δ֪�ļ������");
			return false;
		}
		break;
	}

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");
	//tsNo	���������
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	����߱�� 
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	�������
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//sbjclx	�豸������� 1-���ػ��� 2-������ʧ 3-�����飨�ͱ�����4-�����飨������5-�����飨�߱����� 6-�����
	strData.AppendFormat(L"<sbjclx>%d</sbjclx>", njclx);
	//jckssj	��鿪ʼʱ��
	strData.AppendFormat(L"<jckssj>%s</jckssj>", odtStart.Format(L"%Y%m%d%H%M%S"));
	//jcjssj	������ʱ��
	strData.AppendFormat(L"<jcjssj>%s</jcjssj>", odtEnd.Format(L"%Y%m%d%H%M%S"));
	//cysx	����ʱ��
	strData.AppendFormat(L"<cysx>%s</cysx>", L"1");
	//zgzs	ת��ת��
	strData.AppendFormat(L"<zgzs>%s</zgzs>", L"");
	//cgjjzfh	�⹦�����ظ���
	strData.AppendFormat(L"<cgjjzfh>%s</cgjjzfh>", L"");
	//hc	HCŨ��
	strData.AppendFormat(L"<hc>%s</hc>", L"");
	//co	COŨ��
	strData.AppendFormat(L"<co>%s</co>", L"");
	//no	NOŨ��
	strData.AppendFormat(L"<no>%s</no>", L"");
	//no2	NO2Ũ��
	strData.AppendFormat(L"<no2>%s</no2>", L"");
	//co2	CO2Ũ��
	strData.AppendFormat(L"<co2>%s</co2>", L"");
	//o2	O2Ũ��
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
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"����¼�ϴ�", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"����¼�ϴ�", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"����¼�ϴ�", L"����ʧ��");
		return false;
	}

	return true;
}