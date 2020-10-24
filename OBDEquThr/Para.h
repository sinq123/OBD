/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�Para.h
* �ļ���ʶ��
* ժҪ�����������ǳ�����
*		�Դ����⺯����ʽ������������ǲ����ӿ�
*
* ��ǰ�汾��1.0
* ���ߣ�Hyh
* ������ڣ�2010-05-11
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-06-08
* ���ߣ�Hyh
* ������1)��ӵ��ʽ�ͳ���������麯��
*
* ��ǰ�汾��1.0.0
* ������ڣ�2010-05-11
* ���ߣ�Hyh
* ��������ʽ����ʹ��
*/
#pragma once


#ifndef PARA_H
#define PARA_H

#include <string>

// ����ִ�н��
#define PARA_OPEN_SUCCEED							0x00  // �򿪴��ڳɹ�
#define PARA_OPEN_FAIL								0x01  // �򿪴���ʧ��
#define PARA_CLOSE_SUCCEED							0x00  // �رմ��ڳɹ�
#define PARA_CLOSE_FAIL_WIN32_API					0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define PARA_CLOSE_FAIL_NO_OPEN_PORT				0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���
#define PARA_WR_SUCCEED								0x00  // д���ɹ�
#define PARA_WR_PORT_NOT_OPEN						0x01  // д��ʧ�ܣ�����δ��
#define PARA_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // д��ʧ�ܣ���ָ������ʧ��
#define PARA_WR_READ_SPECIFIED_DATA_FAIL			0x03  // д��ʧ�ܣ���ָ������ʧ��
#define PARA_WR_EQUIP_RETURE_0X15					0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define PARA_WR_CHECK_SUM_FAIL						0x05  // д��ʧ�ܣ���������У���ʧ��
#define PARA_WR_UNKNOWN_ERROR						0xFF  // д��ʧ�ܣ�δ֪����

class CPara
{
public:
	CPara(void);
	virtual ~CPara(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// PARA_OPEN_SUCCEED
	// PARA_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;
	virtual DWORD Open(std::wstring strIP, const int nPort) = 0;

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// PARA_CLOSE_SUCCEED
	// PARA_CLOSE_FAIL_WIN32API
	// PARA_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// �жϴ����ǻ��
	virtual bool IsOpen(void) const = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���end

public:
	enum ParaType
	{
		// ���в���
		AllParameters,
		// �����¶Ȳ���
		EnvironmentTemperaturePara,
		// ���ʪ�Ȳ���
		RelativeHumidityPara,
		// ����ѹ������
		AirPressurePara,
		// ���²���
		OilTemperaturePara
	};

public:
	// �¶�б��У׼
	virtual DWORD EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// ʪ��б��У׼
	virtual DWORD RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// ����ѹб��У׼
	virtual DWORD AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// �������б��У׼
	virtual DWORD OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// �����ȶ�б��У׼
	virtual DWORD OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) = 0;

	// �¶���λУ׼
	virtual DWORD EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// ʪ����λУ׼
	virtual DWORD RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// ����ѹ��λУ׼
	virtual DWORD AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// ���������λУ׼
	virtual DWORD OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// �����ȶ���λУ׼
	virtual DWORD OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero) = 0;

	// ����У׼����
	virtual DWORD BackupCalibrationParameters(BYTE *const pbResult) = 0;

	// �ָ�У׼����
	virtual DWORD RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType) = 0;

	// ���ó��Ϊ4
	virtual DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL) = 0;

	// ���ó��Ϊ2
	virtual DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL) = 0;

	// ���õ��ʽΪ����
	virtual DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) = 0;

	// ���õ��ʽΪ����
	virtual DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL) = 0;

	// ��ȫ������
	virtual DWORD GetEntireData(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL) = 0;

	// ����������
	virtual DWORD GetEnvironmentParameters(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL) = 0;

	// ��ת������
	virtual DWORD GetOilTempRpm(float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL) = 0;

	// ���ݴ���
	virtual DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen) = 0;

		// ��ȡIMEI��ICCID
	virtual DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID) = 0;
};
#endif