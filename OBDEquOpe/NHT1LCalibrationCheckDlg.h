/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHT1LCalibrationCheckDlg.h
* �ļ���ʶ��
* ժҪ����͸��NHT-1LУ׼�ͼ�飬�����2���׼�˹�Ƭ
*
*
* ��ǰ�汾��1.0
* ���ߣ�Raylee
* ������ڣ�2009-12-25
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.3
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* �汾��1.0.2
* ���ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.1
* ʱ�䣺2010-01-13
* ���ߣ�Cui
* ������1)����Equipment.ini�ļ��ĸ��µ�������
*
* ��ǰ�汾��1.0.0
* ʱ�䣺2009-12-30
* ���ߣ�Raylee
* ���������,����ʹ��
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

// ���ÿ�(MFC��չDLL)
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment( lib, "..\\debug\\OBDEquThr_D.lib" )
#else
#pragma comment( lib, "..\\release\\OBDEquThr.lib" )
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT


#include "resource.h"		// ������


// CNHT1LCalibrationCheckDlg �Ի���

class CNHT1LCalibrationCheckDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHT1LCalibrationCheckDlg)

public:
	CNHT1LCalibrationCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHT1LCalibrationCheckDlg();

// �Ի�������
	enum { IDD = IDD_NHT1LCALCHK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();

private:
	CShadeTitle m_lbTitle;	// �Ի���title
	CLabel m_lbInfo;		// ��Ϣ����
	CLabel m_lb1;			// ������,ʵʱԭʼֵ
	CLabel m_lb2;			// ������,�궨���
	CLabel m_lb3;			// ������,��͸��ȼ�У׼���
	CLabel m_lbN;			// ʵʱNֵ
	CLabel m_lbK;			// ʵʱKֵ
	CLabel m_lbStep1;		// ��鲽��1
	CLabel m_lbStep2;		// ��鲽��2
	CLabel m_lbStep3;		// ��鲽��3
	CLabel m_lbStep4;		// ��鲽��4
	CLabel m_lbStep5;		// ��鲽��5
	CLabel m_lbAvg1;		// �˹�Ƭ1����ƽ��ֵ
	CLabel m_lbAvg2;		// �˹�Ƭ2����ƽ��ֵ
	CLabel m_lbLinear1;		// �˹�Ƭ1���Զ�
	CLabel m_lbLinear2;		// �˹�Ƭ2���Զ�
	CLabel m_lbError1;		// �˹�Ƭ1ʾֵ���
	CLabel m_lbError2;		// �˹�Ƭ2ʾֵ���
	CLabel m_lbJudgement;	// ������ж�
	CListCtrl m_lstResult;	// ����б�
	CMathLabel m_mlK;		// Kֵ��λ,m-1�ϱ�
	CNumericEdit m_edStdN1;	// �˹�Ƭ1��׼ֵ
	CNumericEdit m_edStdN2;	// �˹�Ƭ2��׼ֵ

private:
	void InitCtrls(void);			// ��ʼ���ؼ�
	void SetDlgFont(void);			// ���öԻ�������
	void ClearResult(void);			// ����ϴ�У׼���
	void ExportCalChkDetail(void);	// ���������ϸ��Ϣ
	void ExportCalChkResult(void);	// ���������
	void CalcResult(void);			// ������
	void SaveStdNVal(void);			// �����׼ֵ
	void OpenSerialPortAndTestCommunication(void);			// �򿪴��ڲ�����ͨѶ

private:	
	CFont m_fontDlgFont;	// ͨ�öԻ�������
	int m_nDlgFontSize;		// ͨ�öԻ��������С
	int m_nTestStep;        // ��ʶ��ǰ���Բ���
	int m_nTime;            // ��ʱ��ʶ
	int m_nN1[24];			// 0~19Ϊʵʱֵ,20Ϊƽ��ֵ,21Ϊ���Զ�,22Ϊ�����˹�Ƭֵ150%��ռ�ı���,23Ϊʾֵ���	�Ŵ�100��,��������
	int m_nN2[24];			// 0~19Ϊʵʱֵ,20Ϊƽ��ֵ,21Ϊ���Զ�,22Ϊ�����˹�Ƭֵ150%��ռ�ı���,23Ϊʾֵ���	�Ŵ�100��,��������
	int m_nStdN1;			// ��׼�˹�Ƭ1��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nStdN2;			// ��׼�˹�Ƭ2��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nAELimit;			// ���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nLinearLimit;		// ���Զ���ֵ,�Ŵ�100��,��������

	bool m_bIsLinear1Pass;	// �˹�Ƭ1���Զ��Ƿ�ϸ�
	bool m_bIsLinear2Pass;	// �˹�Ƭ2���Զ��Ƿ�ϸ�
	bool m_bIsAE1Pass;		// �˹�Ƭ1��������Ƿ�ϸ�
	bool m_bIsAE2Pass;		// �˹�Ƭ2��������Ƿ�ϸ�
	bool m_bIsN1Pass;		// �˹�Ƭ1����Ƿ�ϸ�
	bool m_bIsN2Pass;		// �˹�Ƭ2����Ƿ�ϸ�
	bool m_bIsCalChkPass;	// У׼����Ƿ�ͨ��
	bool m_bIsOperationFinishedAtLeaseOnce;	// �����Ƿ��������1��

	enum						//Timer��ʶ
	{	TIMER_OPEN_COM = 1,		//�򿪴���
		TIMER_WAIT_FOR_CAL,		//�ȴ�У׼����
		TIMER_GET_STATUS,		//��ȡʵ״̬
		TIMER_GET_REAL_TIME_VAL,//��ȡʵʱֵ
		TIMER_MEASURE_N1,		//��ȡ�˹�Ƭ1����ֵ
		TIMER_MEASURE_N2		//��ȡ�˹�Ƭ2����ֵ
	};

	enum						//Step��ʶ
	{	STEP_PREPARE,			//����У׼ָ��
		STEP_INPUT_N,			//�����˹�Ƭʾֵ
		STEP_MEASURE_N1,		//�����˹�Ƭ1
		STEP_MEASURE_N2,		//�����˹�Ƭ2
		STEP_DONE				//���,������
	};

private:	
	COpaThread * m_pOpaThread;	// ��͸���߳�

};
