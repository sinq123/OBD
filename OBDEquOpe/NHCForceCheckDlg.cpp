// NHCForceCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NHCForceCheckDlg.h"
#include "math.h"



// CNHCForceCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CNHCForceCheckDlg, CDialogZoom)

CNHCForceCheckDlg::CNHCForceCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCForceCheckDlg::IDD, pParent)
{
	// �����⹦�������߳�
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

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

	m_pfProcessCtrl = NULL;
	m_bIsPass = true;
	m_bIsOperationFinishedAtLeaseOnce = false;
	m_nTestStep = 0;
	m_nZeroTime = 0;
}

CNHCForceCheckDlg::~CNHCForceCheckDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCForceCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_STEP1, m_lbStep1);
	DDX_Control(pDX, IDC_STATIC_STEP2, m_lbStep2);
	DDX_Control(pDX, IDC_STATIC_STEP3, m_lbStep3);
	DDX_Control(pDX, IDC_STATIC_STEP4, m_lbStep4);
	DDX_Control(pDX, IDC_STATIC_N, m_lbTorsion);
	DDX_Control(pDX, IDC_LIST_PROC, m_lsData);
	DDX_Control(pDX, IDC_COMBO_CHKTIME, m_cbCkTime);
	DDX_Control(pDX, IDC_COMBO_PROC, m_cbType);
	DDX_Control(pDX, IDC_COMBO_DATA, m_cbCkData);
	DDX_Control(pDX, IDC_STATIC_STEP, m_lbStep);
	DDX_Control(pDX, IDC_STATIC_TESTRESULT, m_lbResult);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CNHCForceCheckDlg, CDialogZoom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STARTCHK, &CNHCForceCheckDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNHCForceCheckDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_GETDATA, &CNHCForceCheckDlg::OnBnClickedButtonGetdata)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CNHCForceCheckDlg::OnBnClickedButtonCalculate)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CNHCForceCheckDlg::OnBnClickedButtonClear)
	ON_MESSAGE(WM_DYN_READ_VAR,OnReadVar)
END_MESSAGE_MAP()


// CNHCForceCheckDlg ��Ϣ�������

BOOL CNHCForceCheckDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ���ؼ�
	InitCtrls();

	// ��ʱ�򿪴���
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNHCForceCheckDlg::OpenSerialPortAndTestCommunication(void)
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
		std::bitset<16> bit16(0);
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(&bit16))
		{
			m_lbInfo.SetText(_T("�����ǰ����װУ׼����"));
			GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(TRUE);
		}
		else
		{
			m_lbInfo.SetText(_T("ͨѶ��ʱ"));
			GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
		GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(FALSE);
	}		
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CNHCForceCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			OpenSerialPortAndTestCommunication();
		}
		break;

	case 2:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1)
			{
				m_nZeroTime++;
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = 2;
					KillTimer(2);
					OnBnClickedButtonStart();
				}
			}
			else
			{
				m_nZeroTime = 0;
			}
		}
		break;

	case 3:
		{
			if (fabsf(m_pDynThread->GetFValue()) <= 1)
			{
				m_nZeroTime++;
				if (m_nZeroTime >= 2)//�ȶ�1��
				{
					m_nTestStep = 5;
					KillTimer(3);
					OnBnClickedButtonStart();
				}
			}
			else
				m_nZeroTime = 0;
		}
		break;

	case 10:
		{
			m_pDynThread->GetRealTimeData_Asyn(NULL);//��ȡʵʱ����

			CString str;
			str.Format(_T("%.1f"), m_pDynThread->GetFValue());
			m_lbTorsion.SetText(str);
		}
		break;
	default:
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCForceCheckDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCForceCheckDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"�⹦�������");

	m_lbInfo.SetText(_T("����ͨѶ..."));
	m_lbInfo.SetTitleStyle();

	m_lbStep.SetTextColor(RGB(0, 102, 204));
	m_lbResult.SetBkColor(RGB(230, 230, 230));

	m_lbTorsion.SetLEDStyle();
	//m_lbTorsion.SetFontName("LiquidCrystal");

	m_cbCkTime.SetCurSel(0);
	m_cbType.SetCurSel(0);

	m_lsData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CString strPath;
	// ��ȡexe(dll)�ļ�����·��
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// ����2��
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// �����ļ�·��
	strPath = strPath + L"\\Config\\Demarcation.ini";

	CSimpleIni si(strPath);
	m_strWeight = si.GetString(L"DynForceChk", L"ForceChk", L"0,600,1200,1800");

	StrToComb(m_strWeight, m_cbCkData);
	m_cbCkData.SetCurSel(1);

	int nCount=m_cbCkData.GetCount();
	m_lsData.InsertColumn(0, _T("����\\���ֵ"), 0, 0);
	m_lsData.SetColumnWidth(0, 165);
	for (int i=0; i<nCount; i++)
	{
		CString strTValue;
		m_cbCkData.GetLBText(i, strTValue);
		m_lsData.InsertColumn(i+1, strTValue, 0, 0);
	}
	for (int i=0; i<nCount; i++)
	{
		m_lsData.SetColumnWidth(i+1, 125);
	}
	ClearResult();

	ClearStep();
}

