/*
* Copyright (c) ��ɽ�л�������������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHEVOP.h
* �ļ���ʶ��
* ����: NHEVAP-1����ȼ������ϵͳ�ۺϼ����
*
* �汾��1.0
* ���ڣ�2018-08-03
* ���ߣ�Raylee
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2018-08-03
* ���ߣ�Raylee
* ���������ڿ���
*/



#pragma once

#ifndef NHEVOP_H
#define NHEVOP_H

#include <vector>
#include "EVOP.h"

struct NHEVOPPacket
{
	BYTE bHeader;			// ֡ͷ1�ֽ� 0x02
	BYTE bAddress;			// ��ַ1�ֽ� 0x04
	WORD wPacketLen;		// ����2�ֽ� 0x30 0x35 = 05
	BYTE bCmd;				// ָ��1�ֽ�
	std::string strData;	// ���ݲ�����
	WORD wSum;				// У���
	BYTE bTail;				// ֡β1�ֽ� 0x03
	
	NHEVOPPacket()
	{
		NHEVOPPacket(0);
	}
	NHEVOPPacket(BYTE bCmd)
	{
		this->bHeader = 0x02;
		this->bAddress = 0x04;
		this->wPacketLen = 0x00;
		this->bCmd = bCmd;
		this->wSum = 0x00;
		this->bTail = 0x03;
	}

	std::string ToString()
	{
		Update();

		std::string strRet;
		strRet.append((char *)&bHeader, sizeof(bHeader));
		strRet.append((char *)&bAddress, sizeof(bAddress));
		strRet.append((char *)&wPacketLen, sizeof(wPacketLen));
		strRet.append((char *)&bCmd, sizeof(bCmd));
		strRet.append(strData);
		strRet.append((char *)&wSum, sizeof(wSum));
		strRet.append((char *)&bTail, sizeof(bTail));

		return strRet;
	}

	void Update()
	{
		// ���㳤��
		unsigned char nPacketLen = 0;
		nPacketLen += sizeof(bCmd);
		nPacketLen += strData.size();
		char szPacketLen[3] = {0};
		sprintf_s(szPacketLen, "%02d", nPacketLen);
		memcpy(&wPacketLen, szPacketLen, 2);

		// ����У��
		BYTE bSum = bAddress;
		bSum += ((wPacketLen&0xFF00)>>8) + (wPacketLen&0xFF);
		bSum += bCmd;
		for (WORD i=0; i<strData.size(); i++)
		{
			bSum += strData.at(i);
		}
		char szSum[3] = {0};
		sprintf_s(szSum, "%02X", bSum);
		memcpy(&wSum, szSum, 2);
	}

	bool Prase(std::string strRecv)
	{
		if (strRecv.size() < sizeof(bHeader)+sizeof(wPacketLen)+sizeof(bCmd)+sizeof(wSum)+sizeof(bTail))
		{
			return false;
		}

		int nOffset = 0;
		// ����ͷ
		memcpy(&bHeader, strRecv.data()+nOffset, sizeof(bHeader));
		nOffset += sizeof(bHeader);
		if (bHeader != 0x02)
		{
			return false;
		}

		// ������ַ
		memcpy(&bAddress, strRecv.data()+nOffset, sizeof(bAddress));
		nOffset += sizeof(bAddress);
		if (bAddress != 0x84)
		{
			return false;
		}
		
		// ��������
		memcpy(&wPacketLen, strRecv.data()+nOffset, sizeof(wPacketLen));
		nOffset += sizeof(wPacketLen);

		char szPacketLen[3] = {0};
		memcpy(szPacketLen, &wPacketLen, 2);		
		int nPacketLen = atoi(szPacketLen);
		if (nPacketLen+7 != strRecv.size())
		{
			return false;
		}

		// ��������
		memcpy(&bCmd, strRecv.data()+nOffset, sizeof(bCmd));
		nOffset += sizeof(bCmd);
		
		// ��������
		strData.assign(strRecv.data()+nOffset, nPacketLen-sizeof(bCmd));
		nOffset += nPacketLen-sizeof(bCmd);

		// У���
		memcpy(&wSum, strRecv.data()+nOffset, sizeof(wSum));
		nOffset += sizeof(wSum);
		
		BYTE bSumCalc = bAddress;
		bSumCalc += ((wPacketLen&0xFF00)>>8) + (wPacketLen&0xFF);
		bSumCalc += bCmd;
		for (WORD i=0; i<strData.size(); i++)
		{
			bSumCalc += strData.at(i);
		}
		char szSum[3] = {0};
		sprintf_s(szSum, "%02X", bSumCalc);
		if (memcmp(&wSum, szSum, 2) != 0)
		{
			return false;
		}

		// ����β
		memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
		nOffset += sizeof(bTail);
		if (bTail != 0x03)
		{
			return false;
		}

		return true;
	}
};

class CNHEVOP : public CEVOP
{
public:
	CNHEVOP(void);
	~CNHEVOP(void);

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
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_9600);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���أ�
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	////////////////////////////////////////////////////////////////////////////////////���ڲ���end

public:
	// ��ʼ���
	DWORD StartTest(const int nItem, const int nType);
	// ȡʵʱ����
	//P1��ͨ��1��ƽ����ѹֵ��
	//P2��ͨ��2��ƽ����ѹֵ��
	//P1_T��ͨ��1�ļ�ʱ��ѹֵ��
	//P2_T��ͨ��2�ļ�ʱ��ѹֵ��
	DWORD GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T);
	// ȡ״̬
	DWORD GetState(int &nState);
	// ȡ������Ϣ
	DWORD GetTestInfo(int &nTestState, int &nTimeCount);
	// �������
	DWORD EndTest(int &nItem, int &nJudge);

private:
	DWORD SendCMD(NHEVOPPacket sZYPSend, NHEVOPPacket &sZYPRecv);
	
};
#endif