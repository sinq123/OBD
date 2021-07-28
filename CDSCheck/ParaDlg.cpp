// ParaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ParaDlg.h"

// CParaDlg �Ի���

IMPLEMENT_DYNAMIC(CParaDlg, CDialogZoom)

CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParaDlg::IDD, pParent)
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

CParaDlg::~CParaDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CParaDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ENV_PARAMS_CAL, &CParaDlg::OnBnClickedButtonEnvParamsCal)
END_MESSAGE_MAP()


// CParaDlg ��Ϣ�������
BOOL CParaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CParaDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"����������");
}

void CParaDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}


void CParaDlg::OnBnClickedButtonEnvParamsCal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	

	ShowLoadParameterEnvParamsCalibrationDlg(UpEnvParamsAndTachometerProcessCtrlCallBack);

}


void CParaDlg::UpEnv(void)
{
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		// ��ʱ������������
	}
	CSimpleIni si(wchPath);

	CString strTemp;

	SEnvPara sEnvPara;
	sEnvPara.JYY = theApp.m_sUserInfo.wchName;
	sEnvPara.JYYSFZH = L"123456200001010101";
	strTemp = si.GetString(L"EnvParams", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sEnvPara.odtbegin.ParseDateTime(strTemp))
	{
		sEnvPara.odtbegin = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"EnvParams", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sEnvPara.odtEnd.ParseDateTime(strTemp))
	{
		sEnvPara.odtEnd = COleDateTime::GetCurrentTime();
	}
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"ETMea", L"")));
	sEnvPara.scwd = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"ETStd", L"")));
	sEnvPara.hjwd = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"RHMea", L"")));
	sEnvPara.scsd = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"RHStd", L"")));
	sEnvPara.hjsd = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"APMea", L"")));
	sEnvPara.scdqy = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"APStd", L"")));
	sEnvPara.hjdqy = strTemp.GetString();


	SHBMsg sHBMsg;
	CZYHttp_PAI::GetInstance().SetParaCheck(sEnvPara, sHBMsg);
	
	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"�ϴ��ɹ�");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"�ϴ�ʧ�ܣ�%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}
}

void CParaDlg::UpTac(void)
{
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfTachometer.ini", wchPath, false))
	{
		// ��ʱ������������
	}
	CSimpleIni si(wchPath);

	CString strTemp;

	STachometer sTachometer;
	sTachometer.JYY = theApp.m_sUserInfo.wchName;
	sTachometer.JYYSFZH = L"123456200001010101";
	strTemp = si.GetString(L"Tachometer", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sTachometer.odtbegin.ParseDateTime(strTemp))
	{
		sTachometer.odtbegin = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"Tachometer", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sTachometer.odtEnd.ParseDateTime(strTemp))
	{
		sTachometer.odtEnd = COleDateTime::GetCurrentTime();
	}

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs1", L"")));
	sTachometer.bzzs1 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc1", L"")));
	sTachometer.sc1 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs2", L"")));
	sTachometer.bzzs2 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc2", L"")));
	sTachometer.sc2 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs3", L"")));
	sTachometer.bzzs3 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc3", L"")));
	sTachometer.sc3 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs4", L"")));
	sTachometer.bzzs4 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc4", L"")));
	sTachometer.sc4 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs5", L"")));
	sTachometer.bzzs5 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc5", L"")));
	sTachometer.sc5 = strTemp.GetString();

	SHBMsg sHBMsg;
	CZYHttp_PAI::GetInstance().SetSpeCheck(sTachometer, sHBMsg);
	
	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"�ϴ��ɹ�");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"�ϴ�ʧ�ܣ�%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}
}


DWORD CParaDlg::UpEnvParamsAndTachometerProcessCtrlCallBack(const DWORD dwStatus)
{
	// �ϴ���������
	switch (dwStatus)
	{
	case 0x01:
		{
			// ��������
			UpEnv();
		}
		break;
	case 0x02:
		{
			// ת��
			UpTac();
		}
		break;
	}

	return 0x00;
}
