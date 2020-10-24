// DigitalThermometer.cpp : ���� DLL Ӧ�ó���ĵ���������
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

// �򿪴���
DWORD CSG506::Open(BYTE bPort)
{
	// �ر��Ѵ��ڵĴ��ھ��
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
		// �򿪴���ʧ��
		return PARA_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��������������������С
	SetupComm(m_hComm, 512, 256);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨������Ϊ9600 bps
	dcb.BaudRate = CBR_9600;
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨һ��ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	 //���г�ʱ���ã�500����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	///////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	// �뿪���ڲ����ٽ���
	::LeaveCriticalSection(&m_sCriticalSection);
	// �򿪴��ڳɹ�
	return PARA_OPEN_SUCCEED;
}

// �رմ���
DWORD CSG506::Close(void)
{
	if (IsOpen())
	{
		// ���봮�ڲ����ٽ���
		::EnterCriticalSection(&m_sCriticalSection);

		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;

		// �뿪���ڲ����ٽ���
		::LeaveCriticalSection(&m_sCriticalSection);

		if (b)
		{
			// �رմ��ڳɹ�
			return PARA_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			return PARA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
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
	// ���������ݳ��ȱ������д�����ݵĳ���
	//assert(sizeof(pbWriteBuffer) >= dwNumberOfBytesToWrite);

	DWORD dwBytesWritten(0x00);

	// �����������
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	// дComm
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
	// ��Comm
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
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);
	
	// �ж϶˿��Ƿ��
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