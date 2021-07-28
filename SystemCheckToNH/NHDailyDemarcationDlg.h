#pragma once

#include "DynDlg.h"
#include "OpaDlg.h"
#include "SmoDlg.h"
#include "AnaDlg.h"
#include "NOxDlg.h"
#include "FloDlg.h"
#include "ParaDlg.h"
#include "ObdDlg.h"

// CNHDailyDemarcationDlg �Ի���

class CNHDailyDemarcationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHDailyDemarcationDlg)

public:
	CNHDailyDemarcationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHDailyDemarcationDlg();

// �Ի�������
	enum { IDD = IDD_NH_DAILY_DEMARCATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedBtnClose();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls();
	// ���öԻ�������
	void SetDlgFont(void);
	// �����Ӵ���
	void LoadSubDlg(void);

private:
	// �궨�Ƿ�Pass
	bool m_bDemarcationPass;
	// �豸У׼�����Ϣ
	EQUCALCHKINFO m_sEquCalChkInfo;
	// �������Ϣ
	LINEINFO m_sLineInfo;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

	// �⹦���Ӵ���
	CDynDlg m_DynDlg;
	// ��͸��ȼ��Ӵ���
	COpaDlg m_OpaDlg;
	// �̶ȼ��Ӵ���
	CSmoDlg m_SmoDlg;
	// �������Ӵ���
	CAnaDlg m_AnaDlg;
	// �����������Ӵ���
	CNOxDlg m_NOxDlg;
	// �������Ӵ���
	CFloDlg m_FloDlg;
	// �����������Ӵ���
	CParaDlg m_ParaDlg;
	// OBD�Ӵ���
	CObdDlg m_ObdDlg;

	std::vector<int> m_vtID;
public:
	
};
