/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHT6CalChkDlg.h
* �ļ���ʶ��
* ժҪ����͸��NHT6У׼�ͼ�飬�����2���׼�˹�Ƭ
*       Ŀǰ���Լ���NHT-1L�豸
*
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-07-13
*
* ��ʷ��Ϣ��
*
* �汾��1.0.15
* ���ڣ�2012-10-11
* ���ߣ�Raylee
* ��������Ӹ���FLB-100����ȪMQY-200�豸֧��
*
* �汾��1.0.14
* ���ڣ�2012-04-24
* ���ߣ�Raylee
* ���������FoFenFTY100֧��
*
* �汾��1.0.13
* ���ڣ�2011-01-19
* ���ߣ�Raylee
* ���������ԭʼNֵ�ͱ�׼��ͨ�����µ�Nֵͬʱ��ʾ
*
* �汾��1.0.12
* ���ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
*
* ��ǰ�汾��1.0.11
* ������ڣ�2010-05-21
* ���ߣ�Raylee
* ������1�������ص�����λ�ã���͸��У׼��顰��������Ϊ����ɡ�
*		2���ص���ʾ��wcscpy_s����wcsncpy_s
*
* ��ǰ�汾��1.0.10
* ������ڣ�2010-05-18
* ���ߣ�Raylee
* ������1�����ù��̿��ƻص�����������Ӧ����״̬
*		2��ԭ�������ݵĻص�����ɾ��
*
* ��ǰ�汾��1.0.9
* ʱ�䣺2010-05-12
* ���ߣ�Raylee
* �������궨��¼д��������ûص�����
*
* ��ǰ�汾��1.0.8
* ʱ�䣺2010-04-09
* ���ߣ�Raylee
* �������ĵ����궨��־�ͼ��������
*
* ��ǰ�汾��1.0.7
* ʱ�䣺2010-01-18
* ���ߣ�Cui
* ������1)����ɿ��Լ���NHT1L�豸
*
* ��ǰ�汾��1.0.6
* ʱ�䣺2010-01-14
* ���ߣ�Cui
* ������1)����Equipment.ini�ļ��ĸ��µ�������
*
* ��ǰ�汾��1.0.5
* ʱ�䣺2009-12-24
* ���ߣ�Cui
* ������1)�����ϴδ���ġ���ǰ�汾�š�
*       2)����ExportCalChkResult����:�����ʡ����ֵ��ͳһ�ĳɡ���׼ֵ������������ʽ����
*
* ��ǰ�汾��1.0.4
* ʱ�䣺2009-12-09
* ���ߣ�Cui
* ���������²�͸���߳�
*
* ʱ�䣺2009-11-25
* ���ߣ�Raylee
* ����������afx_msg void OnSysCommand(UINT nID, LPARAM lParam);λ��
*
* ��ǰ�汾��1.0.3
* ʱ�䣺2009-11-23
* ���ߣ�Raylee
* �������ĺ���ExportCalChkDetail���ѱ궨���ݵ�����ini
*
* ��ǰ�汾��1.0.2
* ʱ�䣺2009-11-20
* ���ߣ�Raylee
* �������ĺ���ExportCalChkResult���ѱ궨���ݱ�����Reserved1�ֶ�
*
* ��ǰ�汾��1.0.1
* ʱ�䣺2009-09-21
* ���ߣ�Cui
* �������������ϵ�m-1ʵ�����±���Ӿ�Ч��
*
* ��ǰ�汾��1.0.0
* ʱ�䣺2009-08-15
* ���ߣ�Cui
* �������ѡ��ճ��궨�����ʸ�Ϊ��У׼��顱������ʽʹ��
*
*
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


#include "resource.h"

// CNHT6CalChkDlg �Ի���

class CNHT6CalChkDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHT6CalChkDlg)

public:
	CNHT6CalChkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHT6CalChkDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHT6_CAL_CHK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

private:
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�begin
	// ����У׼������
	void ImportCalChkParams(void);
	// ������͸��У׼�����(�б궨��)
	void ExportEquCalChkInfo(void);
	// ������͸��׼���ʵʱ����
	void ExportCalibrationCheckRTData(void);
	// �����궨��־(�ڲ�����)
	void ExportDemarcationLog(void);
	// ������͸��У׼�����(�ڲ�����)
	void ExportOpaCalChkResult(void);
	//////////////////////////////////////////////////////////////// ���ݽ����ӿ�end

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
	// У׼�������
	void CalChkProcess(void);
	// ����У׼���
	void ResetCalChk(void); 
	// �˹�Ƭ1���
	void CalculateResult1(void);
	// �˹�Ƭ2���
	void CalculateResult2(void);
	// �˹�Ƭ3���
	void CalculateResult3(void);
	// ��ʾ�˹�Ƭ1���
	void ShowResult1(void);
	// ��ʾ�˹�Ƭ2���
	void ShowResult2(void);
	// ��ʾ�˹�Ƭ3���
	void ShowResult3(void);
	// ��ʾ����ж�
	void ShowJudgement(void);
	// ֹͣУ׼���
	void StopCalChk(void);
	// ��ȡ��׼�˹�Ƭʾֵ����
	// ����0x00,��׼�˹�Ƭʾֵ�������Ҫ��
	// ��������,��׼�˹�Ƭʾֵ���벻��Ҫ��
	DWORD GetStdValue(void);
	// �����׼�˹�Ƭʾֵ��ini�����ļ�
	void SaveStdValueToIniFile(void);
	// ���ʵʱ��ʾ��
	void ClearRTDisplay(void);

