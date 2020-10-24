// NHCVarLoadGBDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "math.h"
#include "NHCVarLoadDlg2.h"

// �Զ���
#include "..\NHDetCommModu\NHDetCommModu.h"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// NHCVarLoadGBDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCVarLoadDlg2, CDialogZoom)

CNHCVarLoadDlg2::CNHCVarLoadDlg2(CWnd* pParent /*=NULL*/)
: CDialogZoom(CNHCVarLoadDlg2::IDD, pParent)
, m_ListReal(8)
, m_usDIW(907)
, m_fNominalTime1(0.00)
, m_fNominalTime2(0.00)
, m_fNominalTime3(0.00)
, m_fRealityTime1(0.00)
, m_fRealityTime2(0.00)
, m_fRealityTime3(0.00)
, m_fErr1(0.00)
, m_fErr2(0.00)
, m_fErr3(0.00)
, m_nAccType(0)
, m_bResult(false)
, m_pNHCVarLoadThread2 (NULL)
{
	
	// �����⹦�������߳�
	m_pNHCVarLoadThread2 = (CNHCVarLoadThread2 *)AfxBeginThread(RUNTIME_CLASS(CNHCVarLoadThread2));
	m_pNHCVarLoadThread2->SetAccType(m_nAccType);

	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 25 / 1024;
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
		_T("����"));
}

CNHCVarLoadDlg2::~CNHCVarLoadDlg2()
{
	if (NULL != m_pNHCVarLoadThread2)
	{
		m_pNHCVarLoadThread2->Close();
		m_pNHCVarLoadThread2->QuitThread();
		WaitForSingleObject(m_pNHCVarLoadThread2->m_hThread, 5000);
	}
}

void CNHCVarLoadDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_lbSpeed);
	DDX_Control(pDX, IDC_STATIC_POWER, m_lbPower);
	DDX_Control(pDX, IDC_STATIC_2, m_lbTime_Rel2);
	DDX_Control(pDX, IDC_STATIC_3, m_lbTime_Rel3);
	DDX_Control(pDX, IDC_STATIC_5, m_lbTime_Rel4);
	DDX_Control(pDX, IDC_STATIC_TIP, m_lbMsg);
	DDX_Control(pDX, IDC_STATIC_TIP1, m_lbTip1);
	DDX_Control(pDX, IDC_STATIC_TIP2, m_lbTip2);
	DDX_Control(pDX, IDC_STATIC_TIP3, m_lbTip3);
	DDX_Control(pDX, IDC_STATIC_TIP4, m_lbTip4);
	DDX_Control(pDX, IDC_STATIC_4, m_lbTime_Result);
	DDX_Control(pDX, IDC_STATIC_6, m_lbTime_Err2);
	DDX_Control(pDX, IDC_STATIC_7, m_lbTime_Err3);
	DDX_Control(pDX, IDC_STATIC_8, m_lbTime_Err4);
	DDX_Control(pDX, IDC_STATIC_10, m_lbDIW);
	DDX_Control(pDX, IDC_LIST_REAL, m_ListReal);
	DDX_Control(pDX, IDC_STATIC_TIME2, m_lbTime2);
	DDX_Control(pDX, IDC_STATIC_TIME3, m_lbTime3);
	DDX_Control(pDX, IDC_STATIC_TIME4, m_lbTime4);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
	DDX_Radio(pDX, IDC_RADIO_ENG, m_nAccType);
}


BEGIN_MESSAGE_MAP(CNHCVarLoadDlg2,CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STARTTEST, &CNHCVarLoadDlg2::OnBnClickedButtonStarttest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCVarLoadDlg2::OnBnClickedButtonStopTest)
	ON_BN_CLICKED(IDC_RADIO_CAR, &CNHCVarLoadDlg2::OnBnClickedRadioCar)
	ON_BN_CLICKED(IDC_RADIO_ENG, &CNHCVarLoadDlg2::OnBnClickedRadioEng)
END_MESSAGE_MAP()

