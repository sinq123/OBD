// FlowmeterDilO2CalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterDilO2CalibrationDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterDilO2CalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterDilO2CalibrationDlg, CDialogZoom)

CFlowmeterDilO2CalibrationDlg::CFlowmeterDilO2CalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterDilO2CalibrationDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)
, m_bIsCalibrationPass(true)
, m_pfProcessCtrl(NULL)
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

	//m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pFloThread = CNHEuqController::GetInstance().m_pFloThread;
}

CFlowmeterDilO2CalibrationDlg::~CFlowmeterDilO2CalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterDilO2CalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_T, m_lbT);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);

	DDX_Control(pDX, IDC_STATIC_DIL_O2, m_lbDilO2);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);

	DDX_Control(pDX, IDC_EDIT_HIGH, m_neHighO2Value);
	DDX_Control(pDX, IDC_EDIT_MIDDLE, m_neMiddleO2Value);
	DDX_Control(pDX, IDC_EDIT_LOW, m_neLowO2Value);

	DDX_Control(pDX, IDC_RADIO1, m_rdo1);
	DDX_Control(pDX, IDC_RADIO2, m_rdo2);
	DDX_Control(pDX, IDC_RADIO_HIGH, m_rdoHigh);
	DDX_Control(pDX, IDC_RADIO_MIDDLE, m_rdoMiddle);
	DDX_Control(pDX, IDC_RADIO_LOW, m_rdoLow);
	DDX_Control(pDX, IDC_STATIC_BOX, m_rdoBox);
}


BEGIN_MESSAGE_MAP(CFlowmeterDilO2CalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_RADIO1, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO_HIGH, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioHigh)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioMiddle)
	ON_BN_CLICKED(IDC_RADIO_LOW, &CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioLow)
END_MESSAGE_MAP()


// CFlowmeterDilO2CalibrationDlg ��Ϣ�������

BOOL CFlowmeterDilO2CalibrationDlg::OnInitDialog()
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

void CFlowmeterDilO2CalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterDilO2CalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_nType = 1;
	m_nPoint = 1;
	m_rdo1.SetCheck(TRUE);
	m_rdoHigh.SetCheck(TRUE);
	ShowEntrance();

#ifdef _DEBUG
	m_neHighO2Value.SetWindowTextW(L"20.8");
	m_neMiddleO2Value.SetWindowTextW(L"4.52");
	m_neLowO2Value.SetWindowTextW(L"1.13");
#endif

	m_neHighO2Value.AllowNegative(FALSE);
	m_neHighO2Value.SetMaxDecimalPlaces(0);
	//m_neHighO2Value.SetLimitText(2);
	m_neMiddleO2Value.AllowNegative(FALSE);
	m_neMiddleO2Value.SetMaxDecimalPlaces(0);
	//m_neMiddleO2Value.SetLimitText(2);
	m_neLowO2Value.AllowNegative(FALSE);
	m_neLowO2Value.SetMaxDecimalPlaces(0);
	//m_neLowO2Value.SetLimitText(2);

	m_lbTitle.SetTitle(L"������ϡ����У׼");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));

	// ϡ����ʵʱֵ
	m_lbDilO2.SetLEDStyle();
	m_lbDilO2.SetText(L"--");

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	EnableBasicOperation(false);
}

void CFlowmeterDilO2CalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
	bool bIsFloOk(false);
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
					m_lbInfo.SetText(_T("������ϡ����У׼"));
					// λ��ʵʱͨѶ����
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
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
					m_lbInfo.SetText(_T("������ϡ����У׼"));
					// λ��ʵʱͨѶ����
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
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
			EnableBasicOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(false);
	}
}

void CFlowmeterDilO2CalibrationDlg::OnTimer(UINT_PTR nIDEvent)
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

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ��������
	ResetProcess();

	m_lbInfo.SetText(_T("ϡ����У׼ֹͣ"));
	
	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"������ϡ����У׼���", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_DILO2CAL_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"������ϡ����У׼��ʼ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_DILO2CAL_START, wchInfo, sizeof(wchInfo));
	}

	if(m_nType == 1)
	{
		Process();
	}
	else
	{
		Process1();
	}
}

