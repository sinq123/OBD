// NHCControlModeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCControlModeDlg.h"


// CNHCControlModeDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCControlModeDlg, CDialogZoom)

CNHCControlModeDlg::CNHCControlModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCControlModeDlg::IDD, pParent)
	,m_fNominalV(100.0f)
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
}

CNHCControlModeDlg::~CNHCControlModeDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCControlModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_P_VALUE, m_nePValue);
	DDX_Control(pDX, IDC_EDIT_F_VALUE, m_neFValue);
	DDX_Control(pDX, IDC_EDIT_V_VALUE, m_neVValue);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_F, m_lbF);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHCControlModeDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_POWER_ON, &CNHCControlModeDlg::OnBnClickedButtonConstantPowerOn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_FORCE_ON, &CNHCControlModeDlg::OnBnClickedButtonConstantForceOn)
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_VELOCITY_ON, &CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCControlModeDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCControlModeDlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCControlModeDlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCControlModeDlg::OnBnClickedButtonMotorOff)
	ON_BN_CLICKED(IDC_BUTTON_CONTROL_MODE_OFF, &CNHCControlModeDlg::OnBnClickedButtonControlModeOff)
	ON_BN_CLICKED(IDCANCEL, &CNHCControlModeDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_DYN_GET_REALTIME_DATA, OnGetRealTimeData)
	ON_BN_CLICKED(IDC_BUTTON_CONSTANT_VELOCITY_ON2, &CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn2)
END_MESSAGE_MAP()


// CNHCControlModeDlg ��Ϣ�������

BOOL CNHCControlModeDlg::OnInitDialog()
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

void CNHCControlModeDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCControlModeDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦������ģʽ");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);

	// ���ù��ʡ������ٶ���ʾ����
	m_lbP.SetLEDStyle();
	m_lbF.SetLEDStyle();
	m_lbV.SetLEDStyle();

	//// �༭��ֻ����������
	//m_edPValue.ModifyStyle(0, ES_NUMBER);
	//m_edFValue.ModifyStyle(0, ES_NUMBER);
	//m_edVValue.ModifyStyle(0, ES_NUMBER);

	// ���ñ�ǩ������ɫ
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
}

void CNHCControlModeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCControlModeDlg::OpenSerialPortAndTestCommunication(void)
{
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
			m_pDynThread->GetMotorMaxVelocity(&m_fNominalV);
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("�⹦������ģʽ"));
			GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_CONSTANT_POWER_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONSTANT_FORCE_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONSTANT_VELOCITY_ON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTROL_MODE_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
	}
}

LRESULT CNHCControlModeDlg::OnGetRealTimeData(WPARAM wParam, LPARAM lParam)
{
	if (DYN_WR_SUCCEED != (DWORD)wParam)
	{
		TRACE("ȡʵʱ����ʧ��(ͨѶ��ʱ)\n");
	}

	return 0;
}

