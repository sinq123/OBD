
// YZDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YZ.h"
#include "YZDlg.h"
#include "afxdialogex.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYZDlg �Ի���




CYZDlg::CYZDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYZDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYZDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CYZDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CYZDlg ��Ϣ�������

BOOL CYZDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CString strTemp;
	bool bGetHdd = GetHddSN(strTemp);
	if (bGetHdd)
	{
		strTemp = L"AC" + strTemp;
		GetDlgItem(IDC_ED_YPXLH)->SetWindowTextW(strTemp);
	}

	bool bGetPCName = GetPCName(strTemp);
	if (bGetPCName)
	{
		GetDlgItem(IDC_ED_JSJSJ)->SetWindowTextW(strTemp);
	}
	GetDlgItem(IDC_ED_DLMM)->SetWindowTextW(L"hunancsjcz13579");


	strTemp.Empty();
	strTemp.Format(L"˵����1.OBD���\n          2.�Լ��ϴ�\n\nʹ����������д����");
	GetDlgItem(IDC_ST_ACCOUNT)->SetWindowTextW(strTemp);
	GetDlgItem(IDC_ED_USE_TYPE)->SetWindowTextW(L"1");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CYZDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CYZDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CYZDlg::GetHddSN(CString& strHddSN)
{

	std::string strSN("");
	DiskInfo& di = DiskInfo::GetDiskInfo();
	UINT cnt = di.LoadDiskInfo();
	if (cnt > 0)
	{
		strSN = di.SerialNumber(0);
		strSN.erase(0, strSN.find_first_not_of(" ")); 
		strSN.erase(strSN.find_last_not_of(" ")+1); 
	}
	di.Destroy();

	if (cnt<=0 || ""==strSN)
	{
		return false;
	}
	else
	{
		strHddSN = KANSIToUTF16(strSN.c_str());
		return true;
	}
}

bool CYZDlg::GetPCName(CString &strPCName)
{
	const int MAX_BUFFER_LEN = 500;  
	//char  szBuffer[MAX_BUFFER_LEN];
	DWORD dwNameLen;  

	LPWSTR szBuffer;
	szBuffer = (LPWSTR)malloc(500);
	dwNameLen = MAX_BUFFER_LEN;

	if (!GetComputerName(szBuffer, &dwNameLen))
	{
		return false;
	}
	else  
	{
		strPCName = szBuffer;
		if (strPCName == L"YLMF-2020SKOXRZ")
		{
			strPCName.Format(L"A-PC");
		}
		return true;
	}
}


void CYZDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strPCName, strHddSN, strTimeSta, strTimeEnd, strDLMM, strUseType;
	
	GetDlgItem(IDC_ED_YPXLH)->GetWindowTextW(strHddSN);
	GetDlgItem(IDC_ED_JSJSJ)->GetWindowTextW(strPCName);
	GetDlgItem(IDC_DATETIMEPICKER1)->GetWindowTextW(strTimeEnd);
	GetDlgItem(IDC_ED_DLMM)->GetWindowTextW(strDLMM);
	GetDlgItem(IDC_ED_USE_TYPE)->GetWindowTextW(strUseType);
	strTimeSta.Format(L"%s 00:00:00", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strTimeEnd.AppendFormat(L" 23:59:59");

	if (strPCName.IsEmpty() || strHddSN.IsEmpty() || strTimeSta.IsEmpty() ||
		strTimeEnd.IsEmpty() || strDLMM.IsEmpty() || strUseType.IsEmpty()
		)
	{
		MessageBox(L"��д�п�ֵ����������");
		return;
	}

	CString strSave;
	strSave.Format(L"%s\n%s\n%s\n%s\n%s\n%s\n", strHddSN, strPCName, strTimeSta, strTimeEnd, strDLMM, strUseType);

	std::string sstrSave = KUTF16ToANSI(strSave.GetString());
	byte * pbEnData = new byte[sstrSave.length()]();
	// ����
	CAes Aes;
	Aes.Encryption((BYTE *)sstrSave.c_str(), pbEnData, sstrSave.length());
	
	//// ����
	//byte * pbDeData = new byte[sstrSave.length() + 1]();
	//Aes.Decryption(pbEnData, pbDeData, sstrSave.length());
	//CString strCerData = KANSIToUTF16((char*)pbDeData);

	//if (NULL != pbDeData)
	//{
	//	delete[] pbDeData;
	//	pbDeData = NULL;
	//}

	// ����
	CFileDialog FileDlg(FALSE, L"cer", strHddSN); 
	FileDlg.m_ofn.lpstrTitle = L"Save File";
	//FileDlg.m_ofn.lpstrFilter = L"Text Files(*txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
	if (IDOK == FileDlg.DoModal())
	{
		CFile File;
		File.Open(FileDlg.GetPathName(),CFile::modeCreate| CFile::modeReadWrite);

		File.Write(pbEnData, sstrSave.length());

		File.Close(); 
	}

	if (NULL != pbEnData)
	{
		delete[] pbEnData;
		pbEnData = NULL;
	}

	CDialogEx::OnOK();
}
