
// YJOBDTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YJOBDT.h"
#include "YJOBDTDlg.h"
#include "afxdialogex.h"

#include "OBDTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYJOBDTDlg 对话框




CYJOBDTDlg::CYJOBDTDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CYJOBDTDlg::IDD, pParent)
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

CYJOBDTDlg::~CYJOBDTDlg()
{
	// 删除字体
	m_fontDlgFont.DeleteObject();

	CNHSQLServerDBO::CloseDB(m_pConnection);
	if (m_pConnection != NULL)
	{
		m_pConnection = NULL;
	}
}

void CYJOBDTDlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(CYJOBDTDlg, CDialogZoom)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CYJOBDTDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CYJOBDTDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CYJOBDTDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_UP, &CYJOBDTDlg::OnBnClickedButtonLiftBeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_DOWN, &CYJOBDTDlg::OnBnClickedButtonLiftBeamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CYJOBDTDlg::OnNMDblclkLstVehicle)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CYJOBDTDlg 消息处理程序

BOOL CYJOBDTDlg::OnInitDialog()
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

void CYJOBDTDlg::OnPaint()
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
HCURSOR CYJOBDTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYJOBDTDlg::OperationHint(const CStringW& strOH)
{
	m_lbDynInfo.SetWindowTextW(strOH);
}


void CYJOBDTDlg::OnBnClickedButtonUpdateVehicleList()
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


void CYJOBDTDlg::OnBnClickedBtnNext()
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

	dlg.m_bIsOBDRealTime = m_chkUpOBDRealTime.GetCheck();
	dlg.m_sTestLog = m_sTestLog;
	dlg.m_sVehicleInfo = m_sVehicleInfo;

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


void CYJOBDTDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}


void CYJOBDTDlg::OnBnClickedButtonLiftBeamUp()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CYJOBDTDlg::OnBnClickedButtonLiftBeamDown()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CYJOBDTDlg::OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	SelectVehicle();

	*pResult = 0;
}


void CYJOBDTDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_fontVehicleLCFont.DeleteObject();
	CNHSQLServerDBO::CloseDB(m_pConnection);
}

void CYJOBDTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogZoom::OnTimer(nIDEvent);
}

// 连接数据库
DWORD CYJOBDTDlg::InitDatabase(void)
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

void CYJOBDTDlg::InitCtrls(void)
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

void CYJOBDTDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CYJOBDTDlg::InitVehicleList(void)
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
	m_lstVehicle.InsertColumn(2, L"车牌号码",	0,	150);
	m_lstVehicle.InsertColumn(3, L"车牌类型",	0,	120);
	m_lstVehicle.InsertColumn(4, L"检测类型",	0,	120);
	m_lstVehicle.InsertColumn(5, L"报告单编号",	0,	300);
	m_lstVehicle.InsertColumn(6, L"OBD结果",	0,	100);
	m_lstVehicle.InsertColumn(7, L"OBD检测状态",	0,	100);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CYJOBDTDlg::InitVehicleInfoRegion(BOOL Enable)
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

void CYJOBDTDlg::InitTestTypeCombobox(void)
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

void CYJOBDTDlg::InitDriverCombobox(void)
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

void CYJOBDTDlg::InitComboboxPlateType(void)
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

void CYJOBDTDlg::InitComboboxDriveType(void)
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

void CYJOBDTDlg::InitComboboxFuelType(void)
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

void CYJOBDTDlg::InitComboboxOilSupplyMode(void)
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

void CYJOBDTDlg::InitComboboxAirIntakeMode(void)
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

void CYJOBDTDlg::InitEquipmentPrepareRegion(void)
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

void CYJOBDTDlg::LoadGasolineVehicleEquipmentPrepare(void)
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

void CYJOBDTDlg::LoadDieselVehicleEquipmentPrepare(void)
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

