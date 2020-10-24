// FlowmeterTPCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterTPCalDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterTPCalDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterTPCalDlg, CDialogZoom)

CFlowmeterTPCalDlg::CFlowmeterTPCalDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CFlowmeterTPCalDlg::IDD, pParent)
, m_nStep(0)
, m_bIsOperationFinishedAtLeaseOnce(false)
, m_bIsCalibrationPass(true)
, m_fTPCalValue(101.0f)
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

CFlowmeterTPCalDlg::~CFlowmeterTPCalDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterTPCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_STATIC_TP, m_lbTP);

	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);

	DDX_Control(pDX, IDC_EDIT_TP_CALIBRATION_VALUE, m_neTPCalibrationValue);
}


BEGIN_MESSAGE_MAP(CFlowmeterTPCalDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterTPCalDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFlowmeterTPCalDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlowmeterTPCalDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterTPCalDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterTPCalDlg::OnBnClickedButtonFanOff)
END_MESSAGE_MAP()


// CFlowmeterTPCalDlg ��Ϣ�������

BOOL CFlowmeterTPCalDlg::OnInitDialog()
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


void CFlowmeterTPCalDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterTPCalDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�������¶�У׼");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// �¶�ʵʱֵ
	m_lbTP.SetLEDStyle();
	m_lbTP.SetText(L"--");

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);

	// ��ʼ���¶�У׼ֵΪ25
	// У׼ֵ�༭��
	m_neTPCalibrationValue.AllowNegative(FALSE);
	m_neTPCalibrationValue.SetMaxDecimalPlaces(1);
	m_neTPCalibrationValue.AllowNegative(1);
	m_neTPCalibrationValue.SetWindowText(L"25");

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
	EnableBasicOperation(false);
}

void CFlowmeterTPCalDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("�������¶�У׼"));
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
					m_lbInfo.SetText(_T("�������¶�У׼"));
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

void CFlowmeterTPCalDlg::OnTimer(UINT_PTR nIDEvent)
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

void CFlowmeterTPCalDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ֹͣˢ��ʵʱ����
	RefreshRTData(false);

	OnCancel();
}

void CFlowmeterTPCalDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ��������
	ResetProcess();

	m_lbInfo.SetText(_T("�¶�У׼ֹͣ"));

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�������¶�У׼ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_FLO_TPCAL_STOP, wchInfo, sizeof(wchInfo));
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CFlowmeterTPCalDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	Process();
}

void CFlowmeterTPCalDlg::Process(void)
{
	if (0 == m_nStep)
	{
				
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�������¶�У׼��ʼ", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_TPCAL_START, wchInfo, sizeof(wchInfo));
		}

		// ��������
		ResetProcess();

		m_lbInfo.SetText(_T("�������¶�У׼ֵ,Ȼ����һ��"));
		//m_lbInfo.FlashText(60);
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		EnableBasicOperation(false);

		m_nStep = 1;
	}
	else if (1 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////���У׼ֵ����start
		if (0x00 != GetCalibrationValue())
		{
			// У׼ֵ���벻����Ҫ��
			return;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////���У׼ֵ����stop

		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		BeginWaitCursor();

		// ֹͣˢ��ʵʱ����
		RefreshRTData(false);

		if (0x00 == CalibrationProcess())
		{
			m_bIsCalibrationPass = true;

			// У׼�ɹ�
			m_lbInfo.SetText(_T("�¶�У׼�ɹ�"));
		}
		else
		{
			m_bIsCalibrationPass = false;

			// У׼ʧ��
			m_lbInfo.SetText(_T("�¶�У׼ʧ��"));
		}

		// ����ˢ��ʵʱ����
		RefreshRTData(true);

		EndWaitCursor();

		// ��ǲ����Ѿ��������1��
		m_bIsOperationFinishedAtLeaseOnce = true;

		// �������
		ExportFloTPCalResult();
		
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�������¶�У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_FLO_TPCAL_FINISH, wchInfo, sizeof(wchInfo));
		}

		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));

		//m_lbInfo.SetText(_T("�¶�У׼���"));

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		EnableBasicOperation(true);

		// ���ò���
		m_nStep = 0;
	}
}


void CFlowmeterTPCalDlg::ResetProcess(void)
{
	// ������ʾ��
	m_lbStep1.SetTextColor(RGB(0,0,0));
	m_lbStep2.SetTextColor(RGB(0,0,0));

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);

	// ��������
	m_nStep = 0;
	// У׼����ó�ʼ��ֵ
	m_bIsCalibrationPass = true;

	// �¶�У׼ֵ
	m_fTPCalValue = 101.0f;
}

DWORD CFlowmeterTPCalDlg::GetCalibrationValue(void)
{
	// ��ȡУ׼ֵ�����
	CString strTPCalibrationValue;
	m_neTPCalibrationValue.GetWindowText(strTPCalibrationValue);
	strTPCalibrationValue.Trim();
	if (L"" == strTPCalibrationValue)
	{
		m_neTPCalibrationValue.SetFocus();
		MessageBox(_T("�������¶�У׼ֵ"), _T("�¶�У׼"), MB_OK|MB_ICONWARNING);
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

DWORD CFlowmeterTPCalDlg::CalibrationProcess(void)
{
	// �¶ȹ��������²�����ɣ�
	// 1)�رշ��
	// 2)�����¶�У׼ֵ
	// 3)�¶�У׼

	m_pFloThread->RunFan_Flowmeter(false);

	BYTE bResult(0);
	m_pFloThread->SetCalibrationTemp_Flowmeter(&bResult, m_fTPCalValue);
	if (0x06 == bResult)
	{
		// д���¶�У׼ֵ�ɹ�

		BYTE bCalResult(0);
		m_pFloThread->TempCalibration_Flowmeter(&bResult, &bCalResult);
		if (0x06==bResult && 0x00==bCalResult)
		{
			// �¶�У׼�ɹ�

			return 0x00;
		}
		else
		{
			// �¶�У׼ʧ��

			return 0x01;
		}
	}
	else
	{
		// д���¶�У׼ֵʧ��

		return 0x01;
	}
}

void CFlowmeterTPCalDlg::ExportFloTPCalResult()
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
	str.Format(L"\r\nPass=%s", m_bIsCalibrationPass ? L"1" : L"0");
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

void CFlowmeterTPCalDlg::RefreshRTData(bool bRefresh)
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

void CFlowmeterTPCalDlg::DisplayRTData(void)
{
	m_pFloThread->GetRealTimeData_Flowmeter_Aysn();

	float f = m_pFloThread->GetVmasRTDGasTempValue();
	CString str;
	str.Format(L"%.2f", f);
	m_lbTP.SetText(str);
}

void CFlowmeterTPCalDlg::ClearRTDisplay(void)
{
	m_lbTP.SetText(L"--");
}

void CFlowmeterTPCalDlg::OnBnClickedButtonFanOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(true);
}

void CFlowmeterTPCalDlg::OnBnClickedButtonFanOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_pFloThread->RunFan_Flowmeter(false);
}

void CFlowmeterTPCalDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterTPCalDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}