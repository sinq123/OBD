// FlowmeterFreeFlowCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterFreeFlowCheckDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterFreeFlowCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterFreeFlowCheckDlg, CDialogZoom)

CFlowmeterFreeFlowCheckDlg::CFlowmeterFreeFlowCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterFreeFlowCheckDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)
, m_bIsPass(true)
, m_nTimesOfGettingData(0)
, m_nFluxMeasuredValue(0)
, m_nFreeFlowStandardValue(101)
, m_nRE(0)
, m_nRELimit(10)
, m_fMaximumZeroOffsetValue(36.7f)
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

	ZeroMemory(m_nFluxRTData, sizeof(m_nFluxRTData));

	// �������
	ImportParams();

	//m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pFloThread = CNHEuqController::GetInstance().m_pFloThread;
}

CFlowmeterFreeFlowCheckDlg::~CFlowmeterFreeFlowCheckDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterFreeFlowCheckDlg::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);

	DDX_Control(pDX, IDC_EDIT_FREE_FLOW_STANDARD_VALUE, m_neFreeFlowStandardValue);
	DDX_Control(pDX, IDC_STATIC_MEASURED_VALUE, m_lbMeauredValue);
	DDX_Control(pDX, IDC_STATIC_RE, m_lbRE);
	DDX_Control(pDX, IDC_STATIC_RE_LIMIT, m_lbRELimit);
	DDX_Control(pDX, IDC_STATIC_JUDGEMENT, m_lbJudgement);
	DDX_Control(pDX, IDC_LIST_DATA, m_lcData);
}


BEGIN_MESSAGE_MAP(CFlowmeterFreeFlowCheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOff)
END_MESSAGE_MAP()


// CFlowmeterFreeFlowCheckDlg ��Ϣ�������

BOOL CFlowmeterFreeFlowCheckDlg::OnInitDialog()
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


void CFlowmeterFreeFlowCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterFreeFlowCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�������������");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// ����ʵʱֵ
	m_lbFlux.SetLEDStyle();
	m_lbFlux.SetText(L"--");

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);

	// ��ʼ��������׼ֵ
	CString str;
	str.Format(L"%.2f", m_nFreeFlowStandardValue/100.0f);
	m_neFreeFlowStandardValue.SetWindowText(str);
	// ��ʼ��Ϊ�����ڽ���༭��׼ֵ(���Դ������ļ��޸�)
	m_neFreeFlowStandardValue.EnableWindow(FALSE);

	// ����ֵ��ʾ��
	m_lbMeauredValue.SetBkColor(RGB(230, 230, 230));
	// ��������ʾ��
	m_lbRE.SetBkColor(RGB(230, 230, 230));
	// ��������ֵ��ʾ��
	m_lbRELimit.SetBkColor(RGB(230, 230, 230));
	// �ж������ʾ��
	m_lbJudgement.SetBkColor(RGB(230, 230, 230));

	m_lcData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcData.InsertColumn(0,	L"ʱ��[s]",		LVCFMT_LEFT,	100*nSM_CXSCREEN/1280,	0);
	m_lcData.InsertColumn(1,	L"����[L/s]",	LVCFMT_LEFT,	180*nSM_CXSCREEN/1280,	1);
	// �ָ�
	m_lcData.InsertColumn(2,	L"",			LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	2);
	m_lcData.InsertColumn(3,	L"ʱ��[s]",		LVCFMT_LEFT,	100*nSM_CXSCREEN/1280,	3);
	m_lcData.InsertColumn(4,	L"����[L/s]",	LVCFMT_LEFT,	180*nSM_CXSCREEN/1280,	4);

	for(int i=0; i<10; i++)
	{
		str.Format(L"%d", i+1);
		m_lcData.InsertItem(i, str);

		str.Format(L"%d", i+11);
		m_lcData.SetItemText(i, 3, str);
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	EnableBasicOperation(false);
}

void CFlowmeterFreeFlowCheckDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("�������������"));
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
					m_lbInfo.SetText(_T("�������������"));
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