BOOL  CNHCForceCheckDlg::StrToComb(CString &strTemp,CComboBox &combTemp)
{
	combTemp.ResetContent();
	int Length = strTemp.GetLength();	
	int ePoint = 0; 
	for (int i=0; i<100; i++ )
	{
		ePoint = strTemp.Find(_T(","), 0);
		if (ePoint != -1)
		{
			combTemp.AddString(strTemp.Left(ePoint));
		}
		else 
		{
			combTemp.AddString(strTemp);			
			i = 100;		
		}
		strTemp = strTemp.Right(Length-ePoint-1);
		Length = strTemp.GetLength();		
	}
	return TRUE;
}

void CNHCForceCheckDlg::ClearResult()                                       //��ռ������
{
	m_lsData.SetRedraw(FALSE);
	m_lsData.DeleteAllItems();
	m_lsData.InsertItem(0, _T("����ֵ1(��)"));
	m_lsData.InsertItem(1, _T("����ֵ1(��)"));
	m_lsData.InsertItem(2, _T("����ֵ2(��)"));
	m_lsData.InsertItem(3, _T("����ֵ2(��)"));
	m_lsData.InsertItem(4, _T("����ֵ3(��)"));
	m_lsData.InsertItem(5, _T("����ֵ3(��)"));
	m_lsData.InsertItem(6, _T("ƽ��ֵ(��)"));
	m_lsData.InsertItem(7, _T("ƽ��ֵ(��)"));
	m_lsData.InsertItem(8, _T("�������[N]"));
	m_lsData.InsertItem(9, _T("������[%]"));
	m_lsData.InsertItem(10, _T("�ж�"));


	m_lsData.SetItemText(0, 1, _T("--"));
	m_lsData.SetItemText(2, 1, _T("--"));
	m_lsData.SetItemText(4, 1, _T("--"));
	m_lsData.SetItemText(6, 1, _T("--"));
	m_lsData.SetItemText(8, 1, _T("--"));
	m_lsData.SetItemText(9, 1, _T("--"));

	int ncol = m_cbCkData.GetCount();
	m_lsData.SetItemText(1, ncol, _T("--"));
	m_lsData.SetItemText(3, ncol, _T("--"));
	m_lsData.SetItemText(5, ncol, _T("--"));
	m_lsData.SetItemText(7, ncol, _T("--"));

	m_lsData.SetRedraw(TRUE);

	m_cbCkData.SetCurSel(1);
	m_cbCkTime.SetCurSel(0);
	m_cbType.SetCurSel(0);

	m_lbTorsion.SetText(L"--");

	m_lbResult.SetText(L"--");
	m_bIsPass = true;
}

void  CNHCForceCheckDlg::ClearStep()        //�����������
{
	m_lbInfo.StopFlashText();
	m_lbStep1.SetTextColor(RGB(0, 0, 0));
	m_lbStep1.SetText(L"   ��װУ׼����");
	m_lbStep2.SetTextColor(RGB(0, 0, 0));	
	m_lbStep2.SetText(L"   ��ʼ��鲢��¼����ֵ");
	m_lbStep3.SetTextColor(RGB(0, 0, 0));	
	m_lbStep3.SetText(L"   ��ȥУ׼����");
	m_lbStep4.SetTextColor(RGB(0, 0, 0));	
	m_lbStep4.SetText(L"   ���");
}

