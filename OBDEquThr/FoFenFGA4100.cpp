#include "StdAfx.h"
#include "FoFenFGA4100.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <stdlib.h>
#include <cassert>
#include <cstdio>

CFoFenFGA4100::CFoFenFGA4100(void)
//: m_hComm(INVALID_HANDLE_VALUE)
//, m_opUsedNow(ToAnalyzer)
{
	::InitializeCriticalSection(&cInfoSection);
	
	m_bStatus = 0;
	m_unTimerID = NULL;
}

CFoFenFGA4100::~CFoFenFGA4100(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CFoFenFGA4100::Open(BYTE bPort)
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
DWORD CFoFenFGA4100::Close(void)
{
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
	}

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

bool CFoFenFGA4100::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CFoFenFGA4100::WriteAndReadData(BYTE *const pbWriteBuffer, 
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


BYTE CFoFenFGA4100::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	
	return ~bSum;
}

DWORD CFoFenFGA4100::WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
												 const DWORD dwWriteLength, 
												 BYTE *const pbReadBuffer, 
												 const DWORD dwReadLength, 
												 const bool bNeedWriteChkSum, 
												 const bool bNeedReadChkSum)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

WORD CFoFenFGA4100::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);

	WORD wSum(0);
	for (UINT i=0; i<un; i++)
	{
		wSum += pb[i];
	}
	return ~wSum;
}

DWORD CFoFenFGA4100::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
{

	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
{

	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{

	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{

	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{

	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{

	return ANA_WR_EQUIP_RETURE_0X15;
}


// 1取主数据
DWORD CFoFenFGA4100::GetMajorData(short *const psHC, 
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

DWORD CFoFenFGA4100::GetMajorData(short *const psHC, 
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
	BYTE bWriteBuf[] = {0x7E, 0x01, 0xF4, 0x0A, 0x7E, 0x04, 0x00, 0x01, 0x02, 0x02, 0xF6};
	BYTE bReadBuf[51];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw
		&& bReadBuf[50] == CheckSum(&bReadBuf[18], 32))
	{
		char chTemp[5];
		// (1) HC
		if (NULL != psHC)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[18], 4);
			*psHC = (short)(atoi(chTemp));
		}
		// (2) CO
		if (NULL != pfCO)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[22], 4);
			*pfCO = (short)(atoi(chTemp)) / 100.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[26], 4);
			*pfCO2 = (short)(atoi(chTemp)) / 10.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[30], 4);			
			*pfO2 = (short)(atoi(chTemp)) / 10.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[34], 4);			
			*psNO = (short)(atoi(chTemp));
		}
		if (NULL != psNO2)
		{			
			*psNOx = 0;
		}
		if (NULL != psNOx)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[34], 4);			
			*psNOx = (short)(atoi(chTemp));
		}
		// (6) Lambda
		if (NULL != pfLambda)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[38], 4);	
			*pfLambda = (short)(atoi(chTemp)) / 100.0f;
		}
		// (7) RPM
		if (NULL != pusRpm)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[42], 4);
			*pusRpm = atoi(chTemp);
		}
		// (8) OilTemp
		if (NULL != pfOilTemp)
		{
			memset(chTemp, 0, 5);
			memcpy(chTemp, &bReadBuf[46], 4);
			*pfOilTemp = (short)(atoi(chTemp)) / 10.0f;
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
DWORD CFoFenFGA4100::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 3抽空气
DWORD CFoFenFGA4100::DeflateEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(34, pbResult);
}

// 4抽管道气(开泵,气样从探头进入)
DWORD CFoFenFGA4100::DeflatePipeAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[] = {0x7E, 0x01, 0xF4, 0x0A, 0x7E, 0x0A, 0x00, 0x01, 0x0E, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE3};
	BYTE bReadBuf[] = {0x7E, 0x01, 0xC6, 0x38, 0x7E, 0x01, 0x81, 0x7D, 0x7E, 0x0B, 0x01, 0x01, 0x0E, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE3};
	// 期待的成功返回
	BYTE bReturn[] = {0x7E, 0x01, 0xC6, 0x38, 0x7E, 0x01, 0x81, 0x7D, 0x7E, 0x0B, 0x01, 0x01, 0x0E, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE3};

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (memcmp(bReadBuf, bReturn, sizeof(bReadBuf)) == 0)
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
DWORD CFoFenFGA4100::InverseBlow(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(5, pbResult);
}

