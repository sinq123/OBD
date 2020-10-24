/*
* Copyright (c) 佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：EVOP.h
* 文件标识：
* 摘要：汽车燃油蒸发系统综合检测仪抽象类，以纯虚拟函数形式定义
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

#ifndef EVOP_H
#define EVOP_H

#include <vector>

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

class CEVOP
{
public:
	CEVOP(void);
	virtual ~CEVOP(void);

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
	///////////////////////////////////////////////////////////////////////////////////串口读写end

public:
	//////////////////////////////////////////////////////////////////////////////////////// 仪器操作 start
	// 初始化适配器
	virtual DWORD StartTest(const int nItem, const int nType) = 0;
	// 取实时数据
	virtual DWORD GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T) = 0;
	// 取状态
	virtual DWORD GetState(int &nState) = 0;
	// 取反馈信息
	virtual DWORD GetTestInfo(int &nTestState, int &nTimeCount) = 0;
	// 结束检测
	virtual DWORD EndTest(int &nItem, int &nJudge) = 0;
	//////////////////////////////////////////////////////////////////////////////////////// 仪器操作 end

protected:
	// 串口句柄
	HANDLE m_hComm;

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