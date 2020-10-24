
// JNSXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JNSX.h"
#include "JNSXDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJNSXDlg �Ի���




CJNSXDlg::CJNSXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJNSXDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJNSXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_VEHICLE, m_lstVehicle);
	DDX_Control(pDX, ID_ED_JKDZ, m_edJKDZ);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_lbMessage);
	DDX_Control(pDX, IDC_CXDATE, m_edCXDate);
	DDX_Control(pDX, ID_ED_LINENUMBER, m_edLineNumber);
	DDX_Control(pDX, ID_ED_OBDJCY, m_edOBDJXY);
}

BEGIN_MESSAGE_MAP(CJNSXDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VEHICLE_LIST, &CJNSXDlg::OnBnClickedButtonUpdateVehicleList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_VEHICLE, &CJNSXDlg::OnLvnItemchangedLstVehicle)
	ON_BN_CLICKED(IDC_BTN_UPINTER, &CJNSXDlg::OnBnClickedBtnUpinter)
END_MESSAGE_MAP()


// CJNSXDlg ��Ϣ�������

BOOL CJNSXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��Ϣ����
	{
		CFont font;
		font.CreatePointFont(320,L"����");
		m_lbMessage.SetFont(&font);
	}

	GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);

	SetConfig();
	SetVehCtrl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CJNSXDlg::OnPaint()
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
HCURSOR CJNSXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJNSXDlg::SetConfig(void)
{
	// ��ȡexe(dll)�ļ�����·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"JNConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"EPB", L"WebAddress", L"");
		m_edJKDZ.SetWindowTextW(strTemp);
	}

	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NHClient.ini", wchPath))
	{
		CSimpleIni si(wchPath);
		CString strTemp;
		strTemp = si.GetString(L"System", L"LineNumber", L"");
		m_edLineNumber.SetWindowTextW(strTemp);
	}

}

void CJNSXDlg::SetVehCtrl(void)
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
	m_lstVehicle.InsertColumn(4, L"ȼ������",	0,	120);
	m_lstVehicle.InsertColumn(5, L"VIN",	0,	300);
	m_lstVehicle.InsertColumn(6, L"�Ƿ񸴼�",	0,	100);
	m_lstVehicle.InsertColumn(7, L"��ۼ����",	0,	100);

	CHeaderCtrl *pHeaderCtrl = m_lstVehicle.GetHeaderCtrl();
	pHeaderCtrl->SetFont(&fontDlgFont);
}

