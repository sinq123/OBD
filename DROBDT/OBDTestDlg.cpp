// OBDTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DROBDT.h"
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

	//m_vtGasolineItem.push_back(L"失火检测MFT");
	//m_vtGasolineItem.push_back(L"燃油系统检测FST");
	m_vtGasolineItem.push_back(L"催化器CC");
	//m_vtGasolineItem.push_back(L"催化器加热CCH");
	m_vtGasolineItem.push_back(L"氧传器OS");
	m_vtGasolineItem.push_back(L"氧传器加热OSH");
	//m_vtGasolineItem.push_back(L"蒸发排放控制系统EECS");
	m_vtGasolineItem.push_back(L"排气再循环系统EGR");
	//m_vtGasolineItem.push_back(L"二次空气喷射系统SAIS");
	//m_vtGasolineItem.push_back(L"综合部件监控ICM");

	//m_vtDieselItem.push_back(L"供油系统检测FST");
	m_vtDieselItem.push_back(L"排气再循环系统EGR");
	m_vtDieselItem.push_back(L"氧化型催化转化器DOC");
	m_vtDieselItem.push_back(L"选择性催化还原器SCR");
	m_vtDieselItem.push_back(L"颗粒捕集(过滤)器DPF");
	//m_vtDieselItem.push_back(L"氨逃逸催化器ASC");
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
	DDX_Control(pDX, IDC_STATIC_DYN_INFO, m_lbDynInfo);

	DDX_Control(pDX, ID_ED_CALID, m_edCALID);
	DDX_Control(pDX, ID_ED_CVN, m_edCVN);
	DDX_Control(pDX, ID_ED_OBDTYPE, m_edOBDType);
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

	// 写操作提示上方
	m_lbInfo.SetWindowTextW(L"按开始检测后按提示操作");


	m_lbPlateNumber.SetWindowTextW(m_sTestLog.wchPlateNumber);
	m_lbVIN.SetWindowTextW(m_sTestLog.wchVIN);
	CString strLog;
	strLog.Format(L"%s, %s, %s, %s, %s, %s", m_sTestLog.wchRunningNumber, m_sTestLog.wchPlateNumber, m_sTestLog.wchVIN, m_sTestLog.wchFuelType, 
		m_sTestLog.wchDriver, m_bIsOBDRealTime? L"是":L"否");
	CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"记录", L"获取信息", strLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COBDTestDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
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
	m_lbPlateNumber.SetText(m_sTestLog.wchPlateNumber);

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
	CString strFuelType = m_sTestLog.wchFuelType;
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

	//if (m_strRunningNumber == L"")
	{
		GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(SW_HIDE);
	}
	//else
	//{
	//}

	m_lbDynInfo.SetTextColor(RGB(255, 0, 0));
	//m_lbDynInfo.AutoFitFont();
	m_lbDynInfo.SetFontSize(25);
	m_lbDynInfo.SetFontName(L"黑体");
	OperationHint(L"请确认信息，点击开始上传按钮.");
}



void COBDTestDlg::OnBnClickedButtonPrev()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogZoom::OnCancel();
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

	TESTLOG sTestLog;
	SResultOfOBD sResultOfOBD;

	CString strSQL;

	CString strMsg;

	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_sTestLog.wchRunningNumber);
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		strMsg.AppendFormat(L"获取检测日志失败");
		OperationHint(strMsg);
		return;
	}
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_sTestLog.wchRunningNumber);
	if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
	{
		strMsg.AppendFormat(L"获取OBD检测数据失败");
		OperationHint(strMsg);
		return;
	}

	strMsg.Empty();

	if (UPRealTimeData(sTestLog, sResultOfOBD, strMsg))
	{
		OperationHint(L"OBD诊断完毕");
	}
	else
	{
		OperationHint(strMsg);
	}
	SetTestLogAndVehDB(L"4", L"1", strMsg);
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

void COBDTestDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void COBDTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogZoom::OnTimer(nIDEvent);
}

void COBDTestDlg::OperationHint(const CStringW& strOH)
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

