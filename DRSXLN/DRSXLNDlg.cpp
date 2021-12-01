
// DRSXLNDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DRSXLN.h"
#include "DRSXLNDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDRSXLNDlg �Ի���




CDRSXLNDlg::CDRSXLNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDRSXLNDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDRSXLNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassword);
	DDX_Control(pDX, ID_ED_PlateNumber, m_edPlateNumber);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMessAge);
	DDX_Control(pDX, IDC_RA_SQL, m_edchkSQL);
	DDX_Control(pDX, IDC_RA_INTET, m_edIntet);

	DDX_Control(pDX, ID_ED_SERVERIP, m_edServerIP);
	DDX_Control(pDX, ID_ED_SERVERPORT, m_edServerPost);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDCZYMM, m_edOBDCZYMM);
	DDX_Control(pDX, ID_ED_OBDCZYID, m_edOBDCZYID);

	DDX_Control(pDX, IDC_COM_OBDJCY, m_cbOBDJCY);

}

BEGIN_MESSAGE_MAP(CDRSXLNDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CDRSXLNDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CDRSXLNDlg::OnBnClickedBtnSetconfig)
	ON_EN_CHANGE(ID_ED_PlateNumber, &CDRSXLNDlg::OnEnChangeEdPlatenumber)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CDRSXLNDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UP, &CDRSXLNDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_UP_TELET, &CDRSXLNDlg::OnBnClickedBtnUpTelet)
END_MESSAGE_MAP()


// CDRSXLNDlg ��Ϣ�������

BOOL CDRSXLNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(L"����");

	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();

	m_edchkSQL.SetCheck(TRUE);
	m_edIntet.SetCheck(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDRSXLNDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDRSXLNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CDRSXLNDlg::SendAndRecvPacket(const CString& strIP, const CString& strPort, LPCTSTR szXML, std::wstring &strRecv)
{
	CString strXML = szXML;
	strXML.Replace(L" ", L"%20");	// ת��
	strXML.Replace(L"\r", L"%0d");	// ת��
	strXML.Replace(L"\n", L"%0a");	// ת��

	CString strSend;
	CString strURL(L"/CtrlCenter/ASM");
	strSend.Format(L" %s?data=%s\r\n",strURL, strXML);	// ��ַǰ�����пո�

	CNSSocket nsSocket;

	int nPort = _wtoi(strPort);

	nsSocket.Open(strIP, nPort);
	return nsSocket.SendAndRecvPacket(strSend, strRecv);
}

void CDRSXLNDlg::SetConfig(void)
{
	// ��ȡexe(dll)�ļ�����·��
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
		m_edPassword.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NSConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"NSServer", L"IP", L"");
		m_edServerIP.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"NSServer", L"Port", L"");
		m_edServerPost.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"NSServer", L"DeviceSN", L"");
		m_edLineNumber.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"NSServer", L"OperatorName", L"");
		m_edOBDCZYID.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"NSServer", L"OperatorPassword", L"");
		m_edOBDCZYMM.SetWindowTextW(strTemp);
	}

}


void CDRSXLNDlg::SetVehCtrl(void)
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
		L"����");

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
		L"����");


	m_lstVehicle.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstVehicle.SetFont(&fontVehicleLCFont);
	m_lstVehicle.SetTextColor(RGB(0, 255, 0));
	m_lstVehicle.SetBkColor(RGB(0, 0, 0));
	m_lstVehicle.SetTextBkColor(RGB(0, 0, 0));

	m_lstVehicle.InsertColumn(0, L"���",		0,	0);
	m_lstVehicle.InsertColumn(1, L"��ˮ��",		0,	300);
	m_lstVehicle.InsertColumn(2, L"���ƺ���",	0,	150);
	m_lstVehicle.InsertColumn(3, L"��������",	0,	120);
	m_lstVehicle.InsertColumn(4, L"VIN",	0,	120);
	m_lstVehicle.InsertColumn(5, L"�������",	0,	300);
	//m_lstVehicle.InsertColumn(6, L"OBD���",	0,	100);
	//m_lstVehicle.InsertColumn(7, L"OBD���״̬",	0,	100);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}


