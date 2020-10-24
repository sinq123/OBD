
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

	DDX_Control(pDX, IDC_ED_TOKEN, m_edToken);
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

	SetTimer(1, 5000, NULL);
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
}


void CATDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
#ifndef _DEBUG
	m_cbDriver.GetWindowTextW(m_strDriver);
	if (L"" == m_strDriver)
	{
		AfxMessageBox(L"请选择引车员");
		return;
	}

	if(!GetToken())
	{
		OperationHint(L"请输入用户令牌");
		return;
	}

#endif

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
	KillTimer(1);
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

	SetTimer(1, 5000, NULL);
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

	switch (nIDEvent)
	{
	case 1:
		{
			theApp.GetKey();
			if (!theApp.m_strKey.IsEmpty())
			{
				m_edToken.SetWindowTextW(theApp.m_strKey);
			}
		}
		break;
	}

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

	m_edToken.SetWindowTextW(theApp.m_strKey);

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
	m_lstVehicle.InsertColumn(1, L"号牌号码",	0,	320*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(2, L"号牌种类",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(3, L"检测流水号",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(4, L"检测状态",		30,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(5, L"检测时间",		30,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(6, L"检测项目ID",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(7, L"未完成项目",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(8, L"检验类型",	0,	120*nSM_CXSCREEN/1280);

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

	//CString strSql;
	//strSql.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
	//strSql.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
	//strSql.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");

	//// 增加筛选
	//if (!strFilter.IsEmpty())
	//{
	//	strSql.AppendFormat(L" and TestLog.PlateNumber like '%%%s%%'", strFilter);
	//}
	//// 检测方法分
	//switch(siTestType)
	//{
	//case 0:
	//	{
	//		CStringW strSqlTestType(L" and (");
	//		CStringW str(L"");
	//		bool b(true);
	//		if (L'1' == m_sIniNHClient.wchSystemType[0])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '1'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '1'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[1])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '2'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '2'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[2])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '3'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '3'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[3])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '4'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '4'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[4])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '5'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '5'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[5])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '6'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '6'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[6])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '7'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '7'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[7])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '8'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '8'";
	//			}
	//			strSqlTestType += str;
	//		}
	//		if (L'1' == m_sIniNHClient.wchSystemType[8])
	//		{
	//			if (b)
	//			{
	//				str = L"TestType = '9'";
	//				b = false;
	//			}
	//			else
	//			{
	//				str = L"or TestType = '9'";
	//			}
	//			strSqlTestType += str;
	//		}

	//		strSqlTestType += L" )";
	//		strSql.AppendFormat(L" %s", strSqlTestType);
	//	}
	//	break;
	//default:
	//	{
	//		strSql.AppendFormat(L" and TestLog.TestType = '%d'", siTestType);
	//	}
	//	break;
	//}

	//DWORD dwDBORet(0x00);
	//_RecordsetPtr pRecordset(NULL);
	//if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSql))
	//{ // 打开查询成功

	//	try
	//	{
	//		int i(0);
	//		while(!pRecordset->adoEOF)
	//		{
	//			int n(0);
	//			CString str;
	//			_variant_t v;

	//			// 序号
	//			str.Format(L"%d", i+1);
	//			//int nItemIndex = m_lstVehicle.InsertItem(LVIF_TEXT | LVIF_PARAM,i, str, 0, 0, 0, 0);            
	//			int nItemIndex = m_lstVehicle.InsertItem(i, str);

	//			// 车牌号码
	//			v.ChangeType(VT_NULL);
	//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateNumber"));
	//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	//			{
	//				str =  (LPCTSTR)(_bstr_t)v;
	//			}
	//			else
	//			{
	//				str = L"";
	//			}
	//			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);

	//			// 号牌种类
	//			v.ChangeType(VT_NULL);
	//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateType"));
	//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	//			{
	//				str =  (LPCTSTR)(_bstr_t)v;
	//			}
	//			else
	//			{
	//				str = L"";
	//			}
	//			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);

	//			// 检测类型
	//			v.ChangeType(VT_NULL);
	//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("TestType"));
	//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	//			{
	//				n =  v;

	//				if (6 == n)
	//				{
	//					// FSUNHT代替FSUYD(begin)
	//					CSimpleIni si(m_wchPathOfNHClient);
	//					CStringW str = si.GetString(L"FSUYDParams", L"UseFSUNHTInsteadOfFSUYD", L"0");
	//					if (L"1" == str)
	//					{
	//						n = 5;
	//					}
	//					// FSUNHT代替FSUYD(end)
	//				}
	//			}
	//			else
	//			{
	//				n = 0;
	//			}
	//			switch(n)
	//			{
	//			case 1:
	//				{
	//					str = L"稳态";
	//				}
	//				break;
	//			case 2:
	//				{
	//					str = L"简易瞬态";
	//				}
	//				break;
	//			case 3:
	//				{
	//					str = L"加载减速";
	//				}
	//				break;
	//			case 4:
	//				{
	//					str = L"双怠速";
	//				}
	//				break;
	//			case 5:
	//				{
	//					str = L"不透光自由加速";
	//				}
	//				break;
	//			case 6:
	//				{
	//					str = L"滤纸式自由加速";
	//				}
	//				break;
	//			case 7:
	//				{
	//					str = L"农用车自由加速";
	//				}
	//				break;
	//			case 8:
	//				{
	//					str = L"摩托车怠速";
	//				}
	//				break;
	//			case 9:
	//				{
	//					str = L"摩托车双怠速";
	//				}
	//				break;
	//			default:
	//				{
	//					str = L"错误类型";
	//				}
	//				break;
	//			}
	//			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);

	//			// 流水号
	//			v.ChangeType(VT_NULL);
	//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("RunningNumber"));
	//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	//			{
	//				str =  (LPCTSTR)(_bstr_t)v;
	//			}
	//			else
	//			{
	//				str = L"";
	//			}
	//			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);

	//			// 是否复检
	//			v.ChangeType(VT_NULL);
	//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("IsRetest"));
	//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
	//			{
	//				bool b =  (bool)v;
	//				if (b)
	//				{
	//					str = L"复检";
	//				}
	//				else
	//				{
	//					str = L"初检";
	//				}
	//			}
	//			else
	//			{
	//				str = L"";
	//			}
	//			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

	//			i++;
	//			pRecordset->MoveNext();
	//		}
	//	}
	//	catch (_com_error& e)
	//	{
	//		CNHSQLServerDBO::OutputDBErrMsg(e);
	//		dwDBORet = 0xFFFFFFFF;

	//		OperationHint(L"更新列表错误");
	//	}
	//}

	//if (dwDBORet != 0xFFFFFFFF)
	//{
	//	OperationHint(L"请双击车牌");
	//}

	//CNHSQLServerDBO::CloseQuery(pRecordset);



	CString strTemp;
	SHGBYMSG sHGMsg;
	std::wstring strRet;
	int nRet;
	// 获取联网待检列表
	// 先获取令牌
#ifndef _DEBUG
	if(!GetToken())
	{
		OperationHint(L"请输入用户令牌");
		return;
	}
#endif

	CStringW strData;
	strData.AppendFormat(L"<Params>");
	strData.AppendFormat(L"<szToken>%s</szToken>", theApp.m_strKey);
	strData.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", theApp.m_strLineNum);
	strData.AppendFormat(L"<nInspectType>%s</nInspectType>", L"0");
	strData.AppendFormat(L"<nIncrement>%s</nIncrement>", L"0");
	strData.AppendFormat(L"</Params>");


#ifdef _DEBUG
	nRet = 0;
	strRet = L"";
	strRet = L"<response><head><code>0</code><message>数据保存成功</message></head>";
	strRet = strRet + L"<body><VEHICLELST>";
	strRet = strRet + L"<VEHICLE AUTO_ID=\"1129925\" AUT_PLATE=\"粤QGN509\" AUT_PCLASS=\"蓝\" DETECT_ID=\"5503013\" DET_STATUS=\"0\" DET_DATE=\"2020/10/12 16:36:48\" INSPECT_ID=\"ASM\" UNDO_ITEMS=\"V,G,O,\" />";
	strRet = strRet + L"<VEHICLE AUTO_ID=\"1129928\" AUT_PLATE=\"粤C72J81\" AUT_PCLASS=\"蓝\" DETECT_ID=\"5503007\" DET_STATUS=\"0\" DET_DATE=\"2020/10/12 16:34:54\" INSPECT_ID=\"ASM\" UNDO_ITEMS=\"V,G,\" />";
	strRet = strRet + L"</VEHICLELST></body></response>";
#else


	nRet = CHGBYWebLib_API::RSInfo(theApp.m_pchURL, strRet, sHGMsg, L"GetJobList", strData.GetString());
#endif
	// 重新解析一次
	std::list<SGetJobList> lsGetJobList;
	CXmlReader xmlReader;
	if (nRet == 0)
	{
		if (sHGMsg.strcode.empty())
		{
			if (xmlReader.Parse(strRet.c_str()))
			{
				if (xmlReader.OpenNode(L"response/head/code"))
				{
					xmlReader.GetNodeContent(sHGMsg.strcode);
				}

				if (xmlReader.OpenNode(L"response/head/message"))
				{
					xmlReader.GetNodeContent(sHGMsg.strmessage);
				}

				CString strBody = strRet.c_str();
				//strBody = strBody.MakeLower();
				int nBegin = strBody.Find(L"<body>");
				int nEnd = strBody.Find(L"</body>");
				sHGMsg.strbody = strBody.Mid(nBegin+6, nEnd-nBegin-6);
			}
		}

		if (_wtoi(sHGMsg.strcode.c_str()) > -1)
		{
			if (xmlReader.Parse(sHGMsg.strbody.c_str()))
			{
				if (xmlReader.OpenNode(L"VEHICLELST/VEHICLE"))
				{
					std::wstring strNodeName, strName, strContent;
					do
					{
						SGetJobList sGetJobList;
						xmlReader.EnterNode();
						//AUT_PLATE	String	号牌号码
						xmlReader.GetNodeAttribute(L"AUT_PLATE", sGetJobList.strAUT_PLATE);
						//AUT_PCLASS	String	号牌种类
						xmlReader.GetNodeAttribute(L"AUT_PCLASS", sGetJobList.strAUT_PCLASS);
						//DETECT_ID	Long	检测流水号
						xmlReader.GetNodeAttribute(L"DETECT_ID", sGetJobList.strDETECT_ID);
						//DET_STATUS	Int	检测状态
						xmlReader.GetNodeAttribute(L"DET_STATUS", sGetJobList.strDET_STATUS);
						//DET_DATE	String	检测时间
						xmlReader.GetNodeAttribute(L"DET_DATE", sGetJobList.strDET_DATE);
						//INSPECT_ID	String	检测项目ID ASM  稳态工况法；LUGD 加载减速法；TSIC 双怠速法；FRAO自由加速-不透光烟度计法；FRAF 自由加速-滤纸烟度法；
						xmlReader.GetNodeAttribute(L"INSPECT_ID", sGetJobList.strINSPECT_ID);
						//UNDO_ITEMS	String	未完成项目，以”,”分开,如“V,O,G”V 外观检测O OBD检测G 尾气检测
						xmlReader.GetNodeAttribute(L"UNDO_ITEMS", sGetJobList.strUNDO_ITEMS);
						//TESTCATEGORY	String	检验类型,1- 定期检验、2-注册登记检验、3-实验比对 4- 监督性抽检、5- 变更登记和转移登记检验
						xmlReader.GetNodeAttribute(L"TESTCATEGORY", sGetJobList.strTESTCATEGORY);

						lsGetJobList.push_back(sGetJobList);
					}
					while (xmlReader.NextSibling());
				}
			}
		}
		else
		{
			strTemp.Format(L"获取车辆列表错误 代号:%s,信息:%s", sHGMsg.strcode.c_str(), sHGMsg.strmessage.c_str());
			OperationHint(strTemp);
			return;
		}

		int nItem(0);
		if (lsGetJobList.size() > 0)
		{
			int nItem(0);
			for (list<SGetJobList>::iterator ir=lsGetJobList.begin(); ir!=lsGetJobList.end(); ++ir)
			{
				CString str;
				// 序号
				str.Format(L"%d", nItem+1);
				int nItemIndex = m_lstVehicle.InsertItem(nItem, str);
				//AUT_PLATE	String	号牌号码
				m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strAUT_PLATE.c_str(), 0, 0, 0, 0);
				//AUT_PCLASS	String	号牌种类
				m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strAUT_PCLASS.c_str(), 0, 0, 0, 0);
				//DETECT_ID	Long	检测流水号
				m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strDETECT_ID.c_str(), 0, 0, 0, 0);
				//DET_STATUS	Int	检测状态
				m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strDET_STATUS.c_str(), 0, 0, 0, 0);
				//DET_DATE	String	检测时间
				m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strDET_DATE.c_str(), 0, 0, 0, 0);
				//INSPECT_ID	String	检测项目IDASM  稳态工况法；LUGD 加载减速法；TSIC 双怠速法；FRAO自由加速-不透光烟度计法；FRAF 自由加速-滤纸烟度法；
				m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, ir->strINSPECT_ID.c_str(), 0, 0, 0, 0);
				//UNDO_ITEMS	String	未完成项目，以”,”分开,如“V,O,G”V 外观检测O OBD检测G 尾气检测
				m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, ir->strUNDO_ITEMS.c_str(), 0, 0, 0, 0);
				//TESTCATEGORY	String	检验类型,1- 定期检验、2-注册登记检验、3-实验比对 4- 监督性抽检、5- 变更登记和转移登记检验
				m_lstVehicle.SetItem (nItemIndex, 8, LVIF_TEXT, ir->strTESTCATEGORY.c_str(), 0, 0, 0, 0);

			}

			OperationHint(L"请双击车牌");
		}
		else
		{
			OperationHint(L"获取列表为空");
			return;
		}
	}
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
		m_strCurrentTestType = m_lstVehicle.GetItemText(nIndex,6);
		strRunningNumber = m_lstVehicle.GetItemText(nIndex,3);

		m_strCurrentRunningNumber = strRunningNumber;

		//// 获取开单信息
		//wchar_t wchSql[SQLMAX];
		//TESTLOG sTestLog;
		//wsprintf(wchSql, L"select * from TestLog where RunningNumber = '%s'", m_strCurrentRunningNumber);
		//GetDboTestLog(wchSql, &sTestLog);

		//// 获取车辆信息
		//CString strSql;
		//strSql.Format(L"select * from VehicleInfo where PlateNumber = '%s' and PlateType = '%s'", strPlateNumber, strPlateType);
		//VEHICLEINFO SVehicleInfo;
		//GetDboVehicleInfo(strSql, &SVehicleInfo);

		//if (wcscmp(sTestLog.wchItemApp, L"") != 0)
		//{
		//	switch (_wtoi(sTestLog.wchItemApp))
		//	{
		//	case 0:	m_edItemApp.SetWindowText(L"免除检测");	break;
		//	case 1:	m_edItemApp.SetWindowText(L"等待检测");	break;
		//	case 2:	m_edItemApp.SetWindowText(L"准备检测");	break;
		//	case 3:	m_edItemApp.SetWindowText(L"正在检测");	break;
		//	case 4:	m_edItemApp.SetWindowText(L"完成检测");	break;
		//	default:	m_edItemApp.SetWindowText(sTestLog.wchItemApp);	break;
		//	}
		//}
		//else
		//{
		//	m_edItemApp.SetWindowText(L"--");
		//}

		//if (wcscmp(sTestLog.wchItemOBD, L"") != 0)
		//{
		//	switch (_wtoi(sTestLog.wchItemOBD))
		//	{
		//	case 0:	m_edItemOBD.SetWindowText(L"免除检测");	break;
		//	case 1:	m_edItemOBD.SetWindowText(L"等待检测");	break;
		//	case 2:	m_edItemOBD.SetWindowText(L"准备检测");	break;
		//	case 3:	m_edItemOBD.SetWindowText(L"正在检测");	break;
		//	case 4:	m_edItemOBD.SetWindowText(L"完成检测");	break;
		//	default:	m_edItemOBD.SetWindowText(sTestLog.wchItemOBD);	break;
		//	}
		//}
		//else
		//{
		//	m_edItemOBD.SetWindowText(L"--");
		//}

		//if (wcscmp(sTestLog.wchItemEmi, L"") != 0)
		//{
		//	switch (_wtoi(sTestLog.wchItemEmi))
		//	{
		//	case 0:	m_edItemEmi.SetWindowText(L"免除检测");	break;
		//	case 1:	m_edItemEmi.SetWindowText(L"等待检测");	break;
		//	case 2:	m_edItemEmi.SetWindowText(L"准备检测");	break;
		//	case 3:	m_edItemEmi.SetWindowText(L"正在检测");	break;
		//	case 4:	m_edItemEmi.SetWindowText(L"完成检测");	break;
		//	default:	m_edItemEmi.SetWindowText(sTestLog.wchItemEmi);	break;
		//	}
		//}
		//else
		//{
		//	m_edItemEmi.SetWindowText(L"--");
		//}

		CString strTemp;
		SHGBYMSG sHGMsg;
		std::wstring strRet;
		int nRet;
		// 获取联网待检列表
		// 先获取令牌
