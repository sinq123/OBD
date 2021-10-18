#pragma once


// COBDTestDlg �Ի���

class COBDTestDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(COBDTestDlg)

public:
	COBDTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDTestDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_OBD_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonStartTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonFinish();
	afx_msg void OnBnClickedButtonNext();

private:
	// ������
	CShadeTitle m_lbHead;
	// ��Ϣ��ʾ��
	CLabel m_lbInfo;
	// ���ƺ�����ʾ��
	CLabel m_lbPlateNumber;
	// VIN����ʾ��
	CLabel m_lbVIN;
	// ���ϵ�״̬
	CLabel m_lbLampState;
	// ������
	CLabel m_lbDTC;
	// �������
	CLabel m_lbDTCMileage;
	// ����֡����
	CLabel m_lbFreezeData;
	// �ж�
	CLabel m_lbJudge;
	// �����Ŀ
	CListCtrl m_lsItem;
	// Э��
	CComboBox m_cbProtocol;

	
	// ��ǩ1
	CLabel m_lb1;
	// ��ǩ2
	CLabel m_lb2;
	// ��ǩ3
	CLabel m_lb3;
	// ��ǩ4
	CLabel m_lb4;

	CLabel m_lbMsgInfo;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	
	// �б��ؼ����壬�����б��ؼ�����ʹ�ô�����
	CFont m_fontListCtrl;
	// λͼ�б������������б��и�
	CImageList m_ilRowHeight;

	std::vector<std::wstring> m_vtGasolineItem;
	std::map<std::wstring, DWORD> m_mapGasolineJudge;
	std::vector<std::wstring> m_vtDieselItem;
	std::map<std::wstring, DWORD> m_mapDieselJudge;

private:
	void StartItem(void);

private:
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
	int randEx(void);


private:
	void SetTestLogAndVehDB(const CString& strItemOBD, const CString& strHasOBD, CString& strstrMsg);



public:
	// ���ݿ�����
	_ConnectionPtr m_pConnection;
	// 
	TESTLOG m_sTestLog;
	// 
	VEHICLEINFO m_sVehInfo;
};