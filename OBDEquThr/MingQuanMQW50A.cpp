#include "StdAfx.h"
#include "MingQuanMQW50A.h"

#include <stdlib.h>
#include <cassert>
#include <cstdio>

CMingQuanMQW50A::CMingQuanMQW50A(void)
//: m_hComm(INVALID_HANDLE_VALUE)
//, m_opUsedNow(ToAnalyzer)
{
	::InitializeCriticalSection(&cInfoSection);
}

CMingQuanMQW50A::~CMingQuanMQW50A(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CMingQuanMQW50A::Open(BYTE bPort)
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
	CommTimeouts.ReadTotalTimeoutConstant = 2500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	return ANA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CMingQuanMQW50A::Close(void)
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

bool CMingQuanMQW50A::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CMingQuanMQW50A::WriteAndReadData(BYTE *const pbWriteBuffer, 
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

BYTE CMingQuanMQW50A::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	
	return (~bSum+1);
}

DWORD CMingQuanMQW50A::WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
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

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 3, &dwBytesRead, NULL);
		if (bSucceed && 3==dwBytesRead)
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
			// assert(0x06==pbReadBuffer[1] || 0x05==pbReadBuffer[1] || 0x15==pbReadBuffer[1]);
			if (0x06 == pbReadBuffer[0])
			{
				// 流量计命令已经接受，并且已经完成

				// TODO:暂时屏蔽
				// 根据返回的数据长度判断VMAS内部程序版本是否对应F版通讯协议说明书
				//assert(MAKEWORD(pbReadBuffer[2],pbReadBuffer[3]) == (WORD)((dwReadLength-6)/2));

				// 读余下的数据
				bSucceed = ReadFile(m_hComm, pbReadBuffer+3, dwReadLength-3, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-3))
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

WORD CMingQuanMQW50A::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
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

DWORD CMingQuanMQW50A::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
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

// 适合于发四个指令  回四个指令的
DWORD CMingQuanMQW50A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = bCmd;
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if(dw == ANA_WR_SUCCEED)
	{
		ret = bReadBuf[0];
	}
	else
	{
		ret = 0x15;
	}
	return ret;
}

// 适合于发五个指令  回四个指令的   且第四个为选择指令的
DWORD CMingQuanMQW50A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult,const BYTE bType)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[5] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = bCmd;
	bWriteBuf[2] = 0x04;
	bWriteBuf[3] = bType;
	bWriteBuf[4] = 0x00;
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if(dw == ANA_WR_SUCCEED)
	{
		ret = bReadBuf[0];
	}
	else
	{
		ret = 0x15;
	}
	return ret;
}

DWORD CMingQuanMQW50A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[6] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = bCmd;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[5] = 0x00;
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if(dw == ANA_WR_SUCCEED)
	{
		ret = bReadBuf[0];
	}
	else
	{
		ret = 0x15;
	}
	return ret;
}


// 1取主数据
DWORD CMingQuanMQW50A::GetMajorData(short *const psHC, 
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

DWORD CMingQuanMQW50A::GetMajorData(short *const psHC, 
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
	BYTE bWriteBuf[4] = {0x02, 0x60, 0x03, 0x9B};
	BYTE bReadBuf[28];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) CO2
		if (NULL != pfCO2)
		{
			// 可以为正或者负数,添加short强制转换
			*pfCO2 = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// 可以为正或者负数,添加short强制转换
			*pfCO = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (3) HC
		if (NULL != psHC)
		{
			// 可以为正或者负数,添加short强制转换
			*psHC = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7]));
		}
		// (4) NO
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
		if (NULL != psNOx)
		{
			// 可以为正或者负数,添加short强制转换
			*psNOx = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		// (5) O2
		if (NULL != pfO2)
		{
			// 可以为正或者负数,添加short强制转换
			*pfO2 = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11])) / 100.0f;
		}
		// (6) OilTemp
		if (NULL != pfOilTemp)
		{
			// 可以为正或者负数,添加short强制转换
			*pfOilTemp = (short)(MAKEWORD(bReadBuf[16], bReadBuf[15]))/ 10.0f;
		}
		// (7) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[20], bReadBuf[19]);
		}
		// Lambda
		if (NULL != pfLambda)
		{
			// 可以为正或者负数,添加short强制转换
			*pfLambda = (short)(MAKEWORD(bReadBuf[24], bReadBuf[23])) / 1000.0f;
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
	}
	return dw;
}

