
// SGOBDDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SGOBD.h"
#include "SGOBDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSGOBDDlg �Ի���




CSGOBDDlg::CSGOBDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSGOBDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSGOBDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassword);
	DDX_Control(pDX, ID_ED_PlateNumber, m_edPlateNumber);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMessAge);

	DDX_Control(pDX, ID_ED_JKDZ, m_edURL);
	DDX_Control(pDX, ID_ED_KEY, m_edKey);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNum);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNUmber);

	DDX_Control(pDX, IDC_COM_OBDJCY, m_cbOBDJCY);

	DDX_Control(pDX, IDC_DTP_COND_BEGIN_TIME, m_dtcBegTime);
	DDX_Control(pDX, IDC_DTP_COND_END_TIME, m_dtcEndTime);

	DDX_Control(pDX, IDC_CB_JCFF, m_cbJCFF);
	DDX_Control(pDX, IDC_CB_ISOBDJC, m_cbIsOBD);
}

BEGIN_MESSAGE_MAP(CSGOBDDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CSGOBDDlg::OnBnClickedButtonUpdateVehicleList)
	ON_EN_CHANGE(ID_ED_PlateNumber, &CSGOBDDlg::OnEnChangeEdPlatenumber)
	ON_BN_CLICKED(IDC_BTN_UP, &CSGOBDDlg::OnBnClickedBtnUp)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CSGOBDDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CSGOBDDlg::OnBnClickedBtnSetconfig)
	ON_BN_CLICKED(IDC_BTN_UP_TELET, &CSGOBDDlg::OnBnClickedBtnUpTelet)
	ON_CBN_SELCHANGE(IDC_CB_JCFF, &CSGOBDDlg::OnCbnSelchangeCbJcff)
	ON_CBN_SELCHANGE(IDC_CB_ISOBDJC, &CSGOBDDlg::OnCbnSelchangeCbIsobdjc)
END_MESSAGE_MAP()


// CSGOBDDlg ��Ϣ�������

BOOL CSGOBDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();

	GetVehicleList();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSGOBDDlg::OnPaint()
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
HCURSOR CSGOBDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSGOBDDlg::OnBnClickedBtnSetconfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SetConfig();
}


void CSGOBDDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edMessAge.SetWindowTextW(L"");

	BeginWaitCursor();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strVIN = m_strIDDB = m_strPlateNum = m_strJCFF = L"";
		m_strPlateNum = m_lstVehicle.GetItemText(nIndex,1);
		m_strIDDB = m_lstVehicle.GetItemText(nIndex,9);
		m_strJCFF = m_lstVehicle.GetItemText(nIndex,6);
		m_strVIN = m_lstVehicle.GetItemText(nIndex,3);
		// ѡ�г��ƺ���Ե��UP
		if (!m_strPlateNum.IsEmpty() && !m_strIDDB.IsEmpty())
		{
			GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
		}
		else
		{
			m_edMessAge.SetWindowTextW(L"��⳵�ƺ���Ϊ��");
		}
	}
	EndWaitCursor();

	*pResult = 0;
}


void CSGOBDDlg::OnBnClickedBtnUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edMessAge.SetWindowTextW(L"");

	// �������ݿ��¼
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"SGDB.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		// ��д�������ļ�����
		si.SetString(L"Database", L"Source", theApp.m_strSourceDB);
		si.SetString(L"Database", L"Catalog", theApp.m_strCatalogDB);
		si.SetString(L"Database", L"UserID", theApp.m_strUserIDDB);
		si.SetString(L"Database", L"Password", theApp.m_strPasswordDB);
	}

	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, theApp.m_strSourceDB, theApp.m_strCatalogDB, theApp.m_strUserIDDB, theApp.m_strPasswordDB))
	{
		CString strSQL, strMsg;
		strSQL.Format(_T(" Update ������Ϣ set OBD = '��' where CLHP = '%s' "), m_strPlateNum);
		int nRowsInvolved(0);
		if (0x00 != CNHSQLServerDBO::ExecuteDML(pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"�޸����ݿ�->������Ϣ��ʧ��\r\n");
		}
		else
		{
			strMsg.AppendFormat(L"�޸����ݿ�->������Ϣ��ɹ�\r\n");
		}

		strSQL.Format(_T(" Update OutLookCheck set Data13 = 'N' where CLHP = '%s' and ID = '%s'"), m_strPlateNum, m_strIDDB);
		nRowsInvolved = 0;
		if (0x00 != CNHSQLServerDBO::ExecuteDML(pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"�޸����ݿ�->OutLookCheck��ʧ��\r\n");
		}
		else
		{
			strMsg.AppendFormat(L"�޸����ݿ�->������Ϣ��ɹ�\r\n");
		}
		if (strMsg.Find(L"ʧ��") == -1)
		{
			strMsg.AppendFormat(L"�޸����ݿ�->�������ڷ��ز���UP_TELET\r\n");
			GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
		}

		m_edMessAge.SetWindowTextW(strMsg);

	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�����ݿ�ʧ��");
		return;
	}

	CNHSQLServerDBO::CloseDB(pConnection);
}


void CSGOBDDlg::OnBnClickedBtnUpTelet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_edKey.GetWindowTextW(theApp.m_strkey);
	if (theApp.m_strkey.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"KeyΪ�գ�����д����ȡ��dlkl��");
		return;
	}

	// 
	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, theApp.m_strSourceDB, theApp.m_strCatalogDB, theApp.m_strUserIDDB, theApp.m_strPasswordDB))
	{
		// ��ȡOBD����
		SOBD sOBD;
		if (!SetOBDLOG(pConnection, sOBD))
		{
			CNHSQLServerDBO::CloseDB(pConnection);
			return;
		}
		// ����OBD����
		if (COBDService::SetOBDData(pConnection, sOBD) != 0x01)
		{
			CNHSQLServerDBO::CloseDB(pConnection);
			m_edMessAge.SetWindowTextW(L"����OBD��-����OBD��ʧ��");
			return;
		}
		//// �ϴ�OBD������ݺ�IUPR
		if(!UPReport(pConnection, sOBD))
		{
			CNHSQLServerDBO::CloseDB(pConnection);
			return;
		}
		if(!UPOBDIUPR(pConnection, sOBD))
		{
			CNHSQLServerDBO::CloseDB(pConnection);
			return;
		}
		// �ϴ�OBD��������
		if(!UPRealTimeData(pConnection))
		{
			CNHSQLServerDBO::CloseDB(pConnection);
			return;
		}
		//���������Ƿ�ҲҪ���֣���
		// ����֮ǰ���޸�
		CString strSQL, strMsg;
		strSQL.Format(_T(" Update ������Ϣ set OBD = '��' where CLHP = '%s' "), m_strPlateNum);
		int nRowsInvolved(0);
		if (0x00 != CNHSQLServerDBO::ExecuteDML(pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"�޸����ݿ�->������Ϣ��ʧ��\r\n");
		}
		else
		{
			strMsg.AppendFormat(L"�޸����ݿ�->������Ϣ��ɹ�\r\n");
		}

		strSQL.Format(_T(" Update OutLookCheck set Data13 = 'Y' where CLHP = '%s' and ID = '%s'"), m_strPlateNum, m_strIDDB);
		nRowsInvolved = 0;
		if (0x00 != CNHSQLServerDBO::ExecuteDML(pConnection, strSQL, &nRowsInvolved))
		{
			strMsg.AppendFormat(L"�޸����ݿ�->OutLookCheck��ʧ��\r\n");
		}
		else
		{
			strMsg.AppendFormat(L"�޸����ݿ�->������Ϣ��ɹ�\r\n");
		}
		if (strMsg.Find(L"ʧ��") == -1)
		{
			GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
		}
		m_edMessAge.SetWindowTextW(strMsg);
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"UP_TELET-�����ݿ�ʧ��");
	}
	CNHSQLServerDBO::CloseDB(pConnection);
}


void CSGOBDDlg::OnEnChangeEdPlatenumber()
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


void CSGOBDDlg::OnCbnSelchangeCbJcff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strJCFF(L"");

	const int nIndex = m_cbJCFF.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_cbJCFF.GetLBText(nIndex, strJCFF);
	}

	if (!strJCFF.IsEmpty())
	{
		GetVehicleList();
	}
}


void CSGOBDDlg::OnCbnSelchangeCbIsobdjc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strIsOBD(L"");

	const int nIndex = m_cbIsOBD.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_cbIsOBD.GetLBText(nIndex, strIsOBD);
	}

	if (!strIsOBD.IsEmpty())
	{
		GetVehicleList();
	}
}


void CSGOBDDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetVehicleList();
}


void CSGOBDDlg::SetConfig(void)
{
	m_edSource.SetWindowTextW(theApp.m_strSourceDB);

	m_edCatalog.SetWindowTextW(theApp.m_strCatalogDB);

	m_edUserID.SetWindowTextW(theApp.m_strUserIDDB);

	m_edPassword.SetWindowTextW(theApp.m_strPasswordDB);

	m_edKey.SetWindowTextW(theApp.m_strkey);

	m_edURL.SetWindowTextW(theApp.m_strURL);

	m_edStationNum.SetWindowTextW(theApp.m_StationNum);

	m_edLineNUmber.SetWindowTextW(theApp.m_LineNum);

}


void CSGOBDDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(1, L"���ƺ���",		0,	150);
	m_lstVehicle.InsertColumn(2, L"������ɫ",	0,	90);
	m_lstVehicle.InsertColumn(3, L"VIN",	0,	200);
	m_lstVehicle.InsertColumn(4, L"ȼ�����",	0,	90);
	m_lstVehicle.InsertColumn(5, L"�������",	0,	150);
	m_lstVehicle.InsertColumn(6, L"��ⷽ��",	0,	90);
	m_lstVehicle.InsertColumn(7, L"�����",	0,	90);
	m_lstVehicle.InsertColumn(8, L"�Ƿ���OBD",	0,	60);
	m_lstVehicle.InsertColumn(9, L"ID",	0,	0);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);



	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(7, 0, 0, 0);
	COleDateTime odtBegTime = COleDateTime::GetCurrentTime() - odts;
	COleDateTime odtEndTime = COleDateTime::GetCurrentTime();

	m_dtcBegTime.SetTime(odtBegTime);
	m_dtcEndTime.SetTime(odtEndTime);

	//  ;ASM;SDS;JZJS;ZYJS
	m_cbJCFF.AddString(L"");
	m_cbJCFF.AddString(L"ASM");
	m_cbJCFF.AddString(L"SDS");
	m_cbJCFF.AddString(L"JZJS");
	m_cbJCFF.AddString(L"ZYJS");

	// 
	m_cbIsOBD.AddString(L"");
	m_cbIsOBD.AddString(L"Y");
	m_cbIsOBD.AddString(L"N");
}

