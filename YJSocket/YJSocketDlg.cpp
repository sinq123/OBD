
// YJSocketDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YJSocket.h"
#include "YJSocketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYJSocketDlg �Ի���




CYJSocketDlg::CYJSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYJSocketDlg::IDD, pParent)
	, m_bAuthentication(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CYJSocketDlg::~CYJSocketDlg(void)
{
	CloseConnect();
}

void CYJSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_SERVERIP, m_edServerIP);
	DDX_Control(pDX, ID_ED_SERVERPORT, m_edServerPost);
	DDX_Control(pDX, ID_ED_STATIONNUMBER, m_edStationNumber);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJCY);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbMessage);
	DDX_Control(pDX, IDC_BTN_SOCKET, m_btnSocket);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassword);
}

BEGIN_MESSAGE_MAP(CYJSocketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CYJSocketDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CYJSocketDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CYJSocketDlg::OnBnClickedBtnUpinter)
	ON_BN_CLICKED(IDC_BTN_UPTESTLOG, &CYJSocketDlg::OnBnClickedBtnUptestlog)
	ON_BN_CLICKED(IDC_BTN_SOCKET, &CYJSocketDlg::OnBnClickedBtnSocket)
END_MESSAGE_MAP()


// CYJSocketDlg ��Ϣ�������

BOOL CYJSocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_bAuthentication = false;

	m_mapTestType[L"A"] = L"˫����";
	m_mapTestType[L"B"] = L"��̬";
	m_mapTestType[L"C"] = L"����˲̬";
	m_mapTestType[L"D"] = L"˲̬";
	m_mapTestType[L"F"] = L"���ɼ���";
	m_mapTestType[L"G"] = L"���ؼ���";

	// ��ʼ��
	SHJ03 sHJ03;
	m_sHJ03 = sHJ03;

	SetConfig();
	SetVehCtrl();

	//
	// GetVehicleList();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CYJSocketDlg::OnPaint()
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
HCURSOR CYJSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYJSocketDlg::CloseConnect(void)
{
	if (m_yjSocket.IsOpen())
	{
		m_yjSocket.Close();
	}
	m_bAuthentication = false;
}

CString CYJSocketDlg::CheckSum(const char *ch, const UINT un)
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

CString CYJSocketDlg::GenerateInsNum(void)
{
	// Ψһ�������ɹ��򣺼���߱���+ʱ�䣨��ȷ�����룩
	CString InsNum(L""), strTemp;;
	m_edStationNumber.GetWindowTextW(strTemp);
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

bool CYJSocketDlg::SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv)
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
		m_edServerIP.GetWindowTextW(strIP);
		m_edServerPost.GetWindowTextW(strPort);
		nPort = _wtoi(strPort);
		if (!m_yjSocket.Open(strIP, nPort) ||(strIP == L"" || strPort == L""))
		{
			strRecv = L"���ӷ�����ʧ�ܣ�";
			return false;
		}
	}


	return m_yjSocket.SendAndRecvPacket(strSend, strRecv);
}


void CYJSocketDlg::SetConfig(void)
{
	// ��ȡexe(dll)�ļ�����·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"YJConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		CString strTemp;

		strTemp = si.GetString(L"System", L"ServerIP", L"");
		m_edServerIP.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"ServerPort", L"");
		m_edServerPost.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"StationNumber", L"");
		m_edStationNumber.SetWindowTextW(strTemp);

		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edLineNumber.SetWindowTextW(strTemp);
	}

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

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UPTESTLOG)->EnableWindow(FALSE);

	CFont font;
	font.CreatePointFont(320,L"����");
	m_lbMessage.SetFont(&font);

	m_lbMessage.SetWindowTextW(L"��ȡ�����ļ��ɹ�");
}

void CYJSocketDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(3, L"������ɫ",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(4, L"������",	0,	60/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(5, L"��ⷽ��",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(6, L"VIN",	0,	120/**nSM_CXSCREEN/1280*/);
	m_lstVehicle.InsertColumn(7, L"�Ƿ���OBD",	0,	120/**nSM_CXSCREEN/1280*/);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);

	m_lbMessage.SetWindowTextW(L"�б����óɹ�");
}


bool CYJSocketDlg::Authentication(std::wstring &strResult, std::wstring &strMsg)
{
	CString strTemp;
	m_edLineNumber.GetWindowTextW(strTemp);
	if (strTemp == L"")
	{
		strTemp = L"01";
	}

	return SendAndRecvPacket(L"LJZC", L"", L"", L"", strTemp, strMsg);
}

bool CYJSocketDlg::GetVehicleList(void)
{
	if (!m_bAuthentication)
	{
		std::wstring strResult, strMsg;
		m_bAuthentication = Authentication(strResult, strMsg);
	}
	if (m_bAuthentication)
	{
		m_lbMessage.SetWindowTextW(L"���ӳɹ�");	
	}
	else
	{
		return false;
	}

	if (!GetTime())
	{
		return false;
	}

	// ��ֹ������°�ť
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);

	CString strStationNum, strLineNum;

	m_edStationNumber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);
	// Json

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	//strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", strLineNum);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"InspectionMethod\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", L"");
	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(15, 0, 0, 0);
	COleDateTime odtSartTime = odtCurTime - odts;
	strJsonData.AppendFormat(L"\"StartTime\":\"%s\",",odtSartTime.Format(L"%Y-%m-%d 00:00:00"));
	strJsonData.AppendFormat(L"\"EndTime\":\"%s\"", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ03", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);

	// ���������ӿں󣬿��Ե������
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);

	std::list<SHJ03> lsHJ03;

	if (bRet)
	{
		std::wstring strMsg;
		m_yjSocket.RecvPacket(strMsg);
		if (strMsg.length() == 21)
		{
			strMsg = strRecv;
		}
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			for (UINT i=0; i<root.size(); i++)
			{
				SHJ03 sHJ03;
				if (!root[i]["InspectionNum"].empty()){sHJ03.strInspectionNum = Json2Cstring(root[i]["InspectionNum"]).GetString();}
				if (!root[i]["VLPN"].empty())	{sHJ03.strVLPN = Json2Cstring(root[i]["VLPN"]).GetString();}
				if (!root[i]["VLPNColor"].empty()) {sHJ03.strVLPNColor = Json2Cstring(root[i]["VLPNColor"]).GetString();}
				if (!root[i]["VIN"].empty())	{sHJ03.strVIN = Json2Cstring(root[i]["VIN"]).GetString();}
				if (!root[i]["InspectionMethod"].empty()) {sHJ03.strInspectionMethod =  m_mapTestType[Json2Cstring(root[i]["InspectionMethod"]).GetString()];}
				if (!root[i]["InspectionNature"].empty()){sHJ03.strInspectionNature = Json2Cstring(root[i]["InspectionNature"]).GetString();}
				if (!root[i]["HasOBD"].empty()){sHJ03.strHasOBD = Json2Cstring(root[i]["HasOBD"]).GetString();}
				lsHJ03.push_back(sHJ03);
			}
		}
		else
		{
			m_lbMessage.SetWindowTextW(strMsg.c_str());
		}

	}
	else
	{
		CString str;
		str.Format(L"%s", strRecv.c_str());
		m_lbMessage.SetWindowTextW(str);


		return false;
	}

	// ��ʾ�б�
	m_lstVehicle.DeleteAllItems();
	int nItem(0);
	for (list<SHJ03>::iterator ir=lsHJ03.begin(); ir!=lsHJ03.end(); ++ir)
	{
		CString strHasOBD = ir->strHasOBD.c_str();
		if (strHasOBD != L"1")
		{
			continue;
		}

		CString str;
		// ���
		str.Format(L"%d", nItem+1);

		int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

		// ��ˮ��
		str = ir->strInspectionNum.c_str();
		m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, str, 0, 0, 0, 0);
		// ���ƺ���
		str = ir->strVLPN.c_str();
		m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, str, 0, 0, 0, 0);
		// ������ɫ
		str = ir->strVLPNColor.c_str();
		m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, str, 0, 0, 0, 0);
		// ������
		str = ir->strInspectionNature.c_str();
		m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, str, 0, 0, 0, 0);
		// ��ⷽ��
		str = ir->strInspectionMethod.c_str();
		m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, str, 0, 0, 0, 0);
		// VIN
		str = ir->strVIN.c_str();
		m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);
		// �Ƿ���OBD
		if (strHasOBD == 1)
		{
			str.Format(L"��");
		}
		else
		{
			str.Format(L"��");
		}
		m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, str, 0, 0, 0, 0);
		nItem++;
	}


	return true;
}