bool CDRSXLNDlg::GetVehicleList(void)
{
	m_edMessAge.SetWindowTextW(L"");
	// ɾ���б�����
	m_lstVehicle.DeleteAllItems();
	CString strSQL;

	CString strSource, strCatalog, strUserID, strPassword;

	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassword.GetWindowTextW(strPassword);

	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"���ݿ����Ϊ��");
		return false;
	}


	// ��ȡ�����б�
	{
		std::list<SOBDVEHLIST> lsOBDVehList;



		CString strDeviceSN, strIP, strPort;
		m_edLineNumber.GetWindowTextW(strDeviceSN);
		m_edServerIP.GetWindowTextW(strIP);
		m_edServerPost.GetWindowTextW(strPort);

		CString strXML;
		strXML.AppendFormat(L"<Message Device=\"%s\">", strDeviceSN);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVehicleList");
		strXML.AppendFormat(L"</Request>");
		strXML.AppendFormat(L"</Message>");

		std::wstring strRecv;
		bool bRet = SendAndRecvPacket(strIP, strPort, strXML, strRecv);

		if (bRet)
		{
			CXmlReader xmlReader;

			if (xmlReader.Parse(strRecv.c_str()))
			{
				std::wstring strResult, strMsg;

				if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
				{
					xmlReader.GetNodeContent(strResult);
				}
				if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
				{
					xmlReader.GetNodeContent(strMsg);
				}

				if (strResult == L"")	// û��Result�ڵ�ʱ�г����б�
				{
					if (xmlReader.OpenNode(L"Message/Respond/Row/Vehicle"))
					{
						do
						{
							std::map<std::wstring, std::wstring> mapField;	
							xmlReader.EnterNode();

							do
							{
								std::wstring strName, strContent;
								xmlReader.GetNodeName(strName);
								xmlReader.GetNodeContent(strContent);
								mapField[strName] = strContent;
							}
							while (xmlReader.NextSibling());
							xmlReader.ExitNode();

							SOBDVEHLIST sOBDVehList;

							sOBDVehList.strRunningNumber = mapField[L"OutlookID"].c_str();
							sOBDVehList.strPaletNumber = mapField[L"License"].c_str();
							sOBDVehList.strPlateType = mapField[L"LicenseType"].c_str();
							sOBDVehList.strVIN = mapField[L"Vin"].c_str();
							sOBDVehList.strVehType = mapField[L"VehicleKind"].c_str();

							lsOBDVehList.push_back(sOBDVehList);
						}
						while (xmlReader.NextSibling());
					}
				}
			}

		}

		int nItem(0);
		for(list<SOBDVEHLIST>::iterator ir =lsOBDVehList.begin(); ir != lsOBDVehList.end(); ir++)
		{
			CString str;
			// ���
			str.Format(L"%d", nItem+1);
			int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

			// ��ˮ��
			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strRunningNumber.c_str(), 0, 0, 0, 0);
			// ���ƺ���
			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strPaletNumber.c_str(), 0, 0, 0, 0);
			// ��������
			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strPlateType.c_str(), 0, 0, 0, 0);
			// VIN
			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strVIN.c_str(), 0, 0, 0, 0);
			// �������
			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strVehType.c_str(), 0, 0, 0, 0);

			nItem++;

		}
	}
	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		//strSQL.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
		//strSQL.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
		//strSQL.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");
		////strSQL.AppendFormat(L" where (TestLog.ItemOBD = '4' or TestLog.ItemOBD = '1') and (ResultOfOBD.Judge = '2'/* or ResultOfOBD.Judge = '1'*/)");

		//// ����ɸѡ
		//if (!strPlateNumber.IsEmpty())
		//{
		//	strSQL.AppendFormat(L" and PlateNumber like '%%%s%%'", strPlateNumber);
		//}
		_RecordsetPtr pRecordset(NULL);
		//if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
		//{
		//	try
		//	{
		//		int i(0);
		//		while(!pRecordset->adoEOF)
		//		{
		//			int n(0);
		//			CString str;
		//			_variant_t v;

		//			// ���
		//			str.Format(L"%d", i+1);
		//			int nItemIndex = m_lstVehicle.InsertItem(i, str);

		//			// ��ˮ��
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("RunningNumber"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				str =  (LPCTSTR)(_bstr_t)v;
		//			}
		//			else
		//			{
		//				str = L"";
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);

		//			// ���ƺ���
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateNumber"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				str =  (LPCTSTR)(_bstr_t)v;
		//			}
		//			else
		//			{
		//				str = L"";
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);

		//			// ��������
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateType"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				str =  (LPCTSTR)(_bstr_t)v;
		//			}
		//			else
		//			{
		//				str = L"";
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);

		//			// �������
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("TestType"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				n =  v;
		//			}
		//			else
		//			{
		//				n = 0;
		//			}
		//			switch(n)
		//			{
		//			case 1:{str = L"��̬";} break;
		//			case 2:{str = L"����˲̬";} break;
		//			case 3:{str = L"���ؼ���";} break;
		//			case 4:{str = L"˫����";} break;
		//			case 5:{str = L"��͸�����ɼ���";} break;
		//			case 6:{str = L"��ֽʽ���ɼ���";} break;
		//			case 7:{str = L"ũ�ó����ɼ���";} break;
		//			case 8:{str = L"Ħ�г�����";} break;
		//			case 9:{str = L"Ħ�г�˫����";} break;
		//			default:{str = L"��������";} break;
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);

		//			// ���浥���
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("ReportNumber"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				str =  (LPCTSTR)(_bstr_t)v;
		//			}
		//			else
		//			{
		//				str = L"";
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

		//			// OBD���
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("Judge"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				n =  v;
		//			}
		//			else
		//			{
		//				n = 99;
		//			}
		//			switch(n)
		//			{
		//			case 0:{str = L"δ�ж�";} break;
		//			case 1:{str = L"�ϸ�";} break;
		//			case 2:{str = L"���ϸ�";} break;
		//			default:{str = L"���δ֪";} break;
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);

		//			// OBD���״̬
		//			v.ChangeType(VT_NULL);
		//			CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("ItemOBD"));
		//			if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
		//			{
		//				n =  v;
		//			}
		//			else
		//			{
		//				n = 99;
		//			}
		//			switch(n)
		//			{
		//			case 1:{str = L"����";} break;
		//			case 4:{str = L"���";} break;
		//			default:{str = L"�����";} break;
		//			}
		//			m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, str, 0, 0, 0, 0);

		//			i++;
		//			pRecordset->MoveNext();

		//		}
		//	}
		//	catch (_com_error& e)
		//	{
		//		CNHSQLServerDBO::OutputDBErrMsg(e);
		//	}
		//}
		//else
		//{
		//	CString strMsg;
		//	strMsg.Format(L"��ѯ����Ϊ�ջ���������\r\n���ݿ��ѯ���:%s", strSQL);
		//	m_edMessAge.SetWindowTextW(strMsg);
		//}
		//CNHSQLServerDBO::CloseQuery(pRecordset);

		// ��ȡ�¼����Ա
		strSQL.Format(L"select * from UserInfo /*where Role = 6*/");
		pRecordset = NULL;
		if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
		{
			// �򿪲�ѯ�ɹ�
			try
			{
				while (!pRecordset->adoEOF)
				{
					_variant_t v;
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"Name");
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						m_cbOBDJCY.AddString((wchar_t*)(_bstr_t)v);
					}
					else
					{
						m_cbOBDJCY.AddString(L"");
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
		CNHSQLServerDBO::CloseDB(pConnection);
		m_edMessAge.SetWindowTextW(L"���ݿ���ϵ����");
		return false;
	}

	CNHSQLServerDBO::CloseDB(pConnection);
	return true;
}


void CDRSXLNDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (GetVehicleList())
	{
	}
}


