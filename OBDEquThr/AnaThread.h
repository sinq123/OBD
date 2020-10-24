/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�AnaThread.h
* �ļ���ʶ��
* ժҪ���������߳�
*
* ��ǰ�汾��1.0.5
* ���ߣ�Hyh
* ������ڣ�2010-06-21
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.23
* ������ڣ�2014-07-18
* ���ߣ�Lihy
* ��������ӻ�ȡ����������ʵʱ����ָ��
*
* ��ǰ�汾��1.0.22
* ������ڣ�2014-06-26
* ���ߣ�Lihy
* ��������ӻ�ȡ΢״ָ̬������ж�VMAS�������з������Ƿ����(΢״̬�и�λ�еĵ�4��2����λ�����жϣ�1Ϊ������0Ϊ����)
*
* ��ǰ�汾��1.0.21
* ������ڣ�2014-05-10
* ���ߣ�Lihy
* ��������ӹ��������Ƹ��е�����У׼����
*
* �汾��1.0.20
* ���ڣ�2013-1-18
* ���ߣ�Wyq
* ���������Ӷ�ȡ�����¶�ʵʱ��ֵ
*
* �汾��1.0.19
* ���ڣ�2012-10-12
* ���ߣ�Raylee
* ����������ϻ�NHA-501A�豸֧��
*
* �汾��1.0.18
* ���ڣ�2012-10-11
* ���ߣ�Raylee
* ��������ӷ��FGA4100������FLA501����ȪMQW511�豸֧��
*
* ��ǰ�汾��1.0.7
* ������ڣ�2011-03-03
* ���ߣ�Ming
* �������ֱ��������HCCOCO2,O2,NOxͨ����ʱֵ�ĺ���,GetxxxDelayTime_Flowmeter(),SetxxxDelayTime_Flowmeter()��xxxDelayTime_Flowmeter()
*
* ��ǰ�汾��1.0.6
* ������ڣ�2010-08-26
* ���ߣ�Raylee
* ���������GetTestResultData()��������ȡ���������
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
* ������ڣ�2010-02-01
* ���ߣ�Cui,hyh
* ����������Open()�л�ͨѶģʽ�޸�,ԭ�л�����Ϊ1,���л�����Ϊ50
*
* ��ǰ�汾��1.0.2
* ������ڣ�2009-12-31
* ���ߣ�Cui
* ����������Open������β�bool bConnectToFlowmeter(��Ƿ������Ƿ�������������)
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

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaNha503V.h"
#include "NanhuaNha503.h"
#include "NanhuaNha501A.h"
#include "NanhuaNha506I.h"
#include "NanhuaNha509Encrypt.h"
#include "NanhuaNha610Encrypt.h"
#include "NanhuaNha100.h"
#include "MingQuanML100.h"
#include "MingQuanMQW511.h"
#include "MingQuanMQW50A.h"
#include "FuLiFLA501.h"
#include "FuLiFLA502.h"
#include "FuLiFLV1000.h"
#include "FoFenFGA4100.h"
// CAnaThread

class AFX_EXT_CLASS CAnaThread : public CWinThread
{
	DECLARE_DYNCREATE(CAnaThread)

protected:
	CAnaThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAnaThread();

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
		// ������503
		IM_NANHUA_NHA503,
		// ����Vmas���ӵ�503
		IM_NANHUA_NHA503V,
		// ����Э���501A
		IM_NANHUA_NHA501A,
		// �ڲ�ͨѶЭ���506,605
		IM_NANHUA_NHA506I,
		// ���FGA-4100
		IM_FOFEN_FGA4100,
		// ����FLA-501
		IM_FULI_FLA501,
		// ����FLA-502
		IM_FULI_FLA502,
		// ����FLV1000
		IM_FULI_FLV1000,
		// ��ȪMQW-511
		IM_MINGQUAN_MQW511,
		// ��ȪMQW-50A
		IM_MINGQUAN_MQW50A,
		// ��ȪML-100
		IM_MINGQUAN_ML100,
		// NHA509
		IM_NANHUA_NHA509,
		// NHA509����
		IM_NANHUA_NHA509EN,
		// NHA610����
		IM_NANHUA_NHA610EN,
		// NHA100
		IM_NANHUA_NHA100,
	};

