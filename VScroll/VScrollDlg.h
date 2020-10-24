
// VScrollDlg.h : ͷ�ļ�
//

#pragma once


// CVScrollDlg �Ի���
class CVScrollDlg : public CDialogEx
{
// ����
public:
	CVScrollDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CVScrollDlg(void);
// �Ի�������
	enum { IDD = IDD_VSCROLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton* m_pBtns;

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
