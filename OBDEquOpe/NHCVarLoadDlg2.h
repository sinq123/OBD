/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCVarLoadDlg2
* 文件标识：
* 描述：南华测功机变载荷测试(按照国标执行)
*
* 版本：1.0
* 作者：hemincong
* 日期：2010-09-29
*
* 历史信息：
*
* 版本：1.0.1
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.0
* 日期：2010-09-29
* 作者：hemincong
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

#include "resource.h"		// 主符号
#include "afxwin.h"
#include "NHCVarLoadThread2.h"
// NHCVarLoadGBDlg 对话框

class CNHCVarLoadDlg2 : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCVarLoadDlg2)

public:
	CNHCVarLoadDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCVarLoadDlg2();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_VAR_LOAD_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStarttest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedRadioCar();
	afx_msg void OnBnClickedRadioEng();

	DECLARE_MESSAGE_MAP()

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 标题信息
	CShadeTitle m_shdTitle;
	// “测试结果”label
	CLabel m_lbTip1;
	// “测试数据”label
	CLabel m_lbTip2;
	// “当前速度”label
	CLabel m_lbTip3;
	// “当前功率”label
	CLabel m_lbTip4;
	//提示消息
	CLabel m_lbMsg;
	//显示当前速度
	CLabel m_lbSpeed;  
	//当前功率显示
	CLabel m_lbPower;
	//80.5~8.0名义时间
	CLabel m_lbTime2;
	//72.4~16.1名义时间
	CLabel m_lbTime3;
	//61.1~43.4名义时间
	CLabel m_lbTime4;
	//显示80.5~8.0所用时间
	CLabel m_lbTime_Rel2;
	//显示72.4~16.1所用时间
	CLabel m_lbTime_Rel3;  
	//显示61.1~43.4所用时间
	CLabel m_lbTime_Rel4;
	//显示最终结果
	CLabel m_lbTime_Result; 
	//显示80.5~8.0所用时间与标准时间误差
	CLabel m_lbTime_Err2;
	//显示72.4~16.1所用时间与标准时间误差
	CLabel m_lbTime_Err3; 
	//显示61.1~43.4所用时间与标准时间误差
	CLabel m_lbTime_Err4; 
	//显示基本惯量
	CLabel m_lbDIW;
	//显示实时数据
	CColorListCtrl m_ListReal;
	//加速类型选项
	CButton m_CtrlRadio1;
	CImageList m_SmallImageList;

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

private:
	//变载荷滑行（HJ/T290-2006）控制线程
	CNHCVarLoadThread2  *m_pNHCVarLoadThread2;

private:
	//加速类型
	int m_nAccType;
	//正在测试的速度段数
	int m_nStatues;                
	//各段的加载功率
	float m_fPowerAdd[46];
	//判断速度是否已经超过88km/h,只要有一次超过就设为true
	bool  m_bIsSpeedUp; 
	//基本惯量
	USHORT m_usDIW; 
	//80.5-8.0名义时间
	float m_fNominalTime1;
	//72.4-16.1名义时间
	float m_fNominalTime2;
	//61.4-43.4名义时间
	float m_fNominalTime3;
	//72.4~16.1实测时间
	float m_fRealityTime1;
	//48.3~16.1实测时间
	float m_fRealityTime2;
	//61.1-43.4实测时间
	float m_fRealityTime3;
	//80.5-8.0实际误差
	float m_fErr1;
	//72.4-16.1实际误差
	float m_fErr2;	
	//61.1-43.4实际误差
	float m_fErr3;
	//测试是否合格
	bool m_bResult;
};
