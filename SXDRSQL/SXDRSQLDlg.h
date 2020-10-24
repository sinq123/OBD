
// SXDRSQLDlg.h : 头文件
//

#pragma once
#include <string>
#include <list>
#include "NSSocket.h"

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
#include "afxwin.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif


// CSXDRSQLDlg 对话框
class CSXDRSQLDlg : public CDialogEx
{
// 构造
public:
	CSXDRSQLDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSXDRSQLDlg(void);
// 对话框数据
	enum { IDD = IDD_SXDRSQL_DIALOG };

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
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpinter();
public:
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;

	CEdit m_edServerIP;
	CEdit m_edServerPost;
	CEdit m_edLineNumber;
	CEdit m_edOBDJCY;

	CEdit m_edMessage;

	CEdit m_edPlateNumber;
	CListCtrl m_lstVehicle;

	CEdit m_edOBDCZYMM;
	CEdit m_edOBDCZYID;

	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);

public:
	// 待检列表
	bool GetVehicleList(void);

	CString m_strRunningNumber;

	// 发送接收数据包
	bool SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv);
	
	bool CloaseScket(void);
	
};
