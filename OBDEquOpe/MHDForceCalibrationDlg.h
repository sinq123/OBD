/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�MHDForceCalibrationDlg.h
* �ļ���ʶ��
* ժҪ���⹦����У׼(����⹦��ר��)
*       ��Ϊ���ϻ��⹦����У׼�ǳ���һ�������Ե�������һ��ģ��
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2010-07-08
*
* 
* ��ʷ��Ϣ��
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
* �汾��1.0.0
* ʱ�䣺2010-07-08
* ���ߣ�Cui
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

// CMHDForceCalibrationDlg �Ի���

class CMHDForceCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CMHDForceCalibrationDlg)

public:
	CMHDForceCalibrationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMHDForceCalibrationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MHD_FORCE_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonReset();

private:
	CLabel m_lbInfo;
	CLabel m_lbTorsion;
	CLabel m_lbStep1;
	CLabel m_lbStep2;
	CLabel m_lbStep3;
	CLabel m_lbStep4;
	CLabel m_lbStep5;
	CLabel m_lbStep6;
	CLabel m_lbStep;
	CLabel m_lbReset;
	CShadeTitle m_lbTitle;

private:
	// ��ʼ���ؼ�
	void InitCtrls(void);
	// ���öԻ�������
	void SetDlgFont(void);

private:
    // �����ǰ���
	void ClearResult();   
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);
	// �Ƿ�����ˢ������
	void EnableRefreshRealTimeData(bool bEnable=true);
	// ˢ������
	void RefreshRealTimeData(void);

	// ������У׼ʵʱ����
	void ExportDynForceCalibrationRTData(void);
	// ������У׼���(�ڲ�����)
	void ExportDynForceCalibrationResult(void);

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
	// У׼ʱ�������ȶ�ʱ��
	int m_nStayTime;	
	// �������5�����ʵʱֵ
	float m_fStayF[5];
	// ��У׼����
	float m_fCalForce;	
	// �����Ƿ��������1��
	bool m_bIsOperationFinishedAtLeaseOnce;
	// У׼�Ƿ�ͨ��
	bool m_bIsPass;

private:
	// �⹦�������߳�
	CDynThread *m_pDynThread;
public:
	afx_msg void OnBnClickedCancel();
};
