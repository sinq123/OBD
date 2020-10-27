/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCPLHP2Dlg.h
* �ļ���ʶ��
* �������⹦���������ʻ��в���(����JJF-1221��׼ִ��)
*
* �汾��1.0
* ���ߣ�Qin
* ���ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.13
* ���ڣ�2012-09-28
* ���ߣ�wuyq
* �������ж��⹦�������������ٶ��ж�������ΪС�ڵ���10e-6
*
* �汾��1.0.12
* ���ڣ�2011-11-28
* ���ߣ�Raylee
* ����������û�е�������������bug
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
* �汾��1.0.1
* ���ڣ�2010-07-08
* ���ߣ�Cui
* ���������Ӷ������⹦��֧��
*
* �汾��1.0.0
* ���ڣ�2010-06-08
* ���ߣ�Qin
* ��������ʽ����ʹ��
*/
#pragma once

#include "resource.h"		// ������
#include "NHCPLHP2Thread.h"
#include "PLHPCurve.h"

// �Զ���
#include "..\NHDetCommModu\NHDetCommModu.h"

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


// CNHCPLHP2Dlg �Ի���

class CNHCPLHP2Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCPLHP2Dlg)

public:
	CNHCPLHP2Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCPLHP2Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_PLHP2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStartTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedRadioAsmRange();
	afx_msg void OnBnClickedRadioVmasRange();
	afx_msg void OnBnClickedRadioLugdownRange();

private:
	//////////////////////////////////////////////////////////////////////////////////////////////////�ؼ�����begin
	// ������
	CShadeTitle m_lbTitle;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;

	// ����������ʾ��
	CLabel m_lbDIW;

	// ��ǩ
	CLabel m_lb1;
	CLabel m_lb2;
	CLabel m_lb3;
	CLabel m_lb4;

	// ������ʾ��
	CLabel m_lbP;
	// ����ʾ��
	CLabel m_lbF;
	// �ٶ���ʾ��
	CLabel m_lbV;

	// Ap��ʾ��
	CLabel m_lbAp;
	// Bp��ʾ��
	CLabel m_lbBp;
	// Cp��ʾ��
	CLabel m_lbCp;

	// ��һAp��ʾ��
	CLabel m_lbAnotherAp;
	// ��һBp��ʾ��
	CLabel m_lbAnotherBp;
	// ��һCp��ʾ��
	CLabel m_lbAnotherCp;

	CButton m_rdoAsmRange;
	CButton m_rdoVmasRange;
	CButton m_rdoLugdownRange;

	// ��������б�
	CListCtrl m_lcResult;

	// ������ʾ��
	CPLHPCurve m_stcPLHPCurve;
	//////////////////////////////////////////////////////////////////////////////////////////////////�ؼ�����end

private:	
	// �����������ʲ��Խ��(�б궨��)
	//void ExportEquCalChkInfo(void);
	// �����������ʲ���ʵʱ����
	void ExportDynPLHPRTData(void);
	// �����������ʲ��Լ�¼(�ڲ�����)
	//void ExportDemarcationLog(void);
	// �����������ʲ��Խ��(�ڲ�����)
	void ExportDynPLHPResult(void);

	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �Ƿ�����ˢ������
	void EnableRefreshRealTimeData(bool bEnable=true);
	// ˢ������
	void RefreshRealTimeData(void);
	// �Ƿ������⹦����������
	void EnableBasicOperation(bool bEnable=true);
	// �ָ��ؼ�״̬
	void RestoreCtrls(void);
	// ��ʾ�������ʲ�������
	void DisplayPLHPTestProcess(void);
	// ��ʾ�������ʻ���ʱ��
	inline void DisplayPLHPSldingTime(void);
	inline void DisplayPLHPSldingTime_ASM(void);
	inline void DisplayPLHPSldingTime_VMAS(void);
	inline void DisplayPLHPSldingTime_LUGDOWN(void);
	// ����������ʻ��в���
	void CalculatePLHPParams(void);
	void CalculatePLHPParams_ASM(void);
	void CalculatePLHPParams_VMAS(void);
	void CalculatePLHPParams_LUGDOWN(void);
	// ��ʾ������������
	void ShowPLHPCurve(bool bShow=false);
	// ��ʾ��ʵ������������
	void ShowAnotherPLHPCurve(bool bShow=false);
	// д��������ʻ��в������⹦��
	void WritePLHPParamsToDyn(void);
	// ����������ʽ��,�����������ٶȵ�ļ�������ֵm_fPLHP,����ͼ,����,ListCtrl�ϵ���ֵ��
	void ClearPLHPResult(void);
	// ת��Ap��Bp��Cp����
	void ConvertApBpCp(bool bConvertToPower=true);
	// ��ʵAp��Bp��Cp����
	void ShowtApBpCp(void);

