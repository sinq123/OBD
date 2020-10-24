/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHAGasResponseTime.h
* 文件标识：
* 摘要：分析仪其他响应时间
*
* 当前版本：1.0.0
* 作者：Raylee
* 完成日期：2019-05-06
*
* 历史信息：
*
* 版本：1.0.0
* 时间：2019-05-06
* 作者：Raylee
* 描述：开发ing
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


// CNHAGasResponseTimeDlg 对话框
class CNHAGasResponseTimeDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHAGasResponseTimeDlg)

public:
	CNHAGasResponseTimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHAGasResponseTimeDlg();

// 对话框数据
	enum { IDD = IDD_NHA_GAS_RESPONSE_TIME };

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
	// 计算结果
	void CalculateResult(void);
	// 显示结果
	void ShowResult(void);
	// 限值设置
	void SetLimit(void);
	// 计算响应时间
	void CalculateRespTimeT90(void);
	void CalculateRespTimeT10(void);

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

	//// 上下标文本控件
	//CMathLabel m_mlb1;
	//// 上下标文本控件
	//CMathLabel m_mlb2;
	//// 上下标文本控件
	//CMathLabel m_mlb3;
	//// 上下标文本控件
	//CMathLabel m_mlb4;
	//// 上下标文本控件
	//CMathLabel m_mlb5;
	//// 上下标文本控件
	//CMathLabel m_mlb6;
	//// 上下标文本控件
	//CMathLabel m_mlb8;
	//// 上下标文本控件
	//CMathLabel m_mlb12;
	//// 上下标文本控件
	//CMathLabel m_mlb13;
	//// 上下标文本控件
	//CMathLabel m_mlb14;
	//// 上下标文本控件
	//CMathLabel m_mlb15;
	//// 上下标文本控件
	//CMathLabel m_mlb16;
	//// 上下标文本控件
	//CMathLabel m_mlb17;
	//// 上下标文本控件
	//CMathLabel m_mlb18;
	//CMathLabel m_mlb19;

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
	// NO2实时值
	CLabel m_lbNO2;
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
	// 步骤5
	CLabel m_lbStep5;

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

	// HC判定显示框
	CLabel m_lbHCJudgement;
	// CO判定显示框
	CLabel m_lbCOJudgement;
	// CO2判定显示框
	CLabel m_lbCO2Judgement;
	// NO判定显示框
	CLabel m_lbNOJudgement;
	// NO2判定显示框
	CLabel m_lbNO2Judgement;
	// O2判定显示框
	CLabel m_lbO2Judgement;

	CLabel m_lbHCT90Limit;
	CLabel m_lbCOT90Limit;
	CLabel m_lbCO2T90Limit;
	CLabel m_lbNOT90Limit;
	CLabel m_lbNO2T90Limit;
	CLabel m_lbO2T90Limit;

	CLabel m_lbHCT10Limit;
	CLabel m_lbCOT10Limit;
	CLabel m_lbCO2T10Limit;
	CLabel m_lbNOT10Limit;
	CLabel m_lbNO2T10Limit;
	CLabel m_lbO2T10Limit;

	// HC响应时间
	CLabel m_lbHCT90RespTime;
	// CO响应时间
	CLabel m_lbCOT90RespTime;
	// CO2响应时间
	CLabel m_lbCO2T90RespTime;
	// NO响应时间
	CLabel m_lbNOT90RespTime;
	// NO2响应时间
	CLabel m_lbNO2T90RespTime;
	// O2响应时间
	CLabel m_lbO2T90RespTime;

	// HC响应时间
	CLabel m_lbHCT10RespTime;
	// CO响应时间
	CLabel m_lbCOT10RespTime;
	// CO2响应时间
	CLabel m_lbCO2T10RespTime;
	// NO响应时间
	CLabel m_lbNOT10RespTime;
	// NO2响应时间
	CLabel m_lbNO2T10RespTime;
	// O2响应时间
	CLabel m_lbO2T10RespTime;
	
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
		// NO2值,单位:ppm
		short sNO2;
		// Pef值
		float fPef;
		// 时间
		SYSTEMTIME st;

		RTData(){ZeroMemory(this,sizeof(RTData));}
	};	
	// 声明链表
	std::vector<RTData> m_vtRTDataT90;
	std::vector<RTData> m_vtRTDataT10;

private:
	// 响应时间要求
	float m_fHCT90Limit;
	float m_fHCT10Limit;
	float m_fCOT90Limit;
	float m_fCOT10Limit;	
	float m_fCO2T90Limit;
	float m_fCO2T10Limit;
	float m_fNOT90Limit;
	float m_fNOT10Limit;
	float m_fNO2T90Limit;
	float m_fNO2T10Limit;
	float m_fO2T90Limit;
	float m_fO2T10Limit;
	
	// HC通道是否通过
	bool m_bIsHCPass;
	// CO通道是否通过
	bool m_bIsCOPass;
	// CO2通道是否通过
	bool m_bIsCO2Pass;
	// NO通道是否通过
	bool m_bIsNOPass;
	// NO2通道是否通过
	bool m_bIsNO2Pass;
	// O2通道是否通过
	bool m_bIsO2Pass;
	
	// 响应时间
	float m_fHCT90RespTime;
	float m_fCOT90RespTime;
	float m_fCO2T90RespTime;
	float m_fNOT90RespTime;
	float m_fNO2T90RespTime;
	float m_fO2T90RespTime;

	float m_fHCT10RespTime;
	float m_fCOT10RespTime;
	float m_fCO2T10RespTime;
	float m_fNOT10RespTime;
	float m_fNO2T10RespTime;
	float m_fO2T10RespTime;

	// 检查是否通过
	bool m_bIsCheckPass;

};
