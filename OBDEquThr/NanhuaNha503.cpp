#include "StdAfx.h"
#include "NanhuaNha503.h"

#include <cassert>
#include <cstdio>

CNanhuaNha503::CNanhuaNha503(void)
//: m_hComm(INVALID_HANDLE_VALUE)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CNanhuaNha503::~CNanhuaNha503(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNha503::Open(BYTE bPort)
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
		OutputDebugStringW(L"Error:fail to open com port\n");
		return ANA_OPEN_FAIL;
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
	// ���г�ʱ���ã�2500����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 2500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return ANA_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaNha503::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// �رմ��ڳɹ�
			return ANA_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			return ANA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
		return ANA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CNanhuaNha503::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNha503::WriteAndReadData(BYTE *const pbWriteBuffer, 
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
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		// �ж��Ƿ����ô��ڲ���
		if (bPurgeComm)
		{
			PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		}

		// �ж��Ƿ���Ҫ����д���ݲ���
		if (NULL != pbWriteBuffer)
		{
			// ��Ҫ����д�����ݲ���

			// �ж�д�������Ƿ���Ҫ����У��
			if (dwWriteLength>1 && bNeedWriteChkSum)
			{
				pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
			}
			WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		}
		
		// һ�ζ���Ԥ��Ҫ���յ��ֽ���

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			if (dwReadLength>1 && bNeedReadChkSum)
			{
				// ��֤У���
				if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer, dwReadLength-1))
				{
					::LeaveCriticalSection(&cInfoSection);

					// ��������У���������ʧ��
					return ANA_WR_CHECK_SUM_FAIL;
				}
				else
				{
					::LeaveCriticalSection(&cInfoSection);

					// �����ɹ�
					return ANA_WR_SUCCEED;
				}
			}
			else
			{
				::LeaveCriticalSection(&cInfoSection);

				// �����ɹ�
				return ANA_WR_SUCCEED;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);

			// �������󣺶�ָ������ʧ��
			return ANA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);

		// �������󣺴���û�д�
		return ANA_WR_PORT_NOT_OPEN;
	}
}

BYTE CNanhuaNha503::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	// ������У�鲻����ȡ����һ
	//bSum = ~bSum + 1;
	return bSum;
}

DWORD CNanhuaNha503::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE bWriteBuf = bCmd;
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (NULL != pbResult)
	{
		// ��ȡ����ִ�н��

		if (ANA_WR_SUCCEED == dw)
		{
			// �������ؽ��,һ�㷵��0x06
			*pbResult = bReadBuf;
		}
	}
	return dw;
}

DWORD CNanhuaNha503::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE bWriteBuf = bCmd;
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (NULL != pbResult)
	{
		// ��ȡ����ִ�н��

		if (ANA_WR_SUCCEED == dw)
		{
			*pbResult = bReadBuf;
		}
	}
	return dw;
}

DWORD CNanhuaNha503::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
{
	assert(NULL != pbResult);

	// ��2�����,��1��������ͷ,��2�����������������������

	// ��1��:������ͷ
	DWORD dw = CmdFormat_Return00_01_05(bCmd, pbResult);
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x05 == *pbResult)
		{
			// �������ؽ��:���ڽ��в���,Ҫ����������Ĳ�����������

			// ��2��:��������������(����ÿ�ζ�Ҫִ��,��������05����ͷʱ����Ҫִ��)
			BYTE bReadBuf[14];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			dw = WriteAndReadData(NULL, 0, bReadBuf+1, 13, false, false, false);
			if (ANA_WR_SUCCEED == dw)
			{
				// (1)HC
				if (NULL != psHC)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
				}
				// (2)CO
				if (NULL != pfCO)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
				}
				// (3)CO2
				if (NULL != pfCO2)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
				}
				// (4)O2
				if (NULL != pfO2)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
				}
				// (5)NO
				if (NULL != psNO)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
				}
				// (6)Pef
				if (NULL != pfPef)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*pfPef = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11])) / 1000.0f;
				}
			}
			else
			{
				// (1)HC
				if (NULL != psHC)
				{
					*psHC = 0;
				}
				// (2)CO
				if (NULL != pfCO)
				{
					*pfCO = 0;
				}
				// (3)CO2
				if (NULL != pfCO2)
				{
					*pfCO2 = 0;
				}
				// (4)O2
				if (NULL != pfO2)
				{
					*pfO2 = 0;
				}
				// (5)NO
				if (NULL != psNO)
				{
					*psNO = 0;
				}
				// (6)Pef
				if (NULL != pfPef)
				{
					*pfPef = 0;
				}
			}
		}
	}

	return dw;
}

