/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHNOxCalibrationDlg.h
* 文件标识：
* 摘要：氮氧分析仪气体校准(NO,NO2通道)
*
* 版本：1.0.0
* 作者：Raylee
* 日期：2018-05-14
*
* 历史信息：
*
*
* 版本：1.0.0
* 日期：2018-05-14
* 作者：Raylee
* 描述：开发ing
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


// CNHNOxCalibrationDlg 对话框

class CNHNOxCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHNOxCalibrationDlg)

public:
	CNHNOxCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHNOxCalibrationDlg();

// 对话框数据
	enum { IDD = IDD_NHNOXCALDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedRadioChnCal();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

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

	// CO2实时值
	CLabel m_lbCO2;
	// NO实时值
	CLabel m_lbNO;
	// NO2实时值
	CLabel m_lbNO2;
	// NOx实时值
	CLabel m_lbNOx;

	// CO2标称值编辑框
	CNumericEdit m_edCO2NominalValue;
	// NO标称值编辑框
	CEdit m_edNONominalValue;
	// NO2标称值编辑框
	CEdit m_edNO2NominalValue;

	// 上下标文本控件
	CMathLabel m_mlb1;
	CMathLabel m_mlb2;
	CMathLabel m_mlb3;
	CMathLabel m_mlb4;
	CMathLabel m_mlb5;

	// 步骤1标签
	CStatic m_stcStep1Label;
	// 步骤2标签
	CStatic m_stcStep2Label;
	// 步骤3标签
	CStatic m_stcStep3Label;
	// 步骤4标签
	CStatic m_stcStep4Label;

	// 步骤1
	CLabel m_lbStep1;
	// 步骤2
	CLabel m_lbStep2;
	// 步骤3
	CLabel m_lbStep3;
	// 步骤4
	CLabel m_lbStep4;


private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 导入参数
	void ImportParams(void);
	// 导出气体检查实时数据
	void ExportGasCalibrationRTData(void);
	// 导出气体检查结果(内部定义)
	void ExportGasCalibrationResult(void);
	
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口end

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 气体校准流程
	void Process(void);
	// 重置校准流程
	void ResetProcess(void);
	// 获取检查气标称值输入
	// 返回0x00,标称值输入符合要求
	// 返回其它,标称值输入不符要求
	DWORD GetNominalValue(void);
	// 记录标称值到配置文件
	void SaveNominalValueToIniFile(void);
	// 清除实时显示框
	void ClearRTDisplay(void);
	// 调零流程
	void ZeroProcess(void);
	// 校准流程
	void CalibrationProcess(void);



private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// CO2测量值,单位:0.001%,放大1000倍,四舍五入
	int m_nCO2MeasuredValue;
	// NO测量值,单位:ppm
	int m_nNOMeasuredValue;
	// NO2测量值,单位:ppm
	int m_nNO2MeasuredValue;
	 
	// 标准气CO2标称值,单位:0.001%(放到1000倍)
	int m_nCO2NominalValue;
    // 标准气NO标称值,单位:ppm
	int m_nNONominalValue;
    // 标准气NO2标称值,单位:ppm
	int m_nNO2NominalValue;
	
	// 校准气标准
	// JJG668/GB18285/GB3847
	int m_nStandardGas1;
	int m_nStandardGas2;
	int m_nStandardGas3;
	int m_nStandardGas4;
	int m_nStandardGasCur;

	// 校准通道
	//0x01 校准CO2
	//0x02 校准NO
	//0x03 校准NO,CO2
	//0x04 校准NO2
	int m_nCalChn;

	// 是否查询调零状态
	bool m_bCheckZeroStatus;

	// 步骤
	int m_nStep;
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;
	// 校准结果是否Pass
	bool m_bIsCalibrationPass;

private:
	// 分析仪线程
	CAnaThread *m_pAnaThread;
};
