#include "StdAfx.h"
#include "NanhuaNht1L.h"

#include <cassert>

CNanhuaNht1L::CNanhuaNht1L(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CNanhuaNht1L::~CNanhuaNht1L(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}


// 打开串口
DWORD CNanhuaNht1L::Open(BYTE bPort)
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
DWORD CNanhuaNht1L::Close(void)
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

bool CNanhuaNht1L::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNht1L::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CNanhuaNht1L::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

BYTE CNanhuaNht1L::CheckSum(BYTE *pb, UINT un)
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
DWORD CNanhuaNht1L::StopWarmUp(void)
{
	BYTE bWriteBuf[2] = {0xA2, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// 校准
DWORD CNanhuaNht1L::Calibrate(void)
{
	BYTE bWriteBuf[2] = {0xA4, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// 清除最大值
DWORD CNanhuaNht1L::ClearMaxValue(void)
{
	BYTE bWriteBuf[2] = {0xA7, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

//DWORD CNanhuaNht1L::SetControlUnitParams(const enum ParamType pt, const EngineStrokeType est)
//{
//	return SetControlUnitParams(BYTE(pt), BYTE(est));
//}
//
//DWORD CNanhuaNht1L::SetControlUnitParams(const enum ParamType pt, const RpmMeasurementType rmt)
//{
//	return SetControlUnitParams(BYTE(pt), BYTE(rmt));
//}
//
//DWORD CNanhuaNht1L::SetControlUnitParams(const enum ParamType pt, const FilterParamType fpt)
//{
//	return SetControlUnitParams(BYTE(pt), BYTE(fpt));
//}
//
//DWORD CNanhuaNht1L::SetControlUnitParams(const enum ParamType pt)
//{
//	return SetControlUnitParams(BYTE(pt), BYTE(0));
//}

DWORD CNanhuaNht1L::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	assert(NULL != psControlUnitParams);

	if (EngineStroke == psControlUnitParams->pt)
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.est));
	}
	else if (RpmMeasurement == psControlUnitParams->pt)
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.rmt));
	}
	else if (FilterParam == psControlUnitParams->pt)
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.fpt));
	}
	else
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.NoParam));
	}
}

// 设置控制单元参数
DWORD CNanhuaNht1L::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	BYTE bWriteBuf[4] = {0xB2, bParamType, bParamValue, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 2, true, true);
	return dw;
}

// 设置相对湿度
DWORD CNanhuaNht1L::SetRH(float fStdValue)
{
	assert(fStdValue>10e-6f && fStdValue<100.0f);

	// 放大10倍
	WORD wRH = WORD(fStdValue * 10.0f + 0.5f);
	BYTE bWriteBuf[4] = {0xB5, HIBYTE(wRH), LOBYTE(wRH), 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf,2, true, true);
	return dw;
}

