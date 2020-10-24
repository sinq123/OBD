/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：YD1CheckDlg.h
* 文件标识：
* 摘要：YD1日常检查窗口
*
* 当前版本：1.0
* 作者：hyh
* 完成日期：2009年9月15日
*
* 历史信息：
*
* 当前版本：1.0.8
* 完成日期：2010-05-21
* 作者：Raylee
* 描述：1、回调提示检查结束改为检查完成
*		2、对话框尺寸改为755x455
*		3、回调提示的wcscpy_s换成wcsncpy_s
*
* 当前版本：1.0.7
* 完成日期：2010-05-18
* 作者：Raylee
* 描述：1、改用过程控制回调函数，可响应多种状态
*		2、原保存数据的回调函数删除
*
* 版本：1.0.6
* 日期：2010-05-12
* 作者：Raylee
* 描述：标定记录写表操作改用回调函数
*
* 版本：1.0.5
* 日期：2010-04-16
* 作者：Raylee
* 描述：1、添加判断是否合格的成员变量
*		2、改导出标定日志和检查结果数据
*
* 版本：1.0.4
* 日期：2010-03-15
* 作者：Cui
* 描述：将标题栏由“重复性和示值误差检查”改为“滤纸式烟度计重复性和示值误差检查”
*
* 版本：1.0.3
* 日期：2010-01-13
* 作者：Cui
* 描述：1)根据Equipment.ini、Demarcation.ini文件的更新调整程序
*
* 版本：1.0.2
* 日期：2009-12-24
* 作者：Cui
* 描述：1)调整界面等细节，令与其它标定界面一致
*       2)以上修改，对功能没有影响，与1.0.1版本功能一致
*
* 版本：1.0.1
* 日期：2009-12-09
* 作者：Cui
* 描述：更新烟度计线程
*
* 时间：2009-11-25
* 作者：Raylee
* 描述：调整afx_msg void OnSysCommand(UINT nID, LPARAM lParam);位置
*
* 版本：1.0.0
* 日期：2009-09-15
* 作者：hyh
* 描述：正式发布
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

#include "resource.h"

// CYD1CheckDlg 对话框

class CYD1CheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CYD1CheckDlg)

public:
	CYD1CheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CYD1CheckDlg();

// 对话框数据
	enum { IDD = IDD_YD1CHECK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();

	// 响应重复性检查消息
	afx_msg LRESULT OnRepeatCheck(WPARAM wParam, LPARAM lParam);
	// 响应示值误差检查消息
	afx_msg LRESULT OnRealCheck(WPARAM wParam, LPARAM lParam);

private:
	// 标签
	CLabel m_lbLabel1;
	CLabel m_lbLabel2;
	CLabel m_lbLabel3;
	CLabel m_lbLabel4;

	CLabel m_lbRepeatError1;
	CLabel m_lbRepeatError2;
	CLabel m_lbRepeatError3;
	CLabel m_lbRepeatError4;
	CLabel m_lbRepeatError5;
	CLabel m_lbRepeatResult;

	CLabel m_lbBasicErr1;
	CLabel m_lbBasicErr2;
	CLabel m_lbBasicErrF;
	CLabel m_lbBasicErr5;
	CLabel m_lbBasicErr6;
	CLabel m_lbBasicResult;

	CNumericEdit m_edFSN1;
	CNumericEdit m_edFSN2;
	CNumericEdit m_edFSNCal;
	CNumericEdit m_edFSNF;
	CNumericEdit m_edFSN5;
	CNumericEdit m_edFSN6;

	CStatic m_stStep1;
	CLabel m_lbStep1;
	CStatic m_stStep2;
	CLabel m_lbStep2;

private:
	// 标题栏
	CShadeTitle m_lbHead;
	// 大标题
	CLabel m_lbInfo;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);
	// 清除检测结果
	void ClearResult(void);
	// 显示重复性检查结果
	void ShowRepeatCheckResult(void);
	// 显示示值误差检查结果
	void ShowBasicErrorCheckResult(void);
	// Open烟度卡输入框
	void EnableFSNEdit(void);
	// 导入烟度卡检查参数
	void ImportFSNCheckParams(void);
	// 保存烟度卡检查参数
	void SaveFSNCheckParams(void);
	// 导出校准检查结果(行标定义)
	void ExportEquCalChkInfo(void);
	// 导出准检查实时数据
	void ExportSmokeCheckRTData(void);
	// 导出标定日志(内部定义)
	void ExportDemarcationLog(void);
	// 导出校准检查结果(内部定义)
	void ExportSmoChkResult(void);
	
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// 过程控制回调函数接口end

private:
	// 烟度计线程
	CSmoThread *m_pSmoThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 当前步骤
	short m_siStep;
	// 操作是否至少完成1次
	bool m_bIsOperationFinishedAtLeaseOnce;
	// 重复性检查是否通过
	bool m_bRepeatPass;
	// 示值误差检查是否通过
	bool m_bBasicErrorPass;
	// 检查是否通过
	bool m_bPass;
};
