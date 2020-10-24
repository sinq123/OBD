// NHA503XEnvParamsCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHA503XEnvParamsCalibrationDlg.h"


// CNHA503XEnvParamsCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CNHA503XEnvParamsCalibrationDlg, CDialogZoom)

CNHA503XEnvParamsCalibrationDlg::CNHA503XEnvParamsCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHA503XEnvParamsCalibrationDlg::IDD, pParent)
	, m_pAnaThread(NULL)
	, m_bRefreshDataSign(true)
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

	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pAnaThread;
}

CNHA503XEnvParamsCalibrationDlg::~CNHA503XEnvParamsCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	//if (NULL != m_pAnaThread)
	//{
	//	m_pAnaThread->Close();
	//	m_pAnaThread->QuitThread();
	//	WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	//}
}

void CNHA503XEnvParamsCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_EDIT_ET_MEA, m_edETMea);
	DDX_Control(pDX, IDC_EDIT_RH_MEA, m_edRHMea);
	DDX_Control(pDX, IDC_EDIT_AP_MEA, m_edAPMea);
	DDX_Control(pDX, IDC_EDIT_OT_MEA, m_edOTMea);

	DDX_Control(pDX, IDC_EDIT_ET_STD, m_neETStd);
	DDX_Control(pDX, IDC_EDIT_RH_STD, m_neRHStd);
	DDX_Control(pDX, IDC_EDIT_AP_STD, m_neAPStd);
	DDX_Control(pDX, IDC_EDIT_OT_STD, m_neOTStd);
}


BEGIN_MESSAGE_MAP(CNHA503XEnvParamsCalibrationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ET_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonEtCal)
	ON_BN_CLICKED(IDC_BUTTON_RH_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonRhCal)
	ON_BN_CLICKED(IDC_BUTTON_AP_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonApCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_CAL, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonOtCal)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonQuit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHA503XEnvParamsCalibrationDlg ��Ϣ�������

BOOL CNHA503XEnvParamsCalibrationDlg::OnInitDialog()
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

