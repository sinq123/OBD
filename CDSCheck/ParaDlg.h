
#pragma once

#include "Resource.h"
#include "CDSCheck.h"

// CParaDlg �Ի���

class CParaDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CParaDlg)

public:
	CParaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaDlg();

// �Ի�������
	enum { IDD = IDD_PARA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedButtonEnvParamsCal();

private:
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;


private:
	// �ϴ����������ص�����// �ϴ�ת�ٻص�����
	static DWORD __stdcall UpEnvParamsAndTachometerProcessCtrlCallBack(const DWORD dwStatus);

	// �ϴ���������
	static void UpEnv(void);

	// �ϴ�ת��
	static void UpTac(void);
};
