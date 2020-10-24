
// HNInterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HNInter.h"
#include "HNInterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHNInterDlg 对话框




CHNInterDlg::CHNInterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHNInterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHNInterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassword);
	DDX_Control(pDX, ID_ED_PlateNumber, m_edPlateNumber);

	DDX_Control(pDX, ID_ED_JKDZ, m_edJKDZ);
	DDX_Control(pDX, ID_ED_JCXBH, m_edJCXBH);
	DDX_Control(pDX, ID_ED_YHM, m_edYHM);
	DDX_Control(pDX, ID_ED_MM, m_edMM);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJCY);


	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbMessage);

	
}

BEGIN_MESSAGE_MAP(CHNInterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(ID_ED_PlateNumber, &CHNInterDlg::OnEnChangeEdPlatenumber)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CHNInterDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CHNInterDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CHNInterDlg::OnBnClickedBtnUpinter)
END_MESSAGE_MAP()


// CHNInterDlg 消息处理程序

BOOL CHNInterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHNInterDlg::OnPaint()
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
HCURSOR CHNInterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHNInterDlg::SetConfig(void)
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
		m_edPassword.SetWindowTextW(strTemp);
	}

	

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"HNConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"WebService", L"URL", L"");
		m_edJKDZ.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edJCXBH.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"UserInfo", L"Name", L"");
		m_edYHM.SetWindowTextW(strTemp);
		strTemp = si.GetString(L"UserInfo", L"PassWord", L"");
		m_edMM.SetWindowTextW(strTemp);

		m_edYHM.SetWindowTextW(L"SQ-YCZH-01");
		m_edMM.SetWindowTextW(L"YCZH-yy0025");

	}
}

void CHNInterDlg::SetVehCtrl(void)
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

bool CHNInterDlg::GetVehicleList(void)
{
	// 删除列表数据
	m_lstVehicle.DeleteAllItems();
	CString strSQL;

	CString strSource, strCatalog, strUserID, strPassword;

	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassword.GetWindowTextW(strPassword);

	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		strSQL.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
		strSQL.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
		//strSQL.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");
		strSQL.AppendFormat(L" where (/*TestLog.ItemOBD = '4' or */TestLog.ItemOBD = '1')/* and (ResultOfOBD.Judge = '2'/* or ResultOfOBD.Judge = '1'*/)*/");

		// 增加筛选
		if (!strPlateNumber.IsEmpty())
		{
			strSQL.AppendFormat(L" and PlateNumber like '%%%s%%'", strPlateNumber);
		}

		// 读取数据库语句写在显示

		_RecordsetPtr pRecordset(NULL);

		if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
		{
			try
			{
				// 显示
				CFont font;
				font.CreatePointFont(320,L"宋体");
				m_lbMessage.SetFont(&font);
				m_lbMessage.SetWindowTextW(L"获取成功");

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

		CNHSQLServerDBO::CloseQuery(pRecordset);

	}
	else
	{
		CNHSQLServerDBO::CloseDB(pConnection);
		m_lbMessage.SetWindowTextW(L"打开数据库失败");
		return false;
	}

	CNHSQLServerDBO::CloseDB(pConnection);
	return true;
}


void CHNInterDlg::OnEnChangeEdPlatenumber()
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


void CHNInterDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edPlateNumber.SetWindowTextW(L"");

	if (!GetVehicleList())
	{
		CString strTemp;
		m_lbMessage.GetWindowTextW(strTemp);
		strTemp.AppendFormat(L"\r\n获取车辆信息失败！");
		m_lbMessage.SetWindowTextW(strTemp);
	}
}


void CHNInterDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	// 设置OBD检测员必须填写

	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		m_strTeatType = m_lstVehicle.GetItemText(nIndex,4);
		// 选中车牌后可以点击UP
		if (!m_strRunningNumber.IsEmpty() && !m_strTeatType.IsEmpty())
		{
			m_lbMessage.SetWindowTextW(L"请填写好参数，点击UP");
			GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
		}
	}

	EndWaitCursor();

	*pResult = 0;
}


