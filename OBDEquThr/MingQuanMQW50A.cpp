#include "StdAfx.h"
#include "MingQuanMQW50A.h"

#include <stdlib.h>
#include <cassert>
#include <cstdio>

CMingQuanMQW50A::CMingQuanMQW50A(void)
//: m_hComm(INVALID_HANDLE_VALUE)
//, m_opUsedNow(ToAnalyzer)
{
	::InitializeCriticalSection(&cInfoSection);
}

CMingQuanMQW50A::~CMingQuanMQW50A(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CMingQuanMQW50A::Open(BYTE bPort)
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
DWORD CMingQuanMQW50A::Close(void)
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

bool CMingQuanMQW50A::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CMingQuanMQW50A::WriteAndReadData(BYTE *const pbWriteBuffer, 
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

BYTE CMingQuanMQW50A::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	
	return (~bSum+1);
}

DWORD CMingQuanMQW50A::WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
												 const DWORD dwWriteLength, 
												 BYTE *const pbReadBuffer, 
												 const DWORD dwReadLength, 
												 const bool bNeedWriteChkSum, 
												 const bool bNeedReadChkSum)
{
	assert(NULL!=pbWriteBuffer);
	assert(1UL<dwWriteLength);
	assert(NULL!=pbReadBuffer);
	assert(1UL<dwReadLength);

	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		// ���ò���մ���
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			// ����дУ��
			WORD wSum = CheckSum_Flowmeter(pbWriteBuffer, dwWriteLength-2);
			pbWriteBuffer[dwWriteLength-2] = LOBYTE(wSum);
			pbWriteBuffer[dwWriteLength-1] = HIBYTE(wSum);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// �ֶ��ζ���
		// ��һ������ͷ��4byte��
		// �ڶ��ζ�ʣ�µ���Ч����(��Ч���ݸ��ݵ�һ�ζ����������ݲ�ͬ����ͬ)

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 3, &dwBytesRead, NULL);
		if (bSucceed && 3==dwBytesRead)
		{
#ifdef _DEBUG
			SYSTEMTIME st;
			GetSystemTime(&st);
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"�����Ʒ���:%d\r\n",pbReadBuffer[1]);
			OutputDebugString(wchOutPutDebugString);
#endif	
			// ����F��ͨѶЭ���ж�Reponse mode:
			// ACK  (0x06): command received OK and completely issued
			// BUSY (0x05): command received OK but process is still pending. Use process status command to determine when command is done
			// NAK  (0x15): transmission error or command error
			// assert(0x06==pbReadBuffer[1] || 0x05==pbReadBuffer[1] || 0x15==pbReadBuffer[1]);
			if (0x06 == pbReadBuffer[0])
			{
				// �����������Ѿ����ܣ������Ѿ����

				// TODO:��ʱ����
				// ���ݷ��ص����ݳ����ж�VMAS�ڲ�����汾�Ƿ��ӦF��ͨѶЭ��˵����
				//assert(MAKEWORD(pbReadBuffer[2],pbReadBuffer[3]) == (WORD)((dwReadLength-6)/2));

				// �����µ�����
				bSucceed = ReadFile(m_hComm, pbReadBuffer+3, dwReadLength-3, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-3))
				{
					if (bNeedReadChkSum)
					{
						// ��Ҫ��У��
						WORD wSum = CheckSum_Flowmeter(pbReadBuffer, dwReadLength-2);
						if (wSum != MAKEWORD(pbReadBuffer[dwReadLength-2], pbReadBuffer[dwReadLength-1]))
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

					// �������󣺶�ָ������ʧ�ܣ���������Ч����ʧ�ܣ�
					return ANA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else if (0x15 == pbReadBuffer[0])
			{
				// �����������Ѿ����ܣ�������ִ��
				// ���������������
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);

				// �����ɹ�
				return ANA_WR_SUCCEED;
			}
			else
			{
				// �������������
				// ���������������
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);

				// ���������������ش�����
				return ANA_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);

			// �������󣺶�ָ������ʧ�ܣ�������ͷʧ�ܣ�
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

WORD CMingQuanMQW50A::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);

	WORD wSum(0);
	for (UINT i=0; i<un; i++)
	{
		wSum += pb[i];
	}
	return wSum;
}

