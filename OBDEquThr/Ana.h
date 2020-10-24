/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�Ana.h
* �ļ���ʶ��
* ժҪ�������ǳ�����
*		�Դ����⺯����ʽ��������ǲ����ӿ�
*
* ��ǰ�汾��1.0.3
* ���ߣ�Hyh
* ������ڣ�2010-06-21
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.6
* ������ڣ�2014-05-10
* ���ߣ�Lihy
* ��������ӹ��������Ƹ��е�����У׼����
*
* ��ǰ�汾��1.0.5
* ������ڣ�2011-03-03
* ���ߣ�Ming
* �������ֱ��������HCCOCO2,O2,NOxͨ����ʱֵ�ĺ���,GetxxxDelayTime_Flowmeter(),SetxxxDelayTime_Flowmeter()��xxxDelayTime_Flowmeter()
*
* ��ǰ�汾��1.0.4
* ������ڣ�2010-08-26
* ���ߣ�Raylee
* ���������GetTestResultData()�����ӿڣ���ȡ���������
*
* ��ǰ�汾��1.0.3
* ������ڣ�2010-06-21
* ���ߣ�Hyh
* ������1)����GetProcessDataStatus_CT_Flowmeter����
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-03-17
* ���ߣ�Cui
* ������������ѹ��У׼������APCalibrate��ΪAPCalibration
*
* ��ǰ�汾��1.0.1
* ������ڣ�2009-12-10
* ���ߣ�Cui
* �������޸�������������ѹ����ϡ����У׼����
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef ANA_H
#define ANA_H

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

// ����ִ�н��
#define ANA_OPEN_SUCCEED							0x00  // �򿪴��ڳɹ�
#define ANA_OPEN_FAIL								0x01  // �򿪴���ʧ��
#define ANA_CLOSE_SUCCEED							0x00  // �رմ��ڳɹ�
#define ANA_CLOSE_FAIL_WIN32_API					0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define ANA_CLOSE_FAIL_NO_OPEN_PORT					0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���
#define ANA_WR_SUCCEED								0x00  // д���ɹ�
#define ANA_WR_PORT_NOT_OPEN						0x01  // д��ʧ�ܣ�����δ��
#define ANA_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // д��ʧ�ܣ���ָ������ʧ��
#define ANA_WR_READ_SPECIFIED_DATA_FAIL				0x03  // д��ʧ�ܣ���ָ������ʧ��
#define ANA_WR_EQUIP_RETURE_0X15					0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define ANA_WR_CHECK_SUM_FAIL						0x05  // д��ʧ�ܣ���������У���ʧ��
#define ANA_WR_UNKNOWN_ERROR						0xFF  // д��ʧ�ܣ�δ֪����

class CAna
{
public:
	CAna(void);
	virtual ~CAna(void);

public:
	// �����ھ��
	HANDLE GetComHandle(void);
	void AttachComHandle(HANDLE hCom);
	void DetachComHandle(void);
	// �����ٽ���
	CRITICAL_SECTION GetCriticalSection(void);
	void AttachCriticalSection(CRITICAL_SECTION cs);
	void DetachCriticalSection(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// ANA_CLOSE_SUCCEED
	// ANA_CLOSE_FAIL_WIN32API
	// ANA_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// �жϴ����ǻ��
	virtual bool IsOpen(void) const = 0;
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
	// (out)pbWaterFull,�ж�ˮλ�Ƿ�����,��λ:�ޣ��£�
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetMajorData(short *const psHC=NULL, 
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
		bool *const pbNOSensor=NULL) = 0;

	virtual DWORD GetMajorData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO,
		short *const psNO2,
		short *const psNOx,
		USHORT *const pusRpm, 
		float *const pfOilTemp, 
		USHORT *const pusFlux, 
		float *const pfLambda,
		bool *const pbO2Sensor,
		bool *const pbNOSensor,
		bool *const pbWaterFull) = 0;

