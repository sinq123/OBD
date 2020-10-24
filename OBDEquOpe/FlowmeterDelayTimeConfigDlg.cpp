// FlowmeterDelayTimeConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlowmeterDelayTimeConfigDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CFlowmeterDelayTimeConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CFlowmeterDelayTimeConfigDlg, CDialogZoom)

CFlowmeterDelayTimeConfigDlg::CFlowmeterDelayTimeConfigDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CFlowmeterDelayTimeConfigDlg::IDD, pParent)
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

CFlowmeterDelayTimeConfigDlg::~CFlowmeterDelayTimeConfigDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pFloThread)
	//{
	//	m_pFloThread->Close();
	//	m_pFloThread->QuitThread();
	//	WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	//}
}

void CFlowmeterDelayTimeConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_ED_HCCOCO2_VALUE, m_neHCCOCO2Value);
	DDX_Control(pDX, IDC_ED_O2_VALUE, m_neO2Value);
	DDX_Control(pDX, IDC_ED_NOX_VALUE, m_neNOxValue);
}


BEGIN_MESSAGE_MAP(CFlowmeterDelayTimeConfigDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFlowmeterDelayTimeConfigDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BTN_HCCOCO2_READ, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Read)
	ON_BN_CLICKED(IDC_BTN_O2_READ, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Read)
	ON_BN_CLICKED(IDC_BTN_NOX_READ, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxRead)
	ON_BN_CLICKED(IDC_BTN_HCCOCO2_WRITE, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Write)
	ON_BN_CLICKED(IDC_BTN_O2_WRITE, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Write)
	ON_BN_CLICKED(IDC_BTN_NOX_WRITE, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxWrite)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_CONFIG, &CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnDefaultConfig)
END_MESSAGE_MAP()


// CFlowmeterDelayTimeConfigDlg ��Ϣ�������

BOOL CFlowmeterDelayTimeConfigDlg::OnInitDialog()
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

void CFlowmeterDelayTimeConfigDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CFlowmeterDelayTimeConfigDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"������ͨ����ʱ����");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	m_neHCCOCO2Value.AllowNegative(FALSE);
	m_neHCCOCO2Value.SetMaxDecimalPlaces(0);
	m_neHCCOCO2Value.SetLimitText(2);
	m_neO2Value.AllowNegative(FALSE);
	m_neO2Value.SetMaxDecimalPlaces(0);
	m_neO2Value.SetLimitText(2);
	m_neNOxValue.AllowNegative(FALSE);
	m_neNOxValue.SetMaxDecimalPlaces(0);
	m_neNOxValue.SetLimitText(2);
}

void CFlowmeterDelayTimeConfigDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CFlowmeterDelayTimeConfigDlg::OpenSerialPortAndTestCommunication(void)
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

					// ����
					m_lbInfo.SetText(_T("������ͨ����ʱ����"));
					// λ��ʵʱͨѶ����
					EnableOperation(true);
				}
				break;
			case 0x05:
				{
					// ͨѶ����������δ�л��������Ʋ���ģʽ
					// ԭ��������æ

					// ����
					m_lbInfo.SetText(_T("������ͨ����ʱ����"));
					// λ��ʵʱͨѶ����
					EnableOperation(true);
				}
				break;
			case 0x15:
				{
					// ͨѶ���󣺷���0x15
					m_lbInfo.SetText(_T("ͨѶ����"));
					EnableOperation(false);
				}
				break;
			default:
				{
					// ͨѶ���󣺲���ʶ�����
					m_lbInfo.SetText(L"ͨѶ����");
					EnableOperation(false);
				}
				break;
			}
		}
		else
		{
			// ͨѶʧ��
			m_lbInfo.SetText(L"ͨѶ��ʱ");
			EnableOperation(false);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		EnableOperation(false);
	}

#ifdef _DEBUG
	EnableOperation(true);
