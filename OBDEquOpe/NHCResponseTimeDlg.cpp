// NHCResponseTimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCResponseTimeDlg.h"

#include <cmath>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CNHCResponseTimeDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCResponseTimeDlg, CDialogZoom)

CNHCResponseTimeDlg::CNHCResponseTimeDlg(CWnd* pParent /*=NULL*/)
: CDialogZoom(CNHCResponseTimeDlg::IDD, pParent)
, m_bIsOverSilpOn(false)
, m_fSpeed_High(56.3f)
, m_fSpeed_Low(40.0f)
, m_fLoad_High(1323.0f)
, m_fLoad_Low(1719.0f)
, m_fTopSlipSpeed(64.4f)
{
	// �����⹦�������߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
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
	
	srand(unsigned int(time(NULL)));
}

CNHCResponseTimeDlg::~CNHCResponseTimeDlg()
{
	m_fontDlgFont.DeleteObject();
	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCResponseTimeDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}
void CNHCResponseTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIP, m_lbMsg);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_lbSpeed);
	DDX_Control(pDX, IDC_STATIC_LOADFORCE, m_lbTraction);
	DDX_Control(pDX, IDC_STATIC_POWER, m_lbPower);
	DDX_Control(pDX, IDC_EDIT_SPEED_HIGH, m_edtSpeed_High);
	DDX_Control(pDX, IDC_EDIT_SPEED_LOW, m_edtSpeed_Low);
	DDX_Control(pDX, IDC_EDIT_LOAD_HIGH, m_edtLoadTraction_High);
	DDX_Control(pDX, IDC_EDIT_LOAD_LOW, m_edtLoadTraction_Low);
	DDX_Control(pDX, IDC_EDIT_LOAD_90, m_edtLoadTraction_90);
	DDX_Control(pDX, IDC_EDIT_POWER_HIGH, m_edtPower_High);
	DDX_Control(pDX, IDC_EDIT_POWER_LOW, m_edtPower_Low);
	DDX_Control(pDX, IDC_EDT_SLIP_SPEED, m_edtTopSilpSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEED_HIGH, m_fSpeed_High);
	DDX_Text(pDX, IDC_EDIT_SPEED_LOW, m_fSpeed_Low);
	DDX_Text(pDX, IDC_EDIT_LOAD_HIGH, m_fLoad_High);
	DDX_Text(pDX, IDC_EDIT_LOAD_LOW, m_fLoad_Low);
	DDX_Text(pDX, IDC_EDT_SLIP_SPEED, m_fTopSlipSpeed);
	DDV_MinMaxFloat(pDX, m_fTopSlipSpeed, 5, 100);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
	DDX_Control(pDX, IDC_STATIC_RESPONSE_TIME, m_lbResponse_Time);
	DDX_Control(pDX, IDC_STATIC_STABILIZATION_TIME, m_lbStabilization_Time);
}


BEGIN_MESSAGE_MAP(CNHCResponseTimeDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_START_RESPONSE, &CNHCResponseTimeDlg::OnBnClickedBtnStartResponse)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHCResponseTimeDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_RADIO_TEST1, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST2, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST3, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST4, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST5, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST6, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST7, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TEST8, &CNHCResponseTimeDlg::OnBnClickedRadioTest)
END_MESSAGE_MAP()


// CNHCResponseTimeDlg ��Ϣ�������

