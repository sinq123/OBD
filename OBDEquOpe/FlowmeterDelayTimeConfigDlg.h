/*
* Copyright (c) 2011,佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：FlowmeterDelayTimeConfigDlg.h
* 文件标识：
* 摘要：Sensor流量计通道延时设置
*
* 当前版本：1.0
* 作者：Ming
* 完成日期：2009-12-22
*
* 历史信息：
*
* 当前版本：1.0.1
* 完成日期：2011-03-011
* 作者：Ming
* 描述：添加恢复默认设置功能
*
* 当前版本：1.0.0
* 完成日期：2011-03-03
* 作者：Ming
* 描述：正式发布使用
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


// CFlowmeterDelayTimeConfigDlg 对话框

class CFlowmeterDelayTimeConfigDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterDelayTimeConfigDlg)

public:
	CFlowmeterDelayTimeConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowmeterDelayTimeConfigDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOWMETER_DELAYTIME_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedBtnHccoco2Read();
	afx_msg void OnBnClickedBtnO2Read();
	afx_msg void OnBnClickedBtnNoxRead();
	afx_msg void OnBnClickedBtnHccoco2Write();
	afx_msg void OnBnClickedBtnO2Write();
	afx_msg void OnBnClickedBtnNoxWrite();
	afx_msg void OnBnClickedBtnDefaultConfig();

private:
	// 标题栏
	CShadeTitle m_lbTitle;
	// 信息提示栏
	CLabel m_lbInfo;

	// 流量实时值显示框
	CLabel m_lbFlux;

	// HC/CO/CO2校准值编辑框
	CNumericEdit m_neHCCOCO2Value;
	// O2校准值编辑框
	CNumericEdit m_neO2Value;
	// NOx校准值编辑框
	CNumericEdit m_neNOxValue;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 是否允许操作
	void EnableOperation(bool bEnable=true);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 分析仪线程
	CAnaThread *m_pFloThread;
};