// 复位EEPROM参数
DWORD CNanhuaNht1L::ResetEEPROM(void)
{
	BYTE bWriteBuf[2] = {0xB6, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// 取仪器报警信息
DWORD CNanhuaNht1L::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
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
DWORD CNanhuaNht1L::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
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
DWORD CNanhuaNht1L::GetMeasureUnitData(float *pfOriginalN, 
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
	BYTE bWriteBuf[2] = {0xB0, 0};
	BYTE bReadBuf[18];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 18, true, true);
	if (0UL == dw)
	{
		if (NULL != pfOriginalN)
		{
			*pfOriginalN =  MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		}
		if (NULL != pfOriginalK)
		{
			*pfOriginalK =  MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusSmokeTemp)
		{
			*pusSmokeTemp = bReadBuf[5];
		}
		if (NULL != pusTubeTemp)
		{
			*pusTubeTemp = bReadBuf[6];
		}
		if (NULL != pusDetectorTemp)
		{
			*pusDetectorTemp = bReadBuf[7];
		}
		if (NULL != pusLEDTemp)
		{
			*pusLEDTemp = bReadBuf[8];
		}
		if (NULL != pusBoardTemp)
		{
			*pusBoardTemp = bReadBuf[9];
		}
		if (NULL != pusLeftFanCurrent)
		{
			*pusLeftFanCurrent = bReadBuf[10] * 2;
		}
		if (NULL != pusRightFanCurrent)
		{
			*pusRightFanCurrent = bReadBuf[11] * 2;
		}
		if (NULL != pfPowerVoltage)
		{
			*pfPowerVoltage = bReadBuf[12] / 10.0f;
		}
		if (NULL != pusRealTimeIntensity)
		{
			*pusRealTimeIntensity = MAKEWORD(bReadBuf[14], bReadBuf[13]);
		}
		if (NULL != pusFullShadingIntensity)
		{
			*pusFullShadingIntensity = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		}
	}
	else
	{
		// 发生错误

		if (NULL != pfOriginalN)
		{
			*pfOriginalN =  0.0f;
		}
		if (NULL != pfOriginalK)
		{
			*pfOriginalK =  0.0f;
		}
		if (NULL != pusSmokeTemp)
		{
			*pusSmokeTemp = 0;
		}
		if (NULL != pusTubeTemp)
		{
		*pusTubeTemp = 0;
		}
		if (NULL != pusDetectorTemp)
		{
			*pusDetectorTemp = 0;
		}
		if (NULL != pusLEDTemp)
		{
			*pusLEDTemp = 0;
		}
		if (NULL != pusBoardTemp)
		{
			*pusBoardTemp = 0;
		}
		if (NULL != pusLeftFanCurrent)
		{
			*pusLeftFanCurrent = 0;
		}
		if (NULL != pusRightFanCurrent)
		{
			*pusRightFanCurrent = 0;
		}
		if (NULL != pfPowerVoltage)
		{
			*pfPowerVoltage = 0.0f;
		}
		if (NULL != pusRealTimeIntensity)
		{
			*pusRealTimeIntensity = 0;
		}
		if (NULL != pusFullShadingIntensity)
		{
			*pusFullShadingIntensity = 0;
		}
	}
	return dw;
}

// 取最大值
DWORD CNanhuaNht1L::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	BYTE bWriteBuf[2] = {0xA6, 0};
	BYTE bReadBuf[8];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 8, true, true);
	if (0UL == dw)
	{
		if (NULL != pfMaxN)
		{
			*pfMaxN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		}
		if (NULL != pfMaxK)
		{
			*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusMaxRpm)
		{
			*pusMaxRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		}
		if (NULL != pusMaxOilTemp)
		{
			*pusMaxOilTemp = MAKEWORD(bReadBuf[8], bReadBuf[7]) - 273;
		}
	}
	else
	{
		// 发生错误

		if (NULL != pfMaxN)
		{
			*pfMaxN = 0.0f;
		}
		if (NULL != pfMaxK)
		{
			*pfMaxK = 0.0f;
		}
		if (NULL != pusMaxRpm)
		{
			*pusMaxRpm = 0;
		}
		if (NULL != pusMaxOilTemp)
		{
			*pusMaxOilTemp = 0;
		}
	}
	return dw;
}

// 取仪器状态
DWORD CNanhuaNht1L::GetStatus(BYTE *pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[2] = {0xAD, 0};
	BYTE bReadBuf[3] = {0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 3, true, true);
	if (0UL == dw)
	{
		const BYTE b = bReadBuf[1];
		if (0xf1 == b)		// 新版本状态，转为旧版本状态
		{
			bReadBuf[1] = 0x01;
		}
		//else if (0x01 == b)		// 旧版本状态，转为不支持
		//{
		//	bReadBuf[1] = 0xf1;
		//}

		*pbStatus = m_bStatus = bReadBuf[1];
		//if(m_bStatus > 5)
		//{
		//	m_bStatus = 5;
		//}
	}
	return dw;
}