void CNHCForceCheckDlg::OnBnClickedButtonStart()
{
	GetDlgItem(IDC_BUTTON_GETDATA)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CALCULATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);

	if (m_nTestStep == 0)
	{
		m_lbInfo.SetText(_T("�����ǰ����װУ׼����"));

		if (MessageBox(_T("ȷ�Ͽ�ʼ�������?"), _T("�����"), MB_OKCANCEL) == IDOK)
		{
			// �ص������Ϣ
			if (NULL != m_pfProcessCtrl)
			{
				wchar_t wchInfo[256] = {0};
				wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦������鿪ʼ", _countof(wchInfo)-1);
				m_pfProcessCtrl(NH_DYN_FORCE_CHK_START, wchInfo, sizeof(wchInfo));
			}

			m_pDynThread->ReadVar_Asyn(6, m_hWnd);

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

			ClearResult();
			ClearStep();

			GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("��һ��"));

			m_pDynThread->SendCtrlWord_Asyn('0');//�˳����п���ģʽ
			m_pDynThread->SendCtrlWord_Asyn('U');//�ر��Զ���λ����

			m_lbStep1.SetTextColor(RGB(0, 0, 255));
			m_lbInfo.SetText(_T("�밲װ��У׼���ߺ����һ��"));

			m_nTestStep = 1;
            return;
		}
		else
		{
			return;
		}
	}
	else if (m_nTestStep == 1)        //У׼����
	{
		// �ж��Ƿ��Ѿ���װУ׼����
		if (fabs(m_pDynThread->GetFValue()) < 50)
		{
			if (MessageBox(_T("�밲װУ׼���ߺ��ȷ��!"), _T("�����"), MB_OKCANCEL) != IDOK)
			{
				m_nTestStep = 4;
				m_lbStep1.SetTextColor(RGB(0, 0, 0));
				m_lbStep1.SetText(L"�� ��װУ׼����");
				m_lbStep2.SetTextColor(RGB(0, 0, 0));
				m_lbStep2.SetText(L"�� ��ʼ��鲢��¼����ֵ");
				m_lbStep3.SetTextColor(RGB(0, 0, 255));
				return;
			}
			OnBnClickedButtonStart();
			return;
		}
		m_lbStep1.SetTextColor(RGB(0, 0, 0));
		m_lbStep1.SetText(L"�� ��װУ׼����");

		m_lbInfo.SetText(_T("���ڵ���..."));
		m_pDynThread->SendCtrlWord_Asyn('T');	//���Զ���λ����
		GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		SetTimer(2, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == 2)        
	{
		m_lbInfo.SetText(_T("��ʼ��鲢��¼����ֵ"));
		m_lbStep2.SetTextColor(RGB(0, 0, 255));
		m_pDynThread->SendCtrlWord_Asyn('U');	//�ر��Զ���λ����
		GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("������¼"));
		m_nTestStep = 3;
	}
	else if (m_nTestStep == 3)       
	{
		OnBnClickedButtonCalculate(); //�������
		m_lbStep2.SetTextColor(RGB(0, 0, 0));
		m_lbStep2.SetText(L"�� ��ʼ��鲢��¼����ֵ");
		m_lbInfo.SetText(_T("���ȥУ׼���ߺ����һ��"));
		m_lbStep3.SetTextColor(RGB(0, 0, 255));
		GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("��һ��"));
		m_nTestStep = 4;
	}
	else if (m_nTestStep == 4)       
	{
		m_lbStep3.SetTextColor(RGB(0, 0, 0));
		m_lbStep3.SetText(L"�� ��ȥУ׼����");
		m_lbInfo.SetText(_T("���ڵ���..."));
		m_lbStep4.SetTextColor(RGB(0, 0, 255));

		GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		m_pDynThread->SendCtrlWord_Asyn('T');//���Զ���λ����
		SetTimer(3, 500, NULL);
		m_nZeroTime = 0;
	}
	else if (m_nTestStep == 5)       
	{
		GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("��ɼ��"));
		m_lbInfo.SetText(_T("������,���'��ɼ��'���������"));

		m_pDynThread->SendCtrlWord_Asyn('V');//���Զ�-ƫ��-������Ϊ����

		m_nTestStep = 6;

		OnBnClickedButtonStart();
	}
	else if (m_nTestStep == 6)           //������
	{
		m_lbStep4.SetTextColor(RGB(0, 0, 0));
		m_lbStep4.SetText(L"�� ���");

		m_lbInfo.SetText(_T("�����ǰ����װУ׼����"));
		GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("��ʼ"));
		m_nTestStep = 0;

		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);	
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	

		OnBnClickedButtonCalculate();
		m_bIsOperationFinishedAtLeaseOnce = true;	//��¼�������

		//ExportEquCalChkInfo();			//�������
		//ExportDemarcationLog();			//�����궨��¼
		ExportDynForceCheckResult();	//�������

		// �ص������Ϣ
		if (NULL != m_pfProcessCtrl)
		{
			wchar_t wchInfo[256] = {0};
			wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦����������", _countof(wchInfo)-1);
			m_pfProcessCtrl(NH_DYN_FORCE_CHK_FINISH, wchInfo, sizeof(wchInfo));
		}
	}
}