bool CSGOBDDlg::GetVehicleList(void)
{
	m_edMessAge.SetWindowTextW(L"");
	// ɾ���б�����
	m_lstVehicle.DeleteAllItems();

	m_edSource.GetWindowTextW(theApp.m_strSourceDB);
	m_edCatalog.GetWindowTextW(theApp.m_strCatalogDB);
	m_edUserID.GetWindowTextW(theApp.m_strUserIDDB);
	m_edPassword.GetWindowTextW(theApp.m_strPasswordDB);

	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);
	CString strJCFF(L"");
	const int nIndexJCFF = m_cbJCFF.GetCurSel();
	if (nIndexJCFF != CB_ERR)
	{
		m_cbJCFF.GetLBText(nIndexJCFF, strJCFF);
	}
	CString strIsOBD(L"");
	const int nIndexIsOBD = m_cbIsOBD.GetCurSel();
	if (nIndexIsOBD != CB_ERR)
	{
		m_cbIsOBD.GetLBText(nIndexIsOBD, strIsOBD);
	}

	if (theApp.m_strSourceDB.IsEmpty() || theApp.m_strCatalogDB.IsEmpty() ||
		theApp.m_strUserIDDB.IsEmpty() || theApp.m_strPasswordDB.IsEmpty()
		)
	{
		m_edMessAge.SetWindowTextW(L"���ݿ����Ϊ��");
		return false;
	}

	_ConnectionPtr pConnection(NULL);

	// �趨���ݿ������־
	CNHSQLServerDBO::SetLogFilePath(theApp.m_strDBLogFilePath);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, theApp.m_strSourceDB, theApp.m_strCatalogDB, theApp.m_strUserIDDB, theApp.m_strPasswordDB))
	{
		// ��ȡ�����б�
		// ��ʱ���ȡ����ǰʱ����ǰ7���ڵļ�¼
		COleDateTime odtBegTime = COleDateTime::GetCurrentTime();
		COleDateTime odtEndTime = COleDateTime::GetCurrentTime();
		m_dtcBegTime.GetTime(odtBegTime);
		m_dtcEndTime.GetTime(odtEndTime);

		CString strSQL;

		strSQL.AppendFormat(L" select * ");
		strSQL.AppendFormat(L" from OutLookCheck ");
		strSQL.AppendFormat(L" where (OutLookCheck.CheckTime between '%s 00:00:00' and '%s 23:59:59') ", odtBegTime.Format(L"%Y-%m-%d"), odtEndTime.Format(L"%Y-%m-%d"));

		// ���ӳ���ɸѡ
		if (!strPlateNumber.IsEmpty())
		{
			strSQL.AppendFormat(L" and CLHP like '%%%s%%' ", strPlateNumber);
		}
		// ��ⷽ��ɸѡ
		if (!strJCFF.IsEmpty())
		{
			strSQL.AppendFormat(L" and JCFF like '%%%s%%' ", strJCFF);
		}
		// �Ƿ���OBDɸѡ
		if (!strIsOBD.IsEmpty())
		{
			strSQL.AppendFormat(L" and Data13 like '%%%s%%' ", strIsOBD);
		}

		strSQL.AppendFormat(L" order by ID asc");

		std::list<SOutLookCheck> lsOutLookCheck;
		if (COutLookCheckService::GetOutLookCheckData(pConnection, strSQL, lsOutLookCheck) == 0xFFFFFFFF)
		{
			CString strMsg;
			strMsg.Format(L"��ȡ�����쳣\r\n���ݿ��ѯ���:%s", strSQL);
			m_edMessAge.SetWindowTextW(strMsg);
			return false;
		}
		if (lsOutLookCheck.size() > 0)
		{
			int nItem(0);
			for (list<SOutLookCheck>::iterator ir=lsOutLookCheck.begin(); ir!=lsOutLookCheck.end(); ++ir)
			{
				CString str;
				// ���
				str.Format(L"%d", nItem+1);
				int nItemIndex = m_lstVehicle.InsertItem(nItem, str);
				// ���ƺ���
				m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strCLHP.c_str(), 0, 0, 0, 0);
				// ������ɫ
				m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strHPYS.c_str(), 0, 0, 0, 0);
				// VIN
				m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strVIN.c_str(), 0, 0, 0, 0);
				// ȼ�����
				m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strRLZL.c_str(), 0, 0, 0, 0);
				// �������
				m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strJYLB.c_str(), 0, 0, 0, 0);
				// ��ⷽ��
				m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, ir->strJCFF.c_str(), 0, 0, 0, 0);
				// �����
				m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, ir->strJYR.c_str(), 0, 0, 0, 0);
				// �Ƿ���OBD
				if (ir->strData13.find(L"Y") != std::string::npos)
				{
					str.Format(L"��");
				}
				else
				{
					str.Format(L"��");
				}
				m_lstVehicle.SetItem (nItemIndex, 8, LVIF_TEXT, str, 0, 0, 0, 0);
				// ID
				m_lstVehicle.SetItem (nItemIndex, 9, LVIF_TEXT, ir->strID.c_str(), 0, 0, 0, 0);
			}
		}
		//else
		//{
		//	CString strMsg;
		//	strMsg.Format(L"��ѯ��������Ϊ��\r\n���ݿ��ѯ���:%s", strSQL);
		//	m_edMessAge.SetWindowTextW(strMsg);
		//	return false;
		//}

		// ��ȡ�����Ա
		_RecordsetPtr pRecordset(NULL);
		strSQL.Format(L"select * from Ա�� ");
		if (0x00 == CNHSQLServerDBO::OpenQuery(pConnection, pRecordset, strSQL))
		{
			m_cbOBDJCY.ResetContent();
			m_cbOBDJCY.AddString(L"");
			// �򿪲�ѯ�ɹ�
			try
			{
				while (!pRecordset->adoEOF)
				{
					_variant_t v;
					v.ChangeType(VT_NULL);
					CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"NAME");
					if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
					{
						m_cbOBDJCY.AddString((wchar_t*)(_bstr_t)v);
					}
					pRecordset->MoveNext();
				}
			}
			catch (_com_error& e)
			{
				CNHSQLServerDBO::OutputDBErrMsg(e);
			}

			CNHSQLServerDBO::CloseQuery(pRecordset);
		}
		else
		{
			CNHSQLServerDBO::CloseQuery(pRecordset);
			CString strMsg;
			strMsg.Format(L"��ѯ��������ʧ��\r\n���ݿ��ѯ���:%s", strSQL);
			m_edMessAge.SetWindowTextW(strMsg);
			return false;
		}
		
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

bool CSGOBDDlg::SetOBDLOG(_ConnectionPtr &pConnection, SOBD& sOBD)
{
	CNHSQLServerDBO::SetLogFilePath(theApp.m_strDBLogFilePath);
	bool bRet(false);
	CString strOBDJCName;
	m_cbOBDJCY.GetWindowTextW(strOBDJCName);
	if (strOBDJCName.IsEmpty())
	{
		m_edMessAge.SetWindowTextW(L"����OBD��-���ԱΪ��");
		return bRet;
	}
	CString strSQL;
	SVehInfo sVehInfo;
	strSQL.Format(L" select * from ������Ϣ where CLHP = '%s'", m_strPlateNum);
	if (CVehInfoService::GetVehInfoData(pConnection, strSQL, sVehInfo) != 0x01)
	{
		m_edMessAge.SetWindowTextW(L"����OBD��-��ȡ������Ϣʧ��");
		return bRet;
	}

	if (m_strJCFF == L"ASM")
	{
		m_sASM = SASM();
		strSQL.Format(L" select a.* from ASM a where CLID = (select max(CLID) from ASM where CLPH = '%s') order by a.CLPH ", m_strPlateNum);
		DWORD dw(0x00);
		dw = CASMService::GetASMData(pConnection, strSQL, m_sASM);
		if (dw == 0x00)
		{
			// û�л�ȡ����500�����ڻ�ȡһ��
			Sleep(500);
			dw = CASMService::GetASMData(pConnection, strSQL, m_sASM);
		}

		if (dw != 0x01)
		{
			CString str;
			str.Format(L"%x", dw);
			CNHLogAPI::WriteLogEx(theApp.m_strDBLogFilePath, L"��ȡASMʧ��", L"���غ���", str);
			m_edMessAge.SetWindowTextW(L"����OBD��-��ȡASMʧ��");
			return bRet;
		}

		sOBD.strCLID = m_sASM.strCLID;
	}
	else if (m_strJCFF == L"SDS")
	{
		m_sSDS = SSDS();
		strSQL.Format(L" select a.* from SDS a where CLID = (select max(CLID) from SDS where CLPH = '%s') order by a.CLPH ", m_strPlateNum);

		DWORD dw(0x00);
		dw = CSDSService::GetSDSData(pConnection, strSQL, m_sSDS);

		if (dw == 0x00)
		{
			// û�л�ȡ����500�����ڻ�ȡһ��
			Sleep(500);
			dw = CSDSService::GetSDSData(pConnection, strSQL, m_sSDS);
		}

		if (dw != 0x01)
		{
			CString str;
			str.Format(L"%x", dw);
			CNHLogAPI::WriteLogEx(theApp.m_strDBLogFilePath, L"��ȡSDSʧ��", L"���غ���", str);
			m_edMessAge.SetWindowTextW(L"����OBD��-��ȡSDSʧ��");
			return bRet;
		}
		sOBD.strCLID = m_sSDS.strCLID;
	}
	else if (m_strJCFF == L"JZJS")
	{
		m_sJZJS = SJZJS();
		strSQL.Format(L" select a.* from JZJS a where CLID = (select max(CLID) from JZJS where CLPH = '%s') order by a.CLPH ", m_strPlateNum);

		DWORD dw(0x00);
		dw = CJZJSService::GetJZJSData(pConnection, strSQL, m_sJZJS);

		if (dw == 0x00)
		{
			// û�л�ȡ����500�����ڻ�ȡһ��
			Sleep(500);
			dw = CJZJSService::GetJZJSData(pConnection, strSQL, m_sJZJS);
		}

		if (dw != 0x01)
		{
			CString str;
			str.Format(L"%x", dw);
			CNHLogAPI::WriteLogEx(theApp.m_strDBLogFilePath, L"��ȡJZJSʧ��", L"���غ���", str);
			m_edMessAge.SetWindowTextW(L"����OBD��-��ȡJZJSʧ��");
			return bRet;
		}
		sOBD.strCLID = m_sJZJS.strCLID;
	}
	else if (m_strJCFF == L"ZYJS")
	{
		m_sZYJS_Btg = SZYJS_Btg();
		strSQL.Format(L" select a.* from ZYJS a where CLID = (select max(CLID) from ZYJS where CLPH = '%s') order by a.CLPH ", m_strPlateNum);

		DWORD dw(0x00);
		dw = CZYJS_BtgService::GetZYJS_BtgData(pConnection, strSQL, m_sZYJS_Btg);

		if (dw == 0x00)
		{
			// û�л�ȡ����500�����ڻ�ȡһ��
			Sleep(500);
			dw = CZYJS_BtgService::GetZYJS_BtgData(pConnection, strSQL, m_sZYJS_Btg);
		}

		if (dw != 0x01)
		{
			CString str;
			str.Format(L"%x", dw);
			CNHLogAPI::WriteLogEx(theApp.m_strDBLogFilePath, L"��ȡZYJS_Btgʧ��", L"���غ���", str);
			m_edMessAge.SetWindowTextW(L"����OBD��-��ȡZYJS_Btgʧ��");
			return bRet;
		}
		sOBD.strCLID = m_sZYJS_Btg.strCLID;
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"����OBD��-δ֪�������");
		return bRet;
	}

	CString strTemp;

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 2, 0);
	COleDateTime odtSartTime = odtCurTime - odts;

	m_edStationNum.GetWindowTextW(theApp.m_StationNum);
	sOBD.strStationID = theApp.m_StationNum.GetString();
	m_edLineNUmber.GetWindowTextW(theApp.m_LineNum);
	sOBD.strLineID = theApp.m_LineNum.GetString();
	sOBD.strStartTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sOBD.strEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

	sOBD.strVIN = m_strVIN.GetString();
	sOBD.strXSLC = L"0";
	sOBD.strKZDYMC = L"_|__|_";
	sOBD.strTX = L"1";
	sOBD.strTXSBYY = L"0";
	sOBD.strXSMILZT = L"1";
	sOBD.strSCMILZT = L"1";
	sOBD.strMILXSLC = L"0";
	sOBD.strZDJXZT = L"1///1//1/1//1//////";
	// IUPR
	int nIUPR1, nIUPR2;
	float fIUPR;
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR1 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR2 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR3 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR3 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR4 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR5 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR, false);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR6 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR7 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR8 = strTemp.GetString();
	Sleep(1000);
	GetIUPR(nIUPR1, nIUPR2, fIUPR);
	strTemp.Format(L"%d_&_%d_&_%.2f", nIUPR1, nIUPR2, fIUPR);
	sOBD.strIUPR9 = strTemp.GetString();
	sOBD.strPDJG = L"1";
	sOBD.strUploadStatus = L"N";
	sOBD.strBY1 = strOBDJCName.GetString();
	sOBD.strBY2 = m_strPlateNum.GetString();
	sOBD.strBY3 = sVehInfo.strXH.c_str();
	// ����
	if (m_strJCFF == L"JZJS" || m_strJCFF == L"ZYJS")
	{
		
		CString strOBDType;
		srand(time(0));
		int n = rand()%10;
		strOBDType.Format(L"%s", GetOBDType(n, L"����")); 
		CString strEngineCALID, strEngineCVN;
		Sleep(1000);
		GetEngineCALID(strOBDType, L"����", strEngineCALID, strEngineCVN);

		sOBD.strXSJYYQ = strOBDType.GetString();
		strTemp.Format(L"%s_|_%s_|_%s", strEngineCALID, L"",L"");
		sOBD.strCALID = strTemp.GetString();
		strTemp.Format(L"%s_|_%s_|_%s", strEngineCVN, L"",L"");
		sOBD.strCVN = strTemp.GetString();

	}
	// ����
	else
	{
		CString strOBDType;
		srand(time(0));
		int n = rand()%10;
		strOBDType.Format(L"%s", GetOBDType(n, L"����")); 
		CString strEngineCALID, strEngineCVN;
		Sleep(1000);
		GetEngineCALID(strOBDType, L"����", strEngineCALID, strEngineCVN);

		sOBD.strXSJYYQ = strOBDType.GetString();
		strTemp.Format(L"%s_|_%s_|_%s", strEngineCALID, L"",L"");
		sOBD.strCALID = strTemp.GetString();
		strTemp.Format(L"%s_|_%s_|_%s", strEngineCVN, L"",L"");
		sOBD.strCVN = strTemp.GetString();
	}

	return true;
}

