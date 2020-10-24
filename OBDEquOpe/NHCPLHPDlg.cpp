// NHCPLHPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCPLHPDlg.h"
#include <cmath>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// ��ʾʣ���������
//#ifndef SHOW_REMANENCE
//#define SHOW_REMANENCE
//#endif

// CNHCPLHPDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCPLHPDlg, CDialogZoom)

CNHCPLHPDlg::CNHCPLHPDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCPLHPDlg::IDD, pParent)
	//, m_bSerialPort(0)
	, m_usDIW(0)

	, m_dApWithRemanence(0.0)
	, m_dBpWithRemanence(0.0)
	, m_dCpWithRemanence(0.0)
	, m_dApWithoutRemanence(0.0)
	, m_dBpWithoutRemanence(0.0)
	, m_dCpWithoutRemanence(0.0)

	, m_fApWithRemanence(0.0)
	, m_fBpWithRemanence(0.0)
	, m_fCpWithRemanence(0.0)
	, m_fApWithoutRemanence(0.0)
	, m_fBpWithoutRemanence(0.0)
	, m_fCpWithoutRemanence(0.0)

	, m_pmUsed(PLHP_F_MODE)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_pfProcessCtrl(NULL)
{
	// �����⹦�������߳�
	m_pNHCPLHPThread = (CNHCPLHPThread*)AfxBeginThread(RUNTIME_CLASS(CNHCPLHPThread));

    ZeroMemory(m_fNomialVelocity, sizeof(m_fNomialVelocity));
    ZeroMemory(m_fPLHP, sizeof(m_fPLHP));
    ZeroMemory(m_fUpperVelocity, sizeof(m_fUpperVelocity));
    ZeroMemory(m_fLowerVelocity, sizeof(m_fLowerVelocity));

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
}

CNHCPLHPDlg::~CNHCPLHPDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pNHCPLHPThread)
	{
		m_pNHCPLHPThread->Close();
		m_pNHCPLHPThread->QuitThread();
		WaitForSingleObject(m_pNHCPLHPThread->m_hThread, 5000);
	}
}

void CNHCPLHPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_ASM_RANGE, m_rdoAsmRange);
	DDX_Control(pDX, IDC_RADIO_VMAS_RANGE, m_rdoVmasRange);
	DDX_Control(pDX, IDC_RADIO_LUGDOWN_RANGE, m_rdoLugdownRange);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lcResult);
	DDX_Control(pDX, IDC_STATIC_CURVE, m_stcPLHPCurve);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_DIW, m_lbDIW);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lb4);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_F, m_lbF);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_AP, m_lbAp);
	DDX_Control(pDX, IDC_STATIC_BP, m_lbBp);
	DDX_Control(pDX, IDC_STATIC_CP, m_lbCp);
	DDX_Control(pDX, IDC_STATIC_ANOTHER_AP, m_lbAnotherAp);
	DDX_Control(pDX, IDC_STATIC_ANOTHER_BP, m_lbAnotherBp);
	DDX_Control(pDX, IDC_STATIC_ANOTHER_CP, m_lbAnotherCp);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHCPLHPDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCPLHPDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCPLHPDlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCPLHPDlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCPLHPDlg::OnBnClickedButtonMotorOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CNHCPLHPDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START_TEST, &CNHCPLHPDlg::OnBnClickedButtonStartTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCPLHPDlg::OnBnClickedButtonStopTest)
	ON_MESSAGE(WM_DYN_PLHP_TEST_FINISHED, OnPLHPTestFinished)
	ON_MESSAGE(WM_DYN_PLHP_SECTION_FINISHED, OnPLHPSectionFinished)
	ON_BN_CLICKED(IDC_RADIO_ASM_RANGE, &CNHCPLHPDlg::OnBnClickedRadioAsmRange)
	ON_BN_CLICKED(IDC_RADIO_VMAS_RANGE, &CNHCPLHPDlg::OnBnClickedRadioVmasRange)
	ON_BN_CLICKED(IDC_RADIO_LUGDOWN_RANGE, &CNHCPLHPDlg::OnBnClickedRadioLugdownRange)
END_MESSAGE_MAP()


// CNHCPLHPDlg ��Ϣ�������

BOOL CNHCPLHPDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// �򿪴��ڣ�������ͨѶ����
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCPLHPDlg::InitCtrls(void)
{
	// ���öԻ�������
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦���������ʻ���");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	EnableBasicOperation(false);

	// ���ñ�ǩ������ɫ
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lb4.SetTextColor(RGB(0, 102, 204));

	// ���û���������ʾ����
	m_lbDIW.SetBkColor(RGB(230, 230, 230));

	// ����Ap,Bp,Cp��ʾ����
	m_lbAp.SetBkColor(RGB(230, 230, 230));
	m_lbAp.SetTextColor(RGB(0, 102, 204));
	m_lbBp.SetBkColor(RGB(230, 230, 230));
	m_lbBp.SetTextColor(RGB(0, 102, 204));
	m_lbCp.SetBkColor(RGB(230, 230, 230));
	m_lbCp.SetTextColor(RGB(0, 102, 204));

	// ������һAp,Bp,Cp��ʾ����
	m_lbAnotherAp.SetBkColor(RGB(230, 230, 230));
	m_lbAnotherAp.SetTextColor(RGB(0, 204, 102));
	m_lbAnotherBp.SetBkColor(RGB(230, 230, 230));
	m_lbAnotherBp.SetTextColor(RGB(0, 204, 102));
	m_lbAnotherCp.SetBkColor(RGB(230, 230, 230));
	m_lbAnotherCp.SetTextColor(RGB(0, 204, 102));

#ifndef SHOW_REMANENCE
	m_lbAnotherAp.ShowWindow(SW_HIDE);
	m_lbAnotherBp.ShowWindow(SW_HIDE);
	m_lbAnotherCp.ShowWindow(SW_HIDE);
#endif

	// ���ù��ʡ��ٶ���ʾ����
	m_lbF.SetLEDStyle();
	m_lbP.SetLEDStyle();
	m_lbV.SetLEDStyle();
	
	m_lcResult.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//m_lcResult.SetBkColor(RGB(230, 234, 243));
	//m_lcResult.SetTextBkColor(RGB(230, 234, 243));
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcResult.InsertColumn(0, L"�ٶ�����[km/h]",	LVCFMT_LEFT,	170*nSM_CXSCREEN/1280, 0);
	m_lcResult.InsertColumn(1, L"�����ٶ�[km/h]",	LVCFMT_LEFT,	170*nSM_CXSCREEN/1280, 1);
	m_lcResult.InsertColumn(2, L"����ʱ��[s]",		LVCFMT_LEFT,	150*nSM_CXSCREEN/1280, 2);
	m_lcResult.InsertColumn(3, L"��������[kW]",		LVCFMT_LEFT,	150*nSM_CXSCREEN/1280, 3);
#ifdef SHOW_REMANENCE
	m_lcResult.InsertColumn(4, L"ʣ��[kW]",			LVCFMT_LEFT,	120*nSM_CXSCREEN/1280, 4);
	m_lcResult.InsertColumn(5, L"ʣ��[N]",			LVCFMT_LEFT,	100*nSM_CXSCREEN/1280, 5);
#endif

	// ��ʼ����������ASM����
	m_rdoAsmRange.SetCheck(BST_CHECKED);
	OnBnClickedRadioAsmRange();
}

void CNHCPLHPDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCPLHPDlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pNHCPLHPThread->Open(bPort, imDyn))
	{
		// ����ͨѶ����
		// ��ȡ������������ͨѶ����
		if (DYN_WR_SUCCEED == m_pNHCPLHPThread->GetDIW(&m_usDIW))
		{
			// ��ʾ��������
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);
			// ��ȡ�������ʲ���,��ʣ��
			m_pNHCPLHPThread->ReadApWithRemanence(&m_fApWithRemanence);       //��ȡAp
			m_pNHCPLHPThread->ReadBpWithRemanence(&m_fBpWithRemanence);       //��ȡBp
			m_pNHCPLHPThread->ReadCpWithRemanence(&m_fCpWithRemanence);       //��ȡCp
			// ��ȡ�������ʲ���,��ʣ��
			m_pNHCPLHPThread->ReadApWithoutRemanence(&m_fApWithoutRemanence);       //��ȡAp
			m_pNHCPLHPThread->ReadBpWithoutRemanence(&m_fBpWithoutRemanence);       //��ȡBp
			m_pNHCPLHPThread->ReadCpWithoutRemanence(&m_fCpWithoutRemanence);       //��ȡCp

			// ת��ApBpCp
			ConvertApBpCp(false);

			// ��ʾApBpCp
			ShowtApBpCp();

			// ��ʾ����
			ShowPLHPCurve(true);
			// ��ʾ��ʵ����
			ShowAnotherPLHPCurve(true);

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("�⹦���������ʻ���"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
			EnableBasicOperation(true);
			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			EnableBasicOperation(false);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		EnableBasicOperation(false);
	}
}

void CNHCPLHPDlg::ShowtApBpCp(void)
{
	CString str;

	// ��ʾ��ʣ�Ų���
	str.Format(L"%.2f", m_fApWithRemanence);
	m_lbAp.SetText(str);
	str.Format(L"%.2f", m_fBpWithRemanence);
	m_lbBp.SetText(str);
	str.Format(L"%.2f", m_fCpWithRemanence);
	m_lbCp.SetText(str);

	// ��ʾ��ʣ�Ų���
	str.Format(L"%.2f", m_fApWithoutRemanence);
	m_lbAnotherAp.SetText(str);
	str.Format(L"%.2f", m_fBpWithoutRemanence);
	m_lbAnotherBp.SetText(str);
	str.Format(L"%.2f", m_fCpWithoutRemanence);
	m_lbAnotherCp.SetText(str);
}

void CNHCPLHPDlg::EnableRefreshRealTimeData(bool bEnable)
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

void CNHCPLHPDlg::RefreshRealTimeData(void)
{
	// ˢ��������
	// ͬʱ����ͨѶ����
	// 500����ִ��һ��

	CString str(L"");

	//if (DYN_WR_SUCCEED == m_pNHCPLHPThread->GetRealTimeData_Asyn())
	m_pNHCPLHPThread->GetRealTimeData_Asyn();
	{
		// ͨѶ�ɹ�
		str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
		GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
		GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
	}
	//else
	//{
	//	// ͨѶʧ��
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCPLHPDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(bEnable);
}

void CNHCPLHPDlg::RestoreCtrls(void)
{
	m_rdoAsmRange.EnableWindow(TRUE);
	m_rdoVmasRange.EnableWindow(TRUE);
	m_rdoLugdownRange.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CNHCPLHPDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCPLHPThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->GetVValue() > 10e-6)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ʱ�������������", L"��������", MB_OK|MB_ICONWARNING);
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

	m_pNHCPLHPThread->LiftBeamUp();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPDlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	m_pNHCPLHPThread->LiftBeamDown();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPDlg::OnBnClickedButtonMotorOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCPLHPThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"�����", MB_OK|MB_ICONERROR);
		return;
	}

	// ���ͽ�����������
	m_pNHCPLHPThread->LiftBeamDown();

	// ��������״̬
	dw = m_pNHCPLHPThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->IsTheStatusBitSignaled(0))
		{
			// �������Ѿ�����

			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"�뽵�;�������,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
		else if (!(m_pNHCPLHPThread->IsTheStatusBitSignaled(0)) && !(m_pNHCPLHPThread->IsTheStatusBitSignaled(1)))
		{
			// ���������ڽ��»�����������

			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ⱦ�������ȫ���º�,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"�����", MB_OK|MB_ICONERROR);
		return;
	}

	// ���а�ȫ��ʾ
	if (IDYES == MessageBox(L"��Ͳ����ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"�����", MB_YESNO|MB_ICONWARNING))
	{
		m_pNHCPLHPThread->MotorOn();
	}
	else
	{
		MessageBox(L"�������ȡ��", L"�����", MB_OK|MB_ICONWARNING);
	}

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPDlg::OnBnClickedButtonMotorOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	m_pNHCPLHPThread->MotorOff();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCPLHPDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if (1 == nIDEvent)
	{
		// �򿪴��ڣ�������ͨѶ

		KillTimer(1);
		OpenSerialPortAndTestCommunication();
		//TRACE("OnTimer 1\n");
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
		//TRACE("OnTimer 2\n");
	}
	else if (3 == nIDEvent)
	{
		DisplayPLHPTestProcess();
		//TRACE("OnTimer 3\n");
	}

	CDialogZoom::OnTimer(nIDEvent);
}

// ��ʾ�������ʲ�������
void CNHCPLHPDlg::DisplayPLHPTestProcess(void)
{
	switch (m_pNHCPLHPThread->GetPLHPState())
	{
	case NHC_PLHP_STATE_NOT_START:
		{
		}
		break;
	case NHC_PLHP_STATE_READY:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڳ�ʼ�����Բ���...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
		}
		break;
	case NHC_PLHP_STATE_LIFTBEAM_DOWN:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڽ�������...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
		}
		break;
	case NHC_PLHP_STATE_ACCELERATION:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڼ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
		}
		break;
	case NHC_PLHP_STATE_SLIP:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڻ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// ��ʾ�������ʻ���ʱ��
			DisplayPLHPSldingTime();
		}
		break;
	case NHC_PLHP_STATE_FINISHED:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"�������");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCPLHPThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetFValue());
			GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);

			// ֹͣˢ�¼������ʻ�������
			KillTimer(3);

			// �������ø��ɽ��ղ��Խ�����Ϣ�ĺ����ڴ���,����:OnPLHPTestFinished
			//// ����ȫ�����
			//// ����������ʻ��н��
			//CalculatePLHPParams();
			//// ��ʾ������������
			//ShowPLHPCurve();
			//// д��������ʻ��в������⹦��
			//// �ָ��ؼ�
			//RestoreCtrls();
			//// ��ʼˢ������
			//EnableRefreshRealTimeData(true);
		}
		break;
	case NHC_PLHP_STATE_ERROR:
		{
			if (NHC_PLHP_COMMUNICATION_ERROR == m_pNHCPLHPThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"������ֹ,��⹦��ͨѶ��ʱ");
			}
			else if (NHC_PLHP_START_MMTIMER_FAIL == m_pNHCPLHPThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"������ֹ,������ʱ��ʧ��");
			}
			//GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			//GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
			//GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			// ֹͣˢ�¼��ػ�������
			KillTimer(3);
			// �ָ��ؼ�
			RestoreCtrls();
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
		}
		break;
	default:
		break;
	}
}

