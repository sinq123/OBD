#include "StdAfx.h"
#include "EVOP.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>

CEVOP::CEVOP(void)
	: m_hComm(INVALID_HANDLE_VALUE)
{
}


CEVOP::~CEVOP(void)
{
}

CEVOP::CSLock & CEVOP::GetLogLocker()
{
	static CSLock csLock;
	return csLock;
}

DWORD CEVOP::Close(void)
{
	if (IsOpen())
	{
		BOOL bCloseHandle = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (bCloseHandle)
		{
			// 关闭串口成功
			return COM_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return COM_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return COM_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CEVOP::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

bool CEVOP::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
{
	if (!IsOpen())
	{
		return false;
	}

	DWORD dwBytesWritten(0x00);
	// 清除缓冲数据
	//PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	// 写Comm
	if (WriteFile(m_hComm, pbWriteBuffer, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
	{
		if (dwNumberOfBytesToWrite == dwBytesWritten)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CEVOP::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
{
	if (!IsOpen())
	{
		return false;
	}

	DWORD dwBytesRead(0x00);
	// 读Comm
	BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwNumberOfBytesToRead, &dwBytesRead, NULL);
	if (bSucceed && dwNumberOfBytesToRead==dwBytesRead)
	{	
		return true;
	}
	else
	{
		return false;
	}
}
