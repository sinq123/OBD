#include "StdAfx.h"
#include "FoFenFTY100.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cassert>

CFoFenFTY100::CFoFenFTY100(void)
: m_bStatus(0U)
, m_fMaxValueK(0.0f)
, m_fMaxValueN(0.0f)
{
	//::InitializeCriticalSection(&cInfoSection);
}

CFoFenFTY100::~CFoFenFTY100(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}


// �򿪴���
DWORD CFoFenFTY100::Open(BYTE bPort)
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
DWORD CFoFenFTY100::Close(void)
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

bool CFoFenFTY100::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CFoFenFTY100::GetErrorCodeFromEquipment(void) const
{
	return m_bLastErrorCode;
}

DWORD CFoFenFTY100::WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD dwReadLength, bool bNeedWriteChkSum, bool bNeedReadChkSum)
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

BYTE CFoFenFTY100::CheckSum(BYTE *pb, UINT un)
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
DWORD CFoFenFTY100::StopWarmUp(void)
{
	BYTE bWriteBuf[2] = {0xA2, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// У׼
DWORD CFoFenFTY100::Calibrate(void)
{
	BYTE bWriteBuf[7] = {'f', 't', 'y', 0x01, 0x02, 0x05, 0xA5};
	BYTE bReadBuf[4] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 4, false, false);
	return dw;
}

// ������ֵ
DWORD CFoFenFTY100::ClearMaxValue(void)
{
	m_fMaxValueK = 0.0f;
	m_fMaxValueN = 0.0f;
		
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
	}
	m_unTimerID = timeSetEvent(500, 1000, (LPTIMECALLBACK)TimePro, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerID)
	{
		// ������ý�嶨ʱ��ʧ��
		//TRACE("������ý�嶨ʱ��ʧ��");
	}

	return OPA_WR_SUCCEED;
}

DWORD CFoFenFTY100::SetControlUnitParams(const ControlUnitParams *const psControlUnitParams)
{
	assert(NULL != psControlUnitParams);

	if (EngineStroke == psControlUnitParams->pt)
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.est));
	}
	else if (RpmMeasurement == psControlUnitParams->pt)
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.rmt));
	}
	else if (FilterParam == psControlUnitParams->pt)
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.fpt));
	}
	else
	{
		return SetControlUnitParams(BYTE(psControlUnitParams->pt), BYTE(psControlUnitParams->pv.NoParam));
	}
}

// ���ÿ��Ƶ�Ԫ����
DWORD CFoFenFTY100::SetControlUnitParams(BYTE bParamType, BYTE bParamValue)
{
	BYTE bWriteBuf[4] = {0xB2, bParamType, bParamValue, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf, 2, true, true);
	return dw;
}

// �������ʪ��
DWORD CFoFenFTY100::SetRH(float fStdValue)
{
	assert(fStdValue>10e-6f && fStdValue<100.0f);

	// �Ŵ�10��
	WORD wRH = WORD(fStdValue * 10.0f + 0.5f);
	BYTE bWriteBuf[4] = {0xB5, HIBYTE(wRH), LOBYTE(wRH), 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 4, bReadBuf,2, true, true);
	return dw;
}

// ��λEEPROM����
DWORD CFoFenFTY100::ResetEEPROM(void)
{
	BYTE bWriteBuf[2] = {0xB6, 0};
	BYTE bReadBuf[2] = {0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 2, true, true);
	return dw;
}

