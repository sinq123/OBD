// NHCReadWriteVarDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCReadWriteVarDlg.h"


// CNHCReadWriteVarDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCReadWriteVarDlg, CDialogZoom)

CNHCReadWriteVarDlg::CNHCReadWriteVarDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCReadWriteVarDlg::IDD, pParent)
	, m_nDlgFontSize(45)
{
	// �����⹦���߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// ���1280*1024�ֱ��ʽ��п���
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// ���ݷֱ���Y������е���
	m_nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
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

CNHCReadWriteVarDlg::~CNHCReadWriteVarDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCReadWriteVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VAR, m_lcVar);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FOR_WRITE, m_edNumberForWrite);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FOR_READ, m_edNumberForRead);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edValue);
	DDX_Control(pDX, IDC_PROGRESS_READ_ALL_VAR, m_pgRealAllVar);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCReadWriteVarDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_READ_VAR, &CNHCReadWriteVarDlg::OnBnClickedButtonReadVar)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_VAR, &CNHCReadWriteVarDlg::OnBnClickedButtonWriteVar)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHCReadWriteVarDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CNHCReadWriteVarDlg ��Ϣ�������

BOOL CNHCReadWriteVarDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCReadWriteVarDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCReadWriteVarDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();
	m_shdTitle.SetTitle(L"�⹦����д����");

	GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(FALSE);

	m_pgRealAllVar.ShowWindow(SW_HIDE);

	// �༭��ֻ����������
	m_edNumberForRead.ModifyStyle(0, ES_NUMBER);
	m_edNumberForWrite.ModifyStyle(0, ES_NUMBER);
	m_edValue.ModifyStyle(0, ES_NUMBER);

	m_lcVar.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	// ����1280*1024�ֱ��ʽ��п���,�������ķֱ�����,Ҫ���е���
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcVar.InsertColumn(0,  L"������",	LVCFMT_LEFT,	150*nSM_CXSCREEN/1280,		0);
	m_lcVar.InsertColumn(1,  L"����ֵ",	LVCFMT_LEFT,	250*nSM_CXSCREEN/1280,		1);
	m_lcVar.InsertColumn(2,  L"˵��",   LVCFMT_LEFT,	550*nSM_CXSCREEN/1280,		2);
	m_lcVar.InsertColumn(3,  L"��ֵ",   LVCFMT_LEFT,	300*nSM_CXSCREEN/1280,		3);
}

void CNHCReadWriteVarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCReadWriteVarDlg::OpenSerialPortAndTestCommunication(void)
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

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
	{
		// �򿪴��ڳɹ�

		// ����ͨѶ����
		// ��ȡ״̬����ͨѶ����
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(NULL))
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��д����"));
			GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(FALSE);
		}
	}
	else
	{
		// �򿪴���ʧ��
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(FALSE);
	}
}

