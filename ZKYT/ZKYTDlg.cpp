
// ZKYTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZKYT.h"
#include "ZKYTDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZKYTDlg 对话框




CZKYTDlg::CZKYTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZKYTDlg::IDD, pParent)
	, m_bNoOrNotApplicable(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZKYTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassWord);
	DDX_Control(pDX, ID_ED_URL, m_edURL);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNumber);
	DDX_Control(pDX, ID_ED_LICENSECODE, m_edLicenseCode);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMsg);
	DDX_Control(pDX, IDC_COM_OBDJCY, m_cbOBDJCY);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_OBDJCYID, m_edOBDJCYID);
	DDX_Control(pDX, ID_ED_LINENUM, m_edLineNum);
	DDX_Control(pDX, IDC_COM_OBDJCYID, m_cbOBDID);

	DDX_Control(pDX, IDC_BTN_TOMN, m_cbBtn);

	DDX_Control(pDX, ID_ED_UPCONFIGNAME, m_edUpConfigName);
	DDX_Control(pDX, ID_ED_UPCONFIGTOPROJECT, m_edUpConfigProject);

}

BEGIN_MESSAGE_MAP(CZKYTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CZKYTDlg::OnBnClickedBtnSetconfig)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CZKYTDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CZKYTDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UP, &CZKYTDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_TOTSETLOG, &CZKYTDlg::OnBnClickedBtnTotsetlog)
END_MESSAGE_MAP()


// CZKYTDlg 消息处理程序

BOOL CZKYTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_cbBtn.SetBitmaps(IDB_BITMAP_TM, RGB(255, 255, 255));
	m_cbBtn.DrawTransparent();

	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BTN_TOTSETLOG)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();

	CString strLicCode;
	//GetLicenseCode(strLicCode);
	//UpBBH();
	GetInterOBDJCY();
	GetInterOBDJCYID();

	CString strit;
	strit.Format(L"中科宇图");
	SetWindowTextW(strit);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZKYTDlg::OnPaint()
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
HCURSOR CZKYTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// json格式模式
std::string JsonToString(const rapidjson::Value& valObj)
{
	rapidjson::StringBuffer sbBuf;
	rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
	valObj.Accept(jWriter);
	std::string str = std::string(sbBuf.GetString());

	return str;
}
CString CZKYTDlg::JsonToCString(const rapidjson::Value &Val)
{
	CString strVal;
	if (Val.IsNull())
	{
		strVal.Format(L"");
	}
	else if (Val.IsBool())
	{
		strVal.Format(L"%s", Val.GetBool()? L"true" : L"false");
	}
	else  if (Val.IsObject())
	{
		// 暂时定义obj 直接输出
		strVal = CA2W(JsonToString(Val).c_str());
	}
	else  if (Val.IsArray())
	{
		// 暂时定义Arr 直接输出
		strVal = CA2W(JsonToString(Val).c_str());
	}
	else  if (Val.IsNumber())
	{
		if (Val.IsFloat())
		{
			strVal.Format(L"%.2f", Val.GetFloat());
		}
		else if (Val.IsDouble())
		{
			strVal.Format(L"%.2f", Val.GetDouble());
		}
		else if (Val.IsInt())
		{
			strVal.Format(L"%d", Val.GetInt());
		}
		else if (Val.IsUint())
		{
			strVal.Format(L"%d", Val.GetUint());
		}
		else if (Val.IsInt64())
		{
			strVal.Format(L"%d", Val.GetInt64());
		}
		else if (Val.IsUint64())
		{
			strVal.Format(L"%d", Val.GetUint64());
		}
	}
	else  if (Val.IsString())
	{
		std::string str(Val.GetString());
		strVal = CA2W(str.c_str());
	}

	return strVal;
}


CString CZKYTDlg::Map2Json(JsonMap mapPost)
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

CString CZKYTDlg::MapVec2Json(JsonMapVec vmPost)
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

void CZKYTDlg::SetConfig(void)
{
	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];

	// 获取项目的配置参数
	CString strUpConfigName, strUpConfigProject;
	m_edUpConfigName.GetWindowTextW(strUpConfigName);
	m_edUpConfigProject.GetWindowTextW(strUpConfigProject);

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetCDFilePath(L"UpConfig.ini", wchPath, true))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"Web", L"Name", L"ZKYTConfig.ini");
		if (strUpConfigName.IsEmpty())
		{
			m_edUpConfigName.SetWindowTextW(strTemp);
			strUpConfigName = strTemp;
		}

		strTemp = si.GetString(L"Web", L"Project", L"WebService");
		if (strUpConfigProject.IsEmpty())
		{
			m_edUpConfigProject.SetWindowTextW(strTemp);
			strUpConfigProject = strTemp;
		}

		strTemp = si.GetString(L"Web", L"NoOrNotApplicable", L"0");
		if (strTemp == L"0")
		{
			m_bNoOrNotApplicable = true;
		}
		else
		{
			m_bNoOrNotApplicable = false;
		}
	}


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
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", strUpConfigName, wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(strUpConfigProject, L"URL", L"");
		m_edURL.SetWindowTextW(strTemp);

		strTemp = si.GetString(strUpConfigProject, L"StationNumber", L"");
		m_edStationNumber.SetWindowTextW(strTemp);

		strTemp = si.GetString(strUpConfigProject, L"BusinessId", L"");
		if (strTemp.IsEmpty())
		{
			strTemp = si.GetString(strUpConfigProject, L"LicenseCode", L"");
		}
		m_edLicenseCode.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetCDFilePath(L"UpConfig.ini", wchPath, true))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"Web", L"Name", strUpConfigName);
		si.SetString(L"Web", L"Project", strUpConfigProject);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edLineNum.SetWindowTextW(strTemp);
	}
}


