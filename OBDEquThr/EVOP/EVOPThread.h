#pragma once

#include "EVOP.h"
#include "NHEVOP.h"


class AFX_EXT_CLASS CEVOPThread : public CWinThread
{
	DECLARE_DYNCREATE(CEVOPThread)
	
protected:
	CEVOPThread(void);
	virtual ~CEVOPThread(void);

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
		IM_NHEVOP,
	};

public:
	// �򿪴���
	DWORD Open(const BYTE bPort, const int nBaudRate, const wchar_t *szProtocol);

	// �رմ���
	DWORD Close(void);

	// �жϴ����ǻ��
	bool IsOpen(void) const;

	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emInstrumentModel;
	}
	

	// ��ʼ���
	DWORD StartTest(const int nItem, const int nType);
	// ȡʵʱ����
	DWORD GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T);
	// ȡ״̬
	DWORD GetState(int &nState);
	// ȡ������Ϣ
	DWORD GetTestInfo(int &nTestState, int &nTimeCount);
	// �������
	DWORD EndTest(int &nItem, int &nJudge);

private:
	// ��ʼ���豸
	// ������
	// szProtocol���豸Э��
	void InitializeInstrument(const wchar_t *szProtocol);
	// ж���豸
	void UninitializeInstrument(void);

public:
	// �˳��̲߳���
	void QuitThread(void);


private:
	// ����
	CEVOP* m_pAdapter;
	InstrumentModel m_emInstrumentModel;
	

private:
	// �߳��˳���־
	bool m_bQuitThread;

	// ���ں�
	BYTE m_bComPort;
	// ���ڲ�����
	int m_nComBaudRate;
};