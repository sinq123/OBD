/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCForceCalibrationHeavyDlg.h
* 文件标识：
* 摘要：重型测功机力校准
*
* 当前版本：1.0
* 作者：Raylee
* 完成日期：2010-01-11
*
* 
* 历史信息：
*
* 版本：1.0.12
* 日期：2012-01-30
* 作者：Qin
* 描述：加入自由配置是否显示测功机的四个涡流机
*
* 版本：1.0.11
* 日期：2010-10-28
* 作者：Raylee
* 描述：验证放砝码重量由正负200N放宽到正负1000N
*
* 版本：1.0.10
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.9
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
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
* 描述：改导出标定日志和检查结果数据
*
* 当前版本：1.0.5
* 完成日期：2010-05-12
* 作者：Raylee
* 描述：写标定记录表改用回调函数
*
* 版本：1.0.4
* 时间：2010-03-18
* 作者：Raylee
* 描述：改进退出提示
*
* 版本：1.0.3
* 时间：2010-03-15
* 作者：Cui
* 描述：将标题栏由“力校准”改为“测功机力校准”
*
* 版本：1.0.2
* 时间：2010-01-21
* 作者：Raylee
* 描述：1、所有消息加"力校准"标题
*		2、根据重型测功机力校准重置命令更新程序
*
* 版本：1.0.1
* 时间：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 版本：1.0.0
* 时间：2010-01-11
* 作者：Raylee
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

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CNHCForceCalibrationHeavyDlg 对话框

class CNHCForceCalibrationHeavyDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCForceCalibrationHeavyDlg)

public:
	CNHCForceCalibrationHeavyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCForceCalibrationHeavyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_FORCE_CALIBRATION_HEAVY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedRadioPau();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedCancel();

private:
	CShadeTitle m_lbTitle;
	CLabel m_lbInfo;
	CLabel m_lbTorsion;
	CLabel m_lbStep1;
	CLabel m_lbStep2;
	CLabel m_lbStep3;
	CLabel m_lbStep4;
	CLabel m_lbStep;
	CLabel m_lbReset;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
    // 清除当前结果
	void ClearResult();   
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 复位PAU按钮状态
	void ResetRadioPau(void);

	// 导出力校准结果(行标定义)
	//void ExportEquCalChkInfo(void);
	// 导出力校准实时数据
	void ExportDynForceCalibrationRTData(void);
	// 导出力校准记录(内部定义)
	//void ExportDemarcationLog(void);
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
	bool m_bIsPass[4];
	// 是否校准过 
	bool m_bIsCalibrated[4];
	// PAU数
	int m_nPAUCount;
	// 当前选择的PAU
	int m_nCurPAU;

	enum						//Timer标识
	{	
		TIMER_OPEN_COM = 1,		//打开串口
		TIMER_INSTALL_TOOL,		//安装校准工具
		TIMER_GET_RTDATA,		//获取实时数据
		TIMER_ZERO_BALANCE,		//零位取得平衡
		TIMER_WEIGHT_BALANCE,	//校准取得平衡
		TIMER_REMOVE_ALL		//移走所以砝码和校准工具
	};
	enum						//Step标识
	{	
		STEP_PREPARE,			//发送校准指令
		STEP_INTALL_TOOL,		//安装校准工具
		STEP_PUT_WEIGHT,		//放砝码
		STEP_CALIBRATION,		//校准
		STEP_REMOVE_WEIGHT,		//移走砝码
		STEP_RESET_ZERO,		//零位调整
		STEP_RESET_ZERO_DONE,	//零位调整完成
		STEP_DONE				//校准完成
	};
	enum						//当前选择的PAU
	{	
		NOPAU = -1,
		PAU1,				
		PAU2,
		PAU3,
		PAU4
	};

	struct stPAU						//要显示的PAU
	{
		bool bPAU1;				
		bool bPAU2;
		bool bPAU3;
		bool bPAU4;
	} m_stPAUShow;

private:
	// 测功机控制线程
	CDynThread *m_pDynThread;

};