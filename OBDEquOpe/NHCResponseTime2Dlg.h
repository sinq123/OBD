/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCResponseTime2Dlg.h
* 文件标识：
* 描述：测功机响应时间测试
*
* 版本：1.0
* 作者：zhangwf
* 日期：2016-09-14
*
* 历史信息：
*
* 版本：1.0.1
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格

* 版本：1.0.0
* 日期：2009-12-08
* 作者：Qin
* 描述：正式发布使用
*/
#pragma once

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT


#define ID_CTRL_PLOT_SCAN    WM_USER+1010

// 自用库
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
#include "afxwin.h"
#include "LinePlot.h"
#include "NHCResponseTime2Thread.h"

// CNHCResponseTime2Dlg 对话框

class CNHCResponseTime2Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCResponseTime2Dlg)

public:
	CNHCResponseTime2Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCResponseTime2Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_RESPONSE_TIME2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedBtnStartResponse();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();

public:
	virtual BOOL OnInitDialog();

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 显示标题
	CShadeTitle m_shdTitle;
	// 显示速度
	CLabel m_lbSpeed;
	// 显示力
	CLabel m_lbTraction;
	// 显示功率
	CLabel m_lbPower;
	// 提示信息
	CLabel m_lbMsg;
	// 高速点设定
	CEdit m_edtSpeed_High;
	// 低速点设定
	CEdit m_edtSpeed_Low;
	// 低速点加载力设定
	CEdit m_edtLoadTraction_Low;
	// 高速点加载力设定
	CEdit m_edtLoadTraction_High;
	// 开始滑行速度点设定
	CEdit m_edtTopSilpSpeed;
	// 是否到达滑行速度设定点
	bool m_bIsOverSilpOn;
	// 响应时间
	CLabel m_lbResponse_Time;
	// 稳定时间
	CLabel m_lbStabilization_Time ;

private:
	// 测功机线程
	//CDynThread * m_pDynThread;
	// 测功机控制线程
	CNHCResponseTime2Thread * m_pCNHCResponseTime2Thread;

private:
	// 实时数据链表结构
	struct SRTData
	{
		// 时间序号
		int nTime;
		// 滚筒线速度,单位:km/h
		float fVelocity;
		// 力,单位:N
		float fForce;

		SRTData(){ZeroMemory(this, sizeof(SRTData));}
	};	
	// 声明链表
	CList<SRTData, SRTData> m_lsRTDataOfDynResponseTime;

	// 曲线画点位置
	int m_nInedxPosition;

private:
	// 响应时间测试设定参数
	float m_fSpeed_High;
	float m_fSpeed_Low;
	float m_fLoad_High;
	float m_fLoad_Low;
	float m_fTopSlipSpeed;


	// 基本惯量，单位-kg
	USHORT m_usDIW;

private:
	// 初始化曲线
	void InitCurve();

	// 刷新曲线
	void RefreshCurve();

	void ControlEnableWindows(bool bEnable =1);
	// 曲线显示点
	POINTF  m_n_tempPoint[1], m_n_showPoint;
	// 线的颜色
	COLORREF m_crColor;

	bool IsCanRefreshCurve;
	int nPOINTF;

protected:
	CLinePlot m_stResponseTimeCurve;


private:
	// 数据分析
	void DataAnalysis();
	afx_msg LRESULT SaveRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResponseTimeTestFinished(WPARAM wParam, LPARAM lParam);
};
