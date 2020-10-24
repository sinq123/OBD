
// SGSCheckDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CSGSCheckDlg 对话框
class CSGSCheckDlg : public CDialogZoom
{
// 构造
public:
	CSGSCheckDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSGSCheckDlg();
// 对话框数据
	enum { IDD = IDD_SGSCHECK_DIALOG };

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
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
private:
	// 退出
	CButtonST m_btnExit;
	// 开始
	CButtonST m_btnState;
	// 重新开始
	CButtonST m_btnReState;
	// 停止
	CButtonST m_btnStop;
	// 设备自检
	CLabel m_lbEquSelfIns;
	// 间隔
	CLabel m_lbInterva;
	// 检漏
	CLabel m_lbGasleakhunnting;
	// 当前时间
	CLabel m_lbCurrentTime;

	//
	CXPGroupBox m_BK1;
	CXPGroupBox m_BK3;
	CButtonST m_chkQYCCoustLoad;
	CButtonST m_chkCYCCoustLoad;
	CButtonST m_chkQYCPLHP;
	CButtonST m_chkCYCPLHP;

	CButtonST m_chkGasChech;
	CButtonST m_chkLowFlowChech;
	CButtonST m_chkSmokeMeter;
	CButtonST m_chkNOxAnalyzeer;
	CButtonST m_chkFlowMeter;
	CButtonST m_chkWetherStation;
	CButtonST m_chkTachoMerter;
	// 车速表
	CMeter m_stSpeedometer;
	// 转速表
	CMeter m_stTachometer;
private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

};
