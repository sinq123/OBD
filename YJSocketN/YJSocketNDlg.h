
// YJSocketNDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Socket.h"
#include "json\json.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
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
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

// CYJSocketNDlg �Ի���
class CYJSocketNDlg : public CDialogEx
{
// ����
public:
	CYJSocketNDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CYJSocketNDlg();
// �Ի�������
	enum { IDD = IDD_YJSOCKETN_DIALOG };

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
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnBnClickedBtnSocket();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpTelet();
private:
	// OBD���Ա
	CComboBox m_cmOBDJCY;
	// ��Ϣ
	CEdit m_edMsg;
	// �����б�
	CListCtrl m_lstVehicle;
	// ����Դ
	CEdit m_edCatalog;
	// �����
	CEdit m_edLineNumber;
	// ����
	CEdit m_edPassWord;
	// ���ƺ���
	CEdit m_edPlateNumber;
	// ����socketIP
	CEdit m_edIP;
	// ����Post
	CEdit m_edPort;
	// ���վ���
	CEdit m_edStationNUmber;
	// ���ݿ�����
	CEdit m_edSource;
	// ��¼��
	CEdit m_edUserID;

	CButton m_edchkSQL;
	CButton m_edIntet;
	CButton m_btnSocket;

	CEdit m_edCALID;
	CEdit m_edCVN;
	CEdit m_edOBDType;
public:
	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);
	// �����б�
	bool GetVehicleList(void);
	
	
	// ��֤\����ע��
	bool m_bAuthentication;
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	// ���ͽ���������
	CSocket m_yjSocket;
	// ����Ψһ����
	CString GenerateInsNum(void);
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv);
	// У����
	// ����ǰnλ��У���
	// ��ӣ�Ȼ��ȡ����ֽ�λ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	CString CheckSum(const char *ch, const UINT un);
	// �ر�����
	void CloseConnect(void);
	// ʱ��
	bool GetTime(void);
	
	
public:
	CString m_strRunningNumber;
	bool SetOBDLOG(const TESTLOG& sTestLog, SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	bool UpOBDStart(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, 
		const SResultOfOBD& sResultOfOBD, const VEHICLEINFO& sVehInfo, CString& strMsg);

	bool UpOBDReaust(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, 
		const SResultOfOBD& sResultOfOBD, const VEHICLEINFO& sVehInfo, CString& strMsg);
	//bool UpOBDIURP(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);

public:
	CStringW m_strStationNum;
	CStringW m_strLineNum;

	bool UPRealTimeData(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	bool UPTestSignal(const CStringW& strStationNum, const CStringW& strLineNum, const TESTLOG& sTestLog, const SResultOfOBD& sResultOfOBD, CString& strMsg);


	afx_msg void OnBnClickedBtnTestlog();
};
