/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHAMPGasCalibrationDlg.h
* 文件标识：
* 摘要：气体校准(不包括气体检查)
*
* 当前版本：1.0.10
* 作者：Hyh
* 完成日期：2010-07-05
*
* 历史信息：
*
* 版本：1.0.13
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 当前版本：1.0.12
* 完成日期：2010-09-29
* 作者：Raylee
* 描述：1、修改界面的字体大小
*		2、添加CO2和O2控件的下标显示
*
* 当前版本：1.0.11
* 完成日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.10
* 完成日期：2010-07-05
* 作者：Hyh
* 描述：兼容分析仪协议3.6版本,添加分析仪版本标志位,根据不同版本进行分析仪调零
*
* 当前版本：1.0.9
* 完成日期：2010-07-01
* 作者：Ming
* 描述：去掉ExportEquCalChkInfo和ExportDemarcationLog两函数的声明和定义
*
* 当前版本：1.0.8
* 完成日期：2010-05-26
* 作者：Raylee
* 描述：只保留导出校准结果数据
*
* 当前版本：1.0.7
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：1、回调提示校准结束改为校准完成
*		2、回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.6
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：改用过程控制回调函数，可响应多种状态
*
* 当前版本：1.0.5
* 完成日期：2010-05-12
* 作者：Raylee
* 描述：写校准检查数据改用回调函数
*
* 当前版本：1.0.4
* 时间：2010-04-09
* 作者：Raylee
* 描述：改导出标定日志和检查结果数据
*
* 当前版本：1.0.3
* 时间：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序
*
* 当前版本：1.0.2
* 时间：2010-01-05
* 作者：Cui
* 描述：1)添加分析仪连接方式：用于识别是否与流量计连接，如果是，需要执行切换到分析仪通讯模式操作
*       2)修改界面字体“检查气标称值设置”为“校准气标称值设置”
*
* 当前版本：1.0.1
* 完成日期：2009-12-08
* 作者：Cui
* 描述：更新分析仪线程
*
* 当前版本：1.0.0
* 完成日期：2009-11-26
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


// CNHAMPGasCalibrationDlg 对话框

class CNHAMPGasCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHAMPGasCalibrationDlg)

public:
	CNHAMPGasCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHAMPGasCalibrationDlg();

// 对话框数据
	enum { IDD = IDD_NHA_MP_CAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedRadioStandard();
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
	// NO实时值
	CLabel m_lbNO;
	// NO2实时值
	CLabel m_lbNO2;
	// PEF实时值
	CLabel m_lbPEF;

	// C3H8标称值编辑框
	CEdit m_edC3H8NominalValue;
	// CO标称值编辑框
	CNumericEdit m_edCONominalValue;
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
	CMathLabel m_mlb6;
	CMathLabel m_mlb7;
	CMathLabel m_mlb8;
	CMathLabel m_mlb9;
	CMathLabel m_mlb10;

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
	// PEF测量值,放大1000倍,四舍五入
	int m_nPEFMeasuredValue;
	// HC测量值,单位:ppm
	int m_nHCMeasuredValue;
	// CO测量值,单位:0.001%,放大1000倍,四舍五入
	int m_nCOMeasuredValue;
	// CO2测量值,单位:0.001%,放大1000倍,四舍五入
	int m_nCO2MeasuredValue;
	// O2测量值,单位:0.001%,放大1000倍,四舍五入
	int m_nO2MeasuredValue;
	// NO测量值,单位:ppm
	int m_nNOMeasuredValue;
	// NO2测量值,单位:ppm
	int m_nNO2MeasuredValue;

    // 标准气C3H8标称值,单位:ppm
	int m_nC3H8NominalValue;
    // 标准气CO标称值,单位:0.001%(放到1000倍)
	int m_nCONominalValue;
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
	//0x01 校准HC,CO,CO2
	//0x02 校准NO
	//0x04 校准NO2
	//0x03 校准HC,CO,CO2,NO
	//0x05 校准HC,CO,CO2,NO2
	//0x06 校准NO,NO2
	//0x07 校准HC,CO,CO2,NO,NO2
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
