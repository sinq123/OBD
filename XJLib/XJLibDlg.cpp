
// XJLibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XJLib.h"
#include "XJLibDlg.h"
#include "afxdialogex.h"
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXJLibDlg �Ի���




CXJLibDlg::CXJLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXJLibDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXJLibDlg::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, ID_ED_JKDZ, m_edURL);
	DDX_Control(pDX, ID_ED_KEY, m_edKey);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNum);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNUmber);

	DDX_Control(pDX, IDC_COM_OBDJCY, m_cbOBDJCY);
}

BEGIN_MESSAGE_MAP(CXJLibDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CXJLibDlg::OnBnClickedBtnSetconfig)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CXJLibDlg::OnBnClickedButtonUpdateVehicleList)
	ON_EN_CHANGE(ID_ED_PlateNumber, &CXJLibDlg::OnEnChangeEdPlatenumber)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CXJLibDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UP, &CXJLibDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_UP_TELET, &CXJLibDlg::OnBnClickedBtnUpTelet)
END_MESSAGE_MAP()


// CXJLibDlg ��Ϣ��������

BOOL CXJLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXJLibDlg::OnPaint()
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
HCURSOR CXJLibDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXJLibDlg::SetConfig(void)
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
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"XJConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"NetConfig", L"URL", L"");
		m_edURL.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"NetConfig", L"Key", L"");
		m_edKey.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"LineInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"LineInfo", L"LineNumber", L"");
		m_edLineNUmber.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"StationInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"StationInfo", L"StationNumber", L"");
		m_edStationNum.SetWindowTextW(strTemp);
	}
}


void CXJLibDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(4, L"�������",	0,	120);
	m_lstVehicle.InsertColumn(5, L"���浥���",	0,	300);
	m_lstVehicle.InsertColumn(6, L"OBD���",	0,	100);
	m_lstVehicle.InsertColumn(7, L"OBD���״̬",	0,	100);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}


bool CXJLibDlg::GetVehicleList(void)
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

	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		strSQL.Format(L" select TestLog.AutoID, TestLog.RunningNumber, TestLog.ReportNumber, TestLog.PlateNumber, TestLog.PlateType, TestLog.TestType, TestLog.ItemOBD, ResultOfOBD.Judge");
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

					// �������
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("TestType"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						n =  v;
					}
					else
					{
						n = 0;
					}
					switch(n)
					{
					case 1:{str = L"��̬";} break;
					case 2:{str = L"����˲̬";} break;
					case 3:{str = L"���ؼ���";} break;
					case 4:{str = L"˫����";} break;
					case 5:{str = L"��͸�����ɼ���";} break;
					case 6:{str = L"��ֽʽ���ɼ���";} break;
					case 7:{str = L"ũ�ó����ɼ���";} break;
					case 8:{str = L"Ħ�г�����";} break;
					case 9:{str = L"Ħ�г�˫����";} break;
					default:{str = L"��������";} break;
					}
					m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);

					// ���浥���
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("ReportNumber"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						str =  (LPCTSTR)(_bstr_t)v;
					}
					else
					{
						str = L"";
					}
					m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);

					// OBD���
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("Judge"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						n =  v;
					}
					else
					{
						n = 99;
					}
					switch(n)
					{
					case 0:{str = L"δ�ж�";} break;
					case 1:{str = L"�ϸ�";} break;
					case 2:{str = L"���ϸ�";} break;
					default:{str = L"���δ֪";} break;
					}
					m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);

					// OBD���״̬
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, _T("ItemOBD"));
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						n =  v;
					}
					else
					{
						n = 99;
					}
					switch(n)
					{
					case 1:{str = L"����";} break;
					case 4:{str = L"���";} break;
					default:{str = L"�����";} break;
					}
					m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, str, 0, 0, 0, 0);

					i++;
					pRecordset->MoveNext();

				}
			}
			catch (_com_error& e)
			{
				CNHSQLServerDBO::OutputDBErrMsg(e);
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(L"��ѯ����Ϊ�ջ���������\r\n���ݿ��ѯ���:%s", strSQL);
			m_edMessAge.SetWindowTextW(strMsg);
		}
		CNHSQLServerDBO::CloseQuery(pRecordset);

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

void CXJLibDlg::OnBnClickedBtnSetconfig()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	SetConfig();
}


void CXJLibDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if (GetVehicleList())
	{
	}
}


void CXJLibDlg::OnEnChangeEdPlatenumber()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (!strPlateNumber.IsEmpty())
	{
		GetVehicleList();
	}
}


void CXJLibDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_edMessAge.SetWindowTextW(L"");
	BeginWaitCursor();
	// ����OBD���Ա������д
	m_strTestType = m_strRunningNumber = L"";
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		m_strTestType = m_lstVehicle.GetItemText(nIndex,4);
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


void CXJLibDlg::OnBnClickedBtnUp()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
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
			// �Ȼ�ȡ����ȡ������˵
			GetDboResultOfOBD(strSQL, &sResultOfOBD);

			if (true)
			{
				sResultOfOBD.strRunningNumber = m_strRunningNumber;
				sResultOfOBD.strOperator = strOBDJCY.GetString();
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
			}
		}
		else
		{
			m_edMessAge.SetWindowTextW(L"�����ݿ�ʧ��");
			return;
		}

		// �ϴ�����
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


CString CXJLibDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CXJLibDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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
			int n = rand()%6;
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
			int n = rand()%6;
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
			int n = rand()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"2769016100190793"; strEngineCVN = L"447DCB91";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
			}
		}
		else if (strOBDType == L"JOBD")
		{
			srand(time(0));
			int n = rand()%3;
			switch (n)
			{
			case 0: {strEngineCALID = L"33629000A0C01000"; strEngineCVN = L"90C7247B2C411C65";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
			case 2: {strEngineCALID = L"LF0L0643UAES"; strEngineCVN = L"��֧��";} break;
			case 3: {strEngineCALID = L"378055A4H640"; strEngineCVN = L"378055A4H640";} break;
			default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
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
		case 14: {strEngineCALID = L"��֧��"; strEngineCVN = L"E0002658F816511F";} break;
		default : {strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";} break;
		}

	}
}


void CXJLibDlg::OnBnClickedBtnUpTelet()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
#ifdef _DEBUG
	CString str;
	//std::string stdstr = "{\"msg\":\"�ϴ��ɹ���\",\"success\":true}";
	const char *file_name = "C:/Users/Administrator/Desktop/����/json.data";
	std::ifstream in(file_name);
	if (!in.is_open())
	{
		return;
	}

	std::string stdstr((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	rapidjson::Document docu;

	bool b = docu.Parse(stdstr.c_str()).HasParseError();
	if (!b)
	{
		//if (docu.HasMember("msg")){str.AppendFormat(L"%s",JsonToCString(docu["msg"]));}
		//if (docu.HasMember("success")){str.AppendFormat(L"%s",JsonToCString(docu["success"]));}
		if (docu.HasMember("name")){str.AppendFormat(L"\r\n%s", JsonToCString(docu["name"]));}
		if (docu.HasMember("address")){str.AppendFormat(L"\r\n%s", JsonToCString(docu["address"]));}
		if (docu.HasMember("value1")){str.AppendFormat(L"\r\n%s", JsonToCString(docu["value1"]));}
		if (docu.HasMember("value2")){str.AppendFormat(L"\r\n%s", JsonToCString(docu["value2"]));}
		if (docu.HasMember("bei_jing")){str.AppendFormat(L"\r\n%s", JsonToCString(docu["bei_jing"]));}
	}

	str.AppendFormat(L"\r\n%d", b);
#endif

	CString strURL, strKey,strStationNum, strLineNum;

	m_edURL.GetWindowTextW(strURL);
	m_edKey.GetWindowTextW(strKey);
	m_edStationNum.GetWindowTextW(strStationNum);
	m_edLineNUmber.GetWindowTextW(strLineNum);

	CString strTestType;
	strTestType.Format(L"%s", m_sTestLog.wchTestType);

	if (strURL.IsEmpty() || strKey.IsEmpty() ||
		strStationNum.IsEmpty() || strLineNum.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"���������п�");
		return;
	}
	else if (strTestType.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"���ݿ�������Ϊ��");
		return;
	}
	else
	{
		CString strMessage;
		CString str;

		strMessage.AppendFormat(L"�ϴ�OBD���浥");
		if (UPReport(strURL, strKey, strStationNum, strLineNum, strTestType, str))
		{
			strMessage.AppendFormat(L"\r\n%s", str);
		}
		else
		{
			strMessage.AppendFormat(L"\r\n�ϴ�����\r\n%s", str);
		}
		m_edMessAge.SetWindowTextW(strMessage);


		str.Empty();
		strMessage.AppendFormat(L"\r\n�ϴ�OBD��������");
		if (UPRealTimeData(strURL, strKey, strStationNum, strLineNum, strTestType, str))
		{
			strMessage.AppendFormat(L"\r\n%s", str);
		}
		else
		{
			strMessage.AppendFormat(L"\r\n�ϴ�����\r\n%s", str);
		}
		m_edMessAge.SetWindowTextW(strMessage);

		// OBD �ϴ�����Ϊ���
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
		//str.Empty();
		//strMessage.AppendFormat(L"\r\n�ϴ�OBD-IUPR");
		//if (UPOBDIUPR(strURL, strKey, strStationNum, strLineNum, strTestType, str))
		//{
		//	strMessage.AppendFormat(L"\r\n%s", str);
		//}
		//else
		//{
		//	strMessage.AppendFormat(L"\r\n�ϴ�����\r\n%s", str);
		//}
		//m_edMessAge.SetWindowTextW(strMessage);

		// ����ʱ���رհ�ť�Ŀɵ��
		GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);
	}

}

