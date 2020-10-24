#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

// COBDTestDlg 对话框
typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

class COBDTestDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(COBDTestDlg)

public:
	COBDTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDTestDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OBD_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonStartTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonFinish();
	afx_msg void OnBnClickedButtonNext();

public:
	// 标题栏
	CShadeTitle m_lbHead;
	// 信息提示栏
	CLabel m_lbInfo;
	// 车牌号码显示框
	CLabel m_lbPlateNumber;
	// VIN码显示框
	CLabel m_lbVIN;
	// 故障灯状态
	CLabel m_lbLampState;
	// 故障码
	CLabel m_lbDTC;
	// 故障里程
	CLabel m_lbDTCMileage;
	// 冻结帧数据
	CLabel m_lbFreezeData;
	// 判定
	CLabel m_lbJudge;
	// 检测项目
	CListCtrl m_lsItem;
	// 协议
	CComboBox m_cbProtocol;

	
	// 标签1
	CLabel m_lb1;
	// 标签2
	CLabel m_lb2;
	// 标签3
	CLabel m_lb3;
	// 标签4
	CLabel m_lb4;

	CLabel m_lbDynInfo;

public:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	
	// 列表控件字体，所有列表控件尽量使用此字体
	CFont m_fontListCtrl;
	// 位图列表，用于设置列表行高
	CImageList m_ilRowHeight;

	// 流水号
	CString m_strRunningNumber;
	// 车牌号码
	CString m_strPlateNumber;
	// 燃油类型
	CString m_strFuelType;
	// 操作员
	CString m_strOperator;
	// VIN码
	CString m_strVIN;


	std::vector<std::wstring> m_vtGasolineItem;
	std::map<std::wstring, DWORD> m_mapGasolineJudge;
	std::vector<std::wstring> m_vtDieselItem;
	std::map<std::wstring, DWORD> m_mapDieselJudge;

	// 操作提示
	void OperationHint(const CStringW& strOH);


public:
	bool m_bIsOBDRealTime;
	// 数据库连接
	_ConnectionPtr m_pConnection;

private:
	// Json模式
	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);
	CString JsonToCString(const rapidjson::Value &Val);

private:
	void StartItem(void);

	bool SetOBDLOG(SResultOfOBD& sResultOfOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	void SetTestLogAndVehDB(const CString& strItemOBD, const CString& strHasOBD, CString& strstrMsg);
	//联网部分
private:
	bool UPBeginEvent(void);
	bool UPEndEvent(void);
	bool UPPutOBDInspectData(const SResultOfOBD& sResultOfOBD);

};

