#pragma once

#include "OBDAdapter.h"
#include "ZenYuanOBD.h"
#include "ZhengDeOBD.h"
#include "NanHuaWSOBD.h"


class AFX_EXT_CLASS COBDAdapterThread : public CWinThread
{
	DECLARE_DYNCREATE(COBDAdapterThread)
	
protected:
	COBDAdapterThread(void);
	virtual ~COBDAdapterThread(void);

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
		IM_ZENYUAN,
		IM_ZHENGDE,
		IM_NHWS,
	};

public:
	// �򿪴���
	DWORD Open(const BYTE bPort, const int nBaudRate, const wchar_t *szProtocol);
	DWORD Open(std::wstring strIP, const int nPort, const wchar_t *szProtocol);
	DWORD ReOpen(void);

	// �رմ���
	DWORD Close(void);

	// �жϴ����ǻ��
	bool IsOpen(void) const;

	InstrumentModel GetInstrumentModel(void) const;

	
	// �������Ƿ�����
	bool IsAdapterOnline(void);
	// ��ʼ��������
	bool Init(DWORD &dwProtocol, const int nFuelType);
	void Init_Asyn(DWORD dwProtocol, const int nFuelType, HWND hCallBackWnd=NULL);
	// ȡ������
	bool GetDTC(std::vector<SDTCData> &vtDTC);
	// ȡ����ָʾ��״̬
	bool GetLampState(bool &bIsOn);
	// ȡϵͳ�����
	bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem);
	void GetSystemCheck_Asyn(HWND hCallBackWnd=NULL);
	// ���������
	bool ClearDTC(void);
	// ȡʵʱ����
	bool GetRTData(void);
	bool GetRTData(std::map<std::wstring, std::wstring> &mapData);
	void GetRTData_Asyn(HWND hCallBackWnd=NULL);
	// ȡ������ת��
	int GetEngineRevValue(void) { return m_nRev;}
	// ȡ����
	float GetVelocityValue(void) { return m_fV;}
	// ȡ��ȴҺ�¶�
	float GetCoolantTempValue(void) { return m_fTemp;}
	// �����ſ���[%]
	float GetSolarTermDoorPositionValue(void) { return m_fSolarTermDoorPosition;}
	// ���㸺��ֵ[%]
	float GetCalculationLoadValue(void) { return m_fCalculationLoad;}
	// OBDǰ��������ѹ[mV]
	float GetForwardSensorVValue(void) { return m_fForwardSensorV;}
	// OBDǰ����������[mA]
	float GetForwardSensorIValue(void) { return m_fForwardSensorI;}
	// OBD��������ϵ��[��]
	float GetAirCoefficientValue(void) { return m_fAirCoefficient;}
	// ������(g/s)
	float GetMAFValue(void) { return m_fMAF;}
	// ����ѹ��(kpa)
	float GetMAPValue(void) { return m_fMAP;}
	// �������������(kw)
	float GetEngineOutputPowerValue(void) { return m_fEngineOutputPower;}
	// ��ѹѹ��[kpa]
	float GetChargeAirPressureValue(void) { return m_fChargeAirPressure;}
	// ������[L/100km]
	float GetFuelConsumptionValue(void) { return m_fFuelConsumption;}
	// ����������Ũ��[ppm]
	int GetNOXConcentrationValue(void) { return m_nNOXConcentration;}
	// ����������[mL/s]
	float GetUreaInjectionVolumeValue(void) { return m_fUreaInjectionVolume;}
	// �����¶�[��]
	float GetEGTValue(void) { return m_fEGT;}
	// ����������ѹ��[Kpa]
	float GetDPFDifferentialPressureValue(void) { return m_fDPFDifferentialPressure;}
	// EGR����[%]
	float GetEGRPositionValue(void) { return m_fEGRPosition;}
	// ȼ������ѹ��[bar]	
	int GetFuelDeliveryPressureValue(void) { return m_nFuelDeliveryPressure;}

	// ��ȡ������̣���λkm
	bool GetDTCMileage(DWORD &dwMileage);
	// ��ȡ��������
	bool GetFreezeData(std::wstring &strData);
	// ȡVIN
	bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL);
	// ȡOBD��ʽ����Ҫ��
	bool GetOBDType(std::wstring &strType);
	// ȡ��ʻ��̣���λkm
	bool GetMileage(DWORD &dwMileage);
	// ȡECU��Ϣ
	bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// ȡ����汾
	bool GetSoftwareVer(std::wstring &strVer);
	// ȡIUPR����
	bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// ����ʼ��
	bool UnInit(void);
	// ȡOBD����Э��
	std::wstring GetOBDProtocolName(DWORD dwProtocol);

private:
	afx_msg void OnInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetSystemCheck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetRTData(WPARAM wParam, LPARAM lParam);

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

public:
	std::vector<DWORD> m_vtDTC;
	SOBDSystemCheckItem m_sSystemCheckItem;

private:
	// ����
	COBDAdapter* m_pAdapter;
	InstrumentModel m_emInstrumentModel;
	
	int m_nRev;		// ������ת��
	float m_fV;		// ����
	float m_fTemp;	// ��ȴҺ�¶�
	float m_fSolarTermDoorPosition;	// �����ſ���[%]
	float m_fCalculationLoad;		// ���㸺��ֵ[%]
	// OBDǰ��������ѹ[mV]
	float m_fForwardSensorV;
	// OBDǰ����������[mA]
	float m_fForwardSensorI;
	// OBD��������ϵ��[��]
	float m_fAirCoefficient;
	float m_fMAF;					// ������(g/s)
	float m_fMAP;                   // ����ѹ��(kpa)
	float m_fEngineOutputPower;		// �������������(kw)
	float m_fChargeAirPressure;		// ��ѹѹ��[kpa]
	float m_fFuelConsumption;		// ������[L/100km]
	int m_nNOXConcentration;		// ����������Ũ��[ppm]
	float m_fUreaInjectionVolume;	// ����������[mL/s]
	float m_fEGT;					// �����¶�[��]
	float m_fDPFDifferentialPressure;	// ����������ѹ��[Kpa]
	float m_fEGRPosition;				// EGR����[%]
	int m_nFuelDeliveryPressure;		// ȼ������ѹ��[bar]	
	// ȼ�����ͣ�1���ͣ�2����
	int m_nFuelType;

private:
	// �߳��˳���־
	bool m_bQuitThread;

	// ���ں�
	BYTE m_bComPort;
	// ���ڲ�����
	int m_nComBaudRate;

	// ������IP��ַ
	std::wstring m_strServerIP;
	// �������˿ں�
	unsigned short m_usServerPort;
	
	// �ϴβ���ʱ��
	clock_t m_LastOPTime;
	// ȡʵʱ����
	int m_nGetRTDataTimeInterval;
};