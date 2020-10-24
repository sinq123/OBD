
// HYLibDlg.h : ͷ�ļ�
//

#pragma once

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
#include "afxwin.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "..\HYInterfaceLib\HYInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\HYInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\HYInterfaceLib.lib")
#endif

// CHYLibDlg �Ի���
class CHYLibDlg : public CDialogEx
{
// ����
public:
	CHYLibDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HYLIB_DIALOG };

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

	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;
	CEdit m_edPlateNumber;
	CEdit m_edMessAge;
	CEdit m_edURL;
	CEdit m_edKey;
	CEdit m_edStationNum;
	CEdit m_edLineNUmber;

	CButton m_edchkSQL;
	CButton m_edIntet;

	CListCtrl m_lstVehicle;
	CComboBox m_cbOBDJCY;
public:
	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);

	// �����б�
	bool GetVehicleList(void);
	
	// �����־
	CString m_strRunningNumber;
	CString m_strTestType;
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedBtnUpTelet();

	TESTLOG m_sTestLog;
	SResultOfOBD m_sResultOfOBD;

	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);


	bool UPReport(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
		const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg);
	bool UPRealTimeData(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
		const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg);
	bool UPOBDIUPR(const CStringW& strURL, const CStringW& strKey, const CStringW& strStationNum, 
		const CStringW& strLineNum, const CStringW& strTestType, CStringW& strMsg);
};
