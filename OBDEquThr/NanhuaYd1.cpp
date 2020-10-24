#include "StdAfx.h"
#include "NanhuaYd1.h"

#include <cassert>
#include <cstdio>
#include <string>

CNanhuaYd1::CNanhuaYd1(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}

CNanhuaYd1::~CNanhuaYd1(void)
{
	::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CNanhuaYd1::Open(BYTE bPort)
{
	assert(0 != bPort);

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
		OutputDebugStringW(L"Error:fail to open com port\n");
		return SMO_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数begin
	// 设置输入和输出缓存区大小
	SetupComm(m_hComm, 512, 256);
	// 设置DCB结构
	DCB dcb;
	// 获取缺省的DCB结构的值
	GetCommState(m_hComm, &dcb);
	// 设定波特率为2400 bps
	//dcb.BaudRate = CBR_2400;
	dcb.BaudRate = CBR_2400; //change for qin
	// 设定无奇偶校验
	dcb.Parity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;
	//dcb.StopBits = TWOSTOPBITS;//change for qin 
	// 设置串行设备控制参数
	SetCommState(m_hComm, &dcb);
	// 进行超时设置（700毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 1000;
	CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 700;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	return SMO_OPEN_SUCCEED;
}

bool CNanhuaYd1::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

// 关闭串口
DWORD CNanhuaYd1::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			return SMO_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return SMO_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return SMO_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

DWORD CNanhuaYd1::WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const DWORD dwSleepBeforeRead)
{
	assert(NULL!=pbWriteBuffer && 0UL<dwWriteLength);
	assert(NULL!=pbReadBuffer && 0UL<dwReadLength);
	assert(dwSleepBeforeRead < 1000UL);

	::EnterCriticalSection(&cInfoSection);

	if(IsOpen())
	{

		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		if(dwSleepBeforeRead>0)
		{
			Sleep(dwSleepBeforeRead);
		}
		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			// 判断非法或错误指令返回
			if(0x15 != pbReadBuffer[0])
			{
				::LeaveCriticalSection(&cInfoSection);
				return 7UL;
			}
			else
			{
				::LeaveCriticalSection(&cInfoSection);
				// 错误：仪器返回错误状态码
				return 6UL;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// 错误：读第一个字节数据失败
			return 3UL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// 错误：串口没有打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

BYTE CNanhuaYd1::CheckSum(BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	bSum = ~bSum + 1;
	return bSum;                

}

DWORD CNanhuaYd1::AutoAdj(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9B);
	BYTE bReadBuf(0);
	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			if (0x06 == bReadBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bReadBuf)
			{
				*pbResult = 0x15;
			}

			else
			{
                 *pbResult = 0x06;
			}
			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// 函数执行错误：读指定数据错误
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}

}

DWORD CNanhuaYd1::Reset(BYTE *const pbResult)
{
	//assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9A);
	BYTE bReadBuf(0);

	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			if (NULL != pbResult)
			{
				*pbResult = bReadBuf;
			}

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			if (NULL != pbResult)
			{
				*pbResult = 0xFF;
			}
			// 函数执行错误：读指定数据错误
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			*pbResult = 0xFF;
		}
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::StartAutoTest(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x92);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		// 清缓冲区
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);	
		if (bSucceed && 1==dwBytesRead)
		{
			if (0x06 == bReadBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bReadBuf)
			{
				*pbResult = 0x15;
			}
			else
			{
				// 理论上不可能执行到这里
			}

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// 函数执行错误：读指定数据错误
			return 2UL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}
