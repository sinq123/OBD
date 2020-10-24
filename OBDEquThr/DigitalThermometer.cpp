// DigitalThermometer.cpp : 定义 DLL 应用程序的导出函数。
//

#include "StdAfx.h"
#include "DigitalThermometer.h"
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#define SEND_CMD			{'@','1','R'};
#define SEND_BUFFLENTH		3
#define RECV_BUFFLENTH		10
#define RECV_CODE			40
CDigitalThermometer::CDigitalThermometer(void)
	: m_hComm(INVALID_HANDLE_VALUE)
, m_fET(0.0f)
, m_fAP(0.0f)
, m_fRH(0.0f)
, m_nFlagET(0)
, m_nFlagRH(0)
, m_nFlagAP(0)
{
	::InitializeCriticalSection(&m_sCriticalSection);
}

CDigitalThermometer::~CDigitalThermometer(void)
{
	Close();

	::DeleteCriticalSection(&m_sCriticalSection);
}

// 打开串口
DWORD CDigitalThermometer::Open(BYTE bPort)
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
	 //进行超时设置（200毫秒之内要执行完读写操作）
	 //注意,不能够设置CommTimeouts.ReadTotalTimeoutConstant = 100;因为比较慢的计算机,将会发生超时。
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
DWORD CDigitalThermometer::Close(void)
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

bool CDigitalThermometer::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

void CDigitalThermometer::CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
}

bool CDigitalThermometer::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
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

bool CDigitalThermometer::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
{
	return false;;
}

DWORD CDigitalThermometer::EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::BackupCalibrationParameters(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::SetFireTypeToSingleFire(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

bool CDigitalThermometer::ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2)
{
		return false;
}

DWORD CDigitalThermometer::GetOilTempRpm(float *const pfOilTemp, 
		USHORT *const pusRpm)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::GetIMEI_ICCID(char* chIMEI, char* chICCID)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	return GetEnvironmentParameters(pfET,pfRH,pfAP);
}

DWORD CDigitalThermometer::GetEnvironmentParameters(float *const pfET, 
		float *const pfRH, 
		float *const pfAP)
{
	::EnterCriticalSection(&m_sCriticalSection);
	if (IsOpen())
	{
		BYTE bWriteBuf[3] = {'@','1','R'};
		DWORD dwNumberOfBytesToWrite = 3;
		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead = 10;
			BYTE bReadBuf[10];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			DWORD dwBytesRead(0);// 成功读出的数据字节数
			BOOL bSucceed = ReadFile(m_hComm, bReadBuf, dwNumberOfBytesToRead, &dwBytesRead, NULL);
			if (bSucceed && dwNumberOfBytesToRead == dwBytesRead && 0x40 == bReadBuf[0])
			{
				int nSum = bReadBuf[0]^bReadBuf[1]^bReadBuf[2]^bReadBuf[3]^bReadBuf[4]^bReadBuf[5]^bReadBuf[6]^bReadBuf[7]^bReadBuf[8];
				if(nSum != bReadBuf[9])
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_CHECK_SUM_FAIL;
				}
				DWORD dw1=bReadBuf[1]%16;
				DWORD dw2=bReadBuf[2];
				DWORD dw3=bReadBuf[3];
				DWORD dw4=bReadBuf[4];
				//AP:hPa
				float m_fAP_hPa =(float)((bReadBuf[1]>>4)*1000+((bReadBuf[1]&0x0f))*100+(bReadBuf[2]>>4)*10+((bReadBuf[2]&0x0f))*1+(bReadBuf[3]>>4)*0.1+((bReadBuf[3]&0x0f))*0.01);
				m_fAP = m_fAP_hPa/10.0f;
				//ET:.00
				m_fET =(float)((bReadBuf[5]>>4)*10+((bReadBuf[5]&0x0f))*1+(bReadBuf[6]>>4)*0.1+((bReadBuf[6]&0x0f))*0.01);
				(bReadBuf[4] == 0x00) ? m_fET = m_fET : m_fET = -m_fET;
				//RH:(%)
				m_fRH =(float)((bReadBuf[7]>>4)*100+((bReadBuf[7]&0x0f))*10+(bReadBuf[8]>>4)*1+((bReadBuf[8]&0x0f))*0.1);

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
			else
			{
#ifdef _DEBUG
				if (NULL != pfET)
				{
					(*pfET) =  rand()%100*1.0f;
				}
				if (NULL != pfRH)
				{
					(*pfRH) =   rand()%100*1.0f;
				}
				if (NULL != pfAP)
				{
					(*pfAP) =  rand()%100*1.0f;
				}
#else
				if (NULL != pfET)
				{
					(*pfET) = 0.0f;
				}
				if (NULL != pfRH)
				{
					(*pfRH) = 0.0f;
				}
				if (NULL != pfAP)
				{
					(*pfAP) = 0.0f;
				}
#endif
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
		// 写读失败：串口未打开
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}