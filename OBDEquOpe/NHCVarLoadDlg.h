/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCVarLoadDlg.h
* �ļ���ʶ��
* �������ϻ��⹦�����غɲ���(����JJF-1221��׼ִ��)
*
* �汾��1.0
* ���ߣ�Qin
* ���ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
* �汾��1.0.1
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.0
* ���ڣ�2010-06-08
* ���ߣ�Qin
* ��������ʽ����ʹ��
*/

#pragma once

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

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#include "resource.h"		// ������
#include "afxwin.h"
#include "NHCVarLoadThread.h"
// CNHCVarLoadDlg �Ի���

class CNHCVarLoadDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCVarLoadDlg)

public:
	CNHCVarLoadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCVarLoadDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_VAR_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// ������Ϣ
	CShadeTitle m_shdTitle;
	//��ʾ��Ϣ
	CLabel m_lbMsg;
	//��ʾ��ǰ�ٶ�
	CLabel m_lbSpeed;  
	//��ǰ������ʾ
	CLabel m_lbPower;
	//72.4~16.1����ʱ��
	CLabel m_lbTime2;
	//48.3~16.1����ʱ��
	CLabel m_lbTime3;
	//��ʾ72.4~16.1����ʱ��
	CLabel m_lbTime_Rel2;
	//��ʾ48.3~16.1����ʱ��
	CLabel m_lbTime_Rel3;  
	//��ʾ���ս��
	CLabel m_lbTime_Result; 
	//��ʾ72.4~16.1����ʱ�����׼ʱ�����
	CLabel m_lbTime_Err2;
	//��ʾ48.3~16.1����ʱ�����׼ʱ�����
	CLabel m_lbTime_Err3; 
	//��ʾ��������
	CLabel m_lbDIW;
	//��ʾʵʱ����
	CColorListCtrl m_ListReal;
	CButton m_CtrlRadio1;
	CImageList m_SmallImageList;

protected:
	afx_msg void OnBnClickedButtonStarttest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	// ��������;ASM,LUGDOWN,VMAS
	int m_TestType;
	//���ڲ��Ե��ٶȶ���
	int m_nStatues;                
	//���εļ��ع���
	float m_fPowerAdd[46];
	//�ж��ٶ��Ƿ��Ѿ�����88km/h,ֻҪ��һ�γ�������Ϊtrue
	bool  m_bIsSpeedUp; 
	//��������
	USHORT m_usDIW; 
	//72.4~16.1����ʱ��
	float m_fNominalTimeOfHigh;
	//48.3~16.1����ʱ��
	float m_fNominalTimeOfLow;
	//72.4~16.1ʵ��ʱ��
	float m_fRealityTimeOfHigh;
	//48.3~16.1ʵ��ʱ��
	float m_fRealityTimeOfLow;
	//72.4~16.1ʵ�����
	float m_fErrOfHigh;
	//48.3~16.1ʵ�����
	float m_fErrOfLow;
	//�����Ƿ�ϸ�
	bool m_bResult;
private:
	// �⹦���������ʿ����߳�
	CNHCVarLoadThread *m_pNHCVarLoadThread;

protected:
	///��ʼ���ؼ�״̬
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// ˢ������
	void RefreshRealTimeData(void);
	// �Ƿ�����ˢ������
	void EnableRefreshRealTimeData(bool bEnable=true);
	// ˢ�¹�������
	void DisplayVarLoadTestProcess(void);
	// ��ʾ���غɻ���ʱ��
	void DisplayVarLoadSldingTime(void);
	// ���غ�ASM����ʱ����ʾ
	void DisplayVarLoadSldingTime_ASM(void);
	// ���غ�VMAS����ʱ����ʾ
	void DisplayVarLoadSldingTime_VMAS(void);
	// ���غ�LUGDOWN����ʱ����ʾ
	void DisplayVarLoadSldingTime_LUGDOWN(void);
	// ������
	void CalculateVarLoadParams(void);
	// ��ʾ���
	void ShowResult(void);
};
