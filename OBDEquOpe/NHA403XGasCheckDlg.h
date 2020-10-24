/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHA403XGasCheckDlg.h
* 文件标识：
* 摘要：用分析仪统一协议和普通分析仪气体检查流程进行气体气体检查(不包括气体气体校准)
        不能用于503系列分析仪,因为503分析仪调零,通入检查气流程与普通分析仪不一样
        (分析仪统一协议主要由503分析仪协议衍生,但执行流程有些不一样)
*
* 当前版本：1.0.15
* 作者：Ming
* 完成日期：2010-10-30
*
* 历史信息：
*
* 版本：1.0.15
* 日期：2010-10-30
* 作者：Ming
* 描述：各气体计算相对误差时,添加其标准值的合理性约束
*
* 版本：1.0.14
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 当前版本：1.0.13
* 完成日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.12
* 完成日期：2010-07-05
* 作者：Hyh
* 描述：兼容分析仪协议3.6版本,添加分析仪版本标志位,根据不同版本进行分析仪调零
*
* 当前版本：1.0.11
* 日期：2010-07-01
* 作者：Ming
* 描述：1、修正界面CO2和O2下标显示
*
* 当前版本：1.0.10
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：1、回调提示检查结束改为检查完成
*		2、回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.9
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.8
* 完成日期：2010-05-12
* 作者：Raylee
* 描述：写校准检查数据改用回调函数
*
*
* 当前版本：1.0.7
* 时间：2010-04-08
* 作者：Raylee
* 描述：改导出标定日志和检查结果数据
*
* 当前版本：1.0.6
* 时间：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序
*
* 当前版本：1.0.5
* 时间：2010-01-05
* 作者：Cui
* 描述：1)添加分析仪连接方式：用于识别是否与流量计连接，如果是，需要执行切换到分析仪通讯模式操作
*
* 当前版本：1.0.4
* 时间：2009-12-08
* 作者：Cui
* 描述：更新分析仪线程
*
* 当前版本：1.0.3
* 时间：2009-11-23
* 作者：Raylee
* 描述：改函数ExportGasCheckDetail，把标定数据保存到ini，	
*		调整afx_msg void OnSysCommand(UINT nID, LPARAM lParam);位置
*
* 当前版本：1.0.2
* 时间：2009-11-20
* 作者：Raylee
* 描述：改函数ExportGasCheckResult，把标定数据保存在Reserved1字段
*
* 当前版本：1.0.1
* 时间：2009-09-21
* 作者：Cui
* 描述：将界面上的ppm改为10-6，并且实现上下标的视觉效果
*
* 当前版本：1.0.0
* 完成日期：2009-08-15
* 作者：Cui
* 描述：将“通道检查”名词改为“气体检查”，并正式使用
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
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
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

// CNHA403XGasCheckDlg 对话框

class CNHA403XGasCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA403XGasCheckDlg)

public:
	CNHA403XGasCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHA403XGasCheckDlg();

