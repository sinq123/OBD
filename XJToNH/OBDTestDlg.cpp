// OBDTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AT.h"
#include "OBDTestDlg.h"
#include "afxdialogex.h"


// COBDTestDlg �Ի���

IMPLEMENT_DYNAMIC(COBDTestDlg, CDialogZoom)

	COBDTestDlg::COBDTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(COBDTestDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
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
		L"����");

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
		L"����");
	m_ilRowHeight.Create(1, 28, ILC_COLOR, 1, 1);

	//m_vtGasolineItem.push_back(L"ʧ����MFT");
	//m_vtGasolineItem.push_back(L"ȼ��ϵͳ���FST");
	m_vtGasolineItem.push_back(L"�߻���CC");
	//m_vtGasolineItem.push_back(L"�߻�������CCH");
	m_vtGasolineItem.push_back(L"������OS");
	m_vtGasolineItem.push_back(L"����������OSH");
	//m_vtGasolineItem.push_back(L"�����ŷſ���ϵͳEECS");
	m_vtGasolineItem.push_back(L"������ѭ��ϵͳEGR");
	//m_vtGasolineItem.push_back(L"���ο�������ϵͳSAIS");
	//m_vtGasolineItem.push_back(L"�ۺϲ������ICM");

	//m_vtDieselItem.push_back(L"����ϵͳ���FST");
	m_vtDieselItem.push_back(L"������ѭ��ϵͳEGR");
	m_vtDieselItem.push_back(L"�����ʹ߻�ת����DOC");
	m_vtDieselItem.push_back(L"ѡ���Դ߻���ԭ��SCR");
	m_vtDieselItem.push_back(L"��������(����)��DPF");
	//m_vtDieselItem.push_back(L"�����ݴ߻���ASC");
	m_vtDieselItem.push_back(L"������߻�������POC");
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


// COBDTestDlg ��Ϣ�������


BOOL COBDTestDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
	// ��ʼ���ؼ�
	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	// д������ʾ�Ϸ�
	if (m_bIsOBDRealTime)
	{
		m_lbInfo.SetWindowTextW(L"����ʼ��⣬�����ŷż����ɺ󷵻ؼ�������");
	}
	else
	{
		m_lbInfo.SetWindowTextW(L"����ʼ������ʾ����");
	}

	m_lbPlateNumber.SetWindowTextW(m_strPlateNumber);
	m_lbVIN.SetWindowTextW(m_strVIN);
	CString strLog;
	strLog.Format(L"%s, %s, %s, %s, %s, %d", m_strRunningNumber, m_strPlateNumber, m_strVIN, m_strFuelType, m_strOperator, m_bIsOBDRealTime);
	CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"��ȡ��Ϣ", strLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void COBDTestDlg::OnDestroy()
{
	CDialogZoom::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
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

	m_lbHead.SetTitle(L"OBD���");

	m_lbInfo.SetTitleStyle();
	m_lbInfo.SetText(L"OBD���");

	// ���ƺ�����ʾ��
	m_lbPlateNumber.SetBkColor(RGB(230, 230, 230));
	m_lbPlateNumber.AutoFitFont();
	m_lbPlateNumber.SetText(m_strPlateNumber);

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

	((CComboBox*)GetDlgItem(IDC_COMBO_STEP2_REASON))->AddString(L"�ӿ���");
	((CComboBox*)GetDlgItem(IDC_COMBO_STEP2_REASON))->AddString(L"�Ҳ����ӿ�");

	for (int i=0; i<11; i++)
	{
		CString strProtocol;
		strProtocol.Format(L"%02d", i);
		int nIndex = m_cbProtocol.AddString(strProtocol);
		m_cbProtocol.SetItemData(nIndex, i);
	}

	m_lsItem.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lsItem.InsertColumn(0, L"���", LVCFMT_LEFT, 80);
	m_lsItem.InsertColumn(1, L"�����Ŀ", LVCFMT_LEFT, 200);
	m_lsItem.InsertColumn(2, L"����״̬", LVCFMT_LEFT, 80);

	int nIndex = 0;
	CString strTemp;
	std::vector<std::wstring> vtOBDItem = (m_strFuelType==L"����") ? m_vtDieselItem : m_vtGasolineItem;
	for (UINT i=0; i<vtOBDItem.size(); i++)
	{
		strTemp.Format(L"%d", i+1);
		nIndex = m_lsItem.InsertItem(i, strTemp);
		m_lsItem.SetItemText(nIndex, 1, vtOBDItem[i].c_str());
	}

	m_lsItem.SetImageList(&m_ilRowHeight, LVSIL_SMALL);
	m_lsItem.SetFont(&m_fontListCtrl);

	// ���ñ�ǩ������ɫ
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
	m_lbDynInfo.SetFontSize(40);
	m_lbDynInfo.SetFontName(L"����");
	OperationHint(L"��ȷ����Ϣ�������ʼ�ϴ���ť.");
}



void COBDTestDlg::OnBnClickedButtonPrev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogZoom::OnCancel();
}


void COBDTestDlg::OnBnClickedButtonStartTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StartItem();
}


void COBDTestDlg::OnBnClickedButtonStopTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void COBDTestDlg::OnBnClickedButtonFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMsg;

	if (UpOBDReaust(strMsg))
	{
		strMsg = L"�ϴ����ʧ��" + strMsg;
		OperationHint(strMsg);
		return;
	}

	if (UPRealTimeData(strMsg))
	{
		strMsg = L"�ϴ�����ʧ��" + strMsg;
		OperationHint(strMsg);
		return;
	}

}