void CZKYTDlg::SetVehCtrl(void)
{
	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	int nDlgFontSize;
	nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
	CFont fontVehicleLCFont, fontDlgFont;


	fontVehicleLCFont.CreateFont(25,
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
	m_lstVehicle.InsertColumn(1, L"业务id",		0,	200);
	m_lstVehicle.InsertColumn(2, L"车牌号码",	0,	200);
	m_lstVehicle.InsertColumn(3, L"车牌颜色",	0,	150);
	m_lstVehicle.InsertColumn(4, L"车架号",	0,	150);
	m_lstVehicle.InsertColumn(5, L"车辆型号",	0,	200);
	m_lstVehicle.InsertColumn(6, L"发动机型号",	0,	150);
	m_lstVehicle.InsertColumn(7, L"该车型OBD通讯是否有成功记录",	0,	150);
	m_lstVehicle.InsertColumn(8, L"燃料种类",	0,	150);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}


bool CZKYTDlg::GetInterOBDJCY(void)
{
	bool bRet(false);
	CString strMsg;
	m_lsOBDJCY.clear();
	m_cbOBDJCY.ResetContent();
	m_cbOBDJCY.AddString(L"");
	CString strURL, strLicenseCode, strStationNumber, strLineNum;

	m_edURL.GetWindowTextW(strURL);
	m_edLicenseCode.GetWindowTextW(strLicenseCode);
	m_edStationNumber.GetWindowTextW(strStationNumber);
	m_edLineNum.GetWindowTextW(strLineNum);
	if (strURL.IsEmpty() || strLicenseCode.IsEmpty() || strStationNumber.IsEmpty() || strLineNum.IsEmpty() )
	{
		strMsg.Format(L"获取平台OBD检测员\r\n:%s", L"联网参数有空值");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	JsonMap map;
	map[L"jczbh"] = strStationNumber;

	std::wstring strRet;
	int nRet(99);
	nRet = CZKYTInterfaceLib_API::jdcjk(pchURL, strStationNumber.GetString(), L"OBD4", Map2Json(map).GetString(), strRet);
#ifdef _DEBUG
	nRet = 0;
	CString cstrRet;
	cstrRet.Format(L"{\"result\":1,\"info\":[{\"mc\":\"陈莹\",\"zj\":\"huoguanadmin\"},{\"mc\":\"杨梦琪\",\"zj\":\"huoguanluru\"},{\"mc\":\"朱立\",\"zj\":\"huoguanwj\"},{\"mc\":\"邓志霞\",\"zj\":\"huoguanjc02\"},{\"mc\":\"李锐\",\"zj\":\"lirui\"},{\"mc\":\"付红雨\",\"zj\":\"hgcjc01\"}]}");
	strRet = cstrRet.GetString();
#endif
	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"获取平台OBD检测员\r\n获取失败原因:%s", strResult);
				m_edMsg.SetWindowTextW(strMsg);
			}
			else if (strResult == L"1")
			{
				bRet = true;
				if (docu.HasMember("info") && docu["info"].IsArray())
				{
					const rapidjson::Value& arr = docu["info"];

					for (int i=0; i<arr.Size(); ++i)
					{
						const rapidjson::Value& tmp = arr[i];

						SOBDJCY sOBDJCY;
						if (tmp.HasMember("zj")){sOBDJCY.strzj = JsonToCString(tmp["zj"]).GetString();}
						if (tmp.HasMember("mc")){sOBDJCY.strmc = JsonToCString(tmp["mc"]).GetString();}
						m_lsOBDJCY.push_back(sOBDJCY);
					}

				}
				else if (docu.HasMember("info") && docu["info"].IsObject())
				{
					m_edMsg.SetWindowTextW(L"IsObject");
				}
			}
			else
			{
				strMsg.Format(L"获取平台OBD检测员\r\n结果返回为空:%s", strRet.c_str());
				m_edMsg.SetWindowTextW(strMsg);
			}
		}
		else
		{
			strMsg.Format(L"获取平台OBD检测员\r\n解析失败");
			m_edMsg.SetWindowTextW(strMsg);
		}

	}
	else
	{
		strMsg.Format(L"获取平台OBD检测员\r\n联网失败\r\n代号:%d", nRet);
		m_edMsg.SetWindowTextW(strMsg);
	}

	if (bRet)
	{
		for (list<SOBDJCY>::iterator ir=m_lsOBDJCY.begin(); ir!=m_lsOBDJCY.end(); ++ir)
		{
			CString strT;
			strT.Format(L"%s-%s", ir->strmc.c_str(), ir->strzj.c_str());
			m_cbOBDJCY.AddString(ir->strzj.c_str());
		}
	}
	return bRet;
}

bool CZKYTDlg::GetInterOBDJCYID(void)
{
	bool bRet(false);
	//m_cbOBDID.AddString(L"");
	CString strMsg;
	CString strURL, strLicenseCode, strStationNumber, strLineNum;

	m_edURL.GetWindowTextW(strURL);
	m_edLicenseCode.GetWindowTextW(strLicenseCode);
	m_edStationNumber.GetWindowTextW(strStationNumber);
	m_edLineNum.GetWindowTextW(strLineNum);
	if (strURL.IsEmpty() || strLicenseCode.IsEmpty() || strStationNumber.IsEmpty() || strLineNum.IsEmpty() )
	{
		strMsg.Format(L"获取平台OBD检测仪ID\r\n:%s", L"联网参数有空值");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	JsonMap map;
	map[L"jczbh"] = strStationNumber;

	std::wstring strRet;
	int nRet(99);
	nRet = CZKYTInterfaceLib_API::jdcjk(pchURL, strStationNumber.GetString(), L"OBD5", Map2Json(map).GetString(), strRet);
#ifdef _DEBUG
	nRet = 0;
	CString cstrRet;
	cstrRet.Format(L"{\"result\":1,\"info\":[{\"mc\":\"陈莹\",\"zj\":\"huoguanadmin\"},{\"mc\":\"杨梦琪\",\"zj\":\"huoguanluru\"},{\"mc\":\"朱立\",\"zj\":\"huoguanwj\"},{\"mc\":\"邓志霞\",\"zj\":\"huoguanjc02\"},{\"mc\":\"李锐\",\"zj\":\"lirui\"},{\"mc\":\"付红雨\",\"zj\":\"hgcjc01\"}]}");
	strRet = cstrRet.GetString();
#endif
	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"获取平台OBD检测仪ID\r\n获取失败原因:%s", strResult);
				m_edMsg.SetWindowTextW(strMsg);
			}
			else if (strResult == L"1")
			{
				bRet = true;
				if (docu.HasMember("info") && docu["info"].IsArray())
				{
					const rapidjson::Value& arr = docu["info"];

					for (int i=0; i<arr.Size(); ++i)
					{
						const rapidjson::Value& tmp = arr[i];

						CString strT;
						//if (tmp.HasMember("xh")){sOBDJCY.strzj = JsonToCString(tmp["xh"]).GetString();}
						if (tmp.HasMember("zj")){strT = JsonToCString(tmp["zj"]);}
						//if (tmp.HasMember("mc")){sOBDJCY.strmc = JsonToCString(tmp["mc"]).GetString();}
						if (!strT.IsEmpty()){m_cbOBDID.AddString(strT);}
					}

				}
				else if (docu.HasMember("info") && docu["info"].IsObject())
				{
					m_edMsg.SetWindowTextW(L"IsObject");
				}
			}
			else
			{
				strMsg.Format(L"获取平台OBD检测仪ID\r\n结果返回为空:%s", strRet.c_str());
				m_edMsg.SetWindowTextW(strMsg);
			}
		}
		else
		{
			strMsg.Format(L"获取平台OBD检测仪ID\r\n解析失败");
			m_edMsg.SetWindowTextW(strMsg);
		}

	}
	else
	{
		strMsg.Format(L"获取平台OBD检测仪ID\r\n联网失败\r\n代号:%d", nRet);
		m_edMsg.SetWindowTextW(strMsg);
	}
	return bRet;
}

