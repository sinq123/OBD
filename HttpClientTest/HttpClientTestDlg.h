
// HttpClientTestDlg.h : ͷ�ļ�
//

#pragma once

#include "..\CurlHttp\CurlHttp.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\CurlHttp_D.lib")
#else
#pragma comment(lib, "..\\Release\\CurlHttp.lib")
#endif

// CHttpClientTestDlg �Ի���
class CHttpClientTestDlg : public CDialogEx
{
// ����
public:
	CHttpClientTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPCLIENTTEST_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();

private:
	CEdit m_edPort;
	CIPAddressCtrl m_IPAddress;

};
