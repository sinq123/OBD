/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHA503XGasCheckDlg.h
* 文件标识：
* 摘要：用分析仪统一协议和普通分析仪气体检查流程进行气体气体检查(不包括气体气体校准)
        由403X改进，添加NO通道
*
* 当前版本：1.0.16
* 作者：Ming
* 完成日期：2010-10-30
*
* 历史信息：
*
* 版本：1.0.18
* 日期：2012-10-12
* 作者：Raylee
* 描述：添加南华NHA-501A设备支持
*
* 版本：1.0.17
* 日期：2012-10-11
* 作者：Raylee
* 描述：添加佛分FGA4100，福立FLA501，鸣泉MQW511设备支持
*
* 版本：1.0.16
* 日期：2010-10-30
* 作者：Ming
* 描述：各气体计算相对误差时,添加其标准值的合理性约束
*
* 版本：1.0.15
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 当前版本：1.0.14
* 完成日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.13
* 完成日期：2010-07-05
* 作者：Hyh
* 描述：兼容分析仪协议3.6版本,添加分析仪版本标志位,根据不同版本进行分析仪调零
*
* 当前版本：1.0.12
* 日期：2010-07-01
* 作者：Ming
* 描述：1)去掉先前的外部限值导入情况
*       2)修正界面CO2和O2下标显示
*	    3)添加三个限值设置函数,初始化限值显示为双怠速限值
*
* 当前版本：1.0.11
* 日期：2010-06-29
* 作者：Ming
* 描述：1)写过程数据导出函数
*		2)添加外部限值导入情况,通过判断App_Data中是否存在LimitOfAnaGasCheck.ini文件,确定限值获取方式,计算结果按照双怠速类型计算
*
* 当前版本：1.0.10
* 完成日期：2010-06-01
* 作者：Raylee
* 描述：标准上DIS不用判断NO，这里就按照我们的说明书上设置误差值
*
* 当前版本：1.0.9
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：1、回调提示检查结束改为检查完成
*		2、回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.8
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.7
* 完成日期：2010-05-12
* 作者：Raylee
* 描述：写校准检查数据改用回调函数
*
* 当前版本：1.0.6
* 时间：2010-04-08
* 作者：Raylee
* 描述：1、改导出标定日志和检查结果数据
*		2、中途停止时，为503添加关闭气阀命令
*
* 当前版本：1.0.5
* 时间：2010-04-02
* 作者：Raylee
* 描述：1、为503添加打开/关闭检查气阀命令
*		2、按照DIS、ASM、VMAS三种检测方法不同的仪器精度要求，分别做气体检查判断
*		3、修正NO限值的单位为ppm
*
* 当前版本：1.0.4
* 时间：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序
*
* 当前版本：1.0.3
* 时间：2010-01-05
* 作者：Cui
* 描述：1)添加分析仪连接方式：用于识别是否与流量计连接，如果是，需要执行切换到分析仪通讯模式操作
*
* 当前版本：1.0.2
* 时间：2009-12-23
* 作者：Cui
* 描述：进入程序，正在测试通讯时，屏蔽“退出”按钮，即改成与4分检查同一模式
*
* 当前版本：1.0.1
* 时间：2009-12-08
* 作者：Cui
* 描述：更新分析仪线程

* 当前版本：1.0.0
* 时间：2009-12-08
* 作者：Cui
* 描述：正式发布使用
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


// CNHA503XGasCheckDlg 对话框
class CNHA503XGasCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA503XGasCheckDlg)

public:
	CNHA503XGasCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHA503XGasCheckDlg();

