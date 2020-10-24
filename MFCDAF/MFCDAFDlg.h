
// MFCDAFDlg.h : 头文件
//

#pragma once

#include "afxwin.h"
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>


#include "..\NHWin32Lib\NHWin32Lib.h"
#include "afxcmn.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// CMFCDAFDlg 对话框
class CMFCDAFDlg : public CDialogEx
{
// 构造
public:
	CMFCDAFDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CMFCDAFDlg();
// 对话框数据
	enum { IDD = IDD_MFCDAF_DIALOG };

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
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 地址
	CEdit m_edFilePath;
	afx_msg void OnEnChangeEdFilepath();

	void PrintLog(LPCTSTR szFormat, ...);
	CRichEditCtrl m_reLog;

	cv::Mat m_img;
	void DrawMat(cv::Mat& img, UINT nID);
};