CString CSGOBDDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CSGOBDDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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

void CSGOBDDlg::GetIUPR(int& nIUPR1, int& nIUPR2, float& fIUPR, const bool& bFir/* = true*/)
{
	nIUPR1 = 0;
	nIUPR2 = 0;
	fIUPR = 0.00;

	srand(time(0));

	int n = rand()%10;
	int n1 = rand()%5;
	if (bFir)
	{
		if (n > 0 && n1 > 0)
		{
			nIUPR1 = n * 100 + 24;
			nIUPR2 = n1 * 1000 + 245;

			fIUPR = ((float)nIUPR1) / ((float)nIUPR2);
		}
	}
	else
	{
		if (n > 0 && n1 > 0)
		{
			nIUPR2 = n * 100 + 24;
			nIUPR1 = n1 * 1000 + 245;

			fIUPR = ((float)nIUPR1) / ((float)nIUPR2);
		}
	}
}

bool CSGOBDDlg::UPReport(_ConnectionPtr &pConnection, const SOBD& sOBD)
{
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());

	CStringW OBDData(L"");
	CString strTemp, strT;
	CString strMsg;
	int nPos;
	// �ϴ�OBD���浥
	OBDData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	OBDData.AppendFormat(L"<root><obd>");
	if (m_strJCFF == L"ASM")
	{
		strTemp = m_sASM.strCLID.c_str();
		strT = m_sASM.strJYCS.c_str();
	}
	else if (m_strJCFF == L"SDS")
	{
		strTemp = m_sSDS.strCLID.c_str();
		strT = m_sSDS.strJYCS.c_str();
	}
	else if (m_strJCFF == L"JZJS")
	{
		strTemp = m_sJZJS.strCLID.c_str();
		strT = m_sJZJS.strJYCS.c_str();
	}
	else if (m_strJCFF == L"ZYJS")
	{
		strTemp = m_sZYJS_Btg.strCLID.c_str();
		strT = m_sZYJS_Btg.strJYCS.c_str();
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�ϴ�OBD���-δ֪�������");
		return false;
	}
	OBDData.AppendFormat(L"<jylsh>%s</jylsh>", strTemp);
	OBDData.AppendFormat(L"<jccs>%s</jccs>", strT);
	OBDData.AppendFormat(L"<clsbdh>%s</clsbdh>", sOBD.strVIN.c_str());
	strTemp.Format(L"%s", sOBD.strCALID.c_str());
	nPos = strTemp.Find(L"_|_");
	strT = strTemp.Mid(0, nPos);
	OBDData.AppendFormat(L"<fdj_calid>%s</fdj_calid>", strT);
	strTemp.Format(L"%s", sOBD.strCVN.c_str());
	nPos = strTemp.Find(L"_|_");
	strT = strTemp.Mid(0, nPos);
	OBDData.AppendFormat(L"<fdj_cvn>%s</fdj_cvn>", strT);
	OBDData.AppendFormat(L"<hc_calid></hc_calid>");
	OBDData.AppendFormat(L"<hc_cvn></hc_cvn>");
	OBDData.AppendFormat(L"<qt_calid></qt_calid>");
	OBDData.AppendFormat(L"<qt_cvn></qt_cvn>");
	OBDData.AppendFormat(L"<obd_gzq>1</obd_gzq>");
	OBDData.AppendFormat(L"<obd_txqk>1</obd_txqk>");
	OBDData.AppendFormat(L"<obd_txqk_yy></obd_txqk_yy>");
	OBDData.AppendFormat(L"<obd_gzd_zt>0</obd_gzd_zt>");
	OBDData.AppendFormat(L"<obd_gzdm></obd_gzdm>");
	OBDData.AppendFormat(L"<obd_zd_zt>0</obd_zd_zt>");
	OBDData.AppendFormat(L"<obd_zd_yy></obd_zd_yy>");
	OBDData.AppendFormat(L"<mil_lc></mil_lc>");
	OBDData.AppendFormat(L"<obdzdyscqy>��������</obdzdyscqy>");
	OBDData.AppendFormat(L"<obdzdyxh>D91T</obdzdyxh>");
	OBDData.AppendFormat(L"<obd_bgd_jczt>1</obd_bgd_jczt>");
	OBDData.AppendFormat(L"<jyy>%s</jyy>", sOBD.strBY1.c_str());
	OBDData.AppendFormat(L"</obd></root>");

	std::wstring strJkid(L"11C21");
	std::wstring strRet;
	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), strJkid, OBDData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
					strMsg.AppendFormat(L"�ϴ�OBD-%s,%s", strCode.c_str(), strContent.c_str());
					m_edMessAge.SetWindowTextW(strMsg);
				}
				return false;
			}
			else
			{
				
				strMsg.AppendFormat(L"�ϴ�OBD-%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				m_edMessAge.SetWindowTextW(strMsg);
			}
		}
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�ϴ�OBD���-����ʧ��");
		return false;
	}


	return true;
}

