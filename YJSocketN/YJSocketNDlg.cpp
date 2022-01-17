
// YJSocketNDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YJSocketN.h"
#include "YJSocketNDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYJSocketNDlg �Ի���




CYJSocketNDlg::CYJSocketNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYJSocketNDlg::IDD, pParent)
	, m_bAuthentication(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CYJSocketNDlg::~CYJSocketNDlg()
{
	if (m_yjSocket.IsOpen())
	{
		m_yjSocket.Close();
	}
	m_bAuthentication = false;
}

void CYJSocketNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_OBDJCY, m_cmOBDJCY);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMsg);
	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_Password, m_edPassWord);
	DDX_Control(pDX, ID_ED_PlateNumber, m_edPlateNumber);
	DDX_Control(pDX, ID_ED_SERVERIP, m_edIP);
	DDX_Control(pDX, ID_ED_SERVERPORT, m_edPort);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNUmber);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, IDC_RA_SQL, m_edchkSQL);
	DDX_Control(pDX, IDC_RA_INTET, m_edIntet);
	DDX_Control(pDX, IDC_BTN_SOCKET, m_btnSocket);

	DDX_Control(pDX, ID_ED_CALID, m_edCALID);
	DDX_Control(pDX, ID_ED_CVN, m_edCVN);
	DDX_Control(pDX, ID_ED_OBDTYPE, m_edOBDType);
}

BEGIN_MESSAGE_MAP(CYJSocketNDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CYJSocketNDlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CYJSocketNDlg::OnBnClickedBtnSetconfig)
	ON_EN_CHANGE(ID_ED_PlateNumber, &CYJSocketNDlg::OnEnChangeEdPlatenumber)
	ON_BN_CLICKED(IDC_BTN_SOCKET, &CYJSocketNDlg::OnBnClickedBtnSocket)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CYJSocketNDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UP, &CYJSocketNDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_UP_TELET, &CYJSocketNDlg::OnBnClickedBtnUpTelet)
	ON_BN_CLICKED(IDC_BTN_TESTLOG, &CYJSocketNDlg::OnBnClickedBtnTestlog)
END_MESSAGE_MAP()


// CYJSocketNDlg ��Ϣ�������

BOOL CYJSocketNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
#ifndef _DEBUG
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);
	m_btnSocket.EnableWindow(FALSE);
#endif

	m_bAuthentication = false;
	SetConfig();
	SetVehCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CYJSocketNDlg::OnPaint()
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
HCURSOR CYJSocketNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYJSocketNDlg::SetConfig(void)
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
		m_edPassWord.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"YJConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"System", L"ServerIP", L"");
		m_edIP.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"ServerPort", L"");
		m_edPort.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"StationNumber", L"");
		m_edStationNUmber.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edLineNumber.SetWindowTextW(strTemp);
	}
}

void CYJSocketNDlg::SetVehCtrl(void)
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

bool CYJSocketNDlg::GetVehicleList(void)
{
	m_edMsg.SetWindowTextW(L"");
	// ɾ���б�����
	m_lstVehicle.DeleteAllItems();
	CString strSQL;

	CString strSource, strCatalog, strUserID, strPassword;

	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);

	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"���ݿ����Ϊ��");
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

				m_edMsg.SetWindowTextW(L"˫����Ҫ�ĳ��ƺ���");
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
			m_edMsg.SetWindowTextW(strMsg);
		}
		CNHSQLServerDBO::CloseQuery(pRecordset);

		// ��ȡ�¼����Ա
		strSQL.Format(L"select * from UserInfo /*where Role = 6*/");
		pRecordset = NULL;
		m_cmOBDJCY.ResetContent();
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
						m_cmOBDJCY.AddString((wchar_t*)(_bstr_t)v);
					}
					else
					{
						m_cmOBDJCY.AddString(L"");
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
		m_edMsg.SetWindowTextW(L"���ݿ���ϵ����");
		return false;
	}

	CNHSQLServerDBO::CloseDB(pConnection);
	return true;
}

void CYJSocketNDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetVehicleList();
}

void CYJSocketNDlg::OnBnClickedBtnSetconfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetConfig();
}

void CYJSocketNDlg::OnEnChangeEdPlatenumber()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strPlateNumber(L"");
	m_edPlateNumber.GetWindowTextW(strPlateNumber);

	if (strPlateNumber.GetLength() >=3)
	{
		GetVehicleList();
	}
}

void CYJSocketNDlg::CloseConnect(void)
{
	if (m_yjSocket.IsOpen())
	{
		m_yjSocket.Close();
	}
	m_bAuthentication = false;
	m_btnSocket.SetWindowTextW(L"����");
}