DWORD CNanhuaNha503::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	assert(NULL != pbResult);
	assert(usC3H8>500 && usC3H8<60000);
	assert(fCO>10e-6f && fCO<60000);
	assert(fCO2>10e-6f && fCO2<60000);
	assert(usNO>500 && usNO<60000);

	BYTE bWriteBuf[9];
	BYTE bReadBuf(0);
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));

	// ��2����ִ��

	// ��1����,д������ͷ
	DWORD dw = CmdFormat_OnlyReturn06(bCmd, pbResult);
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 != *pbResult)
		{
			// �������ش�����Ϣ
			return dw;
		}
	}
	else
	{
		// ��1����,������������
		return dw;
	}

	// ��2����,д������Ũ������
	bWriteBuf[0] = HIBYTE(usC3H8);
	bWriteBuf[1] = LOBYTE(usC3H8);
	bWriteBuf[2] = HIBYTE((WORD)(fCO * 100.0f));
	bWriteBuf[3] = LOBYTE((WORD)(fCO * 100.0f));
	bWriteBuf[4] = HIBYTE((WORD)(fCO2 * 100.0f));
	bWriteBuf[5] = LOBYTE((WORD)(fCO2 * 100.0f));
	bWriteBuf[6] = HIBYTE(usNO);
	bWriteBuf[7] = LOBYTE(usNO);
	bWriteBuf[8] = 0;
	dw = WriteAndReadData(bWriteBuf, 9, &bReadBuf, 1, true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// �������ؽ��
		*pbResult = bReadBuf;
	}

	return dw;
}

DWORD CNanhuaNha503::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	assert(NULL != pbResult);
	//assert(fValue>-20.0f && fValue<150.0f);

	// 503��2����ִ��

	// ��1����,д������ͷ
	DWORD dw = CmdFormat_OnlyReturn06(bCmd, pbResult);
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 != *pbResult)
		{
			// �������ش�����Ϣ
			return dw;
		}
	}
	else
	{
		// ��1����,������������
		return dw;
	}

	// ��2����,д������Ũ������
	BYTE bReadBuf(0);
	BYTE bWriteBuf[2];
	bWriteBuf[0] = HIBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[1] = LOBYTE((WORD)(fValue * 10.0f));
	dw = WriteAndReadData(bWriteBuf, 2, &bReadBuf, 1, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// �������ؽ��
		*pbResult = bReadBuf;
	}

	return dw;
}


// 1ȡ������
DWORD CNanhuaNha503::GetMajorData(short *const psHC, 
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
	BYTE bWriteBuf(0x81), bReadBuf[21];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 21, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) HC
		if (NULL != psHC)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		if (NULL != psNO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNO2 = 0;
		}
		if (NULL != psNOx)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNOx = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[12], bReadBuf[11]);
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfOilTemp = (short)MAKEWORD(bReadBuf[14], bReadBuf[13]) / 10.0f;
		}
		// (8) Flux
		if (NULL != pusFlux)
		{
			*pusFlux = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		}
		// (9) Lambda
		if (NULL != pfLambda)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfLambda = (short)(MAKEWORD(bReadBuf[19], bReadBuf[18])) / 100.0f;
		}
		// (10)���������Ƿ��ϻ�
		if (NULL != pbO2Sensor)
		{
			if (0xFF == (bReadBuf[17]|0xFE))
			{
				*pbO2Sensor = true;
			}
			else
			{
				*pbO2Sensor = false;
			}
		}
		// (11)NO�������Ƿ��ϻ�
		if (NULL != pbNOSensor)
		{
			if (0xFF == (bReadBuf[17]|0xFD))
			{
				*pbNOSensor = true;
			}
			else
			{
				*pbNOSensor = false;
			}
		}

		// ����ȡֵ��Χ
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
		// ��������
		// (1) HC
		if (NULL != psHC)
		{
			*psHC = 0;
		}
		// (2) CO
		if (NULL != pfCO)
		{
			*pfCO = 0.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			*pfCO2 = 0.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			*pfO2 = 0.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			*psNO = 0;
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = 0;
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			*pfOilTemp = 0.0f;
		}
		// (8) Flux
		if (NULL != pusFlux)
		{
			*pusFlux = 0;
		}
		// (9) Lambda
		if (NULL != pfLambda)
		{
			*pfLambda = 0.0f;
		}
	}
	return dw;
}

