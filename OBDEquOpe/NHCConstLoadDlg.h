/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCConstLoadDlg.h
* 文件标识：
* 摘要：测功机加载滑行测试
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-12-22
*
* 历史信息：
*
* 版本：1.0.18
* 日期：2014-07-10
* 作者：Lihy
* 描述：修改写入ResultOfDynConstLoad.ini文件时，相对误差限值错误写入到绝对误差限值字段中的小错误
*
* 版本：1.0.17
* 日期：2012-09-28
* 作者：wuyq
* 描述：判定测功机升举升器的速度判定条件改为小于等于10e-6
*
* 版本：1.0.16
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 当前版本：1.0.15
* 完成日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.14
* 完成日期：2010-09-26
* 作者：Ming
* 描述：1)添加实时计算寄生功率函数和CalculatePLHP()
*       2)实时数据导出函数增加寄生功率和指示功率值的导出
*
* 版本：1.0.13
* 日期：2010-07-07
* 作者：Cui
* 描述：添加测功机通讯协议选择功能
*
* 当前版本：1.0.12
* 完成日期：2010-07-07
* 作者：Ming
* 描述：1)添加记录实时数据函数AddRTData()
*       2)写实时数据导出函数
*
* 当前版本：1.0.11
* 完成日期：2010-07-05
* 作者：Ming
* 描述：到处结果时,添加导出项IsOperationFinishedAtLeaseOnce,以表示操作是否完成过至少一次.
*
* 当前版本：1.0.10
* 完成日期：2010-06-25
* 作者：Qin
* 描述：修正CalculateCCTD函数计算理论滑行时间错误
*
* 当前版本：1.0.9
* 完成日期：2010-06-02
* 作者：Raylee
* 描述：修正导出功率标准值
*
* 当前版本：1.0.8
* 完成日期：2010-05-27
* 作者：Raylee
* 描述：只保留导出校准结果数据
*
* 当前版本：1.0.7
* 完成日期：2010-05-22
* 作者：Raylee
* 描述：注释导出EquCalChkInfo.ini的代码
*
* 当前版本：1.0.6
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.5
* 完成日期：2010-05-20
* 作者：Raylee
* 描述：导出ini标定结果相关信息改在外部合并，内部不再依赖LineInfo、UserInfo、StationInfo等
*
* 当前版本：1.0.4
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.3
* 完成日期：2010-05-14
* 作者：Raylee
* 描述：1、写标定数据改用回调函数
*		2、改导出标定日志和检查结果数据
*
* 版本：1.0.3
* 日期：2010-04-10
* 作者：Qin
* 描述：1）将滑行状态的复选框改成和系统检查一样的图标表示方式
*       2）修正了原来做完加载滑行完成后写人日志的字段Dynmometer改正为Dynamometer

* 版本：1.0.2
* 日期：2010-03-15
* 作者：Cui
* 描述：1）将标题栏由“加载滑行”改为“测功机加载滑行”
*       2）将“开始测试”按钮和“停止测试”按钮改为“开始”按钮和“停止”按钮
*
* 版本：1.0.1
* 日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 版本：1.0.0
* 日期：2009-12-22
* 作者：Cui
* 描述：正式发布使用
*
*
*/

#pragma once

#include "resource.h"		// 主符号
#include "NHCConstLoadThread.h"

// 自定义
#include "..\NHDetCommModu\NHDetCommModu.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// 自用库
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


// CNHCConstLoadDlg 对话框

class CNHCConstLoadDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCConstLoadDlg)