void CDRSXLNDlg::OnBnClickedBtnSetconfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetConfig();

}


void CDRSXLNDlg::OnEnChangeEdPlatenumber()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (!strPlateNumber.IsEmpty())
	{
		GetVehicleList();
	}
}


void CDRSXLNDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edMessAge.SetWindowTextW(L"");
	BeginWaitCursor();
	// ����OBD���Ա������д
	m_strTestType = m_strRunningNumber = L"";
	m_sOBDVehList = SOBDVEHLIST();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		//m_strTestType = m_lstVehicle.GetItemText(nIndex,5);
		m_sOBDVehList.strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		m_sOBDVehList.strPaletNumber = m_lstVehicle.GetItemText(nIndex,2);
		m_sOBDVehList.strPlateType = m_lstVehicle.GetItemText(nIndex,3);
		m_sOBDVehList.strVIN = m_lstVehicle.GetItemText(nIndex,4);
		m_sOBDVehList.strVehType = m_lstVehicle.GetItemText(nIndex,5);
		if (m_sOBDVehList.strVehType.find(L"1") != -1)
		{
			m_strTestType = L"����";
		}
		else
		{
			m_strTestType = L"˲̬";
		}
		// ѡ�г��ƺ���Ե��UP
		if (!m_strRunningNumber.IsEmpty() && !m_strTestType.IsEmpty())
		{
			GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
		}
		else
		{
			m_edMessAge.SetWindowTextW(L"�����־���Ϊ�գ����ⷽ��Ϊ��");
		}
	}
	EndWaitCursor();

	*pResult = 0;
}


