/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FlowmeterPressureCalibrationDlg.h
* 文件标识：
* 摘要：Sensor流量计VMAS压力校准
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-12-11
*
* 历史信息：
*
* 版本：1.0.8
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.7
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.6
* 完成日期：2010-05-27
* 作者：Raylee
* 描述：使用新的标定数据导出格式
*
* 当前版本：1.0.5
* 完成日期：2010-05-22
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.4
* 完成日期：2010-05-14
* 作者：Raylee
* 描述：写标定数据改用回调函数
*
* 当前版本：1.0.3
* 时间：2010-02-02
* 作者：Cui
* 描述：1)修正分析仪线程Open函数使用，将连接到流量计实参bConnectToFlowmeter置为true
*
* 当前版本：1.0.2
* 时间：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序

* 当前版本：1.0.1
* 完成日期：2009-12-22
* 作者：Cui
* 描述：1)完善ExportResult函数
*       2)添加基本操作(开关风机)
*
* 当前版本：1.0.0
* 完成日期：2009-12-11
* 作者：Cui
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

// CFlowmeterPressureCalibrationDlg 对话框

class CFlowmeterPressureCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterPressureCalibrationDlg)

public:
	CFlowmeterPressureCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowmeterPressureCalibrationDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOWMETER_PRESSURE_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonStart();
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

	// 压力实时值显示框
	CLabel m_lbPressure;

	// 步骤1标签
	CStatic m_stcStep1Label;
	// 步骤2标签
	CStatic m_stcStep2Label;

	// 步骤1
	CLabel m_lbStep1;
	// 步骤2
	CLabel m_lbStep2;

	// 压力校准值编辑框,kPa
	CNumericEdit m_nePressureCalibrationValue;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 导出实时数据
	void ExportFloPresCalRTData(void);
	// 导出结果
	void ExportFloPresCalResult(void);

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 校准流程
	void Process(void);
	// 重置校准流程
	void ResetProcess(void);
	// 获取压力校准值输入
	// 返回0x00,标称值输入符合要求
	// 返回其它,标称值输入不符要求
	DWORD GetCalibrationValue(void);
	// 清除实时显示框
	void ClearRTDisplay(void);
	// 校准流程
	// 返回0x00,校准成功
	// 返回其它,校准失败
	DWORD CalibrationProcess(void);
	// 刷新实时数据
	// (in)bRefresh:true,刷新实时数据,false,停止刷新实时数据
	void RefreshRTData(bool bRefresh = true);
	// 显示实时数据
	void DisplayRTData(void);
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
	bool m_bIsCalibrationPass;
	
private:
	// 压力校准值
	float m_fPressureCalibrationValue;

private:
	// 分析仪线程
	CAnaThread *m_pFloThread;
};
