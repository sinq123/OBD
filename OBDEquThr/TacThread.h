/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�TacThread.h
* �ļ���ʶ��
* ժҪ��ת�ټ��߳�
*
* ��ǰ�汾��1.0
* ���ߣ�Cui,hyh
* ������ڣ�2009-12-08
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/


#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaRpm5300.h"
#include "FuLiVMT2000.h"
#include "HuaGongVTET1000.h"
#include "MingQuanMQZ3.h"

// CTacThread

class AFX_EXT_CLASS CTacThread : public CWinThread
{
	DECLARE_DYNCREATE(CTacThread)

protected:
	CTacThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTacThread();

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
		// RPM5300
		IM_NANHUA_RPM5300,
		// RPM5500(��δ����)
		//IM_NANHUA_RPM5500,
		// ����
		IM_FULI_VMT2000,
		// ����
		IM_HUAGONG_VTET1000,
		// ��Ȫ
		IM_MINGQUAN_MQZ3,
	};

public:
	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emInstrumentModel;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���begin
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// (return)��
	// TAC_OPEN_SUCCEED
	// TAC_OPEN_FAIL
	DWORD Open(const BYTE bPort, const wchar_t *szProtocol);
		
	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// (return)��
	// TAC_CLOSE_SUCCEED
	// TAC_CLOSE_FAIL_WIN32API
	// TAC_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����ǻ��
	// (return)��
	// true:�����Ѵ�
	// false:����δ��
	inline bool IsOpen(void) const
	{
		return m_pTac->IsOpen();
	}
	// ��ȡ���ڲ���״̬
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���end

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����ͬ������begin
	// ��ȡת��
	// (out)pnRev��������ת��ָ�룬��λ��r/min
	// (return)��
	// TAC_WR_FUNC_SUCCEED
	// TAC_WR_FUNC_PORT_NOT_OPEN
	// TAC_WR_FUNC_WRITE_SPECIFIED_DATA_FAIL
	// TAC_WR_READ_SPECIFIED_DATA_FAIL
	// TAC_WR_EQUIP_RETURE_0X15
	// TAC_WR_CHECK_SUM_FAIL
	// TAC_WR_UNKNOWN_ERROR
	DWORD GetRev(int *pnRev);
	inline DWORD GetRev(void)
	{
		return GetRev(&m_nRev);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����ͬ������end

public:
	// �첽��ȡ
	void GetRev_Asyn(HWND hCallBackWnd=NULL);

public:
	// �˳��̲߳���
	void QuitThread(void);

public:
	// ��ȡת��ֵ
	inline int GetRevValue(void) const
	{
		return m_nRev;
	}

private:
	// ��Ӧ�첽��������
	afx_msg void OnGetRev(WPARAM wParam, LPARAM lParam);

private:
	// ��ʼ���豸
	void InitializeInstrument(const wchar_t *szProtocol);
	// ж���豸
	void UninitializeInstrument(void);

private:
	// RPM5300������
	CTac* m_pTac;
	// ת��,��λ:r/min
	int m_nRev;
	// �߳��˳���־
	bool m_bQuitThread;
	// �豸�ͺ�
	InstrumentModel m_emInstrumentModel;
	// ���ڲ���״̬
	DWORD m_dwStatus;
};


