
// SGOBDDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

// CSGOBDDlg 对话框
class CSGOBDDlg : public CDialogEx
{
// 构造
public:
	CSGOBDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SGOBD_DIALOG };

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

	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedBtnUpTelet();
	afx_msg void OnCbnSelchangeCbJcff();
	afx_msg void OnCbnSelchangeCbIsobdjc();
public:
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;
	CEdit m_edPlateNumber;
	CEdit m_edMessAge;
	CEdit m_edURL;
	CEdit m_edKey;
	CEdit m_edStationNum;
	CEdit m_edLineNUmber;

	CListCtrl m_lstVehicle;
	CComboBox m_cbOBDJCY;
	CComboBox m_cbJCFF;
	CComboBox m_cbIsOBD;

	CDateTimeCtrl m_dtcBegTime;
	CDateTimeCtrl m_dtcEndTime;

public:
	SASM m_sASM;
	SJZJS m_sJZJS;
	SSDS m_sSDS;
	SZYJS_Btg m_sZYJS_Btg;
	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);
	// 待检列表
	bool GetVehicleList(void);
	
	// 获取列表信息
	CStringW m_strIDDB;
	CStringW m_strPlateNum;
	CStringW m_strJCFF;
	CStringW m_strVIN;
	// 保存OBD检测数据
	bool SetOBDLOG(_ConnectionPtr &pConnection, SOBD& sOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	// IUPR
	// bool bFir true: 第一个大于第二个，flase 第二个大于第一个
	void GetIUPR(int& nIUPR1, int& nIUPR2, float& fIUPR, const bool& bFir = true);

	// 上传
	bool UPReport(_ConnectionPtr &pConnection, const SOBD& sOBD);
	bool UPRealTimeData(_ConnectionPtr &pConnection);
	bool UPOBDIUPR(_ConnectionPtr &pConnection, const SOBD& sOBD);

	// ASM OBD过程
	void GetOBDTimeDataASM(CString& strOBDJQMJDKD, CString& strMMOBDJSFHZ, CString& strMMOBDQYCGQXH, CString& strMMOBDSPEED,
		CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDJQYL, CString& strMMOBDLAMBDA, const float& fSpeed, const int& nCou);
	// SDS OBD过程
	void GetOBDTimeDataSDS(CString& strOBDJQMJDKD, CString& strMMOBDJSFHZ, CString& strMMOBDQYCGQXH, CString& strMMOBDSPEED,
		CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDJQYL, CString& strMMOBDLAMBDA, const int& nZS, const int& nCou);
	// JZJS OBD过程
	//MMOBDYMKD->油门开度
	//MMOBDSPEED->车速
	//MMOBDPOWER->发动机输出功率
	//MMOBDROTATESPEED->转速
	//MMOBDJQL->进气量
	//MMOBDZYYL->增压压力
	//MMOBDHYL->耗油量
	//MMOBDNOND->氮氧传感器浓度
	//MMOBDNSPSL->尿素喷射量
	//MMOBDWD->排气温度
	//MMOBDKLBJQYC->颗粒捕集器压差
	//MMOBDEGRKD->EGR 开度
	//MMOBDRYPSYL->燃油喷射压力
	void GetOBDTimeDataJZJS(CString& strMMOBDYMKD, CString& strMMOBDSPEED, CString& strMMOBDPOWER, CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDZYYL, 
		CString& strMMOBDHYL, CString& strMMOBDNOND, CString& strMMOBDNSPSL, CString& strMMOBDWD, CString& strMMOBDKLBJQYC, CString& strMMOBDEGRKD, CString& strMMOBDRYPSYL,
		const float& fSpeed, const int& nCou);
	// ZYJS OBD过程
	void GetOBDTimeDataZYJS(CString& strMMOBDYMKD, CString& strMMOBDSPEED, CString& strMMOBDPOWER, CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDZYYL, 
		CString& strMMOBDHYL, CString& strMMOBDNOND, CString& strMMOBDNSPSL, CString& strMMOBDWD, CString& strMMOBDKLBJQYC, CString& strMMOBDEGRKD, CString& strMMOBDRYPSYL,
		const int& nZS, const int& nCou);
};
