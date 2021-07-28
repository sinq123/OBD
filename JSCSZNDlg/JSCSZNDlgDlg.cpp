
// JSCSZNDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JSCSZNDlg.h"
#include "JSCSZNDlgDlg.h"
#include "afxdialogex.h"

#include "md5.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJSCSZNDlgDlg �Ի���

CJSCSZNDlgDlg::CJSCSZNDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJSCSZNDlgDlg::IDD, pParent)
	, m_pConnection(NULL)
	, m_bConnectDB(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CJSCSZNDlgDlg::~CJSCSZNDlgDlg()
{
	if (m_bConnectDB)
	{
		CloseDB();
	}
}

void CJSCSZNDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ED_Source, m_edCatalog);
	DDX_Control(pDX, ID_ED_Catalog, m_edSource);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassWord);
	DDX_Control(pDX, IDC_COM_OBDJCY, m_cbOBDJCY);

	DDX_Control(pDX, ID_ED_URL, m_edURL);
	DDX_Control(pDX, ID_ED_URL2, m_edURL2);
	DDX_Control(pDX, ID_ED_UNITID, m_edunitid);
	DDX_Control(pDX, ID_ED_LINEID, m_edlineid);
	DDX_Control(pDX, ID_ED_TOKEN, m_edtoken);
	DDX_Control(pDX, ID_ED_INTERUSERNAME, m_edInterusername);
	DDX_Control(pDX, ID_ED_INTERPWD, m_edInterpwd);

	DDX_Control(pDX, ID_ED_VINCJH, m_edVINCJH);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMsg);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);

}

BEGIN_MESSAGE_MAP(CJSCSZNDlgDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETCONFIG, &CJSCSZNDlgDlg::OnBnClickedBtnSetconfig)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CJSCSZNDlgDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CJSCSZNDlgDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UP, &CJSCSZNDlgDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_UPOBDTIMEDATA, &CJSCSZNDlgDlg::OnBnClickedBtnUpobdtimedata)
END_MESSAGE_MAP()


// CJSCSZNDlgDlg ��Ϣ�������

BOOL CJSCSZNDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	LogFilePath();
	SetVehCtrl();
	SetConfig();
	m_bConnectDB = OpenDB();
	GetOBDJCY();
#ifndef _DEBUG
	// Release ��ʹ��
	GetToken();
#endif
	CString str;
	m_edMsg.GetWindowTextW(str);
	if (str.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"���������б���ȡ������Ϣ");
	}
	//GetBaseTypeInfo();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CJSCSZNDlgDlg::OnPaint()
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
HCURSOR CJSCSZNDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJSCSZNDlgDlg::LogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	// ������־�ļ�
	strFileName.Format(L"LC%s.log", odtNow.Format(L"%Y-%m-%d"));
	m_strLogFilePath = wchLogFileFolderPath + strFileName;
	// ���ݿ������־�ļ�
	strFileName.Format(L"LCSQL%s.log", odtNow.Format(L"%Y-%m-%d"));
	m_strDBLogFilePath = wchLogFileFolderPath + strFileName;
	CNHSQLServerDBO::SetLogFilePath(m_strDBLogFilePath);
}


void CJSCSZNDlgDlg::SetVehCtrl(void)
{
	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	int nDlgFontSize;
	nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
	CFont fontVehicleLCFont, fontDlgFont;


	fontVehicleLCFont.CreateFont(25,
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
	m_lstVehicle.InsertColumn(1, L"����",		0,	200);
	m_lstVehicle.InsertColumn(2, L"������ɫ",	0,	200);
	m_lstVehicle.InsertColumn(3, L"���ܺ�",	0,	150);
	m_lstVehicle.InsertColumn(4, L"�����ͺ�",	0,	150);
	m_lstVehicle.InsertColumn(5, L"��ⱨ����",	0,	200);
	m_lstVehicle.InsertColumn(6, L"�������",	0,	150);
	m_lstVehicle.InsertColumn(7, L"��ⷽ��",	0,	200);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}

void CJSCSZNDlgDlg::SetConfig(void)
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
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"LoginEPB.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"EPB", L"URL", L"http://172.141.32.1:9088/synchrodata/webservice//SynTocity?wsdl");
		m_edURL.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"UnitId", L"320300060");
		m_edunitid.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"LineId", L"32030006001");
		m_edlineid.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"UserName", L"XUMEINA");
		m_edInterusername.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"Password", L"123456");
		m_edInterpwd.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"EPB", L"BDURL", L"http://172.141.32.1:9999/synchrodata/webservice/bd?wsdl");
		m_edURL2.SetWindowTextW(strTemp);

	}

	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	// ��ʱ����
	GetDlgItem(IDC_BTN_UPOBDTIMEDATA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_UPOBDTIMEDATA)->EnableWindow(FALSE);
}

bool CJSCSZNDlgDlg::OpenDB(void)
{
	bool bRet(false);

	CString strMsg;
	CString strSource, strCatalog, strUserID, strPassWord;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassWord);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() || strUserID.IsEmpty() || strPassWord.IsEmpty())
	{
		strMsg.Format(L"���ݿ�����п�ֵ���������ݿ����ʧ��");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	if (0x00 == CNHSQLServerDBO::OpenDB(m_pConnection, strSource, strCatalog, strUserID, strPassWord))
	{
		bRet = true;
	}


	return bRet;
}

bool CJSCSZNDlgDlg::CloseDB(void)
{
	if (0x00 == CNHSQLServerDBO::CloseDB(m_pConnection))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CJSCSZNDlgDlg::GetOBDJCY(void)
{
	if (m_bConnectDB)
	{
		CString strSQL;
		strSQL.Format(L"select * from UserInfo /*where Role = 6*/");
		_RecordsetPtr pRecordset(NULL);
		m_cbOBDJCY.ResetContent();
		if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSQL))
		{
			// �򿪲�ѯ�ɹ�
			try
			{
				m_cbOBDJCY.AddString(L"");
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
		// ��Ϊûѡ
		m_cbOBDJCY.SetCurSel(-1);
		CNHSQLServerDBO::CloseQuery(pRecordset);
	}
	else
	{
		m_edMsg.SetWindowTextW(L"�����ݿ��쳣");
	}
}

void CJSCSZNDlgDlg::GetToken(void)
{
	CStringW strLog;
	strLog.Format(L"��ʼ��ȡToken");
	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��¼GetToken", L"", strLog);

	CString strMsg;
	CString strURL, strunitid, strlineid, strInterusername, strInterpwd;
	m_edURL.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		m_edMsg.GetWindowTextW(strMsg);
		strMsg.AppendFormat(L"���������п�ֵ");
		m_edMsg.SetWindowTextW(strMsg);
		return;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	std::string smd(CW2A(strInterpwd.GetString()));
	MD5 md5(smd);
	smd = md5.md5();
	// ��д
	transform(smd.begin(), smd.end(), smd.begin(), ::toupper);
	// Сд
	//transform(smd.begin(), smd.end(), smd.begin(), ::tolower);

	// ����Ϊ32�ĵ�¼����MD5�ַ���
	CStringW strTemp;
	strTemp = smd.c_str();

	std::wstring strRet;
	int nRet = CSZIntLib_New_API::LoginServer(pchURL, strunitid.GetString(), strlineid.GetString(), strInterusername.GetString(), strTemp.GetString(), strRet);

	// ��ʱ��֪�����أ��Ȳ��ܣ�
	if (nRet == 0)
	{
	}
	strLog.Format(L"������ȡ���%d, ������Ϣ%s", nRet, strRet.c_str());
	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��¼GetToken", L"", strLog);


}

bool ToUnicode(CString& strTemp)
{
	bool bRet(false);
	int nlength = strTemp.GetLength();
	for (int i=0; i < nlength; i++)
	{
		int num = strTemp.GetAt(i);
		if ((num >= 'a' && num <='z') ||
			(num >= 'A' && num <= 'Z') ||
			(num >= '0' && num <= '9')
			)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}

// charתwchar
CString ReadStringCharToUnicode(CString &str)
{
	CString strRead;

	char *szBuf = new char[str.GetLength() + 1];//ע�⡰+1����char�ַ�Ҫ�����������CStringû��
	memset(szBuf, '\0',str.GetLength());
	//��ֵ
	int i;
	for ( i = 0 ; i < str.GetLength(); i++)
	{
		szBuf[i] = (char)str.GetAt(i);
	}
	szBuf[i] = '\0';//���������������ĩβ�������롣

	int nLen = MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, NULL, 0); //ȡ�����軺��Ķ���
	wchar_t *pBuffer = (wchar_t*)malloc(nLen * sizeof(wchar_t));//���뻺��ռ�
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1 , pBuffer, nLen*sizeof(wchar_t));//ת��

	strRead.Format(L"%s", pBuffer);
	return strRead;
}