// 设仪器状态
DWORD CNanhuaNht1L::SetStatus(const BYTE bStatus)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取预热剩余时间
DWORD CNanhuaNht1L::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	assert(NULL!=pusMin && NULL!=pusSec);

	BYTE bWriteBuf[2] = {0xAE, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (0UL == dw)
	{
		*pusMin	= bReadBuf[1];
		*pusSec	= bReadBuf[2];
	}
	else
	{
		// 发生错误
		*pusMin	= 0;
		*pusSec	= 0;
	}
	return dw;
}

// 取环境参数
DWORD CNanhuaNht1L::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	assert(NULL!=pfET && NULL!=pfRH && NULL!=pfAP);

	BYTE bWriteBuf[2] = {0xAF, 0};
	BYTE bReadBuf[8];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 8, true, true);
	if (0UL == dw)
	{
		*pfET = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f - 273;
		*pfRH = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 10.0f;
		*pfAP = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 10.0f;
	}
	else
	{
		// 发生错误
		*pfET = 0.0f;
		*pfRH = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 10.0f;
		*pfAP = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 10.0f;
	}
	return dw;
}

// 取版本信息
DWORD CNanhuaNht1L::GetVersion(float *pfVersion)
{
	assert(NULL != pfVersion);

	BYTE bWriteBuf[2] = {0xB1, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (0UL == dw)
	{
		*pfVersion = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
	}
	else
	{
		// 发生错误
		*pfVersion = 0.0f;
	}
	return dw;
}

DWORD CNanhuaNht1L::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	BYTE b1(0), b2(0), b3(0);

	DWORD dw = GetControlUnitParams(&b1, &b2, &b3);


	if(0x01 == b1)
		*pest = FourStroke;
	else if(0x02 == b1)
		*pest = TwoStroke;

	if(0x01 == b2)
		*prmt = Piezoelectric;
	else if(0x02 == b2)
		*prmt = Photoelectric;
	else if(0x03 == b2)
		*prmt = BatteryType;

	if(0x01 == b3)
		*pfpt = FilterMode1;
	else if(0x02 == b3)
		*pfpt = FilterMode2;
	else if(0x03 == b3)
		*pfpt = NoFilter;

	return dw;
}

// 取控制单元参数
DWORD CNanhuaNht1L::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	assert(NULL!=pbEngineStroke);
	assert(NULL!=pbRpmMeasureType);
	assert(NULL!=pbFilterConstant);

	BYTE bWriteBuf[2] = {0xB3, 0};
	BYTE bReadBuf[5] = {0, 0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 5, true, true);
	if (0UL == dw)
	{
		*pbEngineStroke = bReadBuf[1];
		*pbRpmMeasureType = bReadBuf[2];
		*pbFilterConstant = bReadBuf[3];
	}
	else
	{
		*pbEngineStroke = 0;
		*pbRpmMeasureType = 0;
		*pbFilterConstant = 0;
	}
	return dw;
}

// 取补偿油温
DWORD CNanhuaNht1L::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	assert(NULL!=pfCorrectedOilTemp);

	BYTE bWriteBuf[2] = {0xB4, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (0UL == dw)
	{
		*pfCorrectedOilTemp = MAKEWORD(bReadBuf[2], bReadBuf[1]) /10.0f - 273;
    }
	else
	{
		*pfCorrectedOilTemp = 0.0f;
	}
	
	return FALSE;
}

// 取手动检测数据
DWORD CNanhuaNht1L::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNht1L::EntrySensitivityCalStatus()
{

	BYTE bWriteBuf[2] = {0xC1, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

DWORD CNanhuaNht1L::ProceedSensitivityCal(float const pfValue)
{
	BYTE bWriteBuf[4] = {0xC2, 0, 0, 0};
	BYTE bReadBuf[2] = {0, 0};
	bWriteBuf[1] = HIBYTE((WORD)(pfValue * 10.0f + 0.5f));
	bWriteBuf[2] = LOBYTE((WORD)(pfValue * 10.0f + 0.5f)); 
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 2, true, true);
	return dw;
}

DWORD CNanhuaNht1L::QuitSensitivityCalStatus()
{
	BYTE bWriteBuf[2] = {0xC3, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}