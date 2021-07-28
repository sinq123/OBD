
// XJLibDlg.h : 头文件
//

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
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

#include "..\PostInterfaceLib\PostInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\PostInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\PostInterfaceLib.lib")
#endif

typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;
// CXJLibDlg 对话框
class CXJLibDlg : public CDialogEx
{
// 构造
public:
	CXJLibDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_XJLIB_DIALOG };

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

	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpTelet();
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

	CButton m_edchkSQL;
	CButton m_edIntet;

	CListCtrl m_lstVehicle;
	CComboBox m_cbOBDJCY;

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

	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);
	CString JsonToCString(const rapidjson::Value &Val);
	//std::string JsonToString(const rapidjson::Value& valObj);

	bool UPReport(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
		const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg);
	bool UPRealTimeData(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
		const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg);
	bool UPOBDIUPR(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
		const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg);
};
