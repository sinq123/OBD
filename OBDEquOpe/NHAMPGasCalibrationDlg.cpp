// NHAMPGasCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHAMPGasCalibrationDlg.h"


// CNHAMPGasCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CNHAMPGasCalibrationDlg, CDialogZoom)

CNHAMPGasCalibrationDlg::CNHAMPGasCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHAMPGasCalibrationDlg::IDD, pParent)

	// PEF����ֵ,�Ŵ�1000��,��������
	, m_nPEFMeasuredValue(0)
	// HC����ֵ,��λ:ppm
	, m_nHCMeasuredValue(0)
	// CO����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	, m_nCOMeasuredValue(0)
	// CO2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	, m_nCO2MeasuredValue(0)
	// O2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	, m_nO2MeasuredValue(0)
	// NO����ֵ,��λ:ppm
	, m_nNOMeasuredValue(0)
	// NO2����ֵ,��λ:ppm
	, m_nNO2MeasuredValue(0)

	// ��׼��C3H8���ֵ,��λ:ppm
	, m_nC3H8NominalValue(0)
	// ��׼��CO���ֵ,��λ:0.001%(�ŵ�1000��)
	, m_nCONominalValue(0)
	// ��׼��CO2���ֵ,��λ:0.001%(�ŵ�1000��)
	, m_nCO2NominalValue(0)
	// ��׼��NO���ֵ,��λ:ppm
	, m_nNONominalValue(0)
	// ��׼��NO2���ֵ,��λ:ppm
	, m_nNO2NominalValue(0)

	// У׼ͨ��
	, m_nCalChn(4)

	// �Ƿ��ѯ����״̬
	, m_bCheckZeroStatus(true)

	// ����
	, m_nStep(0)
	// �����Ƿ��������1��
	, m_bIsOperationFinishedAtLeaseOnce(false)
	// У׼�Ƿ�Pass
	, m_bIsCalibrationPass(true)

	// ��ʼ���ص�����ָ��
	, m_pfProcessCtrl(NULL)
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
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
		_T("����"));

	// �������
	ImportParams();

	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pAnaThread;
}

CNHAMPGasCalibrationDlg::~CNHAMPGasCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pAnaThread)
	//{
	//	m_pAnaThread->Close();
	//	m_pAnaThread->QuitThread();
	//	WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	//}
}

void CNHAMPGasCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);

	DDX_Control(pDX, IDC_STATIC_HC, m_lbHC);
	DDX_Control(pDX, IDC_STATIC_C3H8, m_lbC3H8);
	DDX_Control(pDX, IDC_STATIC_CO, m_lbCO);
	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_O2, m_lbO2);
	DDX_Control(pDX, IDC_STATIC_NO, m_lbNO);
	DDX_Control(pDX, IDC_STATIC_NO2, m_lbNO2);
	DDX_Control(pDX, IDC_STATIC_PEF, m_lbPEF);

	DDX_Control(pDX, IDC_EDIT_C3H8_NOMINAL_VALUE, m_edC3H8NominalValue);
	DDX_Control(pDX, IDC_EDIT_CO_NOMINAL_VALUE, m_edCONominalValue);
	DDX_Control(pDX, IDC_EDIT_CO2_NOMINAL_VALUE, m_edCO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_NO_NOMINAL_VALUE, m_edNONominalValue);
	DDX_Control(pDX, IDC_EDIT_NO2_NOMINAL_VALUE, m_edNO2NominalValue);

	DDX_Control(pDX, IDC_MATHLABLE1, m_mlb1);
	DDX_Control(pDX, IDC_MATHLABLE2, m_mlb2);
	DDX_Control(pDX, IDC_MATHLABLE3, m_mlb3);
	DDX_Control(pDX, IDC_MATHLABLE4, m_mlb4);
	DDX_Control(pDX, IDC_MATHLABLE5, m_mlb5);
	DDX_Control(pDX, IDC_MATHLABLE6, m_mlb6);
	DDX_Control(pDX, IDC_MATHLABLE7, m_mlb7);
	DDX_Control(pDX, IDC_MATHLABLE8, m_mlb8);
	DDX_Control(pDX, IDC_MATHLABLE9, m_mlb9);
	DDX_Control(pDX, IDC_MATHLABLE10, m_mlb10);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
}


