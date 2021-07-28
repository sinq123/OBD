
// ATDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AT.h"
#include "ATDlg.h"
#include "afxdialogex.h"

#include "OBDTestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CATDlg �Ի���




CATDlg::CATDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CATDlg::IDD, pParent)
	, m_pConnection(NULL)
	, m_siPrepareRegionIndex(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		L"����");

	// ��ȡexe(dll)�ļ�����·��
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", m_wchPathOfNHClient))
	{
	}
}

CATDlg::~CATDlg()
{
	// ɾ������
	m_fontDlgFont.DeleteObject();

	if (m_pConnection != NULL)
	{
		m_pConnection = NULL;
	}
}

void CATDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, IDC_CB_FILTER_TEST_TYPE, m_cbFilterTestType);
	DDX_Control(pDX, IDC_ED_FILTER, m_edFilter);
	//DDX_Control(pDX, IDC_ST_VEHICLE_INFO, m_stVehicleInfo);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_ST_VEHICLE_LIST, m_lbVehicleList);
	DDX_Control(pDX, IDC_LB_VEHICLE_INFO, m_lbVehicleInfo);
	DDX_Control(pDX, IDC_STATIC_DYN_INFO, m_lbDynInfo);
	DDX_Control(pDX, IDC_COMBO_DRIVER, m_cbDriver);
	DDX_Control(pDX, IDC_CB_TEST_TYPE, m_cbTestType);
	DDX_Control(pDX, IDC_ED_PLATE_NUMBER, m_edPlateNumber);
	DDX_Control(pDX, IDC_ED_OWNER, m_edOwner);
	DDX_Control(pDX, IDC_CB_PLATE_TYPE, m_cbPlateType);
	DDX_Control(pDX, IDC_ED_VIN, m_edVIN);
	DDX_Control(pDX, IDC_ED_MODEL, m_edModel);
	DDX_Control(pDX, IDC_DTP_PRODUCTION_DATE, m_dtcProductionDate);
	DDX_Control(pDX, IDC_DTP_REGISTRATION_DATE, m_dtcRegistrationDate);
	DDX_Control(pDX, IDC_CB_DRIVE_TYPE, m_cbDriveType);
	DDX_Control(pDX, IDC_ED_UNLADEN_MASS, m_edUnladenMass);
	DDX_Control(pDX, IDC_ED_MAXIMUM_TOTAL_MASS, m_edMaximumTotalMass);
	DDX_Control(pDX, IDC_ED_IS_RETEST, m_edIsRetest);
	DDX_Control(pDX, IDC_CB_FUEL_TYPE, m_cbFuelType);
	DDX_Control(pDX, IDC_ED_RATED_POWER, m_edRatedPower);
	DDX_Control(pDX, IDC_ED_RATED_REV, m_edRatedRev);
	DDX_Control(pDX, IDC_CB_HAS_CATALYTIC_CONVERTER, m_cbHasCatalyticConverter);
	DDX_Control(pDX, IDC_CB_AIRINTAKE_MODE, m_cbAirIntakeMode);
	DDX_Control(pDX, IDC_CB_OILSUPPLY_MODE, m_cbOilSupplyMode);

	DDX_Control(pDX, IDC_ED_ITEM_APP, m_edItemApp);
	DDX_Control(pDX, IDC_ED_ITEM_OBD, m_edItemOBD);
	DDX_Control(pDX, IDC_ED_ITEM_EMI, m_edItemEmi);

	DDX_Control(pDX, IDC_ED_LAMBDA_MIN, m_edLambdaMin);
	DDX_Control(pDX, IDC_ED_LAMBDA_MAX, m_edLambdaMax);

	DDX_Control(pDX, IDC_EDIT_ET, m_edET);
	DDX_Control(pDX, IDC_EDIT_RH, m_edRH);
	DDX_Control(pDX, IDC_EDIT_AP, m_edAP);
	DDX_Control(pDX, IDC_EDIT_OT, m_edOT);
	DDX_Control(pDX, IDC_EDIT_RPM, m_edRPM);
	DDX_Control(pDX, IDC_LB_EPARAM_INFO, m_lbEParamInfo);
	DDX_Control(pDX, IDC_ED_EPPORTINFO, m_lbEPPortInfo);

	DDX_CBString(pDX, IDC_COMBO_DRIVER, m_strDriver);

	DDX_Control(pDX, IDC_CHK_UPOBDREALTIME, m_chkUpOBDRealTime);
}

