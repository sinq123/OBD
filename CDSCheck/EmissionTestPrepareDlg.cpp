// EmissionTestPrepareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CDSCheck.h"
#include "EmissionTestPrepareDlg.h"
#include "afxdialogex.h"
#include "OBDTestDlg.h"

// CEmissionTestPrepareDlg 对话框

IMPLEMENT_DYNAMIC(CEmissionTestPrepareDlg, CDialogZoom)

CEmissionTestPrepareDlg::CEmissionTestPrepareDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CEmissionTestPrepareDlg::IDD, pParent)
	, m_pConnection(NULL)
	, m_siPrepareRegionIndex(0)
{
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

CEmissionTestPrepareDlg::~CEmissionTestPrepareDlg()
{
	// 删除字体
	m_fontDlgFont.DeleteObject();
	m_fontVehicleLCFont.DeleteObject();
}

void CEmissionTestPrepareDlg::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, IDC_ST_ACCEPTANCE_TIME, m_lbAcceptanceTime);
	DDX_Control(pDX, IDC_DATE_BEGIN, m_dtcAcceptanceStartTime);
	DDX_Control(pDX, IDC_DATE_END, m_dtcAcceptanceEndTime);
}


BEGIN_MESSAGE_MAP(CEmissionTestPrepareDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CHANGE_VEHICLE_INFO, &CEmissionTestPrepareDlg::OnBnClickedBtnChangeVehicleInfo)
	ON_BN_CLICKED(IDC_BTN_LAMBDA_LIMIT, &CEmissionTestPrepareDlg::OnBnClickedBtnLambdaLimit)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_UP, &CEmissionTestPrepareDlg::OnBnClickedButtonLiftBeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_BEAM_DOWN, &CEmissionTestPrepareDlg::OnBnClickedButtonLiftBeamDown)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CEmissionTestPrepareDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CEmissionTestPrepareDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CEmissionTestPrepareDlg::OnBnClickedButtonUpdateVehicleList)
	ON_EN_CHANGE(IDC_ED_FILTER, &CEmissionTestPrepareDlg::OnEnChangeEdFilter)
	ON_CBN_SELCHANGE(IDC_CB_FILTER_TEST_TYPE, &CEmissionTestPrepareDlg::OnCbnSelchangeCbFilterTestType)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CEmissionTestPrepareDlg::OnNMDblclkLstVehicle)
END_MESSAGE_MAP()


// CEmissionTestPrepareDlg 消息处理程序


BOOL CEmissionTestPrepareDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 连接数据库
	InitDatabase();

	// 初始化控件
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEmissionTestPrepareDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 5:
		{
			GetEPModeSimu();
		}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}


void CEmissionTestPrepareDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	KillTimer(5);

	// 关闭数据库连接
	UnInitDatabase();
}


void CEmissionTestPrepareDlg::OnBnClickedBtnChangeVehicleInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CEmissionTestPrepareDlg::OnBnClickedBtnLambdaLimit()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CEmissionTestPrepareDlg::OnBnClickedButtonLiftBeamUp()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CEmissionTestPrepareDlg::OnBnClickedButtonLiftBeamDown()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CEmissionTestPrepareDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码

	if (wcscmp(m_sTestLog.wchReportNumber, L"") == 0)
	{
		MessageBox(L"报告单编号为空", L"下一步", MB_ICONWARNING|MB_OK);
		return;
	}

	// 比对南华的App_Data是否和选的一直
	TESTLOG sTestIniLog;
	GetIniTestLog(&sTestIniLog);
	// 报告单编号 和 车牌号码
	if ((wcscmp(m_sTestLog.wchReportNumber, sTestIniLog.wchReportNumber) != 0)
		|| (wcscmp(m_sTestLog.wchPlateNumber, sTestIniLog.wchPlateNumber) != 0)
		)
	{
		MessageBox(L"选择的检测记录和南华记录的不一致", L"下一步", MB_ICONWARNING|MB_OK);
		return;
	}
	// 写入南华的日志编号，方便更新插入数据
	wcscpy_s(m_sTestLog.wchRunningNumber , sTestIniLog.wchRunningNumber);

	m_cbDriver.GetWindowTextW(m_strDriver);
	// 引车员
	if (m_strDriver.IsEmpty())
	{
		MessageBox(L"请选择引车员", L"下一步", MB_ICONWARNING|MB_OK);
		return;
	}
	// 写入引车员
	wcscpy_s(m_sTestLog.wchDriver , m_strDriver);
	

	COBDTestDlg dlg;

	dlg.m_pConnection = m_pConnection;
	dlg.m_sTestLog = m_sTestLog;
	dlg.m_sVehInfo = m_sVehInfo;

	dlg.DoModal();

	m_lstVehicle.GetItemState(-1, LVIS_SELECTED);
	m_lstVehicle.SetItemState(-1, 0, LVIS_SELECTED);

	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);

	m_lbDynInfo.SetWindowTextW(L"");
}


void CEmissionTestPrepareDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CEmissionTestPrepareDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	GetIntVehInfoList();
}


void CEmissionTestPrepareDlg::OnEnChangeEdFilter()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogZoom::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	int nIndex(-1);
	m_lstVehicle.GetItemState(nIndex, LVIS_SELECTED);
	m_lstVehicle.SetItemState(nIndex, 0, LVIS_SELECTED);

	CStringW strFilter;
	m_edFilter.GetWindowText(strFilter);

	int nItemCount = m_lstVehicle.GetItemCount();
	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		CString strItemText = m_lstVehicle.GetItemText(nIndex, 1);
		if (strItemText.Find(strFilter, 0) < 0)
		{
			m_lstVehicle.DeleteItem(nIndex);
			nIndex--;
			nItemCount = m_lstVehicle.GetItemCount();
		}
	}
}


void CEmissionTestPrepareDlg::OnCbnSelchangeCbFilterTestType()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex(-1);
	m_lstVehicle.GetItemState(nIndex, LVIS_SELECTED);
	m_lstVehicle.SetItemState(nIndex, 0, LVIS_SELECTED);

	CStringW strTestType;
	const int nCurSel = m_cbFilterTestType.GetCurSel();
	m_cbFilterTestType.GetLBText(nCurSel, strTestType);

	int nItemCount = m_lstVehicle.GetItemCount();
	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		CString strItemText = m_lstVehicle.GetItemText(nIndex, 2);
		if (strItemText.Find(strTestType, 0) < 0)
		{
			m_lstVehicle.DeleteItem(nIndex);
			nIndex--;
			nItemCount = m_lstVehicle.GetItemCount();
		}
	}
}

void CEmissionTestPrepareDlg::OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	GetIntVehInfo();

	*pResult = 0;
}

// 连接数据库
DWORD CEmissionTestPrepareDlg::InitDatabase(void)
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
	}

	return dwReturn;
}

// 关闭数据库连接
DWORD CEmissionTestPrepareDlg::UnInitDatabase(void)
{
	CNHSQLServerDBO::CloseDB(m_pConnection);

	return 0x00;
}

void CEmissionTestPrepareDlg::InitCtrls(void)
{
	// 设置字体
	SetDlgFont();

	// 标题栏
	m_lbHead.SetTitle(L"设备准备和车辆选择");

	// 获取检测线类型
	GetIniNHClient(&m_sIniNHClient);

	// 测功机信息提示
	m_lbDynInfo.SetTextColor(RGB(255, 0, 0));
	m_lbDynInfo.AutoFitFont();
	m_lbDynInfo.SetFontName(L"黑体");

	// 是否需要显示升降举升器按钮
	//if (L'1' == m_sIniNHClient.wchSystemType[0]
	//|| L'1' == m_sIniNHClient.wchSystemType[1]
	//|| L'1' == m_sIniNHClient.wchSystemType[2])
	{
		GetDlgItem(IDC_BUTTON_LIFT_BEAM_UP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LIFT_BEAM_DOWN)->ShowWindow(SW_SHOW);
		m_lbDynInfo.ShowWindow(SW_SHOW);
	}
	//else
	//{
	//	GetDlgItem(IDC_BUTTON_LIFT_BEAM_UP)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_BUTTON_LIFT_BEAM_DOWN)->ShowWindow(SW_HIDE);
	//	m_lbDynInfo.ShowWindow(SW_HIDE);
	//}

	InitVehicleList();
	InitVehicleInfoRegion(FALSE);
	InitTestTypeCombobox();
	InitDriverCombobox();
	InitComboboxPlateType();
	InitComboboxDriveType();
	InitComboboxFuelType();
	InitComboboxOilSupplyMode();
	InitComboboxAirIntakeMode();

	// 最大化显示
	ShowWindow(SW_MAXIMIZE);

	InitEquipmentPrepareRegion();

	//转速显示
	GetDlgItem(IDC_EDIT_RPM)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_RPM)->ShowWindow(TRUE);

	
	GetDlgItem(IDC_BUTTON_LIFT_BEAM_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LIFT_BEAM_DOWN)->EnableWindow(TRUE);

	// 就绪
	GetDlgItem(IDC_ED_EPPORTINFO)->ShowWindow(FALSE);
	m_lbEPPortInfo.SetText(_T("实时环境参数"));

	m_dtcAcceptanceStartTime.SetTime(COleDateTime::GetCurrentTime() - COleDateTimeSpan(3, 0, 0, 0));
	m_dtcAcceptanceEndTime.SetTime(COleDateTime::GetCurrentTime() + COleDateTimeSpan(4, 0, 0, 0));

	SetTimer(5, 1000, NULL);
}

void CEmissionTestPrepareDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CEmissionTestPrepareDlg::InitVehicleList(void)
{
	// 待检车列表
	m_lbVehicleList.SetTitle(L"待检车列表");

	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath))
	{
	}

	CSimpleIni si(wchPath);

	const CString strFontHeight = si.GetString(L"VehicleList", L"FontHeight", L"0");
	int nFontHeight(m_nDlgFontSize);
	if (L"0" != strFontHeight)
	{
		nFontHeight = _wtoi(strFontHeight);
		// 合理性约束
		if (nFontHeight > 1000)
		{
			nFontHeight = 1000;
		}
	}

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
	m_lstVehicle.InsertColumn(2, L"检测类型",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(3, L"流水号",		0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(4, L"检测次数",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(5, L"VIN",	0,	120*nSM_CXSCREEN/1280);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CEmissionTestPrepareDlg::InitVehicleInfoRegion(BOOL Enable)
{
	m_lbVehicleInfo.SetTitle(L"车辆信息");
	m_lbEParamInfo.SetTitle(L"环境参数");
	m_lbAcceptanceTime.SetTitle(L"受理时间");

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

void CEmissionTestPrepareDlg::InitTestTypeCombobox(void)
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

void CEmissionTestPrepareDlg::InitDriverCombobox(void)
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

void CEmissionTestPrepareDlg::InitComboboxPlateType(void)
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

void CEmissionTestPrepareDlg::InitComboboxDriveType(void)
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

void CEmissionTestPrepareDlg::InitComboboxFuelType(void)
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

void CEmissionTestPrepareDlg::InitComboboxOilSupplyMode(void)
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

void CEmissionTestPrepareDlg::InitComboboxAirIntakeMode(void)
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

void CEmissionTestPrepareDlg::InitEquipmentPrepareRegion(void)
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

void CEmissionTestPrepareDlg::LoadGasolineVehicleEquipmentPrepare(void)
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

void CEmissionTestPrepareDlg::LoadDieselVehicleEquipmentPrepare(void)
{
	m_cDieselVehicleEquipmentPrepareDlg.Create(CDieselVehicleEquipmentPrepareDlg::IDD, this);
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

void CEmissionTestPrepareDlg::GetEPModeSimu(void)
{
	int a;
	int b;
	int c;
	int d;
	//tp
	b=25;
	a=30;

	//rpm
	c=2400;
	d=2600;

	float fET=float(rand()%(a-b+1)+b);
	float fRH=float(rand()%(4*b-a+1)+a);
	float fAP=4*b+rand()%(a-b)/10.0f;
	float fOT=float(rand()%(2*a-2*b+1)+b*3+5);
	USHORT usRev=rand()%(d-c)+c;

	CString str(L"");
	
	str.Format(L"%.1f", fET);
	m_edET.SetWindowTextW(str);

	str.Format(L"%.1f", fRH);
	m_edRH.SetWindowTextW(str);

	str.Format(L"%.1f", fAP);
	m_edAP.SetWindowTextW(str);

	str.Format(L"%.1f", fOT);
	m_edOT.SetWindowTextW(str);

	str.Format(L"%d", usRev);
	m_edRPM.SetWindowTextW(str);
}

void CEmissionTestPrepareDlg::GetIntVehInfoList(void)
{
	std::list<TESTLOG> lsTestLog;
	SHBMsg sHBMsg;

	CString strBeginTime, strEndTime;
	COleDateTime odtBeginTime, odtEndTime;


	m_dtcAcceptanceStartTime.GetTime(odtBeginTime);
	m_dtcAcceptanceEndTime.GetTime(odtEndTime);

	strBeginTime.Format(L"%s", odtBeginTime.Format(L"%Y-%m-%d 00:00:00"));
	strEndTime.Format(L"%s", odtEndTime.Format(L"%Y-%m-%d 00:00:00"));

	if (!CZYHttp_PAI::GetInstance().GetVehAccInfo(L"", strBeginTime, strEndTime, L"", L"", L"", lsTestLog, sHBMsg))
	{
		CString strMsg;
		strMsg.Format(L"获取车辆列表失败\r\n%s\r\n%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		MessageBox(strMsg, L"获取列表", MB_ICONWARNING|MB_OK);
		return;
	}

	// 删除列表数据
	m_lstVehicle.DeleteAllItems();

	if (lsTestLog.size() > 0)
	{
		int nItem(0);
		for (std::list<TESTLOG>::iterator ir=lsTestLog.begin();ir != lsTestLog.end(); ++ir)
		{
			// 序号
			const int nItemIndex = m_lstVehicle.InsertItem(nItem, ir->wchReserved1);
			// 车牌号码
			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->wchPlateNumber, 0, 0, 0, 0);
			// 检测类型
			int nType = _wtoi(ir->wchTestType);
			switch(nType)
			{
			case 1:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"稳态", 0, 0, 0, 0);}break;
			case 2:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"简易瞬态", 0, 0, 0, 0);}break;
			case 3:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"加载减速", 0, 0, 0, 0);}break;
			case 4:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"双怠速", 0, 0, 0, 0);}break;
			case 5:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"不透光自由加速", 0, 0, 0, 0);}break;
			case 6:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"滤纸式自由加速", 0, 0, 0, 0);}break;
			case 7:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"农用车自由加速", 0, 0, 0, 0);}break;
			case 8:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"摩托车怠速", 0, 0, 0, 0);}break;
			case 9:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"摩托车双怠速", 0, 0, 0, 0);}break;
			default:{m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, L"错误类型", 0, 0, 0, 0);}break;
			}
			// 流水号
			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->wchReportNumber, 0, 0, 0, 0);
			// 检测次数
			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->wchNumberOfTestPeriod, 0, 0, 0, 0);
			// VIN
			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->wchVIN, 0, 0, 0, 0);

			nItem++;
		}
	}
}