private:
	/// ��С���˷�
	// ��AP,BP,CP
	// (in)pdXArray:X����ָ��
	// (in)pdYArray:Y����ָ��
	// (in)nAmount:�����С
	// (out)pdResult:�������ָ��,����˳��,pdResult[0]-0�ײ���(����),�Դ�����
	// (in)grad:����,���ֻ��Ap,Bp,����Ϊ2,���Ҫ��Ap,Bp,Cp,����Ϊ3
	// (return)��
	static void Polynom_berechnen(double *const pdXArray, double *const pdYArray, const int nAmount, double *const pdResult, const int grad=3);

	// �����������
	// (in)fUpperVelocity:�ٶ�������ٵ㣬��λ-km/h
	// (in)fLowerVelocity:�ٶ�������ٵ㣬��λ-km/h
	// (in)unSlidingTime_ms:��Ӧ�ٶ����们��ʱ�䣬��λ-ms
	// (return)��������ֵ����λ-kW,����2λС��
	float CalculatePLHP(const float fUpperVelocity, const float fLowerVelocity, const UINT unSlidingTime);

	// �����������,��ʣ��������
	// (in)fUpperVelocity:�ٶ�������ٵ㣬��λ-km/h
	// (in)fLowerVelocity:�ٶ�������ٵ㣬��λ-km/h
	// (in)fSlidingTime:��Ӧ�ٶ����们��ʱ�䣬��λ-s
	// (in)fRemanence:ʣ��������λ-N
	// (return)��������ֵ����λ-kW
	//float CalculatePLHPEx(const float fUpperVelocity, const float fLowerVelocity, const float fSlidingTime, const float fRemanence);

private:
	// �������ʻ��в������
	afx_msg LRESULT OnPLHPTestFinished(WPARAM wParam, LPARAM lParam);
	// �������ʻ��в���ĳһ�ٶȶ����
	afx_msg LRESULT OnPLHPSectionFinished(WPARAM wParam, LPARAM lParam);
	inline void PLHPSectionFinished_ASM(UINT unSection);
	inline void PLHPSectionFinished_VMAS(UINT unSection);
	inline void PLHPSectionFinished_LUGDOWN(UINT unSection);

private:
	// �⹦���������ʿ����߳�
	CNHCPLHP2Thread *m_pNHCPLHP2Thread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// �������ʲ���ģʽ
	enum PLHPMode
	{
		// ƽ��ģʽ
		PLHP_NORMAL_MODE,
		// ��ģʽ
		PLHP_F_MODE,
		// ����ģʽ
		PLHP_P_MODE
	};

private:
	// ѡ�õļ������ʲ���ģʽ
	PLHPMode m_pmUsed;
	// ������������λ-kg
	USHORT m_usDIW;
	// �����ٶ�,��λ-km/h
    float m_fNomialVelocity[9];
	// ��������,��λ-kW
	float m_fPLHP[9];
	// ���ٵ�
	float m_fUpperVelocity[9];
	// ���ٵ�
	float m_fLowerVelocity[9];

	// ����ʣ�ŵļ������ʻ��в���(δת��Ϊ����ʱ����ֵ����δ����80.5ʱ�����ֵ)
	// �Ѿ���ȥʣ��
	double m_dApWithoutRemanence;
	double m_dBpWithoutRemanence;
	double m_dCpWithoutRemanence;
	// ��ʣ�ŵļ������ʻ��в���(δת��Ϊ����ʱ����ֵ����δ����80.5ʱ�����ֵ)
	// û�м�ȥʣ��
	double m_dApWithRemanence;
	double m_dBpWithRemanence;
	double m_dCpWithRemanence;

	// ����ʣ�ŵļ������ʻ��в���(ת��Ϊ����ʱ����ֵ��������80.5ʱ�����ֵ)
	// �Ѿ���ȥʣ��
	float m_fApWithoutRemanence;
	float m_fBpWithoutRemanence;
	float m_fCpWithoutRemanence;
	// ��ʣ�ŵļ������ʻ��в���(ת��Ϊ����ʱ����ֵ��������80.5ʱ�����ֵ)
	// û�м�ȥʣ��
	float m_fApWithRemanence;
	float m_fBpWithRemanence;
	float m_fCpWithRemanence;

private:
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;
};