bool CSGOBDDlg::UPRealTimeData(_ConnectionPtr &pConnection)
{
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	CString strSql;
	CString strMsg;

	CStringW OBDData;
	OBDData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	OBDData.AppendFormat(L"<root><obd>");

	CStringW strTemp[23]; //��22����������0��ʼ
	for (int i=0;i<23;i++)
	{strTemp[i].Empty();}

	if (m_strJCFF == L"ASM")
	{
		strSql.Format(L"SELECT * FROM ASM_DATASECONDS WHERE CLID = '%s'", m_sASM.strCLID.c_str());
		SASM_DataSeconds sASM_DataSeconds;
		if (CASM_DataSecondsService::GetASM_DataSecondsData(pConnection, strSql, sASM_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-��ȡASM_DATASECONDSʧ��");
			return false;
		}

		OBDData.AppendFormat(L"<jylsh>%s</jylsh>", m_sASM.strCLID.c_str());
		OBDData.AppendFormat(L"<jccs>%s</jccs>", m_sASM.strJYCS.c_str());
		OBDData.AppendFormat(L"<cyds>%s</cyds>", sASM_DataSeconds.strCYDS.c_str());// �����ܵ���
		OBDData.AppendFormat(L"<cysx>%s</cysx>",sASM_DataSeconds.strMMSX.c_str());// ��ʱ�������ఴ˳������
		OBDData.AppendFormat(L"<sjxl>%s</sjxl>",sASM_DataSeconds.strMMTIME.c_str());// ȫ��ʱ�� ��ʽΪ yyyy-mm-dd 24h:mi:ss.fff
		for (int i = 0; i < _wtoi(sASM_DataSeconds.strCYDS.c_str()); i++)
		{
			if (i%5 == 0)
			{
				Sleep(500);
			}

			CString strOBDJQMJDKD, strMMOBDJSFHZ, strMMOBDQYCGQXH, strMMOBDSPEED, strMMOBDROTATESPEED, strMMOBDJQL, strMMOBDJQYL, strMMOBDLAMBDA;
			GetOBDTimeDataASM(strOBDJQMJDKD, strMMOBDJSFHZ, strMMOBDQYCGQXH, strMMOBDSPEED, strMMOBDROTATESPEED,
				strMMOBDJQL, strMMOBDJQYL, strMMOBDLAMBDA, _wtof(sASM_DataSeconds.strMMCS.c_str()), i);
			strTemp[1].AppendFormat(L"%s,", strOBDJQMJDKD);// �����ž��Կ���
			strTemp[2].AppendFormat(L"%s,", strMMOBDJSFHZ);// ���㸺��ֵ
			strTemp[3].AppendFormat(L"%s,", strMMOBDQYCGQXH);// ǰ���������ź�
			strTemp[4].AppendFormat(L"%s,", strMMOBDLAMBDA);// ��������ϵ�����ˣ�
			strTemp[5].AppendFormat(L"%s,", L"");// ���ſ���
			strTemp[6].AppendFormat(L"%s,", strMMOBDSPEED);// ����
			strTemp[7].AppendFormat(L"%s,", strMMOBDROTATESPEED);// ������ת��
			strTemp[8].AppendFormat(L"%s,", strMMOBDJQL);// ������
			strTemp[9].AppendFormat(L"%s,", strMMOBDJQYL);// ����ѹ��
			strTemp[10].AppendFormat(L"%s,", L"");// �������������
			strTemp[11].AppendFormat(L"%s,", L"");// ��ѹѹ��
			strTemp[12].AppendFormat(L"%s,", L"");// ������
			strTemp[13].AppendFormat(L"%s,", L"");// ����������Ũ��
			strTemp[14].AppendFormat(L"%s,", L"");// ����������
			strTemp[15].AppendFormat(L"%s,", L"");// �����¶�
			strTemp[16].AppendFormat(L"%s,", L"");// ����������ѹ��
			strTemp[17].AppendFormat(L"%s,", L"");// EGR ����
			strTemp[18].AppendFormat(L"%s,", L"");// ȼ������ѹ��
			strTemp[19].AppendFormat(L"%s,", L"");// �������ͣ�Ԥ����
			strTemp[20].AppendFormat(L"%s,", L"");// 
		}

		// ȥ�����һ������
		for (int i=0; i<20; i++)
		{
			if (!strTemp[i+1].IsEmpty())
			{
				strTemp[i+1].Delete(strTemp[i+1].GetLength()-1, 1);
			}
		}
		sASM_DataSeconds.strMMOBDJQMJDKD = strTemp[1].GetString();
		sASM_DataSeconds.strMMOBDJSFHZ = strTemp[2].GetString();
		sASM_DataSeconds.strMMOBDQYCGQXH = strTemp[3].GetString();
		sASM_DataSeconds.strMMOBDLAMBDA = strTemp[4].GetString();
		sASM_DataSeconds.strMMOBDSPEED = strTemp[6].GetString();
		sASM_DataSeconds.strMMOBDROTATESPEED = strTemp[7].GetString();
		sASM_DataSeconds.strMMOBDJQL = strTemp[8].GetString();
		sASM_DataSeconds.strMMOBDJQYL = strTemp[9].GetString();

		if (CASM_DataSecondsService::SetASM_DataSecondsData(pConnection, sASM_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-����ASM_DATASECONDSʧ��");
			return false;
		}
	}
	else if (m_strJCFF == L"SDS")
	{
		strSql.Format(L"SELECT * FROM SDS_DATASECONDS WHERE CLID = '%s'", m_sSDS.strCLID.c_str());
		SSDS_DataSeconds sSDS_DataSeconds;
		if (CSDS_DATASECONDSService::GetSDS_DATASECONDSData(pConnection, strSql, sSDS_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-��ȡSDS_DATASECONDSʧ��");
			return false;
		}
		OBDData.AppendFormat(L"<jylsh>%s</jylsh>", m_sSDS.strCLID.c_str());
		OBDData.AppendFormat(L"<jccs>%s</jccs>", m_sSDS.strJYCS.c_str());
		OBDData.AppendFormat(L"<cyds>%s</cyds>", sSDS_DataSeconds.strCYDS.c_str());// �����ܵ���
		OBDData.AppendFormat(L"<cysx>%s</cysx>",sSDS_DataSeconds.strMMSX.c_str());// ��ʱ�������ఴ˳������
		OBDData.AppendFormat(L"<sjxl>%s</sjxl>",sSDS_DataSeconds.strMMTIME.c_str());// ȫ��ʱ�� ��ʽΪ yyyy-mm-dd 24h:mi:ss.fff
		for (int i = 0; i < _wtoi(sSDS_DataSeconds.strCYDS.c_str()); i++)
		{
			if (i%5 == 0)
			{
				Sleep(500);
			}

			CString strOBDJQMJDKD, strMMOBDJSFHZ, strMMOBDQYCGQXH, strMMOBDSPEED, strMMOBDROTATESPEED, strMMOBDJQL, strMMOBDJQYL, strMMOBDLAMBDA;
			GetOBDTimeDataSDS(strOBDJQMJDKD, strMMOBDJSFHZ, strMMOBDQYCGQXH, strMMOBDSPEED, strMMOBDROTATESPEED,
				strMMOBDJQL, strMMOBDJQYL, strMMOBDLAMBDA, _wtoi(sSDS_DataSeconds.strMMZS.c_str()), i);
			strTemp[1].AppendFormat(L"%s,", strOBDJQMJDKD);// �����ž��Կ���
			strTemp[2].AppendFormat(L"%s,", strMMOBDJSFHZ);// ���㸺��ֵ
			strTemp[3].AppendFormat(L"%s,", strMMOBDQYCGQXH);// ǰ���������ź�
			strTemp[4].AppendFormat(L"%s,", strMMOBDLAMBDA);// ��������ϵ�����ˣ�
			strTemp[5].AppendFormat(L"%s,", L"");// ���ſ���
			strTemp[6].AppendFormat(L"%s,", strMMOBDSPEED);// ����
			strTemp[7].AppendFormat(L"%s,", strMMOBDROTATESPEED);// ������ת��
			strTemp[8].AppendFormat(L"%s,", strMMOBDJQL);// ������
			strTemp[9].AppendFormat(L"%s,", strMMOBDJQYL);// ����ѹ��
			strTemp[10].AppendFormat(L"%s,", L"");// �������������
			strTemp[11].AppendFormat(L"%s,", L"");// ��ѹѹ��
			strTemp[12].AppendFormat(L"%s,", L"");// ������
			strTemp[13].AppendFormat(L"%s,", L"");// ����������Ũ��
			strTemp[14].AppendFormat(L"%s,", L"");// ����������
			strTemp[15].AppendFormat(L"%s,", L"");// �����¶�
			strTemp[16].AppendFormat(L"%s,", L"");// ����������ѹ��
			strTemp[17].AppendFormat(L"%s,", L"");// EGR ����
			strTemp[18].AppendFormat(L"%s,", L"");// ȼ������ѹ��
			strTemp[19].AppendFormat(L"%s,", L"");// �������ͣ�Ԥ����
			strTemp[20].AppendFormat(L"%s,", L"");// 
		}

		// ȥ�����һ������
		for (int i=0; i<20; i++)
		{
			if (!strTemp[i+1].IsEmpty())
			{
				strTemp[i+1].Delete(strTemp[i+1].GetLength()-1, 1);
			}
		}
		sSDS_DataSeconds.strMMOBDJQMJDKD = strTemp[1].GetString();
		sSDS_DataSeconds.strMMOBDJSFHZ = strTemp[2].GetString();
		sSDS_DataSeconds.strMMOBDQYCGQXH = strTemp[3].GetString();
		sSDS_DataSeconds.strMMOBDLAMBDA = strTemp[4].GetString();
		sSDS_DataSeconds.strMMOBDSPEED = strTemp[6].GetString();
		sSDS_DataSeconds.strMMOBDROTATESPEED = strTemp[7].GetString();
		sSDS_DataSeconds.strMMOBDJQL = strTemp[8].GetString();
		sSDS_DataSeconds.strMMOBDJQYL = strTemp[9].GetString();

		if (CSDS_DATASECONDSService::SetSDS_DATASECONDSData(pConnection, sSDS_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-����SDS_DATASECONDSʧ��");
			return false;
		}

	}
	else if (m_strJCFF == L"JZJS")
	{
		strSql.Format(L"SELECT * FROM JZJS_DATASECONDS WHERE CLID = '%s'", m_sJZJS.strCLID.c_str());
		SJZJS_DataSeconds sJZJS_DataSeconds;
		if (CJZJS_DATASECONDSService::GetJZJS_DATASECONDSData(pConnection, strSql, sJZJS_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-��ȡJZJS_DATASECONDSʧ��");
			return false;
		}
		OBDData.AppendFormat(L"<jylsh>%s</jylsh>", m_sJZJS.strCLID.c_str());
		OBDData.AppendFormat(L"<jccs>%s</jccs>", m_sJZJS.strJYCS.c_str());
		OBDData.AppendFormat(L"<cyds>%s</cyds>", sJZJS_DataSeconds.strCYDS.c_str());// �����ܵ���
		OBDData.AppendFormat(L"<cysx>%s</cysx>",sJZJS_DataSeconds.strMMSX.c_str());// ��ʱ�������ఴ˳������
		OBDData.AppendFormat(L"<sjxl>%s</sjxl>",sJZJS_DataSeconds.strMMTIME.c_str());// ȫ��ʱ�� ��ʽΪ yyyy-mm-dd 24h:mi:ss.fff
		for (int i = 0; i < _wtoi(sJZJS_DataSeconds.strCYDS.c_str()); i++)
		{
			if (i%5 == 0)
			{
				Sleep(500);
			}
			
			CString strMMOBDYMKD, strMMOBDSPEED, strMMOBDPOWER, strMMOBDROTATESPEED, strMMOBDJQL, strMMOBDZYYL, strMMOBDHYL, 
				strMMOBDNOND, strMMOBDNSPSL, strMMOBDWD, strMMOBDKLBJQYC, strMMOBDEGRKD, strMMOBDRYPSYL;

			GetOBDTimeDataJZJS(strMMOBDYMKD, strMMOBDSPEED, strMMOBDPOWER, strMMOBDROTATESPEED, strMMOBDJQL, strMMOBDZYYL, strMMOBDHYL,
				strMMOBDNOND, strMMOBDNSPSL, strMMOBDWD, strMMOBDKLBJQYC, strMMOBDEGRKD, strMMOBDRYPSYL, _wtof(sJZJS_DataSeconds.strMMCS.c_str()), i);
			strTemp[1].AppendFormat(L"%s,", L"");// �����ž��Կ���
			strTemp[2].AppendFormat(L"%s,", L"");// ���㸺��ֵ
			strTemp[3].AppendFormat(L"%s,", L"");// ǰ���������ź�
			strTemp[4].AppendFormat(L"%s,", L"");// ��������ϵ�����ˣ�
			strTemp[5].AppendFormat(L"%s,", strMMOBDYMKD);// ���ſ���
			strTemp[6].AppendFormat(L"%s,", strMMOBDSPEED);// ����
			strTemp[7].AppendFormat(L"%s,", strMMOBDROTATESPEED);// ������ת��
			strTemp[8].AppendFormat(L"%s,", strMMOBDJQL);// ������
			strTemp[9].AppendFormat(L"%s,", L"");// ����ѹ��
			strTemp[10].AppendFormat(L"%s,", strMMOBDPOWER);// �������������
			strTemp[11].AppendFormat(L"%s,", strMMOBDZYYL);// ��ѹѹ��
			strTemp[12].AppendFormat(L"%s,", strMMOBDHYL);// ������
			strTemp[13].AppendFormat(L"%s,", strMMOBDNOND);// ����������Ũ��
			strTemp[14].AppendFormat(L"%s,", strMMOBDNSPSL);// ����������
			strTemp[15].AppendFormat(L"%s,", strMMOBDWD);// �����¶�
			strTemp[16].AppendFormat(L"%s,", strMMOBDKLBJQYC);// ����������ѹ��
			strTemp[17].AppendFormat(L"%s,", strMMOBDEGRKD);// EGR ����
			strTemp[18].AppendFormat(L"%s,", strMMOBDRYPSYL);// ȼ������ѹ��
			strTemp[19].AppendFormat(L"%s,", L"");// �������ͣ�Ԥ����
			strTemp[20].AppendFormat(L"%s,", L"");// 
		}

		// ȥ�����һ������
		for (int i=0; i<20; i++)
		{
			if (!strTemp[i+1].IsEmpty())
			{
				strTemp[i+1].Delete(strTemp[i+1].GetLength()-1, 1);
			}
		}
		sJZJS_DataSeconds.strMMOBDYMKD = strTemp[5].GetString();
		sJZJS_DataSeconds.strMMOBDSPEED = strTemp[6].GetString();
		sJZJS_DataSeconds.strMMOBDROTATESPEED = strTemp[7].GetString();
		sJZJS_DataSeconds.strMMOBDJQL = strTemp[8].GetString();
		sJZJS_DataSeconds.strMMOBDPOWER = strTemp[10].GetString();
		sJZJS_DataSeconds.strMMOBDZYYL = strTemp[11].GetString();
		sJZJS_DataSeconds.strMMOBDHYL = strTemp[12].GetString();
		sJZJS_DataSeconds.strMMOBDNOND = strTemp[13].GetString();
		sJZJS_DataSeconds.strMMOBDNSPSL = strTemp[14].GetString();
		sJZJS_DataSeconds.strMMOBDWD = strTemp[15].GetString();
		sJZJS_DataSeconds.strMMOBDKLBJQYC = strTemp[16].GetString();
		sJZJS_DataSeconds.strMMOBDEGRKD = strTemp[17].GetString();
		sJZJS_DataSeconds.strMMOBDRYPSYL = strTemp[18].GetString();

		if (CJZJS_DATASECONDSService::SetJZJS_DATASECONDSData(pConnection, sJZJS_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-����JZJS_DATASECONDSʧ��");
			return false;
		}
	}
	else if (m_strJCFF == L"ZYJS")
	{
		strSql.Format(L"SELECT * FROM ZYJS_DATASECONDS WHERE CLID = '%s'", m_sZYJS_Btg.strCLID.c_str());
		SZYJS_DataSeconds sZYJS_DataSeconds;
		if (CZYJS_DATASECONDSService::GetZYJS_DATASECONDSData(pConnection, strSql, sZYJS_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-��ȡZYJS_DATASECONDSʧ��");
			return false;
		}
		OBDData.AppendFormat(L"<jylsh>%s</jylsh>", m_sZYJS_Btg.strCLID.c_str());
		OBDData.AppendFormat(L"<jccs>%s</jccs>", m_sZYJS_Btg.strJYCS.c_str());
		OBDData.AppendFormat(L"<cyds>%s</cyds>", sZYJS_DataSeconds.strCYDS.c_str());// �����ܵ���
		OBDData.AppendFormat(L"<cysx>%s</cysx>",sZYJS_DataSeconds.strMMSX.c_str());// ��ʱ�������ఴ˳������
		OBDData.AppendFormat(L"<sjxl>%s</sjxl>",sZYJS_DataSeconds.strMMTIME.c_str());// ȫ��ʱ�� ��ʽΪ yyyy-mm-dd 24h:mi:ss.fff
		for (int i = 0; i < _wtoi(sZYJS_DataSeconds.strCYDS.c_str()); i++)
		{
			if (i%5 == 0)
			{
				Sleep(500);
			}

			CString strMMOBDYMKD, strMMOBDSPEED, strMMOBDPOWER, strMMOBDROTATESPEED, strMMOBDJQL, strMMOBDZYYL, strMMOBDHYL, 
				strMMOBDNOND, strMMOBDNSPSL, strMMOBDWD, strMMOBDKLBJQYC, strMMOBDEGRKD, strMMOBDRYPSYL;

			GetOBDTimeDataZYJS(strMMOBDYMKD, strMMOBDSPEED, strMMOBDPOWER, strMMOBDROTATESPEED, strMMOBDJQL, strMMOBDZYYL, strMMOBDHYL,
				strMMOBDNOND, strMMOBDNSPSL, strMMOBDWD, strMMOBDKLBJQYC, strMMOBDEGRKD, strMMOBDRYPSYL, _wtoi(sZYJS_DataSeconds.strMMZS.c_str()), i);
			strTemp[1].AppendFormat(L"%s,", L"");// �����ž��Կ���
			strTemp[2].AppendFormat(L"%s,", L"");// ���㸺��ֵ
			strTemp[3].AppendFormat(L"%s,", L"");// ǰ���������ź�
			strTemp[4].AppendFormat(L"%s,", L"");// ��������ϵ�����ˣ�
			strTemp[5].AppendFormat(L"%s,", strMMOBDYMKD);// ���ſ���
			strTemp[6].AppendFormat(L"%s,", strMMOBDSPEED);// ����
			strTemp[7].AppendFormat(L"%s,", strMMOBDROTATESPEED);// ������ת��
			strTemp[8].AppendFormat(L"%s,", strMMOBDJQL);// ������
			strTemp[9].AppendFormat(L"%s,", L"");// ����ѹ��
			strTemp[10].AppendFormat(L"%s,", strMMOBDPOWER);// �������������
			strTemp[11].AppendFormat(L"%s,", strMMOBDZYYL);// ��ѹѹ��
			strTemp[12].AppendFormat(L"%s,", strMMOBDHYL);// ������
			strTemp[13].AppendFormat(L"%s,", strMMOBDNOND);// ����������Ũ��
			strTemp[14].AppendFormat(L"%s,", strMMOBDNSPSL);// ����������
			strTemp[15].AppendFormat(L"%s,", strMMOBDWD);// �����¶�
			strTemp[16].AppendFormat(L"%s,", strMMOBDKLBJQYC);// ����������ѹ��
			strTemp[17].AppendFormat(L"%s,", strMMOBDEGRKD);// EGR ����
			strTemp[18].AppendFormat(L"%s,", strMMOBDRYPSYL);// ȼ������ѹ��
			strTemp[19].AppendFormat(L"%s,", L"");// �������ͣ�Ԥ����
			strTemp[20].AppendFormat(L"%s,", L"");// 
		}

		// ȥ�����һ������
		for (int i=0; i<20; i++)
		{
			if (!strTemp[i+1].IsEmpty())
			{
				strTemp[i+1].Delete(strTemp[i+1].GetLength()-1, 1);
			}
		}
		sZYJS_DataSeconds.strMMOBDYMKD = strTemp[5].GetString();
		sZYJS_DataSeconds.strMMOBDSPEED = strTemp[6].GetString();
		sZYJS_DataSeconds.strMMOBDROTATESPEED = strTemp[7].GetString();
		sZYJS_DataSeconds.strMMOBDJQL = strTemp[8].GetString();
		sZYJS_DataSeconds.strMMOBDPOWER = strTemp[10].GetString();
		sZYJS_DataSeconds.strMMOBDZYYL = strTemp[11].GetString();
		sZYJS_DataSeconds.strMMOBDHYL = strTemp[12].GetString();
		sZYJS_DataSeconds.strMMOBDNOND = strTemp[13].GetString();
		sZYJS_DataSeconds.strMMOBDNSPSL = strTemp[14].GetString();
		sZYJS_DataSeconds.strMMOBDWD = strTemp[15].GetString();
		sZYJS_DataSeconds.strMMOBDKLBJQYC = strTemp[16].GetString();
		sZYJS_DataSeconds.strMMOBDEGRKD = strTemp[17].GetString();
		sZYJS_DataSeconds.strMMOBDRYPSYL = strTemp[18].GetString();

		if (CZYJS_DATASECONDSService::SetZYJS_DATASECONDSData(pConnection, sZYJS_DataSeconds) != 0x01)
		{
			m_edMessAge.SetWindowTextW(L"����OBD��������-����ZYJS_DATASECONDSʧ��");
			return false;
		}
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�ϴ�OBD��������-δ֪�������");
		return false;
	}

	OBDData.AppendFormat(L"<jqmkd>%s</jqmkd>",strTemp[1]);// �����ž��Կ���
	OBDData.AppendFormat(L"<fh>%s</fh>",strTemp[2]);// ���㸺��ֵ
	OBDData.AppendFormat(L"<qycgqxh>%s</qycgqxh>",strTemp[3]);// ǰ���������ź�
	OBDData.AppendFormat(L"<glkqxs>%s</glkqxs>",strTemp[4]);// ��������ϵ�����ˣ�
	OBDData.AppendFormat(L"<ymkd>%s</ymkd>",strTemp[5]);// ���ſ���
	OBDData.AppendFormat(L"<cs>%s</cs>",strTemp[6]);// ����
	OBDData.AppendFormat(L"<fdjzs>%s</fdjzs>",strTemp[7]);// ������ת��
	OBDData.AppendFormat(L"<jql>%s</jql>",strTemp[8]);// ������
	OBDData.AppendFormat(L"<jqyl>%s</jqyl>",strTemp[9]);// ����ѹ��
	OBDData.AppendFormat(L"<scgl>%s</scgl>",strTemp[10]);// �������������
	OBDData.AppendFormat(L"<zyyl>%s</zyyl>",strTemp[11]);// ��ѹѹ��
	OBDData.AppendFormat(L"<hyl>%s</hyl>",strTemp[12]);// ������
	OBDData.AppendFormat(L"<no>%s</no>",strTemp[13]);// ����������Ũ��
	OBDData.AppendFormat(L"<nsbsl>%s</nsbsl>",strTemp[14]);// ����������
	OBDData.AppendFormat(L"<pqwd>%s</pqwd>",strTemp[15]);// �����¶�
	OBDData.AppendFormat(L"<klbjqyc>%s</klbjqyc>",strTemp[16]);// ����������ѹ��
	OBDData.AppendFormat(L"<egrkd>%s</egrkd>",strTemp[17]);// EGR ����
	OBDData.AppendFormat(L"<rypsyl>%s</rypsyl>",strTemp[18]);// ȼ������ѹ��
	OBDData.AppendFormat(L"<jczt>%s</jczt>",strTemp[19]);// �������ͣ�Ԥ����
	OBDData.AppendFormat(L"<obd_gc_jczt>%s</obd_gc_jczt>",strTemp[20]);// 
	OBDData.AppendFormat(L"</obd></root>");

	std::wstring strJkid(L"11C20");

	std::wstring strRet;
	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), strJkid, OBDData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
					strMsg.AppendFormat(L"�ϴ�OBD����-%s,%s", strCode.c_str(), strContent.c_str());
					m_edMessAge.SetWindowTextW(strMsg);
				}
			}
			else
			{
				strMsg.AppendFormat(L"�ϴ�OBD����-%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				m_edMessAge.SetWindowTextW(strMsg);
			}
		}
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�ϴ�OBD����-����ʧ��");
		return false;
	}

	return true;
}

bool CSGOBDDlg::UPOBDIUPR(_ConnectionPtr &pConnection, const SOBD& sOBD)
{
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	CStringW OBDData(L"");

	// �ϴ�OBD-IUPR
	CString strTemp, strT;
	CString strMsg;
	int nPos;
	OBDData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	OBDData.AppendFormat(L"<root><obd>");
	if (m_strJCFF == L"ASM")
	{
		strTemp = m_sASM.strCLID.c_str();
		strT = m_sASM.strJYCS.c_str();
	}
	else if (m_strJCFF == L"SDS")
	{
		strTemp = m_sSDS.strCLID.c_str();
		strT = m_sSDS.strJYCS.c_str();
	}
	else if (m_strJCFF == L"JZJS")
	{
		strTemp = m_sJZJS.strCLID.c_str();
		strT = m_sJZJS.strJYCS.c_str();
	}
	else if (m_strJCFF == L"ZYJS")
	{
		strTemp = m_sZYJS_Btg.strCLID.c_str();
		strT = m_sZYJS_Btg.strJYCS.c_str();
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�ϴ�OBD���-δ֪�������");
		return false;
	}
	OBDData.AppendFormat(L"<jylsh>%s</jylsh>", strTemp);
	strTemp = sOBD.strIUPR1.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<chq1jccs>%s</chq1jccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<chq1fhcs>%s</chq1fhcs>", strT);
		OBDData.AppendFormat(L"<chq1iupr>%s</chq1iupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<chq1jccs>%s</chq1jccs>", L"");
		OBDData.AppendFormat(L"<chq1fhcs>%s</chq1fhcs>", L"");
		OBDData.AppendFormat(L"<chq1iupr>%s</chq1iupr>", L"");
	}
	strTemp = sOBD.strIUPR2.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<chq2jccs>%s</chq2jccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<chq2fhcs>%s</chq2fhcs>", strT);
		OBDData.AppendFormat(L"<chq2iupr>%s</chq2iupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<chq2jccs>%s</chq2jccs>", L"");
		OBDData.AppendFormat(L"<chq2fhcs>%s</chq2fhcs>", L"");
		OBDData.AppendFormat(L"<chq2iupr>%s</chq2iupr>", L"");
	}
	strTemp = sOBD.strIUPR3.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<qycqq1jccs>%s</qycqq1jccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<qycqq1fhcs>%s</qycqq1fhcs>", strT);
		OBDData.AppendFormat(L"<qycqq1iupr>%s</qycqq1iupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<qycqq1jccs>%s</qycqq1jccs>", L"");
		OBDData.AppendFormat(L"<qycqq1fhcs>%s</qycqq1fhcs>", L"");
		OBDData.AppendFormat(L"<qycqq1iupr>%s</qycqq1iupr>", L"");
	}
	strTemp = sOBD.strIUPR4.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<qycqq2jccs>%s</qycqq2jccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<qycqq2fhcs>%s</qycqq2fhcs>", strT);
		OBDData.AppendFormat(L"<qycqq2iupr>%s</qycqq2iupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<qycqq2jccs>%s</qycqq2jccs>", L"");
		OBDData.AppendFormat(L"<qycqq2fhcs>%s</qycqq2fhcs>", L"");
		OBDData.AppendFormat(L"<qycqq2iupr>%s</qycqq2iupr>", L"");
	}
	strTemp = sOBD.strIUPR5.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<hycqq1jccs>%s</hycqq1jccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<hycqq1fhcs>%s</hycqq1fhcs>", strT);
		OBDData.AppendFormat(L"<hycqq1iupr>%s</hycqq1iupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<hycqq1jccs>%s</hycqq1jccs>", L"");
		OBDData.AppendFormat(L"<hycqq1fhcs>%s</hycqq1fhcs>", L"");
		OBDData.AppendFormat(L"<hycqq1iupr>%s</hycqq1iupr>", L"");
	}
	strTemp = sOBD.strIUPR6.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<hycqq2jccs>%s</hycqq2jccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<hycqq2fhcs>%s</hycqq2fhcs>", strT);
		OBDData.AppendFormat(L"<hycqq2iupr>%s</hycqq2iupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<hycqq2jccs>%s</hycqq2jccs>", L"");
		OBDData.AppendFormat(L"<hycqq2fhcs>%s</hycqq2fhcs>", L"");
		OBDData.AppendFormat(L"<hycqq2iupr>%s</hycqq2iupr>", L"");
	}
	strTemp = sOBD.strIUPR8.c_str();
	if (!strTemp.IsEmpty())
	{
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<egrvvtjccs>%s</egrvvtjccs>", strT);
		nPos = strTemp.Find(L"_&_");
		strT = strTemp.Mid(0, nPos);
		strTemp.Delete(0, nPos+3);
		OBDData.AppendFormat(L"<egrvvtfhcs>%s</egrvvtfhcs>", strT);
		OBDData.AppendFormat(L"<egrvvtiupr>%s</egrvvtiupr>", strTemp);
	}
	else
	{
		OBDData.AppendFormat(L"<egrvvtjccs>%s</egrvvtjccs>", L"");
		OBDData.AppendFormat(L"<egrvvtfhcs>%s</egrvvtfhcs>", L"");
		OBDData.AppendFormat(L"<egrvvtiupr>%s</egrvvtiupr>", L"");
	}
	// ������
	if ((m_strJCFF == L"ASM") || (m_strJCFF == L"SDS"))
	{
		strTemp = sOBD.strIUPR7.c_str();
		if (!strTemp.IsEmpty())
		{
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<evapjccs>%s</evapjccs>", strT);
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<evapfhcs>%s</evapfhcs>", strT);
			OBDData.AppendFormat(L"<evapiupr>%s</evapiupr>", strTemp);
		}
		else
		{
			OBDData.AppendFormat(L"<evapjccs>%s</evapjccs>", L"");
			OBDData.AppendFormat(L"<evapfhcs>%s</evapfhcs>", L"");
			OBDData.AppendFormat(L"<evapiupr>%s</evapiupr>", L"");
		}
		strTemp = sOBD.strIUPR9.c_str();
		if (!strTemp.IsEmpty())
		{
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<gpf1jccs>%s</gpf1jccs>", strT);
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<gpf1fhcs>%s</gpf1fhcs>", strT);
			OBDData.AppendFormat(L"<gpf1iupr>%s</gpf1iupr>", strTemp);
		}
		else
		{
			OBDData.AppendFormat(L"<gpf1jccs>%s</gpf1jccs>", L"");
			OBDData.AppendFormat(L"<gpf1fhcs>%s</gpf1fhcs>", L"");
			OBDData.AppendFormat(L"<gpf1iupr>%s</gpf1iupr>", L"");
		}
		strTemp = sOBD.strIUPR10.c_str();
		if (!strTemp.IsEmpty())
		{
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<gpf2jccs>%s</gpf2jccs>", strT);
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<gpf2fhcs>%s</gpf2fhcs>", strT);
			OBDData.AppendFormat(L"<gpf2iupr>%s</gpf2iupr>", strTemp);
		}
		else
		{
			OBDData.AppendFormat(L"<gpf2jccs>%s</gpf2jccs>", L"");
			OBDData.AppendFormat(L"<gpf2fhcs>%s</gpf2fhcs>", L"");
			OBDData.AppendFormat(L"<gpf2iupr>%s</gpf2iupr>", L"");
		}
		strTemp = sOBD.strIUPR11.c_str();
		if (!strTemp.IsEmpty())
		{
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<eckqpsxtjccs>%s</eckqpsxtjccs>", strT);
			nPos = strTemp.Find(L"_&_");
			strT = strTemp.Mid(0, nPos);
			strTemp.Delete(0, nPos+3);
			OBDData.AppendFormat(L"<eckqpsxtfhcs>%s</eckqpsxtfhcs>", strT);
			OBDData.AppendFormat(L"<eckqpsxtiupr>%s</eckqpsxtiupr>", strTemp);
		}
		else
		{
			OBDData.AppendFormat(L"<eckqpsxtjccs>%s</eckqpsxtjccs>", L"");
			OBDData.AppendFormat(L"<eckqpsxtfhcs>%s</eckqpsxtfhcs>", L"");
			OBDData.AppendFormat(L"<eckqpsxtiupr>%s</eckqpsxtiupr>", L"");
		}
	}

	OBDData.AppendFormat(L"</obd></root>");


	std::wstring strJkid(L"11C22");

	std::wstring strRet;
	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), strJkid, OBDData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
					strMsg.AppendFormat(L"�ϴ�OBD IUPR-%s,%s", strCode.c_str(), strContent.c_str());
					m_edMessAge.SetWindowTextW(strMsg);
				}
			}
			else
			{
				strMsg.AppendFormat(L"�ϴ�OBD IUPR-%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				m_edMessAge.SetWindowTextW(strMsg);
			}
		}
	}
	else
	{
		m_edMessAge.SetWindowTextW(L"�ϴ�OBD IUPR-����ʧ��");
		return false;
	}

	return true;
}