void CFlowmeterFreeFlowCheckDlg::OnTimer(UINT_PTR nIDEvent)
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
		// ��ȡ20������

		float f = m_pFloThread->GetVmasRTDFluxStd20Value();
		// �Ŵ�100��������������
		m_nFluxRTData[m_nTimesOfGettingData] = (int)(f * 100.0f + 0.5f);
		CString str;
		str.Format(L"%.2f", m_nFluxRTData[m_nTimesOfGettingData]/100.0f);
		if (m_nTimesOfGettingData < 10)
		{
			m_lcData.SetItemText(m_nTimesOfGettingData, 1, str);
		}
		else
		{
			m_lcData.SetItemText(m_nTimesOfGettingData-10, 4, str);
		}
		m_nTimesOfGettingData++;
		if (20 == m_nTimesOfGettingData)
		{
			// 20�����ݻ�ȡ���
			KillTimer(3);

			// ����ִ������
			Process();
		}
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	// �ر����������Ƿ��
	m_pFloThread->RunFan_Flowmeter(FALSE);

	// ��������
	ResetProcess();

	m_lbInfo.SetText(_T("�������ֹͣ"));
	
	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�������������ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_FLUXCHK_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	Process();
}

void CFlowmeterFreeFlowCheckDlg::Process(void)
{
	if (0 == m_nStep)
	{				
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"������������鿪ʼ", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCHK_START, wchInfo, sizeof(wchInfo));
		}

		// ��ȥ���м�����

		// ��������
		ResetProcess();

		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("���ȥ���м�����,Ȼ����һ��"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		// �򿪷��,�ȴ������ȶ�

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////����׼ֵ����start
		if (0x00 != GetStandardValue())
		{
			// ��׼ֵ���벻����Ҫ��
			return;
		}
		// ��¼��׼ֵ�������ļ�
		SaveStandardValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////����׼ֵ����stop


		BeginWaitCursor();

		// ֹͣˢ��ʵʱ����
		RefreshRTData(false);

		// �����������Ƿ��
		m_pFloThread->RunFan_Flowmeter(TRUE);

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();
		
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("��ȴ������ȶ���Ȼ����һ��"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 2;
	}
	else if (2 == m_nStep)
	{
		// ��¼20S�ȶ���������

		// ���ݻ�ȡ��������
		m_nTimesOfGettingData = 0;

		// ��ȡ20������
		SetTimer(3, 1000, NULL);

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("���ڻ�ȡ����..."));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		// ������
		
		// �ر����������Ƿ��
		m_pFloThread->RunFan_Flowmeter(FALSE);

		CalculateResult();
		DisplayResult();

		// ��ǲ����Ѿ��������1��
		m_bIsOperationFinishedAtLeaseOnce = true;

		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("������"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		// ������������
		ExportFloFluxChkResult();

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"����������������", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_FLUXCHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));

		// ��������
		m_nStep = 0;
	}
}

void CFlowmeterFreeFlowCheckDlg::ResetProcess(void)
{
	// ֹͣ��ȡ���ݶ�ʱ��
	KillTimer(3);

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
	
	// ������
	ClearResult();

	// ��������
	m_nStep = 0;
	// ������ó�ʼ��ֵ
	m_bIsPass = true;
	// ���ݻ�ȡ��������
	m_nTimesOfGettingData = 0;
	// �����ȡ����
	ZeroMemory(m_nFluxRTData, sizeof(m_nFluxRTData));
	// �������ֵ
	m_nFluxMeasuredValue = 0;
	// ���������
	m_nRE = 0;
}

