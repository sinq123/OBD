// FlowmeterRealTimeDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterExternalAgreementCalDlg.h"
#include "afxdialogex.h"

// CFlowmeterExternalCalDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterExternalCalDlg, CDialogZoom)

CFlowmeterExternalCalDlg::CFlowmeterExternalCalDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterExternalCalDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 19 / 1024;
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

	m_pFloThread = (CFlowThread*)AfxBeginThread(RUNTIME_CLASS(CFlowThread));
}

CFlowmeterExternalCalDlg::~CFlowmeterExternalCalDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pFloThread)
	{
		m_pFloThread->Close();
		m_pFloThread->QuitThread();
		WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	}
}

void CFlowmeterExternalCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_FLUX_STD0, m_lbStd0Flux);
	DDX_Control(pDX, IDC_STATIC_FLUX_STD20, m_lbStd20Flux);
	DDX_Control(pDX, IDC_STATIC_FLUX_ACT, m_lbActFlux);
	DDX_Control(pDX, IDC_STATIC_TEMP, m_lbTemp);
	DDX_Control(pDX, IDC_STATIC_PRES, m_lbPres);
	DDX_Control(pDX, IDC_STATIC_UDILO2, m_lbUDilO2);
	DDX_Control(pDX, IDC_EDIT_TP_CALIBRATION_VALUE, m_neTPCalibrationValue);
	DDX_Control(pDX, IDC_EDIT_TP_CALIBRATION_VALUE2, m_nePressureCalibrationValue);
	DDX_Control(pDX, IDC_EDIT_HIGH, m_neHighO2Value);
	DDX_Control(pDX, IDC_EDIT_MIDDLE, m_neMiddleO2Value);
	DDX_Control(pDX, IDC_EDIT_LOW, m_neLowO2Value);
	DDX_Control(pDX, IDC_EDIT_ZERO, m_neZeroO2Value);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_HIGH_FLUX2, m_neStandardValueHighFlux);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_LOW_FLUX, m_neStandardValueLowFlux);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_HIGH_FLUX3, m_neStandardValueSingleFlux);
	DDX_Control(pDX, IDC_RADIO_HIGH, m_rdoHigh);
	DDX_Control(pDX, IDC_RADIO_MIDDLE, m_rdoMiddle);
	DDX_Control(pDX, IDC_RADIO_LOW, m_rdoLow);
}


BEGIN_MESSAGE_MAP(CFlowmeterExternalCalDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterExternalCalDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterExternalCalDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterExternalCalDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_TP, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationTp)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_PRESS, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationPress)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_O2, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationO2)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_HIGH_FLUX, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationHighFlux)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_LOW_FLUX, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationLowFlux)
	ON_BN_CLICKED(IDC_BUTTON_RECOVERY, &CFlowmeterExternalCalDlg::OnBnClickedButtonRecovery)
	ON_BN_CLICKED(IDC_RADIO_HIGH, &CFlowmeterExternalCalDlg::OnBnClickedRadioHigh)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, &CFlowmeterExternalCalDlg::OnBnClickedRadioMiddle)
	ON_BN_CLICKED(IDC_RADIO_LOW, &CFlowmeterExternalCalDlg::OnBnClickedRadioLow)
	ON_BN_CLICKED(IDC_RADIO_ZERO, &CFlowmeterExternalCalDlg::OnBnClickedRadioZero)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_SINGLE_FLUX, &CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationSingleFlux)
END_MESSAGE_MAP()

BOOL CFlowmeterExternalCalDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// CFlowmeterExternalCalDlg ��Ϣ�������

void CFlowmeterExternalCalDlg::InitCtrls(void)
{
	SetDlgFont();
	m_lbTitle.SetTitle(L"�ⲿЭ��������У׼");
	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	m_nPoint = 1;
	m_rdoHigh.SetCheck(TRUE);

	// ����ʵʱֵ;
	m_lbStd0Flux.SetLEDStyle();
	m_lbStd0Flux.SetText(L"--");
	m_lbStd20Flux.SetLEDStyle();
	m_lbStd20Flux.SetText(L"--");
	m_lbActFlux.SetLEDStyle();
	m_lbActFlux.SetText(L"--");

	// �¶�
	m_lbTemp.SetLEDStyle();
	m_lbTemp.SetText(L"--");

	// ѹ��
	m_lbPres.SetLEDStyle();
	m_lbPres.SetText(L"--");

	// ϡ����
	m_lbUDilO2.SetLEDStyle();
	m_lbUDilO2.SetText(L"--");

	// ��ʼ���¶�У׼ֵΪ25
	m_neTPCalibrationValue.SetWindowText(L"25");
	// ��ʼ��ѹ��У׼ֵΪ101kPa
	m_nePressureCalibrationValue.SetWindowText(L"101");
	// ��ʼ�����ϡ����У׼ֵΪ101kPa
	m_neZeroO2Value.SetWindowText(L"20.8");

	EnableBasicOperation(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_TP)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_PRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_O2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALIBRATION_SINGLE_FLUX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(false);
}

void CFlowmeterExternalCalDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterExternalCalDlg::OpenSerialPortAndTestCommunication(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Equipment.ini";

	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);

	BYTE bPort(0);
	// ��ȡͨѶ�˿�(�������ƻ�ȡ)
	bPort = (BYTE)_wtoi(si.GetString(L"FloParams", L"Port", L"4"));

	m_pFloThread->Open(bPort);
	if (m_pFloThread->IsOpen())
	{	
		SetTimer(2,1000,NULL);
		// ����
		m_lbInfo.SetText(_T("������У׼"));
		EnableBasicOperation(true);	
		GetDlgItem(IDC_BUTTON_CALIBRATION_TP)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_PRESS)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_O2)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALIBRATION_SINGLE_FLUX)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(true);
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		EnableBasicOperation(false);
	}
}

void CFlowmeterExternalCalDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterExternalCalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		KillTimer(1);


		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent && m_pFloThread->IsOpen())
	{
		float fFluxStd0;
		float fFluxStd20;
		float fFluxAct;
		float fTemp;
		float fPres;
		float fUDilO2;
		BYTE bResult(0);
		m_pFloThread->GetRealTimeData_Flowmeter(&fFluxStd0,&fFluxStd20,&fFluxAct,&fPres,&fUDilO2,&fTemp,&bResult);
		CString str;
		str.Format(L"%.2f",fFluxStd0);
		m_lbStd0Flux.SetText(str);
		str.Format(L"%.2f",fFluxStd20);
		m_lbStd20Flux.SetText(str);
		str.Format(L"%.2f",fFluxAct);
		m_lbActFlux.SetText(str);
		str.Format(L"%.2f",fPres);
		m_lbPres.SetText(str);
		str.Format(L"%.2f",fUDilO2);
		m_lbUDilO2.SetText(str);
		str.Format(L"%.2f",fTemp);
		m_lbTemp.SetText(str);
	}
	CDialogZoom::OnTimer(nIDEvent);

}