#ifndef _DEBUG
		if(!GetToken())
		{
			OperationHint(L"请输入用户令牌");
			return;
		}
		m_cbDriver.GetWindowTextW(m_strDriver);
#endif
		CStringW strData;
		strData.AppendFormat(L"<Params>");
		strData.AppendFormat(L"<szToken>%s</szToken>", theApp.m_strKey);
		strData.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", theApp.m_strLineNum);
		strData.AppendFormat(L"<lDetectID>%s</lDetectID>", m_strCurrentRunningNumber);
		strData.AppendFormat(L"<szInspectorID>%s</szInspectorID>", theApp.m_strUserName);
		strData.AppendFormat(L"<szDriverID>%s</szDriverID>", m_strDriver);
		strData.AppendFormat(L"</Params>");
#ifdef _DEBUG
		nRet = 0;
		strRet.clear();
		strRet = L"<response><head><code>0</code><message>数据保存成功</message></head>";
		strRet += L"<body><InspectInfo>";
		strRet += L"<Vehicle DETECT_ID=\"5498878\" INSPECT_ID=\"ASM\" AUTO_ID=\"910587\" AUT_PLATE=\"粤GPS290\" AUT_PCLASS=\"蓝\" AUT_VIN=\"LGBK22E55CY861062\" ";
		strRet += L"AUT_CHASSISNO=\"LGBK22E55CY861062\" AUT_ENGINENO=\"123681W\" AUT_ONAME=\"廖官龙\" AUT_OTELE=\"6520111\" AUT_OADDR=\"廉江市西街190917号\" ";
		strRet += L"AUT_ODOMETER=\"10000\" AUT_DMANUF=\"2012/6/14 0:00:00\" AUT_DSERVI=\"2012/10/17 0:00:00\" MOD_ID=\"0\" AUT_BRAND=\"骊威牌\" AUT_MANUFACTORY=\"\" ";
		strRet += L"AUT_VCLASS=\"小型轿车\" AUT_MASSRD=\"1140\" AUT_MASSTT=\"1560\" AUT_MASSLD=\"420\" AUT_PASSENGER=\"5\" AUT_PATTERN=\"后驱\" AUT_SHIFTP=\"自动\" ";
		strRet += L"AUT_GEAR=\"5\" AUT_ENGINEMODEL=\"HR16\" AUT_ENGINEMANUFACTORY=\"0\" AUT_FUEL=\"汽油\" AUT_FUELST=\"闭环电喷\" AUT_VOLUME=\"1.6\" AUT_EPOWER=\"77.0\" ";
		strRet += L"AUT_REV=\"2000\" AUT_CYLNDS=\"2\" AUT_GASST=\"自然吸气\" AUT_IS_NEW=\"0\" AUT_ENABLE=\"1\" RGN_ID=\"\" AUT_MODEL=\"DFL7163BC\" AUT_TWC=\"0\" AUT_USAGE=\"非营运\" ";
		strRet += L"AUT_USAGECLASS=\"其它用途\" AUT_SCRAP=\"0\" AUT_STDID=\"4\" AUT_LABELTYPE=\"绿标\" AUT_LABELVALIDATE=\"一年\" AUT_LABELNEXTDATE=\"2021/10/31 0:00:00\" AUT_IDESN=\"\" ";
		strRet += L"AUT_LOCAL=\"0\" AUT_TRANSFER=\"0\" AUT_RGNNAME=\"\" AUT_LABELAFTERNEXTDATE=\"\" AUT_LABELPREVVALIDATE=\"\" TRANSFER_OPERATOR=\"\" TRANSFER_DATETIME=\"\" TRANSFER_TOPLATE=\"\" AUT_LABELVALIDMONTH=\"\" ";
		strRet += L"AUT_BELIMIT=\"\" AUT_NOTLIMITREASON=\"\" TRANSFER_RECORD=\"\" AUT_MDATE=\"\" AUT_YZ=\"\" AUT_LASTLABELTYPE=\"\" AUT_HPZL=\"\" AUT_CLLX=\"\" AUT_SYXZ=\"\" AUT_RLZL=\"\" AUT_ZT=\"\" AUT_XH=\"\" AUT_XZQH=\"\" ";
		strRet += L"JJAUT_MODEL=\"\" JJAUT_ENGINEMODEL=\"\" JJAUT_DSERVI=\"\" JJAUT_FUEL=\"\" JJAUT_MASSTT=\"\" JJAUT_PASSENGER=\"\" JJAUT_LABELTYPE=\"\" AUT_OBD=\"True\" AUT_MOTORYMODEL=\"\" AUT_ENERGYSTRDMODEL=\"\" AUT_BATTERYCAP=\"\" ";
		strRet += L"AUT_TWCMODEL=\"\" AUT_SCR=\"False\" AUT_SCRMODEL=\"\" AUT_DPF=\"False\" AUT_DPFMODEL=\"\" AUT_OBD_OK=\"\" AUT_ETC=\"\" AUT_EGR=\"\" AUT_EVAP=\"\" AUT_REMOTETERMINAL=\"\" AUT_TRACTIONCONTROL=\"\" ASM_HCS5025=\"90.00\" ";
		strRet += L"ASM_COS5025=\"0.50\" ASM_NOS5025=\"700.00\" ASM_HCS2540=\"80.00\" ASM_COS2540=\"0.40\" ASM_NOS2540=\"650.00\" />";
		strRet += L"</InspectInfo></body></response>";