BEGIN_MESSAGE_MAP(CNHAMPGasCalibrationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHAMPGasCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHAMPGasCalibrationDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHAMPGasCalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_RADIO_STANDARD_JJG668, &CNHAMPGasCalibrationDlg::OnBnClickedRadioStandard)
	ON_BN_CLICKED(IDC_RADIO_STANDARD_GB18258, &CNHAMPGasCalibrationDlg::OnBnClickedRadioStandard)
	ON_BN_CLICKED(IDC_RADIO_STANDARD_GB3847, &CNHAMPGasCalibrationDlg::OnBnClickedRadioStandard)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_HC_CO_CO2, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_NO, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_NO2, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_NO_NO2, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_BN_CLICKED(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2, &CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHAMPGasCalibrationDlg ��Ϣ�������

BOOL CNHAMPGasCalibrationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHAMPGasCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHAMPGasCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"����������У׼");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// �������±��ı��ؼ�
	m_mlb1.SetScriptText(L"C_3H_8[10^-^6]");
	m_mlb2.SetScriptText(L"HC[10^-^6]");
	m_mlb3.SetScriptText(L"NO[10^-^6]");
	m_mlb4.SetScriptText(L"C_3H_8[10^-^6]");
	m_mlb5.SetScriptText(L"NO[10^-^6]");
	m_mlb6.SetScriptText(L"CO_2[%]");
	m_mlb7.SetScriptText(L"O_2[%]");
	m_mlb8.SetScriptText(L"CO_2[%]");
	m_mlb9.SetScriptText(L"NO_2[10^-^6]");
	m_mlb10.SetScriptText(L"NO_2[10^-^6]");

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));

	// HCʵʱֵ
	m_lbHC.SetLEDStyle();
	m_lbHC.SetText(L"--");
	// C3H8ʵʱֵ
	m_lbC3H8.SetLEDStyle();
	m_lbC3H8.SetText(L"--");
	// COʵʱֵ
	m_lbCO.SetLEDStyle();
	m_lbCO.SetText(L"--");
	// CO2ʵʱֵ
	m_lbCO2.SetLEDStyle();
	m_lbCO2.SetText(L"--");
	// O2ʵʱֵ
	m_lbO2.SetLEDStyle();
	m_lbO2.SetText(L"--");
	// NOʵʱֵ
	m_lbNO.SetLEDStyle();
	m_lbNO.SetText(L"--");
	// NO2ʵʱֵ
	m_lbNO2.SetLEDStyle();
	m_lbNO2.SetText(L"--");
	// PEFʵʱֵ
	m_lbPEF.SetLEDStyle();
	m_lbPEF.SetText(L"--");

	// CO,CO2У׼�����ֵ�༭��
	m_edCONominalValue.AllowNegative(FALSE);
	m_edCONominalValue.SetMaxDecimalPlaces(3);
	m_edCO2NominalValue.AllowNegative(FALSE);
	m_edCO2NominalValue.SetMaxDecimalPlaces(3);

	// ��ʾУ׼�����ֵ��ʷ��¼
	CString str;
	str.Format(L"%d", m_nC3H8NominalValue);
	m_edC3H8NominalValue.SetWindowText(str);
	str.Format(L"%.3f", m_nCONominalValue/1000.0f);
	m_edCONominalValue.SetWindowText(str);
	str.Format(L"%.3f", m_nCO2NominalValue/1000.0f);
	m_edCO2NominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNONominalValue);
	m_edNONominalValue.SetWindowText(str);
	str.Format(L"%d", m_nNO2NominalValue);
	m_edNO2NominalValue.SetWindowText(str);

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO_STANDARD_JJG668))->SetCheck(BST_CHECKED);
	OnBnClickedRadioStandard();

	((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2))->SetCheck(BST_CHECKED);
	OnBnClickedRadioChnCal();

	((CButton *)GetDlgItem(IDC_CHECK_MP4))->SetCheck(BST_CHECKED);
}

void CNHAMPGasCalibrationDlg::ImportParams(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);
	CString str;

	str = si.GetString(L"AnaCal", L"C3H8NominalValue", L"3000");
	m_nC3H8NominalValue = _wtoi(str);

	str = si.GetString(L"AnaCal", L"CONominalValue", L"6");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6f);
	// �Ŵ�1000��,����������
	m_nCONominalValue = (int)(f * 1000 + 0.5f);

	str = si.GetString(L"AnaCal", L"CO2NominalValue", L"12");
	f = (float)_wtof(str);
	ASSERT(f > -10e-6f);
	// �Ŵ�1000��,����������
	m_nCO2NominalValue = (int)(f * 1000 + 0.5f);

	str = si.GetString(L"AnaCal", L"NONominalValue", L"3000");
	m_nNONominalValue = _wtoi(str);

	str = si.GetString(L"AnaCal", L"NO2NominalValue", L"600");
	m_nNO2NominalValue = _wtoi(str);
}

void CNHAMPGasCalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	GetDlgItem(IDC_BUTTON_START)->GetWindowText(strTemp);
	if (strTemp == L"��ʼ")
	{
		// ���ð�ť
		GetDlgItem(IDC_RADIO_STANDARD_JJG668)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STANDARD_GB18258)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STANDARD_GB3847)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_MP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MP1)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CAL_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CAL_NO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CAL_NO_NO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2)->EnableWindow(FALSE);
	}

	if (GetKeyState(VK_RMENU)<0 && GetKeyState('S')<0)
	{
		// ��������
		ResetProcess();
		// ���Alt+S,����������(�ұ�Alt��,s�����ִ�Сд)
		m_nStep = 1;
	}

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼��ʼ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CAL_START, wchInfo, sizeof(wchInfo));
	}

	Process();
}

void CNHAMPGasCalibrationDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);

	// ��������
	ResetProcess();

	m_lbInfo.SetText(_T("����У׼ֹͣ"));

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	
	// ���ð�ť
	{
		GetDlgItem(IDC_RADIO_STANDARD_JJG668)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STANDARD_GB18258)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STANDARD_GB3847)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CAL_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CAL_NO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CAL_NO_NO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2)->EnableWindow(TRUE);
	}

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CAL_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHAMPGasCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CNHAMPGasCalibrationDlg::OnBnClickedRadioStandard()
{
	if (((CButton *)GetDlgItem(IDC_RADIO_STANDARD_JJG668))->GetCheck() == BST_CHECKED)
	{
		m_nStandardGas1 = 0x01;
		m_nStandardGas2 = 0x02;
		m_nStandardGas3 = 0x03;
		m_nStandardGas4 = 0x04;
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_STANDARD_GB18258))->GetCheck() == BST_CHECKED)
	{
		m_nStandardGas1 = 0x05;
		m_nStandardGas2 = 0x06;
		m_nStandardGas3 = 0x07;
		m_nStandardGas4 = 0x08;
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_STANDARD_GB3847))->GetCheck() == BST_CHECKED)
	{
		m_nStandardGas1 = 0x09;
		m_nStandardGas2 = 0x0A;
		m_nStandardGas3 = 0x0B;
		m_nStandardGas4 = 0x0C;
	}
}

void CNHAMPGasCalibrationDlg::OnBnClickedRadioChnCal()
{
	if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x01;

		m_edC3H8NominalValue.EnableWindow(TRUE);
		m_edCONominalValue.EnableWindow(TRUE);
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_NO))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x02;

		m_edC3H8NominalValue.EnableWindow(FALSE);
		m_edCONominalValue.EnableWindow(FALSE);
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_NO2))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x04;

		m_edC3H8NominalValue.EnableWindow(FALSE);
		m_edCONominalValue.EnableWindow(FALSE);
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(TRUE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x03;

		m_edC3H8NominalValue.EnableWindow(TRUE);
		m_edCONominalValue.EnableWindow(TRUE);
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(FALSE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x05;

		m_edC3H8NominalValue.EnableWindow(TRUE);
		m_edCONominalValue.EnableWindow(TRUE);
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(FALSE);
		m_edNO2NominalValue.EnableWindow(TRUE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_NO_NO2))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x06;

		m_edC3H8NominalValue.EnableWindow(FALSE);
		m_edCONominalValue.EnableWindow(FALSE);
		m_edCO2NominalValue.EnableWindow(FALSE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(TRUE);
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2))->GetCheck() == BST_CHECKED)
	{
		m_nCalChn = 0x07;

		m_edC3H8NominalValue.EnableWindow(TRUE);
		m_edCONominalValue.EnableWindow(TRUE);
		m_edCO2NominalValue.EnableWindow(TRUE);
		m_edNONominalValue.EnableWindow(TRUE);
		m_edNO2NominalValue.EnableWindow(TRUE);
	}
}

void CNHAMPGasCalibrationDlg::ClearRTDisplay(void)
{
	// HCʵʱֵ
	m_lbHC.SetText(L"--");
	// C3H8ʵʱֵ
	m_lbC3H8.SetText(L"--");
	// COʵʱֵ
	m_lbCO.SetText(L"--");
	// CO2ʵʱֵ
	m_lbCO2.SetText(L"--");
	// O2ʵʱֵ
	m_lbO2.SetText(L"--");
	// NOʵʱֵ
	m_lbNO.SetText(L"--");
	// NO2ʵʱֵ
	m_lbNO2.SetText(L"--");
	// PEFʵʱֵ
	m_lbPEF.SetText(L"--");
}

void CNHAMPGasCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		CalibrationProcess();
	}
	else if (2 == nIDEvent)
	{
		KillTimer(2);

		OpenSerialPortAndTestCommunication();
	}
	else if (3 == nIDEvent)
	{
		ZeroProcess();
	}
	else if (4 == nIDEvent)
	{
		// ��ʱ��������ʼ��ѯ������
		KillTimer(4);
		SetTimer(3, 1000, NULL);

		// ��ʱ����,���¿���"ֹͣ"��ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	}
	else if (5 == nIDEvent)
	{
		// ������ʱˢ������
		KillTimer(5);

		Process();
	}
	else if (6 == nIDEvent)
	{
		// ��ʱ��������ʼ��ѯУ׼����
		KillTimer(6);
		SetTimer(1, 1000, NULL);

		// ��ʱ����,���¿���"ֹͣ"��ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	}


	CDialogZoom::OnTimer(nIDEvent);
}

void CNHAMPGasCalibrationDlg::CalibrationProcess(void)
{
	BYTE bResult(0);
	short sHC(0);
	float fCO(0.0f);
	float fCO2(0.0f);
	float fO2(0.0f);
	short sNO(0);
	short sNO2(0);
	float fPef(0.0f);

	if (ANA_WR_SUCCEED == m_pAnaThread->GasCalibration(&bResult, m_nCalChn, &sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &fPef))
	{
		ASSERT(0x00==bResult || 0x01==bResult || 0x05==bResult);
		if (0x05 == bResult)
		{
			// ����У׼
			m_lbInfo.SetText(L"����У׼...");

			CString str;

			str.Format(L"%hd", sHC);
			m_lbHC.SetText(str);

			str.Format(L"%hd", sNO);
			m_lbNO.SetText(str);

			str.Format(L"%hd", sNO2);
			m_lbNO2.SetText(str);

			// PEF��һ�����仯ʱ,����ʾC3H8
			if (fPef > 0.1f)
			{
				float fC3H8 = sHC / fPef;
				str.Format(L"%.0f", fC3H8);
			}
			else
			{
				str = L"0";
			}
			m_lbC3H8.SetText(str);

			str.Format(L"%.2f", fCO);
			m_lbCO.SetText(str);
			str.Format(L"%.2f", fCO2);
			m_lbCO2.SetText(str);
			str.Format(L"%.2f", fO2);
			m_lbO2.SetText(str);
			str.Format(L"%.3f", fPef);
			m_lbPEF.SetText(str);

			//////////////////////////////////////////////////////////////////////////////////////��¼����ֵbegin
			// PEF����ֵ,�Ŵ�1000��,��������
			if (fPef < 0.0f)
			{
				fPef = 0.0f;
			}
			m_nPEFMeasuredValue = (int)(fPef * 1000.0f + 0.5f);
			// HC����ֵ,��λ:ppm
			if (sHC < 0)
			{
				sHC = 0;
			}
			m_nHCMeasuredValue = sHC;
			// CO����ֵ,��λ:0.001%,�Ŵ�1000��,��������
			if (fCO < 0.0f)
			{
				fCO = 0.0f;
			}
			m_nCOMeasuredValue = (int)(fCO * 1000.0f + 0.5f);
			// CO2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
			if (fCO2 < 0.0f)
			{
				fCO2 = 0.0f;
			}
			m_nCO2MeasuredValue = (int)(fCO2 * 1000.0f + 0.5f);
			// O2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
			if (fO2 < 0.0f)
			{
				fO2 = 0.0f;
			}
			m_nO2MeasuredValue = (int)(fO2 * 1000.0f + 0.5f);
			// NO����ֵ,��λ:ppm
			if (sNO < 0)
			{
				sNO = 0;
			}
			m_nNOMeasuredValue = sNO;
			m_nNO2MeasuredValue = sNO2;
			//////////////////////////////////////////////////////////////////////////////////////��¼����ֵend
		}
		else if (0x00 == bResult)
		{
			// У׼�ɹ�
			KillTimer(1);

			m_bIsCalibrationPass = true;

			// ���ʵʱ������ʾ
			ClearRTDisplay();

			// TODO:��¼У׼���

			// ��ǲ����Ѿ��������1��
			m_bIsOperationFinishedAtLeaseOnce = true;

			// ����У׼���
			ExportGasCalibrationResult();

			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼���", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_ANA_GAS_CAL_FINISH, wchInfo, sizeof(wchInfo));
			}

			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));

			m_lbInfo.SetText(_T("����У׼�ɹ�"));

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

			if (((CButton *)GetDlgItem(IDC_CHECK_MP3))->GetCheck()==BST_CHECKED
				&& m_nStandardGasCur==m_nStandardGas4)
			{
				m_nStep = 1;
				m_nStandardGasCur = m_nStandardGas3;
				Process();
			}
			else if (((CButton *)GetDlgItem(IDC_CHECK_MP2))->GetCheck()==BST_CHECKED
				&& (m_nStandardGasCur==m_nStandardGas4||m_nStandardGasCur==m_nStandardGas3))
			{
				m_nStep = 1;
				m_nStandardGasCur = m_nStandardGas2;
				Process();
			}
			else if (((CButton *)GetDlgItem(IDC_CHECK_MP1))->GetCheck()==BST_CHECKED
				&& (m_nStandardGasCur==m_nStandardGas4||m_nStandardGasCur==m_nStandardGas3||m_nStandardGasCur==m_nStandardGas2))
			{
				m_nStep = 1;
				m_nStandardGasCur = m_nStandardGas1;
				Process();
			}
			else
			{
				// ���ò���
				m_nStep = 0;

				// ���ð�ť
				{
					GetDlgItem(IDC_RADIO_STANDARD_JJG668)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_STANDARD_GB18258)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_STANDARD_GB3847)->EnableWindow(TRUE);

					GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
					GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
					GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

					GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_CAL_NO)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_CAL_NO2)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_CAL_NO_NO2)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2)->EnableWindow(TRUE);
				}
			}
		}
		else if (0x01 == bResult)
		{
			// У׼ʧ��
			KillTimer(1);

			m_bIsCalibrationPass = false;

			// ���ʵʱ������ʾ
			ClearRTDisplay();

			// TODO:��¼У׼���

			// ��ǲ����Ѿ��������1��
			m_bIsOperationFinishedAtLeaseOnce = true;

			// ����У׼���
			ExportGasCalibrationResult();

			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼���", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_ANA_GAS_CAL_FINISH, wchInfo, sizeof(wchInfo));
			}

			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));

			m_lbInfo.SetText(_T("����У׼ʧ��"));

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
			
			// ���ð�ť
			{
				GetDlgItem(IDC_RADIO_STANDARD_JJG668)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_STANDARD_GB18258)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_STANDARD_GB3847)->EnableWindow(TRUE);

				GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

				GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_CAL_NO)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_CAL_NO2)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_CAL_NO_NO2)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2)->EnableWindow(TRUE);
			}

			// ���ò���
			m_nStep = 0;
		}
	}
	else
	{
		KillTimer(1);
		m_lbInfo.SetText(_T("У׼ʧ��"));

		// ��������
		ResetProcess();

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

		// ���ð�ť
		{
			GetDlgItem(IDC_RADIO_STANDARD_JJG668)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_STANDARD_GB18258)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_STANDARD_GB3847)->EnableWindow(TRUE);

			GetDlgItem(IDC_CHECK_MP3)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_MP2)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_MP1)->EnableWindow(TRUE);

			GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_CAL_NO)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_CAL_NO2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_CAL_NO_NO2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CHN_CAL_HC_CO_CO2_NO_NO2)->EnableWindow(TRUE);
		}
	}
}

