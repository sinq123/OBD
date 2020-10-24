/*
* Copyright (c) ��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�OBDAdapter.h
* �ļ���ʶ��
* ժҪ��OBD�����������࣬�Դ����⺯����ʽ����
*
* �汾��1.0
* ���ڣ�2017-11-18
* ���ߣ�Raylee
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2017-11-18
* ���ߣ�Raylee
* ���������ڿ���
*/

#pragma once

#ifndef OBDAdapter_H
#define OBDAdapter_H

#include <vector>
#include <string>
#include <map>

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

// OBD�첽
#define WM_OBD_INIT													 WM_USER + 0x1201  // ��ʼ��
#define WM_OBD_GET_DTC										 		 WM_USER + 0x1202  // ȡ������
#define WM_OBD_GET_ENGINE_REV										 WM_USER + 0x1203  // ȡת��
#define WM_OBD_GET_VELOCITY											 WM_USER + 0x1204  // ȡ����
#define WM_OBD_GET_COOLANT_TEMP										 WM_USER + 0x1205  // ȡ��ȴҺ�¶�
#define WM_OBD_GET_SYSTEMCHECK										 WM_USER + 0x1206  // ȡϵͳ�����
#define WM_OBD_GET_RT_DATA											 WM_USER + 0x1207  // ȡ������

//BCDתʮ������
#define BCD2DEC(b)	(((b)>>4)*10+((b)&0x0F))

// ��������
enum OBDInitState
{
	OBD_InitSucceed,
	OBD_InitFailed,
	OBD_Initing,
	OBD_Offline,
	OBD_Online,
};

struct SOBDSystemCheckItem
{
	// 0����֧��/�����ã�1����ɣ�2��δ��ɣ�
	int nMFT;
	int nFST;
	int nCC;	// ����
	int nCCH;
	int nOS;	// ����
	int nOSH;	// ����
	int nEECS;
	int nEGR;	// ���ͣ�����
	int nSAIS;
	int nICM;
	int nVVT;

	//int nFST;
	//int nEGR;
	int nDOC;	// ����
	int nSCR;	// ����
	int nDPF;	// ����
	int nASC;
	int nPOC;	// ����

	SOBDSystemCheckItem(){ZeroMemory(this, sizeof(SOBDSystemCheckItem));}
};

struct SDTCData
{
	std::wstring strCode;
	std::wstring strComment;
};

class COBDAdapter
{
public:
	COBDAdapter(void);
	virtual ~COBDAdapter(void);

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
	virtual DWORD Open(std::wstring strIP, const int nPort) = 0;

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

	// ����У���
	// ������
	// pbData��Ҫ����У�����������
	// uiStart��Ҫ����У������ݿ�ʼλ��
	// uiEnd��Ҫ����У������ݽ���λ��
	// pbCRC_HIGH������У��͸�λ
	// pbCRC_LOW������У��͵�λ
	BYTE CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW);
	///////////////////////////////////////////////////////////////////////////////////���ڶ�дend

public:
	//////////////////////////////////////////////////////////////////////////////////////// �������� start
	// �������Ƿ�����
	virtual bool IsAdapterOnline(void) = 0;
	// ��ʼ��������
	virtual bool Init(DWORD &dwProtocol, const int nFuelType) = 0;
	// ��ʼ����״̬
	virtual bool GetInitState(OBDInitState &enState, DWORD &dwProtocol) = 0;
	// ȡ������
	virtual bool GetDTC(std::vector<SDTCData> &vtDTC) = 0;	
	// ȡ����ָʾ��״̬
	virtual bool GetLampState(bool &bIsOn) = 0;
	// ȡϵͳ�����
	virtual bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem) = 0;
	// ���������
	virtual bool ClearDTC(void) = 0;
	// ȡʵʱ����
	virtual bool GetRTData(int &nRev, float &fV, float &fTemp) = 0;
	virtual bool GetRTData(std::map<std::wstring, std::wstring> &mapData) = 0;
	// ��ȡ������̣���λkm
	virtual bool GetDTCMileage(DWORD &dwMileage) = 0;
	// ��ȡ��������
	virtual bool GetFreezeData(std::wstring &strData) = 0;
	// ȡVIN
	virtual bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL) = 0;
	// ȡOBD��ʽ����Ҫ��
	virtual bool GetOBDType(std::wstring &strType) = 0;
	// ȡ��ʻ��̣���λkm
	virtual bool GetMileage(DWORD &dwMileage) = 0;
	// ȡECU��Ϣ
	virtual bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL) = 0;
	// ȡ����汾
	virtual bool GetSoftwareVer(std::wstring &strVer) = 0;
	// ȡIUPR����
	virtual bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL) = 0;
	// ����ʼ��
	virtual bool UnInit(void) = 0;
	// ȡOBD����Э��
	virtual std::wstring GetOBDProtocolName(DWORD dwProtocol) = 0;
	//////////////////////////////////////////////////////////////////////////////////////// �������� end

protected:
	// ���ھ��
	HANDLE m_hComm;
	//// �ٽ籣����
	//CRITICAL_SECTION m_sCriticalSection;

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