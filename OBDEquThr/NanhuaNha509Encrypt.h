/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaNha509Encrypt.h
* �ļ���ʶ��
* ժҪ��������503VͨѶ�࣬�̳��ڳ�����CAna
*       ����VMAS������ͨѶ�ӿ�
*
* ��ǰ�汾��1.0.6
* ���ߣ�Hyh
* ������ڣ�2010-07-05
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.9
* ������ڣ�2011-03-03
* ���ߣ�Ming
* �������ֱ��������HCCOCO2,O2,NOxͨ����ʱֵ�ĺ���,GetxxxDelayTime_Flowmeter(),SetxxxDelayTime_Flowmeter()��xxxDelayTime_Flowmeter()
*
* ��ǰ�汾��1.0.8
* ������ڣ�2010-08-26
* ���ߣ�Raylee
* ���������GetTestResultData()��������ȡ���������
*
* ��ǰ�汾��1.0.7
* ������ڣ�2010-08-26
* ���ߣ�Ming
* ������1)�������ݳ�ʱʱ����Ϊ2.5s,ԭΪ2s
*
* ��ǰ�汾��1.0.6
* ������ڣ�2010-07-05
* ���ߣ�Hyh
* ������Ϊ����3.6�汾������Э��,��ʷ����ȡ�������,�ֽ������ݳ�ʱʱ����Ϊ2s,ԭΪ1s
*
* ��ǰ�汾��1.0.5
* ������ڣ�2010-06-21
* ���ߣ�Hyh
* ������1)����GetProcessDataStatus_CT_Flowmeter����
*
* ��ǰ�汾��1.0.4
* ������ڣ�2010-03-17
* ���ߣ�Cui
* ������������ѹ��У׼������APCalibrate��ΪAPCalibration
*
* ��ǰ�汾��1.0.3
* ������ڣ�2009-12-21
* ���ߣ�Cui
* ������1)�޸�������ȡ��������������GetEnvironmentO2_Flowmeter
*       2)��������ƶ�У׼���ݺ�����ReadCalibrationData_Flowmeter
*
* ��ǰ�汾��1.0.2
* ������ڣ�2009-12-10
* ���ߣ�Cui
* �������޸�������������ѹ����ϡ����У׼����
*
* ��ǰ�汾��1.0.1
* ������ڣ�2009-12-10
* ���ߣ�Cui
* ��������������ƶ�д����WriteAndReadData_Flowmeter
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef NANHUA_NHA509_Encrypt_H
#define NANHUA_NHA509_Encrypt_H

#include "Ana.h"

class CNanhuaNha509Encrypt : public CAna
{
public:
	CNanhuaNha509Encrypt(void);
	~CNanhuaNha509Encrypt(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// NHC_CLOSE_SUCCEED
	// NHC_CLOSE_FAIL_WIN32API
	// NHC_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����ǻ��
	bool IsOpen(void) const;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���end

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������start 
	// 1ȡ������
	// (out)psHC,HCŨ��ָ��,��λ:ppm,����Ϊ��ָ��
	// (out)pfCO,COŨ��ָ��,��λ:%,����Ϊ��ָ��
	// (out)pfCO2,CO2Ũ��ָ��,��λ:%,����Ϊ��ָ��
	// (out)pfO2,O2Ũ��ָ��,��λ:%,����Ϊ��ָ��
	// (out)pfNO,NOŨ��ָ��,��λ:ppm,����Ϊ��ָ��
	// (out)pusRpm,ת��,��λ:r/min,����Ϊ��ָ��
	// (out)pfOilTemp,����,��λ:���϶�,����Ϊ��ָ��
	// (out)pusFlux,��������,��λ:δ֪,����Ϊ��ָ��
	// (out)pfLambda,��������ϵ��,��λ:��,����Ϊ��ָ��
	// (out)pbO2Sensor,���������Ƿ�ʧЧ,��λ:��,����Ϊ��ָ��
	// (out)pbNOSensor,NO�������Ƿ�ʧЧ,��λ:��,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetMajorData(short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL,
		short *const psNO2=NULL,
		short *const psNOx=NULL,
		USHORT *const pusRpm=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusFlux=NULL, 
		float *const pfLambda=NULL,
		bool *const pbO2Sensor=NULL,
		bool *const pbNOSensor=NULL);

