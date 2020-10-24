
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// COpaDlg 对话框

class COpaDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(COpaDlg)

public:
	COpaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpaDlg();

// 对话框数据
	enum { IDD = IDD_OPA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonOpa();

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
	// 写LineInfo表
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo表
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo.ini文件
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// 保存不透光校准检查数据
	static DWORD WriteOpaCalChkData(void);
	// 写DemarcationLog表
	static DWORD WriteDemarcationLog(void);
	// 写DemarcationResultOfOpaCalChk表
	static DWORD WriteDemarcationResultOfOpaCalChk(void);
	// 不透光校准检查过程控制回调函数
	static DWORD __stdcall ProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);

private:
	// 标定是否Pass
	static bool m_bDemarcationPass;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;


};
