#include "StdAfx.h"
#include "MingQuanMQY200.h"

#include <cassert>

CMingQuanMQY200::CMingQuanMQY200(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CMingQuanMQY200::~CMingQuanMQY200(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}


// �򿪴���
DWORD CMingQuanMQY200::Open(BYTE bPort)
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
DWORD CMingQuanMQY200::Close(void)
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

bool CMingQuanMQY200::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CMingQuanMQY200::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CMingQuanMQY200::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

BYTE CMingQuanMQY200::CheckSum(BYTE *pb, UINT un)
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
DWORD CMingQuanMQY200::StopWarmUp(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// У׼
DWORD CMingQuanMQY200::Calibrate(void)
{
	BYTE bWriteBuf[2] = {0xA2, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// ������ֵ
DWORD CMingQuanMQY200::ClearMaxValue(void)
{
	BYTE bWriteBuf[2] = {0xA8, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

DWORD CMingQuanMQY200::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ���ÿ��Ƶ�Ԫ����
DWORD CMingQuanMQY200::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// �������ʪ��
DWORD CMingQuanMQY200::SetRH(float fStdValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ��λEEPROM����
DWORD CMingQuanMQY200::ResetEEPROM(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ����������Ϣ
DWORD CMingQuanMQY200::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡʵʱ����
DWORD CMingQuanMQY200::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[2] = {0xA6, 0};
	BYTE bReadBuf[10] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 10, true, true);
	if (0UL == dw)
	{
		if (NULL != pfN)
		{
			*pfN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		}
		if (NULL != pfK)
		{
			*pfK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		}
		if (NULL != pusOilTemp)
		{
			*pusOilTemp = MAKEWORD(bReadBuf[8], bReadBuf[7])- 273;
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
DWORD CMingQuanMQY200::GetMeasureUnitData(float *pfOriginalN, 
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
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ���ֵ
DWORD CMingQuanMQY200::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	BYTE bWriteBuf[2] = {0xA9, 0};
	BYTE bReadBuf[8];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 8, true, true);
	if (0UL == dw)
	{
		if (NULL != pfMaxN)
		{
			*pfMaxN = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
		}
		if (NULL != pfMaxK)
		{
			*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusMaxRpm)
		{
			*pusMaxRpm = MAKEWORD(bReadBuf[6], bReadBuf[5]);
		}
	}
	else
	{
		// ��������

		if (NULL != pfMaxN)
		{
			*pfMaxN = 0.0f;
		}
		if (NULL != pfMaxK)
		{
			*pfMaxK = 0.0f;
		}
		if (NULL != pusMaxRpm)
		{
			*pusMaxRpm = 0;
		}
	}
	return dw;
}

// ȡ����״̬
DWORD CMingQuanMQY200::GetStatus(BYTE *pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[2] = {0xA1, 0};
	BYTE bReadBuf[3] = {0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 3, true, true);
	if (0UL == dw)
	{
		*pbStatus = m_bStatus = bReadBuf[1];
	}
	return dw;
}

// ������״̬
DWORD CMingQuanMQY200::SetStatus(const BYTE bStatus)
{
	BYTE bWriteBuf[3] = {0xA0, bStatus, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 3, bReadBuf, 2, true, true);
	return dw;
}

// ȡԤ��ʣ��ʱ��
DWORD CMingQuanMQY200::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CMingQuanMQY200::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�汾��Ϣ
DWORD CMingQuanMQY200::GetVersion(float *pfVersion)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CMingQuanMQY200::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ���Ƶ�Ԫ����
DWORD CMingQuanMQY200::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CMingQuanMQY200::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�ֶ��������
DWORD CMingQuanMQY200::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}