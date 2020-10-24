/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCVarLoadDlg.h
* 文件标识：
* 描述：南华测功机变载荷测试(按照JJF-1221标准执行)
*
* 版本：1.0
* 作者：Qin
* 日期：2010-06-08
*
* 历史信息：
*
* 版本：1.0.1
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.0
* 日期：2010-06-08
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

// 自用库
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
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
#include "NHCVarLoadThread.h"
// CNHCVarLoadDlg 对话框

class CNHCVarLoadDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCVarLoadDlg)

public:
	CNHCVarLoadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCVarLoadDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_VAR_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 标题信息
	CShadeTitle m_shdTitle;
	//提示消息
	CLabel m_lbMsg;
	//显示当前速度
	CLabel m_lbSpeed;  
	//当前功率显示
	CLabel m_lbPower;
	//72.4~16.1名义时间
	CLabel m_lbTime2;
	//48.3~16.1名义时间
	CLabel m_lbTime3;
	//显示72.4~16.1所用时间
	CLabel m_lbTime_Rel2;
	//显示48.3~16.1所用时间
	CLabel m_lbTime_Rel3;  
	//显示最终结果
	CLabel m_lbTime_Result; 
	//显示72.4~16.1所用时间与标准时间误差
	CLabel m_lbTime_Err2;
	//显示48.3~16.1所用时间与标准时间误差
	CLabel m_lbTime_Err3; 
	//显示基本惯量
	CLabel m_lbDIW;
	//显示实时数据
	CColorListCtrl m_ListReal;
	CButton m_CtrlRadio1;
	CImageList m_SmallImageList;

protected:
	afx_msg void OnBnClickedButtonStarttest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	// 测试种类;ASM,LUGDOWN,VMAS
	int m_TestType;
	//正在测试的速度段数
	int m_nStatues;                
	//各段的加载功率
	float m_fPowerAdd[46];
	//判断速度是否已经超过88km/h,只要有一次超过就设为true
	bool  m_bIsSpeedUp; 
	//基本惯量
	USHORT m_usDIW; 
	//72.4~16.1名义时间
	float m_fNominalTimeOfHigh;
	//48.3~16.1名义时间
	float m_fNominalTimeOfLow;
	//72.4~16.1实测时间
	float m_fRealityTimeOfHigh;
	//48.3~16.1实测时间
	float m_fRealityTimeOfLow;
	//72.4~16.1实际误差
	float m_fErrOfHigh;
	//48.3~16.1实际误差
	float m_fErrOfLow;
	//测试是否合格
	bool m_bResult;
private:
	// 测功机寄生功率控制线程
	CNHCVarLoadThread *m_pNHCVarLoadThread;

protected:
	///初始化控件状态
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 刷新数据
	void RefreshRealTimeData(void);
	// 是否允许刷新数据
	void EnableRefreshRealTimeData(bool bEnable=true);
	// 刷新过程数据
	void DisplayVarLoadTestProcess(void);
	// 显示变载荷滑行时间
	void DisplayVarLoadSldingTime(void);
	// 变载荷ASM滑行时间显示
	void DisplayVarLoadSldingTime_ASM(void);
	// 变载荷VMAS滑行时间显示
	void DisplayVarLoadSldingTime_VMAS(void);
	// 变载荷LUGDOWN滑行时间显示
	void DisplayVarLoadSldingTime_LUGDOWN(void);
	// 计算结果
	void CalculateVarLoadParams(void);
	// 显示结果
	void ShowResult(void);
};