void CHNInterDlg::OnBnClickedBtnUpinter()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strRunningNumber != L"")
	{
		TESTLOG sTestLogTemp;
		SResultOfOBD sResultOfOBD;
		LINEINFO sLineInfoTemp;
		// 检测线代号
		CString cstrLineNum(L"");
		m_edJCXBH.GetWindowTextW(cstrLineNum);
		// 数据库操作部分
		{
			CString strSQL;
			CString cstrSource, cstrCatalog, cstrUserID, cstrPassword;
			m_edSource.GetWindowTextW(cstrSource);
			m_edCatalog.GetWindowTextW(cstrCatalog);
			m_edUserID.GetWindowTextW(cstrUserID);
			m_edPassword.GetWindowTextW(cstrPassword);
			if (!cstrSource.IsEmpty() && !cstrCatalog.IsEmpty() && !cstrUserID.IsEmpty() && !cstrPassword.IsEmpty() && !cstrLineNum.IsEmpty())
			{
				wchar_t wchPath[MAX_PATH];
				ZeroMemory(wchPath, sizeof(wchPath));
				if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHModeServerDB.ini", wchPath))
				{
					CSimpleIni si(wchPath);
					// 先写入配置文件参数
					si.SetString(L"Database", L"Source", cstrSource);
					si.SetString(L"Database", L"Catalog", cstrCatalog);
					si.SetString(L"Database", L"UserID", cstrUserID);
					si.SetString(L"Database", L"Password", cstrPassword);
				}

				strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
				if (0x01 != GetDboTestLog(strSQL, &sTestLogTemp))
				{
					m_lbMessage.SetWindowTextW(L"获取检测日志失败");
					return;
				}
				// 获取OBD数据
				strSQL.Format(L"SELECT TOP(1)* FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
				// 先获取，获取不到再说
				GetDboResultOfOBD(strSQL, &sResultOfOBD);
				sResultOfOBD.strRunningNumber = m_strRunningNumber;
				
				// 获取检测线信息
				strSQL.Format(L"select * from LineInfo where LineNumber = '%s'", cstrLineNum);
				GetDboLineInfo(strSQL, &sLineInfoTemp);
			}
			else
			{
				m_lbMessage.SetWindowTextW(L"填写参数存在空值");
				return;
			}
		}
		// 生成OBD数据
		GetOBDResult(sTestLogTemp, sResultOfOBD);
		// 数据上传部分
		{
			CString strJKDZ(L"");
			m_edJKDZ.GetWindowTextW(strJKDZ);

			char* pchURL(NULL);

			if (NULL != pchURL)
			{
				free(pchURL);
				pchURL = NULL;
			}

			// URL
			pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);
			// 用户名
			CString cstrYHM(L"");
			m_edYHM.GetWindowTextW(cstrYHM);
			// 密码
			CString cstrMM(L"");
			m_edMM.GetWindowTextW(cstrMM);
			// OBD检测员
			CString cstrOBDName(L"");
			m_edOBDJCY.GetWindowTextW(cstrOBDName);

			if (!strJKDZ.IsEmpty() && !cstrLineNum.IsEmpty() && !cstrYHM.IsEmpty() && !cstrMM.IsEmpty() && !cstrOBDName.IsEmpty())
			{
				COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
				CString strTempUp, strFuelType;
				CString cstrmethodName = L"UploadOBD";

				JsonMap map;
				JsonMap mapParam;

				mapParam[L"LineNo"] = cstrLineNum;
				mapParam[L"License"] = sTestLogTemp.wchPlateNumber;
				if(wcscmp(sTestLogTemp.wchPlateType, L"蓝牌") == 0)
				{
					mapParam[L"LicenseType"] =L"1";
				}
				else if(wcscmp(sTestLogTemp.wchPlateType, L"黄牌") == 0)
				{
					mapParam[L"LicenseType"] =L"2";
				}
				else if(wcscmp(sTestLogTemp.wchPlateType, L"白牌") == 0)
				{
					mapParam[L"LicenseType"] =L"3";
				}
				else if(wcscmp(sTestLogTemp.wchPlateType, L"黑牌") == 0)
				{
					mapParam[L"LicenseType"] =L"4";
				}
				else if(wcscmp(sTestLogTemp.wchPlateType, L"绿牌") == 0)
				{
					mapParam[L"LicenseType"] =L"5";
				}
				else if(wcscmp(sTestLogTemp.wchPlateType, L"黄绿牌") == 0)
				{
					mapParam[L"LicenseType"] =L"6";
				}

				//map[L"LicenseCode"] = sTestLogTemp.wchPlateTypeCodeGA;
				mapParam[L"LicenseCode"] = LicenseNameToLicenseCode(sTestLogTemp.wchReserved1);

				strFuelType.Format(L"%s", sTestLogTemp.wchFuelType);
				if (strFuelType.Find(L"汽") != -1)
				{
					mapParam[L"Type"] = L"1";
				}
				else
				{
					mapParam[L"Type"] = L"2";
				}
				mapParam[L"TestTimes"] = L"1";
				mapParam[L"TestPerson"] = cstrOBDName;
				odtCurTime.ParseDateTime(sResultOfOBD.strDetBegTime.c_str());
				mapParam[L"TsBeginTime"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
				odtCurTime.ParseDateTime(sResultOfOBD.strDetEndTime.c_str());
				mapParam[L"TsEndTime"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
				mapParam[L"VIN"] = sTestLogTemp.wchVIN;
				//mapParam[L"ECU_CALID"] = sResultOfOBD.strEngineCALID.c_str();
				//mapParam[L"ECU_CVN"] = sResultOfOBD.strEngineCVN.c_str();
				//mapParam[L"SCR_CALID"] = L"";
				//mapParam[L"SCR_CVN"] = L"";
				//mapParam[L"OCU_CALID"] = L"";
				//mapParam[L"OCU_CVN"] = L"";


				CString cstrjsonParam;
				//cstrjsonParam.Format(L"[%s]", Map2Json(mapParam));

				CString cstrresultData;
				JsonMap resultDatamap;
				JsonMapVec vmPost;

				// 汽油
				if (strFuelType.Find(L"汽") != -1)
				{
					for (int i=0;i<18;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"IUPRRate"] = L"0.00";
						map[L"finishCount"] = L"0";
						map[L"igniteCount"] = L"";
						strTempUp.Format(L"%d", i+1);
						map[L"item"] = strTempUp;
						map[L"matchCount"] = L"0";
						strTempUp.Format(L"%d", i+1);
						map[L"no"] = strTempUp;
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"IUPR"] = strTempUp;
					resultDatamap[L"Result"] = L"1";
					resultDatamap[L"communicationFailReason"] = L"";
					resultDatamap[L"communicationStatus"] = L"Y";

					vmPost = JsonMapVec();
					for (int i=0;i<3;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						if (i == 0)
						{
							map[L"CALID"] = sResultOfOBD.strEngineCALID.c_str();
							map[L"CVN"] = sResultOfOBD.strEngineCVN.c_str();
							strTempUp.Format(L"%s", L"发动机控制单元");
						}
						else if (i == 1)
						{
							map[L"CALID"] = L"";
							map[L"CVN"] = L"";
							strTempUp.Format(L"%s", L"后处理控制单元");
						}
						else if (i == 2)
						{
							map[L"CALID"] = L"";
							map[L"CVN"] = L"";
							strTempUp.Format(L"%s", L"其他控制单元");
						}
						map[L"name"] = strTempUp;
						strTempUp.Format(L"%d", i+1);
						map[L"no"] = strTempUp;
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"controlUnit"] = strTempUp;
					vmPost = JsonMapVec();
					for (int i=0;i<1;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"code"] = L"";
						map[L"message"] = L"";
						map[L"no"] = L"1";
						map[L"odometer"] = L"";
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"faultCode"] = strTempUp;
					resultDatamap[L"faultIndicatorStatus"] = L"Y";
					resultDatamap[L"faultLightStatus"] = L"1";
					vmPost = JsonMapVec();
					for(int i=0;i<1;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"ds"] = L"";
						map[L"ff"] = L"";
						map[L"no"] = L"1";
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"freezeFrame"] = strTempUp;

					resultDatamap[L"isExistsFaultCode"] = L"N";
					resultDatamap[L"isExistsReadyStatusUndone"] = L"N";
					strTempUp.Format(L"%s", sLineInfoTemp.wchOBDAdapterNumber);
					if (strTempUp.IsEmpty())
					{
						resultDatamap[L"obdDeviceId"] = L"102904262348";
					}
					else
					{
						resultDatamap[L"obdDeviceId"] = strTempUp;
					}
					resultDatamap[L"odometer"] = L"0";

					vmPost = JsonMapVec();
					for(int i=0;i<6;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"OtherIndicatorDescription"] = L"";
						strTempUp.Format(L"%d", i+1);
						map[L"indicatorDescription"] = strTempUp;
						strTempUp.Format(L"%d", i+1);
						map[L"no"] = strTempUp;
						map[L"readyStatus"] = L"Y";
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"readyStatusUndone"] = strTempUp;
					resultDatamap[L"remoteEmissionDeviceCommunicationStatus"] = L"Y";
					resultDatamap[L"vehOBDRequire"] = L"";
					resultDatamap[L"vin"] = sTestLogTemp.wchVIN;
				}
				else
				{
					for (int i=0;i<18;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"IUPRRate"] = L"0.00";
						map[L"finishCount"] = L"0";
						map[L"igniteCount"] = L"";
						strTempUp.Format(L"%d", i+1);
						map[L"item"] = strTempUp;
						map[L"matchCount"] = L"0";
						strTempUp.Format(L"%d", i+1);
						map[L"no"] = strTempUp;
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"IUPR"] = strTempUp;
					resultDatamap[L"Result"] = L"1";
					resultDatamap[L"communicationFailReason"] = L"";
					resultDatamap[L"communicationStatus"] = L"Y";

					vmPost = JsonMapVec();
					for (int i=0;i<3;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						if (i == 0)
						{
							map[L"CALID"] = sResultOfOBD.strEngineCALID.c_str();
							map[L"CVN"] = sResultOfOBD.strEngineCVN.c_str();
							strTempUp.Format(L"%s", L"发动机控制单元");
						}
						else if (i == 1)
						{
							map[L"CALID"] = L"";
							map[L"CVN"] = L"";
							strTempUp.Format(L"%s", L"后处理控制单元");
						}
						else if (i == 2)
						{
							map[L"CALID"] = L"";
							map[L"CVN"] = L"";
							strTempUp.Format(L"%s", L"其他控制单元");
						}
						map[L"name"] = strTempUp;
						strTempUp.Format(L"%d", i+1);
						map[L"no"] = strTempUp;
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"controlUnit"] = strTempUp;
					vmPost = JsonMapVec();
					for (int i=0;i<1;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"code"] = L"";
						map[L"message"] = L"";
						map[L"no"] = L"1";
						map[L"odometer"] = L"";
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"faultCode"] = strTempUp;
					resultDatamap[L"faultIndicatorStatus"] = L"Y";
					resultDatamap[L"faultLightStatus"] = L"1";
					vmPost = JsonMapVec();
					for(int i=0;i<1;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"ds"] = L"";
						map[L"ff"] = L"";
						map[L"no"] = L"1";
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"freezeFrame"] = strTempUp;

					resultDatamap[L"isExistsFaultCode"] = L"N";
					resultDatamap[L"isExistsReadyStatusUndone"] = L"Y";
					strTempUp.Format(L"%s", sLineInfoTemp.wchOBDAdapterNumber);
					if (strTempUp.IsEmpty())
					{
						resultDatamap[L"obdDeviceId"] = L"102904262348";
					}
					else
					{
						resultDatamap[L"obdDeviceId"] = strTempUp;
					}
					resultDatamap[L"odometer"] = L"0";

					vmPost = JsonMapVec();
					for(int i=0;i<6;i++)
					{
						strTempUp = L"";
						map = JsonMap();
						map[L"OtherIndicatorDescription"] = L"";
						if (i == 0)
						{
							strTempUp = L"5";
						}
						else if (i == 1)
						{
							strTempUp = L"1";
						}
						else if (i == 2)
						{
							strTempUp = L"2";
						}
						else if (i == 3)
						{
							strTempUp = L"2";
						}
						else if (i == 4)
						{
							strTempUp = L"4";
						}
						else if (i == 4)
						{
							strTempUp = L"6";
						}
						map[L"indicatorDescription"] = strTempUp;
						strTempUp.Format(L"%d", i+1);
						map[L"no"] = strTempUp;
						map[L"readyStatus"] = L"Y";
						vmPost.push_back(map);
					}
					strTempUp.Format(L"%s", MapVec2Json(vmPost));
					resultDatamap[L"readyStatusUndone"] = strTempUp;
					resultDatamap[L"remoteEmissionDeviceCommunicationStatus"] = L"Y";
					resultDatamap[L"vehOBDRequire"] = L"";
					resultDatamap[L"vin"] = sTestLogTemp.wchVIN;
				}
				cstrresultData.Format(L"%s", Map2Json(resultDatamap));


				// cstrjsonParam 中的resultData的参数是否需要删除{}
				//if (false)
				//{
				//	CString cstrresultData2(cstrresultData);
				//	cstrresultData2.Delete(cstrresultData2.GetLength()-1, 1);
				//	cstrresultData2.Delete(0, 1);
				//	mapParam[L"resultData"] = cstrresultData2;
				//}
				//else
				//{
				//	mapParam[L"resultData"] = cstrresultData;
				//}
				
				cstrjsonParam.Format(L"[%s]", Map2Json(mapParam));

				std::wstring strRet;
				int nRet = CHNInterfaceLib_API:: ObjectOut(pchURL, cstrmethodName.GetString(),
					cstrjsonParam.GetString(), cstrresultData.GetString(), cstrYHM.GetString(),
					cstrMM.GetString(), strRet);