	DWORD GetMajorData(short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL,
		short *const psNO2=NULL,
		short *const psNOx=NULL,
		USHORT *const pusRpm=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusFlux=NULL, 
		float *const pfLambda=NULL,
		bool *const pbO2Sensor=NULL,
		bool *const pbNOSensor=NULL,
		bool *const pbWaterLevel=NULL);

	DWORD GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx);

	// 2ȡ��������
	// ����ET:Environment Temperature(�����¶�)��д
	// ����AP:Atmospheric Pressure(����ѹ��)��д
	// ����:RH:Relative Humidity(���ʪ��)��д
	// (out)pfET:�����¶�ָ��,��λ:���϶�,����Ϊ��ָ��
	// (out)pfAP:����ѹ��ָ��,��λ:kPa,����Ϊ��ָ��
	// (out)pfRH:���ʪ��ָ��,��λ:%,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetMinorData(float *const pfET, float *const pfAP, float *const pfRH);

	// 3�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflateEnvAir(BYTE *const pbResult = NULL);

	// 4��ܵ���(����,������̽ͷ����)
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflatePipeAir(BYTE *const pbResult = NULL);

	// 5��ϴ�ܵ�(����)
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD InverseBlow(BYTE *const pbResult = NULL);

	// 6ֹͣ��ϴ(ֹͣ������ͣ��)
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StopPump(BYTE *const pbResult = NULL);

	// 7��©
	// (out)pbResult,����ִ�н��ָ��,0x00-��й¶,0x01-��й¶,0x05-���ڼ�©,����Ϊ��ָ��(���ݷ�����ͨ��Э��,Ҳ���ܷ���0x06)
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CheckLeak(BYTE *const pbResult = NULL);

	// 8����
	// (out)pbResult,����ִ�н��ָ��,0x00-����ɹ�,0x01-����ʧ��,0x05-���ڵ���,����Ϊ��ָ��(���ݷ�����ͨ��Э��,Ҳ���ܷ���0x06)
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD Zero(BYTE *const pbResult = NULL);

	// 9д��У׼��Ũ��
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��(У��ʹ���),����Ϊ��ָ��
	// (in)usCalC3H8,C3H8У׼ֵ,��λ:ppm
	// (in)fCalCO,COУ׼ֵ,��λ:%
	// (in)fCalCO2,CO2У׼ֵ,��λ:%
	// (in)usCalNO,NOУ׼ֵ,��λ:ppm
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2);

	// 10У׼
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,0x05-����У׼,����Ϊ��ָ��
	// (out)psHC,HCֵָ��,��λ:ppm,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfCO,COֵָ��,��λ:%,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfCO2,CO2ֵָ��,��λ:%,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfO2,O2ֵָ��,��λ:%,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)psNO,NOֵָ��,��λ:ppm,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfPef,Pefֵָ��,��λ:��,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GasCalibration(BYTE *const pbResult, const int nChanel, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		short *const psNO2=NULL, 
		float *const pfPef=NULL);
	
	// д��NO2У׼��Ũ��
	// (in)usNO2,NO2У׼ֵ,��λ:ppm
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2);

	// У׼NO2
	// (out)psNO2,NO2ֵָ��,��λ:ppm,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD NO2Calibration(BYTE *const pbResult, short *const psNO2=NULL);

	// 11д������Ũ��
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��(У��ʹ���),����Ϊ��ָ��
	// (in)uChkC3H8:C3H8���ֵ,��λ:ppm
	// (in)fChkCO:CO���ֵ,��λ:%
	// (in)fChkCO2:CO2���ֵ,��λ:%
	// (in)usChkNO:NO���ֵ,��λ:ppm
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO);

	// 12�Զ����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,0x05-���ڼ��,����Ϊ��ָ��
	// (out)psHC,HCֵָ��,��λ:ppm,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfCO,COֵָ��,��λ:%,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfCO2,CO2ֵָ��,��λ:%,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfO2,O2ֵָ��,��λ:%,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)psNO,NOֵָ��,��λ:ppm,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// (out)pfPef,Pefֵָ��,��λ:��,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GasCheck(BYTE *const pbResult, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		float *const pfPef=NULL);

	// 13ȡPEF
	// (out)pfPef,Pefֵָ�룬��λ���ޣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetPef(float *const pfPef);

	// 14���NO�ϻ���־
	// 503����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ClearNoSign(BYTE *const pbResult = NULL);

	// 15���õ�������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToEnvAir(BYTE *const pbResult = NULL);

	// 16���õ�������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToZeroAir(BYTE *const pbResult = NULL);

	// 17���ó��Ϊ4
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL);

	// 18���ó��Ϊ2
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL);

	// 19���õ��ʽΪ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL);

	// 20���õ��ʽΪ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL);

	// 21�����Զ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD EnableAutoZero(BYTE *const pbResult = NULL);

	// 22��ֹ�Զ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DisableAutoZero(BYTE *const pbResult = NULL);

	// 23������̲���

	// 24��ֹ���̲���

	// 25LCD��

	// 26LCD��

	// 27�����¶�У׼
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,����Ϊ��ָ��
	// (in)fCalET:�����¶�У׼ֵ,��λ:���϶�
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ETCalibration(BYTE *const pbResult, const float fCalET);

	// 28����У׼

	// 29����У׼
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,����Ϊ��ָ��
	// (in)fCalOT:����У׼ֵ,��λ:���϶�
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OTCalibration(BYTE *const pbResult, const float fCalOT);

	// 30����ʪ��У׼
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,����Ϊ��ָ��
	// (in)fCalRH:���ʪ��У׼ֵ,��λ-%
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD RHCalibration(BYTE *const pbResult, const float fCalRH);

	// 31����ѹ��У׼
	// ���غ���ִ�н����
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,����Ϊ��ָ��
	// (in)fCalAP:����ѹ��У׼ֵ,��λ:kPa
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD APCalibration(BYTE *const pbResult, const float fCalAP);

	// 32�������
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OpenCheckGas(BYTE *const pbResult = NULL);

	// 33�������
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CloseCheckGas(BYTE *const pbResult = NULL);

	// 34ȡ����״̬
	// (out)pbStatus,����״ָ̬��,0x00-����(δ����),0x01-���ڼ��(�ѿ���),0x02-����Ԥ��,0x03-���ڼ�©,�ȵ�(����״̬�뺬����Ҫ����ͨѶЭ��),����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);

	// 35ȡԤ��ʣ��ʱ��
	// (out)pusSecondsLeft:Ԥ��ʣ��ʱ��ָ��,Ӧ��[AA][BB],ʣ��ʱ��=[AA]*256+[BB]��;����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetWarmUpTimeLeft(USHORT *const pusSecondsLeft);

	// 36����ȼ������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToGas(BYTE *const pbResult = NULL);

	// 37����ȼ������ΪҺ��ʯ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToLPG(BYTE *const pbResult = NULL);

	// 38����ȼ������Ϊ��Ȼ��
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToNG(BYTE *const pbResult = NULL);

	// 39����ȼ������Ϊ�Ҵ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFuelTypeToGasohol(BYTE *const pbResult = NULL);

	// ����ģʽת��
	// (in)op:����ģʽ,ToAnalyzer-������ͨѶģʽ,ToFlowmeter-������ͨѶģʽ
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetOperationMode(const enum OperationMode op, BYTE *const pbResult=NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��״̬begin
	// ȡ����ִ��״̬(�ж������������Ƿ��Ѿ�ִ�����)
	// ��Ҫ�����ж�ϡ����У׼����Ҫ�ȴ��������Ƿ��Ѿ�ִ�����
	// (out)pbResult�������������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	// һ��ֻ�᷵��0x06����Ϊ���Ǽ�ʱ����
	// (out)pwProcessStatus������ִ��״ָ̬�룬0-����������ִ�У���0-����������ִ��
	// (out)pwProcess������ִ��״̬��������ָ��
	// (return)
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetProcessStatus_Flowmeter(BYTE *const pbResult=NULL, WORD *const pwProcessStatus=NULL, WORD *const pwProcess=NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��״̬end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�ռ��������begin
	// ��ʼ�ռ�����
	// (int)wCollectionTime:�ռ�ʱ��,��λ:s(��)��VMAS�����ƣ����ֻ����600�롣
	// (out)pwMiniStatus:״̬�����庬����ο�ͨѶЭ��
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StartCollectData_Flowmeter(const WORD wCollectionTime=195, WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL); 

	// ֹͣ�ռ�����
	// (out)pwMiniStatus:״̬�����庬����ο�ͨѶЭ�顣����ΪNULL
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StopCollectData_Flowmeter(WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL);

	// ���������ݺ�״̬(����ģʽ)
	// (in)wCntStart:�������ݿ�ʼ��ţ�ֻ����1~600
	// (in)wCntLength:�������ݳ��ȣ�ֻ����1~600
	// (out)pfHCwt:HC����ָ�룻��λ������(mg)������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfCOwt:CO����ָ�룻��λ������(mg)������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfCO2wt:CO2����ָ�룻��λ������(mg)������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfNOwt:NO����ָ�룻��λ������(mg)������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pwMiniStatus:״̬�����庬����ο�ͨѶЭ�顣����ΪNULL
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL);

	// ���������ݺ�״̬(Ũ��ģʽ) 
	// (in)wCntStart:�������ݿ�ʼ��ţ�ֻ����1~600
	// (in)wCntLength:�������ݳ��ȣ�ֻ����1~600
	// (out)psHCct:HCŨ��ָ��,��λ:ppm������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)psCOct:COŨ��ָ��,��λ:%������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)psCO2ct:CO2Ũ��ָ��,��λ:%������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)psNOct:NOŨ��ָ��,��λ:ppm������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfDilO2:ϡ����Ũ��ָ��,��λ:%������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfBenchO2:��������Ũ��ָ��,��λ:%������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)psGasTemp;ϡ�������¶�,��λ:���϶ȡ�����ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfGasFluxStd0:ϡ������0���׼����,��λ:L/s������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfGasFluxStd20:ϡ������20���׼����,��λ:L/s������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfExhaustFluxStd0:����0������,��λ:L/s������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfExhaustFluxStd20:����20������,��λ:L/s������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfGasPres:ϡ������ѹ��,��λ:kPa������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pfGasFluxAct:ϡ������ʵ������,��λ:L/s������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pusRpm:������ת��,��λ:r/min������ΪNULL����ָ����ڴ��С������ڵ���wCntLength
	// (out)pwMiniStatus:״̬�����庬����ο�ͨѶЭ�顣����ΪNULL
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct=NULL, 
		float *const pfCOct=NULL, 
		float *const pfCO2ct=NULL, 
		short *const psNOct=NULL, 
		float *const pfDilO2=NULL, 
		float *const pfBenchO2=NULL, 
		short *const psGasTemp=NULL, 
		float *const pfGasFluxStd0=NULL, 
		float *const pfGasFluxStd20=NULL, 
		float *const pfExhaustFluxStd0=NULL, 
		float *const pfExhaustFluxStd20=NULL, 
		float *const pfGasPres=NULL, 
		float *const pfGasFluxAct=NULL, 
		USHORT *const pusRpm=NULL,
		WORD *const pwMiniStatus=NULL,
		const BYTE bDataMode=0x01,
		BYTE *const pbResult=NULL);

	// ��ȡ�ܿ���(mg) 
	// (out)pusHCwt,HC������ָ��,��λ:mg(����)������ΪNULL
	// (out)pusCOwt,CO������ָ��,��λ:mg(����)������ΪNULL
	// (out)pusCO2wt,CO2������ָ��,��λ:mg(����)������ΪNULL
	// (out)pusNOwt,NO������ָ��,��λ:mg(����)������ΪNULL
	// (out)pfExhaustVolume:�������,��λ:L������ΪNULL
	// (out)pfGasVolume:ϡ���������,��λ:L������ΪNULL
	// (out)pwMiniStatus:״̬�����庬����ο�ͨѶЭ�顣����ΪNULL
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL);

	// ��ȡ�Ѿ��ռ����ݵ�����
	// (out)pwCorrectedDataNumber:�Ѿ��ռ����ݸ���ָ�룻�������ִ�д����򷵻�0x00������ΪNULL
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult=NULL);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�ռ��������end


	// ��ȡ������ʵʱ����
	// pfFluxStd0:0���׼������L/s
	// pfFluxStd20:20���׼������L/s
	// pfFluxAct:ʵ��������L/s
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);
	
	// ��ȡ������ȫ��ʵʱ����
	// pfFluxStd0:0���׼������L/s
	// pfFluxStd20:20���׼������L/s
	// pfFluxAct:ʵ��������L/s
	// (out)pbResult:�����������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)�� 0x15-ʧ��(��������)
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����У׼���begin
	// TODO:��Ҫ��������
	// ���ñ�׼����
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux);

	// TODO:��Ҫ��������
	// ����У׼ 
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����У׼���end


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ѹ��У׼���begin
	// TODO:��Ҫ��������
	// ���ñ�׼ѹ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);

	// TODO:��Ҫ��������
	// ѹ��У׼ 
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ѹ��У׼���end
	
	//�������¶�У׼��ע�⣺���������ƻ�о��֧��
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);
	DWORD TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ϡ����У׼���begin
	// TODO:��Ҫ��������
	// ���ñ�׼����
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2);

	// ����У׼
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);
	DWORD DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);
	DWORD DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);
	DWORD DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ϡ����У׼���end
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����ʱУ׼���begin
	// ����HC/CO/CO2ͨ����ʱֵ
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// ����O2ͨ����ʱֵ
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// ����NOxͨ����ʱֵ
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����ʱУ׼���end

	// TODO:��Ҫ��������
	// ��ʼ�ɼ�������
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SatrtGetEnvironmentO2_Flowmeter(void);
	DWORD GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2);
  
	// ���ط��
	// (in)bRun:���ط��������true-�������false-�ط��
	// (out)pbResult:�����������ָ�룻0x06-����ģʽ�ɹ���0x05-����ģʽʧ��(������æ)�� 0x15-����ģʽʧ��(��������)
	//               �������ִ�й��̴��󣬷���0xFF
	//				 ����ΪNULL
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);

