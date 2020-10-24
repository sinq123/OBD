#include "StdAfx.h"
#include "NanhuaNHT610.h"
#include "md5.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cassert>

#include "NHA509EnLib\NHA509En.h"
#ifdef _DEBUG
#pragma comment(lib, "NHA509EnLib\\NHA509En_D.lib")
#else
#pragma comment(lib, "NHA509EnLib\\NHA509En.lib")
#endif

CNanhuaNHT610::CNanhuaNHT610(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);

	m_unTimerID = timeSetEvent(200*1000, 1, (LPTIMECALLBACK)TimePro, (DWORD_PTR)this, TIME_PERIODIC);
}

CNanhuaNHT610::~CNanhuaNHT610(void)
{
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
	}

	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNHT610::Open(BYTE bPort)
{
	assert(0 != bPort);

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
		return OPA_OPEN_FAIL;
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
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return OPA_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaNHT610::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// �رմ��ڳɹ�
			return OPA_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			return OPA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
		return OPA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CNanhuaNHT610::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNHT610::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CNanhuaNHT610::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
{
	//assert(NULL!=pbWriteBuffer);
	//assert(1UL<dwWriteLength);
	//assert(NULL!=pbReadBuffer);
	//assert(1UL<dwReadLength);

	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// �ֶ��ζ�������һ������ͷ��1byte�����ڶ��ζ�ʣ�µ���Ч����

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1UL==dwBytesRead)
		{
			// �жϷǷ������ָ���
			if(0x15 != pbReadBuffer[0])
			{
				// NHT-1L�϶�����������

				// �����µ�����
				bSucceed = ReadFile(m_hComm, pbReadBuffer+1, dwReadLength-1, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-1))
				{
					if (bNeedReadChkSum)
					{
						// ��Ҫ��У��
						if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer,dwReadLength-1))
						{
							::LeaveCriticalSection(&cInfoSection);
							// ��������У���������ʧ��
							return OPA_WR_CHECK_SUM_FAIL;
						}
						else
						{
							::LeaveCriticalSection(&cInfoSection);
							// �����ɹ�
							return OPA_WR_SUCCEED;
						}
					}
					else
					{
						::LeaveCriticalSection(&cInfoSection);
						// �����ɹ�
						return OPA_WR_SUCCEED;
					}
				}
				else
				{
					// ͨѶʧ��
					m_bStatus = 5;

					::LeaveCriticalSection(&cInfoSection);
					// �������󣺶�ָ������ʧ�ܣ���������Ч����ʧ�ܣ�
					return OPA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				// ������״̬��
				ReadFile(m_hComm, &m_bLastErrorCode, 1, &dwBytesRead, NULL);
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				//m_bLastErrorCode = pbReadBuffer[1];

				::LeaveCriticalSection(&cInfoSection);
				// ���������������ش���״̬��
				return OPA_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			// ͨѶʧ��
			m_bStatus = 5;

			::LeaveCriticalSection(&cInfoSection);
			// �������󣺶�ָ������ʧ�ܣ�������ͷʧ�ܣ�
			return OPA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// ���󣺴���û�д�
		return OPA_WR_PORT_NOT_OPEN;
	}
}

BYTE CNanhuaNHT610::CheckSum(BYTE *pb, UINT un)
{
	if (un == 0)
	{
		return 0;
	}
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	//bSum = ~bSum + 1;
	return bSum;                

}

// ��ֹԤ��	
DWORD CNanhuaNHT610::StopWarmUp(void)
{
	BYTE bWriteBuf[1] = {0x29};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 1, false, false);
	return dw;
}

// У׼
DWORD CNanhuaNHT610::Calibrate(void)
{
	BYTE bWriteBuf[1] = {0x1F};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 1, false, false);
	return dw;
}

// ������ֵ
DWORD CNanhuaNHT610::ClearMaxValue(void)
{
	BYTE bWriteBuf[1] = {0x1E};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf,  1, false, false);
	return dw;
}