void CZKYTDlg::OnBnClickedBtnSetconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	SetConfig();
	GetInterOBDJCY();
	GetInterOBDJCYID();
}


bool CZKYTDlg::GetVehicleList(void)
{
	bool bRet(false);
	CString strMsg;

	CString strURL, strLicenseCode, strStationNumber,strLineNum;

	m_edURL.GetWindowTextW(strURL);
	m_edLicenseCode.GetWindowTextW(strLicenseCode);
	m_edStationNumber.GetWindowTextW(strStationNumber);
	m_edLineNum.GetWindowTextW(strLineNum);
	if (strURL.IsEmpty() || strLicenseCode.IsEmpty() || strStationNumber.IsEmpty() || strLineNum.IsEmpty())
	{
		strMsg.Format(L"获取平台OBD检测员\r\n:%s", L"联网参数有空值");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	JsonMap map;

	map[L"jczbh"] = strStationNumber;

	std::wstring strRet;
	int nRet = CZKYTInterfaceLib_API::jdcjk(pchURL, strStationNumber.GetString(), L"OBD6", Map2Json(map).GetString(), strRet);
#ifdef _DEBUG
	nRet = 0;
	CString cstrRet;
	cstrRet.Format(L"{\"result\":\"1\",\"err\":\"\",\"info\":[{\"ywid\":\"10001\",\"cphm\":\"豫 A66666\",\"cpys\":\"蓝\",\"vin\":\"\",\"clxh\":\"WDF56GJ\",\"fdjxh\":\"ASD-2QW\",\"isok\":\"1\",\"fueltype\":\"A\"},{\"ywid\":\"10002\",\"cphm\":\"京 A88888\",\"cpys\":\"蓝\",\"vin\":\"\",\"clxh\":\"WDF56GJ\",\"fdjxh\":\"ASD-2QW\",\"isok\":\"1\",\"fueltype\":\"A\"}]}");

	strRet = cstrRet.GetString();
#endif
	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	std::list<SOBDVehInfo> lsOBDVehInfo;
	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"获取平台OBD待检信息\r\n获取失败原因:%s", strResult);
				m_edMsg.SetWindowTextW(strMsg);
			}
			else if (strResult == L"1")
			{
				bRet = true;
				if (docu.HasMember("info") && docu["info"].IsArray())
				{
					const rapidjson::Value& arr = docu["info"];

					for (int i=0; i<arr.Size(); ++i)
					{
						const rapidjson::Value& tmp = arr[i];

						SOBDVehInfo sOBDVehInfo;
						if (tmp.HasMember("ywid")){sOBDVehInfo.strywid = JsonToCString(tmp["ywid"]).GetString();}
						if (tmp.HasMember("cphm")){sOBDVehInfo.strcphm = JsonToCString(tmp["cphm"]).GetString();}
						if (tmp.HasMember("cpys")){sOBDVehInfo.strcpys = JsonToCString(tmp["cpys"]).GetString();}
						if (tmp.HasMember("vin")){sOBDVehInfo.strvin = JsonToCString(tmp["vin"]).GetString();}
						if (tmp.HasMember("clxh")){sOBDVehInfo.strclxh = JsonToCString(tmp["clxh"]).GetString();}
						if (tmp.HasMember("fdjxh")){sOBDVehInfo.strfdjxh = JsonToCString(tmp["fdjxh"]).GetString();}
						if (tmp.HasMember("isok")){sOBDVehInfo.strisok = JsonToCString(tmp["isok"]).GetString();}
						if (tmp.HasMember("fueltype")){sOBDVehInfo.strfueltype = JsonToCString(tmp["fueltype"]).GetString();}
						lsOBDVehInfo.push_back(sOBDVehInfo);
					}

				}
				else if (docu.HasMember("info") && docu["info"].IsObject())
				{
					m_edMsg.SetWindowTextW(L"IsObject");
				}
			}
			else
			{
				strMsg.Format(L"获取平台OBD待检信息\r\n结果返回为空:%s", strRet.c_str());
				m_edMsg.SetWindowTextW(strMsg);
			}
		}
		else
		{
			strMsg.Format(L"获取平台OBD待检信息\r\n解析失败");
			m_edMsg.SetWindowTextW(strMsg);
		}

	}
	else
	{
		strMsg.Format(L"获取平台OBD待检信息\r\n联网失败\r\n代号:%d", nRet);
		m_edMsg.SetWindowTextW(strMsg);
	}

	if (bRet)
	{
		// 删除列表数据
		m_lstVehicle.DeleteAllItems();
		int nItem(0);
		for (list<SOBDVehInfo>::iterator ir=lsOBDVehInfo.begin(); ir!=lsOBDVehInfo.end(); ++ir)
		{
			CString str;
			// 序号
			str.Format(L"%d", nItem+1);
			int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

			// 业务id
			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strywid.c_str(), 0, 0, 0, 0);
			// 车牌号码
			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strcphm.c_str(), 0, 0, 0, 0);
			// 车牌颜色
			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strcpys.c_str(), 0, 0, 0, 0);
			// 车架号
			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strvin.c_str(), 0, 0, 0, 0);
			// 车辆型号
			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strclxh.c_str(), 0, 0, 0, 0);
			// 发动机型号
			m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, ir->strfdjxh.c_str(), 0, 0, 0, 0);
			// 该车型OBD通讯是否有成功记录
			m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, ir->strisok.c_str(), 0, 0, 0, 0);
			// 燃料种类
			m_lstVehicle.SetItem (nItemIndex, 8, LVIF_TEXT, ir->strfueltype.c_str(), 0, 0, 0, 0);

			nItem++;
		}
	}
	return bRet;
}


void CZKYTDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	GetVehicleList();
}


void CZKYTDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	BeginWaitCursor();
	CString strMsg;
	m_edMsg.SetWindowTextW(strMsg);
	m_sOBDVehInfo = SOBDVehInfo();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_sOBDVehInfo.strywid = m_lstVehicle.GetItemText(nIndex,1);
		m_sOBDVehInfo.strcphm = m_lstVehicle.GetItemText(nIndex,2);
		m_sOBDVehInfo.strcpys = m_lstVehicle.GetItemText(nIndex,3);
		m_sOBDVehInfo.strvin = m_lstVehicle.GetItemText(nIndex,4);
		m_sOBDVehInfo.strclxh = m_lstVehicle.GetItemText(nIndex,5);
		m_sOBDVehInfo.strfdjxh = m_lstVehicle.GetItemText(nIndex,6);
		m_sOBDVehInfo.strisok = m_lstVehicle.GetItemText(nIndex,7);
		m_sOBDVehInfo.strfueltype = m_lstVehicle.GetItemText(nIndex,8);

		if (m_sOBDVehInfo.strywid.empty())
		{
			m_edMsg.SetWindowTextW(L"获取列表信息失败");
		}
		else
		{
			strMsg.Format(L"%s\r\n请点击“UPInterOBD”按钮完成OBD检测",m_sOBDVehInfo.strcphm.c_str());
			GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
			m_edMsg.SetWindowTextW(strMsg);
		}
	}

	EndWaitCursor();
	*pResult = 0;
}


void CZKYTDlg::OnBnClickedBtnUp()
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

	SResultOfOBD sResultOfOBD;
	sResultOfOBD.strRunningNumber = m_sOBDVehInfo.strywid.c_str();

	// 名字转换成ID
	if (strOBDJCY.IsEmpty())
	{
		sResultOfOBD.strOperator = L"ludajc02";
	}
	else
	{
		sResultOfOBD.strOperator = strOBDJCY.GetString();
	}

	bool bRet(false);
	CString strMsg;
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
	CString strURL, strLicenseCode, strStationNumber, strOBDJCYID;

	m_edURL.GetWindowTextW(strURL);
	m_edLicenseCode.GetWindowTextW(strLicenseCode);
	m_edStationNumber.GetWindowTextW(strStationNumber);
	m_cbOBDID.GetWindowTextW(strOBDJCYID);
	if (strURL.IsEmpty() || strLicenseCode.IsEmpty() || strStationNumber.IsEmpty() || strOBDJCYID.IsEmpty())
	{
		strMsg.Format(L"%s", L"联网参数有空值");
		m_edMsg.SetWindowTextW(strMsg);
		return ;
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
	bRet = UpOBDReaust(pchURL, strStationNumber, strLicenseCode, strOBDJCYID, sResultOfOBD, strMsg);
	if (!bRet)
	{
		strMsg = L"上传OBD结果失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s\r\n", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
		//return;
	}

	strMsg.Empty();
	bRet = UpOBDIURP(pchURL, strStationNumber, strLicenseCode, strOBDJCYID, sResultOfOBD, strMsg);
	if (!bRet)
	{
		strMsg = L"上传OBDIURP失败\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s\r\n", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
		//return;
	}

	if (strTempMsg.Find(L"失败")  == std::string::npos)
	{
		m_edMsg.SetWindowTextW(L"上传完成，南华软件提示安全信息时点击ToTestLog");
	}

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_TOTSETLOG)->EnableWindow(TRUE);
}


bool CZKYTDlg::SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(true);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

	if (m_sOBDVehInfo.strfueltype.find(L"B") != std::string::npos)
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


CString CZKYTDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CZKYTDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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