void CNHCReadWriteVarDlg::OnBnClickedButtonReadVar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BeginWaitCursor();

	CString str(L"");
	GetDlgItem(IDC_EDIT_NUMBER_FOR_READ)->GetWindowText(str);
	if (L"" == str)
	{
		// ��ȫ������

		m_pgRealAllVar.ShowWindow(SW_SHOW);
		m_pgRealAllVar.SetRange(0, 249);

		m_lcVar.SetRedraw(FALSE);
		m_lcVar.DeleteAllItems();
		for (int i=0; i<=249; i++)
		{
			// �������ñ���
			if (120 == i)
			{
				i = 200;
			}

			WORD wVarContent;
			if (DYN_WR_SUCCEED == m_pDynThread->ReadVar((BYTE)i, &wVarContent))
			{
				// �������ɹ�
				str.Format(L"%d", i);
				int nItemIndex = m_lcVar.InsertItem(i, str);
				str.Format(L"%hu", wVarContent);
				m_lcVar.SetItemText(nItemIndex, 1, str);
			}
			else
			{
				m_lcVar.SetRedraw(TRUE);
				// �������쳣
				MessageBoxW(L"�������쳣", L"������", MB_OK|MB_ICONERROR);
				break;
			}
			m_pgRealAllVar.SetPos(i);
		}
		m_lcVar.SetRedraw(TRUE);
		m_pgRealAllVar.ShowWindow(SW_HIDE);
	}
	else
	{
		// ���ض�����

		// �������
		UINT un = _wtoi(str);
		if (un > 254)
		{
			MessageBoxW(L"��������ȷ�ı�����(0~254)", L"������", MB_OK|MB_ICONERROR);
			GetDlgItem(IDC_EDIT_NUMBER_FOR_READ)->SetFocus();
		}
		else if ((un>119 && un<200) || un>249)
		{
			MessageBoxW(L"�˱�������,�ݲ��ܶ�", L"������", MB_OK|MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_NUMBER_FOR_READ)->SetFocus();
		}
		else
		{
			m_lcVar.SetRedraw(FALSE);
			m_lcVar.DeleteAllItems();
			WORD wVarContent;
			if (DYN_WR_SUCCEED == m_pDynThread->ReadVar((BYTE)un, &wVarContent))
			{
				// �������ɹ�
				str.Format(L"%u", un);
				int nItemIndex = m_lcVar.InsertItem(un, str);
				str.Format(L"%hu", wVarContent);
				m_lcVar.SetItemText(nItemIndex, 1, str);
			}
			else
			{
				// �������쳣
				MessageBoxW(L"�������쳣", L"������", MB_OK|MB_ICONERROR);
			}
			m_lcVar.SetRedraw(TRUE);
		}
	}

	EndWaitCursor();
}

void CNHCReadWriteVarDlg::OnBnClickedButtonWriteVar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str(L"");

	// ��������
	GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->GetWindowTextW(str);
	if (L"" == str)
	{
		MessageBoxW(L"�����Ų���Ϊ��", L"д����", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->SetFocus();
		return;
	}
	UINT unNumber = _wtoi(str);
	if (unNumber > 254)
	{
		MessageBoxW(L"��������ȷ�ı�����(0~254)", L"������", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->SetFocus();
		return;
	}
	if ((unNumber>119 && unNumber<200) || unNumber>249)
	{
		MessageBoxW(L"�˱�������,�ݲ���д", L"д����", MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->SetFocus();
		return;
	}

	// ������ֵ
	GetDlgItem(IDC_EDIT_VALUE)->GetWindowTextW(str);
	if (L"" == str)
	{
		MessageBoxW(L"����ֵ����Ϊ��", L"д����", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
		return;
	}
	UINT unValue = _wtoi(str);
	if (unValue > 65535)
	{
		MessageBoxW(L"��������ȷ�ı���ֵ(0~65535)", L"д����", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
		return;
	}

	// ��ʽд����
	BeginWaitCursor();
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
	if(strDynProtocol == L"MAHA")
	{
		// ��д����,����9864
		m_pDynThread->WriteVar(0 , 9864);
	}
	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar((BYTE)unNumber, (WORD)unValue))
	{
		// ��ʾ�ոճɹ�д��ı���
		m_lcVar.SetRedraw(FALSE);
		m_lcVar.DeleteAllItems();
		WORD wVarContent;
		if (DYN_WR_SUCCEED == m_pDynThread->ReadVar((BYTE)unNumber, &wVarContent))
		{
			// �������ɹ�
			str.Format(L"%u", unNumber);
			int nItemIndex = m_lcVar.InsertItem(0, str);
			str.Format(L"%hu", wVarContent);
			m_lcVar.SetItemText(nItemIndex, 1, str);
		}
		m_lcVar.SetRedraw(TRUE);

		MessageBoxW(L"д�����ɹ�", L"д����", MB_OK);
	}
	else
	{
		MessageBoxW(L"д�����쳣", L"д����", MB_OK|MB_ICONERROR);
	}

	if(strDynProtocol == L"MAHA")
	{
		// �ر�д����
		m_pDynThread->WriteVar(0, 0);
	}
	EndWaitCursor();
}

void CNHCReadWriteVarDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}