BOOL CNHCVarLoadDlg2::OnInitDialog()
{
	CDialogZoom::OnInitDialog();
	m_ListReal.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	int nHeight = nSM_CYSCREEN * 30 / 1024;
	m_SmallImageList.Create(1,nHeight,0,0,0);
	m_ListReal.SetImageList(&m_SmallImageList, LVSIL_SMALL);
	CStringArray Head;
	CByteArray   Cols;

	Head.Add(L"�ٶ�");
	Cols.Add(12);       
	Head.Add(L"ʱ��");
	Cols.Add(13);       
	Head.Add(L"�ٶ�");
	Cols.Add(12);       
	Head.Add(L"ʱ��");
	Cols.Add(13);       
	Head.Add(L"�ٶ�");
	Cols.Add(12);       
	Head.Add(L"ʱ��");
	Cols.Add(13);       
	Head.Add(L"�ٶ�");
	Cols.Add(12);       
	Head.Add(L"ʱ��");
	Cols.Add(13);       
	m_ListReal.InitCtrl(&Head, &Cols);
	Head.RemoveAll();
	Cols.RemoveAll();
	InitCtrls();

	m_shdTitle.SetTitle(L"�⹦�����غɻ���");

	ShowWindow(SW_MAXIMIZE);

	SetDlgFont();

	// �򿪴��ڣ�������ͨѶ����
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
// NHCVarLoadGBDlg ��Ϣ�������

void CNHCVarLoadDlg2::InitCtrls(void)
{
	CStringArray Head;
	m_lbMsg.SetText(L"���غɻ��в���");
	m_lbMsg.SetTitleStyle();

	m_lbSpeed.SetLEDStyle();
	m_lbPower.SetLEDStyle();
	m_lbTime_Rel2.SetLEDStyle();
	m_lbTime_Rel3.SetLEDStyle();
	m_lbTime_Rel4.SetLEDStyle();

	m_lbTime_Result.SetLEDStyle();

	m_lbTime_Err2.SetLEDStyle();
	m_lbTime_Err3.SetLEDStyle();
	m_lbTime_Err4.SetLEDStyle();
	m_lbTime2.SetLEDStyle();
	m_lbTime3.SetLEDStyle();
	m_lbTime4.SetLEDStyle();
	m_lbDIW.SetLEDStyle();

	// ���ñ�ǩ������ɫ
	m_lbTip1.SetTextColor(RGB(0, 102, 204));
	m_lbTip2.SetTextColor(RGB(0, 102, 204));
	m_lbTip3.SetTextColor(RGB(0, 102, 204));
	m_lbTip4.SetTextColor(RGB(0, 102, 204));

	for (int i=0;i<8;i++)
	{
		m_ListReal.SetColumnWidth(i,85);
	}
	Head.RemoveAll();
	Head.Add(L"80.5");Head.Add(L"");Head.Add(L"61.1");Head.Add(L"");Head.Add(L"41.8");Head.Add(L"");Head.Add(L"24.1");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"78.8");Head.Add(L"");Head.Add(L"59.5");Head.Add(L"");Head.Add(L"40.2");Head.Add(L"");Head.Add(L"22.5");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"77.2");Head.Add(L"");Head.Add(L"57.9");Head.Add(L"");Head.Add(L"38.6");Head.Add(L"");Head.Add(L"20.9");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"75.6");Head.Add(L"");Head.Add(L"56.3");Head.Add(L"");Head.Add(L"37.0");Head.Add(L"");Head.Add(L"19.3");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"74.0");Head.Add(L"");Head.Add(L"54.7");Head.Add(L"");Head.Add(L"35.4");Head.Add(L"");Head.Add(L"17.7");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"72.4");Head.Add(L"");Head.Add(L"53.1");Head.Add(L"");Head.Add(L"33.8");Head.Add(L"");Head.Add(L"16.1");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();		
	Head.Add(L"70.8");Head.Add(L"");Head.Add(L"51.5");Head.Add(L"");Head.Add(L"32.2");Head.Add(L"");Head.Add(L"14.5");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"69.2");Head.Add(L"");Head.Add(L"49.9");Head.Add(L"");Head.Add(L"30.6");Head.Add(L"");Head.Add(L"12.9");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"67.6");Head.Add(L"");Head.Add(L"48.3");Head.Add(L"");Head.Add(L"29.0");Head.Add(L"");Head.Add(L"11.3");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"66.0");Head.Add(L"");Head.Add(L"46.7");Head.Add(L"");Head.Add(L"27.4");Head.Add(L"");Head.Add(L"9.7");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"64.4");Head.Add(L"");Head.Add(L"45.1");Head.Add(L"");Head.Add(L"25.7");Head.Add(L"");Head.Add(L"8.0");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	Head.Add(L"62.8");Head.Add(L"");Head.Add(L"43.4");Head.Add(L"");Head.Add(L"");Head.Add(L"");Head.Add(L"");Head.Add(L"");
	m_ListReal.AddItem(&Head);
	Head.RemoveAll();
	for (int line=1;line<12;line=line+2)
	{
		m_ListReal.SetItemBackgndColor(RGB(234, 234, 234),line,0);
		m_ListReal.SetItemBackgndColor(RGB(237, 237, 237),line,1);
		m_ListReal.SetItemBackgndColor(RGB(205, 205, 205),line-1,0);
		m_ListReal.SetItemBackgndColor(RGB(210, 210, 210),line-1,1);

		m_ListReal.SetItemBackgndColor(RGB(240, 240, 240),line,2);
		m_ListReal.SetItemBackgndColor(RGB(243, 243, 243),line,3);

		m_ListReal.SetItemBackgndColor(RGB(215, 215, 215),line-1,2);
		m_ListReal.SetItemBackgndColor(RGB(220, 220, 220),line-1,3);
	}

	for (int line=1;line<12;line=line+2)
	{
		m_ListReal.SetItemBackgndColor(RGB(246, 246, 246),line,4);
		m_ListReal.SetItemBackgndColor(RGB(249, 249, 249),line,5);
		m_ListReal.SetItemBackgndColor(RGB(225, 225, 225),line-1,4);
		m_ListReal.SetItemBackgndColor(RGB(230, 230, 230),line-1,5);

		m_ListReal.SetItemBackgndColor(RGB(252, 252, 252),line,6);
		m_ListReal.SetItemBackgndColor(RGB(255, 255, 255),line,7);

		m_ListReal.SetItemBackgndColor(RGB(235, 235, 235),line-1,6);
		m_ListReal.SetItemBackgndColor(RGB(240, 240, 240),line-1,7);
	}
	m_ListReal.SetBkColor(RGB(243, 243, 243));
	m_lbTime_Rel2.SetText(L"--");
	m_lbTime_Rel3.SetText(L"--");
	m_lbTime_Rel4.SetText(L"--");
	m_lbTime_Result.SetText(L"--");
	m_lbTime_Err2.SetText(L"--");
	m_lbTime_Err3.SetText(L"--");
	m_lbTime_Err4.SetText(L"--");
	m_lbTime2.SetText(L"--");
	m_lbTime3.SetText(L"--");
	m_lbTime4.SetText(L"--");	
}

