#pragma once


// CGasolineVehicleEquipmentPrepareDlg �Ի���

class CGasolineVehicleEquipmentPrepareDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CGasolineVehicleEquipmentPrepareDlg)

public:
	CGasolineVehicleEquipmentPrepareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGasolineVehicleEquipmentPrepareDlg();

// �Ի�������
	enum { IDD = IDD_GASOLINE_VEHICLE_EQUIPMENT_PREPARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnRetry();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnAnaZero();
	afx_msg void OnBnClickedBtnAnaChkEc();
	afx_msg void OnBnClickedBtnAnaChkRc();
	afx_msg void OnBnClickedBtnFloChkO2();
	afx_msg void OnClose();

private:
	CLabel m_lbInfo;
	CButton m_chkZero;
	CButton m_chkEC;
	CButton m_chkRC;
	CButton m_chkFlowmeterCheckO2;
	CShadeTitle m_lbHead;
	CLabel m_lbProbeTips;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

	NHCLIENT m_sNHClient;

	
	// 
	bool m_bRet;
	bool m_bFlo;
};