void CSGOBDDlg::GetOBDTimeDataASM(CString& strOBDJQMJDKD, CString& strMMOBDJSFHZ, CString& strMMOBDQYCGQXH, CString& strMMOBDSPEED,
		CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDJQYL, CString& strMMOBDLAMBDA, const float& fSpeed, const int& nCou)
{
	srand(time(0));
	int n = rand()%7;

	// MMOBDJQMJDKD->�����ž��Կ���
	//MMOBDJSFHZ->���㸺��ֵ
	//MMOBDQYCGQXH->ǰ���������ź�
	//MMOBDJQYL->����ѹ��
	if (fSpeed < 10.0f)
	{
		if (n ==0)
		{
			strOBDJQMJDKD.Format(L"%.2f", 8.63f);
			strMMOBDJSFHZ.Format(L"%.2f", 16.08f);
			strMMOBDQYCGQXH.Format(L"%.2f", 667.61f);
			strMMOBDJQYL.Format(L"%d", 26);
		}
		else if ((n%7) == 1)
		{
			strOBDJQMJDKD.Format(L"%.2f", 8.24f);
			strMMOBDJSFHZ.Format(L"%.1f", 25.1f);
			strMMOBDQYCGQXH.Format(L"%.2f", 742.9f);
			strMMOBDJQYL.Format(L"%d", 41);
		}
		else if ((n%7) == 2)
		{
			strOBDJQMJDKD.Format(L"%.2f", 12.55f);
			strMMOBDJSFHZ.Format(L"%.2f", 17.65f);
			strMMOBDQYCGQXH.Format(L"%.2f", 110.43f);
			strMMOBDJQYL.Format(L"%d", 26);
		}
		else if ((n%7) == 3)
		{
			strOBDJQMJDKD.Format(L"%.2f", 7.45f);
			strMMOBDJSFHZ.Format(L"%.2f", 18.43f);
			strMMOBDQYCGQXH.Format(L"%.2f", 120.47f);
			strMMOBDJQYL.Format(L"%d", 37);
		}
		else
		{
			strOBDJQMJDKD.Format(L"%d", 0);
			strMMOBDJSFHZ.Format(L"%d", 0);
			strMMOBDQYCGQXH.Format(L"%d", 0);
			strMMOBDJQYL.Format(L"%d", 0);
		}
	}
	else if (fSpeed >= 10.0f && fSpeed < 27.0f)
	{
		if (n ==0)
		{
			strOBDJQMJDKD.Format(L"%.2f", 22.75f);
			strMMOBDJSFHZ.Format(L"%.2f", 43.53f);
			strMMOBDQYCGQXH.Format(L"%.2f", 707.76f);
			strMMOBDJQYL.Format(L"%d", 57);
		}
		else if ((n%7) == 1)
		{
			strOBDJQMJDKD.Format(L"%.2f", 22.35f);
			strMMOBDJSFHZ.Format(L"%.2f", 41.57f);
			strMMOBDQYCGQXH.Format(L"%.2f", 752.94f);
			strMMOBDJQYL.Format(L"%d", 41);
		}
		else if ((n%7) == 2)
		{
			strOBDJQMJDKD.Format(L"%.2f", 23.92f);
			strMMOBDJSFHZ.Format(L"%.2f", 43.53f);
			strMMOBDQYCGQXH.Format(L"%.2f", 727.84f);
			strMMOBDJQYL.Format(L"%d", 38);
		}
		else if ((n%7) == 3)
		{
			strOBDJQMJDKD.Format(L"%.2f", 19.22f);
			strMMOBDJSFHZ.Format(L"%.2f", 39.61f);
			strMMOBDQYCGQXH.Format(L"%.2f", 747.92f);
			strMMOBDJQYL.Format(L"%d", 43);
		}
		else
		{
			strOBDJQMJDKD.Format(L"%d", 0);
			strMMOBDJSFHZ.Format(L"%d", 0);
			strMMOBDQYCGQXH.Format(L"%d", 0);
			strMMOBDJQYL.Format(L"%d", 0);
		}
	}
	else
	{
		if (n ==0)
		{
			strOBDJQMJDKD.Format(L"%.1f", 24.8f);
			strMMOBDJSFHZ.Format(L"%.2f", 74.9f);
			strMMOBDQYCGQXH.Format(L"%.2f", 788.08f);
			strMMOBDJQYL.Format(L"%d", 59);
		}
		else if ((n%7) == 1)
		{
			strOBDJQMJDKD.Format(L"%.2f", 25.88f);
			strMMOBDJSFHZ.Format(L"%.2f", 53.33f);
			strMMOBDQYCGQXH.Format(L"%.2f", 737.88f);
			strMMOBDJQYL.Format(L"%d", 41);
		}
		else if ((n%7) == 2)
		{
			strOBDJQMJDKD.Format(L"%.2f", 24.31f);
			strMMOBDJSFHZ.Format(L"%.2f", 61.96f);
			strMMOBDQYCGQXH.Format(L"%.2f", 778.04f);
			strMMOBDJQYL.Format(L"%d", 46);
		}
		else if ((n%7) == 3)
		{
			strOBDJQMJDKD.Format(L"%.1f", 25.1f);
			strMMOBDJSFHZ.Format(L"%.2f", 51.76f);
			strMMOBDQYCGQXH.Format(L"%.2f", 833.25f);
			strMMOBDJQYL.Format(L"%d", 57);
		}
		else
		{
			strOBDJQMJDKD.Format(L"%d", 0);
			strMMOBDJSFHZ.Format(L"%d", 0);
			strMMOBDQYCGQXH.Format(L"%d", 0);
			strMMOBDJQYL.Format(L"%d", 0);
		}
	}
	
	//MMOBDLAMBDA->��������ϵ�����ˣ�
	//MMOBDJQL->������
	if (n ==0)
	{
		strMMOBDLAMBDA.Format(L"%d", 0);
		strMMOBDJQL.Format(L"%d", 0);
	}
	else if ((n%7) == 1)
	{
		strMMOBDLAMBDA.Format(L"%d", 1);
		strMMOBDJQL.Format(L"%.2f", 1.76);
	}
	else if ((n%7) == 2)
	{
		strMMOBDLAMBDA.Format(L"%d", 2);
		strMMOBDJQL.Format(L"%.2f", 2.44);
	}
	else if ((n%7) == 3)
	{
		strMMOBDLAMBDA.Format(L"%d", 4);
		strMMOBDJQL.Format(L"%.2f", 20.83);
	}
	else
	{
		strMMOBDLAMBDA.Format(L"%d", 0);
		strMMOBDJQL.Format(L"%d", 0);
	}

	SYSTEMTIME st;
	//MMOBDSPEED->����
	//MMOBDROTATESPEED->ת��
	if (fSpeed > 1.0f)
	{
		GetLocalTime(&st);
		srand(st.wMilliseconds);
		float f;
		if (nCou%2 == 0) { f = (rand()%2 * 2) + fSpeed +0.5f;}
		else if (nCou%3 == 0) { f = (rand()%2 * 2) + fSpeed -1.5f;}
		else if (nCou%5 == 0) { f = (rand()%2 * -1) + fSpeed +2.5f;}
		else { f = (rand()%2 * -1) + fSpeed +1.0f;}
		strMMOBDSPEED.Format(L"%.2f", f);

		int nZS(0);
		if (nCou%2 == 0){nZS = rand()%300 + 1752;}
		else if (nCou%3 == 0){nZS = rand()%300 + 1825;}
		else if (nCou%5 == 0){nZS = rand()%300 + 1872;}
		else {nZS = rand()%300 + 1852;}
		strMMOBDROTATESPEED.Format(L"%d", nZS);

	}
	else
	{
		strMMOBDSPEED.Format(L"%.2f", 0.00f);

		GetLocalTime(&st);
		srand(st.wMilliseconds);
		int nZS(0);
		if (nCou%2 == 0){nZS = rand()%300 + 650;}
		else if (nCou%3 == 0){nZS = rand()%300 + 678;}
		else if (nCou%5 == 0){nZS = rand()%300 + 687;}
		else {nZS = rand()%300 + 640;}
		strMMOBDROTATESPEED.Format(L"%d", nZS);
	}
}

