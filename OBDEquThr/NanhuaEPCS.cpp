#include "StdAfx.h"
#include "NanhuaEPCS.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>

#define STX		0x02  // STX ��ʶ
#define ACK		0x06  // ACK ��ʶ
#define NAK		0x15  // NAK ��ʶ
#define ETB		0x17  // ETB ��ʶ
#define ETX		0x03  // ETX ��ʶ
#define ADD		0x08  // ETX ��ʶ

CNanhuaEPCS::CNanhuaEPCS(void)
	: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_sCriticalSection);
}

CNanhuaEPCS::~CNanhuaEPCS(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	::DeleteCriticalSection(&m_sCriticalSection);
}

// �򿪴���
DWORD CNanhuaEPCS::Open(BYTE bPort)
{
	// �ر��Ѵ��ڵĴ��ھ��
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
		// �򿪴���ʧ��
		OutputDebugStringW(L"\nError:fail to open com port\n");
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
	dcb.BaudRate = CBR_38400;
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨һ��ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ���ã�1000����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return PARA_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaEPCS::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
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

bool CNanhuaEPCS::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

void CNanhuaEPCS::CheckSum(const BYTE* pbData, const UINT uiStart, const UINT uiEnd, BYTE* const pbCRC_HIGH, BYTE* const pbCRC_LOW)
{
	assert(NULL != pbData);
	assert(0UL != uiEnd);
	assert(uiEnd >= uiStart);
	assert(NULL != pbCRC_HIGH);
	assert(NULL != pbCRC_LOW);
	//assert(sizeof(pbData) >= uiEnd);

	WORD wSum(0x00);
	for (UINT ui=uiStart; ui<=uiEnd; ui++)
	{
		wSum += pbData[ui];
	}
	/////////////////////////////////////
	wSum &= 0xFF;
	char chCrc[3];
	ZeroMemory(chCrc, sizeof(chCrc));
	sprintf_s(chCrc, 3, "%02X", wSum);
	(*pbCRC_HIGH) = chCrc[0];
	(*pbCRC_LOW) = chCrc[1];
	/////////////////////////////////////
}

bool CNanhuaEPCS::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
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

bool CNanhuaEPCS::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
{
	// ��ȡ�����ݵĳ��ȱ������10
	// ���ݰ�ͷ(8λ)+���󷵻�(8λ)
	assert(dwNumberOfBytesToRead >= 16);
	//assert(sizeof(pbReadBuffer) >= 16);
	//assert(sizeof(pbReadBuffer) >= dwNumberOfBytesToRead);

	DWORD dwBytesRead(0x00);

	// ��Comm
	DWORD dwNumberOfCommandToRead = 8;
	BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwNumberOfCommandToRead, &dwBytesRead, NULL);
	if (bSucceed && dwNumberOfCommandToRead==dwBytesRead)
	{
		// �жϷǷ������ָ���
		if(ACK == pbReadBuffer[6]) // ����
		{
			// �����µ�����
			// ��������:��Ҫ��ȡ������
			bSucceed = ReadFile(m_hComm, pbReadBuffer+8, dwNumberOfBytesToRead-8, &dwBytesRead, NULL);
			if(bSucceed && dwBytesRead==(dwNumberOfBytesToRead-8))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(NAK == pbReadBuffer[6]) // �쳣
		{
			// �����µ�����
			// ��������:���ݳ���(2λ)+<ETB>+����(1λ)+<ETB>+У����(2λ)+<ETX>
			const DWORD dwNumberOfErrorToRead(8);
			bSucceed = ReadFile(m_hComm, pbReadBuffer+8, dwNumberOfErrorToRead, &dwBytesRead, NULL);
			if(bSucceed && dwNumberOfErrorToRead==dwBytesRead)
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
	else
	{
		return false;
	}
}

DWORD CNanhuaEPCS::EnvironmentTemperatureSlopeCalibration(BYTE *const pbResult,
	const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		if(fSlope >= 0 )
		{
			int nSlope = int((fSlope + 0.05)*10.0f)%10000;
			sprintf_s(chSlope, 5, "%04d", nSlope);
		}
		else
		{
			// ������
			int nSlope(0);
			nSlope = int((fSlope - 0.05)*10.0f)*-1;
			sprintf_s(chSlope, 5, "%04d", nSlope);
			chSlope[0] =  0x30-(chSlope[0]-0x30);
			chSlope[1] =  0x30-(chSlope[1]-0x30);
			chSlope[2] =  0x30-(chSlope[2]-0x30);
			chSlope[3] =  0x30-(chSlope[3]-0x30);
		}
		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'T';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::RelativeHumiditySlopeCalibration(BYTE *const pbResult,
	const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'R';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::AirPressureSlopeCalibration(BYTE *const pbResult,
	const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'P';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::OilTemperatureColdSideSlopeCalibration(BYTE *const pbResult,
	const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::OilTemperatureHotSideSlopeCalibration(BYTE *const pbResult,
	const float fSlope)
{
	assert(NULL != pbResult);
	assert(fSlope >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nSlope = int((fSlope + 0.05)*10.0f)%10000;
		char chSlope[5];
		ZeroMemory(chSlope, sizeof(chSlope));
		sprintf_s(chSlope, 5, "%04d", nSlope);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'C';
		bWriteBuf[4] = 'O';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chSlope[0];
		bWriteBuf[13] = chSlope[1];
		bWriteBuf[14] = chSlope[2];
		bWriteBuf[15] = chSlope[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::EnvironmentTemperatureZeroCalibration(BYTE *const pbResult,
	const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		if(fZero >= 0 )
		{
			int nZero = int((fZero + 0.05)*10.0f)%10000;
			sprintf_s(chZero, 5, "%04d", nZero);
		}
		else
		{
			// ������
			int nZero(0);
			nZero = int((fZero - 0.05)*10.0f)*-1;
			sprintf_s(chZero, 5, "%04d", nZero);
			chZero[0] =  0x30-(chZero[0]-0x30);
			chZero[1] =  0x30-(chZero[1]-0x30);
			chZero[2] =  0x30-(chZero[2]-0x30);
			chZero[3] =  0x30-(chZero[3]-0x30);
		}

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'T';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::RelativeHumidityZeroCalibration(BYTE *const pbResult,
	const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nZero = int((fZero + 0.05)*10.0f)%10000;
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'R';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::AirPressureZeroCalibration(BYTE *const pbResult,
	const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nZero = int((fZero + 0.05)*10.0f)%10000;
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'P';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::OilTemperatureColdSideZeroCalibration(BYTE *const pbResult,
	const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 20;
		BYTE bWriteBuf[20];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		int nZero = int((fZero + 0.05)*10.0f)%10000;
		char chZero[5];
		ZeroMemory(chZero, sizeof(chZero));
		sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x35;
		bWriteBuf[11] = ETB;
		bWriteBuf[12] = chZero[0];
		bWriteBuf[13] = chZero[1];
		bWriteBuf[14] = chZero[2];
		bWriteBuf[15] = chZero[3];
		bWriteBuf[16] = ETB;
		CheckSum(bWriteBuf, 1, 16, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[17] = bCRC_HIGH;
		bWriteBuf[18] = bCRC_LOW;
		bWriteBuf[19] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::OilTemperatureHotSideZeroCalibration(BYTE *const pbResult,
	const float fZero)
{
	assert(NULL != pbResult);
	assert(fZero >= 0);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		// �Ŵ�10��,��ʽ��Ϊ4λint
		//int nZero = int((fZero + 0.05)*10.0f)%10000;
		//char chZero[5];
		//ZeroMemory(chZero, sizeof(chZero));
		//sprintf_s(chZero, 5, "%04d", nZero);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'Z';
		bWriteBuf[4] = 'O';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[16];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::BackupCalibrationParameters(BYTE *const pbResult)
{
	assert(NULL != pbResult);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'S';
		bWriteBuf[4] = 0x00;
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						(*pbResult) = bReadBuf[14];

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::RecoverCalibrationParameters(BYTE *const pbResult, const ParaType emParaType)
{
	assert(NULL != pbResult);

	if (EnvironmentTemperaturePara == emParaType)
	{
		if (ExecuteRecoverCmd('E','A') && ExecuteRecoverCmd('E','B'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (RelativeHumidityPara == emParaType)
	{
		if (ExecuteRecoverCmd('E','C') && ExecuteRecoverCmd('E','D'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (AirPressurePara == emParaType)
	{
		if (ExecuteRecoverCmd('E','E') && ExecuteRecoverCmd('E','F'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (OilTemperaturePara == emParaType)
	{
		if (ExecuteRecoverCmd('E','G') && ExecuteRecoverCmd('E','H') && ExecuteRecoverCmd('E','I') && ExecuteRecoverCmd('E','J'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else if (AllParameters == emParaType)
	{
		if (ExecuteRecoverCmd('E','K'))
		{
			(*pbResult) = 0x31;
			return PARA_WR_SUCCEED;
		}
		else
		{
			(*pbResult) = 0x30;
			return PARA_WR_UNKNOWN_ERROR;
		}
	}
	else
	{
		(*pbResult) = 0x30;
		return PARA_WR_UNKNOWN_ERROR;
	}
}

DWORD CNanhuaEPCS::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'B';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'C';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[11];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'I';
		bWriteBuf[4] = 'D';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						if (NULL != pbResult)
						{
							(*pbResult) = bReadBuf[14];
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

bool CNanhuaEPCS::ExecuteRecoverCmd(const BYTE bCmd1, const BYTE bCmd2)
{
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = bCmd1;
		bWriteBuf[4] = bCmd2;
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 19;
			BYTE bReadBuf[19];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						BYTE bResult = bReadBuf[14];

						if (0x31 == bReadBuf[14])
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return true;
						}
						else
						{
							::LeaveCriticalSection(&m_sCriticalSection);
							return false;
						}
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return false;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return false;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return false;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return false;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_sCriticalSection);
				return false;
			}
		}
		else
		{
			::LeaveCriticalSection(&m_sCriticalSection);
			return false;
		}
	}
	else
	{
		::LeaveCriticalSection(&m_sCriticalSection);
		return false;
	}
}

DWORD CNanhuaEPCS::GetEntireData(float *const pfET, 
	float *const pfRH, 
	float *const pfAP, 
	float *const pfOilTemp, 
	USHORT *const pusRpm)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);
	//assert(NULL != pfOilTemp);
	//assert(NULL != pusRpm);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

#ifdef USE_NORMAL_PARA_DATA //��ʱȡ������,ע�����ʱ���״̬�Ƿ�����By LiangQW@20161107
	// �¶�
	if (NULL != pfET)
	{
		(*pfET) = 0.0f;
	}
	// ʪ��
	if (NULL != pfRH)
	{
		(*pfRH) = 0.0f;
	}
	// ����ѹ
	if (NULL != pfAP)
	{
		(*pfAP) = 0.0f;
	}
	// ����
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0.0f;
	}
	// ת��
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}
#endif

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ���Ͷ�Command
		DWORD dwNumberOfBytesToWrite = 3;
		BYTE bWriteBuf[3];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));

		bWriteBuf[0] = STX;
		bWriteBuf[1] = 0x41;
		bWriteBuf[2] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 42;
			BYTE bReadBuf[42];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 38, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[39]==bCRC_HIGH && bReadBuf[40]==bCRC_LOW)
					{
						// �¶�
						BYTE bET[5];
						ZeroMemory(bET, sizeof(bET));
						bET[0] = bReadBuf[14];
						bET[1] = bReadBuf[15];
						bET[2] = bReadBuf[16];
						bET[3] = bReadBuf[17];
						if (NULL != pfET)
						{
							(*pfET) = float(atoi((char*)bET))/10.0f - 30.0f;
						}

						// ʪ��
						BYTE bRH[5];
						ZeroMemory(bRH, sizeof(bRH));
						bRH[0] = bReadBuf[19];
						bRH[1] = bReadBuf[20];
						bRH[2] = bReadBuf[21];
						bRH[3] = bReadBuf[22];
						if (NULL != pfRH)
						{
							(*pfRH) = float(atoi((char*)bRH))/10.0f;
						}

						// ����ѹ
						BYTE bAP[5];
						ZeroMemory(bAP, sizeof(bAP));
						bAP[0] = bReadBuf[24];
						bAP[1] = bReadBuf[25];
						bAP[2] = bReadBuf[26];
						bAP[3] = bReadBuf[27];
						if (NULL != pfAP)
						{
							(*pfAP) = float(atoi((char*)bAP))/10.0f;
						}

						// ����
						BYTE bOilTemp[5];
						ZeroMemory(bOilTemp, sizeof(bOilTemp));
						bOilTemp[0] = bReadBuf[29];
						bOilTemp[1] = bReadBuf[30];
						bOilTemp[2] = bReadBuf[31];
						bOilTemp[3] = bReadBuf[32];
						if (NULL != pfOilTemp)
						{
							(*pfOilTemp) = float(atoi((char*)bOilTemp))/10.0f - 30.0f;
						}

						// ת��
						BYTE bRpm[5];
						ZeroMemory(bRpm, sizeof(bRpm));
						bRpm[0] = bReadBuf[34];
						bRpm[1] = bReadBuf[35];
						bRpm[2] = bReadBuf[36];
						bRpm[3] = bReadBuf[37];
						if (NULL != pusRpm)
						{
							(*pusRpm) = atoi((char*)bRpm);
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::GetEnvironmentParameters(float *const pfET, 
	float *const pfRH, 
	float *const pfAP)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// �¶�
	if (NULL != pfET)
	{
		(*pfET) = 0.0f;
	}
	// ʪ��
	if (NULL != pfRH)
	{
		(*pfRH) = 0.0f;
	}
	// ����ѹ
	if (NULL != pfAP)
	{
		(*pfAP) = 0.0f;
	}

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'L';
		bWriteBuf[4] = 'A';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 32;
			BYTE bReadBuf[32];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 28, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[29]==bCRC_HIGH && bReadBuf[30]==bCRC_LOW)
					{
						// �¶�
						BYTE bET[5];
						ZeroMemory(bET, sizeof(bET));
						bET[0] = bReadBuf[14];
						bET[1] = bReadBuf[15];
						bET[2] = bReadBuf[16];
						bET[3] = bReadBuf[17];
						if (NULL != pfET)
						{
							(*pfET) = float(atoi((char*)bET))/10.0f - 30.0f;
						}

						// ʪ��
						BYTE bRH[5];
						ZeroMemory(bRH, sizeof(bRH));
						bRH[0] = bReadBuf[19];
						bRH[1] = bReadBuf[20];
						bRH[2] = bReadBuf[21];
						bRH[3] = bReadBuf[22];
						if (NULL != pfRH)
						{
							(*pfRH) = float(atoi((char*)bRH))/10.0f;
						}

						// ����ѹ
						BYTE bAP[5];
						ZeroMemory(bAP, sizeof(bAP));
						bAP[0] = bReadBuf[24];
						bAP[1] = bReadBuf[25];
						bAP[2] = bReadBuf[26];
						bAP[3] = bReadBuf[27];
						if (NULL != pfAP)
						{
							(*pfAP) = float(atoi((char*)bAP))/10.0f;
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::GetOilTempRpm(float *const pfOilTemp, 
	USHORT *const pusRpm)
{
	//assert(NULL != pfOilTemp);
	//assert(NULL != pusRpm);

	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	// ����
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0.0f;
	}
	// ת��
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}

	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		// ����Command
		DWORD dwNumberOfBytesToWrite = 15;
		BYTE bWriteBuf[15];
		ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
		BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);

		bWriteBuf[0] = STX;
		bWriteBuf[1] = ADD;
		bWriteBuf[2] = ETB;
		bWriteBuf[3] = 'L';
		bWriteBuf[4] = 'B';
		bWriteBuf[5] = ETB;
		bWriteBuf[6] = 0x30;
		bWriteBuf[7] = 0x30;
		bWriteBuf[8] = 0x30;
		bWriteBuf[9] = 0x30;
		bWriteBuf[10] = 0x30;
		bWriteBuf[11] = ETB;
		CheckSum(bWriteBuf, 1, 11, &bCRC_HIGH, &bCRC_LOW);
		bWriteBuf[12] = bCRC_HIGH;
		bWriteBuf[13] = bCRC_LOW;
		bWriteBuf[14] = ETX;

		if (WriteCommBuffer(bWriteBuf, dwNumberOfBytesToWrite))
		{
			// ��ȡ����
			DWORD dwNumberOfBytesToRead = 27;
			BYTE bReadBuf[27];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			if (ReadCommBuffer(bReadBuf, dwNumberOfBytesToRead))
			{
				if (ACK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 23, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[24]==bCRC_HIGH && bReadBuf[25]==bCRC_LOW)
					{
						// ����
						BYTE bOilTemp[5];
						ZeroMemory(bOilTemp, sizeof(bOilTemp));
						bOilTemp[0] = bReadBuf[14];
						bOilTemp[1] = bReadBuf[15];
						bOilTemp[2] = bReadBuf[16];
						bOilTemp[3] = bReadBuf[17];
						if (NULL != pfOilTemp)
						{
							(*pfOilTemp) = float(atoi((char*)bOilTemp))/10.0f - 30.0f;
						}

						// ת��
						BYTE bRpm[5];
						ZeroMemory(bRpm, sizeof(bRpm));
						bRpm[0] = bReadBuf[19];
						bRpm[1] = bReadBuf[20];
						bRpm[2] = bReadBuf[21];
						bRpm[3] = bReadBuf[22];
						if (NULL != pusRpm)
						{
							(*pusRpm) = atoi((char*)bRpm);
						}

						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_SUCCEED;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else if (NAK == bReadBuf[6])
				{
					BYTE bCRC_HIGH(0x00),bCRC_LOW(0x00);
					CheckSum(bReadBuf, 1, 15, &bCRC_HIGH, &bCRC_LOW);
					if (bReadBuf[16]==bCRC_HIGH && bReadBuf[17]==bCRC_LOW)
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_EQUIP_RETURE_0X15;
					}
					else
					{
						::LeaveCriticalSection(&m_sCriticalSection);
						return PARA_WR_CHECK_SUM_FAIL;
					}
				}
				else
				{
					::LeaveCriticalSection(&m_sCriticalSection);
					return PARA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
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
		::LeaveCriticalSection(&m_sCriticalSection);
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaEPCS::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
	return PARA_WR_UNKNOWN_ERROR;
}

DWORD CNanhuaEPCS::GetIMEI_ICCID(char* chIMEI, char* chICCID)
{
	return PARA_WR_UNKNOWN_ERROR;
}