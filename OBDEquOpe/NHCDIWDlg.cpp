// NHCDIWDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCDIWDlg.h"
#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif
// CNHCDIWDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCDIWDlg, CDialogZoom)

CNHCDIWDlg::CNHCDIWDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCDIWDlg::IDD, pParent)
	, m_fLowerForce(0.0f)
	, m_fUpperForce(0.0f)
	, m_fLowerVelocity(0.0f)
	, m_fUpperVelocity(0.0f)
	, m_unLowerSlipingTime(0)
	, m_unUpperSlipingTime(0)
	, m_usDIW(0)
	, m_usMeasuredDIW(0)
	, m_fAveLowerForce(110.0f)
	, m_fAveUpperForce(1110.0f)
	, m_wAp(0)
	, m_wBp(0)
	, m_wCp(0)
{
	// �����⹦�������߳�
	m_pNHCDIWThread = (CNHCDIWThread*)AfxBeginThread(RUNTIME_CLASS(CNHCDIWThread));

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

	// �������ļ���ȡ�������ͻ�������
	////////////////////////////////////////////////////////// �������ļ���ȡ�������ͻ�������begin
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
	str = si.GetString(L"DynDIW", L"LowerForce", L"110");
	m_fLowerForce = (float)_tstof(str);
	str = si.GetString(L"DynDIW", L"UpperForce", L"1110");
	m_fUpperForce = (float)_tstof(str);
	ASSERT(m_fUpperForce>m_fLowerForce);
	str = si.GetString(L"DynDIW", L"LowerVelocity", L"16");
	m_fLowerVelocity = (float)_tstof(str);
	str = si.GetString(L"DynDIW", L"UpperVelocity", L"48");
	m_fUpperVelocity = (float)_tstof(str);
	ASSERT(m_fUpperVelocity>m_fLowerVelocity);
	////////////////////////////////////////////////////////// �������ļ���ȡ�������ͻ�������end
	
	//// д������������������
	//m_fLowerForce = 110;
	//m_fUpperForce = 1110;
	//m_fLowerVelocity = 16.0f;
	//m_fUpperVelocity = 48.0f;
}

CNHCDIWDlg::~CNHCDIWDlg()
{
	m_fontDlgFont.DeleteObject();

	// ���»ָ��������ʲ���
	if (0 != m_wAp)
	{
		m_pNHCDIWThread->WriteVar(39, m_wAp);
	}
	if (0 != m_wBp)
	{
		m_pNHCDIWThread->WriteVar(40, m_wBp);
	}
	if (0 != m_wCp)
	{
		m_pNHCDIWThread->WriteVar(41, m_wCp);
	}

	if (NULL != m_pNHCDIWThread)
	{
		m_pNHCDIWThread->Close();
		m_pNHCDIWThread->QuitThread();
		WaitForSingleObject(m_pNHCDIWThread->m_hThread, 5000);
	}
}

void CNHCDIWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_F, m_lbF);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_EDIT_LOWER_FORCE, m_edLowerForce);
	DDX_Control(pDX, IDC_EDIT_UPPER_FORCE, m_edUpperForce);
	DDX_Control(pDX, IDC_EDIT_LOWER_VELOCITY, m_edLowerVelocity);
	DDX_Control(pDX, IDC_EDIT_UPPER_VELOCITY, m_edUpperVelocity);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_LOWER_SLIPINGTIME, m_lbLowerSlipingTime);
	DDX_Control(pDX, IDC_STATIC_UPPER_SLIPINGTIME, m_lbUpperSlipingTime);
	DDX_Control(pDX, IDC_STATIC_AVE_LOWER_FORCE, m_lbAveLowerForce);
	DDX_Control(pDX, IDC_STATIC_AVE_UPPER_FORCE, m_lbAveUpperForce);
	DDX_Control(pDX, IDC_STATIC_MEASURED_DIW, m_lbMeasuredDIW);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lb4);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCDIWDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_TEST, &CNHCDIWDlg::OnBnClickedButtonStartTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCDIWDlg::OnBnClickedButtonStopTest)
	ON_BN_CLICKED(IDCANCEL, &CNHCDIWDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DIW, &CNHCDIWDlg::OnBnClickedButtonWriteDiw)
END_MESSAGE_MAP()


// CNHCDIWDlg ��Ϣ�������

