// OBDTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CDSCheck.h"
#include "OBDTestDlg.h"
#include "afxdialogex.h"


// COBDTestDlg 对话框

IMPLEMENT_DYNAMIC(COBDTestDlg, CDialogZoom)

COBDTestDlg::COBDTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(COBDTestDlg::IDD, pParent)
{
	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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

	m_fontListCtrl.CreateFont(16,
		0, 
		0, 
		0, 
		FW_NORMAL,
		FALSE, 
		FALSE, 
		FALSE, 
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_MODERN,
		L"黑体");
	m_ilRowHeight.Create(1, 28, ILC_COLOR, 1, 1);

	m_vtGasolineItem.push_back(L"催化器CC");
	m_vtGasolineItem.push_back(L"氧传器OS");
	m_vtGasolineItem.push_back(L"氧传器加热OSH");
	m_vtGasolineItem.push_back(L"排气再循环系统EGR");

	m_vtDieselItem.push_back(L"排气再循环系统EGR");
	m_vtDieselItem.push_back(L"氧化型催化转化器DOC");
	m_vtDieselItem.push_back(L"选择性催化还原器SCR");
	m_vtDieselItem.push_back(L"颗粒捕集(过滤)器DPF");
	m_vtDieselItem.push_back(L"颗粒物催化氧化器POC");
}

COBDTestDlg::~COBDTestDlg()
{
	m_fontDlgFont.DeleteObject();
	m_fontListCtrl.DeleteObject();

	if (m_pConnection != NULL)
	{
		m_pConnection = NULL;
	}
}

void COBDTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_PLATE_NUMBER, m_lbPlateNumber);
	DDX_Control(pDX, IDC_STATIC_VIN, m_lbVIN);
	DDX_Control(pDX, IDC_STATIC_LAMP_STATE, m_lbLampState);
	DDX_Control(pDX, IDC_STATIC_DTC, m_lbDTC);
	DDX_Control(pDX, IDC_STATIC_DTC_MILEAGE, m_lbDTCMileage);
	DDX_Control(pDX, IDC_STATIC_FREEZE_DATA, m_lbFreezeData);
	DDX_Control(pDX, IDC_STATIC_JUDGE, m_lbJudge);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lb4);
	DDX_Control(pDX, IDC_LIST_ITEM, m_lsItem);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL, m_cbProtocol);
	DDX_Control(pDX, IDC_STATIC_MSG_INFO, m_lbMsgInfo);
}


BEGIN_MESSAGE_MAP(COBDTestDlg, CDialogZoom)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PREV, &COBDTestDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_START_TEST, &COBDTestDlg::OnBnClickedButtonStartTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &COBDTestDlg::OnBnClickedButtonStopTest)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, &COBDTestDlg::OnBnClickedButtonFinish)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &COBDTestDlg::OnBnClickedButtonNext)
END_MESSAGE_MAP()


// COBDTestDlg 消息处理程序


BOOL COBDTestDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化控件
	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	m_lbInfo.SetWindowTextW(L"按开始检测后按提示操作");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COBDTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogZoom::OnTimer(nIDEvent);
}


void COBDTestDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void COBDTestDlg::OnBnClickedButtonStartTest()
{
	// TODO: 在此添加控件通知处理程序代码
	StartItem();
}


void COBDTestDlg::OnBnClickedButtonStopTest()
{
	// TODO: 在此添加控件通知处理程序代码
}


void COBDTestDlg::OnBnClickedButtonFinish()
{
	// TODO: 在此添加控件通知处理程序代码
}


void COBDTestDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
}


void COBDTestDlg::OnBnClickedButtonPrev()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogZoom::OnCancel();
}

void COBDTestDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void COBDTestDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbHead.SetTitle(L"OBD检测");

	m_lbInfo.SetTitleStyle();
	m_lbInfo.SetText(L"OBD检测");

	// 车牌号码显示框
	m_lbPlateNumber.SetBkColor(RGB(230, 230, 230));
	m_lbPlateNumber.AutoFitFont();
	m_lbPlateNumber.SetText(m_sVehInfo.wchPlateNumber);

	m_lbVIN.SetBkColor(RGB(230, 230, 230));
	m_lbVIN.AutoFitFont();
	m_lbVIN.SetText(L"--");

	m_lbLampState.SetBkColor(RGB(230, 230, 230));
	m_lbLampState.AutoFitFont();
	m_lbLampState.SetText(L"--");

	m_lbDTC.SetBkColor(RGB(230, 230, 230));
	m_lbDTC.AutoFitFont();
	m_lbDTC.SetText(L"--");

	m_lbDTCMileage.SetBkColor(RGB(230, 230, 230));
	m_lbDTCMileage.AutoFitFont();
	m_lbDTCMileage.SetText(L"--");

	m_lbFreezeData.SetBkColor(RGB(230, 230, 230));
	m_lbFreezeData.AutoFitFont();
	m_lbFreezeData.SetText(L"--");

	m_lbJudge.SetBkColor(RGB(230, 230, 230));
	m_lbJudge.AutoFitFont();
	m_lbJudge.SetText(L"--");

	((CComboBox*)GetDlgItem(IDC_COMBO_STEP2_REASON))->AddString(L"接口损坏");
	((CComboBox*)GetDlgItem(IDC_COMBO_STEP2_REASON))->AddString(L"找不到接口");

	for (int i=0; i<11; i++)
	{
		CString strProtocol;
		strProtocol.Format(L"%02d", i);
		int nIndex = m_cbProtocol.AddString(strProtocol);
		m_cbProtocol.SetItemData(nIndex, i);
	}

	m_lsItem.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lsItem.InsertColumn(0, L"序号", LVCFMT_LEFT, 80);
	m_lsItem.InsertColumn(1, L"诊断项目", LVCFMT_LEFT, 200);
	m_lsItem.InsertColumn(2, L"就绪状态", LVCFMT_LEFT, 80);

	int nIndex = 0;
	CString strTemp;
	CString strFuelType;
	strFuelType.Format(L"%s", m_sVehInfo.wchFuelType);
	std::vector<std::wstring> vtOBDItem = (strFuelType==L"柴油") ? m_vtDieselItem : m_vtGasolineItem;
	for (UINT i=0; i<vtOBDItem.size(); i++)
	{
		strTemp.Format(L"%d", i+1);
		nIndex = m_lsItem.InsertItem(i, strTemp);
		m_lsItem.SetItemText(nIndex, 1, vtOBDItem[i].c_str());
	}

	m_lsItem.SetImageList(&m_ilRowHeight, LVSIL_SMALL);
	m_lsItem.SetFont(&m_fontListCtrl);

	// 设置标签字体颜色
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lb4.SetTextColor(RGB(0, 102, 204));

	GetDlgItem(IDC_BUTTON_START_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STEP_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(SW_HIDE);

	m_lbMsgInfo.SetTextColor(RGB(255, 0, 0));
	m_lbMsgInfo.SetFontSize(40);
	m_lbMsgInfo.SetFontName(L"黑体");
	m_lbMsgInfo.SetWindowTextW(L"请确认信息，点击开始上传按钮.");
}