void COBDTestDlg::StartItem(void)
{

	std::wstring strResult, wstrMsg;

	// 设定联网配置文件日志
	CNHSQLServerDBO::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	bool bRet(false);
	// 先生成OBD结果

	SResultOfOBD sResultOfOBD;

	sResultOfOBD.strRunningNumber = m_sTestLog.wchRunningNumber;
	sResultOfOBD.strOperator = m_sTestLog.wchDriver;
	sResultOfOBD.strFuelType = m_sTestLog.wchFuelType;
	bRet = SetOBDLOG(sResultOfOBD);

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

	if (!bRet)
	{
		OperationHint(L"生成OBD结果数据失败");
		return;
	}

	// 写入数据库
	SetDboResultOfOBD(sResultOfOBD);
	// 写入临时文件
	SetIniResultOfOBD(sResultOfOBD);

	CString strMsg(L"");

	{
		OperationHint(L"等待5秒");
		Sleep(5000);
		UpOBDReaust(m_sTestLog, sResultOfOBD, strMsg);
		if (strMsg.IsEmpty())
		{
			strMsg.Empty();
			if (m_bIsOBDRealTime)
			{
				SetTestLogAndVehDB(L"0", L"0", strMsg);
				if (strMsg.IsEmpty())
				{
					OperationHint(L"正常检测完成后在点击->过程数据上传");
					GetDlgItem(IDC_BUTTON_FINISH)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_FINISH)->SetWindowTextW(L"过程数据上传");
				}
				else
				{
					OperationHint(strMsg);
				}
			}
			else
			{
				SetTestLogAndVehDB(L"4", L"1", strMsg);
				if (strMsg.IsEmpty())
				{
					OperationHint(L"OBD诊断完毕");
				}
				else
				{
					OperationHint(strMsg);
				}
			}
		}
		else
		{
			OperationHint(strMsg);
		}
	}
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

	CString strFuelType = m_sTestLog.wchFuelType;

	if (strFuelType.Find(L"柴") != -1)
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

