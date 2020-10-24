
// JNSXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JNSX.h"
#include "JNSXDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJNSXDlg 对话框




CJNSXDlg::CJNSXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJNSXDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJNSXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_JKDZ, m_edJKDZ);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbMessage);
	DDX_Control(pDX, IDC_CXDATE, m_edCXDate);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJXY);
}

BEGIN_MESSAGE_MAP(CJNSXDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CJNSXDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CJNSXDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CJNSXDlg::OnBnClickedBtnUpinter)
END_MESSAGE_MAP()


// CJNSXDlg 消息处理程序

BOOL CJNSXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 信息字体
	{
		CFont font;
		font.CreatePointFont(320,L"宋体");
		m_lbMessage.SetFont(&font);
	}

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJNSXDlg::OnPaint()
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
HCURSOR CJNSXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJNSXDlg::SetConfig(void)
{
	// 获取exe(dll)文件绝对路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"JNConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"EPB", L"WebAddress", L"");
		m_edJKDZ.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edLineNumber.SetWindowTextW(strTemp);
	}

}

void CJNSXDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(4, L"燃料类型",	0,	120);
	m_lstVehicle.InsertColumn(5, L"VIN",	0,	300);
	m_lstVehicle.InsertColumn(6, L"是否复检",	0,	100);
	m_lstVehicle.InsertColumn(7, L"外观检查结果",	0,	100);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}

bool CJNSXDlg::GetVehicleList(void)
{
	// 禁止点击更新按钮
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);

	CString strStaDate(L"");
	m_edCXDate.GetWindowTextW(strStaDate);
	strStaDate.AppendFormat(L" 00:00:00");

	CString strJKDZ(L"");
	m_edJKDZ.GetWindowTextW(strJKDZ);

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

	std::wstring strRet;
	int nRet(99);

	nRet = CJNSXInterfaceLib_API::GetOtherInspectQueueByDate(pchURL, strStaDate.GetString(), L"", L"1", L"" , strRet);

	// 调用联网接口后，可以点击更新
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);

	CString strMsg; 
	std::list<SVehDate> lsVehDate;

#ifdef _DEBUG
	nRet = 0;
	CStringW cstrCS;
	cstrCS.AppendFormat(L"\n");
	cstrCS.AppendFormat(L"0\n");
	cstrCS.AppendFormat(L"成功\n");
	cstrCS.AppendFormat(L"0\n");
	cstrCS.AppendFormat(L"\n");
	cstrCS.AppendFormat(L"<Count>1</Count><Inspect_1><InspectID>YCBC20191129171555540</InspectID><IsFirstInspect>2</IsFirstInspect>\
						 <WJResult>1</WJResult><InspectMethod>1</InspectMethod><CarInfo><VIN>LVVDB12A99D396836</VIN><PlateID>鲁R50W75</PlateID>\
						 <PlateType>1</PlateType><HPZL>02</HPZL><BrandName>奇瑞</BrandName><ModelName>SQR7080S117</ModelName>\
						 <EngineModel>SQR372</EngineModel><EngineSN>FD9L07576</EngineSN><VehicleTypeCode>K33</VehicleTypeCode>\
						 <IfHGL>false</IfHGL><CarType>1</CarType><IfGoIntoCity>true</IfGoIntoCity><IsTurbo>false</IsTurbo><FuelType>1</FuelType>\
						 <FuelSupplyID>1</FuelSupplyID><Is3WCC>True</Is3WCC><RatedSpeed>5000</RatedSpeed><DeliveryCapacity>0.8</DeliveryCapacity>\
						 <Cylinders>4</Cylinders><StrokeCycles></StrokeCycles><NorminalPower>38</NorminalPower><NominalPower>38</NominalPower>\
						 <FactoryDate>2009-11-01</FactoryDate><BaseWeight>980</BaseWeight><WholeWeight>1255</WholeWeight><RegDate>2009-12-23</RegDate>\
						 <PassengerCount>5</PassengerCount><GearType>1</GearType><InspectCount>3</InspectCount><DriveType>1</DriveType><EPStage>4</EPStage>\
						 <Owner>张祥</Owner><OwnerTel>18364009686</OwnerTel><OwnerAddress>山东省郓城县郓城镇</OwnerAddress><OdoMeter>47464</OdoMeter>\
						 <ExhaustPipesNum>1</ExhaustPipesNum><AxleWeight></AxleWeight><ChassisModel></ChassisModel><ChassisFactory></ChassisFactory></CarInfo></Inspect_1>\n");
	cstrCS.AppendFormat(L"2019-11-29 17:17:52,请忽略本提示。\n");
	strRet = cstrCS.GetString();