void CNHCForceCheckDlg::OnBnClickedButtonStop()
{
	if (MessageBox(_T("�Ƿ�Ҫֹͣ����飿\n��ʾ���ڵ��ȷ��ǰ���������������У׼���ߣ�"), _T("�����"), MB_OKCANCEL) != IDOK)
	{
		return;
	}

	// �ص������Ϣ
	if (NULL != m_pfProcessCtrl)
	{
		wchar_t wchInfo[256] = {0};
		wcsncpy_s(wchInfo, _countof(wchInfo), L"�⹦�������ֹͣ", _countof(wchInfo)-1);
		m_pfProcessCtrl(NH_DYN_FORCE_CHK_STOP, wchInfo, sizeof(wchInfo));
	}

	m_pDynThread->SendCtrlWord_Asyn('T');	//���Զ���λ����
	m_pDynThread->SendCtrlWord_Asyn('V');	//���Զ�-ƫ��-������Ϊ����
	
	m_lbInfo.SetText(_T("�������ֹͣ"));
	GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("��ʼ"));
	GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
	
	ClearStep();
	m_nTestStep = 0;

	//m_pDynThread->SendCtrlWord_Asyn('T');//���Զ���λ����
	//m_lbInfo.SetText(_T("���ڵ���..."));
	//SetTimer(4, 500, NULL);
	//m_nZeroTime = 0;
	//GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
}

void CNHCForceCheckDlg::OnBnClickedButtonGetdata()
{
	int nCkTime, nType, nTValue;
	CString strCkTime, strType, strTValue;
	nCkTime=m_cbCkTime.GetCurSel();
	nType=m_cbType.GetCurSel();
	nTValue=m_cbCkData.GetCurSel();

	m_cbCkTime.GetLBText(nCkTime, strCkTime);
	m_cbType.GetLBText(nType, strType);
	m_cbCkData.GetLBText(nTValue, strTValue);

	if (strType == _T("����"))
	{
		if (m_cbCkData.GetCurSel() == 0)
		{
			KillTimer(10);
			MessageBox(_T("���̲���ѡ0!"), L"�����");
			SetTimer(10, 500, NULL);
			return;
		}
		m_lsData.SetItemText(2*nCkTime, nTValue+1, m_lbTorsion.GetText());
	}
	else if (strType == _T("�س�"))
	{
		if (m_cbCkData.GetCurSel() == m_cbCkData.GetCount()-1)
		{
			KillTimer(10);
			CString str;
			str.Format(_T("�س̲���ѡ���ֵ%s"), strTValue);
			MessageBox(str, L"�����");
			SetTimer(10, 500, NULL);
			return;
		}
		m_lsData.SetItemText(2*nCkTime+1, nTValue+1, m_lbTorsion.GetText());
	}

	if (strType == _T("����"))
	{
		if (nTValue < m_cbCkData.GetCount()-1)
			m_cbCkData.SetCurSel(nTValue+1);
		else
		{
			m_cbType.SetCurSel(1);
            m_cbCkData.SetCurSel(nTValue-1);
		}
	}
	else if (strType == _T("�س�"))
	{
		if (nTValue > 0)
			m_cbCkData.SetCurSel(nTValue-1);
		else
		{
			m_cbType.SetCurSel(0);
            m_cbCkData.SetCurSel(nTValue+1);
			if (nCkTime == m_cbCkTime.GetCount()-1)
			{
				nCkTime = 0;
			}
			else
			{
				nCkTime++;
			}
			m_cbCkTime.SetCurSel(nCkTime);
		}
	}
}