// 1ȡ������(��)
DWORD CNanhuaNha503::GetMajorData(short *const psHC, 
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
	BYTE bWriteBuf(0x2C), bReadBuf[23];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 23, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) HC
		if (NULL != psHC)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		if (NULL != psNO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNO2 = 0;
		}
		if (NULL != psNOx)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNOx = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[12], bReadBuf[11]);
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfOilTemp = (short)MAKEWORD(bReadBuf[14], bReadBuf[13]) / 10.0f;
		}
		// (8) Flux
		if (NULL != pusFlux)
		{
			*pusFlux = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		}
		// (9) Lambda
		if (NULL != pfLambda)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfLambda = (short)(MAKEWORD(bReadBuf[19], bReadBuf[18])) / 100.0f;
		}
		// (10)���������Ƿ��ϻ�
		if (NULL != pbO2Sensor)
		{
			if (0xFF == (bReadBuf[17]|0xFE))
			{
				*pbO2Sensor = true;
			}
			else
			{
				*pbO2Sensor = false;
			}
		}
		// (11)NO�������Ƿ��ϻ�
		if (NULL != pbNOSensor)
		{
			if (0xFF == (bReadBuf[17]|0xFD))
			{
				*pbNOSensor = true;
			}
			else
			{
				*pbNOSensor = false;
			}
		}

		// (12)ˮλ�Ƿ�����
		if (NULL != pbWaterFull)
		{
			if (0xFF == (bReadBuf[21]|0xBF))
			{
				*pbWaterFull = true;
			}
			else
			{
				*pbWaterFull = false;
			}
		}

		// ����ȡֵ��Χ
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
		// ��������
		// (1) HC
		if (NULL != psHC)
		{
			*psHC = 0;
		}
		// (2) CO
		if (NULL != pfCO)
		{
			*pfCO = 0.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			*pfCO2 = 0.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			*pfO2 = 0.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			*psNO = 0;
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = 0;
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			*pfOilTemp = 0.0f;
		}
		// (8) Flux
		if (NULL != pusFlux)
		{
			*pusFlux = 0;
		}
		// (9) Lambda
		if (NULL != pfLambda)
		{
			*pfLambda = 0.0f;
		}
	}
	return dw;
}


// 2ȡ��������
// ET:Environment Temperature��д
// AP:Atmospheric Pressure��д
// RH:Relative Humidity��д
DWORD CNanhuaNha503::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	assert(NULL != pfET);
	assert(NULL != pfAP);
	assert(NULL != pfRH);

	BYTE bWriteBuf(2);
	BYTE bReadBuf[8];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 8, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) ET�������¶�
		// ����Ϊ�����߸���,���shortǿ��ת��
		*pfET = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1])) / 10.0f;
		// (2) AP������ѹ��
		// ����Ϊ�����߸���,���shortǿ��ת��
		*pfAP = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 10.0f;
		// (3) RH�����ʪ��
		*pfRH = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 10.0f;
	}
	else
	{
		//// ��������

		// (1) ET�������¶�
		*pfET = 0.0f;
		// (2) AP������ѹ��
		*pfAP = 0.0f;
		// (3) RH�����ʪ��
		*pfRH = 0.0f;
	}
	return dw;
}

// 3�����
DWORD CNanhuaNha503::DeflateEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(3, pbResult);
}

// 4��ܵ���(����,������̽ͷ����)
DWORD CNanhuaNha503::DeflatePipeAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(4, pbResult);
}

