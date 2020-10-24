/*
* Copyright (c) 佛山市华贝软件技术有限公司
* All rights reserved.
*
* 文件名称：ZhengDeOBD.h
* 文件标识：
* 描述: 深圳正德友邦ODB数据交互
*
* 版本：1.0
* 日期：2018-06-01
* 作者：Raylee
*
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2018-06-01
* 作者：Raylee
* 描述：正在开发
*/

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef ZhengDeOBD_H
#define ZhengDeOBD_H

#include <vector>
#include "OBDAdapter.h"

struct ZDPacket
{
	WORD wHeader;				// 包头2字节 NH	
	char szPacketLen[7];		// 包长度6字节字符串表示
	std::wstring strXML;		// UTF8格式的xml字符串
	BYTE bTail;					// 包尾1字节 $
	
	ZDPacket()
	{
		ZDPacket(L"");
	}

	ZDPacket(LPCTSTR szXML)
	{
		strXML = szXML;

		wHeader = MAKEWORD('N', 'H');
		memset(szPacketLen, 0x00, sizeof(szPacketLen));
		bTail = '$';
	}

	std::string ToString()
	{
		std::string strData = TCharToUTF8(strXML.c_str());

		std::string strRet;
		strRet.append((char *)&wHeader, sizeof(wHeader));

		sprintf_s(szPacketLen, "%06d", min(strData.size()+9, 999999));
		strRet.append((char *)&szPacketLen, sizeof(szPacketLen)-1);	

		strRet.append(strData);
		strRet.append((char *)&bTail, sizeof(bTail));

		return strRet;
	}

	bool Prase(std::string strRecv)
	{
		if (strRecv.size() < 9)
		{
			return false;
		}

		int nOffset = 0;
		memcpy(&wHeader, strRecv.data()+nOffset, sizeof(wHeader));
		nOffset += sizeof(wHeader);
		// 校验包尾
		if (wHeader != MAKEWORD('N', 'H'))
		{
			return false;
		}
		
		memcpy(&szPacketLen, strRecv.data()+nOffset, sizeof(szPacketLen)-1);
		szPacketLen[6] = '\0';
		nOffset += sizeof(szPacketLen)-1;
		unsigned int nPacketLen = atoi(szPacketLen);	
		// 校验数据长度
		if (nPacketLen != strRecv.size())
		{
			return false;
		}
		
		std::string strData;
		strData.assign(strRecv.data()+nOffset, nPacketLen-9);
		nOffset += nPacketLen-9;

		memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
		nOffset += sizeof(bTail);
		
		// 校验包尾
		if (bTail != '$')
		{
			return false;
		}

		strXML = UTF8ToTChar(strData.c_str());

		return true;
	}
};

class CZhengDeOBD : public COBDAdapter
{
public:
	CZhengDeOBD(void);
	~CZhengDeOBD(void);

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
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_115200){ return COM_OPEN_FAIL;}
	DWORD Open(std::wstring strIP, const int nPort);
	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回：
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	
	bool IsOpen(void) const;
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
	bool SendAndRecvPacket(ZDPacket sZYPSend, ZDPacket &sZYPRecv);
	
	// 接收数据
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// 发送数据
	bool SendAll(SOCKET s, char *sendbuf, int len);
	
	bool Xml2Map(LPCTSTR szXML, 
		std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, 
		std::map<std::wstring, std::wstring> &mapData);

	// 客户端连接线程
	SOCKET m_socket;
	//// 服务器IP地址
	//char m_chServerIP[50];
	//// 服务器端口号
	//unsigned short m_usServerPort;

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

	// 日志文件路径（主程序）
	CStringW m_strLogFilePath;
	// 本模块名
	CStringW m_strModName;
	// 生成日志文件
	void GenLogFile(void);

};
#endif