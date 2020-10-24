
// SYSQLDlg.h : 头文件
//

#pragma once

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

// CSYSQLDlg 对话框
class CSYSQLDlg : public CDialogEx
{
// 构造
public:
	CSYSQLDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSYSQLDlg(void);

// 对话框数据
	enum { IDD = IDD_SYSQL_DIALOG };

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
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedButtonUpdateVehicleList();

private:
	// 数据源
	CEdit m_edCatalog;
	// 数据库名称
	CEdit m_edSource;
	// 登录名
	CEdit m_edUserID;
	// 密码
	CEdit m_edPassWord;
	// OBD检测员列表
	CComboBox m_comOBDJCY;
	// 信息
	CEdit m_edMsg;

	// 车牌号码
	CLabel m_lbPlateNumber;
	// 车架号
	CLabel m_lbVIN;
	// 报告单编号
	CLabel m_lbRepoetNumber;
	// 日志编号
	CLabel m_lbRunningNumber;
	// 检测方法
	CLabel m_lbTestType;
	// OBD检测
	CLabel m_lbOBDItem;

public:
	CString ToTestType(const CString& strTestTypeCode);
	CString ToItem(const CString& strItemCode);
	// 获取配置信息
	void SetConfig(void);
	// OBD检测员
	void GetOBDJCY(void);
	// 获取检测参数
	bool GetTestLog(CString& strMsg);

	// OBD数据部分
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);


private:
	// 检测日志
	TESTLOG m_sTestLog;
	// 日志路径
	CString m_strLogFilePath;
};