// 2取辅助数据
// ET:Environment Temperature缩写
// AP:Atmospheric Pressure缩写
// RH:Relative Humidity缩写
DWORD CMingQuanMQW50A::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	assert(NULL != pfET);
	assert(NULL != pfAP);
	assert(NULL != pfRH);
	
	BYTE bWriteBuf[4] = {0x02, 0x60, 0x03, 0x9B};
	BYTE bReadBuf[28];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 28, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) ET：环境温度
		// 可以为正或者负数,添加short强制转换
		*pfET = (short)(MAKEWORD(bReadBuf[18], bReadBuf[17])) / 10.0f;
		// (2) AP：大气压力
		// 可以为正或者负数,添加short强制转换
		*pfAP = (short)(MAKEWORD(bReadBuf[26], bReadBuf[25])) / 10.0f;
		// (3) RH：相对湿度
		*pfRH = (short)(MAKEWORD(bReadBuf[14], bReadBuf[13])) / 10.0f;
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
DWORD CMingQuanMQW50A::DeflateEnvAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x7C, 0x03, 0x7F};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 4抽管道气(开泵,气样从探头进入)
DWORD CMingQuanMQW50A::DeflatePipeAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x7B, 0x03, 0x80};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 5清洗管道(反吹)
DWORD CMingQuanMQW50A::InverseBlow(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x80, 0x03, 0x7B};
	if (pbResult != NULL)
	{
		if (*pbResult == 0x01)
		{
			bWriteBuf[0] = 0x02;
			bWriteBuf[1] = 0x65;
			bWriteBuf[2] = 0x03;
			bWriteBuf[3] = 0x96;
		}
	}
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 6停止清洗(停止反吹及停泵)
DWORD CMingQuanMQW50A::StopPump(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x78, 0x03, 0x83};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 7检漏
DWORD CMingQuanMQW50A::CheckLeak(BYTE *const pbResult)
{
	DWORD dw(0x00);
	if (pbResult != NULL)
	{
		// 0x01指示调零，否则查询调零状态
		if (*pbResult == 0x01)
		{
			BYTE bWriteBuf[4] = {0x02, 0x66, 0x03, 0x95};
			BYTE bReadBuf[4];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));

			dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
			if (ANA_WR_SUCCEED == dw)
			{
				if (bReadBuf[0] == 0x06)
				{
					dw = ANA_WR_SUCCEED;

					if (pbResult != NULL)
					{
						*pbResult = 0x06;
					}
				}
				else
				{
					dw = ANA_WR_CHECK_SUM_FAIL;
				}
			}
			else
			{
				//// 发生错误
				dw = ANA_WR_EQUIP_RETURE_0X15;
			}
			return dw;
		}
	}

	BYTE bWriteBufStatus[] = {0x02, 0x61, 0x03, 0x9A};
	BYTE bReadBufStatus[11];
	ZeroMemory(bReadBufStatus, sizeof(bReadBufStatus));
	
	if (pbResult != NULL)
	{
		*pbResult = 0xFF;
	}
	dw = WriteAndReadData(bWriteBufStatus, sizeof(bWriteBufStatus), bReadBufStatus, sizeof(bReadBufStatus), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBufStatus[0] == 0x06)
		{
			BYTE b(0x00);
			if (bReadBufStatus[4]&0x80)
			{
				if (bReadBufStatus[3]&0x10)
				{
					// 正在检漏
					b = 0x04;
				}
				else
				{
					b = 0xFF;
				}
			}
			else
			{
				if (bReadBufStatus[5]&0x01)
				{
					b = 0x01;
				}
				else
				{
					b = 0x00;
				}
			}
			if (pbResult != NULL)
			{
				*pbResult = b;
			}
		}
	}

	return dw;
}

