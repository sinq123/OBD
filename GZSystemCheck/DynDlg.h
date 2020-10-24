
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

// CDynDlg �Ի���

class CDynDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CDynDlg)

public:
	CDynDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDynDlg();

// �Ի�������
	enum { IDD = IDD_DYN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonConstLoad();
	afx_msg void OnBnClickedButtonPlhp();
	afx_msg void OnBnClickedButtonForceCalibration();
	afx_msg void OnBnClickedButtonForceCheck();
	afx_msg void OnBnClickedButtonControlMode();
	afx_msg void OnBnClickedButtonDiw();
	afx_msg void OnBnClickedButtonRespondTime();
	afx_msg void OnBnClickedButtonVarLoad();
	afx_msg void OnBnClickedButtonPlhpJjf();
	afx_msg void OnBnClickedButtonVarLoadJjf();

private:
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);

private:

private:
	// ɾ���ϴ�����ʱ�ļ�.ini�ļ�
	static void DeleteInfoFile(void);
	// дEquCalChkInfo.ini
	static DWORD WriteEquCalChkInfoFile(int nDemarcationItem);
	// дEquCalChkInfo��
	static DWORD WriteEquCalChkInfo(void);
	// дDemarcationLog.ini
	static DWORD WriteDemarcationLogFile(int nDemarcationItem);
	// дDemarcationLog��
	static DWORD WriteDemarcationLog(void);
	// дDemarcationResultOfDynConstLoad.ini
	static DWORD WriteDemarcationResultOfDynConstLoadFile(void);
	// дDemarcationResultOfDynConstLoad��
	static DWORD WriteDemarcationResultOfDynConstLoad(void);
	// дDemarcationResultOfDynPLHP.ini
	static DWORD WriteDemarcationResultOfDynPLHPFile(void);
	// дDemarcationResultOfDynPLHP��
	static DWORD WriteDemarcationResultOfDynPLHP(void);
	// дDemarcationResultOfDynForceCal.ini
	static DWORD WriteDemarcationResultOfDynForceCalFile(void);
	// дDemarcationResultOfDynForceCal��
	static DWORD WriteDemarcationResultOfDynForceCal(void);
	// дDemarcationResultOfDynForceChk.ini
	static DWORD WriteDemarcationResultOfDynForceChkFile(void);
	// дDemarcationResultOfDynForceChk��
	static DWORD WriteDemarcationResultOfDynForceChk(void);
	// дLineInfo��
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// дLineInfo��
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// дLineInfo.ini�ļ�
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// ������ػ�������
	static DWORD WriteDynConstLoadData(void);
	// ����������ʻ�������
	static DWORD WriteDynPLHPData(void);
	// ������У׼����
	static DWORD WriteDynForceCalData(void);
	// �������������
	static DWORD WriteDynForceChkData(void);

	// ���ػ��й��̿��ƻص�����
	static DWORD __stdcall DynConstLoadProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// �������ʻ��й��̿��ƻص�����
	static DWORD __stdcall DynPLHPProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// ��У׼���̿��ƻص�����
	static DWORD __stdcall DynForceCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// �������̿��ƻص�����
	static DWORD __stdcall DynForceChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
private:
	// �궨�Ƿ�Pass
	static bool m_bDemarcationPass;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �ϴ����ػ�������
	static bool UpConstLoad(void);
};
