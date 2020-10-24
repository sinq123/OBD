
// SYDATADlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SYDATA.h"
#include "SYDATADlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSYDATADlg �Ի���




CSYDATADlg::CSYDATADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSYDATADlg::IDD, pParent)
	, m_pConnection(NULL)
	, m_bIsOKOpenDB(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSYDATADlg::~CSYDATADlg()
{
	// �˳��ر����ݿ�����
	Close();
}

void CSYDATADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_VEHINFOPATH, m_edVehInfoPath);
	DDX_Control(pDX, IDC_ED_MESSAGE, m_edMsg);

	DDX_Control(pDX, IDC_ST_TSNO, m_lbTsNO);
	DDX_Control(pDX, IDC_ST_HP, m_lbHP);
	DDX_Control(pDX, IDC_ST_CPYS, m_lbCpys);
	DDX_Control(pDX, IDC_ST_RLZL, m_lbRlzl);
	DDX_Control(pDX, IDC_ST_Jqfs, m_lbJqfs);
	DDX_Control(pDX, IDC_ST_TESTOBD, m_lbTestOBD);

	DDX_Control(pDX, ID_ED_Catalog, m_edCatalog);
	DDX_Control(pDX, ID_ED_Source, m_edSource);
	DDX_Control(pDX, ID_ED_UserID, m_edUserID);
	DDX_Control(pDX, ID_ED_Password, m_edPassWord);
	DDX_Control(pDX, IDC_COM_OBDJCY, m_comOBDJCY);

}

BEGIN_MESSAGE_MAP(CSYDATADlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CSYDATADlg::OnBnClickedButtonUpdateVehicleList)
	ON_BN_CLICKED(IDC_BTN_JUMPTESTOBD, &CSYDATADlg::OnBnClickedBtnJumptestobd)
	ON_BN_CLICKED(IDC_BTN_UP, &CSYDATADlg::OnBnClickedBtnUp)
END_MESSAGE_MAP()


// CSYDATADlg ��Ϣ�������

BOOL CSYDATADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BTN_JUMPTESTOBD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	
	SetConfig();
	m_bIsOKOpenDB = OpenSQL();
	GetOBDJCY();
	// ���ݿ��ʧ�ܲ��������
	if (!m_bIsOKOpenDB)
	{
		GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSYDATADlg::OnPaint()
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
HCURSOR CSYDATADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSYDATADlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetVehXml();
}


void CSYDATADlg::OnBnClickedBtnJumptestobd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	JumpTestOBD();
}


void CSYDATADlg::OnBnClickedBtnUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WriterResultOfOBDAndTestLog();
}



void CSYDATADlg::SetConfig(void)
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

	m_edVehInfoPath.SetWindowTextW(L"C:\\JcdataTxt");
}

bool CSYDATADlg::OpenSQL(void)
{
	CString strSource, strCatalog, strUserID, strPassWord;
	m_edSource.GetWindowTextW(strSource);
	m_edCatalog.GetWindowTextW(strCatalog);
	m_edUserID.GetWindowTextW(strUserID);
	m_edPassWord.GetWindowTextW(strPassWord);

	if (strSource.IsEmpty() || strCatalog.IsEmpty() || strUserID.IsEmpty() || strPassWord.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"���ݿ����ò����п�ֵ");
		return false;
	}

	if (0x00 != CNHSQLServerDBO::OpenDB(m_pConnection, strSource, strCatalog, strUserID, strPassWord))
	{
		m_edMsg.SetWindowTextW(L"���ݿ���ϵ����");
		return false;
	}

	return true;
}

bool CSYDATADlg::Close(void)
{
	// �ر����ݿ�
	if (m_pConnection != NULL && m_bIsOKOpenDB)
	{
		CNHSQLServerDBO::CloseDB(m_pConnection);
	}
	return true;
}