void CFlowmeterExternalCalDlg::OnBnClickedButtonFanOn()
{
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(true,&bResult);
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonFanOff()
{
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(false,&bResult);
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonQuit()
{
	KillTimer(2);
	OnCancel();
}


DWORD CFlowmeterExternalCalDlg::GetTpCalibrationValue(void)
{
	// ��ȡУ׼ֵ�����
	CString strTPCalibrationValue;
	m_neTPCalibrationValue.GetWindowText(strTPCalibrationValue);
	strTPCalibrationValue.Trim();
	if (L"" == strTPCalibrationValue)
	{
		m_neTPCalibrationValue.SetFocus();
		m_lbInfo.SetText(_T("�������¶�У׼ֵ"));
		return 0x01;
	}
	// У׼ֵ,��λ:��
	m_fTPCalValue = (float)_wtof(strTPCalibrationValue);
	// ��ʽ��Ϊ2ΪС��
	m_fTPCalValue = int(m_fTPCalValue * 100.0f + 0.5f) / 100.0f;

	if (m_fTPCalValue>150.0f || m_fTPCalValue<-50.0f)
	{
		m_neTPCalibrationValue.SetFocus();
		MessageBox(_T("�¶�У׼ֵ���ں���Χ(-50��~150��)"), _T("�¶�У׼"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

DWORD CFlowmeterExternalCalDlg::TpCalibrationProcess(void)
{
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(false,&bResult);
	m_pFloThread->SetCalibrationTemp_Flowmeter(&bResult, m_fTPCalValue);
	if (0x31 == bResult)
	{
		// �¶�У׼ֵ�ɹ�
		return 0x00;	
	}
	else
	{
		// �¶�У׼ֵʧ��
		return 0x01;
	}

}

void CFlowmeterExternalCalDlg::ExportFloTPCalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloTPCal]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	strData.Format(L"��׼ֵ:%.2f��", m_fTPCalValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strError;
	strError.Format(L"�������:0.01��");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsTpCalibrationPass ? L"1" : L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloTPCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CFlowmeterExternalCalDlg::RefreshRTData(bool bRefresh)
{
	if (bRefresh)
	{
		SetTimer(2, 1000, NULL);
	}
	else
	{
		KillTimer(2);
	}
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationTp()
{
	if (0x00 != GetTpCalibrationValue())
	{
		// У׼ֵ���벻����Ҫ��
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	if (0x00 == TpCalibrationProcess())
	{
		m_bIsTpCalibrationPass = true;
		// У׼�ɹ�
		m_lbInfo.SetText(_T("�¶�У׼�ɹ�"));
	}
	else
	{
		m_bIsTpCalibrationPass = false;
		// У׼ʧ��
		m_lbInfo.SetText(_T("�¶�У׼ʧ��"));
	}

	// ����ˢ��ʵʱ����
	RefreshRTData(true);
	EndWaitCursor();

	// �������
	ExportFloTPCalResult();
}



DWORD CFlowmeterExternalCalDlg::GetPresCalibrationValue(void)
{
	// ��ȡУ׼ֵ�����
	CString strPressureCalibrationValue;
	m_nePressureCalibrationValue.GetWindowText(strPressureCalibrationValue);
	strPressureCalibrationValue.Trim();
	if (L"" == strPressureCalibrationValue)
	{
		m_nePressureCalibrationValue.SetFocus();
		m_lbInfo.SetText(_T("������ѹ��У׼ֵ"));
		return 0x01;
	}
	// У׼ֵ,��λ:kPa
	m_fPressureCalibrationValue = (float)_wtof(strPressureCalibrationValue);
	// ��ʽ��Ϊ2ΪС��
	m_fPressureCalibrationValue = int(m_fPressureCalibrationValue * 100.0f + 0.5f) / 100.0f;
	if (m_fPressureCalibrationValue>150.0f || m_fPressureCalibrationValue<50.0f)
	{
		m_nePressureCalibrationValue.SetFocus();
		MessageBox(_T("ѹ��У׼ֵ���ں���Χ(50kPa~150kPa)"), _T("ѹ��У׼"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

DWORD CFlowmeterExternalCalDlg::PresCalibrationProcess(void)
{
	// ѹ�����������²�����ɣ�
	// 1)�رշ��
	// 2)����ѹ��У׼ֵ
	// 3)ѹ��У׼
	BYTE bResult(0);
	m_pFloThread->RunFan_Flowmeter(false,&bResult);
	m_pFloThread->SetCalibrationPres_Flowmeter(&bResult, m_fPressureCalibrationValue);
	if (0x31 == bResult)
	{
		// д��ѹ��У׼ֵ�ɹ�
		return 0x00;
	}
	else
	{
		// д��ѹ��У׼ֵʧ��
		return 0x01;
	}
}

void CFlowmeterExternalCalDlg::ExportFloPresCalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloPresCal]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	strData.Format(L"��׼ֵ:%.2fkPa", m_fPressureCalibrationValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strError;
	strError.Format(L"�������:0.01kPa");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPresCalibrationPass ? L"1" : L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloPresCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}



void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationPress()
{
	if (0x00 != GetPresCalibrationValue())
	{
		// У׼ֵ���벻����Ҫ��
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	if (0x00 == PresCalibrationProcess())
	{
		m_bIsPresCalibrationPass = true;

		// У׼�ɹ�
		m_lbInfo.SetText(_T("ѹ��У׼�ɹ�"));
	}
	else
	{
		m_bIsPresCalibrationPass = false;

		// У׼ʧ��
		m_lbInfo.SetText(_T("ѹ��У׼ʧ��"));
	}

	// ����ˢ��ʵʱ����
	RefreshRTData(true);

	EndWaitCursor();

	// �������
	ExportFloPresCalResult();
}




DWORD CFlowmeterExternalCalDlg::CalibrationProcess_High(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	CString str;
	m_neHighO2Value.GetWindowTextW(str);
	float fHighValue = (float)_wtof(str);
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_High(&bResult, fHighValue);
	if (0x31 == bResult)
	{
		// ϡ����У׼���
		return 0x00;
	}
	else
	{
		// ϡ����У׼ʧ��
		return 0x01;
	}
}

DWORD CFlowmeterExternalCalDlg::CalibrationProcess_Middle(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	CString str;
	m_neMiddleO2Value.GetWindowTextW(str);
	float fMiddleValue = (float)_wtof(str);
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Middle(&bResult, fMiddleValue);
	if (0x31 == bResult)
	{
		// ϡ����У׼���
		return 0x00;
	}
	else
	{
		// ϡ����У׼ʧ��
		return 0x01;
	}
}

DWORD CFlowmeterExternalCalDlg::CalibrationProcess_Low(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	CString str;
	m_neLowO2Value.GetWindowTextW(str);
	float fLowValue = (float)_wtof(str);
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Low(&bResult, fLowValue);
	if (0x31 == bResult)
	{
		// ϡ����У׼���
		return 0x00;
	}
	else
	{
		// ϡ����У׼ʧ��
		return 0x01;
	}


}

void CFlowmeterExternalCalDlg::ExportFloDilO2CalResult()
{
	CString str;
	CString strResult(L"[ResultOfFloDilO2Cal]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	strData.Format(L"��׼ֵ:%.2fkPa", 20.8f);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// �Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsO2CalibrationPass ? L"1" : L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloDilO2Cal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}



DWORD CFlowmeterExternalCalDlg::DilO2CalibrationProcess(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Zero(&bResult);
	if (0x31 == bResult)
	{
		
		// ϡ����У׼�ɹ�
		return 0x00;
			
	}
	else
	{
		// д��ϡ����У׼ʧ��
		return 0x01;
	}
}

void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationO2()
{
	if(m_nPoint == 1)
	{
		CString str;
		m_neHighO2Value.GetWindowTextW(str);
		float fHighValue = (float)_wtof(str);
		if (fHighValue<18.0f || fHighValue>21.0f)
		{
			MessageBoxW(L"�ߵ�У׼Ӧ��18��21֮��ȡֵ", L"������У׼", MB_OK|MB_ICONWARNING);
			m_neHighO2Value.SetFocus();
			return;
		}
	}
	else if(m_nPoint == 2)
	{
		CString str;
		m_neMiddleO2Value.GetWindowTextW(str);
		float fMiddleValue = (float)_wtof(str);
		if (fMiddleValue<4.5f || fMiddleValue>11.0f)
		{
			MessageBoxW(L"�е�У׼Ӧ��4.5��11֮��ȡֵ", L"������У׼", MB_OK|MB_ICONWARNING);
			m_neMiddleO2Value.SetFocus();
			return;
		}
	}
	else if(m_nPoint == 3)
	{
		CString str;
		m_neLowO2Value.GetWindowTextW(str);
		float fLowValue = (float)_wtof(str);
		if (fLowValue<0.5f || fLowValue>2.0f)
		{
			MessageBoxW(L"�͵�У׼Ӧ��0.5��2֮��ȡֵ", L"������У׼", MB_OK|MB_ICONWARNING);
			m_neLowO2Value.SetFocus();
			return;
		}
	}


	BeginWaitCursor();

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	EnableBasicOperation(false);

	if(m_nPoint == 1)
	{
		if (0x00 == CalibrationProcess_High())
		{
			m_bIsO2CalibrationPass = true;

			// У׼�ɹ�
			m_lbInfo.SetText(_T("�ߵ�ϡ����У׼�ɹ�"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// У׼ʧ��
			m_lbInfo.SetText(_T("�ߵ�ϡ����У׼ʧ��"));
		}
	}

	else if(m_nPoint == 2)
	{
		if (0x00 == CalibrationProcess_Middle())
		{
			m_bIsO2CalibrationPass = true;

			// У׼�ɹ�
			m_lbInfo.SetText(_T("�е�ϡ����У׼�ɹ�"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// У׼ʧ��
			m_lbInfo.SetText(_T("�е�ϡ����У׼ʧ��"));
		}
	}

	else if(m_nPoint == 3)
	{
		if (0x00 == CalibrationProcess_Low())
		{
			m_bIsO2CalibrationPass = true;

			// У׼�ɹ�
			m_lbInfo.SetText(_T("�͵�ϡ����У׼�ɹ�"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// У׼ʧ��
			m_lbInfo.SetText(_T("�͵�ϡ����У׼ʧ��"));
		}
	}
	else if(m_nPoint == 4)
	{
		if (0x00 == DilO2CalibrationProcess())
		{
			m_bIsO2CalibrationPass = true;

			// У׼�ɹ�
			m_lbInfo.SetText(_T("���ϡ����У׼�ɹ�"));
		}
		else
		{
			m_bIsO2CalibrationPass = false;

			// У׼ʧ��
			m_lbInfo.SetText(_T("���ϡ����У׼ʧ��"));
		}
	}

	EnableBasicOperation(true);

	// ����ˢ��ʵʱ����
	RefreshRTData(true);
	EndWaitCursor();

	// �������
	ExportFloDilO2CalResult();	
}

DWORD CFlowmeterExternalCalDlg::GetStandardValueOfLowFlux(void)
{
	// ��ȡ��׼ֵ�����
	CString strStandardValue;
	m_neStandardValueLowFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueLowFlux.SetFocus();
		m_lbInfo.SetText(_T("�����������У׼ֵ"));
		return 0x01;
	}
	// ������׼ֵ,��λ:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// ��ʽ��Ϊ2ΪС��������������
	m_fStandardValueOfLowFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// ������Լ��(0L/s~500L/s)
	if (m_fStandardValueOfLowFlux<0.0f || m_fStandardValueOfLowFlux>500.0f)
	{
		m_neStandardValueLowFlux.SetFocus();
		MessageBox(_T("��������׼ֵ���ں���Χ(0L/s~500L/s)"), _T("����У׼"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}
DWORD CFlowmeterExternalCalDlg::GetStandardValueOfSingleFlux(void)
{
	// ��ȡ��׼ֵ�����
	CString strStandardValue;
	m_neStandardValueSingleFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueSingleFlux.SetFocus();
		m_lbInfo.SetText(_T("�����뵥������У׼ֵ"));
		return 0x01;
	}
	// ������׼ֵ,��λ:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// ��ʽ��Ϊ2ΪС��������������
	m_fStandardValueOfSingleFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// ������Լ��(0L/s~500L/s)
	if ((m_fStandardValueOfSingleFlux<50.0f || m_fStandardValueOfSingleFlux>235.0f))
	{
		m_neStandardValueSingleFlux.SetFocus();
		MessageBox(_T("����������׼ֵ���ں���Χ(50L/s~235L/s)"), _T("����У׼"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}





DWORD CFlowmeterExternalCalDlg::GetStandardValueOfHighFlux(void)
{
	// ��ȡ��׼ֵ�����
	CString strStandardValue;
	m_neStandardValueHighFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueHighFlux.SetFocus();
		m_lbInfo.SetText(_T("�����������У׼ֵ"));
		return 0x01;
	}
	// ������׼ֵ,��λ:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// ��ʽ��Ϊ2ΪС��������������
	m_fStandardValueOfHighFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// ������Լ��:ͨ�ø�����(50L/s~500L/s) �ϻ�����(50L/s~235L/s)
	if (m_fStandardValueOfHighFlux<50.0f || m_fStandardValueOfHighFlux>500.0f)
	{
		m_neStandardValueHighFlux.SetFocus();
		MessageBox(_T("��������׼ֵ���ں���Χ(50L/s~500L/s)"), _T("����У׼"), MB_OK|MB_ICONWARNING);

		return 0x01;
	}
	
	return 0x00;
}


void CFlowmeterExternalCalDlg::ImportParams(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CString strEquipmentPath = strPath + L"\\Config\\Equipment.ini";
	CSimpleIni sio(strEquipmentPath);
	CString str = sio.GetString(L"FloParams", L"MaximumZeroOffsetValue", L"36.7");
	float f = (float)_wtof(str);
	// ��ʽ����λС��
	m_fMaximumZeroOffsetValue = (int)(f * 100.0f +0.5f) / 100.0f;
	ASSERT(m_fMaximumZeroOffsetValue > -10e-6);
	// ������Լ��(0L/s~36.7L/s)
	if (m_fMaximumZeroOffsetValue < 0.0f)
	{
		m_fMaximumZeroOffsetValue = 0.0f;
	}
	if (m_fMaximumZeroOffsetValue > 36.7f)
	{
		m_fMaximumZeroOffsetValue = 36.7f;
	}
}

void CFlowmeterExternalCalDlg::ExportFloFluxCalResult(CString strName,float fValue,BOOL bPass)
{
	CString str;
	CString strResult(L"[ResultOfFloFluxCal]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;
	
	// ����
	CString strData;
	strData.Format(L"%sУ׼ֵ:%.2fL/s",strName, fValue);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// �Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", bPass ? L"1" : L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationSingleFlux()
{
	
	// ��ȡ����������׼ֵ
	if (0x00 != GetStandardValueOfSingleFlux())
	{
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ������
	RefreshRTData(false);

	CString strName(L"");
	strName.Format(L"��������");

	// д��У׼ֵ��У׼
	BYTE bResult(0);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CNanhuaNHF1::SinglePointFlowCal, m_fStandardValueOfSingleFlux);
	if (0x31 == bResult)
	{
		// ��������У׼�ɹ�
		m_lbInfo.SetText(L"��������У׼�ɹ�");
		m_bIsFlowCalibrationPass = true;
	}
	else
	{

		// ��������У׼ʧ��
		m_lbInfo.SetText(L"��������У׼�ɹ�");
		m_bIsFlowCalibrationPass = false;
	}
	// �������
	ExportFloFluxCalResult(strName,m_fStandardValueOfSingleFlux,m_bIsFlowCalibrationPass);
	// ��ʼˢ������
	RefreshRTData(true);
	EndWaitCursor();

	
	
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationHighFlux()
{
	// ��ȡ��������׼ֵ
	if (0x00 != GetStandardValueOfHighFlux())
	{
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ������
	RefreshRTData(false);

	CString strName(L"");
	strName.Format(L"������");

	// д��У׼ֵ��У׼
	BYTE bResult(0);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CNanhuaNHF1::HighFlowCal, m_fStandardValueOfHighFlux);
	if (0x31 == bResult)
	{
		// �ߵ�����У׼�ɹ�
		m_lbInfo.SetText(L"������У׼�ɹ�");
		m_bIsFlowCalibrationPass = true;
		
	}
	else
	{
		// �ߵ�������׼ֵʧ��
		m_lbInfo.SetText(L"������У׼ʧ��");
		m_bIsFlowCalibrationPass = false;
	}
	// �������
	ExportFloFluxCalResult(strName,m_fStandardValueOfHighFlux,m_bIsFlowCalibrationPass);
	// ��ʼˢ������
	RefreshRTData(true);

	EndWaitCursor();
	
	
}


void CFlowmeterExternalCalDlg::OnBnClickedButtonCalibrationLowFlux()
{

	// ��ȡ��������׼ֵ
	if (0x00 != GetStandardValueOfLowFlux())
	{
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ������
	RefreshRTData(false);

	CString strName(L"");
	strName.Format(L"������");
	// д��У׼ֵ��У׼
	BYTE bResult(0x00);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CNanhuaNHF1::LowFlowCal, m_fStandardValueOfLowFlux);
	if (0x31 == bResult)
	{
		// ������У׼�ɹ�
		m_lbInfo.SetText(L"������У׼�ɹ�");
		m_bIsFlowCalibrationPass = true;
	}
	else
	{
		// ������У׼ʧ��
		m_lbInfo.SetText(L"������У׼ʧ��");
		m_bIsFlowCalibrationPass = false;
	}
	// �������
	ExportFloFluxCalResult(strName,m_fStandardValueOfLowFlux,m_bIsFlowCalibrationPass);
	// ��ʼˢ������
	RefreshRTData(true);

	EndWaitCursor();
	
	
}


DWORD CFlowmeterExternalCalDlg::RestoreFactorySettings(void)
{

	BYTE bResult(0);
	m_pFloThread->RestoreFactorySettings_Flowmeter(&bResult);
	if (0x31 == bResult)
	{
		// �¶�У׼ֵ�ɹ�
		return 0x00;	
	}
	else
	{
		// �¶�У׼ֵʧ��
		return 0x01;
	}
}

void CFlowmeterExternalCalDlg::OnBnClickedButtonRecovery()
{
	if (0x00 == RestoreFactorySettings())
	{
		m_lbInfo.SetText(_T("�ָ��������óɹ�"));
	}
	else
	{
		m_lbInfo.SetText(_T("�ָ���������ʧ��"));
	}
}

void CFlowmeterExternalCalDlg::OnBnClickedRadioHigh()
{
	m_nPoint = 1;
}


void CFlowmeterExternalCalDlg::OnBnClickedRadioMiddle()
{
	m_nPoint = 2;
}


void CFlowmeterExternalCalDlg::OnBnClickedRadioLow()
{
	m_nPoint = 3;
}

void CFlowmeterExternalCalDlg::OnBnClickedRadioZero()
{
	m_nPoint = 4;
}


