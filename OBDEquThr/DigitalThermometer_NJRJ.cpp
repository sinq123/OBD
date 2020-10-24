// DigitalThermometer.cpp : 定义 DLL 应用程序的导出函数。
//

#include "StdAfx.h"
#include "DigitalThermometer_NJRJ.h"
#include <cassert>
#include <cstdio>
#include <stdlib.h>

#define SEND_CMD_LENGTH		1
#define SEND_CMD			{'1'};
#define SEND_BUFFLENTH1		34
#define SEND_BUFFLENTH2		43
#define RECV_BUFFLENTH1		34
#define RECV_BUFFLENTH2		43
#define RECV_CODE			77

CDigitalThermometer_NJRJ::CDigitalThermometer_NJRJ(void)
	: m_hComm(INVALID_HANDLE_VALUE)
	, m_fET(0.0f)
	, m_fAP(0.0f)
	, m_fRH(0.0f)
	, m_nFlagET(0)
	, m_nFlagRH(0)
	, m_nFlagAP(0)
	, m_dwReadIntervalTimeout(500)// 读间隔超时400～1000
	, m_dwReadTotalTimeoutMultiplier(0)// 读时间系数
	, m_dwReadTotalTimeoutConstant(1000)// 读时间超时常量800～1000
	, m_dwWriteTotalTimeoutMultiplier(0)// 写时间系数
	, m_dwWriteTotalTimeoutConstant(1000)// 写时间超时常量
{
	::InitializeCriticalSection(&m_sCriticalSection);
}

CDigitalThermometer_NJRJ::~CDigitalThermometer_NJRJ(void)
{
	Close();

	::DeleteCriticalSection(&m_sCriticalSection);
}

// 打开串口
DWORD CDigitalThermometer_NJRJ::Open(BYTE bPort)
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
	//COMMTIMEOUTS CommTimeouts;
	m_sCommTimeouts.ReadIntervalTimeout = m_dwReadIntervalTimeout;// 读间隔超时
	m_sCommTimeouts.ReadTotalTimeoutMultiplier = m_dwReadTotalTimeoutMultiplier;// 读时间系数
	m_sCommTimeouts.ReadTotalTimeoutConstant = m_dwReadTotalTimeoutConstant;// 读时间超时
	m_sCommTimeouts.WriteTotalTimeoutMultiplier = m_dwWriteTotalTimeoutMultiplier;// 写时间系数
	m_sCommTimeouts.WriteTotalTimeoutConstant = m_dwWriteTotalTimeoutConstant;// 写时间超时
	SetCommTimeouts(m_hComm, &m_sCommTimeouts);
	///////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	// 离开串口操作临界区
	::LeaveCriticalSection(&m_sCriticalSection);
	// 打开串口成功
	return PARA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CDigitalThermometer_NJRJ::Close(void)
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

bool CDigitalThermometer_NJRJ::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

void CDigitalThermometer_NJRJ::CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
}

bool CDigitalThermometer_NJRJ::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
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

bool CDigitalThermometer_NJRJ::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
{
	return false;;
}

DWORD CDigitalThermometer_NJRJ::EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::RelativeHumiditySlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::AirPressureSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
		const float fSlope)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::RelativeHumidityZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::AirPressureZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
		const float fZero)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::BackupCalibrationParameters(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::SetFireTypeToSingleFire(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
		return PARA_WR_UNKNOWN_ERROR;
}

bool CDigitalThermometer_NJRJ::ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2)
{
		return false;
}

DWORD CDigitalThermometer_NJRJ::GetOilTempRpm(float *const pfOilTemp, 
		USHORT *const pusRpm)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::GetIMEI_ICCID(char* chIMEI, char* chICCID)
{
		return PARA_WR_UNKNOWN_ERROR;
}

DWORD CDigitalThermometer_NJRJ::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	return GetEnvironmentParameters(pfET,pfRH,pfAP);
}

