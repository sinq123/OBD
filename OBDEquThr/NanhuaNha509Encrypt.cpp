#include "StdAfx.h"
#include "NanhuaNha509Encrypt.h"
#include "md5.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cassert>
#include <cstdio>

#include "NHA509EnLib\NHA509En.h"
#ifdef _DEBUG
#pragma comment(lib, "NHA509EnLib\\NHA509En_D.lib")
#else
#pragma comment(lib, "NHA509EnLib\\NHA509En.lib")
#endif

CNanhuaNha509Encrypt::CNanhuaNha509Encrypt(void)
//: m_hComm(INVALID_HANDLE_VALUE)
//, m_opUsedNow(ToAnalyzer)
{
	//::InitializeCriticalSection(&cInfoSection);

	m_unTimerID = timeSetEvent(250*1000, 1, (LPTIMECALLBACK)TimePro, (DWORD_PTR)this, TIME_PERIODIC);
}

CNanhuaNha509Encrypt::~CNanhuaNha509Encrypt(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNha509Encrypt::Open(BYTE bPort)
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
	// ���г�ʱ����
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return ANA_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaNha509Encrypt::Close(void)
{
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
	}

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

bool CNanhuaNha509Encrypt::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNha509Encrypt::WriteAndReadData(BYTE *const pbWriteBuffer, 
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

			if (1==dwBytesRead && pbReadBuffer[0]==0x15)
			{
				// �Ѿ�����
				return ANA_WR_EQUIP_RETURE_0X15;
			}

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

BYTE CNanhuaNha509Encrypt::CheckSum(const BYTE *const pb, const UINT un)
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

DWORD CNanhuaNha509Encrypt::WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
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

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 4, &dwBytesRead, NULL);
		if (bSucceed && 4==dwBytesRead)
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
			if (0x06 == pbReadBuffer[1])
			{
				// �����������Ѿ����ܣ������Ѿ����

				// TODO:��ʱ����
				// ���ݷ��ص����ݳ����ж�VMAS�ڲ�����汾�Ƿ��ӦF��ͨѶЭ��˵����
				assert(MAKEWORD(pbReadBuffer[2],pbReadBuffer[3]) == (WORD)((dwReadLength-6)/2));

				// �����µ�����
				bSucceed = ReadFile(m_hComm, pbReadBuffer+4, dwReadLength-4, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-4))
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
			else if (0x05 == pbReadBuffer[1])
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

WORD CNanhuaNha509Encrypt::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
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

DWORD CNanhuaNha509Encrypt::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
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

DWORD CNanhuaNha509Encrypt::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
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

DWORD CNanhuaNha509Encrypt::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
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
		}
	}

	return dw;
}

DWORD CNanhuaNha509Encrypt::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
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

DWORD CNanhuaNha509Encrypt::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
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

DWORD CNanhuaNha509Encrypt::CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	assert(NULL != pbResult);
	assert(usC3H8 >= 0);
	assert(fCO >= 0.0f);
	assert(fCO2 >= 0.0f);
	assert(usNO >= 0);

	BYTE bReadBuf(0);
	BYTE bWriteBuf[10];
	bWriteBuf[0] = bCmd;
	bWriteBuf[1] = HIBYTE(usC3H8);
	bWriteBuf[2] = LOBYTE(usC3H8);
	bWriteBuf[3] = HIBYTE((WORD)(fCO * 100.0f + 0.5f));
	bWriteBuf[4] = LOBYTE((WORD)(fCO * 100.0f + 0.5f));
	bWriteBuf[5] = HIBYTE((WORD)(fCO2 * 100.0f + 0.5f));
	bWriteBuf[6] = LOBYTE((WORD)(fCO2 * 100.0f + 0.5f));
	bWriteBuf[7] = HIBYTE(usNO);
	bWriteBuf[8] = LOBYTE(usNO);
	bWriteBuf[9] = 0;
	DWORD dw = WriteAndReadData(bWriteBuf, 10, &bReadBuf, 1, true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// �������ؽ��
		*pbResult = bReadBuf;
	}

	return dw;
}

DWORD CNanhuaNha509Encrypt::CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	assert(NULL != pbResult);

	BYTE bReadBuf(0);
	BYTE bWriteBuf[4];
	bWriteBuf[0] = bCmd;
	bWriteBuf[1] = HIBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[2] = LOBYTE((WORD)(fValue * 10.0f));
	bWriteBuf[3] = 0;
	DWORD dw = WriteAndReadData(bWriteBuf, 4, &bReadBuf, 1, true, false, true);
	if (fValue == dw)
	{
		// �������ؽ��
		*pbResult = bReadBuf;
	}

	return dw;
}


// 1ȡ������
DWORD CNanhuaNha509Encrypt::GetMajorData(short *const psHC, 
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
	bool bWaterLevel = false;
	return GetMajorData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx, pusRpm, pfOilTemp, pusFlux, pfLambda, pbO2Sensor, pbNOSensor, &bWaterLevel);
}


// 1ȡ������(��)
DWORD CNanhuaNha509Encrypt::GetMajorData(short *const psHC, 
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
							bool *const pbWaterLevel)
{
	BYTE bWriteBuf(0x2C), bReadBuf[31];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 31, false, true, true);
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
		// (6) NO2
		if (NULL != psNO2)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNO2 = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11]));
		}
		// (7) NOx
		if (NULL != psNOx)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*psNOx = (short)(MAKEWORD(bReadBuf[14], bReadBuf[13]));
		}
		// (8) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		}
		// (9) OilTemp
		if (NULL != pfOilTemp)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfOilTemp = (short)MAKEWORD(bReadBuf[18], bReadBuf[17]) / 10.0f;
		}
		// (10) Flux
		if (NULL != pusFlux)
		{
			*pusFlux = MAKEWORD(bReadBuf[20], bReadBuf[19]);
		}
		// (11) Lambda
		if (NULL != pfLambda)
		{
			// ����Ϊ�����߸���,���shortǿ��ת��
			*pfLambda = (short)(MAKEWORD(bReadBuf[25], bReadBuf[24])) / 1000.0f;
		}
		// (12)���������Ƿ��ϻ�
		if (NULL != pbO2Sensor)
		{
			if (0xFF == (bReadBuf[21]|0xFE))
			{
				*pbO2Sensor = true;
			}
			else
			{
				*pbO2Sensor = false;
			}
		}
		// (13)NO�������Ƿ��ϻ�
		if (NULL != pbNOSensor)
		{
			if (0xFF == (bReadBuf[21]|0xFD))
			{
				*pbNOSensor = true;
			}
			else
			{
				*pbNOSensor = false;
			}
		}

		// (14)ˮλ�Ƿ�����
		if (NULL != pbWaterLevel)
		{
			if (0xFF == (bReadBuf[21]|0xBF))
			{
				*pbWaterLevel = true;
			}
			else
			{
				*pbWaterLevel = false;
			}
		}
		
		WORD wLeft = MAKEWORD(bReadBuf[29], bReadBuf[28]) & 0x0FFF;
		bool bLock = ((bReadBuf[28]&0x80) != 0x00);
		BYTE bCheckTimes = (bReadBuf[28]&0x70)>>4;
		TRACE("GetMajorDate(), wLeft=%d, bLock=%d, bCheckTimes=%d\n", wLeft, bLock, bCheckTimes);
		if (wLeft < 300)
		{
			Authentication();
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
		 // NO2
		if (*psNO2 < ANALYZER_NO_LOWER_LIMIT)
		{
			*psNO2 = ANALYZER_NO_LOWER_LIMIT;
		}
		else if (*psNO2 > ANALYZER_NO_UPPER_LIMIT)
		{
			*psNO2 = ANALYZER_NO_UPPER_LIMIT;
		}
		 // NOx
		if (*psNOx < ANALYZER_NO_LOWER_LIMIT)
		{
			*psNOx = ANALYZER_NO_LOWER_LIMIT;
		}
		else if (*psNOx > ANALYZER_NO_UPPER_LIMIT)
		{
			*psNOx = ANALYZER_NO_UPPER_LIMIT;
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
		// (5) NO
		if (NULL != psNO2)
		{
			*psNO2 = 0;
		}
		// (5) NO
		if (NULL != psNOx)
		{
			*psNOx = 0;
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

		if (ANA_WR_EQUIP_RETURE_0X15 == dw)
		{
			Authentication();
		}		
	}
#ifdef _DEBUG
	//*pbWaterLevel = true;
#endif
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetNOxData(short *const psHC, 
		float *const pfCO, 
		float *const pfCO2, 
		float *const pfO2, 
		short *const psNO, 
		short *const psNO2, 
		short *const psNOx)
{
	return GetMajorData(psHC, pfCO, pfCO2, pfO2, psNO, psNO2, psNOx, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
/*
	short sHC = 0;
	float fCO = 0;
	float fCO2 = 0;
	float fO2 = 0;
	short sNO = 0;
	short sNO2 = 0;

	BYTE bWriteBuf[2] = {0xB5, 0x4B};
	BYTE bReadBuf[24] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 24, false, false);
	if (ANA_WR_SUCCEED == dw)
	{
		//*pfN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		//*pfK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		//*pusRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		//*pusOilTemp = MAKEWORD(bReadBuf[8], bReadBuf[7]);
		//if (0xFFFF == *pusOilTemp)
		//{
		//	*pusOilTemp = 0;
		//}
		//else
		//{
		//	*pusOilTemp -= 273;
		//}
		sNO = MAKEWORD(bReadBuf[10], bReadBuf[9]);
		sNO2 = MAKEWORD(bReadBuf[12], bReadBuf[11]);
		fCO2 = MAKEWORD(bReadBuf[14], bReadBuf[13]) / 100.0f;
		sHC = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		fCO = MAKEWORD(bReadBuf[18], bReadBuf[17]) / 100.0f;
		fO2 = MAKEWORD(bReadBuf[20], bReadBuf[19]) / 100.0f;
		
		WORD wLeft = MAKEWORD(bReadBuf[22], bReadBuf[21]) & 0x0FFF;
		bool bLock = ((bReadBuf[21]&0x80) != 0x00);
		BYTE bCheckTimes = (bReadBuf[21]&0x70)>>4;
		TRACE("GetMajorDate(), wLeft=%d, bLock=%d, bCheckTimes=%d\n", wLeft, bLock, bCheckTimes);
		if (wLeft < 60)
		{
			Authentication();
		}
	}
	if (psHC != NULL)
	{
		*psHC = sHC;
	}
	if (pfCO != NULL)
	{
		*pfCO = fCO;
	}
	if (pfCO2 != NULL)
	{
		*pfCO2 = fCO2;
	}
	if (pfO2 != NULL)
	{
		*pfO2 =fO2;
	}
	if (psNO != NULL)
	{
		*psNO = sNO;
	}
	if (psNO2 != NULL)
	{
		*psNO2 = sNO2;
	}
	return dw;
*/
}

// 2ȡ��������
// ET:Environment Temperature��д
// AP:Atmospheric Pressure��д
// RH:Relative Humidity��д
DWORD CNanhuaNha509Encrypt::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
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
DWORD CNanhuaNha509Encrypt::DeflateEnvAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x22, pbResult);
}

// 4��ܵ���(����,������̽ͷ����)
DWORD CNanhuaNha509Encrypt::DeflatePipeAir(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x23, pbResult);
}

