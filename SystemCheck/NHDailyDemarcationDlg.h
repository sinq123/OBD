#pragma once

#include "DynDlg.h"
#include "OpaDlg.h"
#include "SmoDlg.h"
#include "AnaDlg.h"
#include "NOxDlg.h"
#include "FloDlg.h"
#include "ParaDlg.h"
#include "ObdDlg.h"

// CNHDailyDemarcationDlg 对话框

class CNHDailyDemarcationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHDailyDemarcationDlg)

public:
	CNHDailyDemarcationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHDailyDemarcationDlg();

// 对话框数据
	enum { IDD = IDD_NH_DAILY_DEMARCATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedBtnClose();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CShadeTitle m_lbTitle;

private:
	// 初始化控件
	void InitCtrls();
	// 设置对话框字体
	void SetDlgFont(void);
	// 载入子窗口
	void LoadSubDlg(void);

private:
	// 标定是否Pass
	bool m_bDemarcationPass;
	// 设备校准检查信息
	EQUCALCHKINFO m_sEquCalChkInfo;
	// 检测线信息
	LINEINFO m_sLineInfo;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

	// 测功机子窗口
	CDynDlg m_DynDlg;
	// 不透光度计子窗口
	COpaDlg m_OpaDlg;
	// 烟度计子窗口
	CSmoDlg m_SmoDlg;
	// 分析仪子窗口
	CAnaDlg m_AnaDlg;
	// 氮氧分析仪子窗口
	CNOxDlg m_NOxDlg;
	// 流量计子窗口
	CFloDlg m_FloDlg;
	// 参数测量仪子窗口
	CParaDlg m_ParaDlg;
	// OBD子窗口
	CObdDlg m_ObdDlg;

	std::vector<int> m_vtID;
public:
	
};
