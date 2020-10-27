
// YNDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

// CYNDlg �Ի���
class CYNDlg : public CDialogEx
{
// ����
public:
	CYNDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YN_DIALOG };

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

private:
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnBnClickedBtnUp();
private:
	// ����Դ
	CEdit m_edCatalog;
	// ���ݿ�����
	CEdit m_edSource;
	// ��¼��
	CEdit m_edUserID;
	// ����
	CEdit m_edPassWord;
	// OBD���Ա�б�
	CComboBox m_comOBDJCY;
	// ��Ϣ
	CEdit m_edMsg;
	// �ӿڵ�ַ
	CEdit m_edURL;
	// ����߱��
	CEdit m_edLineNumber;
	// ��������
	CEdit m_edLincenseCode;
	// ���ƺ���
	CLabel m_lbPlateNumber;
	// ���ܺ�
	CLabel m_lbVIN;
	// ���浥���
	CLabel m_lbRepoetNumber;
	// ��־���
	CLabel m_lbRunningNumber;
	// ��ⷽ��
	CLabel m_lbTestType;
	// OBD���
	CLabel m_lbOBDItem;

public:
	CString ToTestType(const CString& strTestTypeCode);
	CString ToItem(const CString& strItemCode);
	// ��ȡ������Ϣ
	void SetConfig(void);
	// OBD���Ա
	void GetOBDJCY(void);
	// ��ȡ������
	bool GetTestLog(CString& strMsg);

	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	bool UpOBDReaust(const char * pchURL, const CStringW& strLineNumber, const CStringW& strLincenseCode,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	bool UpOBDIURP(const char * pchURL, const CStringW& strLineNumber, const CStringW& strLincenseCode,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);

private:
	// �����־
	TESTLOG m_sTestLog;

	CString m_strLogFilePath;

};