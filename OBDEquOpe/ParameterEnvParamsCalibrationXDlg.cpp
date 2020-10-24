// ParameterEnvParamsCalibrationXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ParameterEnvParamsCalibrationXDlg.h"


// CParameterEnvParamsCalibrationXDlg �Ի���

IMPLEMENT_DYNAMIC(CParameterEnvParamsCalibrationXDlg, CDialogZoom)

CParameterEnvParamsCalibrationXDlg::CParameterEnvParamsCalibrationXDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParameterEnvParamsCalibrationXDlg::IDD, pParent)
	, m_pParaThread(NULL)
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

	//m_pParaThread = (CParaThread*)AfxBeginThread(RUNTIME_CLASS(CParaThread));
	m_pParaThread = CNHEuqController::GetInstance().m_pParaThread;
}

CParameterEnvParamsCalibrationXDlg::~CParameterEnvParamsCalibrationXDlg()
{
	m_fontDlgFont.DeleteObject();
	
	//if (NULL != m_pParaThread)
	//{
	//	m_pParaThread->Close();
	//	m_pParaThread->QuitThread();
	//	WaitForSingleObject(m_pParaThread->m_hThread, 5000);
	//}
}

void CParameterEnvParamsCalibrationXDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CParameterEnvParamsCalibrationXDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ET_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonEtCal)
	ON_BN_CLICKED(IDC_BUTTON_RH_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonRhCal)
	ON_BN_CLICKED(IDC_BUTTON_AP_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonApCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_LOW_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonOtLowCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_HIGH_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonOtHighCal)
	ON_BN_CLICKED(IDC_BUTTON_ET_ZERO_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonEtZeroCal)
	ON_BN_CLICKED(IDC_BUTTON_RH_ZERO_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonRhZeroCal)
	ON_BN_CLICKED(IDC_BUTTON_AP_ZERO_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonApZeroCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_ZERO_CAL, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonOtZeroCal)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_CONFIG, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonDefaultConfig)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonQuit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CParameterEnvParamsCalibrationXDlg ��Ϣ�������

BOOL CParameterEnvParamsCalibrationXDlg::OnInitDialog()
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

void CParameterEnvParamsCalibrationXDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CParameterEnvParamsCalibrationXDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"���������ǻ�������������У׼");

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
	m_neETStd.AllowNegative(1);
	m_neRHStd.AllowNegative(FALSE);
	m_neRHStd.SetMaxDecimalPlaces(1);
	m_neAPStd.AllowNegative(FALSE);
	m_neAPStd.SetMaxDecimalPlaces(1);
	m_neOTStd.AllowNegative(FALSE);
	m_neOTStd.SetMaxDecimalPlaces(1);

	GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ET_ZERO_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RH_ZERO_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AP_ZERO_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OT_ZERO_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OT_LOW_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OT_HIGH_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonEtCal()
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
	DWORD dwResult = m_pParaThread->EnvironmentTemperatureSlopeCalibration(&bResult, fStdET);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"�����¶�У׼�ɹ�", L"�����¶�У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"�����¶�У׼ʧ��", L"�����¶�У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonEtZeroCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������// �����׼ֵ������
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
	DWORD dwResult = m_pParaThread->EnvironmentTemperatureZeroCalibration(&bResult, fStdET);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"�����¶���λУ׼�ɹ�", L"�����¶���λУ׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"�����¶���λУ׼ʧ��", L"�����¶���λУ׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonRhCal()
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
	DWORD dwResult = m_pParaThread->RelativeHumiditySlopeCalibration(&bResult, fStdRH);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"���ʪ��У׼�ɹ�", L"���ʪ��У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"���ʪ��У׼ʧ��", L"���ʪ��У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonRhZeroCal()
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
	DWORD dwResult = m_pParaThread->RelativeHumidityZeroCalibration(&bResult, fStdRH);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"���ʪ����λУ׼�ɹ�", L"���ʪ����λУ׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"���ʪ����λУ׼ʧ��", L"���ʪ����λУ׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonApCal()
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
	DWORD dwResult = m_pParaThread->AirPressureSlopeCalibration(&bResult, fStdAP);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"����ѹ��У׼�ɹ�", L"����ѹ��У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����ѹ��У׼ʧ��", L"����ѹ��У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonApZeroCal()
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
	DWORD dwResult = m_pParaThread->AirPressureZeroCalibration(&bResult, fStdAP);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"����ѹ����λУ׼�ɹ�", L"����ѹ����λУ׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����ѹ����λУ׼ʧ��", L"����ѹ����λУ׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonOtLowCal()
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
	DWORD dwResult = m_pParaThread->OilTemperatureColdSideSlopeCalibration(&bResult, fStdOT);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"����У׼�ɹ�", L"���µ͵�У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����У׼ʧ��", L"���µ͵�У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonOtHighCal()
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
	DWORD dwResult = m_pParaThread->OilTemperatureHotSideSlopeCalibration(&bResult, fStdOT);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"����У׼�ɹ�", L"���¸ߵ�У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����У׼ʧ��", L"���¸ߵ�У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonOtZeroCal()
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
	DWORD dwResult = m_pParaThread->OilTemperatureColdSideZeroCalibration(&bResult, fStdOT);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"����У׼�ɹ�", L"������λУ׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����У׼ʧ��", L"������λУ׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CParameterEnvParamsCalibrationXDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"ParaParams", L"Port", L"7"));
	// ��ȡ������ͨѶЭ������,Ĭ��Ϊ503
	CString strParaProtocol = si.GetString(L"ParaParams", L"Protocol", L"NHP");
	// ת��Ϊ��д
	strParaProtocol.MakeUpper();
	//if (PARA_OPEN_SUCCEED == m_pParaThread->Open(bPort, CParaThread::IM_NANHUA_PARA))
	if (m_pParaThread->IsOpen())
	{
		if (PARA_WR_SUCCEED == m_pParaThread->GetEntireData())
		{
			// ����
			m_lbInfo.SetText(_T("���������ǻ�������������У׼"));
			// λ��ʵʱͨѶ����
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ET_ZERO_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RH_ZERO_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AP_ZERO_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_OT_ZERO_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_OT_LOW_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_OT_HIGH_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

			// ��ʼˢ������
			EnableRefreshData(true);
		}
		else
		{
			m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ET_ZERO_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RH_ZERO_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AP_ZERO_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OT_ZERO_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OT_LOW_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OT_HIGH_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ET_ZERO_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RH_ZERO_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AP_ZERO_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_ZERO_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_LOW_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_HIGH_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CParameterEnvParamsCalibrationXDlg::OnTimer(UINT_PTR nIDEvent)
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

void CParameterEnvParamsCalibrationXDlg::RefreshData(void)
{
	// ˢ������
	float fET(0.0f);
	float fRH(0.0f);
	float fAP(0.0f);
	float fOT(0.0f);
	USHORT usRev(0);

	m_pParaThread->GetEntireData(&fET, &fRH, &fAP, &fOT, &usRev);

	CString str(L"");
	
	str.Format(L"%.1f", fET);
	m_edETMea.SetWindowTextW(str);

	str.Format(L"%.1f", fRH);
	m_edRHMea.SetWindowTextW(str);

	str.Format(L"%.1f", fAP);
	m_edAPMea.SetWindowTextW(str);

	str.Format(L"%.1f", fOT);
	m_edOTMea.SetWindowTextW(str);
}

void CParameterEnvParamsCalibrationXDlg::EnableRefreshData(bool bEnable)
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

void CParameterEnvParamsCalibrationXDlg::OnBnClickedButtonDefaultConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IDOK != MessageBoxW(L"ȷ�����ڻָ�����������У׼����Ĭ�����ã�", L"����������У׼����", MB_OKCANCEL|MB_ICONWARNING))
	{
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pParaThread->RecoverCalibrationParameters(&bResult, CPara::AllParameters);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"�ָ�����������У׼����Ĭ�����óɹ�", L"����������У׼����", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"�ָ�����������У׼����Ĭ������ʧ��", L"����������У׼����", MB_OK|MB_ICONINFORMATION);
	}
}


