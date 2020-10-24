/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaNHF1.h
* �ļ���ʶ��
* ժҪ��������NHF1ͨѶ��
*
*
* ��ǰ�汾��
* ���ߣ�
* ������ڣ�
*
*/

#pragma once

#ifndef NANHUA_NHF1_H
#define NANHUA_NHF1_H

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


class CNanhuaNHF1
{
public:
	CNanhuaNHF1(void);
	~CNanhuaNHF1(void);

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
		bool bNeedReadChkSum=true);

	// ����ǰnλ��У���
	// ��ӣ�Ȼ��ȡ����ֽ�λ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	BYTE CheckSum_Flowmeter(const BYTE *const pb, const UINT un);

	//�����У���Ƿ�Ϊ��
	bool CheckReadSum_Flowmeter(const BYTE *const pb, const UINT un);

public:
	
	////////////////////////////////////////////////////////////////////////////////////////////////////������У׼����
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL

	// 1.��ȡ������ʵʱ����
	// pfFluxStd0:0���׼������L/s
	// pfFluxStd20:20���׼������L/s
	// pfFluxAct:ʵ��������L/s
	// (out)pbResult:�����������ָ�룻0x31-�ɹ���0x30-ʧ��
	//               �������ִ�й��̴��󣬷���0x00
	//				 ����ΪNULL
	DWORD GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, 
		float *const pfPres, float *const pfUDilO2, float *const pfGasTemp=NULL, BYTE *const pbResult=NULL);

	// 2.��ȡ״̬
	DWORD Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau);
	
	// 3.���ط��
	// (in)bRun:���ط��������true-�������false-�ط��
	// (out)pbResult:�����������ָ�룻0x06-����ģʽ�ɹ���0x05-����ģʽʧ��(������æ)�� 0x15-����ģʽʧ��(��������)
	//               �������ִ�й��̴��󣬷���0xFF
	//				 ����ΪNULL
	DWORD RunFan_Flowmeter(const bool bRun, BYTE *const pbResult=NULL);

	// 4.ϡ������������
	DWORD SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult);


	// 5.���ñ�׼ѹ��
	// ���غ���ִ�н����
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);

	// 6.�������¶�У׼��ע�⣺���������ƻ�о��֧��
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);

	// 7.������/��/�����̱궨
	// ���غ���ִ�н����
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux);

	// 8.ϡ���������̱궨
	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);

	// 9.ϡ���������̱궨
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);

	// 10.ϡ���������̱궨
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);
	
	// 11.�ָ���������
	DWORD RestoreFactorySettings_Flowmeter(BYTE *const pbResult);

protected:
	// ���ھ��
	HANDLE m_hComm;
	// �ٽ籣����
	CRITICAL_SECTION cInfoSection;
};
#endif