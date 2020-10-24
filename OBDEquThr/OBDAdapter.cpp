#include "StdAfx.h"
#include "OBDAdapter.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>

#define STX		0x02  // STX 标识
#define ACK		0x06  // ACK 标识
#define NAK		0x15  // NAK 标识
#define ETB		0x17  // ETB 标识
#define ETX		0x03  // ETX 标识
//#define ADD		0x36  // 地址码

COBDAdapter::COBDAdapter(void)
	: m_hComm(INVALID_HANDLE_VALUE)
{
	//::InitializeCriticalSection(&m_sCriticalSection);
}


COBDAdapter::~COBDAdapter(void)
{
	//::DeleteCriticalSection(&m_sCriticalSection);
}

COBDAdapter::CSLock & COBDAdapter::GetLogLocker()
{
	static CSLock csLock;
	return csLock;
}

DWORD COBDAdapter::Close(void)
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

bool COBDAdapter::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

bool COBDAdapter::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
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

bool COBDAdapter::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
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

BYTE COBDAdapter::CheckSum(const BYTE* pbData, const UINT uiBegin, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
	assert(NULL != pbData);
	assert(0UL != uiEnd);
	assert(uiEnd >= uiBegin);
	assert(NULL != pbCRC_HIGH);
	assert(NULL != pbCRC_LOW);

	BYTE bSum(0x00);
	for (UINT ui=uiBegin; ui<=uiEnd; ++ui)
	{
		bSum += pbData[ui];
	}

	char chCrc[3] = {'\0'};
	_snprintf_s(chCrc, sizeof(chCrc), _TRUNCATE, "%02X", bSum);
	(*pbCRC_HIGH) = (BYTE)chCrc[0];
	(*pbCRC_LOW) = (BYTE)chCrc[1];
	return bSum;
}
