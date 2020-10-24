// NHT1LCalibrationCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHT1LCalibrationCheckDlg.h"
#include "math.h"

// CNHT1LCalibrationCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CNHT1LCalibrationCheckDlg, CDialogZoom)

CNHT1LCalibrationCheckDlg::CNHT1LCalibrationCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHT1LCalibrationCheckDlg::IDD, pParent)
{
	// ������͸���߳�
	m_pOpaThread = (COpaThread*)AfxBeginThread(RUNTIME_CLASS(COpaThread));

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

	ZeroMemory(m_nN1, sizeof(int)*24);
	ZeroMemory(m_nN2, sizeof(int)*24);

	m_nTestStep = STEP_PREPARE;
	m_nTime = 0;
	m_nAELimit = 150;
	m_nLinearLimit = 110;
	m_nStdN1 = 0;
	m_nStdN2 = 0;

	m_bIsLinear1Pass = true;	
	m_bIsLinear2Pass = true;	
	m_bIsAE1Pass = true;		
	m_bIsAE2Pass = true;		
	m_bIsN1Pass = true;	
	m_bIsN2Pass = true;
	m_bIsCalChkPass = true;
	m_bIsOperationFinishedAtLeaseOnce = false;
}

CNHT1LCalibrationCheckDlg::~CNHT1LCalibrationCheckDlg()
{	
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// �˳�����ʱ,�������1�ζ�û����ɹ�,ҲҪ������Ӧ���
		ExportCalChkResult();
	}
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pOpaThread)
	{
		m_pOpaThread->Close();
		m_pOpaThread->QuitThread();
		WaitForSingleObject(m_pOpaThread->m_hThread, 5000);
	}
}

void CNHT1LCalibrationCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LB1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LB2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LB3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_N, m_lbN);
	DDX_Control(pDX, IDC_STATIC_K, m_lbK);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResult);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_ML1, m_mlK);
	DDX_Control(pDX, IDC_EDIT_STDN1, m_edStdN1);
	DDX_Control(pDX, IDC_EDIT_STDN2, m_edStdN2);
	DDX_Control(pDX, IDC_STATIC_AVG1, m_lbAvg1);
	DDX_Control(pDX, IDC_STATIC_AVG2, m_lbAvg2);
	DDX_Control(pDX, IDC_STATIC_LINEAR1, m_lbLinear1);
	DDX_Control(pDX, IDC_STATIC_LINEAR2, m_lbLinear2);
	DDX_Control(pDX, IDC_STATIC_ERR1, m_lbError1);
	DDX_Control(pDX, IDC_STATIC_ERR2, m_lbError2);
	DDX_Control(pDX, IDC_STATIC_JUDGEMENT, m_lbJudgement);
}


