
// DRSXLNDlg.h : 头文件
//

#pragma once
#include "NSSocket.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\NHModeServerDB\NHModeServerDB.h"
#include "afxwin.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

struct SOBDVEHLIST
{
	std::wstring strRunningNumber;
	std::wstring strPaletNumber;
	std::wstring strPlateType;
	std::wstring strVIN;
	std::wstring strVehType;
};

// CDRSXLNDlg 对话框
class CDRSXLNDlg : public CDialogEx
{
// 构造
public:
	CDRSXLNDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRSXLN_DIALOG };

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
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();

public:
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;
	CEdit m_edPlateNumber;
	CEdit m_edMessAge;

	CButton m_edchkSQL;
	CButton m_edIntet;

	CListCtrl m_lstVehicle;
	CComboBox m_cbOBDJCY;

	CEdit m_edServerIP;
	CEdit m_edServerPost;
	CEdit m_edLineNumber;
	CEdit m_edOBDCZYMM;
	CEdit m_edOBDCZYID;

public:
	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);

	// 待检列表
	bool GetVehicleList(void);

	// 检测日志
	CString m_strRunningNumber;
	CString m_strTestType;
	TESTLOG m_sTestLog;
	SResultOfOBD m_sResultOfOBD;
	SOBDVEHLIST m_sOBDVehList;

	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	bool UPReport(const CString& strIP, const CString& strPort, const CString& strOBDCZYID, const CString& strOBDCZYMM,
		const CString& strDeviceSN,const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);

	bool UPRealTimeData(const CString& strIP, const CString& strPort, const CString& strOBDCZYID, const CString& strOBDCZYMM,
		const CString& strDeviceSN,const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);

	// 发送接收数据包
	bool SendAndRecvPacket(const CString& strIP, const CString& strPort, LPCTSTR szXML, std::wstring &strRecv);
	afx_msg void OnBnClickedBtnUpTelet();
};