DWORD CMingQuanMQW50A::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
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

// �ʺ��ڷ��ĸ�ָ��  ���ĸ�ָ���
DWORD CMingQuanMQW50A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = bCmd;
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if(dw == ANA_WR_SUCCEED)
	{
		ret = bReadBuf[0];
	}
	else
	{
		ret = 0x15;
	}
	return ret;
}

// �ʺ��ڷ����ָ��  ���ĸ�ָ���   �ҵ��ĸ�Ϊѡ��ָ���
DWORD CMingQuanMQW50A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult,const BYTE bType)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[5] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = bCmd;
	bWriteBuf[2] = 0x04;
	bWriteBuf[3] = bType;
	bWriteBuf[4] = 0x00;
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if(dw == ANA_WR_SUCCEED)
	{
		ret = bReadBuf[0];
	}
	else
	{
		ret = 0x15;
	}
	return ret;
}

DWORD CMingQuanMQW50A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[6] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = bCmd;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[5] = 0x00;
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if(dw == ANA_WR_SUCCEED)
	{
		ret = bReadBuf[0];
	}
	else
	{
		ret = 0x15;
	}
	return ret;
}


// 1ȡ������
DWORD CMingQuanMQW50A::GetMajorData(short *const psHC, 
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
	return GetMajorData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx, pusRpm, pfOilTemp, pusFlux, pfLambda, pbO2Sensor, pbNOSensor, NULL);
}

DWORD CMingQuanMQW50A::GetMajorData(short *const psHC, 
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
	BYTE bWriteBuf[4] = {0x02, 0x60, 0x03, 0x9B};
	BYTE bReadBuf[28];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) CO2
		if (NULL != pfCO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO2 = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfCO = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (3) HC
		if (NULL != psHC)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psHC = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7]));
		}
		// (4) NO
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
		// (5) O2
		if (NULL != pfO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfO2 = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11])) / 100.0f;
		}
		// (6) OilTemp
		if (NULL != pfOilTemp)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfOilTemp = (short)(MAKEWORD(bReadBuf[16], bReadBuf[15]))/ 10.0f;
		}
		// (7) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[20], bReadBuf[19]);
		}
		// Lambda
		if (NULL != pfLambda)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfLambda = (short)(MAKEWORD(bReadBuf[24], bReadBuf[23])) / 1000.0f;
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
		//// ��������
	}
	return dw;
}

// 2ȡ��������
// ET:Environment Temperature��д
// AP:Atmospheric Pressure��д
// RH:Relative Humidity��д
DWORD CMingQuanMQW50A::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	assert(NULL != pfET);
	assert(NULL != pfAP);
	assert(NULL != pfRH);
	
	BYTE bWriteBuf[4] = {0x02, 0x60, 0x03, 0x9B};
	BYTE bReadBuf[28];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 28, false, true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) ET�������¶�
		// ����Ϊ�����߸���,���shortǿ��ת��
		*pfET = (short)(MAKEWORD(bReadBuf[18], bReadBuf[17])) / 10.0f;
		// (2) AP������ѹ��
		// ����Ϊ�����߸���,���shortǿ��ת��
		*pfAP = (short)(MAKEWORD(bReadBuf[26], bReadBuf[25])) / 10.0f;
		// (3) RH�����ʪ��
		*pfRH = (short)(MAKEWORD(bReadBuf[14], bReadBuf[13])) / 10.0f;
	}
	else
	{
		//// ��������

		//// (1) ET�������¶�
		//*pfET = 0.0f;
		//// (2) AP������ѹ��
		//*pfAP = 0.0f;
		//// (3) RH�����ʪ��
		//*pfRH = 0.0f;
	}
	return dw;
}