BEGIN_MESSAGE_MAP(CNHT1LCalibrationCheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHT1LCalibrationCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHT1LCalibrationCheckDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CNHT1LCalibrationCheckDlg ��Ϣ�������

BOOL CNHT1LCalibrationCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ���ؼ�
	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(TIMER_OPEN_COM, 500, NULL);

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHT1LCalibrationCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHT1LCalibrationCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	//��������С��λ��,������Ϊ����
	m_edStdN1.AllowNegative(FALSE);
	m_edStdN2.AllowNegative(FALSE);
	m_edStdN1.SetMaxDecimalPlaces();
	m_edStdN2.SetMaxDecimalPlaces();

	// �������±��ı��ؼ�
	m_mlK.SetScriptText(L"K[m^-^1]:");
	// Nԭʼʵʱֵ
	m_lbN.SetLEDStyle();
	m_lbN.SetText(L"--");
	// Kԭʼʵʱֵ
	m_lbK.SetLEDStyle();
	m_lbK.SetText(L"--");
	// ���ñ�ǩ��ɫ
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lbAvg1.SetBkColor(RGB(230, 230, 230));	
	m_lbAvg2.SetBkColor(RGB(230, 230, 230));
	m_lbLinear1.SetBkColor(RGB(230, 230, 230));
	m_lbLinear2.SetBkColor(RGB(230, 230, 230));
	m_lbError1.SetBkColor(RGB(230, 230, 230));
    m_lbError2.SetBkColor(RGB(230, 230, 230));
	m_lbJudgement.SetBkColor(RGB(230, 230, 230));

	m_lbTitle.SetTitle(_T("��͸��ȼ�У׼���"));
	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	//�б�
	m_lstResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstResult.InsertColumn(0, _T("ʱ��[s]"), 0, 110);
	m_lstResult.InsertColumn(1, _T("�˹�Ƭ1[%]"), 0, 150);
	m_lstResult.InsertColumn(2, _T("�˹�Ƭ2[%]"), 0, 150);
	m_lstResult.InsertColumn(3, _T(""), 0, 50);
	m_lstResult.InsertColumn(4, _T("ʱ��[s]"), 0, 110);
	m_lstResult.InsertColumn(5, _T("�˹�Ƭ1[%]"), 0, 150);
	m_lstResult.InsertColumn(6, _T("�˹�Ƭ2[%]"), 0, 150);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	for (int i=0; i<10; i++)
	{
		CString str;
		str.Format(_T("%.1f"), (i+1)*0.5);
		m_lstResult.InsertItem(i, str);

		str.Format(_T("%.1f"), (i+11)*0.5);
		m_lstResult.SetItemText(i, 4, str);
	}

	//��ȡ����
	CString strPathOfDataExchange;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// ����2��
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// ���ݽ���Ŀ¼·��
	strPathOfDataExchange = strPathOfDataExchange + L"\\Config";

	CSimpleIni iniDemarcation(strPathOfDataExchange + L"\\Demarcation.ini");
	m_edStdN1.SetWindowText(iniDemarcation.GetString(L"OpaDem", L"StdN1"));
	m_edStdN2.SetWindowText(iniDemarcation.GetString(L"OpaDem", L"StdN2"));
}