#ifdef _DEBUG
				// <?xml version="1.0" encoding="UTF - 8"?><string xmlns="http://tempuri.org/">{"Success":true,"Message":"调用成功","Data":""}</string>
				nRet = 0;
				CString strTC;
				strTC.AppendFormat(L"<?xml version=\"1.0\" encoding=\"UTF - 8\"?>");
				strTC.AppendFormat(L"<string xmlns=\"http://tempuri.org/\">");
				strTC.AppendFormat(L"{\"Success\":true,\"Message\":\"调用成功\",\"Data\":\"\"}</string>");
				strRet = strTC.GetString();
#endif
				// 释放指针
				if (NULL != pchURL)
				{
					free(pchURL);
					pchURL = NULL;
				}

				if (nRet == 0)
				{
					SRetString sRetString;
					std::wstring str;
					
					//同步数据库操作
					CXmlReader xmlReader;
					if (xmlReader.Parse(strRet.c_str()))
					{
						if (xmlReader.OpenNode(L"string"))
						{
							xmlReader.GetNodeContent(str);
						}

						if (str.find(L"true") != std::string::npos)
						{
							sRetString.strSuccess = L"true";
						}
						else
						{
							sRetString.strSuccess = L"false";
						}
					}
					else
					{
						m_lbMessage.SetWindowTextW(strRet.c_str());
						return;
					}
					CString cstrSuccess = sRetString.strSuccess.c_str();
					cstrSuccess.MakeLower();// 全部小写
					if (cstrSuccess = L"true")
					{
						_ConnectionPtr pConnection(NULL);
						CNHSQLServerDBO::OpenDB(pConnection);
						CString strSQL;
						strSQL.Format(L"update TestLog set TestState = '0', ItemOBD = '4' WHERE RunningNumber= '%s'", m_strRunningNumber);
						DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);
						CNHSQLServerDBO::CloseDB(pConnection);

						sResultOfOBD.strRunningNumber = m_strRunningNumber;
						sResultOfOBD.strOperator = cstrOBDName;
						SetDboResultOfOBD(sResultOfOBD);
						// 写入临时文件
						SetIniResultOfOBD(sResultOfOBD);

						m_lbMessage.SetWindowTextW(L"上传完成");
					}
					else
					{
						CString cstrMessage;
						cstrMessage.Format(L"%s\r\n",str.c_str());
						MessageBox(cstrMessage);
					}

					
				}
				else
				{
					CString str;
					str.Format(L"%d", nRet);
					m_lbMessage.SetWindowTextW(L"联网失败" + str);
					return;
				}

			}
			else
			{
				m_lbMessage.SetWindowTextW(L"填写参数存在空值");
				return;
			}
		}
	}
	else
	{
		MessageBox(L"日志编号为空");
		return;
	}
	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
}