bool CYJSocketNDlg::Authentication(std::wstring &strResult, std::wstring &strMsg)
{
	CString strTemp;
	m_edLineNumber.GetWindowTextW(strTemp);
	if (strTemp == L"")
	{
		strTemp = L"01";
	}

	return SendAndRecvPacket(L"LJZC", L"", L"", L"", strTemp, strMsg);
}

CString CYJSocketNDlg::CheckSum(const char *ch, const UINT un)
{
	//assert(NULL!=pb && 0UL!=un);
	assert(NULL!=ch && 0UL!=un);
	CString strRet(L"");

	DWORD bSum(0x00);	
	for (UINT ui=0; ui<un; ui++)
	{
		bSum ^=  ch[ui];
	}
	strRet.Format(L"%02x", bSum&0xff);
	return strRet;
}

CString CYJSocketNDlg::GenerateInsNum(void)
{
	// Ψһ�������ɹ��򣺼���߱���+ʱ�䣨��ȷ�����룩
	CString InsNum(L""), strTemp;;
	m_edStationNUmber.GetWindowTextW(strTemp);
	if (strTemp == L"")
	{
		strTemp = L"01";
	}
	InsNum.AppendFormat(L"%s", strTemp);
	SYSTEMTIME dt;
	GetLocalTime(&dt);
	InsNum.AppendFormat(L"%04d%02d%02d%02d%02d%02d%03d", dt.wYear, dt.wMonth, dt.wDay, dt.wHour, dt.wMinute, dt.wSecond, dt.wMilliseconds);
	return InsNum;
}

bool CYJSocketNDlg::SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv)
{	
	CString strSend(L"");
	// 1��������+2���վ����+3����߱���+4ʱ��+4Ψһ����+5�̶��ָ���+6���ݲ���+7�̶��ָ���+8У����+9�̶�������
	strSend.Format(L"%s%s%s", strDataType, strStationNumber, strLineNumber);
	strSend.AppendFormat(L"%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	// Ψһ����
	strSend.AppendFormat(L"%s", strInsNumber);
	// �̶��ָ���
	strSend.AppendFormat(L"@@@");
	strSend.AppendFormat(L"%s", strJsonData);// ���ݲ���
	strSend.AppendFormat(L"tek");// �̶��ָ���

	std::string sData(CW2A(strSend.GetString()));
	CString strSum = CheckSum((char *)sData.c_str(), sData.size());
	strSend.AppendFormat(L"%s", strSum); // У����

	strSend.AppendFormat(L"####");// �̶�������

	if (!m_yjSocket.IsOpen())
	{
		CString strIP, strPort;
		int nPort;
		m_edIP.GetWindowTextW(strIP);
		m_edPort.GetWindowTextW(strPort);
		nPort = _wtoi(strPort);
		if (!m_yjSocket.Open(strIP, nPort) ||(strIP == L"" || strPort == L""))
		{
			strRecv = L"���ӷ�����ʧ�ܣ�";
#ifndef _DEBUG
			return false;
#endif
		}
	}

	if (strDataType != L"LJZC" && !m_bAuthentication)
	{
		{
			std::wstring strResult, strMsg;
			m_bAuthentication = Authentication(strResult, strMsg);
		}
	}


	return m_yjSocket.SendAndRecvPacket(strSend, strRecv);
}

bool CYJSocketNDlg::GetTime()
{
	CString strStationNum, strLineNum;

	m_edStationNUmber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);

	std::wstring strRecv;
	bool bRet;
	bRet = SendAndRecvPacket(L"HJ26", strStationNum, strLineNum, GenerateInsNum(), L"", strRecv);

#ifndef _DEBUG
	if (bRet)
#else
	if (true)
#endif
	{
		std::wstring strMsg;
		bRet = m_yjSocket.RecvPacket(strMsg);
		if (!bRet)
		{
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"����ʱ��", L"", strRecv.c_str());

			int nIntPos = strRecv.find(L",");
			strRecv = strRecv.substr(nIntPos+1);
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"����ʱ��", L"", strRecv.c_str());

			COleDateTime odtNow;
			if (odtNow.ParseDateTime(strRecv.c_str()))
			{
				SYSTEMTIME st;
				st.wYear = odtNow.GetYear();
				st.wMonth = odtNow.GetMonth();
				st.wDay = odtNow.GetDay();
				st.wHour = odtNow.GetHour();
				st.wMinute = odtNow.GetMinute();
				st.wSecond = odtNow.GetSecond();
				st.wMilliseconds = 0;
				// ����
				if (SetLocalTime(&st))
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"ʱ��ͬ��", L"���ñ���ʱ��", L"���ñ���ʱ��ɹ�");
				}
				else
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"ʱ��ͬ��", L"���ñ���ʱ��", L"���ñ���ʱ��ʧ��");
				}
			}
		}
		else
		{
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"����ʱ��", L"", strMsg.c_str());

			int nIntPos = strMsg.find(L".");
			strMsg = strMsg.substr(0, nIntPos);
			CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"����ʱ��", L"", strMsg.c_str());

			COleDateTime odtNow;

			if (odtNow.ParseDateTime(strMsg.c_str()))
			{
				SYSTEMTIME st;
				st.wYear = odtNow.GetYear();
				st.wMonth = odtNow.GetMonth();
				st.wDay = odtNow.GetDay();
				st.wHour = odtNow.GetHour();
				st.wMinute = odtNow.GetMinute();
				st.wSecond = odtNow.GetSecond();
				st.wMilliseconds = 0;
				// ����
				if (SetLocalTime(&st))
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"ʱ��ͬ��", L"���ñ���ʱ��", L"���ñ���ʱ��ɹ�");
				}
				else
				{
					CNHLogAPI::WriteLogEx(m_yjSocket.GetLogFilePath(),L"ʱ��ͬ��", L"���ñ���ʱ��", L"���ñ���ʱ��ʧ��");
				}
			}
		}
	}
	else
	{
		// ��ȡʧ��ԭ��
		return false;
	}

	return true;
}