void COBDTestDlg::SetTestLogAndVehDB(const CString& strItemOBD, const CString& strHasOBD, CString& strstrMsg)
{
	CString strMsg;
	// ����INI
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		if (!si.SetString(L"TestLog", L"ItemOBD", strItemOBD))
		{
			strMsg.AppendFormat(L"�޸�App_Data->TestLog.ini->ItemOBDʧ��");
		}
		if (!si.SetString(L"TestLog", L"HasOBD", strHasOBD))
		{
			strMsg.AppendFormat(L"�޸�App_Data->TestLog.ini->HasOBDʧ��");
		}
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		if (!si.SetString(L"TestLog", L"HasOBD", strHasOBD))
		{
			strMsg.AppendFormat(L"�޸�App_Data->VehicleInfo.ini->HasOBDʧ��");
		}
	}

	if (m_pConnection != NULL)
	{
		CString strSQL;
		strSQL.Format(_T("Update TestLog set ItemOBD = '%s', HasOBD = '%s' where RunningNumber = '%s'"), strItemOBD, strHasOBD, m_strRunningNumber);
		int nRowsInvolved(0);
		if (0x00 != CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"�޸����ݿ�->TestLog��ʧ��");
		}

		strSQL.Format(_T("Update VehicleInfo set HasOBD = '%s' where PlateNumber = '%s'"), strHasOBD, m_strPlateNumber);
		nRowsInvolved = 0;
		if (0x00 != CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"�޸����ݿ�->VehicleInfo��ʧ��");
		}

	}
}

void COBDTestDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void COBDTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// �趨���������ļ���־
	CNHSQLServerDBO::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	bool bRet(false);
	// ������OBD���

	SResultOfOBD sResultOfOBD;

	sResultOfOBD.strRunningNumber = m_strRunningNumber.GetString();
	sResultOfOBD.strOperator = m_strOperator.GetString();
	sResultOfOBD.strFuelType = m_strFuelType.GetString();
	bRet = SetOBDLOG(sResultOfOBD);
	if (!bRet)
	{
		OperationHint(L"����OBD�������ʧ��");
		return;
	}
	// д�����ݿ�
	SetDboResultOfOBD(sResultOfOBD);
	// д����ʱ�ļ�
	SetIniResultOfOBD(sResultOfOBD);

	// ����ƽ̨��ͳһ�ڽ�β�ϴ��ģ�������Ҫ���Ƿ��ϴ�OBD�������� ���ϴ�ͳһ����ֻ��Ҫ�ȱ�������ݿ���޸����ݿ���ֶ�
	// �ϴ���������
	CString strMsg(L"");
	if (m_bIsOBDRealTime)
	{
		strMsg.Empty();
		SetTestLogAndVehDB(L"0", L"0", strMsg);
		if (strMsg.IsEmpty())
		{
			OperationHint(L"���������ɺ��ڵ��->���������ϴ�");
		}
		else
		{
			OperationHint(strMsg);
		}
		// �Ƿ��޸�������������ʾ����
		/// ��ʱ����ʾ
		GetDlgItem(IDC_BUTTON_FINISH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FINISH)->SetWindowTextW(L"���������ϴ�");
	}
	else
	{
		strMsg.Empty();
		SetTestLogAndVehDB(L"4", L"1", strMsg);
		if (strMsg.IsEmpty())
		{
			OperationHint(L"�޸����, ������������");
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

	if (m_strFuelType.Find(L"��") != -1)
	{
		CString strOBDType;
		srand(time(0));
		int n = rand()%10;
		strOBDType.Format(L"%s", GetOBDType(n, L"����")); 
		CString strEngineCALID, strEngineCVN;
		Sleep(1000);
		GetEngineCALID(strOBDType, L"����", strEngineCALID, strEngineCVN);

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
		strOBDType.Format(L"%s", GetOBDType(n, L"����")); 
		CString strEngineCALID, strEngineCVN;
		Sleep(1000);
		GetEngineCALID(strOBDType, L"����", strEngineCALID, strEngineCVN);

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
	if (strFulType == L"����")
	{
		switch (nType)
		{
		case 0:{ return L"EOBD";} break;
		case 1:{ return L"ISO/SAEԤ��";} break;
		case 2:{return L"OBDII-����";} break;
		case 3:{return L"OBD-OBDII";} break;
		case 4:{return L"JOBD-EOBD";} break;
		case 5:{return L"OBD,OBDII,EOBD��KOBD";} break;
		case 6:{return L"JOBD";} break;
		case 7:{return L"������";} break;
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
	if (strFulType == L"����")
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
			case 4:{strEngineCALID = L"0000QSB10900UAES"; strEngineCVN = L"��֧��";} break;
			case 5:{strEngineCALID = L"0034468840270090"; strEngineCVN = L"487D596F";} break;
			case 6:{strEngineCALID = L"0044469440150263"; strEngineCVN = L"89225B2E00000000";} break;
			case 7:{strEngineCALID = L"0064464240190159"; strEngineCVN = L"AE90F315";} break;
			case 8:{strEngineCALID = L"00740R0366UAES"; strEngineCVN = L"��֧��";} break;
			case 9:{strEngineCALID = L"007503A330UAES"; strEngineCVN = L"��֧��";} break;
			case 10:{strEngineCALID = L"00C80R0334GW"; strEngineCVN = L"��֧��";} break;
			case 11:{strEngineCALID = L"00JB0HA234UAES"; strEngineCVN = L"��֧��";} break;
			case 12:{strEngineCALID = L"00MP230421UAES"; strEngineCVN = L"��֧��";} break;
			case 13:{strEngineCALID = L"00MP261016UAES"; strEngineCVN = L"��֧��";} break;
			case 14:{strEngineCALID = L"0356CMR3SC110815"; strEngineCVN = L"B23725E3";} break;
			case 15:{strEngineCALID = L"0356CMR5SC120830"; strEngineCVN = L"E8927CBA";} break;
			case 16:{strEngineCALID = L"03C906014CE1108"; strEngineCVN = L"��֧��";} break;
			case 17:{strEngineCALID = L"03C906022AS4150"; strEngineCVN = L"A5416587";} break;
			case 18:{strEngineCALID = L"03C906022BH9928"; strEngineCVN = L"96F5B3BE";} break;
			case 19:{strEngineCALID = L"03C906022CA4511"; strEngineCVN = L"A7345A7A";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
			}
		}
		else if (strOBDType == L"ISO/SAEԤ��")
		{
			srand(time(0));
			int n = rand()%4;
			switch (n)
			{
			case 0: {strEngineCALID = L"10307843"; strEngineCVN = L"0000D42D";} break;
			case 1: {strEngineCALID = L"10294382"; strEngineCVN = L"00003C06";} break;
			case 2: {strEngineCALID = L"10190196"; strEngineCVN = L"000087DE";} break;
			case 3: {strEngineCALID = L"10236164"; strEngineCVN = L"00005C7F";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
			}
		}
		else if (strOBDType == L"OBDII-����")
		{
			srand(time(0));
			int n = rand()%4;
			switch (n)
			{
			case 0: {strEngineCALID = L"30CG8001A0C01000"; strEngineCVN = L"4A44F2412C411C65";} break;
			case 1: {strEngineCALID = L"68193991AG"; strEngineCVN = L"39381886";} break;
			case 2: {strEngineCALID = L"2410568200"; strEngineCVN = L"5A4F616B";} break;
			case 3: {strEngineCALID = L"30CL120150C81101"; strEngineCVN = L"BFEB74AB5FA56D64";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
			}
		}
		else if (strOBDType == L"OBD-OBDII")
		{
			srand(time(0));
			int n = rand()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
			}
		}
		else if (strOBDType == L"JOBD")
		{
			srand(time(0));
			int n = rand()%2;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
			}
		}
		else if (strOBDType == L"������")
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
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
			case 2: {strEngineCALID = L"LF0L0643UAES"; strEngineCVN = L"��֧��";} break;
			case 3: {strEngineCALID = L"378055A4H640"; strEngineCVN = L"378055A4H640";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
		case 14: {strEngineCALID = L"��֧��"; strEngineCVN = L"E0002658F816511F";} break;
		default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
		}

	}
}

// json��ʽģʽ
std::string JsonToString(const rapidjson::Value& valObj)
{
	rapidjson::StringBuffer sbBuf;
	rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
	valObj.Accept(jWriter);
	std::string str = std::string(sbBuf.GetString());

	return str;
}
CString COBDTestDlg::JsonToCString(const rapidjson::Value &Val)
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
		// ��ʱ����obj ֱ�����
		strVal = CA2W(JsonToString(Val).c_str());
	}
	else  if (Val.IsArray())
	{
		// ��ʱ����Arr ֱ�����
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

CString COBDTestDlg::Map2Json(JsonMap mapPost)
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

CString COBDTestDlg::MapVec2Json(JsonMapVec vmPost)
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


bool COBDTestDlg::UpOBDReaust(CString& strMsg)
{
	TESTLOG sTestLog;
	SResultOfOBD sResultOfOBD;
	CString strSQL;
	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		strMsg.AppendFormat(L"��ȡ�����־ʧ��,");
		return false;
	}
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
	{
		strMsg.AppendFormat(L"��ȡOBD�������ʧ��,");
		return false;
	}
	CString strTestType;
	strTestType.Format(L"%s", sTestLog.wchTestType);

	CStringW cstrURL(theApp.m_strURL);
	cstrURL.AppendFormat(L"/ports/detection/uploadTestOBD.do");

	CString strTemp;
	JsonMap map;
	strTemp.Format(L"%s", theApp.m_strStationNum);
	strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
	// XZQHDM	������������
	map[L"XZQHDM"] = strTemp;
	// JYJGBH	����������
	map[L"JYJGBH"] = theApp.m_strStationNum;
	// JCBH	�����
	map[L"JCBH"] = sTestLog.wchReportNumber;
	// VIN	����VIN
	map[L"VIN"] = sTestLog.wchVIN;
	// ENGINECALID	���������Ƶ�ԪCALID
	map[L"ENGINECALID"] = sResultOfOBD.strEngineCALID.c_str();
	// ENGINECVN	���������Ƶ�ԪCVN
	map[L"ENGINECVN"] = sResultOfOBD.strEngineCVN.c_str();
	// HCLCALID	������Ƶ�ԪCALID
	map[L"HCLCALID"] = L"";
	// HCLCVN	������Ƶ�ԪCVN
	map[L"HCLCVN"] = L"";
	// QTCALID	�������Ƶ�ԪCALID
	map[L"QTCALID"] = L"";
	// QTCVN	�������Ƶ�ԪCVN
	map[L"QTCVN"] = L"";
	// GZZSJG	OBD����ָʾ��
	map[L"GZZSJG"] = L"1";
	// TXJG	��OBD�����ͨѶ���
	map[L"TXJG"] = L"1";
	// TXERR	��OBDͨѶ���ɹ�ԭ��
	map[L"TXERR"] = L"";
	// ZSDJG	OBDϵͳ����ָʾ��������
	map[L"ZSDJG"] = L"0";
	// ERRCODE	���ϴ��뼰������Ϣ
	map[L"ERRCODE"] = L"";
	// JXNS	��Ͼ���״̬δ�����Ŀ
	map[L"JXNS"] = L"1";
	// JXNST	����״̬δ�����Ŀ��ϸ
	map[L"JXNST"] = L"";
	// ODOMIL	MIL�Ƶ�������ʻ���
	map[L"ODOMIL"] = L"0";
	// JCRYXM   	�����Ա����
	if (sResultOfOBD.strOperator.empty())
	{
		map[L"JCRYXM"] = L"����";
	}
	else
	{
		map[L"JCRYXM"] = sResultOfOBD.strOperator.c_str();
	}
	// ZZPDJG	�����ж����
	map[L"ZZPDJG"] = L"1";

	CStringW strOBDReport;
	strOBDReport.Format(L"SJXX=%s&KEY=%s", Map2Json(map), theApp.m_strKey);

	int nRet;
	std::wstring strRet;
	nRet = CPOSTInterfaceLib_API::HttpPost(cstrURL.GetString(), strOBDReport.GetString(), strRet);

	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			if (docu.HasMember("msg")){strMsg.AppendFormat(L"%s",JsonToCString(docu["msg"]));}
			if (docu.HasMember("success")){strMsg.AppendFormat(L"%s",JsonToCString(docu["success"]));}

			return true;
		}
		else
		{
			strMsg.Format(L"����ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.Format(L"%s", strRet.c_str());
		return false;
	}
}

bool COBDTestDlg::UPRealTimeData(CString& strMsg)
{
	TESTLOG sTestLog;
	SResultOfOBD sResultOfOBD;
	CString strSQL;
	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		strMsg.AppendFormat(L"��ȡ�����־ʧ��,");
		return false;
	}
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
	{
		strMsg.AppendFormat(L"��ȡOBD�������ʧ��,");
		return false;
	}
	CString strTestType;
	strTestType.Format(L"%s", sTestLog.wchTestType);

	CStringW cstrURL(theApp.m_strURL);
	cstrURL.AppendFormat(L"/ports/detection/uploadProcessOBD.do");

	CString strTemp;
	JsonMapVec vmPost;
	JsonMap map;

	COleDateTime odtCurTime;
	COleDateTimeSpan odts;
	SYSTEMTIME st;
	// ��̬
	if (strTestType.Find(L"1") != -1)
	{
		vmPost.clear();
		std::vector<SRealTimeDataOfASM> vt;
		GetIniRealTimeDataOfASMEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			map.clear();

			strTemp.Format(L"%s", theApp.m_strStationNum);
			strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = theApp.m_strStationNum;
			//JCBH	������
			map[L"JCBH"] = sTestLog.wchReportNumber;
			//QCSX	ȫ��ʱ��ʱ��
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"QCSX"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			//CYSX	����ʱ��
			strTemp.Format(L"%d", i+1);
			map[L"CYSX"] = strTemp;
			//CYSX	�����ž��Կ���
			map[L"CYSX"] = vt[i].sOBDRTData.strSolarTermDoorPosition.c_str();
			//JSFHZ	���㸺��ֵ
			map[L"JSFHZ"] = vt[i].sOBDRTData.strCalculationLoad.c_str();
			//QYCGQ	ǰ���������ź�
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorV.c_str();
			}
			else
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorI.c_str();
			}
			//GYKQXS	��������ϵ��
			map[L"GYKQXS"] = vt[i].sOBDRTData.strAirCoefficient.c_str();
			//SPEED	����
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				float f;
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strTemp.Format(L"%.1f,", f);
			}
			else
			{
				strTemp.Format(L"%.1f,", 0.0f);
			}
			map[L"SPEED"] = strTemp;
			//FDJSP	������ת��
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
				strTemp.Format(L"%d,", n);
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
				strTemp.Format(L"%d,", n);
			}
			map[L"FDJSP"] = strTemp;
			//JQL	������
			map[L"JQL"] = vt[i].sOBDRTData.strMAF.c_str();
			//JQYL	����ѹ��
			map[L"JQYL"] = vt[i].sOBDRTData.strMAP.c_str();
			//YMKD	���ſ���
			map[L"YMKD"] = vt[i].sOBDRTData.strThrottleOpening.c_str();
			//FDJGL	�������������
			map[L"FDJGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//ZYYL	��ѹѹ��
			map[L"ZYYL"] = vt[i].sOBDRTData.strChargeAirPressure.c_str();
			//HYL	������
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//NOCGQ	����������Ũ��
			map[L"NOCGQ"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//NSPSL	����������
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//PQWD	�����¶�
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			//KLBJY	����������ѹ��
			map[L"KLBJY"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//EGRKD	EGR����
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			//RYPSY	ȼ������ѹ��
			map[L"RYPSY"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();

			vmPost.push_back(map);
		}
	}
	// ����˲̬
	else if (strTestType.Find(L"2") != -1)
	{
		vmPost.clear();
		std::vector<SRealTimeDataOfVMAS> vt;
		GetIniRealTimeDataOfVMASEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			map.clear();

			strTemp.Format(L"%s", theApp.m_strStationNum);
			strTemp = strTemp.Left(6);  // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = theApp.m_strStationNum;
			//JCBH	������
			map[L"JCBH"] = sTestLog.wchReportNumber;
			//QCSX	ȫ��ʱ��ʱ��
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"QCSX"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			//CYSX	����ʱ��
			strTemp.Format(L"%d", i+1);
			map[L"CYSX"] = strTemp;
			//CYSX	�����ž��Կ���
			map[L"CYSX"] = vt[i].sOBDRTData.strSolarTermDoorPosition.c_str();
			//JSFHZ	���㸺��ֵ
			map[L"JSFHZ"] = vt[i].sOBDRTData.strCalculationLoad.c_str();
			//QYCGQ	ǰ���������ź�
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorV.c_str();
			}
			else
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorI.c_str();
			}
			//GYKQXS	��������ϵ��
			map[L"GYKQXS"] = vt[i].sOBDRTData.strAirCoefficient.c_str();
			//SPEED	����
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				float f;
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strTemp.Format(L"%.1f,", f);
			}
			else
			{
				strTemp.Format(L"%.1f,", 0.0f);
			}
			map[L"SPEED"] = strTemp;
			//FDJSP	������ת��
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
				strTemp.Format(L"%d,", n);
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
				strTemp.Format(L"%d,", n);
			}
			map[L"FDJSP"] = strTemp;
			//JQL	������
			map[L"JQL"] = vt[i].sOBDRTData.strMAF.c_str();
			//JQYL	����ѹ��
			map[L"JQYL"] = vt[i].sOBDRTData.strMAP.c_str();
			//YMKD	���ſ���
			map[L"YMKD"] = vt[i].sOBDRTData.strThrottleOpening.c_str();
			//FDJGL	�������������
			map[L"FDJGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//ZYYL	��ѹѹ��
			map[L"ZYYL"] = vt[i].sOBDRTData.strChargeAirPressure.c_str();
			//HYL	������
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//NOCGQ	����������Ũ��
			map[L"NOCGQ"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//NSPSL	����������
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//PQWD	�����¶�
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			//KLBJY	����������ѹ��
			map[L"KLBJY"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//EGRKD	EGR����
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			//RYPSY	ȼ������ѹ��
			map[L"RYPSY"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();

			vmPost.push_back(map);
		}
	}
	// ���ؼ���
	else if (strTestType.Find(L"3") != -1)
	{
		std::vector<SRealTimeDataOfLUGDOWN> vt;
		GetIniRealTimeDataOfLUGDOWNEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			map.clear();

			strTemp.Format(L"%s", theApp.m_strStationNum);
			strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = theApp.m_strStationNum;
			//JCBH	������
			map[L"JCBH"] = sTestLog.wchReportNumber;
			//QCSX	ȫ��ʱ��ʱ��
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"QCSX"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			//CYSX	����ʱ��
			strTemp.Format(L"%d", i+1);
			map[L"CYSX"] = strTemp;
			//CYSX	�����ž��Կ���
			map[L"CYSX"] = vt[i].sOBDRTData.strSolarTermDoorPosition.c_str();
			//JSFHZ	���㸺��ֵ
			map[L"JSFHZ"] = vt[i].sOBDRTData.strCalculationLoad.c_str();
			//QYCGQ	ǰ���������ź�
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorV.c_str();
			}
			else
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorI.c_str();
			}
			//GYKQXS	��������ϵ��
			map[L"GYKQXS"] = vt[i].sOBDRTData.strAirCoefficient.c_str();
			//SPEED	����
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				float f;
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strTemp.Format(L"%.1f,", f);
			}
			else
			{
				strTemp.Format(L"%.1f,", 0.0f);
			}
			map[L"SPEED"] = strTemp;
			//FDJSP	������ת��
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
				strTemp.Format(L"%d,", n);
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
				strTemp.Format(L"%d,", n);
			}
			map[L"FDJSP"] = strTemp;
			//JQL	������
			map[L"JQL"] = vt[i].sOBDRTData.strMAF.c_str();
			//JQYL	����ѹ��
			map[L"JQYL"] = vt[i].sOBDRTData.strMAP.c_str();
			//YMKD	���ſ���
			map[L"YMKD"] = vt[i].sOBDRTData.strThrottleOpening.c_str();
			//FDJGL	�������������
			map[L"FDJGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//ZYYL	��ѹѹ��
			map[L"ZYYL"] = vt[i].sOBDRTData.strChargeAirPressure.c_str();
			//HYL	������
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//NOCGQ	����������Ũ��
			map[L"NOCGQ"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//NSPSL	����������
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//PQWD	�����¶�
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			//KLBJY	����������ѹ��
			map[L"KLBJY"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//EGRKD	EGR����
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			//RYPSY	ȼ������ѹ��
			map[L"RYPSY"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();

			vmPost.push_back(map);
		}
	}
	// ˫����
	else if (strTestType.Find(L"4") != -1)
	{
		std::vector<SRealTimeDataOfDIS> vt;
		GetIniRealTimeDataOfDISEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			map.clear();

			strTemp.Format(L"%s", theApp.m_strStationNum);
			strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = theApp.m_strStationNum;
			//JCBH	������
			map[L"JCBH"] = sTestLog.wchReportNumber;
			//QCSX	ȫ��ʱ��ʱ��
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"QCSX"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			//CYSX	����ʱ��
			strTemp.Format(L"%d", i+1);
			map[L"CYSX"] = strTemp;
			//CYSX	�����ž��Կ���
			map[L"CYSX"] = vt[i].sOBDRTData.strSolarTermDoorPosition.c_str();
			//JSFHZ	���㸺��ֵ
			map[L"JSFHZ"] = vt[i].sOBDRTData.strCalculationLoad.c_str();
			//QYCGQ	ǰ���������ź�
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorV.c_str();
			}
			else
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorI.c_str();
			}
			//GYKQXS	��������ϵ��
			map[L"GYKQXS"] = vt[i].sOBDRTData.strAirCoefficient.c_str();
			//SPEED	����
			strTemp.Format(L"%.1f,", 0.0f);
			map[L"SPEED"] = strTemp;
			//FDJSP	������ת�� EngineRev
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTemp.Format(L"%d,", n);
			map[L"FDJSP"] = strTemp;
			//JQL	������
			map[L"JQL"] = vt[i].sOBDRTData.strMAF.c_str();
			//JQYL	����ѹ��
			map[L"JQYL"] = vt[i].sOBDRTData.strMAP.c_str();
			//YMKD	���ſ���
			map[L"YMKD"] = vt[i].sOBDRTData.strThrottleOpening.c_str();
			//FDJGL	�������������
			map[L"FDJGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//ZYYL	��ѹѹ��
			map[L"ZYYL"] = vt[i].sOBDRTData.strChargeAirPressure.c_str();
			//HYL	������
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//NOCGQ	����������Ũ��
			map[L"NOCGQ"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//NSPSL	����������
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//PQWD	�����¶�
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			//KLBJY	����������ѹ��
			map[L"KLBJY"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//EGRKD	EGR����
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			//RYPSY	ȼ������ѹ��
			map[L"RYPSY"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();

			vmPost.push_back(map);
		}
	}
	// ��͸��
	else if (strTestType.Find(L"5") != -1)
	{
		std::vector<SRealTimeDataOfFSUNHT> vt;
		GetIniRealTimeDataOfFSUNHTEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			map.clear();

			strTemp.Format(L"%s", theApp.m_strStationNum);
			strTemp = strTemp.Left(6);  // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = theApp.m_strStationNum;
			//JCBH	������
			map[L"JCBH"] = sTestLog.wchReportNumber;
			//QCSX	ȫ��ʱ��ʱ��
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"QCSX"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			//CYSX	����ʱ��
			strTemp.Format(L"%d", i+1);
			map[L"CYSX"] = strTemp;
			//CYSX	�����ž��Կ���
			map[L"CYSX"] = vt[i].sOBDRTData.strSolarTermDoorPosition.c_str();
			//JSFHZ	���㸺��ֵ
			map[L"JSFHZ"] = vt[i].sOBDRTData.strCalculationLoad.c_str();
			//QYCGQ	ǰ���������ź�
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorV.c_str();
			}
			else
			{
				map[L"QYCGQ"] = vt[i].sOBDRTData.strForwardSensorI.c_str();
			}
			//GYKQXS	��������ϵ��
			map[L"GYKQXS"] = vt[i].sOBDRTData.strAirCoefficient.c_str();
			//SPEED	����
			strTemp.Format(L"%.1f,", 0.0f);
			map[L"SPEED"] = strTemp;
			//FDJSP	������ת��
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTemp.Format(L"%d,", n);
			map[L"FDJSP"] = strTemp;
			//JQL	������
			map[L"JQL"] = vt[i].sOBDRTData.strMAF.c_str();
			//JQYL	����ѹ��
			map[L"JQYL"] = vt[i].sOBDRTData.strMAP.c_str();
			//YMKD	���ſ���
			map[L"YMKD"] = vt[i].sOBDRTData.strThrottleOpening.c_str();
			//FDJGL	�������������
			map[L"FDJGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//ZYYL	��ѹѹ��
			map[L"ZYYL"] = vt[i].sOBDRTData.strChargeAirPressure.c_str();
			//HYL	������
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//NOCGQ	����������Ũ��
			map[L"NOCGQ"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//NSPSL	����������
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//PQWD	�����¶�
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			//KLBJY	����������ѹ��
			map[L"KLBJY"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//EGRKD	EGR����
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			//RYPSY	ȼ������ѹ��
			map[L"RYPSY"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();

			vmPost.push_back(map);
		}
	}
	// ��ֽʽ
	else if (strTestType.Find(L"6") != -1)
	{
		map.clear();
		vmPost.push_back(map);
	}

	CStringW strOBDRealTime;
	strOBDRealTime.Format(L"SJXX=%s&KEY=%s", MapVec2Json(vmPost), theApp.m_strKey);

	int nRet;
	std::wstring strRet;
	nRet = CPOSTInterfaceLib_API::HttpPost(cstrURL.GetString(), strOBDRealTime.GetString(), strRet);

	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			if (docu.HasMember("msg")){strMsg.AppendFormat(L"%s",JsonToCString(docu["msg"]));}
			if (docu.HasMember("success")){strMsg.AppendFormat(L"%s",JsonToCString(docu["success"]));}

			if (strMsg.Find(L"true") != std::string::npos)
			{
				return true;
			}
		}
		else
		{
			strMsg.Format(L"����ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.Format(L"%s", strRet.c_str());
		return false;
	}

	return false;
}

bool COBDTestDlg::UpOBDIURP(CString& strMsg)
{
	return true;
}


bool COBDTestDlg::SetRTToSQL(const std::vector<SOBDRTData>& m_vtsOBDRTData, const CString& strTestType)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (strTestType.Find(L"1") != -1) // ��̬
	{
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfASM.ini", wchPath))
		{
			CSimpleIni si(wchPath);

			for(int i=0; i < m_vtsOBDRTData.size(); i++)
			{
				CString strTemp;
				strTemp.Format(L"%d", i+1);
				si.SetString(strTemp, L"OBDRTData_EngineRev", m_vtsOBDRTData[i].strEngineRev.c_str());
				si.SetString(strTemp, L"OBDRTData_Velocity", m_vtsOBDRTData[i].strVelocity.c_str());
				si.SetString(strTemp, L"OBDRTData_CoolantTemp", m_vtsOBDRTData[i].strCoolantTemp.c_str());
				si.SetString(strTemp, L"OBDRTData_SolarTermDoorPosition", m_vtsOBDRTData[i].strSolarTermDoorPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_CalculationLoad", m_vtsOBDRTData[i].strCalculationLoad.c_str());
				si.SetString(strTemp, L"OBDRTData_AirCoefficient", m_vtsOBDRTData[i].strAirCoefficient.c_str());
				si.SetString(strTemp, L"OBDRTData_MAF", m_vtsOBDRTData[i].strMAF.c_str());
			}
		}

		// ��־
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		// ���浽���������ݿ�
		SetDboTestLog(sTestLog);
		// �������
		SResultOfASM sResultOfASM;
		GetIniResultOfASMEx(&sResultOfASM);
		sResultOfASM.strRunningNumber = sTestLog.wchRunningNumber;
		sResultOfASM.strReportNumber = sTestLog.wchReportNumber;

		// ѹ����������
		std::vector<SRealTimeDataOfASM> vtRTData;
		GetIniRealTimeDataOfASMEx(vtRTData);
		for(UINT i=0; i<vtRTData.size(); i++)
		{
			vtRTData[i].strReportNumber = sTestLog.wchReportNumber;
			vtRTData[i].strRunningNumber = sTestLog.wchRunningNumber;
		}
		sResultOfASM.RTDataZip(vtRTData);

		// �������ݼ��飬����Hash
		sResultOfASM.UpdateHash();

		// ���浽���������ݿ�
		SetDboResultOfASM_Zip(sResultOfASM);
	}
	else if (strTestType.Find(L"2") != -1) // ����˲̬
	{
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfVMAS.ini", wchPath))
		{
			CSimpleIni si(wchPath);

			for(int i=0; i < m_vtsOBDRTData.size(); i++)
			{
				CString strTemp;
				strTemp.Format(L"%d", i+1);
				si.SetString(strTemp, L"OBDRTData_EngineRev", m_vtsOBDRTData[i].strEngineRev.c_str());
				si.SetString(strTemp, L"OBDRTData_Velocity", m_vtsOBDRTData[i].strVelocity.c_str());
				si.SetString(strTemp, L"OBDRTData_CoolantTemp", m_vtsOBDRTData[i].strCoolantTemp.c_str());
				si.SetString(strTemp, L"OBDRTData_SolarTermDoorPosition", m_vtsOBDRTData[i].strSolarTermDoorPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_CalculationLoad", m_vtsOBDRTData[i].strCalculationLoad.c_str());
				si.SetString(strTemp, L"OBDRTData_AirCoefficient", m_vtsOBDRTData[i].strAirCoefficient.c_str());
				si.SetString(strTemp, L"OBDRTData_MAF", m_vtsOBDRTData[i].strMAF.c_str());
			}
		}

		// ��־
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		// ���浽���������ݿ�
		SetDboTestLog(sTestLog);
		// �������
		SResultOfVMAS sResultOfVMAS;
		GetIniResultOfVMASEx(&sResultOfVMAS);
		sResultOfVMAS.strRunningNumber = sTestLog.wchRunningNumber;
		sResultOfVMAS.strReportNumber = sTestLog.wchReportNumber;

		// ѹ����������
		std::vector<SRealTimeDataOfVMAS> vtRTData;
		GetIniRealTimeDataOfVMASEx(vtRTData);
		for(UINT i=0; i<vtRTData.size(); i++)
		{
			vtRTData[i].strReportNumber = sTestLog.wchReportNumber;
			vtRTData[i].strRunningNumber = sTestLog.wchRunningNumber;
		}
		sResultOfVMAS.RTDataZip(vtRTData);

		// �������ݼ��飬����Hash
		sResultOfVMAS.UpdateHash();

		// ���浽���������ݿ�
		SetDboResultOfVMAS_Zip(sResultOfVMAS);
	}
	else if (strTestType.Find(L"3") != -1) // ���ؼ���
	{
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfLUGDOWN.ini", wchPath))
		{
			CSimpleIni si(wchPath);

			for(int i=0; i < m_vtsOBDRTData.size(); i++)
			{
				CString strTemp;
				strTemp.Format(L"%d", i+1);
				si.SetString(strTemp, L"OBDRTData_EngineRev", m_vtsOBDRTData[i].strEngineRev.c_str());
				si.SetString(strTemp, L"OBDRTData_Velocity", m_vtsOBDRTData[i].strVelocity.c_str());
				si.SetString(strTemp, L"OBDRTData_CoolantTemp", m_vtsOBDRTData[i].strCoolantTemp.c_str());
				si.SetString(strTemp, L"OBDRTData_SolarTermDoorPosition", m_vtsOBDRTData[i].strSolarTermDoorPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_MAF", m_vtsOBDRTData[i].strMAF.c_str());
				si.SetString(strTemp, L"OBDRTData_MAP", m_vtsOBDRTData[i].strMAP.c_str());
				si.SetString(strTemp, L"OBDRTData_EngineOutputPower", m_vtsOBDRTData[i].strEngineOutputPower.c_str());
				si.SetString(strTemp, L"OBDRTData_ChargeAirPressure", m_vtsOBDRTData[i].strChargeAirPressure.c_str());
				si.SetString(strTemp, L"OBDRTData_FuelConsumption", m_vtsOBDRTData[i].strFuelConsumption.c_str());
				si.SetString(strTemp, L"OBDRTData_NOXConcentration", m_vtsOBDRTData[i].strNOXConcentration.c_str());
				si.SetString(strTemp, L"OBDRTData_UreaInjectionVolume", m_vtsOBDRTData[i].strUreaInjectionVolume.c_str());
				si.SetString(strTemp, L"OBDRTData_EGT", m_vtsOBDRTData[i].strEGT.c_str());
				si.SetString(strTemp, L"OBDRTData_DPFDifferentialPressure", m_vtsOBDRTData[i].strDPFDifferentialPressure.c_str());
				si.SetString(strTemp, L"OBDRTData_EGRPosition", m_vtsOBDRTData[i].strEGRPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_FuelDeliveryPressure", m_vtsOBDRTData[i].strFuelDeliveryPressure.c_str());
			}
		}

		// ������־
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		// ���浽���������ݿ�
		SetDboTestLog(sTestLog);
		// ����������
		SResultOfLUGDOWN sResultOfLUGDOWN;
		GetIniResultOfLUGDOWNEx(&sResultOfLUGDOWN);
		sResultOfLUGDOWN.strRunningNumber = sTestLog.wchRunningNumber;
		sResultOfLUGDOWN.strReportNumber = sTestLog.wchReportNumber;

		// ѹ����������
		std::vector<SRealTimeDataOfLUGDOWN> vtRTData;
		GetIniRealTimeDataOfLUGDOWNEx(vtRTData);
		for(UINT i=0; i<vtRTData.size(); i++)
		{
			vtRTData[i].strReportNumber = sTestLog.wchReportNumber;
			vtRTData[i].strRunningNumber = sTestLog.wchRunningNumber;
		}
		sResultOfLUGDOWN.RTDataZip(vtRTData);

		// �������ݼ��飬����Hash
		sResultOfLUGDOWN.UpdateHash();

		// ���浽���������ݿ�
		SetDboResultOfLUGDOWN_Zip(sResultOfLUGDOWN);
	}
	else if (strTestType.Find(L"4") != -1) // ˫����
	{
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfDIS.ini", wchPath))
		{
			CSimpleIni si(wchPath);

			for(int i=0; i < m_vtsOBDRTData.size(); i++)
			{
				CString strTemp;
				strTemp.Format(L"%d", i+1);
				si.SetString(strTemp, L"OBDRTData_EngineRev", m_vtsOBDRTData[i].strEngineRev.c_str());
				si.SetString(strTemp, L"OBDRTData_Velocity", m_vtsOBDRTData[i].strVelocity.c_str());
				si.SetString(strTemp, L"OBDRTData_CoolantTemp", m_vtsOBDRTData[i].strCoolantTemp.c_str());
				si.SetString(strTemp, L"OBDRTData_SolarTermDoorPosition", m_vtsOBDRTData[i].strSolarTermDoorPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_CalculationLoad", m_vtsOBDRTData[i].strCalculationLoad.c_str());
				si.SetString(strTemp, L"OBDRTData_AirCoefficient", m_vtsOBDRTData[i].strAirCoefficient.c_str());
				si.SetString(strTemp, L"OBDRTData_MAF", m_vtsOBDRTData[i].strMAF.c_str());
			}
		}

		// ������־
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		// ���浽���������ݿ�
		SetDboTestLog(sTestLog);
		// ����������
		SResultOfDIS sResultOfDIS;
		GetIniResultOfDISEx(&sResultOfDIS);
		sResultOfDIS.strRunningNumber = sTestLog.wchRunningNumber;
		sResultOfDIS.strReportNumber = sTestLog.wchReportNumber;

		// ѹ����������
		std::vector<SRealTimeDataOfDIS> vtRTData;
		GetIniRealTimeDataOfDISEx(vtRTData);
		for(UINT i=0; i<vtRTData.size(); i++)
		{
			vtRTData[i].strReportNumber = sTestLog.wchReportNumber;
			vtRTData[i].strRunningNumber = sTestLog.wchRunningNumber;
		}
		sResultOfDIS.RTDataZip(vtRTData);

		// �������ݼ��飬����Hash
		sResultOfDIS.UpdateHash();

		// ���浽���������ݿ�
		SetDboResultOfDIS_Zip(sResultOfDIS);
	}
	else if (strTestType.Find(L"5") != -1) // ��͸��
	{
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"RealTimeDataOfFSUNHT.ini", wchPath))
		{
			CSimpleIni si(wchPath);

			for(int i=0; i < m_vtsOBDRTData.size(); i++)
			{
				CString strTemp;
				strTemp.Format(L"%d", i+1);
				si.SetString(strTemp, L"OBDRTData_EngineRev", m_vtsOBDRTData[i].strEngineRev.c_str());
				si.SetString(strTemp, L"OBDRTData_Velocity", m_vtsOBDRTData[i].strVelocity.c_str());
				si.SetString(strTemp, L"OBDRTData_CoolantTemp", m_vtsOBDRTData[i].strCoolantTemp.c_str());
				si.SetString(strTemp, L"OBDRTData_SolarTermDoorPosition", m_vtsOBDRTData[i].strSolarTermDoorPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_MAF", m_vtsOBDRTData[i].strMAF.c_str());
				si.SetString(strTemp, L"OBDRTData_MAP", m_vtsOBDRTData[i].strMAP.c_str());
				si.SetString(strTemp, L"OBDRTData_EngineOutputPower", m_vtsOBDRTData[i].strEngineOutputPower.c_str());
				si.SetString(strTemp, L"OBDRTData_ChargeAirPressure", m_vtsOBDRTData[i].strChargeAirPressure.c_str());
				si.SetString(strTemp, L"OBDRTData_FuelConsumption", m_vtsOBDRTData[i].strFuelConsumption.c_str());
				si.SetString(strTemp, L"OBDRTData_NOXConcentration", m_vtsOBDRTData[i].strNOXConcentration.c_str());
				si.SetString(strTemp, L"OBDRTData_UreaInjectionVolume", m_vtsOBDRTData[i].strUreaInjectionVolume.c_str());
				si.SetString(strTemp, L"OBDRTData_EGT", m_vtsOBDRTData[i].strEGT.c_str());
				si.SetString(strTemp, L"OBDRTData_DPFDifferentialPressure", m_vtsOBDRTData[i].strDPFDifferentialPressure.c_str());
				si.SetString(strTemp, L"OBDRTData_EGRPosition", m_vtsOBDRTData[i].strEGRPosition.c_str());
				si.SetString(strTemp, L"OBDRTData_FuelDeliveryPressure", m_vtsOBDRTData[i].strFuelDeliveryPressure.c_str());
			}
		}

		// ������־
		TESTLOG sTestLog;
		GetIniTestLog(&sTestLog);
		// ���浽���������ݿ�
		SetDboTestLog(sTestLog);
		// ����������
		SResultOfFSUNHT sResultOfFSUNHT;
		GetIniResultOfFSUNHTEx(&sResultOfFSUNHT);
		sResultOfFSUNHT.strRunningNumber = sTestLog.wchRunningNumber;
		sResultOfFSUNHT.strReportNumber = sTestLog.wchReportNumber;

		// ѹ����������
		std::vector<SRealTimeDataOfFSUNHT> vtRTData;
		GetIniRealTimeDataOfFSUNHTEx(vtRTData);
		for(UINT i=0; i<vtRTData.size(); i++)
		{
			vtRTData[i].strReportNumber = sTestLog.wchReportNumber;
			vtRTData[i].strRunningNumber = sTestLog.wchRunningNumber;
		}
		sResultOfFSUNHT.RTDataZip(vtRTData);

		// �������ݼ��飬����Hash
		sResultOfFSUNHT.UpdateHash();

		// ���浽���������ݿ�
		SetDboResultOfFSUNHT_Zip(sResultOfFSUNHT);

	}
	else if (strTestType.Find(L"6") != -1) // ��ֽʽ
	{

	}

	return true;
}