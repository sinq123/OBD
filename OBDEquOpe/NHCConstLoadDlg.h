/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCConstLoadDlg.h
* �ļ���ʶ��
* ժҪ���⹦�����ػ��в���
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-12-22
*
* ��ʷ��Ϣ��
*
* �汾��1.0.18
* ���ڣ�2014-07-10
* ���ߣ�Lihy
* �������޸�д��ResultOfDynConstLoad.ini�ļ�ʱ����������ֵ����д�뵽���������ֵ�ֶ��е�С����
*
* �汾��1.0.17
* ���ڣ�2012-09-28
* ���ߣ�wuyq
* �������ж��⹦�������������ٶ��ж�������ΪС�ڵ���10e-6
*
* �汾��1.0.16
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* ��ǰ�汾��1.0.15
* ������ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.14
* ������ڣ�2010-09-26
* ���ߣ�Ming
* ������1)���ʵʱ����������ʺ�����CalculatePLHP()
*       2)ʵʱ���ݵ����������Ӽ������ʺ�ָʾ����ֵ�ĵ���
*
* �汾��1.0.13
* ���ڣ�2010-07-07
* ���ߣ�Cui
* ��������Ӳ⹦��ͨѶЭ��ѡ����
*
* ��ǰ�汾��1.0.12
* ������ڣ�2010-07-07
* ���ߣ�Ming
* ������1)��Ӽ�¼ʵʱ���ݺ���AddRTData()
*       2)дʵʱ���ݵ�������
*
* ��ǰ�汾��1.0.11
* ������ڣ�2010-07-05
* ���ߣ�Ming
* �������������ʱ,��ӵ�����IsOperationFinishedAtLeaseOnce,�Ա�ʾ�����Ƿ���ɹ�����һ��.
*
* ��ǰ�汾��1.0.10
* ������ڣ�2010-06-25
* ���ߣ�Qin
* ����������CalculateCCTD�����������ۻ���ʱ�����
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-06-02
* ���ߣ�Raylee
* �����������������ʱ�׼ֵ
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-05-27
* ���ߣ�Raylee
* ������ֻ��������У׼�������
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-05-22
* ���ߣ�Raylee
* ������ע�͵���EquCalChkInfo.ini�Ĵ���
*
* ��ǰ�汾��1.0.6
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* �������ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.5
* ������ڣ�2010-05-20
* ���ߣ�Raylee
* ����������ini�궨��������Ϣ�����ⲿ�ϲ����ڲ���������LineInfo��UserInfo��StationInfo��
*
* ��ǰ�汾��1.0.4
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* ��ǰ�汾��1.0.3
* ������ڣ�2010-05-14
* ���ߣ�Raylee
* ������1��д�궨���ݸ��ûص�����
*		2���ĵ����궨��־�ͼ��������
*
* �汾��1.0.3
* ���ڣ�2010-04-10
* ���ߣ�Qin
* ������1��������״̬�ĸ�ѡ��ĳɺ�ϵͳ���һ����ͼ���ʾ��ʽ
*       2��������ԭ��������ػ�����ɺ�д����־���ֶ�Dynmometer����ΪDynamometer

* �汾��1.0.2
* ���ڣ�2010-03-15
* ���ߣ�Cui
* ������1�����������ɡ����ػ��С���Ϊ���⹦�����ػ��С�
*       2��������ʼ���ԡ���ť�͡�ֹͣ���ԡ���ť��Ϊ����ʼ����ť�͡�ֹͣ����ť
*
* �汾��1.0.1
* ���ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* �汾��1.0.0
* ���ڣ�2009-12-22
* ���ߣ�Cui
* ��������ʽ����ʹ��
*
*
*/

#pragma once

#include "resource.h"		// ������
#include "NHCConstLoadThread.h"

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


// CNHCConstLoadDlg �Ի���

class CNHCConstLoadDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCConstLoadDlg)