void CDRSXLNDlg::OnBnClickedBtnUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���¶�����֮ǰ�ļ�¼
	m_edMessAge.SetWindowTextW(L"");

	if (!m_strRunningNumber.IsEmpty())
	{

		// �ȴ���SQL�����ڴ����ϴ�����
		CString strSource, strCatalog, strUserID, strPassword, strOBDJCY;
		m_edSource.GetWindowTextW(strSource);
		m_edCatalog.GetWindowTextW(strCatalog);
		m_edUserID.GetWindowTextW(strUserID);
		m_edPassword.GetWindowTextW(strPassword);
		m_cbOBDJCY.GetWindowTextW(strOBDJCY);

		if (strSource.IsEmpty() || strCatalog.IsEmpty() 
			|| strUserID.IsEmpty() || strPassword.IsEmpty() 
			|| strOBDJCY.IsEmpty()
			)
		{
			m_edMessAge.SetWindowTextW(L"���ݿ����Ϊ��");
			return;
		}
		else
		{
			wchar_t wchPath[MAX_PATH];
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHModeServerDB.ini", wchPath))
			{
				CSimpleIni si(wchPath);
				// ��д�������ļ�����
				si.SetString(L"Database", L"Source", strSource);
				si.SetString(L"Database", L"Catalog", strCatalog);
				si.SetString(L"Database", L"UserID", strUserID);
				si.SetString(L"Database", L"Password", strPassword);
			}


		}

		CString strIP, strPort, strOBDCZYID, strOBDCZYMM, strDeviceSN;

		m_edServerIP.GetWindowTextW(strIP);
		m_edServerPost.GetWindowTextW(strPort);
		m_edOBDCZYID.GetWindowTextW(strOBDCZYID);
		m_edOBDCZYMM.GetWindowTextW(strOBDCZYMM);
		m_edLineNumber.GetWindowTextW(strDeviceSN);

		if (strIP.IsEmpty() || strPort.IsEmpty() || strDeviceSN.IsEmpty() ||
			strOBDCZYID.IsEmpty() || strOBDCZYMM.IsEmpty() || strDeviceSN.IsEmpty())
		{
			m_edMessAge.SetWindowTextW(L"��������Ϊ��");
			return;
		}

		_ConnectionPtr pConnection(NULL);
		if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
		{
			TESTLOG sTestLogTemp;
			wcscpy_s(sTestLogTemp.wchRunningNumber, m_sOBDVehList.strRunningNumber.c_str());
			wcscpy_s(sTestLogTemp.wchReportNumber, m_sOBDVehList.strRunningNumber.c_str());
			wcscpy_s(sTestLogTemp.wchPlateNumber, m_sOBDVehList.strPaletNumber.c_str());
			wcscpy_s(sTestLogTemp.wchPlateType, m_sOBDVehList.strPlateType.c_str());
			wcscpy_s(sTestLogTemp.wchVIN, m_sOBDVehList.strVIN.c_str());
			wcscpy_s(sTestLogTemp.wchVehicleType, m_sOBDVehList.strVehType.c_str());
			if (m_sOBDVehList.strVehType.find(L"1") != -1)
			{
				wcscpy_s(sTestLogTemp.wchFuelType, L"����");
			}
			else
			{
				wcscpy_s(sTestLogTemp.wchFuelType, L"����");
			}
			SetDboTestLog(sTestLogTemp);
			CString strSQL;
			// �ȴ���TestLog
			strSQL.Format(L"update TestLog set TestState = '0', ItemOBD = '4' WHERE RunningNumber= '%s'", m_strRunningNumber);
			DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);

			// ��ȡOBD����
			strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
			SResultOfOBD sResultOfOBD;
			// �Ȼ�ȡ����ȡ������˵
			GetDboResultOfOBD(strSQL, &sResultOfOBD);
			if (true)
			{
				COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
				COleDateTimeSpan odts;
				odts.SetDateTimeSpan(0, 0, 0, 40);
				COleDateTime odtSartTime = odtCurTime - odts;

				sResultOfOBD.strRunningNumber = m_strRunningNumber;
				sResultOfOBD.strOperator = strOBDJCY.GetString();
				sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
				sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
				// ����
				if (m_strTestType.Find(L"��̬") != -1
					|| m_strTestType.Find(L"˫����") != -1
					|| m_strTestType.Find(L"˲̬") != -1)
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
				// ����
				else if (m_strTestType.Find(L"����") != -1
					|| m_strTestType.Find(L"��͸��") != -1)
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

				// д�����ݿ�
				if (0x00 ==SetDboResultOfOBD(sResultOfOBD))
				{
				}
				else
				{
				}
				// д����ʱ�ļ�
				SetIniResultOfOBD(sResultOfOBD);

				// �ϴ�OBD���
				TESTLOG sTestLog;
				strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
				GetDboTestLog(strSQL, &sTestLog);
				CString strMsg;
				if (UPReport(strIP, strPort, strOBDCZYID, strOBDCZYMM, strDeviceSN, sTestLog, sResultOfOBD, strMsg))
				{
					strMsg = L"�ϴ��ɹ�\r\n" + strMsg;
				}
				else
				{
					strMsg = L"�ϴ�ʧ��\r\n" + strMsg;
				}

				m_edMessAge.SetWindowTextW(strMsg);
			}
		}
		else
		{
			m_edMessAge.SetWindowTextW(L"�����ݿ�ʧ��");
			return;
		}

		// �ϴ��������ݲ���
		if (m_edIntet.GetCheck() == 1)
		{
			TESTLOG sTestLog;
			SResultOfOBD sResultOfOBD;
			// ��ʼ��
			m_sTestLog = sTestLog;
			m_sResultOfOBD = sResultOfOBD;
			CString strSQL;
			CString strMsg(L"�ϴ�����");
			strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
			if (0x01 != GetDboTestLog(strSQL, &sTestLog))
			{
				strMsg.AppendFormat(L"\r\n��ȡ�����־ʧ��");
				m_edMessAge.SetWindowTextW(strMsg);
				return;
			}
			m_sTestLog = sTestLog;
			strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
			if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
			{
				strMsg.AppendFormat(L"\r\n��ȡOBD�������ʧ��");
				m_edMessAge.SetWindowTextW(strMsg);
				return;
			}
			m_sResultOfOBD = sResultOfOBD;
			// �ȴ���TestLog ��Ϊ�����
			strSQL.Format(L"update TestLog set TestState = '0', ItemOBD = '0' WHERE RunningNumber= '%s'", m_strRunningNumber);
			DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);

			GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(TRUE);
			CNHSQLServerDBO::CloseDB(pConnection);
		}
		GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
		CNHSQLServerDBO::CloseDB(pConnection);
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�����־���Ϊ�գ�������˫������б�");
	}
}