void CNHCControlModeDlg::RefreshRealTimeData(void)
{
	// ˢ��������
	// ͬʱ����ͨѶ����
	// 500����ִ��һ��

	CString str(L"");
	//if (DYN_WR_SUCCEED == m_pDynThread->GetRealTimeData())
	m_pDynThread->GetRealTimeData_Asyn(m_hWnd);
	{
		// ͨѶ�ɹ�
		str.Format(L"%.2f", m_pDynThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.1f", m_pDynThread->GetFValue());
		GetDlgItem(IDC_STATIC_F)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pDynThread->GetVValue());
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

void CNHCControlModeDlg::EnableRefreshRealTimeData(bool bEnable)
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

void CNHCControlModeDlg::EnableBasicOperation(bool bEnable)
{
	if (bEnable)
	{
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
	}
}


void CNHCControlModeDlg::OnBnClickedButtonConstantPowerOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ���Ŀ�깦��ֵ
	CString str(L"");
	GetDlgItem(IDC_EDIT_P_VALUE)->GetWindowText(str);
	if (L"" == str)
	{
		MessageBoxW(L"�㹦��Ŀ��ֵ����Ϊ��", L"�㹦��", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_P_VALUE)->SetFocus();
		return;
	}
	float fPValue = (float)_wtof(str);
	if (fPValue<0.0f || fPValue>50.0f)
	{
		MessageBoxW(L"��������ȷ�ĺ㹦��Ŀ��ֵ(0~50kW)", L"�㹦��", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_P_VALUE)->SetFocus();
		return;
	}

	BeginWaitCursor();
	// ֹͣˢ������
	EnableRefreshRealTimeData(false);
	// ����Ŀ��ֵ
	if (DYN_WR_SUCCEED != m_pDynThread->SetConstantPowerTarget(fPValue))
	{
		EndWaitCursor();
		MessageBoxW(L"���ú㹦��Ŀ��ֵ��ʱ", L"�㹦��", MB_OK|MB_ICONERROR);
		return;
	}
	// �����㹦��
	//if (DYN_WR_SUCCEED != m_pDynThread->SendCtrlWord('7'))ConstantPowerControlModeOn
	if (DYN_WR_SUCCEED != m_pDynThread->ConstantPowerControlModeOn())
	{
		MessageBoxW(L"�����㹦�ʿ���ģʽ��ʱ", L"�㹦��", MB_OK|MB_ICONERROR);
	}
	else
	{
		str.Format(L"�������к㹦�ʿ���ģʽ:%.2fkW", fPValue);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(str);
	}
	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	// ��ֹ���л�������
	EnableBasicOperation(false);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonConstantForceOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ���Ŀ����ֵ
	CString str(L"");
	GetDlgItem(IDC_EDIT_F_VALUE)->GetWindowText(str);
	if (L"" == str)
	{
		MessageBoxW(L"����Ŀ��ֵ����Ϊ��", L"����", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_F_VALUE)->SetFocus();
		return;
	}
	float fFValue = (float)_wtof(str);
	if (fFValue<0.0f || fFValue>8000.0f)
	{
		MessageBoxW(L"��������ȷ�ĺ���Ŀ��ֵ(0~8000.0N)", L"����", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_F_VALUE)->SetFocus();
		return;
	}

	BeginWaitCursor();
	// ֹͣˢ������
	EnableRefreshRealTimeData(false);
	// ����Ŀ��ֵ
	if (DYN_WR_SUCCEED != m_pDynThread->SetConstantForceTarget(fFValue))
	{
		EndWaitCursor();
		MessageBoxW(L"���ú���Ŀ��ֵ��ʱ", L"����", MB_OK|MB_ICONERROR);
		return;
	}
	// ��������
	if (DYN_WR_SUCCEED != m_pDynThread->ConstantForceControlModeOn())
	{
		MessageBoxW(L"������������ģʽ��ʱ", L"����", MB_OK|MB_ICONERROR);
	}
	else
	{
		str.Format(L"�������к�������ģʽ:%.1fN", fFValue);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(str);
	}
	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	// ��ֹ���л�������
	EnableBasicOperation(false);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ���Ŀ���ٶ�ֵ
	CString str(L"");
	GetDlgItem(IDC_EDIT_V_VALUE)->GetWindowText(str);
	if (L"" == str)
	{
		MessageBoxW(L"���ٶ�Ŀ��ֵ����Ϊ��", L"���ٶ�", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE)->SetFocus();
		return;
	}
	float fVValue = (float)_wtof(str);
	if (fVValue<0.0f || fVValue>100.0f)
	{
		MessageBoxW(L"��������ȷ�ĺ��ٶ�Ŀ��ֵ(0~100.0km/h)", L"���ٶ�", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE)->SetFocus();
		return;
	}

	BeginWaitCursor();
	// ֹͣˢ������
	EnableRefreshRealTimeData(false);
	// ����Ŀ��ֵ
	if (DYN_WR_SUCCEED != m_pDynThread->SetConstantVelocityTarget(fVValue))
	{
		EndWaitCursor();
		MessageBoxW(L"���ú��ٶ�Ŀ��ֵ��ʱ", L"���ٶ�", MB_OK|MB_ICONERROR);
		return;
	}
	// �������ٶ�
	if (DYN_WR_SUCCEED != m_pDynThread->ConstantVelocityControlModeOn())
	{
		MessageBoxW(L"�������ٶȿ���ģʽ��ʱ", L"���ٶ�", MB_OK|MB_ICONERROR);
	}
	else
	{
		str.Format(L"�������к��ٶȿ���ģʽ:%.1fkm/h", fVValue);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(str);
	}
	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	// ��ֹ���л�������
	EnableBasicOperation(false);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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

	m_pDynThread->LiftBeamUp();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	m_pDynThread->LiftBeamDown();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonMotorOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
		m_pDynThread->LiftBeamDown();
		m_pDynThread->MotorOn();
	}
	else
	{
		MessageBox(L"�������ȡ��", L"�����", MB_OK|MB_ICONWARNING);
	}

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonMotorOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	m_pDynThread->MotorOff();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedButtonControlModeOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	m_pDynThread->ControlModeOff();
	///�ָ��⹦���������ٶ�
	m_pDynThread->SetMotorMaxVelocity(m_fNominalV);

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ֹͣ���п���ģʽ"));

	// ���Խ��л�������
	EnableBasicOperation(true);

	EndWaitCursor();
}

void CNHCControlModeDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ֹͣ���п���ģʽ
	m_pDynThread->ControlModeOff();

	///�ָ��⹦���������ٶ�
	m_pDynThread->SetMotorMaxVelocity(m_fNominalV);

	OnCancel();
}

void CNHCControlModeDlg::OnBnClickedButtonConstantVelocityOn2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nVValue(0);
	// Ŀ���ٶ�ֵ
	CString str(L"");
	EnableRefreshRealTimeData(false);
	GetDlgItem(IDC_EDIT_V_VALUE2)->GetWindowText(str);
	float fVValue = (float)_wtof(str);
	if (m_pDynThread->GetVValue() != 0.0f)
	{
		MessageBoxW(L"��Ͳ����ת��,���⹦����Ͳ��ȫֹͣ��������", L"���ٶ�", MB_OK|MB_ICONERROR);
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		return;
	}
	else
	{
		MessageBoxW(L"��Ͳ��ʼת��,��ȷ�ϲ⹦����Ͳ�Բ������˺����", L"��ȫ����", MB_OK|MB_ICONERROR);
	}
	if (L"" == str)
	{
		MessageBoxW(L"���ٶ�Ŀ��ֵ����Ϊ��", L"���ٶ�", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE2)->SetFocus();
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		return;
	}
	if (fVValue<0.0f || fVValue>100.0f)
	{
		MessageBoxW(L"��������ȷ�ĺ��ٶ�Ŀ��ֵ(0~100.0km/h)", L"���ٶ�", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_V_VALUE2)->SetFocus();
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		return;
	}
	else
	{
		// ����Ŀ��ֵ
		if (DYN_WR_SUCCEED != m_pDynThread->SetMotorMaxVelocity(fVValue))
		{
			EndWaitCursor();
			MessageBoxW(L"���ú��ٶ�Ŀ��ֵ��ʱ", L"���ٶ�", MB_OK|MB_ICONERROR);
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			return;
		}
	}
	BeginWaitCursor();
	// ��ʼˢ������
	EnableRefreshRealTimeData(true);
	///�������
	m_pDynThread->MotorOn();
}