public:
	CNHCConstLoadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCConstLoadDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_CONST_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStartLowerRangeTest();
	afx_msg void OnBnClickedButtonStartHigherRangeTest();
	afx_msg void OnBnClickedButtonStartCustomRangeTest();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonLiftbeamUp();
	afx_msg void OnBnClickedButtonLiftbeamDown();
	afx_msg void OnBnClickedButtonMotorOn();
	afx_msg void OnBnClickedButtonMotorOff();
	afx_msg void OnBnClickedRadioAsmRange();
	afx_msg void OnBnClickedRadioVmasRange();
	afx_msg void OnBnClickedRadioLugdownRange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�ؼ�����begin
	// ������
	CShadeTitle m_lbTitle;
	// ��ʾ��Ϣ
	CLabel m_lbInfo;

	// ��ǩ
	CLabel m_lb1;
	CLabel m_lb2;
	CLabel m_lb3;
	CLabel m_lb4;

	// ������ʾ��
	CLabel m_lbP;
	// �ٶ���ʾ��
	CLabel m_lbV;

	// ����������ʾ�򣬵�λ:kg
	CLabel m_lbDIW;

	// �ܽ����ʾ��
	CLabel m_lbTotalResult;
	// ���ٶν����ʾ��
	CLabel m_lbLowerRangeResult;
	// ���ٶν����ʾ��
	CLabel m_lbHigherRangeResult;
	// �Զ�������ν����ʾ��
	CLabel m_lbCustomRangeResult;

	CButton m_rdoAsmRange;
	CButton m_rdoVmasRange;
	CButton m_rdoLugdownRange;
	CEdit m_edLowerRangePower;
	CEdit m_edHigherRangePower;
	CEdit m_edCustomRangePower;
	//CButton m_chkLowerRange;
	//CButton m_chkHigherRange;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�ؼ�����end

private:
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�begin
	// �������
	void ImportParams(void);
	
	// �������ػ��м����(�б궨��)
	//void ExportEquCalChkInfo(void);
	// �������ػ��м��ʵʱ����
	void ExportDynConstLoadRTData(void);
	// �������ػ��м���¼(�ڲ�����)
	//void ExportDemarcationLog(void);
	// �������ػ��м����(�ڲ�����)
	void ExportDynConstLoadResult(void);
	// �����ϴ��ļ����(�ϴ�����)
	void ExportUpDynConstLoadResult(void);
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�end

	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�begin
private:
	typedef DWORD (CALLBACK * PFProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen);
	PFProcessCtrl m_pfProcessCtrl;
public:
	void SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen));
	//////////////////////////////////////////////////////////////// ���̿��ƻص������ӿ�end

private:
	// ���ػ��в������
	afx_msg LRESULT OnConstLoadTestFinished(WPARAM wParam, LPARAM lParam);

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �Ƿ�����ˢ������
	void EnableRefreshRealTimeData(bool bEnable=true);
	// ����Icon״̬
	void SetStepIcon(UINT nStaticID, int nIconType);
	// ˢ������
	void RefreshRealTimeData(void);
	// �Ƿ�����⹦����������,������������������������������������ص��
	void EnableBasicOperation(bool bEnable=true);
	// �ָ��ؼ�״̬
	void RestoreCtrls(void);
	// ��ʾ���غɲ�������
	void DisplayConstantLoadTestProcess(void);
	// ����CCTD
	// (return)���ۻ���ʱ��,��λ-ms
	static UINT CalculateCCTD(const USHORT usDIW, const float fUpperVelocity, const float fLowerVelocity, const float fConstLoad);
	// ������ٶν��
	void ClearLowerRangeResult(void);
	// ������ٶν��
	void ClearHigherRangeResult(void);
	// ����Զ���ν��
	void ClearCustomRangeResult(void);
	// ��ʾ���Խ��
	void DisplayTestResult(void);
	// �����������
	float CalculatePLHP(const float fV);

