
// ParameterParamsCheckDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "resource.h"

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

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\Debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\Release\\OBDEquThr.lib" )
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

// CParameterParamsCheckDlg 对话框
class CParameterParamsCheckDlg : public CDialogZoom
{
// 构造
public:
	CParameterParamsCheckDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CParameterParamsCheckDlg();

// 对话框数据
	enum { IDD = IDD_PARAMETER_PARAMS_CHECK_DLG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 参数测量仪线程
	CParaThread *m_pParaThread;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 上标题
	CShadeTitle m_lbTitle;
	// 提示信息
	CLabel m_lbInfo;
	// 标题1
	CLabel m_lbLabel1;
	// 标题2
	CLabel m_lbLabel2;
	// 标题3
	CLabel m_lbLabel3;
	// 标题4
	CLabel m_lbLabel4;
	// 标题5
	CLabel m_lbLabel5;
	// 温度显示框
	CLabel m_lbET;
	// 湿度显示框
	CLabel m_lbRH;
	// 大气压显示框
	CLabel m_lbAP;
	// 油温显示框
	CLabel m_lbOT;
	// 转速显示框
	CLabel m_lbRev;

private:
	void RefreshData(void);
};
