
// PLMRDlgDlg.h : ͷ�ļ�
//

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

#include "afxwin.h"
#include "afxcmn.h"

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

#include "..\ZKYTInterfaceLib\ZKYTInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ZKYTInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ZKYTInterfaceLib.lib")
#endif

struct SOBDVehInfo
{
	// ywid ҵ�� id
	std::wstring strywid;
	// cphm ���ƺ���
	std::wstring strcphm;
	// cpys ������ɫ
	std::wstring strcpys;
	// vin ���ܺ�
	std::wstring strvin;
	// clxh �����ͺ�
	std::wstring strclxh;
	// fdjxh �������ͺ�
	std::wstring strfdjxh;
	// isok �ó���OBDͨѶ�Ƿ��гɹ���¼
	std::wstring strisok;
	// fueltype ȼ������
	std::wstring strfueltype;
};

struct SOBDJCY
{
	std::wstring strzj;
	std::wstring strmc;
};

typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

// CPLMRDlgDlg �Ի���
class CPLMRDlgDlg : public CDialogEx
{
// ����
public:
	CPLMRDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLMRDLG_DIALOG };

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
	CComboBox m_cbOBDID;
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
	// ��ȡƽ̨OBD���Ա
	bool GetInterOBDJCY(void);
	// ��ȡƽ̨OBD�����ID
	bool GetInterOBDJCYID(void);
	std::list<SOBDJCY> m_lsOBDJCY;

public:
	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);
	CString JsonToCString(const rapidjson::Value &Val);


	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	
	bool UpOBDReaust(const char * pchURL, const CStringW& strStationNumber, const CStringW& strLincenseCode,
		const CStringW& strOBDJCYID, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	bool UpOBDIURP(const char * pchURL, const CStringW& strStationNumber, const CStringW& strLincenseCode,
		const CStringW& strOBDJCYID, const SResultOfOBD& sResultOfOBD, CString& strMsg);


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

	// ��ȡ������Ϣ
	SOBDVehInfo m_sOBDVehInfo;
	bool GetIntVehInfo(void);
};
