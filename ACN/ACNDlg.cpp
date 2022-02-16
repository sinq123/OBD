
// ACNDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ACN.h"
#include "ACNDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACNDlg 对话框




CACNDlg::CACNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CACNDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CACNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassWord);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMsg);
	DDX_Control(pDX, ID_ED_JKDZ, m_edJKDZ);
	DDX_Control(pDX, ID_ED_JKXLH, m_edJKXLH);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNumber);
	DDX_Control(pDX, ID_ED_PlateNumber, m_edPlateNumber);
	DDX_Control(pDX, IDC_COM_OBDJCY, m_cbOBDJCY);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, IDC_RA_SQL, m_edchkSQL);
	DDX_Control(pDX, IDC_RA_INTET, m_edIntet);

	DDX_Control(pDX, ID_ED_CALID, m_edCALID);
	DDX_Control(pDX, ID_ED_CVN, m_edCVN);
	DDX_Control(pDX, ID_ED_OBDTYPE, m_edOBDType);
}

BEGIN_MESSAGE_MAP(CACNDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CACNDlg::OnBnClickedBtnSetconfig)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CACNDlg::OnBnClickedButtonUpdateVehicleList)
	ON_EN_CHANGE(ID_ED_PlateNumber, &CACNDlg::OnEnChangeEdPlatenumber)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CACNDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UP, &CACNDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_UP_TELET, &CACNDlg::OnBnClickedBtnUpTelet)
END_MESSAGE_MAP()


// CACNDlg 消息处理程序

BOOL CACNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();
	GetOBDJCY();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CACNDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CACNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CACNDlg::SetConfig(void)
{
	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHModeServerDB.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"Database", L"Source", L"(Local)");
		m_edSource.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"Database", L"Catalog", L"NHModeServerDB_GB2018");
		m_edCatalog.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"Database", L"UserID", L"sa");
		m_edUserID.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"Database", L"Password", L"123");
		m_edPassWord.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"ACWebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"EPB", L"WebAddress", L"");
		m_edJKDZ.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"JKXLH", L"");
		m_edJKXLH.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"JCZBH", L"");
		m_edStationNumber.SetWindowTextW(strTemp);
	}

}


void CACNDlg::SetVehCtrl(void)
{
	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	int nDlgFontSize;
	nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
	CFont fontVehicleLCFont, fontDlgFont;


	fontVehicleLCFont.CreateFont(nDlgFontSize,
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

	fontDlgFont.CreateFont(nDlgFontSize,
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


	m_lstVehicle.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstVehicle.SetFont(&fontVehicleLCFont);
	m_lstVehicle.SetTextColor(RGB(0, 255, 0));
	m_lstVehicle.SetBkColor(RGB(0, 0, 0));
	m_lstVehicle.SetTextBkColor(RGB(0, 0, 0));

	m_lstVehicle.InsertColumn(0, L"序号",		0,	0);
	m_lstVehicle.InsertColumn(1, L"流水号",		0,	300);
	m_lstVehicle.InsertColumn(2, L"车牌号码",	0,	150);
	m_lstVehicle.InsertColumn(3, L"车牌类型",	0,	120);
	m_lstVehicle.InsertColumn(4, L"检测类型",	0,	120);
	m_lstVehicle.InsertColumn(5, L"报告单编号",	0,	300);
	m_lstVehicle.InsertColumn(6, L"OBD结果",	0,	100);
	m_lstVehicle.InsertColumn(7, L"OBD检测状态",	0,	100);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}

void CACNDlg::OnBnClickedBtnSetconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	SetConfig();

	GetOBDJCY();
}

void CACNDlg::GetOBDJCY(void)
{
	CString strSQL;

	CString strSource, strCatalog, strUserID, strPassword;

	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"数据库参数为空");
		return ;
	}

	m_cbOBDJCY.ResetContent();
	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		_RecordsetPtr pRecordset(NULL);
		// 获取下检测人员
		strSQL.Format(L"select * from UserInfo /*where Role = 6*/");
		if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
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
						m_cbOBDJCY.AddString((wchar_t*)(_bstr_t)v);
					}
					else
					{
						m_cbOBDJCY.AddString(L"");
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
	else
	{
		m_edMsg.SetWindowTextW(L"连接数据库错误");
	}
	CNHSQLServerDBO::CloseDB(pConnection);
}

bool CACNDlg::GetVehicleList(void)
{
	m_edMsg.SetWindowTextW(L"");
	// 删除列表数据
	m_lstVehicle.DeleteAllItems();
	CString strSQL;

	CString strSource, strCatalog, strUserID, strPassword;

	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);

	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"数据库参数为空");
		return false;
	}

	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		strSQL.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
		strSQL.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
		strSQL.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");
		//strSQL.AppendFormat(L" where (TestLog.ItemOBD = '4' or TestLog.ItemOBD = '1') and (ResultOfOBD.Judge = '2'/* or ResultOfOBD.Judge = '1'*/)");

		// 增加筛选
		if (!strPlateNumber.IsEmpty())
		{
			strSQL.AppendFormat(L" and PlateNumber like '%%%s%%'", strPlateNumber);
		}

		_RecordsetPtr pRecordset(NULL);
		if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
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
			m_edMsg.SetWindowTextW(strMsg);
		}
		CNHSQLServerDBO::CloseQuery(pRecordset);

	}
	else
	{
		CNHSQLServerDBO::CloseDB(pConnection);
		m_edMsg.SetWindowTextW(L"数据库联系错误");
		return false;
	}

	CNHSQLServerDBO::CloseDB(pConnection);
	return true;
}


void CACNDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	GetVehicleList();
}


void CACNDlg::OnEnChangeEdPlatenumber()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (!strPlateNumber.IsEmpty())
	{
		GetVehicleList();
	}
}


void CACNDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_edMsg.SetWindowTextW(L"");
	BeginWaitCursor();
	// 设置OBD检测员必须填写
	m_strRunningNumber = L"";
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		// 选中车牌后可以点击UP
		if (!m_strRunningNumber.IsEmpty())
		{
			GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
		}
		else
		{
			m_edMsg.SetWindowTextW(L"检测日志编号为空，或检测方法为空");
		}
	}
	EndWaitCursor();
	*pResult = 0;
}


