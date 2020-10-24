#include "StdAfx.h"
#include "NanhuaNht6.h"

#include <cassert>

CNanhuaNht6::CNanhuaNht6(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CNanhuaNht6::~CNanhuaNht6(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CNanhuaNht6::Open(BYTE bPort)
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
		return OPA_OPEN_FAIL;
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
	// 进行超时设置（100毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 20;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 100;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	return OPA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CNanhuaNht6::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			return OPA_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return OPA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return OPA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CNanhuaNht6::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNht6::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CNanhuaNht6::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// 分二次读数，第一次数据头（1byte），第二次读剩下的有效数据

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1UL==dwBytesRead)
		{
			// 判断非法或错误指令返回
			if(0x15 != pbReadBuffer[0])
			{
				// NHT-1L肯定有余下数据

				// 读余下的数据
				bSucceed = ReadFile(m_hComm, pbReadBuffer+1, dwReadLength-1, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-1))
				{
					if (bNeedReadChkSum)
					{
						// 需要读校验
						if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer,dwReadLength-1))
						{
							::LeaveCriticalSection(&cInfoSection);
							// 函数错误：校验读出数据失败
							return OPA_WR_CHECK_SUM_FAIL;
						}
						else
						{
							::LeaveCriticalSection(&cInfoSection);
							// 函数成功
							return OPA_WR_SUCCEED;
						}
					}
					else
					{
						::LeaveCriticalSection(&cInfoSection);
						// 函数成功
						return OPA_WR_SUCCEED;
					}
				}
				else
				{
					// 通讯失败
					m_bStatus = 5;

					::LeaveCriticalSection(&cInfoSection);
					// 函数错误：读指定数据失败（读余下有效数据失败）
					return OPA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				// 读错误状态码
				ReadFile(m_hComm, &m_bLastErrorCode, 1, &dwBytesRead, NULL);
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				//m_bLastErrorCode = pbReadBuffer[1];

				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：仪器返回错误状态码
				return OPA_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			// 通讯失败
			m_bStatus = 5;

			::LeaveCriticalSection(&cInfoSection);
			// 函数错误：读指定数据失败（读数据头失败）
			return OPA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// 错误：串口没有打开
		return OPA_WR_PORT_NOT_OPEN;
	}
}

BYTE CNanhuaNht6::CheckSum(BYTE *pb, UINT un)
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