// 5��ϴ�ܵ�(����)
DWORD CNanhuaNha503::InverseBlow(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(5, pbResult);
}

// 6ֹͣ��ϴ(ֹͣ������ͣ��)
DWORD CNanhuaNha503::StopPump(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(6, pbResult);
}

// 7��©
DWORD CNanhuaNha503::CheckLeak(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(7, pbResult);
}

// 8����
DWORD CNanhuaNha503::Zero(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(8, pbResult);
}

// 9д��У׼��Ũ��
DWORD CNanhuaNha503::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
	const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	return CmdFormat_Return06_00_01(9, pbResult, usCalC3H8, fCalCO, fCalCO2, usCalNO);
}

// 10У׼
DWORD CNanhuaNha503::GasCalibration(BYTE *const pbResult, const int nChanel, 
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

// 11д������Ũ��
DWORD CNanhuaNha503::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return06_00_01(11, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12�Զ����
DWORD CNanhuaNha503::GasCheck(BYTE *const pbResult, 
						short *const psHC, 
						float *const pfCO, 
						float *const pfCO2, 
						float *const pfO2, 
						short *const psNO, 
						float *const pfPef)
{
	return CmdFormat_Return00_01_05(12, pbResult, psHC, pfCO, pfCO2, pfO2, psNO, pfPef);
}

// 13ȡPEF
DWORD CNanhuaNha503::GetPef(float *const pfPef)
{
	assert(NULL != pfPef);

	BYTE bWriteBuf(13);
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 2, false ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// ����Ϊ�����߸���,���shortǿ��ת��
		*pfPef = (short)(MAKEWORD(bReadBuf[1], bReadBuf[0])) / 1000.0f;
	}
	else
	{
		// ��������

		*pfPef = 0.0f;
	}
	return dw;
}

// 14���NO�ϻ���־
// 503����
DWORD CNanhuaNha503::ClearNoSign(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(14, pbResult);
}

// 15���õ�������Ϊ����
DWORD CNanhuaNha503::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(15, pbResult);
}

// 16���õ�������Ϊ����
DWORD CNanhuaNha503::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(16, pbResult);
}

// 17���ó��Ϊ4
DWORD CNanhuaNha503::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(17, pbResult);
}

// 18���ó��Ϊ2
DWORD CNanhuaNha503::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(18, pbResult);
}

// 19���õ��ʽΪ����
DWORD CNanhuaNha503::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(19, pbResult);
}

// 20���õ��ʽΪ����
DWORD CNanhuaNha503::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(20, pbResult);
}

// 21�����Զ�����
DWORD CNanhuaNha503::EnableAutoZero(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(21, pbResult);
}

// 22��ֹ�Զ�����
DWORD CNanhuaNha503::DisableAutoZero(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(22, pbResult);
}

// 23������̲���

// 24��ֹ���̲���

// 25LCD��

// 26LCD��

// 27�����¶�У׼
DWORD CNanhuaNha503::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	assert(fCalET>-20.0f && fCalET<150.0f);

	return CmdFormat_Return06_00_01(27, pbResult, fCalET);
}

// 28����У׼

// 29����У׼
DWORD CNanhuaNha503::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	assert(fCalOT>-20.0f && fCalOT<150.0f);

	return CmdFormat_Return06_00_01(29, pbResult, fCalOT);
}

// 30����ʪ��У׼
DWORD CNanhuaNha503::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	assert(fCalRH>0.0f && fCalRH<101.0f);

	return CmdFormat_Return06_00_01(30, pbResult, fCalRH);
}

// 31����ѹ��У׼
DWORD CNanhuaNha503::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	assert(fCalAP>0.0f && fCalAP<150.0f);

	return CmdFormat_Return06_00_01(31, pbResult, fCalAP);
}

// 32�������
DWORD CNanhuaNha503::OpenCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(32, pbResult);
}

// 33�������
DWORD CNanhuaNha503::CloseCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(33, pbResult);
}

// 34ȡ����״̬
DWORD CNanhuaNha503::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf(38);
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		*pbStatus = bReadBuf;
	}
	else
	{
		*pbStatus = 0xFF;
	}

	return dw;
}

