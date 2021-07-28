// ParameterEnvParamsCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ParameterEnvParamsCalibrationDlg.h"
#include "UpTachometerDlg.h"

// CParameterEnvParamsCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(CParameterEnvParamsCalibrationDlg, CDialogZoom)

CParameterEnvParamsCalibrationDlg::CParameterEnvParamsCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParameterEnvParamsCalibrationDlg::IDD, pParent)
	, m_pParaThread(NULL)
	, m_bRefreshDataSign(true)
	, m_pfProcessCtrl(NULL)
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

CParameterEnvParamsCalibrationDlg::~CParameterEnvParamsCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();
	
	//if (NULL != m_pParaThread)
	//{
	//	m_pParaThread->Close();
	//	m_pParaThread->QuitThread();
	//	WaitForSingleObject(m_pParaThread->m_hThread, 5000);
	//}
}

void CParameterEnvParamsCalibrationDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CParameterEnvParamsCalibrationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ET_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonEtCal)
	ON_BN_CLICKED(IDC_BUTTON_RH_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonRhCal)
	ON_BN_CLICKED(IDC_BUTTON_AP_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonApCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonOtCal)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_CONFIG, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonDefaultConfig)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_UP_ENV_PARA, &CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpEnvPara)
	ON_BN_CLICKED(IDC_BTN_UP_TACHOMETER, &CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpTachometer)
END_MESSAGE_MAP()


// CParameterEnvParamsCalibrationDlg ��Ϣ�������

BOOL CParameterEnvParamsCalibrationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_odtbegin = COleDateTime::GetCurrentTime();
	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CParameterEnvParamsCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CParameterEnvParamsCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"���������ǻ�������У׼");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// ����ֵ�༭��
	m_edETMea.SetReadOnly(FALSE);
	m_edRHMea.SetReadOnly(FALSE);
	m_edAPMea.SetReadOnly(FALSE);
	m_edOTMea.SetReadOnly(FALSE);

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
	GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonEtCal()
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
		MessageBox(L"�����¶ȱ�׼�ɹ�", L"�����¶�У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"�����¶ȱ�׼ʧ��", L"�����¶�У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonRhCal()
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
		MessageBox(L"���ʪ�ȱ�׼�ɹ�", L"���ʪ��У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"���ʪ�ȱ�׼ʧ��", L"���ʪ��У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonApCal()
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
		MessageBox(L"����ѹ����׼�ɹ�", L"����ѹ��У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"����ѹ����׼ʧ��", L"����ѹ��У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonOtCal()
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
		MessageBox(L"���±�׼�ɹ�", L"����У׼", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"���±�׼ʧ��", L"����У׼", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CParameterEnvParamsCalibrationDlg::OpenSerialPortAndTestCommunication(void)
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
			m_lbInfo.SetText(_T("���������ǻ�������У׼"));
			// λ��ʵʱͨѶ����
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DEFAULT_CONFIG)->EnableWindow(TRUE);
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
			GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DEFAULT_CONFIG)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEFAULT_CONFIG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CParameterEnvParamsCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (nIDEvent)
	{
	case 1:
		{
			//RefreshData();
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

void CParameterEnvParamsCalibrationDlg::RefreshData(void)
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

void CParameterEnvParamsCalibrationDlg::EnableRefreshData(bool bEnable)
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

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonDefaultConfig()
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

void CParameterEnvParamsCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpEnvPara()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str[6];
	for (int i = 0; i < 6; i++)
	{
		str[i] = L"";
	}

	//��ȡȫ���ǲ���
	// �¶ȣ�������
	m_edETMea.GetWindowTextW(str[0]);
	m_neETStd.GetWindowTextW(str[1]);
	// ʪ��
	m_edRHMea.GetWindowTextW(str[2]);
	m_neRHStd.GetWindowTextW(str[3]);
	// ����ѹ
	m_edAPMea.GetWindowTextW(str[4]);
	m_neAPStd.GetWindowTextW(str[5]);

	bool bEmp(false);
	for (int i = 0; i < 6; i++)
	{
		if (str[i].IsEmpty())
		{
			bEmp = true;
			break;
		}
	}
	if (bEmp)
	{
		MessageBox(L"���������п�ֵ");
		return;
	}

	// ��ɾ��֮ǰ���ĵ�
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		DeleteFile(wchPath);
	}
	
	CString strTemp;
	CString strResult;

	strResult.AppendFormat(L"\r\n[EnvParams]");

	// ��ʼʱ��
	strTemp.Format(L"\r\nStartTime=%s", m_odtbegin.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strResult += strTemp;

	// ����ʱ��
	strTemp.Format(L"\r\nEndTime=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strResult += strTemp;

	// �¶Ȳ���
	strTemp.Format(L"\r\nETMea=%s", str[0]);
	strResult += strTemp;
	// �¶ȱ�׼
	strTemp.Format(L"\r\nETStd=%s", str[1]);
	strResult += strTemp;
	// ʪ�Ȳ���
	strTemp.Format(L"\r\nRHMea=%s", str[2]);
	strResult += strTemp;
	// ʪ�ȱ�׼
	strTemp.Format(L"\r\nRHStd=%s", str[3]);
	strResult += strTemp;
	// ����ѹ����
	strTemp.Format(L"\r\nAPMea=%s", str[4]);
	strResult += strTemp;
	// ����ѹ��׼
	strTemp.Format(L"\r\nAPStd=%s", str[5]);
	strResult += strTemp;

	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CStdioFileEx::modeWriteUnicode);
	// ׷��д��
	sfe.SeekToEnd();
	sfe.WriteString(strResult);
	sfe.Close();

	Sleep(200);
	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		m_pfProcessCtrl(0x01);
	}
}


void CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpTachometer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CUpTachometerDlg dlg;

	if (IDOK == dlg.DoModal())
	{
		Sleep(200);
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			m_pfProcessCtrl(0x02);
		}
	}
}
