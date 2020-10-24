// FlowmeterRealTimeDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterRealTimeDataDlg.h"
#include "afxdialogex.h"


// CFlowmeterRealTimeDataDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterRealTimeDataDlg, CDialogZoom)

CFlowmeterRealTimeDataDlg::CFlowmeterRealTimeDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CFlowmeterRealTimeDataDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 40 / 1024;
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

CFlowmeterRealTimeDataDlg::~CFlowmeterRealTimeDataDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterRealTimeDataDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CFlowmeterRealTimeDataDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FAN_ON, &CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOn)
	ON_BN_CLICKED(IDC_BUTTON_FAN_OFF, &CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOff)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterRealTimeDataDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()

BOOL CFlowmeterRealTimeDataDlg::OnInitDialog()
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


// CFlowmeterRealTimeDataDlg ��Ϣ�������

void CFlowmeterRealTimeDataDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"������ʵʱ����");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();


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

	EnableBasicOperation(false);
}

void CFlowmeterRealTimeDataDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterRealTimeDataDlg::OpenSerialPortAndTestCommunication(void)
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
					SetTimer(2,1000,NULL);

					// ����
					m_lbInfo.SetText(_T("������ʵʱ����"));

					EnableBasicOperation(true);
				}
				break;
			case 0x05:
				{
					// ͨѶ����������δ�л��������Ʋ���ģʽ
					// ԭ��������æ
					SetTimer(2,1000,NULL);
					// ����
					m_lbInfo.SetText(_T("������ʵʱ����"));

					EnableBasicOperation(true);
				}
				break;
			case 0x15:
				{
					// ͨѶ���󣺷���0x15
					m_lbInfo.SetText(_T("ͨѶ����"));

					EnableBasicOperation(false);
				}
				break;
			default:
				{
					// ͨѶ���󣺲���ʶ�����
					m_lbInfo.SetText(L"ͨѶ����");

					EnableBasicOperation(false);
				}
				break;
			}
		}
		else
		{
			// ͨѶʧ��
			m_lbInfo.SetText(L"ͨѶ��ʱ");

			EnableBasicOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));

		EnableBasicOperation(false);
	}
}

void CFlowmeterRealTimeDataDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_FAN_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_FAN_OFF)->EnableWindow(bEnable);
}

void CFlowmeterRealTimeDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		WORD status0;
		WORD status1;
		WORD status2;
		WORD status3;
		WORD status4;
		WORD status5;
		WORD status6;
		WORD status7;
		WORD status8;
		WORD status9;
		WORD status10;
		WORD status11;
		WORD status12;
		WORD status13;
		WORD status14;
		WORD status15;
		WORD status16;
		WORD status17;
		WORD status18;
		WORD status19;
		WORD status20;
		WORD status21;
		WORD status22;
		WORD status23;
		WORD status24;
		WORD status25;
		float fFluxStd0;
		float fFluxStd20;
		float fFluxAct;
		float fTemp;
		float fPres;
		float fUDilO2;

		m_pFloThread->GetRealTimeData_Flowmeter_ALL(&status0,&status1,&status2,&status3,&status4,&status5,&status6,&status7,&status8,&status9,&status10,&status11,&status12,&status13,&status14,&status15,
		&status16,&status17,&status18,&status19,&status20,&status21,&status22,&status23,&status24,&status25,&fFluxStd0,&fFluxStd20,&fFluxAct,&fPres,&fUDilO2,&fTemp);
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


void CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pFloThread->RunFan_Flowmeter(true);
}


void CFlowmeterRealTimeDataDlg::OnBnClickedButtonFanOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pFloThread->RunFan_Flowmeter(false);
}


void CFlowmeterRealTimeDataDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(2);
	OnCancel();
}