bool CJNSXDlg::GetVehicleList(void)
{
	// ��ֹ������°�ť
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(FALSE);

	CString strStaDate(L"");
	m_edCXDate.GetWindowTextW(strStaDate);
	strStaDate.AppendFormat(L" 00:00:00");

	CString strJKDZ(L"");
	m_edJKDZ.GetWindowTextW(strJKDZ);

	char* pchURL(NULL);

	if (NULL != pchURL)
	{
		free(pchURL);
		pchURL = NULL;
	}

	pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

	std::wstring strRet;
	int nRet(99);

	nRet = CJNSXInterfaceLib_API::GetOtherInspectQueueByDate(pchURL, strStaDate.GetString(), L"", L"1", L"" , strRet);

	// ���������ӿں󣬿��Ե������
	GetDlgItem(IDC_BUTTON_UPDATE_VEHICLE_LIST)->EnableWindow(TRUE);

	CString strMsg; 
	std::list<SVehDate> lsVehDate;

#ifdef _DEBUG
	nRet = 0;
	CStringW cstrCS;
	cstrCS.AppendFormat(L"\n");
	cstrCS.AppendFormat(L"0\n");
	cstrCS.AppendFormat(L"�ɹ�\n");
	cstrCS.AppendFormat(L"0\n");
	cstrCS.AppendFormat(L"\n");
	cstrCS.AppendFormat(L"<Count>1</Count><Inspect_1><InspectID>YCBC20191129171555540</InspectID><IsFirstInspect>2</IsFirstInspect>\
						 <WJResult>1</WJResult><InspectMethod>1</InspectMethod><CarInfo><VIN>LVVDB12A99D396836</VIN><PlateID>³R50W75</PlateID>\
						 <PlateType>1</PlateType><HPZL>02</HPZL><BrandName>����</BrandName><ModelName>SQR7080S117</ModelName>\
						 <EngineModel>SQR372</EngineModel><EngineSN>FD9L07576</EngineSN><VehicleTypeCode>K33</VehicleTypeCode>\
						 <IfHGL>false</IfHGL><CarType>1</CarType><IfGoIntoCity>true</IfGoIntoCity><IsTurbo>false</IsTurbo><FuelType>1</FuelType>\
						 <FuelSupplyID>1</FuelSupplyID><Is3WCC>True</Is3WCC><RatedSpeed>5000</RatedSpeed><DeliveryCapacity>0.8</DeliveryCapacity>\
						 <Cylinders>4</Cylinders><StrokeCycles></StrokeCycles><NorminalPower>38</NorminalPower><NominalPower>38</NominalPower>\
						 <FactoryDate>2009-11-01</FactoryDate><BaseWeight>980</BaseWeight><WholeWeight>1255</WholeWeight><RegDate>2009-12-23</RegDate>\
						 <PassengerCount>5</PassengerCount><GearType>1</GearType><InspectCount>3</InspectCount><DriveType>1</DriveType><EPStage>4</EPStage>\
						 <Owner>����</Owner><OwnerTel>18364009686</OwnerTel><OwnerAddress>ɽ��ʡ۩����۩����</OwnerAddress><OdoMeter>47464</OdoMeter>\
						 <ExhaustPipesNum>1</ExhaustPipesNum><AxleWeight></AxleWeight><ChassisModel></ChassisModel><ChassisFactory></ChassisFactory></CarInfo></Inspect_1>\n");
	cstrCS.AppendFormat(L"2019-11-29 17:17:52,����Ա���ʾ��\n");
	strRet = cstrCS.GetString();
#endif
	if (nRet == 0)
	{
		CString strTipMessage, strErrNum, strErrMsg, strInternalErrNum, strInternalErrMsg, strValue, strMemo, cstrRet;
		cstrRet = strRet.c_str();
		AfxExtractSubString(strTipMessage, cstrRet, 0, L'\n');
		AfxExtractSubString(strErrNum, cstrRet, 1, L'\n');
		AfxExtractSubString(strErrMsg, cstrRet, 2, L'\n');
		AfxExtractSubString(strInternalErrNum, cstrRet, 3, L'\n');
		AfxExtractSubString(strInternalErrMsg, cstrRet, 4, L'\n');
		AfxExtractSubString(strValue, cstrRet, 5, L'\n');
		AfxExtractSubString(strMemo, cstrRet, 6, L'\n');

		if (!strTipMessage.IsEmpty())
		{
			MessageBox(strTipMessage);
			m_lbMessage.SetWindowTextW(strTipMessage);
		}

		if (strErrNum == L"0")
		{
			if (strInternalErrNum != L"0")
			{
				//MessageBox(strInternalErrMsg);
			}

			CXmlReader xmlReader;
			if (xmlReader.Parse(strValue))
			{
				std::wstring strCode, strContent;

				if (xmlReader.OpenNode(L"Count"))
				{
					xmlReader.GetNodeContent(strCode);
				}
				if (strCode == L"0")
				{
					m_lbMessage.SetWindowTextW(L"ƽ̨����Ϊ��");
					return true;
				}
				else
				{
					int n = _wtoi(strCode.c_str());

					SVehDate sVehDate;

					for (int i=0; i<n ; i++)
					{
						CString strXmlCode;
						strXmlCode.Format(L"Inspect_%d/", i+1);
						// ����¼��ˮ��
						if (xmlReader.OpenNode((strXmlCode + L"InspectID")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strInspectID = strContent;
						}
						// IsFirstInspect �������ͣ�int �ͣ�1.�����죻2�����졣��V3.0 ������
						if (xmlReader.OpenNode((strXmlCode + L"IsFirstInspect")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strIsFirstInspect = strContent;
						}
						// WJResult ��ۼ������int �ͣ�1: �ϸ񡣣�V3.0 ������
						if (xmlReader.OpenNode((strXmlCode + L"WJResult")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strWJResult = strContent;
						}
						// CarInfo ������Ϣ��Xml ��ʽ����ʽ���·��� 4
						// VIN String ����ʶ����� VIN
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/VIN")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strVIN = strContent;
						}
						// PlateID string ���ƺ��롣
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/PlateID")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strPlateID = strContent;
						}
						// PlateType int �������࣬��ö�ٱ�-�������ࡣ
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/PlateType")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strPlateType = strContent;
						}
						// BrandName string �������ơ�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/BrandName")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ModelName string �����ͺš�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ModelName")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// EngineModel String �������ͺ�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/EngineModel")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// EngineSN String ����������
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/EngineSN")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// CarType int �������ͣ���ö�ٱ�-�������͡�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/CarType")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// VehicleTypeCode String ������ϸ���ʹ��ţ���ö����Ϣ-������ϸ���͡�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/VehicleTypeCode")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// IfHGL bool �Ƿ�Ƹ��̳��� true���ǣ��Ƹ��̳������� false���񣨷ǻƸ��̳�������
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/IfHGL")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// IfGoIntoCity bool �������ڡ����ٻ�����ũ�ó������� �ó����Ƿ������򽨳����� true���ǣ�������򽨳������� false���񣨲�������򽨳�������
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/IfGoIntoCity")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// IsTurbo bool �Ƿ����������ѹ�� true���ǣ�������ѹ���� false������Ȼ��������
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/IsTurbo")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// FuelType int ȼ�����ͣ���ö�ٱ�-ȼ�����͡�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/FuelType")))
						{
							xmlReader.GetNodeContent(strContent);
							sVehDate.strFuelType = strContent;
						}
						// FuelSupplyID Int ���ͷ�ʽ����ö�ٱ�-���ͷ�ʽ��
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/FuelSupplyID")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// Is3WCC bool �Ƿ�ʹ����Ԫ�߻�ת��װ�ã� true: ʹ�ã� false: ��ʹ�á�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/Is3WCC")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// RatedSpeed int �������ת�٣������ֲ��֣�����λ��rpm�� ����Ħ�г������Ħ�г�����ֵΪ���ߵ���ת�١��� 2 ����
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/RatedSpeed")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// DeliveryCapacity string �����������������ֲ��֣�����λ��L��
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/DeliveryCapacity")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// Cylinders int ����������
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/Cylinders")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// StrokeCycles int �������������2 �� 4��������Ħ�г������Ħ�г���
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/StrokeCycles")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// NominalPower int �������궨���ʣ������ڼ��ؼ��ٹ�������
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/NominalPower")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// FactoryDate string �������ڡ���ʽΪ��yyyy-MM-dd����
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/FactoryDate")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// BaseWeight string ��׼�����������ֲ��֣�����λ��kg��
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/BaseWeight")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// WholeWeight string ����������������ֲ��֣�����λ��kg��
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/WholeWeight")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// RegDate string ���εǼ����ڡ���ʽΪ��yyyy-MM-dd����
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/RegDate")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// PassengerCount int ��������(������ʻԱ)�� 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/PassengerCount")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// GearType int ��������ʽ����ö�ٱ�-��������ʽ��
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/GearType")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// InspectCount int ������ DriveType int �������ͣ���ö�ٱ�-�������͡�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/InspectCount")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// EPStage Int �����ŷŽ׶Σ���ö�ٱ�-�ŷŽ׶Ρ�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/EPStage")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// Owner string �������� 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/Owner")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// OwnerTel string �����绰 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/OwnerTel")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// OwnerAddress string ������ַ 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/OwnerAddress")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// OdoMeter string �ۼ���ʻ����� 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/OdoMeter")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ExhaustPipesNum Int ���������� 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ExhaustPipesNum")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// AxleWeight string �������أ����ڼ���˲̬��������������Ϣ����Ϊ�գ� 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/AxleWeight")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ChassisModel string �����ͺţ����ڼ���˲̬��������������Ϣ����Ϊ�գ� 
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ChassisModel")))
						{
							xmlReader.GetNodeContent(strContent);
						}
						// ChassisFactory string ����������ҵ�����ڼ���˲̬��������������Ϣ����Ϊ�գ�
						if (xmlReader.OpenNode((strXmlCode + L"CarInfo/ChassisFactory")))
						{
							xmlReader.GetNodeContent(strContent);
						}

						lsVehDate.push_back(sVehDate);
					}
				}

			}
			else
			{
				strMsg.Format(L"��������");
				m_lbMessage.SetWindowTextW(strMsg);
				return false;
			}

		}
		else
		{
			MessageBox(strErrMsg);
			m_lbMessage.SetWindowTextW(strErrMsg);
			return false;
		}

	}
	else
	{
		strMsg.Format(L"���Ӵ���\r\n���ش��ţ�%d", nRet);
		m_lbMessage.SetWindowTextW(strMsg);
		return false;
	}

	// ɾ���б�����
	m_lstVehicle.DeleteAllItems();
	int nItem(0);
	for (list<SVehDate>::iterator ir=lsVehDate.begin(); ir!=lsVehDate.end(); ++ir)
	{
		CString str;
		// ���
		str.Format(L"%d", nItem+1);
		int nItemIndex = m_lstVehicle.InsertItem(nItem, str);

		// ��ˮ��
		m_lstVehicle.SetItem (nItemIndex, 1, LVIF_TEXT, ir->strInspectID.c_str(), 0, 0, 0, 0);
		// ���ƺ���
		m_lstVehicle.SetItem (nItemIndex, 2, LVIF_TEXT, ir->strPlateID.c_str(), 0, 0, 0, 0);
		// ��������
		m_lstVehicle.SetItem (nItemIndex, 3, LVIF_TEXT, ir->strPlateType.c_str(), 0, 0, 0, 0);
		// ȼ������
		m_lstVehicle.SetItem (nItemIndex, 4, LVIF_TEXT, ir->strFuelType.c_str(), 0, 0, 0, 0);
		// VIN
		m_lstVehicle.SetItem (nItemIndex, 5, LVIF_TEXT, ir->strVIN.c_str(), 0, 0, 0, 0);
		// �Ƿ񸴼�
		str = ir->strIsFirstInspect.c_str();
		if (str == L"2"){str = L"����";}
		else{str = L"����";}
		m_lstVehicle.SetItem (nItemIndex, 6, LVIF_TEXT, str, 0, 0, 0, 0);
		// ��ۼ����
		m_lstVehicle.SetItem (nItemIndex, 7, LVIF_TEXT, ir->strWJResult.c_str(), 0, 0, 0, 0);

		nItem++;
	}

	return true;
}