// 5��ϴ�ܵ�(����)
DWORD CNanhuaNha509Encrypt::InverseBlow(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x05, pbResult);
}

// 6ֹͣ��ϴ(ֹͣ������ͣ��)
DWORD CNanhuaNha509Encrypt::StopPump(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x06, pbResult);
}

// 7��©
DWORD CNanhuaNha509Encrypt::CheckLeak(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x07, pbResult);
}

// 8����
DWORD CNanhuaNha509Encrypt::Zero(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(0x08, pbResult);
}

// 9д��У׼��Ũ��
DWORD CNanhuaNha509Encrypt::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
	const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	BYTE bReadBuf(0);
	BYTE bWriteBuf[13];
	bWriteBuf[0] = 0x09;
	bWriteBuf[1] = nPoint;
	bWriteBuf[2] = HIBYTE((WORD)(usCalC3H8));
	bWriteBuf[3] = LOBYTE((WORD)(usCalC3H8));
	bWriteBuf[4] = HIBYTE((WORD)(fCalCO * 100.0f));
	bWriteBuf[5] = LOBYTE((WORD)(fCalCO * 100.0f));
	bWriteBuf[6] = HIBYTE((WORD)(fCalCO2 * 100.0f));
	bWriteBuf[7] = LOBYTE((WORD)(fCalCO2 * 100.0f));
	bWriteBuf[8] = HIBYTE(usCalNO);
	bWriteBuf[9] = LOBYTE(usCalNO);
	bWriteBuf[10] = HIBYTE(usCalNO2);
	bWriteBuf[11] = LOBYTE(usCalNO2);
	bWriteBuf[12] = 0;
	DWORD dw = WriteAndReadData(bWriteBuf, 13, &bReadBuf, 1, true, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (pbResult != NULL)
		{
			// �������ؽ��
			*pbResult = bReadBuf;
		}
	}
	return dw;
}

// 10У׼
DWORD CNanhuaNha509Encrypt::GasCalibration(BYTE *const pbResult, const int nChanel, 
							  short *const psHC, 
							  float *const pfCO, 
							  float *const pfCO2, 
							  float *const pfO2, 
							  short *const psNO, 
							  short *const psNO2, 
							  float *const pfPef)
{	
	BYTE bWriteBuf[3] = {0x0A, nChanel, 0};
	BYTE bReadBuf[16] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 3, bReadBuf, 1, true, false, true);
	
	// ��ȡ����ִ�н��
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			*pbResult = bReadBuf[0];
		}
		
		if (0x05 == *pbResult)
		{
			// �������ؽ��:���ڽ��в���,Ҫ����������Ĳ�����������

			// ��2��:��������������(����ÿ�ζ�Ҫִ��,��������05����ͷʱ����Ҫִ��)
			dw = WriteAndReadData(NULL, 0, bReadBuf+1, 15, false, false, false);
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
				// (6)NO2
				if (NULL != psNO2)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*psNO2 = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11]));
				}
				// (7)Pef
				if (NULL != pfPef)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*pfPef = (short)(MAKEWORD(bReadBuf[14], bReadBuf[13])) / 1000.0f;
				}
			}
		}
		else if (0x0A == *pbResult)
		{
			// �������ؽ��:���ڽ��в���,Ҫ����������Ĳ�����������

			// ��2��:��������������(����ÿ�ζ�Ҫִ��,��������05����ͷʱ����Ҫִ��)
			dw = WriteAndReadData(NULL, 0, bReadBuf+1, 2, false, false, false);
			if (ANA_WR_SUCCEED == dw)
			{
				if (bReadBuf[1] != 0x00)
				{
					//��1λ��ӦHC��
					//��2λ��ӦCO��
					//��3λ��ӦCO2��
					//��4λ��ӦNO��
					//��5λ��ӦNO2;
					if ((nChanel&0x0F) == 0x01)	//У׼HC,CO,CO2
					{
						if (bReadBuf[1]&0x07)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else if ((nChanel&0x0F) == 0x02)	//У׼NO
					{
						if (bReadBuf[1]&0x08)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else if ((nChanel&0x0F) == 0x03)	//У׼HC,CO,CO2,NO
					{
						if (bReadBuf[1]&0x0F)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else if ((nChanel&0x0F) == 0x04)	//У׼NO2
					{
						if (bReadBuf[1]&0x10)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else if ((nChanel&0x0F) == 0x05)	//У׼HC,CO,CO2,NO2
					{
						if (bReadBuf[1]&0x17)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else if ((nChanel&0x0F) == 0x06)	//У׼NO,NO2
					{
						if (bReadBuf[1]&0x18)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else if ((nChanel&0x0F) == 0x07)	//У׼HC,CO,CO2,NO,NO2
					{
						if (bReadBuf[1]&0x1F)
						{
							*pbResult = 0x01;
						}
						else
						{
							*pbResult = 0x00;	// �ɹ�
						}
					}
					else
					{
						*pbResult = 0x01;
					}
				}
				else
				{
					*pbResult = 0x00;	// �ɹ�
				}
			}
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationNO2Concentration(BYTE *const pbResult, const USHORT usNO2)
{
	return SetCalibrationGasConcentration(pbResult, 0x04, 0, 0.0f, 0.0f, 0, usNO2);
}

DWORD CNanhuaNha509Encrypt::NO2Calibration(BYTE *const pbResult, short *const psNO2/*=NULL*/)
{
	BYTE bWriteBuf = 0xB8;
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	
	// ��ȡ����ִ�н��
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			*pbResult = bReadBuf;
		}
		
		if (0x05 == *pbResult)
		{
			// �������ؽ��:���ڽ��в���,Ҫ����������Ĳ�����������

			// ��2��:��������������(����ÿ�ζ�Ҫִ��,��������05����ͷʱ����Ҫִ��)
			BYTE bReadBuf[3];
			ZeroMemory(bReadBuf, sizeof(bReadBuf));
			dw = WriteAndReadData(NULL, 0, bReadBuf+1, 3, false, false, false);
			if (ANA_WR_SUCCEED == dw)
			{
				// (1)NO2
				if (NULL != psNO2)
				{
					// ����Ϊ�����߸���,���shortǿ��ת��
					*psNO2 = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
				}
			}
		}
	}
	return dw;
}

// 11д������Ũ��
DWORD CNanhuaNha509Encrypt::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return00_01(36, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12�Զ����
DWORD CNanhuaNha509Encrypt::GasCheck(BYTE *const pbResult, 
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
DWORD CNanhuaNha509Encrypt::GetPef(float *const pfPef)
{
	assert(NULL != pfPef);

	BYTE bWriteBuf(0x01);
	BYTE bReadBuf[27] = {0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 27, false ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// ����Ϊ�����߸���,���shortǿ��ת��
		*pfPef = (short)(MAKEWORD(bReadBuf[23], bReadBuf[22])) / 1000.0f;
	}
	else
	{
		// ��������

		//*pfPef = 0.0f;
	}
	return dw;
}

// 14���NO�ϻ���־
// 503����
DWORD CNanhuaNha509Encrypt::ClearNoSign(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(14, pbResult);
}

// 15���õ�������Ϊ����
DWORD CNanhuaNha509Encrypt::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 16���õ�������Ϊ����
DWORD CNanhuaNha509Encrypt::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 17���ó��Ϊ4
DWORD CNanhuaNha509Encrypt::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 18���ó��Ϊ2
DWORD CNanhuaNha509Encrypt::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 19���õ��ʽΪ����
DWORD CNanhuaNha509Encrypt::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 20���õ��ʽΪ����
DWORD CNanhuaNha509Encrypt::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 21�����Զ�����
DWORD CNanhuaNha509Encrypt::EnableAutoZero(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 22��ֹ�Զ�����
DWORD CNanhuaNha509Encrypt::DisableAutoZero(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 23������̲���

// 24��ֹ���̲���

// 25LCD��

// 26LCD��

// 27�����¶�У׼
DWORD CNanhuaNha509Encrypt::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	assert(fCalET>-20.0f && fCalET<150.0f);
	
	WORD wValue = (WORD)(fCalET * 10.0f + 0.5f);
	BYTE bWriteBuf[5] = {0x0F, 0x01, HIBYTE(wValue), LOBYTE(wValue), 0x00};
	BYTE bReadBuf[3] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 5, bReadBuf, 3, true ,true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			*pbResult= bReadBuf[1];
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 28����У׼

// 29����У׼
DWORD CNanhuaNha509Encrypt::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	assert(fCalOT>-20.0f && fCalOT<150.0f);
	
	WORD wValue = (WORD)(fCalOT * 10.0f + 0.5f);
	BYTE bWriteBuf[5] = {0x0F, 0x03, HIBYTE(wValue), LOBYTE(wValue), 0x00};
	BYTE bReadBuf[3] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 5, bReadBuf, 3, true ,true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			*pbResult= bReadBuf[1];
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 30����ʪ��У׼
DWORD CNanhuaNha509Encrypt::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	assert(fCalRH>0.0f && fCalRH<101.0f);
		
	WORD wValue = (WORD)(fCalRH * 10.0f + 0.5f);
	BYTE bWriteBuf[5] = {0x0F, 0x04, HIBYTE(wValue), LOBYTE(wValue), 0x00};
	BYTE bReadBuf[3] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 5, bReadBuf, 3, true ,true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			*pbResult= bReadBuf[1];
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 31����ѹ��У׼
DWORD CNanhuaNha509Encrypt::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	assert(fCalAP>0.0f && fCalAP<150.0f);
		
	WORD wValue = (WORD)(fCalAP * 10.0f + 0.5f);
	BYTE bWriteBuf[5] = {0x0F, 0x05, HIBYTE(wValue), LOBYTE(wValue), 0x00};
	BYTE bReadBuf[3] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 5, bReadBuf, 3, true ,true, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			*pbResult= bReadBuf[1];
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 32�������
DWORD CNanhuaNha509Encrypt::OpenCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x20, pbResult);
}

// 33�������
DWORD CNanhuaNha509Encrypt::CloseCheckGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x21, pbResult);
}

// 34ȡ����״̬
DWORD CNanhuaNha509Encrypt::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf(0x13);
	BYTE bReadBuf[3];
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 3, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0xf0)
		{
			*pbStatus = 0x00;
		}
		else if (bReadBuf[0] == 0xf1)
		{
			*pbStatus = 0x01;
		}
		else
		{
			*pbStatus = bReadBuf[0];
		}

		WORD wLeft = MAKEWORD(bReadBuf[2], bReadBuf[1]) & 0x0FFF;
		bool bLock = ((bReadBuf[1]&0x80) != 0x00);
		BYTE bCheckTimes = (bReadBuf[1]&0x70)>>4;
		TRACE("GetStatus(), *pbStatus=%02X,wLeft=%d,bLock=%d,bCheckTimes=%d\n", *pbStatus, wLeft, bLock, bCheckTimes);

		if ((wLeft<600) && ((*pbStatus==0x00) || (*pbStatus==0x01)))
		{
			Authentication();
		}
	}

	return dw;
}

DWORD CNanhuaNha509Encrypt::Authentication(void)
{
	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// ���ô��ڲ���
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		
		BYTE bCMD[MAX_PATH] = {0x10};
		// д���ݲ���
		if (WriteCommBuffer((BYTE *)bCMD, 1))
		{
			// һ�ζ���Ԥ��Ҫ���յ��ֽ���
			BYTE szRecv[MAX_PATH] = {0};
			if (ReadCommBuffer((BYTE *)szRecv, 4))
			{
				TRACE("Authentication(), GetSeed %02X %02X %02X %02X\n", szRecv[0], szRecv[1], szRecv[2], szRecv[3]);

				bCMD[0] = 0x11;
				for (int i=0; i<4; i++)
				{
					CStringA strXML;
					strXML.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
					strXML.AppendFormat("<root>");
					strXML.AppendFormat("<table>%u</table>", 'A'+i);
					strXML.AppendFormat("<seed>%u</seed>", szRecv[i]);
					strXML.AppendFormat("<time>%u</time>", (DWORD)time(NULL));
					strXML.AppendFormat("</root>");		
					MD5 hash((LPCSTR)strXML);					
					bCMD[i+1] = GetAuthenticationKey('A'+i, szRecv[i], hash.md5().c_str());
				}
				TRACE("Authentication(), SendKey %02X %02X %02X %02X\n", bCMD[1], bCMD[2], bCMD[3], bCMD[4]);

				// д���ݲ���
				if (WriteCommBuffer((BYTE *)bCMD, 5))
				{
					memset(szRecv, 0, MAX_PATH);
					if (ReadCommBuffer((BYTE *)szRecv, 1))
					{
						if (szRecv[0] == 0x06)
						{
							TRACE("Authentication() succeed\n");
							::LeaveCriticalSection(&cInfoSection);

							// �����ɹ�
							return ANA_WR_SUCCEED;
						}
					}
				}
			}
			else
			{
				bCMD[0] = 0x11;
				bCMD[1] = 0x00;
				bCMD[2] = 0x00;
				bCMD[3] = 0x00; 
				bCMD[4] = 0x00;
				// д���ݲ���
				if (WriteCommBuffer((BYTE *)bCMD, 5))
				{
					memset(szRecv, 0, MAX_PATH);
					ReadCommBuffer((BYTE *)szRecv, 1);
				}
			}
		}
		TRACE("Authentication() faild\n");
		::LeaveCriticalSection(&cInfoSection);

		// �������󣺶�ָ������ʧ��
		return ANA_WR_READ_SPECIFIED_DATA_FAIL;
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);

		// �������󣺴���û�д�
		return ANA_WR_PORT_NOT_OPEN;
	}
}