void CNHT1LCalibrationCheckDlg::OpenSerialPortAndTestCommunication(void)
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

	CSimpleIni si(strPath);
	// ��ȡͨѶ�˿�
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"OpaParams", L"Port", L"2"));
	if (OPA_OPEN_SUCCEED == m_pOpaThread->Open(bPort, COpaThread::IM_NANHUA_NHT1L))
	{
		BYTE bStatusValue = 0;
		if (OPA_WR_SUCCEED == m_pOpaThread->GetStatus(&bStatusValue)) //0��Ԥ�ȣ�1��ʵʱ��⣬2��У׼��3���Ƶ�Ԫ�������ԪͨѶ����
		{
			// ͨѶ�ɹ�
			switch (bStatusValue)
			{
			case 0x00:
				{
					// ����Ԥ��
					m_lbInfo.SetText(L"����Ԥ��...");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
					if (MessageBox(_T("�Ƿ��˳�Ԥ��?"), _T("��͸��ȼ�У׼"), MB_YESNO)==IDYES)
					{
						m_pOpaThread->StopWarmUp();		//ֹͣԤ��
						m_lbInfo.SetText(L"�����˳�Ԥ��...");
						SetTimer(TIMER_GET_STATUS, 1000, NULL);
					}
				}
				break;
			case 0x01:
				{
					// ʵʱ���Է�ʽ,���ò����л����Է�ʽ
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

					// ����
					m_lbInfo.SetText(L"��͸��ȼ�У׼���");
				}
				break;
			case 0x03:	
				{
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

					// ͨѶ����
					m_lbInfo.SetText(L"���Ƶ�Ԫ�������ԪͨѶ����");
				}
				break;
			default:
				{
					// ͨѶ����
					m_lbInfo.SetText(L"ͨѶ����");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
				}
				break;
			}
		}
		else
		{
			m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	}
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHT1LCalibrationCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case TIMER_OPEN_COM:
		{
			KillTimer(TIMER_OPEN_COM);
			OpenSerialPortAndTestCommunication();
		}
		break;
	case TIMER_GET_STATUS:
		{
			BYTE bStatus(0);
			if (OPA_WR_SUCCEED == m_pOpaThread->GetStatus(&bStatus))
			{
				if (1 == bStatus)
				{
					// ����
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					
					m_lbInfo.SetText(L"��͸��ȼ�У׼���");

					KillTimer(TIMER_GET_STATUS);
				}
			}
		}
		break;
	case TIMER_WAIT_FOR_CAL:
		{
			KillTimer(TIMER_WAIT_FOR_CAL);
			m_nTestStep = STEP_INPUT_N;
			m_lbInfo.SetText(_T("������2���˹�Ƭʾֵ�����һ��"));
			m_lbStep1.SetTextColor(RGB(0, 0, 0));
			m_lbStep1.SetText(L"�� ��͸��ȼ�У׼");
			m_lbStep2.SetTextColor(RGB(0, 0, 255));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		}
		break;
	case TIMER_GET_REAL_TIME_VAL:
		{
			m_pOpaThread->GetRealTimeData_Asyn(NULL); 

			CString strN, strK;
			float fN(0.0f), fK(0.0f);
			fN = m_pOpaThread->GetRealTimeNValue();
			fK = m_pOpaThread->GetRealTimeKValue();
			strN.Format(_T("%.2f"), fN);
			strK.Format(_T("%.2f"), fK);
			m_lbN.SetText(strN);
			m_lbK.SetText(strK);
		}
		break;
	case TIMER_MEASURE_N1:
		{
			if (m_nTime < 20)
			{
				CString str;
				float f = m_pOpaThread->GetRealTimeNValue();
				m_nN1[m_nTime] = (int)(f * 100 + 0.5f);

				str.Format(_T("%.2f"), f);

				if (m_nTime < 10)
				{
					m_lstResult.SetItemText(m_nTime, 1, str);
				}
				else
				{
					m_lstResult.SetItemText(m_nTime-10, 5, str);
				}

				m_nTime++;
				str.Format(_T("���ڻ�ȡ��������....%d"), 10-m_nTime/2);
				m_lbInfo.SetText(str);
			}
			else
			{
				KillTimer(TIMER_MEASURE_N1);
				m_nTime = 0;
				m_nTestStep = STEP_MEASURE_N2;
				OnBnClickedButtonStart();
			}
		}
		break;
	case TIMER_MEASURE_N2:
		{
			if (m_nTime < 20)
			{
				CString str;
				float f = m_pOpaThread->GetRealTimeNValue();
				m_nN2[m_nTime] = (int)(f * 100 + 0.5f);

				str.Format(_T("%.2f"), f);

				if (m_nTime < 10)
				{
					m_lstResult.SetItemText(m_nTime, 2, str);
				}
				else
				{
					m_lstResult.SetItemText(m_nTime-10, 6, str);
				}

				m_nTime++;
				str.Format(_T("���ڻ�ȡ��������....%d"), 10-m_nTime/2);
				m_lbInfo.SetText(str);
			}
			else
			{
				KillTimer(TIMER_MEASURE_N2);
				m_nTime = 0;
				m_nTestStep = STEP_DONE;
				OnBnClickedButtonStart();
			}
		}
		break;
	default:
		break;
	}
	CDialogZoom::OnTimer(nIDEvent);
}

