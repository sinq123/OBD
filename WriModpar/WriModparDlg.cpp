
// WriModparDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriModpar.h"
#include "WriModparDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWriModparDlg �Ի���




CWriModparDlg::CWriModparDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriModparDlg::IDD, pParent)
	, m_pchURL(NULL)
	, m_strURL(L"")
	, m_strStationNum(L"")
	, m_strLineNum(L"")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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

}

CWriModparDlg::~CWriModparDlg()
{
	// ɾ������
	m_fontDlgFont.DeleteObject();

	if (m_pchURL != NULL)
	{
		free(m_pchURL);
		m_pchURL = NULL;
	}
}

void CWriModparDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, IDC_LST_PENDING, m_lstPending);
	DDX_Control(pDX, IDC_BTN_UP_VEHICLE, m_btnUpVehicle);
	DDX_Control(pDX, IDC_BTN_UP_PENDING, m_btnUpPending);
}

BEGIN_MESSAGE_MAP(CWriModparDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CWriModparDlg::OnLvnItemchangedLstVehicle)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_PENDING, &CWriModparDlg::OnLvnItemchangedLstPending)
	ON_BN_CLICKED(IDC_BTN_UP_VEHICLE, &CWriModparDlg::OnBnClickedBtnUpVehicle)
	ON_BN_CLICKED(IDC_BTN_UP_PENDING, &CWriModparDlg::OnBnClickedBtnUpPending)
END_MESSAGE_MAP()


// CWriModparDlg ��Ϣ�������

BOOL CWriModparDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��ʼ���ؼ�
	InitCtrls();

	GetConfig();
	ShowWindow(SW_MAXIMIZE); // ���

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWriModparDlg::OnPaint()
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
HCURSOR CWriModparDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWriModparDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);
}


void CWriModparDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(nType==1) return;//��С����ʲô������  

	float fsp[2];
	POINT Newp;//��ȡ���ڶԻ���Ĵ�С(VS2005����CPOINT)
	CRect recta;
	GetClientRect(&recta); //ȡ�ͻ�����С
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/m_Old.x;
	fsp[1]=(float)Newp.y/m_Old.y;

	if (fsp[0] != 0 && fsp[1] != 0)
	{
		CRect Rect;
		int woc;
		CPoint OldTLPoint,TLPoint; //���Ͻ�
		CPoint OldBRPoint,BRPoint; //���½�
		HWND hwndChild=::GetWindow(m_hWnd,GW_CHILD); //�г����пؼ�

		while(hwndChild)
		{
			woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft(); 
			TLPoint.x = long(OldTLPoint.x*fsp[0]); 
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			OldBRPoint = Rect.BottomRight();
			BRPoint.x = long(OldBRPoint.x *fsp[0]); 
			BRPoint.y = long(OldBRPoint.y *fsp[1]); //�߶Ȳ��ɶ��Ŀؼ�����:combBox),��Ҫ�ı��ֵ.
			Rect.SetRect(TLPoint,BRPoint); 
			GetDlgItem(woc)->MoveWindow(Rect,TRUE);
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
		}
		m_Old=Newp;
	}
}


void CWriModparDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveToDB();
	GetPending();
	*pResult = 0;
}


void CWriModparDlg::OnLvnItemchangedLstPending(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DetWhiModPar();
	GetPending();
	*pResult = 0;
}


void CWriModparDlg::OnBnClickedBtnUpVehicle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetIntVehList();
}


void CWriModparDlg::OnBnClickedBtnUpPending()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetPending();
}


void CWriModparDlg::InitCtrls(void)
{
	// ��������
	SetDlgFont();

	InitVehicleList();

	m_btnUpVehicle.SetFlat(FALSE);
	m_btnUpPending.SetFlat(FALSE);
}

void CWriModparDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CWriModparDlg::InitVehicleList(void)
{

	int nFontHeight(80);

	m_fontVehicleLCFont.CreateFont(nFontHeight,
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

	const int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);

	m_lstVehicle.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstVehicle.SetFont(&m_fontVehicleLCFont);
	m_lstVehicle.SetTextColor(RGB(0, 255, 0));
	m_lstVehicle.SetBkColor(RGB(0, 0, 0));
	m_lstVehicle.SetTextBkColor(RGB(0, 0, 0));

	m_lstVehicle.InsertColumn(0, L"���",		0,	0*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(1, L"��ˮ��",	0,	320*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(2, L"���ƺ���",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(3, L"������",	0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(4, L"��������",		0,	120*nSM_CXSCREEN/1280);
	m_lstVehicle.InsertColumn(5, L"��ⷽ��",	0,	120*nSM_CXSCREEN/1280);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);

	m_lstPending.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstPending.SetFont(&m_fontVehicleLCFont);
	m_lstPending.SetTextColor(RGB(0, 255, 0));
	m_lstPending.SetBkColor(RGB(0, 0, 0));
	m_lstPending.SetTextBkColor(RGB(0, 0, 0));

	m_lstPending.InsertColumn(0, L"���",		0,	0*nSM_CXSCREEN/1280);
	m_lstPending.InsertColumn(1, L"��ˮ��",	0,	320*nSM_CXSCREEN/1280);
	m_lstPending.InsertColumn(2, L"���ƺ���",	0,	120*nSM_CXSCREEN/1280);
	m_lstPending.InsertColumn(3, L"������",	0,	120*nSM_CXSCREEN/1280);
	m_lstPending.InsertColumn(4, L"��������",		0,	120*nSM_CXSCREEN/1280);
	m_lstPending.InsertColumn(5, L"��ⷽ��",	0,	120*nSM_CXSCREEN/1280);

	pHeaderCtrl = m_lstPending.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&m_fontDlgFont);
}

void CWriModparDlg::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"__%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

void CWriModparDlg::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"ACWebConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strURL = si.GetString(L"EPB", L"WebAddress", L"");
		if (NULL != m_pchURL)
		{
			free(m_pchURL);
			m_pchURL = NULL;
		}
		if (!m_strURL.IsEmpty())
		{
			m_pchURL = CNHCommonAPI::UnicodeToANSI(m_strURL);
		}
		m_strStationNum = si.GetString(L"EPB", L"JCZBH", L"");
		m_strLineNum = si.GetString(L"EPB", L"JKXLH", L"");
	}

	m_odtDemarcationDate = COleDateTime::GetCurrentTime();
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationLog.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;
		strTemp = si.GetString(L"DemarcationLog", L"DemarcationDate", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));

		if(!m_odtDemarcationDate.ParseDateTime(strTemp))
		{
			m_odtDemarcationDate = COleDateTime::GetCurrentTime();
		}
	}

}

CString CWriModparDlg::DecodeURI(LPCSTR szURIString)
{
	std::string strSource(szURIString);
	int nSrcLen = strSource.size();

	std::string strDestA;

	int nIndex(0);
	while (nIndex < nSrcLen)
	{
		if (strSource.at(nIndex) == '%'
			&& nIndex+2 <= nSrcLen)
		{
			std::string strValue = strSource.substr(nIndex, 3);
			int nValue(0);
			sscanf_s(strValue.c_str(), "%%%x", &nValue);
			strDestA += (char)nValue;
			nIndex += 3;
		}
		else if (strSource.at(nIndex) == '+')
		{
			strDestA += ' ';
			nIndex++;
		}
		else
		{
			strDestA += (char)strSource.at(nIndex);
			nIndex++;
		}
	}
	CString strDest(UTF8ToTChar(strDestA.c_str()));
	return strDest;
}

CString CWriModparDlg::DecodeURI(LPCWSTR szURIString)
{
	return DecodeURI(KUTF16ToANSI(szURIString));
}


