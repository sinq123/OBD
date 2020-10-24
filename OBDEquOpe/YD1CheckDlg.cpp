// YD1CheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YD1CheckDlg.h"
#include <math.h>


// CYD1CheckDlg �Ի���

IMPLEMENT_DYNAMIC(CYD1CheckDlg, CDialogZoom)

CYD1CheckDlg::CYD1CheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CYD1CheckDlg::IDD, pParent)
	, m_siStep(0)
	, m_bIsOperationFinishedAtLeaseOnce(false)
	, m_bRepeatPass(false)
	, m_bBasicErrorPass(false)
	, m_bPass(false)
	, m_pfProcessCtrl(NULL)
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

	// �����̶��߳�
	m_pSmoThread = (CSmoThread*)AfxBeginThread(RUNTIME_CLASS(CSmoThread));
}

CYD1CheckDlg::~CYD1CheckDlg()
{
	m_fontDlgFont.DeleteObject();

	// �˳���ֽʽ�̶ȼ��߳�
	if (NULL != m_pSmoThread)
	{
		m_pSmoThread->Close();
		m_pSmoThread->QuitThread();
		WaitForSingleObject(m_pSmoThread->m_hThread, 5000);
	}
}

void CYD1CheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_ST_REPEAT_ERROR_1, m_lbRepeatError1);
	DDX_Control(pDX, IDC_ST_REPEAT_ERROR_2, m_lbRepeatError2);
	DDX_Control(pDX, IDC_ST_REPEAT_ERROR_3, m_lbRepeatError3);
	DDX_Control(pDX, IDC_ST_REPEAT_ERROR_4, m_lbRepeatError4);
	DDX_Control(pDX, IDC_ST_REPEAT_ERROR_5, m_lbRepeatError5);
	DDX_Control(pDX, IDC_ST_REPEAT_RESULT, m_lbRepeatResult);
	DDX_Control(pDX, IDC_ST_BASICERR_1, m_lbBasicErr1);
	DDX_Control(pDX, IDC_ST_BASICERR_2, m_lbBasicErr2);
	DDX_Control(pDX, IDC_ST_BASICERR_F, m_lbBasicErrF);
	DDX_Control(pDX, IDC_ST_BASICERR_5, m_lbBasicErr5);
	DDX_Control(pDX, IDC_ST_BASICERR_6, m_lbBasicErr6);
	DDX_Control(pDX, IDC_ST_BASICERR_RESULT, m_lbBasicResult);
	DDX_Control(pDX, IDC_ED_FSN_1, m_edFSN1);
	DDX_Control(pDX, IDC_ED_FSN_2, m_edFSN2);
	DDX_Control(pDX, IDC_ED_FSN_F, m_edFSNF);
	DDX_Control(pDX, IDC_ED_FSN_5, m_edFSN5);
	DDX_Control(pDX, IDC_ED_FSN_6, m_edFSN6);
	DDX_Control(pDX, IDC_ED_FSN_CAL, m_edFSNCal);
	DDX_Control(pDX, IDC_STATIC_STEP1_LABEL, m_stStep1);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2_LABEL, m_stStep2);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
}


BEGIN_MESSAGE_MAP(CYD1CheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CYD1CheckDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_START, &CYD1CheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CYD1CheckDlg::OnBnClickedButtonStop)
	ON_MESSAGE(WM_SMO_REPEAT_END, OnRepeatCheck)
	ON_MESSAGE(WM_SMO_REAL_END, OnRealCheck)
END_MESSAGE_MAP()


// CYD1CheckDlg ��Ϣ�������

BOOL CYD1CheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ʼ���ؼ�
	InitCtrls();

	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CYD1CheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CYD1CheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbHead.SetTitle(L"��ֽʽ�̶ȼ��ظ��Ժ�ʾֵ�����");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	// ���ñ�ǩ��ɫ
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));

	m_lbRepeatError1.SetBkColor(RGB(230, 230, 230));
	m_lbRepeatError2.SetBkColor(RGB(230, 230, 230));
	m_lbRepeatError3.SetBkColor(RGB(230, 230, 230));
	m_lbRepeatError4.SetBkColor(RGB(230, 230, 230));
	m_lbRepeatError5.SetBkColor(RGB(230, 230, 230));
	m_lbRepeatResult.SetBkColor(RGB(230, 230, 230));

	m_lbBasicErr1.SetBkColor(RGB(230, 230, 230));
	m_lbBasicErr2.SetBkColor(RGB(230, 230, 230));
	m_lbBasicErrF.SetBkColor(RGB(230, 230, 230));
	m_lbBasicErr5.SetBkColor(RGB(230, 230, 230));
	m_lbBasicErr6.SetBkColor(RGB(230, 230, 230));
	m_lbBasicResult.SetBkColor(RGB(230, 230, 230));

	// �����̶ȿ�ֵ
	ImportFSNCheckParams();

	ClearResult();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CYD1CheckDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"SmoParams", L"Port", L"2"));
	if (SMO_OPEN_SUCCEED == m_pSmoThread->Open(bPort, CSmoThread::IM_NANHUA_YD1))
	{
		// �򿪴��ڳɹ�
		// ����ͨѶ
		BYTE bResult(0);
		if (SMO_WR_SUCCEED == m_pSmoThread->Reset(&bResult))
		{
			// ͨѶ�ɹ�

			// ����
			m_lbInfo.SetText(L"��ֽʽ�̶ȼ��ظ��Ժ�ʾֵ�����");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
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

void CYD1CheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			// ��ͨѶ�˿�
			OpenSerialPortAndTestCommunication();
		}
		break;
	default:
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CYD1CheckDlg::OnBnClickedButtonQuit()
{
	if (false == m_bIsOperationFinishedAtLeaseOnce)
	{
		// �˳�����ʱ,�������1�ζ�û����ɹ�,ҲҪ������Ӧ���
		ExportEquCalChkInfo();
	}

	OnCancel();
}

void CYD1CheckDlg::OnBnClickedButtonStart()
{
	switch(m_siStep)
	{
	case 0:
		{
			ClearResult();
			CString str;
			GetDlgItem(IDC_ED_FSN_CAL)->GetWindowText(str);
			if (L"" == str)
			{
				AfxMessageBox(L"�������ظ��Լ���̶ȿ�ֵ");
				return;
			}

			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"�̶ȼƼ�鿪ʼ", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_SMO_CHK_START, wchInfo, sizeof(wchInfo));
			}

			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��һ��");
			m_lbInfo.SetText(L"�����4.5FSN���ҵ��̶ȿ�");
			GetDlgItem(IDC_ED_FSN_CAL)->EnableWindow(FALSE);
			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_siStep = 1;
			m_pSmoThread->m_bIsQuit = false;
		}
		break;
	case 1:
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"�����ظ��Լ��...");
			m_pSmoThread->OnRepeatCheck((WPARAM)m_hWnd, NULL);		
		}
		break;
	case 2:
		{
			CString str;
			GetDlgItem(IDC_ED_FSN_1)->GetWindowText(str);
			if (L"" == str)
			{
				AfxMessageBox(L"������<1��>�̶ȿ�ֵ");
				return;
			}
			m_lbInfo.SetText(L"�����" + str + L"FSN���̶ȿ�");
			GetDlgItem(IDC_ED_FSN_1)->EnableWindow(FALSE);
			m_lbStep2.SetTextColor(RGB(0, 0, 255));
			m_siStep = 3;
		}
		break;
	case 3:
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"���ڽ��е�1��ʾֵ�����...");
			m_pSmoThread->OnRealCheck((WPARAM)m_hWnd, 1);
		}
		break;
	case 4:
		{
			CString str;
			GetDlgItem(IDC_ED_FSN_2)->GetWindowText(str);
			if (L"" == str)
			{
				AfxMessageBox(L"������<2��>�̶ȿ�ֵ");
				return;
			}
			m_lbInfo.SetText(L"�����" + str + L"FSN���̶ȿ�");
			GetDlgItem(IDC_ED_FSN_2)->EnableWindow(FALSE);
			m_siStep = 5;
		}
		break;
	case 5:
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"���ڽ��е�2��ʾֵ�����...");
			m_pSmoThread->OnRealCheck((WPARAM)m_hWnd, 2);
		}
		break;
	case 6:
		{
			CString str;
			GetDlgItem(IDC_ED_FSN_F)->GetWindowText(str);
			if (L"" == str)
			{
				AfxMessageBox(L"������<��У׼��>�̶ȿ�ֵ");
				return;
			}
			m_lbInfo.SetText(L"�����" + str + L"FSN���̶ȿ�");
			GetDlgItem(IDC_ED_FSN_F)->EnableWindow(FALSE);
			m_siStep = 7;
		}
		break;
	case 7:
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"���ڽ��з�У׼��ʾֵ�����...");
			m_pSmoThread->OnRealCheck((WPARAM)m_hWnd, 3);
		}
		break;
	case 8:
		{
			CString str;
			GetDlgItem(IDC_ED_FSN_5)->GetWindowText(str);
			if (L"" == str)
			{
				AfxMessageBox(L"������<��5��>�̶ȿ�ֵ");
				return;
			}
			m_lbInfo.SetText(L"�����" + str + L"FSN���̶ȿ�");
			GetDlgItem(IDC_ED_FSN_5)->EnableWindow(FALSE);
			m_siStep = 9;
		}
		break;
	case 9:
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"���ڽ��е�5��ʾֵ�����...");
			m_pSmoThread->OnRealCheck((WPARAM)m_hWnd, 4);
		}
		break;
	case 10:
		{
			CString str;
			GetDlgItem(IDC_ED_FSN_6)->GetWindowText(str);
			if (L"" == str)
			{
				AfxMessageBox(L"������<��6��>�̶ȿ�ֵ");
				return;
			}
			m_lbInfo.SetText(L"�����" + str + L"FSN���̶ȿ�");
			GetDlgItem(IDC_ED_FSN_6)->EnableWindow(FALSE);
			m_siStep = 11;
		}
		break;
	case 11:
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"���ڽ��е�6��ʾֵ�����...");
			m_pSmoThread->OnRealCheck((WPARAM)m_hWnd, 5);
		}
		break;
	case 12:
		{
			m_pSmoThread->m_bIsQuit = true;
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			m_lbInfo.SetText(L"��ֽʽ�̶ȼ��ظ��Ժ�ʾֵ�����");
			GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
			// ��յ�ǰ����
			m_siStep = 0;
			// ��ǲ����Ѿ��������1��
			m_bIsOperationFinishedAtLeaseOnce = true;
			// ���̶�ֵ�����
			EnableFSNEdit();
			// �����̶ȿ�����
			SaveFSNCheckParams();

			// �����궨��־
			ExportDemarcationLog();
			// �����̶ȼƼ����
			ExportSmoChkResult();
			// �����̶ȼƼ����
			ExportEquCalChkInfo();
			
			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"�̶ȼƼ�����", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_SMO_CHK_FINISH, wchInfo, sizeof(wchInfo));
			}
		}
		break;
	default:
		break;
	}
}

void CYD1CheckDlg::OnBnClickedButtonStop()
{
	m_pSmoThread->m_bIsQuit = true;
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	m_lbInfo.SetText(L"�����ֹ");
	// ��յ�ǰ����
	m_siStep = 0;
	// ���̶�ֵ�����
	EnableFSNEdit();

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�̶ȼƼ��ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_SMO_CHK_STOP, wchInfo, sizeof(wchInfo));
	}
}