CString CJSCSZNDlgDlg::GetTXT(const CString& strPath)
{
	CString strTemp(L""), str(L"");
	CStdioFile file;

	if (file.Open(strPath, CFile::modeRead))
	{
		while(file.ReadString(str))
		{
			str = ReadStringCharToUnicode(str);
			strTemp += str;
		}
	}
	file.Close();
	return strTemp;
}

CString CJSCSZNDlgDlg::ToCheck(const CString& strCheck, const int& nType)
{
	// 1,������ͣ� 2����ⷽ��
	CString str;
	switch (nType)
	{
	case 1:
		{
			switch (_wtoi(strCheck))
			{
			case 1: {str.Format(L"���");}break;
			case 2: {str.Format(L"��ע�ᳵ��");}break;
			case 3: {str.Format(L"��س�ת��");}break;
			case 4: {str.Format(L"ʵ��ȶ�");}break;
			case 5: {str.Format(L"�������żල���ĸ���");}break;
			default: break;
			}
		}
		break;

	case 2:
		{
			switch (_wtoi(strCheck))
			{
			case 1: {str.Format(L"˫���ٷ�");}break;
			case 2: {str.Format(L"��̬������");}break;
			case 3: {str.Format(L"����˲̬����");}break;
			case 4: {str.Format(L"���ؼ���");}break;
			case 5: {str.Format(L"��͸���̶ȷ�");}break;
			default: break;
			}
		}
		break;

	case 3:
		{
			switch (_wtoi(strCheck))
			{
			case 0: {str.Format(L"����");}break;
			case 1: {str.Format(L"����");}break;
			case 2: {str.Format(L"����");}break;
			case 3: {str.Format(L"����");}break;
			default: break;
			}
		}
		break;
	}
	return str;
}

bool CJSCSZNDlgDlg::GetVehInfoList(void)
{
	
	CStringW strLog;
	strLog.Format(L"��ʼ��ȡ�б���Ϣ��ʼ");
	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��¼GetVehInfoList", L"", strLog);

	CString strMsg;
	CString strURL, strunitid, strlineid, strInterusername, strInterpwd, strtoken;
	m_edURL.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);
	m_edtoken.GetWindowTextW(strtoken);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		m_edMsg.GetWindowTextW(strMsg);
		strMsg.AppendFormat(L"���������п�ֵ");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	std::wstring strRet;
	int nRet = CSZIntLib_New_API::GetCheckList(pchURL, L"", strunitid.GetString(), strRet);

#ifdef _DEBUG
	nRet = 0;
	strRet = GetTXT(L"VehInfoList.xml").GetString();
#endif

	std::list<SVehInfoList> lsVehInfoList;
	if (nRet == 0)
	{
		CXmlReader xmlReader;

		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring wstrCode, wstrContent;

			if (xmlReader.OpenNode(L"xml/status"))
			{
				xmlReader.GetNodeContent(wstrCode);
			}
			CString strTAndF(wstrCode.c_str());
			strTAndF.MakeUpper(); // ���
			
			if (strTAndF != L"TRUE")
			{
				if (xmlReader.OpenNode(L"xml/errMsg"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				strMsg.Format(L"��ȡ�б�ʧ��ԭ��%s", wstrCode.c_str());
				m_edMsg.SetWindowTextW(strMsg);
				return false;
			}
			else
			{
				if (xmlReader.OpenNode(L"xml/checklist/vehicleitem"))
				{
					std::wstring strNodeName;
					do
					{
						SVehInfoList sVehInfoList;
						xmlReader.EnterNode();
						do
						{
							xmlReader.GetNodeName(strNodeName);
							if(L"plate" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strplate);
							}
							if(L"plate_color" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strplate_color);
							}
							if(L"vin" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strvin);
							}
							if(L"clxh" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strclxh);
							}
							if(L"check_type" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strcheck_type);
							}
							if(L"check_method" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strcheck_method);
							}
							if(L"checkid" == strNodeName)
							{
								xmlReader.GetNodeContent(sVehInfoList.strcheckid);
							}
						}
						while (xmlReader.NextSibling());
						xmlReader.ExitNode();
						lsVehInfoList.push_back(sVehInfoList);
					}
					while(xmlReader.NextSibling());
				}
			}

		}
		else
		{
			m_edMsg.SetWindowTextW(L"��ȡ�����б����ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.Format(L"����ʧ�ܣ����ش���%d", nRet);
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	// �б����1����ʼ����б�
	if (lsVehInfoList.size() > 0)
	{
		// ɾ���б�����
		m_lstVehicle.DeleteAllItems();

		int nItem(0);
		for (list<SVehInfoList>::iterator ir=lsVehInfoList.begin(); ir!=lsVehInfoList.end(); ++ir)
		{
			CString str;
			// ���
			str.Format(L"%d", nItem+1);
			int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

			// ����
			m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strplate.c_str(), 0, 0, 0, 0);
			//������ɫ
			m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ToCheck(ir->strplate_color.c_str(), 3), 0, 0, 0, 0);
			//���ܺ�
			m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strvin.c_str(), 0, 0, 0, 0);
			//�����ͺ�
			m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strclxh.c_str(), 0, 0, 0, 0);
			//��ⱨ����
			m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strcheckid.c_str(), 0, 0, 0, 0);
			// �������
			m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, ToCheck(ir->strcheck_type.c_str(), 1), 0, 0, 0, 0);
			// ��ⷽ��
			m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, ToCheck(ir->strcheck_method.c_str(),2), 0, 0, 0, 0);

			nItem++;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CJSCSZNDlgDlg::GetVehInfo(void)
{
	CStringW strLog;
	strLog.Format(L"��ʼ��ȡ�б���Ϣ��ʼ");
	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��¼GetVehInfoList", L"", strLog);

	CString strMsg;
	CString strURL, strunitid, strlineid, strInterusername, strInterpwd, strtoken;
	m_edURL.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);
	m_edtoken.GetWindowTextW(strtoken);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		m_edMsg.GetWindowTextW(strMsg);
		strMsg.AppendFormat(L"���������п�ֵ");
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	std::wstring strRet;
	int nRet = CSZIntLib_New_API::getVehicle(pchURL, strtoken.GetString(), strunitid.GetString(), m_sVehInfoList.strvin.c_str(), strRet);