BEGIN_MESSAGE_MAP(CATDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CATDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CATDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CATDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_UP, &CATDlg::OnBnClickedButtonLiftBeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_DOWN, &CATDlg::OnBnClickedButtonLiftBeamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CATDlg::OnNMDblclkLstVehicle)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CATDlg ��Ϣ�������

BOOL CATDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// �������ݿ�
	InitDatabase();

	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ
	ShowWindow(SW_MAXIMIZE);
	// ��󻯺���ܼ��ش���
	InitEquipmentPrepareRegion();

	OperationHint(L"������б�");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CATDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogZoom::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CATDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CATDlg::OperationHint(const CStringW& strOH)
{
	//if (bGet)
	//{
	//	m_lbDynInfo.GetWindowTextW(strOH);
	//}
	//else
	{
		m_lbDynInfo.SetWindowTextW(strOH);
	}
}


void CATDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strFilter, strTestType;
	m_cbFilterTestType.GetWindowText(strTestType);
	m_edFilter.GetWindowText(strFilter);

	if (L"��̬" == strTestType)
	{
		UpdateVehicleList(1, strFilter);
	}
	else if (L"����˲̬" == strTestType)
	{
		UpdateVehicleList(2, strFilter);
	}
	else if (L"���ؼ���" == strTestType)
	{
		UpdateVehicleList(3, strFilter);
	}
	else if (L"˫����" == strTestType)
	{
		UpdateVehicleList(4, strFilter);
	}
	else if (L"��͸�����ɼ���" == strTestType)
	{
		UpdateVehicleList(5, strFilter);
	}
	else if (L"��ֽʽ���ɼ���" == strTestType)
	{
		UpdateVehicleList(6, strFilter);
	}
	else if (L"ũ�ó����ɼ���" == strTestType)
	{
		UpdateVehicleList(7, strFilter);
	}
	else if (L"Ħ�г�����" == strTestType)
	{
		UpdateVehicleList(8, strFilter);
	}
	else if (L"Ħ�г�˫����" == strTestType)
	{
		UpdateVehicleList(9, strFilter);
	}
	else
	{
		UpdateVehicleList(0, strFilter);
	}
}


void CATDlg::OnBnClickedBtnNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_cbDriver.GetWindowTextW(m_strDriver);
	if (L"" == m_strDriver)
	{
		AfxMessageBox(L"��ѡ������Ա");
		return;
	}

	COBDTestDlg dlg;
	CString str;

	dlg.m_bIsOBDRealTime = m_chkUpOBDRealTime.GetCheck() == 1;
	m_edPlateNumber.GetWindowTextW(str);
	dlg.m_strPlateNumber = str.GetString();
	m_edVIN.GetWindowTextW(str);
	dlg.m_strVIN = str.GetString();
	m_cbFuelType.GetWindowTextW(str);
	dlg.m_strFuelType = str.GetString();
	dlg.m_strRunningNumber = m_strCurrentRunningNumber.GetString();
	dlg.m_strOperator = m_strDriver;
	dlg.m_pConnection = m_pConnection;
	INT_PTR nDoModel = dlg.DoModal();
	if (IDOK == nDoModel)
	{

	}
	else if (IDCANCEL == nDoModel) 
	{

	}
	// ���غ������Ϊ��
	m_strCurrentRunningNumber = L"";

	const int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if (nIndex != -1)
	{
		m_lstVehicle.SetItemState(nIndex, 0, -1);
	}
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	OperationHint(L"");
}


void CATDlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}


