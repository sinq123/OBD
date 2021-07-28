
// ACNDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

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

#include "..\ACInterfaceLib\ACInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACInterfaceLib.lib")
#endif

// CACNDlg 对话框
class CACNDlg : public CDialogEx
{
// 构造
public:
	CACNDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ACN_DIALOG };

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
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpTelet();
public:
	// 数据库
	CEdit m_edCatalog;
	CEdit m_edSource;
	CEdit m_edUserID;
	CEdit m_edPassWord;
	// 提示信息
	CEdit m_edMsg;
	// 联网参数
	CEdit m_edJKDZ;
	CEdit m_edJKXLH;
	CEdit m_edStationNumber;
	CEdit m_edPlateNumber;
	// OBD检测员
	CComboBox m_cbOBDJCY;
	// 待检列表
	CListCtrl m_lstVehicle;

	CButton m_edchkSQL;
	CButton m_edIntet;

public:
	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);
	void GetOBDJCY(void);

	// 待检列表
	bool GetVehicleList(void);

	
	// 检测日志
	CString m_strRunningNumber;
	
	bool SetOBDLOG(const TESTLOG& sTestLog, SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	bool UpOBDReaust(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString OBDIURT(const SResultOfOBD& sResultOfOBD);

	bool UpOBDRealTime(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, CString& strMsg);

	bool UpTestEnd(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, CString& strMsg);
	
	CString ToGACode(const CString& PlateType, const CString& PlateNumber, const CString& strVehicleType);
};
