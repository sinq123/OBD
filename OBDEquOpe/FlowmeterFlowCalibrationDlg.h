/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FlowmeterFlowCalibrationDlg.h
* 文件标识：
* 摘要：Sensor流量计VMAS流量校准(需要标准流量计)
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-12-22
*
* 历史信息：
*
* 版本：1.0.10
* 日期：2011-06-24
* 作者：Sjming
* 描述：1）实时值添加零偏移值的约束
*		2）通讯正常时，不屏蔽开关风机按钮，低流量时自动关风机
*
* 版本：1.0.9
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.8
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.7
* 完成日期：2010-05-27
* 作者：Raylee
* 描述：使用新的标定数据导出格式
*
* 当前版本：1.0.6
* 完成日期：2010-05-22
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.5
* 完成日期：2010-05-14
* 作者：Raylee
* 描述：写标定数据改用回调函数
*
* 当前版本：1.0.4
* 时间：2010-02-02
* 作者：Cui
* 描述：1)修正分析仪线程Open函数使用，将连接到流量计实参bConnectToFlowmeter置为true
*
* 当前版本：1.0.3
* 时间：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序
*
* 当前版本：1.0.2
* 完成日期：2009-12-23
* 作者：Cui
* 描述：1)对校准流程进行小修改
*       2)整理代码
*
* 当前版本：1.0.1
* 完成日期：2009-12-22
* 作者：Cui
* 描述：添加基本操作(开关风机等)
*
* 当前版本：1.0.0
* 完成日期：2009-12-22
* 作者：Cui
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


// CFlowmeterFlowCalibrationDlg 对话框

class CFlowmeterFlowCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterFlowCalibrationDlg)

public:
	CFlowmeterFlowCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowmeterFlowCalibrationDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOWMETER_FLOW_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonCalibrationHighFlux();
	afx_msg void OnBnClickedButtonCalibrationLowFlux();
	afx_msg void OnBnClickedButtonFanOn();
	afx_msg void OnBnClickedButtonFanOff();

private:
	// 标题栏
	CShadeTitle m_lbTitle;
	// 信息提示栏
	CLabel m_lbInfo;
	// 标题1
	CLabel m_lbLabel1;
	// 标题2
	CLabel m_lbLabel2;
	// 标题3
	CLabel m_lbLabel3;
	// 标题4
	CLabel m_lbLabel4;

	// 流量实时值显示框
	CLabel m_lbFlux;

	// 步骤1标签
	CStatic m_stcStep1Label;
	// 步骤2标签
	CStatic m_stcStep2Label;
	// 步骤3标签
	CStatic m_stcStep3Label;
	// 步骤4标签
	CStatic m_stcStep4Label;
	// 步骤5标签
	CStatic m_stcStep5Label;

	// 步骤1
	CLabel m_lbStep1;
	// 步骤2
	CLabel m_lbStep2;
	// 步骤3
	CLabel m_lbStep3;
	// 步骤4
	CLabel m_lbStep4;
	// 步骤5
	CLabel m_lbStep5;

	// 高流量标准值编辑框
	CNumericEdit m_neStandardValueHighFlux;
	// 低流量标准值编辑框
	CNumericEdit m_neStandardValueLowFlux;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	//显示控件
	void ShowEntrance(void);
	void ShowDomestic(void);

private:
	// 导入参数
	void ImportParams(void);
	// 导出实时数据
	void ExportFloFluxCalRTData(void);
	// 导出结果
	void ExportFloFluxCalResult(void);

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 刷新实时数据
	// (in)bRefresh:true,刷新实时数据,false,停止刷新实时数据
	void RefreshRTData(bool bRefresh = true);
	// 显示实时数据
	void DisplayRTData(void);
	// 流程
	void Process(void);
	// 重置流程
	void ResetProcess(void);
	// 获取高流量标准值输入
	// 返回0x00,标准值输入符合要求
	// 返回其它,标准值输入不符要求
	DWORD GetStandardValueOfHighFlux(void);
	// 获取低流量标准值输入
	// 返回0x00,标准值输入符合要求
	// 返回其它,标准值输入不符要求
	DWORD GetStandardValueOfLowFlux(void);
	// 是否允许基本操作(开关风机等)
	void EnableBasicOperation(bool bEnable=true);

	//////////////////////////////////////////////////////////////// 过程控制回调函数接口begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口end

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 步骤
	int m_nStep;
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;
	// 校准结果是否Pass
	bool m_bIsPass;
	// 高流量标准值,单位:L/s
	float m_fStandardValueOfHighFlux;
	// 低流量标准值,单位:L/s
	float m_fStandardValueOfLowFlux;
	// 最大零点偏移值，单位:L/s
	float m_fMaximumZeroOffsetValue;
	// 南华Sensor
	bool m_bDomestic;
private:
	// 分析仪线程
	CAnaThread *m_pFloThread;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