void CNHAMPGasCalibrationDlg::ZeroProcess(void)
{
	if (m_bCheckZeroStatus)
	{
		// ��״̬�����ѯ�������

		BYTE bStatus(0);
		if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		{
			if (0x05 == bStatus)
			{
				// ���ڵ���
				m_lbInfo.SetText(L"���ڵ���...");
			}
			else if (0x10 == bStatus)
			{
				// ����ɹ�
				KillTimer(3);

				// ��ʱ�׶�,���ΰ�ť
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// ��ʱ3��
				// ��Ϊ�յ���������,����ָ�û�н���
				SetTimer(5, 3000, NULL);
			}
			else if (0x11 == bStatus)
			{
				// ����ʧ��(����������һ��)
				KillTimer(3);

				// ��ʱ�׶�,���ΰ�ť
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// ��ʱ3��
				// ��Ϊ�յ���������,����ָ�û�н���
				SetTimer(5, 3000, NULL);
			}
			else
			{
				// ����δ�������
				//KillTimer(3);
				TRACE("����δ�������\n");
			}
		}
		else
		{
			KillTimer(3);
			m_lbInfo.SetText(_T("����ʧ��"));

			// ��������
			ResetProcess();

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		// �õ��������ѯ�������

		BYTE bResult;
		if (ANA_WR_SUCCEED == m_pAnaThread->Zero(&bResult))
		{
			ASSERT(0x00==bResult || 0x01==bResult || 0x05==bResult);
			if (0x05 == bResult)
			{
				// ���ڵ���
				m_lbInfo.SetText(L"���ڵ���...");
			}
			else if (0x00 == bResult)
			{
				// ����ɹ�
				KillTimer(3);

				// ��ʱ�׶�,���ΰ�ť
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// ��ʱ3��
				// ��Ϊ�յ���������,����ָ�û�н���
				SetTimer(5, 3000, NULL);
			}
			else if (0x01 == bResult)
			{
				// ����ʧ��(����������һ��)
				KillTimer(3);

				// ��ʱ�׶�,���ΰ�ť
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

				// ��ʱ3��
				// ��Ϊ�յ���������,����ָ�û�н���
				SetTimer(5, 3000, NULL);
			}
		}
		else
		{
			KillTimer(3);
			m_lbInfo.SetText(_T("����ʧ��"));

			// ��������
			ResetProcess();

			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
}

void CNHAMPGasCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
	// ��ȡͨѶ�˿�
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"AnaParams", L"Port", L"3"));
	// ��ȡ������ͨѶЭ������,Ĭ��Ϊ503
	CString strAnaProtocol = si.GetString(L"AnaParams", L"Protocol", L"503");
	// ��ȡ������Э������,3��NHA503AB,4��ͳһͨ��Э��
	CString strAnaProtocolVersion = si.GetString(L"AnaParams", L"ProtocolVersion", L"4");
	if (L"3" == strAnaProtocolVersion)
	{
		m_bCheckZeroStatus = false;
	}
	else
	{
		m_bCheckZeroStatus = true;
	}
	// ת��Ϊ��д
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
	else
	{
		// �������ܱ�ʶ������,ͳһ��Ϊ503
		im = CAnaThread::IM_NANHUA_NHA503;
	}
	bool bConnectToFlowmeter(false);
	if (CAnaThread::IM_NANHUA_NHA503V == im)
	{
		// ��ȡ���������ӷ�ʽ
		CString strConnectToFlowmeter = si.GetString(L"AnaParams", L"ConnectToFlo", L"0");
		if (L"1" == strConnectToFlowmeter)
		{
			// ���ӵ�������
			bConnectToFlowmeter = true;
		}
		else
		{
			// ���ӵ������
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

		// ����ͨѶ
		BYTE bStatus(0);
		if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		{
			// ͨѶ�ɹ�
			switch (bStatus)
			{
			case 0x00:  // ����(δ����)
			case 0x01:  // ����(�ѿ���)
				{
					// �ر�
					m_pAnaThread->StopPump();
					// ��ֹ�Զ�����
					m_pAnaThread->DisableAutoZero();

					// ����
					m_lbInfo.SetText(_T("����������У׼"));
					// λ��ʵʱͨѶ����
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x02:  // Ԥ��
				{
					// ����Ԥ��
					m_lbInfo.SetText(_T("����Ԥ��.."));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x03:	// ��©
				{
					// ���ڼ�©
					m_lbInfo.SetText(_T("���ڼ�©.."));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0xFF:
				{
					// ͨѶ����,���ڿ�ͨѶ����
					m_lbInfo.SetText(L"��������ʵʱ���״̬");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			default:
				{
					// ͨѶ����,���ڿ�ͨѶ����
					m_lbInfo.SetText(L"��������ʵʱ���״̬");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			}
		}
		else
		{
			// ͨѶʧ��
			m_lbInfo.SetText(L"ͨѶ��ʱ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CNHAMPGasCalibrationDlg::Process(void)
{
	if (0 == m_nStep)
	{
		// ��������
		ResetProcess();

		// ��ʼУ׼ǰ,����һ��ͨѶ
		if (ANA_WR_SUCCEED != m_pAnaThread->Zero())
		{
			m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			return;
		}

		m_lbInfo.SetText(_T("���ڵ���..."));
		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		// ��ʱ�׶�,���ΰ�ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		// ��ʱ2���ѯ������
		// ��Ϊ�����,����������Ӧ��һ����
		SetTimer(4, 2000, NULL);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		if (m_nStandardGasCur == m_nStandardGas4)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"��д4��У׼��ֵ");
			m_lbStep3.SetText(L"ͨ��4��У׼��");
			m_lbStep4.SetText(L"4����У׼");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}
		else if (m_nStandardGasCur == m_nStandardGas3)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"��д3��У׼��ֵ");
			m_lbStep3.SetText(L"ͨ��3��У׼��");
			m_lbStep4.SetText(L"3����У׼");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}
		else if (m_nStandardGasCur == m_nStandardGas2)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"��д2��У׼��ֵ");
			m_lbStep3.SetText(L"ͨ��2��У׼��");
			m_lbStep4.SetText(L"2����У׼");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}
		else if (m_nStandardGasCur == m_nStandardGas1)
		{
			m_stcStep2Label.ShowWindow(SW_HIDE);
			m_stcStep3Label.ShowWindow(SW_HIDE);
			m_stcStep4Label.ShowWindow(SW_HIDE);
			m_lbStep2.SetText(L"��д1��У׼��ֵ");
			m_lbStep3.SetText(L"ͨ��1��У׼��");
			m_lbStep4.SetText(L"1����У׼");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
		}

		//m_lbInfo.StopFlashText();
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		m_lbInfo.SetText(_T("������У׼��ֵ,Ȼ����һ��"));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////���У׼�����ֵ����start
		if (0x00 != GetNominalValue())
		{
			// ���ֵ���벻����Ҫ��
			return;
		}
		// ������ֵ��ini�����ļ�
		SaveNominalValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////���У׼�����ֵ����stop

		//m_lbInfo.StopFlashText();

		// д��У׼��Ũ��
		//0x01 У׼HC,CO,CO2
		//0x02 У׼NO
		//0x04 У׼NO2
		//0x03 У׼HC,CO,CO2,NO
		//0x05 У׼HC,CO,CO2,NO2
		//0x06 У׼NO,NO2
		//0x07 У׼HC,CO,CO2,NO,NO2
		BYTE bResult(0);
		if (m_nCalChn == 0x01)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, m_nC3H8NominalValue, m_nCONominalValue/1000.0f, m_nCO2NominalValue/1000.0f, 0, 0);
		}
		else if (m_nCalChn == 0x02)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, 0, m_nNONominalValue, 0);
		}
		else if (m_nCalChn == 0x04)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, 0, 0, m_nNO2NominalValue);
		}
		else if (m_nCalChn == 0x03)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, m_nC3H8NominalValue, m_nCONominalValue/1000.0f, m_nCO2NominalValue/1000.0f, m_nNONominalValue, 0);
		}
		else if (m_nCalChn == 0x05)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, m_nC3H8NominalValue, m_nCONominalValue/1000.0f, m_nCO2NominalValue/1000.0f, 0, m_nNO2NominalValue);
		}
		else if (m_nCalChn == 0x06)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, 0, 0, 0, m_nNONominalValue, m_nNO2NominalValue);
		}
		else if (m_nCalChn == 0x07)
		{
			m_pAnaThread->SetCalibrationGasConcentration(&bResult, m_nStandardGasCur, m_nC3H8NominalValue, m_nCONominalValue/1000.0f, m_nCO2NominalValue/1000.0f, m_nNONominalValue, m_nNO2NominalValue);
		}

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		m_lbInfo.SetText(_T("��ͨ��У׼��,Ȼ����һ��"));

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		// ��ʱ�׶�,���ΰ�ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		// �����ǵ�һ�ν���У׼����󣬷��ؽ������Ҫ3���
		// �������������ȡ��������ʱ�϶���ʱ�����������Ѿ�������У׼�������У׼���̡�
		BYTE bResult(0);
		m_pAnaThread->GasCalibration(&bResult, m_nCalChn);

		// ��ʱ3���ʼ��ѯУ׼����
		SetTimer(6, 3000, NULL);
	}
}