CString CDRSXLNDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CDRSXLNDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	if (strFulType == L"����")
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
			case 4:{strEngineCALID = L"0000QSB10900UAES"; strEngineCVN = L"000050D5";} break;
			case 5:{strEngineCALID = L"0034468840270090"; strEngineCVN = L"487D596F";} break;
			case 6:{strEngineCALID = L"0044469440150263"; strEngineCVN = L"89225B2E00000000";} break;
			case 7:{strEngineCALID = L"0064464240190159"; strEngineCVN = L"AE90F315";} break;
			case 8:{strEngineCALID = L"00740R0366UAES"; strEngineCVN = L"000050D5";} break;
			case 9:{strEngineCALID = L"007503A330UAES"; strEngineCVN = L"000050D5";} break;
			case 10:{strEngineCALID = L"00C80R0334GW"; strEngineCVN = L"000050D5";} break;
			case 11:{strEngineCALID = L"00JB0HA234UAES"; strEngineCVN = L"000050D5";} break;
			case 12:{strEngineCALID = L"00MP230421UAES"; strEngineCVN = L"000050D5";} break;
			case 13:{strEngineCALID = L"00MP261016UAES"; strEngineCVN = L"000050D5";} break;
			case 14:{strEngineCALID = L"0356CMR3SC110815"; strEngineCVN = L"B23725E3";} break;
			case 15:{strEngineCALID = L"0356CMR5SC120830"; strEngineCVN = L"E8927CBA";} break;
			case 16:{strEngineCALID = L"03C906014CE1108"; strEngineCVN = L"000050D5";} break;
			case 17:{strEngineCALID = L"03C906022AS4150"; strEngineCVN = L"A5416587";} break;
			case 18:{strEngineCALID = L"03C906022BH9928"; strEngineCVN = L"96F5B3BE";} break;
			case 19:{strEngineCALID = L"03C906022CA4511"; strEngineCVN = L"A7345A7A";} break;
			default : {strEngineCALID = L"03C906022CA4511"; strEngineCVN = L"A7345A7A";} break;
			}
		}
		else if (strOBDType == L"ISO/SAEԤ��")
		{
			srand(time(0));
			int n = rand()%6;
			switch (n)
			{
			case 0: {strEngineCALID = L"10307843"; strEngineCVN = L"0000D42D";} break;
			case 1: {strEngineCALID = L"10294382"; strEngineCVN = L"00003C06";} break;
			case 2: {strEngineCALID = L"10190196"; strEngineCVN = L"000087DE";} break;
			case 3: {strEngineCALID = L"10236164"; strEngineCVN = L"00005C7F";} break;
			default : {strEngineCALID = L"10236164"; strEngineCVN = L"00005C7F";} break;
			}
		}
		else if (strOBDType == L"OBDII-����")
		{
			srand(time(0));
			int n = rand()%6;
			switch (n)
			{
			case 0: {strEngineCALID = L"30CG8001A0C01000"; strEngineCVN = L"4A44F2412C411C65";} break;
			case 1: {strEngineCALID = L"68193991AG"; strEngineCVN = L"39381886";} break;
			case 2: {strEngineCALID = L"2410568200"; strEngineCVN = L"5A4F616B";} break;
			case 3: {strEngineCALID = L"30CL120150C81101"; strEngineCVN = L"BFEB74AB5FA56D64";} break;
			default : {strEngineCALID = L"30CL120150C81101"; strEngineCVN = L"BFEB74AB5FA56D64";} break;
			}
		}
		else if (strOBDType == L"OBD-OBDII")
		{
			srand(time(0));
			int n = rand()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
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
			default : {strEngineCALID = L"32267993"; strEngineCVN = L"1595668E";} break;
			}
		}
		else if (strOBDType == L"JOBD")
		{
			srand(time(0));
			int n = rand()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"43629000A0C01"; strEngineCVN = L"90C7247B2C411C66";} break;
			}
		}
		else if (strOBDType == L"������")
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
			default : {strEngineCALID = L"YM008AT0600300G0"; strEngineCVN = L"2D863958";} break;
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
			case 2: {strEngineCALID = L"LF0L0643UAES"; strEngineCVN = L"000050D5";} break;
			case 3: {strEngineCALID = L"378055A4H640"; strEngineCVN = L"378055A4H640";} break;
			default : {strEngineCALID = L"378055A4H640"; strEngineCVN = L"378055A4H640";} break;
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
		case 14: {strEngineCALID = L"000050D5"; strEngineCVN = L"E0002658F816511F";} break;
		default : {strEngineCALID = L"SCHD13823352AC"; strEngineCVN = L"E0002658F816511F";} break;
		}

	}
}


