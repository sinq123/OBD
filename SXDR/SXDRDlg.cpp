
// SXDRDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SXDR.h"
#include "SXDRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSXDRDlg �Ի���




CSXDRDlg::CSXDRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSXDRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSXDRDlg::~CSXDRDlg()
{
	CloaseScket();
}

void CSXDRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_SERVERIP, m_edServerIP);
	DDX_Control(pDX, ID_ED_SERVERPORT, m_edServerPost);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJCY);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbMessage); 
}

BEGIN_MESSAGE_MAP(CSXDRDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CSXDRDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CSXDRDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CSXDRDlg::OnBnClickedBtnUpinter)
END_MESSAGE_MAP()


// CSXDRDlg ��Ϣ�������

BOOL CSXDRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	SetConfig();
	SetVehCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSXDRDlg::OnPaint()
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
HCURSOR CSXDRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CSXDRDlg::SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv)
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


void CSXDRDlg::SetConfig(void)
{
	// ��ȡexe(dll)�ļ�����·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"NSConfig.ini", wchPath))
	{
	}

	CSimpleIni si(wchPath);

	CString strTemp;

	strTemp = si.GetString(L"NSServer", L"IP", L"");
	m_edServerIP.SetWindowTextW(strTemp);

	strTemp = si.GetString(L"NSServer", L"Port", L"");
	m_edServerPost.SetWindowTextW(strTemp);

	strTemp = si.GetString(L"NSServer", L"DeviceSN", L"");
	m_edLineNumber.SetWindowTextW(strTemp);

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

	CFont font;
	font.CreatePointFont(320,L"����");
	m_lbMessage.SetFont(&font);

	m_lbMessage.SetWindowTextW(L"��ȡ�����ļ��ɹ�");
}

void CSXDRDlg::SetVehCtrl(void)
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

	m_lbMessage.SetWindowTextW(L"�б����óɹ�");
}

bool CSXDRDlg::GetVehList(void)
{
	CString strDeviceSN;
	m_edLineNumber.GetWindowTextW(strDeviceSN);

	std::list<SOBDVEHLIST> lsOBDVehList;

	// ��ʾ�б�
	m_lstVehicle.DeleteAllItems();

	if (!strDeviceSN.IsEmpty())
	{
		// ��ֹ������°�ť
		GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);

		CString strXML;
		strXML.AppendFormat(L"<Message Device=\"%s\">", strDeviceSN);
		strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDVehicleList");
		strXML.AppendFormat(L"</Request>");
		strXML.AppendFormat(L"</Message>");


		std::wstring strRecv;
		bool bRet = SendAndRecvPacket(strXML, strRecv);

		// ���������ӿں󣬿��Ե������
		GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);

#ifdef _DEBUG
		bRet = true;
		CFile pFile;
		byte * pbEnData(NULL);
		UINT unDataLen(0);
		if (pFile.Open(L"C:\\Users\\Administrator\\Desktop\\ɽ���ٷ�\\�½��ı��ĵ� (2).txt", CFile::modeRead|CFile::shareDenyNone))
		{
			unDataLen = (UINT)pFile.GetLength();
			pbEnData = new byte[unDataLen]();
			pFile.Read(pbEnData, unDataLen);
			pFile.Close();
		}
		CString strCS = KANSIToUTF16((char*) pbEnData);
		strRecv = strCS.GetString();
		if (NULL != pbEnData)
		{
			delete[] pbEnData;
			pbEnData = NULL;
		}
#endif
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
			else
			{
				m_lbMessage.SetWindowTextW(L"����ʧ��");
				return false;
			}
		}
		else
		{
			CloaseScket();
			m_lbMessage.SetWindowTextW(strRecv.c_str());
			return false;
		}
	}
	else
	{
		CloaseScket();
		m_lbMessage.SetWindowTextW(L"����ߺ�Ϊ��");
		return false;
	}

	// ��ʾ�б�
	m_lstVehicle.DeleteAllItems();
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

	
	CloaseScket();
	return true;
}

void CSXDRDlg::CloaseScket(void)
{
	// �˳��ǶϿ�����
	CNSSocket nsSocket;
	nsSocket.Close();
}

void CSXDRDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!GetVehList())
	{
		CString str;
		m_lbMessage.GetWindowTextW(str);
		str.AppendFormat(L"\r\n��ȡʧ��");
		m_lbMessage.SetWindowTextW(str);
	}
}


void CSXDRDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		m_sOBDVehList = SOBDVEHLIST();
		CString strTemp(L"");

		strTemp = m_lstVehicle.GetItemText(nIndex,1);
		m_sOBDVehList.strRunningNumber = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,2);
		m_sOBDVehList.strPaletNumber = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,3);
		m_sOBDVehList.strPlateType = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,4);
		m_sOBDVehList.strVIN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,5);
		m_sOBDVehList.strVehType = strTemp.GetString();

		// ѡ�г��ƺ���Ե��UP
		GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
	}

	EndWaitCursor();

	*pResult = 0;
}


void CSXDRDlg::OnBnClickedBtnUpinter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString cstrOBDJCY(L"");
	m_edOBDJCY.GetWindowTextW(cstrOBDJCY);

	CString strDeviceSN;
	m_edLineNumber.GetWindowTextW(strDeviceSN);

	if (!cstrOBDJCY.IsEmpty() && !strDeviceSN.IsEmpty())
	{
		COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 40);
		COleDateTime odtSartTime = odtCurTime - odts;

		CString strXML;
		CString strVehType = m_sOBDVehList.strVehType.c_str();
		if (strVehType == L"1") // ����
		{
			strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, m_sOBDVehList.strRunningNumber.c_str());
			strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadDieselResult");
			strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");

			strXML.AppendFormat(L"<Result><Row><Info>");
			strXML.AppendFormat(L"<VIN>%s</VIN>",m_sOBDVehList.strVIN.c_str());
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
		else
		{
			strXML.AppendFormat(L"<Message Device=\"%s\" OutlookID=\"%s\">", strDeviceSN, m_sOBDVehList.strRunningNumber.c_str());
			strXML.AppendFormat(L"<Request Name=\"%s\">", L"OBDUploadGasResult");
			strXML.AppendFormat(L"<CLWGJC><Row></Row></CLWGJC><DQBZ>3</DQBZ>");

			strXML.AppendFormat(L"<Result><Row><Info>");
			strXML.AppendFormat(L"<VIN>%s</VIN>",m_sOBDVehList.strVIN.c_str());
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
				}
				if (xmlReader.OpenNode(L"Message/Respond/Row/ErrorMessage"))
				{
					xmlReader.GetNodeContent(strMsg);
				}
				if (strResult == L"1")
				{
					m_lbMessage.SetWindowTextW(L"�ϴ��ɹ�");
				}
				else
				{
					CString cstrMsg;
					cstrMsg.Format(L"�ϴ�ʧ��\r\n%s", strMsg.c_str());
					m_lbMessage.SetWindowTextW(cstrMsg);
				}
			}
		}
		else
		{
			m_lbMessage.SetWindowTextW(L"����ʧ��");
		}

		GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

		CloaseScket();
	}
	else
	{
		m_lbMessage.SetWindowTextW(L"�����ԱΪ��\r\n�����Ϊ��");
	}

}
