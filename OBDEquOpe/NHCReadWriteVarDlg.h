/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCReadWriteVarDlg.h
* 文件标识：
* 描述：测功机读写变量接口
*
* 版本：1.0
* 作者：Qin
* 日期：2010-06-08
*
* 历史信息：
*
*
* 版本：1.0.2
* 日期：2015-06-10
* 作者：Sun
* 描述：马哈测功机写变量BUG修正,写入之前打开写保护
*
* 版本：1.0.1
* 日期：2010-07-07
* 作者：Cui
* 描述：按照编程规范整理本模块
*
* 版本：1.0.0
* 日期：2010-12-08
* 作者：Qin
* 描述：正式发布使用
*/

#pragma once

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
#endif

// 自用库
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include "resource.h"		// 主符号
#include "afxwin.h"

// CNHCReadWriteVarDlg 对话框

class CNHCReadWriteVarDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCReadWriteVarDlg)

public:
	CNHCReadWriteVarDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCReadWriteVarDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_READ_WRITE_VAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonReadVar();
	afx_msg void OnBnClickedButtonWriteVar();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();

private:
	CEdit m_edNumberForWrite;
	CEdit m_edNumberForRead;
	// 变量值
	CEdit m_edValue;
	CListCtrl m_lcVar;
	// 读变量进度
	CProgressCtrl m_pgRealAllVar;
	// 提示信息
	CLabel m_lbInfo;
	// 标题信息
	CShadeTitle m_shdTitle;

private:
	void InitCtrls(void);
	void SetDlgFont(void);
	
private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);

private:
	// 测功机控制线程
	CDynThread * m_pDynThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
};
