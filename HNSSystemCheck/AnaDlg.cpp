// AnaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AnaDlg.h"

// CAnaDlg �Ի���

IMPLEMENT_DYNAMIC(CAnaDlg, CDialogZoom)

// �궨�Ƿ�Pass
bool CAnaDlg::m_bDemarcationPass = false;

CAnaDlg::CAnaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CAnaDlg::IDD, pParent)
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


// CAnaDlg ��Ϣ�������
BOOL CAnaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CAnaDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"������(����)");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	DeleteEquCalChkInfoFile();

	//ShowNHA503XGasCalibrationDlg(AnaCalProcessCtrlCallBack);
	ShowNHAMPGasCalibrationDlg(AnaCalProcessCtrlCallBack);
}

void CAnaDlg::OnBnClickedButtonGasChk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	DeleteEquCalChkInfoFile();

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// �ļ�������
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
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// �ļ�������
		return;
	}
	DeleteFile(wchPath);
}

DWORD CAnaDlg::CheckEquCalChkInfoFile(void)
{
	// ����ļ��Ƿ����

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}

	// �������Ƿ����

	CSimpleIni si(wchPath);
	CString str = si.GetString(L"EquCalChkInfo", L"IsOperationFinishedAtLeaseOnce", L"0");
	if (L"0" == str)
	{
		// ����δ���
		return 0x02;
	}
	else
	{
		// ��¼�궨�Ƿ�Pass(��������)
		str = si.GetString(L"EquCalChkInfo", L"Judgement", L"0");
		m_bDemarcationPass = (L"ͨ��"==str)?true:false;

		return 0x00;
	}
}

DWORD CAnaDlg::UpdateLineInfoFile(const CString strItem, const CString strItemValue)
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

DWORD CAnaDlg::WriteEquCalChkInfo(void)
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

DWORD CAnaDlg::WriteDemarcationLog(void)
{
	DWORD dwReturn(0);

	// дDemarcationLog��
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}

DWORD CAnaDlg::WriteDemarcationResultOfAnaGasCal(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfAnaGasCal��
	DEMARCATIONRESULTOFANAGASCAL sResultOfAnaGasCal;
	ZeroMemory(&sResultOfAnaGasCal, sizeof(DEMARCATIONRESULTOFANAGASCAL));
	GetIniResultOfAnaGasCal(&sResultOfAnaGasCal);

	dwReturn = SetDboDemarcationResultOfAnaGasCal(sResultOfAnaGasCal);
	return dwReturn;
}

DWORD CAnaDlg::WriteDemarcationResultOfAnaGasChk(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfAnaGasChk��
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

DWORD CAnaDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
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

DWORD CAnaDlg::WriteAnaGasChkData(void)
{
	// Ԥ���EquCalChkinfo.ini�ļ�
	// �����Ŀ��
	// 1)�ļ��Ƿ����
	// 2)�����Ƿ����
	if (0x00 != CheckEquCalChkInfoFile())
	{
		// ��鲻ͨ��
		return 0xff;
	}
	// дEquCalChkInfo��
	WriteEquCalChkInfo();
	
	// дDemarcationLog��
	WriteDemarcationLog();

	// дDemarcationResultOfAnaGasChk��
	WriteDemarcationResultOfAnaGasChk();

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
		// ����������
		ZeroMemory(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate));
		wcscpy_s(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate)/sizeof(wchar_t), str);

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

		//// ����sLineInfo�������������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// ����������
		//ZeroMemory(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate));
		//wcscpy_s(sLineInfo.wchAnalyzerChannelCheckDate, sizeof(sLineInfo.wchAnalyzerChannelCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchAnalyzerChannelCheckDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// ��ʱ������������
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
	// ����EquCalChkInfo.ini�ļ�ʹ����ϣ���Ҫɾ��
	DeleteEquCalChkInfoFile();
	return 0x00;
}