// 6停止清洗(停止反吹及停泵)
DWORD CFoFenFGA4100::StopPump(BYTE *const pbResult)
{
	BYTE bWriteBuf[] = {0x7E, 0x01, 0xF4, 0x0A, 0x7E, 0x0A, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE4};
	BYTE bReadBuf[] = {0x7E, 0x01, 0xC6, 0x38, 0x7E, 0x01, 0x81, 0x7D, 0x7E, 0x0B, 0x01, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE4};
	// 期待的成功返回
	BYTE bReturn[] = {0x7E, 0x01, 0xC6, 0x38, 0x7E, 0x01, 0x81, 0x7D, 0x7E, 0x0B, 0x01, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE4};

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (memcmp(bReadBuf, bReturn, sizeof(bReadBuf)) == 0)
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
DWORD CFoFenFGA4100::CheckLeak(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(7, pbResult);
}

// 8调零
DWORD CFoFenFGA4100::Zero(BYTE *const pbResult)
{
	BYTE bWriteBuf[] = {0x7E, 0x01, 0xF4, 0x0A, 0x7E, 0x04, 0x00, 0x01, 0x16, 0x45, 0x9F};
	BYTE bReadBuf[] = {0x7E, 0x01, 0xC6, 0x38, 0x7E, 0x01, 0x81, 0x7D/*, 0x7E, 0x05, 0x01, 0x01, 0x16, 0x00, 0x00, 0xE2*/};
	// 期待的成功返回
	BYTE bReturn[] = {0x7E, 0x01, 0xC6, 0x38, 0x7E, 0x01, 0x81, 0x7D/*, 0x7E, 0x05, 0x01, 0x01, 0x16, 0x00, 0x00, 0xE2*/};

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (memcmp(bReadBuf, bReturn, sizeof(bReadBuf)) == 0)
		{
			dw = ANA_WR_SUCCEED;

			m_bStatus = 0;
			if (NULL != m_unTimerID)
			{
				timeKillEvent(m_unTimerID);
				m_unTimerID = NULL;
			}
			m_unTimerID = timeSetEvent(30*1000, 1000, (LPTIMECALLBACK)TimePro, (DWORD_PTR)this, TIME_PERIODIC);
			if (NULL == m_unTimerID)
			{
				// 启动多媒体定时器失败
				//TRACE("启动多媒体定时器失败");
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

// 9写入校准气浓度
DWORD CFoFenFGA4100::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
	const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	return CmdFormat_Return00_01(9, pbResult, usCalC3H8, fCalCO, fCalCO2, usCalNO);
}

// 10校准
DWORD CFoFenFGA4100::GasCalibration(BYTE *const pbResult, const int nChanel, 
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
DWORD CFoFenFGA4100::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return00_01(36, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12自动检查
DWORD CFoFenFGA4100::GasCheck(BYTE *const pbResult, 
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
DWORD CFoFenFGA4100::GetPef(float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 14清除NO老化标志
// 503特有
DWORD CFoFenFGA4100::ClearNoSign(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(14, pbResult);
}

// 15设置调零气体为空气
DWORD CFoFenFGA4100::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(15, pbResult);
}

// 16设置调零气体为零气
DWORD CFoFenFGA4100::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(16, pbResult);
}

// 17设置冲程为4
DWORD CFoFenFGA4100::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(17, pbResult);
}

// 18设置冲程为2
DWORD CFoFenFGA4100::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(18, pbResult);
}

// 19设置点火方式为单次
DWORD CFoFenFGA4100::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(19, pbResult);
}

// 20设置点火方式为二次
DWORD CFoFenFGA4100::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(20, pbResult);
}

// 21允许自动调零
DWORD CFoFenFGA4100::EnableAutoZero(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(21, pbResult);
}

// 22禁止自动调零
DWORD CFoFenFGA4100::DisableAutoZero(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(22, pbResult);
}

// 23允许键盘操作

// 24禁止键盘操作

// 25LCD开

// 26LCD关

// 27环境温度校准
DWORD CFoFenFGA4100::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	assert(fCalET>-20.0f && fCalET<150.0f);

	return CmdFormat_Return00_01_02(27, pbResult, fCalET);
}

// 28室温校准

// 29油温校准
DWORD CFoFenFGA4100::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	assert(fCalOT>-20.0f && fCalOT<150.0f);

	return CmdFormat_Return00_01_02(29, pbResult, fCalOT);
}

// 30环境湿度校准
DWORD CFoFenFGA4100::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	assert(fCalRH>0.0f && fCalRH<101.0f);

	return CmdFormat_Return00_01_02(30, pbResult, fCalRH);
}

// 31大气压力校准
DWORD CFoFenFGA4100::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	assert(fCalAP>0.0f && fCalAP<150.0f);

	return CmdFormat_Return00_01_02(31, pbResult, fCalAP);
}

// 32检查气开
DWORD CFoFenFGA4100::OpenCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(32, pbResult);
}

// 33检查气关
DWORD CFoFenFGA4100::CloseCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(33, pbResult);
}

// 34取仪器状态
DWORD CFoFenFGA4100::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);
	*pbStatus = m_bStatus;
	return ANA_WR_SUCCEED;
}

// 35取预热剩余时间
DWORD CFoFenFGA4100::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 36设置燃料类型为汽油
DWORD CFoFenFGA4100::SetFuelTypeToGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(40, pbResult);
}


// 37设置燃料类型为液化石油气
DWORD CFoFenFGA4100::SetFuelTypeToLPG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(41, pbResult);
}

// 38设置燃料类型为天然气
DWORD CFoFenFGA4100::SetFuelTypeToNG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(42, pbResult);
}

// 39设置燃料类型为乙醇汽油
DWORD CFoFenFGA4100::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(43, pbResult);
}

DWORD CFoFenFGA4100::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}


DWORD CFoFenFGA4100::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
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

DWORD CFoFenFGA4100::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct, 
		float *const pfCOct, 
		float *const pfCO2ct, 
		short *const psNOct, 
		float *const pfDilO2, 
		float *const pfBenchO2, 
		short *const psGasTemp, 
		float *const pfGasFluxStd, 
		float *const pfExhaustFlux, 
		float *const pfGasPres, 
		float *const pfGasFluxAct, 
		USHORT *const pusRpm,
		WORD *const pwMiniStatus,
		const BYTE bDataMode,
		BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
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

DWORD CFoFenFGA4100::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::SatrtGetEnvironmentO2_Flowmeter(void)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CFoFenFGA4100::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 获取手动检测的数据
DWORD CFoFenFGA4100::GetTestResultData(short *const psHCHigh/*=NULL*/, 
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

void  CFoFenFGA4100::TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CFoFenFGA4100 *pf = (CFoFenFGA4100 *)dwUser;
	pf->m_bStatus = 0x10;
	timeKillEvent(uID);
}