bool CDRSXLNDlg::UPReport(const CString& strIP, const CString& strPort, const CString& strOBDCZYID, const CString& strOBDCZYMM,
	const CString& strDeviceSN,const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	CString strXML;

	if (!strOBDCZYID.IsEmpty() && !strOBDCZYMM.IsEmpty() && !strIP.IsEmpty() && !strPort.IsEmpty()
		&&!strDeviceSN.IsEmpty())
	{
		// �ϴ�����¼Ա
		strXML.Empty();

		strXML.AppendFormat(L"<Message Device=\"%s\">", strDeviceSN);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVerify");
		strXML.AppendFormat(L"<Row>");	
		strXML.AppendFormat(L"<User>%s</User>", strOBDCZYID);
		strXML.AppendFormat(L"<Pwd>%s</Pwd>", strOBDCZYMM);
		strXML.AppendFormat(L"<LoginType>%s</LoginType>", L"2");
		strXML.AppendFormat(L"</Row>");
		strXML.AppendFormat(L"</Request>");
		strXML.AppendFormat(L"</Message>");

		std::wstring strRecv;
		if (SendAndRecvPacket(strIP, strPort, strXML, strRecv))
		{
			CXmlReader xmlReader;
			std::wstring strResult;

			if (xmlReader.Parse(strRecv.c_str()))
			{
				if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
				{
					xmlReader.GetNodeContent(strResult);
					if (strResult.find(L"1") != std::string::npos)
					{
						bRet = true;
					}
				}
				if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
				{
					xmlReader.GetNodeContent(strResult);
					strMsg.Format(L"%s", strResult.c_str());
				}

			}
			else
			{
				strMsg.Format(L"�ϴ�����¼Ա ����ʧ��");
			}
		}
		else
		{
			strMsg.Format(L"�ϴ�����¼Ա ����ʧ��");
		}

		// �ϴ�����¼Ա�ɹ�
#ifndef _DEBUG
		if (bRet)
#endif
		{
			strXML.Empty();
			CString strVehType(sTestLog.wchFuelType);

			COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
			odtCurTime.ParseDateTime(sResultOfOBD.strDetBegTime.c_str());
			if (strVehType.Find(L"��") != std::string::npos)
			{
				strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, sTestLog.wchReportNumber);
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
				strTemp.Format(L"%d", _wtoi(m_sResultOfOBD.strIUPR_RO2SMCCB2.c_str()));
				strTemp1.Format(L"%d", _wtoi(m_sResultOfOBD.strIUPR_RO2SMCECB2.c_str()));
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
			else
			{
				strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, sTestLog.wchReportNumber);
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
			strXML.AppendFormat(L"</Request></Message>");
			strXML.Replace(L"��֧��",L"0");

			strRecv = L"";
			if (SendAndRecvPacket(strIP, strPort, strXML, strRecv))
			{
				CXmlReader xmlReader;
				std::wstring strResult;

				if (xmlReader.Parse(strRecv.c_str()))
				{
					if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
					{
						xmlReader.GetNodeContent(strResult);
						if (strResult.find(L"1") != std::string::npos)
						{
							bRet = true;
						}
					}
					if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
					{
						xmlReader.GetNodeContent(strResult);
						strMsg.Format(L"%s", strResult.c_str());
					}

				}
				else
				{
					strMsg.Format(L"����ʧ��");
					bRet = false;
				}
			}
			else
			{
				strMsg.Format(L"����ʧ��");
				bRet = false;
			}
		}
	}
	else
	{
		strMsg.Format(L"�ϴ������п�ֵ");
		bRet = false;
	}

	return bRet;
}