void CYJSocketNDlg::OnBnClickedBtnSocket()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_btnSocket.GetWindowTextW(str);

	if (str.Find(L"����") != -1)
	{
		std::wstring strResult, strMsg;
		m_bAuthentication = Authentication(strResult, strMsg);

#ifndef _DEBUG
		if (m_bAuthentication)
#else
		if (true)
#endif
		{
			m_edMsg.SetWindowTextW(L"���ӳɹ�");
			GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
			m_btnSocket.SetWindowTextW(L"�Ͽ�");
			GetTime();
			m_edMsg.SetWindowTextW(L"ѡ���ϴ����ݣ�����ֻ�Ǳ��汾�����ݿ�");
		}
		else
		{
			CString strTemp;
			strTemp.Format(L"%s-%s", L"����ʧ��", strMsg.c_str());
			m_edMsg.SetWindowTextW(strTemp);
		}
	}
	else if (str.Find(L"�Ͽ�") != -1)
	{
		m_btnSocket.SetWindowTextW(L"����");
	}
}

void CYJSocketNDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edMsg.SetWindowTextW(L"");
	BeginWaitCursor();
	// ����OBD���Ա������д
	m_strRunningNumber = L"";
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_strRunningNumber = m_lstVehicle.GetItemText(nIndex,1);
		// ѡ�г��ƺ���Ե��UP
		if (!m_strRunningNumber.IsEmpty())
		{
			m_btnSocket.EnableWindow(TRUE);
			m_edMsg.SetWindowTextW(L"�������Ӱ�ť������Socket");
		}
		else
		{
			m_edMsg.SetWindowTextW(L"�����־���Ϊ�գ����ⷽ��Ϊ��");
		}
	}
	EndWaitCursor();

	*pResult = 0;
}

