
#pragma once

#include "Resource.h"
#include "SystemCheck.h"

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// COpaDlg �Ի���

class COpaDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(COpaDlg)

public:
	COpaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpaDlg();

// �Ի�������
	enum { IDD = IDD_OPA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonOpa();

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
	// дLineInfo��
	static DWORD ReadLineInfo(LINEINFO &sLineInfo);
	// дLineInfo��
	static DWORD WriteLineInfo(LINEINFO &sLineInfo);
	// дLineInfo.ini�ļ�
	static DWORD WriteLineInfoFile(LINEINFO &sLineInfo);
	// ���治͸��У׼�������
	static DWORD WriteOpaCalChkData(void);
	// дDemarcationLog��
	static DWORD WriteDemarcationLog(void);
	// дDemarcationResultOfOpaCalChk��
	static DWORD WriteDemarcationResultOfOpaCalChk(void);
	// ��͸��У׼�����̿��ƻص�����
	static DWORD __stdcall ProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf = NULL, const LONG lBufLen = 0);

private:
	// �궨�Ƿ�Pass
	static bool m_bDemarcationPass;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;


};