protected:
	// д����������
	// д����Ӧ�������ȡ��Ӧ�ķ�������
	// ��������Ҫ���ֱ��д��Ͷ�ȡ�����ݽ���У�����֤
	// (in)pbWriteBuffer��Ҫд�����������ָ�룬����Ϊ��ָ�룬���Ϊ��ָ�룬�򲻽���д���ݲ���
	// (in)dwWriteLength��Ҫд������ݳ��ȣ��ֽ���������pbWriteBufferΪ��ָ��ʱ���������������
	// (out)pbReadBuffer��Ҫ��ȡ����������ָ�룬����Ϊ��ָ��
	// (in)dwReadLength��Ҫ��ȡ�����ݳ��ȣ��ֽ�����
	// (in)bNeedWriteChkSum��true����ҪдУ�飬false������Ҫ����pbWriteBufferΪ��ָ��ʱ���������������
	// (in)bNeedReadChkSum��true����Ҫ��У�飬false������Ҫ
	// (in)bPurgeComm,�Ƿ����ô��ڲ���,true,���ô��ڲ���(ִ��PurgeComm����),false,�����ô��ڲ���(��ִ��PurgeComm����)
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *const pbWriteBuffer, 
		const DWORD dwWriteLength, 
		BYTE *const pbReadBuffer, 
		const DWORD dwReadLength, 
		const bool bNeedWriteChkSum=true, 
		const bool bNeedReadChkSum=true, 
		const bool bPurgeComm=true);

	// ����ǰnλ��У���
	// ��ӣ�Ȼ��ȡ����ֽ�λ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	BYTE CheckSum(const BYTE *const pb, const UINT un);

	// ֻ����06���ֽڽ���������ʽ
	// ��������һ��Ҳ�ǵ��ֽ�
	// ���󲿷ַ�����������������������ʽ
	// (in)bCmd,���ֽ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult = NULL);

	// ֻ����00,01,05��3�ֵ��ֽڽ���е�����һ�ֵ������ʽ,�����05,���Դ���������
	// ����ͼ�©�����������������ʽ
	// У׼���Զ��������Ҳ�������������ʽ,���ҵ�����05ʱ,���к�������
	// (in)bCmd,���ֽ�����
	// (out)pbResult,����ִ�н��ָ��,0x00-�����ɹ�,0x01-����ʧ��,0x05-���ڲ���;���û�к�������,�����Ϊ��ָ��,����к�������,����Ϊ��ָ��
	// (out)psHC,HCŨ��ֵָ��,��λ-ppm
	// (out)pfCO,COŨ��ֵָ��,��λ-%
	// (out)pfCO2,CO2Ũ��ֵָ��,��λ-%
	// (out)pfO2,O2Ũ��ֵָ��,��λ-%
	// (out)psNO,NOŨ��ֵָ��,��λ-ppm
	// (out)pfPef,Pefֵָ��
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult = NULL);
	DWORD CmdFormat_Return00_01_05(const BYTE bCmd, 
		BYTE *const pbResult, 
		short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		float *const pfPef);

