
// TachometerCheckDlg.h : 头文件
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
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\OBDEquThr_D.lib")
#else
#pragma comment(lib, "..\\release\\OBDEquThr.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CTachometerCheckDlg 对话框
class CTachometerCheckDlg : public CDialogZoom
{
// 构造
public:
	CTachometerCheckDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CTachometerCheckDlg();

// 对话框数据
	enum { IDD = IDD_TACHOMETERCHK_DIALOG };

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
	// 转速计线程
	CTacThread *m_pTacThread;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	// 上标题
	CShadeTitle m_lbTitle;
	// 提示信息
	CLabel m_lbInfo;
	// 转速显示框
	CLabel m_lbRev;
};
