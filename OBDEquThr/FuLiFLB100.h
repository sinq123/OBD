/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FuLiFLB100.h
* �ļ���ʶ��
* ժҪ��������͸��ȼ�FLB-100ͨѶ�࣬�̳��ڳ�����COpa
*		
*
* �汾��1.0
* ���ߣ�Raylee
* ������ڣ�2012-10-11
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ߣ�Raylee
* ���ڣ�2012-10-11
* ����������ʹ��
*/

#pragma once

#ifndef FULI_FLB100_H
#define FULI_FLB100_H

#include "Opa.h"

class CFuLiFLB100 : public COpa
{
public:
	CFuLiFLB100(void);
	~CFuLiFLB100(void);

public:
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// OPA_OPEN_SUCCEED
	// OPA_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// OPA_CLOSE_SUCCEED
	// OPA_CLOSE_FAIL_WIN32API
	// OPA_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����ǻ��
	bool IsOpen(void) const;

	// ȡ�������صĴ�����
	DWORD GetErrorCodeFromEquipment(void) const;

	// ��ֹԤ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD StopWarmUp(void);

	// У׼
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD Calibrate(void);

	// ������ֵ
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD ClearMaxValue(void);

	// ���ÿ��Ƶ�Ԫ����
	// (in)bParamType����������
	// (out)bParamValue������ֵ
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	//DWORD SetControlUnitParams(const enum ParamType pt);
	//DWORD SetControlUnitParams(const enum ParamType pt, const EngineStrokeType est);
	//DWORD SetControlUnitParams(const enum ParamType pt, const RpmMeasurementType rmt);
	//DWORD SetControlUnitParams(const enum ParamType pt, const FilterParamType fpt);
	DWORD SetControlUnitParams(const ControlUnitParams *const psControlUnitParams);
	DWORD SetControlUnitParams(const BYTE bParamType, const BYTE bParamValue);

	// �������ʪ��
	// (in)fRHValue����׼ֵ����λ��%
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD SetRH(const float fStdValue);

	// ��λEEPROM����
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD ResetEEPROM(void);

	// ȡ����������Ϣ
	// (out)pbs16AlarmInfo��������Ϣָ�룬��16λ������λ��Ӧ�ĺ�����ο�ͨѶ˵���飬����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo);

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
	DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp);
	DWORD GetRealTimeData(float *const pfN, float *const pfK, USHORT *const pusRpm, USHORT *const pusOilTemp,
						short *const psNO, short *const psNO2, float *const pfCO2, 
						short *const psHC, float *const pfCO, float *const pfO2)
	{
		return OPA_WR_EQUIP_RETURE_0X15;
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
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
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
	DWORD GetMaxValue(float *const pfMaxN=NULL, float *const pfMaxK=NULL, USHORT *const pusMaxRpm=NULL, USHORT *const pusMaxOilTemp=NULL);

	// ȡ����״̬
	// (out)pbStatus������״ֵָ̬�루0��Ԥ�ȣ�1��ʵʱ��⣬2��У׼��3���Ƶ�Ԫ�������Ԫͨ�Ŵ��󣩲���Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);

	// ������״̬
	// (int)bStatus������״ֵָ̬�루״ֵ̬����ο�˵���飩
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD SetStatus(const BYTE bStatus);

	// ȡԤ��ʣ��ʱ��
	// (out)pusMin������ֵָ�룬����δ��ָ��
	// (out)pusSec������ֵָ�룬����δ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetWarmUpTime(USHORT *const pusMin, USHORT *const pusSec);

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
	DWORD GetEnvParams(float *const pfET, float *const pfRH, float *const pfAP);

	// ȡ�汾��Ϣ
	// (out)pfVersion���汾ֵָ�룬�����ǿ�ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetVersion(float *const pfVersion);

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
	DWORD GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt);
	DWORD GetControlUnitParams(BYTE *const pbEngineStroke, BYTE *const pbRpmMeasureType, BYTE *const pbFilterConstant);

	// ȡ��������
	// (out)pfCorrectedOilTemp����������ֵָ�룬��λ�����϶ȣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetCorrectedOilTemp(float *const pfCorrectedOilTemp);
		
	// ȡ�ֶ�������������
	// ���غ���ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg);
	
	//����������У׼״̬
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD EntrySensitivityCalStatus(void){return OPA_WR_EQUIP_RETURE_0X15;}

	//������У׼
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD ProceedSensitivityCal(float const pfValue){return OPA_WR_EQUIP_RETURE_0X15;}

	//�˳�������У׼״̬
	// ���غ���ִ�н����
	// OPA_WR_FUNC_SUCCEED
	// OPA_WR_FUNC_PORT_NOT_OPEN
	// OPA_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_FUNC_EQUIP_RETURE_0X15
	// OPA_WR_FUNC_CHECK_SUM_FAIL
	DWORD QuitSensitivityCalStatus(void){return OPA_WR_EQUIP_RETURE_0X15;}

protected:
	// д����������
	// д����Ӧ�������ȡ��Ӧ�ķ�������
	// ��������Ҫ���ֱ��д��Ͷ�ȡ�����ݽ���У�����֤
	// (in)pbWriteBuffer��Ҫд����������ݣ�����Ϊ��ָ��
	// (in)dwWriteLength��Ҫд������ݳ���
	// (out)pbReadBuffer��Ҫ��ȡ���������ݣ�����Ϊ��ָ��
	// (in)dwReadLength��Ҫ��ȡ�����ݳ���
	// (in)bNeedWriteChkSum��true����ҪдУ�飬false������Ҫ
	// (in)bNeedReadChkSum��true����Ҫ��У�飬false������Ҫ
	// ���أ�����ִ�н����
	// OPA_WR_SUCCEED
	// OPA_WR_PORT_NOT_OPEN
	// OPA_WR_READ_SPECIFIED_DATA_FAIL
	// OPA_WR_EQUIP_RETURE_0X15
	// OPA_WR_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum=true, bool bNeedReadChkSum=true);

	// ����У���
	// ��ӣ�Ȼ��ȡ����һ
	// (in)p��Ҫ����У�����������
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	BYTE CheckSum(BYTE *p, UINT un);
protected:
	//// ���ھ��
	//HANDLE m_hComm;
	// ״̬��:0,Ԥ��״̬;1,ʵʱ���״̬;2,У׼״̬;3,���Ƶ�Ԫ�������Ԫͨ�Ŵ���;
	BYTE m_bStatus;
	// ���һ���Ƿ������ָ���״̬��
	BYTE m_bLastErrorCode;
private:
	////�ͻ���Ϣ�ٽ籣����
	//CRITICAL_SECTION cInfoSection;
};
#endif