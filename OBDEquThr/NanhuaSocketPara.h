/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaSocketPara.h
* 文件标识：
* 摘要：参数测量仪实体类(通过Socket获取)
*
* 版本：1.0
* 作者：Raylee
* 日期：2018-10-24
*
* 历史信息：
*

* 版本：1.0.0
* 日期：2018-10-24
* 作者：Raylee
* 描述：正式发布使用
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
	WORD wHeader;				// 包头2字节 EP	
	DWORD dwPacketLen;			// 包长度，4字节
	std::wstring strXML;		// UTF8格式的xml字符串
	BYTE bTail;					// 包尾1字节 $
	
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
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// 返回打开结果：
	// ANA_OPEN_SUCCEED
	// ANA_OPEN_FAIL
	DWORD Open(const BYTE bPort) { return PARA_OPEN_FAIL;}
	DWORD Open(std::wstring strIP, const int nPort);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回关闭结果：
	// NHC_CLOSE_SUCCEED
	// NHC_CLOSE_FAIL_WIN32API
	// NHC_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

private:
	// 通过socket获取环境参数
	bool GetEP(float &fET, float &fRH, float &fAP);
	
	// 发送接收数据包
	bool SendAndRecvPacket(EPPacket sSend, EPPacket &sRecv);
	
	// 接收数据
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// 发送数据
	bool SendAll(SOCKET s, char *sendbuf, int len);
	// 解析XML报文
	bool Xml2Map(LPCTSTR szXML, 
		std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, 
		std::map<std::wstring, std::wstring> &mapData);

public:
	// 温度斜率校准
	DWORD EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 湿度斜率校准
	DWORD RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 大气压斜率校准
	DWORD AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 油温冷端斜率校准
	DWORD OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 油温热端斜率校准
	DWORD OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 温度零位校准
	DWORD EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 湿度零位校准
	DWORD RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 大气压零位校准
	DWORD AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 油温冷端零位校准
	DWORD OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 油温热端零位校准
	DWORD OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 备份校准参数
	DWORD BackupCalibrationParameters(BYTE *const pbResult) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 恢复校准参数
	DWORD RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 设置冲程为4
	DWORD SetStrokeTypeToFourStroke(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 设置冲程为2
	DWORD SetStrokeTypeToTwoStroke(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 设置点火方式为单次
	DWORD SetFireTypeToSingleFire(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 设置点火方式为二次
	DWORD SetFireTypeToDoubleFire(BYTE *const pbResult = NULL) { return PARA_WR_EQUIP_RETURE_0X15;}

	// 读全部数据
	DWORD GetEntireData(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL);

	// 读全部数据
	DWORD GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm,
		WORD *const pwStatus);

	// 读环境参数
	DWORD GetEnvironmentParameters(float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL);

	// 读转速油温
	DWORD GetOilTempRpm(float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL){ return PARA_WR_EQUIP_RETURE_0X15;}
	
	// 数据传输
	DWORD DataTrans(const BYTE * pbDataBuf, const UINT nDatalen);

	// 获取IMEI和ICCID
	DWORD GetIMEI_ICCID(char* chIMEI, char* chICCID);

protected:
	// 客户端连接线程
	SOCKET m_socket;

	// 临界保护量
	CRITICAL_SECTION m_sCriticalSection;

protected:
		// 日志文件路径（主程序）
	CStringW m_strLogFilePath;
	// 本模块名
	CStringW m_strModName;
	// 生成日志文件
	void GenLogFile(void);
};
#endif