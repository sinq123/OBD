
// OBDOperationDlg.h : 头文件
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
#pragma comment(lib, "..\\debug\\OBDEquThr_D.lib")
#else
#pragma comment(lib, "..\\release\\OBDEquThr.lib")
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

// COBDOperationDlg 对话框
class COBDOperationDlg : public CDialogZoom
{
// 构造
public:
	COBDOperationDlg(CWnd* pParent = NULL);	// 标准构造函数
	~COBDOperationDlg();

// 对话框数据
	enum { IDD = IDD_OBD_OPERATION_DLG };

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
	afx_msg void OnBnClickedBtnVciVersion();
	afx_msg void OnBnClickedBtnVIN();
	afx_msg void OnBnClickedBtnEcuInfo();
	afx_msg void OnBnClickedBtnGetIupr();
	afx_msg void OnBnClickedBtnLampState();
	afx_msg void OnBnClickedBtnObdType();
	afx_msg void OnBnClickedBtnGetDtc();
	afx_msg void OnBnClickedBtnGetRtdata();
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedRadioFuelType();

private:
	// 初始化OBD	
	LRESULT OnInit(WPARAM wParam, LPARAM lParam);

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 界面日志
	void WriteLog(LPCTSTR szLog);

private:
	COBDAdapterThread *m_pAdapterThread;
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	
	// 初始化计时
	UINT m_unInitTimeout;
	// 汽油/柴油
	int m_nFuelType;

private:
	// 上标题
	CShadeTitle m_lbTitle;
	// 提示信息
	CLabel m_lbInfo;
	// 返回的协议
	CLabel m_lbProtocol;
	// 车速
	CLabel m_lbVelocity;
	// 转速
	CLabel m_lbRev;
	// 冷却液温度
	CLabel m_lbCoolantTemp;
	// 协议
	CComboBox m_cbProtocol;

	CRichEditCtrl m_recLog;

};
