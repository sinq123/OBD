/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHA503XGasCheckDlg2.h
* 文件标识：
* 摘要：用分析仪统一协议和普通分析仪气体检查流程进行气体检查(不包括气体校准)
        由NHA503XGasCheckDlg改进,使用外来限值进行判定,只导出过程数据和结果数据
*
* 当前版本：1.0.4
* 作者：Ming
* 完成日期：2010-10-30
*
* 历史信息：
*
* 版本：1.0.6
* 日期：2012-10-12
* 作者：Raylee
* 描述：添加南华NHA-501A设备支持
*
* 版本：1.0.5
* 日期：2012-10-11
* 作者：Raylee
* 描述：添加佛分FGA4100，福立FLA501，鸣泉MQW511设备支持
*
* 版本：1.0.4
* 日期：2010-10-30
* 作者：Ming
* 描述：各气体计算相对误差时,添加其标准值的合理性约束
*
* 版本：1.0.3
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 当前版本：1.0.2
* 完成日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.1
* 完成日期：2010-07-05
* 作者：Hyh
* 描述：兼容分析仪协议3.6版本,添加分析仪版本标志位,根据不同版本进行分析仪调零
*
* 当前版本：1.0.0
* 时间：2010-07-01
* 作者：Ming
* 描述：正式发布使用
*/
#pragma once
#include "resource.h"
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


// CNHA503XGasCheckDlg2 对话框
class CNHA503XGasCheckDlg2 : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA503XGasCheckDlg2)

public:
	CNHA503XGasCheckDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHA503XGasCheckDlg2();

