/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHT1LCalibrationCheckDlg.h
* 文件标识：
* 摘要：不透光NHT-1L校准和检查，检查用2块标准滤光片
*
*
* 当前版本：1.0
* 作者：Raylee
* 完成日期：2009-12-25
*
* 历史信息：
*
*
* 版本：1.0.3
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.2
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 当前版本：1.0.1
* 时间：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini文件的更新调整程序
*
* 当前版本：1.0.0
* 时间：2009-12-30
* 作者：Raylee
* 描述：完成,发布使用
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

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT


#include "resource.h"		// 主符号


// CNHT1LCalibrationCheckDlg 对话框

class CNHT1LCalibrationCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHT1LCalibrationCheckDlg)

public:
	CNHT1LCalibrationCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHT1LCalibrationCheckDlg();

// 对话框数据
	enum { IDD = IDD_NHT1LCALCHK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();

private:
	CShadeTitle m_lbTitle;	// 对话框title
	CLabel m_lbInfo;		// 信息标题
	CLabel m_lb1;			// 副标题,实时原始值
	CLabel m_lb2;			// 副标题,标定结果
	CLabel m_lb3;			// 副标题,不透光度计校准检查
	CLabel m_lbN;			// 实时N值
	CLabel m_lbK;			// 实时K值
	CLabel m_lbStep1;		// 检查步骤1
	CLabel m_lbStep2;		// 检查步骤2
	CLabel m_lbStep3;		// 检查步骤3
	CLabel m_lbStep4;		// 检查步骤4
	CLabel m_lbStep5;		// 检查步骤5
	CLabel m_lbAvg1;		// 滤光片1测量平均值
	CLabel m_lbAvg2;		// 滤光片2测量平均值
	CLabel m_lbLinear1;		// 滤光片1线性度
	CLabel m_lbLinear2;		// 滤光片2线性度
	CLabel m_lbError1;		// 滤光片1示值误差
	CLabel m_lbError2;		// 滤光片2示值误差
	CLabel m_lbJudgement;	// 检查结果判定
	CListCtrl m_lstResult;	// 结果列表
	CMathLabel m_mlK;		// K值单位,m-1上标
	CNumericEdit m_edStdN1;	// 滤光片1标准值
	CNumericEdit m_edStdN2;	// 滤光片2标准值

private:
	void InitCtrls(void);			// 初始化控件
	void SetDlgFont(void);			// 设置对话框字体
	void ClearResult(void);			// 清除上次校准结果
	void ExportCalChkDetail(void);	// 导出检查详细信息
	void ExportCalChkResult(void);	// 导出检查结果
	void CalcResult(void);			// 计算结果
	void SaveStdNVal(void);			// 保存标准值
	void OpenSerialPortAndTestCommunication(void);			// 打开串口并测试通讯

private:	
	CFont m_fontDlgFont;	// 通用对话框字体
	int m_nDlgFontSize;		// 通用对话框字体大小
	int m_nTestStep;        // 标识当前测试步骤
	int m_nTime;            // 计时标识
	int m_nN1[24];			// 0~19为实时值,20为平均值,21为线性度,22为超出滤光片值150%所占的比例,23为示值误差	放大100倍,四舍五入
	int m_nN2[24];			// 0~19为实时值,20为平均值,21为线性度,22为超出滤光片值150%所占的比例,23为示值误差	放大100倍,四舍五入
	int m_nStdN1;			// 标准滤光片1数值,单位:0.01%,放大100倍,四舍五入
	int m_nStdN2;			// 标准滤光片2数值,单位:0.01%,放大100倍,四舍五入
	int m_nAELimit;			// 绝对误差限值,单位:0.01%,放大100倍,四舍五入
	int m_nLinearLimit;		// 线性度限值,放大100倍,四舍五入

	bool m_bIsLinear1Pass;	// 滤光片1线性度是否合格
	bool m_bIsLinear2Pass;	// 滤光片2线性度是否合格
	bool m_bIsAE1Pass;		// 滤光片1绝对误差是否合格
	bool m_bIsAE2Pass;		// 滤光片2绝对误差是否合格
	bool m_bIsN1Pass;		// 滤光片1检查是否合格
	bool m_bIsN2Pass;		// 滤光片2检查是否合格
	bool m_bIsCalChkPass;	// 校准检查是否通过
	bool m_bIsOperationFinishedAtLeaseOnce;	// 操作是否至少完成1次

	enum						//Timer标识
	{	TIMER_OPEN_COM = 1,		//打开串口
		TIMER_WAIT_FOR_CAL,		//等待校准命令
		TIMER_GET_STATUS,		//获取实状态
		TIMER_GET_REAL_TIME_VAL,//获取实时值
		TIMER_MEASURE_N1,		//获取滤光片1测量值
		TIMER_MEASURE_N2		//获取滤光片2测量值
	};

	enum						//Step标识
	{	STEP_PREPARE,			//发送校准指令
		STEP_INPUT_N,			//输入滤光片示值
		STEP_MEASURE_N1,		//插入滤光片1
		STEP_MEASURE_N2,		//插入滤光片2
		STEP_DONE				//完成,计算结果
	};

private:	
	COpaThread * m_pOpaThread;	// 不透光线程

};
