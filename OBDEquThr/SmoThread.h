/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�SmoThread.h
* �ļ���ʶ��
* ժҪ���̶ȼ��߳�
*
* ��ǰ�汾��1.0.1
* ���ߣ�hyh
* ������ڣ�2010-01-14
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.2
* ���ڣ�2012-04-26
* ���ߣ�Raylee
* ���������FoFenFBY201֧��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-01-14
* ���ߣ�hyh
* ��������OnRepeatCheck��OnRealCheck��Ӳ��λ��Ϊ��λ
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaYd1.h"
#include "FoFenFBY201.h"

// CSmoThread

class AFX_EXT_CLASS CSmoThread : public CWinThread
{
	DECLARE_DYNCREATE(CSmoThread)

protected:
	CSmoThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSmoThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()

public:
	// �����ͺ�
	enum InstrumentModel
	{
		// YD1
		IM_NANHUA_YD1,
		// �����ֽʽ�̶ȼ�
		IM_FOFEN_FBY201
	};

public:
	//ͬ������
		// �򿪴���
	 DWORD Open(const BYTE bPort, const enum InstrumentModel im);

	// �رմ���
	DWORD Close(void);

	// �жϴ����ǻ��
	inline bool IsOpen(void) const
	{
		return m_pSmo->IsOpen();
	}
	///@brief �̶��Զ�У׼      //append for qin      
	///��<9B> ���أ�<06>
	DWORD AutoAdj(BYTE *const pbResult=NULL);
	///@brief �̶��Զ�����      //append for qin      
	///��<9C> ���أ�<06>��ʧ��<15>,������<94>
	DWORD AutoReadData(BYTE *const pbResult);
	///@brief �̶ȼƸ�λ            
	///��<9A> ���أ�<06>��ʧ��<15>
	DWORD Reset(BYTE *const pbResult=NULL);
	////@brief�˳����
	///��<9D> ����:�ɹ�<06>��ʧ��<15>
	DWORD QuitAutoTest(BYTE *const pbResult);
	///@brief ��ʼ�Զ����          
	///��<92> ���أ���һ�� <93>���ڶ��� <95>�� ������ <95>�����Ĵ� <94>��ʧ��<15>
	DWORD StartAutoTest(BYTE *const pbResult=NULL);    
	///@brief ȡ��һ������          
	/// ��<96> ���أ��ɹ�<����>��ʧ��<15>
	DWORD GetData1(BYTE *const pbResult, float *const pfData1);
	///@brief ȡ�ڶ��������Ĵ�����
	///��<97> ���أ��ɹ�<����>��ʧ��<15>
	DWORD GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4);
	///@brief ȡ���״̬
	DWORD GetTestState(BYTE *const pbResult, BYTE *const pbState); 
	///@brief ����ʵʱ���ģʽ
    DWORD StartRealTest(BYTE *const pbResult=NULL);      
	////@brief ������Ӧ����ģʽ,������Ƚ�̤���ز���Ч(����ʵʱģʽ��)
    DWORD AllowTest(BYTE *const pbResult);  
	///@brief ����һ�μ���Ƿ����,������̤�����Ƿ��ɿ�
    DWORD GetSign(BYTE *const pbResult);  
	///@brief �̶ȼƸ�λ  ֻ����µ�оƬ(���Ҵ�������,��ʱ����)          
	///��<9D> ���أ�<98>��ʧ��<15>
	DWORD HardReset(BYTE *const pbResult=NULL);

	//�첽����

	//�ظ��Լ��
	 void OnRepeatCheck(WPARAM wParam, LPARAM lParam);
	 //ʾֵ�����
	 void OnRealCheck(WPARAM wParam, LPARAM lParam);

public:
	// �첽����
	 void Reset_Asyn(HWND hCallBackWnd=NULL);
	 void GetData1234_Asyn(HWND hCallBackWnd=NULL);
private:
	// ��Ӧ�첽��������
	afx_msg void OnReset(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetData1234(WPARAM wParam, LPARAM lParam);

private:
	float m_fData1,m_fData2,m_fData3,m_fData4;

public:
	// ��ȡʵʱ����Data1
	inline float GetData1Value(void) const
	{
		return m_fData1;
	}
	// ��ȡʵʱ����Data2
	inline float GetData2Value(void) const
	{
		return m_fData2;
	}
	// ��ȡʵʱ����Data3
	inline float GetData3Value(void) const
	{
		return m_fData3;
	}
	// ��ȡʵʱ����Data4
	inline float GetData4Value(void) const
	{
		return m_fData4;
	}


public:
	// �˳��̲߳���
	void QuitThread(void);

private:
	// ��ʼ���豸
	void InitializeInstrument(const enum InstrumentModel im);
	// ж���豸
	void UninitializeInstrument(void);

public:
	float m_fRepatData[6],m_fRealData[6][4];
	// �Ƿ���ֹ���
	BOOL m_bIsQuit;

private:
	CSmo* m_pSmo;
	// �߳��˳���־
	bool m_bQuitThread;
};


