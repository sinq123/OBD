
// ZKYTDlg.h : 头文件
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
	// ywid 业务 id
	std::wstring strywid;
	// cphm 车牌号码
	std::wstring strcphm;
	// cpys 车牌颜色
	std::wstring strcpys;
	// vin 车架号
	std::wstring strvin;
	// clxh 车辆型号
	std::wstring strclxh;
	// fdjxh 发动机型号
	std::wstring strfdjxh;
	// isok 该车型OBD通讯是否有成功记录
	std::wstring strisok;
	// fueltype 燃料种类
	std::wstring strfueltype;
};

struct SOBDJCY
{
	std::wstring strzj;
	std::wstring strmc;
};

typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;
// CZKYTDlg 对话框
class CZKYTDlg : public CDialogEx
{
// 构造
public:
	CZKYTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ZKYT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 数据库
	CEdit m_edCatalog;
	CEdit m_edSource;
	CEdit m_edUserID;
	CEdit m_edPassWord;
	// 联网
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
	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);
	// 获取平台OBD检测员
	bool GetInterOBDJCY(void);
	// 获取平台OBD检测仪ID
	bool GetInterOBDJCYID(void);
	// 待检列表
	bool GetVehicleList(void);
	// 获取联网令牌
	bool GetLicenseCode(CString& strLicCode);
	// 上传软件版本
	bool UpBBH(void);
	// 是否模拟
	bool m_bMNOBDCAL;
	// 是否为不支持还是不适用
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
