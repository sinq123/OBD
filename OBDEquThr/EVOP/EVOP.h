/*
* Copyright (c) ��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�EVOP.h
* �ļ���ʶ��
* ժҪ������ȼ������ϵͳ�ۺϼ���ǳ����࣬�Դ����⺯����ʽ����
*
* �汾��1.0
* ���ڣ�2018-08-03
* ���ߣ�Raylee
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2018-08-03
* ���ߣ�Raylee
* ���������ڿ���
*/

#pragma once

#ifndef EVOP_H
#define EVOP_H

#include <vector>

// ���ڲ������
#define COM_OPEN_SUCCEED							0x00  // �򿪴��ڳɹ�
#define COM_OPEN_FAIL								0x01  // �򿪴���ʧ��

#define COM_CLOSE_SUCCEED							0x00  // �رմ��ڳɹ�
#define COM_CLOSE_FAIL_WIN32_API					0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define COM_CLOSE_FAIL_NO_OPEN_PORT					0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���

#define COM_WR_SUCCEED								0x00  // д���ɹ�
#define COM_WR_PORT_NOT_OPEN						0x01  // д��ʧ�ܣ�����δ��
#define COM_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // д��ʧ�ܣ�дָ������ʧ��
#define COM_WR_READ_SPECIFIED_DATA_FAIL				0x03  // д��ʧ�ܣ���ָ������ʧ��
#define COM_WR_EQUIP_RETURE_0X15					0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define COM_WR_CHECK_SUM_FAIL						0x05  // д��ʧ�ܣ���������У���ʧ��
#define COM_WR_UNKNOWN_ERROR						0xFF  // д��ʧ�ܣ�δ֪����

class CEVOP
{
public:
	CEVOP(void);
	virtual ~CEVOP(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// ������
	// in bPort�����ں�(1~255)
	// in nBaudRate�������� (110��300��600��1200��2400��4800��9600��14400��19200��38400��56000��57600��115200��128000��256000)
	// ���ش򿪽����
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort, const int nBaudRate = CBR_2400) = 0;

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���أ�
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32_API
	// COM_CLOSE_FAIL_NO_OPEN_PORT
	DWORD Close(void);

	// �жϴ����ǻ��
	virtual bool IsOpen(void) const;
	///////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���end

protected:
	///////////////////////////////////////////////////////////////////////////////////���ڶ�дstart
	// д��������
	// pbWriteBuffer��Ҫд����������ݣ�����Ϊ��ָ��
	// dwNumberOfBytesToWrite��Ҫд������ݳ���
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);

	// ����������
	// pbReadBuffer��Ҫ��ȡ���������ݣ�����Ϊ��ָ��
	// dwNumberOfBytesToRead��Ҫ��ȡ�����ݳ���
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);
	///////////////////////////////////////////////////////////////////////////////////���ڶ�дend

public:
	//////////////////////////////////////////////////////////////////////////////////////// �������� start
	// ��ʼ��������
	virtual DWORD StartTest(const int nItem, const int nType) = 0;
	// ȡʵʱ����
	virtual DWORD GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T) = 0;
	// ȡ״̬
	virtual DWORD GetState(int &nState) = 0;
	// ȡ������Ϣ
	virtual DWORD GetTestInfo(int &nTestState, int &nTimeCount) = 0;
	// �������
	virtual DWORD EndTest(int &nItem, int &nJudge) = 0;
	//////////////////////////////////////////////////////////////////////////////////////// �������� end

protected:
	// ���ھ��
	HANDLE m_hComm;

	// �ٽ�����װ��
	class CSLock:public CRITICAL_SECTION
	{
	public:
		CSLock()
		{
			InitializeCriticalSection(this);
		}

		~CSLock()
		{
			DeleteCriticalSection(this);
		}

		void Enter()
		{
			EnterCriticalSection(this);
		}

		void Leave()
		{
			LeaveCriticalSection(this);
		}
	};

	// �Զ������Զ���������
	class CAutoLock
	{
	public:
		CAutoLock(CSLock& lock)
			:m_csLock(lock)
		{
			Lock();
		}
		~CAutoLock()
		{
			UnLock();
		}

	public:
		// һ�㲻�ֶ�����
		void Lock()
		{
			m_csLock.Enter();
		}

		// һ�㲻�ֶ�����
		void UnLock()
		{
			m_csLock.Leave();
		}

	private:
		CSLock& m_csLock;
	};
	
	static CSLock & GetLogLocker();	
};
#endif