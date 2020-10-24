
// SGSCheDlg.h : 头文件
//

#pragma once
#include "SelfInsrectionItemsDlg.h"
// CSGSCheDlg 对话框
class CSGSCheDlg : public CDialogZoom
{
// 构造
public:
	CSGSCheDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSGSCheDlg();
// 对话框数据
	enum { IDD = IDD_SGSCHE_DIALOG };

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
	afx_msg void OnBnClickedBtnStatr();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnRestatr();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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

	// 
	CLabel m_lb1;
	CLabel m_lb2;
	CLabel m_lb3;
	CLabel m_lb4;
	CLabel m_lb5;
	CLabel m_lb6;
	CLabel m_lb7;
	CLabel m_lb8;
	CLabel m_lb9;
	CLabel m_lb10;
	CLabel m_lb11;
	CLabel m_lb12;
	CLabel m_lb13;
	CLabel m_lb14;
	CLabel m_lb15;
	CLabel m_lb16;
	CLabel m_lb17;
	CLabel m_lb18;
	//
	CLabel m_lbNiuLi;
	CLabel m_lbGongLv;

	CLabel m_lbCO;
	CLabel m_lbHC;
	CLabel m_lbNO;
	CLabel m_lbCO2;
	CLabel m_lbO2;
	CLabel m_lbPEF;

	CLabel m_lbNO2;
	CLabel m_lbCO22;
	CLabel m_lbN;
	CLabel m_lbNs;
	CLabel m_lbLLJO2;
	CLabel m_lbLLJBZ;
	CLabel m_lbLLJSJ;
private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 是否最大话后踩附加
	void LoadSubDlg(void);
private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	HBRUSH m_hrPicture;

private:
	// 检测站信息
	CString m_strSelfInfoTimes;
public:
	

private:
	CSelfInsrectionItemsDlg m_SelfInsrectionItemsDlg;
};