// 35ȡԤ��ʣ��ʱ��
DWORD CNanhuaNha509Encrypt::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	assert(NULL != pusSecondsLeft);

	BYTE bWriteBuf(0x27);
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 2, false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		*pusSecondsLeft = MAKEWORD(bReadBuf[1], bReadBuf[0]);
	}

	return dw;
}

// 36����ȼ������Ϊ����
DWORD CNanhuaNha509Encrypt::SetFuelTypeToGas(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}


// 37����ȼ������ΪҺ��ʯ����
DWORD CNanhuaNha509Encrypt::SetFuelTypeToLPG(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 38����ȼ������Ϊ��Ȼ��
DWORD CNanhuaNha509Encrypt::SetFuelTypeToNG(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

// 39����ȼ������Ϊ�Ҵ�����
DWORD CNanhuaNha509Encrypt::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	BYTE bWriteBuf[8] = {0x0E, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
	BYTE bReadBuf[8] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 8, bReadBuf, 8, true ,false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (NULL != pbResult)
		{
			if (bReadBuf[1] != 0x15)
			{
				*pbResult = 0x06;
			}
			else
			{
				*pbResult = 0x15;
			}
		}
	}
	else
	{
		if (NULL != pbResult)
		{
			// ��������
			*pbResult = 0x15;
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<SetOperationMode>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
	m_enMode = op;
#endif	

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(8);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	if (op == ToAnalyzer)
	{ 
		// �����ǲ���ģʽ
		pbWriteBuf[0] = 0x32;
		pbWriteBuf[1] = 0x00;  // normal mode
		pbWriteBuf[2] = 0x01;
		pbWriteBuf[3] = 0x00;
		pbWriteBuf[4] = 0x00;  // Concentration mode
		pbWriteBuf[5] = 0x00;
		pbWriteBuf[6] = 0x33;
		pbWriteBuf[7] = 0x00;
	}
	else
	{
		// �����Ʋ���ģʽ
		pbWriteBuf[0] = 0x32;
		pbWriteBuf[1] = 0x00;  // normal mode
		pbWriteBuf[2] = 0x01;
		pbWriteBuf[3] = 0x00;
		pbWriteBuf[4] = 0x01;  // Vmas mode
		pbWriteBuf[5] = 0x00;
		pbWriteBuf[6] = 0x34;
		pbWriteBuf[7] = 0x00;
	}
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
		
		// ��¼�л��ĵ�ǰģʽ
		if (*pbResult == 0x06)
		{
			m_enMode = op;
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}


DWORD CNanhuaNha509Encrypt::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	const DWORD dwBytesToRead(10);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(6);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x0b;
	pbWriteBuf[1] = 0x00;
	pbWriteBuf[2] = 0x00;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x0b;
	pbWriteBuf[5] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	// ��¼����ִ��״̬
	if (NULL != pwProcessStatus)
	{
		*pwProcessStatus = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
	}
	// ��¼����ִ��״̬��������
	if (NULL != pwProcess)
	{
		*pwProcess = MAKEWORD(pbReadBuf[6], pbReadBuf[7]);
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<StartCollectData_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif	
	
	assert(wCollectionTime>0 && wCollectionTime<=600);
	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(24);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	//��ʼ�ռ�����
	pbWriteBuf[0] = 0x33;  // Data collection control
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x09;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x01;  // Colletion mode:start data collection
	pbWriteBuf[5] = 0x00;
	//bWriteBuf[6] = 0xc3;  // Collection time:195 sec
	//bWriteBuf[7] = 0x00;
	pbWriteBuf[6] = LOBYTE(wCollectionTime);  // �ռ�ʱ��
	pbWriteBuf[7] = HIBYTE(wCollectionTime);
	pbWriteBuf[8] = 0x41;  // RH
	pbWriteBuf[9] = 0x00;
	pbWriteBuf[10] = 0x16;  // Temp_Ambient
	pbWriteBuf[11] = 0x00;
	pbWriteBuf[12] = 0x0b;  // Probe Extration
	pbWriteBuf[13] = 0x00;
	pbWriteBuf[14] = 0x02;  // Unit selection
	pbWriteBuf[15] = 0x00;
	pbWriteBuf[16] = 0x01;  // Calculation_iterations
	pbWriteBuf[17] = 0x00;
	pbWriteBuf[18] = 0x00;  // Gas_flow_lower_threshold
	pbWriteBuf[19] = 0x5e;
	pbWriteBuf[20] = 0x01;  // Gas_temp_upper_limit
	pbWriteBuf[21] = 0x01;
	pbWriteBuf[22] = 0x00;
	pbWriteBuf[23] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
			// �����Ʒ���:��ʼ�ռ����ݳɹ�
#ifdef _DEBUG
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
			OutputDebugString(wchOutPutDebugString);
#endif
			// ��¼״̬
			if (NULL != pwMiniStatus)
			{
				*pwMiniStatus = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
			}
		}

	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;


	//BYTE bReadBuf[8];
	//ZeroMemory(bReadBuf, sizeof(bReadBuf));
	//BYTE bWriteBuf[24];
	////��ʼ�ռ�����
	//bWriteBuf[0] = 0x33;  // Data collection control
	//bWriteBuf[1] = 0x00;  // normal mode
	//bWriteBuf[2] = 0x09;
	//bWriteBuf[3] = 0x00;
	//bWriteBuf[4] = 0x01;  // Colletion mode:start data collection
	//bWriteBuf[5] = 0x00;
	////bWriteBuf[6] = 0xc3;  // Collection time:195 sec
	////bWriteBuf[7] = 0x00;
	//bWriteBuf[6] = LOBYTE(wCollectionTime);  // �ռ�ʱ��
	//bWriteBuf[7] = HIBYTE(wCollectionTime);
	//bWriteBuf[8] = 0x41;  // RH
	//bWriteBuf[9] = 0x00;
	//bWriteBuf[10] = 0x16;  // Temp_Ambient
	//bWriteBuf[11] = 0x00;
	//bWriteBuf[12] = 0x0b;  // Probe Extration
	//bWriteBuf[13] = 0x00;
	//bWriteBuf[14] = 0x02;  // Unit selection
	//bWriteBuf[15] = 0x00;
	//bWriteBuf[16] = 0x01;  // Calculation_iterations
	//bWriteBuf[17] = 0x00;
	//bWriteBuf[18] = 0x00;  // Gas_flow_lower_threshold
	//bWriteBuf[19] = 0x5e;
	//bWriteBuf[20] = 0x01;  // Gas_temp_upper_limit
	//bWriteBuf[21] = 0x01;
	////// ����chksum
	////WORD wChkSum = bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[4] +
	////	bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9] +
	////	bWriteBuf[10] + bWriteBuf[11] + bWriteBuf[12] + bWriteBuf[13] + bWriteBuf[14] +
	////	bWriteBuf[15] + bWriteBuf[16] + bWriteBuf[17] + bWriteBuf[18] + bWriteBuf[19] +
	////	bWriteBuf[20] + bWriteBuf[21];
	////bWriteBuf[22] = LOBYTE(wChkSum);
	////bWriteBuf[23] = HIBYTE(wChkSum);
	//DWORD dw = WriteAndReadData(bWriteBuf, 24, bReadBuf, 8, true, true);
	//// ��¼���
	//if (NULL != pbResult)
	//{
	//	*pbResult = bReadBuf[1];
	//}
	//if (ANA_WR_SUCCEED == dw)
	//{
	//	if (0x06 == bReadBuf[1])
	//	{
	//		// �����Ʒ���:��ʼ�ռ����ݳɹ�
	//		
	//		// ��¼״̬
	//		if (NULL != pwMiniStatus)
	//		{
	//			*pwMiniStatus = MAKEWORD(bReadBuf[4], bReadBuf[5]);
	//		}
	//	}

	//}
	//return dw;
}

DWORD CNanhuaNha509Encrypt::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<StopCollectData_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(24);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x33;  // Data collection control
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x09;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x00;  // Colletion mode:stop data collection
	pbWriteBuf[5] = 0x00;
	pbWriteBuf[6] = 0x00;  // Collection time
	pbWriteBuf[7] = 0x00;
	pbWriteBuf[8] = 0x00;  // RH
	pbWriteBuf[9] = 0x00;
	pbWriteBuf[10] = 0x00;  // Temp_Ambient
	pbWriteBuf[11] = 0x00;
	pbWriteBuf[12] = 0x00;  // Probe Extration
	pbWriteBuf[13] = 0x00;
	pbWriteBuf[14] = 0x00;  // Unit selection
	pbWriteBuf[15] = 0x00;
	pbWriteBuf[16] = 0x00;  // Calculation_iterations
	pbWriteBuf[17] = 0x00;
	pbWriteBuf[18] = 0x00;  // Gas_flow_lower_threshold
	pbWriteBuf[19] = 0x00;
	pbWriteBuf[20] = 0x00;  // Gas_temp_upper_limit
	pbWriteBuf[21] = 0x00;
	pbWriteBuf[22] = 0x00;
	pbWriteBuf[23] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
			// �����Ʒ���:ֹͣ�ռ����ݳɹ�
#ifdef _DEBUG
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
			OutputDebugString(wchOutPutDebugString);
#endif
			// ��¼״̬
			if (NULL != pwMiniStatus)
			{
				*pwMiniStatus = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
			}
		}
	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus/*=NULL*/,
		BYTE *const pbResult/*=NULL*/)
{
	assert(wCntStart>0 && wCntStart<=600);
	assert(wCntLength>0 && wCntLength<=600);
	assert(NULL != pfHCwt);
	assert(NULL != pfCOwt);
	assert(NULL != pfCO2wt);
	assert(NULL != pfNOwt);


#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<GetProcessDataStatus_WT_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif

	const DWORD dwBytesToRead = 10+8*wCntLength;
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(12);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x34;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x03;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x00;  // Data mode,0:Data reported in grams/seconds
	pbWriteBuf[5] = 0x00;
	pbWriteBuf[6] = LOBYTE(wCntStart - 1);  // Cnt start
	pbWriteBuf[7] = HIBYTE(wCntStart - 1);
	pbWriteBuf[8] = LOBYTE(wCntLength);  // Cnt length
	pbWriteBuf[9] = HIBYTE(wCntLength);
	pbWriteBuf[10] = 0x00;
	pbWriteBuf[11] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
#ifdef _DEBUG
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
			OutputDebugString(wchOutPutDebugString);
#endif
			if (NULL != pwMiniStatus)
			{
				*pwMiniStatus = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
			}

			for (WORD w=0; w<wCntLength; w++)
			{
				pfHCwt[w] = (short)(MAKEWORD(pbReadBuf[8+w*8], pbReadBuf[9+w*8])) / 100.0f;
				pfCOwt[w] = (short)(MAKEWORD(pbReadBuf[10+w*8], pbReadBuf[11+w*8])) / 10.0f;
				pfCO2wt[w] = (float)(short)((MAKEWORD(pbReadBuf[12+w*8], pbReadBuf[13+w*8])));
				pfNOwt[w] = (short)(MAKEWORD(pbReadBuf[14+w*8], pbReadBuf[15+w*8])) / 100.0f;
			}
		}
	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;

	//DWORD dwBytesRead(0);
	//BYTE bWriteBuf[12], bReadBuf[18];
	//ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	//ZeroMemory(bReadBuf, sizeof(bReadBuf));
	//bWriteBuf[0] = 0x34;
	//bWriteBuf[1] = 0x00;  // normal mode
	//bWriteBuf[2] = 0x03;
	//bWriteBuf[3] = 0x00;
	//bWriteBuf[4] = 0x00;  // Data mode,0:Data reported in grams/seconds
	//bWriteBuf[5] = 0x00;
	//bWriteBuf[6] = LOBYTE(wCntStart - 1);  // Cnt start
	//bWriteBuf[7] = HIBYTE(wCntStart - 1);
	//bWriteBuf[8] = LOBYTE(wCntLength);  // Cnt length
	//bWriteBuf[9] = HIBYTE(wCntLength);
	//WORD wChkSum = bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[4] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9];
	//bWriteBuf[10] = LOBYTE(wChkSum);
	//bWriteBuf[11] = HIBYTE(wChkSum);

	//DWORD dw = WriteAndReadData(bWriteBuf, 12, bReadBuf, 2, false, false);
	//if (ANA_WR_SUCCEED == dw)
	//{
	//	if (0x06 == bReadBuf[1])
	//	{
	//		// �����µ�����
	//		ReadFile(m_hComm, bReadBuf+2, 16, &dwBytesRead, NULL);

	//		*pfHCwt = MAKEWORD(bReadBuf[8], bReadBuf[9]) / 100.0f;
	//		*pfHCwt = MAKEWORD(bReadBuf[10], bReadBuf[11]) / 10.0f;
	//		*pfCO2wt = float(MAKEWORD(bReadBuf[12], bReadBuf[13]));
	//		*pfNOwt = MAKEWORD(bReadBuf[14], bReadBuf[15]) / 100.0f;
	//		*pbResult = 0x06;
	//	}
	//	else if (0x05 == bReadBuf[1])  // δ���,��Ҫ�ȴ�
	//	{
	//		*pbResult = 0x05;
	//	}
	//	else
	//	{
	//		// �����ϲ�����ִ�е�����
	//		wchar_t *pwch = new wchar_t[100];
	//		swprintf_s(pwch, 100, L"�����߼�����\n�ļ���%s\n�кţ�%d\n", __FILE__, __LINE__);
	//		OutputDebugStringW(pwch);
	//		delete[] pwch;
	//	}
	//}
	//else
	//{
	//	// ��������
	//	*pbResult = 0xFF;
	//}
	//delete[] pbReadBuf;
	//return dw;
}

