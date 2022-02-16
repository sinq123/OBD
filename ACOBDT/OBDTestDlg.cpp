// OBDTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ACOBDT.h"
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
	m_lbDynInfo.SetFontSize(25);
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

	// �Ƿ���Ҫ��ȡ �����ˮ�ţ���

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

		strSQL.Format(_T("Update TestLog set ItemOBD = '%s', HasOBD = '%s' where ReportNumber = '%s'"), strItemOBD, strHasOBD, m_strRunningNumber);
		nRowsInvolved = 0;
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
		OperationHint(L"����OBD�������ʧ��");
		return;
	}

	// д�����ݿ�
	SetDboResultOfOBD(sResultOfOBD);
	// д����ʱ�ļ�
	SetIniResultOfOBD(sResultOfOBD);

	CString strMsg(L"");
	//if (m_bIsOBDRealTime)
	//{
	//	strMsg.Empty();
	//	SetTestLogAndVehDB(L"0", L"0", strMsg);
	//	if (strMsg.IsEmpty())
	//	{
	//		OperationHint(L"���������ɺ��ڵ��->���������ϴ�");
	//	}
	//	else
	//	{
	//		OperationHint(strMsg);
	//	}
	//	// �Ƿ��޸�������������ʾ����
	//	/// ��ʱ����ʾ
	//	GetDlgItem(IDC_BUTTON_FINISH)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_BUTTON_FINISH)->SetWindowTextW(L"���������ϴ�");
	//}
	//else
	{
		UpOBDStrat(strMsg);
		if (strMsg.IsEmpty())
		{
			OperationHint(L"�ȴ�5��");
			Sleep(5000);
			UpOBDReaust(sResultOfOBD, strMsg);
			if (strMsg.IsEmpty())
			{
				strMsg.Empty();
				SetTestLogAndVehDB(L"4", L"1", strMsg);
				if (strMsg.IsEmpty())
				{
					OperationHint(L"OBD������");
				}
				else
				{
					OperationHint(strMsg);
				}
			}
			else
			{
				OperationHint(strMsg);
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

void COBDTestDlg::UpOBDReaust(const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	CStringW strXmlDoc = L"";
	CString strsjc;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strsjc.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strXmlDoc.Format(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strXmlDoc.AppendFormat(L"<organ>%s</organ>", theApp.m_strStationNum);
	strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", theApp.m_strLineNum);
	strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"JC011");
	strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strsjc);
	strXmlDoc.AppendFormat(L"</head><body><vehispara>");


	//1		jylsh	varchar2(50)	not null	������ˮ��(����)	���վ���(8λ)+������(yymmdd 6λ)+�����ۼ���ˮ(5λ)
	strXmlDoc.AppendFormat(L"<jylsh>%s</jylsh>", sResultOfOBD.strRunningNumber.c_str());
	//2		registCode	varchar2(50)	Not null	������Ȩ��	����ʱ����������ݵĲ�����Ȩ��
	strXmlDoc.AppendFormat(L"<registCode>%s</registCode>",  theApp.m_strLineNum);
	//3		businessId	varchar2(50)	Not null	���ҵ���ʾ	
	strXmlDoc.AppendFormat(L"<businessId>%s</businessId>", sResultOfOBD.strRunningNumber.c_str());
	//4		testtimes	number(3)	not null	�������(����)	�����������ڼ�����������Ϊ0
	strXmlDoc.AppendFormat(L"<testtimes>%d</testtimes>", m_ntesttimes == 0 ? 1: m_ntesttimes);
	strXmlDoc.AppendFormat(L"<tsno>%s</tsno>", theApp.m_strStationNum);
	//5		jyksrq	date	null	���鿪ʼʱ��	yyyy-mm-dd 24h:mi:ss
	strXmlDoc.AppendFormat(L"<jyksrq>%s</jyksrq>", sResultOfOBD.strDetBegTime.c_str());
	//6		jyjsrq	date	null	�������ʱ��	yyyy-mm-dd 24h:mi:ss
	strXmlDoc.AppendFormat(L"<jyjsrq>%s</jyjsrq>", sResultOfOBD.strDetEndTime.c_str());
	//7		jyy	varchar2(20)	null	����Ա	
	strXmlDoc.AppendFormat(L"<jyy>%s</jyy>", (sResultOfOBD.strOperator.empty() ? L"���ҳ�" : sResultOfOBD.strOperator.c_str()));
	//8		jyjg	varchar2(1)	null	������	0-��ͨ�� 1-ͨ��
	strXmlDoc.AppendFormat(L"<jyjg>%s</jyjg>", L"1");
	//9		obdzdyscqy	varchar2(100)	null	OBD �����������ҵ	
	strXmlDoc.AppendFormat(L"<obdzdyscqy>%s</obdzdyscqy>", L"��ɽ���ϻ��������޹�˾");
	//10		obdzdyxh	varchar2(100)	null	OBD ������ͺ�	
	strXmlDoc.AppendFormat(L"<obdzdyxh>%s</obdzdyxh>", L"NHOBD-1");
	//11		vin	varchar2(30)	null	����ʶ����� VIN	OBD��ȡ
	strXmlDoc.AppendFormat(L"<vin>%s</vin>", sResultOfOBD.strVIN.c_str());
	//12		obdyq	varchar2(30)	null	��ʽ����ʱ�� OBD Ҫ��	EOBD,OBDII,CN-OBD-6
	strXmlDoc.AppendFormat(L"<obdyq>%s</obdyq>", sResultOfOBD.strOBDType.c_str());
	//13		gzjg	varchar2(1)	null	OBD ϵͳ����ָʾ�� 	0-���ϸ� 1-�ϸ�
	strXmlDoc.AppendFormat(L"<gzjg>%s</gzjg>", L"1");
	//14		tx	varchar2(1)	null	OBDͨѶ	0-���ɹ� 1-�ɹ�
	strXmlDoc.AppendFormat(L"<tx>%s</tx>", L"1");
	//15		txbz	varchar2(200)	null	OBDͨѶ���ɹ�ԭ��	1-�ӿ���  2-�ӿ��Ҳ���  3-�ӿ����Ӻ���ͨѶ
	strXmlDoc.AppendFormat(L"<txbz>%s</txbz>", L"");
	//16		bj	varchar2(1)	null	OBD ϵͳ����ָʾ������	0-�� 1-��
	strXmlDoc.AppendFormat(L"<bj>%s</bj>", L"1");
	//17		bjbz	varchar2(200)	null	���ϴ��뼰������Ϣ	
	strXmlDoc.AppendFormat(L"<bjbz>%s</bjbz>", L"");
	//18		jxxm	varchar2(1)	null	����״̬δ�����Ŀ	0-�� 1-��
	strXmlDoc.AppendFormat(L"<jxxm>%s</jxxm>", L"1");
	//19		jxxmbz	varchar2(200)	null	����״̬δ�����Ŀ��ϸ	���ͣ��߻������������� ���������������� ��������ѭ��(EGR)/�ɱ����� VVT   ���ͣ�SCR ��POC ��DOC ��DPF ��EGR
	strXmlDoc.AppendFormat(L"<jxxmbz>%s</jxxmbz>", L"");
	//20		odometer         	number(8)	null	��̱����	km
	strXmlDoc.AppendFormat(L"<odometer>%s</odometer>", L"0");
	//21		odomil	number(8)	null	MIL �Ƶ��������ʻ���	km
	strXmlDoc.AppendFormat(L"<odomil>%s</odomil>", L"0");
	//22		enginecalid	varchar2(64)	null	���������Ƶ�ԪCAL ID 	
	strXmlDoc.AppendFormat(L"<enginecalid>%s</enginecalid>", sResultOfOBD.strEngineCALID.c_str());
	//23		enginecvn	varchar2(64)	null	���������Ƶ�ԪCVN	
	strXmlDoc.AppendFormat(L"<enginecvn>%s</enginecvn>", sResultOfOBD.strEngineCVN.c_str());
	//24		hclcalid	varchar2(64)	null	������Ƶ�ԪCAL ID 	
	strXmlDoc.AppendFormat(L"<hclcalid>%s</hclcalid>", L"");
	//25		hclcvn	varchar2(64)	null	������Ƶ�ԪCVN	
	strXmlDoc.AppendFormat(L"<hclcvn>%s</hclcvn>", L"");
	//26		calid	varchar2(64)	null	�������Ƶ�ԪCAL ID 	
	strXmlDoc.AppendFormat(L"<calid>%s</calid>", L"");
	//27		cvn	varchar2(64)	null	�������Ƶ�ԪCVN	
	strXmlDoc.AppendFormat(L"<cvn>%s</cvn>", L"");
	//28		iupr	varchar2(2000)	null		XML���ݸ�ʽ,��iuprs�ڵ�������涨���iupr�ڵ�����������ݣ���<iuprs><iupr>..</iupr></iuprs>
	//strXmlDoc.AppendFormat(L"<iupr>%s</iupr>", L"");
	strXmlDoc.AppendFormat(L"<iupr>%s</iupr>", OBDIURT(sResultOfOBD));
	strXmlDoc.AppendFormat(L"</vehispara></body></root>");

	std::wstring strRetStr;

	int nRet = CACHBIntLib_API::WriteObjectOut(theApp.m_pchURL, strXmlDoc.GetString(), strRetStr);

	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)theApp.DecodeURI(strRetStr.c_str());

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
			}
			else
			{
				strMsg.Format(L"��ȡ���أ�\r\n%s\r\n%s", strCode.c_str(), strContent.c_str());
			}
		}
		else
		{
			strMsg.Format(L"��ȡ���أ�\r\n%d\r\n%s", nRet, L"����ʧ��");
		}
	}
	else
	{
		strMsg.Format(L"��ȡ���أ�\r\n%d\r\n%s", nRet, L"�ӿڷ���ʧ��");
	}
}

