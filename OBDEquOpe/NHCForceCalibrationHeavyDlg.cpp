// NHCForceCalibrationHeavyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCForceCalibrationHeavyDlg.h"
#include "math.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCForceCalibrationHeavyDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCForceCalibrationHeavyDlg, CDialogZoom)

CNHCForceCalibrationHeavyDlg::CNHCForceCalibrationHeavyDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCForceCalibrationHeavyDlg::IDD, pParent)
{
    m_nTestStep = STEP_PREPARE;
	m_nZeroTime = 0;
	m_fCalForce = 0;
	m_nStayTime = 0;
	m_bIsOperationFinishedAtLeaseOnce = false;
	m_nPAUCount = 0;
	m_nCurPAU = NOPAU;

	ZeroMemory(m_fStayF, sizeof(float)*5);
	for (int i=0; i<4; i++)
	{	
		m_bIsPass[i] = true;
		m_bIsCalibrated[i] = false;
	}

	// �����⹦�������߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

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

	m_pfProcessCtrl = NULL;
}

CNHCForceCalibrationHeavyDlg::~CNHCForceCalibrationHeavyDlg()
{	
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCForceCalibrationHeavyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_N, m_lbTorsion);
	DDX_Control(pDX, IDC_STATIC_STEP, m_lbStep);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_RESET, m_lbReset);
}


BEGIN_MESSAGE_MAP(CNHCForceCalibrationHeavyDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_PAU1, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_RADIO_PAU2, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_RADIO_PAU3, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_RADIO_PAU4, &CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHCForceCalibrationHeavyDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CNHCForceCalibrationHeavyDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDCANCEL, &CNHCForceCalibrationHeavyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNHCForceCalibrationHeavyDlg ��Ϣ�������

BOOL CNHCForceCalibrationHeavyDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ���ؼ�
	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(TIMER_OPEN_COM, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCForceCalibrationHeavyDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCForceCalibrationHeavyDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦����У׼");

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbReset.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	m_lbTorsion.SetLEDStyle();

	GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
}

void CNHCForceCalibrationHeavyDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));

	// ��ȡҪ��ʾ��PAU���
	bool bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"TR", L"1")));
	m_stPAUShow.bPAU1 = bShow;
	bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"TL", L"1")));
	m_stPAUShow.bPAU2 = bShow;
	bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"BR", L"1")));
	m_stPAUShow.bPAU3 = bShow;
	bShow = 0!=(_wtoi(si.GetString(L"DynParams", L"BL", L"1")));
	m_stPAUShow.bPAU4 = bShow;

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, CDynThread::IM_NANHUA_NHC03))
	{
		std::bitset<16> bit16(0);
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bit16))
		{
			m_lbInfo.SetText(_T("��ѡ�� [��������] ������У׼"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
			
			ResetRadioPau();	
		}
		else
		{
			m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	}
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHCForceCalibrationHeavyDlg::OnTimer(UINT_PTR nIDEvent)
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
	case TIMER_INSTALL_TOOL:
		{
			KillTimer(TIMER_INSTALL_TOOL);
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(_T("�밲װ��У׼���ߺ����һ��"));
			m_nTestStep = STEP_INTALL_TOOL;
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		}
		break;

	case TIMER_GET_RTDATA:
		{
			m_pDynThread->GetRealTimeData_Asyn(NULL);//��ȡʵʱ����
			CString str;
			str.Format(_T("%.1f"), m_pDynThread->GetFValue());
			m_lbTorsion.SetText(str);
		}
		break;

	case TIMER_ZERO_BALANCE:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{
				m_nZeroTime ++;
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = STEP_PUT_WEIGHT;	// �ز�⹦��ֻ��У׼һ��
					KillTimer(TIMER_ZERO_BALANCE);
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case TIMER_WEIGHT_BALANCE:
		{
			// �ж��Ƿ�������
			if (m_nStayTime >= 5)// �����Ѿ�����
			{
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					m_nTestStep = STEP_REMOVE_WEIGHT;
					KillTimer(TIMER_WEIGHT_BALANCE);
					OnBnClickedButtonStart();
				}
			}
			m_fStayF[m_nStayTime%5] = m_pDynThread->GetFValue();
			m_nStayTime++;
		}
		break;

	case TIMER_REMOVE_ALL:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{	
				m_nZeroTime++;
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = STEP_RESET_ZERO_DONE;
					KillTimer(TIMER_REMOVE_ALL);
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;
	default:
		break;
	}
	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCForceCalibrationHeavyDlg::ClearResult()             //��������
{
	m_lbInfo.StopFlashText();
	m_lbInfo.SetText(_T("��У׼ǰ����װУ׼����"));
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep1.SetText(L"   ��װУ׼����");
	m_lbStep2.SetText(L"   ��������");
	m_lbStep3.SetText(L"   ���������У׼����");
	m_lbStep4.SetText(L"   ���");

	m_nTestStep = STEP_PREPARE;
	m_nZeroTime = 0;
	m_fCalForce = 0.0f;	
}

void CNHCForceCalibrationHeavyDlg::ResetRadioPau(void)
{	
	GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO_PAU1))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_PAU2))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_PAU3))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_PAU4))->SetCheck(BST_UNCHECKED);
	
	if(m_stPAUShow.bPAU1)
	{
		GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU1)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU1)->ShowWindow(FALSE);
	}
	if(m_stPAUShow.bPAU2)
	{
		GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU2)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU2)->ShowWindow(FALSE);
	}
	if(m_stPAUShow.bPAU3)
	{
		GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU3)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU3)->ShowWindow(FALSE);
	}
	if(m_stPAUShow.bPAU4)
	{
		GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PAU4)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PAU4)->ShowWindow(FALSE);
	}
}

