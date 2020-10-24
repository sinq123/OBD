// NHCForceCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCForceCalibrationDlg.h"
#include "math.h"

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCForceCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCForceCalibrationDlg, CDialogZoom)

CNHCForceCalibrationDlg::CNHCForceCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCForceCalibrationDlg::IDD, pParent)
{
    m_nTestStep = 0;
	m_nZeroTime = 0;
	m_fCalForce = 0;
	m_nStayTime = 0;
	m_bIsOperationFinishedAtLeaseOnce = false;
	m_bIsPass = true;

	ZeroMemory(m_fStayF, sizeof(float)*5);

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

CNHCForceCalibrationDlg::~CNHCForceCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCForceCalibrationDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CNHCForceCalibrationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHCForceCalibrationDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CNHCForceCalibrationDlg::OnBnClickedButtonReset)
	ON_MESSAGE(WM_DYN_READ_VAR,OnReadVar)
END_MESSAGE_MAP()


// CNHCForceCalibrationDlg ��Ϣ�������

BOOL CNHCForceCalibrationDlg::OnInitDialog()
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

void CNHCForceCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, CDynThread::IM_NANHUA_NHC03))
	{
		std::bitset<16> bit16(0);
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bit16))
		{
			m_lbInfo.SetText(_T("��У׼ǰ����װУ׼����"));
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
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

void CNHCForceCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
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
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(_T("�밲װ��У׼���ߺ����һ��"));
			m_nTestStep = 1;
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		}
		break;

	case 3:
		{
			m_pDynThread->GetRealTimeData_Asyn(NULL);//��ȡʵʱ����
			CString str;
			str.Format(_T("%.1f"), m_pDynThread->GetFValue());
			m_lbTorsion.SetText(str);
		}
		break;

	case 4:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1)
			{
				m_nZeroTime ++;
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = 2;
					KillTimer(4);
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
			if (m_nStayTime >= 5)// �����Ѿ�����
			{
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					m_nTestStep = 4;
					KillTimer(5);
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
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = 6;
					KillTimer(6);
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
			if (m_nStayTime >= 5)// �����Ѿ�����
			{
				if ( fabs(m_pDynThread->GetFValue() - m_fStayF[0]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[1]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[2]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[3]) < 20.0f &&
					fabs(m_pDynThread->GetFValue() - m_fStayF[4]) < 20.0f )
				{
					m_nTestStep = 8;
					KillTimer(7);
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
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = 10;
					KillTimer(8);
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


void CNHCForceCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCForceCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦����У׼");

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbReset.SetTextColor(RGB(0, 102, 204));

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	m_lbTorsion.SetLEDStyle();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";

	CSimpleIni si(strPath);
	m_strWeightCount = si.GetString(L"DynForceCal", L"WeightCount", L"3");
}

void CNHCForceCalibrationDlg::ClearResult()             //��������
{
	m_lbInfo.StopFlashText();
	m_lbInfo.SetText(_T("��У׼ǰ����װУ׼����"));
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

void CNHCForceCalibrationDlg::OnBnClickedButtonStart()
{
	if (m_nTestStep == 0)
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

			ClearResult();             //��������

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

			m_pDynThread->ReadVar_Asyn(54, m_hWnd);// ȡ��������У׼����(�ϻ�)
			m_pDynThread->SendCtrlWord_Asyn('0');//�˳����п���ģʽ
			m_pDynThread->SendCtrlWord_Asyn('U');//�ر��Զ���λ����
			m_pDynThread->SendCtrlWord_Asyn('L');//������λУ׼(��¼��λ)

			m_lbInfo.SetText(_T("���ڵ���..."));
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == 1)        //У׼����
	{
		// �ж��Ƿ��Ѿ���װУ׼����
		if (fabs(m_pDynThread->GetFValue()) < 50.0f)
		{
			if (MessageBox(_T("�밲װУ׼���ߺ��ȷ��!"), _T("��У׼"), MB_OKCANCEL) != IDOK)
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
	
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"�� ��װУ׼����");

		m_lbInfo.SetText(_T("���ڵ���..."));
		m_pDynThread->SendCtrlWord_Asyn('T');//���Զ���λ����
		SetTimer(4, 500, NULL);
		m_nZeroTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (m_nTestStep == 2)        
	{
		m_lbInfo.SetText(_T("�����")+m_strWeightCount+_T("����������һ��"));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		m_pDynThread->SendCtrlWord_Asyn('U');//�ر��Զ���λ����
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 3;
	}
	else if (m_nTestStep == 3)
	{
		// �ж����������Ƿ�ﵽҪ������1000N��Χ
		if (m_fCalForce > 0.0f)
		{
			if (fabs(m_pDynThread->GetFValue()-m_fCalForce) > 1000.0f)
			{
				MessageBox(_T("���������쳣!"), _T("��У׼"));
				m_bIsPass = false;
				m_nTestStep = 8;
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"�� ��������");
				m_lbStep3.SetTextColor(RGB(0, 0, 0));
				m_lbStep3.SetText(L"�� ��������");
				m_lbStep4.SetTextColor(RGB(0, 0, 0));
				m_lbStep4.SetText(L"�� ��������");
				OnBnClickedButtonStart();
				return;
			}
		}

        m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"�� ��������");

		m_pDynThread->SendCtrlWord_Asyn('M');//У׼����
		m_lbInfo.SetText(_T("���ڵ���..."));
		m_nStayTime = 0;
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		SetTimer(5, 1000, NULL);
	}
	else if (m_nTestStep == 4)
	{
		m_lbInfo.SetText(_T("������������������һ��"));	
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 5;
	}
	else if (m_nTestStep == 5)
	{
        m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"�� ��������");

		m_lbInfo.SetText(_T("���ڵ���..."));		
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');//���Զ���λ����
		SetTimer(6, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == 6)
	{
		m_pDynThread->SendCtrlWord_Asyn('U');//�ر��Զ���λ����

		m_lbInfo.SetText(_T("�����")+m_strWeightCount+_T("����������һ��"));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 7;
	}
	else if (m_nTestStep == 7)
	{
		// �ж����������Ƿ�ﵽҪ������1000N��Χ
		if (m_fCalForce > 0.0f)
		{
			if (fabs(m_pDynThread->GetFValue()-m_fCalForce) > 1000.0f)
			{
				MessageBox(_T("���������쳣!"), _T("��У׼"));
				m_bIsPass = false;
				m_nTestStep = 8;
				m_lbStep4.SetTextColor(RGB(0, 0, 0));
				m_lbStep4.SetText(L"�� ��������");
				OnBnClickedButtonStart();
				return;
			}
		}

		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"�� ��������");

		m_lbInfo.SetText(_T("���ڵ���..."));
		ZeroMemory(m_fStayF, sizeof(m_fStayF));
		m_nStayTime = 0;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('N');//��������У׼����	
		SetTimer(7, 1000, NULL);
	}
	else if (m_nTestStep == 8)
	{
		m_lbInfo.SetText(_T("�����������У׼���ߺ����һ��"));	
		m_lbStep5.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_nTestStep = 9;
	}
	else if (m_nTestStep == 9)
	{
		if (m_pDynThread->GetFValue() > 50.0f)
		{
			MessageBox(_T("�����������У׼����,�ٵ����һ��"), _T("��У׼"), MB_OK|MB_ICONWARNING);
			return;
		}

        m_lbStep5.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetText(L"�� ���������У׼����");

		m_lbInfo.SetText(_T("���ڵ���..."));
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');//���Զ���λ����
		SetTimer(8, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == 10)
	{
		m_lbStep6.SetTextColor(RGB(0, 0, 255));
		m_lbInfo.SetText(_T("���������У׼��������У׼"));
		m_pDynThread->SendCtrlWord_Asyn('V');    //���Զ�-ƫ��-������Ϊ����
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("���У׼"));
		m_nTestStep = 11;

		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == 11)
	{
		m_lbInfo.SetText(_T("У׼���"));	
		m_lbStep6.SetTextColor(RGB(0, 0, 0));
		m_lbStep6.SetText(L"�� ���");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	
		m_nTestStep = 0;
		//KillTimer(3);

		m_bIsOperationFinishedAtLeaseOnce = true;	//��¼�������

		//ExportEquCalChkInfo();	// �����б���
		//ExportDemarcationLog();	// �����궨��¼
		ExportDynForceCalibrationResult();	// �����Զ�����

		// �ص�������ȡУ׼��Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦����У׼���", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CAL_FINISH, wchInfo, sizeof(wchInfo));
		}
	}
}



