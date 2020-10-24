/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCForceCheckHeavyDlg.h
* �ļ���ʶ��
* ժҪ�����Ͳ⹦�������
*
* ��ǰ�汾��1.0
* ���ߣ�Raylee
* ������ڣ�2010-01-11
*
* 
* ��ʷ��Ϣ��
*
* �汾��1.0.14
* ���ڣ�2012-01-30
* ���ߣ�Qin
* �������������������Ƿ���ʾ�⹦�����ĸ�������
*
* �汾��1.0.13
* ���ڣ�2011-01-10
* ���ߣ�Raylee
* �������������Ƶ������ini
*
* �汾��1.0.12
* ���ڣ�2011-01-05
* ���ߣ�Raylee
* ���������Ƶ������ini
*
* �汾��1.0.11
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.10
* ���ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-05-22
* ���ߣ�Raylee
* ������ע�͵���EquCalChkInfo.ini�Ĵ���
*		
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
* �汾��1.0.3
* ʱ�䣺2010-03-15
* ���ߣ�Cui
* ���������������ɡ�����顱��Ϊ���⹦������顱
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-01-21
* ���ߣ�Raylee
* ������1�����ʱֻ��¼һ�ݣ����ݿ�һ��PAU��Ӧһ������¼
*		2���Ŀؼ���λ����
*		3����Ϣ�ӡ�����顱����
*		4����������� Ĭ��ֵΪ500N��3��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* ��ǰ�汾��1.0.0
* ʱ�䣺2010-01-11
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
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

// CNHCForceCheckHeavyDlg �Ի���

class CNHCForceCheckHeavyDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCForceCheckHeavyDlg)

public:
	CNHCForceCheckHeavyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCForceCheckHeavyDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_FORCE_CHECK_HEAVY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedRadioPau();
	afx_msg void OnBnClickedButtonGetdata();
	afx_msg void OnBnClickedButtonCalculate();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CShadeTitle m_lbTitle;
	CLabel m_lbInfo;
	CLabel m_lbStep1;
	CLabel m_lbStep2;
	CLabel m_lbStep3;
	CLabel m_lbStep4;
	CLabel m_lbTorsion;
	CLabel m_lbStep;
	CLabel m_lbPAU;
	CLabel m_lbResult;
	CListCtrl m_lsData;
	CComboBox m_cbCkTime;
	CComboBox m_cbType;
	CComboBox m_cbCkData;

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
	// ��λPAU��ť״̬
	void ResetRadioPau(void);

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

	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;
	// ����Ƿ�ͨ��
	bool m_bIsPass[4];
	// �Ƿ���� 
	bool m_bIsChecked[4];
	// PAU��
	int m_nPAUCount;
	// ��ǰѡ���PAU
	int m_nCurPAU;

	enum						//Timer��ʶ
	{	TIMER_OPEN_COM = 1,		//�򿪴���
	TIMER_INSTALL_TOOL,		//��װУ׼����
	TIMER_ZERO_BALANCE,		//��λȡ��ƽ��
	TIMER_GET_RTDATA		//��ȡʵʱ����
	};
	enum						//Step��ʶ
	{	STEP_PREPARE,			//����У׼ָ��
	STEP_INTALL_TOOL,		//��װУ׼����
	STEP_RECORD_CHECK,		//������,��¼
	STEP_REMOVE_TOOL,		//����У׼����
	STEP_RESET_ZERO,		//��λ����
	STEP_RESET_ZERO_DONE,	//��λ�������
	STEP_DONE				//У׼���
	};
	enum						//��ǰѡ���PAU
	{	
		NOPAU = -1,
		PAU1,				
		PAU2,
		PAU3,
		PAU4
	};
	struct stPAU						//Ҫ��ʾ��PAU
	{
		bool bPAU1;				
		bool bPAU2;
		bool bPAU3;
		bool bPAU4;
	} m_stPAUShow;

private:
	// �⹦�������߳�
	CDynThread *m_pDynThread;

};