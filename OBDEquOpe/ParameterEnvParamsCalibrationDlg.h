/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：ParameterEnvParamsCalibrationDlg.h
* 文件标识：
* 描述：参数测量仪自带环境参数仪及油温传感器校准
*
* 版本：1.0
* 作者：Ming
* 日期：2010-08-27
*
* 历史信息：
*
* 版本：1.0.1
* 日期：2011-05-04
* 作者：Ming
* 描述：添加恢复默认设置按钮，重置参数功能
*
* 版本：1.0.0
* 日期：2010-08-27
* 作者：Ming
* 描述：beta
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

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\Debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\Release\\OBDEquThr.lib" )
#endif

// 自用库
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
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

#include "resource.h"		// 主符号


// CParameterEnvParamsCalibrationDlg 对话框

class CParameterEnvParamsCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CParameterEnvParamsCalibrationDlg)

public:
	CParameterEnvParamsCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParameterEnvParamsCalibrationDlg();

// 对话框数据
	enum { IDD = IDD_PARAMETER_ENVPARAMS_CALIBRATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedButtonEtCal();
	afx_msg void OnBnClickedButtonRhCal();
	afx_msg void OnBnClickedButtonApCal();
	afx_msg void OnBnClickedButtonOtCal();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonDefaultConfig();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	// 标题栏
	CShadeTitle m_lbTitle;
	// 信息提示栏
	CLabel m_lbInfo;

	// 标题1
	CLabel m_lbLabel1;
	// 标题2
	CLabel m_lbLabel2;
	// 标题3
	CLabel m_lbLabel3;
	// 标题4
	CLabel m_lbLabel4;

	// 环境温度测量值编辑框
	CEdit m_edETMea;
	// 相对湿度测量值编辑框
	CEdit m_edRHMea;
	// 大气压力测量值编辑框
	CEdit m_edAPMea;
	// 油温测量值编辑框
	CEdit m_edOTMea;

	// 环境温度标准值编辑框
	CNumericEdit m_neETStd;
	// 相对湿度标准值编辑框
	CNumericEdit m_neRHStd;
	// 大气压力标准值编辑框
	CNumericEdit m_neAPStd;
	// 油温标准值编辑框
	CNumericEdit m_neOTStd;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 刷新数据
	void RefreshData(void);
	// 使刷新数据
	void EnableRefreshData(bool bEnable = true);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 数据刷新标志
	bool m_bRefreshDataSign;

private:
	// 参数测量仪线程
	CParaThread *m_pParaThread;
};
