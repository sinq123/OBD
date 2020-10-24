#include "StdAfx.h"
#include "FuLiFLV1000.h"

#include <cassert>
#include <cstdio>

CFuLiFLV1000::CFuLiFLV1000(void)
: m_hComm(INVALID_HANDLE_VALUE)
//, m_opUsedNow(ToAnalyzer)
{
	::InitializeCriticalSection(&cInfoSection);
}

CFuLiFLV1000::~CFuLiFLV1000(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CFuLiFLV1000::Open(BYTE bPort)
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
		OutputDebugStringW(L"Error:fail to open com port\n");
		return ANA_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数begin
	// 设置输入和输出缓存区大小
	SetupComm(m_hComm, 512, 256);
	// 设置DCB结构
	DCB dcb;
	// 获取缺省的DCB结构的值
	GetCommState(m_hComm, &dcb);
	// 设定波特率为9600 bps
	dcb.BaudRate = CBR_9600;
	// 设定无奇偶校验
	dcb.Parity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;
	// 设置串行设备控制参数
	SetCommState(m_hComm, &dcb);
	// 进行超时设置（2500毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	return ANA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CFuLiFLV1000::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			return ANA_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return ANA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return ANA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CFuLiFLV1000::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CFuLiFLV1000::WriteAndReadData(BYTE *const pbWriteBuffer, 
								const DWORD dwWriteLength, 
								BYTE *const pbReadBuffer, 
								const DWORD dwReadLength, 
								const bool bNeedWriteChkSum, 
								const bool bNeedReadChkSum, 
								const bool bPurgeComm)
{
	//assert(NULL!=pbWriteBuffer);
	//assert(0UL!=dwWriteLength);
	assert(NULL!=pbReadBuffer);
	assert(0UL!=dwReadLength);

	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		// 判断是否重置串口操作
		if (bPurgeComm)
		{
			PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		}

		// 判断是否需要进行写数据操作
		if (NULL != pbWriteBuffer)
		{
			// 需要进行写入数据操作

			// 判断写入数据是否需要计算校验
			if (dwWriteLength>1 && bNeedWriteChkSum)
			{
				pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
			}
			WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		}
		
		// 一次读完预计要接收的字节数

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			if (dwReadLength>1 && bNeedReadChkSum)
			{
				// 验证校验和
				if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer, dwReadLength-1))
				{
					::LeaveCriticalSection(&cInfoSection);

					// 函数错误：校验读出数据失败
					return ANA_WR_CHECK_SUM_FAIL;
				}
				else
				{
					::LeaveCriticalSection(&cInfoSection);

					// 函数成功
					return ANA_WR_SUCCEED;
				}
			}
			else
			{
				::LeaveCriticalSection(&cInfoSection);

				// 函数成功
				return ANA_WR_SUCCEED;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);

			// 函数错误：读指定数据失败
			return ANA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);

		// 函数错误：串口没有打开
		return ANA_WR_PORT_NOT_OPEN;
	}
}

BYTE CFuLiFLV1000::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	// 校验不进行取反加一
	bSum = ~bSum + 1;
	return bSum;
}

DWORD CFuLiFLV1000::WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
												 const DWORD dwWriteLength, 
												 BYTE *const pbReadBuffer, 
												 const DWORD dwReadLength, 
												 const bool bNeedWriteChkSum, 
												 const bool bNeedReadChkSum)
{
	assert(NULL!=pbWriteBuffer);
	assert(1UL<dwWriteLength);
	assert(NULL!=pbReadBuffer);
	assert(1UL<dwReadLength);

	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		// 重置并清空串口
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			// 计算写校验
			WORD wSum = CheckSum_Flowmeter(pbWriteBuffer, dwWriteLength-2);
			pbWriteBuffer[dwWriteLength-2] = LOBYTE(wSum);
			pbWriteBuffer[dwWriteLength-1] = HIBYTE(wSum);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// 分二次读数
		// 第一次数据头（4byte）
		// 第二次读剩下的有效数据(有效数据根据第一次读回来的数据不同而不同)

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1==dwBytesRead)
		{
#ifdef _DEBUG
			SYSTEMTIME st;
			GetSystemTime(&st);
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"流量计返回:%d\r\n",pbReadBuffer[1]);
			OutputDebugString(wchOutPutDebugString);