void CNHT1LCalibrationCheckDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch (m_nTestStep)            
	{
	case STEP_PREPARE://����0:��ʼ���
		{
				ClearResult();

				m_lbStep1.SetTextColor(RGB(0, 0, 255));
				m_lbInfo.SetText(_T("����У׼..."));
				GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
				GetDlgItem(IDCANCEL)->EnableWindow(FALSE); // �����˳���ť
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

				m_pOpaThread->Calibrate();	//����У׼ָ��
				SetTimer(TIMER_WAIT_FOR_CAL, 5000, NULL);	//��5s
		}
		break;
	case STEP_INPUT_N:
		{
			//��ʼʵʱȡN��K��ֵ
			SetTimer(TIMER_GET_REAL_TIME_VAL, 400, NULL);

			CString strStdN1, strStdN2;
			m_edStdN1.GetWindowText(strStdN1);
			m_edStdN2.GetWindowText(strStdN2);
			if (strStdN1.IsEmpty() || strStdN2.IsEmpty())
			{
				MessageBox(_T("������2���˹�Ƭʾֵ�����һ��"), _T("��͸��ȼ�У׼"), MB_OK+MB_ICONINFORMATION);
				return;
			}
			
			float f = (float)_wtof(strStdN1);
			ASSERT(f > -10e-6f);
			
			m_nStdN1 = (int)(f * 100 + 0.5f);	// �Ŵ�100��,����������
			if (m_nStdN1 == 0)
			{	
				MessageBox(_T("�˹�Ƭ1ʾֵ����Ϊ0"), _T("��͸��ȼ�У׼"), MB_OK+MB_ICONINFORMATION);
				m_edStdN1.SetFocus();
				return;
			}

			f = (float)_wtof(strStdN2);
			ASSERT(f > -10e-6f);
			
			m_nStdN2 = (int)(f * 100 + 0.5f);	// �Ŵ�100��,����������
			if (m_nStdN2 == 0)
			{			
				MessageBox(_T("�˹�Ƭ2ʾֵ����Ϊ0"), _T("��͸��ȼ�У׼"), MB_OK+MB_ICONINFORMATION);
				m_edStdN2.SetFocus();
				return;
			}

			m_edStdN1.SetReadOnly(TRUE);
			m_edStdN2.SetReadOnly(TRUE);

			// �����׼ֵ
			SaveStdNVal();

			//��һ����ʾ��Ϣ
			m_lbInfo.SetText(L"������˹�Ƭ1");
			m_lbStep2.SetTextColor(RGB(0, 0, 0));
			m_lbStep2.SetText(L"�� ��д��׼�˹�Ƭ��ֵ");
			m_lbStep3.SetTextColor(RGB(0, 0, 255));
			
			m_nTestStep = STEP_MEASURE_N1;
		}
		break;
	case STEP_MEASURE_N1:
		{
			if (MessageBox(_T("������˹�Ƭ1���㡰ȷ������ʼ��ȡ�������ݣ�"), _T("��͸��ȼ�У׼"), MB_OKCANCEL) != IDOK)
			{
				return;
			}
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			m_nTime = 0;
			SetTimer(TIMER_MEASURE_N1, 500, NULL);                //��ȡ10��Nֵ
			m_lbInfo.SetText(_T("���ڻ�ȡ��������..."));
		}
		break;
	case STEP_MEASURE_N2:
		{
			m_lbStep3.SetTextColor(RGB(0, 0, 0));
			m_lbStep3.SetText(L"�� ��׼�˹�Ƭ1���");
			m_lbStep4.SetTextColor(RGB(0, 0, 255));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			m_lbInfo.SetText(L"������˹�Ƭ2");
			if (MessageBox(_T("������˹�Ƭ2���㡰ȷ������ʼ��ȡ�������ݣ�"), _T("��͸��ȼ�У׼"), MB_OKCANCEL) != IDOK)
			{
				return;
			}
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			m_nTime = 0;
			SetTimer(TIMER_MEASURE_N2, 500, NULL);                //��ȡ10��Nֵ
			m_lbInfo.SetText(_T("���ڻ�ȡ��������..."));
		}
		break;
	case STEP_DONE:
		{
			m_lbStep4.SetTextColor(RGB(0, 0, 0));
			m_lbStep4.SetText(L"�� ��׼�˹�Ƭ2���");
			m_lbStep5.SetTextColor(RGB(0, 0, 255));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			MessageBox(_T("У׼�����ɣ��������˹�Ƭ��"), _T("��͸��ȼ�У׼"));
			
			// ������
			CalcResult();

			m_lbStep5.SetTextColor(RGB(0, 0, 0));
			m_lbStep5.SetText(L"�� ������");
			m_lbInfo.SetText(_T("У׼������"));

			m_nTestStep = STEP_PREPARE;
			KillTimer(TIMER_GET_REAL_TIME_VAL);

			m_bIsOperationFinishedAtLeaseOnce = true;

			// �������ϸ��
			ExportCalChkDetail();
			// ���������
			ExportCalChkResult();

			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		}
		break;
	default:
		break;
	}
}

