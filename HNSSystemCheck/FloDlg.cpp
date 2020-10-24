// FloDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FloDlg.h"

// CFloDlg �Ի���

IMPLEMENT_DYNAMIC(CFloDlg, CDialogZoom)

// �궨�Ƿ�Pass
bool CFloDlg::m_bDemarcationPass = false;

CFloDlg::CFloDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFloDlg::IDD, pParent)
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


// CFloDlg ��Ϣ�������
BOOL CFloDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CFloDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"������");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterDilO2CalibrationDlg(FloDilO2CalProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonPresCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterPressureCalibrationDlg(FloPresCalProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonFluxChk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterFreeFlowCheckDlg(FloFluxChkProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonFluxCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	//DeleteEquCalChkInfoFile();

	LoadFlowmeterFlowCalibrationDlg(FloFluxCalProcessCtrlCallBack);
}

void CFloDlg::OnBnClickedButtonDelayTimeCfg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	LoadFlowmeterDelayTimeConfigDlg();
}

//void CFloDlg::DeleteEquCalChkInfoFile(void)
//{
//	// ��ȡ�ļ�·��
//	wchar_t wchPath[MAX_PATH];
//	ZeroMemory(wchPath, sizeof(wchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
//	{
//		// �ļ�������
//		return;
//	}
//	DeleteFile(wchPath);
//}
//
//DWORD CFloDlg::CheckEquCalChkInfoFile(void)
//{
//	// ����ļ��Ƿ����
//
//	// ��ȡ�ļ�·��
//	wchar_t wchPath[MAX_PATH];
//	ZeroMemory(wchPath, sizeof(wchPath));
//	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
//	{
//		// �ļ�������
//		return 0x01;
//	}
//
//	// �������Ƿ����
//
//	CSimpleIni si(wchPath);
//	CString str = si.GetString(L"EquCalChkInfo", L"IsOperationFinishedAtLeaseOnce", L"0");
//	if (L"0" == str)
//	{
//		// ����δ���
//		return 0x02;
//	}
//	else
//	{
//		// ��¼�궨�Ƿ�Pass(��������)
//		str = si.GetString(L"EquCalChkInfo", L"Judgement", L"0");
//		m_bDemarcationPass = (L"ͨ��"==str)?true:false;
//
//		return 0x00;
//	}
//}

DWORD CFloDlg::UpdateLineInfoFile(const CString strItem, const CString strItemValue)
{
	ASSERT(L"" != strItem);
	ASSERT(L"" != strItemValue);

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}

	CSimpleIni si(wchPath);
	si.SetString(L"LineInfo", strItem, strItemValue);

	return 0x00;
}

DWORD CFloDlg::WriteEquCalChkInfo(void)
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

DWORD CFloDlg::WriteEquCalChkInfoFile(int nDemarcationItem)
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
	if (nDemarcationItem == FLO_FLUXCAL)
	{
		strItem = L"����������У׼";
	}
	else if (nDemarcationItem == FLO_FLUXCHK)
	{
		strItem = L"�������������";
	}
	else if (nDemarcationItem == FLO_DILO2CAL)
	{
		strItem = L"������ϡ����У׼";
	}
	else if (nDemarcationItem == FLO_PRESCAL)
	{
		strItem = L"������ѹ��У׼";
	}
	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchFlowmeterNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchFlowmeterModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), strItem, _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	wcsncpy_s(sEquCalChkInfo.wchJudgement, _countof(sEquCalChkInfo.wchJudgement), m_bDemarcationPass ? L"ͨ��" : L"��ͨ��", _countof(sEquCalChkInfo.wchJudgement)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);	// Data��Error��ʱ��д�룬����ÿ���궨��Ŀ�����ݸ�ʽ����ͬ��������дDemarcationResult�ļ�ʱ��д��

	return 0x00;
}

