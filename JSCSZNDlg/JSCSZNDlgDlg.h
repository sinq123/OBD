
// JSCSZNDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

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

#include "..\SZInterfaceLib\SZInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\SZInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\SZInterfaceLib.lib")
#endif

struct SVehInfoList
{
	//	plate         ����
	std::wstring strplate;
	//	plate_color   ������ɫ
	std::wstring strplate_color;
	//	vin           ���ܺ�
	std::wstring strvin;
	//	clxh          �����ͺ�
	std::wstring strclxh;
	//	checkid       ��ⱨ����
	std::wstring strcheckid;
	// check_type �������
	std::wstring strcheck_type;
	// check_method ��ⷽ��
	std::wstring strcheck_method;
};

// CJSCSZNDlgDlg �Ի���
class CJSCSZNDlgDlg : public CDialogEx
{
// ����
public:
	CJSCSZNDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CJSCSZNDlgDlg();
// �Ի�������
	enum { IDD = IDD_JSCSZNDLG_DIALOG };

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

	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpobdtimedata();
public:
	// ���ݿ�
	CEdit m_edCatalog;
	CEdit m_edSource;
	CEdit m_edUserID;
	CEdit m_edPassWord;
	CComboBox m_cbOBDJCY;

	// ����
	CEdit m_edURL;
	CEdit m_edunitid;
	CEdit m_edlineid;
	CEdit m_edtoken;
	CEdit m_edInterusername;
	CEdit m_edInterpwd;
	// ����VIN
	CEdit m_edVINCJH;
	// ��Ϣ
	CEdit m_edMsg;
	// �б�
	CListCtrl m_lstVehicle;
private:
	// ���ô����б�
	void SetVehCtrl(void);
	// ���ÿؼ�����
	void SetConfig(void);

private:
	// ���ݿ������־�ļ�
	CStringW m_strDBLogFilePath;
	// �������ݿ�
	bool OpenDB(void);
	// �ر����ݿ�
	bool CloseDB(void);
	// �Ƿ��Ѿ��������ݿ�
	bool m_bConnectDB;
	// ���ݿ�����
	_ConnectionPtr m_pConnection;
private:
	// ������־�ļ�
	CStringW m_strLogFilePath;
	void LogFilePath(void);
private:
	// OBD���Ա
	void GetOBDJCY(void);
	// ��½�ɹ��󷵻ص�Ψһ��ʶ�ַ���
	void GetToken(void);
	// ��ȡƽ̨�б����
	bool GetVehInfoList(void);
	// ��ȡ�ļ���Ϣ
	CString GetTXT(const CString& strPath);
	// ������ͺͼ�ⷽ��ת��
	// 1,������ͣ� 2����ⷽ��, 3 ������ɫ
	CString ToCheck(const CString& strCheck, const int& nType);

private:
	// ������Ϣ����
	SVehInfoList m_sVehInfoList;
	TestLog m_sTestLog;
	// ��ȡ������Ϣ
	bool GetVehInfo(void);


private:
	// OBD������
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	// ����INI�����ݿ���ϴ�
	bool SaveINIAndSQLAndUP(void);
	bool UpOBDReaustAndIURP(const SResultOfOBD& sResultOfOBD, CString& strMsg);
	// OBD��������
	bool UpOBDRealTimeData(CString& strMsg);
};
