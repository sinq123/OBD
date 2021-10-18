
// YJSocketDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YJSocket.h"
#include "YJSocketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYJSocketDlg 对话框




CYJSocketDlg::CYJSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYJSocketDlg::IDD, pParent)
	, m_bAuthentication(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CYJSocketDlg::~CYJSocketDlg(void)
{
	CloseConnect();
}

void CYJSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_SERVERIP, m_edServerIP);
	DDX_Control(pDX, ID_ED_SERVERPORT, m_edServerPost);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNumber);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJCY);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbMessage);
	DDX_Control(pDX, IDC_BTN_SOCKET, m_btnSocket);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassword);
}

BEGIN_MESSAGE_MAP(CYJSocketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CYJSocketDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CYJSocketDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CYJSocketDlg::OnBnClickedBtnUpinter)
	ON_BN_CLICKED(IDC_BTN_UPTESTLOG, &CYJSocketDlg::OnBnClickedBtnUptestlog)
	ON_BN_CLICKED(IDC_BTN_SOCKET, &CYJSocketDlg::OnBnClickedBtnSocket)
END_MESSAGE_MAP()


// CYJSocketDlg 消息处理程序

BOOL CYJSocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_bAuthentication = false;

	m_mapTestType[L"A"] = L"双怠速";
	m_mapTestType[L"B"] = L"稳态";
	m_mapTestType[L"C"] = L"简易瞬态";
	m_mapTestType[L"D"] = L"瞬态";
	m_mapTestType[L"F"] = L"自由加速";
	m_mapTestType[L"G"] = L"加载减速";

	// 初始化
	SHJ03 sHJ03;
	m_sHJ03 = sHJ03;

	SetConfig();
	SetVehCtrl();

	//
	// GetVehicleList();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYJSocketDlg::OnPaint()
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
HCURSOR CYJSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYJSocketDlg::CloseConnect(void)
{
	if (m_yjSocket.IsOpen())
	{
		m_yjSocket.Close();
	}
	m_bAuthentication = false;
}

CString CYJSocketDlg::CheckSum(const char *ch, const UINT un)
{
	//assert(NULL!=pb && 0UL!=un);
	assert(NULL!=ch && 0UL!=un);
	CString strRet(L"");

	DWORD bSum(0x00);	
	for (UINT ui=0; ui<un; ui++)
	{
		bSum ^=  ch[ui];
	}
	strRet.Format(L"%02x", bSum&0xff);
	return strRet;
}

CString CYJSocketDlg::GenerateInsNum(void)
{
	// 唯一编码生成规则：检测线编码+时间（精确到毫秒）
	CString InsNum(L""), strTemp;;
	m_edStationNumber.GetWindowTextW(strTemp);
	if (strTemp == L"")
	{
		strTemp = L"01";
	}
	InsNum.AppendFormat(L"%s", strTemp);
	SYSTEMTIME dt;
	GetLocalTime(&dt);
	InsNum.AppendFormat(L"%04d%02d%02d%02d%02d%02d%03d", dt.wYear, dt.wMonth, dt.wDay, dt.wHour, dt.wMinute, dt.wSecond, dt.wMilliseconds);
	return InsNum;
}

