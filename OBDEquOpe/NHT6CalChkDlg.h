/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHT6CalChkDlg.h
* 文件标识：
* 摘要：不透光NHT6校准和检查，检查用2块标准滤光片
*       目前可以兼容NHT-1L设备
*
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-07-13
*
* 历史信息：
*
* 版本：1.0.15
* 日期：2012-10-11
* 作者：Raylee
* 描述：添加福立FLB-100，鸣泉MQY-200设备支持
*
* 版本：1.0.14
* 日期：2012-04-24
* 作者：Raylee
* 描述：添加FoFenFTY100支持
*
* 版本：1.0.13
* 日期：2011-01-19
* 作者：Raylee
* 描述：添加原始N值和标准光通长度下的N值同时显示
*
* 版本：1.0.12
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.11
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：1、调整回调代码位置，不透光校准检查“结束”改为“完成”
*		2、回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.10
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.9
* 时间：2010-05-12
* 作者：Raylee
* 描述：标定记录写表操作改用回调函数
*
* 当前版本：1.0.8
* 时间：2010-04-09
* 作者：Raylee
* 描述：改导出标定日志和检查结果数据
*
* 当前版本：1.0.7
* 时间：2010-01-18
* 作者：Cui
* 描述：1)改造成可以兼容NHT1L设备
*
* 当前版本：1.0.6
* 时间：2010-01-14
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序
*
* 当前版本：1.0.5
* 时间：2009-12-24
* 作者：Cui
* 描述：1)更正上次错误的“当前版本号”
*       2)修正ExportCalChkResult函数:将名词“标称值”统一改成“标准值”，并修正格式问题
*
* 当前版本：1.0.4
* 时间：2009-12-09
* 作者：Cui
* 描述：更新不透光线程
*
* 时间：2009-11-25
* 作者：Raylee
* 描述：调整afx_msg void OnSysCommand(UINT nID, LPARAM lParam);位置
*
* 当前版本：1.0.3
* 时间：2009-11-23
* 作者：Raylee
* 描述：改函数ExportCalChkDetail，把标定数据导出到ini
*
* 当前版本：1.0.2
* 时间：2009-11-20
* 作者：Raylee
* 描述：改函数ExportCalChkResult，把标定数据保存在Reserved1字段
*
* 当前版本：1.0.1
* 时间：2009-09-21
* 作者：Cui
* 描述：将界面上的m-1实现上下标的视觉效果
*
* 当前版本：1.0.0
* 时间：2009-08-15
* 作者：Cui
* 描述：把“日常标定”名词改为“校准检查”，并正式使用
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

// 自用库(MFC扩展DLL)
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
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


#include "resource.h"

// CNHT6CalChkDlg 对话框

class CNHT6CalChkDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHT6CalChkDlg)

public:
	CNHT6CalChkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHT6CalChkDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHT6_CAL_CHK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

private:
	//////////////////////////////////////////////////////////////// 数据交换接口begin
	// 导入校准检查参数
	void ImportCalChkParams(void);
	// 导出不透光校准检查结果(行标定义)
	void ExportEquCalChkInfo(void);
	// 导出不透光准检查实时数据
	void ExportCalibrationCheckRTData(void);
	// 导出标定日志(内部定义)
	void ExportDemarcationLog(void);
	// 导出不透光校准检查结果(内部定义)
	void ExportOpaCalChkResult(void);
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
	// 校准检查流程
	void CalChkProcess(void);
	// 重置校准检查
	void ResetCalChk(void); 
	// 滤光片1结果
	void CalculateResult1(void);
	// 滤光片2结果
	void CalculateResult2(void);
	// 滤光片3结果
	void CalculateResult3(void);
	// 显示滤光片1结果
	void ShowResult1(void);
	// 显示滤光片2结果
	void ShowResult2(void);
	// 显示滤光片3结果
	void ShowResult3(void);
	// 显示结果判定
	void ShowJudgement(void);
	// 停止校准检查
	void StopCalChk(void);
	// 获取标准滤光片示值输入
	// 返回0x00,标准滤光片示值输入符合要求
	// 返回其它,标准滤光片示值输入不符要求
	DWORD GetStdValue(void);
	// 保存标准滤光片示值到ini配置文件
	void SaveStdValueToIniFile(void);
	// 清除实时显示框
	void ClearRTDisplay(void);

