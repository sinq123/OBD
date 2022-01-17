
// YJSocketNDlg.h : 头文件
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

// CYJSocketNDlg 对话框
class CYJSocketNDlg : public CDialogEx
{
// 构造
public:
	CYJSocketNDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CYJSocketNDlg();
// 对话框数据
	enum { IDD = IDD_YJSOCKETN_DIALOG };

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
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnBnClickedBtnSocket();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnUpTelet();
private:
	// OBD检测员
	CComboBox m_cmOBDJCY;
	// 信息
	CEdit m_edMsg;
	// 车辆列表
	CListCtrl m_lstVehicle;
	// 数据源
	CEdit m_edCatalog;
	// 检测线
	CEdit m_edLineNumber;
	// 密码
	CEdit m_edPassWord;
	// 车牌号码
	CEdit m_edPlateNumber;
	// 联网socketIP
	CEdit m_edIP;
	// 联网Post
	CEdit m_edPort;
	// 检测站编号
	CEdit m_edStationNUmber;
	// 数据库名称
	CEdit m_edSource;
	// 登录名
	CEdit m_edUserID;

	CButton m_edchkSQL;
	CButton m_edIntet;
	CButton m_btnSocket;

	CEdit m_edCALID;
	CEdit m_edCVN;
	CEdit m_edOBDType;
public:
	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);
	// 待检列表
	bool GetVehicleList(void);
	
	
	// 认证\连接注册
	bool m_bAuthentication;
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	// 发送接收数据类
	CSocket m_yjSocket;
	// 生成唯一编码
	CString GenerateInsNum(void);
	// 发送接收数据包
	bool SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv);
	// 校验码
	// 计算前n位的校验和
	// 相加，然后取最低字节位
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	CString CheckSum(const char *ch, const UINT un);
	// 关闭连接
	void CloseConnect(void);
	// 时间
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