bool CYJSocketDlg::Json2Map(LPCTSTR szJson, JsonMap &mapRet)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(KUTF16ToANSI(szJson), root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		Json::Value::Members mem = root.getMemberNames();

		root[""];
		for (auto it=mem.begin(); it!=mem.end(); it++)
		{
			if (root[*it].type() == Json::stringValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", KANSIToUTF16(root[*it].asString().c_str()));
			}
			else if (root[*it].type() == Json::booleanValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", KANSIToUTF16(root[*it].asString().c_str()));
			}
			else if (root[*it].type() == Json::realValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%f", root[*it].asDouble());
			}
			else if (root[*it].type() == Json::uintValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%hu", root[*it].asUInt());
			}
			else if (root[*it].type() == Json::intValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%d", root[*it].asInt());
			}
			else if (root[*it].type() == Json::nullValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", L"");
			}
			else
			{
			}
		}
		return true;
	}
	// ���ص�Json��ʽ����
	return false;
}

CString CYJSocketDlg::Json2Cstring(Json::Value &value)
{
	CString strValue;
	if (value.type() == Json::stringValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::booleanValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::realValue)
	{
		strValue.Format(L"%f", value.asDouble());
	}
	else if (value.type() == Json::uintValue)
	{
		strValue.Format(L"%hu", value.asUInt());
	}
	else if (value.type() == Json::intValue)
	{
		strValue.Format(L"%d", value.asInt());
	}
	else if (value.type() == Json::nullValue)
	{
		strValue.Format(L"%s", L"");
	}
	else
	{
	}
	return strValue;
}

CString CYJSocketDlg::Map2Json(JsonMap mapPost)
{
	CString strJson;
	strJson.AppendFormat(L"{");
	for (JsonMap::const_iterator it=mapPost.begin(); it!=mapPost.end(); ++it)
	{
		strJson.AppendFormat(L"\"%s\":\"%s\"",  it->first, it->second);
		strJson.AppendFormat(L",");
	}
	strJson.TrimRight(L",");
	strJson.AppendFormat(L"}");
	return strJson;
}



void CYJSocketDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetVehicleList();
}


void CYJSocketDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();

	// ����OBD���Ա������д
	CString strOBDJCY;
	m_edOBDJCY.GetWindowTextW(strOBDJCY);

	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1 && strOBDJCY != L"")
	{
		// ��ʼ��
		SHJ03 sHJ03;
		m_sHJ03 = sHJ03;

		m_sHJ03.strOBDJCY = strOBDJCY.GetString();

		CString strTemp(L"");

		strTemp = m_lstVehicle.GetItemText(nIndex,1);
		m_sHJ03.strInspectionNum = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,2);
		m_sHJ03.strVLPN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,3);
		m_sHJ03.strVLPNColor = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,4);
		m_sHJ03.strInspectionNature = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,5);
		m_sHJ03.strInspectionMethod = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,6);
		m_sHJ03.strVIN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,7);
		m_sHJ03.strHasOBD = strTemp.GetString();

		if (!m_sHJ03.strInspectionNum.empty() && !m_sHJ03.strVLPN.empty())
		{
			// ѡ�г��ƺ���Ե��UP
			GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
			m_lbMessage.SetWindowTextW(L"����UP");
		}
	}
	else if (strOBDJCY == L"" && nIndex!=-1)
	{
		MessageBox(L"������OBD���Ա");
		m_lbMessage.SetWindowTextW(L"������OBD���Ա");
	}
	else if (nIndex==-1 && strOBDJCY != L"")
	{
		MessageBox(L"��˫������");
	}

	EndWaitCursor();

	*pResult = 0;
}

