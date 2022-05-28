
// SocketServerDlg.h : 头文件
//

#pragma once
#include "TCPService.h"
#include "TCPClient.h"

#include <map>

// CSocketServerDlg 对话框
class CSocketServerDlg : public CDialogEx
{
// 构造
public:
	CSocketServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSocketServerDlg();

// 对话框数据
	enum { IDD = IDD_SOCKETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 加载配置
	void LoadConfig(void);
	// 保存配置
	void SetConfig(void);


private:
	CTCPService m_tcpService;
	CTCPClient m_tcpClient;

	bool m_bResponseRunning;
public:
	
};