// 对话框数据
	enum { IDD = IDD_NHA403XGASCHECKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

private:
	//////////////////////////////////////////////////////////////// 数据交换接口begin
	// 导入分析仪气体检查参数
	void ImportGasCheckParams(void);
	// 导出气体检查结果(行标定义)
	void ExportEquCalChkInfo(void);
	// 导出气体检查实时数据
	void ExportGasCheckRTData(void);
	// 导出标定日志(内部定义)
	void ExportDemarcationLog(void);
	// 导出气体检查结果(内部定义)
	void ExportGasCheckResult(void);
	//////////////////////////////////////////////////////////////// 数据交换接口end

	//////////////////////////////////////////////////////////////// 过程控制回调函数接口begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口end

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 气体检查流程
	void GasCheckProcess(void);
	// 重置检查流程
	void ResetProcess(void); 
	// 清除实时显示框
	void ClearRTDisplay(void);
	// 获取检查气标称值输入
	// 返回0x00,标称值输入符合要求
	// 返回其它,标称值输入不符要求
	DWORD GetNominalValue(void);
	// 记录标称值到配置文件
	void SaveNominalValueToIniFile(void);
	// 计算结果
	void CalculateResult(void);
	// 显示结果
	void ShowResult(void);

private:
	// 分析仪线程
	CAnaThread * m_pAnaThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 标题栏
	CShadeTitle m_lbHead;
	// 大标题
	CLabel m_lbInfo;
	// 实时值标题
	CLabel m_lbLabel1;
	// 检查值标题
	CLabel m_lbLabel2;
	// 误差标题
	CLabel m_lbLabel3;
	// 检查步骤标题
	CLabel m_lbLabel4;

	// HC实时值
	CLabel m_lbHC;
	// C3H8实时值
	CLabel m_lbC3H8;
	// CO实时值
	CLabel m_lbCO;
	// CO2实时值
	CLabel m_lbCO2;
	// O2实时值
	CLabel m_lbO2;
	// PEF实时值
	CLabel m_lbPEF;

	// C3H8标称值编辑框
	CEdit m_edC3H8NominalValue;
	// CO标称值编辑框
	CNumericEdit m_edCONominalValue;
	// CO2标称值编辑框
	CNumericEdit m_edCO2NominalValue;

	// 步骤1
	CLabel m_lbStep1;
	// 步骤2
	CLabel m_lbStep2;
	// 步骤3
	CLabel m_lbStep3;
	// 步骤4
	CLabel m_lbStep4;

	// 步骤1标签
	CStatic m_stcStep1Label;
	// 步骤2标签
	CStatic m_stcStep2Label;
	// 步骤3标签
	CStatic m_stcStep3Label;
	// 步骤4标签
	CStatic m_stcStep4Label;

	// PEF测量值显示框
	CLabel m_lbPEFMeasuredValue;
	
	// HC测量值显示框
	CLabel m_lbHCMeasuredValue;
	// CO测量值显示框
	CLabel m_lbCOMeasuredValue;
	// CO2测量值显示框
	CLabel m_lbCO2MeasuredValue;

	// HC标称值显示框
	CLabel m_lbHCNominalValue;
	// CO标称值显示框
	CLabel m_lbCONominalValue;
	// CO2标称值显示框
	CLabel m_lbCO2NominalValue;

	// HC绝对误差显示框
	CLabel m_lbHCAE;
	// CO绝对误差显示框
	CLabel m_lbCOAE;
	// CO2绝对误差显示框
	CLabel m_lbCO2AE;

	// HC相对误差显示框
	CLabel m_lbHCRE;
	// CO相对误差显示框
	CLabel m_lbCORE;
	// CO2相对误差显示框
	CLabel m_lbCO2RE;

	// HC判定显示框
	CLabel m_lbHCJudgement;
	// CO判定显示框
	CLabel m_lbCOJudgement;
	// CO2判定显示框
	CLabel m_lbCO2Judgement;

	// 裁决显示框
	CLabel m_lbDetermination;

	// 上下标文本控件
	CMathLabel m_mlb1;
	// 上下标文本控件
	CMathLabel m_mlb2;
	// 上下标文本控件
	CMathLabel m_mlb3;
	// 上下标文本控件
	CMathLabel m_mlb4;
	// 上下标文本控件
	CMathLabel m_mlb5;
	// 上下标文本控件
	CMathLabel m_mlb6;
	// 上下标文本控件
	CMathLabel m_mlb7;
	// 上下标文本控件
	CMathLabel m_mlb8;
	// 上下标文本控件
	CMathLabel m_mlb9;
	// 上下标文本控件
	CMathLabel m_mlb10;

private:
	// 分析仪协议版本
	int m_nAnaProtocolVersion;

private:
	// 步骤
	int m_nStep;
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
    // 标准气C3H8标称值,单位:ppm
	int m_nC3H8NominalValue;
    // 标准气CO标称值,单位:0.01%(放到100倍)
	int m_nCONominalValue;
    // 标准气CO2标称值,单位:0.01%(放到100倍)
	int m_nCO2NominalValue;
	// 标准气HC标称值,单位:ppm,
	// 由C3H8标称值*PEF得到
	int m_nHCNominalValue;

private:
	// HC绝对误差限值,单位:ppm
	const int m_nHCAELimit;
	// CO绝对误差限值,单位:0.01%,放大100倍,四舍五入
	const int m_nCOAELimit;
	// CO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	const int m_nCO2AELimit;

	// HC相对误差限值,单位:%
	const int m_nHCRELimit;
	// CO相对误差限值,单位:%
	const int m_nCORELimit;
	// CO2相对误差限值,单位:%
	const int m_nCO2RELimit;

private:
	// PEF测量值,放大1000倍,四舍五入
	int m_nPEFMeasuredValue;
	// HC测量值,单位:ppm
	int m_nHCMeasuredValue;
	// CO测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCOMeasuredValue;
	// CO2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2MeasuredValue;
	// O2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nO2MeasuredValue;

	// HC绝对误差,单位:ppm
	int m_nHCAE;
	// CO绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCOAE;
	// CO2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AE;

	// HC绝对误差是否通过
	bool m_bIsHCAEPass;
	// CO绝对误差是否通过
	bool m_bIsCOAEPass;
	// CO2绝对误差是否通过
	bool m_bIsCO2AEPass;

	// HC相对误差,单位:%
	int m_nHCRE;
	// CO相对误差,单位:%,放大100倍,四舍五入
	int m_nCORE;
	// CO2相对误差,单位:%,放大100倍,四舍五入
	int m_nCO2RE;

	// HC相对误差是否通过
	bool m_bIsHCREPass;
	// CO相对误差是否通过
	bool m_bIsCOREPass;
	// CO2相对误差是否通过
	bool m_bIsCO2REPass;

	// HC通道是否通过
	bool m_bIsHCPass;
	// CO通道是否通过
	bool m_bIsCOPass;
	// CO2通道是否通过
	bool m_bIsCO2Pass;

	// 检查是否通过
	bool m_bIsCheckPass;

};
