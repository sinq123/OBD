// FlowmeterFlowCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterFlowCalibrationDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterFlowCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterFlowCalibrationDlg, CDialogZoom)

CFlowmeterFlowCalibrationDlg::CFlowmeterFlowCalibrationDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CFlowmeterFlowCalibrationDlg::IDD, pParent)
// ����
, m_nStep(0)
// �����Ƿ��������1��
, m_bIsOperationFinishedAtLeaseOnce(false)
// У׼����Ƿ�Pass
, m_bIsPass(false)
// ��������׼ֵ,��λ:L/s
, m_fStandardValueOfHighFlux(0.0f)
// ��������׼ֵ,��λ:L/s
, m_fStandardValueOfLowFlux(0.0f)
, m_fMaximumZeroOffsetValue(36.7f)
, m_pfProcessCtrl(NULL)
, m_bDomestic(false)
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

	// �������
	ImportParams();

	//m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pFloThread = CNHEuqController::GetInstance().m_pFloThread;
}

CFlowmeterFlowCalibrationDlg::~CFlowmeterFlowCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterFlowCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_STATIC_FLUX, m_lbFlux);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP5_LABEL, m_stcStep5Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_HIGH_FLUX, m_neStandardValueHighFlux);
	DDX_Control(pDX, IDC_EDIT_STANDARD_VALUE_LOW_FLUX, m_neStandardValueLowFlux);
}


BEGIN_MESSAGE_MAP(CFlowmeterFlowCalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_HIGH_FLUX, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationHighFlux)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_LOW_FLUX, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationLowFlux)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_RADIO1, &CFlowmeterFlowCalibrationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFlowmeterFlowCalibrationDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CFlowmeterFlowCalibrationDlg ��Ϣ�������

BOOL CFlowmeterFlowCalibrationDlg::OnInitDialog()
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

void CFlowmeterFlowCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterFlowCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"����������У׼");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// ����ʵʱֵ;
	m_lbFlux.SetLEDStyle();
	m_lbFlux.SetText(L"--");

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	// ���水ť
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	EnableBasicOperation(false);
}

void CFlowmeterFlowCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		DisplayRTData();
	}
	else if (3 == nIDEvent)
	{
		KillTimer(3);

		// ��������
		Process();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterFlowCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
	const CString strConnection = si.GetString(L"AnaParams", L"ConnectToFlo", L"0");
	const bool bAnaConnectToFlo = (L"0" != strConnection); // �������Ƿ����ӵ�������
	BYTE bPort(0);
	if (bAnaConnectToFlo)
	{
		// ��ȡͨѶ�˿�(�ӷ����ǻ�ȡ)
		bPort = (BYTE)_wtoi(si.GetString(L"AnaParams", L"Port", L"3"));
	}
	else
	{
		// ��ȡͨѶ�˿�(�������ƻ�ȡ)
		bPort = (BYTE)_wtoi(si.GetString(L"FloParams", L"Port", L"4"));
	}
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

	//if (ANA_OPEN_SUCCEED == m_pFloThread->Open(bPort, strAnaProtocol, true))
	if (m_pFloThread->IsOpen())
	{
		// ����ͨѶ
		BYTE bResult(0);
		if (ANA_WR_SUCCEED == m_pFloThread->SetOperationMode(CAna::ToFlowmeter, &bResult))
		{
			// ͨѶ�ɹ�
			switch (bResult)
			{
			case 0x06:
				{
					// �ɹ��л��������Ʋ���ģʽ

					// ˢ��ʵʱ����
					RefreshRTData(true);

					// ����
					m_lbInfo.SetText(_T("����������У׼"));
					// λ��ʵʱͨѶ����
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
					EnableBasicOperation(true);
				}
				break;
			case 0x05:
				{
					// ͨѶ����������δ�л��������Ʋ���ģʽ
					// ԭ��������æ

					// ˢ��ʵʱ����
					RefreshRTData(true);

					// ����
					m_lbInfo.SetText(_T("����������У׼"));
					// λ��ʵʱͨѶ����
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
					EnableBasicOperation(true);
				}
				break;
			case 0x15:
				{
					// ͨѶ���󣺷���0x15
					m_lbInfo.SetText(_T("ͨѶ����"));
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
					EnableBasicOperation(false);
				}
				break;
			default:
				{
					// ͨѶ���󣺲���ʶ�����
					m_lbInfo.SetText(L"ͨѶ����");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
					EnableBasicOperation(false);
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
			GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
			EnableBasicOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(false);
	}

#ifdef _DEBUG
	// ˢ��ʵʱ����
	RefreshRTData(true);

	// ����
	m_lbInfo.SetText(_T("����������У׼"));
	// λ��ʵʱͨѶ����
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
	EnableBasicOperation(true);
#endif

}

void CFlowmeterFlowCalibrationDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterFlowCalibrationDlg::DisplayRTData(void)
{
	m_pFloThread->GetRealTimeData_Flowmeter_Aysn();

	float f = m_pFloThread->GetVmasRTDFluxStd20Value();
	// ��ƫ��ֵԼ��
	if (f < m_fMaximumZeroOffsetValue)
	{
		f = 0.0f;
	}
	CString str;
	str.Format(L"%.2f", f);
	m_lbFlux.SetText(str);
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);

	Process();

}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �ر����������Ƿ��
	m_pFloThread->RunFan_Flowmeter(false);

	// ��������
	ResetProcess();

	m_lbInfo.SetText(_T("����У׼ֹͣ"));
		
	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_FLUXCAL_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);

	EnableBasicOperation(true);
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationHighFlux()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ��ȡ��������׼ֵ
	if (0x00 != GetStandardValueOfHighFlux())
	{
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ������
	RefreshRTData(false);

	// д��У׼ֵ��У׼
	BYTE bResult(0x00);
	CAna::FluxCalibrationMode nFCM;
	CString strFCM(L"");
	CString strMsg(L"");
	if (m_bDomestic)
	{
		nFCM = CAna::SinglePointFlowCal;
		strFCM.Format(L"��������");
	}
	else
	{
		nFCM = CAna::HighFlowCal;
		strFCM.Format(L"������");
	}

	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, nFCM, m_fStandardValueOfHighFlux);
	if (0x06 == bResult)
	{
		// д���/����������׼ֵ�ɹ�

		BYTE bCalResult(0);
		
		m_pFloThread->FluxCalibration_Flowmeter(&bResult, &bCalResult, nFCM);
		if (0x06==bResult && 0x00==bCalResult)
		{
			// ��/��������У׼�ɹ�
			strMsg.Format(L"%sУ׼�ɹ�", strFCM);
			MessageBox(strMsg, L"����У׼", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			// ��/��������У׼ʧ��
			strMsg.Format(L"%sУ׼ʧ��", strFCM);
			MessageBox(strMsg, L"����У׼", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		// д���/����������׼ֵʧ��
		strMsg.Format(L"%sУ׼ʧ�ܣ�д��%s��׼ֵʧ��", strFCM, strFCM);
		MessageBox(strMsg, L"����У׼", MB_OK|MB_ICONWARNING);
	}

	// ��ʼˢ������
	RefreshRTData(true);

	EndWaitCursor();
}

DWORD CFlowmeterFlowCalibrationDlg::GetStandardValueOfHighFlux(void)
{
	// ��ȡ��׼ֵ�����
	CString strStandardValue;
	m_neStandardValueHighFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueHighFlux.SetFocus();
		if (m_bDomestic)
		{
			MessageBox(_T("�����뵥��������׼ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
		}
		else
		{
			MessageBox(_T("�������������׼ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
		}
		return 0x01;
	}
	// ������׼ֵ,��λ:L/s
	float f = (float)_wtof(strStandardValue);
	ASSERT(f > -10e-6f);
	// ��ʽ��Ϊ2ΪС��������������
	m_fStandardValueOfHighFlux = int(f * 100.0f + 0.5f) / 100.0f;
	// ������Լ��:ͨ�ø�����(50L/s~500L/s) �ϻ�����(50L/s~235L/s)
	if (m_bDomestic)
	{
		if ((m_fStandardValueOfHighFlux<50.0f || m_fStandardValueOfHighFlux>235.0f))
		{
			m_neStandardValueHighFlux.SetFocus();
			MessageBox(_T("����������׼ֵ���ں���Χ(50L/s~235L/s)"), _T("����У׼"), MB_OK|MB_ICONWARNING);
			return 0x01;
		}
	}
	else
	{
		if (m_fStandardValueOfHighFlux<50.0f || m_fStandardValueOfHighFlux>500.0f)
		{
			m_neStandardValueHighFlux.SetFocus();
			MessageBox(_T("��������׼ֵ���ں���Χ(50L/s~500L/s)"), _T("����У׼"), MB_OK|MB_ICONWARNING);

			return 0x01;
		}
	}
	return 0x00;
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonCalibrationLowFlux()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ��ȡ��������׼ֵ
	if (0x00 != GetStandardValueOfLowFlux())
	{
		return;
	}

	BeginWaitCursor();

	// ֹͣˢ������
	RefreshRTData(false);

	// д��У׼ֵ��У׼
	BYTE bResult(0x00);
	m_pFloThread->SetCalibrationFlux_Flowmeter(&bResult, CAna::LowFlowCal, m_fStandardValueOfLowFlux);
	if (0x06 == bResult)
	{
		// д���������׼ֵ�ɹ�

		BYTE bCalResult(0);
		m_pFloThread->FluxCalibration_Flowmeter(&bResult, &bCalResult, CAna::LowFlowCal);
		if (0x06==bResult && 0x00==bCalResult)
		{
			// ������У׼�ɹ�

			MessageBox(L"������У׼�ɹ�", L"����У׼", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			// ������У׼ʧ��

			MessageBox(L"������У׼ʧ��", L"����У׼", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		// д���������׼ֵʧ��

		MessageBox(L"������У׼ʧ�ܣ�д���������׼ֵʧ��", L"����У׼", MB_OK|MB_ICONWARNING);
	}

	// ��ʼˢ������
	RefreshRTData(true);

	EndWaitCursor();
}

DWORD CFlowmeterFlowCalibrationDlg::GetStandardValueOfLowFlux(void)
{
	// ��ȡ��׼ֵ�����
	CString strStandardValue;
	m_neStandardValueLowFlux.GetWindowText(strStandardValue);
	strStandardValue.Trim();
	if (L"" == strStandardValue)
	{
		m_neStandardValueLowFlux.SetFocus();
		MessageBox(_T("�������������׼ֵ"), _T("����У׼"), MB_OK|MB_ICONWARNING);
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

void CFlowmeterFlowCalibrationDlg::Process(void)
{
	if (0 == m_nStep)
	{
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼��ʼ", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCAL_START, wchInfo, sizeof(wchInfo));
		}
		
		// ���ӱ�׼������

		// ��������
		ResetProcess();

		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("�����ӱ�׼������,Ȼ����һ��"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(true);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		// �򿪷��

		BeginWaitCursor();

		// ֹͣˢ��ʵʱ����
		RefreshRTData(false);

		// �����������Ƿ��
		m_pFloThread->RunFan_Flowmeter(true);

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();
		
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("���ڴ򿪷��..."));

		// ��ʱ2���Զ�������һ��
		SetTimer(3, 2000, NULL);

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(true);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		// ������У׼ �� ��������У׼

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		if (m_bDomestic)
		{
			m_lbInfo.SetText(_T("����е�������У׼,Ȼ����һ��"));
			m_nStep = 4;
		}
		else
		{
			m_lbInfo.SetText(_T("����и�����У׼,Ȼ����һ��"));
			m_nStep = 3;
		}
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		EnableBasicOperation(true);
	}
	else if (3 == m_nStep)
	{
		// ������У׼
		BeginWaitCursor();
		
		// �ر����������Ƿ��
		m_pFloThread->RunFan_Flowmeter(false);
		
		EndWaitCursor();

		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("����е�����У׼,Ȼ����һ��"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		m_nStep = 4;
	}
	else if (4 == m_nStep)
	{
		BeginWaitCursor();

		// ֹͣˢ��ʵʱ����
		RefreshRTData(false);

		// �ر����������Ƿ��
		m_pFloThread->RunFan_Flowmeter(false);

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();

		if (m_bDomestic)
		{
			m_stcStep3Label.ShowWindow(SW_SHOW);
			m_lbStep3.SetTextColor(RGB(0, 0, 0));
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));
		}
		else
		{
			m_stcStep4Label.ShowWindow(SW_SHOW);
			m_lbStep4.SetTextColor(RGB(0, 0, 0));
			m_lbStep5.SetTextColor(RGB(0, 0, 255));
			m_stcStep5Label.ShowWindow(SW_SHOW);
			m_lbStep5.SetTextColor(RGB(0, 0, 0));
		}

		m_lbInfo.SetText(_T("У׼���"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		// �������
		ExportFloFluxCalResult();

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"����������У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		// ��������
		m_nStep = 0;
	}
}

void CFlowmeterFlowCalibrationDlg::ResetProcess(void)
{
	// ������ʾ��
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));
	m_lbStep4.SetTextColor(RGB(0,0,0));
	m_lbStep5.SetTextColor(RGB(0,0,0));

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);
	
	// ��������
	m_nStep = 0;
	// У׼����ó�ʼ��ֵ
	m_bIsPass = true;
}

void CFlowmeterFlowCalibrationDlg::ImportParams(void)
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

void CFlowmeterFlowCalibrationDlg::ExportFloFluxCalResult()
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
	strData.Format(L"��������׼ֵ:%.2fL/s,��������׼ֵ%.2fL/s", m_fStandardValueOfHighFlux, m_fStandardValueOfLowFlux);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// �Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass ? L"1" : L"0");
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

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedButtonFanOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterFlowCalibrationDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterFlowCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CFlowmeterFlowCalibrationDlg::ShowEntrance()
{
	m_bDomestic = false;
	m_lbStep3.SetWindowTextW(L"������У׼");
	m_lbStep4.SetWindowTextW(L"������У׼");
	m_lbStep5.ShowWindow(SW_SHOW);
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);


	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_STANDARD_VALUE_LOW_FLUX)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_LFC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_HFC)->SetWindowText(L"������У׼");
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->SetWindowText(L"������У׼");
	m_neStandardValueHighFlux.SetWindowText(L"");
	m_neStandardValueLowFlux.SetWindowText(L"");
}

void CFlowmeterFlowCalibrationDlg::ShowDomestic()
{
	m_bDomestic = true;
	m_lbStep4.SetWindowTextW(L"���У׼");
	m_lbStep3.SetWindowTextW(L"��������У׼");
	m_lbStep5.ShowWindow(SW_HIDE);
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_CALIBRATION_LOW_FLUX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_STANDARD_VALUE_LOW_FLUX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_LFC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_HFC)->SetWindowText(L"��������У׼");
	GetDlgItem(IDC_BUTTON_CALIBRATION_HIGH_FLUX)->SetWindowText(L"��������У׼");
	m_neStandardValueHighFlux.SetWindowText(L"");
	m_neStandardValueLowFlux.SetWindowText(L"");
}

void CFlowmeterFlowCalibrationDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowEntrance();
}


void CFlowmeterFlowCalibrationDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowDomestic();
}
