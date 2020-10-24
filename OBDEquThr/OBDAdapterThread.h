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
	// 仪器型号
	enum InstrumentModel
	{
		IM_ZENYUAN,
		IM_ZHENGDE,
		IM_NHWS,
	};

public:
	// 打开串口
	DWORD Open(const BYTE bPort, const int nBaudRate, const wchar_t *szProtocol);
	DWORD Open(std::wstring strIP, const int nPort, const wchar_t *szProtocol);
	DWORD ReOpen(void);

	// 关闭串口
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;

	InstrumentModel GetInstrumentModel(void) const;

	
	// 适配器是否在线
	bool IsAdapterOnline(void);
	// 初始化适配器
	bool Init(DWORD &dwProtocol, const int nFuelType);
	void Init_Asyn(DWORD dwProtocol, const int nFuelType, HWND hCallBackWnd=NULL);
	// 取故障码
	bool GetDTC(std::vector<SDTCData> &vtDTC);
	// 取故障指示灯状态
	bool GetLampState(bool &bIsOn);
	// 取系统检测结果
	bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem);
	void GetSystemCheck_Asyn(HWND hCallBackWnd=NULL);
	// 清除故障码
	bool ClearDTC(void);
	// 取实时数据
	bool GetRTData(void);
	bool GetRTData(std::map<std::wstring, std::wstring> &mapData);
	void GetRTData_Asyn(HWND hCallBackWnd=NULL);
	// 取发动机转速
	int GetEngineRevValue(void) { return m_nRev;}
	// 取车速
	float GetVelocityValue(void) { return m_fV;}
	// 取冷却液温度
	float GetCoolantTempValue(void) { return m_fTemp;}
	// 节气门开度[%]
	float GetSolarTermDoorPositionValue(void) { return m_fSolarTermDoorPosition;}
	// 计算负荷值[%]
	float GetCalculationLoadValue(void) { return m_fCalculationLoad;}
	// OBD前传感器电压[mV]
	float GetForwardSensorVValue(void) { return m_fForwardSensorV;}
	// OBD前传感器电流[mA]
	float GetForwardSensorIValue(void) { return m_fForwardSensorI;}
	// OBD过量空气系数[λ]
	float GetAirCoefficientValue(void) { return m_fAirCoefficient;}
	// 进气量(g/s)
	float GetMAFValue(void) { return m_fMAF;}
	// 进气压力(kpa)
	float GetMAPValue(void) { return m_fMAP;}
	// 发动机输出功率(kw)
	float GetEngineOutputPowerValue(void) { return m_fEngineOutputPower;}
	// 增压压力[kpa]
	float GetChargeAirPressureValue(void) { return m_fChargeAirPressure;}
	// 耗油量[L/100km]
	float GetFuelConsumptionValue(void) { return m_fFuelConsumption;}
	// 氮氧传感器浓度[ppm]
	int GetNOXConcentrationValue(void) { return m_nNOXConcentration;}
	// 尿素喷射量[mL/s]
	float GetUreaInjectionVolumeValue(void) { return m_fUreaInjectionVolume;}
	// 排气温度[℃]
	float GetEGTValue(void) { return m_fEGT;}
	// 颗粒捕集器压差[Kpa]
	float GetDPFDifferentialPressureValue(void) { return m_fDPFDifferentialPressure;}
	// EGR开度[%]
	float GetEGRPositionValue(void) { return m_fEGRPosition;}
	// 燃油喷射压力[bar]	
	int GetFuelDeliveryPressureValue(void) { return m_nFuelDeliveryPressure;}

	// 获取故障里程，单位km
	bool GetDTCMileage(DWORD &dwMileage);
	// 获取冻结数据
	bool GetFreezeData(std::wstring &strData);
	// 取VIN
	bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL);
	// 取OBD型式检验要求
	bool GetOBDType(std::wstring &strType);
	// 取行驶里程，单位km
	bool GetMileage(DWORD &dwMileage);
	// 取ECU信息
	bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// 取软件版本
	bool GetSoftwareVer(std::wstring &strVer);
	// 取IUPR数据
	bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// 反初始化
	bool UnInit(void);
	// 取OBD总线协议
	std::wstring GetOBDProtocolName(DWORD dwProtocol);

private:
	afx_msg void OnInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetSystemCheck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetRTData(WPARAM wParam, LPARAM lParam);

private:
	// 初始化设备
	// 参数：
	// szProtocol：设备协议
	void InitializeInstrument(const wchar_t *szProtocol);
	// 卸载设备
	void UninitializeInstrument(void);

public:
	// 退出线程操作
	void QuitThread(void);

public:
	std::vector<DWORD> m_vtDTC;
	SOBDSystemCheckItem m_sSystemCheckItem;

private:
	// 对象
	COBDAdapter* m_pAdapter;
	InstrumentModel m_emInstrumentModel;
	
	int m_nRev;		// 发动机转速
	float m_fV;		// 车速
	float m_fTemp;	// 冷却液温度
	float m_fSolarTermDoorPosition;	// 节气门开度[%]
	float m_fCalculationLoad;		// 计算负荷值[%]
	// OBD前传感器电压[mV]
	float m_fForwardSensorV;
	// OBD前传感器电流[mA]
	float m_fForwardSensorI;
	// OBD过量空气系数[λ]
	float m_fAirCoefficient;
	float m_fMAF;					// 进气量(g/s)
	float m_fMAP;                   // 进气压力(kpa)
	float m_fEngineOutputPower;		// 发动机输出功率(kw)
	float m_fChargeAirPressure;		// 增压压力[kpa]
	float m_fFuelConsumption;		// 耗油量[L/100km]
	int m_nNOXConcentration;		// 氮氧传感器浓度[ppm]
	float m_fUreaInjectionVolume;	// 尿素喷射量[mL/s]
	float m_fEGT;					// 排气温度[℃]
	float m_fDPFDifferentialPressure;	// 颗粒捕集器压差[Kpa]
	float m_fEGRPosition;				// EGR开度[%]
	int m_nFuelDeliveryPressure;		// 燃油喷射压力[bar]	
	// 燃油类型，1汽油；2柴油
	int m_nFuelType;

private:
	// 线程退出标志
	bool m_bQuitThread;

	// 串口号
	BYTE m_bComPort;
	// 串口波特率
	int m_nComBaudRate;

	// 服务器IP地址
	std::wstring m_strServerIP;
	// 服务器端口号
	unsigned short m_usServerPort;
	
	// 上次操作时刻
	clock_t m_LastOPTime;
	// 取实时数据
	int m_nGetRTDataTimeInterval;
};