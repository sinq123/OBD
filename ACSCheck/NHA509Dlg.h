#pragma once

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT


//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CNHA509Dlg �Ի���

class CNHA509Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA509Dlg)

public:
	CNHA509Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHA509Dlg();

	// �Ի�������
	enum { IDD = IDD_NHA_509LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedBtnInsSam();
	afx_msg void OnBnClickedBtnInsZero();
	afx_msg void OnBnClickedBtnInsReset();
	afx_msg void OnBnClickedBtnInsBlo();
	afx_msg void OnBnClickedBtnLinearDetection();
	afx_msg void OnBnClickedBtnLinearCheckOut();
	afx_msg void OnBnClickedBtnCheckInFive();
	afx_msg void OnBnClickedBtnCheckOutFive();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	void LoadConfig(void);
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	CString m_strConfigIsNH;
	CString m_strConfigPEF;
private:
	// ������
	CShadeTitle m_lbHead;
	// ��Ϣ��ʾ��
	CLabel m_lbInfo;

	// ���±��ı��ؼ�
	CMathLabel m_mlb1;
	CMathLabel m_mlb2;
	CMathLabel m_mlb3;
	CMathLabel m_mlb4;
	CMathLabel m_mlb5;
	CMathLabel m_mlb6;

	// HCʵʱֵ
	CLabel m_lbHC;
	// COʵʱֵ
	CLabel m_lbCO;
	// CO2ʵʱֵ
	CLabel m_lbCO2;
	// NOʵʱֵ
	CLabel m_lbNO;
	// O2ʵʱֵ
	CLabel m_lbO2;
	// PEFʵʱֵ
	CLabel m_lbPEF;
	// HC����ֵ��ʾ��
	CLabel m_lbHCMeasuredValue;
	// CO����ֵ��ʾ��
	CLabel m_lbCOMeasuredValue;
	// CO2����ֵ��ʾ��
	CLabel m_lbCO2MeasuredValue;
	// O2����ֵ��ʾ��
	CLabel m_lbO2MeasuredValue;
	// NO����ֵ��ʾ��
	CLabel m_lbNOMeasuredValue;
	// HC���������ʾ��
	CLabel m_lbHCAE;
	// CO���������ʾ��
	CLabel m_lbCOAE;
	// CO2���������ʾ��
	CLabel m_lbCO2AE;
	// O2���������ʾ��
	CLabel m_lbO2AE;
	// NO���������ʾ��
	CLabel m_lbNOAE;
	// HC��������ʾ��
	CLabel m_lbHCRE;
	// CO��������ʾ��
	CLabel m_lbCORE;
	// CO2��������ʾ��
	CLabel m_lbCO2RE;
	// O2��������ʾ��
	CLabel m_lbO2RE;
	// NO��������ʾ��
	CLabel m_lbNORE;
	// HC�ж���ʾ��
	CLabel m_lbHCJudgement;
	// CO�ж���ʾ��
	CLabel m_lbCOJudgement;
	// CO2�ж���ʾ��
	CLabel m_lbCO2Judgement;
	// O2�ж���ʾ��
	CLabel m_lbO2Judgement;
	// NO�ж���ʾ��
	CLabel m_lbNOJudgement;
	// HC���������ʾ��
	CLabel m_lbHCAE2;
	// CO���������ʾ��
	CLabel m_lbCOAE2;
	// CO2���������ʾ��
	CLabel m_lbCO2AE2;
	// O2���������ʾ��
	CLabel m_lbO2AE2;
	// NO���������ʾ��
	CLabel m_lbNOAE2;
	// HC��������ʾ��
	CLabel m_lbHCRE2;
	// CO��������ʾ��
	CLabel m_lbCORE2;
	// CO2��������ʾ��
	CLabel m_lbCO2RE2;
	// O2��������ʾ��
	CLabel m_lbO2RE2;
	// NO��������ʾ��
	CLabel m_lbNORE2;
	// IDC_COM_LINEAR_DETECTION
	CComboBox m_cbLinearDet;
	// IDC_COM_CHECK_IN_FIVE
	CComboBox m_cbChechInFive;
	// IDC_ST_TYPR_OF_EQUIPMENT
	CLabel m_lbTyprOfEqu;
	// C3H8���ֵ�༭��
	CEdit m_edC3H8NominalValue;
	// CO���ֵ�༭��
	CNumericEdit m_edCONominalValue;
	// CO2���ֵ�༭��
	CNumericEdit m_edCO2NominalValue;
	// NO���ֵ�༭��
	CEdit m_edNONominalValue;
	// HC���ֵ�༭��
	CEdit m_edHCNominalValue;
	// O2���ֵ�༭��
	CNumericEdit m_edO2NominalValue;

	// HC
	CLabel m_lbSJHC1;
	CLabel m_lbSJHC2;
	CLabel m_lbSJHC3;
	CLabel m_lbSJHC4;
	CLabel m_lbSJHC5;
	// CO
	CLabel m_lbSJCO1;
	CLabel m_lbSJCO2;
	CLabel m_lbSJCO3;
	CLabel m_lbSJCO4;
	CLabel m_lbSJCO5;
	// CO2
	CLabel m_lbSJCO21;
	CLabel m_lbSJCO22;
	CLabel m_lbSJCO23;
	CLabel m_lbSJCO24;
	CLabel m_lbSJCO25;
	// NO
	CLabel m_lbSJNO1;
	CLabel m_lbSJNO2;
	CLabel m_lbSJNO3;
	CLabel m_lbSJNO4;
	CLabel m_lbSJNO5;
	// O2
	CLabel m_lbSJO21;
	CLabel m_lbSJO22;
	CLabel m_lbSJO23;
	CLabel m_lbSJO24;
	CLabel m_lbSJO25;

	// IDC_STATIC_STATION
	CLabel m_lbStation;
private:
	// ���վ��Ϣ
	CString m_strStationName;
private:
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);