void CSGOBDDlg::GetOBDTimeDataSDS(CString& strOBDJQMJDKD, CString& strMMOBDJSFHZ, CString& strMMOBDQYCGQXH, CString& strMMOBDSPEED,
	CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDJQYL, CString& strMMOBDLAMBDA, const int& nZS, const int& nCou)
{
	srand(time(0));
	int n = rand()%7;

	// MMOBDJQMJDKD->�����ž��Կ���
	//MMOBDJSFHZ->���㸺��ֵ
	//MMOBDQYCGQXH->ǰ���������ź�
	//MMOBDJQYL->����ѹ��
	if (nZS < 1200)
	{
		if (n ==0)
		{
			strOBDJQMJDKD.Format(L"%.2f", 8.63f);
			strMMOBDJSFHZ.Format(L"%.2f", 16.08f);
			strMMOBDQYCGQXH.Format(L"%.2f", 667.61f);
			strMMOBDJQYL.Format(L"%d", 26);
		}
		else if ((n%7) == 1)
		{
			strOBDJQMJDKD.Format(L"%.2f", 8.24f);
			strMMOBDJSFHZ.Format(L"%.1f", 25.1f);
			strMMOBDQYCGQXH.Format(L"%.2f", 742.9f);
			strMMOBDJQYL.Format(L"%d", 41);
		}
		else if ((n%7) == 2)
		{
			strOBDJQMJDKD.Format(L"%.2f", 12.55f);
			strMMOBDJSFHZ.Format(L"%.2f", 17.65f);
			strMMOBDQYCGQXH.Format(L"%.2f", 110.43f);
			strMMOBDJQYL.Format(L"%d", 26);
		}
		else if ((n%7) == 3)
		{
			strOBDJQMJDKD.Format(L"%.2f", 7.45f);
			strMMOBDJSFHZ.Format(L"%.2f", 18.43f);
			strMMOBDQYCGQXH.Format(L"%.2f", 120.47f);
			strMMOBDJQYL.Format(L"%d", 37);
		}
		else
		{
			strOBDJQMJDKD.Format(L"%d", 0);
			strMMOBDJSFHZ.Format(L"%d", 0);
			strMMOBDQYCGQXH.Format(L"%d", 0);
			strMMOBDJQYL.Format(L"%d", 0);
		}
	}
	else if (nZS >= 1200 && nZS < 2800)
	{
		if (n ==0)
		{
			strOBDJQMJDKD.Format(L"%.2f", 22.75f);
			strMMOBDJSFHZ.Format(L"%.2f", 43.53f);
			strMMOBDQYCGQXH.Format(L"%.2f", 707.76f);
			strMMOBDJQYL.Format(L"%d", 57);
		}
		else if ((n%7) == 1)
		{
			strOBDJQMJDKD.Format(L"%.2f", 22.35f);
			strMMOBDJSFHZ.Format(L"%.2f", 41.57f);
			strMMOBDQYCGQXH.Format(L"%.2f", 752.94f);
			strMMOBDJQYL.Format(L"%d", 41);
		}
		else if ((n%7) == 2)
		{
			strOBDJQMJDKD.Format(L"%.2f", 23.92f);
			strMMOBDJSFHZ.Format(L"%.2f", 43.53f);
			strMMOBDQYCGQXH.Format(L"%.2f", 727.84f);
			strMMOBDJQYL.Format(L"%d", 38);
		}
		else if ((n%7) == 3)
		{
			strOBDJQMJDKD.Format(L"%.2f", 19.22f);
			strMMOBDJSFHZ.Format(L"%.2f", 39.61f);
			strMMOBDQYCGQXH.Format(L"%.2f", 747.92f);
			strMMOBDJQYL.Format(L"%d", 43);
		}
		else
		{
			strOBDJQMJDKD.Format(L"%d", 0);
			strMMOBDJSFHZ.Format(L"%d", 0);
			strMMOBDQYCGQXH.Format(L"%d", 0);
			strMMOBDJQYL.Format(L"%d", 0);
		}
	}
	else
	{
		if (n ==0)
		{
			strOBDJQMJDKD.Format(L"%.1f", 24.8f);
			strMMOBDJSFHZ.Format(L"%.2f", 74.9f);
			strMMOBDQYCGQXH.Format(L"%.2f", 788.08f);
			strMMOBDJQYL.Format(L"%d", 59);
		}
		else if ((n%7) == 1)
		{
			strOBDJQMJDKD.Format(L"%.2f", 25.88f);
			strMMOBDJSFHZ.Format(L"%.2f", 53.33f);
			strMMOBDQYCGQXH.Format(L"%.2f", 737.88f);
			strMMOBDJQYL.Format(L"%d", 41);
		}
		else if ((n%7) == 2)
		{
			strOBDJQMJDKD.Format(L"%.2f", 24.31f);
			strMMOBDJSFHZ.Format(L"%.2f", 61.96f);
			strMMOBDQYCGQXH.Format(L"%.2f", 778.04f);
			strMMOBDJQYL.Format(L"%d", 46);
		}
		else if ((n%7) == 3)
		{
			strOBDJQMJDKD.Format(L"%.1f", 25.1f);
			strMMOBDJSFHZ.Format(L"%.2f", 51.76f);
			strMMOBDQYCGQXH.Format(L"%.2f", 833.25f);
			strMMOBDJQYL.Format(L"%d", 57);
		}
		else
		{
			strOBDJQMJDKD.Format(L"%d", 0);
			strMMOBDJSFHZ.Format(L"%d", 0);
			strMMOBDQYCGQXH.Format(L"%d", 0);
			strMMOBDJQYL.Format(L"%d", 0);
		}
	}

	//MMOBDLAMBDA->��������ϵ�����ˣ�
	//MMOBDJQL->������
	if (n ==0)
	{
		strMMOBDLAMBDA.Format(L"%d", 0);
		strMMOBDJQL.Format(L"%d", 0);
	}
	else if ((n%7) == 1)
	{
		strMMOBDLAMBDA.Format(L"%d", 1);
		strMMOBDJQL.Format(L"%.2f", 1.76);
	}
	else if ((n%7) == 2)
	{
		strMMOBDLAMBDA.Format(L"%d", 2);
		strMMOBDJQL.Format(L"%.2f", 2.44);
	}
	else if ((n%7) == 3)
	{
		strMMOBDLAMBDA.Format(L"%d", 4);
		strMMOBDJQL.Format(L"%.2f", 20.83);
	}
	else
	{
		strMMOBDLAMBDA.Format(L"%d", 0);
		strMMOBDJQL.Format(L"%d", 0);
	}

	SYSTEMTIME st;
	//MMOBDSPEED->����
	strMMOBDSPEED.Format(L"%.2f", 0.00f);
	//MMOBDROTATESPEED->ת��
	GetLocalTime(&st);
	srand(st.wMilliseconds);
	int nZSM(0);
	if (nCou%2 == 0){nZSM = rand()%300 + nZS;}
	else if (nCou%3 == 0){nZSM = rand()%300  + nZS;}
	else if (nCou%5 == 0){nZSM = rand()%300  + nZS;}
	else {nZSM = rand()%300  + nZS;}
	strMMOBDROTATESPEED.Format(L"%d", nZSM);
}

