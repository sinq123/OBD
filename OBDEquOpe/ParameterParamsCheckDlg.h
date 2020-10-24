
// ParameterParamsCheckDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "resource.h"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\Debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\Release\\OBDEquThr.lib" )
#endif

#include "..\OBDNHEuqController\NHEuqController.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHEuqController_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHEuqController.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CParameterParamsCheckDlg �Ի���
class CParameterParamsCheckDlg : public CDialogZoom
{
// ����
public:
	CParameterParamsCheckDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CParameterParamsCheckDlg();

// �Ի�������
	enum { IDD = IDD_PARAMETER_PARAMS_CHECK_DLG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
	// �����������߳�
	CParaThread *m_pParaThread;
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �ϱ���
	CShadeTitle m_lbTitle;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;
	// ����1
	CLabel m_lbLabel1;
	// ����2
	CLabel m_lbLabel2;
	// ����3
	CLabel m_lbLabel3;
	// ����4
	CLabel m_lbLabel4;
	// ����5
	CLabel m_lbLabel5;
	// �¶���ʾ��
	CLabel m_lbET;
	// ʪ����ʾ��
	CLabel m_lbRH;
	// ����ѹ��ʾ��
	CLabel m_lbAP;
	// ������ʾ��
	CLabel m_lbOT;
	// ת����ʾ��
	CLabel m_lbRev;

private:
	void RefreshData(void);
};
