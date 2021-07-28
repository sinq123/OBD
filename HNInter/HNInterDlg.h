
// HNInterDlg.h : 头文件
//

#pragma once
#include "json\json.h"

#include "..\HNInterfaceLib\HNInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HNInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HNInterfaceLib.lib")
#endif

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

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

struct SRetString
{
	std::wstring strSuccess;
	std::wstring strMessage;
	std::wstring strData;
};
// CHNInterDlg 对话框
class CHNInterDlg : public CDialogEx
{
// 构造
public:
	CHNInterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HNINTER_DIALOG };

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

	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpinter();
public:
	CLabel m_lbMessage;
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;
	CEdit m_edPlateNumber;
	CEdit m_edJKDZ;
	CEdit m_edJCXBH;
	CEdit m_edYHM;
	CEdit m_edMM;
	CEdit m_edOBDJCY;
	CListCtrl m_lstVehicle;

	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);

public:
	// 待检列表
	bool GetVehicleList(void);

	CString m_strRunningNumber;
	CString m_strTeatType;
	
	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);

	CString LicenseNameToLicenseCode(LPCTSTR szLicenseCode);
	CString Json2Cstring(Json::Value &value);


	void GetOBDResult(const TESTLOG& sTestLog,SResultOfOBD& sResultOfOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
};