void CNHCForceCheckDlg::OnBnClickedButtonCalculate()
{
	m_bIsPass = true; //��Ϊ�ϸ�
	double dAvg = 0;		//����ƽ��ֵ
	double dAvg2 = 0;       //�س�ƽ��ֵ
	double dErr = 0;        //���
	double dMax = 0;
	double dMin = 0;
	CString str1, str2, str3, strChk, str_avg, strErr;

	for (int i=1; i<=m_cbCkData.GetCount(); i++)
	{
		dAvg = 0;
		dAvg2 = 0; 
		dErr = 0;
		m_cbCkData.GetLBText(i-1, strChk);
		if (i != 1)//����
		{
			str1 = m_lsData.GetItemText(0, i);
			str2 = m_lsData.GetItemText(2, i);
			str3 = m_lsData.GetItemText(4, i);
			dAvg += _wtof(str1) + _wtof(str2) + _wtof(str3);
			dAvg = dAvg / 3;
			str_avg.Format(_T("%.2f"), dAvg);
			m_lsData.SetItemText(6, i, str_avg);

			//ʾֵ�������
			dErr = dAvg - _wtof(strChk);
			strErr.Format(_T("%.2f"), dErr);

			m_lsData.SetItemText(8, i, strErr);

			//ʾֵ������
			dErr = (dAvg / _wtof(strChk) - 1) * 100;
			strErr.Format(_T("%.2f"), dErr);

			m_lsData.SetItemText(9, i, strErr);
			
			//������>1%   �������>20N
			if (abs(_wtof(m_lsData.GetItemText(8, i))) > 20.0f && abs(_wtof(m_lsData.GetItemText(9, i))) > 1.0f)	
			{
				m_bIsPass = false;
				m_lsData.SetItemText(10, i, L"���ϸ�");
				
			}
			else
			{
				m_lsData.SetItemText(10, i, L"�ϸ�");
			}

			dMax = _wtof(str1);
			if (dMax < _wtof(str2)) 
			{
				dMax = _wtof(str2);
			}
			if (dMax < _wtof(str3))
			{
				dMax = _wtof(str3);
			}

			dMin = _wtof(str1);
			if (dMin > _wtof(str2)) 
			{
				dMin = _wtof(str2);
			}
			if (dMin > _wtof(str3)) 
			{
				dMin = _wtof(str3);
			}
		}

		if (i != m_cbCkData.GetCount())			//�س�
		{
			str1 = m_lsData.GetItemText(1, i);
			str2 = m_lsData.GetItemText(3, i);
			str3 = m_lsData.GetItemText(5, i);
			dAvg2 += _wtof(str1) + _wtof(str2) + _wtof(str3);
			dAvg2 = dAvg2 / 3;
			str_avg.Format(_T("%.2f"), dAvg2);
			m_lsData.SetItemText(7, i, str_avg);
		}
	}
	if (m_bIsPass) 
	{
		m_lbResult.SetTextColor(RGB(0, 0, 0));
		GetDlgItem(IDC_STATIC_TESTRESULT)->SetWindowText(_T("ͨ��"));
	}
	else
	{
		m_lbResult.SetTextColor(RGB(255, 0, 0));
		GetDlgItem(IDC_STATIC_TESTRESULT)->SetWindowText(_T("��ͨ��"));
	}
}

void CNHCForceCheckDlg::OnBnClickedButtonClear()
{
	ClearResult();
}

LRESULT  CNHCForceCheckDlg::OnReadVar(WPARAM bSucess,LPARAM lParam)          //��ȡ�⹦��״̬
{
	if (bSucess != DYN_WR_SUCCEED)
	{
		GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
		m_lbInfo.SetText(_T("��⹦��ͨѶ��ʱ"));
		GetDlgItem(IDC_BUTTON_STARTCHK)->SetWindowText(_T("��ʼ"));
		m_nTestStep = 0;
		ClearStep(); 
	}
	else
	{
		if ((int)lParam == 6)
		{
			GetDlgItem(IDC_BUTTON_STARTCHK)->EnableWindow(TRUE);
			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
			SetTimer(10, 500, NULL);
		}
	}
	return 0;
}