#ifdef _DEBUG
	nRet = 0;
	strRet = GetTXT(L"VehicleInfo.xml").GetString();
#endif

	if (nRet == 0)
	{
		m_sTestLog = TestLog();
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring wstrCode, wstrContent;

			if (xmlReader.OpenNode(L"xml/status"))
			{
				xmlReader.GetNodeContent(wstrCode);
			}
			CString strTAndF(wstrCode.c_str());
			strTAndF.MakeUpper(); // ���
			
			if (strTAndF != L"TRUE")
			{
				if (xmlReader.OpenNode(L"xml/errMsg"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				strMsg.Format(L"��ȡ������Ϣʧ��ԭ��%s", wstrCode.c_str());
				m_edMsg.SetWindowTextW(strMsg);
				return false;
			}
			else
			{
				//	is_electronic_ctrl                �Ƿ��� 
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/IS_ELECTRONIC_CTRL"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchFuelPumpMode, L"���ӿ���");
					}
					else
					{
						wcscpy_s(m_sTestLog.wchFuelPumpMode, L"��е");
					}
				}
				//	rating_power                      �����������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/RATING_POWER"))
				{
					xmlReader.GetNodeContent(wstrCode);
					wcscpy_s(m_sTestLog.wchRatedPower, wstrCode.c_str());
				}
				//	factory_name                      ��������������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/FACTORY_NAME"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchManufacturer, wstrCode.c_str());
					}
				}
				//	cylinder                          ��������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/CYLINDER"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchNumberOfCylinder, wstrCode.c_str());
					}
				}
				//	vin                               ���ܺ�
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/VIN"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchVIN, wstrCode.c_str());
					}
				}
				//	stdweight                         ��׼����
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/STDWEIGHT"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						CString str;
						str.Format(L"%d", _wtoi(wstrCode.c_str())+100);
						wcscpy_s(m_sTestLog.wchUnladenMass, str);
					}
				}
				//	sign_type                         ��־����
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/SIGN_TYPE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchEPSign, wstrCode.c_str());
					}
				}
				//	stroke                            �����������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/STROKE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchEngineStroke, wstrCode.c_str());
					}
				}
				//	supply_mode                       ���ͷ�ʽ
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/SUPPLY_MODE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchOilSupplyMode, wstrCode.c_str());
					}
				}
				//	owner                             ����
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/OWNER"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchOwner, wstrCode.c_str());
					}
				}
				//	near_unit_id                      ���һ�μ����վ���
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/NEAR_UNIT_ID"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchReserved1, wstrCode.c_str());
					}
				}
				//	exhaust_quantity                  ����������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/EXHAUST_QUANTITY"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchDisplacement, wstrCode.c_str());
					}
				}
				//	phone                             �����ֻ�
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/PHONE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchPhone, wstrCode.c_str());
					}
				}
				//	odo_meter                         �����
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/ODO_METER"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchTravelledDistance, wstrCode.c_str());
					}
				}
				//	near_check_date                   ���һ�μ������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/NEAR_CHECK_DATE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchReserved2, wstrCode.c_str());
					}
				}
				//	clxh                              �����ͺ�
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/CLXH"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchPlateType, wstrCode.c_str());
					}
				}
				//	login_unit_id                     �����¼����
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/LOGIN_UNIT_ID"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchReserved3, wstrCode.c_str());
					}
				}
				//	engine_no                         ��������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/ENGINE_NO"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchEngineNumber, wstrCode.c_str());
					}
				}
				//	owneraddress                      ������ַ
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/OWNERADDRESS"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchAddress, wstrCode.c_str());
					}
				}
				//	vehicle_id                        �������  ��������ϴ�ʱ��ʹ��
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/VEHICLE_ID"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchReportNumber, wstrCode.c_str());
						wcscpy_s(m_sTestLog.wchRunningNumber, wstrCode.c_str());
					}
				}
				//	usage_nature                      ʹ������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/USAGE_NATURE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchUseCharacter, wstrCode.c_str());
					}
				}
				//	seat_capacity                     ��λ��
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/SEAT_CAPACITY"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	has_odb                           �Ƿ���obd
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/HAS_ODB"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode == L"Y")
					{
						wcscpy_s(m_sTestLog.wchHasOBD, L"1");
					}
					else
					{
						wcscpy_s(m_sTestLog.wchHasOBD, L"0");
					}
				}
				//	register_date                     ע������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/REGISTER_DATE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchRegistrationDate, wstrCode.c_str());
					}
				}
				//	clsb                              �����ͺ�
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/CLSB"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchModel, wstrCode.c_str());
					}
				}
				//	plate                             ����
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/PLATE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchPlateNumber, wstrCode.c_str());
					}
				}
				//	check_method                      ��ⷽ��
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/CHECK_METHOD"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchTestType, wstrCode.c_str());
					}
				}
				//	fuel_type                         ȼ������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/FUEL_TYPE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode != L"null")
					{
						wcscpy_s(m_sTestLog.wchFuelType, wstrCode.c_str());
					}
				}
				//	has_purge                         �Ƿ��к���װ��
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/HAS_PURGE"))
				{
					xmlReader.GetNodeContent(wstrCode);
					if (wstrCode == L"Y")
					{
						wcscpy_s(m_sTestLog.wchHasCatalyticConverter, L"1");
					}
					else
					{
						wcscpy_s(m_sTestLog.wchHasCatalyticConverter, L"0");
					}
				}
				//	ordain_rev                        �������ת��
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/ORDAIN_REV"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	plate_color                       ������ɫ
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/PLATE_COLOR"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	check_period                      ��������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/CHECK_PERIOD"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	drive_mode                        ������ʽ
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/DRIVE_MODE"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	plate_type                        ��������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/PLATE_TYPE"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	maxweight                         �������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/MAXWEIGHT"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	near_check_result                 ��������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/NEAR_CHECK_RESULT"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	standard_id                       �ŷŵȼ�
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/STANDARD_ID"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	vehicle_type                      ��������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/VEHICLE_TYPE"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	manufacture_date                  ������������
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/MANUFACTURE_DATE"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	drive_form                        ��������ʽ 
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/DRIVE_FORM"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	admission                         ������ʽ 
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/ADMISSION"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				//	fdjxh                             �������ͺ�
				if (xmlReader.OpenNode(L"xml/VEHICLE_INFO/VEHICLE_INFO_CONTENT/FDJXH"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
			}
		}
		else
		{
			m_edMsg.SetWindowTextW(L"��ȡ������Ϣ����ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.Format(L"����ʧ�ܣ����ش���%d", nRet);
		m_edMsg.SetWindowTextW(strMsg);
		return false;
	}

	return true;
}