DWORD CNanhuaNha509Encrypt::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct, 
		float *const pfCOct, 
		float *const pfCO2ct, 
		short *const psNOct, 
		float *const pfDilO2, 
		float *const pfBenchO2, 
		short *const psGasTemp, 
		float *const pfGasFluxStd0, 
		float *const pfGasFluxStd20, 
		float *const pfExhaustFluxStd0, 
		float *const pfExhaustFluxStd20, 
		float *const pfGasPres, 
		float *const pfGasFluxAct, 
		USHORT *const pusRpm,
		WORD *const pwMiniStatus,
		const BYTE bDataMode,
		BYTE *const pbResult)
{
	assert(wCntStart>0 && wCntStart<=600);
	assert(wCntLength>0 && wCntLength<=600);
	assert(bDataMode==0x01 || bDataMode==0x03);

//#ifdef _DEBUG
//	SYSTEMTIME st;
//	GetSystemTime(&st);
//	wchar_t wchOutPutDebugString[2048];
//	swprintf_s(wchOutPutDebugString, L"\r\n����<GetProcessDataStatus_CT_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
//	OutputDebugString(wchOutPutDebugString);
//#endif

	const DWORD dwBytesToRead = 10+24*wCntLength;
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(12);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x34;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x03;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = bDataMode;  // Data mode
	pbWriteBuf[5] = 0x00;
	pbWriteBuf[6] = LOBYTE(wCntStart - 1);  // Cnt start
	pbWriteBuf[7] = HIBYTE(wCntStart - 1);
	pbWriteBuf[8] = LOBYTE(wCntLength);  // Cnt length
	pbWriteBuf[9] = HIBYTE(wCntLength);
	pbWriteBuf[10] = 0x00;
	pbWriteBuf[11] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
//#ifdef _DEBUG
//			wchar_t wchOutPutDebugString[2048];
//			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
//			OutputDebugString(wchOutPutDebugString);
//#endif
			if (NULL != pwMiniStatus)
			{
				*pwMiniStatus = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
			}

			for (WORD w=0; w<wCntLength; w++)
			{

				// HCŨ��,��λ:ppm
				if (NULL != psHCct)
				{
					*psHCct = (short)(MAKEWORD(pbReadBuf[8+w*8], pbReadBuf[9+w*8]));
				}
				// COŨ��,��λ:%
				if (NULL != pfCOct)
				{
					*pfCOct = (short)(MAKEWORD(pbReadBuf[10+w*8], pbReadBuf[11+w*8])) / 100.0f;
				}
				// CO2Ũ��,��λ:%
				if (NULL != pfCO2ct)
				{
					*pfCO2ct = (short)(MAKEWORD(pbReadBuf[12+w*8], pbReadBuf[13+w*8])) / 10.0f;
				}
				// NOŨ��,��λ:ppm
				if (NULL != psNOct)
				{
					*psNOct = (short)(MAKEWORD(pbReadBuf[14+w*8], pbReadBuf[15+w*8]));
				}
				// ϡ����Ũ��,��λ:%
				if (NULL != pfDilO2)
				{
					*pfDilO2 = (short)(MAKEWORD(pbReadBuf[16+w*8], pbReadBuf[17+w*8])) / 100.0f;
				}
				// ��������������Ũ��,��λ:%
				if (NULL != pfBenchO2)
				{
					*pfBenchO2 = (short)(MAKEWORD(pbReadBuf[18+w*8], pbReadBuf[19]+w*8)) / 100.0f;
				}
				// ϡ�������¶�,��λ:���϶�
				const short sGasTemp = (short)(MAKEWORD(pbReadBuf[20+w*8], pbReadBuf[21+w*8]));
				if (NULL != psGasTemp)
				{
					*psGasTemp = sGasTemp;
				}
				// ϡ������ѹ��,��λ:kPa
				const float fGasPres = (short)(MAKEWORD(pbReadBuf[26+w*8], pbReadBuf[27+w*8])) / 10.0f;
				if (NULL != pfGasPres)
				{
					*pfGasPres = fGasPres;
				}
				// ������ת��,��λ:r/min
				if (NULL != pusRpm)
				{
					*pusRpm = MAKEWORD(pbReadBuf[30+w*8], pbReadBuf[31+w*8]);
				}
				// ϡ������ʵ������,��λ:L/s
				// ʵ������Ӣ��/����->��/��
				float fGasFluxAct = (short)(MAKEWORD(pbReadBuf[28+w*8], pbReadBuf[29+w*8])) * 0.471917f;
				// ����2λС��
				fGasFluxAct = CNHCommonAPI::round(fGasFluxAct * 100.0f) / 100.0f;
				if (NULL != pfGasFluxAct)
				{
					*pfGasFluxAct = fGasFluxAct;
				}
				// ϡ����������,��λ:L/s
				// ��׼����Ӣ��/����->��/��
				float fGasFluxStd20 = (short)(MAKEWORD(pbReadBuf[22+w*8], pbReadBuf[23+w*8])) * 0.471917f;
				// ����2λС��
				fGasFluxStd20 = CNHCommonAPI::round(fGasFluxStd20 * 100.0f) / 100.0f;
				if (NULL != pfGasFluxStd20)
				{
					*pfGasFluxStd20 = fGasFluxStd20;
				}
				if (NULL != pfGasFluxStd0)
				{
					*pfGasFluxStd0 = ActFlux2StdFlux0(fGasFluxAct, (float)sGasTemp, fGasPres);
				}
				// ��������,��λ:L/s
				// ��׼����Ӣ��/����->��/��
				float fExhaustFluxStd20 = (short)(MAKEWORD(pbReadBuf[24+w*8], pbReadBuf[25+w*8])) * 0.471917f;
				// ����2λС��
				fExhaustFluxStd20 = CNHCommonAPI::round(fExhaustFluxStd20 * 100.0f) / 100.0f;
				if (NULL != pfExhaustFluxStd20)
				{
					*pfExhaustFluxStd20 = fExhaustFluxStd20;
				}
				if (NULL != pfExhaustFluxStd0)
				{
					*pfExhaustFluxStd0 = StdFlux202StdFlux0(fExhaustFluxStd20, (float)sGasTemp, fGasPres);
				}
			}
		}
	}
	else
	{
		// HCŨ��,��λ:ppm
		if (NULL != psHCct)
		{
			*psHCct = 0;
		}
		// COŨ��,��λ:%
		if (NULL != pfCOct)
		{
			*pfCOct = 0.0f;
		}
		// CO2Ũ��,��λ:%
		if (NULL != pfCO2ct)
		{
			*pfCO2ct = 0.0f;
		}
		// NOŨ��,��λ:ppm
		if (NULL != psNOct)
		{
			*psNOct = 0;
		}
		// ϡ����Ũ��,��λ:%
		if (NULL != pfDilO2)
		{
			*pfDilO2 = 0.0f;
		}
		// ��������������Ũ��,��λ:%
		if (NULL != pfBenchO2)
		{
			*pfBenchO2 = 0.0f;
		}
		// ϡ�������¶�,��λ:���϶�
		if (NULL != psGasTemp)
		{
			*psGasTemp = 0;
		}
		// ϡ����������,��λ:L/s
		// ��׼����Ӣ��/����->��/��
		if (NULL != pfGasFluxStd0)
		{
			*pfGasFluxStd0 =0.0f;
		}
		if (NULL != pfGasFluxStd20)
		{
			*pfGasFluxStd20 =0.0f;
		}
		// ��������,��λ:L/s
		// ��׼����Ӣ��/����->��/��
		if (NULL != pfExhaustFluxStd0)
		{
			*pfExhaustFluxStd0 = 0.0f;
		}
		if (NULL != pfExhaustFluxStd20)
		{
			*pfExhaustFluxStd20 = 0.0f;
		}
		// ϡ������ѹ��,��λ:kPa
		if (NULL != pfGasPres)
		{
			*pfGasPres = 0.0f;
		}
		// ϡ������ʵ������,��λ:L/s
		// ʵ������Ӣ��/����->��/��
		if (NULL != pfGasFluxAct)
		{
			*pfGasFluxAct = 0.0f;
		}
		// ������ת��,��λ:r/min
		if (NULL != pusRpm)
		{
			*pusRpm = 0;
		}

		if (NULL != pwMiniStatus)
		{
			*pwMiniStatus = 0x00;
		}
	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus/*=NULL*/,
		BYTE *const pbResult/*=NULL*/)
{
	assert(NULL != pusHCwt);
	assert(NULL != pusCOwt);
	assert(NULL != pusCO2wt);
	assert(NULL != pusNOwt);
	assert(NULL != pfExhaustVolume);
	assert(NULL != pfGasVolume);

#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<GetTotalWeight_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif

	const DWORD dwBytesToRead(22);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(12);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x34;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x03;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x02;  // Data mode:data reported in total grams
	pbWriteBuf[5] = 0x00;
	pbWriteBuf[6] = 0x00;  // Cnt start
	pbWriteBuf[7] = 0x00;
	pbWriteBuf[8] = 0xc3;  // Cnt length: 195 sec
	pbWriteBuf[9] = 0x00;
	pbWriteBuf[10] = 0x00;
	pbWriteBuf[11] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_OPEN_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
#ifdef _DEBUG
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
			OutputDebugString(wchOutPutDebugString);
#endif
			if (NULL != pwMiniStatus)
			{
				*pwMiniStatus = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
			}

			// HC������,��λ:mg
			*pusHCwt = MAKEWORD(pbReadBuf[8], pbReadBuf[9]);
			// CO������,��λ:mg
			*pusCOwt= MAKEWORD(pbReadBuf[10], pbReadBuf[11]) * 10;
			// CO2������,��λ:mg
			*pusCO2wt = MAKEWORD(pbReadBuf[12], pbReadBuf[13]) * 100;
			// NO������,��λ:mg
			*pusNOwt = MAKEWORD(pbReadBuf[14], pbReadBuf[15]);
			// �������,��λ:L
			// ����Ӣ��->��
			*pfExhaustVolume = (short)(MAKEWORD(pbReadBuf[16], pbReadBuf[17])) * 28.316846f;
			// ϡ���������,��λ:L
			// ����Ӣ��->��
			*pfGasVolume = (short)(MAKEWORD(pbReadBuf[18], pbReadBuf[19])) * 28.316846f;
		}
	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;


	//DWORD dwBytesRead(0);
	//BYTE bWriteBuf[12], bReadBuf[22];
	//ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	//ZeroMemory(bReadBuf, sizeof(bReadBuf));
	//bWriteBuf[0] = 0x34;
	//bWriteBuf[1] = 0x00;  // normal mode
	//bWriteBuf[2] = 0x03;
	//bWriteBuf[3] = 0x00;
	//bWriteBuf[4] = 0x02;  // Data mode:data reported in total grams
	//bWriteBuf[5] = 0x00;
	//bWriteBuf[6] = 0x00;  // Cnt start
	//bWriteBuf[7] = 0x00;
	//bWriteBuf[8] = 0xc3;  // Cnt length: 195 sec
	//bWriteBuf[9] = 0x00;
	//WORD wChkSum = bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[4] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9];
	//bWriteBuf[10] = LOBYTE(wChkSum);
	//bWriteBuf[11] = HIBYTE(wChkSum);

	//DWORD dw = WriteAndReadData(bWriteBuf, 12, bReadBuf, 2, false, false);
	//if (0UL == dw)
	//{
	//	if (0x05 == bReadBuf[1])
	//	{
	//		// δ�����Ҫ�ȴ�
	//		*pbResult = 0x05;
	//	}
	//	else if (0x06 == bReadBuf[1])
	//	{
	//		// ����������
	//		ReadFile(m_hComm, bReadBuf+2, 20, &dwBytesRead, NULL);

	//		*pusHCwt = MAKEWORD(bReadBuf[8], bReadBuf[9]);
	//		*pusCOwt= MAKEWORD(bReadBuf[10], bReadBuf[11]) * 10;
	//		*pusCO2wt = MAKEWORD(bReadBuf[12], bReadBuf[13]) * 100;
	//		*pusNOwt = MAKEWORD(bReadBuf[14], bReadBuf[15]);
	//		*pbResult = 0x06;
	//	}
	//	else
	//	{
	//		// �����ϲ�����ִ�е�����
	//		wchar_t *pwch = new wchar_t[100];
	//		swprintf_s(pwch, 100, L"�����߼�����\n�ļ���%s\n�кţ�%d\n", __FILE__, __LINE__);
	//		OutputDebugStringW(pwch);
	//		delete[] pwch;
	//	}
	//}
	//else
	//{
	//	// ��������
	//	*pbResult = 0xFF;
	//}
	//return dw;
}

