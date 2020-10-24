
// SXDRDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include <list>
#include "NSSocket.h"

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif


struct SOBDVEHLIST
{
	std::wstring strRunningNumber;
	std::wstring strPaletNumber;
	std::wstring strPlateType;
	std::wstring strVIN;
	std::wstring strVehType;
};
// CSXDRDlg �Ի���
class CSXDRDlg : public CDialogEx
{
// ����
public:
	CSXDRDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSXDRDlg(void);
// �Ի�������
	enum { IDD = IDD_SXDR_DIALOG };

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

	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpinter();

public:
	CEdit m_edServerIP;
	CEdit m_edServerPost;
	CEdit m_edLineNumber;
	CEdit m_edOBDJCY;
	CListCtrl m_lstVehicle;
	CLabel m_lbMessage;

	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);

public:
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv);

	// ��ȡ�б�
	bool GetVehList(void);
	
	void CloaseScket(void);

	SOBDVEHLIST m_sOBDVehList;
	
};
