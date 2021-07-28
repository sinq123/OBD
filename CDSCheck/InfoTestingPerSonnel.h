#pragma once


// CInfoTestingPerSonnel 对话框

class CInfoTestingPerSonnel : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoTestingPerSonnel)

public:
	CInfoTestingPerSonnel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoTestingPerSonnel();

// 对话框数据
	enum { IDD = IDD_TESTING_PER_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdJyy();
};