void CNHAMPGasCalibrationDlg::ResetProcess(void)
{
	// ������ʾ��
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));
	m_lbStep4.SetTextColor(RGB(0,0,0));

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	m_lbStep2.SetText(L"��д4��У׼��ֵ");
	m_lbStep3.SetText(L"ͨ��4��У׼��");
	m_lbStep4.SetText(L"4����У׼");

	// ��������
	m_nStep = 0;
	// У׼����ó�ʼ��ֵ
	m_bIsCalibrationPass = true;
	// ��У׼4����
	m_nStandardGasCur = m_nStandardGas4;

	// PEF����ֵ,�Ŵ�1000��,��������
	m_nPEFMeasuredValue = 0;
	// HC����ֵ,��λ:ppm
	m_nHCMeasuredValue = 0;
	// CO����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	m_nCOMeasuredValue = 0;
	// CO2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	m_nCO2MeasuredValue = 0;
	// O2����ֵ,��λ:0.001%,�Ŵ�1000��,��������
	m_nO2MeasuredValue = 0;
	// NO����ֵ,��λ:ppm
	m_nNOMeasuredValue = 0;
	// NO2����ֵ,��λ:ppm
	m_nNO2MeasuredValue = 0;
}

DWORD CNHAMPGasCalibrationDlg::GetNominalValue(void)
{
	CString strC3H8, strCO, strCO2, strNO, strNO2;

	//��ȡ����ֵ�����

	if (0x01==m_nCalChn || 0x03==m_nCalChn || 0x05==m_nCalChn || 0x07==m_nCalChn)
	{
		// ��ͨ��������ͨ��У׼

		GetDlgItem(IDC_EDIT_C3H8_NOMINAL_VALUE)->GetWindowText(strC3H8);
		strC3H8.Trim();
		if (L"" == strC3H8)
		{
			m_edC3H8NominalValue.SetFocus();
			MessageBox(_T("������У׼��C3H8���ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// ��׼��HC���ֵ,��λ:ppm
		m_nC3H8NominalValue = _wtoi(strC3H8);
		if (m_nC3H8NominalValue <= 0)
		{
			m_edC3H8NominalValue.SetFocus();
			MessageBox(_T("У׼��C3H8���ֵ����Ϊ0"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}

		GetDlgItem(IDC_EDIT_CO_NOMINAL_VALUE)->GetWindowText(strCO);
		strCO.Trim();
		if (L"" == strCO)
		{
			m_edCONominalValue.SetFocus();
			MessageBox(_T("������У׼��CO���ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// ��׼��CO���ֵ,��λ:0.001%(�ŵ�1000��)
		float f = (float)_wtof(strCO);
		m_nCONominalValue = int(f * 1000.0f + 0.5f);
		if (m_nCONominalValue <= 0)
		{
			m_edCONominalValue.SetFocus();
			MessageBox(_T("У׼��CO���ֵ����Ϊ0"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}

		GetDlgItem(IDC_EDIT_CO2_NOMINAL_VALUE)->GetWindowText(strCO2);
		strCO2.Trim();
		if (L"" == strCO2)
		{
			m_edCO2NominalValue.SetFocus();
			MessageBox(_T("������У׼��CO2���ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// ��׼��CO2���ֵ,��λ:0.001%(�ŵ�1000��)
		f = (float)_wtof(strCO2);
		m_nCO2NominalValue = int(f * 1000.0f + 0.5f);
		if (m_nCO2NominalValue <= 0)
		{
			m_edCO2NominalValue.SetFocus();
			MessageBox(_T("У׼��CO2���ֵ����Ϊ0"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}

	if (0x02==m_nCalChn || 0x03==m_nCalChn || 0x06==m_nCalChn || 0x07==m_nCalChn)
	{
		// ��ͨ�����ߵ�ͨ��У׼

		GetDlgItem(IDC_EDIT_NO_NOMINAL_VALUE)->GetWindowText(strNO);
		strNO.Trim();
		if (L"" == strNO)
		{
			m_edNONominalValue.SetFocus();
			MessageBox(_T("������У׼��NO���ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// ��׼��NO���ֵ,��λ:ppm
		m_nNONominalValue = _wtoi(strNO);
		if (m_nNONominalValue <= 0)
		{
			m_edNONominalValue.SetFocus();
			MessageBox(_T("У׼��NO���ֵ����Ϊ0"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}

	if (0x04==m_nCalChn || 0x05==m_nCalChn || 0x06==m_nCalChn || 0x07==m_nCalChn)
	{
		// ��ͨ�����ߵ�ͨ��У׼

		GetDlgItem(IDC_EDIT_NO2_NOMINAL_VALUE)->GetWindowText(strNO2);
		strNO2.Trim();
		if (L"" == strNO2)
		{
			m_edNO2NominalValue.SetFocus();
			MessageBox(_T("������У׼��NO2���ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
		// ��׼��NO2���ֵ,��λ:ppm
		m_nNO2NominalValue = _wtoi(strNO2);
		if (m_nNO2NominalValue <= 0)
		{
			m_edNO2NominalValue.SetFocus();
			MessageBox(_T("У׼��NO2���ֵ����Ϊ0"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}

	return 0x00;
}

void CNHAMPGasCalibrationDlg::SaveNominalValueToIniFile(void)
{
	///////////////////////////////////////////////////////////// ��¼�������ļ�start
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);
	CString str;
	str.Format(L"%d", m_nC3H8NominalValue);
	si.SetString(L"AnaCal", L"C3H8NominalValue", str);
	str.Format(L"%.3f", m_nCONominalValue/1000.0f);
	si.SetString(L"AnaCal", L"CONominalValue", str);
	str.Format(L"%.3f", m_nCO2NominalValue/1000.0f);
	si.SetString(L"AnaCal", L"CO2NominalValue", str);
	str.Format(L"%d", m_nNONominalValue);
	si.SetString(L"AnaCal", L"NONominalValue", str);
	str.Format(L"%d", m_nNO2NominalValue);
	si.SetString(L"AnaCal", L"NO2NominalValue", str);
	///////////////////////////////////////////////////////////// ��¼�������ļ�stop
}

void CNHAMPGasCalibrationDlg::ExportGasCalibrationRTData()
{
	// ĿǰΪ�գ�Ԥ��
}

void CNHAMPGasCalibrationDlg::ExportGasCalibrationResult()
{
	CString strPathOfDataExchange;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// ����2��
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// ���ݽ���Ŀ¼·��
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// �ж�·���Ƿ����,�����Խ��ж�д
	if (_taccess(strPathOfDataExchange, 06))
	{
		// ·��������

		// ����·��
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// ����·��ʧ��,����
			return;
		}
	}

	CString str;
	CString strResult(L"[ResultOfAnaGasCal]");
//////////////////////////////////////////////////////////////////////////////
	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// PEF
	str.Format(L"\r\nPEFMeasuredValue=%.3f", m_nPEFMeasuredValue/1000.0f);
	strResult += str;

	// �Ƿ�����HCͨ��У׼
	str.Format(L"\r\nSkipHCCalibration=%s", (0x02==m_nCalChn || 0x04==m_nCalChn || 0x06==m_nCalChn)?L"1":L"0");
	strResult += str;

	// C3H8��׼ֵ,��λ:ppm
	str.Format(L"\r\nC3H8StandardValue=%d", m_nC3H8NominalValue);
	strResult += str;
		
	// HC����ֵ,��λ:ppm
	str.Format(L"\r\nHCMeasuredValue=%d", m_nHCMeasuredValue);
	strResult += str;

	// �Ƿ�����COͨ��У׼
	str.Format(L"\r\nSkipCOCalibration=%s", (0x02==m_nCalChn || 0x04==m_nCalChn || 0x06==m_nCalChn)?L"1":L"0");
	strResult += str;

	// CO���ֵ,��λ:%
	str.Format(L"\r\nCOStandardValue=%.3f", m_nCONominalValue/1000.0f);
	strResult += str;

	// CO����ֵ,��λ:%
	CString strFormat;
	str.Format(L"\r\nCOMeasuredValue=%.3f", m_nCOMeasuredValue/1000.0f);
	strResult += str;

	// �Ƿ�����CO2ͨ��У׼
	str.Format(L"\r\nSkipCO2Calibration=%s", (0x02==m_nCalChn || 0x04==m_nCalChn || 0x06==m_nCalChn)?L"1":L"0");
	strResult += str;

	// CO2���ֵ,��λ:%
	str.Format(L"\r\nCO2StandardValue=%.3f", m_nCO2NominalValue/1000.0f);
	strResult += str;

	// CO2����ֵ,��λ:%
	str.Format(L"\r\nCO2MeasuredValue=%.3f", m_nCO2MeasuredValue/1000.0f);
	strResult += str;

	// �Ƿ�����NOͨ��У׼
	str.Format(L"\r\nSkipNOCalibration=%s", (0x01==m_nCalChn || 0x04==m_nCalChn || 0x05==m_nCalChn)?L"1":L"0");
	strResult += str;

	// NO���ֵ,��λ:%
	str.Format(L"\r\nNOStandardValue=%d", m_nNONominalValue);
	strResult += str;

	// NO����ֵ,��λ:%
	str.Format(L"\r\nNOMeasuredValue=%d", m_nNOMeasuredValue);
	strResult += str;

	// �Ƿ�����NO2ͨ��У׼
	str.Format(L"\r\nSkipNO2Calibration=%s", (0x01==m_nCalChn || 0x02==m_nCalChn || 0x03==m_nCalChn || 0x05==m_nCalChn)?L"1":L"0");
	strResult += str;

	// NO2���ֵ,��λ:%
	str.Format(L"\r\nNO2StandardValue=%d", m_nNO2NominalValue);
	strResult += str;

	// NO2����ֵ,��λ:%
	str.Format(L"\r\nNO2MeasuredValue=%d", m_nNO2MeasuredValue);
	strResult += str;

	// �Ƿ�����O2ͨ��У׼
	str.Format(L"\r\nSkipO2Calibration=%s", L"0");
	strResult += str;

	// O2����ֵ,��λ:%
	str.Format(L"\r\nO2MeasuredValue=%.3f", m_nO2MeasuredValue/1000.0f);
	strResult += str;

	// O2���ֵ,��λ:%
	str.Format(L"\r\nO2StandardValue=%s", L"");
	strResult += str;

	// У׼�Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsCalibrationPass?L"1":L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	CString strPathOfAnaGasCalibrationResult = strPathOfDataExchange + L"\\ResultOfAnaGasCal.ini";
	if (0 == _taccess(strPathOfAnaGasCalibrationResult, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfAnaGasCalibrationResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfAnaGasCalibrationResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfAnaGasCalibrationResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHAMPGasCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}