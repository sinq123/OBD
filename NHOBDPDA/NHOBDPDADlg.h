
// NHOBDPDADlg.h : ͷ�ļ�
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
///////////////////////////����ȫ�ֱ����Ķ���////////////////////////////////////////
#define BACKLOG (100)	// Max. request backlog 
#define MAX_THR (10) // Size of thread pool 
#define MAX_QUEUE (1000) // Max. size of request queue 
/////////////////////////////////////////////////////////////////////////////////////

// CNHOBDPDADlg �Ի���
class CNHOBDPDADlg : public CDialogEx
{
// ����
public:
	CNHOBDPDADlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NHOBDPDA_DIALOG };

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
	afx_msg void OnBnClickedBtnStartWebservice();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	void WriteLog(LPCTSTR szLog);
	CRichEditCtrl m_reLog;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ��������
	void LoadConfig(void);
	// �������
	void Reboot(void);

private:
	// �����߳�
	static UINT GSoapServerThread(LPVOID pParam);
	// ����WebService����
	void StartWebService(void);
	// ֹͣWebService����
	void StopWebService(void);

private:
	static int HttpGet(struct soap* soap);

private:
	static void* Process_Queue(void*);
	static int Enqueue(SOAP_SOCKET);
	static SOAP_SOCKET Dequeue();

public:
	// ����WebServiceURL
	char m_chGAURL[1024];

private:
	// WebService�˿�
	static int m_nWebServicePort;
	// WebService�����Ƿ�����
	static bool m_bWebServiceRun;
	
private:
	// soap����ӳ��
	static struct soap* m_mapSoap;

private:
	static SOAP_SOCKET m_Queue_Soap[MAX_QUEUE]; // The global request queue of sockets 
	static int m_nHead; // Queue head 
	static int m_nTail; // Queue tail 
	static pthread_mutex_t m_Queue_CS;
	static pthread_cond_t m_Queue_CV;

private:
	// ʱ��ID
	enum TimerID
	{
		TID_INIT_RUN = 1, // ��������
		TID_REBOOT = 2, // �������
	};
	// Timer�������λ��ms
	const static UINT m_unTI_InitRun = 500U;
	const static UINT m_unTI_Reboot = 60000U;

private:
	// ����ʱ��
	CStringW m_strRebootTime;

private:
	void SetOBDLOG(SResultOfOBD& sResultOfOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

	std::wstring UploadOBDDetData(const SResultOfOBD &sResultOfOBD);

	/* 
	   strDetType	�������
	*/
	void GetSimulationData(SOBDRTData &sOBDRTData, const CString& strDetType);

public:
	// ��ѯ����
	int QueryObjectOut(const std::wstring strJkid, const std::wstring strUTF8XmlDoc, std::wstring &strQOOR);
	// ����ӿ�
	// �ϴ�OBD�����
	void QueryObjectOut_W01(const std::wstring strUTF8XmlDoc, std::wstring &strXml);
	// �ϴ�OBDʵʱ������
	void QueryObjectOut_W02(const std::wstring strUTF8XmlDoc, std::wstring &strXml);
};