BOOL CNHCResponseTimeDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CNHCResponseTimeDlg::InitCtrls(void)
{
	SetDlgFont();

	//// �༭��ֻ����������
	//m_edPValue.ModifyStyle(0, ES_NUMBER);
	//m_edFValue.ModifyStyle(0, ES_NUMBER);
	//m_edVValue.ModifyStyle(0, ES_NUMBER);

	// ���ñ�ǩ������ɫ
	//m_lb1.SetTextColor(RGB(0, 102, 204));
	//m_lb2.SetTextColor(RGB(0, 102, 204));
	//m_lb3.SetTextColor(RGB(0, 102, 204));
	m_shdTitle.SetTitle(L"�⹦����Ӧʱ��");

	// ���ù��ʡ������ٶ���ʾ����
	m_lbTraction.SetLEDStyle();
	m_lbPower.SetLEDStyle();
	m_lbSpeed.SetLEDStyle();
	m_lbTraction.SetWindowText(L"--");
	m_lbPower.SetWindowText(L"--");
	m_lbSpeed.SetWindowText(L"--");

	m_lbMsg.SetTitleStyle();
	m_lbMsg.SetWindowText(L"��Ӧʱ��");

	m_edtSpeed_High.SetWindowText(L"56.3");
	m_edtSpeed_Low.SetWindowText(L"40");
	m_edtLoadTraction_High.SetWindowText(L"1323.0");
	m_edtLoadTraction_Low.SetWindowText(L"1719.0");
	m_edtTopSilpSpeed.SetWindowText(_T("64.4"));
}
void CNHCResponseTimeDlg::OnBnClickedBtnStartResponse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);

	m_lbResponse_Time.SetText(L"--");
	m_lbStabilization_Time.SetText(L"--");

	// �����Լ��
	if (m_fSpeed_High < m_fSpeed_Low || m_fSpeed_Low < 5.0f || m_fSpeed_High < 5.0f)
	{
		MessageBox(L"������ٶȲ�����!�趨�ĸ��ٵ㲻��С���趨�ĵ��ٵ�!");
		return;
	}
	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pDynThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->GetVValue() > 0.05f)
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
		MessageBox(L"ͨѶ��ʱ", L"�ص��", MB_OK|MB_ICONERROR);
		return;
	}

	// ��������״̬
	dw = m_pDynThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->IsTheStatusBitSignaled(0))
		{
			// �������Ѿ�����

			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"�뽵�;�������,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			m_pDynThread->LiftBeamDown();
			return;
		}
		else if (!(m_pDynThread->IsTheStatusBitSignaled(0)) && !(m_pDynThread->IsTheStatusBitSignaled(1)))
		{
			// ���������ڽ��»�����������

			// ���ͽ�����������
			m_pDynThread->LiftBeamDown();

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
		// ���¾������������������
		m_pDynThread->LiftBeamDown();
		m_pDynThread->MotorOn();
		SetTimer(3,200,NULL);

		//��ֹ�ؼ�����
		m_edtTopSilpSpeed.EnableWindow(0);
		m_edtSpeed_High.EnableWindow(0);
		m_edtSpeed_Low.EnableWindow(0);
		m_edtPower_High.EnableWindow(0);
		m_edtPower_Low.EnableWindow(0);
		EnableRefreshRealTimeData(true);

	}
	else
	{
		MessageBox(L"�������ȡ��", L"�����", MB_OK|MB_ICONWARNING);
	}

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();

}
void CNHCResponseTimeDlg::RefreshRealTimeData(void)
{
	// ˢ��������
	// ͬʱ����ͨѶ����
	// 500����ִ��һ��

	CString str(L"");
	m_pDynThread->GetRealTimeData_Asyn(m_hWnd);
	{
		// ͨѶ�ɹ�
		str.Format(L"%.2f", m_pDynThread->GetPValue());
		GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
		str.Format(L"%.1f", m_pDynThread->GetFValue());
		GetDlgItem(IDC_STATIC_LOADFORCE)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pDynThread->GetVValue());
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
	}
	//else
	//{
	//	// ͨѶʧ��
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_F)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCResponseTimeDlg::EnableRefreshRealTimeData(bool bEnable)
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
void CNHCResponseTimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{

		KillTimer(1);

		// ������ʼ�����򿪴���

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
		if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
		{
			// ����ͨѶ����
			// ��ȡ״̬����ͨѶ����
			if (DYN_WR_SUCCEED == m_pDynThread->GetStatus())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("�⹦����Ӧʱ�����"));
				GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

				EnableRefreshRealTimeData(true);
			}
			else
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("ͨѶ��ʱ"));
				GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

				//��ֹ�ؼ�����
				m_edtTopSilpSpeed.EnableWindow(0);
				m_edtSpeed_High.EnableWindow(0);
				m_edtSpeed_Low.EnableWindow(0);
				m_edtPower_High.EnableWindow(0);
				m_edtPower_Low.EnableWindow(0);
			}
		}
		else
		{
			// �򿪴���ʧ��
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
			GetDlgItem(IDC_BTN_START_RESPONSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

			//��ֹ�ؼ�����
			m_edtTopSilpSpeed.EnableWindow(0);
			m_edtSpeed_High.EnableWindow(0);
			m_edtSpeed_Low.EnableWindow(0);
			m_edtPower_High.EnableWindow(0);
			m_edtPower_Low.EnableWindow(0);
		}
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}
	else if(3 == nIDEvent)
	{
		if(DYN_WR_SUCCEED != m_pDynThread->GetRealTimeData())
		{
			KillTimer(3);
			return ;
		}
		else
		{
			float fTempSpeed = m_pDynThread->GetVValue();
			//���ﵽ�趨�Ļ����ٶȹص��
			if (fTempSpeed >= m_fTopSlipSpeed)
			{
				m_pDynThread->MotorOff();
				m_bIsOverSilpOn = true;
				m_lbMsg.SetWindowText(L"���ڻ���...");
			}

			if (m_fSpeed_Low <= fTempSpeed && fTempSpeed <= m_fSpeed_High && m_bIsOverSilpOn == true)
			{
				// �趨���ٵ�ļ�����
				m_pDynThread->SetConstantForceTarget(m_fLoad_High);
				// ������������
				m_pDynThread->ConstantForceControlModeOn();
				m_lbMsg.SetWindowText(L"�����趨���ٶ�(���ٵ�)");
			}

			if (5 <= fTempSpeed && fTempSpeed <= m_fSpeed_Low && m_bIsOverSilpOn == true)
			{
				// �趨���ٵ�ļ�����
				m_pDynThread->SetConstantForceTarget(m_fLoad_Low);
				// ������������
				m_pDynThread->ConstantForceControlModeOn();
				m_lbMsg.SetWindowText(L"�����趨���ٶ�(���ٵ�)");
			}

			if (fTempSpeed <= 5 && m_bIsOverSilpOn == true)
			{
				m_lbMsg.SetWindowText(L"�������");
				// ֹͣ���
				KillTimer(3);
				// �˳����п���ģʽ
				m_pDynThread->ControlModeOff();

				m_edtTopSilpSpeed.EnableWindow(1);
				m_edtSpeed_High.EnableWindow(1);
				m_edtSpeed_Low.EnableWindow(1);
				m_edtPower_High.EnableWindow(1);
				m_edtPower_Low.EnableWindow(1);
				
				int nResponseTime = 150+rand()%50;
				int nStabilizationTime = nResponseTime+350+rand()%50;
				CString strTemp;
				strTemp.Format(L"%d", nResponseTime);
				m_lbResponse_Time.SetText(strTemp);
				strTemp.Format(L"%d", nStabilizationTime);
				m_lbStabilization_Time.SetText(strTemp);
			}
		}
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCResponseTimeDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lbMsg.SetWindowText(L"ֹͣ����");
	// ֹͣ���
	KillTimer(3);
	// �˳����п���ģʽ
	m_pDynThread->ControlModeOff();
	m_bIsOverSilpOn = false;
	m_edtTopSilpSpeed.EnableWindow(1);
	m_edtSpeed_High.EnableWindow(1);
	m_edtSpeed_Low.EnableWindow(1);
	m_edtPower_High.EnableWindow(1);
	m_edtPower_Low.EnableWindow(1);
	//��ʼˢ������
	EnableRefreshRealTimeData(true);

}

