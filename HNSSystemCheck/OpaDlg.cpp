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
	// �趨���������ļ���־
	CHYInterfaceLib_API::SetLogFilePath(theApp.m_strIntLogFilePath.GetString());
	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOpaCalChk.ini", wchPath, false))
	{
		// �ļ�������
		AfxMessageBox(L"��͸�����ļ�INI����");
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"��͸���ϴ�", L"��͸�����ļ�INI����");
		return false;
	}
	CSimpleIni si(wchPath);

	CStringW strData, strTemp, strPass;
	strData.AppendFormat(L"<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	strData.AppendFormat(L"<root><zj>");

	//tsNo	���������
	strData.AppendFormat(L"<tsNo>%s</tsNo>", theApp.m_StationNum);
	//testLineNo	����߱��
	strData.AppendFormat(L"<testLineNo>%s</testLineNo>", theApp.m_LineNum);
	//jcrq	�������
	strData.AppendFormat(L"<jcrq>%s</jcrq>", COleDateTime::GetCurrentTime().Format(L"%Y%m%d"));
	//jckssj	��鿪ʼʱ��
	COleDateTime otd1;
	strTemp = si.GetString(L"ResultOfOpaCalChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strData.AppendFormat(L"<jckssj>%s</jckssj>", otd1.Format(L"%Y%m%d%H%M%S"));
	//gxsxswc	������ϵ�����
	strTemp = si.GetString(L"ResultOfOpaCalChk", L"AE1", L"0.0");
	strData.AppendFormat(L"<gxsxswc>%s</gxsxswc>", strTemp);
	//xysj	��Ӧʱ��
	strData.AppendFormat(L"<xysj>%s</xysj>", L"304");
	//yqwdszwc	�����¶�ʾֵ���
	strData.AppendFormat(L"<yqwdszwc>%s</yqwdszwc>", L"");
	//jcjg	�����
	strTemp = si.GetString(L"ResultOfOpaCalChk", L"Pass", L"1");
	strData.AppendFormat(L"<jcjg>%s</jcjg>", strTemp);
	//bhgsm	���ϸ�˵��
	strData.AppendFormat(L"<bhgsm>%s</bhgsm>", L"");
	//jcry	�����Ա
	if (theApp.m_strName.IsEmpty())
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", L"GXCZ1");
	}
	else
	{
		strData.AppendFormat(L"<jcry>%s</jcry>", theApp.m_strName);
	}
	strData.AppendFormat(L"</zj></root>");

	std::wstring strRet;

	int nRet = CHYInterfaceLib_API::ObjectOut(theApp.m_pchURL, theApp.m_strkey.GetString(), L"13W10", strData.GetString(), strRet);

	if (nRet == 0)
	{
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			std::wstring strCode, strContent;
			if (xmlReader.OpenNode(L"root/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
			}
			if (strCode != L"1")
			{
				if (xmlReader.OpenNode(L"root/head/message"))
				{
					xmlReader.GetNodeContent(strContent);
				}
				CString str;
				str.AppendFormat(L"%s,%s, �ϴ�ʧ��", strCode.c_str(), strContent.c_str());
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
				AfxMessageBox(str);
				return false;
			}
			else
			{
				CString str;
				str.AppendFormat(L"%s,%s", strCode.c_str(), L"�ϴ��ɹ�");
				CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", str);
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(theApp.m_strIntLogFilePath, L"��¼", L"���ػ����ϴ�", L"����ʧ��");
		return false;
	}

	return true;
}