void CSGOBDDlg::GetOBDTimeDataJZJS(CString& strMMOBDYMKD, CString& strMMOBDSPEED, CString& strMMOBDPOWER, CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDZYYL, 
	CString& strMMOBDHYL, CString& strMMOBDNOND, CString& strMMOBDNSPSL, CString& strMMOBDWD, CString& strMMOBDKLBJQYC, CString& strMMOBDEGRKD, CString& strMMOBDRYPSYL,
	const float& fSpeed, const int& nCou)
{
	srand(time(0));
	int n = rand()%7;

	//MMOBDYMKD->���ſ���
	//MMOBDJQL->������
	//MMOBDZYYL->��ѹѹ��
	//MMOBDHYL->������
	if (fSpeed < 10.0f)
	{
		if (n ==0)
		{
			strMMOBDYMKD.Format(L"%.2f", 14.90f);
			strMMOBDJQL.Format(L"%.2f", 6.74f);
			strMMOBDZYYL.Format(L"%.2f", 101.10f);
			strMMOBDHYL.Format(L"%.2f", 27.05f);
		}
		else if ((n%7) == 1)
		{
			strMMOBDYMKD.Format(L"%.2f", 20.39f);
			strMMOBDJQL.Format(L"%.2f", 94.99f);
			strMMOBDZYYL.Format(L"%.2f", 99.00f);
			strMMOBDHYL.Format(L"%.2f", 12.55f);
		}
		else if ((n%7) == 2)
		{
			strMMOBDYMKD.Format(L"%.2f", 14.12f);
			strMMOBDJQL.Format(L"%.2f", 19.36f);
			strMMOBDZYYL.Format(L"%.2f", 39.00f);
			strMMOBDHYL.Format(L"%.2f", 6.80f);
		}
		else if ((n%7) == 3)
		{
			strMMOBDYMKD.Format(L"%.2f", 12.16f);
			strMMOBDJQL.Format(L"%.2f", 27.83f);
			strMMOBDZYYL.Format(L"%.2f", 124.00f);
			strMMOBDHYL.Format(L"%.2f", 1.95f);
		}
		else
		{
			strMMOBDYMKD.Format(L"%.2f", 0.00f);
			strMMOBDJQL.Format(L"%.2f", 0.00f);
			strMMOBDZYYL.Format(L"%.2f", 0.00f);
			strMMOBDHYL.Format(L"%.2f", 0.00f);
		}
	}
	else if (fSpeed >= 10.0f && fSpeed < 50.0f)
	{
		if (n ==0)
		{
			strMMOBDYMKD.Format(L"%.2f", 24.70f);
			strMMOBDJQL.Format(L"%.2f", 33.77f);
			strMMOBDZYYL.Format(L"%.2f", 141.00f);
			strMMOBDHYL.Format(L"%.2f", 17.95f);
		}
		else if ((n%7) == 1)
		{
			strMMOBDYMKD.Format(L"%.2f", 42.39f);
			strMMOBDJQL.Format(L"%.2f", 94.99f);
			strMMOBDZYYL.Format(L"%.2f", 120.00f);
			strMMOBDHYL.Format(L"%.2f", 19.15f);
		}
		else if ((n%7) == 2)
		{
			strMMOBDYMKD.Format(L"%.2f", 41.57f);
			strMMOBDJQL.Format(L"%.2f", 39.30f);
			strMMOBDZYYL.Format(L"%.2f", 40.00f);
			strMMOBDHYL.Format(L"%.2f", 10.10f);
		}
		else if ((n%7) == 3)
		{
			strMMOBDYMKD.Format(L"%.2f", 41.96f);
			strMMOBDJQL.Format(L"%.2f", 94.35f);
			strMMOBDZYYL.Format(L"%.2f", 143.00f);
			strMMOBDHYL.Format(L"%.2f", 17.20f);
		}
		else
		{
			strMMOBDYMKD.Format(L"%.2f", 0.00f);
			strMMOBDJQL.Format(L"%.2f", 0.00f);
			strMMOBDZYYL.Format(L"%.2f", 0.00f);
			strMMOBDHYL.Format(L"%.2f", 0.00f);
		}
	}
	else
	{
		if (n ==0)
		{
			strMMOBDYMKD.Format(L"%.2f", 44.71f);
			strMMOBDJQL.Format(L"%.2f", 80.94f);
			strMMOBDZYYL.Format(L"%.2f", 176.00f);
			strMMOBDHYL.Format(L"%.2f", 28.23f);
		}
		else if ((n%7) == 1)
		{
			strMMOBDYMKD.Format(L"%.2f", 61.31f);
			strMMOBDJQL.Format(L"%.2f", 94.99f);
			strMMOBDZYYL.Format(L"%.2f", 195.00f);
			strMMOBDHYL.Format(L"%.2f", 28.75f);
		}
		else if ((n%7) == 2)
		{
			strMMOBDYMKD.Format(L"%.2f", 64.71f);
			strMMOBDJQL.Format(L"%.2f", 64.25f);
			strMMOBDZYYL.Format(L"%.2f", 51.00f);
			strMMOBDHYL.Format(L"%.2f", 20.05f);
		}
		else if ((n%7) == 3)
		{
			strMMOBDYMKD.Format(L"%.2f", 53.33f);
			strMMOBDJQL.Format(L"%.2f", 111.91f);
			strMMOBDZYYL.Format(L"%.2f", 132.00f);
			strMMOBDHYL.Format(L"%.2f", 25.75f);
		}
		else
		{
			strMMOBDYMKD.Format(L"%.2f", 0.00f);
			strMMOBDJQL.Format(L"%.2f", 0.00f);
			strMMOBDZYYL.Format(L"%.2f", 0.00f);
			strMMOBDHYL.Format(L"%.2f", 0.00f);
		}
	}
	
	//MMOBDSPEED->����
	//MMOBDROTATESPEED->ת��
	SYSTEMTIME st;
	if (fSpeed > 1.0f)
	{
		GetLocalTime(&st);
		srand(st.wMilliseconds);
		float f;
		if (nCou%2 == 0) { f = (rand()%2 * 2) + fSpeed +0.5f;}
		else if (nCou%3 == 0) { f = (rand()%2 * 2) + fSpeed -1.5f;}
		else if (nCou%5 == 0) { f = (rand()%2 * -1) + fSpeed +2.5f;}
		else { f = (rand()%2 * -1) + fSpeed +1.0f;}
		strMMOBDSPEED.Format(L"%.2f", f);

		int nZS(0);
		if (nCou%2 == 0){nZS = rand()%300 + 1752;}
		else if (nCou%3 == 0){nZS = rand()%300 + 1825;}
		else if (nCou%5 == 0){nZS = rand()%300 + 1872;}
		else {nZS = rand()%300 + 1852;}
		strMMOBDROTATESPEED.Format(L"%d", n);
	}
	else
	{
		strMMOBDSPEED.Format(L"%.2f", 0.00f);

		GetLocalTime(&st);
		srand(st.wMilliseconds);

		int nZS(0);
		if (nCou%2 == 0){nZS = rand()%300 + 650;}
		else if (nCou%3 == 0){nZS = rand()%300 + 678;}
		else if (nCou%5 == 0){nZS = rand()%300 + 687;}
		else {nZS = rand()%300 + 640;}
		strMMOBDROTATESPEED.Format(L"%d", n);
	}

	//MMOBDPOWER->�������������
	//MMOBDNOND->����������Ũ��
	//MMOBDNSPSL->����������
	//MMOBDWD->�����¶�
	//MMOBDKLBJQYC->����������ѹ��
	//MMOBDEGRKD->EGR ����
	//MMOBDRYPSYL->ȼ������ѹ��
	strMMOBDPOWER.Format(L"%.2f", 0.00f);
	strMMOBDNOND.Format(L"%.2f", 0.00f);
	strMMOBDNSPSL.Format(L"%.2f", 0.00f);
	strMMOBDWD.Format(L"%.2f", 0.00f);
	strMMOBDKLBJQYC.Format(L"%.2f", 0.00f);
	strMMOBDEGRKD.Format(L"%.2f", 0.00f);
	strMMOBDRYPSYL.Format(L"%.2f", 0.00f);

}