void CNHCForceCalibrationDlg::OnBnClickedButtonReset()
{
	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar(29, 1000) 
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(55, 127)
		&& DYN_WR_SUCCEED == m_pDynThread->WriteVar(56, 250))
	{
		m_lbInfo.SetText(_T("�������,�����½�����У׼"));
	}
	else
	{
		m_lbInfo.SetText(_T("����ʧ��"));
	}
	ClearResult();
}

LRESULT  CNHCForceCalibrationDlg::OnReadVar(WPARAM bSucess, LPARAM lParam)          //��ȡ�⹦��״̬
{
	if (bSucess != DYN_WR_SUCCEED)
	{
		m_lbInfo.SetText(_T("��⹦��ͨѶ��ʱ"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��ʼ"));
	}
	else
	{
		//if ((int)lParam == 6)
		//{
		//	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		//	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
		//}
		if ((int)lParam == 54)// ȡ����������У׼����(�ϻ�)
		{
			m_fCalForce = (float)m_pDynThread->GetVarValue(54)/10.0f;
			SetTimer(2, 1500, NULL);
			SetTimer(3, 500, NULL);
		}
	}
	return 0;
}

/*
void CNHCForceCalibrationDlg::ExportEquCalChkInfo(void)
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
void CNHCForceCalibrationDlg::ExportDemarcationLog(void)
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
	
void CNHCForceCalibrationDlg::ExportDynForceCalibrationResult(void)
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

void CNHCForceCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}
