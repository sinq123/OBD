/*
* Copyright (c) ��ɽ�л�������������޹�˾
* All rights reserved.
*
* �ļ����ƣ�ZenYuanOBD.h
* �ļ���ʶ��
* ����: ������ԭODB������
*
* �汾��1.0
* ���ڣ�2017-11-18
* ���ߣ�Raylee
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2017-11-18
* ���ߣ�Raylee
* ���������ڿ���
*/



#pragma once

#ifndef ZenYuanOBD_H
#define ZenYuanOBD_H

#include <vector>
#include "OBDAdapter.h"

struct ZYPacket
{
	WORD wHeader;			// ֡ͷ2�ֽ� 0xAA 0x55
	WORD wPacketLen;		// ����2�ֽ� 0x00 0x05 = 5
	BYTE bCmdG;				// ����Groupָ��
	BYTE bCmdF;				// ����Functionָ��
	std::string strData;
	BYTE bSum;
	
	ZYPacket()
	{
		ZYPacket(0, 0);
	}
	ZYPacket(BYTE bCmdG, BYTE bCmdF)
	{
		this->wHeader = 0x55AA;
		this->wPacketLen = 0x04;
		this->bCmdG = bCmdG;
		this->bCmdF = bCmdF;
		this->bSum = 0x00;
	}

	std::string ToString()
	{
		Update();

		std::string strRet;
		strRet.append((char *)&wHeader, sizeof(wHeader));
		strRet.append((char *)&wPacketLen, sizeof(wPacketLen));
		strRet.append((char *)&bCmdG, sizeof(bCmdG));
		strRet.append((char *)&bCmdF, sizeof(bCmdF));
		strRet.append(strData);
		strRet.append((char *)&bSum, sizeof(bSum));

		return strRet;
	}

	void Update()
	{
		// ���㳤��
		wPacketLen = 0;
		wPacketLen += sizeof(wPacketLen);
		wPacketLen += sizeof(bCmdG);
		wPacketLen += sizeof(bCmdF);
		wPacketLen += strData.size();
		wPacketLen = ntohs(wPacketLen);	// �ߵ��ֽڷ�ת

		// ����У��
		bSum = ((wPacketLen&0xFF00)>>8) + (wPacketLen&0xFF);
		bSum += bCmdG;
		bSum += bCmdF;
		for (WORD i=0; i<strData.size(); i++)
		{
			bSum += strData.at(i);
		}
	}

	bool Prase(std::string strRecv)
	{
		if (strRecv.size() < sizeof(wHeader)+sizeof(wPacketLen)+sizeof(bCmdG)+sizeof(bCmdF)+sizeof(bSum))
		{
			return false;
		}

		int nOffset = 0;
		memcpy(&wHeader, strRecv.data()+nOffset, sizeof(wHeader));
		nOffset += sizeof(wHeader);
		if (wHeader != 0x55AA)
		{
			return false;
		}

		memcpy(&wPacketLen, strRecv.data()+nOffset, sizeof(wPacketLen));
		nOffset += sizeof(wPacketLen);
		wPacketLen = ntohs(wPacketLen);	// �ߵ��ֽڷ�ת
		if (wPacketLen+3 != strRecv.size())
		{
			return false;
		}

		memcpy(&bCmdG, strRecv.data()+nOffset, sizeof(bCmdG));
		nOffset += sizeof(bCmdG);

		memcpy(&bCmdF, strRecv.data()+nOffset, sizeof(bCmdF));
		nOffset += sizeof(bCmdF);
		
		strData.assign(strRecv.data()+nOffset, wPacketLen-sizeof(wPacketLen)-sizeof(bCmdG)-sizeof(bCmdF));
		nOffset += wPacketLen-sizeof(wPacketLen)-sizeof(bCmdG)-sizeof(bCmdF);

		memcpy(&bSum, strRecv.data()+nOffset, sizeof(bSum));
		nOffset += sizeof(bSum);
		
		// У���
		BYTE bSumCalc = ((wPacketLen&0xFF00)>>8) + (wPacketLen&0xFF);
		bSumCalc += bCmdG;
		bSumCalc += bCmdF;
		for (WORD i=0; i<strData.size(); i++)
		{
			bSumCalc += strData.at(i);
		}
		if (bSumCalc != bSum)
		{
			return false;
		}

		return true;
	}
};

class CZenYuanOBD : public COBDAdapter
{
public:
	CZenYuanOBD(void);
	~CZenYuanOBD(void);

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
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_115200);
	DWORD Open(std::wstring strIP, const int nPort) { return COM_OPEN_FAIL;}

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���أ�
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
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

private:
	bool SendCMD(ZYPacket sZYPSend, ZYPacket &sZYPRecv);
	
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

	// �Ƿ��Ѿ���ʼ��
	bool m_bIsInit;

	// OBDType
	std::map<BYTE, std::wstring> m_mapOBDType;
};
#endif