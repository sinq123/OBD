
// SystemCheckDlg.h : ͷ�ļ�
//

#pragma once


// CSystemCheckDlg �Ի���
class CSystemCheckDlg : public CDialogZoom
{
// ����
public:
	CSystemCheckDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSystemCheckDlg();

// �Ի�������
	enum { IDD = IDD_SYSTEMCHECK_DIALOG };

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
	afx_msg void OnBnClickedBtnDailyDemarcation();
	afx_msg void OnBnClickedBtnEmissionTest();
	afx_msg void OnBnClickedBtnDataManagement();
	afx_msg void OnBnClickedBtnSystemManagement();
	afx_msg void OnBnClickedBtnSystemCheck();
	afx_msg void OnBnClickedBtnTrainingPresentation();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CInfoButton m_ibEmissionTest;
	CInfoButton m_ibDailyDemarcation;
	CInfoButton m_ibDataMangement;
	CInfoButton m_ibSystemConfig;
	CInfoButton m_ibTrainingPresentation;
	CInfoButton m_ibSystemCheck;
	CButtonST m_btnExit;
	// ���Ƽ��汾��ǩ
	CShadeStatic m_ssVersion;
	// ������
	CShadeTitle m_stTitle;
	// �ϻ�Logo
	CImageWnd m_iwNHLogo;
	// ����Logo
	CImageWnd m_iwEPLogo;
	// ����߱��+�豸��֤�����ʾ��
	CLabel m_lbLineNumberAndEquipmentLicense;
	// time
	CLabel m_lbTime;
	// ���ư汾��־
	CLabel m_lbSign;

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
	// ϵͳ����
	NHCLIENT m_sIniNHClient;
	// ��ǰϵͳʱ��
	SYSTEMTIME m_sCurrencyTime;

};