void COBDTestDlg::UpOBDReaust(const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	CString strXML;

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	odtCurTime.ParseDateTime(sResultOfOBD.strDetBegTime.c_str());
	// 柴油
	if (sResultOfOBD.strFuelType.find(L"柴") != std::string::npos)
	{
		strXML.Format(L"<Message Device=\"%s\" OutlookID=\"%s\">", theApp.m_strDeviceSN, sTestLog.wchReportNumber);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadDieselResult");
		strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");
		strXML.AppendFormat(L"<Result><Row><Info>");
		strXML.AppendFormat(L"<VIN>%s</VIN>",sTestLog.wchVIN);
		strXML.AppendFormat(L"<XSJYOBDYQ>%s</XSJYOBDYQ>", sResultOfOBD.strOBDType.c_str());
		strXML.AppendFormat(L"<LJXSLC>%s</LJXSLC>", L"0");
		strXML.AppendFormat(L"<FDJKZCALID>%s</FDJKZCALID>", sResultOfOBD.strEngineCALID.c_str());
		strXML.AppendFormat(L"<FDJKZCVN>%s</FDJKZCVN>", sResultOfOBD.strEngineCVN.c_str());
		strXML.AppendFormat(L"<HCLKZCALID>%s</HCLKZCALID>", L"");
		strXML.AppendFormat(L"<HCLKZCVN>%s</HCLKZCVN>", L"");
		strXML.AppendFormat(L"<QTKZCALID>%s</QTKZCALID>", L"");
		strXML.AppendFormat(L"<QTKZCVN>%s</QTKZCVN>", L"");
		strXML.AppendFormat(L"<JCSJ>%s</JCSJ>", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
		strXML.AppendFormat(L"<GZZSQ>%s</GZZSQ>", L"1");
		strXML.AppendFormat(L"<TXQK>%s</TXQK>", L"1");
		strXML.AppendFormat(L"<GZZSQBJ>%s</GZZSQBJ>", L"0");
		strXML.AppendFormat(L"<GZDMGZXX>%s</GZDMGZXX>", L"");
		strXML.AppendFormat(L"<JXZTWWCXM>%s</JXZTWWCXM>", L"0");
		strXML.AppendFormat(L"<MILXSLC>%s</MILXSLC>", L"");
		strXML.AppendFormat(L"<JCJG>%s</JCJG>", L"1");
		strXML.AppendFormat(L"<SFXYFJ>%s</SFXYFJ>", L"0");
		strXML.AppendFormat(L"<FJNR>%s</FJNR>", L"0");
		strXML.AppendFormat(L"<FJJG>%s</FJJG>", L"0");
		strXML.AppendFormat(L"<TXJCJG>%s</TXJCJG>", L"1");
		strXML.AppendFormat(L"<ZTBYZ>%s</ZTBYZ>", L"1");
		strXML.AppendFormat(L"</Info></Row></Result>");

		strXML.AppendFormat(L"<FaultCode><Row><Info>");
		strXML.AppendFormat(L"<DQGZM>%s</DQGZM>", L"");
		strXML.AppendFormat(L"<WJGZM>%s</WJGZM>",L"");
		strXML.AppendFormat(L"</Info></Row></FaultCode>");

		strXML.AppendFormat(L"<Readiness><Row><Info>");
		strXML.AppendFormat(L"<GZMGS>%s</GZMGS>",L"0");
		strXML.AppendFormat(L"<GZDZT>%s</GZDZT>",L"1");
		strXML.AppendFormat(L"<SHJC>%s</SHJC>",L"1");
		strXML.AppendFormat(L"<RYXTJC>%s</RYXTJC>",L"1");
		strXML.AppendFormat(L"<ZHBJJC>%s</ZHBJJC>",L"1");
		strXML.AppendFormat(L"<NMHCCHJJC>%s</NMHCCHJJC>",L"1");
		strXML.AppendFormat(L"<NOHCLJC>%s</NOHCLJC>",L"1");
		strXML.AppendFormat(L"<ZYYLXTJC>%s</ZYYLXTJC>",L"1");
		strXML.AppendFormat(L"<FQCGQJC>%s</FQCGQJC>",L"1");
		strXML.AppendFormat(L"<PMJC>%s</PMJC>",L"1");
		strXML.AppendFormat(L"<EGRHVVTXTJC>%s</EGRHVVTXTJC>",L"1");
		strXML.AppendFormat(L"</Info></Row></Readiness>");

		CString strTemp,strTemp1;
		strXML.AppendFormat(L"<IUPR><Row><Info>");
		strXML.AppendFormat(L"<NMHCWCCS>%d</NMHCWCCS>", _wtoi(sResultOfOBD.strIUPR_NMHCC.c_str()));
		strXML.AppendFormat(L"<NMHCFHCS>%d</NMHCFHCS>", _wtoi(sResultOfOBD.strIUPR_NMHCEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NMHCC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NMHCEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<NMHCIUPRL>%s</NMHCIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<NMHCIUPRL>%.2f</NMHCIUPRL>", ((_wtof(sResultOfOBD.strIUPR_NMHCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NMHCC.c_str()))));
		}
		strXML.AppendFormat(L"<NOXCHQWCCS>%d</NOXCHQWCCS>", _wtoi(sResultOfOBD.strIUPR_NOXCC.c_str()));
		strXML.AppendFormat(L"<NOXCHQFHCS>%d</NOXCHQFHCS>", _wtoi(sResultOfOBD.strIUPR_NOXCEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXCC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXCEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<NOXCHQIUPRL>%s</NOXCHQIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<NOXCHQIUPRL>%.2f</NOXCHQIUPRL>", ((_wtof(sResultOfOBD.strIUPR_NOXCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXCC.c_str()))));
		}
		strXML.AppendFormat(L"<NOXXFQWCCS>%d</NOXXFQWCCS>", _wtoi(sResultOfOBD.strIUPR_NOXAC.c_str()));
		strXML.AppendFormat(L"<NOXXFQFHCS>%d</NOXXFQFHCS>", _wtoi(sResultOfOBD.strIUPR_NOXAEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXAC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXAEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<NOXXFQIUPRL>%s</NOXXFQIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<NOXXFQIUPRL>%.2f</NOXXFQIUPRL>", ( _wtof(sResultOfOBD.strIUPR_NOXAEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXAC.c_str())));
		}
		strXML.AppendFormat(L"<PMWCCS>%d</PMWCCS>", _wtoi(sResultOfOBD.strIUPR_PMC.c_str()));
		strXML.AppendFormat(L"<PMFHCS>%d</PMFHCS>", _wtoi(sResultOfOBD.strIUPR_PMEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PMC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PMEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<PMIUPRL>%s</PMIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<PMIUPRL>%.2f</PMIUPRL>", (_wtof(sResultOfOBD.strIUPR_PMEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PMC.c_str())));
		}
		strXML.AppendFormat(L"<FQCGQWCCS>%d</FQCGQWCCS>", _wtoi(sResultOfOBD.strIUPR_WSC.c_str()));
		strXML.AppendFormat(L"<FQCGQFHCS>%d</FQCGQFHCS>", _wtoi(sResultOfOBD.strIUPR_WSEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_WSC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_WSEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<FQCGQIUPRL>%s</FQCGQIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<FQCGQIUPRL>%.2f</FQCGQIUPRL>", (_wtof(sResultOfOBD.strIUPR_WSEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_WSC.c_str())));
		}
		strXML.AppendFormat(L"<EGRHVVTWCCS>%d</EGRHVVTWCCS>", _wtoi(sResultOfOBD.strIUPR_PPC.c_str()));
		strXML.AppendFormat(L"<EGRHVVTFHCS>%d</EGRHVVTFHCS>", _wtoi(sResultOfOBD.strIUPR_PPEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PPC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PPEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<EGRHVVTIUPRL>%s</EGRHVVTIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<EGRHVVTIUPRL>%.2f</EGRHVVTIUPRL>", (_wtof(sResultOfOBD.strIUPR_PPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PPC.c_str())));
		}
		strXML.AppendFormat(L"<ZYYLWCCS>%d</ZYYLWCCS>", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
		strXML.AppendFormat(L"<ZYYLFHCS>%d</ZYYLFHCS>", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<ZYYLIUPRL>%s</ZYYLIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<ZYYLIUPRL>%.2f</ZYYLIUPRL>", (_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str())));
		}
		strXML.AppendFormat(L"</Info></Row></IUPR>");
	}
	else
	{
		strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", theApp.m_strDeviceSN, sTestLog.wchReportNumber);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadGasResult");
		strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");
		strXML.AppendFormat(L"<Result><Row><Info>");
		strXML.AppendFormat(L"<VIN>%s</VIN>", sTestLog.wchVIN);
		strXML.AppendFormat(L"<XSJYOBDYQ>%s</XSJYOBDYQ>", sResultOfOBD.strOBDType.c_str());
		strXML.AppendFormat(L"<LJXSLC>%s</LJXSLC>", L"0");
		strXML.AppendFormat(L"<FDJKZCALID>%s</FDJKZCALID>", sResultOfOBD.strEngineCALID.c_str());
		strXML.AppendFormat(L"<FDJKZCVN>%s</FDJKZCVN>", sResultOfOBD.strEngineCVN.c_str());
		strXML.AppendFormat(L"<HCLKZCALID>%s</HCLKZCALID>", L"");
		strXML.AppendFormat(L"<HCLKZCVN>%s</HCLKZCVN>", L"");
		strXML.AppendFormat(L"<QTKZCALID>%s</QTKZCALID>", L"");
		strXML.AppendFormat(L"<QTKZCVN>%s</QTKZCVN>", L"");
		strXML.AppendFormat(L"<JCSJ>%s</JCSJ>", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
		strXML.AppendFormat(L"<GZZSQ>%s</GZZSQ>", L"1");
		strXML.AppendFormat(L"<TXQK>%s</TXQK>", L"1");
		strXML.AppendFormat(L"<GZZSQBJ>%s</GZZSQBJ>", L"0");
		strXML.AppendFormat(L"<GZDMGZXX>%s</GZDMGZXX>", L"");
		strXML.AppendFormat(L"<JXZTWWCXM>%s</JXZTWWCXM>", L"0");
		strXML.AppendFormat(L"<MILXSLC>%s</MILXSLC>", L"");
		strXML.AppendFormat(L"<JCJG>%s</JCJG>", L"1");
		strXML.AppendFormat(L"<SFXYFJ>%s</SFXYFJ>", L"0");
		strXML.AppendFormat(L"<FJNR>%s</FJNR>", L"0");
		strXML.AppendFormat(L"<FJJG>%s</FJJG>", L"0");
		strXML.AppendFormat(L"<TXJCJG>%s</TXJCJG>", L"1");
		strXML.AppendFormat(L"<ZTBYZ>%s</ZTBYZ>", L"1");
		strXML.AppendFormat(L"</Info></Row></Result>");

		strXML.AppendFormat(L"<FaultCode><Row><Info>");
		strXML.AppendFormat(L"<DQGZM>%s</DQGZM>", L"");
		strXML.AppendFormat(L"<WJGZM>%s</WJGZM>",L"");
		strXML.AppendFormat(L"</Info></Row></FaultCode>");

		strXML.AppendFormat(L"<Readiness><Row><Info>");
		strXML.AppendFormat(L"<GZMGS>%s</GZMGS>",L"0");
		strXML.AppendFormat(L"<GZDZT>%s</GZDZT>",L"1");
		strXML.AppendFormat(L"<SHJC>%s</SHJC>",L"1");
		strXML.AppendFormat(L"<RYXTJC>%s</RYXTJC>",L"1");
		strXML.AppendFormat(L"<ZHBJJC>%s</ZHBJJC>",L"1");
		strXML.AppendFormat(L"<CHJJC>%s</CHJJC>",L"1");
		strXML.AppendFormat(L"<JRSCHJJC>%s</JRSCHJJC>",L"0");
		strXML.AppendFormat(L"<ZFXTJC>%s</ZFXTJC>",L"0");
		strXML.AppendFormat(L"<ECKQJC>%s</ECKQJC>",L"0");
		strXML.AppendFormat(L"<YCGQJC>%s</YCGQJC>",L"1");
		strXML.AppendFormat(L"<YCGQJRQXTJC>%s</YCGQJRQXTJC>",L"1");
		strXML.AppendFormat(L"<EGRHVVTXTJC>%s</EGRHVVTXTJC>",L"1");
		strXML.AppendFormat(L"</Info></Row></Readiness>");


		CString strTemp, strTemp1;
		strXML.AppendFormat(L"<IUPR><Row><Info>");
		strXML.AppendFormat(L"<CHQZ1WCCS>%d</CHQZ1WCCS>", _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str()));
		strXML.AppendFormat(L"<CHQZ1FHCS>%d</CHQZ1FHCS>", _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<CHQZ1IUPRL>%s</CHQZ1IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<CHQZ1IUPRL>%.2f</CHQZ1IUPRL>", ((_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()))));
		}
		strXML.AppendFormat(L"<CHQZ2WCCS>%d</CHQZ2WCCS>", _wtoi(sResultOfOBD.strIUPR_CMCCB2.c_str()));
		strXML.AppendFormat(L"<CHQZ2FHCS>%d</CHQZ2FHCS>", _wtoi(sResultOfOBD.strIUPR_CMCECB2.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<CHQZ2IUPRL>%s</CHQZ2IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<CHQZ2IUPRL>%.2f</CHQZ2IUPRL>", ((_wtof(sResultOfOBD.strIUPR_CMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB2.c_str()))));
		}
		strXML.AppendFormat(L"<QYCGQZ1WCCS>%d</QYCGQZ1WCCS>", _wtoi(sResultOfOBD.strIUPR_O2SMCCB1.c_str()));
		strXML.AppendFormat(L"<QYCGQZ1FHCS>%d</QYCGQZ1FHCS>", _wtoi(sResultOfOBD.strIUPR_O2SMCECB1.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCCB1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCECB1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<QYCGQZ1IUPRL>%s</QYCGQZ1IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<QYCGQZ1IUPRL>%.2f</QYCGQZ1IUPRL>",  ( _wtof(sResultOfOBD.strIUPR_O2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB1.c_str())));
		}
		strXML.AppendFormat(L"<QYCGQZ2WCCS>%d</QYCGQZ2WCCS>", _wtoi(sResultOfOBD.strIUPR_O2SMCCB2.c_str()));
		strXML.AppendFormat(L"<QYCGQZ2FHCS>%d</QYCGQZ2FHCS>", _wtoi(sResultOfOBD.strIUPR_O2SMCECB2.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<QYCGQZ2IUPRL>%s</QYCGQZ2IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<QYCGQZ2IUPRL>%.2f</QYCGQZ2IUPRL>",  (_wtof(sResultOfOBD.strIUPR_O2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB2.c_str())));
		}
		strXML.AppendFormat(L"<HYCGQZ1WCCS>%d</HYCGQZ1WCCS>", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()));
		strXML.AppendFormat(L"<HYCGQZ1FHCS>%d</HYCGQZ1FHCS>", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<HYCGQZ1IUPRL>%s</HYCGQZ1IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<HYCGQZ1IUPRL>%.2f</HYCGQZ1IUPRL>", (_wtof(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB1.c_str())));
		}
		strXML.AppendFormat(L"<HYCGQZ2WCCS>%d</HYCGQZ2WCCS>", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()));
		strXML.AppendFormat(L"<HYCGQZ2FHCS>%d</HYCGQZ2FHCS>", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<HYCGQZ2IUPRL>%s</HYCGQZ2IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<HYCGQZ2IUPRL>%.2f</HYCGQZ2IUPRL>", (_wtof(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB2.c_str())));
		}
		strXML.AppendFormat(L"<EVAPWCCS>%d</EVAPWCCS>", _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str()));
		strXML.AppendFormat(L"<EVAPFHCS>%d</EVAPFHCS>", _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<EVAPIUPRL>%s</EVAPIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<EVAPIUPRL>%.2f</EVAPIUPRL>", (_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EVAPC.c_str())));
		}
		strXML.AppendFormat(L"<EGRHVVTWCCS>%d</EGRHVVTWCCS>", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
		strXML.AppendFormat(L"<EGRHVVTFHCS>%d</EGRHVVTFHCS>", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<EGRHVVTIUPRL>%s</EGRHVVTIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<EGRHVVTIUPRL>%.2f</EGRHVVTIUPRL>", (_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str())));
		}
		strXML.AppendFormat(L"<GPFZ1WCCS>%d</GPFZ1WCCS>", _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str()));
		strXML.AppendFormat(L"<GPFZ1FHCS>%d</GPFZ1FHCS>", _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<GPFZ1IUPRL>%s</GPFZ1IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<GPFZ1IUPRL>%.2f</GPFZ1IUPRL>", (_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC1.c_str())));
		}
		strXML.AppendFormat(L"<GPFZ2WCCS>%d</GPFZ2WCCS>", _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str()));
		strXML.AppendFormat(L"<GPFZ2FHCS>%d</GPFZ2FHCS>", _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<GPFZ2IUPRL>%s</GPFZ2IUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<GPFZ2IUPRL>%.2f</GPFZ2IUPRL>",  (_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC2.c_str())));
		}
		strXML.AppendFormat(L"<ECKQPSWCCS>%d</ECKQPSWCCS>", _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str()));
		strXML.AppendFormat(L"<ECKQPSFHCS>%d</ECKQPSFHCS>", _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str()));
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{
			strXML.AppendFormat(L"<ECKQPSIUPRL>%s</ECKQPSIUPRL>",L"0.00");
		}
		else
		{
			strXML.AppendFormat(L"<ECKQPSIUPRL>%.2f</ECKQPSIUPRL>", (_wtof(sResultOfOBD.strIUPR_AMCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_AMCCC.c_str())));
		}
		strXML.AppendFormat(L"</Info></Row></IUPR>");
	}

	strXML.AppendFormat(L"</Request></Message>");
	strXML.Replace(L"不支持",L"0");

	std::wstring strRecv;
#ifndef _DEBUG
	bool bRet = theApp.SendAndRecvPacket(strXML, strRecv);
	if (bRet)
#else
	if (true)
#endif
	{
		CXmlReader xmlReader;
#ifndef _DEBUG
		if (xmlReader.Parse(strRecv.c_str()))
#else
		if (xmlReader.LoadFile(L"F:\\程序\\OBD\\OBD\\DROBDT\\OBDUploadGasResult.xml"))
#endif
		{
			std::wstring strResult, strMessage;

			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMessage);
			}
			if (strResult == L"1")
			{
				strMsg.Empty();
			}
			else
			{
				OperationHint(L"OBD结果上传失败");
				CString strTemp;
				strTemp.Format(L"%s->%s->%s", L"结果上传", strResult.c_str(), strMessage.c_str());
				strMsg.AppendFormat(L"%s", strTemp);
			}
		}
		else
		{
			OperationHint(L"解析失败");
			CString strTemp;
			strTemp.Format(L"%s->%s", L"解析失败", strRecv.c_str());
			strMsg.AppendFormat(L"%s", strTemp);
		}
	}
	else
	{
		OperationHint(L"联网失败");
		CString strTemp;
		strTemp.Format(L"%s->%s", L"获取联网失败", strRecv.c_str());
		strMsg.AppendFormat(L"%s", strTemp);
	}
}


int COBDTestDlg::randEx()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}

bool COBDTestDlg::UPRealTimeData(const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	CString strTestType(sTestLog.wchTestType);
	COleDateTime odtCurTime;
	COleDateTimeSpan odts;
	SYSTEMTIME st;

	CString strXML;
	// 汽油
	if (strTestType.Find(L"1") != -1 || strTestType.Find(L"2") != -1 || strTestType.Find(L"4") != -1)
	{
		strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", theApp.m_strDeviceSN, sTestLog.wchReportNumber);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadGasProcessData");
	}
	// 柴油
	else
	{
		strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", theApp.m_strDeviceSN, sTestLog.wchReportNumber);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadDieselProcessData");
	}
	strXML.AppendFormat(L"<SSSJL>");

	float f;
	int n;

	// 稳态
	if (strTestType.Find(L"1") != -1)
	{
		std::vector<SRealTimeDataOfASM> vt;
		GetIniRealTimeDataOfASMEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = n + 32.94f;
			}
			else
			{
				f = n /10.0f;
				f = 32.94f + f;
			}

			// 实时数据留父节点，每个Row节点下有一个。
			strXML.AppendFormat(L"<Row><REALTEMEDATA>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>节气门绝对开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 60.00f - n;
			}
			else
			{
				f = n /10.0f;
				f = 60.00f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>计算负荷值</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>2</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 30.00 - n;
			}
			else
			{
				f = n /10.0f;
				f = 30.00 + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>前氧传感器信号或过量空气系数</SSSJLNR>");
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			strXML.AppendFormat(L"<SSSJLDW>mV/mA</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8988</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>车速</SSSJLNR>");
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				if (i%2 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
			}
			strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机转速</SSSJLNR>");
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				if (i%2 == 0){n = randEx()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
				strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", n);
			}
			else
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				if (i%2 == 0){n = randEx()%300 + 650;}
				else if (i%3 == 0){n = rand()%300 + 678;}
				else if (i%5 == 0){n = rand()%300 + 687;}
				else {n = rand()%300 + 640;}
				strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", n);
			}
			strXML.AppendFormat(L"<SSSJLDW>r/min</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%4 + 1;
			if (n%2 == 0)
			{
				f = 2.87f + n;
			}
			else
			{
				f = n /10.0f;
				f = 2.87f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>进气量或进气压力</SSSJLNR>");
			if (vt[i].sOBDRTData.strMAF.empty())
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>",f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			strXML.AppendFormat(L"<SSSJLDW>g/s</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>5</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"</REALTEMEDATA>");

			// 故障码内容父节点。
			strXML.AppendFormat(L"<FAULTCODE><Info>");
			strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
			strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
			strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
			strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
		}
	}
	// 简易瞬态
	else if (strTestType.Find(L"2") != -1)
	{
		std::vector<SRealTimeDataOfVMAS> vt;
		GetIniRealTimeDataOfVMASEx(vt);
		for(int i=0; i<vt.size(); i++)
		{

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = n + 32.94f;
			}
			else
			{
				f = n /10.0f;
				f = 32.94f + f;
			}

			// 实时数据留父节点，每个Row节点下有一个。
			strXML.AppendFormat(L"<Row><REALTEMEDATA>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>节气门绝对开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 60.00f - n;
			}
			else
			{
				f = n /10.0f;
				f = 60.00f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>计算负荷值</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>2</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 30.00 - n;
			}
			else
			{
				f = n /10.0f;
				f = 30.00 + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>前氧传感器信号或过量空气系数</SSSJLNR>");
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			strXML.AppendFormat(L"<SSSJLDW>mV/mA</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8988</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>车速</SSSJLNR>");
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
			}
			strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机转速</SSSJLNR>");
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0){n = randEx()%300 + 1752;}
				else if (i%3 == 0){n = randEx()%300 + 1825;}
				else if (i%5 == 0){n = randEx()%300 + 1872;}
				else {n = randEx()%300 + 1852;}
				strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", n);
			}
			else
			{
				if (i%2 == 0){n = randEx()%300 + 650;}
				else if (i%3 == 0){n = randEx()%300 + 678;}
				else if (i%5 == 0){n = randEx()%300 + 687;}
				else {n = randEx()%300 + 640;}
				strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", n);
			}
			strXML.AppendFormat(L"<SSSJLDW>r/min</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%4 + 1;
			if (n%2 == 0)
			{
				f = 2.87f + n;
			}
			else
			{
				f = n /10.0f;
				f = 2.87f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>进气量或进气压力</SSSJLNR>");
			if (vt[i].sOBDRTData.strMAF.empty())
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			strXML.AppendFormat(L"<SSSJLDW>g/s</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>5</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"</REALTEMEDATA>");

			// 故障码内容父节点。
			strXML.AppendFormat(L"<FAULTCODE><Info>");
			strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
			strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
			strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
			strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
		}
	}
	// 加载减速
	else if (strTestType.Find(L"3") != -1)
	{

		std::vector<SRealTimeDataOfLUGDOWN> vt;
		GetIniRealTimeDataOfLUGDOWNEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			// 实时数据留父节点，每个Row节点下有一个。
			strXML.AppendFormat(L"<Row><REALTEMEDATA>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = n + 32.94f;
			}
			else
			{
				f = n /10.0f;
				f = 32.94f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>油门开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>车速</SSSJLNR>");
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
			}
			strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机输出功率</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>kw</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11001</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机转速</SSSJLNR>");
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0){n = randEx()%300 + 1752;}
				else if (i%3 == 0){n = randEx()%300 + 1825;}
				else if (i%5 == 0){n = randEx()%300 + 1872;}
				else {n = randEx()%300 + 1852;}
				strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", n);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", 0);
			}
			strXML.AppendFormat(L"<SSSJLDW>r/min</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%4 + 1;
			if (n%2 == 0)
			{
				f = 2.87f + n;
			}
			else
			{
				f = n /10.0f;
				f = 2.87f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>进气量</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>g/s</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>1001</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 72.00f - n;
			}
			else
			{
				f = n /10.0f;
				f = 72.00f + f;
			}
			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>增压压力</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>",f);
			strXML.AppendFormat(L"<SSSJLDW>kPa</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7008</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>耗油量</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>L/100km</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11002</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>氮氧传感器浓度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>ppm</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8305</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>尿素喷射量</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>L/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8505</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>排气温度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>°C</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7805</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>颗粒捕集器压差</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>kpa</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7933</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>EGR开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6908</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>>燃油喷射压力</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>bar</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6975</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"</REALTEMEDATA>");

			// 故障码内容父节点。
			strXML.AppendFormat(L"<FAULTCODE><Info>");
			strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
			strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
			strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
			strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
		}
	}
	// 双怠速
	else if (strTestType.Find(L"4") != -1)
	{
		std::vector<SRealTimeDataOfDIS> vt;
		GetIniRealTimeDataOfDISEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			// 实时数据留父节点，每个Row节点下有一个。
			strXML.AppendFormat(L"<Row><REALTEMEDATA>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = n + 32.94f;
			}
			else
			{
				f = n /10.0f;
				f = 32.94f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>节气门绝对开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 60.00f - n;
			}
			else
			{
				f = n /10.0f;
				f = 60.00f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>计算负荷值</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>2</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 30.00 - n;
			}
			else
			{
				f = n /10.0f;
				f = 30.00 + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>前氧传感器信号或过量空气系数</SSSJLNR>");
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			strXML.AppendFormat(L"<SSSJLDW>mV/mA</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8988</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>车速</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
			strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机转速</SSSJLNR>");
			if (i%2 == 0){n = randEx()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", n);
			strXML.AppendFormat(L"<SSSJLDW>r/min</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%4 + 1;
			if (n%2 == 0)
			{
				f = 2.87f + n;
			}
			else
			{
				f = n /10.0f;
				f = 2.87f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>进气量或进气压力</SSSJLNR>");
			if (vt[i].sOBDRTData.strMAF.empty())
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			else
			{
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			}
			strXML.AppendFormat(L"<SSSJLDW>g/s</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>5</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"</REALTEMEDATA>");

			// 故障码内容父节点。
			strXML.AppendFormat(L"<FAULTCODE><Info>");
			strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
			strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
			strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
			strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
		}
	}
	// 不透光
	else if (strTestType.Find(L"5") != -1)
	{
		std::vector<SRealTimeDataOfFSUNHT> vt;
		GetIniRealTimeDataOfFSUNHTEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			// 实时数据留父节点，每个Row节点下有一个。
			strXML.AppendFormat(L"<Row><REALTEMEDATA>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = n + 32.94f;
			}
			else
			{
				f = n /10.0f;
				f = 32.94f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>油门开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>车速</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
			strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机输出功率</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>kw</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11001</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>发动机转速</SSSJLNR>");

			if (i%2 == 0){n = randEx()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strXML.AppendFormat(L"<SSSJLJG>%d</SSSJLJG>", 0);
			strXML.AppendFormat(L"<SSSJLDW>r/min</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%4 + 1;
			if (n%2 == 0)
			{
				f = 2.87f + n;
			}
			else
			{
				f = n /10.0f;
				f = 2.87f + f;
			}
			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>进气量</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>g/s</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>1001</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			n = randEx()%10 + 1;
			if (n%2 == 0)
			{
				f = 72.00f - n;
			}
			else
			{
				f = n /10.0f;
				f = 72.00f + f;
			}

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>增压压力</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
			strXML.AppendFormat(L"<SSSJLDW>kPa</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7008</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>耗油量</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>L/100km</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>11002</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>氮氧传感器浓度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>ppm</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8305</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>尿素喷射量</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>L/h</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>8505</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>排气温度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>°C</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7805</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>颗粒捕集器压差</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>kpa</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>7933</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>EGR开度</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6908</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"<Info>");
			strXML.AppendFormat(L"<SSSJLNR>>燃油喷射压力</SSSJLNR>");
			strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strXML.AppendFormat(L"<SSSJLDW>bar</SSSJLDW>");
			strXML.AppendFormat(L"<SSSJLID>6975</SSSJLID>");
			strXML.AppendFormat(L"</Info>");

			strXML.AppendFormat(L"</REALTEMEDATA>");

			// 故障码内容父节点。
			strXML.AppendFormat(L"<FAULTCODE><Info>");
			strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
			strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
			strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
			strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
		}
	}
	// 滤纸式
	else if (strTestType.Find(L"6") != -1)
	{
	}
	strXML.AppendFormat(L"</SSSJL></Request></Message>");

	std::wstring strRecv;
	bool bRet = theApp.SendAndRecvPacket(strXML, strRecv);

	if (bRet)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			std::wstring strResult, strMessage;

			if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
			{
				xmlReader.GetNodeContent(strResult);
			}
			if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
			{
				xmlReader.GetNodeContent(strMessage);
			}
			if (strResult == L"1")
			{
				strMsg.Empty();
				return true;
			}
			else
			{
				OperationHint(L"OBD过程上传失败");
				CString strTemp;
				strTemp.Format(L"%s->%s->%s", L"过程上传", strResult.c_str(), strMessage.c_str());
				strMsg.AppendFormat(L"%s", strTemp);
				return false;
			}
		}
		else
		{
			OperationHint(L"解析失败");
			CString strTemp;
			strTemp.Format(L"%s->%s", L"解析失败", strRecv.c_str());
			strMsg.AppendFormat(L"%s", strTemp);
			return false;
		}
	}
	else
	{
		OperationHint(L"联网失败");
		CString strTemp;
		strTemp.Format(L"%s->%s", L"获取联网失败", strRecv.c_str());
		strMsg.AppendFormat(L"%s", strTemp);
		return false;
	}

	return true;
}

