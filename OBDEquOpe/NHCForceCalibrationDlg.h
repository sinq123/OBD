/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCForceCalibrationDlg.h
* �ļ���ʶ��
* ժҪ���⹦����У׼
*
* ��ǰ�汾��1.0
* ���ߣ�Raylee
* ������ڣ�2009-12-28
*
* 
* ��ʷ��Ϣ��
*
* �汾��1.0.12
* ���ڣ�2011-02-21
* ���ߣ�Raylee
* �����������������������������Ӧ�޸�
*
* �汾��1.0.11
* ���ڣ�2010-10-28
* ���ߣ�Raylee
* ��������֤����������������200N�ſ�����1000N
*
* �汾��1.0.10
* ���ڣ�2010-09-30
* ���ߣ�Qin
* �������Ľ�������ʾ������ɫ�ͷ��
*
* ��ǰ�汾��1.0.9
* ������ڣ�2010-09-27
* ���ߣ�Ming
* ��������ԭ��������ɫΪRGB(58, 248, 250)�Ŀؼ��ĳ���ɫRGB(0, 255, 0)
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
* ������1���ص���ʾ��У׼������Ϊ��У׼���
*		2���ص���ʾ��wcscpy_s����wcsncpy_s
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
* �������ĵ����궨��־�ͼ��������
*
* ��ǰ�汾��1.0.5
* ������ڣ�2010-05-12
* ���ߣ�Raylee
* ������д�궨��¼����ûص�����
*
*
* �汾��1.0.4
* ʱ�䣺2010-03-15
* ���ߣ�Cui
* ���������������ɡ���У׼����Ϊ���⹦����У׼��
*
* �汾��1.0.3
* ʱ�䣺2010-01-21
* ���ߣ�Raylee
* ������������Ϣ��"��У׼"����
*
* �汾��1.0.2
* ������ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* �汾��1.0.1
* ʱ�䣺2010-1-5
* ���ߣ�Raylee
* ������1����У׼�豸��ͳһΪ��У׼���ߡ�������ȥ��ͳһΪ�����ߡ�
*		2����ӱ�������Ƿ�ͨ��
*		3������m_StayF������Ϊm_fStayF
*		4���淶����
*
* �汾��1.0.0
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

// CNHCForceCalibrationDlg �Ի���

class CNHCForceCalibrationDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHCForceCalibrationDlg)

public:
	CNHCForceCalibrationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNHCForceCalibrationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NHC_FORCE_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonReset();

	DECLARE_MESSAGE_MAP()

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
    // �����ǰ���
	void ClearResult();   
	// �򿪴��ڲ�����ͨѶ
	void OpenSerialPortAndTestCommunication(void);

	///����������ͨѶ
	///@param bSucess ������ȡ��� 1�ɹ� 0ʧ��
	///@param lParam ��ȡ�ı�����
	LRESULT  OnReadVar(WPARAM bSucess, LPARAM lParam);

	// ������У׼���(�б궨��)
	//void ExportEquCalChkInfo(void);
	// ������У׼ʵʱ����
	void ExportDynForceCalibrationRTData(void);
	// ������У׼��¼(�ڲ�����)
	//void ExportDemarcationLog(void);
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
	// ��������
	CString m_strWeightCount;

private:
	// �⹦�������߳�
	CDynThread *m_pDynThread;

};
