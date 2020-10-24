
// OBDOperationDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "resource.h"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\OBDEquThr_D.lib")
#else
#pragma comment(lib, "..\\release\\OBDEquThr.lib")
#endif

#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// COBDOperationDlg �Ի���
class COBDOperationDlg : public CDialogZoom
{
// ����
public:
	COBDOperationDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~COBDOperationDlg();

// �Ի�������
	enum { IDD = IDD_OBD_OPERATION_DLG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnVciVersion();
	afx_msg void OnBnClickedBtnVIN();
	afx_msg void OnBnClickedBtnEcuInfo();
	afx_msg void OnBnClickedBtnGetIupr();
	afx_msg void OnBnClickedBtnLampState();
	afx_msg void OnBnClickedBtnObdType();
	afx_msg void OnBnClickedBtnGetDtc();
	afx_msg void OnBnClickedBtnGetRtdata();
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedRadioFuelType();

private:
	// ��ʼ��OBD	
	LRESULT OnInit(WPARAM wParam, LPARAM lParam);

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ������־
	void WriteLog(LPCTSTR szLog);

private:
	COBDAdapterThread *m_pAdapterThread;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	
	// ��ʼ����ʱ
	UINT m_unInitTimeout;
	// ����/����
	int m_nFuelType;

private:
	// �ϱ���
	CShadeTitle m_lbTitle;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;
	// ���ص�Э��
	CLabel m_lbProtocol;
	// ����
	CLabel m_lbVelocity;
	// ת��
	CLabel m_lbRev;
	// ��ȴҺ�¶�
	CLabel m_lbCoolantTemp;
	// Э��
	CComboBox m_cbProtocol;

	CRichEditCtrl m_recLog;

};
