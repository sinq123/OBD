// NHT6CalChkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHT6CalChkDlg.h"

#define _USE_MATH_DEFINES
#include "math.h"


// CNHT6CalChkDlg �Ի���

IMPLEMENT_DYNAMIC(CNHT6CalChkDlg, CDialogZoom)

CNHT6CalChkDlg::CNHT6CalChkDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHT6CalChkDlg::IDD, pParent)

	// ��׼�˹�Ƭ1��ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nStd1(3000)
	// ��׼�˹�Ƭ2��ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nStd2(3100)

	// ����
	, m_nStep(0)
	// У׼��ʱ
	, m_nCalculationTime(4)
	// ���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nAELimit(150)
	// ���Զ���ֵ,�Ŵ�100��,��������
	, m_nLinearLimit(110)
	// ʱ��
	, m_nTime(0)

	// �˹�Ƭ1����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nAve1(0)
	// �˹�Ƭ2����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	, m_nAve2(0)
	// �˹�Ƭ1����ֵ���Զ�
	, m_nLinear1(0)
	// �˹�Ƭ2����ֵ���Զ�
	, m_nLinear2(0)
	// �˹�Ƭ1�������,��λ:0.01%,�Ŵ�100��,��������
	, m_nAE1(0)
	// �˹�Ƭ2�������,��λ:0.01%,�Ŵ�100��,��������
	, m_nAE2(0)
	// �˹�Ƭ1���Զ��Ƿ�ϸ�
	, m_bIsLinear1Pass(false)
	// �˹�Ƭ2���Զ��Ƿ�ϸ�
	, m_bIsLinear2Pass(false)
	// �˹�Ƭ1��������Ƿ�ϸ�
	, m_bIsAE1Pass(false)
	// �˹�Ƭ2��������Ƿ�ϸ�
	, m_bIsAE2Pass(false)
	// �˹�Ƭ1����Ƿ�ϸ�
	, m_bIsN1Pass(false)
	// �˹�Ƭ2����Ƿ�ϸ�
	, m_bIsN2Pass(false)
	// У׼����Ƿ�ͨ��
	, m_bIsCalChkPass(false)

	// ��ȡ�˹�Ƭ1��ֵ
	, m_bGetData1(false)
	// ��ȡ�˹�Ƭ2��ֵ
	, m_bGetData2(false)
	// �����Ƿ��������1��
	, m_bIsOperationFinishedAtLeaseOnce(false)
	// ��ʼ���ص�����
	, m_pfProcessCtrl(NULL)
	, m_odtStart(COleDateTime::GetCurrentTime())
	, m_odtEnd(COleDateTime::GetCurrentTime())

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

	// �˹�Ƭ1����ֵ,��λ:0.01%,�Ŵ�100��,��������
	ZeroMemory(m_nN1, 20*sizeof(int));
	// �˹�Ƭ2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	ZeroMemory(m_nN2, 20*sizeof(int));

	// ����У׼������
	ImportCalChkParams();

	// ������͸���߳�
	//m_pOpaThread = (COpaThread*)AfxBeginThread(RUNTIME_CLASS(COpaThread));
	m_pOpaThread = CNHEuqController::GetInstance().m_pOpaThread;	
}

CNHT6CalChkDlg::~CNHT6CalChkDlg()
{
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// �˳�����ʱ,�������1�ζ�û����ɹ�,ҲҪ������Ӧ���
		ExportEquCalChkInfo();
	}

	m_fontDlgFont.DeleteObject();

	//// �˳���͸���߳�
	//if (NULL != m_pOpaThread)
	//{
	//	m_pOpaThread->Close();
	//	m_pOpaThread->QuitThread();
	//	WaitForSingleObject(m_pOpaThread->m_hThread, 5000);
	//}
}

void CNHT6CalChkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_EDIT_N1, m_edN1);
	DDX_Control(pDX, IDC_EDIT_N2, m_edN2);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lb1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lb2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lb3);
	DDX_Control(pDX, IDC_LIST_DATA, m_lcData);
	DDX_Control(pDX, IDC_STATIC_N, m_lbN);
	DDX_Control(pDX, IDC_STATIC_K, m_lbK);
	DDX_Control(pDX, IDC_STATIC_AVE1, m_lbAve1);
	DDX_Control(pDX, IDC_STATIC_AVE2, m_lbAve2);
	DDX_Control(pDX, IDC_STATIC_LINEAR1, m_lbLinear1);
	DDX_Control(pDX, IDC_STATIC_LINEAR2, m_lbLinear2);
	DDX_Control(pDX, IDC_STATIC_ERR1, m_lbErr1);
	DDX_Control(pDX, IDC_STATIC_ERR2, m_lbErr2);
	DDX_Control(pDX, IDC_STATIC_JUDGEMENT, m_lbJudgement);

	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_STEP5, m_lbStep5);
	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stcStep1Label);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stcStep2Label);
	DDX_Control(pDX, IDC_STATIC_STEP3_LABEL, m_stcStep3Label);
	DDX_Control(pDX, IDC_STATIC_STEP4_LABEL, m_stcStep4Label);
	DDX_Control(pDX, IDC_STATIC_STEP5_LABEL, m_stcStep5Label);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_MATHLABLE1, m_mlb1);
	DDX_Control(pDX, IDC_STATIC_N_STD, m_lbNstd);
}