void CFlowmeterDilO2CalibrationDlg::Process(void)
{
	if (0 == m_nStep)
	{
		// ��������
		ResetProcess();
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("ͨ��У׼���壬�ȶ�������һ��"));
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(L"��һ��");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_rdo1.ShowWindow(FALSE);
		m_rdo2.ShowWindow(FALSE);
		m_rdoBox.ShowWindow(FALSE);

		m_nStep = 1;
	}
	else if(1 == m_nStep)
	{
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		BeginWaitCursor();

		// ֹͣˢ��ʵʱ����
		RefreshRTData(false);

		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("����У׼.."));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_fValue = 20.8f;

		if (0x00 == CalibrationProcess())
		{
			m_bIsCalibrationPass = true;

			// У׼�ɹ�
			m_lbInfo.SetText(_T("ϡ����У׼�ɹ�"));
		}
		else
		{
			m_bIsCalibrationPass = false;

			// У׼ʧ��
			m_lbInfo.SetText(_T("ϡ����У׼ʧ��"));
		}
		//У׼���
		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));

		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		m_rdo1.ShowWindow(TRUE);
		m_rdo2.ShowWindow(TRUE);
		m_rdoBox.ShowWindow(TRUE);

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();

		// ��ǲ����Ѿ��������1��
		m_bIsOperationFinishedAtLeaseOnce = true;

		// �������
		ExportFloDilO2CalResult();

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"������ϡ����У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_DILO2CAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		// ���ò���
		m_nStep = 0;
	}
}

void CFlowmeterDilO2CalibrationDlg::Process1(void)
{
	if (0 == m_nStep)
	{
		// ��������
		ResetProcess();
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("ͨ��У׼���壬�ȶ�������һ��"));
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(L"��һ��");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_rdo1.ShowWindow(FALSE);
		m_rdo2.ShowWindow(FALSE);
		m_rdoBox.ShowWindow(FALSE);

		m_nStep = 1;
	}
	else if(1 == m_nStep)
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

		ShowEntrance();

		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		BeginWaitCursor();

		// ֹͣˢ��ʵʱ����
		RefreshRTData(false);

		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("����У׼.."));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		if(m_nPoint == 1)
		{
			if (0x00 == CalibrationProcess_High())
			{
				m_bIsCalibrationPass = true;

				// У׼�ɹ�
				m_lbInfo.SetText(_T("ϡ����У׼�ɹ�"));
			}
			else
			{
				m_bIsCalibrationPass = false;

				// У׼ʧ��
				m_lbInfo.SetText(_T("ϡ����У׼ʧ��"));
			}
		}

		else if(m_nPoint == 2)
		{
			if (0x00 == CalibrationProcess_Middle())
			{
				m_bIsCalibrationPass = true;

				// У׼�ɹ�
				m_lbInfo.SetText(_T("ϡ����У׼�ɹ�"));
			}
			else
			{
				m_bIsCalibrationPass = false;

				// У׼ʧ��
				m_lbInfo.SetText(_T("ϡ����У׼ʧ��"));
			}
		}

		else if(m_nPoint == 3)
		{
			if (0x00 == CalibrationProcess_Low())
			{
				m_bIsCalibrationPass = true;

				// У׼�ɹ�
				m_lbInfo.SetText(_T("ϡ����У׼�ɹ�"));
			}
			else
			{
				m_bIsCalibrationPass = false;

				// У׼ʧ��
				m_lbInfo.SetText(_T("ϡ����У׼ʧ��"));
			}
		}

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));

		m_lbStep1.SetTextColor(RGB(0, 0, 0));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		ShowDomestic();

		m_rdo1.ShowWindow(TRUE);
		m_rdo2.ShowWindow(TRUE);
		m_rdoBox.ShowWindow(TRUE);

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();

		// ��ǲ����Ѿ��������1��
		m_bIsOperationFinishedAtLeaseOnce = true;

		// �������
		ExportFloDilO2CalResult();

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"������ϡ����У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_DILO2CAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		// ���ò���
		m_nStep = 0;
	}
}