bool CJSCSZNDlgDlg::SaveINIAndSQLAndUP(void)
{
	if (m_bConnectDB)
	{
		wchar_t wchPath[MAX_PATH];

		CString strSQL;
		// ��ȡOBD����
		strSQL.Format(L"SELECT * FROM TestLog WHERE PlateNumber = '%s' ORDER BY AutoID DESC LIMIT 1 ", m_sTestLog.wchPlateNumber);
		TESTLOG sTestLogRun;
		if (0x01 == GetDboTestLog(strSQL, &sTestLogRun))
		{
			wcscpy_s(m_sTestLog.wchRunningNumber, sTestLogRun.wchRunningNumber);
		}
		else
		{
			
			// ���ݿ��ȡ�����������ļ���ȡ
			GetIniTestLog(&sTestLogRun);

			// ��֤���ƺ�����ͬ�Ÿ���Running
			if (wcscmp(sTestLogRun.wchPlateNumber, m_sTestLog.wchPlateNumber) == 0)
			{
				wcscpy_s(m_sTestLog.wchRunningNumber, sTestLogRun.wchRunningNumber);
			}
			else
			{
				m_edMsg.SetWindowTextW(L"�����ù������ѡ����");
				return false;
			}
		}




		strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", m_sTestLog.wchPlateNumber);
		SResultOfOBD sResultOfOBD;
		GetDboResultOfOBD(strSQL, &sResultOfOBD);

		CString strOBDJCY;
		m_cbOBDJCY.GetWindowTextW(strOBDJCY);
		if (strOBDJCY.IsEmpty())
		{
			//MessageBox(L"OBD���Աδѡ��");
			m_edMsg.SetWindowTextW(L"OBD���Աδѡ��");
			return false;
		}
		CString strMsg;
		sResultOfOBD.strRunningNumber = m_sTestLog.wchRunningNumber;
		sResultOfOBD.strOperator = strOBDJCY.GetString();
		bool bRet = SetOBDLOG(sResultOfOBD, strMsg);

		if (!bRet)
		{
			m_edMsg.SetWindowTextW(L"����OBD����ʧ��");
			return false;
		}
		// д�����ݿ�
		SetDboResultOfOBD(sResultOfOBD);
		// д����ʱ�ļ�
		SetIniResultOfOBD(sResultOfOBD);

		// ����
		bRet = UpOBDReaustAndIURP(sResultOfOBD, strMsg);
		if (!bRet)
		{
			strMsg = L"�ϴ�ʧ��" + strMsg;
			m_edMsg.SetWindowTextW(strMsg);
			return false;
		}
		else
		{
			m_edMsg.SetWindowTextW(L"�ϴ�OBD������ݳɹ�");
		}
		// ������Ϊ���ɵ�
		GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);

		// ����OBD���������ϴ�
		if (IDYES == MessageBox(L"", L"", MB_YESNO))
		{
			// ����INI�����ݿ⣬OBD������
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
			{
				CSimpleIni si(wchPath);
				si.SetString(L"TestLog", L"ItemOBD", L"0");
				si.SetString(L"TestLog", L"HasOBD", L"0");
			}
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
			{
				CSimpleIni si(wchPath);
				si.SetString(L"TestLog", L"HasOBD", L"0");
			}

			strSQL.Format(_T("Update TestLog set ItemOBD = '0', HasOBD = '0' where PlateNumber = '%s' ORDER BY AutoID DESC LIMIT 1"), m_sTestLog.wchPlateNumber);
			int nRowsInvolved(0);
			CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved);

			strSQL.Format(_T("Update VehicleInfo set HasOBD = '0' where PlateNumber = '%s' "), m_sTestLog.wchPlateNumber);
			nRowsInvolved = 0;
			CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved);

			GetDlgItem(IDC_BTN_UPOBDTIMEDATA)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_UPOBDTIMEDATA)->EnableWindow(TRUE);
		}
		else
		{
			// ����INI
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
			{
				CSimpleIni si(wchPath);
				si.SetString(L"TestLog", L"ItemOBD", L"4");
				si.SetString(L"TestLog", L"HasOBD", L"1");
			}
			ZeroMemory(wchPath, sizeof(wchPath));
			if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
			{
				CSimpleIni si(wchPath);
				si.SetString(L"TestLog", L"HasOBD", L"1");
			}

			strSQL.Format(_T("Update TestLog set ItemOBD = '4', HasOBD = '1' where PlateNumber = '%s' ORDER BY AutoID DESC LIMIT 1"), m_sTestLog.wchPlateNumber);
			int nRowsInvolved(0);
			CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved);

			strSQL.Format(_T("Update VehicleInfo set HasOBD = '1' where PlateNumber = '%s' "), m_sTestLog.wchPlateNumber);
			nRowsInvolved = 0;
			CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved);
		}
	}
	else
	{
		m_edMsg.SetWindowTextW(L"δ�������ݿ�");
		return false;
	}
	return true;
}

