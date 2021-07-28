/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCPLHPGB2018Dlg.h
* 文件标识：
* 摘要：测功机寄生功率滑行测试
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-11-24
*
* 历史信息：
*
* 版本：1.0.12
* 日期：2012-09-28
* 作者：wuyq
* 描述：判定测功机升举升器的速度判定条件改为小于等于10e-6
*
* 版本：1.0.12
* 日期：2011-11-28
* 作者：Raylee
* 描述：修正没有导出基本惯量的bug
*
* 版本：1.0.11
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.10
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 版本：1.0.9
* 日期：2010-07-08
* 作者：Cui
* 描述：添加对马哈测功机支持
*
* 当前版本：1.0.8
* 完成日期：2010-05-27
* 作者：Raylee
* 描述：只保留导出校准结果数据
*
* 当前版本：1.0.7
* 完成日期：2010-05-22
* 作者：Raylee
* 描述：1、注释导出EquCalChkInfo.ini的代码
*		2、删除旧的导出代码
*
* 当前版本：1.0.8
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.7
* 完成日期：2010-05-20
* 作者：Raylee
* 描述：导出ini标定结果相关信息改在外部合并，内部不再依赖LineInfo、UserInfo、StationInfo等
*
* 当前版本：1.0.6
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 当前版本：1.0.5
* 完成日期：2010-05-14
* 作者：Raylee
* 描述：1、写标定数据改用回调函数
*		2、改导出标定日志和检查结果数据
*
* 版本：1.0.4
* 时间：2010-04-17
* 作者：Ming
* 描述：1）添加标定完成时, m_bIsOperationFinishedAtLeaseOnce 标志设为true.
*
* 版本：1.0.3
* 时间：2010-03-15
* 作者：Cui
* 描述：1）将标题栏由“寄生功率滑行”改为“测功机寄生功率滑行”
*       2）将“开始测试”按钮和“停止测试”按钮改为“开始”按钮和“停止”按钮
*
* 当前版本：1.0.2
* 完成日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 当前版本：1.0.1
* 完成日期：2009-12-22
* 作者：Cui
* 描述：整理注释和代码格式
*
* 当前版本：1.0.0
* 完成日期：2009-11-24
* 作者：Cui
* 描述：正式发布使用
*
*
*/

#pragma once

#include "resource.h"		// 主符号
#include "NHCPLHPGB2018Thread.h"
#include "PLHPCurve.h"

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


// CNHCPLHPGB2018Dlg 对话框

class CNHCPLHPGB2018Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCPLHPGB2018Dlg)

public:
	CNHCPLHPGB2018Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCPLHPGB2018Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_PLHP_GB2018 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStartTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedRadioAsmRange();
	afx_msg void OnBnClickedRadioVmasRange();
	afx_msg void OnBnClickedRadioLugdownRange();
	afx_msg void OnBnClickedRadioLugdownRange100();

private:
	//////////////////////////////////////////////////////////////////////////////////////////////////控件变量begin
	// 标题栏
	CShadeTitle m_lbTitle;
	// 提示信息
	CLabel m_lbInfo;

	// 基本惯量显示框
	CLabel m_lbDIW;

	// 标签
	CLabel m_lb1;
	CLabel m_lb2;
	CLabel m_lb3;
	CLabel m_lb4;

	// 功率显示框
	CLabel m_lbP;
	// 力显示框
	CLabel m_lbF;
	// 速度显示框
	CLabel m_lbV;

	// Ap显示框
	CLabel m_lbAp;
	// Bp显示框
	CLabel m_lbBp;
	// Cp显示框
	CLabel m_lbCp;

	// 另一Ap显示框
	CLabel m_lbAnotherAp;
	// 另一Bp显示框
	CLabel m_lbAnotherBp;
	// 另一Cp显示框
	CLabel m_lbAnotherCp;

	CButton m_rdoAsmRange;
	CButton m_rdoVmasRange;
	CButton m_rdoLugdownRange;
	CButton m_rdoLugdownRange_100;
	// 结果数据列表
	CListCtrl m_lcResult;

	// 曲线显示框
	CPLHPCurve m_stcPLHPCurve;
	//////////////////////////////////////////////////////////////////////////////////////////////////控件变量end

