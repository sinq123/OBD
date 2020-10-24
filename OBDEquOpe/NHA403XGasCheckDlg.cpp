// NHA403XGasCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHA403XGasCheckDlg.h"
#include "math.h"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// �Ŵ���,���ڽ��н�������ת��������,������ֵ�Ƚ�(�ø����ͱȽ��о�������)
#define AMPLIFICATION_FACTOR 100
// С��λ��,������ʾ��������ֵ��С��λ��,һ��Ҫ����AMPLIFICATION_FACTOR�ɶ����仯
#define DECIMAL_PLACES 3

// CNHA403XGasCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CNHA403XGasCheckDlg, CDialogZoom)

CNHA403XGasCheckDlg::CNHA403XGasCheckDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CNHA403XGasCheckDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)

, m_nC3H8NominalValue(0)
, m_nCONominalValue(0)
, m_nCO2NominalValue(0)
, m_nHCNominalValue(0)

	// HC���������ֵ,��λ:ppm
	, m_nHCAELimit(12)
	// CO���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nCOAELimit(int(0.06f * AMPLIFICATION_FACTOR+0.5f))
	// CO2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nCO2AELimit(int(0.50f * AMPLIFICATION_FACTOR + 0.5f))

	// HC��������ֵ,��λ:%
	, m_nHCRELimit(5)
	// CO��������ֵ,��λ:%
	, m_nCORELimit(5)
	// CO2��������ֵ,��λ:%
	, m_nCO2RELimit(5)

	// PEF����ֵ,�Ŵ�1000��,��������
	, m_nPEFMeasuredValue(0)
	// HC����ֵ,��λ:ppm
	, m_nHCMeasuredValue(0)
	// CO����ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nCOMeasuredValue(0)
	// CO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nCO2MeasuredValue(0)
	// O2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nO2MeasuredValue(0)

	// HC�������,��λ:ppm
	, m_nHCAE(0)
	// CO�������,��λ:0.01%,�Ŵ�100��,��������
	, m_nCOAE(0)
	// CO2�������,��λ:0.01%,�Ŵ�100��,��������
	, m_nCO2AE(0)

	// HC��������Ƿ�ͨ��
	, m_bIsHCAEPass(false)
	// CO��������Ƿ�ͨ��
	, m_bIsCOAEPass(false)
	// CO2��������Ƿ�ͨ��
	, m_bIsCO2AEPass(false)

	// HC������,��λ:%
	, m_nHCRE(0)
	// CO������,��λ:%,�Ŵ�100��,��������
	, m_nCORE(0)
	// CO2������,��λ:%,�Ŵ�100��,��������
	, m_nCO2RE(0)

	// HC�������Ƿ�ͨ��
	, m_bIsHCREPass(0)
	// CO�������Ƿ�ͨ��
	, m_bIsCOREPass(0)
	// CO2�������Ƿ�ͨ��
	, m_bIsCO2REPass(0)

	// HCͨ���Ƿ�ͨ��
	, m_bIsHCPass(0)
	// COͨ���Ƿ�ͨ��
	, m_bIsCOPass(0)
	// CO2ͨ���Ƿ�ͨ��
	, m_bIsCO2Pass(0)

	// ����Ƿ�ͨ��
	, m_bIsCheckPass(0)
	
	// ��ʼ���ص�����
	, m_pfProcessCtrl(NULL)

	// ������Э��汾
	, m_nAnaProtocolVersion(4)
{
	// �����������߳�
	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pAnaThread;

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

	// �������������
	ImportGasCheckParams();
}

CNHA403XGasCheckDlg::~CNHA403XGasCheckDlg()
{
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// �˳�����ʱ,�������1�ζ�û����ɹ�,ҲҪ������Ӧ���
		ExportEquCalChkInfo();
	}
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pAnaThread)
	//{
	//	m_pAnaThread->Close();
	//	m_pAnaThread->QuitThread();
	//	WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	//}
}

void CNHA403XGasCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_STATIC_HC, m_lbHC);
	DDX_Control(pDX, IDC_STATIC_C3H8, m_lbC3H8);
	DDX_Control(pDX, IDC_STATIC_CO, m_lbCO);
	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_O2, m_lbO2);
	DDX_Control(pDX, IDC_STATIC_PEF, m_lbPEF);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);

	DDX_Control(pDX, IDC_EDIT_C3H8_NOMINAL_VALUE, m_edC3H8NominalValue);
	DDX_Control(pDX, IDC_EDIT_CO_NOMINAL_VALUE, m_edCONominalValue);
	DDX_Control(pDX, IDC_EDIT_CO2_NOMINAL_VALUE, m_edCO2NominalValue);

	DDX_Control(pDX, IDC_STATIC_HC_NOMINAL_VALUE, m_lbHCNominalValue);
	DDX_Control(pDX, IDC_STATIC_CO_NOMINAL_VALUE, m_lbCONominalValue);
	DDX_Control(pDX, IDC_STATIC_CO2_NOMINAL_VALUE, m_lbCO2NominalValue);

	DDX_Control(pDX, IDC_STATIC_HC_MEASURED_VALUE, m_lbHCMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO_MEASURED_VALUE, m_lbCOMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO2_MEASURED_VALUE, m_lbCO2MeasuredValue);

	DDX_Control(pDX, IDC_STATIC_HC_RE, m_lbHCRE);
	DDX_Control(pDX, IDC_STATIC_CO_RE, m_lbCORE);
	DDX_Control(pDX, IDC_STATIC_CO2_RE, m_lbCO2RE);

	DDX_Control(pDX, IDC_STATIC_HC_AE, m_lbHCAE);
	DDX_Control(pDX, IDC_STATIC_CO_AE, m_lbCOAE);
	DDX_Control(pDX, IDC_STATIC_CO2_AE, m_lbCO2AE);

	DDX_Control(pDX, IDC_STATIC_HC_JUDGEMENT, m_lbHCJudgement);
	DDX_Control(pDX, IDC_STATIC_CO_JUDGEMENT, m_lbCOJudgement);
	DDX_Control(pDX, IDC_STATIC_CO2_JUDGEMENT, m_lbCO2Judgement);

	DDX_Control(pDX, IDC_STATIC_DETERMINATION, m_lbDetermination);
	DDX_Control(pDX, IDC_STATIC_PEF_MEASURED_VALUE, m_lbPEFMeasuredValue);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);

	DDX_Control(pDX, IDC_MATHLABLE1, m_mlb1);
	DDX_Control(pDX, IDC_MATHLABLE2, m_mlb2);
	DDX_Control(pDX, IDC_MATHLABLE3, m_mlb3);
	DDX_Control(pDX, IDC_MATHLABLE4, m_mlb4);
	DDX_Control(pDX, IDC_MATHLABLE5, m_mlb5);
	DDX_Control(pDX, IDC_MATHLABLE6, m_mlb6);
	DDX_Control(pDX, IDC_MATHLABLE8, m_mlb7);
	DDX_Control(pDX, IDC_MATHLABLE15, m_mlb8);
	DDX_Control(pDX, IDC_MATHLABLE16, m_mlb9);
	DDX_Control(pDX, IDC_MATHLABLE17, m_mlb10);
}