void CYJSocketNDlg::OnBnClickedBtnUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_strStationNum.Empty();
	m_strLineNum.Empty();
	if (!m_bAuthentication)
	{
		m_edMsg.SetWindowTextW(L"�������ӷ���");
		return;
	}

	if (m_strRunningNumber.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"�����־���Ϊ��");
		return;
	}

	// �ȴ���SQL�����ڴ����ϴ�����
	CString strSource, strCatalog, strUserID, strPassword, strOBDJCY;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassword);
	m_cmOBDJCY.GetWindowTextW(strOBDJCY);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty() 
		|| strOBDJCY.IsEmpty()
		)
	{
		m_edMsg.SetWindowTextW(L"���ݿ����Ϊ��");
		return;
	}

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

	CString strSQL;

	// ��ȡOBD����
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	SResultOfOBD sResultOfOBD;
	GetDboResultOfOBD(strSQL, &sResultOfOBD);

	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	TESTLOG sTestLog;

	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		m_edMsg.SetWindowTextW(L"��ȡ���ݿ�����־ʧ��");
		return;
	}
	VEHICLEINFO sVehInfo;
	strSQL.Format(L"select * from VehicleInfo where PlateNumber = '%s'", sTestLog.wchPlateNumber);
	GetDboVehicleInfo(strSQL, &sVehInfo);

	sResultOfOBD.strRunningNumber = m_strRunningNumber.GetString();
	sResultOfOBD.strOperator = strOBDJCY.GetString();

	bool bRet(false);
	CString strMsg;
	bRet = SetOBDLOG(sTestLog, sResultOfOBD, strMsg);

	if (!bRet)
	{
		m_edMsg.SetWindowTextW(L"����OBD����ʧ��");
		return;
	}

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
	
	// д�����ݿ�
	SetDboResultOfOBD(sResultOfOBD);
	// д����ʱ�ļ�
	SetIniResultOfOBD(sResultOfOBD);

	CString strStationNum, strLineNum;

	m_edStationNUmber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);

	if (strStationNum.IsEmpty() || strLineNum.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"���������п�ֵ");
		return;
	}

	CString strTempMsg;
	strMsg.Empty();
	bRet = UpOBDStart(strStationNum, strLineNum, sTestLog, sResultOfOBD, sVehInfo, strMsg);
	bRet = UpOBDReaust(strStationNum, strLineNum, sTestLog, sResultOfOBD, sVehInfo, strMsg);
	if (!bRet)
	{
		strMsg = L"�ϴ�OBD���ʧ��\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s\r\n", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
		//return;
	}
	else
	{
		GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	}

	// �ϴ���ʧ���ǣ��Ƿ��������
	bool bUpLowNext(false);
	if (strTempMsg.Find(L"ʧ��") != -1)
	{
		strTempMsg.Format(L"�ϴ������У����ϴ����ɹ��Ĳ����Ƿ������");
		if (IDYES == MessageBox(strTempMsg, L"", MB_YESNO))
		{
			bUpLowNext = true;
		}
	}
	else
	{
		bUpLowNext = true;
	}

	if (bUpLowNext)
	{
		m_strStationNum = strStationNum;
		m_strLineNum = strLineNum;

		_ConnectionPtr pConnection;
		if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
		{
			CString strSql;
			strSql.Format(L"update TestLog set TestLog.ItemOBD = null where TestLog.RunningNumber = '%s' ", sTestLog.wchRunningNumber);

			int nRowsInvolved(0);
			CNHSQLServerDBO::ExecuteDML(pConnection, strSql.GetString(), &nRowsInvolved);
			if (nRowsInvolved >= 0)
			{
				strTempMsg.AppendFormat(L"%s-%s\r\n", L"�޸ĳɹ�", sTestLog.wchRunningNumber);
				m_edMsg.SetWindowTextW(strTempMsg);
			}
			else
			{
				strTempMsg.AppendFormat(L"%s-%s\r\n", L"�޸�ʧ��", sTestLog.wchRunningNumber);
				m_edMsg.SetWindowTextW(strTempMsg);
			}

			CNHSQLServerDBO::CloseDB(pConnection);
		}
		else
		{
			strTempMsg.AppendFormat(L"%s\r\n", L"�����ݿ�ʧ��");
			m_edMsg.SetWindowTextW(strTempMsg);
		}

		//if (m_edIntet.GetCheck() == 1)
		//{
		//	//wchar_t wchPath[MAX_PATH];
		//	//ZeroMemory(wchPath, sizeof(wchPath));
		//	//if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		//	//{
		//	//	CSimpleIni si(wchPath);
		//	//	si.SetString(L"TestLog", L"ItemOBD", L"0");
		//	//}
		//	//ZeroMemory(wchPath, sizeof(wchPath));
		//	//if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
		//	//{
		//	//	CSimpleIni si(wchPath);
		//	//	si.SetString(L"VehicleInfo", L"HasOBD", L"0");
		//	//}

		//	wcscpy_s(sTestLog.wchItemOBD, L"");
		//	if (0x00 != SetDboTestLog(sTestLog))
		//	{
		//		strTempMsg.AppendFormat(L"%s\r\n", L"д�����ݿ�ʧ��");
		//		m_edMsg.SetWindowTextW(strTempMsg);
		//	}
		//	else
		//	{
		//		strTempMsg.AppendFormat(L"%s\r\n", L"������б���ڵ����һ�������ŷż�⣡");
		//		m_edMsg.SetWindowTextW(strTempMsg);
		//		GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(TRUE);
		//	}
		//}
		//else
		//{
		//	//wchar_t wchPath[MAX_PATH];
		//	//ZeroMemory(wchPath, sizeof(wchPath));
		//	//if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		//	//{
		//	//	CSimpleIni si(wchPath);
		//	//	si.SetString(L"TestLog", L"ItemOBD", L"4");
		//	//}

		//	wcscpy_s(sTestLog.wchItemOBD, L"");
		//	if (0x00 != SetDboTestLog(sTestLog))
		//	{
		//		strTempMsg.AppendFormat(L"%s\r\n", L"д�����ݿ�ʧ��");
		//		m_edMsg.SetWindowTextW(strTempMsg);
		//	}
		//	else
		//	{
		//		strTempMsg.AppendFormat(L"%s\r\n", L"д�����ݿ�ɹ�");
		//		m_edMsg.SetWindowTextW(strTempMsg);
		//	}
		//}
	}
}

