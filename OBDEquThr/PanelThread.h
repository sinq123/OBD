#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NHSZKS.h"
#include "NH50R.h"

// CPanelThread

class AFX_EXT_CLASS CPanelThread : public CWinThread
{
	DECLARE_DYNCREATE(CPanelThread)

protected:
	CPanelThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPanelThread();

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
		// NH50R �����ɹ������
		IM_NANHUA_NH50R,
		// NH1 ���麣�¸ۣ�
		IM_NANHUA_NH1,
		// NH2 ��������վ��
		IM_NANHUA_NH2,
		// NHSZKS �����ڿ��ˣ�
		IM_NANHUA_NHSZKS,
		// NHSHTHD ���Ϻ�ͬ������
		IM_NANHUA_NHSHTHD,
	};

public:
	// ͨ���������ӵ�����
	// (in)pwchIP:Socket������IP��ַ
	// (in)usPort:Socket�������˿�
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort);

	// �򿪴������ӵ�����
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// in nBaudRate�������� (110��300��600��1200��2400��4800��9600��14400��19200��38400��56000��57600��115200��128000��256000)
	DWORD ConnectServer(const BYTE bPort, const int nBaudRate = CBR_19200, const enum InstrumentModel im = IM_NANHUA_NH50R);

	// �Ͽ�����������
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD DisconnectServer(void);

	// �жϵ������Ƿ�����
	inline bool IsConnect(void) const
	{
		return m_pLEDPanel->IsConnect();
	};

	// ���ò����б�
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD SetList(void);

	// ɾ�������б�
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD DelList(void);

	// ɾ�������ļ�
	// (return): 0x00:����
	//           ��0x00:�쳣
	//DWORD DelFile(void);

	// ��ʾ
	// (in)pwchDisplayTxt:��ʾ�ı�
	// (in)bTwinking:�Ƿ���˸
	// (in)chLineSpace:�м��
	// (in)chInDisplayMode:�뻨��
	// (in)chOutDisplayMode:������
	// (in)chFramePauseTime:֡ͣ��ʱ��
	// (in)chDisplaySpeed:��ʾ�ٶ�
	// (in)chFontAndSize:���弰��С
	// (in)chSetupType:����ģʽ
	// (in)chhAlign:ˮƽ����
	// (in)chvAlign:��ֱ����
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD Display(const wchar_t *const pwchDisplayTxt,
		const bool bTwinking=false,
		const char chLineSpace='0',
		const char chInDisplayMode=0x30,
		const char chOutDisplayMode=0x30,
		const char chFramePauseTime='3',
		const char chDisplaySpeed='0',
		const char chFontAndSize='5',
		const char chSetupType='b',
		const char chhAlign='0',
		const char chvAlign='1'
		);

	// ��ʾ����
	// (in)nPanel������
	// (in)nLine���к�
	// (in)pwchDisLine��������ʾ����
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt);

public:
	// ����
	// �������������õ�����
	// (in)pch�����ͻ���
	// (in)nLength�����ͻ��峤��
	// (return): 0x00������
	//           ��0x00���쳣
	DWORD SendToServer(const char *const pchBuf, const int nLen);

public:
	// �˳��̲߳���
	void QuitThread(void);

private:
	// ��ʼ���豸
	void InitializeInstrument(const enum InstrumentModel im);
	// ж���豸
	void UninitializeInstrument(void);

private:
	// ������������
	CPanel* m_pLEDPanel;
	// �߳��˳���־
	bool m_bQuitThread;
};


