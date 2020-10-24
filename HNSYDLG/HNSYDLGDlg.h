
// HNSYDLGDlg.h : ͷ�ļ�
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
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

// CHNSYDLGDlg �Ի���
class CHNSYDLGDlg : public CDialogEx
{
// ����
public:
	CHNSYDLGDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HNSYDLG_DIALOG };

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
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnBnClickedBtnSetconfig();
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
	// ���վ���
	CEdit m_edStationNumber;
	// ����߱��
	CEdit m_edLineNumber;
	// ��������
	CEdit m_edLincenseCode;

	// ���ƺ���
	CLabel m_lbPlateNumber;
	// ���ܺ�
	CLabel m_lbVIN;
	// ��ⷽ��
	CLabel m_lbTestType;
	// OBD���
	CLabel m_lbOBDItem;
	// ȼ�����
	CLabel m_lbFuelType;
	// ��������
	CLabel m_lbUnladenMass;
	// ������
	CLabel m_lbMaximumTotalMass;
	// �Ƿ���OBD
	CLabel m_lbHasOBD;
	// �ת��
	CLabel m_lbRatedRev;
	// �����
	CLabel m_lbRatedPower;
	// ��־���
	CLabel m_lbRunningNumber;

	CEdit m_edUpConfigName;
	CEdit m_edUpConfigProject;

private:
	// ��־�ļ�
	void GetLogFilePath(void);
	CString m_strLogFilePath;

	// ��ȡ������Ϣ
	void SetConfig(void);
	// OBD���Ա
	void GetOBDJCY(void);
	// ��ȡ��������
	bool GetAccessToken(void);


	// ��ȡ������
	// �����־
	TESTLOG m_sTestLog;
	bool GetTestLog(void);
	CString ToTestType(const CString& strTestTypeCode);
	CString ToItem(const CString& strItemCode);

	// �趨������Ϣ
	void SetVehWin(const TESTLOG &sTestLog);

	// ����INI�����ݿ���ϴ�
	bool SaveINIAndSQLAndUP(void);
	// ����OBD����
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	// �ϴ�����
	bool UpOBDReaust(const char * pchURL, const CStringW& strLineNumber, const CStringW& strLincenseCode, const CStringW& strStationNumber, 
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	bool UpOBDIURP(const char * pchURL, const CStringW& strLineNumber, const CStringW& strLincenseCode, const CStringW& strStationNumber, 
		const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);
};