bool CYJSocketDlg::SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv)
{	
	CString strSend(L"");
	// 1数据类型+2检测站编码+3检测线编码+4时间+4唯一编码+5固定分隔符+6数据部分+7固定分隔符+8校验码+9固定结束符
	strSend.Format(L"%s%s%s", strDataType, strStationNumber, strLineNumber);
	strSend.AppendFormat(L"%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	// 唯一编码
	strSend.AppendFormat(L"%s", strInsNumber);
	// 固定分隔符
	strSend.AppendFormat(L"@@@");
	strSend.AppendFormat(L"%s", strJsonData);// 数据部分
	strSend.AppendFormat(L"tek");// 固定分隔符

	std::string sData(CW2A(strSend.GetString()));
	CString strSum = CheckSum((char *)sData.c_str(), sData.size());
	strSend.AppendFormat(L"%s", strSum); // 校验码

	strSend.AppendFormat(L"####");// 固定结束符

	if (!m_yjSocket.IsOpen())
	{
		CString strIP, strPort;
		int nPort;
		m_edServerIP.GetWindowTextW(strIP);
		m_edServerPost.GetWindowTextW(strPort);
		nPort = _wtoi(strPort);
		if (!m_yjSocket.Open(strIP, nPort) ||(strIP == L"" || strPort == L""))
		{
			strRecv = L"连接服务器失败！";
			return false;
		}
	}


	return m_yjSocket.SendAndRecvPacket(strSend, strRecv);
}


void CYJSocketDlg::SetConfig(void)
{
	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"YJConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"System", L"ServerIP", L"");
		m_edServerIP.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"ServerPort", L"");
		m_edServerPost.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"StationNumber", L"");
		m_edStationNumber.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edLineNumber.SetWindowTextW(strTemp);
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
		m_edPassword.SetWindowTextW(strTemp);
	}

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UPTESTLOG)->EnableWindow(FALSE);

	CFont font;
	font.CreatePointFont(320,L"宋体");
	m_lbMessage.SetFont(&font);

	m_lbMessage.SetWindowTextW(L"获取配置文件成功");
}

void CYJSocketDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(1, L"流水号",		0,	220/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(2, L"车牌号码",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(3, L"车牌颜色",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(4, L"检测次数",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(5, L"检测方法",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(6, L"VIN",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(7, L"是否有OBD",	0,	120/**nSM_CXSCREEN/1280*/);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);

	m_lbMessage.SetWindowTextW(L"列表设置成功");
}


bool CYJSocketDlg::Authentication(std::wstring &strResult, std::wstring &strMsg)
{
	CString strTemp;
	m_edLineNumber.GetWindowTextW(strTemp);
	if (strTemp == L"")
	{
		strTemp = L"01";
	}

	return SendAndRecvPacket(L"LJZC", L"", L"", L"", strTemp, strMsg);
}

bool CYJSocketDlg::GetVehicleList(void)
{
	if (!m_bAuthentication)
	{
		std::wstring strResult, strMsg;
		m_bAuthentication = Authentication(strResult, strMsg);
	}
	if (m_bAuthentication)
	{
		m_lbMessage.SetWindowTextW(L"连接成功");	
	}
	else
	{
		return false;
	}

	if (!GetTime())
	{
		return false;
	}

	// 禁止点击更新按钮
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);

	CString strStationNum, strLineNum;

	m_edStationNumber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);
	// Json

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	//strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", strLineNum);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"InspectionMethod\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", L"");
	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(15, 0, 0, 0);
	COleDateTime odtSartTime = odtCurTime - odts;
	strJsonData.AppendFormat(L"\"StartTime\":\"%s\",",odtSartTime.Format(L"%Y-%m-%d 00:00:00"));
	strJsonData.AppendFormat(L"\"EndTime\":\"%s\"", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ03", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

	// 调用联网接口后，可以点击更新
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);

	std::list<SHJ03> lsHJ03;

	if (bRet)
	{
		std::wstring strMsg;
		m_yjSocket.RecvPacket(strMsg);
		if (strMsg.length() == 21)
		{
			strMsg = strRecv;
		}
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			for (UINT i=0; i<root.size(); i++)
			{
				SHJ03 sHJ03;
				if (!root[i]["InspectionNum"].empty()){sHJ03.strInspectionNum = Json2Cstring(root[i]["InspectionNum"]).GetString();}
				if (!root[i]["VLPN"].empty())	{sHJ03.strVLPN = Json2Cstring(root[i]["VLPN"]).GetString();}
				if (!root[i]["VLPNColor"].empty()) {sHJ03.strVLPNColor = Json2Cstring(root[i]["VLPNColor"]).GetString();}
				if (!root[i]["VIN"].empty())	{sHJ03.strVIN = Json2Cstring(root[i]["VIN"]).GetString();}
				if (!root[i]["InspectionMethod"].empty()) {sHJ03.strInspectionMethod =  m_mapTestType[Json2Cstring(root[i]["InspectionMethod"]).GetString()];}
				if (!root[i]["InspectionNature"].empty()){sHJ03.strInspectionNature = Json2Cstring(root[i]["InspectionNature"]).GetString();}
				if (!root[i]["HasOBD"].empty()){sHJ03.strHasOBD = Json2Cstring(root[i]["HasOBD"]).GetString();}
				lsHJ03.push_back(sHJ03);
			}
		}
		else
		{
			m_lbMessage.SetWindowTextW(strMsg.c_str());
		}

	}
	else
	{
		CString str;
		str.Format(L"%s", strRecv.c_str());
		m_lbMessage.SetWindowTextW(str);


		return false;
	}

	// 显示列表
	m_lstVehicle.DeleteAllItems();
	int nItem(0);
	for (list<SHJ03>::iterator ir=lsHJ03.begin(); ir!=lsHJ03.end(); ++ir)
	{
		CString strHasOBD = ir->strHasOBD.c_str();
		if (strHasOBD != L"1")
		{
			continue;
		}

		CString str;
		// 序号
		str.Format(L"%d", nItem+1);

		int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

		// 流水号
		str = ir->strInspectionNum.c_str();
		m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);
		// 车牌号码
		str = ir->strVLPN.c_str();
		m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);
		// 车牌颜色
		str = ir->strVLPNColor.c_str();
		m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);
		// 检测次数
		str = ir->strInspectionNature.c_str();
		m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);
		// 检测方法
		str = ir->strInspectionMethod.c_str();
		m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);
		// VIN
		str = ir->strVIN.c_str();
		m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);
		// 是否有OBD
		if (strHasOBD == 1)
		{
			str.Format(L"有");
		}
		else
		{
			str.Format(L"无");
		}
		m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, str, 0, 0, 0, 0);
		nItem++;
	}


	return true;
}