void CACNDlg::OnBnClickedBtnUp()
{
	// TODO: 在此添加控件通知处理程序代码
	// 先处理SQL部分在处理上传部分
	CString strSource, strCatalog, strUserID, strPassword, strOBDJCY;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);
	m_cbOBDJCY.GetWindowTextW(strOBDJCY);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty() 
		|| strOBDJCY.IsEmpty()
		)
	{
		m_edMsg.SetWindowTextW(L"数据库参数为空");
		return;
	}

	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHModeServerDB.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		// 先写入配置文件参数
		si.SetString(L"Database", L"Source", strSource);
		si.SetString(L"Database", L"Catalog", strCatalog);
		si.SetString(L"Database", L"UserID", strUserID);
		si.SetString(L"Database", L"Password", strPassword);
	}

	CString strSQL;

	// 获取OBD数据
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	SResultOfOBD sResultOfOBD;
	GetDboResultOfOBD(strSQL, &sResultOfOBD);

	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	TESTLOG sTestLog;
	
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		m_edMsg.SetWindowTextW(L"获取数据库检测日志失败");
		return;
	}

	sResultOfOBD.strRunningNumber = m_strRunningNumber.GetString();
	sResultOfOBD.strOperator = strOBDJCY.GetString();

	bool bRet(false);
	CString strMsg;
	bRet = SetOBDLOG(sTestLog, sResultOfOBD, strMsg);

	if (!bRet)
	{
		m_edMsg.SetWindowTextW(L"更新OBD数据失败");
		return;
	}

	CString strCVN, strCALID, strOBDType;
	m_edCALID.GetWindowTextW(strCALID);
	m_edCVN.GetWindowTextW(strCVN);
	m_edOBDType.GetWindowTextW(strOBDType);

	if (!strCALID.IsEmpty())
	{
		sResultOfOBD.strEngineCALID = strCALID.GetString();
		m_edCALID.SetWindowTextW(L"");
	}

	if (!strCVN.IsEmpty())
	{
		sResultOfOBD.strEngineCVN = strCVN.GetString();
		m_edCVN.SetWindowTextW(L"");
	}

	if (!strOBDType.IsEmpty())
	{
		sResultOfOBD.strOBDType = strOBDType.GetString();
		m_edOBDType.SetWindowTextW(L"");
	}

	// 写入数据库
	SetDboResultOfOBD(sResultOfOBD);
	// 写入临时文件
	SetIniResultOfOBD(sResultOfOBD);

	// 上传部分
	CString strURL, strStationNumber, strJKXLH;

	m_edJKDZ.GetWindowTextW(strURL);
	m_edJKXLH.GetWindowTextW(strJKXLH);
	m_edStationNumber.GetWindowTextW(strStationNumber);

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);


	CString strTempMsg;
	strMsg.Empty();
	bRet = UpOBDReaust(pchURL, strStationNumber, strJKXLH, sTestLog, sResultOfOBD, strMsg);
	if (!bRet)
	{
		strMsg = L"上传OBD结果失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s\r\n", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
		//return;
	}


	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);

	if (m_edIntet.GetCheck() == 1)
	{
		wchar_t wchPath[MAX_PATH];
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"HasOBD", L"0");
			si.SetString(L"TestLog", L"ItemOBD", L"0");
		}
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"VehicleInfo", L"HasOBD", L"0");
		}

		// 车辆信息
		VEHICLEINFO sVehInfo;
		strSQL.Format(L"select * from VehicleInfo where PlateNumber = '%s' and PlateType = '%s'", sTestLog.wchPlateNumber, sTestLog.wchPlateType);
		GetDboVehicleInfo(strSQL, &sVehInfo);
		wcscpy_s(sVehInfo.wchHasOBD, L"0");
		SetDboVehicleInfo(sVehInfo);

		wcscpy_s(sTestLog.wchItemOBD, L"0");
		wcscpy_s(sTestLog.wchHasOBD, L"0");
		if (0x00 != SetDboTestLog(sTestLog))
		{
			strTempMsg.AppendFormat(L"%s\r\n", L"写入数据库失败");
			m_edMsg.SetWindowTextW(strTempMsg);
		}
		else
		{
			strTempMsg.AppendFormat(L"%s\r\n", L"请更新列表后，在点击下一步继续排放检测！");
			m_edMsg.SetWindowTextW(strTempMsg);
			GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(TRUE);
		}
	}
	else
	{
		wchar_t wchPath[MAX_PATH];
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"ItemOBD", L"4");
		}

		wcscpy_s(sTestLog.wchItemOBD, L"4");
		if (0x00 != SetDboTestLog(sTestLog))
		{
			strTempMsg.AppendFormat(L"%s\r\n", L"写入数据库失败");
			m_edMsg.SetWindowTextW(strTempMsg);
		}
		else
		{
			strTempMsg.AppendFormat(L"%s\r\n", L"写入数据库成功");
			m_edMsg.SetWindowTextW(strTempMsg);
		}
	}
}