/*
void CNHCForceCheckDlg::ExportEquCalChkInfo(void)
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
	str.Format(L"\r\nItem=%s", L"�⹦�������");
	strResult += str;

	// ����
	COleDateTime odt = COleDateTime::GetCurrentTime();
	str.Format(L"\r\nDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;

	// ����
	CString strInAvg1 = m_lsData.GetItemText(6, 2);		//1���������ƽ��ֵ(��)
	CString strInAvg2 = m_lsData.GetItemText(6, 3);		//2���������ƽ��ֵ(��)
	CString strOutAvg1 = m_lsData.GetItemText(7, 1);	//0���������ƽ��ֵ(��)
	CString strOutAvg2 = m_lsData.GetItemText(7, 2);	//1���������ƽ��ֵ(��)
	str.Format(L"\r\nData=ƽ��ֵ1(��):%sN,ƽ��ֵ2(��):%sN,ƽ��ֵ1(��):%sN,ƽ��ֵ2(��):%sN", strInAvg1, strInAvg2, strOutAvg1, strOutAvg2);
	strResult += str;

	// ���
	CString strAE1 = m_lsData.GetItemText(8, 2);	//������� 1������
	CString strAE2 = m_lsData.GetItemText(8, 3);	//������� 2������
	CString strRE1 = m_lsData.GetItemText(9, 2);	//������ 1������
	CString strRE2 = m_lsData.GetItemText(9, 3);	//������ 2������
	str.Format(L"\r\nError=�������1:%sN,�������2:%sN,������1:%s%%,������2:%s%%", strAE1, strAE2, strRE1, strRE2);
	strResult += str;

	// �ж�
	if (m_bIsPass)
	{
		str.Format(L"\r\nJudgement=%s", L"ͨ��");
	}
	else
	{	
		str.Format(L"\r\nJudgement=%s", L"��ͨ��");
	}
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
void CNHCForceCheckDlg::ExportDemarcationLog(void)
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
	str.Format(L"\r\nDemarcationItem=%d", DYN_FORCE_CHK);
	strDemarcationLog += str;

	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strDemarcationLog += str;

	// ����Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass?L"1":L"0");
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

void CNHCForceCheckDlg::ExportDynForceCheckResult(void)
{	
	CString str;
	CString strResult(L"[ResultOfDynForceChk]");

	// ��ȡ��ǰʱ��
	COleDateTime odt = COleDateTime::GetCurrentTime();
	// �궨����
	str.Format(L"\r\nDemarcationDate=%s", odt.Format(L"%Y-%m-%d %H:%M:%S"));
	strResult += str;
	
	// ���ʹ�õ���������
	str.Format(L"\r\nWeightCount=%d", m_cbCkData.GetCount()-1);
	strResult += str;
	
	for (int i=1; i<m_cbCkData.GetCount(); i++)
	{
		TCHAR szTitle[MAX_PATH] = {0};
		LVCOLUMN lvCol;
		lvCol.mask = LVCF_TEXT;
		lvCol.pszText = szTitle;
		lvCol.cchTextMax = MAX_PATH;
		m_lsData.GetColumn(i+1 , &lvCol);

		// ��׼ֵ
		float fStdValue = (float)_wtof(lvCol.pszText);
		str.Format(L"\r\nStdValue%d=%.0f", i, fStdValue);
		strResult += str;

		// ����ƽ��ֵ
		float fForwardAvgValue = (float)_wtof(m_lsData.GetItemText(6, i+1));
		str.Format(L"\r\nForwardAvgValue%d=%.2f", i, fForwardAvgValue);
		strResult += str;

		// �س�ƽ��ֵ
		float fBackwardAvgValue = (float)_wtof(m_lsData.GetItemText(7, i));
		str.Format(L"\r\nBackwardAvgValue%d=%.2f", i, fBackwardAvgValue);
		strResult += str;

		// �������
		str.Format(L"\r\nAE%d=%.2f", i, fForwardAvgValue-fStdValue);
		strResult += str;

		// ������
		str.Format(L"\r\nRE%d=%.2f", i, (fForwardAvgValue-fStdValue)/fStdValue*100);
		strResult += str;
	}

	// У׼�Ƿ�ͨ��
	str.Format(L"\r\nPass=%s", m_bIsPass?L"1":L"0");
	strResult += str;

	// д�뵽���ini�ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynForceChk.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(strResult);
	sfe.Close();
}

void CNHCForceCheckDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen))
{
	m_pfProcessCtrl = pfProcessCtrl;
}