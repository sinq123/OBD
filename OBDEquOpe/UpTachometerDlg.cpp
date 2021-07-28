// UpTachometerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpTachometerDlg.h"
#include "afxdialogex.h"


// CUpTachometerDlg �Ի���

IMPLEMENT_DYNAMIC(CUpTachometerDlg, CDialogEx)

CUpTachometerDlg::CUpTachometerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpTachometerDlg::IDD, pParent)
{
	m_odtbegin = COleDateTime::GetCurrentTime();
}

CUpTachometerDlg::~CUpTachometerDlg()
{
}

void CUpTachometerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpTachometerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUpTachometerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUpTachometerDlg ��Ϣ�������


void CUpTachometerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str[10];

	GetDlgItem(IDC_ED_TAC_1000)->GetWindowTextW(str[0]);
	GetDlgItem(IDC_ED_SC_1000)->GetWindowTextW(str[1]);
	GetDlgItem(IDC_ED_TAC_1500)->GetWindowTextW(str[2]);
	GetDlgItem(IDC_ED_SC_1500)->GetWindowTextW(str[3]);
	GetDlgItem(IDC_ED_TAC_2000)->GetWindowTextW(str[4]);
	GetDlgItem(IDC_ED_SC_2000)->GetWindowTextW(str[5]);
	GetDlgItem(IDC_ED_TAC_2500)->GetWindowTextW(str[6]);
	GetDlgItem(IDC_ED_SC_2500)->GetWindowTextW(str[7]);
	GetDlgItem(IDC_ED_TAC_3000)->GetWindowTextW(str[8]);
	GetDlgItem(IDC_ED_SC_3000)->GetWindowTextW(str[9]);

	// ���ж��Ƿ�Ϊ��
	//bool bEmp(false);
	//for (int i = 0; i < 10; i++)
	//{
	//	if (str[i].IsEmpty())
	//	{
	//		bEmp = true;
	//		break;
	//	}
	//}
	//if (bEmp)
	//{
	//	MessageBox(L"ת���п�ֵ");
	//	return;
	//}

	// ��ɾ��֮ǰ���ĵ�
	// ��ȡ�ļ�·��
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		DeleteFile(wchPath);
	}
	
	CString strTemp;
	CString strResult;

	strResult.AppendFormat(L"\r\n[Tachometer]");

	// ��ʼʱ��
	strTemp.Format(L"\r\nStartTime=%s", m_odtbegin.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strResult += strTemp;

	// ����ʱ��
	strTemp.Format(L"\r\nEndTime=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strResult += strTemp;


	//bzzs1	1000r/min��ѧת�ٱ����
	strTemp.Format(L"\r\nbzzs1=%s", str[0]);
	strResult += strTemp;
	//sc1	1000r/minʵ�����
	strTemp.Format(L"\r\nsc1=%s", str[1]);
	strResult += strTemp;

	//bzzs2	1500r/min��ѧת�ٱ����
	strTemp.Format(L"\r\nbzzs2=%s", str[2]);
	strResult += strTemp;
	//sc2	1500r/minʵ�����
	strTemp.Format(L"\r\nsc2=%s", str[3]);
	strResult += strTemp;

	//bzzs3	2000r/min��ѧת�ٱ����
	strTemp.Format(L"\r\nbzzs3=%s", str[4]);
	strResult += strTemp;
	//sc3	2000r/minʵ�����
		strTemp.Format(L"\r\nsc3=%s", str[5]);
	strResult += strTemp;

	//bzzs4	2500r/min��ѧת�ٱ����
	strTemp.Format(L"\r\nbzzs4=%s", str[6]);
	strResult += strTemp;
	//sc4	2500r/minʵ�����
	strTemp.Format(L"\r\nsc4=%s", str[7]);
	strResult += strTemp;

	//bzzs5	3000r/min��ѧת�ٱ����
	strTemp.Format(L"\r\nbzzs5=%s", str[8]);
	strResult += strTemp;
	//sc5	3000r/minʵ�����
	strTemp.Format(L"\r\nsc5=%s", str[9]);
	strResult += strTemp;

	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfTachometer.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CStdioFileEx::modeWriteUnicode);
	// ׷��д��
	sfe.SeekToEnd();
	sfe.WriteString(strResult);
	sfe.Close();

	CDialogEx::OnOK();
}