#else
		nRet = CHGBYWebLib_API::RSInfo(theApp.m_pchURL, strRet, sHGMsg, L"RequestJob", strData.GetString());
#endif
		bool bIsUpVeh(false);
		VEHICLEINFO SVehicleInfo;
		TESTLOG sTestLog;
		//
		if (nRet == 0)
		{
			CXmlReader xmlReader;
			if (sHGMsg.strcode.empty())
			{
				if (xmlReader.Parse(strRet.c_str()))
				{
					if (xmlReader.OpenNode(L"response/head/code"))
					{
						xmlReader.GetNodeContent(sHGMsg.strcode);
					}

					if (xmlReader.OpenNode(L"response/head/message"))
					{
						xmlReader.GetNodeContent(sHGMsg.strmessage);
					}

					CString strBody = strRet.c_str();
					//strBody = strBody.MakeLower();
					int nBegin = strBody.Find(L"<body>");
					int nEnd = strBody.Find(L"</body>");
					sHGMsg.strbody = strBody.Mid(nBegin+6, nEnd-nBegin-6);
				}
			}

			if (_wtoi(sHGMsg.strcode.c_str()) > -1)
			{
				bIsUpVeh = true;
				if (xmlReader.Parse(sHGMsg.strbody.c_str()))
				{
					if (xmlReader.OpenNode(L"InspectInfo/Vehicle"))
					{
						std::wstring strCon;
						//DETECT_ID	Long	检测流水号
						xmlReader.GetNodeAttribute(L"DETECT_ID", strCon);
						wcscpy_s(sTestLog.wchRunningNumber, strCon.c_str());
						//INSPECT_ID	String	检测项目ID//ASM  稳态工况法；//LUGD 加载减速法；//TSIC 双怠速法；//FRAO自由加速-不透光烟度计法；FRAF 自由加速-滤纸烟度法；
						xmlReader.GetNodeAttribute(L"INSPECT_ID", strCon);
						if (strCon.find(L"ASM") != std::string::npos){strCon = L"1"; strTemp = L"汽油";}
						else if (strCon.find(L"LUGD") != std::string::npos){strCon = L"3"; strTemp = L"柴油";}
						else if (strCon.find(L"TSIC") != std::string::npos){strCon = L"4"; strTemp = L"汽油";}
						else if (strCon.find(L"FRAO") != std::string::npos){strCon = L"5"; strTemp = L"柴油";}
						else if (strCon.find(L"FRAF") != std::string::npos){strCon = L"6"; strTemp = L"柴油";}
						wcscpy_s(sTestLog.wchTestType, strCon.c_str());
						
						//AUT_PLATE	String	号牌号码
						xmlReader.GetNodeAttribute(L"AUT_PLATE", strCon);
						wcscpy_s(sTestLog.wchPlateNumber, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchPlateNumber, strCon.c_str());
						//AUT_PCLASS	String	号牌种类
						xmlReader.GetNodeAttribute(L"AUT_PCLASS", strCon);
						wcscpy_s(sTestLog.wchPlateType, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchPlateType, strCon.c_str());
						//AUT_DSERVI	String	登记日期，格式：2010/1/1//注：新车无此字段
						xmlReader.GetNodeAttribute(L"AUT_DSERVI", strCon);
						wcscpy_s(sTestLog.wchRegistrationDate, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchRegistrationDate, strCon.c_str());
						//AUT_VIN	String	车辆识别号
						xmlReader.GetNodeAttribute(L"AUT_VIN", strCon);
						wcscpy_s(sTestLog.wchVIN, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchVIN, strCon.c_str());
						//AUT_VCLASS	String	车辆类别
						xmlReader.GetNodeAttribute(L"AUT_VCLASS", strCon);
						wcscpy_s(sTestLog.wchVehicleType, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchVehicleType, strCon.c_str());
						//AUT_PASSENGER	Integer	乘员数(单位：人）
						xmlReader.GetNodeAttribute(L"AUT_PASSENGER", strCon);
						wcscpy_s(sTestLog.wchRatedPassengerCapacity, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchRatedPassengerCapacity, strCon.c_str());
						//AUT_MASSRD	Integer	基准质量（单位：kg）
						xmlReader.GetNodeAttribute(L"AUT_MASSRD", strCon);
						wcscpy_s(sTestLog.wchUnladenMass, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchUnladenMass, strCon.c_str());
						//AUT_MASSTT	Integer	最大总质量（单位：kg）
						xmlReader.GetNodeAttribute(L"AUT_MASSTT", strCon);
						wcscpy_s(sTestLog.wchMaximumTotalMass, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchMaximumTotalMass, strCon.c_str());
						//AUT_FUEL	String	燃料类别
						xmlReader.GetNodeAttribute(L"AUT_FUEL", strCon);
						wcscpy_s(sTestLog.wchFuelType, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchFuelType, strCon.c_str());
						//AUT_FUELST	String	供油方式//开环电喷；闭环电喷；非电喷
						xmlReader.GetNodeAttribute(L"AUT_FUELST", strCon);
						wcscpy_s(sTestLog.wchOilSupplyMode, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchOilSupplyMode, strCon.c_str());
						//AUT_TWC	Integer	是否安装三元催化转换器//0: 没有；1：有
						xmlReader.GetNodeAttribute(L"AUT_TWC", strCon);
						wcscpy_s(sTestLog.wchHasCatalyticConverter, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchHasCatalyticConverter, strCon.c_str());
						//AUT_GASST	String	进气方式
						xmlReader.GetNodeAttribute(L"AUT_GASST", strCon);
						wcscpy_s(sTestLog.wchAirIntakeMode, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchAirIntakeMode, strCon.c_str());
						//AUT_EPOWER	Float	额定功率 kW
						xmlReader.GetNodeAttribute(L"AUT_EPOWER", strCon);
						wcscpy_s(sTestLog.wchRatedPower, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchRatedPower, strCon.c_str());
						//AUT_REV	Integer	额定转速 r/min
						xmlReader.GetNodeAttribute(L"AUT_REV", strCon);
						wcscpy_s(sTestLog.wchRatedRev, strCon.c_str());
						wcscpy_s(SVehicleInfo.wchRatedRev, strCon.c_str());
						//ASM_COS5025	Float	ASM5025工况CO限值
						//ASM_HCS5025	Float	ASM5025工况HC限值
						//ASM_NOS5025	Float	ASM5025工况NO限值
						//ASM_COS2540	Float	ASM2540工况CO限值
						//ASM_HCS2540	Float	ASM2540工况HC限值
						//ASM_NOS2540	Float	ASM2540工况NO限值
						//LUGD_KS	Float	加载减速法不透光系数限值 m-1
						//LUGD_NOxS	Float	加载减速法氮氧化合物限值
						//LUGD_MAXHPS	Float	轮边最大功率限值
						//TSIC_HCS_H	Float	双怠速法高怠速工况HC限值
						//TSIC_COS_H	Float	双怠速法高怠速工况CO限值
						//TSIC_HCS_L	Float	双怠速法怠速工况HC限值
						//TSIC_COS_L	Float	双怠速法怠速工况CO限值
						//TSIC_AQS_L	Float	双怠速法过量空气系数下线
						//TSIC_AQS_H	Float	双怠速法过量空气系数上线
						//FRAO_KS	Float	不透光系数限值
					}
				}
			}
			else
			{
				strTemp.Format(L"获取车辆信息错误 代号:%s,信息:%s", sHGMsg.strcode.c_str(), sHGMsg.strmessage.c_str());
				OperationHint(strTemp);
			}
		}

		if (bIsUpVeh)
		{
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

			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
#ifdef Up_OBD_RealTime
			OperationHint(L"确定信息正确->选着是否上传OBD过程数据->选择OBD操作员");
#else
			OperationHint(L"确定信息正确后->选择OBD操作员");
#endif
		}
		else
		{
			InitVehicleInfoRegion();

			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
		}
	}
	else
	{
		InitVehicleInfoRegion();

		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	}

	EndWaitCursor();
}

bool CATDlg::GetToken(void)
{
	theApp.m_strKey.Empty();
	m_edToken.GetWindowTextW(theApp.m_strKey);

	if (theApp.m_strKey.IsEmpty())
	{
		return false;
	}

	return true;
}
