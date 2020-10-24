#include "StdAfx.h"
#include "FuLiVMT2000.h"

#include <cassert>
#include <cstdio>
#include <string>

CFuLiVMT2000::CFuLiVMT2000(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_csComm);
}

CFuLiVMT2000::~CFuLiVMT2000(void)
{
	Close();

	::DeleteCriticalSection(&m_csComm);
}

// 打开串口
DWORD CFuLiVMT2000::Open(BYTE bPort)
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
	// 设定波特率为19200 bps
	dcb.BaudRate = CBR_19200;
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
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 200;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	// 离开串口操作临界区
	::LeaveCriticalSection(&m_csComm);
	// 打开串口成功
	return TAC_OPEN_SUCCEED;
}

bool CFuLiVMT2000::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

// 关闭串口
DWORD CFuLiVMT2000::Close(void)
{
	if (IsOpen())
	{
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

DWORD CFuLiVMT2000::GetRev(int *pnRev)
{
	assert(NULL != pnRev);

	// 检查串口是否打开
	if (!IsOpen())
	{
		// 写读失败：串口未打开
		return TAC_WR_PORT_NOT_OPEN;
	}
		
	DWORD dw(0);
	BYTE bWriteBuffer[9] = {0x4E, 0x54, 0x30, 0xFF, 0x53, 0x03, 0x01, 0x80, 0x7B};
	BYTE pbReadBuffer[50] = {0};
	DWORD dwReadLength = 50;
	dw = WriteAndReadData(bWriteBuffer, 9, pbReadBuffer, dwReadLength, true, true);


	if (TAC_WR_SUCCEED == dw)
	{
		if (dwReadLength>6 && pbReadBuffer[4]==0x41)
		{
			if (pnRev != NULL)
			{
				*pnRev = MAKEWORD(pbReadBuffer[9], pbReadBuffer[8]);
			}
		}
		else
		{
			return TAC_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	return dw;
}

DWORD CFuLiVMT2000::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD &dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
{
	assert(NULL!=pbWriteBuffer);
	assert(1UL<dwWriteLength);
	assert(NULL!=pbReadBuffer);
	assert(1UL<dwReadLength);

	::EnterCriticalSection(&m_csComm);
	if(IsOpen())
	{
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer+5, dwWriteLength-6);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		dwReadLength = 0;
		// 分二次读数，第一次数据头（6byte），第二次读剩下的有效数据
		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 6, &dwBytesRead, NULL);
		if (bSucceed && 6UL==dwBytesRead)
		{
			dwReadLength = dwBytesRead;
			// 判断非法或错误指令返回
			if(memcmp(pbReadBuffer, pbWriteBuffer, 4) == 0)	// 比较包头
			{
				// 读余下的数据
				bSucceed = ReadFile(m_hComm, pbReadBuffer+6, pbReadBuffer[5], &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(pbReadBuffer[5]))
				{
					dwReadLength += dwBytesRead;
					if (bNeedReadChkSum)
					{
						// 需要读校验
						if (pbReadBuffer[6+dwBytesRead-1] != CheckSum(pbReadBuffer+5, dwBytesRead))
						{
							::LeaveCriticalSection(&m_csComm);
							// 函数错误：校验读出数据失败
							return TAC_WR_CHECK_SUM_FAIL;
						}
						else
						{
							::LeaveCriticalSection(&m_csComm);
							// 函数成功
							return TAC_WR_SUCCEED;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_csComm);
						// 函数成功
						return TAC_WR_SUCCEED;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_csComm);
					// 函数错误：读指定数据失败（读余下有效数据失败）
					return TAC_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&m_csComm);
				// 函数错误：仪器返回错误状态码
				return TAC_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_csComm);
			// 函数错误：读指定数据失败（读数据头失败）
			return TAC_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_csComm);
		// 错误：串口没有打开
		return TAC_WR_PORT_NOT_OPEN;
	}
}

BYTE CFuLiVMT2000::CheckSum(BYTE *pb, UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	bSum = ~bSum;
	return bSum;                

}