// 35ȡԤ��ʣ��ʱ��
DWORD CNanhuaNha503::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	assert(NULL != pusSecondsLeft);

	BYTE bWriteBuf(39);
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 2, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		*pusSecondsLeft = MAKEWORD(bReadBuf[1], bReadBuf[0]);
	}

	return dw;
}

// 36����ȼ������Ϊ����
DWORD CNanhuaNha503::SetFuelTypeToGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(40, pbResult);
}


// 37����ȼ������ΪҺ��ʯ����
DWORD CNanhuaNha503::SetFuelTypeToLPG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(41, pbResult);
}

// 38����ȼ������Ϊ��Ȼ��
DWORD CNanhuaNha503::SetFuelTypeToNG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(42, pbResult);
}

// 39����ȼ������Ϊ�Ҵ�����
DWORD CNanhuaNha503::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(43, pbResult);
}

DWORD CNanhuaNha503::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
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

DWORD CNanhuaNha503::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct, 
		float *const pfCOct, 
		float *const pfCO2ct, 
		short *const psNOct, 
		float *const pfDilO2, 
		float *const pfBenchO2, 
		short *const psGasTemp, 
		float *const pfGasFluxStd0, 
		float *const pfGasFluxStd2, 
		float *const pfExhaustFluxStd0, 
		float *const pfExhaustFluxStd20, 
		float *const pfGasPres, 
		float *const pfGasFluxAct, 
		USHORT *const pusRpm,
		WORD *const pwMiniStatus,
		const BYTE bDataMode,
		BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
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

//DWORD CNanhuaNha503::GetOperationModeUsedNow(void) const
//{
//	return ToAnalyzer;
//}

DWORD CNanhuaNha503::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::SatrtGetEnvironmentO2_Flowmeter(void)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::GetEnvironmentO2_Flowmeter(BYTE *const pbResult,float *const pfEnvO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha503::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// ��ȡ�ֶ���������
DWORD CNanhuaNha503::GetTestResultData(short *const psHCHigh/*=NULL*/, 
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
	BYTE bWriteBuf(0x88), bReadBuf[35] = {0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 35, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) HCHigh
		if (NULL != psHCHigh)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psHCHigh = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
		}
		// (2) COHigh
		if (NULL != pfCOHigh)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCOHigh = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (3) CO2High
		if (NULL != pfCO2High)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO2High = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (4) O2High
		if (NULL != pfO2High)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfO2High = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
		}
		// (5) NOHigh
		if (NULL != psNOHigh)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNOHigh = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		// (6) RPMHigh
		if (NULL != pusRpmHigh)
		{
			*pusRpmHigh = MAKEWORD(bReadBuf[12], bReadBuf[11]);
		}
		// (7) OilTempHigh
		if (NULL != pfOilTempHigh)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfOilTempHigh = (short)MAKEWORD(bReadBuf[14], bReadBuf[13]) / 10.0f;
		}
		// (8) LambdaHigh
		if (NULL != pfLambdaHigh)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfLambdaHigh = (short)(MAKEWORD(bReadBuf[16], bReadBuf[15])) / 100.0f;
		}
		// (9) HCLow
		if (NULL != psHCLow)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psHCLow = (short)(MAKEWORD(bReadBuf[18], bReadBuf[17]));
		}
		// (10) COLow
		if (NULL != pfCOLow)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCOLow = (short)(MAKEWORD(bReadBuf[20], bReadBuf[19])) / 100.0f;
		}
		// (11) CO2Low
		if (NULL != pfCO2Low)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO2Low = (short)(MAKEWORD(bReadBuf[22], bReadBuf[21])) / 100.0f;
		}
		// (12) O2Low
		if (NULL != pfO2Low)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfO2Low = (short)(MAKEWORD(bReadBuf[24], bReadBuf[23])) / 100.0f;
		}
		// (13) NOLow
		if (NULL != psNOLow)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNOLow = (short)(MAKEWORD(bReadBuf[26], bReadBuf[25]));
		}
		// (14) RPMLow
		if (NULL != pusRpmLow)
		{
			*pusRpmLow = MAKEWORD(bReadBuf[28], bReadBuf[27]);
		}
		// (15) OilTempLow
		if (NULL != pfOilTempLow)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfOilTempLow = (short)MAKEWORD(bReadBuf[30], bReadBuf[29]) / 10.0f;
		}
		// (16) LambdaLow
		if (NULL != pfLambdaLow)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfLambdaLow = (short)(MAKEWORD(bReadBuf[32], bReadBuf[31])) / 100.0f;
		}

		// ����ȡֵ��Χ