public:
	CNHCConstLoadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCConstLoadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_CONST_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStartLowerRangeTest();
	afx_msg void OnBnClickedButtonStartHigherRangeTest();
	afx_msg void OnBnClickedButtonStartCustomRangeTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedRadioAsmRange();
	afx_msg void OnBnClickedRadioVmasRange();
	afx_msg void OnBnClickedRadioLugdownRange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////控件变量begin
	// 标题栏
	CShadeTitle m_lbTitle;
	// 提示信息
	CLabel m_lbInfo;

	// 标签
	CLabel m_lb1;
	CLabel m_lb2;
	CLabel m_lb3;
	CLabel m_lb4;

	// 功率显示框
	CLabel m_lbP;
	// 速度显示框
	CLabel m_lbV;

	// 基本惯量显示框，单位:kg
	CLabel m_lbDIW;

	// 总结果显示框
	CLabel m_lbTotalResult;
	// 低速段结果显示框
	CLabel m_lbLowerRangeResult;
	// 高速段结果显示框
	CLabel m_lbHigherRangeResult;
	// 自定义区间段结果显示框
	CLabel m_lbCustomRangeResult;

	CButton m_rdoAsmRange;
	CButton m_rdoVmasRange;
	CButton m_rdoLugdownRange;
	CEdit m_edLowerRangePower;
	CEdit m_edHigherRangePower;
	CEdit m_edCustomRangePower;
	//CButton m_chkLowerRange;
	//CButton m_chkHigherRange;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////控件变量end

private:
	//////////////////////////////////////////////////////////////// 数据交换接口begin
	// 导入参数
	void ImportParams(void);
	
	// 导出加载滑行检查结果(行标定义)
	//void ExportEquCalChkInfo(void);
	// 导出加载滑行检查实时数据
	void ExportDynConstLoadRTData(void);
	// 导出加载滑行检查记录(内部定义)
	//void ExportDemarcationLog(void);
	// 导出加载滑行检查结果(内部定义)
	void ExportDynConstLoadResult(void);
	// 导出上传的检查结果(上传定义)
	void ExportUpDynConstLoadResult(void);
	//////////////////////////////////////////////////////////////// 数据交换接口end

	//////////////////////////////////////////////////////////////// 过程控制回调函数接口begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口end

private:
	// 加载滑行测试完成
	afx_msg LRESULT OnConstLoadTestFinished(WPARAM wParam, LPARAM lParam);

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 是否允许刷新数据
	void EnableRefreshRealTimeData(bool bEnable=true);
	// 设置Icon状态
	void SetStepIcon(UINT nStaticID, int nIconType);
	// 刷新数据
	void RefreshRealTimeData(void);
	// 是否允许测功机基本操作,基本包括升举升器、降举升器、开电机、关电机
	void EnableBasicOperation(bool bEnable=true);
	// 恢复控件状态
	void RestoreCtrls(void);
	// 显示定载荷测试流程
	void DisplayConstantLoadTestProcess(void);
	// 计算CCTD
	// (return)理论滑行时间,单位-ms
	static UINT CalculateCCTD(const USHORT usDIW, const float fUpperVelocity, const float fLowerVelocity, const float fConstLoad);
	// 清除低速段结果
	void ClearLowerRangeResult(void);
	// 清除高速段结果
	void ClearHigherRangeResult(void);
	// 清除自定义段结果
	void ClearCustomRangeResult(void);
	// 显示测试结果
	void DisplayTestResult(void);
	// 计算寄生功率
	float CalculatePLHP(const float fV);

