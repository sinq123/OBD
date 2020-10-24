#include "StdAfx.h"
#include "NanhuaEPCS4G.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>

#define STX		0x02  // STX 标识
#define ACK		0x06  // ACK 标识
#define NAK		0x15  // NAK 标识
#define ETB		0x17  // ETB 标识
#define ETX		0x03  // ETX 标识
#define ADD		0x08  // ETX 标识

CNanhuaEPCS4G::CNanhuaEPCS4G(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_sCriticalSection);
}

CNanhuaEPCS4G::~CNanhuaEPCS4G(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	::DeleteCriticalSection(&m_sCriticalSection);
}

// 打开串口
DWORD CNanhuaEPCS4G::Open(BYTE bPort)
{
	// 关闭已存在的串口句柄
	Close();

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
		// 打开串口失败
		OutputDebugStringW(L"\nError:fail to open com port\n");
		return PARA_OPEN_FAIL;
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
	// 进行超时设置（1000毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 5000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	return PARA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CNanhuaEPCS4G::Close(void)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CNanhuaEPCS4G::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

void CNanhuaEPCS4G::CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
	assert(NULL != pbData);
	assert(0UL != uiEnd);
	assert(uiEnd >= uiStart);
	assert(NULL != pbCRC_HIGH);
	assert(NULL != pbCRC_LOW);
	//assert(sizeof(pbData) >= uiEnd);

	WORD wSum(0x00);
	for (UINT ui=uiStart; ui<=uiEnd; ui++)
	{
		wSum += pbData[ui];
	}
	/////////////////////////////////////
	wSum &= 0xFF;
	char chCrc[3];
	ZeroMemory(chCrc, sizeof(chCrc));
	sprintf_s(chCrc, 3, "%02X", wSum);
	(*pbCRC_HIGH) = chCrc[0];
	(*pbCRC_LOW) = chCrc[1];
	/////////////////////////////////////
}