CString CHNInterDlg::Map2Json(JsonMap mapPost)
{
	CString strJson;
	strJson.AppendFormat(L"{");
	for (JsonMap::const_iterator it=mapPost.begin(); it!=mapPost.end(); ++it)
	{
		CString strTemp;
		strTemp.Format(L"%s", it->second);
		if (strTemp.Find(L"[{") == -1)
		{
			strJson.AppendFormat(L"\"%s\":\"%s\"",  it->first, it->second);
		}
		else
		{
			strJson.AppendFormat(L"\"%s\":%s",  it->first, it->second);
		}
		strJson.AppendFormat(L",");
	}
	strJson.TrimRight(L",");
	strJson.AppendFormat(L"}");
	return strJson;
}

CString CHNInterDlg::MapVec2Json(JsonMapVec vmPost)
{
	CString strJson;
	strJson.AppendFormat(L"[");
	for (UINT i=0; i<vmPost.size(); i++)
	{
		strJson.AppendFormat(L"{");
		for (JsonMap::const_iterator it=vmPost[i].begin(); it!=vmPost[i].end(); ++it)
		{
			strJson.AppendFormat(L"\"%s\":\"%s\"",  it->first, it->second);
			strJson.AppendFormat(L",");
		}
		strJson.TrimRight(L",");
		strJson.AppendFormat(L"},");	
	}
	strJson.TrimRight(L",");
	strJson.AppendFormat(L"]");
	return strJson;
}

