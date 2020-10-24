// MHDForceCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MHDForceCalibrationDlg.h"
#include <cmath>

// CMHDForceCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CMHDForceCalibrationDlg, CDialogZoom)

CMHDForceCalibrationDlg::CMHDForceCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CMHDForceCalibrationDlg::IDD, pParent)
	, m_nTestStep(0)
	, m_nZeroTime(0)
	, m_fCalForce(0)
	, m_nStayTime(0)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_bIsPass(true)
	, m_pfProcessCtrl(NULL)
{
	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		L"����");

	// �����⹦���߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	ZeroMemory(m_fStayF, sizeof(m_fStayF));
}

CMHDForceCalibrationDlg::~CMHDForceCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	// ֹͣ�⹦���߳�
	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CMHDForceCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_N, m_lbTorsion);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_STATIC_STEP6, m_lbStep6);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_STEP, m_lbStep);
	DDX_Control(pDX, IDC_STATIC_RESET, m_lbReset);
}


BEGIN_MESSAGE_MAP(CMHDForceCalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMHDForceCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMHDForceCalibrationDlg::OnBnClickedButtonReset)
	//ON_MESSAGE(WM_DYN_READ_VAR,OnReadVar)
	ON_BN_CLICKED(IDCANCEL, &CMHDForceCalibrationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMHDForceCalibrationDlg ��Ϣ�������

BOOL CMHDForceCalibrationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMHDForceCalibrationDlg::OpenSerialPortAndTestCommunication(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Equipment.ini";

	CSimpleIni si(strPath);
	// ��ȡͨѶ�˿�
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, CDynThread::IM_MAHA_ASMP))
	{
		// �û�ȡУ׼��m_fCalForce����ͨѶ
		// ˳���У׼����¼����
		WORD w(0);
		if (DYN_WR_SUCCEED == m_pDynThread->ReadVar(88, &w))
		{
			m_fCalForce = w / 10.0f;

			m_lbInfo.SetText(L"��У׼ǰ����װУ׼����");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			m_lbInfo.SetText(L"ͨѶ��ʱ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(L"��ͨѶ�˿�ʧ��");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	}
}

void CMHDForceCalibrationDlg::EnableRefreshRealTimeData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(3, 500, NULL);
	}
	else
	{
		KillTimer(3);
	}
}

void CMHDForceCalibrationDlg::RefreshRealTimeData(void)
{
	m_pDynThread->GetRealTimeData_Asyn(NULL);
	CString str;
	str.Format(L"%.1f", m_pDynThread->GetFValue());
	m_lbTorsion.SetText(str);
}

void CMHDForceCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			OpenSerialPortAndTestCommunication();
		}
		break;

	case 2:
		{
			KillTimer(2);
			m_nTestStep = 1;
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(L"�밲װ��У׼���ߺ����һ��");
			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		}
		break;

	case 3:
		{
			RefreshRealTimeData();
		}
		break;

	case 4:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{
				m_nZeroTime++;
				// �ȶ�1��
				if (m_nZeroTime >= 2)
				{
					KillTimer(4);
					m_nTestStep = 2;
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case 5:
		{
			// �ж��Ƿ�������
			if (m_nStayTime >= 5)
			{
				// �����Ѿ�����
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					KillTimer(5);
					m_nTestStep = 4;
					OnBnClickedButtonStart();
				}
			}
			m_fStayF[m_nStayTime%5] = m_pDynThread->GetFValue();
			m_nStayTime++;
		}
		break;

	case 6:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{	
				m_nZeroTime++;
				// �ȶ�1��
				if (m_nZeroTime >= 2)
				{
					KillTimer(6);
					m_nTestStep = 6;
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case 7:
		{
			// �ж��Ƿ�������
			if (m_nStayTime >= 5)
			{
				// �����Ѿ�����
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					KillTimer(7);
					m_nTestStep = 8;
					OnBnClickedButtonStart();
				}
			}
			m_fStayF[m_nStayTime%5] = m_pDynThread->GetFValue();
			m_nStayTime++;
		}
		break;

	case 8:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1.0f)
			{	
				m_nZeroTime++;
				// �ȶ�1��
				if (m_nZeroTime >= 2)
				{
					KillTimer(8);
					m_nTestStep = 10;
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
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CMHDForceCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CMHDForceCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦����У׼");

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbReset.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetText(L"����ͨѶ...");
	m_lbInfo.SetTitleStyle();

	m_lbTorsion.SetLEDStyle();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
}

void CMHDForceCalibrationDlg::ClearResult()
{
	m_lbInfo.StopFlashText();
	m_lbInfo.SetText(L"��У׼ǰ����װУ׼����");
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep5.SetTextColor(RGB(0, 0, 0));
	m_lbStep6.SetTextColor(RGB(0, 0, 0));

	m_lbStep1.SetText(L"   ��װУ׼����");
	m_lbStep2.SetText(L"   ��������");
	m_lbStep3.SetText(L"   ��������");
	m_lbStep4.SetText(L"   ��������");
	m_lbStep5.SetText(L"   ���������У׼����");
	m_lbStep6.SetText(L"   ���");

	m_nTestStep = 0;
	m_nZeroTime = 0;
	m_fCalForce = 0;	
	m_bIsPass = true;
}

void CMHDForceCalibrationDlg::OnBnClickedButtonStart()
{
	if (m_nTestStep == 0)
	{	
		m_lbInfo.SetText(L"��У׼ǰ����װУ׼����");

		if (MessageBox(L"ȷ����ʼУ׼��?", L"��У׼", MB_OKCANCEL) == IDOK)
		{
			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦����У׼��ʼ", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_DYN_FORCE_CAL_START, wchInfo, sizeof(wchInfo));
			}

			// ��������
			ClearResult();

			BeginWaitCursor();
			EnableRefreshRealTimeData(false);

			// �˳����п���ģʽ
			m_pDynThread->SendCtrlWord('0');
			// �ر��Զ���λ����
			m_pDynThread->SendCtrlWord('U');
			// ������λУ׼(��¼��λ)
			m_pDynThread->SendCtrlWord('L');

			EnableRefreshRealTimeData(true);
			EndWaitCursor();

			SetTimer(2, 1500, NULL);

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��һ��");

			m_lbInfo.SetText(L"���ڵ���...");
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == 1)
	{
		// У׼����
		// �ж��Ƿ��Ѿ���װУ׼����
		if (fabs(m_pDynThread->GetFValue()) < 50.0f)
		{
			if (MessageBox(L"�밲װУ׼���ߺ��ȷ��", L"��У׼", MB_OKCANCEL) != IDOK)
			{
				m_nTestStep = 8;
				m_bIsPass = false;
				m_lbStep1.SetTextColor(RGB(0, 0, 0));
				m_lbStep1.SetText(L"�� ��װУ׼����");
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"�� ��������");
				m_lbStep3.SetTextColor(RGB(0, 0, 0));
				m_lbStep3.SetText(L"�� ��������");
				m_lbStep4.SetTextColor(RGB(0, 0, 0));
				m_lbStep4.SetText(L"�� ��������");
			}
			OnBnClickedButtonStart();
			return;
		}

		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// ���Զ���λ����
		m_pDynThread->SendCtrlWord('T');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"�� ��װУ׼����");
		m_lbInfo.SetText(L"���ڵ���...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nZeroTime = 0;
		SetTimer(4, 500, NULL);
	}
	else if (m_nTestStep == 2)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// �ر��Զ���λ����
		m_pDynThread->SendCtrlWord('U');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"�����5����������һ��");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 3;
	}
	else if (m_nTestStep == 3)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// У׼����
		m_pDynThread->SendCtrlWord('M');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

        m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"�� ��������");
		m_lbInfo.SetText(L"���ڵ���...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nStayTime = 0;
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		SetTimer(5, 1000, NULL);
	}
	else if (m_nTestStep == 4)
	{
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"������������������һ��");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 5;
	}
	else if (m_nTestStep == 5)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// ���Զ���λ����
		m_pDynThread->SendCtrlWord('T');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

        m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"�� ��������");
		m_lbInfo.SetText(L"���ڵ���...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nZeroTime = 0;
		SetTimer(6, 500, NULL);
	}
	else if (m_nTestStep == 6)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// �ر��Զ���λ����
		m_pDynThread->SendCtrlWord('U');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbInfo.SetText(L"�����5����������һ��");
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 7;
	}
	else if (m_nTestStep == 7)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// ��������У׼����	
		m_pDynThread->SendCtrlWord('N');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"�� ��������");
		m_lbInfo.SetText(L"���ڵ���...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nStayTime = 0;
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		SetTimer(7, 1000, NULL);
	}
	else if (m_nTestStep == 8)
	{
		m_lbStep5.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"�����������У׼���ߺ����һ��");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

		m_nTestStep = 9;
	}
	else if (m_nTestStep == 9)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// ���Զ���λ����
		m_pDynThread->SendCtrlWord('T');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

        m_lbStep5.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetText(L"�� ���������У׼����");
		m_lbInfo.SetText(L"���ڵ���...");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		m_nZeroTime = 0;
		SetTimer(8, 500, NULL);
	}
	else if (m_nTestStep == 10)
	{
		BeginWaitCursor();
		EnableRefreshRealTimeData(false);

		// ���Զ�-ƫ��-������Ϊ����
		m_pDynThread->SendCtrlWord('V');

		EnableRefreshRealTimeData(true);
		EndWaitCursor();

		m_lbStep6.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(L"���������У׼��������У׼");

		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"���У׼");

		m_nTestStep = 11;
		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == 11)
	{
		m_lbStep6.SetTextColor(RGB(0, 0, 0));
		m_lbStep6.SetText(L"�� ���");
		m_lbInfo.SetText(L"У׼���");	
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	
		m_nTestStep = 0;

		// ��¼�������
		m_bIsOperationFinishedAtLeaseOnce = true;

		// �����������
		ExportDynForceCalibrationResult();

		// �ص�������ȡУ׼��Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦����У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CAL_FINISH, wchInfo, sizeof(wchInfo));
		}
	}
}

