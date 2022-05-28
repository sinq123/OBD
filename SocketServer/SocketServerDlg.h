
// SocketServerDlg.h : ͷ�ļ�
//

#pragma once
#include "TCPService.h"
#include "TCPClient.h"

#include <map>

// CSocketServerDlg �Ի���
class CSocketServerDlg : public CDialogEx
{
// ����
public:
	CSocketServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSocketServerDlg();

// �Ի�������
	enum { IDD = IDD_SOCKETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStart();

private:
	static DWORD WINAPI ListenThread(LPVOID lpParam);
	static DWORD WINAPI ResponseThread(LPVOID lpParam);

	bool Response(SOCKET sockfd, ZDPacket &sPacketReq);
	bool Xml2Map(LPCTSTR szXML, std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, std::map<std::wstring, std::wstring> &mapData);

private:
	void PrintLog(LPCTSTR szFormat, ...);

private:
	CRichEditCtrl m_reLog;
	CEdit m_edBootPort;
	CIPAddressCtrl m_IAParentIP;
	CEdit m_edParentPort;
	CButton m_btnStart;


private:
	// ��������
	void LoadConfig(void);
	// ��������
	void SetConfig(void);


private:
	CTCPService m_tcpService;
	CTCPClient m_tcpClient;

	bool m_bResponseRunning;
public:
	
};