void CNHT1LCalibrationCheckDlg::SaveStdNVal(void)
{	
	//д������
	CString strPathOfDataExchange;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// ����2��
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// ���ݽ���Ŀ¼·��
	strPathOfDataExchange = strPathOfDataExchange + L"\\Config";

	CSimpleIni iniDemarcation(strPathOfDataExchange + L"\\Demarcation.ini");
	
	CString strStdN1;
	strStdN1.Format(L"%.2f", m_nStdN1/100.0);
	iniDemarcation.SetString(L"OpaDem", L"StdN1", strStdN1);	//�����˹�Ƭ1��׼���ֵ
	
	CString strStdN2;
	strStdN2.Format(L"%.2f", m_nStdN2/100.0);
	iniDemarcation.SetString(L"OpaDem", L"StdN2", strStdN2);	//�����˹�Ƭ2��׼���ֵ
}

void CNHT1LCalibrationCheckDlg::CalcResult(void)
{
	for (int i=0; i<20; i++)
	{
		m_nN1[20] += m_nN1[i];
		m_nN2[20] += m_nN2[i];
	}
	m_nN1[20] /= 20;		//N1ƽ��ֵ
	m_nN2[20] /= 20;		//N2ƽ��ֵ

	for (int i=0; i<20; i++)
	{
		m_nN1[21] += (int)pow((float)(m_nN1[i]-m_nN1[20]), 2);           //ƽ��
		m_nN2[21] += (int)pow((float)(m_nN2[i]-m_nN2[20]), 2);           //ƽ��
	}
	m_nN1[21] = (int)sqrt((float)m_nN1[21]) / 20;             //�����ٳ���20,�õ����Զ�ֵ
	m_nN2[21] = (int)sqrt((float)m_nN2[21]) / 20;             //�����ٳ���20,�õ����Զ�ֵ
	
	CString strTemp;
	strTemp.Format(L"%.2f", m_nN1[20]/100.0);	//��ʾN1ƽ��ֵ
	m_lbAvg1.SetText(strTemp);

	strTemp.Format(L"%.2f", m_nN2[20]/100.0);	//��ʾN2ƽ��ֵ
	m_lbAvg2.SetText(strTemp);
	
	if (abs(m_nN1[21]) > m_nLinearLimit)				//��ʾN1���Զ�
	{
		m_lbLinear1.SetTextColor(RGB(255, 0, 0));
		m_bIsLinear1Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN1[21]/100.0);	
	m_lbLinear1.SetText(strTemp);

	if (abs(m_nN2[21]) > m_nLinearLimit)				//��ʾN2���Զ�
	{
		m_lbLinear2.SetTextColor(RGB(255, 0, 0));
		m_bIsLinear2Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN2[21]/100.0);	
	m_lbLinear2.SetText(strTemp);

	m_nN1[23] = m_nN1[20] - m_nStdN1;			//��ʾN1���
	if (abs(m_nN1[23]) > m_nLinearLimit)
	{
		m_lbError1.SetTextColor(RGB(255, 0, 0));	
		m_bIsAE1Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN1[23]/100.0);
	m_lbError1.SetText(strTemp);

	m_nN2[23] = m_nN2[20] - m_nStdN2;			//��ʾN2���
	if (abs(m_nN2[23]) > m_nLinearLimit)
	{
		m_lbError2.SetTextColor(RGB(255, 0, 0));	
		m_bIsAE2Pass = false;
	}
	strTemp.Format(L"%.2f", m_nN2[23]/100.0);
	m_lbError2.SetText(strTemp);

	m_bIsN1Pass = m_bIsLinear1Pass && m_bIsAE1Pass;
	m_bIsN2Pass = m_bIsLinear2Pass && m_bIsAE2Pass;

	if (m_bIsN1Pass && m_bIsN2Pass)
	{		
		m_lbJudgement.SetTextColor(RGB(0, 0, 0));
		m_lbJudgement.SetText(_T("ͨ��"));
	}
	else
	{
		m_lbJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbJudgement.SetText(_T("��ͨ��"));
	}
}

