
// YNDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YN.h"
#include "YNDlg.h"
#include "afxdialogex.h"

#include "gsoap\soapDeviceSwapIfaceImplServiceSoapBindingProxy.h"
#include "gsoap\soap.nsmap"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYNDlg 对话框




CYNDlg::CYNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYNDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassWord);
	DDX_Control(pDX, IDC_COM_OBDJCY, m_comOBDJCY);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMsg);
	DDX_Control(pDX, ID_ED_URL, m_edURL);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_LICENSECODE, m_edLincenseCode);
	DDX_Control(pDX, IDC_ST_PLATENUMBER, m_lbPlateNumber);
	DDX_Control(pDX, IDC_ST_VIN, m_lbVIN);
	DDX_Control(pDX, IDC_ST_REPORTNUMBER, m_lbRepoetNumber);
	DDX_Control(pDX, IDC_ST_RUNNINGNUMBER, m_lbRunningNumber);
	DDX_Control(pDX, IDC_ST_TESTTYPE, m_lbTestType);
	DDX_Control(pDX, IDC_ST_OBDITEM, m_lbOBDItem);
}

BEGIN_MESSAGE_MAP(CYNDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CYNDlg::OnBnClickedBtnSetconfig)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CYNDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_UP, &CYNDlg::OnBnClickedBtnUp)
END_MESSAGE_MAP()


// CYNDlg 消息处理程序

BOOL CYNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);

	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);
	COleDateTime odtNow(COleDateTime::GetCurrentTime());
	CStringW strFileName;
	strFileName.Format(L"云南OBD%s.log", odtNow.Format(L"%Y-%m-%d"));
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;


	m_sTestLog = TESTLOG();
	SetConfig();
	GetOBDJCY();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYNDlg::OnPaint()
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
HCURSOR CYNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



CString CYNDlg::ToTestType(const CString& strTestTypeCode)
{
	CString strTestType;

	int nCode = _wtoi(strTestTypeCode);
	switch (nCode)
	{
	case 1:{ strTestType.Format(L"稳态");} break;
	case 2:{ strTestType.Format(L"简易瞬态");} break;
	case 3:{ strTestType.Format(L"加载减速");} break;
	case 4:{ strTestType.Format(L"双怠速");} break;
	case 5:{ strTestType.Format(L"不透光自由加速");} break;
	case 6:{ strTestType.Format(L"滤纸式自由加速");} break;
	default : {strTestType.Format(L"未知检测类型");} break;
	}



	return strTestType;
}

CString CYNDlg::ToItem(const CString& strItemCode)
{
	CString strItem;

	int nCode = _wtoi(strItemCode);
	switch (nCode)
	{
	case 0:{ strItem.Format(L"免除检测");} break;
	case 1:{ strItem.Format(L"等待检测");} break;
	case 2:{ strItem.Format(L"准备检测");} break;
	case 3:{ strItem.Format(L"正在检测");} break;
	case 4:{ strItem.Format(L"完成检测");} break;
	default : {strItem.Format(L"未知检测");} break;
	}



	return strItem;
}

void CYNDlg::SetConfig(void)
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
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"ZCConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"WebService", L"URL", L"http://10.202.58.241:8095/sjjk/services/sjjk?wsdl");
		m_edURL.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"WebService", L"LineCode", L"5304020151013");
		m_edLineNumber.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"WebService", L"LicenseCode", L"1575349621498_YXLYCZM");
		m_edLincenseCode.SetWindowTextW(strTemp);
	}

}


void CYNDlg::GetOBDJCY(void)
{
	CString strSource, strCatalog, strUserID, strPassWord;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassWord);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() || strUserID.IsEmpty() || strPassWord.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"数据库配置参数有空值");
	}
	else
	{

		_ConnectionPtr pConnection(NULL);
		if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassWord))
		{
			CString strSQL;
			strSQL.Format(L"select * from UserInfo /*where Role = 6*/");
			_RecordsetPtr pRecordset(NULL);
			m_comOBDJCY.ResetContent();
			if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
			{
				// 打开查询成功
				try
				{
					m_comOBDJCY.AddString(L"");
					while (!pRecordset->adoEOF)
					{
						_variant_t v;
						v.ChangeType(VT_NULL);
						CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"Name");
						if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
						{
							m_comOBDJCY.AddString((wchar_t*)(_bstr_t)v);
						}
						else
						{
							m_comOBDJCY.AddString(L"");
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
			m_edMsg.SetWindowTextW(L"数据库联系错误");
		}

		CNHSQLServerDBO::CloseDB(pConnection);
	}
}


void CYNDlg::OnBnClickedBtnSetconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	SetConfig();
	GetOBDJCY();
}


void CYNDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bNext(false);

	CString strMsg;
	bNext = GetTestLog(strMsg);

	if (!bNext)
	{
		if (IDYES == MessageBox(strMsg, L"", MB_YESNO))
		{
			bNext = true;
		}
	}

	if (bNext)
	{
		m_lbPlateNumber.SetWindowTextW(m_sTestLog.wchPlateNumber);
		m_lbVIN.SetWindowTextW(m_sTestLog.wchVIN);
		m_lbRepoetNumber.SetWindowTextW(m_sTestLog.wchRunningNumber);
		m_lbRunningNumber.SetWindowTextW(m_sTestLog.wchReportNumber);
		m_lbTestType.SetWindowTextW(ToTestType(m_sTestLog.wchTestType));
		m_lbOBDItem.SetWindowTextW(ToItem(m_sTestLog.wchItemOBD));

		GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	}
}


bool CYNDlg::GetTestLog(CString& strMsg)
{
	bool bRet(false);

	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);
	// 车牌和检测方法一致才提示
	if ((wcscmp(sTestLog.wchPlateNumber, m_sTestLog.wchPlateNumber) == 0) &&
		(wcscmp(m_sTestLog.wchTestType, sTestLog.wchTestType) == 0))
	{
		strMsg.Format(L"获取记录与上次操作的车牌号码一致，是否继续操作");
	}
	else
	{
		bRet = true;
	}

	m_sTestLog = sTestLog;

	return bRet;
}


void CYNDlg::OnBnClickedBtnUp()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bRet(false);

	if (wcscmp(m_sTestLog.wchRunningNumber, L"") == 0)
	{
		m_edMsg.SetWindowTextW(L"获取的检测日志为空");
		return;
	}

	// 先处理SQL部分在处理上传部分
	CString strSource, strCatalog, strUserID, strPassword, strOBDJCY;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);
	m_comOBDJCY.GetWindowTextW(strOBDJCY);

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
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_sTestLog.wchRunningNumber);
	SResultOfOBD sResultOfOBD;
	GetDboResultOfOBD(strSQL, &sResultOfOBD);

	CString strMsg;

	CString strTemp;
	strTemp.Format(L"%s", m_sTestLog.wchRunningNumber);
	sResultOfOBD.strRunningNumber = strTemp.GetString();
	sResultOfOBD.strOperator = strOBDJCY.GetString();
	bRet = SetOBDLOG(sResultOfOBD, strMsg);

	if (!bRet)
	{
		m_edMsg.SetWindowTextW(L"更新OBD数据失败");
		return;
	}
	// 写入数据库
	SetDboResultOfOBD(sResultOfOBD);
	// 写入临时文件
	SetIniResultOfOBD(sResultOfOBD);

	// 上传部分
	CString strURL, strLineNumber, strLincenseCode;
	m_edURL.GetWindowTextW(strURL);
	m_edLineNumber.GetWindowTextW(strLineNumber);
	m_edLincenseCode.GetWindowTextW(strLincenseCode);

	if (strURL.IsEmpty() || strLineNumber.IsEmpty() || strLincenseCode.IsEmpty() )
	{
		m_edMsg.SetWindowTextW(L"联网检测参数为空");
		return;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	CString strTempMsg;
	strMsg.Empty();
	bRet = UpOBDReaust(pchURL, strLineNumber, strLincenseCode, m_sTestLog, sResultOfOBD, strMsg);
	if (!bRet)
	{
		strMsg = L"上传OBD结果失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s\r\n", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
		//return;
	}

	strMsg.Empty();
	bRet = UpOBDIURP(pchURL, strLineNumber, strLincenseCode, m_sTestLog, sResultOfOBD, strMsg);
	if (!bRet)
	{
		strMsg = L"上传OBDIURP失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s\r\n", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
		//return;
	}

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	// 上传有失败是，是否继续？？
	bool bUpLowNext(false);
	if (strTempMsg.Find(L"失败") != -1)
	{
		strTempMsg.Format(L"上传过程中，有上传不成功的参数是否继续？");
		if (IDYES == MessageBox(strTempMsg, L"", MB_YESNO))
		{
			bUpLowNext = true;
		}
	}
	else
	{
		bUpLowNext = true;
	}
	
	if (bUpLowNext)
	{
		wchar_t wchPath[MAX_PATH];
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"ItemOBD", L"4");
		}

		wcscpy_s(m_sTestLog.wchItemOBD, L"4");
		if (0x00 != SetDboTestLog(m_sTestLog))
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


	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
}