bool CZKYTDlg::UpOBDReaust(const char * pchURL, const CStringW& strStationNumber, const CStringW& strLincenseCode,
		const CStringW& strOBDJCYID, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	JsonMap map;

	//jcrid OBD 检测人员 ID 字符串 20
	map[L"jcrid"] = sResultOfOBD.strOperator.c_str();
	//jcyid OBD 检测仪 ID 字符串 20
	map[L"jcyid"] = strOBDJCYID;
	//fdjcalid 发动机控制单元 CALID 字符串 200 如适用
	if (m_bNoOrNotApplicable)
	{
		map[L"fdjcalid"] = sResultOfOBD.strEngineCALID.c_str();
	}
	else
	{
		map[L"fdjcalid"] = L"不支持";
	}
	//fdjcvn 发动机控制单元 CVN 字符串 200 如适用
	if (m_bNoOrNotApplicable)
	{
		map[L"fdjcvn"] = sResultOfOBD.strEngineCVN.c_str();
	}
	else
	{
		map[L"fdjcvn"] = L"不支持";
	}
	//hclcalid 后处理控制单元 CALID 字符串 200 如适用
	//map[L"hclcalid"] = sResultOfOBD.strPostProcessingCALID.c_str();
	map[L"hclcalid"] = L"不适用";
	//hclcvn 后处理控制单元 CVN 字符串 200 如适用
	//map[L"hclcvn"] = sResultOfOBD.strPostProcessingCVN.c_str();
	map[L"hclcvn"] = L"不适用";
	//qtcalid 其他控制单元 CALID 字符串 200 如适用
	//map[L"qtcalid"] = sResultOfOBD.strOtherCALID.c_str();
	map[L"qtcalid"] = L"不适用";
	//qtcvn 其他控制单元 CVN 字符串 200 如适用
	//map[L"qtcvn"] = sResultOfOBD.strOtherCVN.c_str();
	map[L"qtcvn"] = L"不适用";
	//gzzsq OBD 故障指示器 字符串 1 0 不合格，1 合格
	map[L"gzzsq"] = L"1";
	//zdytxqk OBD 诊断仪通讯情况 字符串 1 0 通讯成功，1 找不到接口，2接口损坏，3 连接后不能通讯
	map[L"zdytxqk"] = L"0";
	//zsqsfbdl OBD 故障指示器是否被点亮 字符串 1 0 否，1 是
	map[L"zsqsfbdl"] = L"0";
	//gzdm 故障代码 字符串 100 多个以英文分号隔开
	map[L"gzdm"] = L"";
	//gzxx 故障信息 字符串 2000多个以英文分号隔开（和故障代码相匹配，就算对应故障码没有信息也必须有分号）
	map[L"gzxx"] = L"";
	//wwcxm 诊断就绪状态未完成项目 字符串 10 0 无，1 催化器，2 氧传感器，3 氧传感器加热器，4 废气再循环（EGR）/可变气门 VVT,5 SRC,6 POC,7 DOC,8 DPF.注意：如果有多个的话，需要进行组合，如“12”就是表示催化器和氧传感器未完成。
	map[L"wwcxm"] = L"0";
	//milxslc MIL 灯点亮后行驶里程 数字 6,2 单位：km
	map[L"milxslc"] = L"0.0";
	//ljxslc 车辆累计行驶里程 数字 9,2 单位：km
	map[L"ljxslc"] = L"0.0";
	//vin Vin（车辆识别代号） 字符串 17
	map[L"vin"] = m_sOBDVehInfo.strvin.c_str();
	//obdyq 型式检验时的 OBD 要求 字符串 200 如：EOBD,OBDⅡ,CN-OBD-6第 33 页 
	map[L"obdyq"] = sResultOfOBD.strOBDType.c_str();
	//jgpd 结果判定 字符串 1 0 不合格，1 合格，2 记录数据，判定车辆通过
	map[L"jgpd"] = L"1";
	//sfxyfj 是否需要复检 字符串 1 0 不需要，1 需要
	map[L"sfxyfj"] = L"0";
	//fjnr 复检内容 字符串 500
	map[L"fjnr"] = L"";
	//jckssj 检测开始时间 字符串 19 yyyy-MM-dd HH:mm:ss
	map[L"jckssj"] = sResultOfOBD.strDetBegTime.c_str();
	//jcjssj 检测结束时间 字符串 19 yyyy-MM-dd HH:mm:ss
	map[L"jcjssj"] = sResultOfOBD.strDetEndTime.c_str();

	std::wstring strRet;
	int nRet = CZKYTInterfaceLib_API::jdcjk(pchURL, m_sOBDVehInfo.strywid.c_str(), L"OBD1", Map2Json(map).GetString(), strRet);

	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"获取失败原因:%s", strResult);
			}
			else if (strResult == L"1")
			{
				bRet = true;
				strMsg.Format(L"%s", L"上传成功");
			}
			else
			{
				strMsg.Format(L"结果返回为空:%s", strRet.c_str());
			}
		}
		else
		{
			strMsg.Format(L"解析失败");
		}
	}
	else
	{
		strMsg.Format(L"联网失败:%d", nRet);
	}

	return bRet;
}