private:
	// ֻ����06,00,01��3�ֵ��ֽڽ���е�����һ�ֵ������ʽ
	// д��У׼�������д�����������������������ʽ
	// �����¶�,����,����ѹ��,���ʪ��У׼Ҳ�������������ʽ
	// (in)bCmd,���ֽ�����
	// (out)pbResult,����ִ�н��ָ��,0x00-�����ɹ�,0x01-����ʧ��,����Ϊ��ָ��
	// usC3H8,C3H8Ũ��ֵ,��λ-ppm
	// fCO,COŨ��ֵ,��λ-%
	// fCO2,CO2Ũ��ֵ,��λ-%
	// usNO,NOŨ��ֵ,��λ-ppm
	// fValue:�����¶�(�������»��ߴ���ѹ���������ѹ��)У׼ֵ
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO);
	DWORD CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue);

private:
	// ֻ����00,01��2�ֵ��ֽڽ���е�����һ�ֵ������ʽ,ֻ����503BЭ��
	// д��У׼�������д�����������������������ʽ
	// (in)bCmd,���ֽ�����
	// (out)pbResult,����ִ�н��ָ��,0x00-�����ɹ�,0x01-����ʧ��(У���ʧ��),����Ϊ��ָ��
	// usC3H8,C3H8Ũ��ֵ,��λ-ppm
	// fCO,COŨ��ֵ,��λ-%
	// fCO2,CO2Ũ��ֵ,��λ-%
	// usNO,NOŨ��ֵ,��λ-ppm
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO);

	// ֻ����00,01,02��3�ֵ��ֽڽ���е�����һ�ֵ������ʽ,ֻ����503BЭ��
	// �����¶�У׼,����У׼,����ѹ��У׼,���ʪ��У׼�����������������ʽ
	// (in)bCmd,���ֽ�����
	// (out)pbResult,����ִ�н��ָ��,0x00-�����ɹ�,0x01-����ʧ��,0x02-У���ʧ��,����Ϊ��ָ��
	// fValue:�����¶�(�������»��ߴ���ѹ���������ѹ��)У׼ֵ
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue);

