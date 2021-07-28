/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCCommonOperationDlg.h
* 文件标识：
* 描述：测功机常用操作
*
* 版本：1.0
* 作者：Qin
* 日期：2010-06-17
*
* 历史信息：
*
* 版本：1.0.2
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.1
* 日期：2010-07-07
* 作者：Cui
* 描述：1)按照编程规范整理本模块
*       2)改进状态显示
*
* 版本：1.0.0
* 日期：2010-06-17
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
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include "resource.h"		// 主符号

// CNHCCommonOperationDlg 对话框

class CNHCCommonOperationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCCommonOperationDlg)

public:
	CNHCCommonOperationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCCommonOperationDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_COMMON_OPERATION };

protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonViewState();
	afx_msg void OnBnClickedButtonQuit();

private:
	// 提示信息
	CLabel m_lbInfo;
	// 标题
	CShadeTitle m_shdTitle;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);

private:
	// 测功机线程
	CDynThread *m_pDynThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
};