#endif
	if (nRet == 0)
	{
		CString strTipMessage, strErrNum, strErrMsg, strInternalErrNum, strInternalErrMsg, strValue, strMemo, cstrRet;
		cstrRet = strRet.c_str();
		AfxExtractSubString(strTipMessage, cstrRet, 0, L'\n');
		AfxExtractSubString(strErrNum, cstrRet, 1, L'\n');
		AfxExtractSubString(strErrMsg, cstrRet, 2, L'\n');
		AfxExtractSubString(strInternalErrNum, cstrRet, 3, L'\n');
		AfxExtractSubString(strInternalErrMsg, cstrRet, 4, L'\n');
		AfxExtractSubString(strValue, cstrRet, 5, L'\n');
		AfxExtractSubString(strMemo, cstrRet, 6, L'\n');

		if (!strTipMessage.IsEmpty())
		{
			MessageBox(strTipMessage);
			m_lbMessage.SetWindowTextW(strTipMessage);
		}

		if (strErrNum == L"0")
		{
			if (strInternalErrNum != L"0")
			{
				//MessageBox(strInternalErrMsg);
			}

			CXmlReader xmlReader;
			if (xmlReader.Parse(strValue))
			{
				std::wstring strCode, strContent;

				if (xmlReader.OpenNode(L"Count"))
				{
					xmlReader.GetNodeContent(strCode);
				}
				if (strCode == L"0")
				{
					m_lbMessage.SetWindowTextW(L"平台待检为空");
					return true;
				}
				else
				{
					int n = _wtoi(strCode.c_str());

					SVehDate sVehDate;

					for (int i=0; i<n ; i++)
					{
						CString strXmlCode;
						strXmlCode.Format(L"Inspect_%d/", i+1);
						// 检测记录流水号
						if (xmlReader.OpenNode((strXmlCode + L"InspectID")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strInspectID = strContent;
						}
						// IsFirstInspect 检验类型，int 型；1.：初检；2：复检。（V3.0 新增）
						if (xmlReader.OpenNode((strXmlCode + L"IsFirstInspect")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strIsFirstInspect = strContent;
						}
						// WJResult 外观检查结果，int 型；1: 合格。（V3.0 新增）
						if (xmlReader.OpenNode((strXmlCode + L"WJResult")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strWJResult = strContent;
						}
						// CarInfo 车辆信息，Xml 格式，格式见下方表 4
						// VIN String 车辆识别代号 VIN
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/VIN")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strVIN = strContent;
						}
						// PlateID string 号牌号码。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/PlateID")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strPlateID = strContent;
						}
						// PlateType int 号牌种类，见枚举表-号牌种类。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/PlateType")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strPlateType = strContent;
						}
						// BrandName string 车辆厂牌。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/BrandName")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ModelName string 车辆型号。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ModelName")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// EngineModel String 发动机型号
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/EngineModel")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// EngineSN String 发动机号码
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/EngineSN")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// CarType int 车辆类型，见枚举表-车辆类型。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/CarType")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// VehicleTypeCode String 车辆详细类型代号；见枚举信息-车辆详细类型。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/VehicleTypeCode")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// IfHGL bool 是否黄改绿车辆 true：是（黄改绿车辆）； false：否（非黄改绿车辆）。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/IfHGL")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// IfGoIntoCity bool （仅用于“低速货车（农用车）”） 该车辆是否进入城镇建成区： true：是（进入城镇建成区）； false：否（不进入城镇建成区）。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/IfGoIntoCity")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// IsTurbo bool 是否带有涡轮增压： true：是（涡轮增压）； false：否（自然进气）。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/IsTurbo")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// FuelType int 燃料类型，见枚举表-燃料类型。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/FuelType")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strFuelType = strContent;
						}
						// FuelSupplyID Int 供油方式，见枚举表-供油方式。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/FuelSupplyID")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// Is3WCC bool 是否使用三元催化转换装置： true: 使用； false: 不使用。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/Is3WCC")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// RatedSpeed int 发动机额定转速（仅数字部分）。单位：rpm。 对于摩托车或轻便摩托车，此值为“高怠速转速”的 2 倍。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/RatedSpeed")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// DeliveryCapacity string 发动机排量（仅数字部分）。单位：L。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/DeliveryCapacity")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// Cylinders int 发动机缸数
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/Cylinders")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// StrokeCycles int 发动机冲程数，2 或 4（仅用于摩托车或轻便摩托车）
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/StrokeCycles")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// NominalPower int 发动机标定功率（仅用于加载减速工况法）
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/NominalPower")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// FactoryDate string 出厂日期。格式为“yyyy-MM-dd”。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/FactoryDate")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// BaseWeight string 基准质量（仅数字部分）。单位：kg。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/BaseWeight")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// WholeWeight string 最大总质量（仅数字部分）。单位：kg。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/WholeWeight")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// RegDate string 初次登记日期。格式为“yyyy-MM-dd”。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/RegDate")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// PassengerCount int 核载人数(包括驾驶员)。 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/PassengerCount")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// GearType int 变速器形式，见枚举表-变速器形式。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/GearType")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// InspectCount int 检测次数 DriveType int 驱动类型，见枚举表-驱动类型。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/InspectCount")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// EPStage Int 车辆排放阶段，见枚举表-排放阶段。
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/EPStage")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// Owner string 车主姓名 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/Owner")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// OwnerTel string 车主电话 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/OwnerTel")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// OwnerAddress string 车主地址 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/OwnerAddress")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// OdoMeter string 累计行驶里程数 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/OdoMeter")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ExhaustPipesNum Int 排气管数量 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ExhaustPipesNum")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// AxleWeight string 单车轴重（用于简易瞬态工况法，若无信息，则为空） 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/AxleWeight")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ChassisModel string 底盘型号（用于简易瞬态工况法，若无信息，则为空） 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ChassisModel")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ChassisFactory string 底盘生产企业（用于简易瞬态工况法，若无信息，则为空）
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ChassisFactory")))
						{
							xmlReader.GetNodeContent(strContent);
						}

						lsVehDate.push_back(sVehDate);
					}
				}

			}
			else
			{
				strMsg.Format(L"解析错误");
				m_lbMessage.SetWindowTextW(strMsg);
				return false;
			}

		}
		else
		{
			MessageBox(strErrMsg);
			m_lbMessage.SetWindowTextW(strErrMsg);
			return false;
		}

	}
	else
	{
		strMsg.Format(L"连接错误\r\n返回代号：%d", nRet);
		m_lbMessage.SetWindowTextW(strMsg);
		return false;
	}

	// 删除列表数据
	m_lstVehicle.DeleteAllItems();
	int nItem(0);
	for (list<SVehDate>::iterator ir=lsVehDate.begin(); ir!=lsVehDate.end(); ++ir)
	{
		CString str;
		// 序号
		str.Format(L"%d", nItem+1);
		int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

		// 流水号
		m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strInspectID.c_str(), 0, 0, 0, 0);
		// 车牌号码
		m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strPlateID.c_str(), 0, 0, 0, 0);
		// 车牌类型
		m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strPlateType.c_str(), 0, 0, 0, 0);
		// 燃料类型
		m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strFuelType.c_str(), 0, 0, 0, 0);
		// VIN
		m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strVIN.c_str(), 0, 0, 0, 0);
		// 是否复检
		str = ir->strIsFirstInspect.c_str();
		if (str == L"2"){str = L"复检";}
		else{str = L"初检";}
		m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);
		// 外观检查结果
		m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, ir->strWJResult.c_str(), 0, 0, 0, 0);

		nItem++;
	}

	return true;
}

void CJNSXDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bret = GetVehicleList();

	if (bret)
	{
		CString strMsg;
		m_lbMessage.GetWindowTextW(strMsg);
		strMsg.AppendFormat(L"\r\n获取车辆信息成功返回");
		m_lbMessage.SetWindowTextW(strMsg);
	}
}


void CJNSXDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	BeginWaitCursor();
	
	m_sVehDate = SVehDate();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		CString strTemp(L"");

		strTemp = m_lstVehicle.GetItemText(nIndex,1);
		m_sVehDate.strInspectID = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,2);
		m_sVehDate.strPlateID = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,3);
		m_sVehDate.strPlateType = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,4);
		m_sVehDate.strFuelType = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,5);
		m_sVehDate.strVIN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,6);
		m_sVehDate.strIsFirstInspect = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,7);
		m_sVehDate.strWJResult = strTemp.GetString();

		// 选中车牌后可以点击UP
		GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
	}

	EndWaitCursor();

	*pResult = 0;
}


void CJNSXDlg::OnBnClickedBtnUpinter()
{
	// TODO: 在此添加控件通知处理程序代码

	CString cstrLineNumber(L""), cstrOBDJCY(L"");

	m_edLineNumber.GetWindowTextW(cstrLineNumber);
	m_edOBDJXY.GetWindowTextW(cstrOBDJCY);

	if (!cstrLineNumber.IsEmpty() && !cstrOBDJCY.IsEmpty())
	{
		CString strJKDZ(L"");
		m_edJKDZ.GetWindowTextW(strJKDZ);

		char* pchURL(NULL);

		if (NULL != pchURL)
		{
			free(pchURL);
			pchURL = NULL;
		}

		pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

		CString cstrsOBDData(L"");

		COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 40);
		COleDateTime odtSartTime = odtCurTime - odts;

		
		// 组字符串
		{
			CString cstrOBDdata(L"");
			cstrsOBDData.AppendFormat(L"<OBDGZZSQ>0</OBDGZZSQ>");
			cstrsOBDData.AppendFormat(L"<OBDInspectCount>1</OBDInspectCount>");
			cstrsOBDData.AppendFormat(L"<OBDTXZT>1</OBDTXZT>");
			cstrsOBDData.AppendFormat(L"<OBDTXSBYY>1</OBDTXSBYY>");
			cstrsOBDData.AppendFormat(L"<OBDGZ>0</OBDGZ>");
			cstrsOBDData.AppendFormat(L"<OBDGZDM></OBDGZDM>");
			cstrsOBDData.AppendFormat(L"<OBDJXZT>0</OBDJXZT>");
			cstrsOBDData.AppendFormat(L"<OBDWWCXM>0</OBDWWCXM>");
			cstrsOBDData.AppendFormat(L"<OBDWWCXM>0</OBDWWCXM>");
			cstrsOBDData.AppendFormat(L"<MILDLXSLC>0.000</MILDLXSLC>");
			cstrsOBDData.AppendFormat(L"<FDJKZDYCALID>不支持</FDJKZDYCALID>");
			cstrsOBDData.AppendFormat(L"<FDJKZDYCVN>不支持</FDJKZDYCVN>");
			cstrsOBDData.AppendFormat(L"<HCLKZDYCALID></HCLKZDYCALID>");
			cstrsOBDData.AppendFormat(L"<HCLKZDYCVN></HCLKZDYCVN>");
			cstrsOBDData.AppendFormat(L"<QTKZDYCALID></QTKZDYCALID>");
			cstrsOBDData.AppendFormat(L"<QTKZDYCVN></QTKZDYCVN>");
			cstrsOBDData.AppendFormat(L"<OBDResult>1</OBDResult>");
			cstrsOBDData.AppendFormat(L"<JYR>%s</JYR>", cstrOBDJCY); // 检验人 ID
			cstrsOBDData.AppendFormat(L"<BeginTime>%s</BeginTime>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
			cstrsOBDData.AppendFormat(L"<EndTime>%s</EndTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			cstrsOBDData.AppendFormat(L"<IsReInspect>0</IsReInspect>");
			cstrsOBDData.AppendFormat(L"<ReInspectProject></ReInspectProject>");
			cstrsOBDData.AppendFormat(L"<ReOBDResult>0</ReOBDResult>");
			{
				CString strdata(L"");

				cstrOBDdata.AppendFormat(L"<VIN>%s</VIN>", m_sVehDate.strVIN.c_str());
				cstrOBDdata.AppendFormat(L"<OBDYQ></OBDYQ>");
				cstrOBDdata.AppendFormat(L"<ODO>0</ODO>");
				cstrOBDdata.AppendFormat(L"<ODO>0</ODO>");
				{
					CString strTemp1;
					strdata.AppendFormat(L"<KZDYXXCount>1</KZDYXXCount>");
					{
						strTemp1.AppendFormat(L"<KZDYMC>发动机</KZDYMC>");
						strTemp1.AppendFormat(L"<CALID>不支持</CALID>");
						strTemp1.AppendFormat(L"<CVN>不支持</CVN>");
					}
					strdata.AppendFormat(L"<KZDYXX001>%s</KZDYXX001>", strTemp1);
				}
				cstrOBDdata.AppendFormat(L"<KZDYXX>%s</KZDYXX>", strdata);
				cstrOBDdata.AppendFormat(L"<GZNR_GZDM>无</GZNR_GZDM>");
				cstrOBDdata.AppendFormat(L"<GZNR_MIL>0.000</GZNR_MIL>");
				cstrOBDdata.AppendFormat(L"<JXZT_GZZDQMS>无</JXZT_GZZDQMS>");

				strdata = L"";
				{
					CString strTemp1;
					strdata.AppendFormat(L"<JXZTCount>2</JXZTCount>");
					{
						strTemp1.AppendFormat(L"<SBMC>1</SBMC>");
						strTemp1.AppendFormat(L"<SBZT>1</SBZT>");
					}
					strdata.AppendFormat(L"<JXZT001>%s</JXZT001>", strTemp1);
					strTemp1 = L"";
					{
						strTemp1.AppendFormat(L"<SBMC>2</SBMC>");
						strTemp1.AppendFormat(L"<SBZT>1</SBZT>");
					}
					strdata.AppendFormat(L"<JXZT002>%s</JXZT002>", strTemp1);
				}
				cstrOBDdata.AppendFormat(L"<JXZT_JXZT>%s</JXZT_JXZT>", strdata);
				strdata = L"";
				{
					CString strTemp1;
					strdata.AppendFormat(L"<EGRVVTCount>1</EGRVVTCount>");
					{
						strTemp1.AppendFormat(L"<XMMC>EGR</XMMC>");
						strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
						strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
						strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
					}
					strdata.AppendFormat(L"<EGRVVT001>%s</EGRVVT001>", strTemp1);
				}
				cstrOBDdata.AppendFormat(L"<IUPR_EGRVVT>%s</IUPR_EGRVVT>", strdata);
				if (m_sVehDate.strFuelType == L"柴油")
				{
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<NMHCCount>1</NMHCCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>催化器</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<NMHC001>%s</NMHC001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_NMHC>%s</IUPR_NMHC>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<NOXCHCount>1</NOXCHCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>NOX催化器</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<NOXCH001>%s</NOXCH001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_NOXCH>%s</IUPR_NOXCH>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<NOXXFCount>1</NOXXFCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>NOX吸附器</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<NOXXF001>%s</NOXXF001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_NOXXF>%s</IUPR_NOXXF>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<PMCount>1</PMCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>PM捕集器</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<PM001>%s</PM001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_PM>%s</IUPR_PM>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<FQCGQCount>1</FQCGQCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>废气传感器</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<FQCGQ001>%s</FQCGQ001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_FQCGQ>%s</IUPR_FQCGQ>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<ZYYLCount>1</ZYYLCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>增压压力</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<ZYYL001>%s</ZYYL001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_ZYYL>%s</IUPR_ZYYL>", strdata);
				}
				// 混合动力也默认汽油
				else
				{
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<CHQ1Count>1</CHQ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>催化器组1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<CHQ1001>%s</CHQ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_CHQ1>%s</IUPR_CHQ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<CHQ2Count>1</CHQ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>催化器组2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<CHQ2001>%s</CHQ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_CHQ2>%s</IUPR_CHQ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<QYCGQZ1Count>1</QYCGQZ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>前氧传感器组1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<QYCGQZ1001>%s</QYCGQZ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_QYCGQZ1>%s</IUPR_QYCGQZ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<QYCGQZ2Count>1</QYCGQZ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>前氧传感器组2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<QYCGQZ2001>%s</QYCGQZ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_QYCGQZ2>%s</IUPR_QYCGQZ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<HYCGQZ1Count>1</HYCGQZ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>后氧传感器组1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<HYCGQZ1001>%s</HYCGQZ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_HYCGQZ1>%s</IUPR_HYCGQZ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<HYCGQZ2Count>1</HYCGQZ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>后氧传感器组2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<HYCGQZ2001>%s</HYCGQZ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_HYCGQZ2>%s</IUPR_HYCGQZ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<EVAPCount>1</EVAPCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>EVAP</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<EVAP001>%s</EVAP001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_EVAP>%s</IUPR_EVAP>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<GPFZ1Count>1</GPFZ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>GPF组1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<GPFZ1001>%s</GPFZ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_GPFZ1>%s</IUPR_GPFZ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<GPFZ2Count>1</GPFZ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>GPF组2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<GPFZ2001>%s</GPFZ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_GPFZ2>%s</IUPR_GPFZ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<ECKQPSCount>1</ECKQPSCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>二次空气喷射系统</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<ECKQPS001>%s</ECKQPS001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_ECKQPS>%s</IUPR_ECKQPS>", strdata);
				}
			}
			cstrsOBDData.AppendFormat(L"<OBDdata>%s</OBDdata>", cstrOBDdata);
		}

		std::wstring strRet;
		int nRet(99);

		nRet = CJNSXInterfaceLib_API::OBDInspect(pchURL, cstrLineNumber.GetString(), m_sVehDate.strInspectID, cstrsOBDData.GetString(), L"", strRet);

		if (nRet == 0)
		{
			CString strTipMessage, strErrNum, strErrMsg, strInternalErrNum, strInternalErrMsg, strValue, strMemo, cstrRet;
			cstrRet = strRet.c_str();
			AfxExtractSubString(strTipMessage, cstrRet, 0, L'\n');
			AfxExtractSubString(strErrNum, cstrRet, 1, L'\n');
			AfxExtractSubString(strErrMsg, cstrRet, 2, L'\n');
			AfxExtractSubString(strInternalErrNum, cstrRet, 3, L'\n');
			AfxExtractSubString(strInternalErrMsg, cstrRet, 4, L'\n');
			AfxExtractSubString(strValue, cstrRet, 5, L'\n');
			AfxExtractSubString(strMemo, cstrRet, 6, L'\n');

			if (!strTipMessage.IsEmpty())
			{
				MessageBox(strTipMessage);
				m_lbMessage.SetWindowTextW(strTipMessage);
			}

			if (strErrNum == L"0")
			{
				// MessageBox(strValue);
				m_lbMessage.SetWindowTextW(strValue);
			}
			else
			{
				//MessageBox(strErrMsg);
				m_lbMessage.SetWindowTextW(strErrMsg);
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(L"连接错误\r\n返回代号：%d", nRet);
			m_lbMessage.SetWindowTextW(strMsg);
		}
		GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
	}
	else
	{
			m_lbMessage.SetWindowTextW(L"检测线编号为空或\r\n检测人员为空");
	}
}
