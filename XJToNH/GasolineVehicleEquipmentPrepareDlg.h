#pragma once


// CGasolineVehicleEquipmentPrepareDlg 对话框

class CGasolineVehicleEquipmentPrepareDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CGasolineVehicleEquipmentPrepareDlg)

public:
	CGasolineVehicleEquipmentPrepareDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGasolineVehicleEquipmentPrepareDlg();

// 对话框数据
	enum { IDD = IDD_GASOLINE_VEHICLE_EQUIPMENT_PREPARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnRetry();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnAnaZero();
	afx_msg void OnBnClickedBtnAnaChkEc();
	afx_msg void OnBnClickedBtnAnaChkRc();
	afx_msg void OnBnClickedBtnFloChkO2();
	afx_msg void OnClose();

private:
	CLabel m_lbInfo;
	CButton m_chkZero;
	CButton m_chkEC;
	CButton m_chkRC;
	CButton m_chkFlowmeterCheckO2;
	CShadeTitle m_lbHead;
	CLabel m_lbProbeTips;

private:
	// 初始化控件
	void InitCtrls();
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

	NHCLIENT m_sNHClient;

	
	// 
	bool m_bRet;
	bool m_bFlo;
};
