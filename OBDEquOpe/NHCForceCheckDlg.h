/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCForceCheckDlg.h
* �ļ���ʶ��
* ժҪ���⹦�������
*
* �汾��1.0
* ���ߣ�Raylee
* ���ڣ�2009-12-28
*
* 
* ��ʷ��Ϣ��
*
* �汾��1.0.14
* ���ڣ�2011-01-10
* ���ߣ�Raylee
* �������������Ƶ������ini
*
* �汾��1.0.13
* ���ڣ�2011-01-05
* ���ߣ�Raylee
* ���������Ƶ������ini
*
* �汾��1.0.12
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.11
* ���ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* �汾��1.0.10
* ���ڣ�2010-07-08
* ���ߣ�Cui
* ������1����Ӳ⹦��ͨѶЭ��ѡ����
*       2��������ģ��NHWin32Lib�����ô���
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-05-22
* ���ߣ�Raylee
* ������ע�͵���EquCalChkInfo.ini�Ĵ���
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* �������ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-05-20
* ���ߣ�Raylee
* ����������ini�궨��������Ϣ�����ⲿ�ϲ����ڲ���������LineInfo��UserInfo��StationInfo��
*
* ��ǰ�汾��1.0.6
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* ��ǰ�汾��1.0.5
* ������ڣ�2010-05-14
* ���ߣ�Raylee
* ������1��д�궨���ݸ��ûص�����
*		2���ĵ����궨��־�ͼ��������
*
* �汾��1.0.4
* ʱ�䣺2010-03-15
* ���ߣ�Cui
* ���������������ɡ�����顱��Ϊ���⹦������顱
*
* ��ǰ�汾��1.0.3
* ʱ�䣺2010-01-21
* ���ߣ�Raylee
* ��������Ϣ�ӡ�����顱����
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* ��ǰ�汾��1.0.1
* ʱ�䣺2010-1-5
* ���ߣ�Raylee
* ������1����У׼�豸��ͳһΪ��У׼���ߡ�
*		2���Ľ��ж�����ʾ���棬�޸Ĳ�����ʾ
*		3��ɾ�������桱��ť
*		4���淶����
*		5���ѡ����㡱��Ϊ��ֹͣ��
*
* ��ǰ�汾��1.0.0
* ʱ�䣺2009-12-30
* ���ߣ�Raylee
* ���������,����ʹ��
*/

#pragma once

#include "resource.h"		// ������

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
#include "afxwin.h"
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

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CNHCForceCheckDlg �Ի���

class CNHCForceCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCForceCheckDlg)

public:
	CNHCForceCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCForceCheckDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_FORCE_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonGetdata();
	afx_msg void OnBnClickedButtonCalculate();
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

private:
	CLabel m_lbInfo;
	CLabel m_lbStep1;
	CLabel m_lbStep2;
	CLabel m_lbStep3;
	CLabel m_lbStep4;
	CLabel m_lbTorsion;
	CLabel m_lbStep;
	CLabel m_lbResult;
	CListCtrl m_lsData;
	CComboBox m_cbCkTime;
	CComboBox m_cbType;
	CComboBox m_cbCkData;
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ��ȡ�ַ�����ComboBox
	BOOL  StrToComb(CString &strTemp, CComboBox &combTemp);
	// ��ռ������
	void  ClearResult();                                     
	// �����������
	void  ClearStep();                                       
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);

	///����������ͨѶ
	///@param bSucess ������ȡ��� 1�ɹ� 0ʧ��
	///@param lParam ��ȡ�ı�����
	LRESULT  OnReadVar(WPARAM bSucess, LPARAM lParam);        //����������ͨѶ

	// �����������(�б궨��)
	//void ExportEquCalChkInfo(void);
	// ���������ʵʱ����
	void ExportDynForceCheckRTData(void);
	// ����������¼(�ڲ�����)
	//void ExportDemarcationLog(void);
	// �����������(�ڲ�����)
	void ExportDynForceCheckResult(void);

	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
    // ���Բ���
	int m_nTestStep; 
	// ��λ�ȶ�ʱ��                
	int m_nZeroTime;
	// ��ʱ����,����ini���ֵ�趨
	CString m_strWeight;
	// ��Ǽ���Ƿ�ͨ��
	bool m_bIsPass;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
	// �⹦�������߳�
	CDynThread *m_pDynThread;

};