	virtual DWORD GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx) = 0;

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
	virtual DWORD GetMinorData(float *const pfET, float *const pfAP, float *const pfRH) = 0;

	// 3�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DeflateEnvAir(BYTE *const pbResult = NULL) = 0;

	// 4��ܵ���(����,������̽ͷ����)
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DeflatePipeAir(BYTE *const pbResult = NULL) = 0;

	// 5��ϴ�ܵ�(����)
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD InverseBlow(BYTE *const pbResult = NULL) = 0;

	// 6ֹͣ��ϴ(ֹͣ������ͣ��)
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD StopPump(BYTE *const pbResult = NULL) = 0;

	// 7��©
	// (out)pbResult,����ִ�н��ָ��,0x00-��й¶,0x01-��й¶,0x05-���ڼ�©,����Ϊ��ָ��(���ݷ�����ͨ��Э��,Ҳ���ܷ���0x06)
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD CheckLeak(BYTE *const pbResult = NULL) = 0;

	// 8����
	// (out)pbResult,����ִ�н��ָ��,0x00-����ɹ�,0x01-����ʧ��,0x05-���ڵ���,����Ϊ��ָ��(���ݷ�����ͨ��Э��,Ҳ���ܷ���0x06)
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD Zero(BYTE *const pbResult = NULL) = 0;

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
	virtual DWORD SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)  = 0;
	
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
	virtual DWORD GasCalibration(BYTE *const pbResult, const int nChanel, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL,  
		short *const psNO2=NULL,
		float *const pfPef=NULL) = 0;

	// д��NO2У׼��Ũ��
	// (in)usNO2,NO2У׼ֵ,��λ:ppm
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2)  = 0;

	// У׼NO2
	// (out)psNO2,NO2ֵָ��,��λ:ppm,�������ִ�й��̴���,����Ϊ0,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD NO2Calibration(BYTE *const pbResult, short *const psNO2=NULL) = 0;

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
	virtual DWORD SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO) = 0;

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
	virtual DWORD GasCheck(BYTE *const pbResult, 
		short *const psHC=NULL, 
		float *const pfCO=NULL, 
		float *const pfCO2=NULL, 
		float *const pfO2=NULL, 
		short *const psNO=NULL, 
		float *const pfPef=NULL) = 0;

	// 13ȡPEF
	// (out)pfPef,Pefֵָ�룬��λ���ޣ�����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetPef(float *const pfPef) = 0;

	// 14���NO�ϻ���־
	// 503����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD ClearNoSign(BYTE *const pbResult = NULL) = 0;

	// 15���õ�������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetZeroAirTypeToEnvAir(BYTE *const pbResult = NULL) = 0;

	// 16���õ�������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetZeroAirTypeToZeroAir(BYTE *const pbResult = NULL) = 0;

	// 17���ó��Ϊ4
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL) = 0;

	// 18���ó��Ϊ2
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL) = 0;

	// 19���õ��ʽΪ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) = 0;

	// 20���õ��ʽΪ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL) = 0;

	// 21�����Զ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD EnableAutoZero(BYTE *const pbResult = NULL) = 0;

	// 22��ֹ�Զ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DisableAutoZero(BYTE *const pbResult = NULL) = 0;

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
	virtual DWORD ETCalibration(BYTE *const pbResult, const float fCalET) = 0;

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
	virtual DWORD OTCalibration(BYTE *const pbResult, const float fCalOT) = 0;

	// 30����ʪ��У׼
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,����Ϊ��ָ��
	// (in)fCalRH:���ʪ��У׼ֵ,��λ-%
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD RHCalibration(BYTE *const pbResult, const float fCalRH) = 0;

	// 31����ѹ��У׼
	// ���غ���ִ�н����
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,����Ϊ��ָ��
	// (in)fCalAP:����ѹ��У׼ֵ,��λ:kPa
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD APCalibration(BYTE *const pbResult, const float fCalAP) = 0;

	// 32�������
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD OpenCheckGas(BYTE *const pbResult = NULL) = 0;

	// 33�������
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD CloseCheckGas(BYTE *const pbResult = NULL) = 0;

	// 34ȡ����״̬
	// (out)pbStatus,����״ָ̬��,0x00-����(δ����),0x01-���ڼ��(�ѿ���),0x02-����Ԥ��,0x03-���ڼ�©,�ȵ�(����״̬�뺬����Ҫ����ͨѶЭ��),����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetStatus(BYTE *const pbStatus) = 0;

	// 35ȡԤ��ʣ��ʱ��
	// (out)pusSecondsLeft:Ԥ��ʣ��ʱ��ָ��,Ӧ��[AA][BB],ʣ��ʱ��=[AA]*256+[BB]��;����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetWarmUpTimeLeft(USHORT *const pusSecondsLeft) = 0;

	// 36����ȼ������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToGas(BYTE *const pbResult = NULL) = 0;

	// 37����ȼ������ΪҺ��ʯ����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToLPG(BYTE *const pbResult = NULL) = 0;

	// 38����ȼ������Ϊ��Ȼ��
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToNG(BYTE *const pbResult = NULL) = 0;

	// 39����ȼ������Ϊ�Ҵ�����
	// (out)pbResult,����ִ�н��ָ��,һ�㷵��0x06,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetFuelTypeToGasohol(BYTE *const pbResult = NULL) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������end

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����ģʽ���start
public:
	// ����ģʽ
	enum OperationMode
	{
		// ������ͨѶģʽ
		ToAnalyzer,
		// ������ͨѶģʽ
		ToFlowmeter
	};