private:
	// 测功机控制线程
	CNHCConstLoadThread *m_pNHCConstLoadThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 实时数据链表结构
	struct RTData
	{
		// 时间序号
		int nTime;
		// 滚筒线速度,单位:km/h
		float fVelocity;
		// 功率,单位:kW
		float fPower;
		// 力,单位:N
		float fForce;
		// 寄生功率,单位：kW
		float fPLHP;
		// 指示功率,单位：kW
		float fIHP;

		RTData(){ZeroMemory(this, sizeof(RTData));}
	};	
	// 声明链表
	CList<RTData, RTData> m_lsRTDataOfDynConstLoad;
	// 添加实时数据
	void AddRTData(void);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////测试结果begin
	// 基本惯量，单位-kg
	USHORT m_usDIW;
	// 低速段加载功率，单位：kW
	float m_fLowerRangePower;
	// 高速段加载功率，单位：kW
	float m_fHigherRangePower;
	// 自定义段加载功率，单位：kW
	float m_fCustomRangePower;
	// CCDT，加载滑行时间计算值，低速段，单位-毫秒
	UINT m_unLowerRangeCCDT;
	// ACCT，加载滑行时间实测值，低速段，单位-毫秒
	UINT m_unLowerRangeACDT;
	// CCDT，加载滑行时间计算值，高速段，单位-毫秒
	UINT m_unHigherRangeCCDT;
	// ACCT，加载滑行时间实测值，高速段，单位-毫秒
	UINT m_unHigherRangeACDT;
	// CCDT，加载滑行时间计算值，自定义段，单位-毫秒
	UINT m_unCustomRangeCCDT;
	// ACCT，加载滑行时间实测值，自定义段，单位-毫秒
	UINT m_unCustomRangeACDT;
	// 相对误差限值，单位-%，低速段（放大100倍，例如，7%，则录入700）
	UINT m_unLowerRangeRELimitX100;
	// 相对误差限值，单位-%，高速段（放大100倍，例如，7%，则录入700）
	UINT m_unHigherRangeRELimitX100;
	// 相对误差限值，单位-%，自定义段（放大100倍，例如，7%，则录入700）
	UINT m_unCustomRangeRELimitX100;
	// 实测值相对误差，单位-%，低速段，（放大100倍）
	int m_nLowerRangeREX100;
	// 实测值相对误差，单位-%，高速段，（放大100倍）
	int m_nHigherRangeREX100;
	// 实测值相对误差，单位-%，自定义段，（放大100倍）
	int m_nCustomRangeREX100;
	// 区间测试结果，低速段
	bool m_bIsLowerRangePass;
	// 区间测试结果，高速段
	bool m_bIsHigherRangePass;
	// 区间测试结果，自定义段
	bool m_bIsCustomRangePass;
	// 至少完成过一次测试，低速段
	bool m_bLowerRangeTestFinishedAtLeastOnce;
	// 至少完成过一次测试，高速段
	bool m_bHigherRangeTestFinishedAtLeastOnce;
	// 至少完成过一次测试，自定义段
	bool m_bCustomRangeTestFinishedAtLeastOnce;
	// 操作是否至少完成1次(高速段和低速段都至少完成过一次)
	bool m_bIsOperationFinishedAtLeaseOnce;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////测试结果end

private:
	// 当前使用的低速段滑行区间的低速点，单位：km/h
	float m_fLowerVelocityOfLowerRange;
	// 当前使用的低速段滑行区间的高速点，单位：km/h
	float m_fUpperVelocityOfLowerRange;
	// 当前使用的高速段滑行区间的低速点，单位：km/h
	float m_fLowerVelocityOfHigherRange;
	// 当前使用的高速段滑行区间的高速点，单位：km/h
	float m_fUpperVelocityOfHigherRange;
	// 当前使用的自定义段滑行区间的低速点，单位：km/h
	float m_fLowerVelocityOfCustomRange;
	// 当前使用的自定义段滑行区间的高速点，单位：km/h
	float m_fUpperVelocityOfCustomRange;
	// 低速区间加载滑行步骤是否正在进行.     false:进行中,true:未进行
	bool  m_bLowerRangeLoadConstStep;
	// 高速区间加载滑行步骤是否正在进行.     false:进行中,true:未进行
	bool  m_bHigherRangeLoadConstStep;
	// 自定义区间加载滑行步骤是否正在进行.     false:进行中,true:未进行
	bool  m_bCustomRangeLoadConstStep;

private:
	// 寄生功率参数
	float m_fAp;
	float m_fBp;
	float m_fCp;

private:
	// 随机生成一个误差值
	int m_nRandomError;
	int RandomError(const UINT& m_unRangeCCDT);
	// 内损
	float m_fInternalDamage;
	// 开始时间
	bool m_bWriteTimer;
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;
	// Lugdown区间数值
	void LugdownRangeACDT(void);
	UINT m_unLugdownRangeACDT30[9];
	UINT m_unLugdownRangeCCDT30[9];
	UINT m_unLugdownRangeACDT20[9];
	UINT m_unLugdownRangeCCDT20[9];
	UINT m_unLugdownRangeACDT10[9];
	UINT m_unLugdownRangeCCDT10[9];
	float m_fLugdownPLHP[9];
	// 广州要求柴油线4个区间
	UINT m_unGZLugdownRangeACDT[2];
	UINT m_unGZLugdownRangeCCDT[2];


	bool Precomputation(void);
};
