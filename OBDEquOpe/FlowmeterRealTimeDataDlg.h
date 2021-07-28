/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FlowmeterRealTimeDataDlg.h
* 文件标识：
* 摘要：流量计实时数据显示
*
* 当前版本：1.0
* 作者：Lihy
* 完成日期：2014-07-18
*
* 历史信息：
*
* 当前版本：1.0.0
* 完成日期：2014-07-18
* 作者：Lihy
* 描述：正式发布使用
*
*
*/

#pragma once

// 自定义
#include "..\NHDetCommModu\NHDetCommModu.h"

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

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
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


// CFlowmeterRealTimeDataDlg 对话框

class CFlowmeterRealTimeDataDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterRealTimeDataDlg)

public:
	CFlowmeterRealTimeDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowmeterRealTimeDataDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOWMETER_REALTIME_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	// 标题栏
	CShadeTitle m_lbTitle;
	// 信息提示栏
	CLabel m_lbInfo;

	// 标准流量
	CLabel m_lbStd0Flux;
	CLabel m_lbStd20Flux;
	CLabel m_lbActFlux;
	// 温度
	CLabel m_lbTemp;
	// 压力
	CLabel m_lbPres;
	// 稀释氧
	CLabel m_lbUDilO2;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 是否允许基本操作(开关风机等)
	void EnableBasicOperation(bool bEnable=true);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 分析仪线程
	CAnaThread *m_pFloThread;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFanOn();
	afx_msg void OnBnClickedButtonFanOff();
	afx_msg void OnBnClickedButtonQuit();
};
