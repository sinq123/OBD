#pragma once


// CInfoTestingPerSonnel �Ի���

class CInfoTestingPerSonnel : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoTestingPerSonnel)

public:
	CInfoTestingPerSonnel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoTestingPerSonnel();

// �Ի�������
	enum { IDD = IDD_TESTING_PER_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdJyy();
};