void CMHDForceCalibrationDlg::OnBnClickedButtonReset()
{
	if (IDOK != MessageBox(L"ȷ��������У׼������?\r\n\r\n������У׼������,��Ҫ���½�����У׼", L"��У׼", MB_OKCANCEL|MB_ICONINFORMATION))
	{
		return;
	}

	BeginWaitCursor();
	EnableRefreshRealTimeData(false);

	// ��д����
	m_pDynThread->WriteVar(0, 9864);

	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar(29, 1000) 
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(55, 127)
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(56, 250))
	{
		m_lbInfo.SetText(L"�������,�����½�����У׼");
	}
	else
	{
		m_lbInfo.SetText(L"����ʧ��");
	}
	ClearResult();

	// �ر�д����
	m_pDynThread->WriteVar(0, 0);

	EnableRefreshRealTimeData(true);
	EndWaitCursor();
}

//LRESULT  CMHDForceCalibrationDlg::OnReadVar(WPARAM bSucess, LPARAM lParam)          //��ȡ�⹦��״̬
//{
//	if (bSucess != DYN_WR_SUCCEED)
//	{
//		m_lbInfo.SetText(L"��⹦��ͨѶ��ʱ");
//		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
//		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
//		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
//	}
//	else
//	{
//		if ((int)lParam == 54)// ȡ����������У׼����(�ϻ�)
//		{
//			m_fCalForce = (float)m_pDynThread->GetVarValue(54)/10.0f;
//		}
//	}
//	return 0;
//}

void CMHDForceCalibrationDlg::ExportDynForceCalibrationResult(void)
{	
	CString str;
	CString strResult(L"[ResultOfDynForceCal]");
	
	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// У׼�Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass?L"1":L"0");
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

void CMHDForceCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CMHDForceCalibrationDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	EnableRefreshRealTimeData(false);

	OnCancel();
}