void CNHT1LCalibrationCheckDlg::ClearResult()
{
	ZeroMemory(m_nN1, sizeof(int)*24);
	ZeroMemory(m_nN2, sizeof(int)*24);

	m_edStdN1.SetReadOnly(FALSE);
	m_edStdN2.SetReadOnly(FALSE);

	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep5.SetTextColor(RGB(0, 0, 0));

	m_lbStep1.SetText(L"   ��͸��ȼ�У׼");
	m_lbStep2.SetText(L"   ��д��׼�˹�Ƭ��ֵ");
	m_lbStep3.SetText(L"   ��׼�˹�Ƭ1���");	
	m_lbStep4.SetText(L"   ��׼�˹�Ƭ2���");	
	m_lbStep5.SetText(L"   ������");

	m_lbN.SetText(_T("--")); 
	m_lbK.SetText(_T("--")); 

	m_lbAvg1.SetText(_T("--"));                 
	m_lbAvg2.SetText(_T("--"));                

	m_lbLinear1.SetText(_T("--"));       
	m_lbLinear1.SetTextColor(RGB(0, 0, 0));

	m_lbLinear2.SetText(_T("--"));        
	m_lbLinear2.SetTextColor(RGB(0, 0, 0));

	m_lbError1.SetText(_T("--"));   
	m_lbError1.SetTextColor(RGB(0, 0, 0));

	m_lbError2.SetText(_T("--"));      
	m_lbError2.SetTextColor(RGB(0, 0, 0));
	
	m_lbJudgement.SetText(_T("--"));  
	m_lbJudgement.SetTextColor(RGB(0, 0, 0));

	for (int i =0; i<10; i++)
	{
		m_lstResult.SetItemText(i, 1, _T(""));
		m_lstResult.SetItemText(i, 2, _T(""));
		m_lstResult.SetItemText(i, 5, _T(""));
		m_lstResult.SetItemText(i, 6, _T(""));
	}

	m_bIsLinear1Pass = true;	
	m_bIsLinear2Pass = true;	
	m_bIsAE1Pass = true;		
	m_bIsAE2Pass = true;		
	m_bIsN1Pass = true;	
	m_bIsN2Pass = true;
	m_bIsCalChkPass = true;

	m_nTestStep = STEP_PREPARE;
	m_nTime = 0;
}

void CNHT1LCalibrationCheckDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWaitCursor c;
	KillTimer(TIMER_GET_REAL_TIME_VAL);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	m_lbInfo.SetText(_T("У׼���ֹͣ"));
	Sleep(400);
	ClearResult();
}