void CNHCForceCalibrationHeavyDlg::OnBnClickedRadioPau()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU1))->GetCheck())
	{
		m_nCurPAU = PAU1;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU2))->GetCheck())
	{
		m_nCurPAU = PAU2;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU3))->GetCheck())
	{
		m_nCurPAU = PAU3;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_PAU4))->GetCheck())
	{
		m_nCurPAU = PAU4;
	}	
	else
	{
		m_nCurPAU = NOPAU;
		return;
	}
	if (m_bIsCalibrated[m_nCurPAU])
	{
		CString strMsg;
		strMsg.Format(L"��ʾ����[��������]�ѽ��й�У׼��У׼�����%s", m_bIsPass[m_nCurPAU]?L"ͨ��":L"��ͨ��");
		MessageBox(strMsg, _T("��У׼"));
	}

	m_pDynThread->WriteVar(231, (WORD)m_nCurPAU);	//���ò⹦����ѡ��PAU

	CString strTemp;
	strTemp.Format(L"׼���� [��������]-%d ����У׼", m_nCurPAU+1);
	m_lbInfo.SetText(strTemp);
}

void CNHCForceCalibrationHeavyDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nCurPAU == NOPAU)	//ûѡ��PAU
	{
		MessageBox(L"����ѡ�� [��������]��", _T("��У׼"));
		return;
	}

	if (m_nTestStep == STEP_PREPARE)
	{	
		m_lbInfo.SetText(_T("��У׼ǰ����װУ׼����"));

		if (MessageBox(_T("ȷ�Ͽ�ʼУ׼��?"), _T("��У׼"), MB_OKCANCEL) == IDOK)
		{		
			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦����У׼��ʼ", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_DYN_FORCE_CAL_START, wchInfo, sizeof(wchInfo));
			}

			ClearResult();		//��������

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));
			GetDlgItem(IDC_RADIO_PAU1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_PAU2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_PAU3)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_PAU4)->EnableWindow(FALSE);

			m_fCalForce = (float)m_pDynThread->ReadVar(54)/10.0f;	//ȡ��������У׼����(�ϻ�)
			m_pDynThread->SendCtrlWord_Asyn('0');					//�˳����п���ģʽ
			m_pDynThread->SendCtrlWord_Asyn('U');					//�ر��Զ���λ����
			m_pDynThread->SendCtrlWord_Asyn('L');					//������λУ׼(��¼��λ)

			SetTimer(TIMER_INSTALL_TOOL, 1500, NULL);
			SetTimer(TIMER_GET_RTDATA, 500, NULL);

			m_lbInfo.SetText(_T("���ڵ���..."));
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == STEP_INTALL_TOOL)        //У׼����
	{
		// �ж��Ƿ��Ѿ���װУ׼����
		if (fabs(m_pDynThread->GetFValue()) < 50.0f)
		{
			if (MessageBox(_T("�밲װУ׼���ߺ��ȷ��!"), _T("��У׼"), MB_OKCANCEL) != IDOK)
			{
				m_bIsPass[m_nCurPAU] = false;
				m_nTestStep = STEP_REMOVE_WEIGHT;
				m_lbStep1.SetTextColor(RGB(0, 0, 0));
				m_lbStep1.SetText(L"�� ��װУ׼����");
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"�� ��������");
			}
			OnBnClickedButtonStart();
			return;
		}
	
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"�� ��װУ׼����");

		m_lbInfo.SetText(_T("���ڵ���..."));
		m_pDynThread->SendCtrlWord_Asyn('T');	//���Զ���λ����
		SetTimer(TIMER_ZERO_BALANCE, 500, NULL);
		m_nZeroTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (m_nTestStep == STEP_PUT_WEIGHT)
	{
		m_pDynThread->SendCtrlWord_Asyn('U');	//�ر��Զ���λ����

		m_lbInfo.SetText(_T("�����3����������һ��"));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = STEP_CALIBRATION;
	}
	else if (m_nTestStep == STEP_CALIBRATION)
	{
		// �ж����������Ƿ�ﵽҪ������1000N��Χ
		if (m_fCalForce > 0.0f)
		{
			if (fabs(m_pDynThread->GetFValue()-m_fCalForce) > 1000.0f)
			{
				MessageBox(_T("���������쳣!"), _T("��У׼"));
				m_bIsPass[m_nCurPAU] = false;
				m_nTestStep = STEP_REMOVE_WEIGHT;
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"�� ��������");
				OnBnClickedButtonStart();
				return;
			}
		}

		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"�� ��������");

		m_lbInfo.SetText(_T("���ڵ���..."));
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		m_nStayTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('N');	//��������У׼����	
		SetTimer(TIMER_WEIGHT_BALANCE, 1000, NULL);
	}
	else if (m_nTestStep == STEP_REMOVE_WEIGHT)
	{
		m_lbInfo.SetText(_T("�����������У׼���ߺ����һ��"));	
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = STEP_RESET_ZERO;
	}
	else if (m_nTestStep == STEP_RESET_ZERO)
	{
		if (m_pDynThread->GetFValue() > 50.0f)
		{
			MessageBox(_T("�����������У׼����,�ٵ����һ��"), _T("��У׼"), MB_OK|MB_ICONWARNING);
			return;
		}

        m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"�� ���������У׼����");

		m_lbInfo.SetText(_T("���ڵ���..."));
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');	//���Զ���λ����
		SetTimer(TIMER_REMOVE_ALL, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == STEP_RESET_ZERO_DONE)
	{
		m_lbStep4.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(_T("���������У׼��������У׼"));
		m_pDynThread->SendCtrlWord_Asyn('V');    //���Զ�-ƫ��-������Ϊ����
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("���У׼"));
		m_nTestStep = STEP_DONE;

		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == STEP_DONE)
	{
		m_lbInfo.SetText(_T("У׼���"));	
		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"�� ���");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		
		m_nTestStep = STEP_PREPARE;
		m_bIsCalibrated[m_nCurPAU] = true;

		m_bIsOperationFinishedAtLeaseOnce = true;

		ResetRadioPau();

		//ExportEquCalChkInfo();	// �����б���
		//ExportDemarcationLog();	// �����궨��¼
		ExportDynForceCalibrationResult();	// �����Զ�����

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦����У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CAL_FINISH, wchInfo, sizeof(wchInfo));
		}
	}

}

