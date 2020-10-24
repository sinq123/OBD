/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：MHDForceCalibrationDlg.h
* 文件标识：
* 摘要：测功机力校准(马哈测功机专用)
*       因为和南华测功机力校准非常不一样，所以单独出来一个模块
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2010-07-08
*
* 
* 历史信息：
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
* 版本：1.0.0
* 时间：2010-07-08
* 作者：Cui
* 描述：完成,发布使用
*/

#pragma once

#include "resource.h"		// 主符号

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

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CMHDForceCalibrationDlg 对话框

class CMHDForceCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CMHDForceCalibrationDlg)

public:
	CMHDForceCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMHDForceCalibrationDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MHD_FORCE_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonReset();

private:
	CLabel m_lbInfo;
	CLabel m_lbTorsion;
	CLabel m_lbStep1;
	CLabel m_lbStep2;
	CLabel m_lbStep3;
	CLabel m_lbStep4;
	CLabel m_lbStep5;
	CLabel m_lbStep6;
	CLabel m_lbStep;
	CLabel m_lbReset;
	CShadeTitle m_lbTitle;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
    // 清除当前结果
	void ClearResult();   
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 是否允许刷新数据
	void EnableRefreshRealTimeData(bool bEnable=true);
	// 刷新数据
	void RefreshRealTimeData(void);

	// 导出力校准实时数据
	void ExportDynForceCalibrationRTData(void);
	// 导出力校准结果(内部定义)
	void ExportDynForceCalibrationResult(void);

	//////////////////////////////////////////////////////////////// 过程控制回调函数接口begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口end

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
    // 测试步骤
    int m_nTestStep;
	// 零位稳定时间
	int m_nZeroTime;	
	// 校准时的连续稳定时间
	int m_nStayTime;	
	// 保存最近5秒的力实时值
	float m_fStayF[5];
	// 力校准重量
	float m_fCalForce;	
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;
	// 校准是否通过
	bool m_bIsPass;

private:
	// 测功机控制线程
	CDynThread *m_pDynThread;
public:
	afx_msg void OnBnClickedCancel();
};
