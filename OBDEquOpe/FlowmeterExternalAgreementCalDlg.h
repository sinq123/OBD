/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FlowmeterExternalCalibrationDlg.h.h
* 文件标识：
* 摘要：外部协议流量计校准
*
* 当前版本：
* 作者：
* 完成日期：
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


// CFlowmeterExternalCalDlg 对话框

class CFlowmeterExternalCalDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFlowmeterExternalCalDlg)

public:
	CFlowmeterExternalCalDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowmeterExternalCalDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOWMETER_EXTERNAL_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFanOn();
	afx_msg void OnBnClickedButtonFanOff();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonCalibrationTp();
	afx_msg void OnBnClickedButtonCalibrationPress();
	afx_msg void OnBnClickedButtonCalibrationO2();
	afx_msg void OnBnClickedButtonCalibrationHighFlux();
	afx_msg void OnBnClickedButtonCalibrationLowFlux();
	afx_msg void OnBnClickedButtonCalibrationSingleFlux();
	afx_msg void OnBnClickedButtonRecovery();
	afx_msg void OnBnClickedRadioHigh();
	afx_msg void OnBnClickedRadioMiddle();
	afx_msg void OnBnClickedRadioLow();
	afx_msg void OnBnClickedRadioZero();
;
private:
	// 标题栏
	CShadeTitle m_lbTitle;
	// 信息提示栏
	CLabel m_lbInfo;
	// 标准流量显示框
	CLabel m_lbStd0Flux;
	CLabel m_lbStd20Flux;
	CLabel m_lbActFlux;
	// 温度显示框
	CLabel m_lbTemp;
	// 压力显示框
	CLabel m_lbPres;
	// 稀释氧显示框
	CLabel m_lbUDilO2;
	// 稀释氧选项
	CButton m_rdoHigh;
	CButton m_rdoMiddle;
	CButton m_rdoLow;
	// 稀释氧校准值编辑框,℃
	CNumericEdit m_neHighO2Value;
	CNumericEdit m_neMiddleO2Value;
	CNumericEdit m_neLowO2Value;
	CNumericEdit m_neZeroO2Value;
	// 温度校准值编辑框,℃
	CNumericEdit m_neTPCalibrationValue;
	// 压力校准值编辑框,kPa
	CNumericEdit m_nePressureCalibrationValue;
	// 高点流量标准值编辑框
	CNumericEdit m_neStandardValueHighFlux;
	// 低点流量标准值编辑框
	CNumericEdit m_neStandardValueLowFlux;
	// 单点流量标准值编辑框
	CNumericEdit m_neStandardValueSingleFlux;

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
	// 刷新实时数据
	// (in)bRefresh:true,刷新实时数据,false,停止刷新实时数据
	void RefreshRTData(bool bRefresh = true);

	// 导出温度校准结果
	void ExportFloTPCalResult(void);
	// 获取温度校准值输入
	DWORD GetTpCalibrationValue(void);
	// 温度校准流程
	DWORD TpCalibrationProcess(void);
	
	// 导出压力校准结果
	void ExportFloPresCalResult(void);
	// 获取压力校准值输入
	DWORD GetPresCalibrationValue(void);
	// 压力校准流程
	DWORD PresCalibrationProcess(void);

	// 导出稀释氧校准结果
	void ExportFloDilO2CalResult(void);
	// 校准流程
	DWORD DilO2CalibrationProcess(void);
	DWORD CalibrationProcess_High(void);
	DWORD CalibrationProcess_Middle(void);
	DWORD CalibrationProcess_Low(void);

	// 导入参数
	void ImportParams(void);
	// 导出结果
	void ExportFloFluxCalResult(CString strName,float fValue,BOOL bPass);
	// 获取高点流量标准值输入
	DWORD GetStandardValueOfHighFlux(void);
	// 获取低点流量标准值输入
	DWORD GetStandardValueOfLowFlux(void);
	// 获取单点流量标准值输入
	DWORD GetStandardValueOfSingleFlux(void);

	// 恢复出厂设置
	DWORD RestoreFactorySettings(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 温度校准值
	float m_fTPCalValue;
	// 压力校准值
	float m_fPressureCalibrationValue;
	// 温度校准结果是否Pass
	bool m_bIsTpCalibrationPass;
	// 压力校准结果是否Pass
	bool m_bIsPresCalibrationPass;

	//校准点(1:高 2:中 3:低 4:零)
	int m_nPoint;
	// 稀释氧校准结果是否Pass
	bool m_bIsO2CalibrationPass;
	
	// 流量校准结果是否Pass
	bool m_bIsFlowCalibrationPass;
	// 高点流量标准值,单位:L/s
	float m_fStandardValueOfHighFlux;
	// 单点流量标准值,单位:L/s
	float m_fStandardValueOfSingleFlux;
	// 低点流量标准值,单位:L/s
	float m_fStandardValueOfLowFlux;
	// 最大零点偏移值，单位:L/s
	float m_fMaximumZeroOffsetValue;

	// 分析仪线程
	CFlowThread *m_pFloThread;
};
