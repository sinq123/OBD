
// NHOBDPDADlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "pthread.h"
#include "sched.h"
#include "semaphore.h"
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib, "pthreadVCE2.lib")
#pragma comment(lib, "pthreadVSE2.lib")

#include "gsoap/soapH.h"


#include "..\NHDetCommModu\NHDetCommModu.h"
///////////////////////////宏与全局变量的定义////////////////////////////////////////
#define BACKLOG (100)	// Max. request backlog 
#define MAX_THR (10) // Size of thread pool 
#define MAX_QUEUE (1000) // Max. size of request queue 
/////////////////////////////////////////////////////////////////////////////////////

// CNHOBDPDADlg 对话框
class CNHOBDPDADlg : public CDialogEx
{
// 构造
public:
	CNHOBDPDADlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NHOBDPDA_DIALOG };

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
	afx_msg void OnBnClickedBtnStartWebservice();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	void WriteLog(LPCTSTR szLog);
	CRichEditCtrl m_reLog;

private:
	// 初始化控件
	void InitCtrls(void);
	// 加载配置
	void LoadConfig(void);
	// 重启软件
	void Reboot(void);

private:
	// 服务线程
	static UINT GSoapServerThread(LPVOID pParam);
	// 开启WebService服务
	void StartWebService(void);
	// 停止WebService服务
	void StopWebService(void);

private:
	static int HttpGet(struct soap* soap);

private:
	static void* Process_Queue(void*);
	static int Enqueue(SOAP_SOCKET);
	static SOAP_SOCKET Dequeue();

public:
	// 公安WebServiceURL
	char m_chGAURL[1024];

private:
	// WebService端口
	static int m_nWebServicePort;
	// WebService对象是否运行
	static bool m_bWebServiceRun;
	
private:
	// soap对象映射
	static struct soap* m_mapSoap;

private:
	static SOAP_SOCKET m_Queue_Soap[MAX_QUEUE]; // The global request queue of sockets 
	static int m_nHead; // Queue head 
	static int m_nTail; // Queue tail 
	static pthread_mutex_t m_Queue_CS;
	static pthread_cond_t m_Queue_CV;

private:
	// 时间ID
	enum TimerID
	{
		TID_INIT_RUN = 1, // 启动服务
		TID_REBOOT = 2, // 重启软件
	};
	// Timer间隔，单位：ms
	const static UINT m_unTI_InitRun = 500U;
	const static UINT m_unTI_Reboot = 60000U;

private:
	// 重启时间
	CStringW m_strRebootTime;

private:
	void SetOBDLOG(SResultOfOBD& sResultOfOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	std::wstring UploadOBDDetData(const SResultOfOBD &sResultOfOBD);

	/* 
	   strDetType	检测类型
	*/
	void GetSimulationData(SOBDRTData &sOBDRTData, const CString& strDetType);

public:
	// 查询处理
	int QueryObjectOut(const std::wstring strJkid, const std::wstring strUTF8XmlDoc, std::wstring &strQOOR);
	// 命令接口
	// 上传OBD检测结果
	void QueryObjectOut_W01(const std::wstring strUTF8XmlDoc, std::wstring &strXml);
	// 上传OBD实时数据流
	void QueryObjectOut_W02(const std::wstring strUTF8XmlDoc, std::wstring &strXml);
};
