
// JSCSZNDlgDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
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
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "..\SZInterfaceLib\SZInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\SZInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\SZInterfaceLib.lib")
#endif

struct SVehInfoList
{
	//	plate         车牌
	std::wstring strplate;
	//	plate_color   车牌颜色
	std::wstring strplate_color;
	//	vin           车架号
	std::wstring strvin;
	//	clxh          车辆型号
	std::wstring strclxh;
	//	checkid       检测报告编号
	std::wstring strcheckid;
	// check_type 检测类型
	std::wstring strcheck_type;
	// check_method 检测方法
	std::wstring strcheck_method;
};

// CJSCSZNDlgDlg 对话框
class CJSCSZNDlgDlg : public CDialogEx
{
// 构造
public:
	CJSCSZNDlgDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CJSCSZNDlgDlg();
// 对话框数据
	enum { IDD = IDD_JSCSZNDLG_DIALOG };

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
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpobdtimedata();
public:
	// 数据库
	CEdit m_edCatalog;
	CEdit m_edSource;
	CEdit m_edUserID;
	CEdit m_edPassWord;
	CComboBox m_cbOBDJCY;

	// 联网
	CEdit m_edURL;
	CEdit m_edunitid;
	CEdit m_edlineid;
	CEdit m_edtoken;
	CEdit m_edInterusername;
	CEdit m_edInterpwd;
	// 车架VIN
	CEdit m_edVINCJH;
	// 信息
	CEdit m_edMsg;
	// 列表
	CListCtrl m_lstVehicle;
private:
	// 设置待检列表
	void SetVehCtrl(void);
	// 设置控件参数
	void SetConfig(void);

private:
	// 数据库操作日志文件
	CStringW m_strDBLogFilePath;
	// 连接数据库
	bool OpenDB(void);
	// 关闭数据库
	bool CloseDB(void);
	// 是否已经连接数据库
	bool m_bConnectDB;
	// 数据库连接
	_ConnectionPtr m_pConnection;
private:
	// 操作日志文件
	CStringW m_strLogFilePath;
	void LogFilePath(void);
private:
	// OBD检测员
	void GetOBDJCY(void);
	// 登陆成功后返回的唯一标识字符串
	void GetToken(void);
	// 获取平台列表参数
	bool GetVehInfoList(void);
	// 获取文件信息
	CString GetTXT(const CString& strPath);
	// 检测类型和检测方法转换
	// 1,检测类型， 2，检测方法, 3 车牌颜色
	CString ToCheck(const CString& strCheck, const int& nType);

private:
	// 车辆信息参数
	SVehInfoList m_sVehInfoList;
	TestLog m_sTestLog;
	// 获取车辆信息
	bool GetVehInfo(void);


private:
	// OBD检测参数
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	// 保存INI，数据库和上传
	bool SaveINIAndSQLAndUP(void);
	bool UpOBDReaustAndIURP(const SResultOfOBD& sResultOfOBD, CString& strMsg);
	// OBD过程数据
	bool UpOBDRealTimeData(CString& strMsg);
};