#endif	
			// 根据F版通讯协议判断Reponse mode:
			// ACK  (0x06): command received OK and completely issued
			// BUSY (0x05): command received OK but process is still pending. Use process status command to determine when command is done
			// NAK  (0x15): transmission error or command error
			if (0x06 == pbReadBuffer[0])
			{
				// 流量计命令已经接受，并且已经完成

				// 读余下的数据
				bSucceed = ReadFile(m_hComm, pbReadBuffer+1, dwReadLength-1, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-1))
				{
					if (bNeedReadChkSum)
					{
						// 需要读校验
						WORD wSum = CheckSum_Flowmeter(pbReadBuffer, dwReadLength-2);
						if (wSum != MAKEWORD(pbReadBuffer[dwReadLength-2], pbReadBuffer[dwReadLength-1]))
						{
							::LeaveCriticalSection(&cInfoSection);

							// 函数错误：校验读出数据失败
							return ANA_WR_CHECK_SUM_FAIL;
						}
						else
						{
							::LeaveCriticalSection(&cInfoSection);

							// 函数成功
							return ANA_WR_SUCCEED;
						}
					}
					else
					{
						::LeaveCriticalSection(&cInfoSection);

						// 函数成功
						return ANA_WR_SUCCEED;
					}
				}
				else
				{
					::LeaveCriticalSection(&cInfoSection);

					// 函数错误：读指定数据失败（读余下有效数据失败）
					return ANA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else if (0x15 == pbReadBuffer[0])
			{
				// 流量计命令已经接受，但还在执行
				// 清除后续无用数据
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);

				// 函数成功
				return ANA_WR_SUCCEED;
			}
			else
			{
				// 流量计命令错误
				// 清除后续无用数据
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);

				// 函数错误：仪器返回错误码
				return ANA_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);

			// 函数错误：读指定数据失败（读数据头失败）
			return ANA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);

		// 函数错误：串口没有打开
		return ANA_WR_PORT_NOT_OPEN;
	}
}

WORD CFuLiFLV1000::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);

	WORD wSum(0);
	for (UINT i=0; i<un; i++)
	{
		wSum += pb[i];
	}
	return wSum;
}

DWORD CFuLiFLV1000::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE bWriteBuf = bCmd;
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (NULL != pbResult)
	{
		// 获取仪器执行结果

		if (ANA_WR_SUCCEED == dw)
		{
			// 仪器返回结果,一般返回0x06
			*pbResult = bReadBuf;
		}
	}
	return dw;
}

DWORD CFuLiFLV1000::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE bWriteBuf = bCmd;
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (NULL != pbResult)
	{
		// 获取仪器执行结果

		if (ANA_WR_SUCCEED == dw)
		{
			*pbResult = bReadBuf;
		}
	}
	return dw;
}

DWORD CFuLiFLV1000::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
{
	assert(NULL != pbResult);

	// 分2步骤读,第1步读命令头,第2步根据情况读操作过程数据

	// 第1步:读命令头
	DWORD dw = CmdFormat_Return00_01_05(bCmd, pbResult);
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x05 == *pbResult)
		{
			// 仪器返回结果:正在进行操作,要继续读后面的操作过程数据

			// 第2步:读操作过程数据(不是每次都要执行,仅当返回05命令头时才需要执行)
			BYTE bReadBuf[14];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			dw = WriteAndReadData(NULL, 0, bReadBuf+1, 13, false, false, false);
			if (ANA_WR_SUCCEED == dw)
			{
				// (1)HC
				if (NULL != psHC)
				{
					// 可以为正或者负数,添加short强制转换
					*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
				}
				// (2)CO
				if (NULL != pfCO)
				{
					// 可以为正或者负数,添加short强制转换
					*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
				}
				// (3)CO2
				if (NULL != pfCO2)
				{
					// 可以为正或者负数,添加short强制转换
					*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
				}
				// (4)O2
				if (NULL != pfO2)
				{
					// 可以为正或者负数,添加short强制转换
					*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
				}
				// (5)NO
				if (NULL != psNO)
				{
					// 可以为正或者负数,添加short强制转换
					*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
				}
				// (6)Pef
				if (NULL != pfPef)
				{
					// 可以为正或者负数,添加short强制转换
					*pfPef = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11])) / 1000.0f;
				}
			}
		}
	}

	return dw;
}

