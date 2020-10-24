
// SYDATADlg.h : 头文件
//

#pragma once

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

struct SCarInfo
{
	//1	检验机构编号	TsNo
	CString strTsNo;
	//2	检测线编号	TestLineNo
	CString strTestLineNo;
	//3	车辆ID	Clid
	CString strClid;
	//4	号牌	Hp
	CString strHp;
	//5	车牌颜色	Cpys
	CString strCpys;
	//6	变速器形式	Bxqxs
	CString strBxqxs;
	//7	基准质量	Jzzl
	CString strJzzl;
	//8	燃料种类	Rlzl
	CString strRlzl;
	//9	额定功率	Edgl
	CString strEdgl;
	//10	额定转数	Edzs
	CString strEdzs;
	//11	冲程	Cc
	CString strCc;
	//12	C0+Co2合值	CoCo2
	CString strCoCo2;
	//13	连续超差	Lxcz
	CString strLxcz;
	//14	累计超差	Ljcz
	CString strLjcz;
	//15	进气方式	Jqfs
	CString strJqfs;
	//16	是否OBD检测	OBD
	CString strOBD;

};

// CSYDATADlg 对话框
class CSYDATADlg : public CDialogEx
{
// 构造
public:
	CSYDATADlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSYDATADlg();
// 对话框数据
	enum { IDD = IDD_SYDATA_DIALOG };

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
	afx_msg void OnBnClickedBtnJumptestobd();
	afx_msg void OnBnClickedBtnUp();
private:
	// 车辆信息地址
	CEdit m_edVehInfoPath;

	CEdit m_edMsg;
	// 检验机构编号
	CLabel m_lbTsNO;
	// 号牌
	CLabel m_lbHP;
	// 车牌颜色
	CLabel m_lbCpys;
	// 燃料种类
	CLabel m_lbRlzl;
	// 进气方式
	CLabel m_lbJqfs;
	// 是否OBD检测
	CLabel m_lbTestOBD;

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

	SCarInfo m_sCarInfo;
private:
	_ConnectionPtr m_pConnection;

	bool OpenSQL(void);
	bool Close(void);
	bool m_bIsOKOpenDB;
private:
	// 获取配置信息
	void SetConfig(void);
	// OBD检测员
	void GetOBDJCY(void);

	// 获取车辆信息
	void GetVehXml(void);

	// 读取文件信息
	wstring FilePathToContent(const CString strPath);

	// 设定界面信息
	void SetVehWin(const SCarInfo& sCarInfo);

	// 获取文件路径
	bool GetFilePath(CString& strMsg, CString& strFilePath);

	// 设置跳过OBD检测
	void JumpTestOBD(void);

	// 写入OBD检测结果和TestLog修改参数
	void WriterResultOfOBDAndTestLog(void);

	// OBD数据部分
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

};