void CNHCVarLoadDlg2::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCVarLoadDlg2::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent)
	{
		// �򿪴��ڣ�������ͨѶ
		KillTimer(1);
		OpenSerialPortAndTestCommunication();
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
	}
	else if (3 == nIDEvent)
	{
		DisplayVarLoadTestProcess();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCVarLoadDlg2::OpenSerialPortAndTestCommunication(void)
{
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Equipment.ini";

	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);
	// ��ȡͨѶ�˿�
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));

	if (DYN_OPEN_SUCCEED == m_pNHCVarLoadThread2->Open(bPort, CDynThread::IM_NANHUA_NHC03))
	{
		// ����ͨѶ����
		// ��ȡ������������ͨѶ����
		if (DYN_WR_SUCCEED == m_pNHCVarLoadThread2->GetDIW(&m_usDIW))
		{
			// ��ʾ��������
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_10)->SetWindowTextW(str);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("�⹦�����غɻ��в���"));
			GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
	}
}


void CNHCVarLoadDlg2::OnBnClickedButtonStarttest()
{
	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCVarLoadThread2->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCVarLoadThread2->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ�ʼ����", L"���غɻ���", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"���غɻ���", MB_OK|MB_ICONERROR);
		return;
	}
	// ��ȫ��ʾ���⹦����Ͳ��Ҫת��
	if (IDYES == MessageBox(L"���Թ��̹�Ͳ�������ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"���غɻ���", MB_YESNO|MB_ICONWARNING))
	{
		// ���ÿ�ʼ��ť
		GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(false);
		// �����˳���ť
		GetDlgItem(IDOK)->EnableWindow(false);	
		// ���ü�������ѡ��
		GetDlgItem(IDC_RADIO_CAR)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_ENG)->EnableWindow(false);
		
		// ��ʼˢ�¼�����
		SetTimer(3, 50, NULL);
		for(int i=0;i<12;i++)
		{
			m_ListReal.SetItemText(i,1,L"");
			m_ListReal.SetItemText(i,3,L"");
			m_ListReal.SetItemText(i,5,L"");
			m_ListReal.SetItemText(i,7,L"");
		}	
		// ��ʼ���غɲ���
		m_pNHCVarLoadThread2->StartVarLoadTest_Asyn(m_hWnd);
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		MessageBox(L"���Ա�ȡ��", L"���غɻ���", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();	
}
void CNHCVarLoadDlg2::RefreshRealTimeData(void)
{
	// ˢ��������
	// ͬʱ����ͨѶ����
	// 500����ִ��һ��

	CString str(L"");

	m_pNHCVarLoadThread2->GetRealTimeData_Asyn();

	str.Format(L"%.2f", m_pNHCVarLoadThread2->GetPValue());
	GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
	str.Format(L"%.2f", m_pNHCVarLoadThread2->GetVValue());
	GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);	
}

void CNHCVarLoadDlg2::DisplayVarLoadTestProcess(void)
{
	switch (m_pNHCVarLoadThread2->GetVarLoadState())
	{
	//����δ��ʼ
	case NHC_VARLOAD_STATE_NOT_START:
		{
		}
		break;
	//׼������
	case NHC_VARLOAD_STATE_READY:
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"���ڳ�ʼ�����Բ���...");
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
		}
		break;
	//��������
	case NHC_VARLOAD_STATE_LIFTBEAM_DOWN:
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"���ڽ�������...");
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(L"--");
		}
		break;
	//����
	case NHC_VARLOAD_STATE_ACCELERATION:
		{
			EnableRefreshRealTimeData(false);
			if (1 == m_nAccType)//�������
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"���ڼ���...");
			}
			else//��������
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"����ٵ�88.5Km/h...");
			}
			CString str(L"");
			str.Format(L"%.2f", m_pNHCVarLoadThread2->GetPValue());
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCVarLoadThread2->GetVValue());
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
		}
		break;
	//����
	case NHC_VARLOAD_STATE_SLIP:
		{
			EnableRefreshRealTimeData(false);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"���ڽ��б��غɲ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCVarLoadThread2->GetPValue());
			GetDlgItem(IDC_STATIC_POWER)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCVarLoadThread2->GetVValue());
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowTextW(str);
			// ��ʾ���ػ���ʱ��
			DisplayVarLoadSldingTime();
		}
		break;
	//�ȴ���Ͳֹͣ
	case NHC_VARLOAD_STATE_WAIT_FOR_STOP:
		{
			// ��ʾ���ػ���ʱ��
			DisplayVarLoadSldingTime();

			EnableRefreshRealTimeData(true);
		}
		break;
	//�������
	case NHC_VARLOAD_STATE_FINISHED:
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"�������");
			GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(TRUE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);

			EnableRefreshRealTimeData(true);

			// ֹͣˢ�¼������ʻ�������
			KillTimer(3);

			// ����ı��غɻ��н��
			CalculateVarLoadParams();

			// ��ʾ����
			ShowResult();
		}
		break;
	//���Թ��̷�������
	case NHC_VARLOAD_STATE_ERROR:
		{
			if (NHC_VARLOAD_COMMUNICATION_ERROR == m_pNHCVarLoadThread2->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"������ֹ,��⹦��ͨѶ��ʱ");
			}
			else if (NHC_VARLOAD_START_MMTIMER_FAIL == m_pNHCVarLoadThread2->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(L"������ֹ,������ʱ��ʧ��");
			}
			// ֹͣˢ�¼��ػ�������
			KillTimer(3);
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
		}
		break;
	default:
		break;
	}
}

