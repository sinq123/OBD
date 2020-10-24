/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�OpaThread.h
* �ļ���ʶ��
* ժҪ����͸��ȼ��߳�
*
* ��ǰ�汾��1.0
* ���ߣ�Cui,hyh
* ������ڣ�2009-12-08
*
* ��ʷ��Ϣ��
*
* �汾��1.0.3
* ���ڣ�2012-10-11
* ���ߣ�Raylee
* ��������Ӹ���FLB-100����ȪMQY-200�豸֧��
*
* �汾��1.0.2
* ���ڣ�2012-04-26
* ���ߣ�Raylee
* ���������FoFenFTY100֧��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-08-26
* ���ߣ�Raylee
* ���������GetTestResultData()���������� 0xB1 ָ���ȡ���������
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaNht6.h"
#include "NanhuaNht1l.h"
#include "NanhuaNHT610.h"
#include "FoFenFTY100.h"
#include "MingQuanMQY200.h"
#include "FuLiFLB100.h"


// COpaThread

class AFX_EXT_CLASS COpaThread : public CWinThread
{
	DECLARE_DYNCREATE(COpaThread)

protected:
	COpaThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~COpaThread();

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
		// NHT1L
		IM_NANHUA_NHT1L,
		// NHT6
		IM_NANHUA_NHT6,
		// NHT610
		IM_NANHUA_NHT610,
		// ���FTY-100
		IM_FOFEN_FTY100,
		// ����
		IM_FULI_FLB100,
		// ��Ȫ
		IM_MINGQUAN_MQY200,
	};