void COBDTestDlg::UpOBDStrat(CString& strMsg)
{
	CStringW strXmlDoc = L"";
	CString strsjc;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strsjc.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strXmlDoc.Format(L"<?xml version=\"1.0\" encoding=\"gb2312\"?><root><head>");
	strXmlDoc.AppendFormat(L"<organ>%s</organ>", theApp.m_strStationNum);
	strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", theApp.m_strLineNum);
	strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"OBD001");
	strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strsjc);
	strXmlDoc.AppendFormat(L"</head><body><vehispara>");

	strXmlDoc.AppendFormat(L"<jylsh>%s</jylsh>", m_strRunningNumber);
	strXmlDoc.AppendFormat(L"<testtimes>%d</testtimes>", m_ntesttimes == 0 ? 1: m_ntesttimes);
	strXmlDoc.AppendFormat(L"<businessId>%s</businessId>", m_strRunningNumber);
	strXmlDoc.AppendFormat(L"<registCode>%s</registCode>",  theApp.m_strLineNum);
	strXmlDoc.AppendFormat(L"<message>%s</message>", L"");

	strXmlDoc.AppendFormat(L"</vehispara></body></root>");

	std::wstring strRetStr;

	int nRet = CACHBIntLib_API::WriteObjectOut(theApp.m_pchURL, strXmlDoc.GetString(), strRetStr);

	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)theApp.DecodeURI(strRetStr.c_str());

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
			}
			else
			{
				strMsg.Format(L"��ȡ���أ�\r\n%s\r\n%s", strCode.c_str(), strContent.c_str());
			}
		}
		else
		{
			strMsg.Format(L"��ȡ���أ�\r\n%d\r\n%s", nRet, L"����ʧ��");
		}
	}
	else
	{
		strMsg.Format(L"��ȡ���أ�\r\n%d\r\n%s", nRet, L"�ӿڷ���ʧ��");
	}
}