void CWriModparDlg::GetIntVehList(void)
{
	// ����
	CString strTimes, strXmlDoc;

	SYSTEMTIME st;
	GetLocalTime(&st);
	strTimes.Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strXmlDoc.AppendFormat(L"<?xml version=\"1.0\" encoding=\"GBK\"?><root><head>");
	strXmlDoc.AppendFormat(L"<organ>%s</organ>", m_strStationNum);
	strXmlDoc.AppendFormat(L"<jkxlh>%s</jkxlh>", m_strLineNum);
	strXmlDoc.AppendFormat(L"<jkid>%s</jkid>", L"DJ001");
	strXmlDoc.AppendFormat(L"<sjc>%s</sjc>", strTimes);
	strXmlDoc.AppendFormat(L"</head><vehispara>");
	strXmlDoc.AppendFormat(L"</vehispara></root>");

	std::wstring strRetStr(L"");
	int nRet;
	nRet = CACInterfaceLib_API::QueryObjectOut(m_pchURL, strXmlDoc.GetString(), strRetStr);

	if (nRet == 0)
	{
		strRetStr = (LPCTSTR)DecodeURI(strRetStr.c_str());

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
				if (xmlReader.OpenNode(L"root/body/vehispara"))
				{
					// ɾ���б�����
					m_lstVehicle.DeleteAllItems();

					int nItem(0);

					std::wstring strNodeName, strName, strContent;
					do
					{
						CString str;
						// ���
						str.Format(L"%d", nItem+1);
						int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

						xmlReader.EnterNode();
						do
						{
							xmlReader.GetNodeName(strNodeName);
							// ���վ���
							if(L"tsno"==strNodeName)  
							{
							}
							// ������ˮ��
							if(L"jylsh"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent);
								m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// �������
							if(L"testtimes"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ���ƺ���
							if(L"license"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ��������
							if(L"licensecode"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, strContent.c_str(), 0, 0, 0, 0);
							}
							// ��ⷽ��
							if(L"testtype"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
								int n = _wtoi(strContent.c_str());
								switch (n)
								{
								case 1:{m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"˫����", 0, 0, 0, 0);}break;
								case 2:{m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"��̬", 0, 0, 0, 0);}break;
								case 3:{m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"����˲̬", 0, 0, 0, 0);}break;
								case 4:{m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"���ؼ���", 0, 0, 0, 0);}break;
								case 5:{m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"��͸���̶ȷ�", 0, 0, 0, 0);}break;
								case 6:{m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"��ֽʽ�̶ȷ�", 0, 0, 0, 0);}break;
								default: {m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, L"��������", 0, 0, 0, 0);}break;
								}
							}
							// ��¼ʱ��
							if(L"dlsj"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
							}
							// ״̬
							if(L"zt"==strNodeName)  
							{ 
								xmlReader.GetNodeContent(strContent); 
							}
						}
						while (xmlReader.NextSibling());
						nItem++;
						xmlReader.ExitNode();
					}
					while (xmlReader.NextSibling());
				}
			}
			else
			{
				CString strMsg;
				strMsg.Format(L"��ȡ�б���Ϣ����:%s %s", strCode.c_str(), strContent.c_str());
				MessageBox(strMsg);
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(L"��ȡ�б���Ϣ����:%d %s", nRet, L"����ʧ��");
			MessageBox(strMsg);
			return;
		}


	}
	else
	{
		CString strMsg;
		strMsg.Format(L"��ȡ�б���Ϣ����:%d %s", nRet, L"�ӿڷ���ʧ��");
		MessageBox(strMsg);
		return;
	}
}