// 中止预热	
DWORD CNanhuaNht6::StopWarmUp(void)
{
	BYTE bWriteBuf[2] = {0xA2, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// 校准
DWORD CNanhuaNht6::Calibrate(void)
{
	BYTE bWriteBuf[2] = {0xA4, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// 清除最大值
DWORD CNanhuaNht6::ClearMaxValue(void)
{
	BYTE bWriteBuf[2] = {0xA7,0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf,  2, true, true);
	return dw;
}

DWORD CNanhuaNht6::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 设置控制单元参数
DWORD CNanhuaNht6::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 设置相对湿度
DWORD CNanhuaNht6::SetRH(float fStdValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 复位EEPROM参数
DWORD CNanhuaNht6::ResetEEPROM(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取仪器报警信息
DWORD CNanhuaNht6::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	assert(NULL != pbs16AlarmInfo);

	BYTE bWriteBuf[2] = {0xA3, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (0UL == dw)
	{
		//*pwAlarmInfo = MAKEWORD(bReadBuf[2], bReadBuf[1]);
		*pbs16AlarmInfo = MAKEWORD(bReadBuf[2], bReadBuf[1]);
	}
	else
	{
		// 发生错误
		//pwAlarmInfo = 0;
		*pbs16AlarmInfo = 0x0000;
	}
	return dw;
}

// 取实时数据
DWORD CNanhuaNht6::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[2] = {0xA5, 0};
	BYTE bReadBuf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 10, true, true);
	if (0UL == dw)
	{
		*pfN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		*pfK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		*pusRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		*pusOilTemp = MAKEWORD(bReadBuf[8], bReadBuf[7]);
		if (0xFFFF == *pusOilTemp)
		{
			*pusOilTemp = 0;
		}
		else
		{
			*pusOilTemp -= 273;
		}
	}
	else
	{
		// 发生错误
		*pfN = 0.0f;
		*pfK = 0.0f;
		*pusRpm = 0;
		*pusOilTemp = 0;
	}
	return dw;
}

// 取测量单元内部数据
DWORD CNanhuaNht6::GetMeasureUnitData(float *pfOriginalN, 
								 float *pfOriginalK,
								 USHORT *pusSmokeTemp, 
								 USHORT *pusTubeTemp, 
								 USHORT *pusDetectorTemp, 
								 USHORT *pusLEDTemp, 
								 USHORT *pusBoardTemp,
								 USHORT *pusLeftFanCurrent, 
								 USHORT *pusRightFanCurrent, 
								 float *pfPowerVoltage,
								 USHORT *pusRealTimeIntensity, 
								 USHORT *pusFullShadingIntensity)
{
	//return OPA_WR_EQUIP_RETURE_0X15;

	assert(NULL!=pfOriginalN);

	BYTE bWriteBuf[2] = {0xB0, 0};
	BYTE bReadBuf[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 20, true, true);
	if (0UL == dw)
	{
		*pfOriginalN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
	}
	else
	{
		// 发生错误
		*pfOriginalN = 0.0f;
	}
	return dw;
}

// 取最大值
DWORD CNanhuaNht6::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	assert(NULL!=pfMaxN && NULL!=pfMaxK && NULL!=pusMaxRpm);

	BYTE bWriteBuf[2] = {0xA6, 0};
	BYTE bReadBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 8, true, true);
	if (0UL == dw)
	{
		*pfMaxN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		*pusMaxRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
	}
	else
	{
		// 发生错误
		*pfMaxN = 0.0f;
		*pfMaxK = 0.0f;
		*pusMaxRpm = 0;
	}
	return dw;
}

// 取仪器状态
DWORD CNanhuaNht6::GetStatus(BYTE *pbStatus)
{
	//return OPA_WR_EQUIP_RETURE_0X15;

	assert(NULL != pbStatus);

	BYTE bWriteBuf[2] = {0xA1, 0};
	BYTE bReadBuf[3] = {0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf,  3, true, true);
	*pbStatus = bReadBuf[1];
	return dw;
}

// 设仪器状态
DWORD CNanhuaNht6::SetStatus(const BYTE bStatus)
{
	// 测试状态码是否合法
	assert(bStatus>0 && bStatus<5);

	BYTE bWriteBuf[3] = {0xA0, bStatus, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 3, bReadBuf,  2, true, true);
	return dw;
}

// 取预热剩余时间
DWORD CNanhuaNht6::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取环境参数
DWORD CNanhuaNht6::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取版本信息
DWORD CNanhuaNht6::GetVersion(float *pfVersion)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNht6::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取控制单元参数
DWORD CNanhuaNht6::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取补偿油温
DWORD CNanhuaNht6::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取手动检测数据
DWORD CNanhuaNht6::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	BYTE bWriteBuf[2] = {0xB1,0};
	BYTE bReadBuf[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 12, true, true);
	if (0UL == dw)
	{
		if (NULL != pfValueK1)
		{
			*pfValueK1= MAKEWORD(bReadBuf[2], bReadBuf[1]) / 100.0f;
		}
		if (NULL != pfValueK2)
		{
			*pfValueK2 = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pfValueK3)
		{
			*pfValueK3 = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 100.0f;
		}
		if (NULL != pfValueKAvg)
		{
			*pfValueKAvg = MAKEWORD(bReadBuf[8], bReadBuf[7]) / 100.0f;
		}
		if(NULL != psLowRpm)
		{
			*psLowRpm = MAKEWORD(bReadBuf[10], bReadBuf[9]);
		}
	}
	else
	{
		// 发生错误

		if (NULL != pfValueK1)
		{
			*pfValueK1 = 0.0f;
		}
		if (NULL != pfValueK2)
		{
			*pfValueK2 = 0.0f;
		}
		if (NULL != pfValueK3)
		{
			*pfValueK3 = 0.0f;
		}
		if (NULL != pfValueKAvg)
		{
			*pfValueKAvg = 0.0f;
		}	
	}
	return dw;

}

DWORD CNanhuaNht6::EntrySensitivityCalStatus()
{

	BYTE bWriteBuf[2] = {0xC1, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

DWORD CNanhuaNht6::ProceedSensitivityCal(float const pfValue)
{
	BYTE bWriteBuf[4] = {0xC2, 0, 0, 0};
	BYTE bReadBuf[2] = {0, 0};
	bWriteBuf[1] = HIBYTE((WORD)(pfValue * 10.0f + 0.5f));
	bWriteBuf[2] = LOBYTE((WORD)(pfValue * 10.0f + 0.5f)); 
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 2, true, true);
	return dw;
}

DWORD CNanhuaNht6::QuitSensitivityCalStatus()
{
	BYTE bWriteBuf[2] = {0xC3, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}