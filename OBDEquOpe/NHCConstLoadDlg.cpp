// NHCConstLoadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCConstLoadDlg.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif


// CNHCConstLoadDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCConstLoadDlg, CDialogZoom)

CNHCConstLoadDlg::CNHCConstLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCConstLoadDlg::IDD, pParent)
	, m_pNHCConstLoadThread(NULL)
	, m_usDIW(0)
	, m_fLowerRangePower(6.1f)
	, m_fHigherRangePower(6.1f)
	, m_fCustomRangePower(6.1f)
	, m_unLowerRangeCCDT(0)
	, m_unLowerRangeACDT(0)
	, m_unHigherRangeACDT(0)
	, m_unHigherRangeCCDT(0)
	, m_unLowerRangeRELimitX100(700)
	, m_unHigherRangeRELimitX100(700)
	, m_unCustomRangeRELimitX100(700)
	, m_nLowerRangeREX100(0)
	, m_nHigherRangeREX100(0)
	, m_nCustomRangeREX100(0)
	, m_bIsLowerRangePass(false)
	, m_bIsHigherRangePass(false)
	, m_bLowerRangeTestFinishedAtLeastOnce(false)
	, m_bHigherRangeTestFinishedAtLeastOnce(false)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_bLowerRangeLoadConstStep(false)
	, m_bHigherRangeLoadConstStep(false)
	, m_bCustomRangeLoadConstStep(false)
	, m_fLowerVelocityOfLowerRange(17.0f)
	, m_fUpperVelocityOfLowerRange(33.0f)
	, m_fLowerVelocityOfHigherRange(32.0f)
	, m_fUpperVelocityOfHigherRange(48.0f)
	, m_fAp(0.0f)
	, m_fBp(0.0f)
	, m_fCp(0.0f)
	, m_pfProcessCtrl(NULL)
	, m_nRandomError(0)
	, m_bWriteTimer(false)
	, m_odtStart(COleDateTime::GetCurrentTime())
	, m_odtEnd(COleDateTime::GetCurrentTime())
{
	// ���1280*1024�ֱ��ʽ��п���
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
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
		_T("����"));

	ImportParams();

	m_lsRTDataOfDynConstLoad.RemoveAll();

	for(int i=0; i<9; i++)
	{
		m_unLugdownRangeACDT30[i] = 0;
		m_unLugdownRangeACDT20[i] = 0;
		m_unLugdownRangeACDT10[i] = 0;
		m_fLugdownPLHP[i] = 0.0f;
	}

	// �����⹦�������߳�
	m_pNHCConstLoadThread = (CNHCConstLoadThread*)AfxBeginThread(RUNTIME_CLASS(CNHCConstLoadThread));
}

CNHCConstLoadDlg::~CNHCConstLoadDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pNHCConstLoadThread)
	{
		m_pNHCConstLoadThread->Close();
		m_pNHCConstLoadThread->QuitThread();
		WaitForSingleObject(m_pNHCConstLoadThread->m_hThread, 5000);
	}
}

void CNHCConstLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_ASM_RANGE, m_rdoAsmRange);
	DDX_Control(pDX, IDC_RADIO_VMAS_RANGE, m_rdoVmasRange);
	DDX_Control(pDX, IDC_RADIO_LUGDOWN_RANGE, m_rdoLugdownRange);
	DDX_Control(pDX, IDC_EDIT_LOWER_RANGE_POWER, m_edLowerRangePower);
	DDX_Control(pDX, IDC_EDIT_HIGHER_RANGE_POWER, m_edHigherRangePower);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_RANGE_POWER, m_edCustomRangePower);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lb4);
	DDX_Control(pDX, IDC_STATIC_P, m_lbP);
	DDX_Control(pDX, IDC_STATIC_V, m_lbV);
	DDX_Control(pDX, IDC_STATIC_DIW, m_lbDIW);
	DDX_Control(pDX, IDC_STATIC_TOTAL_RESULT, m_lbTotalResult);
	DDX_Control(pDX, IDC_STATIC_LOWER_RANGE_RESULT, m_lbLowerRangeResult);
	DDX_Control(pDX, IDC_STATIC_HIGHER_RANGE_RESULT, m_lbHigherRangeResult);
	DDX_Control(pDX, IDC_STATIC_CUSTOM_RANGE_RESULT, m_lbCustomRangeResult);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}

BEGIN_MESSAGE_MAP(CNHCConstLoadDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_ASM_RANGE, &CNHCConstLoadDlg::OnBnClickedRadioAsmRange)
	ON_BN_CLICKED(IDC_RADIO_VMAS_RANGE, &CNHCConstLoadDlg::OnBnClickedRadioVmasRange)
	ON_BN_CLICKED(IDC_RADIO_LUGDOWN_RANGE, &CNHCConstLoadDlg::OnBnClickedRadioLugdownRange)
	ON_BN_CLICKED(IDCANCEL, &CNHCConstLoadDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START_LOWER_RANGE_TEST, &CNHCConstLoadDlg::OnBnClickedButtonStartLowerRangeTest)
	ON_BN_CLICKED(IDC_BUTTON_START_HIGHER_RANGE_TEST, &CNHCConstLoadDlg::OnBnClickedButtonStartHigherRangeTest)
	ON_BN_CLICKED(IDC_BUTTON_START_CUSTOM_RANGE_TEST, &CNHCConstLoadDlg::OnBnClickedButtonStartCustomRangeTest)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_UP, &CNHCConstLoadDlg::OnBnClickedButtonLiftbeamUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFTBEAM_DOWN, &CNHCConstLoadDlg::OnBnClickedButtonLiftbeamDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ON, &CNHCConstLoadDlg::OnBnClickedButtonMotorOn)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_OFF, &CNHCConstLoadDlg::OnBnClickedButtonMotorOff)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CNHCConstLoadDlg::OnBnClickedButtonStopTest)
	ON_MESSAGE(WM_DYN_CONST_LOAD_TEST_FINISHED, OnConstLoadTestFinished)
END_MESSAGE_MAP()


// CNHCConstLoadDlg ��Ϣ�������

BOOL CNHCConstLoadDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// �򿪴��ڣ�������ͨѶ����
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCConstLoadDlg::InitCtrls(void)
{
	// ���öԻ�������
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦�����ػ���");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();
	GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);

	// ��ֹ��������
	EnableBasicOperation(false);

	// ���ñ�ǩ������ɫ
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));
	m_lb4.SetTextColor(RGB(0, 102, 204));

	// ���û���������ʾ����
	m_lbDIW.SetBkColor(RGB(230, 230, 230));

	// ���ý����ʾ����
	m_lbTotalResult.SetBkColor(RGB(230, 230, 230));
	m_lbLowerRangeResult.SetBkColor(RGB(230, 230, 230));
	m_lbHigherRangeResult.SetBkColor(RGB(230, 230, 230));
	m_lbCustomRangeResult.SetBkColor(RGB(230, 230, 230));

	// ���ù��ʡ��ٶ���ʾ����
	m_lbP.SetLEDStyle();
	m_lbV.SetLEDStyle();

	// �����ܽ������
	m_lbTotalResult.SetText(L"--");

	// ��ʾ��������ֵ
	CString str(L"");
	str.Format(L"[-%.2f,%.2f]", m_unLowerRangeRELimitX100/100.0f, m_unLowerRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_LOWER_RANGE_LIMIT)->SetWindowTextW(str);
	str.Format(L"[-%.2f,%.2f]", m_unHigherRangeRELimitX100/100.0f, m_unHigherRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_LIMIT)->SetWindowTextW(str);

	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_LIMIT)->SetWindowTextW(str);

	// �༭��ֻ����������
	m_edLowerRangePower.ModifyStyle(0, ES_NUMBER);
	m_edHigherRangePower.ModifyStyle(0, ES_NUMBER);
	m_edCustomRangePower.ModifyStyle(0, ES_NUMBER);
	((CEdit*)GetDlgItem(IDC_EDIT_CUSTOM_RANGE_LOW_SPEED))->ModifyStyle(0, ES_NUMBER);
	((CEdit*)GetDlgItem(IDC_EDIT_CUSTOM_RANGE_HIGH_SPEED))->ModifyStyle(0, ES_NUMBER);

	// ��ʾ���ع���
	str.Format(L"%.2f", m_fLowerRangePower);
	m_edLowerRangePower.SetWindowTextW(str);
	str.Format(L"%.2f", m_fHigherRangePower);
	m_edHigherRangePower.SetWindowTextW(str);
	str.Format(L"%.2f", m_fCustomRangePower);
	m_edCustomRangePower.SetWindowTextW(str);

	// ��ʼ����������ASM����
	SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE, 0);
	GetDlgItem(IDC_STATIC_LOWER_RANGE)->SetWindowTextW(L"33~17");
	GetDlgItem(IDC_STATIC_HIGHER_RANGE)->SetWindowTextW(L"48~32");
	m_rdoAsmRange.SetCheck(BST_CHECKED);
	m_fLowerVelocityOfLowerRange = 17.0f;
	m_fUpperVelocityOfLowerRange = 33.0f;
	m_fLowerVelocityOfHigherRange = 32.0f;
	m_fUpperVelocityOfHigherRange = 48.0f;

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", wchPath, false))
	{
		// �ļ�������
	}
	CSimpleIni si(wchPath);

	CString strHDyn;
	strHDyn = si.GetString(L"DynParams", L"HeavyDyn", L"0");

	if (strHDyn == L"1")
	{
		// ��ʼ��Lugdown����
		OnBnClickedRadioLugdownRange();
	}
	else
	{
		// ��ʼ��ASM����
		OnBnClickedRadioAsmRange();
	}
}

void CNHCConstLoadDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCConstLoadDlg::OpenSerialPortAndTestCommunication(void)
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
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// ת��Ϊ��д
	strDynProtocol.MakeUpper();
	CDynThread::InstrumentModel imDyn;
	if (L"NANHUA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}
	else if (L"MAHA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_MAHA_ASMP;
	}
	//else if (L"VAMAGRSR576" == strDynProtocol)
	//{
	//	imDyn = CDynThread::IM_VAMAG_RSR576;
	//}
	else
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}

	if (DYN_OPEN_SUCCEED == m_pNHCConstLoadThread->Open(bPort, imDyn))
	{
		// ����ͨѶ����
		// ��ȡ״̬����ͨѶ����
		if (DYN_WR_SUCCEED == m_pNHCConstLoadThread->GetDIW(&m_usDIW))
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("�⹦�����ػ���"));
			GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
			EnableBasicOperation(true);
			// ��ʾ��������
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);

			// ��ȡ�������ʲ���,��ʣ��
			//��ȡAp
			m_pNHCConstLoadThread->ReadApWithRemanence(&m_fAp);
			//��ȡBp
			m_pNHCConstLoadThread->ReadBpWithRemanence(&m_fBp); 
			//��ȡCp
			m_pNHCConstLoadThread->ReadCpWithRemanence(&m_fCp); 

			EnableRefreshRealTimeData(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
			EnableBasicOperation(false);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(FALSE);
		EnableBasicOperation(false);
	}