bool CNanhuaEPCS4G::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
{
	// 缓冲区数据长度必须大于写入数据的长度
	//assert(sizeof(pbWriteBuffer) >= dwNumberOfBytesToWrite);

	DWORD dwBytesWritten(0x00);

	// 清除缓冲数据
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
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

bool CNanhuaEPCS4G::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
{
	// 读取的数据的长度必须大于10
	// 数据包头(8位)+错误返回(8位)
	assert(dwNumberOfBytesToRead >= 16);
	//assert(sizeof(pbReadBuffer) >= 16);
	//assert(sizeof(pbReadBuffer) >= dwNumberOfBytesToRead);

	DWORD dwBytesRead(0x00);

	// 读Comm
	DWORD dwNumberOfCommandToRead = 8;
	BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwNumberOfCommandToRead, &dwBytesRead, NULL);
	if (bSucceed && dwNumberOfCommandToRead==dwBytesRead)
	{
		// 判断非法或错误指令返回
		if(ACK == pbReadBuffer[6]) // 正常
		{
			// 读余下的数据
			// 余下数据:需要读取的数据
			bSucceed = ReadFile(m_hComm, pbReadBuffer+8, dwNumberOfBytesToRead-8, &dwBytesRead, NULL);
			if(bSucceed && dwBytesRead==(dwNumberOfBytesToRead-8))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(NAK == pbReadBuffer[6]) // 异常
		{
			// 读余下的数据
			// 余下数据:数据长度(2位)+<ETB>+数据(1位)+<ETB>+校验码(2位)+<ETX>
			const DWORD dwNumberOfErrorToRead(8);
			bSucceed = ReadFile(m_hComm, pbReadBuffer+8, dwNumberOfErrorToRead, &dwBytesRead, NULL);
			if(bSucceed && dwNumberOfErrorToRead==dwBytesRead)
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
	else
	{
		return false;
	}
}

DWORD CNanhuaEPCS4G::EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		if(fSlope >= 0 )
		{
			int nSlope = int((fSlope + 0.05)*10.0f)%10000;
			sprintf_s(chSlope, 5, "%04d", nSlope);
		}
		else
		{
			// 处理负数
			int nSlope(0);
			nSlope = int((fSlope - 0.05)*10.0f)*-1;
			sprintf_s(chSlope, 5, "%04d", nSlope);
			chSlope[0] =  0x30-(chSlope[0]-0x30);
			chSlope[1] =  0x30-(chSlope[1]-0x30);
			chSlope[2] =  0x30-(chSlope[2]-0x30);
			chSlope[3] =  0x30-(chSlope[3]-0x30);
		}
		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'T';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'R';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'P';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'O';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		if(fZero >= 0 )
		{
			int nZero = int((fZero + 0.05)*10.0f)%10000;
			sprintf_s(chZero, 5, "%04d", nZero);
		}
		else
		{
			// 处理负数
			int nZero(0);
			nZero = int((fZero - 0.05)*10.0f)*-1;
			sprintf_s(chZero, 5, "%04d", nZero);
			chZero[0] =  0x30-(chZero[0]-0x30);
			chZero[1] =  0x30-(chZero[1]-0x30);
			chZero[2] =  0x30-(chZero[2]-0x30);
			chZero[3] =  0x30-(chZero[3]-0x30);
		}

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'T';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nZero = int((fZero + 0.05)*10.0f)%10000;
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'R';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nZero = int((fZero + 0.05)*10.0f)%10000;
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'P';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		int nZero = int((fZero + 0.05)*10.0f)%10000;
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// 放大10倍,格式化为4位int
		//int nZero = int((fZero + 0.05)*10.0f)%10000;
		//char chZero[5];
		//ZeroMemory(chZero, sizeof(chZero));
		//sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'O';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[16];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::BackupCalibrationParameters(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'S';
		bWriteBuf[4] = 0x00;
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType)
{
	assert(NULL != pbResult);

	if (EnvironmentTemperaturePara == emParaType)
	{
		if (ExecuteRecoverCmd('E','A') && ExecuteRecoverCmd('E','B'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (RelativeHumidityPara == emParaType)
	{
		if (ExecuteRecoverCmd('E','C') && ExecuteRecoverCmd('E','D'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (AirPressurePara == emParaType)
	{
		if (ExecuteRecoverCmd('E','E') && ExecuteRecoverCmd('E','F'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (OilTemperaturePara == emParaType)
	{
		if (ExecuteRecoverCmd('E','G') && ExecuteRecoverCmd('E','H') && ExecuteRecoverCmd('E','I') && ExecuteRecoverCmd('E','J'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (AllParameters == emParaType)
	{
		if (ExecuteRecoverCmd('E','K'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else
	{
		(*pbResult) = 0x30;
		return PARA_WR_UNKNOWN_ERROR;
	}
}

DWORD CNanhuaEPCS4G::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'B';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'C';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[11];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'D';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

bool CNanhuaEPCS4G::ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = bCmd1;
		bWriteBuf[4] = bCmd2;
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						BYTE bResult = bReadBuf[14];

						if (0x31 == bReadBuf[14])
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return true;
						}
						else
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return false;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return false;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return false;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return false;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return false;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return false;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return false;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return false;
	}
}

DWORD CNanhuaEPCS4G::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);
	//assert(NULL != pfOilTemp);
	//assert(NULL != pusRpm);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

#ifdef USE_NORMAL_PARA_DATA //暂时取消置零,注意调用时检查状态是否正常By LiangQW@20161107
	// 温度
	if (NULL != pfET)
	{
		(*pfET) = 0.0f;
	}
	// 湿度
	if (NULL != pfRH)
	{
		(*pfRH) = 0.0f;
	}
	// 大气压
	if (NULL != pfAP)
	{
		(*pfAP) = 0.0f;
	}
	// 油温
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0.0f;
	}
	// 转速
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}
#endif

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送短Command
		DWORD dwNumberOfBytesToWrite = 3;
		BYTE bWriteBuf[3];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));

		bWriteBuf[0] = STX;
		bWriteBuf[1] = 0x41;
		bWriteBuf[2] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 42;
			BYTE bReadBuf[42];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 38, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[39]==bCRC_HIGH && bReadBuf[40]==bCRC_LOW)
					{
						// 温度
						BYTE bET[5];
						ZeroMemory(bET, sizeof(bET));
						bET[0] = bReadBuf[14];
						bET[1] = bReadBuf[15];
						bET[2] = bReadBuf[16];
						bET[3] = bReadBuf[17];
						if (NULL != pfET)
						{
							(*pfET) = float(atoi((char*)bET))/10.0f - 30.0f;
						}

						// 湿度
						BYTE bRH[5];
						ZeroMemory(bRH, sizeof(bRH));
						bRH[0] = bReadBuf[19];
						bRH[1] = bReadBuf[20];
						bRH[2] = bReadBuf[21];
						bRH[3] = bReadBuf[22];
						if (NULL != pfRH)
						{
							(*pfRH) = float(atoi((char*)bRH))/10.0f;
						}

						// 大气压
						BYTE bAP[5];
						ZeroMemory(bAP, sizeof(bAP));
						bAP[0] = bReadBuf[24];
						bAP[1] = bReadBuf[25];
						bAP[2] = bReadBuf[26];
						bAP[3] = bReadBuf[27];
						if (NULL != pfAP)
						{
							(*pfAP) = float(atoi((char*)bAP))/10.0f;
						}

						// 油温
						BYTE bOilTemp[5];
						ZeroMemory(bOilTemp, sizeof(bOilTemp));
						bOilTemp[0] = bReadBuf[29];
						bOilTemp[1] = bReadBuf[30];
						bOilTemp[2] = bReadBuf[31];
						bOilTemp[3] = bReadBuf[32];
						if (NULL != pfOilTemp)
						{
							(*pfOilTemp) = float(atoi((char*)bOilTemp))/10.0f - 30.0f;
						}

						// 转速
						BYTE bRpm[5];
						ZeroMemory(bRpm, sizeof(bRpm));
						bRpm[0] = bReadBuf[34];
						bRpm[1] = bReadBuf[35];
						bRpm[2] = bReadBuf[36];
						bRpm[3] = bReadBuf[37];
						if (NULL != pusRpm)
						{
							(*pusRpm) = atoi((char*)bRpm);
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 38, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[39]==bCRC_HIGH && bReadBuf[40]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::GetEnvironmentParameters(float *const pfET, 
		float *const pfRH, 
		float *const pfAP)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);
	
	// 温度
	if (NULL != pfET)
	{
		(*pfET) = 0.0f;
	}
	// 湿度
	if (NULL != pfRH)
	{
		(*pfRH) = 0.0f;
	}
	// 大气压
	if (NULL != pfAP)
	{
		(*pfAP) = 0.0f;
	}

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'L';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 32;
			BYTE bReadBuf[32];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 28, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[29]==bCRC_HIGH && bReadBuf[30]==bCRC_LOW)
					{
						// 温度
						BYTE bET[5];
						ZeroMemory(bET, sizeof(bET));
						bET[0] = bReadBuf[14];
						bET[1] = bReadBuf[15];
						bET[2] = bReadBuf[16];
						bET[3] = bReadBuf[17];
						if (NULL != pfET)
						{
							(*pfET) = float(atoi((char*)bET))/10.0f - 30.0f;
						}

						// 湿度
						BYTE bRH[5];
						ZeroMemory(bRH, sizeof(bRH));
						bRH[0] = bReadBuf[19];
						bRH[1] = bReadBuf[20];
						bRH[2] = bReadBuf[21];
						bRH[3] = bReadBuf[22];
						if (NULL != pfRH)
						{
							(*pfRH) = float(atoi((char*)bRH))/10.0f;
						}

						// 大气压
						BYTE bAP[5];
						ZeroMemory(bAP, sizeof(bAP));
						bAP[0] = bReadBuf[24];
						bAP[1] = bReadBuf[25];
						bAP[2] = bReadBuf[26];
						bAP[3] = bReadBuf[27];
						if (NULL != pfAP)
						{
							(*pfAP) = float(atoi((char*)bAP))/10.0f;
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 28, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[29]==bCRC_HIGH && bReadBuf[30]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::GetOilTempRpm(float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	//assert(NULL != pfOilTemp);
	//assert(NULL != pusRpm);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 油温
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0.0f;
	}
	// 转速
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'L';
		bWriteBuf[4] = 'B';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 27;
			BYTE bReadBuf[27];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 23, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[24]==bCRC_HIGH && bReadBuf[25]==bCRC_LOW)
					{
						// 油温
						BYTE bOilTemp[5];
						ZeroMemory(bOilTemp, sizeof(bOilTemp));
						bOilTemp[0] = bReadBuf[14];
						bOilTemp[1] = bReadBuf[15];
						bOilTemp[2] = bReadBuf[16];
						bOilTemp[3] = bReadBuf[17];
						if (NULL != pfOilTemp)
						{
							(*pfOilTemp) = float(atoi((char*)bOilTemp))/10.0f - 30.0f;
						}

						// 转速
						BYTE bRpm[5];
						ZeroMemory(bRpm, sizeof(bRpm));
						bRpm[0] = bReadBuf[19];
						bRpm[1] = bReadBuf[20];
						bRpm[2] = bReadBuf[21];
						bRpm[3] = bReadBuf[22];
						if (NULL != pusRpm)
						{
							(*pusRpm) = atoi((char*)bRpm);
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 23, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[24]==bCRC_HIGH && bReadBuf[25]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::DataTrans(BYTE *const pbResult,const BYTE bCom[], const BYTE * pbDataBuf, const UINT nDatalen)
{
	assert(NULL != pbResult);
	assert(NULL != pbDataBuf);

	if (nDatalen > 99989)
	{
		return PARA_WR_UNKNOWN_ERROR;
	}

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 25+nDatalen;
		BYTE* bWriteBuf = new BYTE[dwNumberOfBytesToWrite]();

		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		char chLen[6];
		ZeroMemory(chLen, sizeof(chLen));
		sprintf_s(chLen, "%05d", nDatalen+10);

		char chTimeNow[9];
		ZeroMemory(chTimeNow, sizeof(chTimeNow));
		SYSTEMTIME stUTC;  
        ::GetSystemTime(&stUTC); 
		sprintf_s(chTimeNow,"%02d%02d%02d%02d", stUTC.wDay,stUTC.wHour+8, stUTC.wMinute, stUTC.wSecond);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = bCom[0];
		bWriteBuf[4] = bCom[1];
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = chLen[0];
		bWriteBuf[7] = chLen[1];
		bWriteBuf[8] = chLen[2];
		bWriteBuf[9] = chLen[3];
		bWriteBuf[10] = chLen[4];
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chTimeNow[0];
		bWriteBuf[13] = chTimeNow[1];
		bWriteBuf[14] = chTimeNow[2];
		bWriteBuf[15] = chTimeNow[3];
		bWriteBuf[16] = chTimeNow[4];
		bWriteBuf[17] = chTimeNow[5];
		bWriteBuf[18] = chTimeNow[6];
		bWriteBuf[19] = chTimeNow[7];
		bWriteBuf[20] = ETB;

		for (int i=0; i<(int)nDatalen; i++)
		{
			bWriteBuf[21+i] = pbDataBuf[i];
		}

		bWriteBuf[21+nDatalen] = ETB;
		CheckSum(bWriteBuf, 1, (21+nDatalen), &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[22+nDatalen] = bCRC_HIGH;
		bWriteBuf[23+nDatalen] = bCRC_LOW;
		bWriteBuf[24+nDatalen] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 释放bWriteBuf
			if (bWriteBuf!=NULL)
			{
				delete[] bWriteBuf;
				bWriteBuf = NULL;
			}

			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			// 释放bWriteBuf
			if (bWriteBuf!=NULL)
			{
				delete[] bWriteBuf;
				bWriteBuf = NULL;
			}

			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::EnterDTMode(BYTE *const pbResult)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'G';
		bWriteBuf[4] = 'B';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::TransData(BYTE *const pbResult, const BYTE * pbDataBuf, const UINT nDatalen)
{
	assert(NULL != pbResult);
	assert(NULL != pbDataBuf);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 16+nDatalen;
		BYTE* bWriteBuf = new BYTE[dwNumberOfBytesToWrite]();

		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		char chLen[6];
		ZeroMemory(chLen, sizeof(chLen));
		sprintf_s(chLen, "%05d", nDatalen+1);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'G';
		bWriteBuf[4] = 'S';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = chLen[0];
		bWriteBuf[7] = chLen[1];
		bWriteBuf[8] = chLen[2];
		bWriteBuf[9] = chLen[3];
		bWriteBuf[10] = chLen[4];
		bWriteBuf[11] = ETB;

		for (int i=0; i<(int)nDatalen; i++)
		{
			bWriteBuf[12+i] = pbDataBuf[i];
		}

		bWriteBuf[12+nDatalen] = ETB;
		CheckSum(bWriteBuf, 1, (12+nDatalen), &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[13+nDatalen] = bCRC_HIGH;
		bWriteBuf[14+nDatalen] = bCRC_LOW;
		bWriteBuf[15+nDatalen] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 释放bWriteBuf
			if (bWriteBuf!=NULL)
			{
				delete[] bWriteBuf;
				bWriteBuf = NULL;
			}

			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			// 释放bWriteBuf
			if (bWriteBuf!=NULL)
			{
				delete[] bWriteBuf;
				bWriteBuf = NULL;
			}

			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::ExitDTMode(BYTE *const pbResult)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'G';
		bWriteBuf[4] = 'E';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::GetIMEI_ICCID(char* chIMEI, char* chICCID)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'G';
		bWriteBuf[4] = 'I';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 54;
			BYTE bReadBuf[54];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 50, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[51]==bCRC_HIGH && bReadBuf[52]==bCRC_LOW)
					{
						// IMEI
						for(int i=0; i<15; i++)
						{
							chIMEI[i] = bReadBuf[14+i];
						}
						
						// ICCID
						for(int i=0; i<20; i++)
						{
							chICCID[i] = bReadBuf[30+i];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 50, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[51]==bCRC_HIGH && bReadBuf[52]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS4G::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
	assert(NULL != pbDataBuf);

	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	// 判断端口是否打开
	if(IsOpen())
	{
		// 发送Command
		DWORD dwNumberOfBytesToWrite_GB = 15;
		BYTE bWriteBuf_GB[15];
		ZeroMemory(bWriteBuf_GB, sizeof(bWriteBuf_GB));
		BYTE bCRC_HIGH_GB(0x00),bCRC_LOW_GB(0x00);

		bWriteBuf_GB[0] = STX;
		bWriteBuf_GB[1] = ADD;
		bWriteBuf_GB[2] = ETB;
		bWriteBuf_GB[3] = 'G';
		bWriteBuf_GB[4] = 'B';
		bWriteBuf_GB[5] = ETB;
		bWriteBuf_GB[6] = 0x30;
		bWriteBuf_GB[7] = 0x30;
		bWriteBuf_GB[8] = 0x30;
		bWriteBuf_GB[9] = 0x30;
		bWriteBuf_GB[10] = 0x30;
		bWriteBuf_GB[11] = ETB;
		CheckSum(bWriteBuf_GB, 1, 11, &bCRC_HIGH_GB, &bCRC_LOW_GB);
		bWriteBuf_GB[12] = bCRC_HIGH_GB;
		bWriteBuf_GB[13] = bCRC_LOW_GB;
		bWriteBuf_GB[14] = ETX;

		if (WriteCommBuffer(bWriteBuf_GB, dwNumberOfBytesToWrite_GB))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (bReadBuf[14] == '0')
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}

		// 发送Command
		DWORD dwNumberOfBytesToWrite_GS = 16+nDatalen;
		BYTE* bWriteBuf_GS = new BYTE[dwNumberOfBytesToWrite_GS]();

		BYTE bCRC_HIGH_GS(0x00),bCRC_LOW_GS(0x00);

		char chLen[6];
		ZeroMemory(chLen, sizeof(chLen));
		sprintf_s(chLen, "%05d", nDatalen+1);

		bWriteBuf_GS[0] = STX;
		bWriteBuf_GS[1] = ADD;
		bWriteBuf_GS[2] = ETB;
		bWriteBuf_GS[3] = 'G';
		bWriteBuf_GS[4] = 'S';
		bWriteBuf_GS[5] = ETB;
		bWriteBuf_GS[6] = chLen[0];
		bWriteBuf_GS[7] = chLen[1];
		bWriteBuf_GS[8] = chLen[2];
		bWriteBuf_GS[9] = chLen[3];
		bWriteBuf_GS[10] = chLen[4];
		bWriteBuf_GS[11] = ETB;

		for (int i=0; i<(int)nDatalen; i++)
		{
			bWriteBuf_GS[12+i] = pbDataBuf[i];
		}

		bWriteBuf_GS[12+nDatalen] = ETB;
		CheckSum(bWriteBuf_GS, 1, (12+nDatalen), &bCRC_HIGH_GS, &bCRC_LOW_GS);
		bWriteBuf_GS[13+nDatalen] = bCRC_HIGH_GS;
		bWriteBuf_GS[14+nDatalen] = bCRC_LOW_GS;
		bWriteBuf_GS[15+nDatalen] = ETX;

		if (WriteCommBuffer(bWriteBuf_GS, dwNumberOfBytesToWrite_GS))
		{
			// 释放bWriteBuf
			if (bWriteBuf_GS!=NULL)
			{
				delete[] bWriteBuf_GS;
				bWriteBuf_GS = NULL;
			}

			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (bReadBuf[14] == '0')
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							//return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						//return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						//return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						//return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					//return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				//return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			// 释放bWriteBuf
			if (bWriteBuf_GS!=NULL)
			{
				delete[] bWriteBuf_GS;
				bWriteBuf_GS = NULL;
			}

			::LeaveCriticalSection(&m_sCriticalSection);
			//return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}

		// 发送Command
		DWORD dwNumberOfBytesToWrite_GE = 15;
		BYTE bWriteBuf_GE[15];
		ZeroMemory(bWriteBuf_GE, sizeof(bWriteBuf_GE));
		BYTE bCRC_HIGH_GE(0x00),bCRC_LOW_GE(0x00);

		bWriteBuf_GE[0] = STX;
		bWriteBuf_GE[1] = ADD;
		bWriteBuf_GE[2] = ETB;
		bWriteBuf_GE[3] = 'G';
		bWriteBuf_GE[4] = 'E';
		bWriteBuf_GE[5] = ETB;
		bWriteBuf_GE[6] = 0x30;
		bWriteBuf_GE[7] = 0x30;
		bWriteBuf_GE[8] = 0x30;
		bWriteBuf_GE[9] = 0x30;
		bWriteBuf_GE[10] = 0x30;
		bWriteBuf_GE[11] = ETB;
		CheckSum(bWriteBuf_GE, 1, 11, &bCRC_HIGH_GE, &bCRC_LOW_GE);
		bWriteBuf_GE[12] = bCRC_HIGH_GE;
		bWriteBuf_GE[13] = bCRC_LOW_GE;
		bWriteBuf_GE[14] = ETX;

		if (WriteCommBuffer(bWriteBuf_GE, dwNumberOfBytesToWrite_GE))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if(bReadBuf[14]=='1')
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return PARA_WR_SUCCEED;
						}
						else
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}