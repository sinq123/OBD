
// SXDRSQLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SXDRSQL.h"
#include "SXDRSQLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSXDRSQLDlg �Ի���




CSXDRSQLDlg::CSXDRSQLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSXDRSQLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSXDRSQLDlg::~CSXDRSQLDlg()
{
	CloaseScket();
}

void CSXDRSQLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMessage);
	DDX_Control(pDX, ID_ED_PlateNumber, m_edPlateNumber);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassword);

	DDX_Control(pDX, ID_ED_SERVERIP, m_edServerIP);
	DDX_Control(pDX, ID_ED_SERVERPORT, m_edServerPost);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJCY);


	DDX_Control(pDX, ID_ED_OBDCZYMM, m_edOBDCZYMM);
	DDX_Control(pDX, ID_ED_OBDCZYID, m_edOBDCZYID);
}

BEGIN_MESSAGE_MAP(CSXDRSQLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CSXDRSQLDlg::OnBnClickedButtonUpdateVehicleList)
	ON_EN_CHANGE(ID_ED_PlateNumber, &CSXDRSQLDlg::OnEnChangeEdPlatenumber)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CSXDRSQLDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CSXDRSQLDlg::OnBnClickedBtnUpinter)
END_MESSAGE_MAP()


// CSXDRSQLDlg ��Ϣ�������

BOOL CSXDRSQLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(L"����");

	SetConfig();
	SetVehCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSXDRSQLDlg::OnPaint()
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
HCURSOR CSXDRSQLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSXDRSQLDlg::SetConfig(void)
{
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

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

	m_edMessage.SetWindowTextW(L"��ȡ�����ļ��ɹ�");
}

void CSXDRSQLDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(1, L"��ˮ��",		0,	220/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(2, L"���ƺ���",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(3, L"��������",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(4, L"VIN",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(5, L"�������",	0,	120/**nSM_CXSCREEN/1280*/);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);

	m_edMessage.SetWindowTextW(L"�б����óɹ�");
}

bool CSXDRSQLDlg::GetVehicleList(void)
{
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

	_ConnectionPtr pConnection(NULL);

	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		strSQL.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, TestLog.VIN, TestLog.VehicleType, ResultOfOBD.Judge");
		strSQL.AppendFormat(L" from TestLog left join ResultOfOBD on TestLog.RunningNumber = ResultOfOBD.RunningNumber ");
		strSQL.AppendFormat(L" where ((TestLog.ItemOBD = '4'  and  ResultOfOBD.Judge = '2') or TestLog.ItemOBD = '1')");
		//strSQL.AppendFormat(L" where (TestLog.ItemOBD = '4' or TestLog.ItemOBD = '1') and (ResultOfOBD.Judge = '2'/* or ResultOfOBD.Judge = '1'*/)");

		// ����ɸѡ
		if (!strPlateNumber.IsEmpty())
		{
			strSQL.AppendFormat(L" and PlateNumber like '%%%s%%'", strPlateNumber);
		}

		_RecordsetPtr pRecordset(NULL);

		if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
		{
			try
			{
				m_edMessage.SetWindowTextW(L"��ȡ�ɹ�");

				int i(0);

				while(!pRecordset->adoEOF)
				{
					int n(0);
					CString str;
					_variant_t v;

					// ���
					str.Format(L"%d", i+1);
					int nItemIndex = m_lstVehicle.InsertItem(i, str);

					// ��ˮ��
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("RunningNumber"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						str =  (LPCTSTR)(_bstr_t)v;
					}
					else
					{
						str = L"";
					}
					m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);

					// ���ƺ���
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateNumber"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						str =  (LPCTSTR)(_bstr_t)v;
					}
					else
					{
						str = L"";
					}
					m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);

					// ��������
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("PlateType"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						str =  (LPCTSTR)(_bstr_t)v;
					}
					else
					{
						str = L"";
					}
					m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);

					// Vin
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("VIN"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						str =  (LPCTSTR)(_bstr_t)v;
					}
					else
					{
						str = L"";
					}
					m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);

					// VehicleType
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("VehicleType"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						str =  (LPCTSTR)(_bstr_t)v;
					}
					else
					{
						str = L"";
					}
					m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

					i++;
					pRecordset->MoveNext();

				}

			}
			catch (_com_error& e)
			{
				CNHSQLServerDBO::OutputDBErrMsg(e);
			}
		}
	}
	else
	{
		CNHSQLServerDBO::CloseDB(pConnection);
		MessageBox(L"�����ݿ�ʧ��");
		return false;
	}

	CNHSQLServerDBO::CloseDB(pConnection);
	return true;

}

void CSXDRSQLDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetVehicleList();
}


void CSXDRSQLDlg::OnEnChangeEdPlatenumber()
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


void CSXDRSQLDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ����OBD���Ա������д

	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		// ѡ�г��ƺ���Ե��UP
		if (!m_strRunningNumber.IsEmpty())
		{
			m_edMessage.SetWindowTextW(L"����д�ò��������UP");
			GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
		}
	}

	EndWaitCursor();

	*pResult = 0;
}


void CSXDRSQLDlg::OnBnClickedBtnUpinter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_strRunningNumber != L"")
	{
		TESTLOG sTestLogTemp;
		SResultOfOBD sResultOfOBD;
		
		CString strSource, strCatalog, strUserID, strPassword;
		m_edSource.GetWindowTextW(strSource);
		m_edCatalog.GetWindowTextW(strCatalog);
		m_edUserID.GetWindowTextW(strUserID);
		m_edPassword.GetWindowTextW(strPassword);

		// ��ȡ���ݿ����
		if (!strSource.IsEmpty() && !strCatalog.IsEmpty() 
			&& !strUserID.IsEmpty() && !strPassword.IsEmpty())
		{
			CString strSQL;

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

			strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
			if (0x01 != GetDboTestLog(strSQL, &sTestLogTemp))
			{
				m_edMessage.SetWindowTextW(L"��ȡ�����־ʧ��");
				return;
			}
		}
		else
		{
			m_edMessage.SetWindowTextW(L"���ݿ�����п�ֵ");
			return;
		}


		CString cstrOBDJCY(L"");
		m_edOBDJCY.GetWindowTextW(cstrOBDJCY);

		CString strDeviceSN;
		m_edLineNumber.GetWindowTextW(strDeviceSN);

		CString cstrResult;


		CString cstrOBDCZYID, cstrOBDCZYMM;
		m_edOBDCZYID.GetWindowTextW(cstrOBDCZYID);
		m_edOBDCZYMM.GetWindowTextW(cstrOBDCZYMM);

		if (!cstrOBDCZYID.IsEmpty() && !cstrOBDCZYMM.IsEmpty())
		{
			CString strXML1;

			strXML1.AppendFormat(L"<Message Device=\"%s\">", strDeviceSN);
			strXML1.AppendFormat(L"<Request Name=\"%s\">", L"OBDVerify");
			strXML1.AppendFormat(L"<Row>");	
			strXML1.AppendFormat(L"<User>%s</User>", cstrOBDCZYID);
			strXML1.AppendFormat(L"<Pwd>%s</Pwd>", cstrOBDCZYMM);
			strXML1.AppendFormat(L"<LoginType>%s</LoginType>", L"2");
			strXML1.AppendFormat(L"</Row>");
			strXML1.AppendFormat(L"</Request>");
			strXML1.AppendFormat(L"</Message>");

			std::wstring strRecv;
			if (SendAndRecvPacket(strXML1, strRecv))
			{
				CXmlReader xmlReader;
				std::wstring strResult;
				if (xmlReader.Parse(strRecv.c_str()))
				{
					if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
					{
						xmlReader.GetNodeContent(strResult);
						cstrResult = strResult.c_str();
					}
					if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
					{
						//xmlReader.GetNodeContent(strResult);
					}
				}
			}

		}
		else
		{
			m_edMessage.SetWindowTextW(L"����ԱID������Ϊ��");
			return;
		}

		if (!(cstrResult == L"1"))
		{
			m_edMessage.SetWindowTextW(L"OBD����¼");
			return;
		}
		cstrResult = L"";
		if (!cstrOBDJCY.IsEmpty() && !strDeviceSN.IsEmpty())
		{

			COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
			COleDateTimeSpan odts;
			odts.SetDateTimeSpan(0, 0, 0, 40);
			COleDateTime odtSartTime = odtCurTime - odts;

			CString strXML;
			CString strVehType(sTestLogTemp.wchFuelType);

			if (strVehType.Find(L"��") != -1)
			{
				strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, sTestLogTemp.wchReportNumber);
				strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadGasResult");
				strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");
				strXML.AppendFormat(L"<Result><Row><Info>");
				strXML.AppendFormat(L"<VIN>%s</VIN>", sTestLogTemp.wchVIN);
				strXML.AppendFormat(L"<XSJYOBDYQ>%s</XSJYOBDYQ>", L"EOBD");
				strXML.AppendFormat(L"<LJXSLC>%s</LJXSLC>", L"0");
				strXML.AppendFormat(L"<FDJKZCALID>%s</FDJKZCALID>", L"��֧��");
				strXML.AppendFormat(L"<FDJKZCVN>%s</FDJKZCVN>", L"��֧��");
				strXML.AppendFormat(L"<HCLKZCALID>%s</HCLKZCALID>", L"");
				strXML.AppendFormat(L"<HCLKZCVN>%s</HCLKZCVN>", L"");
				strXML.AppendFormat(L"<QTKZCALID>%s</QTKZCALID>", L"");
				strXML.AppendFormat(L"<QTKZCVN>%s</QTKZCVN>", L"");
				strXML.AppendFormat(L"<JCSJ>%s</JCSJ>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
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

				strXML.AppendFormat(L"<IUPR><Row><Info>");
				strXML.AppendFormat(L"<CHQZ1WCCS>%s</CHQZ1WCCS>",L"0");
				strXML.AppendFormat(L"<CHQZ1FHCS>%s</CHQZ1FHCS>",L"0");
				strXML.AppendFormat(L"<CHQZ1IUPRL>%s</CHQZ1IUPRL>",L"0.00");
				strXML.AppendFormat(L"<CHQZ2WCCS>%s</CHQZ2WCCS>",L"0");
				strXML.AppendFormat(L"<CHQZ2FHCS>%s</CHQZ2FHCS>",L"0");
				strXML.AppendFormat(L"<CHQZ2IUPRL>%s</CHQZ2IUPRL>",L"0.00");
				strXML.AppendFormat(L"<QYCGQZ1WCCS>%s</QYCGQZ1WCCS>",L"0");
				strXML.AppendFormat(L"<QYCGQZ1FHCS>%s</QYCGQZ1FHCS>",L"0");
				strXML.AppendFormat(L"<QYCGQZ1IUPRL>%s</QYCGQZ1IUPRL>",L"0.00");
				strXML.AppendFormat(L"<QYCGQZ2WCCS>%s</QYCGQZ2WCCS>",L"0");
				strXML.AppendFormat(L"<QYCGQZ2FHCS>%s</QYCGQZ2FHCS>",L"0");
				strXML.AppendFormat(L"<QYCGQZ2IUPRL>%s</QYCGQZ2IUPRL>",L"0.00");
				strXML.AppendFormat(L"<HYCGQZ1WCCS>%s</HYCGQZ1WCCS>",L"0");
				strXML.AppendFormat(L"<HYCGQZ1FHCS>%s</HYCGQZ1FHCS>",L"0");
				strXML.AppendFormat(L"<HYCGQZ1IUPRL>%s</HYCGQZ1IUPRL>",L"0");
				strXML.AppendFormat(L"<HYCGQZ2WCCS>%s</HYCGQZ2WCCS>",L"0");
				strXML.AppendFormat(L"<HYCGQZ2FHCS>%s</HYCGQZ2FHCS>",L"0");
				strXML.AppendFormat(L"<HYCGQZ2IUPRL>%s</HYCGQZ2IUPRL>",L"0");
				strXML.AppendFormat(L"<EVAPWCCS>%s</EVAPWCCS>",L"0");
				strXML.AppendFormat(L"<EVAPFHCS>%s</EVAPFHCS>",L"0");
				strXML.AppendFormat(L"<EVAPIUPRL>%s</EVAPIUPRL>",L"0.00");
				strXML.AppendFormat(L"<EGRHVVTWCCS>%s</EGRHVVTWCCS>",L"0");
				strXML.AppendFormat(L"<EGRHVVTFHCS>%s</EGRHVVTFHCS>",L"0");
				strXML.AppendFormat(L"<EGRHVVTIUPRL>%s</EGRHVVTIUPRL>",L"0.00");
				strXML.AppendFormat(L"<GPFZ1WCCS>%s</GPFZ1WCCS>",L"0");
				strXML.AppendFormat(L"<GPFZ1FHCS>%s</GPFZ1FHCS>",L"0");
				strXML.AppendFormat(L"<GPFZ1IUPRL>%s</GPFZ1IUPRL>",L"0.00");
				strXML.AppendFormat(L"<GPFZ2WCCS>%s</GPFZ2WCCS>",L"0");
				strXML.AppendFormat(L"<GPFZ2FHCS>%s</GPFZ2FHCS>",L"0");
				strXML.AppendFormat(L"<GPFZ2IUPRL>%s</GPFZ2IUPRL>",L"0.00");
				strXML.AppendFormat(L"<ECKQPSWCCS>%s</ECKQPSWCCS>",L"0");
				strXML.AppendFormat(L"<ECKQPSFHCS>%s</ECKQPSFHCS>",L"0");
				strXML.AppendFormat(L"<ECKQPSIUPRL>%s</ECKQPSIUPRL>",L"0.00");
				strXML.AppendFormat(L"</Info></Row></IUPR>");
			}
			else
			{
				strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, sTestLogTemp.wchReportNumber);
				strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadDieselResult");
				strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");

				strXML.AppendFormat(L"<Result><Row><Info>");
				strXML.AppendFormat(L"<VIN>%s</VIN>",sTestLogTemp.wchVIN);
				strXML.AppendFormat(L"<XSJYOBDYQ>%s</XSJYOBDYQ>", L"OBDII-����");
				strXML.AppendFormat(L"<LJXSLC>%s</LJXSLC>", L"0");
				strXML.AppendFormat(L"<FDJKZCALID>%s</FDJKZCALID>", L"��֧��");
				strXML.AppendFormat(L"<FDJKZCVN>%s</FDJKZCVN>", L"��֧��");
				strXML.AppendFormat(L"<HCLKZCALID>%s</HCLKZCALID>", L"");
				strXML.AppendFormat(L"<HCLKZCVN>%s</HCLKZCVN>", L"");
				strXML.AppendFormat(L"<QTKZCALID>%s</QTKZCALID>", L"");
				strXML.AppendFormat(L"<QTKZCVN>%s</QTKZCVN>", L"");
				strXML.AppendFormat(L"<JCSJ>%s</JCSJ>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
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

				strXML.AppendFormat(L"<IUPR><Row><Info>");
				strXML.AppendFormat(L"<NMHCWCCS>%s</NMHCWCCS>",L"0");
				strXML.AppendFormat(L"<NMHCFHCS>%s</NMHCFHCS>",L"0");
				strXML.AppendFormat(L"<NMHCIUPRL>%s</NMHCIUPRL>",L"0.00");
				strXML.AppendFormat(L"<NOXCHQWCCS>%s</NOXCHQWCCS>",L"0");
				strXML.AppendFormat(L"<NOXCHQFHCS>%s</NOXCHQFHCS>",L"0");
				strXML.AppendFormat(L"<NOXCHQIUPRL>%s</NOXCHQIUPRL>",L"0.00");
				strXML.AppendFormat(L"<NOXXFQWCCS>%s</NOXXFQWCCS>",L"0");
				strXML.AppendFormat(L"<NOXXFQFHCS>%s</NOXXFQFHCS>",L"0");
				strXML.AppendFormat(L"<NOXXFQIUPRL>%s</NOXXFQIUPRL>",L"0.00");
				strXML.AppendFormat(L"<PMWCCS>%s</PMWCCS>",L"0");
				strXML.AppendFormat(L"<PMFHCS>%s</PMFHCS>",L"0");
				strXML.AppendFormat(L"<PMIUPRL>%s</PMIUPRL>",L"0.00");
				strXML.AppendFormat(L"<FQCGQWCCS>%s</FQCGQWCCS>",L"0");
				strXML.AppendFormat(L"<FQCGQFHCS>%s</FQCGQFHCS>",L"0");
				strXML.AppendFormat(L"<FQCGQIUPRL>%s</FQCGQIUPRL>",L"0.00");
				strXML.AppendFormat(L"<EGRHVVTWCCS>%s</EGRHVVTWCCS>",L"0");
				strXML.AppendFormat(L"<EGRHVVTFHCS>%s</EGRHVVTFHCS>",L"0");
				strXML.AppendFormat(L"<EGRHVVTIUPRL>%s</EGRHVVTIUPRL>",L"0.00");
				strXML.AppendFormat(L"<ZYYLWCCS>%s</ZYYLWCCS>",L"0");
				strXML.AppendFormat(L"<ZYYLFHCS>%s</ZYYLFHCS>",L"0");
				strXML.AppendFormat(L"<ZYYLIUPRL>%s</ZYYLIUPRL>",L"0.00");
				strXML.AppendFormat(L"</Info></Row></IUPR>");
			}
			strXML.AppendFormat(L"</Request></Message>");
			strXML.Replace(L"��֧��",L"0");

			std::wstring strRecv;
			if (SendAndRecvPacket(strXML, strRecv))
			{
				CXmlReader xmlReader;
				std::wstring strResult, strMsg;
				if (xmlReader.Parse(strRecv.c_str()))
				{
					if (xmlReader.OpenNode(L"Message/Respond/Row/Result"))
					{
						xmlReader.GetNodeContent(strResult);
						cstrResult = strResult.c_str();
					}
					if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
					{
						xmlReader.GetNodeContent(strMsg);
					}
					if (strResult == L"1")
					{
						m_edMessage.SetWindowTextW(L"�ϴ��ɹ�");
					}
					else
					{
						CString cstrMsg;
						cstrMsg.Format(L"�ϴ�ʧ��\r\n%s", strMsg.c_str());
						m_edMessage.SetWindowTextW(cstrMsg);
						return;
					}
				}
			}
			else
			{
				m_edMessage.SetWindowTextW(L"����ʧ��");
				return;
			}
		}
		else
		{
			m_edMessage.SetWindowTextW(L"�����ԱΪ��\r\n�����Ϊ��");
			return;
		}

		// �ϴ��ɹ��Ų������ݿ�
		if (cstrResult == L"1")
		{
			_ConnectionPtr pConnection(NULL);
			if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
			{
				CString strSQL;
				// �ȴ���TestLog
				strSQL.Format(L"update TestLog set TestState = '0', ItemOBD = '4' WHERE RunningNumber= '%s'", m_strRunningNumber);
				DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);

				// ��ȡOBD����
				strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
				SResultOfOBD sResultOfOBD;

				if (true)
				{
					CString strTestType(sTestLogTemp.wchTestType);
					sResultOfOBD.strRunningNumber = m_strRunningNumber;
					// ����
					if (strTestType.Find(L"1") != -1
						|| strTestType.Find(L"2") != -1
						|| strTestType.Find(L"4") != -1)
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
					// ����
					else if (strTestType.Find(L"3") != -1
						|| strTestType.Find(L"5") != -1)
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

					// д�����ݿ�
					if (0x00 ==SetDboResultOfOBD(sResultOfOBD))
					{
						// �ɹ�
					}
					// д����ʱ�ļ�
					SetIniResultOfOBD(sResultOfOBD);

					if (true)
					{
						m_edPlateNumber.SetWindowTextW(L"");
						GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
						GetVehicleList();
					}
				}

			}
		}

	}
	else
	{
		m_edMessage.SetWindowTextW(L"��־���Ϊ��");
		return;
	}
	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
}

bool CSXDRSQLDlg::SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv)
{

	CString strXML = szXML;
	strXML.Replace(L" ", L"%20");	// ת��
	strXML.Replace(L"\r", L"%0d");	// ת��
	strXML.Replace(L"\n", L"%0a");	// ת��

	CString strSend;
	CString strURL(L"/CtrlCenter/ASM");
	strSend.Format(L" %s?data=%s\r\n",strURL, strXML);	// ��ַǰ�����пո�

	CNSSocket nsSocket;

	CString strIP, strPort;
	m_edServerIP.GetWindowTextW(strIP);
	m_edServerPost.GetWindowTextW(strPort);
	int nPort = _wtoi(strPort);

	nsSocket.Open(strIP, nPort);
	return nsSocket.SendAndRecvPacket(strSend, strRecv);

}


bool CSXDRSQLDlg::CloaseScket(void)
{
	// �˳��ǶϿ�����
	CNSSocket nsSocket;
	return nsSocket.Close();
}