#ifdef _DEBUG
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("�⹦�����ػ���"));
			GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
			EnableBasicOperation(true);
			// ��ʾ��������
			CString str(L"");
			str.Format(L"%hu", m_usDIW);
			GetDlgItem(IDC_STATIC_DIW)->SetWindowTextW(str);

			// ��ȡ�������ʲ���,��ʣ��
			//��ȡAp
			m_pNHCConstLoadThread->ReadApWithRemanence(&m_fAp);
			//��ȡBp
			m_pNHCConstLoadThread->ReadBpWithRemanence(&m_fBp); 
			//��ȡCp
			m_pNHCConstLoadThread->ReadCpWithRemanence(&m_fCp); 

			EnableRefreshRealTimeData(true);
#endif
}

void CNHCConstLoadDlg::ImportParams()
{
	////////////////////////////////////////////////////////// �������ļ���ȡ���غɻ��в���begin
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);
	CSimpleIni si(strPath);
	CString str;
	str = si.GetString(L"DynConstLoad", L"LowerRangePower", L"6.1");
	m_fLowerRangePower = (float)_tstof(str);
	str = si.GetString(L"DynConstLoad", L"HigherRangePower", L"6.1");
	m_fHigherRangePower = (float)_tstof(str);
	str = si.GetString(L"DynConstLoad", L"CustomRangePower", L"6.1");
	m_fCustomRangePower = (float)_tstof(str);
	////////////////////////////////////////////////////////// �������ļ���ȡ���غɻ��в���end
}

/*
void CNHCConstLoadDlg::ExportEquCalChkInfo()
{
	// ��������������ݽ���Ŀ¼

	CString strPathOfDataExchange;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// ����2��
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// ���ݽ���Ŀ¼·��
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// �ж�·���Ƿ����,�����Խ��ж�д
	if (_taccess(strPathOfDataExchange, 06))
	{
		// ·��������

		// ����·��
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// ����·��ʧ��,����,������
			return;
		}
	}

	// ��ȡ����߱�ź��豸�ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strDynamometerModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

	// ��ȡ����Ա
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	CString str;
	CString strResult(L"[EquCalChkInfo]");

	// �����Ƿ���ɹ�����һ��
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce ? L"1" : L"0");
	strResult += str;

	// ����߱��
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strResult += str;

	// ����Ա
	str.Format(L"\r\nOperator=%s", strOperator);
	strResult += str;

	// �豸
	str.Format(L"\r\nEquipment=%s", strDynamometerModel);
	strResult += str;

	// ��Ŀ
	str.Format(L"\r\nItem=%s", L"�⹦�����ػ���");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	if (BST_CHECKED == m_rdoAsmRange.GetCheck())
	{
		strData.Format(L"ACDT25:%ums,ACDT40:%ums", m_unLowerRangeACDT, m_unHigherRangeACDT);
	}
	else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
	{
		strData.Format(L"ACDT24:%ums,ACDT40:%ums", m_unLowerRangeACDT, m_unHigherRangeACDT);
	}
	else if (BST_CHECKED == m_rdoLugdownRange.GetCheck())
	{
		strData.Format(L"ACDT40:%ums,ACDT56:%ums", m_unLowerRangeACDT, m_unHigherRangeACDT);
	}
	else
	{
		// Ĭ��ΪASM��Χ
		strData.Format(L"ACDT25:%ums,ACDT40:%ums", m_unLowerRangeACDT, m_unHigherRangeACDT);
	}
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strError;
	strError.Format(L"���:%.2f%%,%.2f%%", m_nLowerRangeREX100/100.0f, m_nHigherRangeREX100/100.0f);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	str.Format(L"\r\nJudgement=%s", (m_bIsLowerRangePass && m_bIsHigherRangePass) ? L"ͨ��" : L"��ͨ��");
	strResult += str;

	// д�뵽���ini�ļ�
	CString strPathOfEquCalChkInfo = strPathOfDataExchange + L"\\EquCalChkInfo.ini";
	if (0 == _taccess(strPathOfEquCalChkInfo, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfEquCalChkInfo);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfEquCalChkInfo, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfEquCalChkInfo, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}
*/
	
/*
void CNHCConstLoadDlg::ExportDemarcationLog(void)
{	
	CString strPathOfDataExchange;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPathOfDataExchange.GetBuffer(MAX_PATH), MAX_PATH);
	strPathOfDataExchange.ReleaseBuffer();
	// ����2��
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	strPathOfDataExchange = strPathOfDataExchange.Left(strPathOfDataExchange.ReverseFind(_T('\\')));
	// ���ݽ���Ŀ¼·��
	strPathOfDataExchange = strPathOfDataExchange + L"\\App_Data";

	// �ж�·���Ƿ����,�����Խ��ж�д
	if (_taccess(strPathOfDataExchange, 06))
	{
		// ·��������

		// ����·��
		if (!CreateDirectory(strPathOfDataExchange, NULL))
		{ 
			// ����·��ʧ��,����
			return;
		}
	}

	// ��ȡ���վ���
	CString strPathOfStationInfoFile = strPathOfDataExchange + L"\\StationInfo.ini";
	CSimpleIni siStationInfo(strPathOfStationInfoFile);
	CString strStationNumber = siStationInfo.GetString(L"StationInfo", L"StationNumber", L"");

	// ��ȡ����߱�ź��豸�ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentNumber = siLineInfo.GetString(L"LineInfo", L"DynamometerNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"DynamometerModel", L"");

	// ��ȡ����Ա
	CString strPathOfUserInfoFile = strPathOfDataExchange + L"\\UserInfo.ini";
	CSimpleIni siUserInfo(strPathOfUserInfoFile);
	CString strOperator = siUserInfo.GetString(L"UserInfo", L"Name", L"");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString str;
	CString strDemarcationLog(L"[DemarcationLog]");

	// �궨���
	str.Format(L"\r\nDemarcationNumber=%s-%s-%s", strStationNumber, strLineNumber, odt.Format(L"%Y%m%d%H%M%S"));
	strDemarcationLog += str;

	// ����߱��
	str.Format(L"\r\nLineNumber=%s", strLineNumber);
	strDemarcationLog += str;

	// ����Ա
	str.Format(L"\r\nOperator=%s", strOperator);
	strDemarcationLog += str;

	// �豸���
	str.Format(L"\r\nEquipmentNumber=%s", strEquipmentNumber);
	strDemarcationLog += str;

	// �豸�ͺ�
	str.Format(L"\r\nEquipmentModel=%s", strEquipmentModel);
	strDemarcationLog += str;

	// �豸����
	str.Format(L"\r\nEquipmentName=%s", L"���̲⹦��");
	strDemarcationLog += str;

	// �궨��Ŀ
	str.Format(L"\r\nDemarcationItem=%d", DYN_CL);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", (m_bIsLowerRangePass && m_bIsHigherRangePass)?L"1":L"0");
	strDemarcationLog += str;

	// д�뵽���ini�ļ�
	CString strPathOfDemarcationLog = strPathOfDataExchange + L"\\DemarcationLog.ini";
	if (0 == _taccess(strPathOfDemarcationLog, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfDemarcationLog);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfDemarcationLog, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfDemarcationLog, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strDemarcationLog);
	sfe.Close();
}
*/