DWORD CNanhuaNHT610::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ���ÿ��Ƶ�Ԫ����
DWORD CNanhuaNHT610::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// �������ʪ��
DWORD CNanhuaNHT610::SetRH(float fStdValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ��λEEPROM����
DWORD CNanhuaNHT610::ResetEEPROM(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ����������Ϣ
DWORD CNanhuaNHT610::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	assert(NULL != pbs16AlarmInfo);

	BYTE bWriteBuf[1] = {0x1B};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 4, false, true);
	if (0UL == dw)
	{
		if (pbs16AlarmInfo != NULL)
		{
			*pbs16AlarmInfo = MAKEWORD(bReadBuf[2], bReadBuf[1]);
		}
	}
	else
	{
		// ��������
		if (pbs16AlarmInfo != NULL)
		{
			*pbs16AlarmInfo = 0x0000;
		}
	}
	return dw;
}

// ȡʵʱ����
DWORD CNanhuaNHT610::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	short sNO(0), sNO2(0), sHC(0);
	float fCO2(0), fCO(0), fO2(0); 
	return GetRealTimeData(pfN, pfK, pusRpm, pusOilTemp, &sNO, &sNO2, &fCO2, &sHC, &fCO, &fO2);
}

DWORD CNanhuaNHT610::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp, 
							short *const psNO, short *const psNO2, float *const pfCO2, 
							short *const psHC, float *const pfCO, float *const pfO2)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[1] = {0x2D};
	BYTE bReadBuf[34] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 1, bReadBuf, 34, false, true);
	if (OPA_WR_SUCCEED == dw)
	{
		if (psHC != NULL)
		{
			*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
		}
		if (pfCO != NULL)
		{
			*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		if (pfCO2 != NULL)
		{
			*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		if (pfO2 != NULL)
		{
			*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
		}
		if (psNO != NULL)
		{
			*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		if (psNO2 != NULL)
		{
			*psNO2 = (short)(MAKEWORD(bReadBuf[12], bReadBuf[11]));
		}
		if (pfN != NULL)
		{
			*pfN = MAKEWORD(bReadBuf[16], bReadBuf[15]) / 10.0f;
		}
		if (pfK != NULL)
		{
			*pfK = MAKEWORD(bReadBuf[18], bReadBuf[17]) / 100.0f;
		}
		if (pusRpm != NULL)
		{
			*pusRpm = MAKEWORD(bReadBuf[20], bReadBuf[19]);
		}
		if (pusOilTemp != NULL)
		{
			*pusOilTemp = (short)MAKEWORD(bReadBuf[22], bReadBuf[21]) / 10.0f;
		}
		
		WORD wLeft = MAKEWORD(bReadBuf[32], bReadBuf[31]) & 0x0FFF;
		bool bLock = ((bReadBuf[31]&0x80) != 0x00);
		BYTE bCheckTimes = (bReadBuf[31]&0x70)>>4;
		if (wLeft < 300)
		{
			Authentication();
		}
	}
	else
	{
		// ��������
		if (psHC != NULL)
		{
			*psHC =  0;
		}
		if (pfCO != NULL)
		{
			*pfCO = 0.0f;
		}
		if (pfCO2 != NULL)
		{
			*pfCO2 = 0.0f;
		}
		if (pfO2 != NULL)
		{
			*pfO2 = 0.0f;
		}
		if (psNO != NULL)
		{
			*psNO = 0;
		}
		if (psNO2 != NULL)
		{
			*psNO2 = 0;
		}
		if (pfN != NULL)
		{
			*pfN = 0.0f;
		}
		if (pfK != NULL)
		{
			*pfK = 0.0f;
		}
		if (pusRpm != NULL)
		{
			*pusRpm = 0;
		}
		if (pusOilTemp != NULL)
		{
			*pusOilTemp = 0;
		}

		if (OPA_WR_EQUIP_RETURE_0X15 == dw)
		{
			Authentication();
		}
	}
	return dw; 
}

// ȡ������Ԫ�ڲ�����
DWORD CNanhuaNHT610::GetMeasureUnitData(float *pfOriginalN, 
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
	BYTE bWriteBuf[1] = {0x1C};
	BYTE bReadBuf[18];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 18, false, true);
	if (0UL == dw)
	{
		if (NULL != pfOriginalN)
		{
			*pfOriginalN =  MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		}
		if (NULL != pfOriginalK)
		{
			*pfOriginalK =  MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusSmokeTemp)
		{
			*pusSmokeTemp = bReadBuf[5];
		}
		if (NULL != pusTubeTemp)
		{
			*pusTubeTemp = bReadBuf[6];
		}
		if (NULL != pusDetectorTemp)
		{
			*pusDetectorTemp = bReadBuf[7];
		}
		if (NULL != pusLEDTemp)
		{
			*pusLEDTemp = bReadBuf[8];
		}
		if (NULL != pusBoardTemp)
		{
			*pusBoardTemp = bReadBuf[9];
		}
		if (NULL != pusLeftFanCurrent)
		{
			*pusLeftFanCurrent = bReadBuf[10] * 2;
		}
		if (NULL != pusRightFanCurrent)
		{
			*pusRightFanCurrent = bReadBuf[11] * 2;
		}
		if (NULL != pfPowerVoltage)
		{
			*pfPowerVoltage = bReadBuf[12] / 10.0f;
		}
		if (NULL != pusRealTimeIntensity)
		{
			*pusRealTimeIntensity = MAKEWORD(bReadBuf[14], bReadBuf[13]);
		}
		if (NULL != pusFullShadingIntensity)
		{
			*pusFullShadingIntensity = MAKEWORD(bReadBuf[16], bReadBuf[15]);
		}
	}
	else
	{
		// ��������

		if (NULL != pfOriginalN)
		{
			*pfOriginalN =  0.0f;
		}
		if (NULL != pfOriginalK)
		{
			*pfOriginalK =  0.0f;
		}
		if (NULL != pusSmokeTemp)
		{
			*pusSmokeTemp = 0;
		}
		if (NULL != pusTubeTemp)
		{
		*pusTubeTemp = 0;
		}
		if (NULL != pusDetectorTemp)
		{
			*pusDetectorTemp = 0;
		}
		if (NULL != pusLEDTemp)
		{
			*pusLEDTemp = 0;
		}
		if (NULL != pusBoardTemp)
		{
			*pusBoardTemp = 0;
		}
		if (NULL != pusLeftFanCurrent)
		{
			*pusLeftFanCurrent = 0;
		}
		if (NULL != pusRightFanCurrent)
		{
			*pusRightFanCurrent = 0;
		}
		if (NULL != pfPowerVoltage)
		{
			*pfPowerVoltage = 0.0f;
		}
		if (NULL != pusRealTimeIntensity)
		{
			*pusRealTimeIntensity = 0;
		}
		if (NULL != pusFullShadingIntensity)
		{
			*pusFullShadingIntensity = 0;
		}
	}
	return dw;
}

// ȡ���ֵ
DWORD CNanhuaNHT610::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	assert(NULL!=pfMaxN && NULL!=pfMaxK && NULL!=pusMaxRpm);

	BYTE bWriteBuf[1] = {0x1D};
	BYTE bReadBuf[8] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 1, bReadBuf, 8, false, true);
	if (OPA_WR_SUCCEED == dw)
	{
		if (pfMaxN != NULL)
		{
			*pfMaxN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		}
		if (pfMaxK != NULL)
		{
			*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (pusMaxRpm != NULL)
		{
			*pusMaxRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		}
	}
	else
	{
		// ��������
		if (pfMaxN != NULL)
		{
			*pfMaxN = 0.0f;
		}
		if (pfMaxK != NULL)
		{
			*pfMaxK = 0.0f;
		}
		if (pusMaxRpm != NULL)
		{
			*pusMaxRpm = 0;
		}
	}
	return dw;
}

