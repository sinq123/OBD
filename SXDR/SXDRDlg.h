
// SXDRDlg.h : 头文件
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


struct SOBDVEHLIST
{
	std::wstring strRunningNumber;
	std::wstring strPaletNumber;
	std::wstring strPlateType;
	std::wstring strVIN;
	std::wstring strVehType;
};
// CSXDRDlg 对话框
class CSXDRDlg : public CDialogEx
{
// 构造
public:
	CSXDRDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSXDRDlg(void);
// 对话框数据
	enum { IDD = IDD_SXDR_DIALOG };

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
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpinter();

public:
	CEdit m_edServerIP;
	CEdit m_edServerPost;
	CEdit m_edLineNumber;
	CEdit m_edOBDJCY;
	CListCtrl m_lstVehicle;
	CLabel m_lbMessage;

	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);

public:
	// 发送接收数据包
	bool SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv);

	// 获取列表
	bool GetVehList(void);
	
	void CloaseScket(void);

	SOBDVEHLIST m_sOBDVehList;
	
};
