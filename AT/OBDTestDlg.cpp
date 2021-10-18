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

	// �Ƿ���Ҫ��ȡ �����ˮ�ţ���

	theApp.m_strDetectLSBH = L"";
	//���»�ȡApp_data
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		theApp.m_strDetectLSBH = si.GetString(L"TestLog", L"ReportNumber", L"");
	}
	// ���»�ȡƽ̨
	int nRet;
	std::wstring strRet;
	//CString strTemp;
	//JsonMap map;
	////jczNo�����վ���
	//map[L"jczNo"] = theApp.m_strStationNum;
	////CarPZ�����ƺ�
	//map[L"CarPZ"] = m_strPlateNumber;
	////jcxNo�����������
	//map[L"jcxNo"] = theApp.m_strLineNum;
	////bdrq:�궨����
	//map[L"bdrq"] = theApp.m_odtDemarcationDate.Format(L"%Y-%m-%d 00:00:00");

	//	nRet = CATWebServic_API::OnlineJudge(theApp.m_pchURL, theApp.m_strStationNum.GetString(), theApp.m_strLineNum.GetString(),
	//		theApp.m_odtDemarcationDate.Format(L"%Y-%m-%d 00:00:00").GetString(), m_strPlateNumber.GetString(), strRet);
	//
	//
	//#ifdef _DEBUG
	//	nRet = 0;
	//	strRet = L"{\"Status\":\"1\",\"Result\":\"141102211805261622030085\"}";
	//#endif
	//	if (nRet == 0)
	//	{
	//		CString strR;
	//		strR.Format(L"%s", strRet.c_str());
	//		std::string strd(CW2A(strR.GetString()));
	//		rapidjson::Document docu;
	//		if(!docu.Parse(strd.c_str()).HasParseError())
	//		{
	//			CString strStatus, strResult;
	//			if (docu.HasMember("Status")){strStatus.Format(L"%s",JsonToCString(docu["Status"]));}
	//			if (docu.HasMember("Result")){strResult.Format(L"%s",JsonToCString(docu["Result"]));}
	//
	//			if (strStatus.Find(L"1") != std::string::npos)
	//			{
	//				strResult.Trim();
	//				theApp.m_strDetectLSBH = strResult;
	//			}
	//			else
	//			{
	//				// ��ȡƽ̨��ˮ�Ŵ��󣬲�����
	//				OperationHint(strResult);
	//				return;
	//			}
	//		}
	//		else
	//		{
	//			// ����ʧ�ܴ��󣬲�����
	//			OperationHint(L"����ʧ��");
	//			return;
	//		}
	//	}
	//	else
	//	{
	//		// �������󣬲�����
	//		OperationHint(strRet.c_str());
	//		return;
	//	}

	if(!theApp.m_strDetectLSBH.IsEmpty())
	{
		CString strMsg;
		UpOBDReaust(strMsg);
		UpOBDIURP(strMsg);

		if (strMsg.Find(L"ʧ��") == -1)
		{
			OperationHint(L"�ϴ����");
		}
		else
		{
			OperationHint(strMsg);
		}

		strMsg.Empty();
		SetTestLogAndVehDB(L"4", L"1", strMsg);

		// �Ƿ�Ҫ��ȡ��ⱨ�浥��ţ�
		// ���»�ȡ

		nRet = 99;
		strRet = L"";

		nRet = CATWebServic_API::PrintJudge(theApp.m_pchURL, theApp.m_strDetectLSBH.GetString(), theApp.m_strStationNum.GetString(), strRet);
#ifdef _DEBUG
		nRet = 0;
		strRet = L"{\"Status\":\"1\",\"Result\":\"141102211805261622030085\"}";
#endif
		if (nRet == 0)
		{
			CString strR;
			strR.Format(L"%s", strRet.c_str());
			std::string strd(CW2A(strR.GetString()));
			rapidjson::Document docu;
			if(!docu.Parse(strd.c_str()).HasParseError())
			{
				CString strStatus, strResult;
				if (docu.HasMember("Status")){strStatus.Format(L"%s",JsonToCString(docu["Status"]));}
				if (docu.HasMember("Result")){strResult.Format(L"%s",JsonToCString(docu["Result"]));}

				if (strStatus.Find(L"1") != std::string::npos)
				{
					// ��ʱ���޸�
					//strResult.Trim();
					//CString strSQL;
					//strSQL.Format(_T("Update TestLog set ReportNumber = '%s' where RunningNumber = '%s'"), strResult, m_strRunningNumber);
					//int nRowsInvolved(0);
					//if (0x00 != CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
					//{
					//	strMsg.AppendFormat(L"�޸����ݿ�->TestLog��ʧ��");
					//}

				}
				else
				{
					// ��ȡƽ̨��ˮ�Ŵ��󣬲�����
					OperationHint(strResult);
					return;
				}
			}
			else
			{
				// ����ʧ�ܴ��󣬲�����
				OperationHint(L"����ʧ��");
				return;
			}
		}
		else
		{
			// �������󣬲�����
			OperationHint(strRet.c_str());
			return;
		}
	}
	else
	{
		OperationHint(L"��ȡƽ̨��ˮ��Ϊ��");
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
	CATWebServic_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
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

int randEx()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}