void CNHCPLHPDlg::DisplayPLHPSldingTime(void)
{
	// Ҫע��,��λms��>s

	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM��������
		DisplayPLHPSldingTime_ASM();
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS��������
		DisplayPLHPSldingTime_VMAS();
	}
	else
	{
		// LUGDOWN��������
		DisplayPLHPSldingTime_LUGDOWN();
	}
}

void CNHCPLHPDlg::DisplayPLHPSldingTime_ASM(void)
{
	CString str(L"");
	//if (m_pNHCPLHPThread->GetVValue()<=51.0f && m_pNHCPLHPThread->GetVValue()>=48.0f)
	if (m_pNHCPLHPThread->GetVValue()>=48.0f)
	{
		// ˢ�µ�1�λ���ʱ��(51~45)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=45.0f)
	{
		// ˢ�µ�1�λ���ʱ��(51~45)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=40.0f)
	{
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=33.0f)
	{
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
		// ˢ�µ�3�λ���ʱ��(40~24)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=32.0f)
	{
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
		// ˢ�µ�3�λ���ʱ��(40~24)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
		// ˢ�µ�4�λ���ʱ��(33~17)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=24.0f)
	{
		// ˢ�µ�3�λ���ʱ��(40~24)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
		// ˢ�µ�4�λ���ʱ��(33~17)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=19.0f)
	{
		// ˢ�µ�4�λ���ʱ��(33~17)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=17.0f)
	{
		// ˢ�µ�4�λ���ʱ��(33~17)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
		// ˢ�µ�5�λ���ʱ��(19~13)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=13.0f)
	{
		// ˢ�µ�5�λ���ʱ��(19~13)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
}

void CNHCPLHPDlg::DisplayPLHPSldingTime_VMAS(void)
{
	CString str(L"");
	//if (m_pNHCPLHPThread->GetVValue()<=51.0f && m_pNHCPLHPThread->GetVValue()>=48.0f)
	if (m_pNHCPLHPThread->GetVValue()>=48.0f)
	{
		// ˢ�µ�1�λ���ʱ��(51~45)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=45.0f)
	{
		// ˢ�µ�1�λ���ʱ��(51~45)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=40.0f)
	{
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=32.0f)
	{
		// ˢ�µ�2�λ���ʱ��(48~32)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
		// ˢ�µ�3�λ���ʱ��(40~24)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=24.0f)
	{
		// ˢ�µ�3�λ���ʱ��(40~24)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
		// ˢ�µ�4�λ���ʱ��(32~16)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=19.0f)
	{
		// ˢ�µ�4�λ���ʱ��(32~16)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=16.0f)
	{
		// ˢ�µ�4�λ���ʱ��(32~16)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
		// ˢ�µ�5�λ���ʱ��(19~13)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=13.0f)
	{
		// ˢ�µ�5�λ���ʱ��(19~13)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
}

void CNHCPLHPDlg::DisplayPLHPSldingTime_LUGDOWN(void)
{
	CString str(L"");
	//if (m_pNHCPLHPThread->GetVValue()<=92.0f && m_pNHCPLHPThread->GetVValue()>=84.0f)
	if (m_pNHCPLHPThread->GetVValue()>=84.0f)
	{
		// ˢ�µ�1�λ���ʱ��(92~84)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(0)/1000.0f);
		m_lcResult.SetItemText(0, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=76.0f)
	{
		// ˢ�µ�2�λ���ʱ��(84~76)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(1)/1000.0f);
		m_lcResult.SetItemText(1, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=68.0f)
	{
		// ˢ�µ�3�λ���ʱ��(76~68)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(2)/1000.0f);
		m_lcResult.SetItemText(2, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=60.0f)
	{
		// ˢ�µ�4�λ���ʱ��(68~60)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(3)/1000.0f);
		m_lcResult.SetItemText(3, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=52.0f)
	{
		// ˢ�µ�5�λ���ʱ��(60~52)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(4)/1000.0f);
		m_lcResult.SetItemText(4, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=44.0f)
	{
		// ˢ�µ�6�λ���ʱ��(52~44)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(5)/1000.0f);
		m_lcResult.SetItemText(5, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=36.0f)
	{
		// ˢ�µ�7�λ���ʱ��(44~36)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(6)/1000.0f);
		m_lcResult.SetItemText(6, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=28.0f)
	{
		// ˢ�µ�8�λ���ʱ��(36~28)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(7)/1000.0f);
		m_lcResult.SetItemText(7, 2, str);
	}
	else if (m_pNHCPLHPThread->GetVValue()>=20.0f)
	{
		// ˢ�µ�9�λ���ʱ��(28~20)
		str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(8)/1000.0f);
		m_lcResult.SetItemText(8, 2, str);
	}
}

void CNHCPLHPDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	EnableRefreshRealTimeData(false);

	OnCancel();
}

void CNHCPLHPDlg::OnBnClickedButtonStartTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	CString str(L"");

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCPLHPThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCPLHPThread->GetVValue() > 0.05f)
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

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦���������ʻ��п�ʼ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_PLHP_START, wchInfo, sizeof(wchInfo));
	}

	// ��ȫ��ʾ���⹦����Ͳ��Ҫת��
	if (IDYES == MessageBox(L"���Թ��̹�Ͳ�������ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"��������", MB_YESNO|MB_ICONWARNING))
	{
		m_rdoAsmRange.EnableWindow(FALSE);
		m_rdoVmasRange.EnableWindow(FALSE);
		m_rdoLugdownRange.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// ��ֹ��������
		EnableBasicOperation(false);
		// ��������
		ClearPLHPResult();
		// ��ֹ�˳����Խ���
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		// ��ʼˢ�¼�����
		SetTimer(3, 200, NULL);
		// ������������
		CNHCPLHPThread::PLHPType pt;
		if (BST_CHECKED == m_rdoAsmRange.GetCheck())
		{
			// ASM��������
			pt = CNHCPLHPThread::PLHP_ASM;
		}
		else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
		{
			// VMAS��������
			pt = CNHCPLHPThread::PLHP_VMAS;
		}
		else
		{
			// LUGDOWN��������
			pt = CNHCPLHPThread::PLHP_LUGDOWN;
		}
		m_pNHCPLHPThread->StartPLHPTest_Asyn(pt, m_hWnd);
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		MessageBox(L"���Ա�ȡ��", L"��������", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
}

void CNHCPLHPDlg::OnBnClickedButtonStopTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���Ա��û�ֹͣ");

	KillTimer(3);

	m_pNHCPLHPThread->StopPLHPTest();

	RestoreCtrls();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦���������ʻ���ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_PLHP_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHCPLHPDlg::Polynom_berechnen(double *const pdXArray, double *const pdYArray, const int nAmount, double *const pdResult, const int grad)
{
	ASSERT(NULL != pdXArray);
	ASSERT(NULL != pdYArray);
	ASSERT(NULL != pdResult);
	ASSERT(nAmount> 0);

	register int i(0), j(0), k(0);
	double fakt(0.0);
	double hv(0.0);
	double alpha[5][6];
	double sum_x[6];
	ZeroMemory(alpha, sizeof(alpha));
	ZeroMemory(sum_x, sizeof(sum_x));

	// pdResult initialisieren
	for (i=0; i<=grad; i++)
	{
		pdResult[i] = 0.0;
	}

	/* Berechnung der rechten Seiten der Gleichungen */
	for (i = 0; i <= (grad + 1); i++)
	{
		for (j = 0; j <= grad+1; j++)
			alpha[i][j] = 0;
	}

	for (j = 0; j <= grad; j++)
	{
		for (i = 0; i < nAmount; i++)
		{
			hv = pdYArray[i];

			if (j > 0)
				hv *= pow (pdXArray[i], j);

			alpha[j][grad+1] += hv;
		}
	}

	/* Berechnung der pdResult-Matrix */
	alpha[0][0] = nAmount;

	for (i = 0; i < (2 * grad); i++)
		sum_x[i] = 0;

	for (i = 0; i < nAmount; i++)
	{
		for (j = 0; j < (2 * grad); j++)
			sum_x[j] += pow (pdXArray[i], j+1);
	}

	for (i = 0; i <= grad; i++)
	{
		for (j = 0; j<= grad; j++)
		{
			if (i == 0 && j == 0)
				continue;

			alpha[i][j] = sum_x[i+j-1];
		}
	}

	/* Gau?scher Algorithmus */
	for (k = 0; k <= grad; k++)
	{
		for (i = 0; i <= grad; i++)
		{
			if (i == k)
				continue;

			fakt = alpha[i][k];
			for (j = k; j <= (grad + 1); j++)
			{
				alpha[i][j] = (- fakt / alpha[k][k])
					* alpha[k][j] + alpha[i][j];
			}
		}
	}

	/* pdResult berechnen */
	for (i = 0; i <= grad; i++)
		pdResult[i] = alpha[i][grad+1] / alpha[i][i];
}

float CNHCPLHPDlg::CalculatePLHP(const float fUpperVelocity, const float fLowerVelocity, const UINT unSlidingTime)
{
	ASSERT(fUpperVelocity > 0.0f);
	ASSERT(fLowerVelocity > 0.0f);
	ASSERT(fUpperVelocity >= fLowerVelocity);
	ASSERT(unSlidingTime > 0);

	float f = m_usDIW * (fUpperVelocity*fUpperVelocity - fLowerVelocity*fLowerVelocity) / (2.0f * unSlidingTime * 3.6f * 3.6f);
	ASSERT(f > 0.0f);
	return (f * 100.0f + 0.5f) / 100.0f;
}

LRESULT CNHCPLHPDlg::OnPLHPTestFinished(WPARAM wParam, LPARAM lParam)
{
	// ���ò�����ɱ�־
	m_bIsOperationFinishedAtLeaseOnce = true;

	///////////////////////////����ȫ����ɣ����в������ݴ���begin
	// ����������ʻ��н��
	CalculatePLHPParams();
	// ��ʾ������������
	ShowPLHPCurve(true);
	// ��ʾ��ʵ������������
	ShowAnotherPLHPCurve(true);
	// ��ʾApBpCp
	ShowtApBpCp();
	// д��������ʻ��в������⹦��
	WritePLHPParamsToDyn();

	//ExportEquCalChkInfo();	// �����������ʲ��Խ��
	//ExportDemarcationLog();	// �����������ʲ��Լ�¼
	ExportDynPLHPResult();	// �����������ʲ��Խ��

	// �ָ��ؼ�
	RestoreCtrls();
	// ��ʼˢ������
	EnableRefreshRealTimeData(true);
	///////////////////////////����ȫ����ɣ����в������ݴ���end
	
	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦���������ʻ������", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_PLHP_FINISH, wchInfo, sizeof(wchInfo));
	}

	return 0;
}

LRESULT CNHCPLHPDlg::OnPLHPSectionFinished(WPARAM wParam, LPARAM lParam)
{
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM��������
		PLHPSectionFinished_ASM((UINT)wParam);
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS��������
		PLHPSectionFinished_VMAS((UINT)wParam);
	}
	else
	{
		// LUGDOWN��������
		PLHPSectionFinished_LUGDOWN((UINT)wParam);
	}
	return 0;
}

void CNHCPLHPDlg::PLHPSectionFinished_ASM(UINT unSection)
{
	ASSERT(unSection<5);

	CString str(L"");
	switch (unSection)
	{
	case 0:
		{
			// ��1�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(51, 45, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 1:
		{
			// ��2�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(48, 32, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 2:
		{
			// ��3�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(40, 24, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 3:
		{
			// ��4�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(33, 17, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 4:
		{
			// ��5�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(19, 13, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	default:
		break;
	}
}

void CNHCPLHPDlg::PLHPSectionFinished_VMAS(UINT unSection)
{
	ASSERT(unSection<5);

	CString str(L"");
	switch (unSection)
	{
	case 0:
		{
			// ��1�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(51, 45, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 1:
		{
			// ��2�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(48, 32, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 2:
		{
			// ��3�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(40, 24, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 3:
		{
			// ��4�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(32, 16, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 4:
		{
			// ��5�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(19, 13, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	default:
		break;
	}
}

void CNHCPLHPDlg::PLHPSectionFinished_LUGDOWN(UINT unSection)
{
	ASSERT(unSection<9);

	CString str(L"");
	switch (unSection)
	{
	case 0:
		{
			// ��1�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(92, 84, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 1:
		{
			// ��2�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(84, 76, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 2:
		{
			// ��3�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(76, 69, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 3:
		{
			// ��4�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(68, 60, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 4:
		{
			// ��5�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(60, 52, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 5:
		{
			// ��6�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(52, 44, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 6:
		{
			// ��7�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(44, 36, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 7:
		{
			// ��8�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(36, 28, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	case 8:
		{
			// ��9�����
			// ��ʾ����ʱ��
			str.Format(L"%.3f", m_pNHCPLHPThread->GetPLHPTime(unSection)/1000.0f);
			m_lcResult.SetItemText(unSection, 2, str);
			// ��ʾ��������
			m_fPLHP[unSection] = CalculatePLHP(28, 20, m_pNHCPLHPThread->GetPLHPTime(unSection));
			str.Format(L"%.2f",m_fPLHP[unSection]);
			m_lcResult.SetItemText(unSection, 3, str);
#ifdef SHOW_REMANENCE
			// ��ʾʣ��
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByPower(unSection));
			m_lcResult.SetItemText(unSection, 4, str);
			str.Format(L"%.2f", m_pNHCPLHPThread->GetRemanenceAveByForce(unSection));
			m_lcResult.SetItemText(unSection, 5, str);
#endif
		}
		break;
	default:
		break;
	}
}

void CNHCPLHPDlg::OnBnClickedRadioAsmRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����ٶ�
	m_fNomialVelocity[0] = 48.0f;
	m_fNomialVelocity[1] = 40.0f;
	m_fNomialVelocity[2] = 32.0f;
	m_fNomialVelocity[3] = 25.0f;
	m_fNomialVelocity[4] = 16.0f;
	m_fNomialVelocity[5] = 0.0f;
	m_fNomialVelocity[6] = 0.0f;
	m_fNomialVelocity[7] = 0.0f;
	m_fNomialVelocity[8] = 0.0f;

	// ���ٵ�
	m_fUpperVelocity[0] = 51.0f;
	m_fUpperVelocity[1] = 48.0f;
	m_fUpperVelocity[2] = 40.0f;
	m_fUpperVelocity[3] = 33.0f;
	m_fUpperVelocity[4] = 19.0f;
	m_fUpperVelocity[5] = 0.0f;
	m_fUpperVelocity[6] = 0.0f;
	m_fUpperVelocity[7] = 0.0f;
	m_fUpperVelocity[8] = 0.0f;

	// ���ٵ�
	m_fLowerVelocity[0] = 45.0f;
	m_fLowerVelocity[1] = 32.0f;
	m_fLowerVelocity[2] = 24.0f;
	m_fLowerVelocity[3] = 17.0f;
	m_fLowerVelocity[4] = 13.0f;
	m_fLowerVelocity[5] = 0.0f;
	m_fLowerVelocity[6] = 0.0f;
	m_fLowerVelocity[7] = 0.0f;
	m_fLowerVelocity[8] = 0.0f;

	m_lcResult.DeleteAllItems();
	m_lcResult.InsertItem(0, L"51~45");
	m_lcResult.SetItemText(0, 1, L"48");
	m_lcResult.InsertItem(1, L"48~32");
	m_lcResult.SetItemText(1, 1, L"40");
	m_lcResult.InsertItem(2, L"40~24");
	m_lcResult.SetItemText(2, 1, L"32");
	m_lcResult.InsertItem(3, L"33~17");
	m_lcResult.SetItemText(3, 1, L"25");
	m_lcResult.InsertItem(4, L"19~13");
	m_lcResult.SetItemText(4, 1, L"16");
}

void CNHCPLHPDlg::OnBnClickedRadioVmasRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����ٶ�
	m_fNomialVelocity[0] = 48.0f;
	m_fNomialVelocity[1] = 40.0f;
	m_fNomialVelocity[2] = 32.0f;
	m_fNomialVelocity[3] = 24.0f;
	m_fNomialVelocity[4] = 16.0f;
	m_fNomialVelocity[5] = 0.0f;
	m_fNomialVelocity[6] = 0.0f;
	m_fNomialVelocity[7] = 0.0f;
	m_fNomialVelocity[8] = 0.0f;

	// ���ٵ�
	m_fUpperVelocity[0] = 51.0f;
	m_fUpperVelocity[1] = 48.0f;
	m_fUpperVelocity[2] = 40.0f;
	m_fUpperVelocity[3] = 32.0f;
	m_fUpperVelocity[4] = 19.0f;
	m_fUpperVelocity[5] = 0.0f;
	m_fUpperVelocity[6] = 0.0f;
	m_fUpperVelocity[7] = 0.0f;
	m_fUpperVelocity[8] = 0.0f;

	// ���ٵ�
	m_fLowerVelocity[0] = 45.0f;
	m_fLowerVelocity[1] = 32.0f;
	m_fLowerVelocity[2] = 24.0f;
	m_fLowerVelocity[3] = 16.0f;
	m_fLowerVelocity[4] = 13.0f;
	m_fLowerVelocity[5] = 0.0f;
	m_fLowerVelocity[6] = 0.0f;
	m_fLowerVelocity[7] = 0.0f;
	m_fLowerVelocity[8] = 0.0f;

	m_lcResult.DeleteAllItems();
	m_lcResult.InsertItem(0, L"51~45");
	m_lcResult.SetItemText(0, 1, L"48");
	m_lcResult.InsertItem(1, L"48~32");
	m_lcResult.SetItemText(1, 1, L"40");
	m_lcResult.InsertItem(2, L"40~24");
	m_lcResult.SetItemText(2, 1, L"32");
	m_lcResult.InsertItem(3, L"32~16");
	m_lcResult.SetItemText(3, 1, L"24");
	m_lcResult.InsertItem(4, L"19~13");
	m_lcResult.SetItemText(4, 1, L"16");
}

void CNHCPLHPDlg::OnBnClickedRadioLugdownRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����ٶ�
	m_fNomialVelocity[0] = 88.0f;
	m_fNomialVelocity[1] = 80.0f;
	m_fNomialVelocity[2] = 72.0f;
	m_fNomialVelocity[3] = 64.0f;
	m_fNomialVelocity[4] = 56.0f;
	m_fNomialVelocity[5] = 48.0f;
	m_fNomialVelocity[6] = 40.0f;
	m_fNomialVelocity[7] = 32.0f;
	m_fNomialVelocity[8] = 24.0f;

	// ���ٵ�
	m_fUpperVelocity[0] = 92.0f;
	m_fUpperVelocity[1] = 84.0f;
	m_fUpperVelocity[2] = 76.0f;
	m_fUpperVelocity[3] = 68.0f;
	m_fUpperVelocity[4] = 60.0f;
	m_fUpperVelocity[5] = 52.0f;
	m_fUpperVelocity[6] = 44.0f;
	m_fUpperVelocity[7] = 36.0f;
	m_fUpperVelocity[8] = 28.0f;

	// ���ٵ�
	m_fLowerVelocity[0] = 84.0f;
	m_fLowerVelocity[1] = 76.0f;
	m_fLowerVelocity[2] = 68.0f;
	m_fLowerVelocity[3] = 60.0f;
	m_fLowerVelocity[4] = 52.0f;
	m_fLowerVelocity[5] = 44.0f;
	m_fLowerVelocity[6] = 36.0f;
	m_fLowerVelocity[7] = 28.0f;
	m_fLowerVelocity[8] = 20.0f;

	m_lcResult.DeleteAllItems();
	m_lcResult.InsertItem(0, L"92~84");
	m_lcResult.SetItemText(0, 1, L"88");
	m_lcResult.InsertItem(1, L"84~76");
	m_lcResult.SetItemText(1, 1, L"80");
	m_lcResult.InsertItem(2, L"76~68");
	m_lcResult.SetItemText(2, 1, L"72");
	m_lcResult.InsertItem(3, L"68~60");
	m_lcResult.SetItemText(3, 1, L"64");
	m_lcResult.InsertItem(4, L"60~52");
	m_lcResult.SetItemText(4, 1, L"56");
	m_lcResult.InsertItem(5, L"52~44");
	m_lcResult.SetItemText(5, 1, L"48");
	m_lcResult.InsertItem(6, L"44~36");
	m_lcResult.SetItemText(6, 1, L"40");
	m_lcResult.InsertItem(7, L"36~28");
	m_lcResult.SetItemText(7, 1, L"32");
	m_lcResult.InsertItem(8, L"28~20");
	m_lcResult.SetItemText(8, 1, L"24");
}

void CNHCPLHPDlg::CalculatePLHPParams(void)
{
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM
		CalculatePLHPParams_ASM();
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS
		CalculatePLHPParams_VMAS();
	}
	else
	{
		// LUGDOWN
		CalculatePLHPParams_LUGDOWN();
	}

	// ת��ApBpCp
	ConvertApBpCp(true);
}

void CNHCPLHPDlg::CalculatePLHPParams_ASM(void)
{
	double *pdXArray = new double[4]();
	double *pdYArray = new double[4]();
	double *pdResult = new double[4]();

	// ��1������
	pdXArray[0] = 0.0;
	pdYArray[0] = 0.0;
	// ��2������(��5��:19~13)
	pdXArray[1] = m_fNomialVelocity[4];
	pdYArray[1] = m_fPLHP[4];
	// ��3������(��3��:40~24)
	pdXArray[2] = m_fNomialVelocity[2];
	pdYArray[2] = m_fPLHP[2];
	// ��4������(��1��:51~45)
	pdXArray[3] = m_fNomialVelocity[0];
	pdYArray[3] = m_fPLHP[0];

	Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
	m_dApWithRemanence = pdResult[1];
	m_dBpWithRemanence = pdResult[2];
	m_dCpWithRemanence = pdResult[3];

	if (PLHP_NORMAL_MODE == m_pmUsed)
	{
		// ƽ��ģʽ
		m_dApWithoutRemanence = m_dApWithRemanence;
		m_dBpWithoutRemanence = m_dBpWithRemanence;
		m_dCpWithoutRemanence = m_dCpWithRemanence;
	}
	else if (PLHP_F_MODE == m_pmUsed)
	{
		// ��ģʽ

		// ���������������

		// ��1������
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// ��2������(��5��:19~13)
		pdXArray[1] = m_fNomialVelocity[4];
		pdYArray[1] = m_fPLHP[4] - (m_pNHCPLHPThread->GetRemanenceAveByForce(4)*m_fNomialVelocity[4]/3.6f/1000.0f);
		// ��3������(��3��:40~24)
		pdXArray[2] = m_fNomialVelocity[2];
		pdYArray[2] = m_fPLHP[2] - (m_pNHCPLHPThread->GetRemanenceAveByForce(2)*m_fNomialVelocity[2]/3.6f/1000.0f);
		// ��4������(��1��:51~45)
		pdXArray[3] = m_fNomialVelocity[0];
		pdYArray[3] = m_fPLHP[0] - (m_pNHCPLHPThread->GetRemanenceAveByForce(0)*m_fNomialVelocity[0]/3.6f/1000.0f);
		// ��Ͻ��
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}
	else
	{
		// ����ģʽ

		// ���������������

		// ��1������
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// ��2������(��5��:19~13)
		pdXArray[1] = m_fNomialVelocity[4];
		pdYArray[1] = m_fPLHP[4] - m_pNHCPLHPThread->GetRemanenceAveByPower(4);
		// ��3������(��3��:40~24)
		pdXArray[2] = m_fNomialVelocity[2];
		pdYArray[2] = m_fPLHP[2] - m_pNHCPLHPThread->GetRemanenceAveByPower(2);
		// ��4������(��1��:51~45)
		pdXArray[3] = m_fNomialVelocity[0];
		pdYArray[3] = m_fPLHP[0] - m_pNHCPLHPThread->GetRemanenceAveByPower(0);
		// ��Ͻ��
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}

	delete[] pdXArray;
	delete[] pdYArray;
	delete[] pdResult;
}

void CNHCPLHPDlg::CalculatePLHPParams_VMAS(void)
{
	// ��ASMһ��
	CalculatePLHPParams_ASM();
}

void CNHCPLHPDlg::CalculatePLHPParams_LUGDOWN(void)
{
	double *pdXArray = new double[4]();
	double *pdYArray = new double[4]();
	double *pdResult = new double[4]();

	// ��2�����ݣ��õ�9������(��9��:28~20)
	const int nArray2(8);
	// ��3�����ݣ��õ�8������(��8��:36~28)
	const int nArray3(7);
	// ��4�����ݣ��õ�2������(��2��:84~76)
	const int nArray4(1);

	// ��1������
	pdXArray[0] = 0.0;
	pdYArray[0] = 0.0;
	// ��2������(��9��:28~20)
	pdXArray[1] = m_fNomialVelocity[nArray2];
	pdYArray[1] = m_fPLHP[nArray2];
	// ��3������(��8��:36~28)
	pdXArray[2] = m_fNomialVelocity[nArray3];
	pdYArray[2] = m_fPLHP[nArray3];
	// ��4������(��2��:84~76)
	pdXArray[3] = m_fNomialVelocity[nArray4];
	pdYArray[3] = m_fPLHP[nArray4];

	Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
	m_dApWithRemanence = pdResult[1];
	m_dBpWithRemanence = pdResult[2];
	m_dCpWithRemanence = pdResult[3];

	if (PLHP_NORMAL_MODE == m_pmUsed)
	{
		// ƽ��ģʽ
		m_dApWithoutRemanence = m_dApWithRemanence;
		m_dBpWithoutRemanence = m_dBpWithRemanence;
		m_dCpWithoutRemanence = m_dCpWithRemanence;
	}
	else if (PLHP_F_MODE == m_pmUsed)
	{
		// ��ģʽ

		// ���������������

		// ��1������
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// ��2������(��9��:28~20)
		pdXArray[1] = m_fNomialVelocity[nArray2];
		pdYArray[1] = m_fPLHP[nArray2] - (m_pNHCPLHPThread->GetRemanenceAveByForce(nArray2)*m_fNomialVelocity[nArray2]/3.6f/1000.0f);
		// ��3������(��8��:36~28)
		pdXArray[2] = m_fNomialVelocity[nArray3];
		pdYArray[2] = m_fPLHP[nArray3] - (m_pNHCPLHPThread->GetRemanenceAveByForce(nArray3)*m_fNomialVelocity[nArray3]/3.6f/1000.0f);
		// ��4������(��2��:84~76)
		pdXArray[3] = m_fNomialVelocity[nArray4];
		pdYArray[3] = m_fPLHP[nArray4] - (m_pNHCPLHPThread->GetRemanenceAveByForce(nArray4)*m_fNomialVelocity[nArray4]/3.6f/1000.0f);
		// ��Ͻ��
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}
	else
	{
		// ����ģʽ

		// ���������������

		// ��1������
		pdXArray[0] = 0.0;
		pdYArray[0] = 0.0;
		// ��2������(��9��:28~20)
		pdXArray[1] = m_fNomialVelocity[nArray2];
		pdYArray[1] = m_fPLHP[nArray2] - m_pNHCPLHPThread->GetRemanenceAveByPower(nArray2);
		// ��3������(��8��:36~28)
		pdXArray[2] = m_fNomialVelocity[nArray3];
		pdYArray[2] = m_fPLHP[nArray3] - m_pNHCPLHPThread->GetRemanenceAveByPower(nArray3);
		// ��4������(��2��:84~76)
		pdXArray[3] = m_fNomialVelocity[nArray4];
		pdYArray[3] = m_fPLHP[nArray4] - m_pNHCPLHPThread->GetRemanenceAveByPower(nArray4);
		// ��Ͻ��
		pdResult[0] = pdResult[1] = pdResult[2] = pdResult[3] = 0.0;

		Polynom_berechnen(pdXArray, pdYArray, 4, pdResult, 3);
		m_dApWithoutRemanence = pdResult[1];
		m_dBpWithoutRemanence = pdResult[2];
		m_dCpWithoutRemanence = pdResult[3];
	}

	delete[] pdXArray;
	delete[] pdYArray;
	delete[] pdResult;
}

void CNHCPLHPDlg::ConvertApBpCp(bool bConvertToPower)
{
	if (bConvertToPower)
	{
		// ת���ɹ���(����80.5)

		// ��ʵAp(��ʣ��)
		m_fApWithoutRemanence = (float)(m_dApWithoutRemanence*80.5);
		// ����2λС��������������
		if (m_fApWithoutRemanence >= 0.0f)
		{
			// ����
			m_fApWithoutRemanence = int(m_fApWithoutRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// ����
			m_fApWithoutRemanence = int(m_fApWithoutRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// ��ʵBp(��ʣ��)
		m_fBpWithoutRemanence = (float)(m_dBpWithoutRemanence*80.5*80.5);
		// ����2λС��������������
		if (m_fBpWithoutRemanence >= 0.0f)
		{
			// ����
			m_fBpWithoutRemanence = (int)(m_fBpWithoutRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// ����
			m_fBpWithoutRemanence = (int)(m_fBpWithoutRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// ��ʵCp(��ʣ��)
		m_fCpWithoutRemanence = (float)(m_dCpWithoutRemanence*80.5*80.5*80.5);
		// ����2λС��������������
		if (m_fCpWithoutRemanence >= 0.0f)
		{
			// ����
			m_fCpWithoutRemanence = (int)(m_fCpWithoutRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// ����
			m_fCpWithoutRemanence = (int)(m_fCpWithoutRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// ��ʾAp(��ʣ��)
		m_fApWithRemanence = (float)(m_dApWithRemanence*80.5);
		// ����2λС��������������
		if (m_fApWithRemanence >= 0.0f)
		{
			// ����
			m_fApWithRemanence = (int)(m_fApWithRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// ����
			m_fApWithRemanence = (int)(m_fApWithRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// ��ʾBp(��ʣ��)
		m_fBpWithRemanence = (float)(m_dBpWithRemanence*80.5*80.5);
		// ����2λС��������������
		if (m_fBpWithRemanence >= 0.0f)
		{
			// ����
			m_fBpWithRemanence = (int)(m_fBpWithRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// ����
			m_fBpWithRemanence = (int)(m_fBpWithRemanence * 100.0f - 0.5f) / 100.0f;
		}

		// ��ʾCp(��ʣ��)
		m_fCpWithRemanence = (float)(m_dCpWithRemanence*80.5*80.5*80.5);
		// ����2λС��������������
		if (m_fCpWithRemanence >= 0.0f)
		{
			// ����
			m_fCpWithRemanence = (int)(m_fCpWithRemanence * 100.0f + 0.5f) / 100.0f;
		}
		else
		{
			// ����
			m_fCpWithRemanence = (int)(m_fCpWithRemanence * 100.0f - 0.5f) / 100.0f;
		}
	}
	else
	{
		// ת����һ�����(����80.5)

		// ��ȡ�������ʲ���,��ʣ��
		m_dApWithRemanence = m_fApWithRemanence / 80.5;
		m_dBpWithRemanence = m_fBpWithRemanence / 80.5 / 80.5;
		m_dCpWithRemanence = m_fCpWithRemanence / 80.5 / 80.5 / 80.5;
		// ��ȡ�������ʲ���,��ʣ��
		m_dApWithoutRemanence = m_fApWithoutRemanence / 80.5;
		m_dBpWithoutRemanence = m_fBpWithoutRemanence / 80.5 / 80.5;
		m_dCpWithoutRemanence = m_fCpWithoutRemanence / 80.5 / 80.5 / 80.5;
	}
}

void CNHCPLHPDlg::ShowPLHPCurve(bool bShow)
{
	if (bShow)
	{
		m_stcPLHPCurve.SetParams(m_dCpWithRemanence, m_dBpWithRemanence, m_dApWithRemanence);
	}
	m_stcPLHPCurve.ShowCurve(bShow);
}

void CNHCPLHPDlg::ShowAnotherPLHPCurve(bool bShow)
{
#ifdef SHOW_REMANENCE
	if (bShow)
	{
		m_stcPLHPCurve.SetAnotherParams(m_dCpWithoutRemanence, m_dBpWithoutRemanence, m_dApWithoutRemanence);
	}
	m_stcPLHPCurve.ShowAnotherCurve(bShow);
#endif
}

void CNHCPLHPDlg::WritePLHPParamsToDyn(void)
{
	// Ap��Bp��Cp���ܳ��ָ������
	// �ϻ��⹦��������⹦���Ը�������ͬ����

	//////////////////////////////////////////////////////////////////////////////////////����⹦�����⴦��begin
	// 1)���CpС��0����Ϊ0
	// 2)���Ap��Bpͬʱ�����㣬�Ž���д��������
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
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// ת��Ϊ��д
	strDynProtocol.MakeUpper();
	if (L"MAHA" == strDynProtocol)
	{
		if (m_fCpWithoutRemanence < 0.0f)
		{
			m_fCpWithoutRemanence = 0.0f;
		}
		if (m_fApWithoutRemanence<=0.0f || m_fBpWithoutRemanence<=0.0f)
		{
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////����⹦�����⴦��end

	// д��ʵAp(��ʣ��)
	m_pNHCPLHPThread->WriteApWithoutRemanence(m_fApWithoutRemanence);
	// д��ʵBp(��ʣ��)
	m_pNHCPLHPThread->WriteBpWithoutRemanence(m_fBpWithoutRemanence);
	// д��ʵCp(��ʣ��)
	m_pNHCPLHPThread->WriteCpWithoutRemanence(m_fCpWithoutRemanence);

	// д��ʾAp(��ʣ��)
	m_pNHCPLHPThread->WriteApWithRemanence(m_fApWithRemanence);
	// д��ʾBp(��ʣ��)
	m_pNHCPLHPThread->WriteBpWithRemanence(m_fBpWithRemanence);
	// д��ʾCp(��ʣ��)
	m_pNHCPLHPThread->WriteCpWithRemanence(m_fCpWithRemanence);
}

void CNHCPLHPDlg::ClearPLHPResult(void)
{
	// �������
	m_lbAp.SetText(L"--");
	m_lbBp.SetText(L"--");
	m_lbCp.SetText(L"--");
	m_lbAnotherAp.SetText(L"--");
	m_lbAnotherBp.SetText(L"--");
	m_lbAnotherCp.SetText(L"--");
	// ���������������
	ShowPLHPCurve(false);
	// �����ʵ������������
	ShowAnotherPLHPCurve(false);
	// �����������:m_fPLHP
	ZeroMemory(m_fPLHP, sizeof(m_fPLHP));
	// ���ListCtrl����
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM
		OnBnClickedRadioAsmRange();
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS
		OnBnClickedRadioVmasRange();
	}
	else
	{
		// LUGDOWN
		OnBnClickedRadioLugdownRange();
	}
}

/*
void CNHCPLHPDlg::ExportEquCalChkInfo()
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

	// ��ȡ����߱�ź��豸�ͺ�
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
	str.Format(L"\r\nItem=%s", L"�⹦���������ʻ���");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		strData.Format(L"PLHP40:%.2f,PLHP25:%.2f", m_fPLHP[1], m_fPLHP[3]);
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		strData.Format(L"PLHP40:%.2f,PLHP24:%.2f", m_fPLHP[1], m_fPLHP[3]);
	}
	else if (BST_CHECKED == m_rdoLugdownRange.GetCheck())
	{
		strData.Format(L"PLHP80:%.2f,PLHP72:%.2f,PLHP64:%.2f,PLHP56:%.2f,PLHP48:%.2f,PLHP40:%.2f", m_fPLHP[1], m_fPLHP[2], m_fPLHP[3], m_fPLHP[4], m_fPLHP[5], m_fPLHP[6]);
	}
	else
	{
		// Ĭ��ΪASM��Χ
		strData.Format(L"PLHP40:%.2f,PLHP25:%.2f", m_fPLHP[1], m_fPLHP[3]);
	}
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strError(L"");
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	str.Format(L"\r\nJudgement=%s", L"ͨ��");
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
void CNHCPLHPDlg::ExportDemarcationLog(void)
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
	str.Format(L"\r\nDemarcationItem=%d", DYN_PLHP);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", L"1");
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

void CNHCPLHPDlg::ExportDynPLHPResult(void)
{	
	CString str;
	CString strResult(L"[ResultOfDynPLHP]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ��������
	str.Format(L"\r\nDIW=%d", m_usDIW);
	strResult += str;

	//	PLHPMode	int	Checked
	CString strPLHPMode(L"1");
	if (PLHP_NORMAL_MODE == m_pmUsed)
	{
		strPLHPMode = L"1";
	}
	else if (PLHP_F_MODE == m_pmUsed)
	{
		strPLHPMode = L"2";
	}
	else
	{
		strPLHPMode = L"3";
	}
	str.Format(L"\r\nPLHPMode=%s", strPLHPMode);
	strResult += str;

	//  PLHPType	int	Checked
	CString strPLHPType(L"1");
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		// ASM��������
		strPLHPType = L"1";
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		// VMAS��������
		strPLHPType = L"2";
	}
	else
	{
		// LUGDOWN��������
		strPLHPType = L"3";
	}
	str.Format(L"\r\nPLHPType=%s", strPLHPType);
	strResult += str;

	//	ApWithRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nApWithRemanence=%.2f", m_fApWithRemanence);
	strResult += str;

	//	BpWithRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nBpWithRemanence=%.2f", m_fBpWithRemanence);
	strResult += str;

	//	CpWithRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nCpWithRemanence=%.2f", m_fCpWithRemanence);
	strResult += str;

	//	ApWithoutRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nApWithoutRemanence=%.2f", m_fApWithoutRemanence);
	strResult += str;

	//	BpWithoutRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nBpWithoutRemanence=%.2f", m_fBpWithoutRemanence);
	strResult += str;

	//	CpWithoutRemanence	decimal(18, 4)	Checked
	str.Format(L"\r\nCpWithoutRemanence=%.2f", m_fCpWithoutRemanence);
	strResult += str;

	for (int i=0; i<9; i++)
	{
		str.Format(L"\r\nLowerVelocity%d=%.2f", i, m_fLowerVelocity[i]);
		strResult += str;

		str.Format(L"\r\nUpperVelocity%d=%.2f", i, m_fUpperVelocity[i]);
		strResult += str;

		str.Format(L"\r\nNomialVelocity%d=%.2f", i, m_fNomialVelocity[i]);
		strResult += str;

		str.Format(L"\r\nPLHPTime%d=%u", i, m_pNHCPLHPThread->GetPLHPTime(i));
		strResult += str;

		str.Format(L"\r\nPLHP%d=%.2f", i, m_fPLHP[i]);
		strResult += str;

		str.Format(L"\r\nRemanenceByForce%d=%.2f", i, m_pNHCPLHPThread->GetRemanenceAveByForce(i));
		strResult += str;

		str.Format(L"\r\nRemanenceByPower%d=%.2f", i, m_pNHCPLHPThread->GetRemanenceAveByPower(i));
		strResult += str;
	}
	str.Format(L"\r\nPass=%s", L"1");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCPLHPDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}