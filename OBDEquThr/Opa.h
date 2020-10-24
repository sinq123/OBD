/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�Opa.h
* �ļ���ʶ��
* ժҪ����͸��ȼƳ�����
*		�Դ����⺯����ʽ���岻͸��ȼƲ����ӿ�
*
* ��ǰ�汾��1.0
* ���ߣ�Cui,hyh
* ������ڣ�2009-12-08
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-08-26
* ���ߣ�Raylee
* ���������GetTestResultData()��������ȡ�������
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef OPA_H
#define OPA_H

// ����ִ�н��
#define OPA_OPEN_SUCCEED						0x00  // �򿪴��ڳɹ�
#define OPA_OPEN_FAIL							0x01  // �򿪴���ʧ��
#define OPA_CLOSE_SUCCEED						0x00  // �رմ��ڳɹ�
#define OPA_CLOSE_FAIL_WIN32_API				0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define OPA_CLOSE_FAIL_NO_OPEN_PORT				0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���
#define OPA_WR_SUCCEED							0x00  // д���ɹ�
#define OPA_WR_PORT_NOT_OPEN					0x01  // д��ʧ�ܣ�����δ��
#define OPA_WR_READ_SPECIFIED_DATA_FAIL			0x02  // д��ʧ�ܣ���ָ������ʧ��
#define OPA_WR_WRITE_SPECIFIED_DATA_FAIL		0x03  // д��ʧ�ܣ�дָ������ʧ��
#define OPA_WR_EQUIP_RETURE_0X15				0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define OPA_WR_CHECK_SUM_FAIL					0x05  // д��ʧ�ܣ���������У���ʧ��
#define OPA_WR_UNKNOWN_ERROR					0xFF  // д��ʧ�ܣ�δ֪����

#include <bitset>

class COpa
{
public:
	COpa(void);
	virtual ~COpa(void);

public:
	// ��������
	enum ParamType
	{
		// ���������
		EngineStroke = 0x01,		
		// ת�ٲ�����ʽ
		RpmMeasurement = 0x02,
		// �˲�����
		FilterParam = 0x03,
		// �ָ���������
		RestoreFactorySetting = 0x04
	};
	// �������
	enum EngineStrokeType
	{
		// 4���
		FourStroke = 0x01,
		// 2���
		TwoStroke = 0x02,
	};
	enum RpmMeasurementType
	{
		// ѹ��ʽ
		Piezoelectric = 0x01,
		// ���ʽ
		Photoelectric = 0x02,
		// ���ʽ
		BatteryType = 0x03,
	};
	// �˲�����
	enum FilterParamType
	{
		// �˲�ģʽ1
		FilterMode1 = 0x01,
		// �˲�ģʽ2
		FilterMode2 = 0x02,
		// ���˲�
		NoFilter = 0x03
	};
	struct ControlUnitParams
	{
		ParamType pt;
		union ParamValue
		{
			EngineStrokeType est;
			RpmMeasurementType rmt;
			FilterParamType fpt;
			enum {NoParam = 0x00};
		} pv;
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

	// ��֪������ϵ��K����L(mm)����ͨ���µĲ�͸���N
	float K2N(const float fK, const unsigned short usL);

	// ��֪L(mm)����ͨ���µĲ�͸���N���������ϵ��K
	float N2K(const float fN, const unsigned short usL);

	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// OPA_OPEN_SUCCEED
	// OPA_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// OPA_CLOSE_SUCCEED
	// OPA_CLOSE_FAIL_WIN32_API
	// OPA_CLOSE_FAIL_NO_OPEN_PORT
	virtual DWORD Close(void) = 0;

	// �жϴ����ǻ��
	virtual bool IsOpen(void) const = 0;

	// ȡ�������صĴ�����
	virtual DWORD GetErrorCodeFromEquipment(void) const = 0;

	// ��ֹԤ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD StopWarmUp(void) = 0;

	// У׼
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD Calibrate(void) = 0;

	// ������ֵ
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD ClearMaxValue(void) = 0;

	// ���ÿ��Ƶ�Ԫ����
	// (in)bParamType����������
	// (out)bParamValue������ֵ
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	// DWORD SetControlUnitParams(const enum ParamType pt);
	// DWORD SetControlUnitParams(const enum ParamType pt, const EngineStrokeType est);
	// DWORD SetControlUnitParams(const enum ParamType pt, const RpmMeasurementType rmt);
	// DWORD SetControlUnitParams(const enum ParamType pt, const FilterParamType fpt);
	virtual DWORD SetControlUnitParams(const ControlUnitParams *const psControlUnitParams) = 0;
	virtual DWORD SetControlUnitParams(const BYTE bParamType, const BYTE bParamValue) = 0;

