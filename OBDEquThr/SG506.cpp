// DigitalThermometer.cpp : 定义 DLL 应用程序的导出函数。
//

#include "StdAfx.h"
#include "SG506.h"
#include <cassert>
#include <cstdio>
#include <stdlib.h>

CSG506::CSG506(void)
	: m_hComm(INVALID_HANDLE_VALUE)
	, m_fET(0.0f)
	, m_fAP(0.0f)
	, m_fRH(0.0f)
{
	::InitializeCriticalSection(&m_sCriticalSection);
}

CSG506::~CSG506(void)
{
	Close();

	::DeleteCriticalSection(&m_sCriticalSection);
}

// 打开串口
DWORD CSG506::Open(BYTE bPort)
{
	// 关闭已存在的串口句柄
	Close();

	::EnterCriticalSection(&m_sCriticalSection);

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

		::LeaveCriticalSection(&m_sCriticalSection);
		// 打开串口失败
		return PARA_OPEN_FAIL;
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
	 //进行超时设置（500毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	///////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	// 离开串口操作临界区
	::LeaveCriticalSection(&m_sCriticalSection);
	// 打开串口成功
	return PARA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CSG506::Close(void)
{
	if (IsOpen())
	{
		// 进入串口操作临界区
		::EnterCriticalSection(&m_sCriticalSection);

		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;

		// 离开串口操作临界区
		::LeaveCriticalSection(&m_sCriticalSection);

		if (b)
		{
			// 关闭串口成功
			return PARA_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return PARA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return PARA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CSG506::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

void CSG506::CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
	assert(NULL != pbData);
	assert(0UL != uiEnd);
	assert(uiEnd >= uiStart);
	//assert(NULL != pbCRC_HIGH);
	//assert(NULL != pbCRC_LOW);
	//assert(sizeof(pbData) >= uiEnd);

	WORD wSum(0x00);
	for (UINT ui=uiStart; ui<=uiEnd; ui++)
	{
		wSum += pbData[ui];
	}
	/////////////////////////////////////
	if (NULL != pbCRC_HIGH)
	{
		*pbCRC_HIGH = (wSum>>8) &0xFF;
	}
	if (NULL != pbCRC_LOW)
	{
		*pbCRC_LOW = wSum & 0xFF;
	}
	/////////////////////////////////////
}

bool CSG506::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
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

bool CSG506::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
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

DWORD CSG506::EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::BackupCalibrationParameters(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::SetFireTypeToSingleFire(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

bool CSG506::ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2)
{
		return false;
}

DWORD CSG506::GetOilTempRpm(float *const pfOilTemp, 
		USHORT *const pusRpm)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::GetIMEI_ICCID(char* chIMEI, char* chICCID)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CSG506::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	return GetEnvironmentParameters(pfET, pfRH, pfAP);
}

DWORD CSG506::GetEnvironmentParameters(float *const pfET, 
		float *const pfRH, 
		float *const pfAP)
{
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);
	
	// 判断端口是否打开
	if (IsOpen())
	{
		for (int i=0; i<3; i++)
		{
			PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		
			BYTE bHead[4] = {0};
			if (ReadCommBuffer(bHead, sizeof(bHead)))
			{
				if (memcmp(bHead, "SGHJ", 4) == 0)
				{
					BYTE bData[8] = {0};
					if (ReadCommBuffer(bData, sizeof(bData)))
					{
						m_fET = float(MAKEWORD(bData[0], bData[1]))/100.0f;
						m_fRH = float(MAKEWORD(bData[2], bData[3]))/100.0f;
						m_fAP = float(MAKELONG(MAKEWORD(bData[4], bData[5]), MAKEWORD(bData[6], bData[7])))/100.0f/1000.0f;//kpa
						if (NULL != pfET)
						{
							(*pfET) = m_fET;
						}
						if (NULL != pfRH)
						{
							(*pfRH) = m_fRH;
						}
						if (NULL != pfAP)
						{
							(*pfAP) = m_fAP;
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
				}
			}
		}
		
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}