#ifdef ANALYZER_DATA_RANGE
		// HCHigh
		if (*psHCHigh < ANALYZER_HC_LOWER_LIMIT)
		{
			*psHCHigh = ANALYZER_HC_LOWER_LIMIT;
		}
		else if (*psHCHigh > ANALYZER_HC_UPPER_LIMIT)
		{
			*psHCHigh = ANALYZER_HC_UPPER_LIMIT;
		}
		// COHigh
		if (*pfCOHigh < ANALYZER_CO_LOWER_LIMIT)
		{
			*pfCOHigh = ANALYZER_CO_LOWER_LIMIT;
		}
		else if (*pfCOHigh > ANALYZER_CO_UPPER_LIMIT)
		{
			*pfCOHigh = ANALYZER_CO_UPPER_LIMIT;
		}
		// CO2High
		if (*pfCO2High < ANALYZER_CO2_LOWER_LIMIT)
		{
			*pfCO2High = ANALYZER_CO2_LOWER_LIMIT;
		}
		else if (*pfCO2High > ANALYZER_CO2_UPPER_LIMIT)
		{
			*pfCO2High = ANALYZER_CO2_UPPER_LIMIT;
		}
		 // O2High
		if (*pfO2High < ANALYZER_O2_LOWER_LIMIT)
		{
			*pfO2High = ANALYZER_O2_LOWER_LIMIT;
		}
		else if (*pfO2High > ANALYZER_O2_UPPER_LIMIT)
		{
			*pfO2High = ANALYZER_O2_UPPER_LIMIT;
		}
		 // NOHigh
		if (*psNOHigh < ANALYZER_NO_LOWER_LIMIT)
		{
			*psNOHigh = ANALYZER_NO_LOWER_LIMIT;
		}
		else if (*psNOHigh > ANALYZER_NO_UPPER_LIMIT)
		{
			*psNOHigh = ANALYZER_NO_UPPER_LIMIT;
		}
		 // RPMHigh
		if (*pusRpmHigh > ANALYZER_RPM_UPPER_LIMIT)
		{
			*pusRpmHigh = ANALYZER_RPM_UPPER_LIMIT;
		}
		 // OilTempHigh
		if (*pfOilTempHigh < ANALYZER_OILTEMP_LOWER_LIMIT)
		{
			*pfOilTempHigh = ANALYZER_OILTEMP_LOWER_LIMIT;
		}
		else if (*pfOilTempHigh > ANALYZER_OILTEMP_UPPER_LIMIT)
		{
			*pfOilTempHigh = ANALYZER_OILTEMP_UPPER_LIMIT;
		}
		 // LambdaHigh
		if (*pfLambdaHigh > ANALYZER_LAMBDA_LOWER_LIMIT)
		{
			*pfLambdaHigh = ANALYZER_LAMBDA_LOWER_LIMIT;
		}
		else if (*pfLambdaHigh > ANALYZER_LAMBDA_UPPER_LIMIT)
		{
			*pfLambdaHigh = ANALYZER_LAMBDA_UPPER_LIMIT;
		}
		// HCLow
		if (*psHCLow < ANALYZER_HC_LOWER_LIMIT)
		{
			*psHCLow = ANALYZER_HC_LOWER_LIMIT;
		}
		else if (*psHCLow > ANALYZER_HC_UPPER_LIMIT)
		{
			*psHCLow = ANALYZER_HC_UPPER_LIMIT;
		}
		// COLow
		if (*pfCOLow < ANALYZER_CO_LOWER_LIMIT)
		{
			*pfCOLow = ANALYZER_CO_LOWER_LIMIT;
		}
		else if (*pfCOLow > ANALYZER_CO_UPPER_LIMIT)
		{
			*pfCOLow = ANALYZER_CO_UPPER_LIMIT;
		}
		// CO2Low
		if (*pfCO2Low < ANALYZER_CO2_LOWER_LIMIT)
		{
			*pfCO2Low = ANALYZER_CO2_LOWER_LIMIT;
		}
		else if (*pfCO2Low > ANALYZER_CO2_UPPER_LIMIT)
		{
			*pfCO2Low = ANALYZER_CO2_UPPER_LIMIT;
		}
		 // O2Low
		if (*pfO2Low < ANALYZER_O2_LOWER_LIMIT)
		{
			*pfO2Low = ANALYZER_O2_LOWER_LIMIT;
		}
		else if (*pfO2Low > ANALYZER_O2_UPPER_LIMIT)
		{
			*pfO2Low = ANALYZER_O2_UPPER_LIMIT;
		}
		 // NOLow
		if (*psNOLow < ANALYZER_NO_LOWER_LIMIT)
		{
			*psNOLow = ANALYZER_NO_LOWER_LIMIT;
		}
		else if (*psNOLow > ANALYZER_NO_UPPER_LIMIT)
		{
			*psNOLow = ANALYZER_NO_UPPER_LIMIT;
		}
		 // RPMLow
		if (*pusRpmLow > ANALYZER_RPM_UPPER_LIMIT)
		{
			*pusRpmLow = ANALYZER_RPM_UPPER_LIMIT;
		}
		 // OilTempLow
		if (*pfOilTempLow < ANALYZER_OILTEMP_LOWER_LIMIT)
		{
			*pfOilTempLow = ANALYZER_OILTEMP_LOWER_LIMIT;
		}
		else if (*pfOilTempLow > ANALYZER_OILTEMP_UPPER_LIMIT)
		{
			*pfOilTempLow = ANALYZER_OILTEMP_UPPER_LIMIT;
		}
		 // LambdaLow
		if (*pfLambdaLow > ANALYZER_LAMBDA_LOWER_LIMIT)
		{
			*pfLambdaLow = ANALYZER_LAMBDA_LOWER_LIMIT;
		}
		else if (*pfLambdaLow > ANALYZER_LAMBDA_UPPER_LIMIT)
		{
			*pfLambdaLow = ANALYZER_LAMBDA_UPPER_LIMIT;
		}