public:
	// �����ھ��
	HANDLE GetComHandle(void);
	void AttachComHandle(HANDLE hCom, const wchar_t *szProtocol);
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
	// (in)bPort�����ں�(1~255)
	// (in)im����������
	// (in)bConnectToFlowmeter���Ƿ�������������
	// ���ش򿪽����
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort, const wchar_t *szProtocol, const bool bConnectToFlowmeter=false);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// ANA_CLOSE_SUCCEED
	// ANA_CLOSE_FAIL_WIN32API
	// ANA_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����ǻ��
	inline bool IsOpen(void) const
	{
		return m_pAna->IsOpen();
	}
	// ��ȡ���ڲ���״̬
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���end

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����ǲ��ֻ�������begin
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ͬ������begin
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

	DWORD GetMajorData(short *const psHC, 
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
		bool *const pbWaterFull);
	
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
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflateEnvAir(BYTE *const pbResult = NULL);

	// 4��ܵ���(����,������̽ͷ����)
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD DeflatePipeAir(BYTE *const pbResult = NULL);

	// 5��ϴ�ܵ�(����)
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD InverseBlow(BYTE *const pbResult = NULL);

	// 6ֹͣ��ϴ(ֹͣ������ͣ��)
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD StopPump(BYTE *const pbResult = NULL);

	// 7��©
	// (out)pbResult,����ִ�н��ָ��,0x00-��й¶,0x01-��й¶,0x05-���ڼ�©,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CheckLeak(BYTE *const pbResult = NULL);

	// 8����
	// (out)pbResult,����ִ�н��ָ��,0x00-����ɹ�,0x01-����ʧ��,0x05-���ڵ���,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD Zero(BYTE *const pbResult = NULL);

	// 9д��У׼��Ũ��
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��(У��ʹ���),�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,0x05-����У׼,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��(У��ʹ���),�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,0x05-���ڼ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD ClearNoSign(BYTE *const pbResult = NULL);

	// 15���õ�������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToEnvAir(BYTE *const pbResult = NULL);

	// 16���õ�������Ϊ����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetZeroAirTypeToZeroAir(BYTE *const pbResult = NULL);

	// 17���ó��Ϊ4
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL);

	// 18���ó��Ϊ2
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL)  ;

	// 19���õ��ʽΪ����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) ;

	// 20���õ��ʽΪ����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL);

	// 21�����Զ�����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD EnableAutoZero(BYTE *const pbResult = NULL);

	// 22��ֹ�Զ�����
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// (in)fCalOT:����У׼ֵ,��λ:���϶�
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OTCalibration(BYTE *const pbResult, const float fCalOT);

	// 30����ʪ��У׼
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
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
	// (out)pbResult:����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// (in)fCalAP:����ѹ��У׼ֵ,��λ:kPa
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD APCalibration(BYTE *const pbResult, const float fCalAP);

	// 32�������
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD OpenCheckGas(BYTE *const pbResult = NULL);

	// 33�������
	// (out)pbResult,����ִ�н��ָ��,0x00-�ɹ�,0x01-ʧ��,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD CloseCheckGas(BYTE *const pbResult = NULL);

	// 34ȡ����״̬
	// (out)pbStatus,����״ָ̬��,0x00-����(δ����),0x01-���ڼ��(�ѿ���),0x02-����Ԥ��,0x03-���ڼ�©,�������ִ�й��̴���,����Ϊ0xFF,����Ϊ��ָ��
	// ���غ���ִ�н����
	// ANA_WR_SUCCEED
	// ANA_WR_PORT_NOT_OPEN
	// ANA_WR_READ_SPECIFIED_DATA_FAIL
	// ANA_WR_EQUIP_RETURE_0X15
	// ANA_WR_CHECK_SUM_FAIL
	DWORD GetStatus(BYTE *const pbStatus);

	// 35ȡԤ��ʣ��ʱ��
	// (out)pusSecondsLeft:Ԥ��ʣ��ʱ��ָ��,Ӧ��[AA][BB],ʣ��ʱ��=[AA]*256+[BB]��;�������ִ�й��̴���,����Ϊ0xFFFF;����Ϊ��ָ��
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ͬ������end


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �첽����begin
	// �첽��ȡ������������
	void GetMajorData_Asyn(HWND hCallBackWnd=NULL);
	// �첽��ȡ������������(��)
	void GetMajorData_Asyn_N(HWND hCallBackWnd=NULL);
	// �첽��ȡNOx����
	void GetNOxData_Asyn(HWND hCallBackWnd=NULL);
	// �첽��ȡ�����Ǹ�������
	void GetMinorData_Asyn(HWND hCallBackWnd=NULL);
	//  �첽��ȡPef
	void GetPef_Asyn(HWND hCallBackWnd=NULL);
	// �첽��ȡ����״̬
	void GetStatus_Aysn(HWND hCallBackWnd=NULL);
	// �첽������У׼
	void GasCalibration_Aysn(const int nChanel, HWND hCallBackWnd=NULL);
	// �첽������
	void GasCheck_Aysn(HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �첽����end
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����ǲ��ֻ�������begin

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����Ʋ��ֻ�������begin
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ͬ������begin
	// ��ȡ��ǰʹ�õĲ���ģʽ
	inline DWORD GetOperationModeUsedNow(void) const
	{
		return m_opUsedNow;
	}

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
	DWORD SetOperationMode(const enum CAna::OperationMode op, BYTE *const pbResult=NULL);

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

	// ���ñ�׼���� ע:fCalFlux���׼����20��״̬�� Mark By LiangQW@20160721
	DWORD SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum CAna::FluxCalibrationMode cm, const float fCalFlux);
	// ����У׼ //����У׼���pbCalResult���ɹ���������0,������2,����1 Mark By LiangQW@20160722
	DWORD FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum CAna::FluxCalibrationMode cm);

	// ���ñ�׼ѹ��
	DWORD SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres);
	// ѹ��У׼ 
	DWORD PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	//��ʱȡ����ԭ�򣺽��������Ʋ�֧�� Mark By LiangQW@20160721
	// �����¶�
	DWORD SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp);
	// �¶�У׼ 
	DWORD TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// ���ñ�׼����
	DWORD SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2);
	// ����У׼ 
	DWORD DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// ���øߵ��׼����(���ù���������)
	DWORD SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2);
	// �ߵ�����У׼ 
	DWORD DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// �����е��׼����(���ù���������)
	DWORD SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2);
	// �е�����У׼ 
	DWORD DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// ���õ͵��׼����(���ù���������)
	DWORD SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2);
	// �͵�����У׼ 
	DWORD DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode=false);

	// ��ʼ�ɼ�������
	DWORD SatrtGetEnvironmentO2_Flowmeter(void);
	// bResult 0x00ʱ��ȡ�������ɹ�,����״̬����
	DWORD GetEnvironmentO2_Flowmeter(BYTE *const pbResult,float *const pfEnvO2);

	// ����HC/CO/CO2ͨ����ʱֵ
	DWORD GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// ����O2ͨ����ʱֵ
	DWORD GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

	// ����NOxͨ����ʱֵ
	DWORD GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime);
	DWORD SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime);
	DWORD NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult);

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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ͬ������end

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �첽����begin
	//ȡ������ʵʱ����
	void GetRealTimeData_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// ȡ��������,����ģʽ(�첽ģʽֻ����ȡһ������)
	void GetProcessDataStatus_WT_Flowmeter_Aysn(const WORD wCntStart, HWND hCallBackWnd=NULL);
	// ȡ��������,Ũ��ģʽ(�첽ģʽֻ����ȡһ������)
	void GetProcessDataStatus_CT_Flowmeter_Aysn(const WORD wCntStart, HWND hCallBackWnd=NULL);
	// ȡ������
	void GetTotalWeight_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// ��ʼ�ɼ�����
	void StartCollectData_Flowmeter_Aysn(const WORD wCollectionTime=195,HWND hCallBackWnd=NULL);
	// ֹͣ�ɼ�
	void StopCollectData_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// ��ʼ�ɼ�������
	void SatrtGetEnvironmentO2_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	// ȡ������
	void GetEnvironmentO2_Flowmeter_Aysn(HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �첽����end
	
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

	DWORD SetAnalyzerOperationMode(void);
	DWORD SetFlowmeterOperationMode(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����Ʋ��ֻ�������end

private:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��������Ӧ�첽��������begin
	afx_msg void OnGetMajorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMajorDataN(WPARAM wParam, LPARAM lParam); // �µ�ȡ����Э�飨����ˮλ�жϣ�
	afx_msg void OnGetNOxData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMinorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetPef(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGasCalibration(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGasCheck(WPARAM wParam, LPARAM lParam);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��������Ӧ�첽��������end


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��������Ӧ�첽��������begin
	afx_msg void OnGetRealTimeData_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetProcessDataStatus_WT_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetProcessDataStatus_CT_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetTotalWeight_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStartCollectData_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopCollectData_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSatrtGetEnvironmentO2_Flowmeter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetEnvironmentO2_Flowmeter(WPARAM wParam, LPARAM lParam);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��������Ӧ�첽��������end


public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�����ǲ��ַ�������begin
	// ȡ������������,HCֵ,��λ:ppm
	inline short GetMajorDataHCValue(void) const
	{
		return m_sHC;
	}
	// ȡ������������,COֵ,��λ:%
	inline float GetMajorDataCOValue(void) const
	{
		return m_fCO;
	}
	// ȡ������������,CO2ֵ,��λ:%
	inline float GetMajorDataCO2Value(void) const
	{
		return m_fCO2;
	}
	// ȡ������������,O2ֵ,��λ:%
	inline float GetMajorDataO2Value(void) const
	{
		return m_fO2;
	}
	// ȡ������������,NOֵ,��λ:ppm
	inline short GetMajorDataNOValue(void) const
	{
		return m_sNO;
	}
	// ȡ������������,NO2ֵ,��λ:ppm
	inline short GetMajorDataNO2Value(void) const
	{
		return m_sNO2;
	}
	// ȡ������������,NOxֵ,��λ:ppm
	inline short GetMajorDataNOxValue(void) const
	{
		return m_sNOx;
	}
	// ȡ������������,ת��ֵ,��λ:r/min
	inline USHORT GetMajorDataRpmValue(void) const
	{
		return m_usRpm;
	}
	// ȡ������������,����ֵ,��λ:���϶�
	inline float GetMajorDataOilTempValue(void) const
	{
		return m_fOilTemp;
	}
	// ȡ������������,��������ֵ,��λ:��
	inline USHORT GetMajorDataFluxValue(void) const
	{
		return m_usFlux;
	}
	// ȡ������������,��������ϵ��,��λ:��
	inline float GetMajorDataLambdaValue(void) const
	{
		return m_fLambda;
	}
	// ȡPefֵ,��λ:��
	inline float GetPefValue(void) const
	{
		return m_fPef;
	}
	// ȡ״ֵ̬
	inline USHORT GetStatusValue(void) const
	{
		return m_bStatus;
	}
	// ȡ�����Ǹ�����,�����¶�ֵ,��λ:���϶�
	inline float GetMinorDataETValue(void) const
	{
		return m_fET;
	}
	// ȡ�����Ǹ�����,����ѹ��ֵ,��λ:kPa
	inline float GetMinorDataAPValue(void) const
	{
		return m_fAP;
	}
	// ȡ�����Ǹ�����,���ʪ��ֵ,��λ:%
	inline float GetMinorDataRHValue(void) const
	{
		return m_fRH;
	}
	inline bool GetMajorDataWaterFull(void) const
	{
		return m_bWaterFull;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�����ǲ��ַ�������end

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�����Ʋ��ַ�������begin
	// ȡ������ʵʱ����,����ֵ,��λ:scfm(��׼����Ӣ��/����)
	inline float GetVmasRTDFluxStd0Value(void) const
	{
		return m_fFluxStd0;
	}
	inline float GetVmasRTDFluxStd20Value(void) const
	{
		return m_fFluxStd20;
	}
	inline float GetVmasRTDFluxActValue(void) const
	{
		return m_fFluxAct;
	}
	// ȡ������ʵʱ����,ѹ��ֵ,��λ:kPa
	inline float GetVmasRTDPresValue(void) const
	{
		return m_fPres;
	}
	// ȡ������ʵʱ����,δ����ϡ����ֵ,��λ:%
	inline float GetVmasRTDUDilO2Value(void) const
	{
		return m_fUDilO2;
	}
	// ȡ������ʵʱ����,ϡ�������¶�ֵ,��λ:���϶�
	inline float GetVmasRTDGasTempValue(void) const
	{
		return m_fGasTemp;
	}
	// ȡ��������,HC����ֵ,��λ:mg
	inline float GetVmasPDSHCwtValue(void) const
	{
		return m_fHCwt;
	}
	// ȡ��������,CO����ֵ,��λ:mg
	inline float GetVmasPDSCOwtValue(void) const
	{
		return m_fCOwt;
	}
	// ȡ��������,CO2����ֵ,��λ:mg
	inline float GetVmasPDSCO2wtValue(void) const
	{
		return m_fCO2wt;
	}
	// ȡ��������,NO����ֵ,��λ:mg
	inline float GetVmasPDSNOwtValue(void) const
	{
		return m_fNOwt;
	}
	// ȡ��������,HCŨ��ֵ,��λ:ppm
	inline short GetVmasPDSHCctValue(void) const
	{
		return m_sHCct;
	}
	// ȡ��������,COŨ��ֵ,��λ:%
	inline float GetVmasPDSCOctValue(void) const
	{
		return m_fCOct;
	}
	// ȡ��������,CO2Ũ��ֵ,��λ:%
	inline float GetVmasPDSCO2ctValue(void) const
	{
		return m_fCO2ct;
	}
	// ȡ��������,NOŨ��ֵ,��λ:ppm
	inline short GetVmasPDSNOctValue(void) const
	{
		return m_sNOct;
	}
	// ȡ��������,ϡ��O2Ũ��ֵ,��λ:%
	inline float GetVmasPDSDilO2Value(void) const
	{
		return m_fDilO2;
	}
	// ȡ��������,������O2Ũ��ֵ,��λ:%
	inline float GetVmasPDSBenchO2Value(void) const
	{
		return m_fBenchO2;
	}
	// ȡ��������,ϡ�������¶�ֵ,��λ:���϶�
	inline short GetVmasPDSGasTempValue(void) const
	{
		return m_sGasTemp;
	}
	// ȡ��������,ϡ�������׼����ֵ,��λ:scfm
	inline float GetVmasPDSGasFluxStd0Value(void) const
	{
		return m_fGasFluxStd0;
	}
	inline float GetVmasPDSGasFluxStd20Value(void) const
	{
		return m_fGasFluxStd20;
	}
	// ȡ��������,������׼����ֵ,��λ:scfm
	inline float GetVmasPDSExhaustFluxStd0Value(void) const
	{
		return m_fExhaustFluxStd0;
	}
	inline float GetVmasPDSExhaustFluxStd20Value(void) const
	{
		return m_fExhaustFluxStd20;
	}
	// ȡ��������,ϡ������ѹ��ֵ,��λ:kPa
	inline float GetVmasPDSGasPresValue(void) const
	{
		return m_fGasPres;
	}
	// ȡ��������,ϡ������ʵ������ֵ,��λ:ascm
	inline float GetVmasPDSGasFluxActValue(void) const
	{
		return m_fGasFluxAct;
	}
	// ȡ��������,������ת��ֵ,��λ:rpm
	inline USHORT GetVmasPDSRpmValue(void) const
	{
		return m_usVmasRpm;
	}
	// ȡHC�ܿ���,��λ:mg
	inline USHORT GetVmasTotalHCwtValue(void) const
	{
		return m_usTotalHCwt;
	}
	// ȡCO�ܿ���,��λ:mg
	inline USHORT GetVmasTotalCOwtValue(void) const
	{
		return m_usTotalCOwt;
	}
	// ȡCO2�ܿ���,��λ:mg
	inline USHORT GetVmasTotalCO2wtValue(void) const
	{
		return m_usTotalCO2wt;
	}
	// ȡNO�ܿ���,��λ:mg
	inline USHORT GetVmasTotalNOwtValue(void) const
	{
		return m_usTotalNOwt;
	}
	// ȡ���������,��λ:L
	inline float GetVmasTotalExhaustVolume(void) const
	{
		return m_fTotalExhaustVolume;
	}
	// ȡϡ�����������,��λ:L
	inline float GetVmasTotalGasVolume(void) const
	{
		return m_fTotalGasVolume;
	}
	// ȡ������,��λ:mg
	inline float GetVmasEnvironmentO2Value(void) const
	{
		return m_fEnvO2;
	}

	//ȡ΢״̬
	inline WORD GetMiniStatus(void) const
	{
		return m_wMiniStatus;
	}
	// ��ȡ�����Ƕ�д״̬
	inline DWORD GetAnaWRSta(void) const
	{
		return m_dwAnaWRSta;
	}
	// ��ȡ�����ƶ�д״̬
	inline DWORD GetFloWRSta(void) const
	{
		return m_dwFloWRSta;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�����Ʋ��ַ�������begin


public:
	// �˳��̲߳���
	void QuitThread(void);

private:
	// ��ʼ���豸
	void InitializeInstrument(const wchar_t *szProtocol);
	// ж���豸
	void UninitializeInstrument(void);

private:
	// Ana������
	CAna* m_pAna;
	// �豸�ͺ�
	InstrumentModel m_emInstrumentModel;
	// �߳��˳���־
	bool m_bQuitThread;

protected:
	//////////////////////////////////////////////////////////�����ǲ��ֳ�Ա����begin
	// ������
	// HCŨ��ֵ,��λ:ppm
	short  m_sHC;
	// COŨ��ֵ,��λ:%
	float  m_fCO;
	// CO2Ũ��ֵ,��λ:%
	float  m_fCO2;
	// O2Ũ��ֵ,��λ:%
	float  m_fO2;
	// NOŨ��ֵ,��λ:%
	short  m_sNO;
	// NO2Ũ��ֵ,��λ:%
	short  m_sNO2;
	// NOxŨ��ֵ,��λ:%
	short  m_sNOx;
	// ת��,��λ:r/min
	USHORT  m_usRpm;
	// ����,��λ:���϶�
	float  m_fOilTemp;
	// ��������ֵ(��ʵ��ѹ��ֵ,ѹ��ֵԽ��,����ԽС),��λ:��
	USHORT  m_usFlux;
	// ��������ϵ��,��λ:��
	float  m_fLambda;
	// ���������Ƿ��ϻ�
	bool m_bO2Sensor;
	// �����������Ƿ��ϻ�
	bool m_bNOSensor;
	// ˮλ�Ƿ�����
	bool m_bWaterFull;
	// ��������
	// �����¶�(Environment Temperature),��λ:���϶�
	float m_fET;
	// ����ѹ��(Atmospheric Pressure),��λ:kPa
	float m_fAP;
	// ���ʪ��(Relative Humidity),��λ:%
	float m_fRH;

	// pef,��λ:��
	float m_fPef;

	// ״̬
	// 0x00-����(δ����)
	// 0x01-���ڼ��(�ѿ���)
	// 0x02-����Ԥ��
	// 0x03-���ڼ�©
	BYTE m_bStatus;
	//////////////////////////////////////////////////////////�����ǲ��ֳ�Ա����end

	//////////////////////////////////////////////////////////�����Ʋ��ֳ�Ա����begin
	// ������ʵʱ����
	// ����ֵ,��λ:scfm(��׼����Ӣ��/����)
	float  m_fFluxStd0;
	float  m_fFluxStd20;
	float  m_fFluxAct;
	// ѹ��ֵ,��λ:kPa
	float  m_fPres;
	// δ����ϡ����,��λ:%
	float  m_fUDilO2;
	// ϡ�������¶�,��λ:���϶�
	float m_fGasTemp; 

	// ������������
	// HC����ֵ,��λ:mg
	float m_fHCwt; 
	// CO����ֵ,��λ:mg
	float m_fCOwt; 
	// CO2����ֵ,��λ:mg
	float m_fCO2wt; 
	// NO����ֵ,��λ:mg
	float m_fNOwt;

	// Ũ�ȹ�������
	// HCŨ��ֵ,��λ:ppm
	short m_sHCct;
	// COŨ��ֵ,��λ:%
	float m_fCOct; 
	// CO2Ũ��ֵ,��λ:%
	float m_fCO2ct; 
	// NOŨ��ֵ,��λ:ppm
	short m_sNOct; 
	// ϡ����Ũ��ֵ,��λ:%
	float m_fDilO2; 
	// ��������Ũ��ֵ,��λ:%
	float m_fBenchO2; 
	// ϡ�������¶�,��λ:���϶�
	short m_sGasTemp; 
	// ϡ�������׼����ֵ,��λ:scfm(��׼����Ӣ��/����)
	float m_fGasFluxStd0; 
	float m_fGasFluxStd20; 
	// ������׼����ֵ,��λ:scfm(��׼����Ӣ��/����)
	float m_fExhaustFluxStd0;
	float m_fExhaustFluxStd20;
	// ϡ������ѹ��ֵ,��λ:kPa
	float  m_fGasPres; 
	// ϡ������ʵ������ֵ,��λ:acfm(��׼����Ӣ��/����)
	float  m_fGasFluxAct;
	// ��������ȡ������ת��ֵ,�������Է�����,��λ:r/min
	USHORT  m_usVmasRpm;

	// �ܿ���
	// HC������,��λ:mg
	USHORT m_usTotalHCwt;
	// CO������,��λ:mg
	USHORT m_usTotalCOwt;
	// CO2������,��λ:mg
	USHORT m_usTotalCO2wt;
	// NO������,��λ:mg
	USHORT m_usTotalNOwt;
	// ���������,��λ:L
	float m_fTotalExhaustVolume;
	// ϡ�����������,��λ:L
	float m_fTotalGasVolume;

	// ������
	float m_fEnvO2;

	// ΢״̬,�����ռ������������:51,52
	WORD m_wMiniStatus;
	DWORD m_dwAnaWRSta; // �����Ƕ�д״̬
	DWORD m_dwFloWRSta; // �����ƶ�д״̬
	//////////////////////////////////////////////////////////�����Ʋ��ֳ�Ա����begin

	// Ŀǰ����ͨѶ������
	enum CAna::OperationMode m_opUsedNow;
	// ���ڲ���״̬
	DWORD m_dwStatus;
};


