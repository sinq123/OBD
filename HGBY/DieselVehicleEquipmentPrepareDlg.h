#pragma once


// CDieselVehicleEquipmentPrepareDlg 对话框

class CDieselVehicleEquipmentPrepareDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CDieselVehicleEquipmentPrepareDlg)

public:
	CDieselVehicleEquipmentPrepareDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDieselVehicleEquipmentPrepareDlg();

// 对话框数据
	enum { IDD = IDD_DIESEL_VEHICLE_EQUIPMENT_PREPARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnRetry();
	afx_msg void OnBnClickedBtnOpaCal();
	afx_msg void OnBnClickedBtnNOxZero();

private:
	CLabel m_lbInfo;
	CButton m_chkAdjust;
	CButton m_chkZero;
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

private:
	NHCLIENT m_sNHClient;
	LINEINFO m_sLineInfo;

	bool m_bOpaCal;
};