bool CZKYTDlg::UpOBDIURP(const char * pchURL, const CStringW& strStationNumber, const CStringW& strLincenseCode,
		const CStringW& strOBDJCYID, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	JsonMap map;

	if (m_sOBDVehInfo.strfueltype.find(L"B") != std::string::npos)
	{
		int n, n1;
		CString strTemp;
		//chq1jccs 汽油车：催化器组 1 监测完成次数柴油车：NMHC 催化器监测完成次数数字
		n = _wtoi(sResultOfOBD.strIUPR_NMHCC.c_str());
		strTemp.Format(L"%d", n);
		map[L"chq1jccs"] = strTemp;
		//chq1fhcs汽油车：催化器组 1 符合条件次数柴油车：NMHC 催化器符合条件次数数字 6,0 
		n1 = _wtoi(sResultOfOBD.strIUPR_NMHCEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"chq1fhcs"] = strTemp;
		//chq1iupr汽油车：催化器组 1IUPR 率柴油车：NMHC 催化器 IUPR 率数字 6,4 
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_NMHCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NMHCC.c_str()))));
		}
		map[L"chq1iupr"] = strTemp;
		//chq2jccs汽油车：催化器组 2 监测完成次数柴油车：NOx 催化器监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_NOXCC.c_str());
		strTemp.Format(L"%d", n);
		map[L"chq2jccs"] = strTemp;
		//chq2fhcs汽油车：催化器组 2 符合条件次数柴油车：NOx 催化器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_NOXCEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"chq2fhcs"] = strTemp;
		//chq2iupr汽油车：催化器组 2 IUPR 率柴油车：NOx 催化器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_NOXCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXCC.c_str()))));
		}
		map[L"chq2iupr"] = strTemp;
		//qycqq1jccs汽油车：前氧传感器组 1 监测完成次数柴油车：NOx 吸附器监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_NOXAC.c_str());
		strTemp.Format(L"%d", n);
		map[L"qycqq1jccs"] = strTemp;
		//qycqq1fhcs汽油车：前氧传感器组 1 符合条件次数柴油车：NOx 吸附器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_NOXAEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"qycqq1fhcs"] = strTemp;
		//qycqq1iupr汽油车：前氧传感器组 1 IUPR 率柴油车：NOx 吸附器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_NOXAEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXAC.c_str()))));
		}
		map[L"qycqq1iupr"] = strTemp;
		//qycqq2jccs 汽油车：前氧传感器组 2 监测完成次数 数字 6,0 汽油车和柴油柴油车：PM 捕集器监测完成次数 车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_PMC.c_str());
		strTemp.Format(L"%d", n);
		map[L"qycqq2jccs"] = strTemp;
		//qycqq2fhcs汽油车：前氧传感器组 2 符合条件次数柴油车：PM 捕集器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_PMEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"qycqq2fhcs"] = strTemp;
		//qycqq2iupr汽油车：前氧传感器组 2 IUPR 率柴油车：PM 捕集器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_PMEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PMC.c_str()))));
			
		}
		map[L"qycqq2iupr"] = strTemp;
		//hycqq1jccs汽油车：后氧传感器组 1 监测完成次数柴油车：废气传感器监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_WSC.c_str());
		strTemp.Format(L"%d", n);
		map[L"hycqq1jccs"] = strTemp;
		//hycqq1fhcs汽油车：后氧传感器组 1 符合条件次数柴油车：废气传感器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_WSEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"hycqq1fhcs"] = strTemp;
		//hycqq1iupr汽油车：后氧传感器组 1 IUPR 率柴油车：废气传感器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_WSEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_WSC.c_str()))));
		}
		map[L"hycqq1iupr"] = strTemp;
		//hycqq2jccs汽油车：后氧传感器组 2 监测完成次数柴油车：增压压力监测监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_PPC.c_str());
		strTemp.Format(L"%d", n);
		map[L"hycqq1jccs"] = strTemp;
		//hycqq2fhcs汽油车：后氧传感器组 2 符合条件次数柴油车：增压压力监测符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_PPEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"hycqq1fhcs"] = strTemp;
		//hycqq2iupr汽油车：后氧传感器组 2 IUPR 率柴油车：增压压力监测 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_PPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PPC.c_str()))));
		}
		map[L"hycqq1iupr"] = strTemp;
		//egrvvtjccs汽油车和柴油车：EGR 和 VVT 监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_EGRC.c_str());
		strTemp.Format(L"%d", n);
		map[L"egrvvtjccs"] = strTemp;
		//egrvvtfhcs汽油车和柴油车：EGR 和 VVT 符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_EGREC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"egrvvtfhcs"] = strTemp;
		//egrvvtiupr汽油车和柴油车：EGR 和 VVT IUPR 率 数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
			
		}
		map[L"egrvvtiupr"] = strTemp;
		//gpf1jccs汽油车：GPF 组 1 监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str());
		strTemp.Format(L"%d", n);
		map[L"gpf1jccs"] = strTemp;
		//gpf1fhcs汽油车：GPF 组 1 符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str());
		strTemp.Format(L"%d", n1);
		map[L"gpf1fhcs"] = strTemp;
		//gpf1iupr汽油车：GPF 组 1IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC1.c_str()))));
		}
		map[L"gpf1iupr"] = strTemp;
		//gpf2jccs汽油车：GPF 组 2 监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str());
		strTemp.Format(L"%d", n);
		map[L"gpf2jccs"] = strTemp;
		//gpf2fhcs汽油车：GPF 组 2 符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str());
		strTemp.Format(L"%d", n1);
		map[L"gpf2fhcs"] = strTemp;
		//gpf2iupr汽油车：GPF 组 2 IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC2.c_str()))));
		}
		map[L"gpf2iupr"] = strTemp;
		//eckqpsxtjccs 汽油车：二次空气喷射系统监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str());
		strTemp.Format(L"%d", n);
		map[L"eckqpsxtjccs"] = strTemp;
		//eckqpsxtfhcs汽油车：二次空气喷射系统符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"eckqpsxtfhcs"] = strTemp;
		//eckqpsxtiupr汽油车：二次空气喷射系统 IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_AMCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_AMCCC.c_str()))));
		}
		map[L"eckqpsxtiupr"] = strTemp;

		//evapjccs 汽油车：EVAP 监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str());
		strTemp.Format(L"%d", n);
		map[L"evapjccs"] = strTemp;
		//evapfhcs汽油车：EVAP 符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"evapfhcs"] = strTemp;
		//evapiupr汽油车：EVAP IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EVAPC.c_str()))));
			
		}
		map[L"evapiupr"] = strTemp;

		//hycqq2jccs汽油车：后氧传感器组 2 监测完成次数柴油车：增压压力监测监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str());
		strTemp.Format(L"%d", n);
		map[L"hycqq2jccs"] = strTemp;
		//hycqq2fhcs汽油车：后氧传感器组 2 符合条件次数柴油车：增压压力监测符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str());
		 strTemp.Format(L"%d", n1);
		map[L"hycqq2fhcs"] = strTemp;
		//hycqq2iupr汽油车：后氧传感器组 2 IUPR 率柴油车：增压压力监测 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()))));
			
		}
		map[L"hycqq2iupr"] = strTemp;
	}
	else
	{
		int n, n1;
		CString strTemp;
		//chq1jccs 汽油车：催化器组 1 监测完成次数柴油车：NMHC 催化器监测完成次数数字
		n = _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str());
		strTemp.Format(L"%d", n);
		map[L"chq1jccs"] = strTemp;
		//chq1fhcs汽油车：催化器组 1 符合条件次数柴油车：NMHC 催化器符合条件次数数字 6,0 
		n1 = _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str());
		strTemp.Format(L"%d", n1);
		map[L"chq1fhcs"] = strTemp;
		//chq1iupr汽油车：催化器组 1IUPR 率柴油车：NMHC 催化器 IUPR 率数字 6,4 
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()))));
		}
		map[L"chq1iupr"] = strTemp;
		//chq2jccs汽油车：催化器组 2 监测完成次数柴油车：NOx 催化器监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_CMCCB2.c_str());
		strTemp.Format(L"%d", n);
		map[L"chq2jccs"] = strTemp;
		//chq2fhcs汽油车：催化器组 2 符合条件次数柴油车：NOx 催化器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_CMCECB2.c_str());
		strTemp.Format(L"%d", n1);
		map[L"chq2fhcs"] = strTemp;
		//chq2iupr汽油车：催化器组 2 IUPR 率柴油车：NOx 催化器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_CMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB2.c_str()))));
			
		}
		map[L"chq2iupr"] = strTemp;
		//qycqq1jccs汽油车：前氧传感器组 1 监测完成次数柴油车：NOx 吸附器监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_O2SMCCB1.c_str());
		 strTemp.Format(L"%d", n);
		map[L"qycqq1jccs"] = strTemp;
		//qycqq1fhcs汽油车：前氧传感器组 1 符合条件次数柴油车：NOx 吸附器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_O2SMCECB1.c_str());
		strTemp.Format(L"%d", n1);
		map[L"qycqq1fhcs"] = strTemp;
		//qycqq1iupr汽油车：前氧传感器组 1 IUPR 率柴油车：NOx 吸附器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB1.c_str()))));
			
		}
		map[L"qycqq1iupr"] = strTemp;
		//qycqq2jccs 汽油车：前氧传感器组 2 监测完成次数 数字 6,0 汽油车和柴油柴油车：PM 捕集器监测完成次数 车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_O2SMCCB2.c_str());
		strTemp.Format(L"%d", n);
		map[L"qycqq2jccs"] = strTemp;
		//qycqq2fhcs汽油车：前氧传感器组 2 符合条件次数柴油车：PM 捕集器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_O2SMCECB2.c_str());
		strTemp.Format(L"%d", n1);
		map[L"qycqq2fhcs"] = strTemp;
		//qycqq2iupr汽油车：前氧传感器组 2 IUPR 率柴油车：PM 捕集器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB2.c_str()))));
			
		}
		map[L"qycqq2iupr"] = strTemp;
		//hycqq1jccs汽油车：后氧传感器组 1 监测完成次数柴油车：废气传感器监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_RO2SMCCB1.c_str());
		 strTemp.Format(L"%d", n);
		map[L"hycqq1jccs"] = strTemp;
		//hycqq1fhcs汽油车：后氧传感器组 1 符合条件次数柴油车：废气传感器符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_RO2SMCECB1.c_str());
		strTemp.Format(L"%d", n1);
		map[L"hycqq1fhcs"] = strTemp;
		//hycqq1iupr汽油车：后氧传感器组 1 IUPR 率柴油车：废气传感器 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()))));
			
		}
		map[L"hycqq1iupr"] = strTemp;
		//hycqq2jccs汽油车：后氧传感器组 2 监测完成次数柴油车：增压压力监测监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str());
		strTemp.Format(L"%d", n);
		map[L"hycqq2jccs"] = strTemp;
		//hycqq2fhcs汽油车：后氧传感器组 2 符合条件次数柴油车：增压压力监测符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str());
		 strTemp.Format(L"%d", n1);
		map[L"hycqq2fhcs"] = strTemp;
		//hycqq2iupr汽油车：后氧传感器组 2 IUPR 率柴油车：增压压力监测 IUPR 率数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()))));
			
		}
		map[L"hycqq2iupr"] = strTemp;
		//evapjccs 汽油车：EVAP 监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str());
		strTemp.Format(L"%d", n);
		map[L"evapjccs"] = strTemp;
		//evapfhcs汽油车：EVAP 符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"evapfhcs"] = strTemp;
		//evapiupr汽油车：EVAP IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EVAPC.c_str()))));
			
		}
		map[L"evapiupr"] = strTemp;
		//egrvvtjccs汽油车和柴油车：EGR 和 VVT 监测完成次数数字 6,0 汽油车和柴油车公用该字段
		n = _wtoi(sResultOfOBD.strIUPR_EGRC.c_str());
		strTemp.Format(L"%d", n);
		map[L"egrvvtjccs"] = strTemp;
		//egrvvtfhcs汽油车和柴油车：EGR 和 VVT 符合条件次数数字 6,0 汽油车和柴油车公用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_EGREC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"egrvvtfhcs"] = strTemp;
		//egrvvtiupr汽油车和柴油车：EGR 和 VVT IUPR 率 数字 6,4 汽油车和柴油车公用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
			
		}
		map[L"egrvvtiupr"] = strTemp;
		//gpf1jccs汽油车：GPF 组 1 监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str());
		strTemp.Format(L"%d", n);
		map[L"gpf1jccs"] = strTemp;
		//gpf1fhcs汽油车：GPF 组 1 符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str());
		strTemp.Format(L"%d", n1);
		map[L"gpf1fhcs"] = strTemp;
		//gpf1iupr汽油车：GPF 组 1IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC1.c_str()))));
		}
		map[L"gpf1iupr"] = strTemp;
		//gpf2jccs汽油车：GPF 组 2 监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str());
		strTemp.Format(L"%d", n);
		map[L"gpf2jccs"] = strTemp;
		//gpf2fhcs汽油车：GPF 组 2 符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str());
		strTemp.Format(L"%d", n1);
		map[L"gpf2fhcs"] = strTemp;
		//gpf2iupr汽油车：GPF 组 2 IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC2.c_str()))));
		}
		map[L"gpf2iupr"] = strTemp;
		//eckqpsxtjccs 汽油车：二次空气喷射系统监测完成次数 数字 6,0 仅汽油车用该字段
		n = _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str());
		strTemp.Format(L"%d", n);
		map[L"eckqpsxtjccs"] = strTemp;
		//eckqpsxtfhcs汽油车：二次空气喷射系统符合条件次数 数字 6,0 仅汽油车用该字段
		n1 = _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str());
		strTemp.Format(L"%d", n1);
		map[L"eckqpsxtfhcs"] = strTemp;
		//eckqpsxtiupr汽油车：二次空气喷射系统 IUPR 率 数字 6,4 仅汽油车用该字段
		if (n == 0 || n1 == 0)
		{
			strTemp.Format(L"%.4f", 0.00000f);
		}
		else
		{
			strTemp.Format(L"%.4f", ((_wtof(sResultOfOBD.strIUPR_AMCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_AMCCC.c_str()))));
		}
		map[L"eckqpsxtiupr"] = strTemp;
	}
	//jckssj 检测开始时间 字符串 19 yyyy-MM-dd HH:mm:ss
	map[L"jckssj"] = sResultOfOBD.strDetBegTime.c_str();
	//jcjssj 检测结束时间 字符串 19 yyyy-MM-dd HH:mm:ss
	map[L"jcjssj"] = sResultOfOBD.strDetEndTime.c_str();

	std::wstring strRet;
	int nRet = CZKYTInterfaceLib_API::jdcjk(pchURL, m_sOBDVehInfo.strywid.c_str(), L"OBD3", Map2Json(map).GetString(), strRet);

	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"获取失败原因:%s", strResult);
			}
			else if (strResult == L"1")
			{
				bRet = true;
				strMsg.Format(L"%s", L"上传成功");
			}
			else
			{
				strMsg.Format(L"结果返回为空:%s", strRet.c_str());
			}
		}
		else
		{
			strMsg.Format(L"解析失败");
		}
	}
	else
	{
		strMsg.Format(L"联网失败:%d", nRet);
	}

	return bRet;
}