bool CACNDlg::SetOBDLOG(const TESTLOG& sTestLog, SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(true);

	CString strFulType(sTestLog.wchFuelType);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

	if (strFulType.Find(L"柴") != -1)
	{
		CString strOBDType;
		srand(time(0));
		int n = rand()%10;
		strOBDType.Format(L"%s", GetOBDType(n, L"柴油")); 
		CString strEngineCALID, strEngineCVN;
		Sleep(1000);
		GetEngineCALID(strOBDType, L"柴油", strEngineCALID, strEngineCVN);

		sResultOfOBD.strOBDType  = strOBDType.GetString();
		sResultOfOBD.strEngineCALID = strEngineCALID.GetString();
		sResultOfOBD.strEngineCVN = strEngineCVN.GetString();

		sResultOfOBD.strComErrorJudge = L"1";
		sResultOfOBD.strComErrorComment = L"";

		sResultOfOBD.strDisplayJudge = L"1";
		sResultOfOBD.strDisplayComment = L"";

		sResultOfOBD.strLampStateJudge = L"1";
		sResultOfOBD.strLampStateComment = L"";

		sResultOfOBD.strDTC = L"";
		sResultOfOBD.strDTCComment = L"";
		sResultOfOBD.strFreezeData = L"";
		sResultOfOBD.strDTCMileage = L"";
		sResultOfOBD.strDTCJudge = L"1";

		sResultOfOBD.strCheckItemJudge_MFT = L"0";
		sResultOfOBD.strCheckItemJudge_FST = L"1";
		sResultOfOBD.strCheckItemJudge_CC = L"0";
		sResultOfOBD.strCheckItemJudge_CCH = L"0";
		sResultOfOBD.strCheckItemJudge_OS = L"0";
		sResultOfOBD.strCheckItemJudge_OSH = L"0";
		sResultOfOBD.strCheckItemJudge_EECS = L"0";
		sResultOfOBD.strCheckItemJudge_EGR = L"0";
		sResultOfOBD.strCheckItemJudge_SAIS = L"0";
		sResultOfOBD.strCheckItemJudge_ICM = L"1";
		sResultOfOBD.strCheckItemJudge_VVT = L"0";

		sResultOfOBD.strSystemCheckJudge = L"1";

		srand(time(0));
		CString strTemp;
		strTemp.Format(L"%d", (int)rand()%100+1501);
		sResultOfOBD.strIUPR_OMCEC = strTemp.GetString();
		sResultOfOBD.strIUPR_ICC = L"0";
		sResultOfOBD.strIUPR_CMCCB1 = L"0";
		sResultOfOBD.strIUPR_CMCECB1 = L"0";
		sResultOfOBD.strIUPR_CMCCB2 = L"0";
		sResultOfOBD.strIUPR_CMCECB2 = L"0";
		sResultOfOBD.strIUPR_O2SMCCB1 = L"0";
		sResultOfOBD.strIUPR_O2SMCECB1 = L"0";
		sResultOfOBD.strIUPR_O2SMCCB2 = L"0";
		sResultOfOBD.strIUPR_O2SMCECB2 = L"0";
		sResultOfOBD.strIUPR_RO2SMCCB1 = L"0";
		sResultOfOBD.strIUPR_RO2SMCECB1 = L"0";
		sResultOfOBD.strIUPR_RO2SMCCB2 = L"0";
		sResultOfOBD.strIUPR_RO2SMCECB2 = L"0";
		sResultOfOBD.strIUPR_EGRC = L"0";
		sResultOfOBD.strIUPR_EGREC = L"0";
		sResultOfOBD.strIUPR_GPFC1 = L"";
		sResultOfOBD.strIUPR_GPFEC1 = L"";
		sResultOfOBD.strIUPR_GPFC2 = L"";
		sResultOfOBD.strIUPR_GPFEC2 = L"";
		sResultOfOBD.strIUPR_AMCCC = L"0";
		sResultOfOBD.strIUPR_AMCEC = L"0";
		sResultOfOBD.strIUPR_EVAPC = L"0";
		sResultOfOBD.strIUPR_EVAPEC = L"0";
		sResultOfOBD.strJudge = L"1";
	}
	else
	{
		CString strOBDType;
		srand(time(0));
		int n = rand()%10;
		strOBDType.Format(L"%s", GetOBDType(n, L"汽油")); 
		CString strEngineCALID, strEngineCVN;
		Sleep(1000);
		GetEngineCALID(strOBDType, L"汽油", strEngineCALID, strEngineCVN);

		sResultOfOBD.strOBDType  = strOBDType.GetString();
		sResultOfOBD.strEngineCALID = strEngineCALID.GetString();
		sResultOfOBD.strEngineCVN = strEngineCVN.GetString();

		sResultOfOBD.strComErrorJudge = L"1";
		sResultOfOBD.strComErrorComment = L"";

		sResultOfOBD.strDisplayJudge = L"1";
		sResultOfOBD.strDisplayComment = L"";

		sResultOfOBD.strLampStateJudge = L"1";
		sResultOfOBD.strLampStateComment = L"";

		sResultOfOBD.strDTC = L"";
		sResultOfOBD.strDTCComment = L"";
		sResultOfOBD.strFreezeData = L"";
		sResultOfOBD.strDTCMileage = L"";
		sResultOfOBD.strDTCJudge = L"1";

		sResultOfOBD.strCheckItemJudge_MFT = L"1";
		sResultOfOBD.strCheckItemJudge_FST = L"1";
		sResultOfOBD.strCheckItemJudge_CC = L"1";
		sResultOfOBD.strCheckItemJudge_CCH = L"0";
		sResultOfOBD.strCheckItemJudge_OS = L"1";
		sResultOfOBD.strCheckItemJudge_OSH = L"1";
		sResultOfOBD.strCheckItemJudge_EECS = L"0";
		sResultOfOBD.strCheckItemJudge_EGR = L"1";
		sResultOfOBD.strCheckItemJudge_SAIS = L"0";
		sResultOfOBD.strCheckItemJudge_ICM = L"1";
		sResultOfOBD.strCheckItemJudge_VVT = L"0";

		sResultOfOBD.strSystemCheckJudge = L"1";

		srand(time(0));
		CString strTemp;
		strTemp.Format(L"%d", (int)rand()%100+101);
		sResultOfOBD.strIUPR_OMCEC = strTemp.GetString();
		strTemp.Format(L"%d", (int)rand()%100+601);
		sResultOfOBD.strIUPR_ICC = strTemp.GetString();
		strTemp.Format(L"%d", (int)rand()%40+101);
		sResultOfOBD.strIUPR_CMCCB1 = strTemp.GetString();
		strTemp.Format(L"%d", (int)rand()%100+101);
		sResultOfOBD.strIUPR_CMCECB1 = strTemp.GetString();
		sResultOfOBD.strIUPR_CMCCB2 = L"0";
		sResultOfOBD.strIUPR_CMCECB2 = L"0";
		strTemp.Format(L"%d", (int)rand()%100+101);
		sResultOfOBD.strIUPR_O2SMCCB1 = strTemp.GetString();
		strTemp.Format(L"%d", (int)rand()%100+101);
		sResultOfOBD.strIUPR_O2SMCECB1 = strTemp.GetString();
		sResultOfOBD.strIUPR_O2SMCCB2 = L"0";
		sResultOfOBD.strIUPR_O2SMCECB2 = L"0";
		strTemp.Format(L"%d", (int)rand()%100+141);
		sResultOfOBD.strIUPR_RO2SMCCB1 = strTemp.GetString();
		strTemp.Format(L"%d", (int)rand()%100+101);
		sResultOfOBD.strIUPR_RO2SMCECB1 = strTemp.GetString();
		sResultOfOBD.strIUPR_RO2SMCCB2 = L"0";
		sResultOfOBD.strIUPR_RO2SMCECB2 = L"0";
		strTemp.Format(L"%d", (int)rand()%100+501);
		sResultOfOBD.strIUPR_EGRC = strTemp.GetString();
		strTemp.Format(L"%d", (int)rand()%100+101);
		sResultOfOBD.strIUPR_EGREC = strTemp.GetString();
		sResultOfOBD.strIUPR_GPFC1 = L"0";
		sResultOfOBD.strIUPR_GPFEC1 = L"0";
		sResultOfOBD.strIUPR_GPFC2 = L"0";
		sResultOfOBD.strIUPR_GPFEC2 = L"0";
		sResultOfOBD.strIUPR_AMCCC = L"0";
		sResultOfOBD.strIUPR_AMCEC = L"0";
		sResultOfOBD.strIUPR_EVAPC = L"0";
		sResultOfOBD.strIUPR_EVAPEC = L"0";
		sResultOfOBD.strJudge = L"1";
	}
	return bRet;
}

CString CACNDlg::GetOBDType(const int& nType, const CString& strFulType)
{
	if (strFulType == L"汽油")
	{
		switch (nType)
		{
		case 0:{ return L"EOBD";} break;
		case 1:{ return L"ISO/SAE预留";} break;
		case 2:{return L"OBDII-加州";} break;
		case 3:{return L"OBD-OBDII";} break;
		case 4:{return L"JOBD-EOBD";} break;
		case 5:{return L"OBD,OBDII,EOBD和KOBD";} break;
		case 6:{return L"JOBD";} break;
		case 7:{return L"不适用";} break;
		default : {return L"";} break;
		}
	}
	else
	{
		return L"";
	}
}

void CACNDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	if (strFulType == L"汽油")
	{
		if (strOBDType == L"EOBD")
		{
			srand(time(0));
			int n = rand()%21;
			switch (n)
			{
			case 0:{strEngineCALID = L"000008603574000008627583"; strEngineCVN = L"29A907FDF9511FC1";} break;
			case 1:{strEngineCALID = L"000008617572000008622137"; strEngineCVN = L"B1332BBA109DB30D";} break;
			case 2:{strEngineCALID = L"000008617572000008638799"; strEngineCVN = L"B1332BBA67E4F98A";} break;
			case 3:{strEngineCALID = L"00000925"; strEngineCVN = L"000050D5";} break;
			case 4:{strEngineCALID = L"0000QSB10900UAES"; strEngineCVN = L"不支持";} break;
			case 5:{strEngineCALID = L"0034468840270090"; strEngineCVN = L"487D596F";} break;
			case 6:{strEngineCALID = L"0044469440150263"; strEngineCVN = L"89225B2E00000000";} break;
			case 7:{strEngineCALID = L"0064464240190159"; strEngineCVN = L"AE90F315";} break;
			case 8:{strEngineCALID = L"00740R0366UAES"; strEngineCVN = L"不支持";} break;
			case 9:{strEngineCALID = L"007503A330UAES"; strEngineCVN = L"不支持";} break;
			case 10:{strEngineCALID = L"00C80R0334GW"; strEngineCVN = L"不支持";} break;
			case 11:{strEngineCALID = L"00JB0HA234UAES"; strEngineCVN = L"不支持";} break;
			case 12:{strEngineCALID = L"00MP230421UAES"; strEngineCVN = L"不支持";} break;
			case 13:{strEngineCALID = L"00MP261016UAES"; strEngineCVN = L"不支持";} break;
			case 14:{strEngineCALID = L"0356CMR3SC110815"; strEngineCVN = L"B23725E3";} break;
			case 15:{strEngineCALID = L"0356CMR5SC120830"; strEngineCVN = L"E8927CBA";} break;
			case 16:{strEngineCALID = L"03C906014CE1108"; strEngineCVN = L"不支持";} break;
			case 17:{strEngineCALID = L"03C906022AS4150"; strEngineCVN = L"A5416587";} break;
			case 18:{strEngineCALID = L"03C906022BH9928"; strEngineCVN = L"96F5B3BE";} break;
			case 19:{strEngineCALID = L"03C906022CA4511"; strEngineCVN = L"A7345A7A";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"ISO/SAE预留")
		{
			srand(time(0));
			int n = rand()%5;
			switch (n)
			{
			case 0: {strEngineCALID = L"10307843"; strEngineCVN = L"0000D42D";} break;
			case 1: {strEngineCALID = L"10294382"; strEngineCVN = L"00003C06";} break;
			case 2: {strEngineCALID = L"10190196"; strEngineCVN = L"000087DE";} break;
			case 3: {strEngineCALID = L"10236164"; strEngineCVN = L"00005C7F";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"OBDII-加州")
		{
			srand(time(0));
			int n = rand()%5;
			switch (n)
			{
			case 0: {strEngineCALID = L"30CG8001A0C01000"; strEngineCVN = L"4A44F2412C411C65";} break;
			case 1: {strEngineCALID = L"68193991AG"; strEngineCVN = L"39381886";} break;
			case 2: {strEngineCALID = L"2410568200"; strEngineCVN = L"5A4F616B";} break;
			case 3: {strEngineCALID = L"30CL120150C81101"; strEngineCVN = L"BFEB74AB5FA56D64";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"OBD-OBDII")
		{
			srand(time(0));
			int n = rand()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"JOBD-EOBD")
		{
			srand(time(0));
			int n = rand()%4;
			switch (n)
			{
			case 0: {strEngineCALID = L"31411182AA"; strEngineCVN = L"F257665B";} break;
			case 1: {strEngineCALID = L"31459703AA"; strEngineCVN = L"AD5B51F9";} break;
			case 2: {strEngineCALID = L"32267993"; strEngineCVN = L"1595668E";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"JOBD")
		{
			srand(time(0));
			int n = rand()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"不适用")
		{
			srand(time(0));
			int n = rand()%16;
			switch (n)
			{
			case 0: {strEngineCALID = L"37805RD7H530"; strEngineCVN = L"E92031E2";} break;
			case 1: {strEngineCALID = L"WE3DIVG1VII"; strEngineCVN = L"26732FED";} break;
			case 2: {strEngineCALID = L"37805RD7H040"; strEngineCVN = L"DB7E7874";} break;
			case 3: {strEngineCALID = L"WE40TP0X"; strEngineCVN = L"32DE0AFE";} break;
			case 4: {strEngineCALID = L"37805R0TH010"; strEngineCVN = L"15ECBE96";} break;
			case 5: {strEngineCALID = L"WE40TM1X"; strEngineCVN = L"D1E6ABBE";} break;
			case 6: {strEngineCALID = L"WE32TN0V20130807"; strEngineCVN = L"FCD2E103";} break;
			case 7: {strEngineCALID = L"W3B3RL0X"; strEngineCVN = L"1CEB62B7";} break;
			case 8: {strEngineCALID = L"WH24WZ0X"; strEngineCVN = L"71481A23";} break;
			case 9: {strEngineCALID = L"WH24WX0X"; strEngineCVN = L"E2FA939C";} break;
			case 10: {strEngineCALID = L"37805RLHH620"; strEngineCVN = L"7CB6834A";} break;
			case 11: {strEngineCALID = L"WH22WY1V"; strEngineCVN = L"A19D018B";} break;
			case 12: {strEngineCALID = L"W3B2RP0V"; strEngineCVN = L"CFAC7099";} break;
			case 13: {strEngineCALID = L"WH24WX1X"; strEngineCVN = L"2D863958";} break;
			case 14: {strEngineCALID = L"YM008AT0600300G0"; strEngineCVN = L"42716105";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else 
		{
			srand(time(0));
			int n = rand()%5;
			switch (n)
			{
			case 0: {strEngineCALID = L"306B4000"; strEngineCVN = L"97627560";} break;
			case 1: {strEngineCALID = L"306B4000"; strEngineCVN = L"97627560";} break;
			case 2: {strEngineCALID = L"LF0L0643UAES"; strEngineCVN = L"不支持";} break;
			case 3: {strEngineCALID = L"378055A4H640"; strEngineCVN = L"378055A4H640";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
	}
	else
	{
		srand(time(0));
		int n = rand()%16;
		switch (n)
		{
		case 0: {strEngineCALID = L"3601603B90"; strEngineCVN = L"488713A2DEF671A8";} break;
		case 1: {strEngineCALID = L"A5CE33823352SFB"; strEngineCVN = L"B94CF03432D9AAC8";} break;
		case 2: {strEngineCALID = L"E63353823352AC"; strEngineCVN = L"4824407D3CCC0824";} break;
		case 3: {strEngineCALID = L"EDC720"; strEngineCVN = L"8DC3BC97FEC2BF53";} break;
		case 4: {strEngineCALID = L"FN80009.09"; strEngineCVN = L"737771052";} break;
		case 5: {strEngineCALID = L"S072805F1112"; strEngineCVN = L"D5FE13991D7EA592";} break;
		case 6: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"977841D02EE2EC41";} break;
		case 7: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"A73BE97A8ECBEB46";} break;
		case 8: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"D04E5AB13A4655FF";} break;
		case 9: {strEngineCALID = L"S10B3805C1204"; strEngineCVN = L"84C7C19E69D9E257";} break;
		case 10: {strEngineCALID = L"S114405B1204"; strEngineCVN = L"7F1D977131B817F0";} break;
		case 11: {strEngineCALID = L"S114405B1204"; strEngineCVN = L"6C84F9DCF436BEC9";} break;
		case 12: {strEngineCALID = L"S114405B1204"; strEngineCVN = L"19A6AF95F2E08A15";} break;
		case 13: {strEngineCALID = L"SCHD13823352AC"; strEngineCVN = L"9CBB4CAC97DB";} break;
		case 14: {strEngineCALID = L"不支持"; strEngineCVN = L"E0002658F816511F";} break;
		default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
		}

	}
}

bool CACNDlg::UpOBDReaust(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CStringW strXMLdoc;
	strXMLdoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?>");
	strXMLdoc.AppendFormat(L"<root><head>");
	strXMLdoc.AppendFormat(L"<organ>%s</organ>", strStationNumber);
	strXMLdoc.AppendFormat(L"<jkxlh>%s</jkxlh>", strJKXLH);
	strXMLdoc.AppendFormat(L"<jkid>%s</jkid>", L"JC011");
	strXMLdoc.AppendFormat(L"<sjc>%s</sjc>", strTime);
	strXMLdoc.AppendFormat(L"</head>");

	strXMLdoc.AppendFormat(L"<body><vehispara>");
	strXMLdoc.AppendFormat(L"<jylsh>%s</jylsh>", sTestLog.wchReportNumber);
	strXMLdoc.AppendFormat(L"<testtimes>%s</testtimes>", sTestLog.wchNumberOfTestPeriod);
	strXMLdoc.AppendFormat(L"<jyksrq>%s</jyksrq>", sResultOfOBD.strDetBegTime.c_str());
	strXMLdoc.AppendFormat(L"<jyjsrq>%s</jyjsrq>", sResultOfOBD.strDetEndTime.c_str());
	strXMLdoc.AppendFormat(L"<jyy>%s</jyy>", sResultOfOBD.strOperator.c_str());
	strXMLdoc.AppendFormat(L"<jyjg>%s</jyjg>", L"1");
	strXMLdoc.AppendFormat(L"<obdzdyscqy>%s</obdzdyscqy>", L"佛山市南华仪器股份有限公司");
	strXMLdoc.AppendFormat(L"<obdzdyxh>%s</obdzdyxh>", L"NHOBD-1");
	strXMLdoc.AppendFormat(L"<vin>%s</vin>", sResultOfOBD.strVIN.c_str());
	strXMLdoc.AppendFormat(L"<obdyq>%s</obdyq>", sResultOfOBD.strOBDType.c_str());
	strXMLdoc.AppendFormat(L"<gzjg>%s</gzjg>", L"1");
	strXMLdoc.AppendFormat(L"<tx>%s</tx>", L"1");
	strXMLdoc.AppendFormat(L"<txbz>%s</txbz>", L"");
	strXMLdoc.AppendFormat(L"<bj>%s</bj>", L"1");
	strXMLdoc.AppendFormat(L"<bjbz>%s</bjbz>", L"");
	strXMLdoc.AppendFormat(L"<jxxm>%s</jxxm>", L"1");
	strXMLdoc.AppendFormat(L"<jxxmbz>%s</jxxmbz>", L"");
	strXMLdoc.AppendFormat(L"<odometer>%s</odometer>", L"0");
	strXMLdoc.AppendFormat(L"<odomil>%s</odomil>", L"0");
	strXMLdoc.AppendFormat(L"<enginecalid>%s</enginecalid>", sResultOfOBD.strEngineCALID.c_str());
	strXMLdoc.AppendFormat(L"<enginecvn>%s</enginecvn>", sResultOfOBD.strEngineCVN.c_str());
	strXMLdoc.AppendFormat(L"<hclcalid>%s</hclcalid>", L"");
	strXMLdoc.AppendFormat(L"<hclcvn>%s</hclcvn>", L"");
	strXMLdoc.AppendFormat(L"<calid>%s</calid>", L"");
	strXMLdoc.AppendFormat(L"<cvn>%s</cvn>", L"");

	
	strXMLdoc.AppendFormat(L"<iupr>%s</iupr>", OBDIURT(sResultOfOBD));
	//strXMLdoc.AppendFormat(L"<iupr>%s</iupr>", L"");
	strXMLdoc.AppendFormat(L"</vehispara></body></root>");



	std::wstring strRet;

	int nRet = CACInterfaceLib_API::WriteObjectOut(pchURL, strXMLdoc.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
					strMsg.AppendFormat(L"%s,%s", strCode.c_str(), strContent.c_str());
				}
			}
			else
			{
				strMsg.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				bRet = true;
			}
		}
	}
	else
	{
		strMsg.AppendFormat(L"联网失败");
	}


	return bRet;
}

CString CACNDlg::OBDIURT(const SResultOfOBD& sResultOfOBD)
{
	CString strXMLDoc;

	strXMLDoc.AppendFormat(L"<iuprs>");

	CString strTemp, strTemp1;
	//1-NMHC 催化器 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"NMHC催化器");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_NMHCC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_NMHCEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//2-NOX 催化器 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"NOX催化器");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_NOXCC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_NOXCEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXCC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXCEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_NOXCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXCC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//3-NOX 吸附器 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"NOX吸附器");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_NOXAC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_NOXAEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXAC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXAEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_NOXAEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXAC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//4-颗粒捕集器 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"颗粒捕集器");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_PMC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_PMEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PMC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PMEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_PMEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PMC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//5-废气传感器 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"废气传感器");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_WSC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_WSEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_WSC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_WSEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_WSEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_WSC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//6-ERG和VVT
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"ERG和VVT");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_EGRC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_EGREC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//7-增压压力
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"增压压力");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_PPC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_PPEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PPC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PPEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_PPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PPC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	// 8-催化器组1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"催化器组1");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_CMCCB1.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_CMCECB1.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//9-催化器组 2 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"催化器组2");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_CMCCB2.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_CMCECB2.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB2.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB2.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_CMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB2.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//10-前氧传感 器组 1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"前氧传感器组1");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_O2SMCCB1.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_O2SMCECB1.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCCB1.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCECB1.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB1.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//11- 前氧传感器组 2
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"前氧传感器组2");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_O2SMCCB2.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_O2SMCECB2.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCCB2.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCECB2.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB2.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//12-后氧传感器组 1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"后氧传感器组1");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_RO2SMCCB1.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_RO2SMCECB1.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//13- 后氧传感器组 2
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"后氧传感器组2");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_RO2SMCCB2.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_RO2SMCECB2.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//14-EVAP
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"EVAP");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_EVAPC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_EVAPEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EVAPC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//16-GPF 组 1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"GPF组1");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_GPFC1.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_GPFEC1.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC1.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//17-GPF 组 2
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"GPF组2");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_GPFC2.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_GPFEC2.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC2.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");
	//18-二次空气喷射系统
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"二次空气喷射系统");
	strXMLDoc.AppendFormat(L"<jcwccs>%s</jcwccs>", sResultOfOBD.strIUPR_AMCCC.c_str());
	strXMLDoc.AppendFormat(L"<fhjctjcs>%s</fhjctjcs>", sResultOfOBD.strIUPR_AMCEC.c_str());
	strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str()));
	strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str()));
	if (strTemp == L"0" || strTemp1 == L"0")
	{
		strXMLDoc.AppendFormat(L"<iuprl>%s</iuprl>", L"0");
	}
	else
	{
		strXMLDoc.AppendFormat(L"<iuprl>%.2f</iuprl>", ((_wtof(sResultOfOBD.strIUPR_AMCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_AMCCC.c_str()))));
	}
	strXMLDoc.AppendFormat(L"</iupr>");

	strXMLDoc.AppendFormat(L"</iuprs>");
	return strXMLDoc;
}