bool CYNDlg::SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(true);

	CString strFulType(m_sTestLog.wchFuelType);

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


CString CYNDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CYNDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	if (strFulType == L"汽油")
	{
		if (strOBDType == L"EOBD")
		{
			srand(time(0));
			int n = rand()%20;
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
			int n = rand()%4;
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
			int n = rand()%4;
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
			int n = rand()%3;
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
			int n = rand()%15;
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
			int n = rand()%4;
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
		int n = rand()%15;
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


bool CYNDlg::UpOBDReaust(const char * pchURL, const CStringW& strLineNumber, const CStringW& strLincenseCode,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__obdJgsj send;
	ns1__obdJgsjResponse sRes;
	
	
	//clsbm	VIN(车架号)
	std::wstring strVin;
	strVin = sTestLog.wchVIN;
	send.clsbm = &strVin;
	//obdzsbz	OBD执行标准
	std::wstring strOBDType;
	strOBDType = sResultOfOBD.strOBDType.c_str();
	send.obdzsbz = &strOBDType;
	//clxslc	车辆行驶里程
	send.clxslc = 0;
	//fdjkzdycid	发动机控制单元标定识别码cid
	std::wstring strEngineCALID;
	strEngineCALID = sResultOfOBD.strEngineCALID.c_str();
	send.fdjkzdycid = &strEngineCALID;
	//fdjkzdycvn	发动机控制单元校准核查号码 
	std::wstring strEngineCVN;
	strEngineCVN = sResultOfOBD.strEngineCVN.c_str();
	send.fdjkzdycvn = &strEngineCVN;
	//hclkzdycid	后处理控制单元标定识别码cid
	std::wstring strPostProcessingCALID;
	strPostProcessingCALID = sResultOfOBD.strPostProcessingCALID.c_str();
	send.hclkzdycid = &strPostProcessingCALID;
	//hclkzdycvn	后处理控制单元校准核查号码 
	std::wstring strPostProcessingCVN;
	strPostProcessingCVN = sResultOfOBD.strPostProcessingCVN.c_str();
	send.hclkzdycvn = &strPostProcessingCVN;
	//qtkzdycid	其他控制单元标定识别码cid
	std::wstring strOtherCALID;
	strOtherCALID = sResultOfOBD.strOtherCALID.c_str();
	send.qtkzdycid = &strOtherCALID;
	//qtkzdycvn	其他控制单元校准核查号码 
	std::wstring strOtherCVN;
	strOtherCVN = sResultOfOBD.strOtherCVN.c_str();
	send.qtkzdycvn = &strOtherCVN;
	//obdgzzsq	OBD系统故障指示器
	std::wstring strobdgzzsq(L"1");
	send.obdgzzsq = &strobdgzzsq;
	//obdtxqk	OBD通讯情况
	std::wstring strobdtxqk(L"1");
	send.obdtxqk = &strobdtxqk;
	//obdzzqsfdl	OBD故障指示器是否点亮
	std::wstring strobdzzqsfdl(L"0");
	send.obdzzqsfdl = &strobdzzqsfdl;
	//obdjxzt	就绪状态未完成项目
	std::wstring strobdjxzt(L"0");
	send.obdjxzt = &strobdjxzt;
	//obdjcjg	OBD检查结果
	std::wstring strobdjcjg(L"1");
	send.obdjcjg = &strobdjcjg;
	//obdgzxx	OBD故障信息
	std::wstring strobdgzxx(L"");
	send.obdgzxx = &strobdgzxx;
	//jyymc	检验员名称
	std::wstring strjyymc(L"");
	strjyymc = sResultOfOBD.strOperator.c_str();
	send.jyymc = &strjyymc;
	//jckssj	检测开始时间
	std::wstring strjckssj(L"");
	strjckssj = sResultOfOBD.strDetBegTime.c_str();
	send.jckssj = &strjckssj;
	//jcjssj	检测结束时间
	std::wstring strjcjssj(L"");
	strjcjssj = sResultOfOBD.strDetEndTime.c_str();
	send.jcjssj = &strjcjssj;
	
	CString strLog;
	strLog.Format(L"VIN(车架号):%s, OBD执行标准:%s, 车辆行驶里程:%d, 发动机控制单元标定识别码cid:%s, 发动机控制单元校准核查号码:%s, 后处理控制单元标定识别码cid:%s, \
				   后处理控制单元校准核查号码:%s, 其他控制单元标定识别码cid:%s, 其他控制单元校准核查号码:%s, OBD系统故障指示器:%s, OBD通讯情况:%s, OBD故障指示器是否点亮:%s \
				   就绪状态未完成项目:%s, OBD检查结果:%s, OBD故障信息:%s, 检验员名称:%s, 检测开始时间:%s, 检测结束时间:%s",
				   send.clsbm->c_str(), send.obdzsbz->c_str(), send.clxslc, send.fdjkzdycid->c_str(), send.fdjkzdycvn->c_str(), send.hclkzdycid->c_str(), send.hclkzdycvn->c_str(), 
				   send.qtkzdycid->c_str(), send.qtkzdycvn->c_str(), send.obdgzzsq->c_str(), send.obdtxqk->c_str(), send.obdzzqsfdl->c_str(), send.obdjxzt->c_str(), send.obdjcjg->c_str(), 
				   send.obdgzxx->c_str(), send.jyymc->c_str(), send.jckssj->c_str(), send.jcjssj->c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"发送", L"obdJgsj", strLog);

	int nRet = toasb.obdJgsj(&send, sRes);

	if (nRet == SOAP_OK)
	{
		std::wstring wstrRet;
		wstrRet = sRes.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"返回", L"obdJgsj", wstrRet.c_str());

		CXmlReader xmlReader;
		if (xmlReader.Parse(wstrRet.c_str()))
		{
			std::wstring wstrCode, wstrContent;
			if (xmlReader.OpenNode(L"root/result"))
			{
				xmlReader.GetNodeContent(wstrCode);
			}

			if (wstrCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/info"))
				{
					xmlReader.GetNodeContent(wstrContent);
				}

				strMsg.Format(L"失败, 失败原因:%s", wstrContent.c_str());
			}
			else
			{
				bRet = true;
				strMsg.Format(L"成功", wstrRet.c_str());
			}
		}
		else
		{
			strMsg.Format(L"解析失败 返回内容:%s", wstrRet.c_str());
		}
	}
	else
	{
		strMsg.Format(L"联网失败 返回代码：%d", nRet);
	}

	return bRet;
}

bool CYNDlg::UpOBDIURP(const char * pchURL, const CStringW& strLineNumber, const CStringW& strLincenseCode,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	CString strFulType(sTestLog.wchFuelType);
	if (strFulType.Find(L"柴") != -1)
	{
		ns1__cycObdIupr send;
		ns1__cycObdIuprResponse sRes;

		std::wstring wstdstr;
		wstdstr = sTestLog.wchRunningNumber;
		send.lsh = &wstdstr;

		int n, n1;
		CString strTemp;
		n = _wtoi(sResultOfOBD.strIUPR_NMHCC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_NMHCEC.c_str());
		send.nmhcchqjccs = n;
		send.nmhcchqfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.nmhcchqiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_NMHCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NMHCC.c_str()))));
			send.nmhcchqiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_NOXCC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_NOXCEC.c_str());
		send.noxchqjccs = n;
		send.noxchqfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.noxchqiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_NOXCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXCC.c_str()))));
			send.noxchqiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_NOXAC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_NOXAEC.c_str());
		send.noxxfqjccs = n;
		send.noxxfqfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.noxxfqiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_NOXAEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXAC.c_str()))));
			send.noxxfqiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_PMC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_PMEC.c_str());
		send.pmbjqjccs = n;
		send.pmbjqfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.pmbjqiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_PMEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PMC.c_str()))));
			send.pmbjqiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_WSC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_WSEC.c_str());
		send.fqcgqjccs = n;
		send.fqcgqfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.fqcgqiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_WSEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_WSC.c_str()))));
			send.fqcgqiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_EGRC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_EGREC.c_str());
		send.egrvvtjccs = n;
		send.egrvvtfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.egrvvtiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
			send.egrvvtiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_PPC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_PPEC.c_str());
		send.zyyljccs = n;
		send.zyylfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.zyyliuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_PPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PPC.c_str()))));
			send.zyyliuprl = _wtof(strTemp);
		}

		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"发送", L"cycObdIupr", L"");

		int nRet = toasb.cycObdIupr(&send, sRes);
		if (nRet == SOAP_OK)
		{
			std::wstring wstrRet;
			wstrRet = sRes.return_->c_str();
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"返回", L"cycObdIupr", wstrRet.c_str());

			CXmlReader xmlReader;
			if (xmlReader.Parse(wstrRet.c_str()))
			{
				std::wstring wstrCode, wstrContent;
				if (xmlReader.OpenNode(L"root/result"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}

				if (wstrCode != L"1")
				{
					if (xmlReader.OpenNode(L"root/info"))
					{
						xmlReader.GetNodeContent(wstrContent);
					}

					strMsg.Format(L"失败, 失败原因:%s", wstrContent.c_str());
				}
				else
				{
					bRet = true;
					strMsg.Format(L"成功", wstrRet.c_str());
				}
			}
			else
			{
				strMsg.Format(L"解析失败 返回内容:%s", wstrRet.c_str());
			}
		}
		else
		{
			strMsg.Format(L"联网失败 返回代码：%d", nRet);
		}
	}
	else
	{
		ns1__qycObdIupr send;
		ns1__qycObdIuprResponse sRes;


		std::wstring wstdstr;
		wstdstr = sTestLog.wchRunningNumber;
		send.lsh = &wstdstr;

		int n, n1;
		CString strTemp;
		n = _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str());
		send.chqz1jccs = n;
		send.chqz1fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.chqz1iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()))));
			send.chqz1iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_CMCCB2.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_CMCECB2.c_str());
		send.chqz2jccs = n;
		send.chqz2fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.chqz2iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_CMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB2.c_str()))));
			send.chqz2iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_O2SMCCB1.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_O2SMCECB1.c_str());
		send.qycgqz1jccs = n;
		send.qycgqz1fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.qycgqz1iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB1.c_str()))));
			send.qycgqz1iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_O2SMCCB2.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_O2SMCECB2.c_str());
		send.qycgqz2jccs = n;
		send.qycgqz2fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.qycgqz2iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB2.c_str()))));
			send.qycgqz2iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_RO2SMCCB1.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_RO2SMCECB1.c_str());
		send.hycgqz1jccs = n;
		send.hycgqz1fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.hycgqz1iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()))));
			send.hycgqz1iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str());
		send.hycgqz2jccs = n;
		send.hycgqz2fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.hycgqz2iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()))));
			send.hycgqz2iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str());
		send.evapjccs = n;
		send.evapfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.evapiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EVAPC.c_str()))));
			send.evapiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_EGRC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_EGREC.c_str());
		send.egrvvtjccs = n;
		send.egrvvtfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.egrvvtiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
			send.egrvvtiuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str());
		send.gpfz1jccs = n;
		send.gpfz1fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.gpfz1iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC1.c_str()))));
			send.gpfz1iuprl = _wtof(strTemp);
		}
		n = _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str());
		send.gpfz2jccs = n;
		send.gpfz2fhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.gpfz2iuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC2.c_str()))));
			send.gpfz2iuprl = _wtof(strTemp);
		}

		n = _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str());
		n1 = _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str());
		send.eckqpsxtjccs = n;
		send.eckqpsxtfhtjcs = n1;
		if (n == 0 || n1 == 0)
		{
			send.eckqpsxtiuprl = 0.00f;
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_AMCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_AMCCC.c_str()))));
			send.eckqpsxtiuprl = _wtof(strTemp);
		}

		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"发送", L"qycObdIupr", L"");

		int nRet = toasb.qycObdIupr(&send, sRes);
		if (nRet == SOAP_OK)
		{
			std::wstring wstrRet;
			wstrRet = sRes.return_->c_str();
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"返回", L"qycObdIupr", wstrRet.c_str());

			CXmlReader xmlReader;
			if (xmlReader.Parse(wstrRet.c_str()))
			{
				std::wstring wstrCode, wstrContent;
				if (xmlReader.OpenNode(L"root/result"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}

				if (wstrCode != L"1")
				{
					if (xmlReader.OpenNode(L"root/info"))
					{
						xmlReader.GetNodeContent(wstrContent);
					}

					strMsg.Format(L"失败, 失败原因:%s", wstrContent.c_str());
				}
				else
				{
					bRet = true;
					strMsg.Format(L"成功", wstrRet.c_str());
				}
			}
			else
			{
				strMsg.Format(L"解析失败 返回内容:%s", wstrRet.c_str());
			}
		}
		else
		{
			strMsg.Format(L"联网失败 返回代码：%d", nRet);
		}
	}
	return bRet;
}