
// ACNDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
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

#include "..\ACInterfaceLib\ACInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACInterfaceLib.lib")
#endif

// CACNDlg �Ի���
class CACNDlg : public CDialogEx
{
// ����
public:
	CACNDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ACN_DIALOG };

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
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpTelet();
public:
	// ���ݿ�
	CEdit m_edCatalog;
	CEdit m_edSource;
	CEdit m_edUserID;
	CEdit m_edPassWord;
	// ��ʾ��Ϣ
	CEdit m_edMsg;
	// ��������
	CEdit m_edJKDZ;
	CEdit m_edJKXLH;
	CEdit m_edStationNumber;
	CEdit m_edPlateNumber;
	// OBD���Ա
	CComboBox m_cbOBDJCY;
	// �����б�
	CListCtrl m_lstVehicle;

	CButton m_edchkSQL;
	CButton m_edIntet;

public:
	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);
	void GetOBDJCY(void);

	// �����б�
	bool GetVehicleList(void);

	
	// �����־
	CString m_strRunningNumber;
	
	bool SetOBDLOG(const TESTLOG& sTestLog, SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	bool UpOBDReaust(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString OBDIURT(const SResultOfOBD& sResultOfOBD);

	bool UpOBDRealTime(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, CString& strMsg);

	bool UpTestEnd(const char * pchURL, const CStringW& strStationNumber, const CStringW& strJKXLH,
		const TESTLOG& sTestLog, CString& strMsg);
	
	CString ToGACode(const CString& PlateType, const CString& PlateNumber, const CString& strVehicleType);
};