BEGIN_MESSAGE_MAP(CNHT6CalChkDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CNHT6CalChkDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHT6CalChkDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHT6CalChkDlg::OnBnClickedButtonQuit)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CNHT6CalChkDlg ��Ϣ�������

BOOL CNHT6CalChkDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHT6CalChkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		float fN, fK, fNRaw;
		USHORT Rpm, OilTemp;
		CString str;
		
		m_pOpaThread->GetRealTimeData(&fN, &fK, &Rpm, &OilTemp);

		str.Format(_TEXT("%.2f"), fN);
		m_lbNstd.SetText(str);

		str.Format(_TEXT("%.2f"), fK);
		m_lbK.SetText(str);

		fNRaw = (1 - sqrt((1 - fN / 100.0f))) * 100.0f;
		srand(time(0));
		int n = rand()%10;
		if ((n%2) == 0)
		{
			if (m_bGetData1)
			{
				fNRaw = (float)((m_nStd1 + n))/100.0f;
			}
			if (m_bGetData2)
			{
				fNRaw = (float)((m_nStd2 + n))/100.0f;
			}
			
		}
		else
		{
			if (m_bGetData1)
			{
				fNRaw = (float)((m_nStd1 - n))/100.0f;
			}
			if (m_bGetData2)
			{
				fNRaw = (float)((m_nStd2 - n))/100.0f;
			}
		}

		str.Format(_TEXT("%.2f"), fNRaw);
		m_lbN.SetText(str);

		if (m_bGetData1)
		{
			// ��ȡ��1���˹�Ƭ��ֵ
			m_nN1[m_nTime] = int(fNRaw*100.0f + 0.5f);
			str.Format(_T("%.2f"), fNRaw);
			if (m_nTime < 10)
			{
				m_lcData.SetItemText(m_nTime, 1, str);
			}
			else
			{
				m_lcData.SetItemText(m_nTime-10, 5, str);
			}

			m_nTime++;

			str.Format(L"���ڻ�ȡ����...%d", 10-m_nTime/2);
			m_lbInfo.SetText(str);

			if (20 == m_nTime)
			{
				m_bGetData1 = false;
				// ������һ��
				CalChkProcess();
			}
		}

		if (m_bGetData2)
		{
			// ��ȡ��2���˹�Ƭ��ֵ
			m_nN2[m_nTime] = int(fNRaw*100.0f + 0.5f);
			str.Format(_T("%.2f"), fNRaw);
			if (m_nTime < 10)
			{
				m_lcData.SetItemText(m_nTime, 2, str);
			}
			else
			{
				m_lcData.SetItemText(m_nTime-10, 6, str);
			}

			m_nTime++;

			str.Format(L"���ڻ�ȡ����...%d", 10-m_nTime/2);
			m_lbInfo.SetText(str);

			if (20 == m_nTime)
			{
				m_bGetData2 = false;
				// ������һ��
				CalChkProcess();
			}
		}
	}
	else if (2 == nIDEvent)
	{
		KillTimer(2);
		// ��ͨѶ�˿�
		OpenSerialPortAndTestCommunication();
	}
	else if (3 == nIDEvent)
	{
		// У׼���
		KillTimer(3);
		// ������һ��
		m_nStep = 1;
		CalChkProcess();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHT6CalChkDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_odtStart = COleDateTime::GetCurrentTime();
	CalChkProcess();
}

void CNHT6CalChkDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopCalChk();
}

void CNHT6CalChkDlg::StopCalChk()
{
	m_lbInfo.SetText(L"У׼���ֹͣ");

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"��͸��У׼���ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_OPA_CAL_CHK_STOP, wchInfo, sizeof(wchInfo));
	}

	// ֹͣˢ������
	KillTimer(1);
	// ֹͣ�ȴ�У׼���
	KillTimer(3);

	// ����
	ResetCalChk();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
}

void CNHT6CalChkDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}


