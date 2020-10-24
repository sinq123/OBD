#pragma once

#include "afxwin.h"
// CSelfInsrectionItemsDlg �Ի���

class CGripper : public CScrollBar
{
	// Construction
public:
	CGripper(){};
	// Implementation
public:
	virtual ~CGripper(){};
	// Generated message map functions
protected:
	//{{AFX_MSG(CGripper)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CSelfInsrectionItemsDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CSelfInsrectionItemsDlg)

public:
	CSelfInsrectionItemsDlg(UINT nIDD, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelfInsrectionItemsDlg();

// �Ի�������
	//enum { IDD = IDD_SELF_INSRECTION_ITEMS_DIALOG };
	void SetupScrollbars();
	void ResetScrollbars();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	int  m_nHorzInc, m_nVertInc,
		m_nVscrollMax, m_nHscrollMax,
		m_nVscrollPos, m_nHscrollPos;

	CBrush   m_transbrush;

	CRect m_ClientRect;
	BOOL m_bInitialized;
	CGripper m_Grip;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_scroll;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:

	CSxGroupBox m_grCGJ;
	CSxGroupBox m_grFQYXY;
	CSxGroupBox m_grFQYCGJC;

	CSxGroupBox m_grYDJJNOXFXY;
	CSxGroupBox m_grNOXFXY;
	CSxGroupBox m_grLLJ;
	CSxGroupBox m_grQXZ;
	CSxGroupBox m_grZSJ;

	CLabel m_lbQY;

	CLabel m_lbPLHPQY1;
	CLabel m_lbPLHPQY2;

	CLabel m_lbPLHPCY1;
	CLabel m_lbPLHPCY2;
	CLabel m_lbPLHPCY3;
	CLabel m_lbPLHPCY4;
	CLabel m_lbPLHPCY5;
	CLabel m_lbPLHPCY6;

	CComboBox m_cbQYPLHP;
	CComboBox m_cbCYPLHP;

	CEdit m_edFQYDDJCPEF;
	CEdit m_edFQYDDJCC3H8;
	CEdit m_edFQYDDJCHC;
	CEdit m_edFQYDDJCCO;
	CEdit m_edFQYDDJCCO2;
	CEdit m_edFQYDDJCNO;
	CEdit m_edFQYDDJCNO2;
	CEdit m_edFQYDDJCO2;

	CEdit m_edYDJJNOXFXYBZNLD;
	CEdit m_edYDJJNOXFXYBZNMLC;
	CEdit m_edYDJJNOXFXYBZNDBZ;
	CEdit m_edYDJJNOXFXYBZNGBZ;
	CEdit m_edYDJJNOXFXYKLD;
	CEdit m_edYDJJNOXFXYKMLC;
	CEdit m_edYDJJNOXFXYKDBZ;
	CEdit m_edYDJJNOXFXYKGBZ;

	CEdit m_edNOXFXYCO2;
	CEdit m_edNOXFXYNO;
	CEdit m_edNOXFXYNO2;

	CEdit m_edLLJMYLL;
	CEdit m_edLLJYQBZZ;
	CEdit m_edQXZWD;
	CEdit m_edQXZSD;
	CEdit m_edQXZDQY;

private:
	// ��ȡ����
	void LoadConfig(void);
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ���ù�����
	void SetScroll(void);
private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ��������
	CString m_strDIW;
	CString m_strPLHPQY[6];
	CString m_strPLHPCY[6];

	CString m_strPEF;
	CString m_strCO;
	CString m_strCO2;
	CString m_strNO;
	CString m_strNO2;
	CString m_strO2;
	CString m_strC3H8;

	CString m_strYDJJNOXFXYBZN[4];
	CString m_strYDJJNOXFXYK[4];

	CString m_strNOXFXYNO;
	CString m_strNOXFXYNO2;
	CString m_strNOXFXYCO2;

	CString m_strLLJMYLL;
	CString m_strLLJYQBZZ;
	CString m_strQXZWD;
	CString m_strQXZSD;
	CString m_strQXZDQY;
public:
	afx_msg void OnBtnFqyddjcKsjc();
	afx_msg void OnBtnNoxfxyKsjc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBtnFqycgjcKsjl();
	afx_msg void OnBtnYdjjnoxfxyQrDbz();
	afx_msg void OnBtnYdjjnoxfxyQrGbz();

private:
	// ��׼��C3H8���ֵ,��λ:ppm
	int m_nC3H8NominalValue;
	float m_fCONominalValue;
	float m_fCO2NominalValue;
	int m_nNONominalValue;
	int m_nNO2NominalValue;
	// ��׼��HC���ֵ,��λ:ppm,
	// ��C3H8���ֵ*PEF�õ�
	int m_nHCNominalValue;
	float m_fO2NominalValue;
	float m_fPEFNominalValue;

	short m_sRandomErrorHC;
	short m_sRandomErrorNO;
	short m_sRandomErrorNO2;
	short m_sRandomErrorNOX;
	float m_fRandomErrorCO;
	float m_fRandomErrorCO2;
	float m_fRandomErrorO2;
	float m_fRandomErrorPef;

	void RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx,  const int& nDetType);
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;

	// nDetType == 1 ����������
	// nDetType == 2 ����������
	bool GetNominalValue(const int& nDetType);

	// ��Ӧʱ��
	float m_fHCRespTime;
	float m_fCORespTime;
	float m_fCO2RespTime;
	float m_fNORespTime;
	float m_fNO2RespTime;
	float m_fO2RespTime;

	// ��Ӧʱ��Ҫ��
	float m_fHCT90Limit;
	float m_fHCT10Limit;
	float m_fCOT90Limit;
	float m_fCOT10Limit;	
	float m_fCO2T90Limit;
	float m_fCO2T10Limit;
	float m_fNOT90Limit;
	float m_fNOT10Limit;
	float m_fNO2T90Limit;
	float m_fNO2T10Limit;
	float m_fO2T90Limit;
	float m_fO2T10Limit;


	// nDetType == 1 ����������
	// nDetType == 2 ����������
	// �ϴ������̼�¼
	bool UpEqulChk(const int& njclx);

	// �ϴ���飨�ͱ�������¼
	bool UpGasCheck( const int& nDetType, const int& nQTLX, const CStringW& strJKID = L"13W05");
	// 2.16�ϴ������飨�߱�������¼13W07
	bool UpGasCheck13W07(void);

private:
	// ��͸��
	float m_fBTGDZBZZ;
	float m_fBTGDZCLZ;
	float m_fAE1;
	// �Ƿ�����һ�������ڼ�⣿
	bool m_bMIsType;

	bool UpGasCheck13W10(void);
public:
	afx_msg void OnBtnDownLifter();
	afx_msg void OnBtnUpLifter();

	// �����������
	float CalculatePLHP(const float fV);
	// ����CCTD
	// (return)���ۻ���ʱ��,��λ-ms
	UINT CalculateCCTD(const USHORT usDIW, const float fUpperVelocity, const float fLowerVelocity, const float fConstLoad);
	int RandomError(const UINT& m_unRangeCCDT);


	bool UpConstLoad(void);
	bool UpPLHP(void);
	bool UpConstLoadHeavy(void);
	bool UpPLHPHeavy(void);

	// �⹦���Ƿ��ϴ���һ�Σ�
	bool m_bUpConstLoad;
	bool m_bUpConstLoadHeavy;
};
