// DigitalThermometer.cpp : ���� DLL Ӧ�ó���ĵ���������
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

// �򿪴���
DWORD CDigitalThermometer::Open(BYTE bPort)
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
	 //���г�ʱ���ã�200����֮��Ҫִ�����д������
	 //ע��,���ܹ�����CommTimeouts.ReadTotalTimeoutConstant = 100;��Ϊ�Ƚ����ļ����,���ᷢ����ʱ��
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
DWORD CDigitalThermometer::Close(void)
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

bool CDigitalThermometer::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

void CDigitalThermometer::CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
}

bool CDigitalThermometer::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
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
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 10;
			BYTE bReadBuf[10];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			DWORD dwBytesRead(0);// �ɹ������������ֽ���
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
		// д��ʧ�ܣ�����δ��
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}