BOOL CNHCDIWDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCDIWDlg::InitCtrls(void)
{
	SetDlgFont();
	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	m_shdTitle.SetTitle(L"�⹦��������������");

	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);

	// ���ù��ʡ������ٶ���ʾ����
	m_lbP.SetLEDStyle();
	m_lbF.SetLEDStyle();
	m_lbV.SetLEDStyle();

	// ���ñ༭������(ֻ��,ֻ��������)
	m_edLowerForce.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edUpperForce.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edLowerVelocity.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edUpperVelocity.SendMessage(EM_SETREADONLY, TRUE, 0);
	m_edLowerForce.ModifyStyle(0, ES_NUMBER);
	m_edUpperForce.ModifyStyle(0, ES_NUMBER);
	m_edLowerVelocity.ModifyStyle(0, ES_NUMBER);
	m_edUpperVelocity.ModifyStyle(0, ES_NUMBER);

	// ��ʾ���Բ���
	CString str;
	str.Format(L"%.0f", m_fLowerForce);
	m_edLowerForce.SetWindowTextW(str);
	GetDlgItem(IDC_STATIC_LOWER_FORCE)->SetWindowTextW(str);
	str.Format(L"%.0f", m_fUpperForce);
	m_edUpperForce.SetWindowTextW(str);
	GetDlgItem(IDC_STATIC_UPPER_FORCE)->SetWindowTextW(str);
	str.Format(L"%.0f", m_fLowerVelocity);
	m_edLowerVelocity.SetWindowTextW(str);
	str.Format(L"%.0f", m_fUpperVelocity);
	m_edUpperVelocity.SetWindowTextW(str);

	// ���ñ�ǩ������ɫ
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lb4.SetTextColor(RGB(0, 102, 204));

	// ���ò��Խ�����
	m_lbLowerSlipingTime.SetBkColor(RGB(230, 230, 230));
	m_lbUpperSlipingTime.SetBkColor(RGB(230, 230, 230));
	m_lbAveLowerForce.SetBkColor(RGB(230, 230, 230));
	m_lbAveUpperForce.SetBkColor(RGB(230, 230, 230));
	m_lbMeasuredDIW.SetBkColor(RGB(230, 230, 230));
}

void CNHCDIWDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCDIWDlg::EnableRefreshRealTimeData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(2, 500, NULL);
	}
	else
	{
		KillTimer(2);
	}
}

void CNHCDIWDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	if (1 == nIDEvent)
	{
		// ������ʼ�����򿪴���
		KillTimer(1);
		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}
	else if (3 == nIDEvent)
	{
		DisplayDIWTestProcess();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCDIWDlg::RefreshRealTimeData(void)
{
	// ˢ��������
	// ͬʱ����ͨѶ����
	// 500����ִ��һ��

	CString str(L"");
	//if (DYN_WR_SUCCEED == m_pNHCDIWThread->GetRealTimeData())
	m_pNHCDIWThread->GetRealTimeData_Asyn(NULL);
	{
		// ͨѶ�ɹ�
		str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
		GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
		GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
	}
	//else
	//{
	//	// ͨѶʧ��
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCDIWDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// ת��Ϊ��д
	strDynProtocol.MakeUpper();
	CDynThread::InstrumentModel imDyn;
	if (L"NANHUA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}
	else if (L"MAHA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_MAHA_ASMP;
	}
	//else if (L"VAMAGRSR576" == strDynProtocol)
	//{
	//	imDyn = CDynThread::IM_VAMAG_RSR576;
	//}
	else
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}

	if (DYN_OPEN_SUCCEED == m_pNHCDIWThread->Open(bPort, imDyn))
	{
		// ����ͨѶ����
		// ��ȡ״̬����ͨѶ����
		if (DYN_WR_SUCCEED == m_pNHCDIWThread->GetDIW(&m_usDIW))
		{
			// �����������������������
			// ȡ���������ʲ���,�ݴ�����,�˳�����ʱ����д��
			m_pNHCDIWThread->ReadVar(39, &m_wAp);
			m_pNHCDIWThread->ReadVar(40, &m_wBp);
			m_pNHCDIWThread->ReadVar(41, &m_wCp);
			// �������
			m_pNHCDIWThread->WriteVar(39, 0);
			m_pNHCDIWThread->WriteVar(40, 0);
			m_pNHCDIWThread->WriteVar(41, 0);

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("������������"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
			// ��ʾ��ƻ�������
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);
	}
}

void CNHCDIWDlg::DisplayDIWTestProcess(void)
{
	switch (m_pNHCDIWThread->GetDIWState())
	{
	case NHC_DIW_STATE_NOT_START:
		{
			// ����δ��ʼ
			break;
		}
	case NHC_DIW_STATE_READY:
		{
			// ׼������
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڳ�ʼ�����Բ���...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			break;
		}
	case NHC_DIW_STATE_LIFTBEAM_DOWN:
		{
			// ��������
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڽ�������...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			break;
		}
	case NHC_DIW_STATE_ACCELERATION_1:
		{
			// ����
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڼ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			break;
		}
	case NHC_DIW_STATE_SLIP_1:
		{
			// ����(С������)
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڻ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// ��ʾ����ʱ�䣬��λms��>s
			m_unLowerSlipingTime = m_pNHCDIWThread->GetLowerSlipingTime();
			str.Format(L"%.3f", m_unLowerSlipingTime / 1000.0f);
			GetDlgItem(IDC_STATIC_LOWER_SLIPINGTIME)->SetWindowTextW(str);
			// ��ʾ������ƽ��ֵ(С������)
			m_fAveLowerForce = m_pNHCDIWThread->GetAveLowerForce();
			if (m_fAveLowerForce > 10e-6f)
			{
				// ��Ϊ����ʱ,����ʾ
				str.Format(L"%.1f", m_fAveLowerForce);
				GetDlgItem(IDC_STATIC_AVE_LOWER_FORCE)->SetWindowTextW(str);
			}
			break;
		}
	case NHC_DIW_STATE_WAIT_FOR_STOP:
		{
			// �ȴ���Ͳֹͣ
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڻ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			break;
		}
	case NHC_DIW_STATE_ACCELERATION_2:
		{
			// ����
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڼ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			break;
		}
	case NHC_DIW_STATE_SLIP_2:
		{
			// ����(�������)
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڻ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCDIWThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.1f", m_pNHCDIWThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCDIWThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// ��ʾ����ʱ�䣬��λms��>s
			m_unUpperSlipingTime = m_pNHCDIWThread->GetUpperSlipingTime();
			str.Format(L"%.3f", m_unUpperSlipingTime / 1000.0f);
			GetDlgItem(IDC_STATIC_UPPER_SLIPINGTIME)->SetWindowTextW(str);
			// ��ʾ������ƽ��ֵ(�������)
			m_fAveUpperForce = m_pNHCDIWThread->GetAveUpperForce();
			if (m_fAveUpperForce > 10e-6f)
			{
				// ��Ϊ����ʱ,����ʾ
				str.Format(L"%.1f", m_fAveUpperForce);
				GetDlgItem(IDC_STATIC_AVE_UPPER_FORCE)->SetWindowTextW(str);
			}
			break;
		}
	case NHC_DIW_STATE_FINISHED:
		{
			// ���

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"�������");
			// ֹͣ��ʾ����
			KillTimer(3);
			// ����ʵ���������
			CalculateDIW();
			// ��ʾʵ���������
			CString str;
			str.Format(L"%hu", m_usMeasuredDIW);
			GetDlgItem(IDC_STATIC_MEASURED_DIW)->SetWindowText(str);
			//��ʼˢ������
			EnableRefreshRealTimeData(true);
			// ���ÿ�ʼ����
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			// ����ֹͣ����
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			// ����д��
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
			// �����˳����Խ���
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			break;
		}
	case NHC_DIW_STATE_ERROR:
		{
			// ���Թ��̷�������

			if (NHC_DIW_COMMUNICATION_ERROR == m_pNHCDIWThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"������ֹ,��⹦��ͨѶ��ʱ");
			}
			else if (NHC_DIW_START_MMTIMER_FAIL == m_pNHCDIWThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"������ֹ,������ʱ��ʧ��");
			}
			// ֹͣ��ʾ����
			KillTimer(3);
			//��ʼˢ������
			EnableRefreshRealTimeData(true);
			// ���ÿ�ʼ����
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			// ����ֹͣ����
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			// ����д��
			GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
			// �����˳����Խ���
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			break;
		}
	default:
		{
			break;
		}
	}
}

void CNHCDIWDlg::CalculateDIW(void)
{
	float f = (m_fAveUpperForce-m_fAveLowerForce) * (m_unLowerSlipingTime/1000.0f) * (m_unUpperSlipingTime/1000.0f) / ((m_fUpperVelocity-m_fLowerVelocity)/3.6f) / ((m_unLowerSlipingTime-m_unUpperSlipingTime)/1000.0f);
	// ��������
	m_usMeasuredDIW = USHORT(f + 0.5f);
}