bool CYJSocketDlg::Json2Map(LPCTSTR szJson, JsonMap &mapRet)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(KUTF16ToANSI(szJson), root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		Json::Value::Members mem = root.getMemberNames();

		root[""];
		for (auto it=mem.begin(); it!=mem.end(); it++)
		{
			if (root[*it].type() == Json::stringValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", KANSIToUTF16(root[*it].asString().c_str()));
			}
			else if (root[*it].type() == Json::booleanValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", KANSIToUTF16(root[*it].asString().c_str()));
			}
			else if (root[*it].type() == Json::realValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%f", root[*it].asDouble());
			}
			else if (root[*it].type() == Json::uintValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%hu", root[*it].asUInt());
			}
			else if (root[*it].type() == Json::intValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%d", root[*it].asInt());
			}
			else if (root[*it].type() == Json::nullValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", L"");
			}
			else
			{
			}
		}
		return true;
	}
	// 返回的Json格式有误
	return false;
}

CString CYJSocketDlg::Json2Cstring(Json::Value &value)
{
	CString strValue;
	if (value.type() == Json::stringValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::booleanValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::realValue)
	{
		strValue.Format(L"%f", value.asDouble());
	}
	else if (value.type() == Json::uintValue)
	{
		strValue.Format(L"%hu", value.asUInt());
	}
	else if (value.type() == Json::intValue)
	{
		strValue.Format(L"%d", value.asInt());
	}
	else if (value.type() == Json::nullValue)
	{
		strValue.Format(L"%s", L"");
	}
	else
	{
	}
	return strValue;
}