DWORD CFloDlg::WriteDemarcationLogFile(int nDemarcationItem)
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
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchFlowmeterNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchFlowmeterModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"������", _countof(sDemarcationLog.wchEquipmentName)-1);
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

	// дDemarcationLog��
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

	// дDemarcationResultOfFloFluxChk��
	DEMARCATIONRESULTOFFLOFLUXCHK sResultOfFloFluxChk;
	ZeroMemory(&sResultOfFloFluxChk, sizeof(DEMARCATIONRESULTOFFLOFLUXCHK));
	GetIniResultOfFloFluxChk(&sResultOfFloFluxChk);

	dwReturn = SetDboDemarcationResultOfFloFluxChk(sResultOfFloFluxChk);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloFluxChk(sResultOfFloFluxChk);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloFluxChkFile(void)
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

	// дDemarcationNumber��ResultOfFloFluxChk.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxChk.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfFloFluxChk_ini(wchPath);
	ResultOfFloFluxChk_ini.SetString(L"ResultOfFloFluxChk", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfFloFluxChk.ini
	DEMARCATIONRESULTOFFLOFLUXCHK sResultOfFloFluxChk;
	ZeroMemory(&sResultOfFloFluxChk, sizeof(DEMARCATIONRESULTOFFLOFLUXCHK));
	GetIniResultOfFloFluxChk(&sResultOfFloFluxChk);
	
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

DWORD CFloDlg::WriteDemarcationResultOfFloFluxCal(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfFloFluxCal��
	DEMARCATIONRESULTOFFLOFLUXCAL sResultOfFloFluxCal;
	ZeroMemory(&sResultOfFloFluxCal, sizeof(DEMARCATIONRESULTOFFLOFLUXCAL));
	GetIniResultOfFloFluxCal(&sResultOfFloFluxCal);

	dwReturn = SetDboDemarcationResultOfFloFluxCal(sResultOfFloFluxCal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloFluxCal(sResultOfFloFluxCal);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloFluxCalFile(void)
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

	// дDemarcationNumber��ResultOfFloFluxCal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxCal.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfFloFluxCal_ini(wchPath);
	ResultOfFloFluxCal_ini.SetString(L"ResultOfFloFluxCal", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfFloFluxCal.ini
	DEMARCATIONRESULTOFFLOFLUXCAL sResultOfFloFluxCal;
	ZeroMemory(&sResultOfFloFluxCal, sizeof(DEMARCATIONRESULTOFFLOFLUXCAL));
	GetIniResultOfFloFluxCal(&sResultOfFloFluxCal);
	
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

DWORD CFloDlg::WriteDemarcationResultOfFloDilO2Cal(void)
{	
	DWORD dwReturn(0);

	// дDemarcationResultOfFloDilO2Cal��
	DEMARCATIONRESULTOFFLODILO2CAL sResultOfFloDilO2Cal;
	ZeroMemory(&sResultOfFloDilO2Cal, sizeof(DEMARCATIONRESULTOFFLODILO2CAL));
	GetIniResultOfFloDilO2Cal(&sResultOfFloDilO2Cal);

	dwReturn = SetDboDemarcationResultOfFloDilO2Cal(sResultOfFloDilO2Cal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloDilO2Cal(sResultOfFloDilO2Cal);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloDilO2CalFile(void)
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

	// дDemarcationNumber��ResultOfFloDilO2Cal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloDilO2Cal.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfFloDilO2Cal_ini(wchPath);
	ResultOfFloDilO2Cal_ini.SetString(L"ResultOfFloDilO2Cal", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfFloDilO2Cal.ini
	DEMARCATIONRESULTOFFLODILO2CAL sResultOfFloDilO2Cal;
	ZeroMemory(&sResultOfFloDilO2Cal, sizeof(DEMARCATIONRESULTOFFLODILO2CAL));
	GetIniResultOfFloDilO2Cal(&sResultOfFloDilO2Cal);
	
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

DWORD CFloDlg::WriteDemarcationResultOfFloPresCal(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfFloPresCal��
	DEMARCATIONRESULTOFFLOPRESCAL sResultOfFloPresCal;
	ZeroMemory(&sResultOfFloPresCal, sizeof(DEMARCATIONRESULTOFFLOPRESCAL));
	GetIniResultOfFloPresCal(&sResultOfFloPresCal);

	dwReturn = SetDboDemarcationResultOfFloPresCal(sResultOfFloPresCal);
	//dwReturn = CNHModeClientDB_SQLite::SetDboDemarcationResultOfFloPresCal(sResultOfFloPresCal);
	return dwReturn;
}

DWORD CFloDlg::WriteDemarcationResultOfFloPresCalFile(void)
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

	// дDemarcationNumber��ResultOfFloPresCal.ini
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloPresCal.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}
	CSimpleIni ResultOfFloPresCal_ini(wchPath);
	ResultOfFloPresCal_ini.SetString(L"ResultOfFloPresCal", L"DemarcationNumber", strDemarcationNumber);

	// ��ȡDemarcationResultOfFloPresCal.ini
	DEMARCATIONRESULTOFFLOPRESCAL sResultOfFloPresCal;
	ZeroMemory(&sResultOfFloPresCal, sizeof(DEMARCATIONRESULTOFFLOPRESCAL));
	GetIniResultOfFloPresCal(&sResultOfFloPresCal);
	
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

DWORD CFloDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
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

DWORD CFloDlg::WriteFloDilO2CalData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(FLO_DILO2CAL);
	WriteEquCalChkInfoFile(FLO_DILO2CAL);
	WriteDemarcationResultOfFloDilO2CalFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();

	// дDemarcationLog��
	WriteDemarcationLog();

	// ���ݿ⻹û����Ӧ�������δ˶δ���
	//// дDemarcationResultOfFloDilO2Cal��
	//if (0x00 != WriteDemarcationResultOfFloDilO2Cal())
	//{
	//	// дDemarcationResultOfFloDilO2Cal��ʧ��
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�������������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate));
		wcscpy_s(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate)/sizeof(wchar_t), str);

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

		//// ����sLineInfo�������������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate));
		//wcscpy_s(sLineInfo.wchFlowmeterDilO2CalibrationDate, sizeof(sLineInfo.wchFlowmeterDilO2CalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterDilO2CalibrationDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

DWORD CFloDlg::WriteFloPresCalData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(FLO_PRESCAL);
	WriteEquCalChkInfoFile(FLO_PRESCAL);
	WriteDemarcationResultOfFloPresCalFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();

	// дDemarcationLog��
	WriteDemarcationLog();

	// ���ݿ⻹û����Ӧ�������δ˶δ���
	//// дDemarcationResultOfFloPresCal��
	//if (0x00 != WriteDemarcationResultOfFloPresCal())
	//{
	//	// дDemarcationResultOfFloPresCal��ʧ��
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�������������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate));
		wcscpy_s(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate)/sizeof(wchar_t), str);

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

		//// ����sLineInfo�������������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate));
		//wcscpy_s(sLineInfo.wchFlowmeterPressureCalibrationDate, sizeof(sLineInfo.wchFlowmeterPressureCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterPressureCalibrationDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