// 对话框数据
	enum { IDD = IDD_NHA503XGASCHECKDLG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////////////////// 数据交换接口begin
	// 导入分析仪气体检查参数
	void ImportGasCheckParams(void);
	// 导入外部限值参数
	void ImportGasCheckLimit(void);
	// 导出气体检查实时数据
	void ExportGasCheckRTData(void);
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
	// 上下标文本控件
	CMathLabel m_mlb11;
	// 上下标文本控件
	CMathLabel m_mlb12;

	// HC实时值
	CLabel m_lbHC;
	// C3H8实时值
	CLabel m_lbC3H8;
	// CO实时值
	CLabel m_lbCO;
	// CO2实时值
	CLabel m_lbCO2;
	// NO实时值
	CLabel m_lbNO;
	// O2实时值
	CLabel m_lbO2;
	// PEF实时值
	CLabel m_lbPEF;

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

	// C3H8标称值编辑框
	CEdit m_edC3H8NominalValue;
	// CO标称值编辑框
	CNumericEdit m_edCONominalValue;
	// CO2标称值编辑框
	CNumericEdit m_edCO2NominalValue;
	// NO标称值编辑框
	CEdit m_edNONominalValue;

	// PEF测量值显示框
	CLabel m_lbPEFMeasuredValue;
	
	// HC测量值显示框
	CLabel m_lbHCMeasuredValue;
	// CO测量值显示框
	CLabel m_lbCOMeasuredValue;
	// CO2测量值显示框
	CLabel m_lbCO2MeasuredValue;
	// NO测量值显示框
	CLabel m_lbNOMeasuredValue;

	// HC标称值显示框
	CLabel m_lbHCNominalValue;
	// CO标称值显示框
	CLabel m_lbCONominalValue;
	// CO2标称值显示框
	CLabel m_lbCO2NominalValue;
	// NO标称值显示框
	CLabel m_lbNONominalValue;

	// HC绝对误差显示框
	CLabel m_lbHCAE;
	// CO绝对误差显示框
	CLabel m_lbCOAE;
	// CO2绝对误差显示框
	CLabel m_lbCO2AE;
	// NO绝对误差显示框
	CLabel m_lbNOAE;

	
	// HC相对误差显示框
	CLabel m_lbHCRE;
	// CO相对误差显示框
	CLabel m_lbCORE;
	// CO2相对误差显示框
	CLabel m_lbCO2RE;
	// NO相对误差显示框
	CLabel m_lbNORE;
	
	// HC判定显示框
	CLabel m_lbHCJudgement;
	// CO判定显示框
	CLabel m_lbCOJudgement;
	// CO2判定显示框
	CLabel m_lbCO2Judgement;
	// NO判定显示框
	CLabel m_lbNOJudgement;
	
	// 裁决显示框
	CLabel m_lbDetermination;

private:
	// 分析仪线程
	CAnaThread *m_pAnaThread;
	
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 分析仪协议版本
	int m_nAnaProtocolVersion;

private:
	// 步骤
	int m_nStep;
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
	// 实时数据链表结构
	struct RTData
	{
		// 时间序号
		int nTime;
		// HC值,单位:ppm
		short sHC;
		// CO值,单位:0.01%
		float fCO;
		// CO2值,单位:0.01%
		float fCO2;
		// O2值,单位:%
		float fO2;
		// NO值,单位:ppm
		short sNO;
		// 转速,单位:r/min
		USHORT usRev;
		// 油温,单位:℃
		float fOT;
		// 流量
		USHORT usFlux;
		// Lambda
		float fLambda;
		// Pef
		float fPef;
		// 环境温度,单位:℃
		float fET;
		// 相对湿度,单位:%
		float fRH;
		// 大气压力,单位:kPa
		float fAP;

		RTData(){ZeroMemory(this,sizeof(RTData));}
	};	
	// 声明链表
	CList<RTData, RTData> m_lsRTDataOfAnaGasChk;

private:
    // 标准气C3H8标称值,单位:ppm
	int m_nC3H8NominalValue;
    // 标准气CO标称值,单位:0.01%(放到100倍)
	int m_nCONominalValue;
    // 标准气CO2标称值,单位:0.01%(放到100倍)
	int m_nCO2NominalValue;
	// 标准气NO标称值,单位:0.01%(放到100倍)
	int m_nNONominalValue;
	// 标准气HC标称值,单位:ppm,
	// 由C3H8标称值*PEF得到
	int m_nHCNominalValue;

private:
	// HC绝对误差限值,单位:ppm
	int m_nHCAELimit;
	// CO绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nCOAELimit;
	// CO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AELimit;
	// NO绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nNOAELimit;

	// HC相对误差限值,单位:%
	int m_nHCRELimit;
	// CO相对误差限值,单位:%
	int m_nCORELimit;
	// CO2相对误差限值,单位:%
	int m_nCO2RELimit;
	// NO相对误差限值,单位:%
	int m_nNORELimit;

private:
	// PEF测量值,放大1000倍,四舍五入
	int m_nPEFMeasuredValue;
	// HC测量值,单位:ppm
	int m_nHCMeasuredValue;
	// CO测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCOMeasuredValue;
	// CO2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2MeasuredValue;
	// NO测量值,单位:0.01%,放大100倍,四舍五入
	int m_nNOMeasuredValue;
	// O2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nO2MeasuredValue;

	// HC绝对误差,单位:ppm
	int m_nHCAE;
	// CO绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCOAE;
	// CO2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AE;
	// NO绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nNOAE;

	// HC绝对误差是否通过
	bool m_bIsHCAEPass;
	// CO绝对误差是否通过
	bool m_bIsCOAEPass;
	// CO2绝对误差是否通过
	bool m_bIsCO2AEPass;
	// NO绝对误差是否通过
	bool m_bIsNOAEPass;

	// HC相对误差,单位:%
	int m_nHCRE;
	// CO相对误差,单位:%,放大100倍,四舍五入
	int m_nCORE;
	// CO2相对误差,单位:%,放大100倍,四舍五入
	int m_nCO2RE;
	// NO相对误差,单位:%,放大100倍,四舍五入
	int m_nNORE;

	// HC相对误差是否通过
	bool m_bIsHCREPass;
	// CO相对误差是否通过
	bool m_bIsCOREPass;
	// CO2相对误差是否通过
	bool m_bIsCO2REPass;
	// NO相对误差是否通过
	bool m_bIsNOREPass;

	// HC通道是否通过
	bool m_bIsHCPass;
	// CO通道是否通过
	bool m_bIsCOPass;
	// CO2通道是否通过
	bool m_bIsCO2Pass;
	// NO通道是否通过
	bool m_bIsNOPass;

	// 检查是否通过
	bool m_bIsCheckPass;

public:
	afx_msg void OnStnClickedStaticNoRe();
};