void CNHT1LCalibrationCheckDlg::ExportCalChkDetail(void)
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

	// ��ȡ����߱�ź������ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strOpacimeterNumber = siLineInfo.GetString(L"LineInfo", L"OpacimeterNumber", L"");
	CString strOpacimeterModel = siLineInfo.GetString(L"LineInfo", L"OpacimeterModel", L"");

	// ��ȡ����Ա
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString str;
	CString strDetail(L"[DetailOfOpaCalChk]");

	// �궨���
	str.Format(L"\r\nDemarcationNumber=%s-%s-%s", strStationNumber, strLineNumber, odt.Format(L"%Y%m%d%H%M%S"));
	strDetail += str;

	// ����߱��
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strDetail += str;

	// ����Ա
	str.Format(L"\r\nOperator=%s", strOperator);
	strDetail += str;

	// �豸���
	str.Format(L"\r\nEquipmentNumber=%s", strOpacimeterNumber);
	strDetail += str;

	// �豸�ͺ�
	str.Format(L"\r\nEquipmentModel=%s", strOpacimeterModel);
	strDetail += str;

	// �豸����
	str.Format(L"\r\nEquipmentName=%s", L"��͸��ȼ�");
	strDetail += str;

	// �궨��Ŀ
	str.Format(L"\r\nDemarcationItem=%d", 7);
	strDetail += str;

	// �궨����
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDetail += str;

	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ1ϸ��begin
	// �˹�Ƭ1����ֵƽ��ֵ,��λ:0.01%
	str.Format(L"\r\nMeasuredValue1=%.2f", m_nN1[20]/100.0);
	strDetail += str;

	// ��׼�˹�Ƭ1��ֵ,��λ:0.01%
	str.Format(L"\r\nStandardValue1=%.2f", m_nStdN1/100.0);
	strDetail += str;

	// �˹�Ƭ1�������,��λ:0.01%
	str.Format(L"\r\nAbsoluteError1=%.2f", m_nN1[23]/100.0);
	strDetail += str;
	
	// �˹�Ƭ1������,��λ:0.01%
	str.Format(L"\r\nRelativeError1=%.2f", m_nN1[23]/m_nStdN1*100.0);
	strDetail += str;
	
	// �˹�Ƭ1���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nAbsoluteErrorLimit1=%.2f", m_nAELimit/100.0);
	strDetail += str;

	// �˹�Ƭ1��������ֵ
	str.Format(L"\r\nRelativeErrorLimit1=");
	strDetail += str;

	// �˹�Ƭ1��������Ƿ�ϸ�
	str.Format(L"\r\nAbsoluteErrorPass1=%s", m_bIsAE1Pass ? L"1" : L"0");
	strDetail += str;

	// �˹�Ƭ1�������Ƿ�ϸ�
	str.Format(L"\r\nRelativeErrorPass1=");
	strDetail += str;

	// �˹�Ƭ1����ֵ���Զ�
	str.Format(L"\r\nLinearity1=%.2f", m_nN1[21]/100.0);
	strDetail += str;

	// �˹�Ƭ1����ֵ���Զ���ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearityLimit1=%.2f", m_nLinearLimit/100.0);
	strDetail += str;

	// �˹�Ƭ1���Զ��Ƿ�ϸ�
	str.Format(L"\r\nLinearityPass1=%s", m_bIsLinear1Pass ? L"1" : L"0");
	strDetail += str;

	// �˹�Ƭ1�Ƿ�ϸ�
	str.Format(L"\r\nPass1=%s", m_bIsN1Pass ? L"1" : L"0");
	strDetail += str;
	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ1ϸ��end


	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ2ϸ��begin
	// �˹�Ƭ2����ֵƽ��ֵ,��λ:0.01%
	str.Format(L"\r\nMeasuredValue2=%.2f", m_nN2[20]/100.0);
	strDetail += str;

	// ��׼�˹�Ƭ2��ֵ,��λ:0.01%
	str.Format(L"\r\nStandardValue2=%.2f", m_nStdN2/100.0);
	strDetail += str;

	// �˹�Ƭ2�������,��λ:0.01%
	str.Format(L"\r\nAbsoluteError2=%.2f", m_nN2[23]/100.0);
	strDetail += str;
	
	// �˹�Ƭ2������,��λ:0.01%,��������
	str.Format(L"\r\nRelativeError2=%.2f", m_nN2[23]/m_nStdN2*100.0);
	strDetail += str;
	
	// �˹�Ƭ2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nAbsoluteErrorLimit2=%.2f", m_nAELimit/100.0);
	strDetail += str;

	// �˹�Ƭ2��������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nRelativeErrorLimit2=");
	strDetail += str;

	// �˹�Ƭ2��������Ƿ�ϸ�
	str.Format(L"\r\nAbsoluteErrorPass2=%s", m_bIsAE2Pass ? L"1" : L"0");
	strDetail += str;

	// �˹�Ƭ2�������Ƿ�ϸ�
	str.Format(L"\r\nRelativeErrorPass2=");
	strDetail += str;

	// �˹�Ƭ2����ֵ���Զ�,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearity2=%.2f", m_nN2[21]/100.0);
	strDetail += str;

	// �˹�Ƭ2����ֵ���Զ���ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearityLimit2=%.2f", m_nLinearLimit/100.0);
	strDetail += str;

	// �˹�Ƭ2���Զ��Ƿ�ϸ�
	str.Format(L"\r\nLinearityPass2=%s", m_bIsLinear2Pass ? L"1" : L"0");
	strDetail += str;

	// �˹�Ƭ2�Ƿ�ϸ�
	str.Format(L"\r\nPass2=%s", m_bIsN2Pass ? L"1" : L"0");
	strDetail += str;
	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ2ϸ��end

	CString strIsPass;
	m_lbJudgement.GetWindowText(strIsPass);
	// У׼����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", (m_bIsN1Pass&&m_bIsN2Pass) ? L"1" : L"0");
	strDetail += str;

	// д�뵽���ini�ļ�
	CString strPathOfAnaGasCheckDetail = strPathOfDataExchange + L"\\DetailOfOpaCalChk.ini";
	if (0 == _taccess(strPathOfAnaGasCheckDetail, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfAnaGasCheckDetail);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfAnaGasCheckDetail, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfAnaGasCheckDetail, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strDetail);
	sfe.Close();
}

