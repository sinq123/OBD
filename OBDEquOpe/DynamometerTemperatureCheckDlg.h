
// DynamometerTemperatureCheckDlg.h : ͷ�ļ�
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

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CDynamometerTemperatureCheckDlg �Ի���
class CDynamometerTemperatureCheckDlg : public CDialogZoom
{
// ����
public:
	CDynamometerTemperatureCheckDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDynamometerTemperatureCheckDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_TEMPERATURE_CHECK };

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

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �⹦���߳�
	CDynThread *m_pDynThread;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �ϱ���
	CShadeTitle m_lbTitle;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;
	// ���������¶���ʾ��
	CLabel m_lbForceTemperature;
	// ����¶���ʾ��
	CLabel m_lbBearingTemperature;

private:
	// ����ˢ�±�־
	bool m_bRefreshDataSign;
	// ˢ������
	void RefreshData(bool bSign = true);
};
