
// DROBDTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DROBDT.h"
#include "DROBDTDlg.h"
#include "afxdialogex.h"

#include "OBDTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDROBDTDlg 对话框




CDROBDTDlg::CDROBDTDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CDROBDTDlg::IDD, pParent)
	, m_pConnection(NULL)
	, m_siPrepareRegionIndex(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 针对1280*1024分辨率进行开发
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
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
		L"宋体");

	// 获取exe(dll)文件绝对路径
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", m_wchPathOfNHClient))
	{
	}
}

CDROBDTDlg::~CDROBDTDlg()
{
	// 删除字体
	m_fontDlgFont.DeleteObject();

	CNHSQLServerDBO::CloseDB(m_pConnection);
	if (m_pConnection != NULL)
	{
		m_pConnection = NULL;
	}
}

void CDROBDTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, IDC_CB_FILTER_TEST_TYPE, m_cbFilterTestType);
	DDX_Control(pDX, IDC_ED_FILTER, m_edFilter);
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

BEGIN_MESSAGE_MAP(CDROBDTDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CDROBDTDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CDROBDTDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDROBDTDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_UP, &CDROBDTDlg::OnBnClickedButtonLiftBeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_DOWN, &CDROBDTDlg::OnBnClickedButtonLiftBeamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CDROBDTDlg::OnNMDblclkLstVehicle)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDROBDTDlg 消息处理程序

BOOL CDROBDTDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 连接数据库
	InitDatabase();

	// 初始化控件
	InitCtrls();

	// 最大化显示
	ShowWindow(SW_MAXIMIZE);
	// 最大化后才能加载窗口
	InitEquipmentPrepareRegion();

	OperationHint(L"请更新列表");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDROBDTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogZoom::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDROBDTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDROBDTDlg::OperationHint(const CStringW& strOH)
{
	m_lbDynInfo.SetWindowTextW(strOH);
}

void CDROBDTDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strFilter, strTestType;
	m_cbFilterTestType.GetWindowText(strTestType);
	m_edFilter.GetWindowText(strFilter);

	if (L"稳态" == strTestType)
	{
		UpdateVehicleList(1, strFilter);
	}
	else if (L"简易瞬态" == strTestType)
	{
		UpdateVehicleList(2, strFilter);
	}
	else if (L"加载减速" == strTestType)
	{
		UpdateVehicleList(3, strFilter);
	}
	else if (L"双怠速" == strTestType)
	{
		UpdateVehicleList(4, strFilter);
	}
	else if (L"不透光自由加速" == strTestType)
	{
		UpdateVehicleList(5, strFilter);
	}
	else if (L"滤纸式自由加速" == strTestType)
	{
		UpdateVehicleList(6, strFilter);
	}
	else if (L"农用车自由加速" == strTestType)
	{
		UpdateVehicleList(7, strFilter);
	}
	else if (L"摩托车怠速" == strTestType)
	{
		UpdateVehicleList(8, strFilter);
	}
	else if (L"摩托车双怠速" == strTestType)
	{
		UpdateVehicleList(9, strFilter);
	}
	else
	{
		UpdateVehicleList(0, strFilter);
	}
}


void CDROBDTDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码

	m_cbDriver.GetWindowTextW(m_strDriver);
	if (L"" == m_strDriver)
	{
		AfxMessageBox(L"请选择引车员");
		return;
	}

	if (m_pConnection == NULL)
	{
		AfxMessageBox(L"未连接数据库不允许操作");
		return;
	}

	//复制引车员
	wcscpy_s(m_sTestLog.wchDriver, m_strDriver);

	COBDTestDlg dlg;
	CString str;

	dlg.m_bIsOBDRealTime = false;
	if (m_chkUpOBDRealTime.GetCheck() ==1)
	{
		dlg.m_bIsOBDRealTime = true;
	}
	dlg.m_sTestLog = m_sTestLog;

	dlg.m_pConnection = m_pConnection;

	m_edIsRetest.GetWindowTextW(str);
	int ntesttime = _wtoi(str);

	INT_PTR nDoModel = dlg.DoModal();
	if (IDOK == nDoModel)
	{

	}
	else if (IDCANCEL == nDoModel) 
	{

	}
	// 返回后必须置为空
	m_strCurrentRunningNumber = L"";

	const int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if (nIndex != -1)
	{
		m_lstVehicle.SetItemState(nIndex, 0, -1);
	}
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	OperationHint(L"");
}


void CDROBDTDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}


void CDROBDTDlg::OnBnClickedButtonLiftBeamUp()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDROBDTDlg::OnBnClickedButtonLiftBeamDown()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDROBDTDlg::OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	SelectVehicle();

	*pResult = 0;
}


void CDROBDTDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_fontVehicleLCFont.DeleteObject();
	CNHSQLServerDBO::CloseDB(m_pConnection);
}

void CDROBDTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogZoom::OnTimer(nIDEvent);
}

// 连接数据库
DWORD CDROBDTDlg::InitDatabase(void)
{
	// 返回值
	DWORD dwReturn(0x00);

	// 读配置文件,获取数据库连接信息
	// 获取exe(dll)文件绝对路径
	wchar_t wchPathNHModeServerDB[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHModeServerDB.ini", wchPathNHModeServerDB))
	{
		return 0x01;
	}

	// 获取数据库连接信息
	//  读FDInterface.ini
	CSimpleIni sio(wchPathNHModeServerDB);

	CString strSource, strCatalog, strUserID, strPassword;
	strSource = sio.GetString(L"Database", L"Source", L"");// 服务器名称
	strCatalog = sio.GetString(L"Database", L"Catalog", L"");// 数据库名称
	strUserID = sio.GetString(L"Database", L"UserID", L"");// 用户名
	strPassword = sio.GetString(L"Database", L"Password", L"");// 密码

	if (0x00 != CNHSQLServerDBO::OpenDB(m_pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		dwReturn = 0x01;
		OperationHint(L"连接数据库失败错误");
	}

	return dwReturn;
}

void CDROBDTDlg::InitCtrls(void)
{
	// 设置字体
	SetDlgFont();

	// 标题栏
	m_lbHead.SetTitle(L"设备准备和车辆选择");

	// 获取检测线类型
	GetIniNHClient(&m_sIniNHClient);

	// 测功机信息提示
	m_lbDynInfo.SetTextColor(RGB(255, 0, 0));
	//m_lbDynInfo.AutoFitFont();
	m_lbDynInfo.SetFontSize(40);
	m_lbDynInfo.SetFontName(L"黑体");

	// 是否需要显示升降举升器按钮
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

	// 通过宏定义，有些平台不支持上传过程数据
#ifdef Up_OBD_RealTime
	m_chkUpOBDRealTime.ShowWindow(SW_SHOW);
	m_chkUpOBDRealTime.EnableWindow(TRUE);
	m_chkUpOBDRealTime.SetCheck(FALSE);
#endif

}

void CDROBDTDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CDROBDTDlg::InitVehicleList(void)
{
	// 待检车列表
	m_lbVehicleList.SetTitle(L"待检车列表");

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
		L"黑体");

	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);

	m_lstVehicle.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstVehicle.SetFont(&m_fontVehicleLCFont);
	m_lstVehicle.SetTextColor(RGB(0, 255, 0));
	m_lstVehicle.SetBkColor(RGB(0, 0, 0));
	m_lstVehicle.SetTextBkColor(RGB(0, 0, 0));

	m_lstVehicle.InsertColumn(0, L"序号",		0,	0*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(1, L"流水号",		0,	300);
	m_lstVehicle.InsertColumn(2, L"号牌号码",	0,	150);
	m_lstVehicle.InsertColumn(3, L"号牌种类",	0,	120);
	m_lstVehicle.InsertColumn(4, L"VIN",	0,	120);
	m_lstVehicle.InsertColumn(5, L"车辆类别",	0,	300);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CDROBDTDlg::InitVehicleInfoRegion(BOOL Enable)
{
	m_lbVehicleInfo.SetTitle(L"车辆信息");
	m_lbEParamInfo.SetTitle(L"环境参数");

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

void CDROBDTDlg::InitTestTypeCombobox(void)
{
	// 读取系统类型,加载检测类型
	if (L'1' == m_sIniNHClient.wchSystemType[0])
	{
		m_cbFilterTestType.AddString(L"稳态");
		m_cbTestType.AddString(L"稳态");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[1])
	{
		m_cbFilterTestType.AddString(L"简易瞬态");
		m_cbTestType.AddString(L"简易瞬态");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[2])
	{
		m_cbFilterTestType.AddString(L"加载减速");
		m_cbTestType.AddString(L"加载减速");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[3])
	{
		m_cbFilterTestType.AddString(L"双怠速");
		m_cbTestType.AddString(L"双怠速");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[4])
	{
		m_cbFilterTestType.AddString(L"不透光自由加速");
		m_cbTestType.AddString(L"不透光自由加速");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[5])
	{
		m_cbFilterTestType.AddString(L"滤纸式自由加速");
		m_cbTestType.AddString(L"滤纸式自由加速");
	}
	if (L'1' == m_sIniNHClient.wchSystemType[6])
	{
		m_cbFilterTestType.AddString(L"农用车自由加速");
		m_cbTestType.AddString(L"农用车自由加速");
	} 
	if (L'1' == m_sIniNHClient.wchSystemType[7])
	{
		m_cbFilterTestType.AddString(L"摩托车怠速");
		m_cbTestType.AddString(L"摩托车怠速");
	} 
	if (L'1' == m_sIniNHClient.wchSystemType[8])
	{
		m_cbFilterTestType.AddString(L"摩托车双怠速");
		m_cbTestType.AddString(L"摩托车双怠速");
	} 
	m_cbFilterTestType.AddString(L"");
}

void CDROBDTDlg::InitDriverCombobox(void)
{
	CString strSql = L"select * from UserInfo where Role = 6";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// 打开查询成功
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

void CDROBDTDlg::InitComboboxPlateType(void)
{
	CString strSql = L"select * from ValueOfPlateType order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// 打开查询成功
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

void CDROBDTDlg::InitComboboxDriveType(void)
{
	CString strSql = L"select * from ValueOfDriveType order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// 打开查询成功
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

void CDROBDTDlg::InitComboboxFuelType(void)
{
	CString strSql = L"select * from ValueOfFuelType order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// 打开查询成功
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

void CDROBDTDlg::InitComboboxOilSupplyMode(void)
{
	CString strSql = L"select * from ValueOfOilSupplyMode order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// 打开查询成功
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

void CDROBDTDlg::InitComboboxAirIntakeMode(void)
{
	CString strSql = L"select * from ValueOfAirIntakeMode order by AutoID asc;";

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	{
		// 打开查询成功
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

void CDROBDTDlg::InitEquipmentPrepareRegion(void)
{
	// 读取系统类型,加载检测类型
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

void CDROBDTDlg::LoadGasolineVehicleEquipmentPrepare(void)
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

void CDROBDTDlg::LoadDieselVehicleEquipmentPrepare(void)
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

void CDROBDTDlg::UpdateVehicleList(short const siTestType/* = 0*/, CString strFilter/* = L""*/)
{
	// 删除列表数据
	m_lstVehicle.DeleteAllItems();

	std::list<SOBDVEHLIST> lsOBDVehList;

	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", theApp.m_strDeviceSN);
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVehicleList");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");

	std::wstring strRecv;
	bool bRet;
#ifndef _DEBUG
	bRet = theApp.SendAndRecvPacket(strXML, strRecv);
	if (bRet)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
#else
	if (true)
	{
		CXmlReader xmlReader;
		if (xmlReader.LoadFile(L"F:\\程序\\OBD\\OBD\\DROBDT\\OBDVehicleList.xml"))
		{
#endif
			std::wstring strResult, strMsg;

			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMsg);
			}

			if (strResult == L"")// 没有Result节点时有车辆列表
			{
				if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
				{
					do
					{
						std::map<std::wstring, std::wstring> mapField;	
						xmlReader.EnterNode();

						do
						{
							std::wstring strName, strCon;
							xmlReader.GetNodeName(strName);
							xmlReader.GetNodeContent(strCon);
							mapField[strName] = strCon;
						}
						while(xmlReader.NextSibling());
						xmlReader.ExitNode();

						SOBDVEHLIST sList;

						sList.strRunningNumber = mapField[L"OutlookID"].c_str();
						sList.strPaletNumber = mapField[L"License"].c_str();
						sList.strPlateType = mapField[L"LicenseType"].c_str();
						sList.strVIN = mapField[L"Vin"].c_str();
						sList.strVehType = mapField[L"VehicleKind"].c_str();

						lsOBDVehList.push_back(sList);
					}
					while (xmlReader.NextSibling());
				}
			}
			else
			{
				OperationHint(L"获取列表失败");
				CString strTemp;
				strTemp.Format(L"%s->%s->%s", L"获取列表失败", strResult.c_str(), strMsg.c_str());
				MessageBox(strTemp);
				return;
			}

		}
		else
		{
			OperationHint(L"解析失败");
			CString strTemp;
			strTemp.Format(L"%s->%s", L"解析失败", strRecv.c_str());
			MessageBox(strTemp);
			return;
		}
	}
	else
	{
		OperationHint(L"联网失败");
		CString strTemp;
		strTemp.Format(L"%s->%s", L"获取联网失败", strRecv.c_str());
		MessageBox(strTemp);
		return;
	}

	if (lsOBDVehList.size() > 0)
	{
		int nItem(0);

		for(list<SOBDVEHLIST>::iterator ir =lsOBDVehList.begin(); ir != lsOBDVehList.end(); ir++)
		{
			CString str;
			// 序号
			str.Format(L"%d", nItem+1);
			int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

			// 流水号
			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strRunningNumber.c_str(), 0, 0, 0, 0);
			// 号牌号码
			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strPaletNumber.c_str(), 0, 0, 0, 0);
			// 号牌种类
			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strPlateType.c_str(), 0, 0, 0, 0);
			// VIN
			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strVIN.c_str(), 0, 0, 0, 0);
			// 车辆类别
			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strVehType.c_str(), 0, 0, 0, 0);

			nItem++;

		}
	}
}

void CDROBDTDlg::SelectVehicle(void)
{
	if (theApp.m_strOperatorPassword.IsEmpty())
	{
		CString strSql;
		strSql.Format(L"SELECT * from userInfo WHERE Name = '%s'", theApp.m_strOperatorName);
		USERINFO sUSERINFO;
		if (GetDboUserInfo(strSql, &sUSERINFO) == 0x01)
		{
			theApp.m_strOperatorPassword = sUSERINFO.wchPassword;
		}
		else
		{
			theApp.m_strOperatorPassword = L"111111";
		}
	}

	CString strMsg;
	if (OBDVerify(strMsg))
	{
		GetVehicleInfo();
	}
	
}

void CDROBDTDlg::GetVehicleInfo(void)
{
	BeginWaitCursor();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(nIndex!=-1)
	{

		TESTLOG sTestLog;
		SOBDVEHLIST sOBDVehList;

		sOBDVehList.strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		sOBDVehList.strPaletNumber = m_lstVehicle.GetItemText(nIndex,2);
		sOBDVehList.strPlateType = m_lstVehicle.GetItemText(nIndex,3);
		sOBDVehList.strVIN = m_lstVehicle.GetItemText(nIndex,4);
		sOBDVehList.strVehType = m_lstVehicle.GetItemText(nIndex,5);

		m_strCurrentRunningNumber = sOBDVehList.strRunningNumber.c_str();


		CString strXML;
		strXML.AppendFormat(L"<Message Device=\"%s\">", theApp.m_strDeviceSN);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVehicleInfo");
		strXML.AppendFormat(L"<Row>");
		strXML.AppendFormat(L"<OutlookID>%s</OutlookID>", sOBDVehList.strRunningNumber.c_str());
		strXML.AppendFormat(L"<License>%s</License>", L"");
		strXML.AppendFormat(L"<LicenseType>%s</LicenseType>", L"");
		strXML.AppendFormat(L"<VIN>%s</VIN>", L"");
		strXML.AppendFormat(L"<DateTime>%s</DateTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
		strXML.AppendFormat(L"</Row>");
		strXML.AppendFormat(L"</Request>");
		strXML.AppendFormat(L"</Message>");

		std::wstring strRecv;
#ifndef _DEBUG
		bool bRet = theApp.SendAndRecvPacket(strXML, strRecv);
		if (bRet)
#else
		if (true)
#endif
		{
			CXmlReader xmlReader;
#ifndef _DEBUG
			if (xmlReader.Parse(strRecv.c_str()))
#else
			if (xmlReader.LoadFile(L"F:\\程序\\OBD\\OBD\\DROBDT\\VehicleInfo.xml"))
#endif
			{
				std::wstring strResult, strMsg;

				if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
				{
					xmlReader.GetNodeContent(strResult);
				}
				if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
				{
					xmlReader.GetNodeContent(strMsg);
				}

				if (strResult == L"")
				{
					std::wstring strTemp;
					if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
					{
						std::map<std::wstring, std::wstring> mapField;	
						xmlReader.EnterNode();
						do
						{
							std::wstring strName, strContent;
							xmlReader.GetNodeName(strName);
							xmlReader.GetNodeContent(strContent);
							mapField[strName] = strContent;
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();


						wcscpy_s(sTestLog.wchReportNumber, sOBDVehList.strRunningNumber.c_str());
						wcscpy_s(sTestLog.wchRunningNumber, sOBDVehList.strRunningNumber.c_str());
						wcscpy_s(sTestLog.wchPlateNumber, mapField[L"License"].c_str());
						wcscpy_s(sTestLog.wchVIN, mapField[L"Vin"].c_str());
						wcscpy_s(sTestLog.wchRegistrationDate, mapField[L"RegisterDate"].c_str());

						wcscpy_s(sTestLog.wchModel, mapField[L"CLXH"].c_str());
						wcscpy_s(sTestLog.wchEngineModel, mapField[L"FDJXH"].c_str());
						wcscpy_s(sTestLog.wchHasOBD, mapField[L"HasOBD"].c_str());

						//0	汽油
						//1	柴油
						//2	液化石油气(LPG)
						//3	天然气(CNG)
						//4	其他
						//6	双燃料
						//8	混合动力
						//9	电
						if (mapField[L"FuelType"] == L"0")
						{
							wcscpy_s(sTestLog.wchFuelType, L"汽油");
						}
						else if (mapField[L"FuelType"] == L"1")
						{
							wcscpy_s(sTestLog.wchFuelType, L"柴油");
						}
						else if (mapField[L"FuelType"] == L"2")
						{
							wcscpy_s(sTestLog.wchFuelType, L"液化石油气(LPG)");
						}
						else if (mapField[L"FuelType"] == L"3")
						{
							wcscpy_s(sTestLog.wchFuelType, L"天然气(CNG)");
						}
						else if (mapField[L"FuelType"] == L"4")
						{
							wcscpy_s(sTestLog.wchFuelType, L"其他");
						}
						else if (mapField[L"FuelType"] == L"6")
						{
							wcscpy_s(sTestLog.wchFuelType, L"双燃料");
						}
						else if (mapField[L"FuelType"] == L"8")
						{
							wcscpy_s(sTestLog.wchFuelType, L"混合动力");
						}
						else if (mapField[L"FuelType"] == L"9")
						{
							wcscpy_s(sTestLog.wchFuelType, L"电");
						}
						else
						{
							wcscpy_s(sTestLog.wchFuelType, mapField[L"FuelType"].c_str());
						}
					}
				}
				else
				{
					OperationHint(L"获取车辆信息失败");
					CString strTemp;
					strTemp.Format(L"%s->%s->%s", L"获取车辆信息失败", strResult.c_str(), strMsg.c_str());
					MessageBox(strTemp);
					return;
				}
			}
			else
			{
				OperationHint(L"解析失败");
				CString strTemp;
				strTemp.Format(L"%s->%s", L"解析失败", strRecv.c_str());
				MessageBox(strTemp);
				return;
			}
		}
		else
		{
			OperationHint(L"联网失败");
			CString strTemp;
			strTemp.Format(L"%s->%s", L"获取联网失败", strRecv.c_str());
			MessageBox(strTemp);
			return;
		}

		if (wcscmp(sTestLog.wchReportNumber, L"") != 0)
		{
			//号牌号码
			m_edPlateNumber.SetWindowTextW(sTestLog.wchPlateNumber);
			//车架号
			m_edVIN.SetWindowTextW(sTestLog.wchVIN);
			//车辆出厂日期。YYYY-MM-DD
			m_dtcProductionDate.SetWindowTextW(sTestLog.wchRegistrationDate);
			//检测燃料种类
			m_cbFuelType.SetWindowTextW(sTestLog.wchFuelType);
			// 
			m_edModel.SetWindowTextW(sTestLog.wchModel);

			m_sTestLog = sTestLog;
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
#ifdef Up_OBD_RealTime
			OperationHint(L"确定信息正确->选着是否上传OBD过程数据->选择OBD操作员");
#else
			OperationHint(L"确定信息正确后->选择OBD操作员");
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

bool CDROBDTDlg::OBDVerify(CString& strMsg)
{
	CString strXML;
	strXML.AppendFormat(L"<Message Device=\"%s\">", theApp.m_strDeviceSN.GetString());
	strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVerify");
	strXML.AppendFormat(L"<Row>");	
	strXML.AppendFormat(L"<User>%s</User>", theApp.m_strOperatorName.GetString());
	strXML.AppendFormat(L"<Pwd>%s</Pwd>", theApp.m_strOperatorPassword.GetString());
	strXML.AppendFormat(L"<LoginType>%s</LoginType>", L"2");
	strXML.AppendFormat(L"</Row>");
	strXML.AppendFormat(L"</Request>");
	strXML.AppendFormat(L"</Message>");

	std::wstring strRecv;
#ifndef _DEBUG
	bool bRet = theApp.SendAndRecvPacket(strXML, strRecv);
	if (bRet)
#else
	if (true)
#endif
	{
		CXmlReader xmlReader;
#ifndef _DEBUG
		if (xmlReader.Parse(strRecv.c_str()))
#else
		if (xmlReader.LoadFile(L"F:\\程序\\OBD\\OBD\\DROBDT\\OBDVerify.xml"))
#endif
		{
			std::wstring strResult, strMessage;

			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMessage);
			}
			if (strResult == L"1")
			{
				return true;
			}
			else
			{
				OperationHint(L"OBD登录失败");
				CString strTemp;
				strTemp.Format(L"%s->%s->%s", L"登录失败", strResult.c_str(), strMessage.c_str());
				MessageBox(strTemp);
			}
		}
		else
		{
			OperationHint(L"解析失败");
			CString strTemp;
			strTemp.Format(L"%s->%s", L"解析失败", strRecv.c_str());
			MessageBox(strTemp);
		}
	}
	else
	{
		OperationHint(L"联网失败");
		CString strTemp;
		strTemp.Format(L"%s->%s", L"获取联网失败", strRecv.c_str());
		MessageBox(strTemp);
	}


	return false;
}