#endif
	}
	else
	{
		// (1) HCHigh
		if (NULL != psHCHigh)
		{
			*psHCHigh = 0;
		}
		// (2) COHigh
		if (NULL != pfCOHigh)
		{
			*pfCOHigh = 0;
		}
		// (3) CO2High
		if (NULL != pfCO2High)
		{
			*pfCO2High = 0;
		}
		// (4) O2High
		if (NULL != pfO2High)
		{
			*pfO2High = 0;
		}
		// (5) NOHigh
		if (NULL != psNOHigh)
		{
			*psNOHigh = 0;
		}
		// (6) RPMHigh
		if (NULL != pusRpmHigh)
		{
			*pusRpmHigh = 0;
		}
		// (7) OilTempHigh
		if (NULL != pfOilTempHigh)
		{
			*pfOilTempHigh = 0;
		}
		// (8) LambdaHigh
		if (NULL != pfLambdaHigh)
		{
			*pfLambdaHigh = 0;
		}
		// (9) HCLow
		if (NULL != psHCLow)
		{
			*psHCLow = 0;
		}
		// (10) COLow
		if (NULL != pfCOLow)
		{
			*pfCOLow = 0;
		}
		// (11) CO2Low
		if (NULL != pfCO2Low)
		{
			*pfCO2Low = 0;
		}
		// (12) O2Low
		if (NULL != pfO2Low)
		{
			*pfO2Low = 0;
		}
		// (13) NOLow
		if (NULL != psNOLow)
		{
			*psNOLow = 0;
		}
		// (14) RPMLow
		if (NULL != pusRpmLow)
		{
			*pusRpmLow = 0;
		}
		// (15) OilTempLow
		if (NULL != pfOilTempLow)
		{
			*pfOilTempLow = 0;
		}
		// (16) LambdaLow
		if (NULL != pfLambdaLow)
		{
			*pfLambdaLow = 0;
		}
	}
	return dw;
}