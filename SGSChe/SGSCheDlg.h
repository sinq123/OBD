
// SGSCheDlg.h : ͷ�ļ�
//

#pragma once
#include "SelfInsrectionItemsDlg.h"
// CSGSCheDlg �Ի���
class CSGSCheDlg : public CDialogZoom
{
// ����
public:
	CSGSCheDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSGSCheDlg();
// �Ի�������
	enum { IDD = IDD_SGSCHE_DIALOG };

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
	afx_msg void OnBnClickedBtnStatr();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnRestatr();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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

	// 
	CLabel m_lb1;
	CLabel m_lb2;
	CLabel m_lb3;
	CLabel m_lb4;
	CLabel m_lb5;
	CLabel m_lb6;
	CLabel m_lb7;
	CLabel m_lb8;
	CLabel m_lb9;
	CLabel m_lb10;
	CLabel m_lb11;
	CLabel m_lb12;
	CLabel m_lb13;
	CLabel m_lb14;
	CLabel m_lb15;
	CLabel m_lb16;
	CLabel m_lb17;
	CLabel m_lb18;
	//
	CLabel m_lbNiuLi;
	CLabel m_lbGongLv;

	CLabel m_lbCO;
	CLabel m_lbHC;
	CLabel m_lbNO;
	CLabel m_lbCO2;
	CLabel m_lbO2;
	CLabel m_lbPEF;

	CLabel m_lbNO2;
	CLabel m_lbCO22;
	CLabel m_lbN;
	CLabel m_lbNs;
	CLabel m_lbLLJO2;
	CLabel m_lbLLJBZ;
	CLabel m_lbLLJSJ;
private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �Ƿ���󻰺�ȸ���
	void LoadSubDlg(void);
private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	HBRUSH m_hrPicture;

private:
	// ���վ��Ϣ
	CString m_strSelfInfoTimes;
public:
	

private:
	CSelfInsrectionItemsDlg m_SelfInsrectionItemsDlg;
};