DWORD CFuLiFLV1000::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	assert(NULL != pbResult);
	assert(usC3H8>500 && usC3H8<60000);
	assert(fCO>10e-6f && fCO<60000);
	assert(fCO2>10e-6f && fCO2<60000);
	assert(usNO>500 && usNO<60000);

	BYTE bWriteBuf[9];
	BYTE bReadBuf(0);
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));

	// 分2步骤执行

	// 第1步骤,写入命令头
	DWORD dw = CmdFormat_OnlyReturn06(bCmd, pbResult);
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 != *pbResult)
		{
			// 仪器返回错误信息
			return dw;
		}
	}
	else
	{
		// 第1步骤,函数发生错误
		return dw;
	}

	// 第2步骤,写入气体浓度数据
	bWriteBuf[0] = HIBYTE(usC3H8);
	bWriteBuf[1] = LOBYTE(usC3H8);
	bWriteBuf[2] = HIBYTE((WORD)(fCO * 100.0f));
	bWriteBuf[3] = LOBYTE((WORD)(fCO * 100.0f));
	bWriteBuf[4] = HIBYTE((WORD)(fCO2 * 100.0f));
	bWriteBuf[5] = LOBYTE((WORD)(fCO2 * 100.0f));
	bWriteBuf[6] = HIBYTE(usNO);
	bWriteBuf[7] = LOBYTE(usNO);
	bWriteBuf[8] = 0;
	dw = WriteAndReadData(bWriteBuf, 9, &bReadBuf, 1, true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// 仪器返回结果
		*pbResult = bReadBuf;
	}

	return dw;
}

DWORD CFuLiFLV1000::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	assert(NULL != pbResult);
	//assert(fValue>-20.0f && fValue<150.0f);

	// 503分2步骤执行

	// 第1步骤,写入命令头
	DWORD dw = CmdFormat_OnlyReturn06(bCmd, pbResult);
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 != *pbResult)
		{
			// 仪器返回错误信息
			return dw;
		}
	}
	else
	{
		// 第1步骤,函数发生错误
		return dw;
	}

	// 第2步骤,写入气体浓度数据
	BYTE bReadBuf(0);
	BYTE bWriteBuf[2];
	bWriteBuf[0] = HIBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[1] = LOBYTE((WORD)(fValue * 10.0f));
	dw = WriteAndReadData(bWriteBuf, 2, &bReadBuf, 1, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// 仪器返回结果
		*pbResult = bReadBuf;
	}

	return dw;
}

DWORD CFuLiFLV1000::CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	assert(NULL != pbResult);
	assert(usC3H8 >= 0);
	assert(fCO >= 0.0f);
	assert(fCO2 >= 0.0f);
	assert(usNO >= 0);

	BYTE bReadBuf(0);
	BYTE bWriteBuf[10];
	bWriteBuf[0] = bCmd;
	bWriteBuf[1] = HIBYTE(usC3H8);
	bWriteBuf[2] = LOBYTE(usC3H8);
	bWriteBuf[3] = HIBYTE((WORD)(fCO * 100.0f + 0.5f));
	bWriteBuf[4] = LOBYTE((WORD)(fCO * 100.0f + 0.5f));
	bWriteBuf[5] = HIBYTE((WORD)(fCO2 * 100.0f + 0.5f));
	bWriteBuf[6] = LOBYTE((WORD)(fCO2 * 100.0f + 0.5f));
	bWriteBuf[7] = HIBYTE(usNO);
	bWriteBuf[8] = LOBYTE(usNO);
	bWriteBuf[9] = 0;
	DWORD dw = WriteAndReadData(bWriteBuf, 10, &bReadBuf, 1, true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// 仪器返回结果
		*pbResult = bReadBuf;
	}

	return dw;
}