// 对话框数据
	enum { IDD = IDD_NHA503XGASCHECKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	//afx_msg void OnBnClickedRadioTestType();
	afx_msg void OnBnClickedRadioChn();
	afx_msg void OnBnClickedRadioCheckType();
	DECLARE_MESSAGE_MAP()

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
	//void CalculateResult_DIS(void);
	//void CalculateResult_ASM(void);
	void CalculateResult_VMAS(void);
	// 显示结果
	//void ShowResult_DIS(void);	
	//void ShowResult_ASM(void);
	void ShowResult_VMAS(void);
	// 限值设置
	//void SetLimit_DIS(void);
	//void SetLimit_ASM(void);
	void SetLimit_VMAS(void);
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
	CMathLabel m_mlb8;
	// 上下标文本控件
	CMathLabel m_mlb12;
	// 上下标文本控件
	CMathLabel m_mlb13;
	// 上下标文本控件
	CMathLabel m_mlb14;
	// 上下标文本控件
	CMathLabel m_mlb15;
	// 上下标文本控件
	CMathLabel m_mlb16;
	// 上下标文本控件
	CMathLabel m_mlb17;
	// 上下标文本控件
	CMathLabel m_mlb18;
	CMathLabel m_mlb19;

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
	// NO2标称值编辑框
	CEdit m_edNO2NominalValue;
	// O2标称值编辑框
	CNumericEdit m_edO2NominalValue;

	// PEF测量值显示框
	CLabel m_lbPEFMeasuredValue;
	
	// HC测量值显示框
	CLabel m_lbHCMeasuredValue;
	// CO测量值显示框
	CLabel m_lbCOMeasuredValue;
	// CO2测量值显示框
	CLabel m_lbCO2MeasuredValue;
	// O2测量值显示框
	CLabel m_lbO2MeasuredValue;
	// NO测量值显示框
	CLabel m_lbNOMeasuredValue;
	// NO2测量值显示框
	CLabel m_lbNO2MeasuredValue;

	// HC标称值显示框
	CLabel m_lbHCNominalValue;
	// CO标称值显示框
	CLabel m_lbCONominalValue;
	// CO2标称值显示框
	CLabel m_lbCO2NominalValue;
	// O2标称值显示框
	CLabel m_lbO2NominalValue;
	// NO标称值显示框
	CLabel m_lbNONominalValue;
	// NO2标称值显示框
	CLabel m_lbNO2NominalValue;

	// HC绝对误差显示框
	CLabel m_lbHCAE;
	// CO绝对误差显示框
	CLabel m_lbCOAE;
	// CO2绝对误差显示框
	CLabel m_lbCO2AE;
	// O2绝对误差显示框
	CLabel m_lbO2AE;
	// NO绝对误差显示框
	CLabel m_lbNOAE;
	// NO2绝对误差显示框
	CLabel m_lbNO2AE;
		
	// HC相对误差显示框
	CLabel m_lbHCRE;
	// CO相对误差显示框
	CLabel m_lbCORE;
	// CO2相对误差显示框
	CLabel m_lbCO2RE;
	// O2相对误差显示框
	CLabel m_lbO2RE;
	// NO相对误差显示框
	CLabel m_lbNORE;
	// NO2相对误差显示框
	CLabel m_lbNO2RE;
	
	// HC判定显示框
	CLabel m_lbHCJudgement;
	// CO判定显示框
	CLabel m_lbCOJudgement;
	// CO2判定显示框
	CLabel m_lbCO2Judgement;
	// O2判定显示框
	CLabel m_lbO2Judgement;
	// NO判定显示框
	CLabel m_lbNOJudgement;
	// NO2判定显示框
	CLabel m_lbNO2Judgement;

	// HC响应时间
	CLabel m_lbHCRespTime;
	// CO响应时间
	CLabel m_lbCORespTime;
	// CO2响应时间
	CLabel m_lbCO2RespTime;
	// O2响应时间
	CLabel m_lbO2RespTime;
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
	// 通道
	// 1：HC_CO_CO2
	// 2：NO
	// 3：NO2
	// 4：HC_CO_CO2_NO
	int m_nChannel;
	// 标准气浓度
	// 1：低浓度
	// 2：中低浓度
	// 3：中高浓度
	// 4：高浓度
	int m_nCheckType;

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
		// 时间
		SYSTEMTIME st;

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
	// 标准气NO2标称值,单位:0.01%(放到100倍)
	int m_nNO2NominalValue;
	// 标准气HC标称值,单位:ppm,
	// 由C3H8标称值*PEF得到
	int m_nHCNominalValue;
    // 标准气O2标称值,单位:0.01%(放到100倍)
	int m_nO2NominalValue;

private:
	// HC绝对误差限值,单位:ppm
	int m_nHCAELimit;
	// CO绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nCOAELimit;
	// CO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AELimit;
	// NO绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nNOAELimit;
	// NO2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nNO2AELimit;
	// O2绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nO2AELimit;

	// HC相对误差限值,单位:%
	int m_nHCRELimit;
	// CO相对误差限值,单位:%
	int m_nCORELimit;
	// CO2相对误差限值,单位:%
	int m_nCO2RELimit;
	// NO相对误差限值,单位:%
	int m_nNORELimit;
	// NO2相对误差限值,单位:%
	int m_nNO2RELimit;
	// O2相对误差限值,单位:%
	int m_nO2RELimit;
	
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
	// NO2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nNO2MeasuredValue;
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
	// NO2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nNO2AE;
	// O2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nO2AE;

	// HC绝对误差是否通过
	bool m_bIsHCAEPass;
	// CO绝对误差是否通过
	bool m_bIsCOAEPass;
	// CO2绝对误差是否通过
	bool m_bIsCO2AEPass;
	// NO绝对误差是否通过
	bool m_bIsNOAEPass;
	// NO2绝对误差是否通过
	bool m_bIsNO2AEPass;
	// O2绝对误差是否通过
	bool m_bIsO2AEPass;

	// HC相对误差,单位:%
	int m_nHCRE;
	// CO相对误差,单位:%,放大100倍,四舍五入
	int m_nCORE;
	// CO2相对误差,单位:%,放大100倍,四舍五入
	int m_nCO2RE;
	// NO相对误差,单位:%,放大100倍,四舍五入
	int m_nNORE;
	// NO2相对误差,单位:%,放大100倍,四舍五入
	int m_nNO2RE;
	// O2相对误差,单位:%,放大100倍,四舍五入
	int m_nO2RE;

	// HC相对误差是否通过
	bool m_bIsHCREPass;
	// CO相对误差是否通过
	bool m_bIsCOREPass;
	// CO2相对误差是否通过
	bool m_bIsCO2REPass;
	// NO相对误差是否通过
	bool m_bIsNOREPass;
	// NO2相对误差是否通过
	bool m_bIsNO2REPass;
	// O2相对误差是否通过
	bool m_bIsO2REPass;

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
	float m_fHCRespTime;
	float m_fCORespTime;
	float m_fCO2RespTime;
	float m_fNORespTime;
	float m_fNO2RespTime;
	float m_fO2RespTime;

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
	float m_fRandomErrorPef;

	void RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx);
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;
};
