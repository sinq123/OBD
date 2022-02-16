
// HttpServerTestDlg.h : 头文件
//


#pragma once
#include "mongoose.h"

#include "..\CurlHttp\CurlHttp.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\CurlHttp_D.lib")
#else
#pragma comment(lib, "..\\Release\\CurlHttp.lib")
#endif

// CHttpServerTestDlg 对话框
class CHttpServerTestDlg : public CDialogEx
{
// 构造
public:
	CHttpServerTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTTPSERVERTEST_DIALOG };

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

private:
	CEdit m_edBootPort;
	CEdit m_edParentPort;
	CIPAddressCtrl m_IAParentIP;

	CRichEditCtrl m_reLog;

private:
	void PrintLog(LPCTSTR szFormat, ...);
	
private:
	// 加载配置
	void LoadConfig(void);
	// 保存配置
	void SetConfig(void);
	// 开启WebService服务
	void StartWebService(void);

private:
	// 启动端口
	static CString m_strBootPort;
	// 上级服务器IP
	static CString m_strParentIP;
	// 上级服务器端口
	static CString m_strParentPort;
private:
	// 单线程
	static int env_handler(struct mg_connection *conn);

	// 多线程
	static int request_handler(struct mg_connection *conn);
	static void *serve(void *server);

private:
	int CurlPost(const std::wstring strURl, const std::wstring strPost, std::wstring& strRet);

public:
	
};