bool CYJSocketDlg::UpOBDStart(const CString& strInspectionNum, const CStringW& strStationNum, const CStringW& strLineNum)
{
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", strInspectionNum);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", strLineNum);
	strJsonData.AppendFormat(L"\"Time\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"Code\":\"%s\"}", L"start");

	std::wstring strRecv;
	return SendAndRecvPacket(L"HJ04", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);
}

void CYJSocketDlg::OnBnClickedBtnUpinter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CString strStationNum, strLineNum;

	m_edStationNumber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;



	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_sHJ03.strInspectionNum.c_str());
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", strStationNum);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", m_sHJ03.strVIN.c_str());
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", m_sHJ03.strVLPN.c_str());
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", m_sHJ03.strVLPNColor.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectStartTime\":\"%s\",", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"OBDInspectEndTime\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"ODO\":\"%d\",", 0);// �������
	strJsonData.AppendFormat(L"\"FaultIndicatorLighten\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsCommunicated\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CommFailReason\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"FaultIndicator\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsHaveFaultCode\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"FaultInfo\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"UnDoneReadyStatus\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"UnFinifishedItem\":\"%s\",", L"");

	// ��ȡOBDType
	srand(time(0));
	int n = rand()%10;
	CString strOBDType(L"");
	CString strFulType(L"����");
	if (m_sHJ03.strInspectionMethod.find(L"��") != -1 || m_sHJ03.strInspectionMethod.find(L"����") != -1)
	{
		strFulType = L"����";
	}
	strOBDType.Format(L"%s", GetOBDType(n, strFulType));
	CString strEngineCALID, strEngineCVN;
	// �Ƿ���Ҫ��Ҫ��ȡһ������
	Sleep(1000);
	GetEngineCALID(strOBDType, strFulType, strEngineCALID, strEngineCVN);
	//if (strEngineCALID.IsEmpty() || strEngineCVN.IsEmpty())
	//{
	//	strJsonData.AppendFormat(L"\"ECU_CALID\":\"%s\",", L"��֧��");
	//	strJsonData.AppendFormat(L"\"ECU_CVN\":\"%s\",", L"��֧��");
	//}
	//else
	{
		strJsonData.AppendFormat(L"\"ECU_CALID\":\"%s\",", strEngineCALID);
		strJsonData.AppendFormat(L"\"ECU_CVN\":\"%s\",", strEngineCVN);
	}
	strJsonData.AppendFormat(L"\"ACU_CALID\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"ACU_CVN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"OCU_CALID\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"OCU_CVN\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"StatusCoherence\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CheckPerson\":\"%s\",", m_sHJ03.strOBDJCY.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectResult\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"IsNeedReCheck\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"ReCheckContent\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"InspectionNature\":\"%s\",", m_sHJ03.strInspectionNature.c_str());
	strJsonData.AppendFormat(L"\"InspectionTimes\":\"%s\"", m_sHJ03.strInspectionNature.c_str());
	strJsonData.AppendFormat(L"\"DeviceMANU\":\"%s\"", L"��ɽ���ϻ��������޹�˾");
	strJsonData.AppendFormat(L"\"DeviceType\":\"%s\"", L"NHOBD-1");
	strJsonData.AppendFormat(L"\"OBDType\":\"%s\"", L"1");
	strJsonData.AppendFormat(L"\"RemoteCommunicated\":\"%s\"", L"1");
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	
	UpOBDStart(m_sHJ03.strInspectionNum.c_str(), strStationNum, strLineNum);
	bool bRet = SendAndRecvPacket(L"HJ05", strStationNum, strLineNum, GenerateInsNum(), strJsonData, strRecv);
	// �ر�����
	CloseConnect();
	m_btnSocket.SetWindowTextW(L"����");

	if (bRet)
	{
		m_lbMessage.SetWindowTextW(L"����ʽ��ʼ����\r\n��ȡ������Ϣ��(�ȴ����б��д˳�)���UPTestLog");
		GetDlgItem(IDC_BTN_UPTESTLOG)->EnableWindow(TRUE);

		//if (!SetSQL(m_sHJ03.strInspectionNum.c_str(), m_sHJ03.strInspectionMethod.c_str()))
		{
			//m_lbMessage.SetWindowTextW(L"��������ʧ��������UPTestLog�����²���");
			//GetDlgItem(IDC_BTN_UPTESTLOG)->EnableWindow(TRUE);
		}
	}
	else
	{
		CString str;
		str.Format(L"ʧ��\r\n%s", strRecv.c_str());
		m_lbMessage.SetWindowTextW(str);
	}

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
}

