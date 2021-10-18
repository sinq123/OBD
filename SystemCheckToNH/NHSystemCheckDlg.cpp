// NHSystemCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemCheckToNH.h"
#include "NHSystemCheckDlg.h"
#include "afxdialogex.h"


// CNHSystemCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CNHSystemCheckDlg, CDialogZoom)

CNHSystemCheckDlg::CNHSystemCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHSystemCheckDlg::IDD, pParent)
{
	// ���1280*1024�ֱ��ʽ��п���
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

CNHSystemCheckDlg::~CNHSystemCheckDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CNHSystemCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHSystemCheckDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_CHECH_SELF, &CNHSystemCheckDlg::OnBnClickedBtnSystemChechSelf)
	ON_BN_CLICKED(IDC_BTN_LEAK_UPLOAD, &CNHSystemCheckDlg::OnBnClickedBtnLeakUpload)
END_MESSAGE_MAP()


// CNHSystemCheckDlg ��Ϣ�������


BOOL CNHSystemCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrls();

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHSystemCheckDlg::InitCtrls()
{

	SetDlgFont();

	m_lbTitle.SetTitle(L"ϵͳ�Լ�");
}

void CNHSystemCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHSystemCheckDlg::OnBnClickedBtnSystemChechSelf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��ȡϵͳ����
	NHCLIENT SiniNHClient;
	GetIniNHClient(&SiniNHClient);

	LINEINFO sLineInfo;
	ZeroMemory(&sLineInfo, sizeof(LINEINFO));
	GetIniLineInfo(&sLineInfo);

	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);
	CString str;
	COleDateTime odtNow = COleDateTime::GetCurrentTime();

	if (L'1' == SiniNHClient.wchSystemType[0]		/* ASM */
	|| L'1' == SiniNHClient.wchSystemType[1]	/* VMAS */
	|| L'1' == SiniNHClient.wchSystemType[2]	/* LUGDOWN */
	)
	{
		// ��������
		str.Format(L"%d", 1);
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerLeafUp", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 0);
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerLeafUpFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ��������
		str.Format(L"%d", 1);
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerLeafDown", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerLeafDownFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ����1
		str.Format(L"%d", 1);
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerConstLoad", str);
		sio.SetString(L"DynamometerSystemCheck", L"StartTime1", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		odtNow += COleDateTimeSpan(0, 0, 0, 30);
		sio.SetString(L"DynamometerSystemCheck", L"EndTime1", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerConstLoadFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ����2
		str.Format(L"%d", 1);
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerConstLoad2", str);
		sio.SetString(L"DynamometerSystemCheck", L"StartTime2", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		odtNow += COleDateTimeSpan(0, 0, 0, 30);
		sio.SetString(L"DynamometerSystemCheck", L"EndTime2", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerConstLoadFinishedTime2", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		sio.SetString(L"DynamometerSystemCheck", L"DynamometerSystemCheckFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"DynamometerSystemCheck", L"DynamometerSystemCheckFinished", L"1");
	}

	if (L'1' == SiniNHClient.wchSystemType[0]		/* ASM */
	|| L'1' == SiniNHClient.wchSystemType[1]	/* VMAS */
	|| L'1' == SiniNHClient.wchSystemType[3]	/* DIS */
	)
	{
		// Ԥ��
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerWarmUp", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerWarmUpFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ��©
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerCheckLeak", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerCheckLeakFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ����
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerZero", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerZeroFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ������
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerCheckFlux", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerCheckFluxFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		//O2
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerCheckO2", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerCheckO2FinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerSystemCheckFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"AnalyzerSystemCheck", L"AnalyzerSystemCheckFinished", L"1");

	}

	if (L'1' == SiniNHClient.wchSystemType[2]		/* LUGDOWN */ 
	//&& L'1' == sLineInfo.wchHasNOxAnalyzer[0]	/* NOx*/
	)
	{
		// Ԥ��
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerWarmUp", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerWarmUpFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		// ��©
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerCheckLeak", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerCheckLeakFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		// ����
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerZero", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerZeroFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		// ������
		str.Format(L"%d", 1);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerCheckFlux", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerCheckFluxFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerSystemCheckFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"AnalyzerNOxSystemCheck", L"AnalyzerSystemCheckFinished", L"1");

	}

	if (L'1' == SiniNHClient.wchSystemType[2]		/* LUGDOWN */
	|| L'1' == SiniNHClient.wchSystemType[4]	/* FSUNHT */
	)
	{
		// Ԥ��
		str.Format(L"%d", 1);
		sio.SetString(L"OpacimeterSystemCheck", L"OpacimeterWarmUp", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"OpacimeterSystemCheck", L"OpacimeterWarmUpFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		// У׼
		str.Format(L"%d", 1);
		sio.SetString(L"OpacimeterSystemCheck", L"OpacimeterCalibration", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"OpacimeterSystemCheck", L"OpacimeterCalibrationFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));

		sio.SetString(L"OpacimeterSystemCheck", L"OpacimeterSystemCheckFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"OpacimeterSystemCheck", L"OpacimeterSystemCheckFinished", L"1");
	}

	if (L'1' == SiniNHClient.wchSystemType[1]) /* VMAS */
	{
		// �������
		str.Format(L"%d", 1);
		sio.SetString(L"FlowmeterSystemCheck", L"FlowmeterCheckFlux", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"FlowmeterSystemCheck", L"FlowmeterCheckFluxFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		// ������
		str.Format(L"%d", 1);
		sio.SetString(L"FlowmeterSystemCheck", L"FlowmeterCheckO2", str);
		odtNow += COleDateTimeSpan(0, 0, 0, 10);
		sio.SetString(L"FlowmeterSystemCheck", L"FlowmeterCheckO2FinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));


		sio.SetString(L"FlowmeterSystemCheck", L"FlowmeterSystemCheckFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
		sio.SetString(L"FlowmeterSystemCheck", L"FlowmeterSystemCheckFinished", L"1");
	}

	sio.SetString(L"SystemCheck", L"SystemCheckFinishedTime", odtNow.Format(L"%Y-%m-%d %H:%M:%S"));
	sio.SetString(L"SystemCheck", L"SystemCheckFinished", L"1");
}


void CNHSystemCheckDlg::OnBnClickedBtnLeakUpload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