// 8调零
DWORD CMingQuanMQW50A::Zero(BYTE *const pbResult)
{
	DWORD dw(0x00);
	if (pbResult != NULL)
	{
		// 0x01指示调零，否则查询调零状态
		if (*pbResult == 0x01)
		{
			BYTE bWriteBuf[4] = {0x02, 0x67, 0x03, 0x94};
			BYTE bReadBuf[4];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));

			dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
			if (ANA_WR_SUCCEED == dw)
			{
				if (bReadBuf[0] == 0x06)
				{
					dw = ANA_WR_SUCCEED;

					if (pbResult != NULL)
					{
						*pbResult = 0x06;
					}
				}
				else
				{
					dw = ANA_WR_CHECK_SUM_FAIL;
				}
			}
			else
			{
				//// 发生错误
				dw = ANA_WR_EQUIP_RETURE_0X15;
			}
			return dw;
		}
	}

	BYTE bWriteBufStatus[] = {0x02, 0x61, 0x03, 0x9A};
	BYTE bReadBufStatus[11];
	ZeroMemory(bReadBufStatus, sizeof(bReadBufStatus));
	
	if (pbResult != NULL)
	{
		*pbResult = 0xFF;
	}
	dw = WriteAndReadData(bWriteBufStatus, sizeof(bWriteBufStatus), bReadBufStatus, sizeof(bReadBufStatus), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBufStatus[0] == 0x06)
		{
			BYTE b(0x00);
			if (bReadBufStatus[4]&0x80)
			{
				if (bReadBufStatus[3]&0x40)
				{
					// 正在调零
					b = 0x05;
				}
				else
				{
					b = 0xFF;
				}
			}
			else
			{
				if (bReadBufStatus[5]&0x04)
				{
					b = 0x01;
				}
				else
				{
					b = 0x00;
				}
			}
			if (pbResult != NULL)
			{
				*pbResult = b;
			}
		}
	}

	return dw;
}

// 9写入校准气浓度
DWORD CMingQuanMQW50A::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	return CmdFormat_Return00_01(9, pbResult, usCalC3H8, fCalCO, fCalCO2, usCalNO);
}

// 10校准
DWORD CMingQuanMQW50A::GasCalibration(BYTE *const pbResult, const int nChanel, 
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
DWORD CMingQuanMQW50A::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return00_01(36, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12自动检查
DWORD CMingQuanMQW50A::GasCheck(BYTE *const pbResult, 
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
DWORD CMingQuanMQW50A::GetPef(float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 14清除NO老化标志
// 503特有
DWORD CMingQuanMQW50A::ClearNoSign(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 15设置调零气体为空气
DWORD CMingQuanMQW50A::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x68, 0x04, 0x00, 0x93};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 16设置调零气体为零气
DWORD CMingQuanMQW50A::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x68, 0x04, 0x01, 0x92};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 17设置冲程为4
DWORD CMingQuanMQW50A::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6E, pbResult,0x01);
}

// 18设置冲程为2
DWORD CMingQuanMQW50A::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6E, pbResult,0x00);
}
// 19设置点火方式为单次
DWORD CMingQuanMQW50A::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6D, pbResult,0x00);
}

// 20设置点火方式为二次
DWORD CMingQuanMQW50A::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6D, pbResult,0x01);
}

// 21允许自动调零
DWORD CMingQuanMQW50A::EnableAutoZero(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x76, 0x04, 0x00, 0x84};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 22禁止自动调零
DWORD CMingQuanMQW50A::DisableAutoZero(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x76, 0x04, 0x01, 0x83};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 23允许键盘操作
DWORD CMingQuanMQW50A::EnableKeyOperate(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6A, pbResult);
}
// 24禁止键盘操作
DWORD CMingQuanMQW50A::DisableKeyOperate(BYTE *const pbResult) 
{
	return CmdFormat_Return00_01_05(0x6B, pbResult);
}

