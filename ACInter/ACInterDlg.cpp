
// ACInterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ACInter.h"
#include "ACInterDlg.h"
#include "afxdialogex.h"
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CACInterDlg 对话框




CACInterDlg::CACInterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CACInterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);

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

	m_fontVehicleLCFont.CreateFont(m_nDlgFontSize,
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

}

CACInterDlg::~CACInterDlg(void)
{
	m_fontDlgFont.DeleteObject();
	m_fontVehicleLCFont.DeleteObject();
}

void CACInterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);

	DDX_Control(pDX, ID_ED_JKDZ, m_edJKDZ);
	DDX_Control(pDX, ID_ED_JCZBH, m_edJCZBH);
	DDX_Control(pDX, ID_ED_JCZBH2, m_edJKXLH);
	DDX_Control(pDX, IDC_ED_UPINTER, m_edUPINTER);
	DDX_Control(pDX, IDC_ED_DOWRINTER, m_edDOWRINTER);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJCY);
}

BEGIN_MESSAGE_MAP(CACInterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CACInterDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CACInterDlg::OnBnClickedBtnUpinter)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CACInterDlg::OnLvnItemchangedLstVehicle)
END_MESSAGE_MAP()


// CACInterDlg 消息处理程序

BOOL CACInterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_lstVehicle.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstVehicle.SetFont(&m_fontVehicleLCFont);
	m_lstVehicle.SetTextColor(RGB(0, 255, 0));
	m_lstVehicle.SetBkColor(RGB(0, 0, 0));
	m_lstVehicle.SetTextBkColor(RGB(0, 0, 0));

	m_lstVehicle.InsertColumn(0, L"序号",		0,	0);
	m_lstVehicle.InsertColumn(1, L"流水号",		0,	220/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(2, L"车牌号码",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(3, L"检测次数",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(4, L"车牌类型",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(5, L"检测方法",	0,	120/**nSM_CXSCREEN/1280*/);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
	SetConfig();

	SetRefreshVehicleList();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CACInterDlg::OnPaint()
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
HCURSOR CACInterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CACInterDlg::SetConfig(void)
{
	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"ACWebConfig.ini", wchPath))
	{
	}

	CSimpleIni si(wchPath);

	CString strJKDZ, strJKXLH, strJCZBH;

	strJKDZ = si.GetString(L"EPB", L"WebAddress", L"");
	strJKXLH = si.GetString(L"EPB", L"JKXLH", L"");
	strJCZBH = si.GetString(L"EPB", L"JCZBH", L"");

	m_edJKDZ.SetWindowTextW(strJKDZ);
	m_edJKXLH.SetWindowTextW(strJKXLH);
	m_edJCZBH.SetWindowTextW(strJCZBH);
}

void CACInterDlg::SetRefreshVehicleList(void)
{
	// 禁止点击更新按钮
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);

	CString strXmlDoc, strTemp, strsjc,strMsg;
	CString strJKDZ, strJKXLH, strJCZBH;

	m_edJKDZ.GetWindowTextW(strJKDZ);
	m_edJKXLH.GetWindowTextW(strJKXLH);
	m_edJCZBH.GetWindowTextW(strJCZBH);


	SYSTEMTIME st;
	GetLocalTime(&st);
	strsjc.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// 头串
	strXmlDoc.Format(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strTemp.AppendFormat(L"<organ>%s</organ>", strJCZBH);
	strTemp.AppendFormat(L"<jkxlh>%s</jkxlh>", strJKXLH);
	strTemp.AppendFormat(L"<jkid>%s</jkid>", L"DJ001");
	strTemp.AppendFormat(L"<sjc>%s</sjc>", strsjc);
	strXmlDoc = strXmlDoc + strTemp;
	strXmlDoc.AppendFormat(L"</head><vehispara>");
	strXmlDoc.AppendFormat(L"</vehispara></root>");

	strMsg.Format(L"%s", strXmlDoc);
	m_edUPINTER.SetWindowTextW(strMsg);

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

	std::wstring strRetStr(L"");

	int nRet;

	nRet = CACInterfaceLib_API::QueryObjectOut(pchURL, strXmlDoc.GetString(), strRetStr);

	// 调用联网接口后，可以点击更新
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);

	std::list<SDJ001> lsDJ001;
	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)DecodeURI(strRetStr.c_str());

		strMsg.Format(L"获取列表信息返回：\r\n%d\r\n%s", nRet, strRetStr.c_str());
		m_edDOWRINTER.SetWindowTextW(strMsg);

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
				strMsg.Format(L"获取列表信息返回：%s", strRetStr.c_str());
				m_edDOWRINTER.SetWindowTextW(strMsg);

				if (xmlReader.OpenNode(L"root/body/vehispara"))
				{
					std::wstring strNodeName, strName, strContent;
					do
					{
						SDJ001 sDJ001;
						xmlReader.EnterNode();
						do
						{
							xmlReader.GetNodeName(strNodeName);
							// 检测站编号
							if(L"tsno"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strtsno); 
							}
							// 检验流水号
							if(L"jylsh"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strjylsh); 
							}
							// 检验次数
							if(L"testtimes"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strtesttimes); 
							}
							// 号牌号码
							if(L"license"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strlicense); 
							}
							// 号牌种类
							if(L"licensecode"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strlicensecode); 
							}
							// 检测方法
							if(L"testtype"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strtesttype); 
							}
							// 登录时间
							if(L"dlsj"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strdlsj); 
							}
							// 状态
							if(L"zt"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(sDJ001.strzt); 
							}
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();
						lsDJ001.push_back(sDJ001);
					}
					while (xmlReader.NextSibling());

					
				}
			}
			else
			{
				//strMsg.Format(L"获取列表信息返回：\r\n%s\r\n%s", strCode.c_str(), strContent.c_str());
				//m_edDOWRINTER.SetWindowTextW(strMsg);
				return;
			}


		}
		else
		{
			//strMsg.Format(L"获取列表信息返回：\r\n%d\r\n%s", nRet, L"解析失败");
			//m_edDOWRINTER.SetWindowTextW(strMsg);
			return;
		}
	}
	else
	{
		strMsg.Format(L"获取列表信息返回：\r\n%d\r\n%s", nRet, L"接口访问失败");
		m_edDOWRINTER.SetWindowTextW(strMsg);
		return;
	}


	// 删除列表数据
	m_lstVehicle.DeleteAllItems();

	int nItem(0);
	for (list<SDJ001>::iterator ir=lsDJ001.begin(); ir!=lsDJ001.end(); ++ir)
	{
		CString strzt = ir->strzt.c_str();
		if (L"1" != strzt)
		{
			continue;
		}

		CString str;
		// 序号
		str.Format(L"%d", nItem+1);

		int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

		// 流水号
		str = ir->strjylsh.c_str();
		m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);
		// 车牌号码
		str = ir->strlicense.c_str();
		m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);
		// 检测次数
		str = ir->strtesttimes.c_str();
		m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);
		// 车牌类型
		str = ir->strlicensecode.c_str();
		m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);
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
		m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

		nItem++;
	}


}