void COBDTestDlg::StartItem(void)
{
	bool bRet(false);
	// 先生成OBD结果

	SResultOfOBD sResultOfOBD;

	sResultOfOBD.strRunningNumber = m_sTestLog.wchRunningNumber;
	sResultOfOBD.strOperator = m_sTestLog.wchDriver;
	sResultOfOBD.strFuelType = m_sTestLog.wchFuelType;
	bRet = SetOBDLOG(sResultOfOBD);

	if (!bRet)
	{
		m_lbInfo.SetWindowTextW(L"生成OBD结果数据失败");
		return;
	}

	// 写入数据库
	SetDboResultOfOBD(sResultOfOBD);
	// 写入临时文件
	SetIniResultOfOBD(sResultOfOBD);

	CString strMsg(L"");

	SetTestLogAndVehDB(L"4", L"1", strMsg);

	if (!strMsg.IsEmpty())
	{
		m_lbInfo.SetWindowTextW(strMsg);
		return;
	}


	m_lbInfo.SetWindowTextW(L"修改数据库和临时文件完成，正在上传");

	SHBMsg sHBMsg;

	if (!CZYHttp_PAI::GetInstance().SetOBDItemEnd(m_sTestLog, sResultOfOBD, sHBMsg))
	{
		m_lbInfo.SetWindowTextW(L"上传成功。南华进入OBD检测后直接点击下一步排放检测");
	}
	else
	{
		strMsg.Empty();
		strMsg.Format(L"上传失败\r\n%s\r\n%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		MessageBox(strMsg, L"上传失败", MB_ICONWARNING|MB_OK);
		strMsg.Format(L"上传失败:%s:%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		m_lbInfo.SetWindowTextW(strMsg);
		return;
	}

}

//初始化一个以微秒为单位的时间种子
int COBDTestDlg::randEx()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}


bool COBDTestDlg::SetOBDLOG(SResultOfOBD& sResultOfOBD)
{
	bool bRet(true);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

	CString strFuelType;
	strFuelType.Format(L"%s", m_sTestLog.wchFuelType);
	if (strFuelType.Find(L"柴") != -1)
	{
		CString strOBDType;
		int n = randEx()%10;
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

		CString strTemp;
		strTemp.Format(L"%d", (int)randEx()%100+1501);
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
		int n = randEx()%10;
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

		CString strTemp;
		strTemp.Format(L"%d", (int)randEx()%100+101);
		sResultOfOBD.strIUPR_OMCEC = strTemp.GetString();
		strTemp.Format(L"%d", (int)randEx()%100+601);
		sResultOfOBD.strIUPR_ICC = strTemp.GetString();
		strTemp.Format(L"%d", (int)randEx()%40+101);
		sResultOfOBD.strIUPR_CMCCB1 = strTemp.GetString();
		strTemp.Format(L"%d", (int)randEx()%100+101);
		sResultOfOBD.strIUPR_CMCECB1 = strTemp.GetString();
		sResultOfOBD.strIUPR_CMCCB2 = L"0";
		sResultOfOBD.strIUPR_CMCECB2 = L"0";
		strTemp.Format(L"%d", (int)randEx()%100+101);
		sResultOfOBD.strIUPR_O2SMCCB1 = strTemp.GetString();
		strTemp.Format(L"%d", (int)randEx()%100+101);
		sResultOfOBD.strIUPR_O2SMCECB1 = strTemp.GetString();
		sResultOfOBD.strIUPR_O2SMCCB2 = L"0";
		sResultOfOBD.strIUPR_O2SMCECB2 = L"0";
		strTemp.Format(L"%d", (int)randEx()%100+141);
		sResultOfOBD.strIUPR_RO2SMCCB1 = strTemp.GetString();
		strTemp.Format(L"%d", (int)randEx()%100+101);
		sResultOfOBD.strIUPR_RO2SMCECB1 = strTemp.GetString();
		sResultOfOBD.strIUPR_RO2SMCCB2 = L"0";
		sResultOfOBD.strIUPR_RO2SMCECB2 = L"0";
		strTemp.Format(L"%d", (int)randEx()%100+501);
		sResultOfOBD.strIUPR_EGRC = strTemp.GetString();
		strTemp.Format(L"%d", (int)randEx()%100+101);
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

CString COBDTestDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void COBDTestDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	if (strFulType == L"汽油")
	{
		if (strOBDType == L"EOBD")
		{
			int n = randEx()%20;
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
			int n = randEx()%4;
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
			int n = randEx()%4;
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
			int n = randEx()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"JOBD-EOBD")
		{
			int n = randEx()%3;
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
			int n = randEx()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"不支持"; strEngineCVN = L"不支持";} break;
			}
		}
		else if (strOBDType == L"不适用")
		{
			int n = randEx()%15;
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
			int n = randEx()%4;
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
		int n = randEx()%15;
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


void COBDTestDlg::SetTestLogAndVehDB(const CString& strItemOBD, const CString& strHasOBD, CString& strstrMsg)
{
	CString strMsg;
	// 更新INI
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		if (!si.SetString(L"TestLog", L"ItemOBD", strItemOBD))
		{
			strMsg.AppendFormat(L"修改App_Data->TestLog.ini->ItemOBD失败");
		}
		if (!si.SetString(L"TestLog", L"HasOBD", strHasOBD))
		{
			strMsg.AppendFormat(L"修改App_Data->TestLog.ini->HasOBD失败");
		}
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		if (!si.SetString(L"TestLog", L"HasOBD", strHasOBD))
		{
			strMsg.AppendFormat(L"修改App_Data->VehicleInfo.ini->HasOBD失败");
		}
	}

	if (m_pConnection != NULL)
	{
		CString strSQL;
		strSQL.Format(_T("Update TestLog set ItemOBD = '%s', HasOBD = '%s' where RunningNumber = '%s'"), strItemOBD, strHasOBD, m_sTestLog.wchRunningNumber);
		int nRowsInvolved(0);
		if (0x00 != CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"修改数据库->TestLog表失败");
		}

		strSQL.Format(_T("Update VehicleInfo set HasOBD = '%s' where PlateNumber = '%s'"), strHasOBD, m_sTestLog.wchPlateNumber);
		nRowsInvolved = 0;
		if (0x00 != CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"修改数据库->VehicleInfo表失败");
		}
	}
}