public:
	// �����ھ��
	HANDLE GetComHandle(void);
	void AttachComHandle(HANDLE hCom);
	void DetachComHandle(void);
	// �����ٽ���
	CRITICAL_SECTION GetCriticalSection(void);
	void AttachCriticalSection(CRITICAL_SECTION cs);
	void DetachCriticalSection(void);
	
	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emInstrumentModel;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���begin
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// NHT1L_OPEN_SUCCEED
	// NHT1L_OPEN_FAIL
	DWORD Open(const BYTE bPort, const enum InstrumentModel im);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// NHT1L_CLOSE_SUCCEED
	// NHT1L_CLOSE_FAIL_WIN32API
	// NHT1L_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����ǻ��
	inline bool IsOpen(void) const
	{
		return m_pOpa->IsOpen();
	}

	// ��ȡ���ڲ���״̬
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���end

	// ȡ�������صĴ�����
	inline DWORD GetErrorCodeFromEquipment(void) const
	{
		return m_pOpa->GetErrorCodeFromEquipment();
	}

	// ��ֹԤ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD StopWarmUp(void);

	// У׼
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD Calibrate(void);

	// ������ֵ
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD ClearMaxValue(void);

	// ���ÿ��Ƶ�Ԫ����
	// (in)bParamType����������
	// (out)bParamValue������ֵ
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD SetControlUnitParams(const COpa::ControlUnitParams *const psControlUnitParams);
	//DWORD SetControlUnitParams(const BYTE bParamType, const BYTE bParamValue);

	// �������ʪ��
	// (in)fRHValue����׼ֵ����λ��%
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD SetRH(const float fStdValue);

	// ��λEEPROM����
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD ResetEEPROM(void);

	// ȡ����������Ϣ
	// (out)pbs16AlarmInfo��������Ϣָ�룬��16λ������λ��Ӧ�ĺ�����ο�ͨѶ˵���飬����Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo);
	inline DWORD GetAlarmInfo(void)
	{
		return GetAlarmInfo(&m_bs16AlarmInfo);
	}

	// ȡʵʱ����
	// (out)pfN����͸���ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfK��������ϵ��ֵָ�룬��λ��m-1������Ϊ��ָ��
	// (out)pusRpm��ת��ֵָ�룬��λ��r/min������Ϊ��ָ��
	// (out)pusOilTemp������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp);
	inline DWORD GetRealTimeData(void)
	{
		return GetRealTimeData(&m_fN, &m_fK, &m_usRpm, &m_usOilTemp);
	}

	// ȡ������Ԫ�ڲ�����
	// (out)pfOriginalN��ԭʼ��͸���ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfOriginalK��ԭʼ������ϵ��ֵָ�룬��λ��m-1������Ϊ��ָ��
	// (out)pusSmokeTemp�������¶�ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pusTubeTemp�������ܱ�����ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pusDetectorTemp�������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pusLEDTemp������Դֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pusBoardTemp���ڲ��¶�ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pusLeftFanCurrent������ȵ���ֵָ�룬��λ��δ֪������Ϊ��ָ��
	// (out)pusRightFanCurrent���ҷ��ȵ���ֵָ�룬��λ��δ֪������Ϊ��ָ��
	// (out)pfPowerVoltage����Դ��ѹֵָ�룬��λ��δ֪������Ϊ��ָ��
	// (out)pusRealTimeIntensity��ʵʱ��ǿֵָ�룬��λ��δ֪������Ϊ��ָ��
	// (out)pusFullShadingIntensity��ȫ����ǿֵָ�룬��λ��δ֪������Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetMeasureUnitData(float *const pfOriginalN=NULL, 
		float *const pfOriginalK=NULL, 
		USHORT *const pusSmokeTemp=NULL, 
		USHORT *const pusTubeTemp=NULL, 
		USHORT *const pusDetectorTemp=NULL, 
		USHORT *const pusLEDTemp=NULL, 
		USHORT *const pusBoardTemp=NULL, 
		USHORT *const pusLeftFanCurrent=NULL, 
		USHORT *const pusRightFanCurrent=NULL, 
		float *const pfPowerVoltage=NULL, 
		USHORT *const pusRealTimeIntensity=NULL, 
		USHORT *const pusFullShadingIntensity=NULL);
	inline DWORD GetMeasureUnitData(void)
	{
		return GetMeasureUnitData(&m_fOriginalN,
			&m_fOriginalK,
			&m_usSmokeTemp,
			&m_usTubeTemp,
			&m_usDetectorTemp,
			&m_usLEDTemp,
			&m_usBoardTemp,
			&m_usLeftFanCurrent,
			&m_usRightFanCurrent,
			&m_fPowerVoltage,
			&m_usRealTimeIntensity,
			&m_usFullShadingIntensity);
	}

	// ȡ���ֵ
	// (out)pfMaxN�����͸���ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfMaxK����������ϵ��ֵָ�룬��λ��m-1������Ϊ��ָ��
	// (out)pusMaxRpm�����ת��ֵָ�룬��λ��r/min������Ϊ��ָ��
	// (out)pusMaxOilTemp���������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetMaxValue(float *const pfMaxN, float *const pfMaxK, USHORT *const pusMaxRpm, USHORT *const pusMaxOilTemp);
	inline DWORD GetMaxValue(void)
	{
		return GetMaxValue(&m_fMaxN, &m_fMaxK, &m_usMaxRpm, &m_usMaxOilTemp);
	}

	// ȡ����״̬
	// (out)pbStatus������״ֵָ̬�루0��Ԥ�ȣ�1��ʵʱ��⣬2��У׼��3���Ƶ�Ԫ�������Ԫͨ�Ŵ��󣩲���Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);
	inline DWORD GetStatus(void)
	{
		return GetStatus(&m_bStatus);
	}

	// ������״̬
	// (in)bStatus������״ֵ̬
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD SetStatus(const BYTE bStatus);

	// ȡԤ��ʣ��ʱ��
	// (out)pusMin������ֵָ�룬����δ��ָ��
	// (out)pusSec������ֵָ�룬����δ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec);
	inline DWORD GetWarmUpTime(void)
	{
		return GetWarmUpTime(&m_usMin, &m_usSec);
	}

	// ȡ��������
	// (out)pfET�������¶�ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pfRH�����ʪ��ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfAP������ѹ��ֵָ�룬��λ��kPa������Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP);
	inline DWORD GetEnvParams(void)
	{
		return GetEnvParams(&m_fET, &m_fRH, &m_fAP);
	}
	// ȡ�����¶�ֵ,��λ:���϶�
	inline float GetEnvDataETValue(void) const
	{
		return m_fET;
	}
	// ȡ����ѹ��ֵ,��λ:kPa
	inline float GetEnvDataAPValue(void) const
	{
		return m_fAP;
	}
	// ȡ���ʪ��ֵ,��λ:%
	inline float GetEnvDataRHValue(void) const
	{
		return m_fRH;
	}
	// ȡ�汾��Ϣ
	// (out)pfVersion���汾ֵָ�룬�����ǿ�ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetVersion(float *const pfVersion);
	inline DWORD GetVersion(void)
	{
		return GetVersion(&m_fVersion);
	}

	// ȡ���Ƶ�Ԫ����
	// (out)pbEngineStroke�����������
	// (out)pbRpmMeasureType��ת�ٲ�����ʽ
	// (out)pbFilterConstant���˲�����
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetControlUnitParams(enum COpa::EngineStrokeType *const pest, enum COpa::RpmMeasurementType *const prmt, enum COpa::FilterParamType *const pfpt);
	inline DWORD GetControlUnitParams(void)
	{
		return GetControlUnitParams(&m_est, &m_rmt, &m_fpt);
	}

	// ȡ��������
	// (out)pfCorrectedOilTemp����������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// NHT1L_WR_FUNC_SUCCEED
	// NHT1L_WR_FUNC_PORT_NOT_OPEN
	// NHT1L_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHT1L_WR_FUNC_EQUIP_RETURE_0X15
	// NHT1L_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetCorrectedOilTemp(float *const pfCorrectedOilTemp);
	inline DWORD GetCorrectedOilTemp(void)
	{
		return GetCorrectedOilTemp(&m_fCorrectedOilTemp);
	}
		
	// ȡ�ֶ�������������
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���������������첽��begin 
	// ��Ӧͬ��Calibrate
	void Calibrate_Asyn(HWND hCallBackWnd=NULL);
	// ��Ӧͬ��GetRealTimeData
	void GetRealTimeData_Asyn(HWND hCallBackWnd=NULL);
	// ��Ӧͬ��GetEnvParams
	void GetEnvRealTimeData_Asyn(HWND hCallBackWnd=NULL);
	// ȡ���ֵ
	void GetMaxValue_Asyn(HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���������������첽��end 

private:
	// ��Ӧ�첽��������
	afx_msg void OnCalibrate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetEnvRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMaxValue(WPARAM wParam, LPARAM lParam);

public:
	// ָ��������Ϣλ�Ƿ������ź�״̬
	// (in)bAlarmInfoBitNumber��������Ϣλ�ţ�����ÿ��������Ϣλ�ĺ�����鿴ͨѶЭ�飬ȡֵ��Χֻ����0~15
	// return�������״̬Ϊ1������true�����򣬷���false
	inline bool IsTheAlarmInfoBitSignaled(const BYTE bAlarmInfoBitNumber) const
	{
		ASSERT(bAlarmInfoBitNumber < 16);

		return m_bs16AlarmInfo.test(bAlarmInfoBitNumber);
	}
	// ��ȡ���͸���ֵ����λ��%
	inline float GetMaxNValue(void) const
	{
		return m_fMaxN;
	}
	// ��ȡ��������ϵ��ֵ����λ��m-1
	inline float GetMaxKValue(void) const
	{
		return m_fMaxK;
	}
	// ��ȡ���ת��ֵ,��λ:r/min
	inline USHORT GetMaxRpmValue(void) const
	{
		return m_usMaxRpm;
	}
	// ��ȡ�������ֵ,��λ:���϶�
	inline USHORT GetMaxOilTempValue(void) const
	{
		return m_usMaxOilTemp;
	}
	// ��ȡʵʱ��͸���ֵ,��λ:%
	inline float GetRealTimeNValue(void) const
	{
		return m_fN;
	}
	// ��ȡʵʱ������ϵ��ֵ,��λ:m-1
	inline float GetRealTimeKValue(void) const
	{
		return m_fK;
	}
	// ��ȡʵʱת��ֵ,��λ:r/min
	inline USHORT GetRealTimeRpmValue(void) const
	{
		return m_usRpm;
	}
	// ��ȡʵʱ����ֵ,��λ:���϶�
	inline USHORT GetRealTimeOilTempValue(void) const
	{
		return m_usOilTemp;
	}
	// ��ȡ����״ֵ̬
	inline BYTE GetStatusValue(void) const
	{
		return m_bStatus;
	}

	inline short GetNOValue(void) const
	{
		return m_sNO;
	}
	inline short GetNO2Value(void) const
	{
		return m_sNO2;
	}
	inline float GetCO2Value(void) const
	{
		return m_fCO2;
	}
	inline short GetHCValue(void) const
	{
		return m_sHC;
	}
	inline float GetCOValue(void) const
	{
		return m_fCO;
	}
	inline float GetO2Value(void) const
	{
		return m_fO2;
	}

public:
	// �˳��̲߳���
	void QuitThread(void);

private:
	// ��ʼ���豸
	void InitializeInstrument(const enum InstrumentModel im);
	// ж���豸
	void UninitializeInstrument(void);

private:
	// ������Ϣ
	std::bitset<16> m_bs16AlarmInfo;

	// ��͸���,��λ:%
	float m_fN;
	// ������ϵ��,��λ:m-1
	float m_fK;
	// ת��,��λ:r/min
	USHORT m_usRpm;
	// ����,��λ:���϶�
	USHORT m_usOilTemp;

	short m_sNO;
	short m_sNO2;
	float m_fCO2;
	short m_sHC;
	float m_fCO;
	float m_fO2;

	// ԭʼ��͸���ֵ,��λ:%
	float m_fOriginalN;
	// ԭʼ������ϵ��ֵ,��λ:m-1
	float m_fOriginalK;
	// �����¶�ֵ,��λ:���϶�
	USHORT m_usSmokeTemp;
	// �����ܱ�����ֵָ�룬��λ�����϶�
	USHORT m_usTubeTemp;
	// �����ֵָ�룬��λ�����϶�
	USHORT m_usDetectorTemp;
	// ����Դֵָ�룬��λ�����϶�
	USHORT m_usLEDTemp;
	// �ڲ��¶�ֵָ�룬��λ�����϶�
	USHORT m_usBoardTemp;
	// ����ȵ���ֵָ�룬��λ��δ֪
	USHORT m_usLeftFanCurrent;
	// �ҷ��ȵ���ֵָ�룬��λ��δ֪
	USHORT m_usRightFanCurrent;
	// ��Դ��ѹֵָ�룬��λ��δ֪
	float m_fPowerVoltage;
	// ʵʱ��ǿֵָ�룬��λ��δ֪
	USHORT m_usRealTimeIntensity;
	// ȫ����ǿֵָ�룬��λ��δ֪
	USHORT m_usFullShadingIntensity;

	// ���͸���ֵ����λ��%
	float m_fMaxN;
	// ��������ϵ��ֵ����λ��m-1
	float m_fMaxK;
	// ���ת��ֵ,��λ:r/min
	USHORT m_usMaxRpm;
	// �������ֵ,��λ:���϶�
	USHORT m_usMaxOilTemp;

	// ����״ֵ̬
	// 0��Ԥ�ȣ�1��ʵʱ��⣬2��У׼��3���Ƶ�Ԫ�������Ԫͨ�Ŵ���
	BYTE m_bStatus;

	// ��
	USHORT m_usMin;
	// ��
	USHORT m_usSec;

	// �����¶�,��λ:���϶�
	float m_fET;
	// ���ʪ��,��λ:%
	float m_fRH;
	// ����ѹ��,��λ:kPa
	float m_fAP;

	// �汾
	float m_fVersion;

	// ���������
	enum COpa::EngineStrokeType m_est;
	// ת�ٲ�����ʽ
	enum COpa::RpmMeasurementType m_rmt;
	// �˲�����
	enum COpa::FilterParamType m_fpt;

	// �����¶�ֵ
	float m_fCorrectedOilTemp;

	// �ϴβ���ʱ��
	clock_t m_LastOPTime;

private:
	// NHT1L������
	COpa* m_pOpa;
	// �豸�ͺ�
	InstrumentModel m_emInstrumentModel;
	// �߳��˳���־
	bool m_bQuitThread;
	// ���ڲ���״̬
	DWORD m_dwStatus;
};