void CNHA503XEnvParamsCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHA503XEnvParamsCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�����ǻ�������������У׼");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// ����ֵ�༭��
	m_edETMea.SetReadOnly(TRUE);
	m_edRHMea.SetReadOnly(TRUE);
	m_edAPMea.SetReadOnly(TRUE);
	m_edOTMea.SetReadOnly(TRUE);

	// У׼ֵ�༭��
	m_neETStd.AllowNegative(FALSE);
	m_neETStd.SetMaxDecimalPlaces(1);
	m_neRHStd.AllowNegative(FALSE);
	m_neRHStd.SetMaxDecimalPlaces(1);
	m_neAPStd.AllowNegative(FALSE);
	m_neAPStd.SetMaxDecimalPlaces(1);
	m_neOTStd.AllowNegative(FALSE);
	m_neOTStd.SetMaxDecimalPlaces(1);

	GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonEtCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����׼ֵ������
	CString strStdET;
	GetDlgItem(IDC_EDIT_ET_STD)->GetWindowTextW(strStdET);
	if (L"" == strStdET)
	{
		MessageBox(L"�����뻷���¶ȱ�׼ֵ", L"�����¶�У׼", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdET = static_cast<float>(_wtof(strStdET));
	if (fStdET<-50.0f || fStdET>200.0f)
	{
		MessageBox(L"�����¶ȱ�׼ֵ���ں���Χ\r\n���������Ļ���������׼ֵ", L"�����¶�У׼", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->ETCalibration(&bResult, fStdET);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"�����¶ȱ�׼�ɹ�", L"�����¶�У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"�����¶ȱ�׼ʧ��", L"�����¶�У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonRhCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����׼ֵ������
	CString strStdRH;
	GetDlgItem(IDC_EDIT_RH_STD)->GetWindowTextW(strStdRH);
	if (L"" == strStdRH)
	{
		MessageBox(L"���������ʪ�ȱ�׼ֵ", L"���ʪ��У׼", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdRH = static_cast<float>(_wtof(strStdRH));
	if (fStdRH<0.0f || fStdRH>100.0f)
	{
		MessageBox(L"���ʪ�ȱ�׼ֵ���ں���Χ\r\n�������������ʪ�ȱ�׼ֵ", L"���ʪ��У׼", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->RHCalibration(&bResult, fStdRH);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"���ʪ�ȱ�׼�ɹ�", L"���ʪ��У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"���ʪ�ȱ�׼ʧ��", L"���ʪ��У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonApCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����׼ֵ������
	CString strStdAP;
	GetDlgItem(IDC_EDIT_AP_STD)->GetWindowTextW(strStdAP);
	if (L"" == strStdAP)
	{
		MessageBox(L"���������ѹ����׼ֵ", L"����ѹ��У׼", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdAP = static_cast<float>(_wtof(strStdAP));
	if (fStdAP<0.0f || fStdAP>200.0f)
	{
		MessageBox(L"����ѹ����׼ֵ���ں���Χ\r\n���������Ĵ���ѹ����׼ֵ", L"����ѹ��У׼", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->APCalibration(&bResult, fStdAP);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"����ѹ����׼�ɹ�", L"����ѹ��У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����ѹ����׼ʧ��", L"����ѹ��У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonOtCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����׼ֵ������
	CString strStdOT;
	GetDlgItem(IDC_EDIT_OT_STD)->GetWindowTextW(strStdOT);
	if (L"" == strStdOT)
	{
		MessageBox(L"���������±�׼ֵ", L"����У׼", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdOT = static_cast<float>(_wtof(strStdOT));
	if (fStdOT<-50.0f || fStdOT>200.0f)
	{
		MessageBox(L"���±�׼ֵ���ں���Χ\r\n�������������±�׼ֵ", L"����У׼", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pAnaThread->OTCalibration(&bResult, fStdOT);

	EnableRefreshData(true);
	EndWaitCursor();

	if (ANA_WR_SUCCEED==dwResult && 0==bResult)
	{
		MessageBox(L"���±�׼�ɹ�", L"����У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"���±�׼ʧ��", L"����У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CNHA503XEnvParamsCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
					m_lbInfo.SetText(_T("�����ǻ�������������У׼"));
					// λ��ʵʱͨѶ����
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

					// ��ʼˢ������
					EnableRefreshData(true);
				}
				break;
			case 0x02:  // Ԥ��
				{
					// ����Ԥ��
					m_lbInfo.SetText(_T("����Ԥ��.."));
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x03:	// ��©
				{
					// ���ڼ�©
					m_lbInfo.SetText(_T("���ڼ�©.."));
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0xFF:
				{
					// ͨѶ����,���ڿ�ͨѶ����
					m_lbInfo.SetText(L"��������ʵʱ���״̬");
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			default:
				{
					// ͨѶ����,���ڿ�ͨѶ����
					m_lbInfo.SetText(L"��������ʵʱ���״̬");
					GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			}
		}
		else
		{
			// ͨѶʧ��
			m_lbInfo.SetText(L"ͨѶ��ʱ");
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CNHA503XEnvParamsCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (nIDEvent)
	{
	case 1:
		{
			RefreshData();
		}
		break;
	case 2:
		{
			KillTimer(2);
			OpenSerialPortAndTestCommunication();
		}
		break;
	default:
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}


void CNHA503XEnvParamsCalibrationDlg::RefreshData(void)
{
	// ����ˢ�������ݺ͸�����
	if (m_bRefreshDataSign)
	{
		m_pAnaThread->GetMinorData_Asyn();
	}
	else
	{
		m_pAnaThread->GetMajorData_Asyn();
	}
	m_bRefreshDataSign = !m_bRefreshDataSign;

	float fET = m_pAnaThread->GetMinorDataETValue();
	float fRH = m_pAnaThread->GetMinorDataRHValue();
	float fAP = m_pAnaThread->GetMinorDataAPValue();
	float fOT = m_pAnaThread->GetMajorDataOilTempValue();

	CString str;
	
	str.Format(L"%.1f", fET);
	m_edETMea.SetWindowTextW(str);

	str.Format(L"%.1f", fRH);
	m_edRHMea.SetWindowTextW(str);

	str.Format(L"%.1f", fAP);
	m_edAPMea.SetWindowTextW(str);

	str.Format(L"%.1f", fOT);
	m_edOTMea.SetWindowTextW(str);
}

void CNHA503XEnvParamsCalibrationDlg::EnableRefreshData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(1, 1000, NULL);
	}
	else
	{
		KillTimer(1);
	}
}