void CDRSXLNDlg::OnBnClickedBtnUpTelet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strIP, strPort, strOBDCZYID, strOBDCZYMM, strDeviceSN;

	m_edServerIP.GetWindowTextW(strIP);
	m_edServerPost.GetWindowTextW(strPort);
	m_edOBDCZYID.GetWindowTextW(strOBDCZYID);
	m_edOBDCZYMM.GetWindowTextW(strOBDCZYMM);
	m_edLineNumber.GetWindowTextW(strDeviceSN);

	if (strIP.IsEmpty() || strPort.IsEmpty() || strDeviceSN.IsEmpty() ||
		strOBDCZYID.IsEmpty() || strOBDCZYMM.IsEmpty() || strDeviceSN.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"��������Ϊ��");
		return;
	}

	CString strTestType;
	strTestType.Format(L"%s", m_sTestLog.wchTestType);

	if (strTestType.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"���ݿ�������Ϊ��");
		return;
	}

	CString strMessage;
	CString str;

	str.Empty();
	strMessage.AppendFormat(L"\r\n�ϴ�OBD��������");
	if (UPRealTimeData(strIP, strPort, strOBDCZYID, strOBDCZYMM, strDeviceSN, m_sTestLog, m_sResultOfOBD, str))
	{
		strMessage.AppendFormat(L"\r\n%s", str);
	}
	else
	{
		strMessage.AppendFormat(L"\r\n�ϴ�����\r\n%s", str);
	}
	m_edMessAge.SetWindowTextW(strMessage);

	// OBD �ϴ�����Ϊ���
	if (true)
	{
		CString strSource, strCatalog, strUserID, strPassword;
		m_edSource.GetWindowTextW(strSource);
		m_edCatalog.GetWindowTextW(strCatalog);
		m_edUserID.GetWindowTextW(strUserID);
		m_edPassword.GetWindowTextW(strPassword);

		_ConnectionPtr pConnection(NULL);
		if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
		{
			CString strSQL;
			// �ȴ���TestLog
			strSQL.Format(L"update TestLog ItemOBD = '4' WHERE RunningNumber= '%s'", m_sTestLog.wchRunningNumber);
			DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);
		}
		CNHSQLServerDBO::CloseDB(pConnection);
	}
	// ����ʱ���رհ�ť�Ŀɵ��
	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);
}

int randEx()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}