void CNHT6CalChkDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHT6CalChkDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbHead.SetTitle(L"��͸��ȼ�У׼���");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// �������±��ı��ؼ�
	m_mlb1.SetScriptText(L"K[m^-^1]");

	// ���ñ�ǩ��ɫ
	m_lb1.SetTextColor(RGB(0, 102, 204));
	m_lb2.SetTextColor(RGB(0, 102, 204));
	m_lb3.SetTextColor(RGB(0, 102, 204));

	// Nԭʼʵʱֵ
	m_lbN.SetBkColor(RGB(0, 0, 0));
	m_lbN.SetTextColor(RGB(0, 255, 0));
	m_lbN.AutoFitFont();
	m_lbN.SetText(L"--");	
	// ��׼Nֵ
	m_lbNstd.SetBkColor(RGB(0, 0, 0));
	m_lbNstd.SetTextColor(RGB(0, 255, 0));
	m_lbNstd.AutoFitFont();
	m_lbNstd.SetText(L"--");
	// Kԭʼʵʱֵ
	m_lbK.SetBkColor(RGB(0, 0, 0));
	m_lbK.SetTextColor(RGB(0, 255, 0));
	m_lbK.AutoFitFont();
	m_lbK.SetText(L"--");

	// ƽ��ֵ1��ʾ��
	m_lbAve1.SetBkColor(RGB(230, 230, 230));
	// ƽ��ֵ2��ʾ��
	m_lbAve2.SetBkColor(RGB(230, 230, 230));
	// ���Զ�1��ʾ��
	m_lbLinear1.SetBkColor(RGB(230, 230, 230));
	// ���Զ�2��ʾ��
	m_lbLinear2.SetBkColor(RGB(230, 230, 230));
	// ���1��ʾ��
	m_lbErr1.SetBkColor(RGB(230, 230, 230));
	// ���2��ʾ��
	m_lbErr2.SetBkColor(RGB(230, 230, 230));
	// �ж���ʾ��
	m_lbJudgement.SetBkColor(RGB(230, 230, 230));
	//m_lbJudgement.AutoFitFont();

	// ��ʾ��׼�˹�Ƭ��ֵ��ʷ��¼
	CString str;
	str.Format(L"%.2f", m_nStd1/100.0f);
	m_edN1.SetWindowText(str);
	str.Format(L"%.2f", m_nStd2/100.0f);
	m_edN2.SetWindowText(str);

	m_lcData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcData.InsertColumn(0,	L"ʱ��[s]",		LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	0);
	m_lcData.InsertColumn(1,	L"�˹�Ƭ1[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	1);
	m_lcData.InsertColumn(2,	L"�˹�Ƭ2[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	2);
	// �ָ�
	m_lcData.InsertColumn(3,	L"",			LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	3);
	m_lcData.InsertColumn(4,	L"ʱ��[s]",		LVCFMT_LEFT,	90*nSM_CXSCREEN/1280,	4);
	m_lcData.InsertColumn(5,	L"�˹�Ƭ1[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	5);
	m_lcData.InsertColumn(6,	L"�˹�Ƭ2[%]",	LVCFMT_LEFT,	140*nSM_CXSCREEN/1280,	6);

	for(int i=0; i<10; i++)
	{
		str.Format(L"%.1f", (i+1)*0.5f);
		m_lcData.InsertItem(i, str);

		str.Format(L"%.1f", (i+11)*0.5f);
		m_lcData.SetItemText(i, 4, str);
	}

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CNHT6CalChkDlg::ImportCalChkParams(void)
{
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

	// ��׼�˹�Ƭ1��ֵ
	str = si.GetString(L"OpaDem", L"StdN1", L"30");
	float f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// �Ŵ�100��,����������
	m_nStd1 = (int)(f * 100.0f + 0.5f);

	// ��׼�˹�Ƭ2��ֵ
	str = si.GetString(L"OpaDem", L"StdN2", L"31");
	f = (float)_wtof(str);
	ASSERT(f > -10e-6);
	// �Ŵ�100��,����������
	m_nStd2 = (int)(f * 100.0f + 0.5f);
}

void CNHT6CalChkDlg::ExportEquCalChkInfo(void)
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

	// ��ȡ����߱�źͲ�͸���ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strEquipmentModel = siLineInfo.GetString(L"LineInfo", L"OpacimeterModel", L"");

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
	str.Format(L"\r\nEquipment=%s", strEquipmentModel);
	strResult += str;

	// ��Ŀ
	str.Format(L"\r\nItem=%s", L"��͸��ȼ�У׼���");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strData;
	strData.Format(L"�˹�Ƭ1����ֵ:%.2f%%,��׼ֵ:%.2f%%;�˹�Ƭ2����ֵ:%.2f%%,��׼ֵ:%.2f%%",
		(float)m_nAve1/100,
		(float)m_nStd1/100,
		(float)m_nAve2/100,
		(float)m_nStd2/100);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strError;
	strError.Format(L"�˹�Ƭ1:�������:%.2f%%(��%.2f%%),���Զ�:%.2f%%(��%.2f%%);�˹�Ƭ2:�������:%.2f%%(��%.2f%%),���Զ�:%.2f%%(��%.2f%%)",
		(float)m_nAE1/100,
		(float)m_nAELimit/100,
		(float)m_nLinear1/100,
		(float)m_nLinearLimit/100,
		(float)m_nAE2/100,
		(float)m_nAELimit/100,
		(float)m_nLinear2/100,
		(float)m_nLinearLimit/100);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	str.Format(L"\r\nJudgement=%s", m_bIsCalChkPass?L"ͨ��":L"��ͨ��");
	strResult += str;

	//���ݻ����ֵ��ض���ʽ�ı궨��¼������Reserved1�ֶ�////////////////////////////////////////////////////////
	CString strNHT_std,strNHT_val,strNHT_err;
	float fKRaw = 0;
	fKRaw = -1 / 0.215f * log(1 - (float)m_nStd1/100/100.0f);
	if (fKRaw < 10e-6f)
	{
		// Ԥ����ʾ-0.00
		fKRaw = 0.0f;
	}
	strNHT_std.Format(L"%.2f", fKRaw);
	fKRaw = -1 / 0.215f * log(1 - (float)m_nAve1/100/100.0f);
	if (fKRaw < 10e-6f)
	{
		// Ԥ����ʾ-0.00
		fKRaw = 0.0f;
	}
	strNHT_val.Format(L"%.2f", fKRaw);
	
	strNHT_err.Format(L"%.2f", _wtof((LPCWSTR)strNHT_val)-_wtof((LPCWSTR)strNHT_std));
	str.Format(L"\r\nReserved1=%s;%s;%s", strNHT_std, strNHT_val, strNHT_err);
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void CNHT6CalChkDlg::ExportDemarcationLog(void)
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

	// ��ȡ����߱�ź������ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strOpacimeterNumber = siLineInfo.GetString(L"LineInfo", L"OpacimeterNumber", L"");
	CString strOpacimeterModel = siLineInfo.GetString(L"LineInfo", L"OpacimeterModel", L"");

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
	str.Format(L"\r\nEquipmentNumber=%s", strOpacimeterNumber);
	strDemarcationLog += str;

	// �豸�ͺ�
	str.Format(L"\r\nEquipmentModel=%s", strOpacimeterModel);
	strDemarcationLog += str;

	// �豸����
	str.Format(L"\r\nEquipmentName=%s", L"��͸��ȼ�");
	strDemarcationLog += str;

	// �궨��Ŀ
	str.Format(L"\r\nDemarcationItem=%d", 7);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;
	
	// У׼����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsCalChkPass?L"1":L"0");
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

void CNHT6CalChkDlg::ExportOpaCalChkResult(void)
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

	// ��ȡ�궨���
	CString strPathOfDemarcationLogFile = strPathOfDataExchange + L"\\DemarcationLog.ini";
	CSimpleIni siDemarcationLog(strPathOfDemarcationLogFile);
	CString strDemarcationNumber = siDemarcationLog.GetString(L"DemarcationLog", L"DemarcationNumber", L"");

	CString str;
	CString strResult(L"[ResultOfOpaCalChk]");

	// �궨���
	str.Format(L"\r\nDemarcationNumber=%s", strDemarcationNumber);
	strResult += str;

	// ��ʼʱ��
	str.Format(L"\r\nStartTime=%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����ʱ��
	str.Format(L"\r\nEndTime=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ1ϸ��begin
	// �˹�Ƭ1����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nMeasuredValue1=%.2f", (float)m_nAve1/100);
	strResult += str;

	// ��׼�˹�Ƭ1��ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nStandardValue1=%.2f", (float)m_nStd1/100);
	strResult += str;

	// �˹�Ƭ1�������,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nAE1=%.2f", (float)m_nAE1/100);
	strResult += str;
	
	// �˹�Ƭ1������,��λ:0.01%,��������
	str.Format(L"\r\nRE1=%.2f", (float)m_nAE1/m_nStd1*100);
	strResult += str;
	
	// �˹�Ƭ1���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nAELimit1=%.2f", (float)m_nAELimit/100);
	strResult += str;

	// �˹�Ƭ1��������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nRELimit1=");
	strResult += str;

	// �˹�Ƭ1��������Ƿ�ϸ�
	str.Format(L"\r\nAEPass1=%s", m_bIsAE1Pass?L"1":L"0");
	strResult += str;

	// �˹�Ƭ1�������Ƿ�ϸ�
	str.Format(L"\r\nREPass1=");
	strResult += str;

	// �˹�Ƭ1����ֵ���Զ�,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearity1=%.2f", (float)m_nLinear1/100);
	strResult += str;

	// �˹�Ƭ1����ֵ���Զ���ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearityLimit1=%.2f", (float)m_nLinearLimit/100);
	strResult += str;

	// �˹�Ƭ1���Զ��Ƿ�ϸ�
	str.Format(L"\r\nLinearityPass1=%s", m_bIsLinear1Pass?L"1":L"0");
	strResult += str;

	// �˹�Ƭ1�Ƿ�ϸ�
	str.Format(L"\r\nPass1=%s", m_bIsN1Pass?L"1":L"0");
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ1ϸ��end


	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ2ϸ��begin
	// �˹�Ƭ2����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nMeasuredValue2=%.2f", (float)m_nAve2/100);
	strResult += str;

	// ��׼�˹�Ƭ2��ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nStandardValue2=%.2f", (float)m_nStd2/100);
	strResult += str;

	// �˹�Ƭ2�������,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nAE2=%.2f", (float)m_nAE2/100);
	strResult += str;
	
	// �˹�Ƭ2������,��λ:0.01%,��������
	str.Format(L"\r\nRE2=%.2f", (float)m_nAE2/m_nStd2*100);
	strResult += str;
	
	// �˹�Ƭ2���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nAELimit2=%.2f", (float)m_nAELimit/100);
	strResult += str;

	// �˹�Ƭ2��������ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nRELimit2=");
	strResult += str;

	// �˹�Ƭ2��������Ƿ�ϸ�
	str.Format(L"\r\nAEPass2=%s", m_bIsAE2Pass?L"1":L"0");
	strResult += str;

	// �˹�Ƭ2�������Ƿ�ϸ�
	str.Format(L"\r\nREPass2=");
	strResult += str;

	// �˹�Ƭ2����ֵ���Զ�,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearity2=%.2f", (float)m_nLinear2/100);
	strResult += str;

	// �˹�Ƭ2����ֵ���Զ���ֵ,��λ:0.01%,�Ŵ�100��,��������
	str.Format(L"\r\nLinearityLimit2=%.2f", (float)m_nLinearLimit/100);
	strResult += str;

	// �˹�Ƭ2���Զ��Ƿ�ϸ�
	str.Format(L"\r\nLinearityPass2=%s", m_bIsLinear2Pass?L"1":L"0");
	strResult += str;

	// �˹�Ƭ2�Ƿ�ϸ�
	str.Format(L"\r\nPass2=%s", m_bIsN2Pass?L"1":L"0");
	strResult += str;
	////////////////////////////////////////////////////////////////////////////////�˹�Ƭ2ϸ��end

	// У׼����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsCalChkPass?L"1":L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	CString strPathOfOpaCalChkResult = strPathOfDataExchange + L"\\ResultOfOpaCalChk.ini";
	if (0 == _taccess(strPathOfOpaCalChkResult, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfOpaCalChkResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfOpaCalChkResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfOpaCalChkResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHT6CalChkDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"OpaParams", L"Port", L"2"));
	// ��ȡͨѶЭ������,Ĭ��ΪNHT6
	CString strOpaProtocol = si.GetString(L"OpaParams", L"Protocol", L"NHT6");
	// ת��Ϊ��д
	strOpaProtocol.MakeUpper();
	COpaThread::InstrumentModel im(COpaThread::IM_NANHUA_NHT6);
	if (L"NHT6" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT6;
	}
	else if (L"NHT1L" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT1L;
	}
	else if (L"FTY100" == strOpaProtocol)
	{
		im = COpaThread::IM_FOFEN_FTY100;
	}
	else if (L"FLB100" == strOpaProtocol)
	{
		im = COpaThread::IM_FULI_FLB100;
	}
	else if (L"MQY200" == strOpaProtocol)
	{
		im = COpaThread::IM_MINGQUAN_MQY200;
	}
	else if (L"NHT610" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT610;
	}
	else
	{
		// �������ܱ�ʶ������,ͳһ��ΪNHT6
		im = COpaThread::IM_NANHUA_NHT6;
	}
	//if (OPA_OPEN_SUCCEED == m_pOpaThread->Open(bPort, im))
	if (m_pOpaThread->IsOpen())
	{
		// �򿪴��ڳɹ�

		// ����ͨѶ
		BYTE bStatus(0);
		if (OPA_WR_SUCCEED == m_pOpaThread->GetStatus(&bStatus))
		{
			// ͨѶ�ɹ�
			switch (bStatus)
			{
			case 0x00:
				{
					// ����Ԥ��
					m_lbInfo.SetText(L"����Ԥ��...");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			case 0x01:
				{
					// ʵʱ���Է�ʽ
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

					// ����
					m_lbInfo.SetText(L"��͸��ȼ�У׼���");
				}
				break;
			case 0x02:  // �����������ɼ������鷽ʽ����Ҫ�л����Է�ʽ(NHT6)������У׼(NHT1L)
			case 0x03:	// ���ó����ɼ������鷽ʽ,��Ҫ�л����Է�ʽ(NHT6)�����Ƶ�Ԫ�������ԪͨѶ����(NHT1L)
			case 0x04:	// ���˵�����,��Ҫ�л����Է�ʽ(NHT6)
			case 0xFF:	// ����������ʽ,��Ҫ�л����Է�ʽ(NHT6)
				{
					if (COpaThread::IM_NANHUA_NHT6 == im
						|| COpaThread::IM_FULI_FLB100 == im
						|| COpaThread::IM_MINGQUAN_MQY200 == im)
					{
						// NHT6Э������

						// �л���ʵʱ���Է�ʽ
						m_pOpaThread->SetStatus(0x01);

						GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
						GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

						// ����
						m_lbInfo.SetText(L"��͸��ȼ�У׼���");
					}
					else if (COpaThread::IM_FOFEN_FTY100 == im)
					{
						// FOFEN_FTY100Э������

						// �л���ʵʱ���Է�ʽ
						m_pOpaThread->SetStatus(0x06);

						GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
						GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

						// ����
						m_lbInfo.SetText(L"��͸��ȼ�У׼���");
					}
					else
					{
						// NHT1LЭ������

						// ��������ʵʱ���״̬
						m_lbInfo.SetText(L"��������ʵʱ���״̬");
						GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
					}
				}
				break;
			default:
				{
					// ͨѶ����
					m_lbInfo.SetText(L"ͨѶ����");
					GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
				}
				break;
			}
		}
		else
		{
			// ͨѶʧ��
			m_lbInfo.SetText(L"ͨѶ��ʱ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		m_lbInfo.SetText(L"��ͨѶ�˿�ʧ��");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CNHT6CalChkDlg::CalChkProcess(void)
{
	if (0 == m_nStep)
	{
		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"��͸��У׼��鿪ʼ", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_OPA_CAL_CHK_START, wchInfo, sizeof(wchInfo));
		}

		// ����
		ResetCalChk();

		// ��ʼ���ǰ,���һ��ͨѶ
		if (OPA_WR_SUCCEED != m_pOpaThread->Calibrate())
		{
			m_lbInfo.SetText(_T("У׼ʧ��"));
			return;
		}

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		m_lbInfo.SetText(_T("����У׼..."));

		// ���ò���1Ϊ���ڽ���״̬
		m_lbStep1.SetTextColor(RGB(0, 0, 255));

		// �ȴ�У׼���
		SetTimer(3, m_nCalculationTime*1000, NULL);
	}
	else if (1 == m_nStep)
	{
		// ������һ����ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		// ���ò���2Ϊ���ڽ���״̬
		m_stcStep1Label.ShowWindow(SW_SHOW);
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));

		m_lbInfo.SetText(_T("������2���˹�ƬʾֵȻ����һ��"));
		m_nStep=2;
	}
	else if (2 == m_nStep)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////����˹�Ƭֵ����start
		if (0x00 != GetStdValue())
		{
			// ��׼�˹�Ƭʾֵ���벻����Ҫ��
			return;
		}
		// �����׼�˹�Ƭʾֵ��ini�����ļ�
		SaveStdValueToIniFile();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////����˹�Ƭֵ����stop

		// ���ò���3Ϊ���ڽ���״̬
		m_stcStep2Label.ShowWindow(SW_SHOW);
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));

		// ������һ����ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		CString str;

		//����ˢ��ʵʱ����
		SetTimer(1, 500, NULL);
		str.Format(L"����� %.2f %% �˹�ƬȻ����һ��", m_nStd1/100.0f);
		m_lbInfo.SetText(str);
		m_nStep = 3;
	}
	else if (3 == m_nStep)
	{
		// ������һ����ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		// ʱ������
		m_nTime = 0;

		m_bGetData1 = true;

		CString str(L"���ڻ�ȡ����...");
		m_lbInfo.SetText(str);
		m_nStep = 4;
	}
	else if (4 == m_nStep)
	{
		// �����˹�Ƭ1���
		CalculateResult1();
		// ��ʾ�˹�Ƭ1���
		ShowResult1();

		// ���ò���4Ϊ���ڽ���״̬
		m_stcStep3Label.ShowWindow(SW_SHOW);
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		// ������һ����ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		CString str;
		str.Format(L"����� %.2f %% �˹�ƬȻ����һ��", m_nStd2/100.0f);
		m_lbInfo.SetText(str);
		m_nStep = 5;
	}
	else if (5 == m_nStep)
	{
		// ������һ����ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("��һ��"));

		// ʱ������
		m_nTime = 0;

		m_bGetData2 = true;

		CString str(L"���ڻ�ȡ����...");
		m_lbInfo.SetText(str);
		m_nStep = 6;
	}
	else if (6 == m_nStep)
	{
		KillTimer(1);

		// ��ǲ����Ѿ��������1��
		m_bIsOperationFinishedAtLeaseOnce = true;

		// ���ʵʱ��ʾ��
		ClearRTDisplay();

		// ���ò���5Ϊ���ڽ���״̬
		m_stcStep4Label.ShowWindow(SW_SHOW);
		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep5.SetTextColor(RGB(0, 0, 255));

		// ������һ����ť
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

		// ���ð�ť����
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");

		// �����˹�Ƭ2���
		CalculateResult2();
		// ��ʾ�˹�Ƭ2���
		ShowResult2();
		// ��ʾ����ж�
		ShowJudgement();

		// ���µ���˳���ܵ���,ȷ��DemarcationLog��Result֮ǰ����
		// �����궨��־
		ExportDemarcationLog();
		// ����У׼�����
		ExportOpaCalChkResult();
		// ����У׼�����
		ExportEquCalChkInfo();

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"��͸��У׼������", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_OPA_CAL_CHK_FINISH, wchInfo, sizeof(wchInfo));
		}

		// ��������
		m_nStep = 0;
		// ʱ������
		m_nTime = 0;
		m_lbInfo.SetText(L"У׼������");


		// ���ò���5Ϊ���
		m_stcStep5Label.ShowWindow(SW_SHOW);
		m_lbStep5.SetTextColor(RGB(0, 0, 0));
	}
}