private:
	// ������
	CShadeTitle m_lbHead;

	// ��Ϣ��ʾ��
	CLabel m_lbInfo;

	// ��׼�˹�Ƭ1��ֵ�༭��
	CNumericEdit m_edN1;
	// ��׼�˹�Ƭ2��ֵ�༭��
	CNumericEdit m_edN2;
	// ��׼�˹�Ƭ3��ֵ�༭��
	CNumericEdit m_edN3;


	// 10�������б��
	CListCtrl m_lcData;

	// Nԭʼֵ��ʾ��
	CLabel m_lbN;
	// ��׼��ͨ�������µ�Nֵ
	CLabel m_lbNstd;
	// Kԭʼֵ��ʾ��
	CLabel m_lbK;

	// ƽ��ֵ1��ʾ��
	CLabel m_lbAve1;
	// ƽ��ֵ2��ʾ��
	CLabel m_lbAve2;
	// ƽ��ֵ3��ʾ��
	CLabel m_lbAve3;
	// ���Զ�1��ʾ��
	CLabel m_lbLinear1;
	// ���Զ�2��ʾ��
	CLabel m_lbLinear2;
	// ���Զ�3��ʾ��
	CLabel m_lbLinear3;

	// ���1��ʾ��
	CLabel m_lbErr1;
	// ���2��ʾ��
	CLabel m_lbErr2;
	// ���2��ʾ��
	CLabel m_lbErr3;
	// �ж���ʾ��
	CLabel m_lbJudgement;

	// ��ǩ1
	CLabel m_lb1;
	// ��ǩ2
	CLabel m_lb2;
	// ��ǩ3
	CLabel m_lb3;

	// ����1
	CLabel m_lbStep1;
	// ����2
	CLabel m_lbStep2;
	// ����3
	CLabel m_lbStep3;
	// ����4
	CLabel m_lbStep4;
	// ����5
	CLabel m_lbStep5;
	// ����5
	CLabel m_lbStep6;

	// ����1��ǩ
	CStatic m_stcStep1Label;
	// ����2��ǩ
	CStatic m_stcStep2Label;
	// ����3��ǩ
	CStatic m_stcStep3Label;
	// ����4��ǩ
	CStatic m_stcStep4Label;
	// ����5��ǩ
	CStatic m_stcStep5Label;
	// ����5��ǩ
	CStatic m_stcStep6Label;

	// ���±��ı��ؼ�
	CMathLabel m_mlb1;

private:
	// ��͸���߳�
	COpaThread *m_pOpaThread;

private:
	// ͨ�öԻ�������
	CFont m_fontDlgFont;
	// ͨ�öԻ��������С
	int m_nDlgFontSize;

private:
	// ����
	int m_nStep;
	// У׼��ʱ,��λ:��
	const int m_nCalculationTime;
	// ���������ֵ,��λ:0.01%,�Ŵ�100��,��������
	const int m_nAELimit;
	// ���Զ���ֵ,�Ŵ�100��,��������
	const int m_nLinearLimit;
	// ʱ��
	int m_nTime;
	// ��ȡ�˹�Ƭ1��ֵ
	bool m_bGetData1;
	// ��ȡ�˹�Ƭ2��ֵ
	bool m_bGetData2;
	// ��ȡ�˹�Ƭ3��ֵ
	bool m_bGetData3;
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;

private:
	// ��׼�˹�Ƭ1��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nStd1;
	// ��׼�˹�Ƭ2��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nStd2;
	// ��׼�˹�Ƭ3��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nStd3;

private:
	// �˹�Ƭ1����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nN1[20];
	// �˹�Ƭ2����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nN2[20];
	// �˹�Ƭ3����ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nN3[20];
	// �˹�Ƭ1����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nAve1;
	// �˹�Ƭ2����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nAve2;
	// �˹�Ƭ3����ֵƽ��ֵ,��λ:0.01%,�Ŵ�100��,��������
	int m_nAve3;
	// �˹�Ƭ1����ֵ���Զ�,��λ:0.01%,�Ŵ�100��,��������
	int m_nLinear1;
	// �˹�Ƭ2����ֵ���Զ�,��λ:0.01%,�Ŵ�100��,��������
	int m_nLinear2;
	// �˹�Ƭ3����ֵ���Զ�,��λ:0.01%,�Ŵ�100��,��������
	int m_nLinear3;
	// �˹�Ƭ1�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nAE1;
	// �˹�Ƭ2�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nAE2;
	// �˹�Ƭ3�������,��λ:0.01%,�Ŵ�100��,��������
	int m_nAE3;
	// �˹�Ƭ1���Զ��Ƿ�ϸ�
	bool m_bIsLinear1Pass;
	// �˹�Ƭ2���Զ��Ƿ�ϸ�
	bool m_bIsLinear2Pass;
	// �˹�Ƭ3���Զ��Ƿ�ϸ�
	bool m_bIsLinear3Pass;
	// �˹�Ƭ1��������Ƿ�ϸ�
	bool m_bIsAE1Pass;
	// �˹�Ƭ2��������Ƿ�ϸ�
	bool m_bIsAE2Pass;
	// �˹�Ƭ3��������Ƿ�ϸ�
	bool m_bIsAE3Pass;
	// �˹�Ƭ1����Ƿ�ϸ�
	bool m_bIsN1Pass;
	// �˹�Ƭ2����Ƿ�ϸ�
	bool m_bIsN2Pass;
	// �˹�Ƭ3����Ƿ�ϸ�
	bool m_bIsN3Pass;
	// У׼����Ƿ�ͨ��
	bool m_bIsCalChkPass;

private:
	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;
};