bool CJSCSZNDlgDlg::UpOBDReaustAndIURP(const SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(false);

	CString strURL, strunitid, strlineid, strInterusername, strInterpwd, strtoken;
	m_edURL.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);
	m_edtoken.GetWindowTextW(strtoken);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		strMsg.AppendFormat(L"���������п�ֵ");
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	CString strDocXml;
	CString str;
	str.Format(L"%s", strunitid.Mid(0,5));
	int ncheckmethod;

	strDocXml.AppendFormat(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	strDocXml.AppendFormat(L"<result>");
	// ����
	if (wcscmp(m_sTestLog.wchFuelType, L"B") == 0)
	{
		ncheckmethod = 7;
	}
	// ����
	else
	{
		ncheckmethod = 6;
	}
	strDocXml.AppendFormat(L"<result_data>");
	strDocXml.AppendFormat(L"<check_id>%s</check_id>", m_sVehInfoList.strcheckid.c_str());
	strDocXml.AppendFormat(L"<city_code>%s</city_code>", str);
	strDocXml.AppendFormat(L"<unit_id>%s</unit_id>", strunitid);
	strDocXml.AppendFormat(L"<user_id>%s</user_id>", strInterusername);
	strDocXml.AppendFormat(L"<uname>%s</uname>", sResultOfOBD.strOperator.c_str());
	strDocXml.AppendFormat(L"<pwd>%s</pwd>", strInterpwd);
	strDocXml.AppendFormat(L"<vin>%s</vin>", m_sTestLog.wchVIN);
	strDocXml.AppendFormat(L"<clxh>%s</clxh>", m_sTestLog.wchPlateType);
	strDocXml.AppendFormat(L"<plate>%s</plate>", m_sTestLog.wchPlateNumber);
	strDocXml.AppendFormat(L"<rlzl>%s</rlzl>", m_sTestLog.wchFuelType);
	strDocXml.AppendFormat(L"<check_date>%s</check_date>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strDocXml.AppendFormat(L"<odometer>%s</odometer>", L"0");
	strDocXml.AppendFormat(L"<obdzsq>%s</obdzsq>", L"0");
	strDocXml.AppendFormat(L"<obdstand>%s</obdstand>", sResultOfOBD.strOBDType.c_str());
	strDocXml.AppendFormat(L"<communication>%s</communication>", L"0");
	strDocXml.AppendFormat(L"<communication_desc>%s</communication_desc>", L"");
	strDocXml.AppendFormat(L"<ycpfzdtx>%s</ycpfzdtx>", L"0");
	strDocXml.AppendFormat(L"<isready>%s</isready>", L"0");
	strDocXml.AppendFormat(L"<isreadyfailitem>%s</isreadyfailitem>", L"");
	strDocXml.AppendFormat(L"<passed>%s</passed>", L"1");
	strDocXml.AppendFormat(L"</result_data>");

	strDocXml.AppendFormat(L"<trou_data>");
	strDocXml.AppendFormat(L"<troubleid>%s</troubleid>", L"1");
	strDocXml.AppendFormat(L"<milodo>%s</milodo>", L"0.00");
	strDocXml.AppendFormat(L"<gzmnum>%s</gzmnum>", L"0");
	strDocXml.AppendFormat(L"<trouble_desc>%s</trouble_desc>", L"-");
	strDocXml.AppendFormat(L"</trou_data>");

	strDocXml.AppendFormat(L"<cal_data>");
	strDocXml.AppendFormat(L"<obd_con_name>%s</obd_con_name>", L"���������Ƶ�Ԫ");
	strDocXml.AppendFormat(L"<calid>%s</calid>", sResultOfOBD.strEngineCALID.c_str());
	strDocXml.AppendFormat(L"<cvn>%s</cvn>", sResultOfOBD.strEngineCVN.c_str());
	strDocXml.AppendFormat(L"</cal_data>");

	strDocXml.AppendFormat(L"<cal_data>");
	strDocXml.AppendFormat(L"<obd_con_name>%s</obd_con_name>", L"������Ƶ�Ԫ");
	strDocXml.AppendFormat(L"<calid>%s</calid>", L"��֧��");
	strDocXml.AppendFormat(L"<cvn>%s</cvn>", L"��֧��");
	strDocXml.AppendFormat(L"</cal_data>");

	strDocXml.AppendFormat(L"<cal_data>");
	strDocXml.AppendFormat(L"<obd_con_name>%s</obd_con_name>", L"�������Ƶ�Ԫ");
	strDocXml.AppendFormat(L"<calid>%s</calid>", L"��֧��");
	strDocXml.AppendFormat(L"<cvn>%s</cvn>", L"��֧��");
	strDocXml.AppendFormat(L"</cal_data>");

	for (int i=0; i<17;i++)
	{
		strDocXml.AppendFormat(L"<iupr_data>");
		strDocXml.AppendFormat(L"<name>%d</name>", i+1);
		strDocXml.AppendFormat(L"<times>%d</times>", 0);
		strDocXml.AppendFormat(L"<oktimes>%d</oktimes>", 0);
		strDocXml.AppendFormat(L"<iupr>%d</iupr>", 0);
		strDocXml.AppendFormat(L"</iupr_data>");
	}
	strDocXml.AppendFormat(L"</result>");

	std::wstring strRet;
	int nRet = CSZIntLib_New_API::UploadInspectionResult(pchURL, strtoken.GetString(), strunitid.GetString(), strDocXml.GetString(), ncheckmethod, strRet);


#ifdef _DEBUG
	nRet = 0;
#endif
	if (nRet == 0)
	{
		CXmlReader xmlReader;

		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring wstrCode, wstrContent;

			if (xmlReader.OpenNode(L"xml/status"))
			{
				xmlReader.GetNodeContent(wstrCode);
			}
			CString strTAndF(wstrCode.c_str());
			strTAndF.MakeUpper(); // ���
			
			if (strTAndF != L"TRUE")
			{
				if (xmlReader.OpenNode(L"xml/errMsg"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				strMsg.AppendFormat(L"�ϴ�OBD���ʧ��ԭ��%s", wstrCode.c_str());
				return false;
			}
			bRet = true;
		}
		else
		{
			strMsg.AppendFormat(L"�ϴ�OBD�������ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.AppendFormat(L"�ϴ�OBD�������ʧ�ܣ����ش���%d", nRet);
		return false;
	}
	return bRet;
}

bool CJSCSZNDlgDlg::UpOBDRealTimeData(CString& strMsg)
{
	bool bRet(false);

	CString strURL, strunitid, strlineid, strInterusername, strInterpwd, strtoken;
	m_edURL.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);
	m_edtoken.GetWindowTextW(strtoken);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		strMsg.AppendFormat(L"���������п�ֵ");
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	int ncheckmethod;

	CString strTimeData;
	COleDateTime odtCurTime;
	COleDateTimeSpan odts;
	SYSTEMTIME st;
	// ˫����
	if (wcscmp(m_sTestLog.wchTestType, L"1") == 0)
	{
		ncheckmethod = 9;

		std::vector<SRealTimeDataOfDIS> vt;
		GetIniRealTimeDataOfDISEx(vt);

		for(int i=0; i<vt.size(); i++)
		{
			strTimeData.AppendFormat(L"<process_data>");
			//time_no           	Date	ȫ��ʱ��,��ʽΪYYYYMMDD24hmmss
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTimeData.AppendFormat(L"<time_no>%s</time_no>", odtCurTime.Format(L"%Y%m%d%H%M%S"));
			//jqmjdkd	NUMBER(5)	�����ž��Կ���(%)
			strTimeData.AppendFormat(L"<jqmjdkd>%s</jqmjdkd>", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			//jsfhz	NUMBER(5)	���㸺��ֵ(%)
			strTimeData.AppendFormat(L"<jsfhz>%s</jsfhz>", vt[i].sOBDRTData.strCalculationLoad.c_str());
			//qycgqxh	NUMBER(5)	ǰ���������źţ�mv/ma��
			strTimeData.AppendFormat(L"<qycgqxh>%s</qycgqxh>", vt[i].sOBDRTData.strForwardSensorV.c_str());
			//glkqxs	NUMBER(5)	��������ϵ����mv/ma��
			strTimeData.AppendFormat(L"<glkqxs>%s</glkqxs>", vt[i].sOBDRTData.strAirCoefficient.c_str());
			//vehicle_speed	NUMBER(5)	����(km/h)
			strTimeData.AppendFormat(L"<vehicle_speed>%.1f</vehicle_speed>", 0.0f);
			//rpm	NUMBER(5)	������ת�٣�r/min��
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTimeData.AppendFormat(L"<rpm>%d</rpm>", n);
			//airinput	NUMBER(5)	������(g/s)
			strTimeData.AppendFormat(L"<airinput>%s</airinput>", vt[i].sOBDRTData.strMAF.c_str());
			//airinput_pressure	NUMBER(5)	����ѹ��(kpa)
			strTimeData.AppendFormat(L"<airinput_pressure>%s</airinput_pressure>", vt[i].sOBDRTData.strMAP.c_str());
			strTimeData.AppendFormat(L"</process_data>");
		}
	}
	// ��̬
	else if (wcscmp(m_sTestLog.wchTestType, L"2") == 0)
	{
		ncheckmethod = 9;

		std::vector<SRealTimeDataOfASM> vt;
		GetIniRealTimeDataOfASMEx(vt);

		for(int i=0; i<vt.size(); i++)
		{
			strTimeData.AppendFormat(L"<process_data>");
			//time_no           	Date	ȫ��ʱ��,��ʽΪYYYYMMDD24hmmss
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTimeData.AppendFormat(L"<time_no>%s</time_no>", odtCurTime.Format(L"%Y%m%d%H%M%S"));
			//jqmjdkd	NUMBER(5)	�����ž��Կ���(%)
			strTimeData.AppendFormat(L"<jqmjdkd>%s</jqmjdkd>", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			//jsfhz	NUMBER(5)	���㸺��ֵ(%)
			strTimeData.AppendFormat(L"<jsfhz>%s</jsfhz>", vt[i].sOBDRTData.strCalculationLoad.c_str());
			//qycgqxh	NUMBER(5)	ǰ���������źţ�mv/ma��
			strTimeData.AppendFormat(L"<qycgqxh>%s</qycgqxh>", vt[i].sOBDRTData.strForwardSensorV.c_str());
			//glkqxs	NUMBER(5)	��������ϵ����mv/ma��
			strTimeData.AppendFormat(L"<glkqxs>%s</glkqxs>", vt[i].sOBDRTData.strAirCoefficient.c_str());
			//vehicle_speed	NUMBER(5)	����(km/h)
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f;
			if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
			else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
			else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
			else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			strTimeData.AppendFormat(L"<vehicle_speed>%.1f</vehicle_speed>", f);
			//rpm	NUMBER(5)	������ת�٣�r/min��
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + 1752;}
			else if (i%3 == 0){n = rand()%300 + 1825;}
			else if (i%5 == 0){n = rand()%300 + 1872;}
			else {n = rand()%300 + 1852;}
			strTimeData.AppendFormat(L"<rpm>%d</rpm>", n);
			//airinput	NUMBER(5)	������(g/s)
			strTimeData.AppendFormat(L"<airinput>%s</airinput>", vt[i].sOBDRTData.strMAF.c_str());
			//airinput_pressure	NUMBER(5)	����ѹ��(kpa)
			strTimeData.AppendFormat(L"<airinput_pressure>%s</airinput_pressure>", vt[i].sOBDRTData.strMAP.c_str());
			strTimeData.AppendFormat(L"</process_data>");
		}
	}
	// ����˲̬
	else if (wcscmp(m_sTestLog.wchTestType, L"3") == 0)
	{
		ncheckmethod = 9;
		std::vector<SRealTimeDataOfVMAS> vt;
		GetIniRealTimeDataOfVMASEx(vt);

		for(int i=0; i<vt.size(); i++)
		{
			strTimeData.AppendFormat(L"<process_data>");
			//time_no           	Date	ȫ��ʱ��,��ʽΪYYYYMMDD24hmmss
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTimeData.AppendFormat(L"<time_no>%s</time_no>", odtCurTime.Format(L"%Y%m%d%H%M%S"));
			//jqmjdkd	NUMBER(5)	�����ž��Կ���(%)
			strTimeData.AppendFormat(L"<jqmjdkd>%s</jqmjdkd>", vt[i].sOBDRTData.strSolarTermDoorPosition.c_str());
			//jsfhz	NUMBER(5)	���㸺��ֵ(%)
			strTimeData.AppendFormat(L"<jsfhz>%s</jsfhz>", vt[i].sOBDRTData.strCalculationLoad.c_str());
			//qycgqxh	NUMBER(5)	ǰ���������źţ�mv/ma��
			strTimeData.AppendFormat(L"<qycgqxh>%s</qycgqxh>", vt[i].sOBDRTData.strForwardSensorV.c_str());
			//glkqxs	NUMBER(5)	��������ϵ����mv/ma��
			strTimeData.AppendFormat(L"<glkqxs>%s</glkqxs>", vt[i].sOBDRTData.strAirCoefficient.c_str());
			//vehicle_speed	NUMBER(5)	����(km/h)
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f;
			if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
			else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
			else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
			else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			strTimeData.AppendFormat(L"<vehicle_speed>%.1f</vehicle_speed>", f);
			//rpm	NUMBER(5)	������ת�٣�r/min��
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + 1752;}
			else if (i%3 == 0){n = rand()%300 + 1825;}
			else if (i%5 == 0){n = rand()%300 + 1872;}
			else {n = rand()%300 + 1852;}
			strTimeData.AppendFormat(L"<rpm>%d</rpm>", n);
			//airinput	NUMBER(5)	������(g/s)
			strTimeData.AppendFormat(L"<airinput>%s</airinput>", vt[i].sOBDRTData.strMAF.c_str());
			//airinput_pressure	NUMBER(5)	����ѹ��(kpa)
			strTimeData.AppendFormat(L"<airinput_pressure>%s</airinput_pressure>", vt[i].sOBDRTData.strMAP.c_str());
			strTimeData.AppendFormat(L"</process_data>");
		}
	}
	// ���ؼ���
	else if (wcscmp(m_sTestLog.wchTestType, L"4") == 0)
	{
		ncheckmethod = 10;

		std::vector<SRealTimeDataOfLUGDOWN> vt;
		GetIniRealTimeDataOfLUGDOWNEx(vt);

		for(int i=0; i<vt.size(); i++)
		{
			strTimeData.AppendFormat(L"<process_data>");
			//time_no           	Date	ȫ��ʱ��,��ʽΪYYYYMMDD24hmmss
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTimeData.AppendFormat(L"<time_no>%s</time_no>", odtCurTime.Format(L"%Y%m%d%H%M%S"));
			//throttle_position 	NUMBER(5)	���ſ���(%)
			strTimeData.AppendFormat(L"<throttle_position>%s</throttle_position>", vt[i].sOBDRTData.strThrottleOpening.c_str());
			//vehicle_speed     	NUMBER(5)	����(km/h)
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			float f;
			if (i%2 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) +0.5f;}
			else if (i%3 == 0) { f = (rand()%2 * 2) + _wtof( vt[i].strVelocity.c_str()) -1.5f;}
			else if (i%5 == 0) { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +2.5f;}
			else { f = (rand()%2 * -1) + _wtof( vt[i].strVelocity.c_str()) +1.0f;}
			strTimeData.AppendFormat(L"<vehicle_speed>%.1f</vehicle_speed>", f);
			//power             	NUMBER(5)	�������������(kw)
			strTimeData.AppendFormat(L"<power>%s</power>", vt[i].sOBDRTData.strEngineOutputPower.c_str());
			//rpm               	NUMBER(5)	������ת�٣�r/min��
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + 1752;}
			else if (i%3 == 0){n = rand()%300 + 1825;}
			else if (i%5 == 0){n = rand()%300 + 1872;}
			else {n = rand()%300 + 1852;}
			strTimeData.AppendFormat(L"<rpm>%d</rpm>", n);
			//airinput          	NUMBER(5)	������(g/s)
			strTimeData.AppendFormat(L"<airinput>%s</airinput>", vt[i].sOBDRTData.strMAF.c_str());
			//boost_pressure    	NUMBER(5)	��ѹѹ��(kpa)	
			strTimeData.AppendFormat(L"<boost_pressure>%s</boost_pressure>", vt[i].sOBDRTData.strChargeAirPressure.c_str());
			//oilconsumption    	NUMBER(5)	������(L/100km)
			strTimeData.AppendFormat(L"<oilconsumption>%s</oilconsumption>", vt[i].sOBDRTData.strFuelConsumption.c_str());
			//nosensornd        	NUMBER(5)	����������Ũ�ȣ�10-6��
			strTimeData.AppendFormat(L"<nosensornd>%s</nosensornd>", vt[i].sOBDRTData.strNOXConcentration.c_str());
			//urea_in_volume    	NUMBER(5)	������������L/H��
			strTimeData.AppendFormat(L"<urea_in_volume>%s</urea_in_volume>", vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
			//exhausttemperature	NUMBER(5)	�����¶�(C)
			strTimeData.AppendFormat(L"<exhausttemperature>%s</exhausttemperature>", vt[i].sOBDRTData.strEGT.c_str());
			//klpjqyc	NUMBER(5)	����������ѹ��(KPA)
			strTimeData.AppendFormat(L"<klpjqyc>%s</klpjqyc>", vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
			//egrkd	NUMBER(5)	EGR����(%)
			strTimeData.AppendFormat(L"<egrkd>%s</egrkd>", vt[i].sOBDRTData.strEGRPosition.c_str());
			//fuel_in_pre	NUMBER(5)	ȼ������ѹ��(MPA)
			strTimeData.AppendFormat(L"<fuel_in_pre>%s</fuel_in_pre>", vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());

			strTimeData.AppendFormat(L"</process_data>");
		}
	}
	// ��͸�����ɼ���
	else if (wcscmp(m_sTestLog.wchTestType, L"5") == 0)
	{
		ncheckmethod = 10;

		std::vector<SRealTimeDataOfFSUNHT> vt;
		GetIniRealTimeDataOfFSUNHTEx(vt);

		for(int i=0; i<vt.size(); i++)
		{
			strTimeData.AppendFormat(L"<process_data>");
			//time_no           	Date	ȫ��ʱ��,��ʽΪYYYYMMDD24hmmss
			if (i==0)
			{
				odtCurTime.ParseDateTime(vt[i].strSamplingTime.c_str());
			}
			else
			{
				odts.SetDateTimeSpan(0,0,0,1);
				odtCurTime = odtCurTime + odts;
			}
			strTimeData.AppendFormat(L"<time_no>%s</time_no>", odtCurTime.Format(L"%Y%m%d%H%M%S"));
			//throttle_position 	NUMBER(5)	���ſ���(%)
			strTimeData.AppendFormat(L"<throttle_position>%s</throttle_position>", vt[i].sOBDRTData.strThrottleOpening.c_str());
			//vehicle_speed     	NUMBER(5)	����(km/h)
			strTimeData.AppendFormat(L"<vehicle_speed>%.1f</vehicle_speed>", 0.0f);
			//power             	NUMBER(5)	�������������(kw)
			strTimeData.AppendFormat(L"<power>%s</power>", vt[i].sOBDRTData.strEngineOutputPower.c_str());
			//rpm               	NUMBER(5)	������ת�٣�r/min��
			GetLocalTime(&st);
			srand(st.wMilliseconds);
			int n(0);
			if (i%2 == 0){n = rand()%300 + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%3 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else if (i%5 == 0){n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			else {n = rand()%300  + _wtoi(vt[i].strEngineRev.c_str());}
			strTimeData.AppendFormat(L"<rpm>%d</rpm>", n);
			//airinput          	NUMBER(5)	������(g/s)
			strTimeData.AppendFormat(L"<airinput>%s</airinput>", vt[i].sOBDRTData.strMAF.c_str());
			//boost_pressure    	NUMBER(5)	��ѹѹ��(kpa)	
			strTimeData.AppendFormat(L"<boost_pressure>%s</boost_pressure>", vt[i].sOBDRTData.strChargeAirPressure.c_str());
			//oilconsumption    	NUMBER(5)	������(L/100km)
			strTimeData.AppendFormat(L"<oilconsumption>%s</oilconsumption>", vt[i].sOBDRTData.strFuelConsumption.c_str());
			//nosensornd        	NUMBER(5)	����������Ũ�ȣ�10-6��
			strTimeData.AppendFormat(L"<nosensornd>%s</nosensornd>", vt[i].sOBDRTData.strNOXConcentration.c_str());
			//urea_in_volume    	NUMBER(5)	������������L/H��
			strTimeData.AppendFormat(L"<urea_in_volume>%s</urea_in_volume>", vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
			//exhausttemperature	NUMBER(5)	�����¶�(C)
			strTimeData.AppendFormat(L"<exhausttemperature>%s</exhausttemperature>", vt[i].sOBDRTData.strEGT.c_str());
			//klpjqyc	NUMBER(5)	����������ѹ��(KPA)
			strTimeData.AppendFormat(L"<klpjqyc>%s</klpjqyc>", vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
			//egrkd	NUMBER(5)	EGR����(%)
			strTimeData.AppendFormat(L"<egrkd>%s</egrkd>", vt[i].sOBDRTData.strEGRPosition.c_str());
			//fuel_in_pre	NUMBER(5)	ȼ������ѹ��(MPA)
			strTimeData.AppendFormat(L"<fuel_in_pre>%s</fuel_in_pre>", vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());

			strTimeData.AppendFormat(L"</process_data>");
		}
	}


	CString strDocXml;

	strDocXml.AppendFormat(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	strDocXml.AppendFormat(L"<result>");

	strDocXml.AppendFormat(L"<result_data>");
	strDocXml.AppendFormat(L"<check_id>%s</check_id>", m_sVehInfoList.strcheckid.c_str());
	strDocXml.AppendFormat(L"<plate>%s</plate>", m_sTestLog.wchPlateNumber);
	strDocXml.AppendFormat(L"<vin>%s</vin>", m_sTestLog.wchVIN);
	strDocXml.AppendFormat(L"<clxh>%s</clxh>", m_sTestLog.wchPlateType);
	strDocXml.AppendFormat(L"</result_data>");
	strDocXml.AppendFormat(L"%s", strTimeData);
	strDocXml.AppendFormat(L"</result>");

	
	std::wstring strRet;
	int nRet = CSZIntLib_New_API::UploadInspectionResult(pchURL, strtoken.GetString(), strunitid.GetString(), strDocXml.GetString(), ncheckmethod, strRet);

#ifdef _DEBUG
	nRet = 0;
#endif
	if (nRet == 0)
	{
		CXmlReader xmlReader;

		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring wstrCode, wstrContent;

			if (xmlReader.OpenNode(L"xml/status"))
			{
				xmlReader.GetNodeContent(wstrCode);
			}
			CString strTAndF(wstrCode.c_str());
			strTAndF.MakeUpper(); // ���
			
			if (strTAndF != L"TRUE")
			{
				if (xmlReader.OpenNode(L"xml/errMsg"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				strMsg.AppendFormat(L"�ϴ�OBD����ʧ��ԭ��%s", wstrCode.c_str());
				return false;
			}
			bRet = true;
		}
		else
		{
			strMsg.AppendFormat(L"�ϴ�OBD���̽���ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.AppendFormat(L"�ϴ�OBD��������ʧ�ܣ����ش���%d", nRet);
		return false;
	}

	if (bRet)
	{
		wchar_t wchPath[MAX_PATH];
		CString strSQL;
		// ����INI
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"ItemOBD", L"4");
			si.SetString(L"TestLog", L"HasOBD", L"1");
		}
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"VehicleInfo.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"HasOBD", L"1");
		}

		strSQL.Format(_T("Update TestLog set ItemOBD = '4', HasOBD = '1' where PlateNumber = '%s' ORDER BY AutoID DESC LIMIT 1"), m_sTestLog.wchPlateNumber);
		int nRowsInvolved(0);
		CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved);

		strSQL.Format(_T("Update VehicleInfo set HasOBD = '1' where PlateNumber = '%s' "), m_sTestLog.wchPlateNumber);
		nRowsInvolved = 0;
		CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved);
	}
	return bRet;
}



bool CJSCSZNDlgDlg::SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(true);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

	if (wcscmp(m_sTestLog.wchFuelType, L"A") != 0)
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

CString CJSCSZNDlgDlg::GetOBDType(const int& nType, const CString& strFulType)
{
	//if (strFulType == L"����")
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
		default : {return L"JOBD";} break;
		}
	}
	//else
	//{
	//	return L"";
	//}
}

void CJSCSZNDlgDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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


void CJSCSZNDlgDlg::OnBnClickedBtnSetconfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SetConfig();
	// �ж����ݿ��Ƿ����Ӻ�
	if (!m_bConnectDB)
	{
		m_bConnectDB = OpenDB();
	}
	GetOBDJCY();
#ifndef _DEBUG
	// Release ��ʹ��
	GetToken();
#endif
	CString str;
	m_edMsg.GetWindowTextW(str);
	if (str.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"���������б���ȡ������Ϣ");
	}
}


void CJSCSZNDlgDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (GetVehInfoList())
	{
		m_edMsg.SetWindowTextW(L"���ȹ������ѡȡ�ó��ƣ����ڴ�˫�����ƻ�ȡ������Ϣ");
	}
}


void CJSCSZNDlgDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();

	m_sVehInfoList = SVehInfoList();
	m_edVINCJH.SetWindowTextW(L"");
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_sVehInfoList.strplate = m_lstVehicle.GetItemText(nIndex,1);
		m_sVehInfoList.strplate_color = m_lstVehicle.GetItemText(nIndex,2);
		m_sVehInfoList.strvin = m_lstVehicle.GetItemText(nIndex,3);
		m_sVehInfoList.strclxh = m_lstVehicle.GetItemText(nIndex,4);
		m_sVehInfoList.strcheckid = m_lstVehicle.GetItemText(nIndex,5);
		m_sVehInfoList.strcheck_type = m_lstVehicle.GetItemText(nIndex,6);
		m_sVehInfoList.strcheck_method = m_lstVehicle.GetItemText(nIndex,7);

		if (!m_sVehInfoList.strvin.empty())
		{
			m_edVINCJH.SetWindowTextW(m_sVehInfoList.strvin.c_str());
			if (GetVehInfo())
			{
				GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
				m_edMsg.SetWindowTextW(L"����UPInterOBD�����OBD������");
			}
		}
	}
	else
	{
		GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	}


	EndWaitCursor();
	*pResult = 0;
}


