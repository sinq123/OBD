
// MFCDAFDlg.h : ͷ�ļ�
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

// CMFCDAFDlg �Ի���
class CMFCDAFDlg : public CDialogEx
{
// ����
public:
	CMFCDAFDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CMFCDAFDlg();
// �Ի�������
	enum { IDD = IDD_MFCDAF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// ��ַ
	CEdit m_edFilePath;
	afx_msg void OnEnChangeEdFilepath();

	void PrintLog(LPCTSTR szFormat, ...);
	CRichEditCtrl m_reLog;

	cv::Mat m_img;
	void DrawMat(cv::Mat& img, UINT nID);
};