void CNHCConstLoadDlg::ExportDynConstLoadResult(void)
{	
	CString str;
	CString strResult(L"[ResultOfDynConstLoad]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// �����Ƿ���ɹ�����һ��
	str.Format(L"\r\nIsOperationFinishedAtLeaseOnce=%s", m_bIsOperationFinishedAtLeaseOnce ? L"1" : L"0");
	strResult += str;

	// ��������,��λ:kg
	str.Format(L"\r\nDIW=%hu", m_usDIW);
	strResult += str;

	if (m_bLowerRangeLoadConstStep)
	{
		// ���ٶ�

		// �Ƿ�Pass
		str.Format(L"\r\nPass=%s", m_bIsLowerRangePass ? L"1" : L"0");
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nLowerVelocity=%.2f", m_fLowerVelocityOfLowerRange);
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nUpperVelocity=%.2f", m_fUpperVelocityOfLowerRange);
		strResult += str;

		// ���ع���
		str.Format(L"\r\nPowerStandardValue=%.2f", m_fLowerRangePower);
		strResult += str;

		// ACDT,��λ:ms
		str.Format(L"\r\nACDT=%u", m_unLowerRangeACDT);
		strResult += str;

		// CCDT,��λ:ms
		str.Format(L"\r\nCCDT=%u", m_unLowerRangeCCDT);
		strResult += str;

		// CDTAE,��λ:ms
		str.Format(L"\r\nCDTAE=%d", m_unLowerRangeACDT-m_unLowerRangeCCDT);
		strResult += str;

		// ������
		str.Format(L"\r\nCDTRE=%.2f", m_nLowerRangeREX100/100.0f);
		strResult += str;

		// ��������ֵ
		str.Format(L"\r\nCDTRELimit=%.2f", m_unLowerRangeRELimitX100/100.0f);
		strResult += str;
	}
	else if (m_bHigherRangeLoadConstStep)
	{
		// ���ٶ�

		// �Ƿ�Pass
		str.Format(L"\r\nPass=%s", m_bIsHigherRangePass ? L"1" : L"0");
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nLowerVelocity=%.2f", m_fLowerVelocityOfHigherRange);
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nUpperVelocity=%.2f", m_fUpperVelocityOfHigherRange);
		strResult += str;

		// ���ع���
		str.Format(L"\r\nPowerStandardValue=%.2f", m_fHigherRangePower);
		strResult += str;

		// ACDT,��λ:ms
		str.Format(L"\r\nACDT=%u", m_unHigherRangeACDT);
		strResult += str;

		// CCDT,��λ:ms
		str.Format(L"\r\nCCDT=%u", m_unHigherRangeCCDT);
		strResult += str;

		// CDTAE,��λ:ms
		str.Format(L"\r\nCDTAE=%d", m_unHigherRangeACDT-m_unHigherRangeCCDT);
		strResult += str;

		// ������
		str.Format(L"\r\nCDTRE=%.2f", m_nHigherRangeREX100/100.0f);
		strResult += str;

		// ��������ֵ
		str.Format(L"\r\nCDTRELimit=%.2f", m_unHigherRangeRELimitX100/100.0f);
		strResult += str;
	}
	else if (m_bCustomRangeLoadConstStep)
	{
		// �Զ����

		// �Ƿ�Pass
		str.Format(L"\r\nPass=%s", m_bIsCustomRangePass ? L"1" : L"0");
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nLowerVelocity=%.2f", m_fLowerVelocityOfCustomRange);
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nUpperVelocity=%.2f", m_fUpperVelocityOfCustomRange);
		strResult += str;

		// ���ع���
		str.Format(L"\r\nPowerStandardValue=%.2f", m_fCustomRangePower);
		strResult += str;

		// ACDT,��λ:ms
		str.Format(L"\r\nACDT=%u", m_unCustomRangeACDT);
		strResult += str;

		// CCDT,��λ:ms
		str.Format(L"\r\nCCDT=%u", m_unCustomRangeCCDT);
		strResult += str;

		// CDTAE,��λ:ms
		str.Format(L"\r\nCDTAE=%d", m_unCustomRangeACDT-m_unCustomRangeCCDT);
		strResult += str;

		// ������
		str.Format(L"\r\nCDTRE=%.2f", m_nCustomRangeREX100/100.0f);
		strResult += str;

		// ��������ֵ
		str.Format(L"\r\nCDTRELimit=%.2f", m_unCustomRangeRELimitX100/100.0f);
		strResult += str;
	}

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynConstLoad.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCConstLoadDlg::ExportDynConstLoadRTData()
{
	CString strRTData(L""), str(L"");

	int nCount = (int)(m_lsRTDataOfDynConstLoad.GetCount());

	////////////////////////////////////////////// ����begin
	strRTData.Format(L"\r\n[Sum]\r\nTotal=%d", nCount);
	////////////////////////////////////////////// ����end

	m_fInternalDamage = 0.0f;
	RTData rd;
	POSITION pos;
	for (int i=0; i<nCount; i++ )
	{			
		pos = m_lsRTDataOfDynConstLoad.FindIndex(i);
		rd = m_lsRTDataOfDynConstLoad.GetAt(pos);

		str.Format(_T("\r\n\r\n[%d]"), i+1);
		strRTData += str;

		// ʱ�����
		str.Format(_T("\r\nTime=%d"), rd.nTime);
		strRTData += str;

		// ��Ͳ���ٶ�
		str.Format(_T("\r\nVelocity=%.2f"), rd.fVelocity);
		strRTData += str;

		// ����
		str.Format(_T("\r\nPower=%.2f"), rd.fPower);
		strRTData += str;

		// ��
		str.Format(_T("\r\nForce=%.2f"), rd.fForce);
		strRTData += str;

		// ��������
		str.Format(_T("\r\nPLHP=%.2f"), rd.fPLHP);
		strRTData += str;
		// �������
		m_fInternalDamage += rd.fPLHP;

		// ָʾ����
		str.Format(_T("\r\nIHP=%.2f"), rd.fIHP);
		strRTData += str;
	}
	// ȡƽ����
	m_fInternalDamage = m_fInternalDamage / nCount;
	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"DemarcationRealTimeDataOfDynConstLoad.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strRTData);
	sfe.Close();
}

void CNHCConstLoadDlg::ExportUpDynConstLoadResult(void)
{
	CString str;
	CString strResult;

	//// ��ȡ��ǰʱ��
	//// �궨����
	//str.Format(L"\r\nDemarcationDate=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//strResult += str;

	//// ��������,��λ:kg
	//str.Format(L"\r\nDIW=%hu", m_usDIW);
	//strResult += str;

	m_odtEnd = COleDateTime::GetCurrentTime();

	if (m_bLowerRangeLoadConstStep)
	{
		// ��������
		strResult.AppendFormat(L"\r\n[LowerRangeLoadConst]");

		// ��������,��λ:kg
		str.Format(L"\r\nDIW=%hu", m_usDIW);
		strResult += str;

		// ���������Ƿ����
		str.Format(L"\r\nLowerRangeLoadConstFinish=%s", L"1");
		strResult += str;

		// ��ʼʱ��
		str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
		strResult += str;

		// ����ʱ��
		str.Format(L"\r\nEndTime=%s", m_odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
		strResult += str;

		// ����
		str.Format(L"\r\nInternalDamage=%.2f", m_fInternalDamage);
		strResult += str;

		// �Ƿ�Pass
		str.Format(L"\r\nPass=%s", m_bIsLowerRangePass ? L"1" : L"0");
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nLowerVelocity=%.2f", m_fLowerVelocityOfLowerRange);
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nUpperVelocity=%.2f", m_fUpperVelocityOfLowerRange);
		strResult += str;

		// ���ع���
		str.Format(L"\r\nPowerStandardValue=%.2f", m_fLowerRangePower);
		strResult += str;

		// ACDT,��λ:ms
		str.Format(L"\r\nACDT=%u", m_unLowerRangeACDT);
		strResult += str;

		// CCDT,��λ:ms
		str.Format(L"\r\nCCDT=%u", m_unLowerRangeCCDT);
		strResult += str;

		// CDTAE,��λ:ms
		str.Format(L"\r\nCDTAE=%d", m_unLowerRangeACDT-m_unLowerRangeCCDT);
		strResult += str;

		// ������
		str.Format(L"\r\nCDTRE=%.2f", m_nLowerRangeREX100/100.0f);
		strResult += str;

		// ��������ֵ
		str.Format(L"\r\nCDTRELimit=%.2f", m_unLowerRangeRELimitX100/100.0f);
		strResult += str;

		CString strPLHPType(L"1");
		if (BST_CHECKED == m_rdoAsmRange.GetCheck())
		{
			// ASM
			strPLHPType = L"1";
		}
		else if (BST_CHECKED == m_rdoVmasRange.GetCheck())
		{
			// VMAS
			strPLHPType = L"2";
		}
		else 
		{
			// LUGDOWN
			strPLHPType = L"3";

			for (int i=0;i<9;i++)
			{
				str.Format(L"\r\nLugdownRangeACDT30%d=%u", i, m_unLugdownRangeACDT30[i]);
				strResult += str;

				str.Format(L"\r\nLugdownRangeCCDT30%d=%u", i, m_unLugdownRangeCCDT30[i]);
				strResult += str;

				str.Format(L"\r\nLugdownRangeACDT20%d=%u", i, m_unLugdownRangeACDT20[i]);
				strResult += str;

				str.Format(L"\r\nLugdownRangeCCDT20%d=%u", i, m_unLugdownRangeCCDT20[i]);
				strResult += str;

				str.Format(L"\r\nLugdownRangeACDT10%d=%u", i, m_unLugdownRangeACDT10[i]);
				strResult += str;

				str.Format(L"\r\nLugdownRangeCCDT10%d=%u", i, m_unLugdownRangeCCDT10[i]);
				strResult += str;

				str.Format(L"\r\nLugdownPLHP%d=%.2f", i, m_fLugdownPLHP[i]);
				strResult += str;
			}
			for (int i=0; i<2;i++)
			{
				str.Format(L"\r\nGZLugdownRangeCCDT%d=%u", i, m_unGZLugdownRangeCCDT[i]);
				strResult += str;
				str.Format(L"\r\nGZLugdownRangeACDT%d=%u", i, m_unGZLugdownRangeACDT[i]);
				strResult += str;
			}
		}
		str.Format(L"\r\nRangeLoadConstType=%s", strPLHPType);
		strResult += str;


	}
	else if (m_bHigherRangeLoadConstStep)
	{
		// ��������
		strResult.AppendFormat(L"\r\n[HigherRangeLoadConst]");

		// ��������,��λ:kg
		str.Format(L"\r\nDIW=%hu", m_usDIW);
		strResult += str;

		// �����Ƿ����
		str.Format(L"\r\nHigherRangeLoadConstFinish=%s", L"1");
		strResult += str;

		// ��ʼʱ��
		str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
		strResult += str;

		// ����ʱ��
		str.Format(L"\r\nEndTime=%s", m_odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
		strResult += str;

		// ����
		str.Format(L"\r\nInternalDamage=%.2f", m_fInternalDamage);
		strResult += str;

		// �Ƿ�Pass
		str.Format(L"\r\nPass=%s", m_bIsHigherRangePass ? L"1" : L"0");
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nLowerVelocity=%.2f", m_fLowerVelocityOfHigherRange);
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nUpperVelocity=%.2f", m_fUpperVelocityOfHigherRange);
		strResult += str;

		// ���ع���
		str.Format(L"\r\nPowerStandardValue=%.2f", m_fHigherRangePower);
		strResult += str;

		// ACDT,��λ:ms
		str.Format(L"\r\nACDT=%u", m_unHigherRangeACDT);
		strResult += str;

		// CCDT,��λ:ms
		str.Format(L"\r\nCCDT=%u", m_unHigherRangeCCDT);
		strResult += str;

		// CDTAE,��λ:ms
		str.Format(L"\r\nCDTAE=%d", m_unHigherRangeACDT-m_unHigherRangeCCDT);
		strResult += str;

		// ������
		str.Format(L"\r\nCDTRE=%.2f", m_nHigherRangeREX100/100.0f);
		strResult += str;

		// ��������ֵ
		str.Format(L"\r\nCDTRELimit=%.2f", m_unHigherRangeRELimitX100/100.0f);
		strResult += str;
	}
	else if (m_bCustomRangeLoadConstStep)
	{
		// �Զ�������
		strResult.AppendFormat(L"\r\n[CustomRangeLoadConst]");

		// ��������,��λ:kg
		str.Format(L"\r\nDIW=%hu", m_usDIW);
		strResult += str;

		// �����Ƿ����
		str.Format(L"\r\nCustomRangeLoadConstFinish=%s", L"1");
		strResult += str;

		// ��ʼʱ��
		str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
		strResult += str;

		// ����ʱ��
		str.Format(L"\r\nEndTime=%s", m_odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
		strResult += str;

		// ����
		str.Format(L"\r\nInternalDamage=%.2f", m_fInternalDamage);
		strResult += str;

		// �Ƿ�Pass
		str.Format(L"\r\nPass=%s", m_bIsCustomRangePass ? L"1" : L"0");
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nLowerVelocity=%.2f", m_fLowerVelocityOfCustomRange);
		strResult += str;

		// ���ٵ�
		str.Format(L"\r\nUpperVelocity=%.2f", m_fUpperVelocityOfCustomRange);
		strResult += str;

		// ���ع���
		str.Format(L"\r\nPowerStandardValue=%.2f", m_fCustomRangePower);
		strResult += str;

		// ACDT,��λ:ms
		str.Format(L"\r\nACDT=%u", m_unCustomRangeACDT);
		strResult += str;

		// CCDT,��λ:ms
		str.Format(L"\r\nCCDT=%u", m_unCustomRangeCCDT);
		strResult += str;

		// CDTAE,��λ:ms
		str.Format(L"\r\nCDTAE=%d", m_unCustomRangeACDT-m_unCustomRangeCCDT);
		strResult += str;

		// ������
		str.Format(L"\r\nCDTRE=%.2f", m_nCustomRangeREX100/100.0f);
		strResult += str;

		// ��������ֵ
		str.Format(L"\r\nCDTRELimit=%.2f", m_unCustomRangeRELimitX100/100.0f);
		strResult += str;
	}

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CStdioFileEx::modeWriteUnicode);
	// ׷��д��
	sfe.SeekToEnd();
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCConstLoadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		// �򿪴��ڣ�������ͨѶ

		KillTimer(1);
		OpenSerialPortAndTestCommunication();
		//TRACE("OnTimer 1\n");
	}
	else if (2 == nIDEvent)
	{
		RefreshRealTimeData();
		//TRACE("OnTimer 2\n");
	}
	else if (3 == nIDEvent)
	{
		DisplayConstantLoadTestProcess();
		//TRACE("OnTimer 3\n");
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCConstLoadDlg::DisplayConstantLoadTestProcess(void)
{
	switch (m_pNHCConstLoadThread->GetConstLoadState())
	{
	case NHC_CL_STATE_NOT_START:
		{}
		break;
	case NHC_CL_STATE_READY:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڳ�ʼ�����Բ���...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
		}
		break;
	case NHC_CL_STATE_LIFTBEAM_DOWN:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڽ�������...");
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
		}
		break;
	case NHC_CL_STATE_ACCELERATION:
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڼ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCConstLoadThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCConstLoadThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
		}
		break;
	case NHC_CL_STATE_SLIP:
		{
			// ֻ��¼��һ�ν���ʱ��
			if (m_bWriteTimer)
			{
				m_odtStart = COleDateTime::GetCurrentTime();
				m_bWriteTimer = false;
			}
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���ڻ���...");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCConstLoadThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCConstLoadThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// ��¼ʵʱ����
			AddRTData();
			// ��ʾ���ػ���ʱ�䣬��λms��>s
			if (m_bLowerRangeLoadConstStep)
			{
				int nRangeACDT = m_unLowerRangeACDT;
				// ʵ�ʻ��д��������������
				if (m_nRandomError > nRangeACDT)
				{
					m_unLowerRangeACDT = m_pNHCConstLoadThread->GetConstLoadTime();
				}
				str.Format(L"%.3f", m_unLowerRangeACDT / 1000.f);
				GetDlgItem(IDC_STATIC_LOWER_RANGE_ACDT)->SetWindowTextW(str);
			}
			else if (m_bHigherRangeLoadConstStep)
			{
				int nRangeACDT = m_unHigherRangeACDT;
				// ʵ�ʻ��д��������������
				if (m_nRandomError > nRangeACDT)
				{
					m_unHigherRangeACDT = m_pNHCConstLoadThread->GetConstLoadTime();
				}
				str.Format(L"%.3f", m_unHigherRangeACDT / 1000.f);
				GetDlgItem(IDC_STATIC_HIGHER_RANGE_ACDT)->SetWindowTextW(str);
			}
			else if(m_bCustomRangeLoadConstStep)
			{
				int nRangeACDT = m_unCustomRangeACDT;
				// ʵ�ʻ��д��������������
				if (m_nRandomError > nRangeACDT)
				{
					m_unCustomRangeACDT = m_pNHCConstLoadThread->GetConstLoadTime();
				}
				str.Format(L"%.3f", m_unCustomRangeACDT / 1000.f);
				GetDlgItem(IDC_STATIC_CUSTOM_RANGE_ACDT)->SetWindowTextW(str);					
			}
		}
		break;
	case NHC_CL_STATE_FINISHED:
		{
			Precomputation();

			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"�������");
			CString str(L"");
			str.Format(L"%.2f", m_pNHCConstLoadThread->GetPValue());
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
			str.Format(L"%.2f", m_pNHCConstLoadThread->GetVValue());
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
			// ��ʾ���Խ��
			DisplayTestResult();

			//ExportEquCalChkInfo();			//�������
			//ExportDemarcationLog();			//�����궨��¼
			ExportDynConstLoadResult();		//�������
			ExportDynConstLoadRTData();		//������������
			ExportUpDynConstLoadResult();	// ����Up����
			m_nRandomError = 0;
			// ֹͣˢ�¼��ػ�������
			KillTimer(3);
			// �ָ��ؼ�
			RestoreCtrls();
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);

			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦�����ػ������", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_DYN_CL_FINISH, wchInfo, sizeof(wchInfo));
			}
		}
		break;
	case NHC_CL_STATE_ERROR:
		{
			if (NHC_CL_COMMUNICATION_ERROR == m_pNHCConstLoadThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"������ֹ,��⹦��ͨѶ��ʱ");
			}
			else if (NHC_CL_START_MMTIMER_FAIL == m_pNHCConstLoadThread->GetLastErrorCode())
			{
				GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"������ֹ,������ʱ��ʧ��");
			}
			GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
			GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
			// ֹͣˢ�¼��ػ�������
			KillTimer(3);
			// �ָ��ؼ�
			RestoreCtrls();
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
		}
		break;
	default:
		break;
	}
}