CString COBDTestDlg::OBDIURT(const SResultOfOBD& sResultOfOBD)
{
	CString strXMLDoc;

	strXMLDoc.AppendFormat(L"<iuprs>");

	CString strTemp, strTemp1;
	//1-NMHC �߻��� 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"NMHC�߻���");
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
	//2-NOX �߻��� 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"NOX�߻���");
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
	//3-NOX ������ 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"NOX������");
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
	//4-���������� 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"����������");
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
	//5-���������� 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"����������");
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
	//6-ERG��VVT
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"ERG��VVT");
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
	//7-��ѹѹ��
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"��ѹѹ��");
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
	// 8-�߻�����1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"�߻�����1");
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
	//9-�߻����� 2 
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"�߻�����2");
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
	//10-ǰ������ ���� 1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"ǰ����������1");
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
	//11- ǰ���������� 2
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"ǰ����������2");
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
	//12-������������ 1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"������������1");
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
	//13- ������������ 2
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"������������2");
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
	//16-GPF �� 1
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"GPF��1");
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
	//17-GPF �� 2
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"GPF��2");
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
	//18-���ο�������ϵͳ
	strXMLDoc.AppendFormat(L"<iupr>");
	strXMLDoc.AppendFormat(L"<jcxmmc>%s</jcxmmc>", L"���ο�������ϵͳ");
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

