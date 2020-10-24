
// JNSXDlg.h : ͷ�ļ�
//

#pragma once
#include <list>

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\JNSXInterfaceLib\JNSXInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\JNSXInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\JNSXInterfaceLib.lib")
#endif

struct SVehDate
{
	std::wstring strInspectID;
	std::wstring strPlateID;
	std::wstring strPlateType;
	std::wstring strFuelType;
	std::wstring strVIN;
	std::wstring strIsFirstInspect;
	std::wstring strWJResult;
};
// CJNSXDlg �Ի���
class CJNSXDlg : public CDialogEx
{
// ����
public:
	CJNSXDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_JNSX_DIALOG };

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
	CEdit m_edJKDZ;
	CAMSDateEdit m_edCXDate;
	CEdit m_edLineNumber;
	CEdit m_edOBDJXY;

	CListCtrl m_lstVehicle;

	CLabel m_lbMessage;

	// ��ȡ������Ϣ
	void SetConfig(void);
	void SetVehCtrl(void);

public:
	// �����б�
	bool GetVehicleList(void);

	SVehDate m_sVehDate;
};
