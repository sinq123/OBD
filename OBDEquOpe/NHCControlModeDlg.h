/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHCControlModeDlg.h
* 文件标识：
* 摘要：测功机控制模式
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2009-11-22
*
* 历史信息：
*
*
* 版本：1.0.7
* 日期：2010-09-30
* 作者：Qin
* 描述：改进数据显示字体颜色和风格
*
* 版本：1.0.6
* 日期：2010-09-27
* 作者：Ming
* 描述：将原本字体颜色为RGB(58, 248, 250)的控件改成颜色RGB(0, 255, 0)
*
* 版本：1.0.5
* 日期：2010-08-20
* 作者：Qin
* 描述：完善加入恒速模式（电机）
*
* 版本：1.0.4
* 日期：2010-07-07
* 作者：Cui
* 描述：1）整理代码
*       2）添加测功机通讯协议选择功能
*       3）将不成熟的恒速模式（电机）暂时删除
*
* 版本：1.0.3
* 日期：2010-03-15
* 作者：Cui
* 描述：将标题栏由“控制模式”改为“测功机控制模式”
*
* 版本：1.0.2
* 日期：2010-02-02
* 作者：Cui
* 描述：规定目标值只能输入数字和小数点
*
* 版本：1.0.1
* 日期：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 版本：1.0.0
* 日期：2009-11-22
* 作者：Cui
* 描述：正式发布使用
*
*
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

// CNHCControlModeDlg 对话框

class CNHCControlModeDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCControlModeDlg)

public:
	CNHCControlModeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHCControlModeDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_NHC_CONTROL_MODE };
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonConstantPowerOn();
	afx_msg void OnBnClickedButtonConstantForceOn();
	afx_msg void OnBnClickedButtonConstantVelocityOn();
	afx_msg void OnBnClickedButtonConstantVelocityOn2();
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedButtonControlModeOff();
	afx_msg void OnBnClickedCancel();

private:
	// 标题栏
	CShadeTitle m_lbTitle;
	// 提示信息
	CLabel m_lbInfo;

	// 标签1
	CLabel m_lb1;
	// 标签2
	CLabel m_lb2;
	// 标签3
	CLabel m_lb3;

	// 功率显示框
	CLabel m_lbP;
	// 力显示框
	CLabel m_lbF;
	// 速度显示框
	CLabel m_lbV;
	// 保存电机额定最大速度
	float m_fNominalV;

	CNumericEdit m_nePValue;
	CNumericEdit m_neFValue;
	CNumericEdit m_neVValue;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 刷新实时数据
	void EnableRefreshRealTimeData(bool bEnable=true);
	// 刷新实时数据
	void RefreshRealTimeData(void);
	// 允许基本操作
	void EnableBasicOperation(bool bEnable=true);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);

private:
	afx_msg LRESULT OnGetRealTimeData(WPARAM wParam, LPARAM lParam);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 测功机线程
	CDynThread * m_pDynThread;

};