DWORD CFloDlg::WriteFloFluxChkData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(FLO_FLUXCHK);
	WriteEquCalChkInfoFile(FLO_FLUXCHK);
	WriteDemarcationResultOfFloFluxChkFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();

	// дDemarcationLog��
	WriteDemarcationLog();
	
	// ���ݿ⻹û����Ӧ�������δ˶δ���
	//// дDemarcationResultOfFloFluxChk��
	//if (0x00 != WriteDemarcationResultOfFloFluxChk())
	//{
	//	// дDemarcationResultOfFloFluxChk��ʧ��
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�������������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate));
		wcscpy_s(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate)/sizeof(wchar_t), str);

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

		//// ����sLineInfo�������������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate));
		//wcscpy_s(sLineInfo.wchFlowmeterFluxCheckDate, sizeof(sLineInfo.wchFlowmeterFluxCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterFluxCheckDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

DWORD CFloDlg::WriteFloFluxCalData(void)
{
	// �Ⱥϳ�ini�ļ�
	WriteDemarcationLogFile(FLO_FLUXCAL);
	WriteEquCalChkInfoFile(FLO_FLUXCAL);
	WriteDemarcationResultOfFloFluxCalFile();

	// дEquCalChkInfo��
	WriteEquCalChkInfo();

	// дDemarcationLog��
	WriteDemarcationLog();

	// ���ݿ⻹û����Ӧ�������δ˶δ���
	//// дDemarcationResultOfFloFluxCal��
	//if (0x00 != WriteDemarcationResultOfFloFluxCal())
	//{
	//	// дDemarcationResultOfFloFluxCal��ʧ��
	//	return 0xff;
	//}

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo�������������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		ZeroMemory(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate));
		wcscpy_s(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo�������������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//ZeroMemory(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate));
		//wcscpy_s(sLineInfo.wchFlowmeterFluxCalibrationDate, sizeof(sLineInfo.wchFlowmeterFluxCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchFlowmeterFluxCalibrationDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}

	return 0x00;
}

// ����У׼���̿��ƻص�����
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

// ���������̿��ƻص�����
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

// ѹ��У׼���̿��ƻص�����
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

// ϡ����У׼���̿��ƻص�����
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
