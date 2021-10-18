
// YJSocketDlg.h : ͷ�ļ�
//

#pragma once

#include <queue>
#include <memory>
#include <string>
#include <map>
#include <windows.h>
#include "Socket.h"
#include <list>
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
#include "afxwin.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif


typedef std::map<std::wstring,std::wstring> MapData;
typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

struct SHJ03
{
	std::wstring strInspectionNum; // �������
	std::wstring strVLPN; // ���ƺ���
	std::wstring strVLPNColor; // ���ƺ�����ɫ
	std::wstring strVIN; //	���ܺ�
	std::wstring strInspectionMethod;//	��ⷽ��
	std::wstring strInspectionNature;//	������� // ������ͣ�01����/02����/03 ���
	std::wstring strHasOBD;//	�Ƿ��OBDϵͳ
	std::wstring strOBDJCY; // OBD���Ա
};

// CYJSocketDlg �Ի���
class CYJSocketDlg : public CDialogEx
{
// ����
public:
	CYJSocketDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CYJSocketDlg(void);

// �Ի�������
	enum { IDD = IDD_YJSOCKET_DIALOG };

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
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpinter();
	afx_msg void OnBnClickedBtnUptestlog();
	afx_msg void OnBnClickedBtnSocket();
private:
	CButton m_btnSocket;
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;
	CEdit m_edServerIP;
	CEdit m_edServerPost;
	CEdit m_edStationNumber;
	CEdit m_edLineNumber;
	CEdit m_edOBDJCY;

	CListCtrl m_lstVehicle;

	CLabel m_lbMessage;

	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);
private:
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

public:
	// Json�ַ���ת����ʽ
	bool Json2Map(LPCTSTR szJson, JsonMap &mapRet);
	CString Map2Json(JsonMap mapPost);
	CString Json2Cstring(Json::Value &value);
	std::map<std::wstring, std::wstring> m_mapTestType;
	// �Ƿ���Ҫ
	// ��֤\����ע��
	bool m_bAuthentication;
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	bool GetTime(void);
	// �����б�
	bool GetVehicleList(void);
	
	SHJ03 m_sHJ03;
	

	bool SetSQL(const CString& strRunningNumber, const CString& strTestType);

	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	


	bool UpOBDStart(const CString& strInspectionNum, const CStringW& strStationNum, const CStringW& strLineNum);
};