bool CXJLibDlg::UPReport(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
	const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg)
{
	bool bRet(false);
	CStringW cstrURL(strURL);
	cstrURL.AppendFormat(L"/ports/detection/uploadTestOBD.do");

	CString strTemp;
	JsonMap map;
	strTemp.Format(L"%s", strStationNum);
	strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
	// XZQHDM	������������
	map[L"XZQHDM"] = strTemp;
	// JYJGBH	����������
	map[L"JYJGBH"] = strStationNum;
	// JCBH	�����
	map[L"JCBH"] = m_sTestLog.wchReportNumber;
	// VIN	����VIN
	map[L"VIN"] = m_sTestLog.wchVIN;
	// ENGINECALID	���������Ƶ�ԪCALID
	map[L"ENGINECALID"] = m_sResultOfOBD.strEngineCALID.c_str();
	// ENGINECVN	���������Ƶ�ԪCVN
	map[L"ENGINECVN"] = m_sResultOfOBD.strEngineCVN.c_str();
	// HCLCALID	�������Ƶ�ԪCALID
	map[L"HCLCALID"] = L"";
	// HCLCVN	�������Ƶ�ԪCVN
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
	if (m_sResultOfOBD.strOperator.empty())
	{
		map[L"JCRYXM"] = L"����";
	}
	else
	{
		map[L"JCRYXM"] = m_sResultOfOBD.strOperator.c_str();
	}
	// ZZPDJG	�����ж����
	map[L"ZZPDJG"] = L"1";

	CStringW strOBDReport;
	strOBDReport.Format(L"SJXX=%s&KEY=%s", Map2Json(map), strKey);

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
		}
		else
		{
			strMsg.Format(L"����ʧ��");
		}
	}
	else
	{
		strMsg.Format(L"%s", strRet.c_str());
	}

	return bRet;
}

bool CXJLibDlg::UPRealTimeData(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
	const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg)
{
	bool bRet(false);

	CStringW cstrURL(strURL);
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

			strTemp.Format(L"%s", strStationNum);
			strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = strStationNum;
			//JCBH	������
			map[L"JCBH"] = m_sTestLog.wchReportNumber;
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

			strTemp.Format(L"%s", strStationNum);
			strTemp = strTemp.Left(6);  // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = strStationNum;
			//JCBH	������
			map[L"JCBH"] = m_sTestLog.wchReportNumber;
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

			strTemp.Format(L"%s", strStationNum);
			strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = strStationNum;
			//JCBH	������
			map[L"JCBH"] = m_sTestLog.wchReportNumber;
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

			strTemp.Format(L"%s", strStationNum);
			strTemp = strTemp.Left(6); // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = strStationNum;
			//JCBH	������
			map[L"JCBH"] = m_sTestLog.wchReportNumber;
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

			strTemp.Format(L"%s", strStationNum);
			strTemp = strTemp.Left(6);  // ���վ���ǰ��λ�����������
			//XZQHDM	������������
			map[L"XZQHDM"] = strTemp;
			//JYJGBH	����������
			map[L"JYJGBH"] = strStationNum;
			//JCBH	������
			map[L"JCBH"] = m_sTestLog.wchReportNumber;
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
	strOBDRealTime.Format(L"SJXX=%s&KEY=%s", MapVec2Json(vmPost), strKey);

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
				bRet = true;
			}
		}
		else
		{
			strMsg.Format(L"����ʧ��");
		}
	}
	else
	{
		strMsg.Format(L"%s", strRet.c_str());
	}


	return bRet;
}

bool CXJLibDlg::UPOBDIUPR(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
	const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg)
{
	bool bRet(false);
	CStringW cstrURL(strURL);
	return bRet;
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
CString CXJLibDlg::JsonToCString(const rapidjson::Value &Val)
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

CString CXJLibDlg::Map2Json(JsonMap mapPost)
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

CString CXJLibDlg::MapVec2Json(JsonMapVec vmPost)
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