void CNHCConstLoadDlg::DisplayTestResult(void)
{
	if (m_bLowerRangeLoadConstStep)
	{
		m_bLowerRangeTestFinishedAtLeastOnce = true;
		m_nLowerRangeREX100 = int(m_unLowerRangeACDT-m_unLowerRangeCCDT);
		m_nLowerRangeREX100 = int(m_nLowerRangeREX100 * 10000.0f / m_unLowerRangeCCDT + 0.5f);
		CString str(L"");
		str.Format(L"%.2f", m_nLowerRangeREX100/100.0f);
		GetDlgItem(IDC_STATIC_LOWER_RANGE_RE)->SetWindowTextW(str);
		UINT unREX100 = (UINT)abs(m_nLowerRangeREX100);
		if (unREX100 > m_unLowerRangeRELimitX100)
		{
			m_bIsLowerRangePass = false;
			GetDlgItem(IDC_STATIC_LOWER_RANGE_RESULT)->SetWindowTextW(L"���ϸ�");
		}
		else
		{
			m_bIsLowerRangePass = true;
			GetDlgItem(IDC_STATIC_LOWER_RANGE_RESULT)->SetWindowTextW(L"�ϸ�");
		}
		SetStepIcon(IDC_ICON_LOWER_RANGE_STATE,3);
	}
	else if (m_bHigherRangeLoadConstStep)
	{
		m_bHigherRangeTestFinishedAtLeastOnce = true;
		m_nHigherRangeREX100 = int(m_unHigherRangeACDT-m_unHigherRangeCCDT);
		m_nHigherRangeREX100 = int(m_nHigherRangeREX100 * 10000.0f / m_unHigherRangeCCDT + 0.5f);
		CString str(L"");
		str.Format(L"%.2f", m_nHigherRangeREX100/100.0f);
		GetDlgItem(IDC_STATIC_HIGHER_RANGE_RE)->SetWindowTextW(str);
		UINT unREX100 = (UINT)abs(m_nHigherRangeREX100);
		if (unREX100 > m_unHigherRangeRELimitX100)
		{
			m_bIsHigherRangePass = false;
			GetDlgItem(IDC_STATIC_HIGHER_RANGE_RESULT)->SetWindowTextW(L"���ϸ�");
		}
		else
		{
			m_bIsHigherRangePass = true;
			GetDlgItem(IDC_STATIC_HIGHER_RANGE_RESULT)->SetWindowTextW(L"�ϸ�");
		}
		SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE,3);
	}
	else if (m_bCustomRangeLoadConstStep)
	{
		m_bCustomRangeTestFinishedAtLeastOnce = true;
		m_nCustomRangeREX100 = int(m_unCustomRangeACDT-m_unCustomRangeCCDT);
		m_nCustomRangeREX100 = int(m_nCustomRangeREX100 * 10000.0f / m_unCustomRangeCCDT + 0.5f);
		CString str(L"");
		str.Format(L"%.2f", m_nCustomRangeREX100/100.0f);
		GetDlgItem(IDC_STATIC_CUSTOM_RANGE_RE)->SetWindowTextW(str);
		UINT unREX100 = (UINT)abs(m_nCustomRangeREX100);
		if (unREX100 > m_unCustomRangeRELimitX100)
		{
			m_bIsCustomRangePass = false;
			GetDlgItem(IDC_STATIC_CUSTOM_RANGE_RESULT)->SetWindowTextW(L"���ϸ�");
		}
		else
		{
			m_bIsCustomRangePass = true;
			GetDlgItem(IDC_STATIC_CUSTOM_RANGE_RESULT)->SetWindowTextW(L"�ϸ�");
		}
		SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE,3);
	}

	// ���ߵ��������Ƿ��Ѿ����
	// ������Ѿ���ɣ���ʾ���ղ��Խ��
	if (m_bLowerRangeTestFinishedAtLeastOnce && m_bHigherRangeTestFinishedAtLeastOnce)
	{
		m_bIsOperationFinishedAtLeaseOnce = true;

		if (m_bIsLowerRangePass && m_bIsHigherRangePass)
		{
			GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"ͨ��");
		}
		else
		{
			GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"��ͨ��");
		}
		//��������Զ�������
		if(m_bCustomRangeTestFinishedAtLeastOnce)
		{
			if (m_bIsLowerRangePass && m_bIsHigherRangePass && m_bIsCustomRangePass)
			{
				GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"ͨ��");
			}
			else
			{
				GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"��ͨ��");
			}	
		}
	}
	else
	{
		m_bIsOperationFinishedAtLeaseOnce = false;
	}
}