// ȡ����״̬
DWORD CNanhuaNHT610::GetStatus(BYTE *pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[1] = {0x1A};
	BYTE bReadBuf[3] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 3, false, true);
	if (OPA_WR_SUCCEED == dw)
	{
		if (pbStatus != NULL)
		{
			*pbStatus = bReadBuf[1];	// ����ͨѶ����
		}

		// ����Ƿ���Ҫ����
		bWriteBuf[0] = 0x13;
		memset(bReadBuf, 0, 3);
		DWORD dw2 = WriteAndReadData(bWriteBuf, 1, bReadBuf, 3, false, false);
		if (OPA_WR_SUCCEED == dw2)
		{
			if (bReadBuf[0] == 0xf0)
			{
				bReadBuf[0] = 0x00;
			}
			else if (bReadBuf[0] == 0xf1)
			{
				bReadBuf[0] = 0x01;
			}
			WORD wLeft = MAKEWORD(bReadBuf[2], bReadBuf[1]) & 0x0FFF;
			bool bLock = ((bReadBuf[1]&0x80) != 0x00);
			BYTE bCheckTimes = (bReadBuf[1]&0x70)>>4;
			//TRACE("GetStatus(), *pbStatus=%02X,wLeft=%d,bLock=%d,bCheckTimes=%d\n", *pbStatus, wLeft, bLock, bCheckTimes);

			if ((wLeft<300) && ((bReadBuf[0]==0x00) || (bReadBuf[0]==0x01)))
			{
				Authentication();
			}
		}
	}
	return dw;
}