void CYD1CheckDlg::ClearResult(void)
{
	GetDlgItem(IDC_ST_REPEAT_VALUE_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_VALUE_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_VALUE_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_VALUE_4)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_VALUE_5)->SetWindowText(L"--");

	GetDlgItem(IDC_ST_REPEAT_ERROR_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_ERROR_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_ERROR_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_ERROR_4)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_ERROR_5)->SetWindowText(L"--");
	m_lbRepeatError1.SetTextColor(RGB(0, 0, 0));
	m_lbRepeatError2.SetTextColor(RGB(0, 0, 0));
	m_lbRepeatError3.SetTextColor(RGB(0, 0, 0));
	m_lbRepeatError4.SetTextColor(RGB(0, 0, 0));
	m_lbRepeatError5.SetTextColor(RGB(0, 0, 0));

	GetDlgItem(IDC_ST_REPEAT_AVERAGE)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_REPEAT_RESULT)->SetWindowText(L"--");
	m_lbRepeatResult.SetTextColor(RGB(0, 0, 0));

	GetDlgItem(IDC_ST_BASICERR_1_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_1_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_1_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_1_AVERAGE)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_1)->SetWindowText(L"--");

	GetDlgItem(IDC_ST_BASICERR_2_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_2_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_2_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_2_AVERAGE)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_2)->SetWindowText(L"--");

	GetDlgItem(IDC_ST_BASICERR_F_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_F_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_F_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_F_AVERAGE)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_F)->SetWindowText(L"--");

	GetDlgItem(IDC_ST_BASICERR_5_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_5_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_5_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_5_AVERAGE)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_5)->SetWindowText(L"--");

	GetDlgItem(IDC_ST_BASICERR_6_1)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_6_2)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_6_3)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_6_AVERAGE)->SetWindowText(L"--");
	GetDlgItem(IDC_ST_BASICERR_6)->SetWindowText(L"--");

	m_lbBasicErr1.SetTextColor(RGB(0, 0, 0));
	m_lbBasicErr2.SetTextColor(RGB(0, 0, 0));
	m_lbBasicErrF.SetTextColor(RGB(0, 0, 0));
	m_lbBasicErr5.SetTextColor(RGB(0, 0, 0));
	m_lbBasicErr6.SetTextColor(RGB(0, 0, 0));

	GetDlgItem(IDC_ST_BASICERR_RESULT)->SetWindowText(L"--");
	m_lbBasicResult.SetTextColor(RGB(0, 0, 0));

	m_stStep1.ShowWindow(SW_HIDE);
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_stStep2.ShowWindow(SW_HIDE);
	m_lbStep2.SetTextColor(RGB(0, 0, 0));

	m_bRepeatPass = false;
	m_bBasicErrorPass = false;
	m_bPass = false;
}