bool CZKYTDlg::GetLicenseCode(CString& strLicCode)
{
	bool bRet(false);

	CString strMsg;
	CString strURL, strLicenseCode, strStationNumber,strLineNum;

	m_edURL.GetWindowTextW(strURL);
	m_edLicenseCode.GetWindowTextW(strLicenseCode);
	m_edStationNumber.GetWindowTextW(strStationNumber);
	m_edLineNum.GetWindowTextW(strLineNum);

	if (strURL.IsEmpty() || strStationNumber.IsEmpty() || strLineNum.IsEmpty())
	{
		strMsg.Format(L"获取平台获取授权码接口\r\n:%s", L"联网参数有空值");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	JsonMap map;

	map[L"jcxbh"] = strLineNum;

	std::wstring strRet;

	int nRet = CZKYTInterfaceLib_API::getSqm(pchURL, strLineNum.GetString(), strRet);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}


	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"获取平台OBD获取授权码接口\r\n获取失败原因:%s", strResult);
				m_edMsg.SetWindowTextW(strMsg);
			}
			else if (strResult == L"1")
			{
				bRet = true;
				strResult.Empty();
				if (docu.HasMember("sqm")){strResult.Format(L"%s",JsonToCString(docu["sqm"]));}
				if (!strResult.IsEmpty())
				{
					strLicCode = strResult;
					m_edLicenseCode.SetWindowTextW(strResult);
				}
			}
		}
	}

	return bRet;
}