LRESULT CNHCConstLoadDlg::OnConstLoadTestFinished(WPARAM wParam, LPARAM lParam)
{
	// �������������

	//if (BST_CHECKED == m_chkLowerRange.GetCheck())
	//{
	//	m_bLowerRangeTestFinishedAtLeastOnce = true;
	//	m_nLowerRangeREX100 = int(m_unLowerRangeACDT-m_unLowerRangeCCDT);
	//	m_nLowerRangeREX100 = int(m_nLowerRangeREX100 * 10000.0f / m_unLowerRangeCCDT + 0.5f);
	//	CString str(L"");
	//	str.Format(L"%.2f", m_nLowerRangeREX100/100.0f);
	//	GetDlgItem(IDC_STATIC_LOWER_RANGE_RE)->SetWindowTextW(str);
	//	UINT unREX100 = (UINT)abs(m_nLowerRangeREX100);
	//	if (unREX100 > m_unLowerRangeRELimitX100)
	//	{
	//		m_bIsLowerRangePass = false;
	//		GetDlgItem(IDC_STATIC_LOWER_RANGE_RESULT)->SetWindowTextW(L"���ϸ�");
	//	}
	//	else
	//	{
	//		m_bIsLowerRangePass = true;
	//		GetDlgItem(IDC_STATIC_LOWER_RANGE_RESULT)->SetWindowTextW(L"�ϸ�");
	//	}
	//}
	//else
	//{
	//	m_bHigherRangeTestFinishedAtLeastOnce = true;
	//	m_nHigherRangeREX100 = int(m_unHigherRangeACDT-m_unHigherRangeCCDT);
	//	m_nHigherRangeREX100 = int(m_nHigherRangeREX100 * 10000.0f / m_unHigherRangeCCDT + 0.5f);
	//	CString str(L"");
	//	str.Format(L"%.2f", m_nHigherRangeREX100/100.0f);
	//	GetDlgItem(IDC_STATIC_HIGHER_RANGE_RE)->SetWindowTextW(str);
	//	UINT unREX100 = (UINT)abs(m_nHigherRangeREX100);
	//	if (unREX100 > m_unHigherRangeRELimitX100)
	//	{
	//		m_bIsHigherRangePass = false;
	//		GetDlgItem(IDC_STATIC_HIGHER_RANGE_RESULT)->SetWindowTextW(L"���ϸ�");
	//	}
	//	else
	//	{
	//		m_bIsHigherRangePass = true;
	//		GetDlgItem(IDC_STATIC_HIGHER_RANGE_RESULT)->SetWindowTextW(L"�ϸ�");
	//	}
	//}

	//// ���ߵ��������Ƿ��Ѿ����
	//// ������Ѿ���ɣ���ʾ���ղ��Խ��
	//if (m_bLowerRangeTestFinishedAtLeastOnce && m_bHigherRangeTestFinishedAtLeastOnce)
	//{
	//	if (m_bIsLowerRangePass && m_bIsHigherRangePass)
	//	{
	//		GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"ͨ��");
	//	}
	//	else
	//	{
	//		GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"��ͨ��");
	//	}
	//}
	return 0;
}


void CNHCConstLoadDlg::RefreshRealTimeData(void)
{
	// ˢ��������
	// ͬʱ����ͨѶ����
	// 500����ִ��һ��

	CString str(L"");
	//if (DYN_WR_SUCCEED == m_pNHCConstLoadThread->GetRealTimeData_Asyn())
	m_pNHCConstLoadThread->GetRealTimeData_Asyn(NULL);
	{
		// ͨѶ�ɹ�
		str.Format(L"%.2f", m_pNHCConstLoadThread->GetPValue());
		GetDlgItem(IDC_STATIC_P)->SetWindowTextW(str);
		str.Format(L"%.2f", m_pNHCConstLoadThread->GetVValue());
		GetDlgItem(IDC_STATIC_V)->SetWindowTextW(str);
	}
	//else
	//{
	//	// ͨѶʧ��
	//	GetDlgItem(IDC_STATIC_P)->SetWindowTextW(L"--");
	//	GetDlgItem(IDC_STATIC_V)->SetWindowTextW(L"--");
	//}
}

void CNHCConstLoadDlg::EnableRefreshRealTimeData(bool bEnable)
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

void CNHCConstLoadDlg::EnableBasicOperation(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_LIFTBEAM_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LIFTBEAM_DOWN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOTOR_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MOTOR_OFF)->EnableWindow(bEnable);
}

void CNHCConstLoadDlg::ClearLowerRangeResult(void)
{
	m_unLowerRangeCCDT = 0;
	m_unLowerRangeACDT = 0;
	m_nLowerRangeREX100 = 0;
	m_bIsLowerRangePass = false;
	m_bLowerRangeTestFinishedAtLeastOnce = false;
	GetDlgItem(IDC_STATIC_LOWER_RANGE_CCDT)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_LOWER_RANGE_ACDT)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_LOWER_RANGE_RE)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_LOWER_RANGE_RESULT)->SetWindowTextW(L"--");
}

void CNHCConstLoadDlg::ClearHigherRangeResult(void)
{
	m_unHigherRangeACDT = 0;
	m_unHigherRangeCCDT = 0;
	m_nHigherRangeREX100 = 0;
	m_bIsHigherRangePass = false;
	m_bHigherRangeTestFinishedAtLeastOnce = false;
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_CCDT)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_ACDT)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_RE)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_RESULT)->SetWindowTextW(L"--");
}

void CNHCConstLoadDlg::ClearCustomRangeResult(void)
{
	m_unCustomRangeACDT = 0;
	m_unCustomRangeCCDT = 0;
	m_nCustomRangeREX100 = 0;
	m_bIsCustomRangePass = false;
	m_bCustomRangeTestFinishedAtLeastOnce = false;
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_CCDT)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_ACDT)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_RE)->SetWindowTextW(L"--");
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_RESULT)->SetWindowTextW(L"--");
}

void CNHCConstLoadDlg::OnBnClickedRadioAsmRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ASM����
	m_fLowerVelocityOfLowerRange = 15.0f;
	m_fUpperVelocityOfLowerRange = 35.0f;
	m_fLowerVelocityOfHigherRange = 30.0f;
	m_fUpperVelocityOfHigherRange = 50.0f;
	m_fLowerVelocityOfCustomRange = 30.0f;
	m_fUpperVelocityOfCustomRange = 50.0f;
	m_fLowerRangePower = 4.0f;
	m_fHigherRangePower = 11.0f;
	m_fCustomRangePower = 18.0f;
	m_unLowerRangeRELimitX100 = 400;
	m_unHigherRangeRELimitX100 = 200;
	m_unCustomRangeRELimitX100 = 400;

	CString strTemp;
	strTemp.Format(L"%.1f~%.1f", m_fUpperVelocityOfLowerRange, m_fLowerVelocityOfLowerRange);
	GetDlgItem(IDC_STATIC_LOWER_RANGE)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f~%.1f", m_fUpperVelocityOfHigherRange, m_fLowerVelocityOfHigherRange);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fUpperVelocityOfCustomRange);	
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_HIGH_SPEED)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fLowerVelocityOfCustomRange);	
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_LOW_SPEED)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fLowerRangePower);
	m_edLowerRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fHigherRangePower);
	m_edHigherRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fCustomRangePower);
	m_edCustomRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unLowerRangeRELimitX100/100.0f, m_unLowerRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_LOWER_RANGE_LIMIT)->SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unHigherRangeRELimitX100/100.0f, m_unHigherRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_LIMIT)->SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unCustomRangeRELimitX100/100.0f, m_unCustomRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_LIMIT)->SetWindowTextW(strTemp);

	m_rdoAsmRange.SetCheck(BST_CHECKED);
	m_rdoVmasRange.SetCheck(BST_UNCHECKED);
	m_rdoLugdownRange.SetCheck(BST_UNCHECKED);
	SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE, 0);

	// �л�����ʱ��������
	m_bIsOperationFinishedAtLeaseOnce = false;
	ClearLowerRangeResult();
	ClearHigherRangeResult();
	ClearCustomRangeResult();
	GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"--");
}

void CNHCConstLoadDlg::OnBnClickedRadioVmasRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// VMAS����
	m_fLowerVelocityOfLowerRange = 15.0f;
	m_fUpperVelocityOfLowerRange = 35.0f;
	m_fLowerVelocityOfHigherRange = 30.0f;
	m_fUpperVelocityOfHigherRange = 50.0f;
	m_fLowerVelocityOfCustomRange = 30.0f;
	m_fUpperVelocityOfCustomRange = 50.0f;
	m_fLowerRangePower = 4.0f;
	m_fHigherRangePower = 11.0f;
	m_fCustomRangePower = 18.0f;
	m_unLowerRangeRELimitX100 = 400;
	m_unHigherRangeRELimitX100 = 200;
	m_unCustomRangeRELimitX100 = 400;

	CString strTemp;
	strTemp.Format(L"%.1f~%.1f", m_fUpperVelocityOfLowerRange, m_fLowerVelocityOfLowerRange);
	GetDlgItem(IDC_STATIC_LOWER_RANGE)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f~%.1f", m_fUpperVelocityOfHigherRange, m_fLowerVelocityOfHigherRange);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fUpperVelocityOfCustomRange);	
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_HIGH_SPEED)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fLowerVelocityOfCustomRange);	
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_LOW_SPEED)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fLowerRangePower);
	m_edLowerRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fHigherRangePower);
	m_edHigherRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fCustomRangePower);
	m_edCustomRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unLowerRangeRELimitX100/100.0f, m_unLowerRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_LOWER_RANGE_LIMIT)->SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unHigherRangeRELimitX100/100.0f, m_unHigherRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_LIMIT)->SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unCustomRangeRELimitX100/100.0f, m_unCustomRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_LIMIT)->SetWindowTextW(strTemp);

	m_rdoAsmRange.SetCheck(BST_UNCHECKED);
	m_rdoVmasRange.SetCheck(BST_CHECKED);
	m_rdoLugdownRange.SetCheck(BST_UNCHECKED);
	SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE, 0);

	// �л�����ʱ��������
	m_bIsOperationFinishedAtLeaseOnce = false;
	ClearLowerRangeResult();
	ClearHigherRangeResult();
	ClearCustomRangeResult();
	GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"--");
}

