
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

// CNOxDlg 对话框

class CNOxDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNOxDlg)

public:
	CNOxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNOxDlg();

// 对话框数据
	enum { IDD = IDD_NOX_DLG };

private:
	afx_msg void OnBnClickedButtonGasCal();
	afx_msg void OnBnClickedButtonGasChk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
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
	// 更新LineInfo.ini文件
	// (in)strItem,要更新的项目
	// (int)strItemValue,项目值
	DWORD UpdateLineInfoFile(const CString strItem, const CString strItemValue);
	// 写EquCalChkInfo表
	static DWORD WriteEquCalChkInfo(void);
	// 写LineInfo表
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo表
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// 写LineInfo.ini文件
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// 保存分析仪气体检查数据
	static DWORD WriteAnaGasChkData(void);
	// 保存分析仪气体校准数据
	static DWORD WriteAnaGasCalData(void);
	// 写DemarcationLog表
	static DWORD WriteDemarcationLog(void);
	// 写DemarcationResultOfAnaGasCal表
	static DWORD WriteDemarcationResultOfAnaGasCal(void);
	// 写DemarcationResultOfAnaGasChk表
	static DWORD WriteDemarcationResultOfAnaGasChk(void);
	// 气体校准过程控制回调函数
	static DWORD __stdcall AnaCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// 气体校准过程控制回调函数
	static DWORD __stdcall AnaChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
private:
	// 标定是否Pass
	static bool m_bDemarcationPass;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;


private:
	// 上传检查（低标气）记录
	static bool UpGasCheck(const CStringW& strJKID = L"13W05");
	// 2.15上传单点检查（零气）记录13W06
	static bool UpGasCheck13W06(void);
	// 2.16上传单点检查（高标气）记录13W07
	static bool UpGasCheck13W07(void);
	// 上传检查过程记录
	static bool UpEqulChk(const int& njclx);
};