DWORD CFuLiFLV1000::CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	assert(NULL != pbResult);

	BYTE bReadBuf(0);
	BYTE bWriteBuf[4];
	bWriteBuf[0] = bCmd;
	bWriteBuf[1] = HIBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[2] = LOBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[3] = 0;
	DWORD dw = WriteAndReadData(bWriteBuf, 4, &bReadBuf, 1, true, false, true);
	if (fValue == dw)
	{
		// 仪器返回结果
		*pbResult = bReadBuf;
	}

	return dw;
}

// 1取主数据
DWORD CFuLiFLV1000::GetMajorData(short *const psHC, 
							float *const pfCO, 
							float *const pfCO2, 
							float *const pfO2, 
							short *const psNO,
							short *const psNO2,
							short *const psNOx,
							USHORT *const pusRpm, 
							float *const pfOilTemp, 
							USHORT *const pusFlux, 
							float *const pfLambda,
							bool *const pbO2Sensor,
							bool *const pbNOSensor)
{
	return GetMajorData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx, pusRpm, pfOilTemp, pusFlux, pfLambda, pbO2Sensor, pbNOSensor, NULL);
}

DWORD CFuLiFLV1000::GetMajorData(short *const psHC, 
							float *const pfCO, 
							float *const pfCO2, 
							float *const pfO2, 
							short *const psNO,
							short *const psNO2,
							short *const psNOx,
							USHORT *const pusRpm, 
							float *const pfOilTemp, 
							USHORT *const pusFlux, 
							float *const pfLambda,
							bool *const pbO2Sensor,
							bool *const pbNOSensor,
							bool *const pbWaterFull)
{
	BYTE bWriteBuf(1), bReadBuf[21];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 21, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) HC
		if (NULL != psHC)
		{
			// 可以为正或者负数,添加short强制转换
			*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// 可以为正或者负数,添加short强制转换
			*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			// 可以为正或者负数,添加short强制转换
			*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			// 可以为正或者负数,添加short强制转换
			*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			// 可以为正或者负数,添加short强制转换
			*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		if (NULL != psNO2)
		{
			// 可以为正或者负数,添加short强制转换
			*psNO2 = 0;
		}
		// (5) NOx
		if (NULL != psNOx)
		{
			// 可以为正或者负数,添加short强制转换
			*psNOx = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[12], bReadBuf[11]);
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			// 可以为正或者负数,添加short强制转换
			*pfOilTemp = (short)MAKEWORD(bReadBuf[14], bReadBuf[13]) / 10.0f;
		}
		// (8) Flux
		if (NULL != pusFlux)
		{
			*pusFlux = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		}
		// (9) Lambda
		if (NULL != pfLambda)
		{
			// 可以为正或者负数,添加short强制转换
			*pfLambda = (short)(MAKEWORD(bReadBuf[19], bReadBuf[18])) / 100.0f;
		}
		// (10)氧传感器是否老化
		if (NULL != pbO2Sensor)
		{
			if (0xFF == (bReadBuf[17]|0xFE))
			{
				*pbO2Sensor = true;
			}
			else
			{
				*pbO2Sensor = false;
			}
		}
		// (11)NO传感器是否老化
		if (NULL != pbNOSensor)
		{
			if (0xFF == (bReadBuf[17]|0xFD))
			{
				*pbNOSensor = true;
			}
			else
			{
				*pbNOSensor = false;
			}
		}

		// 设置取值范围