private:
	// 标题栏
	CShadeTitle m_lbHead;

	// 信息提示栏
	CLabel m_lbInfo;

	// 标准滤光片1数值编辑框
	CNumericEdit m_edN1;
	// 标准滤光片2数值编辑框
	CNumericEdit m_edN2;
	// 标准滤光片3数值编辑框
	CNumericEdit m_edN3;


	// 10秒数据列表框
	CListCtrl m_lcData;

	// N原始值显示框
	CLabel m_lbN;
	// 标准光通道长度下的N值
	CLabel m_lbNstd;
	// K原始值显示框
	CLabel m_lbK;

	// 平均值1显示框
	CLabel m_lbAve1;
	// 平均值2显示框
	CLabel m_lbAve2;
	// 平均值3显示框
	CLabel m_lbAve3;
	// 线性度1显示框
	CLabel m_lbLinear1;
	// 线性度2显示框
	CLabel m_lbLinear2;
	// 线性度3显示框
	CLabel m_lbLinear3;

	// 误差1显示框
	CLabel m_lbErr1;
	// 误差2显示框
	CLabel m_lbErr2;
	// 误差2显示框
	CLabel m_lbErr3;
	// 判定显示框
	CLabel m_lbJudgement;

	// 标签1
	CLabel m_lb1;
	// 标签2
	CLabel m_lb2;
	// 标签3
	CLabel m_lb3;

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
	// 步骤5
	CLabel m_lbStep6;

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
	// 步骤5标签
	CStatic m_stcStep6Label;

	// 上下标文本控件
	CMathLabel m_mlb1;

private:
	// 不透光线程
	COpaThread *m_pOpaThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 步骤
	int m_nStep;
	// 校准需时,单位:秒
	const int m_nCalculationTime;
	// 绝对误差限值,单位:0.01%,放大100倍,四舍五入
	const int m_nAELimit;
	// 线性度限值,放大100倍,四舍五入
	const int m_nLinearLimit;
	// 时间
	int m_nTime;
	// 获取滤光片1数值
	bool m_bGetData1;
	// 获取滤光片2数值
	bool m_bGetData2;
	// 获取滤光片3数值
	bool m_bGetData3;
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
	// 标准滤光片1数值,单位:0.01%,放大100倍,四舍五入
	int m_nStd1;
	// 标准滤光片2数值,单位:0.01%,放大100倍,四舍五入
	int m_nStd2;
	// 标准滤光片3数值,单位:0.01%,放大100倍,四舍五入
	int m_nStd3;

private:
	// 滤光片1测量值,单位:0.01%,放大100倍,四舍五入
	int m_nN1[20];
	// 滤光片2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nN2[20];
	// 滤光片3测量值,单位:0.01%,放大100倍,四舍五入
	int m_nN3[20];
	// 滤光片1测量值平均值,单位:0.01%,放大100倍,四舍五入
	int m_nAve1;
	// 滤光片2测量值平均值,单位:0.01%,放大100倍,四舍五入
	int m_nAve2;
	// 滤光片3测量值平均值,单位:0.01%,放大100倍,四舍五入
	int m_nAve3;
	// 滤光片1测量值线性度,单位:0.01%,放大100倍,四舍五入
	int m_nLinear1;
	// 滤光片2测量值线性度,单位:0.01%,放大100倍,四舍五入
	int m_nLinear2;
	// 滤光片3测量值线性度,单位:0.01%,放大100倍,四舍五入
	int m_nLinear3;
	// 滤光片1绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nAE1;
	// 滤光片2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nAE2;
	// 滤光片3绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nAE3;
	// 滤光片1线性度是否合格
	bool m_bIsLinear1Pass;
	// 滤光片2线性度是否合格
	bool m_bIsLinear2Pass;
	// 滤光片3线性度是否合格
	bool m_bIsLinear3Pass;
	// 滤光片1绝对误差是否合格
	bool m_bIsAE1Pass;
	// 滤光片2绝对误差是否合格
	bool m_bIsAE2Pass;
	// 滤光片3绝对误差是否合格
	bool m_bIsAE3Pass;
	// 滤光片1检查是否合格
	bool m_bIsN1Pass;
	// 滤光片2检查是否合格
	bool m_bIsN2Pass;
	// 滤光片3检查是否合格
	bool m_bIsN3Pass;
	// 校准检查是否通过
	bool m_bIsCalChkPass;

private:
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;
};