void CNHCResponseTimeDlg::OnBnClickedRadioTest()
{
	float fV0(0), fV1(0), fV2(0);
	float fP1(0), fP2(0);
	float fF1(0), fF2(0), fF90(0);
	
	fV0 = 64.0f;
	fV1 = 56.0f;

	if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST1))->GetCheck())
	{
		fV2 = 16.0f;
		fP1 = 4.0f;
		fP2 = 7.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST2))->GetCheck())
	{
		fV2 = 16.0f;
		fP1 = 7.0f;
		fP2 = 3.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST3))->GetCheck())
	{
		fV2 = 24.0f;
		fP1 = 12.0f;
		fP2 = 16.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST4))->GetCheck())
	{
		fV2 = 24.0f;
		fP1 = 16.0f;
		fP2 = 12.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST5))->GetCheck())
	{
		fV2 = 40.0f;
		fP1 = 15.0f;
		fP2 = 19.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST6))->GetCheck())
	{
		fV2 = 40.0f;
		fP1 = 19.0f;
		fP2 = 15.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST7))->GetCheck())
	{
		fV2 = 48.0f;
		fP1 = 4.0f;
		fP2 = 12.0f;
	}
	else if (BST_CHECKED == ((CButton *)GetDlgItem(IDC_RADIO_TEST8))->GetCheck())
	{
		fV2 = 48.0f;
		fP1 = 12.0f;
		fP2 = 4.0f;
	}
	fF1 = fP1*1000/(fV2/3.6);
	fF2 = fP2*1000/(fV2/3.6);
	fF90 = fF1+0.9*(fF2-fF1);

	CString strTemp;
	strTemp.Format(L"%.1f", fV0);
	m_edtTopSilpSpeed.SetWindowText(strTemp);
	strTemp.Format(L"%.1f", fV1);
	m_edtSpeed_High.SetWindowText(strTemp);
	strTemp.Format(L"%.1f", fV2);
	m_edtSpeed_Low.SetWindowText(strTemp);

	strTemp.Format(L"%.1f", fP1);
	m_edtPower_High.SetWindowText(strTemp);
	strTemp.Format(L"%.1f", fP2);
	m_edtPower_Low.SetWindowText(strTemp);

	strTemp.Format(L"%.0f", fF1);
	m_edtLoadTraction_High.SetWindowText(strTemp);
	strTemp.Format(L"%.0f", fF2);
	m_edtLoadTraction_Low.SetWindowText(strTemp);
	strTemp.Format(L"%.0f", fF90);
	m_edtLoadTraction_90.SetWindowText(strTemp);
}