DWORD CNanhuaNha509Encrypt::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	assert(NULL != pwCorrectedDataNumber);

#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<GetCollectedDataNumber_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(6);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x39;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x00;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x39;
	pbWriteBuf[5] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
#ifdef _DEBUG
			wchar_t wchOutPutDebugString[2048];
			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
			OutputDebugString(wchOutPutDebugString);
#endif
			*pwCorrectedDataNumber = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
		}
	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	//assert(NULL!=pfFlux);
	//assert(NULL!=pfPres);
	//assert(NULL!=pfUDilO2);

//#ifdef _DEBUG
//	SYSTEMTIME st;
//	GetSystemTime(&st);
//	wchar_t wchOutPutDebugString[2048];
//	swprintf_s(wchOutPutDebugString, L"\r\n����<GetRealTimeData_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
//	OutputDebugString(wchOutPutDebugString);
//#endif

	const DWORD dwBytesToRead(58);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(6);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x37;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x00;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x37;
	pbWriteBuf[5] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
//#ifdef _DEBUG
//			wchar_t wchOutPutDebugString[2048];
//			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
//			OutputDebugString(wchOutPutDebugString);
//#endif
			// Pres:ѹ��
			// ת��ΪkPa
			float fPres = (short)(MAKEWORD(pbReadBuf[48], pbReadBuf[49])) / 100.0f;
			if (NULL != pfPres)
			{
				*pfPres = fPres;
			}
			// UDilO2��δ����ϡ����
			float fUDilO2 = (short)(MAKEWORD(pbReadBuf[42], pbReadBuf[43]) + MAKEWORD(pbReadBuf[44], pbReadBuf[45])) / 200.0f;
			if (NULL != pfUDilO2)
			{
				*pfUDilO2 = fUDilO2;
			}
			// ϡ�����¶�
			float fGasTemp = (short)(MAKEWORD(pbReadBuf[46], pbReadBuf[47])) / 10.0f;
			if (NULL != pfGasTemp)
			{
				*pfGasTemp = fGasTemp;
			}

			// 20���׼����
			// ��׼����Ӣ��/����->��/��
			float fFluxStd20 = (short)(MAKEWORD(pbReadBuf[50], pbReadBuf[51])) * 0.471917f;
			// ����2λС��
			fFluxStd20 = CNHCommonAPI::round(fFluxStd20 * 100.0f) / 100.0f;
			if (NULL != pfFluxStd20)
			{
				*pfFluxStd20 = fFluxStd20;
			}
			// 0���׼����
			float fFluxStd0 = StdFlux202StdFlux0(fFluxStd20, fGasTemp, fPres);
			if (NULL != pfFluxStd0)
			{
				*pfFluxStd0 = fFluxStd0;
			}
			// ʵ������
			float fFluxAct = StdFlux202ActFlux(fFluxStd20, fGasTemp, fPres);
			if (NULL != pfFluxAct)
			{
				*pfFluxAct = fFluxAct;
			}
		}
	}
	else
	{
		// Pres:ѹ��
		// ת��ΪkPa
		if (NULL != pfPres)
		{
			*pfPres = 0.0f;
		}
		// UDilO2��δ����ϡ����
		if (NULL != pfUDilO2)
		{
			*pfUDilO2 = 0.0f;
		}
		if (NULL != pfGasTemp)
		{
			// ϡ�����¶�
			*pfGasTemp = 0.0f;
		}

		// 20���׼����
		// ��׼����Ӣ��/����->��/��
		if (NULL != pfFluxStd20)
		{
			*pfFluxStd20 = 0;
		}
		// 0���׼����
		if (NULL != pfFluxStd0)
		{
			*pfFluxStd0 = 0;
		}
		// ʵ������
		if (NULL != pfFluxAct)
		{
			*pfFluxAct = 0;
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	//assert(NULL!=pfFlux);
	//assert(NULL!=pfPres);
	//assert(NULL!=pfUDilO2);

//#ifdef _DEBUG
//	SYSTEMTIME st;
//	GetSystemTime(&st);
//	wchar_t wchOutPutDebugString[2048];
//	swprintf_s(wchOutPutDebugString, L"\r\n����<GetRealTimeData_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
//	OutputDebugString(wchOutPutDebugString);
//#endif

	const DWORD dwBytesToRead(58);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(6);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x37;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x00;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x37;
	pbWriteBuf[5] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
//#ifdef _DEBUG
//			wchar_t wchOutPutDebugString[2048];
//			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
//			OutputDebugString(wchOutPutDebugString);
//#endif
			*pwStatus0 = MAKEWORD(pbReadBuf[4], pbReadBuf[5]);
			*pwStatus1 = MAKEWORD(pbReadBuf[6], pbReadBuf[7]);
			*pwStatus2 = MAKEWORD(pbReadBuf[8], pbReadBuf[9]);
			*pwStatus3 = MAKEWORD(pbReadBuf[10], pbReadBuf[11]);
			*pwStatus4 = MAKEWORD(pbReadBuf[12], pbReadBuf[13]);
			*pwStatus5 = MAKEWORD(pbReadBuf[14], pbReadBuf[15]);
			*pwStatus6 = MAKEWORD(pbReadBuf[16], pbReadBuf[17]);
			*pwStatus7 = MAKEWORD(pbReadBuf[18], pbReadBuf[19]);
			*pwStatus8 = MAKEWORD(pbReadBuf[20], pbReadBuf[21]);
			*pwStatus9 = MAKEWORD(pbReadBuf[22], pbReadBuf[23]);
			*pwStatus10 = MAKEWORD(pbReadBuf[24], pbReadBuf[25]);
			*pwStatus11 = MAKEWORD(pbReadBuf[26], pbReadBuf[27]);
			*pwStatus12 = MAKEWORD(pbReadBuf[28], pbReadBuf[29]);
			*pwStatus13 = MAKEWORD(pbReadBuf[30], pbReadBuf[31]);
			*pwStatus14 = MAKEWORD(pbReadBuf[32], pbReadBuf[33]);
			*pwStatus15 = MAKEWORD(pbReadBuf[34], pbReadBuf[35]);
			*pwStatus16 = MAKEWORD(pbReadBuf[36], pbReadBuf[37]);
			*pwStatus17 = MAKEWORD(pbReadBuf[38], pbReadBuf[39]);
			*pwStatus18 = MAKEWORD(pbReadBuf[40], pbReadBuf[41]);
			*pwStatus19 = MAKEWORD(pbReadBuf[42], pbReadBuf[43]);
			*pwStatus20 = MAKEWORD(pbReadBuf[44], pbReadBuf[45]);
			*pwStatus21 = MAKEWORD(pbReadBuf[46], pbReadBuf[47]);
			*pwStatus22 = MAKEWORD(pbReadBuf[48], pbReadBuf[49]);
			*pwStatus23 = MAKEWORD(pbReadBuf[50], pbReadBuf[51]);
			*pwStatus24 = MAKEWORD(pbReadBuf[52], pbReadBuf[53]);
			*pwStatus25 = MAKEWORD(pbReadBuf[54], pbReadBuf[55]);

			// Pres:ѹ��
			// ת��ΪkPa
			float fPres = (short)(MAKEWORD(pbReadBuf[48], pbReadBuf[49])) / 100.0f;
			if (NULL != pfPres)
			{
				*pfPres = fPres;
			}
			// UDilO2��δ����ϡ����
			float fUDilO2 = (short)(MAKEWORD(pbReadBuf[42], pbReadBuf[43]) + MAKEWORD(pbReadBuf[44], pbReadBuf[45])) / 200.0f;
			if (NULL != pfUDilO2)
			{
				*pfUDilO2 = fUDilO2;
			}
			// ϡ�����¶�
			float fGasTemp = (short)(MAKEWORD(pbReadBuf[46], pbReadBuf[47])) / 10.0f;
			if (NULL != pfGasTemp)
			{
				*pfGasTemp = fGasTemp;
			}

			// 20���׼����
			// ��׼����Ӣ��/����->��/��
			float fFluxStd20 = (short)(MAKEWORD(pbReadBuf[50], pbReadBuf[51])) * 0.471917f;
			// ����2λС��
			fFluxStd20 = CNHCommonAPI::round(fFluxStd20 * 100.0f) / 100.0f;
			if (NULL != pfFluxStd20)
			{
				*pfFluxStd20 = fFluxStd20;
				
			}
			// 0���׼����
			float fFluxStd0 = StdFlux202StdFlux0(fFluxStd20, fGasTemp, fPres);
			if (NULL != pfFluxStd0)
			{
				*pfFluxStd0 = fFluxStd0;
			}
			// ʵ������
			float fFluxAct = StdFlux202ActFlux(fFluxStd20, fGasTemp, fPres);
			if (NULL != pfFluxAct)
			{
				*pfFluxAct = fFluxAct;
			}
		}
	}
	else
	{
		*pwStatus0 = 0xFF;
		*pwStatus1 = 0xFF;
		*pwStatus2 = 0xFF;
		*pwStatus3 = 0xFF;
		*pwStatus4 = 0xFF;
		*pwStatus5 = 0xFF;
		*pwStatus6 = 0xFF;
		*pwStatus7 = 0xFF;
		*pwStatus8 = 0xFF;
		*pwStatus9 = 0xFF;
		*pwStatus10 = 0xFF;
		*pwStatus11 = 0xFF;
		*pwStatus12 = 0xFF;
		*pwStatus13 = 0xFF;
		*pwStatus14 = 0xFF;
		*pwStatus15 = 0xFF;
		*pwStatus16 = 0xFF;
		*pwStatus17 = 0xFF;
		*pwStatus18 = 0xFF;
		*pwStatus19 = 0xFF;
		*pwStatus20 = 0xFF;
		*pwStatus21 = 0xFF;
		*pwStatus22 = 0xFF;
		*pwStatus23 = 0xFF;
		*pwStatus24 = 0xFF;
		*pwStatus25 = 0xFF;

		// Pres:ѹ��
		// ת��ΪkPa
		if (NULL != pfPres)
		{
			*pfPres = 0.0f;
		}
		// UDilO2��δ����ϡ����
		if (NULL != pfUDilO2)
		{
			*pfUDilO2 = 0.0f;
		}
		if (NULL != pfGasTemp)
		{
			// ϡ�����¶�
			*pfGasTemp = 0.0f;
		}

		// 20���׼����
		// ��׼����Ӣ��/����->��/��
		if (NULL != pfFluxStd20)
		{
			*pfFluxStd20 = 0;
		}
		// 0���׼����
		if (NULL != pfFluxStd0)
		{
			*pfFluxStd0 = 0;
		}
		// ʵ������
		if (NULL != pfFluxAct)
		{
			*pfFluxAct = 0;
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	assert(NULL != pbResult);
	assert(fCalFlux>10e-6 && fCalFlux<400.0f);

	BYTE bWriteBuf[18], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// L/sת��Ϊ����Ӣ��/����,��������
	WORD wCalFlux = WORD(fCalFlux / 1000.0f / 0.028315f * 60.0f + 0.5f);

	if (HighFlowCal == cm) 
	{
		// ������У׼

		// д��У׼����
		bWriteBuf[0] = 0x35;
		bWriteBuf[1] = 0x00;  // normal mode
		bWriteBuf[2] = 0x06;
		bWriteBuf[3] = 0x00;
		bWriteBuf[4] = 0x01;  // Write data mode
		bWriteBuf[5] = 0x00;
		bWriteBuf[6] = 0x01;  // Flow meter cal mode
		bWriteBuf[7] = 0x00;
		bWriteBuf[8] = 0xcd;  // Point 1 SCFM value
		bWriteBuf[9] = 0xab;
		bWriteBuf[10] = 0xcd;  // Point 1 Frequency value
		bWriteBuf[11] = 0xab;
		bWriteBuf[12] = LOBYTE(wCalFlux);  // Point 2 SCFM value
		bWriteBuf[13] = HIBYTE(wCalFlux);
		bWriteBuf[14] = 0xff;  // Point 2 Frequency value
		bWriteBuf[15] = 0xff;
		bWriteBuf[16] = 0;
		bWriteBuf[17] = 0;
	}
	else 
	{
		// ������У׼

		// д��У׼����
		bWriteBuf[0] = 0x35;
		bWriteBuf[1] = 0x00;  // normal mode
		bWriteBuf[2] = 0x06;
		bWriteBuf[3] = 0x00;
		bWriteBuf[4] = 0x01;  // Write data mode
		bWriteBuf[5] = 0x00;
		bWriteBuf[6] = 0x01;  // Flow meter cal mode
		bWriteBuf[7] = 0x00;
		bWriteBuf[8] = LOBYTE(wCalFlux);  // Point 1 SCFM value
		bWriteBuf[9] = HIBYTE(wCalFlux);
		bWriteBuf[10] = 0xff;  // Point 1 Frequency value
		bWriteBuf[11] = 0xff;
		bWriteBuf[12] = 0xcd;  // Point 2 SCFM value
		bWriteBuf[13] = 0xab;
		bWriteBuf[14] = 0xcd;  // Point 2 Frequency value
		bWriteBuf[15] = 0xab;
		bWriteBuf[16] = 0;
		bWriteBuf[17] = 0;
	}

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 18, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	BYTE bWriteBuf[8], bReadBuf[8];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	if (HighFlowCal == cm) 
	{
		// ������У׼

		// ����У׼
		bWriteBuf[0] = 0x36;
		bWriteBuf[1] = 0x00;  // normal mode
		bWriteBuf[2] = 0x01;
		bWriteBuf[3] = 0x00;
		bWriteBuf[4] = 0x02;  // Calibrate hi flow rate point 2
		bWriteBuf[5] = 0x00;
		bWriteBuf[6] = 0x39;  // Chksum
		bWriteBuf[7] = 0x00;
	}
	else  // ������У׼
	{
		// ����У׼
		bWriteBuf[0] = 0x36;
		bWriteBuf[1] = 0x00;  // normal mode
		bWriteBuf[2] = 0x01;
		bWriteBuf[3] = 0x00;
		bWriteBuf[4] = 0x01;  // Calibrate low flow rate point 1
		bWriteBuf[5] = 0x00;
		bWriteBuf[6] = 0x38;  // Chksum
		bWriteBuf[7] = 0x00;
	}

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 8, bReadBuf, 8, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == bReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = bReadBuf[4];
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	assert(NULL != pbResult);
	assert(fCalPres>50.0f && fCalPres<400.0f);

	BYTE bWriteBuf[14], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// ��ǧ��->����,��������
	WORD wCalPres = WORD(fCalPres * 10.0f + 0.5f);

	//дѹ��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x04;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write calibration data
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x02;  // Write absolute pressure calibration data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE((WORD)wCalPres);  // Write absolute pressure in mbar
	bWriteBuf[9] = HIBYTE((WORD)wCalPres);
	bWriteBuf[10]=0xff;  // Write voltage corresponding to the above pressure
	bWriteBuf[11]=0xff;
	bWriteBuf[12] = 0x00;
	bWriteBuf[13] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 14, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	BYTE bWriteBuf[8], bReadBuf[8];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// ����У׼
	bWriteBuf[0] = 0x36;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x01;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x04;  // Calibrate absolute pressure transducer
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x3b;  // Check sum
	bWriteBuf[7] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 8, bReadBuf, 8, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == bReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = bReadBuf[4];
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	BYTE bWriteBuf[26], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x0a;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write calibration data
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x07;  // Write diluted O2 calibration data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 0xff; 
	bWriteBuf[9] = 0xff;
	bWriteBuf[10] = 0xff;
	bWriteBuf[11] = 0xff;
	bWriteBuf[12] = 0xff;
	bWriteBuf[13] = 0xff;
	bWriteBuf[14] = 0xff;
	bWriteBuf[15] = 0xff;
	bWriteBuf[16] = 0xff;
	bWriteBuf[17] = 0xff;
	bWriteBuf[18] = 0xff;
	bWriteBuf[19] = 0xff;
	bWriteBuf[20] = 0xff;
	bWriteBuf[21] = 0xff;
	bWriteBuf[22] = 0xff;
	bWriteBuf[23] = 0xff;
	bWriteBuf[24] = 0x00;
	bWriteBuf[25] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 26, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	DWORD dwBytesToWrite(0);
	BYTE *pbWriteBuf(NULL);
	if (bDataRequestMode)
	{
		// Lengthy command

		dwBytesToWrite = 6;
		pbWriteBuf = new BYTE[dwBytesToWrite]();

		pbWriteBuf[0] = 0x36;
		pbWriteBuf[1] = 0x01;  // Data request mode
		pbWriteBuf[2] = 0x00;
		pbWriteBuf[3] = 0x00;
		pbWriteBuf[4] = 0x37;
		pbWriteBuf[5] = 0x00;
	}
	else
	{
		// Short command

		dwBytesToWrite = 8;
		pbWriteBuf = new BYTE[dwBytesToWrite]();

		pbWriteBuf[0] = 0x36;
		pbWriteBuf[1] = 0x00;  // Normal mode
		pbWriteBuf[2] = 0x01;
		pbWriteBuf[3] = 0x00;
		pbWriteBuf[4] = 0x80;  // Calibrate Dilution O2 transducer
		pbWriteBuf[5] = 0x00;
		pbWriteBuf[6] = 0xb7;
		pbWriteBuf[7] = 0x00;
	}

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == pbReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = pbReadBuf[4];
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	BYTE bWriteBuf[12], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write calibration data
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x0e;  // Write diluted O2 calibration data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE((WORD)(fCalDilO2 * 100.0f + 0.5f)); 
	bWriteBuf[9] = HIBYTE((WORD)(fCalDilO2 * 100.0f + 0.5f));
	bWriteBuf[10] = LOBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	bWriteBuf[11] = HIBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	DWORD dwBytesToWrite(0);
	BYTE *pbWriteBuf(NULL);

	dwBytesToWrite = 8;
	pbWriteBuf = new BYTE[dwBytesToWrite]();

	pbWriteBuf[0] = 0x36;
	pbWriteBuf[1] = 0x00;  // Normal mode
	pbWriteBuf[2] = 0x01;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x00;  // Calibrate Dilution O2 transducer
	pbWriteBuf[5] = 0x40;
	pbWriteBuf[6] = 0x77;
	pbWriteBuf[7] = 0x00;

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == pbReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = pbReadBuf[4];
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	BYTE bWriteBuf[12], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write calibration data
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x0d;  // Write diluted O2 calibration data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE((WORD)(fCalDilO2 * 100.0f + 0.5f)); 
	bWriteBuf[9] = HIBYTE((WORD)(fCalDilO2 * 100.0f + 0.5f));
	bWriteBuf[10] = LOBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	bWriteBuf[11] = HIBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	DWORD dwBytesToWrite(0);
	BYTE *pbWriteBuf(NULL);

	dwBytesToWrite = 8;
	pbWriteBuf = new BYTE[dwBytesToWrite]();

	pbWriteBuf[0] = 0x36;
	pbWriteBuf[1] = 0x00;  // Normal mode
	pbWriteBuf[2] = 0x01;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x00;  // Calibrate Dilution O2 transducer
	pbWriteBuf[5] = 0x20;
	pbWriteBuf[6] = 0x77;
	pbWriteBuf[7] = 0x00;

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == pbReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = pbReadBuf[4];
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	BYTE bWriteBuf[12], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write calibration data
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x0c;  // Write diluted O2 calibration data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE((WORD)(fCalDilO2 * 100.0f + 0.5f)); 
	bWriteBuf[9] = HIBYTE((WORD)(fCalDilO2 * 100.0f + 0.5f));
	bWriteBuf[10] = LOBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	bWriteBuf[11] = HIBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	DWORD dwBytesToWrite(0);
	BYTE *pbWriteBuf(NULL);

	dwBytesToWrite = 8;
	pbWriteBuf = new BYTE[dwBytesToWrite]();

	pbWriteBuf[0] = 0x36;
	pbWriteBuf[1] = 0x00;  // Normal mode
	pbWriteBuf[2] = 0x01;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x00;  // Calibrate Dilution O2 transducer
	pbWriteBuf[5] = 0x10;
	pbWriteBuf[6] = 0x77;
	pbWriteBuf[7] = 0x00;

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == pbReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = pbReadBuf[4];
		}
	}

	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	assert(NULL != pbResult);
	assert(NULL != pfDelayTime);

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = ReadCalibrationData_Flowmeter(pbResult, 0x04, pbReadBuf, dwBytesToRead);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
			if (NULL != pfDelayTime)
			{
				*pfDelayTime = (short)(MAKEWORD(pbReadBuf[4], pbReadBuf[5])) / 10.0f;
			}
		}
	}

	delete[] pbReadBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	assert(NULL != pbResult);
	assert(fDelayTime>10e-6f && fDelayTime<100.0f);

	BYTE bWriteBuf[12], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	WORD wDelayTime = (WORD)(fDelayTime * 10.0f + 0.5f);

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // Short command structure
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write data mode
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x04;  // Write HC delay time data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE(wDelayTime);  // Point 1 SCFM value
	bWriteBuf[9] = HIBYTE(wDelayTime);
	bWriteBuf[10] = 0x00;
	bWriteBuf[11] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	BYTE bWriteBuf[8], bReadBuf[8];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x36;
	bWriteBuf[1] = 0x00;  // short command
	bWriteBuf[2] = 0x01;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x10;  // Calibrate HC delay time
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x47;  // Chksum
	bWriteBuf[7] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 8, bReadBuf, 8, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == bReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = bReadBuf[4];
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	assert(NULL != pbResult);
	assert(NULL != pfDelayTime);

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = ReadCalibrationData_Flowmeter(pbResult, 0x06, pbReadBuf, dwBytesToRead);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
			if (NULL != pfDelayTime)
			{
				*pfDelayTime = (short)(MAKEWORD(pbReadBuf[4], pbReadBuf[5])) / 10.0f;
			}
		}
	}

	delete[] pbReadBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	assert(NULL != pbResult);
	assert(fDelayTime>10e-6f && fDelayTime<100.0f);

	BYTE bWriteBuf[12], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	WORD wDelayTime = (WORD)(fDelayTime * 10.0f + 0.5f);

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // Short command structure
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write data mode
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x06;  // Write O2 delay time data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE(wDelayTime);  // Point 1 SCFM value
	bWriteBuf[9] = HIBYTE(wDelayTime);
	bWriteBuf[10] = 0x00;
	bWriteBuf[11] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	BYTE bWriteBuf[8], bReadBuf[8];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x36;
	bWriteBuf[1] = 0x00;  // short command
	bWriteBuf[2] = 0x01;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x40;  // Calibrate O2 delay time
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x77;  // Chksum
	bWriteBuf[7] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 8, bReadBuf, 8, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == bReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = bReadBuf[4];
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	assert(NULL != pbResult);
	assert(NULL != pfDelayTime);

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = ReadCalibrationData_Flowmeter(pbResult, 0x09, pbReadBuf, dwBytesToRead);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
			if (NULL != pfDelayTime)
			{
				*pfDelayTime = (short)(MAKEWORD(pbReadBuf[4], pbReadBuf[5])) / 10.0f;
			}
		}
	}

	delete[] pbReadBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	assert(NULL != pbResult);
	assert(fDelayTime>10e-6f && fDelayTime<100.0f);

	BYTE bWriteBuf[12], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	WORD wDelayTime = (WORD)(fDelayTime * 10.0f + 0.5f);

	// д��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // Short command structure
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write data mode
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x09;  // Write NOX delay time data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE(wDelayTime);  // Point 1 SCFM value
	bWriteBuf[9] = HIBYTE(wDelayTime);
	bWriteBuf[10] = 0x00;
	bWriteBuf[11] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	BYTE bWriteBuf[8], bReadBuf[8];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// д��ֵ
	bWriteBuf[0] = 0x36;
	bWriteBuf[1] = 0x00;  // short command
	bWriteBuf[2] = 0x01;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;  // Calibrate NOX delay time
	bWriteBuf[5] = 0x02;
	bWriteBuf[6] = 0x39;  // Chksum
	bWriteBuf[7] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 8, bReadBuf, 8, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == bReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = bReadBuf[5];
		}
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::SatrtGetEnvironmentO2_Flowmeter(void)
{
	BYTE bWriteBuf[24], bReadBuf[8];

	// �������̣����������ռ�10�����ݣ�Ȼ�������10���������ƽ��ֵ���ռ�����ƽ��ֵ���洢��������������ɣ���Ȼ�����O2��������̵�ƽ��ֵ

	// ��ʼ�ռ�����
	bWriteBuf[0] = 0x33;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x09;  // data count��������9�����ݣ�2���ֽ�һ�����ݣ�
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Collection mode (Start data collection)
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x0a;  // Collection time (10 sec)
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 0x41;  // RH
	bWriteBuf[9] = 0x00;
	bWriteBuf[10] = 0x16;  // Temp_Ambient
	bWriteBuf[11] = 0x00;
	bWriteBuf[12] = 0x0b;  // Probe Extration
	bWriteBuf[13] = 0x00;
	bWriteBuf[14] = 0x02;  // Unit selection
	bWriteBuf[15] = 0x00;
	bWriteBuf[16] = 0x01;  // Calculation_iterations
	bWriteBuf[17] = 0x00;
	bWriteBuf[18] = 0x00;  // Gas_flow_lower_threshold
	bWriteBuf[19] = 0x5e;
	bWriteBuf[20] = 0x01;  // Gas_temp_upper_limit
	bWriteBuf[21] = 0x01;
	bWriteBuf[22] = 0x00;  // chksum
	bWriteBuf[23] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 24, bReadBuf, 8, true, true);

	return dw;
}

