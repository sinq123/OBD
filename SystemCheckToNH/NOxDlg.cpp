// AnaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NOxDlg.h"

// CNOxDlg �Ի���

IMPLEMENT_DYNAMIC(CNOxDlg, CDialogZoom)

// �궨�Ƿ�Pass
bool CNOxDlg::m_bDemarcationPass = false;

CNOxDlg::CNOxDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNOxDlg::IDD, pParent)
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

CNOxDlg::~CNOxDlg()
{	
	m_fontDlgFont.DeleteObject();
}

void CNOxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNOxDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_GAS_CAL, &CNOxDlg::OnBnClickedButtonGasCal)
	ON_BN_CLICKED(IDC_BUTTON_GAS_CHK, &CNOxDlg::OnBnClickedButtonGasChk)
END_MESSAGE_MAP()


// CNOxDlg ��Ϣ�������
BOOL CNOxDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNOxDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"����������(����)");
}

void CNOxDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNOxDlg::OnBnClickedButtonGasCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	DeleteEquCalChkInfoFile();

	ShowNHNOxCalibrationDlg(AnaCalProcessCtrlCallBack);
}

void CNOxDlg::OnBnClickedButtonGasChk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	DeleteEquCalChkInfoFile();
	
	ShowNHNOxCheckDlg(AnaChkProcessCtrlCallBack);

}

void CNOxDlg::DeleteEquCalChkInfoFile(void)
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

DWORD CNOxDlg::CheckEquCalChkInfoFile(void)
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

DWORD CNOxDlg::UpdateLineInfoFile(const CString strItem, const CString strItemValue)
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

DWORD CNOxDlg::WriteEquCalChkInfo(void)
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

DWORD CNOxDlg::WriteDemarcationLog(void)
{
	DWORD dwReturn(0);

	// дDemarcationLog��
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}

DWORD CNOxDlg::WriteDemarcationResultOfAnaGasCal(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfAnaGasCal��
	DEMARCATIONRESULTOFANAGASCAL sResultOfAnaGasCal;
	ZeroMemory(&sResultOfAnaGasCal, sizeof(DEMARCATIONRESULTOFANAGASCAL));
	GetIniResultOfAnaGasCal(&sResultOfAnaGasCal);

	dwReturn = SetDboDemarcationResultOfAnaGasCal(sResultOfAnaGasCal);
	return dwReturn;
}

DWORD CNOxDlg::WriteDemarcationResultOfAnaGasChk(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfAnaGasChk��
	DEMARCATIONRESULTOFANAGASCHK sResultOfAnaGasChk;
	ZeroMemory(&sResultOfAnaGasChk, sizeof(DEMARCATIONRESULTOFANAGASCHK));
	GetIniResultOfAnaGasChk(&sResultOfAnaGasChk);

	dwReturn = SetDboDemarcationResultOfAnaGasChk(sResultOfAnaGasChk);
	return dwReturn;
}

DWORD CNOxDlg::ReadLineInfo(LINEINFO &sLineInfo)
{
	NHCLIENT sNHClient;
	ZeroMemory(&sNHClient, sizeof(NHCLIENT));
	GetIniNHClient(&sNHClient);

	CString strSql;
	strSql.Format(L"select * from LineInfo where LineNumber = '%s'", sNHClient.wchLineNumber);

	DWORD dwReturn = GetDboLineInfo(strSql, &sLineInfo);

	return dwReturn;
}

DWORD CNOxDlg::WriteLineInfo(LINEINFO &sLineInfo)
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

DWORD CNOxDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
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

DWORD CNOxDlg::WriteAnaGasChkData(void)
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
		ZeroMemory(sLineInfo.wchNOxAnalyzerChannelCheckDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCheckDate));
		wcscpy_s(sLineInfo.wchNOxAnalyzerChannelCheckDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCheckDate)/sizeof(wchar_t), str);

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
		//ZeroMemory(sLineInfo.wchNOxAnalyzerChannelCheckDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCheckDate));
		//wcscpy_s(sLineInfo.wchNOxAnalyzerChannelCheckDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchNOxAnalyzerChannelCheckDate, L"2000-01-01");

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

	if (str.Find(L"4") != -1)
	{
	}
	else
	{
	}
	

	// ����EquCalChkInfo.ini�ļ�ʹ����ϣ���Ҫɾ��
	DeleteEquCalChkInfoFile();
	return 0x00;
}

