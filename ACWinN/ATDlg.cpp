
// ATDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AT.h"
#include "ATDlg.h"
#include "afxdialogex.h"

#include "OBDTestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CATDlg 对话框




CATDlg::CATDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CATDlg::IDD, pParent)
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

CATDlg::~CATDlg()
{
	// 删除字体
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


// CATDlg 消息处理程序

BOOL CATDlg::OnInitDialog()
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

void CATDlg::OnPaint()
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

	//以上获取本地数据库
	// 以下获取平台
	//GetIntVehList();
}


void CATDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码

	m_cbDriver.GetWindowTextW(m_strDriver);
	if (L"" == m_strDriver)
	{
		AfxMessageBox(L"请选择引车员");
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


void CATDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}


void CATDlg::OnBnClickedButtonLiftBeamUp()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CATDlg::OnBnClickedButtonLiftBeamDown()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CATDlg::OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	SelectVehicle();
	//以上获取本地数据库
	// 以下获取平台
	GetIntVehInfo();
	*pResult = 0;
}


void CATDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_fontVehicleLCFont.DeleteObject();
}


void CATDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogZoom::OnTimer(nIDEvent);
}

// 连接数据库
DWORD CATDlg::InitDatabase(void)
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

void CATDlg::InitCtrls(void)
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
	m_lstVehicle.InsertColumn(1, L"车牌号码",	0,	320*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(2, L"车牌类型",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(3, L"检测类型",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(4, L"流水号",		0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(5, L"初/复检",	0,	120*nSM_CXSCREEN/1280);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CATDlg::InitVehicleInfoRegion(BOOL Enable)
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

void CATDlg::InitTestTypeCombobox(void)
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

void CATDlg::InitDriverCombobox(void)
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

void CATDlg::InitComboboxPlateType(void)
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

void CATDlg::InitComboboxDriveType(void)
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

void CATDlg::InitComboboxFuelType(void)
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

void CATDlg::InitComboboxOilSupplyMode(void)
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

void CATDlg::InitComboboxAirIntakeMode(void)
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

void CATDlg::InitEquipmentPrepareRegion(void)
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
	// 删除列表数据
	m_lstVehicle.DeleteAllItems();

	CString strSql;
	strSql.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
	strSql.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
	strSql.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");

	// 增加筛选
	if (!strFilter.IsEmpty())
	{
		strSql.AppendFormat(L" and TestLog.PlateNumber like '%%%s%%'", strFilter);
	}
	// 检测方法分
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
	{ // 打开查询成功

		try
		{
			int i(0);
			while(!pRecordset->adoEOF)
			{
				int n(0);
				CString str;
				_variant_t v;

				// 序号
				str.Format(L"%d", i+1);
				//int nItemIndex = m_lstVehicle.InsertItem(LVIF_TEXT | LVIF_PARAM,i, str, 0, 0, 0, 0);            
				int nItemIndex = m_lstVehicle.InsertItem(i, str);

				// 车牌号码
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

				// 号牌种类
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

				// 检测类型
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("TestType"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					n =  v;

					if (6 == n)
					{
						// FSUNHT代替FSUYD(begin)
						CSimpleIni si(m_wchPathOfNHClient);
						CStringW str = si.GetString(L"FSUYDParams", L"UseFSUNHTInsteadOfFSUYD", L"0");
						if (L"1" == str)
						{
							n = 5;
						}
						// FSUNHT代替FSUYD(end)
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
						str = L"稳态";
					}
					break;
				case 2:
					{
						str = L"简易瞬态";
					}
					break;
				case 3:
					{
						str = L"加载减速";
					}
					break;
				case 4:
					{
						str = L"双怠速";
					}
					break;
				case 5:
					{
						str = L"不透光自由加速";
					}
					break;
				case 6:
					{
						str = L"滤纸式自由加速";
					}
					break;
				case 7:
					{
						str = L"农用车自由加速";
					}
					break;
				case 8:
					{
						str = L"摩托车怠速";
					}
					break;
				case 9:
					{
						str = L"摩托车双怠速";
					}
					break;
				default:
					{
						str = L"错误类型";
					}
					break;
				}
				m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);

				// 流水号
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

				// 是否复检
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("IsRetest"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					bool b =  (bool)v;
					if (b)
					{
						str = L"复检";
					}
					else
					{
						str = L"初检";
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

			OperationHint(L"更新列表错误");
		}
	}

	if (dwDBORet != 0xFFFFFFFF)
	{
		OperationHint(L"请双击车牌");
	}

	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CATDlg::SelectVehicle(void)
{
	GetVehicleInfo();
	// 以上获取数据库
	// 以下获取平台
	//GetIntVehInfo();
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

		// 获取开单信息
		wchar_t wchSql[SQLMAX];
		TESTLOG sTestLog;
		wsprintf(wchSql, L"select * from TestLog where RunningNumber = '%s'", m_strCurrentRunningNumber);
		GetDboTestLog(wchSql, &sTestLog);

		// 获取车辆信息
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
			str.Format(L"第%d次复检", nNumberOfTestPeriod-1);
		}
		else
		{
			str = L"初检";
		}
		m_edIsRetest.SetWindowTextW(str);
		str.Format(L"%d", _wtoi(SVehicleInfo.wchRatedRev));
		m_edRatedRev.SetWindowTextW(str);
		str = SVehicleInfo.wchHasCatalyticConverter;
		if (str == L"1")
		{
			str = L"有";
		}
		else if (str == L"0")
		{
			str = L"无";
		}
		m_cbHasCatalyticConverter.SetWindowTextW(str);

		if (wcscmp(sTestLog.wchItemApp, L"") != 0)
		{
			switch (_wtoi(sTestLog.wchItemApp))
			{
			case 0:	m_edItemApp.SetWindowText(L"免除检测");	break;
			case 1:	m_edItemApp.SetWindowText(L"等待检测");	break;
			case 2:	m_edItemApp.SetWindowText(L"准备检测");	break;
			case 3:	m_edItemApp.SetWindowText(L"正在检测");	break;
			case 4:	m_edItemApp.SetWindowText(L"完成检测");	break;
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
			case 0:	m_edItemOBD.SetWindowText(L"免除检测");	break;
			case 1:	m_edItemOBD.SetWindowText(L"等待检测");	break;
			case 2:	m_edItemOBD.SetWindowText(L"准备检测");	break;
			case 3:	m_edItemOBD.SetWindowText(L"正在检测");	break;
			case 4:	m_edItemOBD.SetWindowText(L"完成检测");	break;
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
			case 0:	m_edItemEmi.SetWindowText(L"免除检测");	break;
			case 1:	m_edItemEmi.SetWindowText(L"等待检测");	break;
			case 2:	m_edItemEmi.SetWindowText(L"准备检测");	break;
			case 3:	m_edItemEmi.SetWindowText(L"正在检测");	break;
			case 4:	m_edItemEmi.SetWindowText(L"完成检测");	break;
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

void CATDlg::GetIntVehList(void)
{
	CString strMsg;
	// 安车
	CString strTimes, strXmlDoc;

	SYSTEMTIME st;
	GetLocalTime(&st);
	strTimes.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strXmlDoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strXmlDoc.AppendFormat(L"<organ>%s</organ>", theApp.m_strStationNum);
	strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", theApp.m_strLineNum);
	strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"DJ001");
	strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strTimes);
	strXmlDoc.AppendFormat(L"</head><vehispara>");
	strXmlDoc.AppendFormat(L"</vehispara></root>");

	std::wstring strRetStr(L"");
	int nRet;
	nRet = CACInterfaceLib_API::QueryObjectOut(theApp.m_pchURL, strXmlDoc.GetString(), strRetStr);

	std::list<SWaitQueInfo> lsWaitQueInfo;
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
					std::wstring strNodeName, strName, strContent;
					do
					{
						SWaitQueInfo sWaitQueInfo;
						xmlReader.EnterNode();
						do
						{
							xmlReader.GetNodeName(strNodeName);
							// 检测站编号
							if(L"tsno"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strtsno); 
							}
							// 检验流水号
							if(L"jylsh"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strjylsh); 
							}
							// 检验次数
							if(L"testtimes"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strtesttimes); 
							}
							// 号牌号码
							if(L"license"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strlicense); 
							}
							// 号牌种类
							if(L"licensecode"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strlicensecode); 
							}
							// 检测方法
							if(L"testtype"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strtesttype); 
							}
							// 登录时间
							if(L"dlsj"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strdlsj); 
							}
							// 状态
							if(L"zt"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sWaitQueInfo.strzt); 
							}
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();
						lsWaitQueInfo.push_back(sWaitQueInfo);
					}
					while (xmlReader.NextSibling());
				}
			}
			else
			{
				strMsg.Format(L"获取列表信息返回:%s %s", strCode.c_str(), strContent.c_str());
				OperationHint(strMsg);
				return;
			}
		}
		else
		{
			strMsg.Format(L"获取列表信息返回:%d %s", nRet, L"解析失败");
			OperationHint(strMsg);
			return;
		}
	}
	else
	{

		strMsg.Format(L"获取列表信息返回:%d %s", nRet, L"接口访问失败");
		OperationHint(strMsg);
		return;
	}

	// 删除列表数据
	m_lstVehicle.DeleteAllItems();
	int nItem(0);
	if (lsWaitQueInfo.size() < 0)
	{
		OperationHint(L"无车辆列表返回");
	}
	else
	{
		for (list<SWaitQueInfo>::iterator ir=lsWaitQueInfo.begin(); ir!=lsWaitQueInfo.end(); ++ir)
		{
			// 是否全部返回？？
			CString strzt = ir->strzt.c_str();
			if (L"1" != strzt)
			{
				continue;
			}
			// 注释以上全部返回

			CString str;
			// 序号
			str.Format(L"%d", nItem+1);

			int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

			// 车牌号码
			str = ir->strlicense.c_str();
			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);
			// 车牌类型
			str = ir->strlicensecode.c_str();
			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);
			// 检测方法
			int n = _wtoi(ir->strtesttype.c_str());
			switch (n)
			{
			case 1: {str = L"双怠速";} break;
			case 2: {str = L"稳态";} break;
			case 3: {str = L"简易瞬态";} break;
			case 4: {str = L"加载减速";} break;
			case 5: {str = L"不透光烟度法";} break;
			case 6: {str = L"滤纸式烟度法";} break;
			default: {str = L"错误类型";} break;
			}
			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);
			// 流水号
			str = ir->strjylsh.c_str();
			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);
			// 检测次数
			str = ir->strtesttimes.c_str();
			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

			nItem++;
		}
	}
}