// 25LCD开

// 26LCD关

// 27环境温度校准
DWORD CMingQuanMQW50A::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	//assert(fCalET>-20.0f && fCalET<150.0f);

	//return CmdFormat_Return00_01_02(27, pbResult, fCalET);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x74; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalET * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalET * 10.0f));
	bWriteBuf[5] = 0x00; // 校验和后补
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 28室温校准

// 29油温校准
DWORD CMingQuanMQW50A::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	//assert(fCalOT>-20.0f && fCalOT<150.0f);

	//return CmdFormat_Return00_01_02(29, pbResult, fCalOT);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x73; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalOT * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalOT * 10.0f));
	bWriteBuf[5] = 0x00; // 校验和后补
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 30环境湿度校准
DWORD CMingQuanMQW50A::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	//assert(fCalRH>0.0f && fCalRH<101.0f);

	//return CmdFormat_Return00_01_02(30, pbResult, fCalRH);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x75; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalRH * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalRH * 10.0f));
	bWriteBuf[5] = 0x00; // 校验和后补
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 31大气压力校准
DWORD CMingQuanMQW50A::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	//assert(fCalAP>0.0f && fCalAP<150.0f);

	//return CmdFormat_Return00_01_02(31, pbResult, fCalAP);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x72; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalAP * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalAP * 10.0f));
	bWriteBuf[5] = 0x00; // 校验和后补
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// 发生错误
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 32检查气开
DWORD CMingQuanMQW50A::OpenCheckGas(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 33检查气关
DWORD CMingQuanMQW50A::CloseCheckGas(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 34取仪器状态
DWORD CMingQuanMQW50A::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[] = {0x02,0x61,0x03,0x9A};
	BYTE bReadBuf[11];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if(bReadBuf[0] == 0x06)
		{
			BYTE b(0x00);
			if (bReadBuf[4]&0x80)
			{
				if(bReadBuf[3]&0x01)
				{
					// 正在预热
					b = 0x02;
				}
				else if(bReadBuf[3]&0x40)
				{
					// 正在调零
					b = 0x05;
				}
				else if(bReadBuf[3] == 0x10)
				{
					// 正在密封性检查
					b = 0x13;
				}
				else
				{
					b = 0xFF;
				}
			}
			else
			{
				// 就绪
				b = 0x00;
			}
			*pbStatus = b;
		}
	}

	return ANA_WR_SUCCEED;
}

// 35取预热剩余时间
DWORD CMingQuanMQW50A::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 36设置燃料类型为汽油
DWORD CMingQuanMQW50A::SetFuelTypeToGas(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult,0x00);
}


// 37设置燃料类型为液化石油气
DWORD CMingQuanMQW50A::SetFuelTypeToLPG(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult,0x01);
}

// 38设置燃料类型为天然气
DWORD CMingQuanMQW50A::SetFuelTypeToNG(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult,0x02);
}

// 39设置燃料类型为乙醇汽油
DWORD CMingQuanMQW50A::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult);
}

DWORD CMingQuanMQW50A::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
	if (pbResult != NULL)
	{
		*pbResult = 0x06;
	}
	return ANA_WR_SUCCEED;
}


DWORD CMingQuanMQW50A::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
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

//DWORD CMingQuanMQW50A::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
//		const WORD wCntLength,
//		short *const psHCct=NULL, 
//		float *const pfCOct=NULL, 
//		float *const pfCO2ct=NULL, 
//		short *const psNOct=NULL, 
//		float *const pfDilO2=NULL, 
//		float *const pfBenchO2=NULL, 
//		short *const psGasTemp=NULL, 
//		float *const pfGasFluxStd0=NULL, 
//		float *const pfGasFluxStd20=NULL, 
//		float *const pfExhaustFluxStd0=NULL, 
//		float *const pfExhaustFluxStd20=NULL, 
//		float *const pfGasPres=NULL, 
//		float *const pfGasFluxAct=NULL, 
//		USHORT *const pusRpm=NULL,
//		WORD *const pwMiniStatus=NULL,
//		const BYTE bDataMode=0x01,
//		BYTE *const pbResult=NULL)
//{
//	return ANA_WR_EQUIP_RETURE_0X15;
//}

