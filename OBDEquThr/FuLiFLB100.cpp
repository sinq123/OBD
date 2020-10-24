#include "StdAfx.h"
#include "FuLiFLB100.h"

#include <cassert>

CFuLiFLB100::CFuLiFLB100(void)
: m_bStatus(0U)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CFuLiFLB100::~CFuLiFLB100(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}


// �򿪴���
DWORD CFuLiFLB100::Open(BYTE bPort)
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
DWORD CFuLiFLB100::Close(void)
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

bool CFuLiFLB100::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CFuLiFLB100::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CFuLiFLB100::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

BYTE CFuLiFLB100::CheckSum(BYTE *pb, UINT un)
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
DWORD CFuLiFLB100::StopWarmUp(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// У׼
DWORD CFuLiFLB100::Calibrate(void)
{
	BYTE bWriteBuf[4] = {0x02, 0x04, 0x05, 0xf5};
	BYTE bReadBuf[4] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 4, false, false);
	return dw;
}

// ������ֵ
DWORD CFuLiFLB100::ClearMaxValue(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLB100::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ���ÿ��Ƶ�Ԫ����
DWORD CFuLiFLB100::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// �������ʪ��
DWORD CFuLiFLB100::SetRH(float fStdValue)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ��λEEPROM����
DWORD CFuLiFLB100::ResetEEPROM(void)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ����������Ϣ
DWORD CFuLiFLB100::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡʵʱ����
DWORD CFuLiFLB100::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[4] = {0x02, 0x04, 0x02, 0xf8};
	BYTE bReadBuf[13] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 2, bReadBuf, 13, false, false);
	if (0UL == dw)
	{
		*pfN = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 10.0f;
		*pfK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		*pusRpm = MAKEWORD(bReadBuf[11], bReadBuf[10]);
		*pusOilTemp = MAKEWORD(bReadBuf[9], bReadBuf[8]);
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
DWORD CFuLiFLB100::GetMeasureUnitData(float *pfOriginalN, 
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
DWORD CFuLiFLB100::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	// ֹͣ����
	BYTE bWriteBuf[4] = {0x02, 0x04, 0x09, 0xf1};
	BYTE bReadBuf[12];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 4, false, false);

	// ȡ��
	bWriteBuf[0] = 0x02;
	bWriteBuf[1] = 0x04;
	bWriteBuf[2] = 0x03;
	bWriteBuf[3] = 0xf7;
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 12, false, false);

	if (0UL == dw)
	{
		if (NULL != pfMaxN)
		{
			*pfMaxN = MAKEWORD(bReadBuf[6], bReadBuf[5]) / 10.0f;
		}
		if (NULL != pfMaxK)
		{
			*pfMaxK = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 100.0f;
		}
		if (NULL != pusMaxOilTemp)
		{
			*pusMaxOilTemp = MAKEWORD(bReadBuf[10], bReadBuf[9]);
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
		if (NULL != pusMaxOilTemp)
		{
			*pusMaxOilTemp = 0;
		}
	}
	return dw;
}

// ȡ����״̬
DWORD CFuLiFLB100::GetStatus(BYTE *pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[4] = {0x02, 0x04, 0x01, 0xf9};
	BYTE bReadBuf[7] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 7, false, false);
	if (0UL == dw)
	{
		if (bReadBuf[3]&0x02)
		{
			m_bStatus = 0x00;
		}
		else if (bReadBuf[3]&0x01)
		{
			m_bStatus = 0x01;
		}
		else
		{
			m_bStatus = 0xFF;
		}
		*pbStatus = m_bStatus;
	}
	return dw;
}

// ������״̬
DWORD CFuLiFLB100::SetStatus(const BYTE bStatus)
{
	BYTE bWriteBuf[4] = {0};
	BYTE bReadBuf[4] = {0};
	if (bStatus == 0x01)
	{
		bWriteBuf[0] = 0x02;
		bWriteBuf[1] = 0x04;
		bWriteBuf[2] = 0x07;
		bWriteBuf[3] = 0xf3;
	}
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 4, false, false);
	return dw;
}

// ȡԤ��ʣ��ʱ��
DWORD CFuLiFLB100::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CFuLiFLB100::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�汾��Ϣ
DWORD CFuLiFLB100::GetVersion(float *pfVersion)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

DWORD CFuLiFLB100::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ���Ƶ�Ԫ����
DWORD CFuLiFLB100::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CFuLiFLB100::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�ֶ��������
DWORD CFuLiFLB100::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}