void CYJOBDTDlg::UpdateVehicleList(short const siTestType/* = 0*/, CString strFilter/* = L""*/)
{
	// 删除列表数据
	m_lstVehicle.DeleteAllItems();

	CString strSQL;
	strSQL.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
	strSQL.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
	strSQL.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");
	//strSQL.AppendFormat(L" where (TestLog.ItemOBD = '4' or TestLog.ItemOBD = '1') and (ResultOfOBD.Judge = '2'/* or ResultOfOBD.Judge = '1'*/)");

	// 增加筛选
	if (!strFilter.IsEmpty())
	{
		strSQL.AppendFormat(L" and PlateNumber like '%%%s%%'", strFilter);
	}

	_RecordsetPtr pRecordset(NULL);
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSQL))
	{
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
				int nItemIndex = m_lstVehicle.InsertItem(i, str);

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
				m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);

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
				m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);

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
				m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);

				// 检测类型
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("TestType"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					n =  v;
				}
				else
				{
					n = 0;
				}
				switch(n)
				{
				case 1:{str = L"稳态";} break;
				case 2:{str = L"简易瞬态";} break;
				case 3:{str = L"加载减速";} break;
				case 4:{str = L"双怠速";} break;
				case 5:{str = L"不透光自由加速";} break;
				case 6:{str = L"滤纸式自由加速";} break;
				case 7:{str = L"农用车自由加速";} break;
				case 8:{str = L"摩托车怠速";} break;
				case 9:{str = L"摩托车双怠速";} break;
				default:{str = L"错误类型";} break;
				}
				m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);

				// 报告单编号
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("ReportNumber"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					str =  (LPCTSTR)(_bstr_t)v;
				}
				else
				{
					str = L"";
				}
				m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

				// OBD结果
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("Judge"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					n =  v;
				}
				else
				{
					n = 99;
				}
				switch(n)
				{
				case 0:{str = L"未判定";} break;
				case 1:{str = L"合格";} break;
				case 2:{str = L"不合格";} break;
				default:{str = L"结果未知";} break;
				}
				m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);

				// OBD检测状态
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("ItemOBD"));
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					n =  v;
				}
				else
				{
					n = 99;
				}
				switch(n)
				{
				case 1:{str = L"待检";} break;
				case 4:{str = L"完成";} break;
				default:{str = L"不检测";} break;
				}
				m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, str, 0, 0, 0, 0);

				i++;
				pRecordset->MoveNext();

			}

			OperationHint(L"双击需要的车牌号码");
		}
		catch (_com_error& e)
		{
			CNHSQLServerDBO::OutputDBErrMsg(e);
		}
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"查询数据为空或其他问题\r\n数据库查询语句:%s", strSQL);
		OperationHint(strMsg);
	}
	CNHSQLServerDBO::CloseQuery(pRecordset);

}

void CYJOBDTDlg::SelectVehicle(void)
{
	GetVehicleInfo();
}

void CYJOBDTDlg::GetVehicleInfo(void)
{
	BeginWaitCursor();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(nIndex!=-1)
	{
		CString strRunningNumber,strPlateNumber,strPlateType;

		strPlateNumber = m_lstVehicle.GetItemText(nIndex,2);
		strPlateType = m_lstVehicle.GetItemText(nIndex,3);
		m_strCurrentTestType = m_lstVehicle.GetItemText(nIndex,4);
		strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		m_strCurrentRunningNumber = strRunningNumber;

		// 显示信息
		wchar_t wchSql[SQLMAX];
		TESTLOG sTestLog;
		wsprintf(wchSql, L"select * from TestLog where RunningNumber = '%s'", m_strCurrentRunningNumber);
		GetDboTestLog(wchSql, &sTestLog);
		m_sTestLog = sTestLog;
		// 获取车辆信息
		CString strSql;
		strSql.Format(L"select * from VehicleInfo where PlateNumber = '%s' and PlateType = '%s'", strPlateNumber, strPlateType);
		VEHICLEINFO SVehicleInfo;
		GetDboVehicleInfo(strSql, &SVehicleInfo);
		m_sVehicleInfo = SVehicleInfo;

		CString str(L"");
		m_cbTestType.SetWindowTextW(m_strCurrentTestType);
		m_edPlateNumber.SetWindowTextW(SVehicleInfo.wchPlateNumber);
		m_edOwner.SetWindowTextW(SVehicleInfo.wchOwner);
		m_cbPlateType.SetWindowTextW(SVehicleInfo.wchPlateType);
		m_edVIN.SetWindowTextW(SVehicleInfo.wchVIN);
		m_edModel.SetWindowTextW(SVehicleInfo.wchModel);
		COleDateTime odtProductionDate;
		odtProductionDate.ParseDateTime(SVehicleInfo.wchProductionDate);
		m_dtcProductionDate.SetTime(odtProductionDate);
		COleDateTime odtRegistrationDate;
		odtRegistrationDate.ParseDateTime(SVehicleInfo.wchRegistrationDate);
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
