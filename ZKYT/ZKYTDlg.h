
// ZKYTDlg.h : ͷ�ļ�
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
// CZKYTDlg �Ի���
class CZKYTDlg : public CDialogEx
{
// ����
public:
	CZKYTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZKYT_DIALOG };

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
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnTotsetlog();
public:
	// ���ݿ�
	CEdit m_edCatalog;
	CEdit m_edSource;
	CEdit m_edUserID;
	CEdit m_edPassWord;
	// ����
	CEdit m_edURL;
	CEdit m_edStationNumber;
	CEdit m_edLicenseCode;
	CEdit m_edLineNum;
	CEdit m_edMsg;
	CComboBox m_cbOBDJCY;
	CComboBox m_cbOBDID;
	CListCtrl m_lstVehicle;
	CEdit m_edOBDJCYID;
	CButtonST m_cbBtn;

	CEdit m_edUpConfigName;
	CEdit m_edUpConfigProject;

public:
	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);
	CString JsonToCString(const rapidjson::Value &Val);
	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);
	// ��ȡƽ̨OBD���Ա
	bool GetInterOBDJCY(void);
	// ��ȡƽ̨OBD�����ID
	bool GetInterOBDJCYID(void);
	// �����б�
	bool GetVehicleList(void);
	// ��ȡ��������
	bool GetLicenseCode(CString& strLicCode);
	// �ϴ�����汾
	bool UpBBH(void);
	// �Ƿ�ģ��
	bool m_bMNOBDCAL;
	// �Ƿ�Ϊ��֧�ֻ��ǲ�����
	bool m_bNoOrNotApplicable;

	SOBDVehInfo m_sOBDVehInfo;
	std::list<SOBDJCY> m_lsOBDJCY;
	
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	
	bool UpOBDReaust(const char * pchURL, const CStringW& strStationNumber, const CStringW& strLincenseCode,
		const CStringW& strOBDJCYID, const SResultOfOBD& sResultOfOBD, CString& strMsg);
	bool UpOBDIURP(const char * pchURL, const CStringW& strStationNumber, const CStringW& strLincenseCode,
		const CStringW& strOBDJCYID, const SResultOfOBD& sResultOfOBD, CString& strMsg);
};
