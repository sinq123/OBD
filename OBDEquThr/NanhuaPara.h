/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaPara.h
* �ļ���ʶ��
* ժҪ������������ʵ����
*
* ��ǰ�汾��1.0.2
* ���ߣ�Hyh
* ������ڣ�2010-06-08
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-06-08
* ���ߣ�Hyh
* ������1)��ӵ��ʽ�ͳ���������麯��
*
* �汾��1.0.1
* ���ڣ�2010-05-27
* ���ߣ�Cui
* �������޸�GetEntireData��GetEnvironmentParameters��GetOilTempRpm�����壬ʹ���Խ���NULLָ��
*
* ��ǰ�汾��1.0.0
* ������ڣ�2010-05-11
* ���ߣ�Hyh
* ��������ʽ����ʹ��
*/
#pragma once

#ifndef NANHUAPARA_H
#define NANHUAPARA_H

#include "Para.h"

class CNanhuaPara : public CPara
{
public:
	CNanhuaPara(void);
	~CNanhuaPara(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort);
	DWORD Open(std::wstring strIP, const int nPort) { return PARA_OPEN_FAIL;}

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

protected:
	// CRC Calculation
	void CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW);
	// Write Buffer
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);
	// Read Comm Buffer
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);

protected:
	// ִ�лָ���������
	bool ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2);

public:
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
	DWORD RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType);

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

	// ���ݴ���
	DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

	// ��ȡIMEI��ICCID
	DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID);

protected:
	// ���ھ��
	HANDLE m_hComm;
	// �ٽ籣����
	CRITICAL_SECTION m_sCriticalSection;
};
#endif