#ifdef ANALYZER_DATA_RANGE
		// HC
		if (*psHC < ANALYZER_HC_LOWER_LIMIT)
		{
			*psHC = ANALYZER_HC_LOWER_LIMIT;
		}
		else if (*psHC > ANALYZER_HC_UPPER_LIMIT)
		{
			*psHC = ANALYZER_HC_UPPER_LIMIT;
		}
		// CO
		if (*pfCO < ANALYZER_CO_LOWER_LIMIT)
		{
			*pfCO = ANALYZER_CO_LOWER_LIMIT;
		}
		else if (*pfCO > ANALYZER_CO_UPPER_LIMIT)
		{
			*pfCO = ANALYZER_CO_UPPER_LIMIT;
		}
		// CO2
		if (*pfCO2 < ANALYZER_CO2_LOWER_LIMIT)
		{
			*pfCO2 = ANALYZER_CO2_LOWER_LIMIT;
		}
		else if (*pfCO2 > ANALYZER_CO2_UPPER_LIMIT)
		{
			*pfCO2 = ANALYZER_CO2_UPPER_LIMIT;
		}
		 // O2
		if (*pfO2 < ANALYZER_O2_LOWER_LIMIT)
		{
			*pfO2 = ANALYZER_O2_LOWER_LIMIT;
		}
		else if (*pfO2 > ANALYZER_O2_UPPER_LIMIT)
		{
			*pfO2 = ANALYZER_O2_UPPER_LIMIT;
		}
		 // NO
		if (*psNO < ANALYZER_NO_LOWER_LIMIT)
		{
			*psNO = ANALYZER_NO_LOWER_LIMIT;
		}
		else if (*psNO > ANALYZER_NO_UPPER_LIMIT)
		{
			*psNO = ANALYZER_NO_UPPER_LIMIT;
		}
		 // RPM
		if (*pusRpm > ANALYZER_RPM_UPPER_LIMIT)
		{
			*pusRpm = ANALYZER_RPM_UPPER_LIMIT;
		}
		 // OilTemp
		if (*pfOilTemp < ANALYZER_OILTEMP_LOWER_LIMIT)
		{
			*pfOilTemp = ANALYZER_OILTEMP_LOWER_LIMIT;
		}
		else if (*pfOilTemp > ANALYZER_OILTEMP_UPPER_LIMIT)
		{
			*pfOilTemp = ANALYZER_OILTEMP_UPPER_LIMIT;
		}
		 // Flux
		if (*pusFlux > ANALYZER_FLUX_LOWER_LIMIT)
		{
			*pusFlux = ANALYZER_FLUX_UPPER_LIMIT;
		}
		 // Lambda
		if (*pfLambda > ANALYZER_LAMBDA_LOWER_LIMIT)
		{
			*pfLambda = ANALYZER_LAMBDA_LOWER_LIMIT;
		}
		else if (*pfLambda > ANALYZER_LAMBDA_UPPER_LIMIT)
		{
			*pfLambda = ANALYZER_LAMBDA_UPPER_LIMIT;
		}
#endif

	}
	else
	{
		//// 发生错误
		//// (1) HC
		//if (NULL != psHC)
		//{
		//	*psHC = 0;
		//}
		//// (2) CO
		//if (NULL != pfCO)
		//{
		//	*pfCO = 0.0f;
		//}
		//// (3) CO2
		//if (NULL != pfCO2)
		//{
		//	*pfCO2 = 0.0f;
		//}
		//// (4) O2
		//if (NULL != pfO2)
		//{
		//	*pfO2 = 0.0f;
		//}
		//// (5) NO
		//if (NULL != psNO)
		//{
		//	*psNO = 0;
		//}
		//// (6) RPM
		//if (NULL != pusRpm)
		//{
		//	*pusRpm = 0;
		//}
		//// (7) OilTemp
		//if (NULL != pfOilTemp)
		//{
		//	*pfOilTemp = 0.0f;
		//}
		//// (8) Flux
		//if (NULL != pusFlux)
		//{
		//	*pusFlux = 0;
		//}
		//// (9) Lambda
		//if (NULL != pfLambda)
		//{
		//	*pfLambda = 0.0f;
		//}
	}
	return dw;
}