DWORD CFlowmeterFreeFlowCheckDlg::GetStandardValue(void)
{
	// ��ȡ��׼ֵ�����
	CString strFreeFlowStandardValue;
	m_neFreeFlowStandardValue.GetWindowText(strFreeFlowStandardValue);
	strFreeFlowStandardValue.Trim();
	if (L"" == strFreeFlowStandardValue)
	{
		m_neFreeFlowStandardValue.SetFocus();
		MessageBox(_T("������������׼ֵ"), _T("�������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// ������׼ֵ,��λ:0.01L/s(�Ŵ�100��)
	float f = (float)_wtof(strFreeFlowStandardValue);
	ASSERT(f > -10e-6f);
	// ��ʽ��Ϊ2ΪС��������������
	m_nFreeFlowStandardValue = int(f * 100.0f + 0.5f);
	// ������Լ��(50L/s~500L/s)
	if (m_nFreeFlowStandardValue<5000 || m_nFreeFlowStandardValue>50000)
	{
		m_neFreeFlowStandardValue.SetFocus();
		MessageBox(_T("������׼ֵ���ں���Χ(50L/s~500L/s)"), _T("�������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CFlowmeterFreeFlowCheckDlg::SaveStandardValueToIniFile(void)
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
	str.Format(L"%.2f", m_nFreeFlowStandardValue/100.0f);
	si.SetString(L"FloDem", L"FreeFlowStandardValue", str);
}

void CFlowmeterFreeFlowCheckDlg::ImportParams(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	CString strDemarcationPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strDemarcationPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strDemarcationPath, dwAttributes);

	CSimpleIni si(strDemarcationPath);
	CString str;
	str = si.GetString(L"FloDem", L"FreeFlowStandardValue", L"");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// �Ŵ�100��,����������
	m_nFreeFlowStandardValue = (int)(f * 100.0f + 0.5f);
	// ������Լ��(50L/s~500L/s)
	if (m_nFreeFlowStandardValue < 5000)
	{
		m_nFreeFlowStandardValue = 5000;
	}
	if (m_nFreeFlowStandardValue > 50000)
	{
		m_nFreeFlowStandardValue = 50000;
	}

	CString strEquipmentPath = strPath + L"\\Config\\Equipment.ini";
	CSimpleIni sio(strEquipmentPath);
	str = sio.GetString(L"FloParams", L"MaximumZeroOffsetValue", L"36.7");
	f = (float)_wtof(str);
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


void CFlowmeterFreeFlowCheckDlg::ExportFloFluxChkResult()
{
	CString str;
	CString strResult(L"[ResultOfFloFluxChk]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	strData.Format(L"��׼ֵ:%.2fL/s,����ֵ%.2fL/s", m_nFreeFlowStandardValue/100.0f, m_nFluxMeasuredValue/100.0f);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ��׼ֵ
	str.Format(L"\r\nFreeFlowStandardValue=%.2f", m_nFreeFlowStandardValue/100.0f);
	strResult += str;
	// ����ֵ
	str.Format(L"\r\nFluxMeasuredValue=%.2f", m_nFluxMeasuredValue/100.0f);
	strResult += str;

	// ���
	CString strError;
	strError.Format(L"������:%d%%(��%d%%)", m_nRE, m_nRELimit);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass ? L"1" : L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfFloFluxChk.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CFlowmeterFreeFlowCheckDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterFreeFlowCheckDlg::DisplayRTData(void)
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

void CFlowmeterFreeFlowCheckDlg::ClearRTDisplay(void)
{
	m_lbFlux.SetText(L"--");
}

void CFlowmeterFreeFlowCheckDlg::CalculateResult(void)
{
	// ����ƽ��ֵ
	int nSum(0);
	for (int i=0; i<20; i++)
	{
		nSum += m_nFluxRTData[i];
	}
	m_nFluxMeasuredValue = (int)(nSum / 20.0f + 0.5f);

	// �������
	m_nRE = (m_nFluxMeasuredValue - m_nFreeFlowStandardValue) * 100 / m_nFreeFlowStandardValue;

	if (abs(m_nRE) > m_nRELimit)
	{
		// Failed
		m_bIsPass = false;
	}
	else
	{
		// Succeed
		m_bIsPass = true;
	}
}

void CFlowmeterFreeFlowCheckDlg::DisplayResult(void)
{
	CString str;

	// ����ֵ��ʾ��
	str.Format(L"%.2f", m_nFluxMeasuredValue/100.0f);
	m_lbMeauredValue.SetText(str);

	// ��������ʾ��
	str.Format(L"%d", m_nRE);
	m_lbRE.SetText(str);

	// ��������ֵ��ʾ��
	str.Format(L"��%d", m_nRELimit);
	m_lbRELimit.SetText(str);

	// �ж������ʾ��
	if (m_bIsPass)
	{
		m_lbJudgement.SetText(L"ͨ��");

		m_lbRE.SetTextColor(RGB(0, 128, 0));
		m_lbJudgement.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbJudgement.SetText(L"��ͨ��");

		m_lbRE.SetTextColor(RGB(255, 0, 0));
		m_lbJudgement.SetTextColor(RGB(255, 0, 0));
	}
}

void CFlowmeterFreeFlowCheckDlg::ClearResult(void)
{
	// ����б�
	for (int i=0; i<10; i++)
	{
		m_lcData.SetItemText(i, 1, L"");

		m_lcData.SetItemText(i, 4, L"");
	}

	// ����ֵ��ʾ��
	m_lbMeauredValue.SetText(L"--");
	m_lbMeauredValue.SetTextColor(RGB(0, 0, 0));

	// ��������ʾ��
	m_lbRE.SetText(L"--");
	m_lbRE.SetTextColor(RGB(0, 0, 0));

	// ��������ֵ��ʾ��
	m_lbRELimit.SetText(L"--");
	m_lbRELimit.SetTextColor(RGB(0, 0, 0));

	// �ж������ʾ��
	m_lbJudgement.SetText(L"--");
	m_lbJudgement.SetTextColor(RGB(0, 0, 0));
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterFreeFlowCheckDlg::OnBnClickedButtonFanOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterFreeFlowCheckDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterFreeFlowCheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}