
#pragma once

#include "Resource.h"
#include "YJSCheck.h"

// CNOxDlg �Ի���

class CNOxDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNOxDlg)

public:
	CNOxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNOxDlg();

// �Ի�������
	enum { IDD = IDD_NOX_DLG };

private:
	afx_msg void OnBnClickedButtonGasCal();
	afx_msg void OnBnClickedButtonGasChk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
private:
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ɾ��EquCalChkinfo.ini�ļ�
	static void DeleteEquCalChkInfoFile(void);
	// ���EquCalChkinfo.ini�ļ�
	// �����Ŀ��
	// 1)�Ƿ����
	// 2)�����Ƿ����
	static DWORD CheckEquCalChkInfoFile(void);
	// ����LineInfo.ini�ļ�
	// (in)strItem,Ҫ���µ���Ŀ
	// (int)strItemValue,��Ŀֵ
	DWORD UpdateLineInfoFile(const CString strItem, const CString strItemValue);
	// дEquCalChkInfo��
	static DWORD WriteEquCalChkInfo(void);
	// дLineInfo��
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// дLineInfo��
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// дLineInfo.ini�ļ�
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// �������������������
	static DWORD WriteAnaGasChkData(void);
	// �������������У׼����
	static DWORD WriteAnaGasCalData(void);
	// дDemarcationLog��
	static DWORD WriteDemarcationLog(void);
	// дDemarcationResultOfAnaGasCal��
	static DWORD WriteDemarcationResultOfAnaGasCal(void);
	// дDemarcationResultOfAnaGasChk��
	static DWORD WriteDemarcationResultOfAnaGasChk(void);
	// ����У׼���̿��ƻص�����
	static DWORD __stdcall AnaCalProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
	// ����У׼���̿��ƻص�����
	static DWORD __stdcall AnaChkProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);
private:
	// �궨�Ƿ�Pass
	static bool m_bDemarcationPass;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �Ƿ������ж� CO2��NO, NO2
	// ����ȫ����0���ϴ�
	static CString m_strJudgeMent[3];

	COleDateTime m_otdStart;
};
