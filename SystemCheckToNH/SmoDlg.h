
#pragma once

#include "Resource.h"
#include "SystemCheckToNH.h"

// CSmoDlg �Ի���

class CSmoDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CSmoDlg)

public:
	CSmoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSmoDlg();

// �Ի�������
	enum { IDD = IDD_SMO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonSmo();

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
	// дEquCalChkInfo��
	static DWORD WriteEquCalChkInfo(void);
	// дDemarcationLog��
	static DWORD WriteDemarcationLog(void);
	// дDemarcationResultOfSmoChk��
	static DWORD WriteDemarcationResultOfSmoChk(void);
	// дLineInfo��
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// дLineInfo��
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// дLineInfo.ini�ļ�
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// ������ֽʽ�̶ȼƼ������
	static DWORD WriteSmoChkData(void);
	// �̶ȼƼ����̿��ƻص�����
	static DWORD __stdcall ProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);

private:
	// �궨�Ƿ�Pass
	static bool m_bDemarcationPass;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

};