void CNHCDIWDlg::OnBnClickedButtonStartTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	CString str(L"");

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCDIWThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCDIWThread->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ�ʼ����", L"��������", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"��������", MB_OK|MB_ICONERROR);
		return;
	}

	// ��ȫ��ʾ���⹦����Ͳ��Ҫת��
	if (IDYES == MessageBox(L"���Թ��̹�Ͳ�������ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"��������", MB_YESNO|MB_ICONWARNING))
	{
		// ��ֹ��ʼ����
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		// ����ֹͣ����
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// ����д��
		GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(FALSE);
		// ��ֹ�˳����Խ���
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

		// ������������
		m_lbLowerSlipingTime.SetText(L"--");
		m_lbUpperSlipingTime.SetText(L"--");
		m_lbAveLowerForce.SetText(L"--");
		m_lbAveUpperForce.SetText(L"--");
		m_lbMeasuredDIW.SetText(L"--");

		// ��ʼˢ�¼�����
		SetTimer(3, 200, NULL);
		m_pNHCDIWThread->StartDIWTest_Asyn(m_fLowerForce, m_fUpperForce, m_fLowerVelocity, m_fUpperVelocity, m_hWnd);
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		MessageBox(L"���Ա�ȡ��", L"��������", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
	////////////////////////////////////////д�����������Ե���־
	CString strBZDIW;//��׼ֵ
	CString strCSDIW;//����ֵ
	EQUCALCHKINFO sEquCalChkInfo;	
	ZeroMemory(&sEquCalChkInfo, sizeof(sEquCalChkInfo));
	wcscpy_s(sEquCalChkInfo.wchItem, L"�⹦��������������");
	GetDlgItem(IDC_STATIC_DIW)->GetWindowTextW(strBZDIW);
	GetDlgItem(IDC_STATIC_DIW)->GetWindowTextW(strCSDIW);
	CString strData;
	strData.Format(L"��׼ֵ%c,����ֵ%c",strBZDIW,strCSDIW);
	wcscpy_s(sEquCalChkInfo.wchData, strData);
	//// дEquCalChkInfo��
	SetDboEquCalChkInfo(sEquCalChkInfo);
	
}

void CNHCDIWDlg::OnBnClickedButtonStopTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���Ա��û�ֹͣ");

	// ֹͣ��ʾ��������
	KillTimer(3);

	m_pNHCDIWThread->StopDIWTest();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	// ���ÿ�ʼ����
	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
	// ����ֹͣ����
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	// ����д��
	GetDlgItem(IDC_BUTTON_WRITE_DIW)->EnableWindow(TRUE);
	// �����˳����Խ���
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHCDIWDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	EnableRefreshRealTimeData(false);

	OnCancel();
}

void CNHCDIWDlg::OnBnClickedButtonWriteDiw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	CString str;
	GetDlgItem(IDC_EDIT_DIW_VALUE_TO_WRITE)->GetWindowText(str);
	// �ж��Ƿ�������д��ֵ
	if (L"" == str)
	{
		MessageBox(L"д��ֵ����Ϊ��!", L"��������", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_DIW_VALUE_TO_WRITE)->SetFocus();
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		return;
	}
	USHORT usNewDIW = (USHORT)_tstoi(str);
	if (usNewDIW<800 || usNewDIW>1000)
	{
		// ��ʾд��ֵ����һ�㷶Χ
		str.Format(L"��Ҫд��Ļ�������ֵΪ %hukg,����һ�㷶Χ��(800~1000kg)\n\n�Ƿ����д��?", usNewDIW);
		if (IDNO == MessageBox(str, L"��������", MB_YESNO|MB_ICONWARNING))
		{
			MessageBox(L"д�������ȡ��", L"��������", MB_OK|MB_ICONINFORMATION);
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			return;
		}
	}
	// д���������
	if (DYN_WR_SUCCEED != m_pNHCDIWThread->SetDIW(usNewDIW))
	{
		MessageBox(L"д���������ʧ��", L"��������", MB_OK|MB_ICONERROR);
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		return;
	}
	// �ȱ���ɵĻ�������
	USHORT usOldDIW = m_usDIW;
	// ������������,�����µ���ʾ��
	m_pNHCDIWThread->GetDIW(&m_usDIW);
	str.Format(L"%hu", m_usDIW);
	GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);
	EndWaitCursor();

	if (usOldDIW != usNewDIW)
	{
		// ��ʾ��Ҫ���м������ʲ���
		MessageBox(L"��ƻ��������Ѿ��ı�,��Ҫ���½��м������ʲ���", L"��������", MB_OK|MB_ICONINFORMATION);
	}
}
