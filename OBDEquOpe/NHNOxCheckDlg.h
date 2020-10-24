/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHNOxCheckDlg.h
* 文件标识：
* 摘要：氮氧分析仪气体检查(NO,NO2通道)
*
* 当前版本：1.0.0
* 作者：Raylee
* 完成日期：2018-05-16
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2018-05-16
* 作者：Raylee
* 描述：开发ing
*
*/
#pragma once
#include "resource.h"
// 自定义
#include "..\NHDetCommModu\NHDetCommModu.h"
//#include "..\NHDef\NHStructDef.h"

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


// CNHNOxCheckDlg 对话框
class CNHNOxCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHNOxCheckDlg)

public:
	CNHNOxCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHNOxCheckDlg();

// 对话框数据
	enum { IDD = IDD_NHNOXCHECKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedRadioChnChk();
	afx_msg void OnBnClickedRadioCheckType();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////////////////// 数据交换接口begin
	// 导入分析仪气体检查参数
	void ImportGasCheckParams(void);
	// 导出气体检查结果(行标定义)
	void ExportEquCalChkInfo(void);
	// 导出标定日志(内部定义)
	void ExportDemarcationLog(void);
	// 导出气体检查结果(内部定义)
	void ExportGasCheckResult(void);
	// 导出气体检查实时数据
	void ExportGasCheckRTData(void);
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
	void CalculateResultCO2(void);
	void CalculateResultNO(void);
	void CalculateResultNO2(void);
	// 显示结果
	void ShowResultCO2(void);
	void ShowResultNO(void);
	void ShowResultNO2(void);
	void ShowJudgement(void);
	// 限值设置
	void SetLimit(void);
	// 计算响应时间
	void CalculateRespTime(void);

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

	// CO2实时值
	CLabel m_lbCO2;
	// NO实时值
	CLabel m_lbNO;
	// NO2实时值
	CLabel m_lbNO2;
	// NOx实时值
	CLabel m_lbNOx;

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

	// CO2标称值编辑框
	CNumericEdit m_edCO2NominalValue;
	// NO标称值编辑框
	CEdit m_edNONominalValue;
	// NO2标称值编辑框
	CEdit m_edNO2NominalValue;
		
	// CO2测量值显示框
	CLabel m_lbCO2MeasuredValue;
	// NO测量值显示框
	CLabel m_lbNOMeasuredValue;
	// NO2测量值显示框
	CLabel m_lbNO2MeasuredValue;
	
	// CO2标称值显示框
	CLabel m_lbCO2NominalValue;
	// NO标称值显示框
	CLabel m_lbNONominalValue;
	// NO2标称值显示框
	CLabel m_lbNO2NominalValue;

	// CO2绝对误差显示框
	CLabel m_lbCO2AE;
	// NO绝对误差显示框
	CLabel m_lbNOAE;
	// NO2绝对误差显示框
	CLabel m_lbNO2AE;
		
	// CO2相对误差显示框
	CLabel m_lbCO2RE;
	// NO相对误差显示框
	CLabel m_lbNORE;
	// NO2相对误差显示框
	CLabel m_lbNO2RE;
	
	// CO2判定显示框
	CLabel m_lbCO2Judgement;
	// NO判定显示框
	CLabel m_lbNOJudgement;
	// NO2判定显示框
	CLabel m_lbNO2Judgement;
	
	// CO2响应时间
	CLabel m_lbCO2RespTime;
	// NO响应时间
	CLabel m_lbNORespTime;
	// NO2响应时间
	CLabel m_lbNO2RespTime;

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

	// 检查通道
	// 1：CO2
	// 2：NO
	// 3：NO2
	// 4：CO2,NO
	int m_nChannel;
	// 标准气浓度
	// 1：低浓度
	// 2：中低浓度
	// 3：中高浓度
	// 4：高浓度
	int m_nCheckType;
	
	// 实时数据链表结构
	struct RTData
	{
		// 时间序号
		int nTime;
		// CO2值,单位:0.01%
		float fCO2;
		// NO值,单位:ppm
		short sNO;
		// NO2值,单位:ppm
		short sNO2;
		// 流量
		USHORT usFlux;
		// 时间
		SYSTEMTIME st;

		RTData(){ZeroMemory(this,sizeof(RTData));}
	};	
	// 声明链表
	CList<RTData, RTData> m_lsRTDataOfNOx;

private:
    // 标准气CO2标称值,单位:0.01%(放到100倍)
	int m_nCO2NominalValue;
	// 标准气NO标称值,单位:ppm
	int m_nNONominalValue;
	// 标准气NO2标称值,单位:ppm
	int m_nNO2NominalValue;

private:	
	// CO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AELimit;
	// CO2相对误差限值,单位:%
	int m_nCO2RELimit;
	// NO绝对误差限值,单位:ppm
	int m_nNOAELimit;
	// NO相对误差限值,单位:%
	int m_nNORELimit;
	// NO2绝对误差限值,单位:ppm
	int m_nNO2AELimit;
	// NO相对误差限值,单位:%
	int m_nNO2RELimit;
	
	// 响应时间要求
	float m_fCO2T90Limit;
	float m_fCO2T10Limit;
	float m_fNOT90Limit;
	float m_fNOT10Limit;
	float m_fNO2T90Limit;
	float m_fNO2T10Limit;

private:
	// CO2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2MeasuredValue;
	// CO2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AE;
	// CO2绝对误差是否通过
	bool m_bIsCO2AEPass;
	// CO2相对误差,单位:%,放大100倍,四舍五入
	int m_nCO2RE;
	// CO2相对误差是否通过
	bool m_bIsCO2REPass;
	// CO2通道是否通过
	bool m_bIsCO2Pass;

	// NO测量值,单位:ppm
	int m_nNOMeasuredValue;
	// NO绝对误差,单位:ppm
	int m_nNOAE;
	// NO绝对误差是否通过
	bool m_bIsNOAEPass;
	// NO相对误差,单位:%,放大100倍,四舍五入
	int m_nNORE;
	// NO相对误差是否通过
	bool m_bIsNOREPass;
	// NO通道是否通过
	bool m_bIsNOPass;

	// NO2测量值,单位:ppm
	int m_nNO2MeasuredValue;
	// NO2绝对误差,单位:ppm
	int m_nNO2AE;
	// NO2绝对误差是否通过
	bool m_bIsNO2AEPass;
	// NO2相对误差,单位:%,放大100倍,四舍五入
	int m_nNO2RE;
	// NO2相对误差是否通过
	bool m_bIsNO2REPass;
	// NO2通道是否通过
	bool m_bIsNO2Pass;
	
	// 响应时间
	float m_fCO2RespTime;
	float m_fNORespTime;
	float m_fNO2RespTime;

	// 检查是否通过
	bool m_bIsCheckPass;
	
private:
	short m_sRandomErrorHC;
	short m_sRandomErrorNO;
	short m_sRandomErrorNO2;
	short m_sRandomErrorNOX;
	float m_fRandomErrorCO;
	float m_fRandomErrorCO2;
	float m_fRandomErrorO2;

	void RandomError(short& sHC, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx);
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;

	void Precomputation(int& nMeasuredValue, const int& nNominalValue, const int& nAELimit);
};
