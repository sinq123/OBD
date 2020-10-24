/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaSocketPara.h
* �ļ���ʶ��
* ժҪ������������ʵ����(ͨ��Socket��ȡ)
*
* �汾��1.0
* ���ߣ�Raylee
* ���ڣ�2018-10-24
*
* ��ʷ��Ϣ��
*

* �汾��1.0.0
* ���ڣ�2018-10-24
* ���ߣ�Raylee
* ��������ʽ����ʹ��
*/

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef NANHUA_SOCKET_PARA_H
#define NANHUA_SOCKET_PARA_H

#include "Para.h"

struct EPPacket
{
	WORD wHeader;				// ��ͷ2�ֽ� EP	
	DWORD dwPacketLen;			// �����ȣ�4�ֽ�
	std::wstring strXML;		// UTF8��ʽ��xml�ַ���
	BYTE bTail;					// ��β1�ֽ� $
	
	EPPacket();

	EPPacket(LPCTSTR szXML);

	std::string ToString();

	bool Prase(std::string strRecv);
};

class CNanhuaSocketPara : public CPara
{
public:
	CNanhuaSocketPara(void);
	~CNanhuaSocketPara(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// ���ش򿪽����
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort) { return PARA_OPEN_FAIL;}
	DWORD Open(std::wstring strIP, const int nPort);

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

private:
	// ͨ��socket��ȡ��������
	bool GetEP(float &fET, float &fRH, float &fAP);
	
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(EPPacket sSend, EPPacket &sRecv);
	
	// ��������
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// ��������
	bool SendAll(SOCKET s, char *sendbuf, int len);
	// ����XML����
	bool Xml2Map(LPCTSTR szXML, 
		std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, 
		std::map<std::wstring, std::wstring> &mapData);

public:
	// �¶�б��У׼
	DWORD EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ʪ��б��У׼
	DWORD RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ����ѹб��У׼
	DWORD AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// �������б��У׼
	DWORD OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// �����ȶ�б��У׼
	DWORD OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// �¶���λУ׼
	DWORD EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ʪ����λУ׼
	DWORD RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ����ѹ��λУ׼
	DWORD AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ���������λУ׼
	DWORD OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// �����ȶ���λУ׼
	DWORD OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ����У׼����
	DWORD BackupCalibrationParameters(BYTE *const pbResult) { return PARA_WR_EQUIP_RETURE_0X15;}

	// �ָ�У׼����
	DWORD RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ���ó��Ϊ4
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ���ó��Ϊ2
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ���õ��ʽΪ����
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ���õ��ʽΪ����
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// ��ȫ������
	DWORD GetEntireData(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL);

	// ��ȫ������
	DWORD GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm,
		WORD *const pwStatus);

	// ����������
	DWORD GetEnvironmentParameters(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL);

	// ��ת������
	DWORD GetOilTempRpm(float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL){ return PARA_WR_EQUIP_RETURE_0X15;}
	
	// ���ݴ���
	DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

	// ��ȡIMEI��ICCID
	DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID);

protected:
	// �ͻ��������߳�
	SOCKET m_socket;

	// �ٽ籣����
	CRITICAL_SECTION m_sCriticalSection;

protected:
		// ��־�ļ�·����������
	CStringW m_strLogFilePath;
	// ��ģ����
	CStringW m_strModName;
	// ������־�ļ�
	void GenLogFile(void);
};
#endif