CString CYJSocketDlg::Map2Json(JsonMap mapPost)
{
	CString strJson;
	strJson.AppendFormat(L"{");
	for (JsonMap::const_iterator it=mapPost.begin(); it!=mapPost.end(); ++it)
	{
		strJson.AppendFormat(L"\"%s\":\"%s\"",  it->first, it->second);
		strJson.AppendFormat(L",");
	}
	strJson.TrimRight(L",");
	strJson.AppendFormat(L"}");
	return strJson;
}



void CYJSocketDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	GetVehicleList();
}


void CYJSocketDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	BeginWaitCursor();

	// 设置OBD检测员必须填写
	CString strOBDJCY;
	m_edOBDJCY.GetWindowTextW(strOBDJCY);

	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1 && strOBDJCY != L"")
	{
		// 初始化
		SHJ03 sHJ03;
		m_sHJ03 = sHJ03;

		m_sHJ03.strOBDJCY = strOBDJCY.GetString();

		CString strTemp(L"");

		strTemp = m_lstVehicle.GetItemText(nIndex,1);
		m_sHJ03.strInspectionNum = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,2);
		m_sHJ03.strVLPN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,3);
		m_sHJ03.strVLPNColor = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,4);
		m_sHJ03.strInspectionNature = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,5);
		m_sHJ03.strInspectionMethod = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,6);
		m_sHJ03.strVIN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,7);
		m_sHJ03.strHasOBD = strTemp.GetString();

		if (!m_sHJ03.strInspectionNum.empty() && !m_sHJ03.strVLPN.empty())
		{
			// 选中车牌后可以点击UP
			GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
			m_lbMessage.SetWindowTextW(L"请点击UP");
		}
	}
	else if (strOBDJCY == L"" && nIndex!=-1)
	{
		MessageBox(L"请输入OBD检测员");
		m_lbMessage.SetWindowTextW(L"请输入OBD检测员");
	}
	else if (nIndex==-1 && strOBDJCY != L"")
	{
		MessageBox(L"请双击车牌");
	}

	EndWaitCursor();

	*pResult = 0;
}

bool CYJSocketDlg::UpOBDStart(const CString& strInspectionNum, const CStringW& strStationNum, const CStringW& strLineNum)
{
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", strInspectionNum);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", strLineNum);
	strJsonData.AppendFormat(L"\"Time\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"Code\":\"%s\"}", L"start");

	std::wstring strRecv;
	return SendAndRecvPacket(L"HJ04", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);
}