BEGIN_MESSAGE_MAP(CNHA403XGasCheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHA403XGasCheckDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHA403XGasCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHA403XGasCheckDlg::OnBnClickedButtonStop)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CNHA403XGasCheckDlg ��Ϣ�������

BOOL CNHA403XGasCheckDlg::OnInitDialog()
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

void CNHA403XGasCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHA403XGasCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbHead.SetTitle(L"������������");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// �������±��ı��ؼ�
	m_mlb1.SetScriptText(L"C_3H_8[10^-^6]");
	m_mlb2.SetScriptText(L"HC[10^-^6]");
	m_mlb3.SetScriptText(L"C_3H_8[10^-^6]");
	m_mlb4.SetScriptText(L"����ֵ[10^-^6]");
	m_mlb5.SetScriptText(L"���ֵ[10^-^6]");
	m_mlb6.SetScriptText(L"�������[10^-^6]");
	m_mlb7.SetScriptText(L"CO_2ͨ��");
	m_mlb8.SetScriptText(L"CO_2[%]");
	m_mlb9.SetScriptText(L"CO_2[%]");
	m_mlb10.SetScriptText(L"O_2[%]");

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

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
	// NOʵʱֵ
	//m_lbNO.SetBkColor(RGB(0, 0, 0));
	//m_lbNO.SetTextColor(RGB(0, 255, 0));
	//m_lbNO.AutoFitFont();
	//m_lbNO.SetText(L"--");
	// O2ʵʱֵ
	m_lbO2.SetLEDStyle();
	m_lbO2.SetText(L"--");
	// PEFʵʱֵ
	m_lbPEF.SetLEDStyle();
	m_lbPEF.SetText(L"--");

	// CO,CO2��������ֵ�༭��
	m_edCONominalValue.AllowNegative(FALSE);
	m_edCONominalValue.SetMaxDecimalPlaces(DECIMAL_PLACES);
	m_edCO2NominalValue.AllowNegative(FALSE);
	m_edCO2NominalValue.SetMaxDecimalPlaces(DECIMAL_PLACES);

	CString strFormat;

	// ��ʾ��������ֵ��ʷ��¼
	CString str;
	str.Format(L"%d", m_nC3H8NominalValue);
	m_edC3H8NominalValue.SetWindowText(str);
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	m_edCONominalValue.SetWindowText(str);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	m_edCO2NominalValue.SetWindowText(str);

	// PEF����ֵ��ʾ��
	m_lbPEFMeasuredValue.SetBkColor(RGB(230, 230, 230));

	// HC����ֵ��ʾ��
	m_lbHCMeasuredValue.SetBkColor(RGB(230, 230, 230));
	// CO����ֵ��ʾ��
	m_lbCOMeasuredValue.SetBkColor(RGB(230, 230, 230));
	// CO2����ֵ��ʾ��
	m_lbCO2MeasuredValue.SetBkColor(RGB(230, 230, 230));


	// HC���ֵ��ʾ��
	m_lbHCNominalValue.SetBkColor(RGB(230, 230, 230));
	// CO���ֵ��ʾ��
	m_lbCONominalValue.SetBkColor(RGB(230, 230, 230));
	// CO2���ֵ��ʾ��
	m_lbCO2NominalValue.SetBkColor(RGB(230, 230, 230));

	
	// HC���������ʾ��
	m_lbHCAE.SetBkColor(RGB(230, 230, 230));
	// CO���������ʾ��
	m_lbCOAE.SetBkColor(RGB(230, 230, 230));
	// CO2���������ʾ��
	m_lbCO2AE.SetBkColor(RGB(230, 230, 230));


	// HC��������ʾ��
	m_lbHCRE.SetBkColor(RGB(230, 230, 230));
	// CO��������ʾ��
	m_lbCORE.SetBkColor(RGB(230, 230, 230));
	// CO2��������ʾ��
	m_lbCO2RE.SetBkColor(RGB(230, 230, 230));


	// HC�ж���ʾ��
	m_lbHCJudgement.SetBkColor(RGB(230, 230, 230));
	// CO�ж���ʾ��
	m_lbCOJudgement.SetBkColor(RGB(230, 230, 230));
	// CO2�ж���ʾ��
	m_lbCO2Judgement.SetBkColor(RGB(230, 230, 230));

	
	// �þ���ʾ��
	m_lbDetermination.SetBkColor(RGB(230, 230, 230));
	m_lbDetermination.AutoFitFont();

	//////////////////////////////////////// ��ʼ������������������ֵstart
	// HC���������ʾ��
	m_lbHCAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nHCAELimit);
	m_lbHCAE.SetText(str);
	// CO���������ʾ��
	m_lbCOAE.SetTextColor(RGB(0, 0, 0));
	strFormat.Format(L"--(��%%.%df)", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	m_lbCOAE.SetText(str);
	// CO2���������ʾ��
	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);

	// HC��������ʾ��
	m_lbHCRE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nHCRELimit);
	m_lbHCRE.SetText(str);
	// CO��������ʾ��
	m_lbCORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nCORELimit);
	m_lbCORE.SetText(str);
	// CO2��������ʾ��
	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nCO2RELimit);
	m_lbCO2RE.SetText(str);
	//////////////////////////////////////// ��ʼ������������������ֵstop
	
	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CNHA403XGasCheckDlg::ImportGasCheckParams(void)
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
	str = si.GetString(L"AnaChk", L"C3H8NominalValue", L"3000");
	m_nC3H8NominalValue = _wtoi(str);
	str = si.GetString(L"AnaChk", L"CONominalValue", L"6");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// �Ŵ�100��,����������
	m_nCONominalValue = (int)(f * AMPLIFICATION_FACTOR + 0.5f);
	str = si.GetString(L"AnaChk", L"CO2NominalValue", L"12");
	f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// �Ŵ�100��,����������
	m_nCO2NominalValue = (int)(f * AMPLIFICATION_FACTOR + 0.5f);
}