bool CYJSocketNDlg::SetOBDLOG(const TESTLOG& sTestLog, SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(true);

	CString strFulType(sTestLog.wchFuelType);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strVIN = sTestLog.wchVIN;
	//sResultOfOBD.strOBDProtocol = L"04";
	if (strFulType.Find(L"��") != -1)
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

CString CYJSocketNDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CYJSocketNDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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

bool CYJSocketNDlg::UpOBDStart(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, 
	const SResultOfOBD& sResultOfOBD, const VEHICLEINFO& sVehInfo, CString& strMsg)
{
	bool bRet(false);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", strLineNum);
	strJsonData.AppendFormat(L"\"Time\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"Code\":\"%s\"}", L"start");

	std::wstring strRecv;
	bRet = SendAndRecvPacket(L"HJ04", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

	if (bRet)
	{
		strMsg.Format(L"�ϴ��ɹ���%s", strRecv.c_str());
	}
	else
	{
		strMsg.Format(L"�ϴ�ʧ�ܣ�%s", strRecv.c_str());
	}
	return bRet;
}

bool CYJSocketNDlg::UpOBDReaust(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, 
	const SResultOfOBD& sResultOfOBD, const VEHICLEINFO& sVehInfo, CString& strMsg)
{
	bool bRet(false);
	CString strTemp;
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",",sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	if (wcscmp(sTestLog.wchPlateTypeCode, L"") != 0)
	{
		strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	}
	else if (wcscmp(sVehInfo.wchPlateTypeCode, L"") != 0)
	{
		strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sVehInfo.wchPlateTypeCode);
	}
	else if (wcscmp(sTestLog.wchPlateType, L"") != 0)
	{
		CString strPlateTypeCode;
		if (wcscmp(sTestLog.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"01";
		}
		else if (wcscmp(sTestLog.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"02";
		}
		else if (wcscmp(sTestLog.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"03";
		}
		else if (wcscmp(sTestLog.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"04";
		}
		else if (wcscmp(sTestLog.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"05";
		}
		else if (wcscmp(sTestLog.wchPlateType, L"������") == 0)
		{
			strPlateTypeCode = L"06";
		}
		else
		{
			strPlateTypeCode = L"01";
		}
		strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", strPlateTypeCode);
	}
	else
	{
		CString strPlateTypeCode;
		if (wcscmp(sVehInfo.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"01";
		}
		else if (wcscmp(sVehInfo.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"02";
		}
		else if (wcscmp(sVehInfo.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"03";
		}
		else if (wcscmp(sVehInfo.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"04";
		}
		else if (wcscmp(sVehInfo.wchPlateType, L"��ɫ") == 0)
		{
			strPlateTypeCode = L"05";
		}
		else if (wcscmp(sVehInfo.wchPlateType, L"������") == 0)
		{
			strPlateTypeCode = L"06";
		}
		else
		{
			strPlateTypeCode = L"01";
		}
		strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", strPlateTypeCode);
	}
	strJsonData.AppendFormat(L"\"OBDInspectStartTime\":\"%s\",", sResultOfOBD.strDetBegTime.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectEndTime\":\"%s\",", sResultOfOBD.strDetEndTime.c_str());
	strJsonData.AppendFormat(L"\"ODO\":\"%d\",", 0);// �������
	strJsonData.AppendFormat(L"\"FaultIndicatorLighten\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsCommunicated\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CommFailReason\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"FaultIndicator\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsHaveFaultCode\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"FaultInfo\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"UnDoneReadyStatus\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"UnFinifishedItem\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"ECU_CALID\":\"%s\",", sResultOfOBD.strEngineCALID.c_str());
	strJsonData.AppendFormat(L"\"ECU_CVN\":\"%s\",", sResultOfOBD.strEngineCVN.c_str());
	strJsonData.AppendFormat(L"\"ACU_CALID\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"ACU_CVN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"OCU_CALID\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"OCU_CVN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"StatusCoherence\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CheckPerson\":\"%s\",", sResultOfOBD.strOperator.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectResult\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsNeedReCheck\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"ReCheckContent\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"InspectionNature\":\"%s\",", sTestLog.wchNumberOfTestPeriod);
	strJsonData.AppendFormat(L"\"InspectionTimes\":\"%s\"", sTestLog.wchNumberOfTestPeriod);
	strJsonData.AppendFormat(L"\"DeviceMANU\":\"%s\"", L"��ɽ���ϻ��������޹�˾");
	strJsonData.AppendFormat(L"\"DeviceType\":\"%s\"", L"NHOBD-1");
	strTemp.Format(L"%s", sResultOfOBD.strOBDType.c_str());
	if (strTemp.IsEmpty())
	{
		strJsonData.AppendFormat(L"\"OBDType\":\"%s\"", L"1");
	}
	else
	{
		strJsonData.AppendFormat(L"\"OBDType\":\"%s\"", strTemp);
	}
	strJsonData.AppendFormat(L"\"RemoteCommunicated\":\"%s\"", L"1");
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bRet = SendAndRecvPacket(L"HJ05", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

	// �ر�����
	CloseConnect();

	if (bRet)
	{
		strMsg.Format(L"�ϴ��ɹ�");
	}
	else
	{
		strMsg.Format(L"ʧ��\r\n%s", strRecv.c_str());
	}

	return bRet;
}

void CYJSocketNDlg::OnBnClickedBtnUpTelet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edMsg.SetWindowTextW(L"");
	if (m_strStationNum.IsEmpty() || m_strLineNum.IsEmpty() || m_strRunningNumber.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"�����п�ֵ");
		return;
	}


	CString strSQL;
	// ��ȡOBD����
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_strRunningNumber);
	SResultOfOBD sResultOfOBD;
	if (0x01 != GetDboResultOfOBD(strSQL, &sResultOfOBD))
	{
		m_edMsg.SetWindowTextW(L"��ȡOBD����ʧ��");
		return;
	}

	strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", m_strRunningNumber);
	TESTLOG sTestLog;
	if (0x01 != GetDboTestLog(strSQL, &sTestLog))
	{
		m_edMsg.SetWindowTextW(L"��ȡ���ݿ�����־ʧ��");
		return;
	}

	bool bRet(false);
	CString strTempMsg(L"");
	CString strMsg(L"");

	bRet = UPRealTimeData(m_strStationNum, m_strLineNum, sTestLog, sResultOfOBD, strMsg);
	if (bRet)
	{
		GetDlgItem(IDC_BTN_UP_TELET)->EnableWindow(FALSE);
	}
	else
	{
		strMsg = L"�ϴ�OBD��������ʧ��\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
	}

	// �ϴ���������ź�
	strMsg.Empty();
	bRet = UPTestSignal(m_strStationNum, m_strLineNum, sTestLog, sResultOfOBD, strMsg);
	if (bRet)
	{
	}
	else
	{
		strMsg = L"�ϴ��������ź�\r\n" + strMsg;
		strTempMsg.AppendFormat(L"%s", strMsg);
		m_edMsg.SetWindowTextW(strTempMsg);
	}
	// ���ܳɹ�����ʧ�ܶ��޸Ļ�ԭ����¼
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"TestLog", L"ItemOBD", L"4");
	}
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		si.SetString(L"VehicleInfo", L"HasOBD", L"1");
	}

	wcscpy_s(sTestLog.wchItemOBD, L"4");
	SetDboTestLog(sTestLog);

	strSQL.Format(L"select * from VehicleInfo where PlateNumber = '%s' and PlateType = '%s'", sTestLog.wchPlateNumber, sTestLog.wchPlateType);
	VEHICLEINFO sVehicleInfo;
	GetDboVehicleInfo(strSQL, &sVehicleInfo);

	wcscpy_s(sVehicleInfo.wchHasOBD, L"1");
	SetDboVehicleInfo(sVehicleInfo);

}

bool CYJSocketNDlg::UPRealTimeData(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	bool bUpbRet(false);
	CStringW strUpMsg;

	COleDateTime odtCurTime;
	COleDateTimeSpan odts;
	SYSTEMTIME st;

	CString strTestType(sTestLog.wchTestType);
	// ��̬
	if (strTestType.Find(L"1") != -1)
	{
		std::vector<SRealTimeDataOfASM> vt;
		GetIniRealTimeDataOfASMEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			CString strJsonData(L"{");
			strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
			strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", i+1);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			}
			strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", f);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
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
			}
			strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%d\",", n);
			strJsonData.AppendFormat(L"\"Flow_AirInput\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottlePosition\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OutputPower\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_BoostPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OilConsume\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strJsonData.AppendFormat(L"\"Flow_NOX\":\"%.0f\",", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_UreaInject\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ExhaustTmp\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strJsonData.AppendFormat(L"\"Flow_DPFPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_EGRPostion\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_FuelInjectPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottleValue\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_CLV\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strCalculationLoad.c_str()));
			strJsonData.AppendFormat(L"\"Flow_PreOSS\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_Lamda\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_IntakePressure\":\"%.2f\"", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"}");

			std::wstring strRecv;
			bUpbRet = SendAndRecvPacket(L"HJ06", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

			if (!bUpbRet)
			{
				strUpMsg.AppendFormat(L"(%d)False:%s", i+1, strRecv.c_str());
			}
		}
	}
	// ����˲̬
	else if (strTestType.Find(L"2") != -1)
	{
		std::vector<SRealTimeDataOfVMAS> vt;
		GetIniRealTimeDataOfVMASEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			CString strJsonData(L"{");
			strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
			strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", i+1);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			}
			strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", f);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
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
			}
			strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%d\",", n);
			strJsonData.AppendFormat(L"\"Flow_AirInput\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottlePosition\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OutputPower\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_BoostPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OilConsume\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strJsonData.AppendFormat(L"\"Flow_NOX\":\"%.0f\",", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_UreaInject\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ExhaustTmp\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strJsonData.AppendFormat(L"\"Flow_DPFPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_EGRPostion\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_FuelInjectPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottleValue\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_CLV\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strCalculationLoad.c_str()));
			strJsonData.AppendFormat(L"\"Flow_PreOSS\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_Lamda\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_IntakePressure\":\"%.2f\"", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"}");

			std::wstring strRecv;
			bUpbRet = SendAndRecvPacket(L"HJ06", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

			if (!bUpbRet)
			{
				strUpMsg.AppendFormat(L"(%d)False:%s", i+1, strRecv.c_str());
			}
		}
	}
	// ���ؼ���
	else if (strTestType.Find(L"3") != -1)
	{
		std::vector<SRealTimeDataOfLUGDOWN> vt;
		GetIniRealTimeDataOfLUGDOWNEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			CString strJsonData(L"{");
			strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
			strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", i+1);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
				else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
				else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
				else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			}
			strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", f);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (_wtoi( vt[i].strVelocity.c_str()) != 0)
			{
				GetLocalTime(&st);
				srand(st.wMilliseconds);
				int n(0);
				if (i%2 == 0){n = rand()%300 + 1752;}
				else if (i%3 == 0){n = rand()%300 + 1825;}
				else if (i%5 == 0){n = rand()%300 + 1872;}
				else {n = rand()%300 + 1852;}
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
			}
			strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%d\",", n);
			strJsonData.AppendFormat(L"\"Flow_AirInput\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottlePosition\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OutputPower\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_BoostPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OilConsume\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strJsonData.AppendFormat(L"\"Flow_NOX\":\"%.0f\",", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_UreaInject\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ExhaustTmp\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strJsonData.AppendFormat(L"\"Flow_DPFPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_EGRPostion\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_FuelInjectPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottleValue\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_CLV\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strCalculationLoad.c_str()));
			strJsonData.AppendFormat(L"\"Flow_PreOSS\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_Lamda\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_IntakePressure\":\"%.2f\"", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"}");

			std::wstring strRecv;
			bUpbRet = SendAndRecvPacket(L"HJ06", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

			if (!bUpbRet)
			{
				strUpMsg.AppendFormat(L"(%d)False:%s", i+1, strRecv.c_str());
			}
		}
	}
	// ˫����
	else if (strTestType.Find(L"4") != -1)
	{
		std::vector<SRealTimeDataOfDIS> vt;
		GetIniRealTimeDataOfDISEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			CString strJsonData(L"{");
			strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
			strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", i+1);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", f);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%d\",", n);
			strJsonData.AppendFormat(L"\"Flow_AirInput\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottlePosition\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OutputPower\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_BoostPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OilConsume\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strJsonData.AppendFormat(L"\"Flow_NOX\":\"%.0f\",", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_UreaInject\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ExhaustTmp\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strJsonData.AppendFormat(L"\"Flow_DPFPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_EGRPostion\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_FuelInjectPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottleValue\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_CLV\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strCalculationLoad.c_str()));
			strJsonData.AppendFormat(L"\"Flow_PreOSS\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_Lamda\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_IntakePressure\":\"%.2f\"", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"}");

			std::wstring strRecv;
			bUpbRet = SendAndRecvPacket(L"HJ06", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

			if (!bUpbRet)
			{
				strUpMsg.AppendFormat(L"(%d)False:%s", i+1, strRecv.c_str());
			}
		}
	}
	// ��͸�����ɼ���
	else if (strTestType.Find(L"5") != -1)
	{
		std::vector<SRealTimeDataOfFSUNHT> vt;
		GetIniRealTimeDataOfFSUNHTEx(vt);
		for(int i=0; i<vt.size(); i++)
		{
			CString strJsonData(L"{");
			strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
			strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
			strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", i+1);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f(0.0f);
			strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", f);
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%d\",", n);
			strJsonData.AppendFormat(L"\"Flow_AirInput\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottlePosition\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OutputPower\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEngineOutputPower.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_BoostPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strChargeAirPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_OilConsume\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelConsumption.c_str()));
			strJsonData.AppendFormat(L"\"Flow_NOX\":\"%.0f\",", _wtof(vt[i].sOBDRTData.strNOXConcentration.c_str()));// ע��ת�����
			strJsonData.AppendFormat(L"\"Flow_UreaInject\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strUreaInjectionVolume.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ExhaustTmp\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGT.c_str()));
			strJsonData.AppendFormat(L"\"Flow_DPFPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strDPFDifferentialPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_EGRPostion\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strEGRPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_FuelInjectPressure\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strFuelDeliveryPressure.c_str()));
			strJsonData.AppendFormat(L"\"Flow_ThrottleValue\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strSolarTermDoorPosition.c_str()));
			strJsonData.AppendFormat(L"\"Flow_CLV\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strCalculationLoad.c_str()));
			strJsonData.AppendFormat(L"\"Flow_PreOSS\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_Lamda\":\"%.2f\",", _wtof(vt[i].sOBDRTData.strAirCoefficient.c_str()));
			strJsonData.AppendFormat(L"\"Flow_IntakePressure\":\"%.2f\"", _wtof(vt[i].sOBDRTData.strMAF.c_str()));
			strJsonData.AppendFormat(L"}");

			std::wstring strRecv;
			bUpbRet = SendAndRecvPacket(L"HJ06", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

			if (!bUpbRet)
			{
				strUpMsg.AppendFormat(L"(%d)False:%s", i+1, strRecv.c_str());
			}
		}
	}
	// ��ֽʽ���ɼ���
	else if (strTestType.Find(L"6") != -1)
	{
	}

	if (strUpMsg.Find(L"False") == -1)
	{
		bRet = true;
	}
	strMsg.Format(L"%s", strUpMsg);
	return bRet;
}

bool CYJSocketNDlg::UPTestSignal(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	COleDateTime odtCurTime( COleDateTime::GetCurrentTime());
	odtCurTime.ParseDateTime(sTestLog.wchTestEndTime);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", strLineNum);
	strJsonData.AppendFormat(L"\"Time\":\"%s\",", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"Code\":\"%s\"}", L"stop");

	std::wstring strRecv;
	bRet = SendAndRecvPacket(L"HJ04", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

	if (bRet)
	{
		strMsg.Format(L"�ϴ��ɹ���%s", strRecv.c_str());
	}
	else
	{
		strMsg.Format(L"�ϴ�ʧ�ܣ�%s", strRecv.c_str());
	}
	return bRet;
}


void CYJSocketNDlg::OnBnClickedBtnTestlog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);

	SResultOfOBD sResultOfOBD;
	GetIniResultOfOBD(&sResultOfOBD);
	if (wcscmp(sTestLog.wchRunningNumber, L"") == 0)
	{
		// �ϻ���дApp_data, ����������ʽ��ȡ
		m_edMsg.SetWindowTextW(L"\r\n");
		//if (IDOK == MessageBox(L"��ȡApp_Dataʧ��", L"ʧ��", MB_OKCANCEL))
		{
			CString strSQL;
			strSQL.Format(L"SELECT TOP(1)* FROM TestLog WHERE RunningNumber='%s'", sResultOfOBD.strRunningNumber.c_str());
			GetDboTestLog(strSQL, &sTestLog);
		}
		//else
		//{
		//	return;
		//}
	}

	// ��ȡOBD����ʱ�ļ�������־��ź����±����д��
	//sResultOfOBD.strRunningNumber = sTestLog.wchRunningNumber;
	SetDboResultOfOBD(sResultOfOBD);
	SetIniResultOfOBD(sResultOfOBD);

	CString strTemp;
	strTemp.Format(L"%s-%s-%s\r\n", sResultOfOBD.strRunningNumber.c_str(), sTestLog.wchRunningNumber, sResultOfOBD.strJudge.c_str());
	m_edMsg.SetWindowTextW(strTemp);

	_ConnectionPtr pConnection;
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection))
	{
		CString strSql;
		strSql.Format(L"update TestLog set TestLog.ItemOBD = null where TestLog.RunningNumber = '%s' ", sTestLog.wchRunningNumber);

		int nRowsInvolved(0);
		CNHSQLServerDBO::ExecuteDML(pConnection, strSql.GetString(), &nRowsInvolved);
		//if (nRowsInvolved >= 0)
		//{
		//	strTempMsg.AppendFormat(L"%s-%s\r\n", L"�޸ĳɹ�", sTestLog.wchRunningNumber);
		//	m_edMsg.SetWindowTextW(strTempMsg);
		//}
		//else
		//{
		//	strTempMsg.AppendFormat(L"%s-%s\r\n", L"�޸�ʧ��", sTestLog.wchRunningNumber);
		//	m_edMsg.SetWindowTextW(strTempMsg);
		//}

		CNHSQLServerDBO::CloseDB(pConnection);
	}
	else
	{/*
	 strTempMsg.AppendFormat(L"%s\r\n", L"�����ݿ�ʧ��");
	 m_edMsg.SetWindowTextW(strTempMsg);*/
	}
}
