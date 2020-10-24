#include "StdAfx.h"
#include "HuaGongVTET1000.h"

#include <cassert>
#include <cstdio>
#include <string>

CHuaGongVTET1000::CHuaGongVTET1000(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_csComm);
}

CHuaGongVTET1000::~CHuaGongVTET1000(void)
{
	Close();

	::DeleteCriticalSection(&m_csComm);
}

// 打开串口
DWORD CHuaGongVTET1000::Open(BYTE bPort)
{
	// 关闭已存在的串口句柄
	Close();

	// 进入串口操作临界区
	::EnterCriticalSection(&m_csComm);

	wchar_t *pwchPort = new wchar_t[20]();
	swprintf_s(pwchPort, 20, L"\\\\.\\COM%hhu", bPort);
	m_hComm = CreateFileW(pwchPort,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	delete[] pwchPort;
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		OutputDebugStringW(L"Error:fail to open com port\n");

		// 离开串口操作临界区
		::LeaveCriticalSection(&m_csComm);
		// 打开串口失败
		return TAC_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数begin
	// 设置输入和输出缓存区大小
	SetupComm(m_hComm, 512, 256);
	// 设置DCB结构
	DCB dcb;
	// 获取缺省的DCB结构的值
	GetCommState(m_hComm, &dcb);
	// 设定波特率为38400 bps
	dcb.BaudRate = CBR_38400;
	// 设定无奇偶校验
	dcb.Parity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;
	// 设置串行设备控制参数
	SetCommState(m_hComm, &dcb);
	// 进行超时设置（200毫秒之内要执行完读写操作）
	// 注意,不能够设置CommTimeouts.ReadTotalTimeoutConstant = 100;因为比较慢的计算机,将会发生超时。
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	// 离开串口操作临界区
	::LeaveCriticalSection(&m_csComm);
	// 打开串口成功
	return TAC_OPEN_SUCCEED;
}

bool CHuaGongVTET1000::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

// 关闭串口
DWORD CHuaGongVTET1000::Close(void)
{
	if (IsOpen())
	{
		// 清缓冲区
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		// 进入串口操作临界区
		::EnterCriticalSection(&m_csComm);

		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;

		// 离开串口操作临界区
		::LeaveCriticalSection(&m_csComm);

		if (b)
		{
			// 关闭串口成功
			return TAC_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return TAC_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return TAC_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

DWORD CHuaGongVTET1000::GetRev(int *pnRev)
{
	assert(NULL != pnRev);
	
	// 检查串口是否打开
	if (!IsOpen())
	{
		// 写读失败：串口未打开
		return TAC_WR_PORT_NOT_OPEN;
	}

	// 进入串口操作临界区
	::EnterCriticalSection(&m_csComm);

	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	DWORD dw(0);

	const DWORD dwBytesToRead(14);	
	DWORD dwBytesRead(0);
	char szBuf[128] = {0};
	BOOL b = ReadFile(m_hComm, szBuf, dwBytesToRead, &dwBytesRead, NULL);

	// 离开串口操作临界区
	::LeaveCriticalSection(&m_csComm);

	//返回 "fdjzsj:0000rpm";
	if (b && dwBytesToRead==dwBytesRead)
	{
		std::string strTmp = szBuf;
		if (strTmp.substr(0, 7) == "fdjzsj:")
		{
			*pnRev = atoi(strTmp.substr(7, 4).c_str());

			// 写读成功
			dw = TAC_WR_SUCCEED;
		}
		else
		{
			*pnRev = 0;
			// 写读失败：读指定数据失败
			dw = TAC_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		// 写读失败：读指定数据失败
		dw = TAC_WR_READ_SPECIFIED_DATA_FAIL;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////end

	return dw;
}