void CNHCForceCalibrationHeavyDlg::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nCurPAU == NOPAU)	//ûѡ��PAU
	{
		MessageBox(L"����ѡ�� [��������]��", _T("��У׼"));
		return;
	}
	CString strMsg;
	strMsg.Format(L"�Ƿ�Ҫ���� [��������]-%d ������", m_nCurPAU+1);
	if (MessageBox(strMsg, _T("��У׼"), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	const int nAyyry [4][2]= {{29, 55}, {95, 56}, {96, 91}, {97, 92}};	// PAUx ������ַ{У׼���ӣ���λ}
	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar(nAyyry[m_nCurPAU][0], 1280) 
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(nAyyry[m_nCurPAU][1], 960))
	{
		m_lbInfo.SetText(_T("�������,�����½�����У׼"));
	}
	else
	{
		m_lbInfo.SetText(_T("����ʧ��"));
	}
	ClearResult();
	ResetRadioPau();
	m_bIsPass[m_nCurPAU] = true;
	m_bIsCalibrated[m_nCurPAU] = false;
}

void CNHCForceCalibrationHeavyDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// û��PAU
	if (m_nPAUCount <= 0)
	{
		OnCancel();
		return;
	}
	// �Ƿ���й�У׼
	bool bIsCalibrated = false;
	CString strMsg;	
	for (int i=0; i<m_nPAUCount; i++)
	{
		CString strTemp;
		if (!m_bIsCalibrated[i])
		{
			strTemp.Format(L"[��������]-%d δУ׼!\n", i+1);
			strMsg += strTemp;
		}
		bIsCalibrated |= m_bIsCalibrated[i];
	}
	// û�н���У׼��
	if (!bIsCalibrated)
	{
		OnCancel();
		return;
	}
	// û��δУ׼��PAU
	if (strMsg.IsEmpty())
	{
		OnCancel();
		return;
	}
	// ����δУ׼��PAU
	if (MessageBox(strMsg+L"�Ƿ�Ҫ�˳���", _T("��У׼"), MB_OKCANCEL) == IDOK)
	{
		OnCancel();
	}
}