// 3�����
DWORD CMingQuanMQW50A::DeflateEnvAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x7C, 0x03, 0x7F};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 4��ܵ���(����,������̽ͷ����)
DWORD CMingQuanMQW50A::DeflatePipeAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x7B, 0x03, 0x80};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 5��ϴ�ܵ�(����)
DWORD CMingQuanMQW50A::InverseBlow(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x80, 0x03, 0x7B};
	if (pbResult != NULL)
	{
		if (*pbResult == 0x01)
		{
			bWriteBuf[0] = 0x02;
			bWriteBuf[1] = 0x65;
			bWriteBuf[2] = 0x03;
			bWriteBuf[3] = 0x96;
		}
	}
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 6ֹͣ��ϴ(ֹͣ������ͣ��)
DWORD CMingQuanMQW50A::StopPump(BYTE *const pbResult)
{
	BYTE bWriteBuf[4] = {0x02, 0x78, 0x03, 0x83};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 7��©
DWORD CMingQuanMQW50A::CheckLeak(BYTE *const pbResult)
{
	DWORD dw(0x00);
	if (pbResult != NULL)
	{
		// 0x01ָʾ���㣬�����ѯ����״̬
		if (*pbResult == 0x01)
		{
			BYTE bWriteBuf[4] = {0x02, 0x66, 0x03, 0x95};
			BYTE bReadBuf[4];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));

			dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
			if (ANA_WR_SUCCEED == dw)
			{
				if (bReadBuf[0] == 0x06)
				{
					dw = ANA_WR_SUCCEED;

					if (pbResult != NULL)
					{
						*pbResult = 0x06;
					}
				}
				else
				{
					dw = ANA_WR_CHECK_SUM_FAIL;
				}
			}
			else
			{
				//// ��������
				dw = ANA_WR_EQUIP_RETURE_0X15;
			}
			return dw;
		}
	}

	BYTE bWriteBufStatus[] = {0x02, 0x61, 0x03, 0x9A};
	BYTE bReadBufStatus[11];
	ZeroMemory(bReadBufStatus, sizeof(bReadBufStatus));
	
	if (pbResult != NULL)
	{
		*pbResult = 0xFF;
	}
	dw = WriteAndReadData(bWriteBufStatus, sizeof(bWriteBufStatus), bReadBufStatus, sizeof(bReadBufStatus), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBufStatus[0] == 0x06)
		{
			BYTE b(0x00);
			if (bReadBufStatus[4]&0x80)
			{
				if (bReadBufStatus[3]&0x10)
				{
					// ���ڼ�©
					b = 0x04;
				}
				else
				{
					b = 0xFF;
				}
			}
			else
			{
				if (bReadBufStatus[5]&0x01)
				{
					b = 0x01;
				}
				else
				{
					b = 0x00;
				}
			}
			if (pbResult != NULL)
			{
				*pbResult = b;
			}
		}
	}

	return dw;
}

// 8����
DWORD CMingQuanMQW50A::Zero(BYTE *const pbResult)
{
	DWORD dw(0x00);
	if (pbResult != NULL)
	{
		// 0x01ָʾ���㣬�����ѯ����״̬
		if (*pbResult == 0x01)
		{
			BYTE bWriteBuf[4] = {0x02, 0x67, 0x03, 0x94};
			BYTE bReadBuf[4];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));

			dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
			if (ANA_WR_SUCCEED == dw)
			{
				if (bReadBuf[0] == 0x06)
				{
					dw = ANA_WR_SUCCEED;

					if (pbResult != NULL)
					{
						*pbResult = 0x06;
					}
				}
				else
				{
					dw = ANA_WR_CHECK_SUM_FAIL;
				}
			}
			else
			{
				//// ��������
				dw = ANA_WR_EQUIP_RETURE_0X15;
			}
			return dw;
		}
	}

	BYTE bWriteBufStatus[] = {0x02, 0x61, 0x03, 0x9A};
	BYTE bReadBufStatus[11];
	ZeroMemory(bReadBufStatus, sizeof(bReadBufStatus));
	
	if (pbResult != NULL)
	{
		*pbResult = 0xFF;
	}
	dw = WriteAndReadData(bWriteBufStatus, sizeof(bWriteBufStatus), bReadBufStatus, sizeof(bReadBufStatus), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBufStatus[0] == 0x06)
		{
			BYTE b(0x00);
			if (bReadBufStatus[4]&0x80)
			{
				if (bReadBufStatus[3]&0x40)
				{
					// ���ڵ���
					b = 0x05;
				}
				else
				{
					b = 0xFF;
				}
			}
			else
			{
				if (bReadBufStatus[5]&0x04)
				{
					b = 0x01;
				}
				else
				{
					b = 0x00;
				}
			}
			if (pbResult != NULL)
			{
				*pbResult = b;
			}
		}
	}

	return dw;
}