void COBDTestDlg::UpOBDReaust(CString& strMsg)
{
	TESTLOG sTestLog;
	SResultOfOBD sResultOfOBD;
	CString strSQL;
	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		strMsg.AppendFormat(L"��ȡ�����־ʧ��,");
		return;
	}
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
	{
		strMsg.AppendFormat(L"��ȡOBD�������ʧ��,");
		return;
	}
	CString strTestType;
	strTestType.Format(L"%s", sTestLog.wchTestType);

	JsonMap OBDMap;
	CString strTemp;
	//OBD����ָʾ��	OBDGZZSQ
	OBDMap[L"OBDGZZSQ"] = L"�ϸ�";
	//OBDָʾ��ͨѶ���	OBDLIGHT
	OBDMap[L"OBDLIGHT"] = L"ͨѶ�ɹ�";
	//OBD���ϱ������	OBDFAULT
	OBDMap[L"OBDFAULT"] = L"��";
	//����״̬	OBDPLAN
	OBDMap[L"OBDPLAN"] = L"��";
	//MIL�Ƶ��������ʻ���	OBDMILSUM
	OBDMap[L"OBDMILSUM"] = L"0";
	//�����ۼ���ʽ���	ODOSUM
	OBDMap[L"ODOSUM"] = L"4944";
	//���������Ƶ�ԪCALID	OBDFDJCAL
	OBDMap[L"OBDFDJCAL"] = sResultOfOBD.strEngineCALID.c_str();
	//���������Ƶ�ԪCVN	OBDFDJCVN
	OBDMap[L"OBDFDJCVN"] = sResultOfOBD.strEngineCVN.c_str();
	//������Ƶ�ԪCALID	OBDHCLCAL
	OBDMap[L"OBDHCLCAL"] = L"";
	//������Ƶ�ԪCVN	OBDHCLCVN
	OBDMap[L"OBDHCLCVN"] = L"";
	//�������Ƶ�ԪCALID	OBDQTCAL
	OBDMap[L"OBDQTCAL"] = L"";
	//�������Ƶ�ԪCVN	OBDQTCVN
	OBDMap[L"OBDQTCVN"] = L"";
	//OBD�����	OBDRESULT
	OBDMap[L"OBDRESULT"] = L"1";
	//���վ���	JCZBH
	OBDMap[L"JCZBH"] = theApp.m_strStationNum;
	//����߱��	LineNo
	OBDMap[L"LineNo"] = theApp.m_strLineNum;
	//���ƺ�	CPH
	OBDMap[L"CPH"] = m_strPlateNumber;
	//����ʶ����	VIN
	OBDMap[L"VIN"] = m_strVIN;
	//��ʽ����ʱ��OBDҪ��	XSJYOBD
	OBDMap[L"XSJYOBD"] = sResultOfOBD.strOBDType.c_str();
	//�����ˮ��	DetectLSBH
	OBDMap[L"DetectLSBH"] = theApp.m_strDetectLSBH;
	//��⿪ʼʱ��	JCKSSJ
	OBDMap[L"JCKSSJ"] = sResultOfOBD.strDetBegTime.c_str();
	//������ʱ��	JCJSSJ
	OBDMap[L"JCJSSJ"] = sResultOfOBD.strDetEndTime.c_str();
	//���鷽��	JYFF
	//
	// ���������ݣ���ʽ���±�	JCGCSJ
	m_vtsOBDRTData.clear();
	JsonMapVec vm;
	COleDateTime odtCurTime;
	COleDateTimeSpan odts;
	SYSTEMTIME st;
	strTemp.Empty();

	float f;
	int n;

	if (strTestType.Find(L"1") != -1) // ��̬
	{
		OBDMap[L"JYFF"] = L"2";
		std::vector<SRealTimeDataOfASM> vt;
		GetIniRealTimeDataOfASMEx(vt);
		vm.clear();
		for(int i=0; i<vt.size(); i++)
		{
			SOBDRTData sOBDRTData;
			JsonMap map;
			//1	�����ž��Կ��ȣ�%��	YMKD
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
			strTemp.Format(L"%.2f", f);
			map[L"YMKD"] = strTemp;
			sOBDRTData.strSolarTermDoorPosition = strTemp;
			//2	���٣�Km/h��	CS
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strTemp.Format(L"%.1f", f);
			}
			else
			{
				strTemp.Format(L"%.1f", 0.0f);
			}
			map[L"CS"] = strTemp;
			sOBDRTData.strVelocity = strTemp.GetString();
			//3	���㸺��ֵkw)	FDJSCGL
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
			strTemp.Format(L"%.2f", f);

			map[L"FDJSCGL"] = strTemp;
			sOBDRTData.strCalculationLoad = strTemp;
			//4	������ת��(r/min)	FDJZS
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0){n = randEx()%300 + 1752;}
				else if (i%3 == 0){n = randEx()%300 + 1825;}
				else if (i%5 == 0){n = randEx()%300 + 1872;}
				else {n = randEx()%300 + 1852;}
				strTemp.Format(L"%d", n);
			}
			else
			{
				if (i%2 == 0){n = randEx()%300 + 650;}
				else if (i%3 == 0){n = randEx()%300 + 678;}
				else if (i%5 == 0){n = randEx()%300 + 687;}
				else {n = randEx()%300 + 640;}
				strTemp.Format(L"%d", n);
			}
			map[L"FDJZS"] = strTemp;
			sOBDRTData.strEngineRev = strTemp.GetString();
			//5	������(g/s)	JQL
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
			strTemp.Format(L"%.2f", f);
			map[L"JQL"] = strTemp;
			sOBDRTData.strMAF = strTemp;
			//6	ǰ���������ź� (mV/mA)	ZYYL
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
			strTemp.Format(L"%.2f", f);
			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"ZYYL"] = strTemp;
				sOBDRTData.strForwardSensorV = strTemp;
			}
			else
			{
				map[L"ZYYL"] = strTemp;
				sOBDRTData.strForwardSensorI = strTemp;
			}
			//7	���ʱ��	CJSJ
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"CJSJ"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

			vm.push_back(map);
			m_vtsOBDRTData.push_back(sOBDRTData);
		}
	}
	else if (strTestType.Find(L"2") != -1) // ����˲̬
	{
		std::vector<SRealTimeDataOfVMAS> vt;
		GetIniRealTimeDataOfVMASEx(vt);
		vm.clear();
		for(int i=0; i<vt.size(); i++)
		{
			JsonMap map;
			SOBDRTData sOBDRTData;
			//1	�����ž��Կ��ȣ�%��	YMKD
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
			strTemp.Format(L"%.2f", f);
			map[L"YMKD"] = strTemp;
			sOBDRTData.strSolarTermDoorPosition = strTemp;
			//2	���٣�Km/h��	CS
			if (i%2 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
			else if (i%3 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
			else if (i%5 == 0) { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
			else { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			strTemp.Format(L"%.1f", f);
			map[L"CS"] = strTemp;
			sOBDRTData.strVelocity = strTemp.GetString();
			//3	���㸺��ֵkw)	FDJSCGL
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
			strTemp.Format(L"%.2f", f);

			map[L"FDJSCGL"] = strTemp;
			sOBDRTData.strCalculationLoad = strTemp.GetString();
			//4	������ת��(r/min)	FDJZS
			if (i%2 == 0){n = randEx()%300 + 1752;}
			else if (i%3 == 0){n = randEx()%300 + 1825;}
			else if (i%5 == 0){n = randEx()%300 + 1872;}
			else {n = randEx()%300 + 1852;}
			strTemp.Format(L"%d", n);
			map[L"FDJZS"] = strTemp;
			sOBDRTData.strEngineRev = strTemp.GetString();
			//5	������(g/s)	JQL
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
			strTemp.Format(L"%.2f", f);

			map[L"JQL"] = strTemp;
			sOBDRTData.strMAF = strTemp.GetString();
			//6	ǰ���������ź� (mV/mA)	ZYYL
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
			strTemp.Format(L"%.2f", f);

			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"ZYYL"] = strTemp;
				sOBDRTData.strForwardSensorV = strTemp;
			}
			else
			{
				map[L"ZYYL"] = strTemp;
				sOBDRTData.strForwardSensorI = strTemp;
			}
			//7	���ʱ��	CJSJ
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"CJSJ"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

			vm.push_back(map);

			m_vtsOBDRTData.push_back(sOBDRTData);
		}
	}
	else if (strTestType.Find(L"3") != -1) // ���ؼ���
	{
		OBDMap[L"JYFF"] = L"4";
		std::vector<SRealTimeDataOfLUGDOWN> vt;
		GetIniRealTimeDataOfLUGDOWNEx(vt);
		vm.clear();
		for(int i=0; i<vt.size(); i++)
		{
			JsonMap map;
			SOBDRTData sOBDRTData;
			//1	���ſ��ȣ�%��	YMKD
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
			strTemp.Format(L"%.2f", f);

			map[L"YMKD"] = strTemp;
			sOBDRTData.strThrottleOpening = strTemp;
			//2	���٣�Km/h��	CS
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (randEx()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (randEx()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
				strTemp.Format(L"%.1f", f);
			}
			else
			{
				strTemp.Format(L"%.1f", 0.0f);
			}
			map[L"CS"] = strTemp;
			sOBDRTData.strVelocity = strTemp.GetString();
			//3	�������������(kw)	FDJSCGL
			map[L"FDJSCGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			sOBDRTData.strEngineOutputPower = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//4	������ת��(r/min)	FDJZS
			if (_wtoi( vt[i].strEngineRev.c_str()) != 0)
			{
				if (i%2 == 0){n = randEx()%300 + 1752;}
				else if (i%3 == 0){n = randEx()%300 + 1825;}
				else if (i%5 == 0){n = randEx()%300 + 1872;}
				else {n = randEx()%300 + 1852;}
				strTemp.Format(L"%d", n);
			}
			else
			{
				if (i%2 == 0){n = randEx()%300 + 650;}
				else if (i%3 == 0){n = randEx()%300 + 678;}
				else if (i%5 == 0){n = randEx()%300 + 687;}
				else {n = randEx()%300 + 640;}
				strTemp.Format(L"%d,", n);
			}
			map[L"FDJZS"] = strTemp;
			sOBDRTData.strEngineRev = strTemp.GetString();
			//5	������(g/s)	JQL
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
			strTemp.Format(L"%.2f", f);

			map[L"JQL"] = strTemp;
			sOBDRTData.strMAF = strTemp;
			//6	��ѹѹ��(kPa)	ZYYL
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
			strTemp.Format(L"%.2f", f);

			map[L"ZYYL"] = strTemp;
			sOBDRTData.strChargeAirPressure = strTemp;
			//7	������(L/100km)	HYL
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			sOBDRTData.strFuelConsumption = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//8	����������Ũ��(ppm)	DYCGQND
			map[L"DYCGQND"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			sOBDRTData.strNOXConcentration = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//9	����������(L/h)	NSPSL
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			sOBDRTData.strUreaInjectionVolume = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//10	�����¶�(��C)	PQWD
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			sOBDRTData.strEGT = vt[i].sOBDRTData.strEGT.c_str();
			//11	����������ѹ��(kpa)	KLBJQYC
			map[L"KLBJQYC"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			sOBDRTData.strDPFDifferentialPressure = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//12	EGR����(%)	EGRKD
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			sOBDRTData.strEGRPosition = vt[i].sOBDRTData.strEGRPosition.c_str();
			//13	ȼ������ѹ��(bar)	RYPSYL
			map[L"RYPSYL"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();
			sOBDRTData.strFuelDeliveryPressure = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();
			//9	���ʱ��	CJSJ
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"CJSJ"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			vm.push_back(map);

			m_vtsOBDRTData.push_back(sOBDRTData);
		}
	}
	else if (strTestType.Find(L"4") != -1) // ˫����
	{
		OBDMap[L"JYFF"] = L"1";
		std::vector<SRealTimeDataOfDIS> vt;
		GetIniRealTimeDataOfDISEx(vt);
		vm.clear();
		for(int i=0; i<vt.size(); i++)
		{
			JsonMap map;
			SOBDRTData sOBDRTData;
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
			strTemp.Format(L"%.2f", f);
			//1	�����ž��Կ��ȣ�%��	YMKD
			map[L"YMKD"] = strTemp;
			sOBDRTData.strSolarTermDoorPosition = strTemp;
			//2	���٣�Km/h��	CS
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			strTemp.Format(L"%.1f", 0.0f);
			map[L"CS"] = strTemp;
			sOBDRTData.strVelocity = strTemp.GetString();
			//3	���㸺��ֵkw)	FDJSCGL
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
			strTemp.Format(L"%.2f", f);

			map[L"FDJSCGL"] = strTemp;
			sOBDRTData.strCalculationLoad = strTemp;
			//4	������ת��(r/min)	FDJZS
			if (i%2 == 0){n = randEx()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTemp.Format(L"%d", n);
			map[L"FDJZS"] = strTemp;
			sOBDRTData.strEngineRev = strTemp.GetString();
			//5	������(g/s)	JQL
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
			strTemp.Format(L"%.2f", f);
			map[L"JQL"] = strTemp;
			sOBDRTData.strMAF = strTemp.GetString();
			//6	ǰ���������ź� (mV/mA)	ZYYL
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
			strTemp.Format(L"%.2f", f);

			if (vt[i].sOBDRTData.strForwardSensorI.empty())
			{
				map[L"ZYYL"] = strTemp;
				sOBDRTData.strForwardSensorV = strTemp;
			}
			else
			{
				map[L"ZYYL"] = strTemp;
				sOBDRTData.strForwardSensorI = strTemp;
			}
			//7	���ʱ��	CJSJ
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"CJSJ"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

			vm.push_back(map);

			m_vtsOBDRTData.push_back(sOBDRTData);
		}
	}
	else if (strTestType.Find(L"5") != -1) // ��͸��
	{
		OBDMap[L"JYFF"] = L"6";
		std::vector<SRealTimeDataOfFSUNHT> vt;
		GetIniRealTimeDataOfFSUNHTEx(vt);
		vm.clear();
		for(int i=0; i<vt.size(); i++)
		{
			JsonMap map;
			SOBDRTData sOBDRTData;
			//1	���ſ��ȣ�%��	YMKD
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
			strTemp.Format(L"%.2f", f);

			map[L"YMKD"] = strTemp;
			sOBDRTData.strThrottleOpening = strTemp;
			//2	���٣�Km/h��	CS
			strTemp.Format(L"%.1f,", 0.0f);
			map[L"CS"] = strTemp;
			sOBDRTData.strVelocity = strTemp.GetString();
			//3	�������������(kw)	FDJSCGL
			map[L"FDJSCGL"] = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			sOBDRTData.strEngineOutputPower = vt[i].sOBDRTData.strEngineOutputPower.c_str();
			//4	������ת��(r/min)	FDJZS
			if (i%2 == 0){n = randEx()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = randEx()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTemp.Format(L"%d", n);
			map[L"FDJZS"] = strTemp;
			sOBDRTData.strEngineRev = strTemp.GetString();
			//5	������(g/s)	JQL
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
			strTemp.Format(L"%.2f", f);

			map[L"JQL"] = strTemp;
			sOBDRTData.strMAF = strTemp;
			//6	��ѹѹ��(kPa)	ZYYL
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
			strTemp.Format(L"%.2f", f);

			map[L"ZYYL"] = strTemp;
			sOBDRTData.strChargeAirPressure = strTemp;
			//7	������(L/100km)	HYL
			map[L"HYL"] = vt[i].sOBDRTData.strFuelConsumption.c_str();
			sOBDRTData.strFuelConsumption = vt[i].sOBDRTData.strFuelConsumption.c_str();
			//8	����������Ũ��(ppm)	DYCGQND
			map[L"DYCGQND"] = vt[i].sOBDRTData.strNOXConcentration.c_str();
			sOBDRTData.strNOXConcentration = vt[i].sOBDRTData.strNOXConcentration.c_str();
			//9	����������(L/h)	NSPSL
			map[L"NSPSL"] = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			sOBDRTData.strUreaInjectionVolume = vt[i].sOBDRTData.strUreaInjectionVolume.c_str();
			//10	�����¶�(��C)	PQWD
			map[L"PQWD"] = vt[i].sOBDRTData.strEGT.c_str();
			sOBDRTData.strEGT = vt[i].sOBDRTData.strEGT.c_str();
			//11	����������ѹ��(kpa)	KLBJQYC
			map[L"KLBJQYC"] = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			sOBDRTData.strDPFDifferentialPressure = vt[i].sOBDRTData.strDPFDifferentialPressure.c_str();
			//12	EGR����(%)	EGRKD
			map[L"EGRKD"] = vt[i].sOBDRTData.strEGRPosition.c_str();
			sOBDRTData.strEGRPosition = vt[i].sOBDRTData.strEGRPosition.c_str();
			//13	ȼ������ѹ��(bar)	RYPSYL
			map[L"RYPSYL"] = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();
			sOBDRTData.strFuelDeliveryPressure = vt[i].sOBDRTData.strFuelDeliveryPressure.c_str();
			//9	���ʱ��	CJSJ
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			map[L"CJSJ"] = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
			vm.push_back(map);

			m_vtsOBDRTData.push_back(sOBDRTData);
		}
	}
	else if (strTestType.Find(L"6") != -1) // ��ֽʽ
	{

	}

	OBDMap[L"JCGCSJ"] = MapVec2Json(vm);

	//Map2Json(OBDMap);
	if (m_vtsOBDRTData.size() > 0)
	{
		SetRTToSQL(m_vtsOBDRTData, strTestType);
	}
	int nRet;
	std::wstring strRet;
	nRet = CATWebServic_API::AddObdData(theApp.m_pchURL, Map2Json(OBDMap).GetString(),  strRet);

#ifdef _DEBUG
	nRet = 0;
	strRet = L"{\"Status\":\"1\",\"Result\":\"�ɹ�\"}";
#endif
	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strStatus, strResult;
			if (docu.HasMember("Status")){strStatus.Format(L"%s",JsonToCString(docu["Status"]));}
			if (docu.HasMember("Result")){strResult.Format(L"%s",JsonToCString(docu["Result"]));}

			if (strStatus.Find(L"1") != std::string::npos)
			{
				strMsg.AppendFormat(L"�ϴ��ɹ�,");
			}
			else
			{
				strMsg.AppendFormat(L"�ϴ�ʧ��,ԭ��%s", strResult);
				return;
			}
		}
		else
		{
			// ����ʧ�ܴ��󣬲�����
			OperationHint(L"����ʧ��");
			return;
		}
	}
	else
	{
		// �������󣬲�����
		strMsg.AppendFormat(L"�ϴ�ʧ��,ԭ��%s", strRet.c_str());
		return;
	}

}

void COBDTestDlg::UpOBDIURP(CString& strMsg)
{
	TESTLOG sTestLog;
	SResultOfOBD sResultOfOBD;
	CString strSQL;
	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		strMsg.AppendFormat(L"��ȡ�����־ʧ��,");
		return;
	}
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
	{
		strMsg.AppendFormat(L"��ȡOBD�������ʧ��,");
		return;
	}

	JsonMap OBDMap;
	CString strTemp, strTemp1;
	if (m_strFuelType.Find(L"��") != -1)
	{
		//�߻�����1����	CHQZ1NAME
		OBDMap[L"CHQZ1NAME"] = L"�߻�����1";
		//�߻�����1�����ɴ���	CHQZ1NUM
		OBDMap[L"CHQZ1NUM"] = sResultOfOBD.strIUPR_CMCCB1.c_str();
		//�߻�����1���ϼ����������	CHQZ1COUNT
		OBDMap[L"CHQZ1COUNT"] = sResultOfOBD.strIUPR_CMCECB1.c_str();
		//�߻�����1IUPR��	CHQZ1IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_CMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB1.c_str()))));
		}
		OBDMap[L"CHQZ1IUPR"] = strTemp;
		//�߻�����2����	CHQZ2NAME
		OBDMap[L"CHQZ2NAME"] = L"�߻�����2";
		//�߻�����2�����ɴ���	CHQZ2NUM
		OBDMap[L"CHQZ2NUM"] = sResultOfOBD.strIUPR_CMCCB2.c_str();
		//�߻�����2���ϼ����������	CHQZ2COUNT
		OBDMap[L"CHQZ2COUNT"] = sResultOfOBD.strIUPR_CMCECB2.c_str();
		//�߻�����2IUPR��	CHQZ2IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_CMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_CMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_CMCCB2.c_str()))));
		}
		OBDMap[L"CHQZ2IUPR"] = strTemp;
		//ǰ����������1����	QYCGQZ1NAME
		OBDMap[L"QYCGQZ1NAME"] = L"ǰ����������1";
		//ǰ����������1�����ɴ���	QYCGQZ1NUM
		OBDMap[L"QYCGQZ1NUM"] = sResultOfOBD.strIUPR_O2SMCCB1.c_str();
		//ǰ����������1���ϼ����������	QYCGQZ1COUNT
		OBDMap[L"QYCGQZ1COUNT"] = sResultOfOBD.strIUPR_O2SMCECB1.c_str();
		//ǰ����������1IUPR��	QYCGQZ1IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCCB1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCECB1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB1.c_str()))));
		}
		OBDMap[L"QYCGQZ1IUPR"] = strTemp;
		//ǰ����������2����	QYCGQZ2NAME
		OBDMap[L"QYCGQZ2NAME"] = L"ǰ����������2";
		//ǰ����������2�����ɴ���	QYCGQZ2NUM
		OBDMap[L"QYCGQZ2NUM"] = sResultOfOBD.strIUPR_O2SMCCB2.c_str();
		//ǰ����������2���ϼ����������	QYCGQZ2COUNT
		OBDMap[L"QYCGQZ2COUNT"] = sResultOfOBD.strIUPR_O2SMCECB2.c_str();
		//ǰ����������2IUPR��	QYCGQZ2IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_O2SMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_O2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_O2SMCCB2.c_str()))));
		}
		OBDMap[L"QYCGQZ2IUPR"] = strTemp;
		//������������1����	HYCGQZ1NAME
		OBDMap[L"HYCGQZ1NAME"] = L"������������1";
		//������������1�����ɴ���	HYCGQZ1NUM
		OBDMap[L"HYCGQZ1NUM"] = sResultOfOBD.strIUPR_RO2SMCCB1.c_str();
		//������������1���ϼ����������	HYCGQZ1COUNT
		OBDMap[L"HYCGQZ1COUNT"] = sResultOfOBD.strIUPR_RO2SMCECB1.c_str();
		//������������1IUPR��	HYCGQZ1IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB1.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB1.c_str()))));
		}
		OBDMap[L"HYCGQZ1IUPR"] = strTemp;
		//������������2����	HYCGQZ2NAME
		OBDMap[L"HYCGQZ2NAME"] = L"������������2";
		//������������2�����ɴ���	HYCGQZ2NUM
		OBDMap[L"HYCGQZ2NUM"] = sResultOfOBD.strIUPR_RO2SMCCB2.c_str();
		//������������2���ϼ����������	HYCGQZ2COUNT
		OBDMap[L"HYCGQZ2COUNT"] = sResultOfOBD.strIUPR_RO2SMCECB2.c_str();
		//������������2���IUPR��	HYCGQZ2IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_RO2SMCECB2.c_str()))/(_wtof(sResultOfOBD.strIUPR_RO2SMCCB2.c_str()))));
		}
		OBDMap[L"HYCGQZ2IUPR"] = strTemp;
		//EVAP����	EVAPNAME
		OBDMap[L"EVAPNAME"] = L"EVAP";
		//EVAP�����ɴ���	EVAPNUM
		OBDMap[L"EVAPNUM"] = sResultOfOBD.strIUPR_EVAPC.c_str();
		//EVAP���ϼ����������	EVAPCOUNT
		OBDMap[L"EVAPCOUNT"] = sResultOfOBD.strIUPR_EVAPEC.c_str();
		//EVAP���IUPR��	EVAPIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EVAPC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EVAPEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_EVAPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EVAPC.c_str()))));
		}
		OBDMap[L"EVAPIUPR"] = strTemp;
		//EGR��VVT����	EGRVVTNAME
		OBDMap[L"EGRVVTNAME"] = L"EGR��VVT";
		//EGR��VVT�����ɴ���	EGRVVTNUM
		OBDMap[L"EGRVVTNUM"] = sResultOfOBD.strIUPR_EGRC.c_str();
		//EGR��VVT�����������	EGRVVTCOUNT
		OBDMap[L"EGRVVTCOUNT"] = sResultOfOBD.strIUPR_EGREC.c_str();
		//EGR��VVT���IUPR��	EGRVVTIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
		}
		OBDMap[L"EGRVVTIUPR"] = strTemp;
		//GPF��1����	GPF1NAME
		OBDMap[L"GPF1NAME"] = L"GPF��1";
		//GPF��1�����ɴ���	GPF1NUM
		OBDMap[L"GPF1NUM"] = sResultOfOBD.strIUPR_GPFC1.c_str();
		//GPF��1���ϼ����������	GPF1COUNT
		OBDMap[L"GPF1COUNT"] = sResultOfOBD.strIUPR_GPFEC1.c_str();
		//GPF��1���IUPR��	GPF1IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFC1.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFEC1.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_GPFEC1.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC1.c_str()))));
		}
		OBDMap[L"GPF1IUPR"] = strTemp;
		//GPF��2����	GPF2NAME
		OBDMap[L"GPF2NAME"] = L"GPF��2";
		//GPF��2�����ɴ���	GPF2NUM
		OBDMap[L"GPF2NUM"] = sResultOfOBD.strIUPR_GPFC2.c_str();
		//GPF��2���ϼ����������	GPFCOUNT
		OBDMap[L"GPFCOUNT"] = sResultOfOBD.strIUPR_GPFEC2.c_str();
		//GPF��2���IUPR��	GPF2IUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFC2.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_GPFEC2.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_GPFEC2.c_str()))/(_wtof(sResultOfOBD.strIUPR_GPFC2.c_str()))));
		}
		OBDMap[L"GPF2IUPR"] = strTemp;
		//���ο�������ϵͳ����	KQPSXTNAME
		OBDMap[L"KQPSXTNAME"] = L"���ο�������ϵͳ";
		//���ο�������ϵͳ�����ɴ���	KQPSXTNUM
		OBDMap[L"KQPSXTNUM"] = sResultOfOBD.strIUPR_AMCCC.c_str();
		//���ο�������ϵͳ���ϼ����������	KQPSXTCOUNT
		OBDMap[L"KQPSXTCOUNT"] = sResultOfOBD.strIUPR_AMCEC.c_str();
		//���ο�������ϵͳ���IUPR��	KQPSXTIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_AMCCC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_AMCEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_AMCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_AMCCC.c_str()))));
		}
		OBDMap[L"KQPSXTIUPR"] = strTemp;
		//���վ���	JCZBH
		OBDMap[L"JCZBH"] = theApp.m_strStationNum;
		//����߱��	LineNo
		OBDMap[L"LineNo"] = theApp.m_strLineNum;
		//���ƺ�	CPH
		OBDMap[L"CPH"] = m_strPlateNumber;
		//����ʶ����	VIN
		OBDMap[L"VIN"] = m_strVIN;
		//�����ˮ��	DetectLSBH
		OBDMap[L"DetectLSBH"] = theApp.m_strDetectLSBH;
		//��ʼʱ��	STARTTIME
		OBDMap[L"JCKSSJ"] = sResultOfOBD.strDetBegTime.c_str();
		//����ʱ��	ENDTIME
		OBDMap[L"JCJSSJ"] = sResultOfOBD.strDetEndTime.c_str();
	}
	else
	{
		//NMHC�߻�������	NMHCNAME
		OBDMap[L"NMHCNAME"] = L"NMHC�߻���";
		//NMHC�߻��������ɴ���	NMHCNUM
		OBDMap[L"NMHCNUM"] = sResultOfOBD.strIUPR_NMHCC.c_str();
		//NMHC�߻������ϼ����������	NMHCCOUNT
		OBDMap[L"NMHCCOUNT"] = sResultOfOBD.strIUPR_NMHCEC.c_str();
		//NMHC�߻���IUPR��	NMHCIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NMHCC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NMHCEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_NMHCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NMHCC.c_str()))));
		}
		OBDMap[L"NMHCIUPR"] = strTemp;
		//NOX�߻�������	NOXCHQNAME
		OBDMap[L"NOXCHQNAME"] = L"NOX�߻���";
		//NOX�߻��������ɴ���	NOXCHQNUM
		OBDMap[L"NOXCHQNUM"] = sResultOfOBD.strIUPR_NOXCC.c_str();
		//NOX�߻������ϼ����������	NOXCHQCOUNT
		OBDMap[L"NOXCHQCOUNT"] = sResultOfOBD.strIUPR_NOXCEC.c_str();
		//NOX�߻���IUPR��	NOXCHQIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXCC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXCEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_NOXCEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXCC.c_str()))));
		}
		OBDMap[L"NOXCHQIUPR"] = strTemp;
		//NOX����������	NOXXFQNAME
		OBDMap[L"NOXXFQNAME"] = L"NOX������";
		//NOX�����������ɴ���	NOXXFNUM
		OBDMap[L"NOXXFNUM"] = sResultOfOBD.strIUPR_NOXAC.c_str();
		//NOX���������ϼ����������	NOXXFCONT
		OBDMap[L"NOXXFCONT"] = sResultOfOBD.strIUPR_NOXAEC.c_str();
		//NOX������IUPR��	NOXXFIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXAC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_NOXAEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_NOXAEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_NOXAC.c_str()))));
		}
		OBDMap[L"NOXXFIUPR"] = strTemp;
		//PM����������	PMNAME
		OBDMap[L"PMNAME"] = L"PM������";
		//PM�����������ɴ���	PMNUM
		OBDMap[L"PMNUM"] = sResultOfOBD.strIUPR_PMC.c_str();
		//PM���������ϼ����������	PMCOUNT
		OBDMap[L"PMCOUNT"] = sResultOfOBD.strIUPR_PMEC.c_str();
		//PM������IUPR��	PMIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PMC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PMEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_PMEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PMC.c_str()))));
		}
		OBDMap[L"PMIUPR"] = strTemp;
		//��������������	FQNAME
		OBDMap[L"FQNAME"] = L"����������";
		//���������������ɴ���	GQNUM
		OBDMap[L"GQNUM"] = sResultOfOBD.strIUPR_WSC.c_str();
		//�������������ϼ����������	FQCOUNT
		OBDMap[L"FQCOUNT"] = sResultOfOBD.strIUPR_WSEC.c_str();
		//����������IUPR��	FQIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_WSC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_WSEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_WSEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_WSC.c_str()))));
		}
		OBDMap[L"FQIUPR"] = strTemp;
		//EGR��VVT����	EGRVVTNAME
		OBDMap[L"EGRVVTNAME"] = L"EGR��VVT";
		//EGR��VVT�����ɴ���	EGRVVTNUM
		OBDMap[L"EGRVVTNUM"] = sResultOfOBD.strIUPR_EGRC.c_str();
		//EGR��VVT���ϼ����������	EGRVVTCOUNT
		OBDMap[L"EGRVVTCOUNT"] = sResultOfOBD.strIUPR_EGREC.c_str();
		//EGR��VVT���IUPR��	EGRVVTIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGRC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_EGREC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_EGREC.c_str()))/(_wtof(sResultOfOBD.strIUPR_EGRC.c_str()))));
		}
		OBDMap[L"EGRVVTIUPR"] = strTemp;
		//��ѹѹ������	ZYYLNAME
		OBDMap[L"ZYYLNAME"] = L"��ѹѹ��";
		//��ѹѹ�������ɴ���	ZYYLNUM
		OBDMap[L"ZYYLNUM"] = sResultOfOBD.strIUPR_PPC.c_str();
		//��ѹѹ�����ϼ����������	ZYYLCOUNT
		OBDMap[L"ZYYLCOUNT"] = sResultOfOBD.strIUPR_PPEC.c_str();
		//��ѹѹ�����IUPR��	ZYYLIUPR
		strTemp.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PPC.c_str()));
		strTemp1.Format(L"%d", _wtoi(sResultOfOBD.strIUPR_PPEC.c_str()));
		if (strTemp == L"0" || strTemp1 == L"0")
		{

			strTemp = L"";
		}
		else
		{
			strTemp.Format(L"%.2f", ((_wtof(sResultOfOBD.strIUPR_PPEC.c_str()))/(_wtof(sResultOfOBD.strIUPR_PPC.c_str()))));
		}
		OBDMap[L"ZYYLIUPR"] = strTemp;
		//���վ���	JCZBH
		OBDMap[L"JCZBH"] = theApp.m_strStationNum;
		//����߱��	LineNo
		OBDMap[L"LineNo"] = theApp.m_strLineNum;
		//���ƺ�	CPH
		OBDMap[L"CPH"] = m_strPlateNumber;
		//����ʶ����	VIN
		OBDMap[L"VIN"] = m_strVIN;
		//�����ˮ��	DetectLSBH
		OBDMap[L"DetectLSBH"] = theApp.m_strDetectLSBH;
		//��ʼʱ��	STARTTIME
		OBDMap[L"JCKSSJ"] = sResultOfOBD.strDetBegTime.c_str();
		//����ʱ��	ENDTIME
		OBDMap[L"JCJSSJ"] = sResultOfOBD.strDetEndTime.c_str();
	}

	Map2Json(OBDMap);

	int nRet;
	std::wstring strRet;
	if (m_strFuelType.Find(L"��") != -1)
	{
		nRet = CATWebServic_API::AddIuprData(theApp.m_pchURL, Map2Json(OBDMap).GetString(), L"QY", strRet);
	}
	else
	{
		nRet = CATWebServic_API::AddIuprData(theApp.m_pchURL, Map2Json(OBDMap).GetString(), L"CY", strRet);
	}

#ifdef _DEBUG
	nRet = 0;
	strRet = L"{\"Status\":\"1\",\"Result\":\"�ɹ�\"}";
#endif
	if (nRet == 0)
	{
		CString strR;
		strR.Format(L"%s", strRet.c_str());
		std::string strd(CW2A(strR.GetString()));
		rapidjson::Document docu;
		if(!docu.Parse(strd.c_str()).HasParseError())
		{
			CString strStatus, strResult;
			if (docu.HasMember("Status")){strStatus.Format(L"%s",JsonToCString(docu["Status"]));}
			if (docu.HasMember("Result")){strResult.Format(L"%s",JsonToCString(docu["Result"]));}

			if (strStatus.Find(L"1") != std::string::npos)
			{
				strMsg.AppendFormat(L"�ϴ��ɹ�,");
			}
			else
			{
				strMsg.AppendFormat(L"�ϴ�ʧ��,ԭ��%s", strResult);
				return;
			}
		}
		else
		{
			// ����ʧ�ܴ��󣬲�����
			OperationHint(L"����ʧ��");
			return;
		}
	}
	else
	{
		// �������󣬲�����
		strMsg.AppendFormat(L"�ϴ�ʧ��,ԭ��%s", strRet.c_str());
		return;
	}
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