void CNHT1LCalibrationCheckDlg::ExportCalChkResult(void)
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

	// ��ȡ����߱�źͲ�͸���ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"OpacimeterModel", L"");

	// ��ȡ����Ա
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	CString str;
	CString strResult(L"[EquCalChkInfo]");

	// �����Ƿ���ɹ�����һ��
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce?L"1":L"0");
	strResult += str;

	// ����߱��
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strResult += str;

	// ����Ա
	str.Format(L"\r\nOperator=%s", strOperator);
	strResult += str;

	// �豸
	str.Format(L"\r\nEquipment=%s", strEquipmentModel);
	strResult += str;

	// ��Ŀ
	str.Format(L"\r\nItem=%s", L"��͸��ȼ�У׼���");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	strData.Format(L"�˹�Ƭ1:����ֵ:%.2f%%,��׼ֵ:%.2f%%;�˹�Ƭ2:����ֵ:%.2f%%,��׼ֵ:%.2f%%",
		m_nN1[20]/100.0, m_nStdN1/100.0, m_nN2[20]/100.0, m_nStdN2/100.0);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strError;
	strError.Format(L"�˹�Ƭ1:�������:%.2f%%(��%.2f%%),���Զ�:%.2f%%(��%.2f%%);�˹�Ƭ2:�������:%.2f%%(��%.2f%%),���Զ�:%.2f%%(��%.2f%%)",
		 m_nN1[23]/100.0,
		(float)m_nAELimit/100,
		m_nN1[21]/100.0,
		(float)m_nLinearLimit/100,
		m_nN2[23]/100.0,
		(float)m_nAELimit/100,
		m_nN2[21]/100.0,
		(float)m_nLinearLimit/100);
	if (!m_bIsOperationFinishedAtLeaseOnce)
	{
		strError = L"�˹�Ƭ1:�������:0%(��1.5%),���Զ�:0%(��1.1%);�˹�Ƭ2:�������:0%(��1.5%),���Զ�:0%(��1.1%)";
	}
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	str.Format(L"\r\nJudgement=%s", (m_bIsN1Pass&&m_bIsN2Pass) ? L"ͨ��" : L"��ͨ��");
	strResult += str;

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