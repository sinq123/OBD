/*
* Copyright (c) 佛山市华贝软件技术有限公司
* All rights reserved.
*
* 文件名称：NHEVOP.h
* 文件标识：
* 描述: NHEVAP-1汽车燃油蒸发系统综合检测仪
*
* 版本：1.0
* 日期：2018-08-03
* 作者：Raylee
*
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2018-08-03
* 作者：Raylee
* 描述：正在开发
*/



#pragma once

#ifndef NHEVOP_H
#define NHEVOP_H

#include <vector>
#include "EVOP.h"

struct NHEVOPPacket
{
	BYTE bHeader;			// 帧头1字节 0x02
	BYTE bAddress;			// 地址1字节 0x04
	WORD wPacketLen;		// 长度2字节 0x30 0x35 = 05
	BYTE bCmd;				// 指令1字节
	std::string strData;	// 数据不定长
	WORD wSum;				// 校验和
	BYTE bTail;				// 帧尾1字节 0x03
	
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
		// 计算长度
		unsigned char nPacketLen = 0;
		nPacketLen += sizeof(bCmd);
		nPacketLen += strData.size();
		char szPacketLen[3] = {0};
		sprintf_s(szPacketLen, "%02d", nPacketLen);
		memcpy(&wPacketLen, szPacketLen, 2);

		// 计算校验
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
		// 解析头
		memcpy(&bHeader, strRecv.data()+nOffset, sizeof(bHeader));
		nOffset += sizeof(bHeader);
		if (bHeader != 0x02)
		{
			return false;
		}

		// 解析地址
		memcpy(&bAddress, strRecv.data()+nOffset, sizeof(bAddress));
		nOffset += sizeof(bAddress);
		if (bAddress != 0x84)
		{
			return false;
		}
		
		// 解析长度
		memcpy(&wPacketLen, strRecv.data()+nOffset, sizeof(wPacketLen));
		nOffset += sizeof(wPacketLen);

		char szPacketLen[3] = {0};
		memcpy(szPacketLen, &wPacketLen, 2);		
		int nPacketLen = atoi(szPacketLen);
		if (nPacketLen+7 != strRecv.size())
		{
			return false;
		}

		// 解析命令
		memcpy(&bCmd, strRecv.data()+nOffset, sizeof(bCmd));
		nOffset += sizeof(bCmd);
		
		// 解析数据
		strData.assign(strRecv.data()+nOffset, nPacketLen-sizeof(bCmd));
		nOffset += nPacketLen-sizeof(bCmd);

		// 校验和
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

		// 解析尾
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
	////////////////////////////////////////////////////////////////////////////////////串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// 参数：
	// bPort：串口号(1~255)
	// nBaudRate：波特率 (110、300、600、1200、2400、4800、9600、14400、19200、38400、56000、57600、115200、128000、256000)
	// 返回：
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_9600);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回：
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	////////////////////////////////////////////////////////////////////////////////////串口操作end

public:
	// 开始检测
	DWORD StartTest(const int nItem, const int nType);
	// 取实时数据
	//P1：通道1的平均气压值；
	//P2：通道2的平均气压值；
	//P1_T：通道1的即时气压值；
	//P2_T：通道2的即时气压值；
	DWORD GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T);
	// 取状态
	DWORD GetState(int &nState);
	// 取反馈信息
	DWORD GetTestInfo(int &nTestState, int &nTimeCount);
	// 结束检测
	DWORD EndTest(int &nItem, int &nJudge);

private:
	DWORD SendCMD(NHEVOPPacket sZYPSend, NHEVOPPacket &sZYPRecv);
	
};
#endif