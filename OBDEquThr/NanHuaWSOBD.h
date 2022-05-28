/*
* Copyright (c) ��ɽ�л�������������޹�˾
* All rights reserved.
*
* �ļ����ƣ�ZhengDeOBD.h
* �ļ���ʶ��
* ����: ���������Ѱ�ODB���ݽ���
*
* �汾��1.0
* ���ڣ�2018-06-01
* ���ߣ�Raylee
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2018-06-01
* ���ߣ�Raylee
* ���������ڿ���
*/

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef NanHuaWSOBD_H
#define NanHuaWSOBD_H

#include <vector>
#include "OBDAdapter.h"

class CNanHuaWSOBD : public COBDAdapter
{
public:
	CNanHuaWSOBD(void);
	~CNanHuaWSOBD(void);

public:
	////////////////////////////////////////////////////////////////////////////////////���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// ������
	// bPort�����ں�(1~255)
	// nBaudRate�������� (110��300��600��1200��2400��4800��9600��14400��19200��38400��56000��57600��115200��128000��256000)
	// ���أ�
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_115200){ return COM_OPEN_FAIL;}
	DWORD Open(std::wstring strIP, const int nPort);
	DWORD Open(const BYTE bPort, const wchar_t* wchPath){ return COM_OPEN_FAIL;}
	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���أ�
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	
	bool IsOpen(void) const;
	////////////////////////////////////////////////////////////////////////////////////���ڲ���end

public:
	// �������Ƿ�����
	bool IsAdapterOnline(void);
	// ��ʼ��������
	bool Init(DWORD &dwProtocol, const int nFuelType);
	// ��ʼ����״̬
	bool GetInitState(OBDInitState &enState, DWORD &dwProtocol);
	// ȡ������
	bool GetDTC(std::vector<SDTCData> &vtDTC);
	// ȡ����ָʾ��״̬
	bool GetLampState(bool &bIsOn);
	// ȡϵͳ�����
	bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem);
	// ���������
	bool ClearDTC(void);
	// ȡʵʱ����
	bool GetRTData(int &nRev, float &fV, float &fTemp);
	bool GetRTData(std::map<std::wstring, std::wstring> &mapData);
	// ��ȡ������̣���λkm
	bool GetDTCMileage(DWORD &dwMileage);
	// ��ȡ��������
	bool GetFreezeData(std::wstring &strData);
	// ȡVIN
	bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL);
	// ȡOBD��ʽ����Ҫ��
	bool GetOBDType(std::wstring &strType);
	// ȡ��ʻ��̣���λkm
	bool GetMileage(DWORD &dwMileage);
	// ȡECU��Ϣ
	bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// ȡ����汾
	bool GetSoftwareVer(std::wstring &strVer);
	// ȡIUPR����
	bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// ����ʼ��
	bool UnInit(void);
	// ȡOBD����Э��
	std::wstring GetOBDProtocolName(DWORD dwProtocol);

	int JBT_OBDDiagnosisInit(void){return 99;}
	int JBT_CommInit(void){return 99;}
	bool GetScanStartTest(std::wstring& strProgress, DWORD &dwProtocol){return true;}

private:
	// ������IP��ַ
	std::wstring m_strServerIP;
	// �������˿ں�
	unsigned short m_usServerPort;

	// ��������
	int m_nRev;		// ������ת��
	float m_fV;		// ����
	float m_fTemp;	// ��ȴҺ�¶�
	int m_nFuelType;	// ȼ�����ͣ�1���ͣ�2����
	std::map<std::wstring, std::wstring> m_mapRTData;

	// �ϴβ���ʱ��
	clock_t m_LastOPTime;
	// ȡʵʱ����
	int m_nGetRTDataTimeInterval;

};
#endif