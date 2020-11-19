
// WriModparDlg.h : 头文件
//

#pragma once


// CWriModparDlg 对话框
class CWriModparDlg : public CDialogEx
{
	// 构造
public:
	CWriModparDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CWriModparDlg();
	// 对话框数据
	enum { IDD = IDD_WRIMODPAR_DIALOG };

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

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 待检车ListCtrl字体
	CFont m_fontVehicleLCFont;

private:
	// 控件随窗口变化而变化
	CPoint m_Old;//存放对话框的宽和高

private:
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedLstPending(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpVehicle();
	afx_msg void OnBnClickedBtnUpPending();

private:
	CListCtrl m_lstVehicle;
	CListCtrl m_lstPending;

	CButtonST m_btnUpVehicle;
	CButtonST m_btnUpPending;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 初始化待检测列表
	void InitVehicleList(void);

private:

	char* m_pchURL;
	CStringW m_strURL;
	CStringW m_strStationNum;
	CStringW m_strLineNum;
	CStringW m_strDetectLSBH;
	COleDateTime m_odtDemarcationDate;

	// 生成日志文件
	CStringW m_strIntLogFilePath;
	void GetLogFilePath(void);

	// 获取配置信息
	void GetConfig(void);

	CString DecodeURI(LPCSTR szURIString);
	CString DecodeURI(LPCWSTR szURIString);

private:
	// 获取平台车辆列表
	void GetIntVehList(void);
	// 获取待处理列表
	void GetPending(void);

	bool SaveToDB(void);
	bool DetWhiModPar(void);
};
