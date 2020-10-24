#pragma once


// CDieselVehicleEquipmentPrepareDlg �Ի���

class CDieselVehicleEquipmentPrepareDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CDieselVehicleEquipmentPrepareDlg)

public:
	CDieselVehicleEquipmentPrepareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDieselVehicleEquipmentPrepareDlg();

// �Ի�������
	enum { IDD = IDD_DIESEL_VEHICLE_EQUIPMENT_PREPARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnRetry();
	afx_msg void OnBnClickedBtnOpaCal();
	afx_msg void OnBnClickedBtnNOxZero();

private:
	CLabel m_lbInfo;
	CButton m_chkAdjust;
	CButton m_chkZero;
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

private:
	NHCLIENT m_sNHClient;
	LINEINFO m_sLineInfo;

	bool m_bOpaCal;
};