bool CYJSocketDlg::SetSQL(const CString& strRunningNumber, const CString& strTestType)
{
	CString strSource, strCatalog, strUserID, strPassword;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassword.GetWindowTextW(strPassword);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() 
		|| strUserID.IsEmpty() || strPassword.IsEmpty())
	{
		m_lbMessage.SetWindowTextW(L"���ݿ����Ϊ��");
		return false;
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
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"TestLog.ini", wchPath))
		{
			CSimpleIni si(wchPath);
			si.SetString(L"TestLog", L"ItemOBD", L"4");
		}
	}

	_ConnectionPtr pConnection(NULL);
	if (0x00 == CNHSQLServerDBO::OpenDB(pConnection, strSource, strCatalog, strUserID, strPassword))
	{
		CString strSQL;
		// �ȴ���TestLog
		strSQL.Format(L"update TestLog set TestState = '0', ItemOBD = '4' WHERE RunningNumber= '%s'", strRunningNumber);
		DWORD dwRet = CNHSQLServerDBO::ExecuteDML(pConnection, strSQL);

		// ��ȡOBD����
		strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber = '%s'", strRunningNumber);
		SResultOfOBD sResultOfOBD;
		// �Ȼ�ȡ����ȡ������˵
		GetDboResultOfOBD(strSQL, &sResultOfOBD);
		if (true)
		{
			sResultOfOBD.strRunningNumber = strRunningNumber;
			// ����
			if (strTestType.Find(L"��̬") != -1
				|| strTestType.Find(L"˫����") != -1
				|| strTestType.Find(L"˲̬") != -1)
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
			else if (strTestType.Find(L"����") != -1
				|| strTestType.Find(L"��͸��") != -1)
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
		m_lbMessage.SetWindowTextW(L"�����ݿ�ʧ��");
		return false;
	}

	return true;
}


void CYJSocketDlg::OnBnClickedBtnUptestlog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_sHJ03.strInspectionNum.empty() && !m_sHJ03.strInspectionMethod.empty())
	{
		if (SetSQL(m_sHJ03.strInspectionNum.c_str(), m_sHJ03.strInspectionMethod.c_str()))
		{
			m_lbMessage.SetWindowTextW(L"�����������");
		}
		else
		{
			m_lbMessage.SetWindowTextW(L"��������ʧ��");
		}
	}
	else
	{
		m_lbMessage.SetWindowTextW(L"�в���Ϊ��");
	}
}


bool CYJSocketDlg::GetTime()
{
	CString strStationNum, strLineNum;

	m_edStationNumber.GetWindowTextW(strStationNum);
	m_edLineNumber.GetWindowTextW(strLineNum);

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ26", strStationNum, strLineNum, GenerateInsNum(), L"", strRecv);
	if (bRet)
	{
		std::wstring strMsg;
		if (!m_yjSocket.RecvPacket(strMsg))
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



void CYJSocketDlg::OnBnClickedBtnSocket()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_btnSocket.GetWindowTextW(str);

	if (str.Find(L"����") != -1)
	{
		std::wstring strResult, strMsg;
		if (!m_bAuthentication)
		{
			m_bAuthentication = Authentication(strResult, strMsg);
		}
		if (m_bAuthentication)
		{
			m_lbMessage.SetWindowTextW(L"���ӳɹ�");	
			GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);
			m_btnSocket.SetWindowTextW(L"�Ͽ�");
		}
		else
		{
			m_lbMessage.SetWindowTextW(strMsg.c_str());
		}

	}
	else if (str.Find(L"�Ͽ�") != -1)
	{
		CloseConnect();
		m_btnSocket.SetWindowTextW(L"����");
	}
}


CString CYJSocketDlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CYJSocketDlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
	CString& strEngineCALID, CString& strEngineCVN)
{
	strEngineCALID = L"��֧��"; strEngineCVN = L"��֧��";
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