void CNHCConstLoadDlg::OnBnClickedRadioLugdownRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// LUGDOWN����
	m_fLowerVelocityOfLowerRange = 10.0f;
	m_fUpperVelocityOfLowerRange = 80.0f;
	m_fLowerVelocityOfHigherRange = 10.0f;
	m_fUpperVelocityOfHigherRange = 80.0f;
	m_fUpperVelocityOfCustomRange = 80.0f;
	m_fLowerVelocityOfCustomRange = 10.0f;
	m_fLowerRangePower = 10.0f;
	m_fHigherRangePower = 20.0f;
	m_fCustomRangePower = 30.0f;
	m_unLowerRangeRELimitX100 = 200;
	m_unHigherRangeRELimitX100 = 200;
	m_unCustomRangeRELimitX100 = 400;

	CString strTemp;
	strTemp.Format(L"%.1f~%.1f", m_fUpperVelocityOfLowerRange, m_fLowerVelocityOfLowerRange);
	GetDlgItem(IDC_STATIC_LOWER_RANGE)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f~%.1f", m_fUpperVelocityOfHigherRange, m_fLowerVelocityOfHigherRange);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fUpperVelocityOfCustomRange);	
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_HIGH_SPEED)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fLowerVelocityOfCustomRange);	
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_LOW_SPEED)->SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fLowerRangePower);
	m_edLowerRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fHigherRangePower);
	m_edHigherRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"%.1f", m_fCustomRangePower);
	m_edCustomRangePower.SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unLowerRangeRELimitX100/100.0f, m_unLowerRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_LOWER_RANGE_LIMIT)->SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unHigherRangeRELimitX100/100.0f, m_unHigherRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_HIGHER_RANGE_LIMIT)->SetWindowTextW(strTemp);
	strTemp.Format(L"[-%.2f,%.2f]", m_unCustomRangeRELimitX100/100.0f, m_unCustomRangeRELimitX100/100.0f);
	GetDlgItem(IDC_STATIC_CUSTOM_RANGE_LIMIT)->SetWindowTextW(strTemp);

	m_rdoAsmRange.SetCheck(BST_UNCHECKED);
	m_rdoVmasRange.SetCheck(BST_UNCHECKED);
	m_rdoLugdownRange.SetCheck(BST_CHECKED);
	SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE, 0);

	// �л�����ʱ��������
	m_bIsOperationFinishedAtLeaseOnce = false;
	ClearLowerRangeResult();
	ClearHigherRangeResult();
	ClearCustomRangeResult();
	GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"--");
}

void CNHCConstLoadDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	EnableRefreshRealTimeData(false);

	OnCancel();
}

void CNHCConstLoadDlg::OnBnClickedButtonLiftbeamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCConstLoadThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCConstLoadThread->GetVValue() > 10e-6)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ʱ�������������", L"��������", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"��������", MB_OK|MB_ICONERROR);
		return;
	}

	m_pNHCConstLoadThread->LiftBeamUp();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCConstLoadDlg::OnBnClickedButtonLiftbeamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	m_pNHCConstLoadThread->LiftBeamDown();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCConstLoadDlg::OnBnClickedButtonMotorOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCConstLoadThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCConstLoadThread->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"�����", MB_OK|MB_ICONERROR);
		return;
	}

	// ��������״̬
	dw = m_pNHCConstLoadThread->GetStatus();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCConstLoadThread->IsTheStatusBitSignaled(0))
		{
			// �������Ѿ�����

			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"�뽵�;�������,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
		else if (!(m_pNHCConstLoadThread->IsTheStatusBitSignaled(0)) && !(m_pNHCConstLoadThread->IsTheStatusBitSignaled(1)))
		{
			// ���������ڽ��»�����������

			// ���ͽ�����������
			m_pNHCConstLoadThread->LiftBeamDown();

			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ⱦ�������ȫ���º�,�ٿ����", L"�����", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"�����", MB_OK|MB_ICONERROR);
		return;
	}

	// ���а�ȫ��ʾ
	if (IDYES == MessageBox(L"��Ͳ����ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"�����", MB_YESNO|MB_ICONWARNING))
	{
		m_pNHCConstLoadThread->LiftBeamDown();
		m_pNHCConstLoadThread->MotorOn();
	}
	else
	{
		MessageBox(L"�������ȡ��", L"�����", MB_OK|MB_ICONWARNING);
	}

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCConstLoadDlg::OnBnClickedButtonMotorOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	m_pNHCConstLoadThread->MotorOff();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);

	EndWaitCursor();
}

void CNHCConstLoadDlg::OnBnClickedButtonStartLowerRangeTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	CString str(L"");

	// �������ļ��ع��ʷ�Χ
	// ��������
	m_edLowerRangePower.GetWindowTextW(str);
	m_fLowerRangePower = (float)_wtof(str);
	if (m_fLowerRangePower<4.0f || m_fLowerRangePower>10.0f)
	{
		MessageBoxW(L"��Ϊ���ٻ���������дָ�����ع���(4~10kW)", L"���ػ���", MB_OK|MB_ICONINFORMATION);
		m_edLowerRangePower.SetFocus();
		return;
	}

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦�����ػ��У����٣���ʼ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_CL_START_LOW, wchInfo, sizeof(wchInfo));
	}

	////////////////////////////////////////////////////////// // ��¼�������ֵ�������ļ�begin
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);
	CSimpleIni si(strPath);
	str.Format(L"%.2f", m_fLowerRangePower);
	si.SetString(L"DynConstLoad", L"LowerRangePower", str);
	////////////////////////////////////////////////////////// // ��¼�������ֵ�������ļ�end

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCConstLoadThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCConstLoadThread->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ�ʼ����", L"���ػ���", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"���ػ���", MB_OK|MB_ICONERROR);
		return;
	}

	// ��ȫ��ʾ���⹦����Ͳ��Ҫת��
	if (IDYES == MessageBox(L"���Թ��̹�Ͳ�������ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"���ػ���", MB_YESNO|MB_ICONWARNING))
	{
		//m_chkLowerRange.SetCheck(BST_CHECKED);
		//m_chkHigherRange.SetCheck(BST_UNCHECKED);
		SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 1);
		SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
		m_bLowerRangeLoadConstStep = true;
		m_bHigherRangeLoadConstStep = false;
		m_rdoAsmRange.EnableWindow(FALSE);
		m_rdoVmasRange.EnableWindow(FALSE);
		m_rdoLugdownRange.EnableWindow(FALSE);
		m_edLowerRangePower.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// ���ʵʱ������Ϣ
		m_lsRTDataOfDynConstLoad.RemoveAll();
		// ������ٶν��
		ClearLowerRangeResult();
		// ����ܽ��
		GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"--");
		// ��ֹ��������
		EnableBasicOperation(false);
		// ���㲢��ʾ����ʱ��
		m_unLowerRangeCCDT = CalculateCCTD(m_usDIW, m_fUpperVelocityOfLowerRange, m_fLowerVelocityOfLowerRange, m_fLowerRangePower);
		str.Format(L"%.3f", m_unLowerRangeCCDT/1000.0f);
		GetDlgItem(IDC_STATIC_LOWER_RANGE_CCDT)->SetWindowTextW(str);
		m_nRandomError = RandomError(m_unLowerRangeCCDT);
		m_bWriteTimer = true;
		LugdownRangeACDT();
		// ��ֹ�˳����Խ���
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		// ��ʼˢ�¼�����
		SetTimer(3, 200, NULL);
		m_pNHCConstLoadThread->StartConstLoadTest_Asyn(m_fUpperVelocityOfLowerRange, m_fLowerVelocityOfLowerRange, m_fLowerRangePower, m_hWnd);
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		MessageBox(L"���Ա�ȡ��", L"���ػ���", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
}

void CNHCConstLoadDlg::OnBnClickedButtonStartHigherRangeTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	CString str(L"");

	// �������ļ��ع��ʷ�Χ
	// ��������
	m_edHigherRangePower.GetWindowTextW(str);
	m_fHigherRangePower = (float)_wtof(str);
	if (m_fHigherRangePower<10.0f || m_fHigherRangePower>20.0f)
	{
		MessageBoxW(L"��Ϊ���ٻ���������дָ�����ع���(10~20kW)", L"���ػ���", MB_OK|MB_ICONINFORMATION);
		m_edHigherRangePower.SetFocus();
		return;
	}

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦�����ػ��У����٣���ʼ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_CL_START_HIGH, wchInfo, sizeof(wchInfo));
	}
	////////////////////////////////////////////////////////// // ��¼�������ֵ�������ļ�begin
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);
	CSimpleIni si(strPath);
	str.Format(L"%.2f", m_fHigherRangePower);
	si.SetString(L"DynConstLoad", L"HigherRangePower", str);
	////////////////////////////////////////////////////////// // ��¼�������ֵ�������ļ�end

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCConstLoadThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCConstLoadThread->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ�ʼ����", L"���ػ���", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"���ػ���", MB_OK|MB_ICONERROR);
		return;
	}

	// ��ȫ��ʾ���⹦����Ͳ��Ҫת��
	if (IDYES == MessageBox(L"���Թ��̹�Ͳ�������ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"���ػ���", MB_YESNO|MB_ICONWARNING))
	{
		//m_chkLowerRange.SetCheck(BST_UNCHECKED);
		//m_chkHigherRange.SetCheck(BST_CHECKED);
		SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
		SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 1);
		m_bLowerRangeLoadConstStep = false;
		m_bHigherRangeLoadConstStep = true;
		m_rdoAsmRange.EnableWindow(FALSE);
		m_rdoVmasRange.EnableWindow(FALSE);
		m_rdoLugdownRange.EnableWindow(FALSE);
		m_edHigherRangePower.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// ���ʵʱ������Ϣ
		m_lsRTDataOfDynConstLoad.RemoveAll();
		// ������ٶν��
		ClearHigherRangeResult();
		// ����ܽ��
		GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"--");
		// ��ֹ��������
		EnableBasicOperation(false);
		// ���㲢��ʾ����ʱ��
		m_unHigherRangeCCDT = CalculateCCTD(m_usDIW, m_fUpperVelocityOfHigherRange, m_fLowerVelocityOfHigherRange, m_fHigherRangePower);
		str.Format(L"%.3f", m_unHigherRangeCCDT/1000.0f);
		GetDlgItem(IDC_STATIC_HIGHER_RANGE_CCDT)->SetWindowTextW(str);
		m_nRandomError = RandomError(m_unHigherRangeCCDT);
		m_bWriteTimer = true;
		// ��ֹ�˳����Խ���
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		// ��ʼˢ�¼�����
		SetTimer(3, 200, NULL);
		m_pNHCConstLoadThread->StartConstLoadTest_Asyn(m_fUpperVelocityOfHigherRange, m_fLowerVelocityOfHigherRange, m_fHigherRangePower);
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		MessageBox(L"���Ա�ȡ��", L"���ػ���", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
}

void CNHCConstLoadDlg::OnBnClickedButtonStartCustomRangeTest()
{
	BeginWaitCursor();

	CString str(L"");

	// �������ļ��ع��ʷ�Χ
	// �Զ�������
	m_edCustomRangePower.GetWindowTextW(str);
	m_fCustomRangePower = (float)_wtof(str);
	if (m_fCustomRangePower<15.0f || m_fCustomRangePower>30.0f)
	{
		MessageBoxW(L"��Ϊ�Զ��廬��������дָ�����ع���(15~30kW)", L"���ػ���", MB_OK|MB_ICONINFORMATION);
		m_edCustomRangePower.SetFocus();
		return;
	}

	// ***********************�Զ����ٶ�����Լ��*********************************
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_LOW_SPEED)->GetWindowTextW(str);
	m_fLowerVelocityOfCustomRange = (float)_wtof(str);
	if (m_fLowerVelocityOfCustomRange<10.0f || m_fLowerVelocityOfCustomRange>90.0f)
	{
		MessageBoxW(L"����дָ�������ٶ�(10~90km/h)", L"���ػ���", MB_OK|MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT_CUSTOM_RANGE_LOW_SPEED)->SetFocus();
		return;
	}
	GetDlgItem(IDC_EDIT_CUSTOM_RANGE_HIGH_SPEED)->GetWindowTextW(str);
	m_fUpperVelocityOfCustomRange = (float)_wtof(str);
	if (m_fUpperVelocityOfCustomRange<10.0f || m_fUpperVelocityOfCustomRange>90.0f)
	{
		MessageBoxW(L"����дָ�������ٶ�(10~90km/h)", L"���ػ���", MB_OK|MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT_CUSTOM_RANGE_HIGH_SPEED)->SetFocus();
		return;
	}
	if (m_fLowerVelocityOfCustomRange >= m_fUpperVelocityOfCustomRange)
	{
		MessageBoxW(L"�����ٶȲ���С�ڻ���ڵ����ٶ�", L"���ػ���", MB_OK|MB_ICONINFORMATION);
		return;
	}
	// ***********************�Զ����ٶ��������*********************************


	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦�����ػ��У��Զ��壩��ʼ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_CL_START_HIGH, wchInfo, sizeof(wchInfo));
	}
	////////////////////////////////////////////////////////// // ��¼�������ֵ�������ļ�begin
	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";
	// ȥ�������ļ�ֻ������
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);
	CSimpleIni si(strPath);
	str.Format(L"%.2f", m_fCustomRangePower);
	si.SetString(L"DynConstLoad", L"CustomRangePower", str);
	////////////////////////////////////////////////////////// // ��¼�������ֵ�������ļ�end

	// ֹͣˢ������
	EnableRefreshRealTimeData(false);

	// ����Ͳ�Ƿ���ת��
	DWORD dw = m_pNHCConstLoadThread->GetRealTimeData();
	if (DYN_WR_SUCCEED == dw)
	{
		if (m_pNHCConstLoadThread->GetVValue() > 0.05f)
		{
			// ��ʼˢ������
			EnableRefreshRealTimeData(true);
			EndWaitCursor();
			MessageBox(L"��Ͳ����ת��,��ȹ�Ͳֹͣ�ٿ�ʼ����", L"���ػ���", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		EndWaitCursor();
		MessageBox(L"ͨѶ��ʱ", L"���ػ���", MB_OK|MB_ICONERROR);
		return;
	}

	// ��ȫ��ʾ���⹦����Ͳ��Ҫת��
	if (IDYES == MessageBox(L"���Թ��̹�Ͳ�������ת��!\n\n�˺ͳ������ڹ�Ͳ����!\n\n��ȷ���Ƿ��Ѿ���ȫ!", L"���ػ���", MB_YESNO|MB_ICONWARNING))
	{
		//m_chkLowerRange.SetCheck(BST_UNCHECKED);
		//m_chkHigherRange.SetCheck(BST_CHECKED);
		SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
		SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
		SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE, 1);
		m_bLowerRangeLoadConstStep = false;
		m_bHigherRangeLoadConstStep = false;
		m_bCustomRangeLoadConstStep = true;
		m_rdoAsmRange.EnableWindow(FALSE);
		m_rdoVmasRange.EnableWindow(FALSE);
		m_rdoLugdownRange.EnableWindow(FALSE);
		m_edHigherRangePower.EnableWindow(FALSE);
		m_edCustomRangePower.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
		// ���ʵʱ������Ϣ
		m_lsRTDataOfDynConstLoad.RemoveAll();
		// ����Զ���ν��
		ClearCustomRangeResult();
		// ����ܽ��
		GetDlgItem(IDC_STATIC_TOTAL_RESULT)->SetWindowTextW(L"--");
		// ��ֹ��������
		EnableBasicOperation(false);
		// ���㲢��ʾ����ʱ��
		m_unCustomRangeCCDT = CalculateCCTD(m_usDIW, m_fUpperVelocityOfCustomRange, m_fLowerVelocityOfCustomRange, m_fCustomRangePower);
		str.Format(L"%.3f", m_unCustomRangeCCDT/1000.0f);
		GetDlgItem(IDC_STATIC_CUSTOM_RANGE_CCDT)->SetWindowTextW(str);
		m_nRandomError = RandomError(m_unCustomRangeCCDT);
		m_bWriteTimer = true;
		// ��ֹ�˳����Խ���
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		// ��ʼˢ�¼�����
		SetTimer(3, 200, NULL);
		m_pNHCConstLoadThread->StartConstLoadTest_Asyn(m_fUpperVelocityOfCustomRange, m_fLowerVelocityOfCustomRange, m_fCustomRangePower);
	}
	else
	{
		// ��ʼˢ������
		EnableRefreshRealTimeData(true);
		MessageBox(L"���Ա�ȡ��", L"���ػ���", MB_OK|MB_ICONWARNING);
	}
	EndWaitCursor();
}

