
// HttpClientTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HttpClientTest.h"
#include "HttpClientTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHttpClientTestDlg �Ի���




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


// CHttpClientTestDlg ��Ϣ�������

BOOL CHttpClientTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHttpClientTestDlg::OnPaint()
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
HCURSOR CHttpClientTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpClientTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
