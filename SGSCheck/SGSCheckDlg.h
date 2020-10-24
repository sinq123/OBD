
// SGSCheckDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSGSCheckDlg �Ի���
class CSGSCheckDlg : public CDialogZoom
{
// ����
public:
	CSGSCheckDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSGSCheckDlg();
// �Ի�������
	enum { IDD = IDD_SGSCHECK_DIALOG };

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
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
private:
	// �˳�
	CButtonST m_btnExit;
	// ��ʼ
	CButtonST m_btnState;
	// ���¿�ʼ
	CButtonST m_btnReState;
	// ֹͣ
	CButtonST m_btnStop;
	// �豸�Լ�
	CLabel m_lbEquSelfIns;
	// ���
	CLabel m_lbInterva;
	// ��©
	CLabel m_lbGasleakhunnting;
	// ��ǰʱ��
	CLabel m_lbCurrentTime;

	//
	CXPGroupBox m_BK1;
	CXPGroupBox m_BK3;
	CButtonST m_chkQYCCoustLoad;
	CButtonST m_chkCYCCoustLoad;
	CButtonST m_chkQYCPLHP;
	CButtonST m_chkCYCPLHP;

	CButtonST m_chkGasChech;
	CButtonST m_chkLowFlowChech;
	CButtonST m_chkSmokeMeter;
	CButtonST m_chkNOxAnalyzeer;
	CButtonST m_chkFlowMeter;
	CButtonST m_chkWetherStation;
	CButtonST m_chkTachoMerter;
	// ���ٱ�
	CMeter m_stSpeedometer;
	// ת�ٱ�
	CMeter m_stTachometer;
private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

};