DWORD CNanhuaNha509Encrypt::GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2)
{
#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<GetEnvironmentO2_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif

	assert(NULL != pbResult);
	assert(NULL != pfEnvO2);

	const DWORD dwBytesToRead(8);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();

	DWORD dw = ReadCalibrationData_Flowmeter(pbResult, 0x03, pbReadBuf, dwBytesToRead);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x06 == pbReadBuf[1])
		{
			if (NULL != pfEnvO2)
			{
				*pfEnvO2 = (short)(MAKEWORD(pbReadBuf[4], pbReadBuf[5])) / 100.0f;
			}
		}
	}

	delete[] pbReadBuf;
	return dw;

//	BYTE bWriteBuf[24], bReadBuf[28];
//
//	DWORD dw;
//
//	//����ȡ��������
//	bWriteBuf[0] = 0x34;
//	bWriteBuf[1] = 0x00;  // normal mode
//	bWriteBuf[2] = 0x03;
//	bWriteBuf[3] = 0x00;
//	bWriteBuf[4] = 0x03;  // Data mode 3
//	bWriteBuf[5] = 0x00;
//	bWriteBuf[6] = 0x0a;   //. Cnt start
//	bWriteBuf[7] = 0x00;
//	bWriteBuf[8] = 0x01;  // Cnt Length
//	bWriteBuf[9] = 0x00;
//	bWriteBuf[10] = 0x00;
//	bWriteBuf[11] = 0x00;
//
//	dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
//
//	if(ANA_WR_SUCCEED == dw)
//	{
//		if(bReadBuf[1] == 0x06)  
//		{
//			DWORD dwBytesRead;
//			if(ReadFile(m_hComm, bReadBuf, 28, &dwBytesRead, NULL)==TRUE)  // ����ʣ�໺������
//			{
//
//				//��ȡ��������
//				bWriteBuf[0] = 0x35;
//				bWriteBuf[1] = 0x00;  // normal mode
//				bWriteBuf[2] = 0x02;
//				bWriteBuf[3] = 0x00;
//				bWriteBuf[4] = 0x00;  // Read data
//				bWriteBuf[5] = 0x00;
//				bWriteBuf[6] = 0x03;  // Read O2 background
//				bWriteBuf[7] = 0x00;
//				bWriteBuf[8] = 0x00;
//				bWriteBuf[9] = 0x00;
//
//				ZeroMemory(bReadBuf,28);
//				dw = WriteAndReadData_Flowmeter(bWriteBuf, 10, bReadBuf, 8, true, true);
//				if(ANA_WR_SUCCEED == dw)
//				{
//					if(bReadBuf[1] == 0x06)  
//					{
//#ifdef _DEBUG
//			wchar_t wchOutPutDebugString[2048];
//			swprintf_s(wchOutPutDebugString, L"\r\n���:�ɹ�\r\n");
//			OutputDebugString(wchOutPutDebugString);
//#endif
//						(*pfEnvO2) =  (short)(MAKEWORD(bReadBuf[4], bReadBuf[5])) / 100.0f;
//						(*pbResult) = 0x00;
//						return dw;
//					}
//					else
//					{
//						(*pbResult) = 0x03;
//						return dw;
//					}
//				}
//				else
//				{
//					return dw;
//				}
//			}
//			else
//			{
//				(*pbResult) = 0x02;
//				return dw;
//			}
//		}
//		else
//		{
//			(*pbResult) = 0x01;
//			return dw;
//		}
//	}
//	else
//	{
//		return dw;
//	}
}