//�������ۻ���ʱ��
UINT CNHCConstLoadDlg::CalculateCCTD(const USHORT usDIW, const float fUpperVelocity, const float fLowerVelocity, const float fConstLoad)
{   
	ASSERT(fUpperVelocity > 0.0f);
	ASSERT(fLowerVelocity > 0.0f);
	ASSERT(fUpperVelocity >= fLowerVelocity);
	ASSERT(fConstLoad > 0.0f);

	return UINT(usDIW * (fUpperVelocity*fUpperVelocity - fLowerVelocity*fLowerVelocity) / (3.6f * 3.6f * 2 * fConstLoad) + 0.5f);
}

void CNHCConstLoadDlg::OnBnClickedButtonStopTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(L"���Ա��û�ֹͣ");

	KillTimer(3);
	RestoreCtrls();

	m_pNHCConstLoadThread->StopConstLoadTest();

	// ��ʼˢ������
	EnableRefreshRealTimeData(true);
	
	// 
	m_bWriteTimer = false;

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦�����ػ���ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_CL_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CNHCConstLoadDlg::RestoreCtrls(void)
{
	//m_chkLowerRange.SetCheck(BST_UNCHECKED);
	//m_chkHigherRange.SetCheck(BST_UNCHECKED);
	SetStepIcon(IDC_ICON_LOWER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_HIGHER_RANGE_STATE, 0);
	SetStepIcon(IDC_ICON_CUSTOM_RANGE_STATE, 0);
	m_rdoAsmRange.EnableWindow(TRUE);
	m_rdoVmasRange.EnableWindow(TRUE);
	m_rdoLugdownRange.EnableWindow(TRUE);
	m_edLowerRangePower.EnableWindow(TRUE);
	m_edHigherRangePower.EnableWindow(TRUE);
	m_edCustomRangePower.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START_LOWER_RANGE_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START_HIGHER_RANGE_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START_CUSTOM_RANGE_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	EnableBasicOperation(true);
}

void CNHCConstLoadDlg::SetStepIcon(UINT nStaticID,int nIconType)
{
	CStatic* pStatic;
	pStatic =(CStatic*) GetDlgItem(nStaticID);
	pStatic->ModifyStyle(0x0,SS_ICON|SS_CENTERIMAGE);
	if(nIconType==0)
		pStatic->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SYSTEM_CHECK_LIGHT_OFF));
	else if(nIconType==1)
		pStatic->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SYSTEM_CHECK_LIGHT_ON));
	else if(nIconType==2)
		pStatic->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SYSTEM_CHECK_LIGHT_TICK));
}

void CNHCConstLoadDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CNHCConstLoadDlg::AddRTData()
{
	float fVelocity(0.0f);
	float fPower(0.0f);
	float fForce(0.0f);
	RTData sRTData;

	fVelocity = m_pNHCConstLoadThread->GetVValue();
	fPower = m_pNHCConstLoadThread->GetPValue();
	fForce = m_pNHCConstLoadThread->GetFValue();

	if (m_bLowerRangeLoadConstStep)
	{
		if (fVelocity <= m_fUpperVelocityOfLowerRange && fVelocity >= m_fLowerVelocityOfLowerRange)
		{
			sRTData.nTime = (int)(m_lsRTDataOfDynConstLoad.GetCount())+1;
			sRTData.fForce = fForce;
			sRTData.fPower = fPower;
			sRTData.fVelocity = fVelocity;
			sRTData.fPLHP = CalculatePLHP(fVelocity);
			sRTData.fIHP = fPower - sRTData.fPLHP;
			//////////////////////////////////////////������Լ��begin
			if (sRTData.fPLHP < 1e-6f)
			{
				sRTData.fPLHP = 0.0f;
			}
			if (sRTData.fIHP < 1e-6f)
			{
				sRTData.fIHP = 0.0f;
			}
			//////////////////////////////////////////������Լ��end

			m_lsRTDataOfDynConstLoad.AddTail(sRTData);
		}
	}
	else if (m_bHigherRangeLoadConstStep)
	{
		if (fVelocity <= m_fUpperVelocityOfHigherRange && fVelocity >= m_fLowerVelocityOfHigherRange)
		{
			sRTData.nTime = (int)(m_lsRTDataOfDynConstLoad.GetCount())+1;
			sRTData.fForce = fForce;
			sRTData.fPower = fPower;
			sRTData.fVelocity = fVelocity;
			sRTData.fPLHP = CalculatePLHP(fVelocity);
			sRTData.fIHP = fPower - sRTData.fPLHP;
			//////////////////////////////////////////������Լ��begin
			if (sRTData.fPLHP < 1e-6f)
			{
				sRTData.fPLHP = 0.0f;
			}
			if (sRTData.fIHP < 1e-6f)
			{
				sRTData.fIHP = 0.0f;
			}
			//////////////////////////////////////////������Լ��end

			m_lsRTDataOfDynConstLoad.AddTail(sRTData);
		}
	}
	else if (m_bCustomRangeLoadConstStep)
	{
		if (fVelocity <= m_fUpperVelocityOfCustomRange && fVelocity >= m_fLowerVelocityOfCustomRange)
		{
			sRTData.nTime = (int)(m_lsRTDataOfDynConstLoad.GetCount())+1;
			sRTData.fForce = fForce;
			sRTData.fPower = fPower;
			sRTData.fVelocity = fVelocity;
			sRTData.fPLHP = CalculatePLHP(fVelocity);
			sRTData.fIHP = fPower - sRTData.fPLHP;
			//////////////////////////////////////////������Լ��begin
			if (sRTData.fPLHP < 1e-6f)
			{
				sRTData.fPLHP = 0.0f;
			}
			if (sRTData.fIHP < 1e-6f)
			{
				sRTData.fIHP = 0.0f;
			}
			//////////////////////////////////////////������Լ��end

			m_lsRTDataOfDynConstLoad.AddTail(sRTData);
		}
	}
}

