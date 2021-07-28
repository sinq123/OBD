
#pragma once

#include "Resource.h"
#include "SystemCheckToNH.h"

// CSmoDlg 对话框

class CSmoDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CSmoDlg)

public:
	CSmoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSmoDlg();

// 对话框数据
	enum { IDD = IDD_SMO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonSmo();

private:
	CShadeTitle m_lbTitle;

private:
	// 初始化控件
	void InitCtrls();
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 删除EquCalChkinfo.ini文件
	static void DeleteEquCalChkInfoFile(void);
	// 检查EquCalChkinfo.ini文件
	// 检查项目：
	// 1)是否存在
	// 2)操作是否完成
	static DWORD CheckEquCalChkInfoFile(void);
	// 写EquCalChkInfo表
	static DWORD WriteEquCalChkInfo(void);
	// 写DemarcationLog表
	static DWORD WriteDemarcationLog(void);
	// 写DemarcationResultOfSmoChk表
	static DWORD WriteDemarcationResultOfSmoChk(void);
	// 写LineInfo表
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo表
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo.ini文件
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// 保存滤纸式烟度计检查数据
	static DWORD WriteSmoChkData(void);
	// 烟度计检查过程控制回调函数
	static DWORD __stdcall ProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);

private:
	// 标定是否Pass
	static bool m_bDemarcationPass;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

};