private:	
	// 导出寄生功率测试结果(行标定义)
	//void ExportEquCalChkInfo(void);
	// 导出寄生功率测试实时数据
	void ExportDynPLHPRTData(void);
	// 导出寄生功率测试记录(内部定义)
	//void ExportDemarcationLog(void);
	// 导出寄生功率测试结果(内部定义)
	void ExportDynPLHPResult(void);

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
	// 是否允许刷新数据
	void EnableRefreshRealTimeData(bool bEnable=true);
	// 刷新数据
	void RefreshRealTimeData(void);
	// 是否允许测功机基本操作
	void EnableBasicOperation(bool bEnable=true);
	// 恢复控件状态
	void RestoreCtrls(void);
	// 显示寄生功率测试流程
	void DisplayPLHPTestProcess(void);
	// 显示寄生功率滑行时间
	inline void DisplayPLHPSldingTime(void);
	inline void DisplayPLHPSldingTime_ASM(void);
	inline void DisplayPLHPSldingTime_VMAS(void);
	inline void DisplayPLHPSldingTime_LUGDOWN(void);
	// 计算寄生功率滑行参数
	void CalculatePLHPParams(void);
	void CalculatePLHPParams_ASM(void);
	void CalculatePLHPParams_VMAS(void);
	void CalculatePLHPParams_LUGDOWN(void);
	// 显示寄生功率曲线
	void ShowPLHPCurve(bool bShow=false);
	// 显示真实寄生功率曲线
	void ShowAnotherPLHPCurve(bool bShow=false);
	// 写入寄生功率滑行参数到测功机
	void WritePLHPParamsToDyn(void);
	// 清除寄生功率结果,包括：各个速度点的寄生功率值m_fPLHP,曲线图,参数,ListCtrl上的数值等
	void ClearPLHPResult(void);
	// 转化Ap、Bp、Cp参数
	void ConvertApBpCp(bool bConvertToPower=true);
	// 现实Ap、Bp、Cp参数
	void ShowtApBpCp(void);

private:
	/// 最小二乘法
	// 求AP,BP,CP
	// (in)pdXArray:X数组指针
	// (in)pdYArray:Y数组指针
	// (in)nAmount:数组大小
	// (out)pdResult:结果数组指针,升幂顺序,pdResult[0]-0阶参数(常数),以此类推
	// (in)grad:阶数,如果只求Ap,Bp,阶数为2,如果要求Ap,Bp,Cp,阶数为3
	// (return)无
	static void Polynom_berechnen(double *const pdXArray, double *const pdYArray, const int nAmount, double *const pdResult, const int grad=3);

	// 计算寄生功率
	// (in)fUpperVelocity:速度区间高速点，单位-km/h
	// (in)fLowerVelocity:速度区间低速点，单位-km/h
	// (in)unSlidingTime_ms:相应速度区间滑行时间，单位-ms
	// (return)寄生功率值，单位-kW,保留2位小数
	float CalculatePLHP(const float fUpperVelocity, const float fLowerVelocity, const UINT unSlidingTime);

	// 计算寄生功率,含剩磁力计算
	// (in)fUpperVelocity:速度区间高速点，单位-km/h
	// (in)fLowerVelocity:速度区间低速点，单位-km/h
	// (in)fSlidingTime:相应速度区间滑行时间，单位-s
	// (in)fRemanence:剩磁力，单位-N
	// (return)寄生功率值，单位-kW
	//float CalculatePLHPEx(const float fUpperVelocity, const float fLowerVelocity, const float fSlidingTime, const float fRemanence);

private:
	// 寄生功率滑行测试完成
	afx_msg LRESULT OnPLHPTestFinished(WPARAM wParam, LPARAM lParam);
	// 寄生功率滑行测试某一速度段完成
	afx_msg LRESULT OnPLHPSectionFinished(WPARAM wParam, LPARAM lParam);
	inline void PLHPSectionFinished_ASM(UINT unSection);
	inline void PLHPSectionFinished_VMAS(UINT unSection);
	inline void PLHPSectionFinished_LUGDOWN(UINT unSection);

private:
	// 测功机寄生功率控制线程
	CNHCPLHPGB2018Thread *m_pNHCPLHPThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 寄生功率测试模式
	enum PLHPMode
	{
		// 平常模式
		PLHP_NORMAL_MODE,
		// 力模式
		PLHP_F_MODE,
		// 功率模式
		PLHP_P_MODE
	};

private:
	// 选用的寄生功率测试模式
	PLHPMode m_pmUsed;
	// 基本惯量，单位-kg
	USHORT m_usDIW;
	// 名义速度,单位-km/h
    float m_fNomialVelocity[9];
	// 寄生功率,单位-kW
	float m_fPLHP[9];
	// 高速点
	float m_fUpperVelocity[9];
	// 低速点
	float m_fLowerVelocity[9];

	// 不带剩磁的寄生功率滑行参数(未转化为功率时的数值，即未乘以80.5时候的数值)
	// 已经减去剩磁
	double m_dApWithoutRemanence;
	double m_dBpWithoutRemanence;
	double m_dCpWithoutRemanence;
	// 带剩磁的寄生功率滑行参数(未转化为功率时的数值，即未乘以80.5时候的数值)
	// 没有减去剩磁
	double m_dApWithRemanence;
	double m_dBpWithRemanence;
	double m_dCpWithRemanence;

	// 不带剩磁的寄生功率滑行参数(转化为功率时的数值，即乘以80.5时候的数值)
	// 已经减去剩磁
	float m_fApWithoutRemanence;
	float m_fBpWithoutRemanence;
	float m_fCpWithoutRemanence;
	// 带剩磁的寄生功率滑行参数(转化为功率时的数值，即乘以80.5时候的数值)
	// 没有减去剩磁
	float m_fApWithRemanence;
	float m_fBpWithRemanence;
	float m_fCpWithRemanence;

private:
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
	// 开始时间
	bool m_bWriteTimer;
	COleDateTime m_odtStart;
};