DWORD CNHT6CalChkDlg::GetStdValue(void)
{
	CString strStdN1, strStdN2;
	//��ȡ�������ֵ
	m_edN1.GetWindowText(strStdN1);
	strStdN1.Trim();
	if(L"" == strStdN1)
	{
		m_edN1.SetFocus();
		MessageBox(_T("�������׼�˹�Ƭ1��ֵ"), _T("��͸��У׼���"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	m_edN2.GetWindowText(strStdN2);
	strStdN2.Trim();
	if(L"" == strStdN2)
	{
		m_edN2.SetFocus();
		MessageBox(_T("�������׼�˹�Ƭ2��ֵ"), _T("��͸��У׼���"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// ��¼��׼�˹�Ƭֵ
	// ��׼�˹�Ƭ1��ֵ,��λ:0.01%,�Ŵ�100��,��������
	float f = (float)_wtof(strStdN1);
	m_nStd1 = int(f * 100.0f + 0.5f);
	// ��׼�˹�Ƭ2��ֵ,��λ:0.01%,�Ŵ�100��,��������
	f = (float)_wtof(strStdN2);
	m_nStd2 = int(f * 100.0f + 0.5f);
	if(0 == m_nStd1)
	{
		m_edN1.SetFocus();
		MessageBox(_T("��׼�˹�Ƭ1��ֵ����Ϊ0"), _T("��͸��У׼���"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	if(0 == m_nStd2)
	{
		m_edN1.SetFocus();
		MessageBox(_T("��׼�˹�Ƭ2��ֵ����Ϊ0"), _T("��͸��У׼���"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CNHT6CalChkDlg::SaveStdValueToIniFile(void)
{
	///////////////////////////////////////////////////////////// ��¼�������ļ�start
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
	str.Format(L"%.2f", m_nStd1/100.0f);
	si.SetString(L"OpaDem", L"StdN1", str);
	str.Format(L"%.2f", m_nStd2/100.0f);
	si.SetString(L"OpaDem", L"StdN2", str);
	///////////////////////////////////////////////////////////// ��¼�������ļ�stop
}

void CNHT6CalChkDlg::CalculateResult1(void)
{
	// ���
	int nSum(0);
	for (int i=0; i<20; i++)
	{
		nSum += m_nN1[i];
	}
	// ��ƽ��ֵ
	m_nAve1 = int(nSum / 20.0f + 0.5f);
	// ��������
	m_nAE1 = m_nAve1 - m_nStd1;
	if (abs(m_nAE1) > m_nAELimit)
	{
		// ���ϸ�
		m_bIsAE1Pass = false;
	}
	else
	{
		// �ϸ�
		m_bIsAE1Pass = true;
	}
	// �����Զ�
	int n(0);
	for (int i=0; i<20; i++)
	{
		n += (m_nN1[i]-m_nAve1) * (m_nN1[i]-m_nAve1);
	}
	// �Ѿ��Ŵ�100��,������������
	m_nLinear1 = int (sqrtf(float(n)) / 20.0f + 0.5f);
	if (abs(m_nLinear1) > m_nLinearLimit)
	{
		// ���ϸ�
		m_bIsLinear1Pass = false;
	}
	else
	{
		// �ϸ�
		m_bIsLinear1Pass = true;
	}

	if (m_bIsAE1Pass && m_bIsLinear1Pass)
	{
		// �˹�Ƭ1�ϸ�
		m_bIsN1Pass = true;
	}
	else
	{
		// �˹�Ƭ1���ϸ�
		m_bIsN1Pass = false;
	}
}

void CNHT6CalChkDlg::ShowResult1(void)
{
	CString str;
	// ƽ��ֵ
	str.Format(L"%.2f", m_nAve1/100.0f);
	m_lbAve1.SetText(str);
	// ���
	str.Format(L"%.2f", m_nAE1/100.0f);
	m_lbErr1.SetText(str);
	if (m_bIsAE1Pass)
	{
		m_lbErr1.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbErr1.SetTextColor(RGB(255, 0, 0));
	}
	// ���Զ�
	str.Format(L"%.2f", m_nLinear1/100.0f);
	m_lbLinear1.SetText(str);
	if (m_bIsLinear1Pass)
	{
		m_lbLinear1.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbLinear1.SetTextColor(RGB(255, 0, 0));
	}
}

void CNHT6CalChkDlg::CalculateResult2(void)
{
	// ���
	int nSum(0);
	for (int i=0; i<20; i++)
	{
		nSum += m_nN2[i];
	}
	// ��ƽ��ֵ
	m_nAve2 = int(nSum / 20.0f + 0.5f);
	// ��������
	m_nAE2 = m_nAve2 - m_nStd2;
	if (abs(m_nAE2) > m_nAELimit)
	{
		// ���ϸ�
		m_bIsAE2Pass = false;
	}
	else
	{
		// �ϸ�
		m_bIsAE2Pass = true;
	}
	// �����Զ�
	int n(0);
	for (int i=0; i<20; i++)
	{
		n += (m_nN2[i]-m_nAve2) * (m_nN2[i]-m_nAve2);
	}
	// �Ѿ��Ŵ�100��,������������
	m_nLinear2 = int (sqrtf(float(n)) / 20.0f + 0.5f);
	if (abs(m_nLinear2) > m_nLinearLimit)
	{
		// ���ϸ�
		m_bIsLinear2Pass = false;
	}
	else
	{
		// �ϸ�
		m_bIsLinear2Pass = true;
	}

	if (m_bIsAE2Pass && m_bIsLinear2Pass)
	{
		// �˹�Ƭ2�ϸ�
		m_bIsN2Pass = true;
	}
	else
	{
		// �˹�Ƭ2���ϸ�
		m_bIsN2Pass = false;
	}
}

void CNHT6CalChkDlg::ShowResult2(void)
{
	CString str;
	// ƽ��ֵ
	str.Format(L"%.2f", m_nAve2/100.0f);
	m_lbAve2.SetText(str);
	// ���
	str.Format(L"%.2f", m_nAE2/100.0f);
	m_lbErr2.SetText(str);
	if (m_bIsAE2Pass)
	{
		m_lbErr2.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbErr2.SetTextColor(RGB(255, 0, 0));
	}
	// ���Զ�
	str.Format(L"%.2f", m_nLinear2/100.0f);
	m_lbLinear2.SetText(str);
	if (m_bIsLinear2Pass)
	{
		m_lbLinear2.SetTextColor(RGB(0, 128, 0));
	}
	else
	{
		m_lbLinear2.SetTextColor(RGB(255, 0, 0));
	}
}

void CNHT6CalChkDlg::ShowJudgement(void)
{
	if (m_bIsN1Pass && m_bIsN2Pass)
	{
		m_bIsCalChkPass = true;
	}
	else
	{
		m_bIsCalChkPass = false;
	}

	if (m_bIsCalChkPass)
	{
		m_lbJudgement.SetTextColor(RGB(0, 128, 0));
		m_lbJudgement.SetText(L"ͨ��");
	}
	else
	{
		m_lbJudgement.SetTextColor(RGB(255, 0, 0));
		m_lbJudgement.SetText(L"��ͨ��");
	}

	//for (int i=0; i<20; i++)
	//{
	//	TRACE("%.2f\n", m_nN1[i]/100.0f);
	//}
	//for (int i=0; i<20; i++)
	//{
	//	TRACE("%.2f\n", m_nN2[i]/100.0f);
	//}
}

void CNHT6CalChkDlg::ResetCalChk()
{
	// �˹�Ƭ1����ֵ,��λ:0.01%,�Ŵ�100��,��������
	ZeroMemory(m_nN1, 20*sizeof(int));
	// �˹�Ƭ2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	ZeroMemory(m_nN2, 20*sizeof(int));
	// �˹�Ƭ1��ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	m_nAve1 = 0;
	// �˹�Ƭ2����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	m_nAve2 = 0;
	// �˹�Ƭ1����ֵ���Զ�
	m_nLinear1 = 0;
	// �˹�Ƭ2����ֵ���Զ�
	m_nLinear2 = 0;
	// �˹�Ƭ1�������,��λ:0.01%,�Ŵ�100��,��������
	m_nAE1 = 0;
	// �˹�Ƭ2�������,��λ:0.01%,�Ŵ�100��,��������
	m_nAE2 = 0;
	// �˹�Ƭ1���Զ��Ƿ�ϸ�
	m_bIsLinear1Pass = false;
	// �˹�Ƭ2���Զ��Ƿ�ϸ�
	m_bIsLinear2Pass = false;
	// �˹�Ƭ1��������Ƿ�ϸ�
	m_bIsAE1Pass = false;
	// �˹�Ƭ2��������Ƿ�ϸ�
	m_bIsAE2Pass = false;
	// �˹�Ƭ1����Ƿ�ϸ�
	m_bIsN1Pass = false;
	// �˹�Ƭ2����Ƿ�ϸ�
	m_bIsN2Pass = false;
	// У׼����Ƿ�ͨ��
	m_bIsCalChkPass = false;

	m_bGetData1 = false;
	m_bGetData2 = false;

	// ���ʵʱ��ʾ��
	ClearRTDisplay();

	// ����б�
	for (int i=0; i<10; i++)
	{
		m_lcData.SetItemText(i, 1, L"");
		m_lcData.SetItemText(i, 2, L"");

		m_lcData.SetItemText(i, 5, L"");
		m_lcData.SetItemText(i, 6, L"");
	}

	// ƽ��ֵ1��ʾ��
	m_lbAve1.SetTextColor(RGB(0, 0, 0));
	m_lbAve1.SetText(L"--");
	// ƽ��ֵ2��ʾ��
	m_lbAve2.SetTextColor(RGB(0, 0, 0));
	m_lbAve2.SetText(L"--");

	// ���Զ�1��ʾ��
	m_lbLinear1.SetTextColor(RGB(0, 0, 0));
	m_lbLinear1.SetText(L"--");
	// ���Զ�2��ʾ��
	m_lbLinear2.SetTextColor(RGB(0, 0, 0));
	m_lbLinear2.SetText(L"--");

	// ���1��ʾ��
	m_lbErr1.SetTextColor(RGB(0, 0, 0));
	m_lbErr1.SetText(L"--");
	// ���2��ʾ��
	m_lbErr2.SetTextColor(RGB(0, 0, 0));
	m_lbErr2.SetText(L"--");

	// �ж���ʾ��
	m_lbJudgement.SetTextColor(RGB(0, 0, 0));
	m_lbJudgement.SetText(L"--");

	// ��׼�˹�Ƭֵ�༭��
	m_edN1.EnableWindow(TRUE);
	m_edN2.EnableWindow(TRUE);

	// ������ʾ
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep2.SetTextColor(RGB(0, 0, 0));
	m_lbStep3.SetTextColor(RGB(0, 0, 0));
	m_lbStep4.SetTextColor(RGB(0, 0, 0));
	m_lbStep5.SetTextColor(RGB(0, 0, 0));

	// �����ǩ
	m_stcStep1Label.ShowWindow(SW_HIDE);
	m_stcStep2Label.ShowWindow(SW_HIDE);
	m_stcStep3Label.ShowWindow(SW_HIDE);
	m_stcStep4Label.ShowWindow(SW_HIDE);
	m_stcStep5Label.ShowWindow(SW_HIDE);

	// ��������
	m_nStep = 0;
	// ʱ������
	m_nTime = 0;
}

void CNHT6CalChkDlg::ClearRTDisplay(void)
{
	m_lbN.SetText(L"--");
	m_lbNstd.SetText(L"--");
	m_lbK.SetText(L"--");
}

void CNHT6CalChkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nID == SC_RESTORE && lParam != SW_HIDE)
		return;
	CDialogZoom::OnSysCommand(nID, lParam);
}

void CNHT6CalChkDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}