void CACInterDlg::GetVehInfo(const CString strjylsh, const CString strtesttimes)
{
	CString strXmlDoc, strTemp, strsjc,strMsg;
	CString strJKDZ, strJKXLH, strJCZBH;

	m_edJKDZ.GetWindowTextW(strJKDZ);
	m_edJKXLH.GetWindowTextW(strJKXLH);
	m_edJCZBH.GetWindowTextW(strJCZBH);


	SYSTEMTIME st;
	GetLocalTime(&st);
	strsjc.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// 头串
	strXmlDoc.Format(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strTemp.AppendFormat(L"<organ>%s</organ>", strJCZBH);
	strTemp.AppendFormat(L"<jkxlh>%s</jkxlh>", strJKXLH);
	strTemp.AppendFormat(L"<jkid>%s</jkid>", L"DL002");
	strTemp.AppendFormat(L"<sjc>%s</sjc>", strsjc);
	strXmlDoc = strXmlDoc + strTemp;
	strXmlDoc.AppendFormat(L"</head><vehispara>");
	strXmlDoc.AppendFormat(L"<jylsh>%s</jylsh>", strjylsh);
	strXmlDoc.AppendFormat(L"<testtimes>%s</testtimes>", strtesttimes);
	strXmlDoc.AppendFormat(L"</vehispara></root>");

	strMsg.Format(L"%s", strXmlDoc);
	m_edUPINTER.SetWindowTextW(strMsg);


	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

	std::wstring strRetStr;

	int nRet;

	nRet = CACInterfaceLib_API::QueryObjectOut(pchURL, strXmlDoc.GetString(), strRetStr);

	strMsg.Format(L"获取车辆登录信息返回：\r\n%d\r\n%s", nRet, strRetStr.c_str());
	m_edDOWRINTER.SetWindowTextW(strMsg);

	SDL002 sDL002;
	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)DecodeURI(strRetStr.c_str());

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
				strMsg.Format(L"获取车辆登录信息返回：%s", strRetStr.c_str());
				m_edDOWRINTER.SetWindowTextW(strMsg);

				// jylsh
				if (xmlReader.OpenNode(L"root/body/vehispara/jylsh"))
				{
					xmlReader.GetNodeContent(strContent);
					sDL002.strjylsh = strContent.c_str();
				}
				
				// testtimes
				if (xmlReader.OpenNode(L"root/body/vehispara/testtimes"))
				{
					xmlReader.GetNodeContent(strContent);
					sDL002.strtesttimes = strContent.c_str();
				}

				// vin
				if (xmlReader.OpenNode(L"root/body/vehispara/vin"))
				{
					xmlReader.GetNodeContent(strContent);
					sDL002.strvin = strContent.c_str();
				}

				// vin
				if (xmlReader.OpenNode(L"root/body/vehispara/fueltype"))
				{
					xmlReader.GetNodeContent(strContent);
					sDL002.strfueltype = strContent.c_str();
				}
			}
			else
			{
				strMsg.Format(L"获取车辆登录信息返回：\r\n%s\r\n%s", strCode.c_str(), strContent.c_str());
				m_edDOWRINTER.SetWindowTextW(strMsg);
				return;
			}

		}
		else
		{
			//strMsg.Format(L"获取车辆登录信息返回：\r\n%d\r\n%s", nRet, L"解析失败");
			//m_edDOWRINTER.SetWindowTextW(strMsg);
			return;
		}
	}
	else
	{
		strMsg.Format(L"获取车辆登录信息返回：\r\n%d\r\n%s", nRet, L"接口访问失败");
		m_edDOWRINTER.SetWindowTextW(strMsg);
		return;
	}



	// 写上传OBD
	CString strOBDJCY;
	m_edOBDJCY.GetWindowTextW(strOBDJCY);

	COleDateTime odtParseEnd(COleDateTime::GetCurrentTime());
	COleDateTimeSpan odtParse(0,0,0,20);
	COleDateTime odtParseStat = odtParseEnd - odtParse;
	
	strXmlDoc = L"";
	strTemp = L"";

	GetLocalTime(&st);
	strsjc.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strXmlDoc.Format(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strXmlDoc.AppendFormat(L"<organ>%s</organ>", strJCZBH);
	strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", strJKXLH);
	strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"JC011");
	strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strsjc);
	strXmlDoc.AppendFormat(L"</head><body><vehispara>");
	strXmlDoc.AppendFormat(L"<jylsh>%s</jylsh>", sDL002.strjylsh.c_str());
	strXmlDoc.AppendFormat(L"<testtimes>%s</testtimes>", sDL002.strtesttimes.c_str());
	strXmlDoc.AppendFormat(L"<tsno>%s</tsno>", strJCZBH);
	strXmlDoc.AppendFormat(L"<jyksrq>%s</jyksrq>", odtParseStat.Format(L"%Y-%m-%d %H:%M:%S"));
	strXmlDoc.AppendFormat(L"<jyjsrq>%s</jyjsrq>", odtParseEnd.Format(L"%Y-%m-%d %H:%M:%S"));
	strXmlDoc.AppendFormat(L"<jyy>%s</jyy>", (strOBDJCY == L""? L"刘家成": strOBDJCY));
	strXmlDoc.AppendFormat(L"<jyjg>%s</jyjg>", L"1");
	strXmlDoc.AppendFormat(L"<obdzdyscqy>%s</obdzdyscqy>", L"佛山市南华仪器有限公司");
	strXmlDoc.AppendFormat(L"<obdzdyxh>%s</obdzdyxh>", L"NHOBD-1");
	strXmlDoc.AppendFormat(L"<vin>%s</vin>", sDL002.strvin.c_str());

	CString strOBDType, strEngineCALID, strEngineCVN, strFulType;
	srand(time(0));
	int n = rand()%10;
	if (sDL002.strfueltype.find(L"B") != std::string::npos)
	{
		strFulType.Format(L"柴油");
	}
	else
	{
		strFulType.Format(L"汽油");
	}
	strOBDType.Format(L"%s", GetOBDType(n, strFulType)); 
	strXmlDoc.AppendFormat(L"<obdyq>%s</obdyq>", strOBDType);

	strXmlDoc.AppendFormat(L"<gzjg>%s</gzjg>", L"1");
	strXmlDoc.AppendFormat(L"<tx>%s</tx>", L"1");
	strXmlDoc.AppendFormat(L"<txbz>%s</txbz>", L"");
	strXmlDoc.AppendFormat(L"<bj>%s</bj>", L"1");
	strXmlDoc.AppendFormat(L"<bjbz>%s</bjbz>", L"");
	strXmlDoc.AppendFormat(L"<jxxm>%s</jxxm>", L"1");
	strXmlDoc.AppendFormat(L"<jxxmbz>%s</jxxmbz>", L"");
	strXmlDoc.AppendFormat(L"<odometer>%s</odometer>", L"0");
	strXmlDoc.AppendFormat(L"<odomil>%s</odomil>", L"");
	Sleep(1000);
	GetEngineCALID(strOBDType, strFulType, strEngineCALID, strEngineCVN);
	strXmlDoc.AppendFormat(L"<enginecalid>%s</enginecalid>", strEngineCALID);
	strXmlDoc.AppendFormat(L"<enginecvn>%s</enginecvn>", strEngineCVN);
	strXmlDoc.AppendFormat(L"<hclcalid>%s</hclcalid>", L"");
	strXmlDoc.AppendFormat(L"<hclcvn>%s</hclcvn>", L"");
	strXmlDoc.AppendFormat(L"<calid>%s</calid>", L"");
	strXmlDoc.AppendFormat(L"<cvn>%s</cvn>", L"");
	//strXmlDoc.AppendFormat(L"<iupr>%s</iupr>", L"");
	strXmlDoc.AppendFormat(L"</vehispara></body></root>");

	m_edUPINTER.SetWindowTextW(strXmlDoc);

	
}