// 2取辅助数据
// ET:Environment Temperature缩写
// AP:Atmospheric Pressure缩写
// RH:Relative Humidity缩写
DWORD CFuLiFLV1000::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	assert(NULL != pfET);
	assert(NULL != pfAP);
	assert(NULL != pfRH);

	BYTE bWriteBuf(2);
	BYTE bReadBuf[8];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 8, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) ET：环境温度
		// 可以为正或者负数,添加short强制转换
		*pfET = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1])) / 10.0f;
		// (2) AP：大气压力
		// 可以为正或者负数,添加short强制转换
		*pfAP = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 10.0f;
		// (3) RH：相对湿度
		*pfRH = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 10.0f;
	}
	else
	{
		//// 发生错误

		//// (1) ET：环境温度
		//*pfET = 0.0f;
		//// (2) AP：大气压力
		//*pfAP = 0.0f;
		//// (3) RH：相对湿度
		//*pfRH = 0.0f;
	}
	return dw;
}

// 3抽空气
DWORD CFuLiFLV1000::DeflateEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(34, pbResult);
}

// 4抽管道气(开泵,气样从探头进入)
DWORD CFuLiFLV1000::DeflatePipeAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(35, pbResult);
}

// 5清洗管道(反吹)
DWORD CFuLiFLV1000::InverseBlow(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(5, pbResult);
}

// 6停止清洗(停止反吹及停泵)
DWORD CFuLiFLV1000::StopPump(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(6, pbResult);
}

// 7检漏
DWORD CFuLiFLV1000::CheckLeak(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(7, pbResult);
}

// 8调零
DWORD CFuLiFLV1000::Zero(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(8, pbResult);
}

// 9写入校准气浓度
DWORD CFuLiFLV1000::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	return CmdFormat_Return00_01(9, pbResult, usCalC3H8, fCalCO, fCalCO2, usCalNO);
}

// 10校准
DWORD CFuLiFLV1000::GasCalibration(BYTE *const pbResult, const int nChanel, 
							  short *const psHC, 
							  float *const pfCO, 
							  float *const pfCO2, 
							  float *const pfO2, 
							  short *const psNO, 
							  short *const psNO2,
							  float *const pfPef)
{
	return CmdFormat_Return00_01_05(10, pbResult, psHC, pfCO, pfCO2, pfO2, psNO, pfPef);
}

// 11写入检查气浓度
DWORD CFuLiFLV1000::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return00_01(36, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12自动检查
DWORD CFuLiFLV1000::GasCheck(BYTE *const pbResult, 
						short *const psHC, 
						float *const pfCO, 
						float *const pfCO2, 
						float *const pfO2, 
						short *const psNO, 
						float *const pfPef)
{
	return CmdFormat_Return00_01_05(12, pbResult, psHC, pfCO, pfCO2, pfO2, psNO, pfPef);
}

// 13取PEF
DWORD CFuLiFLV1000::GetPef(float *const pfPef)
{
	assert(NULL != pfPef);

	BYTE bWriteBuf(13);
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 2, false ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// 可以为正或者负数,添加short强制转换
		*pfPef = (short)(MAKEWORD(bReadBuf[1], bReadBuf[0])) / 1000.0f;
	}
	else
	{
		// 发生错误

		//*pfPef = 0.0f;
	}
	return dw;
}

// 14清除NO老化标志
// 503特有
DWORD CFuLiFLV1000::ClearNoSign(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(14, pbResult);
}

// 15设置调零气体为空气
DWORD CFuLiFLV1000::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(15, pbResult);
}

// 16设置调零气体为零气
DWORD CFuLiFLV1000::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(16, pbResult);
}

// 17设置冲程为4
DWORD CFuLiFLV1000::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(17, pbResult);
}

// 18设置冲程为2
DWORD CFuLiFLV1000::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(18, pbResult);
}

// 19设置点火方式为单次
DWORD CFuLiFLV1000::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(19, pbResult);
}

// 20设置点火方式为二次
DWORD CFuLiFLV1000::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(20, pbResult);
}

// 21允许自动调零
DWORD CFuLiFLV1000::EnableAutoZero(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(21, pbResult);
}

// 22禁止自动调零
DWORD CFuLiFLV1000::DisableAutoZero(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(22, pbResult);
}