// 9д��У׼��Ũ��
DWORD CMingQuanMQW50A::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
		const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	return CmdFormat_Return00_01(9, pbResult, usCalC3H8, fCalCO, fCalCO2, usCalNO);
}

// 10У׼
DWORD CMingQuanMQW50A::GasCalibration(BYTE *const pbResult, const int nChanel, 
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
DWORD CMingQuanMQW50A::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return00_01(36, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12�Զ����
DWORD CMingQuanMQW50A::GasCheck(BYTE *const pbResult, 
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
DWORD CMingQuanMQW50A::GetPef(float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 14���NO�ϻ���־
// 503����
DWORD CMingQuanMQW50A::ClearNoSign(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 15���õ�������Ϊ����
DWORD CMingQuanMQW50A::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x68, 0x04, 0x00, 0x93};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 16���õ�������Ϊ����
DWORD CMingQuanMQW50A::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x68, 0x04, 0x01, 0x92};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 17���ó��Ϊ4
DWORD CMingQuanMQW50A::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6E, pbResult,0x01);
}

// 18���ó��Ϊ2
DWORD CMingQuanMQW50A::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6E, pbResult,0x00);
}
// 19���õ��ʽΪ����
DWORD CMingQuanMQW50A::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6D, pbResult,0x00);
}

// 20���õ��ʽΪ����
DWORD CMingQuanMQW50A::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6D, pbResult,0x01);
}

// 21�����Զ�����
DWORD CMingQuanMQW50A::EnableAutoZero(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x76, 0x04, 0x00, 0x84};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 22��ֹ�Զ�����
DWORD CMingQuanMQW50A::DisableAutoZero(BYTE *const pbResult)
{
	BYTE bWriteBuf[5] = {0x02, 0x76, 0x04, 0x01, 0x83};
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 23������̲���
DWORD CMingQuanMQW50A::EnableKeyOperate(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6A, pbResult);
}
// 24��ֹ���̲���
DWORD CMingQuanMQW50A::DisableKeyOperate(BYTE *const pbResult) 
{
	return CmdFormat_Return00_01_05(0x6B, pbResult);
}

// 25LCD��

// 26LCD��

// 27�����¶�У׼
DWORD CMingQuanMQW50A::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	//assert(fCalET>-20.0f && fCalET<150.0f);

	//return CmdFormat_Return00_01_02(27, pbResult, fCalET);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x74; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalET * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalET * 10.0f));
	bWriteBuf[5] = 0x00; // У��ͺ�
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 28����У׼

// 29����У׼
DWORD CMingQuanMQW50A::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	//assert(fCalOT>-20.0f && fCalOT<150.0f);

	//return CmdFormat_Return00_01_02(29, pbResult, fCalOT);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x73; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalOT * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalOT * 10.0f));
	bWriteBuf[5] = 0x00; // У��ͺ�
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 30����ʪ��У׼
DWORD CMingQuanMQW50A::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	//assert(fCalRH>0.0f && fCalRH<101.0f);

	//return CmdFormat_Return00_01_02(30, pbResult, fCalRH);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x75; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalRH * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalRH * 10.0f));
	bWriteBuf[5] = 0x00; // У��ͺ�
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 31����ѹ��У׼
DWORD CMingQuanMQW50A::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	//assert(fCalAP>0.0f && fCalAP<150.0f);

	//return CmdFormat_Return00_01_02(31, pbResult, fCalAP);
	BYTE bWriteBuf[6];
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x72; // cmd
	bWriteBuf[2] = 0x05; // len
	bWriteBuf[3] = HIBYTE((WORD)(fCalAP * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalAP * 10.0f));
	bWriteBuf[5] = 0x00; // У��ͺ�
	BYTE bReadBuf[4];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 32�������
DWORD CMingQuanMQW50A::OpenCheckGas(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 33�������
DWORD CMingQuanMQW50A::CloseCheckGas(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 34ȡ����״̬
DWORD CMingQuanMQW50A::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[] = {0x02,0x61,0x03,0x9A};
	BYTE bReadBuf[11];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if(bReadBuf[0] == 0x06)
		{
			BYTE b(0x00);
			if (bReadBuf[4]&0x80)
			{
				if(bReadBuf[3]&0x01)
				{
					// ����Ԥ��
					b = 0x02;
				}
				else if(bReadBuf[3]&0x40)
				{
					// ���ڵ���
					b = 0x05;
				}
				else if(bReadBuf[3] == 0x10)
				{
					// �����ܷ��Լ��
					b = 0x13;
				}
				else
				{
					b = 0xFF;
				}
			}
			else
			{
				// ����
				b = 0x00;
			}
			*pbStatus = b;
		}
	}

	return ANA_WR_SUCCEED;
}

// 35ȡԤ��ʣ��ʱ��
DWORD CMingQuanMQW50A::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 36����ȼ������Ϊ����
DWORD CMingQuanMQW50A::SetFuelTypeToGas(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult,0x00);
}


// 37����ȼ������ΪҺ��ʯ����
DWORD CMingQuanMQW50A::SetFuelTypeToLPG(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult,0x01);
}

// 38����ȼ������Ϊ��Ȼ��
DWORD CMingQuanMQW50A::SetFuelTypeToNG(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult,0x02);
}

// 39����ȼ������Ϊ�Ҵ�����
DWORD CMingQuanMQW50A::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x6C, pbResult);
}

DWORD CMingQuanMQW50A::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
	if (pbResult != NULL)
	{
		*pbResult = 0x06;
	}
	return ANA_WR_SUCCEED;
}


DWORD CMingQuanMQW50A::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
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

//DWORD CMingQuanMQW50A::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
//		const WORD wCntLength,
//		short *const psHCct=NULL, 
//		float *const pfCOct=NULL, 
//		float *const pfCO2ct=NULL, 
//		short *const psNOct=NULL, 
//		float *const pfDilO2=NULL, 
//		float *const pfBenchO2=NULL, 
//		short *const psGasTemp=NULL, 
//		float *const pfGasFluxStd0=NULL, 
//		float *const pfGasFluxStd20=NULL, 
//		float *const pfExhaustFluxStd0=NULL, 
//		float *const pfExhaustFluxStd20=NULL, 
//		float *const pfGasPres=NULL, 
//		float *const pfGasFluxAct=NULL, 
//		USHORT *const pusRpm=NULL,
//		WORD *const pwMiniStatus=NULL,
//		const BYTE bDataMode=0x01,
//		BYTE *const pbResult=NULL)
//{
//	return ANA_WR_EQUIP_RETURE_0X15;
//}

DWORD CMingQuanMQW50A::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
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

DWORD CMingQuanMQW50A::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

//  �Ǳ�׼״̬��ȫ������
DWORD CMingQuanMQW50A::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, 
								float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, 
								float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	BYTE ret(0x00);

	BYTE bWriteBuf[4] = {0};
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x86;
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x75;
	BYTE bReadBuf[14];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[0];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		float fFluxAct = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9])) / 6.0f;
		if (NULL != pfFluxAct)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfFluxAct = fFluxAct;
		}
		float fPres = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) * 0.1f;
		if (NULL != pfPres)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfPres = fPres;
		}
		float fUDilO2 = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) * 0.01f;
		if (NULL != pfUDilO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfUDilO2 = fUDilO2;
		}
		float fGasTemp = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) * 0.1f;
		if (NULL != pfGasTemp)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfGasTemp = fGasTemp;
		}
		if (NULL != pfFluxStd0)
		{
			*pfFluxStd0 = ActFlux2StdFlux0(fFluxAct, fGasTemp, fPres);
		}
		if (NULL != pfFluxStd20)
		{
			*pfFluxStd20 = ActFlux2StdFlux20(fFluxAct, fGasTemp, fPres);
		}
	}
	else
	{
		//// ��������
	}
	return dw;
}

