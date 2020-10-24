/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�ParaThread.h
* �ļ���ʶ��
* ժҪ�������������߳�
*
* ��ǰ�汾��1.0.1
* ���ߣ�Hyh
* ������ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-06-08
* ���ߣ�Hyh
* ������1)��ӵ��ʽ�ͳ��������
*
* ��ǰ�汾��1.0.0
* ������ڣ�2010-05-12
* ���ߣ�Hyh
* ��������ʽ����ʹ��
*/
#pragma once
#include "..\NHDetCommModu\NHDetCommModu.h"
#include "..\NHWin32Lib\NHWin32Lib.h"

#include "NanhuaPara.h"
#include "NanhuaEPCS.h"
#include "NanhuaEPCS4G.h"
#include "NanhuaSocketPara.h"
#include "DigitalThermometer.h"
#include "DigitalThermometer_NJRJ.h"
#include "SG506.h"

// CParaThread
class AFX_EXT_CLASS CParaThread : public CWinThread
{
	DECLARE_DYNCREATE(CParaThread)

protected:
	CParaThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CParaThread();

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
		// PARA
		IM_NANHUA_PARA = 0,
		IM_PARA_DIM,
		IM_PARA_DIM_NJRJ,
		IM_PARA_SG506,
		IM_NANHUA_EPCS,
		//֧��д��汾
		IM_NANHUA_EPCS_4G,
		// �ϻ������ǣ�socket��ȡ
		IM_NANHUA_PARA_Socket,
	};

public:
	// �򿪴���
	DWORD Open(const BYTE bPort, const enum InstrumentModel im);
	DWORD Open(std::wstring strIP, const int nPort, const enum InstrumentModel im);
	DWORD ReOpen(void);

	// �رմ���
	DWORD Close(void);

	// �жϴ����ǻ��
	inline bool IsOpen(void) const
	{
		return m_pPara->IsOpen();
	}
	
	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emIM;
	}

	// ��ȡ���ڲ���״̬
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
public:
	// �˳��̲߳���
	void QuitThread(void);

public:
	// ��ȡʵʱ����-�¶�
	inline float GetETValue(void) const
	{
		return m_fET;
	}
	// ��ȡʵʱ����-ʪ��
	inline float GetRHValue(void) const
	{
		return m_fRH;
	}
	// ��ȡʵʱ����-����ѹ
	inline float GetAPValue(void) const
	{
		return m_fAP;
	}
	// ��ȡʵʱ����-����
	inline float GetOilTempValue(void) const
	{
		return m_fOilTemp;
	}
	// ��ȡʵʱ����-ת��
	inline USHORT GetRpmValue(void) const
	{
		return m_usRpm;
	}

public:
	// ͬ������
	// �¶�б��У׼
	DWORD EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// ʪ��б��У׼
	DWORD RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// ����ѹб��У׼
	DWORD AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// �������б��У׼
	DWORD OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// �����ȶ�б��У׼
	DWORD OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope);

	// �¶���λУ׼
	DWORD EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// ʪ����λУ׼
	DWORD RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// ����ѹ��λУ׼
	DWORD AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// ���������λУ׼
	DWORD OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// �����ȶ���λУ׼
	DWORD OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero);

	// ����У׼����
	DWORD BackupCalibrationParameters(BYTE *const pbResult);

	// �ָ�У׼����
	DWORD RecoverCalibrationParameters(BYTE *const pbResult, const CPara::ParaType emParaType);

	// ���ó��Ϊ4
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL);

	// ���ó��Ϊ2
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL);

	// ���õ��ʽΪ����
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL);

	// ���õ��ʽΪ����
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL);

	// ��ȫ������
	DWORD GetEntireData(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL);

	// ����������
	DWORD GetEnvironmentParameters(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL);

	// ��ת������
	DWORD GetOilTempRpm(float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL);

	// DT 
	bool DT_ASM(const TESTLOG &sTestLog, const SResultOfASM &sResultOfASM);
	bool DT_VMAS(const TESTLOG &sTestLog, const SResultOfVMAS &sResultOfVMAS);
	bool DT_LUGDOWN(const TESTLOG &sTestLog, const SResultOfLUGDOWN &sResultOfLUGDOWN);
	bool DT_DIS(const TESTLOG &sTestLog, const SResultOfDIS &sResultOfDIS);
	bool DT_FSUNHT(const TESTLOG &sTestLog, const SResultOfFSUNHT &sResultOfFSUNHT);
	bool DT_FSUYD(const TESTLOG &sTestLog, const RESULTOFFSUYD &sResultOfFSUYD);

public:
	// �첽����
	// ��ȫ������
	void GetEntireData_Asyn(HWND hCallBackWnd=NULL);

	// ����������
	void GetEnvironmentParameters_Asyn(HWND hCallBackWnd=NULL);

	// ��ת������
	void GetOilTempRpm_Asyn(HWND hCallBackWnd=NULL);

	// DT����
	void DTCommand_Asyn(const BYTE * pbDataBuf, const UINT nDatalen);

private:
	// ��ʼ���豸
	void InitializeInstrument(const enum InstrumentModel im);
	// ж���豸
	void UninitializeInstrument(void);

private:
	// ��Ӧ�첽��������
	afx_msg void OnGetEntireData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetEnvironmentParameters(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetOilTempRpm(WPARAM wParam, LPARAM lParam);

private:
	// �첽SQLite�߳�
	static DWORD __stdcall WorkerThread(LPVOID pParam);

private:
	// ��SQLite3���ݿ�
	DWORD OpenDB(void);
	// �ر�SQLite3���ݿ�
	DWORD CloseDB(void);
	// ���SQLite3���ݿ�
	void CheckDB(void);
	// �жϴ����ǻ��
	inline bool IsDBOpen(void) const
	{
		return m_bOpenDB;
	}

	// ��������
	void  DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

private:
	// ����
	CPara* m_pPara;
	// �߳��˳���־
	bool m_bQuitThread;

private:
	// �¶�,ʪ��,����ѹ,����
	float m_fET,m_fRH,m_fAP,m_fOilTemp;
	// ת��
	USHORT m_usRpm;
	// ���ڲ���״̬
	DWORD m_dwStatus;

private:
	enum InstrumentModel m_emIM;
	// SQLite3���ݿ�
	CppSQLite3DB m_db;
	// SQLite3���ݿ��Ƿ��
	bool m_bOpenDB;
	// ICCID
	char m_chICCID[21];

	// �ٽ籣����
	CRITICAL_SECTION m_sCriticalSection;
	// �Ƿ��˳��������߳�
	bool m_bWorkerRunning;
	HANDLE m_hCloseWorker;
	// ������
	int m_nWorker;
		
	// ���ں�
	BYTE m_bComPort;
	// ���ڲ�����
	int m_nComBaudRate;

	// ������IP��ַ
	std::wstring m_strServerIP;
	// �������˿ں�
	unsigned short m_usServerPort;
};