bool CDRSXLNDlg::UPRealTimeData(const CString& strIP, const CString& strPort, const CString& strOBDCZYID, const CString& strOBDCZYMM,
	const CString& strDeviceSN,const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	CString strXML;
	if (!strOBDCZYID.IsEmpty() && !strOBDCZYMM.IsEmpty() && strIP.IsEmpty() && strPort.IsEmpty()
		&&!strDeviceSN.IsEmpty())
	{
		CString strTestType(sTestLog.wchTestType);
		COleDateTime odtCurTime;
		COleDateTimeSpan odts;
		SYSTEMTIME st;

		// ����
		if (strTestType.Find(L"1") != -1 || strTestType.Find(L"2") != -1 || strTestType.Find(L"4") != -1)
		{
			strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, sTestLog.wchReportNumber);
			strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadGasProcessData");
		}
		// ����
		else
		{
			strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, sTestLog.wchReportNumber);
			strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadDieselProcessData");
		}
		strXML.AppendFormat(L"<SSSJL>");

		float f;
		int n;

		// ��̬
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

				// ʵʱ���������ڵ㣬ÿ��Row�ڵ�����һ����
				strXML.AppendFormat(L"<Row><REALTEMEDATA>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>�����ž��Կ���</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>���㸺��ֵ</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>ǰ���������źŻ��������ϵ��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>����</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>������ת��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>�����������ѹ��</SSSJLNR>");
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

				// ���������ݸ��ڵ㡣
				strXML.AppendFormat(L"<FAULTCODE><Info>");
				strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
				strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
				strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
				strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
			}
		}
		// ����˲̬
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

				// ʵʱ���������ڵ㣬ÿ��Row�ڵ�����һ����
				strXML.AppendFormat(L"<Row><REALTEMEDATA>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>�����ž��Կ���</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>���㸺��ֵ</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>ǰ���������źŻ��������ϵ��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>����</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>������ת��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>�����������ѹ��</SSSJLNR>");
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

				// ���������ݸ��ڵ㡣
				strXML.AppendFormat(L"<FAULTCODE><Info>");
				strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
				strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
				strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
				strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
			}
		}
		// ���ؼ���
		else if (strTestType.Find(L"3") != -1)
		{

			std::vector<SRealTimeDataOfLUGDOWN> vt;
			GetIniRealTimeDataOfLUGDOWNEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				// ʵʱ���������ڵ㣬ÿ��Row�ڵ�����һ����
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
				strXML.AppendFormat(L"<SSSJLNR>���ſ���</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
				strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>�������������</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>kw</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>11001</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>������ת��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>������</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>��ѹѹ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>",f);
				strXML.AppendFormat(L"<SSSJLDW>kPa</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7008</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>������</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>L/100km</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>11002</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����������Ũ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>ppm</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>8305</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����������</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>L/h</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>8505</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>�����¶�</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>��C</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7805</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����������ѹ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>kpa</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7933</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>EGR����</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>6908</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>>ȼ������ѹ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>bar</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>6975</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"</REALTEMEDATA>");

				// ���������ݸ��ڵ㡣
				strXML.AppendFormat(L"<FAULTCODE><Info>");
				strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
				strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
				strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
				strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
			}
		}
		// ˫����
		else if (strTestType.Find(L"4") != -1)
		{
			std::vector<SRealTimeDataOfDIS> vt;
			GetIniRealTimeDataOfDISEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				// ʵʱ���������ڵ㣬ÿ��Row�ڵ�����һ����
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
				strXML.AppendFormat(L"<SSSJLNR>�����ž��Կ���</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>���㸺��ֵ</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>ǰ���������źŻ��������ϵ��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>����</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
				strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>������ת��</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>�����������ѹ��</SSSJLNR>");
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

				// ���������ݸ��ڵ㡣
				strXML.AppendFormat(L"<FAULTCODE><Info>");
				strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
				strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
				strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
				strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
			}
		}
		// ��͸��
		else if (strTestType.Find(L"5") != -1)
		{
			std::vector<SRealTimeDataOfFSUNHT> vt;
			GetIniRealTimeDataOfFSUNHTEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				// ʵʱ���������ڵ㣬ÿ��Row�ڵ�����һ����
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
				strXML.AppendFormat(L"<SSSJLNR>���ſ���</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
				strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>11</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", 0.00f);
				strXML.AppendFormat(L"<SSSJLDW>km/h</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>�������������</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>kw</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>11001</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>������ת��</SSSJLNR>");

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
				strXML.AppendFormat(L"<SSSJLNR>������</SSSJLNR>");
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
				strXML.AppendFormat(L"<SSSJLNR>��ѹѹ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", f);
				strXML.AppendFormat(L"<SSSJLDW>kPa</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7008</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>������</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>L/100km</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>11002</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����������Ũ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>ppm</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>8305</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����������</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>L/h</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>8505</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>�����¶�</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>��C</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7805</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>����������ѹ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>kpa</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>7933</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>EGR����</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>%%</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>6908</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"<Info>");
				strXML.AppendFormat(L"<SSSJLNR>>ȼ������ѹ��</SSSJLNR>");
				strXML.AppendFormat(L"<SSSJLJG>%.2f</SSSJLJG>", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
				strXML.AppendFormat(L"<SSSJLDW>bar</SSSJLDW>");
				strXML.AppendFormat(L"<SSSJLID>6975</SSSJLID>");
				strXML.AppendFormat(L"</Info>");

				strXML.AppendFormat(L"</REALTEMEDATA>");

				// ���������ݸ��ڵ㡣
				strXML.AppendFormat(L"<FAULTCODE><Info>");
				strXML.AppendFormat(L"<BREAKCODE>0</BREAKCODE>");
				strXML.AppendFormat(L"<BREAKkDESC>0</BREAKkDESC>");
				strXML.AppendFormat(L"<BREAKID>0</BREAKID>");
				strXML.AppendFormat(L"</Info></FAULTCODE></Row>");
			}
		}
		// ��ֽʽ
		else if (strTestType.Find(L"6") != -1)
		{
		}
		strXML.AppendFormat(L"</SSSJL></Request></Message>");

		std::wstring strRecv;
		if (SendAndRecvPacket(strIP, strPort, strXML, strRecv))
		{
			CXmlReader xmlReader;
			std::wstring strResult;

			if (xmlReader.Parse(strRecv.c_str()))
			{
				if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
				{
					xmlReader.GetNodeContent(strResult);
					if (strResult.find(L"1") != std::string::npos)
					{
						bRet = true;
					}
				}
				if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
				{
					xmlReader.GetNodeContent(strResult);
					strMsg.Format(L"%s", strResult.c_str());
				}

			}
			else
			{
				strMsg.Format(L"�ϴ�����¼Ա ����ʧ��");
			}
		}
		else
		{
			strMsg.Format(L"�ϴ�����¼Ա ����ʧ��");
		}
	}
	else
	{
		strMsg.Format(L"�ϴ������п�ֵ");
	}

	return bRet;
}