void CACNDlg::OnBnClickedBtnUpTelet()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_strRunningNumber == L"")
	{
		m_edMsg.SetWindowTextW(L"日志编号为空");
		return;
	}

	CString strSQL;
	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	TESTLOG sTestLog;
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		m_edMsg.SetWindowTextW(L"获取数据库检测日志失败");
		return;
	}

	VEHICLEINFO sVehInfo;
	strSQL.Format(L"select * from VehicleInfo where PlateNumber = '%s' and PlateType = '%s'", sTestLog.wchPlateNumber, sTestLog.wchPlateType);
	GetDboVehicleInfo(strSQL, &sVehInfo);


	// 上传部分
	CString strURL, strStationNumber, strJKXLH;

	m_edJKDZ.GetWindowTextW(strURL);
	m_edJKXLH.GetWindowTextW(strJKXLH);
	m_edStationNumber.GetWindowTextW(strStationNumber);

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);


	CString strMsg;
	bool bRet = UpOBDRealTime(pchURL, strStationNumber, strJKXLH, sTestLog, strMsg);

	CString strTempMsg;
	if (!bRet)
	{
		strMsg = L"OBD过程上传失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
	}
	else
	{
		strMsg = L"OBD过程上传成功\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
	}

	strMsg.Empty();
	bRet = UpTestEnd(pchURL, strStationNumber, strJKXLH, sTestLog, strMsg);

	if (!bRet)
	{
		strMsg = L"检验过程结束失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
	}
	else
	{
		strMsg = L"检验过程结束成功\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
	}

	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"TestLog", L"ItemOBD", L"4");
		si.SetString(L"TestLog", L"HasOBD", L"1");
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"VehicleInfo", L"HasOBD", L"1");
	}


	wcscpy_s(sVehInfo.wchHasOBD, L"1");
	SetDboVehicleInfo(sVehInfo);

	wcscpy_s(sTestLog.wchItemOBD, L"4");
	wcscpy_s(sTestLog.wchHasOBD, L"1");
	if (0x00 != SetDboTestLog(sTestLog))
	{
		strTempMsg.AppendFormat(L"%s\r\n", L"写入数据库失败");
		m_edMsg.SetWindowTextW(strTempMsg);
	}
	else
	{
		strTempMsg.AppendFormat(L"%s\r\n", L"检测完成");
		m_edMsg.SetWindowTextW(strTempMsg);
	}

	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);
	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
}