DWORD CNanhuaNha509Encrypt::ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead)
{
	assert(NULL != pbResult);
	assert(NULL != pbReadBuf);
	// У���ȡ�ֽ������Ƿ����(�����峤���Ƿ��㹻)
	DWORD dwDataCout(0);
	switch (bCalMode)
	{
	case 1:
		{
			// Cal Mode 1 Read flow meter calibration parameter
			// 4�����ݷ���
			dwDataCout = 4;
		}
		break;
	case 2:
		{
			// Cal Mode 2 Read absolute pressure calibration data
			// 2�����ݷ���
			dwDataCout = 2;
		}
		break;
	case 3:
		{
			// Cal Mode 3 Read O2 background
			// 1�����ݷ���
			dwDataCout = 1;
		}
		break;
	case 4:
		{
			// Cal Mode 4 Read HC delay time data
			// 1�����ݷ���
			dwDataCout = 1;
		}
		break;
	case 5:
		{
			// Cal Mode 5 Read Diluted O2 delay time data ( set=0)
			// 1�����ݷ���
			dwDataCout = 1;
		}
		break;
	case 6:
		{
			// Cal Mode 6 Read electrochemical O2 delay time data
			// 1�����ݷ���
			dwDataCout = 1;
		}
		break;
	case 7:
		{
			// Cal Mode 7 Read Diluted O2 calibration data (%)
			// 8�����ݷ���
			dwDataCout = 8;
		}
		break;
	case 8:
		{
			// û������У׼ģʽ
		}
		break;
	case 9:
		{
			// Cal Mode 9 Read NOX delay times
			// 1�����ݷ���
			dwDataCout = 1;
		}
		break;
	default:
		{
			// �����У׼ģʽ����
		}
		break;
	}
	assert(dwBytesToRead >= (dwDataCout*2+6));

	const DWORD dwBytesToWrite(10);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x35;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x02;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x00;  // Read data
	pbWriteBuf[5] = 0x00;
	pbWriteBuf[6] = bCalMode;  // ����0x03:Read O2 background
	pbWriteBuf[7] = 0x00;
	pbWriteBuf[8] = 0x00;
	pbWriteBuf[9] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);

	delete[] pbWriteBuf;
	return dw;
}

