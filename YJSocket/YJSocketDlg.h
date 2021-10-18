
// YJSocketDlg.h : 头文件
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
	std::wstring strInspectionNum; // 受理编码
	std::wstring strVLPN; // 车牌号码
	std::wstring strVLPNColor; // 车牌号码颜色
	std::wstring strVIN; //	车架号
	std::wstring strInspectionMethod;//	检测方法
	std::wstring strInspectionNature;//	检测类型 // 检测类型：01初检/02复检/03 多检
	std::wstring strHasOBD;//	是否带OBD系统
	std::wstring strOBDJCY; // OBD检测员
};

// CYJSocketDlg 对话框
class CYJSocketDlg : public CDialogEx
{
// 构造
public:
	CYJSocketDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CYJSocketDlg(void);

// 对话框数据
	enum { IDD = IDD_YJSOCKET_DIALOG };

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

	// 获取配置信息
	void SetConfig(void);
	void SetVehCtrl(void);
private:
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

public:
	// Json字符串转换格式
	bool Json2Map(LPCTSTR szJson, JsonMap &mapRet);
	CString Map2Json(JsonMap mapPost);
	CString Json2Cstring(Json::Value &value);
	std::map<std::wstring, std::wstring> m_mapTestType;
	// 是否需要
	// 认证\连接注册
	bool m_bAuthentication;
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	bool GetTime(void);
	// 待检列表
	bool GetVehicleList(void);
	
	SHJ03 m_sHJ03;
	

	bool SetSQL(const CString& strRunningNumber, const CString& strTestType);

	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	


	bool UpOBDStart(const CString& strInspectionNum, const CStringW& strStationNum, const CStringW& strLineNum);
};
