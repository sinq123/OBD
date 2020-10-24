
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

// CFloDlg �Ի���

class CFloDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CFloDlg)

public:
	CFloDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFloDlg();

// �Ի�������
	enum { IDD = IDD_FLO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonDilo2Cal();
	afx_msg void OnBnClickedButtonPresCal();
	afx_msg void OnBnClickedButtonFluxChk();
	afx_msg void OnBnClickedButtonFluxCal();
	afx_msg void OnBnClickedButtonDelayTimeCfg();
	afx_msg void OnBnClickedButtonFloCal();

private:
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);

private:
	static DWORD UpdateLineInfoFile(const CString strItem, const CString strItemValue);
	// дEquCalChkInfo��
	static DWORD WriteEquCalChkInfo(void);
	// дEquCalChkInfo.ini
	static DWORD WriteEquCalChkInfoFile(int nDemarcationItem);
	// дDemarcationLog��
	static DWORD WriteDemarcationLog(void);
	// дDemarcationLog.ini
	static DWORD WriteDemarcationLogFile(int nDemarcationItem);
	// дDemarcationResultOfFloFluxCal��
	static DWORD WriteDemarcationResultOfFloFluxCal(void);
	// дResultOfFloFluxCal.ini
	static DWORD WriteDemarcationResultOfFloFluxCalFile(void);
	// дDemarcationResultOfFloFluxChk��
	static DWORD WriteDemarcationResultOfFloFluxChk(void);
	// дResultOfFloFluxChk.ini
	static DWORD WriteDemarcationResultOfFloFluxChkFile(void);
	// дDemarcationResultOfFloDilO2Cal��
	static DWORD WriteDemarcationResultOfFloDilO2Cal(void);
	// дResultOfFloDilO2Cal.ini
	static DWORD WriteDemarcationResultOfFloDilO2CalFile(void);
	// дDemarcationResultOfFloPresCal��
	static DWORD WriteDemarcationResultOfFloPresCal(void);
	// дResultOfFloPresCal.ini
	static DWORD WriteDemarcationResultOfFloPresCalFile(void);
	// дLineInfo��
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// дLineInfo��
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// дLineInfo.ini�ļ�
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// ����ϡ����У׼����
	static DWORD WriteFloDilO2CalData(void);
	// ����ѹ��У׼����
	static DWORD WriteFloPresCalData(void);
	// ���������������
	static DWORD WriteFloFluxChkData(void);
	// ��������У׼����
	static DWORD WriteFloFluxCalData(void);
	// ����У׼���̿��ƻص�����
	static DWORD __stdcall FloFluxCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// ���������̿��ƻص�����
	static DWORD __stdcall FloFluxChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// ѹ��У׼���̿��ƻص�����
	static DWORD __stdcall FloPresCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// ϡ����У׼���̿��ƻص�����
	static DWORD __stdcall FloDilO2CalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
private:
	// �궨�Ƿ�Pass
	static bool m_bDemarcationPass;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

};
