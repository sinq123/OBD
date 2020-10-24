
// YZDlg.cpp : 实现文件
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


// CYZDlg 对话框




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


// CYZDlg 消息处理程序

BOOL CYZDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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
	strTemp.Format(L"说明：1.OBD检测\n          2.自检上传\n\n使用类型请填写数字");
	GetDlgItem(IDC_ST_ACCOUNT)->SetWindowTextW(strTemp);
	GetDlgItem(IDC_ED_USE_TYPE)->SetWindowTextW(L"1");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYZDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加控件通知处理程序代码

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
		MessageBox(L"填写有空值，请勿留空");
		return;
	}

	CString strSave;
	strSave.Format(L"%s\n%s\n%s\n%s\n%s\n%s\n", strHddSN, strPCName, strTimeSta, strTimeEnd, strDLMM, strUseType);

	std::string sstrSave = KUTF16ToANSI(strSave.GetString());
	byte * pbEnData = new byte[sstrSave.length()]();
	// 加密
	CAes Aes;
	Aes.Encryption((BYTE *)sstrSave.c_str(), pbEnData, sstrSave.length());
	
	//// 解码
	//byte * pbDeData = new byte[sstrSave.length() + 1]();
	//Aes.Decryption(pbEnData, pbDeData, sstrSave.length());
	//CString strCerData = KANSIToUTF16((char*)pbDeData);

	//if (NULL != pbDeData)
	//{
	//	delete[] pbDeData;
	//	pbDeData = NULL;
	//}

	// 保存
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
