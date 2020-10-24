// NHA509Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ACSCheck.h"
#include "NHA509Dlg.h"
#include "afxdialogex.h"


// CNHA509Dlg �Ի���

IMPLEMENT_DYNAMIC(CNHA509Dlg, CDialogZoom)

	CNHA509Dlg::CNHA509Dlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHA509Dlg::IDD, pParent)
{

	// �����������߳�
	//m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	m_pAnaThread = CNHEuqController::GetInstance().m_pAnaThread;

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
}

CNHA509Dlg::~CNHA509Dlg()
{
	m_fontDlgFont.DeleteObject();
}

void CNHA509Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_ST_HEAD, m_lbHead);

	DDX_Control(pDX, IDC_MATHLABLE2, m_mlb1);
	DDX_Control(pDX, IDC_MATHLABLE13, m_mlb2);
	DDX_Control(pDX, IDC_MATHLABLE3, m_mlb3);
	DDX_Control(pDX, IDC_MATHLABLE14, m_mlb4);
	DDX_Control(pDX, IDC_MATHLABLE4, m_mlb5);
	DDX_Control(pDX, IDC_MATHLABLE19, m_mlb6);
	DDX_Control(pDX, IDC_STATIC_HC, m_lbHC);
	DDX_Control(pDX, IDC_STATIC_CO, m_lbCO);
	DDX_Control(pDX, IDC_STATIC_NO, m_lbNO);
	DDX_Control(pDX, IDC_STATIC_CO2, m_lbCO2);
	DDX_Control(pDX, IDC_STATIC_O2, m_lbO2);
	DDX_Control(pDX, IDC_STATIC_PEF, m_lbPEF);
	DDX_Control(pDX, IDC_STATIC_HC_MEASURED_VALUE, m_lbHCMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO_MEASURED_VALUE, m_lbCOMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_CO2_MEASURED_VALUE, m_lbCO2MeasuredValue);
	DDX_Control(pDX, IDC_STATIC_NO_MEASURED_VALUE, m_lbNOMeasuredValue);
	DDX_Control(pDX, IDC_STATIC_O2_MEASURED_VALUE, m_lbO2MeasuredValue);
	DDX_Control(pDX, IDC_STATIC_HC_RE2, m_lbHCAE);
	DDX_Control(pDX, IDC_STATIC_CO_RE2, m_lbCOAE);
	DDX_Control(pDX, IDC_STATIC_CO2_RE2, m_lbCO2AE);
	DDX_Control(pDX, IDC_STATIC_NO_RE2, m_lbNOAE);
	DDX_Control(pDX, IDC_STATIC_O2_RE2, m_lbO2AE);
	DDX_Control(pDX, IDC_STATIC_HC_RE, m_lbHCRE);
	DDX_Control(pDX, IDC_STATIC_CO_RE, m_lbCORE);
	DDX_Control(pDX, IDC_STATIC_CO2_RE, m_lbCO2RE);
	DDX_Control(pDX, IDC_STATIC_NO_RE, m_lbNORE);
	DDX_Control(pDX, IDC_STATIC_O2_RE, m_lbO2RE);
	DDX_Control(pDX, IDC_STATIC_HC_JUDGEMENT, m_lbHCJudgement);
	DDX_Control(pDX, IDC_STATIC_CO_JUDGEMENT, m_lbCOJudgement);
	DDX_Control(pDX, IDC_STATIC_CO2_JUDGEMENT, m_lbCO2Judgement);
	DDX_Control(pDX, IDC_STATIC_NO_JUDGEMENT, m_lbNOJudgement);
	DDX_Control(pDX, IDC_STATIC_O2_JUDGEMENT, m_lbO2Judgement);
	DDX_Control(pDX, IDC_STATIC_HC_RE3, m_lbHCAE2);
	DDX_Control(pDX, IDC_STATIC_CO_RE3, m_lbCOAE2);
	DDX_Control(pDX, IDC_STATIC_CO2_RE3, m_lbCO2AE2);
	DDX_Control(pDX, IDC_STATIC_NO_RE3, m_lbNOAE2);
	DDX_Control(pDX, IDC_STATIC_O2_RE3, m_lbO2AE2);
	DDX_Control(pDX, IDC_STATIC_HC_RE4, m_lbHCRE2);
	DDX_Control(pDX, IDC_STATIC_CO_RE4, m_lbCORE2);
	DDX_Control(pDX, IDC_STATIC_CO2_RE4, m_lbCO2RE2);
	DDX_Control(pDX, IDC_STATIC_NO_RE4, m_lbNORE2);
	DDX_Control(pDX, IDC_STATIC_O2_RE4, m_lbO2RE2);
	DDX_Control(pDX, IDC_COM_LINEAR_DETECTION, m_cbLinearDet);
	DDX_Control(pDX, IDC_COM_CHECK_IN_FIVE, m_cbChechInFive);
	DDX_Control(pDX, IDC_ST_TYPR_OF_EQUIPMENT, m_lbTyprOfEqu);
	DDX_Control(pDX, IDC_EDIT_HC_HCMINAL_VALUE, m_edHCNominalValue);
	DDX_Control(pDX, IDC_EDIT_CO_NOMINAL_VALUE, m_edCONominalValue);
	DDX_Control(pDX, IDC_EDIT_NO_NOMINAL_VALUE, m_edNONominalValue);
	DDX_Control(pDX, IDC_EDIT_CO2_NOMINAL_VALUE, m_edCO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_O2_NOMINAL_VALUE, m_edO2NominalValue);
	DDX_Control(pDX, IDC_EDIT_C3H8_NOMINAL_VALUE, m_edC3H8NominalValue);
	DDX_Control(pDX, IDC_STATIC_HC_HCMINAL_VALUE, m_lbSJHC1);
	DDX_Control(pDX, IDC_STATIC_HC_MEASURED_VALUE2, m_lbSJHC2);
	DDX_Control(pDX, IDC_STATIC_HC_RE5, m_lbSJHC3);
	DDX_Control(pDX, IDC_STATIC_HC_RE6, m_lbSJHC4);
	DDX_Control(pDX, IDC_STATIC_HC_JUDGEMENT2, m_lbSJHC5);
	DDX_Control(pDX, IDC_STATIC_CO_NOMINAL_VALUE, m_lbSJCO1);
	DDX_Control(pDX, IDC_STATIC_CO_MEASURED_VALUE2, m_lbSJCO2);
	DDX_Control(pDX, IDC_STATIC_CO_RE5, m_lbSJCO3);
	DDX_Control(pDX, IDC_STATIC_CO_RE6, m_lbSJCO4);
	DDX_Control(pDX, IDC_STATIC_CO_JUDGEMENT2, m_lbSJCO5);
	DDX_Control(pDX, IDC_STATIC_CO2_NOMINAL_VALUE, m_lbSJCO21);
	DDX_Control(pDX, IDC_STATIC_CO2_MEASURED_VALUE2, m_lbSJCO22);
	DDX_Control(pDX, IDC_STATIC_CO2_RE5, m_lbSJCO23);
	DDX_Control(pDX, IDC_STATIC_CO2_RE6, m_lbSJCO24);
	DDX_Control(pDX, IDC_STATIC_CO2_JUDGEMENT2, m_lbSJCO25);
	DDX_Control(pDX, IDC_STATIC_NO_NOMINAL_VALUE, m_lbSJNO1);
	DDX_Control(pDX, IDC_STATIC_NO_MEASURED_VALUE2, m_lbSJNO2);
	DDX_Control(pDX, IDC_STATIC_NO_RE5, m_lbSJNO3);
	DDX_Control(pDX, IDC_STATIC_NO_RE6, m_lbSJNO4);
	DDX_Control(pDX, IDC_STATIC_NO_JUDGEMENT2, m_lbSJNO5);
	DDX_Control(pDX, IDC_STATIC_O2_NOMINAL_VALUE, m_lbSJO21);
	DDX_Control(pDX, IDC_STATIC_O2_MEASURED_VALUE2, m_lbSJO22);
	DDX_Control(pDX, IDC_STATIC_O2_RE5, m_lbSJO23);
	DDX_Control(pDX, IDC_STATIC_O2_RE6, m_lbSJO24);
	DDX_Control(pDX, IDC_STATIC_O2_JUDGEMENT2, m_lbSJO25);
	DDX_Control(pDX, IDC_STATIC_STATION, m_lbStation);
}


