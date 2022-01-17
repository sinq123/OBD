// OpaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpaDlg.h"

// COpaDlg �Ի���

IMPLEMENT_DYNAMIC(COpaDlg, CDialogZoom)

// �궨�Ƿ�Pass
bool COpaDlg::m_bDemarcationPass = false;

COpaDlg::COpaDlg(CWnd* pParent /*=NULL*/) 
	: CDialogZoom(COpaDlg::IDD, pParent)
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

COpaDlg::~COpaDlg()
{
	m_fontDlgFont.DeleteObject();
}

void COpaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(COpaDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_OPA, &COpaDlg::OnBnClickedButtonOpa)
END_MESSAGE_MAP()


// COpaDlg ��Ϣ�������
BOOL COpaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void COpaDlg::InitCtrls()
{
	// ��ʼ������
	SetDlgFont();

	m_lbTitle.SetTitle(L"��͸��ȼ�");
}

void COpaDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}


void COpaDlg::OnBnClickedButtonOpa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	// ɾ��ԭ���豸У׼�����Ϣ�ļ�
	DeleteEquCalChkInfoFile();

	ShowNHT6CalChkDlg(ProcessCtrlCallBack);
}

void COpaDlg::DeleteEquCalChkInfoFile(void)
{
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// �ļ�������
		return;
	}
	DeleteFile(wchPath);
}

DWORD COpaDlg::CheckEquCalChkInfoFile(void)
{
	// ����ļ��Ƿ����

	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// �ļ�������
		return 0x01;
	}

	// �������Ƿ����

	CSimpleIni si(wchPath);
	CString str = si.GetString(L"EquCalChkInfo", L"IsOperationFinishedAtLeaseOnce", L"0");
	if (L"0" == str)
	{
		// ����δ���
		return 0x02;
	}
	else
	{
		// ��¼�궨�Ƿ�Pass(��������)
		str = si.GetString(L"EquCalChkInfo", L"Judgement", L"0");
		m_bDemarcationPass = (L"ͨ��"==str)?true:false;

		return 0x00;
	}
}

DWORD COpaDlg::WriteEquCalChkInfo(void)
{
	DWORD dwReturn(0);

	// дEquCalChkInfo��
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);

	dwReturn = SetDboEquCalChkInfo(sEquCalChkInfo);
	//CNHModeClientDB_SQLite::SetDboEquCalChkInfo(sEquCalChkInfo);
	return dwReturn;
}

DWORD COpaDlg::WriteDemarcationLog(void)
{
	DWORD dwReturn(0);

	// дDemarcationLog��
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}

DWORD COpaDlg::WriteDemarcationResultOfOpaCalChk(void)
{
	DWORD dwReturn(0);

	// дDemarcationResultOfOpaCalChk��
	DEMARCATIONRESULTOFOPACALCHK sResultOfOpaCalChk;
	ZeroMemory(&sResultOfOpaCalChk, sizeof(DEMARCATIONRESULTOFOPACALCHK));
	GetIniResultOfOpaCalChk(&sResultOfOpaCalChk);

	dwReturn = SetDboDemarcationResultOfOpaCalChk(sResultOfOpaCalChk);
	return dwReturn;
}

DWORD COpaDlg::WriteOpaCalChkData(void)
{
	// Ԥ���EquCalChkinfo.ini�ļ�
	// �����Ŀ��
	// 1)�ļ��Ƿ����
	// 2)�����Ƿ����
	if (0x00 != CheckEquCalChkInfoFile())
	{
		// ��鲻ͨ��
		return 0xff;
	}

	// дEquCalChkInfo��
	WriteEquCalChkInfo();
	
	// дDemarcationLog��
	WriteDemarcationLog();
	
	// дDemarcationResultOfOpaCalChk��
	WriteDemarcationResultOfOpaCalChk();

	if (m_bDemarcationPass)
	{
		// �궨Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		// ����sLineInfo��͸���������
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// У׼����
		ZeroMemory(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate));
		wcscpy_s(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate)/sizeof(wchar_t), str);
		// �������
		ZeroMemory(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate));
		wcscpy_s(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate)/sizeof(wchar_t), str);

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// �궨 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// �����ݿ���¼������Ϣ��Ա����sLineInfo
		ReadLineInfo(sLineInfo);

		//// ����sLineInfo��͸���������
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// У׼����
		//ZeroMemory(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate));
		//wcscpy_s(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate)/sizeof(wchar_t), str);
		//// �������
		//ZeroMemory(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate));
		//wcscpy_s(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchOpacimeterChannelCalibrationDate, L"2000-01-01");
		wcscpy_s(sLineInfo.wchOpacimeterChannelCheckDate, L"2000-01-01");

		// ����Ա����sLineInfoд��LineInfo��
		WriteLineInfo(sLineInfo);
		// ����Ա����sLineInfoд��LineInfo.ini�ļ�
		WriteLineInfoFile(sLineInfo);
	}

	UpGasCheck();

	// ����EquCalChkInfo.ini�ļ�ʹ����ϣ���Ҫɾ��
	DeleteEquCalChkInfoFile();
	return 0x00;
}

DWORD COpaDlg::ReadLineInfo(LINEINFO &sLineInfo)
{
	NHCLIENT sNHClient;
	ZeroMemory(&sNHClient, sizeof(NHCLIENT));
	GetIniNHClient(&sNHClient);

	CString strSql;
	strSql.Format(L"select * from LineInfo where LineNumber = '%s'", sNHClient.wchLineNumber);

	DWORD dwReturn = GetDboLineInfo(strSql, &sLineInfo);

	return dwReturn;
}