DWORD CDigitalThermometer_NJRJ::GetEnvironmentParameters(float *const pfET, 
		float *const pfRH, 
		float *const pfAP)
{
	::EnterCriticalSection(&m_sCriticalSection);
	if (IsOpen())
	{
		BYTE bWriteBuf[SEND_CMD_LENGTH] = SEND_CMD;
		DWORD dwNumberOfBytesToWrite = sizeof(bWriteBuf);
		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
#ifndef _READCOM_ONCE
		{
			// 一次取所有数据
			DWORD dwNumberOfBytesToRead = 77;
			DWORD dwBytesRead(0);// 成功读出的数据字节数
			BYTE bReadBuf[77];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			BOOL bSucceed = ReadFile(m_hComm, bReadBuf, dwNumberOfBytesToRead, &dwBytesRead, NULL);
			if (bSucceed && dwNumberOfBytesToRead == dwBytesRead)
			{
				char chAP[6] ={bReadBuf[5],bReadBuf[6],bReadBuf[7],bReadBuf[8],bReadBuf[9],bReadBuf[10]};
				m_fAP = static_cast<float> (atof(chAP));
				if (NULL != pfAP)
				{
					(*pfAP) = m_fAP;
				}
				char chET[5] ={bReadBuf[24],bReadBuf[25],bReadBuf[26],bReadBuf[27],bReadBuf[28]};
				m_fET = static_cast<float> (atof(chET));
				if (NULL != pfET)
				{
					*pfET = m_fET;
				}
				//HT:湿温
				char chHT[5] ={bReadBuf[39],bReadBuf[40],bReadBuf[41],bReadBuf[42],bReadBuf[43]};
				//露点：
				char chHP[5] ={bReadBuf[66],bReadBuf[67],bReadBuf[68],bReadBuf[69],bReadBuf[70]};
				//相对湿度
				char chRH[5] ={bReadBuf[52],bReadBuf[53],bReadBuf[54],bReadBuf[55],bReadBuf[56]};
				m_fRH = static_cast<float> (atof(chRH));
				if (NULL != pfRH)
				{
					(*pfRH) = m_fRH;
				}

				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_SUCCEED;
			}
			else
			{
				if (NULL != pfAP)
				{
					(*pfAP) = 0.0f;
				}
				if (NULL != pfET)
				{
					*pfET = 0.0f;
				}
				if (NULL != pfRH)
				{
					(*pfRH) = 0.0f;
				}
				::LeaveCriticalSection(&m_sCriticalSection);
				return PARA_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
#else
		{
			// 读取数据
			DWORD dwNumberOfBytesToRead1 = 34, dwNumberOfBytesToRead2 = 43;
			BYTE bReadBuf1[34], bReadBuf2[43];
			ZeroMemory(bReadBuf1, sizeof(bReadBuf1));
			ZeroMemory(bReadBuf2, sizeof(bReadBuf2));
			m_fET=0.0f;
			m_fAP=0.0f;
			m_fRH=0.0f;
			DWORD dwBytesRead1(0), dwBytesRead2(0);// 成功读出的数据字节数
			BOOL bSucceed = ReadFile(m_hComm, bReadBuf1, dwNumberOfBytesToRead1, &dwBytesRead1, NULL);
			if (bSucceed && dwNumberOfBytesToRead1 == dwBytesRead1)
			{
				char chAP[6] ={bReadBuf1[5],bReadBuf1[6],bReadBuf1[7],bReadBuf1[8],bReadBuf1[9],bReadBuf1[10]};
				m_fAP = static_cast<float> (atof(chAP));
				if (NULL != pfAP)
				{
					(*pfAP) = m_fAP;
				}
				char chET[5] ={bReadBuf1[24],bReadBuf1[25],bReadBuf1[26],bReadBuf1[27],bReadBuf1[28]};
				m_fET = static_cast<float> (atof(chET));
				if (NULL != pfET)
				{
					*pfET = m_fET;
				}
			}
			else
			{
				if (NULL != pfAP)
				{
					(*pfAP) = 0.0f;
				}
				if (NULL != pfET)
				{
					*pfET = 0.0f;
				}
			}

			bSucceed = ReadFile(m_hComm, bReadBuf2, dwNumberOfBytesToRead2, &dwBytesRead2, NULL);
			if (bSucceed && dwNumberOfBytesToRead2 == dwBytesRead2)
			{
				//HT:湿温
				char chHT[5] ={bReadBuf2[5],bReadBuf2[6],bReadBuf2[7],bReadBuf2[8],bReadBuf2[9]};
				//露点：
				char chHP[5] ={bReadBuf2[32],bReadBuf2[33],bReadBuf2[34],bReadBuf2[35],bReadBuf2[36]};
				//相对湿度
				char chRH[5] ={bReadBuf2[18],bReadBuf2[19],bReadBuf2[20],bReadBuf2[21],bReadBuf2[22]};
				m_fRH = static_cast<float> (atof(chRH));
				if (NULL != pfRH)
				{
					(*pfRH) = m_fRH;
				}
			}
			else
			{
				if (NULL != pfRH)
				{
					(*pfRH) = 0.0f;
				}
			}

			::LeaveCriticalSection(&m_sCriticalSection);
			return PARA_WR_SUCCEED;
		}
#endif
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