// ȡ����������Ϣ
DWORD CFoFenFTY100::GetAlarmInfo(std::bitset<16> *const pbs16AlarmInfo)
{
	assert(NULL != pbs16AlarmInfo);

	BYTE bWriteBuf[2] = {0xA3, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (OPA_WR_SUCCEED == dw)
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
DWORD CFoFenFTY100::GetRealTimeData(float *pfN, float *pfK, USHORT *pusRpm, USHORT *pusOilTemp)
{
	assert(NULL!=pfN && NULL!=pfK && NULL!=pusRpm && NULL!=pusOilTemp);

	BYTE bWriteBuf[7] = {'f', 't', 'y', 0x01, 0x02, 0x02, 0xA8};
	BYTE bReadBuf[10] = {0};
	DWORD dw =WriteAndReadData(bWriteBuf, 7, bReadBuf, 10, false, false);
	if (OPA_WR_SUCCEED == dw)
	{
		*pfN = MAKEWORD(bReadBuf[4], bReadBuf[3]) / 10.0f;
		*pfK = N2K(*pfN, 430);
		*pusRpm = MAKEWORD(bReadBuf[8], bReadBuf[7]);
		*pusOilTemp = MAKEWORD(bReadBuf[6], bReadBuf[5]);
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
DWORD CFoFenFTY100::GetMeasureUnitData(float *pfOriginalN, 
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
	BYTE bWriteBuf[2] = {0xB0, 0};
	BYTE bReadBuf[18];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 18, true, true);
	if (OPA_WR_SUCCEED == dw)
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
DWORD CFoFenFTY100::GetMaxValue(float *pfMaxN, float *pfMaxK, USHORT *pusMaxRpm, USHORT *pusMaxOilTemp)
{
	*pfMaxN = m_fMaxValueN;
	*pfMaxK = m_fMaxValueK;
	return OPA_WR_SUCCEED;
}

// ȡ����״̬����ֲ�͸��FTY-100
DWORD CFoFenFTY100::GetStatus(BYTE *pbStatus)
{
	assert(NULL != pbStatus);

	BYTE bWriteBuf[7] = {'f', 't', 'y', 0x01, 0x02, 0x01, 0xA9};
	BYTE bReadBuf[7] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false);
	if (OPA_WR_SUCCEED == dw)
	{
		// ���״̬ת��Ϊ�ϻ�״̬
		if (bReadBuf[3] & 0x02)
		{
			// Ԥ��
			m_bStatus = 0x00;
			*pbStatus = 0x00;
		}
		else if (bReadBuf[3] & 0x04)
		{
			// ʵʱ����
			m_bStatus = 0x01;
			*pbStatus = 0x01;
		}
		else
		{
			m_bStatus = 0xff;
			*pbStatus = 0xff;
		}
	}
	return dw;
}

// ������״̬
DWORD CFoFenFTY100::SetStatus(const BYTE bStatus)
{
	BYTE bWriteBuf[7] = {'f', 't', 'y', 0x01, 0x02, bStatus, 0x00};
	BYTE bReadBuf[4] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), true, false);

	return dw;
}

// ȡԤ��ʣ��ʱ��
DWORD CFoFenFTY100::GetWarmUpTime(USHORT *pusMin, USHORT *pusSec)
{
	//assert(NULL!=pusMin && NULL!=pusSec);

	//BYTE bWriteBuf[2] = {0xAE, 0};
	//BYTE bReadBuf[4] = {0, 0, 0, 0};
	//DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	//if (OPA_WR_SUCCEED == dw)
	//{
	//	*pusMin	= bReadBuf[1];
	//	*pusSec	= bReadBuf[2];
	//}
	//else
	//{
	//	// ��������
	//	*pusMin	= 0;
	//	*pusSec	= 0;
	//}
	//return dw;
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ��������
DWORD CFoFenFTY100::GetEnvParams(float *pfET, float *pfRH, float *pfAP)
{
	*pfET = 25;
	*pfRH = 50;
	*pfAP = 100;
	return OPA_WR_EQUIP_RETURE_0X15;
}

// ȡ�汾��Ϣ
DWORD CFoFenFTY100::GetVersion(float *pfVersion)
{
	assert(NULL != pfVersion);

	BYTE bWriteBuf[2] = {0xB1, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (OPA_WR_SUCCEED == dw)
	{
		*pfVersion = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 10.0f;
	}
	else
	{
		// ��������
		*pfVersion = 0.0f;
	}
	return dw;
}

DWORD CFoFenFTY100::GetControlUnitParams(enum EngineStrokeType *const pest, enum RpmMeasurementType *const prmt, enum FilterParamType *const pfpt)
{
	BYTE b1(0), b2(0), b3(0);

	DWORD dw = GetControlUnitParams(&b1, &b2, &b3);


	if(0x01 == b1)
		*pest = FourStroke;
	else if(0x02 == b1)
		*pest = TwoStroke;

	if(0x01 == b2)
		*prmt = Piezoelectric;
	else if(0x02 == b2)
		*prmt = Photoelectric;
	else if(0x03 == b2)
		*prmt = BatteryType;

	if(0x01 == b3)
		*pfpt = FilterMode1;
	else if(0x02 == b3)
		*pfpt = FilterMode2;
	else if(0x03 == b3)
		*pfpt = NoFilter;

	return dw;
}

// ȡ���Ƶ�Ԫ����
DWORD CFoFenFTY100::GetControlUnitParams(BYTE *pbEngineStroke, BYTE *pbRpmMeasureType, BYTE *pbFilterConstant)
{
	assert(NULL!=pbEngineStroke);
	assert(NULL!=pbRpmMeasureType);
	assert(NULL!=pbFilterConstant);

	BYTE bWriteBuf[2] = {0xB3, 0};
	BYTE bReadBuf[5] = {0, 0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 5, true, true);
	if (OPA_WR_SUCCEED == dw)
	{
		*pbEngineStroke = bReadBuf[1];
		*pbRpmMeasureType = bReadBuf[2];
		*pbFilterConstant = bReadBuf[3];
	}
	else
	{
		*pbEngineStroke = 0;
		*pbRpmMeasureType = 0;
		*pbFilterConstant = 0;
	}
	return dw;
}

// ȡ��������
DWORD CFoFenFTY100::GetCorrectedOilTemp(float *pfCorrectedOilTemp)
{
	assert(NULL!=pfCorrectedOilTemp);

	BYTE bWriteBuf[2] = {0xB4, 0};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 2, bReadBuf, 4, true, true);
	if (OPA_WR_SUCCEED == dw)
	{
		*pfCorrectedOilTemp = MAKEWORD(bReadBuf[2], bReadBuf[1]) /10.0f - 273;
    }
	else
	{
		*pfCorrectedOilTemp = 0.0f;
	}
	
	return FALSE;
}

// ȡ�ֶ��������
DWORD CFoFenFTY100::GetTestResultData(float *const pfValueK1, float *const pfValueK2, float *const pfValueK3, short *const psLowRpm, float *const pfValueKAvg)
{
	return OPA_WR_EQUIP_RETURE_0X15;
}

void  CFoFenFTY100::TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CFoFenFTY100 *pf = (CFoFenFTY100 *)dwUser;
	float fN = 0;
	float fK = 0;
	unsigned short usRPM = 0;
	unsigned short usOilTemp = 0;
	pf->GetRealTimeData(&fN, &fK, &usRPM, &usOilTemp);
	
	pf->m_fMaxValueN = fN>pf->m_fMaxValueN ? fN : pf->m_fMaxValueN;
	pf->m_fMaxValueK = fK>pf->m_fMaxValueK ? fK : pf->m_fMaxValueK;

}