// 23允许键盘操作

// 24禁止键盘操作

// 25LCD开

// 26LCD关

// 27环境温度校准
DWORD CFuLiFLV1000::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	assert(fCalET>-20.0f && fCalET<150.0f);

	return CmdFormat_Return00_01_02(27, pbResult, fCalET);
}

// 28室温校准

// 29油温校准
DWORD CFuLiFLV1000::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	assert(fCalOT>-20.0f && fCalOT<150.0f);

	return CmdFormat_Return00_01_02(29, pbResult, fCalOT);
}

// 30环境湿度校准
DWORD CFuLiFLV1000::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	assert(fCalRH>0.0f && fCalRH<101.0f);

	return CmdFormat_Return00_01_02(30, pbResult, fCalRH);
}

// 31大气压力校准
DWORD CFuLiFLV1000::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	assert(fCalAP>0.0f && fCalAP<150.0f);

	return CmdFormat_Return00_01_02(31, pbResult, fCalAP);
}

// 32检查气开
DWORD CFuLiFLV1000::OpenCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(32, pbResult);
}

// 33检查气关
DWORD CFuLiFLV1000::CloseCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(33, pbResult);
}

// 34取仪器状态
DWORD CFuLiFLV1000::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf(38);
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		const BYTE b = bReadBuf;
		if (0xf0 == b)			// 新版本状态，转为旧版本状态
		{
			bReadBuf = 0x00;
		}
		else if (0xf1 == b)		// 新版本状态，转为旧版本状态
		{
			bReadBuf = 0x01;
		}
		//else if (0x00 == b)		// 旧版本状态，转为不支持
		//{
		//	bReadBuf = 0xf0;
		//}
		//else if (0x01 == b)		// 旧版本状态，转为不支持
		//{
		//	bReadBuf = 0xf1;
		//}
		*pbStatus = bReadBuf;
	}
	//CString strReturnStatus;
	//strReturnStatus.Format(L"%d", *pbStatus);
	//CNHLogAPI::WriteLog(L"CFuLiFLV1000", L"GetStatus返回状态码", strReturnStatus);

	return dw;
}

// 35取预热剩余时间
DWORD CFuLiFLV1000::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	assert(NULL != pusSecondsLeft);

	BYTE bWriteBuf(39);
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 2, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		*pusSecondsLeft = MAKEWORD(bReadBuf[1], bReadBuf[0]);
	}

	return dw;
}

// 36设置燃料类型为汽油
DWORD CFuLiFLV1000::SetFuelTypeToGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(40, pbResult);
}


// 37设置燃料类型为液化石油气
DWORD CFuLiFLV1000::SetFuelTypeToLPG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(41, pbResult);
}

// 38设置燃料类型为天然气
DWORD CFuLiFLV1000::SetFuelTypeToNG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(42, pbResult);
}

// 39设置燃料类型为乙醇汽油
DWORD CFuLiFLV1000::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(43, pbResult);
}

DWORD CFuLiFLV1000::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
	if (pbResult != NULL)
	{
		*pbResult = 0x06;
	}
	return ANA_WR_SUCCEED;
}


DWORD CFuLiFLV1000::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus/*=NULL*/,
		BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus/*=NULL*/,
		BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, 
								float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, 
								float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0x86;
	bWriteBuf[1] = 0x02;
	bWriteBuf[2] = 0x78;
	bWriteBuf[3] = 0x00;
	BYTE bReadBuf[12];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[0];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		float fFluxAct = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9])) * 0.1f;
		if (NULL != pfFluxAct)
		{
			// 可以为正或者负数,添加short强制转换
			*pfFluxAct = fFluxAct;
		}
		float fPres = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) * 0.1f;
		if (NULL != pfPres)
		{
			// 可以为正或者负数,添加short强制转换
			*pfPres = fPres;
		}
		float fUDilO2 = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) * 0.01f;
		if (NULL != pfUDilO2)
		{
			// 可以为正或者负数,添加short强制转换
			*pfUDilO2 = fUDilO2;
		}
		float fGasTemp = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) * 0.1f;
		if (NULL != pfGasTemp)
		{
			// 可以为正或者负数,添加short强制转换
			*pfGasTemp = fGasTemp;
		}
		if (NULL != pfFluxStd0)
		{
			*pfFluxStd0 = ActFlux2StdFlux0(fFluxAct, fGasTemp, fPres);
		}
		if (NULL != pfFluxStd20)
		{
			*pfFluxStd20 = ActFlux2StdFlux20(fFluxAct, fGasTemp, fPres);
		}
	}
	else
	{
		//// 发生错误
	}
	return dw;
}