void CWriModparDlg::GetPending(void)
{
	CString strSql;
	strSql.Format(L"select * from WhiteModifiedParameters");

	std::vector<SWhiteModifiedParameters> vtWhiteModifiedParameters;
	if (GetDboWhiteModifiedParameters(strSql, vtWhiteModifiedParameters) == 0xff) 
	{
		CString strMsg;
		strMsg.Format(L"��ȡ��������Ϣ����:%s", L"���ݿ����ʧ��");
		MessageBox(strMsg);
		return;
	}

	if (vtWhiteModifiedParameters.size() > 0)
	{
		// ɾ���б�����
		m_lstPending.DeleteAllItems();
		int nItem(0);
		for (UINT i=0; i<vtWhiteModifiedParameters.size(); i++)
		{
			CString str;
			// ���
			str.Format(L"%d", nItem+1);
			int nItemIndex = m_lstPending.InsertItem(nItem++, str);

			m_lstPending.SetItem (nItemIndex, 1, LVIF_TEXT, vtWhiteModifiedParameters[i].strA.c_str(), 0, 0, 0, 0);
			m_lstPending.SetItem (nItemIndex, 2, LVIF_TEXT, vtWhiteModifiedParameters[i].strB.c_str(), 0, 0, 0, 0);
			m_lstPending.SetItem (nItemIndex, 3, LVIF_TEXT, vtWhiteModifiedParameters[i].strC.c_str(), 0, 0, 0, 0);
			m_lstPending.SetItem (nItemIndex, 4, LVIF_TEXT, vtWhiteModifiedParameters[i].strD.c_str(), 0, 0, 0, 0);
			m_lstPending.SetItem (nItemIndex, 5, LVIF_TEXT, vtWhiteModifiedParameters[i].strE.c_str(), 0, 0, 0, 0);
			m_lstPending.SetItem (nItemIndex, 6, LVIF_TEXT, vtWhiteModifiedParameters[i].strF.c_str(), 0, 0, 0, 0);
		}
	}
}


bool CWriModparDlg::SaveToDB(void)
{
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		SWhiteModifiedParameters sWhiteModifiedParameters;
		sWhiteModifiedParameters.strA = m_lstVehicle.GetItemText(nIndex,1).GetString();
		sWhiteModifiedParameters.strB = m_lstVehicle.GetItemText(nIndex,2).GetString();
		sWhiteModifiedParameters.strC = m_lstVehicle.GetItemText(nIndex,3).GetString();
		sWhiteModifiedParameters.strD = m_lstVehicle.GetItemText(nIndex,4).GetString();
		sWhiteModifiedParameters.strE = m_lstVehicle.GetItemText(nIndex,5).GetString();


		if (SetDboWhiteModifiedParameters(sWhiteModifiedParameters) != 0x00)
		{
			MessageBox(L"����WhiteModifiedParameters��ʧ��");
			return false;
		}
	}
	else
	{
		return false;
	}


	return true;
}

bool CWriModparDlg::DetWhiModPar(void)
{
	CString strMsg;
	int nIndex = m_lstPending.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		SWhiteModifiedParameters sWhiteModifiedParameters;
		sWhiteModifiedParameters.strA = m_lstPending.GetItemText(nIndex,1).GetString();
		sWhiteModifiedParameters.strB = m_lstPending.GetItemText(nIndex,2).GetString();

		strMsg.Format(L"�Ƿ�ɾ��[%s]��¼", sWhiteModifiedParameters.strB.c_str());
		if (MessageBox(strMsg, L"ɾ����¼", MB_YESNO) == IDNO)
		{
			return false;
		}

		_ConnectionPtr pConnection;
		if (0x00 == CNHSQLServerDBO::OpenDB(pConnection))
		{
			CString strSql;
			strSql.Format(L"delete WhiteModifiedParameters where A = '%s'", sWhiteModifiedParameters.strA.c_str());

			if (0x00 != CNHSQLServerDBO::ExecuteDML(pConnection, strSql))
			{
				strMsg.Format(L"ɾ��[%s]��¼ʧ��", sWhiteModifiedParameters.strB.c_str());
				MessageBox(strMsg);
				return false;
			}
		}

		CNHSQLServerDBO::CloseDB(pConnection);
	}
	else
	{
		return false;
	}

	return true;
}