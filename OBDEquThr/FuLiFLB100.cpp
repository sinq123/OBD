#include "StdAfx.h"
#include "FuLiFLB100.h"

#include <cassert>

CFuLiFLB100::CFuLiFLB100(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CFuLiFLB100::~CFuLiFLB100(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}


// 打开串口
DWORD CFuLiFLB100::Open(BYTE bPort)
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
DWORD CFuLiFLB100::Close(void)
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

bool CFuLiFLB100::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CFuLiFLB100::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CFuLiFLB100::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

BYTE CFuLiFLB100::CheckSum(BYTE *pb, UINT un)
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
DWORD CFuLiFLB100::StopWarmUp(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 校准
DWORD CFuLiFLB100::Calibrate(void)
{
	BYTE bWriteBuf[4] = {0x02, 0x04, 0x05, 0xf5};
	BYTE bReadBuf[4] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 4, false, false);
	return dw;
}

// 清除最大值
DWORD CFuLiFLB100::ClearMaxValue(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLB100::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 设置控制单元参数
DWORD CFuLiFLB100::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 设置相对湿度
DWORD CFuLiFLB100::SetRH(float fStdValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 复位EEPROM参数
DWORD CFuLiFLB100::ResetEEPROM(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取仪器报警信息
DWORD CFuLiFLB100::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取实时数据
DWORD CFuLiFLB100::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[4] = {0x02, 0x04, 0x02, 0xf8};
	BYTE bReadBuf[13] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 13, false, false);
	if (0UL == dw)
	{
		*pfN = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 10.0f;
		*pfK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		*pusRpm = MAKEWORD(bReadBuf[11], bReadBuf[10]);
		*pusOilTemp = MAKEWORD(bReadBuf[9], bReadBuf[8]);
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
DWORD CFuLiFLB100::GetMeasureUnitData(float *pfOriginalN, 
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
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取最大值
DWORD CFuLiFLB100::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	// 停止测量
	BYTE bWriteBuf[4] = {0x02, 0x04, 0x09, 0xf1};
	BYTE bReadBuf[12];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 4, false, false);

	// 取数
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x04;
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0xf7;
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 12, false, false);

	if (0UL == dw)
	{
		if (NULL != pfMaxN)
		{
			*pfMaxN = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 10.0f;
		}
		if (NULL != pfMaxK)
		{
			*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusMaxOilTemp)
		{
			*pusMaxOilTemp = MAKEWORD(bReadBuf[10], bReadBuf[9]);
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
		if (NULL != pusMaxOilTemp)
		{
			*pusMaxOilTemp = 0;
		}
	}
	return dw;
}

// 取仪器状态
DWORD CFuLiFLB100::GetStatus(BYTE *pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[4] = {0x02, 0x04, 0x01, 0xf9};
	BYTE bReadBuf[7] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 7, false, false);
	if (0UL == dw)
	{
		if (bReadBuf[3]&0x02)
		{
			m_bStatus = 0x00;
		}
		else if (bReadBuf[3]&0x01)
		{
			m_bStatus = 0x01;
		}
		else
		{
			m_bStatus = 0xFF;
		}
		*pbStatus = m_bStatus;
	}
	return dw;
}

// 设仪器状态
DWORD CFuLiFLB100::SetStatus(const BYTE bStatus)
{
	BYTE bWriteBuf[4] = {0};
	BYTE bReadBuf[4] = {0};
	if (bStatus == 0x01)
	{
		bWriteBuf[0] = 0x02;
		bWriteBuf[1] = 0x04;
		bWriteBuf[2] = 0x07;
		bWriteBuf[3] = 0xf3;
	}
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 4, false, false);
	return dw;
}

// 取预热剩余时间
DWORD CFuLiFLB100::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取环境参数
DWORD CFuLiFLB100::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取版本信息
DWORD CFuLiFLB100::GetVersion(float *pfVersion)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLB100::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取控制单元参数
DWORD CFuLiFLB100::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取补偿油温
DWORD CFuLiFLB100::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// 取手动检测数据
DWORD CFuLiFLB100::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}