// ������״̬
DWORD CNanhuaNHT610::SetStatus(const BYTE bStatus)
{
	// ����״̬���Ƿ�Ϸ�
	assert(bStatus>0 && bStatus<5);

	BYTE bWriteBuf[3] = {0x19, bStatus, 0};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 3, bReadBuf,  1, true, true);
	return dw;
}

// ȡԤ��ʣ��ʱ��
DWORD CNanhuaNHT610::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CNanhuaNHT610::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	assert(NULL != pfET);
	assert(NULL != pfAP);
	assert(NULL != pfRH);

	BYTE bWriteBuf(0x02);
	BYTE bReadBuf[8];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, bReadBuf, 8, false, true);
	if (OPA_WR_SUCCEED == dw)
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

// ȡ�汾��Ϣ
DWORD CNanhuaNHT610::GetVersion(float *pfVersion)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNHT610::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ���Ƶ�Ԫ����
DWORD CNanhuaNHT610::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CNanhuaNHT610::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�ֶ��������
DWORD CNanhuaNHT610::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNHT610::EntrySensitivityCalStatus()
{
	BYTE bWriteBuf[1] = {0x24};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 1, false, false);
	return dw;
}

DWORD CNanhuaNHT610::ProceedSensitivityCal(float const pfValue)
{
	BYTE bWriteBuf[4] = {0x25, 0, 0, 0};
	BYTE bReadBuf[1] = {0};
	bWriteBuf[1] = HIBYTE((WORD)(pfValue * 10.0f + 0.5f));
	bWriteBuf[2] = LOBYTE((WORD)(pfValue * 10.0f + 0.5f)); 
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 1, true, false);
	return dw;
}

DWORD CNanhuaNHT610::QuitSensitivityCalStatus()
{
	BYTE bWriteBuf[1] = {0x28};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 1, bReadBuf, 1, false, false);
	return dw;
}

bool CNanhuaNHT610::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
{
	if (!IsOpen())
	{
		return false;
	}

	DWORD dwBytesWritten(0x00);
	// �����������
	//PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
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

bool CNanhuaNHT610::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
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

DWORD CNanhuaNHT610::Authentication(void)
{
	::EnterCriticalSection(&cInfoSection);
	if (IsOpen())
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
							return OPA_WR_SUCCEED;
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
		return OPA_WR_READ_SPECIFIED_DATA_FAIL;
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);

		// �������󣺴���û�д�
		return OPA_WR_PORT_NOT_OPEN;
	}
}

void  CNanhuaNHT610::TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNanhuaNHT610 *pThis = (CNanhuaNHT610 *)dwUser;
	
	TRACE("TimePro running...\n");
	pThis->Authentication();
}