protected:
	// д����������(������������ͨѶ)
	// д����Ӧ�������ȡ��Ӧ�ķ�������
	// ��������Ҫ���ֱ��д��Ͷ�ȡ�����ݽ���У�����֤
	// (in)pbWriteBuffer��Ҫд�����������ָ�룬����Ϊ��ָ�룬���Ϊ��ָ�룬�򲻽���д���ݲ���
	// (in)dwWriteLength��Ҫд������ݳ��ȣ��ֽ���������pbWriteBufferΪ��ָ��ʱ���������������
	// (out)pbReadBuffer��Ҫ��ȡ����������ָ�룬����Ϊ��ָ��
	// (in)dwReadLength��Ҫ��ȡ�����ݳ��ȣ��ֽ�����
	// (in)bNeedWriteChkSum��true����ҪдУ�飬false������Ҫ����pbWriteBufferΪ��ָ��ʱ���������������
	// (in)bNeedReadChkSum��true����Ҫ��У�飬false������Ҫ
	// (in)bPurgeComm,�Ƿ����ô��ڲ���,true,���ô��ڲ���(ִ��PurgeComm����),false,�����ô��ڲ���(��ִ��PurgeComm����)
	// ���أ�����ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
		const DWORD dwWriteLength, 
		BYTE *const pbReadBuffer, 
		const DWORD dwReadLength, 
		const bool bNeedWriteChkSum=true, 
		const bool bNeedReadChkSum=true);

	// ����ǰnλ��У���
	// ��ӣ�Ȼ��ȡ����ֽ�λ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	WORD CheckSum_Flowmeter(const BYTE *const pb, const UINT un);

	// ��У׼����(Read calibration data)
	// ��������Ҫ���ֱ��д��Ͷ�ȡ�����ݽ���У�����֤
	// (out)pbResult�������������ָ�룻0x06-�ɹ���0x05-ʧ��(������æ)��0x15-ʧ��(��������)
	// (in)bCalMode��У׼ģʽ������������ο�������˵����(����0x03����ȡ������)
	// (in)pbReadBuf��������
	// (in)dwBytesToRead����ȡ�ֽ�����
	// (return)
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead);
	
	// ��ȡ�ֶ���������
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetTestResultData(short *const psHCHigh=NULL, 
		float *const pfCOHigh=NULL, 
		float *const pfCO2High=NULL, 
		float *const pfO2High=NULL, 
		short *const psNOHigh=NULL,
		USHORT *const pusRpmHigh=NULL, 
		float *const pfOilTempHigh=NULL, 
		float *const pfLambdaHigh=NULL,
		short *const psHCLow=NULL, 
		float *const pfCOLow=NULL, 
		float *const pfCO2Low=NULL, 
		float *const pfO2Low=NULL, 
		short *const psNOLow=NULL,
		USHORT *const pusRpmLow=NULL, 
		float *const pfOilTempLow=NULL, 
		float *const pfLambdaLow=NULL);
	
	DWORD Authentication(void);
	
	UINT m_unTimerID;
	static void CALLBACK TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

protected:
	//// ���ھ��
	//HANDLE m_hComm;
	//// �ٽ籣����
	//CRITICAL_SECTION cInfoSection;
	//// Ŀǰ����ͨѶ������
	//enum OperationMode m_opUsedNow;
};
#endif