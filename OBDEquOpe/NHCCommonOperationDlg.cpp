// NHCCommonOperationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCCommonOperationDlg.h"

//#include "bitset"

// CNHCCommonOperationDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCCommonOperationDlg, CDialogZoom)

CNHCCommonOperationDlg::CNHCCommonOperationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCCommonOperationDlg::IDD, pParent)
	, m_nDlgFontSize(45)
{
	// �����⹦���߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		L"����");
}

CNHCCommonOperationDlg::~CNHCCommonOperationDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCCommonOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCCommonOperationDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCCommonOperationDlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCCommonOperationDlg::OnBnClickedButtonMotorOff)
	ON_BN_CLICKED(IDCANCEL, &CNHCCommonOperationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_STATE, &CNHCCommonOperationDlg::OnBnClickedButtonViewState)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHCCommonOperationDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CNHCCommonOperationDlg ��Ϣ�������

BOOL CNHCCommonOperationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCCommonOperationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCCommonOperationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbInfo.SetText(L"����ͨѶ...");
	m_lbInfo.SetTitleStyle();

	m_shdTitle.SetTitle(L"�⹦�����ò���");

	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(FALSE);
}

void CNHCCommonOperationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCCommonOperationDlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
	{
		// ��ȡ״̬����ͨѶ����
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus())
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("�⹦�����ò���"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(FALSE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_VIEW_STATE)->EnableWindow(FALSE);
	}
}

void CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pDynThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->GetVValue() > 0.05f)
		{
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ʱ�������������", L"��������", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"��������", MB_OK|MB_ICONERROR);
		return;
	}

	m_pDynThread->LiftBeamUp();

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	m_pDynThread->LiftBeamDown();

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonMotorOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pDynThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->GetVValue() > 0.05f)
		{
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"�ص��", MB_OK|MB_ICONERROR);
		return;
	}

	// ��������״̬
	//std::bitset<16> bs;
	dw = m_pDynThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pDynThread->IsTheStatusBitSignaled(0))
		{
			// �������Ѿ�����

			EndWaitCursor();
			MessageBox(L"�뽵�;�������,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
		else if (!(m_pDynThread->IsTheStatusBitSignaled(0)) && !(m_pDynThread->IsTheStatusBitSignaled(1)))
		{
			// ���������ڽ��»�����������(�����͵����źŶ�û�б�����)

			// ���ͽ�����������
			m_pDynThread->LiftBeamDown();
			EndWaitCursor();
			MessageBox(L"��Ⱦ�������ȫ���º�,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
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

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonMotorOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	m_pDynThread->MotorOff();

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CNHCCommonOperationDlg::OnBnClickedButtonViewState()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ��������״̬
	std::bitset<16> bs;
	if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bs))
	{
		EndWaitCursor();
		CString str(L"");
		str.Format(L"��߸�λ�ұߵ�λ:\n\n%S\n----------------\nFEDCBA9876543210\n", bs.to_string().c_str());
		MessageBox(str, L"�鿴״̬", MB_OK);
		return;
	}
	else
	{
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"�鿴״̬", MB_OK|MB_ICONERROR);
		return;
	}

	EndWaitCursor();
}

void CNHCCommonOperationDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}