private:
	// �⹦�������߳�
	CNHCConstLoadThread *m_pNHCConstLoadThread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ʵʱ��������ṹ
	struct RTData
	{
		// ʱ�����
		int nTime;
		// ��Ͳ���ٶ�,��λ:km/h
		float fVelocity;
		// ����,��λ:kW
		float fPower;
		// ��,��λ:N
		float fForce;
		// ��������,��λ��kW
		float fPLHP;
		// ָʾ����,��λ��kW
		float fIHP;

		RTData(){ZeroMemory(this, sizeof(RTData));}
	};	
	// ��������
	CList<RTData, RTData> m_lsRTDataOfDynConstLoad;
	// ���ʵʱ����
	void AddRTData(void);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////���Խ��begin
	// ������������λ-kg
	USHORT m_usDIW;
	// ���ٶμ��ع��ʣ���λ��kW
	float m_fLowerRangePower;
	// ���ٶμ��ع��ʣ���λ��kW
	float m_fHigherRangePower;
	// �Զ���μ��ع��ʣ���λ��kW
	float m_fCustomRangePower;
	// CCDT�����ػ���ʱ�����ֵ�����ٶΣ���λ-����
	UINT m_unLowerRangeCCDT;
	// ACCT�����ػ���ʱ��ʵ��ֵ�����ٶΣ���λ-����
	UINT m_unLowerRangeACDT;
	// CCDT�����ػ���ʱ�����ֵ�����ٶΣ���λ-����
	UINT m_unHigherRangeCCDT;
	// ACCT�����ػ���ʱ��ʵ��ֵ�����ٶΣ���λ-����
	UINT m_unHigherRangeACDT;
	// CCDT�����ػ���ʱ�����ֵ���Զ���Σ���λ-����
	UINT m_unCustomRangeCCDT;
	// ACCT�����ػ���ʱ��ʵ��ֵ���Զ���Σ���λ-����
	UINT m_unCustomRangeACDT;
	// ��������ֵ����λ-%�����ٶΣ��Ŵ�100�������磬7%����¼��700��
	UINT m_unLowerRangeRELimitX100;
	// ��������ֵ����λ-%�����ٶΣ��Ŵ�100�������磬7%����¼��700��
	UINT m_unHigherRangeRELimitX100;
	// ��������ֵ����λ-%���Զ���Σ��Ŵ�100�������磬7%����¼��700��
	UINT m_unCustomRangeRELimitX100;
	// ʵ��ֵ�������λ-%�����ٶΣ����Ŵ�100����
	int m_nLowerRangeREX100;
	// ʵ��ֵ�������λ-%�����ٶΣ����Ŵ�100����
	int m_nHigherRangeREX100;
	// ʵ��ֵ�������λ-%���Զ���Σ����Ŵ�100����
	int m_nCustomRangeREX100;
	// ������Խ�������ٶ�
	bool m_bIsLowerRangePass;
	// ������Խ�������ٶ�
	bool m_bIsHigherRangePass;
	// ������Խ�����Զ����
	bool m_bIsCustomRangePass;
	// ������ɹ�һ�β��ԣ����ٶ�
	bool m_bLowerRangeTestFinishedAtLeastOnce;
	// ������ɹ�һ�β��ԣ����ٶ�
	bool m_bHigherRangeTestFinishedAtLeastOnce;
	// ������ɹ�һ�β��ԣ��Զ����
	bool m_bCustomRangeTestFinishedAtLeastOnce;
	// �����Ƿ��������1��(���ٶκ͵��ٶζ�������ɹ�һ��)
	bool m_bIsOperationFinishedAtLeaseOnce;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////���Խ��end

private:
	// ��ǰʹ�õĵ��ٶλ�������ĵ��ٵ㣬��λ��km/h
	float m_fLowerVelocityOfLowerRange;
	// ��ǰʹ�õĵ��ٶλ�������ĸ��ٵ㣬��λ��km/h
	float m_fUpperVelocityOfLowerRange;
	// ��ǰʹ�õĸ��ٶλ�������ĵ��ٵ㣬��λ��km/h
	float m_fLowerVelocityOfHigherRange;
	// ��ǰʹ�õĸ��ٶλ�������ĸ��ٵ㣬��λ��km/h
	float m_fUpperVelocityOfHigherRange;
	// ��ǰʹ�õ��Զ���λ�������ĵ��ٵ㣬��λ��km/h
	float m_fLowerVelocityOfCustomRange;
	// ��ǰʹ�õ��Զ���λ�������ĸ��ٵ㣬��λ��km/h
	float m_fUpperVelocityOfCustomRange;
	// ����������ػ��в����Ƿ����ڽ���.     false:������,true:δ����
	bool  m_bLowerRangeLoadConstStep;
	// ����������ػ��в����Ƿ����ڽ���.     false:������,true:δ����
	bool  m_bHigherRangeLoadConstStep;
	// �Զ���������ػ��в����Ƿ����ڽ���.     false:������,true:δ����
	bool  m_bCustomRangeLoadConstStep;

private:
	// �������ʲ���
	float m_fAp;
	float m_fBp;
	float m_fCp;

private:
	// �������һ�����ֵ
	int m_nRandomError;
	int RandomError(const UINT& m_unRangeCCDT);
	// ����
	float m_fInternalDamage;
	// ��ʼʱ��
	bool m_bWriteTimer;
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;
	// Lugdown������ֵ
	void LugdownRangeACDT(void);
	UINT m_unLugdownRangeACDT30[9];
	UINT m_unLugdownRangeCCDT30[9];
	UINT m_unLugdownRangeACDT20[9];
	UINT m_unLugdownRangeCCDT20[9];
	UINT m_unLugdownRangeACDT10[9];
	UINT m_unLugdownRangeCCDT10[9];
	float m_fLugdownPLHP[9];
	// ����Ҫ�������4������
	UINT m_unGZLugdownRangeACDT[2];
	UINT m_unGZLugdownRangeCCDT[2];


	bool Precomputation(void);
};