DWORD CNanhuaNha509Encrypt::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
#ifdef _DEBUG
	SYSTEMTIME st;
	GetSystemTime(&st);
	wchar_t wchOutPutDebugString[2048];
	swprintf_s(wchOutPutDebugString, L"\r\n����<RunFan_Flowmeter>ִ��ʱ��:%d-%d-%d %d:%d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
	OutputDebugString(wchOutPutDebugString);
#endif

	const DWORD dwBytesToRead(6);
	BYTE *pbReadBuf = new BYTE[dwBytesToRead]();
	const DWORD dwBytesToWrite(10);
	BYTE *pbWriteBuf = new BYTE[dwBytesToWrite]();
	pbWriteBuf[0] = 0x38;
	pbWriteBuf[1] = 0x00;  // normal mode
	pbWriteBuf[2] = 0x02;
	pbWriteBuf[3] = 0x00;
	pbWriteBuf[4] = 0x01;  // Write I/O control Port
	pbWriteBuf[5] = 0x00;
	if (bRun)
	{
		pbWriteBuf[6] = 0x10;  // PIO�������
	}
	else
	{
		pbWriteBuf[6] = 0x00;  // PIO���ط��
	}
	pbWriteBuf[7] = 0x00;
	pbWriteBuf[8] = 0x00;
	pbWriteBuf[9] = 0x00;
	DWORD dw = WriteAndReadData_Flowmeter(pbWriteBuf, dwBytesToWrite, pbReadBuf, dwBytesToRead, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = pbReadBuf[1];
	}
	delete[] pbReadBuf;
	delete[] pbWriteBuf;
	return dw;

	//BYTE bReadBuf[6];
	//ZeroMemory(bReadBuf, sizeof(bReadBuf));
	//BYTE bWriteBuf[10];
	//bWriteBuf[0] = 0x38;
	//bWriteBuf[1] = 0x00;  // normal mode
	//bWriteBuf[2] = 0x02;
	//bWriteBuf[3] = 0x00;
	//bWriteBuf[4] = 0x01;  // Write I/O control Port
	//bWriteBuf[5] = 0x00;
	//if (bRun)
	//{
	//	bWriteBuf[6] = 0x10;  // PIO�������
	//}
	//else
	//{
	//	bWriteBuf[6] = 0x00;  // PIO���ط��
	//}
	//bWriteBuf[7] = 0x00;
	////WORD wChkSum = bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[4] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7];
	////bWriteBuf[8] = LOBYTE(wChkSum);
	////bWriteBuf[9] = HIBYTE(wChkSum);
	//DWORD dw = WriteAndReadData(bWriteBuf, 10, bReadBuf, 6, false, false);
	//if (ANA_WR_SUCCEED == dw)
	//{
	//	// ��¼���
	//	if (NULL != pbResult)
	//	{
	//		*pbResult = bReadBuf[1];
	//	}

	//	if (0x06 == bReadBuf[1])
	//	{
	//	}
	//}
	//else if (ANA_WR_EQUIP_RETURE_0X15 == dw)
	//{
	//	// ��¼���
	//	if (NULL != pbResult)
	//	{
	//		*pbResult = bReadBuf[1];
	//	}
	//}
	//else
	//{
	//	// ��������
	//	if (NULL != pbResult)
	//	{
	//		*pbResult = 0xFF;
	//	}
	//}
	//return dw;
}

// ��ȡ�ֶ���������
DWORD CNanhuaNha509Encrypt::GetTestResultData(short *const psHCHigh/*=NULL*/, 
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
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp)
{
	assert(NULL != pbResult);
	assert(fCalTemp>-50.0f && fCalTemp<100.0f);

	BYTE bWriteBuf[14], bReadBuf[6];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// ��������
	WORD wCalTemp = WORD(fCalTemp * 10.0f + 0.5f);

	//дѹ��ֵ
	bWriteBuf[0] = 0x35;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x01;  // Write calibration data
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x08;  // Write absolute pressure calibration data
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = LOBYTE((WORD)wCalTemp);  // Write absolute pressure in mbar
	bWriteBuf[9] = HIBYTE((WORD)wCalTemp);
	bWriteBuf[10] = LOBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);
	bWriteBuf[11] = HIBYTE(bWriteBuf[0] + bWriteBuf[1] + bWriteBuf[2] + bWriteBuf[3] + bWriteBuf[3] + bWriteBuf[5] + bWriteBuf[6] + bWriteBuf[7] + bWriteBuf[8] + bWriteBuf[9]);

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 12, bReadBuf, 6, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	return dw;
}

DWORD CNanhuaNha509Encrypt::TempCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	assert(NULL != pbResult);
	assert(NULL != pbCalResult);

	BYTE bWriteBuf[8], bReadBuf[8];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	// ����У׼
	bWriteBuf[0] = 0x36;
	bWriteBuf[1] = 0x00;  // normal mode
	bWriteBuf[2] = 0x01;
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;  // Calibrate absolute pressure transducer
	bWriteBuf[5] = 0x01;
	bWriteBuf[6] = 0x38;  // Check sum
	bWriteBuf[7] = 0x00;

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, 8, bReadBuf, 8, true, true);
	// ��¼���
	if (NULL != pbResult)
	{
		*pbResult = bReadBuf[1];
	}
	// ��¼У׼���
	if (0x06 == bReadBuf[1])
	{
		if (NULL != pbCalResult)
		{
			*pbCalResult = bReadBuf[4];
		}
	}
	return dw;
}

void  CNanhuaNha509Encrypt::TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNanhuaNha509Encrypt *pThis = (CNanhuaNha509Encrypt *)dwUser;

	if (pThis->m_enMode != CAna::ToAnalyzer)
	{
		return;
	}

	TRACE("TimePro running...\n");
	BYTE bStatus(0x00);
	pThis->GetStatus(&bStatus);
}