void CNHCVarLoadDlg2::EnableRefreshRealTimeData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(2, 500, NULL);
	}
	else
	{
		KillTimer(2);
	}
}

void CNHCVarLoadDlg2::OnBnClickedButtonStopTest()
{
	GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("ֹͣ"));
	GetDlgItem(IDC_BUTTON_STARTTEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CAR)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_ENG)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);	

	m_pNHCVarLoadThread2 ->StopVarLoadTest();
	KillTimer(3);
	// ��ʼˢ������
	EnableRefreshRealTimeData(true);
}

void CNHCVarLoadDlg2::DisplayVarLoadSldingTime(void)
{
	CString str(L"");
	if (m_pNHCVarLoadThread2->GetVValue() >=80.5f)
	{

	}
	if (m_pNHCVarLoadThread2->GetVValue() >=78.8f)
	{
		// ˢ�µ�1�λ���ʱ��(80.5�̶���0��)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(0)/1000.0f);
		m_ListReal.SetItemText(0, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=77.2f)
	{
		// ˢ�µ�2�λ���ʱ��(80.5-78.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(1)/1000.0f);
		m_ListReal.SetItemText(1, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=75.6f)
	{
		// ˢ�µ�3�λ���ʱ��(78.8-77.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(2)/1000.0f);
		m_ListReal.SetItemText(2, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=74.0f)
	{
		// ˢ�µ�4�λ���ʱ��(77.2-75.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(3)/1000.0f);
		m_ListReal.SetItemText(3, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=72.4f)
	{
		// ˢ�µ�5�λ���ʱ��(75.6-74.0)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(4)/1000.0f);
		m_ListReal.SetItemText(4, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=70.8f)
	{
		// ˢ�µ�6�λ���ʱ��(74.0-72.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(5)/1000.0f);
		m_ListReal.SetItemText(5, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=69.2f)
	{
		// ˢ�µ�7�λ���ʱ��(72.4-70.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(6)/1000.0f);
		m_ListReal.SetItemText(6, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=67.60f)
	{
		// ˢ�µ�8�λ���ʱ��(70.8-69.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(7)/1000.0f);
		m_ListReal.SetItemText(7, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=66.0f)
	{
		// ˢ�µ�9�λ���ʱ��(69.2-67.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(8)/1000.0f);
		m_ListReal.SetItemText(8, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=64.4f)
	{
		// ˢ�µ�10�λ���ʱ��(67.6-66.0)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(9)/1000.0f);
		m_ListReal.SetItemText(9, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=62.8f)
	{
		// ˢ�µ�11�λ���ʱ��(66.0-64.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(10)/1000.0f);
		m_ListReal.SetItemText(10, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=61.1f)
	{
		// ˢ�µ�12�λ���ʱ��(64.4-62.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(11)/1000.0f);
		m_ListReal.SetItemText(11, 1, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=59.5f)
	{
		// ˢ�µ�13�λ���ʱ��(62.8-61.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(12)/1000.0f);
		m_ListReal.SetItemText(0, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=57.9f)
	{
		// ˢ�µ�14�λ���ʱ��(61.1-59.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(13)/1000.0f);
		m_ListReal.SetItemText(1, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=56.3f)
	{
		// ˢ�µ�15�λ���ʱ��(59.5-57.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(14)/1000.0f);
		m_ListReal.SetItemText(2, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=54.7f)
	{
		// ˢ�µ�16�λ���ʱ��(57.9-56.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(15)/1000.0f);
		m_ListReal.SetItemText(3, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=53.1f)
	{
		// ˢ�µ�17�λ���ʱ��(56.3-54.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(16)/1000.0f);
		m_ListReal.SetItemText(4, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue() >=51.50f)
	{
		// ˢ�µ�18�λ���ʱ��(54.7-53.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(17)/1000.0f);
		m_ListReal.SetItemText(5, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=49.9f)
	{
		// ˢ�µ�19�λ���ʱ��(53.1-51.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(18)/1000.0f);
		m_ListReal.SetItemText(6, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=48.3f)
	{
		// ˢ�µ�20�λ���ʱ��(51.5-49.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(19)/1000.0f);
		m_ListReal.SetItemText(7, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=46.7f)
	{
		// ˢ�µ�21�λ���ʱ��(49.9-48.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(20)/1000.0f);
		m_ListReal.SetItemText(8, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=45.1f)
	{
		// ˢ�µ�22�λ���ʱ��(48.3-46.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(21)/1000.0f);
		m_ListReal.SetItemText(9, 3, str);

	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=43.4f)
	{
		// ˢ�µ�23�λ���ʱ��(46.7-45.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(22)/1000.0f);
		m_ListReal.SetItemText(10, 3, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=41.8f)
	{
		// ˢ�µ�24�λ���ʱ��(45.1-43.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(23)/1000.0f);
		m_ListReal.SetItemText(11, 3, str);

	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=40.2f)
	{
		// ˢ�µ�25�λ���ʱ��(43.4-41.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(24)/1000.0f);
		m_ListReal.SetItemText(0, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=38.6f)
	{
		// ˢ�µ�26�λ���ʱ��(41.8-40.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(25)/1000.0f);
		m_ListReal.SetItemText(1, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=37.0f)
	{
		// ˢ�µ�27�λ���ʱ��(40.2-38.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(26)/1000.0f);
		m_ListReal.SetItemText(2, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=35.4f)
	{
		// ˢ�µ�28�λ���ʱ��(38.6-37.0)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(27)/1000.0f);
		m_ListReal.SetItemText(3, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=33.8f)
	{
		// ˢ�µ�29�λ���ʱ��(37.0-35.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(28)/1000.0f);
		m_ListReal.SetItemText(4, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=32.2f)
	{
		// ˢ�µ�30�λ���ʱ��(35.4-33.8)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(29)/1000.0f);
		m_ListReal.SetItemText(5, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=30.6f)
	{
		// ˢ�µ�31�λ���ʱ��(33.8-32.2)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(30)/1000.0f);
		m_ListReal.SetItemText(6, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=29.0f)
	{
		// ˢ�µ�32�λ���ʱ��(32.2-30.6)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(31)/1000.0f);
		m_ListReal.SetItemText(7, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=27.4f)
	{
		// ˢ�µ�33�λ���ʱ��(30.6-29.0)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(32)/1000.0f);
		m_ListReal.SetItemText(8, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=25.7f)
	{
		// ˢ�µ�34�λ���ʱ��(29.0-27.4)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(33)/1000.0f);
		m_ListReal.SetItemText(9, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=24.1f)
	{
		// ˢ�µ�35�λ���ʱ��(27.4-25.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(34)/1000.0f);
		m_ListReal.SetItemText(10, 5, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=22.5f)
	{
		// ˢ�µ�36�λ���ʱ��(25.7-24.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(35)/1000.0f);
		m_ListReal.SetItemText(0 ,7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=20.9f)
	{
		// ˢ�µ�37�λ���ʱ��(24.1-22.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(36)/1000.0f);
		m_ListReal.SetItemText(1, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=19.3f)
	{
		// ˢ�µ�38�λ���ʱ��(22.5-20.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(37)/1000.0f);
		m_ListReal.SetItemText(2, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=17.7f)
	{
		// ˢ�µ�39�λ���ʱ��(20.9-19.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(38)/1000.0f);
		m_ListReal.SetItemText(3, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=16.1f)
	{
		// ˢ�µ�40�λ���ʱ��(19.3-17.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(39)/1000.0f);
		m_ListReal.SetItemText(4, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=14.5f)
	{
		// ˢ�µ�41�λ���ʱ��(17.7-16.1)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(40)/1000.0f);
		m_ListReal.SetItemText(5, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=12.9f)
	{
		// ˢ�µ�42�λ���ʱ��(16.1-14.5)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(41)/1000.0f);
		m_ListReal.SetItemText(6, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=11.3f)
	{
		// ˢ�µ�43�λ���ʱ��(14.5-12.9)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(42)/1000.0f);
		m_ListReal.SetItemText(7, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=9.7f)
	{
		// ˢ�µ�44�λ���ʱ��(12.9-11.3)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(43)/1000.0f);
		m_ListReal.SetItemText(8, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=8.0f)
	{
		// ˢ�µ�45�λ���ʱ��(11.3-9.7)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(44)/1000.0f);
		m_ListReal.SetItemText(9, 7, str);
	}
	else if (m_pNHCVarLoadThread2->GetVValue()>=2.0f)
	{
		// ˢ�µ�46�λ���ʱ��(9.7-8.0)
		str.Format(L"%.3f", m_pNHCVarLoadThread2->GetVarLoadTime(45)/1000.0f);
		m_ListReal.SetItemText(10, 7, str);
	}
}

void CNHCVarLoadDlg2::CalculateVarLoadParams()
{
	m_fRealityTime1 = m_pNHCVarLoadThread2->GetVarLoadTotalTime() / 1000.0f;
	m_fRealityTime2 = (m_pNHCVarLoadThread2->GetVarLoadTime(40) / 1000.0f) - (m_pNHCVarLoadThread2->GetVarLoadTime(5) / 1000.0f);
	m_fRealityTime3 = (m_pNHCVarLoadThread2->GetVarLoadTime(23) / 1000.0f) - (m_pNHCVarLoadThread2->GetVarLoadTime(12) / 1000.0f);

	if (m_usDIW == 907)
	{
		m_fNominalTime1 = 25.77f;
		m_fNominalTime2 = 15.54f;
		m_fNominalTime3 = 3.98f;
	}
	else
	{
		m_fNominalTime1 = (float)(m_usDIW*28394) / 1000000 ;
		m_fNominalTime2 = (float)(m_usDIW*17133) / 1000000 ;
		m_fNominalTime3 = (float)(m_usDIW*43866) / 10000000 ;
	}
	m_fErr1 = ((m_fRealityTime1 - m_fNominalTime1) / m_fNominalTime1) * 100;
	m_fErr2 = ((m_fRealityTime2 - m_fNominalTime2) / m_fNominalTime2) * 100;
	m_fErr3 = ((m_fRealityTime3 - m_fNominalTime3) / m_fNominalTime3) * 100;
}

void CNHCVarLoadDlg2::ShowResult(void)
{
	CString strTemp;
	strTemp.Format(L"%.2f",m_fNominalTime1);
	GetDlgItem(IDC_STATIC_TIME2)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fNominalTime2);
	GetDlgItem(IDC_STATIC_TIME3)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fNominalTime3);
	GetDlgItem(IDC_STATIC_TIME4)->SetWindowText(strTemp);

	strTemp.Format(L"%.2f",m_fRealityTime1);
	GetDlgItem(IDC_STATIC_2)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fRealityTime2);
	GetDlgItem(IDC_STATIC_3)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fRealityTime3);
	GetDlgItem(IDC_STATIC_5)->SetWindowText(strTemp);

	strTemp.Format(L"%.2f",m_fErr1);
	GetDlgItem(IDC_STATIC_6)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fErr2);
	GetDlgItem(IDC_STATIC_7)->SetWindowText(strTemp);
	strTemp.Format(L"%.2f",m_fErr3);
	GetDlgItem(IDC_STATIC_8)->SetWindowText(strTemp);

	if (fabs(m_fErr1) > 4 || fabs(m_fErr2) > 2 || fabs(m_fErr3) > 3)
	{
		GetDlgItem(IDC_STATIC_4)->SetWindowText(L"���ϸ�");
	}
	else
	{
		GetDlgItem(IDC_STATIC_4)->SetWindowText(L"�ϸ�");
	}
}

void CNHCVarLoadDlg2::OnBnClickedRadioCar()
{
	//���������Ϊ��������
	m_nAccType=1;
	m_pNHCVarLoadThread2->SetAccType(m_nAccType);
}


void CNHCVarLoadDlg2::OnBnClickedRadioEng()
{
	//���������Ϊ�������
	m_nAccType=0;
	m_pNHCVarLoadThread2->SetAccType(m_nAccType);
}
