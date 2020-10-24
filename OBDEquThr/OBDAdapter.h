/*
* Copyright (c) 佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：OBDAdapter.h
* 文件标识：
* 摘要：OBD适配器抽象类，以纯虚拟函数形式定义
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

#ifndef OBDAdapter_H
#define OBDAdapter_H

#include <vector>
#include <string>
#include <map>

// 串口操作结果
#define COM_OPEN_SUCCEED							0x00  // 打开串口成功
#define COM_OPEN_FAIL								0x01  // 打开串口失败
#define COM_CLOSE_SUCCEED							0x00  // 关闭串口成功
#define COM_CLOSE_FAIL_WIN32_API					0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define COM_CLOSE_FAIL_NO_OPEN_PORT					0x02  // 关闭串口失败：没有可以关闭的串口
#define COM_WR_SUCCEED								0x00  // 写读成功
#define COM_WR_PORT_NOT_OPEN						0x01  // 写读失败：串口未打开
#define COM_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // 写读失败：写指定数据失败
#define COM_WR_READ_SPECIFIED_DATA_FAIL				0x03  // 写读失败：读指定数据失败
#define COM_WR_EQUIP_RETURE_0X15					0x04  // 写读失败：仪器返回错误代码（0x15）
#define COM_WR_CHECK_SUM_FAIL						0x05  // 写读失败：读出数据校验和失败
#define COM_WR_UNKNOWN_ERROR						0xFF  // 写读失败：未知错误

// OBD异步
#define WM_OBD_INIT													 WM_USER + 0x1201  // 初始化
#define WM_OBD_GET_DTC										 		 WM_USER + 0x1202  // 取故障码
#define WM_OBD_GET_ENGINE_REV										 WM_USER + 0x1203  // 取转速
#define WM_OBD_GET_VELOCITY											 WM_USER + 0x1204  // 取车速
#define WM_OBD_GET_COOLANT_TEMP										 WM_USER + 0x1205  // 取冷却液温度
#define WM_OBD_GET_SYSTEMCHECK										 WM_USER + 0x1206  // 取系统检测结果
#define WM_OBD_GET_RT_DATA											 WM_USER + 0x1207  // 取数据流

//BCD转十进制数
#define BCD2DEC(b)	(((b)>>4)*10+((b)&0x0F))

// 定义类型
enum OBDInitState
{
	OBD_InitSucceed,
	OBD_InitFailed,
	OBD_Initing,
	OBD_Offline,
	OBD_Online,
};

struct SOBDSystemCheckItem
{
	// 0：不支持/不适用，1：完成，2：未完成；
	int nMFT;
	int nFST;
	int nCC;	// 汽油
	int nCCH;
	int nOS;	// 汽油
	int nOSH;	// 汽油
	int nEECS;
	int nEGR;	// 汽油，柴油
	int nSAIS;
	int nICM;
	int nVVT;

	//int nFST;
	//int nEGR;
	int nDOC;	// 柴油
	int nSCR;	// 柴油
	int nDPF;	// 柴油
	int nASC;
	int nPOC;	// 柴油

	SOBDSystemCheckItem(){ZeroMemory(this, sizeof(SOBDSystemCheckItem));}
};

struct SDTCData
{
	std::wstring strCode;
	std::wstring strComment;
};

class COBDAdapter
{
public:
	COBDAdapter(void);
	virtual ~COBDAdapter(void);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// 参数：
	// in bPort：串口号(1~255)
	// in nBaudRate：波特率 (110、300、600、1200、2400、4800、9600、14400、19200、38400、56000、57600、115200、128000、256000)
	// 返回打开结果：
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort, const int nBaudRate = CBR_2400) = 0;
	virtual DWORD Open(std::wstring strIP, const int nPort) = 0;

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回：
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32_API
	// COM_CLOSE_FAIL_NO_OPEN_PORT
	DWORD Close(void);

	// 判断串口是或打开
	virtual bool IsOpen(void) const;
	///////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

protected:
	///////////////////////////////////////////////////////////////////////////////////串口读写start
	// 写串口数据
	// pbWriteBuffer：要写入的数据内容，不能为空指针
	// dwNumberOfBytesToWrite：要写入的数据长度
	bool WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite);

	// 读串口数据
	// pbReadBuffer：要读取的数据内容，不能为空指针
	// dwNumberOfBytesToRead：要读取的数据长度
	bool ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead);

	// 计算校验和
	// 参数：
	// pbData：要进行校验的数据内容
	// uiStart：要进行校验的数据开始位置
	// uiEnd：要进行校验的数据结束位置
	// pbCRC_HIGH：返回校验和高位
	// pbCRC_LOW：返回校验和低位
	BYTE CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW);
	///////////////////////////////////////////////////////////////////////////////////串口读写end

public:
	//////////////////////////////////////////////////////////////////////////////////////// 仪器操作 start
	// 适配器是否在线
	virtual bool IsAdapterOnline(void) = 0;
	// 初始化适配器
	virtual bool Init(DWORD &dwProtocol, const int nFuelType) = 0;
	// 初始化的状态
	virtual bool GetInitState(OBDInitState &enState, DWORD &dwProtocol) = 0;
	// 取故障码
	virtual bool GetDTC(std::vector<SDTCData> &vtDTC) = 0;	
	// 取故障指示灯状态
	virtual bool GetLampState(bool &bIsOn) = 0;
	// 取系统检测结果
	virtual bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem) = 0;
	// 清除故障码
	virtual bool ClearDTC(void) = 0;
	// 取实时数据
	virtual bool GetRTData(int &nRev, float &fV, float &fTemp) = 0;
	virtual bool GetRTData(std::map<std::wstring, std::wstring> &mapData) = 0;
	// 获取故障里程，单位km
	virtual bool GetDTCMileage(DWORD &dwMileage) = 0;
	// 获取冻结数据
	virtual bool GetFreezeData(std::wstring &strData) = 0;
	// 取VIN
	virtual bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL) = 0;
	// 取OBD型式检验要求
	virtual bool GetOBDType(std::wstring &strType) = 0;
	// 取行驶里程，单位km
	virtual bool GetMileage(DWORD &dwMileage) = 0;
	// 取ECU信息
	virtual bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL) = 0;
	// 取软件版本
	virtual bool GetSoftwareVer(std::wstring &strVer) = 0;
	// 取IUPR数据
	virtual bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL) = 0;
	// 反初始化
	virtual bool UnInit(void) = 0;
	// 取OBD总线协议
	virtual std::wstring GetOBDProtocolName(DWORD dwProtocol) = 0;
	//////////////////////////////////////////////////////////////////////////////////////// 仪器操作 end

protected:
	// 串口句柄
	HANDLE m_hComm;
	//// 临界保护量
	//CRITICAL_SECTION m_sCriticalSection;

	// 临界区封装类
	class CSLock:public CRITICAL_SECTION
	{
	public:
		CSLock()
		{
			InitializeCriticalSection(this);
		}

		~CSLock()
		{
			DeleteCriticalSection(this);
		}

		void Enter()
		{
			EnterCriticalSection(this);
		}

		void Leave()
		{
			LeaveCriticalSection(this);
		}
	};

	// 自动锁，自动落锁解锁
	class CAutoLock
	{
	public:
		CAutoLock(CSLock& lock)
			:m_csLock(lock)
		{
			Lock();
		}
		~CAutoLock()
		{
			UnLock();
		}

	public:
		// 一般不手动调用
		void Lock()
		{
			m_csLock.Enter();
		}

		// 一般不手动调用
		void UnLock()
		{
			m_csLock.Leave();
		}

	private:
		CSLock& m_csLock;
	};
	
	static CSLock & GetLogLocker();	
};
#endif