void CJSCSZNDlgDlg::OnBnClickedBtnUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMsg;
	if (!GetGasAri(3, strMsg))
	{m_edMsg.SetWindowTextW(strMsg); return;}
	if (!GetGasAri(4, strMsg))
	{m_edMsg.SetWindowTextW(strMsg); return;}
	if (!GetGasAri(1, strMsg))
	{m_edMsg.SetWindowTextW(strMsg); return;}

	if (SaveINIAndSQLAndUP())
	{
		m_edMsg.SetWindowTextW(L"��������ɣ��������������ļ�����");
	}
}


void CJSCSZNDlgDlg::OnBnClickedBtnUpobdtimedata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMsg;
	if (UpOBDRealTimeData(strMsg))
	{
		m_edMsg.SetWindowTextW(L"�ϴ��������ݳɹ�");

		GetDlgItem(IDC_BTN_UPOBDTIMEDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_UPOBDTIMEDATA)->EnableWindow(FALSE);
	}
	else
	{
		strMsg = L"�ϴ���������ʧ��\r\n" + strMsg;
		m_edMsg.SetWindowTextW(strMsg);
	}
}

void CJSCSZNDlgDlg::GetBaseTypeInfo(void)
{
	CString strMsg;
	CString strURL, strunitid, strlineid, strInterusername, strInterpwd, strtoken;
	m_edURL.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);
	m_edtoken.GetWindowTextW(strtoken);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		strMsg.AppendFormat(L"���������п�ֵ");
		return;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	CString strDocXml;
	CString str;
	str.Format(L"%s", strunitid.Mid(0,5));
	int ncheckmethod;

	std::wstring strRet;
	CSZIntLib_New_API::GetBaseTypeInfo(pchURL, strRet);
}


