
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

// CDynDlg 对话框

class CDynDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CDynDlg)

public:
	CDynDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDynDlg();

// 对话框数据
	enum { IDD = IDD_DYN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonConstLoad();
	afx_msg void OnBnClickedButtonPlhp();
	afx_msg void OnBnClickedButtonForceCalibration();
	afx_msg void OnBnClickedButtonForceCheck();
	afx_msg void OnBnClickedButtonControlMode();
	afx_msg void OnBnClickedButtonDiw();
	afx_msg void OnBnClickedButtonRespondTime();
	afx_msg void OnBnClickedButtonVarLoad();
	afx_msg void OnBnClickedButtonPlhpJjf();
	afx_msg void OnBnClickedButtonVarLoadJjf();

private:
	CShadeTitle m_lbTitle;

private:
	// 初始化控件
	void InitCtrls();
	// 设置对话框字体
	void SetDlgFont(void);

private:

private:
	// 删除上传的临时文件.ini文件
	static void DeleteInfoFile(void);
	// 写EquCalChkInfo.ini
	static DWORD WriteEquCalChkInfoFile(int nDemarcationItem);
	// 写EquCalChkInfo表
	static DWORD WriteEquCalChkInfo(void);
	// 写DemarcationLog.ini
	static DWORD WriteDemarcationLogFile(int nDemarcationItem);
	// 写DemarcationLog表
	static DWORD WriteDemarcationLog(void);
	// 写DemarcationResultOfDynConstLoad.ini
	static DWORD WriteDemarcationResultOfDynConstLoadFile(void);
	// 写DemarcationResultOfDynConstLoad表
	static DWORD WriteDemarcationResultOfDynConstLoad(void);
	// 写DemarcationResultOfDynPLHP.ini
	static DWORD WriteDemarcationResultOfDynPLHPFile(void);
	// 写DemarcationResultOfDynPLHP表
	static DWORD WriteDemarcationResultOfDynPLHP(void);
	// 写DemarcationResultOfDynForceCal.ini
	static DWORD WriteDemarcationResultOfDynForceCalFile(void);
	// 写DemarcationResultOfDynForceCal表
	static DWORD WriteDemarcationResultOfDynForceCal(void);
	// 写DemarcationResultOfDynForceChk.ini
	static DWORD WriteDemarcationResultOfDynForceChkFile(void);
	// 写DemarcationResultOfDynForceChk表
	static DWORD WriteDemarcationResultOfDynForceChk(void);
	// 写LineInfo表
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo表
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo.ini文件
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// 保存加载滑行数据
	static DWORD WriteDynConstLoadData(void);
	// 保存寄生功率滑行数据
	static DWORD WriteDynPLHPData(void);
	// 保存力校准数据
	static DWORD WriteDynForceCalData(void);
	// 保存力检查数据
	static DWORD WriteDynForceChkData(void);

	// 加载滑行过程控制回调函数
	static DWORD __stdcall DynConstLoadProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 寄生功率滑行过程控制回调函数
	static DWORD __stdcall DynPLHPProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 力校准过程控制回调函数
	static DWORD __stdcall DynForceCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 力检查过程控制回调函数
	static DWORD __stdcall DynForceChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
private:
	// 标定是否Pass
	static bool m_bDemarcationPass;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 上传加载滑行数据
	static bool UpConstLoad(void);
};
