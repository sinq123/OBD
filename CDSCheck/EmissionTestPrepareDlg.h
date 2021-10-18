#pragma once

#include "DieselVehicleEquipmentPrepareDlg.h"
#include "GasolineVehicleEquipmentPrepareDlg.h"
#include "GasolineVehicleEquipmentPrepareNDlg.h"
#include "afxdtctl.h"

// CEmissionTestPrepareDlg �Ի���

class CEmissionTestPrepareDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CEmissionTestPrepareDlg)

public:
	CEmissionTestPrepareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEmissionTestPrepareDlg();

// �Ի�������
	enum { IDD = IDD_EMISSION_TEST_PREPARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnCbnSelchangeCbFilterTestType();
	afx_msg void OnEnChangeEdFilter();
	afx_msg void OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnBnClickedButtonLiftBeamUp();
	afx_msg void OnBnClickedButtonLiftBeamDown();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnChangeVehicleInfo();
	afx_msg void OnBnClickedBtnLambdaLimit();
	
private:
	CListCtrl m_lstVehicle;
	CComboBox m_cbFilterTestType;
	CComboBox m_cbDriver;
	CEdit m_edFilter;
	CShadeTitle m_lbHead;
	CShadeTitle m_lbVehicleList;
	CShadeTitle m_lbVehicleInfo;
	CShadeTitle m_lbEParamInfo;
	CShadeTitle m_lbAcceptanceTime;

	//
	CDateTimeCtrl m_dtcAcceptanceStartTime;		// ��ʼ���ڱ���
	CDateTimeCtrl  m_dtcAcceptanceEndTime;		// �������ڱ���

	// �⹦����Ϣ��
	CLabel m_lbDynInfo;
	CLabel m_lbEPPortInfo;

	// ������Ϣ
	CComboBox m_cbTestType;
	CComboBox m_cbPlateType;
	CComboBox m_cbDriveType;
	CComboBox m_cbFuelType;
	CComboBox m_cbOilSupplyMode;
	CComboBox m_cbAirIntakeMode;
	CComboBox m_cbHasCatalyticConverter;
	CEdit m_edPlateNumber;
	CEdit m_edOwner;
	CEdit m_edVIN;
	CEdit m_edModel;
	CEdit m_edIsRetest;
	CEdit m_edUnladenMass;
	CDateTimeCtrl m_dtcProductionDate;
	CDateTimeCtrl m_dtcRegistrationDate;
	CNumericEdit m_edMaximumTotalMass;
	CNumericEdit m_edRatedPower;
	CNumericEdit m_edRatedRev;
	CEdit m_edItemApp;
	CEdit m_edItemOBD;
	CEdit m_edItemEmi;
	CNumericEdit m_edLambdaMin;
	CNumericEdit m_edLambdaMax;

	//��������
	CEdit m_edET;
	CEdit m_edRH;
	CEdit m_edAP;
	CEdit m_edOT;
	CEdit m_edRPM;
	
	// ����Ա
	CString m_strDriver;
	// ���ݿ�����
	_ConnectionPtr m_pConnection;

private:
	// �������ݿ�
	DWORD InitDatabase(void);
	// �ر����ݿ�����
	DWORD UnInitDatabase(void);
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ��ʼ��������б�
	void InitVehicleList(void);

	// ��ʼ��������Ϣ��ʾ����
	void InitVehicleInfoRegion(BOOL Enable=FALSE);
	// ��ʼ��TestType Combobox
	void InitTestTypeCombobox(void);
	// ��ʼ������Ա�б�
	void InitDriverCombobox(void);
	// ��ʼ����������
	void InitComboboxPlateType(void);
	// ��ʼ��������ʽ
	void InitComboboxDriveType(void);
	// ��ʼ��ȼ������
	void InitComboboxFuelType(void);
	// ��ʼ�����ͷ�ʽ
	void InitComboboxOilSupplyMode(void);
	// ��ʼ��������ʽ
	void InitComboboxAirIntakeMode(void);
	// ��ʼ���豸׼������
	void InitEquipmentPrepareRegion(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// ���쳵ListCtrl����
	CFont m_fontVehicleLCFont;

	// ϵͳ����
	NHCLIENT m_sIniNHClient;

	wchar_t m_wchPathOfNHClient[MAX_PATH];

private:
	// ���ͳ��豸׼��
	CGasolineVehicleEquipmentPrepareNDlg m_cGasolineVehicleEquipmentPrepareNDlg;
	// ���ͳ��豸׼��
	CDieselVehicleEquipmentPrepareDlg m_cDieselVehicleEquipmentPrepareDlg;
	// �豸׼�������б�
	short m_siPrepareRegionIndex;
	// ���ط������豸׼������
	void LoadGasolineVehicleEquipmentPrepare(void);
	// ���ز�͸���豸׼������
	void LoadDieselVehicleEquipmentPrepare(void);

private:
	// ��ǰѡ�г�����ˮ��
	CString m_strCurrentRunningNumber;
	// ��ǰѡ�г�����������
	CString m_strCurrentTestType;
	
private:
	//��ȡ��������
	void GetEPModeSimu(void);
	// ��ȡ������Ϣ
	void GetIntVehInfoList(void);
	// ��ȡ������Ϣ
	void GetIntVehInfo(void);

private:
	TESTLOG m_sTestLog;
	VEHICLEINFO m_sVehInfo;
};
