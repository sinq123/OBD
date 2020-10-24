#include "StdAfx.h"
#include "NanhuaNht6.h"

#include <cassert>

CNanhuaNht6::CNanhuaNht6(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CNanhuaNht6::~CNanhuaNht6(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNht6::Open(BYTE bPort)
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
	// ���г�ʱ���ã�100����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 20;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 100;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return OPA_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaNht6::Close(void)
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

bool CNanhuaNht6::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNht6::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CNanhuaNht6::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

BYTE CNanhuaNht6::CheckSum(BYTE *pb, UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	bSum = ~bSum + 1;
	return bSum;                

}

// ��ֹԤ��	
DWORD CNanhuaNht6::StopWarmUp(void)
{
	BYTE bWriteBuf[2] = {0xA2, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// У׼
DWORD CNanhuaNht6::Calibrate(void)
{
	BYTE bWriteBuf[2] = {0xA4, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// ������ֵ
DWORD CNanhuaNht6::ClearMaxValue(void)
{
	BYTE bWriteBuf[2] = {0xA7,0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf,  2, true, true);
	return dw;
}

DWORD CNanhuaNht6::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ���ÿ��Ƶ�Ԫ����
DWORD CNanhuaNht6::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// �������ʪ��
DWORD CNanhuaNht6::SetRH(float fStdValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ��λEEPROM����
DWORD CNanhuaNht6::ResetEEPROM(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ����������Ϣ
DWORD CNanhuaNht6::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	assert(NULL != pbs16AlarmInfo);

	BYTE bWriteBuf[2] = {0xA3, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (0UL == dw)
	{
		//*pwAlarmInfo = MAKEWORD(bReadBuf[2], bReadBuf[1]);
		*pbs16AlarmInfo = MAKEWORD(bReadBuf[2], bReadBuf[1]);
	}
	else
	{
		// ��������
		//pwAlarmInfo = 0;
		*pbs16AlarmInfo = 0x0000;
	}
	return dw;
}

// ȡʵʱ����
DWORD CNanhuaNht6::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[2] = {0xA5, 0};
	BYTE bReadBuf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 10, true, true);
	if (0UL == dw)
	{
		*pfN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		*pfK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		*pusRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		*pusOilTemp = MAKEWORD(bReadBuf[8], bReadBuf[7]);
		if (0xFFFF == *pusOilTemp)
		{
			*pusOilTemp = 0;
		}
		else
		{
			*pusOilTemp -= 273;
		}
	}
	else
	{
		// ��������
		*pfN = 0.0f;
		*pfK = 0.0f;
		*pusRpm = 0;
		*pusOilTemp = 0;
	}
	return dw;
}

// ȡ������Ԫ�ڲ�����
DWORD CNanhuaNht6::GetMeasureUnitData(float *pfOriginalN, 
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
	//return OPA_WR_EQUIP_RETURE_0X15;

	assert(NULL!=pfOriginalN);

	BYTE bWriteBuf[2] = {0xB0, 0};
	BYTE bReadBuf[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 20, true, true);
	if (0UL == dw)
	{
		*pfOriginalN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
	}
	else
	{
		// ��������
		*pfOriginalN = 0.0f;
	}
	return dw;
}

// ȡ���ֵ
DWORD CNanhuaNht6::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	assert(NULL!=pfMaxN && NULL!=pfMaxK && NULL!=pusMaxRpm);

	BYTE bWriteBuf[2] = {0xA6, 0};
	BYTE bReadBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 8, true, true);
	if (0UL == dw)
	{
		*pfMaxN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		*pusMaxRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
	}
	else
	{
		// ��������
		*pfMaxN = 0.0f;
		*pfMaxK = 0.0f;
		*pusMaxRpm = 0;
	}
	return dw;
}

// ȡ����״̬
DWORD CNanhuaNht6::GetStatus(BYTE *pbStatus)
{
	//return OPA_WR_EQUIP_RETURE_0X15;

	assert(NULL != pbStatus);

	BYTE bWriteBuf[2] = {0xA1, 0};
	BYTE bReadBuf[3] = {0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf,  3, true, true);
	*pbStatus = bReadBuf[1];
	return dw;
}

// ������״̬
DWORD CNanhuaNht6::SetStatus(const BYTE bStatus)
{
	// ����״̬���Ƿ�Ϸ�
	assert(bStatus>0 && bStatus<5);

	BYTE bWriteBuf[3] = {0xA0, bStatus, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 3, bReadBuf,  2, true, true);
	return dw;
}

// ȡԤ��ʣ��ʱ��
DWORD CNanhuaNht6::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CNanhuaNht6::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�汾��Ϣ
DWORD CNanhuaNht6::GetVersion(float *pfVersion)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNht6::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ���Ƶ�Ԫ����
DWORD CNanhuaNht6::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CNanhuaNht6::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�ֶ��������
DWORD CNanhuaNht6::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	BYTE bWriteBuf[2] = {0xB1,0};
	BYTE bReadBuf[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 12, true, true);
	if (0UL == dw)
	{
		if (NULL != pfValueK1)
		{
			*pfValueK1= MAKEWORD(bReadBuf[2], bReadBuf[1]) / 100.0f;
		}
		if (NULL != pfValueK2)
		{
			*pfValueK2 = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pfValueK3)
		{
			*pfValueK3 = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 100.0f;
		}
		if (NULL != pfValueKAvg)
		{
			*pfValueKAvg = MAKEWORD(bReadBuf[8], bReadBuf[7]) / 100.0f;
		}
		if(NULL != psLowRpm)
		{
			*psLowRpm = MAKEWORD(bReadBuf[10], bReadBuf[9]);
		}
	}
	else
	{
		// ��������

		if (NULL != pfValueK1)
		{
			*pfValueK1 = 0.0f;
		}
		if (NULL != pfValueK2)
		{
			*pfValueK2 = 0.0f;
		}
		if (NULL != pfValueK3)
		{
			*pfValueK3 = 0.0f;
		}
		if (NULL != pfValueKAvg)
		{
			*pfValueKAvg = 0.0f;
		}	
	}
	return dw;

}

DWORD CNanhuaNht6::EntrySensitivityCalStatus()
{

	BYTE bWriteBuf[2] = {0xC1, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

DWORD CNanhuaNht6::ProceedSensitivityCal(float const pfValue)
{
	BYTE bWriteBuf[4] = {0xC2, 0, 0, 0};
	BYTE bReadBuf[2] = {0, 0};
	bWriteBuf[1] = HIBYTE((WORD)(pfValue * 10.0f + 0.5f));
	bWriteBuf[2] = LOBYTE((WORD)(pfValue * 10.0f + 0.5f)); 
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 2, true, true);
	return dw;
}

DWORD CNanhuaNht6::QuitSensitivityCalStatus()
{
	BYTE bWriteBuf[2] = {0xC3, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}