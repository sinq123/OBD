/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCVarLoadDlg2
* �ļ���ʶ��
* �������ϻ��⹦�����غɲ���(���չ���ִ��)
*
* �汾��1.0
* ���ߣ�hemincong
* ���ڣ�2010-09-29
*
* ��ʷ��Ϣ��
*
* �汾��1.0.1
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.0
* ���ڣ�2010-09-29
* ���ߣ�hemincong
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
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
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
#include "NHCVarLoadThread2.h"
// NHCVarLoadGBDlg �Ի���

class CNHCVarLoadDlg2 : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCVarLoadDlg2)

public:
	CNHCVarLoadDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCVarLoadDlg2();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_VAR_LOAD_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStarttest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedRadioCar();
	afx_msg void OnBnClickedRadioEng();

	DECLARE_MESSAGE_MAP()

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;
	// ������Ϣ
	CShadeTitle m_shdTitle;
	// �����Խ����label
	CLabel m_lbTip1;
	// ���������ݡ�label
	CLabel m_lbTip2;
	// ����ǰ�ٶȡ�label
	CLabel m_lbTip3;
	// ����ǰ���ʡ�label
	CLabel m_lbTip4;
	//��ʾ��Ϣ
	CLabel m_lbMsg;
	//��ʾ��ǰ�ٶ�
	CLabel m_lbSpeed;  
	//��ǰ������ʾ
	CLabel m_lbPower;
	//80.5~8.0����ʱ��
	CLabel m_lbTime2;
	//72.4~16.1����ʱ��
	CLabel m_lbTime3;
	//61.1~43.4����ʱ��
	CLabel m_lbTime4;
	//��ʾ80.5~8.0����ʱ��
	CLabel m_lbTime_Rel2;
	//��ʾ72.4~16.1����ʱ��
	CLabel m_lbTime_Rel3;  
	//��ʾ61.1~43.4����ʱ��
	CLabel m_lbTime_Rel4;
	//��ʾ���ս��
	CLabel m_lbTime_Result; 
	//��ʾ80.5~8.0����ʱ�����׼ʱ�����
	CLabel m_lbTime_Err2;
	//��ʾ72.4~16.1����ʱ�����׼ʱ�����
	CLabel m_lbTime_Err3; 
	//��ʾ61.1~43.4����ʱ�����׼ʱ�����
	CLabel m_lbTime_Err4; 
	//��ʾ��������
	CLabel m_lbDIW;
	//��ʾʵʱ����
	CColorListCtrl m_ListReal;
	//��������ѡ��
	CButton m_CtrlRadio1;
	CImageList m_SmallImageList;

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

private:
	//���غɻ��У�HJ/T290-2006�������߳�
	CNHCVarLoadThread2  *m_pNHCVarLoadThread2;

private:
	//��������
	int m_nAccType;
	//���ڲ��Ե��ٶȶ���
	int m_nStatues;                
	//���εļ��ع���
	float m_fPowerAdd[46];
	//�ж��ٶ��Ƿ��Ѿ�����88km/h,ֻҪ��һ�γ�������Ϊtrue
	bool  m_bIsSpeedUp; 
	//��������
	USHORT m_usDIW; 
	//80.5-8.0����ʱ��
	float m_fNominalTime1;
	//72.4-16.1����ʱ��
	float m_fNominalTime2;
	//61.4-43.4����ʱ��
	float m_fNominalTime3;
	//72.4~16.1ʵ��ʱ��
	float m_fRealityTime1;
	//48.3~16.1ʵ��ʱ��
	float m_fRealityTime2;
	//61.1-43.4ʵ��ʱ��
	float m_fRealityTime3;
	//80.5-8.0ʵ�����
	float m_fErr1;
	//72.4-16.1ʵ�����
	float m_fErr2;	
	//61.1-43.4ʵ�����
	float m_fErr3;
	//�����Ƿ�ϸ�
	bool m_bResult;
};
