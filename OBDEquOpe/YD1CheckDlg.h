/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�YD1CheckDlg.h
* �ļ���ʶ��
* ժҪ��YD1�ճ���鴰��
*
* ��ǰ�汾��1.0
* ���ߣ�hyh
* ������ڣ�2009��9��15��
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* ������1���ص���ʾ��������Ϊ������
*		2���Ի���ߴ��Ϊ755x455
*		3���ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* �汾��1.0.6
* ���ڣ�2010-05-12
* ���ߣ�Raylee
* �������궨��¼д��������ûص�����
*
* �汾��1.0.5
* ���ڣ�2010-04-16
* ���ߣ�Raylee
* ������1������ж��Ƿ�ϸ�ĳ�Ա����
*		2���ĵ����궨��־�ͼ��������
*
* �汾��1.0.4
* ���ڣ�2010-03-15
* ���ߣ�Cui
* ���������������ɡ��ظ��Ժ�ʾֵ����顱��Ϊ����ֽʽ�̶ȼ��ظ��Ժ�ʾֵ����顱
*
* �汾��1.0.3
* ���ڣ�2010-01-13
* ���ߣ�Cui
* ������1)����Equipment.ini��Demarcation.ini�ļ��ĸ��µ�������
*
* �汾��1.0.2
* ���ڣ�2009-12-24
* ���ߣ�Cui
* ������1)���������ϸ�ڣ����������궨����һ��
*       2)�����޸ģ��Թ���û��Ӱ�죬��1.0.1�汾����һ��
*
* �汾��1.0.1
* ���ڣ�2009-12-09
* ���ߣ�Cui
* �����������̶ȼ��߳�
*
* ʱ�䣺2009-11-25
* ���ߣ�Raylee
* ����������afx_msg void OnSysCommand(UINT nID, LPARAM lParam);λ��
*
* �汾��1.0.0
* ���ڣ�2009-09-15
* ���ߣ�hyh
* ��������ʽ����
*/

#pragma once

// �Զ���
#include "..\NHDetCommModu\NHDetCommModu.h"

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
#endif

// ���ÿ�
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include "resource.h"

// CYD1CheckDlg �Ի���

class CYD1CheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CYD1CheckDlg)

public:
	CYD1CheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CYD1CheckDlg();

// �Ի�������
	enum { IDD = IDD_YD1CHECK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();

	// ��Ӧ�ظ��Լ����Ϣ
	afx_msg LRESULT OnRepeatCheck(WPARAM wParam, LPARAM lParam);
	// ��Ӧʾֵ�������Ϣ
	afx_msg LRESULT OnRealCheck(WPARAM wParam, LPARAM lParam);

private:
	// ��ǩ
	CLabel m_lbLabel1;
	CLabel m_lbLabel2;
	CLabel m_lbLabel3;
	CLabel m_lbLabel4;

	CLabel m_lbRepeatError1;
	CLabel m_lbRepeatError2;
	CLabel m_lbRepeatError3;
	CLabel m_lbRepeatError4;
	CLabel m_lbRepeatError5;
	CLabel m_lbRepeatResult;

	CLabel m_lbBasicErr1;
	CLabel m_lbBasicErr2;
	CLabel m_lbBasicErrF;
	CLabel m_lbBasicErr5;
	CLabel m_lbBasicErr6;
	CLabel m_lbBasicResult;

	CNumericEdit m_edFSN1;
	CNumericEdit m_edFSN2;
	CNumericEdit m_edFSNCal;
	CNumericEdit m_edFSNF;
	CNumericEdit m_edFSN5;
	CNumericEdit m_edFSN6;

	CStatic m_stStep1;
	CLabel m_lbStep1;
	CStatic m_stStep2;
	CLabel m_lbStep2;

private:
	// ������
	CShadeTitle m_lbHead;
	// �����
	CLabel m_lbInfo;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ��������
	void ClearResult(void);
	// ��ʾ�ظ��Լ����
	void ShowRepeatCheckResult(void);
	// ��ʾʾֵ�������
	void ShowBasicErrorCheckResult(void);
	// Open�̶ȿ������
	void EnableFSNEdit(void);
	// �����̶ȿ�������
	void ImportFSNCheckParams(void);
	// �����̶ȿ�������
	void SaveFSNCheckParams(void);
	// ����У׼�����(�б궨��)
	void ExportEquCalChkInfo(void);
	// ����׼���ʵʱ����
	void ExportSmokeCheckRTData(void);
	// �����궨��־(�ڲ�����)
	void ExportDemarcationLog(void);
	// ����У׼�����(�ڲ�����)
	void ExportSmoChkResult(void);
	
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// �̶ȼ��߳�
	CSmoThread *m_pSmoThread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ��ǰ����
	short m_siStep;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;
	// �ظ��Լ���Ƿ�ͨ��
	bool m_bRepeatPass;
	// ʾֵ������Ƿ�ͨ��
	bool m_bBasicErrorPass;
	// ����Ƿ�ͨ��
	bool m_bPass;
};
