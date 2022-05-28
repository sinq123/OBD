
// DROBDT.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DROBDT.h"
#include "DROBDTDlg.h"
#include "RJYZ.h"
#include "LogIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDROBDTApp

BEGIN_MESSAGE_MAP(CDROBDTApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDROBDTApp ����

CDROBDTApp::CDROBDTApp()
	: m_strIP(L"")
	, m_strPost(L"")
	, m_strDeviceSN(L"")
	, m_strOperatorName(L"")
	, m_strOperatorPassword(L"")

{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDROBDTApp ����

CDROBDTApp theApp;


// CDROBDTApp ��ʼ��

BOOL CDROBDTApp::InitInstance()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, m_pszAppName);
	if (ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, L"�����Ѿ�����", L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

#ifndef _DEBUG
	std::wstring strMsg, strDLMM;
	if (!CRJYZ::IsYZ(strMsg, strDLMM))
	{
		MessageBox(NULL,strMsg.c_str(), L"", MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	CLogIn LogInDlg;
	LogInDlg.m_strDLMM = strDLMM.c_str();
	if (LogInDlg.DoModal() == IDCANCEL)
	{
		return FALSE;
	}
#endif

	GetLogFilePath();
	GetConfig();

	CDROBDTDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void CDROBDTApp::GetLogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"YJOBDTA%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	m_strIntLogFilePath = strLogFilePath;
}

void CDROBDTApp::GetConfig(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"Config", L"NSConfig.ini", wchPath))
	{
		CSimpleIni si(wchPath);

		m_strIP = si.GetString(L"NSServer", L"IP", L"");

		m_strPost = si.GetString(L"NSServer", L"Port", L"");

		m_strDeviceSN = si.GetString(L"NSServer", L"DeviceSN", L"");

		m_strOperatorName = si.GetString(L"NSServer", L"OperatorName", L"");

		m_strOperatorPassword = si.GetString(L"NSServer", L"OperatorPassword", L"");
	}

	if (m_strOperatorName.IsEmpty())
	{
		ZeroMemory(wchPath, sizeof(wchPath));
		if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UserInfo.ini", wchPath))
		{
			CSimpleIni si(wchPath);

			m_strOperatorName = si.GetString(L"UserInfo", L"Name", L"");
		}
	}

}

bool CDROBDTApp::SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv)
{
	CString strXML = szXML;
	strXML.Replace(L" ", L"%20");	// ת��
	strXML.Replace(L"\r", L"%0d");	// ת��
	strXML.Replace(L"\n", L"%0a");	// ת��

	CString strSend;
	CString strURL(L"/CtrlCenter/ASM");
	strSend.Format(L" %s?data=%s\r\n",strURL, strXML);	// ��ַǰ�����пո�

	CNSSocket nsSocket;

	int nPort = _wtoi(m_strPost);

	nsSocket.Open(m_strIP, nPort);
	return nsSocket.SendAndRecvPacket(strSend, strRecv);
}