void CJNSXDlg::OnBnClickedButtonUpdateVehicleList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bret = GetVehicleList();

	if (bret)
	{
		CString strMsg;
		m_lbMessage.GetWindowTextW(strMsg);
		strMsg.AppendFormat(L"\r\n��ȡ������Ϣ�ɹ�����");
		m_lbMessage.SetWindowTextW(strMsg);
	}
}


void CJNSXDlg::OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();
	
	m_sVehDate = SVehDate();
	int nIndex = m_lstVehicle.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(nIndex!=-1)
	{
		CString strTemp(L"");

		strTemp = m_lstVehicle.GetItemText(nIndex,1);
		m_sVehDate.strInspectID = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,2);
		m_sVehDate.strPlateID = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,3);
		m_sVehDate.strPlateType = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,4);
		m_sVehDate.strFuelType = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,5);
		m_sVehDate.strVIN = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,6);
		m_sVehDate.strIsFirstInspect = strTemp.GetString();

		strTemp = m_lstVehicle.GetItemText(nIndex,7);
		m_sVehDate.strWJResult = strTemp.GetString();

		// ѡ�г��ƺ���Ե��UP
		GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(TRUE);
	}

	EndWaitCursor();

	*pResult = 0;
}


void CJNSXDlg::OnBnClickedBtnUpinter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString cstrLineNumber(L""), cstrOBDJCY(L"");

	m_edLineNumber.GetWindowTextW(cstrLineNumber);
	m_edOBDJXY.GetWindowTextW(cstrOBDJCY);

	if (!cstrLineNumber.IsEmpty() && !cstrOBDJCY.IsEmpty())
	{
		CString strJKDZ(L"");
		m_edJKDZ.GetWindowTextW(strJKDZ);

		char* pchURL(NULL);

		if (NULL != pchURL)
		{
			free(pchURL);
			pchURL = NULL;
		}

		pchURL = CNHCommonAPI::UnicodeToANSI(strJKDZ);

		CString cstrsOBDData(L"");

		COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
		COleDateTimeSpan odts;
		odts.SetDateTimeSpan(0, 0, 0, 40);
		COleDateTime odtSartTime = odtCurTime - odts;

		
		// ���ַ���
		{
			CString cstrOBDdata(L"");
			cstrsOBDData.AppendFormat(L"<OBDGZZSQ>0</OBDGZZSQ>");
			cstrsOBDData.AppendFormat(L"<OBDInspectCount>1</OBDInspectCount>");
			cstrsOBDData.AppendFormat(L"<OBDTXZT>1</OBDTXZT>");
			cstrsOBDData.AppendFormat(L"<OBDTXSBYY>1</OBDTXSBYY>");
			cstrsOBDData.AppendFormat(L"<OBDGZ>0</OBDGZ>");
			cstrsOBDData.AppendFormat(L"<OBDGZDM></OBDGZDM>");
			cstrsOBDData.AppendFormat(L"<OBDJXZT>0</OBDJXZT>");
			cstrsOBDData.AppendFormat(L"<OBDWWCXM>0</OBDWWCXM>");
			cstrsOBDData.AppendFormat(L"<OBDWWCXM>0</OBDWWCXM>");
			cstrsOBDData.AppendFormat(L"<MILDLXSLC>0.000</MILDLXSLC>");
			cstrsOBDData.AppendFormat(L"<FDJKZDYCALID>��֧��</FDJKZDYCALID>");
			cstrsOBDData.AppendFormat(L"<FDJKZDYCVN>��֧��</FDJKZDYCVN>");
			cstrsOBDData.AppendFormat(L"<HCLKZDYCALID></HCLKZDYCALID>");
			cstrsOBDData.AppendFormat(L"<HCLKZDYCVN></HCLKZDYCVN>");
			cstrsOBDData.AppendFormat(L"<QTKZDYCALID></QTKZDYCALID>");
			cstrsOBDData.AppendFormat(L"<QTKZDYCVN></QTKZDYCVN>");
			cstrsOBDData.AppendFormat(L"<OBDResult>1</OBDResult>");
			cstrsOBDData.AppendFormat(L"<JYR>%s</JYR>", cstrOBDJCY); // ������ ID
			cstrsOBDData.AppendFormat(L"<BeginTime>%s</BeginTime>", odtSartTime.Format(L"%Y-%m-%d %H:%M:%S"));
			cstrsOBDData.AppendFormat(L"<EndTime>%s</EndTime>", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			cstrsOBDData.AppendFormat(L"<IsReInspect>0</IsReInspect>");
			cstrsOBDData.AppendFormat(L"<ReInspectProject></ReInspectProject>");
			cstrsOBDData.AppendFormat(L"<ReOBDResult>0</ReOBDResult>");
			{
				CString strdata(L"");

				cstrOBDdata.AppendFormat(L"<VIN>%s</VIN>", m_sVehDate.strVIN.c_str());
				cstrOBDdata.AppendFormat(L"<OBDYQ></OBDYQ>");
				cstrOBDdata.AppendFormat(L"<ODO>0</ODO>");
				cstrOBDdata.AppendFormat(L"<ODO>0</ODO>");
				{
					CString strTemp1;
					strdata.AppendFormat(L"<KZDYXXCount>1</KZDYXXCount>");
					{
						strTemp1.AppendFormat(L"<KZDYMC>������</KZDYMC>");
						strTemp1.AppendFormat(L"<CALID>��֧��</CALID>");
						strTemp1.AppendFormat(L"<CVN>��֧��</CVN>");
					}
					strdata.AppendFormat(L"<KZDYXX001>%s</KZDYXX001>", strTemp1);
				}
				cstrOBDdata.AppendFormat(L"<KZDYXX>%s</KZDYXX>", strdata);
				cstrOBDdata.AppendFormat(L"<GZNR_GZDM>��</GZNR_GZDM>");
				cstrOBDdata.AppendFormat(L"<GZNR_MIL>0.000</GZNR_MIL>");
				cstrOBDdata.AppendFormat(L"<JXZT_GZZDQMS>��</JXZT_GZZDQMS>");

				strdata = L"";
				{
					CString strTemp1;
					strdata.AppendFormat(L"<JXZTCount>2</JXZTCount>");
					{
						strTemp1.AppendFormat(L"<SBMC>1</SBMC>");
						strTemp1.AppendFormat(L"<SBZT>1</SBZT>");
					}
					strdata.AppendFormat(L"<JXZT001>%s</JXZT001>", strTemp1);
					strTemp1 = L"";
					{
						strTemp1.AppendFormat(L"<SBMC>2</SBMC>");
						strTemp1.AppendFormat(L"<SBZT>1</SBZT>");
					}
					strdata.AppendFormat(L"<JXZT002>%s</JXZT002>", strTemp1);
				}
				cstrOBDdata.AppendFormat(L"<JXZT_JXZT>%s</JXZT_JXZT>", strdata);
				strdata = L"";
				{
					CString strTemp1;
					strdata.AppendFormat(L"<EGRVVTCount>1</EGRVVTCount>");
					{
						strTemp1.AppendFormat(L"<XMMC>EGR</XMMC>");
						strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
						strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
						strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
					}
					strdata.AppendFormat(L"<EGRVVT001>%s</EGRVVT001>", strTemp1);
				}
				cstrOBDdata.AppendFormat(L"<IUPR_EGRVVT>%s</IUPR_EGRVVT>", strdata);
				if (m_sVehDate.strFuelType == L"����")
				{
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<NMHCCount>1</NMHCCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>�߻���</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<NMHC001>%s</NMHC001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_NMHC>%s</IUPR_NMHC>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<NOXCHCount>1</NOXCHCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>NOX�߻���</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<NOXCH001>%s</NOXCH001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_NOXCH>%s</IUPR_NOXCH>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<NOXXFCount>1</NOXXFCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>NOX������</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<NOXXF001>%s</NOXXF001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_NOXXF>%s</IUPR_NOXXF>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<PMCount>1</PMCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>PM������</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<PM001>%s</PM001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_PM>%s</IUPR_PM>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<FQCGQCount>1</FQCGQCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>����������</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<FQCGQ001>%s</FQCGQ001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_FQCGQ>%s</IUPR_FQCGQ>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<ZYYLCount>1</ZYYLCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>��ѹѹ��</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<ZYYL001>%s</ZYYL001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_ZYYL>%s</IUPR_ZYYL>", strdata);
				}
				// ��϶���ҲĬ������
				else
				{
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<CHQ1Count>1</CHQ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>�߻�����1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<CHQ1001>%s</CHQ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_CHQ1>%s</IUPR_CHQ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<CHQ2Count>1</CHQ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>�߻�����2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<CHQ2001>%s</CHQ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_CHQ2>%s</IUPR_CHQ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<QYCGQZ1Count>1</QYCGQZ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>ǰ����������1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<QYCGQZ1001>%s</QYCGQZ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_QYCGQZ1>%s</IUPR_QYCGQZ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<QYCGQZ2Count>1</QYCGQZ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>ǰ����������2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<QYCGQZ2001>%s</QYCGQZ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_QYCGQZ2>%s</IUPR_QYCGQZ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<HYCGQZ1Count>1</HYCGQZ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>������������1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<HYCGQZ1001>%s</HYCGQZ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_HYCGQZ1>%s</IUPR_HYCGQZ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<HYCGQZ2Count>1</HYCGQZ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>������������2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<HYCGQZ2001>%s</HYCGQZ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_HYCGQZ2>%s</IUPR_HYCGQZ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<EVAPCount>1</EVAPCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>EVAP</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<EVAP001>%s</EVAP001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_EVAP>%s</IUPR_EVAP>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<GPFZ1Count>1</GPFZ1Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>GPF��1</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<GPFZ1001>%s</GPFZ1001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_GPFZ1>%s</IUPR_GPFZ1>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<GPFZ2Count>1</GPFZ2Count>");
						{
							strTemp1.AppendFormat(L"<XMMC>GPF��2</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<GPFZ2001>%s</GPFZ2001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_GPFZ2>%s</IUPR_GPFZ2>", strdata);
					strdata = L"";
					{
						CString strTemp1;
						strdata.AppendFormat(L"<ECKQPSCount>1</ECKQPSCount>");
						{
							strTemp1.AppendFormat(L"<XMMC>���ο�������ϵͳ</XMMC>");
							strTemp1.AppendFormat(L"<JCWCCS>0</JCWCCS>");
							strTemp1.AppendFormat(L"<FHJCTJCS>0</FHJCTJCS>");
							strTemp1.AppendFormat(L"<IUPRResult>0</IUPRResult>");
						}
						strdata.AppendFormat(L"<ECKQPS001>%s</ECKQPS001>", strTemp1);
					}
					cstrOBDdata.AppendFormat(L"<IUPR_ECKQPS>%s</IUPR_ECKQPS>", strdata);
				}
			}
			cstrsOBDData.AppendFormat(L"<OBDdata>%s</OBDdata>", cstrOBDdata);
		}

		std::wstring strRet;
		int nRet(99);

		nRet = CJNSXInterfaceLib_API::OBDInspect(pchURL, cstrLineNumber.GetString(), m_sVehDate.strInspectID, cstrsOBDData.GetString(), L"", strRet);

		if (nRet == 0)
		{
			CString strTipMessage, strErrNum, strErrMsg, strInternalErrNum, strInternalErrMsg, strValue, strMemo, cstrRet;
			cstrRet = strRet.c_str();
			AfxExtractSubString(strTipMessage, cstrRet, 0, L'\n');
			AfxExtractSubString(strErrNum, cstrRet, 1, L'\n');
			AfxExtractSubString(strErrMsg, cstrRet, 2, L'\n');
			AfxExtractSubString(strInternalErrNum, cstrRet, 3, L'\n');
			AfxExtractSubString(strInternalErrMsg, cstrRet, 4, L'\n');
			AfxExtractSubString(strValue, cstrRet, 5, L'\n');
			AfxExtractSubString(strMemo, cstrRet, 6, L'\n');

			if (!strTipMessage.IsEmpty())
			{
				MessageBox(strTipMessage);
				m_lbMessage.SetWindowTextW(strTipMessage);
			}

			if (strErrNum == L"0")
			{
				// MessageBox(strValue);
				m_lbMessage.SetWindowTextW(strValue);
			}
			else
			{
				//MessageBox(strErrMsg);
				m_lbMessage.SetWindowTextW(strErrMsg);
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(L"���Ӵ���\r\n���ش��ţ�%d", nRet);
			m_lbMessage.SetWindowTextW(strMsg);
		}
		GetDlgItem(IDC_BTN_UPINTER)->EnableWindow(FALSE);
	}
	else
	{
			m_lbMessage.SetWindowTextW(L"����߱��Ϊ�ջ�\r\n�����ԱΪ��");
	}
}
