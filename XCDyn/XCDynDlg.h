
// XCDynDlg.h : 头文件
//

#pragma once

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

// CXCDynDlg 对话框
class CXCDynDlg : public CDialogZoom
{
// 构造
public:
	CXCDynDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CXCDynDlg(void);
// 对话框数据
	enum { IDD = IDD_XCDYN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	// 提示信息
	CLabel m_lbInfo;
	// 标题
	CShadeTitle m_shdTitle;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);

private:
	// 测功机线程
	CDynThread *m_pDynThread;

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

};
