
// ACSCheckDlg.h : ͷ�ļ�
//

#pragma once


// CACSCheckDlg �Ի���
class CACSCheckDlg : public CDialogEx
{
// ����
public:
	CACSCheckDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CACSCheckDlg();
// �Ի�������
	enum { IDD = IDD_ACSCHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNha509();
	afx_msg void OnBnClickedBtnNht601();
};