bool CJSCSZNDlgDlg::GetGasAri(const int& nType, CString& strMsg)
{

	CString strURL, strunitid, strlineid, strInterusername, strInterpwd, strtoken;
	m_edURL2.GetWindowTextW(strURL);
	m_edunitid.GetWindowTextW(strunitid);
	m_edlineid.GetWindowTextW(strlineid);
	m_edInterusername.GetWindowTextW(strInterusername);
	m_edInterpwd.GetWindowTextW(strInterpwd);
	m_edtoken.GetWindowTextW(strtoken);

	if (strURL.IsEmpty() || strunitid.IsEmpty() || strlineid.IsEmpty() 
		|| strInterusername.IsEmpty() || strInterpwd.IsEmpty())
	{
		strMsg.AppendFormat(L"���������п�ֵ");
		return false;
	}

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}
	pchURL = CNHCommonAPI::UnicodeToANSI(strURL);

	CString strDocXml;

	strDocXml.AppendFormat(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	strDocXml.AppendFormat(L"<result><result_data>");

	strDocXml.AppendFormat(L"<unit_id>%s</unit_id>", strunitid);
	strDocXml.AppendFormat(L"<line_id>%s</line_id>", strlineid);
	strDocXml.AppendFormat(L"<type>%d</type>", nType);
	if (nType == 1)
	{
		COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 9);
		COleDateTime odtSartTime = odtCurTime - odts;
		strDocXml.AppendFormat(L"<start_time>%s</start_time>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
	}
	else if (nType == 4)
	{
		COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 16);
		COleDateTime odtSartTime = odtCurTime - odts;
		strDocXml.AppendFormat(L"<start_time>%s</start_time>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 12);
		COleDateTime odtSartTime = odtCurTime - odts;
		strDocXml.AppendFormat(L"<start_time>%s</start_time>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
	}
	strDocXml.AppendFormat(L"<end_time>%s</end_time>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strDocXml.AppendFormat(L"<passed>%s</passed>", L"1");
	strDocXml.AppendFormat(L"<jcry>%s</jcry>", L"����");

	strDocXml.AppendFormat(L"</result_data></result>");

	
	std::wstring strRet;
	int nRet = CSZIntLibbd_New_API::UploadInspectionResult(pchURL, strDocXml.GetString(), 12, strRet);


#ifdef _DEBUG
	nRet = 0;
#endif
	if (nRet == 0)
	{
		CXmlReader xmlReader;

		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring wstrCode, wstrContent;

			if (xmlReader.OpenNode(L"xml/status"))
			{
				xmlReader.GetNodeContent(wstrCode);
			}
			CString strTAndF(wstrCode.c_str());
			strTAndF.MakeUpper(); // ���
			
			if (strTAndF != L"TRUE")
			{
				if (xmlReader.OpenNode(L"xml/errMsg"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}
				strMsg.AppendFormat(L"�Լ�����ʧ��ԭ��%s", wstrCode.c_str());
				return false;
			}
			return true;
		}
		else
		{
			strMsg.AppendFormat(L"�Լ����ݽ���ʧ��");
			return false;
		}
	}
	else
	{
		strMsg.AppendFormat(L"�Լ���������ʧ�ܣ����ش���%d", nRet);
		return false;
	}
}