private:
	// ������Э��汾
	int m_nAnaProtocolVersion;
	// �������߳�
	CAnaThread *m_pAnaThread;
	// ��ȡ��������ֵ����
	// ����0x00,���ֵ�������Ҫ��
	// ��������,���ֵ���벻��Ҫ��
	DWORD GetNominalValue(void);

private:
	// ��׼��C3H8���ֵ,��λ:ppm
	int m_nC3H8NominalValue;
	// ��׼��CO���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCONominalValue;
	// ��׼��CO2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nCO2NominalValue;
	// ��׼��NO���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nNONominalValue;
	// ��׼��NO2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nNO2NominalValue;
	// ��׼��HC���ֵ,��λ:ppm,
	// ��C3H8���ֵ*PEF�õ�
	int m_nHCNominalValue;
	// ��׼��O2���ֵ,��λ:0.01%(�ŵ�100��)
	int m_nO2NominalValue;

	short m_sRandomErrorHC;
	short m_sRandomErrorNO;
	short m_sRandomErrorNO2;
	short m_sRandomErrorNOX;
	float m_fRandomErrorCO;
	float m_fRandomErrorCO2;
	float m_fRandomErrorO2;
	float m_fRandomErrorPef;

	void RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx);

	// ������
	bool CalculateResult(void);

	// PEF����ֵ,�Ŵ�1000��,��������
	int m_nPEFMeasuredValue;
	// HC����ֵ,��λ:ppm
	int m_nHCMeasuredValue;
	// CO����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOMeasuredValue;
	// CO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2MeasuredValue;
	// NO����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNOMeasuredValue;
	// NO2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nNO2MeasuredValue;
	// O2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nO2MeasuredValue;

	// HC�������,��λ:ppm
	int m_nHCAE;
	// CO�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCOAE;
	// CO2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nCO2AE;
	// NO�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nNOAE;
	// NO2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nNO2AE;
	// O2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nO2AE;

	// HC��������Ƿ�ͨ��
	bool m_bIsHCAEPass;
	// CO��������Ƿ�ͨ��
	bool m_bIsCOAEPass;
	// CO2��������Ƿ�ͨ��
	bool m_bIsCO2AEPass;
	// NO��������Ƿ�ͨ��
	bool m_bIsNOAEPass;
	// NO2��������Ƿ�ͨ��
	bool m_bIsNO2AEPass;
	// O2��������Ƿ�ͨ��
	bool m_bIsO2AEPass;

	// HC������,��λ:%
	int m_nHCRE;
	// CO������,��λ:%,�Ŵ�100��,��������
	int m_nCORE;
	// CO2������,��λ:%,�Ŵ�100��,��������
	int m_nCO2RE;
	// NO������,��λ:%,�Ŵ�100��,��������
	int m_nNORE;
	// NO2������,��λ:%,�Ŵ�100��,��������
	int m_nNO2RE;
	// O2������,��λ:%,�Ŵ�100��,��������
	int m_nO2RE;

	// HC�������Ƿ�ͨ��
	bool m_bIsHCREPass;
	// CO�������Ƿ�ͨ��
	bool m_bIsCOREPass;
	// CO2�������Ƿ�ͨ��
	bool m_bIsCO2REPass;
	// NO�������Ƿ�ͨ��
	bool m_bIsNOREPass;
	// NO2�������Ƿ�ͨ��
	bool m_bIsNO2REPass;
	// O2�������Ƿ�ͨ��
	bool m_bIsO2REPass;

	// HCͨ���Ƿ�ͨ��
	bool m_bIsHCPass;
	// COͨ���Ƿ�ͨ��
	bool m_bIsCOPass;
	// CO2ͨ���Ƿ�ͨ��
	bool m_bIsCO2Pass;
	// NOͨ���Ƿ�ͨ��
	bool m_bIsNOPass;
	// NO2ͨ���Ƿ�ͨ��
	bool m_bIsNO2Pass;
	// O2ͨ���Ƿ�ͨ��
	bool m_bIsO2Pass;
	// ����Ƿ�ͨ��
	bool m_bIsCheckPass;

	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;

private:
	bool UpIntwqfxybd(void);
};