DWORD CNanhuaYd1::AutoReadData(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	BYTE bWriteBuf(0x9c);

	if(IsOpen())
	{		
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		bSucceed = WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(500);
		if (bSucceed && 1==dwBytesWritten)
		{
			if (0x06 == bWriteBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bWriteBuf)
			{
				*pbResult = 0x15;
			}
			else
			{
				if(0x94 == bWriteBuf)
				{
					*pbResult = 0x94; //检测完成
				}
			}

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// 函数执行错误：读指定数据错误
			return 2UL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::GetData1(BYTE *const pbResult, float *const pfData1)
{
	assert(NULL != pbResult);
	assert(NULL != pfData1);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x96);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		//UINT unReadCount(0);
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(500);
		do
		{
			bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
			if (bSucceed && 1==dwBytesRead)
			{
				//ReadBuf[ReadCount] = bReadBuf;
				//unReadCount += dwBytesRead;	
				str.push_back(bReadBuf);
			}
			else
			{
				// 函数执行读取指定数据失败
				*pbResult = 0xFF;
				return SMO_WR_READ_SPECIFIED_DATA_FAIL;
			}
		} while(bReadBuf!=0xFF);

		//str = str.substr(1, 4);
		if(str.length() >= 4)
		{
			*pfData1 = (float)atof(str.substr(1, 4).c_str());
		}
		else
		{
			*pfData1 = 0.0f;
		}

		*pbResult = 0x00;
		return 0x00;
	}
	else
	{
		// 函数执行错误：串口未打开
		*pbResult = 0xFF;
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4)
{
	assert(NULL != pbResult);
	assert(NULL != pfData1);
	assert(NULL != pfData2);
	assert(NULL != pfData3);
	assert(NULL != pfData4);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x97);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		//int ReadCount = 0;
		//BOOL nResult = 0; 
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf,1, &dwBytesWritten, NULL);
		Sleep(500);
		do
		{
			bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
			if (bSucceed && 1==dwBytesRead)
			{
				//ReadBuf[ReadCount] = bReadBuf;
				//unReadCount += dwBytesRead;	
				str.push_back(bReadBuf);
			}
			else
			{
				// 函数执行读取指定数据失败
				*pbResult = 0xFF;
				*pfData1 = 0.0f;
				*pfData2 = 0.0f;
				*pfData3 = 0.0f;
				*pfData4 = 0.0f;
				return SMO_WR_READ_SPECIFIED_DATA_FAIL;
			}
		} while(bReadBuf!=0xFF);

		if(str.length() >= 16)
		{
			*pfData1 = (float)atof(str.substr(1, 4).c_str());
			*pfData2 = (float)atof(str.substr(5, 4).c_str());
			*pfData3 = (float)atof(str.substr(9, 4).c_str());
			*pfData4 = (float)atof(str.substr(13, 4).c_str());
		}
		else
		{
			*pfData1 = 0.0f;
			*pfData2 = 0.0f;
			*pfData3 = 0.0f;
			*pfData4 = 0.0f;
		}

		*pbResult = 0x00;
		return 0x00;

	}
	else
	{
		*pbResult = 0xFF;
		*pfData1 = 0.0f;
		*pfData2 = 0.0f;
		*pfData3 = 0.0f;
		*pfData4 = 0.0f;

		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::GetTestState(BYTE *const pbResult, BYTE *const pbState)
{
	assert(NULL != pbResult);
	assert(NULL != pbState);

	//DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	//BYTE bWriteBuf(0);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		//unsigned char ReadChar;
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1== dwBytesRead)
		{
			if(bReadBuf == 0x93)
			{
				//第一次检测完成
				*pbState = 0x93;
			}
			else if(bReadBuf == 0x95)
			{
				//第二、三次检测完成
				*pbState = 0x95;
			}		
			else if(bReadBuf == 0x94)
			{
				//第四次检测完成
				*pbState = 0x94;
			}
			else
			{
				// 其它数据
				*pbState = 0xFF;
			}

			*pbResult = 0x00;
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			*pbState = 0xFF;
			// 函数执行读取指定数据失败
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		*pbState = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::StartRealTest(BYTE *const pbResult)
{
	//assert(NULL != pbResult);

	BYTE bWriteBuf(0x9e);
	DWORD dwBytesWritten(0);
	BYTE bReadBuf(0);
	DWORD dwBytesRead(0);

	if(IsOpen())
	{
		BOOL bSucceed(FALSE);
		std::string str("");

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			//if (0x06 == bReadBuf)
			//{
			//	*pbResult = 0x06;
			//}
			//else if (0x15 == bReadBuf)
			//{
			//	*pbResult = 0x15;
			//}
			//else
			//{
			//	// 理论上不可能执行到这里
			//}
			if (NULL != pbResult)
			{
				*pbResult = bReadBuf;
			}

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			if (NULL != pbResult)
			{
				*pbResult = 0xFF;
			}
			// 函数执行读取指定数据失败
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			*pbResult = 0xFF;
		}
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::AllowTest(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	BYTE bWriteBuf(0x90);
	DWORD dwBytesWritten(0);
	BYTE bReadBuf(0);
	DWORD dwBytesRead(0);

	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			//if (0x06 == bReadBuf)
			//{
			//	*pbResult = 0x06;
			//}
			//else if (0x15 == bReadBuf)
			//{
			//	*pbResult = 0x15;
			//}
			//else
			//{
			//	// 理论上不可能执行到这里
			//}
			*pbResult = bReadBuf;

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// 函数执行读取指定数据失败
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::GetSign(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	//DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	//BYTE bWriteBuf(0);
	BYTE bReadBuf(0);

	if(IsOpen())
	{
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			//if (bReadBuf == 0x94)
			//{
			//	bReadBuf = 0x94;
			//}
			//else if(bReadBuf == 0x9F)
			//{
			//	bReadBuf = 0x9F;
			//}
			//else
			//{
			//	bReadBuf = 0xFF;
			//}
			*pbResult = bReadBuf;

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// 函数执行读取指定数据失败
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}
DWORD CNanhuaYd1::QuitAutoTest(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9d);
	BYTE bReadBuf(0);

	if(IsOpen())
	{		
		// 清缓冲区
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);	
		if (bSucceed && 1==dwBytesRead)
		{
			if (0x06 == bReadBuf)
			{
				*pbResult = 0x06;
			}
			else if (0x15 == bReadBuf)
			{
				*pbResult = 0x15;
			}
			else
			{
				// 理论上不可能执行到这里
			}

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			*pbResult = 0xFF;
			// 函数执行错误：读指定数据错误
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		*pbResult = 0xFF;
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaYd1::HardReset(BYTE *const pbResult)
{
	//assert(NULL != pbResult);

	DWORD dwBytesWritten(0);
	DWORD dwBytesRead(0);
	BYTE bWriteBuf(0x9D);
	BYTE bReadBuf(0);

	if (IsOpen())
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		WriteFile(m_hComm, &bWriteBuf, 1, &dwBytesWritten, NULL);
		Sleep(200);
		BOOL bSucceed = ReadFile(m_hComm, &bReadBuf, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
			if (NULL != pbResult)
			{
				*pbResult = bReadBuf;
			}

			// 函数执行成功
			return SMO_WR_SUCCEED;
		}
		else
		{
			if (NULL != pbResult)
			{
				*pbResult = 0xFF;
			}
			// 函数执行错误：读指定数据错误
			return SMO_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			*pbResult = 0xFF;
		}
		// 函数执行错误：串口未打开
		return SMO_WR_PORT_NOT_OPEN;
	}
}