void CEmissionTestPrepareDlg::GetIntVehInfo(void)
{
	BeginWaitCursor();
	const int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(nIndex!=-1)
	{
		m_strCurrentTestType = m_lstVehicle.GetItemText(nIndex, 2);
		m_strCurrentRunningNumber = m_lstVehicle.GetItemText(nIndex, 3);

		TESTLOG sTestLog;
		m_sTestLog = sTestLog;
		VEHICLEINFO sVehInfo;
		m_sVehInfo = sVehInfo;
		SHBMsg sHBMsg;

		if (!CZYHttp_PAI::GetInstance().GetAccInfo(m_strCurrentRunningNumber, sTestLog, sVehInfo, sHBMsg))
		{
			CString strMsg;
			strMsg.Format(L"获取车辆信息失败\r\n%s\r\n%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
			MessageBox(strMsg, L"获取车辆信息", MB_ICONWARNING|MB_OK);
			return;
		}
		m_sTestLog = sTestLog;
		m_sVehInfo = sVehInfo;

		CString str(L"");
		m_cbTestType.SetWindowTextW(m_strCurrentTestType);
		m_edPlateNumber.SetWindowTextW(sVehInfo.wchPlateNumber);
		m_edOwner.SetWindowTextW(sVehInfo.wchOwner);
		m_cbPlateType.SetWindowTextW(sVehInfo.wchPlateType);
		m_edVIN.SetWindowTextW(sVehInfo.wchVIN);
		m_edModel.SetWindowTextW(sVehInfo.wchModel);
		COleDateTime odtDate;
		if(odtDate.ParseDateTime(sVehInfo.wchProductionDate)){m_dtcProductionDate.SetTime(odtDate);}
		if(odtDate.ParseDateTime(sVehInfo.wchRegistrationDate)){m_dtcRegistrationDate.SetTime(odtDate);}
		m_cbDriveType.SetWindowTextW(sVehInfo.wchDriveType);
		m_edUnladenMass.SetWindowTextW(sVehInfo.wchUnladenMass);
		m_edMaximumTotalMass.SetWindowTextW(sVehInfo.wchMaximumTotalMass);
		m_cbFuelType.SetWindowTextW(sVehInfo.wchFuelType);
		m_cbAirIntakeMode.SetWindowTextW(sVehInfo.wchAirIntakeMode);
		m_cbOilSupplyMode.SetWindowTextW(sVehInfo.wchOilSupplyMode);
		str.Format(L"%.2f", _wtof(sVehInfo.wchRatedPower));
		m_edRatedPower.SetWindowTextW(str);
		int nNumberOfTestPeriod = _wtoi(sTestLog.wchNumberOfTestPeriod);

		if (nNumberOfTestPeriod > 1){str.Format(L"第%d次复检", nNumberOfTestPeriod-1);}
		else{str = L"初检";}
		m_edIsRetest.SetWindowTextW(str);

		str.Format(L"%d", _wtoi(sVehInfo.wchRatedRev));
		m_edRatedRev.SetWindowTextW(str);

		str = sVehInfo.wchHasCatalyticConverter;
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

		m_lbDynInfo.SetWindowTextW(L"南华软件，进入到安全提示界面在进行下一步");
	}



	EndWaitCursor();
}