// ��׼״̬��ȫ������
//DWORD CMingQuanMQW50A::GetRealTimeData_Flowmeter(float *const pfFlux, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
//{
//	BYTE ret(0x00);
//
//	BYTE bWriteBuf[4] = {0};
//	bWriteBuf[0] = 0x02;
//	bWriteBuf[1] = 0x87;
//	bWriteBuf[2] = 0x03;
//	bWriteBuf[3] = 0x75;
//	BYTE bReadBuf[14];
//	ZeroMemory(bReadBuf, sizeof(bReadBuf));
//	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
//	if (ANA_WR_SUCCEED == dw)
//	{
//		// (1) HC
//		if (NULL != pfFlux)
//		{
//			// ����Ϊ�����߸���,���shortǿ��ת��
//			*pfFlux = (short)(MAKEWORD(bReadBuf[7], bReadBuf[8]));
//		}
//		// (2) CO
//		if (NULL != pfPres)
//		{
//			// ����Ϊ�����߸���,���shortǿ��ת��
//			*pfPres = (short)(MAKEWORD(bReadBuf[5], bReadBuf[6])) / 100.0f;
//		}
//		// (3) CO2
//		if (NULL != pfUDilO2)
//		{
//			// ����Ϊ�����߸���,���shortǿ��ת��
//			*pfUDilO2 = (short)(MAKEWORD(bReadBuf[3], bReadBuf[4])) / 100.0f;
//		}
//		// (4) O2
//		if (NULL != pfGasTemp)
//		{
//			// ����Ϊ�����߸���,���shortǿ��ת��
//			*pfGasTemp = (short)(MAKEWORD(bReadBuf[11], bReadBuf[12])) / 100.0f;
//		}
//	}
//	else
//	{
//		//// ��������
//	}
//	return bReadBuf[0];
//}

DWORD CMingQuanMQW50A::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};
	if( cm == HighFlowCal)
	{
		bWriteBuf[0] = 0x02;
		bWriteBuf[1] = 0xA6;
		bWriteBuf[2] = 0x05;
		bWriteBuf[3] = HIBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[4] = LOBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[5] = 0x00;
	}
	else if( cm == LowFlowCal)
	{
		bWriteBuf[0] = 0x02;
		bWriteBuf[1] = 0xA7;
		bWriteBuf[2] = 0x05;
		bWriteBuf[3] = HIBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[4] = LOBYTE((WORD)(fCalFlux * 10.0f));
		bWriteBuf[5] = 0x00;
	}
	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf == 0x00)
		{
			ret = 0x00;
		}
	}
	else
	{
		ret = 0x01;
	}
	return ret;
}

DWORD CMingQuanMQW50A::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};

	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0xAA;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fCalPres * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalPres * 10.0f));
	bWriteBuf[5] = 0x00;

	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf[0] == 0x00)
		{
			ret = 0x00;
		}
	}
	else
	{
		ret = 0x01;
	}
	return ret;
}

DWORD CMingQuanMQW50A::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	return SetCalibrationDilO2_Flowmeter_High(pbResult, fCalDilO2);
}

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	if (NULL != pbResult)
	{
		*pbResult = 0x06;
	}
	if (NULL != pbCalResult)
	{
		*pbCalResult = 0x00;
	}
	return ANA_WR_SUCCEED;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};

	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0xA2;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[5] = 0x00;

	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf[0] == 0x00)
		{
			ret = 0x00;
		}
	}
	else
	{
		ret = 0x01;
	}
	return ret;
}

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	BYTE ret(0x00);
	BYTE bWriteBuf[6] = {0};

	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0xA3;
	bWriteBuf[2] = 0x05;
	bWriteBuf[3] = HIBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[4] = LOBYTE((WORD)(fCalDilO2 * 10.0f));
	bWriteBuf[5] = 0x00;

	BYTE bReadBuf[4];
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if(dw == ANA_WR_SUCCEED)
	{
		if(bReadBuf[0] == 0x00)
		{
			ret = 0x00;
		}
	}
	else
	{
		ret = 0x01;
	}
	return ret;
}

DWORD CMingQuanMQW50A::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::SatrtGetEnvironmentO2_Flowmeter(void)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQW50A::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
	if (pbResult != NULL)
	{
		*pbResult = 0x06;
	}
	return ANA_WR_SUCCEED;
}

// ��ȡ�ֶ���������
DWORD CMingQuanMQW50A::GetTestResultData(short *const psHCHigh/*=NULL*/, 
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
	return ANA_WR_EQUIP_RETURE_0X15;
}