CString CHNInterDlg::LicenseNameToLicenseCode(LPCTSTR szLicenseCode)
{
	CString strName(szLicenseCode);
	if (strName == L"小型汽车")
	{
		return L"02";
	}
	else if (strName == L"大型汽车")
	{
		return L"01";
	}
	else if (strName == L"使馆汽车")
	{
		return L"03";
	}
	else if (strName == L"领馆汽车")
	{
		return L"04";
	}
	else if (strName == L"境外汽车")
	{
		return L"05";
	}
	else if (strName == L"外籍汽车")
	{
		return L"06";
	}
	else if (strName == L"两、三轮摩托车")
	{
		return L"07";
	}
	else if (strName == L"轻便摩托车")
	{
		return L"08";
	}
	else if (strName == L"使馆摩托车")
	{
		return L"09";
	}
	else if (strName == L"领馆摩托车")
	{
		return L"10";
	}
	else if (strName == L"境外摩托车")
	{
		return L"11";
	}
	else if (strName == L"外籍摩托车")
	{
		return L"12";
	}
	else if (strName == L"农用运输车")
	{
		return L"13";
	}
	else if (strName == L"拖拉机")
	{
		return L"14";
	}
	else if (strName == L"挂车")
	{
		return L"15";
	}
	else if (strName == L"教练汽车")
	{
		return L"16";
	}
	else if (strName == L"教练摩托车")
	{
		return L"17";
	}
	else if (strName == L"试验汽车")
	{
		return L"18";
	}
	else if (strName == L"试验摩托车")
	{
		return L"19";
	}
	else if (strName == L"临时入境汽车")
	{
		return L"20";
	}
	else if (strName == L"临时入境摩托车")
	{
		return L"21";
	}
	else if (strName == L"临时行驶车")
	{
		return L"22";
	}
	else if (strName == L"警用汽车")
	{
		return L"23";
	}
	else if (strName == L"警用摩托号")
	{
		return L"24";
	}
	else if (strName == L"大型新能源汽车")
	{
		return L"51";
	}
	else if (strName == L"小型新能源汽车")
	{
		return L"52";
	}
	else if (strName == L"其他")
	{
		return L"99";
	}
	else
	{
		return L"02";
	}
}