DWORD CAnaDlg::WriteAnaGasCalData(void)
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

	// ��ȡ�궨���
	DEMARCATIONRESULTOFANAGASCAL sResultOfAnaGasCal;
	ZeroMemory(&sResultOfAnaGasCal, sizeof(sResultOfAnaGasCal));
	GetIniResultOfAnaGasCal(&sResultOfAnaGasCal);

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasCal.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}

	CSimpleIni si(wchPath);
	CString strDemarcationDate = si.GetString(L"ResultOfAnaGasCal", L"DemarcationDate", L"");

	// ���ɱ궨���
	COleDateTime oleDateTime;
	oleDateTime.ParseDateTime(strDemarcationDate);
	CString strDemarcationNumber;
	strDemarcationNumber.Format(L"%s-%s-%s", sStationInfo.wchStationNumber, sLineInfo.wchLineNumber, oleDateTime.Format(L"%Y%m%d%H%M%S"));
	wcsncpy_s(sResultOfAnaGasCal.wchDemarcationNumber, _countof(sResultOfAnaGasCal.wchDemarcationNumber), strDemarcationNumber, _countof(sResultOfAnaGasCal.wchDemarcationNumber)-1);
	
	// дDemarcationNumber��ResultOfAnaGasCal.ini
	si.SetString(L"ResultOfAnaGasCal", L"DemarcationNumber", strDemarcationNumber);

	// д��DemarcationLog.ini	
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(sDemarcationLog));
	wcsncpy_s(sDemarcationLog.wchDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber), strDemarcationNumber, _countof(sDemarcationLog.wchDemarcationNumber)-1);
	wcsncpy_s(sDemarcationLog.wchLineNumber, _countof(sDemarcationLog.wchLineNumber), sLineInfo.wchLineNumber, _countof(sDemarcationLog.wchLineNumber)-1);
	wcsncpy_s(sDemarcationLog.wchOperator, _countof(sDemarcationLog.wchOperator), sUserInfo.wchName, _countof(sDemarcationLog.wchOperator)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchAnalyzerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchAnalyzerModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"����������", _countof(sDemarcationLog.wchEquipmentName)-1);
	CString strDemarcationItem;
	strDemarcationItem.Format(L"%d", ANA_GAS_CAL);
	wcsncpy_s(sDemarcationLog.wchDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem), strDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem)-1);
	wcsncpy_s(sDemarcationLog.wchDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate), strDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate)-1);
	wcsncpy_s(sDemarcationLog.wchPass, _countof(sDemarcationLog.wchPass), sResultOfAnaGasCal.wchPass, _countof(sDemarcationLog.wchPass)-1);
	SetIniDemarcationLog(sDemarcationLog);

	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchAnalyzerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchAnalyzerModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), L"����������У׼", _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	CString strData,strError;
	strData.Format(L"У׼��ֵ:");
	strError.Format(L"����ֵ:");
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
	wcsncpy_s(sEquCalChkInfo.wchJudgement, _countof(sEquCalChkInfo.wchJudgement), m_bDemarcationPass ? L"ͨ��" : L"��ͨ��", _countof(sEquCalChkInfo.wchJudgement)-1);
	SetIniEquCalChkInfo(sEquCalChkInfo);

	// дEquCalChkInfo��
	WriteEquCalChkInfo();

	// дDemarcationLog��
	WriteDemarcationLog();
	
	// дDemarcationResultOfAnaGasCal��
	WriteDemarcationResultOfAnaGasCal();

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
		// ����������
		ZeroMemory(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate));
		wcscpy_s(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo�������������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// ����������
		//ZeroMemory(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate));
		//wcscpy_s(sLineInfo.wchAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchAnalyzerChannelCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchAnalyzerChannelCalibrationDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	//DeleteFile(L"EquCalChkInfo.ini");
	//DeleteFile(L"DemarcationLog.ini");
	//DeleteFile(L"ResultOfAnaGasCal.ini");
	return 0x00;
}

void CAnaDlg::OnBnClickedButtonEnvParamsCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"��������������ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"�������ϴ�", L"��������������ļ�INI����");
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
	//jckssj	��鿪ʼʱ��
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	if (strJKID.Find(L"13W08") != -1)
	{
		//lx	����
		strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
		strData.AppendFormat(L"<lx>%s</lx>", strTemp);
	}
	//c3h8	��׼��C3H8Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0");
	strData.AppendFormat(L"<c3h8>%d</c3h8>", _wtoi(strTemp));
	//co	��׼��COŨ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00");
	strData.AppendFormat(L"<co>%.2f</co>", _wtof(strTemp));
	//co2	��׼��CO2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00");
	strData.AppendFormat(L"<co2>%.2f</co2>", _wtof(strTemp));
	//no	��׼��NOŨ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0");
	strData.AppendFormat(L"<no>%d</no>", _wtoi(strTemp));
	//no2	��׼��NO2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0");
	strData.AppendFormat(L"<no2>%d</no2>", _wtoi(strTemp));
	//o2	��׼��O2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00");
	strData.AppendFormat(L"<o2>%.2f</o2>", _wtof(strTemp));
	//hcjcjg	HC�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0");
	strData.AppendFormat(L"<hcjcjg>%d</hcjcjg>", _wtoi(strTemp));
	//cojcjg	CO�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00");
	strData.AppendFormat(L"<cojcjg>%.2f</cojcjg>", _wtof(strTemp));
	//co2jcjg	CO2�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<co2jcjg>%.2f</co2jcjg>", _wtof(strTemp));
	//nojcjg	NO�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0");
	strData.AppendFormat(L"<nojcjg>%d</nojcjg>", _wtoi(strTemp));
	//no2jcjg	NO2�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"0");
	strData.AppendFormat(L"<no2jcjg>%d</no2jcjg>", _wtoi(strTemp));
	//o2jcjg	O2�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<o2jcjg>%.2f</o2jcjg>", _wtof(strTemp));
	//pef	PEFֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3");
	strData.AppendFormat(L"<pef>%.3f</pef>", _wtof(strTemp));
	//jcjg	�����
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
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

