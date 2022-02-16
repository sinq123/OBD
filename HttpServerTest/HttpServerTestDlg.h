
// HttpServerTestDlg.h : ͷ�ļ�
//


#pragma once
#include "mongoose.h"

#include "..\CurlHttp\CurlHttp.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\CurlHttp_D.lib")
#else
#pragma comment(lib, "..\\Release\\CurlHttp.lib")
#endif

// CHttpServerTestDlg �Ի���
class CHttpServerTestDlg : public CDialogEx
{
// ����
public:
	CHttpServerTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPSERVERTEST_DIALOG };

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

private:
	afx_msg void OnBnClickedBtnStartWebservice();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

private:
	CEdit m_edBootPort;
	CEdit m_edParentPort;
	CIPAddressCtrl m_IAParentIP;

	CRichEditCtrl m_reLog;

private:
	void PrintLog(LPCTSTR szFormat, ...);
	
private:
	// ��������
	void LoadConfig(void);
	// ��������
	void SetConfig(void);
	// ����WebService����
	void StartWebService(void);

private:
	// �����˿�
	static CString m_strBootPort;
	// �ϼ�������IP
	static CString m_strParentIP;
	// �ϼ��������˿�
	static CString m_strParentPort;
private:
	// ���߳�
	static int env_handler(struct mg_connection *conn);

	// ���߳�
	static int request_handler(struct mg_connection *conn);
	static void *serve(void *server);

private:
	int CurlPost(const std::wstring strURl, const std::wstring strPost, std::wstring& strRet);

public:
	
};
