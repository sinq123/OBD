
// ACOBDTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ACOBDT.h"
#include "ACOBDTDlg.h"
#include "afxdialogex.h"
#include "IntInfo.h"

#include "OBDTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACOBDTDlg �Ի���




CACOBDTDlg::CACOBDTDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CACOBDTDlg::IDD, pParent)
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

CACOBDTDlg::~CACOBDTDlg()
{
	// ɾ������
	m_fontDlgFont.DeleteObject();

	if (m_pConnection != NULL)
	{
		m_pConnection = NULL;
	}
}


void CACOBDTDlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(CACOBDTDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CACOBDTDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CACOBDTDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CACOBDTDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_UP, &CACOBDTDlg::OnBnClickedButtonLiftBeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_DOWN, &CACOBDTDlg::OnBnClickedButtonLiftBeamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CACOBDTDlg::OnNMDblclkLstVehicle)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CACOBDTDlg ��Ϣ�������

BOOL CACOBDTDlg::OnInitDialog()
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

void CACOBDTDlg::OnPaint()
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
HCURSOR CACOBDTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CACOBDTDlg::OperationHint(const CStringW& strOH)
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


void CACOBDTDlg::OnBnClickedButtonUpdateVehicleList()
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


void CACOBDTDlg::OnBnClickedBtnNext()
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

	dlg.m_bIsOBDRealTime = m_chkUpOBDRealTime.GetCheck();
	m_edPlateNumber.GetWindowTextW(str);
	dlg.m_strPlateNumber = str.GetString();
	m_edVIN.GetWindowTextW(str);
	dlg.m_strVIN = str.GetString();
	m_cbFuelType.GetWindowTextW(str);
	dlg.m_strFuelType = str.GetString();
	dlg.m_strRunningNumber = m_strCurrentRunningNumber.GetString();
	dlg.m_strOperator = m_strDriver;
	dlg.m_pConnection = m_pConnection;

	m_edIsRetest.GetWindowTextW(str);
	int ntesttime = _wtoi(str);
	dlg.m_ntesttimes = ntesttime;

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


void CACOBDTDlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}


void CACOBDTDlg::OnBnClickedButtonLiftBeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CACOBDTDlg::OnBnClickedButtonLiftBeamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CACOBDTDlg::OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SelectVehicle();

	*pResult = 0;
}


void CACOBDTDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

	m_fontVehicleLCFont.DeleteObject();
}


void CACOBDTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogZoom::OnTimer(nIDEvent);
}

// �������ݿ�
DWORD CACOBDTDlg::InitDatabase(void)
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

void CACOBDTDlg::InitCtrls(void)
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

	// ͨ���궨�壬��Щƽ̨��֧���ϴ���������
#ifdef Up_OBD_RealTime
	m_chkUpOBDRealTime.ShowWindow(SW_SHOW);
	m_chkUpOBDRealTime.EnableWindow(TRUE);
	m_chkUpOBDRealTime.SetCheck(TRUE);
#endif

}

void CACOBDTDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CACOBDTDlg::InitVehicleList(void)
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
	m_lstVehicle.InsertColumn(6, L"ȼ�����",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(7, L"VIN",	0,	120*nSM_CXSCREEN/1280);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CACOBDTDlg::InitVehicleInfoRegion(BOOL Enable)
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

void CACOBDTDlg::InitTestTypeCombobox(void)
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

void CACOBDTDlg::InitDriverCombobox(void)
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

void CACOBDTDlg::InitComboboxPlateType(void)
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

void CACOBDTDlg::InitComboboxDriveType(void)
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

void CACOBDTDlg::InitComboboxFuelType(void)
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

void CACOBDTDlg::InitComboboxOilSupplyMode(void)
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

void CACOBDTDlg::InitComboboxAirIntakeMode(void)
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

void CACOBDTDlg::InitEquipmentPrepareRegion(void)
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

void CACOBDTDlg::LoadGasolineVehicleEquipmentPrepare(void)
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

void CACOBDTDlg::LoadDieselVehicleEquipmentPrepare(void)
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

void CACOBDTDlg::UpdateVehicleList(short const siTestType/* = 0*/, CString strFilter/* = L""*/)
{
	// ɾ���б�����
	m_lstVehicle.DeleteAllItems();

	CString strMsg;
	// ����
	CString strTimes, strXmlDoc;

	SYSTEMTIME st;
	GetLocalTime(&st);
	strTimes.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strXmlDoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strXmlDoc.AppendFormat(L"<organ>%s</organ>", theApp.m_strStationNum);
	strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", theApp.m_strLineNum);
	strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"GetObd");
	strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strTimes);
	strXmlDoc.AppendFormat(L"</head><vehispara>");
	strXmlDoc.AppendFormat(L"<license>%s</license>", L"");
	strXmlDoc.AppendFormat(L"<licensetype>%s</licensetype>", L"");
	strXmlDoc.AppendFormat(L"<vin>%s</vin>", L"");
	strXmlDoc.AppendFormat(L"<tsno>%s</tsno>", theApp.m_strStationNum);
	strXmlDoc.AppendFormat(L"</vehispara></root>");

	std::wstring strRetStr(L"");
	int nRet;
	//nRet = CACInterfaceLib_API::QueryObjectOut(m_pchURL, strXmlDoc.GetString(), strRetStr);
	nRet = CACHBIntLib_API::QueryObjectOut(theApp.m_pchURL, strXmlDoc.GetString(), strRetStr);

	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)theApp.DecodeURI(strRetStr.c_str());

		CXmlReader xmlReader;
		if (xmlReader.Parse(strRetStr.c_str()))
		{
			std::wstring strCode, strContent, str;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);

			}
			if (xmlReader.OpenNode(L"root/head/message"))
			{
				xmlReader.GetNodeContent(strContent);
			}
			if (strCode == L"1")
			{
				if (xmlReader.OpenNode(L"root/body/vehispara"))
				{
					int nItem(0);
					std::wstring strNodeName, strName, strContent;

					do
					{
						CString str;
						// ���
						str.Format(L"%d", nItem+1);
						int nItemIndex = m_lstVehicle.InsertItem(nItem, str);
						xmlReader.EnterNode();
						do
						{
							// ���ƺ���
							if(L"jcCarBrandNumber"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ��������
							if(L"jcCarBrandType"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ��ⷽ��
							if(L"jcCarCheckMethod"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
								//int n = _wtoi(strContent.c_str());
								//switch (n)
								//{
								//case 1:{m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"˫����", 0, 0, 0, 0);}break;
								//case 2:{m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"��̬", 0, 0, 0, 0);}break;
								//case 3:{m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"����˲̬", 0, 0, 0, 0);}break;
								//case 4:{m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"���ؼ���", 0, 0, 0, 0);}break;
								//case 5:{m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"��͸���̶ȷ�", 0, 0, 0, 0);}break;
								//case 6:{m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"��ֽʽ�̶ȷ�", 0, 0, 0, 0);}break;
								//default: {m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, L"��������", 0, 0, 0, 0);}break;
								//}
							}
							// ��ˮ��
							if(L"jcCarCheckSerialNumber"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent);
								m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ������
							if(L"jcCarCheckDetectionTimes"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ȼ������
							if(L"jcCarFuelType"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// VIN
							if(L"jcCarIdentificationNumbe"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
						}
						while (xmlReader.NextSibling());
						nItem++;
						xmlReader.ExitNode();
					}
					while (xmlReader.NextSibling());
				}
			}
			else
			{
				strMsg.Format(L"��ȡ�б���Ϣ����:%s %s", strCode.c_str(), strContent.c_str());
				OperationHint(strMsg);
				return;
			}
		}
		else
		{
			strMsg.Format(L"��ȡ�б���Ϣ����:%d %s", nRet, L"����ʧ��");
			OperationHint(strMsg);
			return;
		}
	}
	else
	{
		strMsg.Format(L"��ȡ�б���Ϣ����:%d %s", nRet, L"�ӿڷ���ʧ��");
		OperationHint(strMsg);
		return;
	}

}

void CACOBDTDlg::SelectVehicle(void)
{
	GetVehicleInfo();
}

void CACOBDTDlg::GetVehicleInfo(void)
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

		// ��ʾ��Ϣ
		CString ShowInfo;
		ShowInfo.Empty();
		ShowInfo = m_lstVehicle.GetItemText(nIndex,1);
		m_edPlateNumber.SetWindowTextW(ShowInfo);

		ShowInfo.Empty();
		ShowInfo = m_lstVehicle.GetItemText(nIndex,7);
		m_edVIN.SetWindowTextW(ShowInfo);
		
		ShowInfo.Empty();
		ShowInfo = m_lstVehicle.GetItemText(nIndex,6);
		m_cbFuelType.SetWindowTextW(ShowInfo);

		ShowInfo.Empty();
		ShowInfo = m_lstVehicle.GetItemText(nIndex,5);
		m_edIsRetest.SetWindowTextW(ShowInfo);
		
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