bool CAnaDlg::UpEqulChk(const int& njclx)
{
	COleDateTime odtStart, odtEnd;

	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"��������������ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"�������ϴ�", L"��������������ļ�INI����");
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

bool CAnaDlg::UpGasCheck13W06(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�����Ǽ���ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"�������ϴ�", L"�����Ǽ���ļ�INI����");
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
	//jckssj	��鿪ʼʱ��
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	//c3h8	��׼��C3H8Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0");
	strData.AppendFormat(L"<c3h8>%d</c3h8>", _wtoi(strTemp));
	//co	��׼��COŨ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00");
	strData.AppendFormat(L"<co>%.2f</co>", _wtof(strTemp));
	//co2	��׼��CO2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00");
	strData.AppendFormat(L"<co2>%.2f</co2>", _wtof(strTemp));
	//no	��׼��NOŨ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0");
	strData.AppendFormat(L"<no>%d</no>", _wtoi(strTemp));
	//no2	��׼��NO2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0");
	strData.AppendFormat(L"<no2>%d</no2>", _wtoi(strTemp));
	//o2	��׼��O2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00");
	strData.AppendFormat(L"<o2>%.2f</o2>", _wtof(strTemp));
	//hcjcjg	HC�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0");
	strData.AppendFormat(L"<hcjcjg>%d</hcjcjg>", _wtoi(strTemp));
	//cojcjg	CO�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00");
	strData.AppendFormat(L"<cojcjg>%.2f</cojcjg>", _wtof(strTemp));
	//co2jcjg	CO2�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<co2jcjg>%.2f</co2jcjg>", _wtof(strTemp));
	//nojcjg	NO�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0");
	strData.AppendFormat(L"<nojcjg>%d</nojcjg>", _wtoi(strTemp));
	//no2jcjg	NO2�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"0");
	strData.AppendFormat(L"<no2jcjg>%d</no2jcjg>", _wtoi(strTemp));
	//o2jcjg	O2�����ֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00");
	strData.AppendFormat(L"<o2jcjg>%.2f</o2jcjg>", _wtof(strTemp));
	//pef	PEFֵ
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3");
	strData.AppendFormat(L"<pef>%.3f</pef>", _wtof(strTemp));
	//jcjg	�����
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
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

bool CAnaDlg::UpGasCheck13W07(void)
{
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"�����Ǽ���ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"�������ϴ�", L"�����Ǽ���ļ�INI����");
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
	//jckssj	��鿪ʼʱ��
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	//c3h8	��׼��C3H8Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0");
	strData.AppendFormat(L"<c3h8>%d</c3h8>", _wtoi(strTemp));
	//co	��׼��COŨ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00");
	strData.AppendFormat(L"<co>%.2f</co>", _wtof(strTemp));
	//co2	��׼��CO2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00");
	strData.AppendFormat(L"<co2>%.2f</co2>", _wtof(strTemp));
	//no	��׼��NOŨ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0");
	strData.AppendFormat(L"<no>%d</no>", _wtoi(strTemp));
	//no2	��׼��NO2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0");
	strData.AppendFormat(L"<no2>%d</no2>", _wtoi(strTemp));
	//o2	��׼��O2Ũ��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00");
	strData.AppendFormat(L"<o2>%.2f</o2>", _wtof(strTemp));
	//	noxysj1	NO��Ӧʱ�䣨T90��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOT90Limit", L"0.00");
	strData.AppendFormat(L"<noxysj1>%.2f</noxysj1>", _wtof(strTemp));
	//	no2xysj1	NO2��Ӧʱ�䣨T90��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2T90Limit", L"0.00");
	strData.AppendFormat(L"<no2xysj1>%.2f</no2xysj1>", _wtof(strTemp));
	//coxysj1	CO��Ӧʱ�䣨T90��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COT90Limit", L"0.00");
	strData.AppendFormat(L"<coxysj1>%.2f</coxysj1>", _wtof(strTemp));
	//o2xysj1	O2��Ӧʱ�䣨T90��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2T90Limit", L"0.00");
	strData.AppendFormat(L"<o2xysj1>%.2f</o2xysj1>", _wtof(strTemp));
	//noxysj2	NO��Ӧʱ�䣨T10��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NOT10Limit", L"0.00");
	strData.AppendFormat(L"<noxysj2>%.2f</noxysj2>", _wtof(strTemp));
	//no2xysj2	NO2��Ӧʱ�䣨T10��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"NO2T10Limit", L"0.00");
	strData.AppendFormat(L"<no2xysj2>%.2f</no2xysj2>", _wtof(strTemp));
	//coxysj2	CO��Ӧʱ�䣨T10��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"COT10Limit", L"0.00");
	strData.AppendFormat(L"<coxysj2>%.2f</coxysj2>", _wtof(strTemp));
	//o2xysj2	O2��Ӧʱ�䣨T10��
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"O2T10Limit", L"0.00");
	strData.AppendFormat(L"<o2xysj2>%.2f</o2xysj2>", _wtof(strTemp));
	//jcjg	�����
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
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