void CNHA403XGasCheckDlg::ExportEquCalChkInfo(void)
{
	// ��������������ݽ���Ŀ¼

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
			// ����·��ʧ��,����,������
			return;
		}
	}

	// ��ȡ����߱�źͷ������ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strAnalyzerModel = siLineInfo.GetString(L"LineInfo", L"AnalyzerModel", L"");

	// ��ȡ����Ա
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	CString str;
	CString strResult(L"[EquCalChkInfo]");

	// �����Ƿ���ɹ�����һ��
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce ? L"1" : L"0");
	strResult += str;

	// ����߱��
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strResult += str;

	// ����Ա
	str.Format(L"\r\nOperator=%s", strOperator);
	strResult += str;

	// �豸
	str.Format(L"\r\nEquipment=%s", strAnalyzerModel);
	strResult += str;

	// ��Ŀ
	str.Format(L"\r\nItem=%s", L"������������");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strFormat;
	strFormat.Format(L"[HC]:%%d(%%d);[CO]:%%.%df(%%.%df);[CO2]:%%.%df(%%.%df)", DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES);
	CString strData;
	strData.Format(strFormat, m_nHCMeasuredValue, m_nHCNominalValue, 
		(float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR, (float)m_nCONominalValue/AMPLIFICATION_FACTOR,
		(float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	strFormat.Format(L"HC���,����:%%d(��%%d),���:%%d(��%%d);CO���:����:%%.%df(��%%.%df),���:%%d(��%%d);CO2���:����:%%.%df(��%%.%df),���:%%d(��%%d)", 
		DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES, DECIMAL_PLACES);
	CString strError;
	strError.Format(strFormat, m_nHCAE, m_nHCAELimit, m_nHCRE, m_nHCRELimit,
		(float)m_nCOAE/AMPLIFICATION_FACTOR, (float)m_nCOAELimit/AMPLIFICATION_FACTOR, m_nCORE, m_nCORELimit,
		(float)m_nCO2AE/AMPLIFICATION_FACTOR, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR, m_nCO2RE, m_nCO2RELimit);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	str.Format(L"\r\nJudgement=%s", m_bIsCheckPass?L"ͨ��":L"��ͨ��");
	strResult += str;

	//���ݻ����ֵ��ض���ʽ�ı궨���ݱ�����Reserved1�ֶ�////////////////////////////////////////////////////////
	CString strHC_std,strHC_val,strHC_err,strCO_std,strCO_val,strCO_err,strCO2_std,strCO2_val,strCO2_err;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	strHC_std.Format(L"%d", m_nHCNominalValue);		//HC��׼ֵ
	strHC_val.Format(L"%d", m_nHCMeasuredValue);	//HC����ֵ
	strHC_err.Format(L"%d", m_nHCRE);				//HC������
	strCO_std.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);		//CO��׼ֵ
	strCO_val.Format(strFormat, (float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR);	//CO����ֵ
	strCO_err.Format(L"%d", m_nCORE);				//CO������
	strCO2_std.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);		//CO2��׼ֵ
	strCO2_val.Format(strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);	//CO2����ֵ
	strCO2_err.Format(L"%d", m_nCO2RE);				//CO2������
	str.Format(L"\r\nReserved1=%s;%s;%s;%s;%s;%s;%s;%s;%s", 
		strCO_std, strCO_val, strCO_err, strHC_std, strHC_val, strHC_err, strCO2_std, strCO2_val, strCO2_err); //д��˳���ܷ�
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// д�뵽���ini�ļ�
	CString strPathOfEquCalChkInfo = strPathOfDataExchange + L"\\EquCalChkInfo.ini";
	if (0 == _taccess(strPathOfEquCalChkInfo, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfEquCalChkInfo);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfEquCalChkInfo, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfEquCalChkInfo, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHA403XGasCheckDlg::ExportGasCheckRTData(void)
{
	// ĿǰΪ�գ�Ԥ��
}

void CNHA403XGasCheckDlg::ExportDemarcationLog(void)
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

	// ��ȡ���վ���
	CString strPathOfStationInfoFile = strPathOfDataExchange + L"\\StationInfo.ini";
	CSimpleIni siStationInfo(strPathOfStationInfoFile);
	CString strStationNumber = siStationInfo.GetString(L"StationInfo", L"StationNumber", L"");

	// ��ȡ����߱�źͷ������ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"AnalyzerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"AnalyzerModel", L"");

	// ��ȡ����Ա
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString str;
	CString strDemarcationLog(L"[DemarcationLog]");

	// �궨���
	str.Format(L"\r\nDemarcationNumber=%s-%s-%s", strStationNumber, strLineNumber, odt.Format(L"%Y%m%d%H%M%S"));
	strDemarcationLog += str;

	// ����߱��
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strDemarcationLog += str;

	// ����Ա
	str.Format(L"\r\nOperator=%s", strOperator);
	strDemarcationLog += str;

	// �豸���
	str.Format(L"\r\nEquipmentNumber=%s", strEquipmentNumber);
	strDemarcationLog += str;

	// �豸�ͺ�
	str.Format(L"\r\nEquipmentModel=%s", strEquipmentModel);
	strDemarcationLog += str;

	// �豸����
	str.Format(L"\r\nEquipmentName=%s", L"����������");
	strDemarcationLog += str;

	// �궨��Ŀ
	str.Format(L"\r\nDemarcationItem=%d", 6);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsCheckPass?L"1":L"0");
	strDemarcationLog += str;

	// д�뵽���ini�ļ�
	CString strPathOfDemarcationLog = strPathOfDataExchange + L"\\DemarcationLog.ini";
	if (0 == _taccess(strPathOfDemarcationLog, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfDemarcationLog);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfDemarcationLog, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfDemarcationLog, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strDemarcationLog);
	sfe.Close();
}

void CNHA403XGasCheckDlg::ExportGasCheckResult(void)
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
	// ��ȡ�궨���
	CString strPathOfDemarcationLogFile = strPathOfDataExchange + L"\\DemarcationLog.ini";
	CSimpleIni siDemarcationLog(strPathOfDemarcationLogFile);
	CString strDemarcationNumber = siDemarcationLog.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	CString str;
	CString strResult(L"[ResultOfAnaGasChk]");
//////////////////////////////////////////////////////////////////////////////
	// �궨���
	str.Format(L"\r\nDemarcationNumber=%s", strDemarcationNumber);
	strResult += str;

	// PEF
	str.Format(L"\r\nPEFMeasuredValue=%.3f", m_nPEFMeasuredValue/1000.0f);
	strResult += str;

	// C3H8��׼ֵ,��λ:ppm
	str.Format(L"\r\nC3H8StandardValue=%d", m_nC3H8NominalValue);
	strResult += str;
	
	// �Ƿ�����HC�ж�
	str.Format(L"\r\nSkipHCJudgement=0");
	strResult += str;
		
	// HC����ֵ,��λ:ppm
	str.Format(L"\r\nHCMeasuredValue=%d", m_nHCMeasuredValue);
	strResult += str;

	// HC���ֵ,��λ:ppm
	str.Format(L"\r\nHCStandardValue=%d", m_nHCNominalValue);
	strResult += str;

	// HC�������,��λ:ppm
	str.Format(L"\r\nHCAE=%d", m_nHCAE);
	strResult += str;

	// HC���������ֵ,��λ:ppm
	str.Format(L"\r\nHCAELimit=%d", m_nHCAELimit);
	strResult += str;

	// HC��������Ƿ�ϸ�
	str.Format(L"\r\nHCAEPass=%s", m_bIsHCAEPass?L"1":L"0");
	strResult += str;

	// HC������
	str.Format(L"\r\nHCRE=%d", m_nHCRE);
	strResult += str;

	// HC��������ֵ
	str.Format(L"\r\nHCRELimit=%d", m_nHCRELimit);
	strResult += str;

	// HC�������Ƿ�ϸ�
	str.Format(L"\r\nHCREPass=%s", m_bIsHCREPass?L"1":L"0");
	strResult += str;

	// HC�Ƿ�ϸ�
	str.Format(L"\r\nHCPass=%s", m_bIsHCPass?L"1":L"0");
	strResult += str;

	// �Ƿ�����CO�ж�
	str.Format(L"\r\nSkipCOJudgement=0");
	strResult += str;

	// CO����ֵ,��λ:%
	CString strFormat;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(L"\r\nCOMeasuredValue="+strFormat, (float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO���ֵ,��λ:%
	str.Format(L"\r\nCOStandardValue="+strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO�������,��λ:%
	str.Format(L"\r\nCOAE="+strFormat, (float)m_nCOAE/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO���������ֵ,��λ:%
	str.Format(L"\r\nCOAELimit="+strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO��������Ƿ�ϸ�
	str.Format(L"\r\nCOAEPass=%s", m_bIsCOAEPass?L"1":L"0");
	strResult += str;

	// CO������
	str.Format(L"\r\nCORE=%d", m_nCORE);
	strResult += str;

	// CO��������ֵ
	str.Format(L"\r\nCORELimit=%d", m_nCORELimit);
	strResult += str;

	// CO�������Ƿ�ϸ�
	str.Format(L"\r\nCOREPass=%s", m_bIsCOREPass?L"1":L"0");
	strResult += str;

	// CO�Ƿ�ϸ�
	str.Format(L"\r\nCOPass=%s", m_bIsCOPass?L"1":L"0");
	strResult += str;

	// �Ƿ�����CO2�ж�
	str.Format(L"\r\nSkipCO2Judgement=0");
	strResult += str;

	// CO2����ֵ,��λ:%
	str.Format(L"\r\nCO2MeasuredValue="+strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO2���ֵ,��λ:%
	str.Format(L"\r\nCO2StandardValue="+strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO2�������,��λ:%
	str.Format(L"\r\nCO2AE="+strFormat, (float)m_nCO2AE/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO2���������ֵ,��λ:%
	str.Format(L"\r\nCO2AELimit="+strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	strResult += str;

	// CO2��������Ƿ�ϸ�
	str.Format(L"\r\nCO2AEPass=%s", m_bIsCO2AEPass?L"1":L"0");
	strResult += str;

	// CO2������
	str.Format(L"\r\nCO2RE=%d", m_nCO2RE);
	strResult += str;

	// CO2��������ֵ
	str.Format(L"\r\nCO2RELimit=%d", m_nCO2RELimit);
	strResult += str;

	// CO2�������Ƿ�ϸ�
	str.Format(L"\r\nCO2REPass=%s", m_bIsCO2REPass?L"1":L"0");
	strResult += str;

	// CO2�Ƿ�ϸ�
	str.Format(L"\r\nCO2Pass=%s", m_bIsCO2Pass?L"1":L"0");
	strResult += str;

	// �Ƿ�����NO�ж�
	str.Format(L"\r\nSkipNOJudgement=1");
	strResult += str;

	// NO����ֵ,��λ:%
	str.Format(L"\r\nNOMeasuredValue=");
	strResult += str;

	// NO���ֵ,��λ:%
	str.Format(L"\r\nNOStandardValue=");
	strResult += str;

	// NO�������,��λ:%
	str.Format(L"\r\nNOAE=");
	strResult += str;

	// NO���������ֵ,��λ:%
	str.Format(L"\r\nNOAELimit=");
	strResult += str;

	// NO��������Ƿ�ϸ�
	str.Format(L"\r\nNOAEPass=");
	strResult += str;

	// NO������
	str.Format(L"\r\nNORE=");
	strResult += str;

	// NO��������ֵ
	str.Format(L"\r\nNORELimit=");
	strResult += str;

	// NO�������Ƿ�ϸ�
	str.Format(L"\r\nNOREPass=");
	strResult += str;

	// NO�Ƿ�ϸ�
	str.Format(L"\r\nNOPass=");
	strResult += str;

	// �Ƿ�����O2�ж�
	str.Format(L"\r\nSkipO2Judgement=1");
	strResult += str;

	// O2����ֵ,��λ:%
	str.Format(L"\r\nO2MeasuredValue=");
	strResult += str;

	// O2���ֵ,��λ:%
	str.Format(L"\r\nO2StandardValue=");
	strResult += str;

	// O2�������,��λ:%
	str.Format(L"\r\nO2AE=");
	strResult += str;

	// O2���������ֵ,��λ:%
	str.Format(L"\r\nO2AELimit=");
	strResult += str;

	// O2��������Ƿ�ϸ�
	str.Format(L"\r\nO2AEPass=");
	strResult += str;

	// O2������
	str.Format(L"\r\nO2RE=");
	strResult += str;

	// O2��������ֵ
	str.Format(L"\r\nO2RELimit=");
	strResult += str;

	// O2�������Ƿ�ϸ�
	str.Format(L"\r\nO2REPass=");
	strResult += str;

	// O2�Ƿ�ϸ�
	str.Format(L"\r\nO2Pass=");
	strResult += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsCheckPass?L"1":L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	CString strPathOfAnaGasCheckResult = strPathOfDataExchange + L"\\ResultOfAnaGasChk.ini";
	if (0 == _taccess(strPathOfAnaGasCheckResult, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfAnaGasCheckResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfAnaGasCheckResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfAnaGasCheckResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHA403XGasCheckDlg::OpenSerialPortAndTestCommunication(void)
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
	// ��ȡ������Э������,3��NHA503AB,4��ͳһͨ��Э��
	CString strAnaProtocolVersion = si.GetString(L"AnaParams", L"ProtocolVersion", L"4");
	m_nAnaProtocolVersion = _wtoi(strAnaProtocolVersion);
	// ��ȡ������ͨѶЭ������,Ĭ��Ϊ503
	CString strAnaProtocol = si.GetString(L"AnaParams", L"Protocol", L"503");
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
					m_lbInfo.SetText(_T("������������"));
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

void CNHA403XGasCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case 1:
		{
			m_pAnaThread->GetMajorData_Asyn(NULL);
			m_pAnaThread->GetPef_Asyn(NULL);

			// ������
			short sHC(0);
			float fCO(0.0f);
			float fCO2(0.0f);
			float fO2(0.0f);
			short sNO(0);
			USHORT usRpm(0);
			float fOilTemp(0.0f);
			USHORT usFlux(0);
			float fLambda(0.0f);
			float fPef(0.0f);

			sHC = m_pAnaThread->GetMajorDataHCValue();
			fCO = m_pAnaThread->GetMajorDataCOValue();
			fCO2 = m_pAnaThread->GetMajorDataCO2Value();
			fO2 = m_pAnaThread->GetMajorDataO2Value();
			sNO = m_pAnaThread->GetMajorDataNOValue();
			usRpm = m_pAnaThread->GetMajorDataRpmValue();
			fOilTemp = m_pAnaThread->GetMajorDataOilTempValue();
			usFlux = m_pAnaThread->GetMajorDataFluxValue();
			fLambda = m_pAnaThread->GetMajorDataLambdaValue();
			fPef = m_pAnaThread->GetPefValue();

			CString str;

			str.Format(_T("%d"),sHC);
			m_lbHC.SetText(str);

			// PEF��һ�����仯ʱ,����ʾC3H8
			if (fPef > 0.1f)
			{
				float fC3H8 = sHC / fPef;
				str.Format(_T("%.0f"), fC3H8);
				m_lbC3H8.SetText(str);
			}
			else
			{
				str = _T("0");
				m_lbC3H8.SetText(str);
			}

			CString strFormat;

			strFormat.Format(L"%%.%df", DECIMAL_PLACES);
			str.Format(strFormat, fCO);
			m_lbCO.SetText(str);
			str.Format(strFormat, fCO2);
			m_lbCO2.SetText(str);
			str.Format(strFormat, fO2);
			m_lbO2.SetText(str);
			str.Format(_T("%.3f"), fPef);
			m_lbPEF.SetText(str);
		}
		break;

	case 2:
		{
			KillTimer(2);
			OpenSerialPortAndTestCommunication();
		}
		break;

	case 3:
		{
			if (3 == m_nAnaProtocolVersion) // NHA503AB
			{
				BYTE bResult(0);
				m_pAnaThread->Zero(&bResult);
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
				else
				{
					// ����δ�������
					//KillTimer(3);
					TRACE("����δ�������\n");
				}
			}
			else
			{
				BYTE bStatus(0);
				m_pAnaThread->GetStatus(&bStatus);
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
		}
		break;
	case 4:
		{
			// ������ʱ��ѯ������
			KillTimer(4);
			SetTimer(3, 1000, NULL);

			// ��ʱ����,���¿���"ֹͣ���"��ť
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		}
		break;
	case 5:
		{
			// ������ʱˢ������
			KillTimer(5);

			GasCheckProcess();
		}
		break;
	default:
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHA403XGasCheckDlg::ResetProcess()
{
	//m_lbInfo.StopFlashText();

	// ���ʵʱֵ��ʾ
	ClearRTDisplay();

	// PEF����ֵ��ʾ��
	m_lbPEFMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbPEFMeasuredValue.SetText(L"--");

	// HC����ֵ��ʾ��
	m_lbHCMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbHCMeasuredValue.SetText(L"--");
	// CO����ֵ��ʾ��
	m_lbCOMeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbCOMeasuredValue.SetText(L"--");
	// CO2����ֵ��ʾ��
	m_lbCO2MeasuredValue.SetTextColor(RGB(0, 0, 0));
	m_lbCO2MeasuredValue.SetText(L"--");

	// HC���ֵ��ʾ��
	m_lbHCNominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbHCNominalValue.SetText(L"--");
	// CO���ֵ��ʾ��
	m_lbCONominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbCONominalValue.SetText(L"--");
	// CO2���ֵ��ʾ��
	m_lbCO2NominalValue.SetTextColor(RGB(0, 0, 0));
	m_lbCO2NominalValue.SetText(L"--");

	//////////////////////////////////////// ��ʼ������������������ֵstart
	CString strFormat;
	CString str;
	// HC���������ʾ��
	m_lbHCAE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nHCAELimit);
	m_lbHCAE.SetText(str);
	// CO���������ʾ��
	m_lbCOAE.SetTextColor(RGB(0, 0, 0));
	strFormat.Format(L"--(��%%.%df)", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	m_lbCOAE.SetText(str);
	// CO2���������ʾ��
	m_lbCO2AE.SetTextColor(RGB(0, 0, 0));
	str.Format(strFormat, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);

	// HC��������ʾ��
	m_lbHCRE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nHCRELimit);
	m_lbHCRE.SetText(str);
	// CO��������ʾ��
	m_lbCORE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nCORELimit);
	m_lbCORE.SetText(str);
	// CO2��������ʾ��
	m_lbCO2RE.SetTextColor(RGB(0, 0, 0));
	str.Format(L"--(��%d)", m_nCO2RELimit);
	m_lbCO2RE.SetText(str);
	//////////////////////////////////////// ��ʼ������������������ֵstop

	// HC�ж���ʾ��
	m_lbHCJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbHCJudgement.SetText(L"--");
	// CO�ж���ʾ��
	m_lbCOJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbCOJudgement.SetText(L"--");
	// CO2�ж���ʾ��
	m_lbCO2Judgement.SetTextColor(RGB(0, 0, 0));
	m_lbCO2Judgement.SetText(L"--");

	// �þ���ʾ��
	m_lbDetermination.SetTextColor(RGB(0, 0, 0));
	m_lbDetermination.SetText(L"--");

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

	// ��������
	m_nStep = 0;
}

void CNHA403XGasCheckDlg::ClearRTDisplay(void)
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
	// PEFʵʱֵ
	m_lbPEF.SetText(L"--");
}

void CNHA403XGasCheckDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CNHA403XGasCheckDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (GetKeyState(VK_RMENU)<0 && GetKeyState('S')<0)
	{
		// ��������
		ResetProcess();
		// ���Alt+S,����������(�ұ�Alt��,s�����ִ�Сд)
		m_nStep = 1;
	}

	GasCheckProcess();
}

void CNHA403XGasCheckDlg::GasCheckProcess(void)
{
	if (0 == m_nStep)
	{
		// ��������
		ResetProcess();

		// ��ʼ���ǰ,����һ��ͨѶ
		if (ANA_WR_SUCCEED != m_pAnaThread->Zero())
		{
			m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			return;
		}

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�����������鿪ʼ", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_ANA_GAS_CHK_START, wchInfo, sizeof(wchInfo));
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
		//m_lbInfo.StopFlashText();
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		m_lbInfo.SetText(_T("����������ֵ,Ȼ����һ��"));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////����������ֵ����start
		if (0x00 != GetNominalValue())
		{
			// ���ֵ���벻����Ҫ��
			return;
		}
		// ������ֵ��ini�����ļ�
		SaveNominalValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////����������ֵ����stop

		//m_lbInfo.StopFlashText();

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		m_lbInfo.SetText(_T("��ͨ������,���ȶ�����һ��"));

		// ˢ������,��ִ��2������,ʱ����Ҫ����1000ms
		SetTimer(1, 1500, NULL);

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		// ��ɼ��

		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		KillTimer(1);

		// ��ǲ����Ѿ��������1��
		m_bIsOperationFinishedAtLeaseOnce = true;

		// ������
		CalculateResult();
		// ��ʾ���
		ShowResult();

		// ���µ���˳���ܵ���,ȷ��DemarcationLog��Result֮ǰ����
		// �����궨��־
		ExportDemarcationLog();
		// ����������ʵʱ����
		ExportGasCheckRTData();
		// ������������
		ExportGasCheckResult();
		// ������������
		ExportEquCalChkInfo();
		
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"���������������", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_ANA_GAS_CHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));

		m_lbInfo.SetText(_T("���������"));

		// ���ʵʱֵ��ʾ
		ClearRTDisplay();

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

		// ���ò���
		m_nStep = 0;
	}
}

DWORD CNHA403XGasCheckDlg::GetNominalValue(void)
{
	CString strC3H8, strCO, strCO2;
	//��ȡ�������ֵ������Ƿ�ǿ�
	GetDlgItem(IDC_EDIT_C3H8_NOMINAL_VALUE)->GetWindowText(strC3H8);
	strC3H8.Trim();
	if (L"" == strC3H8)
	{
		m_edC3H8NominalValue.SetFocus();
		MessageBox(_T("����������C3H8���ֵ"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	GetDlgItem(IDC_EDIT_CO_NOMINAL_VALUE)->GetWindowText(strCO);
	strCO.Trim();
	if (L"" == strCO)
	{
		m_edCONominalValue.SetFocus();
		MessageBox(_T("����������CO���ֵ"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	GetDlgItem(IDC_EDIT_CO2_NOMINAL_VALUE)->GetWindowText(strCO2);
	strCO2.Trim();
	if (L"" == strCO2)
	{
		m_edCO2NominalValue.SetFocus();
		MessageBox(_T("����������CO2���ֵ"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// ��¼���ֵ
	// ��׼��HC���ֵ,��λ:ppm
	m_nC3H8NominalValue = _wtoi(strC3H8);
	if (m_nC3H8NominalValue <= 0)
	{
		m_edC3H8NominalValue.SetFocus();
		MessageBox(_T("�����C3H8���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// ��׼��CO���ֵ,��λ:0.01%(�ŵ�100��)
	float f = (float)_wtof(strCO);
	m_nCONominalValue = int(f * AMPLIFICATION_FACTOR + 0.5f);
	if (m_nCONominalValue <= 0)
	{
		m_edCONominalValue.SetFocus();
		MessageBox(_T("�����CO���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// ��׼��CO2���ֵ,��λ:0.01%(�ŵ�100��)
	f = (float)_wtof(strCO2);
	m_nCO2NominalValue = int(f * AMPLIFICATION_FACTOR + 0.5f);
	if (m_nCO2NominalValue <= 0)
	{
		m_edCO2NominalValue.SetFocus();
		MessageBox(_T("�����CO2���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CNHA403XGasCheckDlg::SaveNominalValueToIniFile(void)
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
	si.SetString(L"AnaChk", L"C3H8NominalValue", str);
	CString strFormat;
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	si.SetString(L"AnaChk", L"CONominalValue", str);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	si.SetString(L"AnaChk", L"CO2NominalValue", str);
	///////////////////////////////////////////////////////////// ��¼�������ļ�stop
}

void CNHA403XGasCheckDlg::CalculateResult(void)
{
	// ��ȡһ���������ݽ��н���ж�
	// ������
	short sHC(0);
	float fCO(0.0f);
	float fCO2(0.0f);
	float fO2(0.0f);
	short sNO(0);
	short sNO2(0);
	short sNOx(0);
	USHORT usRpm(0);
	float fOilTemp(0.0f);
	USHORT usFlux(0);
	float fLambda(0.0f);
	m_pAnaThread->GetMajorData(&sHC, &fCO, &fCO2, &fO2, &sNO, &sNO2, &sNOx, &usRpm, &fOilTemp, &usFlux, &fLambda);
	float fPef(0.0f);
	m_pAnaThread->GetPef(&fPef);

	// PEFǿ������
	if (fPef < 0.3)
	{
		fPef = 0.3f;
	}
	// PEF����ֵ,�Ŵ�1000��,��������
	m_nPEFMeasuredValue = int(fPef * 1000 + 0.5f);

	// ��C3H8���ֵת����HC���ֵ
	m_nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
	// ������Լ��
	if (m_nHCNominalValue == 0)
	{
		m_nHCNominalValue = 1;
	}
	// HCͨ���ж�
	m_nHCMeasuredValue = sHC;
	// �������
	m_nHCAE = m_nHCMeasuredValue - m_nHCNominalValue;
	if (abs(m_nHCAE) > m_nHCAELimit)
	{
		// HC�������ϸ�
		m_bIsHCAEPass = false;
	}
	else
	{
		// HC�������ϸ�
		m_bIsHCAEPass = true;
	}
	// ������
	m_nHCRE = m_nHCAE * 100 / m_nHCNominalValue;
	if (abs(m_nHCRE) > m_nHCRELimit)
	{
		// HC�������ϸ�
		m_bIsHCREPass = false;
	}
	else
	{
		// HC������ϸ�
		m_bIsHCREPass = true;
	}
	if (m_bIsHCAEPass || m_bIsHCREPass)
	{
		// HC���ϸ�
		m_bIsHCPass = true;
	}
	else
	{
		// HC��鲻�ϸ�
		m_bIsHCPass =false;
	}

	// COͨ���ж�
	if (fCO < -10e-6f)
	{
		// fCOΪ����,����0.01
		m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR - 0.5f);
	}
	else
	{
		// fCOΪ������
		m_nCOMeasuredValue = int(fCO * AMPLIFICATION_FACTOR + 0.5f);
	}
	// �������
	m_nCOAE = m_nCOMeasuredValue - m_nCONominalValue;
	// CO���������ֵ
	ASSERT(m_nCOAELimit > -10e-6);
	if (abs(m_nCOAE) > m_nCOAELimit)
	{
		// CO�������ϸ�
		m_bIsCOAEPass = false;
	}
	else
	{
		// CO�������ϸ�
		m_bIsCOAEPass = true;
	}
	// ������Լ��
	if (m_nCONominalValue == 0)
	{
		m_nCONominalValue = 1;
	}
	// ������
	m_nCORE = m_nCOAE * 100 / m_nCONominalValue;
	if (abs(m_nCORE) > m_nCORELimit)
	{
		// CO������ϸ�
		m_bIsCOREPass = false;
	}
	else
	{
		// CO������ϸ�
		m_bIsCOREPass = true;
	}
	if (m_bIsCOAEPass || m_bIsCOREPass)
	{
		// CO���ϸ�
		m_bIsCOPass = true;
	}
	else
	{
		// CO��鲻�ϸ�
		m_bIsCOPass = false;
	}

	// CO2ͨ���ж�
	if (fCO2 < -10e-6f)
	{
		// fCO2Ϊ����,����:0.01
		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR - 0.5f);
	}
	else
	{
		// fCO2Ϊ����
		m_nCO2MeasuredValue = int(fCO2 * AMPLIFICATION_FACTOR + 0.5f);
	}
	// �������
	m_nCO2AE = m_nCO2MeasuredValue - m_nCO2NominalValue;
	// CO2���������ֵ
	ASSERT(m_nCO2AELimit > -10e-6);
	if (abs(m_nCO2AE) > m_nCO2AELimit)
	{
		// CO2�������ϸ�
		m_bIsCO2AEPass = false;
	}
	else
	{
		// CO2�������ϸ�
		m_bIsCO2AEPass = true;
	}
	// ������Լ��
	if (m_nCO2NominalValue == 0)
	{
		m_nCO2NominalValue = 1;
	}
	// ������
	m_nCO2RE = m_nCO2AE * 100 / m_nCO2NominalValue;
	if (abs(m_nCO2RE) > m_nCO2RELimit)
	{
		// CO2������ϸ�
		m_bIsCO2REPass = false;
	}
	else
	{
		// CO2������ϸ�
		m_bIsCO2REPass = true;
	}
	if (m_bIsCO2AEPass || m_bIsCO2REPass)
	{
		// CO2���ϸ�
		m_bIsCO2Pass = true;
	}
	else
	{
		// CO2��鲻�ϸ�
		m_bIsCO2Pass = false;
	}

	// ����Ƿ�ͨ��
	if (m_bIsHCPass && m_bIsCOPass && m_bIsCO2Pass)
	{
		m_bIsCheckPass = true;
	}
	else
	{
		m_bIsCheckPass = false;
	}

}

void CNHA403XGasCheckDlg::ShowResult(void)
{
	CString str;
	CString strFormat;

	// ��ʾPEFֵ
	str.Format(L"%.3f", m_nPEFMeasuredValue/1000.0f);
	m_lbPEFMeasuredValue.SetText(str);

	// ��ʾ���ֵ
	str.Format(L"%d", m_nHCNominalValue);
	m_lbHCNominalValue.SetText(str);
	strFormat.Format(L"%%.%df", DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCONominalValue/AMPLIFICATION_FACTOR);
	m_lbCONominalValue.SetText(str);
	str.Format(strFormat, (float)m_nCO2NominalValue/AMPLIFICATION_FACTOR);
	m_lbCO2NominalValue.SetText(str);

	// ��ʾ����ֵ
	str.Format(L"%d", m_nHCMeasuredValue);
	m_lbHCMeasuredValue.SetText(str);
	str.Format(strFormat, (float)m_nCOMeasuredValue/AMPLIFICATION_FACTOR);
	m_lbCOMeasuredValue.SetText(str);
	str.Format(strFormat, (float)m_nCO2MeasuredValue/AMPLIFICATION_FACTOR);
	m_lbCO2MeasuredValue.SetText(str);

	// ��ʾ��������ֵ
	str.Format(L"%d(��%d)", m_nHCAE, m_nHCAELimit);
	m_lbHCAE.SetText(str);
	if (m_bIsHCAEPass)
	{
		m_lbHCAE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbHCAE.SetTextColor(RGB(255, 0, 0));
	}
	strFormat.Format(L"%%.%df(��%%.%df)", DECIMAL_PLACES, DECIMAL_PLACES);
	str.Format(strFormat, (float)m_nCOAE/AMPLIFICATION_FACTOR, (float)m_nCOAELimit/AMPLIFICATION_FACTOR);
	m_lbCOAE.SetText(str);
	if (m_bIsCOAEPass)
	{
		m_lbCOAE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbCOAE.SetTextColor(RGB(255, 0, 0));
	}
	str.Format(strFormat, (float)m_nCO2AE/AMPLIFICATION_FACTOR, (float)m_nCO2AELimit/AMPLIFICATION_FACTOR);
	m_lbCO2AE.SetText(str);
	if (m_bIsCO2AEPass)
	{
		m_lbCO2AE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbCO2AE.SetTextColor(RGB(255, 0, 0));
	}

	// ��ʾ�������ֵ
	str.Format(L"%d(��%d)", m_nHCRE, m_nHCRELimit);
	m_lbHCRE.SetText(str);
	if (m_bIsHCREPass)
	{
		m_lbHCRE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbHCRE.SetTextColor(RGB(255, 0, 0));
	}
	str.Format(L"%d(��%d)", m_nCORE, m_nCORELimit);
	m_lbCORE.SetText(str);
	if (m_bIsCOREPass)
	{
		m_lbCORE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbCORE.SetTextColor(RGB(255, 0, 0));
	}
	str.Format(L"%d(��%d)", m_nCO2RE, m_nCO2RELimit);
	m_lbCO2RE.SetText(str);
	if (m_bIsCO2REPass)
	{
		m_lbCO2RE.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbCO2RE.SetTextColor(RGB(255, 0, 0));
	}

	// ��ʾͨ�����
	// HCͨ��
	if (m_bIsHCPass)
	{
		m_lbHCJudgement.SetTextColor(RGB(0, 128, 0));
		m_lbHCJudgement.SetText(L"�ϸ�");
	}
	else
	{
		m_lbHCJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbHCJudgement.SetText(L"���ϸ�");
	}
	// COͨ��
	if (m_bIsCOPass)
	{
		m_lbCOJudgement.SetTextColor(RGB(0, 128, 0));
		m_lbCOJudgement.SetText(L"�ϸ�");
	}
	else
	{
		m_lbCOJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbCOJudgement.SetText(L"���ϸ�");
	}
	// CO2ͨ��
	if (m_bIsCO2Pass)
	{
		m_lbCO2Judgement.SetTextColor(RGB(0, 128, 0));
		m_lbCO2Judgement.SetText(L"�ϸ�");
	}
	else
	{
		m_lbCO2Judgement.SetTextColor(RGB(255, 0, 0));
		m_lbCO2Judgement.SetText(L"���ϸ�");
	}

	// ��ʾ�ܽ��
	if (m_bIsCheckPass)
	{
		m_lbDetermination.SetTextColor(RGB(0, 128, 0));
		m_lbDetermination.SetText(L"ͨ��");
	}
	else
	{
		m_lbDetermination.SetTextColor(RGB(255, 0, 0));
		m_lbDetermination.SetText(L"��ͨ��");
	}
}

void CNHA403XGasCheckDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	KillTimer(1);
	KillTimer(3);

	// ���ü������
	ResetProcess();

	m_lbInfo.SetText(_T("������ֹͣ"));

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	
	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"������������ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_ANA_GAS_CHK_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHA403XGasCheckDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nID == SC_RESTORE && lParam != SW_HIDE)
		return;
	CDialogZoom::OnSysCommand(nID, lParam);
}

void CNHA403XGasCheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}