public:
	//// ��ȡ��ǰʹ�õĲ���ģʽ
	//virtual DWORD GetOperationModeUsedNow(void) const = 0;

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
	virtual DWORD SetOperationMode(const enum OperationMode op, BYTE *const pbResult=NULL) = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����ģʽ���stop

public:
	// ����У׼ģʽ
	enum FluxCalibrationMode
	{
		// ������У׼
		LowFlowCal,
		// ������У׼
		HighFlowCal,
		// ��������У׼
		SinglePointFlowCal
	};

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������������ʱ��Ҫ�õ��Ĳ���start

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
	virtual DWORD GetProcessStatus_Flowmeter(BYTE *const pbResult=NULL, WORD *const pwProcessStatus=NULL, WORD *const pwProcess=NULL) = 0;
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
	virtual DWORD StartCollectData_Flowmeter(const WORD wCollectionTime=195, WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL) = 0; 

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
	virtual DWORD StopCollectData_Flowmeter(WORD *const pwMiniStatus=NULL, BYTE *const pbResult=NULL) = 0;  

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
	virtual DWORD GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL) = 0;

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
	virtual DWORD GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
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
		BYTE *const pbResult=NULL) = 0;

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
	virtual DWORD GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus=NULL,
		BYTE *const pbResult=NULL) = 0;

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
	virtual DWORD GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult=NULL) = 0;
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
	virtual DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL) = 0;

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
	virtual DWORD GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL) = 0;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����У׼���begin
	// TODO:��Ҫ��������
	// ���ñ�׼����
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux) = 0;

	// TODO:��Ҫ��������
	// ����У׼ 
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm) = 0;
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
	virtual DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres) = 0;

	// TODO:��Ҫ��������
	// ѹ��У׼ 
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ѹ��У׼���end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�¶�У׼���begin
	// TODO:��Ҫ��������
	// ���ñ�׼�¶�
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp) = 0;

	// TODO:��Ҫ��������
	// �¶�У׼ 
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�¶�У׼���end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ϡ����У׼���begin
	// TODO:��Ҫ��������
	// ���ñ�׼����
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2) = 0;

	// ����У׼
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;

	virtual DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2) = 0;
	virtual DWORD DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;
	virtual DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2) = 0;
	virtual DWORD DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;
	virtual DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2) = 0;
	virtual DWORD DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ϡ����У׼���end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����ʱУ׼���begin
	// ����HC/CO/CO2ͨ����ʱֵ
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime) = 0;
	virtual DWORD SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime) = 0;
	virtual DWORD HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;

	// ����O2ͨ����ʱֵ
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime) = 0;
	virtual DWORD SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime) = 0;
	virtual DWORD O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;

	// ����NOxͨ����ʱֵ
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime) = 0;
	virtual DWORD SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime) = 0;
	virtual DWORD NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult) = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����ʱУ׼���end

	// TODO:��Ҫ��������
	// ��ʼ�ɼ�������
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD SatrtGetEnvironmentO2_Flowmeter(void) = 0;
	virtual DWORD GetEnvironmentO2_Flowmeter(BYTE *const pbResult,float *const pfEnvO2) = 0;
  
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
	virtual DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL) = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������������ʱ��Ҫ�õ��Ĳ���end

	// ��ȡ�ֶ���������
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	virtual DWORD GetTestResultData(short *const psHCHigh=NULL, 
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
		float *const pfLambdaLow=NULL) = 0;

protected:
	// ʵ������ת0���׼����
	float ActFlux2StdFlux0(const float fActFlux, const float fTemp, const float fPres);
	// ʵ������ת20���׼����
	float ActFlux2StdFlux20(const float fActFlux, const float fTemp, const float fPres);
	// 20���׼����תʵ������
	float StdFlux202ActFlux(const float fStdFlux20, const float fTemp, const float fPres);
	// 20���׼����ת0���׼����
	float StdFlux202StdFlux0(const float fStdFlux20, const float fTemp, const float fPres);
	
	// д��������
	// pbWriteBuffer��Ҫд����������ݣ�����Ϊ��ָ��
	// dwNumberOfBytesToWrite��Ҫд������ݳ���
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);

	// ����������
	// pbReadBuffer��Ҫ��ȡ���������ݣ�����Ϊ��ָ��
	// dwNumberOfBytesToRead��Ҫ��ȡ�����ݳ���
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);

protected:
	// ���ھ��
	HANDLE m_hComm;
	// �ٽ籣����
	CRITICAL_SECTION cInfoSection;

	// ��¼����ģʽ
	enum OperationMode m_enMode;
};
#endif