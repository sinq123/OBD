
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

// CFloDlg 对话框

class CFloDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFloDlg)

public:
	CFloDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloDlg();

// 对话框数据
	enum { IDD = IDD_FLO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonDilo2Cal();
	afx_msg void OnBnClickedButtonPresCal();
	afx_msg void OnBnClickedButtonFluxChk();
	afx_msg void OnBnClickedButtonFluxCal();
	afx_msg void OnBnClickedButtonDelayTimeCfg();
	afx_msg void OnBnClickedButtonFloCal();

private:
	CShadeTitle m_lbTitle;

private:
	// 初始化控件
	void InitCtrls();
	// 设置对话框字体
	void SetDlgFont(void);

private:
	static DWORD UpdateLineInfoFile(const CString strItem, const CString strItemValue);
	// 写EquCalChkInfo表
	static DWORD WriteEquCalChkInfo(void);
	// 写EquCalChkInfo.ini
	static DWORD WriteEquCalChkInfoFile(int nDemarcationItem);
	// 写DemarcationLog表
	static DWORD WriteDemarcationLog(void);
	// 写DemarcationLog.ini
	static DWORD WriteDemarcationLogFile(int nDemarcationItem);
	// 写DemarcationResultOfFloFluxCal表
	static DWORD WriteDemarcationResultOfFloFluxCal(void);
	// 写ResultOfFloFluxCal.ini
	static DWORD WriteDemarcationResultOfFloFluxCalFile(void);
	// 写DemarcationResultOfFloFluxChk表
	static DWORD WriteDemarcationResultOfFloFluxChk(void);
	// 写ResultOfFloFluxChk.ini
	static DWORD WriteDemarcationResultOfFloFluxChkFile(void);
	// 写DemarcationResultOfFloDilO2Cal表
	static DWORD WriteDemarcationResultOfFloDilO2Cal(void);
	// 写ResultOfFloDilO2Cal.ini
	static DWORD WriteDemarcationResultOfFloDilO2CalFile(void);
	// 写DemarcationResultOfFloPresCal表
	static DWORD WriteDemarcationResultOfFloPresCal(void);
	// 写ResultOfFloPresCal.ini
	static DWORD WriteDemarcationResultOfFloPresCalFile(void);
	// 写LineInfo表
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo表
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo.ini文件
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// 保存稀释氧校准数据
	static DWORD WriteFloDilO2CalData(void);
	// 保存压力校准数据
	static DWORD WriteFloPresCalData(void);
	// 保存流量检查数据
	static DWORD WriteFloFluxChkData(void);
	// 保存流量校准数据
	static DWORD WriteFloFluxCalData(void);
	// 流量校准过程控制回调函数
	static DWORD __stdcall FloFluxCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 流量检查过程控制回调函数
	static DWORD __stdcall FloFluxChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 压力校准过程控制回调函数
	static DWORD __stdcall FloPresCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 稀释氧校准过程控制回调函数
	static DWORD __stdcall FloDilO2CalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
private:
	// 标定是否Pass
	static bool m_bDemarcationPass;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

};
