
// SGOBDDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

// CSGOBDDlg �Ի���
class CSGOBDDlg : public CDialogEx
{
// ����
public:
	CSGOBDDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SGOBD_DIALOG };

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

	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnEnChangeEdPlatenumber();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSetconfig();
	afx_msg void OnBnClickedBtnUpTelet();
	afx_msg void OnCbnSelchangeCbJcff();
	afx_msg void OnCbnSelchangeCbIsobdjc();
public:
	CEdit m_edSource;
	CEdit m_edCatalog;
	CEdit m_edUserID;
	CEdit m_edPassword;
	CEdit m_edPlateNumber;
	CEdit m_edMessAge;
	CEdit m_edURL;
	CEdit m_edKey;
	CEdit m_edStationNum;
	CEdit m_edLineNUmber;

	CListCtrl m_lstVehicle;
	CComboBox m_cbOBDJCY;
	CComboBox m_cbJCFF;
	CComboBox m_cbIsOBD;

	CDateTimeCtrl m_dtcBegTime;
	CDateTimeCtrl m_dtcEndTime;

public:
	SASM m_sASM;
	SJZJS m_sJZJS;
	SSDS m_sSDS;
	SZYJS_Btg m_sZYJS_Btg;
	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);
	// �����б�
	bool GetVehicleList(void);
	
	// ��ȡ�б���Ϣ
	CStringW m_strIDDB;
	CStringW m_strPlateNum;
	CStringW m_strJCFF;
	CStringW m_strVIN;
	// ����OBD�������
	bool SetOBDLOG(_ConnectionPtr &pConnection, SOBD& sOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	// IUPR
	// bool bFir true: ��һ�����ڵڶ�����flase �ڶ������ڵ�һ��
	void GetIUPR(int& nIUPR1, int& nIUPR2, float& fIUPR, const bool& bFir = true);

	// �ϴ�
	bool UPReport(_ConnectionPtr &pConnection, const SOBD& sOBD);
	bool UPRealTimeData(_ConnectionPtr &pConnection);
	bool UPOBDIUPR(_ConnectionPtr &pConnection, const SOBD& sOBD);

	// ASM OBD����
	void GetOBDTimeDataASM(CString& strOBDJQMJDKD, CString& strMMOBDJSFHZ, CString& strMMOBDQYCGQXH, CString& strMMOBDSPEED,
		CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDJQYL, CString& strMMOBDLAMBDA, const float& fSpeed, const int& nCou);
	// SDS OBD����
	void GetOBDTimeDataSDS(CString& strOBDJQMJDKD, CString& strMMOBDJSFHZ, CString& strMMOBDQYCGQXH, CString& strMMOBDSPEED,
		CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDJQYL, CString& strMMOBDLAMBDA, const int& nZS, const int& nCou);
	// JZJS OBD����
	//MMOBDYMKD->���ſ���
	//MMOBDSPEED->����
	//MMOBDPOWER->�������������
	//MMOBDROTATESPEED->ת��
	//MMOBDJQL->������
	//MMOBDZYYL->��ѹѹ��
	//MMOBDHYL->������
	//MMOBDNOND->����������Ũ��
	//MMOBDNSPSL->����������
	//MMOBDWD->�����¶�
	//MMOBDKLBJQYC->����������ѹ��
	//MMOBDEGRKD->EGR ����
	//MMOBDRYPSYL->ȼ������ѹ��
	void GetOBDTimeDataJZJS(CString& strMMOBDYMKD, CString& strMMOBDSPEED, CString& strMMOBDPOWER, CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDZYYL, 
		CString& strMMOBDHYL, CString& strMMOBDNOND, CString& strMMOBDNSPSL, CString& strMMOBDWD, CString& strMMOBDKLBJQYC, CString& strMMOBDEGRKD, CString& strMMOBDRYPSYL,
		const float& fSpeed, const int& nCou);
	// ZYJS OBD����
	void GetOBDTimeDataZYJS(CString& strMMOBDYMKD, CString& strMMOBDSPEED, CString& strMMOBDPOWER, CString& strMMOBDROTATESPEED, CString& strMMOBDJQL, CString& strMMOBDZYYL, 
		CString& strMMOBDHYL, CString& strMMOBDNOND, CString& strMMOBDNSPSL, CString& strMMOBDWD, CString& strMMOBDKLBJQYC, CString& strMMOBDEGRKD, CString& strMMOBDRYPSYL,
		const int& nZS, const int& nCou);
};
