
// SocketServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MYLib_D.lib")
#else
#pragma comment( lib, "..\\Release\\MYLib.lib" )
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketServerDlg �Ի���

CSocketServerDlg *g_pThisDlg = NULL;


CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bResponseRunning = false;
}

CSocketServerDlg:: ~CSocketServerDlg()
{

}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_BOOT_PORT, m_edBootPort);
	DDX_Control(pDX, IDC_PARENT_SERVE, m_IAParentIP);
	DDX_Control(pDX, IDC_ED_PARENT_PORT, m_edParentPort);

	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_reLog);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CSocketServerDlg::OnBnClickedBtnStart)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSocketServerDlg ��Ϣ�������

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	g_pThisDlg = this;

	LoadConfig();

	OnBnClickedBtnStart();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSocketServerDlg::OnPaint()
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
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocketServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	
	m_bResponseRunning = false;
}

void CSocketServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bResponseRunning)
	{
		AfxMessageBox(L"����ֹֹͣͣ����");
		return;
	}

	CDialogEx::OnClose();
}

void CSocketServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSocketServerDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	m_btnStart.GetWindowTextW(strTemp);

	if (L"��������" == strTemp)
	{
		// �������˲���
		CString strBootPort, strParentIP, strParentPort;
		m_edBootPort.GetWindowTextW(strBootPort);
		m_IAParentIP.GetWindowTextW(strParentIP);
		m_edParentPort.GetWindowTextW(strParentPort);

		if (L""==strBootPort || L"0"==strBootPort)
		{
			//AfxMessageBox(L"����������˿�");
			PrintLog(L"����������˿�");
			m_edBootPort.SetFocus();
			return;
		}

		if (L""==strParentPort || L"0"==strParentPort)
		{
			//AfxMessageBox(L"����������˿�");
			PrintLog(L"�������ϼ��˿�");
			m_edParentPort.SetFocus();
			return;
		}

		if (strParentIP.IsEmpty())
		{
			//AfxMessageBox(L"����������˿�");
			PrintLog(L"�������ϼ��˿�");
			m_IAParentIP.SetFocus();
			return;
		}

		SetConfig();
		m_tcpService.SetServer(_wtoi(strBootPort));
		if (!m_tcpService.Listen())
		{
			//AfxMessageBox(L"����ʧ��");
			PrintLog(L"����ʧ��");
			return;
		}

		m_tcpClient.SetClient(strParentIP, _wtoi(strParentPort));
		if (!m_tcpClient.Listen())
		{
			m_tcpService.DisConnect();
			PrintLog(L"socket��ʧ��");
			return;
		}


		CreateThread(NULL, 0, ListenThread, 0, 0, NULL);	
		m_bResponseRunning = true;

		m_btnStart.SetWindowTextW(L"ֹͣ����");
		m_edBootPort.EnableWindow(FALSE);
		m_edParentPort.EnableWindow(FALSE);
		m_IAParentIP.EnableWindow(FALSE);

	}
	else
	{
		m_bResponseRunning = false;
		m_tcpService.DisConnect();
		m_tcpClient.DisConnect();
		m_btnStart.SetWindowTextW(L"��������");
		m_edBootPort.EnableWindow(TRUE);
		m_edParentPort.EnableWindow(TRUE);
		m_IAParentIP.EnableWindow(TRUE);

		PrintLog(L"ֹͣ����");
	}
}


void CSocketServerDlg::PrintLog(LPCTSTR szFormat, ...)
{
	TCHAR szBuf[4096] = {0};
	va_list args;
	va_start(args, szFormat);
	_vstprintf(szBuf, 4096, szFormat, args);
	va_end(args);

	CString strLog;
	strLog.AppendFormat(_T("%s	%s\r\n"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), szBuf);
		
	m_reLog.LineScroll(m_reLog.GetLineCount());
	int nLen = m_reLog.GetWindowTextLength();
	m_reLog.SetSel(nLen, nLen);
	m_reLog.ReplaceSel(strLog);

}

void CSocketServerDlg::LoadConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"SocketService.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	CString strTemp;
	//�����˿�
	//BootPort=10012
	strTemp = (si.GetString(L"Service", L"BootPort", L"10022"));
	m_edBootPort.SetWindowTextW(strTemp);
	//�ϼ�������IP
	strTemp = (si.GetString(L"Service", L"ParentIP", L"127.0.0.1"));
	m_IAParentIP.SetWindowTextW(strTemp);
	//�ϼ��������˿�
	strTemp = (si.GetString(L"Service", L"ParentPort", L"10023"));
	m_edParentPort.SetWindowTextW(strTemp);

}