CString CACInterDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CACInterDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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

void CACInterDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码

	SetRefreshVehicleList();
}


void CACInterDlg::OnBnClickedBtnUpinter()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strMsg;
	CString strJKDZ, strXmlDoc;

	m_edJKDZ.GetWindowTextW(strJKDZ);
	m_edUPINTER.GetWindowTextW(strXmlDoc);

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

	std::wstring strRetStr;

	int nRet;

	nRet = CACInterfaceLib_API::WriteObjectOut(pchURL, strXmlDoc.GetString(), strRetStr);

	// 上传后UP置为不可点击
	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)DecodeURI(strRetStr.c_str());

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
				strMsg.Format(L"获取返回：%s", strRetStr.c_str());
				m_edDOWRINTER.SetWindowTextW(strMsg);
			}
			else
			{
				strMsg.Format(L"获取返回：\r\n%s\r\n%s", strCode.c_str(), strContent.c_str());
				m_edDOWRINTER.SetWindowTextW(strMsg);
				return;
			}

		}
		else
		{
			strMsg.Format(L"获取返回：\r\n%d\r\n%s", nRet, L"解析失败");
			m_edDOWRINTER.SetWindowTextW(strMsg);
			return;
		}
	}
	else
	{
		strMsg.Format(L"获取返回：\r\n%d\r\n%s", nRet, L"接口访问失败");
		m_edDOWRINTER.SetWindowTextW(strMsg);
		return;
	}
}


void CACInterDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
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
		CString strjylsh(L""), strtesttimes(L"");

		strjylsh = m_lstVehicle.GetItemText(nIndex,1);
		strtesttimes = m_lstVehicle.GetItemText(nIndex,3);

		if (strjylsh != L"" && strtesttimes != L"")
		{
			// 选中车牌后可以点击UP
			GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
			GetVehInfo(strjylsh, strtesttimes);
		}
	}
	else if (strOBDJCY == L"" && nIndex!=-1)
	{
		MessageBox(L"请输入OBD检测员");
	}
	else if (nIndex==-1 && strOBDJCY != L"")
	{
		MessageBox(L"请双击车牌");
	}
	
	EndWaitCursor();

	*pResult = 0;
}


CString CACInterDlg::DecodeURI(LPCSTR szURIString)
{
	std::string strSource(szURIString);
	int nSrcLen = strSource.size();

	std::string strDestA;

	int nIndex(0);
	while (nIndex < nSrcLen)
	{
		if (strSource.at(nIndex) == '%'
			&& nIndex+2 <= nSrcLen)
		{
			std::string strValue = strSource.substr(nIndex, 3);
			int nValue(0);
			sscanf_s(strValue.c_str(), "%%%x", &nValue);
			strDestA += (char)nValue;
			nIndex += 3;
		}
		else if (strSource.at(nIndex) == '+')
		{
			strDestA += ' ';
			nIndex++;
		}
		else
		{
			strDestA += (char)strSource.at(nIndex);
			nIndex++;
		}
	}
	CString strDest(UTF8ToTChar(strDestA.c_str()));
	return strDest;
}

CString CACInterDlg::DecodeURI(LPCWSTR szURIString)
{
	return DecodeURI(KUTF16ToANSI(szURIString));
}