DWORD CMingQuanMQW50A::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
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

DWORD CMingQuanMQW50A::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

//  非标准状态下全部参数
DWORD CMingQuanMQW50A::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, 
								float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, 
								float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x86;
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x75;
	BYTE bReadBuf[14];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[0];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		float fFluxAct = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9])) / 6.0f;
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

// 标准状态下全部参数
//DWORD CMingQuanMQW50A::GetRealTimeData_Flowmeter(float *const pfFlux, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
//{
//	BYTE ret(0x00);
//
//	BYTE bWriteBuf[4] = {0};
//	bWriteBuf[0] = 0x02;
//	bWriteBuf[1] = 0x87;
//	bWriteBuf[2] = 0x03;
//	bWriteBuf[3] = 0x75;
//	BYTE bReadBuf[14];
//	ZeroMemory(bReadBuf, sizeof(bReadBuf));
//	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
//	if (ANA_WR_SUCCEED == dw)
//	{
//		// (1) HC
//		if (NULL != pfFlux)
//		{
//			// 可以为正或者负数,添加short强制转换
//			*pfFlux = (short)(MAKEWORD(bReadBuf[7], bReadBuf[8]));
//		}
//		// (2) CO
//		if (NULL != pfPres)
//		{
//			// 可以为正或者负数,添加short强制转换
//			*pfPres = (short)(MAKEWORD(bReadBuf[5], bReadBuf[6])) / 100.0f;
//		}
//		// (3) CO2
//		if (NULL != pfUDilO2)
//		{
//			// 可以为正或者负数,添加short强制转换
//			*pfUDilO2 = (short)(MAKEWORD(bReadBuf[3], bReadBuf[4])) / 100.0f;
//		}
//		// (4) O2
//		if (NULL != pfGasTemp)
//		{
//			// 可以为正或者负数,添加short强制转换
//			*pfGasTemp = (short)(MAKEWORD(bReadBuf[11], bReadBuf[12])) / 100.0f;
//		}
//	}
//	else
//	{
//		//// 发生错误
//	}
//	return bReadBuf[0];
//}

DWORD CMingQuanMQW50A::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};
	if( cm == HighFlowCal)
	{
		bWriteBuf[0] = 0x02;
		bWriteBuf[1] = 0xA6;
		bWriteBuf[2] = 0x05;
		bWriteBuf[3] = HIBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[4] = LOBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[5] = 0x00;
	}
	else if( cm == LowFlowCal)
	{
		bWriteBuf[0] = 0x02;
		bWriteBuf[1] = 0xA7;
		bWriteBuf[2] = 0x05;
		bWriteBuf[3] = HIBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[4] = LOBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[5] = 0x00;
	}
	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf == 0x00)
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

DWORD CMingQuanMQW50A::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};

	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0xAA;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fCalPres * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalPres * 10.0f));
	bWriteBuf[5] = 0x00;

	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false);
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

DWORD CMingQuanMQW50A::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	return SetCalibrationDilO2_Flowmeter_High(pbResult, fCalDilO2);
}

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
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

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};

	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0xA2;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[5] = 0x00;

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

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};

	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0xA3;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[5] = 0x00;

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

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SatrtGetEnvironmentO2_Flowmeter(void)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
	if (pbResult != NULL)
	{
		*pbResult = 0x06;
	}
	return ANA_WR_SUCCEED;
}

// 获取手动检测的数据
DWORD CMingQuanMQW50A::GetTestResultData(short *const psHCHigh/*=NULL*/, 
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