void CSYDATADlg::GetOBDJCY(void)
{
	if (!m_bIsOKOpenDB)
	{
		return;
	}
	CString strSQL;
	strSQL.Format(L"select * from UserInfo /*where Role = 6*/");
	_RecordsetPtr pRecordset(NULL);
	m_comOBDJCY.ResetContent();
	if (0x00 == CNHSQLServerDBO::OpenQuery(m_pConnection, pRecordset, strSQL))
	{
		// �򿪲�ѯ�ɹ�
		try
		{
			m_comOBDJCY.AddString(L"");
			while (!pRecordset->adoEOF)
			{
				_variant_t v;
				v.ChangeType(VT_NULL);
				CNHSQLServerDBO::GetFieldValue(pRecordset, v, L"Name");
				if (VT_NULL!=v.vt && VT_EMPTY!=v.vt)
				{
					m_comOBDJCY.AddString((wchar_t*)(_bstr_t)v);
				}
				else
				{
					m_comOBDJCY.AddString(L"");
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

void CSYDATADlg::GetVehXml(void)
{
	// ��������ʼ��Ϊ���ɰ�
	GetDlgItem(IDC_BTN_JUMPTESTOBD)->EnableWindow(FALSE);

	// ������������Ϣ��Ϊ��
	SetVehWin(SCarInfo());

	// ��ȡ�ļ�·��
	CString strVehInfoPath;
	CString strMsg;
	
	if (!GetFilePath(strMsg, strVehInfoPath))
	{
		m_edMsg.SetWindowTextW(strMsg);
		return;
	}

	// ��ȡ�ļ�����
	wstring wstrCarInfo;
	wstrCarInfo = FilePathToContent(strVehInfoPath);

	SCarInfo sCarInfo;
	CXmlReader xmlReader;
	if (xmlReader.Parse(wstrCarInfo.c_str()))
	{
		wstring strCon;
		// 1	����������	TsNo
		if (xmlReader.OpenNode(L"WQ/CARINFO/TsNo"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strTsNo = strCon.c_str();
		}
		//2	����߱��	TestLineNo
		if (xmlReader.OpenNode(L"WQ/CARINFO/TestLineNo"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strTestLineNo = strCon.c_str();
		}
		//3	����ID	Clid
		if (xmlReader.OpenNode(L"WQ/CARINFO/Clid"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strClid = strCon.c_str();
		}
		//4	����	Hp
		if (xmlReader.OpenNode(L"WQ/CARINFO/Hp"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strHp = strCon.c_str();
		}
		//5	������ɫ	Cpys
		if (xmlReader.OpenNode(L"WQ/CARINFO/Cpys"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strCpys = strCon.c_str();
		}
		//6	��������ʽ	Bxqxs
		if (xmlReader.OpenNode(L"WQ/CARINFO/Bxqxs"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strBxqxs = strCon.c_str();
		}
		//7	��׼����	Jzzl
		if (xmlReader.OpenNode(L"WQ/CARINFO/Jzzl"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strJzzl = strCon.c_str();
		}
		//8	ȼ������	Rlzl
		if (xmlReader.OpenNode(L"WQ/CARINFO/Rlzl"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strRlzl = strCon.c_str();
		}
		//9	�����	Edgl
		if (xmlReader.OpenNode(L"WQ/CARINFO/Edgl"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strEdgl = strCon.c_str();
		}
		//10	�ת��	Edzs
		if (xmlReader.OpenNode(L"WQ/CARINFO/Edzs"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strEdzs = strCon.c_str();
		}
		//11	���	Cc
		if (xmlReader.OpenNode(L"WQ/CARINFO/Cc"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strCc = strCon.c_str();
		}
		//12	C0+Co2��ֵ	CoCo2
		if (xmlReader.OpenNode(L"WQ/CARINFO/CoCo2"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strCoCo2 = strCon.c_str();
		}
		//13	��������	Lxcz
		if (xmlReader.OpenNode(L"WQ/CARINFO/Lxcz"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strLxcz = strCon.c_str();
		}
		//14	�ۼƳ���	Ljcz
		if (xmlReader.OpenNode(L"WQ/CARINFO/Ljcz"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strLjcz = strCon.c_str();
		}
		//15	������ʽ	Jqfs
		if (xmlReader.OpenNode(L"WQ/CARINFO/Jqfs"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strJqfs = strCon.c_str();
		}
		//16	�Ƿ�OBD���	OBD
		if (xmlReader.OpenNode(L"WQ/CARINFO/OBD"))
		{
			xmlReader.GetNodeContent(strCon);
			sCarInfo.strOBD = strCon.c_str();
		}
	}
	else
	{
		m_edMsg.SetWindowTextW(L"��xmlʧ��");
		return;
	}

	if (m_sCarInfo.strHp.Find(sCarInfo.strHp) == std::string::npos)
	{
		m_sCarInfo = sCarInfo;
	}
	else
	{
		if (IDYES != MessageBox(L"��ȡ��¼���ϴβ����ĳ��ƺ���һ�£��Ƿ��������", L"", MB_YESNO))
		{
			m_edMsg.SetWindowTextW(L"��ȡ��¼���ϴβ����ĳ��ƺ���һ��");
			return;
		}
		m_sCarInfo = sCarInfo;
	}

	SetVehWin(m_sCarInfo);

	// ������OBD��ⰴť��Ϊ�ɰ���ͬʱ�������б���Ϊ���ɰ�
	//GetDlgItem(IDC_BTN_JUMPTESTOBD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);
}

bool CSYDATADlg::GetFilePath(CString& strMsg, CString& strFilePath)
{
	CString strVehInfoPath;
	m_edVehInfoPath.GetWindowTextW(strVehInfoPath);

	if (strVehInfoPath.IsEmpty())
	{
		strMsg = (L"�ļ�·��Ϊ��");
		return false;
	}

	strVehInfoPath += L"\\CarInfo.Xml";

	if (_waccess(strVehInfoPath, 00))
	{
		strMsg = (L"CarInfo.Xml�ļ�������");
		return false;
	}

	strFilePath = strVehInfoPath;
	return true;
}

wstring CSYDATADlg::FilePathToContent(const CString strPath)
{
	wstring strContent;

	CFile pFile;
	if (pFile.Open(strPath, CFile::modeRead|CFile::shareDenyNone))
	{
		byte * pbEnData(NULL);
		UINT unDataLen(0);

		unDataLen = (UINT)pFile.GetLength();
		pbEnData = new byte[unDataLen]();
		pFile.Read(pbEnData, unDataLen);

		pFile.Close();

		CString strEndata(KANSIToUTF16((char*)pbEnData));
		strContent = strEndata.GetString();

		if (NULL != pbEnData)
		{
			delete[] pbEnData;
			pbEnData = NULL;
		}
	}

	return strContent;
}


void CSYDATADlg::SetVehWin(const SCarInfo& sCarInfo)
{
	m_edMsg.SetWindowTextW(L"�趨������Ϣ����");
	if (!m_sCarInfo.strTsNo.IsEmpty())
	{
		m_lbTsNO.SetWindowTextW(sCarInfo.strTsNo);
	}

	if (!m_sCarInfo.strHp.IsEmpty())
	{
		m_lbHP.SetWindowTextW(sCarInfo.strHp);
	}

	if (!m_sCarInfo.strCpys.IsEmpty())
	{
		m_lbCpys.SetWindowTextW(sCarInfo.strCpys);
	}

	if (!m_sCarInfo.strRlzl.IsEmpty())
	{
		m_lbRlzl.SetWindowTextW(sCarInfo.strRlzl);
	}

	if (!m_sCarInfo.strJqfs.IsEmpty())
	{
		m_lbJqfs.SetWindowTextW(sCarInfo.strJqfs);
	}

	if (!m_sCarInfo.strOBD.IsEmpty())
	{
		m_lbTestOBD.SetWindowTextW(sCarInfo.strOBD);
	}
}

void CSYDATADlg::JumpTestOBD(void)
{
	// ��ȡ�ļ�·��
	CString strVehInfoPath;
	CString strMsg;
	
	if (!GetFilePath(strMsg, strVehInfoPath))
	{
		m_edMsg.SetWindowTextW(strMsg);
		return;
	}

	// �Ƿ���Ҫ��ȡ�ļ����ݣ���
	// ��ʱ�趨����Ҫ

	CXmlWriter xmlWriter;

	if (xmlWriter.LoadFile(strVehInfoPath))
	{
		//16	�Ƿ�OBD���	OBD
		if (xmlWriter.OpenNode(L"WQ/CARINFO/OBD"))
		{
			xmlWriter.SetNodeContent(L"N");
		}
	}
	else
	{
		m_edMsg.SetWindowTextW(L"��xmlʧ��");
		return;
	}

	// ���º�д�뱣����Ϣ
	if (xmlWriter.WriteFile(strVehInfoPath))
	{
		strMsg.Format(L"����ɹ�\r\n");
	}
	else
	{
		m_edMsg.SetWindowTextW(L"����ʧ��");
		return;
	}

	// ��ʾ��һ������
	strMsg.AppendFormat(L" ����ȷ�Ͻ��������\r\n ֮���ڷ��ؼ�������");
	m_edMsg.SetWindowTextW(strMsg);

	// ��ʼ��������������Ϊ���ɰ�
	GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_JUMPTESTOBD)->EnableWindow(FALSE);
}

void CSYDATADlg::WriterResultOfOBDAndTestLog(void)
{
	// �Ƿ����������ȡ����
	TESTLOG sTestLog;
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM TestLog where PlateNumber = '%s' ORDER BY AutoID DESC LIMIT 1"), m_sCarInfo.strHp);
	GetDboTestLog(strSQL, &sTestLog);
	
	// ��������־���Ϊ��ʱ��ȡApp_data
	if (wcscmp(sTestLog.wchRunningNumber, L"") == 0)
	{
		GetIniTestLog(&sTestLog);
	}

	// ��֤��־��Ų�Ϊ��
	if (wcscmp(sTestLog.wchRunningNumber, L"") == 0)
	{
		m_edMsg.SetWindowTextW(L"��־���Ϊ�գ������޸�");
		return;
	}

	CString strOBDJCY;
	m_comOBDJCY.GetWindowTextW(strOBDJCY);

	//// ���Ա����Ϊ��
	//if (strOBDJCY.IsEmpty())
	//{
	//	m_edMsg.SetWindowTextW(L"���ԱΪ�գ������޸�");
	//	return;
	//}

	bool bRet(false);
	SResultOfOBD sResultOfOBD;
	CString strTemp, strMsg;
	strTemp.Format(L"%s", sTestLog.wchRunningNumber);
	sResultOfOBD.strRunningNumber = strTemp.GetString();
	sResultOfOBD.strOperator = strOBDJCY.GetString();
	bRet = SetOBDLOG(sResultOfOBD, strMsg);

	if (!bRet)
	{
		m_edMsg.SetWindowTextW(L"����OBD����ʧ��");
		return;
	}
	// д�����ݿ�
	SetDboResultOfOBD(sResultOfOBD);
	// д����ʱ�ļ�
	SetIniResultOfOBD(sResultOfOBD);

	// ����App_data
	wchar_t wchPath[MAX_PATH];
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

	// �������ݿⲿ��
	strMsg.Empty();
	strSQL.Format(_T("Update TestLog set ItemOBD = '4', HasOBD = '1' where RunningNumber = '%s'"), sTestLog.wchRunningNumber);
	int nRowsInvolved(0);
	if (0x00 == CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
	{
	}
	else
	{
		strMsg.AppendFormat(L"TestLog����ʧ��\r\n");
	}

	strSQL.Format(_T("Update VehicleInfo set HasOBD = '1' where PlateNumber = '%s'"), sTestLog.wchPlateNumber);
	nRowsInvolved = 0;
	if (0x00 == CNHSQLServerDBO::ExecuteDML(m_pConnection, strSQL, &nRowsInvolved))
	{
	}
	else
	{
		strMsg.AppendFormat(L"VehicleInfo����ʧ��\r\n");
	}
	// �Ƿ���Ҫ�޸�������ȡ���ļ�����
	// ���Ժ���˵
	if (strMsg.IsEmpty())
	{
		m_edMsg.SetWindowTextW(L"�������������");
	}
	else
	{
		strMsg = L"�����������, ���в���ʧ����Ϣ:\r\n" + strMsg;
		m_edMsg.SetWindowTextW(strMsg);
	}
	//�ϴ���ť��Ϊ���ɰ������³����б���Ϊ���԰�
	GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);
}

bool CSYDATADlg::SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg)
{
	bool bRet(true);

	CString strFulType(m_sCarInfo.strRlzl);

	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(0, 0, 0, 40);
	COleDateTime odtSartTime = odtCurTime - odts;

	sResultOfOBD.strDetBegTime = odtSartTime.Format(L"%Y-%m-%d %H:%M:%S");
	sResultOfOBD.strDetEndTime = odtCurTime.Format(L"%Y-%m-%d %H:%M:%S");

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

CString CSYDATADlg::GetOBDType(const int& nType, const CString& strFulType)
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

void CSYDATADlg::GetEngineCALID(const CString& strOBDType, const CString& strFulType,
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