/*
void CNHCForceCalibrationHeavyDlg::ExportEquCalChkInfo(void)
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
	CString strDynamometerModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

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
	str.Format(L"\r\nEquipment=%s", strDynamometerModel);
	strResult += str;

	// ��Ŀ
	str.Format(L"\r\nItem=%s", L"�⹦����У׼");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	str.Format(L"\r\nData=");
	strResult += str;

	// ���
	CString strError(L"");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	str.Format(L"\r\nJudgement=%s", m_bIsPass ? L"ͨ��" : L"��ͨ��");
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
*/

/*	
void CNHCForceCalibrationHeavyDlg::ExportDemarcationLog(void)
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

	// ��ȡ����߱�ź��豸�ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"DynamometerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

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
	str.Format(L"\r\nEquipmentName=%s", L"���̲⹦��");
	strDemarcationLog += str;

	// �궨��Ŀ
	str.Format(L"\r\nDemarcationItem=%d", DYN_FORCE_CAL);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass?L"1":L"0");
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
*/
	
void CNHCForceCalibrationHeavyDlg::ExportDynForceCalibrationResult(void)
{		
	// ÿУ׼һ��PAU�͵���һ����־
	CString str;
	CString strResult(L"[ResultOfDynForceCal]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// У׼�Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass[m_nCurPAU]?L"1":L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceCal.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCForceCalibrationHeavyDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