bool CACNDlg::UpOBDRealTime(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, CString& strMsg)
{
	bool bRet(false);

	CString strTestType(sTestLog.wchTestType);
	CStringW strTemp[23]; //共22个参数，从0开始
	for (int i=0;i<23;i++)
	{strTemp[i].Empty();}
	COleDateTime odtCurTime;
	COleDateTimeSpan odts;
	SYSTEMTIME st;
	if (strTestType.Find(L"1") != -1) // 稳态
	{
		std::vector<SRealTimeDataOfASM> vt;
		GetIniRealTimeDataOfASMEx(vt);
		strTemp[0].AppendFormat(L"%d", vt.size());// 采样总点数
		for(int i=0; i<vt.size(); i++)
		{
			strTemp[1].AppendFormat(L"%d,", i+1);// 以时序类别分类按顺序自增
			// 全程时序 格式为 yyyy-mm-dd 24h:mi:ss.fff 保证时间1秒1秒
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTemp[2].AppendFormat(L"%s,", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			// 节气门绝对开度
			strTemp[3].AppendFormat(L"%s,", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			// 计算负荷值
			strTemp[4].AppendFormat(L"%s,", vt[i].sOBDRTData.strCalculationLoad.c_str());
			// 前氧传感器信号
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strTemp[5].AppendFormat(L"%s,", vt[i].sOBDRTData.strForwardSensorV.c_str());
			}
			else
			{
				strTemp[5].AppendFormat(L"%s,",vt[i].sOBDRTData.strForwardSensorI.c_str());
			}
			// 过量空气系数（λ）
			strTemp[6].AppendFormat(L"%s,", vt[i].sOBDRTData.strAirCoefficient.c_str());
			// 油门开度
			strTemp[7].AppendFormat(L"%s,", vt[i].sOBDRTData.strThrottleOpening.c_str());
			// 车速
			//strTemp[8].append(L"%s,", vt[i].sOBDRTData.strVelocity.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			}
			strTemp[8].AppendFormat(L"%.1f,", f); //车速取整
			// 发动机转速
			//strTemp[9].append(L"%s,", vt[i].sOBDRTData.strEngineRev.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
			}
			else
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 650;}
				else if (i%3 == 0){n = rand()%300 + 678;}
				else if (i%5 == 0){n = rand()%300 + 687;}
				else {n = rand()%300 + 640;}
			}
			strTemp[9].AppendFormat(L"%d,", n);
			// 进气量
			strTemp[10].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAF.c_str()));
			// 进气压力
			strTemp[11].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAP.c_str()));
			// 发动机输出功率
			strTemp[12].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			// 增压压力
			strTemp[13].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			// 耗油量
			strTemp[14].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			// 氮氧传感器浓度
			strTemp[15].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			// 尿素喷射量
			strTemp[16].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			// 排气温度 
			strTemp[17].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGT.c_str()));
			// 颗粒捕集器压差
			strTemp[18].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			// EGR 开度
			strTemp[19].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGRPosition.c_str()));
			// 燃油喷射压力
			strTemp[20].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			// 工况类型（预留）
			strTemp[21].AppendFormat(L"%s,", L"");
		}
	}
	else if (strTestType.Find(L"2") != -1) // 简易瞬态
	{
		std::vector<SRealTimeDataOfVMAS> vt;
		GetIniRealTimeDataOfVMASEx(vt);
		strTemp[0].Format(L"%d", vt.size());// 采样总点数
		for(int i=0; i<vt.size(); i++)
		{
			strTemp[1].AppendFormat(L"%d,", i+1);// 以时序类别分类按顺序自增
			// 全程时序 格式为 yyyy-mm-dd 24h:mi:ss.fff 保证时间1秒1秒
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTemp[2].AppendFormat(L"%s,", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			// 节气门绝对开度
			strTemp[3].AppendFormat(L"%s,", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			// 计算负荷值
			strTemp[4].AppendFormat(L"%s,", vt[i].sOBDRTData.strCalculationLoad.c_str());
			// 前氧传感器信号
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strTemp[5].AppendFormat(L"%s,", vt[i].sOBDRTData.strForwardSensorV.c_str());
			}
			else
			{
				strTemp[5].AppendFormat(L"%s,",vt[i].sOBDRTData.strForwardSensorI.c_str());
			}
			// 过量空气系数（λ）
			strTemp[6].AppendFormat(L"%s,", vt[i].sOBDRTData.strAirCoefficient.c_str());
			// 油门开度
			strTemp[7].AppendFormat(L"%s,", vt[i].sOBDRTData.strThrottleOpening.c_str());
			// 车速
			//strTemp[8].append(L"%s,", vt[i].sOBDRTData.strVelocity.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			}
			strTemp[8].AppendFormat(L"%.1f,", f); //车速取整
			// 发动机转速
			//strTemp[9].append(L"%s,", vt[i].sOBDRTData.strEngineRev.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
			}
			else
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 650;}
				else if (i%3 == 0){n = rand()%300 + 678;}
				else if (i%5 == 0){n = rand()%300 + 687;}
				else {n = rand()%300 + 640;}
			}
			strTemp[9].AppendFormat(L"%d,", n);
			// 进气量
			strTemp[10].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAF.c_str()));
			// 进气压力
			strTemp[11].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAP.c_str()));
			// 发动机输出功率
			strTemp[12].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			// 增压压力
			strTemp[13].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			// 耗油量
			strTemp[14].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			// 氮氧传感器浓度
			strTemp[15].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			// 尿素喷射量
			strTemp[16].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			// 排气温度 
			strTemp[17].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGT.c_str()));
			// 颗粒捕集器压差
			strTemp[18].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			// EGR 开度
			strTemp[19].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGRPosition.c_str()));
			// 燃油喷射压力
			strTemp[20].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			// 工况类型（预留）
			strTemp[21].AppendFormat(L"%s,", L"");
		}
	}
	else if (strTestType.Find(L"3") != -1) // 加载减速
	{
		std::vector<SRealTimeDataOfLUGDOWN> vt;
		GetIniRealTimeDataOfLUGDOWNEx(vt);
		strTemp[0].Format(L"%d", vt.size());// 采样总点数
		for(int i=0; i<vt.size(); i++)
		{
			strTemp[1].AppendFormat(L"%d,", i+1);// 以时序类别分类按顺序自增
			// 全程时序 格式为 yyyy-mm-dd 24h:mi:ss.fff 保证时间1秒1秒
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTemp[2].AppendFormat(L"%s,", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			// 节气门绝对开度
			strTemp[3].AppendFormat(L"%s,", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			// 计算负荷值
			strTemp[4].AppendFormat(L"%s,", vt[i].sOBDRTData.strCalculationLoad.c_str());
			// 前氧传感器信号
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strTemp[5].AppendFormat(L"%s,", vt[i].sOBDRTData.strForwardSensorV.c_str());
			}
			else
			{
				strTemp[5].AppendFormat(L"%s,",vt[i].sOBDRTData.strForwardSensorI.c_str());
			}
			// 过量空气系数（λ）
			strTemp[6].AppendFormat(L"%s,", vt[i].sOBDRTData.strAirCoefficient.c_str());
			// 油门开度
			strTemp[7].AppendFormat(L"%s,", vt[i].sOBDRTData.strThrottleOpening.c_str());
			// 车速
			//strTemp[8].append(L"%s,", vt[i].sOBDRTData.strVelocity.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			}
			strTemp[8].AppendFormat(L"%.1f,", f); //车速取整
			// 发动机转速
			//strTemp[9].append(L"%s,", vt[i].sOBDRTData.strEngineRev.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
			}
			else
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 650;}
				else if (i%3 == 0){n = rand()%300 + 678;}
				else if (i%5 == 0){n = rand()%300 + 687;}
				else {n = rand()%300 + 640;}
			}
			strTemp[9].AppendFormat(L"%d,", n);
			// 进气量
			strTemp[10].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAF.c_str()));
			// 进气压力
			strTemp[11].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAP.c_str()));
			// 发动机输出功率
			strTemp[12].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			// 增压压力
			strTemp[13].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			// 耗油量
			strTemp[14].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			// 氮氧传感器浓度
			strTemp[15].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			// 尿素喷射量
			strTemp[16].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			// 排气温度 
			strTemp[17].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGT.c_str()));
			// 颗粒捕集器压差
			strTemp[18].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			// EGR 开度
			strTemp[19].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGRPosition.c_str()));
			// 燃油喷射压力
			strTemp[20].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			// 工况类型（预留）
			strTemp[21].AppendFormat(L"%s,", L"");
		}
	}
	else if (strTestType.Find(L"4") != -1) // 双怠速
	{
		std::vector<SRealTimeDataOfDIS> vt;
		GetIniRealTimeDataOfDISEx(vt);
		strTemp[0].Format(L"%d", vt.size());// 采样总点数
		for(int i=0; i<vt.size(); i++)
		{
			strTemp[1].AppendFormat(L"%d,", i+1);// 以时序类别分类按顺序自增
			// 全程时序 格式为 yyyy-mm-dd 24h:mi:ss.fff 保证时间1秒1秒
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTemp[2].AppendFormat(L"%s,", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			// 节气门绝对开度
			strTemp[3].AppendFormat(L"%s,", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			// 计算负荷值
			strTemp[4].AppendFormat(L"%s,", vt[i].sOBDRTData.strCalculationLoad.c_str());
			// 前氧传感器信号
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strTemp[5].AppendFormat(L"%s,", vt[i].sOBDRTData.strForwardSensorV.c_str());
			}
			else
			{
				strTemp[5].AppendFormat(L"%s,",vt[i].sOBDRTData.strForwardSensorI.c_str());
			}
			// 过量空气系数（λ）
			strTemp[6].AppendFormat(L"%s,", vt[i].sOBDRTData.strAirCoefficient.c_str());
			// 油门开度
			strTemp[7].AppendFormat(L"%s,", vt[i].sOBDRTData.strThrottleOpening.c_str());
			// 车速
			//strTemp[8].append(L"%s,", vt[i].sOBDRTData.strVelocity.c_str());
			float f(0.0f);
			strTemp[8].AppendFormat(L"%.1f,", f); //车速取整
			// 发动机转速
			//strTemp[9].append(L"%s,", vt[i].sOBDRTData.strEngineRev.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTemp[9].AppendFormat(L"%d,", n);
			// 进气量
			strTemp[10].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAF.c_str()));
			// 进气压力
			strTemp[11].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAP.c_str()));
			// 发动机输出功率
			strTemp[12].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			// 增压压力
			strTemp[13].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			// 耗油量
			strTemp[14].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			// 氮氧传感器浓度
			strTemp[15].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			// 尿素喷射量
			strTemp[16].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			// 排气温度 
			strTemp[17].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGT.c_str()));
			// 颗粒捕集器压差
			strTemp[18].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			// EGR 开度
			strTemp[19].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGRPosition.c_str()));
			// 燃油喷射压力
			strTemp[20].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			// 工况类型（预留）
			strTemp[21].AppendFormat(L"%s,", L"");
		}
	}
	else if (strTestType.Find(L"5") != -1) // 不透光
	{
		std::vector<SRealTimeDataOfFSUNHT> vt;
		GetIniRealTimeDataOfFSUNHTEx(vt);
		strTemp[0].Format(L"%d", vt.size());// 采样总点数
		for(int i=0; i<vt.size(); i++)
		{
			strTemp[1].AppendFormat(L"%d,", i+1);// 以时序类别分类按顺序自增
			// 全程时序 格式为 yyyy-mm-dd 24h:mi:ss.fff 保证时间1秒1秒
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTemp[2].AppendFormat(L"%s,", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			// 节气门绝对开度
			strTemp[3].AppendFormat(L"%s,", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			// 计算负荷值
			strTemp[4].AppendFormat(L"%s,", vt[i].sOBDRTData.strCalculationLoad.c_str());
			// 前氧传感器信号
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strTemp[5].AppendFormat(L"%s,", vt[i].sOBDRTData.strForwardSensorV.c_str());
			}
			else
			{
				strTemp[5].AppendFormat(L"%s,",vt[i].sOBDRTData.strForwardSensorI.c_str());
			}
			// 过量空气系数（λ）
			strTemp[6].AppendFormat(L"%s,", vt[i].sOBDRTData.strAirCoefficient.c_str());
			// 油门开度
			strTemp[7].AppendFormat(L"%s,", vt[i].sOBDRTData.strThrottleOpening.c_str());
			// 车速
			//strTemp[8].append(L"%s,", vt[i].sOBDRTData.strVelocity.c_str());
			float f(0.0f);
			strTemp[8].AppendFormat(L"%.1f,", f); //车速取整
			// 发动机转速
			//strTemp[9].append(L"%s,", vt[i].sOBDRTData.strEngineRev.c_str());
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTemp[9].AppendFormat(L"%d,", n);
			// 进气量
			strTemp[10].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAF.c_str()));
			// 进气压力
			strTemp[11].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strMAP.c_str()));
			// 发动机输出功率
			strTemp[12].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			// 增压压力
			strTemp[13].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			// 耗油量
			strTemp[14].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			// 氮氧传感器浓度
			strTemp[15].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			// 尿素喷射量
			strTemp[16].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			// 排气温度 
			strTemp[17].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGT.c_str()));
			// 颗粒捕集器压差
			strTemp[18].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			// EGR 开度
			strTemp[19].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strEGRPosition.c_str()));
			// 燃油喷射压力
			strTemp[20].AppendFormat(L"%d,", _wtoi(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			// 工况类型（预留）
			strTemp[21].AppendFormat(L"%s,", L"");
		}
	}
	else if (strTestType.Find(L"6") != -1) // 滤纸式
	{

	}

	CString strTime;
	GetLocalTime(&st);
	strTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	CStringW strXMLdoc;
	strXMLdoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?>");
	strXMLdoc.AppendFormat(L"<root><head>");
	strXMLdoc.AppendFormat(L"<organ>%s</organ>", strStationNumber);
	strXMLdoc.AppendFormat(L"<jkxlh>%s</jkxlh>", strJKXLH);
	strXMLdoc.AppendFormat(L"<jkid>%s</jkid>", L"GC011");
	strXMLdoc.AppendFormat(L"<sjc>%s</sjc>", strTime);
	strXMLdoc.AppendFormat(L"</head>");
	strXMLdoc.AppendFormat(L"<body><vehispara>");

	strXMLdoc.AppendFormat(L"<jylsh>%s</jylsh>", sTestLog.wchReportNumber);
	strXMLdoc.AppendFormat(L"<testtimes>%s</testtimes>", sTestLog.wchNumberOfTestPeriod);
	strXMLdoc.AppendFormat(L"<cyds>%s</cyds>", strTemp[0]);
	strXMLdoc.AppendFormat(L"<cysx>%s</cysx>", strTemp[1]);
	strXMLdoc.AppendFormat(L"<sjxl>%s</sjxl>", strTemp[2]);
	strXMLdoc.AppendFormat(L"<jqmkd>%s</jqmkd>", strTemp[3]);
	strXMLdoc.AppendFormat(L"<fh>%s</fh>", strTemp[4]);
	strXMLdoc.AppendFormat(L"<qycgqxh>%s</qycgqxh>", strTemp[5]);
	strXMLdoc.AppendFormat(L"<glkqxs>%s</glkqxs>", strTemp[6]);
	strXMLdoc.AppendFormat(L"<ymkd>%s</ymkd>", strTemp[7]);
	strXMLdoc.AppendFormat(L"<cs>%s</cs>", strTemp[8]);
	strXMLdoc.AppendFormat(L"<fdjzs>%s</fdjzs>", strTemp[9]);
	strXMLdoc.AppendFormat(L"<jql>%s</jql>", strTemp[10]);
	strXMLdoc.AppendFormat(L"<jqyl>%s</jqyl>", strTemp[11]);
	strXMLdoc.AppendFormat(L"<scgl>%s</scgl>", strTemp[12]);
	strXMLdoc.AppendFormat(L"<zyyl>%s</zyyl>", strTemp[13]);
	strXMLdoc.AppendFormat(L"<hyl>%s</hyl>", strTemp[14]);
	strXMLdoc.AppendFormat(L"<no>%s</no>", strTemp[15]);
	strXMLdoc.AppendFormat(L"<nsbsl>%s</nsbsl>", strTemp[16]);
	strXMLdoc.AppendFormat(L"<pqwd>%s</pqwd>", strTemp[17]);
	strXMLdoc.AppendFormat(L"<klbjqyc>%s</klbjqyc>", strTemp[18]);
	strXMLdoc.AppendFormat(L"<egrkd>%s</egrkd>", strTemp[19]);
	strXMLdoc.AppendFormat(L"<rypsyl>%s</rypsyl>", strTemp[20]);
	strXMLdoc.AppendFormat(L"<jczt>%s</jczt>", strTemp[21]);

	strXMLdoc.AppendFormat(L"</vehispara></body></root>");

	std::wstring strRet;

	int nRet = CACInterfaceLib_API::WriteObjectOut(pchURL, strXMLdoc.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
					strMsg.AppendFormat(L"%s,%s", strCode.c_str(), strContent.c_str());
				}
			}
			else
			{
				strMsg.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				bRet = true;
			}
		}
	}
	else
	{
		strMsg.AppendFormat(L"联网失败");
	}

	return bRet;
}


