#pragma once


// CLogIn �Ի���

class CLogIn : public CDialogEx
{
	DECLARE_DYNAMIC(CLogIn)

public:
	CLogIn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogIn();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDLMM;

	afx_msg void OnBnClickedOk();
};