void CATDlg::GetIntVehInfo(void)
{
	BeginWaitCursor();

	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		CString strRunningNumber,strPlateNumber,strPlateType, strDetTimes;

		strPlateNumber = m_lstVehicle.GetItemText(nIndex,1);
		strPlateType = m_lstVehicle.GetItemText(nIndex,2);
		m_strCurrentTestType = m_lstVehicle.GetItemText(nIndex,3);
		strRunningNumber = m_lstVehicle.GetItemText(nIndex,4);
		strDetTimes = m_lstVehicle.GetItemText(nIndex,5);

		m_strCurrentRunningNumber = strRunningNumber;

		CString strMsg;
		// 安车
		CString strTimes, strXmlDoc;

		SYSTEMTIME st;
		GetLocalTime(&st);
		strTimes.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		strXmlDoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
		strXmlDoc.AppendFormat(L"<organ>%s</organ>", theApp.m_strStationNum);
		strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", theApp.m_strLineNum);
		strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"DL002");
		strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strTimes);
		strXmlDoc.AppendFormat(L"</head><vehispara>");
		strXmlDoc.AppendFormat(L"<jylsh>%s</jylsh>", strRunningNumber);
		strXmlDoc.AppendFormat(L"<testtimes>%s</testtimes>", strDetTimes);
		strXmlDoc.AppendFormat(L"</vehispara></root>");

		std::wstring strRetStr;
		int nRet;
		nRet = CACInterfaceLib_API::QueryObjectOut(theApp.m_pchURL, strXmlDoc.GetString(), strRetStr);

		if (nRet == 0)
		{

			m_sIntVehInfo = SIntVehInfo();
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
					m_sIntVehInfo = SIntVehInfo();
					m_cbTestType.SetWindowTextW(m_strCurrentTestType);
					//1 jylsh varchar2(50) 检验流水号 检测站编号(8 位)+年月日 (yymmdd 6 位)+当日累计流 水(5 位) 
					if (xmlReader.OpenNode(L"root/body/vehispara/jylsh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strjylsh = strContent.c_str();
					}
					//2 testtimes number(3) 检验次数 本检验周期内检验次数，免 检为 0 
					if (xmlReader.OpenNode(L"root/body/vehispara/testtimes"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strtesttimes = strContent.c_str();
						m_edIsRetest.SetWindowTextW(m_sIntVehInfo.strtesttimes.c_str());
					}
					//3 license varchar2(20) 号牌号码 鲁 B19928，带首位汉字， 教练车、警车带末位汉字 
					if (xmlReader.OpenNode(L"root/body/vehispara/license"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlicense = strContent.c_str();
						m_edPlateNumber.SetWindowTextW(m_sIntVehInfo.strlicense.c_str());
					}
					//4 licensetype number(1) 车牌颜色 附录 3.4 
					if (xmlReader.OpenNode(L"root/body/vehispara/licensetype"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlicensetype = strContent.c_str();
					}
					//5 licensecode varchar2(2) 号牌种类 按 GA 24.7 
					if (xmlReader.OpenNode(L"root/body/vehispara/licensecode"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlicensecode = strContent.c_str();
					}
					//6 vehicletype varchar2(3) 车辆类型 按 GA24.4-2005 
					if (xmlReader.OpenNode(L"root/body/vehispara/vehicletype"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strvehicletype = strContent.c_str();
						m_cbPlateType.SetWindowTextW(m_sIntVehInfo.strvehicletype.c_str());
					}
					//7 vehiclemodel varchar2(30) 车辆型号 
					if (xmlReader.OpenNode(L"root/body/vehispara/vehiclemodel"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strvehiclemodel = strContent.c_str();
						m_edModel.SetWindowTextW(m_sIntVehInfo.strvehiclemodel.c_str());
					}
					//8 cpmc varchar2(30) 厂牌名称/商标 如：桑塔纳 
					if (xmlReader.OpenNode(L"root/body/vehispara/cpmc"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strcpmc = strContent.c_str();
					}
					//9 vin varchar2(30) 车辆识别代号 填写 VIN 码或者车架号 
					if (xmlReader.OpenNode(L"root/body/vehispara/vin"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strvin = strContent.c_str();
						m_edVIN.SetWindowTextW(m_sIntVehInfo.strvin.c_str());
					}
					//10 clscqy varchar2(50) 车辆生产厂 
					if (xmlReader.OpenNode(L"root/body/vehispara/clscqy"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strclscqy = strContent.c_str();
					}
					//11 engine varchar2(30) 发动机型号 
					if (xmlReader.OpenNode(L"root/body/vehispara/engine"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strengine = strContent.c_str();
					}
					//12 enginemanuf varchar2(50) 发动机生产厂 
					if (xmlReader.OpenNode(L"root/body/vehispara/enginemanuf"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strenginemanuf = strContent.c_str();
					}
					//13 fdjh varchar2(20) 发动机编号 
					if (xmlReader.OpenNode(L"root/body/vehispara/fdjh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strfdjh = strContent.c_str();
					}
					//14 ed number(5,3) 发动机排量 升(L) 
					if (xmlReader.OpenNode(L"root/body/vehispara/ed"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.stred = strContent.c_str();
					}
					//15 enginespeed number(4) 发动机额定转速 转每分(r/min) 
					if (xmlReader.OpenNode(L"root/body/vehispara/enginespeed"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strenginespeed = strContent.c_str();
						m_edRatedRev.SetWindowTextW(m_sIntVehInfo.strenginespeed.c_str());
					}
					//16 enginepower number(4) 发动机额定功率 千瓦(kw)
					if (xmlReader.OpenNode(L"root/body/vehispara/enginepower"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strenginepower = strContent.c_str();
						m_edRatedPower.SetWindowTextW(m_sIntVehInfo.strenginepower.c_str());
					}
					//17 usetype varchar2(3) 使用性质 GA24.3 
					if (xmlReader.OpenNode(L"root/body/vehispara/usetype"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strusetype = strContent.c_str();
					}
					//18 registerdate date 注册日期/初次登记日 期 按“YYYYMMDD”格式填写 (yyyy-mm-dd) 
					if (xmlReader.OpenNode(L"root/body/vehispara/registerdate"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strregisterdate = strContent.c_str();
					}
					//19 mdate date 出厂日期/制造日期 按“YYYYMMDD”格式填写 (yyyy-mm-dd) 
					if (xmlReader.OpenNode(L"root/body/vehispara/mdate"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strmdate = strContent.c_str();
						m_dtcProductionDate.SetWindowTextW(m_sIntVehInfo.strmdate.c_str());
					}
					//20 owner varchar2(150) 车主姓名/单位名称 
					if (xmlReader.OpenNode(L"root/body/vehispara/owner"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strowner = strContent.c_str();
						m_edOwner.SetWindowTextW(m_sIntVehInfo.strowner.c_str());
					}
					//21 lxdh varchar2(15) 车主电话 
					if (xmlReader.OpenNode(L"root/body/vehispara/lxdh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlxdh = strContent.c_str();
					}
					//22 lxdz varchar2(150) 车主地址 
					if (xmlReader.OpenNode(L"root/body/vehispara/lxdz"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlxdz = strContent.c_str();
					}
					//23 standard number(1) 排放标准阶段 附录 3.2 
					if (xmlReader.OpenNode(L"root/body/vehispara/standard"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strstandard = strContent.c_str();
					}
					//24 gear number(1) 变速器型式 附录 3.8 
					if (xmlReader.OpenNode(L"root/body/vehispara/gear"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strgear = strContent.c_str();
					}
					//25 airin number(1) 进气方式 1 自然进气 2 涡轮增压 
					if (xmlReader.OpenNode(L"root/body/vehispara/airin"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strairin = strContent.c_str();
						m_cbAirIntakeMode.SetWindowTextW(m_sIntVehInfo.strairin.c_str());
					}
					//26 fueltype varchar2(3) 燃油种类 可多选，每种按 GA24.9 
					if (xmlReader.OpenNode(L"root/body/vehispara/fueltype"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strfueltype = strContent.c_str();
						m_cbFuelType.SetWindowTextW(m_sIntVehInfo.strfueltype.c_str());
					}
					//27 fuelsupply number(2) 燃料供给系统型式 附录 3.9 
					if (xmlReader.OpenNode(L"root/body/vehispara/fuelsupply"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strfuelsupply = strContent.c_str();
						m_cbOilSupplyMode.SetWindowTextW(m_sIntVehInfo.strfuelsupply.c_str());
					}
					//28 drivemode varchar2(1) 驱动方式 1 前驱 2 后驱 3 四驱 4 全时四驱 
					if (xmlReader.OpenNode(L"root/body/vehispara/drivemode"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdrivemode = strContent.c_str();
						m_cbDriveType.SetWindowTextW(m_sIntVehInfo.strdrivemode.c_str());
					}
					//29 gvm number(6) 最大总质量 千克（kg） 
					if (xmlReader.OpenNode(L"root/body/vehispara/gvm"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strgvm = strContent.c_str();
						m_edMaximumTotalMass.SetWindowTextW(m_sIntVehInfo.strgvm.c_str());
					}
					//30 rm number(6) 基准质量 千克（kg） 
					if (xmlReader.OpenNode(L"root/body/vehispara/rm"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strrm = strContent.c_str();
					}
					//31 zbzl number(8) 整备质量 单位 kg 
					if (xmlReader.OpenNode(L"root/body/vehispara/zbzl"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strzbzl = strContent.c_str();
						m_edUnladenMass.SetWindowTextW(m_sIntVehInfo.strzbzl.c_str());
					}
					//32 egr varchar2(1) 是否有 EGR Y/N 表示有/无 
					if (xmlReader.OpenNode(L"root/body/vehispara/egr"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.stregr = strContent.c_str();
					}
					//33 hcl varchar2(1) 是否有后处理装置 Y/N 表示有/无 
					if (xmlReader.OpenNode(L"root/body/vehispara/hcl"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strhcl = strContent.c_str();
					}
					//34 hcltype varchar2(1) 后处理种类 附录 3.13 
					if (xmlReader.OpenNode(L"root/body/vehispara/hcltype"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strhcltype = strContent.c_str();
					}
					//35 dk varchar2(1) 是否电控 Y/N 表示有/无 
					if (xmlReader.OpenNode(L"root/body/vehispara/dk"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdk = strContent.c_str();
					}
					//36 odometer number(8) 里程表读数 公里 
					if (xmlReader.OpenNode(L"root/body/vehispara/odometer"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strodometer = strContent.c_str();
					}
					//37 cylinders number(2) 气缸数 
					if (xmlReader.OpenNode(L"root/body/vehispara/cylinders"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strcylinders = strContent.c_str();
					}
					//38 obd varchar2(1) 是否有 OBD Y/N 表示有/无 
					if (xmlReader.OpenNode(L"root/body/vehispara/obd"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strobd = strContent.c_str();
					}
					//39 gbwdzz varchar2(1) 是否能关闭车身稳定 装置(如 ESP) Y/N 表示有/无
					if (xmlReader.OpenNode(L"root/body/vehispara/gbwdzz"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strgbwdzz = strContent.c_str();
					}
					//40 zt varchar2(1) 机动车状态 按 GA24.17 
					if (xmlReader.OpenNode(L"root/body/vehispara/zt"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strzt = strContent.c_str();
					}
					//41 jdcxh varchar2(1) 机动车序号 按 GA 329.2 
					if (xmlReader.OpenNode(L"root/body/vehispara/jdcxh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strjdcxh = strContent.c_str();
					}
					//42 dczz number(8) 单车轴重 单位 kg 
					if (xmlReader.OpenNode(L"root/body/vehispara/dczz"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdczz = strContent.c_str();
					}
					//43 dpxh varchar2(20) 底盘型号 
					if (xmlReader.OpenNode(L"root/body/vehispara/dpxh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdpxh = strContent.c_str();
					}
					//44 qdltqy number(6,2) 驱动轮胎气压 kPa 
					if (xmlReader.OpenNode(L"root/body/vehispara/qdltqy"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strqdltqy = strContent.c_str();
					}
					//45 tg varchar2(1) 是否有燃油蒸发控制 装置 Y/N 表示有/无 
					if (xmlReader.OpenNode(L"root/body/vehispara/tg"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strtg = strContent.c_str();
					}
					//46 chzhq varchar2(2) 催化转化器 Y/N 表示有/无 
					if (xmlReader.OpenNode(L"root/body/vehispara/chzhq"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strchzhq = strContent.c_str();
						m_cbHasCatalyticConverter.SetWindowTextW(m_sIntVehInfo.strchzhq.c_str());
					}
					//47 rygg varchar2(5) 燃油规格 
					if (xmlReader.OpenNode(L"root/body/vehispara/rygg"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strrygg = strContent.c_str();
					}
					//48 sjcys number(4) 设计成员数 
					if (xmlReader.OpenNode(L"root/body/vehispara/sjcys"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strsjcys = strContent.c_str();
					}
					//49 ssxq varchar2(6) 所属辖区 地区编码 省+市+县(区)， 其它用-1 
					if (xmlReader.OpenNode(L"root/body/vehispara/ssxq"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strssxq = strContent.c_str();
					}
					//50 hbflbz char(1) 环保分类标志 
					if (xmlReader.OpenNode(L"root/body/vehispara/hbflbz"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strhbflbz = strContent.c_str();
					}
					//51 gb varchar2(2) 国别 代码（0-国产、1-进口、2- 合资） 
					if (xmlReader.OpenNode(L"root/body/vehispara/gb"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strgb = strContent.c_str();
					}
					//52 sfjrcz varchar2(1) 是否进入城镇 0 否 1 是 
					if (xmlReader.OpenNode(L"root/body/vehispara/sfjrcz"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strsfjrcz = strContent.c_str();
					}
					//53 fdjccs number(8) 发动机冲程数 
					if (xmlReader.OpenNode(L"root/body/vehispara/fdjccs"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strfdjccs = strContent.c_str();
					}
					//54 dws number(4) 档位数 
					if (xmlReader.OpenNode(L"root/body/vehispara/dws"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdws = strContent.c_str();
					}
					//55 yxrq date 有效日期 
					if (xmlReader.OpenNode(L"root/body/vehispara/yxrq"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.stryxrq = strContent.c_str();
					}
					//56 testtype varchar2(1) 检测方法 附录 3.1 
					if (xmlReader.OpenNode(L"root/body/vehispara/yxrq"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.stryxrq = strContent.c_str();
					}
					//57 tsno varchar2(15) 检验机构编号 
					if (xmlReader.OpenNode(L"root/body/vehispara/tsno"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strtsno = strContent.c_str();
					}
					//58 dlsj date 登录时间 
					if (xmlReader.OpenNode(L"root/body/vehispara/dlsj"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdlsj = strContent.c_str();
						m_dtcRegistrationDate.SetWindowTextW(m_sIntVehInfo.strdlsj.c_str());
					}
					//59 jclx char(1) 检测类型 附录 3.19 
					if (xmlReader.OpenNode(L"root/body/vehispara/jclx"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strjclx = strContent.c_str();
					}
					//60 jclry varchar2(50) 检测录入员 
					if (xmlReader.OpenNode(L"root/body/vehispara/jclry"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strjclry = strContent.c_str();
					}
					//61 pfpdyj varchar2(600) 排放判定依据 
					if (xmlReader.OpenNode(L"root/body/vehispara/pfpdyj"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strpfpdyj = strContent.c_str();
					}
					//62 rllb varchar2(3) 燃料类别 1：燃油； 2：单一燃料 3：两用燃料 4：混合动力电动 
					if (xmlReader.OpenNode(L"root/body/vehispara/rllb"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strrllb = strContent.c_str();
					}
					//63 fueltype1 varchar2(3) 燃料种类 2 GA24.9
					if (xmlReader.OpenNode(L"root/body/vehispara/fueltype1"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strfueltype1 = strContent.c_str();
					}
					//64 ddjxh varchar2(100) 电动机型号 
					if (xmlReader.OpenNode(L"root/body/vehispara/ddjxh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strddjxh = strContent.c_str();
					}
					//65 cnzzxh varchar2(100) 储能装置型号 
					if (xmlReader.OpenNode(L"root/body/vehispara/cnzzxh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strcnzzxh = strContent.c_str();
					}
					//66 dcrl number(6,2) 电池容量 kwh 
					if (xmlReader.OpenNode(L"root/body/vehispara/dcrl"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strdcrl = strContent.c_str();
					}
					//67 chzhqxh varchar2(100) 催化转化器型号 
					if (xmlReader.OpenNode(L"root/body/vehispara/chzhqxh"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strchzhqxh = strContent.c_str();
					}
					//68 lambdadown number(6,2) 过量空气系数限值下 限 
					if (xmlReader.OpenNode(L"root/body/vehispara/lambdadown"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlambdadown = strContent.c_str();
					}
					//69 lambdaup number(6,2) 过量空气系数限值上 限 
					if (xmlReader.OpenNode(L"root/body/vehispara/lambdaup"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strlambdaup = strContent.c_str();
					}
					//70 hclmodel varchar(30) 后处理装置型号
					if (xmlReader.OpenNode(L"root/body/vehispara/hclmodel"))
					{
						xmlReader.GetNodeContent(strContent);
						m_sIntVehInfo.strhclmodel = strContent.c_str();
					}
				}
				else
				{
					strMsg.Format(L"获取车辆信息返回:%s %s", strCode.c_str(), strContent.c_str());
					OperationHint(strMsg);
					return;
				}
			}
			else
			{
				strMsg.Format(L"获取车辆信息返回:%d %s", nRet, L"解析失败");
				OperationHint(strMsg);
				return;
			}
		}
		else
		{
			strMsg.Format(L"获取车辆信息返回:%d %s", nRet, L"接口访问失败");
			OperationHint(strMsg);
			return;
		}

	}
	else
	{
		InitVehicleInfoRegion();
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	}
	EndWaitCursor();
}