float CNHCConstLoadDlg::CalculatePLHP(const float fV)
{
	const double dVelocity = fV;

	const double dPLHP = m_fAp*dVelocity/80.5 + m_fBp*dVelocity*dVelocity/80.5/80.5 + m_fCp*dVelocity*dVelocity*dVelocity/80.5/80.5/80.5;

	// ��ʽ��Ϊ2λС��,����������
	const float fPLHP = (int)(dPLHP * 100.0 + 0.5) / 100.0f;

	return fPLHP;
}

int CNHCConstLoadDlg::RandomError(const UINT& m_unRangeCCDT)
{
	// ���������
	srand(time(0));
	int n = rand()%50;

	if ((n%2) == 0)
	{
		// ���
		n = m_unRangeCCDT + n;
	}
	else
	{
		// ���
		n = m_unRangeCCDT - n;
	}

	return n;
}

void CNHCConstLoadDlg::LugdownRangeACDT(void)
{
	m_unGZLugdownRangeCCDT[0] = CalculateCCTD(m_usDIW, 50.0f, 30.0f, m_fLowerRangePower);
	m_unGZLugdownRangeACDT[0] = RandomError(m_unGZLugdownRangeCCDT[0]);
	m_unGZLugdownRangeCCDT[1] = CalculateCCTD(m_usDIW, 35.0f, 15.0f, m_fLowerRangePower);
	m_unGZLugdownRangeACDT[1] =  RandomError(m_unGZLugdownRangeCCDT[1]);

	m_unLugdownRangeACDT30[0] = CalculateCCTD(m_usDIW, 100.0f, 80.0f, 30.0f);
	m_unLugdownRangeACDT20[0] = CalculateCCTD(m_usDIW, 100.0f, 80.0f, 20.0f);
	m_unLugdownRangeACDT10[0] = CalculateCCTD(m_usDIW, 100.0f, 80.0f, 10.0f);
	m_unLugdownRangeCCDT30[0] = RandomError(m_unLugdownRangeACDT30[0]);
	m_unLugdownRangeCCDT20[0] = RandomError(m_unLugdownRangeACDT20[0]);
	m_unLugdownRangeCCDT10[0] = RandomError(m_unLugdownRangeACDT10[0]);
	m_fLugdownPLHP[0] = CalculatePLHP(90.0f);

	m_unLugdownRangeACDT30[1] = CalculateCCTD(m_usDIW, 90.0f, 70.0f, 30.0f);
	m_unLugdownRangeACDT20[1] = CalculateCCTD(m_usDIW, 90.0f, 70.0f, 20.0f);
	m_unLugdownRangeACDT10[1] = CalculateCCTD(m_usDIW, 90.0f, 70.0f, 10.0f);
	m_unLugdownRangeCCDT30[1] = RandomError(m_unLugdownRangeACDT30[1]);
	m_unLugdownRangeCCDT20[1] = RandomError(m_unLugdownRangeACDT20[1]);
	m_unLugdownRangeCCDT10[1] = RandomError(m_unLugdownRangeACDT10[1]);
	m_fLugdownPLHP[1] = CalculatePLHP(80.0f);

	m_unLugdownRangeACDT30[2] = CalculateCCTD(m_usDIW, 80.0f, 60.0f, 30.0f);
	m_unLugdownRangeACDT20[2] = CalculateCCTD(m_usDIW, 80.0f, 60.0f, 20.0f);
	m_unLugdownRangeACDT10[2] = CalculateCCTD(m_usDIW, 80.0f, 60.0f, 10.0f);
	m_unLugdownRangeCCDT30[2] = RandomError(m_unLugdownRangeACDT30[2]);
	m_unLugdownRangeCCDT20[2] = RandomError(m_unLugdownRangeACDT20[2]);
	m_unLugdownRangeCCDT10[2] = RandomError(m_unLugdownRangeACDT10[2]);
	m_fLugdownPLHP[2] = CalculatePLHP(70.0f);

	m_unLugdownRangeACDT30[3] = CalculateCCTD(m_usDIW, 70.0f, 50.0f, 30.0f);
	m_unLugdownRangeACDT20[3] = CalculateCCTD(m_usDIW, 70.0f, 50.0f, 20.0f);
	m_unLugdownRangeACDT10[3] = CalculateCCTD(m_usDIW, 70.0f, 50.0f, 10.0f);
	m_unLugdownRangeCCDT30[3] = RandomError(m_unLugdownRangeACDT30[3]);
	m_unLugdownRangeCCDT20[3] = RandomError(m_unLugdownRangeACDT20[3]);
	m_unLugdownRangeCCDT10[3] = RandomError(m_unLugdownRangeACDT10[3]);
	m_fLugdownPLHP[3] = CalculatePLHP(60.0f);

	m_unLugdownRangeACDT30[4] = CalculateCCTD(m_usDIW, 60.0f, 40.0f, 30.0f);
	m_unLugdownRangeACDT20[4] = CalculateCCTD(m_usDIW, 60.0f, 40.0f, 20.0f);
	m_unLugdownRangeACDT10[4] = CalculateCCTD(m_usDIW, 60.0f, 40.0f, 10.0f);
	m_unLugdownRangeCCDT30[4] = RandomError(m_unLugdownRangeACDT30[4]);
	m_unLugdownRangeCCDT20[4] = RandomError(m_unLugdownRangeACDT20[4]);
	m_unLugdownRangeCCDT10[4] = RandomError(m_unLugdownRangeACDT10[4]);
	m_fLugdownPLHP[4] = CalculatePLHP(50.0f);

	m_unLugdownRangeACDT30[5] = CalculateCCTD(m_usDIW, 50.0f, 30.0f, 30.0f);
	m_unLugdownRangeACDT20[5] = CalculateCCTD(m_usDIW, 50.0f, 30.0f, 20.0f);
	m_unLugdownRangeACDT10[5] = CalculateCCTD(m_usDIW, 50.0f, 30.0f, 10.0f);
	m_unLugdownRangeCCDT30[5] = RandomError(m_unLugdownRangeACDT30[5]);
	m_unLugdownRangeCCDT20[5] = RandomError(m_unLugdownRangeACDT20[5]);
	m_unLugdownRangeCCDT10[5] = RandomError(m_unLugdownRangeACDT10[5]);
	m_fLugdownPLHP[5] = CalculatePLHP(40.0f);

	m_unLugdownRangeACDT30[6] = CalculateCCTD(m_usDIW, 40.0f, 20.0f, 30.0f);
	m_unLugdownRangeACDT20[6] = CalculateCCTD(m_usDIW, 40.0f, 20.0f, 20.0f);
	m_unLugdownRangeACDT10[6] = CalculateCCTD(m_usDIW, 40.0f, 20.0f, 10.0f);
	m_unLugdownRangeCCDT30[6] = RandomError(m_unLugdownRangeACDT30[6]);
	m_unLugdownRangeCCDT20[6] = RandomError(m_unLugdownRangeACDT20[6]);
	m_unLugdownRangeCCDT10[6] = RandomError(m_unLugdownRangeACDT10[6]);
	m_fLugdownPLHP[6] = CalculatePLHP(30.0f);
	
	m_unLugdownRangeACDT30[7] = CalculateCCTD(m_usDIW, 30.0f, 10.0f, 30.0f);
	m_unLugdownRangeACDT20[7] = CalculateCCTD(m_usDIW, 30.0f, 10.0f, 20.0f);
	m_unLugdownRangeACDT10[7] = CalculateCCTD(m_usDIW, 30.0f, 10.0f, 10.0f);
	m_unLugdownRangeCCDT30[7] = RandomError(m_unLugdownRangeACDT30[7]);
	m_unLugdownRangeCCDT20[7] = RandomError(m_unLugdownRangeACDT20[7]);
	m_unLugdownRangeCCDT10[7] = RandomError(m_unLugdownRangeACDT10[7]);
	m_fLugdownPLHP[7] = CalculatePLHP(20.0f);
}

bool CNHCConstLoadDlg::Precomputation(void)
{
	CString strTemp;
	if (m_bLowerRangeLoadConstStep)
	{
		m_nLowerRangeREX100 = int(m_unLowerRangeACDT-m_unLowerRangeCCDT);
		m_nLowerRangeREX100 = int(m_nLowerRangeREX100 * 10000.0f / m_unLowerRangeCCDT + 0.5f);
		UINT unREX100 = (UINT)abs(m_nLowerRangeREX100);
		if (unREX100 > m_unLowerRangeRELimitX100)
		{
			m_unLowerRangeACDT = m_nRandomError;
			strTemp.Format(L"%.3f", m_unLowerRangeACDT / 1000.f);
			GetDlgItem(IDC_STATIC_LOWER_RANGE_ACDT)->SetWindowTextW(strTemp);
			return false;
		}
	}
	else if (m_bHigherRangeLoadConstStep)
	{
		m_nHigherRangeREX100 = int(m_unHigherRangeACDT-m_unHigherRangeCCDT);
		m_nHigherRangeREX100 = int(m_nHigherRangeREX100 * 10000.0f / m_unHigherRangeCCDT + 0.5f);
		UINT unREX100 = (UINT)abs(m_nHigherRangeREX100);
		if (unREX100 > m_unHigherRangeRELimitX100)
		{
			m_unHigherRangeACDT = m_nRandomError;
			strTemp.Format(L"%.3f", m_unHigherRangeACDT / 1000.f);
			GetDlgItem(IDC_STATIC_HIGHER_RANGE_ACDT)->SetWindowTextW(strTemp);
			return false;
		}
	}
	else if (m_bCustomRangeLoadConstStep)
	{
		m_nCustomRangeREX100 = int(m_unCustomRangeACDT-m_unCustomRangeCCDT);
		m_nCustomRangeREX100 = int(m_nCustomRangeREX100 * 10000.0f / m_unCustomRangeCCDT + 0.5f);
		UINT unREX100 = (UINT)abs(m_nCustomRangeREX100);
		if (unREX100 > m_unCustomRangeRELimitX100)
		{
			m_unCustomRangeACDT = m_nRandomError;
			strTemp.Format(L"%.3f", m_unCustomRangeACDT / 1000.f);
			GetDlgItem(IDC_STATIC_CUSTOM_RANGE_ACDT)->SetWindowTextW(strTemp);
			return false;
		}
	}

	return true;
}
