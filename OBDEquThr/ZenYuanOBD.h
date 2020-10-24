/*
* Copyright (c) 佛山市华贝软件技术有限公司
* All rights reserved.
*
* 文件名称：ZenYuanOBD.h
* 文件标识：
* 描述: 广州正原ODB适配器
*
* 版本：1.0
* 日期：2017-11-18
* 作者：Raylee
*
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2017-11-18
* 作者：Raylee
* 描述：正在开发
*/



#pragma once

#ifndef ZenYuanOBD_H
#define ZenYuanOBD_H

#include <vector>
#include "OBDAdapter.h"

struct ZYPacket
{
	WORD wHeader;			// 帧头2字节 0xAA 0x55
	WORD wPacketLen;		// 长度2字节 0x00 0x05 = 5
	BYTE bCmdG;				// 分组Group指令
	BYTE bCmdF;				// 功能Function指令
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
		// 计算长度
		wPacketLen = 0;
		wPacketLen += sizeof(wPacketLen);
		wPacketLen += sizeof(bCmdG);
		wPacketLen += sizeof(bCmdF);
		wPacketLen += strData.size();
		wPacketLen = ntohs(wPacketLen);	// 高低字节翻转

		// 计算校验
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
		wPacketLen = ntohs(wPacketLen);	// 高低字节翻转
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
		
		// 校验和
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
	////////////////////////////////////////////////////////////////////////////////////串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// 参数：
	// bPort：串口号(1~255)
	// nBaudRate：波特率 (110、300、600、1200、2400、4800、9600、14400、19200、38400、56000、57600、115200、128000、256000)
	// 返回：
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_115200);
	DWORD Open(std::wstring strIP, const int nPort) { return COM_OPEN_FAIL;}

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回：
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	////////////////////////////////////////////////////////////////////////////////////串口操作end

public:
	// 适配器是否在线
	bool IsAdapterOnline(void);
	// 初始化适配器
	bool Init(DWORD &dwProtocol, const int nFuelType);
	// 初始化的状态
	bool GetInitState(OBDInitState &enState, DWORD &dwProtocol);
	// 取故障码
	bool GetDTC(std::vector<SDTCData> &vtDTC);
	// 取故障指示灯状态
	bool GetLampState(bool &bIsOn);
	// 取系统检测结果
	bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem);
	// 清除故障码
	bool ClearDTC(void);
	// 取实时数据
	bool GetRTData(int &nRev, float &fV, float &fTemp);
	bool GetRTData(std::map<std::wstring, std::wstring> &mapData);
	// 获取故障里程，单位km
	bool GetDTCMileage(DWORD &dwMileage);
	// 获取冻结数据
	bool GetFreezeData(std::wstring &strData);
	// 取VIN
	bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL);
	// 取OBD型式检验要求
	bool GetOBDType(std::wstring &strType);
	// 取行驶里程，单位km
	bool GetMileage(DWORD &dwMileage);
	// 取ECU信息
	bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// 取软件版本
	bool GetSoftwareVer(std::wstring &strVer);	
	// 取IUPR数据
	bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// 反初始化
	bool UnInit(void);
	// 取OBD总线协议
	std::wstring GetOBDProtocolName(DWORD dwProtocol);

private:
	bool SendCMD(ZYPacket sZYPSend, ZYPacket &sZYPRecv);
	
	// 缓存数据
	int m_nRev;		// 发动机转速
	float m_fV;		// 车速
	float m_fTemp;	// 冷却液温度
	int m_nFuelType;	// 燃油类型，1汽油；2柴油
	std::map<std::wstring, std::wstring> m_mapRTData;

	// 上次操作时刻
	clock_t m_LastOPTime;
	// 取实时数据
	int m_nGetRTDataTimeInterval;

	// 是否已经初始化
	bool m_bIsInit;

	// OBDType
	std::map<BYTE, std::wstring> m_mapOBDType;
};
#endif