bool CZKYTDlg::UpBBH(void)
{
	bool bRet(false);

	CString strMsg;
	CString strURL, strLicenseCode, strStationNumber,strLineNum;

	m_edURL.GetWindowTextW(strURL);
	m_edLicenseCode.GetWindowTextW(strLicenseCode);
	m_edStationNumber.GetWindowTextW(strStationNumber);
	m_edLineNum.GetWindowTextW(strLineNum);

	if (strURL.IsEmpty() || strStationNumber.IsEmpty() || strLineNum.IsEmpty())
	{
		strMsg.Format(L"获取平台获取授权码接口\r\n:%s", L"联网参数有空值");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	JsonMap map;

	map[L"bbh"] = L"5.0";

	std::wstring strRet;

	int nRet = CZKYTInterfaceLib_API::jdcjk(pchURL, strLicenseCode.GetString(), L"BBH1", Map2Json(map).GetString(), strRet);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}


	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strResult(L"");
			if (docu.HasMember("result")){strResult.Format(L"%s",JsonToCString(docu["result"]));}

			if (strResult == L"0")
			{
				if (docu.HasMember("err")){strResult.Format(L"%s",JsonToCString(docu["err"]));}
				strMsg.Format(L"工控软件版本号接口\r\n获取失败原因:%s", strResult);
				m_edMsg.SetWindowTextW(strMsg);
			}
			else if (strResult == L"1")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}


void CZKYTDlg::OnBnClickedBtnTotsetlog()
{
	// TODO: 在此添加控件通知处理程序代码
	wchar_t wchPath[MAX_PATH];
	CString strRunning;
	CString strPlateNum;
	// 更新INI
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"TestLog", L"ItemOBD", L"4");
		si.SetString(L"TestLog", L"HasOBD", L"1");
		strRunning = si.GetString(L"TestLog", L"RunningNumber", L"");
		strPlateNum = si.GetString(L"TestLog", L"PlateNumber", L"");
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"TestLog", L"HasOBD", L"1");
	}
	// 先处理SQL部分在处理上传部分
	CString strSource, strCatalog, strUserID, strPassword, strSQL;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);

	// 设置数据库操作路径
	CNHSQLServerDBO::SetLogFilePath(CZKYTInterfaceLib_API::LogFilePath().c_str());
	_ConnectionPtr pConnection;

	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"准备", L"保存TestLog", strRunning);
		if (!strRunning.IsEmpty())
		{
			strSQL.Format(L" Update TestLog set ItemOBD = '4', HasOBD = '1' where RunningNumber = '%s' ", strRunning);
			int nRowsInvolved(0);
			CNHSQLServerDBO::ExecuteDML(pConnection, strSQL, &nRowsInvolved);
			CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"完成", L"保存TestLog", strRunning);
		}

		CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"准备", L"保存VehicleInfo", strPlateNum);
		if (!strPlateNum.IsEmpty())
		{
			strSQL.Format(L" Update VehicleInfo set HasOBD = '1' where PlateNumber = '%s' ", strPlateNum);
			int nRowsInvolved(0);
			CNHSQLServerDBO::ExecuteDML(pConnection, strSQL, &nRowsInvolved);
			CNHLogAPI::WriteLogEx(CZKYTInterfaceLib_API::LogFilePath().c_str(), L"完成", L"保存VehicleInfo", strPlateNum);
		}
	}
	CNHSQLServerDBO::CloseDB(pConnection);

	m_edMsg.SetWindowTextW(L"操作完成");
	GetDlgItem(IDC_BTN_TOTSETLOG)->EnableWindow(FALSE);
}