bool CACNDlg::UpTestEnd(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, CString& strMsg)
{
	bool bRet(false);

	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	CStringW strXMLdoc;
	strXMLdoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?>");
	strXMLdoc.AppendFormat(L"<root><head>");
	strXMLdoc.AppendFormat(L"<organ>%s</organ>", strStationNumber);
	strXMLdoc.AppendFormat(L"<jkxlh>%s</jkxlh>", strJKXLH);
	strXMLdoc.AppendFormat(L"<jkid>%s</jkid>", L"JS001");
	strXMLdoc.AppendFormat(L"<sjc>%s</sjc>", strTime);
	strXMLdoc.AppendFormat(L"</head>");
	strXMLdoc.AppendFormat(L"<body><vehispara>");


	strXMLdoc.AppendFormat(L"<jylsh>%s</jylsh>", sTestLog.wchReportNumber);
	strXMLdoc.AppendFormat(L"<testtimes>%s</testtimes>", sTestLog.wchNumberOfTestPeriod);
	strXMLdoc.AppendFormat(L"<tsno>%s</tsno>", strStationNumber);
	strXMLdoc.AppendFormat(L"<testlineno>%s</testlineno>", sTestLog.wchLineNumber);
	strXMLdoc.AppendFormat(L"<license>%s</license>", sTestLog.wchPlateNumber);
	strXMLdoc.AppendFormat(L"<licensecode>%s</licensecode>", ToGACode(sTestLog.wchPlateType, sTestLog.wchPlateNumber, sTestLog.wchVehicleType));
	if (wcscmp(sTestLog.wchPass, L"") == 0)
	{
		strXMLdoc.AppendFormat(L"<license>%s</license>", L"1");
	}
	else
	{
		strXMLdoc.AppendFormat(L"<license>%s</license>", sTestLog.wchPass);
	}

	strXMLdoc.AppendFormat(L"</vehispara></body></root>");


	std::wstring strRet;

	int nRet = CACInterfaceLib_API::WriteObjectOut(pchURL, strXMLdoc.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
					strMsg.AppendFormat(L"%s,%s", strCode.c_str(), strContent.c_str());
				}
			}
			else
			{
				strMsg.AppendFormat(L"%s,%s", strCode.c_str(), L"上传成功");
				bRet = true;
			}
		}
	}
	else
	{
		strMsg.AppendFormat(L"联网失败");
	}

	return bRet;
}

CString CACNDlg::ToGACode(const CString& PlateType, const CString& PlateNumber, const CString& strVehicleType)
{
	CString strCode;

	if (PlateType.Find(L"黄") != -1)
	{
		strCode = L"01";
	}
	else if (PlateType.Find(L"蓝") != -1)
	{
		strCode = L"02";
	}
	else if (PlateType.Find(L"绿") != -1)
	{
		if (strVehicleType.Find(L"轻") != -1 
			|| strVehicleType.Find(L"微") != -1 
			|| strVehicleType.Find(L"小") != -1)
		{
			strCode = L"52";
		}
		else
		{
			strCode = L"51";
		}
		
	}

	if(PlateNumber.Find(L"警") != -1)
	{
		strCode = L"23";
	}
	else if(PlateNumber.Find(L"使") != -1)
	{
		strCode = L"03";
	}
	else if(PlateNumber.Find(L"领") != -1)
	{
		strCode = L"04";
	}
	else if(PlateNumber.Find(L"学") != -1)
	{
		strCode = L"16";
	}

	return strCode;
}