DWORD CNOxDlg::WriteAnaGasCalData(void)
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
	wcsncpy_s(sDemarcationLog.wchEquipmentNumber, _countof(sDemarcationLog.wchEquipmentNumber), sLineInfo.wchNOxAnalyzerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentModel, _countof(sDemarcationLog.wchEquipmentModel), sLineInfo.wchNOxAnalyzerModel, _countof(sDemarcationLog.wchEquipmentModel)-1);
	wcsncpy_s(sDemarcationLog.wchEquipmentName, _countof(sDemarcationLog.wchEquipmentName), L"����������", _countof(sDemarcationLog.wchEquipmentName)-1);
	CString strDemarcationItem;
	strDemarcationItem.Format(L"%d", NOX_GAS_CAL);
	wcsncpy_s(sDemarcationLog.wchDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem), strDemarcationItem, _countof(sDemarcationLog.wchDemarcationItem)-1);
	wcsncpy_s(sDemarcationLog.wchDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate), strDemarcationDate, _countof(sDemarcationLog.wchDemarcationDate)-1);
	wcsncpy_s(sDemarcationLog.wchPass, _countof(sDemarcationLog.wchPass), sResultOfAnaGasCal.wchPass, _countof(sDemarcationLog.wchPass)-1);
	SetIniDemarcationLog(sDemarcationLog);

	m_bDemarcationPass = (0 == wcscmp(sDemarcationLog.wchPass, L"1"));

	// д��EquCalChkInfo.ini
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcsncpy_s(sEquCalChkInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber), sLineInfo.wchLineNumber, _countof(sEquCalChkInfo.wchLineNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipmentNumber, _countof(sEquCalChkInfo.wchEquipmentNumber), sLineInfo.wchNOxAnalyzerNumber, _countof(sDemarcationLog.wchEquipmentNumber)-1);
	wcsncpy_s(sEquCalChkInfo.wchEquipment, _countof(sEquCalChkInfo.wchEquipment), sLineInfo.wchNOxAnalyzerModel, _countof(sEquCalChkInfo.wchEquipment)-1);
	wcsncpy_s(sEquCalChkInfo.wchOperator, _countof(sEquCalChkInfo.wchOperator), sUserInfo.wchName, _countof(sEquCalChkInfo.wchOperator)-1);
	wcsncpy_s(sEquCalChkInfo.wchItem, _countof(sEquCalChkInfo.wchItem), L"��������������У׼", _countof(sEquCalChkInfo.wchItem)-1);
	wcsncpy_s(sEquCalChkInfo.wchDate, _countof(sEquCalChkInfo.wchDate), sDemarcationLog.wchDemarcationDate, _countof(sEquCalChkInfo.wchDate)-1);
	CString strData,strError;
	strData.Format(L"У׼��ֵ:");
	strError.Format(L"����ֵ:");
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
	if (sResultOfAnaGasCal.wchSkipCO2Calibration[0] == L'0')
	{
		strData.AppendFormat(L"[CO2]:%s,", sResultOfAnaGasCal.wchCO2StandardValue);
		strError.AppendFormat(L"[CO2]:%s,", sResultOfAnaGasCal.wchCO2MeasuredValue);
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
		ZeroMemory(sLineInfo.wchNOxAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCalibrationDate));
		wcscpy_s(sLineInfo.wchNOxAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCalibrationDate)/sizeof(wchar_t), str);

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
		//// ����������
		//ZeroMemory(sLineInfo.wchNOxAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCalibrationDate));
		//wcscpy_s(sLineInfo.wchNOxAnalyzerChannelCalibrationDate, sizeof(sLineInfo.wchNOxAnalyzerChannelCalibrationDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchNOxAnalyzerChannelCalibrationDate, L"2000-01-01");

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

DWORD  CNOxDlg::AnaCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
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

DWORD  CNOxDlg::AnaChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
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