	// �������ʪ��
	// (in)fRHValue����׼ֵ����λ��%
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD SetRH(const float fStdValue) = 0;

	// ��λEEPROM����
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD ResetEEPROM(void) = 0;

	// ȡ����������Ϣ
	// (out)pbs16AlarmInfo��������Ϣָ�룬��16λ������λ��Ӧ�ĺ�����ο�ͨѶ˵���飬����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo) = 0;

	// ȡʵʱ����
	// (out)pfN����͸���ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfK��������ϵ��ֵָ�룬��λ��m-1������Ϊ��ָ��
	// (out)pusRpm��ת��ֵָ�룬��λ��r/min������Ϊ��ָ��
	// (out)pusOilTemp������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp) = 0;
		
	// ȡʵʱ����(���ӵ������)
	// (out)pfN����͸���ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfK��������ϵ��ֵָ�룬��λ��m-1������Ϊ��ָ��
	// (out)pusRpm��ת��ֵָ�룬��λ��r/min������Ϊ��ָ��
	// (out)pusOilTemp������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pfNO��NOŨ��ָ��,��λ:ppm,����Ϊ��ָ��
	// (out)pfNO2��NO2Ũ��ָ��,��λ:ppm,����Ϊ��ָ��
	// (out)pfCO2��CO2Ũ��ָ��,��λ:%,����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp,
						short *const psNO, short *const psNO2, float *const pfCO2, 
						short *const psHC, float *const pfCO, float *const pfO2) = 0;

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
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMeasureUnitData(float *const pfOriginalN=NULL, 
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
		USHORT *const pusFullShadingIntensity=NULL) = 0;

	// ȡ���ֵ
	// (out)pfMaxN�����͸���ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfMaxK����������ϵ��ֵָ�룬��λ��m-1������Ϊ��ָ��
	// (out)pusMaxRpm�����ת��ֵָ�룬��λ��r/min������Ϊ��ָ��
	// (out)pusMaxOilTemp���������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMaxValue(float *const pfMaxN=NULL, float *const pfMaxK=NULL, USHORT *const pusMaxRpm=NULL, USHORT *const pusMaxOilTemp=NULL) = 0;

	// ȡ����״̬
	// (out)pbStatus������״ֵָ̬�루״ֵ̬����ο�˵���飩����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetStatus(BYTE *const pbStatus) = 0;

	// ������״̬
	// (int)bStatus������״ֵָ̬�루״ֵ̬����ο�˵���飩
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD SetStatus(const BYTE bStatus) = 0;

	// ȡԤ��ʣ��ʱ��
	// (out)pusMin������ֵָ�룬����δ��ָ��
	// (out)pusSec������ֵָ�룬����δ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec) = 0;

	// ȡ��������
	// (out)pfET�������¶�ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// (out)pfRH�����ʪ��ֵָ�룬��λ��%������Ϊ��ָ��
	// (out)pfAP������ѹ��ֵָ�룬��λ��kPa������Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP) = 0;

	// ȡ�汾��Ϣ
	// (out)pfVersion���汾ֵָ�룬�����ǿ�ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetVersion(float *const pfVersion) = 0;

	// ȡ���Ƶ�Ԫ����
	// (out)pbEngineStroke�����������
	// (out)pbRpmMeasureType��ת�ٲ�����ʽ
	// (out)pbFilterConstant���˲�����
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt) = 0;
	virtual DWORD GetControlUnitParams(BYTE *const pbEngineStroke, BYTE *const pbRpmMeasureType, BYTE *const pbFilterConstant) = 0;

	// ȡ��������
	// (out)pfCorrectedOilTemp����������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetCorrectedOilTemp(float *const pfCorrectedOilTemp) = 0;

	// ȡ�ֶ�������������
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	virtual DWORD GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm ,float *const pfValueKAvg) = 0;
		
	//����������У׼״̬
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD EntrySensitivityCalStatus(void) = 0;

	//������У׼
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD ProceedSensitivityCal(float const pfValue) = 0;

	//�˳�������У׼״̬
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD QuitSensitivityCalStatus(void) = 0;

protected:
	// ���ھ��
	HANDLE m_hComm;
	// �ٽ籣����
	CRITICAL_SECTION cInfoSection;
};
#endif