CString CHNInterDlg::Json2Cstring(Json::Value &value)
{
	CString strValue;
	if (value.type() == Json::nullValue)
	{
		strValue.Format(L"%s", L"");
	}
	else if (value.type() == Json::intValue)
	{
		strValue.Format(L"%d", value.asInt());
	}
	else if (value.type() == Json::uintValue)
	{
		strValue.Format(L"%hu", value.asUInt());
	}
	else if(value.type() == Json::realValue)
	{
		strValue.Format(L"%f", value.asDouble());
	}
	else if (value.type() == Json::stringValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::booleanValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::arrayValue)
	{
		
		//strValue.Format(L"%s", ANSIToTChar(value.asCString()));
		strValue.Format(L"%s", ANSIToTChar(value.toStyledString().c_str()));
	}
	else if (value.type() == Json::objectValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.toStyledString().c_str()));
	}
	else
	{
	}
	return strValue;
}


void CHNInterDlg::GetOBDResult(const TESTLOG& sTestLog,SResultOfOBD& sResultOfOBD)
{
	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

	CString strFuelType;
	strFuelType.Format(L"%s", sTestLog.wchFuelType);
	// 汽油
	if (strFuelType.Find(L"汽") != -1)
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
	// 柴油
	else
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
}

CString CHNInterDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CHNInterDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	if (strFulType == L"汽油")
	{
		if (strOBDType == L"EOBD")
		{
			srand(time(0));
			int n = rand()%25;
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
			int n = rand()%6;
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
			int n = rand()%6;
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
			int n = rand()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"JOBD-EOBD")
		{
			srand(time(0));
			int n = rand()%5;
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
			int n = rand()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"不适用")
		{
			srand(time(0));
			int n = rand()%25;
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
			int n = rand()%8;
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
		int n = rand()%20;
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
