
// HttpClientTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HttpClientTest.h"
#include "HttpClientTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHttpClientTestDlg 对话框




CHttpClientTestDlg::CHttpClientTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHttpClientTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHttpClientTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_WEBSERVICE_PORT, m_edPort);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddress);
}

BEGIN_MESSAGE_MAP(CHttpClientTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CHttpClientTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHttpClientTestDlg 消息处理程序

BOOL CHttpClientTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHttpClientTestDlg::OnPaint()
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
HCURSOR CHttpClientTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpClientTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strIP, strPort;
	m_IPAddress.GetWindowTextW(strIP);
	m_edPort.GetWindowTextW(strPort);

	if (strIP.IsEmpty() || strPort.IsEmpty())
	{
		return;
	}

	CString strQURL;
	strQURL.Format(L"http://%s:%s/%s", strIP, strPort, L"detecting-api/restapi/detecting/get_access_token");

	
	std::wstring wstrPostData = L"{\"CompanyId\":\"440111010\",\"IPCType\":\"getAccessToken\",\"IPCType.value\":\"{\\\"password\\\":\\\"00BB1E85\\\",\\\"username\\\":\\\"JCZ440111010\\\"}\",\"Source\":\"440111\"}";
	std::wstring wstrRet;
	CCurlHttp_API::CurlPost(strQURL.GetString(), wstrPostData, wstrRet);

	MessageBox(wstrRet.c_str());
}