void CSocketServerDlg::SetConfig(void)
{
	wchar_t wchFilePath[MAX_PATH] = { L'\0' };
	CNHCommonAPI::GetHLDFilePath(L"Config", L"SocketService.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	CString strTemp;
	//�����˿�
	m_edBootPort.GetWindowTextW(strTemp);
	(si.SetString(L"Service", L"BootPort", strTemp));
	//�ϼ�������IP
	m_IAParentIP.GetWindowTextW(strTemp);
	(si.SetString(L"Service", L"ParentIP", strTemp));
	//�ϼ��������˿�
	m_edParentPort.GetWindowTextW(strTemp);
	(si.SetString(L"Service", L"ParentPort", strTemp));
}


DWORD CSocketServerDlg::ListenThread(LPVOID lpParam)
{
	TRACE("ListenThread�߳̿�ʼ...\n");	
	g_pThisDlg->PrintLog(L"�����߳̿�ʼ...");	
	while (true)  
	{ 
		SOCKET sClient = g_pThisDlg->m_tcpService.Accept();
		if (sClient == SOCKET_ERROR)
		{
			break;
		}
		CreateThread(NULL, 0, ResponseThread, (LPVOID)sClient, 0, NULL);
	}
	
	TRACE("ListenThread�߳��˳�...\n");
	g_pThisDlg->PrintLog(L"�����߳��˳�...");
	return 0;
}

DWORD CSocketServerDlg::ResponseThread(LPVOID lpParam)
{
	TRACE("ResponseThread�߳�[%d]��ʼ...\n", GetCurrentThreadId());
	SOCKET sClient = (SOCKET)lpParam;
	g_pThisDlg->PrintLog(L"�����߳�[%d]��ʼ...", GetCurrentThreadId());
	g_pThisDlg->PrintLog(L"�ͻ���[%d]%s����...", sClient, g_pThisDlg->m_tcpService.GetPeerInfo(sClient));
	
	// �ȴ�socket����
	Sleep(200);	

	while (g_pThisDlg->m_bResponseRunning)
	{
		ZDPacket sZDPSend(L"");	// struct���ͣ������Զ����ù��캯��
		if (!g_pThisDlg->m_tcpService.RecvReqPacket(sClient, sZDPSend))
		{
			break;
		}
		else
		{
			// ������ת�ϴ��󷵻�
			ZDPacket sZDPRecv(L"");
			g_pThisDlg->m_tcpClient.SendAndRecvPacket(sZDPSend, sZDPRecv);

			g_pThisDlg->PrintLog(L"�յ�%s����", g_pThisDlg->m_tcpService.GetPeerInfo(sClient));
			// ��Ӧ����		
#ifdef _DEBUG
			CString strXML(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
			strXML.AppendFormat(L"<response>");
			strXML.AppendFormat(L"<cmd>GetVCIState</cmd>");
			strXML.AppendFormat(L"<code>0</code>");
			strXML.AppendFormat(L"<msg>�ɹ�</msg>");
			strXML.AppendFormat(L"</response>");
			ZDPacket zd(strXML);
			if (sZDPRecv.strXML.empty())
			{
				sZDPRecv = zd;
			}
#endif
			if (!g_pThisDlg->Response(sClient, sZDPRecv))
			{
				break;
			}
		}
	}
	
	// �رղ�ɾ����¼
	g_pThisDlg->m_tcpService.ClostClient(sClient);

	TRACE("ResponseThread�߳�[%d]�˳�...\n", GetCurrentThreadId());
	g_pThisDlg->PrintLog(L"�����߳�[%d]�˳�...", GetCurrentThreadId());

	return 0;
}

bool CSocketServerDlg::Xml2Map(LPCTSTR szXML, std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, std::map<std::wstring, std::wstring> &mapData)
{
	CXmlReader xmlReader;
	if (xmlReader.Parse(szXML))
	{
		if (xmlReader.OpenNode(L"response/cmd"))
		{
			xmlReader.GetNodeContent(strCmd);
		}
		if (xmlReader.OpenNode(L"response/code"))
		{
			xmlReader.GetNodeContent(strCode);
		}
		if (xmlReader.OpenNode(L"response/msg"))
		{
			xmlReader.GetNodeContent(strMsg);
		}
		if (xmlReader.OpenNode(L"response/data"))
		{
			xmlReader.EnterNode();
			do
			{
				std::wstring strName, strContent;
				xmlReader.GetNodeName(strName);
				xmlReader.GetNodeContent(strContent);
				mapData[strName] = strContent;
			}
			while (xmlReader.NextSibling());
		}
		return true;
	}
	return false;
}

bool CSocketServerDlg::Response(SOCKET sockfd, ZDPacket &sPacketReq)
{
	std::wstring strCmd, strCode, strMsg;
	std::map<std::wstring, std::wstring> mapData;

	if (Xml2Map(sPacketReq.strXML.c_str(), strCmd, strCode, strMsg, mapData))
	{
		if (strCmd == L"")
		{
		}
	}
	// �Ƿ�����ɹ���Ҫת��
	if (m_tcpService.Response(sockfd, sPacketReq))
	{
		return true;
	}
	return false;
}