DWORD CFuLiFLV1000::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	BYTE ret(0x00);
	
	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0xA3;
	bWriteBuf[1] = HIBYTE((WORD)(fCalPres * 100.0f));
	bWriteBuf[2] = LOBYTE((WORD)(fCalPres * 100.0f));
	bWriteBuf[3] = 0x00;
	BYTE bReadBuf[4] = {0};
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[0];
	}
	if (ANA_WR_SUCCEED == dw)
	{
	}

	return dw;
}

DWORD CFuLiFLV1000::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	
	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0xA0;
	bWriteBuf[1] = HIBYTE((WORD)(fCalDilO2 * 100.0f));
	bWriteBuf[2] = LOBYTE((WORD)(fCalDilO2 * 100.0f));
	bWriteBuf[3] = 0x00;
	BYTE bReadBuf[4] = {0};
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[0];
	}
	if (ANA_WR_SUCCEED == dw)
	{
	}

	return dw;
}

DWORD CFuLiFLV1000::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	if (NULL != pbResult)
	{
		*pbResult = 0x06;
	}
	if (NULL != pbCalResult)
	{
		*pbCalResult = 0x00;
	}
	return ANA_WR_SUCCEED;
}

DWORD CFuLiFLV1000::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[4] = {0};

	bWriteBuf[0] = 0xA2;
	bWriteBuf[1] = HIBYTE((WORD)(fCalDilO2 * 100.0f));
	bWriteBuf[2] = LOBYTE((WORD)(fCalDilO2 * 100.0f));
	bWriteBuf[3] = 0x00;

	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf[0] == 0x00)
		{
			ret = 0x00;
		}
	}
	else
	{
		ret = 0x01;
	}
	return ret;
}

DWORD CFuLiFLV1000::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[4] = {0};

	bWriteBuf[0] = 0xA1;
	bWriteBuf[1] = HIBYTE((WORD)(fCalDilO2 * 100.0f));
	bWriteBuf[2] = LOBYTE((WORD)(fCalDilO2 * 100.0f));
	bWriteBuf[3] = 0x00;

	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf[0] == 0x00)
		{
			ret = 0x00;
		}
	}
	else
	{
		ret = 0x01;
	}
	return ret;
}

DWORD CFuLiFLV1000::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::SatrtGetEnvironmentO2_Flowmeter(void)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLV1000::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
	if (pbResult != NULL)
	{
		*pbResult = 0x06;
	}
	return ANA_WR_SUCCEED;
}

// 获取手动检测的数据
DWORD CFuLiFLV1000::GetTestResultData(short *const psHCHigh/*=NULL*/, 
		float *const pfCOHigh/*=NULL*/, 
		float *const pfCO2High/*=NULL*/, 
		float *const pfO2High/*=NULL*/, 
		short *const psNOHigh/*=NULL*/,
		USHORT *const pusRpmHigh/*=NULL*/, 
		float *const pfOilTempHigh/*=NULL*/, 
		float *const pfLambdaHigh/*=NULL*/,
		short *const psHCLow/*=NULL*/, 
		float *const pfCOLow/*=NULL*/, 
		float *const pfCO2Low/*=NULL*/, 
		float *const pfO2Low/*=NULL*/, 
		short *const psNOLow/*=NULL*/,
		USHORT *const pusRpmLow/*=NULL*/, 
		float *const pfOilTempLow/*=NULL*/, 
		float *const pfLambdaLow/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}