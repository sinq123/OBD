/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCDIWThread.h
* 文件标识：
* 摘要：测功机基本惯量滑行测试
*
* 当前版本：1.0.1
* 作者：Qin
* 完成日期：2009-11-22
*
* 历史信息：
*
* 版本：1.0.1
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
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
#include "afxwin.h"
#include "NHCDIWThread.h"

// CNHCDIWDlg 对话框

class CNHCDIWDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCDIWDlg)

public:
	CNHCDIWDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCDIWDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_DIW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStartTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonWriteDiw();

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 刷新实时数据
	void EnableRefreshRealTimeData(bool bEnable=true);
	void RefreshRealTimeData(void);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 显示基本惯量测试流程
	void DisplayDIWTestProcess(void);
	// 计算基本惯量
	void CalculateDIW(void);

private:
	//////////////////////////////////////////////////////////////////////////////////控件变量begin
	// 标题信息
	CShadeTitle m_shdTitle;
	// 提示信息显示框
	CLabel m_lbInfo;
	// 功率显示框
	CLabel m_lbP;
	// 力显示框
	CLabel m_lbF;
	// 速度显示框
	CLabel m_lbV;
	// 标签1(实时值)
	CLabel m_lb1;
	// 标签2
	CLabel m_lb2;
	// 标签3
	CLabel m_lb3;
	// 标签4
	CLabel m_lb4;
	// 滑行时间显示框(小加载力)
	CLabel m_lbLowerSlipingTime;
	// 滑行时间显示框(大加载力)
	CLabel m_lbUpperSlipingTime;
	// 实测平均加载力显示框(小加载力)
	CLabel m_lbAveLowerForce;
	// 实测平均加载力显示框(大加载力)
	CLabel m_lbAveUpperForce;
	// 基本惯量测量值显示框
	CLabel m_lbMeasuredDIW;
	CEdit m_edLowerForce;
	CEdit m_edUpperForce;
	CEdit m_edLowerVelocity;
	CEdit m_edUpperVelocity;
	//////////////////////////////////////////////////////////////////////////////////控件变量en

private:
	// 测功机定载荷控制线程
	CNHCDIWThread *m_pNHCDIWThread;
	// 标称基本惯量，单位-kg
	USHORT m_usDIW;
	// 实测基本惯量,单位:kg
	USHORT m_usMeasuredDIW;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 小加载力,单位-N
	float m_fLowerForce;
	// 大加载力,单位-N
	float m_fUpperForce;
	// 滑行区间,低速点,单位:km/h
	float m_fLowerVelocity;
	// 滑行区间,高速点,单位:km/h
	float m_fUpperVelocity;

	// 滑行时间(小加载力),单位:ms
	UINT m_unLowerSlipingTime;
	// 滑行时间(大加载力),单位:ms
	UINT m_unUpperSlipingTime;
	// 实测小加载力平均值,单位-N
	float m_fAveLowerForce;
	// 实测大加载力平均值,单位-N
	float m_fAveUpperForce;

	// 暂存Ap,Bp,Cp,变量号:39,40,41
	WORD m_wAp;
	WORD m_wBp;
	WORD m_wCp;
};