LRESULT CYD1CheckDlg::OnRepeatCheck(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 1:
		{
			m_lbInfo.SetText(L"��1���ظ��Լ�����");
			CString str;
			str.Format(L"%.2f", m_pSmoThread->m_fRepatData[wParam]);
			GetDlgItem(IDC_ST_REPEAT_VALUE_1)->SetWindowText(str);
		}
		break;
	case 2:
		{
			m_lbInfo.SetText(L"��2���ظ��Լ�����");
			CString str;
			str.Format(L"%.2f", m_pSmoThread->m_fRepatData[wParam]);
			GetDlgItem(IDC_ST_REPEAT_VALUE_2)->SetWindowText(str);
		}
		break;
	case 3:
		{
			m_lbInfo.SetText(L"��3���ظ��Լ�����");
			CString str;
			str.Format(L"%.2f", m_pSmoThread->m_fRepatData[wParam]);
			GetDlgItem(IDC_ST_REPEAT_VALUE_3)->SetWindowText(str);
		}
		break;
	case 4:
		{
			m_lbInfo.SetText(L"��4���ظ��Լ�����");
			CString str;
			str.Format(L"%.2f", m_pSmoThread->m_fRepatData[wParam]);
			GetDlgItem(IDC_ST_REPEAT_VALUE_4)->SetWindowText(str);
		}
		break;
	case 5:
		{
			m_lbInfo.SetText(L"��5���ظ��Լ�����");
			CString str;
			str.Format(L"%.2f", m_pSmoThread->m_fRepatData[wParam]);
			GetDlgItem(IDC_ST_REPEAT_VALUE_5)->SetWindowText(str);

			ShowRepeatCheckResult();
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			m_lbInfo.SetText(L"�ظ��Լ�����");
			m_stStep1.ShowWindow(SW_SHOW);
			m_lbStep1.SetTextColor(RGB(0, 0, 0));
			m_siStep = 2;
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void CYD1CheckDlg::ShowRepeatCheckResult(void)
{
	float fRepeatCheckAverage = (m_pSmoThread->m_fRepatData[1] + 
		m_pSmoThread->m_fRepatData[2] +
		m_pSmoThread->m_fRepatData[3] +
		m_pSmoThread->m_fRepatData[4] +
		m_pSmoThread->m_fRepatData[5])/5;

	CString str;
	str.Format(L"%.2f", fRepeatCheckAverage);

	GetDlgItem(IDC_ST_REPEAT_AVERAGE)->SetWindowText(str);

	float fErr1,fErr2,fErr3,fErr4,fErr5;
	// ������ε�ƫ��
	fErr1 = fabs(m_pSmoThread->m_fRepatData[1] - fRepeatCheckAverage);
	fErr2 = fabs(m_pSmoThread->m_fRepatData[2] - fRepeatCheckAverage);
	fErr3 = fabs(m_pSmoThread->m_fRepatData[3] - fRepeatCheckAverage);
	fErr4 = fabs(m_pSmoThread->m_fRepatData[4] - fRepeatCheckAverage);
	fErr5 = fabs(m_pSmoThread->m_fRepatData[5] - fRepeatCheckAverage);

	str.Format(L"%.2f", fErr1);
	GetDlgItem(IDC_ST_REPEAT_ERROR_1)->SetWindowText(str);
	str.Format(L"%.2f", fErr2);
	GetDlgItem(IDC_ST_REPEAT_ERROR_2)->SetWindowText(str);
	str.Format(L"%.2f", fErr3);
	GetDlgItem(IDC_ST_REPEAT_ERROR_3)->SetWindowText(str);
	str.Format(L"%.2f", fErr4);
	GetDlgItem(IDC_ST_REPEAT_ERROR_4)->SetWindowText(str);
	str.Format(L"%.2f", fErr5);
	GetDlgItem(IDC_ST_REPEAT_ERROR_5)->SetWindowText(str);

	//���������̵�%2
	float fMaxErr = (float)(10 * 0.02);

	bool bResult = true;
	//����ж�
	if(fErr1 > fMaxErr)
	{
		m_lbRepeatError1.SetTextColor(RGB(255, 0, 0));
		m_lbRepeatResult.SetText(L"���ϸ�");
		m_lbRepeatResult.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}
	if(fErr2 > fMaxErr)
	{
		m_lbRepeatError2.SetTextColor(RGB(255, 0, 0));
		m_lbRepeatResult.SetText(L"���ϸ�");
		m_lbRepeatResult.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}
	if(fErr3 > fMaxErr)
	{
		m_lbRepeatError3.SetTextColor(RGB(255, 0, 0));
		m_lbRepeatResult.SetText(L"���ϸ�");
		m_lbRepeatResult.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}
	if(fErr4 > fMaxErr)
	{
		m_lbRepeatError4.SetTextColor(RGB(255, 0, 0));
		m_lbRepeatResult.SetText(L"���ϸ�");
		m_lbRepeatResult.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}
	if(fErr5 > fMaxErr)
	{
		m_lbRepeatError5.SetTextColor(RGB(255, 0, 0));
		m_lbRepeatResult.SetText(L"���ϸ�");
		m_lbRepeatResult.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}

	if(bResult)
	{
		m_lbRepeatResult.SetTextColor(RGB(0, 128, 0));
		m_lbRepeatResult.SetText(L"�ϸ�");
		m_bRepeatPass = true;
	}
	m_bPass = m_bRepeatPass&&m_bBasicErrorPass;
}
LRESULT CYD1CheckDlg::OnRealCheck(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case 1:
		{
			if (1 == wParam)
			{
				m_lbInfo.SetText(L"��1��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_1_1)->SetWindowText(str);
			}
			else if (2 == wParam)
			{
				m_lbInfo.SetText(L"��2��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_1_2)->SetWindowText(str);
			}
			else if (3 == wParam)
			{
				m_lbInfo.SetText(L"��3��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_1_3)->SetWindowText(str);

				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				m_lbInfo.SetText(L"��1��ʾֵ��������");
				m_siStep = 4;
			}
		}
		break;
	case 2:
		{
			if (1 == wParam)
			{
				m_lbInfo.SetText(L"��1��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_2_1)->SetWindowText(str);
			}
			else if (2 == wParam)
			{
				m_lbInfo.SetText(L"��2��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_2_2)->SetWindowText(str);
			}
			else if (3 == wParam)
			{
				m_lbInfo.SetText(L"��3��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_2_3)->SetWindowText(str);

				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				m_lbInfo.SetText(L"��2��ʾֵ��������");
				m_siStep = 6;
			}
		}
		break;
	case 3:
		{
			if (1 == wParam)
			{
				m_lbInfo.SetText(L"��1��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_F_1)->SetWindowText(str);
			}
			else if (2 == wParam)
			{
				m_lbInfo.SetText(L"��2��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_F_2)->SetWindowText(str);
			}
			else if (3 == wParam)
			{
				m_lbInfo.SetText(L"��3��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_F_3)->SetWindowText(str);

				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				m_lbInfo.SetText(L"��У׼��ʾֵ��������");
				m_siStep = 8;
			}
		}
		break;
	case 4:
		{
			if (1 == wParam)
			{
				m_lbInfo.SetText(L"��1��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_5_1)->SetWindowText(str);
			}
			else if (2 == wParam)
			{
				m_lbInfo.SetText(L"��2��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_5_2)->SetWindowText(str);
			}
			else if (3 == wParam)
			{
				m_lbInfo.SetText(L"��3��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_5_3)->SetWindowText(str);

				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				m_lbInfo.SetText(L"��5��ʾֵ��������");
				m_siStep = 10;
			}
		}
		break;
	case 5:
		{
			if (1 == wParam)
			{
				m_lbInfo.SetText(L"��1��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_6_1)->SetWindowText(str);
			}
			else if (2 == wParam)
			{
				m_lbInfo.SetText(L"��2��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_6_2)->SetWindowText(str);
			}
			else if (3 == wParam)
			{
				m_lbInfo.SetText(L"��3��ʾֵ��������");
				CString str;
				str.Format(L"%.2f", m_pSmoThread->m_fRealData[lParam][wParam]);
				GetDlgItem(IDC_ST_BASICERR_6_3)->SetWindowText(str);

				ShowBasicErrorCheckResult();
				GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"���");
				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				m_lbInfo.SetText(L"ʾֵ��������");
				m_stStep2.ShowWindow(SW_SHOW);
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_siStep = 12;
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

void CYD1CheckDlg::ShowBasicErrorCheckResult(void)
{
	float fBasicStd_1,fBasicErr_1_Average,fBasicErr_1;
	float fBasicStd_2,fBasicErr_2_Average,fBasicErr_2;
	float fBasicStd_F,fBasicErr_F_Average,fBasicErr_F;
	float fBasicStd_5,fBasicErr_5_Average,fBasicErr_5;
	float fBasicStd_6,fBasicErr_6_Average,fBasicErr_6;

	bool bResult(true);//���ս��
	float fMaxValue = (float)(10 * 0.03);//���

	CString str;
	GetDlgItem(IDC_ED_FSN_1)->GetWindowText(str);
	fBasicStd_1 = (float)_wtof(str);
	fBasicErr_1_Average = (m_pSmoThread->m_fRealData[1][1] + m_pSmoThread->m_fRealData[1][2] + m_pSmoThread->m_fRealData[1][3])/3;
	str.Format(L"%.2f", fBasicErr_1_Average);
	GetDlgItem(IDC_ST_BASICERR_1_AVERAGE)->SetWindowText(str);
	fBasicErr_1 = fabs(fBasicErr_1_Average - fBasicStd_1);
	str.Format(L"%.2f", fBasicErr_1);
	m_lbBasicErr1.SetText(str);
	if(fBasicErr_1 > fMaxValue)
	{
		m_lbBasicErr1.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}

	GetDlgItem(IDC_ED_FSN_2)->GetWindowText(str);
	fBasicStd_2 = (float)_wtof(str);
	fBasicErr_2_Average = (m_pSmoThread->m_fRealData[2][1] + m_pSmoThread->m_fRealData[2][2] + m_pSmoThread->m_fRealData[2][3])/3;
	str.Format(L"%.2f", fBasicErr_2_Average);
	GetDlgItem(IDC_ST_BASICERR_2_AVERAGE)->SetWindowText(str);
	fBasicErr_2 = fabs(fBasicErr_2_Average - fBasicStd_2);
	str.Format(L"%.2f", fBasicErr_2);
	m_lbBasicErr2.SetText(str);
	if(fBasicErr_2 > fMaxValue)
	{
		m_lbBasicErr2.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}

	GetDlgItem(IDC_ED_FSN_F)->GetWindowText(str);
	fBasicStd_F = (float)_wtof(str);
	fBasicErr_F_Average = (m_pSmoThread->m_fRealData[3][1] + m_pSmoThread->m_fRealData[3][2] + m_pSmoThread->m_fRealData[3][3])/3;
	str.Format(L"%.2f", fBasicErr_F_Average);
	GetDlgItem(IDC_ST_BASICERR_F_AVERAGE)->SetWindowText(str);
	fBasicErr_F = fabs(fBasicErr_F_Average - fBasicStd_F);
	str.Format(L"%.2f", fBasicErr_F);
	m_lbBasicErrF.SetText(str);
	if(fBasicErr_F > fMaxValue)
	{
		m_lbBasicErrF.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}

	GetDlgItem(IDC_ED_FSN_5)->GetWindowText(str);
	fBasicStd_5 = (float)_wtof(str);
	fBasicErr_5_Average = (m_pSmoThread->m_fRealData[4][1] + m_pSmoThread->m_fRealData[4][2] + m_pSmoThread->m_fRealData[4][3])/3;
	str.Format(L"%.2f", fBasicErr_5_Average);
	GetDlgItem(IDC_ST_BASICERR_5_AVERAGE)->SetWindowText(str);
	fBasicErr_5 = fabs(fBasicErr_5_Average - fBasicStd_5);
	str.Format(L"%.2f", fBasicErr_5);
	m_lbBasicErr5.SetText(str);
	if(fBasicErr_5 > fMaxValue)
	{
		m_lbBasicErr5.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}

	GetDlgItem(IDC_ED_FSN_6)->GetWindowText(str);
	fBasicStd_6 = (float)_wtof(str);
	fBasicErr_6_Average = (m_pSmoThread->m_fRealData[5][1] + m_pSmoThread->m_fRealData[5][2] + m_pSmoThread->m_fRealData[5][3])/3;
	str.Format(L"%.2f", fBasicErr_6_Average);
	GetDlgItem(IDC_ST_BASICERR_6_AVERAGE)->SetWindowText(str);
	fBasicErr_6 = fabs(fBasicErr_6_Average - fBasicStd_6);
	str.Format(L"%.2f", fBasicErr_6);
	m_lbBasicErr6.SetText(str);
	if(fBasicErr_6 > fMaxValue)
	{
		m_lbBasicErr6.SetTextColor(RGB(255, 0, 0));
		bResult = false;
	}

	if(bResult)
	{
		m_lbBasicResult.SetTextColor(RGB(0,128,0));
		m_lbBasicResult.SetText(L"�ϸ�");
		m_bBasicErrorPass = true;
	}
	else
	{
		m_lbBasicResult.SetTextColor(RGB(255,0,0));
		m_lbBasicResult.SetText(L"���ϸ�");
		m_bBasicErrorPass = false;
	}
	m_bPass = m_bRepeatPass&&m_bBasicErrorPass;
}

void CYD1CheckDlg::EnableFSNEdit(void)
{
	GetDlgItem(IDC_ED_FSN_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_ED_FSN_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_ED_FSN_CAL)->EnableWindow(TRUE);
	GetDlgItem(IDC_ED_FSN_F)->EnableWindow(TRUE);
	GetDlgItem(IDC_ED_FSN_5)->EnableWindow(TRUE);
	GetDlgItem(IDC_ED_FSN_6)->EnableWindow(TRUE);
}

void CYD1CheckDlg::ImportFSNCheckParams(void)
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

	CSimpleIni sIni(strPath);
	CString str;
	str = sIni.GetString(L"SmoDem", L"Std1", L"");
	GetDlgItem(IDC_ED_FSN_1)->SetWindowText(str);
	str = sIni.GetString(L"SmoDem", L"Std2", L"");
	GetDlgItem(IDC_ED_FSN_2)->SetWindowText(str);
	str = sIni.GetString(L"SmoDem", L"StdCal", L"");
	GetDlgItem(IDC_ED_FSN_CAL)->SetWindowText(str);
	str = sIni.GetString(L"SmoDem", L"StdF", L"");
	GetDlgItem(IDC_ED_FSN_F)->SetWindowText(str);
	str = sIni.GetString(L"SmoDem", L"Std5", L"");
	GetDlgItem(IDC_ED_FSN_5)->SetWindowText(str);
	str = sIni.GetString(L"SmoDem", L"Std6", L"");
	GetDlgItem(IDC_ED_FSN_6)->SetWindowText(str);
}

void CYD1CheckDlg::SaveFSNCheckParams(void)
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

	CSimpleIni sIni(strPath);
	CString str;
	GetDlgItem(IDC_ED_FSN_1)->GetWindowText(str);
	sIni.SetString(L"SmoDem", L"Std1", str);
	GetDlgItem(IDC_ED_FSN_2)->GetWindowText(str);
	sIni.SetString(L"SmoDem", L"Std2", str);
	GetDlgItem(IDC_ED_FSN_CAL)->GetWindowText(str);
	sIni.SetString(L"SmoDem", L"StdCal", str);
	GetDlgItem(IDC_ED_FSN_F)->GetWindowText(str);
	sIni.SetString(L"SmoDem", L"StdF", str);
	GetDlgItem(IDC_ED_FSN_5)->GetWindowText(str);
	sIni.SetString(L"SmoDem", L"Std5", str);
	GetDlgItem(IDC_ED_FSN_6)->GetWindowText(str);
	sIni.SetString(L"SmoDem", L"Std6", str);
}

void CYD1CheckDlg::ExportEquCalChkInfo(void)
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

	// ��ȡ����߱�źͷ������ͺ�
	CString strPathOfLineInfoFile = strPathOfDataExchange + L"\\LineInfo.ini";
	CSimpleIni siLineInfo(strPathOfLineInfoFile);
	CString strLineNumber = siLineInfo.GetString(L"LineInfo", L"LineNumber", L"");
	CString strSmokemeterModel = siLineInfo.GetString(L"LineInfo", L"SmokemeterModel", L"");

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
	str.Format(L"\r\nEquipment=%s", strSmokemeterModel);
	strResult += str;

	// ��Ŀ
	str.Format(L"\r\nItem=%s", L"��ֽʽ�̶ȼ��ظ��Ժ�ʾֵ�����");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strRepeatValue1,strRepeatValue2,strRepeatValue3,strRepeatValue4,strRepeatValue5;
	CString strBasicErr1Average,strBasicErr2Average,strBasicErrFAverage,strBasicErr5Average,strBasicErr6Average;

	GetDlgItem(IDC_ST_REPEAT_VALUE_1)->GetWindowText(strRepeatValue1);
	GetDlgItem(IDC_ST_REPEAT_VALUE_2)->GetWindowText(strRepeatValue2);
	GetDlgItem(IDC_ST_REPEAT_VALUE_3)->GetWindowText(strRepeatValue3);
	GetDlgItem(IDC_ST_REPEAT_VALUE_4)->GetWindowText(strRepeatValue4);
	GetDlgItem(IDC_ST_REPEAT_VALUE_5)->GetWindowText(strRepeatValue5);

	GetDlgItem(IDC_ST_BASICERR_1_AVERAGE)->GetWindowText(strBasicErr1Average);
	GetDlgItem(IDC_ST_BASICERR_2_AVERAGE)->GetWindowText(strBasicErr2Average);
	GetDlgItem(IDC_ST_BASICERR_F_AVERAGE)->GetWindowText(strBasicErrFAverage);
	GetDlgItem(IDC_ST_BASICERR_5_AVERAGE)->GetWindowText(strBasicErr5Average);
	GetDlgItem(IDC_ST_BASICERR_6_AVERAGE)->GetWindowText(strBasicErr6Average);

	CString strFormat;
	strFormat = L"�ظ���,����ֵ1:%s,����ֵ2:%s,����ֵ3:%s,����ֵ4:%s,����ֵ5:%s;ʾֵ���,1��ƽ��ֵ:%s,2��ƽ��ֵ:%s,��У׼��ƽ��ֵ:%s,5��ƽ��ֵ:%s,6��ƽ��ֵ:%s";
	CString strData;
	strData.Format(strFormat, strRepeatValue1, strRepeatValue2, strRepeatValue3, strRepeatValue4, strRepeatValue5,
		strBasicErr1Average, strBasicErr2Average, strBasicErrFAverage, strBasicErr5Average, strBasicErr6Average);
	str.Format(L"\r\nData=%s", strData);
	strResult += str;

	// ���
	CString strRepeatError1,strRepeatError2,strRepeatError3,strRepeatError4,strRepeatError5;
	CString strBasicErr1,strBasicErr2,strBasicErrF,strBasicErr5,strBasicErr6;

	GetDlgItem(IDC_ST_REPEAT_ERROR_1)->GetWindowText(strRepeatError1);
	GetDlgItem(IDC_ST_REPEAT_ERROR_2)->GetWindowText(strRepeatError2);
	GetDlgItem(IDC_ST_REPEAT_ERROR_3)->GetWindowText(strRepeatError3);
	GetDlgItem(IDC_ST_REPEAT_ERROR_4)->GetWindowText(strRepeatError4);
	GetDlgItem(IDC_ST_REPEAT_ERROR_5)->GetWindowText(strRepeatError5);

	GetDlgItem(IDC_ST_BASICERR_1)->GetWindowText(strBasicErr1);
	GetDlgItem(IDC_ST_BASICERR_2)->GetWindowText(strBasicErr2);
	GetDlgItem(IDC_ST_BASICERR_F)->GetWindowText(strBasicErrF);
	GetDlgItem(IDC_ST_BASICERR_5)->GetWindowText(strBasicErr5);
	GetDlgItem(IDC_ST_BASICERR_6)->GetWindowText(strBasicErr6);

	strFormat = L"�ظ���,ƫ��1:%s,ƫ��2:%s,ƫ��3:%s,ƫ��4:%s,ƫ��5:%s;ʾֵ���,1��ʾֵ���:%s,2��ʾֵ���:%s,��У׼��ʾֵ���:%s,5��ʾֵ���:%s,6��ʾֵ���:%s";

	CString strError;
	strError.Format(strFormat, strRepeatError1, strRepeatError2, strRepeatError3, strRepeatError4, strRepeatError5,
		strBasicErr1, strBasicErr2, strBasicErrF, strBasicErr5, strBasicErr6);
	str.Format(L"\r\nError=%s", strError);
	strResult += str;

	// �ж�
	CString strRepeatResult,strBasicErrorResult;
	GetDlgItem(IDC_ST_REPEAT_RESULT)->GetWindowText(strRepeatResult);
	GetDlgItem(IDC_ST_BASICERR_RESULT)->GetWindowText(strBasicErrorResult);

	bool bIsCheckPass(false);
	if (L"�ϸ�"==strRepeatResult && L"�ϸ�"==strBasicErrorResult)
	{
		bIsCheckPass = true;
	}

	str.Format(L"\r\nJudgement=%s", bIsCheckPass?L"ͨ��":L"��ͨ��");
	strResult += str;

	//���ݻ����ֵ��ض���ʽ�ı궨��¼������Reserved1�ֶ�////////////////////////////////////////////////////////
	CString strYD_std,strYD_val,strYD_err;
	GetDlgItem(IDC_ED_FSN_CAL)->GetWindowText(strYD_std);
	GetDlgItem(IDC_ST_REPEAT_AVERAGE)->GetWindowText(strYD_val);
	strYD_err.Format(L"%.2f", _wtof((LPCWSTR)strYD_val)-_wtof((LPCWSTR)strYD_std));
	str.Format(L"\r\nReserved1=%s;%s;%s", strYD_std, strYD_val, strYD_err);
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

void CYD1CheckDlg::ExportDemarcationLog(void)
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
	CString strSmokemeterNumber = siLineInfo.GetString(L"LineInfo", L"SmokemeterNumber", L"");
	CString strSmokemeterModel = siLineInfo.GetString(L"LineInfo", L"SmokemeterModel", L"");

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
	str.Format(L"\r\nEquipmentNumber=%s", strSmokemeterNumber);
	strDemarcationLog += str;

	// �豸�ͺ�
	str.Format(L"\r\nEquipmentModel=%s", strSmokemeterModel);
	strDemarcationLog += str;

	// �豸����
	str.Format(L"\r\nEquipmentName=%s", L"��ֽʽ�̶ȼ�");
	strDemarcationLog += str;

	// �궨��Ŀ
	str.Format(L"\r\nDemarcationItem=%d", 8);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;
	
	// У׼����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bPass?L"1":L"0");
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

void CYD1CheckDlg::ExportSmoChkResult(void)
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
	CString strResult(L"[ResultOfSmoChk]");
	CString strValue;
	
	// �궨���
	str.Format(L"\r\nDemarcationNumber=%s", strDemarcationNumber);
	strResult += str;

	// �ظ���-����̶ȿ�ֵ
	GetDlgItem(IDC_ED_FSN_CAL)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatStandardValue=%s", strValue);
	strResult += str;

	// �ظ���-ƽ��ֵ
	GetDlgItem(IDC_ST_REPEAT_AVERAGE)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatAverageValue=%s", strValue);
	strResult += str;

	// �ظ���-����ֵ1
	GetDlgItem(IDC_ST_REPEAT_VALUE_1)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatMeasuredValue1=%s", strValue);
	strResult += str;

	// �ظ���-����ֵ2
	GetDlgItem(IDC_ST_REPEAT_VALUE_2)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatMeasuredValue2=%s", strValue);
	strResult += str;

	// �ظ���-����ֵ3
	GetDlgItem(IDC_ST_REPEAT_VALUE_3)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatMeasuredValue3=%s", strValue);
	strResult += str;

	// �ظ���-����ֵ4
	GetDlgItem(IDC_ST_REPEAT_VALUE_4)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatMeasuredValue4=%s", strValue);
	strResult += str;

	// �ظ���-����ֵ5
	GetDlgItem(IDC_ST_REPEAT_VALUE_5)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatMeasuredValue5=%s", strValue);
	strResult += str;

	// �ظ���-ƫ��1
	GetDlgItem(IDC_ST_REPEAT_ERROR_1)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatErrorValue1=%s", strValue);
	strResult += str;

	// �ظ���-ƫ��2
	GetDlgItem(IDC_ST_REPEAT_ERROR_2)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatErrorValue2=%s", strValue);
	strResult += str;

	// �ظ���-ƫ��3
	GetDlgItem(IDC_ST_REPEAT_ERROR_3)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatErrorValue3=%s", strValue);
	strResult += str;

	// �ظ���-ƫ��4
	GetDlgItem(IDC_ST_REPEAT_ERROR_4)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatErrorValue4=%s", strValue);
	strResult += str;

	// �ظ���-ƫ��5
	GetDlgItem(IDC_ST_REPEAT_ERROR_5)->GetWindowText(strValue);
	str.Format(L"\r\nRepeatErrorValue5=%s", strValue);
	strResult += str;

	// �ظ���-���
	str.Format(L"\r\nRepeatPass=%s", m_bRepeatPass?L"1":L"0");
	strResult += str;

	// ʾֵ���-1������̶ȿ�ֵ
	GetDlgItem(IDC_ED_FSN_1)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError1StandardValue=%s", strValue);
	strResult += str;

	// ʾֵ���-1��ƽ��ֵ
	GetDlgItem(IDC_ST_BASICERR_1_AVERAGE)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError1AverageValue=%s", strValue);
	strResult += str;

	// ʾֵ���-1������ֵ1
	GetDlgItem(IDC_ST_BASICERR_1_1)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError1MeasuredValue1=%s", strValue);
	strResult += str;

	// ʾֵ���-1������ֵ2
	GetDlgItem(IDC_ST_BASICERR_1_2)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError1MeasuredValue2=%s", strValue);
	strResult += str;

	// ʾֵ���-1������ֵ3
	GetDlgItem(IDC_ST_BASICERR_1_3)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError1MeasuredValue3=%s", strValue);
	strResult += str;

	// ʾֵ���-2������̶ȿ�ֵ
	GetDlgItem(IDC_ED_FSN_2)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError2StandardValue=%s", strValue);
	strResult += str;

	// ʾֵ���-2��ƽ��ֵ
	GetDlgItem(IDC_ST_BASICERR_2_AVERAGE)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError2AverageValue=%s", strValue);
	strResult += str;

	// ʾֵ���-2������ֵ1
	GetDlgItem(IDC_ST_BASICERR_2_1)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError2MeasuredValue1=%s", strValue);
	strResult += str;

	// ʾֵ���-2������ֵ2
	GetDlgItem(IDC_ST_BASICERR_2_2)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError2MeasuredValue2=%s", strValue);
	strResult += str;

	// ʾֵ���-2������ֵ3
	GetDlgItem(IDC_ST_BASICERR_2_3)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError2MeasuredValue3=%s", strValue);
	strResult += str;

	// ʾֵ���-��У׼������̶ȿ�ֵ
	GetDlgItem(IDC_ED_FSN_F)->GetWindowText(strValue);
	str.Format(L"\r\nBasicErrorFStandardValue=%s", strValue);
	strResult += str;

	// ʾֵ���-��У׼��ƽ��ֵ
	GetDlgItem(IDC_ST_BASICERR_F_AVERAGE)->GetWindowText(strValue);
	str.Format(L"\r\nBasicErrorFAverageValue=%s", strValue);
	strResult += str;

	// ʾֵ���-��У׼������ֵ1
	GetDlgItem(IDC_ST_BASICERR_F_1)->GetWindowText(strValue);
	str.Format(L"\r\nBasicErrorFMeasuredValue1=%s", strValue);
	strResult += str;

	// ʾֵ���-��У׼������ֵ2
	GetDlgItem(IDC_ST_BASICERR_F_2)->GetWindowText(strValue);
	str.Format(L"\r\nBasicErrorFMeasuredValue2=%s", strValue);
	strResult += str;

	// ʾֵ���-��У׼������ֵ3
	GetDlgItem(IDC_ST_BASICERR_F_3)->GetWindowText(strValue);
	str.Format(L"\r\nBasicErrorFMeasuredValue3=%s", strValue);
	strResult += str;

	// ʾֵ���-5������̶ȿ�ֵ
	GetDlgItem(IDC_ED_FSN_5)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError5StandardValue=%s", strValue);
	strResult += str;

	// ʾֵ���-5��ƽ��ֵ
	GetDlgItem(IDC_ST_BASICERR_5_AVERAGE)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError5AverageValue=%s", strValue);
	strResult += str;

	// ʾֵ���-5������ֵ1
	GetDlgItem(IDC_ST_BASICERR_5_1)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError5MeasuredValue1=%s", strValue);
	strResult += str;

	// ʾֵ���-5������ֵ2
	GetDlgItem(IDC_ST_BASICERR_5_2)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError5MeasuredValue2=%s", strValue);
	strResult += str;

	// ʾֵ���-5������ֵ3
	GetDlgItem(IDC_ST_BASICERR_5_3)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError5MeasuredValue3=%s", strValue);
	strResult += str;

	// ʾֵ���-6������̶ȿ�ֵ
	GetDlgItem(IDC_ED_FSN_6)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError6StandardValue=%s", strValue);
	strResult += str;

	// ʾֵ���-6��ƽ��ֵ
	GetDlgItem(IDC_ST_BASICERR_6_AVERAGE)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError6AverageValue=%s", strValue);
	strResult += str;

	// ʾֵ���-6������ֵ1
	GetDlgItem(IDC_ST_BASICERR_6_1)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError6MeasuredValue1=%s", strValue);
	strResult += str;

	// ʾֵ���-6������ֵ2
	GetDlgItem(IDC_ST_BASICERR_6_2)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError6MeasuredValue2=%s", strValue);
	strResult += str;

	// ʾֵ���-6������ֵ3
	GetDlgItem(IDC_ST_BASICERR_6_3)->GetWindowText(strValue);
	str.Format(L"\r\nBasicError6MeasuredValue3=%s", strValue);
	strResult += str;

	// ʾֵ���-���
	str.Format(L"\r\nBasicErrorPass=%s", m_bBasicErrorPass?L"1":L"0");
	strResult += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bPass?L"1":L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	CString strPathOfSmoChkResult = strPathOfDataExchange + L"\\ResultOfSmoChk.ini";
	if (0 == _taccess(strPathOfSmoChkResult, 04))
	{
		// �ļ�����,����ֻ��
		// ȥ���ļ�ֻ������
		DWORD dwAttributes(0);
		dwAttributes = ::GetFileAttributes(strPathOfSmoChkResult);
		dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strPathOfSmoChkResult, dwAttributes);
	}
	CStdioFileEx sfe;
	sfe.Open(strPathOfSmoChkResult, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CYD1CheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}