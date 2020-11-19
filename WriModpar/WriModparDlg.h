
// WriModparDlg.h : ͷ�ļ�
//

#pragma once


// CWriModparDlg �Ի���
class CWriModparDlg : public CDialogEx
{
	// ����
public:
	CWriModparDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CWriModparDlg();
	// �Ի�������
	enum { IDD = IDD_WRIMODPAR_DIALOG };

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// ���쳵ListCtrl����
	CFont m_fontVehicleLCFont;

private:
	// �ؼ��洰�ڱ仯���仯
	CPoint m_Old;//��ŶԻ���Ŀ�͸�

private:
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedLstPending(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpVehicle();
	afx_msg void OnBnClickedBtnUpPending();

private:
	CListCtrl m_lstVehicle;
	CListCtrl m_lstPending;

	CButtonST m_btnUpVehicle;
	CButtonST m_btnUpPending;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ��ʼ��������б�
	void InitVehicleList(void);

private:

	char* m_pchURL;
	CStringW m_strURL;
	CStringW m_strStationNum;
	CStringW m_strLineNum;
	CStringW m_strDetectLSBH;
	COleDateTime m_odtDemarcationDate;

	// ������־�ļ�
	CStringW m_strIntLogFilePath;
	void GetLogFilePath(void);

	// ��ȡ������Ϣ
	void GetConfig(void);

	CString DecodeURI(LPCSTR szURIString);
	CString DecodeURI(LPCWSTR szURIString);

private:
	// ��ȡƽ̨�����б�
	void GetIntVehList(void);
	// ��ȡ�������б�
	void GetPending(void);

	bool SaveToDB(void);
	bool DetWhiModPar(void);
};
