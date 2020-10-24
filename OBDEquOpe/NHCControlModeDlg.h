/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCControlModeDlg.h
* �ļ���ʶ��
* ժҪ���⹦������ģʽ
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-11-22
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.7
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.6
* ���ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* �汾��1.0.5
* ���ڣ�2010-08-20
* ���ߣ�Qin
* ���������Ƽ������ģʽ�������
*
* �汾��1.0.4
* ���ڣ�2010-07-07
* ���ߣ�Cui
* ������1���������
*       2����Ӳ⹦��ͨѶЭ��ѡ����
*       3����������ĺ���ģʽ���������ʱɾ��
*
* �汾��1.0.3
* ���ڣ�2010-03-15
* ���ߣ�Cui
* ���������������ɡ�����ģʽ����Ϊ���⹦������ģʽ��
*
* �汾��1.0.2
* ���ڣ�2010-02-02
* ���ߣ�Cui
* �������涨Ŀ��ֵֻ���������ֺ�С����
*
* �汾��1.0.1
* ���ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* �汾��1.0.0
* ���ڣ�2009-11-22
* ���ߣ�Cui
* ��������ʽ����ʹ��
*
*
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

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CNHCControlModeDlg �Ի���

class CNHCControlModeDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCControlModeDlg)

public:
	CNHCControlModeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCControlModeDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_CONTROL_MODE };
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonConstantPowerOn();
	afx_msg void OnBnClickedButtonConstantForceOn();
	afx_msg void OnBnClickedButtonConstantVelocityOn();
	afx_msg void OnBnClickedButtonConstantVelocityOn2();
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedButtonControlModeOff();
	afx_msg void OnBnClickedCancel();

private:
	// ������
	CShadeTitle m_lbTitle;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;

	// ��ǩ1
	CLabel m_lb1;
	// ��ǩ2
	CLabel m_lb2;
	// ��ǩ3
	CLabel m_lb3;

	// ������ʾ��
	CLabel m_lbP;
	// ����ʾ��
	CLabel m_lbF;
	// �ٶ���ʾ��
	CLabel m_lbV;
	// �����������ٶ�
	float m_fNominalV;

	CNumericEdit m_nePValue;
	CNumericEdit m_neFValue;
	CNumericEdit m_neVValue;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// ˢ��ʵʱ����
	void EnableRefreshRealTimeData(bool bEnable=true);
	// ˢ��ʵʱ����
	void RefreshRealTimeData(void);
	// �����������
	void EnableBasicOperation(bool bEnable=true);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);

private:
	afx_msg LRESULT OnGetRealTimeData(WPARAM wParam, LPARAM lParam);

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �⹦���߳�
	CDynThread * m_pDynThread;

};
