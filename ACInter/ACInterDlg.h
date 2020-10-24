
// ACInterDlg.h : ͷ�ļ�
//

#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif


#include "..\ACInterfaceLib\ACInterfaceLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\ACInterfaceLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\ACInterfaceLib.lib")
#endif

#include "..\NHCerModu\NHCerModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHCerModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHCerModu.lib")
#endif

struct SDJ001
{
	std::wstring strtsno;
	std::wstring strjylsh;
	std::wstring strtesttimes;
	std::wstring strlicense;
	std::wstring strlicensecode;
	std::wstring strtesttype;
	std::wstring strdlsj;
	std::wstring strzt;
};

struct SDL002
{
	std::wstring strjylsh;
	std::wstring strtesttimes;
	std::wstring strvin;
	std::wstring strfueltype;
};

// CACInterDlg �Ի���
class CACInterDlg : public CDialogEx
{
// ����
public:
	CACInterDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CACInterDlg(void);
// �Ի�������
	enum { IDD = IDD_ACINTER_DIALOG };

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
	afx_msg void OnBnClickedBtnUpinter();
	afx_msg void OnLvnItemchangedLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// ���쳵ListCtrl����
	CFont m_fontVehicleLCFont;

private:
	CEdit m_edJKDZ;
	CEdit m_edJCZBH;
	CEdit m_edJKXLH;
	CEdit m_edUPINTER;
	CEdit m_edDOWRINTER;
	CEdit m_edOBDJCY;

	CListCtrl m_lstVehicle;

private:
	// ˢ�´��쳵�б�
	void SetRefreshVehicleList(void);
	// ��ȡ������Ϣ
	void SetConfig(void);
	// ��ȡ������Ϣ
	void GetVehInfo(const CString strjylsh, const CString strtesttimes);

	//
	CString DecodeURI(LPCSTR szURIString);
	CString DecodeURI(LPCWSTR szURIString);


	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);
};