void CFlowmeterDilO2CalibrationDlg::ResetProcess(void)
{
	// ������ʾ��
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));
	m_lbStep3.SetTextColor(RGB(0,0,0));

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);

	// ��������
	m_nStep = 0;
	// У׼����ó�ʼ��ֵ
	m_bIsCalibrationPass = true;
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter(&bResult, 28.5f);
	if (0x06 == bResult)
	{
		// д��ϡ����У׼ֵ�ɹ�

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// ϡ����У׼���

			// һ�㲻���ܵ����λ�ã���Ϊ�᷵��0x05(����æ)

			 if (0x00 == bCalResult)
			 {
				 // У׼�ɹ�
				 return 0x00;
			 }
			 else
			 {
				 // У׼ʧ��
				 return 0x01;
			 }
		}
		else if (0x05 == bResult)
		{
			// �ȴ�У׼ִ�����

			WORD wProcessStatus(0);
			WORD wProcess(0);
			// ���������ѭ����ֻ��ѯ100��
			for (int i=0; i<100; i++)
			{
				bResult = 0;
				wProcessStatus = 0;
				wProcess = 0;
				m_pFloThread->GetProcessStatus_Flowmeter(&bResult, &wProcessStatus, &wProcess);
				if (0x00 == wProcessStatus)
				{
					// У׼����ִ�����
					break;
				}
				else
				{
					// С��һ��^_^
					ASSERT(0x36 == wProcess);
					Sleep(100);
				}
			}
			// ȡУ׼������ó�����ģʽ
			bResult = 0;
			bCalResult = 0;
			m_pFloThread->DilO2Calibration_Flowmeter(&bResult, &bCalResult, true);
			if (0x06==bResult && 0x00==bCalResult)
			{
				// У׼�ɹ�
				return 0x00;
			}
			else
			{
				// У׼ʧ��
				return 0x01;
			}
		}
		else
		{
			// У׼ʧ��
			return 0x01;
		}
	}
	else
	{
		// д��ϡ����У׼ֵʧ��

		return 0x01;
	}
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess_High(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	CString str;
	m_neHighO2Value.GetWindowTextW(str);
	float fHighValue = (float)_wtof(str);
	m_fValue = fHighValue;
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_High(&bResult, fHighValue);
	if (0x06 == bResult)
	{
		// д��ϡ����У׼ֵ�ɹ�

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter_High(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// ϡ����У׼���

			return 0x00;
		}
		else
		{
			// У׼ʧ��
			return 0x01;
		}
	}
	else
	{
		// д��ϡ����У׼ֵʧ��

		return 0x01;
	}
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess_Middle(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	CString str;
	m_neMiddleO2Value.GetWindowTextW(str);
	float fMiddleValue = (float)_wtof(str);
	m_fValue = fMiddleValue;
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Middle(&bResult, fMiddleValue);
	if (0x06 == bResult)
	{
		// д��ϡ����У׼ֵ�ɹ�

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter_Middle(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// ϡ����У׼���

			return 0x00;
		}
		else
		{
			// У׼ʧ��
			return 0x01;
		}
	}
	else
	{
		// д��ϡ����У׼ֵʧ��

		return 0x01;
	}
}

DWORD CFlowmeterDilO2CalibrationDlg::CalibrationProcess_Low(void)
{
	// ϡ�������������²�����ɣ�
	// 1)����ϡ����У׼ֵ
	// 2)ϡ����У׼
	// 3)�ȴ�����У׼���
	// 4)��ȡУ׼���

	CString str;
	m_neLowO2Value.GetWindowTextW(str);
	float fLowValue = (float)_wtof(str);
	m_fValue = fLowValue;
	BYTE bResult(0);
	m_pFloThread->SetCalibrationDilO2_Flowmeter_Low(&bResult, fLowValue);
	if (0x06 == bResult)
	{
		// д��ϡ����У׼ֵ�ɹ�

		BYTE bCalResult(0);
		m_pFloThread->DilO2Calibration_Flowmeter_Low(&bResult, &bCalResult);
		if (0x06 == bResult)
		{
			// ϡ����У׼���

			return 0x00;
		}
		else
		{
			// У׼ʧ��
			return 0x01;
		}
	}
	else
	{
		// д��ϡ����У׼ֵʧ��

		return 0x01;
	}
}

void CFlowmeterDilO2CalibrationDlg::ExportFloDilO2CalResult()
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
	str.Format(L"\r\nPass=%s", m_bIsCalibrationPass ? L"1" : L"0");
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

void CFlowmeterDilO2CalibrationDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterDilO2CalibrationDlg::DisplayRTData(void)
{
	m_pFloThread->GetRealTimeData_Flowmeter_Aysn();

	float f = m_pFloThread->GetVmasRTDUDilO2Value();
	CString str;
	str.Format(L"%.2f", f);
	m_lbDilO2.SetText(str);
}

void CFlowmeterDilO2CalibrationDlg::ClearRTDisplay(void)
{
	m_lbDilO2.SetText(L"--");
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedButtonFanOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterDilO2CalibrationDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterDilO2CalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowEntrance();
	m_nType = 1;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowDomestic();
	m_nType = 2;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioHigh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nPoint = 1;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioMiddle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nPoint = 2;
}


void CFlowmeterDilO2CalibrationDlg::OnBnClickedRadioLow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nPoint = 3;
}

void CFlowmeterDilO2CalibrationDlg::ShowEntrance()
{
	m_rdoHigh.ShowWindow(FALSE);
	m_rdoMiddle.ShowWindow(FALSE);
	m_rdoLow.ShowWindow(FALSE);

	m_neHighO2Value.ShowWindow(FALSE);
	m_neMiddleO2Value.ShowWindow(FALSE);
	m_neLowO2Value.ShowWindow(FALSE);

	m_lbT.ShowWindow(FALSE);
}

void CFlowmeterDilO2CalibrationDlg::ShowDomestic()
{
	m_rdoHigh.ShowWindow(TRUE);
	m_rdoMiddle.ShowWindow(TRUE);
	m_rdoLow.ShowWindow(TRUE);

	m_neHighO2Value.ShowWindow(TRUE);
	m_neMiddleO2Value.ShowWindow(TRUE);
	m_neLowO2Value.ShowWindow(TRUE);

	m_lbT.ShowWindow(TRUE);
}