#endif
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CFlowmeterDelayTimeConfigDlg::EnableOperation(bool bEnable)
{
	GetDlgItem(IDC_BTN_HCCOCO2_WRITE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_O2_WRITE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_NOX_WRITE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_HCCOCO2_READ)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_O2_READ)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_NOX_READ)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DEFAULT_CONFIG)->EnableWindow(bEnable);
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Read()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	float fHCCOCO2_DelayTime(0.0f);
	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->GetHCCOCO2DelayTime_Flowmeter(&bResult, &fHCCOCO2_DelayTime))
	{
		if (0x06 == bResult)
		{
			CString str;
			str.Format(L"%.2f", fHCCOCO2_DelayTime);
			GetDlgItem(IDC_ED_HCCOCO2_VALUE)->SetWindowTextW(str);
		}
		else
		{
			MessageBoxW(L"��ȡHC/CO/CO2ͨ����ʱֵʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"��ȡHC/CO/CO2ͨ����ʱֵʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Read()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	float fO2_DelayTime(0.0f);
	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->GetO2DelayTime_Flowmeter(&bResult, &fO2_DelayTime))
	{
		if (0x06 == bResult)
		{
			CString str;
			str.Format(L"%.2f", fO2_DelayTime);
			GetDlgItem(IDC_ED_O2_VALUE)->SetWindowTextW(str);
		}
		else
		{
			MessageBoxW(L"��ȡO2ͨ����ʱֵʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"��ȡO2ͨ����ʱֵʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	float fNOx_DelayTime(0.0f);
	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->GetNOxDelayTime_Flowmeter(&bResult, &fNOx_DelayTime))
	{
		if (0x06 == bResult)
		{
			CString str;
			str.Format(L"%.2f", fNOx_DelayTime);
			GetDlgItem(IDC_ED_NOX_VALUE)->SetWindowTextW(str);
		}
		else
		{
			MessageBoxW(L"��ȡNOxͨ����ʱֵʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"��ȡNOxͨ����ʱֵʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnHccoco2Write()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_neHCCOCO2Value.GetWindowTextW(str);
	float fDelayTime = (float)_wtof(str);
	if (fDelayTime<10e-6 || fDelayTime>15.0f)
	{
		MessageBoxW(L"��ʱֵӦ��0��15֮��ȡֵ", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		m_neHCCOCO2Value.SetFocus();
		return;
	}

	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->SetHCCOCO2DelayTime_Flowmeter(&bResult, fDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->HCCOCO2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 == bCalResult)
				{
					MessageBoxW(L"HC/CO/CO2ͨ����ʱֵд��ɹ�", L"ͨ����ʱ����", MB_OK);
				}
				else
				{
					MessageBoxW(L"HC/CO/CO2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
				}
			}
			else
			{
				MessageBoxW(L"HC/CO/CO2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
			}
		}
		else
		{
			MessageBoxW(L"HC/CO/CO2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"HC/CO/CO2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnO2Write()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_neO2Value.GetWindowTextW(str);
	float fDelayTime = (float)_wtof(str);
	if (fDelayTime<10e-6 || fDelayTime>15.0f)
	{
		MessageBoxW(L"��ʱֵӦ��0��15֮��ȡֵ", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		m_neO2Value.SetFocus();
		return;
	}

	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->SetO2DelayTime_Flowmeter(&bResult, fDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->O2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 == bCalResult)
				{
					MessageBoxW(L"O2ͨ����ʱֵд��ɹ�", L"ͨ����ʱ����", MB_OK);
				}
				else
				{
					MessageBoxW(L"O2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
				}
			}
			else
			{
				MessageBoxW(L"O2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
			}
		}
		else
		{
			MessageBoxW(L"O2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"O2ͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnNoxWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_neNOxValue.GetWindowTextW(str);
	float fDelayTime = (float)_wtof(str);
	if (fDelayTime<10e-6 || fDelayTime>15.0f)
	{
		MessageBoxW(L"��ʱֵӦ��0��15֮��ȡֵ", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		m_neNOxValue.SetFocus();
		return;
	}

	BYTE bResult(0);
	if (ANA_WR_SUCCEED == m_pFloThread->SetNOxDelayTime_Flowmeter(&bResult, fDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->NOxDelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 == bCalResult)
				{
					MessageBoxW(L"NOxͨ����ʱֵд��ɹ�", L"ͨ����ʱ����", MB_OK);
				}
				else
				{
					MessageBoxW(L"NOxͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
				}
			}
			else
			{
				MessageBoxW(L"NOxͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
			}
		}
		else
		{
			MessageBoxW(L"NOxͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
		}
	}
	else
	{
		MessageBoxW(L"NOxͨ����ʱֵд��ʧ��", L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}
}

void CFlowmeterDelayTimeConfigDlg::OnBnClickedBtnDefaultConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK != MessageBoxW(L"ȷ�����ڻָ�ͨ����ʱֵĬ�����ã�", L"ͨ����ʱ����", MB_OKCANCEL|MB_ICONWARNING))
	{
		return;
	}

	// ͨ����ʱĬ��ֵ��ʼ��
	float fHCCOCO2DelayTime(5.0f);
	float fO2DelayTime(6.0f);
	float fNOxDelayTime(7.0f);

	bool bIsOK_HCCOCO2(true);
	bool bIsOK_O2(true);
	bool bIsOK_NOx(true);

	BYTE bResult(0);
	// дHC/CO/CO2ͨ����ʱĬ��ֵ
	if (ANA_WR_SUCCEED == m_pFloThread->SetHCCOCO2DelayTime_Flowmeter(&bResult, fHCCOCO2DelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->HCCOCO2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 != bCalResult)
				{
					bIsOK_HCCOCO2 = false;
				}
			}
			else
			{
				bIsOK_HCCOCO2 = false;
			}
		}
		else
		{
			bIsOK_HCCOCO2 = false;
		}
	}
	else
	{
		bIsOK_HCCOCO2 = false;
	}

	// дO2ͨ����ʱĬ��ֵ
	if (ANA_WR_SUCCEED == m_pFloThread->SetO2DelayTime_Flowmeter(&bResult, fO2DelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->O2DelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 != bCalResult)
				{
					bIsOK_O2 = false;
				}
			}
			else
			{
				bIsOK_O2 = false;
			}
		}
		else
		{
			bIsOK_O2 = false;
		}
	}
	else
	{
		bIsOK_O2 = false;
	}

	// дNOxͨ����ʱĬ��ֵ
	if (ANA_WR_SUCCEED == m_pFloThread->SetNOxDelayTime_Flowmeter(&bResult, fNOxDelayTime))
	{
		if (0x06 == bResult)
		{
			BYTE bCalResult(-1);
			if (ANA_WR_SUCCEED == m_pFloThread->NOxDelayTime_Flowmeter(&bResult, &bCalResult))
			{
				if (0x00 != bCalResult)
				{
					bIsOK_NOx = false;
				}
			}
			else
			{
				bIsOK_NOx = false;
			}
		}
		else
		{
			bIsOK_NOx = false;
		}
	}
	else
	{
		bIsOK_NOx = false;
	}

	CString str(L"");
	if (!bIsOK_HCCOCO2)
	{
		str += L"�� HC/CO/CO2ͨ����ʱĬ��ֵд��ʧ��\n\n";
	}
	if (!bIsOK_O2)
	{
		str += L"�� O2ͨ����ʱĬ��ֵд��ʧ��\n\n";
	}
	if (!bIsOK_NOx)
	{
		str += L"�� NOxͨ����ʱĬ��ֵд��ʧ��\n\n";
	}

	if (bIsOK_HCCOCO2 && bIsOK_O2 && bIsOK_NOx)
	{
		MessageBoxW(L"ͨ����ʱĬ��ֵ���óɹ�", L"ͨ����ʱ����", MB_OK);
	}
	else
	{
		MessageBoxW(str, L"ͨ����ʱ����", MB_OK|MB_ICONWARNING);
	}	
}