DWORD COpaDlg::WriteLineInfo(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// ������飬�Է����ݱ��۸�(���޶ȼ��)
	// �жϼ���߱���Ƿ�Ϊ��
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// ����߱��Ϊ��
		// ��дLineInfo
		dwReturn = 0xff;
	}
	else
	{
		// ����߱�Ų�Ϊ��
		// дLineInfo
		dwReturn = SetDboLineInfo(sLineInfo);
	}

	return dwReturn;
}

DWORD COpaDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// ������飬�Է����ݱ��۸�(���޶ȼ��)
	// �жϼ���߱���Ƿ�Ϊ��
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// ����߱��Ϊ��
		// ��дLineInfo.ini
		dwReturn = 0xff;
	}
	else
	{
		SetIniLineInfo(sLineInfo);
		dwReturn = 0x00;
	}

	return dwReturn;
}

DWORD  COpaDlg::ProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_OPA_CAL_CHK_START:
	case NH_OPA_CAL_CHK_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_OPA_CAL_CHK_FINISH:
		{
			WriteOpaCalChkData();
		}
		break;
	default:
		{
			return 0xff;
		}
		break;
	}
	return 0x00;
}

bool COpaDlg::UpGasCheck(void)
{
	// ��ȡ��͸�����ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOpaCalChk.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"��͸�����ļ�INI����");
		return false;
	}
	CSimpleIni si(wchPath);
	

	struct Sydjzj
	{
		//accessToken	��������	�ַ���	50	���ݼ���߱�ŵ��û�ȡ
		std::wstring accessToken;
		//ldjcjg	��͸���̶ȼ��������
		std::wstring ldjcjg;
		//lgpz1	��͸���̶ȼ�1�˹�Ƭֵ
		std::wstring lgpz1;
		//lgpz2	��͸���̶ȼ�2�˹�Ƭֵ
		std::wstring lgpz2;
		//jcz1	��͸���̶ȼ�1���ֵ
		std::wstring jcz1;
		//jcz2	��͸���̶ȼ�2���ֵ
		std::wstring jcz2;
		//wcl1	��͸���̶ȼ�1�����
		std::wstring wcl1;
		//wcl2	��͸���̶ȼ�2�����
		std::wstring wcl2;
		//jcjg	�����
		std::wstring jcjg;
		//kssj	��鿪ʼʱ��
		std::wstring kssj;
		//jssj	������ʱ��
		std::wstring jssj;
		//bz	��ע
		std::wstring bz;

		Sydjzj() {ZeroMemory(this, sizeof(Sydjzj));}
	};
	Sydjzj sydjzj;

	CStringW strTemp;

	//accessToken	��������
	sydjzj.accessToken = theApp.m_LicenseCode.GetString();
	//ldjcjg	��͸���̶ȼ��������
	sydjzj.ldjcjg = L"1";
	//lgpz1	��͸���̶ȼ�1�˹�Ƭֵ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue1", L"0.00")));
	sydjzj.lgpz1 = strTemp.GetString();
	//lgpz2	��͸���̶ȼ�2�˹�Ƭֵ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue2", L"0.00")));
	sydjzj.lgpz2 = strTemp.GetString();
	//jcz1	��͸���̶ȼ�1���ֵ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue1", L"0.00")));
	sydjzj.jcz1 = strTemp.GetString();
	//jcz2	��͸���̶ȼ�2���ֵ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue2", L"0.00")));
	sydjzj.jcz2 = strTemp.GetString();
	//wcl1	��͸���̶ȼ�1�����
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"RE1", L"0.00")));
	sydjzj.wcl1 = strTemp.GetString();
	//wcl2	��͸���̶ȼ�2�����
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"RE2", L"0.00")));
	sydjzj.wcl2 = strTemp.GetString();
	//jcjg	�����
	sydjzj.jcjg = L"1";
	//kssj	��鿪ʼʱ��
	sydjzj.kssj = strTemp = si.GetString(L"ResultOfOpaCalChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//jssj	������ʱ��
	sydjzj.jssj = COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S").GetString();
	//bz	��ע
	sydjzj.bz = L"";

	std::wstring strRet;
	int nRet = CHNSY_API:: ydjzj(theApp.m_pchURL, sydjzj.accessToken.c_str(), sydjzj.ldjcjg.c_str(), sydjzj.lgpz1.c_str(),sydjzj.lgpz2.c_str(), sydjzj.jcz1.c_str(), 
		sydjzj.jcz2.c_str(), sydjzj.wcl1.c_str(),sydjzj.wcl2.c_str(), sydjzj.jcjg.c_str(), sydjzj.kssj.c_str(), sydjzj.jssj.c_str(), sydjzj.bz.c_str(), strRet);

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
				AfxMessageBox(wstrContent.c_str(), MB_ICONWARNING|MB_OK);
				return false;
			}
			else
			{
				AfxMessageBox(L"�ϴ��ɹ�", MB_ICONWARNING|MB_OK);
				return true;
			}
		}
		else
		{
			AfxMessageBox(L"����ʧ��", MB_ICONWARNING|MB_OK);
			return false;
		}
	}
	else
	{
		AfxMessageBox(L"����ʧ��", MB_ICONWARNING|MB_OK);
		return false;
	}
}