BEGIN_MESSAGE_MAP(CNHA509Dlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BTN_INS_SAM, &CNHA509Dlg::OnBnClickedBtnInsSam)
	ON_BN_CLICKED(IDC_BTN_INS_ZERO, &CNHA509Dlg::OnBnClickedBtnInsZero)
	ON_BN_CLICKED(IDC_BTN_INS_RESET, &CNHA509Dlg::OnBnClickedBtnInsReset)
	ON_BN_CLICKED(IDC_BTN_INS_BLO, &CNHA509Dlg::OnBnClickedBtnInsBlo)
	ON_BN_CLICKED(IDC_BTN_LINEAR_DETECTION, &CNHA509Dlg::OnBnClickedBtnLinearDetection)
	ON_BN_CLICKED(IDC_BTN_LINEAR_CHECK_OUT, &CNHA509Dlg::OnBnClickedBtnLinearCheckOut)
	ON_BN_CLICKED(IDC_BTN_CHECK_IN_FIVE, &CNHA509Dlg::OnBnClickedBtnCheckInFive)
	ON_BN_CLICKED(IDC_BTN_CHECK_OUT_FIVE, &CNHA509Dlg::OnBnClickedBtnCheckOutFive)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNHA509Dlg ��Ϣ�������


void CNHA509Dlg::OnBnClickedBtnInsSam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nSel;
	nSel = m_cbLinearDet.GetCurSel();

	if (nSel == -1)
	{
		MessageBox(L"��ѡ�����Լ���");
		return;
	}

	CString strName;
	GetDlgItem(IDC_BTN_INS_SAM)->GetWindowTextW(strName);

	if (strName == L"��������")
	{
		// ��¼��ʼʱ��
		m_odtStart = COleDateTime::GetCurrentTime();
		if (GetNominalValue() != 0x00)
		{
			return;
		}

		if (m_strConfigIsNH == L"1")
		{
			// ������
			m_pAnaThread->OpenCheckGas();
			// ��ǰִ��
			m_pAnaThread->GetPef_Asyn(NULL);
		}
		// ˢ������
		SetTimer(4, 500, NULL);

		GetDlgItem(IDC_BTN_INS_SAM)->SetWindowTextW(L"ֹͣ����");
	}
	else
	{
		if (m_strConfigIsNH == L"1")
		{
			// �ر�����
			m_pAnaThread->CloseCheckGas();
		}
		KillTimer(4);

		bool bRet(false);
		// ������
		bRet = CalculateResult();

		GetDlgItem(IDC_BTN_INS_SAM)->SetWindowTextW(L"��������");

		// ��¼����ʱ��
		m_odtEnd = COleDateTime::GetCurrentTime();
#ifndef _DEBUG
		// �����ɹ����ϴ�
		if (bRet)
		{
			UpIntwqfxybd();
		}
#endif

	}
}


void CNHA509Dlg::OnBnClickedBtnInsZero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �ϻ��Ĳ�ʹ��ͨѶ����
	if (m_strConfigIsNH == L"1")
	{
		if (ANA_WR_SUCCEED == m_pAnaThread->Zero())
		{
			m_lbInfo.SetText(_T("���ڵ���..."));
			// ��Ϊ�����,����������Ӧ��һ����
			SetTimer(3, 2000, NULL);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("���ڵ���..."));
		SetTimer(5, 2000, NULL);
	}

}


void CNHA509Dlg::OnBnClickedBtnInsReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_strConfigIsNH == L"1")
	{
		m_pAnaThread->StopPump();
	}
}


void CNHA509Dlg::OnBnClickedBtnInsBlo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName;
	GetDlgItem(IDC_BTN_INS_BLO)->GetWindowTextW(strName);

	if (strName == L"��������")
	{
		if (m_strConfigIsNH == L"1")
		{
			m_pAnaThread->InverseBlow();
		}
		GetDlgItem(IDC_BTN_INS_BLO)->SetWindowTextW(L"ֹͣ����");
	}
	else
	{
		if (m_strConfigIsNH == L"1")
		{
			// ͣ��
			m_pAnaThread->StopPump();
		}
		GetDlgItem(IDC_BTN_INS_BLO)->SetWindowTextW(L"��������");
	}
}


void CNHA509Dlg::OnBnClickedBtnLinearDetection()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNHA509Dlg::OnBnClickedBtnLinearCheckOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


void CNHA509Dlg::OnBnClickedBtnCheckInFive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNHA509Dlg::OnBnClickedBtnCheckOutFive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