void CATDlg::OnBnClickedButtonLiftBeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CATDlg::OnBnClickedButtonLiftBeamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CATDlg::OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SelectVehicle();

	*pResult = 0;
}


void CATDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

	m_fontVehicleLCFont.DeleteObject();
}


void CATDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogZoom::OnTimer(nIDEvent);
}

// �������ݿ�
DWORD CATDlg::InitDatabase(void)
{
	// ����ֵ
	DWORD dwReturn(0x00);

	// �������ļ�,��ȡ���ݿ�������Ϣ
	// ��ȡexe(dll)�ļ�����·��
	wchar_t wchPathNHModeServerDB[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHModeServerDB.ini", wchPathNHModeServerDB))
	{
		return 0x01;
	}

	// ��ȡ���ݿ�������Ϣ
	//  ��FDInterface.ini
	CSimpleIni sio(wchPathNHModeServerDB);

	CString strSource, strCatalog, strUserID, strPassword;
	strSource = sio.GetString(L"Database", L"Source", L"");// ����������
	strCatalog = sio.GetString(L"Database", L"Catalog", L"");// ���ݿ�����
	strUserID = sio.GetString(L"Database", L"UserID", L"");// �û���
	strPassword = sio.GetString(L"Database", L"Password", L"");// ����

	if (0x00 != CNHSQLServerDBO::OpenDB(m_pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		dwReturn = 0x01;
		OperationHint(L"�������ݿ�ʧ�ܴ���");
	}

	return dwReturn;
}

void CATDlg::InitCtrls(void)
{
	// ��������
	SetDlgFont();

	// ������
	m_lbHead.SetTitle(L"�豸׼���ͳ���ѡ��");

	// ��ȡ���������
	GetIniNHClient(&m_sIniNHClient);

	// �⹦����Ϣ��ʾ
	m_lbDynInfo.SetTextColor(RGB(255, 0, 0));
	//m_lbDynInfo.AutoFitFont();
	m_lbDynInfo.SetFontSize(40);
	m_lbDynInfo.SetFontName(L"����");

	// �Ƿ���Ҫ��ʾ������������ť
	if (L'1' == m_sIniNHClient.wchSystemType[0]
	|| L'1' == m_sIniNHClient.wchSystemType[1]
	|| L'1' == m_sIniNHClient.wchSystemType[2])
	{
		GetDlgItem(IDC_BUTTON_LIFT_BEAM_UP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LIFT_BEAM_DOWN)->ShowWindow(SW_SHOW);
		m_lbDynInfo.ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_LIFT_BEAM_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LIFT_BEAM_DOWN)->ShowWindow(SW_HIDE);
		m_lbDynInfo.ShowWindow(SW_HIDE);
	}

	InitVehicleList();
	InitVehicleInfoRegion(FALSE);
	InitTestTypeCombobox();
	InitDriverCombobox();
	InitComboboxPlateType();
	InitComboboxDriveType();
	InitComboboxFuelType();
	InitComboboxOilSupplyMode();
	InitComboboxAirIntakeMode();

	GetDlgItem(IDC_BUTTON_LIFT_BEAM_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LIFT_BEAM_DOWN)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);

	m_chkUpOBDRealTime.EnableWindow(FALSE);

	((CButton *)GetDlgItem(IDC_CHE_NOW_TIME))->SetCheck(TRUE);
	// ͨ���궨�壬��Щƽ̨��֧���ϴ���������
#ifdef Up_OBD_RealTime
	m_chkUpOBDRealTime.ShowWindow(SW_SHOW);
	m_chkUpOBDRealTime.EnableWindow(TRUE);
	m_chkUpOBDRealTime.SetCheck(TRUE);
#endif

}

void CATDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CATDlg::InitVehicleList(void)
{
	// ���쳵�б�
	m_lbVehicleList.SetTitle(L"���쳵�б�");

	int nFontHeight(80);

	m_fontVehicleLCFont.CreateFont(nFontHeight,
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

	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);

	m_lstVehicle.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstVehicle.SetFont(&m_fontVehicleLCFont);
	m_lstVehicle.SetTextColor(RGB(0, 255, 0));
	m_lstVehicle.SetBkColor(RGB(0, 0, 0));
	m_lstVehicle.SetTextBkColor(RGB(0, 0, 0));

	m_lstVehicle.InsertColumn(0, L"���",		0,	0*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(1, L"���ƺ���",	0,	320*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(2, L"��������",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(3, L"�������",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(4, L"��ˮ��",		0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(5, L"��/����",	0,	120*nSM_CXSCREEN/1280);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CATDlg::InitVehicleInfoRegion(BOOL Enable)
{
	m_lbVehicleInfo.SetTitle(L"������Ϣ");
	m_lbEParamInfo.SetTitle(L"��������");

	m_cbTestType.EnableWindow(Enable);
	m_edPlateNumber.EnableWindow(Enable);
	m_edOwner.EnableWindow(Enable);
	m_cbPlateType.EnableWindow(Enable);
	m_edVIN.EnableWindow(Enable);
	m_edModel.EnableWindow(Enable);
	m_dtcProductionDate.EnableWindow(Enable);
	m_cbDriveType.EnableWindow(Enable);
	m_edUnladenMass.EnableWindow(Enable);
	m_edMaximumTotalMass.EnableWindow(Enable);
	m_cbFuelType.EnableWindow(Enable);
	m_edRatedPower.EnableWindow(Enable);
	m_edRatedRev.EnableWindow(Enable);
	m_cbOilSupplyMode.EnableWindow(Enable);
	m_cbAirIntakeMode.EnableWindow(Enable);
	m_cbHasCatalyticConverter.EnableWindow(Enable);
}

void CATDlg::InitTestTypeCombobox(void)
{
	// ��ȡϵͳ����,���ؼ������
	if (L'1' == m_sIniNHClient.wchSystemType[0])
	{
		m_cbFilterTestType.AddString(L"��̬");
		m_cbTestType.AddString(L"��̬");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[1])
	{
		m_cbFilterTestType.AddString(L"����˲̬");
		m_cbTestType.AddString(L"����˲̬");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[2])
	{
		m_cbFilterTestType.AddString(L"���ؼ���");
		m_cbTestType.AddString(L"���ؼ���");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[3])
	{
		m_cbFilterTestType.AddString(L"˫����");
		m_cbTestType.AddString(L"˫����");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[4])
	{
		m_cbFilterTestType.AddString(L"��͸�����ɼ���");
		m_cbTestType.AddString(L"��͸�����ɼ���");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[5])
	{
		m_cbFilterTestType.AddString(L"��ֽʽ���ɼ���");
		m_cbTestType.AddString(L"��ֽʽ���ɼ���");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[6])
	{
		m_cbFilterTestType.AddString(L"ũ�ó����ɼ���");
		m_cbTestType.AddString(L"ũ�ó����ɼ���");
	} 
	if (L'1' == m_sIniNHClient.wchSystemType[7])
	{
		m_cbFilterTestType.AddString(L"Ħ�г�����");
		m_cbTestType.AddString(L"Ħ�г�����");
	} 
	if (L'1' == m_sIniNHClient.wchSystemType[8])
	{
		m_cbFilterTestType.AddString(L"Ħ�г�˫����");
		m_cbTestType.AddString(L"Ħ�г�˫����");
	} 
	m_cbFilterTestType.AddString(L"");
}

void CATDlg::InitDriverCombobox(void)
{
	CString strSql = L"select * from UserInfo where Role = 6";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"Name");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_cbDriver.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_cbDriver.AddString(L"");
				}
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::InitComboboxPlateType(void)
{
	CString strSql = L"select * from ValueOfPlateType order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"PlateType");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_cbPlateType.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_cbPlateType.AddString(L"");
				}
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::InitComboboxDriveType(void)
{
	CString strSql = L"select * from ValueOfDriveType order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"DriveType");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_cbDriveType.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_cbDriveType.AddString(L"");
				}
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::InitComboboxFuelType(void)
{
	CString strSql = L"select * from ValueOfFuelType order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"FuelType");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_cbFuelType.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_cbFuelType.AddString(L"");
				}
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::InitComboboxOilSupplyMode(void)
{
	CString strSql = L"select * from ValueOfOilSupplyMode order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"OilSupplyMode");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_cbOilSupplyMode.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_cbOilSupplyMode.AddString(L"");
				}
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::InitComboboxAirIntakeMode(void)
{
	CString strSql = L"select * from ValueOfAirIntakeMode order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"AirIntakeMode");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_cbAirIntakeMode.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_cbAirIntakeMode.AddString(L"");
				}
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::InitEquipmentPrepareRegion(void)
{
	// ��ȡϵͳ����,���ؼ������
	if (L'1' == m_sIniNHClient.wchSystemType[0]
	|| L'1' == m_sIniNHClient.wchSystemType[1]
	|| L'1' == m_sIniNHClient.wchSystemType[3]
	|| L'1' == m_sIniNHClient.wchSystemType[7])
	{
		LoadGasolineVehicleEquipmentPrepare();
	}

	if (L'1' == m_sIniNHClient.wchSystemType[2]
	|| L'1' == m_sIniNHClient.wchSystemType[4]
	|| L'1' == m_sIniNHClient.wchSystemType[5]
	|| L'1' == m_sIniNHClient.wchSystemType[6])
	{
		LoadDieselVehicleEquipmentPrepare();
	}
}

void CATDlg::LoadGasolineVehicleEquipmentPrepare(void)
{
	m_cGasolineVehicleEquipmentPrepareNDlg.Create(CGasolineVehicleEquipmentPrepareNDlg::IDD, this);
	CRect rt;
	switch(m_siPrepareRegionIndex)
	{
	case 0:
		{
			GetDlgItem(IDC_ST_PREPARE_RECT_0)->GetWindowRect(rt);
			m_siPrepareRegionIndex++;
		}
		break;
	case 1:
		{
			GetDlgItem(IDC_ST_PREPARE_RECT_1)->GetWindowRect(rt);
			m_siPrepareRegionIndex++;
		}
		break;

	default:
		break;
	}
	m_cGasolineVehicleEquipmentPrepareNDlg.MoveWindow(rt);
	m_cGasolineVehicleEquipmentPrepareNDlg.ShowWindow(SW_SHOW);
}

void CATDlg::LoadDieselVehicleEquipmentPrepare(void)
{
	m_cDieselVehicleEquipmentPrepareDlg.Create(CDieselVehicleEquipmentPrepareDlg::IDD, this);
	//m_cDieselVehicleEquipmentPrepareDlg.SetMessageBackWnd(m_hWnd);
	CRect rt;
	switch(m_siPrepareRegionIndex)
	{
	case 0:
		{
			GetDlgItem(IDC_ST_PREPARE_RECT_0)->GetWindowRect(rt);
			m_siPrepareRegionIndex++;
		}
		break;
	case 1:
		{
			GetDlgItem(IDC_ST_PREPARE_RECT_1)->GetWindowRect(rt);
			m_siPrepareRegionIndex++;
		}
		break;
	default:
		break;
	}
	m_cDieselVehicleEquipmentPrepareDlg.MoveWindow(rt);
	m_cDieselVehicleEquipmentPrepareDlg.ShowWindow(SW_SHOW);
}

void CATDlg::UpdateVehicleList(short const siTestType/* = 0*/, CString strFilter/* = L""*/)
{
	// ɾ���б�����
	m_lstVehicle.DeleteAllItems();
	CString strSql;
	// �ϻ���ȡ���ݿ�
	strSql.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.TestState, TestLog.BillDate, \
		TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
	strSql.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
	strSql.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1') ");
	//strSql.AppendFormat(L" and (TestLog.TestState = '0' or TestLog.TestState= '5') ");

	if (((CButton *)GetDlgItem(IDC_CHE_NOW_TIME))->GetCheck() == 1)
	{
		strSql.AppendFormat(L" and (TestLog.BillDate between '%s 00:00:00' and '%s 23:59:59')) ", 
			COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"),
			COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	}

	// ����ɸѡ
	if (!strFilter.IsEmpty())
	{
		strSql.AppendFormat(L" and TestLog.PlateNumber like '%%%s%%'", strFilter);
	}
	// ��ⷽ����
	switch(siTestType)
	{
	case 0:
		{
			CStringW strSqlTestType(L" and (");
			CStringW str(L"");
			bool b(true);
			if (L'1' == m_sIniNHClient.wchSystemType[0])
			{
				if (b)
				{
					str = L"TestType = '1'";
					b = false;
				}
				else
				{
					str = L"or TestType = '1'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[1])
			{
				if (b)
				{
					str = L"TestType = '2'";
					b = false;
				}
				else
				{
					str = L"or TestType = '2'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[2])
			{
				if (b)
				{
					str = L"TestType = '3'";
					b = false;
				}
				else
				{
					str = L"or TestType = '3'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[3])
			{
				if (b)
				{
					str = L"TestType = '4'";
					b = false;
				}
				else
				{
					str = L"or TestType = '4'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[4])
			{
				if (b)
				{
					str = L"TestType = '5'";
					b = false;
				}
				else
				{
					str = L"or TestType = '5'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[5])
			{
				if (b)
				{
					str = L"TestType = '6'";
					b = false;
				}
				else
				{
					str = L"or TestType = '6'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[6])
			{
				if (b)
				{
					str = L"TestType = '7'";
					b = false;
				}
				else
				{
					str = L"or TestType = '7'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[7])
			{
				if (b)
				{
					str = L"TestType = '8'";
					b = false;
				}
				else
				{
					str = L"or TestType = '8'";
				}
				strSqlTestType += str;
			}
			if (L'1' == m_sIniNHClient.wchSystemType[8])
			{
				if (b)
				{
					str = L"TestType = '9'";
					b = false;
				}
				else
				{
					str = L"or TestType = '9'";
				}
				strSqlTestType += str;
			}

			strSqlTestType += L" )";
			strSql.AppendFormat(L" %s", strSqlTestType);
		}
		break;
	default:
		{
			strSql.AppendFormat(L" and TestLog.TestType = '%d'", siTestType);
		}
		break;
	}

	DWORD dwDBORet(0x00);
	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{ // �򿪲�ѯ�ɹ�

		try
		{
			int i(0);
			while(!pRecordset->adoEOF)
			{
				int n(0);
				CString str;
				_variant_t v;

				// ���
				str.Format(L"%d", i+1);
				//int nItemIndex = m_lstVehicle.InsertItem(LVIF_TEXT | LVIF_PARAM,i, str, 0, 0, 0, 0);            
				int nItemIndex = m_lstVehicle.InsertItem(i, str);

				// ���ƺ���
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateNumber"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					str =  (LPCTSTR)(_bstr_t)v;
				}
				else
				{
					str = L"";
				}
				m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);

				// ��������
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateType"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					str =  (LPCTSTR)(_bstr_t)v;
				}
				else
				{
					str = L"";
				}
				m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);

				// �������
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("TestType"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					n =  v;

					if (6 == n)
					{
						// FSUNHT����FSUYD(begin)
						CSimpleIni si(m_wchPathOfNHClient);
						CStringW str = si.GetString(L"FSUYDParams", L"UseFSUNHTInsteadOfFSUYD", L"0");
						if (L"1" == str)
						{
							n = 5;
						}
						// FSUNHT����FSUYD(end)
					}
				}
				else
				{
					n = 0;
				}
				switch(n)
				{
				case 1:
					{
						str = L"��̬";
					}
					break;
				case 2:
					{
						str = L"����˲̬";
					}
					break;
				case 3:
					{
						str = L"���ؼ���";
					}
					break;
				case 4:
					{
						str = L"˫����";
					}
					break;
				case 5:
					{
						str = L"��͸�����ɼ���";
					}
					break;
				case 6:
					{
						str = L"��ֽʽ���ɼ���";
					}
					break;
				case 7:
					{
						str = L"ũ�ó����ɼ���";
					}
					break;
				case 8:
					{
						str = L"Ħ�г�����";
					}
					break;
				case 9:
					{
						str = L"Ħ�г�˫����";
					}
					break;
				default:
					{
						str = L"��������";
					}
					break;
				}
				m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);

				// ��ˮ��
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("RunningNumber"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					str =  (LPCTSTR)(_bstr_t)v;
				}
				else
				{
					str = L"";
				}
				m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);

				// �Ƿ񸴼�
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("IsRetest"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					bool b =  (bool)v;
					if (b)
					{
						str = L"����";
					}
					else
					{
						str = L"����";
					}
				}
				else
				{
					str = L"";
				}
				m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

				i++;
				pRecordset->MoveNext();
			}
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
			dwDBORet = 0xFFFFFFFF;

			OperationHint(L"�����б����");
		}
	}

	if (dwDBORet != 0xFFFFFFFF)
	{
		OperationHint(L"��˫������");
	}

	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::SelectVehicle(void)
{
	GetVehicleInfo();
}

void CATDlg::GetVehicleInfo(void)
{
	BeginWaitCursor();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(nIndex!=-1)
	{
		CString strRunningNumber,strPlateNumber,strPlateType;

		strPlateNumber = m_lstVehicle.GetItemText(nIndex,1);
		strPlateType = m_lstVehicle.GetItemText(nIndex,2);
		m_strCurrentTestType = m_lstVehicle.GetItemText(nIndex,3);
		strRunningNumber = m_lstVehicle.GetItemText(nIndex,4);

		m_strCurrentRunningNumber = strRunningNumber;

		// ��ȡ������Ϣ
		wchar_t wchSql[SQLMAX];
		TESTLOG sTestLog;
		wsprintf(wchSql, L"select * from TestLog where RunningNumber = '%s'", m_strCurrentRunningNumber);
		GetDboTestLog(wchSql, &sTestLog);

		// ��ȡ������Ϣ
		CString strSql;
		strSql.Format(L"select * from VehicleInfo where PlateNumber = '%s' and PlateType = '%s'", strPlateNumber, strPlateType);
		VEHICLEINFO SVehicleInfo;
		GetDboVehicleInfo(strSql, &SVehicleInfo);

		CString str(L"");
		m_cbTestType.SetWindowTextW(m_strCurrentTestType);
		m_edPlateNumber.SetWindowTextW(SVehicleInfo.wchPlateNumber);
		m_edOwner.SetWindowTextW(SVehicleInfo.wchOwner);
		m_cbPlateType.SetWindowTextW(SVehicleInfo.wchPlateType);
		m_edVIN.SetWindowTextW(SVehicleInfo.wchVIN);
		m_edModel.SetWindowTextW(SVehicleInfo.wchModel);
		COleDateTime odtProductionDate;
		if (!odtProductionDate.ParseDateTime(SVehicleInfo.wchProductionDate))
		{
			odtProductionDate = COleDateTime::GetCurrentTime();
		}
		m_dtcProductionDate.SetTime(odtProductionDate);
		COleDateTime odtRegistrationDate;
		if (!odtRegistrationDate.ParseDateTime(SVehicleInfo.wchRegistrationDate))
		{
			odtRegistrationDate = COleDateTime::GetCurrentTime();
		}
		m_dtcRegistrationDate.SetTime(odtRegistrationDate);
		m_cbDriveType.SetWindowTextW(SVehicleInfo.wchDriveType);
		m_edUnladenMass.SetWindowTextW(SVehicleInfo.wchUnladenMass);
		m_edMaximumTotalMass.SetWindowTextW(SVehicleInfo.wchMaximumTotalMass);
		m_cbFuelType.SetWindowTextW(SVehicleInfo.wchFuelType);
		m_cbAirIntakeMode.SetWindowTextW(SVehicleInfo.wchAirIntakeMode);
		m_cbOilSupplyMode.SetWindowTextW(SVehicleInfo.wchOilSupplyMode);
		str.Format(L"%.2f", _wtof(SVehicleInfo.wchRatedPower));
		m_edRatedPower.SetWindowTextW(str);
		int nNumberOfTestPeriod = _wtoi(sTestLog.wchNumberOfTestPeriod);
		if (nNumberOfTestPeriod != 1)
		{
			str.Format(L"��%d�θ���", nNumberOfTestPeriod-1);
		}
		else
		{
			str = L"����";
		}
		m_edIsRetest.SetWindowTextW(str);
		str.Format(L"%d", _wtoi(SVehicleInfo.wchRatedRev));
		m_edRatedRev.SetWindowTextW(str);
		str = SVehicleInfo.wchHasCatalyticConverter;
		if (str == L"1")
		{
			str = L"��";
		}
		else if (str == L"0")
		{
			str = L"��";
		}
		m_cbHasCatalyticConverter.SetWindowTextW(str);

		if (wcscmp(sTestLog.wchItemApp, L"") != 0)
		{
			switch (_wtoi(sTestLog.wchItemApp))
			{
			case 0:	m_edItemApp.SetWindowText(L"������");	break;
			case 1:	m_edItemApp.SetWindowText(L"�ȴ����");	break;
			case 2:	m_edItemApp.SetWindowText(L"׼�����");	break;
			case 3:	m_edItemApp.SetWindowText(L"���ڼ��");	break;
			case 4:	m_edItemApp.SetWindowText(L"��ɼ��");	break;
			default:	m_edItemApp.SetWindowText(sTestLog.wchItemApp);	break;
			}
		}
		else
		{
			m_edItemApp.SetWindowText(L"--");
		}

		if (wcscmp(sTestLog.wchItemOBD, L"") != 0)
		{
			switch (_wtoi(sTestLog.wchItemOBD))
			{
			case 0:	m_edItemOBD.SetWindowText(L"������");	break;
			case 1:	m_edItemOBD.SetWindowText(L"�ȴ����");	break;
			case 2:	m_edItemOBD.SetWindowText(L"׼�����");	break;
			case 3:	m_edItemOBD.SetWindowText(L"���ڼ��");	break;
			case 4:	m_edItemOBD.SetWindowText(L"��ɼ��");	break;
			default:	m_edItemOBD.SetWindowText(sTestLog.wchItemOBD);	break;
			}
		}
		else
		{
			m_edItemOBD.SetWindowText(L"--");
		}

		if (wcscmp(sTestLog.wchItemEmi, L"") != 0)
		{
			switch (_wtoi(sTestLog.wchItemEmi))
			{
			case 0:	m_edItemEmi.SetWindowText(L"������");	break;
			case 1:	m_edItemEmi.SetWindowText(L"�ȴ����");	break;
			case 2:	m_edItemEmi.SetWindowText(L"׼�����");	break;
			case 3:	m_edItemEmi.SetWindowText(L"���ڼ��");	break;
			case 4:	m_edItemEmi.SetWindowText(L"��ɼ��");	break;
			default:	m_edItemEmi.SetWindowText(sTestLog.wchItemEmi);	break;
			}
		}
		else
		{
			m_edItemEmi.SetWindowText(L"--");
		}

		{
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
#ifdef Up_OBD_RealTime
			OperationHint(L"ȷ����Ϣ��ȷ->ѡ���Ƿ��ϴ�OBD��������->ѡ��OBD����Ա");
#else
			OperationHint(L"ȷ����Ϣ��ȷ��->ѡ��OBD����Ա");
#endif
		}


	}
	else
	{
		InitVehicleInfoRegion();

		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	}

	EndWaitCursor();
}