void CYJSocketDlg::OnBnClickedBtnUpinter()
{
	// TODO: 在此添加控件通知处理程序代码


	CString strStationNum, strLineNum;

	m_edStationNumber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;



	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_sHJ03.strInspectionNum.c_str());
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", m_sHJ03.strVIN.c_str());
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", m_sHJ03.strVLPN.c_str());
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", m_sHJ03.strVLPNColor.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectStartTime\":\"%s\",", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"OBDInspectEndTime\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"ODO\":\"%d\",", 0);// 故障里程
	strJsonData.AppendFormat(L"\"FaultIndicatorLighten\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsCommunicated\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CommFailReason\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"FaultIndicator\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsHaveFaultCode\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"FaultInfo\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"UnDoneReadyStatus\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"UnFinifishedItem\":\"%s\",", L"");

	// 获取OBDType
	srand(time(0));
	int n = rand()%10;
	CString strOBDType(L"");
	CString strFulType(L"汽油");
	if (m_sHJ03.strInspectionMethod.find(L"加") != -1 || m_sHJ03.strInspectionMethod.find(L"加速") != -1)
	{
		strFulType = L"柴油";
	}
	strOBDType.Format(L"%s", GetOBDType(n, strFulType));
	CString strEngineCALID, strEngineCVN;
	// 是否需要重要获取一个？？
	Sleep(1000);
	GetEngineCALID(strOBDType, strFulType, strEngineCALID, strEngineCVN);
	//if (strEngineCALID.IsEmpty() || strEngineCVN.IsEmpty())
	//{
	//	strJsonData.AppendFormat(L"\"ECU_CALID\":\"%s\",", L"不支持");
	//	strJsonData.AppendFormat(L"\"ECU_CVN\":\"%s\",", L"不支持");
	//}
	//else
	{
		strJsonData.AppendFormat(L"\"ECU_CALID\":\"%s\",", strEngineCALID);
		strJsonData.AppendFormat(L"\"ECU_CVN\":\"%s\",", strEngineCVN);
	}
	strJsonData.AppendFormat(L"\"ACU_CALID\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"ACU_CVN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"OCU_CALID\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"OCU_CVN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"StatusCoherence\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CheckPerson\":\"%s\",", m_sHJ03.strOBDJCY.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectResult\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsNeedReCheck\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"ReCheckContent\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"InspectionNature\":\"%s\",", m_sHJ03.strInspectionNature.c_str());
	strJsonData.AppendFormat(L"\"InspectionTimes\":\"%s\"", m_sHJ03.strInspectionNature.c_str());
	strJsonData.AppendFormat(L"\"DeviceMANU\":\"%s\"", L"佛山市南华仪器有限公司");
	strJsonData.AppendFormat(L"\"DeviceType\":\"%s\"", L"NHOBD-1");
	strJsonData.AppendFormat(L"\"OBDType\":\"%s\"", L"1");
	strJsonData.AppendFormat(L"\"RemoteCommunicated\":\"%s\"", L"1");
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	
	UpOBDStart(m_sHJ03.strInspectionNum.c_str(), strStationNum, strLineNum);
	bool bRet = SendAndRecvPacket(L"HJ05", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);
	// 关闭连接
	CloseConnect();
	m_btnSocket.SetWindowTextW(L"连接");

	if (bRet)
	{
		m_lbMessage.SetWindowTextW(L"请正式开始检测后，\r\n获取车辆信息后(既待检列表有此车)点击UPTestLog");
		GetDlgItem(IDC_BTN_UPTESTLOG)->EnableWindow(TRUE);

		//if (!SetSQL(m_sHJ03.strInspectionNum.c_str(), m_sHJ03.strInspectionMethod.c_str()))
		{
			//m_lbMessage.SetWindowTextW(L"操作数据失败请点击“UPTestLog”重新操作");
			//GetDlgItem(IDC_BTN_UPTESTLOG)->EnableWindow(TRUE);
		}
	}
	else
	{
		CString str;
		str.Format(L"失败\r\n%s", strRecv.c_str());
		m_lbMessage.SetWindowTextW(str);
	}

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
}

bool CYJSocketDlg::SetSQL(const CString& strRunningNumber, const CString& strTestType)
{
	CString strSource, strCatalog, strUserID, strPassword;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassword.GetWindowTextW(strPassword);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty())
	{
		m_lbMessage.SetWindowTextW(L"数据库参数为空");
		return false;
	}
	else
	{
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
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"ItemOBD", L"4");
		}
	}

	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		CString strSQL;
		// 先处理TestLog
		strSQL.Format(L"update TestLog set TestState = '0', ItemOBD = '4' WHERE RunningNumber= '%s'", strRunningNumber);
		DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);

		// 获取OBD数据
		strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", strRunningNumber);
		SResultOfOBD sResultOfOBD;
		// 先获取，获取不到再说
		GetDboResultOfOBD(strSQL, &sResultOfOBD);
		if (true)
		{
			sResultOfOBD.strRunningNumber = strRunningNumber;
			// 汽油
			if (strTestType.Find(L"稳态") != -1
				|| strTestType.Find(L"双怠速") != -1
				|| strTestType.Find(L"瞬态") != -1)
			{
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
			else if (strTestType.Find(L"加载") != -1
				|| strTestType.Find(L"不透光") != -1)
			{
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

			// 写入数据库
			if (0x00 ==SetDboResultOfOBD(sResultOfOBD))
			{
			}
			else
			{
			}
			// 写入临时文件
			SetIniResultOfOBD(sResultOfOBD);
		}
	}
	else
	{
		m_lbMessage.SetWindowTextW(L"打开数据库失败");
		return false;
	}

	return true;
}