BOOL CNHA509Dlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	LoadConfig();

	InitCtrls();
	// �����ʾ����
	ShowWindow(SW_MAXIMIZE);
	// ˢ��ʱ��
	SetTimer(1, 1000, NULL);
	// �ϻ��Ĳ�ʹ��ͨѶ����
	if (m_strConfigIsNH == L"1")
	{
		// ��ʱ�򿪴���
		SetTimer(2, 500, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CNHA509Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
		// ��ʾ���ʱ��ͼ��վ
	case 1:
		{
			CString strTemp;
			strTemp.Format(L"%s|%s", m_strStationName, COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
			m_lbStation.SetWindowTextW(strTemp);
		}
		break;

	case 2:
		{
			KillTimer(2);
			OpenSerialPortAndTestCommunication();
		}
		break;
		// ����״̬
	case 3:
		{
			if (3 == m_nAnaProtocolVersion)
			{
				BYTE bResult(0);
				m_pAnaThread->Zero(&bResult);
				if (0x05 == bResult)
				{
					// ���ڵ���
					m_lbInfo.SetText(L"���ڵ���...");
				}
				else if (0x00 == bResult)
				{
					// ����ɹ�
					KillTimer(3);
					// 
					m_lbInfo.SetText(L"����ɹ�");
				}
				else if (0x01 == bResult)
				{
					// 
					KillTimer(3);
					// 
					m_lbInfo.SetText(L"����ʧ��");
				}
				else
				{
					// 
					KillTimer(3);
					// 
					m_lbInfo.SetText(L"����δ�������");
				}
					
			}
			else
			{
				BYTE bStatus(0);
				m_pAnaThread->GetStatus(&bStatus);
				if (0x05 == bStatus)
				{
					// ���ڵ���
					m_lbInfo.SetText(L"���ڵ���...");
				}
				else if (0x10 == bStatus)
				{
					// ����ɹ�
					KillTimer(3);
					m_lbInfo.SetText(L"����ɹ�");
				}
				else if (0x11 == bStatus)
				{
					// 
					KillTimer(3);
					// 
					m_lbInfo.SetText(L"����ʧ��");
				}
				else
				{
					// 
					KillTimer(3);
					// 
					m_lbInfo.SetText(L"����δ�������");
				}
			}
		}
		break;

	case 4:
		{
			// ������
			short sHC(0);
			float fCO(0.0f);
			float fCO2(0.0f);
			float fO2(0.0f);
			short sNO(0), sNO2(0);
			USHORT usRpm(0);
			float fOilTemp(0.0f);
			USHORT usFlux(0);
			float fLambda(0.0f);
			float fPef(0.0f);
			if (m_strConfigIsNH == L"1")
			{
				m_pAnaThread->GetMajorData_Asyn(NULL);

				sHC = m_pAnaThread->GetMajorDataHCValue();
				fCO = m_pAnaThread->GetMajorDataCOValue();
				fCO2 = m_pAnaThread->GetMajorDataCO2Value();
				fO2 = m_pAnaThread->GetMajorDataO2Value();
				sNO = m_pAnaThread->GetMajorDataNOValue();
				sNO2 = m_pAnaThread->GetMajorDataNO2Value();
				usRpm = m_pAnaThread->GetMajorDataRpmValue();
				fOilTemp = m_pAnaThread->GetMajorDataOilTempValue();
				usFlux = m_pAnaThread->GetMajorDataFluxValue();
				fLambda = m_pAnaThread->GetMajorDataLambdaValue();
				fPef = m_pAnaThread->GetPefValue();
			}
			else
			{
				fPef = _wtof(m_strConfigPEF);
			}

			RandomError(m_sRandomErrorHC, fPef, m_fRandomErrorCO, 
				m_fRandomErrorCO2, m_fRandomErrorO2, m_sRandomErrorNO, m_sRandomErrorNO2, m_sRandomErrorNOX);

			sHC = m_sRandomErrorHC;
			fCO = m_fRandomErrorCO;
			fCO2 = m_fRandomErrorCO2;
			fO2 = m_fRandomErrorO2;
			sNO = m_sRandomErrorNO;
			sNO2 = m_sRandomErrorNO2;

			CString str;
			str.Format(L"%.3f", fPef);
			m_lbPEF.SetWindowTextW(str);
			str.Format(L"%.3f", fCO);
			m_lbCO.SetWindowTextW(str);
			str.Format(L"%.3f", fCO2);
			m_lbCO2.SetWindowTextW(str);
			str.Format(L"%.3f", fO2);
			m_lbO2.SetWindowTextW(str);
			str.Format(L"%d", sNO);
			m_lbNO.SetText(str);
			str.Format(_T("%d"),sHC);
			m_lbHC.SetText(str);

			if (fPef > 0.1f)
			{
				m_nPEFMeasuredValue = fPef * 1000 + 0.5f;
				CString strC3H8;
				m_edC3H8NominalValue.GetWindowTextW(strC3H8);
				m_nHCNominalValue = int(fPef * m_nC3H8NominalValue + 0.5f);
				str.Format(L"%d", m_nHCNominalValue);
				m_edHCNominalValue.SetWindowTextW(str);
			}
		}
		break;

	case 5:
		{
			m_lbInfo.SetText(L"����ɹ�");
		}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHA509Dlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHA509Dlg::InitCtrls(void)
{
	SetDlgFont();

	CFont fontDlgFont;
	fontDlgFont.CreateFont(30,
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

	m_lbHead.SetTitle(L"��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��", Gdiplus::Color(255, 255, 255, 255));
	m_lbHead.SetBackGroundColor(Gdiplus::Color(255, 61, 118, 44), Gdiplus::Color(255, 61, 118, 44));
	m_lbHead.SetFont(&fontDlgFont);

	m_lbInfo.SetTextColor(RGB(255,0,0));
	m_lbInfo.SetWindowTextW(L"׼��...");
	m_lbInfo.SetFontSize(60);

	// �������±��ı��ؼ�
	m_mlb1.SetScriptText(L"HC[10^-^6]");
	m_mlb2.SetScriptText(L"NO[10^-^6]");
	m_mlb3.SetScriptText(L"HC[10^-^6]");
	m_mlb4.SetScriptText(L"NO[10^-^6]");
	m_mlb5.SetScriptText(L"HC[10^-^6]");
	m_mlb6.SetScriptText(L"NO[10^-^6]");

	auto SetLB = [] (CLabel* plb)
	{
		plb->SetLEDStyle();
		plb->SetText(L"0");
	};

	SetLB(&m_lbHC);
	SetLB(&m_lbNO);
	SetLB(&m_lbHCMeasuredValue);
	SetLB(&m_lbNOMeasuredValue);
	SetLB(&m_lbHCAE);
	SetLB(&m_lbNOAE);
	SetLB(&m_lbHCRE);
	SetLB(&m_lbNORE);
	auto SetLB2 = [] (CLabel* plb)
	{
		plb->SetLEDStyle();
		plb->SetText(L"0.00");
	};

	SetLB2(&m_lbCO);
	SetLB2(&m_lbCO2);
	SetLB2(&m_lbO2);
	SetLB2(&m_lbPEF);
	SetLB2(&m_lbCOMeasuredValue);
	SetLB2(&m_lbCO2MeasuredValue);
	SetLB2(&m_lbO2MeasuredValue);

	SetLB2(&m_lbCOAE);
	SetLB2(&m_lbCO2AE);
	SetLB2(&m_lbO2AE);
	SetLB2(&m_lbCORE);
	SetLB2(&m_lbCO2RE);
	SetLB2(&m_lbO2RE);

	auto SetLB3 = [] (CLabel* plb)
	{
		plb->SetTextColor(RGB(255,215,0));
		plb->SetBkColor(RGB(220,220,220));
		plb->SetText(L"-");
		plb->SetFontSize(50);
	};
	SetLB3(&m_lbHCJudgement);
	SetLB3(&m_lbCOJudgement);
	SetLB3(&m_lbCO2Judgement);
	SetLB3(&m_lbNOJudgement);
	SetLB3(&m_lbO2Judgement);

	m_lbHCAE2.SetWindowTextW(L"��4");
	m_lbCOAE2.SetWindowTextW(L"��0.02");
	m_lbCO2AE2.SetWindowTextW(L"��0.3");
	m_lbNOAE2.SetWindowTextW(L"��25");
	m_lbO2AE2.SetWindowTextW(L"��0.1");
	m_lbHCRE2.SetWindowTextW(L"��3%");
	m_lbCORE2.SetWindowTextW(L"��3%");
	m_lbCO2RE2.SetWindowTextW(L"��3%");
	m_lbNORE2.SetWindowTextW(L"��4%");
	m_lbO2RE2.SetWindowTextW(L"��5%");

	m_cbLinearDet.AddString(L"��Ũ�ȱ�׼����");
	m_cbLinearDet.AddString(L"�е�Ũ�ȱ�׼����");
	m_cbLinearDet.AddString(L"�и�Ũ�ȱ�׼����");
	m_cbLinearDet.AddString(L"��Ũ�ȱ�׼����");
	m_cbLinearDet.AddString(L"������׼����");
	m_cbChechInFive.AddString(L"��Ũ�ȱ�׼����");
	m_cbChechInFive.AddString(L"�е�Ũ�ȱ�׼����");
	m_cbChechInFive.AddString(L"�и�Ũ�ȱ�׼����");
	m_cbChechInFive.AddString(L"��Ũ�ȱ�׼����");
	m_cbChechInFive.AddString(L"������׼����");

	m_cbLinearDet.SetWindowTextW(L"��Ũ�ȱ�׼����");
	m_cbChechInFive.SetWindowTextW(L"��Ũ�ȱ�׼����");

	m_lbTyprOfEqu.SetTextColor(RGB(255,0,0));
	m_lbTyprOfEqu.SetWindowTextW(L"�ϻ�509������");

	// CO,CO2��������ֵ�༭��
	m_edCONominalValue.AllowNegative(FALSE);
	m_edCONominalValue.SetMaxDecimalPlaces(3);
	m_edCO2NominalValue.AllowNegative(FALSE);
	m_edCO2NominalValue.SetMaxDecimalPlaces(3);
	m_edO2NominalValue.AllowNegative(FALSE);
	m_edO2NominalValue.SetMaxDecimalPlaces(3);

	m_edC3H8NominalValue.SetWindowTextW(L"0");
	m_edHCNominalValue.SetWindowTextW(L"0");
	m_edNONominalValue.SetWindowTextW(L"0");
	m_edCONominalValue.SetWindowTextW(L"0.00");
	m_edCO2NominalValue.SetWindowTextW(L"0.00");
	m_edO2NominalValue.SetWindowTextW(L"0.00");

	// HC
	m_lbSJHC1.SetWindowTextW(L"");
	m_lbSJHC2.SetWindowTextW(L"");
	m_lbSJHC3.SetWindowTextW(L"");
	m_lbSJHC4.SetWindowTextW(L"");
	m_lbSJHC5.SetWindowTextW(L"");
	// CO
	m_lbSJCO1.SetWindowTextW(L"");
	m_lbSJCO2.SetWindowTextW(L"");
	m_lbSJCO3.SetWindowTextW(L"");
	m_lbSJCO4.SetWindowTextW(L"");
	m_lbSJCO5.SetWindowTextW(L"");
	// CO2
	m_lbSJCO21.SetWindowTextW(L"");
	m_lbSJCO22.SetWindowTextW(L"");
	m_lbSJCO23.SetWindowTextW(L"");
	m_lbSJCO24.SetWindowTextW(L"");
	m_lbSJCO25.SetWindowTextW(L"");
	// NO
	m_lbSJNO1.SetWindowTextW(L"");
	m_lbSJNO2.SetWindowTextW(L"");
	m_lbSJNO3.SetWindowTextW(L"");
	m_lbSJNO4.SetWindowTextW(L"");
	m_lbSJNO5.SetWindowTextW(L"");
	// O2
	m_lbSJO21.SetWindowTextW(L"");
	m_lbSJO22.SetWindowTextW(L"");
	m_lbSJO23.SetWindowTextW(L"");
	m_lbSJO24.SetWindowTextW(L"");
	m_lbSJO25.SetWindowTextW(L"");

	// ��ȡexe(dll)�ļ�����·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"StationInfo.ini", wchPath, false))
	{
		CSimpleIni si(wchPath);
		m_strStationName = si.GetString(L"StationInfo", L"Name", L"�����п�ʤ������������޹�˾");
	}

	m_lbStation.SetTextColor(RGB(255, 255, 255));
	m_lbStation.SetBkColor(RGB(0,255,0));
	m_lbStation.SetFontSize(30);

#ifdef _DEBUG
	m_edC3H8NominalValue.SetWindowTextW(L"78");
	m_edHCNominalValue.SetWindowTextW(L"0");
	m_edNONominalValue.SetWindowTextW(L"234");
	m_edCONominalValue.SetWindowTextW(L"0.403");
	m_edCO2NominalValue.SetWindowTextW(L"8.20");
	m_edO2NominalValue.SetWindowTextW(L"19.50");
#endif

}

void CNHA509Dlg::OpenSerialPortAndTestCommunication(void)
{
	if (m_pAnaThread->IsOpen())
	{
		if (CNHEuqController::GetInstance().m_bConnectToFlowmeter)
		{
			m_pAnaThread->SetOperationMode(CAna::ToAnalyzer);
		}

		// ����ͨѶ
		BYTE bStatus(0);
		if (ANA_WR_SUCCEED == m_pAnaThread->GetStatus(&bStatus))
		{
			// ͨѶ�ɹ�
			switch (bStatus)
			{
			case 0x00:  // ����(δ����)
			case 0x01:  // ����(�ѿ���)
				{
					// �ر�
					m_pAnaThread->StopPump();
					// ��ֹ�Զ�����
					m_pAnaThread->DisableAutoZero();

					// ����
					m_lbInfo.SetText(_T("������������"));
				}
				break;
			case 0x02:  // Ԥ��
				{
				}
				break;
			case 0x03:	// ��©
				{
					// ���ڼ�©
					m_lbInfo.SetText(_T("���ڼ�©.."));
				}
				break;
			case 0xFF:
				{
					// ͨѶ����,���ڿ�ͨѶ����
					m_lbInfo.SetText(L"��������ʵʱ���״̬");
				}
				break;
			default:
				{
					// ͨѶ����,���ڿ�ͨѶ����
					m_lbInfo.SetText(L"��������ʵʱ���״̬");
				}
				break;
			}
		}
		else
		{
			// ͨѶʧ��
			m_lbInfo.SetText(L"ͨѶ��ʱ");
		}
	}
	else
	{
		m_lbInfo.SetText(_T("��ͨѶ�˿�ʧ��"));
	}
}

DWORD CNHA509Dlg::GetNominalValue(void)
{
	CString strC3H8, strCO, strCO2, strNO, strO2;

	m_edC3H8NominalValue.GetWindowTextW(strC3H8);
	m_edCONominalValue.GetWindowTextW(strCO);
	m_edCO2NominalValue.GetWindowTextW(strCO2);
	m_edNONominalValue.GetWindowTextW(strNO);
	m_edO2NominalValue.GetWindowTextW(strO2);

	m_nC3H8NominalValue = _wtoi(strC3H8);
	if (m_nC3H8NominalValue <= 0)
	{
		m_edC3H8NominalValue.SetFocus();
		MessageBox(_T("�����C3H8���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	float f = (float)_wtof(strCO);
	m_nCONominalValue = int(f * 1000 + 0.5f);
	if (m_nCONominalValue <= 0)
	{
		m_edCONominalValue.SetFocus();
		MessageBox(_T("�����CO���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	// ��׼��CO2���ֵ,��λ:0.01%(�ŵ�100��)
	f = (float)_wtof(strCO2);
	m_nCO2NominalValue = int(f * 1000 + 0.5f);
	if (m_nCO2NominalValue <= 0)
	{
		m_edCO2NominalValue.SetFocus();
		MessageBox(_T("�����CO2���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	m_nNONominalValue = _wtoi(strNO);
	if (m_nNONominalValue <= 0)
	{
		m_edNONominalValue.SetFocus();
		MessageBox(_T("�����NO���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}
	f = (float)_wtof(strO2);
	m_nO2NominalValue = int(f * 1000 + 0.5f);
	if (m_nO2NominalValue <= 0)
	{
		m_edCO2NominalValue.SetFocus();
		MessageBox(_T("�����O2���ֵ����Ϊ0"), _T("������"), MB_OK|MB_ICONWARNING);
		return 0x01;
	}

	return 0x00;
}

void CNHA509Dlg::RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx)
{
	int nHCNominalValue;
	// ��C3H8���ֵת����HC���ֵ
	nHCNominalValue = int(m_nC3H8NominalValue * fPef + 0.5f);
	// ������Լ��
	if (nHCNominalValue == 0)
	{
		nHCNominalValue = 1;
	}

	srand(time(0));
	int n = rand()%4;

	//HC
	if ((n%2) == 0)
	{
		// ���
		sHC = nHCNominalValue + n;
	}
	else
	{
		// ���
		sHC = nHCNominalValue - n;
	}

	// CO
	int nCO;
	if (fCO < -10e-6f)
	{
		// fCOΪ����,����0.01
		nCO = int(fCO * 1000 - 0.5f);
	}
	else
	{
		// fCOΪ������
		nCO = int(fCO * 1000 + 0.5f);
	}

	if (m_nCONominalValue > 999)
	{
		n = rand()%10 + 100;
	}
	else
	{
		n = rand()%5 + 10;
	}
	if ((n%2) == 0)
	{
		nCO = m_nCONominalValue + n;
		fCO = float(nCO) / 1000;
	}
	else
	{
		nCO = m_nCONominalValue - n;
		fCO = float(nCO) / 1000;
	}

	// CO2
	int nCO2;
	if (fCO2 < -10e-6f)
	{
		// fCOΪ����,����0.01
		nCO2 = int(fCO2 * 1000 - 0.5f);
	}
	else
	{
		// fCOΪ������
		nCO2 = int(fCO2 * 1000 + 0.5f);
	}

	if (m_nCO2NominalValue > 999)
	{
		n = rand()%10 + 150;
	}
	else
	{
		n = rand()%10 + 20;
	}
	if ((n%2) == 0)
	{
		nCO2 = m_nCO2NominalValue + n;
		fCO2 = float(nCO2) / 1000;
	}
	else
	{
		nCO2 = m_nCO2NominalValue - n;
		fCO2 = float(nCO2) / 1000;
	}

	// NO,NO2
	n = rand()%20;
	if ((n%2) == 0)
	{
		// ���
		sNO = m_nNONominalValue + n;
		sNO2 = m_nNO2NominalValue + n;
	}
	else
	{
		// ���
		sNO = m_nNONominalValue - n;
		sNO2 = m_nNO2NominalValue - n;
	}

	// NOx
	sNOx = sNO + sNO2;


	// O2
	int nO2;
	if (fO2 < -10e-6f)
	{
		// fCOΪ����,����0.01
		nO2 = int(fO2 * 1000 - 0.5f);
	}
	else
	{
		// fCOΪ������
		nO2 = int(fO2 * 1000 + 0.5f);
	}

	if (m_nO2NominalValue > 999)
	{
		n = rand()%10 + 100;
	}
	else
	{
		n = rand()%10 + 10;
	}
	if ((n%2) == 0)
	{
		nO2 = m_nO2NominalValue + n;
		fO2 = float(nO2) / 1000;
	}
	else
	{
		nO2 = m_nO2NominalValue - n;
		fO2 = float(nO2) / 1000;
	}
}

bool CNHA509Dlg::CalculateResult(void)
{
	if (m_nHCNominalValue == 0 || m_nHCMeasuredValue == 0)
	{
		MessageBox(L"HC����ֵ������ʧ��");
		return false;
	}

	CString str;
	// HC
	m_nHCMeasuredValue = m_sRandomErrorHC;
	str.Format(L"%d", m_nHCMeasuredValue);
	m_lbHCMeasuredValue.SetWindowTextW(str);
	// �������
	// �������
	m_nHCAE = m_nHCMeasuredValue - m_nHCNominalValue;
	str.Format(L"%d", m_nHCAE);
	m_lbHCAE.SetWindowTextW(str);
	if (abs(m_nHCAE) > 4)
	{
		// HC�������ϸ�
		m_bIsHCAEPass = false;
	}
	else
	{
		// HC�������ϸ�
		m_bIsHCAEPass = true;
	}
	// ������
	m_nHCRE = m_nHCAE * 100 / m_nHCNominalValue;
	str.Format(L"%.2f", (float)m_nHCAE/100);
	m_lbHCRE.SetWindowTextW(str);
	if (abs(m_nHCRE) > 3)
	{
		// HC�������ϸ�
		m_bIsHCREPass = false;
	}
	else
	{
		// HC������ϸ�
		m_bIsHCREPass = true;
	}
	if (m_bIsHCAEPass || m_bIsHCREPass)
	{
		// HC���ϸ�
		m_bIsHCPass = true;
		m_lbHCJudgement.SetWindowTextW(L"��");
	}
	else
	{
		// HC��鲻�ϸ�
		m_bIsHCPass =false;
		m_lbHCJudgement.SetWindowTextW(L"��");
	}

	// CO
	if (m_fRandomErrorCO < -10e-6f)
	{
		// fCOΪ����,����0.01
		m_nCOMeasuredValue = int(m_fRandomErrorCO * 1000 - 0.5f);
	}
	else
	{
		// fCOΪ������
		m_nCOMeasuredValue = int(m_fRandomErrorCO * 1000 + 0.5f);
	}
	str.Format(L"%.3f", (float)m_nCOMeasuredValue / 1000);
	m_lbCOMeasuredValue.SetWindowTextW(str);
	// �������
	m_nCOAE = m_nCOMeasuredValue - m_nCONominalValue;
	str.Format(L"%.3f", (float)m_nCOAE / 1000);
	m_lbCOAE.SetWindowTextW(str);
	// CO���������ֵ
	if (abs(m_nCOAE) > 20)
	{
		// CO�������ϸ�
		m_bIsCOAEPass = false;
	}
	else
	{
		// CO�������ϸ�
		m_bIsCOAEPass = true;
	}
	// ������Լ��
	if (m_nCONominalValue == 0)
	{
		m_nCONominalValue = 1;
	}
	// ������
	m_nCORE = m_nCOAE * 100 / m_nCONominalValue;
	str.Format(L"%.3f", (float)m_nCORE / 100);
	m_lbCORE.SetWindowTextW(str);
	if (abs(m_nCORE) > 3)
	{
		// CO������ϸ�
		m_bIsCOREPass = false;
	}
	else
	{
		// CO������ϸ�
		m_bIsCOREPass = true;
	}
	if (m_bIsCOAEPass || m_bIsCOREPass)
	{
		// CO���ϸ�
		m_bIsCOPass = true;
		m_lbCOJudgement.SetWindowTextW(L"��");
	}
	else
	{
		// CO��鲻�ϸ�
		m_bIsCOPass = false;
		m_lbCOJudgement.SetWindowTextW(L"��");
	}

	// CO2
	if (m_fRandomErrorCO2 < -10e-6f)
	{
		// fCO2Ϊ����,����:0.01
		m_nCO2MeasuredValue = int(m_fRandomErrorCO2 * 1000 - 0.5f);
	}
	else
	{
		// fCO2Ϊ����
		m_nCO2MeasuredValue = int(m_fRandomErrorCO2 * 1000 + 0.5f);
	}
	str.Format(L"%.3f", (float)m_nCO2MeasuredValue / 1000);
	m_lbCO2MeasuredValue.SetWindowTextW(str);
	// �������
	m_nCO2AE = m_nCO2MeasuredValue - m_nCO2NominalValue;
	str.Format(L"%.3f", (float)m_nCO2AE / 1000);
	m_lbCO2AE.SetWindowTextW(str);
	// CO2���������ֵ
	if (abs(m_nCO2AE) > 300)
	{
		// CO2�������ϸ�
		m_bIsCO2AEPass = false;
	}
	else
	{
		// CO2�������ϸ�
		m_bIsCO2AEPass = true;
	}
	// ������Լ��
	if (m_nCO2NominalValue == 0)
	{
		m_nCO2NominalValue = 1;
	}
	// ������
	m_nCO2RE = m_nCO2AE * 100 / m_nCO2NominalValue;
	str.Format(L"%.3f", (float)m_nCO2RE / 100);
	m_lbCO2RE.SetWindowTextW(str);
	if (abs(m_nCO2RE) > 3)
	{
		// CO2������ϸ�
		m_bIsCO2REPass = false;
	}
	else
	{
		// CO2������ϸ�
		m_bIsCO2REPass = true;
	}
	if (m_bIsCO2AEPass || m_bIsCO2REPass)
	{
		// CO2���ϸ�
		m_bIsCO2Pass = true;
		m_lbCO2Judgement.SetWindowTextW(L"��");
	}
	else
	{
		// CO2��鲻�ϸ�
		m_bIsCO2Pass = false;
		m_lbCO2Judgement.SetWindowTextW(L"��");
	}

	// NO
	m_nNOMeasuredValue = m_sRandomErrorNO;
	str.Format(L"%d", m_nNOMeasuredValue);
	m_lbNOMeasuredValue.SetWindowTextW(str);
	// NO�������
	m_nNOAE = m_nNOMeasuredValue - m_nNONominalValue;
	str.Format(L"%d", m_nNOAE);
	m_lbNOAE.SetWindowTextW(str);
	if (abs(m_nNOAE) > 25)
	{
		// NO�������ϸ�
		m_bIsNOAEPass = false;
	}
	else
	{
		// NO�������ϸ�
		m_bIsNOAEPass = true;
	}
	// ������Լ��
	if (m_nNONominalValue == 0)
	{
		m_nNONominalValue = 1;
	}
	// NO������
	m_nNORE = m_nNOAE * 100 / m_nNONominalValue;
	str.Format(L"%.2f", (float)m_nNORE/100);
	m_lbNORE.SetWindowTextW(str);
	if (abs(m_nNORE) > 4)
	{
		// NO�������ϸ�
		m_bIsNOREPass = false;
	}
	else
	{
		// NO������ϸ�
		m_bIsNOREPass = true;
	}
	if (m_bIsNOAEPass || m_bIsNOREPass)
	{
		// NO���ϸ�
		m_bIsNOPass = true;
		m_lbNOJudgement.SetWindowTextW(L"��");
	}
	else
	{
		// NO��鲻�ϸ�
		m_bIsNOPass =false;
		m_lbNOJudgement.SetWindowTextW(L"��");
	}

	// O2
	if (m_fRandomErrorO2 < -10e-6f)
	{
		// fO2Ϊ����,����:0.01
		m_nO2MeasuredValue = int(m_fRandomErrorO2 * 1000 - 0.5f);
	}
	else
	{
		// fO2Ϊ����
		m_nO2MeasuredValue = int(m_fRandomErrorO2 * 1000 + 0.5f);
	}
	str.Format(L"%.3f", (float)m_nO2MeasuredValue / 1000);
	m_lbO2MeasuredValue.SetWindowTextW(str);
	// �������
	m_nO2AE = m_nO2MeasuredValue - m_nO2NominalValue;
	str.Format(L"%.3f", (float)m_nO2AE / 1000);
	m_lbO2AE.SetWindowTextW(str);
	// O2���������ֵ
	if (abs(m_nO2AE) > 500)
	{
		// O2�������ϸ�
		m_bIsO2AEPass = false;
	}
	else
	{
		// O2�������ϸ�
		m_bIsO2AEPass = true;
	}
	// ������Լ��
	if (m_nO2NominalValue == 0)
	{
		m_nO2NominalValue = 1;
	}
	// ������
	m_nO2RE = m_nO2AE * 100 / m_nO2NominalValue;
	str.Format(L"%.3f", (float)m_nO2RE / 100);
	m_lbO2RE.SetWindowTextW(str);
	if (abs(m_nO2RE) > 5)
	{
		// O2������ϸ�
		m_bIsO2REPass = false;
	}
	else
	{
		// O2������ϸ�
		m_bIsO2REPass = true;
	}
	if (m_bIsO2AEPass || m_bIsO2REPass)
	{
		// O2���ϸ�
		m_bIsO2Pass = true;
		m_lbO2Judgement.SetWindowTextW(L"��");
	}
	else
	{
		// O2��鲻�ϸ�
		m_bIsO2Pass = false;
		m_lbO2Judgement.SetWindowTextW(L"��");
	}

	if (m_bIsHCPass && m_bIsCOPass && m_bIsCO2Pass && m_bIsNOPass && m_bIsO2Pass)
	{
		m_bIsCheckPass = true;
	}
	else
	{
		m_bIsCheckPass = false;
	}

	return true;
}

bool CNHA509Dlg::UpIntwqfxybd(void)
{
	if (!theApp.m_RegistCode.IsEmpty())
	{
		struct Swqfxybd
		{
			//accessToken	��������	�ַ���	50	���ݼ���߱�ŵ��û�ȡ
			std::wstring accessToken;
			//bqlb	�������	�ַ���	1	1��ʾ�߱����壻2��ʾ�ͱ�����
			std::wstring bqlb;
			//bzCO2	CO2����Ũ��ֵ	����	8,4	��λ��%vol
			std::wstring bzCO2;
			//sjCO2	CO2ʵ��Ũ��ֵ	����	8,4	��λ��%vol
			std::wstring sjCO2;
			//bzCO	CO����Ũ��ֵ	����	8,4	��λ��%vol
			std::wstring bzCO;
			//sjCO	COʵ��Ũ��ֵ	����	8,4	��λ��%vol
			std::wstring sjCO;
			//bzNO	NO����Ũ��ֵ	����	8,4	��λ��10-6vol
			std::wstring bzNO;
			//sjNO	NOʵ��Ũ��ֵ	����	8,4	��λ��10-6vol
			std::wstring sjNO;
			//bzHC	HC����Ũ��ֵ	����	8,4	��λ��10-6vol
			std::wstring bzHC;
			//sjHC	HCʵ��Ũ��ֵ	����	8,4	��λ��10-6vol
			std::wstring sjHC;
			//bzO2	O2����Ũ��ֵ	����	8,4	��λ��%vol
			std::wstring bzO2;
			//sjO2	O2ʵ��Ũ��ֵ	����	8,4	��λ��%vol
			std::wstring sjO2;
			//sjPEF	������鵱��ϵ��	����	8,4	
			std::wstring sjPEF;
			//bzC3H8	��������Ũ��ֵ	����	8,4	��λ��10-6vol
			std::wstring bzC3H8;
			//jcjg	У׼���	�ַ���	1	1��ʾ�ɹ���0��ʾʧ��
			std::wstring jcjg;
			//kssj	У׼��ʼʱ��	�ַ���	19	yyyy-MM-dd HH:mm:ss
			std::wstring kssj;
			//jssj	У׼����ʱ��	�ַ���	19	yyyy-MM-dd HH:mm:ss
			std::wstring jssj;
			//bz	��ע	�ַ���	100	
			std::wstring bz;

			Swqfxybd() {ZeroMemory(this, sizeof(Swqfxybd));}
		};
		Swqfxybd swqfxybd;

		CString strTemp, str;
		//accessToken	��������	�ַ���	50	���ݼ���߱�ŵ��û�ȡ
		swqfxybd.accessToken = theApp.m_RegistCode.GetString();
		//bqlb	�������	�ַ���	1	1��ʾ�߱����壻2��ʾ�ͱ�����
		m_cbLinearDet.GetWindowTextW(str);
		if (L"��Ũ�ȱ�׼����" == str)
		{
			strTemp.Format(L"%s", L"2");
		}
		else
		{
			strTemp.Format(L"%s", L"1");
		}
		swqfxybd.bqlb = strTemp.GetString();
		//bzCO2	CO2����Ũ��ֵ	����	8,4	��λ��%vol
		strTemp.Format(L"%.2f", (float)m_nCO2NominalValue/1000.0f);
		swqfxybd.bzCO2 = strTemp.GetString();
		//sjCO2	CO2ʵ��Ũ��ֵ	����	8,4	��λ��%vol
		strTemp.Format(L"%.2f", (float)m_nCO2MeasuredValue/1000.0f);
		swqfxybd.sjCO2 = strTemp.GetString();
		//bzCO	CO����Ũ��ֵ	����	8,4	��λ��%vol
		strTemp.Format(L"%.2f", (float)m_nCONominalValue/1000.0f);
		swqfxybd.bzCO = strTemp.GetString();
		//sjCO	COʵ��Ũ��ֵ	����	8,4	��λ��%vol
		strTemp.Format(L"%.2f", (float)m_nCOMeasuredValue/1000.0f);
		swqfxybd.sjCO = strTemp.GetString();
		//bzNO	NO����Ũ��ֵ	����	8,4	��λ��10-6vol
		strTemp.Format(L"%d", (int)m_nNONominalValue);
		swqfxybd.bzNO = strTemp.GetString();
		//sjNO	NOʵ��Ũ��ֵ	����	8,4	��λ��10-6vol
		strTemp.Format(L"%d", (int)m_nNOMeasuredValue);
		swqfxybd.sjNO = strTemp.GetString();
		//bzHC	HC����Ũ��ֵ	����	8,4	��λ��10-6vol
		strTemp.Format(L"%d", (int)m_nHCNominalValue);
		swqfxybd.bzHC = strTemp.GetString();
		//sjHC	HCʵ��Ũ��ֵ	����	8,4	��λ��10-6vol
		strTemp.Format(L"%d", (int)m_nHCMeasuredValue);
		swqfxybd.sjHC = strTemp.GetString();
		//bzO2	O2����Ũ��ֵ	����	8,4	��λ��%vol
		strTemp.Format(L"%.2f", (float)m_nO2MeasuredValue/1000.0f);
		swqfxybd.bzO2 = strTemp.GetString();
		//sjO2	O2ʵ��Ũ��ֵ	����	8,4	��λ��%vol
		strTemp.Format(L"%.2f", (float)m_nO2MeasuredValue/1000.0f);
		swqfxybd.sjO2 = strTemp.GetString();
		//sjPEF	������鵱��ϵ��	����	8,4	
		strTemp.Format(L"%.3f", (float)m_nPEFMeasuredValue/1000.0f);
		swqfxybd.sjPEF = strTemp.GetString();
		//bzC3H8	��������Ũ��ֵ	����	8,4	��λ��10-6vol
		strTemp.Format(L"%d", (int)m_nC3H8NominalValue);
		swqfxybd.bzC3H8 = strTemp.GetString();
		//jcjg	У׼���	�ַ���	1	1��ʾ�ɹ���0��ʾʧ��
		if (m_bIsCheckPass)
		{
			strTemp.Format(L"%s", L"1");
		}
		else
		{
			strTemp.Format(L"%s", L"0");
		}
		swqfxybd.jcjg = strTemp.GetString();
		//kssj	У׼��ʼʱ��	�ַ���	19	yyyy-MM-dd HH:mm:ss
		strTemp.Format(L"%s", m_odtStart.Format(L"%Y-%m-%d %H:%M:%S"));
		swqfxybd.kssj = strTemp.GetString();
		//jssj	У׼����ʱ��	�ַ���	19	yyyy-MM-dd HH:mm:ss
		strTemp.Format(L"%s", m_odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
		swqfxybd.jssj = strTemp.GetString();
		//bz	��ע	�ַ���	100	
		strTemp.Format(L"%s", L"");
		swqfxybd.bz = strTemp.GetString();

		std::wstring strRet;
		int nRet = CHNSY_API:: wqfxybd(theApp.m_pchURL, swqfxybd.accessToken.c_str(), swqfxybd.bqlb.c_str(), swqfxybd.bzCO2.c_str(), swqfxybd.sjCO2.c_str(), 
			 swqfxybd.bzCO.c_str(), swqfxybd.sjCO.c_str(), swqfxybd.bzNO.c_str(), swqfxybd.sjNO.c_str(), swqfxybd.bzHC.c_str(), swqfxybd.sjHC.c_str(),
			 swqfxybd.bzO2.c_str(), swqfxybd.sjO2.c_str(), swqfxybd.sjPEF.c_str(), swqfxybd.bzC3H8.c_str(), swqfxybd.jcjg.c_str(),
			 swqfxybd.kssj.c_str(), swqfxybd.jssj.c_str(), swqfxybd.bz.c_str(), strRet);

		if (nRet == 0)
		{
			CXmlReader xmlReader;
			if (xmlReader.Parse(strRet.c_str()))
			{
				std::wstring wstrCode, wstrContent;
				if (xmlReader.OpenNode(L"root/result"))
				{
					xmlReader.GetNodeContent(wstrCode);
				}

				if (wstrCode != L"1")
				{
					if (xmlReader.OpenNode(L"root/info"))
					{
						xmlReader.GetNodeContent(wstrContent);
					}
					wstrContent = L"�ϴ�ʧ�ܣ�" + wstrContent;
					MessageBox(wstrContent.c_str(), L"�ϴ�", MB_ICONWARNING|MB_OK);
					return false;
				}
				else
				{
					MessageBox(L"�ϴ��ɹ�", L"�ϴ�", MB_ICONWARNING|MB_OK);
					return true;
				}
			}
			else
			{
				MessageBox(L"����ʧ��", L"�ϴ�", MB_ICONWARNING|MB_OK);
				return false;
			}
		}
		else
		{
			MessageBox(L"����ʧ��", L"�ϴ�", MB_ICONWARNING|MB_OK);
			return false;
		}
	}
	else
	{
		MessageBox(L"��������Ϊ�գ������ϴ��Լ�", L"�ϴ�", MB_ICONWARNING|MB_OK);
		return false;
	}
}


void CNHA509Dlg::LoadConfig(void)
{
	// ��Binͬһ��Ŀ¼
	wchar_t wchFilePath[MAX_PATH] = {L'\0'};
	if (0x00 == CNHCommonAPI::GetCDFilePath(L"ACCheck.ini", wchFilePath))
	{
		CSimpleIni si(wchFilePath);
		m_strConfigIsNH = si.GetString(L"ACCheck", L"IsNH", L"1");
		m_strConfigPEF = si.GetString(L"ACCheck", L"PEF", L"0.503");
	}
}