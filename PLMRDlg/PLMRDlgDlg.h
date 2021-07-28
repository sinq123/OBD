
// PLMRDlgDlg.h : 头文件
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

// CPLMRDlgDlg 对话框
class CPLMRDlgDlg : public CDialogEx
{
// 构造
public:
	CPLMRDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLMRDLG_DIALOG };

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
	afx_msg void OnBnClickedBtnUp();

private:
	// 数据源
	CEdit m_edCatalog;
	// 数据库名称
	CEdit m_edSource;
	// 登录名
	CEdit m_edUserID;
	// 密码
	CEdit m_edPassWord;
	// OBD检测员列表
	CComboBox m_comOBDJCY;
	CComboBox m_cbOBDID;
	// 信息
	CEdit m_edMsg;
	// 接口地址
	CEdit m_edURL;
	// 检测站编号
	CEdit m_edStationNumber;
	// 检测线编号
	CEdit m_edLineNumber;
	// 联网令牌
	CEdit m_edLincenseCode;

	// 车牌号码
	CLabel m_lbPlateNumber;
	// 车架号
	CLabel m_lbVIN;
	// 检测方法
	CLabel m_lbTestType;
	// OBD检测
	CLabel m_lbOBDItem;
	// 燃料类别
	CLabel m_lbFuelType;
	// 整备质量
	CLabel m_lbUnladenMass;
	// 总质量
	CLabel m_lbMaximumTotalMass;
	// 是否有OBD
	CLabel m_lbHasOBD;
	// 额定转速
	CLabel m_lbRatedRev;
	// 额定功率
	CLabel m_lbRatedPower;
	// 日志编号
	CLabel m_lbRunningNumber;

	CEdit m_edUpConfigName;
	CEdit m_edUpConfigProject;

private:
	// 日志文件
	void GetLogFilePath(void);
	CString m_strLogFilePath;

	// 获取配置信息
	void SetConfig(void);
	// OBD检测员
	void GetOBDJCY(void);
	// 获取联网令牌
	bool GetAccessToken(void);
	// 获取平台OBD检测员
	bool GetInterOBDJCY(void);
	// 获取平台OBD检测仪ID
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


	// 获取检测参数
	// 检测日志
	TESTLOG m_sTestLog;
	bool GetTestLog(void);
	CString ToTestType(const CString& strTestTypeCode);
	CString ToItem(const CString& strItemCode);

	// 设定界面信息
	void SetVehWin(const TESTLOG &sTestLog);
	

	// 保存INI，数据库和上传
	bool SaveINIAndSQLAndUP(void);

	// 获取车辆信息
	SOBDVehInfo m_sOBDVehInfo;
	bool GetIntVehInfo(void);
};
