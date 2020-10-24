/*
* Copyright (c) 2018,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaEPCS.h
* �ļ���ʶ��
* ժҪ�����������Ƿ���ʵ����
*
* ��ǰ�汾��1.0
* ���ߣ�Hyh
* ������ڣ�2018-03-10
*/
#pragma once

#ifndef NANHUAEPCS4G_H
#define NANHUAEPCS4G_H

#include "Para.h"

class CNanhuaEPCS4G : public CPara
{
public:
	CNanhuaEPCS4G(void);
	~CNanhuaEPCS4G(void);

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
	DWORD DataTrans(BYTE *const pbResult,const BYTE bCom[], const BYTE * pbDataBuf, const UINT nDatalen);

	// ���봫������ģʽ
	DWORD EnterDTMode(BYTE *const pbResult);

	// ��������
	DWORD TransData(BYTE *const pbResult, const BYTE * pbDataBuf, const UINT nDatalen);

	// �˳���������ģʽ
	DWORD ExitDTMode(BYTE *const pbResult);

	// ��ȡIMEI��ICCID
	DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID);

	// ��������
	DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

protected:
	// ���ھ��
	HANDLE m_hComm;
	// �ٽ籣����
	CRITICAL_SECTION m_sCriticalSection;
};
#endif