void CYJSocketDlg::OnBnClickedBtnUptestlog()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_sHJ03.strInspectionNum.empty() && !m_sHJ03.strInspectionMethod.empty())
	{
		if (SetSQL(m_sHJ03.strInspectionNum.c_str(), m_sHJ03.strInspectionMethod.c_str()))
		{
			m_lbMessage.SetWindowTextW(L"操作数据完成");
		}
		else
		{
			m_lbMessage.SetWindowTextW(L"操作数据失败");
		}
	}
	else
	{
		m_lbMessage.SetWindowTextW(L"有参数为空");
	}
}


bool CYJSocketDlg::GetTime()
{
	CString strStationNum, strLineNum;

	m_edStationNumber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ26", strStationNum, strLineNum, GenerateInsNum(), L"", strRecv);
	if (bRet)
	{
		std::wstring strMsg;
		if (!m_yjSocket.RecvPacket(strMsg))
		{
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"返回时间", L"", strRecv.c_str());

			int nIntPos = strRecv.find(L",");
			strRecv = strRecv.substr(nIntPos+1);

			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"返回时间", L"", strRecv.c_str());

			COleDateTime odtNow;
			if (odtNow.ParseDateTime(strRecv.c_str()))
			{
				SYSTEMTIME st;
				st.wYear = odtNow.GetYear();
				st.wMonth = odtNow.GetMonth();
				st.wDay = odtNow.GetDay();
				st.wHour = odtNow.GetHour();
				st.wMinute = odtNow.GetMinute();
				st.wSecond = odtNow.GetSecond();
				st.wMilliseconds = 0;
				// 本机
				if (SetLocalTime(&st))
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"时间同步", L"设置本地时间", L"设置本地时间成功");
				}
				else
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"时间同步", L"设置本地时间", L"设置本地时间失败");
				}
			}
		}
		else
		{
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"返回时间", L"", strMsg.c_str());

			int nIntPos = strMsg.find(L".");
			strMsg = strMsg.substr(0, nIntPos);
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"返回时间", L"", strMsg.c_str());

			COleDateTime odtNow;

			if (odtNow.ParseDateTime(strMsg.c_str()))
			{
				SYSTEMTIME st;
				st.wYear = odtNow.GetYear();
				st.wMonth = odtNow.GetMonth();
				st.wDay = odtNow.GetDay();
				st.wHour = odtNow.GetHour();
				st.wMinute = odtNow.GetMinute();
				st.wSecond = odtNow.GetSecond();
				st.wMilliseconds = 0;
				// 本机
				if (SetLocalTime(&st))
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"时间同步", L"设置本地时间", L"设置本地时间成功");
				}
				else
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"时间同步", L"设置本地时间", L"设置本地时间失败");
				}
			}
		}
	}
	else
	{
		// 获取失败原因
		return false;
	}

	return true;
}



void CYJSocketDlg::OnBnClickedBtnSocket()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_btnSocket.GetWindowTextW(str);

	if (str.Find(L"连接") != -1)
	{
		std::wstring strResult, strMsg;
		if (!m_bAuthentication)
		{
			m_bAuthentication = Authentication(strResult, strMsg);
		}
		if (m_bAuthentication)
		{
			m_lbMessage.SetWindowTextW(L"连接成功");	
			GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);
			m_btnSocket.SetWindowTextW(L"断开");
		}
		else
		{
			m_lbMessage.SetWindowTextW(strMsg.c_str());
		}

	}
	else if (str.Find(L"断开") != -1)
	{
		CloseConnect();
		m_btnSocket.SetWindowTextW(L"连接");
	}
}


CString CYJSocketDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CYJSocketDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	strEngineCALID = L"不支持"; strEngineCVN = L"不支持";
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