void CSGOBDDlg::GetOBDTimeDataZYJS(CString& strMMOBDYMKD, CString& strMMOBDSPEED, CString& strMMOBDPOWER, CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDZYYL, 
	CString& strMMOBDHYL, CString& strMMOBDNOND, CString& strMMOBDNSPSL, CString& strMMOBDWD, CString& strMMOBDKLBJQYC, CString& strMMOBDEGRKD, CString& strMMOBDRYPSYL,
	const int& nZS, const int& nCou)
{
	srand(time(0));
	int n = rand()%7;

	//MMOBDYMKD->���ſ���
	//MMOBDJQL->������
	//MMOBDZYYL->��ѹѹ��
	//MMOBDHYL->������
	if (nZS < 1200)
	{
		if (n ==0)
		{
			strMMOBDYMKD.Format(L"%.2f", 14.90f);
			strMMOBDJQL.Format(L"%.2f", 6.74f);
			strMMOBDZYYL.Format(L"%.2f", 101.10f);
			strMMOBDHYL.Format(L"%.2f", 27.05f);
		}
		else if ((n%7) == 1)
		{
			strMMOBDYMKD.Format(L"%.2f", 20.39f);
			strMMOBDJQL.Format(L"%.2f", 94.99f);
			strMMOBDZYYL.Format(L"%.2f", 99.00f);
			strMMOBDHYL.Format(L"%.2f", 12.55f);
		}
		else if ((n%7) == 2)
		{
			strMMOBDYMKD.Format(L"%.2f", 14.12f);
			strMMOBDJQL.Format(L"%.2f", 19.36f);
			strMMOBDZYYL.Format(L"%.2f", 39.00f);
			strMMOBDHYL.Format(L"%.2f", 6.80f);
		}
		else if ((n%7) == 3)
		{
			strMMOBDYMKD.Format(L"%.2f", 12.16f);
			strMMOBDJQL.Format(L"%.2f", 27.83f);
			strMMOBDZYYL.Format(L"%.2f", 124.00f);
			strMMOBDHYL.Format(L"%.2f", 1.95f);
		}
		else
		{
			strMMOBDYMKD.Format(L"%.2f", 0.00f);
			strMMOBDJQL.Format(L"%.2f", 0.00f);
			strMMOBDZYYL.Format(L"%.2f", 0.00f);
			strMMOBDHYL.Format(L"%.2f", 0.00f);
		}
	}
	else if (nZS >= 1200 && nZS < 2700)
	{
		if (n ==0)
		{
			strMMOBDYMKD.Format(L"%.2f", 24.70f);
			strMMOBDJQL.Format(L"%.2f", 33.77f);
			strMMOBDZYYL.Format(L"%.2f", 141.00f);
			strMMOBDHYL.Format(L"%.2f", 17.95f);
		}
		else if ((n%7) == 1)
		{
			strMMOBDYMKD.Format(L"%.2f", 42.39f);
			strMMOBDJQL.Format(L"%.2f", 94.99f);
			strMMOBDZYYL.Format(L"%.2f", 120.00f);
			strMMOBDHYL.Format(L"%.2f", 19.15f);
		}
		else if ((n%7) == 2)
		{
			strMMOBDYMKD.Format(L"%.2f", 41.57f);
			strMMOBDJQL.Format(L"%.2f", 39.30f);
			strMMOBDZYYL.Format(L"%.2f", 40.00f);
			strMMOBDHYL.Format(L"%.2f", 10.10f);
		}
		else if ((n%7) == 3)
		{
			strMMOBDYMKD.Format(L"%.2f", 41.96f);
			strMMOBDJQL.Format(L"%.2f", 94.35f);
			strMMOBDZYYL.Format(L"%.2f", 143.00f);
			strMMOBDHYL.Format(L"%.2f", 17.20f);
		}
		else
		{
			strMMOBDYMKD.Format(L"%.2f", 0.00f);
			strMMOBDJQL.Format(L"%.2f", 0.00f);
			strMMOBDZYYL.Format(L"%.2f", 0.00f);
			strMMOBDHYL.Format(L"%.2f", 0.00f);
		}
	}
	else
	{
		if (n ==0)
		{
			strMMOBDYMKD.Format(L"%.2f", 44.71f);
			strMMOBDJQL.Format(L"%.2f", 80.94f);
			strMMOBDZYYL.Format(L"%.2f", 176.00f);
			strMMOBDHYL.Format(L"%.2f", 28.23f);
		}
		else if ((n%7) == 1)
		{
			strMMOBDYMKD.Format(L"%.2f", 61.31f);
			strMMOBDJQL.Format(L"%.2f", 94.99f);
			strMMOBDZYYL.Format(L"%.2f", 195.00f);
			strMMOBDHYL.Format(L"%.2f", 28.75f);
		}
		else if ((n%7) == 2)
		{
			strMMOBDYMKD.Format(L"%.2f", 64.71f);
			strMMOBDJQL.Format(L"%.2f", 64.25f);
			strMMOBDZYYL.Format(L"%.2f", 51.00f);
			strMMOBDHYL.Format(L"%.2f", 20.05f);
		}
		else if ((n%7) == 3)
		{
			strMMOBDYMKD.Format(L"%.2f", 53.33f);
			strMMOBDJQL.Format(L"%.2f", 111.91f);
			strMMOBDZYYL.Format(L"%.2f", 132.00f);
			strMMOBDHYL.Format(L"%.2f", 25.75f);
		}
		else
		{
			strMMOBDYMKD.Format(L"%.2f", 0.00f);
			strMMOBDJQL.Format(L"%.2f", 0.00f);
			strMMOBDZYYL.Format(L"%.2f", 0.00f);
			strMMOBDHYL.Format(L"%.2f", 0.00f);
		}
	}
	
	//MMOBDSPEED->����
	strMMOBDSPEED.Format(L"%.2f", 0.00f);
	//MMOBDROTATESPEED->ת��
	SYSTEMTIME st;
	GetLocalTime(&st);
	srand(st.wMilliseconds);
	int nZSM(0);
	if (nCou%2 == 0){nZSM = rand()%300 + nZS;}
	else if (nCou%3 == 0){nZSM = rand()%300  + nZS;}
	else if (nCou%5 == 0){nZSM = rand()%300  + nZS;}
	else {nZSM = rand()%300  + nZS;}
	strMMOBDROTATESPEED.Format(L"%d", nZSM);

	//MMOBDPOWER->�������������
	//MMOBDNOND->����������Ũ��
	//MMOBDNSPSL->����������
	//MMOBDWD->�����¶�
	//MMOBDKLBJQYC->����������ѹ��
	//MMOBDEGRKD->EGR ����
	//MMOBDRYPSYL->ȼ������ѹ��
	strMMOBDPOWER.Format(L"%.2f", 0.00f);
	strMMOBDNOND.Format(L"%.2f", 0.00f);
	strMMOBDNSPSL.Format(L"%.2f", 0.00f);
	strMMOBDWD.Format(L"%.2f", 0.00f);
	strMMOBDKLBJQYC.Format(L"%.2f", 0.00f);
	strMMOBDEGRKD.Format(L"%.2f", 0.00f);
	strMMOBDRYPSYL.Format(L"%.2f", 0.00f);
}

